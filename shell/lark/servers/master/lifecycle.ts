import {BaseLifecycle} from "../../base/core/BaseLifecycle";
import {ILifeCycle} from "../../base/core/ILifeCycle";
import {App} from "../../base/App";
import {EnvStr} from "../../const/EvnStr";
import {startSchedule} from "../../task/schedule";


export default function () {
    return new Lifecycle();
}


class Lifecycle extends BaseLifecycle implements ILifeCycle {
    async beforeStartup() {
        const masterConfig = App.configManager.getMaster();
        App.startParam = Object.assign(App.startParam, masterConfig)
        App.startParam.isStatic = false
        App.startParam.client_ip = masterConfig.client_ip
        App.startParam.client_port = masterConfig.client_port
        App.startParam.port = masterConfig.port
        App.startParam.id = masterConfig.id

        super.initLark();

    }

    async afterStartup() {
        console.log(App.startParam.id, '启动完成');

        startSchedule();

        if (App.configManager.getEnv() == EnvStr.Prod) {

        }
    }

}


