#include "App.h"
#include "public.h"
#include "CopyField.h"
#include "mdbStruct.h"
#include "ServiceShare.h"
#include "tradingTools.h"
#include "printinfo.h"
#include "monitorIndex.h"
#include "CFilelogService.h"

void CmdLineArgError()
{
	printf("Usage: private 0 1 ... n|-h|-v");
	printf("       private -v      :	Show Version\n");
	printf("       private -h      :	Show Manual\n");
	printf("       private 1       :	start private at 1 \n");
	exit(0);
}

CApp::CApp()
{
	CMonitorIndex::init(CFilelogService::CreateInstance(GetConfigString(INI_FILE_NAME, "SyslogPath").c_str()), GetConfigString(INI_FILE_NAME, "LogLevel").c_str());
	m_nReady = 0;
	m_sGateWayKey = GetConfigString(NetworkConfigFile, "GateWayKey");
	m_FirstTime.clear();

	if (m_sGateWayKey == "")
		m_sGateWayKey = "kdfhaurksh*uerhgsgjkjesbhDuu#@kdhf";

	//按照00:00:00计算出一个一个余数，这个就是和UTC时间的差值
	time_t nCurrTime = time(NULL);
	struct tm timeinfo = GetTM(&nCurrTime);
	char sTime[9];
	strftime(sTime, 11, "%H:%M:%S", &timeinfo);
	int UTCSeconds = getSecondMinus(sTime, "00:00:00");
	m_nTimeZone = (nCurrTime - UTCSeconds) % (24 * 60 * 60);

	g_cMDBRunLevel = 'p';
	m_pMDB = new CMDB();
	int memorySize = GetConfigInt(INI_FILE_NAME, "MemorySize");
	int blockCount = GetConfigInt(INI_FILE_NAME, "MaxBlockCount");
	m_pMDB->init(memorySize, blockCount);

	memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	m_Ini.Open(INI_FILE_NAME);
	m_pErrorEngine = new CErrorEngine();
	defineErrors(m_pErrorEngine);

	m_pLoginService = new CLoginService(m_pMDB, m_pErrorEngine);

	//m_pMDB->saveAll("snap/snap.100000/");
}

CApp::~CApp()
{
}

bool CApp::RspField(const TOutFogMetaRef* pOutFogRspRef, const void* pRspBuffer)
{
	if (pOutFogRspRef->IsReplay >= 0)
		return true;

	sessionRef.frontID = pOutFogRspRef->FrontNo;
	sessionRef.sessionID = pOutFogRspRef->SessionNo;
	sessionRef.requestID = pOutFogRspRef->RequestNo;
	sessionRef.pToken = pOutFogRspRef->Remark;
	sessionRef.pUserID = pOutFogRspRef->UserID;
	sessionRef.zip = pOutFogRspRef->SubjectNo;

	switch (pOutFogRspRef->Type)
	{
	case OUTFOG_TYPE_MANAGER:
	{
		CErrorField responseField;
		if (!strcmp(pOutFogRspRef->SubIndexName, "DumpMDB"))
		{
			char szPath[256];
			sprintf(szPath, "%s/snap.%.0f/", GetConfigString(INI_FILE_NAME, "SnapPath").c_str(), pOutFogRspRef->SequenceNo);
			//m_pMDB->save(szPath, nSequenceNo);
			if (!m_pMDB->saveAll(szPath, pOutFogRspRef->SequenceNo))
				return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "MDBSnapStatusIsNot0");

			responseField.ErrorNo = 0;
			responseField.ErrorMsg = "DumpMDB_Success";
			m_pServiceResponser->SendResponse(0, &responseField, &(responseField.m_Describe), &sessionRef, OUTFOG_CHAIN_SINGLE, 0, "DumpMDB_Success");
			return true;
		}
	}
	case OUTFOG_TYPE_ADDSESSION:
	{
		//如果系统没有准备好，只能直接断开链接,50次100ms差不多5秒之后启动连接
		if (m_nReady < 5)
		{
			m_pServiceResponser->SendResponse(0, NULL, NULL, &sessionRef, OUTFOG_CHAIN_SINGLE, ERROR_SystemNotReady, "SystemNotReady!");
			m_pServiceResponser->DelSession(pOutFogRspRef->FrontNo, pOutFogRspRef->SessionNo);
			PRINT_TO_STD("LinkID:%0.f,AddSession[%d/%d]:SystemNotReady", pOutFogRspRef->GlobalNo, pOutFogRspRef->FrontNo, pOutFogRspRef->SessionNo);
		}
		return true;
	}
	case OUTFOG_TYPE_REDOOVER:
		return true;
	case OUTFOG_TYPE_TIMER:
	{
		//PRINT_TO_STD_US("OUTFOG_TYPE_TIMER:%s,%d", pOutFogRspRef->UserID, pOutFogRspRef->RequestNo);
		return true;
	}
	default:
	{
	}
	}

	nRecvTNo = 0;

	//if (!pOutFogRspRef->IsReplay)
	//	exit(-1);

	switch (pOutFogRspRef->MessageNo)
	{
	case TID_RecvUserLogin:
	{
		PRINT_TO_STD("LinkID:%0.f,Login:%s", pOutFogRspRef->GlobalNo, pOutFogRspRef->Remark);
		//如果是错误的登录，需要提醒注意
		if (pOutFogRspRef->RequestNo != 0)
			break;

		//如果可以订阅了，这里就订阅，防止中途断线无法继续接受行情的情况
		CLinkMap::iterator iter = m_LinkFrom.find(pOutFogRspRef->GlobalNo);
		if (iter != m_LinkFrom.end())
		{
			SubTopics(iter->second);
		}

		//如果是准备好了，出现了登陆的情况，就是断线重连，断线重连
		if (m_nReady > 0)
		{
			CMarketOrder* pMarketOrder = m_pMDB->m_MarketOrderFactory->getFirst();
			if (pMarketOrder != NULL)
			{
				CMarketOrder* pTobeRemove = pMarketOrder;
				pMarketOrder = m_pMDB->m_MarketOrderFactory->getNext();
				m_pMDB->m_MarketOrderFactory->remove(pTobeRemove);
			}
			CTickMarketOrder* pTickMarketOrder = m_pMDB->m_TickMarketOrderFactory->getFirst();
			if (pTickMarketOrder != NULL)
			{
				CTickMarketOrder* pTobeRemove = pTickMarketOrder;
				pTickMarketOrder = m_pMDB->m_TickMarketOrderFactory->getNext();
				m_pMDB->m_TickMarketOrderFactory->remove(pTobeRemove);
			}
		}
		break;
	}
	case TID_SendCheckToken:
	{
		nRecvTNo = TID_RecvCheckToken;
		CCheckTokenField field;
		CCheckTokenField::m_Describe.getSingleField(&field, pRspBuffer);

		CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByToken(field.Token);
		if (pUserSession == NULL)
		{
			pUserSession = m_pMDB->m_UserSessionFactory->findByToken(sessionRef.pToken);
			if (pUserSession == NULL)
			{
				m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);
				return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &sessionRef, &field, &(field.m_Describe));
			}
		}
		field.Token = pUserSession->Token;
		field.MaxLocalID = pUserSession->MaxLocalID;
		field.MemberID = pUserSession->MemberID;
		field.UserID = pUserSession->UserID;
		m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), &sessionRef, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_RecvTopicAction:
	{
		CTopicActionField field;
		CTopicActionField::m_Describe.getSingleField(&field, pRspBuffer);

		if (pOutFogRspRef->RequestNo != 0)
		{
			PRINT_TO_STD("LinkID:%0.f:%s:TopicID[%s]:Index[%s]", pOutFogRspRef->GlobalNo, pOutFogRspRef->Remark, field.TopicID.getValue(), field.Index.getValue());
			break;
		}

		PRINT_TO_STD("LinkID:%0.f,Sub Success:TopicID[%s]:Index[%s]", pOutFogRspRef->GlobalNo, field.TopicID.getValue(), field.Index.getValue());
		break;
	}
	case TID_SendUserLogin:
	{
		nRecvTNo = TID_RecvUserLogin;
		CUserLoginField field;
		CUserLoginField::m_Describe.getSingleField(&field, pRspBuffer);

		CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByToken(field.Token);
		if (pUserSession == NULL)
		{
			m_pErrorEngine->reportError(ERROR_USER_NO_PERMISSION);
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &sessionRef);
		}

		//if (pUserSession->UserID != field.UserID)
		//{
		//	m_pErrorEngine->reportError(ERROR_USER_MISMATCH);
		//	return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, sessionRef.frontID, sessionRef.sessionID, sessionRef.requestID);
		//}
		CWriteableUserSession UserSession;
		m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &UserSession);
		UserSession.FrontNo = sessionRef.frontID;
		UserSession.SessionNo = sessionRef.sessionID;
		UserSession.ExpireTime = m_CurrentTimeField.CurrTime.getValue() + 60 * 60 * 24 * 2;
		m_pMDB->m_UserSessionFactory->update(pUserSession, &UserSession);
		CUserSessionField userSessionField;
		CopyUserSessionEntity(&userSessionField, pUserSession);
		m_pServiceResponser->SendResponse(TID_RecvUserLogin, &userSessionField, &(userSessionField.m_Describe), &sessionRef, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendTopicAction:
	{
		if (m_nReady < 5)
		{
			m_pServiceResponser->SendResponse(0, NULL, NULL, &sessionRef, OUTFOG_CHAIN_SINGLE, ERROR_SystemNotReady, "SystemNotReady!");
			m_pServiceResponser->DelSession(pOutFogRspRef->FrontNo, pOutFogRspRef->SessionNo);
			PRINT_TO_STD("LinkID:%0.f,SendTopicAction[%d/%d]:SystemNotReady", pOutFogRspRef->GlobalNo, pOutFogRspRef->FrontNo, pOutFogRspRef->SessionNo);
			break;
		}

		bool success = true;
		nRecvTNo = TID_RecvTopicAction;
		CTopicActionField field;
		CTopicActionField::m_Describe.getSingleField(&field, pRspBuffer);
		switch (field.Action)
		{
		case SA_Sub:
		{
			int nReaderTimes = SubTopicAction(&field, &sessionRef);
			if (nReaderTimes <= 0)
				return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &sessionRef, &field, &(field.m_Describe));
			m_pServiceResponser->SubSubjectID(TID_RecvTopicAction, field.TopicID.getInt(), field.ResumeNo.getValue(), field.Index.getValue(), &sessionRef, field.Zip.getValue(), UF_INT8_MAX, nReaderTimes);
			break;
		}
		case SA_UnSub:
		{
			m_pServiceResponser->UnSubSubjectID(field.TopicID.getInt(), field.Index.getValue(), &sessionRef);
			break;
		}
		case SA_Clear:
		{
			m_pServiceResponser->UnSubSubjectIDAll(&sessionRef);
			break;
		}
		default:
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &sessionRef, &field, &(field.m_Describe));
		}
		}
		m_pServiceResponser->SendResponse(TID_RecvTopicAction, &field, &(field.m_Describe), &sessionRef, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_PushMarketOverView:
	{
		CMarketOverViewField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		//PRINT_TO_STD("MarketOverView:[%s_%s],[%0.4f/%0.2f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.LastPrice.getValue(), field.Turnover.getValue());
		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		CWriteableMarketData theMarketData;
		//bool bDelay = false;
		if (pMarketData == NULL)
		{
			theMarketData.init();
			CopyMarketOverViewEntity(&theMarketData, &field);
			theMarketData.HighestPrice24 = field.HighestPrice;
			theMarketData.LowestPrice24 = field.LowestPrice;
			theMarketData.Volume24 = field.Volume;
			theMarketData.Turnover24 = field.Turnover;
			theMarketData.OpenPrice24 = field.OpenPrice;
			//bDelay = true;
			m_pMDB->m_MarketDataFactory->add(&theMarketData);
		}
		else
		{
			//if (field.UpdateTime / 1000 > pMarketData->UpdateTime / 1000)
			//	bDelay = true;
			m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
			CopyMarketDataUpdateTimeEntity(&theMarketData, &field);
			CopyMarketDataLastMatchEntity(&theMarketData, &field);
			theMarketData.InstrumentStatus = field.InstrumentStatus;
			theMarketData.PrePositionFeeRate = field.PrePositionFeeRate;
			m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
		}
		return false;
	}
	case TID_PushUserSession:
	{
		CUserSessionField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(field.UserID, field.APPID);
		if (pUserSession != NULL)
		{
			if (field.Token != pUserSession->Token)
			{
				m_pMDB->m_UserSessionFactory->remove(pUserSession);
				m_pServiceResponser->DelSession(pUserSession->FrontNo.getValue(), pUserSession->SessionNo.getValue());
				m_pServiceResponser->DelApiKey(pUserSession->Token.getValue());
			}
		}

		CWriteableUserSession theUserSession;
		//PRINT_TO_STD("UserSession:[%s]", field.Token.getValue());
		pUserSession = m_pMDB->m_UserSessionFactory->findByToken(field.Token);
		if (pUserSession == NULL)
		{
			theUserSession.init();
			CopyUserSessionEntity(&theUserSession, &field);
			m_pMDB->m_UserSessionFactory->add(&theUserSession);
		}
		else
		{
			m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
			CopyUserSessionEntity(&theUserSession, &field);
			m_pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession);
		}

		m_pServiceResponser->AddApiKey(field.Token.getValue(), field.AuthCode.getValue(), field.UserID.getValue(), field.ExpireTime.getValue(), field.AccessLimit.getValue());
		break;
	}
	case TID_PushTimeSync:
	{
		CMilliSecondsType CurrTime = m_CurrentTimeField.CurrTime;
		m_CurrentTimeField.m_Describe.getSingleField(&m_CurrentTimeField, pRspBuffer);
		//if (CurrTime != m_CurrentTimeField.CurrTime)
		//	m_SubMap.clear();

		//PRINT_TO_STD_US("TID_PushTimeSync:%s,%d", m_CurrentTimeField.ZoneTime.getValue(), m_CurrentTimeField.MilliSecond.getValue());

		//CCurrentTimeField field;
		//m_CurrentTime = field.CurrTime;
		//m_TimeZone = field.TimeZone;
		//memcpy(&m_CurrentTimeField, &field, sizeof(CCurrentTimeField));

		CMonitorIndex::handleOne();
		if (m_nReady < 100)
		{
			if (m_FirstTime.isNull())
			{
				m_FirstTime = m_CurrentTimeField.CurrTime;
				PRINT_TO_STD("FirstTime:%s", m_FirstTime.getString().c_str());
			}
			m_nReady = m_CurrentTimeField.CurrTime.getValue() - m_FirstTime.getValue();
			if (m_nReady == 5)
				PRINT_TO_STD("SystemIsReady");
		}

		if (fmod(g_nBusinessNo + 5, 100) == 0)
			PRINT_TO_STD_US("BusinessNo[%0.f]:ZoneTime:%s,%0.f", g_nBusinessNo, m_CurrentTimeField.ZoneTime.getValue(), m_CurrentTimeField.UpdateTime.getValue());
		break;
	}
	case TID_PushOrder:
	{
		COrderField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		if (field.OrderStatus == OS_PartCanceled || field.OrderStatus == OS_AllCanceled || field.OrderStatus == OS_AllTraded)
		{
			m_pMDB->m_OrderFactory->removeByPK(&field);
		}
		else
		{
			CWriteableOrder theOrder;
			COrder* pOrder = m_pMDB->m_OrderFactory->findByOrderID(field.OrderID);
			if (pOrder == NULL)
			{
				theOrder.init();
				CopyOrderEntity(&theOrder, &field);
				m_pMDB->m_OrderFactory->add(&theOrder);
			}
			else
			{
				m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);
				CopyOrderEntity(&theOrder, &field);
				m_pMDB->m_OrderFactory->update(pOrder, &theOrder);
			}
		}
		break;
	}
	case TID_SendQryOrder:
	{
		nRecvTNo = TID_RecvQryOrder;
		CQryOrderField field;
		CQryOrderField::m_Describe.getSingleField(&field, pRspBuffer);
		if (!CheckSession(m_pMDB, m_pErrorEngine, &sessionRef, field.MemberID))
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &sessionRef, &field, &(field.m_Describe));
		if (field.AccountID.isNull())
			field.AccountID = field.MemberID.getValue();

		COrderField rspField;
		COrder* pData = NULL;
		if (!field.OrderID.isNull())
		{
			COrder* pData = m_pMDB->m_OrderFactory->findByOrderID(field.OrderID);
			if (pData != NULL)
			{
				CopyOrderEntity(&rspField, pData);
				m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(COrderField::m_Describe), &sessionRef, OUTFOG_CHAIN_SINGLE);
			}
			else
			{
				m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_SINGLE);
			}
			break;
		}

		if (!field.LocalID.isNull())
		{
			COrder* pData = m_pMDB->m_OrderFactory->findByLocalID(field.AccountID, field.LocalID);
			if (pData != NULL)
			{
				CopyOrderEntity(&rspField, pData);
				m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(COrderField::m_Describe), &sessionRef, OUTFOG_CHAIN_SINGLE);
			}
			else
			{
				m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_SINGLE);
			}
			break;
		}

		if (!field.AccountID.isNull())
		{
			m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
			if (!field.InstrumentID.isNull())
			{
				COrder* pData = m_pMDB->m_OrderFactory->findFirstByInstrumentID(field.AccountID, field.InstrumentID);
				while (pData != NULL)
				{
					CopyOrderEntity(&rspField, pData);
					m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
					pData = m_pMDB->m_OrderFactory->findNextByInstrumentID();
				}
			}
			else
			{
				COrder* pData = m_pMDB->m_OrderFactory->findFirstByAccountIDPrivate(field.AccountID);
				while (pData != NULL)
				{
					CopyOrderEntity(&rspField, pData);
					m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
					pData = m_pMDB->m_OrderFactory->findNextByAccountIDPrivate();
				}
			}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
			break;
		}

		if (!CheckManagerSession(m_pMDB, m_pErrorEngine, &sessionRef))
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &sessionRef, &field, &(field.m_Describe));

		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
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
			CopyOrderEntity(&rspField, pData);
			m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pData = m_pMDB->m_OrderFactory->getNext();
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_PushAccount:
	{
		CAccountField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		CWriteableAccount theAccount;
		CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(field.MemberID, field.AccountID, field.SettlementGroup, field.Currency);
		if (pAccount == NULL)
		{
			if(field.Balance!=0.0)
			{
				theAccount.init();
				CopyAccountEntity(&theAccount, &field);
				m_pMDB->m_AccountFactory->add(&theAccount);			
			}
		}
		else
		{
			if(field.Balance==0.0)
			{
				m_pMDB->m_AccountFactory->remove(pAccount);
			}
			else
			{
				m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
				CopyAccountEntity(&theAccount, &field);
				m_pMDB->m_AccountFactory->update(pAccount, &theAccount);
			}
		}
		break;
	}
	case TID_PushPosition:
	{
		CPositionField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		if (field.Position == 0.0 && field.ShortFrozen == 0.0 && field.LongFrozen == 0.0)
		{
			m_pMDB->m_PositionFactory->removeByPK(&field);
		}
		else
		{
			CWriteablePosition thePosition;
			CPosition* pPosition = m_pMDB->m_PositionFactory->findByPositionID(field.PositionID);
			if (pPosition == NULL)
			{
				thePosition.init();
				CopyPositionEntity(&thePosition, &field);
				m_pMDB->m_PositionFactory->add(&thePosition);
			}
			else
			{
				m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
				CopyPositionEntity(&thePosition, &field);
				m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
			}
		}
		break;
	}
	case TID_SendQryAccount:
	{
		nRecvTNo = TID_RecvQryAccount;
		CQryAccountField field;
		CQryAccountField::m_Describe.getSingleField(&field, pRspBuffer);
		if (!CheckSession(m_pMDB, m_pErrorEngine, &sessionRef, field.MemberID))
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &sessionRef, &field, &(field.m_Describe));
		
		if (field.MemberID.isNull())
			field.MemberID = field.AccountID.getValue();

		if (field.AccountID.isNull())
			field.AccountID = field.MemberID.getValue();

		if (field.AccountID.isNull())
		{
			CAccountField rspField;
			m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
			CAccount* pData = m_pMDB->m_AccountFactory->findFirstByCurrency(field.SettlementGroup, field.Currency);
			while (pData != NULL)
			{
				CopyAccountEntity(&rspField, pData);
				m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
				pData = m_pMDB->m_AccountFactory->findNextByCurrency();
			}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		}
		else
		{
			CAccountField rspField;
			m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
			double PositionProfit = 0.0;
			double MaintMargin = 0.0;
			double Margin = 0.0;
			CAccountField lastAccount;
			lastAccount.CurrValue = 0.0;
			lastAccount.AccountID = field.AccountID;
			lastAccount.MemberID = field.MemberID;
			lastAccount.Currency.clear();
			CAccount* pAccount = m_pMDB->m_AccountFactory->findFirstByAccountID(field.MemberID, field.AccountID);
			while (pAccount != NULL)
			{
				if (!field.Currency.isNull() && field.Currency != pAccount->Currency)
				{
					pAccount = m_pMDB->m_AccountFactory->findNextByAccountID();
					continue;
				}
				if (!field.SettlementGroup.isNull() && field.SettlementGroup != pAccount->SettlementGroup)
				{
					pAccount = m_pMDB->m_AccountFactory->findNextByAccountID();
					continue;
				}
				CopyAccountEntity(&rspField, pAccount);
				double price = getMarkedPrice(pAccount->getCurrency());
				getRisk(m_pMDB, pAccount, PositionProfit, MaintMargin, price);
				Margin += getHairCut(pAccount, price);
				rspField.CurrValue = price * pAccount->Balance.getValue() + PositionProfit;
				lastAccount.CurrValue += rspField.CurrValue.getValue();	
				if (pAccount->Balance < 0)
				{
					CCurrency* pCurrency = pAccount->getCurrency();
					CMarginRate* pRate = getMarginRate(m_pMDB, pCurrency->MarginRateGroup.getValue(), pAccount);
					if (pRate != NULL)
					{
						double price = getMarkedPrice(pCurrency);
						double MaintMarginRatio = getLongMaintMarginRateValue((-pAccount->Balance.getValue()) * price, pRate);
						MaintMargin += (-pAccount->Balance.getValue()) * MaintMarginRatio;
					}
				}
				m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
				pAccount = m_pMDB->m_AccountFactory->findNextByAccountID();
			}
			lastAccount.Balance = Margin;
			lastAccount.MaintMargin = MaintMargin;
			lastAccount.UnRealProfit = PositionProfit;
			if (MaintMargin > 0.0)
				lastAccount.Withdraw = (Margin + PositionProfit) / MaintMargin;
			m_pServiceResponser->SendNext(&lastAccount, &(lastAccount.m_Describe), OUTFOG_CHAIN_CONTINUE);
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		}
		break;
	}
	case TID_PushCurrency:
	{
		CCurrencyField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(field.Currency, field.SettlementGroup);
		CWriteableCurrency theCurrency;
		if (pCurrency == NULL)
		{
			CopyCurrencyEntity(&theCurrency, &field);
			m_pMDB->m_CurrencyFactory->add(&theCurrency);
		}
		else
		{
			m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
			CopyCurrencyEntity(&theCurrency, &field);
			m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
		}
		break;
	}
	case TID_SendQryPosition:
	{
		nRecvTNo = TID_RecvQryPosition;
		CQryPositionField field;
		CQryPositionField::m_Describe.getSingleField(&field, pRspBuffer);
		if (!CheckSession(m_pMDB, m_pErrorEngine, &sessionRef, field.MemberID))
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &sessionRef, &field, &(field.m_Describe));
		if (field.AccountID.isNull())
			field.AccountID = field.MemberID.getValue();
		CPositionField rspField;
		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
		CPosition* pData = m_pMDB->m_PositionFactory->findFirstByAccountID(field.AccountID);
		while (pData != NULL)
		{
			if (!field.InstrumentID.isNull() && field.InstrumentID != pData->InstrumentID)
			{
				pData = m_pMDB->m_PositionFactory->findNextByAccountID();
				continue;
			}
			CopyPositionEntity(&rspField, pData);
			m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pData = m_pMDB->m_PositionFactory->findNextByAccountID();
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_PushInstrument:
	{
		CInstrumentField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if (field.SortNumber == -9)
		{
			if (pInstrument != NULL)
				m_pMDB->m_InstrumentFactory->remove(pInstrument);
			CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
			if (pMarketData != NULL)
				m_pMDB->m_MarketDataFactory->remove(pInstrument->getMarketData());
			break;
		}

		CWriteableInstrument theInstrument;
		if (pInstrument == NULL)
		{
			CopyInstrumentEntity(&theInstrument, &field);
			m_pMDB->m_InstrumentFactory->add(&theInstrument);
		}
		else
		{
			m_pMDB->m_InstrumentFactory->retrieve(pInstrument, &theInstrument);
			CopyInstrumentEntity(&theInstrument, &field);
			m_pMDB->m_InstrumentFactory->update(pInstrument, &theInstrument);
		}
		break;
	}
	default:
	{
		//CErrorField responseField;
		//responseField.ErrorNo = 999;
		//responseField.ErrorMsg = "UnknownCommand";
		//m_pServiceResponser->SendResponse(0, &responseField, &(responseField.m_Describe),
		//	pOutFogRspRef->FrontNo, pOutFogRspRef->SessionNo, pOutFogRspRef->RequestNo, OUTFOG_CHAIN_SINGLE, 999, "UnknownCommand");
		//return true;
	}
	}

	//直接转发
	//m_pServiceApi->ReqField(&m_OutFogMetaRef, pRspBuffer);
	return true;
}

int CApp::SubTopicAction(CTopicActionField* pField, TSessionRef* pSession)
{
	int nTopicNo = atoi(pField->TopicID.getValue());
	if (nTopicNo == 0)
	{
		if (!strcmp(pField->TopicID.getValue(), "Topic_Broad"))
			nTopicNo = OF_Topic_Market_Broad;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_Order"))
			nTopicNo = OF_Topic_Private_Order;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_SnapShot"))
			nTopicNo = OF_Topic_Private_SnapShot;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_Trade"))
			nTopicNo = OF_Topic_Private_Trade;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_UserSession"))
			nTopicNo = OF_Topic_Private_UserSession;
	}
	pField->TopicID = nTopicNo;
	int nDepth = 0;
	int nReaderTimes = 1;
	vector<string> vFilters = Txtsplit(pField->Index.getValue(), "_");
	switch (nTopicNo)
	{
	case OF_Topic_Market_Broad:
	{
		if (pField->ResumeNo == 0)
			RspInstrument(m_pMDB, m_pServiceResponser, pSession);
		break;
	}
	case OF_Topic_Private_Order:
	case OF_Topic_Private_Trade:
	case OF_Topic_Private_TriggerOrder:
	case OF_Topic_Private_SnapShot:
	case OF_Topic_Private_UserSession:
	{
		if (pField->Token == m_sGateWayKey.c_str())
		{
			if (nTopicNo == OF_Topic_Private_UserSession)
				m_pLoginService->RspUserSession(m_pServiceResponser, pSession);
			break;
		}

		if ((sessionRef.pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && sessionRef.pToken[1] == '\0'))
		{
			if (sessionRef.pUserID[0] != '\0')
			{
				pField->Index = sessionRef.pUserID;
				pField->ResumeNo = -1;
				break;
			}
		}

		if (pField->Token.isNull())
			pField->Token = sessionRef.pToken;

		CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pField->Token);
		if (pUserSession == NULL)
			return m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);
		if (pUserSession->ExpireTime < m_CurrentTimeField.CurrTime)
			return m_pErrorEngine->reportError(ERROR_APIKEY_EXPIRED);
		if (pUserSession->UserType != UT_SuperManager)
			pField->ResumeNo = -1;

		if (pField->Index.isNull())
		{
			if (pUserSession->UserType != UT_SuperManager)
				pField->Index = pUserSession->MemberID;
		}
		else
		{
			//考虑是否需要给初始数据的情况，让订阅者有选择的权利
			//同时此类订阅非0值没有意义
			if (pUserSession->MemberID != pField->Index)
			{
				if (!(pUserSession->UserType == UT_SuperManager || pUserSession->UserType == UT_Observer))
					return m_pErrorEngine->reportError(ERROR_ONLY_TRADE_MANAGER_CAN_DO_THIS);
			}
		}
		break;
	}
	default:
	{
		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "TopicIDNotFound");
	}
	}
	return nReaderTimes;
}


void CApp::SubTopics(CLinkField linkField)
{
	CTopicActionField field;
	m_OutFogMetaRef.FieldNo = field.m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = field.m_Describe.m_nStreamSize;

	field.Action = SA_Sub;
	field.ResumeNo = -1;
	field.Index = "";
	field.Token = m_sGateWayKey.c_str();

	m_OutFogMetaRef.GlobalNo = linkField.LinkNo;
	m_OutFogMetaRef.MessageNo = TID_SendTopicAction;

	field.ResumeNo = 0;
	field.TopicID = OF_Topic_Market_Broad;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
	field.ResumeNo = -1;

	field.TopicID = OF_Topic_Market_OverView;
	field.ResumeNo = 0;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
	field.ResumeNo = -1;

	field.TopicID = OF_Topic_Private_Order;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

	field.TopicID = OF_Topic_Private_SnapShot;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

	field.TopicID = OF_Topic_Private_Trade;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

	field.TopicID = OF_Topic_Private_TriggerOrder;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

	field.TopicID = OF_Topic_Private_UserSession;
	field.ResumeNo = 0;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
	field.ResumeNo = -1;

	//field.TopicID = "3";
	//field.Index = "";
	//m_OutFogMetaRef.ServiceNo = linkField.LinkNo;
	//m_OutFogMetaRef.TNo = TID_SendTopicAction;
	//CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	//m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

	//开放push流水
	m_OutFogMetaRef.Type = OUTFOG_TYPE_SUBTOPIC;
	m_pServiceApi->ReqField(&m_OutFogMetaRef, NULL);
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
}

int main(int argc, char* argv[])
{
#ifdef UNIX
	signal(SIGABRT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, SIG_IGN);
#endif

	showVersion(argc, argv);

	if ((argc == 2) && (!strcmp(argv[1], "-h")))
		CmdLineArgError();

	if (argc > 3)
		CmdLineArgError();

	string sKernelid = "0";
	int nKernelid = 0;
	if (argc > 1)
	{
		if (isValidNumber(argv[1]))
		{
			sKernelid = argv[1];
			nKernelid = atoi(argv[1]);
		}
	}

	TIniFile NetWorkIni;
	NetWorkIni.Open(NETWORK_FILE_NAME);

	char sLink[32];
	char slinks[256];
	sprintf(sLink, "Private_%s_Links", sKernelid.c_str());
	NetWorkIni.ReadString(sLink, "Links", "", slinks, sizeof(slinks) - 1);
	if (slinks[0] == '\0')
	{
		sKernelid = "*";
		sprintf(sLink, "Private_%s_Links", sKernelid.c_str());
		NetWorkIni.ReadString(sLink, "Links", "", slinks, sizeof(slinks) - 1);
	}

	CApp* pApp = new CApp();
	COutFogService* m_pService = new COutFogService();
	COutFogServiceApi* pOutFogServiceApi = m_pService->LoadOutFogApi("omqsapi", pApp, nKernelid);
	pApp->setServiceApi(pOutFogServiceApi);

	TOutFogMetaRef OutFogMetaRef;
	OutFogMetaRef.Type = OUTFOG_TYPE_START;
	pOutFogServiceApi->ReqField(&OutFogMetaRef, NULL);


	CLinkField link;
	char StreamBuf[4096];

	OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
	OutFogMetaRef.Type = OUTFOG_TYPE_INITLINK;
	strncpy(OutFogMetaRef.SubIndexName, link.LinkDllName.getValue(), sizeof(OutFogMetaRef.SubIndexName));
	OutFogMetaRef.GlobalNo = link.LinkNo.getValue();
	OutFogMetaRef.MessageNo = TID_SendInitLink;
	OutFogMetaRef.FieldNo = link.m_Describe.m_FieldID;
	OutFogMetaRef.Length = link.m_Describe.m_nStreamSize;
	OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;

	vector<string> SLinks = Txtsplit(slinks, ",");
	for (int i = 0; i < SLinks.size(); i++)
	{
		link.LinkNo = atoi(SLinks[i].c_str());
		sprintf(sLink, "Private_%s_Link%d", sKernelid.c_str(), link.LinkNo.getValue());
		NetWorkIni.ReadString(sLink, "LinkAddress", "", link.LinkAddress.getBuffer(), sizeof(link.LinkAddress) - 1);
		NetWorkIni.ReadString(sLink, "LinkDllName", "", link.LinkDllName.getBuffer(), sizeof(link.LinkDllName) - 1);
		NetWorkIni.ReadString(sLink, "LinkDllLoad", "", link.LinkDllLoad.getBuffer(), sizeof(link.LinkDllLoad) - 1);
		NetWorkIni.ReadString(sLink, "Config", "", link.Config.getBuffer(), sizeof(link.Config) - 1);

		CLinkField::m_Describe.setSingleField(StreamBuf, &link);

		strncpy(OutFogMetaRef.SubIndexName, link.LinkDllName.getValue(), sizeof(OutFogMetaRef.SubIndexName));
		OutFogMetaRef.GlobalNo = link.LinkNo.getValue();
		pOutFogServiceApi->ReqField(&OutFogMetaRef, StreamBuf);
		pApp->m_LinkFrom.insert(CLinkMap::value_type(link.LinkNo.getValue(), link));
	}

	while (true)
	{
		SLEEP_SECONDS(10);
	}

	return 0;
}

