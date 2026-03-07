#ifndef _MDTRIGGERS_H_
#define _MDTRIGGERS_H_

#include "BaseDataStruct.h"
#include "CMDB.h"
#include "ServiceInterface.h"
#include "FieldData.h"
#include "MarketDataService.h"

const int TOPIC_MARKETDATA_VERSION_INTERVAL = 1000;


class CMarketOrdersNotifyTrigger :public CMarketOrdersCommitTrigger
{
public:
	CMarketOrdersNotifyTrigger(CMarketDataService *pMarketDataService);

	///加入后触发
	///@param	pMarketOrders	已经加入的MarketOrders
	virtual void commitAdd(CMarketOrders *pMarketOrders);

	///更新后触发
	///@param	pMarketOrders	被刷新的CMarketOrders
	///@param	poldMarketOrders	原来的值
	virtual void commitUpdate(CMarketOrders *pMarketOrders, CWriteableMarketOrders *pOldMarketOrders);

	///删除后触发
	///@param	pMarketOrders	已经删除的CMarketOrders
	virtual void commitRemove(CWriteableMarketOrders *pMarketOrders);

private:

	CMarketDataService *m_pMarketDataService;
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

class CKLineTrigger : public CTradeCommitTrigger, public CCurrentTimeCommitTrigger, public CInstrumentCommitTrigger, public CMarketDataCommitTrigger
{
public:
	CKLineTrigger(CMDB *pMDB);
	virtual ~CKLineTrigger();
	virtual void commitAdd(CTrade *pTrade);
	virtual void commitAdd(CMarketData *pMarketData);
	virtual void commitUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pOldCurrentTime);
	virtual void commitUpdate(CInstrument *pInstrument, CWriteableInstrument *pOldInstrument);
	virtual void commitUpdate(CMarketData *pMarketData, CWriteableMarketData *pOldMarketData);
	virtual void commitRemove(CWriteableInstrument *pInstrument);

	void SetLastKLine(CTrade* pTrade, CIndexIDType Period, CTransaction *pTransaction);
	void SetLastKLineByBeginTime(CTrade* pTrade, CIndexIDType Period, CTransaction *pTransaction);
	void SetLastMarkKLineByBeginTime(CMarketData* pMarketData, CIndexIDType Period, CTransaction *pTransaction);

	void CreateLastKLine(CLastKLine *pLastKLine, CTrade* pTrade, CIndexIDType Period, CTransaction *pTransaction);
	void CreateMarkLastKLine(CLastKLine *pLastKLine, CMarketData* pMarketData, CIndexIDType Period, CTransaction *pTransaction);
	void UpdateLastKLine(CWriteableLastKLine* pKLine, CTrade* pTrade);
	void UpdateMarkLastKLine(CWriteableLastKLine* pKLine, CMarketData* pMarketData);

	void InsertKLine(CWriteableKLine *theKline, CTransaction *pTransaction);
	void InsertKLineByBeginTime(CWriteableKLine *theKline, CTransaction *pTransaction);
	void InsertKLineByBeginTime2(CWriteableKLine *pKline, CTransaction *pTransaction);
	void InsertMarkKLineByBeginTime(CInstrument *pInstrument, CWriteableKLine *pKline, CTransaction *pTransaction);
	bool AddKLineTimeUp(CCurrentTime *pCurrentTime, CIndexIDType Period, CTransaction *pTransaction);
	bool AddKLineTimeUp2(CCurrentTime *pCurrentTime, CIndexIDType Period, CTransaction *pTransaction);
	bool AddMarkKLineTimeUp2(CCurrentTime *pCurrentTime, CIndexIDType Period, CTransaction *pTransaction);
	int GetKLineNo(CInstrument *pInstrument, CCurrentTime *pCurrentTime, CIndexIDType Period);

private:
	void SetMarkKLineByMarketData(CMarketData* pMarketData);
	CInstrumentIDType getMarkInstrumentID(const CInstrumentIDType &InstrumentID);
	CSecondsType FormatBeginTime(CSecondsType beginTime, CIndexIDType Period);
	CTimeType m_LastTime;
	CDateType m_LastDate;
	CDateType m_LastDays;
	CMDB *m_pMDB;
	bool m_bTimeUpdate;
};

#endif