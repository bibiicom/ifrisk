/**
 * 时间工具类
 */
export class DateUtil {
    
    /**
     * 格式化日期为UTC时间字符串
     * @param date 日期对象
     * @returns 格式化后的UTC时间字符串，格式：2026-01-16 16:03:00 (UTC)
     */
    public static formatDateUTC(date: Date): string {
        const year = date.getUTCFullYear();
        const month = String(date.getUTCMonth() + 1).padStart(2, '0');
        const day = String(date.getUTCDate()).padStart(2, '0');
        const hour = String(date.getUTCHours()).padStart(2, '0');
        const minute = String(date.getUTCMinutes()).padStart(2, '0');
        const second = String(date.getUTCSeconds()).padStart(2, '0');
        
        return `${year}-${month}-${day} ${hour}:${minute}:${second} (UTC)`;
    }
    
    /**
     * 格式化日期为本地时间字符串
     * @param date 日期对象
     * @param timeZone 时区，默认为'Asia/Shanghai'
     * @returns 格式化后的本地时间字符串
     */
    public static formatDateLocal(date: Date, timeZone: string = 'Asia/Shanghai'): string {
        return date.toLocaleString('zh-CN', {
            timeZone: timeZone,
            year: 'numeric',
            month: '2-digit',
            day: '2-digit',
            hour: '2-digit',
            minute: '2-digit',
            second: '2-digit'
        });
    }
}