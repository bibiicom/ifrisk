// 回购页面：展示我的持有列表 + 我的持有详情 + 价格走势
import React, { useState, useEffect, useContext, useMemo, useCallback, useRef } from 'react'
import { Spin, App, Button } from 'antd'
import { useLocation } from 'react-router-dom'
import intl from '@/component/intl'
import RootContext from '../rootContext'
import { Status } from '../../constants/auth'
import authHOC from '../common/hoc/authHOC'
import EmptyState from '@/component/emptyState'
import Rank from './component/rank'
import Holding from './component/holding'
import Price from './component/price'
import HistoryBuybackModal from './component/HistoryBuybackTable'
import { fetchCurrencyRank } from '../../api/buyback'
import { sortByField } from '../../utils/sort'
import styles from './index.module.less'



// 回购主组件
const Buyback = () => {
  const { message } = App.useApp()
  const { authStatus } = useContext(RootContext)
  const location = useLocation()
  // 页面整体 loading
  const [loading, setLoading] = useState(false)
  // 排行榜渲染用的数据
  const [rankings, setRankings] = useState([])
  // 当前选中的币种数据（用于右侧 Holding 和 Price 组件）
  const [selectedCurrency, setSelectedCurrency] = useState(null)


  const loadSeqRef = useRef(0)
  const selectedInstrumentIDRef = useRef('')
  const appliedQueryRef = useRef(false)

  const queryInstrumentID = useMemo(() => {
    const search = location?.search || ''
    if (!search) return ''
    const sp = new URLSearchParams(search)
    return sp.get('instrumentID') || ''
  }, [location?.search])

  useEffect(() => {
    selectedInstrumentIDRef.current = selectedCurrency?.instrumentID || ''
  }, [selectedCurrency?.instrumentID])

  useEffect(() => {
    appliedQueryRef.current = false
  }, [queryInstrumentID])

  // 加载用户持有的所有币种数据
  const loadRankings = useCallback(async ({ resetSelected = false } = {}) => {
    const seq = ++loadSeqRef.current
    setLoading(true)
    try {
      const res = await fetchCurrencyRank()
      if (seq !== loadSeqRef.current) return

      const rankList = (res && res.data) || []

      // 将接口返回的数据映射为展示结构
      const mappedRankings = rankList.map((item) => {
        const balance = Number(item.balance) || 0
        const totalBalance = Number(item.totalBalance) || 0
        const percentage = totalBalance > 0 ? (balance / totalBalance) * 100 : 0

        return {
          currency: item.currency,
          amount: item.balance,
          percentage: Number(percentage.toFixed(3)),
          instrumentID: item.instrumentID,
          totalBalance: item.totalBalance,
          remark: item.remark || '' // 添加 remark 字段
        }
      })

      const sortedRankings = sortByField(mappedRankings, 'percentage', 'desc').map((item, index) => ({
        ...item,
        rank: index + 1
      }))

      setRankings(sortedRankings)

      // 优先根据 url 参数选中对应交易对，其次默认选中第一个币种
      if (sortedRankings.length > 0) {
        if (resetSelected) {
          setSelectedCurrency(sortedRankings[0])
          appliedQueryRef.current = true
          return
        }

        const currentSelectedInstrumentID = selectedInstrumentIDRef.current
        const shouldApplyQuery = Boolean(queryInstrumentID) && !appliedQueryRef.current

        const matchByQuery = shouldApplyQuery
          ? sortedRankings.find((r) => String(r.instrumentID) === String(queryInstrumentID))
          : null

        const matchByCurrent = !matchByQuery && currentSelectedInstrumentID
          ? sortedRankings.find((r) => String(r.instrumentID) === String(currentSelectedInstrumentID))
          : null

        const match = matchByQuery || matchByCurrent

        if (match) {
          setSelectedCurrency(match)
        } else {
          setSelectedCurrency(sortedRankings[0])
        }

        if (shouldApplyQuery) {
          appliedQueryRef.current = true
        }
      }
    } catch (error) {
      message.error(intl.get('buyback_loading_failed'))
    } finally {
      if (seq === loadSeqRef.current) {
        setLoading(false)
      }
    }
  }, [queryInstrumentID])

  useEffect(() => {
    if (authStatus !== Status.LOGIN) {
      return
    }
    loadRankings()
    return () => {
      loadSeqRef.current++
    }
  }, [authStatus, loadRankings])

  if (authStatus !== Status.LOGIN) {
    return <EmptyState />
  }

  return (
    <div className={styles['buyback-page']}>
      <div className={styles['buyback-header']}>
        <h1>{intl.get('buyback_page_title')}</h1>
        <div style={{display:'flex',justifyContent:'space-between'}}>
          {intl.get('buyback_page_subtitle')}
          <HistoryBuybackModal />
        </div>
      </div>

      <Spin spinning={loading} size="large">
        <div className={styles['buyback-content']}>
          {/* 左列：我的持有 */}
          <div className={styles['buyback-left-column']}>
            <Rank
              rankings={rankings}
              onSelect={setSelectedCurrency}
              selectedRank={selectedCurrency?.rank}
            />
          </div>

          {/* 右列：上 我的持有，下 价格走势 */}
          <div className={styles['buyback-right-column']}>
            {selectedCurrency && (
              <>
                <Holding
                  selectedCoin={selectedCurrency.currency}
                  balance={selectedCurrency.amount}
                  total={selectedCurrency.totalBalance}
                  instrumentID={selectedCurrency.instrumentID}
                  onRefresh={loadRankings}
                />

                <Price instrumentID={selectedCurrency.instrumentID} />
              </>
            )}
          </div>
        </div>
      </Spin>


    </div>
  )
}

export default authHOC(true)(Buyback)
