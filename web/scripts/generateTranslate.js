'use strict';

const path = require('path')
const fs = require('fs');

const appDirectory = fs.realpathSync(process.cwd());

const originalI18nDirectory = path.join(appDirectory, 'src', 'locales')

const genI18nDirectory = path.join(appDirectory, 'i18n', 'translate')

const baseKeyValues = require(path.join(originalI18nDirectory, 'zh-CN.json'));

function gen (locales) {
  // fs.emptyDirSync(genI18nDirectory);
  locales.forEach(locale => {
    const compareKeyValues = require(path.join(originalI18nDirectory, `${locale}.json`));
    if (compareKeyValues) {
      const translate = {}
      for (const key in baseKeyValues) {
        if (!(key in compareKeyValues)) {
          translate[key] = baseKeyValues[key];
        }
      }
      const file = path.join(genI18nDirectory, `${locale}.json`);
      fs.writeFileSync(file, JSON.stringify(translate));
    }
  })
}

gen(['en-US', 'ja-JP', 'ko-KR', 'zh-HK']);