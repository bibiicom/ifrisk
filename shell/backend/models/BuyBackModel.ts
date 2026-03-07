import {DataTypes, Model} from 'sequelize';
import {sequelize} from '../config/database';
import Decimal from "decimal.js";

class BuyBackModel extends Model {
    public LaunchpadID: number; // Launchpad表主键
    public Currency: string; // 要购买的币种
    public InstrumentID: string; // 标的
    public AccountID: string; // 买方钱包地址
    public Volume: Decimal; // 购买数量
    public Price: Decimal; // 购买单价
    public TotalPrice: Decimal; // 购买总价格
    public Status: number; // 状态 0-回购中 1-已回购
    public readonly createdAt!: Date; // 创建时间，由sequelize自动管理
    public readonly updatedAt!: Date; // 更新时间，由sequelize自动管理
    public readonly deletedAt: Date; // 软删除时间，由sequelize自动管理
}


BuyBackModel.init(
    {
        LaunchpadID: {
            type: DataTypes.BIGINT,
            allowNull: false,
            comment: "Launchpad表主键"
        },
        Currency: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "要购买的币种"
        },
        InstrumentID: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "标的"
        },
        AccountID: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "买方钱包地址"
        },
        Volume: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: false,
            comment: "购买数量",
            get() {
                return new Decimal(this.getDataValue('Volume'));
            },
            set(value: Decimal) {
                this.setDataValue('Volume', value.toFixed());
            }
        },
        Price: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: false,
            comment: "购买单价",
            get() {
                return new Decimal(this.getDataValue('Price'));
            },
            set(value: Decimal) {
                this.setDataValue('Price', value.toFixed());
            }
        },
        TotalPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: false,
            comment: "购买总价格",
            get() {
                return new Decimal(this.getDataValue('TotalPrice'));
            },
            set(value: Decimal) {
                this.setDataValue('TotalPrice', value.toFixed());
            }
        },
        Status: {
            type: DataTypes.TINYINT,
            allowNull: true,
            defaultValue: 0,
            comment: "状态 0-待处理 1-已处理"
        },

    },
    {
        sequelize,
        indexes: [
            {
                fields: ['LaunchpadID'],
                name: 'idx_launchpadID',
            },
            {
                fields: ['AccountID'],
                name: 'idx_accountID',
            }
        ],
        modelName: 'BuyBackModel',
        tableName: 't_D_BuyBack',
        timestamps: true,
        paranoid: true,
    }
);

export default BuyBackModel;