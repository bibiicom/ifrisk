import React, { useContext, useEffect } from 'react'

import RootContext from '../../rootContext'

import { checkToken } from '../../../api/public'
import { getStorage, TOKEN } from '../../../utils/storage'

import { Status } from '../../../constants/auth'

export default (isVerify) => Component => {
  const AuthComponent = () => {
    const { setAuthStatus, setUserInfo } = useContext(RootContext)

    useEffect(() => {
      // console.log('isVerify', isVerify)
      if (isVerify) {
        const check = async () => {
          const token = getStorage(TOKEN)
          if (!token) {
            // 没有 token，直接设置为未登录
            setAuthStatus(Status.LOGOUT)
            setUserInfo(null)
            return
          }

          // 有 token，验证是否有效
          try {
            const userInfo = await checkToken()
            // console.log('userInfo111111111', userInfo)
            setAuthStatus(Status.LOGIN)
            setUserInfo(userInfo[0].data)
          } catch (e) {
            setAuthStatus(Status.LOGOUT)
            setUserInfo(null)
          }
        }
        check()
      }
    }, [])

    // if (isProtected && authStatus === Status.LOGOUT) {
    //   return <Redirect to={LOGIN}/>
    // }
    return <Component/>
  }
  return AuthComponent
}
