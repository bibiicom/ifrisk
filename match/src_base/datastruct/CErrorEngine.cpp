#include "CErrorEngine.h"

CErrorEngine::CErrorEngine(void)
{
	m_nLastErrorNo = 0;
	m_pLastErrorMsg = "Success";
	memset(m_sLastErrorMsg, 0, sizeof(m_sLastErrorMsg));
}

CErrorEngine::~CErrorEngine(void)
{
}

void CErrorEngine::registerErrorType(int errorID, char *errorMsg)
{
	char buffer[200];
	if (!insert(value_type(errorID, errorMsg)).second)
	{
		sprintf(buffer, "duplicate errorID definition:%d", errorID);
		EMERGENCY_EXIT(buffer);
	}
}

void CErrorEngine::registerErrorType(TErrorType pErrorType[])
{
	int i;
	for (i = 0;; i++)
	{
		if (pErrorType[i].errorID == 0)
			break;
		registerErrorType(pErrorType[i].errorID, pErrorType[i].errorMsg);
	}
}

bool  CErrorEngine::reportError(const int errorID, const char *format, ...)
{
	iterator it;

	it = find(errorID);
	if (it == end())
		EMERGENCY_EXIT("undefined error id %d", errorID);

	m_nLastErrorNo = errorID;
	if (format == NULL)
	{
		m_pLastErrorMsg = it->second;
	}
	else
	{
		sprintf(m_sLastErrorMsg, "%s:", it->second);
		va_list v;
		va_start(v, format);
		vsnprintf(m_sLastErrorMsg + strlen(m_sLastErrorMsg), sizeof(m_sLastErrorMsg) - strlen(m_sLastErrorMsg), format, v);
		va_end(v);
		m_pLastErrorMsg = m_sLastErrorMsg;
	}
	return false;
}

char *CErrorEngine::getErrorMsg(const int errorID)
{
	iterator it;

	it = find(errorID);
	if (it == end())
		return NULL;
	return it->second;
}

void CErrorEngine::getLastError(int *pLastErrorNo, char **pLastErrorMsg)
{
	if (pLastErrorNo != NULL)
		*pLastErrorNo = m_nLastErrorNo;
	if (pLastErrorMsg != NULL)
		*pLastErrorMsg = m_pLastErrorMsg;
}