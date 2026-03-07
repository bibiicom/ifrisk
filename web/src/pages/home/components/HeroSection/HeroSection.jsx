import React, { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import intl from '../../../../component/intl'
import GlassCard from '../GlassCard/GlassCard'
import GlassButton from '../../../../component/glass-button'
import styles from './HeroSection.module.less'

const HeroSection = () => {
  const [isMobile, setIsMobile] = useState(false)

  useEffect(() => {
    const checkMobile = () => {
      setIsMobile(window.innerWidth <= 1024)
    }
    checkMobile()
    window.addEventListener('resize', checkMobile)
    return () => window.removeEventListener('resize', checkMobile)
  }, [])

  const containerVariants = {
    hidden: { opacity: 0 },
    visible: {
      opacity: 1,
      transition: {
        staggerChildren: 0.15,
        delayChildren: 0.2
      }
    }
  }

  const itemVariants = {
    hidden: { opacity: 0, y: 30 },
    visible: {
      opacity: 1,
      y: 0,
      transition: {
        duration: 0.6,
        ease: [0.4, 0, 0.2, 1]
      }
    }
  }

  const floatVariants = {
    float: {
      y: [-10, 10, -10],
      transition: {
        duration: 4,
        repeat: Infinity,
        ease: 'easeInOut'
      }
    }
  }

  return (
    <motion.section
      className={styles.heroSection}
      variants={containerVariants}
      initial="hidden"
      animate="visible"
    >
      <div className={styles.container}>
        <div className={styles.content}>
          <motion.div variants={itemVariants} className={styles.badge}>
            <span className={styles.badgeText}>{intl.get('home_new_upgrade')}</span>
          </motion.div>

          <motion.h1 variants={itemVariants} className={styles.title}>
          <span className={styles.titleGradient}>scardex</span>
          <br />
          <span className={styles.titleSubtitle}>{intl.get('home_title')}</span>
        </motion.h1>

          <motion.p variants={itemVariants} className={styles.description}>
            {intl.get('home_subtitle')}
            <br />
            {intl.get('home_description')}
          </motion.p>

          <motion.div variants={itemVariants} className={styles.buttons}>
            <GlassButton
              type="primary"
              to="/exchange"
            >
              <span>{intl.get('home_start_trading')}</span>
              <svg className={styles.buttonIcon} viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <path d="M5 12h14M12 5l7 7-7 7" strokeLinecap="round" strokeLinejoin="round"/>
              </svg>
            </GlassButton>
            <GlassButton
              type="secondary"
              to="/launchpad"
            >
              <span>{intl.get('home_learn_launchpad')}</span>
            </GlassButton>
          </motion.div>

          <motion.div variants={itemVariants} className={styles.stats}>
            <div className={styles.statItem}>
              <div className={styles.statLabel}>
                <span>{intl.get('home_encryption_tech')}</span>
                <div className={styles.statCheck}>
                  <span className={styles.checkmark}>✓</span>
                </div>
              </div>
            </div>
            <div className={styles.statDivider} />
            <div className={styles.statItem}>
              <div className={styles.statLabel}>
                <span>{intl.get('home_blockchain_auth')}</span>
                <div className={styles.statCheck}>
                  <span className={styles.checkmark}>✓</span>
                </div>
              </div>
            </div>
            <div className={styles.statDivider} />
            <div className={styles.statItem}>
              <div className={styles.statLabel}>
                <span>{intl.get('home_authority_guarantee')}</span>
                <div className={styles.statCheck}>
                  <span className={styles.checkmark}>✓</span>
                </div>
              </div>
            </div>
          </motion.div>
        </div>

        {!isMobile && (
        <motion.div
          variants={floatVariants}
          className={styles.visual}
           animate="float"
        >
          <div className={styles.centerAnchor}>
            <div className={styles.centerPoint} >
              <GlassCard variant="neon" className={`${styles.featureCard} ${styles.primaryCard}`}>
              <div className={styles.cardContent}>
                <div className={styles.cardIcon}>
                  <svg viewBox="0 0 48 48" fill="none">
                  {/* 背景光晕 */}
                  <circle cx="24" cy="24" r="24" fill="url(#grad1_outer)" opacity="0.3" filter="drop-shadow(0 0 8px rgba(22, 119, 255, 0.5))"/>
                  {/* 主体圆形 */}
                  <circle cx="24" cy="24" r="22" fill="url(#grad1_inner)" stroke="url(#grad1_stroke)" strokeWidth="2"/>
                  {/* 圆形阴影 */}
                  <circle cx="24" cy="24" r="22" fill="none" stroke="rgba(0, 0, 0, 0.4)" strokeWidth="1" filter="blur(3px)" transform="translate(0, 2)" opacity="0.7"/>

                  {/* 申购卡片主体 */}
                  <rect x="10" y="14" width="28" height="18" rx="4" fill="url(#card_gradient)" stroke="url(#card_stroke)" strokeWidth="2" filter="drop-shadow(0 4px 10px rgba(0, 0, 0, 0.4))"/>
                  {/* 卡片内发光效果 */}
                  <rect x="10" y="14" width="28" height="18" rx="4" fill="none" stroke="rgba(255, 255, 255, 0.3)" strokeWidth="1" filter="blur(2px)"/>

                  {/* 卡片顶部装饰 */}
                  <rect x="14" y="16" width="20" height="2" rx="1" fill="url(#decor_gradient)"/>

                  {/* 卡片标题区域 */}




                  <rect x="25" y="29" width="9" height="2.5" rx="1" fill="rgba(255, 255, 255, 0.2)"/>





                  {/* 卡片高光 */}
                  <rect x="12" y="16" width="4" height="12" rx="2" fill="rgba(255, 255, 255, 0.3)" opacity="0.6"/>

                  <defs>
                    <linearGradient id="grad1_outer" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="#1677ff"/>
                      <stop offset="100%" stopColor="#8b5cf6"/>
                    </linearGradient>
                    <linearGradient id="grad1_inner" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(22, 119, 255, 0.15)"/>
                      <stop offset="100%" stopColor="rgba(139, 92, 246, 0.15)"/>
                    </linearGradient>
                    <linearGradient id="grad1_stroke" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="#1677ff"/>
                      <stop offset="100%" stopColor="#8b5cf6"/>
                    </linearGradient>
                    <linearGradient id="card_gradient" x1="0%" y1="0%" x2="0%" y2="100%">
                      <stop offset="0%" stopColor="rgba(255, 255, 255, 0.35)"/>
                      <stop offset="100%" stopColor="rgba(255, 255, 255, 0.08)"/>
                    </linearGradient>
                    <linearGradient id="card_stroke" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(255, 255, 255, 0.8)"/>
                      <stop offset="100%" stopColor="rgba(255, 255, 255, 0.4)"/>
                    </linearGradient>
                    <linearGradient id="decor_gradient" x1="0%" y1="0%" x2="100%" y2="0%">
                      <stop offset="0%" stopColor="#1677ff"/>
                      <stop offset="50%" stopColor="#8b5cf6"/>
                      <stop offset="100%" stopColor="#1677ff"/>
                    </linearGradient>
                    <linearGradient id="progress_gradient" x1="0%" y1="0%" x2="100%" y2="0%">
                      <stop offset="0%" stopColor="#ffd700"/>
                      <stop offset="100%" stopColor="#ff4500"/>
                    </linearGradient>
                    <linearGradient id="button_gradient" x1="0%" y1="0%" x2="0%" y2="100%">
                      <stop offset="0%" stopColor="#4ade80"/>
                      <stop offset="100%" stopColor="#22c55e"/>
                    </linearGradient>
                    <linearGradient id="button_stroke" x1="0%" y1="0%" x2="0%" y2="100%">
                      <stop offset="0%" stopColor="rgba(255, 255, 255, 0.6)"/>
                      <stop offset="100%" stopColor="rgba(255, 255, 255, 0.3)"/>
                    </linearGradient>
                  </defs>
                  </svg>
                </div>
                <div className={styles.cardTitle}>{intl.get('home_launchpad')}</div>
                <div className={styles.cardDesc}>{intl.get('home_launchpad_desc')}</div>
                <div className={styles.cardStatus}>
                  <span className={styles.statusDot} />
                  <span>{intl.get('home_in_progress')}</span>
                </div>
              </div>
            </GlassCard>

            <GlassCard variant="purple" className={`${styles.featureCard} ${styles.secondaryCard}`}>
              <div className={styles.cardContent}>
                <div className={styles.cardIcon}>
                  <svg viewBox="0 0 48 48" fill="none">
                  {/* 背景光晕 */}
                  <circle cx="24" cy="24" r="24" fill="url(#grad2_outer)" opacity="0.3" filter="drop-shadow(0 0 8px rgba(139, 92, 246, 0.5))"/>
                  {/* 主体圆形 */}
                  <circle cx="24" cy="24" r="22" fill="url(#grad2_inner)" stroke="url(#grad2_stroke)" strokeWidth="2"/>
                  {/* 圆形阴影 */}
                  <circle cx="24" cy="24" r="22" fill="none" stroke="rgba(0, 0, 0, 0.4)" strokeWidth="1" filter="blur(3px)" transform="translate(0, 2)" opacity="0.7"/>

                  {/* 立方体主体 - 前面 */}
                  <path d="M24 8L36 16V32L24 40L12 32V16L24 8Z" fill="url(#cube_front_gradient)" stroke="url(#cube_front_stroke)" strokeWidth="2" filter="drop-shadow(0 4px 12px rgba(0, 0, 0, 0.4))"/>

                  {/* 立方体主体 - 右侧 */}
                  <path d="M36 16L44 20V36L36 40V16Z" fill="url(#cube_right_gradient)" stroke="url(#cube_right_stroke)" strokeWidth="2" opacity="0.8"/>

                  {/* 立方体主体 - 左侧 */}
                  <path d="M12 16L4 20V36L12 40V16Z" fill="url(#cube_left_gradient)" stroke="url(#cube_left_stroke)" strokeWidth="2" opacity="0.8"/>

                  {/* 立方体内部线条 */}
                  <path d="M24 16V32" stroke="url(#cube_lines)" strokeWidth="2" strokeLinecap="round"/>
                  <path d="M24 16L36 20" stroke="url(#cube_lines)" strokeWidth="2" strokeLinecap="round"/>
                  <path d="M24 16L12 20" stroke="url(#cube_lines)" strokeWidth="2" strokeLinecap="round"/>
                  <path d="M24 32L36 36" stroke="url(#cube_lines)" strokeWidth="2" strokeLinecap="round"/>
                  <path d="M24 32L12 36" stroke="url(#cube_lines)" strokeWidth="2" strokeLinecap="round"/>

                  {/* NFT卡片细节 - 前面 */}
                  <rect x="15" y="20" width="18" height="14" rx="2" fill="url(#nft_card_gradient)" stroke="rgba(255, 255, 255, 0.6)" strokeWidth="1.5"/>
                  <rect x="17" y="22" width="14" height="2" rx="1" fill="rgba(255, 255, 255, 0.3)"/>
                  <rect x="17" y="26" width="6" height="6" rx="1" fill="url(#nft_image_gradient)"/>
                  <rect x="25" y="26" width="6" height="3" rx="1" fill="rgba(255, 255, 255, 0.2)"/>
                  <rect x="25" y="31" width="6" height="3" rx="1" fill="rgba(255, 255, 255, 0.2)"/>

                  {/* NFT卡片细节 - 右侧 */}
                  <rect x="36.5" y="22" width="4" height="12" rx="1" fill="url(#nft_card_right_gradient)" stroke="rgba(255, 255, 255, 0.4)" strokeWidth="1" opacity="0.9"/>

                  {/* 发光效果 */}
                  <path d="M24 8L36 16L24 24L12 16L24 8Z" fill="none" stroke="rgba(255, 255, 255, 0.4)" strokeWidth="1" filter="blur(2px)"/>
                  <rect x="15" y="20" width="18" height="14" rx="2" fill="none" stroke="rgba(255, 255, 255, 0.3)" strokeWidth="1" filter="blur(2px)"/>

                  {/* 高光效果 */}
                  <circle cx="32" cy="22" r="3" fill="url(#grad2_highlight)" opacity="0.7"/>
                  <circle cx="28" cy="14" r="2" fill="url(#grad2_sparkle)" opacity="0.6"/>
                  <circle cx="18" cy="38" r="1.5" fill="url(#grad2_sparkle)" opacity="0.5"/>

                  <defs>
                    <linearGradient id="grad2_outer" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="#8b5cf6"/>
                      <stop offset="100%" stopColor="#ec4899"/>
                    </linearGradient>
                    <linearGradient id="grad2_inner" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(139, 92, 246, 0.15)"/>
                      <stop offset="100%" stopColor="rgba(236, 72, 153, 0.15)"/>
                    </linearGradient>
                    <linearGradient id="grad2_stroke" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="#8b5cf6"/>
                      <stop offset="100%" stopColor="#ec4899"/>
                    </linearGradient>

                    {/* 立方体渐变 */}
                    <linearGradient id="cube_front_gradient" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(139, 92, 246, 0.7)"/>
                      <stop offset="100%" stopColor="rgba(236, 72, 153, 0.7)"/>
                    </linearGradient>
                    <linearGradient id="cube_front_stroke" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(255, 255, 255, 0.8)"/>
                      <stop offset="100%" stopColor="rgba(255, 255, 255, 0.4)"/>
                    </linearGradient>
                    <linearGradient id="cube_right_gradient" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(139, 92, 246, 0.5)"/>
                      <stop offset="100%" stopColor="rgba(236, 72, 153, 0.5)"/>
                    </linearGradient>
                    <linearGradient id="cube_right_stroke" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(255, 255, 255, 0.6)"/>
                      <stop offset="100%" stopColor="rgba(255, 255, 255, 0.2)"/>
                    </linearGradient>
                    <linearGradient id="cube_left_gradient" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(139, 92, 246, 0.5)"/>
                      <stop offset="100%" stopColor="rgba(236, 72, 153, 0.5)"/>
                    </linearGradient>
                    <linearGradient id="cube_left_stroke" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(255, 255, 255, 0.6)"/>
                      <stop offset="100%" stopColor="rgba(255, 255, 255, 0.2)"/>
                    </linearGradient>
                    <linearGradient id="cube_lines" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="#8b5cf6"/>
                      <stop offset="100%" stopColor="#ec4899"/>
                    </linearGradient>

                    {/* NFT卡片渐变 */}
                    <linearGradient id="nft_card_gradient" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(255, 255, 255, 0.3)"/>
                      <stop offset="100%" stopColor="rgba(255, 255, 255, 0.05)"/>
                    </linearGradient>
                    <linearGradient id="nft_image_gradient" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(255, 255, 255, 0.4)"/>
                      <stop offset="100%" stopColor="rgba(255, 255, 255, 0.1)"/>
                    </linearGradient>
                    <linearGradient id="nft_card_right_gradient" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(255, 255, 255, 0.2)"/>
                      <stop offset="100%" stopColor="rgba(255, 255, 255, 0.05)"/>
                    </linearGradient>

                    <radialGradient id="grad2_highlight" cx="0%" cy="0%" r="100%">
                      <stop offset="0%" stopColor="#ffffff" stopOpacity="1"/>
                      <stop offset="100%" stopColor="#ffffff" stopOpacity="0"/>
                    </radialGradient>
                    <radialGradient id="grad2_sparkle" cx="0%" cy="0%" r="100%">
                      <stop offset="0%" stopColor="#ffffff" stopOpacity="1"/>
                      <stop offset="100%" stopColor="#ffffff" stopOpacity="0"/>
                    </radialGradient>
                  </defs>
                  </svg>
                </div>
                <div className={styles.cardTitle}>{intl.get('home_card_market')}</div>
                <div className={styles.cardDesc}>{intl.get('home_explore_cards')}</div>
              </div>
            </GlassCard>

            <GlassCard variant="default" className={`${styles.featureCard} ${styles.tertiaryCard}`}>
              <div className={styles.cardContent}>
                <div className={styles.cardIcon}>
                  <svg viewBox="0 0 48 48" fill="none">
                  {/* 背景光晕 */}
                  <circle cx="24" cy="24" r="24" fill="url(#grad3_outer)" opacity="0.3" filter="drop-shadow(0 0 8px rgba(6, 182, 212, 0.5))"/>
                  {/* 主体圆形 */}
                  <circle cx="24" cy="24" r="22" fill="url(#grad3_inner)" stroke="url(#grad3_stroke)" strokeWidth="2" filter="drop-shadow(0 4px 8px rgba(0, 0, 0, 0.4))"/>
                  {/* 圆形阴影 */}
                  <circle cx="24" cy="24" r="22" fill="none" stroke="rgba(0, 0, 0, 0.4)" strokeWidth="1" filter="blur(3px)" transform="translate(0, 2)" opacity="0.7"/>

                  {/* 时钟主体填充 - 半透明内层 */}
                  <circle cx="24" cy="24" r="18" fill="url(#clock_inner)"/>

                  {/* 时钟刻度线 - 主要刻度 */}
                  <path d="M24 8v3" stroke="url(#grad3_lines)" strokeWidth="2" strokeLinecap="round"/>
                  <path d="M24 37v3" stroke="url(#grad3_lines)" strokeWidth="2" strokeLinecap="round"/>
                  <path d="M7 24h3" stroke="url(#grad3_lines)" strokeWidth="2" strokeLinecap="round"/>
                  <path d="M38 24h3" stroke="url(#grad3_lines)" strokeWidth="2" strokeLinecap="round"/>

                  {/* 时钟刻度线 - 次要刻度 */}
                  <path d="M10 10l2 2" stroke="url(#grad3_lines)" strokeWidth="1.5" strokeLinecap="round"/>
                  <path d="M36 10l-2 2" stroke="url(#grad3_lines)" strokeWidth="1.5" strokeLinecap="round"/>
                  <path d="M10 38l2-2" stroke="url(#grad3_lines)" strokeWidth="1.5" strokeLinecap="round"/>
                  <path d="M36 38l-2-2" stroke="url(#grad3_lines)" strokeWidth="1.5" strokeLinecap="round"/>

                  {/* 时钟指针 */}
                  <path d="M24 16v8" stroke="url(#grad3_hour_hand)" strokeWidth="3.5" strokeLinecap="round" filter="drop-shadow(0 2px 4px rgba(0, 0, 0, 0.5))"/>
                  <path d="M24 24l8 8" stroke="url(#grad3_minute_hand)" strokeWidth="2.5" strokeLinecap="round" filter="drop-shadow(0 2px 4px rgba(0, 0, 0, 0.5))"/>

                  {/* 指针中心点 - 多层设计 */}
                  <circle cx="24" cy="24" r="3" fill="url(#grad3_center)" filter="drop-shadow(0 2px 3px rgba(0, 0, 0, 0.3))"/>
                  <circle cx="24" cy="24" r="1.5" fill="#ffffff"/>

                  {/* 数据波形图 - 增强实时行情感 */}
                  <path d="M10 28c2-3 4-1 6 0c2 1 4-2 6-1c2 1 4 0 6-1c2 1 4-2 6-1c2 1 4 0 6 0" fill="none" stroke="url(#grad3_wave)" strokeWidth="1.5" strokeLinecap="round" strokeLinejoin="round" opacity="0.8" filter="drop-shadow(0 0 4px rgba(6, 182, 212, 0.7))"/>

                  {/* 发光效果 */}
                  <circle cx="24" cy="24" r="18" fill="none" stroke="rgba(255, 255, 255, 0.3)" strokeWidth="1" filter="blur(2px)"/>
                  <path d="M10 28c2-3 4-1 6 0c2 1 4-2 6-1c2 1 4 0 6-1c2 1 4-2 6-1c2 1 4 0 6 0" fill="none" stroke="rgba(255, 255, 255, 0.4)" strokeWidth="1" filter="blur(2px)" opacity="0.7"/>

                  {/* 高光效果 */}
                  <circle cx="20" cy="20" r="4" fill="url(#grad3_highlight)" opacity="0.7"/>
                  <circle cx="32" cy="16" r="2" fill="url(#grad3_sparkle)" opacity="0.6"/>
                  <circle cx="18" cy="34" r="2" fill="url(#grad3_sparkle)" opacity="0.5"/>

                  <defs>
                    <linearGradient id="grad3_outer" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="#06b6d4"/>
                      <stop offset="100%" stopColor="#1677ff"/>
                    </linearGradient>
                    <linearGradient id="grad3_inner" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(6, 182, 212, 0.15)"/>
                      <stop offset="100%" stopColor="rgba(22, 119, 255, 0.15)"/>
                    </linearGradient>
                    <linearGradient id="grad3_stroke" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="#06b6d4"/>
                      <stop offset="100%" stopColor="#1677ff"/>
                    </linearGradient>
                    <linearGradient id="clock_inner" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="rgba(255, 255, 255, 0.2)"/>
                      <stop offset="100%" stopColor="rgba(255, 255, 255, 0.05)"/>
                    </linearGradient>
                    <linearGradient id="grad3_lines" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="#06b6d4"/>
                      <stop offset="100%" stopColor="#1677ff"/>
                    </linearGradient>
                    <linearGradient id="grad3_hour_hand" x1="0%" y1="0%" x2="0%" y2="100%">
                      <stop offset="0%" stopColor="#1677ff"/>
                      <stop offset="100%" stopColor="#06b6d4"/>
                    </linearGradient>
                    <linearGradient id="grad3_minute_hand" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="#1677ff"/>
                      <stop offset="100%" stopColor="#06b6d4"/>
                    </linearGradient>
                    <linearGradient id="grad3_wave" x1="0%" y1="0%" x2="100%" y2="100%">
                      <stop offset="0%" stopColor="#06b6d4"/>
                      <stop offset="100%" stopColor="#1677ff"/>
                    </linearGradient>
                    <radialGradient id="grad3_center" cx="0%" cy="0%" r="100%">
                      <stop offset="0%" stopColor="#ffffff"/>
                      <stop offset="100%" stopColor="#1677ff"/>
                    </radialGradient>
                    <radialGradient id="grad3_highlight" cx="0%" cy="0%" r="100%">
                      <stop offset="0%" stopColor="#ffffff" stopOpacity="1"/>
                      <stop offset="100%" stopColor="#ffffff" stopOpacity="0"/>
                    </radialGradient>
                    <radialGradient id="grad3_sparkle" cx="0%" cy="0%" r="100%">
                      <stop offset="0%" stopColor="#ffffff" stopOpacity="1"/>
                      <stop offset="100%" stopColor="#ffffff" stopOpacity="0"/>
                    </radialGradient>
                  </defs>
                  </svg>
                </div>
                <div className={styles.cardTitle}>{intl.get('home_real_time_quotes')}</div>
                <div className={styles.cardDesc}>{intl.get('home_24h_monitoring')}</div>
              </div>
            </GlassCard>
            </div>
          </div>
        </motion.div>
        )}
      </div>

      <div className={styles.glowEffect} />
      <div className={styles.glowEffect2} />
    </motion.section>
  )
}

export default HeroSection
