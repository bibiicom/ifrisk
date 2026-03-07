import React from 'react'
import { Empty } from 'antd'
import intl from '../intl'
import styles from './index.module.less'

/**
 * 公共空状态组件
 * @param {string} description - 描述文字，默认为"请先连接钱包"
 * @param {React.ReactNode} image - 自定义图片
 * @param {React.ReactNode} children - 额外内容
 */
const EmptyState = ({ description, image, children }) => {
    return (
        <div className={styles.container}>
            <Empty
                image={image || Empty.PRESENTED_IMAGE_DEFAULT}
                description={description || intl.get('empty_connect_wallet_prompt')}
            />
            {children}
        </div>
    )
}

export default EmptyState
