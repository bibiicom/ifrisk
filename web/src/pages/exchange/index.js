import React, { useEffect, useState, useRef, useContext } from 'react'
import { useNavigate } from 'react-router-dom'

import axios from 'axios'

import usePolling from '../../component/hooks/usePolling'
import useScreenSize, { ScreenSize } from '../../component/hooks/useScreenSize'
import { ComponentContextProvider } from '../../component/context'

import Header from './header'
import SymbolList from './symbolList'
import Chart from './chart'
import OrderBookDealDetail from './orderBookDealDetail'
import OrderBoard from './orderBoard'
import DataQuery from './dataQuery'
import { LeverModal } from './modal'

import RootContext from '../rootContext'
import { ExchangeContextProvider } from './context'

import createWebsocket, { WebsocketStatus, WebsocketMessageType, WebsocketDataType } from './createWebsocket'

import { getPositionType, getTradeConfig, subscribeMarketDetail, unsubscribeMarketDetail } from '../../api/contract'
import { getPosition } from '../../api/position'
import { getAnnouncement } from '../../api/announcement'

import authHOC from '../common/hoc/authHOC'

import { getStorage, saveStorage, VOLUME_UNIT } from '../../utils/storage'

import Section from './components/section'

import { MarginMode, Direction, Type } from '../../constants/position'
import VolumeUnitType from '../../constants/volumeUnitType'
import { Status } from '../../constants/auth'
import { ProductGroup } from '../../constants/symbol'

import { UPDATE_DELAY, POLLING_INTERVAL } from './config'

import './index.less'
import TradingSetting from './tradingSetting'
import { AccountContextProvider } from './accountContext'

export default authHOC(true)(() => {
  const navigate = useNavigate()
  const rootRef = useRef()
  const tradeConfigRequestCancel = useRef()
  const positionTypeRequestCancel = useRef()

  const dataQueryRef = useRef()
  const orderBoardRef = useRef()

  const { authStatus, userInfo, config: { theme } } = useContext(RootContext)

  const screenSize = useScreenSize()

  const [orderConfirm, setOrderConfirm] = useState(true)
  const [ws, setWs] = useState()
  const [wsConnected, setWsConnected] = useState(false)
  const [currentSymbol, setCurrentSymbol] = useState({})
  const [currentSymbolMarket, setCurrentSymbolMarket] = useState({})
  const [symbols, setSymbols] = useState()
  const [markets, setMarkets] = useState()

  const [marginMode, setMarginMode] = useState(MarginMode.CROSS)
  const [positionType, setPositionType] = useState(Type.DOUBLE)

  const [levers, setLevers] = useState({ long: 10, short: 10, longMax: 125, shortMax: 125 })

  const [volumeUnitType, setVolumeUnitType] = useState(getStorage(VOLUME_UNIT) !== VolumeUnitType.SHEET ? VolumeUnitType.COIN : VolumeUnitType.SHEET)

  const [chartVisible, setChartVisible] = useState(screenSize !== ScreenSize.MIN)

  const [newsList, setNewsList] = useState([])
  const [index, setIndex] = useState(0) // 当前显示的索引
  const [isTransitioning, setIsTransitioning] = useState(true) // 是否启用过渡动画
  const listRef = useRef(null)
  const itemHeight = 20

  // 获取公告列表
  useEffect(() => {
    const fetchAnnouncements = async () => {
      try {
        const result = await getAnnouncement({
          page: 1,
          limit: 10,
          type: 1,
          subType: 100
        })
        // console.log('公告接口返回:', result)
        
        // 根据接口返回的数据结构,可能在 result.data.announcementList 或 result.announcementList
        const announcementList = result?.data?.announcementList || result?.announcementList
        
        if (announcementList && announcementList.length > 0) {
          // 保存完整的公告数据(包含ID),用于点击跳转
          const formattedList = announcementList.map((item) => {
            const date = new Date(item.createdAt)
            const dateStr = `${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')} ${String(date.getHours()).padStart(2, '0')}:${String(date.getMinutes()).padStart(2, '0')}:${String(date.getSeconds()).padStart(2, '0')}`
            return {
              id: item.Id,
              text: `${item.Title} ${dateStr}`
            }
          })
          // console.log('格式化后的公告列表:', formattedList)
          setNewsList(formattedList)
        } else {
          // console.log('没有公告数据')
        }
      } catch (error) {
        console.error('获取公告失败:', error)
      }
    }
    fetchAnnouncements()
  }, [])

  // 点击公告跳转到详情页
  const handleNewsClick = (newsItem) => {
    if (newsItem?.id) {
      navigate(`/announcement/${newsItem.id}`)
    }
  }

  const rendernews = () => {
    // 如果没有数据,不渲染
    if (newsList.length === 0) {
      return null
    }

    return (
      <section
        className="exchange-section-dark"
        style={{
          height: 25,
          marginBottom: 2,
          display: 'flex',
          alignItems: 'center',
          overflow: 'hidden',
          fontSize: 12,
          color: '#ccc'
        }}
      >
        <span style={{ paddingLeft: 10, marginRight: 8 }}>📢:</span>
        <div style={{ height: itemHeight, overflow: 'hidden' }}>
          <div
            ref={listRef}
            style={{
              display: 'flex',
              flexDirection: 'column',
              transform: `translateY(-${index * itemHeight}px)`,
              transition: isTransitioning ? 'transform 0.5s ease' : 'none'
            }}
          >
            {newsList.concat(newsList[0]).map((news, i) => (
              <span
                key={i}
                className="news-item"
                style={{
                  height: itemHeight,
                  lineHeight: `${itemHeight}px`,
                  cursor: 'pointer'
                }}
                onClick={() => handleNewsClick(news)}
              >
                {news?.text || news}
              </span>
            ))}
          </div>
        </div>
        {/* <span style={{ marginLeft: 'auto', minWidth: 24 }}>新闻</span> */}
        {/* <span style={{ margin: '0 16px', minWidth: 24 }}>公告</span> */}
        <style>{'.news-item:hover { color: #007bff } '}</style>
      </section>
    )
  }

  useEffect(() => {
    if (newsList.length === 0) return
    const timer = setInterval(() => {
      setIndex(prev => prev + 1)
      setIsTransitioning(true)
    }, 3000) // 每3秒滚动一次
    return () => clearInterval(timer)
  }, [newsList.length])

  useEffect(() => {
    if (newsList.length > 0 && index === newsList.length) {
      const handleTransitionEnd = () => {
        setIsTransitioning(false)
        setIndex(0)
      }
      const el = listRef.current
      if (el) {
        el.addEventListener('transitionend', handleTransitionEnd, { once: true })
      }
    }
  }, [index, newsList.length])

  // const { result: positionResult = [], request: positionRequest } = usePolling({
  //   api: getPosition,
  //   params: {
  //     AccountID: userInfo.MemberID
  //   },
  //   flag: authStatus === Status.LOGIN,
  //   interval: POLLING_INTERVAL
  // }, [userInfo, authStatus === Status.LOGIN])

  useEffect(() => {
    const ws = createWebsocket()
    ws.on(WebsocketMessageType.STATUS, 'rootStatus', (status) => {
      setWsConnected(status === WebsocketStatus.CONNECT)
    })
    setWs(ws)
    return () => {
      ws.disconnect()
    }
  }, [])

  useEffect(() => {
    if (wsConnected && currentSymbol?.InstrumentID) {
      // 切换币种时先清空旧的行情数据，避免显示上一个币种的价格
      setCurrentSymbolMarket({})
      ws.on(WebsocketMessageType.MARKET_DETAIL, 'currentSymbol', (data) => {
        if (data.type === WebsocketDataType.PUSH) {
          // console.log('data', data)
          // console.log('currentSymbol', currentSymbol)
          if (data.result?.I === currentSymbol?.InstrumentID) {
            setCurrentSymbolMarket(data.result)
          }
        }
      })
      subscribeMarketDetail(ws, currentSymbol)
    }
    return () => {
      if (wsConnected && currentSymbol?.InstrumentID) {
        unsubscribeMarketDetail(ws, currentSymbol)
      }
      if (ws) {
        ws.off(WebsocketMessageType.MARKET_DETAIL, 'currentSymbol')
      }
    }
  }, [wsConnected, currentSymbol?.InstrumentID])

  useEffect(() => {
    if (currentSymbol?.ProductGroup === ProductGroup.SPOT) {
      setVolumeUnitType(VolumeUnitType.COIN)
    }
  }, [currentSymbol?.ProductGroup])

  useEffect(() => {
    if (currentSymbol?.ProductGroup !== ProductGroup.SPOT) {
      saveStorage(VOLUME_UNIT, volumeUnitType)
    }
  }, [currentSymbol?.ProductGroup, volumeUnitType])

  useEffect(() => {
    if (currentSymbol?.InstrumentID && authStatus === Status.LOGIN) {
      const queryTradeConfig = async () => {
        try {
          const id = userInfo.MemberID
          const data = await getTradeConfig({
            MemberID: id,
            AccountID: id,
            TradeUnitID: id,
            InstrumentID: currentSymbol?.InstrumentID,
            ExchangeID: currentSymbol.ExchangeID
          }, { cancelToken: new axios.CancelToken(c => { tradeConfigRequestCancel.current = c }) })
          const defaultLever = currentSymbol.DefaultLeverage || 10
          if (data) {
            const marginMode = data.IsCrossMargin ? MarginMode.CROSS : MarginMode.ISOLATED
            setMarginMode(marginMode)
            setLevers({
              long: data.LongLeverage || defaultLever,
              short: data.ShortLeverage || defaultLever,
              longMax: data.LongMaxLeverage || defaultLever,
              shortMax: data.ShortMaxLeverage || defaultLever
            })
          }
        } catch (e) {}
      }
      queryTradeConfig()
    }
    return () => {
      if (tradeConfigRequestCancel.current) {
        tradeConfigRequestCancel.current({ code: -100 })
        tradeConfigRequestCancel.current = null
      }
    }
  }, [currentSymbol?.InstrumentID, positionType, authStatus, userInfo])

  useEffect(() => {
    if (currentSymbol?.ProductGroup === ProductGroup.SPOT) {
      setPositionType(Type.SINGLE)
    } else {
      if (authStatus === Status.LOGIN) {
        const queryPositionType = async () => {
          try {
            const id = userInfo.MemberID
            const result = await getPositionType({
              MemberID: id,
              AccountID: id,
              TradeUnitID: id
            }, { cancelToken: new axios.CancelToken(c => { positionTypeRequestCancel.current = c }) })
            if (result.defaultPositionType) {
              setPositionType(result.defaultPositionType)
            }
          } catch (e) {}
        }
        queryPositionType()
      }
    }
    return () => {
      if (positionTypeRequestCancel.current) {
        positionTypeRequestCancel.current({ code: -100 })
        positionTypeRequestCancel.current = null
      }
    }
  }, [authStatus, userInfo, currentSymbol?.InstrumentID])

  const onLeverClick = (direction) => {
    LeverModal.open({
      isMobile: screenSize === ScreenSize.MIN,
      userInfo,
      symbol: currentSymbol,
      levers,
      direction,
      onSuccess: l => {
        const ls = { ...levers }
        switch (direction) {
          case Direction.LONG: {
            ls.long = l
            break
          }
          case Direction.SHORT: {
            ls.short = l
            break
          }
          case Direction.NET: {
            ls.long = l
            ls.short = l
            break
          }
        }
        setLevers(ls)
        setTimeout(() => {
          dataQueryRef.current.updateData()
          orderBoardRef.current.updateData()
        }, UPDATE_DELAY)
      },
      config: { theme }
    })
  }

  const renderPC = () => {
    return (
      <div
        ref={rootRef}
        className={`exchange-${theme}`}>
        {/* <Helmet>
          <meta name="viewport" content="width=device-width,initial-scale=0.3,minimum-scale=0.3,maximum-scale=1,viewport-fit=cover"/>
        </Helmet> */}
        <Header onLeverClick={onLeverClick}/>
        <div
          className="exchange-middle-container">
          {/* {
            screenSize === ScreenSize.MAX && (
              <Section
                className="exchange-middle-symbol-container">
                <SymbolList style={{ height: '100%' }} />
              </Section>
            )
          } */}
          <div className="exchange-chart">
            {rendernews()}
            <Chart />
          </div>
          <OrderBookDealDetail
            onOrderBookPriceClick={price => { orderBoardRef.current.setOrderPrice(+price) }}
            depth={10} />
          <OrderBoard
            ref={orderBoardRef}
            onLeverClick={onLeverClick}
            onOrderSuccess={_ => {
              dataQueryRef.current.updateData()
              // positionRequest()
            }} />
        </div>
        <DataQuery
          ref={dataQueryRef}
          onLeverClick={onLeverClick}
          onLimitOrderChange={() => {
            orderBoardRef.current.updateData()
            // positionRequest()
          }} />
      </div>
    )
  }

  const renderMobile = () => {
    return (
      <div
        ref={rootRef}
        className={`exchange-${theme}`}>
        <Header
          onLeverClick={onLeverClick}
          onChartIconClick={() => { setChartVisible(!chartVisible) }}/>
        {rendernews()}
        {
          chartVisible && <Chart/>
        }
        {
          !chartVisible &&
          <Section className="exchange-middle-container">
            <div className="exchange-middle-order-board-container">
              <TradingSetting onLeverClick={onLeverClick} />
              <OrderBoard
                ref={orderBoardRef}
                onLeverClick={onLeverClick}
                onOrderSuccess={_ => {
                  dataQueryRef.current.updateData()
                  // positionRequest()
                }} />
            </div>
            <OrderBookDealDetail
              onOrderBookPriceClick={price => { orderBoardRef.current.setOrderPrice(+price) }}
              depth={6} />
          </Section>
        }
        <DataQuery
          ref={dataQueryRef}
          onLeverClick={onLeverClick}
          onLimitOrderChange={() => {
            orderBoardRef.current.updateData()
            // positionRequest()
          }}/>
      </div>
    )
  }

  return (
    <ComponentContextProvider theme={theme}>
      <ExchangeContextProvider
        value={{
          orderConfirm,
          ws,
          wsConnected,
          currentSymbol,
          symbols,
          markets,
          currentSymbolMarket,
          marginMode,
          positionType,
          levers,
          volumeUnitType,
          // positionList: positionResult,
          setOrderConfirm,
          setCurrentSymbol,
          setSymbols,
          setCurrentSymbolMarket,
          setMarkets,
          setVolumeUnitType,
          setMarginMode,
          setPositionType,
          setLevers
        }}>
        { <AccountContextProvider>
          {screenSize === ScreenSize.MIN ? renderMobile() : renderPC()}
        </AccountContextProvider>}
      </ExchangeContextProvider>
    </ComponentContextProvider>
  )
})
