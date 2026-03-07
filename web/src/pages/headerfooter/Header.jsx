import React, { lazy, Suspense, useContext, useMemo, useState, useRef, useEffect } from 'react'
import { CopyOutlined,  } from '@ant-design/icons'

import { useNavigate, Link, NavLink, useLocation } from 'react-router-dom'

import Row from '../../component/row'
import Col from '../../component/col'
import Message from '../../component/message'
import Popover from '../../component/popover'
import { ComponentContextProvider } from '../../component/context'

import RootContext from '../rootContext'

import { Theme } from '../../constants/theme'

import intl from '../../component/intl'

import { Status } from '../../constants/auth'

import { isIp } from '../../utils/ip'
import { TOKEN, getStorage, SYMBOL_KEY, removeStorage } from '../../utils/storage'
import logoPng from './logo.png'
import WalletModal from '../walletmodal'
import { getPubilc } from '../../network/pubilcHttp'
import ThemeSwitch from '../../component/ThemeSwitch'

import {
  HOME_PATH,
  EXCHANGE,
  ASSETS,
  MY_INFO,
  FINANCIAL,
  FINANCIAL2,
  ORDERS,
  MARKET,
  LAUNCHPAD,
  LAUNCHPAD2,
  NFT_ENTRY,
  BUYBACK,
  WALLET_ASSETS2,
  // INBOUND,
  OUTBOUND,
  NEWS,
  ANNOUNCEMENT,
  VAULT,
  RANKING,
} from '../routes'

import { logout } from '../../api/public'

import './header.less'
import Button from '../../component/button'
const SettingModal = lazy(() => import('./setting'))

const Header = (() => {
  const header = useRef()

  const navigate = useNavigate()
  const { pathname } = useLocation()
  const { config, authStatus, userInfo, setAuthStatus, setUserInfo } = useContext(RootContext)
  const [settingModalVisible, setSettingModalVisible] = useState(false)
  const [drawerVisible, setDrawerVisible] = useState(false)
  const [miniRightVisible, setMiniRightVisible] = useState(false)

  const [depositMenuSpread, setDepositMenuSpread] = useState(false)
  const [exchangeMenuSpread, setExchangeMenuSpread] = useState(false)
  const [marketInfoMenuSpread, setMarketInfoMenuSpread] = useState(false)
  const [communityMenuSpread, setCommunityMenuSpread] = useState(false)
  const [positionMenuSpread, setPositionMenuSpread] = useState(false)
  const [informationMenuSpread, setInformationMenuSpread] = useState(false)
  const [userMenuSpread, setUserMenuSpread] = useState(false)
  const [langMenuSpread, setLangSpread] = useState(false)
  const [modalOpen, setModalOpen] = useState(false)

  // console.log('useinfo', userInfo)

  useEffect(() => {
    console.log(authStatus, 'headet')
  }, [authStatus])

  // 点击外部关闭用户菜单
  useEffect(() => {
    const handleClickOutside = (event) => {
      if (userMenuSpread && !event.target.closest('.header-content-menu')) {
        setUserMenuSpread(false)
      }
    }

    if (userMenuSpread) {
      document.addEventListener('click', handleClickOutside)
    }

    return () => {
      document.removeEventListener('click', handleClickOutside)
    }
  }, [userMenuSpread])

  // 点击外部关闭资讯菜单
  useEffect(() => {
    const handleClickOutside = (event) => {
      if (informationMenuSpread && !event.target.closest('.header-content-menu')) {
        setInformationMenuSpread(false)
      }
    }

    if (informationMenuSpread) {
      document.addEventListener('click', handleClickOutside)
    }

    return () => {
      document.removeEventListener('click', handleClickOutside)
    }
  }, [informationMenuSpread])

  const theme = useMemo(() => {
    return pathname.match(EXCHANGE) ? config.theme : Theme.DARK
  }, [pathname, config.theme])

  const jumpPage = (productGroup) => {
    try {
      const symbol = JSON.parse(getStorage(SYMBOL_KEY))
      if (symbol?.ProductGroup !== productGroup) {
        removeStorage(SYMBOL_KEY)
      }
    } catch (e) { }
    navigate(EXCHANGE, { state: { productGroup } })
  }

  const copyText = async (text) => {
    if (!text) return
    try {
      if (navigator?.clipboard?.writeText) {
        await navigator.clipboard.writeText(text)
      } else {
        const textarea = document.createElement('textarea')
        textarea.value = text
        textarea.setAttribute('readonly', '')
        textarea.style.position = 'fixed'
        textarea.style.top = '-9999px'
        document.body.appendChild(textarea)
        textarea.select()
        document.execCommand('copy')
        document.body.removeChild(textarea)
      }
      Message.ok({ message: '已复制' })
    } catch (e) {
      Message.error({ message: '复制失败' })
    }
  }

  const renderLeftLinks = () => {
    return (
      <>
        <NavLink
          to={MARKET}
          className="header-content-link">
          {intl.get('market')}
        </NavLink>
        <NavLink
          to={RANKING}
          className="header-content-link">
          {intl.get('ranking')}
        </NavLink>
        <NavLink
          to={EXCHANGE}
          className="header-content-link">
          {intl.get('trade')}
        </NavLink>
        
        <NavLink
          to={LAUNCHPAD}
          className="header-content-link">
          {intl.get('launchpad')}
        </NavLink>
        <NavLink
          to={BUYBACK}
          className="header-content-link">
          {intl.get('buyback')}
        </NavLink>
        {/* <NavLink
          to={INBOUND}
          className="header-content-link">
          {intl.get('inbound_storage')}
        </NavLink> */}
        <NavLink
          to={OUTBOUND}
          className="header-content-link">
          {intl.get('outbound_storage')}
        </NavLink>
        <NavLink
          to={VAULT}
          className="header-content-link">
          {intl.get('vault')}
        </NavLink>
        <Row className="header-content-menu" onClick={() => setInformationMenuSpread(!informationMenuSpread)}>
          {intl.get('information')}
          <svg viewBox="0 0 1024 1024" className={`header-content-menu-arrow ${informationMenuSpread ? 'spread' : ''}`} style={{ marginLeft: 5 }}>
            <path d="M948.560332 281.179984c-13.765515-13.833053-36.127825-13.833053-49.89334 0L511.991302 668.591431 125.313565 281.179984c-13.763468-13.798261-36.093033-13.798261-49.856501 0-13.799284 13.798261-13.799284 36.161594 0 49.993624l410.857439 411.674037c7.067976 7.085372 16.402575 10.521634 25.675776 10.331299 9.274224 0.191358 18.608823-3.245927 25.677822-10.331299l410.891208-411.708829c6.863315-6.89913 10.331299-15.940041 10.331299-24.979928S955.423647 288.078091 948.560332 281.179984z" />
          </svg>
          <ul className={`header-content-menu-popover ${informationMenuSpread ? 'show' : ''}`}>
            <li>
              <svg viewBox="0 0 1024 1024" width="30" height="30">
                <path d="M928 160H96c-17.7 0-32 14.3-32 32v640c0 17.7 14.3 32 32 32h832c17.7 0 32-14.3 32-32V192c0-17.7-14.3-32-32-32z m-40 632H136V232h752v560zM610.3 476h123.4c1.3 0 2.3-3.6 2.3-8v-48c0-4.4-1-8-2.3-8H610.3c-1.3 0-2.3 3.6-2.3 8v48c0 4.4 1 8 2.3 8z m0 144h123.4c1.3 0 2.3-3.6 2.3-8v-48c0-4.4-1-8-2.3-8H610.3c-1.3 0-2.3 3.6-2.3 8v48c0 4.4 1 8 2.3 8zM224 673h43.9c4.2 0 7.6-3.3 7.9-7.5 3.8-50.5 46-90.5 97.2-90.5s93.4 40 97.2 90.5c0.3 4.2 3.7 7.5 7.9 7.5H522a8 8 0 0 0 8-8.4c-2.8-53.3-32-99.7-74.6-126.1 18.1-19.9 29.1-46.4 29.1-75.5 0-61.9-49.9-112-111.4-112s-111.4 50.1-111.4 112c0 29.1 11 55.5 29.1 75.5-42.7 26.5-71.8 72.8-74.6 126.1-0.4 4.6 3.2 8.4 7.8 8.4z m149-262c28.5 0 51.7 23.3 51.7 52s-23.2 52-51.7 52-51.7-23.3-51.7-52 23.2-52 51.7-52z" fill="currentColor"></path>
              </svg>
              <Link className="header-content-menu-link" to={NEWS}>{intl.get('announcement_news')}</Link>
            </li>
            <li>
              <svg viewBox="0 0 1024 1024" width="30" height="30">
                <path d="M512 64C264.6 64 64 264.6 64 512s200.6 448 448 448 448-200.6 448-448S759.4 64 512 64z m0 820c-205.4 0-372-166.6-372-372s166.6-372 372-372 372 166.6 372 372-166.6 372-372 372z" fill="currentColor"></path>
                <path d="M464 336a48 48 0 1 0 96 0 48 48 0 1 0-96 0z m72 112h-48c-4.4 0-8 3.6-8 8v272c0 4.4 3.6 8 8 8h48c4.4 0 8-3.6 8-8V456c0-4.4-3.6-8-8-8z" fill="currentColor"></path>
              </svg>
              <Link className="header-content-menu-link" to={ANNOUNCEMENT}>{intl.get('announcement_notice')}</Link>
            </li>
          </ul>
        </Row>
        {/* {!isIp() && <a href="https://github.com/bibiicom" target="_blank" rel="noopener noreferrer" className="header-content-link">Github</a>} */}
        {/* <NavLink
          to={FAUCET}
          className="header-content-link">
          {intl.get('test')}
        </NavLink> */}
      </>
    )
  }

  const renderAuth = () => {
    if (authStatus === Status.LOGIN) {
      const memberId = userInfo?.MemberID
      return (
        <>
          <Row className="header-content-menu header-show">
            <svg style={{ marginRight: 8 }} className="header-content-icon" viewBox="64 64 896 896">
              <path d="M858.5 763.6a374 374 0 0 0-80.6-119.5 375.63 375.63 0 0 0-119.5-80.6c-.4-.2-.8-.3-1.2-.5C719.5 518 760 444.7 760 362c0-137-111-248-248-248S264 225 264 362c0 82.7 40.5 156 102.8 201.1-.4.2-.8.3-1.2.5-44.8 18.9-85 46-119.5 80.6a375.63 375.63 0 0 0-80.6 119.5A371.7 371.7 0 0 0 136 901.8a8 8 0 0 0 8 8.2h60c4.4 0 7.9-3.5 8-7.8 2-77.2 33-149.5 87.8-204.3 56.7-56.7 132-87.9 212.2-87.9s155.5 31.2 212.2 87.9C779 752.7 810 825 812 902.2c.1 4.4 3.6 7.8 8 7.8h60a8 8 0 0 0 8-8.2c-1-47.8-10.9-94.3-29.5-138.2zM512 534c-45.9 0-89.1-17.9-121.6-50.4S340 407.9 340 362c0-45.9 17.9-89.1 50.4-121.6S466.1 190 512 190s89.1 17.9 121.6 50.4S684 316.1 684 362c0 45.9-17.9 89.1-50.4 121.6S557.9 534 512 534z" fill="currentColor"></path>
            </svg>
          </Row>
          <Row className="header-content-menu" onClick={() => setUserMenuSpread(!userMenuSpread)}>
            <span style={{ paddingRight: 8, fontSize: 13 }} title={memberId}>
              {
                (() => {
                  const emailName = (userInfo?.UserID || '').split('@')[0]
                  return emailName.length > 9
                    ? `${emailName.slice(0, 5)}...${emailName.slice(-4)}`
                    : emailName
                })()
              }
            </span>

            <svg viewBox="0 0 1024 1024" className={`header-content-menu-arrow ${userMenuSpread ? 'spread' : ''}`}>
              <path d="M948.560332 281.179984c-13.765515-13.833053-36.127825-13.833053-49.89334 0L511.991302 668.591431 125.313565 281.179984c-13.763468-13.798261-36.093033-13.798261-49.856501 0-13.799284 13.798261-13.799284 36.161594 0 49.993624l410.857439 411.674037c7.067976 7.085372 16.402575 10.521634 25.675776 10.331299 9.274224 0.191358 18.608823-3.245927 25.677822-10.331299l410.891208-411.708829c6.863315-6.89913 10.331299-15.940041 10.331299-24.979928S955.423647 288.078091 948.560332 281.179984z" />
            </svg>

            <ul className={`header-content-menu-popover ${userMenuSpread ? 'show' : ''}`}>
              <li>
                <svg viewBox="0 0 1024 1024" width="30" height="30">
                  <path d="M512 128c106.039 0 192 85.961 192 192 0 106.039-85.961 192-192 192-106.039 0-192-85.961-192-192 0-106.039 85.961-192 192-192z m0 64c-70.692 0-128 57.308-128 128s57.308 128 128 128 128-57.308 128-128-57.308-128-128-128zM256 768c0-123.712 100.288-224 224-224h64c123.712 0 224 100.288 224 224v64c0 17.673-14.327 32-32 32H288c-17.673 0-32-14.327-32-32v-64z m64 0v32h384v-32c0-88.366-71.634-160-160-160h-64c-88.366 0-160 71.634-160 160z" fill="currentColor"></path>
                </svg>
                <Link className="header-content-menu-link" to={MY_INFO}>{intl.get('personal_info')}</Link>
              </li>
              {/* <li>
                <svg viewBox="0 0 1024 1024" width="20" height="20">
                  <path d="M846.367347 867.265306H177.632653c-45.97551 0-83.591837-37.616327-83.591837-83.591837V240.326531c0-45.97551 37.616327-83.591837 83.591837-83.591837h668.734694c45.97551 0 83.591837 37.616327 83.591837 83.591837v543.346938c0 45.97551-37.616327 83.591837-83.591837 83.591837zM177.632653 198.530612c-22.987755 0-41.795918 18.808163-41.795918 41.795919v543.346938c0 22.987755 18.808163 41.795918 41.795918 41.795919h668.734694c22.987755 0 41.795918-18.808163 41.795918-41.795919V240.326531c0-22.987755-18.808163-41.795918-41.795918-41.795919H177.632653z" fill="currentColor"></path><path d="M929.959184 689.632653h-323.918368c-45.97551 0-83.591837-37.616327-83.591836-83.591837V417.959184c0-45.97551 37.616327-83.591837 83.591836-83.591837h323.918368v355.265306z m-323.918368-313.469388c-22.987755 0-41.795918 18.808163-41.795918 41.795919v188.081632c0 22.987755 18.808163 41.795918 41.795918 41.795919h282.122449V376.163265h-282.122449z" fill="currentColor"></path><path d="M700.081633 512m-52.244898 0a52.244898 52.244898 0 1 0 104.489796 0 52.244898 52.244898 0 1 0-104.489796 0Z" fill="currentColor"></path>
                </svg>
                <Link className="header-content-menu-link" to={WALLET}>{intl.get('wallet')}</Link>
              </li> */}
              <li>
                <svg viewBox="0 0 1024 1024" width="30" height="30">
                  <path d="M912 190h-69.9c-9.8-19.1-22.2-37-37-53.8C723.5 54.7 623.9 16 512 16s-211.5 38.7-293.1 120.2C136.1 220.2 96 319.8 96 432c0 112.2 40.1 211.8 123.1 295.8C300.7 809.3 400.3 848 512 848c111.7 0 211.3-38.7 293.1-120.2 17.8-16.8 30.2-34.7 37-53.8H912c4.4 0 8-3.6 8-8V198c0-4.4-3.6-8-8-8zM512 784c-194.4 0-352-157.6-352-352s157.6-352 352-352 352 157.6 352 352-157.6 352-352 352z m0-640c-159.1 0-288 128.9-288 288s128.9 288 288 288 288-128.9 288-288-128.9-288-288-288z m0 512c-123.7 0-224-100.3-224-224s100.3-224 224-224 224 100.3 224 224-100.3 224-224 224z" fill="currentColor"></path>
                </svg>
                <Link className="header-content-menu-link" to={WALLET_ASSETS2}>{intl.get('my_assets')}</Link>
              </li>
              <li>
                <svg viewBox="0 0 1024 1024" width="30" height="30">
                  <path d="M853.333333 128H170.666667c-46.933333 0-85.333333 38.4-85.333334 85.333333v597.333334c0 46.933333 38.4 85.333333 85.333334 85.333333h682.666666c46.933333 0 85.333333-38.4 85.333334-85.333333V213.333333c0-46.933333-38.4-85.333333-85.333334-85.333333z m0 682.666667H170.666667V213.333333h682.666666v597.333334z" fill="currentColor"></path>
                  <path d="M512 298.666667c-117.333333 0-213.333333 96-213.333333 213.333333s96 213.333333 213.333333 213.333333 213.333333-96 213.333333-213.333333-96-213.333333-213.333333-213.333333z m0 341.333333c-70.4 0-128-57.6-128-128s57.6-128 128-128 128 57.6 128 128-57.6 128-128 128z" fill="currentColor"></path>
                </svg>
                <Link className="header-content-menu-link" to={NFT_ENTRY}>{intl.get('nft_manage')}</Link>
              </li>
              <li>
                <svg viewBox="0 0 1024 1024" width="30" height="30">
                  <path d="M336.213333 519.466667l112.384 131.114666 239.722667-259.712a21.333333 21.333333 0 1 1 31.36 28.928l-256 277.333334a21.333333 21.333333 0 0 1-31.872-0.576l-128-149.333334a21.333333 21.333333 0 1 1 32.384-27.776zM298.666667 127.936C298.666667 104.405333 317.824 85.333333 341.12 85.333333h341.76C706.304 85.333333 725.333333 104.490667 725.333333 127.957333v42.752A42.645333 42.645333 0 0 1 682.88 213.333333H341.12C317.696 213.333333 298.666667 194.176 298.666667 170.709333V127.957333zM341.333333 170.666667h341.333334V128H341.333333v42.666667z m-105.173333-42.666667v42.666667S170.666667 170.688 170.666667 170.773333V896s682.666667 0.042667 682.666666-0.106667C853.333333 895.893333 853.333333 170.666667 853.205333 170.666667h-63.296V128h63.296C876.842667 128 896 147.072 896 170.773333v725.12C896 919.509333 877.013333 938.666667 853.333333 938.666667H170.666667a42.666667 42.666667 0 0 1-42.666667-42.773334V170.773333C128 147.157333 147.114667 128 170.752 128h65.408z" fill="currentColor"></path>
                </svg>
                <Link className="header-content-menu-link" to={ORDERS}>{intl.get('my_order')}</Link></li>
              {/* <li>
                <svg viewBox="0 0 32 32" width="30" height="30">
                  <path d="M7.122 3.57v24.889h17.778v-24.889h-17.778zM7.122 1.792h17.778c0.982 0 1.778 0.796 1.778 1.778v24.889c0 0.982-0.796 1.778-1.778 1.778h-17.778c-0.982 0-1.778-0.796-1.778-1.778v-24.889c0-0.982 0.796-1.778 1.778-1.778zM11.566 10.681h8.889c0.491 0 0.889 0.398 0.889 0.889s-0.398 0.889-0.889 0.889h-8.889c-0.491 0-0.889-0.398-0.889-0.889s0.398-0.889 0.889-0.889zM11.566 16.014h8.889c0.491 0 0.889 0.398 0.889 0.889s-0.398 0.889-0.889 0.889h-8.889c-0.491 0-0.889-0.398-0.889-0.889s0.398-0.889 0.889-0.889zM11.566 21.348h5.333c0.491 0 0.889 0.398 0.889 0.889s-0.398 0.889-0.889 0.889h-5.333c-0.491 0-0.889-0.398-0.889-0.889s0.398-0.889 0.889-0.889z" fill="currentColor"></path>
                </svg>
                <Link className="header-content-menu-link" to={FINANCIAL}>{intl.get('financial_record')}</Link>
              </li> */}
              <li>
                <svg viewBox="0 0 32 32" width="30" height="30">
                  <path d="M7.122 3.57v24.889h17.778v-24.889h-17.778zM7.122 1.792h17.778c0.982 0 1.778 0.796 1.778 1.778v24.889c0 0.982-0.796 1.778-1.778 1.778h-17.778c-0.982 0-1.778-0.796-1.778-1.778v-24.889c0-0.982 0.796-1.778 1.778-1.778zM11.566 10.681h8.889c0.491 0 0.889 0.398 0.889 0.889s-0.398 0.889-0.889 0.889h-8.889c-0.491 0-0.889-0.398-0.889-0.889s0.398-0.889 0.889-0.889zM11.566 16.014h8.889c0.491 0 0.889 0.398 0.889 0.889s-0.398 0.889-0.889 0.889h-8.889c-0.491 0-0.889-0.398-0.889-0.889s0.398-0.889 0.889-0.889zM11.566 21.348h5.333c0.491 0 0.889 0.398 0.889 0.889s-0.398 0.889-0.889 0.889h-5.333c-0.491 0-0.889-0.398-0.889-0.889s0.398-0.889 0.889-0.889z" fill="currentColor"></path>
                </svg>
                <Link className="header-content-menu-link" to={FINANCIAL2}>{intl.get('financial_record')}</Link>
              </li>
              <li>
                <svg width="30" height="30" viewBox="0 0 24 24" fill="none">
                  <path d="M16 8L14.6 9.4L16.2 11H9V13H16.2L14.6 14.6L16 16L20 12L16 8Z" fill="currentColor" />
                  <path d="M5 6H11V4H5C3.9 4 3 4.9 3 6V18C3 19.1 3.9 20 5 20H11V18H5V6Z" fill="currentColor" />
                </svg>
                <a
                  className="header-content-menu-link"
                  style={{ cursor: 'pointer' }}
                  onClick={async () => {
                    try {
                      await logout({ Token: getStorage(TOKEN) })
                      setAuthStatus(Status.LOGOUT)
                      setUserInfo(null)
                      removeStorage(TOKEN)
                    } catch (e) {
                      console.log('e22', e)
                      Message.error({ message: `${intl.get('exit_login_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}` })
                    }
                  }}
                >
                  {intl.get('exit_login')}
                </a>
              </li>
            </ul>
          </Row>
          {memberId ? (
            <CopyOutlined
              className="header-copy-icon"
              onClick={(e) => {
                e.stopPropagation()
                copyText(memberId)
              }}
              style={{ marginRight: 8, fontSize: 15, color: 'rgba(21, 119, 255, 1)', marginLeft: 8, cursor: 'pointer' }}
            />
          ) : null}
          <i className="header-content-split-line" />
        </>
      )
    }
    if (authStatus === Status.LOGOUT) {
      return (
        <>
          {/* <Link
              to={REGISTER}
              className="header-content-link">
              {intl.get('sign_up')}
            </Link> */}
          {/* <Link
              to={LOGIN}
              className="header-content-auth-link">
              {intl.get('login')}
            </Link> */}
          <Button type='primary' className="header-content-auth-link" onClick={() => setModalOpen(true)}>
            {intl.get('login')}
          </Button>
          <WalletModal
            open={modalOpen}
            onClose={() => setModalOpen(false)}
          />
        </>
      )
    }
  }

  const renderDrawerAuth = () => {
    if (authStatus === Status.LOGIN) {
      return (
        <Col
          className="header-drawer-menu">
          <Row className="header-drawer-menu-row" onClick={_ => { setUserMenuSpread(!userMenuSpread) }}>
            <Row>
              <svg
                style={{ width: '20px' }}
                className="header-content-icon"
                viewBox="0 0 1024 1024">
                <path d="M512.3 460.2c109.2 0 198.1-88.9 198.1-198.1s-88.9-198-198.1-198S314.2 153 314.2 262.2s88.9 198 198.1 198z m0-324.1c69.5 0 126.1 56.6 126.1 126.1s-56.6 126.1-126.1 126.1-126.1-56.6-126.1-126.1c0.1-69.6 56.6-126.1 126.1-126.1zM956.6 873c-6.8-46.5-22.5-91.8-46.6-134.5-24.2-43-55.9-81.4-94.1-114.2-39.8-34.2-85.2-60.9-135-79.5-52.9-19.7-109.7-29.7-168.7-29.7-59 0-115.8 10-168.7 29.7-49.8 18.6-95.2 45.3-135 79.5-38.2 32.8-69.8 71.2-94.1 114.2-24 42.7-39.6 87.9-46.4 134.5-3.2 21.9 3.3 44.1 17.7 60.8 14.4 16.7 35.4 26.3 57.5 26.3h738.3c22.1 0 43-9.6 57.5-26.3 14.3-16.8 20.8-38.9 17.6-60.8z m-72.2 13.7c-1.2 1.4-2.5 1.4-3 1.4H143.2c-0.4 0-1.8 0-3-1.4-0.8-0.9-1.1-2.1-1-3.3 5.5-37.7 18.3-74.6 38.1-109.6 20.1-35.7 46.5-67.6 78.3-94.9 33.3-28.6 71.4-51 113.2-66.6 44.9-16.7 93.2-25.2 143.5-25.2s98.7 8.5 143.5 25.2c41.8 15.6 79.9 38 113.2 66.6 31.8 27.3 58.1 59.3 78.3 94.9 19.7 35 32.5 71.9 38.1 109.6 0.1 1.2-0.2 2.3-1 3.3z" />
              </svg>
              <span style={{ paddingRight: 20 }}>{userInfo?.MemberID}</span>
            </Row>
            <svg
              viewBox="0 0 1024 1024"
              className={`header-drawer-menu-arrow ${userMenuSpread ? 'spread' : ''}`}
              onClick={_ => { setUserMenuSpread(!userMenuSpread) }}>
              <path d="M948.560332 281.179984c-13.765515-13.833053-36.127825-13.833053-49.89334 0L511.991302 668.591431 125.313565 281.179984c-13.763468-13.798261-36.093033-13.798261-49.856501 0-13.799284 13.798261-13.799284 36.161594 0 49.993624l410.857439 411.674037c7.067976 7.085372 16.402575 10.521634 25.675776 10.331299 9.274224 0.191358 18.608823-3.245927 25.677822-10.331299l410.891208-411.708829c6.863315-6.89913 10.331299-15.940041 10.331299-24.979928S955.423647 288.078091 948.560332 281.179984z" />
            </svg>
          </Row>
          {
            userMenuSpread &&
            <>
              <Link className="header-drawer-menu-link" to={ASSETS}>{intl.get('assets_views')}</Link>
              <Link className="header-drawer-menu-link" to={ORDERS}>{intl.get('my_order')}</Link>
              <Link className="header-drawer-menu-link" to={FINANCIAL}>{intl.get('financial_record')}</Link>
              <Link className="header-drawer-menu-link" to={FINANCIAL2}>财务记录2</Link>
              <a
                className="header-drawer-menu-link"
                onClick={async () => {
                  try {
                    await logout()
                    setAuthStatus(Status.LOGOUT)
                    setUserInfo(null)
                    removeStorage(TOKEN)
                  } catch (e) {
                    Message.error({ message: `${intl.get('exit_login_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}` })
                  }
                }}>
                {intl.get('exit_login')}
              </a>
            </>
          }
        </Col>
      )
    }
    if (authStatus === Status.LOGOUT) {
      return (
        <Row className="header-drawer-auth">
          {/* <Link
              to={REGISTER}
              className="header-drawer-auth-link register">
              {intl.get('sign_up')}
            </Link> */}
          {/* <Link
              to={LOGIN}
              className="header-drawer-auth-link login">
              {intl.get('login')}
            </Link> */}
          <Button
            style={{ marginBottom: 16 }}
            className="exchange-order-board-logout-button"
            onClick={() => setModalOpen(true)}
            type="tertiary">
            {/* <Link className="loginIN" to={LOGIN}>{intl.get('login')}</Link> */}
            {/* <span style={{ padding: '0 8px' }}>{intl.get('or')}</span>
                    <Link className="signUp" to={REGISTER}>{intl.get('sign_up')}</Link> */}
          </Button>
          <WalletModal
            open={modalOpen}
            onClose={() => setModalOpen(false)}
          />
        </Row>
      )
    }
  }

  const [searchValue, setSearchValue] = useState('')
  const [suggestions, setSuggestions] = useState([])
  const [symbolList, setSymbolList] = useState([])
  const fetchSymbols = async () => {
    try {
      const requestConfig = {
        headers: { Zip: 3 }
      }
      // console.log('requestConfig', requestConfig)
      const symbolList = await getPubilc('public/v1.0/SendQryInstrument', { All: 'All' }, requestConfig)
      // console.log('symbolList', symbolList)
      setSymbolList(symbolList)
    } catch (e) {
      console.error('获取合约列表失败', e)
    }
  }
  const handleSearchChange = (e) => {
    const value = e.target.value
    setSearchValue(value)
    // console.log('value', value)
    if (value.trim() && symbolList.length > 0) {
      const matches = symbolList.filter(s =>
        s.I.toLowerCase().includes(value.toLowerCase())
      )
      // console.log('matches', matches)
      setSuggestions(matches)
    } else {
      setSuggestions([])
    }
  }
  const handleSelect = (item) => {
    const { I: InstrumentID, G: ProductGroup } = item
    setSearchValue('')
    setSuggestions([])
    navigate(`/exchange?ProductGroup=${ProductGroup}&InstrumentID=${InstrumentID}`)
  }

  return (
    <ComponentContextProvider
      theme={theme}>
      <header
        ref={header}
        className="header">
        {
          <Link
            className="header-logo header-show"
            to={HOME_PATH}>
            <img src={logoPng} />
          </Link>
        }
        <Row className="header-content-container-wrapper" style={{ maxWidth: 520 }}>
          <Row
            className="header-content-container" style={{ minWidth: 505 }}>
            {renderLeftLinks()}
          </Row>
        </Row>

        <Row onClick={() => setDrawerVisible(!drawerVisible)}>
          {!isIp() &&
            <img style={{ minWidth: 32 }} className="header-vsshow" src="/favicon.ico" />}
          {isIp() &&
            <svg className="header-vsshow"
              viewBox="0 0 18 14" width="18" height="18">
              <path fillRule="evenodd" clipRule="evenodd" d="M2.1001 0.999976C2.1001 0.502919 2.50304 0.0999756 3.0001 0.0999756H17.0001C17.4972 0.0999756 17.9001 0.502919 17.9001 0.999976C17.9001 1.49703 17.4972 1.89998 17.0001 1.89998H3.0001C2.50304 1.89998 2.1001 1.49703 2.1001 0.999976ZM0.100098 6.99998C0.100098 6.50292 0.503041 6.09998 1.0001 6.09998H17.0001C17.4972 6.09998 17.9001 6.50292 17.9001 6.99998C17.9001 7.49703 17.4972 7.89998 17.0001 7.89998H1.0001C0.503041 7.89998 0.100098 7.49703 0.100098 6.99998ZM4.1001 13C4.1001 12.5029 4.50304 12.1 5.0001 12.1H17.0001C17.4972 12.1 17.9001 12.5029 17.9001 13C17.9001 13.497 17.4972 13.9 17.0001 13.9H5.0001C4.50304 13.9 4.1001 13.497 4.1001 13Z" />
            </svg>}
        </Row>
        <Popover
          className="header-drawer"
          visible={drawerVisible}
          getContainer={() => header.current}
          placement="bottomLeft"
        >
          {renderLeftLinks()}
        </Popover>

        <Row style={{ position: 'relative', marginRight: 'auto', marginLeft: 10, height: '100%' }}>
          <div style={{ position: 'relative' }} className="header-input" >
            {/* <SearchOutlined
              className="header-search-icon"
            /> */}
            {/* <input
              type="text"
              placeholder="搜索币种"
              value={searchValue}
              onFocus={fetchSymbols}
              onChange={handleSearchChange}
              className="header-search-input"
            /> */}
            {suggestions.length > 0 && (
              <ul className="header-search-suggestions">
                {suggestions.map((item, index) => (
                  <li
                    key={index}
                    onClick={() => handleSelect(item)}
                    className="header-search-suggestion-item"
                  >
                    <span>{item.I}</span>
                    <span className="header-search-suggestion-group">{item.G}</span>
                  </li>
                ))}
              </ul>
            )}
          </div>
        </Row>
        <Row
          className="header-content-container" style={{ justifyContent: 'flex-end', minWidth: 150 }}>
          {
            settingModalVisible &&
            <Suspense
              fallback={<div />}>
              <SettingModal onClose={() => { setSettingModalVisible(false) }} />
            </Suspense>
          }
          {renderAuth()}
          <ThemeSwitch />
          <svg
            viewBox="0 0 1024 1024"
            width="20" height="20"
            style={{ cursor: 'pointer', marginLeft: 12 }}
            onClick={() => { setSettingModalVisible(true) }}>
            <path
              d="M512 128c12.8 0 25.6 0 41.6 3.2l22.4 67.2 32 9.6c19.2 6.4 35.2 12.8 54.4 22.4l28.8 16 64-32c22.4 16 41.6 38.4 60.8 60.8l-32 64 16 28.8c9.6 16 16 35.2 22.4 54.4l9.6 32 32 9.6 35.2 12.8c-3.2 9.6-3.2 22.4-3.2 35.2 0 12.8 0 25.6-3.2 41.6l-35.2 12.8-32 9.6-9.6 32c-6.4 19.2-12.8 35.2-22.4 51.2l-16 28.8 32 64c-9.6 12.8-19.2 22.4-28.8 32-9.6 9.6-19.2 19.2-32 28.8l-64-32-28.8 16c-16 9.6-35.2 16-54.4 22.4l-32 9.6-9.6 32-12.8 35.2H512c-12.8 0-25.6 0-41.6-3.2l-12.8-35.2-9.6-32-32-9.6c-19.2-6.4-35.2-12.8-51.2-22.4l-28.8-16-28.8 16-32 16c-12.8-9.6-22.4-19.2-32-28.8-9.6-9.6-19.2-19.2-28.8-32l32-64-16-28.8c-9.6-16-16-35.2-22.4-54.4l-9.6-32-67.2-22.4C128 540.8 128 524.8 128 512c0-12.8 0-25.6 3.2-41.6l67.2-22.4 9.6-32c6.4-19.2 12.8-35.2 22.4-54.4l16-28.8-16-28.8-16-32c16-22.4 38.4-41.6 57.6-57.6l64 32 28.8-16c16-9.6 35.2-16 54.4-22.4l32-9.6 9.6-32 12.8-35.2c12.8-3.2 25.6-3.2 38.4-3.2m0-64c-16 0-32 0-48 3.2-25.6 3.2-44.8 19.2-54.4 44.8l-12.8 35.2c-22.4 6.4-44.8 16-64 25.6l-32-16c-9.6-6.4-19.2-6.4-28.8-6.4-16 0-28.8 3.2-41.6 12.8-25.6 19.2-48 44.8-67.2 70.4-16 19.2-19.2 44.8-6.4 67.2l16 32c-9.6 22.4-19.2 41.6-25.6 64l-35.2 12.8c-22.4 6.4-41.6 28.8-44.8 54.4-3.2 16-3.2 32-3.2 48s0 32 3.2 51.2c3.2 25.6 19.2 44.8 44.8 54.4l35.2 12.8c6.4 22.4 16 41.6 25.6 64l-16 32c-9.6 22.4-9.6 48 6.4 67.2 9.6 12.8 22.4 25.6 32 38.4 9.6 9.6 22.4 22.4 38.4 32 12.8 9.6 25.6 12.8 38.4 12.8 9.6 0 19.2-3.2 28.8-6.4l32-16c19.2 9.6 41.6 19.2 64 25.6l12.8 35.2c6.4 22.4 28.8 41.6 54.4 44.8H512c16 0 32 0 51.2-3.2 25.6-3.2 44.8-19.2 54.4-44.8l12.8-35.2c22.4-6.4 44.8-16 64-25.6l32 16c9.6 3.2 19.2 6.4 28.8 6.4 12.8 0 28.8-3.2 38.4-12.8 12.8-9.6 25.6-22.4 38.4-32 9.6-9.6 22.4-22.4 32-38.4 16-19.2 19.2-44.8 6.4-67.2l-16-32c9.6-19.2 19.2-41.6 25.6-64l35.2-12.8c22.4-6.4 41.6-28.8 44.8-54.4V512c0-16 0-32-3.2-51.2-3.2-25.6-19.2-44.8-44.8-51.2l-35.2-12.8c-6.4-22.4-16-41.6-25.6-64l16-32c9.6-22.4 9.6-48-6.4-70.4-19.2-25.6-44.8-48-70.4-70.4-12.8-9.6-25.6-12.8-41.6-12.8-9.6 0-19.2 3.2-28.8 6.4l-32 16c-22.4-9.6-41.6-19.2-64-25.6l-12.8-35.2c-6.4-22.4-28.8-41.6-54.4-44.8H512z" />
            <path
              d="M512 736c-124.8 0-224-99.2-224-224s99.2-224 224-224 224 99.2 224 224-99.2 224-224 224z m0-384c-89.6 0-160 70.4-160 160s70.4 160 160 160 160-70.4 160-160-70.4-160-160-160z" />
          </svg>
        </Row>
      </header>
    </ComponentContextProvider>
  )
})

export default Header