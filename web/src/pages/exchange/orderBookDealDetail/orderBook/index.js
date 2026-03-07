import React, { useEffect, useState, useContext, useRef, useMemo, memo } from 'react'

import ExchangeContext from '../../context'

import { WebsocketMessageType, WebsocketDataType } from '../../createWebsocket'

import { formatValue, formatDecimal, formatBigNumber, formatPercent } from '../../../../utils/format'

import { subscribeOrderBook, unsubscribeOrderBook } from '../../../../api/contract'

import { ProductGroup } from '../../../../constants/symbol'
import { Direction } from '../../../../constants/order'
import VolumeUnitType from '../../../../constants/volumeUnitType'

import intl from '../../../../component/intl'
import ComponentContext from '../../../../component/context'

import './index.less'

export const ORDER_BOOK_TYPE_SPLIT = 'split'
export const ORDER_BOOK_TYPE_BID = 'bid'
export const ORDER_BOOK_TYPE_ASK = 'ask'

const OrderBook = (({ orderBookType, orderBookTick, onPriceClick, depth }) => {
  const { wsConnected, ws, currentSymbol = {}, currentSymbolMarket = {}, volumeUnitType } = useContext(ExchangeContext)
  const { theme } = useContext(ComponentContext)

  const updateFrameId = useRef(-1)
  const dataReady = useRef(true)
  const bidMap = useRef({})
  const askMap = useRef({})
  const [bidList, setBidList] = useState([])
  const [askList, setAskList] = useState([])

  useEffect(() => {
    if (wsConnected && currentSymbol?.InstrumentID && orderBookTick) {
      ws.on(WebsocketMessageType.ORDER_BOOK, 'orderBook', (data) => {
        // console.log('orderbook', data)
        if (data.type === WebsocketDataType.UNSUB) {
          dataReady.current = true
        }
        if (dataReady.current && data.type === WebsocketDataType.PUSH) {
          const orderList = data.result
          // if (data.index === `${currentSymbol.ExchangeID}_${currentSymbol.InstrumentID}`) {
          if (data.index.startsWith(`${currentSymbol.ExchangeID}_${currentSymbol.InstrumentID}`)) {
            orderList.forEach(data => {
              if (data[0]) {
                if (data[0] === Direction.BUY) {
                  if (+data[2] === 0) {
                    delete bidMap.current[data[1]]
                  } else {
                    bidMap.current[data[1]] = data
                  }
                } else {
                  if (+data[2] === 0) {
                    delete askMap.current[data[1]]
                  } else {
                    askMap.current[data[1]] = data
                  }
                }
              }
            })
            // console.log('bidMap', bidMap)
            // console.log('askMap', askMap)
            // const bidList = Object.keys(bidMap.current).sort((p1, p2) => (+p2) - (+p1)).map(p => bidMap.current[p])
            // const askList = Object.keys(askMap.current).sort((p1, p2) => (+p1) - (+p2)).map(p => askMap.current[p])
            // setBidList(bidList)
            // setAskList(askList)
            // console.log('bidMap', bidMap)
            cancelAnimationFrame(updateFrameId.current)
            updateFrameId.current = requestAnimationFrame(() => {
              const bidList = Object.keys(bidMap.current).sort((p1, p2) => (+p2) - (+p1)).map(p => bidMap.current[p])
              const askList = Object.keys(askMap.current).sort((p1, p2) => (+p1) - (+p2)).map(p => askMap.current[p])
              // console.log('askList', askList)
              // console.log('bidList', bidList)
              setBidList(bidList)
              setAskList(askList)
            })
          }
        }
      })
      subscribeOrderBook(ws, currentSymbol, orderBookTick)
    }
    return () => {
      if (wsConnected) {
        if (currentSymbol?.InstrumentID && orderBookTick) {
          dataReady.current = false
          unsubscribeOrderBook(ws, currentSymbol, orderBookTick)
        }
      }
      if (ws) {
        ws.off(WebsocketMessageType.ORDER_BOOK, 'orderBook')
      }
      cancelAnimationFrame(updateFrameId.current)
      bidMap.current = {}
      askMap.current = {}
      setBidList([])
      setAskList([])
    }
  }, [wsConnected, currentSymbol?.InstrumentID, orderBookTick])

  useEffect(() => {
    return () => {
      if (!wsConnected) {
        dataReady.current = true
      }
    }
  }, [wsConnected])

  const renderBidList = (pricePrecision, volumePrecision, maxVolume, num = 10) => {
    let totalVolume = 0
    const domArray = []
    for (let i = 0; i < num; i++) {
      const data = formatValue(bidList, i, {})
      // const volume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume: formatValue(data, 'Volume'), price: data.Price, symbol: currentSymbol })
      const volume = formatValue(data, 2)
      totalVolume = volume === '--' ? '--' : totalVolume + (+volume)
      domArray.push(
        <li
          key={`bid${i}`}
          className="exchange-order-book-row">
          <span
            className="volume-rate-container">
            <i
              className="volume-rate buy-rise-background"
              style={{ width: volume === '--' ? 0 : `${volume / maxVolume * 100}%` }}/>
          </span>
          <span
            data-type="price"
            className="buy-rise-color"
            onClick={() => { data[1] && onPriceClick(data[1]) }}>
            {formatDecimal(formatValue(data, 1), pricePrecision)}
          </span>
          <span
            data-type="volume">
            {formatBigNumber(formatDecimal(volume, volumePrecision, 'floor', true))}
          </span>
          <span
            data-type="sum">
            {formatBigNumber(formatDecimal(totalVolume, volumePrecision, 'floor', true))}
          </span>
        </li>
      )
    }
    return (
      <ul className="exchange-order-book-list">
        {domArray}
      </ul>
    )
  }

  const renderAskList = (pricePrecision, volumePrecision, maxVolume, num = 10) => {
    let totalVolume = 0
    const domArray = []
    for (let i = 0; i < num; i++) {
      const data = formatValue(askList, i, {})
      // const volume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume: formatValue(data, 'Volume'), price: data.Price, symbol: currentSymbol })
      const volume = formatValue(data, 2)
      totalVolume = volume === '--' ? '--' : totalVolume + (+volume)
      domArray.unshift(
        <li
          key={`ask${i}`}
          className="exchange-order-book-row">
          <span
            className="volume-rate-container">
            <i
              className="volume-rate sell-fail-background"
              style={{ width: volume === '--' ? 0 : `${volume / maxVolume * 100}%` }}/>
          </span>
          <span
            data-type="price"
            className="sell-fail-color"
            onClick={() => { data[1] && onPriceClick(data[1]) }}>
            {formatDecimal(formatValue(data, 1), pricePrecision)}
          </span>
          <span data-type="volume">
            {formatBigNumber(formatDecimal(volume, volumePrecision, 'floor', true))}
          </span>
          <span data-type="sum">
            {formatBigNumber(formatDecimal(totalVolume, volumePrecision, 'floor', true))}
          </span>
        </li>
      )
    }
    return (
      <ul
        className="exchange-order-book-list exchange-order-book-ask-list"
        data-order-book-type={orderBookType}>
        {domArray}
      </ul>
    )
  }

  const isSheetVolume = volumeUnitType === VolumeUnitType.SHEET
  const pricePrecision = formatValue(currentSymbol, 'PricePrecision', 0)
  const volumePrecision = isSheetVolume ? formatValue(currentSymbol, 'VolumePrecision', 0) : formatValue(currentSymbol, 'CoinPrecision', 0)
  const volumeUnit = isSheetVolume ? intl.get('sheet') : formatValue(currentSymbol, 'BaseCurrency')
  const maxVolume = useMemo(() => {
    switch (orderBookType) {
      case ORDER_BOOK_TYPE_SPLIT: {
        const sliceBidList = bidList.slice(0, 10)
        const sliceAskList = askList.slice(0, 10)
        const maxBidData = (sliceBidList.sort((d1, d2) => d2[2] - d1[2]))[0]
        const maxAskData = (sliceAskList.sort((d1, d2) => d2[2] - d1[2]))[0]
        return Math.max(formatValue(maxBidData, 2, 0), formatValue(maxAskData, 2, 0)) || 1
      }
      case ORDER_BOOK_TYPE_BID: {
        const sliceBidList = bidList.slice(0, 20)
        const maxBidData = (sliceBidList.sort((d1, d2) => d2[2] - d1[2]))[0]
        return (formatValue(maxBidData, 2, 0)) || 1
      }
      case ORDER_BOOK_TYPE_ASK: {
        const sliceAskList = askList.slice(0, 10)
        const maxAskData = (sliceAskList.sort((d1, d2) => d2[2] - d1[2]))[0]
        return (formatValue(maxAskData, 2, 0)) || 1
      }
    }
    return 1
  }, [bidList, askList, orderBookType])
  const change = currentSymbolMarket.Change
  return (
    <div className={`exchange-order-book-${theme}`}>
      <div
        className="exchange-order-book-row exchange-order-book-title">
        <span data-type="price">{intl.get('price')}({formatValue(currentSymbol, 'PriceCurrency')})</span>
        <span data-type="volume">{intl.get('volume')}({volumeUnit})</span>
        <span data-type="sum">{intl.get('cumulative')}({volumeUnit})</span>
      </div>
      {orderBookType !== ORDER_BOOK_TYPE_BID && renderAskList(pricePrecision, volumePrecision, maxVolume, orderBookType === ORDER_BOOK_TYPE_SPLIT ? depth : 2 * depth)}
      <div
        className="exchange-order-book-price"
        data-order-book-type={orderBookType}>
        <div className="exchange-order-book-price-content">
          <span
            className={`last ${change > 0 ? 'buy-rise-color' : (change < 0 ? 'sell-fail-color' : '')}`}>
            {formatDecimal(formatValue(currentSymbolMarket, 'N'), pricePrecision)}
          </span>
          <span
            className={change > 0 ? 'buy-rise-color-opacity-70' : (change < 0 ? 'sell-fail-color-opacity-70' : '')}>
            {formatPercent(change)}
          </span>
        </div>
        {
          currentSymbol?.ProductGroup !== ProductGroup.SPOT &&
          <div className="exchange-order-book-price-content mark-price-content">
            <span className="mark-label">{intl.get('mark_price')}</span>
            <span className="mark">{formatDecimal(formatValue(currentSymbolMarket, 'M'), pricePrecision)}</span>
          </div>
        }
      </div>
      {orderBookType !== ORDER_BOOK_TYPE_ASK && renderBidList(pricePrecision, volumePrecision, maxVolume, orderBookType === ORDER_BOOK_TYPE_SPLIT ? depth : 2 * depth)}
    </div>
  )
})

export default memo(OrderBook)
