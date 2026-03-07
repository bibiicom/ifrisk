#include "BaseEngine.h"
#include "Config.h"
#include "tradingTools.h"

#ifndef WIN32
#include <fcntl.h>
#include <sys/mman.h>
#endif

CBaseEngine::CBaseEngine()
{
	//更多索引，更好查找速度
	g_cMDBRunLevel = 's';

	m_pMDB = new CMDB();
	int memorySize = GetConfigInt(INI_FILE_NAME, "MemorySize");
	int blockCount = GetConfigInt(INI_FILE_NAME, "MaxBlockCount");
	m_pMDB->init(memorySize, blockCount);

	m_pErrorEngine = new CErrorEngine();
	defineErrors(m_pErrorEngine);

	string sFileName = GetConfigString(INI_FILE_NAME, "MDBLog");
	if (sFileName != "")
	{
		m_pMDBLog = new CMDBLog(m_pMDB, sFileName.c_str());
		m_pMDBLog->start();
	}
	else
	{
		m_pMDBLog = NULL;
	}
}

CBaseEngine::~CBaseEngine(void)
{
	if (m_pMDBLog != NULL)
	{
		m_pMDBLog->stop();
		delete m_pMDBLog;
	}
	delete m_pErrorEngine;
	delete m_pMDB;
}
