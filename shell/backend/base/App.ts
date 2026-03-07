import {RouteUtil} from "../tool/RouteUtil";
import {SystemUtil} from "./util/SystemUtil";
import {application} from "../app";
import {ObjectUtils} from "./util/ObjectUtils";
import {MsgUtil} from "./util/MsgUtil";
import {KoaApp} from "./koaApp";
import ConfigManager from "../config/ConfigManager";
import {EventBus} from "./event/EventBus";


export class StartParamDefine {
    id: string
    port: number
    serverType: string
    client_ip: string
    client_port: number
    isStatic: boolean
    started: boolean
}

export class App {
    public static cacheMap: any = {}
    public static app: application
    public static koaApp: KoaApp

    public static startParam: StartParamDefine = new StartParamDefine()
    public static routeUtil: RouteUtil

    // 示例 C:\work\4、服务端\game_server
    public static rootPath: string
    public static startTime: Date
    public static configManager = new ConfigManager();

    public static get SystemUtil(): SystemUtil {
        return SystemUtil.getInstance()
    }

    public static get ObjectUtils(): ObjectUtils {
        return ObjectUtils.getInstance()
    }

    public static get MsgUtil(): MsgUtil {
        return MsgUtil.getInstance()
    }

    public static get EventBus(): EventBus {
        return EventBus.getInstance()
    }


    public static getKey(key: any): any
    public static getKey<T>(key: any): T
    public static getKey(key: any) {
        return App.cacheMap[key]
    }

    public static setKey(key: any, value: any): void {
        App.cacheMap[key] = value;
    }

}
