import {LocalRemote} from "../remote/LocalRemote";
import {ApprovalHandler} from "../handler/ApprovalHandler";
import {App} from "../base/App";
import * as Lark from "@larksuiteoapi/node-sdk";


async function run() {

    // let kLineModels = await KLineModel.findAll({
    //     where: {
    //         InstrumentID: "PikachuUSDT",
    //         Bar: "1h",
    //     },
    //     order: [
    //         ['InsertTime', 'DESC'],
    //     ],
    //     limit: 7 * 24,
    // });
    // console.log(kLineModels.length);
    // await sequelize.close();

    // let res = await HttpRequest.post("https://scardex.com/backend/v1/launchpad/addLaunchpadInfo", {
    //     currency: "KOBE",
    //     name: "Kobe Bryant",
    //     volume: "0.001",
    //     price: "1",
    //     cardId: "4",
    //     openTime: "2025-12-01 00:00:00",
    //     closeTime: "2025-12-31 00:00:00",
    //     tradeTime: "2025-12-31 00:00:00",
    //     summary: "Kobe Bryant Card",
    // });
    // console.log(res);

}

run();