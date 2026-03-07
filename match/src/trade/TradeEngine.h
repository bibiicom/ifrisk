#ifndef TRADEENGINE_H
#define TRADEENGINE_H

#include "Engine.h"

#include "MyAlterServiceInterface.h"
#include "MyAlterService.h"

#include "LoginServiceInterface.h"
#include "LoginService.h"

#include "TradeServiceInterface.h"
#include "CTradeService.h"

#include "SettlementServiceInterface.h"
#include "CSettlementService.h"
#include "tradingTools.h"

//#include "MyTriggers.h"
//#include "bulletinServiceInterface.h"
//#include "CBulletinServiceImpl.h"
#include "OutFogServiceApi.h"
#include "QueryServiceInterface.h"
//#include "MarketDataService.h"
//#include "MarketDataTimer.h"
//#include "MarketDataTriggers.h"
#include "TableDefine.h"
#include "TableTrigger.h"
#include "MyResponser.h"

/////////////////////////////////////////////////////////////////////////
///CMemoryTradeEngine是一个基于内存消息传送的交易引擎，包含了交易用的内存服务连接
/////////////////////////////////////////////////////////////////////////
class CTradeEngine : public CEngine, public COutFogServiceApi, public CTinitTableCallback
{
public:
	///构造函数
	///@param	pConfig	创建引擎需要使用的配置
	///@param	pTradeResult	用于放置输出结果的流
	///@param	recovery	是否使用恢复模式
	///@param	verbose	true表示要输出结果，flase表示不要输出结果
	CTradeEngine(COutFogServiceCallback *pCallback);

	virtual ~CTradeEngine(void);

	virtual void handleMessage(const TOutFogMetaRef* pOutFogReqRef, const void *pStream, TSessionRef *pSession);
	virtual bool SendTable(CFieldDescribe *pFieldDesc, const void *pRspBuffer);
	virtual void setServiceCallback(COutFogServiceCallback *pCallback);
	virtual const char* getVersion();
	virtual bool ReqField(const TOutFogMetaRef* pOutFogReqRef, const void *pStream);
	virtual bool SyncReqField(const TOutFogMetaRef* pOutFogReqRef, const void *pReqStream, TOutFogMetaRef* pOutFogRspRef, void *pRspStream)
	{
		return true;
	}

	virtual bool Start() { return true; };
	//void PushAccountPosition();
	//void PushTriggerTables();

	bool SnapSave();
	bool snapMDB(UF_INT8 nSequenceNo, bool bImmediately = false);
	void RollBack()
	{
		m_Transaction.rollback();
	}
	bool snapRedoMDB(UF_INT8 nSequenceNo);
	bool snapAllMDB(UF_INT8 nSequenceNo, bool bImmediately = false);
	bool UpdateSequence();
	//定时器处理
	void Timer(const TOutFogMetaRef* pOutFogReqRef, CCurrentTime *pOldTime, CCurrentTime *pNewTime);
	bool m_bFirstField;

private:
	bool LoadFromCSV(const char *pszCSVPath, UF_INT8 nBeginID);
	//	bool LoadTable(const char *pszCSVFileName, CFieldDescribe *pFieldDesc);

		//string m_strErrorMsg;

	CTradeService *m_pTradeService;
	CSettlementService *m_pSettlementService;
	CLoginService *m_pLoginService;
	CMyAlterService  *m_pDbmtService;
	//	CBulletinServiceImpl  *m_pBulletinService;
//	CMarketDataService *m_pMarketDataService;

	//	CBulletinServiceInterface *m_pBulletinServiceInterface;
	CTradeServiceInterface *m_pTradeServiceInterface;
	CSettlementServiceInterface *m_pSettlementServiceInterface;
	CLoginServiceInterface *m_pLoginServiceInterface;
	CMyAlterServiceInterface *m_pDbmtServiceLink;
	CQueryServiceInterface *m_pQueryXTPLink;

	CMyXTPResponser *m_pServiceResponser;

	TOutFogMetaRef m_OutFogMetaRef;
	char m_pStreamBuf[OUTFOG_MAX_PACKAGE_SIZE * 3];

	CSyncTriggers* m_SyncTriggers;
	int m_nSnapCount;		//正常运行多少个报文一次snap
	int m_nSnapCountRedo;	//Redo情况下运行多少个报文一次snap

	//bool m_bEngineWork; //是否接受业务报文
	UF_INT8 m_nLastDumpNo;
	string sSnapPath; // Sanp文件的路径
	char m_sTemp[OUTFOG_MAX_PACKAGE_SIZE * 3];
};

#endif
