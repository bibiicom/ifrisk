import {LocalRemote} from "../remote/LocalRemote";
import KLineModel from "../models/KLineModel";
import {sequelize} from "../config/database";
import {HttpRequest} from "../tool/HttpRequest";
import {LarkRemote} from "../remote/LarkRemote";


async function run() {
    const uid = '0x2056bc10b0ae9a92ab28dc676053023dcd0d7ecc'
    const uid2 = '0x1aa490fdcc00a94e892be207b8b7cd6a73b47206'
    const uid3 = '0x564ac2d78db2eb61e03241b3432775f170a217c3'
    const uid4 = '0x7168a24b757564ef3aa9da1afe1677167bf5043c'
    const uid5 = '0xedee6d1aea5728050d470b6571b9df1e7db1099c'
    const uid6 = '0x5da17c2e8afaa9c27a2c74118acc8f43e8e4c0c0'
    const uid7 = '0xb8482b90bfca07327048b50ee878d0f4c1376215'
    const uid8 = '0xc9b6a85a6d44dbacb20f445ab8247f196b2fdede'
    const uid9 = '0xc7b4e95641a7ae19c186410ee611eb62b84b1241'
    const uid10 = '0xe07f9c2b2d3c3e6ff51d87ea4e27b0cfc1a7b73f'
    // await getOrder(uid, null, "100709152804224");
    // let newVar = await LocalRemote.addInstrument("PP");
    // console.log(newVar);
    // let accountInfo = await LocalRemote.getAccountInfo(uid, "kkkkk");
    // console.log(accountInfo);
    // let newVar1 = await LocalRemote.addMoney(uid2, "5000", "USDC");
    // console.log(newVar1);
    // let newVar2 = await LocalRemote.addMoney(uid, "5000", "USDC");
    // console.log(newVar2);
    // let newVar1 = await LocalRemote.useMoneyByCurrency(uid, "0.06", "kkkkk");
    // console.log(newVar1);

    // let newVar = await LocalRemote.orderFrozenMoney(uid2, "0.021569759094755273529", "215.69759094755273529","0.0001", "USDT");
    // console.log(newVar);
    // let newVar = await LocalRemote.releaseFrozenMoneyByOrderID(uid2, "1007091550975330");
    // console.log(newVar);
    // let newVar1 = await LocalRemote.getOrdersByInstrumentID("sub_OHTANI");
    // console.log(newVar1);
    // let newVar = await LocalRemote.insUpdInstrument("KOBE");
    // console.log(newVar);
    // let newVar1 = await LocalRemote.getOrdersByInstrumentID("sub_OHTANI");
    // console.log(newVar1);
    // let newVar = await LocalRemote.insUpdInstrument("KOBE");
    // console.log(newVar);

    // let currency = await LocalRemote.SendOTCTradeInsert("LukaDoncicUSDT", "10000", "0.00001", "0x2056bc10b0ae9a92ab28dc676053023dcd0d7ecc", "0x1aa490fdcc00a94e892be207b8b7cd6a73b47206");
    // console.log(currency);

    // let instrument = await LocalRemote.SendQryInstrument("KOBE/USDT");
    // let instrument = await LocalRemote.SendQryInstrument("LukaDoncicUSDT");
    // let instrument = await LocalRemote.addInstrument("KOBE");
    // console.log(instrument);

    // let instrument = await LocalRemote.switchWithdraw(uid, true);
    // console.log(instrument);
    // let currency = await LocalRemote.SendOTCTradeInsert("ShoheiOhtaniUSDT", "10", "0.0001", uid2, uid);
    // console.log(currency);

    // let currency = await LocalRemote.SendQryCurrency();
    // for (let currencyElement of currency) {
    //     if (currencyElement.data.Currency == "yuyu") {
    //         console.log(currencyElement)
    //         let instrument = await LocalRemote.SendQryInstrument("yuyu/USDC");
    //         console.log(instrument)
    //     }
    // }
    // console.log(currency);

    // let instrument = await LocalRemote.closeInstrument("yuyu/USDC");
    // console.log(instrument);
    //
    // let currency = await LocalRemote.sendSettlement("yuyu");
    // console.log(currency);

    // let currency = await LocalRemote.SendQryMember();
    // console.log(currency);

    // let accountInfo = await LocalRemote.getAccountInfoList(uid);
    // console.log(accountInfo);

    // let accountInfo = await LocalRemote.QryLastKLine("LukaDoncicUSDT");
    // console.log(accountInfo);


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
    //     cardId: "3",
    //     openTime: "2025-12-01 00:00:00",
    //     closeTime: "2025-12-31 00:00:00",
    //     tradeTime: "2025-12-31 00:00:00",
    //     summary: "Kobe Bryant Card",
    // });
    // console.log(res);
    // await LarkRemote.notifyLark("133\n --- \n43");

    // let currency = "X003";
    // let acl = await LocalRemote.getAccountCurrencyList(currency);
    // for (let aclElement of acl) {
    //     console.log(aclElement.data.AccountID, aclElement.data.Balance);
    //     await LocalRemote.useMoneyByCurrency(aclElement.data.AccountID, aclElement.data.Balance, currency);
    // }
    // let instrument = await LocalRemote.closeInstrument(`${currency}/USDC`);
    // console.log(instrument);
    //
    // let settlement = await LocalRemote.sendSettlement(`${currency}/USDC`);
    // console.log(settlement);
    //
    // let currency = await LocalRemote.UpdateCurrency("Pikachu","2020 Pikachu M".replaceAll(" ","_"));
    // console.log(currency);
    // let newVar = await LocalRemote.closeInstrument("MIKETROUT/USDC");
    // console.log(newVar);

    // let currency = await LocalRemote.SendQryCurrency();
    // console.log(currency);
    // let marketDataList = await LocalRemote.SendQryMarketData();
    // console.log(marketDataList);
    // let acl = await LocalRemote.getAccountCurrencyList("PPDP");
    // console.log(acl);
    // let instrument = await LocalRemote.closeInstrument("PPDP/USDC");
    // console.log(instrument);
    // let newVar = await LocalRemote.sendSettlement("PPDP/USDC");
    // console.log(newVar);
}

run();