import React, { memo, useContext } from 'react'

import { OverlayScrollbarsComponent } from 'overlayscrollbars-react'

import Loading from '../loading'
import Empty from '../empty'
import ComponentContext from '../context'

import './index.less'

const List = (({
  style, className = '', rowStyle, rowClassName = '', showTitle, loading, columns = [], dataSource = [], onRowClick, renderRow, renderEmpty
}) => {
  const { theme } = useContext(ComponentContext)

  return (
    <div
      style={style}
      className={`funny-list-${theme} ${className}`}>
      {
        loading ? <Loading/> : (dataSource.length === 0 && (renderEmpty ? renderEmpty() : <Empty/>))
      }
      {
        showTitle &&
        <div
          className="funny-list-row funny-list-title">
          {
            columns.map(({
              key, title, style, className
            }, i) => (
              <span
                key={key || `list_title_item_${i}`}
                style={style}
                className={className}>
                {title}
              </span>
            ))
          }
        </div>
      }
      <OverlayScrollbarsComponent
        style={!showTitle ? { height: '100%' } : {}}
        className="funny-list-content">
        <ul>
          {
            dataSource.map((data, n) => (
              <li
                key={`list_row_${n}`}
                style={rowStyle}
                className={`funny-list-row ${rowClassName}`}
                onClick={(e) => { e.stopPropagation(); onRowClick && onRowClick(data) }}>
                {
                  (renderRow && renderRow(data, columns)) || (
                    columns.map(({
                      key, value, style, className, render
                    }, i) => (
                      <span
                        key={key || `list_content_row_item_${i}`}
                        style={style}
                        className={className}>
                        {(render && (render(data))) || data[value] || '--'}
                      </span>
                    ))
                  )
                }
              </li>
            ))
          }
        </ul>
      </OverlayScrollbarsComponent>
    </div>
  )
})

export default memo(List)
