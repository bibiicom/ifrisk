import { post } from '../network/http'

/**
 * 获取卡片列表
 * @param {Object} params - 请求参数
 * @param {number} params.page - 页码
 * @param {number} params.limit - 每页数量
 * @param {number} params.type - 类型: 0-全部, 1-交易中, 2-申购中, 3-待上架
 * @returns {Promise}
 */
export function getCardList(params) {
    return post('/backend/v1/vault/getCardList', params)
}

/**
 * 获取卡片详情
 * @param {Object} params - 请求参数
 * @param {string|number} params.id - 卡片ID
 * @returns {Promise}
 */
export function getCardDetail(params) {
    return post('/backend/v1/vault/getCardDetail', params)
}
