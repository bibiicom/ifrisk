export const THEME = 'app_theme'
export const COLOR = 'app_color'
export const CONVERTED_CURRENCY = 'converted_currency'
export const ORDER_CONFIRM = 'order_confirm'
export const K_LINE_INTERVAL = 'k_line_interval'

export const VOLUME_UNIT = 'volume_unit'

export const TOKEN = 'app_token'

export const SYMBOL_KEY = 'symbol'

export const FAVORITES_KEY = 'favoriteSymbols'

export function saveStorage (key, value) {
  window.localStorage.setItem(key, value)
}

export function getStorage (key) {
  return window.localStorage.getItem(key)
}

export function removeStorage (key) {
  return window.localStorage.removeItem(key)
}
