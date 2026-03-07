import {BaseHandler} from "../base/core/BaseHandler";
import CardModel from "../models/CardModel";
import {sequelize} from "../config/database";
import {ErrorCode} from "../const/ErrorCode";
import Decimal from "decimal.js";
import {Op} from "sequelize";

export default function () {
    return new larkCardHandler();
}
export var routeName: string = "backend/v1/larkCard";

export class larkCardHandler extends BaseHandler {

    async preHandle(msg: any, session: any): Promise<any> {
        return null;
    }

    /**
     * 新增卡片
     * Status 默认为 1
     */
    async add(msg: {
        owner: string,
        name: string,
        valuation?: string,
        img?: string,
        imgGroup?: string,
        description?: string,
        certNumber?: string,
        itemGrade?: string,
        year?: string,
        brandTitle?: string,
        subject?: string,
        cardNumber?: string,
        varietyPedigree?: string,
    }, session: any) {
        if (!msg.owner || !msg.name) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        return await sequelize.transaction(async (t) => {
            const cardModel = await CardModel.create({
                Owner: msg.owner,
                Name: msg.name,
                Category: 2,
                Valuation: new Decimal(msg.valuation || "0"),
                Img: msg.img || "",
                ImgGroup: msg.imgGroup || "",
                Description: msg.description || "",
                CertNumber: msg.certNumber || "",
                ItemGrade: msg.itemGrade || "",
                Year: msg.year || "",
                BrandTitle: msg.brandTitle || "",
                Subject: msg.subject || "",
                CardNumber: msg.cardNumber || "",
                VarietyPedigree: msg.varietyPedigree || "",
                Status: 1, // 强制默认 Status 为 1
            }, {transaction: t});

            return this.suc(cardModel);
        });
    }

    /**
     * 删除卡片
     */
    async delete(msg: {
        id: string
    }, session: any) {
        const id = Number(msg.id);
        if (!id) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        return await sequelize.transaction(async (t) => {
            const cardModel = await CardModel.findByPk(id, {transaction: t});
            if (!cardModel || cardModel.Status != 1) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }
            await cardModel.destroy({transaction: t});
            return this.suc({id});
        });
    }

    /**
     * 修改卡片
     * 不能修改 Status
     */
    async update(msg: {
        id: string,
        owner?: string,
        name?: string,
        category?: number,
        valuation?: string,
        img?: string,
        imgGroup?: string,
        description?: string,
        certNumber?: string,
        itemGrade?: string,
        year?: string,
        brandTitle?: string,
        subject?: string,
        cardNumber?: string,
        varietyPedigree?: string,
    }, session: any) {
        const id = Number(msg.id);
        if (!id) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }

        return await sequelize.transaction(async (t) => {
            const cardModel = await CardModel.findByPk(id, {transaction: t});
            if (!cardModel) {
                return this.fail(ErrorCode.enum.ERR_PARAM);
            }

            // 更新字段，明确排除 Status
            if (msg.owner !== undefined) cardModel.Owner = msg.owner;
            if (msg.name !== undefined) cardModel.Name = msg.name;
            if (msg.valuation !== undefined) cardModel.Valuation = new Decimal(msg.valuation);
            if (msg.img !== undefined) cardModel.Img = msg.img;
            if (msg.imgGroup !== undefined) cardModel.ImgGroup = msg.imgGroup;
            if (msg.description !== undefined) cardModel.Description = msg.description;
            if (msg.certNumber !== undefined) cardModel.CertNumber = msg.certNumber;
            if (msg.itemGrade !== undefined) cardModel.ItemGrade = msg.itemGrade;
            if (msg.year !== undefined) cardModel.Year = msg.year;
            if (msg.brandTitle !== undefined) cardModel.BrandTitle = msg.brandTitle;
            if (msg.subject !== undefined) cardModel.Subject = msg.subject;
            if (msg.cardNumber !== undefined) cardModel.CardNumber = msg.cardNumber;
            if (msg.varietyPedigree !== undefined) cardModel.VarietyPedigree = msg.varietyPedigree;
            await cardModel.save({transaction: t});
            return this.suc(cardModel);
        });
    }

    /**
     * 获取卡片详情
     */
    async getDetail(msg: {
        id: string
    }, session: any) {
        const id = Number(msg.id);
        if (!id) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        const cardModel = await CardModel.findByPk(id, {raw: true});
        if (!cardModel) {
            return this.fail(ErrorCode.enum.ERR_PARAM);
        }
        return this.suc(cardModel);
    }

    /**
     * 获取卡片列表
     */
    async getList(msg: {
        page?: string,
        pageSize?: string,
        owner?: string,
        name?: string,
        status?: number, // 允许按状态查询，但不允许修改
    }, session: any) {
        const page = parseInt(msg.page || "1") || 1;
        const pageSize = parseInt(msg.pageSize || "10") || 10;
        const offset = (page - 1) * pageSize;

        const where: any = {};
        if (msg.owner) {
            where.Owner = msg.owner;
        }
        if (msg.name) {
            where.Name = {[Op.like]: `%${msg.name}%`};
        }
        if (msg.status !== undefined) {
            where.Status = msg.status;
        }

        const result = await CardModel.findAndCountAll({
            where,
            order: [
                ['Id', 'DESC'],
            ],
            offset,
            limit: pageSize,
            raw: true,
        });

        return this.suc(result);
    }
}
