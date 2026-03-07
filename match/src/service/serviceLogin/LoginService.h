#ifndef AFX_LOGINSERVICEIMPL_H__
#define AFX_LOGINSERVICEIMPL_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "databaseData.h"
#include "CErrorEngine.h"
#include "OutFogServiceApi.h"
#include "ServiceResponser.h"
#include "ServiceInterface.h"
#include "FieldData.h"
#include "LoginService.h"

///CLoginService实现了ILoginService定义的登录功能
class CLoginService
{
public:
	CLoginService(CMDB *pMDB, CErrorEngine *pErrorEngine);
	virtual ~CLoginService();

	/**用户登录
	*@param	pSession	会话编号
	*@param	pTransaction	本操作属于的事务
	*@return	true表示成功，false表示失败
	*/
	virtual bool UserLogin(CServiceResponser *m_pServiceResponser, CUserSessionField *pLoginField, TSessionRef* pSession, CTransaction *pTransaction);

	/**用户登出
	*@param UserID	用户代码
	*@param MemberID	会员代码
	*@param	pSession	会话编号
	*@param	pTransaction	本操作属于的事务
	*@return	true表示成功，false表示失败
	*/
	virtual bool UserLogout(CUserLogoutField* pLogout, TSessionRef* pSession, CTransaction *pTransaction);

	//	virtual bool AddSession(CReadOnlyIPAddressType &IPAddress,TSessionRef* pSession, CTransaction *pTransaction);

		/**密码修改
		*@param UserID	用户代码
		*@param MemberID	会员代码
		*@param	OldPassword	旧密码
		*@param	NewPassword	新密码
		*@param	pSession	会话编号
		*@param	pTransaction	本操作属于的事务
		*@return	true表示成功，false表示失败
		*/
	//virtual bool UserPasswordUpdate(CLongIndexIDType &UserID, CReadOnlyPasswordType &OldPassword, CReadOnlyPasswordType &NewPassword, TSessionRef* pSession, CTransaction *pTransaction);

	//返回订阅频率，>=1 表示成功,并设置频率
	int CheckTopic(CServiceResponser *m_pServiceResponser, CTopicActionField* pField, TSessionRef* pSession);
	int SubTopic(CServiceResponser* m_pServiceResponser, CTopicActionField* pField, TSessionRef* pSession);

	//	bool RspMarketData(CServiceResponser *m_pServiceResponser, const char * pExchangeID, const char * pInstrumentID, TSessionRef* pSession);
		//void RspMarketData(CServiceResponser *m_pServiceResponser, const char * ProductGroup, TSessionRef* pSession);

		//bool RspMarketOrder(CServiceResponser *m_pServiceResponser, const char * pExchangeID, const char * pInstrumentID, int nLevel, TSessionRef* pSession);
		//void RspMarketOrder(CServiceResponser *m_pServiceResponser, TSessionRef* pSession);
		//bool RspMarketOrder(CServiceResponser *m_pServiceResponser, const char * pExchangeID, const char * pInstrumentID, const char * pTick, int nLevel, TSessionRef* pSession);

	bool RspLastKLine(CServiceResponser *m_pServiceResponser, const char * pExchangeID, const char * pInstrumentID, const char * pPeriod, TSessionRef* pSession);
	//bool RspLastKLine(CServiceResponser *m_pServiceResponser, const char * pExchangeID, const char * pInstrumentID, const char * pPeriod, UF_INT8 nNumber, TSessionRef* pSession);
	bool RspLastKLine(CServiceResponser *m_pServiceResponser, TSessionRef* pSession);

//	void RspPrivate(const char* pMemberID, CServiceResponser *m_pServiceResponser, TSessionRef* pSession);
	void RspRisk(CServiceResponser *m_pServiceResponser, TSessionRef* pSession);
	void RspCopy(CServiceResponser *m_pServiceResponser, TSessionRef* pSession);
	void RspTriggerOrder(CServiceResponser *m_pServiceResponser, TSessionRef* pSession);
	void RspSnap(CServiceResponser *m_pServiceResponser, TSessionRef* pSession);
	void RspOrder(CServiceResponser* m_pServiceResponser, TSessionRef* pSession);
	void RspUserSession(CServiceResponser *m_pServiceResponser, TSessionRef* pSession);

	//录入APIKEY
	bool InsertAPIKey(CAPIKeyField *pAPIKey, TSessionRef* pSession, CTransaction *pTransaction);
	//更新APIKEY
	bool UpdateAPIKey(CAPIKeyActionField *pAPIKeyAction, CAPIKeyField *pAPIKey, TSessionRef* pSession, CTransaction *pTransaction);
	//删除APIKEY
	bool DeleteAPIKey(CAPIKeyActionField *pAPIKeyAction, CAPIKeyField *pAPIKey, TSessionRef* pSession, CTransaction *pTransaction);

	bool InsertOToken(COTokenInsertField *pOTokenInsert, CUserSessionField *pUserSessionField, TSessionRef* pSession, CTransaction *pTransaction);
	bool DeleteOToken(COTokenActionField *pOTokenAction, CUserSessionField *pUserSessionField, TSessionRef* pSession, CTransaction *pTransaction);

private:
	////	void ForceLogout(CReadOnlyNumberType &FrontNo, CTransaction *pTransaction);
	//	//添加APIKEY的权限
	//void InsertAPIKeyAllow(CShortContentType &AccessKey, CNameType &Auth, CContentType &IPAddress, CTransaction *pTransaction);
	////删除APIKEY的权限
	//void DeleteAPIKeyAllow(CShortContentType &AccessKey, CTransaction *pTransaction);

private:
	CMDB *m_pMDB;
	CErrorEngine *m_pErrorEngine;
	string m_sGateWayKey;
};

#endif
