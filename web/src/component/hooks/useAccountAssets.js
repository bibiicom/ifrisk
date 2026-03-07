import { useCallback, useState } from 'react'
import { getAssets } from '../../api/account'

/**
 * 通用资产查询 Hook
 *
 * 使用方式：
 * const { assets, loading, error, run } = useAccountAssets()
 *
 * // 在需要的时候手动触发
 * run({ AccountID: '0x....' })
 */
const useAccountAssets = () => {
  const [assets, setAssets] = useState([])
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState(null)

  // 手动执行查询，参数从外部传入
  const run = useCallback(async (params = {}, config = {}) => {
    try {
      setLoading(true)
      setError(null)
      const res = await getAssets(params, config)
      // getAssets 直接返回接口 data，通常是资产列表数组
      setAssets(res || [])
      return res
    } catch (e) {
      setError(e)
      throw e
    } finally {
      setLoading(false)
    }
  }, [])

  return {
    assets,
    loading,
    error,
    run,
  }
}

export default useAccountAssets
