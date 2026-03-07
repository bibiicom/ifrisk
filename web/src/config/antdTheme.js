// 全局 antd 主题配置，集中管理主色等 token
import { theme } from 'antd';

// 暗色主题配置
export const darkTheme = {
  algorithm: theme.darkAlgorithm,
  token: {
    colorPrimary: '#1677ff',
    colorBgContainer: '#1e1e1e',
    colorBgElevated: '#1a1a2e',
    colorBgLayout: '#121212',
    colorText: 'rgba(255, 255, 255, 0.85)',
    colorTextSecondary: 'rgba(255, 255, 255, 0.65)',
    colorBorder: 'rgba(255, 255, 255, 0.1)',
  },
};

// 亮色主题配置
export const lightTheme = {
  algorithm: theme.defaultAlgorithm,
  token: {
    colorPrimary: '#1677ff',
    colorBgContainer: '#ffffff',
    colorBgElevated: '#ffffff',
    colorBgLayout: '#f5f5f5',
    colorText: 'rgba(0, 0, 0, 0.88)',
    colorTextSecondary: 'rgba(0, 0, 0, 0.65)',
    colorBorder: 'rgba(0, 0, 0, 0.1)',
  },
};

/**
 * 根据主题名称获取 antd 主题配置
 * @param {string} themeName - 'dark' | 'light'
 */
export function getAntdTheme(themeName) {
  return themeName === 'light' ? lightTheme : darkTheme;
}

// 默认导出暗色主题（兼容旧代码）
export default darkTheme;
