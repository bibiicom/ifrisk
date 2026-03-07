import {App} from "../base/App";
import {HttpRequest} from "../tool/HttpRequest";
import {BusinessError} from "../const/BusinessError";
import {Currency} from "../const/Currency";

export class LocalRemote {

    static getUrl(path: string) {
        return App.configManager.getServer().localHost + path;
    }

    static async sendCheckToken(token: string) {
        let url = this.getUrl("/action/v1.0/SendCheckToken");
        const payload = {
            Token: token,
        };

        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    static async getOrder(accountID: string = null, localID: string = null, orderID: string = null) {
        let url = this.getUrl("/action/v1.0/SendQryOrder");
        const payload = {};
        if (accountID) {
            payload['MemberID'] = accountID;
            payload['AccountID'] = accountID;
        }
        if (localID) {
            payload['LocalID'] = localID;
        }
        if (orderID) {
            payload['OrderID'] = orderID;
        }
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    static async getOrdersByInstrumentID(instrumentID: string) {
        let url = this.getUrl("/action/v1.0/SendQryOrder");
        const payload = {
            InstrumentID: instrumentID,
        };

        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    // a:使用订单冻结资金
    static async orderFrozenMoney(accountID: string, amount: string, price: string, volume: string, currency: string, instrumentID: string = null, localID: string = null, remark: string = null): Promise<any[]> {
        let url = this.getUrl("/action/v1.0/SendAccountAction");
        // ActionType 1:入金;2:出金;3:冻结资金;4:释放冻结资金;5:释放冻结资金,同时出金;6:内部转账;7:设置保留资金限制;8:切换保留资金类型;9:保留资金激活失效;a:使用订单冻结资金;
        const payload = {
            ExchangeID: "SCARD",
            MemberID: accountID,
            AccountID: accountID,
            SettlementGroup: "SG1",
            Price: price,
            Amount: amount,
            Volume: volume,
            Currency: currency,
            ActionType: "a",
            Remark: remark,
        };
        if (localID) {
            payload['LocalID'] = localID;
        }
        if (instrumentID) {
            payload['InstrumentID'] = instrumentID;
        }
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    // 5:使用订单释放冻结资金,同时出金
    static async useFrozenMoneyByOrderID(accountID: string, orderID: string, amount: string = null) {
        let url = this.getUrl("/action/v1.0/SendAccountAction");
        // ActionType 1:入金;2:出金;3:冻结资金;4:释放冻结资金;5:释放冻结资金,同时出金;6:内部转账;7:设置保留资金限制;8:切换保留资金类型;9:保留资金激活失效;a:使用订单冻结资金;
        const payload = {
            MemberID: accountID,
            AccountID: accountID,
            OrderID: orderID,
            Amount: amount,
            ActionType: "5",
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    // 4:使用订单释放冻结资金
    static async releaseFrozenMoneyByOrderID(accountID: string, orderID: string) {
        let url = this.getUrl("/action/v1.0/SendAccountAction");
        // ActionType 1:入金;2:出金;3:冻结资金;4:释放冻结资金;5:释放冻结资金,同时出金;6:内部转账;7:设置保留资金限制;8:切换保留资金类型;9:保留资金激活失效;a:使用订单冻结资金;
        const payload = {
            MemberID: accountID,
            AccountID: accountID,
            OrderID: orderID,
            ActionType: "4",
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    // 2:出金
    static async useMoneyByCurrency(accountID: string, amount: string, currency: string) {
        let url = this.getUrl("/action/v1.0/SendAccountAction");
        // ActionType 1:入金;2:出金;3:冻结资金;4:释放冻结资金;5:释放冻结资金,同时出金;6:内部转账;7:设置保留资金限制;8:切换保留资金类型;9:保留资金激活失效;a:使用订单冻结资金;
        const payload = {
            ExchangeID: "SCARD",
            SettlementGroup: "SG1",
            MemberID: accountID,
            AccountID: accountID,
            Amount: amount,
            Currency: currency,
            ActionType: "2",
            Remark: `card_${currency}`,
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    // 查询账户信息
    static async getAccountInfo(accountID: string, currency: string) {
        let url = this.getUrl("/action/v1.0/SendQryAccount");
        const payload = {
            MemberID: accountID,
            AccountID: accountID,
            Currency: currency,
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result[0]
    }

    // 查询账户信息
    static async getAccountInfoList(accountID: string): Promise<any[]> {
        let url = this.getUrl("/private/v1.0/SendQryAccount");
        const payload = {
            MemberID: accountID,
            AccountID: accountID,
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    // 1:入金;
    static async addMoney(accountID: string, amount: string, currency: string, localID: string = null, remark: string = null): Promise<any[]> {
        let url = this.getUrl("/action/v1.0/SendAccountAction");
        // ActionType 1:入金;2:出金;3:冻结资金;4:释放冻结资金;5:释放冻结资金,同时出金;6:内部转账;7:设置保留资金限制;8:切换保留资金类型;9:保留资金激活失效;a:使用订单冻结资金;
        const payload = {
            MemberID: accountID,
            AccountID: accountID,
            SettlementGroup: "SG1",
            Amount: amount,
            Currency: currency,
            ActionType: "1",
            Remark: remark,
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    // b:切换是否可以出金
    static async switchWithdraw(accountID: string, isWithdraw: boolean): Promise<any[]> {
        let url = this.getUrl("/action/v1.0/SendAccountAction");
        const payload = {
            MemberID: accountID,
            AccountID: accountID,
            Currency: Currency.USDC,
            SettlementGroup: "SG1",
            Amount: isWithdraw ? "1" : "0",
            ActionType: "b",
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    // 增加币币
    static async addInstrument(baseCurrency: string, instrumentName: string): Promise<any[]> {
        let url = this.getUrl("/action/v1.0/SendInsert/Instrument");
        // let url = this.getUrl("/action/v1.0/SendClear/Instrument");
        const otherCurrency = Currency.USDC;
        const productGroup = "SPOT";
        const payload = {
            ExchangeID: "SCARD",
            InstrumentID: `${baseCurrency}/${otherCurrency}`,
            InstrumentName: instrumentName.replaceAll(" ", "_"),
            SettlementGroup: "SG1",
            ProductClass: 4,
            ProductGroup: productGroup,
            ProductType: 8,
            VolumeMultiple: 1,
            PriceCurrency: otherCurrency,
            ClearCurrency: otherCurrency,
            IsInverse: 0,
            BaseCurrency: baseCurrency,
            TradingModel: 0,
            DayStartTime: "00:00:00",
            ExpireTime: "",
            InstrumentStatus: 2,
            PriceTick: 0.0001,
            VolumeTick: 0.0001,
            MinOrderVolume: 0,
            TradingRightGroup: null,
            Remark: `M|${baseCurrency}/${otherCurrency}_6`,
        }
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }


    // 更新币种 单账户持有量限制
    static async updateCurrency(currency: string, privateEnd: number): Promise<any[]> {
        let url = this.getUrl("/action/v1.0/SendUpdate/Currency");
        const payload = {
            SettlementGroup: "SG1",
            Currency: currency,
            PrivateEnd: privateEnd,
        }
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    // 资金结算
    static async sendSettlement(instrumentId: string): Promise<any[]> {
        let url = this.getUrl("/action/v1.0/SendSettlement");
        const payload = {
            ExchangeID: "SCARD",
            InstrumentID: instrumentId,
            SettleAction: "2",
        }
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }

    // 手续费结算
    static async sendSettlement6(): Promise<any[]> {
        let url = this.getUrl("/action/v1.0/SendSettlement");
        const payload = {
            SettlementGroup: "SG1",
            SettleAction: "6",
        }
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result
    }


    // 查询币种信息
    static async getAccountCurrencyList(currency: string) {
        let url = this.getUrl("/private/v1.0/SendQryAccount");
        const payload = {
            SettlementGroup: "SG1",
            Currency: currency,
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result;
    }

    // 查询币种信息
    static async SendQryInstrument(instrumentID: string = null) {
        let url = this.getUrl("/action/v1.0/SendQryInstrument");
        const payload = {
            // ProductGroup: productGroup,
            // InstrumentID: instrumentID,
        };
        if (instrumentID) {
            payload['InstrumentID'] = instrumentID;
        }
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result;
    }

    // 关闭交易
    static async closeInstrument(instrumentId: string) {
        let url = this.getUrl("/action/v1.0/SendMarketDataUpdate");
        const payload = {
            ExchangeID: "SCARD",
            InstrumentID: instrumentId,
            InstrumentStatus: "7",
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result;
    }

    // 查询币种信息
    static async SendQryCurrency(currency: string = null): Promise<any[]> {
        let url = this.getUrl("/action/v1.0/SendQryCurrency");
        const payload = {
            SettlementGroup: "SG1",
        };
        if (currency) {
            payload['Currency'] = currency;
        }
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result;
    }

    // 增加OTCTrade
    static async SendOTCTradeInsert(instrumentID: string, price: string, volume: string | number, buyAccountID: string, sellAccountID: string) {
        let url = this.getUrl("/action/v1.0/SendOTCTradeInsert");
        const payload = {
            ExchangeID: "SCARD",
            InstrumentID: instrumentID,
            Price: price,
            Volume: volume,
            BuyMemberID: buyAccountID,
            BuyAccountID: buyAccountID,
            SellMemberID: sellAccountID,
            SellAccountID: sellAccountID,
            OTCType: "7",
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result;
    }

    // 查询会员信息
    static async SendQryMember(): Promise<any[]> {
        let url = this.getUrl("/action/v1.0/SendQryMember");
        const payload = {
            MemberType: "a",
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result;
    }

    // 查询最后的K线数据
    static async QryLastKLine(instrumentID: string): Promise<any> {
        let url = this.getUrl("/action/v1.0/SendQryOne/LastKLine");
        const payload = {
            ExchangeID: "SCARD",
            InstrumentID: instrumentID,
            Bar: "1h",
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            return null;
        }
        return res.result[0];
    }

    // 查询最后的K线数据
    static async SendQryMarketData(): Promise<any> {
        let url = this.getUrl("/action/v1.0/SendQryMarketData");
        const payload = {
            ExchangeID: "SCARD",
            // InstrumentID: instrumentID,
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result;
    }

    // 更新币种信息
    static async UpdateCurrency(currency: string, remark: string): Promise<any> {
        let url = this.getUrl("/action/v1.0/SendUpdate/Currency");
        const payload = {
            SettlementGroup: "SG1",
            Currency: currency,
            Remark: remark
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result;
    }

    // 获取交易系统业务配置
    static async QryServiceConfig(configName: string, index1: string, index2: string): Promise<any> {
        let url = this.getUrl("/action/v1.0/SendQryOne/ServiceConfig");
        const payload = {
            ConfigName: configName,
            Index1: index1,
            Index2: index2,
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result[0].data;
    }

    // 全部撤单
    static async cancelAllOrdersByInstrumentID(owner: string, instrumentID: string) {
        let url = this.getUrl("/action/v1.0/SendOrderAction");
        const payload = {
            ActionFlag: "1",
            ExchangeID: "SCARD",
            InstrumentID: instrumentID,
            MemberID: owner,
        };
        const res = await HttpRequest.post(url, payload);
        if (!res || res.errorCode !== 0) {
            throw BusinessError.newMsg(res.errorMsg);
        }
        return res.result[0].data;
    }
}