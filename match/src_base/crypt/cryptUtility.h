/////////////////////////////////////////////////////////////////////////
///cryptUtility.h
///	定义了若干常用的加密使用的工具
/////////////////////////////////////////////////////////////////////////

#ifndef CRYPT_UTILITY_H
#define CRYPT_UTILITY_H

#include "hexEncode.h"
#include "md5Handler.h"

///进行MD5摘要算法，并转换为16进制编码方式返回
///@param	result	用于放置返回值的缓冲区，至少有33个字符的空间
///@param	string1	要进行摘要的第1个字符串
///@param	string2	要进行摘要的第2个字符串
///@param	string3	要进行摘要的第3个字符串
///@param	string4	要进行摘要的第4个字符串
void MD5Hex(char *result, const char *string1, const char *string2=NULL, const char *string3=NULL, const char *string4=NULL);

#endif
