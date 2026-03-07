import moment from 'moment'

import { isValid, isNumber } from '../component/utils/typeChecks'

export function formatValue (data, key, defaultValue = '--') {
  // console.log('data', data, key)
  if (isValid(data) && isValid(key)) {
    const v = data[key]
    if (isValid(v)) {
      return v
    }
  }
  return defaultValue
}

/**
 * 格式化小数位数
 * @param value
 * @param decimal
 * @param floorOrCeil
 * @param notThousands
 * @returns {string|*}
 */

export function formatDecimal (value, decimal = 2, floorOrCeil = 'floor', notThousands) {
  const v = +value
  if (value !== '--' && isNumber(v)) {
    let mathFloorCeil
    if (v < 0) {
      if (floorOrCeil === 'floor') {
        mathFloorCeil = Math.ceil
      } else {
        mathFloorCeil = Math.floor
      }
    } else {
      if (floorOrCeil === 'floor') {
        mathFloorCeil = Math.floor
      } else {
        mathFloorCeil = Math.ceil
      }
    }
    let powValue = +((v * Math.pow(10, 10)).toFixed(0))
    powValue = mathFloorCeil(powValue / Math.pow(10, 10 - decimal))
    const vl = (powValue / Math.pow(10, decimal)).toFixed(decimal)
    if (notThousands) {
      return vl
    }
    return formatThousands(vl)
  }
  return value
}

export function formatDecimalNoZero (value, decimal = 2, floorOrCeil = 'floor', notThousands) {
  const v = +value
  if (value !== '--' && isNumber(v)) {
    let mathFloorCeil
    if (v < 0) {
      mathFloorCeil = floorOrCeil === 'floor' ? Math.ceil : Math.floor
    } else {
      mathFloorCeil = floorOrCeil === 'floor' ? Math.floor : Math.ceil
    }

    let powValue = +((v * Math.pow(10, 10)).toFixed(0))
    powValue = mathFloorCeil(powValue / Math.pow(10, 10 - decimal))
    let vl = (powValue / Math.pow(10, decimal)).toFixed(decimal)

    // 去掉末尾 0 和小数点
    vl = vl.replace(/\.?0+$/, '')

    if (notThousands) {
      return vl
    }
    return formatThousands(vl)
  }
  return value
}

export function formatBigNumber (value) {
  const v = Number(value)
  if (isNaN(v)) {
    return value
  }
  if (value > 1000000) {
    return `${formatThousands(Math.floor(value / 1000000))}M`
  }
  if (value > 1000) {
    return `${Math.floor(value / 1000)}K`
  }
  return value
}

/**
 * 格式化大数字为中文单位(万、亿)
 * @param {number|string} value - 要格式化的数值
 * @param {number} decimal - 小数位数,默认2位
 * @param {string} prefix - 前缀符号,如 '$'、'€' 等
 * @returns {string} 格式化后的字符串
 */
export function formatChineseNumber (value, decimal = 2, prefix = '') {
  const v = Number(value)
  if (isNaN(v) || v === 0) {
    return `${prefix}0`
  }
  
  const absValue = Math.abs(v)
  const sign = v < 0 ? '-' : ''
  
  // 万亿 (1,000,000,000,000)
  if (absValue >= 1000000000000) {
    const result = (absValue / 1000000000000).toFixed(decimal)
    return `${sign}${prefix}${result}t`
  }
  
  // 亿 (100,000,000)
  if (absValue >= 100000000) {
    const result = (absValue / 100000000).toFixed(decimal)
    return `${sign}${prefix}${result}m`
  }
  
  // 万 (10,000)
  if (absValue >= 10000) {
    const result = (absValue / 10000).toFixed(decimal)
    return `${sign}${prefix}${result}w`
  }
  
  // 千 (1,000)
  if (absValue >= 1000) {
    const result = (absValue / 1000).toFixed(decimal)
    return `${sign}${prefix}${result}k`
  }
  
  // 小于1千,保留指定小数位
  return `${sign}${prefix}${absValue.toFixed(decimal)}`
}

/**
 * 格式化千分位
 * @param value
 */
export function formatThousands (value) {
  const vl = value + ''
  if (vl.indexOf('.') > -1) {
    const arr = vl.split('.')
    return `${arr[0].replace(/(\d)(?=(\d{3})+$)/g, $1 => `${$1},`)}.${arr[1]}`
  } else {
    return vl.replace(/(\d)(?=(\d{3})+$)/g, $1 => `${$1},`)
  }
}

/**
 * 格式化百分数
 * @param value
 * @param decimal
 * @returns
 */
export function formatPercent (value, decimal = 2) {
  const v = +value
  if (value !== '--' && isNumber(v)) {
    return `${formatDecimal(v * 100, decimal)}%`
  }
  return value
}

export function formatUtcTimestampToLocalTime (timestamp, format) {
  return moment(moment.utc(timestamp).toDate()).format(format)
}

/**
 * 格式化为指定有效数字位数
 * @param {number|string} value - 要格式化的数值
 * @param {number} significantDigits - 有效数字位数,默认4位
 * @returns {string} 格式化后的字符串
 */
export function formatSignificantDigits (value, significantDigits = 4) {
  const v = Number(value)
  if (isNaN(v) || v === 0) {
    return '0'
  }
  
  // 使用 toPrecision 保留有效数字
  const formatted = v.toPrecision(significantDigits)
  
  // 去掉科学计数法,转换为普通数字,并去掉末尾的零
  const num = parseFloat(formatted)
  
  return num.toString()
}

/**
 * 根据用户配置获取涨跌颜色
 * @param {number|string} change - 涨跌幅
 * @param {string} colorTheme - 颜色主题配置 ('buy-rise-green' | 'buy-rise-red')
 * @returns {string} 颜色值 (十六进制格式)
 */
export function getChangeColor (change, colorTheme) {
  const changeValue = parseFloat(change)
  
  // 处理无效值
  if (isNaN(changeValue)) {
    return '#2ebd85' // 默认返回绿色
  }
  
  const isPositive = changeValue >= 0
  
  // 红涨绿跌
  if (colorTheme === 'buy-rise-red') {
    return isPositive ? '#f6465d' : '#2ebd85'
  }
  
  // 绿涨红跌 (默认)
  return isPositive ? '#2ebd85' : '#f6465d'
}


/**
 * 将数字转换为带单位的简洁格式 (支持千、万)，保留两位有效数字
 * @param {number} num - 需要转换的数字
 * @returns {string} 格式化后的字符串
 */ 
export function formatNumber(num) {
    // 处理非数字输入
    if (typeof num !== 'number' || isNaN(num)) {
        return '0';
    }

    // 定义单位阈值和对应符号
    const units = [
        { value: 1e4, symbol: 'w' },   // 万
        { value: 1e3, symbol: 'k' }    // 千
    ];

    // 根据数字大小选择合适的单位（优先使用更大的单位：万 > 千）
    for (let i = 0; i < units.length; i++) {
        if (Math.abs(num) >= units[i].value) {
            const convertedNum = num / units[i].value;
            
            // 转换为保留两位有效数字
            if (convertedNum >= 100) {
                // 三位数以上不需要小数
                return Math.round(convertedNum).toString() + units[i].symbol;
            } else if (convertedNum >= 10) {
                // 两位数，保留一位小数或整数
                const rounded = Math.round(convertedNum * 10) / 10;
                return (rounded % 1 === 0 ? Math.round(rounded).toString() : rounded.toString()) + units[i].symbol;
            } else {
                // 一位数，保留两位有效数字
                const rounded = Math.round(convertedNum * 100) / 100;
                // 去掉多余的小数点后的0
                return rounded.toString() + units[i].symbol;
            }
        }
    }

    // 小于1000的直接返回原数字符串
    return num.toString();
}
