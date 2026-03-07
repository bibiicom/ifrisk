import React, { useMemo } from 'react'
import intl from '@/component/intl'
import styles from './ActivityRules.module.less'
import { formatDecimals } from '@/utils/number'


const ActivityRules = ({ data }) => {
  const userBuy = 100 - (data?.FeePct || 0)

  const activityRulesData = useMemo(() => {
    // 格式化数据,确保有默认值
    const allVolume = formatDecimals(data?.AllVolume) || '--'
    const feePct = formatDecimals(data?.FeePct) || '--'
    const price = formatDecimals(data?.Price) || '--'

    // 处理最低和最高金额,如果数值为 0 则显示"无限制"
    const minPriceValue = parseFloat(data?.MinPrice || 0)
    const maxPriceValue = parseFloat(data?.MaxPrice || 0)
    
    const minPrice = minPriceValue === 0
      ? intl.get('unlimited') 
      : `${formatDecimals(data?.MinPrice) || '--'} USDC`
    
    const maxPrice = maxPriceValue === 0
      ? intl.get('unlimited')
      : `${formatDecimals(data?.MaxPrice) || '--'} USDC`

    // 格式化最大认购额度用于注意事项
    const maxPriceFormatted = maxPriceValue === 0
      ? intl.get('unlimited')
      : formatDecimals(data?.MaxPrice) || '--'

    return {
      // Basic information
      tokenName: data?.Currency || '',
      platform: intl.get('launchpad2_platform_name'),
      userBuy,
      maxPriceFormatted,

      // Rules content array with title and content
      sections: [
        {
          title: null,
          content: [
            { label: intl.get('launchpad2_token_name'), value: '{tokenName}' },
            { label: intl.get('launchpad2_issuer'), value: 'Scardex' },
            { label: intl.get('launchpad2_token_total'), value: `${allVolume} ${intl.get('launchpad2_token_total_suffix')}`.replace('{userBuy}', userBuy) },
            { label: intl.get('launchpad2_issue_price'), value: `${price} USDC / {tokenName}` },
            { label: intl.get('launchpad2_issue_platform'), value: '{platform}' },
            { label: intl.get('launchpad2_issue_fee'), value: `${feePct}%${intl.get('launchpad2_issue_fee_suffix')}` }
          ]
        },
        {
          title: intl.get('launchpad2_subscription_rules'),
          content: [
            intl.get('launchpad2_subscription_rule_1'),
            `${intl.get('launchpad2_subscription_rule_2_prefix')} **${minPrice}**${intl.get('launchpad2_subscription_rule_2_middle')} **${maxPrice}**`,
            intl.get('launchpad2_subscription_rule_3')
          ]
        },
        {
          title: intl.get('launchpad2_partner_exclusive'),
          content: [intl.get('launchpad2_partner_exclusive_desc')]
        },
        {
          title: intl.get('launchpad2_important_notes'),
          content: [
            intl.get('launchpad2_address_limit').replace('{max}', maxPriceFormatted),
            intl.get('launchpad2_final_interpretation')
          ]
        }
      ]
    }
  }, [data, userBuy])

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
      <h2 className={styles.rulesTitle}>{intl.get('launchpad2_activity_rules')}</h2>
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