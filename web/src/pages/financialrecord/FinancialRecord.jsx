import React, { useState, useMemo, useEffect } from 'react'

import { useParams } from 'react-router-dom'

import Layout from '../common/component/layout/Layout'
import intl from '../../component/intl'

import authHOC from '../common/hoc/authHOC'
import { SettlementGroup } from '../../constants/symbol'
import FinancialRecordContent from './FinancialRecordContent'

export default authHOC(true, true)(() => {
  const [tab, setTab] = useState()

  const { tab: tabParams } = useParams()

  const tabs = useMemo(() => {
    return SettlementGroup.options('Others')
  }, [])

  useEffect(() => {
    let tab = 'SG1'
    if (tabs.indexOf(tabParams) > -1) {
      tab = tabParams
    }
    setTab(tab)
  }, [tabParams])

  return (
    <section>
      <Layout
        tab={tab}
        tabs={tabs}
        title={intl.get('financial_record')}
        onTabChange={setTab}>
        <FinancialRecordContent key={tab} settlementgroup={tab} />
      </Layout>
    </section>
  )
})
