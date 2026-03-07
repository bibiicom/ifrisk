import React, { useState, useEffect, useContext } from 'react'
import { ExclamationCircleOutlined, CheckCircleFilled, CloseCircleFilled } from '@ant-design/icons'
import { Tooltip } from 'antd'
import Decimal from 'decimal.js'
import intl from '@/component/intl'
import ProgressRing from '@/pages/launchpad2/components/ProgressRing'
import RootContext from '@/pages/rootContext'
import { Status } from '@/constants/auth'
import { formatSignificantDigits } from '@/utils/format'
import styles from './index.module.less'

const getStatusFromCode = (statusCode) => {
    switch (statusCode) {
        case 1: return 'live'
        case 2: return 'ended'
        case 0: return 'upcoming'
        default: return 'live'
    }
}

const Sidebar = ({ detail, onApply, onLogin }) => {
    // 获取登录状态
    const { authStatus } = useContext(RootContext)
    const isLoggedIn = authStatus === Status.LOGIN

    // 倒计时状态
    const [countdown, setCountdown] = useState('')

    // 从详情数据中解构出 launchpad 信息和卡片信息
    const { launchpadInfo, cardInfo } = detail || {}

    // 调试日志:查看接口返回的数据
    console.log('Sidebar - launchpadInfo:', launchpadInfo)
    console.log('Sidebar - RoundVolume:', launchpadInfo?.RoundVolume)
    console.log('Sidebar - SubscribeVolume:', launchpadInfo?.SubscribeVolume)

    // === 基础数据提取(使用高精度库) ===
    // 本轮发售总份额
    const roundVolume = new Decimal(launchpadInfo?.RoundVolume || 0)
    // 已购买的数量
    const subscribeVolume = new Decimal(launchpadInfo?.SubscribeVolume || 0)
    // 单价(每份价格)
    const pricePerShare = new Decimal(launchpadInfo?.Price || 0)

    // === 计算数据 ===
    // 申购目标金额 = 单价 × 本轮发售总份额
    const subscriptionGoal = pricePerShare.times(roundVolume).toNumber()
    // 总估值(从卡片信息中获取)
    const totalValuation = new Decimal(cardInfo?.Valuation || 0).toNumber()

    // === 状态数据 ===
    // 活动结束时间
    const closeTime = launchpadInfo?.CloseTime
    // 活动状态(通过状态码转换)
    const status = getStatusFromCode(launchpadInfo?.Status)

    // === 用户申购数据 ===
    // 用户已申购金额(从接口返回的 totalPrice 获取)
    const userSubscriptionAmount = detail?.totalPrice || 0
    // 用户已申购份数(从接口返回的 totalVolume 获取)
    const userSubscriptionShares = detail?.totalVolume || 0

    // === 进度计算 ===
    // 销售进度百分比 = (已购买数量 / 本轮发售总量) × 100 (允许超过 100%)
    const progress = roundVolume.gt(0)
        ? Math.round(subscribeVolume.div(roundVolume).times(100).toNumber())
        : 0

    // 倒计时
    useEffect(() => {
        if (!closeTime) return

        const endTime = new Date(closeTime).getTime()

        const updateCountdown = () => {
            const now = Date.now()
            const diff = endTime - now

            if (diff <= 0) {
                setCountdown('Ended')
                return
            }

            const hours = Math.floor(diff / (1000 * 60 * 60))
            const minutes = Math.floor((diff % (1000 * 60 * 60)) / (1000 * 60))
            const seconds = Math.floor((diff % (1000 * 60)) / 1000)

            setCountdown(`${hours}h : ${minutes.toString().padStart(2, '0')}m : ${seconds.toString().padStart(2, '0')}s`)
        }

        updateCountdown()
        const timer = setInterval(updateCountdown, 1000)
        return () => clearInterval(timer)
    }, [closeTime])

    const isEnded = status === 'ended' || countdown === 'Ended'
    const isUpcoming = status === 'upcoming'

    // 获取状态提示信息（只有用户购买过才显示）
    const getStatusMessage = () => {
        // 判断用户是否有购买记录
        const hasPurchased = userSubscriptionAmount > 0 || userSubscriptionShares > 0
        if (!hasPurchased) {
            return null
        }

        const statusCode = launchpadInfo?.Status
        if (statusCode === 2) {
            return {
                text: intl.get('launchpad2_status_success'),
                type: 'success',
                icon: <CheckCircleFilled />
            }
        }
        if (statusCode === 4) {
            return {
                text: intl.get('launchpad2_status_failed'),
                type: 'failed',
                icon: <CloseCircleFilled />
            }
        }
        return null
    }

    const statusMessage = getStatusMessage()

    return (
        <div className={styles.sidebar}>
            {/* 申购目标卡片 */}
            <div className={styles.goalCard}>
                <div className={styles.goalHeader}>
                    <div className={styles.goalInfo}>
                        <p className={styles.goalLabel}>{intl.get('launchpad2_subscription_goal')}</p>
                        <h2 className={styles.goalValue}>
                            ${subscriptionGoal?.toLocaleString() || '0'}
                        </h2>
                    </div>
                    <ProgressRing percent={progress} size="sm" color="blue" strokeWidth={4} />
                </div>

                <div className={styles.stats}>
                    <StatRow
                        label={intl.get('launchpad2_market_valuation')}
                        value={`${formatSignificantDigits(totalValuation)}`}
                        tooltip={intl.get('launchpad2_market_valuation_tooltip')}
                        highlight
                    />
                    <StatRow
                        label={intl.get('launchpad2_price_per_share')}
                        value={`${formatSignificantDigits(pricePerShare.toNumber())}`}
                    />
                </div>

                {!isLoggedIn && (
                    <button className={styles.subscribeBtn} onClick={onLogin}>
                        {intl.get('login')}
                    </button>
                )}

                {isLoggedIn && isEnded && (
                    <button className={styles.subscribeBtn} disabled>
                        {intl.get('launchpad2_ended_status')}
                    </button>
                )}

                {isLoggedIn && !isEnded && isUpcoming && (
                    <button className={styles.subscribeBtn} disabled>
                        {intl.get('launchpad2_coming_soon')}
                    </button>
                )}

                {isLoggedIn && !isEnded && !isUpcoming && (
                    <button className={styles.subscribeBtn} onClick={onApply}>
                        {intl.get('launchpad2_subscribe')}
                    </button>
                )}
       


                {countdown && status === 'live' && (
                    <p className={styles.countdown}>{intl.get('launchpad2_ends_in')} {countdown}</p>
                )}
            </div>

            {/* 我的申购卡片 */}
            <div className={styles.mySubscription}>
                <p className={styles.myLabel}>{intl.get('launchpad2_my_subscription')}</p>
                <h3 className={styles.myAmount}>
                    ${formatSignificantDigits(userSubscriptionAmount)}
                </h3>
                <div className={styles.myShares}>
                    <span className={styles.sharesDot}></span>
                    <span className={styles.sharesValue}>
                        {formatSignificantDigits(userSubscriptionShares)}
                    </span>
                    <span className={styles.sharesLabel}>{intl.get('launchpad2_shares')}</span>
                </div>
                {statusMessage && (
                    <div className={`${styles.statusMessage} ${styles[statusMessage.type]}`}>
                        <span className={styles.statusIcon}>{statusMessage.icon}</span>
                        <span className={styles.statusText}>{statusMessage.text}</span>
                    </div>
                )}
            </div>
        </div>
    )
}

// 统计行组件
const StatRow = ({ label, value, tooltip, highlight }) => (
    <div className={styles.statRow}>
        <span className={styles.statLabel}>{label}</span>
        <div className={styles.statDivider}></div>
        <div className={styles.statValue}>
            <span className={highlight ? styles.highlight : ''}>{value}</span>
            {tooltip && (
                <Tooltip title={tooltip} placement="top">
                    <span className={styles.infoBtn}>
                        <ExclamationCircleOutlined />
                    </span>
                </Tooltip>
            )}
        </div>
    </div>
)

export default Sidebar
