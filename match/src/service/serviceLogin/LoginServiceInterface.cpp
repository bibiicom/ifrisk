/////////////////////////////////////////////////////////////////////////
///CLoginServiceInterface.cpp
///实现了类 CLoginServiceInterface
///@history 
/////////////////////////////////////////////////////////////////////////

#include "LoginServiceInterface.h"
#include "CopyField.h"
#include "CTransaction.h"
#include "monitorIndex.h"
#include "CErrorEngine.h"
#include "tradingTools.h"
#include "FieldData.h"
#include "BaseFunction.h"
#include "ServiceShare.h"
#include "cryptAll.h"

//CEventMonitor loginMonitor;

static char* makeSessionBuffer(TSessionRef* pSession)
{
	static char buffer[100];
	sprintf(buffer, "front=%d,session=%d", pSession->frontID, pSession->sessionID);
	return buffer;
}

CLoginServiceInterface::CLoginServiceInterface(CLoginService* pLoginService, CServiceResponser* pServiceResponser)
{
	m_pLoginService = pLoginService;
	m_pServiceResponser = pServiceResponser;
}

CLoginServiceInterface::~CLoginServiceInterface()
{
}

bool CLoginServiceInterface::handleMessage(const TOutFogMetaRef* pOutFogReqRef, const void* pStream, TSessionRef* pSession)
{
	bool success = false;
	int nRecvTNo = 0;

	switch (pOutFogReqRef->MessageNo)
	{
	case TID_SendUserLogin:
	{
		nRecvTNo = TID_RecvUserLogin;
		CUserLoginField field;
		CUserLoginField::m_Describe.getSingleField(&field, pStream);
		CUserSessionField UserSession;
		memset(&UserSession, 0, sizeof(CUserSessionField));
		CopyUserLoginEntity(&UserSession, &field);
		success = m_pLoginService->UserLogin(m_pServiceResponser, &UserSession, pSession, m_pTransaction);
		if (!success)
		{
			//如果发现重复了,一定让其断开，要不然客户端不会自动重连
			RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
			m_pServiceResponser->DelSession(pSession->frontID, pSession->sessionID);
			return false;
		}
		m_pServiceResponser->SendResponse(nRecvTNo, &UserSession, &(UserSession.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendUserLogout:
	{
		nRecvTNo = TID_RecvUserLogout;
		CUserLogoutField field;
		CUserLogoutField::m_Describe.getSingleField(&field, pStream);
		success = m_pLoginService->UserLogout(&field, pSession, m_pTransaction);
		if (!success)
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
		m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendWalletNonce:
	{
		nRecvTNo = TID_RecvWalletNonce;
		CWalletNonceField field;
		CWalletNonceField::m_Describe.getSingleField(&field, pStream);
		CWriteableUserSession theUserSession;
		CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(field.UserID, "");
		if (pUserSession == NULL)
		{
			theUserSession.init();
			theUserSession.UserID = field.UserID;
			theUserSession.MemberID = field.UserID;
			theUserSession.Remark = g_nCurrentTime;
			char sToken[33];
			MD5Hex(sToken, theUserSession.UserID.getValue(), theUserSession.LoginTime.getString().c_str(), theUserSession.APPID);
			theUserSession.Token = sToken;
			theUserSession.ExpireTime = g_nCurrentTime + 7 * 24 * 3600 * 1000;
			m_pMDB->m_UserSessionFactory->add(&theUserSession, m_pTransaction);
		}
		else
		{
			if (theUserSession.ExpireTime > g_nCurrentTime)
			{
				m_pMDB->m_UserSessionFactory->remove(pUserSession);
				theUserSession.init();
				theUserSession.UserID = field.UserID;
				theUserSession.MemberID = field.UserID;
				theUserSession.Remark = g_nCurrentTime;
				char sToken[33];
				MD5Hex(sToken, theUserSession.UserID.getValue(), theUserSession.LoginTime.getString().c_str(), theUserSession.APPID);
				theUserSession.Token = sToken;
				theUserSession.ExpireTime = g_nCurrentTime + 7 * 24 * 3600 * 1000;
				m_pMDB->m_UserSessionFactory->add(&theUserSession, m_pTransaction);
			}
			else
			{
				m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
				theUserSession.Remark = g_nCurrentTime;
				theUserSession.MemberID = field.UserID;
				m_pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession, m_pTransaction);
			}
		}
		field.Nonce = theUserSession.Remark.getValue();

		if (field.MemberID.isNull())
			field.MemberID = field.UserID;

		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
		if (pMember == NULL)
		{
			CWriteableMember theMember;
			theMember.init();
			theMember.MemberID = field.MemberID;
			theMember.MemberType = PT_Normal;
			m_pMDB->m_MemberFactory->add(&theMember, m_pTransaction);
		}
		m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		success = true;
		break;
	}
	case TID_SendWalletLogin:
	{
		nRecvTNo = TID_RecvWalletLogin;
		CWalletLoginField field;
		CWalletLoginField::m_Describe.getSingleField(&field, pStream);
		CWriteableUserSession theUserSession;
		CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(field.UserID, "");
		if (pUserSession == NULL)
		{
			m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);

		//if (pUserSession->Password == "")
		//{
		//	m_pErrorEngine->reportError(ERROR_INVALID_USER_OR_PASSWORD);
		//	return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
		//}
		theUserSession.Remark = g_nCurrentTime;
		m_pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession, m_pTransaction);
		CUserSessionField rspField;
		CopyUserSessionEntity(&rspField, &theUserSession);
		m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(rspField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		success = true;
		break;
	}
	//case TID_SendUserPasswordUpdate:
	//{
	//	nRecvTNo = TID_RecvUserPasswordUpdate;
	//	CUserPasswordUpdateField field;
	//	CUserPasswordUpdateField::m_Describe.getSingleField(&field, pStream);
	//	success = m_pLoginService->UserPasswordUpdate(field.UserID, field.OldPassword, field.NewPassword, pSession, m_pTransaction);
	//	if (!success)
	//		return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
	//	m_pServiceResponser->SendResponse(TID_RecvUserPasswordUpdate, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
	//	break;
	//}
	case TID_SendTopicAction:
	{
		nRecvTNo = TID_RecvTopicAction;
		CTopicActionField field;
		CTopicActionField::m_Describe.getSingleField(&field, pStream);
		switch (field.Action)
		{
		case SA_Sub:
		{
			int nReaderTimes = m_pLoginService->CheckTopic(m_pServiceResponser, &field, pSession);
			if (nReaderTimes <= 0)
				return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
			m_pServiceResponser->SendResponse(TID_RecvTopicAction, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			pSession->zip = field.Zip.getValue();
			m_pLoginService->SubTopic(m_pServiceResponser, &field, pSession);
			m_pServiceResponser->SubSubjectID(nRecvTNo, field.TopicID.getInt(), field.ResumeNo.getValue(), field.Index.getValue(), pSession, field.Zip.getValue(), UF_INT8_MAX, nReaderTimes);
			break;
		}
		case SA_UnSub:
		{
			m_pServiceResponser->UnSubSubjectID(field.TopicID.getInt(), field.Index.getValue(), pSession);
			break;
		}
		case SA_Clear:
		{
			m_pServiceResponser->UnSubSubjectIDAll(pSession);
			break;
		}
		default:
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		}
		break;
	}
	case TID_SendAPIKeyInsert:
	{
		nRecvTNo = TID_RecvAPIKeyInsert;
		CAPIKeyInsertField field;
		CAPIKeyInsertField::m_Describe.getSingleField(&field, pStream);
		//field.AccessKey.deleteChar('\\');

		CAPIKeyField outField;
		memset(&outField, 0, sizeof(CAPIKeyField));
		CopyAPIKeyInsertEntity(&outField, &field);

		success = m_pLoginService->InsertAPIKey(&outField, pSession, m_pTransaction);
		if (!success)
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
		m_pServiceResponser->SendResponse(nRecvTNo, &outField, &(outField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendAPIKeyAction:
	{
		nRecvTNo = TID_RecvAPIKeyAction;
		CAPIKeyActionField field;
		CAPIKeyActionField::m_Describe.getSingleField(&field, pStream);
		//field.AccessKey.deleteChar('\\');

		switch (field.ActionFlag)
		{
		case AF_Delete:
		{
			CAPIKeyField outField;
			memset(&outField, 0, sizeof(CAPIKeyField));
			success = m_pLoginService->DeleteAPIKey(&field, &outField, pSession, m_pTransaction);
			if (!success)
				return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
			m_pServiceResponser->SendResponse(nRecvTNo, &outField, &(outField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			break;
		}
		case AF_Modify:
		{
			CAPIKeyField outField;
			memset(&outField, 0, sizeof(CAPIKeyField));
			success = m_pLoginService->UpdateAPIKey(&field, &outField, pSession, m_pTransaction);
			if (!success)
				return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
			m_pServiceResponser->SendResponse(nRecvTNo, &outField, &(outField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			break;
		}
		default:
			break;
		}
		break;
	}
	case TID_SendOTokenInsert:
	{
		nRecvTNo = TID_RecvOTokenInsert;
		COTokenInsertField field;
		COTokenInsertField::m_Describe.getSingleField(&field, pStream);
		//field.Token.deleteChar('\\');

		if (!CheckManagerFront(m_pErrorEngine, pSession->pToken))
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
		CUserSessionField outField;
		memset(&outField, 0, sizeof(CUserSessionField));
		success = m_pLoginService->InsertOToken(&field, &outField, pSession, m_pTransaction);
		if (!success)
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
		m_pServiceResponser->SendResponse(nRecvTNo, &outField, &(outField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendOTokenAction:
	{
		nRecvTNo = TID_RecvOTokenAction;

		COTokenActionField field;
		COTokenActionField::m_Describe.getSingleField(&field, pStream);

		if (!CheckManagerFront(m_pErrorEngine, pSession->pToken))
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));

		switch (field.ActionFlag)
		{
		case AF_Delete:
		{
			CUserSessionField outField;
			memset(&outField, 0, sizeof(CUserSessionField));
			success = m_pLoginService->DeleteOToken(&field, &outField, pSession, m_pTransaction);
			if (!success)
				return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
			m_pServiceResponser->SendResponse(nRecvTNo, &outField, &(outField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			break;
		}
		default:
		{
			m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		}
		break;
	}
	case TID_SendUserRegister:
	{
		nRecvTNo = TID_RecvUserRegister;
		CUserSessionField field;
		CUserSessionField::m_Describe.getSingleField(&field, pStream);
		//if (!CheckManagerFront(m_pErrorEngine, pSession->pToken))
		//	return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));

		if(field.MemberID.isNull())
			field.MemberID = field.UserID;
		
		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
		if (pMember == NULL)
		{
			CWriteableMember theMember;
			theMember.init();
			theMember.MemberID = field.MemberID;
			theMember.MemberType = PT_Normal;
			m_pMDB->m_MemberFactory->add(&theMember, m_pTransaction);
		}

		CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(field.UserID, "");
		if (pUserSession != NULL)
		{
			m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		CWriteableUserSession UserSession;
		UserSession.init();
		CopyUserSessionEntity(&UserSession, &field);
		char sToken[33];
		MD5Hex(sToken, UserSession.UserID.getValue(), UserSession.LoginTime.getString().c_str(), UserSession.APPID);
		UserSession.Token = sToken;
		pUserSession = m_pMDB->m_UserSessionFactory->add(&UserSession, m_pTransaction);
		CUserSessionField outField;
		CopyUserSessionEntity(&outField, pUserSession);
		m_pServiceResponser->SendResponse(nRecvTNo, &outField, &(outField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		success = true;
		break;
	}
	case TID_SendCheckToken:
	{
		nRecvTNo = TID_RecvCheckToken;
		CCheckTokenField field;
		CCheckTokenField::m_Describe.getSingleField(&field, pStream);

		CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->findByToken(field.Token);
		if (pUserSession == NULL)
		{
			pUserSession = m_pMDB->m_UserSessionFactory->findByToken(pSession->pToken);
			if (pUserSession == NULL)
			{
				m_pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);
				return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession, &field, &(field.m_Describe));
			}
		}
		field.Token = pUserSession->Token;
		field.MaxLocalID = pUserSession->MaxLocalID;
		field.MemberID = pUserSession->MemberID;
		field.UserID = pUserSession->UserID;
		m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	default:
		return true;
	}

	if (success)
		m_pTransaction->commit();
	else
		m_pTransaction->rollback();

	return false;
}

bool CLoginServiceInterface::handleRequest(const TOutFogMetaRef* pOutFogReqRef, const void* pStream, TSessionRef* pSession)
{
	return true;
}

