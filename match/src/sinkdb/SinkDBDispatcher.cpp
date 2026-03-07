#include "public.h"
#include "SinkDBDispatcher.h"
#include "CSVParser.h"
#include "Config.h"

const int DBCONN_KEEP_ALIVE_TIMEOUT = 60;
#define INI_FILE_NAME "sinkdb.ini"

CSinkDBDispatcher::CSinkDBDispatcher()
{
	m_strWorkFilePath = GetConfigString(INI_FILE_NAME, "FlowPath");
	m_time = "00:00:00";

	m_sInitDB = GetConfigString(INI_FILE_NAME, "InitDB");
	m_sDownDB = GetConfigString(INI_FILE_NAME, "DownDB");
	m_bSinkInit = true;
	m_bSinkDown = true;
	if (GetConfigString(INI_FILE_NAME, "SinkInit") == "no")
		m_bSinkInit = false;

	if (GetConfigString(INI_FILE_NAME, "SinkDown") == "no")
		m_bSinkDown = false;

	//TSingleMethod *pMethod =  g_SingleMethod;
	//while (pMethod->nWorkerID >= 0)
	//{
	//	m_SingleMethodMap[pMethod->pszFieldName] = pMethod;
	//	pMethod++;
	//}

	m_nCompleteTaskNo = 0.0;

	m_pDelayMDB = new CDelayMDB(this, m_bSinkInit, m_bSinkDown);
}

CSinkDBDispatcher::~CSinkDBDispatcher()
{
	CWorkerMap::iterator i = m_mapWorker.begin();
	while (i != m_mapWorker.end())
	{
		delete (*i).second;
		i++;
	}
}

bool CSinkDBDispatcher::Init(const char *pszDatabaseName, const char *pszUserName, const char *pszPassword)
{
	TTaskMethod *pTaskMethod = g_TaskMethod;
	while (pTaskMethod->nWorkerID >= 0)
	{
		if (pTaskMethod->nGroupNameType == TMT_GN_INIT)
		{
			if (!m_bSinkInit)
			{
				pTaskMethod++;
				continue;
			}
		}
		if (pTaskMethod->nGroupNameType == TMT_GN_DOWN)
		{
			if (!m_bSinkDown)
			{
				pTaskMethod++;
				continue;
			}
		}

		PrepareTaskMethod(pTaskMethod);

		SINKDBWORKER *pWorker = GetWorker(pTaskMethod->nWorkerID);
		if (pWorker == NULL)
		{
			//nWorkerID为0表示同步工作线程
			pWorker = new SINKDBWORKER(pTaskMethod->nWorkerID, m_strWorkFilePath.c_str());
			m_mapWorker[pTaskMethod->nWorkerID] = pWorker;

			if (!pWorker->ConnectDB(pszDatabaseName, pszUserName, pszPassword))
				return false;
			//pWorker->EnableDbKeepAlive(DBCONN_KEEP_ALIVE_TIMEOUT);
		}
		//m_mapDispatch[pTaskMethod->pszObjectName] = pWorker;

		pTaskMethod++;
	}


	CWorkerMap::iterator i = m_mapWorker.begin();
	while (i != m_mapWorker.end())
	{
		(*i).second->InitMethod(g_TaskMethod);
		(*i).second->Create();
		i++;
	}
	return true;
}

void CSinkDBDispatcher::DelayMDBCallback(int nWorkerID, UF_INT8 nTaskNo, CFieldDescribe *pDescirbe, const void *pData, TSQLType nActionType)
{
	SINKDBWORKER *pWorker = GetWorker(nWorkerID);
	if (pWorker != NULL)
		pWorker->AssignTask(nTaskNo, pDescirbe->GetFieldName(), pDescirbe, pData, nActionType);
}

void CSinkDBDispatcher::DispatchAll(UF_INT8 nTaskNo, CFieldDescribe *pDescirbe, const void *pData)
{
	//剩余的必须全部做掉
	m_pDelayMDB->OnTimer(NULL, 0);

	CWorkerMap::iterator i = m_mapWorker.begin();
	while (i != m_mapWorker.end())
	{
		SINKDBWORKER *pWorker = (*i).second;
		pWorker->AssignTask(nTaskNo, pDescirbe->GetFieldName(), pDescirbe, pData, TSL_NOSQL);
		i++;
	}
}

void CSinkDBDispatcher::Dispatch(int nWorkerID, UF_INT8 nTaskNo, CFieldDescribe *pDescirbe, const void *pData, TSQLType nActionType)
{
	//	if (m_queueTask.empty())
	//		return;

		//SINKDBWORKER *pWorker = GetWorker(nWorkerID);
		//if (pWorker == NULL)
		//{
		//	//		if(m_bSinkInit)
		//	//			REPORT_EVENT(LOG_CRITICAL, "sinkdb", "Can't find worker for %s", pDescirbe->GetFieldName());
		//	return;
		//}

		// 	//如果是下场表的第一个内容，则需要删除大于，并且做完然后才能分发，避免多线程问题
		// 	CSingleMethodMap::iterator i = m_SingleMethodMap.find(pDescirbe->GetFieldName());
		// 	if (i != m_SingleMethodMap.end())
		// 	{
		// 		TSingleMethod *pMethod = i->second;
		// 		if(pMethod->bFirst)
		// 		{
		// 			char Datastruct[10000];
		// 			pDescirbe->getSingleField(Datastruct,pData);
		// 			pWorker->DClear(m_queueTask.back(), pDescirbe->GetFieldName(), pDescirbe, Datastruct);
		// 			pMethod->bFirst = false;
		// 		}
		// 	}

			////为了解决重启的情况下，下场表数据重复的问题
			//TSQLType myActionType = nActionType;
			////if(nActionType==TSL_INSERT)
			////	myActionType = TSL_INSUPD;

	if (!m_pDelayMDB->Dispatch(nWorkerID, nTaskNo, pDescirbe, pData, nActionType))
	{
		SINKDBWORKER *pWorker = GetWorker(nWorkerID);
		if (pWorker != NULL)
			pWorker->AssignTask(nTaskNo, pDescirbe->GetFieldName(), pDescirbe, pData, nActionType);
	}
}

SINKDBWORKER *CSinkDBDispatcher::GetWorker(int nWorkerID)
{
	CWorkerMap::iterator i = m_mapWorker.find(nWorkerID);
	if (i == m_mapWorker.end())
		return NULL;

	return (*i).second;
}

//void CSinkDBDispatcher::AssignTask(UF_INT8 nTaskNo)
//{
//	m_queueTask.push_back(nTaskNo);
//}

void CSinkDBDispatcher::OnTimer(const char* ZoneTime, UF_INT8 nTime)
{
	if (m_time == ZoneTime)
		return;
	else
		m_time = ZoneTime;

	m_pDelayMDB->OnTimer(ZoneTime, nTime);
	m_pDelayMDB->OnTimer(nTime);

	CMonitorIndex::handleOne();

	//if (m_queueTask.size() <= 0)
	//	return;

	//UF_INT8 nCompleteTaskNo = m_queueTask.back();
	//	UF_INT8 nCheckCompleteTaskNo = nCompleteTaskNo;

	UF_INT8 nCompleteTaskNo = DOUBLE_NaN;

	CWorkerMap::iterator i = m_mapWorker.begin();
	while (i != m_mapWorker.end())
	{
		SINKDBWORKER *pWork = (*i).second;
		UF_INT8 thisTaskNo = pWork->GetFinishCount();
		if (!pWork->IsEmpty())
			REPORT_EVENT(LOG_WARNING, "OnTimer", "Worker %d,no Empty,FinishCount %0.f", pWork->GetWorkID(), thisTaskNo);
		else
			REPORT_EVENT(LOG_WARNING, "OnTimer", "Worker %d,is Empty,FinishCount %0.f", pWork->GetWorkID(), pWork->GetFinishCount());
		if (thisTaskNo < nCompleteTaskNo)
			nCompleteTaskNo = thisTaskNo;
		i++;
	}

	if (nCompleteTaskNo > m_nCompleteTaskNo)
		m_nCompleteTaskNo = nCompleteTaskNo;

	REPORT_EVENT(LOG_WARNING, "OnTimer", "nCompleteTaskNo[%0.f]", ZoneTime, m_nCompleteTaskNo);

	//while (m_queueTask.size() > 0)
	//{
	//	if (m_queueTask.front() > m_nCompleteTaskNo)
	//		break;
	//	m_queueTask.pop_front();
	//}
}

void CSinkDBDispatcher::PrepareTaskMethod(TTaskMethod *pTaskMethod)
{
	if (pTaskMethod->nMethodType == TMT_PLSSP)
		return;

	int i = 0;
	int *pKeyPosition = pTaskMethod->keyPosition;

	//prepare key position
	CCSVParser csvParser(pTaskMethod->pszKeyName);
	do
	{
		char *pToken = csvParser.GetNextToken();
		if (pToken != NULL)
		{
			for (i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
			{
				if (strcmp(pToken, pTaskMethod->pDescirbe->GetMemberDesc(i)->szName) == 0)
				{
					*pKeyPosition = i + 1;
					pKeyPosition++;
					break;
				}
			}
			if (i == pTaskMethod->pDescirbe->GetMemberCount())
			{
				EMERGENCY_EXIT("Can't find key name");
			}
		}
	} while (csvParser.GetErrorCode() == CPE_HAS_NEXT);
	*pKeyPosition = 0;		//结束符

	char strSQL[MAX_TASK_COMMAND_LEN];

	//prepare insert statement
	strcpy(strSQL, "INSERT INTO ");
	switch (pTaskMethod->nGroupNameType)
	{
	case TMT_GN_INIT:
	{
		strcat(strSQL, m_sInitDB.c_str());
		break;
	}
	case TMT_GN_DOWN:
	{
		strcat(strSQL, m_sDownDB.c_str());
		break;
	}
	default:
	{

	}
	}
	strcat(strSQL, pTaskMethod->pszObjectName);
	strcat(strSQL, " (");
	for (i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
	{
		if (i != 0)
			strcat(strSQL, ", ");
		strcat(strSQL, pTaskMethod->pDescirbe->GetMemberDesc(i)->szName);
	}
	strcat(strSQL, ") VALUES (");
	strcpy(pTaskMethod->szInsertHead, strSQL);
	for (i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
	{
		if (i != 0)
			strcat(strSQL, ", ");
		sprintf(strSQL + strlen(strSQL), ":%d", i + 1);
	}
	strcat(strSQL, ")");
	strcpy(pTaskMethod->szInsertCommand, strSQL);
	REPORT_EVENT(LOG_CRITICAL, "PrepareTaskMethod", "SQL: %s\n", strSQL);

	//prepare update statement
	strcpy(strSQL, "UPDATE ");
	switch (pTaskMethod->nGroupNameType)
	{
	case TMT_GN_INIT:
	{
		strcat(strSQL, m_sInitDB.c_str());
		break;
	}
	case TMT_GN_DOWN:
	{
		strcat(strSQL, m_sDownDB.c_str());
		break;
	}
	default:
	{

	}
	}
	strcat(strSQL, pTaskMethod->pszObjectName);
	strcat(strSQL, " SET ");
	strcpy(pTaskMethod->szUpdateHead, strSQL);
	for (i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
	{
		if (i != 0)
		{
			strcat(strSQL, ", ");
		}
		strcat(strSQL, pTaskMethod->pDescirbe->GetMemberDesc(i)->szName);
		sprintf(strSQL + strlen(strSQL), " = :%d", i + 1);
	}
	strcat(strSQL, ", UpdateTimes=UpdateTimes+1 WHERE ");

	i = 0;
	pKeyPosition = pTaskMethod->keyPosition;
	while (*pKeyPosition > 0)
	{
		if (i != 0)
			strcat(strSQL, " AND ");

		sprintf(strSQL + strlen(strSQL), "%s=:%d",
			pTaskMethod->pDescirbe->GetMemberDesc(*pKeyPosition - 1)->szName,
			i + pTaskMethod->pDescirbe->GetMemberCount() + 1);
		i++;
		pKeyPosition++;
	}
	strcpy(pTaskMethod->szUpdateCommand, strSQL);
	REPORT_EVENT(LOG_CRITICAL, "PrepareTaskMethod", "SQL: %s\n", strSQL);

	//prepare delete statement
	strcpy(strSQL, "DELETE FROM ");
	switch (pTaskMethod->nGroupNameType)
	{
	case TMT_GN_INIT:
	{
		strcat(strSQL, m_sInitDB.c_str());
		break;
	}
	case TMT_GN_DOWN:
	{
		strcat(strSQL, m_sDownDB.c_str());
		break;
	}
	default:
	{

	}
	}
	strcat(strSQL, pTaskMethod->pszObjectName);
	strcat(strSQL, " WHERE ");
	strcpy(pTaskMethod->szDeleteHead, strSQL);
	i = 0;
	pKeyPosition = pTaskMethod->keyPosition;
	while (*pKeyPosition > 0)
	{
		if (i != 0)
			strcat(strSQL, " AND ");
		sprintf(strSQL + strlen(strSQL), "%s=:%d", pTaskMethod->pDescirbe->GetMemberDesc(*pKeyPosition - 1)->szName, i + 1);
		i++;
		pKeyPosition++;
	}
	strcpy(pTaskMethod->szDeleteCommand, strSQL);
	REPORT_EVENT(LOG_CRITICAL, "PrepareTaskMethod", "SQL: %s\n", strSQL);

	//prepare insupd statement
	strcpy(strSQL, "UPDATE ");
	switch (pTaskMethod->nGroupNameType)
	{
	case TMT_GN_INIT:
	{
		strcat(strSQL, m_sInitDB.c_str());
		break;
	}
	case TMT_GN_DOWN:
	{
		strcat(strSQL, m_sDownDB.c_str());
		break;
	}
	default:
	{

	}
	}
	strcat(strSQL, pTaskMethod->pszObjectName);
	strcat(strSQL, " SET ");
	strcpy(pTaskMethod->szInsUpdHead, strSQL);
	for (i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
	{
		if (i != 0)
		{
			strcat(strSQL, ", ");
		}
		strcat(strSQL, pTaskMethod->pDescirbe->GetMemberDesc(i)->szName);
		sprintf(strSQL + strlen(strSQL), " = :%d", i + 1);
	}
	strcat(strSQL, " WHERE ");

	i = 0;
	pKeyPosition = pTaskMethod->keyPosition;
	while (*pKeyPosition > 0)
	{
		if (i != 0)
			strcat(strSQL, " AND ");

		sprintf(strSQL + strlen(strSQL), "%s=:%d",
			pTaskMethod->pDescirbe->GetMemberDesc(*pKeyPosition - 1)->szName,
			i + pTaskMethod->pDescirbe->GetMemberCount() + 1);
		i++;
		pKeyPosition++;
	}
	strcpy(pTaskMethod->szInsUpdCommand, strSQL);
	REPORT_EVENT(LOG_CRITICAL, "PrepareTaskMethod", "SQL: %s\n", strSQL);
}
