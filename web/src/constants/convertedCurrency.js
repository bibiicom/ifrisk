import intl from '../component/intl'
import { formatValue } from '../utils/format'

export const ConvertedCurrency = {
  CNY: 'cny',
  USD: 'usd',
  GBP: 'gbp',
  EUR: 'eur',
  HKD: 'hkd',
  KRW: 'krw',
  createOptions () {
    return [
      { key: this.CNY, text: '¥ CNY' },
      { key: this.USD, text: '$ USD' },
      { key: this.GBP, text: '£ GBP' },
      { key: this.EUR, text: '€ EUR' },
      { key: this.HKD, text: 'HK$ HKD' },
      { key: this.KRW, text: '₩ KRW' }
    ]
  },
  transferUnit (currency) {
    switch (currency) {
      case this.CNY: return '¥'
      case this.USD: return '$'
      case this.GBP: return '£'
      case this.EUR: return '€'
      case this.HKD: return 'HK$'
      case this.KRW: return '₩'
    }
  },
  transferKey (currency) {
    switch (currency) {
      case this.CNY: return '¥ CNY'
      case this.USD: return '$ USD'
      case this.GBP: return '£ GBP'
      case this.EUR: return '€ EUR'
      case this.HKD: return 'HK$ HKD'
      case this.KRW: return '₩ KRW'
    }
  },
  transferText (currency) {
    switch (currency) {
      case this.CNY: return intl.get('cny')
      case this.USD: return intl.get('usd')
      case this.GBP: return intl.get('gbp')
      case this.EUR: return intl.get('eur')
      case this.HKD: return intl.get('hkd')
      case this.KRW: return intl.get('krw')
    }
  },
  getRate (currency, convertedCurrency, rates) {
    if (!currency || !convertedCurrency) {
      return 1
    }
    if (currency === 'usdt') {
      if (convertedCurrency === 'usd') {
        return 1
      }
      const usdtCnyRate = formatValue(rates, 'usdt_cny', 1)
      const cnyConvertedRate = formatValue(rates, `cny_${convertedCurrency}`, 1)
      return usdtCnyRate * cnyConvertedRate
    }
    return formatValue(rates, `${currency}_${convertedCurrency}`, 1)
  },
  transferPrice (currency, convertedCurrency, currencyPrice, rates) {
    if (currencyPrice === '--') {
      return '--'
    }
    const rate = this.getRate(currency, convertedCurrency, rates)
    return currencyPrice * rate
  }
}
