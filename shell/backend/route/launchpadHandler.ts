import {BaseHandler} from "../base/core/BaseHandler";
import LaunchpadModel from "../models/LaunchpadModel";
import Decimal from "decimal.js";
import {ErrorCode} from "../const/ErrorCode";
import {Op, sequelize} from "../config/database";
import {BusinessError} from "../const/BusinessError";
import {LocalRemote} from "../remote/LocalRemote";
import CardModel from "../models/CardModel";
import SubscribeModel from "../models/SubscribeModel";
import {Currency} from "../const/Currency";
import BuyBackModel from "../models/BuyBackModel";

export default function () {
    return new launchpadHandler();
}
export var routeName: string = "backend/v1/launchpad";

export class launchpadHandler extends BaseHandler {
    async preHandle(msg: any, session: any): Promise<any> {
        const notAuthMethods = ["getLiveLaunchpadInfo", "getFutureLaunchpadInfo", "getEndLaunchpadInfo", "getLaunchpadInfoById", "subscriptionInfo", "getBuybackLaunchpadInfo"];
        if (notAuthMethods.includes(this.urlTemp.method)) {
            return null;
        }
        return super.preHandle(msg, session);
    }

    async getLiveLaunchpadInfo(msg: {}, session: any) {
        const now = new Date();
        const rows = await LaunchpadModel.findAll({
            where: {
                Status: 1,
                OpenTime: {[Op.lte]: now},
                CloseTime: {[Op.gt]: now},
            },
            order: [
                ['OpenTime', 'ASC']
            ],
            raw: true,
        });
        await this.buildLaunchpadInfoList(rows);
        return this.suc({
            launchpadList: rows,
        })
    }

    async getFutureLaunchpadInfo(msg: { page: string, limit: string }, session: any) {
        const page = parseInt(msg.page) || 1;
        const limit = parseInt(msg.limit) || 10;
        const offset = (page - 1) * limit;

        const now = new Date();
        const res = await LaunchpadModel.findAndCountAll({
            where: {
                Status: 1,
                OpenTime: {
                    [Op.or]: [
                        {[Op.gte]: now},
                        {[Op.eq]: null},
                    ],
                },
            },
            offset,
            limit,
            // 按开启时间升序排序 空值排最后
            order: [
                [sequelize.literal('`OpenTime` IS NULL'), 'ASC'],
                ['OpenTime', 'ASC']
            ],
            raw: true,
        });
        await this.buildLaunchpadInfoList(res.rows);
        return this.suc({
            launchpadList: res.rows,
            total: res.count,
        })
    }

    async getEndLaunchpadInfo(msg: { page: string, limit: string }, session: any) {
        const page = parseInt(msg.page) || 1;
        const limit = parseInt(msg.limit) || 10;
        const offset = (page - 1) * limit;
        const now = new Date();

        const res = await LaunchpadModel.findAndCountAll({
            where: {
                Status: {[Op.notIn]: [0]},
                CloseTime: {[Op.lte]: now},
            },
            offset,
            limit,
            order: [
                ['OpenTime', 'DESC']
            ],
            raw: true,
        });
        await this.buildLaunchpadInfoList(res.rows);
        return this.suc({
            launchpadList: res.rows,
            total: res.count,
        })
    }

    // 获取已回购列表
    async getBuybackLaunchpadInfo(msg: { page: string, limit: string }, session: any) {
        const page = parseInt(msg.page) || 1;
        const limit = parseInt(msg.limit) || 10;
        const offset = (page - 1) * limit;

        const res = await LaunchpadModel.findAndCountAll({
            where: {
                Status: 3,
            },
            offset,
            limit,
            order: [
                ['updatedAt', 'DESC']
            ],
            raw: true,
        });
        let idList = res.rows.map(row => row.Id);
        let buyBackModelList = await BuyBackModel.findAll({
            where: {
                LaunchpadID: {[Op.in]: idList},
            },
            raw: true,
            paranoid: false,
        });
        let buyBackModelMap = {};
        for (let buyBackModel of buyBackModelList) {
            buyBackModelMap[buyBackModel.LaunchpadID] = buyBackModel;
        }
        for (let row of res.rows) {
            row["BuyBackInfo"] = buyBackModelMap[row.Id] || {};
        }
        return this.suc({
            launchpadList: res.rows,
            total: res.count,
        })
    }

    private async buildLaunchpadInfoList(rows: LaunchpadModel[]) {
        if (!rows || rows.length <= 0) {
            return;
        }
        let currencyList = await LocalRemote.SendQryCurrency();
        let currencyMap = {};
        for (let currency of currencyList) {
            currencyMap[currency.data.Currency] = currency.data;
        }
        // 获取不为空的卡片ID列表 并去重
        let cardIDList = rows.filter(row => row.CardID).map(row => row.CardID).filter((item, index, arr) => arr.indexOf(item) === index);
        let cardModelList = await CardModel.findAll({
            where: {
                Id: {[Op.in]: cardIDList},
            },
            raw: true,
            paranoid: false,
        });
        let cardModelMap = {};
        for (let cardModel of cardModelList) {
            cardModelMap[cardModel.Id] = cardModel;
        }
        // 获取卡片的申购数量之和
        let subscribeModelList = await SubscribeModel.findAll({
            where: {
                LaunchpadID: {[Op.in]: rows.map(row => row.Id)},
            },
            raw: true,
        });
        let subscribeModelMap = {};
        for (let subscribeModel of subscribeModelList) {
            if (!subscribeModelMap[subscribeModel.LaunchpadID]) {
                subscribeModelMap[subscribeModel.LaunchpadID] = new Decimal("0");
            }
            subscribeModelMap[subscribeModel.LaunchpadID] = subscribeModelMap[subscribeModel.LaunchpadID].add(new Decimal(subscribeModel.Volume));
        }
        // 关联查询卡片信息
        for (let row of rows) {
            row["CardInfo"] = cardModelMap[row.CardID];
            row["InstrumentID"] = currencyMap[row.Currency]?.InstrumentID || "";
            row["SubscribeVolume"] = subscribeModelMap[row.Id]?.toFixed() || "0";
        }
    }

    async getLaunchpadInfoById(msg: { id: string }, session: any) {
        const owner = session.userID;
        const id = parseInt(msg.id) || 0;
        if (id <= 0) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        const res = await LaunchpadModel.findOne({
            where: {
                Id: id,
                Status: {[Op.ne]: 0},
            },
            raw: true,
        });
        if (!res) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let currencyList = await LocalRemote.SendQryCurrency(res.Currency);
        for (let currency of currencyList) {
            if (currency.data.Currency == res.Currency) {
                res["InstrumentID"] = currency.data.InstrumentID;
            }
        }
        // 计算所有的申购量
        let subscribeVolume = await SubscribeModel.sum('Volume', {
            where: {
                LaunchpadID: res.Id,
            },
        });
        res["SubscribeVolume"] = subscribeVolume || 0;

        let cardModel = await CardModel.findByPk(res.CardID, {raw: true, paranoid: false});
        // 获取自己一共申购的数量
        let totalVolume = new Decimal("0");
        let totalPrice = new Decimal("0");
        if (owner) {
            let subscribeModelList = await SubscribeModel.findAll({
                where: {
                    LaunchpadID: id,
                    AccountID: session.userID,
                },
            });
            for (let subscribeModel of subscribeModelList) {
                totalVolume = totalVolume.add(subscribeModel.Volume);
                totalPrice = totalPrice.add(subscribeModel.TotalPrice);
            }
        }
        return this.suc({
            launchpadInfo: res,
            cardInfo: cardModel,
            totalVolume: totalVolume.toFixed(),
            totalPrice: totalPrice.toFixed(),
        });
    }

    async subscription(msg: {
        launchpadID: number,
        volume: string,
    }, session: any) {
        let accountID = session.userID;
        if (!accountID) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let currency = Currency.USDC;
        // 验证购买数量是否为正整数
        const buyVolume = new Decimal(msg.volume || "0");
        if (!buyVolume.isInt() || buyVolume.lte("0")) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        // 查询数据
        let launchpadModel = await LaunchpadModel.findByPk(msg.launchpadID);
        if (!launchpadModel) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        let orderID = null;
        // 使用事务确保原子性操作
        return await sequelize.transaction(async (t) => {
            // 检查时间窗口
            const now = new Date();
            if (now < launchpadModel.OpenTime || now > launchpadModel.CloseTime) {
                return this.fail(ErrorCode.enum.NOT_IN_TRADING_TIME);
            }
            // 购买总价
            const price = buyVolume.mul(launchpadModel.Price);
            // 累计购买金额不能超过最大额度
            let totalPrice = await SubscribeModel.sum('TotalPrice', {
                where: {
                    LaunchpadID: msg.launchpadID,
                    AccountID: accountID,
                },
                transaction: t,
            });
            if (price.lt(launchpadModel.MinPrice)) {
                throw BusinessError.new(ErrorCode.enum.PRICE_OUT_OF_RANGE);
            }
            const totalPriceDec = new Decimal(String(totalPrice || "0")).add(price);
            if (launchpadModel.MaxPrice.gt("0") && totalPriceDec.gt(launchpadModel.MaxPrice)) {
                throw BusinessError.new(ErrorCode.enum.PRICE_OUT_OF_RANGE);
            }

            //冻结金额
            let orderFrozenMoneyRes = await LocalRemote.orderFrozenMoney(accountID, price.toFixed(), launchpadModel.Price.toFixed(), buyVolume.toFixed(), currency, launchpadModel.Currency, null, `sub_c_${launchpadModel.Currency}`);
            for (let orderFrozenMoneyRe of orderFrozenMoneyRes) {
                if (orderFrozenMoneyRe.table == 'Account') {

                } else if (orderFrozenMoneyRe.table == 'AccountAction') {
                    let data = orderFrozenMoneyRe.data;
                    orderID = data.OrderID;
                    console.log(' OrderID:', data.OrderID, ' AccountID:', data.AccountID, ' Amount:', data.Amount);
                }
            }

            // 创建申购记录
            await SubscribeModel.create({
                LaunchpadID: launchpadModel.Id,
                Currency: launchpadModel.Currency,
                AccountID: accountID,
                Volume: buyVolume,
                Price: launchpadModel.Price,
                TotalPrice: price,
                OrderID: orderID,
            }, {transaction: t});

            return {volume: buyVolume.toFixed(), totalPrice: price.toFixed()};
        }).then((res) => {
            return this.suc(res);
        }).catch((error) => {
            if (orderID) {
                // 回滚订单冻结金额
                LocalRemote.releaseFrozenMoneyByOrderID(accountID, orderID);
            }
            if (error instanceof BusinessError) {
                return this.fail(error.key, error.message);
            } else {
                console.error('Subscription system error:', error);
                return this.fail(ErrorCode.enum.SERVER_ERROR);
            }
        });

    }

    async subscriptionInfo(msg: { launchpadID: number, filter: boolean, page: string, limit: string }, session: any) {
        const page = parseInt(msg.page) || 1;
        const limit = parseInt(msg.limit) || 10;
        const offset = (page - 1) * limit;
        const owner = session.userID;

        if (msg.filter && !owner) {
            return this.suc({
                list: [],
                total: 0,
            });
        }

        let where: any = {
            LaunchpadID: msg.launchpadID,
        };
        if (msg.filter && owner) {
            where.AccountID = owner;
        }
        let res = await SubscribeModel.findAndCountAll({
            where: where,
            offset: offset,
            limit: limit,
            order: [['Id', 'DESC']],
            raw: true,
        });

        return this.suc({
            list: res.rows,
            total: res.count,
        });
    }
}