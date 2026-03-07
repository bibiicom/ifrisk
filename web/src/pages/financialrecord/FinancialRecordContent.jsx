import React, { useState, useEffect, useMemo, useContext } from 'react'

import Select from '../../component/select'
import Table from '../../component/table'
import Row from '../../component/row'
import intl from '../../component/intl'
import RootContext from '../rootContext'
import { getCurrency } from '../../api/symbol'
import { getFinancialRecord } from '../../api/account'    
import { Status } from '../../constants/auth'
import { FlowType } from '../../constants/financialRecord'

import {
  formatDecimal,
  formatValue,
  formatUtcTimestampToLocalTime
} from '../../utils/format'

const FinancialRecordContent = ({ settlementgroup }) => {
  const { authStatus } = useContext(RootContext)
  const { userInfo } = useContext(RootContext)
  const [currencyList, setCurrencyList] = useState([])
  const [currency, setCurrency] = useState({})
  const [flowType, setFlowType] = useState(FlowType.ALL)
  const [currentPage, setCurrentPage] = useState(1)
  const [loading, setLoading] = useState(true)
  const [result, setResult] = useState([])

  useEffect(() => {
    console.log(settlementgroup)
    if (settlementgroup) {
      const request = async () => {
        try {
          const currencyList = await getCurrency(settlementgroup)
          setCurrencyList(currencyList)
          setCurrency(currencyList[0] || {})
          setCurrentPage(1)
        } catch (e) {
          setLoading(false)
        }
      }
      request()
    }
  }, [settlementgroup])

  useEffect(() => {
    if (authStatus === Status.LOGIN && currency?.Currency) {
      const request = async () => {
        setLoading(true)
        try {
          const result = await getFinancialRecord({
            SettlementGroup: settlementgroup,
            Currency: currency.Currency,
            AccountID: userInfo?.MemberID,
            Limit: 20
          })
          setResult(result || [])
        } catch (e) {} finally { setLoading(false) }
      }
      request()
    }
  }, [authStatus, currency?.Currency, flowType, currentPage])

  return (
    <>
      <Row
        className="layout-content-filter-container">
        <span>
          {intl.get('currency')}
        </span>
        <Select
          style={{ width: 150 }}
          className="layout-content-filter-select"
          valueStyle={{ fontSize: 14 }}
          popoverRowStyle={{ fontSize: 14, height: 36 }}
          dataSource={currencyList || []}
          trigger="hover"
          value={currency.Currency}
          // renderPopoverRow={data => data.Currency}
          renderPopoverRow={item => item.Currency}
          onSelected={setCurrency}
        />
        <span>{intl.get('flow_type')}</span>
        <Select
          style={{ width: 120 }}
          className="layout-content-filter-select"
          valueStyle={{ fontSize: 14 }}
          popoverRowStyle={{ fontSize: 14, height: 36 }}
          value={FlowType.transfer(flowType)}
          dataSource={FlowType.createSpotOptions()}
          trigger="hover"
          onSelected={data => { setFlowType(data.key) }}/>
      </Row>
      <Table
        className="layout-content-table"
        dataSource={result || []}
        loading={loading}
        columns={[
          {
            key: 'trade_symbol',
            title: intl.get('trade_variety'),
            width: 140,
            render: data => <span className="symbol-name">{data?.InstrumentID}</span>
          },
          {
            key: 'time',
            title: intl.get('time'),
            width: 200,
            render: data => formatUtcTimestampToLocalTime(data.InsertTime, 'YYYY-MM-DD HH:mm:ss')
          },
          {
            key: 'flow_type',
            title: intl.get('flow_type'),
            width: 280,
            render: data => FlowType.transfer(data.Source)
          },
          { key: 'money', title: intl.get('amount'), width: 300, render: data => formatDecimal(formatValue(data, 'Amount'), 8) },
          { key: 'total_money', title: intl.get('total_amount'), render: data => formatDecimal(formatValue(data, 'Balance'), 8) }
        ]}
        // pagination={{ current: result.current_page, total: result.total_page, onChange: setCurrentPage }}
      />
    </>
  )
}

export default FinancialRecordContent
