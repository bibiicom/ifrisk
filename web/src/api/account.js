import axios from 'axios'
import { get, post } from '../network/http'
import { getHttpUrl } from '../network/domain'

/**
 * 获取资金信息
 * @returns {Promise}
 */
export async function getAssets (params, config) {
  // const res = await get('private/v1.0/SendQryAccount', params, config)
  // const newList = {
  //   data: []
  // }
  // console.log('res', res)
  // for (let i = 0; i < res?.length; i++) {
  //   newList.data.push(res[i])
  // }
  // console.log('newList', newList)
  // return newList
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 2 }
  // const res = await get('private/v1.0/SendQryAccount', params, requestConfig)
  // console.log('res', res)
  return get('private/v1.0/SendQryAccount', params, requestConfig)
}

export function getWalletAssets (params) {
  return get('wallet/v1/account/get', params)
}

export function getDepositAddresses (params) {
  return get('wallet/v1/address/get', params)
}

export function createDepositAddress (params) {
  return post('wallet/v1/address/create', params)
}

export function getBlockchainRecordList (params, config) {
  return get('wallet/v1/account/flow', params, config)
}

export function transfer (params) {
  return post('wallet/v1/account/transfer', params)
}

export function withdrawal (params) {
  return post('admin/v1.0/SendWithdraw', params)
}

export function stopcock (params) {
  return post('wallet/v1/account/stopcock', params)
}

export function test (params) {
  return get('wallet/v1/account/test', params)
}

/**
 * 获取财务记录
 * @param params
 * @return {Promise<*>}
 */
export function getFinancialRecord (params, config) {
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 2 }
  return get('query/v1.0/SendQryAccountDetail', params, requestConfig)
}

export function deposit (params) {
  return post('admin/v1.0/SendAccountAction', params)
}


export function getAccountHistoryCsv (address, config = {}) {
  const addr = (address || '').trim()
  if (!addr) {
    return Promise.resolve('')
  }
  const url = `his1h/account.${encodeURIComponent(addr)}.csv`
  return axios
    .get(url, {
      baseURL: getHttpUrl(),
      withCredentials: true,
      responseType: 'text',
      validateStatus: (status) => status === 200 || status === 404,
      ...config
    })
    .then((resp) => (resp.status === 404 ? '' : resp.data))
}


/**
 * 获取充值记录CSV
 * @param {string} address - 钱包地址
 * @param {object} config - axios配置
 * @returns {Promise<string>} CSV字符串
 */
export function getDepositHistoryCsv(address, config = {}) {
  const addr = (address || '').trim()
  if (!addr) {
    return Promise.resolve('')
  }
  const url = `hisw/deposit/${encodeURIComponent(addr)}.csv`
  return axios
    .get(url, {
      baseURL: getHttpUrl(),
      withCredentials: true,
      responseType: 'text',
      validateStatus: (status) => status === 200 || status === 404,
      ...config
    })
    .then((resp) => (resp.status === 404 ? '' : resp.data))
}

/**
 * 获取提取记录CSV
 * @param {string} address - 钱包地址
 * @param {object} config - axios配置
 * @returns {Promise<string>} CSV字符串
 */
export function getWithdrawHistoryCsv(address, config = {}) {
  const addr = (address || '').trim()
  if (!addr) {
    return Promise.resolve('')
  }
  const url = `hisw/withdraw/${encodeURIComponent(addr)}.csv`
  return axios
    .get(url, {
      baseURL: getHttpUrl(),
      withCredentials: true,
      responseType: 'text',
      validateStatus: (status) => status === 200 || status === 404,
      ...config
    })
    .then((resp) => (resp.status === 404 ? '' : resp.data))
}

/**
 * 获取账户明细列表
 * @param {object} params - 请求参数
 * @param {object} config - 请求配置
 * @returns {Promise}
 */
export function getAccountDetailList(params, config) {
  return post('backend/v1/accountDetail/getAccountDetailList', params, config)
}
