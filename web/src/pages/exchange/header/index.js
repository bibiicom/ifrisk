import React, { useState, useContext, useRef, useMemo, useEffect, memo } from 'react'
import moment from 'moment-timezone'
import { Link } from 'react-router-dom'
import ExchangeContext from '../context'
import Popover from '../../../component/popover'
import Row from '../../../component/row'
import { isValid } from '../../../component/utils/typeChecks'
import intl from '../../../component/intl'
import ComponentContext from '../../../component/context'
import useScreenSize, { ScreenSize } from '../../../component/hooks/useScreenSize'
import Section from '../components/section'
import SymbolList from '../symbolList'
import SymbolCard from '../components/SymbolCard'
import { formatValue, formatDecimal, formatDecimalNoZero, formatPercent } from '../../../utils/format'
import VolumeUnitType from '../../../constants/volumeUnitType'
import { ProductGroup } from '../../../constants/symbol'
import RootContext from '../../rootContext'
import { Status } from '../../../constants/auth'
import { Button as AntButton, Space } from 'antd'
import { BUYBACK } from '../../routes'
import { getCardByInstrument } from '@/api/exchange'
import defaultCardImg from '@/assets/images/common/default-card.jpeg'
import './index.less'

// 交易状态颜色配置
const TRADING_STATUS_COLORS = {
  '1': { // 停牌
    color: '#f44336',
    backgroundColor: 'rgba(244, 67, 54, 0.1)',
    borderColor: 'rgba(244, 67, 54, 0.3)'
  },
  '2': { // 正在交易
    color: '#4caf50',
    backgroundColor: 'rgba(76, 175, 80, 0.1)',
    borderColor: 'rgba(76, 175, 80, 0.3)'
  },
  '7': { // 交割中
    color: '#2196f3',
    backgroundColor: 'rgba(33, 150, 243, 0.1)',
    borderColor: 'rgba(33, 150, 243, 0.3)'
  },
  default: { // 默认
    color: '#9e9e9e',
    backgroundColor: 'rgba(158, 158, 158, 0.1)',
    borderColor: 'rgba(158, 158, 158, 0.3)'
  }
}

const Info = (props) => {
  return (
    <div
      style={props.style}
      className="exchange-header-info-item">
      <p className={`value ${props.value.className}`}>{props.value.v}</p>
      <p className="label">{props.label}</p>
    </div>
  )
}

const Header = (({ onLeverClick, onChartIconClick }) => {
  const symbolListPopoverRef = useRef()
  const symbolIconRef = useRef()
  const { userInfo, authStatus } = useContext(RootContext)

  const {
    currentSymbol = {}, currentSymbolMarket = {}, volumeUnitType
  } = useContext(ExchangeContext)


  // 获取交易状态
  const tradingStatus = useMemo(() => {
    const status = currentSymbolMarket?.B
    if (status === '1') return intl.get('trading_status_suspended')
    if (status === '2') return intl.get('trading_status_trading')
    if (status === '7') return intl.get('trading_status_settlement')
    return null
  }, [currentSymbolMarket?.B])

  const { theme } = useContext(ComponentContext)

  const screenSize = useScreenSize()

  const [spread, setSpread] = useState(screenSize !== ScreenSize.MIN)


  const [symbolListPopoverVisible, setSymbolListPopoverVisible] = useState(false)
  const [symbolCardVisible, setSymbolCardVisible] = useState(false)
  const [cardData, setCardData] = useState(null)

  const [countdown, setCountdown] = useState()

  useEffect(() => {
    // console.log('currentSymbol', currentSymbol)
    let timer
    if (currentSymbol?.InstrumentID && currentSymbol?.ProductGroup !== ProductGroup.SPOT) {
      const calc = () => {
        const isDelivery = currentSymbol?.ProductGroup === ProductGroup.POSITIVE_DELIVERY || currentSymbol?.ProductGroup === ProductGroup.REVERSE_DELIVERY
        const expireTime = isDelivery ? currentSymbol.ExpireTime : 8 * 3600
        const timestamp = moment.tz(new Date(), 'Asia/Shanghai').unix()
        const now = isDelivery ? timestamp : timestamp % (8 * 3600)
        const diff = expireTime - now
        let countdown
        if (diff < 0) {
          countdown = '00:00:00'
        } else {
          const day = Math.floor(diff / 3600 / 24)
          const hour = Math.floor((diff % (3600 * 24)) / 3600)
          const minute = Math.floor((diff % 3600) / 60)
          const second = Math.floor(diff % 60)
          const dayText = day === 0 ? '' : `${day}'-'}`
          // countdown = `${dayText}${hour}${intl.get('hour')}${minute}${intl.get('minute')}${second}${intl.get('second')}`
          countdown = `${dayText}${hour}${':'}${minute}${':'}${second}`
        }
        setCountdown(countdown)
        timer = setTimeout(() => {
          calc()
        }, 1000)
      }
      calc()
    }
    return () => {
      clearTimeout(timer)
    }
  }, [currentSymbol?.InstrumentID])

  useEffect(() => {
    setSpread(screenSize !== ScreenSize.MIN)
  }, [screenSize])

  // 获取卡片详细信息
  useEffect(() => {
    const fetchCardData = async () => {
      if (currentSymbol?.InstrumentID) {
        try {
          const res = await getCardByInstrument({
            instrumentID: currentSymbol.InstrumentID
          })
          if (res.code === 0 && res.data) {
            setCardData(res.data.card)
          } else {
            setCardData(null)
          }
        } catch (error) {
          // console.error('获取卡片信息失败:', error)
          setCardData(null)
        }
      } else {
        setCardData(null)
      }
    }
    
    fetchCardData()
  }, [currentSymbol?.InstrumentID])

  const isVolumeSheet = volumeUnitType === VolumeUnitType.SHEET

  const change = formatValue(currentSymbolMarket, 'Change')
  const pricePrecision = formatValue(currentSymbol, 'PricePrecision', 0)
  const volumePrecision = isVolumeSheet ? formatValue(currentSymbol, 'VolumePrecision', 0) : formatValue(currentSymbol, 'CoinPrecision', 0)

  const priceUnit = formatValue(currentSymbol, 'PriceCurrency')
  const volumeUnit = isVolumeSheet ? intl.get('sheet') : formatValue(currentSymbol, 'BaseCurrency')

  const turnover = useMemo(() => {
    if (currentSymbol.IsInverse) {
      if (isValid(currentSymbolMarket.V)) {
        return currentSymbolMarket.V * currentSymbol.VolumeMultiple
      }
    } else {
      return formatValue(currentSymbolMarket, 'T')
    }
    return '--'
  }, [currentSymbol.IsInverse, currentSymbolMarket.T, currentSymbolMarket.V])

  const buybackTo = useMemo(() => {
    if (currentSymbol?.InstrumentID) {
      return `${BUYBACK}?instrumentID=${encodeURIComponent(currentSymbol.InstrumentID)}`
    }
    return BUYBACK
  }, [currentSymbol?.InstrumentID])

  // 获取交易状态样式
  const tradingStatusStyle = useMemo(() => {
    const status = currentSymbolMarket?.B
    return TRADING_STATUS_COLORS[status] || TRADING_STATUS_COLORS.default
  }, [currentSymbolMarket?.B])

  const renderCountdown = () => {
    if (currentSymbol?.InstrumentID && currentSymbol?.ProductGroup !== ProductGroup.SPOT) {
      if (currentSymbol?.ProductGroup === ProductGroup.REVERSE_DELIVERY || currentSymbol?.ProductGroup === ProductGroup.POSITIVE_DELIVERY) {
        return (
          <Info
            value={{
              v: currentSymbol?.InstrumentID ? countdown : '--'
            }}
            label={intl.get('before_delivery_countdown')}
          />
        )
      }
      return (
        <>
          <Info
            value={{
              v: <span className="yellow">{formatPercent(formatValue(currentSymbolMarket, 'R'), 4)}</span>
            }}
            label={intl.get('capital_fee_rate')}
          />
          <Info
            value={{
              v: currentSymbol?.InstrumentID ? countdown : '--'
            }}
            label={intl.get('before_settlement_countdown')}
          />
        </>
      )
    }
  }

  return (
    <Section
      className={`exchange-header-${theme}`} style={{ position: 'relative' }}>
      <Row className="exchange-header-title">
        <Row
          ref={symbolListPopoverRef}
          className="exchange-header-popover-container"
          onMouseEnter={() => { screenSize !== ScreenSize.MIN && setSymbolListPopoverVisible(true) }}
          onMouseLeave={() => { screenSize !== ScreenSize.MIN && setSymbolListPopoverVisible(false) }}>
          <Popover
            className="exchange-header-symbol-list-popover"
            getContainer={() => symbolListPopoverRef.current}
            visible={symbolListPopoverVisible}
            onOutsideClick={_ => { setSymbolListPopoverVisible(false) }}
          >
            <SymbolList
              onSymbolClick={_ => { setSymbolListPopoverVisible(false) }}
              onMouseLeave={_ => { setSymbolListPopoverVisible(false) }}
            />
          </Popover>
        </Row>
        <div 
          ref={symbolIconRef}
          className="exchange-header-symbol-icon"
          onMouseEnter={() => { screenSize !== ScreenSize.MIN && setSymbolCardVisible(true) }}
          onMouseLeave={() => { screenSize !== ScreenSize.MIN && setSymbolCardVisible(false) }}>
          <img 
           src={cardData?.Img || defaultCardImg} 
           alt={currentSymbol.InstrumentID}
           onError={(e) => { e.target.src = defaultCardImg }}
          />
          <Popover
            className="exchange-header-symbol-card-popover"
            getContainer={() => symbolIconRef.current}
            visible={symbolCardVisible}
            onOutsideClick={_ => { setSymbolCardVisible(false) }}
          >
            <SymbolCard symbol={currentSymbol} cardData={cardData} />
          </Popover>
        </div>
        <div className="exchange-header-symbol exchange-header-info-item"> 
          <span className="exchange-header-symbol-name"
            onClick={() => { setSymbolListPopoverVisible(!symbolListPopoverVisible) }}>
            {currentSymbol.InstrumentID}
            <svg viewBox="0 0 1024 1024" width={16} height={16} style={{ marginLeft: 6 }}>
              <path d="M948.560332 281.179984c-13.765515-13.833053-36.127825-13.833053-49.89334 0L511.991302 668.591431 125.313565 281.179984c-13.763468-13.798261-36.093033-13.798261-49.856501 0-13.799284 13.798261-13.799284 36.161594 0 49.993624l410.857439 411.674037c7.067976 7.085372 16.402575 10.521634 25.675776 10.331299 9.274224 0.191358 18.608823-3.245927 25.677822-10.331299l410.891208-411.708829c6.863315-6.89913 10.331299-15.940041 10.331299-24.979928S955.423647 288.078091 948.560332 281.179984z" />
            </svg>
          
          </span>
          <svg
            className="exchange-header-icon"
            viewBox="0 0 1024 1024"
            style={{ height: 20, width: 20 }}
            onClick={() => { setSpread(!spread) }}>
            <path d="M512 42.666667c258.133333 0 469.333333 211.2 469.333333 469.333333s-211.2 469.333333-469.333333 469.333333S42.666667 770.133333 42.666667 512 253.866667 42.666667 512 42.666667z m0 853.333333c211.2 0 384-172.8 384-384S723.2 128 512 128 128 300.8 128 512s172.8 384 384 384z" />
            <path d="M512 448c-36.266667 0-64 27.733333-64 64s27.733333 64 64 64 64-27.733333 64-64-27.733333-64-64-64M725.333333 448c-36.266667 0-64 27.733333-64 64s27.733333 64 64 64 64-27.733333 64-64-27.733333-64-64-64M298.666667 448c-36.266667 0-64 27.733333-64 64s27.733333 64 64 64 64-27.733333 64-64-27.733333-64-64-64" />
          </svg>
          <p
            style={{ display: 'flex' }}
            data-color-type={change > 0 ? 'rise' : (change < 0 ? 'fail' : '')}
            className={`exchange-header-symbol-price ${change > 0 ? 'buy-rise-color' : (change < 0 ? 'sell-fail-color' : '')}`}>
            <span className="header-show">{formatDecimal(formatValue(currentSymbolMarket, 'N'), pricePrecision)}</span>
            <span style={{ fontWeight: 'normal', marginLeft: 8 }} className={change > 0 ? 'buy-rise-color-opacity-70' : (change < 0 ? 'sell-fail-color-opacity-70' : '')}>{formatPercent(change)}</span>
          </p>
          <svg
            style={{ marginLeft: 10, height: 20, width: 20 }}
            className="exchange-header-icon"
            viewBox="0 0 1024 1024"
            onClick={onChartIconClick}>
            <path d="M512 256a42.666667 42.666667 0 0 0-42.666667 42.666667v426.666666a42.666667 42.666667 0 0 0 85.333334 0V298.666667a42.666667 42.666667 0 0 0-42.666667-42.666667z m-213.333333 256a42.666667 42.666667 0 0 0-42.666667 42.666667v170.666666a42.666667 42.666667 0 0 0 85.333333 0v-170.666666a42.666667 42.666667 0 0 0-42.666666-42.666667z m426.666666-85.333333a42.666667 42.666667 0 0 0-42.666666 42.666666v256a42.666667 42.666667 0 0 0 85.333333 0v-256a42.666667 42.666667 0 0 0-42.666667-42.666666z m85.333334-341.333334H213.333333a128 128 0 0 0-128 128v597.333334a128 128 0 0 0 128 128h597.333334a128 128 0 0 0 128-128V213.333333a128 128 0 0 0-128-128z m42.666666 725.333334a42.666667 42.666667 0 0 1-42.666666 42.666666H213.333333a42.666667 42.666667 0 0 1-42.666666-42.666666V213.333333a42.666667 42.666667 0 0 1 42.666666-42.666666h597.333334a42.666667 42.666667 0 0 1 42.666666 42.666666z" />
          </svg>

          
        </div>
        {/* <Row className="header-content-menu header-vsshow" style={{ marginLeft: 'auto' }}>
          <span style={{ paddingRight: 8, fontSize: 13 }} title={userInfo?.MemberID}>
            {
              (() => {
                const emailName = (userInfo?.UserID || '').split('@')[0]
                return emailName.length > 9
                  ? `${emailName.slice(0, 2)}...${emailName.slice(-4)}`
                  : emailName
              })()
            }
          </span>
          <svg viewBox="0 0 1024 1024" width={12} height={12} className="header-content-menu-arrow">
            <path d="M948.560332 281.179984c-13.765515-13.833053-36.127825-13.833053-49.89334 0L511.991302 668.591431 125.313565 281.179984c-13.763468-13.798261-36.093033-13.798261-49.856501 0-13.799284 13.798261-13.799284 36.161594 0 49.993624l410.857439 411.674037c7.067976 7.085372 16.402575 10.521634 25.675776 10.331299 9.274224 0.191358 18.608823-3.245927 25.677822-10.331299l410.891208-411.708829c6.863315-6.89913 10.331299-15.940041 10.331299-24.979928S955.423647 288.078091 948.560332 281.179984z" />
          </svg>
          <ul className="header-content-menu-popover" style={{ marginLeft: 20, fontSize: 13 }}>
            <li>
              <Link className="header-content-menu-link" to={SUBACCOUNT}>+ {intl.get('subaccount')}</Link>
            </li>
          </ul>
        </Row> */}
      
      </Row>
    

      <div>
         <div style={{ color: 'var(--text-secondary)',marginTop:'0' ,fontSize:'18px'}}>
             {currentSymbol?.N?.replace(/_/g, ' ')}
         </div>
        {
        spread && (
          <Row className="exchange-header-info-container">
            {
              currentSymbol?.ProductGroup !== ProductGroup.SPOT &&
              <Info
                value={{ v: formatDecimal(formatValue(currentSymbolMarket, 'M'), pricePrecision) }}
                label={`${intl.get('mark_price')}(${priceUnit})`} />
            }
            
            <Info
              style={{display:'flex', columnGap:'10px',minWidth:220}}
              value={{ v: formatDecimal(turnover, pricePrecision) }}
              label={`${intl.get('24h_turnover')}(${priceUnit})`} />
            {tradingStatus && (
              <div style={{ 
                marginLeft: 8, 
                padding: '2px 8px', 
                fontSize: 12, 
                borderRadius: 4,
                display: 'inline-flex',
                alignItems: 'center',
                gap: '6px',
                backgroundColor: tradingStatusStyle.backgroundColor,
                color: tradingStatusStyle.color,
                border: `1px solid ${tradingStatusStyle.borderColor}`
              }}>
                <span style={{
                  width: '6px',
                  height: '6px',
                  borderRadius: '50%',
                  backgroundColor: 'currentColor',
                  animation: currentSymbolMarket?.B === '2' ? 'pulse 2s ease-in-out infinite' : 'none'
                }} />
                {tradingStatus}
              </div>
            )}
            {/* {
              currentSymbol?.ProductGroup !== ProductGroup.SPOT && (
                <Info
                  value={{
                    v: formatDecimalNoZero(
                      VolumeUnitType.transferVolumeForUnit(volumeUnitType, {
                        symbol: currentSymbol,
                        volume: formatValue(currentSymbolMarket, 'P'),
                        price: currentSymbolMarket.N
                      }),
                      volumePrecision
                    )
                  }}
                  label={`${intl.get('position_volume')}(${volumeUnit})`}
                />
              )
            } */}
            {/* {renderCountdown()} */}
          </Row>
        )
        }

      </div>
       
     
      
      {authStatus === Status.LOGIN ? (
        <>
          <div style={{ position: 'absolute', right: '12px' }}>
          <Space size={12}>
           {/* <AntButton
             type='primary'
             color="default"
             variant="solid"
             style={{ height: '30px', width: '70px' }}
             onClick={() => setRankModalOpen(true)}
           >
          {intl.get('rank')}
           </AntButton> */}
            <Link to={buybackTo} >
              <AntButton type='primary' color="default" variant="solid"style={{ height: '30px', width: '70px' }}>{intl.get('buyback')}</AntButton>
            </Link>
         </Space>
          </div>

          {/* <PageRankModal
            open={rankModalOpen}
            onClose={() => setRankModalOpen(false)}
            currency={currentSymbol?.BaseCurrency}
          /> */}
        </>
      ) : null}
     
    </Section>
  )
})

export default memo(Header)
