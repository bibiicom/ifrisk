#ifndef __CPrivate_H__
#define __CPrivate_H__

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
#include "mdb.h"
#include "MarketDataService.h"
#include "MarketDataTimer.h"
#include "MarketDataTriggers.h"
#include "LoginService.h"

#define INI_FILE_NAME "private.ini"
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


class CApp:public COutFogServiceCallback
{
public:
	CApp();
	~CApp();

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

		printf("Private: read config %s \n", INI_FILE_NAME);
		m_Ini.Open(INI_FILE_NAME);
	}

	virtual bool RspField(const TOutFogMetaRef* pOutFogRspRef, const void *pRspBuffer);
	void SubTopics(CLinkField linkField);

	void Init24(CPublishKLine *pKLine, double now);

	//返回订阅频率，>=1 表示成功,并设置频率
	int SubTopicAction(CTopicActionField* pField, TSessionRef* pSession);

	CLinkMap m_LinkFrom;
	TOutFogMetaRef m_OutFogMetaRef;

private:
	void UpgradeTickPrice(const CMarketOrderField& field, const CMarketOrder* pOldMarketOrder);
//	void GetTickArray(double &tick, double *ticks);

	CRspCallback *m_RspCallback;
	CServiceResponser *m_pServiceResponser;
	COutFogServiceApi *m_pServiceApi;
	char m_streamBuf[4096];
	TIniFile m_Ini;

	CErrorEngine *m_pErrorEngine;
	int nRecvTNo;
	TSessionRef sessionRef;	
	string m_sGateWayKey;
		
	CMDB *m_pMDB;
	CMarketDataService *m_pMarketDataService;
	int m_nLoadKLineCount;
	int m_nTimeZone;
	CLoginService* m_pLoginService;
	int m_nReady;
	CMilliSecondsType m_FirstTime;
	//CMilliSecondsType m_CurrentTime;
	//CNumberType m_TimeZone;
	CCurrentTimeField m_CurrentTimeField;
	//SubMap m_SubMap;
};

#endif
