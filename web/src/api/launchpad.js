import { get, post } from '../network/http'

// 获取 Launchpad 信息列表
export function fetchLaunchpadInfo(params = {}) {
  return get('/backend/v1/launchpad/getLaunchpadInfo', params)
}

// 根据 ID 获取单个 Launchpad 项目信息
export function fetchLaunchpadInfoById(params = {}) {
  return get('/backend/v1/launchpad/getLaunchpadInfoById', params)
}

// 新的 Launchpad 申购接口
export function launchpadSubscription(params = {}) {
  return post('/backend/v1/launchpad/subscription', params)
}

// 获取认购记录
export function fetchSubscriptionInfo(params = {}) {
  return post('/backend/v1/launchpad/subscriptionInfo', params)
}

// 获取 Future Launchpad 信息列表
export function fetchFutureLaunchpadInfo(params = {}) {
  return post('/backend/v1/launchpad/getFutureLaunchpadInfo', params)
}

// 获取 End Launchpad 信息列表
export function fetchEndLaunchpadInfo(params = {}) {
  return post('/backend/v1/launchpad/getEndLaunchpadInfo', params)
}

// 获取 Live Launchpad 信息列表
export function fetchLiveLaunchpadInfo(params = {}) {
  return post('/backend/v1/launchpad/getLiveLaunchpadInfo', params)
}