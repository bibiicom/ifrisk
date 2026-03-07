import { get, post } from '../network/http'
import { getPubilc, postPubilc } from '../network/pubilcHttp'

import { saveStorage, TOKEN, getStorage } from '../utils/storage'

export async function register (userId, password) {
  await postPubilc(
    'admin/v1.0/SendUserRegister',
    {
      UserID: userId,
      Password: password,
      MemberID: userId
      // UserType: '1',
      // EncryptType: '0',
      // IsActive: 1,
      // DefaultAccountID: userId,
      // DefaultMemberID: userId,
      // DefaultTradeUnitID: userId
    }
  )
  return login(userId, password)
}

export async function login (userId, password) {
  // console.log('userid', userId)
  const result = await postPubilc('admin/v1.0/SendUserLogin', { UserID: userId, Password: password })
  // console.log('result', result)
  saveStorage(TOKEN, result[0].data.Token)
  // console.log('localetoken', localetoken)
  document.cookie = `token=${result[0].data.Token}; Domain=.tobetrader.com`
  return result
}

export function logout () {
  return post('action/v1.0/SendUserLogout')
}

export function getNonce (address) {
  return getPubilc('admin/v1.0/SendWalletNonce', { UserID: address })
}

export async function loginForWallet (address, signature) {
  const result = await postPubilc('admin/v1.0/SendWalletLogin', { UserID: address, Signature: signature })
  saveStorage(TOKEN, result[0].data.Token)
  // console.log('token', result[0].data)
  document.cookie = `token=${result[0].data.Token}; Domain=.tobetrader.com`
  return result
}

export async function frontRegisterForEmail (email, password, verifyCode) {
  await post('front/v1/auth/email/register', { email, password, verifyCode })
  return login(email, password)
}

export function sendVerifyCode (email) {
  return post('front/v1/auth/email/verify_code', { email })
}

export function checkToken () {
  return get(`private/v1.0/SendCheckToken?Token=${getStorage(TOKEN) || `${Date.now()}`}`)
}

/**
 * 出入金
 * @param params
 * @returns {Promise}
 */
export async function depositWithdrawal (params) {
  return postPubilc('admin/v1.0/SendAccountAction', params)
}
