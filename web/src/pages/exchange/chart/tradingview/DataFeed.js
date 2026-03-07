export default class DataFeed {
  constructor (resolve, getHistoryData, subscribe, unsubscribe) {
    this.resolve = resolve
    this.getHistoryData = getHistoryData
    this.subscribe = subscribe
    this.unsubscribe = unsubscribe
    this.resolution = '1'
  }

  onReady (cb) {
    setTimeout(() => {
      /* eslint-disable */
      cb({
        supported_resolutions: [
          1, 3, 5, 15, 30, 60, 120, 240, 360, 720, 'D', 'W', 'M'
        ],
        supports_marks: false,
        supports_timescale_marks: false,
        supports_time: true
      })
    }, 0)
  }

  resolveSymbol (symbolId, onSymbolResolvedCallback, onResolveErrorCallback) {
    const symbol = this.resolve(symbolId)
    setTimeout(() => {
      onSymbolResolvedCallback({
        ...symbol,
        timezone: 'Asia/Shanghai',
        has_intraday: true,
        has_weekly_and_monthly: true
      })
    }, 0)
  }

  getBars (symbolInfo, resolution, from, to, onHistoryCallback, onErrorCallback, firstDataRequest) {
    this.getHistoryData(symbolInfo, resolution, from, to, onHistoryCallback, onErrorCallback, firstDataRequest)
  }

  subscribeBars (symbolInfo, resolution, onRealtimeCallback, subscriberUID, onResetCacheNeededCallback) {
    this.resolution = resolution
    this.symbolInfo = symbolInfo
    this.subscribe(symbolInfo.ticker, symbolInfo.exchangeId, resolution, onRealtimeCallback)
  }

  unsubscribeBars () {
    this.unsubscribe(this.symbolInfo, this.resolution)
  }
}
