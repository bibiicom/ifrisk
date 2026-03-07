import React, { useState, useEffect, useContext, useMemo } from 'react'
import { Select, Input, App } from 'antd'
import { SwapRightOutlined } from '@ant-design/icons'
import intl from '@/component/intl'
import RootContext from '../../../rootContext'
import { SettlementGroup } from '../../../../constants/symbol'
import { withdrawal } from '../../../../api/account'
import CommonModal from '../../../../component/commonModal'
import styles from './index.module.less'

// 复用原有的链上货币配置
const BlockChainCurrency = {
    ARB_USDT: '0xfd086bc7cd5c481dcc9c85ebe478a1c0b69fcbb9',
    ARB_USDC: '0xaf88d065e77c8cC2239327C5EDb3A432268e5831',
    TON_TON: '',
    SOL_SOL: '',
    ETH_USDC: '0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48',
    getCurrency(sg) {
        const keys = Object.keys(this).filter(key =>
            key.startsWith(`${sg}_`) && typeof this[key] !== 'function'
        )
        return keys.map(key => key.split('_')[1])
    },
    getETHChainID(sg) {
        switch (sg) {
            case 'ETH': return '0x1'
            case 'ETHW': return '0x2711'
            case 'ARB': return '0xa4b1'
            default: return ''
        }
    },
    getToAddress(sg) {
        switch (sg) {
            case 'SOL': return 'E8dLKFia874zyS1pQeRXjdANTKzXL6WEv6YUCEDRUdqn'
            case 'TON': return 'UQBMSowXcH2slJ9eDJRPeKpMTQRhvtggYFqifLouGbOUdwks'
            default: return '0x564ac2d78db2eb61e03241b3432775f170a217c3'
        }
    },
    getChain(sg) {
        switch (sg) {
            case 'SOL': return 'SOL'
            case 'TON': return 'TON'
            default: return 'ETH'
        }
    },
    getAddress(sg, currency) {
        const type = `${sg}_${currency}`
        return this[type]
    }
}

// mode: 'deposit' 充值(链上->交易), 'withdraw' 提现(交易->链上)
const TransferModal = ({ visible, onCancel, mode = 'deposit' }) => {
    const { message } = App.useApp()
    const { userInfo } = useContext(RootContext)

    // 根据 mode 设置初始方向
    const [from, setFrom] = useState(mode === 'deposit' ? SettlementGroup.ARB : SettlementGroup.SG1)
    const [to, setTo] = useState(mode === 'deposit' ? SettlementGroup.SG1 : SettlementGroup.ARB)
    const [currency, setCurrency] = useState('USDC')
    const [volume, setVolume] = useState('')
    const [loading, setLoading] = useState(false)

    // 当 mode 变化时重置方向
    useEffect(() => {
        if (visible) {
            if (mode === 'deposit') {
                setFrom(SettlementGroup.ARB)
                setTo(SettlementGroup.SG1)
            } else {
                setFrom(SettlementGroup.SG1)
                setTo(SettlementGroup.ARB)
            }
            setVolume('')
        }
    }, [visible, mode])

    // 可用币种列表
    const currencyList = useMemo(() => {
        const currencyFrom = BlockChainCurrency.getCurrency(from)
        const currencyTo = BlockChainCurrency.getCurrency(to)
        return currencyFrom.length === 0 ? currencyTo : currencyFrom
    }, [from, to])

    // 确保当前币种在列表中
    useEffect(() => {
        if (currencyList.length > 0 && !currencyList.includes(currency)) {
            setCurrency(currencyList[0])
        }
    }, [currencyList, currency])

    // from/to 选项
    const fromOptions = useMemo(() => {
        return SettlementGroup.options(to).filter(item => item.key !== from)
    }, [to, from])

    const toOptions = useMemo(() => {
        return SettlementGroup.options(from).filter(item => item.key !== to)
    }, [from, to])


    // 获取 token 元数据
    const getTokenMetadata = async (contractAddr) => {
        const decimalsData = '0x313ce567'
        const ethereum = window.ethereum
        try {
            const decimalsHex = await ethereum.request({
                method: 'eth_call',
                params: [{ to: contractAddr, data: decimalsData }, 'latest']
            })
            const decimals = parseInt(decimalsHex, 16)
            return { decimals }
        } catch (error) {
            return { decimals: 18 }
        }
    }

    // 提交划转
    const handleSubmit = async () => {
        if (!volume || Number(volume) <= 0) {
            message.error(intl.get('wallet_assets_transfer_error_amount'))
            return
        }
        if (!userInfo?.MemberID) {
            message.error(intl.get('wallet_assets_transfer_error_login'))
            return
        }

        setLoading(true)
        const params = {
            MemberID: userInfo.MemberID,
            AccountID: userInfo.MemberID,
            SettlementGroup: from,
            OtherSettlementGroup: to,
            Currency: currency,
            ActionType: '6',
            Amount: Number(volume)
        }

        try {
            if (SettlementGroup.isChain(from)) {
                // 从链上转入：需要调用钱包
                const Chain = BlockChainCurrency.getChain(from)
                if (Chain === 'ETH') {
                    const ethereum = window.ethereum
                    if (!ethereum) throw new Error(intl.get('wallet_assets_transfer_error_wallet'))
                    if (ethereum.selectedAddress?.toLowerCase() !== params.MemberID?.toLowerCase()) {
                        throw new Error(intl.get('wallet_assets_transfer_error_address'))
                    }

                    const targetChainId = BlockChainCurrency.getETHChainID(from)
                    const currentChainId = await ethereum.request({ method: 'eth_chainId' })

                    if (currentChainId !== targetChainId) {
                        await ethereum.request({
                            method: 'wallet_switchEthereumChain',
                            params: [{ chainId: targetChainId }]
                        })
                    }

                    const transactionParams = {
                        from: ethereum.selectedAddress,
                        chainId: targetChainId
                    }

                    const contractAddress = BlockChainCurrency.getAddress(from, currency)
                    if (contractAddress) {
                        const { decimals } = await getTokenMetadata(contractAddress)
                        const amount = Math.floor(params.Amount * (10 ** decimals))
                        const toHex = BlockChainCurrency.getToAddress(from).slice(2).padStart(64, '0')
                        const amountHex = amount.toString(16).padStart(64, '0')
                        transactionParams.to = contractAddress
                        transactionParams.value = '0x0'
                        transactionParams.data = `0xa9059cbb${toHex}${amountHex}`
                    } else {
                        const amount = Math.floor(params.Amount * (10 ** 18))
                        transactionParams.to = BlockChainCurrency.getToAddress(from)
                        transactionParams.value = `0x${amount.toString(16)}`
                    }

                    await ethereum.request({
                        method: 'eth_sendTransaction',
                        params: [transactionParams]
                    })
                    message.success(`${intl.get('wallet_assets_transfer_success')} ${currency}: ${params.Amount}`)
                }
            } else {
                // 从交易账户转出：需要签名
                const ethereum = window.ethereum
                const msgToSign = `${params.Currency}:${params.Amount}`
                params.Remark = await ethereum.request({
                    method: 'personal_sign',
                    params: [msgToSign, params.MemberID]
                })
                await withdrawal(params)
                message.success(intl.get('wallet_assets_transfer_success'))
            }
            onCancel()
        } catch (e) {
            console.error('Transfer error:', e)
            message.error(`${intl.get('wallet_assets_transfer_failed')}${e.message ? `: ${e.message}` : ''}`)
        } finally {
            setLoading(false)
        }
    }

    const handleAfterClose = () => {
        setVolume('')
    }

    const modalStyles = CommonModal.styles

    return (
        <CommonModal
            title={mode === 'deposit' ? intl.get('wallet_assets_deposit') : intl.get('wallet_assets_withdraw')}
            open={visible}
            onCancel={onCancel}
            afterClose={handleAfterClose}
            footer={null}
            width={480}
        >
            <div className={styles.content}>
                <div className={styles.directionRow}>
                    <div className={styles.directionItem}>
                        <Select
                            className={styles.select}
                            value={from}
                            onChange={setFrom}
                            options={[
                                { value: from, label: SettlementGroup.transferText(from) },
                                ...fromOptions.map(item => ({ value: item.key, label: item.text }))
                            ]}

                        />
                    </div>
                    <div className={styles.swapBtn}>
                        <SwapRightOutlined style={{ fontSize: '30px' }} />
                    </div>
                    <div className={styles.directionItem}>
                        <Select
                            className={styles.select}
                            value={to}
                            onChange={setTo}
                            options={[
                                { value: to, label: SettlementGroup.transferText(to) },
                                ...toOptions.map(item => ({ value: item.key, label: item.text }))
                            ]}
                        />
                    </div>
                </div>

                <div className={styles.formItem}>
                    <div className={styles.label}>{intl.get('wallet_assets_transfer_currency')}</div>
                    <Select
                        className={styles.select}
                        value={currency}
                        onChange={setCurrency}
                        options={currencyList.map(c => ({ value: c, label: c }))}
                    />
                </div>

                <div className={styles.formItem}>
                    <div className={styles.label}>{intl.get('wallet_assets_transfer_amount')}</div>
                    <Input
                        className={styles.input}
                        placeholder={intl.get('wallet_assets_transfer_amount_placeholder')}
                        suffix={currency}
                        value={volume}
                        onChange={(e) => {
                            const val = e.target.value
                            if (/^\d*\.?\d*$/.test(val)) {
                                setVolume(val)
                            }
                        }}
                    />

                </div>

                <div className={modalStyles.footer}>
                    <button className={modalStyles.cancelBtn} onClick={onCancel}>{intl.get('wallet_assets_transfer_cancel')}</button>
                    <button
                        className={modalStyles.confirmBtn}
                        onClick={handleSubmit}
                        disabled={loading}
                    >
                        {loading ? intl.get('wallet_assets_transfer_processing') : intl.get('wallet_assets_transfer_confirm')}
                    </button>
                </div>
            </div>
        </CommonModal>
    )
}

export default TransferModal
