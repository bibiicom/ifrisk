class RandomUtil {

    // 生成 [min, max] 范围内的随机整数（包含 max）
    static randomIntBetween(min, max) {
        return Math.floor(Math.random() * (max - min + 1)) + min;
    }

    // 随机获取数组中的一条数据
    static randomElement(array) {
        if (!Array.isArray(array) || array.length === 0) {
            return undefined;
        }
        const randomIndex = Math.floor(Math.random() * array.length);
        return array[randomIndex];
    }

    // 随机获取数组中的n条数据 不重复
    static randomElements(array, count = 2) {
        if (!Array.isArray(array) || array.length === 0 || count <= 0) {
            return [];
        }
        const arrayCopy = [...array];
        const result = [];
        const maxCount = Math.min(count, arrayCopy.length);
        
        for (let i = 0; i < maxCount; i++) {
            const randomIndex = Math.floor(Math.random() * arrayCopy.length);
            result.push(arrayCopy[randomIndex]);
            arrayCopy.splice(randomIndex, 1);
        }
        return result;
    }

}

module.exports = RandomUtil;