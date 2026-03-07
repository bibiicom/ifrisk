import {App} from "../App";
import * as fs from "fs";
import * as path from "path";
import {ILifeCycle} from "./ILifeCycle";
import * as Lark from '@larksuiteoapi/node-sdk';
import {registerAllEventHandlers} from "../EventDecorator";

export class BaseLifecycle implements ILifeCycle {

    constructor() {

    }

    initLark() {
        const serverConfig = App.configManager.getServer();
        App.LarkClient = new Lark.Client({
            appId: serverConfig.lark.app_id,
            appSecret: serverConfig.lark.app_secret,
            domain: 'https://open.larksuite.com',
        });

        registerAllEventHandlers();
    }

    async loadOptions() {
        // App.configManager.reload();
        let dir = "../../config";
        let abs_path = path.join(__dirname, dir)
        fs.watch(abs_path, (event, name) => {
            let watchRes = App.SystemUtil.safeRunFunc(() => {
                if (event === 'change') {
                    let routePath = path.join(abs_path, name);
                    let moduleObj = require.cache[path.join(abs_path, name)];
                    if (moduleObj && moduleObj.parent) {
                        moduleObj.parent.children.splice(moduleObj.parent.children.indexOf(moduleObj), 1);
                    }
                    delete require.cache[path.resolve(__dirname, routePath)];
                    App.configManager.reload();
                    console.warn(`options 热更新 成功`, App.configManager.getAll())
                }
            }, this);
        });
    }

}