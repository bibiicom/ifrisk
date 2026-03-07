import React from 'react'

import Col from '../../component/col'
import Loading from '../../component/loading'

import './layout.less'

const Layout = ({ loading, children }) => {
  return (
    <Col className="docs-layout">
      {
        loading ? <Loading/> : children
      }
    </Col>
  )
}

export default Layout
