/**
 * 代理配置文件
 * 开发环境代理到 scardex.com，避免跨域
 */
const { createProxyMiddleware } = require('http-proxy-middleware')

module.exports = function (app) {
  const proxyConfig = {
    target: 'http://43.134.74.19',
    changeOrigin: true,
    // logLevel: 'debug',x x
    timeout: 900000,
    proxyTimeout: 900000
  }

  // const wsConfig = {
  //   target: 'ws://43.134.74.19',
  //   changeOrigin: true,
  //   ws: true,
  // }

  const apiPaths = [
    '/action',
    '/admin',
    '/public',
    '/private',
    '/query',
    '/backend',
    '/wallet',
    '/front',
    '/request',
    '/his1h',
    '/hisw',
    '/api'
  ]

  apiPaths.forEach(path => {
    app.use(path, createProxyMiddleware(proxyConfig))
  })

  // app.use('/wspublic', createProxyMiddleware(wsConfig))
}
