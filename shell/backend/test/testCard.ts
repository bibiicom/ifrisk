import {sequelize} from "../config/database";
import CardModel from "../models/CardModel";
import BuyBackModel from "../models/BuyBackModel";

async function run() {
    // await CardModel.drop();
    // await CardModel.sync();
    await BuyBackModel.drop();
    await BuyBackModel.sync();


    // await CardModel.create({
    //     Contract: "0x1",
    //     Owner: "0x1aa490fdcc00a94e892be207b8b7cd6a73b47206",
    //     CreateTime: new Date(),
    //     Status: 0,
    // });
    // await CardModel.create({
    //     Contract: "0x2",
    //     Owner: "0x1aa490fdcc00a94e892be207b8b7cd6a73b47206",
    //     CreateTime: new Date(),
    //     Status: 0,
    // });
    // await CardModel.create({
    //     Contract: "0x3",
    //     Owner: "0x1aa490fdcc00a94e892be207b8b7cd6a73b47206",
    //     CreateTime: new Date(),
    //     Status: 0,
    // });
    //
    // await CardModel.create({
    //     Contract: "0x4",
    //     Owner: "0x2056bc10b0ae9a92ab28dc676053023dcd0d7ecc",
    //     CreateTime: new Date(),
    //     Status: 0,
    // });
    // await CardModel.create({
    //     Contract: "0x5",
    //     Owner: "0x2056bc10b0ae9a92ab28dc676053023dcd0d7ecc",
    //     CreateTime: new Date(),
    //     Status: 0,
    // });


    await sequelize.close();
}

run();
