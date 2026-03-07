import React, { createRef, useEffect, useState } from 'react'
import { BrowserRouter, Routes, Route, Navigate, useLocation } from 'react-router-dom'
import { OverlayScrollbarsComponent } from 'overlayscrollbars-react'
import { Helmet } from 'react-helmet'

import intl from '../component/intl'
import { RootContextProvider } from './rootContext'
import { ComponentContextProvider } from '../component/context'
import { useTheme } from '../hooks/useTheme'

import Header from './headerfooter/Header'
import Footer from './headerfooter/Footer'

import { Status } from '../constants/auth'
import { getStorageConfig, saveStorageConfig, COLOR_THEME_KEY, LOCALE_KEY } from './storageConfig'
import { routes, REDIRECT_PATH, EXCHANGE } from './routes'
import { isIp } from '../utils/ip'
import { TonConnectUIProvider } from "@tonconnect/ui-react"
import './color-theme.less'

/** 
 * Scrollbars 组件：监听路由变化时滚动到顶部
 * v6 不需要 withRouter，直接用 useLocation
 */
const Scrollbars = ({ children }) => {
  const scrollbar = createRef()
  const { pathname } = useLocation()

  useEffect(() => {
    if (scrollbar.current) {
      scrollbar.current.osInstance().elements().viewport.scrollTo({ top: 0 })
    }
  }, [pathname])


  return (
    <OverlayScrollbarsComponent style={{ height: '100%' }} ref={scrollbar}>
      {children}
    </OverlayScrollbarsComponent>
  )
}

const App = () => {
  const [authStatus, setAuthStatus] = useState(Status.NORMAL)
  const [userInfo, setUserInfo] = useState(null)
  const [config, setConfig] = useState(getStorageConfig())
  const { theme } = useTheme()

  const setInnerConfig = c => {
    const newConfig = { ...config, ...c }
    saveStorageConfig(newConfig)
    setConfig(newConfig)
  }


  useEffect(() => {
    document.querySelector('html').className = `color-theme-${config[COLOR_THEME_KEY]}`
  }, [config[COLOR_THEME_KEY]])

  const locale = config[LOCALE_KEY] || "en_US";

  if (window.location.pathname === '/') {
    window.location.replace(`/${locale}${REDIRECT_PATH}`)
    return null
  }

  return (
    <TonConnectUIProvider manifestUrl={`${window.location.origin}/tonconnect-manifest.json`}>
      <ComponentContextProvider theme={theme}>
        <RootContextProvider
          value={{
            config,
            setConfig: setInnerConfig,
            authStatus,
            setAuthStatus,
            userInfo,
            setUserInfo
          }}>
          <BrowserRouter basename={`/${locale}`}>
            <Scrollbars>
              <Helmet>
                <title>{intl.get(isIp() ? 'html_title_sim' : 'html_title_tobetrader')}</title>
                <meta name="description" content={intl.get(isIp() ? 'html_title_sim' : 'html_title_tobetrader')} />
                <link rel="icon" href={isIp() ? '/x.ico' : '/favicon.ico'} />
              </Helmet>
              <Header />
              <div className={`app-content ${!window.location.pathname.match(EXCHANGE) ? 'app-mini-content' : ''}`}>
                <Routes>
                  {/* 根路径 /en-US/ 自动跳 /en-US/market */}
                  <Route index element={<Navigate to={REDIRECT_PATH} replace />} />

                  {/* 你的正常路由 */}
                  {routes().map((route, index) => (
                    // <Route key={index} path={route.path.replace(`/${locale}`, '')} element={<route.component />} />
                    <Route key={index} path={route.path} element={<route.component />} />
                  ))}

                  {/* 兜底路由，未匹配路径跳 /en-US/market */}
                  <Route path="*" element={<Navigate to={REDIRECT_PATH} replace />} />
                </Routes>
              </div>
              <Footer />
            </Scrollbars>
          </BrowserRouter>
        </RootContextProvider>
      </ComponentContextProvider>
    </TonConnectUIProvider>
  )
}

export default App
