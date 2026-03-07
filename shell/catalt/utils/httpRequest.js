/**
 * HTTP请求工具类
 * 提供常用的HTTP请求方法封装
 */

const https = require('https');
const http = require('http');
const { URL } = require('url');
const log4js = require('../config/log4js.config');
const logger = log4js.getLogger();

class HttpRequest {
    /**
     * 发送GET请求
     * @param {string} url - 请求地址
     * @param {Object} options - 请求选项
     * @returns {Promise<Object>} 响应数据
     */
    static async get(url, options = {}) {
        return this.request('GET', url, null, options);
    }

    /**
     * 发送POST请求
     * @param {string} url - 请求地址
     * @param {Object|string} data - 请求数据
     * @param {Object} options - 请求选项
     * @returns {Promise<Object>} 响应数据
     */
    static async post(url, data, options = {}) {
        return this.request('POST', url, data, options);
    }

    /**
     * 发送PUT请求
     * @param {string} url - 请求地址
     * @param {Object|string} data - 请求数据
     * @param {Object} options - 请求选项
     * @returns {Promise<Object>} 响应数据
     */
    static async put(url, data, options = {}) {
        return this.request('PUT', url, data, options);
    }

    /**
     * 发送DELETE请求
     * @param {string} url - 请求地址
     * @param {Object} options - 请求选项
     * @returns {Promise<Object>} 响应数据
     */
    static async delete(url, options = {}) {
        return this.request('DELETE', url, null, options);
    }

    /**
     * 通用HTTP请求方法
     * @param {string} method - HTTP方法
     * @param {string} url - 请求地址
     * @param {Object|string} data - 请求数据
     * @param {Object} options - 请求选项
     * @returns {Promise<Object>} 响应数据
     */
    static async request(method, url, data, options = {}) {
        return new Promise((resolve, reject) => {
            const urlObj = new URL(url);
            const isHttps = urlObj.protocol === 'https:';
            const lib = isHttps ? https : http;

            // 默认选项
            const defaultOptions = {
                method,
                hostname: urlObj.hostname,
                port: urlObj.port,
                path: urlObj.pathname + urlObj.search,
                headers: {
                    'Content-Type': 'application/json',
                    ...options.headers
                }
            };

            // 如果有数据，设置Content-Length
            if (data) {
                const postData = typeof data === 'string' ? data : JSON.stringify(data);
                defaultOptions.headers['Content-Length'] = Buffer.byteLength(postData);
            }
            delete options.headers;
            // 合并选项
            const requestOptions = { ...defaultOptions, ...options };

            const req = lib.request(requestOptions, (res) => {
                let responseData = '';

                res.on('data', (chunk) => {
                    responseData += chunk;
                });

                res.on('end', () => {
                    try {
                        const result = {
                            statusCode: res.statusCode,
                            headers: res.headers,
                            data: responseData
                        };

                        // 尝试解析JSON
                        try {
                            result.data = JSON.parse(responseData);
                        } catch (e) {
                            // 如果不是JSON，保持原样
                        }

                        resolve(result);
                    } catch (error) {
                        reject(error);
                    }
                });
            });

            req.on('error', (error) => {
                logger.error('HTTP请求错误: ' + error.message);
                reject(error);
            });

            // 写入数据（如果有）
            if (data) {
                const postData = typeof data === 'string' ? data : JSON.stringify(data);
                req.write(postData);
            }

            req.end();
        });
    }

    /**
     * 发送JSON请求
     * @param {string} method - HTTP方法
     * @param {string} url - 请求地址
     * @param {Object} data - JSON数据
     * @param {Object} options - 请求选项
     * @returns {Promise<Object>} 响应数据
     */
    static async jsonRequest(method, url, data, options = {}) {
        const jsonOptions = {
            ...options,
            headers: {
                'Content-Type': 'application/json',
                ...options.headers
            }
        };
        
        return this.request(method, url, data, jsonOptions);
    }
}

module.exports = HttpRequest;