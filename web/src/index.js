import React from 'react';
import { createRoot } from 'react-dom/client';
import { ConfigProvider, App as AntdApp } from 'antd';
import enUS from 'antd/es/locale/en_US';
import App from './pages/App';
import { ThemeProvider, useThemeContext } from './context/ThemeContext';
import { getAntdTheme } from './config/antdTheme';

import './styles/theme.less';
import './index.less';
import 'overlayscrollbars/styles/overlayscrollbars.css';
import 'slick-carousel/slick/slick.css';
import 'slick-carousel/slick/slick-theme.css';
import 'github-markdown-css/github-markdown-light.css';

// 内部组件，用于获取主题上下文
function ThemedApp() {
  const { theme } = useThemeContext();

  return (
    <ConfigProvider
      locale={enUS}
      theme={getAntdTheme(theme)}
    >
      <AntdApp style={{ height: '100%' }}>
        <App />
      </AntdApp>
    </ConfigProvider>
  );
}

const container = document.getElementById('app');
const root = createRoot(container);

root.render(
  <ThemeProvider>
    <ThemedApp />
  </ThemeProvider>
);
