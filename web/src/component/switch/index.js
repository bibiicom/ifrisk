import React, { useState, useEffect, memo } from 'react'

import { isValid } from '../utils/typeChecks'

import './index.less'

const Switch = (({
  defaultOpen, open, onChange
}) => {
  const [innerOpen, setInnerOpen] = useState(defaultOpen)

  useEffect(() => {
    if (isValid(open)) {
      setInnerOpen(open)
    }
  }, [open])

  // 使用 CSS 变量版本，自动适配主题
  return (
    <span
      className={`funny-switch-auto ${innerOpen ? 'open' : ''}`}
      onClick={() => {
        const o = !innerOpen
        setInnerOpen(o)
        onChange && onChange(o)
      }}>
      <i className="funny-switch-thumb"/>
    </span>
  )
})

export default memo(Switch)
