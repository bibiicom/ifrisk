import { post, get } from '../network/http'
import { getStorage, TOKEN } from '../utils/storage'

export function register (userId, password) {
  return post(
    'action/v1.0/SendUserRegister',
    {
      UserID: userId,
      Password: password,
      MemeberID: userId
    }
  )
}

export function login (userId, password) {
  post('action/v1.0/SendUserLogin', { UserID: userId, Password: password }).then((loginResponse) => {
    console.log('loginRes', loginResponse)
    let data = loginResponse.data
    if (typeof data !== 'object') {
      try {
        data = JSON.parse(loginResponse.data)
      } catch (error) { }
    }
    console.log('login', ((data.result || [])[0] || {}).data || {})
    return ((data.result || [])[0] || {}).data || {}
  })
}

export function logout (params) {
  return post('action/v1.0/SendUserLogout', params)
}

export function checkToken () {
  return get(`private/v1.0/SendCheckToken?Token=${getStorage(TOKEN) || `${Date.now()}`}`)
}

/**
 * 开同合约交易
 * @param data
 * @return {Promise<*>}
 */
export function openTrade (data) {
  return post('action/v1.0/SendInsert/User', data)
}

/**
 * 获取开同合约交易信息
 * @param data
 * @return {Promise<AxiosResponse<any>>}
 */
export function getOpenTrade (data = {}) {
  return post('action/v1.0/SendQryOne/User', data, true)
}
