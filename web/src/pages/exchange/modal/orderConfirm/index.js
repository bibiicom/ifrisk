import React, { useContext } from 'react'

import Col from '../../../../component/col'
import Row from '../../../../component/row'
import Checkbox from '../../../../component/checkbox'
import Message from '../../../../component/message'
import Modal from '../../../../component/modal'
import ComponentContext from '../../../../component/context'
import intl from '../../../../component/intl'

import { ProductGroup } from '../../../../constants/symbol'
import { Type, PriceType, Direction, OffsetFlag, VolumeMode, TradeDirection, TriggerOrderTriggerPriceType } from '../../../../constants/order'
import VolumeUnitType from '../../../../constants/volumeUnitType'

import { formatDecimal, formatValue } from '../../../../utils/format'

import { limitOrder, triggerOrder, orderStopProfitLoss } from '../../../../api/order'

function open ({
  isMobile, orderConfirm, positionType, orderType, requestParams, symbol, onSuccess, onFailed, config
}) {
  const orderRequest = async () => {
    // 删除用来计算保证金的参数
    delete requestParams.extend
    let api
    switch (orderType) {
      case Type.LIMIT: {
        api = limitOrder
        break
      }
      case Type.PLAN: {
        api = triggerOrder
        break
      }
      case Type.STOP_PROFIT_LOSS: {
        api = orderStopProfitLoss
        break
      }
    }
    try {
      if (requestParams.TriggerPriceType === TriggerOrderTriggerPriceType.NORMAL) {
        delete requestParams.TriggerPriceType
      }
      const result = await api(requestParams)
      onSuccess && onSuccess(result)
      Message.ok({ message: intl.get('order_success'), config })
    } catch (e) {
      onFailed && onFailed(e)
      Message.error({ message: `${intl.get('order_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config })
    }
  }

  if (orderConfirm.confirm) {
    const extendParams = requestParams.extend
    const isSheetVolume = extendParams.volumeUnitType === VolumeUnitType.SHEET
    const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
    const volumePrecision = isSheetVolume ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0)
    const otherPrecision = formatValue(symbol, 'OtherPrecision', 0)
    const priceUnit = symbol.PriceCurrency
    const marginUnit = symbol.ClearCurrency
    const volumeUnit = isSheetVolume ? intl.get('sheet') : formatValue(symbol, 'BaseCurrency')
    const Content = () => {
      const { theme } = useContext(ComponentContext)
      return (
        <Col className={`exchange-modal-${theme}`}>
          <Row className="exchange-modal-symbol">
            <span>{symbol?.InstrumentID}</span>
            <span
              className={`exchange-modal-tag ${
                requestParams.Direction === Direction.BUY ? 'buy-rise-border-background-opacity-10' : 'sell-fail-border-background-opacity-10'
              } ${
                requestParams.Direction === Direction.BUY ? 'buy-rise-color' : 'sell-fail-color'
              }`}>
              {TradeDirection.transferDirectionOffsetFlagText(requestParams, positionType)}
            </span>
          </Row>
          {
            orderType === Type.PLAN &&
            <Row className='exchange-modal-row'>
              <span>{intl.get('trigger_price')}</span>
              <span className='value'>
                {
                  requestParams.TriggerPriceType === TriggerOrderTriggerPriceType.NORMAL
                    ? `${formatDecimal(requestParams.TriggerPrice, pricePrecision)} ${priceUnit}`
                    : TriggerOrderTriggerPriceType.transfer(requestParams.TriggerPriceType)
                }
              </span>
            </Row>
          }
          <Row className='exchange-modal-row'>
            <span>{intl.get('price')}</span>
            <span className='value'>
              {
                requestParams.OrderPriceType === PriceType.LIMIT
                  ? `${formatDecimal(requestParams.Price, pricePrecision)} ${priceUnit}`
                  : PriceType.transfer(requestParams.OrderPriceType)
              }
            </span>
          </Row>
          <Row className="exchange-modal-row">
            <span>{intl.get('volume')}</span>
            <span className="value">
              {
                requestParams.OffsetFlag === OffsetFlag.CLOSE_ALL
                  ? `${intl.get('all')}`
                  : `${extendParams.volumeMode === VolumeMode.PERCENT ? `${extendParams.showVolume}% ` : formatDecimal(extendParams.showVolume, volumePrecision)}`
              }
              &nbsp;{volumeUnit}
            </span>
          </Row>
          {
            (orderType !== Type.PLAN && requestParams.OffsetFlag === OffsetFlag.OPEN && requestParams?.ProductGroup !== ProductGroup.SPOT) &&
            <Row className="exchange-modal-row">
              <span>{intl.get('margin')}</span>
              <span className="value">
                {formatDecimal(formatValue(extendParams, 'margin', otherPrecision))}&nbsp;{marginUnit}
              </span>
            </Row>
          }
          {
            (orderType !== Type.PLAN && requestParams.OffsetFlag === OffsetFlag.OPEN) &&
            <>
              <Row className="exchange-modal-row">
                <span>{intl.get('stop_profit')}</span>
                <span className="value">
                  {formatDecimal(formatValue(requestParams, 'TPTriggerPrice'), pricePrecision)}&nbsp;{priceUnit}
                </span>
              </Row>
              <Row className="exchange-modal-row">
                <span>{intl.get('stop_loss')}</span>
                <span className="value">
                  {formatDecimal(formatValue(requestParams, 'SLTriggerPrice'), pricePrecision)}&nbsp;{priceUnit}
                </span>
              </Row>
            </>
          }
          <Checkbox style={{ marginTop: 26 }} label={intl.get('not_remind')} onChecked={checked => { orderConfirm.setOrderConfirm(!checked) }}/>
        </Col>
      )
    }

    Modal.open({
      width: isMobile ? '86%' : 0,
      title: intl.get('confirm_order'),
      content: <Content/>,
      buttons: [
        {
          children: intl.get('cancel'),
          type: 'tertiary',
          text: true
        },
        {
          children: intl.get('confirm'),
          onClick: _ => { orderRequest() }
        }
      ],
      config
    })
  } else {
    orderRequest()
  }
}

const OrderConfirmModal = { open }

export default OrderConfirmModal
