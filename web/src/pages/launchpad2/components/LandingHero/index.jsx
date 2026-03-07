import React from 'react'
import intl from '@/component/intl'
import styles from './index.module.less'
import banner1 from '@/assets/images/launchpad/launchpad-banner1.jpeg'
import banner2 from '@/assets/images/launchpad/launchpad-banner2.jpeg'
import banner3 from '@/assets/images/launchpad/launchpad-banner3.jpeg'

// 使用本地卡片素材
const cardImages = [
    banner1,
    banner2,
    banner3
]

const LandingHero = () => {
    return (
        <div className={styles.hero}>
            {/* 背景纹理与氛围光 */}
            <div className={styles.background}>
                <div className={styles.noise} />
                <div className={styles.glow} />
            </div>

            <div className={styles.container}>
                <div className={styles.content}>
                    {/* 左侧：文案部分 */}
                    <div className={styles.textSection}>
                        <div className={styles.badge}>
                            <span className={styles.badgeDot} />
                            {intl.get('launchpad2_institutional_grade')}
                        </div>
                        <h1 className={styles.title}>
                            {intl.get('launchpad2_own_legends')}
                        </h1>
                        <p className={styles.description}>
                            {intl.get('launchpad2_elite_ownership')}
                        </p>
                        <div className={styles.actions}>
                            <button className={styles.actionBtn}>{intl.get('launchpad2_how_it_works')}</button>
                        </div>
                    </div>

                    {/* 右侧：卡片展示 */}
                    <div className={styles.cardsSection}>
                        <div className={styles.cardsWrapper}>
                            {/* 后层卡片 */}
                            <div className={`${styles.card} ${styles.cardBack}`}>
                                <div className={styles.cardInner}>
                                    <img src={cardImages[1]} className={styles.cardImage} alt="Card Back" />
                                </div>
                            </div>

                            {/* 中层卡片 */}
                            <div className={`${styles.card} ${styles.cardMiddle}`}>
                                <div className={styles.cardInner}>
                                    <img src={cardImages[0]} className={styles.cardImage} alt="Card Middle" />
                                    <div className={styles.cardOverlay} />
                                </div>
                            </div>

                            {/* 前层卡片 */}
                            <div className={`${styles.card} ${styles.cardFront}`}>
                                <div className={styles.cardInner}>
                                    <img src={cardImages[2]} className={styles.cardImage} alt="Card Front" />
                                    <div className={styles.cardShine}>
                                        <div className={styles.cardShineEffect} />
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    )
}

export default LandingHero
