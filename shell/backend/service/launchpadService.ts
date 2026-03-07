import Decimal from "decimal.js";
import {sequelize} from '../config/database';
import {Currency} from '../const/Currency';
import CardModel from '../models/CardModel';
import LaunchpadModel from '../models/LaunchpadModel';
import SubscribeModel from '../models/SubscribeModel';
import {LarkRemote} from '../remote/LarkRemote';
import {LocalRemote} from '../remote/LocalRemote';
import {App} from "../base/App";
import {CardTransactedEventData} from "../base/event/EventData";
import {EventType} from "../const/EventTypes";

/**
 * Launchpad服务类，处理Launchpad相关的业务逻辑
 */
export class LaunchpadService {


    /**
     * 删除指定的空 Launchpad（由 Lark 自动任务创建）
     * 1. 查找 Summary="LarkAutoTask" 且 无申购记录 的 Launchpad
     * 2. 重置关联 Card 状态为 1
     * 3. 删除 Launchpad
     */
    async deleteEmptyLaunchpads() {
        console.log("[自动任务] 开始清理空 Launchpad");
        // 1. 查找符合条件的 Launchpad
        // Summary = 'LarkAutoTask'
        const launchpads = await LaunchpadModel.findAll({
            where: {
                Summary: 'LarkAutoTask'
            }
        });

        let deletedCount = 0;

        for (const lp of launchpads) {
            // Check subscription count
            const subCount = await SubscribeModel.count({
                where: {
                    LaunchpadID: lp.Id
                }
            });

            if (subCount === 0) {
                // Delete logic
                await sequelize.transaction(async (t) => {
                    // Reset Card Status to 1
                    if (lp.CardID) {
                        const card = await CardModel.findByPk(lp.CardID, {transaction: t});
                        if (card) {
                            card.Status = 1; // Reset to "Applying/Ready"
                            await card.save({transaction: t});
                        }
                    }
                    // Delete Launchpad
                    await lp.destroy({transaction: t, force: true});
                });
                console.log(`[自动任务] 已删除空 Launchpad: ID=${lp.Id}, Name=${lp.Name}`);
                deletedCount++;
            }
        }
        return {deletedCount};
    }

    /**
     * 同意申购并执行分配逻辑
     * @param launchpadId Launchpad ID
     * @param exchangeVolume 交易所购买量
     * @param exchangeTotalPrice 交易所购买总金额
     * @param cardholderVolume 持卡人购买量
     * @param tradeTime 交易时间
     */
    async agreeSubscribe(launchpadId: number, exchangeVolume: Decimal, exchangeTotalPrice: Decimal, cardholderVolume: Decimal, tradeTime: Date) {
        //  获取 Launchpad 信息
        const launchpad = await LaunchpadModel.findByPk(launchpadId);
        if (!launchpad) {
            throw new Error(`Launchpad not found: ${launchpadId}`);
        }
        const cardModel = await CardModel.findByPk(launchpad.CardID);
        if (!cardModel) {
            throw new Error(`cardModel 不存在`);
        }
        //  检查状态是否为 1 (申购中)
        if (launchpad.Status !== 1 || Date.now() <= launchpad.CloseTime.getTime()) {
            throw new Error(`Launchpad status error: ${launchpad.Status}`);
        }
        // 获取本轮可用总量 (RoundVolume)
        const totalAvailableDecimal = launchpad.RoundVolume;
        if (!totalAvailableDecimal || totalAvailableDecimal.lessThanOrEqualTo(0)) {
            throw new Error(`Invalid RoundVolume for Launchpad: ${launchpad.Id}`);
        }
        console.log(`[分配] 可用总量: ${totalAvailableDecimal.toFixed()}`);
        // 查询所有申购记录 (按创建时间升序排列)
        const subscriptions = await SubscribeModel.findAll({
            where: {LaunchpadID: launchpad.Id},
            order: [['createdAt', 'ASC']]
        });

        // 计算所有的申购量
        const totalSubscribeVolumeDecimal = subscriptions.reduce((acc, sub) => acc.add(sub.Volume), new Decimal(0));
        console.log(`[分配] 总申购量: ${totalSubscribeVolumeDecimal.toFixed()}`);
        if (totalSubscribeVolumeDecimal.gte(totalAvailableDecimal)) {
            // 如果总申购量大于等于可用总量
            //  分配申购记录到不同的账户
            await this.allocateByLaunchpad(launchpad, subscriptions, totalAvailableDecimal);
        } else {
            // 如果总申购量小于可用总量
            // 交易所和持卡人购买余下的
            await this.allocateByLaunchpadComplete(launchpad, cardModel, subscriptions, totalSubscribeVolumeDecimal, totalAvailableDecimal, exchangeVolume, exchangeTotalPrice, cardholderVolume);
        }

        //  注册合约
        try {
            await LocalRemote.addInstrument(launchpad.Currency, cardModel.Name);
        } catch (e) {
            console.error("addInstrument error", launchpad.Currency);
            console.error(e.message, e);
        }
        //  更新币种 单账户持有量限制
        try {
            // const tradeStandardPct = launchpad.TradeStandardPct.div(100).toNumber();
            const haltStandardPct = launchpad.HaltStandardPct.div(100).toNumber();
            await LocalRemote.updateCurrency(launchpad.Currency, haltStandardPct);
        } catch (e) {
            console.error("updateCurrency error", launchpad.Currency);
            console.error(e.message, e);
        }
        //  更新状态为 2 (申购结束)
        launchpad.Status = 2;
        launchpad.TradeTime = tradeTime;
        await launchpad.save();
        cardModel.Status = 5;
        await cardModel.save();

        //  发放申购记录
        await this.distributeSubscriptions(launchpad);
        //  发布卡片交易事件
        const eventData: CardTransactedEventData = {
            name: launchpad.Name,
            symbol: launchpad.Currency,
            tradeTime: tradeTime,
            summary: launchpad.Summary,
        };
        App.EventBus.publish(EventType.CARD_TRANSACTED, eventData);
    }

    /**
     * 拒绝申购并退款
     * @param launchpadId Launchpad ID
     */
    async rejectAndRefundSubscription(launchpadId: number) {
        console.log(`[拒绝&退款] 开始处理 LaunchpadID: ${launchpadId}`);

        // 1. 查询 Launchpad
        const launchpad = await LaunchpadModel.findByPk(launchpadId);
        if (!launchpad) {
            throw new Error(`Launchpad not found: ${launchpadId}`);
        }

        // 验证状态: 必须是申购中(1)
        if (launchpad.Status !== 1) {
            throw new Error(`Launchpad status error: ${launchpad.Status}. Only status 1 can be rejected.`);
        }

        // 2. 获取 Card 信息
        const card = await CardModel.findByPk(launchpad.CardID);
        if (!card) {
            throw new Error(`Card not found: ${launchpad.CardID}`);
        }

        // 3. 查询所有申购记录 (Status = 0: 待处理 / 申购中)
        const subscribeList = await SubscribeModel.findAll({
            where: {
                LaunchpadID: launchpad.Id,
                Status: 0
            }
        });

        console.log(`[拒绝&退款] 待处理申购记录数: ${subscribeList.length}`);

        const addMoneyFailList: Array<{
            AccountID: string;
            Volume: string;
            OrderID: string;
            error: string;
        }> = [];

        // 4. 执行退款 (释放冻结资金)
        for (const subscribe of subscribeList) {
            try {
                // 释放冻结资金并出金
                await LocalRemote.releaseFrozenMoneyByOrderID(subscribe.AccountID, subscribe.OrderID);

                // 更新订单状态为 2 (已退款/失败)
                subscribe.Status = 2;
                await subscribe.save();
                console.log(`[拒绝&退款] 释放资金成功: User=${subscribe.AccountID}, Order=${subscribe.OrderID}`);

            } catch (e: any) {
                console.error(`[拒绝&退款] 释放资金失败: User=${subscribe.AccountID}, Order=${subscribe.OrderID}`, e);
                addMoneyFailList.push({
                    AccountID: subscribe.AccountID,
                    Volume: subscribe.Volume instanceof Decimal ? subscribe.Volume.toFixed() : String(subscribe.Volume),
                    OrderID: subscribe.OrderID,
                    error: e.message || 'Unknown error',
                });
            }
        }

        // 5. 更新 Launchpad 和 Card 状态
        try {
            await sequelize.transaction(async (t) => {
                launchpad.Status = 4; // 4: 失败/关闭
                await launchpad.save({transaction: t});

                card.Status = 1; // 0: 初始/可用 (卡片退回)
                await card.save({transaction: t});
            });
            console.log(`[拒绝&退款] 项目及卡片状态更新完成: Launchpad=${launchpad.Id}, Card=${card.Id}`);

        } catch (e: any) {
            console.error(`[拒绝&退款] 更新项目/卡片状态失败:`, e);
            throw e;
        }

        // 6. 通知 Lark
        let failContent = "";
        if (addMoneyFailList.length > 0) {
            failContent = " --- \n**失败列表：** \n";
            failContent += addMoneyFailList.map(item => `- AccountID:${item.AccountID}\nVolume:${item.Volume}\nOrderID:${item.OrderID}\nError:${item.error}`).join("\n");
        }

        LarkRemote.notifyLark(`
**申购拒绝/退款执行完成：**
**LaunchpadID:** ${launchpad.Id}
**Currency:** ${launchpad.Currency}
**Total Orders:** ${subscribeList.length}
**Failures:** ${addMoneyFailList.length}
${failContent}
`.trim());

        return {
            success: true,
            totalProcessed: subscribeList.length,
            failCount: addMoneyFailList.length,
            failures: addMoneyFailList
        };
    }

    /**
     * 发放申购
     * 根据 launchpadId 查询状态为 3 的申购记录进行发放
     */
    private async distributeSubscriptions(launchpad: LaunchpadModel) {
        console.log(`[发放] 开始处理 LaunchpadID: ${launchpad.Id}`);
        if (!launchpad) {
            throw new Error(`Launchpad not found: ${launchpad.Id}`);
        }

        // 2. 获取 Card 信息
        const card = await CardModel.findByPk(launchpad.CardID);
        if (!card) {
            throw new Error(`Card not found: ${launchpad.CardID}`);
        }

        // 3. 查询所有待发放的申购记录 (Status = 3)
        const subscribeList = await SubscribeModel.findAll({
            where: {
                LaunchpadID: launchpad.Id,
                Status: 3
            }
        });

        if (subscribeList.length === 0) {
            console.log(`[发放] 无待发放申购记录 (Status=3)`);
            return;
        }

        console.log(`[发放] 待处理记录数: ${subscribeList.length}`);
        const addMoneyFailList = await this.doAddCurrency(subscribeList, card);

        // 5. 手续费处理
        // 发送手续费到交易所账户
        let walletAccount = App.configManager.getWallet().account;
        let walletFee = launchpad.AllVolume.sub(launchpad.RoundVolume).toFixed();
        try {
            await LocalRemote.addMoney(walletAccount, walletFee, launchpad.Currency);
        } catch (e) {
            console.error("addMoney error", walletAccount, walletFee, launchpad.Currency);
            console.error(e.message, e);
            addMoneyFailList.push({
                AccountID: walletAccount,
                OrderID: "手续费",
                Volume: walletFee,
                error: e.message,
            });
        }

        // 6. 错误通知
        // 失败列表通知到lark
        let failContent = "";
        if (addMoneyFailList.length > 0) {
            failContent = " --- \n**失败列表：** \n";
            failContent += addMoneyFailList.map(item => `- AccountID:${item.AccountID}\nVolume:${item.Volume}\nOrderID:${item.OrderID}\nError:${item.error}`).join("\n");
        }
        LarkRemote.notifyLark(`
**申购成功：**
**LaunchpadID:** ${launchpad.Id}
**Currency:** ${launchpad.Currency}
${failContent}
`.trim());

    }

    private async doAddCurrency(subscribeList: SubscribeModel[], card: CardModel) {
        const addMoneyFailList: Array<{
            AccountID: string;
            Volume: string;
            OrderID: string;
            error: string;
        }> = [];

        // 4. 遍历处理每条记录
        for (const subscribe of subscribeList) {
            try {
                if (subscribe.Status !== 3) {
                    console.log(`[发放] 订单 ${subscribe.OrderID} 状态不是 3，跳过`);
                    continue;
                }
                console.log(`[发放] 处理订单 ${subscribe.OrderID}, 用户 ${subscribe.AccountID}`);

                // 释放冻结资金并出金
                if (subscribe.OrderID) {
                    await LocalRemote.useFrozenMoneyByOrderID(subscribe.AccountID, subscribe.OrderID, subscribe.AllocatedTotalPrice.toFixed());
                }

                // 入金 (返还多余资金? 或者发放代币? 参照原逻辑是 addMoney Volume)
                await LocalRemote.addMoney(subscribe.AccountID, subscribe.AllocatedVolume.toFixed(), subscribe.Currency);
                console.log(`[发放] 用户入金成功: ${subscribe.AccountID}, ${subscribe.AllocatedVolume.toFixed()} ${subscribe.Currency}`);

                // 持卡人入金 (USDC)
                if (subscribe.AllocatedTotalPrice.gt(0)) {
                    await LocalRemote.addMoney(card.Owner, subscribe.AllocatedTotalPrice.toFixed(), Currency.USDC);
                    console.log(`[发放] 持卡人入金成功: ${card.Owner}, ${subscribe.AllocatedTotalPrice.toFixed()} ${Currency.USDC}`);
                }

                // 更新状态为 1 (已发放)
                subscribe.Status = 1;
                await subscribe.save();

            } catch (e: any) {
                console.error(`[发放] 处理订单 ${subscribe.OrderID} 失败:`, e);
                addMoneyFailList.push({
                    AccountID: subscribe.AccountID,
                    Volume: subscribe.AllocatedVolume.toFixed(),
                    OrderID: subscribe.OrderID,
                    error: e.message || 'Unknown error',
                });
            }
        }
        return addMoneyFailList;
    }

    /**
     * 根据Launchpad执行分配逻辑
     * @param launchpad Launchpad Model
     * @param subscriptions 申购列表
     * @param totalAvailableDecimal 总可用量，Decimal类型
     */
    private async allocateByLaunchpad(launchpad: LaunchpadModel, subscriptions: SubscribeModel[], totalAvailableDecimal: Decimal) {
        console.log(`[分配] 开始处理 LaunchpadID: ${launchpad.Id}`);
        // 3. 聚合用户申购数据
        // Map: AccountID -> { totalVolume, firstTime, orders[] }
        const accountMap = new Map<string, {
            accountID: string;
            totalVolume: Decimal;
            firstTime: number;
            orders: SubscribeModel[]
        }>();

        for (const sub of subscriptions) {
            const accId = sub.AccountID;
            if (!accountMap.has(accId)) {
                accountMap.set(accId, {
                    accountID: accId,
                    totalVolume: new Decimal(0),
                    firstTime: sub.createdAt.getTime(), // 取第一笔订单时间
                    orders: []
                });
            }
            const entry = accountMap.get(accId)!;
            entry.totalVolume = entry.totalVolume.add(sub.Volume);
            entry.orders.push(sub);
        }

        // 准备调用分配算法的数据
        const allocationInput = Array.from(accountMap.values()).map(entry => ({
            accountID: entry.accountID,
            applyVolume: entry.totalVolume, // 直接传递 Decimal
            time: entry.firstTime
        }));

        // 4. 执行分配算法
        const allocationResult = this.allocateSubscription(totalAvailableDecimal, allocationInput);

        // 5. 开启事务，回填分配结果
        await sequelize.transaction(async (t) => {
            // const updatedOrders: SubscribeModel[] = [];

            for (const res of allocationResult) {
                const entry = accountMap.get(res.accountID)!;
                let remainingAlloc = res.allocatedVolume; // 已经是 Decimal

                console.log(`[分配] 用户 ${res.accountID} 总分配: ${remainingAlloc.toString()}`);

                // 遍历该用户的订单（已按时间排序），优先填满早的订单
                for (const order of entry.orders) {
                    const orderVol = order.Volume;
                    let allocForOrder = new Decimal(0);

                    if (remainingAlloc.greaterThan(0)) {
                        if (remainingAlloc.greaterThanOrEqualTo(orderVol)) {
                            // 剩余足够填满此单
                            allocForOrder = orderVol;
                            remainingAlloc = remainingAlloc.minus(orderVol);
                        } else {
                            // 剩余不够，全部分给此单
                            allocForOrder = remainingAlloc;
                            remainingAlloc = new Decimal(0);
                        }
                    }

                    // 更新订单数据
                    order.AllocatedVolume = allocForOrder;
                    order.AllocatedTotalPrice = allocForOrder.mul(order.Price);
                    // 状态更新为待发放
                    order.Status = 3;

                    await order.save({transaction: t});
                    // updatedOrders.push(order);

                    console.log(`  -> 订单 ${order.OrderID}: 申购 ${orderVol}, 分配 ${allocForOrder}`);
                }
            }
            // return updatedOrders;
        });

        console.log(`[分配] 处理完成`);
        // return resultDetails;
    }

    /**
     * 分配申购数量
     * @param totalAvailable 总可分配数量
     * @param subscriptions 申购列表，每个元素包含accountID、applyVolume和time字段
     * @returns 分配结果列表
     */
    private allocateSubscription(totalAvailable: Decimal, subscriptions: Array<{
        accountID: string;
        applyVolume: Decimal;
        time: number
    }>) {
        console.log(`[分配] 开始。总量: ${totalAvailable.toFixed()}, 用户数: ${subscriptions.length}`);

        // 1. 初始化结果结构
        // 复制以避免修改原数组，并添加 'allocated' 字段
        // 按时间（升序）排序，然后是原始索引
        let users = subscriptions.map((s, index) => ({
            ...s,
            allocated: new Decimal(0),
            originalIndex: index
        }));

        // 排序：时间早的优先。使用 originalIndex 进行稳定排序。
        users.sort((a, b) => {
            if (a.time !== b.time) {
                return a.time - b.time;
            }
            return a.originalIndex - b.originalIndex;
        });

        let remainingAvailable = totalAvailable.floor(); // 确保向下取整

        while (remainingAvailable.greaterThan(0)) {
            // 筛选活跃用户（仍需分配额度的用户）
            const activeUsers = users.filter(u => u.allocated.lessThan(u.applyVolume));

            if (activeUsers.length === 0) {
                console.log(`[分配] 无活跃用户。`);
                break;
            }

            const count = new Decimal(activeUsers.length);
            // 公式：可申购量 / 人数（向下取整）
            const ratio = remainingAvailable.div(count).floor();

            console.log(`[分配] 本轮信息: 剩余总量=${remainingAvailable.toFixed()}, 活跃用户数=${count.toFixed()}, 计算值(ratio)=${ratio.toFixed()}`);

            // 情况 1: ratio < 1 (剩余总量 < 人数)
            // "当计算结果小于1时，采用先到先得原则，每人分配1份"
            if (ratio.lessThan(1)) {
                console.log(`[分配] ratio < 1。进入先到先得模式。`);
                // 按顺序给前 N 个用户分配 1 份
                for (let i = 0; i < activeUsers.length && remainingAvailable.greaterThan(0); i++) {
                    const user = activeUsers[i];
                    user.allocated = user.allocated.add(1);
                    remainingAvailable = remainingAvailable.minus(1);
                }
                // 资源已耗尽，结束循环
                break;
            }

            // 情况 2: ratio >= 1
            // 找到活跃用户中最小的剩余申购量
            let minUnfilled = new Decimal(Number.MAX_SAFE_INTEGER); // Initial max
            let hasMaxSafe = true;

            for (const u of activeUsers) {
                const need = u.applyVolume.minus(u.allocated);
                if (hasMaxSafe || need.lessThan(minUnfilled)) {
                    minUnfilled = need;
                    hasMaxSafe = false;
                }
            }

            console.log(`[分配] 最小剩余申购量(minUnfilled)=${minUnfilled.toFixed()}`);

            // 逻辑判断
            if (ratio.lessThanOrEqualTo(minUnfilled)) {
                // "如果计算值 <= 所有用户中最小的申购量..."
                console.log(`[分配] 计算值 (${ratio.toFixed()}) <= 最小申购量。按计算值分配给每个用户。`);

                // 给每个人分配 ratio
                for (const u of activeUsers) {
                    u.allocated = u.allocated.add(ratio);
                    remainingAvailable = remainingAvailable.minus(ratio);
                }

                // "剩余可申购量按时间顺序分配，每人1份"
                if (remainingAvailable.greaterThan(0)) {
                    console.log(`[分配] 分配余量: ${remainingAvailable.toFixed()}`);
                    for (let i = 0; i < activeUsers.length && remainingAvailable.greaterThan(0); i++) {
                        const u = activeUsers[i];
                        // 确保不超过申购量
                        if (u.allocated.lessThan(u.applyVolume)) {
                            u.allocated = u.allocated.add(1);
                            remainingAvailable = remainingAvailable.minus(1);
                        }
                    }
                }
                // 通常在此处结束
                break;
            } else {
                // ratio > minUnfilled
                // "如果计算值 > 所有用户中最小的申购量..."
                console.log(`[分配] 计算值 (${ratio.toFixed()}) > 最小申购量 (${minUnfilled.toFixed()})。按最小申购量平均分配。`);

                // 给每个人分配 minUnfilled
                for (const u of activeUsers) {
                    u.allocated = u.allocated.add(minUnfilled);
                    remainingAvailable = remainingAvailable.minus(minUnfilled);
                }

                // "剩余可申购量重复上述分配逻辑" -> 继续循环
            }
        }

        console.log(`[分配] 完成。最终剩余: ${remainingAvailable.toFixed()}`);

        return users.map(u => ({
            accountID: u.accountID,
            applyVolume: u.applyVolume,
            allocatedVolume: u.allocated,
            time: u.time
        }));
    }


    private async allocateByLaunchpadComplete(launchpad: LaunchpadModel, cardModel: CardModel, subscriptions: SubscribeModel[], totalSubscribeVolumeDecimal: Decimal, totalAvailableDecimal: Decimal, exchangeVolume: Decimal, exchangeTotalPrice: Decimal, cardholderVolume: Decimal) {
        // 交易所购买量与持卡人购买量加上总申购量等于总可用量
        if (!exchangeVolume.plus(cardholderVolume).plus(totalSubscribeVolumeDecimal).equals(totalAvailableDecimal)) {
            throw new Error(`[分配] 交易所购买量(${exchangeVolume.toFixed()}) + 持卡人购买量(${cardholderVolume.toFixed()}) + 总申购量(${totalSubscribeVolumeDecimal.toFixed()}) 不等于 总可用量(${totalAvailableDecimal.toFixed()})`);
        }
        await sequelize.transaction(async t => {
            // 添加交易所申购订单
            // 交易所购买量大于0时，添加交易所申购订单
            if (exchangeVolume.gt(0)) {
                await SubscribeModel.create({
                    LaunchpadID: launchpad.Id,
                    Currency: launchpad.Currency,
                    AccountID: App.configManager.getWallet().account,
                    OrderID: "",
                    Price: exchangeTotalPrice.div(exchangeVolume),
                    Volume: exchangeVolume,
                    TotalPrice: exchangeTotalPrice,
                    AllocatedVolume: exchangeVolume,
                    AllocatedTotalPrice: exchangeTotalPrice,
                    Status: 3,
                }, {transaction: t});
            }
            // 添加持卡人申购订单
            if (cardholderVolume.gt(0)) {
                const cardholderTotalPrice = launchpad.Price.mul(cardholderVolume);
                await SubscribeModel.create({
                    LaunchpadID: launchpad.Id,
                    Currency: launchpad.Currency,
                    AccountID: cardModel.Owner,
                    OrderID: "",
                    Price: launchpad.Price,
                    Volume: cardholderVolume,
                    TotalPrice: cardholderTotalPrice,
                    AllocatedVolume: cardholderVolume,
                    AllocatedTotalPrice: 0,
                    Status: 3,
                }, {transaction: t});
            }
            // 遍历所有订单，更新状态为待发放
            for (const order of subscriptions) {
                // 更新订单数据
                order.AllocatedVolume = order.Volume;
                order.AllocatedTotalPrice = order.TotalPrice;
                // 状态更新为待发放
                order.Status = 3;
                await order.save({transaction: t});
                console.log(`  -> 订单 ${order.OrderID}: 申购 ${order.Volume.toFixed()}, 分配 ${order.AllocatedVolume.toFixed()}`);
            }
        });

    }
}

export default new LaunchpadService();
