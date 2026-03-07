import React, { useState, useEffect, useCallback } from 'react'
import { useNavigate } from 'react-router-dom'
import { Carousel } from 'antd'
import dayjs from 'dayjs'
import styles from './Banner.module.less'
import { formatDecimals } from '@/utils/number'
import CountdownTimer from '@/component/base/CountdownTimer'
import { getAssets } from '@/api/account'
import { isArray } from '@/component/utils/typeChecks'

const Banner = ({ launchpadList, userInfo, loading, now, onCountdownEnd }) => {

  const [assetInfo, setAssetInfo] = useState({ balance: undefined, available: undefined, frozenMoney: undefined })
  const navigate = useNavigate()

  const handleCountdownEnd = useCallback(() => {
    if (onCountdownEnd) onCountdownEnd()
  }, [onCountdownEnd])

  const getAssetsinfo = async () => {
    try {
      const res = await getAssets({ AccountID: userInfo.MemberID })
   
      const usdtAccount = res.find(item => item.Currency === 'USDT')
      setAssetInfo({
        balance: usdtAccount?.Balance ,
        available: usdtAccount?.Available ,
        frozenMoney: usdtAccount?.FrozenMoney
      })
    } catch (e) {
    console.log('获取失败',e)
    }
  }

  useEffect(() => {
    if (userInfo?.MemberID) {
      getAssetsinfo()
    } else {
      setAssetInfo({ balance: undefined, available: undefined, frozenMoney: undefined })
    }
  }, [userInfo?.MemberID])


  return (
    <div className={styles.banner}>
      <div className={styles.bannerLeft}>
        <h1 className={styles.bannerTitle}>
          立即参与申购<br />
          解锁珍卡所有权
        </h1>
        <div className={styles.statsGrid}>
          <div className={styles.statCard}>
            <div className={styles.statLabel}>账户余额</div>
            <div className={styles.statValue}>
              ${assetInfo.balance ?? '--'}
            </div>
          </div>
          <div className={styles.statCard}>
            <div className={styles.statLabel}>可用资金</div>
            <div className={styles.statValue}>
              ${assetInfo.available ?? '--'}

            </div>
          </div>
          <div className={styles.statCard}>
            <div className={styles.statLabel}>冻结资金</div>
            <div className={styles.statValue}>
              ${assetInfo.frozenMoney ?? '--'}

            </div>
          </div>
        </div>
      </div>

      <div className={styles.bannerRight}>
        {launchpadList && launchpadList.length > 0 ? (
          <Carousel
            autoplay
            autoplaySpeed={5000}
            arrows
            dots={true}
            className={styles.carousel}
          >
            {launchpadList.map(item => {
            // 根据 OpenTime、CloseTime、TradeTime 判断当前阶段
            const currentNow = now || Date.now()
            const openTime = item?.OpenTime ? dayjs(item.OpenTime).valueOf() : null
            const closeTime = item?.CloseTime ? dayjs(item.CloseTime).valueOf() : null
            const tradeTime = item?.TradeTime ? dayjs(item.TradeTime).valueOf() : null

            let phaseStatus = '申购中'
            let prefixText = '申购结束还剩'
            let targetEndTime = item?.CloseTime

            if (openTime && currentNow < openTime) {
              phaseStatus = '预展期'
              prefixText = '距离申购开始'
              targetEndTime = item?.OpenTime
            } else if (closeTime && currentNow < closeTime) {
              phaseStatus = '申购中'
              prefixText = '申购结束还剩'
              targetEndTime = item?.CloseTime
            } else if (tradeTime && currentNow < tradeTime) {
              phaseStatus = '申购已结束'
              prefixText = '距离交易开始'
              targetEndTime = item?.TradeTime
            } else {
              phaseStatus = '交易中'
              prefixText = ''
              targetEndTime = null
            }

            const handleJoin = () => {
              if (!item?.Id || phaseStatus === '预展期') return
              navigate(`/launchpad/subscription/${item.Id}?Currency=${encodeURIComponent(item?.Currency || '')}`)
            }

            return (
              <div key={item.Id}>
                <div className={styles.carouselSlide}>
                  
                  {/* 项目主卡片区域 */}
                  <div className={styles.slideImage}>
                    <div className={styles.projectCard}>
                      <div className={styles.projectBadge}>
                        {(item?.Currency || '--').slice(0, 2).toUpperCase()}
                      </div>
                      <div className={styles.projectCardBody}>
                        <div className={styles.projectPhase}>{phaseStatus}</div>
                        <div className={styles.projectName}>{item?.Summary}</div>
                        <div className={styles.projectMeta}>
                          公募份额：{formatDecimals(item?.RoundVolume)}
                        </div>
                      </div>
                    </div>
                  </div>

                  {/* 底部信息区 */}
                  <div className={styles.slideInfo}>
                    <div className={styles.slideDetails}>
                      {/* 倒计时标签 */}
                      <div className={styles.slideCountdown}>
                        <CountdownTimer
                          statusLabel={phaseStatus}
                          prefixText={prefixText}
                          endTime={targetEndTime}
                          onEnd={handleCountdownEnd}
                        />
                      </div>
                      <h3 className={styles.slideTitle}>{item?.Currency}</h3>
                      <div className={styles.slideReward}>
                        <span className={styles.rewardIcon}>🎁</span>
                        <span>{formatDecimals(item?.RoundVolume)}</span>
                      </div>
                    </div>

                    {/* 参与按钮 - 右下角 */}
                    <button 
                      className={`${styles.slideButton} ${phaseStatus === '预展期' ? styles.disabled : ''}`}
                      disabled={phaseStatus === '预展期'}
                      onClick={handleJoin}
                    >
                      {phaseStatus === '预展期' ? '敬请期待' : '参与'}
                    </button>
                  </div>
                </div>
              </div>
            )
            })}
          </Carousel>
        ) : !loading && (
          <div className={styles.emptyState}>
            <div className={styles.emptyCard}>
              <div className={styles.emptyIcon}>
                <svg width="80" height="80" viewBox="0 0 80 80" fill="none">
                  <circle cx="40" cy="40" r="38" stroke="rgba(255,255,255,0.1)" strokeWidth="2" strokeDasharray="4 4"/>
                  <path d="M40 25v30M25 40h30" stroke="rgba(255,255,255,0.2)" strokeWidth="3" strokeLinecap="round"/>
                </svg>
              </div>
              <h3 className={styles.emptyTitle}>暂无项目</h3>
              <p className={styles.emptyDesc}>当前没有可参与的申购项目</p>
              <div className={styles.emptyHint}>敬请期待即将推出的精彩项目</div>
            </div>
          </div>
        )}
      </div>
    </div>
  )
}

export default Banner
