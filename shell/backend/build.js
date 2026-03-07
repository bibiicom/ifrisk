const fs = require("node:fs");
console.log('copy assets');

// 复制 config 中 json5后缀的文件 到 dist 目录
const configDir = './config';
const distDir = './dist/config';

// 读取 config 目录下的所有文件 同步
const files = fs.readdirSync(configDir);
// 遍历所有文件
files.forEach(file => {
    if (file.endsWith('.json5')) {
        const srcPath = `${configDir}/${file}`;
        const destPath = `${distDir}/${file}`;

        try {
            fs.copyFileSync(srcPath, destPath);
            console.log(`复制 ${file}`);
        } catch (err) {
            console.error(`复制 ${file} 失败:`, err);
        }
    }
});
console.log('down');



