import React from 'react'
import styles from './ActivityRules.module.less'
import { formatDecimals } from '@/utils/number'


const ActivityRules = ({ data }) => {



  const activityRulesData = {
    // 基础信息
    tokenName: data?.Name || '',
    platform: 'Scardex 平台',

    // 规则内容数组，只有 title 和 content
    sections: [
      {
        title: null,
        content: [
          { label: '代币名称', value: '{tokenName}' },
          { label: '发行方', value: 'Scardex' },
          { label: '代币总量', value: `${formatDecimals(data?.AllVolume)}（其中 92% 开于 Launchpad 公开发售）` },
          { label: '发行单价', value: '0.05 USDT / {tokenName}' },
          { label: '发行承销方', value: '{platform}' },
          { label: '发行费', value: '2%（由 {platform} 持有）' }
        ]
      },
      {
        title: '认购规则',
        content: [
          '1.本期 Launchpad 开启后，您可实时查看每个账户已参与认购，认购期间，将随机开放 3 轮认购阶段，先到先得，售完即止。',
          `2.最少申购价格为${formatDecimals(data.MinPrice)},申购价格上限为${formatDecimals(data.MaxPrice)}`,
          '3.若库存不足，则会退回申购费用'
        ]
      },
      {
        title: '合作方专属',
        content: ['成为 Scardex 指定合作方，可获得专属认购额度']
      },
      {
        title: '注意事项',
        content: [
          '单个地址认购额度上限为 20000 {tokenName}',
          '本活动最终解释权归于 {platform} 所有'
        ]
      }
    ]
  }

  const { tokenName, platform, sections } = activityRulesData

  // 解析文本中的变量占位符
  const parseText = (text) => {
    if (!text) return text
    return text
      .replace(/{tokenName}/g, tokenName)
      .replace(/{platform}/g, platform)
  }

  // 渲染带有 **bold** 标记的文本
  const renderTextWithBold = (text) => {
    const parsed = parseText(text)
    const parts = parsed.split(/(\*\*.*?\*\*)/g)
    return parts.map((part, index) => {
      if (part.startsWith('**') && part.endsWith('**')) {
        return <strong key={index}>{part.slice(2, -2)}</strong>
      }
      return part
    })
  }

  // 渲染内容项
  const renderContentItem = (item, index) => {
    // 带 label/value 的对象
    if (item.label) {
      return (
        <div key={index} className={styles.contentItem}>
          <span className={styles.label}>{item.label}：</span>
          <span className={styles.value}>{renderTextWithBold(item.value)}</span>
        </div>
      )
    }
    // 纯文本
    return (
      <p key={index} className={styles.contentText}>
        {renderTextWithBold(item)}
      </p>
    )
  }

  return (
    <div className={styles.rulesSection}>
      <h2 className={styles.rulesTitle}>活动规则</h2>
      <div className={styles.rulesContent}>
        {sections.map((section, sectionIndex) => (
          <div key={sectionIndex} className={styles.sectionBlock}>
            {section.title && <h3 className={styles.sectionTitle}>{section.title}</h3>}
            <div className={styles.sectionContent}>
              {section.content.map((item, index) => renderContentItem(item, index))}
            </div>
          </div>
        ))}
      </div>
    </div>
  )
}

export default ActivityRules
