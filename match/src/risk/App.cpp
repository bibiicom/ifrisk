#include "App.h"
#include "public.h"
#include "CopyField.h"
#include "monitorIndex.h"
#include "CFilelogService.h"

void CmdLineArgError()
{
	printf("Usage: risk \n");
	printf("       risk -v      :	Show Version\n");
	exit(0);
}

CRisk::CRisk()
{
	//使用更多的索引
	g_cMDBRunLevel = 'r';

	m_AccountMapLastTime = 0;
	m_AccountMapStatus = 0;
	m_pMDB = new CMDB();
	int memorySize = GetConfigInt(INI_FILE_NAME, "MemorySize");
	int blockCount = GetConfigInt(INI_FILE_NAME, "MaxBlockCount");
	m_pMDB->init(memorySize, blockCount);

	CMonitorIndex::init(CFilelogService::CreateInstance(GetConfigString(INI_FILE_NAME, "SyslogPath").c_str()), GetConfigString(INI_FILE_NAME, "LogLevel").c_str());

	memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;

	m_Ini.Open(INI_FILE_NAME);
	m_pErrorEngine = new CErrorEngine();
	defineErrors(m_pErrorEngine);

	new CIntPtrMonitorIndex("PositionSize", m_pMDB->m_PositionFactory->getCountPtr(), 10);
	new CIntPtrMonitorIndex("AccountSize", m_pMDB->m_AccountFactory->getCountPtr(), 10);
}

CRisk::~CRisk()
{
}

bool CRisk::RspField(const TOutFogMetaRef* pOutFogRspRef, const void* pRspBuffer)
{
	sessionRef.frontID = pOutFogRspRef->FrontNo;
	sessionRef.sessionID = pOutFogRspRef->SessionNo;
	sessionRef.requestID = pOutFogRspRef->RequestNo;
	sessionRef.pToken = pOutFogRspRef->Remark;
	sessionRef.pUserID = pOutFogRspRef->UserID;

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
	default:
	{
	}
	}

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
	case TID_PushMarketOverView:
	{
		CMarketOverViewField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		CWriteableMarketData theMarketData;
		if (pMarketData == NULL)
		{
			theMarketData.InstrumentID = field.InstrumentID;
			theMarketData.ExchangeID = field.ExchangeID;
			theMarketData.LastPrice = field.LastPrice;
			theMarketData.MarkedPrice = field.MarkedPrice;
			m_pMDB->m_MarketDataFactory->add(&theMarketData);
		}
		else
		{
			m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
			theMarketData.InstrumentID = field.InstrumentID;
			theMarketData.ExchangeID = field.ExchangeID;
			theMarketData.LastPrice = field.LastPrice;
			theMarketData.MarkedPrice = field.MarkedPrice;
			m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
		}

		char key[64] = { 0 };
		sprintf(key, "%s:%s", field.ExchangeID.getValue(), field.InstrumentID.getValue());

		REPORT_EVENT(LOG_INFO, "PushMarketData", "[%s_%s][Last:%s][Marked:%s][Time:%0.f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(),
			field.LastPrice.getString().c_str(), field.MarkedPrice.getString().c_str(), field.UpdateTime.getValue());
		break;
	}
	case TID_PushMember:
	{
		CMemberField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		//if (field.MemberType != PT_Normal)
		//	break;
		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
		CWriteableMember theMember;
		if (pMember == NULL)
		{
			CopyMemberEntity(&theMember, &field);
			m_pMDB->m_MemberFactory->add(&theMember);
		}
		else
		{
			m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
			CopyMemberEntity(&theMember, &field);
			m_pMDB->m_MemberFactory->update(pMember, &theMember);
		}
		break;
	}
	case TID_PushMarginRate:
	{
		CMarginRateField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		CMarginRate* pStruct = m_pMDB->m_MarginRateFactory->findByMemberAndInstrument(field.MemberID, field.InstrumentID);
		CWriteableMarginRate theStruct;
		if (pStruct == NULL)
		{
			CopyMarginRateEntity(&theStruct, &field);
			m_pMDB->m_MarginRateFactory->add(&theStruct);
		}
		else
		{
			m_pMDB->m_MarginRateFactory->retrieve(pStruct, &theStruct);
			CopyMarginRateEntity(&theStruct, &field);
			m_pMDB->m_MarginRateFactory->update(pStruct, &theStruct);
		}
		break;
	}
	case TID_PushFee:
	{
		CFeeField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		CFee* pStruct = m_pMDB->m_FeeFactory->findByMemberIDAndInstrument(field.MemberID, field.InstrumentID, field.MatchRole);
		CWriteableFee theStruct;
		if (pStruct == NULL)
		{
			CopyFeeEntity(&theStruct, &field);
			m_pMDB->m_FeeFactory->add(&theStruct);
		}
		else
		{
			m_pMDB->m_FeeFactory->retrieve(pStruct, &theStruct);
			CopyFeeEntity(&theStruct, &field);
			m_pMDB->m_FeeFactory->update(pStruct, &theStruct);
		}
		break;
	}
	case TID_PushInstrument:
	{
		CInstrumentField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
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
	case TID_PushPosition:
	{
		CPositionField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if (pInstrument == NULL)
			break;
		if (pInstrument->ProductClass != PC_Margin)
			break;

		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
		if (pMember == NULL)
			break;

		REPORT_EVENT(LOG_INFO, "PushPosition", "[MemberID:%s][TradeUnitID:%s][InstrumentID:%s][PositionID:%s]",
			field.MemberID.getValue(), field.TradeUnitID.getValue(), field.InstrumentID.getValue(), field.PositionID.getValue());

		CPosition* pPosition = m_pMDB->m_PositionFactory->findByPK(&field);
		CWriteablePosition thePosition;
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
		break;
	}
	case TID_PushAccount:
	{
		CAccountField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
		if (pMember == NULL)
			break;

		REPORT_EVENT(LOG_INFO, "PushAccount", "[MemberID:%s][AccountID:%s]", field.MemberID.getValue(), field.AccountID.getValue());

		CAccount* pAccount = m_pMDB->m_AccountFactory->findByPK(&field);
		CWriteableAccount theAccount;
		if (pAccount == NULL)
		{
			theAccount.init();
			CopyAccountEntity(&theAccount, &field);
			m_pMDB->m_AccountFactory->add(&theAccount);
		}
		else
		{
			m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
			CopyAccountEntity(&theAccount, &field);
			m_pMDB->m_AccountFactory->update(pAccount, &theAccount);
		}
		break;
	}
	case TID_PushTimeSync:
	{
		CCurrentTimeField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		REPORT_EVENT(LOG_EMERGENCY, "PushTimeSync", "[CurrTime:%0.f][ZoneDate:%s][ZoneTime:%s]", field.CurrTime.getValue(), field.ZoneDate.getValue(), field.ZoneTime.getValue());

		CAccountIDType accountID = "";
		CShortIndexIDType settlementGroup = "";
		double PositionProfit = 0.0;
		double MaintMargin = 0.0;
		double Margin = 0.0;
		double AccountValue = 0.0;
		bool bDump = false;
		if (field.CurrTime > m_AccountMapLastTime + 3600.0 && m_AccountMap.size() == 0)
		{
			bDump = true;
			m_AccountMapLastTime = field.CurrTime.getValue();
			m_AccountMapStatus = 0;
		}
		CAccount* pAccount = m_pMDB->m_AccountFactory->findFirstByAll();
		while (pAccount != NULL)
		{
			accountID = pAccount->AccountID;
			settlementGroup = pAccount->SettlementGroup;
			CCurrency* pCurrency = pAccount->getCurrency();
			double price = getMarkedPrice(pCurrency);
			getRisk(m_pMDB, pAccount, PositionProfit, MaintMargin, price);
			Margin += getHairCut(pAccount, price);
			MaintMargin = MaintMargin * price;
			AccountValue += pAccount->Balance.getValue() * price;
			AccountValue += PositionProfit * price;
			if (pAccount->Balance < 0)
			{
				CMarginRate* pRate = getMarginRate(m_pMDB, pCurrency->MarginRateGroup.getValue(), pAccount);
				if (pRate != NULL)
				{
					double MaintMarginRatio = getLongMaintMarginRateValue((-pAccount->Balance.getValue()) * price, pRate);
					MaintMargin += (-pAccount->Balance.getValue()) * MaintMarginRatio * price;
				}
			}
			pAccount = m_pMDB->m_AccountFactory->findNextByAll();
			if (pAccount == NULL || pAccount->AccountID != accountID || pAccount->SettlementGroup != settlementGroup)
			{
				double mm = 1000;
				if (MaintMargin > 0.0)
					mm = (Margin + PositionProfit) / MaintMargin;
				REPORT_EVENT(LOG_EMERGENCY, "Risk", "[A:%s+%s][%f]([M:%0.4f]+[PP:%0.4f])/[MM:%0.4f]=%0.4f", accountID.getValue(), settlementGroup.getValue(), AccountValue, Margin, PositionProfit, MaintMargin, mm);
				if (mm < 1)
				{
					REPORT_EVENT(LOG_EMERGENCY, "Risk", "Close");
				}
				//恢复
				PositionProfit = 0.0;
				MaintMargin = 0.0;
				Margin = 0.0;
			}
			if (pAccount == NULL || pAccount->AccountID != accountID)
			{
				if (bDump)
					m_AccountMap.insert(CAccountMap::value_type(accountID.getValue(), AccountValue));
				AccountValue = 0.0;
			}
		}
		if (bDump)
			m_AccountMapStatus = 1;
		break;
	}
	case TID_RecvTriggerOrderAction:
	{
		if (pOutFogRspRef->SubjectNo != 0)
		{
			CTriggerOrderActionField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);
			REPORT_EVENT(LOG_INFO, "RecvTriggerOrderAction Error", "[ErrorID:%d][ErrorMsg:%s][MemberID:%s][InstrumentID:%s][OrderID:%s]",
				pOutFogRspRef->SubjectNo, pOutFogRspRef->Remark,
				field.MemberID.getValue(), field.InstrumentID.getValue(), field.OrderID.getValue());
		}
		else
		{
			CTriggerOrderField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);
			REPORT_EVENT(LOG_INFO, "RecvTriggerOrderAction Success", "[ErrorID:%d][ErrorMsg:%s][MemberID:%s][TradeUnitID:%s][InstrumentID:%s][OrderID:%s][TriggerStatus:%c]",
				pOutFogRspRef->SubjectNo, pOutFogRspRef->Remark,
				field.MemberID.getValue(), field.TradeUnitID.getValue(), field.InstrumentID.getValue(), field.OrderID.getValue(), field.TriggerStatus.getValue());
		}
		break;
	}
	default:
	{
		REPORT_EVENT(LOG_INFO, "default", "[TNo:%d][SubIndexName:%s][UserID:%s][Remark:%s]",
			pOutFogRspRef->MessageNo, pOutFogRspRef->SubIndexName, pOutFogRspRef->UserID, pOutFogRspRef->Remark);
	}
	}
	return true;
}

bool CRisk::SendForceClose(CAccount* pAccount)
{
	REPORT_EVENT(LOG_INFO, "SendForceClose:AccountID[%s],SettlementGroup[%s],Currency[%s]",
		pAccount->AccountID.getValue(), pAccount->SettlementGroup.getValue(), pAccount->Currency.getValue());

	COrderActionField fieldOrderAction;
	fieldOrderAction.LocalID.clear();
	fieldOrderAction.OrderID.clear();
	fieldOrderAction.MemberID = pAccount->MemberID;
	fieldOrderAction.AccountID = pAccount->AccountID;
	fieldOrderAction.ActionFlag = AF_Delete;

	// 发送TriggerOrderAction
	COrderActionField::m_Describe.setSingleField(m_streamBuf, &fieldOrderAction);
	//m_OutFogMetaRef.ServiceNo = m_LinkTo.LinkNo;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	m_OutFogMetaRef.MessageNo = TID_SendOrderAction;
	m_OutFogMetaRef.FieldNo = COrderActionField::m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = COrderActionField::m_Describe.m_nStreamSize;
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);


	CPositionActionField fieldPositionAction;
	fieldPositionAction.LocalID.clear();
	fieldPositionAction.MemberID = pAccount->MemberID;
	fieldPositionAction.AccountID = pAccount->AccountID;
	fieldPositionAction.ActionType = PAT_CloseAll;

	CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByAccountID(pAccount->AccountID);
	while (pPosition != NULL)
	{
		if (pPosition->SettlementGroup != pAccount->SettlementGroup || pPosition->ClearCurrency != pAccount->Currency)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		CPositionActionField::m_Describe.setSingleField(m_streamBuf, &fieldPositionAction);
		m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
		m_OutFogMetaRef.MessageNo = TID_SendPositionAction;
		m_OutFogMetaRef.FieldNo = CPositionActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CPositionActionField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
		pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
	}
	return true;
}

bool CRisk::SendForceClose(CPosition* pPosition)
{
	CPositionActionField fieldPositionAction;
	fieldPositionAction.LocalID.clear();
	fieldPositionAction.MemberID = pPosition->MemberID;
	fieldPositionAction.AccountID = pPosition->AccountID;
	fieldPositionAction.ActionType = PAT_CloseAll;

	CPositionActionField::m_Describe.setSingleField(m_streamBuf, &fieldPositionAction);
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	m_OutFogMetaRef.MessageNo = TID_SendPositionAction;
	m_OutFogMetaRef.FieldNo = CPositionActionField::m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = CPositionActionField::m_Describe.m_nStreamSize;
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
	pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
	return true;
}

void CRisk::SubTopics(CLinkField linkField)
{
	CTopicActionField field;
	m_OutFogMetaRef.FieldNo = field.m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = field.m_Describe.m_nStreamSize;
	m_OutFogMetaRef.GlobalNo = linkField.LinkNo;

	field.Action = SA_Sub;
	field.ResumeNo = -1;
	int nLocalNo = 2;

	// 最新持仓
	field.TopicID = S_Topic_Private_Risk;
	field.Index = "";
	m_OutFogMetaRef.MessageNo = TID_SendTopicAction;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

	// 最新K线
	/*field.TopicID = "4";
	field.LocalNo = nLocalNo++;
	field.FilterValue = "Test_BTCUSD_1m";
	m_OutFogMetaRef.ServiceNo = linkField.LinkNo;
	m_OutFogMetaRef.TNo = TID_SendTopicAction;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);*/

	// 时钟心跳
	/*field.TopicID = "6";
	field.LocalNo = nLocalNo++;
	field.FilterValue = "";
	m_OutFogMetaRef.ServiceNo = linkField.LinkNo;
	m_OutFogMetaRef.TNo = TID_SendTopicAction;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);*/

	// 实时行情
	field.TopicID = S_Topic_Market_OverView;
	field.Index = "";
	m_OutFogMetaRef.MessageNo = TID_SendTopicAction;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

	// 策略单
	//field.TopicID = "14";
	//field.LocalNo = nLocalNo++;
	//field.FilterValue = "";
	//m_OutFogMetaRef.SubjectNo = linkField.LinkNo;
	//m_OutFogMetaRef.MessageNo = TID_SendTopicAction;
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

	if (argc > 3)
	{
		CmdLineArgError();
	}

	int nKernelid = 0;
	if (argc > 1)
	{
		if (isValidNumber(argv[1]))
		{
			nKernelid = atoi(argv[1]);
		}
	}

	TIniFile NetWorkIni;
	NetWorkIni.Open(NETWORK_FILE_NAME);

	CRisk* pApp = new CRisk();
	COutFogService* m_pService = new COutFogService();
	COutFogServiceApi* pOutFogServiceApi = m_pService->LoadOutFogApi("omqsapi", pApp, nKernelid);
	pApp->setServiceApi(pOutFogServiceApi);

	TOutFogMetaRef OutFogMetaRef;
	OutFogMetaRef.Type = OUTFOG_TYPE_START;
	pOutFogServiceApi->ReqField(&OutFogMetaRef, NULL);

	char sLink[32];
	CLinkField link;
	char StreamBuf[4096];

	OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
	OutFogMetaRef.Type = OUTFOG_TYPE_INITLINK;
	OutFogMetaRef.MessageNo = TID_SendInitLink;
	OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;

	char slinks[1024];
	sprintf(sLink, "Risk_%d_Links", nKernelid);
	NetWorkIni.ReadString(sLink, "Links", "", slinks, sizeof(slinks) - 1);
	vector<string> SLinks = Txtsplit(slinks, ",");
	for (int i = 0; i < SLinks.size(); i++)
	{
		link.LinkNo = atoi(SLinks[i].c_str());
		sprintf(sLink, "Risk_%d_Link%d", nKernelid, link.LinkNo.getValue());
		NetWorkIni.ReadString(sLink, "LinkAddress", "", link.LinkAddress.getBuffer(), sizeof(link.LinkAddress) - 1);
		NetWorkIni.ReadString(sLink, "LinkDllName", "", link.LinkDllName.getBuffer(), sizeof(link.LinkDllName) - 1);
		NetWorkIni.ReadString(sLink, "LinkDllLoad", "", link.LinkDllLoad.getBuffer(), sizeof(link.LinkDllLoad) - 1);
		NetWorkIni.ReadString(sLink, "LinkUserID", "", link.LinkUserID.getBuffer(), sizeof(link.LinkUserID) - 1);
		NetWorkIni.ReadString(sLink, "LinkPassword", "", link.LinkPassword.getBuffer(), sizeof(link.LinkPassword) - 1);
		NetWorkIni.ReadString(sLink, "Config", "", link.Config.getBuffer(), sizeof(link.Config) - 1);

		CLinkField::m_Describe.setSingleField(StreamBuf, &link);

		strncpy(OutFogMetaRef.SubIndexName, link.LinkDllName.getValue(), sizeof(OutFogMetaRef.SubIndexName));
		OutFogMetaRef.GlobalNo = link.LinkNo.getValue();
		OutFogMetaRef.FieldNo = link.m_Describe.m_FieldID;
		OutFogMetaRef.Length = link.m_Describe.m_nStreamSize;
		pOutFogServiceApi->ReqField(&OutFogMetaRef, StreamBuf);
		pApp->m_LinkFrom.insert(CLinkMap::value_type(link.LinkNo.getValue(), link));
	}

	makedir("./his1h/");
	while (true)
	{
		if (pApp->m_AccountMapStatus == 1)
		{
			char filename[300];
			FILE* output = NULL;
			sprintf(filename, "./his1h/Account_%.0f.csv", pApp->m_AccountMapLastTime);
			output = mfopen(filename, "wt");
			CAccountMap::iterator iter = pApp->m_AccountMap.begin();
			while (iter != pApp->m_AccountMap.end())
			{
				fprintf(output, "%0.f,%s,%0.4f\n", pApp->m_AccountMapLastTime, iter->first.c_str(), iter->second);
				iter++;
			}
			fclose(output);
			pApp->m_AccountMap.clear();
			pApp->m_AccountMapStatus = 0;
		}
		SLEEP_SECONDS(2);
	}

	return 0;
}

