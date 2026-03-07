import React, { memo, useContext } from 'react'

import Tab from '../../../../component/tab'
import ComponentContext from '../../../../component/context'

import './index.less'

export default memo(({
  style, className = '', tab, tabs, title, onTabChange, children
}) => {
  const { theme } = useContext(ComponentContext)
  return (
    <section
      style={style}
      className={`exchange-section-${theme} ${className}`}>
      {
        (tabs || title) &&
        <div className="exchange-section-title">
          {
            tabs && <Tab tab={tab} dataSource={tabs} onChange={onTabChange}/>
          }
          {title}
        </div>
      }
      {children}
    </section>
  )
})
