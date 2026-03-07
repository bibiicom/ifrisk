import React, { useState, useEffect } from 'react'
import styles from './index.module.less'

const SuccessModal = ({ isOpen, onClose, cardName, cardImageUrl, shares, amount }) => {
    const [showContent, setShowContent] = useState(false)

    useEffect(() => {
        if (isOpen) {
            const timer = setTimeout(() => setShowContent(true), 100)
            return () => clearTimeout(timer)
        } else {
            setShowContent(false)
        }
    }, [isOpen])

    if (!isOpen) return null

    return (
        <div className={styles.overlay}>
            <div
                className={`${styles.backdrop} ${showContent ? styles.show : ''}`}
                onClick={onClose}
            />

            <div className={`${styles.content} ${showContent ? styles.show : ''}`}>
                <div className={styles.grid}>
                    {/* 左侧图片 */}
                    <div className={styles.imageSection}>
                        <div className={styles.imageGlow}></div>
                        <div className={styles.imageWrapper}>
                            <img src={cardImageUrl} alt="Owned Asset" />
                        </div>
                    </div>

                    {/* 右侧内容 */}
                    <div className={styles.infoSection}>
                        <div className={styles.title}>
                            <h2>Success!</h2>
                            <p>
                                You&apos;ve secured fractional shares of{' '}
                                <span className={styles.cardName}>{cardName}</span>.
                            </p>
                        </div>

                        <div className={styles.stats}>
                            <div className={styles.stat}>
                                <p className={styles.statLabel}>Shares</p>
                                <p className={styles.statValue}>
                                    {shares.toLocaleString(undefined, { maximumFractionDigits: 1 })}
                                </p>
                            </div>
                            <div className={styles.stat}>
                                <p className={styles.statLabel}>Value</p>
                                <p className={styles.statValue}>${amount.toLocaleString()}</p>
                            </div>
                        </div>

                        <div className={styles.actions}>
                            <button className={styles.shareBtn}>Share</button>
                            <button className={styles.closeBtn} onClick={onClose}>Close</button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    )
}

export default SuccessModal
