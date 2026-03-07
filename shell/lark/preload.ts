import {App} from "./base/App";

/**
 *  自动解析sourcemap
 *  捕获全局错误
 */
export function preload() {
    App.SystemUtil.InitLog4js();

    // 自动解析ts的sourcemap
    require('source-map-support').install({
        handleUncaughtExceptions: false
    });


    // 捕获普通异常
    process.on('uncaughtException', function (err) {
        console.warn(App.startParam.id);
        console.warn(err);

    });

    // 捕获async异常
    process.on('unhandledRejection', (reason: any, p) => {
        console.warn(App.startParam.id + 'Caught Unhandled Rejection at:' + p + 'reason:' + reason, "ERR_ASYNC", "stack", reason.stack);
        console.warn(p);
    });

}

