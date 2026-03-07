import React, { useMemo } from 'react'
import styles from './index.module.less'

/**
 * 价格区间条组件
 * 
 * @param {number} lowPrice - 最低价
 * @param {number} highPrice - 最高价
 * @param {number} currentPrice - 当前价格
 * @param {number} width - 组件宽度,默认120px
 */
const PriceRangeBar = ({ lowPrice, highPrice, currentPrice, width = 180 }) => {
    // 计算当前价格在区间中的位置百分比
    const percentage = useMemo(() => {
        if (!lowPrice || !highPrice || !currentPrice) return 50

        const low = parseFloat(lowPrice)
        const high = parseFloat(highPrice)
        const current = parseFloat(currentPrice)

        if (high <= low) return 50

        const position = ((current - low) / (high - low)) * 100
        return Math.max(0, Math.min(100, position))
    }, [lowPrice, highPrice, currentPrice])

    return (
        <div className={styles.container} style={{ width: `${width}px` }}>
            <div className={styles.priceRow}>
                <span className={styles.lowPrice}>{parseFloat(lowPrice).toFixed(2)}</span>
                <span className={styles.highPrice}>{parseFloat(highPrice).toFixed(2)}</span>
            </div>
            <div className={styles.barRow}>
                <div className={styles.priceBar}>
                    <div
                        className={styles.priceIndicator}
                        style={{ left: `${percentage}%` }}
                    >
                        <svg width="10" height="6" viewBox="0 0 10 6" fill="none">
                            <path d="M5 6L0 0H10L5 6Z" fill="currentColor" />
                        </svg>
                    </div>
                </div>
            </div>
        </div>
    )
}

export default PriceRangeBar
