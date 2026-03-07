import React, { useRef, useEffect, useContext, useState, memo } from 'react'

import Loading from '../../../../component/loading'

import { formatValue } from '../../../../utils/format'

import RootContext from '../../../rootContext'
import ExchangeContext from '../../context'
import { WebsocketMessageType, WebsocketDataType } from '../../createWebsocket'

import { Theme, ColorTheme } from '../../../../constants/theme'
import { Direction } from '../../../../constants/order'
import VolumeUnitType from '../../../../constants/volumeUnitType'

import { subscribeDepth, unsubscribeDepth } from '../../../../api/contract'

const DepthChart = (({
  onChartReady
}) => {
  const chart = useRef()

  const bidMap = useRef({})
  const askMap = useRef({})

  const { config: { theme, colorTheme } } = useContext(RootContext)
  const { ws, wsConnected, currentSymbol = {}, volumeUnitType } = useContext(ExchangeContext)

  const [loading, setLoading] = useState(true)

  useEffect(() => {
    const { DepthChart } = window.depthchart
    chart.current = new DepthChart(document.getElementById('depth_chart'))
    const resizeHandler = () => {
      chart.current.resize()
    }
    window.addEventListener('resize', resizeHandler)
    onChartReady(chart.current)
    return () => {
      window.removeEventListener('resize', resizeHandler)
    }
  }, [])

  useEffect(() => {
    if (chart.current) {
      const isDark = theme === Theme.DARK
      const isBuyRiseGreen = colorTheme === ColorTheme.BUY_RISE_GREEN
      const bidColor = isBuyRiseGreen ? '#2DC08E' : '#F92855'
      const bidAreaColor = isBuyRiseGreen ? 'rgba(45, 192, 142, .08)' : 'rgba(249, 40, 85, .08)'
      const bidCicleColor = isBuyRiseGreen ? 'rgba(45, 192, 142, .2)' : 'rgba(249, 40, 85, .2)'
      const askColor = isBuyRiseGreen ? '#F92855' : '#2DC08E'
      const askAreaColor = isBuyRiseGreen ? 'rgba(249, 40, 85, .08)' : 'rgba(45, 192, 142, .08)'
      const askCicleColor = isBuyRiseGreen ? 'rgba(249, 40, 85, .2)' : 'rgba(45, 192, 142, .2)'
      const axisLineColor = isDark ? 'rgba(255, 255, 255, .1)' : 'rgba(0, 0, 0, .1)'
      const axisTextColor = isDark ? 'rgba(255, 255, 255, .5)' : 'rgba(0, 0, 0, .5)'
      const tooltipBackgroundColor = isDark ? 'rgba(255, 255, 255, .2)' : 'rgba(0, 0, 0, .2)'
      chart.current.applyOptions({
        middle: {
          line: {
            color: axisLineColor
          }
        },
        xAxis: {
          axisLine: {
            color: axisLineColor
          },
          tickLine: {
            color: axisLineColor
          },
          tickText: {
            color: axisTextColor
          }
        },
        yAxis: {
          axisLine: {
            color: axisLineColor
          },
          tickLine: {
            color: axisLineColor
          },
          tickText: {
            color: axisTextColor
          }
        },
        bid: {
          areaColor: bidAreaColor,
          lineColor: bidColor
        },
        ask: {
          areaColor: askAreaColor,
          lineColor: askColor
        },
        tooltip: {
          cross: {
            innerCircle: {
              bidColor: bidColor,
              askColor: askColor
            },
            outerCircle: {
              bidColor: bidCicleColor,
              askColor: askCicleColor
            }
          },
          text: {
            bidColor,
            askColor,
            backgroundColor: tooltipBackgroundColor
          },
          line: {
            bidColor,
            askColor
          }
        }
      })
    }
  }, [theme, colorTheme])

  useEffect(() => {
    if (wsConnected && currentSymbol?.InstrumentID) {
      if (chart.current) {
        const volumePrecision = volumeUnitType === VolumeUnitType.SHEET ? formatValue(currentSymbol, 'VolumePrecision', 0) : formatValue(currentSymbol, 'CoinPrecision', 0)
        chart.current.setPricePrecision(formatValue(currentSymbol, 'PricePrecision', 0))
        chart.current.setVolumePrecision(volumePrecision)
      }
      ws.on(WebsocketMessageType.DEPTH, 'depthChart', (data) => {
        if (data.type === WebsocketDataType.SUB) {
          setLoading(false)
        }
        if (data.type === WebsocketDataType.PUSH) {
          const orderList = data.result
          if (orderList[0]?.InstrumentID === currentSymbol?.InstrumentID) {
            orderList.forEach(data => {
              if (data.Direction) {
                if (data.Direction) {
                  if (data.Direction === Direction.BUY) {
                    if (data.Volume === 0) {
                      delete bidMap.current[data.Price]
                    } else {
                      bidMap.current[data.Price] = data
                    }
                  } else {
                    if (data.Volume === 0) {
                      delete askMap.current[data.Price]
                    } else {
                      askMap.current[data.Price] = data
                    }
                  }
                }
              }
            })
            let totalVolume = 0
            const bidList = Object.keys(bidMap.current).sort((p1, p2) => p2 - p1).map(p => {
              const price = bidMap.current[p].Price
              const volumeSheet = bidMap.current[p].Volume || 0
              const volume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, { symbol: currentSymbol, volume: volumeSheet, price })
              totalVolume += volume
              return { price, volume: totalVolume }
            })
            bidList.sort((d1, d2) => d1.price - d2.price)
            totalVolume = 0
            const askList = Object.keys(askMap.current).sort((p1, p2) => p1 - p2).map(p => {
              const price = askMap.current[p].Price
              const volumeSheet = askMap.current[p].Volume || 0
              const volume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, { symbol: currentSymbol, volume: volumeSheet, price })
              totalVolume += volume
              return { price, volume: totalVolume }
            })
            if (chart.current) {
              chart.current.applyData({ bids: bidList, asks: askList })
            }
          }
        }
      })
      subscribeDepth(ws, currentSymbol)
    }
    return () => {
      setLoading(true)
      if (wsConnected) {
        if (currentSymbol?.InstrumentID) {
          unsubscribeDepth(ws, currentSymbol)
        }
      }
      if (ws) {
        ws.off(WebsocketMessageType.DEPTH, 'depthChart')
      }
      bidMap.current = {}
      askMap.current = {}
      if (chart.current) {
        chart.current.applyData({ bids: [], asks: [] })
      }
    }
  }, [wsConnected, currentSymbol?.InstrumentID, volumeUnitType])

  return <div className="chart-container" id="depth_chart">{loading && <Loading/>}</div>
})

export default memo(DepthChart)
