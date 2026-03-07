import React, { createContext, useContext, useMemo } from 'react'
import usePolling from '../../component/hooks/usePolling'
import { getAssets } from '../../api/account'
import RootContext from '../rootContext'

const AccountContext = createContext()

export const AccountContextProvider = ({ children }) => {
  const { userInfo } = useContext(RootContext)

  // 精确参数配置与空值保护
  const {
    result: accountResult = [],
    request: accountRequest
  } = usePolling(
    {
      api: getAssets,
      params: {
        AccountID: userInfo?.MemberID
      },
      flag: userInfo && userInfo.MemberID,
      interval: 5000
    },
    [(userInfo || {}).MemberID])

  // console.log('accountResult', accountResult)

  // 优化值记忆
  const contextValue = useMemo(() => ({
    accountResult,
    accountRequest
  }), [accountResult, accountRequest])

  return (
    <AccountContext.Provider value={contextValue}>
      {children}
    </AccountContext.Provider>
  )
}

// 添加hooks导出
export const useAccountContext = () => {
  const context = useContext(AccountContext)
  if (!context) {
    throw new Error('必须在AccountContextProvider内使用')
  }
  return context
}
