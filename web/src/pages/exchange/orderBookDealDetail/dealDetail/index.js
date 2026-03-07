import React, { useContext, useState, useEffect, useRef, memo } from 'react'

import ExchangeContext from '../../context'

import { WebsocketMessageType, WebsocketDataType } from '../../createWebsocket'

import { formatValue, formatDecimal, formatUtcTimestampToLocalTime } from '../../../../utils/format'

import { subscribeRealDeal, unsubscribeRealDeal } from '../../../../api/contract'

import { Direction } from '../../../../constants/order'
import VolumeUnitType from '../../../../constants/volumeUnitType'

import List from '../../../../component/list'
import intl from '../../../../component/intl'

import './index.less'

const DealDetail = (() => {
  const { wsConnected, ws, currentSymbol = {}, volumeUnitType } = useContext(ExchangeContext)

  const [loading, setLoading] = useState(true)
  const [realDealList, setRealDealList] = useState([])

  const tempRealDealList = useRef([])

  useEffect(() => {
    if (wsConnected && currentSymbol?.InstrumentID) {
      ws.on(WebsocketMessageType.REAL_DEAL, 'realDeal', (data) => {
        // console.log('data', data)
        // console.log('currentSymbol', currentSymbol)
        if (data.type === WebsocketDataType.SUB) {
          setLoading(false)
        }
        if (data.type === WebsocketDataType.PUSH) {
          if (data.index === `${currentSymbol.ExchangeID}_${currentSymbol.InstrumentID}`) {
            const dataList = tempRealDealList.current.concat(data.result)
            dataList.sort((d1, d2) => d2[3] - d1[3])
            tempRealDealList.current = dataList.slice(0, 100)
            setRealDealList(tempRealDealList.current)
          }
        }
      })
      subscribeRealDeal(ws, currentSymbol)
    }
    return () => {
      if (wsConnected) {
        if (currentSymbol?.InstrumentID) {
          unsubscribeRealDeal(ws, currentSymbol)
        }
      }
      if (ws) {
        ws.off(WebsocketMessageType.REAL_DEAL, 'realDeal')
      }
      setLoading(true)
      tempRealDealList.current = []
      setRealDealList([])
    }
  }, [wsConnected, currentSymbol?.InstrumentID])

  const isSheetVolume = volumeUnitType === VolumeUnitType.SHEET
  const pricePrecision = formatValue(currentSymbol, 'PricePrecision', 0)
  const volumePrecision = isSheetVolume ? formatValue(currentSymbol, 'VolumePrecision', 0) : formatValue(currentSymbol, 'CoinPrecision', 0)
  const volumeUnit = isSheetVolume ? intl.get('sheet') : formatValue(currentSymbol, 'BaseCurrency')
  return (
    <List
      className="contract-exchange-real-deal"
      showTitle={true}
      loading={loading}
      columns={[
        { key: 'time', title: intl.get('time'), style: { width: '30%' }, render: (data) => formatUtcTimestampToLocalTime(data[3], 'HH:mm:ss') },
        { key: 'price', title: `${intl.get('price')}(${formatValue(currentSymbol, 'PriceCurrency')})`, style: { width: '30%' }, render: (data) => <span className={data[0] === Direction.BUY ? 'buy-rise-color' : 'sell-fail-color'}>{formatDecimal(formatValue(data, 1), pricePrecision)}</span> },
        {
          key: 'volume',
          title: `${intl.get('volume')}(${volumeUnit})`,
          style: { width: '40%' },
          render: (data) => {
            const volume = formatValue(data, 2)
            return formatDecimal(
              VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume, price: data[1], symbol: currentSymbol }),
              volumePrecision
            )
          }
        }
      ]}
      dataSource={realDealList}/>
  )
})

export default memo(DealDetail)
