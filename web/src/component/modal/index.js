import React from 'react'
import { createRoot } from 'react-dom/client'
import { ComponentContextProvider } from '../context'

import M from './Modal'

const modals = {}

// 获取当前主题
function getCurrentTheme() {
  const saved = localStorage.getItem('app-theme')
  if (saved === 'dark' || saved === 'light') {
    return saved
  }
  const legacySaved = localStorage.getItem('theme')
  if (legacySaved === 'dark' || legacySaved === 'light') {
    return legacySaved
  }
  return 'dark'
}

function close(id) {
  const modal = modals[id]
  if (modal) {
    modal.root.unmount() // 使用 root.unmount() 替代 unmountComponentAtNode
    delete modals[id]
  }
}

function open({ className, title, content, width, buttons = [], config }) {
  let uid = Date.now()
  const id = `${++uid}`

  // 创建真实 DOM 节点
  const div = document.createElement('div')
  document.body.appendChild(div)

  // 按钮点击处理
  const modalButtons = buttons.map(button => {
    const onClick = () => {
      const prevent = button.onClick && button.onClick()
      if (!prevent) {
        close(id)
      }
    }
    return { ...button, onClick }
  })

  // 自动获取当前主题
  const theme = getCurrentTheme()

  // 使用 createRoot 渲染
  const root = createRoot(div)
  root.render(
    <ComponentContextProvider theme={theme} {...config}>
      <M
        className={className}
        title={title}
        onClose={() => { close(id) }}
        width={width || 368}
        buttons={modalButtons}>
        {content}
      </M>
    </ComponentContextProvider>
  )

  modals[id] = { root, div } // 保存 root 和 div，以便卸载时清理
  return id
}

function confirm({ width, title, content, confirmButton, cancelButton, config }) {
  open({
    width,
    title,
    content,
    buttons: [
      { ...cancelButton, style: { width: 'auto' }, type: 'tertiary', text: true },
      { ...confirmButton, style: { width: 'auto' } }
    ],
    config
  })
}

const Modal = { confirm, open }

export default Modal
