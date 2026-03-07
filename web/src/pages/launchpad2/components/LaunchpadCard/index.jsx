import React, { useState, useEffect, useMemo } from 'react'
import { Button } from 'antd'
import dayjs from 'dayjs'
import utc from 'dayjs/plugin/utc'
import duration from 'dayjs/plugin/duration'
import Decimal from 'decimal.js'
import intl from '@/component/intl'
import ProgressRing from '../ProgressRing'
import { getNftMetadata } from '@/api/nft'
import { formatSignificantDigits } from '@/utils/format'
import styles from './index.module.less'

dayjs.extend(utc)
dayjs.extend(duration)

const LaunchpadCard = ({ project, status }) => {
    // 判断项目是否已结束
    const isEnded = status === 'ended'

    // NFT 图片状态
    const [nftImage, setNftImage] = useState('')
    const [imageLoading, setImageLoading] = useState(false)

    // 使用 useMemo 计算派生值
    const { subscriptionGoal, progressPercent } = useMemo(() => {
        // 本轮发售总份额
        const roundVolume = new Decimal(project.RoundVolume || 0)
        // 已购买的数量
        const subscribeVolume = new Decimal(project.SubscribeVolume || 0)
        // 单价
        const price = new Decimal(project.Price || 0)

        // 申购目标金额 = 总份额 × 每份价格
        const goal = roundVolume.times(price).toNumber()

        // 计算订阅进度百分比 = (已购买份额 / 总份额) × 100 (允许超过 100%)
        const percent = roundVolume.gt(0)
            ? Math.round(subscribeVolume.div(roundVolume).times(100).toNumber())
            : 0

        return {
            subscriptionGoal: goal,
            progressPercent: percent
        }
    }, [project.RoundVolume, project.SubscribeVolume, project.Price])

    // 倒计时状态
    const [timeLeft, setTimeLeft] = useState('')

    // 格式化结束日期为 UTC 时间格式
    const formattedEndDate = dayjs(project.CloseTime).utc().format('YYYY-MM-DD HH:mm') + ' (UTC)'

    // 获取 NFT 图片
    useEffect(() => {
        const fetchNftImage = async () => {
            // 判断是否为 NFT: 有 Contract 和 TokenID
            const contract = project.CardInfo?.Contract
            const tokenId = project.CardInfo?.TokenID

            if (contract && tokenId) {
                setImageLoading(true)
                try {
                    const metadata = await getNftMetadata(contract, tokenId)
                    if (metadata?.image) {
                        setNftImage(metadata.image)
                    }
                } catch (error) {
                    console.error('获取 NFT 图片失败:', error)
                } finally {
                    setImageLoading(false)
                }
            }
        }

        fetchNftImage()
    }, [project.CardInfo?.Contract, project.CardInfo?.TokenID])

    useEffect(() => {
        if (isEnded) return

        const calcTimeLeft = () => {
            const diff = dayjs(project.CloseTime).diff(dayjs())
            if (diff < 0) {
                return '00:00:00'
            }
            const d = dayjs.duration(diff)
            const hours = String(Math.floor(d.asHours())).padStart(2, '0')
            const minutes = String(d.minutes()).padStart(2, '0')
            const seconds = String(d.seconds()).padStart(2, '0')
            return `${hours}:${minutes}:${seconds}`
        }

        setTimeLeft(calcTimeLeft())
        const timer = setInterval(() => {
            setTimeLeft(calcTimeLeft())
        }, 1000)

        return () => clearInterval(timer)
    }, [project.CloseTime, isEnded])

    // 确定最终显示的图片: NFT 图片 > CardInfo.Img > 默认图片
    const displayImage = nftImage || project.CardInfo?.Img || 'https://dyn1.heritagestatic.com/ha?p=3-2-9-4-2-32942949&w=850&h=600&it=product'

    const handleCardClick = () => {
        const currentLang = window.location.pathname.split('/')[1]
        window.open(`${window.location.origin}/${currentLang}/launchpad/${project.Id}`, '_blank')
    }

    const handleParticipate = (e) => {
        e.stopPropagation()
        const currentLang = window.location.pathname.split('/')[1]
        window.open(`${window.location.origin}/${currentLang}/launchpad/${project.Id}`, '_blank')
    }

    return (
        <div
            className={`${styles.card} ${isEnded ? styles.cardEnded : ''}`}
            onClick={handleCardClick}
            style={{ cursor: 'pointer' }}
        >
            {/* 左侧：图片展示区域 */}
            <div className={styles.imageSection}>
                <div
                    className={styles.imageBg}
                    style={{ backgroundImage: `url(${displayImage})` }}
                />
                <img
                    src={displayImage}
                    alt={project.Name}
                    className={styles.image}
                />
            </div>

            {/* 右侧：内容区域 */}
            <div className={styles.contentSection}>
                {/* 上部：标题与订阅目标 */}
                <div>
                    <h3 className={styles.title}>{project.Name}</h3>

                    <div>
                        <p className={styles.goalLabel}>
                            {isEnded ? intl.get('launchpad2_final_subscription') : intl.get('launchpad2_subscription_goal')}
                        </p>

                        <div className={styles.goalRow}>
                            <div className={styles.goalLeft}>
                                <p className={styles.goalAmount}>
                                    ${subscriptionGoal.toLocaleString()}
                                </p>

                                {/* 进度环 */}
                                <ProgressRing
                                    percent={progressPercent}
                                    color={isEnded ? 'gray' : 'blue'}
                                />
                            </div>

                            {/* 状态信息 */}
                            {!isEnded ? (
                                <div className={styles.statusInfo}>
                                    <span className={styles.statusDot} />
                                    <p className={styles.statusText}>
                                        {intl.get('launchpad2_ends_in')} <span className={styles.statusTime}>{timeLeft}</span>
                                    </p>
                                </div>
                            ) : (
                                <div className={styles.closedInfo}>
                                    <p className={styles.closedText}>{intl.get('launchpad2_closed_on')} {formattedEndDate}</p>
                                </div>
                            )}
                        </div>
                    </div>
                </div>

                {/* 下部：详情与操作区 */}
                <div className={styles.footer}>
                    <div className={styles.detailsGrid}>
                        <div className={styles.detailItem}>
                            <span className={styles.detailLabel}>{intl.get('launchpad2_valuation')}</span>
                            <span className={styles.detailValue}>
                                ${new Decimal(project.CardInfo?.Valuation || 0).toNumber().toLocaleString()}
                            </span>
                        </div>
                        <div className={styles.detailItem}>
                            <span className={styles.detailLabel}>{intl.get('launchpad2_price_per_share')}</span>
                            <span className={styles.detailValue}>
                                ${formatSignificantDigits(project.Price)}
                            </span>
                        </div>
                    </div>

                    {!isEnded && (
                        <Button
                            type="primary"
                            className={styles.actionBtn}
                            onClick={handleParticipate}
                        >
                            {intl.get('launchpad2_participate')}
                        </Button>
                    )}
                </div>
            </div>
        </div>
    )
}

export default LaunchpadCard
