#ifndef CPROBELOGGER_H
#define CPROBELOGGER_H

#include "platform.h"

#define MAX_PARAMETER_NAME 100

class CProbeLogger
{
public:
	///构造方法
	///@param	programName	指定的程序名
	///@param	globalSeq	程序全局序列号
	CProbeLogger()
	{
	}
	
	///析构方法
	virtual ~CProbeLogger(void)
	{
	}

	///发送探针消息，值为字符串
	///@param	parameter	参数名
	///@param	value	要发送的消息
	void SendProbeMessage(const char *parameter, const char *value);

	///发送探针消息，值为整数
	///@param	parameter	参数名
	///@param	value	要发送的消息
	void SendProbeMessage(const char *parameter, const int value);

	///发送探针消息，值为浮点数
	///@param	parameter	参数名
	///@param	value	要发送的消息
	void SendProbeMessage(const char *parameter, const double value);

	///发送探针消息，值为浮点数，以百分比方式显示
	///@param	parameter	参数名
	///@param	value	要发送的消息
	void SendPercentageProbeMessage(const char *parameter, const double value);
protected:
	virtual void SendProbeMessageImpl(const char *parameter, const char *value) = 0;
};

#endif
