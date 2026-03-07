import React, { memo, useRef } from 'react'

import { createPortal } from 'react-dom'

import './index.less'

const Popover = (({
  style, className = '', children, getContainer, visible, onOutsideClick
}) => {
  const rootRef = useRef()

  // 使用 CSS 变量版本，自动适配主题
  const container = (getContainer && getContainer()) || document.body
  return container ? createPortal((
    <div
      ref={rootRef}
      style={style}
      className={`funny-popover-auto ${className}`}
      data-visible={visible}
      onClick={(e) => {
        if (e.target === rootRef.current) {
          onOutsideClick && onOutsideClick()
        }
      }}>
      {children}
    </div>
  ), container) : null
})

export default memo(Popover)
