import React, { useState } from 'react'
import styles from './index.module.less'

const manualTopics = [
  {
    id: 'getting-started',
    title: '新手入门',
    content: `
      <h3>欢迎来到卡片交易所！</h3>
      <p>在这里，您可以买卖各种集换式卡牌（如宝可梦、万智牌、游戏王等）以及体育球星卡。</p>
      <h4>第一步：完善资料</h4>
      <p>前往账户设置，完善您的个人资料和收货地址。准确的地址是确保您能顺利收到实体卡片的前提。</p>
      <h4>第二步：绑定收款方式</h4>
      <p>为了能够顺利提取您出售卡片所得的资金，请绑定您的银行卡或支持的加密钱包。</p>
      <h4>第三步：浏览市场</h4>
      <p>使用顶部的搜索栏或分类导航，寻找您心仪的卡片！您可以直接购买，或者提交一个"求购"报价。</p>
    `
  },
  {
    id: 'selling',
    title: '如何发布出售信息',
    content: `
      <h3>发布出售信息指南</h3>
      <p>在我们的平台上出售卡片非常简单：</p>
      <ol>
        <li>点击右上角的<strong>"出售"</strong>按钮。</li>
        <li>输入卡片名称，系统会自动匹配数据库中的卡片。</li>
        <li>选择卡片的语言、版本和品相（如 NM, LP, MP）。</li>
        <li>如果是评级卡，请选择评级机构（PSA, BGS, CGC等）并输入分数和认证编号。</li>
        <li>设定您的期望售价，或选择"接受报价"模式。</li>
        <li>上传清晰的卡片正反面照片（建议在充足的光线下拍摄，避免反光）。</li>
        <li>点击"发布"。您的卡片将立即展示在市场上。</li>
      </ol>
    `
  },
  {
    id: 'buyer-protection',
    title: '买家保护政策',
    content: `
      <h3>安全交易保障</h3>
      <p>我们致力于提供安全的交易环境。如果您遇到以下情况，您将受到我们的买家保护政策覆盖：</p>
      <ul>
        <li>收到的卡片与描述严重不符（例如：标明 NM 品相，实际有明显折痕）。</li>
        <li>收到的是假卡。</li>
        <li>包裹在运输途中丢失。</li>
      </ul>
      <h4>如何发起争议？</h4>
      <p>您可以在收到包裹后的 <strong>3 天内</strong>发起争议。请务必保留开箱视频作为证据。我们的仲裁团队将介入调查，并在确认问题后为您办理退款。</p>
    `
  },
  {
    id: 'vault',
    title: '金库服务 (Vault)',
    content: `
      <h3>什么是金库服务？</h3>
      <p>金库服务专为高频交易者和投资者设计。您可以将卡片存放在我们具备银行级安全措施的恒温恒湿金库中。</p>
      <h4>金库的优势：</h4>
      <ul>
        <li><strong>交易无需物流：</strong> 金库内的卡片交易瞬间完成所有权转移，无需邮寄，零运费，零等待。</li>
        <li><strong>节省运费：</strong> 您可以随时将多张卡片打包提取，节省高昂的单次运费。</li>
        <li><strong>高清扫描：</strong> 存入金库的卡片将获得免费的高清正反面扫描图，方便您随时查看或挂牌出售。</li>
        <li><strong>极致安全：</strong> 24小时监控，全额保险覆盖。</li>
      </ul>
    `
  }
]

const ManualSection = () => {
  const [activeTopicId, setActiveTopicId] = useState(manualTopics[0].id)

  const activeTopic = manualTopics.find(t => t.id === activeTopicId) || manualTopics[0]

  return (
    <div className={styles.container}>
      {/* Sidebar */}
      <div className={styles.sidebar}>
        <h3 className={styles.sidebarTitle}>目录</h3>
        <nav className={styles.nav}>
          {manualTopics.map(topic => (
            <button
              key={topic.id}
              onClick={() => setActiveTopicId(topic.id)}
              className={`${styles.navItem} ${activeTopic.id === topic.id ? styles.active : ''}`}
            >
              {topic.title}
            </button>
          ))}
        </nav>
      </div>

      {/* Content */}
      <div className={styles.content}>
        <h2 className={styles.title}>{activeTopic.title}</h2>
        <div 
          className={styles.article}
          dangerouslySetInnerHTML={{ __html: activeTopic.content }}
        />
      </div>
    </div>
  )
}

export default ManualSection
