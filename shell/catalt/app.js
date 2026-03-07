// 应用入口文件
const {testConnection, closeConnection} = require('./database/connection');
const Alt = require("./sub/alt");
const log4js = require('./config/log4js.config');
const logger = log4js.getLogger();

async function main() {
    logger.info('开始测试 MySQL 连接...');

    const isConnected = await testConnection();
    if (isConnected) {
        logger.info('MySQL 连接测试成功！');
    } else {
        logger.error('MySQL 连接测试失败！');
        return
    }
    // 开始抓取数据
    await new Alt().doGetKLine2();

    closeConnection();
}

// 执行主函数
main();