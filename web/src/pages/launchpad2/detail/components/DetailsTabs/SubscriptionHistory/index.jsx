import React, { useState, useEffect, useContext } from 'react'
import { Pagination, Spin } from 'antd'
import Decimal from 'decimal.js'
import dayjs from 'dayjs'
import utc from 'dayjs/plugin/utc'
import { fetchSubscriptionInfo } from '@/api/launchpad'
import RootContext from '@/pages/rootContext'
import intl from '@/component/intl'
import styles from './index.module.less'

dayjs.extend(utc)

const SubscriptionHistory = ({ detail }) => {
    const [filterMode, setFilterMode] = useState('all')
    const [records, setRecords] = useState([])
    const [loading, setLoading] = useState(false)
    const [pagination, setPagination] = useState({
        page: 1,
        limit: 10,
        total: 0
    })

    const { userInfo } = useContext(RootContext)
    const { launchpadInfo } = detail || {}
    const launchpadID = launchpadInfo?.Id

    // 获取当前用户的 AccountID
    const currentUserAccountID = userInfo?.MemberID || ''

    // 获取申购记录
    const fetchRecords = async () => {
        if (!launchpadID) return

        setLoading(true)
        try {
            const res = await fetchSubscriptionInfo({
                launchpadID,
                filter: filterMode === 'mine',
                page: pagination.page,
                limit: pagination.limit
            })

            if (res?.code === 0 && res?.data) {
                const list = res.data.list || []
                // 转换数据格式(使用高精度库)
                const formattedRecords = list.map(item => ({
                    id: item.id,
                    address: item.AccountID,
                    timestamp: dayjs(item.createdAt).utc().format('YYYY-MM-DD HH:mm:ss') + ' (UTC)',
                    shares: new Decimal(item.Volume || 0).toNumber(),
                    amount: new Decimal(item.TotalPrice || 0).toNumber()
                }))
                setRecords(formattedRecords)
                setPagination(prev => ({
                    ...prev,
                    total: res.data.total || 0
                }))
            }
        } catch (error) {
            console.error('获取申购记录失败:', error)
        } finally {
            setLoading(false)
        }
    }

    // 当 filterMode、pagination.page 或 launchpadID 变化时重新获取数据
    useEffect(() => {
        fetchRecords()
    }, [filterMode, pagination.page, launchpadID])

    // 处理分页变化
    const handlePageChange = (page) => {
        setPagination(prev => ({
            ...prev,
            page
        }))
    }

    return (
        <div className={styles.subscriptionHistory}>
            <div className={styles.historyHeader}>
                <h3 className={styles.sectionTitle}>{intl.get('launchpad2_subscription_history_title')}</h3>
                <div className={styles.filterTabs}>
                    <button
                        className={`${styles.filterTab} ${filterMode === 'all' ? styles.active : ''}`}
                        onClick={() => setFilterMode('all')}
                    >
                        {intl.get('launchpad2_all')}
                    </button>
                    <button
                        className={`${styles.filterTab} ${filterMode === 'mine' ? styles.active : ''}`}
                        onClick={() => setFilterMode('mine')}
                    >
                        {intl.get('launchpad2_mine')}
                    </button>
                </div>
            </div>

            <div className={styles.tableWrapper}>
                {loading ? (
                    <div className={styles.loading}>
                        <Spin size="large" />
                    </div>
                ) : records.length === 0 ? (
                    <div className={styles.empty}>{intl.get('launchpad2_no_records')}</div>
                ) : (
                    <>
                        <table className={styles.table}>
                            <thead>
                                <tr>
                                    <th>{intl.get('launchpad2_subscriber')}</th>
                                    <th>{intl.get('launchpad2_time')}</th>
                                    <th className={styles.alignRight}>{intl.get('launchpad2_shares')}</th>
                                    <th className={styles.alignRight}>{intl.get('launchpad2_total')}</th>
                                </tr>
                            </thead>
                            <tbody>
                                {records.map(record => (
                                    <tr key={record.id}>
                                        <td>
                                            <span className={styles.address}>{record.address}</span>
                                            {record.address === currentUserAccountID && (
                                                <span className={styles.youBadge}>{intl.get('launchpad2_you')}</span>
                                            )}
                                        </td>
                                        <td className={styles.timestamp}>{record.timestamp}</td>
                                        <td className={styles.alignRight}>{record.shares.toLocaleString()}</td>
                                        <td className={`${styles.alignRight} ${styles.amount}`}>
                                            ${record.amount.toLocaleString()}
                                        </td>
                                    </tr>
                                ))}
                            </tbody>
                        </table>
                        {pagination.total > 10 && (
                            <div className={styles.paginationWrapper}>
                                <Pagination
                                    current={pagination.page}
                                    pageSize={pagination.limit}
                                    total={pagination.total}
                                    onChange={handlePageChange}
                                    showSizeChanger={false}
                                    showTotal={(total) => intl.get('launchpad2_total_records').replace('{total}', total)}
                                />
                            </div>
                        )}
                    </>
                )}
            </div>
        </div>
    )
}

export default SubscriptionHistory
