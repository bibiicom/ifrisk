import intl from '../component/intl'

export const TransferType = {
  DEPOSIT: '1',
  WITHDRAWAL: '2',
  createOptions () {
    return [
      { key: this.DEPOSIT, text: intl.get('deposit') },
      { key: this.WITHDRAWAL, text: intl.get('withdrawal') }
    ]
  },
  transfer (type) {
    switch (type) {
      case this.DEPOSIT: return intl.get('deposit')
      case this.WITHDRAWAL: return intl.get('withdrawal')
    }
  }
}
