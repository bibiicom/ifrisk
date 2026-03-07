import React, { useState, useEffect, useContext, useRef, memo } from 'react'
import { useLocation } from 'react-router-dom'
// import axios from 'axios'

import ExchangeContext from '../context'
import { WebsocketMessageType, WebsocketDataType } from '../createWebsocket'

import List from '../../../component/list'
import Button from '../../../component/button'
import Row from '../../../component/row'
import intl from '../../../component/intl'

import Section from '../components/section'

import ComponentContext from '../../../component/context'

import { getStorage, saveStorage, SYMBOL_KEY, FAVORITES_KEY } from '../../../utils/storage'
import { getSymbolList } from '../../../api/symbol'
import { subscribeMarketOverview, unsubscribeMarketOverview } from '../../../api/contract'

import { ProductGroup, transferName } from '../../../constants/symbol'

import { formatValue, formatDecimal, formatPercent } from '../../../utils/format'

import './index.less'

const TabScrollPosition = {
  LEFT: 'left',
  RIGHT: 'right'
}

const SymbolList = (({ style, onSymbolClick }) => {
  const tabContainer = useRef()
  const [tabScrollPosition, setTabScrollPosition] = useState(TabScrollPosition.LEFT)

  const location = useLocation()

  const classifySymbols = useRef({})
  const symbolRequestCancel = useRef()

  const { theme } = useContext(ComponentContext)
  const { ws, wsConnected, currentSymbol, markets, setCurrentSymbol, setSymbols, setMarkets } = useContext(ExchangeContext)
  // console.log('makets', markets)
  const [tab, setTab] = useState(null)
  const [loading, setLoading] = useState(true)
  const [symbolList, setSymbolList] = useState([])
  const marketsTemp = useRef({})

  const [listHeight, setListHeight] = useState('calc(100% - 40px)')
  const { search } = useLocation()
  const [favorites, setFavorites] = useState(() => {
    try {
      return JSON.parse(getStorage(FAVORITES_KEY)) || []
    } catch {
      return []
    }
  })

  useEffect(() => {
    let productGroup
    try {
      const symbol = JSON.parse(getStorage(SYMBOL_KEY))
      productGroup = symbol.ProductGroup
    } catch (e) {
    }
    if (!productGroup) {
      const state = location.state
      if (state) {
        productGroup = state.ProductGroup
      }
    }

    const all = ProductGroup.createTradeOptions()
    if (all.findIndex(item => item.key === productGroup) < 0) {
      productGroup = ProductGroup.SPOTC
    }
    setTab(productGroup)
  }, [])

  useEffect(() => {
    if (wsConnected && tab) {
      ws.on(WebsocketMessageType.MARKET_OVERVIEW, 'symbolList', (data) => {
        // console.log('data', data)
        if (data.type === WebsocketDataType.PUSH) {
          const marketMap = { ...marketsTemp.current }
          data.result.forEach(market => {
            marketMap[market.I] = market
          })
          marketsTemp.current = marketMap
          setMarkets(marketMap)
        }
      })
      subscribeMarketOverview(ws, tab)
    }
    return () => {
      if (wsConnected && tab) {
        unsubscribeMarketOverview(ws, tab)
      }
      if (ws) {
        ws.off(WebsocketMessageType.MARKET_OVERVIEW, 'symbolList')
      }
    }
  }, [wsConnected, tab])

  useEffect(() => {
    if (tab) {
      const symbols = (symbolList) => {
        const ss = {}
        symbolList.forEach(symbol => {
          ss[symbol.InstrumentID] = symbol
        })
        return ss
      }
      setLoading(false)
      if (classifySymbols.current[tab]) {
        const symbolList = classifySymbols.current[tab]
        setSymbolList(classifySymbols.current[tab])
        setSymbols(symbols(symbolList))
      } else {
        const request = async () => {
          setLoading(true)
          setSymbolList([])
          try {
            const symbolList = await getSymbolList(tab)
            classifySymbols.current[tab] = symbolList
            setSymbolList(symbolList)
            setSymbols(symbols(symbolList))
          } catch (e) {
          } finally {
            setLoading(false)
          }
        }
        request()
      }
    }
    return () => {
      if (symbolRequestCancel.current) {
        symbolRequestCancel.current({ code: -100 })
        symbolRequestCancel.current = null
      }
    }
  }, [tab])

  useEffect(() => {
    if (tabContainer.current) {
      const bounding = tabContainer.current.getBoundingClientRect()
      setListHeight(`calc(100% - 40px - ${bounding.height}px)`)
    }
  }, [tab])

  useEffect(() => {
    const query = new URLSearchParams(search)
    // console.log('symbolList', symbolList)
    const InstrumentID = query.get('InstrumentID')
    // console.log('InstrumentID', InstrumentID)
    const ProductGroup = query.get('ProductGroup')
    // console.log('ProductGroup', ProductGroup)
    if (InstrumentID) {
      saveStorage(SYMBOL_KEY, JSON.stringify({ InstrumentID: InstrumentID, ProductGroup: ProductGroup }))
    }

    if (ProductGroup) {
      setTab(ProductGroup)
    }
  }, [search])

  useEffect(() => {
    if (symbolList.length > 0) {
      // console.log('symbolList', symbolList)
      if (!currentSymbol?.InstrumentID) {
        const storedData = getStorage(SYMBOL_KEY)
        if (storedData) {
          const symbolId = JSON.parse(getStorage(SYMBOL_KEY)).InstrumentID
          const symbol = symbolList.find(s => s.InstrumentID === symbolId)
          if (symbol) {
            setCurrentSymbol(symbol)
            return
          }
        }
        const symbol = symbolList[0]
        saveStorage(SYMBOL_KEY, JSON.stringify({ InstrumentID: symbol.InstrumentID, ProductGroup: symbol.ProductGroup }))
        setCurrentSymbol(symbol)
      }
    }
  }, [symbolList])

  const tabs = ProductGroup.createTradeOptions()

  useEffect(() => {
    // console.log('currentSymbol', currentSymbol)
    if (currentSymbol.InstrumentID) {
      saveStorage(SYMBOL_KEY, JSON.stringify({ InstrumentID: currentSymbol.InstrumentID, ProductGroup: currentSymbol.ProductGroup }))
    }
  }, [currentSymbol.InstrumentID])

  return (
    <Section
      style={style}
      className={`exchange-symbol-list-${theme}`}
    >
      {
        tabs.length > 0 && (
          <Row
            ref={tabContainer}
            className="exchange-symbol-list-tabs">
            {
              tabs.map(({ key, text }) => (
                <Button
                  key={key}
                  className={`tab-button ${key === tab ? 'selected' : ''}`}
                  size="small"
                  type="tertiary"
                  onClick={() => { setTab(key) }}>
                  {text}
                </Button>
              ))
            }
          </Row>
        )
      }
      <List
        className="exchange-symbol-list-body"
        style={{ height: listHeight }}
        loading={loading}
        showTitle={true}
        columns={[
          {
            key: 'favorite',
            title: '',
            style: { width: '10%', textAlign: 'center' },
            render: (data) => {
              let favorites
              try {
                favorites = JSON.parse(getStorage(FAVORITES_KEY)) || []
              } catch {
                favorites = []
              }
              const isFav = favorites.includes(data.InstrumentID)
              return (
                <span
                  onClick={(e) => {
                    e.stopPropagation()
                    let updated
                    if (isFav) {
                      updated = favorites.filter(id => id !== data.InstrumentID)
                    } else {
                      updated = [...favorites, data.InstrumentID]
                    }
                    saveStorage(FAVORITES_KEY, JSON.stringify(updated))
                    setFavorites(updated)
                  }}
                  style={{ cursor: 'pointer', color: isFav ? '#FFD700' : '#aaa' }}
                >
                  {isFav ? '★' : '☆'}
                </span>
              )
            }
          },
          {
            key: 'name',
            title: intl.get('trade_variety'),
            style: { width: '35%', justifyContent: 'flex-start' },
            // render: data => transferName(data)
            render: (data) => data.InstrumentID
          },
          {
            key: 'price',
            title: intl.get('last_price'),
            style: { width: '25%' },
            render: (data) => {
              const pricePrecision = formatValue(data, 'PricePrecision', 0)
              const market = formatValue(markets, data.InstrumentID, {})
              const className = market.Change < 0 ? 'sell-fail-color' : (market.Change > 0 ? 'buy-rise-color' : '')
              return <span className={className}>{formatDecimal(formatValue(market, 'N'), pricePrecision)}</span>
            }
          },
          {
            key: 'change',
            title: intl.get('change'),
            style: { width: '30%' },
            render: (data) => {
              const market = formatValue(markets, data.InstrumentID, {})
              const className = market.Change < 0 ? 'sell-fail-color' : (market.Change > 0 ? 'buy-rise-color' : '')
              return <span className={className}>{formatPercent(formatValue(market, 'Change'))}</span>
            }
          }
        ]}
        dataSource={symbolList}
        onRowClick={symbol => { setCurrentSymbol(symbol); onSymbolClick && onSymbolClick(symbol) }}
      />
    </Section>
  )
})

export default memo(SymbolList)
