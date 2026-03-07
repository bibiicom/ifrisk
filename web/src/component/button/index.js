import React, { memo } from 'react'

import Loading from '../loading'

import './index.less'

const Button = (({
  style, className = '', type = 'primary', size = 'medium', loading, text, disabled, children, onClick
}) => {
  // 使用 CSS 变量版本，自动适配主题
  return (
    <button
      type="button"
      style={style}
      className={`funny-button-auto ${className}`}
      disabled={loading || disabled}
      data-type={type}
      data-size={size}
      data-text={text}
      onClick={(e) => { e.target.blur(); onClick && onClick(e) }}>
      {
        loading ? <Loading/> : children
      }
    </button>
  )
})

export default memo(Button)
