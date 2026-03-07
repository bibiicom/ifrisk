#ifndef __CStopOrderWorker_H__
#define __CStopOrderWorker_H__

#include "utility.h"
#include "FieldData.h"
#include "WorkerInterface.h"
#include "mdb.h"

class CStopOrderWorker:public CWorkerInterface
{
public:
	CStopOrderWorker(CMDB *pMDB, CWorkerCallback *pWorkerCallback);
	~CStopOrderWorker();

	//bool TimeSync(CCurrentTimeField *pField);
	//bool MarketOverViewSync(CMarketOverViewField *pField);
	bool TriggerOrderSync(CTriggerOrderField *pField);
	//bool TriggerOrderActionSync(CTriggerOrderField *pField);
	//bool MarketTradeSync(CMarketTradeField *pField);
	int TriggerByPrice(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CPriceType Price, CTriggerPriceTypeType TriggerPriceType);

private:
	CWorkerCallback *m_pWorkerCallback;
	CMarketDataField m_MarketDataField;
	CMDB *m_pMDB;
};

#endif
