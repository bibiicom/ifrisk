import React, { useState, useMemo, useEffect } from 'react'

import { useParams } from 'react-router-dom'

import Layout from '../common/component/layout/Layout'
import intl from '../../component/intl'

import authHOC from '../common/hoc/authHOC'

import AssetsContent from './AssetsContent'

import { SettlementGroup } from '../../constants/symbol'

import './assets.less'

export default authHOC(true)(() => {
  const [tab, setTab] = useState(SettlementGroup.SG1)

  const { tab: tabParams } = useParams()

  const tabs = useMemo(() => {
    return SettlementGroup.createOptions()
  }, [])

  useEffect(() => {
    let tab = 'SG1'
    if (tabs.indexOf(tabParams) > -1) {
      tab = tabParams
    }
    setTab(tab)
  }, [tabParams])

  return (
    <Layout
      tab={tab}
      tabs={tabs}
      title={intl.get('assets')}
      onTabChange={setTab}>
      <AssetsContent key={tab} productGroup={tab}/>
    </Layout>
  )
})
