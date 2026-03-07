import React, { useRef, useEffect, useContext, memo } from 'react'

import overrideStyle from './overrideStyle'

import { formatValue } from '../../../../utils/format'

import RootContext from '../../../rootContext'
import ExchangeContext from '../../context'
import { WebsocketMessageType, WebsocketDataType } from '../../createWebsocket'

import { getHistoryKLine, subscribeKLine, unsubscribeKLine } from '../../../../api/contract'

import { transferResolution, transferData, splitRequest, transferCandleCount } from '../transfer'
import update from '../update'

import VolumeUnitType from '../../../../constants/volumeUnitType'

import { KLINE_DATA_TYPE_INDEX } from '..'

const TradingView = (({
  period, kLineDataType, onChartReady
}) => {
  const chart = useRef()
  const prevKLineData = useRef()
  const lastKLineData = useRef()

  const { config: { theme, colorTheme } } = useContext(RootContext)
  const { ws, wsConnected, currentSymbol = {}, symbols, volumeUnitType } = useContext(ExchangeContext)

  useEffect(() => {
    return () => {
      if (chart.current) {
        chart.current = null
      }
    }
  }, [wsConnected, volumeUnitType])

  useEffect(() => {
    if (wsConnected && currentSymbol?.InstrumentID && !chart.current) {
      const Widget = require('./Widget').default
      chart.current = new Widget(
        {
          containerId: 'trading_view_container'
        },
        currentSymbol?.InstrumentID,
        { resolution: period.resolution, chartType: period.type === 'line' ? 3 : 1 },
        'en',
        'Asia/Shanghai',
        overrideStyle(theme, colorTheme),
        (symbolId) => {
          const symbol = symbols[symbolId]
          return {
            name: symbol?.InstrumentID,
            ticker: symbol?.InstrumentID,
            exchangeId: symbol.ExchangeID,
            minmov: 1,
            pricescale: Math.pow(10, formatValue(symbol, 'PricePrecision', 0)),
            volume_precision: volumeUnitType === VolumeUnitType.SHEET ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0),
            session: '24x7',
            IsInverse: symbol.IsInverse,
            VolumeMultiple: symbol.VolumeMultiple
          }
        },
        // 获取历史数据回调
        (symbol, resolution, from, to, onHistoryCallback, onErrorCallback, firstDataRequest) => {
          const interval = transferResolution(resolution)
          const params = splitRequest(from, to, resolution)
          const tasks = []
          for (const param of params) {
            tasks.push(getHistoryKLine(
              kLineDataType === KLINE_DATA_TYPE_INDEX ? `i${symbol.ticker}` : symbol.ticker,
              symbol.exchangeId,
              interval,
              param.to,
              param.count
            ))
          }
          const totalCount = transferCandleCount(from, to, resolution)
          let dataList = []
          Promise.all(tasks).then(results => {
            for (const result of results) {
              dataList = dataList.concat(result || [])
            }
            dataList = dataList.map(data => transferData(data, { volumeUnitType, symbol }))
            dataList.sort((d1, d2) => d1.timestamp - d2.timestamp)
            if (firstDataRequest) {
              prevKLineData.current = dataList[dataList.length - 1] || {}
              lastKLineData.current = dataList[dataList.length - 1]
            }
            onHistoryCallback(dataList, { noData: dataList.length < totalCount })
          }).catch(
            e => { onErrorCallback('error') }
          )
        },
        // 订阅回调
        (symbolId, exchangeId, resolution, onRealtimeCallback) => {
          const symbol = symbols[symbolId]
          ws.on(WebsocketMessageType.K_LINE, 'tradingView', (data) => {
            if (data.type === WebsocketDataType.PUSH) {
              update(volumeUnitType, symbol, prevKLineData, lastKLineData.current, resolution, data.result, (kLineData) => {
                onRealtimeCallback(kLineData)
                lastKLineData.current = kLineData
              })
            }
          })
          subscribeKLine(ws, { symbolId: kLineDataType === KLINE_DATA_TYPE_INDEX ? `i${symbolId}` : symbolId, exchangeId }, transferResolution(resolution))
        },
        // 取消订阅回调
        (symbol, resolution) => {
          unsubscribeKLine(ws, { symbolId: kLineDataType === KLINE_DATA_TYPE_INDEX ? `i${symbol.ticker}` : symbol.ticker, exchangeId: symbol.exchangeId }, transferResolution(resolution))
        }
      )
      chart.current.onChartReady(() => {
        onChartReady(chart.current)
      })
    }

    if (chart.current && chart.current.chartReady) {
      const widget = chart.current.chart()
      if (currentSymbol?.InstrumentID && widget.symbol() !== currentSymbol?.InstrumentID) {
        widget.setSymbol(currentSymbol?.InstrumentID)
      }
      if (period.resolution !== widget.resolution()) {
        widget.setResolution(period.resolution)
      }
      const type = period.type === 'line' ? 3 : 1
      if (widget.chartType() !== type) {
        widget.setChartType(type)
      }
    }

    return () => {
      if (ws) {
        ws.off(WebsocketMessageType.K_LINE, 'tradingView')
      }
      prevKLineData.current = null
    }
  }, [wsConnected, currentSymbol?.InstrumentID, period.key, kLineDataType, volumeUnitType])

  useEffect(() => {
    if (chart.current && chart.current.chartReady) {
      const widget = chart.current
      const { customCssUrl, overrides, studiesOverrides } = overrideStyle('dark', colorTheme)
      // changeTheme(themeName)
      widget.addCustomCSSFile(customCssUrl)
      widget.applyOverrides(overrides)
      widget.applyStudiesOverrides(studiesOverrides)
    }
  }, [theme, colorTheme])

  return <div style={{ background: 'transparent' }} className="chart-container" id="trading_view_container"/>
})

export default memo(TradingView)
