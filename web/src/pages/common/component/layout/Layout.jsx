import React from 'react'

import Col from '../../../../component/col'
import Tab from '../../../../component/tab'

import './layout.less'

const Layout = ({ children, title, tab, tabs, onTabChange }) => {
  return (
    <Col
      className="layout">
      <Col className="layout-title-container">
        <Col
          className="layout-title-content">
          <h1 className="layout-title">{title}</h1>
          <Tab
            style={{ height: 48, overflow: 'auto', whiteSpace: 'nowrap' }}
            tabClassName="layout-tab"
            tabPosition="bottom"
            tab={tab}
            dataSource={tabs}
            onChange={onTabChange}/>
        </Col>
      </Col>
      <Col
        className="layout-content-container">
        {children}
      </Col>
    </Col>
  )
}

export default Layout
