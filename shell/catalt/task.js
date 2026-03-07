// 应用入口文件
const Alt = require("./sub/alt");
const {getLogger} = require("./config/log4js.config");
const logger = getLogger();

async function main() {

    // 开始抓取数据
    let alt = new Alt();
    await alt.doUpdatePrice();
    await alt.doGenerateFile();

    setInterval(async function () {
        logger.info('开始更新价格...')
        await alt.doUpdatePrice();
        logger.info('更新价格完成。')
    }, 1000 * 60 * 5);

    setInterval(async function () {
        logger.info('开始生成文件...')
        await alt.doGenerateFile();
        logger.info('生成文件完成。')
    }, 1000 * 60);

}

// 执行主函数
main();