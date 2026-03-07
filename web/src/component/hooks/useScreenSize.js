import { useState, useEffect } from 'react'

export const ScreenSize = {
  MAX: 'max',
  MIDDLE: 'middle',
  MIN: 'min'
}

export default function useScreenSize () {
  const [size, setSize] = useState(window.innerWidth <= 768 ? ScreenSize.MIN : (window.innerWidth <= 1400 ? ScreenSize.MIDDLE : ScreenSize.MAX))

  useEffect(() => {
    const resizeHandler = () => {
      if (window.innerWidth <= 768) {
        setSize(ScreenSize.MIN)
      } else if (window.innerWidth <= 1400) {
        setSize(ScreenSize.MIDDLE)
      } else {
        setSize(ScreenSize.MAX)
      }
    }
    window.addEventListener('resize', resizeHandler)
    return () => {
      window.removeEventListener('resize', resizeHandler)
    }
  }, [])
  return size
}
