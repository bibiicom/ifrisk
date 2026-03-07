import React, { useRef, useEffect, useContext, useState, memo } from 'react'

import { init, dispose } from 'klinecharts'

import axios from 'axios'

import Loading from '../../../../component/loading'
import intl from '../../../../component/intl'

import { formatValue } from '../../../../utils/format'

import RootContext from '../../../rootContext'
import ExchangeContext from '../../context'
import { WebsocketMessageType, WebsocketDataType } from '../../createWebsocket'

import { getLastKLine, getHistoryKLine, subscribeKLine, unsubscribeKLine } from '../../../../api/contract'

import { transferResolution, transferData } from '../transfer'
import update from '../update'

import { Theme, ColorTheme } from '../../../../constants/theme'
import VolumeUnitType from '../../../../constants/volumeUnitType'

import TimezoneModal from './timezone'
import IndicatorModal from './indicator'
import { KLINE_DATA_TYPE_INDEX } from '..'

const SimpleChart = (({
  timezoneModalVisible, indicatorModalVisible,
  period, kLineDataType, onChartReady,
  onTimezoneModalClose, onIndicatorModalClose
}) => {
  const container = useRef()
  const chart = useRef()
  const prevKLineData = useRef()
  const kLineDataRequestCancel = useRef()

  const currentSymbolTemp = useRef({})

  const { config: { theme, colorTheme } } = useContext(RootContext)
  const { ws, wsConnected, currentSymbol = {}, volumeUnitType } = useContext(ExchangeContext)

  const [loading, setLoading] = useState(true)
  const [timezone, setTimezone] = useState({ key: 'Asia/Shanghai', text: intl.get('shanghai') })
  const [mainIndicator, setMainIndicator] = useState('MA')
  const [subIndicator, setSubIndicator] = useState('')

  useEffect(() => {
    chart.current = init('simple_chart')
    const resizeHandler = () => {
      chart.current.resize()
    }
    chart.current.createIndicator('VOL')
    window.addEventListener('resize', resizeHandler)
    onChartReady(chart.current)
    return () => {
      window.removeEventListener('resize', resizeHandler)
      dispose('simple_chart')
    }
  }, [])

  useEffect(() => {
    if (chart.current) {
      const isDark = theme === Theme.DARK
      const isBuyRiseGreen = colorTheme === ColorTheme.BUY_RISE_GREEN
      const upColor = isBuyRiseGreen ? '#2DC08E' : '#F92855'
      const downColor = isBuyRiseGreen ? '#F92855' : '#2DC08E'
      const upOpacityColor = isBuyRiseGreen ? 'rgba(45, 192, 142, .5)' : 'rgba(249, 40, 85, .5)'
      const downOpacityColor = isBuyRiseGreen ? 'rgba(249, 40, 85, .5)' : 'rgba(45, 192, 142, .5)'
      const mainColor = isDark ? 'rgba(255, 255, 255, .5)' : 'rgba(0, 0, 0, .5)'
      const gridColor = isDark ? 'rgba(255, 255, 255, .03)' : 'rgba(0, 0, 0, .03)'
      const crossTextFillColor = isDark ? '#373a40' : '#777777'
      const crossTextColor = '#FFFFFF'
      const axisLineColor = isDark ? 'rgba(255, 255, 255, .1)' : 'rgba(0, 0, 0, .1)'
      const axisTextColor = isDark ? 'rgba(255, 255, 255, .5)' : 'rgba(0, 0, 0, .5)'

      chart.current.setStyles({
        grid: {
          horizontal: {
            color: gridColor
          },
          vertical: {
            display: true,
            color: gridColor
          }
        },
        candle: {
          bar: {
            upColor: upColor,
            downColor: downColor,
            noChangeColor: upColor
          },
          priceMark: {
            high: {
              color: mainColor,
              textFamily: 'Roboto'
            },
            low: {
              color: mainColor,
              textFamily: 'Roboto'
            },
            last: {
              upColor: upColor,
              downColor: downColor,
              noChangeColor: upColor
            }
          },
          tooltip: {
            // showRule: tooltipTools ? 'none' : 'always',
            text: {
              color: mainColor,
              family: 'Roboto'
            }
          }
        },
        technicalIndicator: {
          bar: {
            upColor: upOpacityColor,
            downColor: downOpacityColor,
            noChangeColor: upOpacityColor
          },
          circle: {
            upColor: upOpacityColor,
            downColor: downOpacityColor,
            noChangeColor: upOpacityColor
          },
          line: {
            colors: ['#F1b615', '#665980', '#478FB2', '#993D7A', '#01C5C4']
          },
          tooltip: {
            // showRule: tooltipTools ? 'none' : 'always',
            text: {
              color: mainColor,
              family: 'Roboto'
            }
          }
        },
        xAxis: {
          minHeight: 26,
          axisLine: {
            color: axisLineColor
          },
          tickText: {
            color: axisTextColor,
            family: 'Roboto'
          },
          tickLine: {
            color: axisLineColor
          }
        },
        yAxis: {
          axisLine: {
            color: axisLineColor
          },
          tickText: {
            color: axisTextColor,
            family: 'Roboto'
          },
          tickLine: {
            color: axisLineColor
          }
        },
        separator: {
          color: axisLineColor,
          activeBackgroundColor: 'rgba(22, 119, 255, .1)'
        },
        crosshair: {
          horizontal: {
            line: {
              color: mainColor
            },
            text: {
              color: crossTextColor,
              borderColor: crossTextFillColor,
              backgroundColor: crossTextFillColor,
              family: 'Roboto'
            }
          },
          vertical: {
            line: {
              color: mainColor
            },
            text: {
              color: crossTextColor,
              borderColor: crossTextFillColor,
              backgroundColor: crossTextFillColor,
              family: 'Roboto'
            }
          }
        }
      })
    }
  }, [theme, colorTheme])

  useEffect(() => {
    if (chart.current) {
      chart.current.setTimezone(timezone.key)
    }
  }, [timezone.key])

  useEffect(() => {
    if (mainIndicator) {
      chart.current.createIndicator(mainIndicator, false, { id: 'candle_pane' })
    } else {
      chart.current.removeIndicator('candle_pane')
    }
  }, [mainIndicator])

  useEffect(() => {
    if (subIndicator) {
      chart.current.createIndicator(subIndicator, false, { id: 'sub_tech_pane' })
    } else {
      chart.current.removeIndicator('sub_tech_pane')
    }
  }, [subIndicator])

  useEffect(() => {
    if (currentSymbol.InstrumentID) {
      currentSymbolTemp.current = currentSymbol
      const volumePrecision = volumeUnitType === VolumeUnitType.SHEET ? formatValue(currentSymbol, 'VolumePrecision', 0) : formatValue(currentSymbol, 'CoinPrecision', 0)
      chart.current.setPriceVolumePrecision(
        formatValue(currentSymbol, 'PricePrecision', 0), volumePrecision
      )
    }
  }, [currentSymbol.InstrumentID, volumeUnitType])

  useEffect(() => {
    if (wsConnected && currentSymbol.InstrumentID) {
      const queryHistory = async (to) => {
        // console.log('to', to)
        try {
          const interval = transferResolution(period.resolution)
          if (isNaN(to)) {
            const result = await getLastKLine(
              kLineDataType === KLINE_DATA_TYPE_INDEX ? `i${currentSymbol.InstrumentID}` : currentSymbol.InstrumentID,
              currentSymbol.ExchangeID,
              interval,
              400,
              { cancelToken: new axios.CancelToken(c => { kLineDataRequestCancel.current = c }) }
            ) || []
            // console.log('result', result)
            const dataList = result.map(data => transferData(data, { volumeUnitType, symbol: currentSymbol }))
            // console.log('dataList', dataList)
            const reversedDataList = [...dataList].reverse()
            chart.current.applyNewData(reversedDataList, reversedDataList.length === 400)
            prevKLineData.current = reversedDataList[reversedDataList.length - 1] || {}
            ws.on(WebsocketMessageType.K_LINE, `simpleChart_${currentSymbol.InstrumentID}`, (data) => {
              if (data && data.type === WebsocketDataType.PUSH && data.result) {
                // console.log('data', data)
                const kLineDataList = chart.current.getDataList()
                const lastKLineData = kLineDataList[kLineDataList.length - 1]
                update(
                  volumeUnitType,
                  currentSymbol,
                  prevKLineData,
                  lastKLineData,
                  period.resolution,
                  data.result,
                  data.index,
                  (kLineData) => { chart.current.updateData(kLineData) }
                )
              }
            })
            chart.current.loadMore((timestamp) => {
              // console.log('timestamp', timestamp)
              queryHistory((timestamp))
            })
            subscribeKLine(ws, {
              symbolId: kLineDataType === KLINE_DATA_TYPE_INDEX ? `i${currentSymbol.InstrumentID}` : currentSymbol.InstrumentID,
              exchangeId: currentSymbol.ExchangeID
            }, interval)
          } else {
            const result = await getHistoryKLine(
              kLineDataType === KLINE_DATA_TYPE_INDEX ? `i${currentSymbol?.InstrumentID}` : currentSymbol?.InstrumentID,
              currentSymbol.ExchangeID,
              interval,
              to,
              400,
              { cancelToken: new axios.CancelToken(c => { kLineDataRequestCancel.current = c }) }
            ) || []
            const dataList = result.map(data => transferData(data, { volumeUnitType, symbol: currentSymbol }))
            const reversedDataList = [...dataList].reverse()
            if (currentSymbolTemp.current.InstrumentID === currentSymbol.InstrumentID) {
              chart.current.applyMoreData(reversedDataList, reversedDataList.length === 400)
            }
          }
        } catch (e) {
          if (isNaN(to)) {
            chart.current.applyNewData([])
          }
        } finally {
          // kLineDataRequestCancel.current = null
          setLoading(false)
          chart.current.setStyles({
            candle: {
              type: period.type === 'line' ? 'area' : 'candle_solid'
            }
          })
        }
      }
      queryHistory()
    }
    return () => {
      if (kLineDataRequestCancel.current) {
        kLineDataRequestCancel.current({ code: -100 })
        kLineDataRequestCancel.current = null
      }
      setLoading(true)
      if (ws) {
        ws.off(WebsocketMessageType.K_LINE, `simpleChart_${currentSymbol.InstrumentID}`)
      }
      if (wsConnected) {
        if (currentSymbol.InstrumentID) {
          unsubscribeKLine(ws, {
            symbolId: kLineDataType === KLINE_DATA_TYPE_INDEX ? `i${currentSymbol.InstrumentID}` : currentSymbol.InstrumentID,
            exchangeId: currentSymbol.ExchangeID
          }, transferResolution(period.resolution))
        }
      }
      prevKLineData.current = null
    }
  }, [wsConnected, currentSymbol.InstrumentID, period.resolution, kLineDataType, volumeUnitType])

  return (
    <div
      ref={container}
      style={{ height: '100%', width: '100%', position: 'relative' }}>
      {loading && <Loading />}
      <div id="simple_chart" className="chart-container" />
      {
        timezoneModalVisible &&
        <TimezoneModal
          getContainer={() => container.current}
          onClose={onTimezoneModalClose}
          timezone={timezone}
          onTimezoneChange={setTimezone} />
      }
      {
        indicatorModalVisible &&
        <IndicatorModal
          getContainer={() => container.current}
          onClose={onIndicatorModalClose}
          mainIndicator={mainIndicator}
          subIndicator={subIndicator}
          onMainIndicatorChange={setMainIndicator}
          onSubIndicatorChange={setSubIndicator} />
      }
    </div>
  )
})

export default memo(SimpleChart)
