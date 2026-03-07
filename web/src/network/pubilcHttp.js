import axios from 'axios'
import { getHttpUrl } from './domain'

const instance = axios.create({
  baseURL: getHttpUrl(),
  timeout: 20000,
  headers: {
    Accept: 'text/plain;',
    'Accept-Language': 'zh-CN',
    'Cache-Control': 'no-cache',
    'Content-Type': 'application/json; charset=UTF-8',
    'Access-Control-Allow-Origin': '*',
    source: 'web'
  },
  withCredentials: true
})

/**
 * 请求拦截器
 */
instance.interceptors.request.use(config => {
  return config
}, error => {
  return Promise.reject(error)
})

/**
 * 响应拦截器
 */
instance.interceptors.response.use(response => {
  // console.log('response', response)
  if (response.data === null) {
    return Promise.reject({ errorCode: -1, errorMsg: 'request error' })
  }
  const data = response.data
  if ('errorCode' in data) {
    if (data.errorCode !== 0) {
      // console.log('data', data)
      return Promise.reject({ errorCode: data.errorCode, errorMsg: data.errorMsg })
    } else {
      return (data.result) || []
    }
  }
}, _ => {
  return Promise.reject({ errorCode: -1, errorMsg: 'network error' })
})

/**
 * 封装后的post方法
 *
 * @param {string} url 请求路径
 * @param {object} data 请求参数
 * @param {boolean} config 配置
 * @returns
 */
function postPubilc (url, data = {}, config = {}) {
  return instance.post(url, JSON.stringify(data), config)
}

/**
 * 封装后的get方法
 *
 * @param {string} url 请求路径
 * @param {object} data 请求参数
 * @param {boolean} config 配置
 * @returns
 */
function getPubilc (url, data = {}, config = {}) {
  // console.log('get', url)
  const keyValues = []
  for (const key in data) {
    keyValues.push(`${key}=${data[key]}`)
  }
  const params = keyValues.length > 0 ? `?${keyValues.join('&')}` : ''
  const spliceUrl = `${url}${params}`
  return instance.get(spliceUrl, config)
}

export { postPubilc, getPubilc }
