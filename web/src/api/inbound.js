import { post } from '../network/http'

export function storageApply (params, config) {
  return post('backend/v1/card/storageApply', params, config)
}
