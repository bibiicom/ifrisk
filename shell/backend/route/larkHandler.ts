import {BaseHandler} from "../base/core/BaseHandler";
import CardModel from "../models/CardModel";
import LaunchpadModel from "../models/LaunchpadModel";
import {Op, sequelize} from "../config/database";
import {ErrorCode} from "../const/ErrorCode";
import Decimal from "decimal.js";
import CardStorageApplyModel from "../models/CardStorageApplyModel";
import CardOutboundApplyModel from "../models/CardOutboundApplyModel";
import AnnouncementModel from "../models/AnnouncementModel";
import SubscribeModel from "../models/SubscribeModel";
import launchpadService from "../service/launchpadService";
import {CardLaunchedEventData, CardOfflineEventData} from "../base/event/EventData";
import {App} from "../base/App";
import {EventType} from "../const/EventTypes";
import {LocalRemote} from "../remote/LocalRemote";
import BuyBackModel from "../models/BuyBackModel";
import buybackService from "../service/buybackService";

export default function () {
    return new larkHandler();
}
export var routeName: string = "backend/v1/lark";

export class larkHandler extends BaseHandler {

    async preHandle(msg: any, session: any): Promise<any> {
        return null;
    }

    // 获取币种列表
    async getCurrencyList(msg: {}, session: any) {
        let currencyInfoList = await LocalRemote.SendQryCurrency();
        let currencyList = currencyInfoList.map((item: any) => {
            return {
                currency: item.data.Currency,
                instrumentID: item.data.InstrumentID,
            }
        });
        return this.suc(currencyList);
    }

    // 获取币种账户列表
    async getAccountCurrencyList(msg: { currency: string }, session: any) {
        let currency = msg.currency;
        if (!currency) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let accountCurrencyInfoList = await LocalRemote.getAccountCurrencyList(currency);
        let currencyList = accountCurrencyInfoList.map((item: any) => {
            return {...item.data}
        });
        return this.suc(currencyList);
    }

    // 获取申请上架卡片列表
    async getApplyCardList(msg: {}, session: any) {
        const cardList = await CardModel.findAll({
            where: {
                Status: 1,
            },
            raw: true,
        });
        return this.suc(cardList);
    }

    // 获取申请入库卡片列表
    async getStorageApplyCardList(msg: {}, session: any) {
        const cardStorageApplyModels = await CardStorageApplyModel.findAll({
            where: {
                Status: 0,
            },
            order: [
                ['updatedAt', 'DESC'],
            ],
            raw: true,
        });
        return this.suc(cardStorageApplyModels);
    }

    // 获取申请出库卡片列表
    async getOutboundApplyCardList(msg: {}, session: any) {
        const cardOutboundApplyModels = await CardOutboundApplyModel.findAll({
            where: {
                Status: 0,
            },
            order: [
                ['updatedAt', 'DESC'],
            ],
            raw: true,
        });
        for (let cardOutboundApplyModel of cardOutboundApplyModels) {
            cardOutboundApplyModel["CardInfo"] = await CardModel.findByPk(cardOutboundApplyModel.CardID, {raw: true});
        }
        return this.suc(cardOutboundApplyModels);
    }

    // 添加上架卡片信息
    async addLaunchpadInfo(msg: {
        currency: string,
        name: string,
        cardId: string,
        volume: string,
        price: string,
        // tradeStandardPct: string,
        haltStandardPct: string,
        feePct: string,
        minPrice: string,
        maxPrice: string,
        openTime: string,
        closeTime: string,
        // tradeTime: string,
        summary: string,
    }, session: any) {
        let currency = msg.currency;
        let name = msg.name;
        if (!currency || !name) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return await sequelize.transaction(async (t) => {
            let count = await LaunchpadModel.count({where: {Currency: currency}, transaction: t});
            if (count > 0) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            let launchpad = new LaunchpadModel();
            launchpad.Currency = currency;
            launchpad.Name = name;
            // 如果 msg.openTime 或者 msg.closeTime 有值， 其他字段必须有值
            if (msg.openTime || msg.closeTime) {
                if (!msg.closeTime || !msg.openTime) {
                    return this.fail(ErrorCode.enum.ERR_PARAM);
                }
                if (!msg.volume) {
                    return this.fail(ErrorCode.enum.ERR_PARAM);
                }
                if (!msg.feePct) {
                    msg.feePct = "0";
                }
                let feePct = new Decimal(String(msg.feePct));
                let volume = new Decimal(String(msg.volume));
                launchpad.AllVolume = volume;
                // 计算 RoundVolume 扣除手续费 保留整数
                launchpad.RoundVolume = volume.sub(volume.mul(feePct).div(100)).toDecimalPlaces(0);
                // launchpad.StockVolume = roundVolume;
                if (!msg.price) {
                    return this.fail(ErrorCode.enum.ERR_PARAM);
                }
                launchpad.Price = new Decimal(String(msg.price));
                // if (!msg.tradeStandardPct) {
                //     return this.fail(ErrorCode.enum.ERR_PARAM);
                // }
                // launchpad.TradeStandardPct = new Decimal(String(msg.tradeStandardPct));
                if (!msg.haltStandardPct) {
                    return this.fail(ErrorCode.enum.ERR_PARAM);
                }
                launchpad.HaltStandardPct = new Decimal(String(msg.haltStandardPct));
                launchpad.FeePct = feePct;
                launchpad.OpenTime = new Date(msg.openTime);
                launchpad.CloseTime = new Date(msg.closeTime);
                launchpad.Round = "A";
                launchpad.Summary = msg.summary;
                if (!msg.minPrice) {
                    msg.minPrice = "0";
                }
                launchpad.MinPrice = new Decimal(String(msg.minPrice));
                if (!msg.maxPrice) {
                    msg.maxPrice = "0";
                }
                launchpad.MaxPrice = new Decimal(String(msg.maxPrice));
                // launchpad.TradeTime = new Date(msg.tradeTime);
                launchpad.Status = 1;
            } else {
                // 都没值 预上架状态 存在就赋值
                if (msg.volume) {
                    launchpad.AllVolume = new Decimal(String(msg.volume));
                }
                launchpad.RoundVolume = null;
                // launchpad.StockVolume = null;
                if (msg.price) {
                    launchpad.Price = new Decimal(String(msg.price));
                }
                // if (msg.tradeStandardPct) {
                //     launchpad.TradeStandardPct = new Decimal(String(msg.tradeStandardPct));
                // }
                if (msg.haltStandardPct) {
                    launchpad.HaltStandardPct = new Decimal(String(msg.haltStandardPct));
                }
                if (msg.feePct) {
                    launchpad.FeePct = new Decimal(String(msg.feePct));
                }
                launchpad.OpenTime = null;
                launchpad.CloseTime = null;
                launchpad.Round = "A";
                launchpad.Summary = msg.summary;
                if (msg.minPrice) {
                    launchpad.MinPrice = new Decimal(String(msg.minPrice));
                }
                if (msg.maxPrice) {
                    launchpad.MaxPrice = new Decimal(String(msg.maxPrice));
                }
                // launchpad.TradeTime = null;
                launchpad.Status = 1;
            }

            let cardId = Number(msg.cardId);
            let cardModel = await CardModel.findOne({where: {Id: cardId}, transaction: t});
            if (!cardModel || cardModel.Status !== 1) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            launchpad.CardID = cardId;
            cardModel.Status = 2;
            await cardModel.save({transaction: t});
            await launchpad.save({transaction: t});
            return this.suc({launchpad, cardModel});
        }).then(value => {
            if (value.code == 0) {
                // 发布卡片上架事件
                const card = value.data.cardModel;
                const launchpad = value.data.launchpad;
                const eventData: CardLaunchedEventData = {
                    name: launchpad.Name,
                    symbol: launchpad.Currency,
                    openTime: launchpad.OpenTime,
                    closeTime: launchpad.CloseTime,
                    summary: card.Summary,
                };
                App.EventBus.publish(EventType.CARD_LAUNCHED, eventData);
            }
            return value;
        });

    }

    async updateLaunchpadInfo(msg: {
        launchpadId: string,
        closeTime: string,
        // tradeTime: string,
    }, session: any) {
        let launchpadId = Number(msg.launchpadId);
        if (!launchpadId) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return await sequelize.transaction(async (t) => {
            let launchpadModel = await LaunchpadModel.findByPk(launchpadId, {transaction: t});
            if (!launchpadModel || launchpadModel.Status !== 1) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            launchpadModel.CloseTime = new Date(msg.closeTime);
            // launchpadModel.TradeTime = new Date(msg.tradeTime);
            await launchpadModel.save({transaction: t});
            return this.suc(launchpadModel)
        });
    }

    async addCard(msg: {
        accountId: string,
        valuation: string,
        name: string,
        img: string,
        imgGroup?: string,
        description?: string,
        certNumber?: string,
        itemGrade?: string,
        year?: string,
        brandTitle?: string,
        subject?: string,
        cardNumber?: string,
        varietyPedigree?: string,
        status?: number,
    }, session: any) {
        // let applyId = Number(msg.applyId);
        if (!msg.accountId) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return await sequelize.transaction(async (t) => {
            // let applyModel = await CardStorageApplyModel.findByPk(applyId, {transaction: t});
            // if (!applyModel || applyModel.Status !== 0) {
            //     return this.fail(ErrorCode.enum.ERR_PARAM);
            // }
            let cardModel = await CardModel.create({
                Valuation: new Decimal(msg.valuation || "0"),
                Owner: msg.accountId,
                Img: msg.img,
                Status: msg.status || 1,
                Name: msg.name,
                Category: 2,
                Contract: "",
                TokenID: "",
                ImgGroup: msg.imgGroup || "",
                Description: msg.description || "",
                CertNumber: msg.certNumber || "",
                ItemGrade: msg.itemGrade || "",
                Year: msg.year || "",
                BrandTitle: msg.brandTitle || "",
                Subject: msg.subject || "",
                CardNumber: msg.cardNumber || "",
                VarietyPedigree: msg.varietyPedigree || "",
            }, {transaction: t});
            // applyModel.Status = 1;
            // applyModel.CardID = cardModel.Id;
            // await applyModel.save({transaction: t});
            return this.suc(cardModel)
        });
    }

    async doOutboundApply(msg: {
        applyId: string,
    }, session: any) {
        let applyId = Number(msg.applyId);
        if (!applyId) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return await sequelize.transaction(async (t) => {
            let applyModel = await CardOutboundApplyModel.findByPk(applyId, {transaction: t});
            if (!applyModel || applyModel.Status !== 0) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            let cardModel = await CardModel.findByPk(applyModel.CardID, {transaction: t});
            if (!cardModel) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            applyModel.Status = 1;
            await applyModel.save({transaction: t});
            await cardModel.destroy({transaction: t});
            return this.suc(applyModel)
        });
    }

    async rejectApply(msg: {
        applyId: string,
        type: number,
    }, session: any) {
        let applyId = Number(msg.applyId);
        if (!applyId) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return await sequelize.transaction(async (t) => {
            // 1 入库申请 2 上架申请  3 出库申请
            if (msg.type === 1) {
                const applyModel = await CardStorageApplyModel.findByPk(applyId, {transaction: t});
                if (!applyModel || applyModel.Status !== 0) {
                    return this.fail(ErrorCode.enum.ERR_PARAM);
                }
                applyModel.Status = 2;
                await applyModel.save({transaction: t});
            } else if (msg.type === 2) {
                const cardModel = await CardModel.findByPk(applyId, {transaction: t});
                if (!cardModel || cardModel.Status !== 1) {
                    return this.fail(ErrorCode.enum.ERR_PARAM);
                }
                // 上架申请被拒绝，将状态设置为持仓
                cardModel.Status = 1;
                await cardModel.save({transaction: t});
            } else if (msg.type === 3) {
                const applyModel = await CardOutboundApplyModel.findByPk(applyId, {transaction: t});
                if (!applyModel || applyModel.Status !== 0) {
                    return this.fail(ErrorCode.enum.ERR_PARAM);
                }
                applyModel.Status = 2;
                await applyModel.save({transaction: t});
            } else {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            return this.suc({});
        });
    }

    async addAnnouncement(msg: {
        title: string,
        content: string,
        fileName: string,
        docUrl: string,
        type: string,
        subType: string,
        priority: string,
    }, session: any) {
        const type = parseInt(msg.type);
        const subType = parseInt(msg.subType) || 0;
        if (!msg.title || !type) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let announcementModel = await AnnouncementModel.create({
            Title: msg.title,
            Content: msg.content || "",
            FileName: msg.fileName || "",
            DocUrl: msg.docUrl || "",
            Type: type,
            SubType: subType,
            Priority: Number(msg.priority) || 1,
            Status: 2,
        });
        return this.suc(announcementModel);
    }

    async updateAnnouncement(msg: {
        announcementId: string,
        title: string,
        content: string,
        fileName: string,
        docUrl: string,
        type: string,
        subType: string,
        status: string,
        priority: string,
    }, session: any) {
        let announcementId = Number(msg.announcementId);
        const type = parseInt(msg.type);
        const subType = parseInt(msg.subType);
        if (!announcementId) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return await sequelize.transaction(async (t) => {
            let announcementModel = await AnnouncementModel.findByPk(announcementId, {transaction: t});
            if (!announcementModel) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            if (msg.title) {
                announcementModel.Title = msg.title;
            }
            if (msg.content) {
                announcementModel.Content = msg.content;
            }
            if (msg.fileName) {
                announcementModel.FileName = msg.fileName;
            }
            if (msg.docUrl) {
                announcementModel.DocUrl = msg.docUrl;
            }
            if (type) {
                announcementModel.Type = type;
            }
            if (subType) {
                announcementModel.SubType = subType;
            }
            if (msg.status) {
                announcementModel.Status = Number(msg.status);
            }
            if (msg.priority) {
                announcementModel.Priority = Number(msg.priority);
            }
            await announcementModel.save({transaction: t});
            return this.suc(announcementModel);
        });
    }

    async deleteAnnouncement(msg: {
        announcementId: string,
    }, session: any) {
        let announcementId = Number(msg.announcementId);
        if (!announcementId) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return await sequelize.transaction(async (t) => {
            let announcementModel = await AnnouncementModel.findByPk(announcementId, {transaction: t});
            if (!announcementModel) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            await announcementModel.destroy({transaction: t});
            return this.suc({id: announcementId});
        });
    }

    async getAnnouncementDetail(msg: {
        announcementId: string,
    }, session: any) {
        let announcementId = Number(msg.announcementId);
        if (!announcementId) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let announcementModel = await AnnouncementModel.findByPk(announcementId, {raw: true});
        if (!announcementModel) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return this.suc(announcementModel);
    }

    async getAnnouncementListAndCount(msg: {
        type: string,
        subType: string,
        page: string,
        pageSize: string,
    }, session: any) {
        let where: any = {};
        const type = parseInt(msg.type);
        const subType = parseInt(msg.subType);
        const page = parseInt(msg.page) || 1;
        const pageSize = parseInt(msg.pageSize) || 10;
        const offset = (page - 1) * pageSize;

        if (type) {
            where.Type = type;
        }
        if (subType) {
            where.SubType = subType;
        }

        let announcementModels = await AnnouncementModel.findAndCountAll({
            where,
            order: [
                ['createdAt', 'DESC'],
            ],
            offset,
            limit: pageSize,
            raw: true,
        });

        return this.suc(announcementModels);
    }

    // 获取申购到期列表
    async getSubscriptionExpiryList(msg: {
        page: string,
        pageSize: string,
    }, session: any) {
        const page = parseInt(msg.page) || 1;
        const pageSize = parseInt(msg.pageSize) || 10;
        const offset = (page - 1) * pageSize;
        // 筛选已经到期的
        let result = await LaunchpadModel.findAndCountAll({
            where: {
                Status: 1,
                CloseTime: {
                    [Op.lt]: new Date(),
                }
            },
            order: [
                ['CloseTime', 'ASC'], // 按截止时间升序，优先显示即将到期的
            ],
            offset,
            limit: pageSize,
            raw: true,
        });
        for (let row of result.rows) {
            let subscribeVolume = await SubscribeModel.sum('Volume', {
                where: {
                    LaunchpadID: row.Id,
                },
            });
            row["SubscribeVolume"] = subscribeVolume || 0;
        }

        return this.suc(result);
    }

    // 申购审批
    async subscriptionApproval(msg: {
        launchpadId: string,
        select: string,
        exchangeVolume: string,
        exchangeTotalPrice: string,
        cardholderVolume: string,
        tradeTime: string,
    }, session: any) {
        const launchpadId = Number(msg.launchpadId);
        if (!launchpadId) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        const select = msg.select;
        if (select == "同意") {
            const exchangeVolume = new Decimal(String(msg.exchangeVolume || 0));
            const exchangeTotalPrice = new Decimal(String(msg.exchangeTotalPrice || 0));
            const cardholderVolume = new Decimal(String(msg.cardholderVolume || 0));
            const tradeTime = msg.tradeTime;
            if (!tradeTime) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            const tradeTimeObj = new Date(tradeTime);
            if (isNaN(tradeTimeObj.getTime())) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            await launchpadService.agreeSubscribe(launchpadId, exchangeVolume, exchangeTotalPrice, cardholderVolume, tradeTimeObj);
        } else if (select == "拒绝") {
            await launchpadService.rejectAndRefundSubscription(launchpadId);
        } else {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return this.suc({});
    }

    // 自动任务：删除空 Launchpad
    async deleteEmptyLaunchpads(msg: {}, session: any) {
        const result = await launchpadService.deleteEmptyLaunchpads();
        return this.suc(result);
    }

    // 获取申请上架卡片列表
    async getLarkCardList(msg: {}, session: any) {
        const cardList = await CardModel.findAll({
            where: {
                Status: 1,
                Description: "LarkAutoTask",
            },
            raw: true,
        });
        return this.suc(cardList);
    }

    // 获取可回购列表
    async getBuybackList(msg: {}, session: any) {
        let buybackList = [];
        let marketDataList = await LocalRemote.SendQryMarketData();
        let stopList = marketDataList.filter((item: any) => item.data.InstrumentStatus == "1" || item.data.InstrumentStatus == "7");
        for (let stopListElement of stopList) {
            let instrumentID = stopListElement.data.InstrumentID;
            let instrument = await LocalRemote.SendQryInstrument(instrumentID);
            //获取余额最多的账户
            let baseCurrency = instrument[0].data.BaseCurrency;
            let accountCurrencyList = await LocalRemote.getAccountCurrencyList(baseCurrency);
            let maxAccount = accountCurrencyList.reduce((prev: any, cur: any) => {
                return prev.data.Balance > cur.data.Balance ? prev : cur;
            });
            if (!maxAccount) {
                continue;
            }
            let launchpadModel = await LaunchpadModel.findOne({
                where: {
                    Currency: baseCurrency,
                    Status: {[Op.in]: [2, 3]},
                },
                raw: true,
            });
            if (!launchpadModel) {
                continue;
            }
            let buyBackModel = await BuyBackModel.count({
                where: {
                    LaunchpadID: launchpadModel.Id,
                    Status: "0",
                },
            });

            let buybackPrice = await buybackService.getBuybackPrice(instrumentID);
            buybackList.push({
                currency: baseCurrency,
                accountID: maxAccount.data.AccountID,
                totalAmount: launchpadModel.AllVolume,
                userAmount: maxAccount.data.Balance,
                buybackPrice: buybackPrice.toFixed(),
                canBuyback: buyBackModel <= 0
            })
        }
        return this.suc(buybackList);
    }

    async applyBuyback(msg: { currency: string }, session: any) {
        const currency = msg.currency;
        if (!currency) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        let currencyList = await LocalRemote.getAccountCurrencyList(currency);
        let maxAccount = currencyList.reduce((prev: any, cur: any) => {
            return prev.data.Balance > cur.data.Balance ? prev : cur;
        });
        const owner = maxAccount.data.AccountID;
        if (!owner) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        let currencyInfoList = await LocalRemote.SendQryCurrency(currency);
        let instrumentID = null;
        for (let currencyInfo of currencyInfoList) {
            if (currencyInfo.data.Currency == currency) {
                instrumentID = currencyInfo.data.InstrumentID;
                break;
            }
        }
        if (!instrumentID) {
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

        let ownerBalance = new Decimal(String(maxAccount.data.Balance));
        let currencyInfo = await LocalRemote.SendQryCurrency(msg.currency);
        let totalBalance = new Decimal(String(currencyInfo[0].data.Balance));
        let otherBalance = totalBalance.sub(ownerBalance);

        if (ownerBalance.eq(totalBalance)) {
            return this.fail(ErrorCode.enum.BUYBACK_ALREADY_APPLIED);
        }
        let avgPrice = await buybackService.getBuybackPrice(instrumentID);
        if (!avgPrice) {
            return this.fail(ErrorCode.enum.NO_PRICE);
        }
        let amount = avgPrice.mul(otherBalance);

        console.log(instrumentID, avgPrice.toFixed(), amount.toFixed(), otherBalance.toFixed());
        try {
            // 冻结资金
            // let orderID = "";
            // let orderFrozenMoneyRes = await LocalRemote.orderFrozenMoney(owner, amount.toFixed(), avgPrice.toFixed(), otherBalance.toFixed(), Currency.USDC);
            // for (let orderFrozenMoneyRe of orderFrozenMoneyRes) {
            //     if (orderFrozenMoneyRe.table == 'Account') {
            //
            //     } else if (orderFrozenMoneyRe.table == 'AccountAction') {
            //         let data = orderFrozenMoneyRe.data;
            //         orderID = data.OrderID;
            //         console.log(' OrderID:', data.OrderID, ' AccountID:', data.AccountID, ' Amount:', data.Amount);
            //     }
            // }
            // 停盘
            await LocalRemote.closeInstrument(instrumentID);
            // 冻结出金权限
            await LocalRemote.switchWithdraw(owner, false);
            // 结算手续费
            await LocalRemote.sendSettlement6();
            // 撤单
            // await LocalRemote.cancelAllOrdersByInstrumentID(owner, instrumentID);
            // 解冻资金
            // await LocalRemote.releaseFrozenMoneyByOrderID(owner, orderID);
            // 变更状态为已回购
            launchpadModel.Status = 3;
            await launchpadModel.save();
            // 生成回购表
            await BuyBackModel.create({
                LaunchpadID: launchpadModel.Id,
                Currency: currency,
                InstrumentID: instrumentID,
                AccountID: owner,
                Volume: new Decimal(otherBalance.toFixed()),
                Price: new Decimal(avgPrice.toFixed()),
                TotalPrice: new Decimal(amount.toFixed()),
                Status: 0,
            });

            // 发布卡片下线事件
            const eventData: CardOfflineEventData = {
                name: launchpadModel.Name,
                symbol: launchpadModel.Currency,
                closeTime: new Date(),
            };
            App.EventBus.publish(EventType.CARD_OFFLINE, eventData);

        } catch (e) {
            console.log("buyBack error:", e);
            return this.fail(ErrorCode.enum.BALANCE_NOT_ENOUGH);
        }
        return this.suc({amount: amount.toFixed()});
    }

}
