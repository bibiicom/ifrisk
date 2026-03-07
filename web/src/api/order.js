import { post, get } from '../network/http'

/**
 * 限价委托下单
 * @param params
 * @returns {Promise}
 */
export function limitOrder (params) {
  return post('action/v1.0/SendOrderInsert', params)
}

/**
 * 条件单下单
 * @param params
 * @return {Promise}
 */
export function triggerOrder (params) {
  return post('action/v1.0/SendTriggerOrderInsert', params)
}

/**
 * 订单止盈止损
 * @param params
 * @returns
 */
export function orderStopProfitLoss (params) {
  return post('action/v1.0/SendCloseOrderInsert', params)
}

/**
 * 撤限价单
 * @param params
 * @returns {Promise}
 */
export function revokeLimitOrder (params) {
  return post('action/v1.0/SendOrderAction', { ...params, ActionFlag: '1' })
}

/**
 * 撤条件单
 * @param params
 * @return {Promise}
 */
export function revokeTriggerOrder (params) {
  return post('action/v1.0/SendTriggerOrderAction', { ...params, ActionFlag: '1' })
}

/**
 * 撤销订单止盈止损
 * @param params
 * @returns
 */
export function revokeOrderStopProfitLoss (params) {
  return post('action/v1.0/SendCloseOrderAction', { ...params, ActionFlag: '1' })
}

/**
 * 修改条件单
 * @param params
 * @returns
 */
export function editTriggerOrder (params) {
  return post('action/v1.0/SendTriggerOrderAction', { ...params, ActionFlag: '4' })
}

/**
 * 修改订单止盈止损
 * @param params
 * @returns
 */
export function editOrderStopProfitLoss (params) {
  return post('action/v1.0/SendCloseOrderAction', { ...params, ActionFlag: '4' })
}

/**
 * 获取当前委托
 * @returns {Promise}
 */
export async function getCurrentLimitOrder (params, config) {
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 2 }
  return get('private/v1.0/SendQryOrder', params, requestConfig)
  // const res = await get('private/v1.0/SendQryOrder', params, requestConfig)
  // const newList = {
  //   data: []
  // }

  // for (let i = 0; i < res?.length; i++) {
  //   newList.data.push(res[i]?.data)
  // }
  // return newList
}

/**
 * 获取当前条件单
 * @param params
 * @return {Promise}
 */
export function getCurrentTriggerOrder (params, config) {
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 2 }
  return get('private/v1.0/SendQryTriggerOrder', params, requestConfig)
}

/**
 * 获取历史委托
 * @returns {Promise}
 */
export function getHistoryLimitOrder (params, config) {
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 2 }
  return get('query/v1.0/SendQryFinishOrder', params, requestConfig)
}

/**
 * 获取历史触发单
 * @param params
 * @return {Promise}
 */
export function getHistoryTriggerOrder (params, config) {
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 2 }
  return get('query/v1.0/SendQryFinishTriggerOrder', params, requestConfig)
}

/**
 * 获取成交订单
 * @param params
 * @returns {Promise}
 */
export function getTradeOrder (params, config) {
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 2 }
  return get('query/v1.0/SendQryTrade', params, requestConfig)
}

/**
 * 获取历史订单列表
 * @param params
 * @param config
 * @returns {Promise}
 */
export function getFinishOrderList (params, config) {
  return post('backend/v1/finishOrder/getFinishOrderList', params, config)
}
