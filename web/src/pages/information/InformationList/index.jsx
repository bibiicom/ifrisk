import React, { useState, useEffect, useCallback, useRef } from 'react'
import { useNavigate } from 'react-router-dom'
import { Input, Pagination, Spin, Empty } from 'antd'
import { SearchOutlined } from '@ant-design/icons'
import dayjs from 'dayjs'
import intl from '../../../component/intl'
import { getAnnouncement } from '../../../api/announcement'
import styles from './index.module.less'

const PAGE_SIZE = 10

const InformationList = ({ type, subTypeList, titleKey }) => {
    const navigate = useNavigate()

    const [activeCategory, setActiveCategory] = useState(subTypeList[0].key)
    const [searchValue, setSearchValue] = useState('')
    const [currentPage, setCurrentPage] = useState(1)
    const [articles, setArticles] = useState([])
    const [total, setTotal] = useState(0)
    const [loading, setLoading] = useState(false)
    const debounceTimer = useRef(null)

    // 获取公告列表
    const fetchArticles = useCallback(async () => {
        setLoading(true)
        try {
            const params = {
                page: currentPage,
                limit: PAGE_SIZE,
                title: searchValue || undefined,
                type: type,
                subType: Number(activeCategory),
            }

            const res = await getAnnouncement(params)
            if (res.code === 0) {
                setArticles(res.data?.announcementList || [])
                setTotal(res.data?.total || 0)
            }
        } catch (e) {
            console.error('获取列表失败:', e)
            setArticles([])
            setTotal(0)
        } finally {
            setLoading(false)
        }
    }, [currentPage, activeCategory, searchValue, type])

    useEffect(() => {
        fetchArticles()
    }, [fetchArticles])

    const handleCategoryChange = (key) => {
        setActiveCategory(key)
        setCurrentPage(1)
    }

    const handleSearch = (value) => {
        if (debounceTimer.current) {
            clearTimeout(debounceTimer.current)
        }
        debounceTimer.current = setTimeout(() => {
            setSearchValue(value)
            setCurrentPage(1)
        }, 500)
    }

    const handleArticleClick = (article) => {
        navigate(`/announcement/${article.Id}`)
    }

    const handlePageChange = (page) => {
        setCurrentPage(page)
    }

    return (
        <div className={styles.page}>
            <div className={styles.container}>
                {/* 头部 */}
                <div className={styles.header}>
                    <h1 className={styles.title}>{intl.get(titleKey)}</h1>
                    <Input
                        className={styles.search}
                        placeholder={intl.get('announcement_search')}
                        prefix={<SearchOutlined />}
                        allowClear
                        onChange={(e) => handleSearch(e.target.value)}
                    />
                </div>

                {/* 内容区 */}
                <div className={styles.content}>
                    {/* 左侧分类 */}
                    <div className={styles.sidebar}>
                        {subTypeList.map(cat => (
                            <div
                                key={cat.key}
                                className={`${styles.categoryItem} ${activeCategory === cat.key ? styles.categoryItemActive : ''}`}
                                onClick={() => handleCategoryChange(cat.key)}
                            >
                                {cat.labelKey ? intl.get(cat.labelKey) : cat.label}
                            </div>
                        ))}
                    </div>

                    {/* 右侧文章列表 */}
                    <div className={styles.list}>
                        {loading ? (
                            <div className={styles.loadingState}>
                                <Spin size="large" />
                            </div>
                        ) : articles.length > 0 ? (
                            articles.map(article => (
                                <div
                                    key={article.Id}
                                    className={styles.articleItem}
                                    onClick={() => handleArticleClick(article)}
                                >
                                    <span className={styles.articleTitle}>{article.Title}</span>
                                    <span className={styles.articleDate}>
                                        {dayjs(article.createdAt).format('YYYY-MM-DD')}
                                    </span>
                                </div>
                            ))
                        ) : (
                            <div className={styles.emptyState}>
                                <Empty description={intl.get('announcement_no_data')} />
                            </div>
                        )}

                        {/* 分页 */}
                        {total > PAGE_SIZE && (
                            <div className={styles.pagination}>
                                <Pagination
                                    current={currentPage}
                                    total={total}
                                    pageSize={PAGE_SIZE}
                                    onChange={handlePageChange}
                                    showSizeChanger={false}
                                />
                            </div>
                        )}
                    </div>
                </div>
            </div>
        </div>
    )
}

export default InformationList
