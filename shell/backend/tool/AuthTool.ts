import {LocalRemote} from "../remote/LocalRemote";

export class AuthTool {
    static async authSessionForHandler(opt: {token: string, session: any, type: string }) {
        if (opt.token && opt.session.token && opt.token == opt.session.token) {
            return true;
        }
        try {
            let res = await LocalRemote.sendCheckToken(opt.token);
            opt.session.token = res[0].data.Token;
            opt.session.userID = res[0].data.UserID;
            opt.session.memberID = res[0].data.MemberID;
        } catch (e) {
            return false;
        }
        return true;
    }
}
