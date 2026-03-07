import { get } from '../network/http'

/**
 * 获取计算记录
 * @param params
 * @returns {Promise}
 */
export function getSettlementRecord (params) {
  return get('query/v1.0/SettleDetail', { SettleAction: '1', ...params })
}

export function getHistoryFundingRate (params) {
  return get('query/v1.0/FundingRate', params)
}
