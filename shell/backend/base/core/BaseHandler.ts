import {ErrorCode} from "../../const/ErrorCode";
import {AuthTool} from "../../tool/AuthTool";

export class BaseHandler {
    public startTime: number;
    public urlTemp: any;

    public constructor() {

    }

    public async preHandle(msg: any, session: any): Promise<any> {
        let token = msg.token || session.otoken;
        // 接口前鉴权
        let auth = await AuthTool.authSessionForHandler({token, session, type: "scardex"});
        if (!auth) {
            console.warn(this.urlTemp)
            return this.fail(ErrorCode.enum.LOGIN_INVALID)
        }

        return null;
    }

    public afterHandle(msg: any, session: any, handler_result: any) {
        let useTime = Date.now() - this.startTime;
        if (useTime > 100) {
            console.warn(`接口请求 ${this.urlTemp.routeName}  ${this.urlTemp.method}  耗时${useTime}ms`);
        }

    }


    public suc(data: any) {
        return {
            code: 0,
            msg: '',
            data: data,
        }
    }

    public fail(key: number, ...args: any[]) {
        if (!args) {
            args = ['err'];
        }
        return {
            code: key,
            msg: ErrorCode.enum[key] || args[0],
            data: args
        };
    }

}
