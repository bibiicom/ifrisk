import React, { useState, useEffect } from 'react'
import { useParams, useNavigate } from 'react-router-dom'
import { Spin } from 'antd'
import { LeftOutlined } from '@ant-design/icons'
import dayjs from 'dayjs'
import showdown from 'showdown'
import intl, { ZH_CN } from '../../../component/intl'
import { getAnnouncementDetail } from '../../../api/announcement'
import authHOC from '../../common/hoc/authHOC'

import styles from './index.module.less'

const converter = new showdown.Converter()

const ArticleDetail = () => {
    const { id } = useParams()
    const navigate = useNavigate()
    const [article, setArticle] = useState(null)
    const [htmlContent, setHtmlContent] = useState('')
    const [loading, setLoading] = useState(false)

    useEffect(() => {
        if (!id) return

        const fetchDetail = async () => {
            setLoading(true)
            try {
                const res = await getAnnouncementDetail({ id })
                if (res.code === 0 && res.data) {
                    setArticle(res.data)
                    if (res.data.Content) {
                        const html = converter.makeHtml(res.data.Content)
                        setHtmlContent(html)
                    }
                }
            } catch (e) {
                console.error('获取公告详情失败:', e)
            } finally {
                setLoading(false)
            }
        }

        fetchDetail()
    }, [id])

    const handleBack = () => {
        navigate(-1)
    }

    const formatDate = (date) => {
        if (intl.language === ZH_CN) {
            return dayjs(date).format('YYYY 年 M 月 D 日')
        }
        return dayjs(date).format('MMMM D, YYYY')
    }

    if (loading) {
        return (
            <div className={styles.page}>
                <div className={styles.container}>
                    <div className={styles.loading}>
                        <Spin size="large" />
                    </div>
                </div>
            </div>
        )
    }

    return (
        <div className={styles.page}>
            <div className={styles.container}>
                <div className={styles.detailHeader}>
                    <div className={styles.backBtn} onClick={handleBack}>
                        <LeftOutlined />
                        <span>{intl.get('announcement_back')}</span>
                    </div>
                    {article && (
                        <>
                            <h1 className={styles.detailTitle}>{article.Title}</h1>
                            <span className={styles.detailDate}>
                                {intl.get('announcement_published_at')} {formatDate(article.createdAt)}
                            </span>
                        </>
                    )}
                </div>
                <div
                    className="markdown-content"
                    dangerouslySetInnerHTML={{ __html: htmlContent }}
                    style={{ maxWidth: '900px', margin: '0 auto' }}
                />
            </div>
        </div>
    )
}

export default authHOC(false)(ArticleDetail)
