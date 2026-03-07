import React, { useState } from 'react'
import styles from './index.module.less'

const faqs = [
  {
    category: '交易与费用',
    items: [
      { 
        q: '交易手续费是多少？', 
        a: '平台对每笔成功的交易收取 2.5% 的手续费。买方无需支付额外费用，手续费由卖方承担并从最终成交价中扣除。' 
      },
      { 
        q: '提现需要多长时间？', 
        a: '法币提现通常在 1-3 个工作日内到达您的银行账户。加密货币提现通常在 30 分钟内完成。' 
      },
      { 
        q: '什么是"求购"和"出售"指令？', 
        a: '"出售"指令是卖家设定的固定价格，买家可以直接购买。"求购"指令是买家设定的愿意支付的价格，卖家如果接受该价格即可立即成交。' 
      }
    ]
  },
  {
    category: '卡片鉴定与寄售',
    items: [
      { 
        q: '如何保证卡片的真实性？', 
        a: '所有在平台上标记为"已鉴定"的卡片均经过 PSA、BGS 或 CGC 等权威机构认证。对于未评级卡片，我们提供可选的第三方中介鉴定服务。' 
      },
      { 
        q: '我可以寄售我的卡片吗？', 
        a: '可以。您可以将高价值卡片邮寄至我们的金库。我们将为您提供高清扫描、安全存储，并在您出售时直接从金库发货。' 
      }
    ]
  },
  {
    category: '账户与安全',
    items: [
      { 
        q: '如何开启双重认证 (2FA)？', 
        a: '请前往"账户设置" -> "安全"，点击"开启 2FA"，使用 Google Authenticator 或 Authy 扫描二维码即可绑定。' 
      },
      { 
        q: '忘记密码怎么办？', 
        a: '在登录页面点击"忘记密码"，输入您的注册邮箱，我们将向您发送重置密码的链接。' 
      }
    ]
  }
]

const FAQSection = () => {
  const [openIndex, setOpenIndex] = useState(null)

  const toggleFAQ = (id) => {
    setOpenIndex(openIndex === id ? null : id)
  }

  return (
    <div className={styles.container}>
      <h2 className={styles.title}>常见问题</h2>
      
      <div className={styles.categories}>
        {faqs.map((category, catIdx) => (
          <div key={catIdx} className={styles.category}>
            <h3 className={styles.categoryTitle}>{category.category}</h3>
            <div className={styles.items}>
              {category.items.map((item, itemIdx) => {
                const id = `${catIdx}-${itemIdx}`
                const isOpen = openIndex === id
                return (
                  <div key={itemIdx} className={styles.faqItem}>
                    <button 
                      className={styles.question}
                      onClick={() => toggleFAQ(id)}
                    >
                      <span>{item.q}</span>
                      <span className={`${styles.icon} ${isOpen ? styles.open : ''}`}>
                        ▼
                      </span>
                    </button>
                    {isOpen && (
                      <div className={styles.answer}>
                        {item.a}
                      </div>
                    )}
                  </div>
                )
              })}
            </div>
          </div>
        ))}
      </div>
    </div>
  )
}

export default FAQSection
