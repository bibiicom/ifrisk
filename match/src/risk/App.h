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
#include "mdb.h"
#include "ServiceResponser.h"
#include "CErrorEngine.h"
#include "errorDefine.h"
#include "MyResponser.h"
#include "TableTrigger.h"
#include "tradingTools.h"

#define INI_FILE_NAME "risk.ini"
#define NETWORK_FILE_NAME "network.ini"

//#define InitOrder(field)						\
//	{											\
//		if (!field.VolumeMode.isValid())		\
//			field.VolumeMode = VM_Absolute;		\
//		if (!field.OrderPriceType.isValid())	\
//			field.OrderPriceType.init();		\
//		if (!field.OrderType.isValid())			\
//			field.OrderType.init();				\
//		if (!field.TimeCondition.isValid())		\
//			field.TimeCondition.init();			\
//		if (!field.IsAutoSuspend.isValid())		\
//			field.IsAutoSuspend.init();			\
//		if (!field.BusinessType.isValid())		\
//			field.BusinessType.init();			\
//		if (field.Volume.isNull())				\
//			field.Volume = 0.0;					\
//		if (field.VolumeDisplay.isNull())		\
//			field.VolumeDisplay = field.Volume;	\
//		if (field.OffsetFlag.isNull())			\
//			field.OffsetFlag = OF_Open;			\
//		if (field.PositionFlag.isNull())		\
//			field.PositionFlag = PF_Speculation;\
//		if (field.MinVolume.isNull())			\
//			field.MinVolume = 0.0;				\
//		if (field.Cost.isNull())				\
//			field.Cost = 0.0;					\
//	}

//typedef map<string, CWorkerInterface*> CWorkerMap;
//typedef map<string, string> CPairMap;
//typedef map<string, CPriceType> CInstrumentPriceMap;
//typedef map<string, CMilliSecondsType> CInstrumentTradeTimeMap;

typedef map<int, CLinkField> CLinkMap;

typedef map<string, double> CAccountMap;

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


class CRisk:public COutFogServiceCallback,public CWorkerCallback
{
public:
	CRisk();
	~CRisk();

	void setServiceApi(COutFogServiceApi* pServiceApi)
	{
		m_pServiceApi = pServiceApi;

		m_RspCallback = new CRspCallback(pServiceApi);
		m_pServiceResponser = new CMyXTPResponser(m_RspCallback);

		printf("Risk: read config %s \n", INI_FILE_NAME);
		m_Ini.Open(INI_FILE_NAME);
	}

	virtual bool RspField(const TOutFogMetaRef* pOutFogRspRef, const void *pRspBuffer);

	virtual bool SendForceClose(CAccount* pAccount);
	virtual bool SendForceClose(CPosition* pPosition);

	void SubTopics(CLinkField linkField);

	CLinkField m_LinkTo;
	CLinkMap m_LinkFrom;
	TOutFogMetaRef m_OutFogMetaRef;
	volatile int m_AccountMapStatus;
	double m_AccountMapLastTime;
	CAccountMap m_AccountMap;
private:

	//COutFogServiceCallback* m_pCallback;
	COutFogServiceApi* m_pServiceApi;
	char m_streamBuf[4096];
	TIniFile m_Ini;
	CMDB *m_pMDB;

	CMyXTPResponser *m_pServiceResponser;
	CErrorEngine *m_pErrorEngine;
	CRspCallback *m_RspCallback;
	int nRecvTNo;
	TSessionRef sessionRef;
};

#endif
