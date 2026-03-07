import { get, post } from '../network/http'
import { Direction as PositionDirection } from '../constants/position'
import { Direction as OrderDirection } from '../constants/order'

/**
 * 获取持仓
 * @returns {Promise}
 */
export function getPosition (params, config) {
  return new Promise((resolve, reject) => {
    const requestConfig = { ...config }
    requestConfig.headers = { ...requestConfig.headers, Zip: 2 }
    get('private/v1.0/SendQryPosition', params, requestConfig).then(
      result => {
        (result || []).forEach(position => {
          let isBuy
          if (position.PosiDirection === PositionDirection.NET) {
            isBuy = position.Position >= 0
          } else {
            isBuy = position.PosiDirection === PositionDirection.LONG
          }
          position.Position = Math.abs(position.Position)
          if (isBuy) {
            position.ClosePosition = position.Position - position.ShortFrozen
          } else {
            position.ClosePosition = position.Position - position.LongFrozen
          }
          position.ClosePosition = Math.max(position.ClosePosition, 0)
          position.Direction = isBuy ? OrderDirection.BUY : OrderDirection.SELL
        })
        resolve(result)
      }
    ).catch(e => { reject(e) })
  })
}

/**
 * 改变持仓类型
 * @param params
 * @returns
 */
export function changePositionType (params) {
  return post('action/v1.0/SendMemberAction', params)
}

/**
 * 改变仓位保证金模式
 * @param params
 * @return {Promise}
 */
export function changePositionMarginMode (params) {
  return post('action/v1.0/SendPositionAction', { ...params, ActionType: '4' })
}

/**
 * 调整杠杆
 * @param params
 * @return {Promise}
 */
export function adjustLever (params) {
  return post('action/v1.0/SendPositionAction', { ...params, ActionType: '1' })
}

/**
 * 增加保证金
 * @param params
 * @return {Promise}
 */
export function increaseMargin (params) {
  return post('action/v1.0/SendPositionAction', { ...params, ActionType: '2' })
}

/**
 * 减少保证金
 * @param params
 * @return {Promise}
 */
export function reduceMargin (params) {
  return post('action/v1.0/SendPositionAction', { ...params, ActionType: '3' })
}
