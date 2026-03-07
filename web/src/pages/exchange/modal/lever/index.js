import React from 'react'

import Modal from '../../../../component/modal'
import Message from '../../../../component/message'
import intl from '../../../../component/intl'

import LeverSlider from '../../components/leverSlider'

import { Direction } from '../../../../constants/position'

import { adjustLever } from '../../../../api/position'

function open ({
  isMobile, userInfo, symbol, levers, direction, onSuccess, config
}) {
  const leverKey = direction === Direction.SHORT ? 'short' : 'long'
  const maxLeverKey = direction === Direction.SHORT ? 'shortMax' : 'longMax'
  let innerLever = levers[leverKey]

  const request = async _ => {
    if (symbol?.InstrumentID) {
      const id = userInfo.MemberID
      try {
        await adjustLever({
          ExchangeID: symbol.ExchangeID,
          InstrumentID: symbol?.InstrumentID,
          MemberID: id,
          AccountID: id,
          TradeUnitID: id,
          PosiDirection: direction,
          Amount: innerLever
        })
        onSuccess(innerLever)
        Message.ok({ message: intl.get('adjust_lever_success'), config })
      } catch (e) {
        Message.error({ message: `${intl.get('adjust_lever_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config })
      }
    }
  }

  Modal.open({
    className: isMobile ? 'exchange-modal-mobile' : '',
    width: isMobile ? '100%' : 0,
    title: intl.get('adjust_lever'),
    content: <LeverSlider lever={innerLever} maxLever={Math.floor(levers[maxLeverKey])} onChange={l => { innerLever = l }}/>,
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

const LeverModal = { open }

export default LeverModal
