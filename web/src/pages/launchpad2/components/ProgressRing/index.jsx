import React from 'react'
import styles from './index.module.less'

/**
 * 进度环组件
 * @param {number} percent - 进度百分比 (可以超过 100)
 * @param {string} size - 尺寸: 'sm' | 'default' | 'lg'
 * @param {string} color - 颜色: 'blue' | 'gray' | 'green'
 * @param {number} strokeWidth - 线条宽度
 */
const ProgressRing = ({
    percent = 0,
    size = 'default',
    color = 'blue',
    strokeWidth = 8,
}) => {
    // 确保百分比不小于 0,但允许超过 100
    const normalizedPercent = Math.max(0, percent)

    // 当进度超过 100% 时,进度环显示为完整闭合(100%),但文字显示实际百分比
    const displayPercent = Math.min(normalizedPercent, 100)

    const viewBoxSize = 100
    const radius = (viewBoxSize - strokeWidth) / 2
    const circumference = radius * 2 * Math.PI
    const strokeDashoffset = circumference - (displayPercent / 100) * circumference

    const sizeClass = {
        sm: styles.sizeSm,
        default: styles.sizeDefault,
        lg: styles.sizeLg,
    }[size]

    const colorClass = {
        blue: styles.colorBlue,
        gray: styles.colorGray,
        green: styles.colorGreen,
    }[color]

    const percentClass = {
        sm: styles.percentSm,
        default: '',
        lg: styles.percentLg,
    }[size]

    return (
        <div className={`${styles.progressRing} ${sizeClass}`}>
            <svg
                viewBox={`0 0 ${viewBoxSize} ${viewBoxSize}`}
                className={styles.svg}
            >
                <circle
                    cx={viewBoxSize / 2}
                    cy={viewBoxSize / 2}
                    r={radius}
                    strokeWidth={strokeWidth}
                    className={styles.bgCircle}
                />
                <circle
                    cx={viewBoxSize / 2}
                    cy={viewBoxSize / 2}
                    r={radius}
                    strokeWidth={strokeWidth}
                    strokeDasharray={circumference}
                    strokeDashoffset={strokeDashoffset}
                    className={`${styles.fillCircle} ${colorClass}`}
                />
            </svg>
            <div className={`${styles.percent} ${percentClass}`}>{normalizedPercent}%</div>
        </div>
    )
}

export default ProgressRing
