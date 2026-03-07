#ifndef _MARKETDATASERVICEIMPL_H__
#define _MARKETDATASERVICEIMPL_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "databaseData.h"
#include "tradingTools.h"
#include "CSequence.h"
#include "FieldData.h"
#include "ServiceResponser.h"

//#define INST_DESC_DEPTH  3

//const double s_instDescArry[] = { 1000000.0, 100000.0, 10000.0, 1000.0,
//	100.0, 10.0, 1.0, 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001, 0.00000001, 0.000000001, 0.0000000001 };

struct InstDescInfo
{
	double desc = 0.0;
	//double tick[INST_DESC_DEPTH];
	vector<double> tick;
	int topicid = 0;
	vector<string> index;
};

extern int g_nMDSnapDepth;
//extern int g_nPrivateBase;
//extern int g_nPrivateDispatch;
extern UF_INT8 g_nBusinessNo;

typedef map<CInstrumentIDType, InstDescInfo> InstDescMap;

class CMarketDataService;

typedef map<double, CMarketOrderField> CMarketOrderMap;
typedef map<double, CTickMarketOrderField> CTickMarketOrderMap;
typedef map<string, CMarketOrderMap> CInstrumentMarketOrderMap;
typedef map<string, CTickMarketOrderMap> CInstrumentTickMarketOrderMap;

class CMarketDataService
{
public:
	CMarketDataService(CServiceResponser *pServiceResponser, CMDB *pMDB);
	virtual ~CMarketDataService();

	virtual bool PublishMarketData(CCurrentTimeField* pField);
	virtual bool PublishDelayMarketData(CCurrentTimeField* pField);
	virtual bool PublishSnapMarketData(CCurrentTimeField* pField);

//	virtual bool PublishDepthMarketData(CMarketData *pMarketData, CReadOnlyMilliSecondsType &CurrTime);
	virtual bool PublishSnapMarketData(CMarketData *pMarketData, CCurrentTimeField* pField);
	bool PublishSnapTickMarketData(CMarketData* pMarketData, CCurrentTimeField* pField, CPriceType Tick);

	bool PublishDelayMarketOrder(CMarketData *pMarketData, CCurrentTimeField* pField);
	bool Publish25MarketOrder(CWriteableMarketData *pMarketData, CCurrentTimeField* pField);
//	bool PublishLastKLine(CWriteableMarketData *pMarketData);
	bool Publish25TickMarketOrder(CWriteableMarketData* pMarketData, CPriceType Tick, const char* index, CCurrentTimeField* pField);
	
	void SetMarketOrder(const char *IndexName, CMarketOrderField *pField);
	void SetDelayMarketOrder(const char *IndexName, CMarketOrderField *pField);
	void SetTickMarketOrder(const char* IndexName, CTickMarketOrderField* pField);

	//TickUp行情
//	virtual bool PublishTickUpDepthMarketData(CMarketData *pMarketData, CPriceType Tick);
//	bool PublishTickUp25MarketOrder(CWriteableMarketData *pMarketData, CPriceType Tick);

	map<CExchangeIDType, InstDescMap> m_InstDescMap;
	static CFieldDescribe m_TimeDescribe;
private:
	CMDB *m_pMDB;

	UF_INT8 m_nMaxMarketDataNo;					//接收到行情的最大变动序号
	UF_INT8 m_nLastMarketDataNo;				//上次发送即时行情的最大变动序号
	CInstrumentMarketOrderMap m_BuyMarketOrderMap;
	CInstrumentMarketOrderMap m_SellMarketOrderMap;
	CInstrumentTickMarketOrderMap m_BuyTickMarketOrderMap;
	CInstrumentTickMarketOrderMap m_SellTickMarketOrderMap;
	CInstrumentMarketOrderMap m_BuyDelayMarketOrderMap;
	CInstrumentMarketOrderMap m_SellDelayMarketOrderMap;
	CServiceResponser *m_pServiceResponser;
};

#endif
