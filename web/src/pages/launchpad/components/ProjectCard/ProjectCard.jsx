import React, { useMemo } from 'react'
import { useNavigate } from 'react-router-dom'
import { Button, Progress } from 'antd'
import dayjs from 'dayjs'
import { calcSubscriptionProgress } from '@/utils/progress'
import CountdownTimer from '@/component/base/CountdownTimer'
import styles from './ProjectCard.module.less'

// 方案 B：接收 { project }，从后端 Launchpad 数据结构中取字段
const ProjectCard = ({ project, now, onCountdownEnd }) => {
  const navigate = useNavigate()

  // 从 project 中映射需要的字段，后端字段为主，兼容本地字段
  const id = project?.Id 
  const logo = project?.logo || null
  const Currency = project?.Currency || ''
  const description = project?.Summary || project?.description || ''
  const participants = project?.UserCount ?? project?.participants ?? 0

  // 根据 OpenTime、CloseTime、TradeTime 判断当前阶段
  const { phaseStatus, prefixText, targetEndTime } = useMemo(() => {
    const currentNow = now || Date.now()
    const openTime = project?.OpenTime ? dayjs(project.OpenTime).valueOf() : null
    const closeTime = project?.CloseTime ? dayjs(project.CloseTime).valueOf() : null
    const tradeTime = project?.TradeTime ? dayjs(project.TradeTime).valueOf() : null

    // 当前时间 < OpenTime → 预展期
    if (openTime && currentNow < openTime) {
      return {
        phaseStatus: '预展期',
        prefixText: '距离申购开始',
        targetEndTime: project?.OpenTime,
      }
    }
    // OpenTime ≤ 当前时间 < CloseTime → 申购期
    if (closeTime && currentNow < closeTime) {
      return {
        phaseStatus: '申购中',
        prefixText: '申购结束还剩',
        targetEndTime: project?.CloseTime,
      }
    }
    // CloseTime ≤ 当前时间 < TradeTime → 申购已结束
    if (tradeTime && currentNow < tradeTime) {
      return {
        phaseStatus: '申购已结束',
        prefixText: '距离交易开始',
        targetEndTime: project?.TradeTime,
      }
    }
    // 当前时间 ≥ TradeTime → 交易期
    return {
      phaseStatus: '交易中',
      prefixText: '',
      targetEndTime: null,
    }
  }, [project?.OpenTime, project?.CloseTime, project?.TradeTime, now])

  const { buyVolumeStr, allVolumeStr, subscribedPercent } = useMemo(() => {
    const totalStr = project?.RoundVolume
    const remainingStr = project?.StockVolume

    const { percent, bought, total } = calcSubscriptionProgress(totalStr, remainingStr)

    return {
      buyVolumeStr: bought,
      allVolumeStr: total,
      subscribedPercent: percent,
    }
  }, [project?.RoundVolume, project?.StockVolume])
  


  const handleViewDetail = () => {
    if (!id) return
    navigate(`/launchpad/subscription/${id}?Currency=${encodeURIComponent(Currency)}`)
  }

  

  return (
    <div className={styles.card}>
      <div className={styles.header}>
        <div className={styles.status}>
          {phaseStatus === '交易中' ? (
            <span>{phaseStatus}</span>
          ) : (
            <CountdownTimer
              statusLabel={phaseStatus}
              prefixText={prefixText}
              endTime={targetEndTime}
              onEnd={onCountdownEnd}
            />
          )}
        </div>
        <Button
          className={styles.actionBtn}
          type="primary"
          onClick={handleViewDetail}
          disabled={phaseStatus === '预展期'}
        >
          {phaseStatus === '预展期' ? '敬请期待' : phaseStatus === '申购中' ? '参与' : '查看详情'}
        </Button>
      </div>

      <div className={styles.body}>
        <div className={styles.info}>
          <div className={styles.logo}>
            {logo ? (
              <img src={logo} alt={Currency} />
            ) : (
              <div className={styles.logoPlaceholder}>
                {(Currency || '--').slice(0, 2).toUpperCase()}
              </div>
            )}
          </div>
          <div className={styles.details}>
            <h3 className={styles.name}>{Currency}</h3>
            <p className={styles.description}>{description}</p>
          </div>
        </div>

        <div className={styles.stats}>
          <div className={styles.statItem}>
            <div className={styles.statLabel}>参与人数</div>
            <div className={styles.statValue}>{participants}</div>
          </div>
          <div className={styles.statItem}>
            <div className={styles.statLabel}>奖池总额</div>
            <div className={styles.progressWrapper}>
              <Progress
                percent={subscribedPercent}
                showInfo={false}
                strokeColor="#c4ff0d"
                trailColor="rgba(255, 255, 255, 0.12)"
              />
              <div className={styles.progressInfo}>
                <span className={styles.progressText}>
                  已申购 {buyVolumeStr} / {allVolumeStr}
                </span>
                <span className={styles.progressPercent}>{subscribedPercent}%</span>
              </div>
            </div>
          </div>
        </div>

      </div>
    </div>
  )
}

export default ProjectCard
