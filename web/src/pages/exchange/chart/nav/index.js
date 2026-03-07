import React, { useState, useEffect, useRef, memo, useContext } from 'react'

import Select from '../../../../component/select'
import Button from '../../../../component/button'
import Row from '../../../../component/row'
import Popover from '../../../../component/popover'
import { Function, ScreenShot, FullScreen, Setting, Menu } from '../../../../component/icons'
import intl from '../../../../component/intl'
import ComponentContext from '../../../../component/context'
import useScreenSize, { ScreenSize } from '../../../../component/hooks/useScreenSize'

import Section from '../../components/section'

import {
  CHART_TYPE_SIMPLE_STOCK_CHART, CHART_TYPE_TRADING_VIEW, CHART_TYPE_DEPTH_CHART,
  KLINE_DATA_TYPE_CURRENT
} from '../index'

import { ProductGroup } from '../../../../constants/symbol'
import ExchangeContext from '../../context'

import './index.less'

const Nav = (({
  period, chartType, kLineDataType, onPeriodChange, onKLineDataTypeChange, onChartTypeChange,
  onMorePeriodClick, onIndicatorClick, onSettingClick, onScreenShotClick, onFullScreenClick
}) => {
  const menuRef = useRef()

  const { theme } = useContext(ComponentContext)

  const { currentSymbol } = useContext(ExchangeContext)

  const screenSize = useScreenSize()

  const [currentMinPeriod, setCurrentMinPeriod] = useState({ key: '1min', resolution: '1', text: '1Min', type: 'min' })
  const [currentHourPeriod, setCurrentHourPeriod] = useState({ key: '1hour', resolution: '60', text: '1H', type: 'long' })
  const [currentLongPeriod, setCurrentLongPeriod] = useState({ key: '1day', resolution: 'D', text: '1D', type: 'long' })

  const [menuDropDownPopoverVisible, setMenuDropDownPopoverVisible] = useState(false)

  useEffect(() => {
    if (period.type === 'min') {
      setCurrentMinPeriod(period)
    } else if (period.type === 'hour') {
      setCurrentHourPeriod(period)
    } else if (period.type === 'long') {
      setCurrentLongPeriod(period)
    }
  }, [period.key])

  return (
    <Section
      className={`exchange-chart-nav-${theme}`}>
      {
        chartType !== CHART_TYPE_DEPTH_CHART &&
        <Row className="exchange-chart-nav-content">
          <Button
            className={`exchange-chart-nav-content-button ${period.key === 'real_time' ? 'selected' : ''}`}
            type="tertiary"
            size="small"
            onClick={() => { onPeriodChange({ key: 'real_time', text: intl.get('real_time'), resolution: '1', type: 'line' }) }}>
            {intl.get('real_time')}
          </Button>

          {
            screenSize === ScreenSize.MIN ? (
              <Select
                className={`exchange-chart-nav-content-select ${period.key !== 'real_time' && period.key !== 'more' ? 'selected' : ''}`}
                selectorClassName="exchange-chart-nav-content-select-selector"
                arrowClassName="exchange-chart-nav-content-select-selector-arrow"
                popoverStyle={{ borderRadius: 2 }}
                popoverRowClassName="exchange-chart-nav-content-select-popover-row"
                trigger="hover"
                value={currentMinPeriod.text}
                dataSource={[
                  { key: '1min', resolution: '1', text: '1Min', type: 'min' },
                  { key: '5min', resolution: '5', text: '5Min', type: 'min' },
                  { key: '10min', resolution: '10', text: '10Min', type: 'min' },
                  { key: '15min', resolution: '15', text: '15Min', type: 'min' },
                  { key: '30min', resolution: '30', text: '30Min', type: 'min' },
                  { key: '1hour', resolution: '60', text: '1H', type: 'hour' },
                  { key: '4hour', resolution: '240', text: '4H', type: 'hour' },
                  { key: '6hour', resolution: '360', text: '6H', type: 'hour' },
                  { key: '12hour', resolution: '720', text: '12H', type: 'hour' },
                  { key: '1day', resolution: 'D', text: '1D', type: 'long' },
                  { key: '3day', resolution: 'D', text: '3D', type: 'long' },
                  { key: '5day', resolution: 'D', text: '5D', type: 'long' },
                  { key: '1week', resolution: 'W', text: '1W', type: 'long' },
                  { key: '1month', resolution: 'M', text: '1M', type: 'long' }
                ]}
                onClick={() => { onPeriodChange(currentMinPeriod) }}
                onPopoverRowClick={onPeriodChange}/>
            ) : (
              <>
                <Select
                  className={`exchange-chart-nav-content-select ${period.type === 'min' ? 'selected' : ''}`}
                  selectorClassName="exchange-chart-nav-content-select-selector"
                  arrowClassName="exchange-chart-nav-content-select-selector-arrow"
                  popoverStyle={{ borderRadius: 2 }}
                  popoverRowClassName="exchange-chart-nav-content-select-popover-row"
                  trigger="hover"
                  value={currentMinPeriod.text}
                  dataSource={[
                    { key: '1min', resolution: '1', text: '1Min', type: 'min' },
                    { key: '5min', resolution: '5', text: '5Min', type: 'min' },
                    { key: '10min', resolution: '10', text: '10Min', type: 'min' },
                    { key: '15min', resolution: '15', text: '15Min', type: 'min' },
                    { key: '30min', resolution: '30', text: '30Min', type: 'min' },
                    { key: '45min', resolution: '45', text: '45Min', type: 'min' }
                  ]}
                  onClick={() => { onPeriodChange(currentMinPeriod) }}
                  onPopoverRowClick={onPeriodChange}/>
                <Select
                  style={{ width: 46 }}
                  className={`exchange-chart-nav-content-select ${period.type === 'hour' ? 'selected' : ''}`}
                  selectorClassName="exchange-chart-nav-content-select-selector"
                  arrowClassName="exchange-chart-nav-content-select-selector-arrow"
                  popoverStyle={{ borderRadius: 2 }}
                  popoverRowClassName="exchange-chart-nav-content-select-popover-row"
                  trigger="hover"
                  value={currentHourPeriod.text}
                  dataSource={[
                    { key: '1hour', resolution: '60', text: '1H', type: 'hour' },
                    { key: '4hour', resolution: '240', text: '4H', type: 'hour' },
                    { key: '6hour', resolution: '360', text: '6H', type: 'hour' },
                    { key: '12hour', resolution: '720', text: '12H', type: 'hour' }
                  ]}
                  onClick={() => { onPeriodChange(currentHourPeriod) }}
                  onPopoverRowClick={onPeriodChange}/>
                <Select
                  style={{ width: 46 }}
                  className={`exchange-chart-nav-content-select ${period.type === 'long' ? 'selected' : ''}`}
                  selectorClassName="exchange-chart-nav-content-select-selector"
                  arrowClassName="exchange-chart-nav-content-select-selector-arrow"
                  popoverStyle={{ borderRadius: 2 }}
                  popoverRowClassName="exchange-chart-nav-content-select-popover-row"
                  trigger="hover"
                  value={currentLongPeriod.text}
                  dataSource={[
                    { key: '1day', resolution: 'D', text: '1D', type: 'long' },
                    { key: '3day', resolution: 'D', text: '3D', type: 'long' },
                    { key: '5day', resolution: 'D', text: '5D', type: 'long' },
                    { key: '1week', resolution: 'W', text: '1W', type: 'long' },
                    { key: '1month', resolution: 'M', text: '1M', type: 'long' }
                  ]}
                  onClick={() => { onPeriodChange(currentLongPeriod) }}
                  onPopoverRowClick={onPeriodChange}/>
                {/* {
            ([
              { key: '1day', resolution: 'D', text: '1D', type: 'day' },
              { key: '1week', resolution: 'W', text: '1W', type: 'week' },
              { key: '1month', resolution: 'M', text: '1M', type: 'month' }
            ]).map(({ key, text, resolution, type }) => (
              <Button
                key={key}
                className={`exchange-chart-nav-content-button ${key === period.key ? 'selected' : ''}`}
                type="tertiary"
                size="small"
                onClick={() => { onPeriodChange({ key, text, resolution, type }) }}>
                {text}
              </Button>
            ))
          } */}
              </>
            )
          }

          <Button
            className={`exchange-chart-nav-content-button ${period.key === 'more' ? 'selected' : ''}`}
            type="tertiary"
            size="small"
            onClick={() => { onMorePeriodClick() }}>
            {period.key === 'more' ? period.text : intl.get('more')}
          </Button>
          <Row
            ref={menuRef}
            className="exchange-chart-nav-content-button-container exchange-chart-nav-content-button-fold-container"
            onMouseEnter={() => { setMenuDropDownPopoverVisible(true) }}
            onMouseLeave={() => { setMenuDropDownPopoverVisible(false) }}>
            <Button
              className="exchange-chart-nav-content-button exchange-chart-nav-content-icon-button"
              type="tertiary"
              size="small">
              <Menu width="12" height="12" className="icon"/>
            </Button>
            <Popover
              className="popover"
              visible={menuDropDownPopoverVisible}
              getContainer={_ => menuRef.current}>
              <Button
                style={{ marginRight: 0 }}
                className="exchange-chart-nav-content-button exchange-chart-nav-content-icon-button"
                type="tertiary"
                size="small"
                onClick={onIndicatorClick}>
                <Function className="icon"/>
              </Button>
              <Button
                style={{ marginRight: 0, marginTop: 8, marginBottom: 8 }}
                className="exchange-chart-nav-content-button exchange-chart-nav-content-icon-button"
                type="tertiary"
                size="small"
                onClick={onSettingClick}>
                <Setting width="12" height="12" className="icon"/>
              </Button>
              <Button
                style={{ marginRight: 0 }}
                className="exchange-chart-nav-content-button exchange-chart-nav-content-icon-button"
                type="tertiary"
                size="small"
                onClick={onScreenShotClick}>
                <ScreenShot className="icon"/>
              </Button>
              <Button
                style={{ marginRight: 0, marginTop: 8 }}
                className="exchange-chart-nav-content-button exchange-chart-nav-content-icon-button"
                type="tertiary"
                size="small"
                onClick={onFullScreenClick}>
                <FullScreen className="icon"/>
              </Button>
            </Popover>
          </Row>
          <Row
            className="exchange-chart-nav-content-button-expand-container">
            <Button
              className="exchange-chart-nav-content-button exchange-chart-nav-content-icon-button"
              type="tertiary"
              size="small"
              onClick={onIndicatorClick}>
              <Function className="icon"/>
            </Button>
            <Button
              className="exchange-chart-nav-content-button exchange-chart-nav-content-icon-button"
              type="tertiary"
              size="small"
              onClick={onSettingClick}>
              <Setting width="12" height="12" className="icon"/>
            </Button>
            <Button
              className="exchange-chart-nav-content-button exchange-chart-nav-content-icon-button"
              type="tertiary"
              size="small"
              onClick={onScreenShotClick}>
              <ScreenShot className="icon"/>
            </Button>
            <Button
              className="exchange-chart-nav-content-button exchange-chart-nav-content-icon-button"
              type="tertiary"
              size="small"
              onClick={onFullScreenClick}>
              <FullScreen className="icon"/>
            </Button>
          </Row>
          {
            (currentSymbol && (
              currentSymbol?.ProductGroup === ProductGroup.POSITIVE_DELIVERY ||
              currentSymbol?.ProductGroup === ProductGroup.REVERSE_DELIVERY ||
              currentSymbol?.ProductGroup === ProductGroup.SPOT ||
              currentSymbol?.ProductGroup === ProductGroup.REVERSE_SWAP ||
              currentSymbol?.ProductGroup === ProductGroup.OPTION
            )) && (
              <Select
                style={{ width: '70px' }}
                className="exchange-chart-nav-content-select"
                selectorClassName="exchange-chart-nav-content-select-selector"
                arrowClassName="exchange-chart-nav-content-select-selector-arrow"
                popoverStyle={{ borderRadius: 2 }}
                popoverRowClassName="exchange-chart-nav-content-select-popover-row"
                trigger="hover"
                value={kLineDataType === KLINE_DATA_TYPE_CURRENT ? intl.get('current') : intl.get('index')}
                dataSource={[
                  { key: KLINE_DATA_TYPE_CURRENT, text: intl.get('current') }
                  // { key: KLINE_DATA_TYPE_INDEX, text: intl.get('index') }
                ]}
                onSelected={(data) => { onKLineDataTypeChange(data.key) }}/>
            )
          }
        </Row>
      }
      {
        chartType === CHART_TYPE_DEPTH_CHART &&
        <Button
          className="exchange-chart-nav-content-button exchange-chart-nav-content-icon-button"
          type="tertiary"
          size="small"
          onClick={onFullScreenClick}>
          <FullScreen className="icon"/>
        </Button>
      }
      <Select
        style={{ width: 80, marginRight: 0 }}
        className="exchange-chart-nav-content-select"
        selectorClassName="exchange-chart-nav-content-select-selector"
        arrowClassName="exchange-chart-nav-content-select-selector-arrow"
        popoverStyle={{ borderRadius: 2, paddingBottom: 8, fontSize: '10px' }}
        popoverRowClassName="exchange-chart-nav-content-select-popover-row"
        trigger="hover"
        value={chartType === CHART_TYPE_SIMPLE_STOCK_CHART ? intl.get('simple') : (chartType === CHART_TYPE_TRADING_VIEW ? intl.get('professional') : intl.get('depth_chart'))}
        dataSource={[
          { key: CHART_TYPE_SIMPLE_STOCK_CHART, text: intl.get('simple') },
          { key: CHART_TYPE_TRADING_VIEW, text: intl.get('professional') },
          { key: CHART_TYPE_DEPTH_CHART, text: intl.get('depth_chart') }
        ]}
        onSelected={(data) => { onChartTypeChange(data.key) }}/>
    </Section>
  )
})

export default memo(Nav)
