import { useState, useEffect, useCallback } from 'react'

const THEME_KEY = 'app-theme'
const LEGACY_THEME_KEY = 'theme' // 兼容旧的 exchange 页面主题系统
const DARK = 'dark'
const LIGHT = 'light'

/**
 * 主题切换 Hook
 * @returns {Object} { theme, isDark, toggleTheme, setTheme }
 */
export function useTheme() {
  const [theme, setThemeState] = useState(() => {
    // 优先从 localStorage 读取
    const saved = localStorage.getItem(THEME_KEY)
    if (saved === DARK || saved === LIGHT) {
      return saved
    }
    // 检查旧的主题键
    const legacySaved = localStorage.getItem(LEGACY_THEME_KEY)
    if (legacySaved === DARK || legacySaved === LIGHT) {
      return legacySaved
    }
    // 默认暗色主题
    return DARK
  })

  // 应用主题到 DOM
  useEffect(() => {
    document.documentElement.setAttribute('data-theme', theme)
    localStorage.setItem(THEME_KEY, theme)
    // 同步到旧的主题键，让 exchange 页面也能正确响应
    localStorage.setItem(LEGACY_THEME_KEY, theme)
  }, [theme])

  // 监听系统主题变化（已禁用，默认使用暗色主题）
  // useEffect(() => {
  //   const mediaQuery = window.matchMedia('(prefers-color-scheme: light)')
  //   const handleChange = (e) => {
  //     const saved = localStorage.getItem(THEME_KEY)
  //     // 只有没有手动设置过才跟随系统
  //     if (!saved) {
  //       setThemeState(e.matches ? LIGHT : DARK)
  //     }
  //   }
  //   mediaQuery.addEventListener('change', handleChange)
  //   return () => mediaQuery.removeEventListener('change', handleChange)
  // }, [])

  const setTheme = useCallback((newTheme) => {
    if (newTheme === DARK || newTheme === LIGHT) {
      setThemeState(newTheme)
    }
  }, [])

  const toggleTheme = useCallback(() => {
    setThemeState(prev => prev === DARK ? LIGHT : DARK)
  }, [])

  return {
    theme,
    isDark: theme === DARK,
    isLight: theme === LIGHT,
    toggleTheme,
    setTheme,
  }
}

export default useTheme
