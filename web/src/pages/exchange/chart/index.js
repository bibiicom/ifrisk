import React, { useState, useRef, useEffect, memo, useContext } from 'react'

import Section from '../components/section'

import Nav from './nav'

import SimpleChart from './simpleChart'
import TradingView from './tradingview'
import DepthChart from './depthChart'

import MorePeriodModal from './morePeriod'

import ExchangeContext from '../context'

import { ProductGroup } from '../../../constants/symbol'

import './index.less'

export const CHART_TYPE_SIMPLE_STOCK_CHART = 'simple_stock_chart'
export const CHART_TYPE_TRADING_VIEW = 'trading_view'
export const CHART_TYPE_DEPTH_CHART = 'depth_chart'

export const KLINE_DATA_TYPE_CURRENT = 'current'
export const KLINE_DATA_TYPE_INDEX = 'index'

const Chart = (() => {
  const container = useRef()
  const chart = useRef()
  const fullScreen = useRef(false)
  const timer = useRef()

  const { currentSymbol } = useContext(ExchangeContext)

  const [chartType, setChartType] = useState(CHART_TYPE_SIMPLE_STOCK_CHART)
  const [period, setPeriod] = useState({ key: '1min', resolution: '1', text: '1Min', type: 'min' })

  const [kLineDataType, setKLineDataType] = useState(KLINE_DATA_TYPE_CURRENT)

  const [morePeriodModalVisible, setMorePeriodModalVisible] = useState(false)

  const [simpleChartTimezoneModalVisible, setSimpleChartTimezoneModalVisible] = useState(false)
  const [simpleChartIndicatorModalVisible, setSimpleChartIndicatorModalVisible] = useState(false)

  useEffect(() => {
    const fullScreenChange = () => {
      fullScreen.current = !fullScreen.current
      if (timer.current) {
        clearTimeout(timer.current)
      }
      if (chart.current && chart.current.resize) {
        timer.current = setTimeout(() => {
          chart.current.resize()
        }, 500)
      }
    }
    document.addEventListener('fullscreenchange', fullScreenChange)
    document.addEventListener('mozfullscreenchange', fullScreenChange)
    document.addEventListener('webkitfullscreenchange', fullScreenChange)
    document.addEventListener('msfullscreenchange', fullScreenChange)

    return () => {
      document.removeEventListener('fullscreenchange', fullScreenChange)
      document.removeEventListener('mozfullscreenchange', fullScreenChange)
      document.removeEventListener('webkitfullscreenchange', fullScreenChange)
      document.removeEventListener('msfullscreenchange', fullScreenChange)
      if (timer.current) {
        clearTimeout(timer.current)
      }
    }
  }, [])

  useEffect(() => {
    if (currentSymbol?.ProductGroup === ProductGroup.OPTION) {
      setKLineDataType(KLINE_DATA_TYPE_INDEX)
    }
  }, [currentSymbol?.ProductGroup])

  const enterExitFullScreen = () => {
    if (!fullScreen.current) {
      const el = container.current
      if (el) {
        const enterFullScreen = el.requestFullScreen || el.webkitRequestFullScreen || el.mozRequestFullScreen || el.msRequestFullScreen
        enterFullScreen.call(el)
      }
    } else {
      const exitFullscreen = document.exitFullscreen || document.msExitFullscreen || document.mozCancelFullScreen || document.webkitExitFullscreen
      exitFullscreen.call(document)
    }
  }

  return (
    <div ref={container}>
      {
        morePeriodModalVisible &&
        <MorePeriodModal
          onClose={() => { setMorePeriodModalVisible(false) }}
          onConfirm={(v) => { setPeriod({ key: 'more', type: 'more', ...v }) }}
        />
      }
      <Nav
        chartType={chartType}
        period={period}
        kLineDataType={kLineDataType}
        onPeriodChange={setPeriod}
        onKLineDataTypeChange={setKLineDataType}
        onChartTypeChange={setChartType}
        onMorePeriodClick={() => { setMorePeriodModalVisible(!morePeriodModalVisible) }}
        onIndicatorClick={() => {
          if (chart.current) {
            if (chartType === CHART_TYPE_TRADING_VIEW) {
              chart.current.closePopupsAndDialogs()
              chart.current.chart().executeActionById('insertIndicator')
            } else if (chartType === CHART_TYPE_SIMPLE_STOCK_CHART) {
              setSimpleChartIndicatorModalVisible(!simpleChartIndicatorModalVisible)
            }
          }
        }}
        onSettingClick={() => {
          if (chart.current) {
            if (chartType === CHART_TYPE_TRADING_VIEW) {
              chart.current.closePopupsAndDialogs()
              chart.current.chart().executeActionById('chartProperties')
            } else if (chartType === CHART_TYPE_SIMPLE_STOCK_CHART) {
              setSimpleChartTimezoneModalVisible(!simpleChartTimezoneModalVisible)
            }
          }
        }}
        onScreenShotClick={() => {
          if (chart.current) {
            if (chartType === CHART_TYPE_TRADING_VIEW) {
              chart.current.takeScreenshot()
            } else if (chartType === CHART_TYPE_SIMPLE_STOCK_CHART) {
              const url = chart.current.getConvertPictureUrl(true, 'jpeg', '#19171e')
              const a = document.createElement('a')
              a.download = 'screen_shot'
              a.href = url
              document.body.appendChild(a)
              a.click()
              a.remove()
            }
          }
        }}
        onFullScreenClick={enterExitFullScreen}/>
      <Section
        className="exchange-chart-content">
        {
          chartType === CHART_TYPE_SIMPLE_STOCK_CHART
            ? <SimpleChart
              period={period}
              kLineDataType={kLineDataType}
              indicatorModalVisible={simpleChartIndicatorModalVisible}
              timezoneModalVisible={simpleChartTimezoneModalVisible}
              onChartReady={c => { chart.current = c }}
              onTimezoneModalClose={() => { setSimpleChartTimezoneModalVisible(false) }}
              onIndicatorModalClose={() => { setSimpleChartIndicatorModalVisible(false) }}
            />
            : (
              chartType === CHART_TYPE_TRADING_VIEW
                ? <TradingView
                  period={period}
                  kLineDataType={kLineDataType}
                  onChartReady={c => { chart.current = c }}/>
                : <DepthChart
                  onChartReady={c => { chart.current = c }}/>
            )
        }
      </Section>
    </div>
  )
})

export default memo(Chart)
