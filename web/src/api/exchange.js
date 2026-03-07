import { post } from '../network/http'
/**
 * 根据交易对获取卡片信息
 */
export function getCardByInstrument(params, config) {
  return post('backend/v1/card/getCardByInstrument', params, config)
}
