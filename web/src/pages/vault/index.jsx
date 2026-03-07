import React, { useState, useEffect, useCallback, useRef } from 'react'
import { useNavigate } from 'react-router-dom'
import { Spin } from 'antd'
import Masonry from 'react-masonry-css'
import styles from './index.module.less'
import intl from '../../component/intl'
import { VAULT_DETAIL } from '../routes'
import { getCardList } from '../../api/vault'
import defaultCardImg from '../../assets/images/common/default-card.jpeg'
import authHOC from '../common/hoc/authHOC'

const Vault = () => {
    const navigate = useNavigate()
    const [items, setItems] = useState([])
    const [loading, setLoading] = useState(false) // 加载状态
    const loadingRef = useRef(false) // 使用 ref 来追踪 loading 状态，避免闭包问题
    const [page, setPage] = useState(1)
    const [hasMore, setHasMore] = useState(true)
    const [total, setTotal] = useState(0)
    const [activeTab, setActiveTab] = useState('all') // 新增：当前激活的 tab



     // Tab 配置
    const tabs = [
        { key: 'all', label: intl.get('vault_tab_all'), type: 0 },
        { key: 'trading', label: intl.get('vault_tab_trading'), type: 1 },
        { key: 'subscription', label: intl.get('vault_tab_subscription'), type: 2 },
        { key: 'pending', label: intl.get('vault_tab_pending'), type: 3 }
    ]


    // 处理卡片点击
    const handleCardClick = useCallback((cardId) => {
        navigate(VAULT_DETAIL.replace(':id', cardId))
    }, [navigate])

    // 获取卡片数据
    const fetchCards = useCallback(async (pageNum, tabKey) => {
        if (loadingRef.current) return
        
        loadingRef.current = true
        setLoading(true)
        
        try {
            // 根据 tab key 获取对应的 type 值
            const currentTab = tabs.find(tab => tab.key === tabKey)
            const typeValue = currentTab ? currentTab.type : 0
            
            const res = await getCardList({
                page: pageNum,
                limit: 20,
                type: typeValue
            })
            
            if (res.code === 0 && res.data && res.data.list) {
                const newCards = res.data.list.map(item => ({
                    id: item.Id,
                    image: item.Img || defaultCardImg,
                    name: item.Name, // alt text
                    player: item.Name, // display title
                    grade: item.ItemGrade
                }))

                setItems(prev => pageNum === 1 ? newCards : [...prev, ...newCards])
                setTotal(res.data.total)
                setHasMore(newCards.length === 20) // 假设limit是10，如果返回少于10条说明没更多了
                setPage(pageNum) // 请求成功后才更新页码
            }
        } catch (error) {
            console.error('Failed to fetch cards:', error)
        } finally {
            loadingRef.current = false
            setLoading(false)
        }
    }, []) 

    // 初始加载
    useEffect(() => {
        fetchCards(1, activeTab)
    }, [fetchCards, activeTab])

    // 加载更多
    const loadMore = useCallback(() => {
        if (loading || !hasMore) return
        
        const nextPage = page + 1
        fetchCards(nextPage, activeTab) // 直接请求下一页，不立即更新 page state
    }, [loading, hasMore, page, fetchCards, activeTab])

    useEffect(() => {
        console.log('Setting up scroll listener')

        let throttleTimer = null

        const handleScrollEvent = () => {
            // 节流:200ms内只执行一次
            if (throttleTimer) return

            throttleTimer = setTimeout(() => {
                throttleTimer = null

                if (loading || !hasMore) return

                // 尝试从OverlayScrollbars容器获取滚动位置
                const osViewport = document.querySelector('.os-viewport')
                let scrollTop, scrollHeight, clientHeight

                if (osViewport) {
                    scrollTop = osViewport.scrollTop
                    scrollHeight = osViewport.scrollHeight
                    clientHeight = osViewport.clientHeight
                } else {
                    scrollTop = window.pageYOffset || document.documentElement.scrollTop
                    scrollHeight = document.documentElement.scrollHeight
                    clientHeight = window.innerHeight
                }

                const remaining = scrollHeight - scrollTop - clientHeight
                const triggerDistance = 200 // 距离底部200px时触发,让用户能看到最后几张卡片

                // 提前触发加载
                if (remaining <= triggerDistance) {
                    console.log('Loading more cards...')
                    loadMore()
                }
            }, 200)
        }

        // 监听多个可能的滚动源
        window.addEventListener('scroll', handleScrollEvent, { capture: true, passive: true })
        document.addEventListener('scroll', handleScrollEvent, { capture: true, passive: true })

        // 延迟查找OverlayScrollbars容器
        const timer = setTimeout(() => {
            const osViewport = document.querySelector('.os-viewport')
            if (osViewport) {
                console.log('Found and attached to OverlayScrollbars viewport')
                osViewport.addEventListener('scroll', handleScrollEvent, { passive: true })
            }
        }, 100)

        return () => {
            console.log('Removing scroll listener')
            if (throttleTimer) clearTimeout(throttleTimer)
            clearTimeout(timer)
            window.removeEventListener('scroll', handleScrollEvent, { capture: true })
            document.removeEventListener('scroll', handleScrollEvent, { capture: true })
            const osViewport = document.querySelector('.os-viewport')
            if (osViewport) {
                osViewport.removeEventListener('scroll', handleScrollEvent)
            }
        }
    }, [loading, hasMore, loadMore])


    const breakpointColumns = {
        default: 6,
        1200: 3,
        900: 2,
        600: 1
    }

   
    // 处理 tab 切换
    const handleTabChange = useCallback((tabKey) => {
        setActiveTab(tabKey)
        setPage(1)
        setItems([])
        setHasMore(true)
    }, [])

    return (
        <div className={styles.container}>
            <div className={styles.hero}>
                <div className={styles.heroContent}>
                    <div className={styles.heroText}>
                        <h1 className={styles.heroTitle}>{intl.get('vault_page_title')}</h1>
                        <p className={styles.heroDescription}>
                            {intl.get('vault_page_description')}
                        </p>
                    </div>
                    <div className={styles.heroImage}>
                        <img src={require('../../assets/images/launchpad/launchpad-banner1.jpeg')} alt="Featured Card" />
                    </div>
                </div>
            </div>

            <div className={styles.content}>
                <div className={styles.header}>
                    <div className={styles.tabs}>
                        {tabs.map(tab => (
                            <button 
                                key={tab.key}
                                className={`${styles.tab} ${activeTab === tab.key ? styles.active : ''}`}
                                onClick={() => handleTabChange(tab.key)}
                            >
                                {tab.label}
                            </button>
                        ))}
                    </div>
                    <div className={styles.headerRight}>
                        <div className={styles.count}>
                            {intl.get('vault_cards')} ({total})
                        </div>
                    </div>
                </div>

                <Masonry
                    breakpointCols={breakpointColumns}
                    className={styles.masonryGrid}
                    columnClassName={styles.masonryColumn}
                >
                    {items.map((item, index) => (
                        <div
                            key={index}
                            className={styles.card}
                            onClick={() => handleCardClick(item.id)}
                        >
                            <div className={styles.cardImage}>
                                <img src={item.image} alt={item.name} />
                            </div>
                            <div className={styles.cardContent}>
                                <h3 className={styles.cardTitle}>{item.player}</h3>
                            </div>
                        </div>
                    ))}
                </Masonry>

                {loading && (
                    <div className={styles.loadingMore}>
                        <Spin size="default" />
                    </div>
                )}
            </div>
        </div>
    )
}

export default authHOC(true)(Vault)
