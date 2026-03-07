import React, { useState, useContext } from 'react'
import Row from '../../component/row'
import Button from '../../component/button'
import Message from '../../component/message'
import authHOC from '../common/hoc/authHOC'
import RootContext from '../rootContext'

import './faucet.less'

export default authHOC(true)(() => {
  const { userInfo } = useContext(RootContext)
  const address = userInfo?.WalletAddress || userInfo?.UserID || '' // 钱包地址

  const [tasks, setTasks] = useState([
    { id: 1, text: '完成一次交易', done: false },
    { id: 2, text: '邀请一个好友', done: false },
    { id: 3, text: '关注官方社交账号', done: false }
  ])

  const completeTask = (id) => {
    setTasks(prev =>
      prev.map(task => task.id === id ? { ...task, done: true } : task)
    )
  }

  const allTasksDone = tasks.every(task => task.done)

  const [loading, setLoading] = useState(false)
  const claimFaucet = async () => {
    if (!allTasksDone) return
    try {
      setLoading(true)
      // 调用发币接口
      await fakeFaucetApi(address)
      Message.ok({ message: '测试币领取成功！' })
      setTasks(prev => prev.map(t => ({ ...t, done: false }))) // 重置任务
    } catch (e) {
      Message.error({ message: `领取失败: ${e.message}` })
    } finally {
      setLoading(false)
    }
  }

  const [history, setHistory] = useState([
    { time: '2025-09-16 10:00', amount: 100 },
    { time: '2025-09-16 10:00', amount: 100 },
    { time: '2025-09-16 10:00', amount: 100 },
    { time: '2025-09-16 10:00', amount: 100 },
    { time: '2025-09-16 10:00', amount: 100 },
    { time: '2025-09-15 18:30', amount: 50 }
  ])

  // 模拟发币接口
  const fakeFaucetApi = async (walletAddress) => {
    return new Promise(resolve => setTimeout(resolve, 1000))
  }

  return (
    <section>
      <Row className="faucet">
        <div className="faucet-content-container" style={{ textAlign: 'center' }}>
          <h2>领币页面</h2>
          <p style={{ fontWeight: 'bold', wordBreak: 'break-all', textAlign: 'left' }}>
            您的钱包：{address}
          </p>

          <div className="tasks-list" style={{ marginTop: 20 }}>
            {tasks.map(task => (
              <div key={task.id} style={{ margin: 10 }}>
                <span style={{ marginRight: 10 }}>{task.text}</span>
                <Button
                  size="small"
                  disabled={task.done}
                  onClick={() => completeTask(task.id)}
                >
                  {task.done ? '已完成' : '去完成'}
                </Button>
              </div>
            ))}
          </div>

          <Button
            className="faucet-claim-button"
            size="big"
            loading={loading}
            disabled={!allTasksDone}
            style={{ marginTop: 20 }}
            onClick={claimFaucet}
          >
            领取USDC
          </Button>
          <div className="faucet-history">
            <h3>领取历史</h3>
            <ul>
              <li className="title">
                <span>时间</span>
                <span>数量</span>
              </li>
              {history.map((item, index) => (
                <li key={index}>
                  <span>{item.time}</span>
                  <span>{item.amount}</span>
                </li>
              ))}
            </ul>
          </div>
        </div>
      </Row>
    </section>
  )
})
