import React, { useState, useContext } from 'react'

import Modal from '../../../../component/modal'
import Toggle from '../../../../component/toggle'
import Message from '../../../../component/message'
import intl from '../../../../component/intl'
import ComponentContext from '../../../../component/context'

import { MarginMode } from '../../../../constants/position'

import { changePositionMarginMode } from '../../../../api/position'

import './index.less'

const Content = ({ marginMode, onMarginModeChange }) => {
  const [innerMarginMode, setInnerMarginMode] = useState(marginMode)
  const { theme } = useContext(ComponentContext)
  return (
    <>
      <Toggle
        value={innerMarginMode}
        buttonStyle={{ width: '50%' }}
        dataSource={[
          { key: MarginMode.CROSS, text: MarginMode.transfer(MarginMode.CROSS) },
          { key: MarginMode.ISOLATED, text: MarginMode.transfer(MarginMode.ISOLATED) }
        ]}
        onChange={(mode) => { setInnerMarginMode(mode); onMarginModeChange(mode) }}/>
      <p className="exchange-margin-mode-title">
        ※&nbsp;{innerMarginMode === MarginMode.ISOLATED ? intl.get('warehouse_by_warehouse_mode') : intl.get('full_warehouse_mode')}
      </p>
      <p className={`exchange-margin-mode-desc-${theme}`}>
        {innerMarginMode === MarginMode.ISOLATED ? intl.get('warehouse_by_warehouse_explain') : intl.get('full_warehouse_explain')}
      </p>
    </>
  )
}

function open ({
  isMobile, symbol, marginMode, onSuccess, config
}) {
  let innerMarginMode = marginMode
  const request = async () => {
    if (symbol?.InstrumentID) {
      try {
        await changePositionMarginMode({
          ExchangeID: symbol.ExchangeID,
          InstrumentID: symbol?.InstrumentID,
          Amount: innerMarginMode
        })
        onSuccess && onSuccess(innerMarginMode)
        Message.ok({ message: intl.get('switch_margin_mode_success'), config })
      } catch (e) {
        Message.error({ message: `${intl.get('switch_margin_mode_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config })
      }
    }
  }

  Modal.open({
    width: isMobile ? '100%' : 0,
    className: isMobile ? 'exchange-modal-mobile' : '',
    title: intl.get('margin_mode'),
    content: <Content marginMode={innerMarginMode} onMarginModeChange={mode => { innerMarginMode = mode }}/>,
    buttons: [
      {
        children: intl.get('cancel'),
        type: 'tertiary',
        text: true
      },
      {
        children: intl.get('confirm'),
        onClick: () => { request() }
      }
    ],
    config
  })
}

const MarginModeModal = { open }

export default MarginModeModal
