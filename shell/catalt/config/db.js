//数据库配置
// const dbConfig = {
//     host: '127.0.0.1',
//     port: 3306,
//     database: 'perpetual',
//     user: 'root',
//     password: 'root'
// };
const dbConfig = {
    host: '127.0.0.1',
    port: 3306,
    database: 'perpetual',
    user: 'root',
    password: '!QAZ2wsx'
};

module.exports = {
    host: dbConfig.host,
    port: dbConfig.port,
    user: dbConfig.user,
    password: dbConfig.password,
    database: dbConfig.database
};