import React, { useState, useEffect, useContext, useMemo } from 'react'
import { Link } from 'react-router-dom'

import Table from '../../component/table'
import Row from '../../component/row'
import Col from '../../component/col'
import Tooltip from '../../component/tooltip'
import { Note } from '../../component/icons'

import RootContext from '../rootContext'

import intl from '../../component/intl'

import { getAssets, getWalletAssets } from '../../api/account'
import { getMarkets, getSymbolList } from '../../api/symbol'
import { getPosition } from '../../api/position'

import { Status } from '../../constants/auth'
import { ProductGroup } from '../../constants/symbol'
import { Direction } from '../../constants/order'
import { ConvertedCurrency } from '../../constants/convertedCurrency'

import {
  formatDecimal,
  formatValue
} from '../../utils/format'

import { BLOCKCHAIN_TRANSFER } from '../routes'

const AssetsContent = ({ productGroup }) => {
  const { authStatus, userInfo, rates, config: { convertedCurrency } } = useContext(RootContext)
  const [symbols, setSymbols] = useState({})
  const [markets, setMarkets] = useState({})
  const [convertMarkets, setConvertMarkets] = useState({})

  const [positionList, setPositionList] = useState([])
  const [assetsList, setAssetsList] = useState([])

  const [loading, setLoading] = useState(true)

  useEffect(() => {
    const request = async () => {
      try {
        const markets = await getMarkets(ProductGroup.SPOT)
        // console.log('market', markets)
        const convertMarkets = {}
        if (markets) {
          for (const key in markets) {
            const coin = key.split('/')[0] || ''
            if (coin) {
              convertMarkets[coin] = (markets[key] || {}).LastPrice || 0
            }
          }
        }
        setConvertMarkets(convertMarkets)
      } catch (e) {
      }
    }
    request()
  }, [])

  useEffect(() => {
    if (productGroup) {
      const requestSymbols = async () => {
        try {
          const symbolList = await getSymbolList(productGroup)
          const symbols = {}
          symbolList.forEach(symbol => {
            symbols[symbol?.InstrumentID] = symbol
          })
          setSymbols(symbols)
        } catch (e) {
        }
      }
      const requestMarkets = async () => {
        try {
          const markets = await getMarkets(productGroup)
          setMarkets(markets)
        } catch (e) {
        }
      }
      requestSymbols()
      requestMarkets()
    }
  }, [productGroup])

  useEffect(() => {
    // console.log('product', productGroup)
    if (authStatus === Status.LOGIN && productGroup) {
      const requestPositionList = async () => {
        try {
          const result = await getPosition({
            AccountID: userInfo.MemberID
          })
          setPositionList(result || [])
        } catch (e) {
        }
      }

      const requestAssetList = async () => {
        try {
          const result = await getAssets({
            // SettlementGroup: 'SG1',
            AccountID: userInfo.MemberID
            // MemberID: userInfo.MemberID
            // pageIndex: 1,
            // pageSize: 100
          })
          // console.log('result', result)
          setAssetsList(result || [])
        } catch (e) {
        } finally { setLoading(false) }
      }

      const requestWalletAssets = async () => {
        try {
          const result = await getWalletAssets({
            user_id: userInfo.UserID
          })
          setAssetsList(result || [])
        } catch (e) {
        } finally { setLoading(false) }
      }

      if (productGroup !== ProductGroup.SPOT && productGroup !== ProductGroup.WALLET) {
        requestPositionList()
      }
      if (productGroup === ProductGroup.WALLET) {
        requestWalletAssets()
      } else {
        requestAssetList()
      }
    }
  }, [authStatus, userInfo, productGroup])

  const unrealizedProfitLoss = useMemo(() => {
    const profitLoss = {}
    if (positionList.length > 0) {
      positionList.forEach(position => {
        const symbol = formatValue(symbols, position?.InstrumentID, {})
        const volumeMultiple = formatValue(symbol, 'VolumeMultiple', 0)
        const volume = formatValue(position, 'Position', 0)
        const market = formatValue(markets, symbol?.InstrumentID, {})
        const markerPrice = market.MarkedPrice
        const openPrice = position.OpenPrice
        let pl = 0
        if (markerPrice && openPrice) {
          let difPrice
          if (position.Direction === Direction.BUY) {
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
        profitLoss[symbol.ClearCurrency] = (profitLoss[symbol.ClearCurrency] || 0) + pl
      })
    }
    return profitLoss
  }, [positionList, markets, symbols])

  const totalConvert = useMemo(() => {
    let total = 0
    assetsList.forEach(assets => {
      const currency = assets.Currency || ''
      const convertPrice = ConvertedCurrency.transferPrice(currency.toLowerCase(), convertedCurrency, convertMarkets[currency] || 1, rates)
      if (typeof convertPrice === 'number') {
        total += (assets.Balance || 0) * convertPrice
      }
    })
    return total
  }, [convertedCurrency, rates, convertMarkets, assetsList])

  const columns = useMemo(() => {
    if (productGroup === ProductGroup.SPOT) {
      return [
        {
          key: 'settlement',
          title: intl.get('settlement'),
          style: { width: 80 },
          render: data => data.SettlementGroup
        },
        {
          key: 'currency',
          title: intl.get('currency'),
          style: { width: 80 },
          render: data => data.Currency
        },
        {
          key: 'coin_valuation',
          title: intl.get('coin_valuation') + '(USD)',
          style: { width: 140 },
          render: data => {
            const currency = data.Currency || ''
            const convertPrice = ConvertedCurrency.transferPrice(currency.toLowerCase(), convertedCurrency, convertMarkets[currency] || 1, rates)
            const convertValue = (data.Balance || 0) * convertPrice
            return formatDecimal(convertValue, 2)
          }
        },
        {
          key: 'can_used',
          title: (
            <Row
              style={{ justifyContent: 'flex-end' }}>
              <span>{intl.get('can_used')}</span>
              <Tooltip
                style={{ marginLeft: 4 }}
                tip={intl.get('can_use')}>
                <Note style={{ width: 14, height: 14 }}/>
              </Tooltip>
            </Row>
          ),
          style: { width: 280 },
          render: data => {
            return formatDecimal(formatValue(data, 'Available', 0), 8)
          }
        },
        {
          key: 'freeze',
          title: (
            <Row
              style={{ justifyContent: 'flex-end' }}>
              <span>{intl.get('freeze')}</span>
              <Tooltip
                style={{ marginLeft: 4 }}
                tip={intl.get('L0005022可用下单余额保证金余额开仓保证')}>
                <Note style={{ width: 14, height: 14 }}/>
              </Tooltip>
            </Row>
          ),
          style: { width: 360 },
          render: data => formatDecimal(formatValue(data, 'FrozenMargin', 0), 8)
        }
      ]
    }
    if (productGroup === ProductGroup.WALLET) {
      return [
        {
          key: 'currency',
          title: intl.get('currency'),
          style: { width: 140 },
          render: data => data.currency
        },
        {
          key: 'balance',
          title: (
            <Row
              style={{ justifyContent: 'flex-end' }}>
              <span>{intl.get('static_equity')}</span>
              <Tooltip
                style={{ marginLeft: 4 }}
                tip={intl.get('static_equity')}>
                <Note style={{ width: 14, height: 14 }}/>
              </Tooltip>
            </Row>
          ),
          style: { width: 280 },
          render: data => {
            return formatDecimal(formatValue(data, 'balance', 0), 8)
          }
        },
        {
          key: 'frozen',
          title: (
            <Row
              style={{ justifyContent: 'flex-end' }}>
              <span>{intl.get('freeze')}</span>
              <Tooltip
                style={{ marginLeft: 4 }}
                tip={intl.get('L0005022可用下单余额保证金余额开仓保证')}>
                <Note style={{ width: 14, height: 14 }}/>
              </Tooltip>
            </Row>
          ),
          style: { width: 360 },
          render: data => formatDecimal(formatValue(data, 'frozen', 0), 8)
        }
        // {
        //   key: 'operation',
        //   title: intl.get('operation'),
        //   render: data => (
        //     <Row
        //       style={{ justifyContent: 'flex-end' }}>
        //       <Button
        //         className="button"
        //         type="tertiary"
        //         size="small"
        //         onClick={() => {
        //           let symbol
        //           for (const key in symbols) {
        //             const s = symbols[key]
        //             if (s.ClearCurrency === data.currency) {
        //               symbol = s
        //             }
        //           }
        //           // TransferModal.open(userInfo, symbol, unrealizedProfitLoss[data.Currency] || 0, () => {
        //           //   setTimeout(() => {
        //           //     request()
        //           //   }, UPDATE_DELAY)
        //           // })
        //         }}>
        //         {intl.get('deposit')}
        //       </Button>
        //     </Row>
        //   )
        // }
      ]
    }
    return [
      {
        key: 'currency',
        title: intl.get('currency'),
        style: { width: 120 },
        render: data => data.Currency
      },
      {
        key: 'static_equity',
        title: (
          <Row
            style={{ justifyContent: 'flex-end' }}>
            <span>{intl.get('static_equity')}</span>
            <Tooltip
              style={{ marginLeft: 4 }}
              tip={intl.get('L0005013钱包余额总共净划入总共已实现盈')}>
              <Note style={{ width: 14, height: 14 }}/>
            </Tooltip>
          </Row>
        ),
        style: { width: 150 },
        render: data => formatDecimal(formatValue(data, 'Balance'), 8)
      },
      {
        key: 'can_used',
        title: (
          <Row
            style={{ justifyContent: 'flex-end' }}>
            <span>{intl.get('can_used')}</span>
            <Tooltip
              style={{ marginLeft: 4 }}
              tip={intl.get('can_use')}>
              <Note style={{ width: 14, height: 14 }}/>
            </Tooltip>
          </Row>
        ),
        style: { width: 250 },
        render: data => {
          const available = formatValue(data, 'Available', 0)
          const profitLoss = formatValue(unrealizedProfitLoss, data.Currency, 0)
          return formatDecimal(available + profitLoss, 8)
        }
      },
      {
        key: 'unrealized_profit_loss',
        title: (
          <Row
            style={{ justifyContent: 'flex-end' }}>
            <span>{intl.get('unrealized_profit_loss')}</span>
            <Tooltip
              style={{ marginLeft: 4 }}
              tip={intl.get('L0005022可用下单余额保证金余额开仓保证')}>
              <Note style={{ width: 14, height: 14 }}/>
            </Tooltip>
          </Row>
        ),
        render: data => formatDecimal(formatValue(unrealizedProfitLoss, data.Currency, 0), 8)
      },
      {
        key: 'realized_profit_loss',
        title: (
          <Row
            style={{ justifyContent: 'flex-end' }}>
            <span>{intl.get('realized_profit_loss')}</span>
            <Tooltip
              style={{ marginLeft: 4 }}
              tip={intl.get('L0005022可用下单余额保证金余额开仓保证')}>
              <Note style={{ width: 14, height: 14 }}/>
            </Tooltip>
          </Row>
        ),
        render: data => formatDecimal(formatValue(data, 'CloseProfit', 0), 8)
      }
      // {
      //   key: 'operation',
      //   title: intl.get('operation'),
      //   render: data => (
      //     <Row
      //       style={{ justifyContent: 'flex-end' }}>
      //       <Button
      //         className="button"
      //         type="tertiary"
      //         size="small"
      //         onClick={() => {
      //           let symbol
      //           for (const key in symbols) {
      //             const s = symbols[key]
      //             if (s.ClearCurrency === data.Currency) {
      //               symbol = s
      //             }
      //           }
      //           // TransferModal.open(userInfo, symbol, unrealizedProfitLoss[data.Currency] || 0, () => {
      //           //   setTimeout(() => {
      //           //     request()
      //           //   }, UPDATE_DELAY)
      //           // })
      //         }}>
      //         {intl.get('transfer')}
      //       </Button>
      //     </Row>
      //   )
      // }
    ]
  }, [productGroup, assetsList, unrealizedProfitLoss])

  return (
    <>
      <Col
        className="assets-convert-container">
        <span style={{ fontSize: 16 }}>{intl.get('assets_valuation')}</span>
        <span style={{ fontSize: 24, fontWeight: 'bold', marginTop: 8 }}>
          ≈&nbsp;{formatDecimal(totalConvert, 2)}&nbsp;USD
        </span>
        <Link
          to={{ pathname: BLOCKCHAIN_TRANSFER }}
          className="deposit-link">
          {intl.get('deposit')}
        </Link>
      </Col>
      <Table
        className="layout-content-table"
        dataSource={assetsList}
        loading={loading}
        columns={columns}
      />
    </>
  )
}

export default AssetsContent
