import intl from '../component/intl'

export const ProductGroup = {
  INDEX: 'Index',
  SPOT: 'SPOT',
  SPOTC: 'SPOTC',
  CRYPTO: 'Crypto',
  REVERSE_DELIVERY: 'FTUR',
  POSITIVE_DELIVERY: 'FTURU',
  REVERSE_SWAP: 'Swap',
  POSITIVE_SWAP: 'SwapU',
  OPTION: 'Options',
  STOCK: 'Stock',
  BOND: 'Bond',
  FOREX: 'Forex',
  CRYPTOCURRENCY: 'cryptocurrency',
  COMM: 'Comm',
  COMM_M: 'CommM',
  COMM_N: 'CommN',
  COMM_O: 'CommO',
  STOCK_A: 'StockA',
  STOCK_HK: 'StockHK',
  STOCK_US: 'StockUS',
  createClassify () {
    return [
      // { key: this.CRYPTOCURRENCY, text: intl.get('cryptocurrency') },
      // { key: this.STOCK, text: intl.get('stock') },
      // { key: this.BOND, text: intl.get('bond') },
      // { key: this.FOREX, text: intl.get('forex') },
      // { key: this.COMM, text: intl.get('comm') },
      {
        key: this.CRYPTOCURRENCY,
        text: intl.get('cryptocurrency'),
        children: [
          { key: this.SPOTC, text: 'NBA' },
          { key: this.SPOT, text: 'FIFA' },
          { key: this.CRYPTO, text: 'MLB' },
          { key: this.POSITIVE_SWAP, text: "NFL" },
          { key: this.REVERSE_SWAP, text: "NHL" },
          //---------------
          // { key: this.SPOTC, text: intl.get('coin_usdc') },
          // { key: this.SPOT, text: intl.get('coin_usdt') },
          // { key: this.CRYPTO, text: intl.get('coin_crypto') },
          // { key: this.POSITIVE_SWAP, text: intl.get('coin_swapu') },
          // { key: this.REVERSE_SWAP, text: intl.get('coin_swap') },
          // { key: this.POSITIVE_DELIVERY, text: intl.get('coin_deliveryu') },
          // { key: this.REVERSE_DELIVERY, text: intl.get('coin_delivery') },
          // { key: this.OPTION, text: intl.get('option') }
        ]
      },
      {
        key: this.STOCK,
        text: intl.get('stock'),
        children: [
          { key: this.STOCK_A, text: intl.get('stock_a') },
          { key: this.STOCK_HK, text: intl.get('stock_hk') },
          { key: this.STOCK_US, text: intl.get('stock_us') }
        ]
      },
      {
        key: this.COMM,
        text: intl.get('comm'),
        children: [
          { key: this.COMM_M, text: intl.get('comm_m') },
          { key: this.COMM_O, text: intl.get('comm_o') },
          { key: this.COMM_N, text: intl.get('comm_n') }
        ]
      },
      {
        key: this.BOND,
        text: intl.get('bond'),
        children: [
          { key: this.STOCK_A, text: intl.get('stock_a') },
          { key: this.STOCK_HK, text: intl.get('stock_hk') },
          { key: this.STOCK_US, text: intl.get('stock_us') }
        ]
      },
      {
        key: this.FOREX,
        text: intl.get('forex'),
        children: [
          { key: this.STOCK_A, text: intl.get('stock_a') },
          { key: this.STOCK_HK, text: intl.get('stock_hk') },
          { key: this.STOCK_US, text: intl.get('stock_us') }
        ]
      }
    ]
  },
  createTradeOptions () {
    return [
      { key: this.SPOTC, text: 'NBA' },
      { key: this.SPOT, text: 'FIFA' },
      { key: this.CRYPTO, text: 'MLB' },
      { key: this.REVERSE_DELIVERY, text: 'NFL' },
      { key: this.POSITIVE_DELIVERY, text: 'NHL' },
      //---------------------
      // { key: this.SPOTC, text: intl.get('coin_usdc') },
      // { key: this.SPOT, text: intl.get('coin_usdt') },
      // { key: this.CRYPTO, text: intl.get('coin_crypto') },
      // { key: this.REVERSE_DELIVERY, text: intl.get('coin_delivery') },
      // { key: this.REVERSE_SWAP, text: intl.get('coin_swap') },
      // { key: this.POSITIVE_DELIVERY, text: intl.get('coin_deliveryu') },
      // { key: this.POSITIVE_SWAP, text: intl.get('coin_swapu') }
      // { key: this.OPTION, text: intl.get('option') }
      // { key: this.BOND, text: intl.get('bond') },
      // { key: this.STOCK, text: intl.get('stock') },
      // { key: this.FOREX, text: intl.get('forex') },
      // { key: this.COMM, text: intl.get('comm') }
    ]
  },
  createContractOptions () {
    return [
      { key: this.REVERSE_DELIVERY, text: intl.get('coin_delivery') },
      { key: this.REVERSE_SWAP, text: intl.get('coin_swap') },
      { key: this.POSITIVE_DELIVERY, text: intl.get('usdt_delivery') },
      { key: this.POSITIVE_SWAP, text: intl.get('usdt_swap') },
      { key: this.OPTION, text: intl.get('option') }
    ]
  },
  createSwapContractOptions () {
    return [
      { key: this.REVERSE_SWAP, text: intl.get('coin_swap') },
      { key: this.POSITIVE_SWAP, text: intl.get('usdt_swap') }
    ]
  },
  createFutureContractOptions () {
    return [
      { key: this.POSITIVE_SWAP, text: intl.get('coin_swapu') },
      { key: this.REVERSE_SWAP, text: intl.get('coin_swap') },
      { key: this.REVERSE_DELIVERY, text: intl.get('coin_delivery') },
      { key: this.POSITIVE_DELIVERY, text: intl.get('coin_deliveryu') }
    ]
  },
  isOpenClose (productGroup) {
    switch (productGroup) {
      case this.REVERSE_DELIVERY:
      case this.POSITIVE_DELIVERY:
      case this.POSITIVE_SWAP:
      case this.REVERSE_SWAP:
        return true
      default:
        return false
    }
  },
  transferText (productGroup) {
    switch (productGroup) {
      case this.SPOTC: return intl.get('coin_usdc')
      case this.SPOT: return intl.get('coin_usdt')
      case this.CRYPTO: return intl.get('coin_crypto')
      case this.REVERSE_DELIVERY: return intl.get('coin_delivery')
      case this.POSITIVE_DELIVERY: return intl.get('coin_deliveryu')
      case this.POSITIVE_SWAP: return intl.get('coin_swapu')
      case this.REVERSE_SWAP: return intl.get('coin_swap')
      // case this.OPTION: return intl.get('option')
      // case this.COMM: return intl.get('comm')
      // case this.STOCK: return intl.get('stock')
      // case this.BOND: return intl.get('bond')
      // case this.FOREX: return intl.get('forex')
    }
  },
  tabAll () {
    return [
      this.SPOTC,
      this.SPOT,
      this.REVERSE_SWAP,
      this.POSITIVE_SWAP,
      this.REVERSE_DELIVERY,
      this.POSITIVE_DELIVERY
    ]
  }
}

export const SettlementGroup = {
  SG1: 'SG1',
  ETH: 'ETH',
  TON: 'TON',
  BASE: 'BASE',
  SOL: 'SOL',
  ARB: 'ARB',
  ETHW: 'ETHW',
  createOptions () {
    return [
      { key: this.ARB, text: 'ArbitrumOne' },
      { key: this.TON, text: 'TON' },
      { key: this.SG1, text: intl.get('sg1') },
      { key: this.ETH, text: 'Ethereum' },
      { key: this.BASE, text: 'Base' },
      { key: this.SOL, text: 'Solana' },
      { key: this.ETHW, text: 'EthereumPoW' }
    ]
  },
  options (fromTo) {
    if (!this.isChain(fromTo)) {
      return [
        { key: this.ARB, text: 'ArbitrumOne' },
        { key: this.TON, text: 'TON' },
        { key: this.ETH, text: 'Ethereum' },
        { key: this.BASE, text: 'Base' },
        { key: this.SOL, text: 'Solana' },
        { key: this.ETHW, text: 'EthereumPoW' }
      ]
    } else {
      return [
        { key: this.SG1, text: intl.get('sg1') }
      ]
    }
  },
  transferText (settlementGroup) {
    switch (settlementGroup) {
      case this.SG1: return intl.get('sg1')
      case this.ARB: return 'ArbitrumOne'
      case this.TON: return 'TON'
      case this.ETH: return 'Ethereum'
      case this.BASE: return 'Base'
      case this.SOL: return 'Solana'
      case this.ETHW: return 'EthereumPoW'
    }
  },
  isChain (settlementGroup) {
    switch (settlementGroup) {
      case this.SG1:
        return false
      default:
        return true
    }
  }
}

export const DeliveryPeriod = {
  CURRENT_WEEK: 'CW',
  NEXT_WEEK: 'NW',
  CURRENT_QUARTER: 'CS',
  NEXT_QUARTER: 'NS',
  SWAP: 'SWAP',
  transfer: function (period) {
    if (period.match(this.CURRENT_WEEK)) {
      return intl.get('current_week')
    }
    if (period.match(this.NEXT_WEEK)) {
      return intl.get('next_week')
    }
    if (period.match(this.CURRENT_QUARTER)) {
      return intl.get('current_quarter')
    }
    if (period.match(this.NEXT_QUARTER)) {
      return intl.get('next_quarter')
    }
    return intl.get('swap')
  }
}

export function transferName (symbol) {
  if (symbol) {
    //   if (symbol?.ProductGroup === ProductGroup.SPOT) {
    //     return `${symbol.BaseCurrency}/${symbol.PriceCurrency}`
    //   }
    //   if (
    //     symbol?.ProductGroup === ProductGroup.REVERSE_SWAP ||
    //     symbol?.ProductGroup === ProductGroup.POSITIVE_SWAP ||
    //     symbol?.ProductGroup === ProductGroup.REVERSE_DELIVERY ||
    //     symbol?.ProductGroup === ProductGroup.POSITIVE_DELIVERY
    //   ) {
    //     return `${symbol.BaseCurrency}/${symbol.PriceCurrency}`
    //   }
    if (symbol.InstrumentName) {
      return symbol.InstrumentName
    } else {
      return symbol.InstrumentID
    }
  } else {
    return '--'
  }
}
