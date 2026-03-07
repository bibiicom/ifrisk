import React, { useState } from 'react'
import styles from './index.module.less'
import intl from '../../../../../component/intl'

const CardGallery = ({ imgGroup, img }) => {
    const [activeIdx, setActiveIdx] = useState(0)

    // 解析图片数据
    const views = React.useMemo(() => {
        if (imgGroup) {
            // 尝试按 | 分割，如果不行再尝试按换行符
            let urls = imgGroup.split('|').filter(Boolean);
            if (urls.length === 1 && imgGroup.includes('\n')) {
                urls = imgGroup.split('\n').filter(Boolean);
            }
            
            return urls.map((url, index) => ({
                label: index === 0 ? intl.get('vault_gallery_front') : intl.get('vault_gallery_detail'),
                url: url.trim()
            }))
        }
        
        if (img) {
            return [{ label: intl.get('vault_gallery_front'), url: img }]
        }
        
        return []
    }, [imgGroup, img])

    if (!views.length) return null

    return (
        <div className={styles.container}>
            {/* 主展示区域 */}
            <div className={styles.mainViewport}>
                <div className={styles.gradientOverlay}></div>
                {/* 预渲染所有图片,通过 display 控制显示,避免重复请求 */}
                {views.map((view, idx) => (
                    <img
                        key={idx}
                        src={view.url}
                        alt={view.label}
                        className={styles.mainImage}
                        style={{ display: idx === activeIdx ? 'block' : 'none' }}
                    />
                ))}
            </div>

            {/* 缩略图 */}
            <div className={styles.thumbnails}>
                {views.map((view, idx) => (
                    <button
                        key={idx}
                        onClick={() => setActiveIdx(idx)}
                        className={`${styles.thumbnail} ${activeIdx === idx ? styles.active : ''}`}
                    >
                        <img src={view.url} alt={view.label} />
                    </button>
                ))}
            </div>
        </div>
    )
}

export default CardGallery


