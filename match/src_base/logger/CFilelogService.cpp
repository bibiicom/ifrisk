#include "CFilelogService.h"

void CFilelogService::SendProbeMessageImpl(const char *parameter, const char *value)
{
	if (m_fp != NULL)
	{
		char pszDate[9];
		struct tm tt = GetNowDate(pszDate);

		if (strcmp(pszDate, m_fDate) != 0)
		{
			strcpy(m_fDate, pszDate);
			reopen();
		}

		char timeBuffer[100];
		strftime(timeBuffer, 100, "%H:%M:%S", &tt);
		//fprintf(m_fp, "%s %s %d[%d]: %s %s\n", timeBuffer, programName, globalSeq, m_pid, parameter, value);
		//fprintf(m_fp, "%s\n",  value);
		fprintf(m_fp, "[%s]: %s %s\n", timeBuffer,parameter,value);
		fflush(m_fp);
	}
}

void CFilelogService::reopen()
{
	if (m_fp)
		fclose(m_fp);
	char fname[256];
	sprintf(fname, "%s.%s.log", m_fname, m_fDate);
	m_fp = mfopen(fname, "a+t");
}

CFilelogService *CFilelogService::CreateInstance(const char *fileName)
{
	return new CFilelogService(fileName);
}
