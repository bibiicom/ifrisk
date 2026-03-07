import React, { useEffect, useRef } from 'react'
import ReactECharts from 'echarts-for-react'

// 通用 ECharts 容器组件，统一宽高和基础配置
// props 与 ReactECharts 基本一致，额外支持 height / width 简化使用
const Echarts = ({ height = '100%', width = '100%', renderer = 'svg', devicePixelRatio, ...other }) => {
  const { opts, useResizeHandler, ...rest } = other
  const dpr = devicePixelRatio ?? (typeof window !== 'undefined' ? window.devicePixelRatio : 1)
  const resizeHandler = useResizeHandler ?? true
  const containerRef = useRef(null)
  const chartRef = useRef(null)

  useEffect(() => {
    if (!resizeHandler) return

    const resize = () => {
      const instance = chartRef.current?.getEchartsInstance?.()
      if (instance) {
        instance.resize()
      }
    }

    const el = containerRef.current
    if (!el) return

    const rafId = typeof requestAnimationFrame !== 'undefined' ? requestAnimationFrame(resize) : null

    if (typeof ResizeObserver !== 'undefined') {
      const observer = new ResizeObserver(() => {
        resize()
      })
      observer.observe(el)
      return () => {
        if (rafId != null) cancelAnimationFrame(rafId)
        observer.disconnect()
      }
    }

    if (typeof window === 'undefined') return

    window.addEventListener('resize', resize)
    return () => {
      if (rafId != null) cancelAnimationFrame(rafId)
      window.removeEventListener('resize', resize)
    }
  }, [height, width, resizeHandler])

  return (
    <div ref={containerRef} style={{ width, height }}>
      <ReactECharts
        ref={chartRef}
        style={{ width, height }}
        notMerge={true}
        lazyUpdate={true}
        opts={{ renderer, devicePixelRatio: dpr, ...(opts || {}) }}
        {...rest}
      />
    </div>
  )
}

export default Echarts
