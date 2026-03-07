/**
 * 事件类型枚举
 */
export enum EventType {
    /** 卡片上架事件 */
    CARD_LAUNCHED = "CARD_LAUNCHED",
    /** 卡片交易事件 */
    CARD_TRANSACTED = "CARD_TRANSACTED",
    /** 卡片下线事件 */
    CARD_OFFLINE = "CARD_OFFLINE",
}