import { DataTypes, Model } from 'sequelize';
import { sequelize } from '../config/database';
import Decimal from "decimal.js";

/**
 * 完成的报单 Model
 */
class FinishOrderModel extends Model {
    public MemberID: string;
    public TradeUnitID: string;
    public AccountID: string;
    public LocalID: string;
    public ExchangeID: string;
    public InstrumentID: string;
    public OrderPriceType: string;
    public Direction: string;
    public OffsetFlag: string;
    public Price: Decimal;
    public Volume: Decimal;
    public VolumeDisplay: Decimal;
    public VolumeMode: string;
    public Cost: Decimal;
    public OrderType: string;
    public GTDTime: number;
    public MinVolume: Decimal;
    public BusinessType: string;
    public BusinessValue: string;
    public CloseOrderID: string;
    public IsCrossMargin: number;
    public Remark: string;
    public OrderID: string;
    public CopyMemberID: string;
    public CopyOrderID: string;
    public Leverage: Decimal;
    public CopyProfitRate: Decimal;
    public APPID: string;
    public PositionID: string;
    public TriggerPrice: Decimal;
    public Reserve: Decimal;
    public OrderStatus: string;
    public DeriveSource: string;
    public DeriveDetail: string;
    public VolumeTraded: Decimal;
    public VolumeRemain: Decimal;
    public VolumeCancled: Decimal;
    public InsertTime: number;
    public UpdateTime: number;
    public Priority: number;
    public TimeSortNo: number;
    public FrontNo: number;
    public PriceCurrency: string;
    public FeeCurrency: string;
    public ClearCurrency: string;
    public FrozenMoney: Decimal;
    public FrozenFee: Decimal;
    public FrozenMargin: Decimal;
    public Fee: Decimal;
    public CloseProfit: Decimal;
    public Turnover: Decimal;
    public RelatedOrderID: string;
    public BusinessResult: string;
    public BusinessNo: number;
    public Tradable: number;
    public SettlementGroup: string;
    public PosiDirection: string;
    public TradePrice: Decimal;
    public OpenPrice: Decimal;
    public TriggerOrderID: string;
    public SLTriggerPrice: Decimal;
    public TPTriggerPrice: Decimal;
    public CopyProfit: Decimal;
    public Position: Decimal;
    public UserID: string;
    public LastPriceByInsert: Decimal;
    public BidPrice1ByInsert: Decimal;
    public AskPrice1ByInsert: Decimal;
    public Available: Decimal;
    public readonly CreateTime: Date;
    public readonly DBTime: Date;
}

FinishOrderModel.init(
    {
        MemberID: {
            type: DataTypes.CHAR(42),
            allowNull: true,
            comment: "成员代码"
        },
        TradeUnitID: {
            type: DataTypes.CHAR(20),
            allowNull: true,
            comment: "交易单元代码"
        },
        AccountID: {
            type: DataTypes.CHAR(44),
            allowNull: true,
            comment: "资金账号"
        },
        LocalID: {
            type: DataTypes.CHAR(20),
            allowNull: true,
            comment: "报单本地标识"
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
        OrderPriceType: {
            type: DataTypes.CHAR(1),
            allowNull: true,
            comment: "报单价格类型"
        },
        Direction: {
            type: DataTypes.CHAR(1),
            allowNull: true,
            comment: "买卖方向"
        },
        OffsetFlag: {
            type: DataTypes.CHAR(1),
            allowNull: true,
            comment: "开平标志"
        },
        Price: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "报单价格",
            get() {
                const value = this.getDataValue('Price');
                if (!value) return null;
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
        Volume: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "数量",
            get() {
                const value = this.getDataValue('Volume');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Volume', null);
                } else {
                    this.setDataValue('Volume', value.toFixed());
                }
            }
        },
        VolumeDisplay: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "显示数量",
            get() {
                const value = this.getDataValue('VolumeDisplay');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('VolumeDisplay', null);
                } else {
                    this.setDataValue('VolumeDisplay', value.toFixed());
                }
            }
        },
        VolumeMode: {
            type: DataTypes.CHAR(1),
            allowNull: true,
            comment: "数量取值方式"
        },
        Cost: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "委托额",
            get() {
                const value = this.getDataValue('Cost');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Cost', null);
                } else {
                    this.setDataValue('Cost', value.toFixed());
                }
            }
        },
        OrderType: {
            type: DataTypes.CHAR(1),
            allowNull: true,
            comment: "订单类型"
        },
        GTDTime: {
            type: DataTypes.DECIMAL(20, 0),
            allowNull: true,
            comment: "GTD时间",
            get() {
                const value = this.getDataValue('GTDTime');
                return value ? Number(value) : null;
            }
        },
        MinVolume: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "最小成交量",
            get() {
                const value = this.getDataValue('MinVolume');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('MinVolume', null);
                } else {
                    this.setDataValue('MinVolume', value.toFixed());
                }
            }
        },
        BusinessType: {
            type: DataTypes.CHAR(1),
            allowNull: true,
            comment: "业务类别"
        },
        BusinessValue: {
            type: DataTypes.CHAR(32),
            allowNull: true,
            comment: "业务值"
        },
        CloseOrderID: {
            type: DataTypes.CHAR(20),
            allowNull: true,
            comment: "平仓指定开仓的订单号"
        },
        IsCrossMargin: {
            type: DataTypes.DECIMAL(1, 0),
            allowNull: true,
            comment: "是否全仓",
            get() {
                const value = this.getDataValue('IsCrossMargin');
                return value ? Number(value) : null;
            }
        },
        Remark: {
            type: DataTypes.CHAR(32),
            allowNull: true,
            comment: "备注"
        },
        OrderID: {
            type: DataTypes.CHAR(20),
            primaryKey: true,
            comment: "报单系统唯一代码"
        },
        CopyMemberID: {
            type: DataTypes.CHAR(42),
            allowNull: true,
            comment: "带单员代码"
        },
        CopyOrderID: {
            type: DataTypes.CHAR(20),
            allowNull: true,
            comment: "带单员报单号"
        },
        Leverage: {
            type: DataTypes.DECIMAL(20, 8),
            allowNull: true,
            comment: "委托单杠杆倍数",
            get() {
                const value = this.getDataValue('Leverage');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Leverage', null);
                } else {
                    this.setDataValue('Leverage', value.toFixed());
                }
            }
        },
        CopyProfitRate: {
            type: DataTypes.DECIMAL(26, 10),
            allowNull: true,
            comment: "带单分配比例",
            get() {
                const value = this.getDataValue('CopyProfitRate');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('CopyProfitRate', null);
                } else {
                    this.setDataValue('CopyProfitRate', value.toFixed());
                }
            }
        },
        APPID: {
            type: DataTypes.CHAR(8),
            allowNull: true,
            comment: "应用编号"
        },
        PositionID: {
            type: DataTypes.CHAR(42),
            allowNull: true,
            comment: "持仓代码"
        },
        TriggerPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "触发价",
            get() {
                const value = this.getDataValue('TriggerPrice');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('TriggerPrice', null);
                } else {
                    this.setDataValue('TriggerPrice', value.toFixed());
                }
            }
        },
        Reserve: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "保留资金",
            get() {
                const value = this.getDataValue('Reserve');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Reserve', null);
                } else {
                    this.setDataValue('Reserve', value.toFixed());
                }
            }
        },
        OrderStatus: {
            type: DataTypes.CHAR(1),
            allowNull: true,
            comment: "报单状态"
        },
        DeriveSource: {
            type: DataTypes.CHAR(1),
            allowNull: true,
            comment: "衍生来源"
        },
        DeriveDetail: {
            type: DataTypes.CHAR(20),
            allowNull: true,
            comment: "衍生明细"
        },
        VolumeTraded: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "成交数量",
            get() {
                const value = this.getDataValue('VolumeTraded');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('VolumeTraded', null);
                } else {
                    this.setDataValue('VolumeTraded', value.toFixed());
                }
            }
        },
        VolumeRemain: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "剩余数量",
            get() {
                const value = this.getDataValue('VolumeRemain');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('VolumeRemain', null);
                } else {
                    this.setDataValue('VolumeRemain', value.toFixed());
                }
            }
        },
        VolumeCancled: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "已经撤单数量",
            get() {
                const value = this.getDataValue('VolumeCancled');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('VolumeCancled', null);
                } else {
                    this.setDataValue('VolumeCancled', value.toFixed());
                }
            }
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
        UpdateTime: {
            type: DataTypes.DECIMAL(20, 0),
            allowNull: true,
            comment: "更新时间",
            get() {
                const value = this.getDataValue('UpdateTime');
                return value ? Number(value) : null;
            }
        },
        Priority: {
            type: DataTypes.DECIMAL(10, 0),
            allowNull: true,
            comment: "优先权",
            get() {
                const value = this.getDataValue('Priority');
                return value ? Number(value) : null;
            }
        },
        TimeSortNo: {
            type: DataTypes.DECIMAL(16, 0),
            allowNull: true,
            comment: "按时间排队的序号",
            get() {
                const value = this.getDataValue('TimeSortNo');
                return value ? Number(value) : null;
            }
        },
        FrontNo: {
            type: DataTypes.DECIMAL(10, 0),
            allowNull: true,
            comment: "前置编号",
            get() {
                const value = this.getDataValue('FrontNo');
                return value ? Number(value) : null;
            }
        },
        PriceCurrency: {
            type: DataTypes.CHAR(12),
            allowNull: true,
            comment: "计价币种"
        },
        FeeCurrency: {
            type: DataTypes.CHAR(12),
            allowNull: true,
            comment: "手续费币种"
        },
        ClearCurrency: {
            type: DataTypes.CHAR(12),
            allowNull: true,
            comment: "清算币种"
        },
        FrozenMoney: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "冻结资金",
            get() {
                const value = this.getDataValue('FrozenMoney');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('FrozenMoney', null);
                } else {
                    this.setDataValue('FrozenMoney', value.toFixed());
                }
            }
        },
        FrozenFee: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "冻结手续费",
            get() {
                const value = this.getDataValue('FrozenFee');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('FrozenFee', null);
                } else {
                    this.setDataValue('FrozenFee', value.toFixed());
                }
            }
        },
        FrozenMargin: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "冻结保证金",
            get() {
                const value = this.getDataValue('FrozenMargin');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('FrozenMargin', null);
                } else {
                    this.setDataValue('FrozenMargin', value.toFixed());
                }
            }
        },
        Fee: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "手续费",
            get() {
                const value = this.getDataValue('Fee');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Fee', null);
                } else {
                    this.setDataValue('Fee', value.toFixed());
                }
            }
        },
        CloseProfit: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "平仓盈亏",
            get() {
                const value = this.getDataValue('CloseProfit');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('CloseProfit', null);
                } else {
                    this.setDataValue('CloseProfit', value.toFixed());
                }
            }
        },
        Turnover: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "成交金额",
            get() {
                const value = this.getDataValue('Turnover');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Turnover', null);
                } else {
                    this.setDataValue('Turnover', value.toFixed());
                }
            }
        },
        RelatedOrderID: {
            type: DataTypes.CHAR(20),
            allowNull: true,
            comment: "关联报单号"
        },
        BusinessResult: {
            type: DataTypes.CHAR(32),
            allowNull: true,
            comment: "业务执行结果"
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
        Tradable: {
            type: DataTypes.DECIMAL(1, 0),
            allowNull: true,
            comment: "是否可以参加交易",
            get() {
                const value = this.getDataValue('Tradable');
                return value ? Number(value) : null;
            }
        },
        SettlementGroup: {
            type: DataTypes.CHAR(8),
            allowNull: true,
            comment: "结算组编号"
        },
        PosiDirection: {
            type: DataTypes.CHAR(1),
            allowNull: true,
            comment: "持仓多空方向"
        },
        TradePrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "成交均价",
            get() {
                const value = this.getDataValue('TradePrice');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('TradePrice', null);
                } else {
                    this.setDataValue('TradePrice', value.toFixed());
                }
            }
        },
        OpenPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "平仓成交时的开仓均价",
            get() {
                const value = this.getDataValue('OpenPrice');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('OpenPrice', null);
                } else {
                    this.setDataValue('OpenPrice', value.toFixed());
                }
            }
        },
        TriggerOrderID: {
            type: DataTypes.CHAR(20),
            allowNull: true,
            comment: "触发报单号"
        },
        SLTriggerPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "止损触发价",
            get() {
                const value = this.getDataValue('SLTriggerPrice');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('SLTriggerPrice', null);
                } else {
                    this.setDataValue('SLTriggerPrice', value.toFixed());
                }
            }
        },
        TPTriggerPrice: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "止盈触发价",
            get() {
                const value = this.getDataValue('TPTriggerPrice');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('TPTriggerPrice', null);
                } else {
                    this.setDataValue('TPTriggerPrice', value.toFixed());
                }
            }
        },
        CopyProfit: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "带单盈利分配",
            get() {
                const value = this.getDataValue('CopyProfit');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('CopyProfit', null);
                } else {
                    this.setDataValue('CopyProfit', value.toFixed());
                }
            }
        },
        Position: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "成交时的持仓量",
            get() {
                const value = this.getDataValue('Position');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Position', null);
                } else {
                    this.setDataValue('Position', value.toFixed());
                }
            }
        },
        UserID: {
            type: DataTypes.CHAR(42),
            allowNull: true,
            comment: "交易用户代码"
        },
        LastPriceByInsert: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "插入时的最新价",
            get() {
                const value = this.getDataValue('LastPriceByInsert');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('LastPriceByInsert', null);
                } else {
                    this.setDataValue('LastPriceByInsert', value.toFixed());
                }
            }
        },
        BidPrice1ByInsert: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "插入时的买一价",
            get() {
                const value = this.getDataValue('BidPrice1ByInsert');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('BidPrice1ByInsert', null);
                } else {
                    this.setDataValue('BidPrice1ByInsert', value.toFixed());
                }
            }
        },
        AskPrice1ByInsert: {
            type: DataTypes.DECIMAL(28, 15),
            allowNull: true,
            comment: "插入时的卖一价",
            get() {
                const value = this.getDataValue('AskPrice1ByInsert');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('AskPrice1ByInsert', null);
                } else {
                    this.setDataValue('AskPrice1ByInsert', value.toFixed());
                }
            }
        },
        Available: {
            type: DataTypes.DECIMAL(38, 10),
            allowNull: true,
            comment: "可用资金",
            get() {
                const value = this.getDataValue('Available');
                if (!value) return null;
                return new Decimal(value);
            },
            set(value: Decimal) {
                if (!value) {
                    this.setDataValue('Available', null);
                } else {
                    this.setDataValue('Available', value.toFixed());
                }
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
        modelName: 'FinishOrderModel',
        tableName: 't_D_FinishOrder',
        timestamps: false,
    }
);

export default FinishOrderModel;
