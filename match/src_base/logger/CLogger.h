/////////////////////////////////////////////////////////////////////////
///@file CLogger.h
///@brief定义了类CLogger
/////////////////////////////////////////////////////////////////////////

#ifndef CLOGGER_H
#define CLOGGER_H

#include "platform.h"

///定义了LOG级别
#if 0
#define LL_EMERG       0       /* system is unusable */
#define LL_ALERT       1       /* action must be taken immediately */
#define LL_CRIT        2       /* critical conditions */
#define LL_ERR         3       /* error conditions */
#define LL_WARNING     4       /* warning conditions */
#define LL_NOTICE      5       /* normal but signification condition */
#define LL_INFO        6       /* informational */
#endif
#define LL_DEBUG       7       /* debug-level messages */

/**CLogger是一个抽象的日志接口，用于应用程序输出日志
* 它在全局只有一个实例，默认实例为向stdout输出日志，
* 应用程序可以继承CLogger，实现自已的日志类
*/
class CLogger
{
public:
	/** 获取CLogger实例
	*@return 获取的CLogger实例
	*/
	static CLogger *GetInstance();

	///进行日志输出
	///@param	indent	输出时的缩进
	///@param	level	日志级别，值越小，级别越高
	///@param	format	输出格式，按照printf的格式要求
	///@param	...	具体的输出项
	void output(int indent,int level,char *format,...);
	
	///进行日志输出
	///@param	level	日志级别，值越小，级别越高
	///@param	format	输出格式，按照printf的格式要求
	///@param	...	具体的输出项
	virtual void output(int level,char *format,...) = 0;
	
	///进行日志输出，日志级别为LL_DEBUG
	///@param	format	输出格式，按照printf的格式要求
	///@param	...	具体的输出项
	void output(char *format,...);

protected:
	/**析构函数
	*/
	virtual ~CLogger(){};
protected:
	static CLogger *m_pInstance;		/**<CLogger实例*/
};

class CFileLogger :public CLogger
{
public:
	CFileLogger()
	{
		m_fpLogFile = NULL;
	}
	~CFileLogger()
	{
		if (m_fpLogFile != NULL)
		{
			fclose(m_fpLogFile);
		}
	}
	
	bool Open(const char *pszFileName)
	{
		m_fpLogFile = fopen(pszFileName, "wb");
		if (m_fpLogFile == NULL)
		{
			return false;
		}
		return true;
	}
	
	virtual void output(int level, char *format, ...)
	{
		va_list v;
		va_start(v,format);
		vfprintf(m_fpLogFile,format,v);
		va_end(v);
		fflush(m_fpLogFile);
	}
private:
	FILE *m_fpLogFile;
};


#define LOG0(level, sz)					CLogger::GetInstance()->output(level, "%s", sz)
#define LOG1(level, sz, p1)				CLogger::GetInstance()->output(level, sz, p1)
#define LOG2(level, sz, p1, p2)			CLogger::GetInstance()->output(level, sz, p1, p2)
#define LOG3(level, sz, p1, p2, p3)		CLogger::GetInstance()->output(level, sz, p1, p2, p3)

#endif
