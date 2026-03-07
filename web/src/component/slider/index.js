import React, { memo, useRef, useState, useEffect, useCallback, useContext } from 'react'

import { isValid } from '../utils/typeChecks'

import ComponentContext from '../context'

import './index.less'

const Slider = (({
  style,
  className = '',
  min = 0,
  max = 100,
  step = 1,
  dots = [0, 25, 50, 75, 100],
  showTip,
  showDotLabel,
  defaultValue,
  value,
  formatTipValue,
  formatDotValue,
  onChange
}) => {
  const maxTemp = useRef(max)
  const minTemp = useRef(min)

  const track = useRef()
  const thumb = useRef()

  const startDragX = useRef(0)
  const startDragValue = useRef(0)

  const { theme } = useContext(ComponentContext)

  const [innerValue, setInnerValue] = useState(defaultValue || 0)

  const [tipVisible, setTipVisible] = useState(false)

  const [left, setLeft] = useState({ realLeft: '0%', optLeft: '0%' })

  const calcValue = (distance, startValue) => {
    const rect = track.current.getBoundingClientRect()
    let newValue = distance / rect.width * (maxTemp.current - minTemp.current) + startValue
    if (newValue > maxTemp.current) {
      newValue = maxTemp.current
    }
    if (newValue < minTemp.current) {
      newValue = minTemp.current
    }
    newValue = newValue - newValue % step
    if (newValue !== startValue) {
      setInnerValue(newValue)
      onChange && onChange(newValue)
    }
  }

  const mouseMove = useCallback((e) => {
    e.preventDefault()
    calcValue(e.clientX - startDragX.current, startDragValue.current)
  }, [])

  const touchMove = useCallback((e) => {
    e.preventDefault()
    calcValue(e.changedTouches[0].clientX - startDragX.current, startDragValue.current)
  }, [])

  const unbindEvent = useCallback(() => {
    setTipVisible(false)
    document.removeEventListener('mousemove', mouseMove, { passive: false })
    document.removeEventListener('mouseup', unbindEvent, { passive: false })
  }, [])

  const bindEvent = useCallback(() => {
    setTipVisible(true)
    document.addEventListener('mousemove', mouseMove, { passive: false })
    document.addEventListener('mouseup', unbindEvent, { passive: false })
  }, [])

  const untouchbindEvent = useCallback(() => {
    setTipVisible(false)
    document.removeEventListener('touchmove', touchMove, { passive: false })
    document.removeEventListener('touchend', untouchbindEvent, { passive: false })
  }, [])

  const bindtouchEvent = useCallback(() => {
    setTipVisible(true)
    document.addEventListener('touchmove', touchMove, { passive: false })
    document.addEventListener('touchend', untouchbindEvent, { passive: false })
  }, [])

  useEffect(() => {
    minTemp.current = min
  }, [min])

  useEffect(() => {
    maxTemp.current = max
  }, [max])

  useEffect(() => {
    if (isValid(value)) {
      setInnerValue(value)
    }
  }, [value])

  useEffect(() => {
    const trackRect = track.current.getBoundingClientRect()
    const thumbRect = thumb.current.getBoundingClientRect()
    const trackWidth = trackRect.width
    const halfThumbWidth = Math.ceil(thumbRect.width / 2)
    const rate = (innerValue - min) / (max - min)
    const optActiveWidth = Math.min(Math.max(Math.floor(trackWidth * rate), halfThumbWidth), trackWidth - halfThumbWidth)
    setLeft({ realLeft: `${Math.round(rate * 100)}%`, optLeft: `${Math.round(optActiveWidth / trackWidth * 100)}%` })
  }, [max, min, innerValue])

  return (
    <div
      style={{ height: 20, marginTop: 16, display: 'block' }}
      className={`funny-slider-${theme} ${className}`}>
      <div
        className="funny-slider-row">
        {
          showTip &&
          <span
            style={{ left: left.optLeft }}
            className={`funny-slider-tip ${tipVisible ? 'show' : ''}`}>
            {(formatTipValue && formatTipValue(innerValue)) || left.realLeft}
          </span>
        }
        <span
          ref={track}
          className="funny-slider-track"
          onClick={(e) => {
            const rect = track.current.getBoundingClientRect()
            calcValue(e.clientX - rect.left, min)
          }}/>
        <span
          style={{ width: left.optLeft }}
          className="funny-slider-track-active"/>
        {
          dots.map((dot, i) => {
            if (dot >= min && dot <= max) {
              const percent = (dot - min) / (max - min) * 100
              const dotStyle = { left: `${percent}%` }
              if (dot === min) {
                dotStyle.transform = 'translateX(0)'
              } else if (dot === max) {
                dotStyle.transform = 'translateX(-100%)'
              }
              return (
                <span
                  key={`slider_dot_${i}`}
                  style={dotStyle}
                  className={`funny-slider-dot ${innerValue > dot ? 'active' : ''}`}/>
              )
            }
          })
        }
        <span
          ref={thumb}
          style={{ left: left.optLeft }}
          className="funny-slider-thumb"
          onMouseDown={(e) => {
            startDragX.current = e.clientX
            startDragValue.current = innerValue
            bindEvent()
          }}
          onTouchStart={(e) => {
            startDragX.current = e.changedTouches[0].clientX
            startDragValue.current = innerValue
            bindtouchEvent()
          }}
          onTouchEnd={untouchbindEvent}
          onMouseUp={unbindEvent}>
          <i className="funny-slider-thumb-circle"/>
        </span>
      </div>
      {
        showDotLabel &&
        <div
          style={{ marginTop: 20 }}
          className="funny-slider-row">
          {
            dots.map((dot, i) => {
              if (dot >= min && dot <= max) {
                const percent = (dot - min) / (max - min) * 100
                const dotStyle = { left: `${percent}%` }
                if (dot === min) {
                  dotStyle.transform = 'translateX(0)'
                } else if (dot === max) {
                  dotStyle.transform = 'translateX(-100%)'
                }
                return (
                  <span
                    key={`slider_dot_label_${i}`}
                    style={dotStyle}
                    className="funny-slider-dot-label">
                    {(formatDotValue && formatDotValue(dot)) || dot}
                  </span>
                )
              }
            })
          }
        </div>
      }
    </div>
  )
})

export default memo(Slider)
