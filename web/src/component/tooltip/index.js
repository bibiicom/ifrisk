import React, { useRef } from 'react'

import Row from '../row'
import Popover from '../popover'

import './index.less'

const Tooltip = ({ style, className = '', children, tip }) => {
  const ref = useRef()
  // 使用 CSS 变量版本，自动适配主题
  return (
    <Row
      ref={ref}
      style={style}
      className={`funny-tooltip-auto ${className}`}>
      <Popover
        className="funny-tooltip-popover"
        visible={true}
        getContainer={_ => ref.current}>
        <Row
          className="funny-tooltip-popover-tip">
          {tip}
        </Row>
        <i className="funny-tooltip-popover-arrow"/>
      </Popover>
      {children}
    </Row>
  )
}

export default Tooltip
