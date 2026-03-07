module.exports = {
  root: true,
  parser: '@babel/eslint-parser', // 使用官方推荐解析器
  parserOptions: {
    requireConfigFile: false, // 如果没有 babel.config.js，可设置为 false
    babelOptions: {
      presets: [
        '@babel/preset-env',   // 支持最新 JS 特性
        '@babel/preset-react', // 支持 JSX
      ],
    },
    ecmaVersion: 2020,
    sourceType: 'module',
    ecmaFeatures: {
      jsx: true, // 开启 JSX 支持
    },
  },
  env: {
    browser: true,
    node: true,
    es6: true,
  },
  plugins: [
    'react',
    'react-hooks',
  ],
  extends: [
    'eslint:recommended',
    'plugin:react/recommended',
    'plugin:react-hooks/recommended',
  ],
  rules: {
    // 可根据项目需求自定义
    'no-unused-vars': 'warn',
    'react/prop-types': 'off', // 如果不使用 prop-types
    'react/react-in-jsx-scope': 'off', // React 17+ 不需要 import React
	'no-empty': 'off',
  },
  settings: {
    react: {
      version: 'detect',
    },
  },
};
