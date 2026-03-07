import React, { useState, useEffect } from 'react'
import { App } from 'antd'
import intl from '@/component/intl'
import LaunchpadCard from '../LaunchpadCard'
import { fetchLiveLaunchpadInfo } from '@/api/launchpad'
import styles from './index.module.less'

const LiveDrops = () => {
    const { message } = App.useApp()
    const [projects, setProjects] = useState([])
    const [loading, setLoading] = useState(false)

    useEffect(() => {
        const fetchData = async () => {
            setLoading(true)
            try {
                const res = await fetchLiveLaunchpadInfo()
                if (res.code === 0 && res.data?.launchpadList) {
                    setProjects(res.data.launchpadList)
                }
            } catch (err) {
                message.error(err?.msg || '获取数据失败')
            } finally {
                setLoading(false)
            }
        }
        fetchData()
    }, [message])

    // 没有数据时不渲染整个区块
    if (projects.length === 0) {
        return null
    }

    return (
        <div className={styles.section}>
            <div className={styles.header}>
                <div className={styles.titleWrapper}>
                    <h2 className={styles.title}>{intl.get('launchpad2_live_drops')}</h2>
                    <span className={styles.badge}>{projects.length}</span>
                </div>
            </div>

            <div className={styles.list}>
                {projects.map(project => (
                    <LaunchpadCard key={project.Id} project={project} status="live" />
                ))}
            </div>
        </div>
    )
}

export default LiveDrops
