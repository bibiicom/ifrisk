import React, { useContext } from 'react'

import ComponentContext from '../context'

const Arrow = (props) => {
  const { theme } = useContext(ComponentContext)

  return (
    <svg
      {...props}
      className={`funny-icon-${theme} ${props.className}`}
      viewBox="0 0 14 15">
      <path fillRule="evenodd" clipRule="evenodd" d="M2.60714 5.44064C2.778 5.26979 3.05501 5.26979 3.22586 5.44064L6.99984 9.21461L10.7738 5.44064C10.9447 5.26979 11.2217 5.26979 11.3925 5.44064C11.5634 5.6115 11.5634 5.8885 11.3925 6.05936L7.3092 10.1427C7.13834 10.3135 6.86133 10.3135 6.69048 10.1427L2.60714 6.05936C2.43629 5.8885 2.43629 5.6115 2.60714 5.44064Z"/>
    </svg>
  )
}

export default Arrow
