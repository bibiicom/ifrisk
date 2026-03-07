/**
 * HTTP请求工具使用示例
 */

// const HttpRequest = require('../utils/httpRequest');
// const TDKLine = require("../models/TDKLine");
// const {closeConnection} = require("../database/connection");
const Alt = require("../sub/alt");

async function runExamples() {

    // let marketTransactions = await require('./market.json');
    let alt = new Alt();
    // await alt.handleWeekKLine("PikachuUSDT", 3 * 12 * 4);
    // await alt.handleMonthKLine("PikachuUSDT", 3 * 12);
    await alt.getOkxKLine();
    // closeConnection();
}


// 执行示例
runExamples();