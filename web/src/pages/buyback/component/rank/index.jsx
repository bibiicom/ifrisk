// 我的持有列表组件:展示当前用户持有的所有币种及占比
import React, { useState } from 'react'
import { WalletOutlined } from '@ant-design/icons'
import { Tooltip } from 'antd'
import intl from '@/component/intl'
import PageRankModal from '@/component/page/PageRankModal'
import styles from './index.module.less'

// rankings：父组件已经计算好 rank/currency/amount/percentage 的列表
// onSelect：点击某一行时的回调函数
// selectedRank：当前选中的排名
const Rank = ({ rankings, onSelect, selectedRank }) => {
  const [rankModalOpen, setRankModalOpen] = useState(false)
  const [rankCurrency, setRankCurrency] = useState('')

  const handleRankClick = (e, currency) => {
    e.stopPropagation()
    setRankCurrency(currency)
    setRankModalOpen(true)
  }

  return (
    <div className={`${styles['buyback-panel']} ${styles['ranking-panel']}`}>
      <div className={styles['panel-header']}>
        <WalletOutlined />
        <span>{intl.get('buyback_my_holding_title')}</span>
      </div>
      <div className={styles['panel-body']}>
        <div className={styles['ranking-list']}>
          {/* 表头：排名 / 币种 / 数量 / 占比 / 操作 */}
          <div className={styles['ranking-header']}>
            <span>{intl.get('buyback_rank_header_rank')}</span>
            <span>{intl.get('buyback_rank_header_currency')}</span>
            <span>{intl.get('buyback_rank_header_amount')}</span>
            <span>{intl.get('buyback_holding_ratio')}</span>
            <span>{intl.get('buyback_rank_header_all_rank')}</span>
          </div>
          {rankings.map((item) => {
            return (
              <div
                key={item.rank}
                className={`${styles['ranking-item']} ${selectedRank === item.rank ? styles['selected'] : ''}`}
                onClick={() => onSelect && onSelect(item)}
                style={{ cursor: onSelect ? 'pointer' : 'default' }}
              >
                <span className={styles['rank']}>{item.rank}</span>
                <Tooltip title={item?.remark?.replace(/_/g, ' ') || ''}>
                  <span className={styles['currency']}>{item.currency}</span>
                </Tooltip>
                <span className={styles['amount']}>{item.amount.toFixed(2)}</span>
                <span className={styles['percentage']}>{item.percentage}%</span>
                <span className={styles['allRank']}>
                  <span
                    className={styles['rankLink']}
                    onClick={(e) => handleRankClick(e, item.currency)}
                  >
                    {intl.get('buyback_rank_link')}
                  </span>
                </span>
              </div>
            )
          })}
        </div>
      </div>

      <PageRankModal
        open={rankModalOpen}
        onClose={() => setRankModalOpen(false)}
        currency={rankCurrency}
      />
    </div>
  )
}

export default Rank
