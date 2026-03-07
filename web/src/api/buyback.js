import { post } from '../network/http'

// 回购持仓排行榜
export function fetchPositionRank(params = {}) {
  return post('/backend/v1/buyback/positionRank', params)
}

// 7 日价格走势
export function fetchSevenDayPrice(params = {}) {
  return post('/backend/v1/buyback/sevenDayPrice', params)
}

// 申请回购
export function applyBuyback(data = {}) {
  return post('/backend/v1/buyback/applyBuyback', data)
}

// 获取回购状态
export function fetchBuybackStatus(params = {}) {
  return post('/backend/v1/buyback/buyBackStatus', params)
}

// 获取全部币种持有人数排行榜
export function fetchCurrencyRank(params = {}) {
  return post('/backend/v1/buyback/currencyRank', params)
}

// 获取历史回购记录
export function fetchBuybackHistory(params = {}) {
  return post('/backend/v1/launchpad/getBuybackLaunchpadInfo', params)
}
