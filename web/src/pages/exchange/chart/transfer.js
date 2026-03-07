import VolumeUnitType from '../../../constants/volumeUnitType'

export function transferResolution (resolution) {
  if (/^[\d]+$/.test(resolution)) {
    if ((+resolution) >= 60) {
      return `${(+resolution) / 60}h`
    }
    return `${resolution}m`
  }

  if (resolution.match('D')) {
    return resolution.length === 1 ? '1d' : resolution.toLowerCase()
  }
  if (resolution === 'W') {
    return '1w'
  }
  if (resolution === 'M') {
    return '1o'
  }
}

export function transferCandleCount (from, to, resolution) {
  if (/^[\d]+$/.test(resolution)) {
    return Math.floor((to - from) / (+resolution) / 60)
  }
  if (resolution === 'D') {
    return Math.floor((to - from) / (24 * 3600))
  }
  if (resolution === 'W') {
    return Math.floor((to - from) / (7 * 24 * 3600))
  }
  if (resolution === 'M') {
    return Math.floor((to - from) / (30 * 24 * 3600))
  }
}

export function splitRequest (from, to, resolution) {
  const params = []
  let interval = 0
  if (/^[\d]+$/.test(resolution)) {
    interval = 800 * (+resolution) * 60
  }
  if (resolution === 'D') {
    interval = 800 * 24 * 3600
  }
  if (resolution === 'W') {
    interval = 800 * 7 * 24 * 3600
  }
  if (resolution === 'M') {
    interval = 800 * 30 * 24 * 3600
  }
  for (let i = to; i >= from; i -= interval) {
    let count
    if (i - interval >= from) {
      count = 800
    } else {
      count = transferCandleCount(Math.max(from, i - interval), i, resolution)
    }
    if (count > 0) {
      params.push({ to: i, count })
    }
  }
  return params
}

export function transferData (data, params) {
  // console.log('data', data)
  // const timestamp = data.InsertTime
  const volume = +data[6]
  return {
    // timestamp: data.InsertTime,
    timestamp: data[0],
    // time: data[0],
    close: +data[4],
    open: +data[1],
    high: +data[2],
    low: +data[3],
    volume: params.symbol.IsInverse
      ? (params.volumeUnitType === VolumeUnitType.SHEET ? volume : +data[5])
      : VolumeUnitType.transferVolumeForUnit(params.volumeUnitType, { ...params, volume })
  }
}

export function transferData2 (data, params) {
  // console.log('data', data)
  // const timestamp = data.InsertTime
  const volume = +data[6]
  return {
    timestamp: data[0],
    // time: timestamp,
    close: +data[4],
    open: +data[1],
    high: +data[2],
    low: +data[3],
    volume: params.symbol.IsInverse
      ? (params.volumeUnitType === VolumeUnitType.SHEET ? volume : +data[5])
      : VolumeUnitType.transferVolumeForUnit(params.volumeUnitType, { ...params, volume })
  }
}
