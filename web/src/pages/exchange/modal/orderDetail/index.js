import React, { useState, useEffect } from 'react'

import Row from '../../../../component/row'
import Modal from '../../../../component/modal'
import Table from '../../../../component/table'
import intl from '../../../../component/intl'

import { Direction, TradeDirection } from '../../../../constants/order'
import VolumeUnitType from '../../../../constants/volumeUnitType'

import { formatUtcTimestampToLocalTime, formatDecimal, formatValue } from '../../../../utils/format'

import { getTradeOrder } from '../../../../api/order'

const Content = ({ volumeUnitType, symbol, order }) => {
  const [currentPage, setCurrentPage] = useState(1)
  const [result = [], setResult] = useState([])
  const [loading, setLoading] = useState(true)

  useEffect(() => {
    const request = async _ => {
      setLoading(true)
      try {
        const result = await getTradeOrder({
          OrderID: order.OrderID
        })
        setResult(result)
      } catch (e) {
      } finally {
        setLoading(false)
      }
    }
    request()
  }, [currentPage])

  const isSheetVolume = volumeUnitType === VolumeUnitType.SHEET
  const volumeUnit = isSheetVolume ? intl.get('sheet') : intl.get('coin')
  const volumePrecision = isSheetVolume ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0)
  const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
  const otherPrecision = formatValue(symbol, 'OtherPrecision', 0)
  const feePrecision = formatValue(symbol, 'FeePrecision', 0)
  return (
    <Table
      style={{ padding: 0, minHeight: 260, marginTop: -24 }}
      columns={[
        {
          key: 'trade_symbol',
          title: intl.get('trade_variety'),
          width: 120,
          render: (data) => {
            return (
              <Row
                style={{ justifyContent: 'flex-start' }}>
                <span className="symbol-name">{symbol.BaseCurrency}-{symbol.PriceCurrency}</span>
                <span className="yellow">
                  &nbsp;·&nbsp;{data.Leverage}X
                </span>
              </Row>
            )
          }
        },
        {
          key: 'trade_time',
          title: intl.get('trade_time'),
          render: (data) => formatUtcTimestampToLocalTime(data.InsertTime, 'YYYY-MM-DD HH:mm:ss')
        },
        {
          key: 'trade_volume',
          title: `${intl.get('trade_volume')}(${volumeUnit})`,
          render: (data) => formatDecimal(VolumeUnitType.transferVolumeForUnit(volumeUnitType, { symbol, price: data.Price, volume: data.Volume }), volumePrecision)
        },
        {
          key: 'trade_price',
          title: intl.get('trade_price'),
          render: (data) => formatDecimal(data.Price, pricePrecision)
        },
        {
          key: 'income',
          title: intl.get('profit'),
          render: (data) => formatDecimal(data.CloseProfit, 8)
        },
        {
          key: 'fee',
          title: intl.get('fee'),
          render: (data) => formatDecimal(data.Fee, feePrecision)
        }
      ]}
      loading={loading}
      dataSource={result || []}
      // pagination={{ current: result.current_page, total: result.total_page, onChange: setCurrentPage }}
    />
  )
}

function open (positionType, symbol, order, volumeUnitType, config) {
  Modal.open(
    {
      title: (<>
        {symbol?.InstrumentID}
        <span
          className={`contract-exchange-confirm-modal-tag ${
            order.Direction === Direction.BUY ? 'buy-rise-border-background-opacity-10' : 'sell-fail-border-background-opacity-10'
          } ${
            order.Direction === Direction.BUY ? 'buy-rise-color' : 'sell-fail-color'
          }`}>
          {TradeDirection.transferDirectionOffsetFlagText(order, positionType)}
        </span>
      </>),
      content: <Content order={order} symbol={symbol} volumeUnitType={volumeUnitType}/>,
      width: 800,
      config
    }
  )
}

const OrderDetailModal = { open }

export default OrderDetailModal
