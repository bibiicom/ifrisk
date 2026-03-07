import React, { useContext } from 'react'
import { Switch, Tooltip } from 'antd'
import { useThemeContext } from '../../context/ThemeContext'
import RootContext from '../../pages/rootContext'
import { THEME_KEY } from '../../pages/storageConfig'
import styles from './index.module.less'

const ThemeSwitch = () => {
    const { isDark, toggleTheme } = useThemeContext()
    const rootContext = useContext(RootContext)

    const handleToggle = () => {
        toggleTheme()
        // 同步更新 RootContext 中的 config.theme
        if (rootContext?.setConfig) {
            const newTheme = isDark ? 'light' : 'dark'
            rootContext.setConfig({ [THEME_KEY]: newTheme })
        }
    }

    return (
        <Tooltip title={isDark ? 'Switch to Light Mode' : 'Switch to Dark Mode'}>
            <div className={styles.wrapper}>
                <span className={styles.icon}>{isDark ? '🌙' : '☀️'}</span>
                <Switch
                    checked={!isDark}
                    onChange={handleToggle}
                    size="small"
                    className={styles.switch}
                />
            </div>
        </Tooltip>
    )
}

export default ThemeSwitch
