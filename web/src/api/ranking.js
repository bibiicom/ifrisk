import { get } from '../network/http'

export function getHoldRank(params, config) {
  return get('backend/v1/rank/holdRank', params, config)
}
