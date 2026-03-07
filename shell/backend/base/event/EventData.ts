/**
 * 卡片上架事件数据
 */
export interface CardLaunchedEventData {
    /** 卡片名称 */
    name: string;
    /** 卡片简称 */
    symbol: string;
    /** 开放交易时间 */
    openTime: Date;
    /** 交易结束时间 */
    closeTime: Date;
    /** 卡片描述 */
    summary: string;
}
/**
 * 卡片交易事件数据
 */
export interface CardTransactedEventData {
    /** 卡片名称 */
    name: string;
    /** 卡片简称 */
    symbol: string;
    /** 交易时间 */
    tradeTime: Date;
    /** 卡片描述 */
    summary: string;
}
/**
 * 卡片下线事件数据
 */
export interface CardOfflineEventData {
    /** 卡片名称 */
    name: string;
    /** 卡片简称 */
    symbol: string;
    /** 下线时间 */
    closeTime: Date;
}