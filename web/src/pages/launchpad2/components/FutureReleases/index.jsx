import React, { useState, useEffect, useRef, useCallback } from 'react'
import { App, Spin } from 'antd'
import intl from '@/component/intl'
import UpcomingCard from '../UpcomingCard'
import { fetchFutureLaunchpadInfo } from '@/api/launchpad'
import styles from './index.module.less'

const PAGE_SIZE = 10

// 转换 Future 接口数据
export const transformFutureData = (item) => {
    return {
        id: String(item.Id),
        name: item.Name,
        status: 'upcoming',
        imageUrl: item.CardInfo?.Img || 'https://dyn1.heritagestatic.com/ha?p=3-2-9-4-2-32942949&w=850&h=600&it=product',
        openTime: item.OpenTime,
        summary: item.Summary,
        CardInfo: item.CardInfo, // 保留完整的 CardInfo 用于判断是否为 NFT
    }
}

const FutureReleases = () => {
    const { message } = App.useApp()
    const [projects, setProjects] = useState([])
    const [loading, setLoading] = useState(false)
    const [page, setPage] = useState(1)
    const [total, setTotal] = useState(0)
    const [hasMore, setHasMore] = useState(true)
    const [initialized, setInitialized] = useState(false)
    const loadingRef = useRef(false)
    const scrollRef = useRef(null)
    const loadingElRef = useRef(null)

    const fetchData = useCallback(async (pageNum = 1, append = false) => {
        if (loadingRef.current) return

        loadingRef.current = true
        setLoading(true)

        try {
            const res = await fetchFutureLaunchpadInfo({ page: pageNum, limit: PAGE_SIZE })
            if (res.code === 0 && res.data) {
                const { launchpadList = [], total: totalCount = 0 } = res.data
                const transformed = launchpadList.map(transformFutureData)

                setProjects(prev => {
                    const newList = append ? [...prev, ...transformed] : transformed
                    setHasMore(newList.length < totalCount)
                    return newList
                })

                setTotal(totalCount)
                setPage(pageNum)
            }
        } catch (err) {
            message.error(err?.msg || intl.get('launchpad2_fetch_failed'))
        } finally {
            setLoading(false)
            loadingRef.current = false
            setInitialized(true)
        }
    }, [message])

    const loadMore = useCallback(() => {
        if (!hasMore || loading) return
        fetchData(page + 1, true)
    }, [hasMore, loading, page, fetchData])

    // 检测是否滚动到最后一个卡片
    const handleScroll = useCallback(() => {
        if (!scrollRef.current || !loadingElRef.current || loading || !hasMore) return

        const container = scrollRef.current
        const loadingEl = loadingElRef.current
        const containerRect = container.getBoundingClientRect()
        const loadingRect = loadingEl.getBoundingClientRect()

        if (loadingRect.left < containerRect.right + 100) {
            loadMore()
        }
    }, [loading, hasMore, loadMore])

    useEffect(() => {
        fetchData(1, false)
    }, [])

    useEffect(() => {
        const scrollEl = scrollRef.current
        if (!scrollEl) return

        scrollEl.addEventListener('scroll', handleScroll)

        const timer = setTimeout(() => {
            handleScroll()
        }, 100)

        return () => {
            scrollEl.removeEventListener('scroll', handleScroll)
            clearTimeout(timer)
        }
    }, [handleScroll])

    // 未初始化或没有数据时不渲染
    if (!initialized || projects.length === 0) return null

    return (
        <div className={styles.section}>
            <div className={styles.header}>
                <div className={styles.titleWrapper}>
                    <h2 className={styles.title}>{intl.get('launchpad2_future_releases')}</h2>
                    <span className={styles.badge}>{total || projects.length}</span>
                </div>
            </div>

            <div className={styles.scrollContainer}>
                <div className={styles.scrollList} ref={scrollRef}>
                    {projects.map(project => (
                        <div key={project.id} className={styles.scrollItem}>
                            <UpcomingCard project={project} />
                        </div>
                    ))}

                    {hasMore && (
                        <div ref={loadingElRef} className={styles.loadingItem}>
                            {loading && (
                                <div className={styles.loadingWrapper}>
                                    <Spin />
                                </div>
                            )}
                        </div>
                    )}

                    <div className={styles.scrollSpacer} />
                </div>
                <div className={styles.scrollFade} />
            </div>
        </div>
    )
}

export default FutureReleases
