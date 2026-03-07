import * as fs from "fs";
import * as path from "path";
import {App} from "../base/App";
import {BaseHandler} from "../base/core/BaseHandler";
import {ErrorCode} from "../const/ErrorCode";

export class RouteUtil {
    public handlerMap: { [routeName: string]: BaseHandler } = {}

    constructor() {
        this.initHandler();
    }

    public getHandlerMap() {
        return this.handlerMap;
    }


    public getHandler(routeName: string, method: string) {
        let handlerClass = this.handlerMap[routeName];
        if (!handlerClass) {
            return null;
        }
        return {handler: handlerClass[method], route: handlerClass};
    }


    public async dealHandle(route: BaseHandler, handler: Function, session: any, params: any) {
        if (!App.startParam.started) {
            return {
                code: ErrorCode.enum.SERVER_CLOSED,
                data: ErrorCode.enum[ErrorCode.enum.SERVER_CLOSED],
                param: []
            }
        }
        let result = null;
        //前置处理
        let beforeRes = await route.preHandle.call(route, params, session);
        if (beforeRes) {
            return beforeRes;
        }
        await App.SystemUtil.safeRunFuncAsync(async () => {
            result = await handler.call(route, params, session);
        }, this);
        //后置处理
        await route.afterHandle.call(route, params, session, result)

        return result;

    }

    private initHandler() {
        let routeDirPath = path.join(__dirname, "../route");
        let routes = fs.readdirSync(routeDirPath);
        this.watchDir(routeDirPath, this.loadHandler)
        routes.forEach(rPath => {
            let routePath = path.join(routeDirPath, rPath);
            this.loadHandler(routePath);
        })
    }

    private loadHandler(routePath: string) {
        let route = require(routePath);
        if (typeof route.default === "function") {
            let res = route.default.call();
            if (res) {
                this.handlerMap[route.routeName] = res;
            }
            console.warn(`${route.routeName} handler load 完成`)
        }
    }


    private watchDir(dir: string, handler: Function) {
        fs.watch(dir, (event, name) => {
            App.SystemUtil.safeRunFunc(() => {
                const serverConfig = App.configManager.getServer();
                if (!serverConfig.hotScript) {
                    return;
                }
                if (event === 'change') {
                    let routePath = path.join(dir, name);
                    let moduleObj = require.cache[path.join(dir, name)];
                    if (moduleObj && moduleObj.parent) {
                        moduleObj.parent.children.splice(moduleObj.parent.children.indexOf(moduleObj), 1);
                    }
                    delete require.cache[routePath];
                    handler.call(this, routePath);
                }
            }, this);
        });
    }


}
