import React from 'react'
import { Button } from 'antd'
import { useNavigate } from 'react-router-dom'
import dayjs from 'dayjs'
import styles from './FinishedProjectCard.module.less'
import { formatDecimals } from '@/utils/number'

const FinishedProjectCard = ({ project }) => {
  const navigate = useNavigate()

  const instrumentId = project?.InstrumentID || ''
  const currency= project?.Currency
  const id = project?.Id

  const handleGoTrade = (e) => {
    // 避免点击按钮时同时触发卡片的详情跳转
    if (e) {
      e.stopPropagation()
    }
    const productGroup = 'SPOT'
    navigate(`/exchange?ProductGroup=${productGroup}&InstrumentID=${instrumentId}`)
  }

  const handleViewDetail = () => {
    if (!id) return
    navigate(`/launchpad/subscription/${id}?Currency=${encodeURIComponent(instrumentId || '')}`)
  }

  return (
    <div className={styles.card} onClick={handleViewDetail}>
      <div className={styles.header}>
        <div className={styles.date}>
          已结束：
          {project?.CloseTime ? dayjs(project.CloseTime).format('YYYY/MM/DD HH:mm:ss') : '--'}
        </div>
      </div>

      <div className={styles.body}>
        <div className={styles.logoRow}>
          <div className={styles.logo}>
            {project?.logo ? (
              <img src={project.logo} alt={project?.name || currency || 'project'} />
            ) : (
              <div className={styles.logoPlaceholder}>
                {(currency || '--').slice(0, 2).toUpperCase()}
              </div>
            )}
          </div>
         { project?.Status ===2 &&
           <Button
            className={styles.tradeBtn}
            type="primary"
            onClick={handleGoTrade}
          >
            去交易
          </Button>
         }
        </div>
        <h3 className={styles.name}>{instrumentId || '--'}</h3>

        <div className={styles.stats}>
          <div className={styles.stat}>
            <span className={styles.statIcon}>👥</span>
            <span className={styles.statText}>{project?.UserCount ?? '--'}</span>
          </div>
          <div className={styles.stat}>
            <span className={styles.statIcon}>🎁</span>
            <span className={styles.statText}>{formatDecimals(project?.RoundVolume)}</span>
          </div>
        </div>
      </div>
    </div>
  )
}

export default FinishedProjectCard
