import {IEventListener} from '../event/EventBus';
import {CardTransactedEventData} from '../event/EventData';
import AnnouncementModel from '../../models/AnnouncementModel';
import {DateUtil} from '../../tool/DateUtil';

/**
 * 卡片交易开启公告生成监听器
 */
export class CardTransactedListener implements IEventListener {
    /**
     * 事件处理方法
     * @param eventData 事件数据
     * @returns 是否取消后续事件处理
     */
    public async handle(eventData: CardTransactedEventData): Promise<boolean> {
        try {
            // 生成公告标题
            const title = `新卡片交易开启通知：${eventData.name} (${eventData.symbol})`;

            // 生成公告内容
            const content = this.generateAnnouncementContent(eventData);

            // 创建公告
            await AnnouncementModel.create({
                Title: title,
                Content: content,
                Type: 2, // 公告类型：公告
                SubType: 200, // 二级分类：新卡上线
                Status: 2, // 状态：已发布
                Priority: 2, // 优先级：中等
                FileName: '',
                DocUrl: ''
            });

            console.log(`Announcement created for card ${eventData.name}(${eventData.symbol})`);
            return false; // 不取消后续事件处理
        } catch (error) {
            console.error(`Failed to create announcement for card ${eventData.name}:`, error);
            return false; // 不取消后续事件处理
        }
    }

    /**
     * 生成公告内容
     * @param eventData 事件数据
     * @returns 公告内容（markdown格式）
     */
    private generateAnnouncementContent(eventData: CardTransactedEventData): string {
        // 构建公告内容
        return `尊敬的用户：

我们即将开启 ${eventData.name} (${eventData.symbol}) 卡片的交易，具体时间安排如下：

交易开始时间：${DateUtil.formatDateUTC(eventData.tradeTime)}

项目介绍
${eventData.name} (${eventData.symbol}) 是一款全新的数字卡片，${eventData.summary || '具有独特的收藏价值和交易潜力'}。

注意事项
1. 请确保您的账户有足够的余额进行交易
2. 请仔细阅读卡片详情，了解相关风险
3. 交易过程中如有任何问题，请及时联系客服

我们将持续为您提供更优秀的产品和更优质的服务！

---

${DateUtil.formatDateUTC(new Date())}`;
    }

}