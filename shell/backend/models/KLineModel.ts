import {DataTypes, Model} from 'sequelize';
import {sequelize} from '../config/database';
import Decimal from "decimal.js";

class KLineModel extends Model {
    public ExchangeID: string; // 交易所代码
    public InstrumentID: string; // 标的代码
    public Bar: string; // K线周期代码
    public InsertTime: Decimal; // 插入时间
    public OpenPrice: Decimal; // 开盘价
    public HighestPrice: Decimal; // 最高价
    public LowestPrice: Decimal; // 最低价
    public ClosePrice: Decimal; // 收盘价
    public Volume: Decimal; // 数量
    public Turnover: Decimal; // 成交金额
    public DBTime: Date; // 落库时间
}


KLineModel.init({
        ExchangeID: {
            type: DataTypes.CHAR(8),
            primaryKey: true,
            allowNull: false,
            comment: "交易所代码"
        },
        InstrumentID: {
            type: DataTypes.CHAR(26),
            primaryKey: true,
            allowNull: false,
            comment: "标的代码"
        },
        Bar: {
            type: DataTypes.CHAR(3),
            primaryKey: true,
            allowNull: false,
            comment: "K线周期代码"
        },
        InsertTime: {
            type: DataTypes.DECIMAL(20),
            primaryKey: true,
            allowNull: false,
            comment: "插入时间",
            get() {
                return new Decimal(this.getDataValue('InsertTime'));
            },
            set(value: Decimal) {
                this.setDataValue('InsertTime', value.toFixed());
            },
        },
        OpenPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "开盘价",
            get() {
                return new Decimal(this.getDataValue('OpenPrice'));
            },
            set(value: Decimal) {
                this.setDataValue('OpenPrice', value.toFixed());
            },
        },
        HighestPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "最高价",
            get() {
                return new Decimal(this.getDataValue('HighestPrice'));
            },
            set(value: Decimal) {
                this.setDataValue('HighestPrice', value.toFixed());
            },
        },
        LowestPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "最低价",
            get() {
                return new Decimal(this.getDataValue('LowestPrice'));
            },
            set(value: Decimal) {
                this.setDataValue('LowestPrice', value.toFixed());
            },
        },
        ClosePrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "收盘价",
            get() {
                return new Decimal(this.getDataValue('ClosePrice'));
            },
            set(value: Decimal) {
                this.setDataValue('ClosePrice', value.toFixed());
            },
        },
        Volume: {
            type: DataTypes.DECIMAL(36, 8),
            allowNull: true,
            comment: "数量",
            get() {
                return new Decimal(this.getDataValue('Volume'));
            },
            set(value: Decimal) {
                this.setDataValue('Volume', value.toFixed());
            },
        },
        Turnover: {
            type: DataTypes.DECIMAL(36, 8),
            allowNull: true,
            comment: "成交金额",
            get() {
                return new Decimal(this.getDataValue('Turnover'));
            },
            set(value: Decimal) {
                this.setDataValue('Turnover', value.toFixed());
            },
        },
        DBTime: {
            type: DataTypes.DATE,
            allowNull: false,
            defaultValue: DataTypes.NOW,
            comment: "落库时间"
        },
    }, {
        sequelize,
        indexes: [
            {
                fields: ['ExchangeID', 'InstrumentID', 'Bar'],
                name: 'mykey_0',
            },
            {
                fields: ['Bar', 'InsertTime'],
                name: 'mykey_1',
            },
        ],
        modelName: 'KLineModel',
        tableName: 't_D_KLine',
        timestamps: false
    }
);

export default KLineModel;
