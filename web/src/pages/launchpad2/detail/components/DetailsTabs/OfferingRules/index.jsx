import React from 'react'
import styles from './index.module.less'
import Timeline from './Timeline'
import ActivityRules from './ActivityRules'


// OfferingRules 主组件
const OfferingRules = ({ detail }) => {
    const { launchpadInfo, cardInfo } = detail || {}
    
    // 合并数据，用于传递给 ActivityRules 组件
    const mergedData = {
        ...launchpadInfo,
        Name: cardInfo?.Name || launchpadInfo?.Name || ''
    }

    return (
        <div className={styles.offeringRules}>
            <Timeline info={launchpadInfo} />
            <ActivityRules data={mergedData} />
        </div>
    )
}

export default OfferingRules
