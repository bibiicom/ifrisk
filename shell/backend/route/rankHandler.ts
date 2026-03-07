import {BaseHandler} from "../base/core/BaseHandler";
import {LocalRemote} from "../remote/LocalRemote";
import Decimal from "decimal.js";

export default function () {
    return new rankHandler();
}
export var routeName: string = "backend/v1/rank";

export class rankHandler extends BaseHandler {
    async preHandle(msg: any, session: any): Promise<any> {
        return null;
    }

    /**
     * 持有排行榜
     */
    async holdRank(msg: any, session: any) {
        let currencies = await LocalRemote.SendQryCurrency();
        let rank: {
            currency: string,
            instrumentID: string,
            holdAmount: string,
            maxHoldAccountID: string,
            totalHoldAmount: string,
            maxHoldAmountPct: number,
        }[] = [];
        for (let currency of currencies) {
            let cur = currency.data.Currency;
            let instrumentID = currency.data.InstrumentID;
            if (!instrumentID || instrumentID.length == 0) {
                continue;
            }
            let accountCurrencyList = await LocalRemote.getAccountCurrencyList(cur);
            // 找到最大的 并且计算总量
            let maxHoldAmount = new Decimal("0");
            let maxHoldAccountID = "";
            let totalHoldAmount = new Decimal("0");
            for (let accountInfo of accountCurrencyList) {
                let accountID = accountInfo.data.AccountID;
                let holdAmount = new Decimal(accountInfo.data.Balance.toString());
                if (holdAmount.lte("0")) {
                    continue;
                }
                if (holdAmount.gt(maxHoldAmount)) {
                    maxHoldAmount = holdAmount;
                    maxHoldAccountID = accountID;
                }
                totalHoldAmount = totalHoldAmount.add(holdAmount);
            }
            // 排除 0 占比的
            if (totalHoldAmount.lte("0")) {
                continue;
            }
            // 计算最大占比
            let maxHoldAmountPct = maxHoldAmount.div(totalHoldAmount).times("100").toNumber();
            rank.push({
                currency: cur,
                instrumentID: instrumentID,
                holdAmount: maxHoldAmount.toFixed(),
                maxHoldAccountID: maxHoldAccountID,
                totalHoldAmount: totalHoldAmount.toFixed(),
                maxHoldAmountPct: maxHoldAmountPct,
            })
        }
        // 排序 最大占比 从大到小
        rank.sort((a, b) => {
            return b.maxHoldAmountPct - a.maxHoldAmountPct;
        })
        return this.suc({
            rank: rank
        })
    }

}