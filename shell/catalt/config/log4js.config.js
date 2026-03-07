// log4js 配置文件
const log4js = require('log4js');

log4js.configure({
  appenders: {
    console: {
      type: 'console',
      layout: {
        type: 'pattern',
        pattern: '[%d{ISO8601}] [%p] %c - %m'
      }
    },
    file: {
      type: 'file',
      filename: 'logs/app.log',
      maxLogSize: 10485760, // 10MB
      backups: 3,
      compress: true,
      layout: {
        type: 'pattern',
        pattern: '[%d{ISO8601}] [%p] %c - %m'
      }
    }
  },
  categories: {
    default: { appenders: ['console', 'file'], level: 'info' },
    app: { appenders: ['console', 'file'], level: 'info' },
    database: { appenders: ['console', 'file'], level: 'info' },
    http: { appenders: ['console', 'file'], level: 'info' }
  }
});

module.exports = log4js;