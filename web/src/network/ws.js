import { getWsUrl } from './domain'

function Ws (suffix) {
  if (!('WebSocket' in window)) {
    console.warn('browser not support websocket!!!')
    return
  }

  const touch = () => {
    this.touchTimer = setTimeout(() => {
      if (this.readyState === WebSocket.OPEN) {
        touch()
      }
    }, 2 * 1000)
    this.ws.send('ping')
  }

  const reconnect = () => {
    this.ws = null
    if (this.touchTimer) {
      clearTimeout(this.touchTimer)
    }
    if (this.reconnectTimer) {
      clearTimeout(this.reconnectTimer)
    }
    this.reconnectTimer = setTimeout(() => {
      connect()
    }, 5000)
  }
  const connect = () => {
    const wsUrl = getWsUrl(suffix)
    this.ws = new WebSocket(wsUrl)
    this.readyState = WebSocket.CONNECTING
    this.ws.onopen = (e) => {
      this.readyState = WebSocket.OPEN
      touch()
      if (typeof this.openReceive === 'function') {
        this.openReceive(e)
      }
    }
    this.ws.onclose = (e) => {
      this.readyState = WebSocket.CLOSED
      if (typeof this.closeReceive === 'function') {
        this.closeReceive(e)
      }
      if (!this.forceClose) {
        reconnect()
      }
    }
    this.ws.onmessage = (e) => {
      try {
        const data = JSON.parse(e.data)
        this.messageReceive(data)
      } catch (e) {
      }
    }
    this.ws.onerror = (e) => {
      this.readyState = WebSocket.CLOSED
      if (typeof this.errorReceive === 'function') {
        this.errorReceive(e)
      }
      reconnect()
    }
  }
  connect()
}

Ws.prototype.onOpen = function (func) {
  this.openReceive = func
}

Ws.prototype.onMessage = function (func) {
  this.messageReceive = func
}

Ws.prototype.onClose = function (func) {
  this.closeReceive = func
}

Ws.prototype.onError = function (func) {
  this.errorReceive = func
}

Ws.prototype.disconnect = function () {
  if (this.ws) {
    this.readyState = WebSocket.CLOSING
    this.forceClose = true
    this.ws.close(3333, 'close')
  }
}

Ws.prototype.sendData = function (data = {}) {
  if (this.ws && this.readyState === WebSocket.OPEN) {
    this.ws.send(JSON.stringify(data))
  }
}

const createWs = (suffix = 'swap') => {
  return new Ws(suffix)
}

export default createWs
