import React from 'react'
import { Modal } from 'antd'
import styles from './index.module.less'

/**
 * 公用弹框组件
 * 统一项目中弹框的样式风格
 * 
 * @param {boolean} open - 是否显示弹框
 * @param {string} title - 弹框标题
 * @param {function} onCancel - 关闭弹框回调
 * @param {number} width - 弹框宽度，默认500
 * @param {React.ReactNode} children - 弹框内容
 * @param {React.ReactNode} footer - 自定义底部，传null则不显示底部
 * @param {string} bodyHeight - 弹框内容区域高度
 * @param {object} ...rest - 其他antd Modal属性
 */
const CommonModal = ({
    open,
    title,
    onCancel,
    width = 500,
    children,
    footer,
    bodyHeight,
    className,
    ...rest
}) => {
    const modalClassName = className ? `${styles.modal} ${className}` : styles.modal

    return (
        <Modal
            open={open}
            title={title}
            onCancel={onCancel}
            footer={footer}
            width={width}
            className={modalClassName}
            centered
            style={bodyHeight ? { '--modal-body-height': bodyHeight } : undefined}
            {...rest}
        >
            {children}
        </Modal>
    )
}

// 导出样式供外部使用
CommonModal.styles = styles

export default CommonModal
