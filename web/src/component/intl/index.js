import zhCN from './zh-CN.json'

export const ZH_CN = 'zh-CN'
export const ZH_HK = 'zh-HK'
export const EN_US = 'en-US'

const intl = {
  language: ZH_CN,
  locals: {
    [ZH_CN]: zhCN
  },
  setLanguage: function (language) {
    this.language = language || ZH_CN
  },
  load: function (lang, local = {}) {
    if (lang) {
      if (lang in this.locals) {
        this.locals[lang] = { ...this.locals[lang], ...local }
      } else {
        this.locals[lang] = local
      }
    }
  },
  get: function (key, lang = this.language) {
    const local = this.locals[lang]
    if (local) {
      return local[key] || '--'
    }
    return '--'
  }
}

export default intl
