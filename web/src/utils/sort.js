export function sortByField(list = [], field, order = 'desc') {
  const arr = Array.isArray(list) ? list.slice() : []

  const dir = String(order).toLowerCase() === 'asc' ? 1 : -1

  const getValue = item => {
    if (typeof field === 'function') return field(item)
    if (!field) return undefined
    return item ? item[field] : undefined
  }

  return arr
    .map((item, index) => ({ item, index }))
    .sort((a, b) => {
      const av = getValue(a.item)
      const bv = getValue(b.item)

      const aEmpty = av === undefined || av === null || (typeof av === 'number' && Number.isNaN(av))
      const bEmpty = bv === undefined || bv === null || (typeof bv === 'number' && Number.isNaN(bv))

      if (aEmpty && bEmpty) return a.index - b.index
      if (aEmpty) return 1
      if (bEmpty) return -1

      const an = Number(av)
      const bn = Number(bv)
      const aNum = Number.isFinite(an)
      const bNum = Number.isFinite(bn)

      if (aNum && bNum) {
        const diff = (an - bn) * dir
        return diff === 0 ? a.index - b.index : diff
      }

      const diff = String(av).localeCompare(String(bv), 'zh-CN', {
        numeric: true,
        sensitivity: 'base',
      }) * dir

      return diff === 0 ? a.index - b.index : diff
    })
    .map(v => v.item)
}
