import { post } from '../network/http'

/**
 * 入库申请
 */
export function storageApply(params, config) {
  return post('backend/v1/card/storageApply', params, config)
}

/**
 * 获取实体卡片列表
 */
export function getEntityCardList(params, config) {
  return post('backend/v1/card/getCardList', params, config)
}

/**
 * 出库申请
 */
export function outboundApply(params, config) {
  return post('backend/v1/card/outboundApply', params, config)
}

/**
 * 获取入库申请列表
 */
export function getStorageApplyList(params, config) {
  return post('/backend/v1/card/getStorageApplyList', params, config)
}

/**
 * 获取出库申请列表
 */
export function getOutboundApplyList(params, config) {
  return post('/backend/v1/card/getOutboundApplyList', params, config)
}
