// TMDBWorker.cpp: implementation of the CTMDBWorker class.
//////////////////////////////////////////////////////////////////////
#include "public.h"
#include "SinkDBCSVWorker.h"
#include "DbConnection.h"
#include "CSVParser.h"
#include "CLogger.h"
#include "monitorIndex.h"
#include "FieldDescribe.h"
#include "FieldData.h"

static char* ActionTypeName[] =
{
	"ins",
	"upd",
	"del",
	"nosql"
};

CCSVWorker::CCSVWorker(int nWorkerID, const char* pszWorkFilePath)
{
	m_nWorkerID = nWorkerID;
	m_pDbConnection = new CDbConnection;
	m_pSACommand = new SACommand(m_pDbConnection);
	m_pWorkData = new char[2048];
	m_fpTmdbAction = NULL;
	FPOS_SET(m_fposWrite, 0);

	m_strWorkFileName = pszWorkFilePath;

	makedir(m_strWorkFileName.c_str());

	char filename[40];
	sprintf(filename, "%d.dbcsv", m_nWorkerID);
	m_strWorkFileName += filename;

	//	m_nConnectTimerCount = 0;

	//	m_nDbKeepAliveTimeout = 0;

	m_nSeepTime = 0;

	m_tLastDbActionTime = time(NULL);

	m_nFinishCount = 0;

	Reset();
}

CCSVWorker::~CCSVWorker()
{
	if (m_fpTmdbAction != NULL)
	{
		fclose(m_fpTmdbAction);
	}
	CSACommandCache::iterator i = m_cacheSACommand.begin();
	for (; i != m_cacheSACommand.end(); i++)
	{
		delete (*i).second;
	}
	m_cacheSACommand.clear();
	delete m_pSACommand;
	delete m_pDbConnection;
	delete m_pWorkData;
}

void CCSVWorker::Run()
{
	while (true)
	{
		SleepMs(TMDB_COMMIT_INTERVAL);
		Work();
		//		DbKeepAlive();
	}
}

void CCSVWorker::InitMethod(TTaskMethod* pTaskMethod)
{
	while (pTaskMethod->nWorkerID >= 0)
	{
		m_mapTaskMethod[pTaskMethod->pszFieldName] = pTaskMethod;
		pTaskMethod++;
	}
}

bool CCSVWorker::ConnectDB(const char* pszDatabaseName, const char* pszUserName, const char* pszPassword)
{
	return m_pDbConnection->ConnectDB(pszDatabaseName, pszUserName, pszPassword);
}

void CCSVWorker::Reset()
{
	m_mutexTask.Lock();
	if (m_fpTmdbAction != NULL)
	{
		fclose(m_fpTmdbAction);
	}
	m_fpTmdbAction = mfopen(m_strWorkFileName.c_str(), "w+b");
	m_queueTask.clear();
	m_mutexTask.UnLock();
}

void CCSVWorker::AssignTask(UF_INT8 nTaskNo, const char* pszFieldName, CFieldDescribe* pDescirbe, const void* pData, TSQLType nActionType)
{
	//BIZ_STATUS_LOG2("CTMDBWorker::AssignTask %d %s\n", nTaskNo, pszObjectName);

	TSQLAction tmdbAction;
	tmdbAction.nTaskNo = nTaskNo;
	tmdbAction.nActionType = nActionType;
	tmdbAction.pszFieldName = pszFieldName;
	//	tmdbAction.pData = m_listTaskData.PushBack((void *)pData, pDescirbe->GetStructSize());
	tmdbAction.pDescirbe = pDescirbe;
	tmdbAction.fposData = m_fposWrite;

	m_mutexTask.Lock();
	int nSleepTime = SaveAction(&tmdbAction, (void*)pData, m_fpTmdbAction);
	m_queueTask.push_back(tmdbAction);
	m_mutexTask.UnLock();

	//中间文件需要到一定大小自动清理，避免文件过大
	//如果Worker一直无法处理完成，Scheduler一直导入数据库，就会一直无法清理
	//这里sleep一下，让worker清理完成，时间由SaveAction的返回值决定
	if (nSleepTime > 0)
	{
		PRINT_TO_STD("AssignTask,WorkNo[%d] File is Big,Sleep[%d]ms to Wait!", m_nWorkerID, nSleepTime);
		SLEEP(nSleepTime);
	}
}

int CCSVWorker::SaveAction(TSQLAction* pAction, void* pData, FILE* fp)
{
	char buf[TMDB_ACTION_MAX_LENGTH];

	sprintf(buf, "\"%0.f\",\"", pAction->nTaskNo);
	strcat(buf, ActionTypeName[pAction->nActionType]);
	strcat(buf, "\",\"");
	strcat(buf, pAction->pszFieldName);
	strcat(buf, "\"");

	if (pAction->pDescirbe != NULL)
	{
		for (int i = 0; i < pAction->pDescirbe->GetMemberCount(); i++)
		{
			TMemberDesc* pMemberDesc = pAction->pDescirbe->GetMemberDesc(i);
			strcat(buf, ",\"");
			strcat(buf, pMemberDesc->szName);
			strcat(buf, "\",\"");
			char strTemp[2] = { '\0', '\0' };
			//char* pMemberData = (char*)pData + pMemberDesc->nStreamOffset;
			char* pMemberData = (char*)pData + pMemberDesc->nStructOffset;
			switch (pMemberDesc->nType)
			{
			case FT_DWORD:					//long
			{
				//char sTmpStruct[4];
				//ChangeEndianCopy4(sTmpStruct, pMemberData);
				//if (*((int*)sTmpStruct) != INT_32_MAX)
				//	sprintf(buf + strlen(buf), "%ld", *((int*)sTmpStruct));
				if (*((int*)pMemberData) != INT_32_MAX)
					sprintf(buf + strlen(buf), "%ld", *((int*)pMemberData));
				break;
			}
			case FT_REAL8:						//double
			{
				//char sTmpStruct[8];
				//ChangeEndianCopy8(sTmpStruct, pMemberData);
				//string value = doubleToStringFormat(*((double*)sTmpStruct));
				string value = doubleToStringFormat(*((double*)pMemberData));
				if (value == "inf" || value == "nan" || value == "-nan" || value == "-inf")
					value.clear();
				strcat(buf, value.c_str());
				break;
			}
			case FT_LONG:						//double
			{
				//char sTmpStruct[8];
				//ChangeEndianCopy8(sTmpStruct, pMemberData);
				//string value = LargeNumberToString(*((UF_INT8*)sTmpStruct));
				string value = LargeNumberToString(*((UF_INT8*)pMemberData));
				if (value == "inf" || value == "nan" || value == "-nan" || value == "-inf")
					value.clear();
				strcat(buf, value.c_str());
				break;
			}
			case FT_BYTE:
			{
				if (pMemberDesc->nSize == 1)
				{
					strTemp[0] = *pMemberData;
					strcat(buf, strTemp);
				}
				else
				{
					strcat(buf, pMemberData);
				}
				break;
			}
			}
			strcat(buf, "\"");
		}
	}
	strcat(buf, "\n");

	int nSleepTime = 0;

	//回收文件
	if (FPOS_GET(m_fposWrite) > TMDB_WF_MAXSIZE)
	{
		if (m_queueTask.empty() && m_actionUncommitted.empty())
		{
			fclose(m_fpTmdbAction);
			m_fpTmdbAction = mfopen(m_strWorkFileName.c_str(), "w+b");
			fp = m_fpTmdbAction;
			FPOS_SET(m_fposWrite, 0);
			pAction->fposData = m_fposWrite;
		}
		else
		{
			nSleepTime = FPOS_GET(m_fposWrite) - TMDB_WF_MAXSIZE;
		}
	}

	fsetpos(fp, &m_fposWrite);

	int len = strlen(buf);
	FPOS_SET(m_fposWrite, FPOS_GET(m_fposWrite) + len);

	fwrite(buf, 1, len, fp);
	fflush(fp);
	return nSleepTime;
}

void CCSVWorker::LoadAction(TSQLAction* pAction, void* pData, FILE* fp)
{
	if (pAction->pDescirbe == NULL)
		return;

	char buf[TMDB_ACTION_MAX_LENGTH];
	fsetpos(fp, &pAction->fposData);
	fread(buf, 1, TMDB_ACTION_MAX_LENGTH, fp);

	CCSVParser parser(buf);
	for (int i = 0; ; i++)
	{
		char* pToken = parser.GetNextToken();
		if (pToken == NULL)
			break;
		if (i < 3 || i % 2 != 0)
			continue;
		int nMemberPos = (i - 3) / 2;
		TMemberDesc* pMemberDesc = pAction->pDescirbe->GetMemberDesc(nMemberPos);
		char* pMemberData = (char*)pData + pMemberDesc->nStructOffset;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:						//long
			if (*pToken == '\0')
				*((int*)pMemberData) = INT_32_MAX;
			else
				*((int*)pMemberData) = atoi(pToken);
			break;
		case FT_REAL8:						//double
			if (*pToken == '\0')
				*((double*)pMemberData) = DOUBLE_NaN;
			else
				*((double*)pMemberData) = atof(pToken);
			break;
		case FT_LONG:						//double
			if (*pToken == '\0')
				*((UF_INT8*)pMemberData) = DOUBLE_NaN;
			else
				*((UF_INT8*)pMemberData) = atof(pToken);
			break;
		case FT_BYTE:
			if (pMemberDesc->nSize == 1)
				*pMemberData = *pToken;
			else
				strcpy(pMemberData, pToken);
			break;
		}

		if (parser.GetErrorCode() != CPE_HAS_NEXT)
			break;
	}
}

void CCSVWorker::Work()
{
	if (m_queueTask.size() == 0 && m_actionUncommitted.size() == 0)
	{
		SleepMs(100);
		return;
	}
		
	if (!m_pDbConnection->KeepAlive())
	{
		if (m_pDbConnection->ConnectDB())
		{
			PRINT_TO_STD("Connect DB %d", m_nWorkerID);
			//只要出现失败连接，不管是否重新连接成功，尝试RollBack之前没有提交的
			RollbackActions();
		}
		else
		{
			PRINT_TO_STD("SinkDBWorker [%d] Connect DB Error", m_nWorkerID);
			SLEEP_SECONDS(1);
			//只要出现失败连接，不管是否重新连接成功，尝试RollBack之前没有提交的
			RollbackActions();
			return;
		}
	}

	m_nSeepTime++;
	if (m_nSeepTime > 200)
	{
		CommitActions();
		m_nSeepTime = 0;
	}

	while (m_queueTask.size() > 0)
	{
		m_mutexTask.Lock();

		TSQLAction tmdbAction = m_queueTask.front();

		//先将任务从队列中取到本地，就可以将任务从队列中去除，
		//防止过长时间锁任务队列
		m_queueTask.pop_front();

		LoadAction(&tmdbAction, m_pWorkData, m_fpTmdbAction);

		//		memcpy(m_pWorkData, tmdbAction.pData, tmdbAction.pDescirbe->GetStructSize());
		//		m_listTaskData.PopFront(tmdbAction.pDescirbe->GetStructSize());

		m_actionUncommitted.push_back(tmdbAction);
		m_mutexTask.UnLock();

		//BIZ_STATUS_LOG3("DbOperation nTaskNo=%d objName=%s ACount=%d\n", tmdbAction.nTaskNo, tmdbAction.pszObjectName, (*tmdbAction.pAccomplishRef));
		if (tmdbAction.nActionType != TSL_NOSQL)
		{
			int r = DbOperation(tmdbAction.pszFieldName, tmdbAction.pDescirbe, tmdbAction.nActionType, tmdbAction.nTaskNo);
			if (r <= 0)
				break;
		}

		if (m_actionUncommitted.size() > TMDB_COMMIT_VOLUME)
			CommitActions();
		m_tLastDbActionTime = time(NULL);
	}
}

int CCSVWorker::DbOperation(const char* pszFieldName, CFieldDescribe* pDescirbe, TSQLType nActionType, UF_INT8 nTaskNo)
{
	TTaskMethod* pTaskMethod = FindMethod(pszFieldName);

	if (pTaskMethod == NULL)
		return -1;

	int ret = 0;
	for (int nRetryTimes = 0; nRetryTimes < 5; nRetryTimes++)		//重做5次
	{
		try
		{
			//第二次尝试用不同的方式来处理
			if (nRetryTimes == 1)
			{
				switch (pTaskMethod->nMethodType)
				{
				case TMT_TABLE:
				{
					switch (nActionType)
					{
					case TSL_INSERT:
						ret = DbTableOperation(pDescirbe, pTaskMethod->szInsUpdCommand, pTaskMethod->keyPosition, TSL_UPDATE);
						break;
					case TSL_UPDATE:
						ret = DbTableOperation(pDescirbe, pTaskMethod->szInsertCommand, pTaskMethod->keyPosition, TSL_INSERT);
						break;
					case TSL_DELETE://删除失败应该是没有数据删除，可以不需要再做了
						break;
					default:
						EMERGENCY_EXIT("Invalid tdmb action");
					}
					break;
				}
				case TMT_PLSSP:
				{
					ret = DbSpOperation(pDescirbe, pTaskMethod->pszKeyName, nActionType);
					break;
				}
				default:
					EMERGENCY_EXIT("Invalid db method type");
				}
				//PRINT_TO_STD("nRetryTimes[%d]=%d", nRetryTimes, ret);
				//第二次不需要纪要影响的行情
				ret = 1;
				nRetryTimes = 5;					//成功了不需要重做
			}
			else
			{
				switch (pTaskMethod->nMethodType)
				{
				case TMT_TABLE:
				{
					switch (nActionType)
					{
					case TSL_INSERT:
						ret = DbTableOperation(pDescirbe, pTaskMethod->szInsertCommand, pTaskMethod->keyPosition, nActionType);
						break;
					case TSL_UPDATE:
						ret = DbTableOperation(pDescirbe, pTaskMethod->szUpdateCommand, pTaskMethod->keyPosition, nActionType);
						break;
					case TSL_DELETE:
						ret = DbTableOperation(pDescirbe, pTaskMethod->szDeleteCommand, pTaskMethod->keyPosition, nActionType);
						break;
					default:
						EMERGENCY_EXIT("Invalid db action");
					}
					break;
				}
				case TMT_PLSSP:
				{
					ret = DbSpOperation(pDescirbe, pTaskMethod->pszKeyName, nActionType);
					break;
				}
				default:
					EMERGENCY_EXIT("Invalid db method type");
				}
				//PRINT_TO_STD("nRetryTimes[%d]=%d", nRetryTimes, ret);
				//影响函数为0，说明没有做动作需要重来
				if (ret == 0)
					continue;
				else
					nRetryTimes = 5;					//成功了不需要重做
			}
		}
		catch (SAException& x)
		{
			//if (nRetryTimes > 2)
			//{
			//ret = -2;
			PRINT_TO_STD("DBError, TaskNo=%.0f Object=%s Code=%d Message=%s", nTaskNo, pszFieldName, x.ErrNativeCode(), (const char*)x.ErrText());
			pDescirbe->StructToJson(m_sJson, m_pWorkData);
			PRINT_TO_STD("DBError, Data=%s", m_sJson);
			SLEEP_SECONDS(nRetryTimes);
			//}
		}
	}
	//if (ret == 0)
	//{
	//	//BIZ_OPERATION_LOG2("Database Operation Success, Object=%s ActionType=%d\n",pszObjectName, nActionType);
	//}
	//else
	//{
	//	//PRINT_TO_STD("Database Operation Failed, Object=%s ActionType=%d ret=%d\n", pTaskMethod->pszObjectName, nActionType, ret);
	//}
	return ret;
}

TTaskMethod* CCSVWorker::FindMethod(const char* pszObjectName)
{
	CTaskMethodMap::iterator i = m_mapTaskMethod.find(pszObjectName);
	if (i == m_mapTaskMethod.end())
	{
		return NULL;
	}
	return (*i).second;
}

int CCSVWorker::DbTableOperation(CFieldDescribe* pDescirbe, const char* pszSQLCommand, int* pKeyPosition, TSQLType nActionType)
{
	SACommand* pSACommand = GetSACommand(pszSQLCommand);

	assert(pSACommand != NULL);

	int nParamPosition = 1;
	if (nActionType == TSL_INSERT || nActionType == TSL_UPDATE)
	{
		for (int i = 0; i < pDescirbe->GetMemberCount(); i++)
		{
			BindParam(pDescirbe->GetMemberDesc(i), nParamPosition, pSACommand);
			nParamPosition++;
		}
	}

	if (nActionType == TSL_DELETE || nActionType == TSL_UPDATE)
	{
		int* pCurrPosition = pKeyPosition;
		while (*pCurrPosition > 0)
		{
			BindParam(pDescirbe->GetMemberDesc(*pCurrPosition - 1), nParamPosition, pSACommand);
			pCurrPosition++;
			nParamPosition++;
		}
	}

	pSACommand->Execute();
	//	m_pDbConnection->Commit();

	return pSACommand->RowsAffected();
}

int CCSVWorker::DbSpOperation(CFieldDescribe* pDescirbe, const char* pszKeyName, TSQLType nActionType)
{
	m_pSACommand->setCommandText(pszKeyName);//, SA_CmdStoredProc);

	m_pSACommand->Param("i_ActionType").setAsLong() = nActionType;

	char szTemp[1000] = "i_";
	for (int i = 0; i < pDescirbe->GetMemberCount(); i++)
	{
		strcpy(szTemp + 2, pDescirbe->GetMemberDesc(i)->szName);
		BindParam(pDescirbe->GetMemberDesc(i), szTemp, m_pSACommand);
	}

	m_pSACommand->Execute();

	m_pDbConnection->Commit();

	//需要放回影响的行数
	return m_pSACommand->Param("o_ReturnNo").asLong();
}

void CCSVWorker::BindParam(TMemberDesc* pMemberDesc, int nParamPosition, SACommand* pSACommand)
{
	char strTemp[2] = { '\0', '\0' };
	char* pMemberData = m_pWorkData + pMemberDesc->nStructOffset;
	switch (pMemberDesc->nType)
	{
	case FT_DWORD:						//int
		if (*((int*)pMemberData) == INT_32_MAX)
			pSACommand->Param(nParamPosition).setAsNull();
		else
			pSACommand->Param(nParamPosition).setAsLong() = *((int*)pMemberData);
		break;
	case FT_REAL8:						//double
		if (*((double*)pMemberData) == DOUBLE_NaN)
			pSACommand->Param(nParamPosition).setAsNull();
		else
			//pSACommand->Param(nParamPosition).setAsDouble() = *((REAL8 *)pMemberData);
			pSACommand->Param(nParamPosition).setAsString() = doubleToString(*((double*)pMemberData), pMemberDesc->nPrecision).c_str();
		break;
	case FT_LONG:						//double
		if (*((UF_INT8*)pMemberData) == DOUBLE_NaN)
			pSACommand->Param(nParamPosition).setAsNull();
		else
			//pSACommand->Param(nParamPosition).setAsDouble() = *((REAL8 *)pMemberData);
			pSACommand->Param(nParamPosition).setAsString() = LargeNumberToString(*((UF_INT8*)pMemberData)).c_str();
		break;
	case FT_BYTE:
		if (pMemberDesc->nSize == 1)
		{
			strTemp[0] = *pMemberData;
			pSACommand->Param(nParamPosition).setAsString() = strTemp;
		}
		else
		{
			pSACommand->Param(nParamPosition).setAsString() = pMemberData;
		}
		break;
	default:
	{

	}
	}
}

void CCSVWorker::BindParam(TMemberDesc* pMemberDesc, const char* pszParamName, SACommand* pSACommand)
{
	char strTemp[2] = { '\0', '\0' };
	char* pMemberData = m_pWorkData + pMemberDesc->nStructOffset;
	switch (pMemberDesc->nType) {
	case FT_DWORD:						//int
		if (*((int*)pMemberData) == INT_32_MAX)
			pSACommand->Param(pszParamName).setAsNull();
		else
			pSACommand->Param(pszParamName).setAsLong() = *((int*)pMemberData);
		break;
	case FT_REAL8:						//double
		if (*((double*)pMemberData) == DOUBLE_NaN)
			pSACommand->Param(pszParamName).setAsNull();
		else
			pSACommand->Param(pszParamName).setAsDouble() = *((double*)pMemberData);
		break;
	case FT_LONG:						//double
		if (*((UF_INT8*)pMemberData) == DOUBLE_NaN)
			pSACommand->Param(pszParamName).setAsNull();
		else
			pSACommand->Param(pszParamName).setAsDouble() = *((UF_INT8*)pMemberData);
		break;
	case FT_BYTE:
		if (pMemberDesc->nSize == 1)
		{
			strTemp[0] = *pMemberData;
			pSACommand->Param(pszParamName).setAsString() = strTemp;
		}
		else
		{
			pSACommand->Param(pszParamName).setAsString() = pMemberData;
		}
		break;
	}
}

SACommand* CCSVWorker::GetSACommand(const char* pszSQL)
{
	SACommand* pSACommand = NULL;

	CSACommandCache::iterator i = m_cacheSACommand.find(pszSQL);

	if (i == m_cacheSACommand.end())
	{
		pSACommand = new SACommand(m_pDbConnection);
		pSACommand->setCommandText(pszSQL, SA_CmdSQLStmt);
		m_cacheSACommand[pszSQL] = pSACommand;
	}
	else
	{
		pSACommand = (*i).second;
	}

	return pSACommand;
}

int CCSVWorker::CommitActions()
{
	if (m_actionUncommitted.size() <= 0)
		return 0;

	//	if (m_actionUncommitted.back().nActionType != TA_COMMIT)
	//	{
	//		return;
	//	}

	try
	{
		m_pDbConnection->Commit();
		//REPORT_EVENT(LOG_CRITICAL, "Database", "CTMDBWorker %d Commit !",m_nWorkerID);
		m_nFinishCount = m_actionUncommitted.back().nTaskNo;
		m_actionUncommitted.clear();
		return 0;
	}
	catch (SAException& x)
	{
		PRINT_TO_STD("Database Operation Exception, Commit error, Message=%s", (const char*)x.ErrText());
		return -1;
	}
}

void CCSVWorker::RollbackActions()
{
	try
	{
		m_pDbConnection->Rollback();
	}
	catch (SAException& x)
	{
		PRINT_TO_STD("Database Operation Exception, Rollback error, Message=%s", (const char*)x.ErrText());
	}

	m_mutexTask.Lock();
	vector<TSQLAction>::reverse_iterator i = m_actionUncommitted.rbegin();
	for (; i != m_actionUncommitted.rend(); i++)
	{
		m_queueTask.push_front(*i);
	}
	m_actionUncommitted.clear();
	m_mutexTask.UnLock();

	PRINT_TO_STD("SinkDB::RollbackActions [%d]", m_nWorkerID);
}

//void CTMDBWorker::EnableDbKeepAlive(int nTimeout)
//{
//	m_nDbKeepAliveTimeout = nTimeout;
//}

//void CTMDBWorker::DbKeepAlive()
//{
//	time_t tNow = time(NULL);
//	if (tNow - m_tLastDbActionTime > m_nDbKeepAliveTimeout)
//	{
//		m_pDbConnection->KeepAlive();
//		m_tLastDbActionTime = time(NULL);
//	}
//}

bool CCSVWorker::IsEmpty()
{
	if (m_queueTask.empty() && m_actionUncommitted.empty())
		return true;
	else
		return false;
}
