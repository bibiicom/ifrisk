#include "MyAlterService.h"
#include "monitorIndex.h"
#include "CopyField.h"
#include "tradingTools.h"
#include "CSVToField.h"
#include "printinfo.h"
#include "cryptUtility.h"

//static CEventMonitor eventMonitor;

// bool CMyAlterService::SyncAccountImpl(int nTid,CWriteableAccount *pAccount,CTransaction *pTransaction)
// {
// 	bool success;
// 	
// 	success=CAlterService::SyncAccountImpl(nTid,pAccount,pTransaction);
// 	
// 	if (!success)
// 	{
// 		return false;
// 	}
// 	
// 	CAccount *pAccountNew = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pAccount->AccountID,pAccount->Currency);

//	CBaseReserveAccount *pBaseReserveAccount = m_pMDB->m_BaseReserveAccountFactory->findByAccountIDAndCurrency(pAccount->AccountID,pAccount->Currency);
// 	if(pBaseReserveAccount!=NULL)
// 	{
// 		pAccountNew->linkBaseReserveAccount(pBaseReserveAccount);
// 	}
// 
// 	return true;
// }

// bool CMyAlterService::SyncInsBaseReserveAccountImpl(CWriteableBaseReserveAccount *pBaseReserveAccount,CTransaction *pTransaction)
// {
// 	bool success;
// 	
// 	success=CAlterService::SyncBaseReserveAccountImpl(TID_SendInsert,pBaseReserveAccount,pTransaction);
// 	
// 	if (!success)
// 	{
// 		return false;
// 	}
// 	
// 	CAccount *pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pBaseReserveAccount->AccountID,pBaseReserveAccount->Currency);
// 	
// 	if(pAccount!=NULL)
// 	{
// 		CBaseReserveAccount *pBaseReserveAccountNew = m_pMDB->m_BaseReserveAccountFactory->findByAccountIDAndCurrency(pBaseReserveAccount->AccountID,pBaseReserveAccount->Currency);
//		pAccount->linkBaseReserveAccount(pBaseReserveAccountNew);
// 	}
// 	
// 	return true;
// }
// 
// bool CMyAlterService::SyncInsHedgeDetailImpl(CWriteableHedgeDetail *pHedgeDetail,CTransaction *pTransaction)
// {
// 	char buffer[40960];
// 	pHedgeDetail->dumpString(buffer);
// 	eventMonitor.report("InsHedgeDetail","","",buffer);
// 
// 	CHedgeVolume *pHedgeVolume;
// 	CWriteableHedgeVolume theHedgeVolume;
// 
// 	pHedgeVolume=m_pMDB->m_HedgeVolumeFactory->findByInstrumentTradeUnit(
// 		pHedgeDetail->InstrumentID,pHedgeDetail->MemberID,pHedgeDetail->TradeUnitID);
// 	///-------
// 	//修正了错误，原来套保额度上场时使用的字段错了
// 	///-------
// 	if (pHedgeVolume==NULL)
// 	{
// 		///原来没有保值额度，那就加上它
// 		theHedgeVolume.TradingDay=pHedgeDetail->TradingDay;
// 		theHedgeVolume.SettlementGroupID=pHedgeDetail->SettlementGroupID;
// 		theHedgeVolume.SettlementNo=pHedgeDetail->SettlementNo;
// 		theHedgeVolume.MemberID=pHedgeDetail->MemberID;
// 		theHedgeVolume.TradeUnitID=pHedgeDetail->TradeUnitID;
// 		theHedgeVolume.InstrumentID=pHedgeDetail->InstrumentID;
// 		if (pHedgeDetail->PosiDirection==PD_Long)
// 		{
// 			theHedgeVolume.LongVolumeOriginal=pHedgeDetail->TradingVolume;
// 			theHedgeVolume.LongVolume=pHedgeDetail->TradingVolume;
// 			theHedgeVolume.LongLimit=pHedgeDetail->TradingVolume;
// 			theHedgeVolume.ShortVolumeOriginal=0.0;
// 			theHedgeVolume.ShortVolume=0.0;
// 			theHedgeVolume.ShortLimit=0.0;
// 		}
// 		else
// 		{
// 			theHedgeVolume.LongVolumeOriginal=0.0;
// 			theHedgeVolume.LongVolume=0.0;
// 			theHedgeVolume.LongLimit=0.0;
// 			theHedgeVolume.ShortVolumeOriginal=pHedgeDetail->TradingVolume;
// 			theHedgeVolume.ShortVolume=pHedgeDetail->TradingVolume;
// 			theHedgeVolume.ShortLimit=pHedgeDetail->TradingVolume;
// 		}
// 	}
// 	else
// 	{
// 		m_pMDB->m_HedgeVolumeFactory->retrieve(pHedgeVolume,&theHedgeVolume);
// 		if (pHedgeDetail->PosiDirection==PD_Long)
// 		{
// 			theHedgeVolume.LongVolumeOriginal+=pHedgeDetail->TradingVolume;
// 			theHedgeVolume.LongVolume+=pHedgeDetail->TradingVolume;
// 			theHedgeVolume.LongLimit+=pHedgeDetail->TradingVolume;
// 		}
// 		else
// 		{
// 			theHedgeVolume.ShortVolumeOriginal+=pHedgeDetail->TradingVolume;
// 			theHedgeVolume.ShortVolume+=pHedgeDetail->TradingVolume;
// 			theHedgeVolume.ShortLimit+=pHedgeDetail->TradingVolume;
// 		}
// 	}
// 
// 	m_pMDB->m_HedgeVolumeFactory->addOrUpdate(pHedgeVolume,&theHedgeVolume,pTransaction);
// 
// 	return true;
// }

//bool CMyAlterService::ReqDBCommand(CDBCommandField *pDBCommandField, char* pDbmtStream, int &nFieldID, TSessionRef* pSession, CTransaction *pTransaction)
//{
//	CUserSession * pUserSession = m_pMDB->m_UserSessionFactory->findBySessionNo(pSession->frontID, pSession->sessionID);
//	if (pUserSession == NULL)
//		return m_pErrorEngine->reportError(ERROR_INVALID_SESSION);
//
//	if (checkLocalID(m_pMDB, m_pErrorEngine, pUserSession->UserID, pDBCommandField->LocalID, NULL) == NULL)
//		return false;
//
//	CCommandInfoMap::iterator itor = m_mapCommandInfo.find(pDBCommandField->TableName);
//	if (itor == m_mapCommandInfo.end())
//		return false;
//
//	nFieldID = itor->second->pFieldDescribe->m_FieldID;
//
//	CFieldAnalysis record;
//	if (!record.Analysis(pDBCommandField->FieldName, pDBCommandField->FieldContent))
//	{
//		REPORT_EVENT(LOG_CRITICAL, "Dbmt_API", "AnalysisRecord error, CommandLocalID=[%s][%s][%s]", pDBCommandField->LocalID.getValue(), pDBCommandField->FieldName.getValue(), pDBCommandField->FieldContent.getValue());
//		return m_pErrorEngine->reportError(ERROR_RecordAnalysisError);
//	}
//
//	if (!record.TransToStruct(itor->second->pFieldDescribe, m_pDbmtStruct))
//	{
//		return m_pErrorEngine->reportError(ERROR_RecordAnalysisError);
//	}
//
//	itor->second->pFieldDescribe->setSingleField(pDbmtStream, m_pDbmtStruct);
//
//	return true;
//}

// bool CAlterService::ReqDBCommandDetail(CDBCommandField *pDBCommandField,CTransaction *pTransaction)
// {
// 	CWriteableDBCommandDetail theDetail;
// 	theDetail.init();
// 	CopyDBCommandEntity(&theDetail,pDBCommandField);
// 
// 	pDBCommandField->CommandNo = m_pDBCommandSequence->getNextValue();
// 	
// 	theDetail.CommandNo = pDBCommandField->CommandNo;
// 	
// 	
// 	CSettlementGroup * pSettlementGroup =  m_pMDB->m_SettlementGroupFactory->getFirst();
// 	if(pSettlementGroup==NULL)
// 	{
// 		theDetail.SettlementGroupID = "SG01";
// 		theDetail.SettlementNo = getSettlementNo(m_pMDB,"SG01");
// 	}
// 	else
// 	{
// 		theDetail.SettlementGroupID = pSettlementGroup->SettlementGroupID;
// 		theDetail.SettlementNo = getSettlementNo(m_pMDB,pSettlementGroup->SettlementGroupID);
// 	}
// 
// 	theDetail.TradingDay = getTradingDay(m_pMDB);	
// 	theDetail.CalendarDate = getCalendarDay(m_pMDB);
// 	theDetail.InsertTime = getCurrentTime(m_pMDB);
// 	theDetail.Status = PTS_Success;
// 	m_pMDB->m_DBCommandDetailFactory->addByPK(&theDetail,pTransaction);
// 
// 	return true;
// }

bool CMyAlterService::SyncFeeImpl(int nTid, CFeeField *pFee, CTransaction *pTransaction)
{
	//bool success;

	//success = CAlterService::SyncFeeImpl(nTid, pFee, pTransaction);

	//if (!success)
	//	return false;

	//if (nTid != TID_SendDelete)
	//{
	//	CFee *pThisFee = m_pMDB->m_FeeFactory->findByMemberIDAndInstrument(pFee->MemberID, pFee->ExchangeID, pFee->InstrumentID, pFee->MatchRole);
	//	if (pThisFee == NULL)
	//		return false;
	//}

	//return true;

	return CAlterService::SyncFeeImpl(nTid, pFee, pTransaction);
}

bool CMyAlterService::SyncInstrumentImpl(int nTid, CInstrumentField *pInstrument, CTransaction *pTransaction)
{
	switch (nTid)
	{
	case TID_SendDelete:
	{
		CInstrument *pInstrumentToRemove = m_pMDB->m_InstrumentFactory->findByPK(pInstrument);
		if (pInstrumentToRemove != NULL)
		{
			if (pInstrumentToRemove->SortNumber != -1)
				return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS);
		}

		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID);
		if (pMarketData != NULL)
		{
			if (pMarketData->InstrumentStatus != IS_NoTrading)
				return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS, "MarketDataExsit");
		}

		COrder *pOrder = m_pMDB->m_OrderFactory->getFirst();
		while (pOrder != NULL)
		{
			if (pOrder->ExchangeID == pInstrument->ExchangeID && pOrder->InstrumentID == pInstrument->InstrumentID)
				return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "Order");
			pOrder = m_pMDB->m_OrderFactory->getNext();
		}

		CTriggerOrder *pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getFirst();
		while (pTriggerOrder != NULL)
		{
			if (pTriggerOrder->ExchangeID == pInstrument->ExchangeID && pTriggerOrder->InstrumentID == pInstrument->InstrumentID)
				return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "TriggerOrder");
			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getNext();
		}

		CPosition *pPosition = m_pMDB->m_PositionFactory->getFirst();
		while (pPosition != NULL)
		{
			if (pPosition->ExchangeID == pInstrument->ExchangeID && pPosition->InstrumentID == pInstrument->InstrumentID)
				return m_pErrorEngine->reportError(ERROR_POSITION_EXIST, "PositionExist");
			pPosition = m_pMDB->m_PositionFactory->getNext();
		}

		//删除行情
		m_pMDB->m_MarketDataFactory->removeByPK(pInstrument->ExchangeID, pInstrument->InstrumentID, pTransaction);
		return  CAlterService::SyncInstrumentImpl(nTid, pInstrument, pTransaction);
	}
	case TID_SendInsert:
	{
		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID);
		if (pMarketData != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST, "MarketData");

		COrder *pOrder = m_pMDB->m_OrderFactory->getFirst();
		while (pOrder != NULL)
		{
			if (pOrder->ExchangeID == pInstrument->ExchangeID && pOrder->InstrumentID == pInstrument->InstrumentID)
				return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "Order");
			pOrder = m_pMDB->m_OrderFactory->getNext();
		}

		CTriggerOrder *pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getFirst();
		while (pTriggerOrder != NULL)
		{
			if (pTriggerOrder->ExchangeID == pInstrument->ExchangeID && pTriggerOrder->InstrumentID == pInstrument->InstrumentID)
				return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "TriggerOrder");
			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getNext();
		}

		CPosition *pPosition = m_pMDB->m_PositionFactory->getFirst();
		while (pPosition != NULL)
		{
			if (pPosition->ExchangeID == pInstrument->ExchangeID && pPosition->InstrumentID == pInstrument->InstrumentID)
				return m_pErrorEngine->reportError(ERROR_POSITION_EXIST, "PositionExist");
			pPosition = m_pMDB->m_PositionFactory->getNext();
		}

		formatTime(pInstrument->DayStartTime.getBuffer());
		if (pInstrument->DefaultLeverage.isNull())
			pInstrument->DefaultLeverage = 1.0;

		if (pInstrument->DefaultLeverage == 0.0)
			return m_pErrorEngine->reportError(ERROR_BAD_FIELD, "DefaultLeverage==0.0");

		pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID);
		if (pMarketData == NULL)
		{
			CWriteableMarketData theMarketData;
			theMarketData.init();
			theMarketData.ExchangeID = pInstrument->ExchangeID;
			theMarketData.InstrumentID = pInstrument->InstrumentID;
			theMarketData.ProductGroup = pInstrument->ProductGroup;
			theMarketData.OpenPrice = pInstrument->BasisPrice;
			theMarketData.HighestPrice = pInstrument->BasisPrice;
			theMarketData.LowestPrice = pInstrument->BasisPrice;
			theMarketData.PreClosePrice = pInstrument->BasisPrice;
			theMarketData.PreSettlementPrice = pInstrument->BasisPrice;
			theMarketData.LastPrice = pInstrument->BasisPrice;
			theMarketData.InstrumentStatus = IS_Continous;
			theMarketData.BusinessNo = 0.0;
			pMarketData = m_pMDB->m_MarketDataFactory->add(&theMarketData, pTransaction);
		}

		bool success = CAlterService::SyncInstrumentImpl(nTid, pInstrument, pTransaction);
		if (!success)
			return false;
		CInstrument *pThisInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID);
		if (pThisInstrument == NULL)
			return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND, "%s_%s", pInstrument->ExchangeID.getValue(), pInstrument->InstrumentID.getValue());
		pThisInstrument->linkMarketData();
		pMarketData->linkInstrument(pThisInstrument);
		return true;
	}
	case TID_SendClear:
	{
		if (!pInstrument->DefaultLeverage.isNull())
			return m_pErrorEngine->reportError(ERROR_BAD_FIELD, "DefaultLeverageCanNotClear");
	}
	case TID_SendInsUpd:
		nTid = TID_SendUpdate;
	case TID_SendUpdate:
	{
		if (!pInstrument->BaseCurrency.isNull())
			pInstrument->BaseCurrency.clear();
		if (!pInstrument->ClearCurrency.isNull())
			pInstrument->ClearCurrency.clear();
		if (!pInstrument->IsInverse.isNull())
			pInstrument->IsInverse.clear();
		if (!pInstrument->PriceCurrency.isNull())
			pInstrument->PriceCurrency.clear();
		if (!pInstrument->ProductType.isNull())
			pInstrument->ProductType.clear();
		if (!pInstrument->StrikePrice.isNull())
			pInstrument->StrikePrice.clear();
		if (!pInstrument->TradingModel.isNull())
			pInstrument->TradingModel.clear();
		if (!pInstrument->VolumeMultiple.isNull())
			pInstrument->VolumeMultiple.clear();
		if (!pInstrument->SettlementGroup.isNull())
			pInstrument->SettlementGroup.clear();
		if (!pInstrument->ProductClass.isNull())
			pInstrument->ProductClass.clear();

		if (!pInstrument->DayStartTime.isNull())
			formatTime(pInstrument->DayStartTime.getBuffer());

		if (!pInstrument->DefaultLeverage.isNull())
		{
			if (pInstrument->DefaultLeverage == 0.0)
				return m_pErrorEngine->reportError(ERROR_BAD_FIELD, "DefaultLeverage==0.0");
		}

		bool success = CAlterService::SyncInstrumentImpl(nTid, pInstrument, pTransaction);
		if (!success)
			return false;
		break;
	}
	case TID_SendInit:
	{
		//if (g_bOnlyRedo)
		//	break;
		//formatTime(pInstrument->DayStartTime.getBuffer());
		//if (pInstrument->CreateTime.isNull())
		//	pInstrument->CreateTime = getMDBTime(m_pMDB);

		CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID);
		if (pMarketData == NULL)
		{
			CWriteableMarketData theMarketData;
			theMarketData.init();
			theMarketData.ExchangeID = pInstrument->ExchangeID;
			theMarketData.InstrumentID = pInstrument->InstrumentID;
			theMarketData.ProductGroup = pInstrument->ProductGroup;
			theMarketData.OpenPrice = pInstrument->BasisPrice;
			theMarketData.HighestPrice = pInstrument->BasisPrice;
			theMarketData.LowestPrice = pInstrument->BasisPrice;
			theMarketData.PreClosePrice = pInstrument->BasisPrice;
			theMarketData.PreSettlementPrice = pInstrument->BasisPrice;
			theMarketData.LastPrice = pInstrument->BasisPrice;
			theMarketData.InstrumentStatus = IS_Continous;
			theMarketData.BusinessNo = 0.0;
			theMarketData.Remark = pInstrument->Remark;
			pMarketData = m_pMDB->m_MarketDataFactory->add(&theMarketData, pTransaction);
		}

	}
	case TID_SendQryOne:
	default:
	{
		return CAlterService::SyncInstrumentImpl(nTid, pInstrument, pTransaction);
	}
	}
	return true;
}

bool CMyAlterService::SyncMarketDataImpl(int nTid, CMarketDataField *pMarketData, CTransaction *pTransaction)
{
	if (nTid == TID_SendDelete)
	{
		CMarketData* pMarketDataToRemove = m_pMDB->m_MarketDataFactory->findByInstrumentID(pMarketData->ExchangeID, pMarketData->InstrumentID);
		if (pMarketDataToRemove != NULL)
		{
			if (pMarketDataToRemove->InstrumentStatus != IS_NoTrading)
				return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS, "InstrumentStatus");
		}

		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pMarketData->ExchangeID, pMarketData->InstrumentID);
		if (pInstrument != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST, "Instrument");

		return  CAlterService::SyncMarketDataImpl(nTid, pMarketData, pTransaction);
	}
	else
	{
		return  CAlterService::SyncMarketDataImpl(nTid, pMarketData, pTransaction);
	}
}

//bool CMyAlterService::SyncUserImpl(int nTid, CUserField *pUser, CTransaction *pTransaction)
//{
//	switch (nTid)
//	{
//	case TID_SendInit:
//	{
//		if (g_bOnlyRedo)
//			break;
//	}
//	case TID_SendInsert:
//	{
//		if (pUser->UserID.isNull())
//			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "UserID");
//		//if (pUser->FeeGrade.isNull())
//		//	pUser->FeeGrade = 9999;
//		if (pUser->FeeGrade == 9999)
//			pUser->FeeGrade.clear();
//		if (pUser->TriggerOrders.isNull())
//			pUser->TriggerOrders = 0;
//		if (pUser->OpenOrders.isNull())
//			pUser->OpenOrders = 0;
//		if (pUser->ApiKeyCount.isNull())
//			pUser->ApiKeyCount = 0;
//		if (pUser->OpenPositions.isNull())
//			pUser->OpenPositions = 0;
//		break;
//	}
//	case TID_SendUpdate:
//	case TID_SendInsUpd:
//	case TID_SendClear:
//	case TID_SendDelete:
//	{
//		CUser *pCUser = m_pMDB->m_UserFactory->findByPK(pUser);
//		if (pCUser != NULL)
//		{
//			CWriteableUser theUser;
//			m_pMDB->m_UserFactory->retrieve(pCUser, &theUser);
//			m_pMDB->m_UserFactory->update(pCUser, &theUser, pTransaction);
//		}
//		break;
//	}
//	case TID_SendQryOne:
//	default:
//	{
//		break;
//	}
//	}
//
//	//User表迁移为Member表，这里需要做两次数据
//	CMemberField theMember;
//	CopyMemberBaseEntity(&theMember, pUser);
//	theMember.MemberID = pUser->DefaultMemberID;
//	if (theMember.MemberID.isNull())
//		theMember.MemberID = pUser->UserID;
//	theMember.MemberType = PT_Normal;
//	theMember.SinkType = ST_SinkToDB;
//	//if (pUser->UserType == UT_SuperManager || !pUser->Password.isNull())
//	//{
//	//	SyncMemberImpl(nTid, &theMember, pTransaction);
//	//	return CAlterService::SyncUserImpl(nTid, pUser, pTransaction);
//	//}
//	//return SyncMemberImpl(nTid, &theMember, pTransaction);
//
//	CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(pUser->UserID, "");
//	if (pUserSession != NULL)
//		m_pMDB->m_UserSessionFactory->remove(pUserSession, NULL);
//
//	CWriteableUserSession UserSession;
//	UserSession.UserID = pUser->UserID;
//	UserSession.Password = pUser->Password;
//	UserSession.UserType = pUser->UserType;
//	UserSession.MemberID = pUser->DefaultMemberID;
//	UserSession.InsertTime = pUser->InsertTime;
//	char sToken[33];
//	MD5Hex(sToken, UserSession.UserID.getValue(), UserSession.InsertTime.getString().c_str(), UserSession.APPID);
//	UserSession.Token = sToken;
//	m_pMDB->m_UserSessionFactory->add(&UserSession, NULL);
//
//	return SyncMemberImpl(nTid, &theMember, pTransaction);
//}

bool CMyAlterService::SyncAccountImpl(int nTid, CAccountField *pAccount, CTransaction *pTransaction)
{
	//ToDo:如果userType正常之后，这个需要删除
	//if (nTid == TID_SendInit && g_nBeginID == -1)
	//{
	//	if (pAccount->Available < 0.0)
	//	{
	//		pAccount->PreBalance -= pAccount->Available;
	//		CWriteableAccount theAccount;
	//		CopyAccountEntity(&theAccount, pAccount);
	//		applyAccountChange(&theAccount);
	//		CopyAccountEntity(pAccount, &theAccount);
	//	}			
	//}
	//pAccount->TotalCloseProfit = pAccount->CloseProfit;
	//pAccount->TotalDeposit = pAccount->Deposit;
	//pAccount->TotalWithdraw = pAccount->Withdraw;
	//pAccount->TotalMoneyChange = pAccount->MoneyChange;
	//pAccount->TotalFee = pAccount->Fee;

	switch (nTid)
	{
	case TID_SendInit:
	{
		if (g_bOnlyRedo)
			break;
		break;
	}
	case TID_SendDelete:
	{
		CAccount *pAccountToRemove = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pAccount->MemberID, pAccount->AccountID, pAccount->SettlementGroup, pAccount->Currency);
		if (pAccountToRemove == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		if (pAccountToRemove->Balance != 0.0 || pAccountToRemove->PreBalance != 0.0)
			return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "BalanceIsNotZero");

		CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByAccountID(pAccountToRemove->AccountID);
		if (pTriggerOrder != NULL)
			return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "TriggerOrderExist[%s]", pTriggerOrder->OrderID.getValue());

		COrder* pOrder = m_pMDB->m_OrderFactory->findFirstByAccountID(pAccountToRemove->AccountID);
		if (pOrder != NULL)
			return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "OrderExist");

		CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByAccountID(pAccountToRemove->AccountID);
		if (pPosition != NULL)
			return m_pErrorEngine->reportError(ERROR_POSITION_EXIST);

		break;
	}
	default:
	{
		break;
	}
	}
	return CAlterService::SyncAccountImpl(nTid, pAccount, pTransaction);
}

bool CMyAlterService::SyncServiceConfigImpl(int nTid, CServiceConfigField *pServiceConfig, CTransaction *pTransaction)
{
	switch (nTid)
	{
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
		if (pServiceConfig->ConfigName == "TimerDay")
			formatTime(pServiceConfig->Index1.getBuffer());

		if (pServiceConfig->ConfigName == "FeeDeducePercent")
		{
			double configValue = atof(pServiceConfig->ConfigValue.getValue());
			if (compare(configValue, 0.0) <= 0 || compare(configValue, 1.0) > 0)
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
		}
		if (pServiceConfig->ConfigName == "MaxExchangeRateGapRate")
		{
			double configValue = atof(pServiceConfig->ConfigValue.getValue());
			if (compare(configValue, 0.0) <= 0 || compare(configValue, 1.0) > 0)
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
		}
		break;
	default:
		break;
	}
	return CAlterService::SyncServiceConfigImpl(nTid, pServiceConfig, pTransaction);
}

bool CMyAlterService::SyncLastKLineImpl(int nTid, CLastKLineField *pLastKLine, CTransaction *pTransaction)
{
	if (nTid == TID_SendInit && !g_bOnlyRedo)
	{
	}
	return CAlterService::SyncLastKLineImpl(nTid, pLastKLine, pTransaction);
}

bool CMyAlterService::SyncMemberImpl(int nTid, CMemberField *pMember, CTransaction *pTransaction)
{
	pMember->SinkType = ST_SinkToDB;

	// 增删改时，检查是否存在持仓
	switch (nTid)
	{
	case TID_SendInit:
	{
		if (g_bOnlyRedo)
			break;
		break;
	}
	case TID_SendDelete:
	{
		//CAccount* pAccount = m_pMDB->m_AccountFactory->findFirstByByMemberID(,pMember->MemberID);
		//if (pAccount != NULL)
		//	return m_pErrorEngine->reportError(ERROR_RECORD_EXIST, "AccountExist");
		break;
	}
	default:
	{
		break;
	}
	}
	return  CAlterService::SyncMemberImpl(nTid, pMember, pTransaction);
}

bool CMyAlterService::SyncOrderImpl(int nTid, COrderField *pOrder, CTransaction *pTransaction)
{
	switch (nTid)
	{
	case TID_SendInit:
	{
		if (g_bOnlyRedo)
			break;
		break;
	}
	default:
	{
		break;
	}
	}
	return  CAlterService::SyncOrderImpl(nTid, pOrder, pTransaction);
}

bool CMyAlterService::SyncTriggerOrderImpl(int nTid, CTriggerOrderField *pTriggerOrder, CTransaction *pTransaction)
{
	switch (nTid)
	{
	case TID_SendInit:
	{
		if (g_bOnlyRedo)
			break;
	}
	default:
	{
		break;
	}
	}
	return  CAlterService::SyncTriggerOrderImpl(nTid, pTriggerOrder, pTransaction);
}

bool CMyAlterService::SyncCurrencyImpl(int nTid, CCurrencyField *pCurrency, CTransaction *pTransaction)
{
	switch (nTid)
	{
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	{
		break;
	}
	case TID_SendDelete:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	default:
		break;
	}

	return  CAlterService::SyncCurrencyImpl(nTid, pCurrency, pTransaction);
}

bool CMyAlterService::SyncPositionImpl(int nTid, CPositionField *pPosition, CTransaction *pTransaction)
{
	switch (nTid)
	{
	case TID_SendInit:
	{
		if (g_bOnlyRedo)
			break;
		break;
	}
	case TID_SendDelete:
	{
		CPosition *pPositionToRemove = m_pMDB->m_PositionFactory->findByPK(pPosition);
		if (pPositionToRemove == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		if (pPositionToRemove->Position != 0.0 || pPositionToRemove->LongFrozen != 0.0 || pPositionToRemove->ShortFrozen != 0.0)
			return m_pErrorEngine->reportError(ERROR_POSITION_EXIST, "PositionIsNotZero");

		CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByAccountID(pPositionToRemove->AccountID);
		while (pTriggerOrder != NULL)
		{
			if (pTriggerOrder->InstrumentID == pPositionToRemove->InstrumentID &&
				pTriggerOrder->PosiDirection == pPositionToRemove->PosiDirection)
				return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "TriggerOrderExist");
			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByAccountID();
		}

		COrder* pOrder = m_pMDB->m_OrderFactory->findFirstByAccountID(pPositionToRemove->AccountID);
		while (pOrder != NULL)
		{
			if (pOrder->InstrumentID == pPositionToRemove->InstrumentID &&
				pOrder->PosiDirection == pPositionToRemove->PosiDirection)
				return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "OrderExist");
			pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
		}

		break;
	}
	default:
	{
		break;
	}
	}
	return  CAlterService::SyncPositionImpl(nTid, pPosition, pTransaction);
}

bool CMyAlterService::SyncUserSessionImpl(int nTid, CUserSessionField *pUserSession, CTransaction *pTransaction)
{
	CUserSession *pUserSessionOld = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(pUserSession->UserID, pUserSession->APPID);
	if (pUserSessionOld != NULL)
		return m_pErrorEngine->reportError(ERROR_RECORD_EXIST, "UserID:%s,APPID:%s", pUserSession->UserID.getValue(), pUserSession->APPID.getValue());

	switch (nTid)
	{
	case TID_SendInit:
	{
		if (g_bOnlyRedo)
			break;
		CCurrentTime *pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
		if (pCurrentTime != NULL && !pUserSession->ExpireTime.isNull() && pUserSession->ExpireTime < pCurrentTime->CurrTime)
			return false;
		break;
	}
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	{
		if (pUserSession->MemberID.isNull())
			pUserSession->MemberID = pUserSession->UserID;

		if (pUserSession->Token.isNull())
		{
			char sToken[33];
			MD5Hex(sToken, pUserSession->UserID.getValue(), pUserSession->LoginTime.getString().c_str(), pUserSession->APPID);
			pUserSession->Token = sToken;
		}

		if (m_pMDB->m_MemberFactory->findByMemberID(pUserSession->MemberID) == NULL)
		{
			CMemberField theMember;
			theMember.MemberID = pUserSession->MemberID;
			theMember.MemberType = PT_Normal;
			theMember.SinkType = ST_SinkToDB;
			SyncMemberImpl(nTid, &theMember, pTransaction);
		}
		break;
	}
	default:
	{
		break;
	}
	}
	return  CAlterService::SyncUserSessionImpl(nTid, pUserSession, pTransaction);
}

void CMyAlterService::afterSync(CTransaction* pTransaction)
{
	CServiceConfig* pServiceConfig = m_pMDB->m_ServiceConfigFactory->findByConfig("BaseClearCurrency", "default", "default");
	if (pServiceConfig != NULL)
	{
		CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pServiceConfig->ConfigValue.getValue(), pServiceConfig->Remark.getValue());
		if (pCurrency == NULL)
		{
			CWriteableCurrency theCurrency;
			theCurrency.init();
			theCurrency.Currency = pServiceConfig->ConfigValue.getValue();
			theCurrency.SettlementGroup = pServiceConfig->Remark.getValue();
			m_pMDB->m_CurrencyFactory->add(&theCurrency);
		}
	}
	return  CAlterService::afterSync(pTransaction);
}