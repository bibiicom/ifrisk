import React, { useState, useMemo, useEffect, useRef, useContext } from 'react'

import { useParams } from 'react-router-dom'

import axios from 'axios'

import Layout from '../common/component/layout/Layout'
import intl from '../../component/intl'
import Table from '../../component/table'
import Col from '../../component/col'

import authHOC from '../common/hoc/authHOC'

import Deposit from './Deposit'
import Withdrawal from './Withdrawal'

import { TransferType } from '../../constants/transfer'

import { getBlockchainRecordList } from '../../api/account'
import RootContext from '../rootContext'

import './index.less'

export default authHOC(true, true)(() => {
  const requestCancel = useRef()

  const { userInfo } = useContext(RootContext)

  const [tab, setTab] = useState()
  const { tab: tabParams } = useParams()

  const [currency, setCurrency] = useState()

  const [recordList, setRecordList] = useState([])

  const [loading, setLoading] = useState(true)

  const tabs = useMemo(() => {
    return TransferType.createOptions()
  }, [])

  useEffect(() => {
    const tabs = [
      TransferType.DEPOSIT,
      TransferType.WITHDRAWAL
    ]
    let tab = TransferType.DEPOSIT
    if (tabs.indexOf(tabParams) > -1) {
      tab = tabParams
    }
    setTab(tab)
  }, [tabParams])

  useEffect(() => {
    if (userInfo) {
      const request = async () => {
        requestCancel.current && requestCancel.current({ code: -100 })
        setRecordList([])
        try {
          const result = await getBlockchainRecordList({ user_id: userInfo.UserID, tab, currency }, { cancelToken: new axios.CancelToken(c => { requestCancel.current = c }) })
          setRecordList(result)
        } catch (error) {
        } finally {
          setLoading(false)
        }
      }
      request()
    }
  }, [tab, currency, userInfo])

  const columns = useMemo(() => {
    return [
      {
        key: 'time',
        title: intl.get('time'),
        render: data => data.time
      },
      {
        key: 'volume',
        title: intl.get('amount'),
        render: data => data.amount
      }
    ]
  }, [tab, recordList])

  return (
    <Layout
      tab={tab}
      tabs={tabs}
      title={intl.get('blockchain_transfer')}
      onTabChange={setTab}>
      {tab === TransferType.DEPOSIT ? <Deposit onCurrencyChange={setCurrency}/> : <Withdrawal onCurrencyChange={setCurrency}/>}
      <Col
        className="blockchain-transfer-table-container">
        <p className="table-title">{tab === TransferType.DEPOSIT ? intl.get('deposit_record') : intl.get('withdrawal_record')}</p>
        <Table
          style={{ height: 300, padding: '0 20px' }}
          dataSource={recordList}
          loading={loading}
          columns={columns}/>
      </Col>
    </Layout>
  )
})
