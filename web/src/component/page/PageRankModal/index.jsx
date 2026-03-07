import React, { useEffect, useState } from 'react'
import { Empty, Spin, Tooltip, App } from 'antd'
import intl from '@/component/intl'
import { fetchPositionRank } from '../../../api/buyback'
import { sortByField } from '../../../utils/sort'
import CommonModal from '../../commonModal'
import styles from './index.module.less'

const PageRankModal = ({ open, onClose, currency }) => {
  const { message } = App.useApp()
  const [loading, setLoading] = useState(false)
  const [rankList, setRankList] = useState([])
  const [total, setTotal] = useState(0)



  const formatNumber = (val, decimals) => {
    const num = Number(val)
    if (!Number.isFinite(num)) return '--'
    const fixed = num.toFixed(decimals)
    return fixed.replace(/\.0+$/, '').replace(/(\.[0-9]*?)0+$/, '$1')
  }

  useEffect(() => {
    const load = async () => {
      if (!currency) return
      setLoading(true)
      try {
        const res = await fetchPositionRank({ currency })
        const list = (res && res.data && res.data.rankList) || []
        const totalBalance = Number(res?.data?.total) || 0
        setRankList(sortByField(list, 'balance', 'desc'))
        setTotal(totalBalance)
      } catch (e) {
        setRankList([])
        setTotal(0)
        message.error(e?.errorMsg || e?.msg || intl.get('buyback_load_rank_failed'))
      } finally {
        setLoading(false)
      }
    }

    if (open) {
      load()
    }
  }, [open, currency, message])

  return (
    <CommonModal
      title={`${currency} ${intl.get('buyback_rank_title')}`}
      open={open}
      onCancel={onClose}
      afterClose={() => {
        setLoading(false)
        setRankList([])
        setTotal(0)
      }}
      footer={null}
      width={720}
      className={styles.pageRankModal}
    >
      <div className={styles.panelBody}>
        <Spin spinning={loading}>
          {!loading && rankList.length === 0 ? (
            <div className={styles.emptyWrapper}>
              <Empty description={intl.get('buyback_no_data')} />
            </div>
          ) : (
            <div className={styles.rankingList}>
              <div className={styles.rankingHeader}>
                <span>{intl.get('buyback_rank_header_rank')}</span>
                <span>{intl.get('buyback_rank_header_address')}</span>
                <span>{intl.get('buyback_rank_header_amount')}</span>
                <span>{intl.get('buyback_rank_header_percentage')}</span>
              </div>

              {rankList.map((item, idx) => {
                const addr = item?.accountId || ''
                const showAddr =
                  addr.length > 14 ? `${addr.slice(0, 10)}...${addr.slice(-12)}` : addr

                const amount = Number(item?.balance) || 0
                const percentage = total > 0 ? (amount / total) * 100 : 0

                return (
                  <div
                    key={`${addr}-${idx}`}
                    className={styles.rankingItem}
                  >
                    <span className={styles.rank}>{idx + 1}</span>
                    <span className={styles.address}>
                      <Tooltip title={addr}>{showAddr}</Tooltip>
                    </span>
                    <span className={styles.amount}>{formatNumber(amount, 8)}</span>
                    <span className={styles.percentage}>{formatNumber(percentage, 3)}%</span>
                  </div>
                )
              })}
            </div>
          )}
        </Spin>
      </div>
    </CommonModal>
  )
}

export default PageRankModal
