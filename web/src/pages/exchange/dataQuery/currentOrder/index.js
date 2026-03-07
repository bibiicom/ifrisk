import React, { useState, useMemo, useContext, useEffect, forwardRef, useImperativeHandle, memo } from 'react'

import RootContext from '../../../rootContext'
import ExchangeContext from '../../context'

import Table from '../../../../component/table'
import Button from '../../../../component/button'
import Row from '../../../../component/row'
import Col from '../../../../component/col'
import Message from '../../../../component/message'
import List from '../../../../component/list'
import ComponentContext from '../../../../component/context'
import { Edit, Clear } from '../../../../component/icons'
import usePolling from '../../../../component/hooks/usePolling'
import useScreenSize, { ScreenSize } from '../../../../component/hooks/useScreenSize'
import intl from '../../../../component/intl'

import { ConfirmModal, StopProfitLossModal } from '../../modal'

import { Status } from '../../../../constants/auth'
import { Type as PositionType } from '../../../../constants/position'
import { Direction, Type as OrderType, LimitOrderStatus, TriggerOrderType, TriggerOrderStatus, PriceType, TradeDirection, TriggerOrderTriggerPriceType } from '../../../../constants/order'
import VolumeUnitType from '../../../../constants/volumeUnitType'

import { formatDecimal, formatValue, formatUtcTimestampToLocalTime } from '../../../../utils/format'

import {
  getCurrentLimitOrder,
  getCurrentTriggerOrder,
  revokeLimitOrder,
  revokeTriggerOrder,
  revokeOrderStopProfitLoss
} from '../../../../api/order'

import { PAGE_SIZE, POLLING_INTERVAL, UPDATE_DELAY } from '../../config'

function useGetParams (type, symbol) {
  const { userInfo } = useContext(RootContext)
  const s = symbol || {}
  const params = {
    AccountID: userInfo?.MemberID
  }
  switch (type) {
    case OrderType.PLAN: {
      params.TriggerOrderType = TriggerOrderType.PLAN
      break
    }
    case OrderType.STOP_PROFIT_LOSS: {
      params.ExchangeID = s.ExchangeID
      params.TriggerOrderType = `${TriggerOrderType.POSITION_STOP_PROFIT_LOSS}${TriggerOrderType.ORDER_STOP_PROFIT_LOSS}`
      break
    }
  }
  return params
}

export default memo(forwardRef(({ onOrderTypeChange, onLimitOrderChange, onRevokeCountChange, orderType: parentOrderType }, ref) => {
  const screenSize = useScreenSize()

  const { authStatus } = useContext(RootContext)
  const { theme } = useContext(ComponentContext)
  const { currentSymbol = {}, currentSymbolMarket, symbols, positionType, volumeUnitType, openTrade } = useContext(ExchangeContext)
  const [orderType, setOrderType] = useState(parentOrderType || OrderType.LIMIT)
  
  // 监听父组件传入的 orderType 变化
  useEffect(() => {
    if (parentOrderType !== undefined) {
      setOrderType(parentOrderType)
    }
  }, [parentOrderType])
  // console.log('order', orderType)
  const { loading, result = [], request } = usePolling({
    api: orderType === OrderType.LIMIT || orderType === OrderType.SUBSCRIPTION ? getCurrentLimitOrder : getCurrentTriggerOrder,
    params: useGetParams(orderType, currentSymbol),
    flag: currentSymbol?.InstrumentID && authStatus === Status.LOGIN,
    interval: POLLING_INTERVAL
  }, [currentSymbol?.InstrumentID, orderType, authStatus === Status.LOGIN], [orderType, authStatus === Status.LOGIN])

  // 根据订单类型过滤数据
  const filteredResult = useMemo(() => {
    // console.log('=== 订单过滤逻辑 ===')
    // console.log('1. 原始订单数据 result:', result)
    // console.log('2. 当前订单类型 orderType:', orderType)
    
    if (orderType === OrderType.SUBSCRIPTION) {
      // 申购订单:只显示包含 sub_c 的订单
      const subscriptionOrders = result.filter(order => order.Remark && order.Remark.includes('sub_c'))
      // console.log('3. 申购订单过滤结果:', subscriptionOrders)
      return subscriptionOrders
    } else if (orderType === OrderType.LIMIT) {
      // 限价订单:排除包含 sub_c 的订单
      const limitOrders = result.filter(order => !order.Remark || !order.Remark.includes('sub_c'))
      // console.log('3. 限价订单过滤结果:', limitOrders)
      return limitOrders
    }
    // console.log('3. 未过滤,返回原始数据')
    return result
  }, [result, orderType])

  const [currentPage, setCurrentPage] = useState(1)

  useEffect(() => {
    localStorage.setItem('currentOrderPage', currentPage)
  }, [currentPage])

  useEffect(() => {
    const savedPage = localStorage.getItem('currentOrderPage')
    if (savedPage) {
      setCurrentPage(parseInt(savedPage))
    }
  }, [])

  const paginatedData = useMemo(() => {
    const startIndex = (currentPage - 1) * PAGE_SIZE
    return filteredResult.slice(startIndex, startIndex + PAGE_SIZE)
  }, [filteredResult, currentPage])

  const totalPages = useMemo(() => {
    return Math.ceil(filteredResult.length / PAGE_SIZE)
  }, [filteredResult])

  useEffect(() => {
    // console.log('=== 全撤按钮调试信息 ===')
    // console.log('1. 原始订单数据 result.length:', result.length)
    // console.log('2. 当前订单类型 orderType:', orderType)
    // console.log('3. 过滤后订单数量 filteredResult.length:', filteredResult.length)
    // console.log('4. 过滤后的订单数据:', filteredResult)
    // console.log('========================')
    onRevokeCountChange && onRevokeCountChange(filteredResult.length)
  }, [filteredResult.length, result.length, orderType])

  useImperativeHandle(ref, () => ({
    updateData: request
  }))
  useEffect(() => {
    onOrderTypeChange(orderType)
  }, [orderType])
  const revokeOrderClick = (data) => {
    ConfirmModal.open({
      isMobile: screenSize === ScreenSize.MIN,
      title: intl.get('revoke_order'),
      content: intl.get('confirm_revoke_this_order'),
      onConfirm: async () => {
        const api = orderType === OrderType.LIMIT ? revokeLimitOrder : revokeTriggerOrder
        const params = {
          OrderID: data.OrderID
        }
        try {
          await api(params)
          setTimeout(() => {
            if (orderType === OrderType.LIMIT) {
              onLimitOrderChange && onLimitOrderChange()
            }
            request()
          }, UPDATE_DELAY)
          Message.ok({ message: intl.get('revoke_order_success'), config: { theme } })
        } catch (e) {
          Message.error({ message: `${intl.get('revoke_order_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config: { theme } })
        }
      },
      config: { theme }
    })
  }

  const revokeLimitOrderStopProfitLossClick = (data) => {
    ConfirmModal.open({
      isMobile: screenSize === ScreenSize.MIN,
      title: intl.get('revoke_order'),
      content: intl.get('confirm_revoke_this_order'),
      onConfirm: async () => {
        const params = {
          OrderID: data.RelatedOrderID
        }
        try {
          await revokeTriggerOrder(params)
          setTimeout(() => {
            request()
          }, UPDATE_DELAY)
          Message.ok({ message: intl.get('revoke_order_success'), config: { theme } })
        } catch (e) {
          Message.error({ message: `${intl.get('revoke_order_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config: { theme } })
        }
      },
      config: { theme }
    })
  }

  const columns = useMemo(() => {
    const isSheetVolume = volumeUnitType === VolumeUnitType.SHEET
    const volumeUnit = isSheetVolume ? intl.get('sheet') : intl.get('coin')
    
    // 申购订单的列定义
    if (orderType === OrderType.SUBSCRIPTION) {
      return [
        {
          key: 'instrument_id',
          title: intl.get('currency'),
          width: 120,
          render: (data) => {
            return (
              <Row style={{ justifyContent: 'flex-start' }}>
                <span className="symbol-name">{data.InstrumentID}</span>
              </Row>
            )
          }
        },
        {
          key: 'price',
          title: intl.get('price'),
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
            return formatDecimal(formatValue(data, 'Price'), pricePrecision)
          }
        },
        {
          key: 'cost',
          title: intl.get('launchpad2_total'),
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
            return formatDecimal(formatValue(data, 'Cost'), pricePrecision)
          }
        },
        {
          key: 'volume',
          title: intl.get('launchpad2_subscription_quantity'),
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const volumePrecision = formatValue(symbol, 'CoinPrecision', 0)
            return formatDecimal(formatValue(data, 'Volume'), volumePrecision)
          }
        },
        {
          key: 'insert_time',
          title: intl.get('launchpad2_time'),
          render: (data) => (
            <Col>
              <span>{formatUtcTimestampToLocalTime(data.InsertTime, 'YYYY-MM-DD')}</span>
              <span style={{ marginTop: -4 }}>{formatUtcTimestampToLocalTime(data.InsertTime, 'HH:mm:ss')}</span>
            </Col>
          )
        }
      ]
    }
    
    switch (orderType) {
      case OrderType.LIMIT: return [
        {
          key: 'trade_symbol',
          title: intl.get('trade_variety'),
          width: 120,
          render: (data) => {
            // const symbol = formatValue(symbols, data?.InstrumentID, {})
            return (
              <Row
                style={{ justifyContent: 'flex-start' }}>
                <span className="symbol-name">{data.InstrumentID}</span>
                {/* <span className="yellow">
                  &nbsp;·&nbsp;{data.Leverage}X
                </span> */}
              </Row>
            )
          }
        },
        {
          key: 'order_time',
          title: intl.get('order_time'),
          render: (data) => (
            <Col>
              <span>{formatUtcTimestampToLocalTime(data.InsertTime, 'YYYY-MM-DD')}</span>
              <span style={{ marginTop: -4 }}>{formatUtcTimestampToLocalTime(data.InsertTime, 'HH:mm:ss')}</span>
            </Col>
          )
        },
        {
          key: 'trade_direction',
          title: intl.get('trade_direction'),
          render: (data) => {
            return (
              <span
                className={data.Direction === Direction.BUY ? 'buy-rise-color' : 'sell-fail-color'}>
                {TradeDirection.transferDirectionOffsetFlagText(data, positionType)}
              </span>
            )
          }
        },
        {
          key: 'order_volume',
          title: `${intl.get('order_volume')}(${volumeUnit})`,
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const volumePrecision = isSheetVolume ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0)
            const volume = formatValue(data, 'Volume')
            return formatDecimal(
              VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume, price: data.Price, symbol }),
              volumePrecision
            )
          }
        },
        {
          key: 'order_price',
          title: intl.get('order_price'),
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
            return (
              data.OrderPriceType !== PriceType.LIMIT
                ? PriceType.transfer(data.OrderPriceType)
                : formatDecimal(formatValue(data, 'Price'), pricePrecision)
            )
          }
        },
        {
          key: 'trade_volume',
          title: `${intl.get('trade_volume')}(${volumeUnit})`,
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const volumePrecision = isSheetVolume ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0)
            const volume = formatValue(data, 'VolumeTraded')
            return formatDecimal(
              VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume, price: data.TradePrice, symbol }),
              volumePrecision
            )
          }
        },
        {
          key: 'trade_average_price',
          title: intl.get('trade_avp_price'),
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
            return formatDecimal(formatValue(data, 'TradePrice'), pricePrecision)
          }
        },
        positionType === PositionType.DOUBLE &&
        {
          key: 'stop_profit_or_loss_price',
          title: intl.get('stop_profit_loss_price'),
          render: data => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
            const tpTriggerPrice = formatDecimal(formatValue(data, 'TPTriggerPrice'), pricePrecision)
            const slTriggerPrice = formatDecimal(formatValue(data, 'SLTriggerPrice'), pricePrecision)
            const disabled = (!data.TPTriggerPrice) && (!data.SLTriggerPrice)
            return (
              <Row style={{ justifyContent: 'flex-end' }}>
                <span>{tpTriggerPrice}/{slTriggerPrice}</span>
                <Edit
                  className="icon"
                  onClick={_ => {
                    StopProfitLossModal.open({
                      isMobile: false,
                      form: StopProfitLossModal.FROM_CURRENT_LIMIT_ORDER,
                      positionType,
                      data,
                      symbol,
                      symbolMarket: currentSymbolMarket,
                      onSuccess: _ => {
                        setTimeout(() => {
                          request()
                        }, UPDATE_DELAY)
                      },
                      config: { theme }
                    })
                  }} />
                <Clear
                  className="icon"
                  disabled={disabled}
                  onClick={_ => {
                    if (disabled) {
                      return
                    }
                    revokeLimitOrderStopProfitLossClick(data)
                  }} />
              </Row>
            )
          }
        },
        {
          key: 'frozen_margin',
          title: intl.get('frozen_margin'),
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const otherPrecision = formatValue(symbol, 'OtherPrecision', 0)
            return formatDecimal(formatValue(data, 'FrozenMargin'), otherPrecision)
          }
        },
        {
          key: 'fee',
          title: intl.get('fee'),
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const feePrecision = formatValue(symbol, 'FeePrecision', 0)
            return formatDecimal(formatValue(data, 'Fee'), feePrecision)
          }
        },
        {
          key: 'status',
          title: intl.get('status'),
          render: (data) => LimitOrderStatus.transfer(data.OrderStatus)
        },
        {
          key: 'operation',
          title: intl.get('operation'),
          render: data => (
            <Row
              style={{ justifyContent: 'flex-end' }}>
              <Button
                className="button"
                type="tertiary"
                size="small"
                onClick={() => { revokeOrderClick(data) }}>
                {intl.get('revoke_order')}
              </Button>
            </Row>
          )
        }
      ].filter(c => !!c)
      case OrderType.PLAN: return [
        {
          key: 'trade_symbol',
          title: intl.get('trade_variety'),
          width: 120,
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            return (
              <Row>
                <span className="symbol-name">{symbol?.InstrumentID}</span>
                {/* <span className="yellow">
                  &nbsp;·&nbsp;{data.Leverage}X
                </span> */}
              </Row>
            )
          }
        },
        { key: 'order_time', title: intl.get('order_time'), render: (data) => formatUtcTimestampToLocalTime(data.InsertTime / 1000, 'YYYY-MM-DD HH:mm:ss') },
        {
          key: 'trade_direction',
          title: intl.get('trade_direction'),
          render: (data) => {
            return (
              <span
                className={data.Direction === Direction.BUY ? 'buy-rise-color' : 'sell-fail-color'}>
                {TradeDirection.transferDirectionOffsetFlagText(data, positionType)}
              </span>
            )
          }
        },
        {
          key: 'order_volume',
          title: `${intl.get('order_volume')}(${volumeUnit})`,
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const volumePrecision = isSheetVolume ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0)
            const volume = formatValue(data, 'Volume')
            return formatDecimal(
              VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume, price: data.Price, symbol }),
              volumePrecision
            )
          }
        },
        {
          key: 'trigger_price',
          title: intl.get('trigger_price'),
          render: (data) => {
            if (
              data.TriggerPriceType !== TriggerOrderTriggerPriceType.LAST_PRICE &&
              data.TPTriggerPrice !== TriggerOrderTriggerPriceType.MARK_PRICE &&
              data.TPTriggerPrice !== TriggerOrderTriggerPriceType.TARGET_INDEX_PRICE
            ) {
              const symbol = formatValue(symbols, data?.InstrumentID, {})
              const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
              return formatDecimal(formatValue(data, 'TriggerPrice'), pricePrecision)
            }
            return TriggerOrderTriggerPriceType.transfer(data.TriggerPriceType)
          }
        },
        {
          key: 'order_price',
          title: intl.get('order_price'),
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
            return (
              data.OrderPriceType !== PriceType.LIMIT
                ? PriceType.transfer(data.OrderPriceType)
                : formatDecimal(formatValue(data, 'Price'), pricePrecision)
            )
          }
        },
        {
          key: 'status',
          title: intl.get('status'),
          render: (data) => TriggerOrderStatus.transfer(data.TriggerStatus)
        },
        {
          key: 'operation',
          title: intl.get('operation'),
          render: data => (
            <Row
              style={{ justifyContent: 'flex-end' }}>
              <Button
                className="button"
                type="tertiary"
                size="small"
                onClick={() => { revokeOrderClick(data) }}>
                {intl.get('revoke_order')}
              </Button>
            </Row>
          )
        }
      ]
      case OrderType.STOP_PROFIT_LOSS: return [
        {
          key: 'trade_symbol',
          title: intl.get('trade_variety'),
          width: 120,
          render: (data) => {
            // const symbol = formatValue(symbols, data?.InstrumentID, {})
            return (
              <Row>
                <span className="symbol-name">{data.InstrumentID}</span>
                {/* <span className="yellow">
                  &nbsp;·&nbsp;{data.Leverage}X
                </span> */}
              </Row>
            )
          }
        },
        { key: 'order_time', title: intl.get('order_time'), render: (data) => formatUtcTimestampToLocalTime(data.InsertTime / 1000, 'YYYY-MM-DD HH:mm:ss') },
        { key: 'type', title: intl.get('type'), render: (data) => TriggerOrderType.transfer(data.TriggerOrderType) },
        {
          key: 'stop_profit_trigger_price',
          title: intl.get('stop_profit_trigger_price'),
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
            return formatDecimal(formatValue(data, 'TPTriggerPrice'), pricePrecision)
          }
        },
        {
          key: 'stop_loss_trigger_price',
          title: intl.get('stop_loss_trigger_price'),
          render: (data) => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
            return formatDecimal(formatValue(data, 'SLTriggerPrice'), pricePrecision)
          }
        },
        {
          key: 'status',
          title: intl.get('status'),
          render: (data) => TriggerOrderStatus.transfer(data.TriggerStatus)
        },
        {
          key: 'operation',
          title: intl.get('operation'),
          render: data => (
            <Row
              style={{ justifyContent: 'flex-end' }}>
              <Button
                className="button"
                type="tertiary"
                size="small"
                onClick={() => { revokeOrderClick(data) }}>
                {intl.get('revoke_order')}
              </Button>
            </Row>
          )
        }
      ]
    }
  }, [orderType, result, volumeUnitType])

  const renderPC = () => {
    return (
      <Table
        className="exchange-data-query-table"
        rowStyle={orderType === OrderType.LIMIT ? { height: 38 } : {}}
        columns={columns}
        loading={loading}
        dataSource={paginatedData || []}
        pagination={{
          current: currentPage,
          total: totalPages,
          onChange: (page) => {
            setCurrentPage(page)
          },
          itemRender: (page, type, element) => {
            if (type === 'page') {
              return (
                <span className={currentPage === page ? 'active-page' : ''}>
                  {page}
                </span>
              )
            }
            return element
          }
        }}
      />
    )
  }

  const renderMobile = () => {
    // 申购订单的移动端渲染
    if (orderType === OrderType.SUBSCRIPTION) {
      return (
        <List
          className="exchange-data-query-list"
          rowClassName="mobile-row"
          loading={loading}
          dataSource={paginatedData || []}
          renderRow={data => {
            const symbol = formatValue(symbols, data?.InstrumentID, {})
            const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
            const volumePrecision = formatValue(symbol, 'CoinPrecision', 0)

            return (
              <Col style={{ width: '100%' }}>
                <Row style={{ width: '100%', justifyContent: 'space-between' }}>
                  <span className="symbol-name">{data.InstrumentID}</span>
                  <span>{formatUtcTimestampToLocalTime(data.InsertTime, 'YYYY-MM-DD HH:mm:ss')}</span>
                </Row>
                <Row style={{ marginTop: 8, width: '100%' }}>
                  <Col style={{ alignItems: 'flex-start', width: '33%' }}>
                    <span className="title">{intl.get('price')}</span>
                    <span>{formatDecimal(formatValue(data, 'Price'), pricePrecision)}</span>
                  </Col>
                  <Col style={{ alignItems: 'flex-start', width: '33%' }}>
                    <span className="title">{intl.get('launchpad2_total')}</span>
                    <span>{formatDecimal(formatValue(data, 'Cost'), pricePrecision)}</span>
                  </Col>
                  <Col style={{ alignItems: 'flex-end', width: '34%' }}>
                    <span className="title">{intl.get('launchpad2_subscription_quantity')}</span>
                    <span>{formatDecimal(formatValue(data, 'Volume'), volumePrecision)}</span>
                  </Col>
                </Row>
              </Col>
            )
          }}
          pagination={{
            current: currentPage,
            total: totalPages,
            onChange: (page) => {
              setCurrentPage(page)
            },
            itemRender: (page, type, element) => {
              if (type === 'page') {
                return (
                  <span className={currentPage === page ? 'active-page' : ''}>
                    {page}
                  </span>
                )
              }
              return element
            }
          }}
        />
      )
    }
    
    return (
      <List
        className="exchange-data-query-list"
        rowClassName="mobile-row"
        loading={loading}
        dataSource={paginatedData || []}
        // renderEmpty={_ => authStatus === Status.LOGOUT && <Logout />}
        renderRow={data => {
          const isSheetVolume = volumeUnitType === VolumeUnitType.SHEET
          const volumeUnit = isSheetVolume ? intl.get('sheet') : intl.get('coin')
          const symbol = formatValue(symbols, data?.InstrumentID, {})
          const volumePrecision = isSheetVolume ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0)
          const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
          const otherPrecision = formatValue(symbol, 'OtherPrecision', 0)
          const feePrecision = formatValue(symbol, 'FeePrecision', 0)
          const volume = formatValue(data, 'Volume')
          const tradeVolume = formatValue(data, 'VolumeTraded')

          const tpTriggerPrice = formatDecimal(formatValue(data, 'TPTriggerPrice'), pricePrecision)
          const slTriggerPrice = formatDecimal(formatValue(data, 'SLTriggerPrice'), pricePrecision)
          const tpSlDisabled = (!data.TPTriggerPrice) && (!data.SLTriggerPrice)
          return (
            <Col style={{ width: '100%' }}>
              <Row style={{ width: '100%', justifyContent: 'space-between' }}>
                <Row>
                  <span className="symbol-name">{symbol?.InstrumentID}</span>
                  {/* <span className="yellow">&nbsp;·&nbsp;{data.Leverage}X</span> */}
                </Row>
                <Button
                  className="button"
                  type="tertiary"
                  size="small"
                  onClick={() => { revokeOrderClick(data) }}>
                  {intl.get('revoke_order')}
                </Button>
              </Row>
              <Row style={{ marginTop: 4 }}>
                {
                  orderType !== OrderType.STOP_PROFIT_LOSS ? (
                    <span
                      className={data.Direction === Direction.BUY ? 'buy-rise-color' : 'sell-fail-color'}>
                      {TradeDirection.transferDirectionOffsetFlagText(data, positionType)}
                    </span>
                  ) : (
                    <span>{TriggerOrderType.transfer(data.TriggerOrderType)}</span>
                  )
                }
                <span style={{ paddingLeft: 12 }}>{formatUtcTimestampToLocalTime(data.InsertTime, 'YYYY-MM-DD HH:mm:ss')}</span>
              </Row>
              {
                orderType !== OrderType.STOP_PROFIT_LOSS ? (
                  <Row style={{ marginTop: 8, width: '100%' }}>
                    <Col style={{ alignItems: 'flex-start', width: '40%' }}>
                      <span className="title">{intl.get('order_volume')}({volumeUnit})</span>
                      <span >{formatDecimal(
                        VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume, price: data.Price, symbol }),
                        volumePrecision
                      )}</span>
                    </Col>
                    <Col style={{ alignItems: 'flex-start', width: '30%' }}>
                      <span className="title">{intl.get('order_price')}</span>
                      <span>{data.OrderPriceType !== PriceType.LIMIT
                        ? PriceType.transfer(data.OrderPriceType)
                        : formatDecimal(formatValue(data, 'Price'), pricePrecision)}</span>
                    </Col>
                    {
                      orderType === OrderType.LIMIT && (
                        <Col style={{ alignItems: 'flex-end', width: '30%' }}>
                          <span className="title">
                            {intl.get('trade_volume')}({volumeUnit})
                          </span>
                          <span>{formatDecimal(
                            VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume: tradeVolume, price: data.TradePrice, symbol }),
                            volumePrecision
                          )}</span>
                        </Col>
                      )
                    }
                    {
                      orderType === OrderType.PLAN && (
                        <Col style={{ alignItems: 'flex-end', width: '30%' }}>
                          <span className="title">
                            {intl.get('trigger_price')}
                          </span>
                          <span>
                            {
                              (data.TriggerPriceType !== TriggerOrderTriggerPriceType.LAST_PRICE &&
                                data.TPTriggerPrice !== TriggerOrderTriggerPriceType.MARK_PRICE &&
                                data.TPTriggerPrice !== TriggerOrderTriggerPriceType.TARGET_INDEX_PRICE)
                                ? formatDecimal(formatValue(data, 'TriggerPrice'), pricePrecision)
                                : TriggerOrderTriggerPriceType.transfer(data.TriggerPriceType)
                            }
                          </span>
                        </Col>
                      )
                    }
                  </Row>
                ) : (
                  <Row style={{ marginTop: 8, width: '100%', justifyContent: 'space-between' }}>
                    <Col>
                      <span className="title">{intl.get('stop_profit_trigger_price')}</span>
                      <span>{formatDecimal(formatValue(data, 'TPTriggerPrice'), pricePrecision)}</span>
                    </Col>
                    <Col>
                      <span className="title">{intl.get('stop_loss_trigger_price')}</span>
                      <span>{formatDecimal(formatValue(data, 'SLTriggerPrice'), pricePrecision)}</span>
                    </Col>
                  </Row>
                )
              }

              {
                orderType === OrderType.LIMIT && (
                  <Row style={{ marginTop: 6, width: '100%' }}>
                    <Col style={{ alignItems: 'flex-start', width: '40%' }}>
                      <span className="title">{intl.get('stop_profit_loss_price')}</span>
                      <Row>
                        <span>{tpTriggerPrice}/{slTriggerPrice}</span>
                        <Edit
                          className="icon"
                          onClick={_ => {
                            StopProfitLossModal.open({
                              isMobile: true,
                              from: StopProfitLossModal.FROM_CURRENT_LIMIT_ORDER,
                              positionType,
                              data,
                              symbol,
                              symbolMarket: currentSymbolMarket,
                              onSuccess: _ => {
                                setTimeout(() => {
                                  request()
                                }, UPDATE_DELAY)
                              },
                              config: { theme }
                            })
                          }} />
                        <Clear
                          className="icon"
                          disabled={tpSlDisabled}
                          onClick={_ => {
                            if (tpSlDisabled) {
                              return
                            }
                            revokeLimitOrderStopProfitLossClick(data)
                          }} />
                      </Row>
                    </Col>
                    <Col style={{ alignItems: 'flex-start', width: '30%' }}>
                      <span className="title">{intl.get('frozen_margin')}</span>
                      <span>{formatDecimal(formatValue(data, 'FrozenMargin'), otherPrecision)}</span>
                    </Col>
                    <Col style={{ alignItems: 'flex-end', width: '30%' }}>
                      <span className="title">{intl.get('fee')}</span>
                      <span>{formatDecimal(formatValue(data, 'Fee'), feePrecision)}</span>
                    </Col>
                  </Row>
                )
              }
            </Col>
          )
        }}
        pagination={{
          current: currentPage,
          total: totalPages,
          onChange: (page) => {
            setCurrentPage(page)
          },
          itemRender: (page, type, element) => {
            if (type === 'page') {
              return (
                <span className={currentPage === page ? 'active-page' : ''}>
                  {page}
                </span>
              )
            }
            return element
          }
        }}
      />
    )
  }

  return (
    <>
      {screenSize === ScreenSize.MIN ? renderMobile() : renderPC()}
    </>
  )
}))
