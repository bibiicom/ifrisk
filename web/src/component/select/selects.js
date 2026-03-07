import React, { memo, useState, useEffect, useContext } from 'react'
import { OverlayScrollbarsComponent } from 'overlayscrollbars-react'

import Row from '../row'
import ComponentContext from '../context'

import { isObject, isValid } from '../utils/typeChecks'

import './index.less'
import intl from '../intl'

export default memo(({
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
  onOpenChange, onSelected, type
}) => {
  const { theme } = useContext(ComponentContext)

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
          onClick={(e) => {
            e.stopPropagation()
            if (innerValue !== value) {
              onSelected && onSelected(data)
              setInnerValue(value)
            }
            onPopoverRowClick && onPopoverRowClick(data)
            setInnerOpen(false)
          }}>
          {renderPopoverRow ? renderPopoverRow(data, index) : value}
        </li>
      )
    })
  }

  return (
    <div
      style={style}
      className={`funny-select-${theme} ${innerOpen ? 'open' : ''} ${className}`}
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
          <span style={{ color: '#cecece' }}>
            {intl.get(type)}
          </span>
          <div style={{ display: 'flex', alignItems: 'center' }}>
            <span
              style={valueStyle}
              className={`funny-select-selector-value ${valueClassName}`}>
              {getDataValue(value)}
            </span>
            {
              showArrow &&
              <span
                style={{ marginLeft: '8px' }}
                className="funny-select-selector-arrow-container">
                {arrow || <i style={{ borderTop: '5px solid rgba(187,187,187,1)' }} className={`funny-select-selector-arrow ${arrowClassName}`}/>}
              </span>
            }
          </div>
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
