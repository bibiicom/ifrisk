import React, { useRef, useState } from 'react'
import styles from './index.module.less'

const VideoSection = ({ title, subtitle, videoUrl, poster, isVault = false }) => {
    const videoRef = useRef(null)
    const [isPlaying, setIsPlaying] = useState(false)

    const togglePlay = () => {
        if (videoRef.current) {
            if (isPlaying) {
                videoRef.current.pause()
            } else {
                videoRef.current.play()
            }
            setIsPlaying(!isPlaying)
        }
    }

    return (
        <div className={styles.container}>
            <div className={styles.videoWrapper}>
                <video
                    ref={videoRef}
                    className={styles.video}
                    poster={poster}
                    onPlay={() => setIsPlaying(true)}
                    onPause={() => setIsPlaying(false)}
                >
                    <source src={videoUrl} type="video/mp4" />
                </video>

                {/* 播放按钮覆盖层 */}
                {!isPlaying && (
                    <div className={styles.playOverlay}>
                        <button onClick={togglePlay} className={styles.playButton}>
                            <svg className={styles.playIcon} fill="currentColor" viewBox="0 0 20 20">
                                <path d="M4.018 14L14.41 8 4.018 2v12z" />
                            </svg>
                        </button>
                    </div>
                )}

                {/* 暂停按钮 */}
                {isPlaying && (
                    <button onClick={togglePlay} className={styles.pauseButton}>
                        <svg className={styles.pauseIcon} fill="currentColor" viewBox="0 0 20 20">
                            <path d="M4 4h2v12H4V4zm10 0h2v12h-2V4z" />
                        </svg>
                    </button>
                )}

                {/* 金库视频标记 */}
                {isVault && (
                    <div className={styles.vaultStamp}>
                        ● REC: VAULT_02A
                    </div>
                )}
            </div>

            <div className={styles.info}>
                <h3 className={styles.title}>{title}</h3>
                <p className={styles.subtitle}>{subtitle}</p>
            </div>
        </div>
    )
}

export default VideoSection
