#ifndef _DELAYMDB_H
#define _DELAYMDB_H

#include "TableSync.h"
#include "Config.h"
#include "OutFogService.h"

#define INI_FILE_NAME "sinkdb.ini"

struct DelayAccount
{
	UF_INT8  nTaskNo;
	CAccountField Data;
};
struct DelayOrder
{
	UF_INT8  nTaskNo;
	COrderField Data;
};
struct DelayPosition
{
	UF_INT8  nTaskNo;
	CPositionField Data;
};

typedef map<string, DelayAccount> CDelayAccountMap;
typedef map<string, DelayOrder> CDelayOrderMap;
typedef map<string, DelayPosition> CDelayPositionMap;

typedef map<string, int> CFinishOrderMap;

class CDelayMDBCallback
{
public:
	virtual void DelayMDBCallback(int nWorkerID, UF_INT8 nTaskNo, CFieldDescribe *pDescirbe, const void *pData, TSQLType nActionType) = 0;
};

class CDelayMDB
{
public:
	CDelayMDB(CDelayMDBCallback *pSinkDBDispatcher,bool bSinkInit,bool bSinkDown)
	{
		m_pSinkDBDispatcher = pSinkDBDispatcher;
		m_nTime = 0;
		m_nTime2 = 0;
		m_nDelaySeconds = GetConfigInt(INI_FILE_NAME, "DelaySeconds");
		if (m_nDelaySeconds < 0)
			m_nDelaySeconds = 0;
		PRINT_TO_STD("DelaySeconds:%d", m_nFinishOrderCount);

		m_nFinishOrderCount = GetConfigInt(INI_FILE_NAME, "FinishOrderCount");
		if (m_nFinishOrderCount <=0)
			m_nFinishOrderCount = 5;
		PRINT_TO_STD("FinishOrderCount:%d", m_nFinishOrderCount);

		m_nFinishOrderSeconds = GetConfigInt(INI_FILE_NAME, "FinishOrderSeconds");
		if (m_nFinishOrderSeconds <= 0)
			m_nFinishOrderSeconds = 2;
		PRINT_TO_STD("FinishOrderSeconds:%d", m_nFinishOrderSeconds);

		m_bSinkInit = bSinkInit;
		m_bSinkDown = bSinkDown;
	}
	virtual ~CDelayMDB() {};
	bool Dispatch(int nWorkerID, UF_INT8 nTaskNo, CFieldDescribe *pDescirbe, const void *pData, TSQLType nActionType);
	void OnTimer(const char* ZoneTime, UF_INT8 nTime);
	void OnTimer(UF_INT8 nTime);

private:

	CFinishOrderMap m_FinishOrder;

	//Account
	CDelayAccountMap m_InsertAccount;
	CDelayAccountMap m_UpdateAccount;
	CDelayAccountMap m_DeleteAccount;

	//Order
	CDelayOrderMap m_InsertOrder;
	CDelayOrderMap m_UpdateOrder;
	CDelayOrderMap m_DeleteOrder;

	//Position
	CDelayPositionMap m_InsertPosition;
	CDelayPositionMap m_UpdatePosition;
	CDelayPositionMap m_DeletePosition;

	CDelayMDBCallback *m_pSinkDBDispatcher;
	UF_INT8 m_nTime;
	UF_INT8 m_nTime2;
	bool m_bSinkInit;
	bool m_bSinkDown;
	int m_nDelaySeconds;
	int m_nFinishOrderSeconds;
	int m_nFinishOrderCount;
};

#endif
