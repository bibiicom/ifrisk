import React, { useState, useEffect, useContext, memo } from 'react'

import Select from '../../../component/select'
import intl from '../../../component/intl'
import ComponentContext from '../../../component/context'
import useScreenSize, { ScreenSize } from '../../../component/hooks/useScreenSize'

import Section from '../components/section'

import OrderBook, { ORDER_BOOK_TYPE_SPLIT, ORDER_BOOK_TYPE_BID, ORDER_BOOK_TYPE_ASK } from './orderBook'
import DealDetail from './dealDetail'

import ExchangeContext from '../context'

import { formatValue } from '../../../utils/format'

import './index.less'

const TAB_ORDER_BOOK = 'order_book'
const TAB_REAL_DEAL = 'real_deal'

const OrderBookDealDetail = (({ onOrderBookPriceClick, depth }) => {
  const { theme } = useContext(ComponentContext)

  const screenSize = useScreenSize()

  const [tab, setTab] = useState(TAB_ORDER_BOOK)
  const [orderBookType, setOrderBookType] = useState(ORDER_BOOK_TYPE_SPLIT)

  const { currentSymbol = {} } = useContext(ExchangeContext)

  const [currentOrderBookTick, setCurrentOrderBookTick] = useState('')
  const [orderBookTicks, setOrderBookTicks] = useState([])

  useEffect(() => {
    if (currentSymbol?.InstrumentID) {
      const ticks = []
      const priceTick = formatValue(currentSymbol, 'PriceTick', 1)

      for (let i = 0; i < 4; i++) {
        ticks.push(priceTick * Math.pow(10, i))
      }

      // const pricePrecision = formatValue(currentSymbol, 'PricePrecision', 0)
      // for (let i = pricePrecision; i > -1; i--) {
      //   ticks.push(1 / Math.pow(10, i))
      // }
      setCurrentOrderBookTick(ticks[0])
      setOrderBookTicks(ticks)
    }
  }, [currentSymbol?.InstrumentID])

  return (
    <Section
      className={`exchange-order-book-deal-detail-${theme}`}
      tab={screenSize === ScreenSize.MIN ? '' : tab}
      tabs={screenSize === ScreenSize.MIN ? null : [
        { key: TAB_ORDER_BOOK, text: intl.get('order_book') },
        { key: TAB_REAL_DEAL, text: intl.get('real_deal') }
      ]}
      onTabChange={setTab}
      title={
        tab === TAB_ORDER_BOOK &&
        <div
          className="exchange-order-book-deal-detail-title">
          <div
            className={`module ${orderBookType === ORDER_BOOK_TYPE_SPLIT ? 'selected' : ''}`}
            onClick={() => { setOrderBookType(ORDER_BOOK_TYPE_SPLIT) }}>
            <i
              className="sell-fail-background"/>
            <i
              className="buy-rise-background"/>
          </div>
          <i
            className={`module sell-fail-background ${orderBookType === ORDER_BOOK_TYPE_ASK ? 'selected' : ''}`}
            onClick={() => { setOrderBookType(ORDER_BOOK_TYPE_ASK) }}/>
          <i
            className={`module buy-rise-background ${orderBookType === ORDER_BOOK_TYPE_BID ? 'selected' : ''}`}
            onClick={() => { setOrderBookType(ORDER_BOOK_TYPE_BID) }}/>
          <Select
            className="precision-select"
            selectorClassName="precision-select-selector"
            valueStyle={{ marginRight: 6 }}
            arrowClassName="precision-select-selector-arrow"
            popoverClassName="precision-select-popover"
            popoverRowClassName="precision-select-popover-row"
            trigger="hover"
            value={currentOrderBookTick || '--'}
            dataSource={orderBookTicks}
            onSelected={setCurrentOrderBookTick}/>
        </div>
      }>
      {
        tab === TAB_ORDER_BOOK
          ? <OrderBook
            orderBookType={orderBookType}
            orderBookTick={currentOrderBookTick}
            onPriceClick={onOrderBookPriceClick}
            depth={depth} />
          : <DealDetail/>
      }
    </Section>
  )
})

export default memo(OrderBookDealDetail)
