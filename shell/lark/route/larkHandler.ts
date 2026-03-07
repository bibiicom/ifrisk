import {BaseHandler} from "../base/core/BaseHandler";
import {App} from "../base/App";

export default function () {
    return new larkHandler();
}
export var routeName: string = "lark/v1/lark";

export class larkHandler extends BaseHandler {

    async preHandle(msg: any, session: any): Promise<any> {
        return null;
    }

    async event(msg: any, session: any) {
        // console.debug("lark event:", JSON.stringify(msg));
        if (msg.type == "url_verification") {
            return {challenge: msg.challenge};
        }
        this.callFunction(msg);
        return 'success';
    }


    async callback(msg: any, session: any) {
        // console.log("lark callback:", JSON.stringify(msg));
        if (msg.type == "url_verification") {
            return {challenge: msg.challenge};
        }
        this.callFunction(msg);
        return {
            "toast": {
                "type": "success",
                "content": "成功",
                "i18n": {
                    "zh_cn": "成功",
                    "en_us": "success"
                }
            }
        };
    }

    private async callFunction(msg: any) {
        let fName = msg.event.type || msg.header.event_type;
        console.debug(`${fName}: `, JSON.stringify(msg));
        if (fName) {
            await App.eventBus.publish(fName, msg);
        } else {
            console.log("未找到事件类型:", msg);
        }
    }


    async notify(msg: any, session: any) {
        console.log("lark notify:", JSON.stringify(msg));
        if (msg.type == "money") {
            App.BotUtil.sendMDMsgToMoney(msg.content);
        } else {
            App.BotUtil.sendMDMsgToNotify(msg.content);
        }
        return this.suc({});
    }
}
