import * as https from 'https';
import * as http from 'http';
import {URL} from 'url';

export class HttpRequest {

    static async get(url: string, options = {}) {
        return this.request('GET', url, null, options);
    }

    static async post(url: string, data: any, options = {}) {
        return this.request('POST', url, data, options);
    }

    static async request(method: string, url: string, data: any, options: any = {}): Promise<any> {
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
            const requestOptions = {...defaultOptions, ...options};

            const req = lib.request(requestOptions, (res) => {
                let responseData = '';

                res.on('data', (chunk) => {
                    responseData += chunk;
                });

                res.on('end', () => {
                    // 尝试解析JSON
                    try {
                        responseData = JSON.parse(responseData);
                    } catch (e) {
                        // 如果不是JSON，保持原样
                    }
                    resolve(responseData);
                });
            });

            req.on('error', (error) => {
                console.error('HTTP请求错误: ' + error.message);
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

}
