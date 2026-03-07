const {pool} = require('../database/connection');
const log4js = require('../config/log4js.config');
const logger = log4js.getLogger();

class TDKLine {

    // 插入
    static async create(ExchangeID, InstrumentID, Bar, InsertTime, OpenPrice, HighestPrice, LowestPrice, ClosePrice, Volume, Turnover, DBTime) {
        const sql = 'INSERT INTO t_D_KLine (ExchangeID, InstrumentID,Bar,InsertTime,OpenPrice,HighestPrice,LowestPrice,ClosePrice,Volume,Turnover,DBTime) ' +
            'VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)';
        try {
            const [result] = await pool.execute(sql, [ExchangeID, InstrumentID, Bar, InsertTime, OpenPrice, HighestPrice, LowestPrice, ClosePrice, Volume, Turnover, DBTime]);
            // const [result] = await pool.execute(sql, ['1', '1', '1', Date.now(), 1.02, 1.03, 1.04, 1.00, 1, 1.01, new Date()]);
            return result;
        } catch (err) {
            logger.error('插入失败: ' + err.message);
        }
    }

    // 插入
    static async createByObject(tDKLine) {
        const sql = 'INSERT INTO t_D_KLine (ExchangeID, InstrumentID,Bar,InsertTime,OpenPrice,HighestPrice,LowestPrice,ClosePrice,Volume,Turnover,DBTime) ' +
            'VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)';
        try {
            const [result] = await pool.execute(sql, [tDKLine.ExchangeID, tDKLine.InstrumentID, tDKLine.Bar, tDKLine.InsertTime, tDKLine.OpenPrice, tDKLine.HighestPrice, tDKLine.LowestPrice, tDKLine.ClosePrice, tDKLine.Volume, tDKLine.Turnover, tDKLine.DBTime]);
            return result;
        } catch (err) {
            logger.error('插入失败: ' + err.message);
        }
    }

    // 插入
    static async searchByDate(instrumentID, startInsertTime, endInsertTime) {
        const sql = `SELECT * FROM t_D_KLine WHERE InstrumentID = ? AND Bar = '1d' AND InsertTime >= ? AND InsertTime < ? ORDER BY InsertTime`;
        try {
            const [result] = await pool.execute(sql, [instrumentID, startInsertTime, endInsertTime]);
            return result;
        } catch (err) {
            logger.error('查询失败: ' + err.message);
        }
    }

}

module.exports = TDKLine;