import {HttpRequest} from "./HttpRequest";
import {App} from "../base/App";
import * as crypto from "crypto";

interface TokenCache {
    token: string;
    expireTime: number;
}

export class AuthTool {
    // Token cache
    private static jsapiTicketCache: TokenCache = null;

    /**
     * 获取JSAPI鉴权参数
     * @param url 当前网页URL
     */
    static async getJsapiAuthParams(url: string): Promise<{
        appId: string;
        timestamp: number;
        nonceStr: string;
        signature: string;
    }> {
        const appId = App.configManager.getServer().lark?.app_id;
        if (!appId) {
            throw new Error("缺少Lark app_id配置");
        }
        // 获取jsapi_ticket
        const jsapiTicket = await this.getJsapiTicket();
        // 生成随机字符串
        const nonceStr = this.generateNonceStr();
        // 获取当前时间戳（秒）
        const timestamp = Date.now();
        // 生成签名
        const signature = this.generateSignature(jsapiTicket, nonceStr, timestamp, url);

        return {
            appId,
            timestamp,
            nonceStr,
            signature
        };
    }

    /**
     * 获取jsapi_ticket
     */
    static async getJsapiTicket(): Promise<string> {
        // 检查缓存是否有效
        if (this.jsapiTicketCache && Date.now() < this.jsapiTicketCache.expireTime) {
            return this.jsapiTicketCache.token;
        }

        // 获取tenant_access_token
        const tenantAccessToken = await App.LarkClient.tokenManager.getCustomTenantAccessToken();

        const response = await HttpRequest.get("https://open.larksuite.com/open-apis/jssdk/ticket/get/", {
                headers: {
                    "Authorization": `Bearer ${tenantAccessToken}`
                }
            }
        );
        console.log("获取jsapi_ticket响应:", JSON.stringify(response));
        if (response.code !== 0) {
            throw new Error(`获取jsapi_ticket失败: ${response.msg}`);
        }

        // 缓存ticket，提前5分钟过期
        const expireTime = Date.now() + (response.data.expire_in - 300) * 1000;
        this.jsapiTicketCache = {
            token: response.data.ticket,
            expireTime
        };

        return response.data.ticket;

    }

    /**
     * 生成随机字符串
     */
    static generateNonceStr(length: number = 16): string {
        const chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        let result = "";
        for (let i = 0; i < length; i++) {
            result += chars.charAt(Math.floor(Math.random() * chars.length));
        }
        return result;
    }

    /**
     * 生成签名
     * @param jsapiTicket jsapi_ticket
     * @param nonceStr 随机字符串
     * @param timestamp 时间戳
     * @param url 当前网页URL
     */
    //计算鉴权参数
    static generateSignature(jsapiTicket: string, nonceStr: string, timestamp: number, url: string) {
        const verifyStr = `jsapi_ticket=${jsapiTicket}&noncestr=${nonceStr}&timestamp=${timestamp}&url=${url}`
        const sha1 = crypto.createHash("sha1");
        sha1.update(verifyStr);
        return sha1.digest("hex");
    }

    static async authSessionForHandler(opt: { token: string, session: any, type: string }) {
        return true;
    }
}
