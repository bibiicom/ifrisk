import React, { forwardRef, memo } from 'react'

import './index.less'

export default memo(forwardRef((props, ref) => {
  return (
    <div
      {...props}
      ref={ref}
      className={`funny-col ${props.className}`}>
      {props.children}
    </div>
  )
}))
