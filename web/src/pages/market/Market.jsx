import React, { useEffect, useState, useContext, useRef, useMemo } from 'react'
import axios from 'axios'
import { Tooltip } from 'antd'
import RootContext from '../rootContext'
import Col from '../../component/col'
import Row from '../../component/row'
import Tab from '../../component/tab'
import Table from '../../component/table'
import Button from '../../component/button'
import usePolling from '../../component/hooks/usePolling'
import useScreenSize, { ScreenSize } from '../../component/hooks/useScreenSize'
import ComponentContext from '../../component/context'
import intl from '../../component/intl'
import { getStorage, FAVORITES_KEY } from '../../utils/storage'
import authHOC from '../common/hoc/authHOC'
// import { ProductGroup, transferName } from '../../constants/newSymbol'
import { ProductGroup } from '../../constants/symbol'
import { ConvertedCurrency } from '../../constants/convertedCurrency'
import { getSymbolList, getMarkets } from '../../api/symbol'
import { formatDecimal, formatPercent, formatValue } from '../../utils/format'
import { getStorageConfig, LOCALE_KEY } from '../storageConfig'
import './market.less'

const sliderConfig = {
  dots: false,
  arrows: false,
  infinite: true,
  slidesToShow: 5,
  slidesToScroll: 1,
  autoplay: true,
  speed: 5000,
  autoplaySpeed: 5000,
  cssEase: 'linear'
}

export default authHOC(true)(() => {
  const symbols = useRef({})
  const symbolRequestCancel = useRef()
  const size = useScreenSize()
  const [config] = useState(getStorageConfig())
  const locale = config[LOCALE_KEY] || "en_US";

  const { config: { convertedCurrency }, rates } = useContext(RootContext)

  // const [indexSymbolList, setIndexSymbolList] = useState([])

  const [classify, setClassify] = useState(ProductGroup.CRYPTOCURRENCY)
  const [tab, setTab] = useState(ProductGroup.SPOT)
  const [loading, setLoading] = useState(true)
  const [symbolList, setSymbolList] = useState([])

  // const [favorites, setFavorites] = useState(() => {
  //   try {
  //     return JSON.parse(getStorage(FAVORITES_KEY)) || []
  //   } catch {
  //     return []
  //   }
  // })

  // 只保留加密大类，其它（股票、商品、债券等）不展示
  const classifyOptions = useMemo(() => {
    const all = ProductGroup.createClassify()
    return all.filter(item => item.key === ProductGroup.CRYPTOCURRENCY)
  }, [])

  const { result: markets = {} } = usePolling({
    api: getMarkets,
    params: tab,
    flag: true,
    interval: 2000
  }, [tab], [tab])

  // const { result: indexMarkets = {} } = usePolling({
  //   api: getMarkets,
  //   params: ProductGroup.INDEX,
  //   flag: true,
  //   interval: 2000
  // }, [])

  const requestSymbolList = async (tab) => {
    // console.log('tab', tab)
    setLoading(true)
    try {
      const symbolList = await getSymbolList(tab, { cancelToken: new axios.CancelToken(c => { symbolRequestCancel.current = c }) })
      setSymbolList(symbolList)
      symbols.current[tab] = symbolList
    } catch (e) { } finally { setLoading(false) }
  }

  // const requestIndexSymbolList = async () => {
  //   try {
  //     const symbolList = await getSymbolList(ProductGroup.INDEX)
  //     setIndexSymbolList(symbolList)
  //   } catch (e) {}
  // }

  useEffect(() => {
    setLoading(false)
    setSymbolList([])
    if (symbols.current[tab]) {
      setSymbolList(symbols.current[tab])
    } else {
      // console.log('tab', tab)
      requestSymbolList(tab)
    }
    return () => {
      if (symbolRequestCancel.current) {
        symbolRequestCancel.current({ code: -100 })
        symbolRequestCancel.current = null
      }
    }
  }, [tab])

  // const renderIndex = () => {
  //   return (
  //     <Slider className="market-index-container" { ...sliderConfig }>
  //       {
  //         indexSymbolList.map(symbol => {
  //           const coin = formatValue(symbol, 'BaseCurrency')
  //           const code = formatValue(symbol, 'InstrumentID')
  //           const market = formatValue(indexMarkets, code)
  //           const change = market.Change
  //           const convertedCurrencyUnit = ConvertedCurrency.transferUnit(convertedCurrency)
  //           const indexPrice = formatValue(market, 'LastPrice')
  //           const chg = formatValue(market, 'Chg')
  //           return (
  //             <Row
  //               key={code}
  //               className="market-index-item-container">
  //               <Col
  //                 className="market-index-item">
  //                 <Row
  //                   className="market-index-item-name-container">
  //                   {getIcon(coin)}
  //                   <span>{coin}{intl.get('index')}</span>
  //                 </Row>
  //                 <div
  //                   className="market-index-item-last-price-container">
  //                   <span
  //                     className={`market-index-item-last-price ${change > 0 ? 'buy-up-text' : (change < 0 ? 'sell-down-text' : '')}`}>
  //                     {formatDecimal(indexPrice)}
  //                   </span>
  //                   <span>
  //                     &nbsp;&nbsp;≈&nbsp;
  //                     {convertedCurrencyUnit}
  //                     &nbsp;
  //                     {formatDecimal(ConvertedCurrency.transferPrice('usdt', convertedCurrency, indexPrice, rates))}</span>
  //                 </div>
  //                 <Row
  //                   className={change > 0 ? 'buy-up-text' : (change < 0 ? 'sell-down-text' : '')}>
  //                   <span
  //                     className={`market-index-item-change ${change > 0 ? 'opacity-buy-up-background' : (change < 0 ? 'opacity-sell-down-background' : '')}`}>
  //                     {formatPercent(change, 2, '+')}
  //                   </span>
  //                   <span
  //                     style={{ paddingLeft: 14 }}>
  //                     {formatDecimal(chg, 2)}
  //                   </span>
  //                 </Row>
  //               </Col>
  //             </Row>
  //           )
  //         })
  //       }
  //     </Slider>
  //   )
  // }

  const getStatusText = (code) => {
    switch (code) {
      case '1':
        return { text: intl.get('trading_status_suspended'), className: 'status-suspended' }
      case '2':
        return { text: intl.get('trading_status_trading'), className: 'status-trading' }
      case '7':
        return { text: intl.get('trading_status_settlement'), className: 'status-settlement' }
      default:
        return { text: '-', className: 'status-default' }
    }
  }

  const columns = useMemo(() => {
    const columns = [
      // {
      //   key: 'favorite',
      //   title: '',
      //   style: { textAlign: 'center', width: 40 },
      //   render: (data) => {
      //     let favorites
      //     try {
      //       favorites = JSON.parse(getStorage(FAVORITES_KEY)) || []
      //     } catch {
      //       favorites = []
      //     }
      //     const isFav = favorites.includes(data.InstrumentID)
      //     return (
      //       <span
      //         onClick={(e) => {
      //           e.stopPropagation()
      //           let updated
      //           if (isFav) {
      //             updated = favorites.filter(id => id !== data.InstrumentID)
      //           } else {
      //             updated = [...favorites, data.InstrumentID]
      //           }
      //           saveStorage(FAVORITES_KEY, JSON.stringify(updated))
      //           setFavorites(updated)
      //         }}
      //         style={{ cursor: 'pointer', color: isFav ? '#FFD700' : '#aaa' }}
      //       >
      //         {isFav ? '★' : '☆'}
      //       </span>
      //     )
      //   }

      // },
      {
        key: 'trade_variety',
        title: intl.get('trade_variety'),
        style: { width: 180, textAlign: 'left' },
        render: data => (
          <Tooltip title={`${data?.N?.replace(/_/g, ' ')}`}>
            <div
              style={{ cursor: 'pointer' }}
              onClick={() => {
                console.log('clicked', data)
                window.location.href = `/${locale}/exchange?ProductGroup=${tab}&InstrumentID=${data.InstrumentID}`
              }}
            >
              {data.InstrumentID}
            </div>
          </Tooltip>
        )
      },
      {
        key: 'B',
        title: intl.get('status'),
        style: { width: 120,textAlign: 'left' },
        render: data => {
          const market = formatValue(markets, formatValue(data, 'InstrumentID'), {})
          const status = formatValue(market, 'B')
          const statusInfo = getStatusText(status)
          return (
            <div style={{fontWeight:500,fontSize:12,width:120}}>
              {statusInfo.text}
            </div>
          )
        }
      },
      {
        key: 'last_price',
        title: intl.get('last_price'),
        style: { width: 100, textAlign: 'left' },
        render: data => {
          // console.log('data', data)
          const priceCurrency = formatValue(data, 'PriceCurrency').toLowerCase()
          const pricePrecision = formatValue(data, 'PricePrecision', 0)
          const market = formatValue(markets, formatValue(data, 'InstrumentID'), {})
          const lastPrice = formatValue(market, 'N')
          const change = market.Change
          return (
            <Col
              className={change > 0 ? 'buy-rise-color' : (change < 0 ? 'sell-fail-color' : '')}>
              <span>{formatDecimal(lastPrice, pricePrecision)}</span>
              <span
                className="market-table-body-row-cell-sub-text">
                ≈&nbsp;
                {/* {ConvertedCurrency.transferUnit(convertedCurrency)} */}
                $
                &nbsp;
                {formatDecimal(ConvertedCurrency.transferPrice(priceCurrency, convertedCurrency, lastPrice, rates))}
              </span>
            </Col>
          )
        }
      },
      {
        key: 'today_change',
        title: intl.get('today_change'),
        style: { width: 150 },
        render: data => {
          const market = formatValue(markets, formatValue(data, 'InstrumentID'), {})
          const change = market.Change
          return (
            <span
              className={change > 0 ? 'buy-rise-color' : (change < 0 ? 'sell-fail-color' : '')}>
              {formatPercent(change)}
            </span>
          )
        }
      }
    ]
    if (size === ScreenSize.MIDDLE || size === ScreenSize.MAX) {
      columns.push({
        key: 'today_high_price',
        title: intl.get('today_high_price'),
        style: { width: 200 },
        render: data => {
          const market = formatValue(markets, formatValue(data, 'InstrumentID'), {})
          const pricePrecision = formatValue(data, 'PricePrecision', 0)
          return formatDecimal(formatValue(market, 'H'), pricePrecision)
        }
      })
      columns.push({
        key: 'today_low_price',
        title: intl.get('today_low_price'),
        style: { width: 180 },
        render: data => {
          const market = formatValue(markets, formatValue(data, 'InstrumentID'), {})
          const pricePrecision = formatValue(data, 'PricePrecision', 0)
          return formatDecimal(formatValue(market, 'L'), pricePrecision)
        }
      })
    }
    if (size === ScreenSize.MAX) {
      // console.log('markets', markets)
      // if (tab !== ProductGroup.SPOT) {
      //   columns.push({
      //     style: { width: 100 },
      //     key: 'position_volume',
      //     title: intl.get('position_volume'),
      //     render: data => {
      //       const market = formatValue(markets, formatValue(data, 'InstrumentID'), {})
      //       const volumePrecision = formatValue(data, 'VolumePrecision', 0)
      //       return formatDecimal(formatValue(market, 'P'), volumePrecision)
      //     }
      //   })
      // }
      columns.push({
        key: 'today_trading_volume',
        title: intl.get('today_trading_volume'),
        style: { width: 180 },
        render: data => {
          const market = formatValue(markets, formatValue(data, 'InstrumentID'), {})
          const volumePrecision = formatValue(data, 'VolumePrecision', 0)
          return formatDecimal(formatValue(market, 'V'), volumePrecision)
        }
      })
      columns.push({
        key: 'today_trading_turnover',
        title: intl.get('today_trading_turnover'),
        style: { width: 150 },
        render: data => {
          const market = formatValue(markets, formatValue(data, 'InstrumentID'), {})
          const pricePrecision = formatValue(data, 'PricePrecision', 0)
          return formatDecimal(formatValue(market, 'T'), pricePrecision)
        }
      })
    }
    // console.log('columns', columns)
    return columns
  }, [size, convertedCurrency, rates, tab, markets, symbolList])

  const tabs = useMemo(() => {
    const item = classifyOptions.find(c => c.key === classify)
    // console.log('item', item)
    return item && item.children ? item.children : []
  }, [classify, classifyOptions])

  return (
    <Col
      className="market">
      {/* {renderIndex()} */}
      <Col
        className="market-list-container">
        <Tab
          className="market-tab-container"
          tab={classify}
          tabPosition="bottom"
          tabClassName="market-tab"
          dataSource={classifyOptions}
          onChange={key => {
            const item = classifyOptions.find(c => c.key === key)
            if (item.children) {
              setTab(item.children[0].key)
            } else {
              setTab(key)
            }
            setClassify(key)
          }} />
        {
          tabs.length > 0 && (
            <Row className="market-button-container">
              {
                tabs.map(({ key, text }) => (
                  <Button
                    key={key}
                    className={key === tab ? 'button-selected' : ''}
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
        <Table
          className="market-table"
          columns={columns}
          dataSource={symbolList}
          loading={loading}
        />
      </Col>
    </Col>
  )
})
