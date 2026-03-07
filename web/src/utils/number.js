export function getDecimal (value, defaultDecimal = 2) {
  if (value !== '--') {
    const v = `${value}`
    if (v.indexOf('e') > -1) {
      const split = v.split('e')
      return Math.abs(parseInt(split[1] || '0', 10))
    }
    const index = v.indexOf('.')
    let decimal = 0
    if (index > -1) {
      decimal = v.substr(index + 1).length
    }
    return decimal
  }
  return defaultDecimal
}

// 按有效数字格式化，默认保留 3 位有效数字
// formatDecimals(12345) => '1.23e+4'
// formatDecimals(0.000012345) => '0.0000123'
// formatDecimals(12.3456, 3) => '12.3'
export function formatDecimals (value, significant = 3) {
  if (value === '--' || value === null || value === undefined || value === '') {
    return value
  }

  const num = Number(value)
  if (Number.isNaN(num)) {
    return value
  }

  // 限制有效数字范围，避免异常值
  const digits = typeof significant === 'number' && significant > 0 ? significant : 3
  if (Math.abs(num) >= 1) {
    return Number(num.toFixed(digits)).toString()
  }

  return Number(num.toPrecision(digits)).toString()
}
