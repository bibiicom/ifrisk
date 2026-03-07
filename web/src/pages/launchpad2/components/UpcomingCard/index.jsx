import React, { useState, useEffect } from 'react'
import { useNavigate } from 'react-router-dom'
import dayjs from 'dayjs'
import utc from 'dayjs/plugin/utc'
import intl from '@/component/intl'
import { getNftMetadata } from '@/api/nft'
import styles from './index.module.less'

dayjs.extend(utc)

const UpcomingCard = ({ project }) => {
    const navigate = useNavigate()

    // NFT 图片状态
    const [nftImage, setNftImage] = useState('')
    const [imageLoading, setImageLoading] = useState(false)

    // 根据 openTime 判断显示内容
    const getTimeLabel = () => {
        if (!project.openTime) {
            return intl.get('launchpad2_coming_soon_label')
        }
        return dayjs(project.openTime).utc().format('YYYY-MM-DD HH:mm') + ' (UTC)'
    }

    const handleClick = () => {
        navigate(`/launchpad/${project.id}`)
    }

    // 获取 NFT 图片
    useEffect(() => {
        const fetchNftImage = async () => {
            // 判断是否为 NFT: 有 Contract 和 TokenID
            const contract = project.CardInfo?.Contract
            const tokenId = project.CardInfo?.TokenID

            if (contract && tokenId) {
                setImageLoading(true)
                try {
                    const metadata = await getNftMetadata(contract, tokenId)
                    if (metadata?.image) {
                        setNftImage(metadata.image)
                    }
                } catch (error) {
                    console.error('获取 NFT 图片失败:', error)
                } finally {
                    setImageLoading(false)
                }
            }
        }

        fetchNftImage()
    }, [project.CardInfo?.Contract, project.CardInfo?.TokenID])

    // 确定最终显示的图片: NFT 图片 > CardInfo.Img > imageUrl(默认图片)
    const displayImage = nftImage || project.CardInfo?.Img || project.imageUrl

    return (
        <div className={styles.card} onClick={handleClick} style={{ cursor: 'pointer' }}>
            {/* 卡片容器 */}
            <div className={styles.cardInner}>
                {/* 背景反光层 */}
                <div className={styles.reflectLayer}>
                    <div className={styles.reflectShine} />
                    <div className={styles.reflectTop} />
                </div>

                {/* 图片区域 */}
                <div className={styles.imageWrapper}>
                    <img
                        src={displayImage}
                        alt={project.name}
                        className={styles.image}
                    />
                </div>

                {/* 底部装饰线条 */}
                <div className={styles.bottomLine} />
            </div>

            {/* 信息区域 */}
            <div className={styles.info}>
                <div className={styles.labelRow}>
                    <span className={styles.label}>{getTimeLabel()}</span>
                    <div className={styles.labelLine} />
                </div>
                <h4 className={styles.name}>{project.name}</h4>
            </div>
        </div>
    )
}

export default UpcomingCard
