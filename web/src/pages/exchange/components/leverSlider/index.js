import React, { useState, useEffect, memo, useContext } from 'react'

import Button from '../../../../component/button'
import Slider from '../../../../component/slider'
import ComponentContext from '../../../../component/context'

import './index.less'

const LeverSlider = (({
  lever, maxLever, onChange
}) => {
  const { theme } = useContext(ComponentContext)

  const [innerLever, setInnerLever] = useState(10)

  const [dots, setDots] = useState([1, maxLever])

  const calcLever = (lever, max) => {
    const l = Math.max(1, Math.min(lever, max))
    setInnerLever(l)
    onChange && onChange(l)
  }

  useEffect(() => {
    setInnerLever(lever)
  }, [lever])

  useEffect(() => {
    const dots = [1]
    const interval = Math.floor(maxLever / 5)
    if (interval > 1) {
      for (let i = interval; i < maxLever; i += interval) {
        dots.push(i)
      }
    }
    dots.push(maxLever)
    setDots(dots)
  }, [maxLever])

  return (
    <>
      <div
        className={`exchange-lever-slider-input-${theme}`}>
        <Button
          className="exchange-lever-slider-input-button"
          type="tertiary"
          size="small"
          onClick={_ => { calcLever(innerLever - 1, maxLever) }}>
          -
        </Button>
        <span className="exchange-lever-slider-input-value">{innerLever}X</span>
        <Button
          className="exchange-lever-slider-input-button"
          type="tertiary"
          size="small"
          onClick={_ => { calcLever(innerLever + 1, maxLever) }}>
          +
        </Button>
      </div>
      <Slider
        min={1}
        max={maxLever}
        dots={dots}
        showDotLabel={true}
        formatDotValue={v => `${v}X`}
        value={innerLever}
        onChange={l => { calcLever(l, maxLever) }}/>
    </>
  )
})

export default memo(LeverSlider)
