import React, { memo, useContext } from 'react'

import Modal from '../../../../../component/modal/Modal'

import intl from '../../../../../component/intl'
import ComponentContext from '../../../../../component/context'

import './index.less'

const Indicator = (({
  getContainer, visible,
  mainIndicator, subIndicator,
  onMainIndicatorChange, onSubIndicatorChange, onClose
}) => {
  const { theme } = useContext(ComponentContext)

  const renderRow = (indicators, onIndicatorChange, indicator, key) => {
    return (
      <div
        key={key}
        className="exchange-simple-chart-indicator-row">
        {
          indicators.map(name => (
            <span
              key={name}
              className={`exchange-simple-chart-indicator-row-item ${indicator === name ? 'selected' : ''}`}
              onClick={() => {
                if (indicator === name) {
                  onIndicatorChange('')
                } else {
                  onIndicatorChange(name)
                }
              }}>
              {name}
            </span>
          ))
        }
      </div>
    )
  }

  return (
    <Modal
      className={`exchange-simple-chart-indicator-${theme}`}
      getContainer={getContainer}
      title={intl.get('indicator')}
      onClose={onClose}
      visible={visible}>
      <p style={{ marginTop: -10 }} className="exchange-simple-chart-indicator-title">{intl.get('main_indicator')}</p>
      {renderRow(['MA', 'EMA', 'BOLL', 'SAR'], onMainIndicatorChange, mainIndicator)}
      <p style={{ marginTop: 20 }} className="exchange-simple-chart-indicator-title">{intl.get('sub_indicator')}</p>
      {
        ([
          ['MACD', 'KDJ', 'RSI', 'BIAS', 'BRAR'],
          ['CCI', 'DMI', 'CR', 'PSY', 'DMA'],
          ['TRIX', 'VR', 'WR', 'MTM']
        ]).map((indicators, i) => renderRow(indicators, onSubIndicatorChange, subIndicator, `sub_tech_row_${i}`))
      }
    </Modal>
  )
})

export default memo(Indicator)
