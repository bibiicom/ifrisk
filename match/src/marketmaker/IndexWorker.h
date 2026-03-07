#ifndef __CIndexWorker_H__
#define __CIndexWorker_H__

#include "utility.h"
#include "FieldData.h"
#include "WorkerInterface.h"

#define WorkerTypeIndex 'I'

class CIndexWorker :public CWorkerInterface
{
public:
	CIndexWorker(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CWorkerCallback *pMarketMaker);
	~CIndexWorker();

	bool SaveOrder(COrderField *pField) { return true; };
	bool InitOrder(COrderField *pField) { return true; };
	bool SaveMarketTrade(CMarketTradeField *pField);
	bool SaveMarketOrder(CMarketOrderField *pField) { return true; };
	bool Init() { return true; };

	bool SaveMarketOverView(CMarketOverViewField *pField);

	virtual CExchangeIDType GetExchangeID()
	{
		return m_OTCTradeInsertField.ExchangeID;
	}
	virtual CInstrumentIDType GetInstrumentID()
	{
		return m_OTCTradeInsertField.InstrumentID;
	}
	virtual char GetWorkerType()
	{
		return WorkerTypeIndex;
	}
	virtual int GetLinkNo()
	{
		return m_nLinkNo;
	}
private:
	CWorkerCallback *m_pMarketMaker;
	COTCTradeInsertField m_OTCTradeInsertField;
	CMarketDataUpdateField m_MarketDataUpdateField;
	int m_nLinkNo;
};

#endif
