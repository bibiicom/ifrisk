import { post } from '../network/http'

export function getAnnouncement(params, config) {
    return post('backend/v1/announcement/getAnnouncement', params, config)
}

export function getAnnouncementDetail(params, config) {
    return post('backend/v1/announcement/getAnnouncementDetail', params, config)
}
