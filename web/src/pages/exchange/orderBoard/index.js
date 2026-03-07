import React, { useState, useContext, useImperativeHandle, forwardRef, useMemo, useEffect, memo, useRef } from 'react'

import { Link } from 'react-router-dom'

import { OverlayScrollbarsComponent } from 'overlayscrollbars-react'
import Decimal from 'decimal.js'

import Row from '../../../component/row'
import Col from '../../../component/col'
import Toggle from '../../../component/toggle'
import Input from '../../../component/input'
import Slider from '../../../component/slider'
import Select from '../../../component/select'
import Button from '../../../component/button'
import Checkbox from '../../../component/checkbox'
import Tooltip from '../../../component/tooltip'
import Popover from '../../../component/popover'
import List from '../../../component/list'
import { Tip, Transfer, Edit, Clear, Arrow } from '../../../component/icons'
import useScreenSize, { ScreenSize } from '../../../component/hooks/useScreenSize'
import intl from '../../../component/intl'
import ComponentContext from '../../../component/context'
import PieChart from '../../../component/echarts/PieChart'

import Section from '../components/section'

import RootContext from '../../rootContext'
import ExchangeContext from '../context'
import { OrderConfirmModal, StopProfitLossModal, TransferModal } from '../modal'

import { formatValue, formatDecimal, formatDecimalNoZero } from '../../../utils/format'

import { getTradeConfig } from '../../../api/contract'
import { fetchPositionRank } from '../../../api/buyback'

import { Status } from '../../../constants/auth'
import { ProductGroup } from '../../../constants/symbol'
import { Direction as PositionDirection, Type as PositionType, MarginMode } from '../../../constants/position'
import { Type as OrderType, PriceType, Direction as OrderDirection, OffsetFlag, VolumeMode, InstructType, TriggerOrderType, TriggerOrderTriggerPriceType } from '../../../constants/order'
import VolumeUnitType from '../../../constants/volumeUnitType'

import { LOGIN, REGISTER } from '../../routes'

import { POLLING_INTERVAL, UPDATE_DELAY } from '../config'

import { TOKEN, getStorage, removeStorage } from '../../../utils/storage'

import { logout } from '../../../api/authApi'

import './index.less'

import { useAccountContext } from '../accountContext.js'
import WalletModal from '../../walletmodal'
import {formatDecimals} from '@/utils/number'
import {formatNumber} from '@/utils/format'



const TAB_ORDER_LIMIT = 'limit'
const TAB_ORDER_MARKET = 'market'
const TAB_ORDER_PLAN = 'plan'

const INPUT_TYPE_TRIGGER_PRICE = 'trigger_price'
const INPUT_TYPE_PRICE = 'price'
const INPUT_TYPE_VOLUME = 'volume'

export default memo(forwardRef(({ onOrderSuccess, onLeverClick = () => {} }, ref) => {
  const { accountResult = [], accountRequest } = useAccountContext()

  const priceFlag = useRef(false)

  const triggerPriceInputContainerRef = useRef()

  const { theme } = useContext(ComponentContext)

  const [openCloseTab, setOpenCloseTab] = useState(OffsetFlag.OPEN)
  const [orderTab, setOrderTab] = useState(TAB_ORDER_LIMIT)

  const { authStatus, userInfo = {}, setAuthStatus, setUserInfo } = useContext(RootContext)
  const {
    orderConfirm, setOrderConfirm,
    currentSymbol = {}, currentSymbolMarket = {}, symbols, markets,
    levers, marginMode, positionType,
    volumeUnitType, positionList = []
  } = useContext(ExchangeContext)

  const screenSize = useScreenSize()

  const [triggerPriceType, setTriggerPriceType] = useState(TriggerOrderTriggerPriceType.NORMAL)
  const [triggerPriceTypeListVisible, setTriggerPriceTypeListVisible] = useState(false)
  const [triggerPrice, setTriggerPrice] = useState('')

  const [priceType, setPriceType] = useState(PriceType.LIMIT)
  const [price, setPrice] = useState('')
  const [volumeMode, setVolumeMode] = useState(VolumeMode.VALUE)
  const [volume, setVolume] = useState('')
  const [volumePercent, setVolumePercent] = useState(0)
  const [inputError, setInputError] = useState({})

  const [tpTriggerPrice, setTpTriggerPrice] = useState('')
  const [slTriggerPrice, setSlTriggerPrice] = useState('')
  const [tradeConfig, setTradeConfig] = useState({})
  const [instructType, setInstructType] = useState({ key: InstructType.NORMAL, text: 'GTC' })

  const tradeConfigRequest = async () => {
    try {
      const result = await getTradeConfig({
        MemberID: userInfo?.MemberID,
        AccountID: userInfo?.MemberID,
        TradeUnitID: userInfo?.MemberID,
        InstrumentID: currentSymbol?.InstrumentID,
        ExchangeID: currentSymbol?.ExchangeID
      })
      setTradeConfig(result)
    } catch (err) {
      setTradeConfig({})
    }
  }
  const updateData = () => {
    tradeConfigRequest()
    accountRequest()
  }

  const setOrderPrice = (price) => {
    if (priceType === PriceType.LIMIT) {
      priceFlag.current = true
      setPrice(price)
    }
  }

  useImperativeHandle(ref, () => ({
    updateData, setOrderPrice
  }))

  useEffect(() => {
    if (!priceFlag.current && currentSymbolMarket.LastPrice && orderTab === TAB_ORDER_LIMIT) {
      setPrice(currentSymbolMarket.LastPrice)
      priceFlag.current = true
    }
  }, [currentSymbolMarket.LastPrice, orderTab])

  useEffect(() => {
    if (currentSymbol?.InstrumentID) {
      priceFlag.current = false
      reset()
    }
  }, [currentSymbol?.InstrumentID])

  useEffect(() => {
    setVolume('')
    setVolumePercent(0)
    setVolumeMode(VolumeMode.VALUE)
  }, [volumeUnitType])

  useEffect(() => {
    setOpenCloseTab(OffsetFlag.OPEN)
    reset(TAB_ORDER_LIMIT)
  }, [positionType])

  const reset = (tab) => {
    setTriggerPrice('')
    setTriggerPriceType(TriggerOrderTriggerPriceType.NORMAL)
    let price = ''
    if (tab) {
      setOrderTab(tab)
      if (tab === TAB_ORDER_MARKET) {
        setPriceType(PriceType.MARKET)
        priceFlag.current = false
      } else {
        setPriceType(PriceType.LIMIT)
        if (tab === TAB_ORDER_LIMIT) {
          price = currentSymbolMarket.LastPrice || ''
          priceFlag.current = !!price
        } else {
          priceFlag.current = false
        }
      }
    }
    setPrice(price)
    setVolume('')
    setVolumePercent(0)
    setVolumeMode(VolumeMode.VALUE)
    setTpTriggerPrice('')
    setSlTriggerPrice('')
    setInstructType({ key: InstructType.NORMAL, text: 'GTC' })
    setInputError({})
  }

  const openStopProfitLossModal = () => {
    StopProfitLossModal.open({
      isMobile: screenSize === ScreenSize.MIN,
      from: StopProfitLossModal.FROM_ORDER_BOARD,
      positionType,
      data: {
        OrderPriceType: priceType,
        Price: price,
        TPTriggerPrice: tpTriggerPrice,
        SLTriggerPrice: slTriggerPrice
      },
      symbol: currentSymbol,
      symbolMarket: currentSymbolMarket,
      onSuccess: (params) => {
        setTpTriggerPrice(params.tpTriggerPrice)
        setSlTriggerPrice(params.slTriggerPrice)
      },
      config: { theme }
    })
  }

  const openTransferModal = (profitLoss) => {
    TransferModal.open({
      isMobile: screenSize === ScreenSize.MIN,
      userInfo,
      symbol: currentSymbol,
      profitLoss,
      onSuccess: () => {
        setTimeout(() => {
          accountRequest()
        }, UPDATE_DELAY)
      },
      logout: async () => {
        try {
          await logout({ Token: getStorage(TOKEN) })
          setAuthStatus(Status.LOGOUT)
          setUserInfo(null)
          removeStorage(TOKEN)
        } catch (e) {
          console.log('ee', e)
        }
      },
      config: { theme }
    })
  }

  const orderClick = (direction, orderSheetVolume, orderVolume, margin) => {
    if (currentSymbol?.InstrumentID) {
      let orderType = OrderType.LIMIT
      const params = {
        InstrumentID: currentSymbol?.InstrumentID,
        ExchangeID: currentSymbol.ExchangeID,
        ProductGroup: currentSymbol?.ProductGroup,
        Direction: direction,
        OffsetFlag: openCloseTab,
        OrderType: priceType === PriceType.MARKET ? InstructType.FAK : instructType.key
      }
      const extendParams = {
        volumeUnitType
      }
      if (orderTab === TAB_ORDER_PLAN) {
        if (triggerPriceType === TriggerOrderTriggerPriceType.NORMAL) {
          if (!triggerPrice) {
            setInputError({ type: INPUT_TYPE_TRIGGER_PRICE, message: intl.get('trigger_price_not_be_empty') })
            return
          }
          params.TriggerPrice = +triggerPrice
        } else {

        }
        params.TriggerPriceType = triggerPriceType
        params.TriggerOrderType = TriggerOrderType.PLAN
        orderType = OrderType.PLAN
      }
      if (priceType === PriceType.LIMIT) {
        if (!price) {
          setInputError({ type: INPUT_TYPE_PRICE, message: intl.get('order_price_not_be_empty') })
          return
        }
        params.Price = +price
      }
      params.OrderPriceType = priceType
      if (volumeMode === VolumeMode.VALUE) {
        if (!volume) {
          setInputError({ type: INPUT_TYPE_VOLUME, message: intl.get('order_volume_not_be_empty') })
          return
        }
        params.Volume = orderSheetVolume
        extendParams.showVolume = orderVolume
      } else {
        if (!volumePercent) {
          setInputError({ type: INPUT_TYPE_VOLUME, message: intl.get('order_volume_not_be_empty') })
          return
        }
        // params.Volume = (+volumePercent)
        params.Volume = orderSheetVolume
        extendParams.showVolume = +volumePercent
      }
      extendParams.margin = margin
      extendParams.volumeMode = volumeMode
      if (tpTriggerPrice) {
        params.TPTriggerPrice = +tpTriggerPrice
        params.TriggerOrderType = TriggerOrderType.ORDER_STOP_PROFIT_LOSS
        orderType = OrderType.STOP_PROFIT_LOSS
      }
      if (slTriggerPrice) {
        params.TriggerOrderType = TriggerOrderType.ORDER_STOP_PROFIT_LOSS
        params.SLTriggerPrice = +slTriggerPrice
        orderType = OrderType.STOP_PROFIT_LOSS
      }
      params.extend = extendParams
      OrderConfirmModal.open({
        isMobile: screenSize === ScreenSize.MIN,
        orderConfirm: { confirm: orderConfirm, setOrderConfirm },
        positionType,
        orderType,
        requestParams: params,
        symbol: currentSymbol,
        onSuccess: _ => {
          setTriggerPrice('')
          setTriggerPriceType(TriggerOrderTriggerPriceType.NORMAL)
          if (priceType === PriceType.LIMIT && orderTab !== TAB_ORDER_PLAN) {
            // setPrice(currentSymbolMarket.LastPrice)
          } else {
            setPrice('')
          }
          setVolumeMode(VolumeMode.VALUE)
          setVolume('')
          setVolumePercent(0)
          setTpTriggerPrice('')
          setSlTriggerPrice('')
          setTimeout(() => {
            onOrderSuccess && onOrderSuccess()
            updateData()
          }, UPDATE_DELAY)
        },
        config: { theme }
      })
    }
  }

  const accountUnrealizedProfitLoss = useMemo(() => {
    let profitLoss = 0
    if (ProductGroup.isOpenClose(currentSymbol.ProductGroup) && positionList.length > 0) {
      positionList.forEach(position => {
        const symbol = formatValue(symbols, position?.InstrumentID, {})
        if (symbol.ClearCurrency === currentSymbol.ClearCurrency) {
          const volumeMultiple = formatValue(symbol, 'VolumeMultiple', 0)
          const volume = formatValue(position, 'Position', 0)
          const market = formatValue(markets, symbol?.InstrumentID, {})
          const markerPrice = market.MarkedPrice
          const openPrice = position.OpenPrice
          let pl = 0
          if (markerPrice && openPrice) {
            let difPrice
            if (position.Direction === OrderDirection.BUY) {
              difPrice = markerPrice - openPrice
            } else {
              difPrice = openPrice - markerPrice
            }
            if (symbol.IsInverse) {
              const coinVolume = volume * volumeMultiple / openPrice
              pl = coinVolume * difPrice / markerPrice
            } else {
              const coinVolume = volume * volumeMultiple
              pl = coinVolume * difPrice
            }
          }
          profitLoss += pl
        }
      })
    }
    return profitLoss
  }, [currentSymbol.ClearCurrency, positionList, markets])

  const calcResult = useMemo(() => {
    // console.log('currentSymbol', currentSymbol)
    let available = '--'
    let useMargin = '--'
    let balance = '--'
    let buyCanOpenAvailable = 0
    let sellCanOpenAvailable = 0
    if (accountResult) {
      if (!ProductGroup.isOpenClose(currentSymbol.ProductGroup)) {
        const buyAccount = accountResult.find(account => account.Currency === currentSymbol.ClearCurrency)
        const sellAccount = accountResult.find(account => account.Currency === currentSymbol.BaseCurrency)
        buyCanOpenAvailable = Math.max(0, formatValue(buyAccount, 'Available', 0))
        available = buyCanOpenAvailable // 用 ClearCurrency（比如 USDT）对应账户的可用余额来显示“可用”，
        sellCanOpenAvailable = Math.max(0, formatValue(sellAccount, 'Available', 0))
      } else {
        const account = accountResult.find(account => account.Currency === currentSymbol.ClearCurrency)
        if (account) {
          // const unrealizedProfitLoss = marginMode === MarginMode.CROSS ? (accountUnrealizedProfitLoss || 0) : 0
          // available = Math.max(0, formatValue(account, 'Available', 0) + unrealizedProfitLoss)
          available = formatValue(account, 'Available', 0)
          useMargin = formatValue(account, 'UseMargin', 0)
          balance = formatValue(account, 'Balance', 0)
          if (positionType === PositionType.SINGLE) {
            if (positionList.length > 0) {
              buyCanOpenAvailable = available
              sellCanOpenAvailable = available
              const position = positionList.find(
                data => (
                  data.PosiDirection === PositionDirection.NET &&
                  currentSymbol?.InstrumentID === data?.InstrumentID &&
                  currentSymbol.ExchangeID === data.ExchangeID
                )
              )
              if (position) {
                // 买入一侧可开 = 可用保证金+max(当前下单交易对多头持仓保证金+当前下单交易对所有买单冻结保证金, 当前下单交易对空头持仓保证金+当前下单交易对所有卖单冻结保证金)-当前下单交易对多头持仓保证金-当前下单交易对所有买单冻结保证金
                const positionUseMargin = formatValue(position, 'UseMargin', 0)
                const positionLongUseMargin = position.Direction === OrderDirection.BUY ? positionUseMargin : 0
                const positionShortUseMargin = position.Direction === OrderDirection.SELL ? positionUseMargin : 0
                const positionLongFrozenMargin = formatValue(position, 'LongFrozenMargin', 0)
                const positionShortFrozenMargin = formatValue(position, 'ShortFrozenMargin', 0)
                const positionMaxMargin = Math.max(positionLongUseMargin + positionLongFrozenMargin, positionShortUseMargin + positionShortFrozenMargin)
                buyCanOpenAvailable += (positionMaxMargin - positionLongUseMargin - positionLongFrozenMargin)
                sellCanOpenAvailable += (positionMaxMargin - positionShortUseMargin - positionShortFrozenMargin)
              }
            }
          } else {
            buyCanOpenAvailable = available
            sellCanOpenAvailable = available
          }
        }
      }
    }
    let buyMaxSheetVolume = '--'
    let sellMaxSheetVolume = '--'
    let buyMaxVolume = '--'
    let sellMaxVolume = '--'
    let buyTransferPrice
    let sellTransferPrice
    let buyOrderPrice = +(price || currentSymbolMarket.LastPrice)
    let sellOrderPrice = buyOrderPrice
    if (orderTab === TAB_ORDER_MARKET) {
      buyOrderPrice = currentSymbolMarket.MarkedPrice * (1 + 0.0005)
      sellOrderPrice = currentSymbolMarket.MarkedPrice * (1 - 0.0005)
    }
    if (currentSymbol?.InstrumentID) {
      if (positionType === PositionType.SINGLE || openCloseTab === OffsetFlag.OPEN) {
        if (!ProductGroup.isOpenClose(currentSymbol.ProductGroup)) {
          if (buyOrderPrice) {
            buyMaxVolume = buyCanOpenAvailable / buyOrderPrice
          }
          sellMaxVolume = sellCanOpenAvailable
        } else {
          // 开仓
          if (accountResult.length > 0) {
            const fee = +((currentSymbol.Fee || {}).takerOpenFeeRate || 0)
            const volumeMultiple = formatValue(currentSymbol, 'VolumeMultiple', 1)
            const config = tradeConfig || {}
            if (buyOrderPrice) {
              let single
              if (currentSymbol.IsInverse) {
                // 反向合约
                // 币本位： 可开仓数量(张) = 可用资金 * 价格 * 杠杆倍数 / 合约乘数
                single = volumeMultiple / buyOrderPrice
              } else {
                // 正向合约
                // USDT本位： 可开仓数量(张) = 可用资金 / 价格 / 合约乘数  * 杠杆倍数
                single = buyOrderPrice * volumeMultiple
              }
              buyMaxSheetVolume = Math.min(
                buyCanOpenAvailable / (single / levers.long + single * fee),
                formatValue(config, 'LongLastVolume', Number.MAX_SAFE_INTEGER)
              )
              buyTransferPrice = buyOrderPrice
              buyMaxVolume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, {
                symbol: currentSymbol,
                volume: buyMaxSheetVolume,
                price: buyTransferPrice
              })
            }
            if (sellOrderPrice) {
              let single
              if (currentSymbol.IsInverse) {
                // 反向合约
                // 币本位： 可开仓数量(张) = 可用资金 * 价格 * 杠杆倍数 / 合约乘数
                single = volumeMultiple / sellOrderPrice
              } else {
                // 正向合约
                // USDT本位： 可开仓数量(张) = 可用资金 / 价格 / 合约乘数  * 杠杆倍数
                single = sellOrderPrice * volumeMultiple
              }
              sellMaxSheetVolume = Math.min(
                sellCanOpenAvailable / (single / levers.short + single * fee),
                formatValue(config, 'ShortLastVolume', Number.MAX_SAFE_INTEGER)
              )
              sellTransferPrice = sellOrderPrice
              sellMaxVolume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, {
                symbol: currentSymbol,
                volume: sellMaxSheetVolume,
                price: sellTransferPrice
              })
            }
          }
        }
      } else {
        if (positionList.length > 0) {
          const shortPosition = positionList.find(
            data => (
              data.PosiDirection === PositionDirection.SHORT &&
              currentSymbol?.InstrumentID === data?.InstrumentID &&
              currentSymbol.ExchangeID === data.ExchangeID
            )
          )
          if (shortPosition) {
            buyMaxSheetVolume = formatValue(shortPosition, 'ClosePosition')
            buyTransferPrice = shortPosition.OpenPrice
            buyMaxVolume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, {
              symbol: currentSymbol,
              volume: buyMaxSheetVolume,
              price: buyTransferPrice
            })
          }

          const longPosition = positionList.find(
            data => (
              data.PosiDirection === PositionDirection.LONG &&
              currentSymbol?.InstrumentID === data?.InstrumentID &&
              currentSymbol.ExchangeID === data.ExchangeID
            )
          )
          if (longPosition) {
            sellMaxSheetVolume = formatValue(longPosition, 'ClosePosition')
            sellTransferPrice = longPosition.OpenPrice
            sellMaxVolume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, {
              symbol: currentSymbol,
              volume: sellMaxSheetVolume,
              price: sellTransferPrice
            })
          }
        }
      }
    }
    return {
      available,
      buyCanOpenAvailable,
      sellCanOpenAvailable,
      useMargin,
      balance,
      buyMaxSheetVolume,
      buyMaxVolume,
      sellMaxSheetVolume,
      sellMaxVolume,
      buyTransferPrice,
      sellTransferPrice
    }
  }, [
    currentSymbol.InstrumentID,
    accountResult,
    (price || currentSymbolMarket.LastPrice),
    currentSymbolMarket.MarkedPrice, accountUnrealizedProfitLoss,
    openCloseTab, orderTab, volumeUnitType, levers, marginMode, positionType,
    (tradeConfig || {}).LongLastVolume,
    (tradeConfig || {}).ShortLastVolume,
    positionList
  ])

  const volumes = useMemo(() => {
    let buyOrderSheetVolume
    let sellOrderSheetVolume
    let buyOrderVolume
    let sellOrderVolume
    let buyMargin
    let sellMargin
    if (currentSymbol?.InstrumentID) {
      if (volumeMode === VolumeMode.VALUE) {
        if (!ProductGroup.isOpenClose(currentSymbol.ProductGroup)) {
          buyOrderSheetVolume = +volume
          sellOrderSheetVolume = +volume
        } else {
          if (volumeUnitType === VolumeUnitType.SHEET) {
            buyOrderSheetVolume = +volume
            sellOrderSheetVolume = +volume
          } else {
            buyOrderSheetVolume = VolumeUnitType.coinToSheet({ volume: +volume, price: calcResult.buyTransferPrice, symbol: currentSymbol })
            sellOrderSheetVolume = VolumeUnitType.coinToSheet({ volume: +volume, price: calcResult.sellTransferPrice, symbol: currentSymbol })
          }
        }
        buyOrderVolume = +volume
        sellOrderVolume = +volume
      } else {
        const power = Math.pow(10, formatValue(currentSymbol, 'VolumePrecision', 0))
        if (!ProductGroup.isOpenClose(currentSymbol.ProductGroup)) {
          buyOrderSheetVolume = Math.round((+calcResult.buyMaxVolume) * (+volumePercent) / 100 * power) / power
          sellOrderSheetVolume = Math.round((+calcResult.sellMaxVolume) * (+volumePercent) / 100 * power) / power
          buyOrderVolume = buyOrderSheetVolume
          sellOrderVolume = sellOrderSheetVolume
        } else {
          buyOrderSheetVolume = Math.round((+calcResult.buyMaxSheetVolume) * (+volumePercent) / 100 * power) / power
          sellOrderSheetVolume = Math.round((+calcResult.sellMaxSheetVolume) * (+volumePercent) / 100 * power) / power
          buyOrderVolume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume: buyOrderSheetVolume, price: calcResult.buyTransferPrice, symbol: currentSymbol })
          sellOrderVolume = VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume: sellOrderSheetVolume, price: calcResult.buyTransferPrice, symbol: currentSymbol })
        }
      }
      const fee = +((currentSymbol.Fee || {}).takerOpenFeeRate || 0)
      const volumeMultiple = formatValue(currentSymbol, 'VolumeMultiple', 1)

      if (calcResult.buyTransferPrice) {
        let buyTotal
        if (currentSymbol.IsInverse) {
          // 反向合约
          buyTotal = buyOrderSheetVolume * volumeMultiple / calcResult.buyTransferPrice
        } else {
          // 正向合约
          buyTotal = buyOrderSheetVolume * volumeMultiple * calcResult.buyTransferPrice
        }
        buyMargin = buyTotal / levers.long + buyTotal * fee
      }
      if (calcResult.sellTransferPrice) {
        let sellTotal
        if (currentSymbol.IsInverse) {
          // 反向合约
          sellTotal = sellOrderSheetVolume * volumeMultiple / calcResult.sellTransferPrice
        } else {
          // 正向合约
          sellTotal = sellOrderSheetVolume * volumeMultiple * calcResult.sellTransferPrice
        }
        sellMargin = sellTotal / levers.short + sellTotal * fee
      }
    }
    return {
      buyOrderSheetVolume,
      sellOrderSheetVolume,
      buyOrderVolume,
      sellOrderVolume,
      buyMargin,
      sellMargin
    }
  }, [
    currentSymbol?.InstrumentID,
    calcResult.buyTransferPrice, calcResult.sellTransferPrice,
    calcResult.buyMaxSheetVolume, calcResult.sellMaxSheetVolume,
    volumeMode, volume, volumePercent, volumeUnitType, levers
  ])

  const isSinglePosition = positionType === PositionType.SINGLE
  const isVolumeSheet = volumeUnitType === VolumeUnitType.SHEET
  const pricePrecision = formatValue(currentSymbol, 'PricePrecision', 0)
  const volumePrecision = isVolumeSheet ? formatValue(currentSymbol, 'VolumePrecision', 0) : formatValue(currentSymbol, 'CoinPrecision', 0)
  const otherPrecision = formatValue(currentSymbol, 'OtherPrecision', 0)

  const marginUnit = formatValue(currentSymbol, 'ClearCurrency')
  const priceUnit = formatValue(currentSymbol, 'PriceCurrency')
  const volumeUnit = isVolumeSheet ? intl.get('sheet') : formatValue(currentSymbol, 'BaseCurrency')

  // const balanceText = formatDecimal(calcResult.balance, otherPrecision)
  const renderPriceInput = () => {
    return (
      <>
        {
          orderTab === TAB_ORDER_PLAN &&
          <>
            <Row
              ref={triggerPriceInputContainerRef}
              style={{ position: 'relative' }}>
              <Input
                valueStyle={{ textAlign: 'right' }}
                prefix={intl.get('trigger_price')}
                suffix={priceUnit}
                precision={pricePrecision}
                min={0}
                errorMessage={inputError.type === INPUT_TYPE_TRIGGER_PRICE ? inputError.message : null}
                value={
                  triggerPriceType === TriggerOrderTriggerPriceType.NORMAL
                    ? triggerPrice
                    : TriggerOrderTriggerPriceType.transfer(triggerPriceType)}
                onFocus={_ => {
                  setInputError({})
                  setTriggerPriceTypeListVisible(true)
                  if (triggerPriceType !== TriggerOrderTriggerPriceType.NORMAL) {
                    setTriggerPrice('')
                    setTriggerPriceType(TriggerOrderTriggerPriceType.NORMAL)
                  }
                }}
                onBlur={_ => {
                  setTimeout(() => {
                    setTriggerPriceTypeListVisible(false)
                  }, 200)
                }}
                onChange={setTriggerPrice} />
              <Popover
                style={{ width: '100%' }}
                getContainer={() => triggerPriceInputContainerRef.current}
                visible={triggerPriceTypeListVisible}>
                <List
                  dataSource={TriggerOrderTriggerPriceType.createOptions()}
                  onRowClick={(data) => {
                    setTriggerPriceType(data.key)
                    setTriggerPriceTypeListVisible(false)
                  }}
                  renderRow={(data) => (
                    <span
                      style={{ fontSize: 14, padding: '4px 0' }}>
                      {data.text}
                    </span>
                  )} />
              </Popover>
            </Row>
            <Input
              valueStyle={{ textAlign: 'right' }}
              prefix={intl.get('limit_Price')}
              suffix={priceUnit}
              disabled={priceType === PriceType.MARKET}
              precision={pricePrecision}
              min={0}
              errorMessage={inputError.type === INPUT_TYPE_PRICE ? inputError.message : null}
              value={priceType === PriceType.MARKET ? PriceType.transfer(priceType) : price}
              onFocus={_ => { setInputError({}) }}
              onChange={v => {
                if (orderTab === TAB_ORDER_LIMIT) {
                  priceFlag.current = true
                }
                setPrice(v)
              }} />
          </>
        }
        {
          orderTab !== TAB_ORDER_PLAN &&
          <Input
            valueStyle={{ textAlign: 'right' }}
            prefix={intl.get('price')}
            suffix={priceUnit}
            disabled={priceType === PriceType.MARKET}
            precision={pricePrecision}
            min={0}
            errorMessage={inputError.type === INPUT_TYPE_PRICE ? inputError.message : null}
            value={priceType === PriceType.MARKET ? PriceType.transfer(priceType) : price}
            onFocus={_ => { setInputError({}) }}
            onChange={v => {
              if (orderTab === TAB_ORDER_LIMIT) {
                priceFlag.current = true
              }
              setPrice(v)
            }} />
        }
      </>
    )
  }

  const renderVolumeInput = () => {
    // 获取最小变动价位 VT,用于限制数量输入的最小精度
    const minVolumeStep = currentSymbol?.VT || 0.01
    
    // 处理失焦时的精度验证
    const handleVolumeBlur = () => {
      if (volume && minVolumeStep) {
        const numValue = parseFloat(volume)
        if (!isNaN(numValue) && numValue > 0) {
          // 使用 Decimal 进行精确计算,避免浮点数精度问题
          const valueDecimal = new Decimal(numValue)
          const stepDecimal = new Decimal(minVolumeStep)
          
          // 计算余数
          const remainder = valueDecimal.mod(stepDecimal)
          
          // 如果余数不为0,说明不符合最小精度
          if (!remainder.isZero()) {
            // 四舍五入到最接近的合法值
            const quotient = valueDecimal.dividedBy(stepDecimal).toDecimalPlaces(0, Decimal.ROUND_HALF_UP)
            const roundedValue = quotient.times(stepDecimal).toNumber()
            
            // 确保结果不为0
            if (roundedValue > 0) {
              setVolume(String(roundedValue))
            }
          }
        }
      }
    }
    
    return (
      <>
        <Input
          valueStyle={{ textAlign: 'right' }}
          prefix={intl.get('volume')}
          suffix={volumeUnit}
          min={0}
          max={currentSymbol.MaxOrderVolume || Number.MAX_SAFE_INTEGER}
          value={volumeMode === VolumeMode.VALUE ? volume : `${volumePercent}%`}
          precision={volumePrecision}
          errorMessage={inputError.type === INPUT_TYPE_VOLUME ? inputError.message : null}
          onFocus={_ => {
            if (volumeMode === VolumeMode.PERCENT) {
              setVolumeMode(VolumeMode.VALUE)
              setVolume('')
              setVolumePercent(0)
            }
            setInputError({})
          }}
          onChange={setVolume}
          onBlur={handleVolumeBlur} />
        <Slider
          showTip
          value={volumePercent}
          onChange={percent => {
            if (percent === 0) {
              setVolumeMode(VolumeMode.VALUE)
            } else {
              setVolumeMode(VolumeMode.PERCENT)
            }
            setVolume('')
            setVolumePercent(percent)
          }} />
      </>
    )
  }

  const renderStopProfitLoss = () => {
    if (tpTriggerPrice || slTriggerPrice) {
      return (
        <Row
          className="exchange-order-board-item">
          <Col>
            {
              tpTriggerPrice &&
              <Row
                style={{ height: 18 }}
                className="exchange-order-board-item">
                <span style={{ paddingRight: 6 }}>{intl.get('stop_profit')}:</span>
                <span
                  style={{ fontSize: 12 }}
                  className="value">
                  {formatDecimal(tpTriggerPrice, pricePrecision)}&nbsp;{priceUnit}
                </span>
              </Row>
            }
            {
              slTriggerPrice &&
              <Row
                style={{ height: 18 }}
                className="exchange-order-board-item">
                <span style={{ paddingRight: 6 }}>{intl.get('stop_loss')}:</span>
                <span
                  style={{ fontSize: 12 }}
                  className="value">
                  {formatDecimal(slTriggerPrice, pricePrecision)}&nbsp;{priceUnit}
                </span>
              </Row>
            }
          </Col>
          <Col>
            {
              tpTriggerPrice &&
              <Row
                style={{ height: 18 }}>
                <Edit
                  className="stop-profit-loss-button-icon"
                  onClick={openStopProfitLossModal} />
                <Clear
                  className="stop-profit-loss-button-icon"
                  onClick={_ => { setTpTriggerPrice('') }} />
              </Row>
            }
            {
              slTriggerPrice &&
              <Row
                style={{ height: 18 }}>
                <Edit
                  className="stop-profit-loss-button-icon"
                  onClick={openStopProfitLossModal} />
                <Clear
                  className="stop-profit-loss-button-icon"
                  onClick={_ => { setSlTriggerPrice('') }} />
              </Row>
            }
          </Col>
        </Row>
      )
    }
    return (
      <Row
        className="stop-profit-loss-button"
        type="tertiary"
        size="big"
        disabled={priceType === PriceType.LIMIT && !price}
        onClick={openStopProfitLossModal}>
        {intl.get('stop_profit_loss')}
        {/* <Tooltip
          style={{ marginLeft: 8 }}
          tip={intl.get('stop_profit_loss_tip')}>
          <Tip className="tip-icon" />
        </Tooltip> */}
      </Row>
    )
  }

  // 持有排行榜数据
  const [holdersData, setHoldersData] = useState([])
  const [totalBalance, setTotalBalance] = useState(0)

  // 获取持有排行榜数据
  useEffect(() => {
    if (currentSymbol?.BaseCurrency && authStatus === Status.LOGIN) {
      fetchPositionRank({
        currency: currentSymbol.BaseCurrency
      }).then(result => {
        // 根据API返回结构获取列表数据和总量
        const list = (result && result.data && result.data.rankList) || result || []
        const total = (result && result.data && result.data.total) || 0
        
        if (Array.isArray(list)) {
          setHoldersData(list)
          setTotalBalance(Number(total))
        } else {
          setHoldersData([])
          setTotalBalance(0)
        }
      }).catch(() => {
        setHoldersData([])
        setTotalBalance(0)
      })
    } else {
      setHoldersData([])
      setTotalBalance(0)
    }
  }, [currentSymbol?.BaseCurrency, authStatus])

  const [modalOpen, setModalOpen] = useState(false)
  const renderButtons = () => {
    if (authStatus === Status.LOGIN) {
      let buyText
      let sellText
      if (isSinglePosition || !ProductGroup.isOpenClose(currentSymbol.ProductGroup)) {
        buyText = intl.get('buy_in')
        sellText = intl.get('sell_out')
      } else {
        buyText = openCloseTab === OffsetFlag.OPEN ? intl.get('open_much') : intl.get('close_void')
        sellText = openCloseTab === OffsetFlag.OPEN ? intl.get('open_void') : intl.get('close_much')
      }
      return (
        <Row
          className="exchange-order-board-item">
          <Button
            className="order-button buy-rise-button"
            onClick={_ => { orderClick(OrderDirection.BUY, volumes.buyOrderSheetVolume, volumes.buyOrderVolume, volumes.buyMargin) }}>
            {buyText}
          </Button>
          {/* {
            positionType === PositionType.SINGLE ? (
              <Button
                className="exchange-trading-setting-button"
                type="tertiary"
                size="small"
                onClick={() => { onLeverClick(PositionDirection.NET) }}>
                {levers.long}X
              </Button>
            ) : (
              <>
                <Button
                  className="exchange-trading-setting-button buy-rise-border-background-opacity-10 buy-rise-color"
                  type="tertiary"
                  size="small"
                  onClick={() => { onLeverClick(PositionDirection.LONG) }}>
                  {levers.long}X
                </Button>
                <Button
                  className="exchange-trading-setting-button sell-fail-border-background-opacity-10 sell-fail-color"
                  type="tertiary"
                  size="small"
                  onClick={() => { onLeverClick(PositionDirection.SHORT) }}>
                  {levers.short}X
                </Button>
              </>
            )
          } */}
          <Button
            className="order-button sell-fail-button"
            onClick={_ => { orderClick(OrderDirection.SELL, volumes.sellOrderSheetVolume, volumes.sellOrderVolume, volumes.sellMargin) }}>
            {sellText}
          </Button>
        </Row>
      )
    }
    return (
      <>
        <Button
          className="exchange-order-board-logout-button"
          onClick={() => setModalOpen(true)}
          type="tertiary">
          {intl.get('login')}
          {/* <Link className="loginIN" to={LOGIN}>{intl.get('login')}</Link> */}
          {/* <span style={{ padding: '0 8px' }}>{intl.get('or')}</span>
        <Link className="signUp" to={REGISTER}>{intl.get('sign_up')}</Link> */}
        </Button>
        <WalletModal
          open={modalOpen}
          onClose={() => setModalOpen(false)}
        />
      </>
    )
  }

  const renderAvailable = () => {
    let canLeftText
    let canRightText
    if (isSinglePosition || !ProductGroup.isOpenClose(currentSymbol.ProductGroup)) {
      canLeftText = intl.get('can_buy')
      canRightText = intl.get('can_sell')
    } else {
      canLeftText = canRightText = openCloseTab === OffsetFlag.OPEN ? intl.get('can_open') : intl.get('can_close')
    }
    return (
      <Row
        className="exchange-order-board-item"
        style={{ height: 20 }}>
        {
          ProductGroup.isOpenClose(currentSymbol.ProductGroup) &&
          <Col style={{ width: '50%' }}>
            <span>{canLeftText}
              {formatDecimalNoZero(calcResult.buyCanOpenAvailable, volumePrecision)}
              {volumeUnit}
            </span>
          </Col>
        }
        {
          !ProductGroup.isOpenClose(currentSymbol.ProductGroup) &&
          <Col style={{ width: '50%' }}>
            <span>{canLeftText}
              {formatDecimalNoZero(calcResult.buyCanOpenAvailable, 0)}
              {/* {formatDecimalNoZero(currentSymbol.BaseCurrency)} */}
            </span>
          </Col>
        }
        {
          ProductGroup.isOpenClose(currentSymbol.ProductGroup) &&
          <Col style={{ width: '50%', textAlign: 'right' }}>
            <span>{canRightText}
              {formatDecimalNoZero(calcResult.sellCanOpenAvailable, volumePrecision)}
              {volumeUnit}
            </span>
          </Col>
        }
        {
          !ProductGroup.isOpenClose(currentSymbol.ProductGroup) &&
          <Col style={{ width: '50%' }}>
            <span>{canLeftText}
              { formatDecimalNoZero(calcResult.buyCanOpenAvailable, 0) }
              {/* { formatDecimalNoZero(currentSymbol.BaseCurrency) } */}
              {/* <Transfer
                onClick={() => { openTransferModal(accountUnrealizedProfitLoss) }} /> */}
            </span>
          </Col>
        }
      </Row>
    )
  }

  // 饼图数据 - 显示当前币种的持有量分布
  const pieChartData = useMemo(() => {
    // console.log('pieChartData calculation:', { holdersData, totalBalance })
    
    if (!holdersData || holdersData?.length === 0 || totalBalance === 0) {
      // console.log('pieChartData: no data', { holdersData, totalBalance })
      return []
    }
    
    try {
      // 使用 Decimal 进行高精度计算
      const totalBalanceDecimal = new Decimal(totalBalance)
      
      // 过滤掉账户ID为88888888的数据,不在饼图上显示
      const filteredHolders = holdersData.filter(holder => {
        const accountId = holder.accountId || ''
        return accountId !== '88888888'
      })
      
      // 显示过滤后的持有者
      const data = filteredHolders.map((holder, index) => {
        // 截取账户ID显示前6位和后4位
        const accountId = holder.accountId || ''
        const shortId = accountId.length > 10 
          ? `${accountId.slice(0, 6)}...${accountId.slice(-4)}`
          : accountId
        
        // 使用持有量
        const balance = holder.balance || 0
        const balanceDecimal = new Decimal(balance)
        
        // 计算百分比 = (balance / totalBalance) * 100
        const percentage = totalBalanceDecimal.greaterThan(0) 
          ? balanceDecimal.dividedBy(totalBalanceDecimal).times(100).toNumber()
          : 0
        
        // 在 name 中添加百分比显示,小于0.1%显示<0.1%
        const displayPercent = percentage < 0.1 ? '<0.1' : percentage.toFixed(1)
        
        return {
          name: String(shortId),
          value:formatDecimals(percentage),
          // 添加自定义字段用于 tooltip 显示
          displayPercent: displayPercent
        }
      }).filter(item => item.value > 0) // 过滤掉 value 为 0 的数据
      
      // console.log('pieChartData after map:', data)
      
      // 计算显示在饼图上的账户的总持有量(不包括88888888)
      const displayedBalanceDecimal = filteredHolders.reduce((sum, holder) => {
        const balance = holder.balance || 0
        return sum.plus(new Decimal(balance))
      }, new Decimal(0))
      
      
      // 判断:如果显示的账户总量不等于总数,就显示"其他"
      // 这样就会包含88888888账户和其他未返回的账户
      if (!displayedBalanceDecimal.equals(totalBalanceDecimal)) {
        // 计算其他持有者的占比 = (总量 - 显示的账户总量) / 总量 * 100
        const othersBalanceDecimal = totalBalanceDecimal.minus(displayedBalanceDecimal)
        const othersPercentage = totalBalanceDecimal.greaterThan(0)
          ? othersBalanceDecimal.dividedBy(totalBalanceDecimal).times(100).toNumber()
          : 0
        
        
        const displayPercent = othersPercentage < 0.1 ? '<0.1' : othersPercentage.toFixed(1)
        data.push({
          name: String(intl.get('others')),
          value: Number(othersPercentage.toFixed(3)),
          displayPercent: displayPercent
        })
        // console.log('已添加"其他"到饼图数据')
      } else {
        // console.log('显示的账户总量等于总数,不显示"其他"')
      }
      
      // console.log('pieChartData final:', data)
      return data
    } catch (error) {
      console.error('pieChartData calculation error:', error)
      return []
    }
  }, [holdersData, totalBalance])

  const renderPieChart = () => {
    if (pieChartData.length === 0) {
      return (
        <div style={{ 
          height: 170, 
          display: 'flex', 
          alignItems: 'center', 
          justifyContent: 'center',
          color: '#888',
          fontSize: 12,
          position: 'absolute',
          bottom: 0,
          left: 0,
          right: 0,
          width: '100%'
        }}>
          {intl.get('no_data')}
        </div>
      )
    }
    
    return (
      <div style={{ 
        width: '100%', 
        height: 200, 
        padding: '10px 0',   
        position: 'absolute',
        bottom: 0,
        left: 0,
        right: 0
      }}>
        <PieChart
          data={pieChartData}
          unit="%"
          option={{
            legend: {
              show: false
            },
            tooltip: {
              formatter: '{b}: {c}%'
            },
            graphic: [
              {
                type: 'text',
                left: 'center',
                top: 'center',
                style: {
                  text: formatNumber(totalBalance),
                  fontSize: 14,
                  fontWeight: 'bold',
                  fill: theme === 'dark' ? '#fff' : '#333',
                  textAlign: 'center'
                }
              },          
            ]
          }}
          seriesOption={{
            radius: ['40%', '70%'],
            label: {
              fontSize: 10,
              formatter: '{b}\n{d}%',
              color: theme === 'dark' ? '#fff' : '#333',
              textBorderWidth: 0
            },
            labelLine: {
              length: 8,
              length2: 5
            }
          }}
        />
      </div>
    )
  }
  return (
    <Section
      className={`exchange-order-board-${theme}`} style={{position:'relative',overflow: 'visible'}}>
      <OverlayScrollbarsComponent
        options={{ 
          overflow: { x: 'hidden' },
          scrollbars: {
            visibility: 'hidden'
          }
        }}>
        {
          (!isSinglePosition && ProductGroup.isOpenClose(currentSymbol.ProductGroup)) &&
          <Toggle
            className="exchange-order-board-open-close-toggle"
            buttonClassName="button"
            value={openCloseTab}
            dataSource={[
              { key: OffsetFlag.OPEN, text: intl.get('open_position') },
              { key: OffsetFlag.CLOSE, text: intl.get('close_position') }
            ]}
            onChange={tab => {
              setOpenCloseTab(tab)
              reset(TAB_ORDER_LIMIT)
            }} />
        }
        <Toggle
          className="exchange-order-board-order-type-toggle"
          buttonClassName="button"
          value={orderTab}
          dataSource={[
            { key: TAB_ORDER_LIMIT, text: intl.get('limit_price') },
            { key: TAB_ORDER_MARKET, text: intl.get('market_price') },
            { key: TAB_ORDER_PLAN, text: intl.get('plan_order') }
          ]}
          onChange={reset} />
        {renderPriceInput()}
        {renderVolumeInput()}
        {
          authStatus === Status.LOGIN &&
          <Row
            className="exchange-order-board-item">
            <span className="align">
              {intl.get('can_used')}
              <span className="value">{formatDecimalNoZero(calcResult.available, otherPrecision)}</span>
              {marginUnit}
            </span>
            <Transfer
              style={{ width: 14, height: 12 }}
              onClick={() => { openTransferModal(accountUnrealizedProfitLoss) }} />
          </Row>
        }
        <Row
          style={{ justifyContent: 'flex-start' }}
          className="exchange-order-board-item">
          {
            <>
              {openCloseTab === OffsetFlag.OPEN && renderStopProfitLoss()}
              {priceType !== PriceType.MARKET &&
                <>
                  <span>TIF</span>
                  <Select
                    className="select"
                    selectorClassName="select-selector"
                    popoverStyle={{ borderRadius: 2, width: 72, left: -22 }}
                    popoverRowClassName="select-popover-row"
                    dataSource={[
                      { key: InstructType.NORMAL, text: 'GTC' },
                      { key: InstructType.FAK, text: 'FAK' },
                      { key: InstructType.FOK, text: 'FOK' },
                      { key: InstructType.ONLY_MAKER, text: 'Post Only' }
                    ]}
                    trigger="hover"
                    arrow={<Arrow className="select-selector-arrow" />}
                    value={instructType.text}
                    onSelected={setInstructType} />
                </>
              }
            </>
          }
          {/* {
            (isSinglePosition) &&
            <Checkbox
              className="checkbox"
              label={intl.get('reduce_only')}
              checked={openCloseTab === OffsetFlag.CLOSE}
              onChecked={checked => { setOpenCloseTab(checked ? OffsetFlag.CLOSE : OffsetFlag.OPEN) }} />
          } */}
        </Row>
        {renderButtons()}
        {authStatus === Status.LOGIN && renderAvailable()}
      
      </OverlayScrollbarsComponent>
        {authStatus === Status.LOGIN && renderPieChart()}
    </Section>
  )
}))
