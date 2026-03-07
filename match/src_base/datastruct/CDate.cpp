#include "public.h"
#include "CDate.h"

CDate::CDate(int lDate)
{
	m_strDate = LongToDate(lDate);
}

CDate::CDate(const char *pszDate)
{
	int lDate = DateToLong(pszDate);
	m_strDate = LongToDate(lDate);
}

CDate::~CDate()
{
}

int CDate::IsLeapYear(int nYear)
{
	if (nYear - nYear / 400 * 400 == 0 || (nYear - nYear / 4 * 4 == 0 && nYear - nYear / 100 * 100 != 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CDate::GetDays(int nYear, int nMonth)
{
	int days;
	switch (nMonth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		days = 31; break;
	case 2:
		days = 28 + IsLeapYear(nYear); break;
	case 4:
	case 6:
	case 9:
	case 11:
		days = 30; break;
	default:
		days = 0;
	}
	return days;
}


int CDate::DateToLong(const char *pszDate)
{
	//char bz=0;
	char tm[9];

	strncpy(tm, pszDate, 4);
	tm[4] = 0;
	int nYear = atoi(tm);

	strncpy(tm, pszDate + 4, 2);
	tm[2] = 0;
	int nMonth = atoi(tm);

	strncpy(tm, pszDate + 6, 2);
	tm[2] = 0;
	int nDay = atoi(tm);

	int lDate = 0;
	int i = 0;
	for (i = 1980; i < nYear; i++)
		lDate += (365 + IsLeapYear(i));

	for (i = 1; i < nMonth; i++)
		lDate += GetDays(nYear, i);

	lDate += nDay;
	return lDate;
}

int CDate::DateFrom2020ToLong(const char *pszDate)
{
	//char bz=0;
	char tm[9];

	strncpy(tm, pszDate, 4);
	tm[4] = 0;
	int nYear = atoi(tm);

	strncpy(tm, pszDate + 4, 2);
	tm[2] = 0;
	int nMonth = atoi(tm);

	strncpy(tm, pszDate + 6, 2);
	tm[2] = 0;
	int nDay = atoi(tm);

	int lDate = 0;
	int i = 0;
	for (i = 2020; i < nYear; i++)
		lDate += (365 + IsLeapYear(i));

	for (i = 1; i < nMonth; i++)
		lDate += GetDays(nYear, i);

	lDate += nDay;
	return lDate;
}

char *CDate::LongToDate(int lDate)
{
	static char date[9];

	int nYear = 0;
	for (nYear = 1980; lDate > 365 + IsLeapYear(nYear); nYear++)
	{
		lDate -= (365 + IsLeapYear(nYear));
	}

	int nMonth = 0;
	for (nMonth = 1; lDate > GetDays(nYear, nMonth); nMonth++)
	{
		lDate -= GetDays(nYear, nMonth);
	}

	int nDay = lDate;

	sprintf(date, "%04d%02d%02d", nYear, nMonth, nDay);
	return date;
}

int CDate::GetWeekDay(const char *pszDate)
{
	int lDate = DateToLong(pszDate);
	return (lDate + 1 - (lDate + 1) / 7 * 7);
}

int CDate::Day()
{
	int nDay = 0;
	char tm[9];

	memset(tm, 0, 9);
	memcpy(tm, ToString() + 6, 2);
	sscanf(tm, "%d", &nDay);
	return nDay;
}

int CDate::Month()
{
	int nMonth = 0;
	char tm[9];

	memset(tm, 0, 9);
	memcpy(tm, ToString() + 4, 2);
	sscanf(tm, "%d", &nMonth);
	return nMonth;
}

int CDate::Year()
{
	int nYear = 0;
	char tm[9];

	memset(tm, 0, 9);
	memcpy(tm, ToString(), 4);
	sscanf(tm, "%d", &nYear);
	return nYear;
}

int CDate::WeekDay()
{
	return (GetWeekDay(ToString()));
}

bool CDate::IsValid(const char *pszDate)
{
	CDate cdate(pszDate);
	return (strcmp(pszDate, cdate.ToString()) == 0);
}

int CDate::ToLong()
{
	return DateToLong(ToString());
}

const char * CDate::ToString()
{
	return m_strDate.c_str();
}

CDate CDate::operator +(int i)
{
	return CDate(ToLong() + i);
}

CDate &CDate::operator ++(int)
{
	int lDate = ToLong() + 1;
	m_strDate = LongToDate(lDate);

	return *this;
}

CDate &CDate::operator +=(int i)
{
	int lDate = ToLong() + i;
	m_strDate = LongToDate(lDate);

	return *this;
}

int CDate::operator -(CDate &date)
{
	return (ToLong() - date.ToLong());
}

int CDate::operator -(const char * pszDate)
{
	return (ToLong() - CDate(pszDate).ToLong());
}

CDate &CDate::operator -=(int i)
{
	int lDate = ToLong() - i;
	m_strDate = LongToDate(lDate);

	return *this;
}

CDate &CDate::operator --(int)
{
	int lDate = ToLong() - 1;
	m_strDate = LongToDate(lDate);

	return *this;
}

CDate CDate::operator -(int i)
{
	return 	CDate(ToLong() - i);
}

bool CDate::operator ==(const char *pszDate)
{
	return (ToLong() == CDate(pszDate).ToLong());
}

bool CDate::operator ==(CDate &date)
{
	return (ToLong() == date.ToLong());
}

////////////////////////////////////////////////////////////////////////////

CTime::CTime(const char *pszTime)
{
	m_lTimeValue = TimeToLong(pszTime);
}

CTime::CTime(int lTime)
{
	m_lTimeValue = lTime;
}

CTime::~CTime()
{
}

bool CTime::IsValid()
{
	return m_lTimeValue != INVALID_TIME;
}

int CTime::ToLong()
{
	return m_lTimeValue;
}

const char *CTime::ToString()
{
	return LongToTime(m_lTimeValue);
}

int CTime::Hour()
{
	return m_lTimeValue / 3600;
}

int CTime::Minute()
{
	return (m_lTimeValue % 3600) / 60;
}

int CTime::Second()
{
	return m_lTimeValue % 60;
}

CTime CTime::operator+(const int lTimeSpan)
{
	m_lTimeValue += lTimeSpan;
	/*	while (m_lTimeValue < 0)
		{
			m_lTimeValue += MAX_TIME_VALUE;
		}*/
	m_lTimeValue = m_lTimeValue % MAX_TIME_VALUE;

	return *this;
}

CTime CTime::operator-(const int lTimeSpan)
{
	return *this + (-lTimeSpan);
}

int CTime::operator-(const CTime &t)
{
	return m_lTimeValue - t.m_lTimeValue;
}

bool CTime::operator==(const CTime &t)
{
	return m_lTimeValue == t.m_lTimeValue;
}

bool CTime::operator==(const char *pszTime)
{
	return m_lTimeValue == TimeToLong(pszTime);
}

bool CTime::IsValid(const char *pszTime)
{
	return TimeToLong(pszTime) != INVALID_TIME;
}

int CTime::TimeToLong(const char *pszTime)
{
	int nLen = STRLEN(pszTime);
	//长度必须为8
	if (nLen != 8)
	{
		if (nLen == 0)
		{
			return 0;
		}
		return INVALID_TIME;
	}

	//必须用冒号分隔时分秒
	if (pszTime[2] != ':' || pszTime[5] != ':')
	{
		return INVALID_TIME;
	}

	int nHour = (pszTime[0] - '0') * 10 + (pszTime[1] - '0');
	if (nHour < 0 || nHour >= 24)
	{
		return INVALID_TIME;
	}
	int nMinute = (pszTime[3] - '0') * 10 + (pszTime[4] - '0');
	if (nMinute < 0 || nMinute >= 60)
	{
		return INVALID_TIME;
	}
	int nSecond = (pszTime[6] - '0') * 10 + (pszTime[7] - '0');
	if (nSecond < 0 || nSecond >= 62/*有可能有闰秒*/)
	{
		return INVALID_TIME;
	}

	return nHour * 3600 + nMinute * 60 + nSecond;
}

int CTime::MinToLong(const char *pszTime)
{
	int nLen = (int)strlen(pszTime);
	//长度必须为8
	if (nLen != 8)
	{
		if (nLen == 0)
		{
			return 0;
		}
		return INVALID_TIME;
	}

	//必须用冒号分隔时分秒
	if (pszTime[2] != ':' || pszTime[5] != ':')
	{
		return INVALID_TIME;
	}

	int nHour = (pszTime[0] - '0') * 10 + (pszTime[1] - '0');
	if (nHour < 0 || nHour >= 24)
	{
		return INVALID_TIME;
	}
	int nMinute = (pszTime[3] - '0') * 10 + (pszTime[4] - '0');
	if (nMinute < 0 || nMinute >= 60)
	{
		return INVALID_TIME;
	}
	int nSecond = (pszTime[6] - '0') * 10 + (pszTime[7] - '0');
	if (nSecond < 0 || nSecond >= 62/*有可能有闰秒*/)
	{
		return INVALID_TIME;
	}

	return nHour * 60 + nMinute;
}

const char *CTime::LongToTime(int lTime)
{
	static char szTime[9];

	if (/*lTime <0 ||*/ lTime >= MAX_TIME_VALUE)
	{
		return NULL;
	}

	CTime t(lTime);
	sprintf(szTime, "%02d:%02d:%02d", t.Hour(), t.Minute(), t.Second());

	return szTime;
}
