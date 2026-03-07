import React, { useState } from 'react'
import styles from './index.module.less'

const mockTickets = [
  { 
    id: 'TK-8829', 
    subject: '提现未到账，已经超过3个工作日', 
    status: 'in-progress', 
    date: '2026-03-04', 
    type: '财务问题' 
  },
  { 
    id: 'TK-8710', 
    subject: '买家恶意取消订单', 
    status: 'resolved', 
    date: '2026-03-01', 
    type: '交易争议' 
  },
  { 
    id: 'TK-8502', 
    subject: '如何修改绑定的邮箱？', 
    status: 'resolved', 
    date: '2026-02-15', 
    type: '账户设置' 
  }
]

const TicketSection = () => {
  const [view, setView] = useState('list')

  return (
    <div className={styles.container}>
      {view === 'list' ? (
        <>
          <div className={styles.header}>
            <h2 className={styles.title}>我的工单</h2>
            <button 
              onClick={() => setView('new')}
              className={styles.newButton}
            >
              <span className={styles.plusIcon}>+</span>
              提交新工单
            </button>
          </div>

          <div className={styles.tableWrapper}>
            <table className={styles.table}>
              <thead>
                <tr>
                  <th>工单号</th>
                  <th>主题</th>
                  <th>类型</th>
                  <th>状态</th>
                  <th>更新时间</th>
                </tr>
              </thead>
              <tbody>
                {mockTickets.map((ticket) => (
                  <tr key={ticket.id}>
                    <td className={styles.ticketId}>{ticket.id}</td>
                    <td className={styles.subject}>{ticket.subject}</td>
                    <td>{ticket.type}</td>
                    <td>
                      <StatusBadge status={ticket.status} />
                    </td>
                    <td>{ticket.date}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </>
      ) : (
        <NewTicketForm onCancel={() => setView('list')} onSubmit={() => setView('list')} />
      )}
    </div>
  )
}

const StatusBadge = ({ status }) => {
  const statusConfig = {
    'open': { text: '待处理', className: styles.statusOpen },
    'in-progress': { text: '处理中', className: styles.statusProgress },
    'resolved': { text: '已解决', className: styles.statusResolved }
  }

  const config = statusConfig[status] || statusConfig['open']

  return (
    <span className={`${styles.statusBadge} ${config.className}`}>
      {config.text}
    </span>
  )
}

const NewTicketForm = ({ onCancel, onSubmit }) => {
  const [isSubmitting, setIsSubmitting] = useState(false)

  const handleSubmit = (e) => {
    e.preventDefault()
    setIsSubmitting(true)
    // 模拟提交
    setTimeout(() => {
      setIsSubmitting(false)
      onSubmit()
    }, 1000)
  }

  return (
    <div className={styles.formContainer}>
      <button 
        onClick={onCancel}
        className={styles.backButton}
      >
        ← 返回工单列表
      </button>

      <div className={styles.formCard}>
        <h2 className={styles.formTitle}>提交新工单</h2>
        
        <form onSubmit={handleSubmit} className={styles.form}>
          <div className={styles.formRow}>
            <div className={styles.formGroup}>
              <label htmlFor="type">问题类型</label>
              <select id="type" required>
                <option value="">请选择问题类型</option>
                <option value="trade">交易争议</option>
                <option value="finance">充值/提现问题</option>
                <option value="account">账户与安全</option>
                <option value="vault">金库服务</option>
                <option value="other">其他</option>
              </select>
            </div>
            <div className={styles.formGroup}>
              <label htmlFor="orderId">相关订单号 (选填)</label>
              <input 
                type="text" 
                id="orderId" 
                placeholder="例如: ORD-123456"
              />
            </div>
          </div>

          <div className={styles.formGroup}>
            <label htmlFor="subject">主题</label>
            <input 
              type="text" 
              id="subject" 
              placeholder="简短描述您的问题"
              required
            />
          </div>

          <div className={styles.formGroup}>
            <label htmlFor="description">详细描述</label>
            <textarea 
              id="description" 
              rows={6}
              placeholder="请详细描述您遇到的问题，以便我们更好地为您提供帮助..."
              required
            />
          </div>

          <div className={styles.formActions}>
            <button 
              type="button"
              onClick={onCancel}
              className={styles.cancelButton}
            >
              取消
            </button>
            <button 
              type="submit"
              disabled={isSubmitting}
              className={styles.submitButton}
            >
              {isSubmitting ? '提交中...' : '提交工单'}
            </button>
          </div>
        </form>
      </div>
    </div>
  )
}

export default TicketSection
