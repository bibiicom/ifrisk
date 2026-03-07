#include "BaseEngine.h"
#include "Config.h"

#ifndef WIN32
#include <fcntl.h>
#include <sys/mman.h>
#endif

CBaseEngine::CBaseEngine()
{
	g_cMDBRunLevel = '0';

	m_pMDB = new CMDB();
	int memorySize = GetConfigInt(INI_FILE_NAME, "MemorySize");
	int blockCount = GetConfigInt(INI_FILE_NAME, "MaxBlockCount");
	m_pMDB->init(memorySize, blockCount);

	//ÉèÖÃ¼à¿ØÖ¸±ê
	new CIntPtrMonitorIndex("OrderSize", m_pMDB->m_OrderFactory->getCountPtr(), 10);
	new CIntPtrMonitorIndex("PositionSize", m_pMDB->m_PositionFactory->getCountPtr(), 10);
	new CIntPtrMonitorIndex("MarketOrderSize", m_pMDB->m_MarketOrderFactory->getCountPtr(), 10);

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
