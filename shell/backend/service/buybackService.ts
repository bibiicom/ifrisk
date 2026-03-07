import KLineModel from "../models/KLineModel";
import {LocalRemote} from "../remote/LocalRemote";
import Decimal from "decimal.js";

/**
 * Buyback服务类，处理Buyback相关的业务逻辑
 */
export class BuybackService {

    async getBuybackPrice(instrumentID: string) {
        let kLineModels: any[] = await KLineModel.findAll({
            where: {
                InstrumentID: instrumentID,
                Bar: "1h",
            },
            order: [
                ['InsertTime', 'DESC'],
            ],
            limit: 7 * 24,
        });
        try {
            let lastKLine = await LocalRemote.QryLastKLine(instrumentID);
            if (lastKLine && lastKLine.data.Volume > 0) {
                kLineModels.push({
                    InsertTime: lastKLine.data.InsertTime,
                    ClosePrice: new Decimal(String(lastKLine.data.ClosePrice)),
                    Volume: new Decimal(String(lastKLine.data.Volume)),
                });
            }
        } catch (e) {
            return;
        }

        let totalPrice = new Decimal("0");
        let totalVolume = new Decimal("0");
        for (let kLineModel of kLineModels) {
            totalPrice = totalPrice.add(kLineModel.ClosePrice.mul(kLineModel.Volume));
            totalVolume = totalVolume.add(kLineModel.Volume);
        }
        let avgPrice = new Decimal("0");
        if (totalVolume.eq(0)) {
            return;
        } else {
            avgPrice = totalPrice.div(totalVolume);
        }
        return avgPrice.toDecimalPlaces(8);
    }


}

export default new BuybackService();
