import React, { memo } from 'react'

import './index.less'

const Tab = (({ style, className = '', tabClassName = '', tabStyle, tabPosition = 'top', tab, dataSource = [], onChange }) => {
  // 使用 CSS 变量版本，自动适配主题
  return (
    <div
      style={style}
      className={`funny-tab-auto ${className}`}>
      {
        dataSource.map(({ key, text }) => (
          <span
            key={key}
            data-tab-position={tabPosition}
            style={tabStyle}
            className={`${tab === key ? 'selected' : ''} ${tabClassName}`}
            onClick={() => { onChange && onChange(key) }}>
            {text}
          </span>
        ))
      }
    </div>
  )
})

export default memo(Tab)
