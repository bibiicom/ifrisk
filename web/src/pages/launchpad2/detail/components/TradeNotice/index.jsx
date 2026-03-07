import React from 'react'
import { CheckCircleFilled } from '@ant-design/icons'
import intl from '@/component/intl'
import styles from './index.module.less'
import dayjs from 'dayjs'
import utc from 'dayjs/plugin/utc'

dayjs.extend(utc)

const TradeNotice = ({ tradeTime, onTradeClick }) => {
    // 格式化交易时间

    return (
        <div className={styles.tradeNotice}>
            <div className={styles.tradeNoticeContent}>
                <div className={styles.tradeNoticeLeft}>
                    <CheckCircleFilled className={styles.tradeNoticeIcon} />
                    <div className={styles.tradeNoticeInfo}>
                        <span className={styles.tradeNoticeTitle}>
                            {intl.get('launchpad2_sale_ended')}
                        </span>
                        <span className={styles.tradeNoticeText}>
                            {intl.get('launchpad2_trade_on_market')} {dayjs(tradeTime).utc().format('YYYY-MM-DD HH:mm:ss')} UTC
                        </span>
                    </div>
                </div>
                <button className={styles.tradeBtn} onClick={onTradeClick}>
                    {intl.get('launchpad2_trade')}
                    <span className={styles.tradeArrow}>→</span>

                </button>
            </div>
        </div>
    )
}

export default TradeNotice
