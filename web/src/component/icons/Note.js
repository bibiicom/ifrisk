import React, { useContext } from 'react'

import ComponentContext from '../context'

const Note = (props) => {
  const { theme } = useContext(ComponentContext)
  return (
    <svg
      {...props}
      className={`funny-icon-${theme} ${props.className}`}
      viewBox="0 0 18 18">
      <path fillRule="evenodd" clipRule="evenodd" d="M9 15.0562C12.3448 15.0562 15.0562 12.3448 15.0562 9C15.0562 5.65523 12.3448 2.94375 9 2.94375C5.65523 2.94375 2.94375 5.65523 2.94375 9C2.94375 12.3448 5.65523 15.0562 9 15.0562ZM9 16.125C12.935 16.125 16.125 12.935 16.125 9C16.125 5.06497 12.935 1.875 9 1.875C5.06497 1.875 1.875 5.06497 1.875 9C1.875 12.935 5.06497 16.125 9 16.125Z"/>
      <path d="M9.92625 5.65125C9.92625 6.1628 9.51155 6.5775 9 6.5775C8.48844 6.5775 8.07375 6.1628 8.07375 5.65125C8.07375 5.1397 8.48844 4.725 9 4.725C9.51155 4.725 9.92625 5.1397 9.92625 5.65125Z"/>
      <path fillRule="evenodd" clipRule="evenodd" d="M9 7.575C9.3935 7.575 9.7125 7.894 9.7125 8.2875V12.5625C9.7125 12.956 9.3935 13.275 9 13.275C8.6065 13.275 8.2875 12.956 8.2875 12.5625V8.2875C8.2875 7.894 8.6065 7.575 9 7.575Z"/>
    </svg>
  )
}

export default Note
