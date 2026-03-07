/////////////////////////////////////////////////////////////////////////
///monitorIndex.h
///定义与监控有关的若干指标
/////////////////////////////////////////////////////////////////////////

#ifndef MONITORINDEX_H
#define MONITORINDEX_H

#include "platform.h"
#include "CProbeLogger.h"

/////////////////////////////////////////////////////////////////////////
///IMonitorIndex是抽象的监控指标的接口
/////////////////////////////////////////////////////////////////////////
class IMonitorIndex
{
public:
	virtual ~IMonitorIndex(void)
	{
	};

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger) = 0;
};

/////////////////////////////////////////////////////////////////////////
///CMonitorIndex是监控指标的基础类，所有的具体类型必须继承此类
/////////////////////////////////////////////////////////////////////////
class CMonitorIndex : public IMonitorIndex
{
public:
	///构造方法
	///@param	frequncy	本指标的刷新频率,小于等于0表示不需要统一报告
	CMonitorIndex(int frequncy)
	{
		if (!m_inited)
		{
			INIT_CRITICAL_VAR(m_criticalVar);
			m_indexList = new vector<CMonitorIndex *>;
			m_inited = true;
		}

		if (frequncy > 0)
		{
			ENTER_CRITICAL(m_criticalVar);
			m_frequncy = frequncy;
			m_indexList->push_back(this);
			m_nextTime = 0;
			LEAVE_CRITICAL(m_criticalVar);
		}
	}

	///析构方法
	virtual ~CMonitorIndex(void)
	{
		ENTER_CRITICAL(m_criticalVar);
		vector<CMonitorIndex *>::iterator it;
		for (it = m_indexList->begin(); it < m_indexList->end(); it++)
		{
			if (*it == this)
			{
				m_indexList->erase(it);
				break;
			}
		}
		LEAVE_CRITICAL(m_criticalVar);
	}

	///设置探针
	///@param	pProbeLogger	新的探针
	static void setProbeLogger(CProbeLogger *pProbeLogger);

	///获取探针
	///@param	pProbeLogger	探针
	static CProbeLogger * getProbeLogger();

	///-------
	///通过配置管理所有的标准输出内容
	///-------
	static void init(CProbeLogger *pProbeLogger, const char *logLevelString);

	///完成一次事情处理，将进行适当地检查，并在必要的情况下，予以报告
	static void handleOne(void);

	///设置新的事情处理检查频率
	///@param	newFrequncy	新的刷新频率
	static void setHandleCountCheckFrequncy(int newFrequncy);

	///报告所有的指标
	static void reportAll(void);
protected:
	///本指标的刷新频率
	int m_frequncy;

	///各个组的下次刷新事件
	time_t m_nextTime;

	///最下的下一次的时间
	static time_t m_minNextTime;

	///记录所有指标清单
	static vector<CMonitorIndex *>	*m_indexList;

	///使用的探针
	static CProbeLogger *m_probeLogger;

	///还要等多少次事情的处理，再进行检查
	static int m_handleCountWait;

	///处理事情的次数每当到多少时，进行一次检查
	static int m_handleCountCheckFrequncy;

	///临界区变量
	static CRITICAL_VAR m_criticalVar;

	///是否所有变量已经完成初始化
	static bool m_inited;

	///目前的日志级别，范围是0至3，数字越大，日志越多
	static int m_logLevel;
};

/////////////////////////////////////////////////////////////////////////
///CIntMonitorIndex是监控一个整数指标的类
/////////////////////////////////////////////////////////////////////////
class CIntMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	frequncy	本指标的刷新频率
	CIntMonitorIndex(const char *name, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_value = 0;
	}

	///析构函数
	~CIntMonitorIndex(void)
	{
	}

	///增加值
	///@param	value	新的值
	void incValue(int value = 1)
	{
		m_value += value;
	}

	///设置值
	///@param	value	新的值
	void setValue(int value)
	{
		m_value = value;
	}

	///获取值
	///@return	得到的当前值
	int getValue(void)
	{
		return m_value;
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	int m_value;
};

/////////////////////////////////////////////////////////////////////////
///CUFINT8MonitorIndex是监控一个UF_INT8指标的类
/////////////////////////////////////////////////////////////////////////
class CUFINT8MonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	frequncy	本指标的刷新频率
	CUFINT8MonitorIndex(const char *name, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_value = 0;
	}

	///析构函数
	~CUFINT8MonitorIndex(void)
	{
	}

	///增加值
	///@param	value	新的值
	void incValue(UF_INT8 value = 1)
	{
		m_value += value;
	}

	///设置值
	///@param	value	新的值
	void setValue(UF_INT8 value)
	{
		m_value = value;
	}

	///获取值
	///@return	得到的当前值
	UF_INT8 getValue(void)
	{
		return m_value;
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	UF_INT8 m_value;
};


/////////////////////////////////////////////////////////////////////////
///CIntUsageMonitorIndex是监控一个整数为基础的需要计算使用率的指标类
/////////////////////////////////////////////////////////////////////////
class CIntUsageMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	totalValue	本指标的总量
	///@param	frequncy	本指标的刷新频率
	CIntUsageMonitorIndex(const char *name, int totalValue, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_value = 0;
		m_totalValue = totalValue;
		sprintf(m_usageName, "%sUsage", m_name);
	}

	///析构函数
	~CIntUsageMonitorIndex(void)
	{
	}

	///增加值
	///@param	value	新的值
	void incValue(int value = 1)
	{
		m_value += value;
	}

	///设置值
	///@param	value	新的值
	void setValue(int value)
	{
		m_value = value;
	}

	///获取值
	///@return	得到的当前值
	int getValue(void)
	{
		return m_value;
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	char m_usageName[MAX_PARAMETER_NAME];
	int m_value;
	int m_totalValue;
};

/////////////////////////////////////////////////////////////////////////
///CIntPtrMonitorIndex是监控一个整数指标的类（使用指针）
/////////////////////////////////////////////////////////////////////////
class CIntPtrMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	ptr	指向整数的指针
	///@param	frequncy	本指标的刷新频率
	CIntPtrMonitorIndex(const char *name, const int *ptr, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_ptr = ptr;
		m_Total = 0;
	}

	///构造函数 计算百分比的指标输出
	///@param	name	参数名称
	///@param	ptr	指向整数的指针
	///@param	nTotal	指向整数的指针
	///@param	frequncy	本指标的刷新频率
	CIntPtrMonitorIndex(const char *name, const int *ptr, int nTotal, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_ptr = ptr;
		m_Total = nTotal;
	}

	///析构函数
	~CIntPtrMonitorIndex(void)
	{
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	const int *m_ptr;
	int	m_Total;
};


/////////////////////////////////////////////////////////////////////////
///CPerIntPtrMonitorIndex是监控一个单位时间内整数指标的类（使用指针）
/////////////////////////////////////////////////////////////////////////
class CPerIntPtrMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	ptr	指向整数的指针
	///@param	frequncy	本指标的刷新频率
	CPerIntPtrMonitorIndex(const char *name, const int *ptr, int frequncy) : CMonitorIndex(frequncy)
	{
		//memset(m_name,0,sizeof(m_name));
		//strncpy(m_name,name,MAX_PARAMETER_NAME-1);
		// 精确复制
		int nLength = (int)strlen(name);
		strncpy(m_name, name, nLength);
		m_name[nLength] = '\0';
		m_ptr = ptr;
		m_per = 0;
	}

	///析构函数
	~CPerIntPtrMonitorIndex(void)
	{
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	char m_name[MAX_PARAMETER_NAME];
	const int *m_ptr;
	int        m_per;
};

/////////////////////////////////////////////////////////////////////////
///CLongPtrMonitorIndex是监控一个长整数指标的类（使用指针）
/////////////////////////////////////////////////////////////////////////
class CLongPtrMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	ptr	指向整数的指针
	///@param	frequncy	本指标的刷新频率
	CLongPtrMonitorIndex(const char *name, const UF_INT8 *ptr, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_ptr = ptr;
		m_Total = 0;
	}

	///构造函数 计算百分比的指标输出
	///@param	name	参数名称
	///@param	ptr	指向整数的指针
	///@param	nTotal	指向整数的指针
	///@param	frequncy	本指标的刷新频率
	CLongPtrMonitorIndex(const char *name, const UF_INT8 *ptr, UF_INT8 nTotal, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_ptr = ptr;
		m_Total = nTotal;
	}

	///析构函数
	~CLongPtrMonitorIndex(void)
	{
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	const UF_INT8 *m_ptr;
	UF_INT8 m_Total;
};

/////////////////////////////////////////////////////////////////////////
///CPerLongPtrMonitorIndex是监控一个单位时间内长整数指标的类（使用指针）
///@author	wl
///@version	1.0,20101029
/////////////////////////////////////////////////////////////////////////
class CPerLongPtrMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	ptr	指向整数的指针
	///@param	frequncy	本指标的刷新频率
	CPerLongPtrMonitorIndex(const char *name, const UF_INT8 *ptr, int frequncy) : CMonitorIndex(frequncy)
	{
		//memset(m_name,0,sizeof(m_name));
		//strncpy(m_name,name,MAX_PARAMETER_NAME-1);
		//精确复制
		int nLength = STRLEN(name);
		strncpy(m_name, name, nLength);
		m_name[nLength] = '\0';
		m_ptr = ptr;
		m_per = 0;
	}

	///析构函数
	~CPerLongPtrMonitorIndex(void)
	{
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	char m_name[MAX_PARAMETER_NAME];
	const UF_INT8 *m_ptr;
	UF_INT8 m_per;
};

/////////////////////////////////////////////////////////////////////////
///CBoolMonitorIndex是监控一个布尔值指标的类
/////////////////////////////////////////////////////////////////////////
class CBoolMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	frequncy	本指标的刷新频率
	CBoolMonitorIndex(const char *name, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_value = 0;
	}

	///析构函数
	~CBoolMonitorIndex(void)
	{
	}

	///设置值
	///@param	value	新的值
	void setValue(bool value)
	{
		m_value = value;
	}

	///获取值
	///@return	得到的当前值
	bool getValue(void)
	{
		return m_value;
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	bool m_value;
};

/////////////////////////////////////////////////////////////////////////
///CStringMonitorIndex是监控一个字符串值指标的类
/////////////////////////////////////////////////////////////////////////
class CStringMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	value	指定的字符串
	///@param	frequncy	本指标的刷新频率
	CStringMonitorIndex(const char *name, const char *value, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_value = value;
	}

	///析构函数
	~CStringMonitorIndex(void)
	{
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	const char *m_value;
};

/////////////////////////////////////////////////////////////////////////
///CIntTotalMonitorIndex是监控一个整数总量的指标类
/////////////////////////////////////////////////////////////////////////
class CIntTotalMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	frequncy	本指标的刷新频率
	CIntTotalMonitorIndex(const char *name, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_value = 0;
		m_totalValue = 0;
		sprintf(m_totalName, "%sTotal", m_name);
	}

	///析构函数
	~CIntTotalMonitorIndex(void)
	{
	}

	///增加值
	///@param	value	新的值
	void incValue(int value = 1)
	{
		m_value += value;
	}

	///获取值
	///@return	得到的当前值
	int getValue(void)
	{
		return m_value;
	}

	///获取累计值
	///@return	当前的累计值
	int getTotalValue(void)
	{
		return m_totalValue + m_value;
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	char m_totalName[MAX_PARAMETER_NAME];
	int m_value;
	int m_totalValue;
};

class CUFINT8TotalMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	frequncy	本指标的刷新频率
	CUFINT8TotalMonitorIndex(const char *name, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_value = 0;
		m_totalValue = 0;
		sprintf(m_totalName, "%sTotal", m_name);
	}

	///析构函数
	~CUFINT8TotalMonitorIndex(void)
	{
	}

	///增加值
	///@param	value	新的值
	void incValue(int value = 1)
	{
		m_value += value;
	}

	///获取值
	///@return	得到的当前值
	int getValue(void)
	{
		return m_value;
	}

	///获取累计值
	///@return	当前的累计值
	double getTotalValue(void)
	{
		return m_totalValue + m_value;
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	char m_totalName[MAX_PARAMETER_NAME];
	int m_value;
	double m_totalValue;
};

/////////////////////////////////////////////////////////////////////////
///CStringBufferMonitorIndex是监控一个字符串值指标的类
/////////////////////////////////////////////////////////////////////////
class CStringBufferMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	frequncy	本指标的刷新频率
	CStringBufferMonitorIndex(const char *name, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_value[0] = '\0';
	}

	///构造函数
	///@param	name	参数名称
	///@param	value	参数值
	///@param	frequncy	本指标的刷新频率
	CStringBufferMonitorIndex(const char *name, const char *value, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		setValue(value);
	}

	///析构函数
	~CStringBufferMonitorIndex(void)
	{
	}

	///设置值
	///@param	value	参数值
	void setValue(const char *value)
	{
		strcpy(m_value, value);
	}

	///获取值
	///@return	当前的值
	const char *getValue(void)
	{
		return m_value;
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	char m_value[1024];
};

/////////////////////////////////////////////////////////////////////////
///CStringBufferArrayMonitorIndex是监控一个字符串值指标数组的类
/////////////////////////////////////////////////////////////////////////
class CStringBufferArrayMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	size	数组大小
	///@param	frequncy	本指标的刷新频率
	CStringBufferArrayMonitorIndex(const char *name, int size, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_size = size;
		m_value = new char*[size];
		for (int i = 0; i < size; i++)
		{
			m_value[i] = new char[1024];
			m_value[i][0] = '\0';
		}
	}

	///析构函数
	~CStringBufferArrayMonitorIndex(void)
	{
	}

	///设置值
	///@param	id	下标
	///@param	value	参数值
	void setValue(int id, const char *value)
	{
		if ((id < 1) || (id > m_size))
		{
			return;
		}
		strcpy(m_value[id - 1], value);
	}

	///获取值
	///@param	id	下标
	///@return	当前的值
	const char *getValue(int id)
	{
		if ((id < 1) || (id > m_size))
		{
			return "";
		}
		return m_value[id - 1];
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	char **m_value;
	int m_size;
};

/////////////////////////////////////////////////////////////////////////
///CFloatMonitorIndex是监控一个浮点数指标的类
/////////////////////////////////////////////////////////////////////////
class CFloatMonitorIndex : public CMonitorIndex
{
public:
	///构造函数
	///@param	name	参数名称
	///@param	frequncy	本指标的刷新频率
	CFloatMonitorIndex(const char *name, int frequncy) : CMonitorIndex(frequncy)
	{
		m_name = name;
		m_value = 0.0;
	}

	///析构函数
	~CFloatMonitorIndex(void)
	{
	}

	///增加值
	///@param	value	新的值
	void incValue(double value)
	{
		m_value += value;
	}

	///设置值
	///@param	value	新的值
	void setValue(double value)
	{
		m_value = value;
	}

	///获取值
	///@return	得到的当前值
	double getValue(void)
	{
		return m_value;
	}

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_name;
	double m_value;
};

#define LOG_NONE		0
#define LOG_EMERGENCY	1
#define LOG_CRITICAL	2
#define LOG_ERROR		3
#define LOG_WARNING		4
#define LOG_INFO		5
#define LOG_DEBUG		6

/////////////////////////////////////////////////////////////////////////
///CEventMonitor是监控一个事件的类
/////////////////////////////////////////////////////////////////////////
class CEventMonitor : public CMonitorIndex
{
public:
	///构造函数
	///@param	type	类型
	///@param	ipAdr	IP地址
	///@param	user	用户名
	CEventMonitor(const char *type = "", const char *ipAdr = "", const char *user = "") : CMonitorIndex(0)
	{
		m_type = type;
		m_ipAdr = ipAdr;
		m_user = user;
	}

	///析构函数
	~CEventMonitor(void)
	{
	}

	///报告事件
	///@param	detail	要报告的详细内容
	void report(const char *detail);

	///报告事件
	///@param	type	类型
	///@param	detail	要报告的详细内容
	void report(const char *type, const char *detail);

	///报告某个类型的事件
	///@param	level	报告的级别
	///@param	type	类型
	///@param	format	输出的格式
	///@param	...	输出的参数
	void report(int level, const char *type, const char *format, ...);

	///报告事件
	///@param	type	类型
	///@param	ipAdr	IP地址
	///@param	user	用户名
	///@param	detail	要报告的详细内容
	void report(const char *type, const char *ipAdr, const char *user, const char *detail);

	///将此指标予以报告
	///@param	pProbeLogger	报告时使用的探针
	virtual void report(CProbeLogger *pProbeLogger);
private:
	const char *m_type;
	const char *m_ipAdr;
	const char *m_user;
};

extern CEventMonitor stdEventMonitor;

#define REPORT_EVENT stdEventMonitor.report

#endif
