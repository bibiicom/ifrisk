import intl from '../component/intl'

import { getStorage, saveStorage } from '../utils/storage'

import { Theme, ColorTheme } from '../constants/theme'
import { ConvertedCurrency } from '../constants/convertedCurrency'

export const LOCALE_KEY = 'locale'
export const THEME_KEY = 'theme'
export const COLOR_THEME_KEY = 'colorTheme'
export const CONVERTED_CURRENCY_KEY = 'convertedCurrency'
export const ORDER_CONFIRM_KEY = 'orderConfirm'

const supportLocales = ['zh-CN', 'zh-HK', 'en-US', 'ko-KR', 'ja-JP']

export function getStorageConfig () {
  const pathname = window.location.pathname || ''
  let locale = pathname.split('/')[1]
  if (supportLocales.indexOf(locale) < 0) {
    locale = getStorage(LOCALE_KEY) || ''
  }
  intl.load(locale || 'en-US', require(`../locales/${locale || 'en-US'}.json`))
  intl.setLanguage(locale || 'en-US')
  saveStorage(LOCALE_KEY, locale)
  const theme = getStorage(THEME_KEY) || Theme.DARK
  const colorTheme = getStorage(COLOR_THEME_KEY) || ColorTheme.BUY_RISE_GREEN
  const convertedCurrency = getStorage(CONVERTED_CURRENCY_KEY) || ConvertedCurrency.CNY
  let orderConfirm = getStorage(ORDER_CONFIRM_KEY)
  if (orderConfirm === '' || orderConfirm === null) {
    orderConfirm = true
  } else {
    orderConfirm = orderConfirm !== 'false'
  }
  return {
    [LOCALE_KEY]: locale,
    [THEME_KEY]: theme,
    [COLOR_THEME_KEY]: colorTheme,
    [CONVERTED_CURRENCY_KEY]: convertedCurrency,
    [ORDER_CONFIRM_KEY]: orderConfirm
  }
}

export function saveStorageConfig (config) {
  for (const key in config) {
    saveStorage(key, config[key])
  }
}
