import React, { memo, useContext } from 'react'

import ComponentContext from '../context'

import './index.less'

export default memo(({ style, className = '', tabClassName = '', tabStyle, tabPosition = 'top', tab, dataSource = [], onChange }) => {
  const { theme } = useContext(ComponentContext)
  return (
    <div
      style={{ ...style, border: '1px solid rgba(217,217,217,1)', borderRadius: '8px', padding: '4px 8px', textAlign: 'center' }}
      className={`funny-tab-${theme} ${className}`}>
      {
        dataSource.map(({ key, text }) => (
          <span
            key={key}
            data-tab-position={tabPosition}
            style={{ ...tabStyle }}
            className={`${tab === key ? 'selecteds' : 'noselected'} ${tabClassName}`}
            onClick={() => { onChange && onChange(key) }}>
            {text}
          </span>
        ))
      }
    </div>
  )
})
