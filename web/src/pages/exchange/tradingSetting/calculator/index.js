import React, { useState, useContext, useEffect, useMemo, useRef, memo } from 'react'
import axios from 'axios'

import Modal from '../../../../component/modal/Modal'
import Row from '../../../../component/row'
import Col from '../../../../component/col'
import Select from '../../../../component/select'
import Button from '../../../../component/button'
import Input from '../../../../component/input'
import Tab from '../../../../component/tab'
import useScreenSize, { ScreenSize } from '../../../../component/hooks/useScreenSize'
import { isValid } from '../../../../component/utils/typeChecks'
import intl from '../../../../component/intl'
import ComponentContext from '../../../../component/context'

import RootContext from '../../../rootContext'
import ExchangeContext from '../../context'

import LeverSlider from '../../components/leverSlider'

import { Status } from '../../../../constants/auth'
import { Direction } from '../../../../constants/position'
import VolumeUnitType from '../../../../constants/volumeUnitType'
import { transferName } from '../../../../constants/symbol'

import { formatPercent, formatValue, formatDecimal } from '../../../../utils/format'

import { getTradeConfig, getFirstStageMarginRate } from '../../../../api/contract'
import { getAssets } from '../../../../api/account'

import './index.less'
import { Calculator } from '../../../../component/icons'

const TAB_INCOME = 'income'
const TAB_LIQUIDATE = 'liquidate_price'

const MyCalculator = (({
  onClose
}) => {
  const accounts = useRef({})
  const levers = useRef({})
  const marginRates = useRef({})
  const directionTemp = useRef(Direction.LONG)

  const tradeConfigRequestCancel = useRef()
  const firstStageMarginRateRequestCancel = useRef()
  const assetsRequestCancel = useRef()

  const { theme } = useContext(ComponentContext)
  const { authStatus, userInfo } = useContext(RootContext)
  const { currentSymbol = {}, symbols = {}, volumeUnitType } = useContext(ExchangeContext)

  const screenSize = useScreenSize()

  const [tab, setTab] = useState(TAB_INCOME)
  const [selectDataSource, setSelectDataSource] = useState([])
  const [selectCurrentValue, setSelectCurrentValue] = useState({})
  const [direction, setDirection] = useState(Direction.LONG)
  const [lever, setLever] = useState({ long: 10, short: 10, longMax: 125, shortMax: 125 })
  const [openPrice, setOpenPrice] = useState('')
  const [volume, setVolume] = useState('')
  const [closePrice, setClosePrice] = useState('')
  const [available, setAvailable] = useState(0)
  const [marginRate, setMarginRate] = useState(0)

  useEffect(() => {
    if (currentSymbol?.InstrumentID) {
      const productGroup = currentSymbol?.ProductGroup
      const selectDataSource = []
      let selectCurrentValue
      for (const key in symbols) {
        const symbol = symbols[key]
        if (symbol?.ProductGroup === productGroup) {
          const item = { key: symbol?.InstrumentID, symbol, text: <Row style={{ fontWeight: 600 }}>{transferName(symbol)}</Row> }
          if (symbol?.InstrumentID === currentSymbol?.InstrumentID) {
            selectCurrentValue = item
          }
          selectDataSource.push(item)
        }
      }
      setSelectDataSource(selectDataSource)
      setSelectCurrentValue(selectCurrentValue)
    }
  }, [currentSymbol?.InstrumentID])

  useEffect(() => {
    const symbol = selectCurrentValue.symbol
    if (symbol && authStatus === Status.LOGIN) {
      if (levers.current[symbol?.InstrumentID]) {
        setLever({ ...levers.current[symbol?.InstrumentID] })
      } else {
        const id = userInfo.MemberID
        const request = async _ => {
          try {
            const data = await getTradeConfig({
              MemberID: id,
              AccountID: id,
              TradeUnitID: id,
              InstrumentID: symbol?.InstrumentID,
              ExchangeID: symbol.ExchangeID
            }, { cancelToken: new axios.CancelToken(c => { tradeConfigRequestCancel.current = c }) })
            const defaultLever = symbol.DefaultLeverage || 10
            if (data) {
              const l = {
                long: data.LongLeverage || defaultLever,
                short: data.ShortLeverage || defaultLever,
                longMax: data.LongMaxLeverage || defaultLever,
                shortMax: data.ShortMaxLeverage || defaultLever
              }
              levers.current[symbol?.InstrumentID] = l
              setLever({ ...l })
            }
          } catch (e) {
          }
        }
        request()
      }
    }
    return () => {
      if (tradeConfigRequestCancel.current) {
        tradeConfigRequestCancel.current({ code: -100 })
        tradeConfigRequestCancel.current = null
      }
    }
  }, [(selectCurrentValue.symbol || {})?.InstrumentID, authStatus, userInfo])

  useEffect(() => {
    const symbol = selectCurrentValue.symbol
    if (symbol && authStatus === Status.LOGIN && tab === TAB_LIQUIDATE) {
      if (isValid(accounts[symbol?.InstrumentID])) {
        setAvailable(+accounts[symbol?.InstrumentID])
      } else {
        const request = async _ => {
          try {
            const result = await getAssets(
              { AccountID: userInfo.MemberID, Currency: symbol.ClearCurrency },
              { cancelToken: new axios.CancelToken(c => { assetsRequestCancel.current = c }) })
            const data = result.data || []
            if (data[0]) {
              accounts.current[symbol?.InstrumentID] = data[0].Available
              setAvailable(data[0].Available)
            }
          } catch (e) {}
        }
        request()
      }
    }
    return () => {
      if (assetsRequestCancel.current) {
        assetsRequestCancel.current({ code: -100 })
        assetsRequestCancel.current = null
      }
    }
  }, [(selectCurrentValue.symbol || {})?.InstrumentID, authStatus, tab])

  useEffect(() => {
    const symbol = selectCurrentValue.symbol
    if (symbol && authStatus === Status.LOGIN && tab === TAB_LIQUIDATE) {
      if (marginRates.current[symbol?.InstrumentID] && isValid(marginRates.current[symbol?.InstrumentID][direction])) {
        setMarginRate(marginRates.current[symbol?.InstrumentID][direction])
      } else {
        const request = async _ => {
          try {
            const result = await getFirstStageMarginRate(
              { InstrumentID: symbol?.InstrumentID, ExchangeID: symbol.ExchangeID },
              { cancelToken: new axios.CancelToken(c => { firstStageMarginRateRequestCancel.current = c }) }
            )
            const rate = (result[0] || {}).data || {}
            marginRates.current[symbol?.InstrumentID] = { [Direction.LONG]: rate.LongMaintMarginRatio || 0, [Direction.SHORT]: rate.ShortMaintMarginRatio || 0 }
            setMarginRate(marginRates.current[symbol?.InstrumentID][direction])
          } catch (e) {}
        }
        request()
      }
    }
    return () => {
      if (firstStageMarginRateRequestCancel.current) {
        firstStageMarginRateRequestCancel.current({ code: -100 })
        firstStageMarginRateRequestCancel.current = null
      }
    }
  }, [(selectCurrentValue.symbol || {})?.InstrumentID, authStatus, tab, direction])

  const reset = (tab) => {
    setTab(tab)
    setDirection(Direction.LONG)
    setOpenPrice('')
    setVolume('')
    setClosePrice('')
    setAvailable(0)
  }

  const isSheetVolume = volumeUnitType === VolumeUnitType.SHEET
  const priceUnit = formatValue(selectCurrentValue.symbol, 'PriceCurrency')
  const pricePrecision = formatValue(selectCurrentValue.symbol, 'PricePrecision', 0)
  const volumeUnit = isSheetVolume ? intl.get('sheet') : formatValue(selectCurrentValue.symbol, 'BaseCurrency')
  const volumePrecision = isSheetVolume ? formatValue(selectCurrentValue.symbol, 'VolumePrecision', 0) : formatValue(selectCurrentValue.symbol, 'CoinPrecision', 0)
  const otherPrecision = formatValue(selectCurrentValue.symbol, 'OtherPrecision', 0)
  const marginUnit = formatValue(selectCurrentValue.symbol, 'ClearCurrency')
  const leverKey = direction === Direction.LONG ? 'long' : 'short'
  const leverMaxKey = direction === Direction.LONG ? 'longMax' : 'shortMax'

  const incomeResult = useMemo(() => {
    if (tab === TAB_INCOME) {
      const symbol = selectCurrentValue.symbol
      let incomeRate = '--'
      let income = '--'
      let margin = '--'
      if (symbol) {
        const feeRate = +((symbol.Fee || {}).takerOpenFeeRate || 0)
        if (symbol && (+openPrice) && (+closePrice) && (+volume)) {
          let l
          let difPrice
          if (direction === Direction.LONG) {
            difPrice = (+closePrice) - (+openPrice)
            l = lever.long
          } else {
            difPrice = (+openPrice) - (+closePrice)
            l = lever.short
          }
          const sheetVolume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume: +volume, price: +openPrice, symbol })
          const coinVolume = VolumeUnitType.sheetToCoin({ volume: sheetVolume, price: +openPrice, symbol })
          const incomeMoney = difPrice * coinVolume
          if (symbol.IsInverse) {
            const fee = coinVolume * feeRate
            income = incomeMoney / (+closePrice) - fee
            margin = coinVolume / l + fee
            incomeRate = income * (+closePrice) / (+openPrice * coinVolume) * l
          } else {
            const fee = coinVolume * (+openPrice) * feeRate
            income = incomeMoney - fee
            margin = coinVolume * (+openPrice) / l + fee
            incomeRate = income / (coinVolume * (+openPrice)) * l
          }
        }
      }
      return {
        incomeRate, income, margin
      }
    }
  }, [direction, volume, openPrice, closePrice, lever, volumeUnitType, selectCurrentValue.symbol])

  const liquidationPrice = useMemo(() => {
    if (tab === TAB_LIQUIDATE) {
      const lPrice = '--'
      const symbol = selectCurrentValue.symbol
      if (symbol && openPrice !== '' && available && (+volume)) {
        const sheetVolume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume: +volume, price: +openPrice, symbol })
        const coinVolume = VolumeUnitType.sheetToCoin({ volume: sheetVolume, price: +openPrice, symbol })

        if (symbol.IsInverse) {
          // 反向待补充
        } else {
          // 买加
          // 预估强平价 = {合约乘数 * abs(持仓数量) * 开仓均价 - [合约乘数 * abs(持仓数量) * 开仓均价 / 杠杆倍数 + 输入的追加资金]} / [合约乘数 * abs(持仓数量) * (1 - 强平手续费率 - MMR)]
          // 卖加
          // 预估强平价 = {合约乘数 * abs(持仓数量) * 开仓均价 + [合约乘数 * abs(持仓数量) * 开仓均价 / 杠杆倍数 + 输入的追加资金]} / [合约乘数 * abs(持仓数量) * (1 - 强平手续费率 - MMR)]

          // 买减
          // 预估强平价 = {合约乘数 * abs(持仓数量) * 开仓均价 - [合约乘数 * abs(持仓数量) * 开仓均价 / 杠杆倍数 - 输入的追加资金]} / [合约乘数 * abs(持仓数量) * (1 + 强平手续费率 + MMR)]
          // 卖减
          // 预估强平价 = {合约乘数 * abs(持仓数量) * 开仓均价 + [合约乘数 * abs(持仓数量) * 开仓均价 / 杠杆倍数 - 输入的追加资金]} / [合约乘数 * abs(持仓数量) * (1 + 强平手续费率 + MMR)]
          const fee = symbol.Fee || {}
          const cost = coinVolume * (+openPrice)
          const openFeeRate = +(fee.takerOpenFeeRate || 0)
          const l = lever[direction === Direction.LONG ? 'long' : 'short']
          if (cost / l + cost * openFeeRate <= available) {
            const forceCloseFeeRate = +(fee.forceCloseRate || 0)
            let margin
            let rate
            if (direction === Direction.LONG) {
              margin = -available
              rate = 1 - forceCloseFeeRate - marginRate
            } else {
              margin = available
              rate = 1 + forceCloseFeeRate + marginRate
            }
            const price = (coinVolume * (+openPrice) + margin) / (coinVolume * rate)
            if (price > 0) {
              return price
            }
          }
        }
      }
      return lPrice
    }
  }, [direction, volume, openPrice, available, lever, marginRate, volumeUnitType, tab, selectCurrentValue.symbol])

  const result = tab === TAB_INCOME ? incomeResult : liquidationPrice

  const renderCommonInput = () => {
    let prefix
    let suffix
    let precision
    let disabled
    let value
    switch (tab) {
      case TAB_INCOME: {
        prefix = intl.get('close_position_price')
        suffix = priceUnit
        precision = pricePrecision
        value = closePrice
        break
      }
      case TAB_LIQUIDATE: {
        prefix = intl.get('wallet_balance')
        suffix = marginUnit
        precision = otherPrecision
        disabled = true
        value = formatDecimal(available, precision)
        break
      }
    }
    return (
      <Input
        valueStyle={{ textAlign: 'right' }}
        prefix={prefix}
        suffix={suffix}
        precision={precision}
        disabled={disabled}
        value={value}
        onChange={setClosePrice}/>
    )
  }

  const renderResult = () => {
    switch (tab) {
      case TAB_INCOME: {
        return (
          <>
            <>
              <p style={{ marginTop: 40 }} className="result-label">{intl.get('rate_return')}</p>
              <p className="result-value">{formatPercent(result.incomeRate)}</p>
            </>
            <>
              <p className="result-label">{intl.get('profit')}</p>
              <p className="result-value">{formatDecimal(result.income, otherPrecision)}&nbsp;{marginUnit}</p>
            </>
            <>
              <p className="result-label">{intl.get('start_margin')}</p>
              <p className="result-value">{formatDecimal(result.margin, otherPrecision)}&nbsp;{marginUnit}</p>
            </>
          </>
        )
      }
      case TAB_LIQUIDATE: {
        return (
          <>
            <p style={{ marginTop: 40 }} className="result-label">{intl.get('L0007308强平价')}</p>
            <p className="result-value">{formatDecimal(result, pricePrecision)}&nbsp;{priceUnit}</p>
          </>
        )
      }
    }
  }

  return (
    <Modal
      className={screenSize === ScreenSize.MIN ? 'exchange-calculator-mobile' : ''}
      title={
        <Tab
          style={{ height: 40 }}
          tabStyle={{ fontSize: 16, padding: 0, marginRight: 24, fontWeight: 'normal' }}
          tabPosition="bottom"
          tab={tab}
          dataSource={[
            { key: TAB_INCOME, text: intl.get('profit') },
            { key: TAB_LIQUIDATE, text: intl.get('liquidation_price') }
          ]}
          onChange={tab => { reset(tab) }}/>
      }
      width={screenSize === ScreenSize.MIN ? '100%' : 800}
      onClose={onClose}>
      <Row
        className={`exchange-calculator-content-${theme}`}>
        <Col
          className="item left-item">
          <Select
            style={{ height: 44 }}
            selectorStyle={{ padding: '0 12px' }}
            popoverRowStyle={{ height: 46, padding: '0 12px' }}
            dataSource={selectDataSource}
            value={selectCurrentValue}
            onSelected={(v) => {
              if (v.symbol !== selectCurrentValue.symbol) {
                reset(tab)
                setSelectCurrentValue(v)
              }
            }}/>
          <Row
            style={{ justifyContent: 'space-between', margin: '24px 0' }}>
            <Button
              className={direction === Direction.LONG ? 'buy-rise-button' : ''}
              style={{ width: 'calc(50% - 6px)' }}
              type="tertiary"
              onClick={_ => {
                directionTemp.current = Direction.LONG
                setDirection(Direction.LONG)
              }}>
              {intl.get('open_much')}
            </Button>
            <Button
              className={direction === Direction.SHORT ? 'sell-fail-button' : ''}
              style={{ width: 'calc(50% - 6px)' }}
              type="tertiary"
              onClick={_ => {
                directionTemp.current = Direction.SHORT
                setDirection(Direction.SHORT)
              }}>
              {intl.get('open_void')}
            </Button>
          </Row>
          <LeverSlider
            lever={lever[leverKey]}
            maxLever={Math.floor(lever[leverMaxKey])}
            onChange={l => {
              setLever(v => {
                const key = directionTemp.current === Direction.LONG ? 'long' : 'short'
                return { ...v, [key]: l }
              })
            }}
          />
          <Input
            style={{ marginTop: 64 }}
            valueStyle={{ textAlign: 'right' }}
            prefix={intl.get('open_position_price')}
            suffix={priceUnit}
            precision={pricePrecision}
            value={openPrice}
            onChange={setOpenPrice}
          />
          <Input
            style={{ margin: '8px 0' }}
            valueStyle={{ textAlign: 'right' }}
            prefix={intl.get('volume')}
            suffix={volumeUnit}
            precision={volumePrecision}
            value={volume}
            onChange={setVolume}/>
          {renderCommonInput()}
        </Col>
        <i className="split-line"/>
        <Col
          className="item right-item">
          <p className="result-title">{intl.get('calc_result')}</p>
          {renderResult()}
        </Col>
      </Row>
    </Modal>
  )
})

export default memo(MyCalculator)
