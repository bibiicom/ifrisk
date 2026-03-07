export class ErrorCode {
    /* 1 - 200 通用返回码 */
    public static enum: any = {
        'SUCCESS': 0, 0: "ok",
        // 'SUCCESS': 200, 200: "ok",
        'SERVER_ERROR': 500, 500: "SERVER_ERROR",
        //通用错误官方回复类
        'ERR_PARAM': 201, 201: "参数错误",
        'ERR_CFG': 202, 202: "配置错误",
        'ERR_SIGN': 203, 203: "签名错误",
        'SERVER_CLOSED': 204, 204: "服务器维护中",
        'LOGIN_INVALID': 205, 205: "登录信息无效,请重新登录",
        'NO_HANDLER_TO_REQ': 207, 207: "没有可访问的接口",
        'GATE_CHANGE_NEED_LOGIN': 208, 208: "路由已变更 请重新登录",
        'SERVER_HANDLE_TIME_OUT': 209, 209: "接口请求超时",

        // 1000 - 1999 通用业务错误码
        'NOT_IN_TRADING_TIME': 1000, 1000: "不在交易时间",
        'OUT_OF_VOLUME': 1001, 1001: "已售罄",
        'NOT_ENOUGH_VOLUME': 1002, 1002: "数量不足",
        'SHARE_NOT_ENOUGH': 1003, 1003: "份额不足",
        'BALANCE_NOT_ENOUGH': 1004, 1004: "余额不足",
        'BUYBACK_ALREADY_APPLIED': 1005, 1005: "已申请回购",
        'NO_PRICE': 1006, 1006: "未有价格",
        'PRICE_OUT_OF_RANGE': 1007, 1007: "价格超出范围",

        // 2000 - 2999 验证错误码
        'INVALID_EMAIL_FORMAT': 2001, 2001: "邮箱格式不正确",
        'INVALID_PHONE_FORMAT': 2002, 2002: "手机号格式不正确",
    }
}
