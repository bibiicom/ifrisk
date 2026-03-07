import React, { useState, useRef, useMemo } from 'react'
import styles from './MediaGallery.module.less'

const MediaGallery = ({ images, info }) => {
  const [selectedImage, setSelectedImage] = useState(0)
  const thumbnailListRef = useRef(null)
  const [isDragging, setIsDragging] = useState(false)
  const [startY, setStartY] = useState(0)
  const [scrollTop, setScrollTop] = useState(0)

console.log(info,'inof')

  // 鼠标拖动滚动处理
  const handleMouseDown = (e) => {
    setIsDragging(true)
    setStartY(e.pageY - thumbnailListRef.current.offsetTop)
    setScrollTop(thumbnailListRef.current.scrollTop)
  }

  const handleMouseMove = (e) => {
    if (!isDragging) return
    e.preventDefault()
    const y = e.pageY - thumbnailListRef.current.offsetTop
    const walk = (startY - y) * 2 // 滚动速度倍数
    thumbnailListRef.current.scrollTop = scrollTop + walk
  }

  const handleMouseUp = () => {
    setIsDragging(false)
  }

  const handleMouseLeave = () => {
    setIsDragging(false)
  }

  // 箭头按钮滚动处理
  const handleScrollUp = () => {
    if (thumbnailListRef.current) {
      thumbnailListRef.current.scrollBy({ top: -120, behavior: 'smooth' })
    }
    if (selectedImage > 0) {
      setSelectedImage(selectedImage - 1)
    }
  }

  const handleScrollDown = () => {
    if (thumbnailListRef.current) {
      thumbnailListRef.current.scrollBy({ top: 120, behavior: 'smooth' })
    }
    if (selectedImage < images.length - 1) {
      setSelectedImage(selectedImage + 1)
    }
  }

  // 时间轴节点数据
  const stats = useMemo(() => {
    const format = (time) => {
      if (!time) return ''
      const d = new Date(time)
      if (Number.isNaN(d.getTime())) return ''
      return d.toLocaleString()
    }

    return [
      {
        label: '预展期',
        value: '', // 无具体时间，第一个点默认点亮
      },
      {
        label: '申购期',
        value: format(info?.OpenTime),
      },
      {
        label: '申购结束',
        value: format(info?.CloseTime),
      },
      {
        label: '交易期',
        value: format(info?.TradeTime),
      },
    ]
  }, [info])

  // 计算当前所在阶段索引：
  // 0: 预展期（默认）
  // 1: 已到达申购期
  // 2: 已到达申购结束
  // 3: 已到达交易期
  const currentPhaseIndex = useMemo(() => {
    const now = Date.now()

    const openTime = info?.OpenTime ? new Date(info.OpenTime).getTime() : null
    const closeTime = info?.CloseTime ? new Date(info.CloseTime).getTime() : null
    const tradeTime = info?.TradeTime ? new Date(info.TradeTime).getTime() : null

    let index = 0 // 第一个点默认点亮

    if (openTime && now >= openTime) {
      index = 1
    }
    if (closeTime && now >= closeTime) {
      index = 2
    }
    if (tradeTime && now >= tradeTime) {
      index = 3
    }

    return index
  }, [info])

  return (
    <div className={styles.leftSection}>
      <div className={styles.galleryContainer}>
        {/* Thumbnail Gallery - Vertical */}
        <div className={styles.thumbnailColumn}>
          <button 
            className={styles.scrollButton}
            onClick={handleScrollUp}
          >
            ↑
          </button>
          
          <div 
            ref={thumbnailListRef}
            className={styles.thumbnailList}
            onMouseDown={handleMouseDown}
            onMouseMove={handleMouseMove}
            onMouseUp={handleMouseUp}
            onMouseLeave={handleMouseLeave}
            style={{ cursor: isDragging ? 'grabbing' : 'grab' }}
          >
            {images.map((img, index) => (
              <div
                key={index}
                className={`${styles.thumbnail} ${selectedImage === index ? styles.activeThumbnail : ''}`}
                onClick={() => setSelectedImage(index)}
              >
                <img src={img} alt={`Thumbnail ${index + 1}`} />
              </div>
            ))}
          </div>
          
          <button 
            className={styles.scrollButton}
            onClick={handleScrollDown}
          >
            ↓
          </button>
        </div>

        {/* Main Image */}
        <div className={styles.mainMedia}>    
          <img 
            src={images[selectedImage]} 
            alt={`Project Image ${selectedImage + 1}`}
            className={styles.mainImage}
          />
        </div>
      </div>

      {/* Stats Section - Timeline */}
      <div className={styles.statsSection}>
        <div className={styles.horizontalTimeline}>
          {stats.map((stat, index) => {
            const isActiveNode = index <= currentPhaseIndex
            const isActiveLine = index < currentPhaseIndex

            return (
              <div key={index} className={styles.timelineItem}>
                <div className={styles.timelineNode}>
                  {isActiveNode ? (
                    <div className={styles.solidDot} />
                  ) : (
                    <div className={styles.hollowDot} />
                  )}
                  {index < stats.length - 1 && (
                    <div className={`${styles.timelineLine} ${isActiveLine ? styles.activeLine : ''}`} />
                  )}
                </div>
                <div className={styles.timelineContent}>
                  <div className={styles.statLabel}>{stat.label}</div>
                  <div className={styles.statValue}>{stat.value}</div>
                </div>
              </div>
            )
          })}
        </div>
      </div>
    </div>
  )
}

export default MediaGallery
