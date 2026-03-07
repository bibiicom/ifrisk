#ifndef _CSVWORKER_H__
#define _CSVWORKER_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "Thread.h"
#include "FieldDescribe.h"
#include "Mutex.h"
#include "TableSync.h"
#include "OutFogServiceApi.h"

class CDbConnection;
class SACommand;

class CCSVWorker : public CThread
{
public:
	CCSVWorker(int nWorkerID, const char *pszWorkFilePath);
	virtual ~CCSVWorker();

	void InitMethod(TTaskMethod *pTaskMethod);	
	bool ConnectDB(const char *pszDatabaseName, const char *pszUserName, const char *pszPassword);
	void AssignTask(UF_INT8 nTaskNo, const char *pszFieldName, CFieldDescribe *pDescirbe, const void *pData, TSQLType nActionType);
	
	void Reset();
	volatile UF_INT8 GetFinishCount();
	void EnableDbKeepAlive(int nTimeout);
	bool IsEmpty();
	int GetWorkID();
	//启动之前删除之前插入的数据，避免重复插入
	bool DClear(UF_INT8 nTaskNo, const char *pszFieldName, CFieldDescribe *pDescirbe, char *pData);
	TTaskMethod *FindMethod(const char *pszObjectName);

private:
	virtual void Run();
	void Work(); 
	int DbOperation(const char *pszFieldName, CFieldDescribe *pDescirbe, TSQLType nActionType, UF_INT8 nTaskNo);

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
	char m_sJson[4 * OUTFOG_MAX_PACKAGE_SIZE];
};

inline volatile UF_INT8 CCSVWorker::GetFinishCount()
{
	return m_nFinishCount;
}

inline int CCSVWorker::GetWorkID()
{
	return m_nWorkerID;
}
#endif
