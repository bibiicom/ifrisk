import {BaseHandler} from "../base/core/BaseHandler";
import AccountDetailModel from "../models/AccountDetailModel";

export default function () {
    return new accountDetailHandler();
}
export var routeName: string = "backend/v1/accountDetail";

export class accountDetailHandler extends BaseHandler {
    async preHandle(msg: any, session: any): Promise<any> {
        return super.preHandle(msg, session);
    }

    /**
     * 查询账户流水列表
     * @param msg.Source 财务流水类型（可选）
     * @param msg.page 页码
     * @param msg.limit 每页数量
     */
    async getAccountDetailList(msg: {
        source?: string;
        page?: string;
        limit?: string;
    }, session: any) {
        const owner = session.userID;
        const page = parseInt(msg.page as string) || 1;
        const limit = parseInt(msg.limit as string) || 10;
        const offset = (page - 1) * limit;

        const where: any = {
            MemberID: owner,
        };

        if (msg.source) {
            where.Source = msg.source;
        }

        const res = await AccountDetailModel.findAndCountAll({
            where,
            offset,
            limit,
            order: [['InsertTime', 'DESC']],
            raw: true,
        });

        return this.suc({
            list: res.rows,
            total: res.count,
        });
    }
}
