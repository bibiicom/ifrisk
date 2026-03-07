import {BaseHandler} from "../base/core/BaseHandler";
import FinishOrderModel from "../models/FinishOrderModel";
import {Op} from "../config/database";

export default function () {
    return new finishOrderHandler();
}
export var routeName: string = "backend/v1/finishOrder";

export class finishOrderHandler extends BaseHandler {
    async preHandle(msg: any, session: any): Promise<any> {
        return super.preHandle(msg, session);
    }

    /**
     * 查询完成报单列表
     * @param msg.direction 买卖方向（可选）
     * @param msg.startTime 开始时间（可选，时间戳，毫秒）
     * @param msg.endTime 结束时间（可选，时间戳，毫秒）
     * @param msg.page 页码
     * @param msg.limit 每页数量
     */
    async getFinishOrderList(msg: {
        direction?: string;
        startTime?: number;
        endTime?: number;
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

        if (msg.direction) {
            where.Direction = msg.direction;
        }

        if (msg.startTime || msg.endTime) {
            where.InsertTime = {};
            if (msg.startTime) {
                where.InsertTime[Op.gte] = new Date(msg.startTime).getTime();
            }
            if (msg.endTime) {
                where.InsertTime[Op.lte] = new Date(msg.endTime).getTime();
            }
        }

        const res = await FinishOrderModel.findAndCountAll({
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
