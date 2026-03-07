import React, { useState, useEffect, useRef, useContext } from 'react'
import authHOC from '../../common/hoc/authHOC'
import RootContext from '../../rootContext'
import { getPubilc } from '../../../network/pubilcHttp'
import merge from 'lodash.merge'
import Echarts from '../../../component/echarts/Echarts'
import './index.less'

const COLORS = ['#4caf50', '#2196f3', '#ff9800', '#9c27b0', '#f44336', '#00bcd4']

// ------------------ 饼图组件 ------------------
function PieChart({ tokens, highlightIndex, setHighlightIndex, mousePos, setMousePos, size = 120 }) {
  const totalValue = tokens.reduce((sum, t) => sum + t.value, 0)
  const radius = size / 2 - 2 // 留2px边距
  const cx = size / 2
  const cy = size / 2

  let cumulativeAngle = 0
  const pieSlices = tokens.map((t, i) => {
    const startAngle = cumulativeAngle
    const angle = (t.value / totalValue) * 2 * Math.PI
    cumulativeAngle += angle
    const endAngle = cumulativeAngle

    const x1 = cx + radius * Math.cos(startAngle)
    const y1 = cy + radius * Math.sin(startAngle)
    const x2 = cx + radius * Math.cos(endAngle)
    const y2 = cy + radius * Math.sin(endAngle)
    const largeArc = angle > Math.PI ? 1 : 0

    return {
      d: `M${cx},${cy} L${x1},${y1} A${radius},${radius} 0 ${largeArc},1 ${x2},${y2} Z`,
      color: COLORS[i % COLORS.length],
      label: t.symbol
    }
  })

  return (
    <div style={{ width: size, height: size, position: 'relative' }}>
      <svg viewBox={`0 0 ${size} ${size}`} width="100%" height="100%">
        {pieSlices.map((slice, i) => (
          <path
            key={i}
            d={slice.d}
            fill={highlightIndex === i ? '#ffeb3b' : slice.color}
            stroke="#fff"
            strokeWidth="1"
            onMouseEnter={e => {
              setHighlightIndex(i)
              setMousePos({ x: e.clientX, y: e.clientY })
            }}
            onMouseMove={e => setMousePos({ x: e.clientX, y: e.clientY })}
            onMouseLeave={() => setHighlightIndex(null)}
          />
        ))}
      </svg>

      {highlightIndex !== null && mousePos && (
        <div
          style={{
            position: 'fixed',
            top: mousePos.y - 30,
            left: mousePos.x + 10,
            background: 'rgba(0,0,0,0.7)',
            color: '#fff',
            padding: '4px 8px',
            borderRadius: 4,
            whiteSpace: 'nowrap',
            pointerEvents: 'none',
            zIndex: 1000
          }}
        >
          {`${tokens[highlightIndex].symbol}: ${tokens[highlightIndex].amount} (${(
            (tokens[highlightIndex].value / totalValue) * 100
          ).toFixed(2)}%)`}
        </div>
      )}
    </div>
  )
}

// 通用折线图：接收 data 数组和可选 option，内部生成基础配置
function LineChartECharts({ data = [], option, ...other }) {
  const formatXAxis = (val) => {
    const date = new Date(val)
    const month = date.getMonth() + 1
    const day = date.getDate()
    const year = date.getFullYear()
    return month === 1 ? `${month}/${day}\n${year}` : `${month}/${day}`
  }

  const values = data.map(h => h.value)

  const baseOption = {
    animation: false, // 全局关闭动画
    tooltip: {
      trigger: 'axis',
      formatter: (params) => {
        const p = params[0]
        return `${p.axisValueLabel}: ${p.data}`
      },
      axisPointer: {
        type: 'cross',
        label: { backgroundColor: '#6a7985' }
      }
    },
    grid: {
      left: 60,
      right: 20,
      top: 20,
      bottom: 50
    },
    xAxis: {
      type: 'category',
      data: data.map(h => h.date),
      axisLabel: { formatter: formatXAxis },
      boundaryGap: false,
      splitLine: { show: false }
    },
    yAxis: {
      type: 'value',
      min: 'dataMin',
      max: 'dataMax',
      axisLabel: {
        formatter: (val) => val.toString() // 完整显示数字
      },
      splitLine: { show: false }
    },
    series: [
      {
        data: values,
        type: 'line',
        smooth: true,
        symbol: 'circle',
        symbolSize: 6,
        lineStyle: { color: '#2196f3' },
        itemStyle: { color: '#2196f3' },
        animation: false, // 系列单独关闭动画
      },
    ],
  }

  const mergedOption = merge({}, baseOption, option || {})

  return <Echarts option={mergedOption} height={200} {...other} />
}

// ------------------ 资产表格 ------------------
function AssetTable({ tokens, highlightIndex, setHighlightIndex }) {
  const totalValue = tokens.reduce((sum, t) => sum + t.value, 0)
  return (
    <table border="1" cellPadding="5" style={{ marginTop: 20, width: '100%', borderCollapse: 'separate', borderRadius: '6px', overflow: 'hidden', borderSpacing: '0' }}>
      <thead>
        <tr>
          <th>币种</th>
          <th>数量</th>
          <th>价值 (USD)</th>
          <th>贷/存利息</th>
          <th>贷/存比</th>
          <th>占比</th>
        </tr>
      </thead>
      <tbody>
        {tokens.map((t, i) => (
          <tr
            key={t.symbol}
            onMouseEnter={() => setHighlightIndex(i)}
            onMouseLeave={() => setHighlightIndex(null)}
            style={{
              cursor: 'pointer',
              backgroundColor: highlightIndex === i ? 'rgba(255,235,59,0.2)' : 'transparent'
            }}
          >
            <td>{t.symbol}</td>
            <td>
              {Number(t.amount) < 1000
                ? Number(t.amount).toFixed(4)
                : Number(t.amount).toFixed(0)}
            </td>
            <td>{t.value.toFixed(0)}</td>
            <td>{t.LD.toFixed(2)}/{t.LB.toFixed(2)}</td>
            <td>{((t.LU/t.B)*100).toFixed(1)}%</td>
            <td style={{ color: COLORS[i % COLORS.length] }}>{((t.value / totalValue) * 100).toFixed(1)}%</td>
          </tr>
        ))}
      </tbody>
    </table>
  )
}

// ------------------ 主组件 ------------------
export default authHOC(true)(() => {
  const { userInfo } = useContext(RootContext)
  const containerRef = useRef(null)
  const [containerWidth, setContainerWidth] = useState(600)
  const [pieSize, setPieSize] = useState(120)
  const [highlightIndex, setHighlightIndex] = useState(null)
  const [hoverIndex, setHoverIndex] = useState(null)
  const [mousePos, setMousePos] = useState(null)
  const [currencyList, setCurrencyList] = useState([])
  const [historyData, setHistoryData] = useState([])

  // ------------------ 获取币种数据 ------------------
  useEffect(() => {
    const request = async () => {
      try {
        const requestConfig = { headers: { Zip: 3 } }
        const res = await getPubilc('public/v1.0/SendQryCurrency', { All: 'All' }, requestConfig)
        const mapped = res.map(item => ({
          symbol: item.C,
          amount: item.B,
          value: item.V,
          LB: item.LB,
          LD: item.LD,
          LU: item.LU
        }))
        setCurrencyList(mapped)
      } catch (e) {
        console.error('获取币种列表失败', e)
      }
    }
    request()
  }, [])

  // ------------------ 获取历史文件 ------------------
  useEffect(() => {
    const fetchHistoryData = async () => {
      try {
        const res = await fetch('http://bibii.com/history/currency.csv')
        const text = await res.text()
        // 直接解析为 {date, value}
        const data = text
          .trim()
          .split('\n')
          .map(line => {
            const [date, value] = line.split(',')
            return { date, value: parseFloat(value) }
          })
        // console.log('data', data)
        setHistoryData(data)
      } catch (e) {
        console.error('获取历史数据失败', e)
      }
    }
    fetchHistoryData()
  }, [])

  // 计算总资产
  const totalUSD = currencyList.reduce((sum, t) => sum + t.value, 0).toFixed(2)

  useEffect(() => {
    const handleResize = () => {
      if (containerRef.current) {
        const width = containerRef.current.offsetWidth
        setContainerWidth(width)
        setPieSize(Math.min(120, width / 4))
      }
    }
    handleResize()
    window.addEventListener('resize', handleResize)
    return () => window.removeEventListener('resize', handleResize)
  }, [])

  return (
    <div className="reserve">
      <section ref={containerRef} style={{ flex: 1, padding: 20, overflowY: 'auto' }}>
        <div style={{ display: 'flex', alignItems: 'center', marginBottom: 20 }}>
          <h2 style={{ marginRight: 40 }}>总资产: {totalUSD} USD</h2>
          <PieChart
            tokens={currencyList}
            highlightIndex={highlightIndex}
            setHighlightIndex={setHighlightIndex}
            mousePos={mousePos}
            setMousePos={setMousePos}
            size={pieSize}
          />
        </div>

        <AssetTable tokens={currencyList} highlightIndex={highlightIndex} setHighlightIndex={setHighlightIndex} />

        <h3 style={{ marginTop: 40 }}>总资产变化历史</h3>
        <LineChartECharts data={historyData} style={{ width: '100%' }} />
      </section>
    </div>
  )
})
