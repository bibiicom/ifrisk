import { post, get } from '../network/http'
import { getPubilc } from '../network/pubilcHttp'

export function getLastKLine (symbolId, exchangeId, interval, count, config) {
  // console.log('config', config)
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 4 }
  // console.log('requestConfig', requestConfig)
  return getPubilc('public/v1.0/SendQryKLine', {
    InstrumentID: symbolId,
    ExchangeID: exchangeId,
    Limit: count,
    Bar: interval
  }, requestConfig)
}

export function getHistoryKLine (symbolId, exchangeId, interval, to, count, config) {
  // console.log('参数', symbolId, exchangeId, interval, to, count)
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 4 }
  return getPubilc('query/v1.0/SendQryKLine', {
    InstrumentID: symbolId,
    ExchangeID: exchangeId,
    EndTime: to,
    Limit: count,
    Bar: interval
  }, requestConfig)
}

/**
 * 获取交易配置
 * @param params
 * @returns
 */
export function getTradeConfig (params, config) {
  return post('action/v1.0/SendQryLeverage', params, config)
}

/**
 * 获取持仓类型
 * @param params
 * @returns
 */
export function getPositionType (params, config) {
  return post('action/v1.0/SendQryMemberPosition', params, config)
}

/**
 * 获取当前维持保证金率
 * @param params
 * @returns
 */
export function getMaintMarginRatio (params) {
  return get('action/v1.0/SendQryOne/MarginRate', params)
}

/**
 * 获取保证金率
 * @param params
 * @returns
 */
export function getFirstStageMarginRate (params, config) {
  return get('action/v1.0/SendQryOne/MarginRate', params, config)
}

/**
 * 获取现货资产
 * @param params
 * @returns
 */
export function getSpotAssets (params) {
  return get('request/customerAsset/withdrawAndC2C/usableAmount', params)
}

/**
 * websocket action
 * @type {{UNSUBSCRIBE: string, SUBSCRIBE: string}}
 */
const Action = {
  // 订阅
  SUBSCRIBE: '1',
  // 取消订阅
  UNSUBSCRIBE: '2'
}

/**
 * ws订阅主题分类
 */
export const Topic = {
  // 成交详情
  TRADE_DETAIL: '2',
  // k线
  K_LINE: '11',
  // 所有行情快照
  MARKET_OVERVIEW: '17',
  // 单个币对订阅
  MARKET_DETAIL: '7',
  // 25档盘口
  MARKET_DEPTH_25: '25',
  // 全量盘口
  MARKET_DEPTH_ALL: '3'
}

export function subscribeMarketOverview (ws, productGroup) {
  ws.send({
    SendTopicAction: {
      Action: Action.SUBSCRIBE,
      TopicID: Topic.MARKET_OVERVIEW,
      Index: productGroup,
      Zip: 3,
      ResumeNo: -1
    }
  })
}

export function unsubscribeMarketOverview (ws, productGroup) {
  ws.send({
    SendTopicAction: {
      Action: Action.UNSUBSCRIBE,
      TopicID: Topic.MARKET_OVERVIEW,
      Index: productGroup,
      ResumeNo: -1
    }
  })
}

export function subscribeMarketDetail (ws, symbol) {
  ws.send({
    SendTopicAction: {
      Action: Action.SUBSCRIBE,
      TopicID: Topic.MARKET_DETAIL,
      Index: `${symbol.ExchangeID}_${symbol?.InstrumentID}`,
      ResumeNo: -1
    }
  })
}

export function unsubscribeMarketDetail (ws, symbol) {
  ws.send({
    SendTopicAction: {
      Action: Action.UNSUBSCRIBE,
      TopicID: Topic.MARKET_DETAIL,
      Index: `${symbol.ExchangeID}_${symbol?.InstrumentID}`,
      ResumeNo: -1
    }
  })
}

export function subscribeKLine (ws, symbol, interval) {
  ws.send({
    SendTopicAction: {
      Action: Action.SUBSCRIBE,
      TopicID: Topic.K_LINE,
      Index: `${symbol.exchangeId}_${symbol.symbolId}_${interval}`,
      ResumeNo: -1
    }
  })
}

export function unsubscribeKLine (ws, symbol, interval) {
  ws.send({
    SendTopicAction: {
      Action: Action.UNSUBSCRIBE,
      TopicID: Topic.K_LINE,
      Index: `${symbol.exchangeId}_${symbol.symbolId}_${interval}`,
      ResumeNo: -1
    }
  })
}

export function subscribeOrderBook (ws, symbol, tick) {
  ws.send({
    RequestNo: Topic.MARKET_DEPTH_25,
    SendTopicAction: {
      Action: Action.SUBSCRIBE,
      TopicID: Topic.MARKET_DEPTH_25,
      Index: `${symbol.ExchangeID}_${symbol?.InstrumentID}_${+tick >= 1 ? `${tick}.0` : tick}`,
      ResumeNo: -2
    }
  })
}

export function unsubscribeOrderBook (ws, symbol, tick) {
  ws.send({
    RequestNo: Topic.MARKET_DEPTH_25,
    SendTopicAction: {
      Action: Action.UNSUBSCRIBE,
      TopicID: Topic.MARKET_DEPTH_25,
      Index: `${symbol.ExchangeID}_${symbol?.InstrumentID}_${+tick >= 1 ? `${tick}.0` : tick}`,
      ResumeNo: -2
    }
  })
}

export function subscribeDepth (ws, symbol) {
  ws.send({
    RequestNo: Topic.MARKET_DEPTH_ALL,
    SendTopicAction: {
      Action: Action.SUBSCRIBE,
      TopicID: Topic.MARKET_DEPTH_ALL,
      Index: `${symbol.ExchangeID}_${symbol?.InstrumentID}`,
      ResumeNo: -1
    }
  })
}

export function unsubscribeDepth (ws, symbol) {
  ws.send({
    RequestNo: Topic.MARKET_DEPTH_ALL,
    SendTopicAction: {
      Action: Action.UNSUBSCRIBE,
      TopicID: Topic.MARKET_DEPTH_ALL,
      Index: `${symbol.ExchangeID}_${symbol?.InstrumentID}`,
      ResumeNo: -1
    }
  })
}

export function subscribeRealDeal (ws, symbol) {
  ws.send({
    SendTopicAction: {
      Action: Action.SUBSCRIBE,
      TopicID: Topic.TRADE_DETAIL,
      Index: `${symbol.ExchangeID}_${symbol?.InstrumentID}`,
      ResumeNo: -18
    }
  })
}

export function unsubscribeRealDeal (ws, symbol) {
  ws.send({
    SendTopicAction: {
      Action: Action.UNSUBSCRIBE,
      TopicID: Topic.TRADE_DETAIL,
      Index: `${symbol.ExchangeID}_${symbol?.InstrumentID}`,
      ResumeNo: -18
    }
  })
}
