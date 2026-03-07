import React, { useContext, useState, useEffect, useImperativeHandle, forwardRef, useMemo, memo } from 'react'

import RootContext from '../../../rootContext'
import ExchangeContext from '../../context'

import Table from '../../../../component/table'
import Input from '../../../../component/input'
import Button from '../../../../component/button'
import Row from '../../../../component/row'
import Col from '../../../../component/col'
import Message from '../../../../component/message'
import List from '../../../../component/list'
import ComponentContext from '../../../../component/context'
import { Edit, Clear } from '../../../../component/icons'
import usePolling from '../../../../component/hooks/usePolling'
import useScreenSize, { ScreenSize } from '../../../../component/hooks/useScreenSize'
import intl from '../../../../component/intl'

import { StopProfitLossModal, ConfirmModal, OrderConfirmModal, AdjustMarginModal } from '../../modal'

import { revokeTriggerOrder } from '../../../../api/order'
import { getPosition } from '../../../../api/position'

import { Status } from '../../../../constants/auth'
import { ProductGroup } from '../../../../constants/symbol'
import { MarginMode, transferDirectionTag } from '../../../../constants/position'
import { Type, Direction as OrderDirection, PriceType, OffsetFlag, InstructType } from '../../../../constants/order'
import VolumeUnitType from '../../../../constants/volumeUnitType'

import { formatDecimal, formatValue } from '../../../../utils/format'

import { PAGE_SIZE, POLLING_INTERVAL, UPDATE_DELAY } from '../../config'

const RISK_LEVEL_LOW = 'low'
const RISK_LEVEL_MEDIUM = 'medium'
const RISK_LEVEL_HIGH = 'high'

export default memo(forwardRef(({ type, visible, onLeverClick, onLimitOrderChange }, ref) => {
  const screenSize = useScreenSize()

  const { userInfo, authStatus } = useContext(RootContext)
  const { theme } = useContext(ComponentContext)
  const {
    orderConfirm, setOrderConfirm,
    currentSymbol = {}, currentSymbolMarket = {}, symbols, markets,
    positionType, volumeUnitType,
    setCurrentSymbol
  } = useContext(ExchangeContext)

  const { loading, result = [], request } = usePolling({
    api: getPosition,
    params: {
      AccountID: userInfo?.MemberID
    },
    flag: authStatus === Status.LOGIN && visible,
    interval: POLLING_INTERVAL
  }, [userInfo?.MemberID, authStatus === Status.LOGIN, visible])

  const [currentPage, setCurrentPage] = useState(1)

  useEffect(() => {
    localStorage.setItem('currentOrderPage', currentPage)
  }, [currentPage])

  useEffect(() => {
    const savedPage = localStorage.getItem('currentOrderPage')
    if (savedPage) {
      setCurrentPage(parseInt(savedPage))
    }
  }, [])

  const paginatedData = useMemo(() => {
    const startIndex = (currentPage - 1) * PAGE_SIZE
    return result.slice(startIndex, startIndex + PAGE_SIZE)
  }, [result, currentPage])

  const totalPages = useMemo(() => {
    return Math.ceil(result.length / PAGE_SIZE)
  }, [result])

  const [inputs, setInputs] = useState({})

  useImperativeHandle(ref, _ => ({
    updateData: request
  }))

  const getInputKey = (position) => {
    return `${position?.InstrumentID}_${position.PosiDirection}`
  }

  useEffect(() => {
    const price = currentSymbolMarket.LastPrice
    if (price && result && result.length > 0) {
      const newInputs = {}
      result.forEach(position => {
        const key = getInputKey(position)
        const input = inputs[key] || {}
        newInputs[key] = { ...input }
        if (!input.priceFlag) {
          newInputs[key].price = price
          newInputs[key].priceFlag = true
        }
      })
      setInputs(newInputs)
    }
  }, [currentSymbolMarket.LastPrice, result])

  const revokeStopProfitLoss = position => {
    ConfirmModal.open({
      isMobile: screenSize === ScreenSize.MIN,
      title: intl.get('revoke_order'),
      content: intl.get('confirm_revoke_current_stop_profit_loss'),
      onConfirm: async () => {
        try {
          await revokeTriggerOrder({ OrderID: position.CloseOrderID })
          Message.ok({ message: intl.get('revoke_order_success'), config: { theme } })
        } catch (e) {
          Message.error({ message: `${intl.get('revoke_order_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config: { theme } })
        }
      },
      config: { theme }
    })
  }

  const order = (position, offsetFlag, orderPriceType, price, volume, transferPrice) => {
    const params = {
      ProductGroup: position?.ProductGroup,
      InstrumentID: position?.InstrumentID,
      ExchangeID: position.ExchangeID,
      Direction: position.Direction === OrderDirection.BUY ? OrderDirection.SELL : OrderDirection.BUY,
      OffsetFlag: offsetFlag,
      OrderPriceType: orderPriceType
    }
    const extendParams = { volumeUnitType }
    if (orderPriceType === PriceType.LIMIT) {
      params.OrderType = InstructType.NORMAL
      params.Price = +price
      if (volumeUnitType === VolumeUnitType.SHEET) {
        params.Volume = +volume
      } else {
        // params.Volume = Math.ceil(VolumeUnitType.coinToSheet({ volume: +volume, price: transferPrice, symbol: currentSymbol }))
        params.Volume = VolumeUnitType.coinToSheet({ volume: +volume, price: transferPrice, symbol: currentSymbol })
      }
      extendParams.showVolume = +volume
    } else {
      params.OrderType = InstructType.FAK
    }
    params.extend = extendParams

    OrderConfirmModal.open({
      isMobile: screenSize === ScreenSize.MIN,
      orderConfirm: { confirm: orderConfirm, setOrderConfirm },
      positionType,
      orderType: Type.LIMIT,
      requestParams: params,
      symbol: currentSymbol,
      onSuccess: () => {
        setTimeout(() => {
          onLimitOrderChange && onLimitOrderChange()
          request()
        }, UPDATE_DELAY)
      },
      config: { theme }
    })
  }

  const columns = useMemo(() => {
    const isSheetVolume = volumeUnitType === VolumeUnitType.SHEET
    const volumeUnit = isSheetVolume ? intl.get('sheet') : intl.get('coin')
    const baseColumns = [
      {
        key: 'trade_symbol',
        title: intl.get('trade_variety'),
        width: 180,
        render: (data) => {
          // console.log('symbols', symbols)
          // const symbol = formatValue(symbols, data.InstrumentID, {})
          const symbol = data
          const isBuy = data.Direction === OrderDirection.BUY
          const bgStyleKey = isBuy ? 'buy-rise-border-background-opacity-10' : 'sell-fail-border-background-opacity-10'
          const colorStyleKey = isBuy ? 'buy-rise-color' : 'sell-fail-color'
          return (
            <Row onClick={() => { setCurrentSymbol(symbol) }} style={{ cursor: 'pointer' }}>
              <span
                className={`tag ${bgStyleKey} ${colorStyleKey}`}>
                {transferDirectionTag(data)}
              </span>
              {
                symbol?.ProductGroup !== ProductGroup.SPOT &&
                <span className="position-margin-mode-tag">{MarginMode.transfer(data.IsCrossMargin)}</span>
              }
              <span className="symbol-name">{symbol?.InstrumentID}</span>
            </Row>
          )
        }
      },
      {
        key: 'position_volume',
        title: `${intl.get('position_volume')}(${volumeUnit})`,
        render: data => {
          const symbol = formatValue(symbols, data?.InstrumentID, {})
          const volumePrecision = isSheetVolume ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0)
          return formatDecimal(
            VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume: formatValue(data, 'Position'), price: data.OpenPrice, symbol }),
            volumePrecision
          )
        }
      },
      {
        key: 'open_average_price',
        title: intl.get('open_position_avp_price'),
        render: data => {
          const symbol = formatValue(symbols, data?.InstrumentID, {})
          const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
          return formatDecimal(formatValue(data, 'OpenPrice'), pricePrecision)
        }
      },
      currentSymbol?.ProductGroup !== ProductGroup.SPOT &&
      {
        key: 'lever',
        title: intl.get('lever'),
        render: data => {
          const lever = <span className="yellow">{data.Leverage}X</span>
          return (
            <Row style={{ justifyContent: 'flex-end' }}>
              {lever}
              <Edit
                className="icon"
                onClick={_ => { onLeverClick(data.PosiDirection) }} />
            </Row>
          )
        }
      },
      currentSymbol?.ProductGroup !== ProductGroup.SPOT &&
      {
        key: 'liquidation_price',
        title: intl.get('liquidation_price'),
        render: data => {
          const symbol = formatValue(symbols, data?.InstrumentID, {})
          const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
          return formatDecimal(formatValue(data, 'FORCECLOSEPRICE'), pricePrecision)
        }
      },
      {
        key: 'hold_position_margin',
        title: intl.get('position_margin'),
        render: data => {
          const symbol = formatValue(symbols, data?.InstrumentID, {})
          const otherPrecision = formatValue(symbol, 'OtherPrecision', 0)
          const useMargin = formatDecimal(formatValue(data, 'UseMargin'), otherPrecision)
          if (!data.IsCrossMargin) {
            return (
              <Row
                style={{ justifyContent: 'center' }}>
                <span>{useMargin}</span>
                <Edit
                  className="icon"
                  onClick={_ => {
                    AdjustMarginModal.open({
                      isMobile: false,
                      symbol,
                      position: data,
                      onSuccess: () => {
                        setTimeout(() => { request() }, UPDATE_DELAY)
                      },
                      config: { theme }
                    })
                  }} />
              </Row>
            )
          }
          return useMargin
        }
      },
      {
        key: 'unrealized_profit_loss',
        title: intl.get('unrealized_profit_loss'),
        render: data => {
          const symbol = formatValue(symbols, data.InstrumentID, {})
          const volumeMultiple = formatValue(symbol, 'VolumeMultiple', 0)
          const otherPrecision = formatValue(symbol, 'OtherPrecision', 0)
          const volume = formatValue(data, 'Position', 0)
          const market = formatValue(markets, symbol?.InstrumentID, {})
          const markerPrice = market.MarkedPrice
          const openPrice = data.OpenPrice
          let unrealizedProfitLoss = '--'
          if (markerPrice && openPrice) {
            let difPrice
            if (data.Direction === OrderDirection.BUY) {
              difPrice = markerPrice - openPrice
            } else {
              difPrice = openPrice - markerPrice
            }

            if (symbol.IsInverse) {
              const coinVolume = volume * volumeMultiple / openPrice
              unrealizedProfitLoss = coinVolume * difPrice / markerPrice
            } else {
              const coinVolume = volume * volumeMultiple
              unrealizedProfitLoss = coinVolume * difPrice
            }
          }
          return (
            <div
              className={unrealizedProfitLoss > 0 ? 'buy-rise-color' : (unrealizedProfitLoss < 0 ? 'sell-fail-color' : '')}
              style={{ justifyContent: 'flex-end' }}>
              {formatDecimal(unrealizedProfitLoss, otherPrecision)}
            </div>
          )
        }
      },
      {
        key: 'realized_profit_loss',
        title: intl.get('realized_profit_loss'),
        render: data => {
          const symbol = formatValue(symbols, data?.InstrumentID, {})
          const otherPrecision = formatValue(symbol, 'OtherPrecision', 0)
          return formatDecimal(formatValue(data, 'CloseProfit'), otherPrecision)
        }
      },
      {
        key: 'can_close_volume',
        title: `${intl.get('can_close_volume')}(${volumeUnit})`,
        render: data => {
          const symbol = formatValue(symbols, data?.InstrumentID, {})
          const volumePrecision = isSheetVolume ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0)
          return formatDecimal(
            VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume: formatValue(data, 'ClosePosition'), price: data.OpenPrice, symbol }),
            volumePrecision
          )
        }
      }
    ]
    const c = baseColumns.filter(column => !!column)
    c.push({
      key: 'stop_profit_or_loss_price',
      title: intl.get('stop_profit_loss_price'),
      render: data => {
        const symbol = formatValue(symbols, data?.InstrumentID, {})
        const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
        const tpTriggerPrice = formatDecimal(formatValue(data, 'TPTriggerPrice'), pricePrecision)
        const slTriggerPrice = formatDecimal(formatValue(data, 'SLTriggerPrice'), pricePrecision)
        const disabled = (!data.TPTriggerPrice) && (!data.SLTriggerPrice)
        return (
          <div style={{ justifyContent: 'flex-end' }}>
            <span>{tpTriggerPrice}/{slTriggerPrice}</span>
            <Edit
              className="icon"
              onClick={_ => {
                StopProfitLossModal.open({
                  isMobile: false,
                  from: StopProfitLossModal.FROM_POSITION,
                  positionType,
                  data,
                  symbol,
                  symbolMarket: currentSymbolMarket,
                  onSuccess: () => {
                    setTimeout(() => { request() }, UPDATE_DELAY)
                  },
                  config: { theme }
                })
              }} />
            <Clear
              className="icon"
              disabled={disabled}
              onClick={_ => {
                if (disabled) {
                  return
                }
                revokeStopProfitLoss(data)
              }} />
          </div>
        )
      }
    })
    c.push(
      {
        key: 'operation',
        title: intl.get('operation'),
        width: 276,
        render: data => {
          const symbol = formatValue(symbols, data?.InstrumentID, {})
          const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
          const volumePrecision = isSheetVolume ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0)
          const key = getInputKey(data)
          const input = formatValue(inputs, key, {})
          const price = input.price || ''
          const volume = input.volumeFlag ? (input.volume || '') : (
            isSheetVolume ? data.ClosePosition : VolumeUnitType.sheetToCoin({ volume: data.ClosePosition, price: data.OpenPrice, symbol })
          )
          return (
            <Row
              style={{ justifyContent: 'flex-end' }}>
              <Input
                className="input"
                min={0}
                precision={pricePrecision}
                placeholder={intl.get('close_position_price')}
                value={price}
                onChange={v => { setInputs({ ...inputs, [key]: { ...input, price: v, priceFlag: true } }) }} />
              <Input
                className="input"
                min={0}
                precision={volumePrecision}
                value={volume}
                placeholder={`${intl.get('close_position_volume')}(${volumeUnit})`}
                onChange={v => { setInputs({ ...inputs, [key]: { ...input, volume: v, volumeFlag: true } }) }} />
              <Button
                className="button"
                type="tertiary"
                size="small"
                disabled={!price || !volume}
                onClick={() => { order(data, OffsetFlag.CLOSE, PriceType.LIMIT, price, volume, data.OpenPrice) }}>
                {intl.get('close_position')}
              </Button>
              <Button
                className="button"
                type="tertiary"
                size="small"
                onClick={() => { order(data, OffsetFlag.CLOSE_ALL, PriceType.MARKET, price, volume, data.OpenPrice) }}>
                {intl.get('market_price_close_all')}
              </Button>
            </Row>
          )
        }
      }
    )
    return c
  }, [theme, currentSymbol, volumeUnitType, result, markets, inputs])

  const renderPC = () => {
    return (
      <Table
        className="exchange-data-query-table"
        columns={columns}
        loading={loading}
        dataSource={paginatedData || []}
        pagination={{
          current: currentPage,
          total: totalPages,
          onChange: (page) => {
            setCurrentPage(page)
          },
          itemRender: (page, type, element) => {
            if (type === 'page') {
              return (
                <span className={currentPage === page ? 'active-page' : ''}>
                  {page}
                </span>
              )
            }
            return element
          }
        }}
      />
    )
  }

  const renderMobile = () => {
    return (
      <List
        className="exchange-data-query-list"
        rowClassName="mobile-row"
        loading={loading}
        dataSource={result || []}
        renderRow={data => {
          const isSheetVolume = volumeUnitType === VolumeUnitType.SHEET
          const volumeUnit = isSheetVolume ? intl.get('sheet') : intl.get('coin')
          const symbol = formatValue(symbols, data.InstrumentID, {})
          const volumeMultiple = formatValue(symbol, 'VolumeMultiple', 0)
          const otherPrecision = formatValue(symbol, 'OtherPrecision', 0)
          const pricePrecision = formatValue(symbol, 'PricePrecision', 0)
          const volumePrecision = isSheetVolume ? formatValue(symbol, 'VolumePrecision', 0) : formatValue(symbol, 'CoinPrecision', 0)
          const isBuy = data.Direction === OrderDirection.BUY
          const bgStyleKey = isBuy ? 'buy-rise-border-background-opacity-10' : 'sell-fail-border-background-opacity-10'
          const colorStyleKey = isBuy ? 'buy-rise-color' : 'sell-fail-color'

          const volume = formatValue(data, 'Position', 0)
          const market = formatValue(markets, symbol?.InstrumentID, {})
          const markerPrice = market.MarkedPrice
          const openPrice = data.OpenPrice
          let unrealizedProfitLoss = '--'
          if (markerPrice && openPrice) {
            let difPrice
            if (data.Direction === OrderDirection.BUY) {
              difPrice = markerPrice - openPrice
            } else {
              difPrice = openPrice - markerPrice
            }

            if (symbol.IsInverse) {
              const coinVolume = volume * volumeMultiple / openPrice
              unrealizedProfitLoss = coinVolume * difPrice / markerPrice
            } else {
              const coinVolume = volume * volumeMultiple
              unrealizedProfitLoss = coinVolume * difPrice
            }
          }

          const tpTriggerPrice = formatDecimal(formatValue(data, 'TPTriggerPrice'), pricePrecision)
          const slTriggerPrice = formatDecimal(formatValue(data, 'SLTriggerPrice'), pricePrecision)
          const tpSlDisabled = (!data.TPTriggerPrice) && (!data.SLTriggerPrice)

          const key = getInputKey(data)
          const input = formatValue(inputs, key, {})
          const closePositionPrice = input.price || ''
          const closePositionVolume = input.volumeFlag ? (input.volume || '') : (
            isSheetVolume ? data.ClosePosition : VolumeUnitType.sheetToCoin({ volume: data.ClosePosition, price: openPrice, symbol })
          )

          return (
            <Col style={{ width: '100%' }}>
              <p className="symbol-name">{symbol?.InstrumentID}</p>
              <Row style={{ marginTop: 4 }}>
                <span
                  className={`tag ${bgStyleKey} ${colorStyleKey}`}>
                  {transferDirectionTag(data)}
                </span>
                {
                  symbol?.ProductGroup !== ProductGroup.SPOT &&
                  <span className="position-margin-mode-tag">{MarginMode.transfer(data.IsCrossMargin)}</span>
                }
                <span className="yellow">{data.Leverage}X</span>
                {
                  <Edit
                    className="icon"
                    onClick={_ => { onLeverClick(data.PosiDirection) }} />
                }
              </Row>
              <Row style={{ justifyContent: 'space-between', marginTop: 8 }}>
                <Col style={{ alignItems: 'flex-start' }}>
                  <span className="title">{intl.get('unrealized_profit_loss')}</span>
                  <span
                    className={unrealizedProfitLoss > 0 ? 'buy-rise-color' : (unrealizedProfitLoss < 0 ? 'sell-fail-color' : '')}
                    style={{ justifyContent: 'flex-end' }}>
                    {formatDecimal(unrealizedProfitLoss, otherPrecision)}
                  </span>
                </Col>
                <Col>
                  <span className="title">{intl.get('realized_profit_loss')}</span>
                  <span>{formatDecimal(formatValue(data, 'CloseProfit'), otherPrecision)}</span>
                </Col>
              </Row>
              <Row style={{ marginTop: 6, width: '100%' }}>
                <Col style={{ alignItems: 'flex-start', width: '40%' }}>
                  <span className="title">{intl.get('position_volume')}({volumeUnit})</span>
                  <span>
                    {formatDecimal(
                      VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume, price: data.OpenPrice, symbol }),
                      volumePrecision
                    )}
                  </span>
                </Col>
                <Col style={{ alignItems: 'flex-start', width: '30%' }}>
                  <span className="title">{intl.get('can_close_volume')}({volumeUnit})</span>
                  <span>
                    {formatDecimal(
                      VolumeUnitType.transferVolumeForUnit(volumeUnitType, { volume: formatValue(data, 'ClosePosition'), price: data.OpenPrice, data }),
                      volumePrecision
                    )}
                  </span>
                </Col>
                <Col style={{ alignItems: 'flex-end', width: '30%' }}>
                  <span className="title">{intl.get('position_margin')}({volumeUnit})</span>
                  <Row>
                    <span>
                      {formatDecimal(formatValue(data, 'UseMargin'), otherPrecision)}
                    </span>
                    {
                      !data.IsCrossMargin && (
                        <Edit
                          className="icon"
                          onClick={_ => {
                            AdjustMarginModal.open({
                              isMobile: true,
                              symbol,
                              position: data,
                              onSuccess: () => {
                                setTimeout(() => { request() }, UPDATE_DELAY)
                              },
                              config: { theme }
                            })
                          }} />
                      )
                    }
                  </Row>
                </Col>
              </Row>
              <Row style={{ marginTop: 6, width: '100%' }}>
                <Col style={{ alignItems: 'flex-start', width: '40%' }}>
                  <span className="title">{intl.get('open_position_avp_price')}</span>
                  <span>{formatDecimal(openPrice, pricePrecision)}</span>
                </Col>
                <Col style={{ alignItems: 'flex-start', width: '30%' }}>
                  <span className="title">{intl.get('liquidation_price')}</span>
                  <span>{formatDecimal(formatValue(data, 'FORCECLOSEPRICE'), pricePrecision)}</span>
                </Col>
                <Col style={{ alignItems: 'flex-end', width: '30%' }}>
                  <span className="title">{intl.get('stop_profit_loss_price')}</span>
                  <Row>
                    <span>{tpTriggerPrice}/{slTriggerPrice}</span>
                    <Edit
                      className="icon"
                      onClick={_ => {
                        StopProfitLossModal.open({
                          isMobile: true,
                          from: StopProfitLossModal.FROM_POSITION,
                          positionType,
                          data,
                          symbol,
                          symbolMarket: currentSymbolMarket,
                          onSuccess: () => {
                            setTimeout(() => { request() }, UPDATE_DELAY)
                          },
                          config: { theme }
                        })
                      }} />
                    <Clear
                      className="icon"
                      disabled={tpSlDisabled}
                      onClick={_ => {
                        if (tpSlDisabled) {
                          return
                        }
                        revokeStopProfitLoss(data)
                      }} />
                  </Row>
                </Col>
              </Row>
              <Row style={{ marginTop: 6, width: '100%', alignItems: 'flex-end' }}>
                <Col style={{ width: '40%' }}>
                  <span className="title">{intl.get('price')}</span>
                  <Input
                    style={{ marginLeft: 0 }}
                    className="input"
                    min={0}
                    precision={pricePrecision}
                    placeholder={intl.get('close_position_price')}
                    value={closePositionPrice}
                    onChange={v => { setInputs({ ...inputs, [key]: { ...input, price: v, priceFlag: true } }) }} />
                </Col>
                <Col style={{ width: '30%' }}>
                  <span className="title">{intl.get('volume')}</span>
                  <Input
                    style={{ marginLeft: 0 }}
                    className="input"
                    min={0}
                    precision={volumePrecision}
                    value={closePositionVolume}
                    placeholder={`${intl.get('close_position_volume')}(${volumeUnit})`}
                    onChange={v => { setInputs({ ...inputs, [key]: { ...input, volume: v, volumeFlag: true } }) }} />
                </Col>
                <Row style={{ width: '30%', justifyContent: 'flex-end' }}>
                  <Button
                    className="button"
                    type="tertiary"
                    size="small"
                    disabled={!closePositionPrice || !closePositionVolume}
                    onClick={() => { order(data, OffsetFlag.CLOSE, PriceType.LIMIT, closePositionPrice, closePositionVolume, data.OpenPrice) }}>
                    {intl.get('close_position')}
                  </Button>
                  <Button
                    className="button"
                    type="tertiary"
                    size="small"
                    onClick={() => { order(data, OffsetFlag.CLOSE_ALL, PriceType.MARKET, closePositionPrice, closePositionVolume, data.OpenPrice) }}>
                    {intl.get('market_price_close_all')}
                  </Button>
                </Row>
              </Row>
            </Col>
          )
        }} />
    )
  }

  return visible ? (
    screenSize === ScreenSize.MIN ? renderMobile() : renderPC()
  ) : null
}))
