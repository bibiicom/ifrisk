#ifndef __CTriggerOrder_H__
#define __CTriggerOrder_H__

#include "OutFogServiceApi.h"
#include "Config.h"
#include "utility.h"
#include "OutFogService.h"
#include "version.h"
#include "FieldData.h"
#include "profile.h"
#include "WorkerInterface.h"
#include "StopOrderWorker.h"
#include "CallbackWorker.h"
#include "mdb.h"


#define INI_FILE_NAME "triggerorder.ini"
#define NETWORK_FILE_NAME "network.ini"
#define ALL_PRODUCTGROUPS "Swap,SwapU,Spot,CFD"

#define InitOrder(field)						\
	{											\
		if (!field.VolumeMode.isValid())		\
			field.VolumeMode = VM_Absolute;		\
		if (!field.OrderPriceType.isValid())	\
			field.OrderPriceType.init();		\
		if (!field.OrderType.isValid())			\
			field.OrderType.init();				\
		if (!field.TimeCondition.isValid())		\
			field.TimeCondition.init();			\
		if (!field.IsAutoSuspend.isValid())		\
			field.IsAutoSuspend.init();			\
		if (!field.BusinessType.isValid())		\
			field.BusinessType.init();			\
		if (field.Volume.isNull())				\
			field.Volume = 0.0;					\
		if (field.VolumeDisplay.isNull())		\
			field.VolumeDisplay = field.Volume;	\
		if (field.OffsetFlag.isNull())			\
			field.OffsetFlag = OF_Open;			\
		if (field.PositionFlag.isNull())		\
			field.PositionFlag = PF_Speculation;\
		if (field.MinVolume.isNull())			\
			field.MinVolume = 0.0;				\
		if (field.Cost.isNull())				\
			field.Cost = 0.0;					\
	}

typedef map<string, CWorkerInterface*> CWorkerMap;
typedef map<string, string> CPairMap;
//typedef map<string, CPriceType> CInstrumentPriceMap;
//typedef map<string, CMilliSecondsType> CInstrumentTradeTimeMap;
typedef map<int, CLinkField> CLinkMap;

typedef struct tMarketCheck_t {
	CPriceType LastPrice;
	CPriceType MarkedPrice;
	CMilliSecondsType UpdateTime;
} MarketCheck_t;

typedef map<string, MarketCheck_t> CMarketCheckMap;

void CmdLineArgError();

class CTriggerOrders:public COutFogServiceCallback,public CWorkerCallback
{
public:
	CTriggerOrders();
	~CTriggerOrders();

	void setServiceApi(COutFogServiceApi* pServiceApi)
	{
		m_pServiceApi = pServiceApi;
		printf("TriggerOrder: read config %s \n", INI_FILE_NAME);
		m_Ini.Open(INI_FILE_NAME);
	}

	virtual bool RspField(const TOutFogMetaRef* pOutFogRspRef, const void *pRspBuffer);

	virtual bool SendTriggerOrder(CTriggerOrder *pTriggerOrder, CPriceType Price, CPriceType LastPrice, CActionFlagType ActionFlag = AF_Trigger);

	void SubTopics(CLinkField linkField);

	CLinkField m_LinkTo;
	CLinkMap m_LinkFrom;
	TOutFogMetaRef m_OutFogMetaRef;

private:

	//COutFogServiceCallback* m_pCallback;
	COutFogServiceApi* m_pServiceApi;
	char m_streamBuf[4096];
	CWorkerMap m_WorkerMap;
	TIniFile m_Ini;
	CMDB *m_pMDB;
	//CInstrumentPriceMap m_InstrumentPriceMap;
	//CInstrumentTradeTimeMap m_InstrumentTradeTimeMap;
};

#endif
