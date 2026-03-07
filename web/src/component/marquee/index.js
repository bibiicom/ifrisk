import React, { memo, useRef, useEffect, useContext } from 'react'

import ComponentContext from '../context'

import './index.less'

const marquee = (({ className = '', style, texts = [], distance = 22, speed = 20, delay = 1500 }) => {
  const el = useRef()
  const timer = useRef()

  const { theme } = useContext(ComponentContext)

  const startMarquee = () => {
    if (timer.current) {
      clearInterval(timer.current)
    }
    let pause = false
    const o = el.current
    o.onmouseover = function () { pause = true }
    o.onmouseout = function () { pause = false }
    o.innerHTML += o.innerHTML
    o.scrollTop = 0
    const start = () => {
      timer.current = setInterval(scrolling, speed)
      if (!pause) {
        o.scrollTop += 1
      }
    }
    const scrolling = () => {
      if (o.scrollTop % distance !== 0) {
        o.scrollTop += 1
        if (o.scrollTop >= o.scrollHeight / 2) {
          o.scrollTop = 0
        }
      } else {
        clearInterval(timer.current)
        setTimeout(start, delay)
      }
    }
    setTimeout(start, delay)
  }

  useEffect(() => {
    startMarquee()
  }, [
    texts, distance, speed, delay
  ])

  useEffect(() => {
    clearInterval(timer.current)
  }, [])

  return (
    <ul
      ref={el}
      style={style}
      className={`funny-marquee-${theme} ${className}`}>
      {
        texts.map((text, index) => {
          return <li key={`marquee_text_${index}`}>{text}</li>
        })
      }
    </ul>
  )
})

export default memo(marquee)
