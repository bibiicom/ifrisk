#include "CSequence.h"
#include "printinfo.h"

CSequence::CSequence(UF_INT8 nGapValue)
{
	m_curValue = 0.0;
	m_digits = nGapValue;
}

CSequence::~CSequence()
{
}

const char* CSequence::getNextValueByHex()
{
	int i = 12;
	for (int i = 12; i >= 0; i--)
	{
		if (m_seqHex[i] == '9')
		{
			m_seqHex[i] = 'A';
			break;
		}
		else if (m_seqHex[i] == 'F')
		{
			m_seqHex[i] = '0';
		}
		else
		{
			m_seqHex[i]++;
			break;
		}
	}
	return m_seqHex;
}

const char* CSequence::getNextValueByString()
{
	//m_curValue++;
	//sprintf(m_seqString, "%.0lf", m_curValue);
	//return m_seqString;
	//为了能优化sprintf的低效率，使用技巧来生产字符串

	int i = 15;
	for (int i = 15; i >= 0; i--)
	{
		if (m_seqString[i] == '9')
		{
			m_seqString[i] = '0';
		}
		else
		{
			m_seqString[i]++;
			break;
		}
	}
	return m_seqString;
}

const UF_INT8 CSequence::getNextValueByDouble()
{
	m_curValue++;
	return m_curValue;
}

const char* CSequence::touchNextValueByHex(UF_INT8 nNext)
{
	m_curValue = atof(m_seqHex);
	snprintf(m_seqTouchHex, 14, "%.0lf", m_curValue + nNext);
	return m_seqTouchHex;
}

const char* CSequence::touchNextValueByString(UF_INT8 nNext)
{
	m_curValue = atof(m_seqString);
	snprintf(m_seqTouchString, 17, "%.0lf", m_curValue + nNext);
	return m_seqTouchString;
}

const UF_INT8 CSequence::touchNextValueByDouble(UF_INT8 nNext)
{
	return m_curValue + nNext;
}

const char* CSequence::getCurrentValueByHex()
{
	//sprintf(m_seqString, "%.0lf", m_curValue);
	return m_seqHex;
}

const char* CSequence::getCurrentValueByString()
{
	//sprintf(m_seqString, "%.0lf", m_curValue);
	return m_seqString;
}

const UF_INT8 CSequence::getCurrentValueByDouble()
{
	return m_curValue;
}

void CSequence::initValueByTime(UF_INT8 nTime)
{
	//不需要移动位数，这样字符串的比较就不会出问题
	UF_INT8 beginValue = 1000000000000000;
	UF_INT8 seconds = nTime - 1621699200;
	m_curValue = beginValue + m_digits * seconds;
	snprintf(m_seqString, 17, "%.0lf", m_curValue);
	snprintf(m_seqHex, 14, "%.0lf", m_curValue);
}

void CSequence::initValueByValue(UF_INT8 nValue)
{
	m_curValue = nValue;
	snprintf(m_seqString, 17, "%.0lf", m_curValue);
	snprintf(m_seqHex, 14, "%.0lf", m_curValue);
}

void CSequence::initValueByValue(int nValue)
{
	m_curValue = nValue;
	snprintf(m_seqString, 17, "%.0lf", m_curValue);
	snprintf(m_seqHex, 14, "%.0lf", m_curValue);
}

void CSequence::initValueByValue(const char* sValue)
{
	snprintf(m_seqString, 17, "%s", sValue);
	m_curValue = atof(sValue);
	snprintf(m_seqHex, 14, "%.0lf", m_curValue);
}

void CSequence::initValueByHex(const char* sValue)
{
	snprintf(m_seqHex, 14, "%s", sValue);
	m_curValue = atof(sValue);
	snprintf(m_seqString, 17, "%.0lf", m_curValue);
}

