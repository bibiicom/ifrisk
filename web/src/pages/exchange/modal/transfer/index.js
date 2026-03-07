import React, { useState, useEffect, useImperativeHandle, forwardRef, createRef, useContext, useRef, useMemo } from 'react'
// import { ethers } from 'ethers'
import Modal from '../../../../component/modal'
import Col from '../../../../component/col'
import Row from '../../../../component/row'
import Input from '../../../../component/input'
import Button from '../../../../component/button'
import Select from '../../../../component/select'
import Message from '../../../../component/message'
import { Transfer } from '../../../../component/icons'
import { isValid } from '../../../../component/utils/typeChecks'
import intl from '../../../../component/intl'
import ComponentContext from '../../../../component/context'
import { formatValue, formatDecimal } from '../../../../utils/format'
import { withdrawal } from '../../../../api/account'
// import { logout } from '../../../../api/authApi'
import { SettlementGroup } from '../../../../constants/symbol'
import './index.less'
// import { SlateElement } from '@wangeditor/editor'

import { Connection, PublicKey, LAMPORTS_PER_SOL, TransactionInstruction, Transaction, SystemProgram, clusterApiUrl } from '@solana/web3.js'
import BN from 'bn.js'

// Token Program 固定地址
const TOKEN_PROGRAM_ID = new PublicKey('TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA')
// Associated Token Program 固定地址
const ASSOCIATED_TOKEN_PROGRAM_ID = new PublicKey('ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL')

// 生成 ATA 地址
function findAssociatedTokenAddress (walletAddress, mint) {
  return PublicKey.findProgramAddressSync(
    [walletAddress.toBuffer(), TOKEN_PROGRAM_ID.toBuffer(), mint.toBuffer()],
    ASSOCIATED_TOKEN_PROGRAM_ID
  )[0]
}

function createTokenTransferInstruction (source, destination, owner, amount) {
  const data = Buffer.alloc(9)
  data.writeUInt8(3, 0) // Transfer
  const bn = new BN(amount.toString())
  data.writeUInt32LE(bn.and(new BN('ffffffff', 16)).toNumber(), 1) // 低 32 位
  data.writeUInt32LE(bn.shrn(32).toNumber(), 5) // 高 32 位
  return new TransactionInstruction({
    keys: [
      { pubkey: source, isSigner: false, isWritable: true },
      { pubkey: destination, isSigner: false, isWritable: true },
      { pubkey: owner, isSigner: true, isWritable: false }
    ],
    programId: TOKEN_PROGRAM_ID,
    data
  })
}

async function getTokenDecimals (connection, mintAddress) {
  const info = await connection.getParsedAccountInfo(mintAddress)
  if (!info.value) throw new Error('Invalid mint address')
  const data = info.value.data
  if (data.parsed?.info?.decimals !== undefined) {
    return data.parsed.info.decimals
  }
  throw new Error('Cannot fetch token decimals')
}

const BlockChainCurrency = {
  // ETH_ETH: '',
  // ETH_USDT: '0xdac17f958d2ee523a2206206994597c13d831ec7',
  // ETH_USDC: '0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48',
  ETH_USDC: '0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48',
  // BASE_ETH: '',
  // BASE_USDT: '0xdac17f958d2ee523a2206206994597c13d831ec7',
  BASE_LINK: '0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48',
  TON_TON: '',
  // TON_USDT: '0xdac17f958d2ee523a2206206994597c13d831ec7',
  // TON_USDC: '0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48',
  ARB_USDT: '0xfd086bc7cd5c481dcc9c85ebe478a1c0b69fcbb9',
  // ARB_USDT: '0x564ac2d78db2eb61e03241b3432775f170a217c3',
  ARB_USDC: '0xaf88d065e77c8cC2239327C5EDb3A432268e5831',
  // ARB_ETH: '',
  ETHW_ETHW: '',
  SOL_SOL: '',
  // SOL_USDT: 'Es9vMFrzaCERmJfrF4H2FYD4KCoNkY11McCe8BenwNYB',
  // SOL_USDC: 'EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v',
  getCurrency (sg) {
    const keys = Object.keys(this).filter(key =>
      key.startsWith(`${sg}_`) && typeof this[key] !== 'function'
    )
    return keys.map(key => key.split('_')[1])
  },
  getETHChainID (sg) {
    switch (sg) {
      case 'ETH': return '0x1'
      case 'ETHW': return '0x2711'
      case 'ARB': return '0xa4b1'
      default: return ''
    }
  },
  getToAddress (sg) {
    switch (sg) {
      case 'SOL':
        return 'E8dLKFia874zyS1pQeRXjdANTKzXL6WEv6YUCEDRUdqn'
      case 'TON':
        return 'UQBMSowXcH2slJ9eDJRPeKpMTQRhvtggYFqifLouGbOUdwks'
      default:
        // return '0x7c224746fD3CC6802A2156d0F318BdC9D4700D48'
        return '0x564ac2d78db2eb61e03241b3432775f170a217c3'
    }
  },
  getChian (sg) {
    switch (sg) {
      case 'SOL':
        return 'SOL'
      case 'TON':
        return 'TON'
      default:
        return 'ETH'
    }
  },
  getAddress (sg, currency) {
    const type = `${sg}_${currency}`
    return this[type]
  }
}

const Content = forwardRef(({ userInfo, symbol, symbols }, ref) => {
  const { theme } = useContext(ComponentContext)
  const [from, setFrom] = useState(SettlementGroup.ARB)
  const [to, setTo] = useState(SettlementGroup.SG1)
  const [rotateCount, setRotateCount] = useState(0)
  const [assets, setAssets] = useState({})
  const [currency, setCurrency] = useState(symbol.ClearCurrency)
  const [currencyList, setCurrencyList] = useState([])
  const [volume, setVolume] = useState('')
  const [inputErrorMessage, setInputErrorMessage] = useState(null)

  useImperativeHandle(ref, () => ({
    verify: () => {
      if (!currency) {
        return false
      }
      if (!volume) {
        setInputErrorMessage(intl.get('transfer_volume_not_be_empty'))
        return false
      }
      // if (from === SettlementGroup.BLOCKCHAIN_WALLET && currency !== 'TBTC') {
      //   Message.warn({ message: intl.get('currency_not_support_blockchain').replace('{currency}', currency) })
      //   return false
      // }
      // if (from === SettlementGroup.BLOCKCHAIN_WALLET && address) {
      //   Message.warn({ message: intl.get('not_deposit_address') })
      //   return false
      // }
      return true
    },
    getParams: () => ({
      MemberID: userInfo.MemberID,
      AccountID: userInfo.MemberID,
      SettlementGroup: from,
      OtherSettlementGroup: to,
      Currency: currency,
      ActionType: '6',
      Amount: +volume
    })
  }))

  useEffect(() => {
    const currencyFrom = BlockChainCurrency.getCurrency(from)
    const currencyTo = BlockChainCurrency.getCurrency(to)
    const newCurrencyList = currencyFrom.length === 0 ? currencyTo : currencyFrom
    setCurrencyList(newCurrencyList)
    if (!newCurrencyList.includes(currency)) {
      setCurrency(newCurrencyList[0])
    }
  }, [from, to, currency])

  const fromOptions = useMemo(() => {
    return SettlementGroup.options(to)
  }, [userInfo?.UserID, to])

  const toOptions = useMemo(() => {
    return SettlementGroup.options(from)
  }, [from])

  // const transferPrecision = symbol.TransferPrecision || 6
  const transferPrecision = 8
  return (
    <Col
      className={`exchange-transfer-content-${theme} exchange-modal-${theme}`}>
      <Row style={{ justifyContent: 'space-between', marginBottom: 24 }}>
        <Col style={{ width: 'calc(50% - 18px)' }}>
          <p className="exchange-transfer-input-title">{intl.get('from')}</p>
          <Select
            value={SettlementGroup.transferText(from)}
            dataSource={fromOptions.filter(item => item.key !== from)}
            trigger="hover"
            onSelected={data => { setFrom(data.key) }}/>
        </Col>
        <Transfer
          style={{ transform: `rotate(${rotateCount * 180}deg)` }}
          className="exchange-transfer-icon"
          onClick={_ => {
            setRotateCount(count => count + 1)
            const tempFrom = from
            setFrom(to)
            setTo(tempFrom)
            // setAssets({})
            setVolume('')
          }}/>
        <Col style={{ width: 'calc(50% - 18px)' }}>
          <p className="exchange-transfer-input-title">{intl.get('to')}</p>
          <Select
            value={SettlementGroup.transferText(to)}
            dataSource={toOptions.filter(item => item.key !== to)}
            trigger="hover"
            onSelected={data => { setTo(data.key) }}/>
        </Col>
      </Row>
      <p className="exchange-transfer-input-title">{intl.get('transfer_currency')}</p>
      <Select
        style={{ marginBottom: 24 }}
        value={currency}
        dataSource={currencyList.filter(item => item.key !== currency)}
        trigger="hover"
        onSelected={currency => {
          // console.log('currency', currency)
          setCurrency(currency)
        }}/>
      <p className="exchange-transfer-input-title">{intl.get('transfer_volume')}</p>
      <Input
        style={{ marginBottom: 24 }}
        valueStyle={{ textAlign: 'right' }}
        prefix={intl.get('volume')}
        suffix={currency}
        precision={transferPrecision}
        min={0}
        max={assets.withdrawable || Number.MAX_SAFE_INTEGER}
        errorMessage={inputErrorMessage}
        value={volume}
        onFocus={_ => { setInputErrorMessage(null) }}
        onChange={setVolume}
      />
      <Row
        className="exchange-modal-row">
        <Row>
          {intl.get('can_transfer_volume')}:
          <span
            style={{ paddingLeft: 8 }}
            className="value">
            {formatDecimal(formatValue(assets, 'withdrawable'), transferPrecision)}&nbsp;{currency}
          </span>
        </Row>
        <Button
          className="exchange-transfer-button"
          size="small"
          onClick={_ => { setVolume(isValid(assets.withdrawable) ? formatDecimal(assets.withdrawable, transferPrecision, 'floor', true) : '') }}>
          {intl.get('all')}
        </Button>
      </Row>
    </Col>
  )
})

Content.displayName = 'TransferContent'

function open ({
  userInfo, symbol, profitLoss, onSuccess, logout, config, isMobile
}) {
  // console.log('symbols', symbols)/
  const contentRef = createRef()
  async function getTokenMetadata (contractAddr) {
    // ABI编码：查询token()函数
    const tokenData = '0x95d89b41'
    // ABI编码：查询decimals()函数
    const decimalsData = '0x313ce567'
    const ethereum = window.ethereum
    try {
      // 调用eth_call查询链上数据（不消耗gas）
      const [tokenHex, decimalsHex] = await Promise.all([
        ethereum.request({
          method: 'eth_call',
          params: [{
            to: contractAddr,
            data: tokenData
          }, 'latest']
        }),
        ethereum.request({
          method: 'eth_call',
          params: [{
            to: contractAddr,
            data: decimalsData
          }, 'latest']
        })
      ])
      const token = hexToString(tokenHex).replace(/[^\x20-\x7E]/g, '').trim()
      const decimals = parseInt(decimalsHex, 16)
      return { token, decimals }
    } catch (error) {
      return { token: '', decimals: 18 }
    }
  }

  // 辅助函数：16进制转字符串（用于解析symbol）
  function hexToString (hex) {
    if (!hex || hex === '0x') return ''
    hex = hex.slice(2)
    let str = ''
    for (let i = 0; i < hex.length; i += 2) {
      const charCode = parseInt(hex.substr(i, 2), 16)
      if (charCode) str += String.fromCharCode(charCode)
    }
    return str
  }

  const request = async () => {
    const params = contentRef.current.getParams()
    if (SettlementGroup.isChain(params.SettlementGroup)) {
      const Chain = BlockChainCurrency.getChian(params.SettlementGroup)
      if (Chain === 'ETH') {
        try {
          const ethereum = window.ethereum
          if (!ethereum) throw new Error('Ethereum wallet not found')
          if (ethereum.selectedAddress.toLowerCase() !== params.MemberID) throw new Error('SelectedAddress not Match')
          const targetChainId = BlockChainCurrency.getETHChainID(params.SettlementGroup)
          const currentChainId = await ethereum.request({ method: 'eth_chainId' })
          if (currentChainId !== targetChainId) {
            try {
              await ethereum.request({
                method: 'wallet_switchEthereumChain',
                params: [{ chainId: targetChainId }]
              })
            } catch (switchError) {
              Message.error({ message: intl.get('failed_to_switch_chain') })
              return
            }
          }
          const transactionParams = {
            from: ethereum.selectedAddress,
            chainId: targetChainId
          }
          const contractAddress = BlockChainCurrency.getAddress(params.SettlementGroup, params.Currency)
          // 如果是合约代币（如USDC），需要构造transfer方法的data
          if (contractAddress) {
            // 计算金额（转为整数，避免精度问题）
            const { token, decimals } = await getTokenMetadata(contractAddress)
            // console.log('token', token)
            // console.log('params', params)
            // if (params.Currency !== token) {
            //   Message.error({ message: `${intl.get('transfer_failed')}: token[${token}] not equal to currency[${params.Currency}]` })
            //   return
            // }
            const amount = Math.floor(params.Amount * (10 ** decimals))
            const valueHex = `0x${amount.toString(16)}`
            transactionParams.to = contractAddress
            transactionParams.value = '0x0'
            // console.log('params', params)
            const toHex = BlockChainCurrency.getToAddress(params.SettlementGroup).slice(2).padStart(64, '0') // 接收地址转64位16进制
            const amountHex = amount.toString(16).padStart(64, '0') // 金额转64位16进制
            transactionParams.data = `0xa9059cbb${toHex}${amountHex}` // ERC20 transfer签名
          } else {
            const amount = Math.floor(params.Amount * (10 ** 18))
            const valueHex = `0x${amount.toString(16)}`
            transactionParams.to = params.getToAddress(params.OtherSettlementGroup)
            transactionParams.value = valueHex
          }
          await ethereum.request({
            method: 'eth_sendTransaction',
            params: [transactionParams]
          })
          Message.ok({ message: `${intl.get('transfer_success')} ${params.Currency} : ${params.Amount}` })
        } catch (e) {
          console.log('error', e)
          Message.error({ message: `${intl.get('transfer_failed')}${(e.err && e.err.message) || e.message ? `, ${e.message || e.err.message}` : ''}` })
        }
      } else if (Chain === 'SOL') {
        try {
          const provider = window.solana || window.okxwallet?.solana || window.tp
          if (!provider) throw new Error('Solana wallet not found')

          const resp = await provider.connect()
          const fromPublicKey = new PublicKey(resp.publicKey.toString())
          console.log('fromPublicKey:', fromPublicKey.toBase58())
          const toPublicKey = new PublicKey(BlockChainCurrency.getToAddress(params.SettlementGroup))
          console.log('toPublicKey:', toPublicKey.toBase58())
          console.log('connection', 'connection0')
          const connection = new Connection(clusterApiUrl('mainnet-beta'), 'confirmed')
          // const connection = new Connection('https://api.mainnet-beta.solana.com', 'confirmed')
          console.log('connection', 'connection1')
          console.log('connection', connection)
          let signature
          const contractAddress = BlockChainCurrency.getAddress(params.SettlementGroup, params.Currency)
          console.log('contractAddress', contractAddress)
          if (contractAddress) {
            const mintPubkey = new PublicKey(BlockChainCurrency.getAddress(params.SettlementGroup, params.Currency))
            const fromTokenAccount = findAssociatedTokenAddress(fromPublicKey, mintPubkey)
            const toTokenAccount = findAssociatedTokenAddress(toPublicKey, mintPubkey)
            const decimals = await getTokenDecimals(connection, mintPubkey)
            const amount = Math.floor(params.Amount * 10 ** decimals)
            const tx = new Transaction().add(
              createTokenTransferInstruction(fromTokenAccount, toTokenAccount, fromPublicKey, amount)
            )
            console.log('provider', provider)
            tx.recentBlockhash = (await connection.getLatestBlockhash()).blockhash
            tx.feePayer = fromPublicKey
            console.log('provider', provider)
            console.log('methods', {
              signTransaction: provider.signTransaction,
              signAndSendTransaction: provider.signAndSendTransaction
            })
            if (provider.signAndSendTransaction) {
              const res = await provider.signAndSendTransaction(tx)
              signature = res.signature || res
            } else if (provider.signTransaction) {
              const signedTx = await provider.signTransaction(tx)
              signature = await connection.sendRawTransaction(signedTx.serialize())
            } else {
              throw new Error('Wallet does not support signTransaction')
            }
          } else {
            const lamports = Math.floor(params.Amount * 1e9)
            console.log('fromPublicKey1', fromPublicKey)
            console.log('toPublicKey1', toPublicKey)
            const transaction = new Transaction().add(
              SystemProgram.transfer({
                fromPubkey: fromPublicKey,
                toPubkey: toPublicKey,
                lamports
              })
            )
            console.log('transaction', transaction)
            transaction.recentBlockhash = (await connection.getLatestBlockhash()).blockhash
            transaction.feePayer = fromPublicKey
            console.log('provider', provider)
            console.log('methods', {
              signTransaction: provider.signTransaction,
              signAndSendTransaction: provider.signAndSendTransaction
            })

            if (provider.signAndSendTransaction) {
              const res = await provider.signAndSendTransaction(transaction)
              signature = res.signature || res
            } else if (provider.signTransaction) {
              const signedTx = await provider.signTransaction(transaction)
              signature = await connection.sendRawTransaction(signedTx.serialize())
            }
          }
          await connection.confirmTransaction(signature, 'confirmed')
          Message.ok({ message: `${intl.get('transfer_success')} ${params.Currency} : ${params.Amount}` })
          return signature
        } catch (e) {
          console.log('error', e)
          Message.error({ message: `${intl.get('transfer_failed')}${(e.err && e.err.message) || e.message ? `, ${e.message || e.err.message}` : ''}` })
        }
      }
    } else {
      try {
        const message = `${params.Currency}:${params.Amount}`
        const ethereum = window.ethereum
        params.Remark = await ethereum.request({
          method: 'personal_sign',
          params: [message, params.MemberID]
        })
        // console.log('params', params)
        await withdrawal(params)
        onSuccess && onSuccess()
        Message.ok({ message: intl.get('transfer_success'), config })
      } catch (e) {
        Message.error({ message: `${intl.get('transfer_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config })
      }
    }
  }

  Modal.open({
    className: isMobile ? 'exchange-modal-mobile' : '',
    title: intl.get('transfer'),
    content: <Content ref={contentRef} userInfo={userInfo} symbol={symbol} profitLoss={profitLoss} />,
    width: isMobile ? '100%' : 500,
    buttons: [
      {
        children: intl.get('cancel'),
        type: 'tertiary',
        text: true
      },
      {
        children: intl.get('confirm'),
        onClick: () => {
          if (!contentRef.current.verify()) {
            return true
          }
          request()
        }
      }
    ],
    config
  })
}

const TransferModal = { open }

export default TransferModal
