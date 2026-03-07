import React, { useState, useEffect, useContext } from 'react'

import Modal from '../../../../component/modal'
import Col from '../../../../component/col'
import Radio from '../../../../component/radio'
import Message from '../../../../component/message'
import intl from '../../../../component/intl'
import ComponentContext from '../../../../component/context'

import { Type } from '../../../../constants/position'

import { changePositionType } from '../../../../api/position'

import './index.less'

const Content = ({ positionType, onPositionTypeChange }) => {
  const [innerPositionType, setInnerPositionType] = useState(positionType)
  const { theme } = useContext(ComponentContext)
  useEffect(() => {
    onPositionTypeChange(innerPositionType)
  }, [innerPositionType])

  const className = `exchange-position-type-content-${theme}`

  return (
    <>
      <Col
        className={className}>
        <Radio
          className="radio"
          label={Type.transfer(Type.DOUBLE)}
          checked={innerPositionType === Type.DOUBLE}
          onChecked={checked => { checked && setInnerPositionType(Type.DOUBLE) }}/>
        <p style={{ marginBottom: 24, paddingLeft: 18 }}>{intl.get('two_way_explain')}</p>
        <Radio
          className="radio"
          label={Type.transfer(Type.SINGLE)}
          checked={innerPositionType === Type.SINGLE}
          onChecked={checked => { checked && setInnerPositionType(Type.SINGLE) }}/>
        <p style={{ paddingLeft: 18 }}>{intl.get('single_way_explain')}</p>
      </Col>
      <p className="exchange-position-type-tip">※&nbsp;{intl.get('position_mode_tip')}</p>
    </>
  )
}

function open ({
  isMobile, positionType, onSuccess, config
}) {
  let innerPositionType = positionType
  const request = async () => {
    try {
      await changePositionType({
        PositionType: innerPositionType
      })
      onSuccess && onSuccess(innerPositionType)
      Message.ok({ message: intl.get('switch_position_type_success'), config })
    } catch (e) {
      Message.error({ message: `${intl.get('switch_position_type_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config })
    }
  }

  Modal.open({
    width: isMobile ? '100%' : 0,
    className: isMobile ? 'exchange-modal-mobile' : '',
    title: intl.get('position_mode'),
    content: <Content positionType={innerPositionType} onPositionTypeChange={type => { innerPositionType = type }}/>,
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

const PositionTypeModal = { open }

export default PositionTypeModal
