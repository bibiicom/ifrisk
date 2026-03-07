#include "public.h"
#include "LoginService.h"
#include "errorDefine.h"
#include "utility.h"
#include "tradingTools.h"
#include "cryptAll.h"
#include "CopyField.h"
#include "FieldData.h"
#include "BaseFunction.h"
#include "printinfo.h"
#include "ServiceShare.h"
#include "Config.h"

#define FRONT_TIME_OUT	60

CLoginService::CLoginService(CMDB* pMDB, CErrorEngine* pErrorEngine)
{
	m_pMDB = pMDB;
	m_pErrorEngine = pErrorEngine;
	m_sGateWayKey = GetConfigString(NetworkConfigFile, "GateWayKey");
	if (m_sGateWayKey == "")
		m_sGateWayKey = "kdfhaurksh*uerhgsgjkjesbhDuu#@kdhf";
}

CLoginService::~CLoginService()
{
}

bool CLoginService::UserLogin(CServiceResponser* m_pServiceResponser, CUserSessionField* pLoginField, TSessionRef* pSession, CTransaction* pTransaction)
{
	if (pLoginField->Token.isNull())
	{
		///检查用户身份
		//CUser* pUser = m_pMDB->m_UserFactory->findByUserID(pLoginField->UserID);
		//if (pUser == NULL)
		//	return m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);

		//if (!pUser->IsActive)
		//	return m_pErrorEngine->reportError(ERROR_USER_NOT_ACTIVE);

		////	if (!(pSession->pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && pSession->pToken[1] == '\0'))
		////	{
		//switch (pUser->EncryptType)
		//{
		//case ET_MD5:
		//{
		//	char md5Password[33];
		//	MD5Hex(md5Password, pLoginField->UserID, pLoginField->Password);
		//	if (pUser->Password != md5Password)
		//		return m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);
		//	break;
		//}
		//case ET_None:
		//default:
		//{
		//	if (pUser->Password != pLoginField->Password)
		//		return m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);
		//	break;
		//}
		//}
		//	}

		if (pLoginField->Password.isNull())
			return m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);

		int nSessionCount = 0;
		CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(pLoginField->UserID, "");

		if (pUserSession == NULL)
			return m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);

		if (!pUserSession->ExpireTime.isNull())
		{
			if (pUserSession->ExpireTime < g_nCurrentTime)
			{
				CUserSession* pUserSessionRemove = pUserSession;
				m_pMDB->m_UserSessionFactory->remove(pUserSessionRemove);
				return m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);
			}
		}

		if (pUserSession->Password != pLoginField->Password)
			return m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);

		CWriteableUserSession UserSession;
		if (pLoginField->APPID.isNull())
		{
			m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &UserSession);
			UserSession.FrontNo = pSession->frontID;
			UserSession.SessionNo = pSession->sessionID;
			UserSession.LoginTime = g_nCurrentTime;
			UserSession.MaxLocalID.clear();
			UserSession.LoginTime = g_nCurrentTime;
			char sToken[33];
			MD5Hex(sToken, UserSession.UserID.getValue(), UserSession.LoginTime.getString().c_str(), UserSession.APPID);
			UserSession.Token = sToken;
			CopyUserSessionEntity(pLoginField, &UserSession);
			m_pMDB->m_UserSessionFactory->update(pUserSession, &UserSession, pTransaction);
		}
		else
		{
			CUserSession* pUserSessionAdd = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(pLoginField->UserID, pLoginField->APPID);
			if (pUserSessionAdd == NULL)
			{
				UserSession.init();
				//UserSession.LoginTime = getMDBTime(m_pMDB);
				UserSession.LoginTime = g_nCurrentTime;
				//if (pUser->UserType == UT_SuperManager)
				//	UserSession.ExpireTime.clear();//一直有效(2038-01-19 11:14:07)
				//else
				UserSession.ExpireTime = UserSession.LoginTime + g_nSessionExpireTime;
				UserSession.UserID = pLoginField->UserID;
				UserSession.APPID = pLoginField->APPID;
				UserSession.UserProductID = pLoginField->UserProductID;
				UserSession.FrontNo = pSession->frontID;
				UserSession.SessionNo = pSession->sessionID;
				UserSession.AccessLimit = g_nDefaultAccessLimit;

				char sToken[33];
				MD5Hex(sToken, UserSession.UserID.getValue(), UserSession.LoginTime.getString().c_str(), UserSession.APPID);
				UserSession.Token = sToken;
				///向UserSession表中增加或修改记录recUserSession
				UserSession.MemberID = pUserSession->MemberID;
				UserSession.UserType = pUserSession->UserType;
				m_pMDB->m_UserSessionFactory->add(&UserSession, pTransaction);
			}
			else
			{
				//通知上一个UserSession登出
				CUserSessionField theUserSessionField;
				CopyUserSessionEntity(&theUserSessionField, pUserSessionAdd);
				TpSession delSession;
				delSession.frontID = pUserSessionAdd->FrontNo.getValue();
				delSession.sessionID = pUserSessionAdd->SessionNo.getValue();
				theUserSessionField.ExpireTime = g_nCurrentTime;
				m_pServiceResponser->SendResponse(TID_RecvUserLogout, &theUserSessionField, &(theUserSessionField.m_Describe), &delSession, OUTFOG_CHAIN_SINGLE, ERROR_USER_RELOGIN, "UserReLogin:KickedOut");
				m_pMDB->m_UserSessionFactory->retrieve(pUserSessionAdd, &UserSession);
				UserSession.FrontNo = pSession->frontID;
				UserSession.SessionNo = pSession->sessionID;
				UserSession.LoginTime = g_nCurrentTime;
				UserSession.MaxLocalID.clear();
				UserSession.SinkType = ST_SinkTillRedis;
				m_pMDB->m_UserSessionFactory->update(pUserSessionAdd, &UserSession, pTransaction);
			}
			CopyUserSessionEntity(pLoginField, &UserSession);
		}
	}
	else
	{
		CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pLoginField->Token);
		if (pUserSession == NULL)
			return m_pErrorEngine->reportError(ERROR_USER_NO_PERMISSION);
		CWriteableUserSession UserSession;
		m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &UserSession);
		UserSession.FrontNo = pSession->frontID;
		UserSession.SessionNo = pSession->sessionID;
		UserSession.MaxLocalID.clear();
		m_pMDB->m_UserSessionFactory->update(pUserSession, &UserSession, pTransaction);
		CopyUserSessionEntity(pLoginField, pUserSession);
	}

	//if (pOldUserSession == NULL)
	//{
	//	m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);
	//	return false;
	//}

	///检查用户IP地址
	//bool bValidIpFlag = true;
	//CUserAllowIteratorByUserIDAccessType itorUserIP(m_pMDB->m_UserAllowFactory, pLoginField->UserID, AT_IPAddress);
	//CUserAllow *pUserIP = NULL;
	//while ((pUserIP = itorUserIP.next()) != NULL)
	//{
	//	vector<string> ipmask = Txtsplit(pUserIP->AccessName.getValue(), "/");
	//	if (ipmask.size() != 2)
	//	{
	//		continue;
	//	}
	//	if (CheckIpAddressMatch(pUserSession->PublicIPAddress, ipmask[0].c_str(), ipmask[1].c_str()))
	//	{
	//		if (pUserIP->AccessValue == "1")
	//		{
	//			bValidIpFlag = true;
	//			break;
	//		}
	//		else
	//		{
	//			bValidIpFlag = false;
	//		}
	//	}
	//}

	//if (!bValidIpFlag)
	//{
	//	m_pErrorEngine->reportError(ERROR_INVALID_LOGIN_IPADDRESS);
	//	return false;
	//}

	///检查是否已登录
//	pUserSession = m_pMDB->m_UserSessionFactory->findByUserID(UserID);
//	if (pUserSession != NULL)
//	{
//		if (m_loginTimeFirst && (pUserSession->IPAddress != IPAddress))
//		{
//			///不同IP地址来源的相同用户，应当认为是重复登录
//			REPORT_ERROR(ERROR_USER_RELOGIN);
//		}
//		else
//		{
//			///相同IP地址的重复登录，需要把原来的会员踢出去
//			m_pMDB->m_UserSessionFactory->remove(pUserSession,pTransaction);
//			pUserSession=NULL;
//		}
//	}

	return true;
}

//bool CLoginService::AddSession(CReadOnlyIPAddressType &IPAddress, TSessionRef* pSession, CTransaction *pTransaction)
//{
//	CUserSession *pUserSession;
//
//	pUserSession = m_pMDB->m_UserSessionFactory->findBySessionNo(pSession->frontID,pSession->sessionID, "");
//	if (pUserSession != NULL)
//	{
//		m_pErrorEngine->reportError(ERROR_DUPLICATE_SESSION);
//		return false;
//	}
//	CWriteableUserSession UserSession;
//	UserSession.init();
//	UserSession.FrontNo = pSession->frontID;
//	UserSession.SessionNo = pSession->sessionID;
//	UserSession.PublicIPAddress = IPAddress;
//	m_pMDB->m_UserSessionFactory->add(&UserSession, pTransaction);
//
//	return true;
//}

bool CLoginService::UserLogout(CUserLogoutField* pLogout, TSessionRef* pSession, CTransaction* pTransaction)
{
	if (pLogout->Token.isNull())
		pLogout->Token = pSession->pToken;

	CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pLogout->Token);
	if (pUserSession == NULL)
		return m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);

	pLogout->UserID = pUserSession->UserID;
	if (!pUserSession->Password.isNull())
	{
		CWriteableUserSession UserSession;
		m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &UserSession);
		UserSession.FrontNo.clear();
		UserSession.SessionNo.clear();
		char sToken[33];
		MD5Hex(sToken, UserSession.UserID.getValue(), UserSession.LoginTime.getString().c_str(), UserSession.APPID);
		UserSession.Token = sToken;
		m_pMDB->m_UserSessionFactory->remove(pUserSession, pTransaction);
		m_pMDB->m_UserSessionFactory->add(&UserSession, pTransaction);
	}
	else
	{
		m_pMDB->m_UserSessionFactory->remove(pUserSession, pTransaction);
	}
	return true;
}

//void CLoginService::ForceLogout(CReadOnlyNumberType &FrontNo, CTransaction *pTransaction)
//{
//	///找到该前置上的所有会话
//	CUserSessionIteratorByFrontNo it(m_pMDB->m_UserSessionFactory, FrontNo);
//	CUserSession *pUserSession = NULL;
//	while ((pUserSession = it.next()) != NULL)
//	{
//		///对于已经登录的会话，全部强制退出
//		//多唯一索引，不能修改，只能删除
//		it.DeleteCurrent(pTransaction);
//	}
//}

//bool CLoginService::UserPasswordUpdate(CLongIndexIDType& UserID,
//	CReadOnlyPasswordType& OldPassword,
//	CReadOnlyPasswordType& NewPassword,
//	TSessionRef* pSession, CTransaction* pTransaction)
//{
//	///一、合法性检查
//	///检查会话;
//	CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findBySessionNo(pSession->frontID, pSession->sessionID);
//	if (pUserSession == NULL)
//		return m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);
//
//	if (pUserSession->UserID != UserID)
//		return m_pErrorEngine->reportError(ERROR_USER_MISMATCH);
//
//	CUser* pUser = m_pMDB->m_UserFactory->findByUserID(UserID);
//	if (pUser == NULL)
//		return m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);
//
//	switch (pUser->EncryptType)
//	{
//	case ET_MD5:
//	{
//		char md5OldPassword[33];
//		MD5Hex(md5OldPassword, UserID, OldPassword);
//		if (pUser->Password != md5OldPassword)
//			return m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);
//		break;
//	}
//	case ET_None:
//	default:
//	{
//		if (pUser->Password != OldPassword)
//			return m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);
//		break;
//	}
//	}
//
//	///二、修改密码
//	CWriteableUser User;
//	m_pMDB->m_UserFactory->retrieve(pUser, &User);
//
//	switch (pUser->EncryptType)
//	{
//	case ET_MD5:
//	{
//		char md5Password[33];
//		MD5Hex(md5Password, UserID, NewPassword);
//		User.Password = md5Password;
//		break;
//	}
//	case ET_None:
//	default:
//	{
//		User.Password = NewPassword;
//		break;
//	}
//	}
//	m_pMDB->m_UserFactory->update(pUser, &User, pTransaction, false);
//	return true;
//}

int CLoginService::CheckTopic(CServiceResponser* m_pServiceResponser, CTopicActionField* pField, TSessionRef* pSession)
{
	int nTopicNo = atoi(pField->TopicID.getValue());
	if (nTopicNo == 0)
	{
		if (!strcmp(pField->TopicID.getValue(), "Topic_Market_Broad"))
			nTopicNo = OF_Topic_Market_Broad;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_Order"))
			nTopicNo = OF_Topic_Private_Order;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_Depth_All"))
			nTopicNo = OF_Topic_Market_Depth_All;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_LastTrade"))
			nTopicNo = OF_Topic_Market_LastTrade;
		//else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_KLine"))
		//	nTopicNo = OF_Topic_Market_KLine;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_LastKLine"))
			nTopicNo = OF_Topic_Market_LastKLine;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_SnapShot"))
			nTopicNo = OF_Topic_Private_SnapShot;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_Risk"))
			nTopicNo = OF_Topic_Private_Risk;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_OverView"))
			nTopicNo = OF_Topic_Market_OverView;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_Trade"))
			nTopicNo = OF_Topic_Private_Trade;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_TriggerOrder"))
			nTopicNo = OF_Topic_Private_TriggerOrder;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_UserSession"))
			nTopicNo = OF_Topic_Private_UserSession;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_Copy_Finish"))
			nTopicNo = OF_Topic_Private_Copy_Finish;
	}
	pField->TopicID = nTopicNo;
	int nReaderTimes = 1;
	//找到交易所和合约号
	vector<string> vFilters = Txtsplit(pField->Index.getValue(), "_");

	nTopicNo = nTopicNo % 10000;

	switch (nTopicNo)
	{
	case OF_Topic_Market_Broad:
	case OF_Topic_Market_LastTrade:
	{
		break;
	}
	case OF_Topic_Market_OverView:
	{
		pField->ResumeNo = -1;
		switch (vFilters.size())
		{
		case 0:
		case 1:
		case 2://不降频
			break;
		case 3://降频
			nReaderTimes = atoi(vFilters[2].c_str());
			pField->Index = vFilters[0].c_str();
			pField->Index += "_";
			pField->Index += vFilters[1].c_str();
			break;
		default:
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
		}
		break;
	}
	case OF_Topic_Private_Order:
	case OF_Topic_Private_Trade:
	case OF_Topic_Private_TriggerOrder:
	case OF_Topic_Private_Risk:
	case OF_Topic_Private_SnapShot:
	case OF_Topic_Private_FinishOrder:
	case OF_Topic_Private_UserSession:
	case OF_Topic_Private_Copy_Finish:
	{
		if (pField->Token.isNull())
			pField->Token = pSession->pToken;

		if (!(pField->Token.isNull() || pField->Token == m_sGateWayKey.c_str()))
		{
			if ((pField->Token[0] == OUTFOG_REMARK_TOKEN_MANAGER && pField->Token[1] == '\0'))
			{
				pField->Index = pSession->pUserID;
			}
			else
			{
				CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pField->Token);
				if (pUserSession == NULL)
					return m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);

				if (!(pUserSession->UserType == UT_SuperManager || pUserSession->UserType == UT_Observer))
					pField->Index = pUserSession->MemberID;
			}
		}
		break;
	}
	case OF_Topic_Market_Depth_25:
	case OF_Topic_Market_Depth_All:
	{
		break;
	}
	case OF_Topic_Market_LastKLine:
	case OF_Topic_Market_KLine:
	{
		break;
	}
	default:
	{
		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "TopicIDNotFound");
	}
	}
	return nReaderTimes;
}


int CLoginService::SubTopic(CServiceResponser* m_pServiceResponser, CTopicActionField* pField, TSessionRef* pSession)
{
	m_pServiceResponser->GetMetaRef()->Reserve[0] = pField->Zip.getValue() + '0';
	int nTopicNo = atoi(pField->TopicID.getValue());
	if (nTopicNo == 0)
	{
		if (!strcmp(pField->TopicID.getValue(), "Topic_Market_Broad"))
			nTopicNo = OF_Topic_Market_Broad;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_Order"))
			nTopicNo = OF_Topic_Private_Order;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_Depth_All"))
			nTopicNo = OF_Topic_Market_Depth_All;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_LastTrade"))
			nTopicNo = OF_Topic_Market_LastTrade;
		//else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_KLine"))
		//	nTopicNo = OF_Topic_Market_KLine;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_LastKLine"))
			nTopicNo = OF_Topic_Market_LastKLine;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_SnapShot"))
			nTopicNo = OF_Topic_Private_SnapShot;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_Risk"))
			nTopicNo = OF_Topic_Private_Risk;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_OverView"))
			nTopicNo = OF_Topic_Market_OverView;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_Trade"))
			nTopicNo = OF_Topic_Private_Trade;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_TriggerOrder"))
			nTopicNo = OF_Topic_Private_TriggerOrder;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_UserSession"))
			nTopicNo = OF_Topic_Private_UserSession;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Private_Copy_Finish"))
			nTopicNo = OF_Topic_Private_Copy_Finish;
	}
	pField->TopicID = nTopicNo;
	int nReaderTimes = 1;
	//找到交易所和合约号
	vector<string> vFilters = Txtsplit(pField->Index.getValue(), "_");

	nTopicNo = nTopicNo % 10000;

	switch (nTopicNo)
	{
	case OF_Topic_Market_Broad:
	{
		RspInstrument(m_pMDB, m_pServiceResponser, pSession);
		RspCurrency(m_pMDB, m_pServiceResponser, pSession);
		break;
	}
	case OF_Topic_Market_LastTrade:
	{
		break;
	}
	case OF_Topic_Market_OverView:
	{
		pField->ResumeNo = -1;
		switch (vFilters.size())
		{
		case 0:
			//为了行情能够级联，这里全部可以订阅
			//CUserSession *pUserSession = m_pMDB->m_UserSessionFactory->findBySessionNo(pSession->frontID, pSession->sessionID);
			//if (pUserSession == NULL)
			//	return m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);

			//if (pUserSession->UserType != UT_SuperManager)
			//	return m_pErrorEngine->reportError(ERROR_ONLY_TRADE_MANAGER_CAN_DO_THIS);
			RspMarketData(m_pMDB, m_pServiceResponser, pSession, OF_Topic_Market_OverView);
			break;
		case 1://Spot这种，不能降频
			RspMarketData(m_pMDB, m_pServiceResponser, pField->Index, pSession, OF_Topic_Market_OverView);
			break;
		case 2://不降频
			break;
		case 3://降频
			nReaderTimes = atoi(vFilters[2].c_str());
			pField->Index = vFilters[0].c_str();
			pField->Index += "_";
			pField->Index += vFilters[1].c_str();
			break;
		default:
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
		}
		break;
	}
	case OF_Topic_Private_Order:
	case OF_Topic_Private_Trade:
	case OF_Topic_Private_TriggerOrder:
	case OF_Topic_Private_Risk:
	case OF_Topic_Private_SnapShot:
	case OF_Topic_Private_FinishOrder:
	case OF_Topic_Private_UserSession:
	case OF_Topic_Private_Copy_Finish:
	{
		if (pField->Token.isNull())
			pField->Token = pSession->pToken;

		if (!(pField->Token.isNull() || pField->Token == m_sGateWayKey.c_str()))
		{
			if ((pField->Token[0] == OUTFOG_REMARK_TOKEN_MANAGER && pField->Token[1] == '\0'))
			{
				pField->Index = pSession->pUserID;
			}
			else
			{
				CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pField->Token);
				if (pUserSession == NULL)
					return m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);

				if (!(pUserSession->UserType == UT_SuperManager || pUserSession->UserType == UT_Observer))
					pField->Index = pUserSession->MemberID;
			}
		}

		//if (pField->ResumeNo == 0.0 && pField->Index.isNull())
		if (pField->Index.isNull())
		{
			switch (nTopicNo)
			{
			case OF_Topic_Private_Risk:
			{
				RspRisk(m_pServiceResponser, pSession);
				break;
			}
			case OF_Topic_Private_SnapShot:
			{
				RspSnap(m_pServiceResponser, pSession);
				break;
			}
			case OF_Topic_Private_Order:
			{
				RspOrder(m_pServiceResponser, pSession);
				break;
			}
			case OF_Topic_Private_TriggerOrder:
			{
				RspTriggerOrder(m_pServiceResponser, pSession);
				break;
			}
			case OF_Topic_Private_UserSession:
			{
				RspUserSession(m_pServiceResponser, pSession);
				break;
			}
			case OF_Topic_Private_Copy_Finish:
			{
				RspCopy(m_pServiceResponser, pSession);
				break;
			}
			}				
		}

		//CUserSession *pUserSession = m_pMDB->m_UserSessionFactory->findBySessionNo(pSession->frontID, pSession->sessionID);
		//if (pUserSession == NULL)
		//	pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pField->Token);

		//if (pUserSession == NULL)
		//{
		//	if (pSession->pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && pSession->pToken[1] == '\0')
		//	{
		//		if (pField->Index.isNull())
		//			pField->Index = pSession->pUserID;
		//		if (nTopicNo == OF_Topic_Private_Risk)
		//			RspRisk(m_pServiceResponser, pSession);
		//		else if (nTopicNo == OF_Topic_Private_TriggerOrder)
		//			RspTriggerOrder(m_pServiceResponser, pSession);
		//		else if (nTopicNo == OF_Topic_Private_UserSession)
		//			RspUserSession(m_pServiceResponser, pSession);
		//	}
		//	else
		//	{
		//		if (pField->Token != m_sGateWayKey.c_str())
		//			return m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);
		//		pField->Index = "";
		//		if (nTopicNo == OF_Topic_Private_Risk)
		//			RspRisk(m_pServiceResponser, pSession);
		//		else if (nTopicNo == OF_Topic_Private_TriggerOrder)
		//			RspTriggerOrder(m_pServiceResponser, pSession);
		//		else if (nTopicNo == OF_Topic_Private_UserSession)
		//			RspUserSession(m_pServiceResponser, pSession);
		//	}
		//}
		//else
		//{
		//	if (pField->Index == "")
		//	{
		//		if (pUserSession->UserType != UT_SuperManager)
		//			return m_pErrorEngine->reportError(ERROR_ONLY_TRADE_MANAGER_CAN_DO_THIS);

		//		if (nTopicNo == OF_Topic_Private_Risk)
		//			RspRisk(m_pServiceResponser, pSession);
		//		else if (nTopicNo == OF_Topic_Private_TriggerOrder)
		//			RspTriggerOrder(m_pServiceResponser, pSession);
		//	}
		//	else
		//	{
		//		//考虑是否需要给初始数据的情况，让订阅者有选择的权利
		//		//同时此类订阅非0值没有意义
		//		if (pUserSession->MemberID == pField->Index)
		//		{
		//			if (pField->ResumeNo == 0)
		//				RspPrivate(pUserSession->MemberID, m_pServiceResponser, pSession);
		//		}
		//		else
		//		{
		//			if (!(pUserSession->UserType == UT_SuperManager || pUserSession->UserType == UT_Observer))
		//				return m_pErrorEngine->reportError(ERROR_ONLY_TRADE_MANAGER_CAN_DO_THIS);
		//		}
		//	}
		//}
		break;
	}
	//case OF_Topic_Market_Depth_5:
	//case OF_Topic_Market_Depth_10:
	//{
	//	if (vFilters.size() == 3)
	//	{
	//		nReaderTimes = atoi(vFilters[2].c_str());
	//		pField->Index = vFilters[0].c_str();
	//		pField->Index += "_";
	//		pField->Index += vFilters[1].c_str();
	//	}
	//	pField->ResumeNo = -1;
	//	break;
	//}
	//case OF_Topic_Market_Depth_5_TickUp:
	//{
	//	if (vFilters.size() == 4)
	//	{
	//		nReaderTimes = atoi(vFilters[3].c_str());
	//		pField->Index = vFilters[0].c_str();
	//		pField->Index += "_";
	//		pField->Index += vFilters[1].c_str();
	//		pField->Index += "_";
	//		pField->Index += vFilters[2].c_str();
	//	}
	//	pField->ResumeNo = -1;
	//	break;
	//}
	//case OF_Topic_Market_Depth_25:
	//{
	//	if (vFilters.size() != 2)
	//		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Filters.size!=2");
	//	RspMarketOrder(m_pMDB, m_pServiceResponser, vFilters[0].c_str(), vFilters[1].c_str(), 25, pSession);
	//	break;
	//}
	case OF_Topic_Market_Depth_25:
	case OF_Topic_Market_Depth_All:
	{
		//trade不支持安装合约订阅行情，这里需要规避掉，防止core
		//if (vFilters.size() == 0)	//全量订阅，对Index没有要求，全部按照没有填来
		//	RspMarketOrder(m_pMDB, m_pServiceResponser, pSession);
		//else
		//	RspMarketOrder(m_pMDB, m_pServiceResponser, vFilters[0].c_str(), vFilters[1].c_str(), INT_MAX, pSession);
		//if (vFilters.size() > 2)
		//{
		//	pField->Index = vFilters[0].c_str();
		//	pField->Index += "_";
		//	pField->Index += vFilters[1].c_str();
		//}

		RspMarketOrder(m_pMDB, m_pServiceResponser, pSession);
		break;
	}
	case OF_Topic_Market_LastKLine:
	{
		if (vFilters.size() == 3)
		{
			CCurrentTime* pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
			RspLastKLine(m_pServiceResponser, vFilters[0].c_str(), vFilters[1].c_str(), vFilters[2].c_str(), pSession);
			pField->Index = vFilters[0].c_str();
			pField->Index += "_";
			pField->Index += vFilters[1].c_str();
			pField->Index += "_1m";
			pField->ResumeNo = -1;
		}
		else
		{
			if (pField->Index.isNull())
				RspLastKLine(m_pServiceResponser, pSession);
			pField->Index.clear();
			pField->ResumeNo = -1;
		}
		break;
	}
	case OF_Topic_Market_KLine:
	{
		//这里能订阅的目录是为了marketdata能补充最近没有来得及从数据库拿到的K线数据
		break;
	}
	//case OF_Topic_Market_KLine:
	//{
	//	if (vFilters.size() != 3)
	//		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Filters.size!=3");

	//	//		RspLastKLine(m_pServiceResponser, vFilters[0].c_str(), vFilters[1].c_str(), vFilters[2].c_str(), -pField->ResumeNo.getValue(), pSession);
	//	pField->Index = vFilters[0].c_str();
	//	pField->Index += "_";
	//	pField->Index += vFilters[1].c_str();
	//	m_pServiceResponser->SubSubjectID(pField->LocalNo, OF_Topic_Market_LastKLine, 0, pField->Index, pSession->frontID, pSession->sessionID);
	//	break;
	//}
	default:
	{
		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "TopicIDNotFound");
	}
	}
	return nReaderTimes;
}

bool CLoginService::RspLastKLine(CServiceResponser* m_pServiceResponser, const char* pExchangeID, const char* pInstrumentID, const char* pBar, TSessionRef* pSession)
{
	//CWriteableKLine theKLine;
	//theKLine.init();
	//CExchangeIDType ExchangeID = pExchangeID;
	//CInstrumentIDType InstrumentID = pInstrumentID;

	//if (getLastKLineByBeginTime(m_pMDB, &theKLine, ExchangeID, InstrumentID, pPeriod))
	//{
	CLastKLine* pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pExchangeID, pInstrumentID, pBar);
	if (pLastKLine != NULL)
	{
		CLastKLineField field;
		CopyLastKLineEntity(&field, pLastKLine);
		m_pServiceResponser->SendResponse(TID_PushKLine, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
	}
	//}
	return true;
}

bool CLoginService::RspLastKLine(CServiceResponser* m_pServiceResponser, TSessionRef* pSession)
{
	m_pServiceResponser->PrepareResponse(TID_PushKLine, 0, "Success", pSession);
	CLastKLine* pLastKLine = m_pMDB->m_LastKLineFactory->getFirst();
	while (pLastKLine != NULL)
	{
		CLastKLineField field;
		CopyLastKLineEntity(&field, pLastKLine);
		m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		pLastKLine = m_pMDB->m_LastKLineFactory->getNext();
	}
	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	return true;
}

//bool CLoginService::RspLastKLine(CServiceResponser *m_pServiceResponser, const char * pExchangeID, const char * pInstrumentID, const char * pPeriod, UF_INT8 nNumber, TSessionRef* pSession)
//{
//	CExchangeIDType ExchangeID = pExchangeID;
//	CInstrumentIDType InstrumentID = pInstrumentID;
//	//先取到历史的K线，内存中暂时存放的
//	m_pServiceResponser->PrepareResponse(TID_PushKLine, 0, "Success", pSession);
//
//	list<CKLineField> thePushKLines;//记录要发送的K线，按照由旧到新的顺序发送
//	CSecondsType theLastBeginTime = 0;//记录最新K线的BeginTime
//	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriodBeginTime(ExchangeID, InstrumentID, pPeriod);
//	int i = 1;
//	while (pLastKLine != NULL && i < nNumber)
//	{
//		CKLineField field;
//		CopyKLineEntity(&field, pLastKLine);
//		thePushKLines.push_front(field);
//		if (field.BeginTime > theLastBeginTime)
//		{
//			theLastBeginTime = field.BeginTime;
//		}
//		pLastKLine = m_pMDB->m_LastKLineFactory->findNextByHisPeriodBeginTime();
//		i++;
//	}
//	m_pMDB->m_LastKLineFactory->endFindByHisPeriodBeginTime();
//
//	//获取最后一根K线，最新的（如果是基础K线，有可能重复）
//	CWriteableKLine theKLine;
//	theKLine.init();
//	if (getLastKLineByBeginTime(m_pMDB, &theKLine, ExchangeID, InstrumentID, pPeriod))
//	{
//		CKLineField field;
//		CopyKLineEntity(&field, &theKLine);
//		if (theKLine.BeginTime > theLastBeginTime)
//		{
//			thePushKLines.push_back(field);
//		}
//	}
//
//	for (list<CKLineField>::iterator it = thePushKLines.begin(); it != thePushKLines.end(); it++)
//	{
//		m_pServiceResponser->SendNext(&(*it), &(it->m_Describe), OUTFOG_CHAIN_CONTINUE);
//	}
//	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
//	return true;
//}

//bool CLoginService::RspMarketData(CServiceResponser *m_pServiceResponser, const char * pExchangeID, const char * pInstrumentID, TSessionRef* pSession)
//{
//	CIndexIDType ExchangeID = pExchangeID;
//	CInstrumentIDType InstrumentID = pInstrumentID;
//	CMarketOverViewField field;
//	CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(ExchangeID, InstrumentID);
//	if (pMarketData == NULL)
//	{
//		m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
//		return false;
//	}
//	else
//	{
//		CopyMarketOverViewEntity(&field, pMarketData);
//		m_pServiceResponser->SendResponse(TID_PushMarketOverView, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
//	}
//	return true;
//}

//
//void CLoginService::RspMarketData(CServiceResponser *m_pServiceResponser, const char * ProductGroup, TSessionRef* pSession)
//{
//	CMarketOverViewField field;
//	CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->findFirstByProductGroup(ProductGroup);
//	if (pMarketData != NULL)
//	{
//		CopyMarketOverViewEntity(&field, pMarketData);
//		m_pServiceResponser->SendResponse(TID_PushMarketOverView, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
//		pMarketData = m_pMDB->m_MarketDataFactory->findNextByProductGroup();
//		while (pMarketData != NULL)
//		{
//			CopyMarketOverViewEntity(&field, pMarketData);
//			m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
//			pMarketData = m_pMDB->m_MarketDataFactory->findNextByProductGroup();
//		}
//		m_pMDB->m_MarketDataFactory->endFindByProductGroup();
//		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
//	}
//}

//void CLoginService::RspPrivate(const char* pMemberID, CServiceResponser* m_pServiceResponser, TSessionRef* pSession)
//{
//	CPositionField pfield;
//	CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByMemberID(pMemberID);
//	if (pPosition != NULL)
//	{
//		CopyPositionEntity(&pfield, pPosition);
//		m_pServiceResponser->SendResponse(TID_PushPosition, &pfield, &(pfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
//		pPosition = m_pMDB->m_PositionFactory->findNextByMemberID();
//		while (pPosition != NULL)
//		{
//			CopyPositionEntity(&pfield, pPosition);
//			m_pServiceResponser->SendNext(&pfield, &(pfield.m_Describe), OUTFOG_CHAIN_CONTINUE);
//			pPosition = m_pMDB->m_PositionFactory->findNextByMemberID();
//		}
//		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
//	}
//	m_pMDB->m_PositionFactory->endFindByMemberID();
//
//	/*CMemberPositionField mpfield;
//	CMemberPosition *pMemberPosition = m_pMDB->m_MemberPositionFactory->findFirstByMemberID(pMemberID);
//	if (pMemberPosition != NULL)
//	{
//		CopyMemberPositionEntity(&mpfield, pMemberPosition);
//		m_pServiceResponser->SendResponse(TID_PushMemberPosition, &mpfield, &(mpfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
//		pMemberPosition = m_pMDB->m_MemberPositionFactory->findNextByMemberID();
//		while (pMemberPosition != NULL)
//		{
//			CopyMemberPositionEntity(&mpfield, pMemberPosition);
//			m_pServiceResponser->SendNext(&mpfield, &(mpfield.m_Describe), OUTFOG_CHAIN_CONTINUE);
//			pMemberPosition = m_pMDB->m_MemberPositionFactory->findNextByMemberID();
//		}
//		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
//	}
//	m_pMDB->m_MemberPositionFactory->endFindByMemberID();*/
//
//	CAccountField afield;
//	CAccount* pAccount = m_pMDB->m_AccountFactory->findFirstByMemberID(pMemberID);
//	if (pAccount != NULL)
//	{
//		CopyAccountEntity(&afield, pAccount);
//		m_pServiceResponser->SendResponse(TID_PushAccount, &afield, &(afield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
//		pAccount = m_pMDB->m_AccountFactory->findNextByMemberID();
//		while (pAccount != NULL)
//		{
//			CopyAccountEntity(&afield, pAccount);
//			m_pServiceResponser->SendNext(&afield, &(afield.m_Describe), OUTFOG_CHAIN_CONTINUE);
//			pAccount = m_pMDB->m_AccountFactory->findNextByMemberID();
//		}
//		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
//	}
//	m_pMDB->m_AccountFactory->endFindByMemberID();
//
//	//COrderField ofield;
//	//COrder *pOrder = m_pMDB->m_OrderFactory->findFirstByMemberID(pMemberID);
//	//if (pOrder != NULL)
//	//{
//	//	CopyOrderEntity(&ofield, pOrder);
//	//	m_pServiceResponser->SendResponse(TID_PushOrder, &ofield, &(ofield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
//	//	pOrder = m_pMDB->m_OrderFactory->findNextByMemberID();
//	//	while (pOrder != NULL)
//	//	{
//	//		CopyOrderEntity(&ofield, pOrder);
//	//		m_pServiceResponser->SendNext(&ofield, &(ofield.m_Describe), OUTFOG_CHAIN_CONTINUE);
//	//		pOrder = m_pMDB->m_OrderFactory->findNextByMemberID();
//	//	}
//	//	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
//	//}
//	//m_pMDB->m_OrderFactory->endFindByMemberID();
//}

void CLoginService::RspRisk(CServiceResponser *m_pServiceResponser, TSessionRef* pSession)
{
	CInstrumentField ifield;
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->getFirst();
	while (pInstrument != NULL)	{
		CopyInstrumentEntity(&ifield, pInstrument);
		m_pServiceResponser->SendResponse(TID_PushInstrument, &ifield, &(ifield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		pInstrument = m_pMDB->m_InstrumentFactory->getNext();
	}

	CMarketOverViewField MarketField;
	CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	while (pMarketData != NULL) {
		CopyMarketOverViewEntity(&MarketField, pMarketData);
		m_pServiceResponser->SendResponse(TID_PushMarketOverView, &MarketField, &(MarketField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		pMarketData = m_pMDB->m_MarketDataFactory->getNext();
	}

	CCurrencyField Currencyfield;
	CCurrency *pCurrency = m_pMDB->m_CurrencyFactory->getFirst();
	while (pCurrency != NULL) {
		CopyCurrencyEntity(&Currencyfield, pCurrency);
		m_pServiceResponser->SendResponse(TID_PushCurrency, &Currencyfield, &(Currencyfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		pCurrency = m_pMDB->m_CurrencyFactory->getNext();
	}

	CMemberField Memberfield;
	CMember *pMember = m_pMDB->m_MemberFactory->getFirst();
	while (pMember != NULL)	{
		CopyMemberEntity(&Memberfield, pMember);
		m_pServiceResponser->SendResponse(TID_PushMember, &Memberfield, &(Memberfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		pMember = m_pMDB->m_MemberFactory->getNext();
	}

	CMarginRateField mfield;
	CMarginRate *pMarginRate = m_pMDB->m_MarginRateFactory->getFirst();
	while (pMarginRate != NULL)	{
		CopyMarginRateEntity(&mfield, pMarginRate);
		m_pServiceResponser->SendResponse(TID_PushMarginRate, &mfield, &(mfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		pMarginRate = m_pMDB->m_MarginRateFactory->getNext();
	}

	CFeeField ffield;
	CFee *pFee = m_pMDB->m_FeeFactory->getFirst();
	while (pFee != NULL){
		CopyFeeEntity(&ffield, pFee);
		m_pServiceResponser->SendResponse(TID_PushFee, &ffield, &(ffield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		pFee = m_pMDB->m_FeeFactory->getNext();
	}
	RspSnap(m_pServiceResponser, pSession);
}

void CLoginService::RspSnap(CServiceResponser *m_pServiceResponser, TSessionRef* pSession)
{
	CPositionField pfield;
	CPosition *pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL) {
		if (pPosition->Position != 0.0) {
			CopyPositionEntity(&pfield, pPosition);
			m_pServiceResponser->SendResponse(TID_PushPosition, &pfield, &(pfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		pPosition = m_pMDB->m_PositionFactory->getNext();
	}

	CAccountField afield;
	CAccount *pAccount = m_pMDB->m_AccountFactory->getFirst();
	while (pAccount != NULL) {
		if (pAccount->Balance == 0) {
			pAccount = m_pMDB->m_AccountFactory->getNext();
			continue;
		}
		CopyAccountEntity(&afield, pAccount);
		m_pServiceResponser->SendResponse(TID_PushAccount, &afield, &(afield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		pAccount = m_pMDB->m_AccountFactory->getNext();
	}
}

void CLoginService::RspOrder(CServiceResponser* m_pServiceResponser, TSessionRef* pSession)
{
	COrderField pfield;
	COrder* pOrder = m_pMDB->m_OrderFactory->getFirst();
	while (pOrder != NULL) {
		if (pOrder->getMember() == NULL)
		{
			pOrder = m_pMDB->m_OrderFactory->getNext();
			continue;
		}
		if (pOrder->getMember()->MemberType == PT_InnerMarketMaker)
		{
			pOrder = m_pMDB->m_OrderFactory->getNext();
			continue;
		}
		CopyOrderEntity(&pfield, pOrder);
		m_pServiceResponser->SendResponse(TID_PushOrder, &pfield, &(pfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		pOrder = m_pMDB->m_OrderFactory->getNext();
	}
}

void CLoginService::RspCopy(CServiceResponser *m_pServiceResponser, TSessionRef* pSession)
{
	CPositionField pfield;
	CPosition *pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		if (pPosition->Position != 0.0 && !pPosition->CopyMemberID.isNull())
		{
			CopyPositionEntity(&pfield, pPosition);
			m_pServiceResponser->SendResponse(TID_PushPosition, &pfield, &(pfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		pPosition = m_pMDB->m_PositionFactory->getNext();
	}

	COrderField ofield;
	COrder *pOrder = m_pMDB->m_OrderFactory->getFirst();
	while (pOrder != NULL)
	{
		if (!pOrder->CopyMemberID.isNull())
		{
			CopyOrderEntity(&ofield, pOrder);
			m_pServiceResponser->SendResponse(TID_PushOrder, &ofield, &(ofield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		pOrder = m_pMDB->m_OrderFactory->getNext();
	}
}

void CLoginService::RspTriggerOrder(CServiceResponser *m_pServiceResponser, TSessionRef* pSession)
{
	CTriggerOrderField afield;
	CTriggerOrder *pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getFirst();
	while (pTriggerOrder != NULL)
	{
		CopyTriggerOrderEntity(&afield, pTriggerOrder);
		m_pServiceResponser->SendResponse(TID_PushTriggerOrder, &afield, &(afield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getNext();
	}
}

void CLoginService::RspUserSession(CServiceResponser *m_pServiceResponser, TSessionRef* pSession)
{
	CUserSessionField field;
	CUserSession *pUserSession = m_pMDB->m_UserSessionFactory->getFirst();
	while (pUserSession != NULL)
	{
		CopyUserSessionEntity(&field, pUserSession);
		m_pServiceResponser->SendResponse(TID_PushUserSession, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		pUserSession = m_pMDB->m_UserSessionFactory->getNext();
	}
}

bool CLoginService::InsertAPIKey(CAPIKeyField *pAPIKey, TSessionRef* pSession, CTransaction *pTransaction)
{
	CUserSession *pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pAPIKey->AccessKey);
	if (pUserSession == NULL)
		pUserSession = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(pAPIKey->UserID, pAPIKey->APPID);

	//检查name的合法性
	if (pAPIKey->Name.isNull())
		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "APIKey.Name");

	//保存到UserSession：AccessKey/SecretKey/CreateTime/ExpireTime/Remark
	CWriteableUserSession theUserSession;

	if (pUserSession == NULL)
	{
		theUserSession.init();
	}
	else
	{
		m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
		m_pMDB->m_UserSessionFactory->remove(pUserSession);
	}

	theUserSession.LoginTime = pAPIKey->CreateTime;
	theUserSession.UserID = pAPIKey->UserID;
	theUserSession.Token = pAPIKey->AccessKey;
	theUserSession.APPID = pAPIKey->APPID;
	theUserSession.AuthCode = pAPIKey->SecretKey;
	theUserSession.AccessLimit = pAPIKey->AccessLimit.isNull() ? g_nDefaultAccessLimit : pAPIKey->AccessLimit.getValue();
	theUserSession.Remark = pAPIKey->Name;
	if (pAPIKey->IPAddress.isNull())
	{
		if (pAPIKey->ExpireTime.isNull())
		{
			//不绑定IP时，有效期90天
			theUserSession.ExpireTime = pAPIKey->CreateTime + g_nSessionExpireTime;
			pAPIKey->ExpireTime = theUserSession.ExpireTime;
		}
		else
		{
			theUserSession.ExpireTime = pAPIKey->ExpireTime;
		}
	}
	else
	{
		//绑定IP时，一直有效(2038-01-19 11:14:07)
		theUserSession.ExpireTime.clear();
		pAPIKey->ExpireTime = theUserSession.ExpireTime;
	}
	if (pAPIKey->MemberID.isNull())
		theUserSession.MemberID = pAPIKey->UserID;
	else
		theUserSession.MemberID = pAPIKey->MemberID;

	theUserSession.UserType = UT_Trader;

	m_pMDB->m_UserSessionFactory->add(&theUserSession, pTransaction);

	//保存到Allow：权限/IP
	//InsertAPIKeyAllow(pAPIKey->AccessKey, pAPIKey->Auth, pAPIKey->IPAddress, pTransaction);

	double RemainDays = 0;
	if (pAPIKey->ExpireTime.isNull())
		RemainDays = 99999;
	else
		RemainDays = (pAPIKey->ExpireTime - pAPIKey->CreateTime) / (24 * 60 * 60);

	pAPIKey->RemainDays = RemainDays > 0 ? RemainDays : 0;
	pAPIKey->Status = RemainDays > 0 ? 1 : 0;
	pAPIKey->AccessLimit = theUserSession.AccessLimit;
	return true;
}

bool CLoginService::UpdateAPIKey(CAPIKeyActionField *pAPIKeyAction, CAPIKeyField *pAPIKey, TSessionRef* pSession, CTransaction *pTransaction)
{
	CUserSession *pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pAPIKeyAction->AccessKey);
	if (pUserSession == NULL)
		return m_pErrorEngine->reportError(ERROR_INVALID_APIKEY);

	//更新UserSession：ExpireTime/Remark
	//CMilliSecondsType CurrentTime = getMDBTime(m_pMDB);
	//CMilliSecondsType CurrentTime = g_nCurrentTime;
	CWriteableUserSession theUserSession;
	m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
	theUserSession.Remark = pAPIKeyAction->Name;
	if (pAPIKeyAction->IPAddress.isNull())
	{
		if (pAPIKeyAction->ExpireTime.isNull())
		{
			//不绑定IP时，有效期90天
			theUserSession.ExpireTime = g_nCurrentTime + g_nSessionExpireTime;
			pAPIKey->ExpireTime = theUserSession.ExpireTime;
		}
		else
		{
			theUserSession.ExpireTime = pAPIKeyAction->ExpireTime;
		}
	}
	else
	{
		//绑定IP时，一直有效(2038-01-19 11:14:07)
		theUserSession.ExpireTime.clear();
		pAPIKey->ExpireTime = theUserSession.ExpireTime;
	}
	if (!pAPIKeyAction->AccessLimit.isNull())
	{
		theUserSession.AccessLimit = pAPIKeyAction->AccessLimit.getValue();
	}
	m_pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession, pTransaction);

	//更新Allow：权限/IP，先删除，后添加
	//DeleteAPIKeyAllow(pAPIKeyAction->AccessKey, pTransaction);
	//InsertAPIKeyAllow(pAPIKeyAction->AccessKey, pAPIKeyAction->Auth, pAPIKeyAction->IPAddress, pTransaction);

	pAPIKey->AccessKey = pAPIKeyAction->AccessKey;
	pAPIKey->SecretKey = theUserSession.AuthCode;
	pAPIKey->Name = pAPIKeyAction->Name;
	pAPIKey->Auth = pAPIKeyAction->Auth;
	pAPIKey->IPAddress = pAPIKeyAction->IPAddress;
	pAPIKey->CreateTime = theUserSession.LoginTime;
	pAPIKey->ExpireTime = theUserSession.ExpireTime;
	pAPIKey->AccessLimit = theUserSession.AccessLimit;

	double RemainDays = 0;
	if (pAPIKey->ExpireTime == INT_32_MAX)
		RemainDays = 99999;
	else
		RemainDays = (pAPIKey->ExpireTime - g_nCurrentTime) / (24 * 60 * 60);

	pAPIKey->RemainDays = RemainDays > 0 ? RemainDays : 0;
	pAPIKey->Status = RemainDays > 0 ? 1 : 0;
	return true;
}

bool CLoginService::DeleteAPIKey(CAPIKeyActionField *pAPIKeyAction, CAPIKeyField *pAPIKey, TSessionRef* pSession, CTransaction *pTransaction)
{
	CUserSession *pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pAPIKeyAction->AccessKey);
	if (pUserSession == NULL)
		return m_pErrorEngine->reportError(ERROR_INVALID_APIKEY);

	//删除UserSession
	m_pMDB->m_UserSessionFactory->remove(pUserSession, pTransaction);

	//删除Allow
	//DeleteAPIKeyAllow(pAPIKeyAction->AccessKey, pTransaction);

	pAPIKey->AccessKey = pAPIKeyAction->AccessKey;
	pAPIKey->SecretKey = pUserSession->AuthCode;
	pAPIKey->Name = pAPIKeyAction->Name;
	pAPIKey->Auth = pAPIKeyAction->Auth;
	pAPIKey->IPAddress = pAPIKeyAction->IPAddress;
	pAPIKey->CreateTime = pUserSession->LoginTime;
	pAPIKey->ExpireTime = pUserSession->ExpireTime;
	pAPIKey->RemainDays = 0;
	pAPIKey->Status = pAPIKey->RemainDays > 0 ? 1 : 0;
	pAPIKey->AccessLimit = pUserSession->AccessLimit;
	return true;
}

//void CLoginService::InsertAPIKeyAllow(CShortContentType &AccessKey, CNameType &Auth, CContentType &IPAddress, CTransaction *pTransaction)
//{
//	if (!Auth.isNull())
//	{
//		CWriteableAllow theAllowAuth;
//		theAllowAuth.AllowID = AccessKey;
//		theAllowAuth.AccessType = AT_APIKey;
//		theAllowAuth.AccessName = "Auth";
//		theAllowAuth.AccessValue = Auth;
//		m_pMDB->m_AllowFactory->add(&theAllowAuth, pTransaction);
//	}
//
//	if (!IPAddress.isNull())
//	{
//		CWriteableAllow theAllowIP;
//		theAllowIP.AllowID = AccessKey;
//		theAllowIP.AccessType = AT_APIKey;
//		theAllowIP.AccessName = "IPAddress";
//		theAllowIP.AccessValue = IPAddress;
//		m_pMDB->m_AllowFactory->add(&theAllowIP, pTransaction);
//	}
//}
//
//void CLoginService::DeleteAPIKeyAllow(CShortContentType &AccessKey, CTransaction *pTransaction)
//{
//	CAllow *pAuthAllow = m_pMDB->m_AllowFactory->findByAllowIDAccessTypeValue(AccessKey, AT_APIKey, "Auth");
//	if (pAuthAllow != NULL)
//		m_pMDB->m_AllowFactory->remove(pAuthAllow, pTransaction);
//
//	CAllow *pIPAllow = m_pMDB->m_AllowFactory->findByAllowIDAccessTypeValue(AccessKey, AT_APIKey, "IPAddress");
//	if (pIPAllow != NULL)
//		m_pMDB->m_AllowFactory->remove(pIPAllow, pTransaction);
//}

bool CLoginService::InsertOToken(COTokenInsertField *pOTokenInsert, CUserSessionField *pUserSessionField, TSessionRef* pSession, CTransaction *pTransaction)
{
	CLongIndexIDType MemberID = pOTokenInsert->MemberID;
	if (MemberID.isNull())
		MemberID = pOTokenInsert->UserID;

	CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(MemberID);
	if (pMember == NULL)
		return m_pErrorEngine->reportError(ERROR_USER_NOT_FOUND);

	CUserSession *pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pOTokenInsert->Token);
	if (pUserSession != NULL)
	{
		if (pOTokenInsert->APPID == pUserSession->APPID && pOTokenInsert->UserID == pUserSession->UserID)
		{
			if (pOTokenInsert->ExpireTime != pUserSession->ExpireTime || pOTokenInsert->AccessLimit != pUserSession->AccessLimit)
			{
				CWriteableUserSession theUserSession;
				m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
				theUserSession.ExpireTime = pOTokenInsert->ExpireTime;
				theUserSession.AccessLimit = pOTokenInsert->AccessLimit;
				m_pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession, pTransaction);
				pUserSessionField->UserID = theUserSession.UserID;
				pUserSessionField->APPID = theUserSession.APPID;
				pUserSessionField->Token = theUserSession.Token;
				pUserSessionField->SessionNo = theUserSession.SessionNo;
				return true;
			}
			else
			{
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			}
		}
		else
		{
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		}
	}
	else
	{
		pUserSession = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(pOTokenInsert->UserID, pOTokenInsert->APPID);
		if (pUserSession != NULL)
			m_pMDB->m_UserSessionFactory->remove(pUserSession, pTransaction);

		CWriteableUserSession theUserSession;
		//保存到UserSession
		theUserSession.init();
		theUserSession.UserID = pOTokenInsert->UserID;
		theUserSession.APPID = pOTokenInsert->APPID;
		theUserSession.Token = pOTokenInsert->Token;
		theUserSession.LoginTime = pOTokenInsert->LoginTime;
		theUserSession.ExpireTime = pOTokenInsert->ExpireTime;
		theUserSession.AccessLimit = pOTokenInsert->AccessLimit.isNull() ? g_nDefaultAccessLimit : pOTokenInsert->AccessLimit.getValue();

		theUserSession.MemberID = MemberID;

		theUserSession.UserType = UT_Trader;
		m_pMDB->m_UserSessionFactory->add(&theUserSession, pTransaction);

		pUserSessionField->UserID = theUserSession.UserID;
		pUserSessionField->APPID = theUserSession.APPID;
		pUserSessionField->Token = theUserSession.Token;
		pUserSessionField->SessionNo = theUserSession.SessionNo;
		return true;
	}
}

bool CLoginService::DeleteOToken(COTokenActionField *pOTokenAction, CUserSessionField *pUserSessionField, TSessionRef* pSession, CTransaction *pTransaction)
{
	CUserSession *pUserSession = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(pOTokenAction->UserID, pOTokenAction->APPID);
	if (pUserSession == NULL)
		return m_pErrorEngine->reportError(ERROR_INVALID_SESSION, "%s+%s", pOTokenAction->UserID.getValue(), pOTokenAction->APPID.getValue());

	pUserSessionField->UserID = pUserSession->UserID;
	pUserSessionField->APPID = pUserSession->APPID;
	pUserSessionField->Token = pUserSession->Token;

	m_pMDB->m_UserSessionFactory->remove(pUserSession, pTransaction);

	return true;
}