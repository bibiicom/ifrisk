import {BaseLifecycle} from "../../base/core/BaseLifecycle";
import {ILifeCycle} from "../../base/core/ILifeCycle";
import {App} from "../../base/App";
import {LocalRemote} from "../../remote/LocalRemote";
import {EnvStr} from "../../const/EvnStr";
import BuyBackModel from "../../models/BuyBackModel";
import {LarkRemote} from "../../remote/LarkRemote";
import Decimal from "decimal.js";


export default function () {
    return new Lifecycle();
}


class Lifecycle extends BaseLifecycle implements ILifeCycle {
    async beforeStartup() {
        const master = App.configManager.getMaster();
        App.startParam = Object.assign(App.startParam, master)
        App.startParam.isStatic = false
        App.startParam.client_ip = master.client_ip
        App.startParam.client_port = master.client_port
        App.startParam.port = master.port
        App.startParam.id = master.id

        Decimal.set({rounding: Decimal.ROUND_DOWN});
        // 注册监听事件
        this.registerListener();
        await this.initMsql();
    }

    async afterStartup() {
        console.log(App.startParam.id, '启动完成');

        if (App.configManager.getEnv() != EnvStr.Local) {
            // this.doLaunchpadWork();
            this.doBuyBackWork();
        }
    }

//     async doLaunchpadWork() {
//         setTimeout(async () => {
//             console.log("start doLaunchpadWork");
//             await App.SystemUtil.safeRunFuncAsync(async () => {
//                 let launchpadList = await LaunchpadModel.findAll({
//                     where: {
//                         Status: 1
//                     },
//                     order: [
//                         [sequelize.literal('`CloseTime` IS NULL'), 'ASC'],
//                         ['CloseTime', 'ASC']
//                     ]
//                 });
//                 if (launchpadList.length <= 0) {
//                     console.log("no launchpad");
//                     return;
//                 }
//                 let launchpad = launchpadList[0];
//                 if (!launchpad.CloseTime || launchpad.CloseTime > new Date()) {
//                     console.log("launchpad not close", launchpad.Currency);
//                     return;
//                 }
//                 let card = await CardModel.findByPk(launchpad.CardID);
//                 if (!card) {
//                     console.error("card not found", launchpad.CardID);
//                     // 通知lark 失败列表
//                     LarkRemote.notifyLark(`${launchpad.Id}(${launchpad.Currency}) 申购结算,卡片 ${launchpad.CardID} 不存在`);
//                     return;
//                 }
//
//                 let subscribeList = await SubscribeModel.findAll({
//                     where: {
//                         LaunchpadID: launchpad.Id,
//                         Status: 0
//                     },
//                 });
//
//                 // 处理订单
//                 // 计算最低申购量
//                 let minStockVolume = launchpad.AllVolume.mul(launchpad.MinAmountPct).div("100");
//                 let subVolume = launchpad.AllVolume.sub(minStockVolume);
//                 if (launchpad.StockVolume.gt(subVolume)) {
//                     // 没有达到最低申购量 全部退款
//                     let addMoneyFailList = [];
//                     for (let subscribe of subscribeList) {
//                         try {
//                             // 释放冻结资金并出金
//                             await LocalRemote.releaseFrozenMoneyByOrderID(subscribe.AccountID, subscribe.OrderID);
//                             subscribe.Status = 2;
//                             await subscribe.save();
//                             console.log("releaseFrozenMoneyByOrderID success", subscribe.AccountID, subscribe.OrderID, launchpad.Currency);
//                         } catch (e) {
//                             console.error("releaseFrozenMoneyByOrderID error", subscribe.AccountID, subscribe.OrderID);
//                             console.error(e.message, e);
//                             addMoneyFailList.push({
//                                 AccountID: subscribe.AccountID,
//                                 Volume: subscribe.Volume,
//                                 OrderID: subscribe.OrderID,
//                                 error: e.message,
//                             });
//                         }
//                     }
//                     launchpad.Status = 4;
//                     await launchpad.save();
//                     // 申购失败 卡片退回
//                     card.Status = 0;
//                     await card.save();
//                     console.log("launchpad close 3", launchpad.Id, launchpad.Currency);
//                     // 通知lark 失败列表
//                     let failContent = "";
//                     if (addMoneyFailList.length > 0) {
//                         failContent = " --- \n**失败列表：** \n";
//                         failContent += addMoneyFailList.map(item => `- AccountID:${item.AccountID}\nVolume:${item.Volume}\nOrderID:${item.OrderID}\nError:${item.error}`).join("\n");
//                     }
//                     LarkRemote.notifyLark(`
// **申购失败：**
// **LaunchpadID:** ${launchpad.Id}
// **Currency:** ${launchpad.Currency}
// **MinPct:** ${launchpad.MinAmountPct.toFixed()}%
// **CurPct:** ${new Decimal(1).sub(launchpad.StockVolume.div(launchpad.AllVolume)).mul("100").toFixed(5)}%
// ${failContent}
// `.trim());
//                     return;
//                 }
//
//                 try {
//                     await LocalRemote.addInstrument(launchpad.Currency);
//                 } catch (e) {
//                     console.error("addInstrument error", launchpad.Currency);
//                     console.error(e.message, e);
//                 }
//
//                 launchpad.Status = 2;
//                 await launchpad.save();
//                 console.log("launchpad close 2", launchpad.Id, launchpad.Currency);
//
//                 console.log("subscribe count", subscribeList.length);
//                 if (subscribeList.length < 0) {
//                     return;
//                 }
//                 let addMoneyFailList = [];
//                 for (let subscribe of subscribeList) {
//                     try {
//                         // 释放冻结资金并出金
//                         await LocalRemote.useFrozenMoneyByOrderID(subscribe.AccountID, subscribe.OrderID);
//                         // 入金
//                         await LocalRemote.addMoney(subscribe.AccountID, subscribe.Volume.toFixed(), launchpad.Currency);
//                         console.log("addMoney success", subscribe.AccountID, subscribe.Volume.toFixed(), launchpad.Currency);
//                         // 持卡人入金
//                         await LocalRemote.addMoney(card.Owner, subscribe.TotalPrice.toFixed(), Currency.USDC);
//                         console.log("addMoney success", card.Owner, subscribe.TotalPrice.toFixed(), Currency.USDC);
//                         subscribe.Status = 1;
//                         await subscribe.save();
//                     } catch (e) {
//                         console.error("useFrozenMoneyByOrderID error", subscribe.AccountID, subscribe.OrderID);
//                         console.error(e.message, e);
//                         addMoneyFailList.push({
//                             AccountID: subscribe.AccountID,
//                             Volume: subscribe.Volume,
//                             OrderID: subscribe.OrderID,
//                             error: e.message,
//                         });
//                     }
//                 }
//                 // 发送手续费到交易所账户
//                 let walletAccount = App.configManager.getWallet().account;
//                 let walletFee = launchpad.AllVolume.sub(launchpad.RoundVolume).toFixed();
//                 try {
//                     await LocalRemote.addMoney(walletAccount, walletFee, launchpad.Currency);
//                 } catch (e) {
//                     console.error("addMoney error", walletAccount, walletFee, launchpad.Currency);
//                     console.error(e.message, e);
//                     addMoneyFailList.push({
//                         AccountID: walletAccount,
//                         Volume: walletFee,
//                         error: e.message,
//                     });
//                 }
//                 // 剩余的发放给上架卡片的人
//                 try {
//                     await LocalRemote.addMoney(card.Owner, launchpad.StockVolume.toFixed(), launchpad.Currency);
//                 } catch (e) {
//                     console.error("addMoney error", card.Owner, launchpad.StockVolume.toFixed(), launchpad.Currency);
//                     console.error(e.message, e);
//                     addMoneyFailList.push({
//                         AccountID: card.Owner,
//                         Volume: launchpad.StockVolume.toFixed(),
//                         error: e.message,
//                     });
//                 }
//
//                 // 失败列表通知到lark
//                 let failContent = "";
//                 if (addMoneyFailList.length > 0) {
//                     failContent = " --- \n**失败列表：** \n";
//                     failContent += addMoneyFailList.map(item => `- AccountID:${item.AccountID}\nVolume:${item.Volume}\nOrderID:${item.OrderID}\nError:${item.error}`).join("\n");
//                 }
//                 LarkRemote.notifyLark(`
// **申购成功：**
// **LaunchpadID:** ${launchpad.Id}
// **Currency:** ${launchpad.Currency}
// ${failContent}
// `.trim());
//             }, this);
//             console.log("done doLaunchpadWork");
//             this.doLaunchpadWork();
//         }, 60 * 1000);
//     }

    // 回购处理
    async doBuyBackWork() {
        setTimeout(async () => {
            console.log("start doBuyBackWork");
            await App.SystemUtil.safeRunFuncAsync(async () => {
                let buyBackList = await BuyBackModel.findAll({
                    where: {
                        Status: 0
                    }
                });
                if (buyBackList.length <= 0) {
                    console.log("no buyBack");
                    return;
                }
                for (let buyBack of buyBackList) {
                    let failList = [];
                    try {
                        // 处理回购
                        let currencyList = await LocalRemote.getAccountCurrencyList(buyBack.Currency);
                        for (let accCurrency of currencyList) {
                            if (accCurrency.data.AccountID === buyBack.AccountID) {
                                continue;
                            }
                            if (accCurrency.data.Balance > 0) {
                                try {
                                    await LocalRemote.SendOTCTradeInsert(buyBack.InstrumentID, buyBack.Price.toFixed(), accCurrency.data.Balance, buyBack.AccountID, accCurrency.data.AccountID);
                                    console.log("SendOTCTradeInsert success", buyBack.InstrumentID, buyBack.Price.toFixed(), accCurrency.data.Balance, buyBack.AccountID, accCurrency.data.AccountID);
                                } catch (e) {
                                    console.error("SendOTCTradeInsert error", buyBack.toJSON(), accCurrency);
                                    console.error(e.message, e);
                                    failList.push({
                                        AccountID: accCurrency.data.AccountID,
                                        Volume: accCurrency.data.Balance,
                                        Error: e.message,
                                    });
                                }
                            } else if (accCurrency.data.Balance < 0) {
                                // 处理负数余额
                                try {
                                    await LocalRemote.SendOTCTradeInsert(buyBack.InstrumentID, buyBack.Price.toFixed(), -1 * accCurrency.data.Balance, accCurrency.data.AccountID, buyBack.AccountID);
                                    console.log("- SendOTCTradeInsert success", buyBack.InstrumentID, buyBack.Price.toFixed(), accCurrency.data.Balance, buyBack.AccountID, accCurrency.data.AccountID);
                                } catch (e) {
                                    console.error("- SendOTCTradeInsert error", buyBack.toJSON(), accCurrency);
                                    console.error(e.message, e);
                                    failList.push({
                                        AccountID: accCurrency.data.AccountID,
                                        Volume: accCurrency.data.Balance,
                                        Error: e.message,
                                    });
                                }
                            }

                        }
                        buyBack.Status = 1;
                        await buyBack.save();
                        // 开启出金
                        await LocalRemote.switchWithdraw(buyBack.AccountID, true);

                    } catch (e) {
                        console.error("doBuyBackWork error", buyBack.toJSON());
                        console.error(e.message, e);
                    }
                    // 失败列表通知到lark
                    let failContent = "";
                    if (failList.length > 0) {
                        failContent = ` --- \n**失败列表：** \n`;
                        failContent += failList.map(item => `- AccountID:${item.AccountID}\nVolume:${item.Volume}\nError:${item.Error}`).join("\n");
                    }
                    LarkRemote.notifyLark(`
**回购结束：**
**Currency:** ${buyBack.Currency}
**InstrumentID:** ${buyBack.InstrumentID}
**Price:** ${buyBack.Price.toFixed()}
**AccountID:** ${buyBack.AccountID}
${failContent}
`.trim());
                }
            }, this);
            console.log("done doBuyBackWork");
            this.doBuyBackWork();
        }, 30 * 1000);
    }


}


