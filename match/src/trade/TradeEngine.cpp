#include "TradeEngine.h"
#include "CWedgeConfig.h"
#include "MyAlterService.h"
#include "CSVToField.h"
//#include "MarketDataTriggers.h"
#include "CFilelogService.h"
#include "BaseFunction.h"
#include "printinfo.h"
#include "Config.h"

CNameType SystemName;

CTradeEngine::CTradeEngine(COutFogServiceCallback *pCallback) :CEngine(), COutFogServiceApi(m_pServiceCallback)
{
	m_nLastDumpNo = 0.0;
	m_bFirstField = true;
	SystemName = GetConfigString(INI_FILE_NAME, "SystemName").c_str();
	if (SystemName.isNull())
		SystemName = "NoNameSystem";

	if (!g_bOnlyRedo)
		CMonitorIndex::init(CFilelogService::CreateInstance(GetConfigString(INI_FILE_NAME, "SyslogPath").c_str()), GetConfigString(INI_FILE_NAME, "LogLevel").c_str());

	//配置文件获取多少个报文dump一次
	m_nSnapCount = GetConfigInt(INI_FILE_NAME, "SnapCount");
	if (m_nSnapCount <= 0)
		m_nSnapCount = GetConfigInt(INI_FILE_NAME, "DumpCount");
	if (m_nSnapCount <= 0)
		m_nSnapCount = 100000;

	m_nSnapCountRedo = GetConfigInt(INI_FILE_NAME, "SnapCountRedo");
	if (m_nSnapCountRedo <= 0)
		m_nSnapCountRedo = 100000;

	//选取dump号最大的作为反演的起点
	sSnapPath = GetConfigString(INI_FILE_NAME, "SnapPath");

	if (sSnapPath == "")
	{
		PRINT_TO_STD("no SnapPath in %s", INI_FILE_NAME);
		exit(-1);
	}

	//获取从哪个号开始反演
	char dumpAllPath[MAX_PATH_LEN + 1];
	string sflowName = "";
	vector<string> flowNames;
	if (g_bOnlyRedo)
	{
		if (g_nBeginID < 0)
			sprintf(dumpAllPath, "%ssnap.*.redo", sSnapPath.c_str());
		else
			sprintf(dumpAllPath, "%ssnap.%0.f*.redo", sSnapPath.c_str(), g_nBeginID);
		flowNames = findNames(dumpAllPath, findNames_DIR);
	}

	if (flowNames.size() == 0)
	{
		if (g_nBeginID < 0)
			sprintf(dumpAllPath, "%ssnap.*", sSnapPath.c_str());
		else
			sprintf(dumpAllPath, "%ssnap.%0.f*", sSnapPath.c_str(), g_nBeginID);
		flowNames = findNames(dumpAllPath, findNames_DIR);
	}

	if (flowNames.size() > 0)
	{
		vector<string>::iterator iter = flowNames.begin();
		while (iter != flowNames.end())
		{
			string eachName = *iter;
			vector<string> singlename = Txtsplit(eachName, "snap.");
			vector<string> singlename2 = Txtsplit(singlename[1], ".");
			UF_INT8 nThisBeginID = atof(singlename2[0].c_str());
			if (nThisBeginID >= g_nBeginID)
			{
				g_nBeginID = nThisBeginID;
				sflowName = eachName;
			}
			iter++;
		}
	}

	if (sflowName == "")
	{
		PRINT_TO_STD("no found SnapName");
		exit(-1);
	}
	else
	{
		//如果在存数字和.redo的快照同时存在的情况下，使用存数字作为启动，因为存数字是原版
		sprintf(dumpAllPath, "%ssnap.%0.f", sSnapPath.c_str(), g_nBeginID);
		flowNames = findNames(dumpAllPath, findNames_DIR);
		if (flowNames.size() > 0)
			sflowName = dumpAllPath;
	}

	m_pServiceResponser = new CMyXTPResponser(sSnapPath.c_str(), g_nBeginID, pCallback, m_pMDB);

	//资金相关服务
	m_pSettlementService = new CSettlementService(m_pMDB, m_pErrorEngine);
	m_pSettlementServiceInterface = new CSettlementServiceInterface(m_pSettlementService, m_pServiceResponser);
	addServiceInterface(m_pSettlementServiceInterface);

	//初始化trade服务
	m_pTradeService = new CWedgeConfig(m_pMDB, m_pErrorEngine);
	m_pTradeServiceInterface = new CTradeServiceInterface(m_pTradeService, m_pServiceResponser);
	m_pTradeServiceInterface->m_pSettlementService = m_pSettlementService;
	addServiceInterface(m_pTradeServiceInterface);

	//登录服务
	m_pLoginService = new CLoginService(m_pMDB, m_pErrorEngine);
	m_pLoginServiceInterface = new CLoginServiceInterface(m_pLoginService, m_pServiceResponser);
	addServiceInterface(m_pLoginServiceInterface);

	//初始化内存修改服务	
	m_pDbmtService = new CMyAlterService(m_pMDB, m_pErrorEngine);
	m_pDbmtServiceLink = new CMyAlterServiceInterface(m_pDbmtService, m_pServiceResponser);
	addServiceInterface(m_pDbmtServiceLink);

	//初始化查询服务
	m_pQueryXTPLink = new CQueryServiceInterface(m_pServiceResponser);
	addServiceInterface(m_pQueryXTPLink);

	//初始化Bulletin服务
// 	m_pBulletinService = new CBulletinServiceImpl(m_pMDB,m_pErrorEngine,false);
// 	m_pBulletinServiceInterface = new CBulletinServiceInterface(m_pBulletinService,m_pErrorEngine,m_pServiceResponser);
// 	addServiceInterface(m_pBulletinServiceInterface);

	//初始化行情服务
	//m_pMarketDataService = new CMarketDataService(m_pServiceResponser, m_pMDB);

	//初始化MBLMarketData的数据,需要再LoadCSV之前，因为需要根据Order自动生成
	//m_pMDB->m_InstrumentFactory->addActionTrigger(new CInstrumentTriggerForOrderBook(m_pTradeService->getOrderBookList()));
	m_pMDB->m_InstrumentFactory->addCommitTrigger(new CInstrumentTriggerForOrderBook(m_pTradeService->getOrderBookList()));
	m_pMDB->m_InstrumentFactory->addCommitTrigger(new CTableCurrencyCurrencyTrigger(m_pMDB));
	//m_pMDB->m_OrderFactory->addActionTrigger(new COrderTriggerForOrderBook(m_pTradeService->getOrderBookList()));
	m_pMDB->m_OrderFactory->addCommitTrigger(new COrderTriggerForOrderBook(m_pTradeService->getOrderBookList()));
	m_pMDB->m_OrderFactory->addCommitTrigger(new COrderMBLTrigger(m_pMDB));

	//为了能让front拿到已经登录的UserSession必须发送出去
	m_pMDB->m_UserSessionFactory->addCommitTrigger(new CUserSessionSendTrigger(m_pServiceResponser));
	//变量设置需要提前做
	m_pMDB->m_ServiceConfigFactory->addCommitTrigger(new CServiceConfigTrigger());
	//行情需要先发布一个快照，为了系统重启之后能够有展示行情
	m_pMDB->m_MarketDataFactory->addCommitTrigger(new CMarketOverViewTrigger(m_pServiceResponser, m_pMDB));

	memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	memset(m_pStreamBuf, 0, sizeof(OUTFOG_MAX_PACKAGE_SIZE));

	UF_INT8 nOMQBegin = g_nBeginID;
	if (nOMQBegin < 0)
		nOMQBegin = 0;
	m_pServiceResponser->InitKernel(nOMQBegin);
	//必须发送一个报文，这样才能发出本次的globleNo,下游系统才能判断启动标识，要不然无法启动
	m_pServiceResponser->GetMetaRef()->Type = OUTFOG_TYPE_NULL;
	m_pServiceResponser->GetMetaRef()->Direction = OUTFOG_DIRECTION_TO_FRONT;
	m_pServiceResponser->GetCallback()->RspField(m_pServiceResponser->GetMetaRef(), NULL);

	LoadFromCSV(sflowName.c_str(), g_nBeginID);

	//这个是最早的Trigger发送过程，要不然会出现后面的Triger业务改变结果(特别是Order删除)无法被发送的情况
	m_SyncTriggers = new CSyncTriggers(m_pMDB, m_pServiceResponser);

	//大部分的Trigger只需要在数据初始完成之后生成即可
	//m_pMDB->m_CurrentTimeFactory->addCommitTrigger(new CDepthMarketDataTimer(m_pMarketDataService));
	//m_pMDB->m_MarketOrderFactory->addCommitTrigger(new CMarketOrderNotifyTrigger(m_pMarketDataService));

	//临时为了WashOnly
	m_pTradeService->m_pMarketTradeTrigger = new CMarketTradeTrigger(m_pServiceResponser, m_pMDB);
	m_pMDB->m_TradeFactory->addCommitTrigger(m_pTradeService->m_pMarketTradeTrigger);

	//K线
	m_pTradeService->m_pKLineTrigger = new CKLineTrigger(m_pMDB);
	m_pMDB->m_TradeFactory->addCommitTrigger(m_pTradeService->m_pKLineTrigger);
	m_pMDB->m_CurrentTimeFactory->addCommitTrigger(m_pTradeService->m_pKLineTrigger);
	m_pMDB->m_InstrumentFactory->addCommitTrigger(m_pTradeService->m_pKLineTrigger);
	//m_pMDB->m_MarketDataFactory->addCommitTrigger(m_pTradeService->m_pKLineTrigger);

	m_pMDB->m_PositionFactory->addCommitTrigger(new CUserOpenPositionsTrigger(m_pMDB));
	//	m_pMDB->m_FinishOrderFactory->addCommitTrigger(new CTriggerOrderStatusTrigger(m_pMDB));
	//m_pMDB->m_AccountFactory->addActionTrigger(new CAccountProductGroupTrigger(m_pMDB));
	//	m_pMDB->m_AccountFactory->addCommitTrigger(new CTableCurrencyAccumulationTrigger(m_pMDB));
		//这里里面会删除Order,如果有其他Order的Trigger在后面，就会拿到删除之后的Order，造成异常
	m_pMDB->m_TriggerOrderFactory->addCommitTrigger(new CPositionCloseAllTrigger(m_pMDB));
//	m_pMDB->m_TriggerOrderFactory->addCommitTrigger(new CUserTriggerOrdersTrigger(m_pMDB));
//	m_pMDB->m_OrderFactory->addCommitTrigger(new CTriggerOrderStatusTrigger(m_pMDB));
	m_pMDB->m_OrderFactory->addCommitTrigger(new CUserOpenOrdersTrigger(m_pMDB));
//	m_pMDB->m_UserSessionFactory->addCommitTrigger(new CTableUserAPIKeyTrigger(m_pMDB));
}

CTradeEngine::~CTradeEngine(void)
{
	/*
		//结束公告服务
		removeServiceInterface(m_pBulletinServiceInterface);
		delete m_pBulletinServiceInterface;
		delete m_pBulletinService;
	*/

	delete m_SyncTriggers;
	delete m_pServiceResponser;

	//结束trade服务
	removeServiceInterface(m_pTradeServiceInterface);
	delete m_pTradeServiceInterface;
	delete m_pTradeService;

	//结束登录服务
	removeServiceInterface(m_pLoginServiceInterface);
	delete m_pLoginServiceInterface;
	delete m_pLoginService;

	//结束内存操作服务
	removeServiceInterface(m_pDbmtServiceLink);
	delete m_pDbmtServiceLink;
	delete m_pDbmtService;

	/*removeServiceInterface(m_pQueryXTPLink);
	delete m_pQueryXTPLink;
	delete m_pQueryService;*/
}

bool CTradeEngine::ReqField(const TOutFogMetaRef* pOutFogReqRef, const void *pStream)
{
	g_nSequenceNo = pOutFogReqRef->SequenceNo;

	if (g_bSnapFirst)
	{
		if (pOutFogReqRef->IsReplay == 0)
		{
			g_bSnapName = "Restart";
			snapMDB(pOutFogReqRef->SequenceNo + 1, true);
			if (g_bRoolback)
				exit(0);
			return true;
		}
	}

	TpSession sessionBase;
	TSessionRef* pSession = &sessionBase;
	pSession->frontID = pOutFogReqRef->FrontNo;
	pSession->sessionID = pOutFogReqRef->SessionNo;
	pSession->requestID = pOutFogReqRef->RequestNo;
	pSession->pToken = pOutFogReqRef->Remark;
	pSession->pUserID = pOutFogReqRef->UserID;
	pSession->zip = pOutFogReqRef->SubjectNo;

	//第一个报文处理同步完成后的处理
	//同步完成之后需要处理很多MDB的数据，这些数据需要被下游感知到
	if (m_bFirstField)
	{
		//如果发现时间没有，不能启动，等时间报文
		CCurrentTime *pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
		if (pCurrentTime == NULL)
		{
			if (pOutFogReqRef->Type == OUTFOG_TYPE_TIMER)
			{
				UF_INT8 nCurrTime = pOutFogReqRef->SessionNo;
				m_gAccountDetailSequence.initValueByTime(nCurrTime);
				m_gTradeSequence.initValueByTime(nCurrTime);
				m_gOrderSequence.initValueByTime(nCurrTime);
				m_gSortSequence.initValueByTime(nCurrTime);
				m_gBusinessSequence.initValueByTime(nCurrTime);
				g_nCurrentTime = pOutFogReqRef->BusinessNo * 1000 + pOutFogReqRef->RequestNo;
			}
			else
				return false;
		}
		else
		{
			UF_INT8 nCurrTime = pCurrentTime->CurrTime;
			if (pCurrentTime->LastAccountDetailID.isNull())
				m_gAccountDetailSequence.initValueByTime(nCurrTime);
			else
				m_gAccountDetailSequence.initValueByValue(pCurrentTime->LastAccountDetailID.getValue());

			if (pCurrentTime->LastOrderID.isNull())
				m_gOrderSequence.initValueByTime(nCurrTime);
			else
				m_gOrderSequence.initValueByValue(pCurrentTime->LastOrderID.getValue());

			if (pCurrentTime->LastTradeID.isNull())
				m_gTradeSequence.initValueByTime(nCurrTime);
			else
				m_gTradeSequence.initValueByValue(pCurrentTime->LastTradeID.getValue());

			if (pCurrentTime->LastTimeSortNo.isNull())
				m_gSortSequence.initValueByTime(nCurrTime);
			else
				m_gSortSequence.initValueByValue(pCurrentTime->LastTimeSortNo.getValue());

			if (pCurrentTime->LastBusinessNo.isNull())
				m_gBusinessSequence.initValueByTime(nCurrTime);
			else
				m_gBusinessSequence.initValueByValue(pCurrentTime->LastBusinessNo.getValue());
		}

		//为什么需要在第一个报文来的时候做
		//TID_DataSyncEnd在第一个报文处理之前，利用报文的序号，把变化让下游感知到
		TOutFogMetaRef OutFogMetaRef;
		memset(&OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
		OutFogMetaRef.MessageNo = TID_DataSyncEnd;
		OutFogMetaRef.FieldNo = CCurrentTimeField::m_Describe.m_FieldID;
		OutFogMetaRef.Length = CCurrentTimeField::m_Describe.m_nStreamSize;
		CCurrentTimeField field;
		char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
		CCurrentTimeField::m_Describe.setSingleField(StreamBuf, &field);
		handleMessage(&OutFogMetaRef, StreamBuf, pSession);

		//m_pTradeService->ResetSequence(m_nCurrTime);

		m_bFirstField = false;
		//		PushTriggerTables();
	}

	switch (pOutFogReqRef->Type)
	{
	case OUTFOG_TYPE_TIMER:
	{
		g_nCurrentTime = pOutFogReqRef->BusinessNo * 1000 + pOutFogReqRef->RequestNo;
		CWriteableCurrentTime OldTime;
		CWriteableCurrentTime NewTime;
		CCurrentTime* pOldTime = m_pMDB->m_CurrentTimeFactory->getFirst();
		if (pOldTime == NULL)
		{
			OldTime.ZoneDate = "20240706";
			OldTime.ZoneTime = "00:00:00";
			OldTime.CurrTime = 1720195200;
			OldTime.TimeZone = 57600;
			OldTime.UpdateTime = 1720195200000.0;
			OldTime.SystemID = SystemName.getValue();
			m_pMDB->m_CurrentTimeFactory->add(&OldTime);
			pOldTime = m_pMDB->m_CurrentTimeFactory->getFirst();
		}
		CopyCurrentTimeEntity(&OldTime, pOldTime);

		CCurrentTimeField field;
		field.ZoneDate = pOutFogReqRef->SubIndexName;
		field.ZoneTime = pOutFogReqRef->UserID;
		field.ZoneDays = pOutFogReqRef->Remark;
		field.CurrTime = pOutFogReqRef->BusinessNo;
		field.TimeZone = pOutFogReqRef->SubjectNo;
		field.UpdateTime = pOutFogReqRef->RequestNo;
		OldTime.SystemID = SystemName.getValue();
		CopyCurrentTimeEntity(&NewTime, &field);
		//		m_OutFogMetaRef.FrontNo = pOutFogReqRef->FrontNo;
		//		m_OutFogMetaRef.SessionNo = pOutFogReqRef->SessionNo;
		//		m_OutFogMetaRef.RequestNo = pOutFogReqRef->RequestNo;
		m_OutFogMetaRef.MessageNo = TID_PushTimeSync;
		CCurrentTimeField::m_Describe.setSingleField(m_pStreamBuf, &field);
		m_OutFogMetaRef.Length = CCurrentTimeField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.FieldNo = CCurrentTimeField::m_Describe.m_FieldID;
		handleMessage(&m_OutFogMetaRef, m_pStreamBuf, pSession);

		//如果需要定时器，这里处理定时器需要执行的接口
		Timer(pOutFogReqRef, &OldTime, &NewTime);
		break;
	}
	//case OUTFOG_TYPE_DELSESSION:
	//{
	//	CUserLogoutField field;
	//	m_OutFogMetaRef.FrontNo = pOutFogReqRef->FrontNo;
	//	m_OutFogMetaRef.SessionNo = pOutFogReqRef->SessionNo;
	//	m_OutFogMetaRef.RequestNo = pOutFogReqRef->RequestNo;
	//	m_OutFogMetaRef.MessageNo = TID_SendUserLogout;
	//	CUserLogoutField::m_Describe.setSingleField(m_pStreamBuf, &field);
	//	m_OutFogMetaRef.Length = CUserLogoutField::m_Describe.GetStreamSize();
	//	m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;
	//	handleMessage(&m_OutFogMetaRef, m_pStreamBuf);
	//	break;
	//}
	//case OUTFOG_TYPE_ADDSESSION:
	//{
	//	CUserSessionField field;
	//	m_OutFogMetaRef.FrontNo = pOutFogReqRef->FrontNo;
	//	m_OutFogMetaRef.SessionNo = pOutFogReqRef->SessionNo;
	//	m_OutFogMetaRef.RequestNo = pOutFogReqRef->RequestNo;
	//	m_OutFogMetaRef.MessageNo = TID_SendUserLogin;
	//	field.PublicIPAddress = pOutFogReqRef->Remark;
	//	CUserSessionField::m_Describe.setSingleField(m_pStreamBuf, &field);
	//	m_OutFogMetaRef.Length = CUserSessionField::m_Describe.GetStreamSize();
	//	m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;
	//	handleMessage(&m_OutFogMetaRef, m_pStreamBuf);
	//	break;
	//}
	case OUTFOG_TYPE_STOP:
	{
		//给sinkdb完成号之前，需要把需要同步的全部给sinkdb
//		PushAccountPosition();

		CErrorField responseField;
		g_bSnapName = "Stop";

		responseField.ErrorNo = 0;
		responseField.ErrorMsg = "EngineStoped";

		m_pServiceResponser->SendResponse(0, &responseField, &(responseField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);

		//完成未发完的数据
		m_pServiceResponser->SendDelay();

		//给sinkdb一个时间信号，要不然sinkdb的完成号和上游检测不一致
		CCurrentTime *pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
		if (pCurrentTime != NULL)
		{
			m_pServiceResponser->PrepareMetaRef(&CCurrentTimeField::m_Describe, (void*)pCurrentTime, (void*)pCurrentTime, OUTFOG_TYPE_STOP);

			CCurrentTimeField field;
			CopyCurrentTimeEntity(&field, pCurrentTime);

			m_pServiceResponser->GetMetaRef()->RequestNo = 1;
			m_pServiceResponser->GetMetaRef()->Type = OUTFOG_TYPE_MANAGER;
			CCurrentTimeField::m_Describe.setSingleField(m_pServiceResponser->GetStreamBuf(), &field);
			m_pServiceResponser->GetCallback()->RspField(m_pServiceResponser->GetMetaRef(), m_pServiceResponser->GetStreamBuf());
		}

		if (g_bOnlyRedo)
		{
			g_bSnapName += ".redo";
			snapMDB(pOutFogReqRef->SequenceNo + 1, true);
			exit(-1);
		}
		else
		{
			snapMDB(pOutFogReqRef->SequenceNo + 1, true);
		}
		break;
	}
	case OUTFOG_TYPE_START:
	{
		CErrorField responseField;
		//	m_pTradeService->dumpMemDb(pOutFogReqRef->SequenceNo, "Start");
		//	m_pServiceResponser->setdumpNo(pOutFogReqRef->SequenceNo);
		//	m_nLastDumpNo = pOutFogReqRef->SequenceNo;
		//	m_bEngineWork = true;
		responseField.ErrorNo = 0;
		responseField.ErrorMsg = "EngineStarted";
		m_pServiceResponser->SendResponse(0, &responseField, &(responseField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case OUTFOG_TYPE_MANAGER:
	{
		CErrorField responseField;
		if (!strcmp(pOutFogReqRef->SubIndexName, "DumpMDB"))
		{
			//如果有多个程序向snap目录dump文件的话，要避免同时写，造成数据混乱
			if (GetConfigString(INI_FILE_NAME, "MannualDump") == "yes")
			{
				if (snapAllMDB(pOutFogReqRef->SequenceNo + 1, pOutFogReqRef->IsReplay >= 0))
				{
					responseField.ErrorNo = 0;
					responseField.ErrorMsg = "DumpMDB_Success";
				}
				else
				{
					char* errMsg = NULL;
					int errorCode = 0;
					m_pErrorEngine->getLastError(&errorCode, &errMsg);
					responseField.ErrorNo = errorCode;
					responseField.ErrorMsg = errMsg;
				}
			}
			else
			{
				responseField.ErrorNo = 0;
				responseField.ErrorMsg = "DumpMDB_NoNeedToDump";
			}
			m_pServiceResponser->SendResponse(0, &responseField, &(responseField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		else
		{
			responseField.ErrorNo = 999;
			responseField.ErrorMsg = "UnknownCommand";
			m_pServiceResponser->SendResponse(0, &responseField, &(responseField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE, 999, "UnknownCommand");
		}
		break;
	}
	case OUTFOG_TYPE_LOOP:
	{
		SnapSave();
		break;
	}
	case OUTFOG_TYPE_REDOOVER:
	{
		if (g_bOnlyRedo)
		{
			REPORT_EVENT(LOG_CRITICAL, "OnlyRedo", "Redo Over,Exit!");
			exit(-1);
		}
		break;
	}
	default:
	{
		m_gBusinessSequence.getNextValueByDouble();
		handleMessage(pOutFogReqRef, pStream, pSession);
	}
	}

	//由于一个package中有多个field，会被多次重复执行，所以大于 m_nLastDumpNo
	//反演中、备系统运行状态一定dump
	if (pOutFogReqRef->IsReplay >= 0)
	{
		if (g_bOnlyRedo)
		{
			if (g_nEndID > 0 && pOutFogReqRef->SequenceNo >= g_nEndID)
			{
				g_bSnapName = "redo";
				snapMDB(pOutFogReqRef->SequenceNo + 1, true);
				exit(-1);
			}

			if (g_bSnapName != "")
			{
				g_bSnapName += ".redo";
				snapMDB(pOutFogReqRef->SequenceNo + 1, true);
			}
			else if (fmod(pOutFogReqRef->SequenceNo + 1, m_nSnapCount) == 0)
			{
				g_bSnapName = "redo";
				snapMDB(pOutFogReqRef->SequenceNo + 1, true);
				//exit(-1);
			}
			else if (fmod(pOutFogReqRef->SequenceNo + 1, m_nSnapCountRedo) == 0)
			{
				//redo的時候需要比較inc，但是inc需要和之前正常运行一致，如果redo的snap也打印一次就会出现很难比较的情况，这里不打印
				snapRedoMDB(pOutFogReqRef->SequenceNo + 1);
				//exit(-1);
			}
		}
		else
		{
			if (g_bSnapName != "")
				snapMDB(pOutFogReqRef->SequenceNo + 1, true);
			else if (fmod(pOutFogReqRef->SequenceNo + 1, m_nSnapCount) == 0)
				snapMDB(pOutFogReqRef->SequenceNo + 1, true);
		}
	}
	else
	{
		if (g_bSnapName != "") //大概率是结算，这种情况下只能强制落地，要不然业务无法开展
			snapMDB(pOutFogReqRef->SequenceNo + 1, true);
		else if (fmod(pOutFogReqRef->SequenceNo + 1, m_nSnapCount) == 0)
			snapMDB(pOutFogReqRef->SequenceNo + 1);
	}

	//统一清理DeleteTriggerOrder与DeleteOrder
	if (!gDeleteOrderSet.empty())
	{
		set<COrder*>::iterator oit;
		for (oit = gDeleteOrderSet.begin(); oit != gDeleteOrderSet.end(); oit++)
		{
			m_pMDB->m_OrderFactory->remove(*oit, NULL, false);
		}
		gDeleteOrderSet.clear();
	}

	if (!gDeleteTriggerOrderSet.empty())
	{
		set<CTriggerOrder*>::iterator toit;
		for (toit = gDeleteTriggerOrderSet.begin(); toit != gDeleteTriggerOrderSet.end(); toit++)
		{
			m_pMDB->m_TriggerOrderFactory->remove(*toit, NULL, false);
		}
		gDeleteTriggerOrderSet.clear();
	}

	if (!gDeletePositionSet.empty())
	{
		set<CPosition*>::iterator toit;
		for (toit = gDeletePositionSet.begin(); toit != gDeletePositionSet.end(); toit++)
		{
			m_pMDB->m_PositionFactory->remove(*toit, NULL, false);
		}
		gDeletePositionSet.clear();
	}

	return true;
}

const char* CTradeEngine::getVersion()
{
	static char version[256];
	sprintf(version, "Service version: %s", "trade");
	return version;
}

bool CTradeEngine::SendTable(CFieldDescribe *pFieldDesc, const void *pRspBuffer)
{
	TOutFogMetaRef OutFogMetaRef;
	memset(&OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_KERNEL;
	//OutFogMetaRef.FrontNo = 0;
	//OutFogMetaRef.SessionNo = 0;
	OutFogMetaRef.MessageNo = TID_SendInit;
	OutFogMetaRef.Type = OUTFOG_TYPE_INSERT;
	OutFogMetaRef.FieldNo = pFieldDesc->m_FieldID;
	OutFogMetaRef.Length = pFieldDesc->m_nStreamSize;
	OutFogMetaRef.Remark[0] = OUTFOG_REMARK_TOKEN_MANAGER;
	pFieldDesc->setSingleField(m_pStreamBuf, pRspBuffer);
	handleMessage(&OutFogMetaRef, m_pStreamBuf, NULL);
	return true;
}

void CTradeEngine::handleMessage(const TOutFogMetaRef* pOutFogReqRef, const void *pStream, TSessionRef* pSession)
{
	CEngine::handleMessage(pOutFogReqRef, pStream, pSession);
}

void CTradeEngine::setServiceCallback(COutFogServiceCallback *pCallback)
{
	m_pServiceCallback = pCallback;
}

bool CTradeEngine::SnapSave()
{
	return m_pMDB->saveSnap();
}

bool CTradeEngine::LoadFromCSV(const char *pszCSVPath, UF_INT8 nBeginID)
{
	m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_KERNEL;
	m_OutFogMetaRef.FrontNo = 0;
	//m_OutFogMetaRef.ServiceNo = 0;

	char szFileName[1000];
	TTinitTableInfo *pInfo = NULL;
	//ToDo:dump.-1完善之后再做

	pInfo = g_InitTableInfo;

	CCurrentTimeField field;
	m_OutFogMetaRef.MessageNo = TID_DataSyncStart;
	m_OutFogMetaRef.FieldNo = CCurrentTimeField::m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = CCurrentTimeField::m_Describe.m_nStreamSize;
	CCurrentTimeField::m_Describe.setSingleField(m_pStreamBuf, &field);

	//ReqField(&m_OutFogMetaRef, m_pStreamBuf);
	handleMessage(&m_OutFogMetaRef, m_pStreamBuf, NULL);

	while (pInfo->pFieldDescribe != NULL)
	{
		sprintf(szFileName, "%s%c%s.csv", pszCSVPath, PATH_SPLIT, pInfo->pszTable);
		//LoadTable(szFileName, pInfo->pFieldDescribe);
		LoadTable(szFileName, pInfo->pFieldDescribe, this);
		pInfo++;
	}

	//TID_DataSyncEnd在第一个报文处理之前，利用报文的序号，把变化让下游感知到

	PRINT_TO_STD("Load Init Data OK");
	return true;
}

bool CTradeEngine::UpdateSequence()
{
	CCurrentTime *pCurrentTime = m_pMDB->m_CurrentTimeFactory->getFirst();
	CWriteableCurrentTime theCurrentTime;
	m_pMDB->m_CurrentTimeFactory->retrieve(pCurrentTime, &theCurrentTime);
	theCurrentTime.LastAccountDetailID = m_gAccountDetailSequence.getCurrentValueByString();
	theCurrentTime.LastOrderID = m_gOrderSequence.getCurrentValueByString();
	theCurrentTime.LastTradeID = m_gTradeSequence.getCurrentValueByString();
	theCurrentTime.LastTimeSortNo = m_gSortSequence.getCurrentValueByDouble();
	theCurrentTime.LastBusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_CurrentTimeFactory->update(pCurrentTime, &theCurrentTime);

	m_pTradeService->UpdateSequence();
	return true;
}

bool CTradeEngine::snapMDB(UF_INT8 nSequenceNo, bool bImmediately)
{
	if (nSequenceNo <= m_nLastDumpNo)
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "nSequenceNo[%0.f]<=m_nLastDumpNo[%0.f]", nSequenceNo, m_nLastDumpNo);

	char szPath[256];

	if (g_bSnapName == "")
	{
		sprintf(szPath, "%s/snap.%.0f/", sSnapPath.c_str(), nSequenceNo);
	}
	else
	{
		sprintf(szPath, "%s/snap.%.0f.%s/", sSnapPath.c_str(), nSequenceNo, g_bSnapName.c_str());
		g_bSnapName = "";
	}

	UpdateSequence();
	m_pServiceResponser->setdumpNo(nSequenceNo);
	m_nLastDumpNo = nSequenceNo;
	if (bImmediately)
	{
		if (g_bCompare)
			m_pMDB->savePreVersion(szPath, nSequenceNo);
		else
			m_pMDB->save(szPath, nSequenceNo);
	}
	else
	{
		if (!m_pMDB->snap(szPath, nSequenceNo))
			return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "MDBSnapStatusIsNot0");
	}
	return true;
}

bool CTradeEngine::snapRedoMDB(UF_INT8 nSequenceNo)
{
	if (nSequenceNo <= m_nLastDumpNo)
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "nSequenceNo[%0.f]<=m_nLastDumpNo[%0.f]", nSequenceNo, m_nLastDumpNo);

	char szPath[256];

	sprintf(szPath, "%s/snap.%.0f.redo/", sSnapPath.c_str(), nSequenceNo);

	UpdateSequence();
	m_pServiceResponser->setdumpNo(nSequenceNo);
	m_nLastDumpNo = nSequenceNo;

	if (g_bCompare)
		m_pMDB->savePreVersion(szPath, nSequenceNo);
	else
		m_pMDB->save(szPath, nSequenceNo);

	return true;
}

bool CTradeEngine::snapAllMDB(UF_INT8 nSequenceNo, bool bImmediately)
{
	if (nSequenceNo <= m_nLastDumpNo)
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "nSequenceNo[%0.f]<=m_nLastDumpNo[%0.f]", nSequenceNo, m_nLastDumpNo);

	char szPath[256];

	if (g_bSnapName == "")
	{
		sprintf(szPath, "%s/snap.%.0f/", sSnapPath.c_str(), nSequenceNo);
	}
	else
	{
		sprintf(szPath, "%s/snap.%.0f.%s/", sSnapPath.c_str(), nSequenceNo, g_bSnapName.c_str());
		g_bSnapName = "";
	}

	UpdateSequence();
	m_pServiceResponser->setdumpNo(nSequenceNo);
	m_nLastDumpNo = nSequenceNo;
	if (bImmediately)
	{
		if (g_bCompare)
			m_pMDB->saveAllPreVersion(szPath, nSequenceNo);
		else
			m_pMDB->saveAll(szPath, nSequenceNo);
	}
	else
	{
		if (!m_pMDB->snapAll(szPath, nSequenceNo))
			return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "MDBSnapStatusIsNot0");
	}
	return true;
}

void CTradeEngine::Timer(const TOutFogMetaRef* pOutFogReqRef, CCurrentTime *pOldTime, CCurrentTime *pNewTime)
{
	if (pOldTime->CurrTime != pNewTime->CurrTime)
	{
		//超过1s需要开始
		m_pServiceResponser->SendDelay();
	}

	//分钟级别的才需要定时器
	if (pOldTime->ZoneTime.getValue()[6] == '5' && pOldTime->ZoneTime.getValue()[7] == '9' &&
		pNewTime->ZoneTime.getValue()[6] == '0' && pNewTime->ZoneTime.getValue()[7] == '0')
	{
		CServiceConfig* pServiceConfig = m_pMDB->m_ServiceConfigFactory->findFirstByIndex1("TimerDay", pNewTime->ZoneTime.getValue());
		while (pServiceConfig != NULL)
		{
			if (pServiceConfig->ConfigValue == "FundingRate")
			{
				CWriteableSettleDetail theSettleDetail;
				theSettleDetail.init();
				theSettleDetail.SettleDetailID = m_gTradeSequence.getNextValueByString();
				theSettleDetail.SettleAction = SAT_FundingRate;
				vector<string> remarks = Txtsplit(pServiceConfig->Remark.getString(), ";");
				for (int i = 0; i < remarks.size(); i++)
				{
					vector<string> item = Txtsplit(remarks[i], ":");
					if (item.size() != 2)
						item = Txtsplit(remarks[i], "=");
					if (item.size() == 2)
					{
						if (item[0] == "Gap")
							theSettleDetail.Value3 = atoi(item[1].c_str());
						if (item[0] == "FundingRateGroup")
							theSettleDetail.FundingRateGroup = item[1].c_str();
					}
				}
				if (!theSettleDetail.FundingRateGroup.isNull())
				{
					m_pSettlementService->FundingRate(&theSettleDetail);
				}					
				else
				{
					//theSettleDetail.Remark = "Error:FundingRateGroup_Is_Null";
					//m_pMDB->m_SettleDetailFactory->add(&theSettleDetail);
					pServiceConfig->dumpString(m_sTemp);
					PRINT_TO_STD("Error:FundingRateGroup_Is_Null:%s", m_sTemp);
				}
			}
			else if (pServiceConfig->ConfigValue == "Settlement")
			{
				CWriteableSettleDetail theSettleDetail;
				theSettleDetail.init();
				theSettleDetail.SettleDetailID = m_gTradeSequence.getNextValueByString();
				theSettleDetail.SettleAction = SAT_Settlement;
				vector<string> remarks = Txtsplit(pServiceConfig->Remark.getString(), ";");
				for (int i = 0; i < remarks.size(); i++)
				{
					vector<string> item = Txtsplit(remarks[i], ":");
					if (item.size() != 2)
						item = Txtsplit(remarks[i], "=");
					if (item.size() == 2)
					{
						if (item[0] == "Gap")
							theSettleDetail.Value3 = atoi(item[1].c_str());
						if (item[0] == "ExchangeID")
							theSettleDetail.ExchangeID = item[1].c_str();
						if (item[0] == "InstrumentID")
							theSettleDetail.InstrumentID = item[1].c_str();
						if (item[0] == "SettlementGroup")
							theSettleDetail.SettlementGroup = item[1].c_str();
					}
				}
				m_pSettlementService->Settlement(&theSettleDetail, pOutFogReqRef->SequenceNo);
			}
			else if (pServiceConfig->ConfigValue == "AccountInit")
			{
				CWriteableSettleDetail theSettleDetail;
				theSettleDetail.init();
				theSettleDetail.SettleDetailID = m_gTradeSequence.getNextValueByString();
				theSettleDetail.SettleAction = SAT_AccountInit;
				vector<string> remarks = Txtsplit(pServiceConfig->Remark.getString(), ";");
				for (int i = 0; i < remarks.size(); i++)
				{
					vector<string> item = Txtsplit(remarks[i], ":");
					if (item.size() != 2)
						item = Txtsplit(remarks[i], "=");
					if (item.size() == 2)
					{
						if (item[0] == "SettlementGroup")
							theSettleDetail.SettlementGroup = item[1].c_str();
					}
				}
				m_pSettlementService->Settlement(&theSettleDetail, pOutFogReqRef->SequenceNo);
			}
			else if (pServiceConfig->ConfigValue == "ClearExpire")
			{

			}
			else if (pServiceConfig->ConfigValue == "LoanInterest")
			{
				CWriteableSettleDetail theSettleDetail;
				theSettleDetail.init();
				theSettleDetail.SettleDetailID = m_gTradeSequence.getNextValueByString();
				theSettleDetail.SettleAction = SAT_LoanInterest;
				theSettleDetail.Remark = pServiceConfig->Index2;
				if (pServiceConfig->Index2 == "i:1h")
					m_pSettlementService->LoanInterest(&theSettleDetail, 1.0 / 24.0 / 365.0);
				else if (pServiceConfig->Index2 == "i:1d")
					m_pSettlementService->LoanInterest(&theSettleDetail, 1.0 / 365.0);
				break;
			}
			pServiceConfig = m_pMDB->m_ServiceConfigFactory->findNextByIndex1();
		}
	}
}

//bool CTradeEngine::LoadTable(const char *pszCSVFileName, CFieldDescribe *pFieldDesc)
//{
//	string m_strErrorMsg = pszCSVFileName;
//	m_strErrorMsg += ":";
//
//	CFieldAnalysis record;
//
//	FILE *fp = mfopen(pszCSVFileName, "rb", false);
//	if (fp == NULL)
//	{
//		m_strErrorMsg += "Can't open csv file";
//		return true;
//	}
//
//	char bufRead[MAX_LINE_LEGTH];
//	if (fgets(bufRead, MAX_LINE_LEGTH, fp) == NULL)
//	{
//		m_strErrorMsg += "Missing csv file header";
//		fclose(fp);
//		return false;
//	}
//
//	if (!record.AnalysisFieldName(bufRead))
//	{
//		m_strErrorMsg += "Anlysis csv file header error";
//		fclose(fp);
//		return false;
//	}
//
//	char bufStruct[MAX_LINE_LEGTH];
//	bool bRet = true;
//	int iLine = 1;
//	char tmpBuf[OUTFOG_MAX_PACKAGE_SIZE] = { 0 };
//	while (fgets(bufRead, MAX_LINE_LEGTH, fp) != NULL)
//	{
//		if (!record.AnalysisFieldContent(bufRead))
//		{
//			m_strErrorMsg += "Anlysis csv file content error";
//			bRet = false;
//			break;
//		}
//		if (!record.TransToStruct(pFieldDesc, bufStruct))
//		{
//			memset(tmpBuf, 0x00, sizeof(tmpBuf));
//			sprintf(tmpBuf, "，第%d行数据错误：缺少字段或者字段多出", iLine);
//			m_strErrorMsg += tmpBuf;
//			bRet = false;
//			break;
//		}
//
//		m_OutFogMetaRef.MessageNo = TID_SendInit;
//		m_OutFogMetaRef.FieldNo = pFieldDesc->m_FieldID;
//		m_OutFogMetaRef.Length = pFieldDesc->m_nStreamSize;
//		pFieldDesc->setSingleField(m_pStreamBuf, bufStruct);
//		//ReqField(&m_OutFogMetaRef, m_pStreamBuf);
//		handleMessage(&m_OutFogMetaRef, m_pStreamBuf);
//		iLine++;
//	}
//	if (bRet)
//	{
//		memset(tmpBuf, 0x00, sizeof(tmpBuf));
//		sprintf(tmpBuf, " Load Records: %d", iLine - 1);
//		m_strErrorMsg += " Success,";
//		m_strErrorMsg += tmpBuf;
//	}
//	else
//	{
//		REPORT_EVENT(LOG_CRITICAL, "Init", "LoadTableError %s", m_strErrorMsg.c_str());
//	}	
//
//	fclose(fp);
//	return bRet;
//}

//void CTradeEngine::PushTriggerTables()
//{
//	m_pServiceResponser->GetMetaRef()->TNo = OUTFOG_TNO_NULL;
//	m_pServiceResponser->GetMetaRef()->SubjectNo = OUTFOG_SUBJECTID_NULL;
//	CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->getFirst();
//	while (pCurrency != NULL)
//	{
//		m_pServiceResponser->SimplePrepareMetaRef(&(CCurrencyField::m_Describe), OUTFOG_TYPE_INSERT);
//		m_SyncTriggers->m_pCurrencyTrigger->commitDo(pCurrency);
//		pCurrency = m_pMDB->m_CurrencyFactory->getNext();
//	}
//	m_pMDB->m_CurrencyFactory->endGet();
//}

//void CTradeEngine::PushAccountPosition()
//{
//	m_pServiceResponser->GetMetaRef()->TNo = OUTFOG_TNO_NULL;
//	m_pServiceResponser->GetMetaRef()->SubjectNo = OUTFOG_SUBJECTID_NULL;
//	CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
//	while (pPosition != NULL)
//	{
//		if (!needSyncDB(m_pMDB, pPosition->MemberID))
//		{
//			m_pServiceResponser->SimplePrepareMetaRef(&(CPositionField::m_Describe), OUTFOG_TYPE_INSUPD);
//			m_SyncTriggers->m_pPositionTrigger->commitDo(pPosition);
//		}
//		pPosition = m_pMDB->m_PositionFactory->getNext();
//	}
//	m_pMDB->m_PositionFactory->endGet();
//
//	CMember *pMember = m_pMDB->m_MemberFactory->getFirst();
//	while (pMember != NULL)
//	{
//		if (pMember->Grade == "X")
//		{
//			CAccount* pAccount = m_pMDB->m_AccountFactory->findFirstByMemberID(pMember->MemberID);
//			while (pAccount != NULL)
//			{
//				m_pServiceResponser->SimplePrepareMetaRef(&(CAccountField::m_Describe), OUTFOG_TYPE_INSUPD);
//				m_SyncTriggers->m_pAccountTrigger->commitDo(pAccount);
//				pAccount = m_pMDB->m_AccountFactory->findNextByMemberID();
//			}
//			m_pMDB->m_AccountFactory->endFindByMemberID();
//		}
//		pMember = m_pMDB->m_MemberFactory->getNext();
//	}
//	m_pMDB->m_MemberFactory->endGet();
//
//	CLastKLine* pLastKLine = m_pMDB->m_LastKLineFactory->getFirst();
//	while (pLastKLine != NULL)
//	{
//		m_pServiceResponser->SimplePrepareMetaRef(&(CLastKLineField::m_Describe), OUTFOG_TYPE_INSUPD);
//		m_SyncTriggers->m_pLastKLineTrigger->commitDo(pLastKLine);
//		pLastKLine = m_pMDB->m_LastKLineFactory->getNext();
//	}
//	m_pMDB->m_LastKLineFactory->endGet();
//
//	COrder* pOrder = m_pMDB->m_OrderFactory->getFirst();
//	while (pOrder != NULL)
//	{
//		if (!needSyncDB(m_pMDB, pOrder->MemberID))
//		{
//			m_pServiceResponser->SimplePrepareMetaRef(&(COrderField::m_Describe), OUTFOG_TYPE_INSUPD);
//			m_SyncTriggers->m_pOrderTrigger->commitDo(pOrder);
//		}
//		pOrder = m_pMDB->m_OrderFactory->getNext();
//	}
//	m_pMDB->m_OrderFactory->endGet();
//
//	CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
//	while (pMarketData != NULL)
//	{
//		m_pServiceResponser->SimplePrepareMetaRef(&(CMarketDataField::m_Describe), OUTFOG_TYPE_INSUPD);
//		m_SyncTriggers->m_pMarketDataTrigger->commitDo(pMarketData);
//		pMarketData = m_pMDB->m_MarketDataFactory->getNext();
//	}
//	m_pMDB->m_MarketDataFactory->endGet();
//
//	CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getFirst();
//	while (pTriggerOrder != NULL)
//	{
//		if (pTriggerOrder->TriggerPrice.isNull())
//		{
//			m_pServiceResponser->SimplePrepareMetaRef(&(CTriggerOrderField::m_Describe), OUTFOG_TYPE_INSUPD);
//			m_SyncTriggers->m_pTriggerOrderTrigger->commitDo(pTriggerOrder);
//		}
//		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getNext();
//	}
//	m_pMDB->m_TriggerOrderFactory->endGet();
//}

//extern "C" COutFogServiceApi* CreateOutFogServiceApi(COutFogServiceCallback *pCallback, int nServiceID)
//{
//	CConfig *pConfig = new CConfig("trade.ini");
//	return new CTradeEngine(pCallback, pConfig);
//}