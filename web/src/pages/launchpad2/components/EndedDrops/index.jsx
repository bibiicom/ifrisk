import React, { useState, useEffect, useRef, useCallback } from 'react'
import { App, Button, Spin } from 'antd'
import intl from '@/component/intl'
import LaunchpadCard from '../LaunchpadCard'
import { fetchEndLaunchpadInfo } from '@/api/launchpad'
import styles from './index.module.less'

const PAGE_SIZE = 10

const EndedDrops = () => {
    const { message } = App.useApp()
    const [projects, setProjects] = useState([])
    const [loading, setLoading] = useState(false)
    const [page, setPage] = useState(1)
    const [total, setTotal] = useState(0)
    const [hasMore, setHasMore] = useState(true)
    const [initialized, setInitialized] = useState(false)
    const loadingRef = useRef(false)

    const fetchData = useCallback(async (pageNum = 1, append = false) => {
        if (loadingRef.current) return

        loadingRef.current = true
        setLoading(true)

        try {
            const res = await fetchEndLaunchpadInfo({ page: pageNum, limit: PAGE_SIZE })
            if (res.code === 0 && res.data) {
                const { launchpadList = [], total: totalCount = 0 } = res.data

                setProjects(prev => {
                    // 确保 launchpadList 不为 null
                    const safeList = launchpadList || []
                    const newList = append ? [...prev, ...safeList] : safeList
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

    useEffect(() => {
        fetchData(1, false)
    }, [])

    // 未初始化或没有数据时不渲染
    if (!initialized || projects.length === 0) return null

    return (
        <div className={styles.section}>
            <div className={styles.header}>
                <div className={styles.titleWrapper}>
                    <h2 className={styles.title}>{intl.get('launchpad2_ended')}</h2>
                    <span className={styles.badge}>{total}</span>
                </div>
            </div>

            <div className={styles.list}>
                {projects.map(project => (
                    <LaunchpadCard key={project.Id} project={project} status="ended" />
                ))}
            </div>

            {hasMore && (
                <div className={styles.loadMoreWrapper}>
                    <Button
                        className={styles.loadMoreBtn}
                        onClick={loadMore}
                        loading={loading}
                    >
                        {loading ? intl.get('launchpad2_loading') : intl.get('launchpad2_load_more')}
                    </Button>
                </div>
            )}

            {loading && projects.length === 0 && (
                <div className={styles.loadingWrapper}>
                    <Spin />
                </div>
            )}
        </div>
    )
}

export default EndedDrops
