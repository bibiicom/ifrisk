import React, { memo, useContext } from 'react'

import Row from '../row'
import ComponentContext from '../context'

import './index.less'

const Toggle = (({ style, className = '', buttonClassName = '', buttonStyle, selectedClassName = '', value, dataSource, onChange }) => {
  const { theme } = useContext(ComponentContext)

  return (
    <Row style={style} className={`funny-toggle-${theme} ${className}`}>
      {
        dataSource.map(({ key, text }) => (
          <button
            key={key}
            style={buttonStyle}
            className={`funny-toggle-button ${buttonClassName} ${value === key ? `selected ${selectedClassName}` : ''} `}
            onClick={() => { onChange && onChange(key) }}>
            {text}
          </button>
        ))
      }
    </Row>
  )
})

export default memo(Toggle)
