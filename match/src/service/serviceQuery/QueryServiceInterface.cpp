// QueryServiceInterface.cpp: 
//////////////////////////////////////////////////////////////////////
#include "QueryServiceInterface.h"
#include "CopyField.h"
#include "tradingTools.h"
#include "ServiceShare.h"

CQueryServiceInterface::CQueryServiceInterface(CServiceResponser *pServiceResponser)
{
	m_pServiceResponser = pServiceResponser;
}

CQueryServiceInterface::~CQueryServiceInterface()
{
	delete m_pBaseDepositAlgorithms;
}

bool CQueryServiceInterface::RspError(int nRecvTNo, TSessionRef* pSession, void *pField, CFieldDescribe *pFieldDescribe)
{
	char* errMsg = NULL;
	int errorCode = 0;
	m_pErrorEngine->getLastError(&errorCode, &errMsg);
	if (pFieldDescribe == NULL)
	{
		CErrorField responseField;
		responseField.ErrorNo = errorCode;
		responseField.ErrorMsg = errMsg;
		m_pServiceResponser->SendResponse(nRecvTNo, &responseField, &(responseField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE, errorCode, errMsg);
	}
	else
	{
		m_pServiceResponser->SendResponse(nRecvTNo, pField, pFieldDescribe, pSession, OUTFOG_CHAIN_SINGLE, errorCode, errMsg);
	}
	return false;
}

bool CQueryServiceInterface::handleMessage(const TOutFogMetaRef* pOutFogReqRef, const void *pStream, TSessionRef* pSession)
{
	//反演情况下，查询不做
	//if (pOutFogReqRef->IsReplay)
	//	return true;

//	bool success = false;
//	char* errMsg;
//	int errorCode = 0;
	int nRecvTNo = 0;

	switch (pOutFogReqRef->MessageNo)
	{
	case TID_SendQryOrder:
	{
		nRecvTNo = TID_RecvQryOrder;
		CQryOrderField field;
		CQryOrderField::m_Describe.getSingleField(&field, pStream);
		if (!CheckSession(m_pMDB, m_pErrorEngine, pSession, field.MemberID))
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		if (field.AccountID.isNull())
			field.AccountID = field.MemberID;

		COrderField rspField;
		COrder* pData = NULL;
		if (!field.LocalID.isNull())
		{
			pData = m_pMDB->m_OrderFactory->findByLocalID(field.AccountID, field.LocalID);
			if (pData != NULL)
			{
				CopyOrderEntity(&rspField, pData);
				m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			}
			else
			{
				m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
				RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
			break;
		}

		if (!field.OrderID.isNull())
		{
			pData = m_pMDB->m_OrderFactory->findByOrderID(field.OrderID);
			if (pData != NULL)
			{
				CopyOrderEntity(&rspField, pData);
				m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			}
			else
			{
				m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
				RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
			break;
		}

		if (!field.MemberID.isNull())
		{
			m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
			pData = m_pMDB->m_OrderFactory->findFirstByAccountID(field.AccountID);
			while (pData != NULL)
			{
				if (!field.InstrumentID.isNull() && (field.InstrumentID != pData->InstrumentID || field.ExchangeID != pData->ExchangeID))
				{
					pData = m_pMDB->m_OrderFactory->findNextByAccountID();
					continue;
				}
				CopyOrderEntity(&rspField, pData);
				m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);

				pData = m_pMDB->m_OrderFactory->findNextByAccountID();
			}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
			break;
		}
		else
		{
			if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

			m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
			pData = m_pMDB->m_OrderFactory->getFirst();
			while (pData != NULL)
			{
				if (!field.ExchangeID.isNull() && field.ExchangeID != pData->ExchangeID)
				{
					pData = m_pMDB->m_OrderFactory->getNext();
					continue;
				}
				if (!field.InstrumentID.isNull() && field.InstrumentID != pData->InstrumentID)
				{
					pData = m_pMDB->m_OrderFactory->getNext();
					continue;
				}
				if (!field.AccountID.isNull() && field.AccountID != pData->AccountID)
				{
					pData = m_pMDB->m_OrderFactory->getNext();
					continue;
				}
				CopyOrderEntity(&rspField, pData);
				m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
				pData = m_pMDB->m_OrderFactory->getNext();
			}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		}
		break;
	}
	case TID_SendQryPosition:
	{
		nRecvTNo = TID_RecvQryPosition;
		CQryPositionField field;
		CQryPositionField::m_Describe.getSingleField(&field, pStream);
		if (field.MemberID.isNull())
			field.AccountID = field.MemberID;
		if (!CheckSession(m_pMDB, m_pErrorEngine, pSession, field.MemberID))
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		if (field.AccountID.isNull())
			field.AccountID = field.MemberID;

		CPositionField rspField;
		CPosition* pData = NULL;

		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
		pData = m_pMDB->m_PositionFactory->getFirst();
		while (pData != NULL)
		{
			if (!field.AccountID.isNull())
			{
				if (field.AccountID != pData->AccountID)
				{
					pData = m_pMDB->m_PositionFactory->getNext();
					continue;
				}
			}

			if (!field.InstrumentID.isNull())
			{
				if ((field.InstrumentID != pData->InstrumentID || field.ExchangeID != pData->ExchangeID))
				{
					pData = m_pMDB->m_PositionFactory->getNext();
					continue;
				}
			}

			if (!field.PositionClass.isNull())
			{
				if ((field.PositionClass != pData->PositionClass))
				{
					pData = m_pMDB->m_PositionFactory->getNext();
					continue;
				}
			}

			if (pData->Position == 0.0)
			{
				pData = m_pMDB->m_PositionFactory->getNext();
				continue;
			}

			CopyPositionEntity(&rspField, pData);
			m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pData = m_pMDB->m_PositionFactory->findNextByAccountID();
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendQryTriggerOrder:
	{
		nRecvTNo = TID_RecvQryTriggerOrder;
		CQryTriggerOrderField field;
		CQryTriggerOrderField::m_Describe.getSingleField(&field, pStream);
		if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		CTriggerOrderField rspField;
		CTriggerOrder* pData = NULL;
		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
		pData = m_pMDB->m_TriggerOrderFactory->getFirst();
		while (pData != NULL)
		{
			if (!field.AccountID.isNull() && field.AccountID != pData->AccountID)
			{
				pData = m_pMDB->m_TriggerOrderFactory->getNext();
				continue;
			}
			if (!field.InstrumentID.isNull() && (field.InstrumentID != pData->InstrumentID || field.ExchangeID != pData->ExchangeID))
			{
				pData = m_pMDB->m_TriggerOrderFactory->getNext();
				continue;
			}
			CopyTriggerOrderEntity(&rspField, pData);
			m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pData = m_pMDB->m_TriggerOrderFactory->getNext();
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendQryInstrument:
	{
		nRecvTNo = TID_RecvQryInstrument;
		CQryInstrumentField field;
		CQryInstrumentField::m_Describe.getSingleField(&field, pStream);

		CInstrumentField rspField;
		CInstrument* pData = NULL;

		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
		pData = m_pMDB->m_InstrumentFactory->getFirst();
		while (pData != NULL)
		{
			if (!field.ExchangeID.isNull() && field.ExchangeID != pData->ExchangeID) {
				pData = m_pMDB->m_InstrumentFactory->getNext();
				continue;
			}
			if (!field.ProductGroup.isNull() && field.ProductGroup != pData->ProductGroup) {
				pData = m_pMDB->m_InstrumentFactory->getNext();
				continue;
			}
			if (!field.InstrumentID.isNull() && field.InstrumentID != pData->InstrumentID) {
				pData = m_pMDB->m_InstrumentFactory->getNext();
				continue;
			}
			CopyInstrumentEntity(&rspField, pData);
			m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pData = m_pMDB->m_InstrumentFactory->getNext();
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendQryMarketData:
	{
		CQryMarketDataField field;
		field.m_Describe.getSingleField(&field, pStream);
		SendMarketData(m_pMDB, m_pServiceResponser, field, pSession);
		break;
	}
	case TID_SendQryCurrency:
	{
		CQryCurrencyField field;
		field.m_Describe.getSingleField(&field, pStream);
		SendCurrency(m_pMDB, m_pServiceResponser, field, pSession);
		break;
	}
	case TID_SendQryMarketOrder:
	{
		CQryMarketOrderField field;
		CQryMarketOrderField::m_Describe.getSingleField(&field, pStream);
		SendMarketOrder(m_pMDB, m_pServiceResponser, field, pSession);
		break;
	}
	case TID_SendQryAccount:
	{
		nRecvTNo = TID_RecvQryAccount;
		CQryAccountField field;
		CQryAccountField::m_Describe.getSingleField(&field, pStream);
		if (!CheckSession(m_pMDB, m_pErrorEngine, pSession, field.MemberID))
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		CAccountField rspField;
		CAccount* pData = NULL;

		if (!field.MemberID.isNull())
		{
			m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
			CAccount* pData = m_pMDB->m_AccountFactory->findFirstByMemberID(field.MemberID);
			while (pData != NULL)
			{
				if (!field.AccountID.isNull() && field.AccountID != pData->AccountID)
				{
					pData = m_pMDB->m_AccountFactory->findNextByMemberID();
					continue;
				}
				if (!field.SettlementGroup.isNull() && field.SettlementGroup != pData->SettlementGroup)
				{
					pData = m_pMDB->m_AccountFactory->findNextByMemberID();
					continue;
				}
				if (!field.Currency.isNull() && field.Currency != pData->Currency)
				{
					pData = m_pMDB->m_AccountFactory->findNextByMemberID();
					continue;
				}
				CopyAccountEntity(&rspField, pData);
				m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
				pData = m_pMDB->m_AccountFactory->findNextByMemberID();
			}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
			break;
		}
		if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
		pData = m_pMDB->m_AccountFactory->getFirst();
		while (pData != NULL)
		{
			CopyAccountEntity(&rspField, pData);
			m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pData = m_pMDB->m_AccountFactory->getNext();
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	//case TID_SendQryAPIKey:
	//{
	//	nRecvTNo = TID_RecvQryAPIKey;
	//	CQryAPIKeyField field;
	//	CQryAPIKeyField::m_Describe.getSingleField(&field, pStream);

	//	if (!CheckSession(m_pMDB, m_pErrorEngine, pSession, "", &(field.UserID)))
	//		return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

	//	CAPIKeyField rspField;
	//	CUserSession *pData = NULL;

	//	m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
	//	pData = m_pMDB->m_UserSessionFactory->findFirstByUserID(field.UserID);
	//	while (pData != NULL)
	//	{
	//		if (!pData->APPID.isNull()) //忽略不是APPKey的数据
	//		{
	//			pData = m_pMDB->m_UserSessionFactory->findNextByUserID();
	//			continue;
	//		}

	//		memset(&rspField, 0, sizeof(CAPIKeyField));
	//		CAllow *pAuthAllow = m_pMDB->m_AllowFactory->findByAllowIDAccessTypeValue(pData->Token, AT_APIKey, "Auth");
	//		if (pAuthAllow != NULL)
	//		{
	//			rspField.Auth = pAuthAllow->AccessValue;
	//		}
	//		CAllow *pIPAllow = m_pMDB->m_AllowFactory->findByAllowIDAccessTypeValue(pData->Token, AT_APIKey, "IPAddress");
	//		if (pIPAllow != NULL)
	//		{
	//			rspField.IPAddress = pIPAllow->AccessValue;
	//		}

	//		rspField.UserID = pData->UserID;
	//		rspField.AccessKey = pData->Token;
	//		//rspField.SecretKey = pData->AuthCode;
	//		rspField.CreateTime = pData->LoginTime;
	//		rspField.ExpireTime = pData->ExpireTime;
	//		rspField.Name = pData->Remark;
	//		double RemainDays = 0;
	//		if (pData->ExpireTime == INT_32_MAX)
	//		{
	//			RemainDays = 99999;
	//		}
	//		else
	//		{
	//			//CMilliSecondsType CurrentTime = getMDBTime(m_pMDB);
	//			RemainDays = (pData->ExpireTime - g_nCurrentTime) / (24 * 60 * 60);
	//		}
	//		rspField.RemainDays = RemainDays > 0 ? RemainDays : 0;
	//		rspField.Status = RemainDays > 0 ? 1 : 0;
	//		m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);

	//		pData = m_pMDB->m_UserSessionFactory->findNextByUserID();
	//	}
	//	m_pMDB->m_UserSessionFactory->endFindByUserID();
	//	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	//	break;
	//}
	case TID_SendQryMarginRate:
	{
		nRecvTNo = TID_RecvQryMarginRate;
		CQryMarginRateField field;
		CQryMarginRateField::m_Describe.getSingleField(&field, pStream);

		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if (pInstrument == NULL)
		{
			m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		CAccount *pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(field.MemberID, field.AccountID, pInstrument->SettlementGroup, pInstrument->ClearCurrency);
		CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
		CMarginRate *pRate = getMarginRate(m_pMDB, pInstrument->MarginRateGroup.getValue(), pAccount);
		if (pRate == NULL)
		{
			m_pErrorEngine->reportError(ERROR_CAN_NOT_FIND_MARGIN_RATE);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}

		CMarginRateField rspField;
		CopyMarginRateEntity(&rspField, pRate);
		m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendQryFee:
	{
		nRecvTNo = TID_RecvQryFee;
		CQryFeeField field;
		CQryFeeField::m_Describe.getSingleField(&field, pStream);
		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if (pInstrument == NULL)
		{
			m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
		CFee *pFee = getFee(m_pMDB, field.MatchRole, pInstrument, pMember);
		if (pFee == NULL)
		{
			m_pErrorEngine->reportError(ERROR_CAN_NOT_FIND_FEE);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}

		CFeeField rspField;
		CopyFeeEntity(&rspField, pFee);
		m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendQryMember:
	{
		nRecvTNo = TID_RecvQryMember;
		CQryMemberField field;
		CQryMemberField::m_Describe.getSingleField(&field, pStream);

		CMemberField rspField;
		CMember *pData = NULL;
		if (!field.MemberType.isNull())
		{
			//if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
			//	return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

			//m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, pSession, OUTFOG_CHAIN_FIRST);
			//pData = m_pMDB->m_MemberFactory->findFirstByMemberType(field.MemberType);
			//while (pData != NULL)
			//{
			//	CopyMemberEntity(&rspField, pData);
			//	rspField.PositionType = transPositionType(rspField.PositionType);
			//	m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			//	pData = m_pMDB->m_MemberFactory->findNextByMemberType();
			//}
			//m_pMDB->m_MemberFactory->endFindByMemberType();
			//m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		}
		else
		{
			if (!CheckSession(m_pMDB, m_pErrorEngine, pSession, field.MemberID))
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			pData = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);

			if (pData != NULL)
			{
				CopyMemberEntity(&rspField, pData);
				rspField.PositionType = transPositionType(rspField.PositionType);
				m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			}
			else
			{
				m_pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
		}

		break;
	}
	case TID_SendQryLeverage:
	{
		nRecvTNo = TID_RecvQryLeverage;
		CQryLeverageField field;
		CQryLeverageField::m_Describe.getSingleField(&field, pStream);
		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if (pInstrument == NULL)
		{
			m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}

		CLeverageField rspField;
		CopyQryLeverageEntity(&rspField, &field);
		if (!queryMarginAndLeverage(m_pMDB, m_pErrorEngine, pInstrument, field.MemberID, field.AccountID, field.TradeUnitID, 0, &rspField, 0, 0, 0))
		{
			//m_pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}

		m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendQryWithLeverage:
	{
		nRecvTNo = TID_RecvQryWithLeverage;
		CQryWithLeverageField field;
		CQryWithLeverageField::m_Describe.getSingleField(&field, pStream);
		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if (pInstrument == NULL)
		{
			m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		//if (field.LongLeverage < 1 || field.ShortLeverage <1)
		//{
		//	//m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
		//	m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Leverage should >= 1");
		//	return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		//}

		CLeverageField rspField;
		CopyQryLeverageEntity(&rspField, &field);
		if (field.NetLeverage.isNull())
			field.NetLeverage = 0;
		if (field.LongLeverage.isNull())
			field.LongLeverage = 0;
		if (field.ShortLeverage.isNull())
			field.ShortLeverage = 0;
		if (!queryMarginAndLeverage(m_pMDB, m_pErrorEngine, pInstrument, field.MemberID, field.AccountID, field.TradeUnitID, -1, &rspField, field.LongLeverage, field.ShortLeverage, field.NetLeverage))
		{
			//m_pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}

		m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	default:
		return true;
	}
	return false;
}

bool CQueryServiceInterface::handleRequest(const TOutFogMetaRef* pOutFogReqRef, const void *pStream, TSessionRef* pSession)
{
	return true;
}