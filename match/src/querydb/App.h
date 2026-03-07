#ifndef __CQueryDB_H__
#define __CQueryDB_H__

#include "OutFogServiceApi.h"
#include "Config.h"
#include "utility.h"
#include "OutFogService.h"
#include "version.h"
#include "FieldData.h"
#include "profile.h"
#include "ServiceResponser.h"
#include "CErrorEngine.h"
#include "errorDefine.h"
#include "KLineHandler.h"
#include "TradeHandler.h"
#include "FinishOrderHandler.h"
#include "AccountDetailHandler.h"
#include "SettleDetailHandler.h"
#include "FinishTriggerOrderHandler.h"

#define INI_FILE_NAME "querydb.ini"
#define NETWORK_FILE_NAME "network.ini"

typedef map<int, CLinkField> CLinkMap;
typedef map<string, CUserSessionField> CUserSessionMap;

void CmdLineArgError();

class CRspCallback :public COutFogServiceCallback
{
public:
	CRspCallback(COutFogServiceApi* pServiceApi)
	{
		m_pServiceApi = pServiceApi;
	}
	~CRspCallback() {}

	virtual bool RspField(const TOutFogMetaRef* pOutFogRspRef, const void* pField)
	{
		switch (pOutFogRspRef->Direction)
		{
		case OUTFOG_DIRECTION_TO_FRONT:
		case OUTFOG_DIRECTION_TO_OFFER:
		{
			return m_pServiceApi->ReqField(pOutFogRspRef, pField);
		}
		case OUTFOG_DIRECTION_TO_KERNEL:
		case OUTFOG_DIRECTION_TO_NULL:
		default:
		{
		}
		}
		return true;
	}

private:
	COutFogServiceApi* m_pServiceApi;
};


class CApp :public COutFogServiceCallback
{
public:
	CApp(int nKernelid);
	~CApp();

	void setServiceApi(COutFogServiceApi* pServiceApi)
	{
		m_RspCallback = new CRspCallback(pServiceApi);
		m_pServiceApi = pServiceApi;
		m_pServiceResponser = new CServiceResponser(m_RspCallback);

		printf("QueryDB: read config %s \n", INI_FILE_NAME);
		m_Ini.Open(INI_FILE_NAME);
	}

	virtual bool RspField(const TOutFogMetaRef* pOutFogRspRef, const void* pRspBuffer);

	bool CheckSession(CErrorEngine* pErrorEngine, TSessionRef* pSession, CLongIndexIDType& MemberID);
	void SubTopics(CLinkField linkField);

	CLinkMap m_LinkFrom;
	TOutFogMetaRef m_OutFogMetaRef;

private:

	CRspCallback* m_RspCallback;
	CServiceResponser* m_pServiceResponser;
	COutFogServiceApi* m_pServiceApi;
	char m_streamBuf[4096];
	TIniFile m_Ini;
	CErrorEngine* m_pErrorEngine;
	int nRecvTNo;
	TSessionRef sessionRef;
	string m_sGateWayKey;

	CDbHelper* m_pDbHelper;
	CKLineHandler* m_pKLineHandler;
	CTradeHandler* m_pTradeHandler;
	CFinishOrderHandler* m_pFinishOrderHandler;
	CFinishTriggerOrderHandler* m_pFinishTriggerOrderHandler;
	CSettleDetailHandler* m_pSettleDetailHandler;
	CAccountDetailHandler* m_pAccountDetailHandler;

	CUserSessionMap m_UserSessionAPPIDMap;
	CUserSessionMap m_UserSessionTokenMap;
};

#endif
