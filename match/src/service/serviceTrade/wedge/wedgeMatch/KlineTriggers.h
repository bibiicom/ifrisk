#ifndef _KLINETRIGGERS_H_
#define _KLINETRIGGERS_H_

#include "mdbStruct.h"
#include "mdb.h"
#include "ServiceInterface.h"
#include "FieldData.h"
//#include "MarketDataService.h"

class CKLineTrigger : public CTradeCommitTrigger, public CCurrentTimeCommitTrigger, public CInstrumentCommitTrigger
{
public:
	CKLineTrigger(CMDB *pMDB);
	virtual ~CKLineTrigger();
	virtual void commitAdd(CTrade *pTrade);
	virtual void commitUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pOldCurrentTime);

	//根据成交更新K线
	void UpdateLastKLine(CTrade* pTrade, CBarType Bar);
	//根据时间更新K线
	bool AddKLineByTime(CWriteableCurrentTime * pCurrentTime, CBarType Bar);

	void AddKLineByTime(CCurrentTime *pCurrentTime, CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CBarType Bar);

private:
	//CTimeType m_LastTime;
	//CDateType m_LastDate;
	//CDateType m_LastDays;
	//CMilliSecondsType m_LastMilliSeconds;
	CMDB *m_pMDB;
};

class CMarketTradeTrigger : public CTradeCommitTrigger
{
public:
	CMarketTradeTrigger(CServiceResponser *pServiceResponser, CMDB *pMDB);
	virtual ~CMarketTradeTrigger();

	virtual void commitAdd(CTrade *pTrade);

private:
	CServiceResponser *m_pServiceResponser;
	CMDB *m_pMDB;
};

#endif