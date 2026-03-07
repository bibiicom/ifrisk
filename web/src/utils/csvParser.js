/**
 * CSV Parser Utility
 * 用于解析充提记录CSV数据
 */

/**
 * 格式化Unix时间戳为可读日期格式
 * @param {number|string} timestamp - Unix时间戳（秒）
 * @returns {string} 格式化后的日期字符串
 */
export function formatTimestamp(timestamp) {
  const ts = Number(timestamp)
  if (!ts || isNaN(ts)) {
    return String(timestamp || '--')
  }
  // 时间戳为秒级，需要转换为毫秒
  const date = new Date(ts)
  if (isNaN(date.getTime())) {
    return String(timestamp)
  }
  return date.toLocaleString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit',
    hour12: false
  })
}

/**
 * 检测第一行是否为header（非数据行）
 * @param {string} firstLine - CSV第一行
 * @returns {boolean} 是否为header
 */
function isHeaderLine(firstLine) {
  if (!firstLine) return false
  const fields = parseCSVLine(firstLine)
  if (fields.length < 4) return false
  // 如果第一个字段是数字（时间戳），则不是header
  const firstField = fields[0].trim()
  return isNaN(Number(firstField)) || firstField === ''
}

/**
 * 解析CSV字符串为交易记录数组
 * @param {string} csvString - CSV格式字符串
 * @returns {{records: Array<{timestamp: string, rawTimestamp: number, amount: string, currency: string, txHash: string}>, header: string}}
 */
export function parseCsvToRecords(csvString) {
  const result = { records: [], header: '' }
  
  if (!csvString || typeof csvString !== 'string') {
    return result
  }

  const lines = csvString.trim().split('\n')
  if (lines.length < 1) {
    return result
  }

  // 检测第一行是否为header
  const hasHeader = isHeaderLine(lines[0])
  const startIndex = hasHeader ? 1 : 0

  // 保存原始header（如果有）
  if (hasHeader) {
    result.header = lines[0].trim()
  }

  // 解析数据行
  for (let i = startIndex; i < lines.length; i++) {
    const line = lines[i].trim()
    if (!line) continue

    const fields = parseCSVLine(line)
    if (fields.length >= 4) {
      const rawTimestamp = Number(fields[0]) || 0
      result.records.push({
        timestamp: formatTimestamp(rawTimestamp),
        rawTimestamp,
        amount: fields[1] || '',
        currency: fields[2] || '',
        txHash: fields[3] || ''
      })
    }
  }

  return result
}

/**
 * 解析单行CSV，处理引号包裹的字段
 * @param {string} line - CSV行
 * @returns {string[]} 字段数组
 */
function parseCSVLine(line) {
  const fields = []
  let current = ''
  let inQuotes = false

  for (let i = 0; i < line.length; i++) {
    const char = line[i]
    
    if (char === '"') {
      if (inQuotes && line[i + 1] === '"') {
        current += '"'
        i++
      } else {
        inQuotes = !inQuotes
      }
    } else if (char === ',' && !inQuotes) {
      fields.push(current)
      current = ''
    } else {
      current += char
    }
  }
  fields.push(current)

  return fields
}

/**
 * 将交易记录数组转换回CSV字符串
 * @param {Array<{rawTimestamp: number, amount: string, currency: string, txHash: string}>} records
 * @param {string} header - 原始CSV header
 * @returns {string} CSV格式字符串
 */
export function recordsToCsv(records, header = '时间戳,金额,币种,交易hash') {
  if (!records || !Array.isArray(records) || records.length === 0) {
    return header
  }

  const rows = records.map(record => {
    const ts = record.rawTimestamp || ''
    const amount = escapeCSVField(record.amount || '')
    const currency = escapeCSVField(record.currency || '')
    const txHash = escapeCSVField(record.txHash || '')
    return `${ts},${amount},${currency},${txHash}`
  })

  return [header, ...rows].join('\n')
}

/**
 * 转义CSV字段中的特殊字符
 * @param {string} field - 字段值
 * @returns {string} 转义后的字段
 */
function escapeCSVField(field) {
  const str = String(field)
  if (str.includes(',') || str.includes('"') || str.includes('\n')) {
    return `"${str.replace(/"/g, '""')}"`
  }
  return str
}
