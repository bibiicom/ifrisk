#ifndef APIKEYUTIL_H
#define APIKEYUTIL_H

#include "platform.h"

/**生成签名
*@param	SecretKey	用于签名的key
*@param	RequestTimestamp	请求时间戳
*@param	RequestMethod	请求方法（GET/POST）
*@param	RequestPath	请求URL路径
*@param	RequestBody	请求体（GET请求可以为空字符串）
*@return	签名后的字符串
*/
string GenerateSign(string SecretKey, string RequestTimestamp, string RequestMethod, string RequestPath, string RequestBody);

/**HMACSHA加密
*@param	Key	用于加密的key
*@param	Input	需要加密的数据
*@return	加密后的字符串
*/
string HmacShaEncode(const char *Key, const char *Input);

string HmacShaEncode2(const char* Key, const char* Input);

#endif //APIKEYUTIL_H