#ifndef _TSQLWORKER_H__
#define _TSQLWORKER_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "Thread.h"
#include "FieldDescribe.h"
//#include "CacheList.h"
#include "Mutex.h"
#include "TableSync.h"

class CDbConnection;
class SACommand;

class CSQLWorker : public CThread
{
public:
	CSQLWorker(int nWorkerID, const char *pszWorkFilePath);
	virtual ~CSQLWorker();

	void InitMethod(TTaskMethod *pTaskMethod);
	bool ConnectDB(const char *pszDatabaseName, const char *pszUserName, const char *pszPassword);
	void AssignTask(UF_INT8 nTaskNo, const char *pszFieldName, CFieldDescribe *pDescirbe, const void *pData, TSQLType nActionType);

	void Reset();
	volatile UF_INT8 GetFinishCount();
	void EnableDbKeepAlive(int nTimeout);
	bool IsEmpty();
	int GetWorkID();
	TTaskMethod *FindMethod(const char *pszObjectName);

private:
	virtual void Run();
	void Work();
	int DbOperation(const char *pszFieldName, CFieldDescribe *pDescirbe, TSQLType nAction, UF_INT8 nTaskNo);

	int DbTableOperation(CFieldDescribe *pDescirbe, const char *pszSQLCommand, int *pKeyPosition, TSQLType nActionType);
	int DbSpOperation(CFieldDescribe *pDescirbe, const char *pszKeyName, TSQLType nActionType);

	void BindParam(TMemberDesc *pMemberDesc, int nParamPosition, SACommand *pCommand);
	void BindParam(TMemberDesc *pMemberDesc, const char *pszParamName, SACommand *pSACommand);

	int SaveAction(TSQLAction *pAction, void *pData, FILE *fp);
	void LoadAction(TSQLAction *pAction, void *pData, FILE *fp);

	SACommand *GetSACommand(const char *pszSQL);

	int CommitActions();
	void RollbackActions();
	//void DbKeepAlive();

	int m_nWorkerID;
	CDbConnection *m_pDbConnection;
	SACommand *m_pSACommand;

	//正在执行的SACommand的主键
	char m_CommandKey[1024];

	//	CCacheList m_listTaskData;

	typedef deque<TSQLAction> CTaskQueue;
	CTaskQueue m_queueTask;

	typedef map<const char *, TTaskMethod *, ltstr>  CTaskMethodMap;
	CTaskMethodMap m_mapTaskMethod;

	CMutex m_mutexTask;
	char *m_pWorkData;

	FILE *m_fpTmdbAction;
	fpos_t m_fposWrite;
	string m_strWorkFileName;

	typedef map<const char *, SACommand *> CSACommandCache;
	CSACommandCache m_cacheSACommand;

	vector<TSQLAction> m_actionUncommitted;

	//int m_nConnectTimerCount;

	//int m_nDbKeepAliveTimeout;
	time_t m_tLastDbActionTime;

	int m_nSeepTime;

	volatile UF_INT8 m_nFinishCount;
};

inline volatile UF_INT8 CSQLWorker::GetFinishCount()
{
	return m_nFinishCount;
}

inline int CSQLWorker::GetWorkID()
{
	return m_nWorkerID;
}
#endif
