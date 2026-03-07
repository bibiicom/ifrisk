import React, { useState } from 'react'
import NavCard from './components/NavCard'
import FAQSection from './components/FAQSection'
import ManualSection from './components/ManualSection'
import TicketSection from './components/TicketSection'
import styles from './index.module.less'

const HelpCenter = () => {
  const [activeTab, setActiveTab] = useState('faq')

  return (
    <div className={styles.container}>
      {/* Header */}
      <header className={styles.header}>
        <div className={styles.headerContent}>
          <h1 className={styles.title}>帮助中心</h1>
          <p className={styles.subtitle}>
            无论您是新手还是资深藏家，我们都在这里为您提供帮助。
          </p>
        </div>
      </header>

      {/* Main Content */}
      <main className={styles.main}>
        {/* Navigation Cards */}
        <div className={styles.navCards}>
          <NavCard 
            title="常见问题 (FAQ)" 
            description="快速找到常见问题的解答"
            isActive={activeTab === 'faq'}
            onClick={() => setActiveTab('faq')}
          />
          <NavCard 
            title="使用手册" 
            description="详细的平台使用指南和规则"
            isActive={activeTab === 'manual'}
            onClick={() => setActiveTab('manual')}
          />
          <NavCard 
            title="工单系统" 
            description="联系客服解决您的专属问题"
            isActive={activeTab === 'tickets'}
            onClick={() => setActiveTab('tickets')}
          />
        </div>

        {/* Tab Content */}
        <div className={styles.content}>
          {activeTab === 'faq' && <FAQSection />}
          {activeTab === 'manual' && <ManualSection />}
          {activeTab === 'tickets' && <TicketSection />}
        </div>
      </main>
    </div>
  )
}

export default HelpCenter
