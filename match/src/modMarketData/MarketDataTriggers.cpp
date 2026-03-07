#include "public.h"
#include "MarketDataTriggers.h"
#include "CopyField.h"
#include "printinfo.h"
#include "monitorIndex.h"
#include "tradingTools.h"

CMarketOrderNotifyTrigger::CMarketOrderNotifyTrigger(CMarketDataService *pMarketDataService)
{
	m_pMarketDataService = pMarketDataService;
}

void CMarketOrderNotifyTrigger::commitAdd(CMarketOrder *pMarketOrder)
{
	CMarketOrderField field;
//	field.ChangeType = DAT_Insert;
	CopyMarketOrderEntity(&field, pMarketOrder);
	string SubIndexName = field.ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += field.InstrumentID.getValue();
	m_pMarketDataService->SetMarketOrder(SubIndexName.c_str(), &field);
}

void CMarketOrderNotifyTrigger::commitUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pOldMarketOrder)
{
	CMarketOrderField field;
//	field.ChangeType = DAT_Update;
	CopyMarketOrderEntity(&field, pMarketOrder);
	string SubIndexName = field.ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += field.InstrumentID.getValue();
	m_pMarketDataService->SetMarketOrder(SubIndexName.c_str(), &field);
}

void CMarketOrderNotifyTrigger::commitRemove(CWriteableMarketOrder *pMarketOrder)
{
	CMarketOrderField field;
//	field.ChangeType = DAT_Delete;
	CopyMarketOrderEntity(&field, pMarketOrder);
	field.Volume = 0.0;
	field.Orders = 0;
	field.TrueVolume = 0.0;
	field.BusinessNo = pMarketOrder->BusinessNo;
	string SubIndexName = field.ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += field.InstrumentID.getValue();
	m_pMarketDataService->SetMarketOrder(SubIndexName.c_str(), &field);
}

CTickMarketOrderNotifyTrigger::CTickMarketOrderNotifyTrigger(CMarketDataService* pMarketDataService)
{
	m_pMarketDataService = pMarketDataService;
}

void CTickMarketOrderNotifyTrigger::commitAdd(CTickMarketOrder* pMarketOrder)
{
	CTickMarketOrderField field;
	//	field.ChangeType = DAT_Insert;
	CopyTickMarketOrderEntity(&field, pMarketOrder);
	//string SubIndexName = field.ExchangeID.getValue();
	//SubIndexName += "_";
	//SubIndexName += field.InstrumentID.getValue();
	//SubIndexName += "_";
	//SubIndexName += pMarketOrder->Tick.getString();
	m_pMarketDataService->SetTickMarketOrder(pMarketOrder->SubIndex.getValue(), &field);
}

void CTickMarketOrderNotifyTrigger::commitUpdate(CTickMarketOrder* pMarketOrder, CWriteableTickMarketOrder* pOldMarketOrder)
{
	CTickMarketOrderField field;
	//	field.ChangeType = DAT_Update;
	CopyTickMarketOrderEntity(&field, pMarketOrder);
	//string SubIndexName = field.ExchangeID.getValue();
	//SubIndexName += "_";
	//SubIndexName += field.InstrumentID.getValue();
	//SubIndexName += "_";
	//SubIndexName += pMarketOrder->Tick.getString();
	m_pMarketDataService->SetTickMarketOrder(pMarketOrder->SubIndex.getValue(), &field);
}

void CTickMarketOrderNotifyTrigger::commitRemove(CWriteableTickMarketOrder* pMarketOrder)
{
	CTickMarketOrderField field;
	//	field.ChangeType = DAT_Delete;
	CopyTickMarketOrderEntity(&field, pMarketOrder);
	field.Volume = 0.0;
	field.Orders = 0;
	field.TrueVolume = 0.0;
	field.BusinessNo = pMarketOrder->BusinessNo;
	//string SubIndexName = field.ExchangeID.getValue();
	//SubIndexName += "_";
	//SubIndexName += field.InstrumentID.getValue();
	//SubIndexName += "_";
	//SubIndexName += pMarketOrder->Tick.getString();
	m_pMarketDataService->SetTickMarketOrder(pMarketOrder->SubIndex.getValue(), &field);
}
