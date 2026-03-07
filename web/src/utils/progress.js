import Decimal from 'decimal.js'

/**
 * 计算申购进度百分比和已申购/总量
 * @param {string|number} totalStr - 总量（如 RoundVolume 或 AllVolume）
 * @param {string|number} remainingStr - 剩余量（如 StockVolume）
 * @returns {{ percent: number, bought: string, total: string }}
 */
export function calcSubscriptionProgress (totalStr, remainingStr) {
  if (!totalStr) {
    return { percent: 0, bought: '0', total: '0' }
  }

  let total
  try {
    total = new Decimal(totalStr)
  } catch (e) {
    return { percent: 0, bought: '0', total: '0' }
  }

  if (total.lte(0)) {
    return { percent: 0, bought: '0', total: total.toString() }
  }

  let remaining
  try {
    remaining = remainingStr ? new Decimal(remainingStr) : new Decimal(0)
  } catch (e) {
    remaining = new Decimal(0)
  }

  if (remaining.lt(0)) remaining = new Decimal(0)
  if (remaining.gt(total)) remaining = total

  const bought = total.minus(remaining)
  const percent = bought.div(total).mul(100)

  // 保留 3 位有效数字，保证小占比也能显示
  const percentForDisplay = percent.gt(0)
    ? percent.toSignificantDigits(3)
    : new Decimal(0)

  return {
    percent: Number(percentForDisplay.toString()),
    bought: bought.toFixed(),
    total: total.toFixed(),
  }
}
