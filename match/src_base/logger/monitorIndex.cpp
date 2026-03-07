#include "monitorIndex.h"

time_t CMonitorIndex::m_minNextTime = 0;
vector<CMonitorIndex *>	*CMonitorIndex::m_indexList;
CProbeLogger *CMonitorIndex::m_probeLogger = NULL;
int CMonitorIndex::m_handleCountWait = 1;
int CMonitorIndex::m_handleCountCheckFrequncy = 1;
CRITICAL_VAR CMonitorIndex::m_criticalVar;
bool CMonitorIndex::m_inited = 0;
int CMonitorIndex::m_logLevel = 0;

void CMonitorIndex::setProbeLogger(CProbeLogger *pProbeLogger)
{
	if (m_probeLogger != NULL)
	{
		//如果重复设置，这里不需要理会
		if (m_probeLogger == pProbeLogger)
			return;
		delete m_probeLogger;	// 对象没有在它创建的地方销毁，要当心。
	}
	m_probeLogger = pProbeLogger;
}

CProbeLogger * CMonitorIndex::getProbeLogger()
{
	return m_probeLogger;
}

///------
//修改了初始化的过程，解决了在没有设置Syslog的情况下，一些其他功能不能使用的问题
///======

void CMonitorIndex::init(CProbeLogger *pProbeLogger, const char *logLevelString)
{
	if (*logLevelString)
	{
		int logLevel;
		if (!strcmp(logLevelString, "debug"))
		{
			logLevel = LOG_DEBUG;
		}
		else if (!strcmp(logLevelString, "info"))
		{
			logLevel = LOG_INFO;
		}
		else if (!strcmp(logLevelString, "critical"))
		{
			logLevel = LOG_CRITICAL;
		}
		else if (!strcmp(logLevelString, "none"))
		{
			logLevel = LOG_NONE;
		}
		else if (isdigit(*logLevelString))
		{
			logLevel = atoi(logLevelString);
		}
		else
		{
			logLevel = LOG_DEBUG;
		}
		if (logLevel > LOG_DEBUG)
		{
			logLevel = LOG_DEBUG;
		}
		else if (logLevel < LOG_NONE)
		{
			logLevel = LOG_NONE;
		}

		m_logLevel = logLevel;
	}
	else
	{
		m_logLevel = LOG_DEBUG;
	}

	if (pProbeLogger == NULL)
		return;

	setProbeLogger(pProbeLogger);

	//	CBoolMonitorIndex *pIsActiveIndex=new CBoolMonitorIndex("IsActive",20);
	//	pIsActiveIndex->setValue(true);
}

void CMonitorIndex::handleOne(void)
{
	m_handleCountWait--;
	if (m_handleCountWait > 0)
	{
		///看来还没有到检查点
		return;
	}
	///已经到了检查点了，首先重新设置下次检查需要等待的次数
	m_handleCountWait = m_handleCountCheckFrequncy;

	if (m_probeLogger == NULL)		//还没有设置探针，那就不干了
		return;

	//然后根据当前的事件，判断是否需要报告
	time_t now;
	time(&now);
	if (now < m_minNextTime) //最小的下次时间都没有到，那就不用干了	
		return;

	vector<CMonitorIndex *>::iterator it;
	for (it = m_indexList->begin(); it < m_indexList->end(); it++)
	{
		if ((*it)->m_nextTime <= now)
		{
			///本指标的时间到了，那就对其进行报告
			(*it)->report(m_probeLogger);
			///设置下次时间
			if ((*it)->m_frequncy <= 60)
			{
				(*it)->m_nextTime = (now / ((*it)->m_frequncy) + 1)*((*it)->m_frequncy);
			}
			else
			{
				(*it)->m_nextTime = now + (*it)->m_frequncy;
			}
		}
		if (it == m_indexList->begin())
		{
			m_minNextTime = (*it)->m_nextTime;
		}
		else
		{
			if ((*it)->m_nextTime < m_minNextTime)
			{
				m_minNextTime = (*it)->m_nextTime;
			}
		}
	}
}

void CMonitorIndex::setHandleCountCheckFrequncy(int newFrequncy)
{
	m_handleCountCheckFrequncy = newFrequncy;
}

void CMonitorIndex::reportAll(void)
{
	if (m_probeLogger == NULL)//还没有设置探针，那就不干了
		return;

	vector<CMonitorIndex *>::iterator it;
	for (it = m_indexList->begin(); it < m_indexList->end(); it++)
	{
		(*it)->report(m_probeLogger);
	}
}

void CIntMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	pProbeLogger->SendProbeMessage(m_name, m_value);
}

void CUFINT8MonitorIndex::report(CProbeLogger *pProbeLogger)
{
	pProbeLogger->SendProbeMessage(m_name, m_value);
}

void CIntUsageMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	pProbeLogger->SendProbeMessage(m_name, m_value);
	pProbeLogger->SendPercentageProbeMessage(m_usageName, ((double)m_value) / m_totalValue);
}

void CIntPtrMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	if (m_Total > 0)
	{
		double per = (*m_ptr);
		pProbeLogger->SendPercentageProbeMessage(m_name, (per / m_Total));
	}
	else
		pProbeLogger->SendProbeMessage(m_name, *m_ptr);
}

void CPerIntPtrMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	if (m_per > 0 && (*m_ptr) > m_per)
		m_per = (*m_ptr) - m_per;
	else
		m_per = 0;

	char nameBuffer[MAX_PARAMETER_NAME];
	sprintf(nameBuffer, "Tatal%s", m_name);
	pProbeLogger->SendProbeMessage(nameBuffer, *m_ptr);
	sprintf(nameBuffer, "Increase%s", m_name);
	pProbeLogger->SendProbeMessage(nameBuffer, m_per);
	m_per = *m_ptr;
}

void CLongPtrMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	if (m_Total > 0)
	{
		double per = (*m_ptr);
		pProbeLogger->SendPercentageProbeMessage(m_name, (per / m_Total));
	}
	else
		pProbeLogger->SendProbeMessage(m_name, *m_ptr);
}

void CPerLongPtrMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	if (m_per > 0 && (*m_ptr) > m_per)
		m_per = (*m_ptr) - m_per;
	else
		m_per = 0;

	char nameBuffer[MAX_PARAMETER_NAME];
	sprintf(nameBuffer, "Tatal%s", m_name);
	pProbeLogger->SendProbeMessage(nameBuffer, *m_ptr);
	sprintf(nameBuffer, "Increase%s", m_name);
	pProbeLogger->SendProbeMessage(nameBuffer, m_per);
	m_per = *m_ptr;
}

void CBoolMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	pProbeLogger->SendProbeMessage(m_name, (int)m_value);
}

void CStringMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	pProbeLogger->SendProbeMessage(m_name, m_value);
}

void CIntTotalMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	pProbeLogger->SendProbeMessage(m_name, m_value);
	m_totalValue += m_value;
	pProbeLogger->SendProbeMessage(m_totalName, m_totalValue);
	m_value = 0;
}

void CUFINT8TotalMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	pProbeLogger->SendProbeMessage(m_name, m_value);
	m_totalValue += m_value;
	pProbeLogger->SendProbeMessage(m_totalName, m_totalValue);
	m_value = 0;
}

void CStringBufferMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	pProbeLogger->SendProbeMessage(m_name, m_value);
}

void CStringBufferArrayMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_value[i][0] != '\0')
		{
			char nameBuffer[MAX_PARAMETER_NAME];
			sprintf(nameBuffer, "%s.%d", m_name, i + 1);
			pProbeLogger->SendProbeMessage(nameBuffer, m_value[i]);
		}
	}
}

void CFloatMonitorIndex::report(CProbeLogger *pProbeLogger)
{
	pProbeLogger->SendProbeMessage(m_name, m_value);
}

void CEventMonitor::report(const char *detail)
{
	if (m_probeLogger == NULL)
		return;

	char buffer[4096] = { 0 };
	snprintf(buffer, sizeof(buffer), "%s %s %s %s", m_type, m_ipAdr, m_user, detail);
	m_probeLogger->SendProbeMessage("event", buffer);
}

void CEventMonitor::report(const char *type, const char *detail)
{
	if (m_probeLogger == NULL)
		return;

	char buffer[4096] = { 0 };
	snprintf(buffer, sizeof(buffer), "%s %s %s %s", type, m_ipAdr, m_user, detail);
	m_probeLogger->SendProbeMessage("event", buffer);
}

void CEventMonitor::report(const char *type, const char *ipAdr, const char *user, const char *detail)
{
	if (m_probeLogger == NULL)
		return;

	char buffer[4096] = { 0 };
	sprintf(buffer, "%s %s %s %s", type, ipAdr, user, detail);
	m_probeLogger->SendProbeMessage("event", buffer);
}

void CEventMonitor::report(CProbeLogger *pProbeLogger)
{
}

void CEventMonitor::report(int level, const char *type, const char *format, ...)
{
	if (level > m_logLevel)
		return;

	char buffer[4096] = { 0 };

	/// 若出现关键错误信息，在日志中添加单词 Critical，这样容易在日志中找到
// 	if(LOG_CRITICAL <= level)
// 	{
// 		sprintf(buffer,"Critical %s %d ",type,level);
// 	}
// 	else
// 	{
// 		sprintf(buffer,"%s %d ",type,level);
//	}

	snprintf(buffer, sizeof(buffer), "%s %d ", type, level);
	va_list v;
	va_start(v, format);
	vsnprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), format, v);
	if (m_probeLogger != NULL)
		m_probeLogger->SendProbeMessage("event", buffer);

	/// 将关键的输出在标准输出里面输出
	if (LOG_CRITICAL >= level)
	{
		time_t now = time(NULL);
		char timeBuffer[100];
		struct tm tt;
		LOCALTIME(&tt, &now);
		strftime(timeBuffer, 100, "%m-%d %H:%M:%S", &tt);

		printf("[%s]: %s \n", timeBuffer, buffer);
		fflush(stdout);
	}
	va_end(v);
}

CEventMonitor stdEventMonitor;
