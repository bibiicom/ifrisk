import { IEventListener } from '../event/EventBus';
import { CardOfflineEventData } from '../event/EventData';
import AnnouncementModel from '../../models/AnnouncementModel';
import { DateUtil } from '../../tool/DateUtil';

/**
 * 卡片下线公告生成监听器
 */
export class CardOfflineListener implements IEventListener {
    /**
     * 事件处理方法
     * @param eventData 事件数据
     * @returns 是否取消后续事件处理
     */
    public async handle(eventData: CardOfflineEventData): Promise<boolean> {
        try {
            const title = `卡片即将下线通知：${eventData.name} (${eventData.symbol})`;

            const content = this.generateAnnouncementContent(eventData);

            await AnnouncementModel.create({
                Title: title,
                Content: content,
                Type: 2,
                SubType: 201,
                Status: 2,
                Priority: 2,
                FileName: '',
                DocUrl: ''
            });

            console.log(`Announcement created for card offline: ${eventData.name} (${eventData.symbol})`);
            return false;
        } catch (error) {
            console.error(`Failed to create announcement for card offline ${eventData.name}:`, error);
            return false;
        }
    }

    /**
     * 生成公告内容
     * @param eventData 事件数据
     * @returns 公告内容（markdown格式）
     */
    private generateAnnouncementContent(eventData: CardOfflineEventData): string {
        return `尊敬的用户：

${eventData.name} (${eventData.symbol}) 卡片即将下线，具体时间安排如下：

下线时间：${DateUtil.formatDateUTC(eventData.closeTime)}

注意事项
1. 如有任何疑问，请及时联系客服
2. 感谢您对本卡片的支持与关注

我们将持续为您提供更优秀的产品和更优质的服务！

---

${DateUtil.formatDateUTC(new Date())}`;
    }

}
