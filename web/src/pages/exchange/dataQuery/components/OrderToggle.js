import React, { memo, useMemo } from 'react'

import Toggle from '../../../../component/toggle'

import { Type } from '../../../../constants/order'

const OrderToggle = (({
  toggle, onChange
}) => {
  const dataSource = useMemo(() => {
    return Type.createOptions()
  }, [])

  return (
    <Toggle
      style={{ margin: '8px 0px 8px 16px', alignSelf: 'flex-start' }}
      value={toggle}
      dataSource={dataSource}
      onChange={onChange}/>
  )
})

export default memo(OrderToggle)
