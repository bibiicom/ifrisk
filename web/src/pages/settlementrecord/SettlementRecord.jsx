import React, { useState, useMemo, useEffect } from 'react'

import { useParams } from 'react-router-dom'

import Layout from '../common/component/layout/Layout'
import intl from '../../component/intl'

import authHOC from '../common/hoc/authHOC'

import SettlementRecordContent from './SettlementRecordContent'

import { ProductGroup } from '../../constants/symbol'

export default authHOC(true)(() => {
  const [tab, setTab] = useState()

  const { tab: tabParams } = useParams()

  const tabs = useMemo(() => {
    return ProductGroup.createFutureContractOptions()
  }, [])

  useEffect(() => {
    let tab = ProductGroup.POSITIVE_SWAP
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
        title={intl.get('settlement_record')}
        onTabChange={setTab}>
        <SettlementRecordContent key={tab} productGroup={tab} />
      </Layout>
    </section>
  )
})
