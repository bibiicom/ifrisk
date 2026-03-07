import React, { useState, useEffect } from 'react'

import Select from '../../component/select'
import Table from '../../component/table'
import Row from '../../component/row'

import intl from '../../component/intl'

import { getSymbolList } from '../../api/symbol'
import { getSettlementRecord } from '../../api/marketInfo'

import { transferName } from '../../constants/symbol'

import {
  formatDecimal,
  formatValue,
  formatUtcTimestampToLocalTime
} from '../../utils/format'

const SettlementRecordContent = ({ productGroup }) => {
  const [symbolList, setSymbolList] = useState([])
  const [symbol, setSymbol] = useState({})
  const [currentPage, setCurrentPage] = useState(1)
  const [loading, setLoading] = useState(true)
  const [result, setResult] = useState([])

  useEffect(() => {
    if (productGroup) {
      const request = async () => {
        try {
          const symbolList = await getSymbolList(productGroup)
          setSymbolList(symbolList)
          setSymbol(symbolList[0] || {})
          setCurrentPage(1)
        } catch (e) {
          setLoading(false)
        }
      }
      request()
    }
  }, [productGroup])

  useEffect(() => {
    if (symbol?.InstrumentID) {
      const request = async () => {
        setLoading(true)
        try {
          const result = await getSettlementRecord({
            InstrumentID: symbol?.InstrumentID,
            // pageIndex: currentPage,
            Limit: 50
          })
          setResult(result || [])
        } catch (e) {} finally { setLoading(false) }
      }
      request()
    }
  }, [symbol?.InstrumentID, currentPage])

  const pricePrecision = formatValue(symbol, 'PricePrecision', 0)

  return (
    <>
      <Row
        className="layout-content-filter-container">
        <span>
          {intl.get('trade_variety')}
        </span>
        <Select
          style={{ width: 150 }}
          className="layout-content-filter-select"
          valueStyle={{ fontSize: 14 }}
          popoverRowStyle={{ fontSize: 14, height: 36 }}
          dataSource={symbolList}
          trigger="hover"
          value={transferName(symbol)}
          renderPopoverRow={data => transferName(data)}
          onSelected={setSymbol}
        />
      </Row>
      <Table
        className="layout-content-table"
        dataSource={result || []}
        loading={loading}
        columns={[
          { key: 'time', title: intl.get('time'), render: data => formatUtcTimestampToLocalTime(formatValue(data, 'SettleTime'), 'YYYY-MM-DD HH:mm') },
          { key: 'price', title: `${intl.get('settlement_price')}(${formatValue(symbol, 'PriceCurrency')})`, render: data => formatDecimal(formatValue(data, 'Value'), pricePrecision) }
        ]}
        // pagination={{ current: result.current_page, total: result.total_page, onChange: setCurrentPage }}
      />
    </>
  )
}

export default SettlementRecordContent
