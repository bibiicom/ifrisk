import React, { PureComponent, createRef } from 'react'
import { createRoot } from 'react-dom/client'

import { ComponentContextProvider } from '../context'
import Message from './Message'

export default class Container extends PureComponent {
  constructor(props) {
    super(props)
    this.messageContainerRef = createRef()
    this.messages = {}
  }

  addMessage = (options) => {
    const div = document.createElement('div')
    div.classList.add('message-wrapper')

    // 使用 createRoot 代替 ReactDOM.render
    const root = createRoot(div)
    root.render(
      <ComponentContextProvider {...options.config}>
        <Message onClose={this.removeMessage} {...options}/>
      </ComponentContextProvider>
    )

    this.messages[options.id] = { dom: div, root, options }

    this.messageContainerRef.current && this.messageContainerRef.current.appendChild(div)

    const timer = setTimeout(() => {
      clearTimeout(timer)
      div.classList.add('message-wrapper-enter')
    }, 100)
  }

  removeMessage = (id) => {
    const message = this.messages[id]
    if (message) {
      const { dom, root } = message
      dom.classList.remove('message-wrapper-enter')
      dom.classList.add('message-wrapper-leave')
      const timer = setTimeout(() => {
        clearTimeout(timer)
        // 使用 root.unmount() 代替 ReactDOM.unmountComponentAtNode
        root.unmount()
        this.messageContainerRef.current && this.messageContainerRef.current.removeChild(dom)
        delete this.messages[id]
      }, 300)
    }
  }

  render() {
    return (
      <div ref={this.messageContainerRef} className="funny-message-container">
      </div>
    )
  }
}
