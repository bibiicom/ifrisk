import React, { useContext } from 'react'
import intl from '@/component/intl'
import RootContext from '@/pages/rootContext'
import { COLOR_THEME_KEY } from '@/pages/storageConfig'
import { getChangeColor, formatChineseNumber } from '@/utils/format'
import styles from './index.module.less'

// 简单的 SVG 组件作为兜底，如果找不到图片文件
const SvgIcon = ({ type }) => {
    // 基础描边风格，继承颜色
    const commonProps = { width: 16, height: 16, fill: "none", stroke: "currentColor", strokeWidth: "2" }
    
    switch (type) {
        case 'hot':
            return (
                <svg {...commonProps} viewBox="0 0 24 24" strokeLinecap="round" strokeLinejoin="round">
                    {/* 火苗: 底部圆润，顶部尖锐 */}
                    <path d="M12 2a5 5 0 0 0-5 5v2a5 5 0 0 0 10 0V7a5 5 0 0 0-5-5z" fill="currentColor" stroke="none" opacity="0.2"/>
                    <path d="M8.5 14.5A2.5 2.5 0 0 0 11 12c0-1.38-.5-2-1-3-1.072-2.143-.224-4.054 2-6 .5 2.5 2 4.9 4 6.5 2 1.6 3 3.5 3 5.5a7 7 0 1 1-14 0c0-1.1.2-2.2.6-3.3.3.9.8 2.2 2.9 3.8z"></path>
                </svg>
            )
        case 'hold':
            return (
                <svg {...commonProps} viewBox="0 0 24 24" strokeLinecap="round" strokeLinejoin="round">
                    {/* 皇冠: 三个尖角 */}
                    <path d="M5 16L3 5l5.5 5L12 4l3.5 6L21 5l-2 11H5z"></path>
                </svg>
            )
        case 'gain':
            return (
                <svg {...commonProps} viewBox="0 0 24 24" strokeLinecap="round" strokeLinejoin="round">
                    {/* 向上箭头: 更加明确 */}
                    <line x1="12" y1="19" x2="12" y2="5"></line>
                    <polyline points="5 12 12 5 19 12"></polyline>
                </svg>
            )
        case 'loss':
            return (
                <svg {...commonProps} viewBox="0 0 24 24" strokeLinecap="round" strokeLinejoin="round">
                    {/* 向下箭头 */}
                    <line x1="12" y1="5" x2="12" y2="19"></line>
                    <polyline points="19 12 12 19 5 12"></polyline>
                </svg>
            )
        case 'turnover':
            return (
                <svg {...commonProps} viewBox="0 0 24 24" strokeLinecap="round" strokeLinejoin="round">
                    {/* 钱币符号 $ 的简化版或堆叠硬币 */}
                    <path d="M12 1v22"></path>
                    <path d="M17 5H9.5a3.5 3.5 0 0 0 0 7h5a3.5 3.5 0 0 1 0 7H6"></path>
                </svg>
            )
        default:
            return null
    }
}

const StatsCards = ({ marketData = [], holdData = [], activeTab, onTabChange }) => {
    const { config } = useContext(RootContext)
    const colorTheme = config[COLOR_THEME_KEY]

    // 计算统计数据
    const calculateStats = () => {
        if (!marketData || marketData.length === 0) {
            return {
                hotCoin: null,
                holdCoin: null,
                maxGainCoin: null,
                maxLossCoin: null,
                maxTurnoverCoin: null
            }
        }

        // 热门榜: 成交量最大的币种
        const hotCoin = [...marketData].sort((a, b) => b.volume24h - a.volume24h)[0]

        // 持有榜: 接口已排序,直接取第一个
        let holdCoin = null
        if (holdData && holdData.length > 0) {
            holdCoin = holdData[0]
        }

        // 涨幅榜: 24小时涨幅最大的币种
        const maxGainCoin = [...marketData].sort((a, b) => b.change24h - a.change24h)[0]

        // 跌幅榜: 24小时跌幅最大的币种
        const maxLossCoin = [...marketData].sort((a, b) => a.change24h - b.change24h)[0]

        // 成交额榜: 成交额最大的币种
        const maxTurnoverCoin = [...marketData].sort((a, b) => b.turnover24h - a.turnover24h)[0]

        return {
            hotCoin,
            holdCoin,
            maxGainCoin,
            maxLossCoin,
            maxTurnoverCoin
        }
    }

    const stats = calculateStats()

    // 卡片配置数组 - 持有榜放在第一个
    const cardsConfig = [
        {
            key: 'hold',
            title: intl.get('ranking_hold_tab'),
            icon: 'hold',
            type: 'currency', // 币种类型
            data: stats.holdCoin
        },
        {
            key: 'hot',
            title: intl.get('ranking_hot_tab'),
            icon: 'hot',
            type: 'percent', // 百分比类型
            data: stats.hotCoin
        },
        {
            key: 'gain',
            title: intl.get('ranking_gain_tab'),
            icon: 'gain',
            type: 'percent', // 百分比类型
            data: stats.maxGainCoin
        },
        {
            key: 'loss',
            title: intl.get('ranking_loss_tab'),
            icon: 'loss',
            type: 'percent', // 百分比类型
            data: stats.maxLossCoin
        },
        {
            key: 'turnover',
            title: intl.get('ranking_turnover_tab'),
            icon: 'turnover',
            type: 'amount', // 金额类型
            data: stats.maxTurnoverCoin
        }
    ]

    // 根据类型渲染不同的内容
    const renderCardValue = (card) => {
        const { type, data } = card

        // 没有数据时显示 --
        if (!data) {
            return <span className={styles.statsValueNumber}>--</span>
        }

        // 币种类型 (持有榜)
        if (type === 'currency') {
            return (
                <>
                    <div className={styles.coinName}>{data.currency || data.symbol}</div>
                    <span className={styles.statsValueNumber}>
                        {formatChineseNumber(data.holdAmount, 2, '')}
                    </span>
                </>
            )
        }

        // 百分比类型
        if (type === 'percent') {
            return (
                <>
                    <div className={styles.coinName}>{data.symbol}</div>
                    <span
                        className={styles.statsValueNumber}
                        style={{ color: getChangeColor(data.change24h, colorTheme) }}
                    >
                        {data.change24h >= 0 ? '+' : ''}{data.change24h.toFixed(2)}%
                    </span>
                </>
            )
        }

        // 金额类型
        if (type === 'amount') {
            return (
                <>
                    <div className={styles.coinName}>{data.symbol}</div>
                    <span className={styles.statsValueNumber}>
                        {formatChineseNumber(data.turnover24h, 2, '')}
                    </span>
                </>
            )
        }

        return <span className={styles.statsValueNumber}>--</span>
    }

    const handleCardClick = (tab) => {
        if (onTabChange) {
            onTabChange(tab)
        }
    }

    const getCardStyle = (tab) => {
        return activeTab === tab ? `${styles.statsCard} ${styles.active}` : styles.statsCard
    }

    return (
        <div className={styles.statsCards}>
            {cardsConfig.map(card => (
                <div 
                    key={card.key}
                    className={getCardStyle(card.key)} 
                    onClick={() => handleCardClick(card.key)}
                    style={{ cursor: 'pointer' }}
                >
                    <div className={styles.statsCardHeader}>
                        <span className={styles.statsCardTitle}>{card.title}</span>
                        <span className={styles.statsCardIcon}><SvgIcon type={card.icon} /></span>
                    </div>
                    <div className={styles.statsCardValue}>
                        {renderCardValue(card)}
                    </div>
                </div>
            ))}
        </div>
    )
}

export default StatsCards
