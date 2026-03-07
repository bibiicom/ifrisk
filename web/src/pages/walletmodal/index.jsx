import React, { useContext } from 'react'
import { Drawer, Collapse, List, App } from 'antd'
import { loginForWallet, getNonce } from '../../api/public'
import { Status } from '../../constants/auth'
import RootContext from '../rootContext'
import Message from '../../component/message'
import intl from '../../component/intl'
// import {
//   useTonWallet,
//   useTonConnectUI
// } from "@tonconnect/ui-react"

const isMobileBrowser = () => {
  const ua = navigator.userAgent || navigator.vendor || window.opera
  return /android|iphone|ipad|ipod/i.test(ua)
}

// 钱包检测配置
const WALLET_CONFIG = {
  'OKXWallet': {
    detector: () => window.okxwallet,
    downloadUrl: 'https://www.okx.com/web3',
    displayName: 'OKX Wallet'
  },
  'MetaMask': {
    detector: () => window.ethereum?.isMetaMask,
    downloadUrl: 'https://metamask.io/download/',
    displayName: 'MetaMask'
  },
  'TokenPocket': {
    detector: () => window.tokenpocket || window.ethereum?.isTokenPocket,
    downloadUrl: 'https://www.tokenpocket.pro/en/download/app',
    displayName: 'TokenPocket'
  },
  'Trust Wallet': {
    detector: () => window.trustwallet || window.ethereum?.isTrust,
    downloadUrl: 'https://trustwallet.com/browser-extension',
    displayName: 'Trust Wallet'
  },
  'Bitget Wallet': {
    detector: () => window.bitkeep || window.isBitKeep,
    downloadUrl: 'https://web3.bitget.com/en/wallet-download',
    displayName: 'Bitget Wallet'
  },
  'Coinbase': {
    detector: () => window.coinbaseWalletExtension || window.ethereum?.isCoinbaseWallet,
    downloadUrl: 'https://www.coinbase.com/wallet/downloads',
    displayName: 'Coinbase Wallet'
  },
  'Phantom': {
    detector: () => window.phantom?.solana,
    downloadUrl: 'https://phantom.app/download',
    displayName: 'Phantom'
  }
}

// 检测钱包是否已安装
const checkWalletInstalled = (walletName) => {
  // 移动端不检测
  if (isMobileBrowser()) {
    return true
  }

  const config = WALLET_CONFIG[walletName]
  if (!config) {
    return true // 未配置的钱包默认通过
  }

  return config.detector()
}

const { Panel } = Collapse

// 获取当前主题的样式
const getThemeStyles = () => {
  const theme = document.documentElement.getAttribute('data-theme')
  const isLight = theme === 'light'

  return {
    bgColor: isLight ? '#ffffff' : '#222',
    textColor: isLight ? '#1c1c1e' : '#fff',
    borderColor: isLight ? '#e8e8e8' : '#444',
    hoverBgColor: isLight ? '#f5f5f5' : '#555',
  }
}

const WalletModal = ({ open, onClose }) => {
  const { setAuthStatus } = useContext(RootContext)
  const { modal } = App.useApp()
  const [themeStyles, setThemeStyles] = React.useState(getThemeStyles())

  // 监听主题变化
  React.useEffect(() => {
    const updateTheme = () => {
      setThemeStyles(getThemeStyles())
    }

    // 初始化
    updateTheme()

    // 监听 data-theme 属性变化
    const observer = new MutationObserver((mutations) => {
      mutations.forEach((mutation) => {
        if (mutation.attributeName === 'data-theme') {
          updateTheme()
        }
      })
    })

    observer.observe(document.documentElement, { attributes: true })

    return () => observer.disconnect()
  }, [open])

  // 显示未安装提示（使用 App.useApp 提供的 modal，继承全局 token）
  const showInstallPrompt = (walletName) => {
    const config = WALLET_CONFIG[walletName]
    if (!config) return

    modal.confirm({
      title: '未检测到钱包插件',
      content: (
        <div>
          <p>您还未安装 <strong>{config.displayName}</strong> 浏览器插件</p>
          <p>请先安装该钱包插件后再进行连接</p>
        </div>
      ),
      okText: '去安装',
      cancelText: '取消',
      onOk: () => {
        window.open(config.downloadUrl, '_blank')
      }
    })
  }

  /** ---------------- EVM 示例 ---------------- **/
  const OKXWalletEVM = async () => {
    // 检测钱包是否安装
    if (!checkWalletInstalled('OKXWallet')) {
      showInstallPrompt('OKXWallet')
      return
    }

    const okxWallet = window.okxwallet
    if (okxWallet) {
      try {
        const accounts = await okxWallet.request({ method: 'eth_requestAccounts' })
        const address = accounts[0].toLowerCase()

        const result = await getNonce(address)

        const signature = await okxWallet.request({
          method: 'personal_sign',
          params: [result[0].data.Nonce, address]
        })

        await loginForWallet(address, signature)
        setAuthStatus(Status.LOGIN)
        window.location.reload()
      } catch (e) {
        console.error('Error:', e)
        Message.error(`${intl.get('wallet_login_failed')}, ${e.message || e.err?.message}`)
      }
      onClose()
    } else if (/android|iphone|ipad|ipod/i.test(navigator.userAgent)) {
      const currentUrl = encodeURIComponent(window.location.href)
      const deeplink = `okex://wallet/dapp/details?dappUrl=${currentUrl}`
      window.location.href = deeplink
    } else {
      window.open('https://web3.okx.com', '_blank')
      return
    }
  }

  // 通用钱包连接处理函数
  const handleWalletConnect = async (walletName, connectLogic) => {
    // 检测钱包是否安装
    if (!checkWalletInstalled(walletName)) {
      showInstallPrompt(walletName)
      return
    }

    // 执行连接逻辑
    if (connectLogic) {
      await connectLogic()
    } else {
      // 默认提示功能开发中（使用自定义 Message.warn，而不是不存在的 Message.info）
      Message.warn({ message: `${WALLET_CONFIG[walletName]?.displayName || walletName} 连接功能开发中` })
    }

    onClose()
  }

  const TokenPocketEVM = () => handleWalletConnect('TokenPocket')
  const MetaMaskEVM = () => handleWalletConnect('MetaMask')
  const TrustWalletEVM = () => handleWalletConnect('Trust Wallet')
  const BitgetWalletEVM = () => handleWalletConnect('Bitget Wallet')
  const CoinbaseWalletEVM = () => handleWalletConnect('Coinbase')
  // const PhantomSOL = () => handleWalletConnect('Phantom')

  /** ---------------- TON 钱包 ---------------- **/
  // const address = useTonAddress()          // 连接后的钱包地址
  // const wallet = useTonWallet()            // 钱包对象（未连接时是 null）
  // const [tonConnectUI] = useTonConnectUI() // UI 控制对象

  // const handleTonWallet = async () => {
  //   onClose()
  //   if (!wallet) {
  //     // 如果未连接，打开钱包选择 UI
  //     tonConnectUI.openModal()
  //     return
  //   }

  //   // 已连接，演示发送交易
  //   const tx = {
  //     validUntil: Math.floor(Date.now() / 1000) + 600,
  //     messages: [
  //       {
  //         address: "EQCxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", // 目标地址
  //         amount: "20000000" // 0.02 TON (nanotons)
  //       }
  //     ]
  //   }

  //   try {
  //     await tonConnectUI.sendTransaction(tx)
  //     Message.success("交易已提交，请在钱包确认")
  //   } catch (err) {
  //     console.error(err)
  //     Message.error("交易失败或用户取消")
  //   }

  //   onClose()
  // }

  /** ---------------- 钱包分组 ---------------- **/
  const walletGroups = [
    {
      name: 'EVM',
      key: 'evm',
      children: [
        { name: 'OKXWallet', icon: '/images/okxwallet.svg', onClick: OKXWalletEVM },
        { name: 'TokenPocket', icon: '/images/tp.svg', onClick: TokenPocketEVM },
        { name: 'Trust Wallet', icon: '/images/trust.png', onClick: TrustWalletEVM },
        { name: 'Bitget Wallet', icon: '/images/bitgate.png', onClick: BitgetWalletEVM },
        { name: 'MetaMask', icon: '/images/metamask.svg', onClick: MetaMaskEVM },
        { name: 'Coinbase', icon: '/images/coinbase.svg', onClick: CoinbaseWalletEVM }
      ]
    },
    // {
    //   name: 'TON',
    //   key: 'ton',
    //   children: [
    //     { name: 'Ton Wallet', icon: '/images/wallet.svg', onClick: handleTonWallet },
    //     { name: 'OKXWallet', icon: '/images/okxwallet.svg', onClick: OKXWalletEVM },
    //     { name: 'TokenPocket', icon: '/images/tp.svg', onClick: TokenPocketEVM },
    //     { name: 'Trust Wallet', icon: '/images/trust.png', onClick: TrustWalletEVM },
    //     { name: 'Bitgate Wallet', icon: '/images/bitgate.png', onClick: BitgetWalletEVM }
    //   ]
    // },
    // {
    //   name: 'SOL',
    //   key: 'sol',
    //   children: [
    //     { name: 'Phantom', icon: '/images/phantom.png', onClick: PhantomSOL },
    //     { name: 'OKXWallet', icon: '/images/okxwallet.svg', onClick: OKXWalletEVM },
    //     { name: 'Trust Wallet', icon: '/images/trust.png', onClick: TrustWalletEVM },
    //     { name: 'Bitgate Wallet', icon: '/images/bitgate.png', onClick: BitgetWalletEVM }
    //   ]
    // },
    // {
    //   name: 'TRON',
    //   key: 'tron',
    //   children: [
    //     { name: 'OKXWallet', icon: '/images/okxwallet.svg', onClick: OKXWalletEVM },
    //     { name: 'TokenPocket', icon: '/images/tp.svg', onClick: TokenPocketEVM },
    //     { name: 'Trust Wallet', icon: '/images/trust.png', onClick: TrustWalletEVM },
    //     { name: 'Bitgate Wallet', icon: '/images/bitgate.png', onClick: BitgetWalletEVM }
    //   ]
    // }
  ]

  return (
    <Drawer
      title={<span style={{ color: themeStyles.textColor }}>选择钱包</span>}
      placement="right"
      onClose={onClose}
      open={open}
      width={280}
      bodyStyle={{ backgroundColor: themeStyles.bgColor, color: themeStyles.textColor, padding: 0 }}
      headerStyle={{ backgroundColor: themeStyles.bgColor, color: themeStyles.textColor, borderBottom: `1px solid ${themeStyles.borderColor}` }}
    >
      <Collapse
        accordion
        defaultActiveKey={['evm']}
        bordered={false}
        style={{ backgroundColor: themeStyles.bgColor, color: themeStyles.textColor }}
      >
        {walletGroups.map(group => (
          <Panel
            header={
              <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between' }}>
                <span style={{ color: themeStyles.textColor, fontWeight: 'bold' }}>
                  {group.name}
                </span>
                <div style={{ display: 'flex', gap: 6 }}>
                  {group.children.map(wallet => (
                    <img
                      key={wallet.name}
                      src={wallet.icon}
                      alt={wallet.name}
                      style={{ width: 16, height: 16, borderRadius: 3 }}
                    />
                  ))}
                </div>
              </div>
            }
            key={group.key}
            style={{ backgroundColor: themeStyles.bgColor, borderBottom: `1px solid ${themeStyles.borderColor}` }}
          >
            <List
              itemLayout="horizontal"
              dataSource={group.children}
              renderItem={wallet => (
                <List.Item
                  key={wallet.name}
                  style={{
                    cursor: 'pointer',
                    padding: '8px 16px',
                    backgroundColor: themeStyles.bgColor,
                    transition: 'background-color 0.2s ease'
                  }}
                  onClick={wallet.onClick}
                  onMouseEnter={e => { e.currentTarget.style.backgroundColor = themeStyles.hoverBgColor }}
                  onMouseLeave={e => { e.currentTarget.style.backgroundColor = themeStyles.bgColor }}
                >
                  <img
                    src={wallet.icon}
                    alt={wallet.name}
                    style={{ width: 28, height: 28, marginLeft: 16 }}
                  />
                  <span style={{ color: themeStyles.textColor, fontSize: 15, marginLeft: 8 }}>
                    {wallet.name}
                  </span>
                </List.Item>
              )}
            />
          </Panel>
        ))}
      </Collapse>
    </Drawer>
  )
}

export default WalletModal
