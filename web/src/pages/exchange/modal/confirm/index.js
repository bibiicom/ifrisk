import React from 'react'

import Modal from '../../../../component/modal'
import intl from '../../../../component/intl'

function open ({ isMobile, title, content, onConfirm, onCancel, config }) {
  Modal.confirm({
    width: isMobile ? '86%' : 0,
    title,
    content: <span style={{ padding: '0 12px' }}>{content}</span>,
    confirmButton: {
      children: intl.get('confirm'),
      onClick: () => { onConfirm && onConfirm() }
    },
    cancelButton: {
      children: intl.get('cancel'),
      onClick: onCancel
    },
    config
  })
}

const ConfirmModal = {
  open
}

export default ConfirmModal
