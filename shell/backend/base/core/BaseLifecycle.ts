import {App} from "../App";
import * as fs from "fs";
import * as path from "path";
import {ILifeCycle} from "./ILifeCycle";
import {sequelize} from "../../config/database";
import BuyBackModel from "../../models/BuyBackModel";
import CardModel from "../../models/CardModel";
import LaunchpadModel from "../../models/LaunchpadModel";
import CardStorageApplyModel from "../../models/CardStorageApplyModel";
import CardOutboundApplyModel from "../../models/CardOutboundApplyModel";
import AnnouncementModel from "../../models/AnnouncementModel";
import SubscribeModel from "../../models/SubscribeModel";
import {EventType} from "../../const/EventTypes";
import {CardLaunchedListener} from "../listener/CardLaunchedListener";
import {CardTransactedListener} from "../listener/CardTransactedListener";
import {CardOfflineListener} from "../listener/CardOfflineListener";

export class BaseLifecycle implements ILifeCycle {

    constructor() {

    }

    async initMsql() {
        await sequelize.authenticate();
        console.log('数据库连接成功');

        // 手动同步每个模型，表不存在则创建，存在则不修改
        await BuyBackModel.sync();
        await CardModel.sync();
        await LaunchpadModel.sync();
        await CardStorageApplyModel.sync();
        await CardOutboundApplyModel.sync();
        await AnnouncementModel.sync();
        await SubscribeModel.sync();
    }

    registerListener() {
        // 注册所有事件监听器
        App.EventBus.subscribe(EventType.CARD_LAUNCHED, new CardLaunchedListener(), 10);
        App.EventBus.subscribe(EventType.CARD_TRANSACTED, new CardTransactedListener(), 10);
        App.EventBus.subscribe(EventType.CARD_OFFLINE, new CardOfflineListener(), 10);
        console.log('All listeners registered');
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
