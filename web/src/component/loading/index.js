import React, { memo } from 'react'

import Row from '../row'

import './index.less'

const Loading =(({ style, className = '' }) => {
  return (
    <Row
      className="funny-loading">
      <div className="loader"/>
    </Row>
  )
})

export default memo(Loading)