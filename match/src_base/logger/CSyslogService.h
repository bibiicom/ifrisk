#ifndef CSYSLOGSERVICE_H
#define CSYSLOGSERVICE_H

#ifdef UNIX
#include <syslog.h>

#include "CProbeLogger.h"

class CSyslogService : public CProbeLogger
{
public:
	///构造方法
	///@param	programName	指定的程序名
	///@param	globalSeq	程序全局序列号
	CSyslogService(): CProbeLogger()
	{
		openlog( "Syslog", LOG_PID, LOG_USER );
	}
	
	///析构函数
	virtual ~CSyslogService(void)
	{
		closelog();
	}

	virtual void SendProbeMessageImpl(const char *parameter, const char *value);
private:
	char m_ident[100];
};

#endif

#endif
