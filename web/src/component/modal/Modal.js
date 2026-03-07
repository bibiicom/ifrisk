import React, { memo } from 'react'

import Popover from '../popover'
import Button from '../button'
import Row from '../row'

import './index.less'

const Modal = (({
  className = '', style, width, height, title, hideCloseIcon, buttons, getContainer, onClose, children
}) => {
  // 使用 CSS 变量版本，自动适配主题
  return (
    <Popover
      className={`funny-modal-auto ${className}`}
      style={style}
      getContainer={getContainer}
      visible={true}>
      <div
        style={{ width, height }}
        className="funny-modal-inner">
        {
          (title || !hideCloseIcon) &&
          <div
            className="funny-modal-inner-title">
            <Row style={{ height: '100%' }}>{title}</Row>
            <svg
              className="funny-modal-inner-title-close-icon"
              viewBox="0 0 12 12"
              onClick={onClose}>
              <path
                fillRule="evenodd"
                clipRule="evenodd"
                d="M0.352252 0.352252C0.571922 0.132583 0.928078 0.132583 1.14775 0.352252L6 5.2045L10.8523 0.352252C11.0719 0.132583 11.4281 0.132583 11.6477 0.352252C11.8674 0.571922 11.8674 0.928078 11.6477 1.14775L6.7955 6L11.6477 10.8523C11.8674 11.0719 11.8674 11.4281 11.6477 11.6477C11.4281 11.8674 11.0719 11.8674 10.8523 11.6477L6 6.7955L1.14775 11.6477C0.928078 11.8674 0.571922 11.8674 0.352252 11.6477C0.132583 11.4281 0.132583 11.0719 0.352252 10.8523L5.2045 6L0.352252 1.14775C0.132583 0.928078 0.132583 0.571922 0.352252 0.352252Z"/>
            </svg>
          </div>
        }
        <div
          className="funny-modal-inner-content">
          {children}
        </div>
        {
          (buttons && buttons.length > 0) &&
          <Row
            className="funny-modal-inner-button-container">
            {
              buttons.map(({ type, children, text, style = {}, onClick }, index) => (
                <Button
                  style={{ paddingLeft: 32, paddingRight: 32, ...style }}
                  key={`funny_modal_button_${index}`}
                  type={type}
                  text={text}
                  onClick={onClick}>
                  {children}
                </Button>
              ))
            }
          </Row>
        }
      </div>
    </Popover>
  )
})

export default memo(Modal)
