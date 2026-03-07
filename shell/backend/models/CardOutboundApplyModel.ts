import {DataTypes, Model} from 'sequelize';
import {sequelize} from '../config/database';

class CardOutboundApplyModel extends Model {
    public Id: number;
    public CardID: number;
    public PostalCode: string;
    public Name: string;
    public Address: string;
    public Phone: string;
    public Email: string;
    public AccountID: string;
    public Status: number; // 状态 0-待处理 1-已通过 2-已拒绝
    public readonly createdAt: Date;
    public readonly updatedAt: Date;
    public readonly deletedAt: Date; // 软删除时间，由sequelize自动管理
}

CardOutboundApplyModel.init({
        Id: {
            type: DataTypes.BIGINT,
            primaryKey: true,
            autoIncrement: true,
            allowNull: false,
            comment: "主键"
        },
        CardID: {
            type: DataTypes.BIGINT,
            allowNull: false,
            comment: "卡片ID"
        },
        PostalCode: {
            type: DataTypes.STRING,
            allowNull: true,
            comment: "邮编"
        },
        Name: {
            type: DataTypes.STRING,
            allowNull: true,
            comment: "姓名"
        },
        Address: {
            type: DataTypes.STRING,
            allowNull: true,
            comment: "地址"
        },
        Phone: {
            type: DataTypes.STRING,
            allowNull: true,
            comment: "手机号"
        },
        Email: {
            type: DataTypes.STRING,
            allowNull: true,
            comment: "邮箱"
        },
        AccountID: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "用户地址"
        },
        Status: {
            type: DataTypes.TINYINT,
            allowNull: true,
            defaultValue: 0,
            comment: "状态 0-待处理 1-已通过 2-已拒绝"
        }
    },
    {
        sequelize,
        indexes: [
            {
                fields: ['AccountID'],
                name: 'idx_outbound_accountID',
            },
            {
                fields: ['Status'],
                name: 'idx_outbound_status',
            }
        ],
        modelName: 'CardOutboundApplyModel',
        tableName: 't_D_CardOutboundApply',
        timestamps: true,
        paranoid: true,
        comment: "卡片出库申请记录表"
    }
);

export default CardOutboundApplyModel;
