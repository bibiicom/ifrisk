import React, { memo, useState, useEffect } from 'react'
import { OverlayScrollbarsComponent } from 'overlayscrollbars-react'

import Row from '../row'

import { isObject, isValid } from '../utils/typeChecks'

import './index.less'

const Select = (({
  style, className = '',
  selectorStyle, selectorClassName = '',
  arrowStyle, arrowClassName = '',
  valueStyle, valueClassName = '',
  popoverStyle, popoverClassName = '',
  popoverRowStyle, popoverRowClassName = '',
  prefix,
  trigger = 'click', defaultValue,
  valueKey = 'text', value, defaultOpen, open,
  arrow, showArrow = true, dataSource = [],
  renderPopover, renderPopoverRow,
  onClick, onPopoverRowClick,
  onOpenChange, onSelected
}) => {
  const [innerValue, setInnerValue] = useState(defaultValue)
  const [innerOpen, setInnerOpen] = useState(defaultOpen)

  useEffect(() => {
    if (isValid(value)) {
      setInnerValue(value)
    }
  }, [value])

  useEffect(() => {
    if (isValid(open)) {
      setInnerOpen(open)
    }
  }, [open])

  const getDataValue = (data) => {
    return isObject(data) ? data[valueKey] : data
  }

  const renderPopoverList = () => {
    const stateValue = getDataValue(value)
    return dataSource.map((data, index) => {
      const value = getDataValue(data)
      return (
        <li
          key={`row_${index}`}
          style={popoverRowStyle}
          className={`${popoverRowClassName} ${value === stateValue ? 'selected' : ''}`}
          onMouseDown={(e) => {
            // 使用 onMouseDown 代替 onClick，避免触发外层的 onBlur
            e.preventDefault()
            e.stopPropagation()
          }}
          onClick={(e) => {
            e.stopPropagation()
            // 移除 innerValue !== value 的判断，每次点击都触发回调
            onSelected && onSelected(data)
            setInnerValue(value)
            onPopoverRowClick && onPopoverRowClick(data)
            setInnerOpen(false)
          }}>
          {renderPopoverRow ? renderPopoverRow(data, index) : value}
        </li>
      )
    })
  }

  // 使用 CSS 变量版本，自动适配主题
  return (
    <div
      style={style}
      className={`funny-select-auto ${innerOpen ? 'open' : ''} ${className}`}
      tabIndex="0"
      onClick={(e) => {
        e.stopPropagation()
        onClick && onClick()
        if (trigger === 'click') {
          const open = !innerOpen
          onOpenChange && onOpenChange(open)
          setInnerOpen(open)
        }
      }}
      onBlur={() => {
        onOpenChange && onOpenChange(false)
        setInnerOpen(false)
      }}
      onMouseEnter={() => {
        if (trigger === 'hover') {
          onOpenChange && onOpenChange(true)
          setInnerOpen(true)
        }
      }}
      onMouseLeave={() => {
        if (trigger === 'hover') {
          onOpenChange && onOpenChange(false)
          setInnerOpen(false)
        }
      }}>
      <Row
        style={selectorStyle}
        className={`funny-select-selector ${selectorClassName}`}>
        {prefix && <span className="funny-select-prefix">{prefix}</span>}
        <Row
          style={{ width: '100%', height: '100%', justifyContent: 'space-between' }}>
          <span
            style={valueStyle}
            className={`funny-select-selector-value ${valueClassName}`}>
            {getDataValue(value)}
          </span>
          {
            showArrow &&
            <span
              className="funny-select-selector-arrow-container">
              {arrow || <i style={arrowStyle} className={`funny-select-selector-arrow ${arrowClassName}`}/>}
            </span>
          }
        </Row>
      </Row>
      <div
        style={popoverStyle}
        className={`funny-select-popover ${popoverClassName}`}>
        <OverlayScrollbarsComponent
          style={{ maxHeight: 200 }}>
          {
            renderPopover
              ? renderPopover()
              : (
                <ul>
                  {renderPopoverList()}
                </ul>
              )
          }
        </OverlayScrollbarsComponent>
      </div>
    </div>
  )
})

export default memo(Select)
