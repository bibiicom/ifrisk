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

class CMarketDataService;

typedef map<double, CMarketOrderField> CMarketOrderMap;
typedef map<string, CMarketOrderMap> CInstrumentMarketOrderMap;

class CMarketDataService
{
public:
	CMarketDataService(CServiceResponser *pServiceResponser, CMDB *pMDB);
	virtual ~CMarketDataService();

	virtual bool PublishMarketData(CReadOnlySecondsType &CurrTime, CReadOnlyNumberType &CurrMillisec, CTransaction *pTransaction);

	virtual bool PublishDepthMarketData(CWriteableMarketData *pMarketData, CReadOnlySecondsType &CurrTime, CReadOnlyNumberType &CurrMillisec, CTransaction *pTransaction);
	bool PublishMarketOrder(CWriteableMarketData *pMarketData);
	bool Publish25MarketOrder(CWriteableMarketData *pMarketData);
	bool PublishLastKLine(CWriteableMarketData *pMarketData);
	
	void SetMarketOrder(const char *IndexName, CMarketOrderField *pField);

	//TickUp行情
	virtual bool PublishTickUpDepthMarketData(CWriteableMarketData *pMarketData, CPriceType Tick);
	bool PublishTickUp25MarketOrder(CWriteableMarketData *pMarketData, CPriceType Tick);

private:
	CMDB *m_pMDB;

	UF_INT8 m_nLastMarketDataNo;				//上次发送即时行情的最大变动序号
	CInstrumentMarketOrderMap m_BuyMarketOrderMap;
	CInstrumentMarketOrderMap m_SellMarketOrderMap;
	CServiceResponser *m_pServiceResponser;
};

#endif
