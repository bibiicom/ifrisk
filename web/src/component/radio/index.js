import React, { useState, useEffect, memo } from 'react'

import Row from '../row'

import { isValid } from '../utils/typeChecks'

import './index.less'

const Radio = (({
  style, className = '', labelStyle, labelClassName = '', defaultChecked, checked, label, onChecked
}) => {
  const [innerChecked, setInnerChecked] = useState(defaultChecked)

  useEffect(() => {
    if (isValid(checked)) {
      setInnerChecked(checked)
    }
  }, [checked])

  // 使用 CSS 变量版本，自动适配主题
  return (
    <Row
      style={style}
      className={`funny-radio-auto ${className}`}
      onClick={() => {
        const cd = !innerChecked
        if (!isValid(checked)) {
          setInnerChecked(cd)
        }
        onChecked && onChecked(cd)
      }}>
      <svg
        className={`funny-radio-icon ${innerChecked ? 'checked' : ''}`}
        viewBox="0 0 1024 1024">
        <path
          d="M512 85.333333C276.48 85.333333 85.333333 276.48 85.333333 512S276.48 938.666667 512 938.666667 938.666667 747.52 938.666667 512 747.52 85.333333 512 85.333333z m0 796.444445c-203.662222 0-369.777778-166.115556-369.777778-369.777778S308.337778 142.222222 512 142.222222 881.777778 308.337778 881.777778 512 715.662222 881.777778 512 881.777778z"/>
        {
          innerChecked &&
          <path
            d="M512 512m-250.311111 0a250.311111 250.311111 0 1 0 500.622222 0 250.311111 250.311111 0 1 0-500.622222 0Z"/>
        }
      </svg>
      {
        label &&
        <span
          style={labelStyle}
          className={labelClassName}>
          {label}
        </span>
      }
    </Row>
  )
})

export default memo(Radio)
