const VolumeUnitType = {
  SHEET: 'sheet',
  COIN: 'coin',
  coinToSheet: function ({ volume = '--', price = 1, symbol }) {
    if (volume === '--') {
      return volume
    }
    const volumeMultiple = symbol.VolumeMultiple || 1
    if (symbol.IsInverse) {
      return volume * price / volumeMultiple
    }
    return volume / volumeMultiple
  },
  sheetToCoin: function ({ volume = '--', price = 1, symbol }) {
    if (volume === '--') {
      return volume
    }
    const volumeMultiple = symbol.VolumeMultiple || 1
    if (symbol.IsInverse) {
      return volume * volumeMultiple / price
    }
    return volume * volumeMultiple
  },
  transferVolumeForUnit: function (unit, params) {
    if (unit === this.SHEET) {
      return params.volume
    }
    return this.sheetToCoin(params)
  }
}

export default VolumeUnitType
