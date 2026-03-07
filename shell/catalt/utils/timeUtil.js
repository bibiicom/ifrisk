class TimeUtil {
    static async sleep(time = 1000) {
        await new Promise(resolve => setTimeout(resolve, time));
    }
}

module.exports = TimeUtil;