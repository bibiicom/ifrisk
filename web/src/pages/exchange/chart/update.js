import moment from 'moment-timezone'

import { transferData } from './transfer'

export default (
  volumeUnitType, currentSymbol, prevKLineData, lastKLineData, resolution, data, index, updateData
) => {
  // console.log('index', index)
  if (index === `${currentSymbol.ExchangeID}_${currentSymbol.InstrumentID}_1m` && prevKLineData.current) {
    const transferKLineData = transferData(data, { volumeUnitType, symbol: currentSymbol })
    if (!('timestamp' in prevKLineData.current)) {
      prevKLineData.current = transferKLineData
    }
    const last = lastKLineData || transferKLineData
    let isNext = false
    if (/^[\d]+$/.test(resolution)) {
      if (transferKLineData.timestamp - last.timestamp >= (+resolution) * 60 * 1000) {
        isNext = true
      }
    } else if (resolution === 'D') {
      if (transferKLineData.timestamp - last.timestamp >= 24 * 3600 * 1000) {
        isNext = true
      }
    } else if (resolution === 'W') {
      if (transferKLineData.timestamp - last.timestamp >= 7 * 24 * 3600 * 1000) {
        isNext = true
      }
    } else if (resolution === 'M') {
      if (moment.tz(last.timestamp, 'Etc/UTC').month() !== moment.tz(transferKLineData.timestamp, 'Etc/UTC').month()) {
        isNext = true
      }
    }
    let kLineData
    if (isNext) {
      kLineData = { ...transferKLineData, volume: 0 }
    } else {
      kLineData = { ...last }
    }
    if (
      prevKLineData.current.timestamp === transferKLineData.timestamp
      // moment.tz(prevKLineData.current.timestamp, 'Etc/UTC').minute() === moment.tz(transferKLineData.timestamp, 'Etc/UTC').minute()
    ) {
      kLineData.volume += (transferKLineData.volume - prevKLineData.current.volume)
    } else {
      kLineData.volume += transferKLineData.volume
    }
    kLineData.high = Math.max(kLineData.high, transferKLineData.high, kLineData.open, kLineData.close)
    kLineData.low = Math.min(kLineData.low, transferKLineData.low, kLineData.open, kLineData.close)
    kLineData.close = transferKLineData.close
    updateData(kLineData)
    prevKLineData.current = transferKLineData
  }
}
