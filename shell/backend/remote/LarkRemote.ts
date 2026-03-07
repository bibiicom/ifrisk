import {App} from "../base/App";
import {HttpRequest} from "../tool/HttpRequest";
import {BusinessError} from "../const/BusinessError";

export class LarkRemote {

    static getUrl(path: string) {
        return App.configManager.getServer().larkHost + path;
    }

    static async notifyLark(msg: string, type: string = "notify") {
        let url = this.getUrl("/lark/v1/lark/notify");
        const payload = {
            content: msg,
            type: type,
        };

        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.data
    }

}