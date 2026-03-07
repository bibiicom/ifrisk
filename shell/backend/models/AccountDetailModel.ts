import {DataTypes, Model} from 'sequelize';
import {sequelize} from '../config/database';
import Decimal from "decimal.js";

/**
 * 账户流水 Model
 */
class AccountDetailModel extends Model {
    public AccountDetailID: string;
    public MemberID: string;
    public ExchangeID: string;
    public InstrumentID: string;
    public SettlementGroup: string;
    public AccountID: string;
    public Currency: string;
    public Amount: Decimal;
    public PreBalance: Decimal;
    public Balance: Decimal;
    public ReserveAmount: Decimal;
    public ReserveBalance: Decimal;
    public Source: string;
    public Remark: string;
    public LocalID: string;
    public SettleSegment: string;
    public BusinessNo: number;
    public RelatedID: string;
    public InsertTime: number;
    public readonly CreateTime: Date;
    public readonly DBTime: Date;
}

AccountDetailModel.init(
    {
        AccountDetailID: {
            type: DataTypes.CHAR(20),
            primaryKey: true,
            comment: "资金明细号"
        },
        MemberID: {
            type: DataTypes.CHAR(42),
            allowNull: true,
            comment: "成员代码"
        },
        ExchangeID: {
            type: DataTypes.CHAR(8),
            allowNull: true,
            comment: "交易所代码"
        },
        InstrumentID: {
            type: DataTypes.CHAR(26),
            allowNull: true,
            comment: "标的代码"
        },
        SettlementGroup: {
            type: DataTypes.CHAR(8),
            allowNull: true,
            comment: "结算组编号"
        },
        AccountID: {
            type: DataTypes.CHAR(44),
            allowNull: true,
            comment: "资金账号"
        },
        Currency: {
            type: DataTypes.CHAR(12),
            allowNull: true,
            comment: "币种"
        },
        Amount: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "发生额",
            get() {
                const value = this.getDataValue('Amount');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Amount', null);
                } else {
                    this.setDataValue('Amount', value.toFixed());
                }
            }
        },
        PreBalance: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "上次静态权益",
            get() {
                const value = this.getDataValue('PreBalance');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('PreBalance', null);
                } else {
                    this.setDataValue('PreBalance', value.toFixed());
                }
            }
        },
        Balance: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "静态权益",
            get() {
                const value = this.getDataValue('Balance');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Balance', null);
                } else {
                    this.setDataValue('Balance', value.toFixed());
                }
            }
        },
        ReserveAmount: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "体验金发生额",
            get() {
                const value = this.getDataValue('ReserveAmount');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('ReserveAmount', null);
                } else {
                    this.setDataValue('ReserveAmount', value.toFixed());
                }
            }
        },
        ReserveBalance: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "体验金静态权益",
            get() {
                const value = this.getDataValue('ReserveBalance');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('ReserveBalance', null);
                } else {
                    this.setDataValue('ReserveBalance', value.toFixed());
                }
            }
        },
        Source: {
            type: DataTypes.CHAR(1),
            allowNull: true,
            comment: "财务流水类型"
        },
        Remark: {
            type: DataTypes.STRING(64),
            allowNull: true,
            comment: "备注"
        },
        LocalID: {
            type: DataTypes.CHAR(20),
            allowNull: true,
            comment: "本地标识"
        },
        SettleSegment: {
            type: DataTypes.CHAR(20),
            allowNull: true,
            comment: "结算段"
        },
        BusinessNo: {
            type: DataTypes.DECIMAL(16, 0),
            allowNull: true,
            comment: "业务序列号",
            get() {
                const value = this.getDataValue('BusinessNo');
                return value ? Number(value) : null;
            }
        },
        RelatedID: {
            type: DataTypes.STRING(64),
            allowNull: true,
            comment: "内外对账ID"
        },
        InsertTime: {
            type: DataTypes.DECIMAL(20, 0),
            allowNull: true,
            comment: "插入时间",
            get() {
                const value = this.getDataValue('InsertTime');
                return value ? Number(value) : null;
            }
        },
        CreateTime: {
            type: DataTypes.DATE,
            allowNull: false,
            defaultValue: DataTypes.NOW,
            comment: "本地创建时间"
        },
        DBTime: {
            type: DataTypes.DATE,
            allowNull: false,
            defaultValue: DataTypes.NOW,
            comment: "落库时间"
        }
    },
    {
        sequelize,
        indexes: [],
        modelName: 'AccountDetailModel',
        tableName: 't_D_AccountDetail',
        timestamps: false,
    }
);

export default AccountDetailModel;
