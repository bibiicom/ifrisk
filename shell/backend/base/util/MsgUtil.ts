import {BaseClass} from "../core/BaseClass";
import {App} from "../App";

export class MsgUtil extends BaseClass {
    public EncodeMsg(msg: any) {
        let json_str = "";
        App.SystemUtil.safeRunFunc(() => {
            json_str = JSON.stringify(msg);
            let component = encodeURIComponent(json_str);
            json_str = this.base64ShiftedEncode(component, 1);
        }, this)
        return json_str;
    }


    public DecodeMsg(msg: string) {
        let json_data = {};
        App.SystemUtil.safeRunFunc(() => {
            let json_str = this.base64ShiftedDecode(msg, 1);
            let component = decodeURIComponent(json_str);
            json_data = JSON.parse(component);
        }, this)
        return json_data;
    }

    private shiftString(s, shiftSize) {
        // 将字符串中每个字符转换成它的 Unicode 码值
        let codes = s.split('').map(function (c) {
            return c.charCodeAt(0);
        });
        // 对每个字符码增加指定数量，实现错位操作
        let shiftedCodes = codes.map(function (code) {
            return (code + shiftSize) % 256;
        });
        // 将错位后的字符码组装回字符串
        let res_str = shiftedCodes.map(function (code) {
            return String.fromCharCode(code);
        }).join('');
        return res_str;
    }

    private base64ShiftedEncode(s, shiftSize) {
        // 先进行错位
        let shifted = this.shiftString(s, shiftSize);
        // 再进行 Base64 编码
        return btoa(shifted);
    }

    private base64ShiftedDecode(encoded, shiftSize) {
        // 先解码
        let decoded = atob(encoded);
        // 再进行反错位
        return this.shiftString(decoded, -shiftSize);
    }

// 在 JavaScript 中可以使用这些函数进行编码和解码
//     let originalStr = JSON.stringify({a:1,asf:1234,adsg:"fassfas"});
//     let shiftSize = 1; // 指定错位大小
//     let encoded = base64ShiftedEncode(originalStr, shiftSize);
//     console.log(encoded);
//
//     let decoded = base64ShiftedDecode(encoded, shiftSize);
//     console.log(JSON.parse(decoded));


}

