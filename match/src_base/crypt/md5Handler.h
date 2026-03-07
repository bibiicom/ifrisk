/////////////////////////////////////////////////////////////////////////
///	定义了md5算法的接口
/////////////////////////////////////////////////////////////////////////

#ifndef MD5_H
#define MD5_H

#include "platform.h"

///md5摘要值得长度
#define MD5_DIGEST_LEN	16

typedef unsigned char byte;
typedef unsigned int uint32;

/////////////////////////////////////////////////////////////////////////
///CMD5Handler是一个进行MD5算法计算的处理器
/////////////////////////////////////////////////////////////////////////
class CMD5Handler
{
public:
	///构造方法，没有设置初始值
	CMD5Handler(void);
	
	///构造方法，有设置初始值
	///@param	input	输入的字符串
	///@param	length	字符串的长度，如果为-1表示字符串以'\0'结尾
	CMD5Handler(const char *input, int length=-1);
	
	///加入一个字符串
	///@param	input	输入的字符串
	///@param	length	字符串的长度，如果为-1表示字符串以'\0'结尾
	void update(const char *input, int length=-1);

	///获取摘要值
	///@return	得到的摘要值，长度一定为MD5_DIGEST_LEN(16)个字节
	const char* digest(void);

	///清空值
	void reset(void);

private:
	///完成最终步骤，产生摘要值
	void final(void);

	///MD5算法使用的内部方法
	void transform(const char block[64]);
	void encode(const uint32 *input, byte *output, size_t length);
	void decode(const byte *input, uint32 *output, size_t length);

	///禁止复制而定义的方法
	///@param	v	要复制的源
	CMD5Handler(const CMD5Handler& v)
	{
	}

	///禁止等于而定义的方法
	///@param	v	要复制的源
	CMD5Handler& operator=(const CMD5Handler& v)
	{
	}

private:
	uint32 _state[4];	// state (ABCD)
	uint32 _count[2];	// number of bits, modulo 2^64 (low-order word first)
	char _buffer[64];	// input buffer
	char _digest[16];	// message digest
	bool _finished;		// calculate finished?
};

#endif
