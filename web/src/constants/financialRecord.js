import intl from '../component/intl'

export const FlowType = {
  ALL: '',
  NONE: '0',
  PROFIT: '1',
  MONEY_CHANGE: '2',
  MONEY_IN: '3',
  MONEY_OUT: '4',
  FEE: '5',
  KEEP_FEE: '6',
  FUNDING_RATE: '7',
  SETTLEMENT_PROFIT: '8',
  DELIVERY_PROFIT: '9',
  LIQUIDATION: 'a',
  // 保留资金获取
  ReserveDeposit: 'b',
  // 保留资金转出
  ReserveWithdraw: 'c',
  // 保留资金盈利回收
  ReserveProfit: 'd',
  createSpotOptions () {
    return [
      { key: this.ALL, text: intl.get('all') },
      { key: this.MONEY_IN, text: intl.get('capital_in') },
      { key: this.MONEY_OUT, text: intl.get('capital_out') },
      { key: this.FEE, text: intl.get('fee') }
    ]
  },
  createSwapContractOptions () {
    return [
      { key: this.ALL, text: intl.get('all') },
      { key: this.PROFIT, text: intl.get('realized_profit_loss') },
      { key: this.MONEY_IN, text: intl.get('capital_in') },
      { key: this.MONEY_OUT, text: intl.get('capital_out') },
      { key: this.FEE, text: intl.get('fee') },
      { key: this.FUNDING_RATE, text: intl.get('capital_fee') },
      { key: this.SETTLEMENT_PROFIT, text: intl.get('settlement_profit') },
      { key: this.LIQUIDATION, text: intl.get('force_liquidation') }
    ]
  },
  createDeliveryContractOptions () {
    return [
      { key: this.ALL, text: intl.get('all') },
      { key: this.PROFIT, text: intl.get('realized_profit_loss') },
      { key: this.MONEY_IN, text: intl.get('capital_in') },
      { key: this.MONEY_OUT, text: intl.get('capital_out') },
      { key: this.FEE, text: intl.get('fee') },
      { key: this.FUNDING_RATE, text: intl.get('capital_fee') },
      { key: this.SETTLEMENT_PROFIT, text: intl.get('settlement_profit') },
      { key: this.DELIVERY_PROFIT, text: intl.get('delivery_profit') },
      { key: this.LIQUIDATION, text: intl.get('force_liquidation') }
    ]
  },
  transfer (type) {
    switch (type) {
      case this.ALL: return intl.get('all')
      case this.NONE: return intl.get('normal')
      case this.PROFIT: return intl.get('realized_profit_loss')
      case this.MONEY_CHANGE: return intl.get('capital_change')
      case this.MONEY_IN: return intl.get('capital_in')
      case this.MONEY_OUT: return intl.get('capital_out')
      case this.FEE: return intl.get('fee')
      case this.KEEP_FEE: return intl.get('keep_fee')
      case this.FUNDING_RATE: return intl.get('capital_fee')
      case this.SETTLEMENT_PROFIT: return intl.get('settlement_profit')
      case this.DELIVERY_PROFIT: return intl.get('delivery_profit')
      case this.LIQUIDATION: return intl.get('force_liquidation')
      default: return intl.get('normal')
    }
  }
}
