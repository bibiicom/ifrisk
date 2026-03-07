import React, { useState, useEffect } from 'react'
import { useParams, useNavigate } from 'react-router-dom'
import { LeftOutlined } from '@ant-design/icons'
import { Spin } from 'antd'
import intl from '@/component/intl'
import ProductGallery from './components/ProductGallery'
import Sidebar from './components/Sidebar'
import DetailsTabs from './components/DetailsTabs'
import PurchaseModal from './components/PurchaseModal'
import SuccessModal from './components/SuccessModal'
import WalletModal from '@/pages/walletmodal'
import { fetchLaunchpadInfoById } from '@/api/launchpad'
import { getNftMetadata } from '@/api/nft'
import authHOC from '@/pages/common/hoc/authHOC'
import styles from './index.module.less'
import banner1 from '@/assets/images/launchpad/launchpad-banner1.jpeg'
import banner2 from '@/assets/images/launchpad/launchpad-banner2.jpeg'
import banner3 from '@/assets/images/launchpad/launchpad-banner3.jpeg'

// 状态配置：统一管理样式
const STATUS_CONFIG = {
    live: {
        textKey: 'launchpad2_live',
        className: 'status_live'
    },
    future: {
        textKey: 'launchpad2_future',
        className: 'status_future'
    },
    ended: {
        textKey: 'launchpad2_ended_status',
        className: 'status_ended'
    },
    coming_soon: {
        textKey: 'launchpad2_coming_soon',
        className: 'status_coming_soon'
    }
}

const getStatusConfig = (status) => {
    const config = STATUS_CONFIG[status]
    if (config) {
        return {
            text: intl.get(config.textKey),
            className: config.className
        }
    }
    return { text: status?.toUpperCase() || '', className: '' }
}

// 根据 OpenTime 和 CloseTime 判断状态
const getStatusFromTime = (openTime, closeTime) => {
    const now = Date.now()

    if (!openTime) {
        return 'coming_soon'
    }

    const openTimestamp = new Date(openTime).getTime()
    const closeTimestamp = closeTime ? new Date(closeTime).getTime() : null

    if (now < openTimestamp) {
        return 'future'
    }

    if (closeTimestamp && now > closeTimestamp) {
        return 'ended'
    }

    return 'live'
}

const LaunchpadDetail = () => {
    const { id } = useParams()
    const navigate = useNavigate()

    const [detail, setDetail] = useState(null)
    const [loading, setLoading] = useState(true)
    const [activeTab, setActiveTab] = useState(intl.get('launchpad2_card_details'))
    const [isPurchaseModalOpen, setIsPurchaseModalOpen] = useState(false)
    const [isSuccessModalOpen, setIsSuccessModalOpen] = useState(false)
    const [isWalletModalOpen, setIsWalletModalOpen] = useState(false)
    const [lastPurchase, setLastPurchase] = useState(null)
    const [nftImage, setNftImage] = useState('')
    const [imageLoading, setImageLoading] = useState(false)

    // 获取详情数据
    const fetchDetail = async () => {
        if (!id) return
        setLoading(true)
        try {
            const res = await fetchLaunchpadInfoById({ id })
            if (res?.data) {
                setDetail(res.data)
            }
        } catch (error) {
            console.error('获取详情失败:', error)
        } finally {
            setLoading(false)
        }
    }

    useEffect(() => {
        fetchDetail()
    }, [id])

    // 获取 NFT 图片
    useEffect(() => {
        const fetchNftImage = async () => {
            if (!detail) return

            const contract = detail.cardInfo?.Contract
            const tokenId = detail.cardInfo?.TokenID

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
    }, [detail])

    const handleBack = () => {
        // 如果有历史记录,返回上一页;否则跳转到列表页
        if (window.history.length > 1) {
            window.history.back()
        } else {
            navigate('/launchpad')
        }
    }

    const handlePurchaseSuccess = (data) => {
        setLastPurchase(data)
        setIsPurchaseModalOpen(false)
        setIsSuccessModalOpen(true)
        // 申购成功后不立即刷新,等用户关闭成功弹窗后再刷新
    }

    const handleSuccessModalClose = () => {
        setIsSuccessModalOpen(false)
        // 关闭成功弹窗后刷新详情数据
        fetchDetail()
    }

    if (loading && !detail) {
        // 只有初始加载时显示全屏 loading
        return (
            <div className={styles.page}>
                <div className={styles.loading}>
                    <Spin size="large" />
                </div>
            </div>
        )
    }

    if (!detail) {
        return (
            <div className={styles.page}>
                <div className={styles.notFound}>
                    <p>未找到该项目</p>
                    <button onClick={handleBack}>返回列表</button>
                </div>
            </div>
        )
    }

    // 从原始数据中提取常用字段
    const { launchpadInfo, cardInfo } = detail || {}
    const name = launchpadInfo?.Name || cardInfo?.Name || ''

    // 默认卡片图片
    const defaultImages = [
        banner1, banner2, banner3
    ]

    // 解析图片数据 - 支持 \n 换行符分割
    const parseImages = (imgStr) => {
        if (!imgStr) return []
        
        // 尝试按 | 分割,如果不行再尝试按换行符
        let urls = imgStr.split('|').filter(Boolean)
        if (urls.length === 1 && imgStr.includes('\n')) {
            urls = imgStr.split('\n').filter(Boolean)
        }
        
        return urls.map(url => url.trim())
    }

    // 确定最终显示的图片: NFT 图片 > CardInfo.ImgGroup > 默认图片
    // 只有在没有 Contract 字段且 ImgGroup 为空时才显示默认图片,避免闪烁
    const hasContract = cardInfo?.Contract
    const imgStr = cardInfo?.ImgGroup
    
    let images = []
    if (nftImage) {
        images = [nftImage]
    } else if (imgStr) {
        images = parseImages(imgStr)
    } else if (!hasContract) {
        images = defaultImages
    }

    // 根据 OpenTime 和 CloseTime 判断状态
    const status = getStatusFromTime(launchpadInfo?.OpenTime, launchpadInfo?.CloseTime)
    const statusConfig = getStatusConfig(status)

    return (
        <div className={styles.page}>
            {/* 刷新时显示转圈和透明遮罩,阻止点击但不遮挡视觉 */}
            {loading && detail && (
                <>
                    {/* 透明遮罩层,阻止点击 */}
                    <div style={{
                        position: 'fixed',
                        top: 0,
                        left: 0,
                        right: 0,
                        bottom: 0,
                        zIndex: 9998,
                        cursor: 'not-allowed'
                    }} />
                    {/* 转圈指示器 */}
                    <div style={{
                        position: 'fixed',
                        top: '50%',
                        left: '50%',
                        transform: 'translate(-50%, -50%)',
                        zIndex: 9999
                    }}>
                        <Spin size="large" />
                    </div>
                </>
            )}
            
            {/* 顶部导航栏 */}
            <div className={styles.header}>
                <div className={styles.headerContent}>
                    <div className={styles.headerLeft}>
                        <button className={styles.backBtn} onClick={handleBack}>
                            <LeftOutlined />
                        </button>
                        <h1 className={styles.headerTitle}>{name}</h1>
                    </div>
                    <div className={styles.headerRight}>
                        <div className={`${styles.statusBadge} ${styles[statusConfig.className]}`}>
                            <span className={styles.statusDot}></span>
                            <span className={styles.statusText}>{statusConfig.text}</span>
                        </div>
                    </div>
                </div>
            </div>

            {/* 主内容区 */}
            <main className={styles.main}>
                <div className={styles.content}>
                    <div className={styles.leftColumn}>
                        <ProductGallery images={images} detail={detail} />
                        <DetailsTabs
                            detail={detail}
                            activeTab={activeTab}
                            onTabChange={setActiveTab}
                        />
                    </div>
                    <div className={styles.rightColumn}>
                        <Sidebar
                            detail={detail}
                            onApply={() => setIsPurchaseModalOpen(true)}
                            onLogin={() => setIsWalletModalOpen(true)}
                        />
                    </div>
                </div>
            </main>

            {/* 申购弹窗 */}
            <PurchaseModal
                isOpen={isPurchaseModalOpen}
                onClose={() => setIsPurchaseModalOpen(false)}
                onSuccess={handlePurchaseSuccess}
                detail={detail}
            />

            {/* 成功弹窗 */}
            <SuccessModal
                isOpen={isSuccessModalOpen}
                onClose={handleSuccessModalClose}
                cardName={name}
                cardImageUrl={cardInfo?.Img}
                shares={lastPurchase?.shares || 0}
                amount={lastPurchase?.amount || 0}
            />

            {/* 登录弹窗 */}
            <WalletModal
                open={isWalletModalOpen}
                onClose={() => setIsWalletModalOpen(false)}
            />
        </div>
    )
}



export default authHOC(true)(LaunchpadDetail)
