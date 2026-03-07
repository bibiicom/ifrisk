import React, { useState, useEffect, useRef, useContext, useMemo } from 'react'
import Decimal from 'decimal.js'
import authHOC from '../../common/hoc/authHOC'
import RootContext from '../../rootContext'
import usePolling from '@/component/hooks/usePolling'
import { getAssets } from '@/api/account'
import { getSymbolList } from '@/api/symbol'
import createWebsocket, { WebsocketStatus, WebsocketMessageType, WebsocketDataType } from '../../exchange/createWebsocket'
import { subscribeMarketDetail, unsubscribeMarketDetail } from '../../../api/contract'

import './index.less'

const COLORS = ['#4caf50', '#2196f3', '#ff9800', '#9c27b0', '#f44336', '#00bcd4']

// 模拟数据
const mockData = {
  total: 0,
  tokens: [],
  history: [
    { date: '08-30', value: 6200 },
    { date: '08-31', value: 6200 },
    { date: '09-01', value: 6200 },
    { date: '09-02', value: 6200 },
    { date: '09-03', value: 6300 },
    { date: '09-04', value: 6400 },
    { date: '09-05', value: 6500 },
    { date: '09-06', value: 6600 },
    { date: '09-07', value: 6500 },
    { date: '09-08', value: 6800 },
    { date: '09-09', value: 6900 },
    { date: '09-10', value: 5800 }
  ],
  tokenHis: [
    { time: '08-30 00:00:00', currency: 'USDT', value: 6200, last: 6200 },
    { time: '08-31 02:00:00', currency: 'USDT', value: 6200, last: 6200 },
    { time: '09-01 02:00:00', currency: 'USDT', value: 6200, last: 6200 },
    { time: '09-02 02:00:00', currency: 'USDT', value: 6200, last: 6200 },
    { time: '09-08 02:00:00', currency: 'USDT', value: 6200, last: 6800 },
    { time: '09-09 02:00:00', currency: 'USDT', value: 6200, last: 6900 },
    { time: '09-10 02:00:00', currency: 'USDT', value: 6200, last: 5800 }
  ]
}

const chainsList = ['Ethereum', 'Solana', 'Arbitrum', 'Tron', 'BinanceSmartChain']


// ------------------ 饼图组件 ------------------
function PieChart({ tokens, highlightIndex, setHighlightIndex, mousePos, setMousePos, size = 120 }) {
  if (!tokens || tokens.length === 0) {
    return null
  }

  const totalValue = tokens.reduce((sum, t) => sum + t.value, 0)
  const radius = size / 2 - 2 // 留2px边距
  const cx = size / 2
  const cy = size / 2

  if (totalValue <= 0) {
    return null
  }

  // 只有一个资产时，用整圆渲染，避免 SVG 弧线退化为一条线
  if (tokens.length === 1) {
    const color = COLORS[0]
    return (
      <div style={{ width: size, height: size, position: 'relative' }}>
        <svg viewBox={`0 0 ${size} ${size}`} width="100%" height="100%">
          <circle
            cx={cx}
            cy={cy}
            r={radius}
            fill={highlightIndex === 0 ? '#ffeb3b' : color}
            stroke="#fff"
            strokeWidth="1"
            onMouseEnter={e => {
              setHighlightIndex(0)
              setMousePos({ x: e.clientX, y: e.clientY })
            }}
            onMouseMove={e => setMousePos({ x: e.clientX, y: e.clientY })}
            onMouseLeave={() => setHighlightIndex(null)}
          />
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
            {`${tokens[0].symbol}: ${tokens[0].amount} (${(
              (tokens[0].value / totalValue) *
              100
            ).toFixed(2)}%)`}
          </div>
        )}
      </div>
    )
  }

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
            (tokens[highlightIndex].value / totalValue) *
            100
          ).toFixed(2)}%)`}
        </div>
      )}
    </div>
  )
}

// ------------------ 折线图组件 ------------------
function LineChart({ historyData, hoverIndex, setHoverIndex, width }) {
  const svgHeight = 150 // 增高
  const marginLeft = 40
  const marginTop = 20 // 上边余量
  const marginRight = 20 // 右边余量

  const pointsArray = historyData.map((h, i) => ({
    x: (i / (historyData.length - 1)) * (width - marginLeft - marginRight),
    y: h.value,
    value: h.value,
    date: h.date
  }))

  const minValue = Math.min(...historyData.map(h => h.value))
  const maxValue = Math.max(...historyData.map(h => h.value))
  const range = maxValue - minValue

  return (
    <div style={{ position: 'relative', width: '100%', overflowX: 'hidden' }}>
      <svg width="100%" height={svgHeight + 40} viewBox={`0 0 ${width} ${svgHeight + 40}`}>
        {/* y轴 */}
        <line
          x1={marginLeft}
          y1={marginTop}
          x2={marginLeft}
          y2={svgHeight}
          stroke="currentColor"
          strokeWidth="1"
        />
        {/* x轴 */}
        <line
          x1={marginLeft}
          y1={svgHeight}
          x2={width - marginRight}
          y2={svgHeight}
          stroke="currentColor"
          strokeWidth="1"
        />

        {/* y轴刻度和水平网格 */}
        {Array.from({ length: 6 }).map((_, i) => {
          const value = minValue + (i / 5) * range
          const y = svgHeight - ((value - minValue) / range) * (svgHeight - marginTop) // 留顶部余量
          return (
            <g key={i}>
              <line
                x1={marginLeft - 5}
                y1={y}
                x2={width - marginRight}
                y2={y}
                stroke="rgba(0,0,0,0.1)"
              />
              <text x={marginLeft - 10} y={y + 4} fontSize="12" textAnchor="end">
                {value.toFixed(0)}
              </text>
            </g>
          )
        })}

        {/* 折线 */}
        <polyline
          fill="none"
          stroke="currentColor"
          strokeWidth="2"
          points={pointsArray
            .map(
              p =>
                `${p.x + marginLeft},${svgHeight - ((p.value - minValue) / range) * (svgHeight - marginTop)}`
            )
            .join(' ')}
        />

        {/* 数据点 */}
        {pointsArray.map((p, i) => (
          <circle
            key={i}
            cx={p.x + marginLeft}
            cy={svgHeight - ((p.value - minValue) / range) * (svgHeight - marginTop)}
            r="5"
            fill="currentColor"
            onMouseEnter={() => setHoverIndex(i)}
            onMouseLeave={() => setHoverIndex(null)}
          />
        ))}

        {/* tooltip */}
        {hoverIndex !== null && (() => {
          const p = pointsArray[hoverIndex]
          const y = svgHeight - ((p.value - minValue) / range) * (svgHeight - marginTop)
          return (
            <g>
              <rect x={p.x + marginLeft - 25} y={y - 30} width="50" height="20" fill="rgba(0,0,0,0.7)" rx="4" ry="4" />
              <text x={p.x + marginLeft} y={y - 15} fontSize="12" textAnchor="middle" fill="#fff">
                {p.value.toFixed(2)}
              </text>
            </g>
          )
        })()}

        {/* x轴日期 */}
        {pointsArray.map((p, i) => (
          <text key={i} x={p.x + marginLeft} y={svgHeight + 15} fontSize="12" textAnchor="middle">
            {p.date}
          </text>
        ))}
      </svg>
    </div>
  )
}

// ------------------ 资产表格 ------------------
function AssetTable({ tokens, highlightIndex, setHighlightIndex }) {
  const totalValue = tokens.reduce((sum, t) => sum + t.value, 0)
  return (
    <table border="1" cellPadding="5" style={{ marginTop: 20, width: '100%', borderCollapse: 'separate', borderRadius: '6px', overflow: 'hidden', borderSpacing: '0', tableLayout: 'fixed' }}>
      <thead>
        <tr>
          <th style={{ width: '25%' }}>币种</th>
          <th style={{ width: '25%' }}>数量</th>
          <th style={{ width: '25%' }}>价值 (USD)</th>
          <th style={{ width: '25%' }}>比例</th>
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
            <td>{t.amount}</td>
            <td>{t.value}</td>
            <td style={{ color: COLORS[i % COLORS.length] }}>{((t.value / totalValue) * 100).toFixed(2)}%</td>
          </tr>
        ))}
      </tbody>
    </table>
  )
}

// ------------------ 历史表格 ------------------
function HistoryTable({ tokenHis }) {
  return (
    <table border="1" cellPadding="5" style={{ width: '100%', marginTop: 10, borderCollapse: 'separate', borderRadius: '6px', overflow: 'hidden', borderSpacing: '0' }}>
      <thead>
        <tr>
          <th>时间</th>
          <th>币种</th>
          <th>变化</th>
          <th>剩余资产</th>
        </tr>
      </thead>
      <tbody>
        {tokenHis.map((h, i) => (
          <tr key={i}>
            <td>{h.time}</td>
            <td>{h.currency}</td>
            <td>{h.value}</td>
            <td>{h.last.toFixed(2)}</td>
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

  const { result: assetsResult = [] } = usePolling(
    {
      api: getAssets,
      params: {
        AccountID: userInfo?.MemberID
      },
      flag: userInfo && userInfo.MemberID,
      interval: 10000
    },
    [(userInfo || {}).MemberID]
  )

  // WebSocket 市场价格状态
  const [marketPrices, setMarketPrices] = useState({})
  const [ws, setWs] = useState(null)
  const [wsConnected, setWsConnected] = useState(false)
  const [symbols, setSymbols] = useState({})
  const subscribedSymbolsRef = useRef(new Set())

  // 创建WebSocket连接（只创建一次）
  useEffect(() => {
    const websocket = createWebsocket()
    
    // 监听连接状态
    websocket.on(WebsocketMessageType.STATUS, 'wallet', (status) => {
      setWsConnected(status === WebsocketStatus.CONNECT)
    })

    // 监听市场详情数据
    websocket.on(WebsocketMessageType.MARKET_DETAIL, 'wallet', (data) => {
      if (data.type === WebsocketDataType.PUSH && data.result) {
        const market = data.result
        const instrumentId = market.I // I 是 InstrumentID
        if (instrumentId && market.N) {
          // 直接使用 InstrumentID 作为 key
          setMarketPrices(prev => ({
            ...prev,
            [instrumentId]: market.N // N 是最新价格
          }))
        }
      }
    })

    setWs(websocket)

    return () => {
      websocket.disconnect()
    }
  }, [])

  // 获取交易对列表
  useEffect(() => {
    const fetchSymbols = async () => {
      try {
        // 获取所有产品组的交易对（可以根据需要调整）
        const symbolList = await getSymbolList('')
        const symbolMap = {}
        symbolList.forEach(symbol => {
          symbolMap[symbol.InstrumentID] = symbol
          // 也按BaseCurrency建立索引，方便查找
          if (symbol.BaseCurrency) {
            symbolMap[symbol.BaseCurrency] = symbol
          }
        })
        setSymbols(symbolMap)
      } catch (error) {
        // 静默处理错误
      }
    }
    fetchSymbols()
  }, [])

  // 管理订阅（当资产列表变化且WebSocket已连接时）
  useEffect(() => {
    if (!assetsResult || assetsResult.length === 0 || !wsConnected || !ws || Object.keys(symbols).length === 0) return

    const currentSymbols = new Set()

    // 收集当前需要订阅的交易对
    assetsResult.forEach(item => {
      const currency = item.Currency
      if (currency && currency !== 'USDT' && currency !== 'USD') {
        // 查找对应的交易对
        const symbol = symbols[currency]
        if (symbol && symbol.InstrumentID) {
          currentSymbols.add(symbol.InstrumentID)
        }
      }
    })

    // 订阅新交易对
    currentSymbols.forEach(instrumentId => {
      if (!subscribedSymbolsRef.current.has(instrumentId)) {
        const symbol = symbols[instrumentId]
        if (symbol) {
          subscribeMarketDetail(ws, symbol)
          subscribedSymbolsRef.current.add(instrumentId)
        }
      }
    })

    // 取消订阅不再需要的交易对
    subscribedSymbolsRef.current.forEach(instrumentId => {
      if (!currentSymbols.has(instrumentId)) {
        const symbol = symbols[instrumentId]
        if (symbol) {
          unsubscribeMarketDetail(ws, symbol)
          subscribedSymbolsRef.current.delete(instrumentId)
        }
      }
    })
  }, [assetsResult, wsConnected, symbols])

  const data = useMemo(() => {
    if (!assetsResult || !Array.isArray(assetsResult) || assetsResult.length === 0) {
      return mockData
    }

    const tokens = assetsResult.map(item => {
      const currency = item.Currency || 'UNKNOWN'
      const amount = item.Available || 0 // 使用 Available 字段作为数量
      
      // 计算价值（使用高精度计算）
      let value = 0
      if (currency === 'USDT' || currency === 'USD') {
        value = amount // USDT/USD 直接使用数量作为价值
      } else {
        // 查找对应的交易对
        const symbol = symbols[currency]
        if (symbol && symbol.InstrumentID) {
          const price = marketPrices[symbol.InstrumentID]
          // 只有当价格存在时才计算价值
          if (price && price > 0) {
            // 使用 Decimal.js 进行高精度计算
            value = new Decimal(amount).mul(new Decimal(price)).toNumber()
          }
        }
      }

      return {
        symbol: currency,
        amount: amount,
        value: value
      }
    })

    // 使用 Decimal.js 计算总价值，避免精度丢失
    const total = tokens.reduce((sum, t) => {
      return new Decimal(sum).add(new Decimal(t.value)).toNumber()
    }, 0)

    return {
      ...mockData,
      total,
      tokens
    }
  }, [assetsResult, marketPrices])

  // 自适应宽度
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
    <div className='wallet'>
      <section ref={containerRef} style={{ flex: 1, padding: 20, overflowY: 'auto' }}>
        <div style={{ display: 'flex', alignItems: 'center', marginBottom: 20 }}>
          <h2 style={{ marginRight: 40 }}>总资产: {data.total.toFixed(2)} USD</h2>
          <PieChart
            tokens={data.tokens}
            highlightIndex={highlightIndex}
            setHighlightIndex={setHighlightIndex}
            mousePos={mousePos}
            setMousePos={setMousePos}
            size={pieSize}
          />
        </div>

        <AssetTable tokens={data.tokens} highlightIndex={highlightIndex} setHighlightIndex={setHighlightIndex} />

        <h3 style={{ marginTop: 40 }}>总资产变化历史</h3>
        <LineChart historyData={data.history} hoverIndex={hoverIndex} setHoverIndex={setHoverIndex} width={containerWidth} />

        <HistoryTable tokenHis={data.tokenHis} />
      </section>
    </div>
  )
})
