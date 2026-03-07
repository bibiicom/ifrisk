#include "public.h"
#include "MarketDataTriggers.h"
#include "CopyField.h"
#include "printinfo.h"
#include "monitorIndex.h"
#include "tradingTools.h"

CMarketOrdersNotifyTrigger::CMarketOrdersNotifyTrigger(CMarketDataService *pMarketDataService)
{
	m_pMarketDataService = pMarketDataService;
}

void CMarketOrdersNotifyTrigger::commitAdd(CMarketOrders *pMarketOrders)
{
	CMarketOrderField field;
//	field.ChangeType = DAT_Insert;
	CopyBaseMarketOrderEntity(&field, pMarketOrders);
	string SubIndexName = field.ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += field.InstrumentID.getValue();
	m_pMarketDataService->SetMarketOrder(SubIndexName.c_str(), &field);
}

void CMarketOrdersNotifyTrigger::commitUpdate(CMarketOrders *pMarketOrders, CWriteableMarketOrders *pOldMarketOrders)
{
	CMarketOrderField field;
//	field.ChangeType = DAT_Update;
	CopyBaseMarketOrderEntity(&field, pMarketOrders);
	string SubIndexName = field.ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += field.InstrumentID.getValue();
	m_pMarketDataService->SetMarketOrder(SubIndexName.c_str(), &field);
}

void CMarketOrdersNotifyTrigger::commitRemove(CWriteableMarketOrders *pMarketOrders)
{
	CMarketOrderField field;
//	field.ChangeType = DAT_Delete;
	CopyBaseMarketOrderEntity(&field, pMarketOrders);
	field.Volume = 0.0;
	field.OrderNumber = 0;
	string SubIndexName = field.ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += field.InstrumentID.getValue();
	m_pMarketDataService->SetMarketOrder(SubIndexName.c_str(), &field);
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
		{
			return;
		}
		CMarketTradeField field;
		CopyMarketTradeEntity(&field, pTrade);
		if (g_bDoubleMarketVolume)
			field.Volume = field.Volume * 2;
		//如果是没有taker和maker的情况下，让前端展示空
		field.Direction = '\0';
		sprintf(m_pServiceResponser->GetMetaRef()->SubIndexName, "%s_%s", field.ExchangeID.getValue(), field.InstrumentID.getValue());
		m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
		//m_pServiceResponser->SendToPublic(TID_PushMarketTrade,&field,&(CMarketTradeField::m_Describe));
		m_pServiceResponser->SendToSubjectID(TID_PushMarketTrade, &field, &(CMarketTradeField::m_Describe), OF_Topic_Market_LastTrade);
		break;
	}
	case MR_Taker:
	{
		CMarketTradeField field;
		CopyMarketTradeEntity(&field, pTrade);
		if (g_bDoubleMarketVolume)
			field.Volume = field.Volume * 2;
		sprintf(m_pServiceResponser->GetMetaRef()->SubIndexName, "%s_%s", field.ExchangeID.getValue(), field.InstrumentID.getValue());
		m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
		//m_pServiceResponser->SendToPublic(TID_PushMarketTrade,&field,&(CMarketTradeField::m_Describe));
		m_pServiceResponser->SendToSubjectID(TID_PushMarketTrade, &field, &(CMarketTradeField::m_Describe), OF_Topic_Market_LastTrade);
		break;
	}
	case MR_Maker:
	default:
	{
		return;
	}
	}
}

////////////////////////////////////////////////////////////////////////
CKLineTrigger::CKLineTrigger(CMDB *pMDB)
{
	m_pMDB = pMDB;
	m_bTimeUpdate = true;
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
		CTransaction transaction;
#if 0
		SetLastKLine(pTrade, "1m", &transaction);
		SetLastKLine(pTrade, "1h", &transaction);
		SetLastKLine(pTrade, "1d", &transaction);
		SetLastKLine(pTrade, "1w", &transaction);
		SetLastKLine(pTrade, "1o", &transaction);
#else
		SetLastKLineByBeginTime(pTrade, "1m", &transaction);
		SetLastKLineByBeginTime(pTrade, "1h", &transaction);
		SetLastKLineByBeginTime(pTrade, "1d", &transaction);
		SetLastKLineByBeginTime(pTrade, "1w", &transaction);
		SetLastKLineByBeginTime(pTrade, "1o", &transaction);
#endif
		transaction.commit();
	}
}

void CKLineTrigger::commitAdd(CMarketData* pMarketData)
{
	SetMarkKLineByMarketData(pMarketData);
}

void CKLineTrigger::commitUpdate(CMarketData* pMarketData, CWriteableMarketData *pOldMarketData)
{
	SetMarkKLineByMarketData(pMarketData);
}

void CKLineTrigger::SetMarkKLineByMarketData(CMarketData* pMarketData)
{
	if (pMarketData->UpdateTime == 0)
	{
		return;
	}

	CTransaction transaction;

	SetLastMarkKLineByBeginTime(pMarketData, "1m", &transaction);
	SetLastMarkKLineByBeginTime(pMarketData, "1h", &transaction);
	SetLastMarkKLineByBeginTime(pMarketData, "1d", &transaction);
	SetLastMarkKLineByBeginTime(pMarketData, "1w", &transaction);
	SetLastMarkKLineByBeginTime(pMarketData, "1o", &transaction);

	transaction.commit();
}

void CKLineTrigger::commitUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pOldCurrentTime)
{
	if (m_LastTime.isNull() || m_LastDate.isNull() || m_LastDays.isNull())
	{
		m_LastTime = pCurrentTime->ZoneTime;
		m_LastDate = pCurrentTime->ZoneDate;
		m_LastDays = pCurrentTime->ZoneDays;
		return;
	}

	//如果是非交易，不进行K线的生成
	if (!m_bTimeUpdate)
		return;

	CTransaction transaction;
	if (pCurrentTime->ZoneTime.getValue()[4] != m_LastTime.getValue()[4])
	{
		//AddKLineTimeUp(pCurrentTime, "1m", &transaction);
		AddKLineTimeUp2(pCurrentTime, "1m", &transaction);
		AddMarkKLineTimeUp2(pCurrentTime, "1m", &transaction);
	}

	if (pCurrentTime->ZoneTime.getValue()[1] != m_LastTime.getValue()[1])
	{
		//AddKLineTimeUp(pCurrentTime, "1h", &transaction);
		AddKLineTimeUp2(pCurrentTime, "1h", &transaction);
		AddMarkKLineTimeUp2(pCurrentTime, "1h", &transaction);
	}

	if (pCurrentTime->ZoneDate.getValue()[7] != m_LastDate.getValue()[7])
	{
		//AddKLineTimeUp(pCurrentTime, "1d", &transaction);
		AddKLineTimeUp2(pCurrentTime, "1d", &transaction);
		AddMarkKLineTimeUp2(pCurrentTime, "1d", &transaction);
	}
	else
	{
		if (m_LastDate.getValue()[6] == '3' && m_LastDate.getValue()[7] == '1')
		{
			if (pCurrentTime->ZoneDate.getValue()[6] == '0' && pCurrentTime->ZoneDate.getValue()[7] == '1')
			{
				//AddKLineTimeUp(pCurrentTime, "1d", &transaction);
				AddKLineTimeUp2(pCurrentTime, "1d", &transaction);
				AddMarkKLineTimeUp2(pCurrentTime, "1d", &transaction);
			}
		}
	}

	if (pCurrentTime->ZoneDate.getValue()[5] != m_LastDate.getValue()[5])
	{
		//AddKLineTimeUp(pCurrentTime, "1o", &transaction);
		AddKLineTimeUp2(pCurrentTime, "1o", &transaction);
		AddMarkKLineTimeUp2(pCurrentTime, "1o", &transaction);
	}

	if (pCurrentTime->ZoneDays.getValue()[0] != m_LastDays.getValue()[0])
	{
		if (pCurrentTime->ZoneDays.getValue()[0] == '0')
		{
			//AddKLineTimeUp(pCurrentTime, "1w", &transaction);
			AddKLineTimeUp2(pCurrentTime, "1w", &transaction);
			AddMarkKLineTimeUp2(pCurrentTime, "1w", &transaction);
		}
	}

	transaction.commit();

	m_LastTime = pCurrentTime->ZoneTime;
	m_LastDate = pCurrentTime->ZoneDate;
	m_LastDays = pCurrentTime->ZoneDays;
}

void CKLineTrigger::commitUpdate(CInstrument *pInstrument, CWriteableInstrument *pOldInstrument)
{
	if (pInstrument->getMarketData()->InstrumentStatus == IS_Continous)
		m_bTimeUpdate = true;
	else
		m_bTimeUpdate = false;
}

void CKLineTrigger::commitRemove(CWriteableInstrument *pInstrument)
{
	m_bTimeUpdate = false;
}

int CKLineTrigger::GetKLineNo(CInstrument *pInstrument,CCurrentTime *pCurrentTime, CIndexIDType Period)
{
	if (pInstrument == NULL || pCurrentTime == NULL)
		return -1;

	if (Period == "1m")
	{
		return getMiniteMinus(pCurrentTime->ZoneTime.getValue(), pInstrument->DayStartTime.getValue());
	}
	else if (Period == "1h")
	{
		return getHourMinus(pCurrentTime->ZoneTime.getValue(), pInstrument->DayStartTime.getValue());
	}
	else if(Period == "1d")
	{
		return atoi(pCurrentTime->ZoneDays.getValue() + 2);
	}
	else if (Period == "1w")
	{
		int nDays = atoi(pCurrentTime->ZoneDays.getValue() + 2);
		int nLossDays = nDays % 7;
		if (nLossDays == 0)
		{
			return (nDays - nLossDays) / 7 + 1;
		}
		else
		{
			if (pCurrentTime->ZoneDays.getValue()[0] == '0')
				return (nDays - nLossDays) / 7 + 2;
			else
				return (nDays - nLossDays) / 7 + 1;
		}
	}
	else if (Period == "1o")
	{
		char Month[2];
		Month[0] = pCurrentTime->ZoneDate.getValue()[4];
		Month[1] = pCurrentTime->ZoneDate.getValue()[5];
		return atoi(Month) - 1;
	}
	else
	{
		return 0;
	}
}

bool CKLineTrigger::AddKLineTimeUp(CCurrentTime *pCurrentTime, CIndexIDType Period, CTransaction *pTransaction)
{
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->getFirst();
	while (pInstrument != NULL)
	{
		CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriod(pInstrument->ExchangeID, pInstrument->InstrumentID, Period);
		m_pMDB->m_LastKLineFactory->endFindByHisPeriod();
		CWriteableKLine theKline;		//准备写入历史的K线
		CWriteableLastKLine theLastKline;	//准备新加入LastKLine的K线

		if (pLastKLine != NULL)
		{
			int nKLineNo = GetKLineNo(pInstrument, pCurrentTime, Period);
			if (nKLineNo == pLastKLine->KLineNo || nKLineNo < 0)
			{
				pInstrument = m_pMDB->m_InstrumentFactory->getNext();
				REPORT_EVENT(LOG_CRITICAL, "AddKLineTimeUp", "theNewLastKline[%d]==theLastKline[%d]", nKLineNo, pLastKLine->KLineNo.getValue());
				continue;
			}
			CopyKLineEntity(&theKline, pLastKLine);

			//之前的最新K线要插入到历史库中
			theKline.UpdateTime = pCurrentTime->CurrTime;
			InsertKLine(&theKline, pTransaction);

			//不完整的最新基础K线
			theLastKline.ExchangeID = pLastKLine->ExchangeID;
			theLastKline.InstrumentID = pLastKLine->InstrumentID;
			theLastKline.LastTradeID = pLastKLine->LastTradeID;
			theLastKline.PeriodID = pLastKLine->PeriodID;
			theLastKline.BeginTime = pCurrentTime->CurrTime;
			theLastKline.UpdateTime = pCurrentTime->CurrTime;
			theLastKline.OpenPrice = pLastKLine->ClosePrice;
			theLastKline.ClosePrice = pLastKLine->ClosePrice;
			theLastKline.HighestPrice = pLastKLine->ClosePrice;
			theLastKline.LowestPrice = pLastKLine->ClosePrice;
			theLastKline.Volume = 0;
			theLastKline.Turnover = 0.0;
			theLastKline.KLineNo = nKLineNo;

			//周期重新开始，删除前一周期所有数据
			if (theLastKline.KLineNo < pLastKLine->KLineNo)
			{
				CLastKLine *pLastKLineRemove = m_pMDB->m_LastKLineFactory->findFirstByHisPeriod(pInstrument->ExchangeID, pInstrument->InstrumentID, Period);
				while (pLastKLineRemove != NULL)
				{
					CLastKLine *pToBeRemove = pLastKLineRemove;
					pLastKLineRemove = m_pMDB->m_LastKLineFactory->findNextByHisPeriod();
					m_pMDB->m_LastKLineFactory->remove(pToBeRemove, pTransaction);
				}
				m_pMDB->m_LastKLineFactory->endFindByHisPeriod();
			}
			m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
		}

		pInstrument = m_pMDB->m_InstrumentFactory->getNext();
	}
	m_pMDB->m_InstrumentFactory->endGet();
	return true;
}

bool CKLineTrigger::AddKLineTimeUp2(CCurrentTime *pCurrentTime, CIndexIDType Period, CTransaction *pTransaction)
{
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->getFirst();
	while (pInstrument != NULL)
	{
		CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriodBeginTime(pInstrument->ExchangeID, pInstrument->InstrumentID, Period);
		m_pMDB->m_LastKLineFactory->endFindByHisPeriodBeginTime();
		CWriteableKLine theKline;		//准备写入历史的K线
		CWriteableLastKLine theLastKline;	//准备新加入LastKLine的K线

		if (pLastKLine != NULL)
		{
			int nSerialNo = GetKLineNo2(pInstrument, pCurrentTime, Period);
			int nKLineNo = GetKLineNo3(pCurrentTime->CurrTime, Period);
			if (nSerialNo == pLastKLine->SerialNo || nSerialNo < 0)
			{
				pInstrument = m_pMDB->m_InstrumentFactory->getNext();
				REPORT_EVENT(LOG_CRITICAL, "AddKLineTimeUp", "theNewLastKline[%d]==theLastKline[%d]", nSerialNo, pLastKLine->SerialNo.getValue());
				continue;
			}
			CopyKLineEntity(&theKline, pLastKLine);

			//之前的最新K线要插入到历史库中
			theKline.UpdateTime = pCurrentTime->CurrTime;
			//InsertKLineByBeginTime(&theKline, pTransaction);
			InsertKLineByBeginTime2(&theKline, pTransaction);

			//不完整的最新基础K线
			theLastKline.ExchangeID = pLastKLine->ExchangeID;
			theLastKline.InstrumentID = pLastKLine->InstrumentID;
			theLastKline.LastTradeID = pLastKLine->LastTradeID;
			theLastKline.PeriodID = pLastKLine->PeriodID;
			theLastKline.BeginTime = pCurrentTime->CurrTime % 60 == 0 ? pCurrentTime->CurrTime : FormatBeginTime(pCurrentTime->CurrTime, pLastKLine->PeriodID);
			theLastKline.UpdateTime = pCurrentTime->CurrTime;
			theLastKline.OpenPrice = pLastKLine->ClosePrice;
			theLastKline.ClosePrice = pLastKLine->ClosePrice;
			theLastKline.HighestPrice = pLastKLine->ClosePrice;
			theLastKline.LowestPrice = pLastKLine->ClosePrice;
			theLastKline.Volume = 0;
			theLastKline.Turnover = 0.0;
			theLastKline.SerialNo = nSerialNo;
			theLastKline.KLineNo = nKLineNo;

			//更新对应LineNo的K线数据
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pInstrument->ExchangeID, pInstrument->InstrumentID, Period, nKLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}

		pInstrument = m_pMDB->m_InstrumentFactory->getNext();
	}
	m_pMDB->m_InstrumentFactory->endGet();
	return true;
}

void CKLineTrigger::InsertKLine(CWriteableKLine *pKline, CTransaction *pTransaction)
{
	m_pMDB->m_KLineFactory->add(pKline, pTransaction);

	//增加基础合约行情的K线，以便连续
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pKline->ExchangeID, pKline->InstrumentID)->getAliasInstrument();
	if (pInstrument != NULL)
	{
		pKline->InstrumentID = pInstrument->InstrumentID;
		pKline->ExchangeID = pInstrument->ExchangeID;
		m_pMDB->m_KLineFactory->add(pKline, pTransaction);
	}

	CSecondsType UpdateTime = pKline->UpdateTime;

	int currentKLineNo = pKline->KLineNo.getValue();
	if (pKline->PeriodID == "1m")
	{
		if ((currentKLineNo + 1) % 5 == 0)
		{
			getLastKLine(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "5m");
			pKline->UpdateTime = UpdateTime;
			m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
		}
		if ((currentKLineNo + 1) % 15 == 0)
		{
			getLastKLine(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "15m");
			//pKline->UpdateTime = UpdateTime;
			//m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
		}
		if ((currentKLineNo + 1) % 30 == 0)
		{
			getLastKLine(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "30m");
			//pKline->UpdateTime = UpdateTime;
			//m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
		}
	}
	else if (pKline->PeriodID == "1h")
	{
		if ((currentKLineNo + 1) % 4 == 0)
		{
			getLastKLine(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "4h");
			pKline->UpdateTime = UpdateTime;
			m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
		}
		if ((currentKLineNo + 1) % 12 == 0)
		{
			getLastKLine(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "12h");
			//pKline->UpdateTime = UpdateTime;
			//m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
		}
	}
	else if (pKline->PeriodID == "1o")
	{
		if ((currentKLineNo + 1) % 12 == 0)
		{
			getLastKLine(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "12o");
			pKline->PeriodID = "1y";
			pKline->UpdateTime = UpdateTime;
			m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
		}
	}
}

void CKLineTrigger::InsertKLineByBeginTime(CWriteableKLine *pKline, CTransaction *pTransaction)
{
	m_pMDB->m_KLineFactory->add(pKline, pTransaction);

	//增加基础合约行情的K线，以便连续
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pKline->ExchangeID, pKline->InstrumentID)->getAliasInstrument();
	if (pInstrument != NULL)
	{
		pKline->InstrumentID = pInstrument->InstrumentID;
		pKline->ExchangeID = pInstrument->ExchangeID;
		m_pMDB->m_KLineFactory->add(pKline, pTransaction);
	}

	CSecondsType UpdateTime = pKline->UpdateTime;

	int currentKLineNo = pKline->KLineNo.getValue();
	if (pKline->PeriodID == "1m")
	{
		if ((currentKLineNo + 1) % 5 == 0)
		{
			getLastKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "5m");
			pKline->UpdateTime = UpdateTime;
			m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
		if ((currentKLineNo + 1) % 15 == 0)
		{
			getLastKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "15m");
			pKline->UpdateTime = UpdateTime;
			m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
		if ((currentKLineNo + 1) % 30 == 0)
		{
			getLastKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "30m");
			pKline->UpdateTime = UpdateTime;
			m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
	}
	else if (pKline->PeriodID == "1h")
	{
		if ((currentKLineNo + 1) % 4 == 0)
		{
			getLastKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "4h");
			pKline->UpdateTime = UpdateTime;
			m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
		if ((currentKLineNo + 1) % 12 == 0)
		{
			getLastKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "12h");
			pKline->UpdateTime = UpdateTime;
			m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
	}
	else if (pKline->PeriodID == "1o")
	{
		if ((currentKLineNo + 1) % 12 == 0)
		{
			getLastKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "12o");
			pKline->PeriodID = "1y";
			pKline->UpdateTime = UpdateTime;
			m_pMDB->m_KLineFactory->add(pKline, pTransaction);

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
	}
}

void CKLineTrigger::InsertKLineByBeginTime2(CWriteableKLine *pKline, CTransaction *pTransaction)
{
	CKLine *pOldKLine = m_pMDB->m_KLineFactory->findByInstrumentPeriodBeginTime(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->BeginTime);
	if (pOldKLine == NULL)
	{
		m_pMDB->m_KLineFactory->add(pKline, pTransaction);
	}
	else
	{
		CWriteableKLine writeableKLine;
		m_pMDB->m_KLineFactory->retrieve(pOldKLine, &writeableKLine);
		CopyLastKLineEntity(&writeableKLine, pKline);
		m_pMDB->m_KLineFactory->update(pOldKLine, &writeableKLine, pTransaction);
	}

	//增加基础合约行情的K线，以便连续
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pKline->ExchangeID, pKline->InstrumentID)->getAliasInstrument();
	if (pInstrument != NULL)
	{
		pKline->InstrumentID = pInstrument->InstrumentID;
		pKline->ExchangeID = pInstrument->ExchangeID;
		CKLine *pOldAliasKLine = m_pMDB->m_KLineFactory->findByInstrumentPeriodBeginTime(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->BeginTime);
		if (pOldAliasKLine == NULL)
		{
			m_pMDB->m_KLineFactory->add(pKline, pTransaction);
		}
		else
		{
			CWriteableKLine writeableAliasKLine;
			m_pMDB->m_KLineFactory->retrieve(pOldAliasKLine, &writeableAliasKLine);
			CopyLastKLineEntity(&writeableAliasKLine, pKline);
			m_pMDB->m_KLineFactory->update(pOldAliasKLine, &writeableAliasKLine, pTransaction);
		}
	}

	CSecondsType UpdateTime = pKline->UpdateTime;

	int currentSerialNo = pKline->SerialNo.getValue();
	if (pKline->PeriodID == "1m")
	{
		if ((currentSerialNo + 1) % 5 == 0)
		{
			getLastKLineByBeginTime2(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "5m");
			pKline->UpdateTime = UpdateTime;
			CKLine *pOldKLine5m = m_pMDB->m_KLineFactory->findByInstrumentPeriodBeginTime(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->BeginTime);
			if (pOldKLine5m == NULL)
			{
				m_pMDB->m_KLineFactory->add(pKline, pTransaction);
			}
			else
			{
				CWriteableKLine writeableKLine;
				m_pMDB->m_KLineFactory->retrieve(pOldKLine5m, &writeableKLine);
				CopyLastKLineEntity(&writeableKLine, pKline);
				m_pMDB->m_KLineFactory->update(pOldKLine5m, &writeableKLine, pTransaction);
			}

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
		if ((currentSerialNo + 1) % 15 == 0)
		{
			getLastKLineByBeginTime2(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "15m");
			pKline->UpdateTime = UpdateTime;

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
		if ((currentSerialNo + 1) % 30 == 0)
		{
			getLastKLineByBeginTime2(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "30m");
			pKline->UpdateTime = UpdateTime;

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
	}
	else if (pKline->PeriodID == "1h")
	{
		if ((currentSerialNo + 1) % 4 == 0)
		{
			getLastKLineByBeginTime2(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "4h");
			pKline->UpdateTime = UpdateTime;
			CKLine *pOldKLine4h = m_pMDB->m_KLineFactory->findByInstrumentPeriodBeginTime(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->BeginTime);
			if (pOldKLine4h == NULL)
			{
				m_pMDB->m_KLineFactory->add(pKline, pTransaction);
			}
			else
			{
				CWriteableKLine writeableKLine;
				m_pMDB->m_KLineFactory->retrieve(pOldKLine4h, &writeableKLine);
				CopyLastKLineEntity(&writeableKLine, pKline);
				m_pMDB->m_KLineFactory->update(pOldKLine4h, &writeableKLine, pTransaction);
			}

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
		if ((currentSerialNo + 1) % 12 == 0)
		{
			getLastKLineByBeginTime2(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "12h");
			pKline->UpdateTime = UpdateTime;

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
	}
	else if (pKline->PeriodID == "1o")
	{
		if ((currentSerialNo + 1) % 12 == 0)
		{
			getLastKLineByBeginTime2(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "12o");
			pKline->PeriodID = "1y";
			pKline->UpdateTime = UpdateTime;

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
	}
}

void CKLineTrigger::SetLastKLine(CTrade* pTrade, CIndexIDType Period, CTransaction *pTransaction)
{
	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriod(pTrade->ExchangeID, pTrade->InstrumentID, Period);
	m_pMDB->m_LastKLineFactory->endFindByHisPeriod();

	CWriteableLastKLine theLastKLine;
	if (pLastKLine != NULL)
	{
		m_pMDB->m_LastKLineFactory->retrieve(pLastKLine, &theLastKLine);
		UpdateLastKLine(&theLastKLine, pTrade);
		m_pMDB->m_LastKLineFactory->update(pLastKLine, &theLastKLine);
	}
	else
	{
		theLastKLine.init();
		theLastKLine.ExchangeID = pTrade->ExchangeID;
		theLastKLine.InstrumentID = pTrade->InstrumentID;
		theLastKLine.PeriodID = Period;
		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pTrade->ExchangeID, pTrade->InstrumentID);
		CCurrentTime *pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
		m_pMDB->m_CurrentTimeFactory->endGet();
		theLastKLine.KLineNo = GetKLineNo(pInstrument, pCurrentTime, Period);

		theLastKLine.OpenPrice = pTrade->Price;
		theLastKLine.HighestPrice = pTrade->Price;
		theLastKLine.LowestPrice = pTrade->Price;
		theLastKLine.LastTradeID = pTrade->TradeID;
		theLastKLine.ClosePrice = pTrade->Price;
		theLastKLine.Volume += pTrade->Volume;
		if (g_bDoubleMarketVolume)
			theLastKLine.Volume += pTrade->Volume;
		theLastKLine.Turnover += pTrade->Turnover;
		if (g_bDoubleMarketVolume)
			theLastKLine.Turnover += pTrade->Turnover;
		theLastKLine.UpdateTime = pTrade->TradeTime;
		theLastKLine.BeginTime = pTrade->TradeTime;

		m_pMDB->m_LastKLineFactory->add(&theLastKLine);
	}
}

void CKLineTrigger::SetLastKLineByBeginTime(CTrade* pTrade, CIndexIDType Period, CTransaction *pTransaction)
{
	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriodBeginTime(pTrade->ExchangeID, pTrade->InstrumentID, Period);
	m_pMDB->m_LastKLineFactory->endFindByHisPeriodBeginTime();
	CWriteableLastKLine theLastKLine;
	if (pLastKLine != NULL)
	{
		//如果tradeTime和最新K线的时间间隔超过一条K线的时间间隔，需要生成一条新的K线
		if (Period == "1m" && pTrade->TradeTime - pLastKLine->BeginTime > 60)// 1m: > 60s
		{
			//闭合当前K线
			CWriteableKLine theKline;
			CopyKLineEntity(&theKline, pLastKLine);
			//InsertKLineByBeginTime(&theKline, pTransaction);
			InsertKLineByBeginTime2(&theKline, pTransaction);

			//生成一条新K线
			CreateLastKLine(pLastKLine, pTrade, Period, pTransaction);
		}
		else if (Period == "1h" && pTrade->TradeTime - pLastKLine->BeginTime > 3600)// 1h: > 1h
		{
			//闭合当前K线
			CWriteableKLine theKline;
			CopyKLineEntity(&theKline, pLastKLine);
			//InsertKLineByBeginTime(&theKline, pTransaction);
			InsertKLineByBeginTime2(&theKline, pTransaction);

			//生成一条新K线
			CreateLastKLine(pLastKLine, pTrade, Period, pTransaction);
		}
		else if (Period == "1d" && pTrade->TradeTime - pLastKLine->BeginTime > 86400)// 1d: > 1d
		{
			//闭合当前K线
			CWriteableKLine theKline;
			CopyKLineEntity(&theKline, pLastKLine);
			//InsertKLineByBeginTime(&theKline, pTransaction);
			InsertKLineByBeginTime2(&theKline, pTransaction);

			//生成一条新K线
			CreateLastKLine(pLastKLine, pTrade, Period, pTransaction);
		}
		else if (Period == "1w" && pTrade->TradeTime - pLastKLine->BeginTime > 604800)// 1w: > 1w
		{
			//闭合当前K线
			CWriteableKLine theKline;
			CopyKLineEntity(&theKline, pLastKLine);
			//InsertKLineByBeginTime(&theKline, pTransaction);
			InsertKLineByBeginTime2(&theKline, pTransaction);

			//生成一条新K线
			CreateLastKLine(pLastKLine, pTrade, Period, pTransaction);
		}
		else
		{
			//CWriteableLastKLine theLastKLine;
			m_pMDB->m_LastKLineFactory->retrieve(pLastKLine, &theLastKLine);
			UpdateLastKLine(&theLastKLine, pTrade);
			m_pMDB->m_LastKLineFactory->update(pLastKLine, &theLastKLine, pTransaction);
		}
	}
	else
	{
		theLastKLine.init();
		theLastKLine.ExchangeID = pTrade->ExchangeID;
		theLastKLine.InstrumentID = pTrade->InstrumentID;
		theLastKLine.PeriodID = Period;
		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pTrade->ExchangeID, pTrade->InstrumentID);
		CCurrentTime *pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
		m_pMDB->m_CurrentTimeFactory->endGet();

		theLastKLine.UpdateTime = pTrade->TradeTime;
		theLastKLine.BeginTime = FormatBeginTime(pTrade->TradeTime, Period);

		theLastKLine.SerialNo = GetKLineNo2(pInstrument, pCurrentTime, Period);
		theLastKLine.KLineNo = GetKLineNo3(theLastKLine.BeginTime, Period);

		theLastKLine.OpenPrice = pTrade->Price;
		theLastKLine.HighestPrice = pTrade->Price;
		theLastKLine.LowestPrice = pTrade->Price;
		theLastKLine.LastTradeID = pTrade->TradeID;
		theLastKLine.ClosePrice = pTrade->Price;
		theLastKLine.Volume += pTrade->Volume;
		if (g_bDoubleMarketVolume)
			theLastKLine.Volume += pTrade->Volume;
		theLastKLine.Turnover += pTrade->Turnover;
		if (g_bDoubleMarketVolume)
			theLastKLine.Turnover += pTrade->Turnover;

		m_pMDB->m_LastKLineFactory->add(&theLastKLine, pTransaction);
	}
}

void CKLineTrigger::CreateLastKLine(CLastKLine *pLastKLine, CTrade* pTrade, CIndexIDType Period, CTransaction *pTransaction)
{
	CWriteableLastKLine newLastKLine;
	newLastKLine.init();
	newLastKLine.ExchangeID = pTrade->ExchangeID;
	newLastKLine.InstrumentID = pTrade->InstrumentID;
	newLastKLine.PeriodID = Period;
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pTrade->ExchangeID, pTrade->InstrumentID);
	CCurrentTime *pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
	m_pMDB->m_CurrentTimeFactory->endGet();

	newLastKLine.UpdateTime = pTrade->TradeTime;
	newLastKLine.BeginTime = FormatBeginTime(pTrade->TradeTime, Period);

	newLastKLine.SerialNo = GetKLineNo2(pInstrument, pCurrentTime, Period);
	newLastKLine.KLineNo = GetKLineNo3(newLastKLine.BeginTime, Period);

	newLastKLine.OpenPrice = pLastKLine->ClosePrice;
	newLastKLine.HighestPrice = pTrade->Price;
	newLastKLine.LowestPrice = pTrade->Price;
	newLastKLine.LastTradeID = pTrade->TradeID;
	newLastKLine.ClosePrice = pTrade->Price;
	newLastKLine.Volume += pTrade->Volume;
	if (g_bDoubleMarketVolume)
		newLastKLine.Volume += pTrade->Volume;
	newLastKLine.Turnover += pTrade->Turnover;
	if (g_bDoubleMarketVolume)
		newLastKLine.Turnover += pTrade->Turnover;

	CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pTrade->ExchangeID, pTrade->InstrumentID, Period, newLastKLine.KLineNo);
	if (pOldLastKLine == NULL)
	{
		m_pMDB->m_LastKLineFactory->add(&newLastKLine, pTransaction);
	}
	else
	{
		CWriteableLastKLine writeableLastKLine;
		m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
		CopyLastKLineEntity(&writeableLastKLine, &newLastKLine);
		m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
	}
}

void CKLineTrigger::UpdateLastKLine(CWriteableLastKLine* pKLine, CTrade* pTrade)
{
	//这里采用两种选择，一种为K线连续没有跳空，一种是需要跳空来
	if (g_bKLineGap && pKLine->Volume == 0.0)
	{
		//pKLine->OpenPrice = pTrade->Price;
		pKLine->HighestPrice = pTrade->Price;
		pKLine->LowestPrice = pTrade->Price;
	}
	else
	{
		if (pTrade->Price > pKLine->HighestPrice)
			pKLine->HighestPrice = pTrade->Price;
		if (pTrade->Price < pKLine->LowestPrice)
			pKLine->LowestPrice = pTrade->Price;
	}

	pKLine->LastTradeID = pTrade->TradeID;
	pKLine->ClosePrice = pTrade->Price;
	pKLine->Volume += pTrade->Volume;
	if(g_bDoubleMarketVolume)
		pKLine->Volume += pTrade->Volume;
	pKLine->Turnover += pTrade->Turnover;
	if (g_bDoubleMarketVolume)
		pKLine->Turnover += pTrade->Turnover;
	pKLine->UpdateTime = pTrade->TradeTime;
	if (pKLine->BeginTime == 0)
		pKLine->BeginTime = FormatBeginTime(pTrade->TradeTime, pKLine->PeriodID);
}

bool CKLineTrigger::AddMarkKLineTimeUp2(CCurrentTime *pCurrentTime, CIndexIDType Period, CTransaction *pTransaction)
{
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->getFirst();
	while (pInstrument != NULL)
	{
		if (pInstrument->ProductType == PT_BitCoinExchange)
		{
			pInstrument = m_pMDB->m_InstrumentFactory->getNext();
			continue;
		}

		CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriodBeginTime(pInstrument->ExchangeID, getMarkInstrumentID(pInstrument->InstrumentID), Period);
		m_pMDB->m_LastKLineFactory->endFindByHisPeriodBeginTime();
		CWriteableKLine theKline;		//准备写入历史的K线
		CWriteableLastKLine theLastKline;	//准备新加入LastKLine的K线

		if (pLastKLine != NULL)
		{
			int nSerialNo = GetKLineNo2(pInstrument, pCurrentTime, Period);
			int nKLineNo = GetKLineNo3(pCurrentTime->CurrTime, Period);
			if (nSerialNo == pLastKLine->SerialNo || nSerialNo < 0)
			{
				pInstrument = m_pMDB->m_InstrumentFactory->getNext();
				REPORT_EVENT(LOG_CRITICAL, "AddMarkKLineTimeUp", "theNewLastKline[%d]==theLastKline[%d]", nSerialNo, pLastKLine->SerialNo.getValue());
				continue;
			}
			CopyKLineEntity(&theKline, pLastKLine);

			//之前的最新K线要插入到历史库中
			theKline.UpdateTime = pCurrentTime->CurrTime;
			InsertMarkKLineByBeginTime(pInstrument, &theKline, pTransaction);

			//不完整的最新基础K线
			theLastKline.ExchangeID = pLastKLine->ExchangeID;
			theLastKline.InstrumentID = pLastKLine->InstrumentID;
			theLastKline.LastTradeID = pLastKLine->LastTradeID;
			theLastKline.PeriodID = pLastKLine->PeriodID;
			theLastKline.BeginTime = pCurrentTime->CurrTime % 60 == 0 ? pCurrentTime->CurrTime : FormatBeginTime(pCurrentTime->CurrTime, pLastKLine->PeriodID);
			theLastKline.UpdateTime = pCurrentTime->CurrTime;
			theLastKline.OpenPrice = pLastKLine->ClosePrice;
			theLastKline.ClosePrice = pLastKLine->ClosePrice;
			theLastKline.HighestPrice = pLastKLine->ClosePrice;
			theLastKline.LowestPrice = pLastKLine->ClosePrice;
			theLastKline.Volume = 0;
			theLastKline.Turnover = 0.0;
			theLastKline.SerialNo = nSerialNo;
			theLastKline.KLineNo = nKLineNo;

			//更新对应LineNo的K线数据
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pInstrument->ExchangeID, getMarkInstrumentID(pInstrument->InstrumentID), Period, nKLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}

		pInstrument = m_pMDB->m_InstrumentFactory->getNext();
	}
	m_pMDB->m_InstrumentFactory->endGet();
	return true;
}

void CKLineTrigger::SetLastMarkKLineByBeginTime(CMarketData* pMarketData, CIndexIDType Period, CTransaction *pTransaction)
{
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pMarketData->ExchangeID, pMarketData->InstrumentID);
	if (pInstrument == NULL || pInstrument->ProductType == PT_BitCoinExchange)
	{
		return;
	}

	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriodBeginTime(pMarketData->ExchangeID, getMarkInstrumentID(pMarketData->InstrumentID), Period);
	m_pMDB->m_LastKLineFactory->endFindByHisPeriodBeginTime();
	CWriteableLastKLine theLastKLine;
	if (pLastKLine != NULL)
	{
		//如果tradeTime和最新K线的时间间隔超过一条K线的时间间隔，需要生成一条新的K线
		if (pInstrument != NULL && Period == "1m" && pMarketData->UpdateTime - pLastKLine->BeginTime > 60)// 1m: > 60s
		{
			//闭合当前K线
			CWriteableKLine theKline;
			CopyKLineEntity(&theKline, pLastKLine);
			InsertMarkKLineByBeginTime(pInstrument, &theKline, pTransaction);

			//生成一条新K线
			CreateMarkLastKLine(pLastKLine, pMarketData, Period, pTransaction);
		}
		else if (pInstrument != NULL && Period == "1h" && pMarketData->UpdateTime - pLastKLine->BeginTime > 3600)// 1h: > 1h
		{
			//闭合当前K线
			CWriteableKLine theKline;
			CopyKLineEntity(&theKline, pLastKLine);
			InsertMarkKLineByBeginTime(pInstrument, &theKline, pTransaction);

			//生成一条新K线
			CreateMarkLastKLine(pLastKLine, pMarketData, Period, pTransaction);
		}
		else if (pInstrument != NULL && Period == "1d" && pMarketData->UpdateTime - pLastKLine->BeginTime > 86400)// 1d: > 1d
		{
			//闭合当前K线
			CWriteableKLine theKline;
			CopyKLineEntity(&theKline, pLastKLine);
			InsertMarkKLineByBeginTime(pInstrument, &theKline, pTransaction);

			//生成一条新K线
			CreateMarkLastKLine(pLastKLine, pMarketData, Period, pTransaction);
		}
		else if (pInstrument != NULL && Period == "1w" && pMarketData->UpdateTime - pLastKLine->BeginTime > 604800)// 1w: > 1w
		{
			//闭合当前K线
			CWriteableKLine theKline;
			CopyKLineEntity(&theKline, pLastKLine);
			InsertMarkKLineByBeginTime(pInstrument, &theKline, pTransaction);

			//生成一条新K线
			CreateMarkLastKLine(pLastKLine, pMarketData, Period, pTransaction);
		}
		else
		{
			m_pMDB->m_LastKLineFactory->retrieve(pLastKLine, &theLastKLine);
			UpdateMarkLastKLine(&theLastKLine, pMarketData);
			m_pMDB->m_LastKLineFactory->update(pLastKLine, &theLastKLine, pTransaction);
		}
	}
	else
	{
		theLastKLine.init();
		theLastKLine.ExchangeID = pMarketData->ExchangeID;
		theLastKLine.InstrumentID = getMarkInstrumentID(pMarketData->InstrumentID);
		theLastKLine.PeriodID = Period;
		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pMarketData->ExchangeID, pMarketData->InstrumentID);
		CCurrentTime *pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
		m_pMDB->m_CurrentTimeFactory->endGet();

		theLastKLine.UpdateTime = pMarketData->UpdateTime;
		theLastKLine.BeginTime = FormatBeginTime(pMarketData->UpdateTime, Period);

		theLastKLine.SerialNo = GetKLineNo2(pInstrument, pCurrentTime, Period);
		theLastKLine.KLineNo = GetKLineNo3(theLastKLine.BeginTime, Period);

		theLastKLine.OpenPrice = pMarketData->MarkedPrice;
		theLastKLine.HighestPrice = pMarketData->MarkedPrice;
		theLastKLine.LowestPrice = pMarketData->MarkedPrice;
		theLastKLine.LastTradeID = "";
		theLastKLine.ClosePrice = pMarketData->MarkedPrice;
		theLastKLine.Volume = 0;
		theLastKLine.Turnover = 0;

		m_pMDB->m_LastKLineFactory->add(&theLastKLine, pTransaction);
	}
}

CSecondsType CKLineTrigger::FormatBeginTime(CSecondsType BeginTime, CIndexIDType Period)
{
	CSecondsType formatTime = 0;
	if (Period == "1m")
	{
		formatTime = BeginTime - BeginTime % 60;
	}
	else if (Period == "1h")
	{
		formatTime = BeginTime - BeginTime % (60 * 60);
	}
	else if (Period == "1d")
	{
		CWriteableCurrentTime theCurrentTime = convertTimestampToCurrentTime(BeginTime);
		formatTime = getLongDayTime((char *)theCurrentTime.ZoneDate.getValue(), "00:00:00");
	}
	else if (Period == "1w")
	{
		CWriteableCurrentTime theCurrentTime = convertTimestampToCurrentTime(BeginTime);
		char weekDays = theCurrentTime.ZoneDays.getValue()[0];
		formatTime = getLongDayTime((char *)theCurrentTime.ZoneDate.getValue(), "00:00:00") - (60 * 60 * 24 * atoi(&weekDays));
	}
	else if (Period == "1o")
	{
		CWriteableCurrentTime theCurrentTime = convertTimestampToCurrentTime(BeginTime);
		string ZoneDate = theCurrentTime.ZoneDate.getValue();
		ZoneDate = ZoneDate.substr(0, 6) + "01";//最后两位设置为01
		formatTime = getLongDayTime((char *)ZoneDate.c_str(), "00:00:00");
	}
	return formatTime;
}

void CKLineTrigger::InsertMarkKLineByBeginTime(CInstrument *pInstrument, CWriteableKLine *pKline, CTransaction *pTransaction)
{
	CKLine *pOldKLine = m_pMDB->m_KLineFactory->findByInstrumentPeriodBeginTime(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->BeginTime);
	if (pOldKLine == NULL)
	{
		m_pMDB->m_KLineFactory->add(pKline, pTransaction);
	}
	else
	{
		CWriteableKLine writeableKLine;
		m_pMDB->m_KLineFactory->retrieve(pOldKLine, &writeableKLine);
		CopyLastKLineEntity(&writeableKLine, pKline);
		m_pMDB->m_KLineFactory->update(pOldKLine, &writeableKLine, pTransaction);
	}

	//增加基础合约行情的K线，以便连续
	CInstrument *pAliasInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID)->getAliasInstrument();
	if (pAliasInstrument != NULL)
	{
		CWriteableKLine theAliasKline;
		CopyKLineEntity(&theAliasKline, pKline);
		theAliasKline.InstrumentID = getMarkInstrumentID(pAliasInstrument->InstrumentID);
		theAliasKline.ExchangeID = pAliasInstrument->ExchangeID;
		CKLine *pOldAliasKLine = m_pMDB->m_KLineFactory->findByInstrumentPeriodBeginTime(theAliasKline.ExchangeID, theAliasKline.InstrumentID, theAliasKline.PeriodID, theAliasKline.BeginTime);
		if (pOldAliasKLine == NULL)
		{
			m_pMDB->m_KLineFactory->add(&theAliasKline, pTransaction);
		}
		else
		{
			CWriteableKLine writeableAliasKLine;
			m_pMDB->m_KLineFactory->retrieve(pOldAliasKLine, &writeableAliasKLine);
			CopyLastKLineEntity(&writeableAliasKLine, &theAliasKline);
			m_pMDB->m_KLineFactory->update(pOldAliasKLine, &writeableAliasKLine, pTransaction);
		}
	}

	CSecondsType UpdateTime = pKline->UpdateTime;

	int currentSerialNo = pKline->SerialNo.getValue();
	if (pKline->PeriodID == "1m")
	{
		if ((currentSerialNo + 1) % 5 == 0)
		{
			getLastMarkKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "5m");
			pKline->UpdateTime = UpdateTime;
			CKLine *pOldKLine5m = m_pMDB->m_KLineFactory->findByInstrumentPeriodBeginTime(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->BeginTime);
			if (pOldKLine5m == NULL)
			{
				m_pMDB->m_KLineFactory->add(pKline, pTransaction);
			}
			else
			{
				CWriteableKLine writeableKLine;
				m_pMDB->m_KLineFactory->retrieve(pOldKLine5m, &writeableKLine);
				CopyLastKLineEntity(&writeableKLine, pKline);
				m_pMDB->m_KLineFactory->update(pOldKLine5m, &writeableKLine, pTransaction);
			}

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
		if ((currentSerialNo + 1) % 15 == 0)
		{
			getLastMarkKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "15m");
			pKline->UpdateTime = UpdateTime;

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
		if ((currentSerialNo + 1) % 30 == 0)
		{
			getLastMarkKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "30m");
			pKline->UpdateTime = UpdateTime;

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
	}
	else if (pKline->PeriodID == "1h")
	{
		if ((currentSerialNo + 1) % 4 == 0)
		{
			getLastMarkKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "4h");
			pKline->UpdateTime = UpdateTime;
			CKLine *pOldKLine4h = m_pMDB->m_KLineFactory->findByInstrumentPeriodBeginTime(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->BeginTime);
			if (pOldKLine4h == NULL)
			{
				m_pMDB->m_KLineFactory->add(pKline, pTransaction);
			}
			else
			{
				CWriteableKLine writeableKLine;
				m_pMDB->m_KLineFactory->retrieve(pOldKLine4h, &writeableKLine);
				CopyLastKLineEntity(&writeableKLine, pKline);
				m_pMDB->m_KLineFactory->update(pOldKLine4h, &writeableKLine, pTransaction);
			}

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
		if ((currentSerialNo + 1) % 12 == 0)
		{
			getLastMarkKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "12h");
			pKline->UpdateTime = UpdateTime;

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
	}
	else if (pKline->PeriodID == "1o")
	{
		if ((currentSerialNo + 1) % 12 == 0)
		{
			getLastMarkKLineByBeginTime(m_pMDB, pKline, pKline->ExchangeID, pKline->InstrumentID, "12o");
			pKline->PeriodID = "1y";
			pKline->UpdateTime = UpdateTime;

			//增加非基础的最新K线
			CWriteableLastKLine theLastKline;
			CopyLastKLineEntity(&theLastKline, pKline);
			CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pKline->ExchangeID, pKline->InstrumentID, pKline->PeriodID, pKline->KLineNo);
			if (pOldLastKLine == NULL)
			{
				m_pMDB->m_LastKLineFactory->add(&theLastKline, pTransaction);
			}
			else
			{
				CWriteableLastKLine writeableLastKLine;
				m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
				CopyLastKLineEntity(&writeableLastKLine, &theLastKline);
				m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
			}
		}
	}
}

void CKLineTrigger::CreateMarkLastKLine(CLastKLine *pLastKLine, CMarketData* pMarketData, CIndexIDType Period, CTransaction *pTransaction)
{
	CWriteableLastKLine newLastKLine;
	newLastKLine.init();
	newLastKLine.ExchangeID = pMarketData->ExchangeID;
	newLastKLine.InstrumentID = getMarkInstrumentID(pMarketData->InstrumentID);
	newLastKLine.PeriodID = Period;
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pMarketData->ExchangeID, pMarketData->InstrumentID);
	CCurrentTime *pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
	m_pMDB->m_CurrentTimeFactory->endGet();

	newLastKLine.UpdateTime = pMarketData->UpdateTime;
	newLastKLine.BeginTime = FormatBeginTime(pMarketData->UpdateTime, Period);

	newLastKLine.SerialNo = GetKLineNo2(pInstrument, pCurrentTime, Period);
	newLastKLine.KLineNo = GetKLineNo3(newLastKLine.BeginTime, Period);

	newLastKLine.OpenPrice = pLastKLine->ClosePrice;
	newLastKLine.HighestPrice = pMarketData->MarkedPrice;
	newLastKLine.LowestPrice = pMarketData->MarkedPrice;
	newLastKLine.LastTradeID = "";
	newLastKLine.ClosePrice = pMarketData->MarkedPrice;
	newLastKLine.Volume = 0;
	newLastKLine.Turnover = 0;

	CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentPeriod(pMarketData->ExchangeID, getMarkInstrumentID(pMarketData->InstrumentID), Period, newLastKLine.KLineNo);
	if (pOldLastKLine == NULL)
	{
		m_pMDB->m_LastKLineFactory->add(&newLastKLine, pTransaction);
	}
	else
	{
		CWriteableLastKLine writeableLastKLine;
		m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &writeableLastKLine);
		CopyLastKLineEntity(&writeableLastKLine, &newLastKLine);
		m_pMDB->m_LastKLineFactory->update(pOldLastKLine, &writeableLastKLine, pTransaction);
	}
}

void CKLineTrigger::UpdateMarkLastKLine(CWriteableLastKLine* pKLine, CMarketData* pMarketData)
{
	//这里采用两种选择，一种为K线连续没有跳空，一种是需要跳空来
	if (g_bKLineGap && pKLine->Volume == 0.0)
	{
		//pKLine->OpenPrice = pMarketData->MarkedPrice;
		pKLine->HighestPrice = pMarketData->MarkedPrice;
		pKLine->LowestPrice = pMarketData->MarkedPrice;
	}
	else
	{
		if (pMarketData->MarkedPrice > pKLine->HighestPrice)
			pKLine->HighestPrice = pMarketData->MarkedPrice;
		if (pMarketData->MarkedPrice < pKLine->LowestPrice)
			pKLine->LowestPrice = pMarketData->MarkedPrice;
	}

	pKLine->LastTradeID = "";
	pKLine->ClosePrice = pMarketData->MarkedPrice;
	pKLine->Volume = 0;
	pKLine->Turnover = 0;
	pKLine->UpdateTime = pMarketData->UpdateTime;
	if (pKLine->BeginTime == 0)
		pKLine->BeginTime = FormatBeginTime(pMarketData->UpdateTime, pKLine->PeriodID);
}

CInstrumentIDType CKLineTrigger::getMarkInstrumentID(const CInstrumentIDType &InstrumentID)
{
	char MarkInstrumentID[300];
	strcpy(MarkInstrumentID, "m");
	strcat(MarkInstrumentID, InstrumentID);
	return MarkInstrumentID;
}