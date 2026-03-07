#ifndef AFX_SINKDBDISPATCH_H__
#define AFX_SINKDBDISPATCH_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "SinkDBCSVWorker.h"
#include "monitorIndex.h"
#include "TableSync.h"
#include "DelayMDB.h"

//typedef map<const char *, TSingleMethod *, ltstr>  CSingleMethodMap;

#define SINKDBWORKER CCSVWorker

class CSinkDBDispatcher :public CDelayMDBCallback
{
public:
	CSinkDBDispatcher();
	virtual ~CSinkDBDispatcher();

	bool Init(const char *pszDatabaseName, const char *pszUserName, const char *pszPassword);
	//void AssignTask(UF_INT8 nTaskNo);
	void Dispatch(int nWorkerID, UF_INT8 nTaskNo, CFieldDescribe *pDescirbe, const void *pData, TSQLType nActionType);

	void DelayMDBCallback(int nWorkerID, UF_INT8 nTaskNo, CFieldDescribe *pDescirbe, const void *pData, TSQLType nActionType);

	virtual void OnTimer(const char* ZoneTime, UF_INT8 nTime);

	void DispatchAll(UF_INT8 nTaskNo, CFieldDescribe *pDescirbe, const void *pData);

private:
	void PrepareTaskMethod(TTaskMethod *pTaskMethod);

	SINKDBWORKER *GetWorker(int nWorkerID);

private:
	typedef map<int, SINKDBWORKER *> CWorkerMap;
	CWorkerMap m_mapWorker;

	//	typedef map<const char *, CTMDBWorker *, ltstr> CDispatchMap;
	//	CDispatchMap m_mapDispatch;

	//typedef deque<UF_INT8> CTaskQueue;
	//CTaskQueue m_queueTask;

	//	int m_nCurrTaskNo;

	UF_INT8 m_nCompleteTaskNo;

	string m_strWorkFilePath;
	string m_time;
	string m_sInitDB;
	string m_sDownDB;
	bool m_bSinkInit;
	bool m_bSinkDown;

	//CSingleMethodMap m_SingleMethodMap;

	CDelayMDB *m_pDelayMDB;
};

#endif
