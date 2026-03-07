import {BaseHandler} from "../base/core/BaseHandler";
import CardModel from "../models/CardModel";
import {LarkRemote} from "../remote/LarkRemote";

export default function () {
    return new commonHandler();
}
export var routeName: string = "backend/v1/common";

export class commonHandler extends BaseHandler {
    async preHandle(msg: any, session: any): Promise<any> {
        return null;
    }

    async test(msg: any, session: any) {
        return this.suc({})
    }

    async addCard(msg: {
        hash: string,
        block: number,
        contract: string,
        tokenId: string,
        from: string,
        to: string,
        timestamp: number
    }, session: any) {
        await CardModel.create({
            Contract: msg.contract,
            TokenID: msg.tokenId,
            Owner: msg.from,
            Status: 1,
            Category: 1,
        });
        return this.suc({})
    }

    async delCard(msg: {
        contract: string,
        tokenId: string,
    }, session: any) {
        await CardModel.destroy({
            where: {
                Contract: msg.contract,
                TokenID: msg.tokenId,
                Status: 3,
            }
        });
        return this.suc({})
    }

    async getWithdrawCardList(msg: {}, session: any) {
        const cardList = await CardModel.findAll({
            where: {
                Status: 3,
            },
            raw: true,
        });
        return this.suc(cardList);
    }

    async notify(msg: {
        nAction: string,
        AccountID: string,
        Amount: number,
        Currency: string,
        TxHash: string
    }, session: any) {
        if (msg.nAction == "Withdraw") {
            // 出金 通知到lark
            LarkRemote.notifyLark(`出金通知: ${msg.AccountID} \n出金 ${msg.Amount} ${msg.Currency} \nhash: ${msg.TxHash}`, "money");
        } else if (msg.nAction == "Deposit") {
            // 入金 通知到lark
            LarkRemote.notifyLark(`入金通知: ${msg.AccountID} \n入金 ${msg.Amount} ${msg.Currency} \nhash: ${msg.TxHash}`, "money");
        }
        return this.suc({});
    }

}
