import React from 'react'
import { Modal, Button } from 'antd'
import styles from './index.module.less'

const ConfirmModal = ({
  visible,
  onCancel,
  onConfirm,
  title = '',
  cancelText = '取消',
  confirmText = '确认',
  loading = false,
  centered = true,
  children,
}) => {
  return (
    <Modal
      title={title}
      open={visible}
      onCancel={onCancel}
      maskClosable={!loading}
      keyboard={!loading}
      closable={!loading}
      footer={null}
      className={styles.confirmModal}
      centered={centered}
    >
      <div className={styles.modalContent}>
        {children}
        <div className={styles.modalActions}>
          <Button onClick={onCancel} disabled={loading}>
            {cancelText}
          </Button>
          <Button type="primary" loading={loading} onClick={onConfirm}>
            {confirmText}
          </Button>
        </div>
      </div>
    </Modal>
  )
}

export default ConfirmModal
