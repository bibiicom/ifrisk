import {Op, Sequelize} from 'sequelize';
import {App} from "../base/App";

const mysqlConfig = App.configManager.getMysql();

const sequelize = new Sequelize(
    mysqlConfig.database,
    mysqlConfig.username,
    mysqlConfig.password,
    {
        host: mysqlConfig.host,
        port: mysqlConfig.port,
        dialect: 'mysql',
        logging: (sql: string) => console.log(sql),
        pool: {
            max: 20,
            min: 3,
            acquire: 30000,
            idle: 10000
        },
        timezone: '+08:00', // 东八时区
    }
);

export {sequelize, Op};
