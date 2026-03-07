import {DataTypes, Model} from 'sequelize';
import {sequelize} from '../config/database';

class CardStorageApplyModel extends Model {
    public Id: number; // 主键
    public AccountID: string; // 用户地址
    public Email: string; // 邮箱
    public Status: number; // 状态 0-待处理 1-已通过 2-已拒绝
    public CardID: number; // 卡片ID
    public readonly createdAt!: Date; // 创建时间，由sequelize自动管理
    public readonly updatedAt!: Date; // 更新时间，由sequelize自动管理
    public readonly deletedAt: Date; // 软删除时间，由sequelize自动管理
}

CardStorageApplyModel.init({
        Id: {
            type: DataTypes.BIGINT,
            primaryKey: true,
            autoIncrement: true,
            allowNull: false,
            comment: "主键"
        },
        AccountID: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "用户地址"
        },
        Email: {
            type: DataTypes.STRING,
            allowNull: false,
            comment: "邮箱"
        },
        Status: {
            type: DataTypes.TINYINT,
            allowNull: true,
            defaultValue: 0,
            comment: "状态 0-待处理 1-已通过 2-已拒绝"
        },
        CardID: {
            type: DataTypes.BIGINT,
            allowNull: true,
            comment: "卡片ID"
        }
    },
    {
        sequelize,
        indexes: [
            {
                fields: ['AccountID'],
                name: 'idx_accountID',
            },
            {
                fields: ['Status'],
                name: 'idx_status',
            }
        ],
        modelName: 'CardStorageApplyModel',
        tableName: 't_D_CardStorageApply',
        timestamps: true, // 启用sequelize的timestamps功能，自动管理createdAt和updatedAt
        paranoid: true, // 启用软删除
        comment: "卡片入库申请记录表"
    }
);

export default CardStorageApplyModel;