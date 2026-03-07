import React from 'react'
import InformationList from '../InformationList'
import authHOC from '../../common/hoc/authHOC'

const NEWS_SUB_TYPES = [
    { key: '100', labelKey: 'announcement_news_industry' },
]

const News = () => {
    return (
        <InformationList
            type={1}
            subTypeList={NEWS_SUB_TYPES}
            titleKey="announcement_news"
        />
    )
}

export default authHOC(true)(News)
