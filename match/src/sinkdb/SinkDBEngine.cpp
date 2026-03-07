#include "public.h"
#include "SinkDBEngine.h"
#include "CFilelogService.h"
#include "DbConnection.h"
#include "BaseFunction.h"
#include "public.h"
#include "TableDefine.h"
#include "Config.h"

#define INI_FILE_NAME "sinkdb.ini"

CSinkDBEngine::CSinkDBEngine(COutFogServiceCallback *pCallback, int nKernelID) :COutFogServiceApi(pCallback)
{
	m_pServiceCallback = pCallback;
	CMonitorIndex::init(CFilelogService::CreateInstance(GetConfigString(INI_FILE_NAME, "SyslogPath").c_str()), GetConfigString(INI_FILE_NAME, "LogLevel").c_str());

	string dbname = GetConfigString(NetworkConfigFile, "dbname");
	string dbuser = GetConfigString(NetworkConfigFile, "dbuser");
	string dbpasswd = GetConfigString(NetworkConfigFile, "dbpasswd");

	REPORT_EVENT(LOG_CRITICAL, "Database", "Connect to database ... %s:%s", dbname.c_str(), dbuser.c_str());

	//检查数据库的Init的内容
	//如果有内容，说明数据库有数据，checksync=true,意味在同步之前不会插入数据
	//如果没有内容，说明数据库没数据，checksync=false,意味一开始就会写入数据
	bool bChecksync = false;
	CDbConnection DbConnection;
	if (!DbConnection.ConnectDB(dbname.c_str(), dbuser.c_str(), dbpasswd.c_str()))
		EMERGENCY_EXIT("Database,CTMDBEngine connect db error");

	m_pSinkDBDispatcher = new CSinkDBDispatcher();
	if (!m_pSinkDBDispatcher->Init(dbname.c_str(), dbuser.c_str(), dbpasswd.c_str()))
		EMERGENCY_EXIT("m_pSinkDBDispatcher,TMDBSchulder init failure");

	char csvdumppath[128];
	sprintf(csvdumppath, "CSVDumpPath_sinkdb_%d", nKernelID);
	string dumppath = GetConfigString(NetworkConfigFile, csvdumppath);
	if (dumppath == "")
		dumppath = GetConfigString(NetworkConfigFile, "CSVDumpPath");

	m_nSeriesNo = -1;
	m_fDumpPhase = mfopen("dumpPhase.log", "a+");
	char buffer[256];
	if (fgets(buffer, 256, m_fDumpPhase) != NULL)
		m_nSeriesNo = atof(buffer);

	PRINT_TO_STD("dumpPhase%0.f", m_nSeriesNo);

	//char dumpPathAndName[MAX_PATH_LEN + 1];
	//sprintf(dumpPathAndName, "%ssnap.*", dumppath.c_str());
	//m_nSeriesNo = 0.0;
	//char sRealDumppath[256];
	//sprintf(sRealDumppath, "%s%csnap.0", dumppath.c_str(), PATH_SPLIT);
	//string sflowName = sRealDumppath;
	//vector<string> flowNames = findNames(dumpPathAndName);
	//if (flowNames.size() > 0)
	//{
	//	vector<string>::iterator iter = flowNames.begin();
	//	while (iter != flowNames.end())
	//	{
	//		string eachName = *iter;
	//		vector<string> singlename = Txtsplit(eachName, "snap.");
	//		vector<string> singlename2 = Txtsplit(singlename[1], ".");
	//		UF_INT8 nThisBeginID = atof(singlename2[0].c_str());
	//		if (nThisBeginID > m_nSeriesNo)
	//		{
	//			m_nSeriesNo = nThisBeginID;
	//			sflowName = eachName;
	//		}
	//		iter++;
	//	}
	//}

	//REPORT_EVENT(LOG_CRITICAL, "SinkFrom", "sinkNo is %f", m_nSeriesNo);
	//
	////如果不同步Init，我们需要
	//if (g_bLoadCSV && GetConfigString(INI_FILE_NAME, "SinkInit") == "yes")
	//{
	//	CDBClear dbClear;
	//	dbClear.Clear();
	//	CDBInsert dbInsert;
	//	dbInsert.LoadFromCSV(sflowName.c_str(), g_LoadTableInfo, this);
	//	m_bRealDo = false;
	//}
	//else
	//{
	//	m_bRealDo = true;
	//}
	//m_nLastTNO = 0;
	//if (m_nSeriesNo < 0)
	//	m_nSeriesNo = 0;
	//m_nSeriesNo++;

}

CSinkDBEngine::~CSinkDBEngine(void)
{
}

bool CSinkDBEngine::SendTable(CFieldDescribe *pFieldDesc, const void *pRspBuffer)
{
	//m_ActionType = TSL_INSERT;
	//m_pSinkDBDispatcher->AssignTask(-1);
	DisPatch(pFieldDesc, pRspBuffer);
	return true;
}

bool CSinkDBEngine::ReqField(const TOutFogMetaRef* pOutFogReqRef, const void *pStream)
{
	//REPORT_EVENT(LOG_WARNING, "ReqField", "%c", pOutFogReqRef->Type);

	//由于需要处理事务性操作，在TNO的时钟报文过来得时候才能100%确定事务结束了，此时统计完成才有意义
	//如果再也没有时钟报文过来，Stop消息报文之后，此时最后的报文一定是时钟报文
	if (pOutFogReqRef->MessageNo == TID_PushTimeSync)
	{
		//if (pOutFogReqRef->GlobalNo < m_nSeriesNo)
		//	return false;
		m_nLastTNO = pOutFogReqRef->MessageNo;
		if (pOutFogReqRef->Type == OUTFOG_TYPE_MANAGER)
		{
			CCurrentTimeField::m_Describe.getSingleField(m_pBuffer, pStream);
			m_pSinkDBDispatcher->DispatchAll(pOutFogReqRef->GlobalNo, &(CCurrentTimeField::m_Describe), pStream);
			return true;
		}

		m_pSinkDBDispatcher->OnTimer(pOutFogReqRef->UserID, pOutFogReqRef->BusinessNo);
		//这里需要assignTask，因为需要标记最终的结束号，用户dump之后的对号，如果不把最后的competeNo更新，sinkdb的号和dump号不一致，导致无法判断是否落库完成
		//m_pSinkDBDispatcher->AssignTask(pOutFogReqRef->GlobalNo);
		//这里不能return，需要对后面的m_nSeriesNo进行判断，要不然m_nSeriesNo的snap初始值无法设置，造成core


		// 移动文件指针到开头
		fseek(m_fDumpPhase, 0, SEEK_SET);
		// 获取文件描述符并截断文件
		int fd = fileno(m_fDumpPhase);
#ifdef WINDOWS
		// Windows 使用 _chsize_s
		if (_chsize_s(fd, 0) != 0) {
			perror("Error truncating file");
		}
#else
		// POSIX 使用 ftruncate
		if (ftruncate(fd, 0) == -1) {
			perror("Error truncating file");
		}
#endif
		// 清除可能的错误标志并确保指针在开头
		rewind(m_fDumpPhase);
		// 写入数据
		fprintf(m_fDumpPhase, "%.0f\n", pOutFogReqRef->GlobalNo);
		// 确保数据写入磁盘
		fflush(m_fDumpPhase);
	}
	else
	{
		//REPORT_EVENT(LOG_WARNING, "m_nLastTNO", "%0X", m_nLastTNO);
		//定时处理，内部报文，不涉及外部系统
		if (pOutFogReqRef->Type == OUTFOG_TYPE_TIMER)
		{
			if (m_nLastTNO == TID_PushTimeSync)
			{
				REPORT_EVENT(LOG_WARNING, "OnTimer", "Using Self Timer:%s", pOutFogReqRef->UserID);
				m_pSinkDBDispatcher->OnTimer(pOutFogReqRef->UserID, pOutFogReqRef->BusinessNo);
			}
			return true;
		}
		else
		{
			m_nLastTNO = pOutFogReqRef->MessageNo;
		}
	}

	if (pOutFogReqRef->GlobalNo < m_nSeriesNo)
	{
		REPORT_EVENT(LOG_WARNING, "", "GlobalNo[%0.f] < SeriesNo[%0.f]", pOutFogReqRef->GlobalNo, m_nSeriesNo);
		return false;
	}

	m_nSeriesNo = pOutFogReqRef->GlobalNo;

	//if (pOutFogReqRef->GlobalNo < m_nSeriesNo)
	//	return false;

	//pOutFogReqRef->SeriesNo需要从m_nSeriesNo开始，否则会丢失数据
	//if (pOutFogReqRef->GlobalNo == m_nSeriesNo)
	//{
	//	if (!m_bRealDo)
	//	{
	//		m_bRealDo = true;
	//		REPORT_EVENT(LOG_CRITICAL, "FlowSeriesTouch", "GlobalNo is %f, SeriesNo is %f", pOutFogReqRef->GlobalNo, m_nSeriesNo);
	//		m_nSeriesNo++;
	//		return false;
	//	}
	//}
	//else
	//{
	//	if (!m_bRealDo)
	//	{
	//		EMERGENCY_EXIT("FlowSeriesNoError,GlobalNo %f come and %f is not touched!", pOutFogReqRef->GlobalNo, m_nSeriesNo);
	//	}
	//	else
	//	{
	//		if (pOutFogReqRef->GlobalNo < m_nSeriesNo + 1)
	//			REPORT_EVENT(LOG_CRITICAL, "FlowSeriesNoError", "GlobalNo %f is not %f+1", pOutFogReqRef->GlobalNo, m_nSeriesNo);
	//		m_nSeriesNo = pOutFogReqRef->GlobalNo;
	//	}
	//}

	//m_nSeriesNo = pOutFogReqRef->GlobalNo;

	char cType = pOutFogReqRef->Type;

	switch (cType)
	{
	case OUTFOG_TYPE_INSERT:
	{
		m_ActionType = TSL_INSERT;
		break;
	}
	case OUTFOG_TYPE_UPDATE:
	{
		m_ActionType = TSL_UPDATE;
		break;
	}
	case OUTFOG_TYPE_DELETE:
	{
		m_ActionType = TSL_DELETE;
		break;
	}
	case OUTFOG_TYPE_INSUPD:
	case OUTFOG_TYPE_RSPPUB:
	{
		m_ActionType = TSL_INSERT;
		break;
	}
	default:
	{
		return false;
	}
	}

	if (pOutFogReqRef->Reserve[0] == OUTFOG_RESERVE_SYNC_NO)
		return false;

	//m_pSinkDBDispatcher->AssignTask(pOutFogReqRef->GlobalNo);
	DisPatch(pOutFogReqRef, pStream);

	return true;
}

const char* CSinkDBEngine::getVersion()
{
	static char version[256];
	sprintf(version, "ServiceBase version: %s", "syncdb");
	return version;
}

//extern "C" COutFogServiceApi* CreateOutFogServiceApi(COutFogServiceCallback *pCallback, int nServiceID)
//{
//	CConfig *pConfig = new CConfig("sinkdb.ini");
//	if (pConfig == NULL)
//	{
//		printf("no sinkdb.ini! \n");
//	}
//	return new CSinkDBEngine(pCallback, pConfig);
//}
