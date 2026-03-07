import {BaseHandler} from "../base/core/BaseHandler";
import CardModel from "../models/CardModel";
import {ErrorCode} from "../const/ErrorCode";
import LaunchpadModel from "../models/LaunchpadModel";
import {LocalRemote} from "../remote/LocalRemote";
import {LarkRemote} from "../remote/LarkRemote";
import CardStorageApplyModel from "../models/CardStorageApplyModel";
import CardOutboundApplyModel from "../models/CardOutboundApplyModel";
import {Op, sequelize} from "../config/database";
import {Currency} from "../const/Currency";

export default function () {
    return new cardHandler();
}
export var routeName: string = "backend/v1/card";

export class cardHandler extends BaseHandler {

    async preHandle(msg: any, session: any): Promise<any> {
        const notAuthMethods = ["getCardByInstrument"];
        if (notAuthMethods.includes(this.urlTemp.method)) {
            return null;
        }
        return super.preHandle(msg, session);
    }


    async getCardList(msg: { page: string, limit: string, category: string }, session: any) {
        const owner = session.userID;
        const category = parseInt(msg.category) || 0;

        const page = parseInt(msg.page) || 1;
        const limit = parseInt(msg.limit) || 10;
        const offset = (page - 1) * limit;
        let where = {
            Owner: owner,
            Status: {
                [Op.notIn]: [2, 5],
            },
        }
        if (category != 0) {
            where["Category"] = category;
        }
        const cardList = await CardModel.findAndCountAll({
            where,
            offset,
            limit,
            raw: true,
        });
        return this.suc({
            cardList: cardList.rows,
            total: cardList.count,
        });
    }

    async applyCard(msg: { cardId: string }, session: any) {
        const owner = session.userID;
        const cardId = msg.cardId;
        if (!cardId) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        const card = await CardModel.findByPk(cardId);
        if (!card || card.Owner != owner) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        if (card.Status != 0) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        card.Status = 1;
        await card.save();
        LarkRemote.notifyLark(`卡片上架申请：\n用户 ${owner} \n申请上架卡片 ${card.Name} ${card.Contract} #${card.TokenID}`);
        return this.suc(card.toJSON());
    }

    async withdrawCard(msg: { cardId: string }, session: any) {
        const owner = session.userID;
        const cardId = msg.cardId;
        if (!cardId) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        const card = await CardModel.findByPk(cardId);
        if (!card || card.Owner != owner || card.Category != 1) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        if (card.Status != 1) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        card.Status = 3;
        await card.save();
        return this.suc(card.toJSON());
    }

    async compositeCard(msg: { currency: string }, session: any) {
        const owner = session.userID;
        const launchpadModel = await LaunchpadModel.findOne({
            where: {
                Currency: msg.currency,
            }
        });
        if (!launchpadModel) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        const card = await CardModel.findByPk(launchpadModel.CardID);
        if (!card) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        // 出金
        try {
            let accountInfo = await LocalRemote.getAccountInfo(owner, launchpadModel.Currency);
            if (accountInfo && !launchpadModel.AllVolume.eq(accountInfo.data.Balance)) {
                return this.fail(ErrorCode.enum.SHARE_NOT_ENOUGH);
            }
            await LocalRemote.useMoneyByCurrency(owner, accountInfo.data.Balance, launchpadModel.Currency);
            console.log(owner, launchpadModel.Currency, "useMoneyByCurrency success");
        } catch (e) {
            console.error("compositeCard error:", e);
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        // if (card.Status != 5) {
        //     return this.fail(ErrorCode.enum.ERR_PARAM);
        // }
        card.Owner = owner;
        card.Status = 1;
        await card.save();
        if (launchpadModel.Status != 3) {
            launchpadModel.Status = 3;
            await launchpadModel.save();
        }
        // 移除交易对
        try {
            let currencyList = await LocalRemote.SendQryCurrency();
            for (let currency of currencyList) {
                if (currency.data.Currency == launchpadModel.Currency) {
                    await LocalRemote.closeInstrument(currency.data.InstrumentID);
                    await LocalRemote.sendSettlement(currency.data.InstrumentID);
                }
            }
        } catch (e) {
            console.error("compositeCard error:", e);
        }
        return this.suc(card.toJSON());
    }

    /**
     * 卡片入库申请接口
     * @param msg 请求参数
     * @param session 会话信息
     * @returns 申请结果
     */
    async storageApply(msg: {
        email: string;
    }, session: any) {
        // 邮箱格式验证
        const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
        const email = msg.email?.trim();
        if (!email || !emailRegex.test(email)) {
            return this.fail(ErrorCode.enum.INVALID_EMAIL_FORMAT);
        }
        const accountID = session.userID;
        // 使用事务确保数据一致性
        return await sequelize.transaction(async (t) => {
            // 检查用户是否已申请
            const existingApply = await CardStorageApplyModel.findOne({
                where: {
                    AccountID: accountID,
                    Status: 0, // 只检查待处理状态
                },
                transaction: t,
            });
            if (existingApply) {
                await existingApply.save({transaction: t});
                return this.suc({
                    applyRecord: existingApply
                });
            }
            // 创建NFT铸造申请记录
            const applyRecord = await CardStorageApplyModel.create({
                    AccountID: accountID,
                    Email: email,
                    Status: 0 // 初始状态为待处理
                },
                {transaction: t,}
            );
            LarkRemote.notifyLark(`卡片入库申请：\n用户 ${accountID} \n申请入库，\n邮箱 ${email}`);
            // 返回成功结果
            return this.suc({
                applyRecord: applyRecord
            });
        });

    }

    /**
     * 卡片出库申请接口
     * @param msg 请求参数
     * @param session 会话信息
     * @returns 申请结果
     */
    async outboundApply(msg: {
        cardID: string;
        postalCode: string;
        name: string;
        address: string;
        phone: string;
        email: string;
    }, session: any) {
        const accountID = session.userID;
        const cardID = parseInt(msg.cardID);
        const {postalCode, name, address, phone, email} = msg;

        if (!cardID || !postalCode || !name || !address || !phone || !email) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
        if (!emailRegex.test(email)) {
            return this.fail(ErrorCode.enum.INVALID_EMAIL_FORMAT);
        }

        let accountInfo = await LocalRemote.getAccountInfo(accountID, Currency.USDC);
        if (!accountInfo || accountInfo.data.Balance < 0) {
            return this.fail(ErrorCode.enum.BALANCE_NOT_ENOUGH);
        }

        return await sequelize.transaction(async (t) => {
            const card = await CardModel.findByPk(cardID, {transaction: t});
            if (!card || card.Status != 0 || card.Owner != accountID || card.Category != 2) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            const existingApply = await CardOutboundApplyModel.findOne({
                where: {
                    CardID: cardID,
                    Status: 0,
                },
                transaction: t,
            });

            if (existingApply) {
                return this.suc({
                    applyRecordId: existingApply.Id
                });
            }

            const applyRecord = await CardOutboundApplyModel.create({
                    CardID: cardID,
                    PostalCode: postalCode,
                    Name: name,
                    Address: address,
                    Phone: phone,
                    Email: email,
                    AccountID: accountID,
                    Status: 0
                },
                {transaction: t}
            );
            card.Status = 4;
            await card.save({transaction: t});
            LarkRemote.notifyLark(`卡片出库申请：\n用户 ${accountID} \n申请出库卡片 ${name}，\n邮箱 ${email}，\n地址 ${address}`);
            return this.suc({
                applyRecordId: applyRecord.Id
            });
        });
    }

    async getStorageApplyList(msg: { page: string, limit: string, status: string }, session: any) {
        const owner = session.userID;
        const page = parseInt(msg.page) || 1;
        const limit = parseInt(msg.limit) || 10;
        const offset = (page - 1) * limit;
        let where = {
            AccountID: owner,
        };
        if (msg.status != null && msg.status != "") {
            where["Status"] = parseInt(msg.status);
        }
        const applyList = await CardStorageApplyModel.findAndCountAll({
            where,
            offset,
            limit,
            raw: true,
            order: [['createdAt', 'DESC']],
        });
        for (let row of applyList.rows) {
            if (!row.CardID) {
                continue;
            }
            row['CardInfo'] = await CardModel.findByPk(row.CardID, {raw: true, paranoid: false});
        }
        return this.suc({
            list: applyList.rows,
            total: applyList.count,
        });
    }

    async getOutboundApplyList(msg: { page: string, limit: string, status: string }, session: any) {
        const owner = session.userID;
        const page = parseInt(msg.page) || 1;
        const limit = parseInt(msg.limit) || 10;
        const offset = (page - 1) * limit;
        let where = {
            AccountID: owner,
        };
        if (msg.status != null && msg.status != "") {
            where["Status"] = parseInt(msg.status);
        }
        const applyList = await CardOutboundApplyModel.findAndCountAll({
            where,
            offset,
            limit,
            raw: true,
            order: [['createdAt', 'DESC']],
        });
        for (let row of applyList.rows) {
            if (!row.CardID) {
                continue;
            }
            row['CardInfo'] = await CardModel.findByPk(row.CardID, {raw: true, paranoid: false});
        }
        return this.suc({
            list: applyList.rows,
            total: applyList.count,
        });
    }

    /**
     *获取交易对对应的卡片信息
     */
    async getCardByInstrument(msg: { instrumentID: string }, session: any) {
        const instrumentID = msg.instrumentID;
        if (!instrumentID) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let instrument = await LocalRemote.SendQryInstrument(instrumentID);
        if (!instrument) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let currency = instrument[0].data.BaseCurrency;
        if (!currency) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let launchpad = await LaunchpadModel.findOne({
            where: {
                Currency: currency,
            },
            raw: true,
        });
        if (!launchpad) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let card = await CardModel.findByPk(launchpad.CardID, {
            raw: true,
            paranoid: false,
        });
        return this.suc({
            card: card,
            launchpad: launchpad,
        });
    }

}
