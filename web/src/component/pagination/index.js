import React, { memo, useContext } from 'react'

import Row from '../row'
import Button from '../button'
import ComponentContext from '../context'

import './index.less'

const Pagination = (({
  style, className = '', current = 1, total = 1, onChange
}) => {
  const { theme } = useContext(ComponentContext)

  const pageChange = (page) => {
    onChange && onChange(Math.max(1, Math.min(page, total)))
  }

  const renderMiddleButtons = () => {
    let shouldJumpPrev = false
    let shouldJumpNext = false
    let start
    let end
    if (total > 6) {
      if (current > 3) {
        shouldJumpPrev = true
        if (total - current > 3) {
          start = current
          end = current + 1
          shouldJumpNext = true
        } else {
          start = Math.min(current, total - 3)
          end = total - 1
        }
      } else {
        start = 2
        const max = Math.max(current, 4)
        if (total - max > 2) {
          end = max
          shouldJumpNext = true
        } else {
          end = total - 1
        }
      }
    } else {
      start = 2
      end = total - 1
    }

    const buttons = []
    for (let i = start; i < end + 1; i++) {
      buttons.push(
        <Button
          key={`funny_pagination-page_${i}`}
          onClick={() => { pageChange(i) }}
          size="small"
          className={`funny-pagination-button ${current === i ? 'funny-pagination-active-button' : ''}`}>
          {i}
        </Button>
      )
    }
    return (
      <>
        {
          shouldJumpPrev &&
          <Button
            className="funny-pagination-button funny-pagination-jump-button"
            size="small"
            onClick={() => { pageChange(current - 3) }}>
            <svg
              width="18" height="18"
              viewBox="0 0 19 19"
              data-type="more">
              <path d="M5.75 9.5C5.75 10.3284 5.07843 11 4.25 11C3.42157 11 2.75 10.3284 2.75 9.5C2.75 8.67157 3.42157 8 4.25 8C5.07843 8 5.75 8.67157 5.75 9.5Z"/>
              <path d="M11 9.5C11 10.3284 10.3284 11 9.5 11C8.67157 11 8 10.3284 8 9.5C8 8.67157 8.67157 8 9.5 8C10.3284 8 11 8.67157 11 9.5Z" />
              <path d="M16.25 9.5C16.25 10.3284 15.5784 11 14.75 11C13.9216 11 13.25 10.3284 13.25 9.5C13.25 8.67157 13.9216 8 14.75 8C15.5784 8 16.25 8.67157 16.25 9.5Z"/>
            </svg>
            <svg
              style={{ transform: 'rotate(180deg)' }}
              width="12"
              height="12"
              viewBox="0 0 12 12"
              data-type="jump">
              <path fillRule="evenodd" clipRule="evenodd" d="M1.8028 2.22511C1.97774 2.03671 2.27229 2.0258 2.46069 2.20075L6.18482 5.65887C6.27968 5.74695 6.33358 5.87055 6.33358 6C6.33358 6.12945 6.27968 6.25305 6.18482 6.34113L2.46069 9.79925C2.27229 9.9742 1.97774 9.96329 1.8028 9.77489C1.62785 9.58649 1.63876 9.29194 1.82716 9.117L5.18393 6L1.82716 2.883C1.63876 2.70806 1.62785 2.41351 1.8028 2.22511Z"/>
              <path fillRule="evenodd" clipRule="evenodd" d="M5.17011 1.64876C5.34506 1.46036 5.6396 1.44945 5.828 1.62439L10.1728 5.65887C10.2677 5.74695 10.3216 5.87055 10.3216 6C10.3216 6.12945 10.2677 6.25305 10.1728 6.34113L5.828 10.3756C5.6396 10.5506 5.34506 10.5396 5.17011 10.3512C4.99517 10.1628 5.00608 9.8683 5.19448 9.69335L9.17194 6L5.19448 2.30665C5.00608 2.1317 4.99517 1.83716 5.17011 1.64876Z"/>
            </svg>
          </Button>
        }
        {buttons}
        {
          shouldJumpNext &&
          <Button
            className="funny-pagination-button funny-pagination-jump-button"
            size="small"
            onClick={() => { pageChange(current + 3) }}>
            <svg
              width="18"
              height="18"
              viewBox="0 0 19 19"
              data-type="more">
              <path d="M5.75 9.5C5.75 10.3284 5.07843 11 4.25 11C3.42157 11 2.75 10.3284 2.75 9.5C2.75 8.67157 3.42157 8 4.25 8C5.07843 8 5.75 8.67157 5.75 9.5Z"/>
              <path d="M11 9.5C11 10.3284 10.3284 11 9.5 11C8.67157 11 8 10.3284 8 9.5C8 8.67157 8.67157 8 9.5 8C10.3284 8 11 8.67157 11 9.5Z" />
              <path d="M16.25 9.5C16.25 10.3284 15.5784 11 14.75 11C13.9216 11 13.25 10.3284 13.25 9.5C13.25 8.67157 13.9216 8 14.75 8C15.5784 8 16.25 8.67157 16.25 9.5Z"/>
            </svg>
            <svg
              width="12"
              height="12"
              viewBox="0 0 12 12"
              data-type="jump">
              <path fillRule="evenodd" clipRule="evenodd" d="M1.8028 2.22511C1.97774 2.03671 2.27229 2.0258 2.46069 2.20075L6.18482 5.65887C6.27968 5.74695 6.33358 5.87055 6.33358 6C6.33358 6.12945 6.27968 6.25305 6.18482 6.34113L2.46069 9.79925C2.27229 9.9742 1.97774 9.96329 1.8028 9.77489C1.62785 9.58649 1.63876 9.29194 1.82716 9.117L5.18393 6L1.82716 2.883C1.63876 2.70806 1.62785 2.41351 1.8028 2.22511Z"/>
              <path fillRule="evenodd" clipRule="evenodd" d="M5.17011 1.64876C5.34506 1.46036 5.6396 1.44945 5.828 1.62439L10.1728 5.65887C10.2677 5.74695 10.3216 5.87055 10.3216 6C10.3216 6.12945 10.2677 6.25305 10.1728 6.34113L5.828 10.3756C5.6396 10.5506 5.34506 10.5396 5.17011 10.3512C4.99517 10.1628 5.00608 9.8683 5.19448 9.69335L9.17194 6L5.19448 2.30665C5.00608 2.1317 4.99517 1.83716 5.17011 1.64876Z"/>
            </svg>
          </Button>
        }
      </>
    )
  }

  return (
    <Row
      style={style}
      className={`funny-pagination-${theme} ${className}`}>
      <Button
        disabled={current === 1}
        size="small"
        className="funny-pagination-button"
        onClick={() => { pageChange(current - 1) }}>
        <svg
          width="16"
          height="16"
          viewBox="0 0 16 16">
          <path fillRule="evenodd" clipRule="evenodd" d="M11.0202 2.97979C10.825 2.78453 10.5084 2.78453 10.3131 2.97979L5.64645 7.64646C5.45118 7.84172 5.45118 8.1583 5.64645 8.35356L10.3131 13.0202C10.5084 13.2155 10.825 13.2155 11.0202 13.0202C11.2155 12.825 11.2155 12.5084 11.0202 12.3131L6.70711 8.00001L11.0202 3.6869C11.2155 3.49163 11.2155 3.17505 11.0202 2.97979Z"/>
        </svg>
      </Button>
      <Button
        className={`funny-pagination-button ${current === 1 ? 'funny-pagination-active-button' : ''}`}
        size="small"
        onClick={() => { pageChange(1) }}>
        1
      </Button>
      {renderMiddleButtons()}
      {
        total > 1 &&
        <Button
          className={`funny-pagination-button ${current === total ? 'funny-pagination-active-button' : ''}`}
          size="small"
          onClick={() => { pageChange(total) }}>
          {total}
        </Button>
      }
      <Button
        disabled={current === total}
        className="funny-pagination-button"
        size="small"
        onClick={() => { pageChange(current + 1) }}>
        <svg
          width="16"
          height="16"
          viewBox="0 0 16 16">
          <path fillRule="evenodd" clipRule="evenodd" d="M4.97978 2.97978C5.17504 2.78452 5.49162 2.78452 5.68689 2.97978L10.3536 7.64645C10.5488 7.84171 10.5488 8.15829 10.3536 8.35356L5.68689 13.0202C5.49162 13.2155 5.17504 13.2155 4.97978 13.0202C4.78452 12.825 4.78452 12.5084 4.97978 12.3131L9.29289 8L4.97978 3.68689C4.78452 3.49163 4.78452 3.17504 4.97978 2.97978Z"/>
        </svg>
      </Button>
    </Row>
  )
})

export default memo(Pagination)
