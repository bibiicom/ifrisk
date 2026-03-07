import React from 'react'
import intl from '@/component/intl'
import CardDetails from './CardDetails'
import OfferingRules from './OfferingRules'
import SubscriptionHistory from './SubscriptionHistory'
import FAQSection from './FAQSection'
import styles from './index.module.less'

const getTabs = () => [
    intl.get('launchpad2_card_details'),
    intl.get('launchpad2_offering_rules'),
    intl.get('launchpad2_subscription_history'),
    intl.get('launchpad2_faq')
]

const DetailsTabs = ({ detail, activeTab, onTabChange }) => {
    const TABS = getTabs()

    const renderContent = () => {
        switch (activeTab) {
            case intl.get('launchpad2_card_details'):
                return <CardDetails detail={detail} />
            case intl.get('launchpad2_offering_rules'):
                return <OfferingRules detail={detail} />
            case intl.get('launchpad2_subscription_history'):
                return <SubscriptionHistory detail={detail} />
            case intl.get('launchpad2_faq'):
                return <FAQSection />
            default:
                return null
        }
    }

    return (
        <div className={styles.tabs}>
            <div className={styles.tabList}>
                {TABS.map(tab => (
                    <button
                        key={tab}
                        className={`${styles.tab} ${activeTab === tab ? styles.active : ''}`}
                        onClick={() => onTabChange(tab)}
                    >
                        {tab}
                    </button>
                ))}
            </div>
            <div className={styles.content}>
                {renderContent()}
            </div>
        </div>
    )
}

export default DetailsTabs
