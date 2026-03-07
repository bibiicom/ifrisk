import React, { useState, useEffect, useMemo, forwardRef, useImperativeHandle, createRef, useContext } from 'react'

import Modal from '../../../../component/modal'
import Col from '../../../../component/col'
import Row from '../../../../component/row'
import Tab from '../../../../component/tab'
import Input from '../../../../component/input'
import Message from '../../../../component/message'
import intl from '../../../../component/intl'
import ComponentContext from '../../../../component/context'

import RootContext from '../../../rootContext'

import { formatDecimal, formatValue } from '../../../../utils/format'

import { Direction } from '../../../../constants/order'

import { getMaintMarginRatio } from '../../../../api/contract'
import { increaseMargin, reduceMargin } from '../../../../api/position'
import { getAssets } from '../../../../api/account'

import './index.less'

const TAB_INCREASE = 'increase'
const TAB_REDUCE = 'reduce'

const Content = forwardRef(({ symbol, position, onTabChange, onVolumeChange }, ref) => {
  const { theme } = useContext(ComponentContext)
  const { userInfo } = useContext(RootContext)

  const [tab, setTab] = useState(TAB_INCREASE)
  const [marginRate, setMarginRate] = useState(0)
  const [account, setAccount] = useState({})
  const [volume, setVolume] = useState('')
  const [inputErrorMessage, setInputErrorMessage] = useState(null)

  useImperativeHandle(ref, () => ({
    verify: () => {
      if (!volume) {
        setInputErrorMessage(intl.get('margin_volume_not_be_empty'))
        return false
      }
      return true
    }
  }), [volume])

  useEffect(async () => {
    try {
      const result = await getMaintMarginRatio({ instrument: symbol?.InstrumentID, direct: position.Direction })
      setMarginRate(+result)
    } catch (e) {}
  }, [])

  useEffect(() => {
    if (tab === TAB_INCREASE) {
      const request = async () => {
        try {
          const result = await getAssets({ AccountID: userInfo.MemberID, Currency: symbol.ClearCurrency })
          const data = result.data || []
          if (data[0] && data[0].Available !== account.Available) {
            setAccount(data[0])
          }
        } catch (e) {}
      }
      request()
    }
  }, [tab])

  const isIncrease = tab === TAB_INCREASE
  const marginUnit = formatValue(symbol, 'ClearCurrency')
  const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
  const otherPrecision = formatValue(symbol, 'OtherPrecision', 0)

  const maxAdjustMargin = useMemo(() => {
    if (tab === TAB_INCREASE) {
      return account.Available || 0
    }
    // max[0, 持仓保证金 - 仓位占用保证金 + min(该逐仓仓位的未实现盈亏, 0)]
    const unrealizedProfitLoss = (position.PROFITPERCENT || 0) * (position.PositionCost || 0)
    return Math.max(0, position.UseMargin - position.PositionCost / position.Leverage + Math.min(0, unrealizedProfitLoss))
  }, [account.Available, tab])

  const liquidatePrice = useMemo(() => {
    const adjustMargin = +volume || 0
    if (symbol.IsInverse) {
      // 反向逻辑待补充
    } else {
      // 买加
      // 预估强平价 = {合约乘数 * abs(持仓数量) * 开仓均价 - [合约乘数 * abs(持仓数量) * 开仓均价 / 杠杆倍数 + 输入的追加资金]} / [合约乘数 * abs(持仓数量) * (1 - 强平手续费率 - MMR)]
      // 卖加
      // 预估强平价 = {合约乘数 * abs(持仓数量) * 开仓均价 + [合约乘数 * abs(持仓数量) * 开仓均价 / 杠杆倍数 + 输入的追加资金]} / [合约乘数 * abs(持仓数量) * (1 + 强平手续费率 + MMR)]
      // 买减
      // 预估强平价 = {合约乘数 * abs(持仓数量) * 开仓均价 - [合约乘数 * abs(持仓数量) * 开仓均价 / 杠杆倍数 - 输入的追加资金]} / [合约乘数 * abs(持仓数量) * (1 - 强平手续费率 - MMR)]
      // 卖减
      // 预估强平价 = {合约乘数 * abs(持仓数量) * 开仓均价 + [合约乘数 * abs(持仓数量) * 开仓均价 / 杠杆倍数 - 输入的追加资金]} / [合约乘数 * abs(持仓数量) * (1 + 强平手续费率 + MMR)]

      let newMargin
      if (tab === TAB_INCREASE) {
        newMargin = position.UseMargin + adjustMargin
        if (position.Direction === Direction.BUY) {
          newMargin = -newMargin
        }
      } else {
        newMargin = position.UseMargin - adjustMargin
        if (position.Direction === Direction.BUY) {
          newMargin = -newMargin
        }
      }
      const fee = +((symbol.Fee || {}).forceCloseRate || 0)
      let rate
      if (position.Direction === Direction.BUY) {
        rate = 1 - fee - marginRate
      } else {
        rate = 1 + fee + marginRate
      }
      return Math.max(
        0,
        (symbol.VolumeMultiple * position.Position * position.OpenPrice + newMargin) / (symbol.VolumeMultiple * position.Position * rate)
      )
    }
  }, [tab, volume, marginRate])

  const maxAdjustMarginText = formatDecimal(maxAdjustMargin, otherPrecision)

  return (
    <Col
      className={`exchange-modal-${theme}`}>
      <Tab
        className={`exchange-adjust-margin-content-tab-${theme}`}
        tabStyle={{ width: '50%' }}
        tab={tab}
        dataSource={[
          { key: TAB_INCREASE, text: intl.get('increase_margin') },
          { key: TAB_REDUCE, text: intl.get('decrease_margin') }
        ]}
        onChange={key => {
          setTab(key)
          setVolume('')
          setInputErrorMessage(null)
          onTabChange(key)
        }}/>
      <Row
        style={{ justifyContent: 'flex-end', marginTop: 40 }}
        className="exchange-modal-row">
        <span>{isIncrease ? intl.get('max_can_increase') : intl.get('max_can_decrease')}</span>:
        <span style={{ marginLeft: 4 }} className="value">
          {maxAdjustMarginText}
          &nbsp;
          {marginUnit}
        </span>
      </Row>
      <Input
        style={{ marginTop: 16 }}
        valueStyle={{ textAlign: 'right' }}
        prefix={intl.get('volume')}
        suffix={marginUnit}
        precision={otherPrecision}
        min={0}
        max={maxAdjustMargin}
        errorMessage={inputErrorMessage}
        value={volume}
        onFocus={_ => { setInputErrorMessage(null) }}
        onChange={v => {
          setVolume(v)
          onVolumeChange(v)
        }}/>
      <Row
        style={{ marginTop: 30 }}
        className="exchange-modal-row">
        <span>{intl.get('current_position_margin')}</span>
        <span className="value">{formatDecimal(formatValue(position, 'UseMargin'), otherPrecision)}&nbsp;{marginUnit}</span>
      </Row>
      <Row
        className="exchange-modal-row">
        <span>{isIncrease ? intl.get('available_balance') : intl.get('can_decrease')}</span>
        <span className="value">
          {maxAdjustMarginText}
          &nbsp;
          {marginUnit}
        </span>
      </Row>
      <Row
        style={{ marginBottom: 0 }}
        className="exchange-modal-row">
        <span>{intl.get('before_adjust_liquidation_price')}</span>
        <span className="value">{formatDecimal(liquidatePrice, pricePrecision)}&nbsp;{marginUnit}</span>
      </Row>
    </Col>
  )
})

Content.displayName = 'AdjustMarginContent'

function open ({
  isMobile, symbol, position, onSuccess, config
}) {
  const contentRef = createRef()
  let tab = TAB_INCREASE
  let volume = ''

  const request = async () => {
    const api = tab === TAB_INCREASE ? increaseMargin : reduceMargin
    try {
      await api({
        ExchangeID: symbol.ExchangeID,
        InstrumentID: symbol?.InstrumentID,
        PosiDirection: position.PosiDirection,
        Amount: +volume,
        TradeUnitID: position.TradeUnitID
      })
      onSuccess && onSuccess()
      Message.ok({ message: tab === TAB_INCREASE ? intl.get('increase_margin_success') : intl.get('decrease_margin_success'), config })
    } catch (e) {
      const message = tab === TAB_INCREASE ? intl.get('increase_margin_failed') : intl.get('decrease_margin_failed')
      Message.error({ message: `${message}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config })
    }
  }

  Modal.open({
    title: intl.get('adjust_margin'),
    width: isMobile ? '100%' : 400,
    className: isMobile ? 'exchange-modal-mobile' : '',
    content: (
      <Content
        ref={contentRef}
        symbol={symbol}
        position={position}
        onTabChange={t => { tab = t }}
        onVolumeChange={v => { volume = v }}/>
    ),
    buttons: [
      {
        children: intl.get('confirm'),
        onClick: () => {
          if (!contentRef.current.verify()) {
            return true
          }
          request()
        }
      }
    ],
    config
  })
}

const AdjustMarginModal = { open }

export default AdjustMarginModal
