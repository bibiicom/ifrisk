import React from 'react'
import InformationList from '../InformationList'
import authHOC from '../../common/hoc/authHOC'

const ANNOUNCEMENT_SUB_TYPES = [
    { key: '200', labelKey: 'announcement_card_online' },
    { key: '201', labelKey: 'announcement_card_offline' },
]

const Announcement = () => {
    return (
        <InformationList
            type={2}
            subTypeList={ANNOUNCEMENT_SUB_TYPES}
            titleKey="announcement_title"
        />
    )
}

export default authHOC(true)(Announcement)
