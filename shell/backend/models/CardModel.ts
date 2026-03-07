import {DataTypes, Model} from 'sequelize';
import {sequelize} from '../config/database';
import Decimal from "decimal.js";

class CardModel extends Model {
    public Id: number; // 主键
    public Contract: string; // 合约地址 （nft 为合约地址，实物为 ""）
    public TokenID: string; // 编号 （nft 为 token id，实物为 ""）
    public Owner: string; // 拥有者
    public Img: string; // 图片
    public ImgGroup: string; // 其他图片组
    public Valuation: Decimal; // 估值
    public Status: number; // 1：持仓可上架 2：已上架申购 3：申请提取到钱包 4：申请出库中 5：开始交易
    public Name: string; // 名称
    public Category: number; // 类别 1：nft 2：实物
    public Description: string; // 简介
    public CertNumber: string; // 证书编号
    public ItemGrade: string; // psa等级
    public Year: string; // 年份
    public BrandTitle: string; // 品牌/标题
    public Subject: string; // 项目/主题
    public CardNumber: string; // 卡片编号
    public VarietyPedigree: string; // 品种/血统
    public readonly createdAt!: Date; // 创建时间，由sequelize自动管理
    public readonly updatedAt!: Date; // 更新时间，由sequelize自动管理
    public readonly deletedAt: Date; // 软删除时间，由sequelize自动管理
}


CardModel.init({
        Id: {
            type: DataTypes.BIGINT,
            primaryKey: true,
            autoIncrement: true,
            allowNull: false,
            comment: "主键"
        },
        Contract: {
            type: DataTypes.STRING,
            allowNull: true,
            comment: "合约地址"
        },
        TokenID: {
            type: DataTypes.STRING,
            allowNull: true,
            comment: "编号"
        },
        Owner: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "拥有者"
        },
        Img: {
            type: DataTypes.STRING,
            allowNull: true,
            comment: "图片"
        },
        ImgGroup: {
            type: DataTypes.TEXT,
            allowNull: true,
            comment: "其他图片组"
        },
        Valuation: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "估值",
            defaultValue: "0",
            get() {
                return new Decimal(this.getDataValue('Valuation'));
            },
            set(value: Decimal) {
                this.setDataValue('Valuation', value.toFixed());
            },
        },
        Status: {
            type: DataTypes.INTEGER,
            allowNull: false,
            defaultValue: 0,
            comment: "状态 0：持仓 1：申请上架中 2：已上架 3：申请提取到钱包 4：申请出库中"
        },
        Name: {
            type: DataTypes.STRING,
            allowNull: true,
            defaultValue: "",
            comment: "名称"
        },
        Category: {
            type: DataTypes.INTEGER,
            allowNull: false,
            comment: "类别 1：nft 2：实物"
        },
        Description: {
            type: DataTypes.TEXT,
            allowNull: true,
            defaultValue: "",
            comment: "简介"
        },
        CertNumber: {
            type: DataTypes.STRING,
            allowNull: true,
            defaultValue: "",
            comment: "证书编号"
        },
        ItemGrade: {
            type: DataTypes.STRING,
            allowNull: true,
            defaultValue: "",
            comment: "psa等级"
        },
        Year: {
            type: DataTypes.STRING,
            allowNull: true,
            defaultValue: "",
            comment: "年份"
        },
        BrandTitle: {
            type: DataTypes.STRING,
            allowNull: true,
            defaultValue: "",
            comment: "品牌/标题"
        },
        Subject: {
            type: DataTypes.STRING,
            allowNull: true,
            defaultValue: "",
            comment: "项目/主题"
        },
        CardNumber: {
            type: DataTypes.STRING,
            allowNull: true,
            defaultValue: "",
            comment: "卡片编号"
        },
        VarietyPedigree: {
            type: DataTypes.STRING,
            allowNull: true,
            defaultValue: "",
            comment: "品种/血统"
        },
    },
    {
        sequelize,
        indexes: [
            {
                fields: ['Contract', 'TokenID'],
            },
        ],
        modelName: 'CardModel',
        tableName: 't_D_Card',
        timestamps: true,
        paranoid: true,
    }
);
export default CardModel;
