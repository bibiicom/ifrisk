import { getDecimal } from '../utils/number'
import { getPubilc } from '../network/pubilcHttp'

export async function getSymbolList (productGroup, config) {
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 3 }
  const symbolList = await getPubilc('public/v1.0/SendQryInstrument', { ProductGroup: productGroup }, requestConfig)
  // console.log('result', result)
  // const symbolList = result.result || []
  const newList = []
  symbolList.forEach(symbol => {
    // console.log('symbol', symbol)
    const pricePrecision = getDecimal(symbol.PT)
    const newSymbol = {
      ...symbol,
      PricePrecision: pricePrecision,
      VolumePrecision: getDecimal(symbol.VT),
      CoinPrecision: getDecimal(symbol.VT),
      OtherPrecision: getDecimal(symbol.VT),
      FeePrecision: 8,
      TransferPrecision: getDecimal(symbol.VT),
      InstrumentID: symbol.I,
      ExchangeID: symbol.X,
      PriceTick: symbol.PT,
      PriceCurrency: symbol.PC,
      BaseCurrency: symbol.BB,
      ClearCurrency: symbol.CC,
      ProductGroup: symbol.G,
      InstrumentName: symbol.N,
    }
    // console.log('newSymbol', newSymbol)
    newList.push(newSymbol)
  })
  return newList
}

export async function getMarkets (productGroup, config) {
  // console.log('getMakets', productGroup, config)
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 3 }
  const marketList = await getPubilc('public/v1.0/SendQryMarketData', { ProductGroup: productGroup }, requestConfig)
  const markets = {}
  marketList.forEach(market => {
    if (market.O) {
      market.Change = ((market.N || market.O) - market.O) / market.O
    }
    markets[market.I] = market
  })
  // console.log('markets', markets)
  return markets
}

export async function getCurrency (productGroup, config) {
  // console.log('getMakets', productGroup, config)
  const requestConfig = { ...config }
  requestConfig.headers = { ...requestConfig.headers, Zip: 3 }
  const CurrencyList = await getPubilc('public/v1.0/SendQryCurrency', { SettlementGroup: productGroup }, requestConfig)
  // 转换字段名：C -> Currency, SG -> SettlementGroup
  return (CurrencyList || []).map(item => ({
    Currency: item.C,
    SettlementGroup: item.SG,
    ...item
  }))
}
