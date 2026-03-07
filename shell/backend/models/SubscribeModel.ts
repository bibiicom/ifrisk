import {DataTypes, Model} from 'sequelize';
import {sequelize} from '../config/database';
import Decimal from "decimal.js";

class SubscribeModel extends Model {
    public LaunchpadID: number; // Launchpad表主键
    public Currency: string; // 币种
    public AccountID: string; // 用户钱包地址
    public Volume: Decimal; // 购买数量
    public AllocatedVolume: Decimal; // 发放数量
    public Price: Decimal; // 购买单价
    public TotalPrice: Decimal; // 购买总价格
    public AllocatedTotalPrice: Decimal; // 发放总价格
    public OrderID: string; // 订单ID
    public Status: number; // 状态 0-待处理 1-已发放 2-已退款 3-待发放
    public readonly createdAt!: Date; // 创建时间，由sequelize自动管理
    public readonly updatedAt!: Date; // 更新时间，由sequelize自动管理
    public readonly deletedAt: Date; // 软删除时间，由sequelize自动管理
}


SubscribeModel.init(
    {
        LaunchpadID: {
            type: DataTypes.BIGINT,
            allowNull: false,
            comment: "Launchpad表主键"
        },
        Currency: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "币种"
        },
        AccountID: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "用户钱包地址"
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
        AllocatedVolume: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: false,
            defaultValue: new Decimal(0),
            comment: "发放数量",
            get() {
                return new Decimal(this.getDataValue('AllocatedVolume'));
            },
            set(value: Decimal) {
                this.setDataValue('AllocatedVolume', value.toFixed());
            }
        },
        AllocatedTotalPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: false,
            defaultValue: new Decimal(0),
            comment: "发放总价格",
            get() {
                return new Decimal(this.getDataValue('AllocatedTotalPrice'));
            },
            set(value: Decimal) {
                this.setDataValue('AllocatedTotalPrice', value.toFixed());
            }
        },
        OrderID: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "订单ID"
        },
        Status: {
            type: DataTypes.TINYINT,
            allowNull: true,
            defaultValue: 0,
            comment: "状态 0-待处理 1-已发放 2-已退款 3-待发放"
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
        modelName: 'SubscribeModel',
        tableName: 't_D_Subscribe',
        timestamps: true,
        paranoid: true,
    }
);
// SubscribeModel.removeAttribute('id');
export default SubscribeModel;