import {preload} from "./preload";
import {App, StartParamDefine} from "./base/App";
import {startApp} from "./base/koaApp";
import * as fs from "fs";
import {ILifeCycle} from "./base/core/ILifeCycle";
import {RouteUtil} from "./tool/RouteUtil";

export class application {
    constructor() {

    }

    public async init() {
        let [nodePath, appPath] = process.argv;
        let startParam: StartParamDefine = App.SystemUtil.formatArgv(process.argv);
        App.startParam = startParam;
        App.startTime = new Date();
        App.rootPath = __dirname;
        console.log(`项目路径：${App.rootPath}`);
        await this.lifecycle?.loadOptions();
        preload();
        console.log(`${startParam.id || "master"} ${App.configManager.getEnv()} 开始启动`)
        await this.lifecycle?.beforeStartup();

        App.routeUtil = new RouteUtil();

        if (!startParam.port) {
            startApp(App.configManager.getServer().httpPort, "http");
        } else {
            startApp(startParam.port);
        }
        await this.lifecycle?.afterStartup();
        this.logGameBanner();
        App.startParam.started = true;

    }

    public get lifecycle(): ILifeCycle {
        let server_dir = `./servers/${App.startParam.serverType || "master"}`;
        const stats1 = fs.existsSync(server_dir)
        if (!stats1) {
            return;
        }
        let lifecycle_path = `./servers/${App.startParam.serverType || "master"}/lifecycle`;

        let lifecycle = require(lifecycle_path);

        if (typeof lifecycle.default === "function") {
            return lifecycle.default.call(this);
        }
        return null;
    }

    private logGameBanner() {
        console.warn(
            ` 
 ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄               ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄ 
▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌             ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌
▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀█░▌ ▐░▌           ▐░▌ ▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀█░▌
▐░▌          ▐░▌          ▐░▌       ▐░▌  ▐░▌         ▐░▌  ▐░▌          ▐░▌       ▐░▌
▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄█░▌   ▐░▌       ▐░▌   ▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄█░▌
▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌    ▐░▌     ▐░▌    ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌
 ▀▀▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀█░█▀▀      ▐░▌   ▐░▌     ▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀█░█▀▀ 
          ▐░▌▐░▌          ▐░▌     ▐░▌        ▐░▌ ▐░▌      ▐░▌          ▐░▌     ▐░▌  
 ▄▄▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░▌      ▐░▌        ▐░▐░▌       ▐░█▄▄▄▄▄▄▄▄▄ ▐░▌      ▐░▌ 
▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌        ▐░▌        ▐░░░░░░░░░░░▌▐░▌       ▐░▌
 ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀         ▀          ▀          ▀▀▀▀▀▀▀▀▀▀▀  ▀         ▀ 
                                                                                                                             启动成功................................${(new Date().getTime() - App.startTime.getTime()) / 1000}s`
        )

    }
}

App.app = new application()
App.app.init()


