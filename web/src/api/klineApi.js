import { get } from '../network/http'

/**
 * 获取历史k线数据
 * @param symbol
 * @param to
 * @param interval
 * @param exchangeId
 * @returns {Promise<T>}
 */
export function getHistoryKLine (symbol, to, interval, exchangeId, config) {
  return get('public/v1.0/SendQryKLine', {
    InstrumentID: symbol,
    ExchangeID: exchangeId,
    Limit: 1000,
    Bar: interval
  }, true, config)
}
