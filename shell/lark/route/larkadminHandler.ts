import {BaseHandler} from "../base/core/BaseHandler";
import {App} from "../base/App";
import {AuthTool} from "../tool/AuthTool";
import {HttpRequest} from "../tool/HttpRequest";
import {ErrorCode} from "../const/ErrorCode";
import * as lark from "@larksuiteoapi/node-sdk";

/**
 * Lark JSAPI 认证处理器
 */
export default function () {
    return new LarkAdminHandler();
}

export var routeName: string = "lark/v1/admin";

export class LarkAdminHandler extends BaseHandler {

    async preHandle(msg: any, session: any): Promise<any> {
        const notAuthMethods = ["jsapi", "newAuth"];
        if (notAuthMethods.includes(this.urlTemp.method)) {
            return null;
        }
        return super.preHandle(msg, session);
    }

    /**
     * 获取 JSAPI 认证参数
     * @param msg 请求参数
     * @param session 会话信息
     */
    async jsapi(msg: { url: string }, session: any) {
        const url = msg.url;
        if (!url) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        // 调用 AuthTool 获取 JSAPI 鉴权参数
        const authParams = await AuthTool.getJsapiAuthParams(url);
        return this.suc(authParams);
    }

    /**
     * 处理免登请求，返回用户的user_access_token
     * @param msg 请求参数
     * @param session 会话信息
     */
    async newAuth(msg: { code: string, redirect_uri?: string }, session: any) {
        const code = msg.code;
        if (!code) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        const config = App.configManager.getServer().lark;

        // 【请求】user_access_token - 使用 v2/oauth/token 接口
        const auth2Res = await HttpRequest.post(
            "https://open.larksuite.com/open-apis/authen/v2/oauth/token",
            {
                grant_type: "authorization_code",
                client_id: config.app_id,
                client_secret: config.app_secret,
                code: code,
                redirect_uri: msg.redirect_uri || undefined,
                // code_verifier: msg.code_verifier || undefined,
                // scope: msg.scope || undefined
            }, {
                headers: {
                    "Content-Type": "application/json; charset=utf-8"
                }
            }
        );

        if (!auth2Res || auth2Res.code !== 0) {
            // 打印错误信息
            console.error("获取user_access_token失败:", auth2Res);
            return this.fail(ErrorCode.enum.SERVER_ERROR);
        }

        // 将user_access_token保存到会话中
        let userInfoRes = await App.LarkClient.authen.userInfo.get({},
            lark.withUserAccessToken(auth2Res.access_token)
        );

        if (!userInfoRes || userInfoRes.code !== 0) {
            // 打印错误信息
            console.error("获取user_info失败:", userInfoRes);
            return this.fail(ErrorCode.enum.SERVER_ERROR);
        }
        // 合并userInfoRes.data到auth2Res
        Object.assign(auth2Res, userInfoRes.data);
        session.userinfo = auth2Res;

        // console.log("userAccessToken:", auth2Res);
        return this.suc(auth2Res);
    }
}