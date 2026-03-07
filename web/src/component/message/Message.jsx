import React, { useContext, useEffect, useRef } from 'react'

import ComponentContext from '../context'

const Message = ({ id, content, children, onClose }) => {
  const timer = useRef()

  const { theme } = useContext(ComponentContext)

  useEffect(() => {
    timer.current = setTimeout(() => {
      onClose && onClose(id)
    }, 3000)
  }, [])

  return (
    <div
      className={`funny-message-${theme}`}>
      {content || children}
    </div>
  )
}

export default Message