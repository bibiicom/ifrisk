import {DataTypes, Model} from 'sequelize';
import {sequelize} from '../config/database';

class AnnouncementModel extends Model {
    public Id: number;
    public Title: string;
    public Content: string;
    public FileName: string;
    public DocUrl: string;
    public Type: number;
    public SubType: number;
    public Status: number;
    public Priority: number;
    public readonly createdAt!: Date;
    public readonly updatedAt!: Date;
    public readonly deletedAt: Date;
}

AnnouncementModel.init({
    Id: {
        type: DataTypes.BIGINT,
        primaryKey: true,
        autoIncrement: true,
        allowNull: false,
        comment: "主键"
    },
    Title: {
        type: DataTypes.STRING,
        allowNull: false,
        comment: "标题"
    },
    Content: {
        type: DataTypes.TEXT,
        allowNull: true,
        comment: "内容",
        defaultValue: ""
    },
    FileName: {
        type: DataTypes.STRING,
        allowNull: true,
        comment: "文件名",
        defaultValue: ""
    },
    DocUrl: {
        type: DataTypes.STRING,
        allowNull: true,
        comment: "原始链接",
        defaultValue: ""
    },
    Type: {
        type: DataTypes.INTEGER,
        allowNull: false,
        comment: "类型 1：新闻 2：公告"
    },
    SubType: {
        type: DataTypes.INTEGER,
        allowNull: true,
        defaultValue: 0,
        comment: "二级分类（100:行业热点, 200:新卡上线, 201:卡片下线）"
    },
    Status: {
        type: DataTypes.INTEGER,
        allowNull: true,
        defaultValue: 1,
        comment: "状态 1：草稿 2：已发布"
    },
    Priority: {
        type: DataTypes.INTEGER,
        allowNull: true,
        defaultValue: 1,
        comment: "优先级"
    }
}, {
    sequelize,
    indexes: [
        {
            fields: ['Status'],
            name: 'idx_status'
        },
        {
            fields: ['Type'],
            name: 'idx_type'
        },
        {
            fields: ['SubType'],
            name: 'idx_subtype'
        },
        {
            fields: ['Priority', 'createdAt'],
            name: 'idx_priority_time'
        }
    ],
    modelName: 'AnnouncementModel',
    tableName: 't_D_Announcement',
    timestamps: true,
    paranoid: true
});

export default AnnouncementModel;