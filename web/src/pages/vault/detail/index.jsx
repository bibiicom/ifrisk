import React, { useState, useEffect } from 'react'
import { useParams } from 'react-router-dom'
import { Spin } from 'antd'
import styles from './index.module.less'
import intl from '../../../component/intl'
import CardGallery from './components/CardGallery'
import PsaInfo from './components/PsaInfo'
import CardIntroduction from './components/CardIntroduction'
import VideoSection from './components/VideoSection'
import VaultStatus from './components/VaultStatus'
import { getCardDetail } from '../../../api/vault'
import { formatSignificantDigits } from '@/utils/format'
import { useNavigate } from "react-router-dom";


const VaultDetail = () => {
    const { id } = useParams()
    const [card, setCard] = useState(null)
    const [loading, setLoading] = useState(true)
    const navigate = useNavigate();

    useEffect(() => {
        const fetchDetail = async () => {
            try {
                const res = await getCardDetail({ id })
                console.log('Card Detail API Response:', res)

                if (res.code === 0 && res.data) {
                    setCard(res.data)
                } else {
                    // 接口失败或无数据时，暂时先不显示错误，或者可以保留 mock 作为测试
                    // setCard(MOCK_CARD) 
                }
            } catch (error) {
                console.error('Failed to fetch card detail:', error)
            } finally {
                setLoading(false)
            }
        }

        fetchDetail()
    }, [id])

    if (loading) {
        return (
            <div className={styles.loading}>
                <Spin size="large" />
            </div>
        )
    }

    if (!card) {
        return (
            <div className={styles.error}>
                <h2>{intl.get('vault_detail_not_found')}</h2>
                <p>{intl.get('vault_detail_not_found_desc')}</p>
            </div>
        )
    }

    const itemGradeParts = (card?.ItemGrade || '').split(' ')
    const grade = itemGradeParts.length > 1 ? itemGradeParts.pop() : ''
    const gradeLabel = itemGradeParts.join(' ') || card?.ItemGrade || 'PSA'

    // 处理按钮点击
    const handleActionClick = () => {
        if (card?.LaunchpadID) {
            const currentLang = window.location.pathname.split('/')[1]
            window.open(`${window.location.origin}/${currentLang}/launchpad/${card.LaunchpadID}`, '_blank')
        } else if (card?.InstrumentID) {
            const productGroup = "SPOT"
            navigate(`/exchange?ProductGroup=${productGroup}&InstrumentID=${card.InstrumentID}`)
        }
    }

    // 获取按钮文字
    const getActionButtonText = () => {
        if (card?.LaunchpadID) return '去申购'
        if (card?.InstrumentID) return '去交易'
        return null
    }

    const actionButtonText = getActionButtonText()

    const promoVideoUrl = "https://www.w3schools.com/html/mov_bbb.mp4"
    const vaultVideoUrl = "https://www.w3schools.com/html/movie.mp4"

    return (
        <div className={styles.container}>
            <main className={styles.main}>
                <div className={styles.detailSection}>
                    {/* 左侧详情栏 */}
                    <div className={styles.leftColumn}>
                        <div className={styles.cardHeader}>
                            <span className={styles.rareBadge}>
                                {intl.get('vault_detail_rare_badge')}
                            </span>
                            <div className={styles.titleGroup}>
                                <h1 className={styles.cardName}>{card.Name}</h1>
                                <p className={styles.cardSeries}>
                                    {card?.BrandTitle || '-'} — {card?.ItemGrade || '-'}
                                </p>
                            </div>

                            {/* 交易/申购按钮 */}
                            {actionButtonText && (
                                <div className={styles.actionButtons}>
                                    <button
                                        className={styles.actionButton}
                                        onClick={handleActionClick}
                                    >
                                        {actionButtonText}
                                    </button>
                                </div>
                            )}
                        </div>

                        <div className={styles.statsBar}>
                            <div className={styles.statItem}>
                                <p className={styles.statLabel}>{intl.get('vault_detail_market_value')}</p>
                                <p className={styles.statValue}>{card?.Valuation ? formatSignificantDigits(card?.Valuation) : '-'} </p>
                            </div>
                            <div className={styles.divider}></div>
                            <div className={styles.statItem}>
                                <p className={styles.statLabel}>{intl.get('vault_detail_storage_status')}</p>
                                <p className={styles.statText}>{intl.get('vault_detail_storage_status_text')}</p>
                            </div>
                        </div>

                        <CardIntroduction description={card.Description} />
                        <PsaInfo
                            grade={grade}
                            gradeLabel={gradeLabel}
                            certNumber={card.CertNumber}
                            year={card.Year}
                            VarietyPedigree={card.VarietyPedigree}
                            population={'-'}
                            subject={card.Subject}
                        />
                    </div>

                    {/* 右侧展示栏 */}
                    <div className={styles.rightColumn}>
                        <CardGallery imgGroup={card.ImgGroup} img={card.Img} />
                    </div>
                </div>

                {/* 媒体展示区域 */}
                <div className={styles.mediaSection}>
                    <div className={styles.sectionHeader}>
                        <h2 className={styles.sectionTitle}>{intl.get('vault_detail_media_title')}</h2>
                        <div className={styles.titleUnderline}></div>
                    </div>

                    <div className={styles.videoGrid}>
                        <VideoSection
                            title={intl.get('vault_detail_promo_title')}
                            subtitle={intl.get('vault_detail_promo_subtitle')}
                            videoUrl={card.PromoVideoUrl || promoVideoUrl}
                            poster="https://picsum.photos/seed/promo/800/450"
                        />
                        <VideoSection
                            title={intl.get('vault_detail_vault_title')}
                            subtitle={intl.get('vault_detail_vault_subtitle')}
                            videoUrl={card.VaultVideoUrl || vaultVideoUrl}
                            poster="https://picsum.photos/seed/vault/800/450"
                            isVault={true}
                        />
                    </div>
                </div>

                <VaultStatus location={'Delaware Vault (Wilmington, DE)'} />
            </main>
        </div>
    )
}

export default VaultDetail
