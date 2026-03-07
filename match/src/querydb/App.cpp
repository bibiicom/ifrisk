#include "App.h"
#include "public.h"
#include "CopyField.h"
#include "monitorIndex.h"
#include "CFilelogService.h"
#include "DbHelper.h"

void CmdLineArgError()
{
	printf("Usage: querydb 0 1 ... n|-h|-v");
	printf("       querydb -v      :	Show Version\n");
	printf("       querydb -h      :	Show Manual\n");
	printf("       querydb 1       :	start private at 1 \n");
	exit(0);
}

TDBMethod g_HelperTaskMethod[]
{
	TASK_METHOD_DEFINE(CTradeField, TDB_TABLE, "t_D_Trade", "TradeID,Direction", new CDataContainer<CTradeField>()),
	TASK_METHOD_DEFINE(CFinishOrderField, TDB_TABLE, "t_D_FinishOrder", "OrderID", new CDataContainer<CFinishOrderField>()),
	TASK_METHOD_DEFINE(CFinishTriggerOrderField, TDB_TABLE, "t_D_FinishTriggerOrder", "OrderID", new CDataContainer<CFinishTriggerOrderField>()),
	TASK_METHOD_DEFINE(CKLineField, TDB_TABLE, "t_D_KLine", "ExchangeID,InstrumentID,Bar", new CDataContainer<CKLineField>()),
	TASK_METHOD_DEFINE(CInstrumentField, TDB_TABLE, "t_Instrument", "ExchangeID,InstrumentID", new CDataContainer<CInstrumentField>()),
	TASK_METHOD_DEFINE(CMarketDataField, TDB_TABLE, "t_MarketData", "ExchangeID,InstrumentID", new CDataContainer<CMarketDataField>()),
	TASK_METHOD_DEFINE(CAccountDetailField, TDB_TABLE, "t_D_AccountDetail", "AccountDetailID", new CDataContainer<CAccountDetailField>()),
	TASK_METHOD_DEFINE(CSettleDetailField, TDB_TABLE, "t_D_SettleDetail", "SettleDetailID", new CDataContainer<CSettleDetailField>()),
	TASK_METHOD_END
};

CApp::CApp(int nKernelid)
{
	CMonitorIndex::init(CFilelogService::CreateInstance(GetConfigString(INI_FILE_NAME, "SyslogPath").c_str()), GetConfigString(INI_FILE_NAME, "LogLevel").c_str());
	m_sGateWayKey = GetConfigString(NetworkConfigFile, "GateWayKey");
	if (m_sGateWayKey == "")
		m_sGateWayKey = "kdfhaurksh*uerhgsgjkjesbhDuu#@kdhf";
	memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;

	m_Ini.Open(INI_FILE_NAME);
	m_pErrorEngine = new CErrorEngine();
	defineErrors(m_pErrorEngine);

	char query_db_name[256] = { 0 };
	sprintf(query_db_name, "dbname_querydb_%d", nKernelid);
	char query_db_user[256] = { 0 };
	sprintf(query_db_user, "dbuser_querydb_%d", nKernelid);
	char query_db_passwd[256] = { 0 };
	sprintf(query_db_passwd, "dbpasswd_querydb_%d", nKernelid);

	string dbName = GetConfigString(NetworkConfigFile, query_db_name);
	string userName = GetConfigString(NetworkConfigFile, query_db_user);
	string pwd = GetConfigString(NetworkConfigFile, query_db_passwd);
	if (dbName == "")
	{
		dbName = GetConfigString(NetworkConfigFile, "dbname");
		userName = GetConfigString(NetworkConfigFile, "dbuser");
		pwd = GetConfigString(NetworkConfigFile, "dbpasswd");
	}
	m_pDbHelper = new CDbHelper(dbName, userName, pwd);
	m_pDbHelper->ConnectDB();
	m_pKLineHandler = new CKLineHandler(m_pDbHelper);
	m_pTradeHandler = new CTradeHandler(m_pDbHelper);
	m_pFinishOrderHandler = new CFinishOrderHandler(m_pDbHelper);
	m_pFinishTriggerOrderHandler = new CFinishTriggerOrderHandler(m_pDbHelper);
	m_pSettleDetailHandler = new CSettleDetailHandler(m_pDbHelper);
	m_pAccountDetailHandler = new CAccountDetailHandler(m_pDbHelper);
}

CApp::~CApp()
{
}

bool CApp::RspField(const TOutFogMetaRef* pOutFogRspRef, const void* pRspBuffer)
{
	sessionRef.frontID = pOutFogRspRef->FrontNo;
	sessionRef.sessionID = pOutFogRspRef->SessionNo;
	sessionRef.requestID = pOutFogRspRef->RequestNo;
	sessionRef.pToken = pOutFogRspRef->Remark;
	sessionRef.pUserID = pOutFogRspRef->UserID;
	sessionRef.zip = pOutFogRspRef->SubjectNo;

	switch (pOutFogRspRef->MessageNo)
	{
	case TID_RecvUserLogin:
	{
		REPORT_EVENT(LOG_INFO, "RecvUserLogin", "[LinkID:%0.f][ErrorID:%d][ErrorMsg:%s]",
			pOutFogRspRef->GlobalNo, pOutFogRspRef->SubjectNo, pOutFogRspRef->Remark);
		//如果登录失败，不处理，继续等待
		if (pOutFogRspRef->SubjectNo != 0)
			break;

		CLinkMap::iterator iter = m_LinkFrom.find(pOutFogRspRef->GlobalNo);
		if (iter != m_LinkFrom.end())
		{
			SubTopics(iter->second);
		}
		break;
	}
	case TID_RecvTopicAction:
	{
		if (pOutFogRspRef->SubjectNo != 0)
		{
			REPORT_EVENT(LOG_INFO, "RecvTopicAction", "[LinkID:%0.f][ErrorID:%d][ErrorMsg:%s]",
				pOutFogRspRef->GlobalNo, pOutFogRspRef->SubjectNo, pOutFogRspRef->Remark);
			break;
		}
		CTopicActionField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		REPORT_EVENT(LOG_INFO, "RecvTopicAction", "[LinkID:%0.f][ErrorID:%d][ErrorMsg:%s][TopicID:%s][Index:%s]",
			pOutFogRspRef->GlobalNo, pOutFogRspRef->SubjectNo, pOutFogRspRef->Remark,
			field.TopicID.getValue(), field.Index.getValue());
		break;
	}
	case TID_PushInstrument:
	{
		CInstrumentField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		//CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		//CWriteableInstrument theInstrument;
		//if (pInstrument == NULL)
		//{
		//	CopyInstrumentEntity(&theInstrument, &field);
		//	m_pMDB->m_InstrumentFactory->add(&theInstrument);
		//}
		//else
		//{
		//	m_pMDB->m_InstrumentFactory->retrieve(pInstrument, &theInstrument);
		//	CopyInstrumentEntity(&theInstrument, &field);
		//	m_pMDB->m_InstrumentFactory->update(pInstrument, &theInstrument);
		//}
		break;
	}
	case TID_PushCurrency:
	{
		CCurrencyField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		//CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(field.Currency, field.SettlementGroup);
		//CWriteableCurrency theCurrency;
		//if (pCurrency == NULL)
		//{
		//	CopyCurrencyEntity(&theCurrency, &field);
		//	m_pMDB->m_CurrencyFactory->add(&theCurrency);
		//}
		//else
		//{
		//	m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
		//	CopyCurrencyEntity(&theCurrency, &field);
		//	m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
		//}
		break;
	}
	case TID_PushTimeSync:
	{
		CCurrentTimeField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		if (field.CurrTime % 600 == 0)
		{
			REPORT_EVENT(LOG_INFO, "PushTimeSync", "[CurrTime:%0.f][ZoneDate:%s][ZoneTime:%s]",
				field.CurrTime.getValue(), field.ZoneDate.getValue(), field.ZoneTime.getValue());
		}
		break;
	}
	case TID_PushUserSession:
	{
		CUserSessionField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		REPORT_EVENT(LOG_INFO, "PushUserSession", "[Token=%s][UserID=%s][APPID=%s]", field.Token.getValue(), field.UserID.getValue(), field.APPID.getValue());
		string key1 = field.UserID.getValue();
		key1 += "_";
		key1 += field.APPID.getValue();
		CUserSessionMap::iterator iter = m_UserSessionAPPIDMap.find(key1);
		if (iter != m_UserSessionAPPIDMap.end())
		{
			if (field.Token != iter->second.Token)
			{
				m_pServiceResponser->DelSession(iter->second.FrontNo.getValue(), iter->second.FrontNo.getValue());
				m_pServiceResponser->DelApiKey(iter->second.Token.getValue());
				m_UserSessionAPPIDMap.erase(iter);
				iter = m_UserSessionTokenMap.find(field.Token.getValue());
				if (iter != m_UserSessionTokenMap.end())
					m_UserSessionTokenMap.erase(iter);
			}
		}
		m_UserSessionAPPIDMap.insert(CUserSessionMap::value_type(key1, field));

		iter = m_UserSessionTokenMap.find(field.Token.getValue());
		if (iter != m_UserSessionTokenMap.end())
		{
			CopyUserSessionEntity(&(iter->second), &field);
		}
		else
		{
			m_UserSessionTokenMap.insert(CUserSessionMap::value_type(field.Token.getValue(), field));
		}

		m_pServiceResponser->AddApiKey(field.Token.getValue(), field.AuthCode.getValue(), field.UserID.getValue(), field.ExpireTime.getValue(), field.AccessLimit.getValue());
		break;
	}
	case TID_SendQryKLine:
	{
		CQryKLineField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		if (field.Limit.isNull())
			field.Limit = 600;
		KLineData* pData = m_pKLineHandler->GetKLine(&field);
		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
		if (pData != NULL)
		{
			int nCount = 0;
			for (auto it = pData->begin(); it != pData->end(); ++it)
			{
				if (nCount >= field.Limit.getValue())
					break;
				CKLineBaseField* pRspField = &(*it);
				if (!field.EndTime.isNull())
				{
					if (pRspField->InsertTime > field.EndTime)
						continue;
				}
				nCount++;
				m_pServiceResponser->SendNext(pRspField, &(pRspField->m_Describe), OUTFOG_CHAIN_CONTINUE);
			}
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendQryTrade:
	{
		nRecvTNo = TID_RecvQryTrade;
		CQryTradeField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		if (!CheckSession(m_pErrorEngine, &sessionRef, field.MemberID))
		{
			char* errMsg = NULL;
			int errorCode = 0;
			m_pErrorEngine->getLastError(&errorCode, &errMsg);
			m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), &sessionRef, OUTFOG_CHAIN_SINGLE, errorCode, errMsg);
			return false;
		}
		CBaseDataContainer* pDataContainer = m_pTradeHandler->Get(&field, &sessionRef);
		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
		if (pDataContainer != NULL)
		{
			for (size_t i = 0; i < pDataContainer->size(); i++)
			{
				CTradeField* pRspField = (CTradeField*)pDataContainer->get(i);
				m_pServiceResponser->SendNext(pRspField, &(pRspField->m_Describe), OUTFOG_CHAIN_CONTINUE);
			}
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendQryFinishOrder:
	{
		nRecvTNo = TID_RecvQryFinishOrder;
		CQryFinishOrderField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		if (!CheckSession(m_pErrorEngine, &sessionRef, field.MemberID))
		{
			char* errMsg = NULL;
			int errorCode = 0;
			m_pErrorEngine->getLastError(&errorCode, &errMsg);
			m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), &sessionRef, OUTFOG_CHAIN_SINGLE, errorCode, errMsg);
			return false;
		}
		CBaseDataContainer* pDataContainer = m_pFinishOrderHandler->Get(&field);
		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
		if (pDataContainer != NULL)
		{
			for (size_t i = 0; i < pDataContainer->size(); i++)
			{
				CFinishOrderField* pRspField = (CFinishOrderField*)pDataContainer->get(i);
				m_pServiceResponser->SendNext(pRspField, &(pRspField->m_Describe), OUTFOG_CHAIN_CONTINUE);
			}
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendQryFinishTriggerOrder:
	{
		nRecvTNo = TID_RecvQryFinishTriggerOrder;
		CQryFinishTriggerOrderField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		if (!CheckSession(m_pErrorEngine, &sessionRef, field.MemberID))
		{
			char* errMsg = NULL;
			int errorCode = 0;
			m_pErrorEngine->getLastError(&errorCode, &errMsg);
			m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), &sessionRef, OUTFOG_CHAIN_SINGLE, errorCode, errMsg);
			return false;
		}
		CBaseDataContainer* pDataContainer = m_pFinishTriggerOrderHandler->Get(&field);
		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
		if (pDataContainer != NULL)
		{
			for (size_t i = 0; i < pDataContainer->size(); i++)
			{
				CFinishTriggerOrderField* pRspField = (CFinishTriggerOrderField*)pDataContainer->get(i);
				m_pServiceResponser->SendNext(pRspField, &(pRspField->m_Describe), OUTFOG_CHAIN_CONTINUE);
			}
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendQrySettleDetail:
	{
		nRecvTNo = TID_RecvQrySettleDetail;
		CQrySettleDetailField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		CBaseDataContainer* pDataContainer = m_pSettleDetailHandler->Get(&field);
		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
		if (pDataContainer != NULL)
		{
			for (size_t i = 0; i < pDataContainer->size(); i++)
			{
				CSettleDetailField* pRspField = (CSettleDetailField*)pDataContainer->get(i);
				m_pServiceResponser->SendNext(pRspField, &(pRspField->m_Describe), OUTFOG_CHAIN_CONTINUE);
			}
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendQryAccountDetail:
	{
		nRecvTNo = TID_RecvQryAccountDetail;
		CQryAccountDetailField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		if (!CheckSession(m_pErrorEngine, &sessionRef, field.MemberID))
		{
			char* errMsg = NULL;
			int errorCode = 0;
			m_pErrorEngine->getLastError(&errorCode, &errMsg);
			m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), &sessionRef, OUTFOG_CHAIN_SINGLE, errorCode, errMsg);
			return false;
		}
		CBaseDataContainer* pDataContainer = m_pAccountDetailHandler->Get(&field, &sessionRef);
		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &sessionRef, OUTFOG_CHAIN_FIRST);
		if (pDataContainer != NULL)
		{
			for (size_t i = 0; i < pDataContainer->size(); i++)
			{
				CAccountDetailField* pRspField = (CAccountDetailField*)pDataContainer->get(i);
				m_pServiceResponser->SendNext(pRspField, &(pRspField->m_Describe), OUTFOG_CHAIN_CONTINUE);
			}
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	}
	return true;
}

bool CApp::CheckSession(CErrorEngine* pErrorEngine, TSessionRef* pSession, CLongIndexIDType& MemberID)
{
	if (pSession->pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && pSession->pToken[1] == '\0')
		return 0;

	CUserSessionMap::iterator iter = m_UserSessionTokenMap.find(pSession->pToken);
	if (iter == m_UserSessionTokenMap.end())
		return pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);;

	if (iter->second.ExpireTime < g_nCurrentTime)
		return pErrorEngine->reportError(ERROR_APIKEY_EXPIRED);;

	//普通用户，文不对其，就直接返回错误
	if (!MemberID.isNull())
	{
		if (MemberID != iter->second.MemberID)
		{
			if (iter->second.UserType != UT_SuperManager)
				return pErrorEngine->reportError(ERROR_TRADE_FORBIDDEN);;
		}
	}
	else
	{
		MemberID = iter->second.MemberID;
	}
	return true;
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

	field.TopicID = OF_Topic_Private_UserSession;
	field.ResumeNo = 0;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
	field.ResumeNo = -1;

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
	sprintf(sLink, "QueryDB_%s_Links", sKernelid.c_str());
	NetWorkIni.ReadString(sLink, "Links", "", slinks, sizeof(slinks) - 1);
	if (slinks[0] == '\0')
	{
		sKernelid = "*";
		sprintf(sLink, "QueryDB_%s_Links", sKernelid.c_str());
		NetWorkIni.ReadString(sLink, "Links", "", slinks, sizeof(slinks) - 1);
	}

	CApp* pApp = new CApp(nKernelid);
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
		sprintf(sLink, "QueryDB_%s_Link%d", sKernelid.c_str(), link.LinkNo.getValue());
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

