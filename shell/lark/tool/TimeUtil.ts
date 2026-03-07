export class TimeUtil {

    /**
     * 格式化日期时间为 YYYY-MM-DD HH:MM:SS 格式
     * @param date 日期对象或时间戳
     * @returns 格式化后的日期时间字符串
     */
    static format(date: Date | number | string): string {
        const d = new Date(date);
        const year = d.getFullYear();
        const month = String(d.getMonth() + 1).padStart(2, '0');
        const day = String(d.getDate()).padStart(2, '0');
        const hours = String(d.getHours()).padStart(2, '0');
        const minutes = String(d.getMinutes()).padStart(2, '0');
        const seconds = String(d.getSeconds()).padStart(2, '0');
        
        return `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
    }

    /**
     * 异步 sleep 函数
     * @param time 等待时间，单位毫秒，默认 1000 毫秒
     */
    static async sleep(time = 1000) {
        await new Promise(resolve => setTimeout(resolve, time));
    }

}
