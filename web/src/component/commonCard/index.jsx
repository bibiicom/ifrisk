import React from 'react'
import styles from './index.module.less'

/**
 * 公用卡片组件
 * 
 * @param {string} image - 卡片图片URL
 * @param {string} imageAlt - 图片alt文本
 * @param {React.ReactNode} imageOverlay - 图片上的覆盖层内容（如等级标签）
 * @param {React.ReactNode} content - 卡片中间内容区域
 * @param {React.ReactNode} footer - 卡片底部按钮区域
 * @param {string} className - 自定义类名
 * @param {boolean} noHover - 是否禁用hover效果
 * @param {function} onClick - 卡片点击事件
 * @param {object} style - 自定义样式
 */
const CommonCard = ({
    image,
    imageAlt = '',
    imageOverlay,
    content,
    footer,
    className,
    noHover = false,
    onClick,
    style,
}) => {
    const cardClassName = [
        styles.card,
        noHover ? styles.noHover : '',
        className || ''
    ].filter(Boolean).join(' ')

    return (
        <div
            className={cardClassName}
            onClick={onClick}
            style={style}
        >
            {/* 图片区域 */}
            <div className={styles.cardImage}>
                {image ? (
                    <img src={image} alt={imageAlt} />
                ) : (
                    <div className={styles.cardPlaceholder}>No Image</div>
                )}
                {imageOverlay && imageOverlay}
            </div>

            {/* 内容区域 */}
            {content && (
                <div className={styles.cardContent}>
                    {content}
                </div>
            )}

            {/* 底部按钮区域 */}
            {footer && (
                <div className={styles.cardFooter}>
                    {footer}
                </div>
            )}
        </div>
    )
}

// 导出样式供外部使用
CommonCard.styles = styles

export default CommonCard
