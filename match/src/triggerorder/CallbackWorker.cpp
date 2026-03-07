#include "CallbackWorker.h"
#include "profile.h"
#include "public.h"

CCallbackWorker::CCallbackWorker(CMDB *pMDB, CWorkerCallback *pWorkerCallback):CWorkerInterface()
{
	m_pWorkerCallback = pWorkerCallback;
	m_pMDB = pMDB;
}

CCallbackWorker::~CCallbackWorker()
{
}

//bool CCallbackWorker::TimeSync(CCurrentTimeField *pField)
//{
//	return true;
//}

//bool CCallbackWorker::MarketOverViewSync(CMarketOverViewField *pField)
//{
//	return true;
//}

//bool CCallbackWorker::TriggerOrderSync(CTriggerOrderField *pField)
//{
//	return true;
//}

//bool CCallbackWorker::TriggerOrderActionSync(CTriggerOrderField *pField)
//{
//	return true;
//}
//
//bool CCallbackWorker::MarketTradeSync(CMarketTradeField *pField)
//{
//	return true;
//}

int CCallbackWorker::TriggerByPrice(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CPriceType Price, CTriggerPriceTypeType TriggerPriceType)
{
	return true;
}