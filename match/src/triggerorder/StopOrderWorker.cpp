#include "StopOrderWorker.h"
#include "profile.h"
#include "public.h"
#include "CopyField.h"
#include "monitorIndex.h"

CStopOrderWorker::CStopOrderWorker(CMDB *pMDB, CWorkerCallback *pWorkerCallback) :CWorkerInterface()
{
	m_pWorkerCallback = pWorkerCallback;
	m_pMDB = pMDB;
}

CStopOrderWorker::~CStopOrderWorker()
{
}

//bool CStopOrderWorker::TimeSync(CCurrentTimeField *pField)
//{
//	return true;
//}

//bool CStopOrderWorker::MarketOverViewSync(CMarketOverViewField *pField)
//{
//	TriggerByPrice(pField->ExchangeID, pField->InstrumentID, pField->LastPrice, TPT_LastPrice);
//	TriggerByPrice(pField->ExchangeID, pField->InstrumentID, pField->MarkedPrice, TPT_MarkedPrice);
//	return true;
//}

//bool CStopOrderWorker::TriggerOrderActionSync(CTriggerOrderField *pField)
//{
//	return true;
//}

//bool CStopOrderWorker::MarketTradeSync(CMarketTradeField *pField)
//{
//	return TriggerByPrice(pField->ExchangeID, pField->InstrumentID, pField->Price, TPT_LastPrice);
//}

int CStopOrderWorker::TriggerByPrice(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CPriceType Price, CTriggerPriceTypeType TriggerPriceType)
{
	int nCount = 0;
	//找到所有买入止损单,TriggerPriceType,最新价/标记价
	CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByBuySLTriggerPrice(ExchangeID, InstrumentID, Price);
	while (pTriggerOrder != NULL)
	{
		//TriggerPriceType，最新价/标记价
		if (pTriggerOrder->TriggerPriceType == TriggerPriceType)
		{
			nCount++;
			m_pWorkerCallback->SendTriggerOrder(pTriggerOrder, pTriggerOrder->SLTriggerPrice, Price);
		}	
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByBuySLTriggerPrice();
	}

	//找到所有卖单止损单
	pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstBySellSLTriggerPrice(ExchangeID, InstrumentID, Price);
	while (pTriggerOrder != NULL)
	{
		if (pTriggerOrder->TriggerPriceType == TriggerPriceType)
		{
			nCount++;
			m_pWorkerCallback->SendTriggerOrder(pTriggerOrder, pTriggerOrder->SLTriggerPrice, Price);
		}
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextBySellSLTriggerPrice();
	}

	//找到所有买单止盈单
	pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByBuyTPTriggerPrice(ExchangeID, InstrumentID, Price);
	while (pTriggerOrder != NULL)
	{
		if (pTriggerOrder->TriggerPriceType == TriggerPriceType)
		{
			nCount++;
			m_pWorkerCallback->SendTriggerOrder(pTriggerOrder, pTriggerOrder->TPTriggerPrice, Price);
		}
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByBuyTPTriggerPrice();
	}

	//找到所有卖单止盈单
	pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstBySellTPTriggerPrice(ExchangeID, InstrumentID, Price);
	while (pTriggerOrder != NULL)
	{
		if (pTriggerOrder->TriggerPriceType == TriggerPriceType)
		{
			nCount++;
			m_pWorkerCallback->SendTriggerOrder(pTriggerOrder, pTriggerOrder->TPTriggerPrice, Price);
		}
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextBySellTPTriggerPrice();
	}

	//REPORT_EVENT(LOG_INFO, "TriggerByPrice", "finish TriggerOrder:[%s][%s_%s][Price:%s][TriggerPriceType:%c]",
	//	ExchangeID.getValue(), InstrumentID.getValue(), Price.getString().c_str(), TriggerPriceType.getValue());

	return nCount;
}
