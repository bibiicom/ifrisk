import {BaseHandler} from "../base/core/BaseHandler";
import {Op} from "../config/database";
import {ErrorCode} from "../const/ErrorCode";
import AnnouncementModel from "../models/AnnouncementModel";
import CardModel from "../models/CardModel";
import LaunchpadModel from "../models/LaunchpadModel";
import {LocalRemote} from "../remote/LocalRemote";

export default function () {
    return new vaultHandler();
}
export var routeName: string = "backend/v1/vault";

export class vaultHandler extends BaseHandler {
    async preHandle(msg: any, session: any): Promise<any> {
        return null;
    }

    /**
     * 获取卡片列表
     * @param msg
     * @param session
     * @returns
     */
    async getCardList(msg: { page: string, limit: string, type: number }, session: any) {
        const page = parseInt(msg.page) || 1;
        const limit = parseInt(msg.limit) || 10;
        const offset = (page - 1) * limit;

        let where: any = {};
        if (msg.type) {
            switch (msg.type) {
                // 交易中
                case 1:
                    where.Status = 5;
                    break;
                // 申购中
                case 2:
                    where.Status = 2;
                    break;
                // 待上架
                case 3:
                    where.Status = 1;
                    break;
                default:
                    break;
            }
        }
        let cardRes = await CardModel.findAndCountAll({
            attributes: ['Id', 'Img', 'Name', 'ItemGrade'],
            where: where,
            // order: [
            //     ['createdAt', 'DESC'],
            // ],
            limit: limit,
            offset: offset,
            raw: true,
        });
        return this.suc({list: cardRes.rows, total: cardRes.count});
    }

    /**
     * 获取卡片详情
     * @param msg
     * @param session
     * @returns
     */
    async getCardDetail(msg: { id: string }, session: any) {
        const id = parseInt(msg.id);
        let cardModel = await CardModel.findOne({
            attributes: {
                exclude: ['Owner'],
            },
            where: {
                Id: id,
            },
            raw: true,
        });
        if (!cardModel) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        if (cardModel.Status == 2) {
            let launchpadModel = await LaunchpadModel.findOne({
                attributes: ['Id'],
                where: {
                    CardID: cardModel.Id,
                    Status: 1,
                },
                raw: true,
            });
            if (launchpadModel) {
                cardModel['LaunchpadID'] = launchpadModel.Id;
            }
        } else if (cardModel.Status == 5) {
            let launchpadModel = await LaunchpadModel.findOne({
                attributes: ['Id', 'Currency'],
                where: {
                    CardID: cardModel.Id,
                    Status: 2,
                },
                raw: true,
            });
            if (launchpadModel) {
                let currencyInfoList = await LocalRemote.SendQryCurrency(launchpadModel.Currency);
                let instrumentID = null;
                for (let currencyInfo of currencyInfoList) {
                    if (currencyInfo.data.Currency == launchpadModel.Currency) {
                        instrumentID = currencyInfo.data.InstrumentID;
                        break;
                    }
                }
                cardModel['InstrumentID'] = instrumentID;
            }
        }

        return this.suc(cardModel);
    }

}