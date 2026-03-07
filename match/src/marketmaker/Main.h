#ifndef __CMarketMaker_H__
#define __CMarketMaker_H__

#include "OutFogServiceApi.h"
#include "utility.h"
#include "OutFogService.h"
#include "version.h"
#include "FieldData.h"
#include "profile.h"
#include "WorkerInterface.h"
#include "Main.h"
#include "IndexWorker.h"
#include "FastWorker.h"


#define NETWORK_FILE_NAME "network.ini"
#define INI_FILE_NAME "marketmaker.ini"

typedef map<string, CWorkerInterface*> CWorkerMap;
typedef map<string, string> CPairMap;
typedef map<int, CLinkField> CLinkMap;

extern int g_nKernelID;

void CmdLineArgError();

class CMarketMaker :public COutFogServiceCallback,public CWorkerCallback
{
public:
	CMarketMaker();
	~CMarketMaker();

	void setServiceApi(COutFogServiceApi* pServiceApi)
	{
		m_pServiceApi = pServiceApi;
	}

	virtual bool RspField(const TOutFogMetaRef* pOutFogRspRef, const void *pRspBuffer);

	string getWorkerKey(int nLinkNo, CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID);

	virtual bool SendOrder(COrderInsertField *pField);
	virtual bool SendOTCTrade(COTCTradeInsertField *pField);
	virtual bool CancelOrder(COrderField *pField);
	virtual bool GetOrder(CQryOrderField *pField);
	virtual bool SendMarketUpdate(CMarketDataUpdateField *pField);
	virtual bool SendInstrumentUpdate(CInstrumentUpdateField *pField);

	char * GetLocalID();

	bool SavePosition(CPositionField *pField);

	void SubTopics(CLinkField linkField);

	CLinkField m_LinkTo;
	bool m_bReady;
	CLongIndexIDType m_MemberID;
	CIndexIDType m_TradeUnitID;
	CLinkMap m_LinkFrom;
	TOutFogMetaRef m_OutFogMetaRef;
	int m_nLocalID;
	char m_sLocalID[21];

private:

	COutFogServiceCallback* m_pCallback;
	COutFogServiceApi* m_pServiceApi;
	char m_streamBuf[4096];
	CPairMap m_PairMap;
	CWorkerMap m_WorkerMap;
	TIniFile m_Ini;
	int m_nInitOrders;
	//统计和省略报文，用于减少报文数量，增加做市品种数目
	//int m_nCount;
	//int m_nSkipCount;
	//CSecondsType m_nTime;
	//CPriceType m_LastPrice;
};

#endif
