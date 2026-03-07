import React, { memo } from 'react'

import Modal from '../../../../../component/modal/Modal'
import Select from '../../../../../component/select'
import intl from '../../../../../component/intl'

import './index.less'

const TimeZone = (({
  getContainer, timezone, onTimezoneChange, onClose
}) => {
  return (
    <Modal
      className="exchange-simple-chart-timezone"
      height={260}
      width={340}
      getContainer={getContainer}
      title={intl.get('timezone')}
      onClose={onClose}>
      <Select
        className="exchange-simple-chart-timezone-select"
        selectorClassName="exchange-simple-chart-timezone-select-selector"
        popoverStyle={{ borderRadius: 4 }}
        popoverRowClassName="exchange-simple-chart-timezone-select-row"
        value={timezone.text}
        onSelected={onTimezoneChange}
        dataSource={[
          { key: 'Etc/UTC', text: intl.get('utc'), utc: '' },
          { key: 'Pacific/Honolulu', text: intl.get('honolulu') },
          { key: 'America/Juneau', text: intl.get('juneau') },
          { key: 'America/Los_Angeles', text: intl.get('los_angeles') },
          { key: 'America/Chicago', text: intl.get('chicago') },
          { key: 'America/Toronto', text: intl.get('toronto') },
          { key: 'America/Sao_Paulo', text: intl.get('sao_paulo') },
          { key: 'Europe/London', text: intl.get('london') },
          { key: 'Europe/Berlin', text: intl.get('berlin') },
          { key: 'Asia/Bahrain', text: intl.get('bahrain') },
          { key: 'Asia/Dubai', text: intl.get('dubai') },
          { key: 'Asia/Ashkhabad', text: intl.get('ashkhabad') },
          { key: 'Asia/Almaty', text: intl.get('almaty') },
          { key: 'Asia/Bangkok', text: intl.get('bangkok') },
          { key: 'Asia/Shanghai', text: intl.get('shanghai') },
          { key: 'Asia/Tokyo', text: intl.get('tokyo') },
          { key: 'Australia/Sydney', text: intl.get('sydney') },
          { key: 'Pacific/Norfolk', text: intl.get('norfolk') }
        ]}/>
    </Modal>
  )
})

export default memo(TimeZone)
