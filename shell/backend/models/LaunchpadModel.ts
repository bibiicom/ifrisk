import {DataTypes, Model} from 'sequelize';
import {sequelize} from '../config/database';
import Decimal from "decimal.js";

class LaunchpadModel extends Model {
    public Id: number; // 主键
    public Currency: string; // 币种
    public Name: string; // 名称
    public Summary: string; // 摘要
    public AllVolume: Decimal; // 总量
    public RoundVolume: Decimal; // 本轮总量
    // public StockVolume: Decimal; // 库存量
    public FeePct: Decimal; // 手续费百分比  8 表示 8%
    public TradeStandardPct: Decimal; // 交易标准  20 表示 20%
    public HaltStandardPct: Decimal; // 停牌标准  80 表示 80%
    public Price: Decimal; // 单价
    public OpenTime: Date; // 开启时间
    public CloseTime: Date; // 关闭时间
    public Round: string; // 轮次
    public MinPrice: Decimal; // 最小价
    public MaxPrice: Decimal; // 最大价
    public TradeTime: Date; // 交易开启时间
    public Status: number; // 上架状态 0-未上架 1-已上架 2-申购结束（成功） 3-已回购 4-申购结束（已退款）
    public CardID: number; // 卡片ID
    public readonly createdAt!: Date; // 创建时间，由sequelize自动管理
    public readonly updatedAt!: Date; // 更新时间，由sequelize自动管理
    public readonly deletedAt: Date; // 软删除时间，由sequelize自动管理
}


LaunchpadModel.init({
        Id: {
            type: DataTypes.BIGINT,
            autoIncrement: true,
            primaryKey: true,
            comment: "主键"
        },
        Currency: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "币种"
        },
        Name: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "名称"
        },
        Summary: {
            type: DataTypes.TEXT,
            allowNull: true,
            comment: "摘要",
            defaultValue: "",
        },
        AllVolume: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "总量",
            get() {
                let value = this.getDataValue('AllVolume');
                if (!value) {
                    return null;
                }
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('AllVolume', null);
                } else {
                    this.setDataValue('AllVolume', value.toFixed());
                }
            }
        },
        RoundVolume: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "本轮总量",
            get() {
                let value = this.getDataValue('RoundVolume');
                if (!value) {
                    return null;
                }
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('RoundVolume', null);
                } else {
                    this.setDataValue('RoundVolume', value.toFixed());
                }
            }
        },
        // StockVolume: {
        //     type: DataTypes.DECIMAL(28, 15),
        //     allowNull: true,
        //     comment: "库存量",
        //     get() {
        //         let value = this.getDataValue('StockVolume');
        //         if (!value) {
        //             return null;
        //         }
        //         return new Decimal(value);
        //     },
        //     set(value: Decimal) {
        //         if (!value) {
        //             this.setDataValue('StockVolume', null);
        //         } else {
        //             this.setDataValue('StockVolume', value.toFixed());
        //         }
        //     }
        // },
        FeePct: {
            type: DataTypes.DECIMAL(10, 5),
            allowNull: true,
            comment: "手续费百分比  8 表示 8%",
            get() {
                let value = this.getDataValue('FeePct');
                if (!value) {
                    return null;
                }
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('FeePct', null);
                } else {
                    this.setDataValue('FeePct', value.toFixed());
                }
            }
        },
        TradeStandardPct: {
            type: DataTypes.DECIMAL(10, 5),
            allowNull: true,
            comment: "交易标准  20 表示 20%",
            get() {
                let value = this.getDataValue('TradeStandardPct');
                if (!value) {
                    return null;
                }
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('TradeStandardPct', null);
                } else {
                    this.setDataValue('TradeStandardPct', value.toFixed());
                }
            }
        },
        HaltStandardPct: {
            type: DataTypes.DECIMAL(10, 5),
            allowNull: true,
            comment: "停牌标准  80 表示 80%",
            get() {
                let value = this.getDataValue('HaltStandardPct');
                if (!value) {
                    return null;
                }
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('HaltStandardPct', null);
                } else {
                    this.setDataValue('HaltStandardPct', value.toFixed());
                }
            }
        },
        Price: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "单价",
            get() {
                let value = this.getDataValue('Price');
                if (!value) {
                    return null;
                }
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Price', null);
                } else {
                    this.setDataValue('Price', value.toFixed());
                }
            }
        },
        OpenTime: {
            type: DataTypes.DATE,
            allowNull: true,
            comment: "开启时间"
        },
        CloseTime: {
            type: DataTypes.DATE,
            allowNull: true,
            comment: "关闭时间"
        },
        Round: {
            type: DataTypes.STRING,
            allowNull: true,
            comment: "轮次"
        },
        MinPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "最小价",
            get() {
                let value = this.getDataValue('MinPrice');
                if (!value) {
                    return null;
                }
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('MinPrice', null);
                } else {
                    this.setDataValue('MinPrice', value.toFixed());
                }
            }
        },
        MaxPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "最大价",
            get() {
                let value = this.getDataValue('MaxPrice');
                if (!value) {
                    return null;
                }
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('MaxPrice', null);
                } else {
                    this.setDataValue('MaxPrice', value.toFixed());
                }
            }
        },
        TradeTime: {
            type: DataTypes.DATE,
            allowNull: true,
            comment: "交易开启时间"
        },
        Status: {
            type: DataTypes.INTEGER,
            comment: "上架状态 0-未上架 1-已上架 2-申购结束 3-已回购",
            defaultValue: 0,
        },
        CardID: {
            type: DataTypes.BIGINT,
            allowNull: true,
            comment: "卡片ID"
        }
    }, {
        sequelize,
        indexes: [
            {
                fields: ['Currency'],
                name: 'idx_currency',
            },
        ],
        modelName: 'LaunchpadModel',
        tableName: 't_D_Launchpad',
        timestamps: true,
        paranoid: true,
    }
);

export default LaunchpadModel;