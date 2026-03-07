import axios from 'axios'
import { getHttpUrl } from './domain'
import { getStorage, TOKEN } from '../utils/storage'

const instance = axios.create({
  baseURL: getHttpUrl(),
  timeout: 20000,
  headers: {
    Accept: 'text/plain;',
    'Accept-Language': 'zh-CN',
    'Cache-Control': 'no-cache',
    'Content-Type': 'application/json; charset=UTF-8',
    'Access-Control-Allow-Origin': '*',
    source: 'web',
    Otoken: getStorage(TOKEN) || `${Date.now()}`
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
  if (response.data === null) {
    return Promise.reject({ errorCode: -1, errorMsg: 'request error' })
  }
  // console.log('response2', response)
  const data = response.data
  // 本地调试兼容逻辑：在保持旧结构兼容的基础上，
  // 仅在 data 为对象时解析 errorCode/c，其他情况直接返回 data

  // 只有当 data 是对象时，才按旧结构(errorCode/c)解析
  if (data && typeof data === 'object') {
    if ('errorCode' in data) {
      if (data.errorCode !== 0) {
        // console.log('data', data)
        return Promise.reject({ errorCode: data.errorCode, errorMsg: data.errorMsg })
      } else {
        // return (data.result[0] || {}).data || {}
        return data.result || []
      }
    }
    if ('c' in data) {
      if (data.c !== 0) {
        return Promise.reject({ errorCode: data.errorCode, errorMsg: data.errorMsg })
      } else {
        return (data.r) || []
      }
    }
    // 处理 code/msg 结构的响应
    if ('code' in data) {
      if (data.code !== 0) {
        return Promise.reject({ errorCode: data.code, errorMsg: data.msg, msg: data.msg })
      } else {
        return data
      }
    }
  }

  // 兼容 code/data 这类普通结构，或任意其他 JSON，直接返回 data
  return data
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
function post (url, data = {}, config = {}) {
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
function get (url, data = {}, config = {}) {
  // console.log('get', url)
  const keyValues = []
  for (const key in data) {
    keyValues.push(`${key}=${data[key]}`)
  }
  const params = keyValues.length > 0 ? `?${keyValues.join('&')}` : ''
  const spliceUrl = `${url}${params}`
  return instance.get(spliceUrl, config)
}

export { post, get }
