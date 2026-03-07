import React, { useState, useEffect, useMemo, useContext } from 'react'

import Select from '../../component/select'
import Table from '../../component/table'
import Row from '../../component/row'

import intl from '../../component/intl'

import { OrderDetailModal } from '../exchange/modal'

import RootContext from '../rootContext'

import { getSymbolList } from '../../api/symbol'
import { getHistoryLimitOrder, getHistoryTriggerOrder } from '../../api/order'

import { ProductGroup, transferName } from '../../constants/symbol'
import {
  Type as OrderType,
  TriggerOrderType,
  LimitOrderStatus,
  TriggerOrderStatus,
  TradeDirection,
  Direction,
  PriceType,
  TriggerOrderTriggerPriceType
} from '../../constants/order'

import { Status } from '../../constants/auth'
import { Type as PositionType } from '../../constants/position'
import VolumeUnitType from '../../constants/volumeUnitType'

import {
  formatDecimal,
  formatValue,
  formatUtcTimestampToLocalTime
} from '../../utils/format'
import Col from '../../component/col'
import Button from '../../component/button'

const HistoryOrderContent = ({ productGroup }) => {
  const { authStatus } = useContext(RootContext)

  const [symbolList, setSymbolList] = useState([])
  const [symbol, setSymbol] = useState({})
  const [orderType, setOrderType] = useState(OrderType.LIMIT)
  const [tradeDirection, setTradeDirection] = useState(TradeDirection.ALL)
  // const [status, setStatus] = useState(LimitOrderStatus.ALL)
  const [currentPage, setCurrentPage] = useState(1)
  const [loading, setLoading] = useState(true)
  const [orderList, setOrderList] = useState([])

  useEffect(() => {
    if (productGroup) {
      const request = async () => {
        try {
          const symbolList = await getSymbolList(productGroup)
          setSymbolList(symbolList)
          setSymbol(symbolList[0] || {})
          // 如果没有币种，直接结束 loading
          if (!symbolList || symbolList.length === 0) {
            setLoading(false)
          }
        } catch (e) {
          setLoading(false)
        }
      }
      request()
    }
  }, [productGroup])

  useEffect(() => {
    if (authStatus === Status.LOGIN && symbol?.InstrumentID) {
      const request = async () => {
        setLoading(true)
        const api = orderType === OrderType.LIMIT ? getHistoryLimitOrder : getHistoryTriggerOrder
        try {
          const result = await api({
            InstrumentID: symbol?.InstrumentID,
            ...TradeDirection.trasnferParams(tradeDirection),
            Limit: 20
          })
          setOrderList(result || [])
          // console.log('orderList', orderList)
        } catch (e) { } finally { setLoading(false) }
      }
      request()
    }
  }, [authStatus, symbol?.InstrumentID, orderType, tradeDirection, currentPage])

  const columns = useMemo(() => {
    const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
    const volumePrecision = formatValue(symbol, 'VolumePrecision', 0)
    const otherPrecision = formatValue(symbol, 'OtherPrecision', 0)
    const feePrecision = formatValue(symbol, 'FeePrecision', 0)
    const positionType = productGroup === ProductGroup.SPOT ? PositionType.SINGLE : PositionType.DOUBLE
    const volumeUnit = intl.get('sheet')
    // console.log('data', result)
    switch (orderType) {
      case OrderType.LIMIT: return [
        {
          key: 'trade_symbol',
          title: intl.get('trade_variety'),
          width: 120,
          render: (data) => {
            return (
              <Row>
                <span className="symbol-name">{symbol?.InstrumentID}</span>
                <span className="yellow">
                  &nbsp;·&nbsp;{data.Leverage}X
                </span>
              </Row>
            )
          }
        },
        {
          key: 'order_time',
          title: intl.get('order_time'),
          render: data => (
            <Col>
              <span>{formatUtcTimestampToLocalTime(data.InsertTime / 1000, 'YYYY-MM-DD')}</span>
              <span style={{ marginTop: -4 }}>{formatUtcTimestampToLocalTime(data.InsertTime / 1000, 'HH:mm:ss')}</span>
            </Col>
          )
        },
        {
          key: 'trade_direction',
          title: intl.get('trade_direction'),
          render: data => {
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
          render: data => formatDecimal(formatValue(data, 'Volume'), volumePrecision)
        },
        {
          key: 'order_price',
          title: intl.get('order_price'),
          render: data => data.OrderPriceType !== PriceType.LIMIT ? PriceType.transfer(data.OrderPriceType) : formatDecimal(formatValue(data, 'Price'), pricePrecision)
        },
        {
          key: 'trade_volume',
          title: `${intl.get('trade_volume')}(${volumeUnit})`,
          render: data => formatDecimal(formatValue(data, 'VolumeTraded'), volumePrecision)
        },
        {
          key: 'trade_average_price',
          title: intl.get('trade_avp_price'),
          render: data => formatDecimal(formatValue(data, 'TradePrice'), pricePrecision)
        },
        {
          key: 'income',
          title: intl.get('profit'),
          render: data => data.Direction === Direction.BUY ? formatDecimal(formatValue(data, 'CloseProfit'), otherPrecision) + `(${String(symbol?.InstrumentID).split('/')[0]})` : formatDecimal(formatValue(data, 'CloseProfit'), otherPrecision) + `(${String(symbol?.InstrumentID).split('/')[1]})`
        },
        {
          key: 'fee',
          title: intl.get('fee'),
          render: data => data.Direction === Direction.BUY ? formatDecimal(formatValue(data, 'Fee'), feePrecision) + `(${String(symbol?.InstrumentID).split('/')[0]})` : formatDecimal(formatValue(data, 'Fee'), feePrecision) + `(${String(symbol?.InstrumentID).split('/')[1]})`
        },
        // {
        //   key: 'status',
        //   title: intl.get('status'),
        //   render: (data) => LimitOrderStatus.transfer(data.OrderStatus)
        // },
        {
          key: 'operation',
          title: intl.get('operation'),
          render: data => (
            <Row
              style={{ justifyContent: 'flex-end' }}>
              {
                (data.OrderStatus === LimitOrderStatus.ALL_TRADE || data.OrderStatus === LimitOrderStatus.PARTIAL_TRADE) &&
                <Button
                  className="button"
                  type="tertiary"
                  size="small"
                  onClick={() => {
                    OrderDetailModal.open(positionType, symbol, data, VolumeUnitType.SHEET)
                  }}>
                  {intl.get('views')}
                </Button>
              }
            </Row>
          )
        }
      ]
      case OrderType.PLAN: return [
        {
          key: 'trade_symbol',
          title: intl.get('trade_variety'),
          width: 120,
          render: data => {
            return (
              <Row>
                <span className="symbol-name">{symbol?.InstrumentID}</span>
                <span className="yellow">
                  &nbsp;·&nbsp;{data.Leverage}X
                </span>
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
          render: data => formatDecimal(formatValue(data, 'Volume'), volumePrecision)
        },
        {
          key: 'trigger_price',
          title: intl.get('trigger_price'),
          render: data => {
            if (
              data.TriggerPriceType !== TriggerOrderTriggerPriceType.LAST_PRICE &&
              data.TPTriggerPrice !== TriggerOrderTriggerPriceType.MARK_PRICE &&
              data.TPTriggerPrice !== TriggerOrderTriggerPriceType.TARGET_INDEX_PRICE
            ) {
              return formatDecimal(formatValue(data, 'TriggerPrice'), pricePrecision)
            }
            return TriggerOrderTriggerPriceType.transfer(data.TriggerPriceType)
          }
        },
        {
          key: 'order_price',
          title: intl.get('order_price'),
          render: data => data.OrderPriceType !== PriceType.LIMIT ? PriceType.transfer(data.OrderPriceType) : formatDecimal(formatValue(data, 'Price'), pricePrecision)
        },
        {
          key: 'status',
          title: intl.get('status'),
          render: data => TriggerOrderStatus.transfer(data.TriggerStatus)
        }
      ]
      case OrderType.STOP_PROFIT_LOSS: return [
        {
          key: 'trade_symbol',
          title: intl.get('trade_variety'),
          width: 120,
          render: data => {
            return (
              <Row>
                <span className="symbol-name">{symbol?.InstrumentID}</span>
                <span className="yellow">
                  &nbsp;·&nbsp;{data.Leverage}X
                </span>
              </Row>
            )
          }
        },
        { key: 'order_time', title: intl.get('order_time'), render: (data) => formatUtcTimestampToLocalTime(data.InsertTime / 1000, 'YYYY-MM-DD HH:mm:ss') },
        { key: 'type', title: intl.get('type'), render: (data) => TriggerOrderType.transfer(data.TriggerOrderType) },
        {
          key: 'stop_profit_trigger_price',
          title: intl.get('stop_profit_trigger_price'),
          render: data => formatDecimal(formatValue(data, 'TPTriggerPrice'), pricePrecision)
        },
        {
          key: 'stop_loss_trigger_price',
          title: intl.get('stop_loss_trigger_price'),
          render: (data) => formatDecimal(formatValue(data, 'SLTriggerPrice'), pricePrecision)
        },
        {
          key: 'status',
          title: intl.get('status'),
          render: (data) => TriggerOrderStatus.transfer(data.TriggerStatus)
        }
      ]
    }
  }, [productGroup, symbol, orderType, orderList])

  return (
    <>
      <Row
        className="layout-content-filter-container">
        <span>
          {intl.get('trade_variety')}
        </span>
        <Select
          style={{ width: 150 }}
          className="layout-content-filter-select"
          valueStyle={{ fontSize: 14 }}
          popoverRowStyle={{ fontSize: 14, height: 36 }}
          dataSource={symbolList}
          trigger="hover"
          value={transferName(symbol)}
          renderPopoverRow={data => transferName(data)}
          onSelected={setSymbol}
        />
        <span>{intl.get('order_type')}</span>
        <Select
          style={{ width: 110 }}
          className="layout-content-filter-select"
          valueStyle={{ fontSize: 14 }}
          popoverRowStyle={{ fontSize: 14, height: 36 }}
          dataSource={OrderType.createOptions()}
          trigger="hover"
          value={OrderType.transferText(orderType)}
          onSelected={data => {
            setOrderType(data.key)
          }}
        />
        <span>{intl.get('trade_direction')}</span>
        <Select
          style={{ width: 106 }}
          className="layout-content-filter-select"
          valueStyle={{ fontSize: 14 }}
          popoverRowStyle={{ fontSize: 14, height: 36 }}
          value={TradeDirection.transferText(tradeDirection)}
          dataSource={productGroup === ProductGroup.SPOT ? TradeDirection.createSpotOptions() : TradeDirection.createContractOptions()}
          trigger="hover"
          onSelected={data => { setTradeDirection(data.key) }} />
        {/* <span>{intl.get('status')}</span>
        <Select
          style={{ width: 146 }}
          className="layout-content-filter-select"
          valueStyle={{ fontSize: 14 }}
          popoverRowStyle={{ fontSize: 14, height: 36 }}
          value={orderType === OrderType.LIMIT ? LimitOrderStatus.transfer(status) : TriggerOrderStatus.transfer(status)}
          dataSource={orderType === OrderType.LIMIT ? LimitOrderStatus.createOptions() : TriggerOrderStatus.createOptions()}
          onSelected={data => { setStatus(data.key) }}/> */}
      </Row>
      <Table
        className="layout-content-table"
        dataSource={orderList || []}
        loading={loading}
        columns={columns}
      // pagination={{ current: result.current_page, total: result.total_page, onChange: setCurrentPage }}
      />
    </>
  )
}

export default HistoryOrderContent
