#include "public.h"
#include "KlineTriggers.h"
#include "CopyField.h"
#include "printinfo.h"
#include "monitorIndex.h"
#include "tradingTools.h"

////////////////////////////////////////////////////////////////////////
CKLineTrigger::CKLineTrigger(CMDB *pMDB)
{
	m_pMDB = pMDB;
}

CKLineTrigger::~CKLineTrigger()
{
}

void CKLineTrigger::commitAdd(CTrade *pTrade)
{
	if (!needShowPrice(m_pMDB, pTrade))
		return;

	//只发送单边，发送买方
	if (pTrade->Direction == D_Buy)
	{
		CNumberType TimeZone = m_pMDB->m_CurrentTimeFactory->getFirst()->TimeZone;
		UpdateLastKLine(pTrade, "1m");
		UpdateLastKLine(pTrade, "5m");
		UpdateLastKLine(pTrade, "1h");
		UpdateLastKLine(pTrade, "1d");
		UpdateLastKLine(pTrade, "1w");
		UpdateLastKLine(pTrade, "1o");
	}
}

void CKLineTrigger::commitUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pOldCurrentTime)
{
	CWriteableCurrentTime theCurrentTime;
	memcpy(&theCurrentTime, pCurrentTime, sizeof(CWriteableCurrentTime));

	//如果重启间隔时间较长，tradeTime和最新K线的时间间隔超过一条K线的时间间隔，需要生成一条新的K线
	if (pCurrentTime->CurrTime >= pOldCurrentTime->CurrTime + 60)
	{
		//theCurrentTime.CurrTime -= fmod(pCurrentTime->CurrTime.getValue(), 60);
		AddKLineByTime(&theCurrentTime, "1m");
		return;
	}

	if (pCurrentTime->CurrTime >= pOldCurrentTime->CurrTime + 5 * 60)
	{
		//theCurrentTime.CurrTime -= fmod(pCurrentTime->CurrTime, (5 * 60));
		AddKLineByTime(&theCurrentTime, "5m");
		return;
	}

	if (pCurrentTime->CurrTime >= pOldCurrentTime->CurrTime + 60 * 60)
	{
		//theCurrentTime.CurrTime -= fmod(pCurrentTime->CurrTime, 60 * 60);
		AddKLineByTime(&theCurrentTime, "1d");
		return;
	}

	if (pCurrentTime->CurrTime >= pOldCurrentTime->CurrTime + 7 * 60 * 60)
	{
		//一个礼拜精确控制起点了，只能先闭合K线，然后按目前时间生成一个
		AddKLineByTime(&theCurrentTime, "1w");
		return;
	}

	if (pCurrentTime->CurrTime >= pOldCurrentTime->CurrTime + 31 * 60 * 60)
	{
		AddKLineByTime(&theCurrentTime, "1o");
		return;		
	}

	//20:30:21	  10:32:10
	//20211206    20211207
	//7                2

	//if (m_LastTime.isNull() || m_LastDate.isNull() || m_LastDays.isNull())
	//{
	//	m_LastTime = pCurrentTime->ZoneTime;
	//	m_LastDate = pCurrentTime->ZoneDate;
	//	m_LastDays = pCurrentTime->ZoneDays;
	//}
	//else
	//{
	if (pCurrentTime->ZoneTime.getValue()[4] != pOldCurrentTime->ZoneTime.getValue()[4])
	{
		AddKLineByTime(&theCurrentTime, "1m");
		if (pCurrentTime->ZoneTime.getValue()[4] == '5' || pCurrentTime->ZoneTime.getValue()[4] == '0')
			AddKLineByTime(&theCurrentTime, "5m");
	}

	if (pCurrentTime->ZoneTime.getValue()[1] != pOldCurrentTime->ZoneTime.getValue()[1])
		AddKLineByTime(&theCurrentTime, "1h");


	if (pCurrentTime->ZoneDate.getValue()[7] != pOldCurrentTime->ZoneDate.getValue()[7])
	{
		AddKLineByTime(&theCurrentTime, "1d");
	}
	else
	{
		if (pOldCurrentTime->ZoneDate.getValue()[6] == '3' && pOldCurrentTime->ZoneDate.getValue()[7] == '1')
		{
			if (pCurrentTime->ZoneDate.getValue()[6] == '0' && pCurrentTime->ZoneDate.getValue()[7] == '1')
				AddKLineByTime(&theCurrentTime, "1d");
		}
	}

	if (pCurrentTime->ZoneDate.getValue()[5] != pOldCurrentTime->ZoneDate.getValue()[5])
		AddKLineByTime(&theCurrentTime, "1o");

	if (pCurrentTime->ZoneDays.getValue()[0] != pOldCurrentTime->ZoneDays.getValue()[0])
	{
		if (pCurrentTime->ZoneDays.getValue()[0] == '1')
			AddKLineByTime(&theCurrentTime, "1w");
	}

	//	m_LastTime = pCurrentTime->ZoneTime;
	//	m_LastDate = pCurrentTime->ZoneDate;
	//	m_LastDays = pCurrentTime->ZoneDays;
	//}

	//m_LastMilliSeconds = pCurrentTime->CurrTime;
}

bool CKLineTrigger::AddKLineByTime(CWriteableCurrentTime *pCurrentTime, CBarType Bar)
{
	CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	while (pMarketData != NULL)
	{
		if (pMarketData->InstrumentStatus != IS_Continous)
		{
			pMarketData = m_pMDB->m_MarketDataFactory->getNext();
			continue;
		}

		if (pMarketData->getInstrument() == NULL)
		{
			pMarketData->linkInstrument(m_pMDB->m_InstrumentFactory->findByInstrumentID(pMarketData->ExchangeID, pMarketData->InstrumentID));
			if (pMarketData->getInstrument() == NULL)
			{
				pMarketData = m_pMDB->m_MarketDataFactory->getNext();
				continue;
			}
		}

		AddKLineByTime(pCurrentTime, pMarketData->ExchangeID, pMarketData->InstrumentID, Bar);

		//if (!pMarketData->UnderlyingPrice.isNull())
		//	AddKLineByTime(pCurrentTime, pMarketData->getInstrument(), getUnderlyingInstrumentID(pMarketData->InstrumentID), Bar);
		if (!pMarketData->MarkedPrice.isNull())
			AddKLineByTime(pCurrentTime, pMarketData->ExchangeID, getMarkedInstrumentID(pMarketData->InstrumentID), Bar);

		if (!pMarketData->UnderlyingPrice.isNull())
		{
			CInstrumentIDType UnderlyingInstrumentID = "i";
			UnderlyingInstrumentID += pMarketData->InstrumentID;
			AddKLineByTime(pCurrentTime, pMarketData->ExchangeID, UnderlyingInstrumentID, Bar);
		}
		pMarketData = m_pMDB->m_MarketDataFactory->getNext();
	}
	return true;
}

void CKLineTrigger::AddKLineByTime(CCurrentTime *pCurrentTime, CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CBarType Bar)
{
	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(ExchangeID, InstrumentID, Bar);
	if (pLastKLine != NULL)
	{
		//InsertKLine(pLastKLine);
		CWriteableLastKLine theLastKline;	//准备新加入LastKLine的K线
		m_pMDB->m_LastKLineFactory->retrieve(pLastKLine, &theLastKline);
		//不完整的最新基础K线

		theLastKline.InsertTime = pCurrentTime->CurrTime.getValue() * 1000;
		theLastKline.OpenPrice = pLastKLine->ClosePrice;
		theLastKline.ClosePrice = pLastKLine->ClosePrice;
		theLastKline.HighestPrice = pLastKLine->ClosePrice;
		theLastKline.LowestPrice = pLastKLine->ClosePrice;
		theLastKline.Volume = 0;
		theLastKline.Turnover = 0.0;
		m_pMDB->m_LastKLineFactory->update(pLastKLine, &theLastKline);
	}
}

void CKLineTrigger::UpdateLastKLine(CTrade* pTrade, CBarType Bar)
{
	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pTrade->ExchangeID, pTrade->InstrumentID, Bar);

	CWriteableLastKLine theLastKLine;
	if (pLastKLine == NULL)
	{
		//REPORT_EVENT(LOG_CRITICAL, "SetLastKLine", "NoLastKLine:%s_%s_%s", pTrade->ExchangeID.getValue(), pTrade->InstrumentID.getValue(), Bar.getValue());
		theLastKLine.init();
		theLastKLine.ExchangeID = pTrade->ExchangeID;
		theLastKLine.InstrumentID = pTrade->InstrumentID;
		theLastKLine.Bar = Bar;
		theLastKLine.InsertTime = pTrade->InsertTime;
		theLastKLine.OpenPrice = pTrade->Price;
		theLastKLine.HighestPrice = pTrade->Price;
		theLastKLine.LowestPrice = pTrade->Price;
		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pTrade->ExchangeID, pTrade->InstrumentID);
		CCurrentTime *pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
		//theLastKLine.SerialNo = GetSerialNo(pInstrument, pCurrentTime, Bar);
	}
	else
	{
		m_pMDB->m_LastKLineFactory->retrieve(pLastKLine, &theLastKLine);
	}

	//这里采用两种选择，一种为K线连续没有跳空，一种是需要跳空来
	if (g_bKLineGap && theLastKLine.Volume == 0.0)
	{
		//pKLine->OpenPrice = pTrade->Price;
		theLastKLine.HighestPrice = pTrade->Price;
		theLastKLine.LowestPrice = pTrade->Price;
	}
	else
	{
		if (pTrade->Price > theLastKLine.HighestPrice)
			theLastKLine.HighestPrice = pTrade->Price;
		if (pTrade->Price < theLastKLine.LowestPrice)
			theLastKLine.LowestPrice = pTrade->Price;
	}

	//	theLastKLine.LastTradeID = pTrade->TradeID;
	theLastKLine.ClosePrice = pTrade->Price;
	theLastKLine.Volume += pTrade->Volume;
	if (g_bDoubleMarketVolume)
		theLastKLine.Volume += pTrade->Volume;
	theLastKLine.Turnover += pTrade->Turnover;
	if (g_bDoubleMarketVolume)
		theLastKLine.Turnover += pTrade->Turnover;

	if (pLastKLine == NULL)
		m_pMDB->m_LastKLineFactory->add(&theLastKLine);
	else
		m_pMDB->m_LastKLineFactory->update(pLastKLine, &theLastKLine);
}

CMarketTradeTrigger::CMarketTradeTrigger(CServiceResponser *pServiceResponser, CMDB *pMDB)
{
	m_pServiceResponser = pServiceResponser;
	m_pMDB = pMDB;
}

CMarketTradeTrigger::~CMarketTradeTrigger()
{
}

void CMarketTradeTrigger::commitAdd(CTrade *pTrade)
{
	if (!needShowPrice(m_pMDB, pTrade))
		return;

	//只发送单边，发送taker的那一边
	switch (pTrade->MatchRole)
	{
	case MR_None:
	{
		if (pTrade->Direction == D_Sell)
			return;

		CMarketTradeField field;
		CopyMarketTradeEntity(&field, pTrade);
		if (g_bDoubleMarketVolume)
			field.Volume = field.Volume.getValue() * 2;
		//如果是没有taker和maker的情况下，让前端展示空
		field.Direction = '\0';
		sprintf(m_pServiceResponser->GetMetaRef()->SubIndexName, "%s_%s", field.ExchangeID.getValue(), field.InstrumentID.getValue());
		//m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
		//m_pServiceResponser->SendToPublic(TID_PushMarketTrade,&field,&(CMarketTradeField::m_Describe));
		m_pServiceResponser->SendToSubjectID(TID_PushMarketTrade, &field, &(CMarketTradeField::m_Describe), OF_Topic_Market_LastTrade, 4, UF_INT8_MAX);
		break;
	}
	case MR_Taker:
	{
		CMarketTradeField field;
		CopyMarketTradeEntity(&field, pTrade);
		if (g_bDoubleMarketVolume)
			field.Volume = field.Volume.getValue() * 2;
		sprintf(m_pServiceResponser->GetMetaRef()->SubIndexName, "%s_%s", field.ExchangeID.getValue(), field.InstrumentID.getValue());
		//m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
		//m_pServiceResponser->SendToPublic(TID_PushMarketTrade,&field,&(CMarketTradeField::m_Describe));
		m_pServiceResponser->SendToSubjectID(TID_PushMarketTrade, &field, &(CMarketTradeField::m_Describe), OF_Topic_Market_LastTrade, 4, UF_INT8_MAX);
		break;
	}
	case MR_Maker:
	default:
	{
		return;
	}
	}
}