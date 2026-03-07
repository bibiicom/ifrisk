const mysql = require('mysql2');
const log4js = require('../config/log4js.config');
const logger = log4js.getLogger();

// 加载数据库配置
const dbConfig = require('../config/db');

// console.log('数据库配置:', dbConfig);

// 创建连接池
const pool = mysql.createPool({
    host: dbConfig.host,
    port: dbConfig.port,
    user: dbConfig.user,
    password: dbConfig.password,
    database: dbConfig.database,
    waitForConnections: true,
    connectionLimit: 10,
    queueLimit: 0
});

// 获取连接
const promisePool = pool.promise();

// 测试连接
async function testConnection() {
    try {
        // 尝试获取连接
        const connection = await promisePool.getConnection();
        logger.info('成功连接到 MySQL 数据库!');

        // 执行简单查询
        const [rows] = await connection.query('SELECT 1 + 1 AS solution');
        logger.info('测试查询结果: ' + rows[0].solution);

        // 释放连接
        connection.release();

        return true;
    } catch (err) {
        logger.error('数据库连接失败: ' + err.message);
        return false;
    }
}

function closeConnection() {
    pool.end();
}

// 导出连接池
module.exports = {
    pool: promisePool,
    testConnection,
    closeConnection
};