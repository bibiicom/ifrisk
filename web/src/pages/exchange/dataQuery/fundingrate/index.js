import React, { useState, useContext, useEffect, useImperativeHandle, useMemo, forwardRef, memo } from 'react'

import RootContext from '../../../rootContext'

import Table from '../../../../component/table'
import Row from '../../../../component/row'
import Col from '../../../../component/col'
import List from '../../../../component/list'
import Button from '../../../../component/button'
import Tooltip from '../../../../component/tooltip'
import ComponentContext from '../../../../component/context'
import { Note } from '../../../../component/icons'
import useScreenSize, { ScreenSize } from '../../../../component/hooks/useScreenSize'
import intl from '../../../../component/intl'

import { formatDecimal, formatValue } from '../../../../utils/format'

import ExchangeContext from '../../context.js'
import { useAccountContext } from '../../accountContext.js'

import { Status } from '../../../../constants/auth'
import { Direction as OrderDirection } from '../../../../constants/order'

import { PAGE_SIZE, POLLING_INTERVAL, UPDATE_DELAY } from '../../config'

import { TransferModal } from '../../modal'

import { TOKEN, getStorage, removeStorage } from '../../../../utils/storage'

export default memo(forwardRef((_, ref) => {
  const { authStatus, userInfo = {}, setAuthStatus, setUserInfo } = useContext(RootContext)
  const { theme } = useContext(ComponentContext)
  const { accountResult = [], accountRequest } = useAccountContext()
  const screenSize = useScreenSize()
  const { currentSymbol = {}, symbols = {}, positionList = [], markets } = useContext(ExchangeContext)

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
    return accountResult.slice(startIndex, startIndex + PAGE_SIZE)
  }, [accountResult, currentPage])

  const totalPages = useMemo(() => {
    return Math.ceil(accountResult.length / PAGE_SIZE)
  }, [accountResult])
  useImperativeHandle(ref, () => ({
    updateData: accountRequest
  }))

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
        profitLoss[symbol.ClearCurrency] = (profitLoss[symbol.ClearCurrency] || 0) + pl
      })
    }
    return profitLoss
  }, [positionList, markets])

  const columns = useMemo(() => {
    return [
      {
        key: 'currency',
        title: intl.get('currency'),
        style: { width: 140 },
        render: data => data.Currency
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
              <Note style={{ width: 14, height: 14 }} />
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
              <Note style={{ width: 14, height: 14 }} />
            </Tooltip>
          </Row>
        ),
        style: { width: 360 },
        render: data => formatDecimal(formatValue(data, 'FrozenMoney', 0), 8)
      },
      {
        key: 'operation',
        title: intl.get('operation'),
        render: data => (
          <Row
            style={{ justifyContent: 'flex-end' }}>
            <Button
              className="button"
              type="tertiary"
              size="small"
              onClick={() => {
                let symbol
                for (const key in symbols) {
                  const s = symbols[key]
                  if (s.ClearCurrency === data.Currency) {
                    symbol = s
                  }
                }
                TransferModal.open({
                  isMobile: screenSize === ScreenSize.MIN,
                  userInfo,
                  symbol,
                  profitLoss: unrealizedProfitLoss[data.Currency] || 0,
                  onSuccess: () => {
                    setTimeout(() => {
                      accountRequest()
                    }, UPDATE_DELAY)
                  }
                })
              }}>
              {intl.get('transfer')}
            </Button>
          </Row>
        )
      }
    ]
  }, [accountResult, unrealizedProfitLoss])

  const renderPC = () => {
    return (
      <Table
        className="exchange-data-query-table"
        columns={columns}
        // loading={loading}
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
        // loading={loading}
        dataSource={accountResult || []}
        // renderEmpty={_ => !authStatus === Status.LOGOUT && <Logout />}
        renderRow={data => {
          const profitLoss = formatValue(unrealizedProfitLoss, data.Currency, 0)
          return (
            <Col style={{ width: '100%' }}>
              <p className="symbol-name">{data.Currency}</p>
              <Row style={{ justifyContent: 'space-between', marginTop: 8 }}>
                <Col>
                  <span className="title">{intl.get('static_equity')}</span>
                  <span>{formatDecimal(formatValue(data, 'Balance'), 8)}</span>
                </Col>
                <Col style={{ alignItems: 'flex-end' }}>
                  <span className="title">{intl.get('can_used')}</span>
                  <span>{formatDecimal(formatValue(data, 'Available', 0) + profitLoss, 8)}</span>
                </Col>
              </Row>
              <Row style={{ justifyContent: 'space-between', marginTop: 6 }}>
                <Col>
                  <span className="title">{intl.get('unrealized_profit_loss')}</span>
                  <span>{formatDecimal(profitLoss, 8)}</span>
                </Col>
                <Col style={{ alignItems: 'flex-end' }}>
                  <span className="title">{intl.get('realized_profit_loss')}</span>
                  <span>{formatDecimal(formatValue(data, 'CloseProfit', 0), 8)}</span>
                </Col>
              </Row>
            </Col>
          )
        }}
      />
    )
  }

  return screenSize === ScreenSize.MIN ? renderMobile() : renderPC()
}))
