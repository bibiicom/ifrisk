import {BaseHandler} from "../base/core/BaseHandler";
import KLineModel from "../models/KLineModel";
import Decimal from "decimal.js";
import {LocalRemote} from "../remote/LocalRemote";
import {ErrorCode} from "../const/ErrorCode";
import BuyBackModel from "../models/BuyBackModel";
import LaunchpadModel from "../models/LaunchpadModel";
import {Op} from "../config/database";
import {Currency} from "../const/Currency";

export default function () {
    return new buybackHandler();
}
export var routeName: string = "backend/v1/buyback";

export class buybackHandler extends BaseHandler {
    async preHandle(msg: any, session: any): Promise<any> {
        const notAuthMethods = ["sevenDayPrice", "positionRank"];
        if (notAuthMethods.includes(this.urlTemp.method)) {
            return null;
        }
        return super.preHandle(msg, session);
    }

    async sevenDayPrice(msg: { instrumentID: string }, session: any) {
        let kLineModels: any[] = await KLineModel.findAll({
            where: {
                InstrumentID: msg.instrumentID,
                Bar: "1h",
            },
            order: [
                ['InsertTime', 'DESC'],
            ],
            limit: 7 * 24 - 1,
        });
        let priceList = [];
        try {
            let lastKLine = await LocalRemote.QryLastKLine(msg.instrumentID);
            kLineModels.push({
                InsertTime: lastKLine.data.InsertTime,
                ClosePrice: new Decimal(lastKLine.data.ClosePrice),
                Volume: new Decimal(lastKLine.data.Volume),
            });
        } catch (e) {
            return this.suc({priceList, avgPrice: "0"});
        }

        let totalPrice = new Decimal("0");
        let totalVolume = new Decimal("0");
        for (let kLineModel of kLineModels) {
            priceList.push({
                time: kLineModel.InsertTime,
                price: kLineModel.ClosePrice.toFixed(),
                volume: kLineModel.Volume.toFixed(),
            });
            totalPrice = totalPrice.add(kLineModel.ClosePrice.mul(kLineModel.Volume));
            totalVolume = totalVolume.add(kLineModel.Volume);
        }

        if (totalVolume.eq(0)) {
            return this.suc({priceList, avgPrice: "0"});
        }
        let avgPrice = totalPrice.div(totalVolume);
        return this.suc({priceList, avgPrice: avgPrice.toFixed(8)});
    }

    async positionRank(msg: { currency: string }, session: any) {
        let currencyList = await LocalRemote.getAccountCurrencyList(msg.currency);
        let rankList = [];
        for (let cu of currencyList) {
            // if (cu.data.Balance <= 0) {
            //     continue;
            // }
            rankList.push({
                accountId: cu.data.AccountID,
                balance: cu.data.Balance,
            });
        }
        let currencyInfo = await LocalRemote.SendQryCurrency(msg.currency);
        // 按余额从大到小排序 取前10个
        rankList.sort((a, b) => b.balance - a.balance);
        rankList = rankList.slice(0, 10);
        return this.suc({rankList: rankList, total: currencyInfo[0].data.Balance});
    }

    async applyBuyback(msg: { instrumentID: string }, session: any) {
        const owner = session.userID;
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

        let launchpadModel = await LaunchpadModel.findOne({
            where: {
                Currency: currency,
                Status: {[Op.in]: [2, 3]},
            }
        });
        if (!launchpadModel) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        // 是否已经申请过回购
        let buyBackModel = await BuyBackModel.findOne({
            where: {
                // LaunchpadID: launchpadModel.Id,
                AccountID: owner,
                Status: "0",
            },
            raw: true,
        });
        if (buyBackModel) {
            return this.fail(ErrorCode.enum.BUYBACK_ALREADY_APPLIED);
        }

        let ownerBalance = new Decimal("0");
        let currencyList = await LocalRemote.getAccountCurrencyList(currency);
        let totalBalance = new Decimal("0");
        let otherBalance = new Decimal("0");
        for (let cu of currencyList) {
            if (cu.data.Balance <= 0) {
                continue;
            }
            if (cu.data.AccountID === owner) {
                ownerBalance = new Decimal(cu.data.Balance);
            }
            totalBalance = totalBalance.add(cu.data.Balance);
        }
        otherBalance = totalBalance.sub(ownerBalance);
        // 检查ownerBalance是否占比超过67%
        if (ownerBalance.div(totalBalance).lt("0.67")) {
            return this.fail(ErrorCode.enum.SHARE_NOT_ENOUGH);
        }
        if (ownerBalance.eq(totalBalance)) {
            return this.fail(ErrorCode.enum.BUYBACK_ALREADY_APPLIED);
        }
        let kLineModels: any[] = await KLineModel.findAll({
            where: {
                InstrumentID: msg.instrumentID,
                Bar: "1h",
            },
            order: [
                ['InsertTime', 'DESC'],
            ],
            limit: 7 * 24,
        });
        try {
            let lastKLine = await LocalRemote.QryLastKLine(msg.instrumentID);
            kLineModels.push({
                InsertTime: lastKLine.data.InsertTime,
                ClosePrice: new Decimal(lastKLine.data.ClosePrice),
                Volume: new Decimal(lastKLine.data.Volume),
            });
        } catch (e) {
            return this.fail(ErrorCode.enum.NO_PRICE);
        }


        let totalPrice = new Decimal("0");
        let totalVolume = new Decimal("0");
        for (let kLineModel of kLineModels) {
            totalPrice = totalPrice.add(kLineModel.ClosePrice.mul(kLineModel.Volume));
            totalVolume = totalVolume.add(kLineModel.Volume);
        }
        let avgPrice = new Decimal("0");
        let amount = new Decimal("0");
        if (totalVolume.eq(0)) {
            return this.fail(ErrorCode.enum.NO_PRICE);
        } else {
            avgPrice = totalPrice.div(totalVolume);
            amount = avgPrice.mul(otherBalance);
        }
        console.log(instrumentID, avgPrice.toFixed(8), amount.toFixed(8), otherBalance.toFixed(8));
        try {
            // 冻结资金
            let orderID = "";
            let orderFrozenMoneyRes = await LocalRemote.orderFrozenMoney(owner, amount.toFixed(8), avgPrice.toFixed(8), otherBalance.toFixed(8), Currency.USDC);
            for (let orderFrozenMoneyRe of orderFrozenMoneyRes) {
                if (orderFrozenMoneyRe.table == 'Account') {

                } else if (orderFrozenMoneyRe.table == 'AccountAction') {
                    let data = orderFrozenMoneyRe.data;
                    orderID = data.OrderID;
                    console.log(' OrderID:', data.OrderID, ' AccountID:', data.AccountID, ' Amount:', data.Amount);
                }
            }
            // 停盘
            await LocalRemote.closeInstrument(instrumentID);
            // 冻结出金权限
            await LocalRemote.switchWithdraw(owner, false);
            // 解冻资金
            await LocalRemote.releaseFrozenMoneyByOrderID(owner, orderID);
            // 变更状态为已回购
            launchpadModel.Status = 3;
            await launchpadModel.save();
            // 生成回购表
            await BuyBackModel.create({
                LaunchpadID: launchpadModel.Id,
                Currency: currency,
                InstrumentID: instrumentID,
                AccountID: owner,
                Volume: new Decimal(otherBalance.toFixed(8)),
                Price: new Decimal(avgPrice.toFixed(8)),
                TotalPrice: new Decimal(amount.toFixed(8)),
                Status: 0,
            });

        } catch (e) {
            console.log("buyBack error:", e);
            return this.fail(ErrorCode.enum.BALANCE_NOT_ENOUGH);
        }
        return this.suc({amount: amount.toFixed(8)});
    }

    async buyBackStatus(msg: { instrumentID: string }, session: any) {
        const instrumentID = msg.instrumentID;
        if (!instrumentID) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let status = 0;
        const owner = session.userID;
        let buyBackModel = await BuyBackModel.count({
            where: {
                AccountID: owner,
                InstrumentID: msg.instrumentID,
                Status: "0",
            },
        });
        if (buyBackModel > 0) {
            status = 1;
        }
        return this.suc({status: status});
    }

    async currencyRank(msg: {}, session: any) {
        const owner = session.userID;
        let rankList = [];
        let currencyMap = {};
        let currencyList = await LocalRemote.SendQryCurrency();
        for (let currencyInfo of currencyList) {
            let currency = currencyInfo.data.Currency;
            currencyMap[currency] = {
                InstrumentID: currencyInfo.data.InstrumentID,
                Remark: currencyInfo.data.Remark,
            };
        }
        let accountInfoList = await LocalRemote.getAccountInfoList(owner);
        for (const accountCurrencyInfo of accountInfoList) {
            let currency = accountCurrencyInfo.data.Currency;
            let balance = accountCurrencyInfo.data.Balance;
            if (!currency || currency.length == 0 || balance <= 0) {
                continue;
            }
            let instrumentID = currencyMap[currency].InstrumentID;
            if (!instrumentID || instrumentID.length == 0) {
                continue;
            }
            let currencyInfo = await LocalRemote.SendQryCurrency(currency);
            let totalBalance = new Decimal(String(currencyInfo[0].data.Balance));

            rankList.push({
                currency: currency,
                // accountID: owner,
                instrumentID: instrumentID,
                balance: balance,
                totalBalance: totalBalance.toFixed(),
                remark: currencyMap[currency].Remark,
            })
        }

        return this.suc(rankList);
    }

}