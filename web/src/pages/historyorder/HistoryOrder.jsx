import React, { useState, useMemo, useEffect } from 'react'

import { useParams } from 'react-router-dom'

import Layout from '../common/component/layout/Layout'
import intl from '../../component/intl'

import authHOC from '../common/hoc/authHOC'

import HistoryOrderContent from './HistoryOrderContent'

import { ProductGroup } from '../../constants/symbol'

export default authHOC(true, true)(() => {
  const [tab, setTab] = useState()

  const { tab: tabParams } = useParams()

  const tabs = useMemo(() => {
    return ProductGroup.createTradeOptions()
  }, [])


console.log('tabs',tabs)
console.log(tab)

  useEffect(() => {
    const tabs = ProductGroup.tabAll()
    let tab = ProductGroup.SPOT
    if (tabs.indexOf(tabParams) > -1) {
      tab = tabParams
    }
    setTab(tab)
  }, [tabParams])

  // console.log('tabs', tabs)

  return (
    <section>
      <Layout
        tab={tab}
        tabs={tabs}
        title={intl.get('history_order')}
        onTabChange={setTab}>
        <HistoryOrderContent key={tab} productGroup={tab} />
      </Layout>
    </section>
  )
})
