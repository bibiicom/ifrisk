#include "CLogger.h"

CLogger * CLogger::m_pInstance;

CLogger * CLogger::GetInstance()
{
	return m_pInstance;
}

void CLogger::output(int indent,int level,char *format,...)
{
	output(level,"\n");
	for (int i=0;i<indent;i++)
	{
		output(level,"\t");
	}
	va_list v;
	va_start(v,format);
	output(level,format,v);
	va_end(v);
}

void CLogger::output(char *format,...)
{
	va_list v;
	va_start(v,format);
	output(LL_DEBUG, format, v);
	va_end(v);
}

class CStdLogger :public CLogger
{
public:
	CStdLogger()
	{
		m_pInstance = this;
		INIT_CRITICAL_VAR(m_varCritical);
	}
	~CStdLogger()
	{
		DELETE_CRITICAL_VAR(m_varCritical);
	}
	
	virtual void output(int level, char *format, ...)
	{
		ENTER_CRITICAL(m_varCritical);
		va_list v;
		va_start(v,format);
		vfprintf(stdout,format,v);
		va_end(v);

		fflush(stdout);
		LEAVE_CRITICAL(m_varCritical);
	}
private:
	static CStdLogger m_stdLogger;
	CRITICAL_VAR m_varCritical;
};

CStdLogger CStdLogger::m_stdLogger;

