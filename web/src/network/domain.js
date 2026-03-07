
import config from "./config";
import { isIp } from "../utils/ip";
const env = process.env.NODE_ENV;
/**
 * 网络域名配置
 * 所有接口跟随地址栏域名
 * 开发环境通过 setupProxy.js 代理转发避免跨域
 */


/**
 * 检查域名是否合法
 * @param domain 域名
 */
function checkDomain(domain = "") {
  if (
    !domain.startsWith("http") &&
    !domain.startsWith("https") &&
    !domain.startsWith("ws") &&
    !domain.startsWith("ws")
  ) {
    throw new Error(
      "Domain is invalid, please edit file src/network/config.js."
    );
  }
}

/**
 * 判断当前是否为 HTTPS
 * @returns {boolean}
 */
function isHttps() {
  return window.location.protocol.startsWith("https");
}

/**
 * 获取 http url
 * @returns {string}
 */
function getHttpUrl() {
  return `${window.location.origin}`;
}

/**
 * 获取文章 url
 * @returns {string}
 */
function getArticleUrl() {
  return `${window.location.origin}/`;
}

/**
 * 获取文档 url
 * @returns {string}
 */
function getDocsUrl() {
  return `${window.location.origin}`;
}

// /**
//  * 获取 ws url
//  * @returns {string}
//  */
// function getWsUrl() {
//   const baseUrl = isHttps()
//     ? `wss://${window.location.host}`
//     : `ws://${window.location.host}`;
//   return `${baseUrl}/wspublic`;
// }

/**
 * 获取ws url
 * @returns {string}
 */
function getWsUrl(suffix) {
  let wsUrl = "";
  if (env !== "production") {
    // wsUrl = `${config.wsDomain}/${suffix}/ws`
    wsUrl = `${config.wsDomain}/wspublic`;
    checkDomain(wsUrl);
  } else {
    let baseUrl;
    if (isHttps()) {
      // baseUrl = process.env.OUTFOG_TRADE_WS_URL
      baseUrl = `wss://${window.location.host}`;
    } else {
      baseUrl = `ws://${window.location.host}`;
    }
    wsUrl = `${baseUrl}/wspublic`;
  }
  return wsUrl;
  // if (isIp())
  //   return `ws://${window.location.host}/wspublic`
  // return `${config.wsDomain}/wspublic`
}



export { getHttpUrl, getWsUrl, getArticleUrl, getDocsUrl };
