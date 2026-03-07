import React, { useState, useContext, forwardRef, useImperativeHandle } from 'react'

import Col from '../../../../component/col'
import Row from '../../../../component/row'
import Modal from '../../../../component/modal'
import Input from '../../../../component/input'
import Message from '../../../../component/message'
import ComponentContext from '../../../../component/context'
import intl from '../../../../component/intl'

import { transferDirectionTag } from '../../../../constants/position'
import { Direction as OrderDirection, PriceType, OffsetFlag, InstructType, TriggerOrderType, TradeDirection } from '../../../../constants/order'

import { formatDecimal, formatValue } from '../../../../utils/format'

import { triggerOrder, editOrderStopProfitLoss } from '../../../../api/order'

const FROM_ORDER_BOARD = 'order_board'
const FROM_POSITION = 'position'
const FROM_CURRENT_LIMIT_ORDER = 'current_limit_order'

const ERROR_INPUT_TYPE_NONE = 'none'
const ERROR_INPUT_TYPE_TP = 'tp'
const ERROR_INPUT_TYPE_SL = 'sl'

const Content = forwardRef(({
  from, positionType, symbol, symbolMarket, data,
  onStopProfitTriggerPriceChange,
  onStopLossTriggerPriceChange
}, ref) => {
  const { theme } = useContext(ComponentContext)

  const [stopProfitPrice, setStopProfitPrice] = useState(data.TPTriggerPrice || '')
  const [stopLossPrice, setStopLossPrice] = useState(data.SLTriggerPrice || '')

  const [errorInputType, setErrorInputType] = useState(ERROR_INPUT_TYPE_NONE)
  const [errorMessage, setErrorMessage] = useState('')

  const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
  const priceUnit = formatValue(symbol, 'PriceCurrency')

  useImperativeHandle(ref, () => ({
    setError: (errorInputType, errorMessage) => {
      setErrorInputType(errorInputType)
      setErrorMessage(errorMessage)
    }
  }))

  const renderTag = () => {
    // console.log('data', priceUnit, pricePrecision)
    const isFromPosition = from === FROM_POSITION
    const isBuy = data.Direction === OrderDirection.BUY
    const tagText = isFromPosition ? transferDirectionTag(data) : TradeDirection.transferDirectionOffsetFlagText(data, positionType)

    return (
      <span
        className={`exchange-modal-tag ${
          isBuy ? 'buy-rise-border-background-opacity-10' : 'sell-fail-border-background-opacity-10'
        } ${
          isBuy ? 'buy-rise-color' : 'sell-fail-color'
        }`}>
        {tagText}
      </span>
    )
  }

  return (
    <Col className={`exchange-modal-${theme}`}>
      <Row
        className='exchange-modal-symbol'>
        <span>{symbol?.InstrumentID}</span>
        {from !== FROM_ORDER_BOARD && renderTag()}
      </Row>
      <Row
        className='exchange-modal-row'>
        <span>
          {from === FROM_POSITION ? intl.get('open_position_avp_price') : intl.get('order_price')}
        </span>
        <span
          className="value">
          {
            from === FROM_POSITION
              ? `${formatDecimal(data.OpenPrice, pricePrecision)} ${priceUnit}`
              : (data.OrderPriceType === PriceType.LIMIT ? `${formatDecimal(data.Price, pricePrecision)} ${priceUnit}` : PriceType.transfer(data.OrderPriceType))
          }
        </span>
      </Row>
      <Row
        style={{ marginBottom: 40 }}
        className="exchange-modal-row">
        <span>{intl.get('last_price')}</span>
        <span
          className="value">
          {formatDecimal(formatValue(symbolMarket, 'LastPrice'), pricePrecision)}
          &nbsp;{priceUnit}
        </span>
      </Row>
      <Input
        className="exchange-modal-input"
        valueStyle={{ textAlign: 'right' }}
        min={0}
        precision={pricePrecision}
        prefix={intl.get('stop_profit_trigger_price')}
        suffix={priceUnit}
        value={stopProfitPrice}
        errorMessage={errorInputType === ERROR_INPUT_TYPE_TP ? errorMessage : ''}
        onFocus={_ => {
          setErrorInputType(ERROR_INPUT_TYPE_NONE)
          setErrorMessage('')
        }}
        onChange={v => {
          setStopProfitPrice(v)
          onStopProfitTriggerPriceChange(v)
        }}/>
      <Input
        className="exchange-modal-input"
        valueStyle={{ textAlign: 'right' }}
        min={0}
        precision={pricePrecision}
        prefix={intl.get('stop_loss_trigger_price')}
        suffix={priceUnit}
        value={stopLossPrice}
        errorMessage={errorInputType === ERROR_INPUT_TYPE_SL ? errorMessage : ''}
        onFocus={_ => {
          setErrorInputType(ERROR_INPUT_TYPE_NONE)
          setErrorMessage('')
        }}
        onChange={v => {
          setStopLossPrice(v)
          onStopLossTriggerPriceChange(v)
        }}/>
    </Col>
  )
})

Content.displayName = 'StopProfitLossContent'

function open ({
  isMobile, from, positionType, data, symbol, symbolMarket, onSuccess, onFailed, config
}) {
  let tpTriggerPrice = data.TPTriggerPrice || ''
  let slTriggerPrice = data.SLTriggerPrice || ''

  Modal.open({
    className: isMobile ? 'exchange-modal-mobile' : '',
    width: isMobile ? '100%' : 0,
    title: intl.get('stop_profit_loss'),
    content: (
      <Content
        from={from}
        positionType={positionType}
        symbol={symbol}
        symbolMarket={symbolMarket}
        data={data}
        onStopProfitTriggerPriceChange={v => { tpTriggerPrice = v }}
        onStopLossTriggerPriceChange={v => { slTriggerPrice = v }}/>
    ),
    buttons: [
      {
        children: intl.get('cancel'),
        type: 'tertiary',
        text: true
      },
      {
        children: intl.get('confirm'),
        onClick: () => {
          switch (from) {
            case FROM_ORDER_BOARD: {
              onSuccess && onSuccess({ tpTriggerPrice, slTriggerPrice })
              break
            }
            case FROM_POSITION: {
              if (tpTriggerPrice || slTriggerPrice) {
                const params = {
                  InstrumentID: data?.InstrumentID,
                  ExchangeID: data.ExchangeID,
                  Direction: data.Direction === OrderDirection.BUY ? OrderDirection.SELL : OrderDirection.BUY,
                  OffsetFlag: OffsetFlag.CLOSE_ALL,
                  OrderType: InstructType.NORMAL,
                  TradeUnitID: data.TradeUnitID,
                  TriggerOrderType: TriggerOrderType.POSITION_STOP_PROFIT_LOSS
                }
                if (tpTriggerPrice) {
                  params.TPTriggerPrice = +tpTriggerPrice
                }
                if (slTriggerPrice) {
                  params.SLTriggerPrice = +slTriggerPrice
                }
                const request = async () => {
                  try {
                    await triggerOrder(params)
                    Message.ok({ message: intl.get('set_stop_profit_loss_success'), config })
                    onSuccess && onSuccess()
                  } catch (e) {
                    Message.error({ message: `${intl.get('set_stop_profit_loss_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config })
                    onFailed && onFailed()
                  }
                }
                request()
              }
              break
            }
            case FROM_CURRENT_LIMIT_ORDER: {
              if (tpTriggerPrice || slTriggerPrice) {
                const params = {
                  TriggerOrderType: TriggerOrderType.ORDER_STOP_PROFIT_LOSS
                }
                let api
                if (data.TPTriggerPrice || data.SLTriggerPrice) {
                  params.OrderID = data.OrderID
                  api = editOrderStopProfitLoss
                } else {
                  params.RelatedOrderID = data.OrderID
                  params.InstrumentID = data.InstrumentID
                  params.ExchangeID = data.ExchangeID
                  api = triggerOrder
                }
                if (tpTriggerPrice) {
                  params.TPTriggerPrice = +tpTriggerPrice
                }
                if (slTriggerPrice) {
                  params.SLTriggerPrice = +slTriggerPrice
                }
                const request = async () => {
                  try {
                    await api(params)
                    Message.ok({ message: intl.get('set_stop_profit_loss_success'), config })
                    onSuccess && onSuccess()
                  } catch (e) {
                    Message.error({ message: `${intl.get('set_stop_profit_loss_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config })
                    onFailed && onFailed()
                  }
                }
                request()
              }
              break
            }
          }
        }
      }
    ],
    config
  })
}

const StopProfitLossModal = {
  open,
  FROM_ORDER_BOARD,
  FROM_POSITION,
  FROM_CURRENT_LIMIT_ORDER
}

export default StopProfitLossModal
