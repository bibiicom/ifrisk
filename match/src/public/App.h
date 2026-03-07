#ifndef __CPublish_H__
#define __CPublish_H__

#include "OutFogServiceApi.h"
#include "Config.h"
#include "utility.h"
#include "OutFogService.h"
#include "version.h"
#include "FieldData.h"
#include "profile.h"
#include "mdb.h"
#include "ServiceResponser.h"
#include "MarketDataService.h"
#include "MarketDataTimer.h"
#include "MarketDataTriggers.h"
#include "DbHelper.h"
#include "LoginService.h"

#define INI_FILE_NAME "public.ini"
#define NETWORK_FILE_NAME "network.ini"

typedef map<int, CLinkField> CLinkMap;
typedef map<int, int> SubMap;

void CmdLineArgError();

class CRspCallback :public COutFogServiceCallback
{
public:
	CRspCallback(COutFogServiceApi* pServiceApi)
	{
		m_pServiceApi = pServiceApi;
	}
	~CRspCallback() {}

	virtual bool RspField(const TOutFogMetaRef* pOutFogRspRef, const void *pField)
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


class CMyEngine :public COutFogServiceCallback
{
public:
	CMyEngine();
	~CMyEngine();

	void setServiceApi(COutFogServiceApi* pServiceApi)
	{
		m_RspCallback = new CRspCallback(pServiceApi);
		m_pServiceApi = pServiceApi;
		m_pServiceResponser = new CServiceResponser(m_RspCallback);
		m_pMarketDataService = new CMarketDataService(m_pServiceResponser, m_pMDB);

		if (g_nMDSnapDepth > 0)
		{
			//m_pMDB->m_CurrentTimeFactory->addCommitTrigger(new CDepthMarketDataTimer(m_pMarketDataService));
			m_pMDB->m_MarketOrderFactory->addCommitTrigger(new CMarketOrderNotifyTrigger(m_pMarketDataService));
			m_pMDB->m_TickMarketOrderFactory->addCommitTrigger(new CTickMarketOrderNotifyTrigger(m_pMarketDataService));
		}

		printf("MarketData: read config %s \n", INI_FILE_NAME);
		m_Ini.Open(INI_FILE_NAME);
	}

	virtual bool RspField(const TOutFogMetaRef* pOutFogRspRef, const void* pRspBuffer);
	void SubTopics(CLinkField linkField);

	void Init24(CPublishKLine* pKLine, double now);

	//返回订阅频率，>=1 表示成功,并设置频率
	int TestTopicAction(CTopicActionField* pField, TSessionRef* pSession);
	int RspTopicAction(CTopicActionField* pField, TSessionRef* pSession);

	CLinkMap m_LinkFrom;

private:
	void UpgradeTickPrice(const CMarketOrderField& field, const CMarketOrder* pOldMarketOrder);
	//	void GetTickArray(double &tick, double *ticks);

private:
	TOutFogMetaRef m_OutFogMetaRef;
	CRspCallback* m_RspCallback;
	CServiceResponser* m_pServiceResponser;
	COutFogServiceApi* m_pServiceApi;
	char m_streamBuf[4096];
	TIniFile m_Ini;
	CMDB* m_pMDB;
	CMarketDataService* m_pMarketDataService;
	CErrorEngine* m_pErrorEngine;
	int m_nLoadKLineCount;
	CDbHelper* m_pDbHelper;
	int m_nTimeZone;
	CLoginService* m_pLoginService;
	string m_sGateWayKey;
	int m_nReady;
	CMilliSecondsType m_FirstTime;
	//CMilliSecondsType m_CurrentTime;
	//CNumberType m_TimeZone;
	CCurrentTimeField m_CurrentTimeField;
	SubMap m_SubMap;
	TSessionRef SessionBase;
};

#endif
