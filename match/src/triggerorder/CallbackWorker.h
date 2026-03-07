#ifndef __CCallbackWorker_H__
#define __CCallbackWorker_H__

#include "utility.h"
#include "FieldData.h"
#include "WorkerInterface.h"
#include "mdb.h"

class CCallbackWorker:public CWorkerInterface
{
public:
	CCallbackWorker(CMDB *pMDB, CWorkerCallback *pWorkerCallback);
	~CCallbackWorker();

	//bool TimeSync(CCurrentTimeField *pField);
//	bool MarketOverViewSync(CMarketOverViewField *pField);
	//bool TriggerOrderSync(CTriggerOrderField *pField);
	//bool TriggerOrderActionSync(CTriggerOrderField *pField);
	//bool MarketTradeSync(CMarketTradeField *pField);
	int TriggerByPrice(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CPriceType Price, CTriggerPriceTypeType TriggerPriceType);

private:
	CWorkerCallback *m_pWorkerCallback;
	CMDB *m_pMDB;
};

#endif
