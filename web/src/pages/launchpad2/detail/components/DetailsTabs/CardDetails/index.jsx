import React, { useMemo } from 'react'
import showdown from 'showdown'
import styles from './index.module.less'
import defaultText from './text.js'

const converter = new showdown.Converter()

const CardDetails = ({ detail }) => {
    const { launchpadInfo, cardInfo } = detail || {}
    const summary = launchpadInfo?.Summary || ''

    // 将 Markdown 转换为 HTML
    const htmlContent = useMemo(() => {
        // 优先使用 summary,如果 summary 为空或太短,使用默认 Markdown
        // const content = (summary && summary.length > 10) ? summary : defaultText.content
        const content = defaultText.content
        if (!content) return ''
        return converter.makeHtml(content)
    }, [summary])

    return (
        <div className={styles.cardDetails}>

            {htmlContent && (
                <div
                    className="markdown-content"
                    dangerouslySetInnerHTML={{ __html: htmlContent }}
                    style={{ maxWidth: '800px', marginBottom: '32px' }}
                />
            )}
        </div>
    )
}



export default CardDetails
