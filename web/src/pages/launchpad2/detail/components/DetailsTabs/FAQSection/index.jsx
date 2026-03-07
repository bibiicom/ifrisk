import React, { useState } from 'react'
import { DownOutlined } from '@ant-design/icons'
import intl from '@/component/intl'
import styles from './index.module.less'

const FAQSection = () => {
    const [openIndex, setOpenIndex] = useState(0)

    const faqs = [
        {
            question: "What is fractionalized ownership?",
            answer: "Fractionalization allows high-value physical assets to be divided into digital shares via smart contracts. By subscribing, you own a proportional legal stake in the underlying asset and its potential appreciation."
        },
        {
            question: "Where is the physical card stored?",
            answer: "All underlying assets are stored in professional third-party vaults protected by Class-3 security systems and fully insured for their full appraised value."
        },
        {
            question: "Can I trade my shares after the subscription ends?",
            answer: "Yes. Once finalized, shares (ERC-20) will be listed on the secondary marketplace, allowing you to buy or sell at market prices."
        }
    ]

    return (
        <div className={styles.faqSection}>
            <h3 className={styles.sectionTitle}>{intl.get('launchpad2_faq_title')}</h3>
            <div className={styles.faqList}>
                {faqs.map((faq, idx) => (
                    <div
                        key={idx}
                        className={`${styles.faqItem} ${openIndex === idx ? styles.open : ''}`}
                    >
                        <button
                            className={styles.faqQuestion}
                            onClick={() => setOpenIndex(openIndex === idx ? null : idx)}
                        >
                            <span>{faq.question}</span>
                            <DownOutlined className={styles.faqArrow} />
                        </button>
                        <div className={`${styles.faqAnswer} ${openIndex === idx ? styles.open : ''}`}>
                            <div>
                                <p>{faq.answer}</p>
                            </div>
                        </div>
                    </div>
                ))}
            </div>
        </div>
    )
}

export default FAQSection
