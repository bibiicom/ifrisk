import React, { memo, useContext } from 'react'

import Pagination from '../pagination'
import Empty from '../empty'
import Loading from '../loading'
import Col from '../col'
import ComponentContext from '../context'

import './index.less'

export default memo(({
  style, className = '', rowStyle, rowClassName, loading, columns = [], dataSource = [], pagination, renderEmpty
}) => {
  const { theme } = useContext(ComponentContext)

  return (
    <Col
      style={style}
      className={`funny-table-${theme} ${className}`}>
      <table>
        <thead>
          <tr>
            {
              columns.map(({ key, title, width, style }) => (
                <td key={key} style={{ width, ...style }}>{title}</td>
              ))
            }
          </tr>
        </thead>
        <tbody>
          {
            dataSource.map((data, index) => (
              <tr
                key={`unny_table_row_${index}`}
                style={rowStyle}
                className={rowClassName}>
                {
                  columns.map(({ key, value, width, style, render }) => (
                    <td
                      key={key} style={{ width, ...style }}>
                      {(render && render(data)) || data[value] || '--'}
                    </td>
                  ))
                }
              </tr>
            ))
          }
        </tbody>
      </table>
      {
        loading ? <Loading/> : (dataSource.length === 0 && ((renderEmpty && renderEmpty()) || <Empty/>))
      }
      {
        (pagination && dataSource.length > 0) && <Pagination style={{ padding: '20px 0' }} current={pagination.current} total={pagination.total} onChange={pagination.onChange}/>
      }
    </Col>
  )
})
