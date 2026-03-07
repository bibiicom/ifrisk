import React, { useEffect, useState, useMemo, useRef } from 'react'
import { Spin, App } from 'antd'
import { LineChartOutlined } from '@ant-design/icons'
import intl from '@/component/intl'
import BasicLineChart from '../../../../component/echarts/BasicLineChart'
import { fetchSevenDayPrice } from '../../../../api/buyback'
import styles from './index.module.less'
import { formatDecimals } from '@/utils/number'

// instrumentID：交易对 ID，例如 PikachuUSDT
const Price = ({ instrumentID }) => {
  const { message } = App.useApp()
  const [loading, setLoading] = useState(false)
  const [priceList, setPriceList] = useState([])
  const [avgPrice, setAvgPrice] = useState()
  const loadSeqRef = useRef(0)

  useEffect(() => {
    if (!instrumentID) return

    const seq = ++loadSeqRef.current
    setAvgPrice(undefined)
    setPriceList([])

    const loadPrice = async () => {
      setLoading(true)
      try {
        const res = await fetchSevenDayPrice({ instrumentID })
        if (seq !== loadSeqRef.current) return
        const list = (res && res.data && res.data.priceList) || []
        if (res?.data?.avgPrice) {
          setAvgPrice(res.data.avgPrice)
        }
        const mapped = list.map((item) => ({
          time: item.time,
          price: Number(item.price) || 0,
        }))
        setPriceList(mapped)
      } catch (e) {
        message.error(intl.get('buyback_load_price_failed'))
      } finally {
        if (seq === loadSeqRef.current) {
          setLoading(false)
        }
      }
    }

    loadPrice()

    return () => {
      loadSeqRef.current++
    }
  }, [instrumentID])

  const chartData = useMemo(() => {
    const sorted = [...(priceList || [])].sort((a, b) => Number(a.time) - Number(b.time))

    return sorted
      .map((item) => {
        const ts = Number(item.time)
        if (!ts) return null
        const d = new Date(ts)
        const month = String(d.getMonth() + 1).padStart(2, '0')
        const date = String(d.getDate()).padStart(2, '0')
        const hour = String(d.getHours()).padStart(2, '0')
        const minute = String(d.getMinutes()).padStart(2, '0')
        return {
          x: `${month}-${date} ${hour}:${minute}`,
          y: Number(item.price) || 0,
        }
      })
      .filter(Boolean)
  }, [priceList])

  return (
    <div
      className={`${styles['buyback-panel']} ${styles['chart-panel']} ${styles['buyback-chart-full']}`}
    >
      <div className={styles['panel-header']}>
        <LineChartOutlined />
        <span>{intl.get('buyback_price_trend_title')}</span>
      </div>
      <div className={styles['panel-body']}>
        <div className={styles['price-meta']}>
          <span className={styles['label']}>{intl.get('buyback_avg_price_label')}</span>
          <span className={styles['value']}>
            {avgPrice ? formatDecimals(avgPrice) : '-'}
          </span>
        </div>

        <div className={styles['price-chart']}>
          <div className={styles['chart-area']}>
            {loading ? (
              <div className={styles['chart-loading']}>
                <Spin size="small" />
              </div>
            ) : (
              <BasicLineChart
                data={chartData}
                extraOption={{
                  xAxis: {
                    axisLabel: {
                      formatter: (value) => (value ? String(value).slice(0, 5) : ''),
                    },
                  },
                }}
              />
            )}
          </div>
        </div>
      </div>
    </div>
  )
}

export default Price
