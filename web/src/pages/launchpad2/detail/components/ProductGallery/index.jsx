import React, { useState } from 'react'
import { useNavigate } from 'react-router-dom'
import TradeNotice from '../TradeNotice'
import styles from './index.module.less'

const ProductGallery = ({ images = [], detail }) => {
    const [activeIndex, setActiveIndex] = useState(0)
    const navigate = useNavigate()

    const { launchpadInfo } = detail || {}
    const tradeTime = launchpadInfo?.TradeTime
    const status = launchpadInfo?.Status
    const instrumentID = launchpadInfo?.InstrumentID

    // 判断是否已到交易时间
    const isTradeTimeReached = tradeTime && Date.now() >= new Date(tradeTime).getTime()

    // 判断是否显示交易提示（Status === 2 且已到交易时间）
    // 临时强制显示，用于预览效果
    const showTradeNotice = status === 2 && isTradeTimeReached // status === 2 && isTradeTimeReached

    const handleTradeClick = () => {
        if (instrumentID) {
            navigate(`/exchange?ProductGroup=SPOT&InstrumentID=${instrumentID}`)
        }
    }

    return (
        <div className={styles.hero}>
            {/* 主图展示区 */}
            <div className={styles.mainImage}>
                {/* 交易提示 - Status === 2 且已到交易时间 */}
                {showTradeNotice && (
                    <TradeNotice
                        tradeTime={tradeTime}
                        onTradeClick={handleTradeClick}
                    />
                )}

                <div className={styles.imageWrapper}>
                    {/* 预渲染所有图片，通过 display 控制显示 */}
                    {images.map((src, idx) => (
                        <img
                            key={src}
                            src={src}
                            alt="Asset Preview"
                            style={{ display: idx === activeIndex ? 'block' : 'none' }}

                        />
                    ))}
                </div>

                {/* 指示器 */}
                {images.length > 1 && (
                    <div className={styles.indicators}>
                        {images.map((_, idx) => (
                            <button
                                key={idx}
                                className={`${styles.indicator} ${idx === activeIndex ? styles.active : ''}`}
                                onClick={() => setActiveIndex(idx)}
                            />
                        ))}
                    </div>
                )}
            </div>

            {/* 缩略图 */}
            {images.length > 1 && (
                <div className={styles.thumbnails}>
                    {images.map((url, idx) => (
                        <button
                            key={idx}
                            className={`${styles.thumbnail} ${idx === activeIndex ? styles.active : ''}`}
                            onClick={() => setActiveIndex(idx)}
                        >
                            <img src={url} alt={`Thumbnail ${idx + 1}`} />
                        </button>
                    ))}
                </div>
            )}
        </div>
    )
}

export default ProductGallery
