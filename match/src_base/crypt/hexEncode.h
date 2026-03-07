/////////////////////////////////////////////////////////////////////////
///hexEncode.h
///定义了16进制编码器的接口
/////////////////////////////////////////////////////////////////////////

#ifndef HEX_ENCODE_H
#define HEX_ENCODE_H

#include "platform.h"

/////////////////////////////////////////////////////////////////////////
///CHexEncoder是一个进行16进制编码的编码器
/////////////////////////////////////////////////////////////////////////
class CHexEncoder
{
public:
	///构造方法
	CHexEncoder(void);
	virtual ~CHexEncoder(){};
	
	///编码方法
	///@param	buffer	放置输出结果的空间
	///@param	bufferLen	输出结果空间的长度
	///@param	input	输入的待编码的字符串
	///@param	inputLen	输入字符串的长度，如果为-1表示字符串以'\0'结尾
	///@return	true表示成功，false表示失败，失败的唯一原因是输出结果的空间不够
	virtual bool encode(char *buffer, int bufferLen, const char *input, int inputLen=-1);
	
	///计算编码结果的长度
	///@param	input	输入的待编码的字符串
	///@param	inputLen	输入字符串的长度，如果为-1表示字符串以'\0'结尾
	///@return	结果的长度
	virtual int encodeLen(const char *input, int inputLen=-1);
};

#endif
