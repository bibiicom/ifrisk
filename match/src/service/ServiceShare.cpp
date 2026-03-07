#include "ServiceShare.h"
#include "CDate.h"
#include "CopyField.h"
#include "BaseFunction.h"
#include "printinfo.h"

int g_nPageMaxLimit = 200;

void SendMarketData(CMDB* m_pMDB, CServiceResponser* pServiceResponser, CQryMarketDataField& field, TSessionRef* pSession)
{
	if (!field.InstrumentID.isNull())
	{
		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if (pMarketData != NULL)
		{
			CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
			if (pInstrument == NULL || pInstrument->SortNumber == -1)
			{
				pServiceResponser->SendResponse(TID_RecvQryMarketData, NULL, NULL, pSession, OUTFOG_CHAIN_SINGLE);
			}
			else
			{
				CMarketDataField rspField;
				CopyMarketDataEntity(&rspField, pMarketData);
				rspField.Remark = pInstrument->Remark;
				pServiceResponser->SendResponse(TID_RecvQryMarketData, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			}
		}
		else
		{
			pServiceResponser->SendResponse(TID_RecvQryMarketData, NULL, NULL, pSession, OUTFOG_CHAIN_SINGLE);
		}
	}
	else
	{
		CNumberType	From = 0;
		CNumberType	Limit = g_nPageMaxLimit;
		if (!field.From.isNull() && field.From >= 0)
			From = field.From;
		if (!field.Limit.isNull() && field.Limit > 0 && field.Limit <= g_nPageMaxLimit)
			Limit = field.Limit;

		CNumberType Index = 0;
		CMarketDataField rspField;
		pServiceResponser->PrepareResponse(TID_RecvQryMarketData, 0, "Success", pSession);
		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
		while (pMarketData != NULL)
		{
			if (!field.ExchangeID.isNull() && pMarketData->ExchangeID != field.ExchangeID)
			{
				pMarketData = m_pMDB->m_MarketDataFactory->getNext();
				continue;
			}
			if (!field.ProductGroup.isNull() && pMarketData->ProductGroup != field.ProductGroup)
			{
				pMarketData = m_pMDB->m_MarketDataFactory->getNext();
				continue;
			}

			CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pMarketData->ExchangeID, pMarketData->InstrumentID);
			if (pInstrument == NULL || pInstrument->SortNumber == -1)
			{
				pMarketData = m_pMDB->m_MarketDataFactory->getNext();
				continue;
			}

			if (Index < From)
			{
				Index += 1;
				pMarketData = m_pMDB->m_MarketDataFactory->getNext();
				continue;
			}
			else if (Index >= From + Limit)
			{
				break;
			}

			CopyMarketDataEntity(&rspField, pMarketData);

			CMarketOrder* pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
			if (pMarketOrder != NULL)
			{
				rspField.AskPrice1 = pMarketOrder->Price;
				rspField.AskVolume1 = pMarketOrder->Volume;
			}
			pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
			if (pMarketOrder != NULL)
			{
				rspField.BidPrice1 = pMarketOrder->Price;
				rspField.BidVolume1 = pMarketOrder->Volume;
			}
			if(rspField.Remark.isNull())
				rspField.Remark = pInstrument->InstrumentName;
			pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pMarketData = m_pMDB->m_MarketDataFactory->getNext();
			Index += 1;
		}
		pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	}
}

void SendMarketOrder(CMDB* m_pMDB, CServiceResponser* pServiceResponser, CQryMarketOrderField& field, TSessionRef* pSession)
{
	CMarketOrderField rspField;
	rspField.ExchangeID = field.ExchangeID;
	rspField.InstrumentID = field.InstrumentID;
	int nLevel = INT_32_MAX;
	if (!field.Depth.isNull())
		nLevel = field.Depth.getValue();
	string sIndex = field.ExchangeID.getValue();
	sIndex += "_";
	sIndex += field.InstrumentID.getValue();
	CMarketOrder* pMarketOrder = NULL;
	int j = 0;
	if (field.Direction.isNull() || field.Direction == D_Sell)
	{
		rspField.Direction = D_Sell;
		pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(field.ExchangeID, field.InstrumentID);
		while (pMarketOrder != NULL)
		{
			if (j >= nLevel)
				break;
			rspField.Price = pMarketOrder->Price;
			rspField.Volume = pMarketOrder->Volume;
			rspField.Orders = pMarketOrder->Orders;
			rspField.TrueVolume = pMarketOrder->TrueVolume;
			rspField.BusinessNo = pMarketOrder->BusinessNo;
			if (j == 0)
				pServiceResponser->SendResponse(TID_RecvQryMarketOrder, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_FIRST, 0, sIndex.c_str());
			else
				pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextBySellMBLData();
			j++;
		}
	}
	int i = 0;
	if (field.Direction.isNull() || field.Direction == D_Buy)
	{
		rspField.Direction = D_Buy;
		pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(field.ExchangeID, field.InstrumentID);
		while (pMarketOrder != NULL)
		{
			if (i >= nLevel)
				break;
			rspField.Price = pMarketOrder->Price;
			rspField.Volume = pMarketOrder->Volume;
			rspField.Orders = pMarketOrder->Orders;
			rspField.TrueVolume = pMarketOrder->TrueVolume;
			rspField.BusinessNo = pMarketOrder->BusinessNo;
			if (i == 0 && j == 0)
				pServiceResponser->SendResponse(TID_RecvQryMarketOrder, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_FIRST, 0, sIndex.c_str());
			else
				pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextByBuyMBLData();
			i++;
		}
	}

	if ((i + j) > 0)
		pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	else
	{
		pServiceResponser->SendResponse(TID_RecvQryMarketOrder, NULL, NULL, pSession, OUTFOG_CHAIN_SINGLE, 0, "NoResult");
	}
}

void CurrencyValue(CMDB* m_pMDB, CCurrencyField* pField)
{
	pField->CurrValue = 0.0;
	if (pField->InstrumentID.isNull())
	{
		pField->CurrValue = pField->Balance.getValue();
	}
	else
	{
		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pField->ExchangeID, pField->InstrumentID);
		if (pMarketData != NULL)
		{
			CPriceType ValuePrice = pMarketData->MarkedPrice;
			if (ValuePrice.isNull())
				ValuePrice = (pMarketData->AskPrice1.getValue() + pMarketData->BidPrice1.getValue()) / 2;
			if (!ValuePrice.isValid())
				ValuePrice = pMarketData->LastPrice;
			if (ValuePrice.isValid() && !ValuePrice.isNull())
				pField->CurrValue = ValuePrice.getValue() * pField->Balance.getValue();
			if (pField->Remark.isNull())
				pField->Remark = pMarketData->getInstrument()->InstrumentName;
		}
	}
}

void SendCurrency(CMDB* m_pMDB, CServiceResponser* pServiceResponser, CQryCurrencyField& field, TSessionRef* pSession)
{
	CCurrencyField rspField;
	double totalValue = 0.0;
	CCurrency* pCurrency = NULL;
	if (field.SettlementGroup.isNull())
	{
		pServiceResponser->SendResponse(TID_RecvQryCurrency, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
		pCurrency = m_pMDB->m_CurrencyFactory->getFirst();
		while (pCurrency != NULL)
		{
			CopyCurrencyEntity(&rspField, pCurrency);
			CurrencyValue(m_pMDB, &rspField);
			pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pCurrency = m_pMDB->m_CurrencyFactory->getNext();
		}
	}
	else
	{
		if (!field.Currency.isNull())
		{
			pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(field.Currency, field.SettlementGroup);
			if (pCurrency != NULL)
			{
				CopyCurrencyEntity(&rspField, pCurrency);
				CurrencyValue(m_pMDB, &rspField);
				pServiceResponser->SendResponse(TID_RecvQryCurrency, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			}
			else
			{
				pServiceResponser->SendResponse(TID_RecvQryCurrency, NULL, NULL, pSession, OUTFOG_CHAIN_SINGLE);
			}
		}
		else
		{
			pServiceResponser->SendResponse(TID_RecvQryCurrency, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
			pCurrency = m_pMDB->m_CurrencyFactory->findFirstBySettlementGroup(field.SettlementGroup);
			while (pCurrency != NULL)
			{
				CopyCurrencyEntity(&rspField, pCurrency);
				CurrencyValue(m_pMDB, &rspField);
				pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
				pCurrency = m_pMDB->m_CurrencyFactory->findNextBySettlementGroup();
			}
		}
	}
	pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
}

void SendTickMarketOrder(CMDB* m_pMDB, CServiceResponser* pServiceResponser, CQryMarketOrderField& field, TSessionRef* pSession)
{
	CMarketOrderField rspField;
	rspField.ExchangeID = field.ExchangeID;
	rspField.InstrumentID = field.InstrumentID;
	int nLevel = INT_32_MAX;
	if (!field.Depth.isNull())
		nLevel = field.Depth.getValue();
	string sIndex = field.ExchangeID.getValue();
	sIndex += "_";
	sIndex += field.InstrumentID.getValue();
	sIndex += "_";
	sIndex += field.Tick.getAllString();
	CTickMarketOrder* pMarketOrder = NULL;
	int j = 0;
	if (field.Direction.isNull() || field.Direction == D_Sell)
	{
		rspField.Direction = D_Sell;
		pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findFirstBySellMBLData(field.ExchangeID, field.InstrumentID, field.Tick);
		while (pMarketOrder != NULL)
		{
			if (j >= nLevel)
				break;
			rspField.Price = pMarketOrder->Price;
			rspField.Volume = pMarketOrder->Volume;
			rspField.Orders = pMarketOrder->Orders;
			rspField.TrueVolume = pMarketOrder->TrueVolume;
			rspField.BusinessNo = pMarketOrder->BusinessNo;
			if (j == 0)
				pServiceResponser->SendResponse(TID_RecvQryMarketOrder, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_FIRST, 0, sIndex.c_str());
			else
				pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextBySellMBLData();
			j++;
		}
	}
	int i = 0;
	if (field.Direction.isNull() || field.Direction == D_Buy)
	{
		rspField.Direction = D_Buy;
		pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findFirstByBuyMBLData(field.ExchangeID, field.InstrumentID, field.Tick);
		while (pMarketOrder != NULL)
		{
			if (i >= nLevel)
				break;
			rspField.Price = pMarketOrder->Price;
			rspField.Volume = pMarketOrder->Volume;
			rspField.Orders = pMarketOrder->Orders;
			rspField.TrueVolume = pMarketOrder->TrueVolume;
			rspField.BusinessNo = pMarketOrder->BusinessNo;
			if (i == 0 && j == 0)
				pServiceResponser->SendResponse(TID_RecvQryMarketOrder, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_FIRST, 0, sIndex.c_str());
			else
				pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextByBuyMBLData();
			i++;
		}
	}

	if ((i + j) > 0)
		pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	else
	{
		pServiceResponser->SendResponse(TID_RecvQryMarketOrder, NULL, NULL, pSession, OUTFOG_CHAIN_SINGLE, 0, "NoResult");
	}
}

bool RspError(CErrorEngine* pErrorEngine, CServiceResponser* pServiceResponser, int nRecvTNo, TSessionRef* pSession, void* pField, CFieldDescribe* pFieldDescribe)
{
	char* errMsg = NULL;
	int errorCode = 0;
	pErrorEngine->getLastError(&errorCode, &errMsg);

	if (pFieldDescribe == NULL)
	{
		CErrorField responseField;
		responseField.ErrorNo = errorCode;
		responseField.ErrorMsg = errMsg;
		pServiceResponser->SendResponse(nRecvTNo, &responseField, &(responseField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE, errorCode, errMsg);
	}
	else
		pServiceResponser->SendResponse(nRecvTNo, pField, pFieldDescribe, pSession, OUTFOG_CHAIN_SINGLE, errorCode, errMsg);
	return false;
}

void RspInstrument(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, TSessionRef* pSession, int nTopicNo)
{
	m_pServiceResponser->SendRspPub(TID_PushInstrument, NULL, NULL, nTopicNo, 3, pSession, "", OUTFOG_CHAIN_FIRST);
	CInstrumentField field;
	CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->getFirst();
	while (pInstrument != NULL)
	{
		CopyInstrumentEntity(&field, pInstrument);
		m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pInstrument = m_pMDB->m_InstrumentFactory->getNext();
	}
	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
}

void RspCurrency(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, TSessionRef* pSession, int nTopicNo)
{
	m_pServiceResponser->SendRspPub(TID_PushCurrency, NULL, NULL, nTopicNo, 3, pSession, "", OUTFOG_CHAIN_FIRST);
	CCurrencyField field;
	CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->getFirst();
	while (pCurrency != NULL)
	{
		CopyCurrencyEntity(&field, pCurrency);
		m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pCurrency = m_pMDB->m_CurrencyFactory->getNext();
	}
	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
}

void RspMarketData(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, TSessionRef* pSession, int nTopicID)
{
	m_pServiceResponser->SendRspPub(TID_PushMarketOverView, NULL, NULL, nTopicID, 3, pSession, "", OUTFOG_CHAIN_FIRST);
	CMarketOverViewField field;
	CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	while (pMarketData != NULL)
	{
		CopyMarketOverViewEntity(&field, pMarketData);
		m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pMarketData = m_pMDB->m_MarketDataFactory->getNext();
	}
	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
}

void RspMarketData(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, const char* ProductGroup, TSessionRef* pSession, int nTopicID)
{
	m_pServiceResponser->SendRspPub(TID_PushMarketOverView, NULL, NULL, nTopicID, 3, pSession, ProductGroup, OUTFOG_CHAIN_FIRST);
	CMarketOverViewField field;
	CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	while (pMarketData != NULL)
	{
		if (pMarketData->ProductGroup == ProductGroup)
		{
			CopyMarketOverViewEntity(&field, pMarketData);
			m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		}
		pMarketData = m_pMDB->m_MarketDataFactory->getNext();
	}
	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
}

bool RspMarketOrder(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, const char* pExchangeID, const char* pInstrumentID, int nLevel, TSessionRef* pSession, int nTopicID)
{
	CExchangeIDType ExchangeID = pExchangeID;
	CInstrumentIDType InstrumentID = pInstrumentID;
	CMarketOrderField field;
	//	field.ChangeType = DAT_Init;
	field.ExchangeID = pExchangeID;
	field.InstrumentID = pInstrumentID;
	field.Direction = D_Buy;
	string sIndex = ExchangeID.getValue();
	sIndex += "_";
	sIndex += InstrumentID.getValue();
	m_pServiceResponser->SendRspPub(TID_PushMarketOrder, NULL, NULL, nTopicID, 4, pSession, sIndex.c_str(), OUTFOG_CHAIN_FIRST);
	int i = 0;
	CMarketOrder* pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(ExchangeID, InstrumentID);
	while (pMarketOrder != NULL)
	{
		if (i > nLevel)
			break;
		field.Price = pMarketOrder->Price;
		field.Volume = pMarketOrder->Volume;
		field.Orders = pMarketOrder->Orders;
		field.BusinessNo = pMarketOrder->BusinessNo;
		field.TrueVolume = pMarketOrder->TrueVolume;
		m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextByBuyMBLData();
		i++;
	}

	field.Direction = D_Sell;
	int j = 0;
	pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(ExchangeID, InstrumentID);
	while (pMarketOrder != NULL)
	{
		if (j > nLevel)
			break;
		field.Price = pMarketOrder->Price;
		field.Volume = pMarketOrder->Volume;
		field.Orders = pMarketOrder->Orders;
		field.BusinessNo = pMarketOrder->BusinessNo;
		field.TrueVolume = pMarketOrder->TrueVolume;
		m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextBySellMBLData();
		j++;
	}
	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	return true;
}

bool RspMarketOrder(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, const char* pExchangeID, const char* pInstrumentID, const char* pTick, int nLevel, TSessionRef* pSession, int nTopicID)
{
	CExchangeIDType ExchangeID = pExchangeID;
	CInstrumentIDType InstrumentID = pInstrumentID;
	CPriceType Tick = atof(pTick);
	CMarketOrderField field;
	//	field.ChangeType = DAT_Init;
	field.ExchangeID = pExchangeID;
	field.InstrumentID = pInstrumentID;
	field.Direction = D_Buy;
	int i = 0;
	string sIndex = ExchangeID.getValue();
	sIndex += "_";
	sIndex += InstrumentID.getValue();
	sIndex += "_";
	sIndex += pTick;
	m_pServiceResponser->SendRspPub(TID_PushMarketOrder, NULL, NULL, nTopicID, 4, pSession, sIndex.c_str(), OUTFOG_CHAIN_FIRST);
	CTickMarketOrder* pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findFirstByBuyMBLData(ExchangeID, InstrumentID, Tick);
	while (pMarketOrder != NULL)
	{
		if (i > nLevel)
			break;
		field.Price = pMarketOrder->Price;
		field.Volume = pMarketOrder->Volume;
		field.Orders = pMarketOrder->Orders;
		field.BusinessNo = pMarketOrder->BusinessNo;
		field.TrueVolume = pMarketOrder->TrueVolume;
		m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextByBuyMBLData();
		i++;
	}

	field.Direction = D_Sell;
	int j = 0;
	pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findFirstBySellMBLData(ExchangeID, InstrumentID, Tick);
	while (pMarketOrder != NULL)
	{
		if (j > nLevel)
			break;
		field.Price = pMarketOrder->Price;
		field.Volume = pMarketOrder->Volume;
		field.Orders = pMarketOrder->Orders;
		field.BusinessNo = pMarketOrder->BusinessNo;
		field.TrueVolume = pMarketOrder->TrueVolume;
		m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextBySellMBLData();
		j++;
	}
	if ((i + j) > 0)
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	return true;
}

void RspMarketOrder(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, TSessionRef* pSession, int nTNo)
{
	CMarketOrderField field;
	int i = 0;
	CMarketOrder* pMarketOrder = m_pMDB->m_MarketOrderFactory->getFirst();
	while (pMarketOrder != NULL)
	{
		field.ExchangeID = pMarketOrder->ExchangeID;
		field.InstrumentID = pMarketOrder->InstrumentID;
		field.Price = pMarketOrder->Price;
		field.Volume = pMarketOrder->Volume;
		field.Direction = pMarketOrder->Direction;
		field.Orders = pMarketOrder->Orders;
		field.TrueVolume = pMarketOrder->TrueVolume;
		field.BusinessNo = pMarketOrder->BusinessNo;
		if (i == 0)
			m_pServiceResponser->SendResponse(TID_PushMarketOrder, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
		else
			m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pMarketOrder = m_pMDB->m_MarketOrderFactory->getNext();
		i++;
	}
	if (i > 0)
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);

	CTickMarketOrderField field1;
	CTickMarketOrder* pTickMarketOrder = m_pMDB->m_TickMarketOrderFactory->getFirst();
	while (pTickMarketOrder != NULL)
	{
		field1.ExchangeID = pTickMarketOrder->ExchangeID;
		field1.InstrumentID = pTickMarketOrder->InstrumentID;
		field1.Price = pTickMarketOrder->Price;
		field1.Volume = pTickMarketOrder->Volume;
		field1.Direction = pTickMarketOrder->Direction;
		field1.Orders = pTickMarketOrder->Orders;
		field1.TrueVolume = pTickMarketOrder->TrueVolume;
		field1.BusinessNo = pTickMarketOrder->BusinessNo;
		field1.Tick = pTickMarketOrder->Tick;
		if (i == 0)
			m_pServiceResponser->SendResponse(TID_PushMarketOrder, &field1, &(field1.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
		else
			m_pServiceResponser->SendNext(&field1, &(field1.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pTickMarketOrder = m_pMDB->m_TickMarketOrderFactory->getNext();
		i++;
	}
	if (i > 0)
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
}

void RspKLine(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, TSessionRef* pSession)
{
	CKLineField field;
	int i = 0;
	CPublishKLine* pKLine = m_pMDB->m_PublishKLineFactory->getFirst();
	while (pKLine != NULL)
	{
		CopyKLineEntity(&field, pKLine);
		if (i == 0)
			m_pServiceResponser->SendResponse(TID_PushKLine, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
		else
			m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pKLine = m_pMDB->m_PublishKLineFactory->getNext();
		i++;
	}
	if (i > 0)
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
}