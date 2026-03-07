import React, { createContext, memo } from 'react'

export const Theme = {
  LIGHT: 'light',
  DARK: 'dark'
}

const ComponentContext = createContext({
  theme: Theme.DARK,
  getPopoverContainer: () => document.body
})

export default ComponentContext

const ComponentContextProvider = memo(({ theme = Theme.DARK, getPopoverContainer, children }) => {
  return (
    <ComponentContext.Provider value={{
      theme,
      getPopoverContainer: getPopoverContainer || (() => document.body)
    }}>
      {children}
    </ComponentContext.Provider>
  )
})

ComponentContextProvider.displayName = 'ComponentContextProvider'

export { ComponentContextProvider }
