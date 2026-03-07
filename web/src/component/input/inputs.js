import React, { useState, useEffect, useRef, useContext, memo } from 'react'

import ComponentContext from '../context'
import searchIcon from './search.png'

import { isValid, isNumber } from '../utils/typeChecks'

import './index.less'

export default memo(({
  className = '', style, valueClassName = '', valueStyle, prefix, suffix, placeholder, precision, disabled, errorMessage,
  min = Number.MIN_SAFE_INTEGER, max = Number.MAX_SAFE_INTEGER, defaultValue, value, type,
  onFocus, onBlur, onChange
}) => {
  const input = useRef()

  const { theme } = useContext(ComponentContext)

  const [status, setStatus] = useState('normal')

  const [innerValue, setInnerValue] = useState(defaultValue || '')

  useEffect(() => {
    if (isValid(value)) {
      setInnerValue(value)
    }
  }, [value])

  useEffect(() => {
    if (isValid(disabled)) {
      setStatus(disabled ? 'disabled' : 'normal')
    }
    if (isValid(errorMessage)) {
      setStatus(errorMessage ? 'error' : 'normal')
    }
  }, [disabled, errorMessage])

  return (
    <div
      style={style}
      className={`funny-input-${theme} ${className}`}
      data-status={status}
      onClick={() => { !disabled && input.current.focus() }}>
      {prefix && <span className="funny-input-prefix">{prefix}</span>}
      <img src={searchIcon}/>
      <input
        ref={input}
        style={{ color: '#fff' }}
        className={`funny-input-value ${valueClassName}`}
        type={type}
        placeholder={placeholder}
        value={innerValue}
        disabled={disabled}
        onFocus={() => { setStatus('focus'); onFocus && onFocus() }}
        onBlur={() => { setStatus('normal'); onBlur && onBlur() }}
        onChange={(e) => {
          const v = e.target.value
          if (isNumber(precision)) {
            let reg
            const decimalDigit = Math.max(0, parseInt(precision, 10))
            if (decimalDigit <= 0) {
              reg = new RegExp(/^[1-9]\d*$/)
            } else {
              reg = new RegExp('^\\d+\\.?\\d{0,' + decimalDigit + '}$')
            }
            if (v === '' || (reg.test(v) && +v >= min && +v <= max)) {
              setInnerValue(v)
              onChange && onChange(v)
            }
          } else {
            onChange && onChange(v)
          }
        }}/>
      {suffix && <span className="funny-input-suffix">{suffix}</span>}
      <span
        className="funny-input-error"
        data-visible={!!errorMessage}>
        ※&nbsp;{errorMessage}
      </span>
    </div>
  )
})
