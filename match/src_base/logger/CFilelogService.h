#ifndef CFILELOGSERVICE_H
#define CFILELOGSERVICE_H

#include "CProbeLogger.h"
#include "utility.h"

class CFilelogService : public CProbeLogger
{
private:
	FILE *m_fp;
	char m_hostname[1000];
	int m_pid;
	char m_fname[256];
	char m_fDate[9];

	void reopen();
public:
	CFilelogService(const char *fileName) : CProbeLogger()
	{
		strncpy(m_fname, fileName, sizeof(m_fname));
		int dp = (int)strlen(m_fname) - 4;
		if (strcmp(&m_fname[dp], ".log") == 0) {
			m_fname[dp] = 0;
		}
		m_fp = NULL;
		GetNowDate(m_fDate);
		reopen();
#ifdef WINDOWS
		WSADATA initData;
		WSAStartup(2, &initData);
		gethostname(m_hostname, 100);
		WSACleanup();
#else
		gethostname(m_hostname, 100);
#endif
		m_pid = getpid();
	}

	///Îö¹¹º¯Êý
	virtual ~CFilelogService(void)
	{
		if (m_fp != NULL)
		{
			fclose(m_fp);
		}
	}

	virtual void SendProbeMessageImpl
	(
		const char *parameter,
		const char *value
	);

	static CFilelogService *CreateInstance(const char *fileName);
};

#endif
