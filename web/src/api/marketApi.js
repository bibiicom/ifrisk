import { get } from '../network/http'

/**
 * 获取汇率
 * @returns {Promise}
 */
export function getExchangeRate () {
  return get('query/v1.0/exchange_rate')
}

/**
 * 获取订单薄
 * @param params
 * @return {Promise}
 */
export function getOrderBook (params) {
  return get('action/v1.0/SendQryMarketOrder', params, true)
}
