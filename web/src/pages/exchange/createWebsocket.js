import { isValid, isFunction } from '../../component/utils/typeChecks'

import connectWs from '../../network/ws'

import { Topic } from '../../api/contract'

export const WebsocketStatus = {
  CONNECT: 'connect',
  DISCONNECT: 'disconnect'
}

export const WebsocketMessageType = {
  STATUS: 'status',
  K_LINE: 'k_line',
  REAL_DEAL: 'real_deal',
  MARKET_OVERVIEW: 'market_overview',
  MARKET_DETAIL: 'market_detail',
  ORDER_BOOK: 'order_book',
  DEPTH: 'depth'
}

export const WebsocketDataType = {
  PUSH: '0',
  SUB: '1',
  UNSUB: '2'
}

class WebSocketClient {
  constructor () {
    this.callbacks = {}
    this.status = WebsocketStatus.DISCONNECT
    this.ws = connectWs()
    this.ws.onOpen(() => {
      this.status = WebsocketStatus.CONNECT
      this.dispatch(this.callbacks[WebsocketMessageType.STATUS], WebsocketStatus.CONNECT)
    })
    this.ws.onMessage((message) => {
      const action = message.action
      const topic = message.topic
      const index = message.index
      const result = message.result || []
      // console.log('message', message)
      let actionCallbacks
      let dispatchData
      if (result.length > 0) {
        switch (action) {
          case 'RecvTopicAction': {
            const data = result[0].data || {}
            dispatchData = { type: data.Action, code: message.errorCode }
            switch (data.TopicID) {
              case Topic.K_LINE: {
                actionCallbacks = this.callbacks[WebsocketMessageType.K_LINE]
                break
              }
              case Topic.MARKET_DEPTH_25: {
                actionCallbacks = this.callbacks[WebsocketMessageType.ORDER_BOOK]
                break
              }
              case Topic.MARKET_DEPTH_ALL: {
                actionCallbacks = this.callbacks[WebsocketMessageType.DEPTH]
                break
              }
              case Topic.TRADE_DETAIL: {
                actionCallbacks = this.callbacks[WebsocketMessageType.REAL_DEAL]
                break
              }
              case Topic.MARKET_OVERVIEW: {
                actionCallbacks = this.callbacks[WebsocketMessageType.MARKET_OVERVIEW]
                break
              }
              case Topic.MARKET_DETAIL: {
                actionCallbacks = this.callbacks[WebsocketMessageType.MARKET_DETAIL]
                break
              }
            }
            break
          }
          case 'PushKLine': {
            actionCallbacks = this.callbacks[WebsocketMessageType.K_LINE]
            dispatchData = { index: index, type: WebsocketDataType.PUSH, result: result[0] }
            break
          }
          case 'PushMarketOverView': {
            if (topic === Topic.MARKET_OVERVIEW) {
              const dataList = []
              // console.log('result1', result)
              const data = result
              result.forEach(data => {
                const market = data
                if (market) {
                  const open = market.O
                  const close = market.N
                  if (open) {
                    market.Change = (close - open) / open
                  }
                  dataList.push(market)
                }
              })
              if (dataList.length > 0) {
                actionCallbacks = this.callbacks[WebsocketMessageType.MARKET_OVERVIEW]
                dispatchData = { type: WebsocketDataType.PUSH, result: dataList }
              }
            } else {
              const market = result[0]
              if (market) {
                const open = market.O
                const close = market.N
                if (open) {
                  market.Change = (close - open) / open
                }
                actionCallbacks = this.callbacks[WebsocketMessageType.MARKET_DETAIL]
                dispatchData = { type: WebsocketDataType.PUSH, result: market }
              }
            }
            break
          }
          case 'PushMarketOrder': {
            const dataList = []
            // console.log('result', result)
            result.forEach(data => {
              dataList.push(data)
            })
            // console.log('dataList', dataList)
            if (dataList.length > 0) {
              if (topic === Topic.MARKET_DEPTH_ALL) {
                actionCallbacks = this.callbacks[WebsocketMessageType.DEPTH]
              } else {
                actionCallbacks = this.callbacks[WebsocketMessageType.ORDER_BOOK]
              }
              dispatchData = { index: index, type: WebsocketDataType.PUSH, result: dataList }
            }
            break
          }
          case 'PushMarketTrade': {
            const dataList = []
            result.forEach(data => {
              dataList.push(data)
            })
            // console.log('dataList', dataList)
            if (dataList.length > 0) {
              actionCallbacks = this.callbacks[WebsocketMessageType.REAL_DEAL]
              dispatchData = { index: message.index, type: WebsocketDataType.PUSH, result: dataList }
            }
            break
          }
        }
        this.dispatch(actionCallbacks, dispatchData)
      }
    })
    this.ws.onClose(() => {
      this.status = WebsocketStatus.DISCONNECT
      this.dispatch(this.callbacks[WebsocketMessageType.STATUS], WebsocketStatus.DISCONNECT)
    })
  }

  dispatch (callbacks, data) {
    if (callbacks) {
      for (const key in callbacks) {
        const callback = callbacks[key]
        isFunction(callback) && callback(data)
      }
    }
  }

  on (type, tag, callback) {
    if (isValid(type) && isValid(tag) && isFunction(callback)) {
      if (!this.callbacks[type]) {
        this.callbacks[type] = {}
      }
      this.callbacks[type][tag] = callback
    }
  }

  off (type, tag) {
    if (isValid(type)) {
      if (this.callbacks[type]) {
        if (isValid(tag)) {
          delete this.callbacks[type][tag]
        } else {
          delete this.callbacks[type]
        }
      }
    } else {
      this.callbacks = {}
    }
  }

  send (data) {
    if (this.status === WebsocketStatus.CONNECT) {
      this.ws.sendData(data)
    }
  }

  disconnect () {
    this.off()
    if (this.status === WebsocketStatus.CONNECT) {
      this.ws.disconnect()
    }
  }
}

export default function createWebsocket () {
  return new WebSocketClient()
}
