import React, { memo, useState } from 'react'

import Modal from '../../../../component/modal/Modal'
import Input from '../../../../component/input'
import Select from '../../../../component/select'

import intl from '../../../../component/intl'

import './index.less'

const MorePeriod = (({
  getContainer, onConfirm, onClose
}) => {
  const [value, setValue] = useState('1')
  const [type, setType] = useState({ key: 'minute', text: intl.get('minute') })

  return (
    <Modal
      className="exchange-more-period"
      width={340}
      getContainer={getContainer}
      title={intl.get('more_period')}
      onClose={onClose}
      buttons={[
        {
          children: intl.get('confirm'),
          onClick: () => {
            if (value) {
              let resolution
              let text
              switch (type.key) {
                case 'minute': {
                  resolution = value
                  text = `${value}Min`
                  break
                }
                case 'hour': {
                  resolution = (+value) * 60
                  text = `${value}H`
                  break
                }
                case 'day': {
                  resolution = `${value}D`
                  text = `${value}D`
                  break
                }
              }
              onConfirm({ resolution, text })
            }
            onClose()
          }
        }
      ]}>
      <Input
        className="exchange-more-period-input"
        suffix={
          <Select
            value={type.text}
            dataSource={[
              { key: 'minute', text: intl.get('minute') },
              { key: 'hour', text: intl.get('hour') },
              { key: 'day', text: intl.get('day') }
            ]}
            onSelected={setType}/>
        }
        precision={0}
        min={1}
        value={value}
        onChange={v => { setValue(v) }}/>
    </Modal>
  )
})

export default memo(MorePeriod)
