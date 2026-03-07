import React from 'react'
import styles from './index.module.less'
import intl from '@/component/intl'

const TradingStatus = ({ status }) => {
  if (!status) return null

  // 根据状态码获取配置
  const getStatusConfig = (statusCode) => {
    switch (statusCode) {
      case '1':
        return {
          text: intl.get('trading_status_suspended'),
          type: 'halted',
          icon: '⏸'
        }
      case '2':
        return {
          text: intl.get('trading_status_trading'),
          type: 'trading',
          icon: '●'
        }
      case '7':
        return {
          text: intl.get('trading_status_settlement'),
          type: 'settlement',
          icon: '⏳'
        }
      default:
        return null
    }
  }

  const config = getStatusConfig(status)
  if (!config) return null

  return (
    <div className={`${styles.statusBadge} ${styles[config.type]}`}>
      <span className={styles.icon}>{config.icon}</span>
      <span className={styles.text}>{config.text}</span>
    </div>
  )
}

export default TradingStatus
