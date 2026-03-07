import React, { useEffect, useState, useMemo } from 'react'

// 通用倒计时组件
// props:
// - statusLabel: 状态名称/标签文案，例如："预展期"、"申购中" 等
// - prefixText: 状态前缀文案，例如："申购开始倒计时"、"申购结束还剩" 等
// - endTime: 结束时间，Date 对象或可被 new Date() 解析的字符串/时间戳
// - onEnd: 可选，倒计时结束时回调
const CountdownTimer = ({ statusLabel, prefixText, endTime, onEnd }) => {
  const [remainMs, setRemainMs] = useState(() => {
    if (!endTime) return 0
    const target = new Date(endTime).getTime()
    const now = Date.now()
    return Math.max(target - now, 0)
  })

  useEffect(() => {
    if (!endTime) return undefined

    const target = new Date(endTime).getTime()

    const timer = setInterval(() => {
      const now = Date.now()
      const diff = Math.max(target - now, 0)
      setRemainMs(diff)
      if (diff === 0) {
        clearInterval(timer)
        if (onEnd) onEnd()
      }
    }, 1000)

    return () => clearInterval(timer)
  }, [endTime, onEnd])

  const timeParts = useMemo(() => {
    let ms = remainMs
    const totalSeconds = Math.floor(ms / 1000)
    const days = Math.floor(totalSeconds / (24 * 3600))
    const hours = Math.floor((totalSeconds % (24 * 3600)) / 3600)
    const minutes = Math.floor((totalSeconds % 3600) / 60)
    const seconds = totalSeconds % 60
    return { days, hours, minutes, seconds }
  }, [remainMs])

  const { days, hours, minutes, seconds } = timeParts

  return (
    <span>
      {statusLabel && <span>{statusLabel} </span>}
      {prefixText && <span>{prefixText}：</span>}
      <span>
        {days}天 {hours}时 {minutes}分 {seconds}秒
      </span>
    </span>
  )
}

export default CountdownTimer
