import {BaseHandler} from "../base/core/BaseHandler";
import {Op} from "../config/database";
import {ErrorCode} from "../const/ErrorCode";
import AnnouncementModel from "../models/AnnouncementModel";

export default function () {
    return new announcementHandler();
}
export var routeName: string = "backend/v1/announcement";

export class announcementHandler extends BaseHandler {
    async preHandle(msg: any, session: any): Promise<any> {
        return null;
    }

    /**
     * 获取公告列表
     * @param msg
     * @param session
     * @returns
     */
    async getAnnouncement(msg: { type: string, subType: string, title: string, page: string, limit: string }, session: any) {
        const type = parseInt(msg.type);
        const subType = parseInt(msg.subType);
        const title = msg.title || "";
        const page = parseInt(msg.page) || 1;
        const limit = parseInt(msg.limit) || 10;
        const offset = (page - 1) * limit;

        let where: any = {
            Status: 2,
        }
        if (title) {
            where.Title = {
                [Op.like]: `%${title}%`,
            }
        }
        if (type) {
            where.Type = type;
        }
        if (subType) {
            where.SubType = subType;
        }
        let announcementList = await AnnouncementModel.findAndCountAll({
            attributes: ['Id', 'Title', 'Type', 'SubType', 'createdAt'],
            where: where,
            order: [
                ['Priority', 'DESC'],
                ['createdAt', 'DESC'],
            ],
            limit: limit,
            offset: offset,
            raw: true,
        });
        return this.suc({announcementList: announcementList.rows, total: announcementList.count});
    }

    /**
     * 获取公告详情
     * @param msg
     * @param session
     * @returns
     */
    async getAnnouncementDetail(msg: { id: string }, session: any) {
        const id = parseInt(msg.id);
        let announcement = await AnnouncementModel.findOne({
            attributes: ['Id', 'Title', 'Content', 'Type', 'SubType', 'createdAt'],
            where: {
                Id: id,
                Status: 2,
            },
            raw: true,
        });
        if (!announcement) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return this.suc(announcement);
    }

}