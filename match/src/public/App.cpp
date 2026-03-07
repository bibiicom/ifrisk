#include "App.h"
#include "public.h"
#include "CopyField.h"
#include "mdbStruct.h"
#include "ServiceShare.h"
#include "printinfo.h"
#include "monitorIndex.h"
#include "CFilelogService.h"

void CmdLineArgError()
{
	printf("Usage: public 0 1 ... n|-h|-v");
	printf("       public -v      :	Show Version\n");
	printf("       public -h      :	Show Manual\n");
	printf("       public 1       :	start public at 1 \n");
	exit(0);
}

TDBMethod g_HelperTaskMethod[]
{
	TASK_METHOD_DEFINE(CKLineField, TDB_TABLE, "t_D_KLine", "ExchangeID,InstrumentID,Bar,InsertTime", new CDataContainer<CKLineField>()),
	TASK_METHOD_END
};

CMyEngine::CMyEngine()
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

	m_nLoadKLineCount = GetConfigInt(INI_FILE_NAME, "LoadKLineCount");
	PRINT_TO_STD("LoadKLineCount:%d", m_nLoadKLineCount);
	if (g_nMDSnapDepth > 0)
	{
		string dbName = GetConfigString(NetworkConfigFile, "dbname");
		string userName = GetConfigString(NetworkConfigFile, "dbuser");
		string pwd = GetConfigString(NetworkConfigFile, "dbpasswd");

		m_pDbHelper = NULL;

		//获取KLine到内存，这里运行与否不影响后续结果，因为后续会更加K线的订阅判断是否有数据到数据库继续补充
		if (m_nLoadKLineCount > 0)
		{
			m_pDbHelper = new CDbHelper(dbName, userName, pwd);
			m_pDbHelper->ConnectDB();
			vector<string> Periods;
			Periods.push_back("1m");
			Periods.push_back("5m");
			Periods.push_back("1h");
			Periods.push_back("1d");
			Periods.push_back("1w");
			Periods.push_back("1o");

			//预处理加载为了加快速度，只能加载最近的200条数据，后续在用户发出订阅请求的时候如果数据库没有再继续加载
			vector<double> BeginTime;
			//能支持的m_nLoadKLineCount数目的K线为：
			//1m,2m,3m,4m
			//5m,10m,15m,20m,25m,30m
			//1h,2h,3h,4h,5h,6h,7h,8h,9h,10h,11h,12h
			//1d,2d,3d,4d,5d
			//1w
			//1o,2o,3o,4o,5o,6o
			//再往上没有意义，假设m_nLoadKLineCount是100，不可能保存100*6o=50年的K线吧
			//如果不是以上周期，其数目会收到 x周期/基本周期 的数目限制
			double nowDouble = time(NULL);
			nowDouble = nowDouble * 1000;
			BeginTime.push_back(nowDouble - 4.0 * MAX(m_nLoadKLineCount, 360) * 60 * 1000);
			BeginTime.push_back(nowDouble - 6.0 * m_nLoadKLineCount * 5 * 60 * 1000);
			BeginTime.push_back(nowDouble - 12.0 * m_nLoadKLineCount * 60 * 60 * 1000);
			BeginTime.push_back(nowDouble - 5.0 * m_nLoadKLineCount * 24 * 60 * 60 * 1000);
			BeginTime.push_back(nowDouble - 5.0 * m_nLoadKLineCount * 7 * 24 * 60 * 60 * 1000);
			BeginTime.push_back(nowDouble - 6.0 * m_nLoadKLineCount * 31 * 24 * 60 * 60 * 1000);

			char szFilter[512] = {};
			for (int i = 0; i < Periods.size(); i++)
			{
				double EachEndTime = time(NULL);
				EachEndTime = EachEndTime * 1000;
				while (EachEndTime >= BeginTime[i])
				{
					//time_t在32位机器linux和windows无法统一打印
					sprintf(szFilter, "Bar='%s' and InsertTime >= %0.f and InsertTime < %0.f and InstrumentID not like 'm%%' and InstrumentID not like 'i%%' order by InsertTime desc limit 10000", Periods[i].c_str(), BeginTime[i], EachEndTime);
					PRINT_TO_STD("%s", szFilter);
					CBaseDataContainer* pDataContainer = m_pDbHelper->Select("t_D_KLine", szFilter);
					if (pDataContainer == NULL)
						break;
					if (pDataContainer->size() == 0)
						break;
					for (size_t i = 0; i < pDataContainer->size(); i++)
					{
						CKLineField* pKLineField = (CKLineField*)pDataContainer->get(i);
						//printf("%s:%s:%d\n", pKLineField->Bar.getValue(), pKLineField->InstrumentID.getValue(), pKLineField->BeginTime);
						CWriteablePublishKLine theKLine;
						theKLine.init();
						CopyPublishKLineEntity(&theKLine, pKLineField);
						m_pMDB->m_PublishKLineFactory->add(&theKLine);
						Init24(&theKLine, nowDouble);
						EachEndTime = theKLine.InsertTime;
					}
					SLEEP(200);		//不能让数据库压力过大
				}
			}
		}
	}

	//m_pMDB->saveAll("snap/snap.100000/");
}

CMyEngine::~CMyEngine()
{
}

bool CMyEngine::RspField(const TOutFogMetaRef* pOutFogRspRef, const void* pRspBuffer)
{
	if (pOutFogRspRef->IsReplay >= 0)
		return true;

	SessionBase.frontID = pOutFogRspRef->FrontNo;
	SessionBase.sessionID = pOutFogRspRef->SessionNo;
	SessionBase.requestID = pOutFogRspRef->RequestNo;
	SessionBase.pToken = pOutFogRspRef->Remark;
	SessionBase.pUserID = pOutFogRspRef->UserID;
	SessionBase.zip = pOutFogRspRef->SubjectNo;

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
			m_pServiceResponser->SendResponse(0, &responseField, &(responseField.m_Describe), &SessionBase, OUTFOG_CHAIN_SINGLE, 0, "DumpMDB_Success");
			return true;
		}
	}
	case OUTFOG_TYPE_ADDSESSION:
	{
		//如果系统没有准备好，只能直接断开链接,50次100ms差不多5秒之后启动连接
		if (m_nReady < 5)
		{
			m_pServiceResponser->SendResponse(0, NULL, NULL, &SessionBase, OUTFOG_CHAIN_SINGLE, ERROR_SystemNotReady, "SystemNotReady!");
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

	int nRecvTNo = 0;
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
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &SessionBase);
		}

		//if (pUserSession->UserID != field.UserID)
		//{
		//	m_pErrorEngine->reportError(ERROR_USER_MISMATCH);
		//	return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, pSession);
		//}
		CWriteableUserSession UserSession;
		m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &UserSession);
		UserSession.FrontNo = SessionBase.frontID;
		UserSession.SessionNo = SessionBase.sessionID;
		UserSession.ExpireTime = m_CurrentTimeField.CurrTime.getValue() + 60 * 60 * 24 * 2;
		m_pMDB->m_UserSessionFactory->update(pUserSession, &UserSession);
		CUserSessionField userSessionField;
		CopyUserSessionEntity(&userSessionField, pUserSession);
		m_pServiceResponser->SendResponse(TID_RecvUserLogin, &userSessionField, &(userSessionField.m_Describe), &SessionBase, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendTopicAction:
	{
		if (m_nReady < 5)
		{
			m_pServiceResponser->SendResponse(0, NULL, NULL, &SessionBase, OUTFOG_CHAIN_SINGLE, ERROR_SystemNotReady, "SystemNotReady!");
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
			int nReaderTimes = TestTopicAction(&field, &SessionBase);
			if (nReaderTimes <= 0)
				return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &SessionBase, &field, &(field.m_Describe));
			m_pServiceResponser->SendResponse(TID_RecvTopicAction, &field, &(field.m_Describe), &SessionBase, OUTFOG_CHAIN_SINGLE);
			RspTopicAction(&field, &SessionBase);
			m_pServiceResponser->SubSubjectID(nRecvTNo, field.TopicID.getInt(), field.ResumeNo.getValue(), field.Index.getValue(), &SessionBase, field.Zip.getValue(), UF_INT8_MAX, nReaderTimes);
			break;
		}
		case SA_UnSub:
		{
			int nTopicNo = atoi(field.TopicID.getValue());
			vector<string> vFilters = Txtsplit(field.Index.getValue(), "_");
			switch (nTopicNo)
			{
			case OF_Topic_Market_LastKLine:
			{
				field.Index = vFilters[0].c_str();
				field.Index += "_";
				field.Index += vFilters[1].c_str();
				field.Index += "_1m";
				break;
			}
			case OF_Topic_Market_Depth_25:
			case OF_Topic_Market_SnapShot:
			{
				if (vFilters.size() == 3)
				{
					map<CExchangeIDType, InstDescMap>::iterator iter = m_pMarketDataService->m_InstDescMap.find(vFilters[0].c_str());
					if (iter == m_pMarketDataService->m_InstDescMap.end())
						return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "NotFoundExchangeID");
					InstDescMap::iterator iter1 = iter->second.find(vFilters[1].c_str());
					if (iter1 == iter->second.end())
						return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "NotFoundInstrumentID");
					CPriceType Tick = vFilters[2].c_str();
					if (Tick == iter1->second.desc)
					{
						field.Index = vFilters[0].c_str();
						field.Index += "_";
						field.Index += vFilters[1].c_str();
					}
					else
					{
						field.Index = vFilters[0].c_str();
						field.Index += "_";
						field.Index += vFilters[1].c_str();
						field.Index += "_";
						field.Index += Tick.getString().c_str();
					}
				}
				break;
			}
			}
			m_pServiceResponser->UnSubSubjectID(field.TopicID.getInt(), field.Index.getValue(), &SessionBase);
			m_pServiceResponser->SendResponse(TID_RecvTopicAction, &field, &(field.m_Describe), &SessionBase, OUTFOG_CHAIN_SINGLE);
			break;
		}
		case SA_Clear:
		{
			m_pServiceResponser->UnSubSubjectIDAll(&SessionBase);
			m_pServiceResponser->SendResponse(TID_RecvTopicAction, &field, &(field.m_Describe), &SessionBase, OUTFOG_CHAIN_SINGLE);
			break;
		}
		default:
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "action=%c", field.Action.getValue());
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &SessionBase, &field, &(field.m_Describe));
		}
		}
		break;
	}
	case TID_SendQryMarketOrder:
	{
		CQryMarketOrderField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		if (field.Tick.isNull())
			SendMarketOrder(m_pMDB, m_pServiceResponser, field, &SessionBase);
		else
			SendTickMarketOrder(m_pMDB, m_pServiceResponser, field, &SessionBase);
		return true;
	}
	case TID_SendQryMarketData:
	{
		CQryMarketDataField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		SendMarketData(m_pMDB, m_pServiceResponser, field, &SessionBase);
		return true;
	}
	case TID_SendQryCurrency:
	{
		CQryCurrencyField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		SendCurrency(m_pMDB, m_pServiceResponser, field, &SessionBase);
		return true;
	}
	case TID_SendQryInstrument:
	{
		CQryInstrumentField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		nRecvTNo = TID_RecvQryInstrument;
		CInstrumentField rspField;
		CInstrument* pData = NULL;
		if (!field.InstrumentID.isNull())
		{
			pData = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
			if (pData != NULL)
			{
				CopyInstrumentEntity(&rspField, pData);
				m_pServiceResponser->SendResponse(nRecvTNo, &rspField, &(CInstrumentField::m_Describe), &SessionBase, OUTFOG_CHAIN_SINGLE);
			}
			else
			{
				m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &SessionBase, OUTFOG_CHAIN_SINGLE);
			}
			break;
		}
		if (!field.ProductGroup.isNull())
		{
			m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &SessionBase, OUTFOG_CHAIN_FIRST);
			pData = m_pMDB->m_InstrumentFactory->findFirstByProductGroup(field.ProductGroup);
			while (pData != NULL)
			{
				CopyInstrumentEntity(&rspField, pData);
				m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
				pData = m_pMDB->m_InstrumentFactory->findNextByProductGroup();
			}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
			break;
		}
		m_pServiceResponser->SendResponse(nRecvTNo, NULL, NULL, &SessionBase, OUTFOG_CHAIN_FIRST);
		pData = m_pMDB->m_InstrumentFactory->getFirst();
		while (pData != NULL)
		{
			CopyInstrumentEntity(&rspField, pData);
			m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			pData = m_pMDB->m_InstrumentFactory->getNext();
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		return true;
	}
	case TID_PushMarketOrder:
	case TID_PushDelayMarketOrder:
	{
		//PRINT_TO_STD("MarketOrder[%s][%d][%d][%c][%0.f]", pOutFogRspRef->SubIndexName, pOutFogRspRef->SubjectNo, pOutFogRspRef->FieldNo, pOutFogRspRef->Chain, pOutFogRspRef->SequenceNo);
		if (pOutFogRspRef->FieldNo == FID_TickMarketOrder)
		{
			CTickMarketOrderField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);
			//PRINT_TO_STD("MarketOrder:[%s_%s],[%c|%0.4f/%0.5f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.Direction.getValue(), field.Price.getValue(), field.Volume.getValue());

			CTickMarketOrder* pTickMarketOrder = m_pMDB->m_TickMarketOrderFactory->findByPrice(field.ExchangeID, field.InstrumentID, field.Tick, field.Price);
			if (field.Orders <= 0)
			{
				m_pMDB->m_TickMarketOrderFactory->remove(pTickMarketOrder);
			}
			else
			{
				//删除错位的订单簿
				if (field.Direction == D_Buy)
				{
					CTickMarketOrder* pOtherTickMarketOrder = m_pMDB->m_TickMarketOrderFactory->findFirstBySellMBLData(field.ExchangeID, field.InstrumentID, field.Tick);
					while (pOtherTickMarketOrder != NULL)
					{
						if (pOtherTickMarketOrder->Price > field.Price)
							break;
						CTickMarketOrder* pTobeRemove = pOtherTickMarketOrder;
						pOtherTickMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextBySellMBLData();
						m_pMDB->m_TickMarketOrderFactory->remove(pTobeRemove);
					}
				}
				else
				{
					CTickMarketOrder* pOtherTickMarketOrder = m_pMDB->m_TickMarketOrderFactory->findFirstByBuyMBLData(field.ExchangeID, field.InstrumentID, field.Tick);
					while (pOtherTickMarketOrder != NULL)
					{
						if (pOtherTickMarketOrder->Price < field.Price)
							break;
						CTickMarketOrder* pTobeRemove = pOtherTickMarketOrder;
						pOtherTickMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextByBuyMBLData();
						m_pMDB->m_TickMarketOrderFactory->remove(pTobeRemove);
					}
				}

				CWriteableTickMarketOrder theTickMarketOrder;
				if (pTickMarketOrder == NULL)
				{
					CopyTickMarketOrderEntity(&theTickMarketOrder, &field);
					m_pMDB->m_TickMarketOrderFactory->add(&theTickMarketOrder);
				}
				else
				{
					m_pMDB->m_TickMarketOrderFactory->retrieve(pTickMarketOrder, &theTickMarketOrder);
					theTickMarketOrder.Volume = field.Volume;
					theTickMarketOrder.Orders = field.Orders;
					theTickMarketOrder.BusinessNo = field.BusinessNo;
					theTickMarketOrder.TrueVolume = field.TrueVolume;
					theTickMarketOrder.Tick = field.Tick;
					m_pMDB->m_TickMarketOrderFactory->update(pTickMarketOrder, &theTickMarketOrder);
				}
			}
		}
		else if (pOutFogRspRef->FieldNo == FID_MarketOrder)
		{
			CMarketOrderField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);
			//PRINT_TO_STD("MarketOrder:[%s_%s],[%c|%0.4f/%0.5f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.Direction.getValue(), field.Price.getValue(), field.Volume.getValue());

			CMarketOrder* pMarketOrder = m_pMDB->m_MarketOrderFactory->findByPrice(field.ExchangeID, field.InstrumentID, field.Price);

			if (pMarketOrder != NULL)
			{
				if (pMarketOrder->BusinessNo > field.BusinessNo)
					break;

				if (pMarketOrder->BusinessNo == field.BusinessNo)
				{
					if (pOutFogRspRef->MessageNo == TID_PushDelayMarketOrder)
						break;
				}
			}
			if (field.BusinessNo > g_nBusinessNo)
				g_nBusinessNo = field.BusinessNo;

			if (g_nMDSnapDepth > 0)
				UpgradeTickPrice(field, pMarketOrder);

			if (field.Orders <= 0)
			{
				m_pMDB->m_MarketOrderFactory->remove(pMarketOrder);
			}
			else
			{
				//删除错位的订单簿
				if (field.Direction == D_Buy)
				{
					CMarketOrder* pOtherMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(field.ExchangeID, field.InstrumentID);
					while (pOtherMarketOrder != NULL)
					{
						if (pOtherMarketOrder->Price > field.Price)
							break;
						CMarketOrder* pTobeRemove = pOtherMarketOrder;
						pOtherMarketOrder = m_pMDB->m_MarketOrderFactory->findNextBySellMBLData();
						m_pMDB->m_MarketOrderFactory->remove(pTobeRemove);
					}
				}
				else
				{
					CMarketOrder* pOtherMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(field.ExchangeID, field.InstrumentID);
					while (pOtherMarketOrder != NULL)
					{
						if (pOtherMarketOrder->Price < field.Price)
							break;
						CMarketOrder* pTobeRemove = pOtherMarketOrder;
						pOtherMarketOrder = m_pMDB->m_MarketOrderFactory->findNextByBuyMBLData();
						m_pMDB->m_MarketOrderFactory->remove(pTobeRemove);
					}
				}

				CWriteableMarketOrder theMarketOrder;
				if (pMarketOrder == NULL)
				{
					CopyMarketOrderEntity(&theMarketOrder, &field);
					m_pMDB->m_MarketOrderFactory->add(&theMarketOrder);
				}
				else
				{
					m_pMDB->m_MarketOrderFactory->retrieve(pMarketOrder, &theMarketOrder);
					theMarketOrder.Volume = field.Volume;
					theMarketOrder.Orders = field.Orders;
					theMarketOrder.BusinessNo = field.BusinessNo;
					theMarketOrder.TrueVolume = field.TrueVolume;
					m_pMDB->m_MarketOrderFactory->update(pMarketOrder, &theMarketOrder);
				}
			}
		}
		if (pOutFogRspRef->SubjectNo == OF_Topic_Market_Depth_All)
			return false;
		break;
	}
	case TID_PushMarketOverView:
	{
		CMarketOverViewField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if(pInstrument == NULL)
			return false;
		//PRINT_TO_STD("MarketOverView:[%s_%s],[%0.4f/%0.2f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.LastPrice.getValue(), field.Turnover.getValue());
		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		CWriteableMarketData theMarketData;
		bool bDelay = false;
		if (pMarketData == NULL)
		{
			theMarketData.init();
			CopyMarketOverViewEntity(&theMarketData, &field);
			theMarketData.HighestPrice24 = field.HighestPrice;
			theMarketData.LowestPrice24 = field.LowestPrice;
			theMarketData.Volume24 = field.Volume;
			theMarketData.Turnover24 = field.Turnover;
			theMarketData.OpenPrice24 = field.OpenPrice;
			bDelay = true;
			m_pMDB->m_MarketDataFactory->add(&theMarketData);
		}
		else
		{
			if (field.UpdateTime / 1000 > pMarketData->UpdateTime / 1000)
				bDelay = true;
			m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
			CopyMarketDataUpdateTimeEntity(&theMarketData, &field);
			CopyMarketDataLastMatchEntity(&theMarketData, &field);
			theMarketData.InstrumentStatus = field.InstrumentStatus;
			theMarketData.PrePositionFeeRate = field.PrePositionFeeRate;
			//theMarketData.PositionFeeTime = field.PositionFeeTime;
			//theMarketData.BidPrice1 = field.BidPrice1;
			//theMarketData.AskPrice1 = field.AskPrice1;

			//if (theMarketData.UpdateTime != pMarketData->UpdateTime && fmod(theMarketData.UpdateTime.getValue(), 2) == 0.0)
			//{
			//	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, field.ProductGroup);
			//	m_pServiceResponser->SendToSubjectID(TID_PushMarketOverView, &field, &(field.m_Describe), OF_Topic_Market_OverView);
			//}
			m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
		}
		field.HighestPrice24 = theMarketData.HighestPrice24;
		field.LowestPrice24 = theMarketData.LowestPrice24;
		field.Volume24 = theMarketData.Volume24;
		field.Turnover24 = theMarketData.Turnover24;
		field.OpenPrice24 = theMarketData.OpenPrice24;

		//m_pServiceResponser->GetMetaRef()->Chain = OUTFOG_CHAIN_SINGLE;
		//memcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, pOutFogRspRef->SubIndexName, 48);
		sprintf(m_pServiceResponser->GetMetaRef()->SubIndexName, "%s,%s_%s", theMarketData.ProductGroup.getValue(), theMarketData.ExchangeID.getValue(), theMarketData.InstrumentID.getValue());

		if (bDelay)
		{
			//m_pServiceResponser->GetMetaRef()->SubjectNo = OUTFOG_SUBJECTID_MULTI;
			sprintf(m_pServiceResponser->GetMetaRef()->Remark, "%d,%d", OF_Topic_Market_OverView, OF_Topic_Market_OverView_Delay);
			//m_pServiceResponser->GetMetaRef()->Direction = OUTFOG_DIRECTION_TO_FRONT;
			//m_pServiceResponser->GetMetaRef()->Type = OUTFOG_TYPE_PUBLISH;
			//m_pServiceResponser->GetMetaRef()->MessageNo = TID_PushMarketOverView;
			//m_pServiceResponser->GetMetaRef()->Chain = OUTFOG_CHAIN_SINGLE;
			//m_pServiceResponser->GetMetaRef()->FieldNo = field.m_Describe.m_FieldID;
			//m_pServiceResponser->GetMetaRef()->Length = field.m_Describe.m_nStreamSize;
			//CMarketOverViewField::m_Describe.setSingleField(m_pServiceResponser->GetStreamBuf(), &field);
			//m_pServiceResponser->GetCallback()->RspField(m_pServiceResponser->GetMetaRef(), m_pServiceResponser->GetStreamBuf());
			m_pServiceResponser->SendToSubjectID(TID_PushMarketOverView, &field, &(CMarketOverViewField::m_Describe), OUTFOG_SUBJECTID_MULTI, 3);
		}
		else
		{
			m_pServiceResponser->SendToSubjectID(TID_PushMarketOverView, &field, &(CMarketOverViewField::m_Describe), OF_Topic_Market_OverView, 3);
		}
		return false;
	}
	case TID_PushKLine:
	{
		if (pOutFogRspRef->FieldNo == CKLineField::m_Describe.m_FieldID)
		{
			CKLineField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);

			//PRINT_TO_STD("KLine:[%s_%s][%s|%s]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.BeginTime.getString().c_str(), field.Bar.getValue());
			CPublishKLine* pKLine = m_pMDB->m_PublishKLineFactory->findByInstrumentBar(field.ExchangeID, field.InstrumentID, field.Bar, field.InsertTime);
			//以数据库为准，trade流水里面推送的数据重复的不需要替换
			if (pKLine != NULL)
				break;

			CWriteablePublishKLine theKLine;
			theKLine.init();
			CopyPublishKLineEntity(&theKLine, &field);
			m_pMDB->m_PublishKLineFactory->add(&theKLine);

			if (theKLine.Bar == "1m")
			{
				double beginTime = theKLine.InsertTime;
				if (theKLine.Bar == "1m")
				{
					beginTime -= 4 * MAX(m_nLoadKLineCount, 360) * 60 * 1000;
				}
				else if (theKLine.Bar == "5m")
				{
					beginTime -= 6 * m_nLoadKLineCount * 5 * 60 * 1000;
				}
				else if (theKLine.Bar == "1h")
				{
					beginTime -= 12 * m_nLoadKLineCount * 60 * 60 * 1000;
				}
				if (pOutFogRspRef->Chain != OUTFOG_CHAIN_SINGLE)
					Init24(&theKLine, beginTime);

				//为什么以后的不做清理了呢
				//1d的K线运行一年是产生 365根K线，同时要满足5d的100根的需求的话，需要500天的运行才能开始剔除K线
				//我们认为，一个系统连续运行500天的可能性不大，如果是200根K线的话，需要1000天连续运行之后才开始剔除k线，那可能性基本没有
				//所以1d以上的K线不需要清理
				if (fmod(beginTime, 24 * 60 * 1000) == 0)
				{
					CPublishKLine* pKLine = m_pMDB->m_PublishKLineFactory->findFirstByBar(field.ExchangeID, field.InstrumentID, field.Bar);
					while (pKLine != NULL)
					{
						if (pKLine->InsertTime.getValue() >= beginTime)
							break;
						CPublishKLine* pKLineToRemove = pKLine;
						pKLine = m_pMDB->m_PublishKLineFactory->findNextByBar();
						m_pMDB->m_PublishKLineFactory->remove(pKLineToRemove);
					}
				}
			}
		}
		if (pOutFogRspRef->FieldNo == CLastKLineField::m_Describe.m_FieldID)
		{
			CLastKLineField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);
			//if (field.InstrumentID[0] == 'm')
			//	break;
			CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
			if(pInstrument == NULL)
				return false;
			CWriteableLastKLine theLastKLine;
			//PRINT_TO_STD("LastKLine:[%s_%s][%s|%s]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.BeginTime.getString().c_str(), field.Bar.getValue());
			CLastKLine* pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(field.ExchangeID, field.InstrumentID, field.Bar);
			if (pLastKLine == NULL)
			{
				theLastKLine.init();
				CopyLastKLineEntity(&theLastKLine, &field);
				m_pMDB->m_LastKLineFactory->add(&theLastKLine);

				if (field.Bar == "1m")
				{
					CWriteablePublishKLine theKLine;
					CopyPublishKLineEntity(&theKLine, &field);
					Init24(&theKLine, field.InsertTime);
				}
			}
			else
			{
				if (field.Bar == "1m")
				{
					CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
					if (pMarketData != NULL)
					{
						CWriteableMarketData theMarketData;
						m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
						if (pLastKLine->InsertTime != field.InsertTime)
						{
							CPublishKLine* pKLine24 = m_pMDB->m_PublishKLineFactory->findByInstrumentBar(field.ExchangeID, field.InstrumentID, field.Bar, field.InsertTime - 24 * 60 * 60 * 1000);
							if (pKLine24 != NULL)
							{
								theMarketData.Volume24 -= pKLine24->Volume;
								theMarketData.Turnover24 -= pKLine24->Turnover;
								theMarketData.OpenPrice24 = pKLine24->ClosePrice;

								if (theMarketData.HighestPrice24 == pKLine24->HighestPrice)
								{
									theMarketData.HighestPrice24 = field.HighestPrice;
									CPublishKLine* pKLineIter = m_pMDB->m_PublishKLineFactory->findFirstByBarInsertTime(field.ExchangeID, field.InstrumentID, field.Bar, field.InsertTime - 24 * 60 * 60 * 1000 + 60 * 1000);
									while (pKLineIter != NULL)
									{
										if (theMarketData.HighestPrice24 < pKLineIter->HighestPrice)
											theMarketData.HighestPrice24 = pKLineIter->HighestPrice;
										pKLineIter = m_pMDB->m_PublishKLineFactory->findNextByBarInsertTime();
									}
								}

								if (theMarketData.LowestPrice24 == pKLine24->LowestPrice)
								{
									theMarketData.LowestPrice24 = field.LowestPrice;
									CPublishKLine* pKLineIter = m_pMDB->m_PublishKLineFactory->findFirstByBarInsertTime(field.ExchangeID, field.InstrumentID, field.Bar, field.InsertTime - 24 * 60 * 60 * 1000 + 60 * 1000);
									while (pKLineIter != NULL)
									{
										if (theMarketData.LowestPrice24 > pKLineIter->LowestPrice)
											theMarketData.LowestPrice24 = pKLineIter->LowestPrice;
										pKLineIter = m_pMDB->m_PublishKLineFactory->findNextByBarInsertTime();
										pKLineIter = m_pMDB->m_PublishKLineFactory->findNextByBarInsertTime();
									}
								}
							}
							theMarketData.Volume24 += field.Volume;
							theMarketData.Turnover24 += field.Turnover;
						}
						else
						{
							theMarketData.Volume24 += field.Volume - pLastKLine->Volume;
							theMarketData.Turnover24 += field.Turnover - pLastKLine->Turnover;
							if (theMarketData.HighestPrice24 < field.HighestPrice)
								theMarketData.HighestPrice24 = field.HighestPrice;
							if (theMarketData.LowestPrice24 > field.LowestPrice)
								theMarketData.LowestPrice24 = field.LowestPrice;
						}
						m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
					}
				}
				m_pMDB->m_LastKLineFactory->retrieve(pLastKLine, &theLastKLine);
				CopyLastKLineEntity(&theLastKLine, &field);
				m_pMDB->m_LastKLineFactory->update(pLastKLine, &theLastKLine);
			}
		}
		((TOutFogMetaRef*)pOutFogRspRef)->Reserve[0] = '4';
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
			CLastKLine* pLastKLine = NULL;
			pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "1m");
			if (pLastKLine != NULL)
				m_pMDB->m_LastKLineFactory->remove(pLastKLine);
			pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "5m");
			if (pLastKLine != NULL)
				m_pMDB->m_LastKLineFactory->remove(pLastKLine);
			pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "1h");
			if (pLastKLine != NULL)
				m_pMDB->m_LastKLineFactory->remove(pLastKLine);
			pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "1d");
			if (pLastKLine != NULL)
				m_pMDB->m_LastKLineFactory->remove(pLastKLine);
			pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "1w");
			if (pLastKLine != NULL)
				m_pMDB->m_LastKLineFactory->remove(pLastKLine);
			pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "1o");
			if (pLastKLine != NULL)
				m_pMDB->m_LastKLineFactory->remove(pLastKLine);
			break;
		}

		InstDescInfo& descInfo = m_pMarketDataService->m_InstDescMap[field.ExchangeID][field.InstrumentID];
		descInfo.desc = field.PriceTick;
		vector<string> configs = Txtsplit(field.Remark.getBuffer(), ";");
		for (int i = 0; i < configs.size(); i++)
		{
			vector<string> pairs = Txtsplit(configs[i], "|");
			if (pairs.size() < 2)
				continue;
			if (pairs[0] != "Ticks")
				continue;
			for (int j = 1; j < pairs.size(); j++)
			{
				double tick = atof(pairs[j].c_str());
				descInfo.tick.push_back(tick);

				string SubIndexName = field.ExchangeID.getValue();
				SubIndexName += "_";
				SubIndexName += field.InstrumentID.getValue();
				SubIndexName += "_";
				CPriceType pricetick = tick;
				SubIndexName += pricetick.getString();
				descInfo.index.push_back(SubIndexName);
			}
		}
		if (descInfo.tick.size() == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				double tick = descInfo.desc * pow(10, i + 1);
				descInfo.tick.push_back(tick);
				string SubIndexName = field.ExchangeID.getValue();
				SubIndexName += "_";
				SubIndexName += field.InstrumentID.getValue();
				SubIndexName += "_";
				CPriceType pricetick = tick;
				SubIndexName += pricetick.getString();
				descInfo.index.push_back(SubIndexName);
				PRINT_TO_STD("Instrument:[%s_%s],Ticks[%s]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), doubleToStringFormat(tick).c_str());
			}
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
	case TID_PushTimeSync:
	{
		CMilliSecondsType CurrTime = m_CurrentTimeField.CurrTime;
		m_CurrentTimeField.m_Describe.getSingleField(&m_CurrentTimeField, pRspBuffer);
		if (CurrTime != m_CurrentTimeField.CurrTime)
			m_SubMap.clear();

		// PRINT_TO_STD_US("TID_PushTimeSync:%s,%0.f", m_CurrentTimeField.ZoneTime.getValue(), m_CurrentTimeField.UpdateTime.getValue());

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

		if (pOutFogRspRef->SubjectNo == OF_Topic_Market_Depth_All)
		{
			m_pMarketDataService->PublishMarketData(&m_CurrentTimeField);
			m_pMarketDataService->PublishSnapMarketData(&m_CurrentTimeField);
			m_pMarketDataService->PublishDelayMarketData(&m_CurrentTimeField);
		}
		break;
	}
	//case TID_PushOrder:
	//{
	//	COrderField field;
	//	field.m_Describe.getSingleField(&field, pRspBuffer);
	//	//PRINT_TO_STD("MarketOverView:[%s_%s],[%0.4f/%0.2f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.LastPrice.getValue(), field.Turnover.getValue());

	//	memcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, pOutFogRspRef->SubIndexName, 48);
	//	m_pServiceResponser->SendToSubjectID(TID_PushMarketOverView, &field, &(CMarketOverViewField::m_Describe), OF_Topic_Market_OverView);
	//	return false;
	//}
	case TID_SendQryKLine:
	{
		nRecvTNo = TID_RecvQryKLine;
		CQryKLineField qryField;
		CQryKLineField::m_Describe.getSingleField(&qryField, pRspBuffer);
		if (qryField.Limit > m_nLoadKLineCount)
			qryField.Limit = m_nLoadKLineCount;

		CBarType Bar = qryField.Bar;
		char baseBar[3];
		baseBar[2] = '\0';
		baseBar[1] = Bar.getBuffer()[Bar.getSize() - 1];
		Bar.getBuffer()[Bar.getSize() - 1] = '\0';
		int nBarTimes = atoi(Bar);
		baseBar[0] = '1';

		if (baseBar[1] == 'm' && nBarTimes % 5 == 0)
		{
			nBarTimes = nBarTimes / 5;
			baseBar[0] = '5';
		}

		CLastKLine* pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(qryField.ExchangeID, qryField.InstrumentID, baseBar);
		if (pLastKLine == NULL)
		{
			m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, qryField.InstrumentID.getValue());
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &SessionBase, &qryField, &(qryField.m_Describe));
		}

		//先取到历史的K线，内存中暂时存放的
		m_pServiceResponser->PrepareResponse(nRecvTNo, 0, "Success", &SessionBase);

		CKLineBaseField field;
		CopyKLineBaseEntity(&field, pLastKLine);
		int nLimit = 0;
		CPublishKLine* pKLine = m_pMDB->m_PublishKLineFactory->findFirstByBar(qryField.ExchangeID, qryField.InstrumentID, baseBar);
		while (pKLine != NULL)
		{
			if (GetSerialNo(pKLine->Bar, field.InsertTime) % nBarTimes == 0)
			{
				m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
				nLimit++;
				field.InsertTime.clear();
			}
			if (nLimit >= qryField.Limit.getValue())
				break;

			if (field.InsertTime.isNull())
			{
				CopyKLineBaseEntity(&field, pKLine);
			}
			else
			{
				if (pKLine->HighestPrice > field.HighestPrice)
					field.HighestPrice = pKLine->HighestPrice;
				if (pKLine->LowestPrice < field.LowestPrice)
					field.LowestPrice = pKLine->LowestPrice;
				field.Turnover += pKLine->Turnover;
				field.Volume += pKLine->Volume;
				field.OpenPrice = pKLine->OpenPrice;
				field.InsertTime = pKLine->InsertTime;
			}
			pKLine = m_pMDB->m_PublishKLineFactory->findNextByBar();
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendQryLine:
	{
		nRecvTNo = TID_RecvQryLine;
		CQryLineField qryField;
		CQryLineField::m_Describe.getSingleField(&qryField, pRspBuffer);
		if (qryField.Limit.isNull())
			qryField.Limit = 1000;

		CLastKLine* pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(qryField.ExchangeID, qryField.InstrumentID, qryField.Bar);
		if (pLastKLine == NULL)
		{
			m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, qryField.InstrumentID.getValue());
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &SessionBase, &qryField, &(qryField.m_Describe));
		}
		m_pServiceResponser->PrepareResponse(nRecvTNo, 0, "Success", &SessionBase);

		double baseSecondGap = 60 * 1000;
		switch (qryField.Bar[1])
		{
		case 'h':
			baseSecondGap = 60 * 60 * 1000;
			break;
		case 'd':
			baseSecondGap = 24 * 60 * 60 * 1000;
			break;
		case 'w':
			baseSecondGap = 7 * 24 * 60 * 60 * 1000;
			break;
		case 'o':
			baseSecondGap = 30 * 524 * 60 * 60 * 1000;
			break;
		default:
			m_pErrorEngine->reportError(ERROR_BAD_FIELD, "BarIsNot1hOr1d");
			return RspError(m_pErrorEngine, m_pServiceResponser, nRecvTNo, &SessionBase, &qryField, &(qryField.m_Describe));
		}
		double timeNow = time(NULL);
		CPublishKLine* pKLine = m_pMDB->m_PublishKLineFactory->findFirstByBar(qryField.ExchangeID, qryField.InstrumentID, qryField.Bar);
		CLineField field;
		int nLimit = 0;
		while (pKLine != NULL)
		{
			if (nLimit > qryField.Limit.getValue())
				break;
			CopyLineEntity(&field, pKLine);
			m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
			nLimit++;
			pKLine = m_pMDB->m_PublishKLineFactory->findNextByBar();
		}
		CopyLineEntity(&field, pLastKLine);
		m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_PushMarketTrade:
	{
		((TOutFogMetaRef*)pOutFogRspRef)->Reserve[0] = '4';
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

void CMyEngine::UpgradeTickPrice(const CMarketOrderField& field, const CMarketOrder* pOldMarketOrder)
{
	//char b[4096];
	//CWriteableMarketOrder a;
	//CopyMarketOrderEntity(&a, &field);
	//a.dumpString(b);
	//PRINT_TO_STD("field:%s", b);


	InstDescInfo& descInfo = m_pMarketDataService->m_InstDescMap[field.ExchangeID][field.InstrumentID];
	if (descInfo.desc == 0.0)
		return;

	CVolumeType DeltaVolume = 0.0;
	CNumberType DeltaOrders = 0;
	CVolumeType DeltaTrueVolume = 0.0;

	if (field.Orders > 0)
	{
		if (pOldMarketOrder == NULL)
		{
			DeltaVolume = field.Volume;
			DeltaOrders = field.Orders;
			DeltaTrueVolume = field.TrueVolume;
		}
		else
		{
			//pOldMarketOrder->dumpString(b);
			//PRINT_TO_STD("older:%s", b);
			DeltaVolume = field.Volume - pOldMarketOrder->Volume;
			DeltaOrders = field.Orders - pOldMarketOrder->Orders;
			DeltaTrueVolume = field.TrueVolume - pOldMarketOrder->TrueVolume;
		}
	}
	else
	{
		if (pOldMarketOrder != NULL)
		{
			//pOldMarketOrder->dumpString(b);
			//PRINT_TO_STD("older:%s", b);
			DeltaVolume = -pOldMarketOrder->Volume;
			DeltaOrders = -pOldMarketOrder->Orders;
			DeltaTrueVolume = -pOldMarketOrder->TrueVolume;
		}
	}

	for (int i = 0; i < descInfo.tick.size(); i++)
	{
		CPriceType Price = field.Price;

		if (field.Direction == D_Buy)
			Price.toLowerTick(descInfo.tick[i], descInfo.desc);
		else
			Price.toUpperTick(descInfo.tick[i], descInfo.desc);

		if (Price == 0.0)
			continue;

		CTickMarketOrder* pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findByPrice(field.ExchangeID, field.InstrumentID, descInfo.tick[i], Price);
		CWriteableTickMarketOrder theMarketOrder;
		if (pMarketOrder == NULL)
		{
			if (field.Orders > 0)
			{
				theMarketOrder.SubIndex = descInfo.index[i].c_str();
				theMarketOrder.ExchangeID = field.ExchangeID;
				theMarketOrder.InstrumentID = field.InstrumentID;
				theMarketOrder.Volume = field.Volume;
				theMarketOrder.Orders = field.Orders;
				theMarketOrder.TrueVolume = field.TrueVolume;
				theMarketOrder.Tick = descInfo.tick[i];
				theMarketOrder.Price = Price;
				theMarketOrder.BusinessNo = field.BusinessNo;
				theMarketOrder.Direction = field.Direction;
				//theMarketOrder.dumpString(b);
				//PRINT_TO_STD("tick1:%s", b);
				m_pMDB->m_TickMarketOrderFactory->add(&theMarketOrder);
			}
		}
		else
		{
			m_pMDB->m_TickMarketOrderFactory->retrieve(pMarketOrder, &theMarketOrder);
			if (pMarketOrder->Direction != field.Direction)
			{
				theMarketOrder.Volume = field.Volume;
				theMarketOrder.Orders = field.Orders;
				theMarketOrder.TrueVolume = field.TrueVolume;
				theMarketOrder.Direction = field.Direction;
				theMarketOrder.BusinessNo = field.BusinessNo;
				//这里由于索引的关系，必须要删除再新增
				m_pMDB->m_TickMarketOrderFactory->remove(pMarketOrder);
				m_pMDB->m_TickMarketOrderFactory->add(&theMarketOrder);
			}
			else
			{
				theMarketOrder.Volume += DeltaVolume;
				theMarketOrder.Orders += DeltaOrders;
				theMarketOrder.TrueVolume += DeltaTrueVolume;
				theMarketOrder.BusinessNo = field.BusinessNo;
			}

			//theMarketOrder.dumpString(b);
			//PRINT_TO_STD("ticker2:%s", b);

			if (theMarketOrder.Orders <= 0)
				m_pMDB->m_TickMarketOrderFactory->remove(pMarketOrder);
			else
				m_pMDB->m_TickMarketOrderFactory->update(pMarketOrder, &theMarketOrder);
		}
	}
}

int CMyEngine::TestTopicAction(CTopicActionField* pField, TSessionRef* pSession)
{
	int nTopicNo = atoi(pField->TopicID.getValue());
	if (nTopicNo == 0)
	{
		if (!strcmp(pField->TopicID.getValue(), "Topic_Broad"))
			nTopicNo = OF_Topic_Market_Broad;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_SnapShot"))
			nTopicNo = OF_Topic_Market_SnapShot;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_Depth_All"))
			nTopicNo = OF_Topic_Market_Depth_Delay;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_LastTrade"))
			nTopicNo = OF_Topic_Market_LastTrade;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_KLine"))
			nTopicNo = OF_Topic_Market_KLine;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_LastKLine"))
			nTopicNo = OF_Topic_Market_LastKLine;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_OverView"))
			nTopicNo = OF_Topic_Market_OverView;
		else if (!strcmp(pField->TopicID.getValue(), "Topic_Market_Depth_25"))
			nTopicNo = OF_Topic_Market_Depth_25;
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
		break;
	}
	case OF_Topic_Market_LastTrade:
	{
		break;
	}
	case OF_Topic_Private_Order:
	case OF_Topic_Private_Trade:
	case OF_Topic_Private_TriggerOrder:
	case OF_Topic_Private_SnapShot:
	case OF_Topic_Private_UserSession:
	{
		if (pField->Token == m_sGateWayKey.c_str())
			break;

		if ((pSession->pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && pSession->pToken[1] == '\0'))
		{
			if (pSession->pUserID[0] != '\0')
			{
				pField->Index = pSession->pUserID;
				pField->ResumeNo = -1;
				break;
			}
		}

		if (pField->Token.isNull())
			pField->Token = pSession->pToken;

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
	case OF_Topic_Market_OverView:
	case OF_Topic_Market_OverView_Delay:
	{
		switch (vFilters.size())
		{
		case 0:
			break;
		case 1:
			break;
		case 3://降频,3和2共用发送流程哦
			nReaderTimes = atoi(vFilters[2].c_str());
			pField->Index = vFilters[0].c_str();
			pField->Index += "_";
			pField->Index += vFilters[1].c_str();
		case 2://不降频
		{
			break;
		}
		default:
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
		}
		break;
	}
	case OF_Topic_Market_SnapShot:
	{
		if (pField->Index == m_sGateWayKey.c_str() || pField->Token == m_sGateWayKey.c_str())
		{
			pField->Index = "";
			pField->ResumeNo = -1;
			break;
		}

		if (vFilters.size() != 3 && vFilters.size() != 4)
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Filters.size!=3 && Filters.size!=4");

		nReaderTimes = atoi(vFilters[2].c_str());
		pField->Index = vFilters[0].c_str();
		pField->Index += "_";
		pField->Index += vFilters[1].c_str();
		if (vFilters.size() == 4)
		{
			map<CExchangeIDType, InstDescMap>::iterator iter = m_pMarketDataService->m_InstDescMap.find(vFilters[0].c_str());
			if (iter == m_pMarketDataService->m_InstDescMap.end())
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "NotFoundExchangeID");
			InstDescMap::iterator iter1 = iter->second.find(vFilters[1].c_str());
			if (iter1 == iter->second.end())
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "NotFoundInstrumentID");

			//InstDescInfo& descInfo = m_pMarketDataService->m_InstDescMap[vFilters[0].c_str()][vFilters[1].c_str()];
			//if (descInfo.desc && descInfo.desc != atof(vFilters[3].c_str()))
			CPriceType Tick = vFilters[3].c_str();
			if (Tick != iter1->second.desc)
			{
				pField->Index += "_";
				pField->Index += Tick.getString().c_str();
			}
		}
		pField->ResumeNo = -1;
		break;
	}
	case OF_Topic_Market_Depth_25:
	{
		nDepth = 25;
	}
	case OF_Topic_Market_Depth_50:
	{
		if (nDepth == 0)
			nDepth = 50;
		if (nTopicNo == OF_Topic_Market_Depth_50)
			pField->TopicID = OF_Topic_Market_Depth_Delay;
	}
	case OF_Topic_Market_Depth_100:
	{
		if (nDepth == 0)
			nDepth = 100;
		if (nTopicNo == OF_Topic_Market_Depth_100)
			pField->TopicID = OF_Topic_Market_Depth_Delay;
	}
	case OF_Topic_Market_Depth_All:
	{
		if (nDepth == 0)
			nDepth = -pField->ResumeNo;

		if (nTopicNo == OF_Topic_Market_Depth_All)
			pField->TopicID = OF_Topic_Market_Depth_Delay;
	}
	case OF_Topic_Market_Depth_Delay:
	{
		if (pField->Token != m_sGateWayKey.c_str())
		{
			if ((m_SubMap[pSession->sessionID]++) >= 10)
				return m_pErrorEngine->reportError(ERROR_NO_TRADING_RIGHT, "MoreThan3TimesPerSecond");
		}

		if (pField->Index.isNull())
		{
			if (pField->Token != m_sGateWayKey.c_str())
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Filters.size!=2 && Filters.size!=3");
			break;
		}

		if (vFilters.size() != 2 && vFilters.size() != 3)
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Filters.size!=2 && Filters.size!=3");

		map<CExchangeIDType, InstDescMap>::iterator iter = m_pMarketDataService->m_InstDescMap.find(vFilters[0].c_str());
		if (iter == m_pMarketDataService->m_InstDescMap.end())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "NotFoundExchangeID");
		InstDescMap::iterator iter1 = iter->second.find(vFilters[1].c_str());
		if (iter1 == iter->second.end())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "NotFoundInstrumentID");

		//InstDescInfo& descInfo = m_pMarketDataService->m_InstDescMap[vFilters[0].c_str()][vFilters[1].c_str()];
		//if (descInfo.desc && vFilters.size() == 3)
		if (vFilters.size() == 3)
		{
			CPriceType Tick = vFilters[2].c_str();
			if (Tick == iter1->second.desc)
				//if (iter1->second.desc == atof(vFilters[2].c_str()))
			{
				pField->Index = vFilters[0].c_str();
				pField->Index += "_";
				pField->Index += vFilters[1].c_str();
			}
			else
			{
				pField->Index = vFilters[0].c_str();
				pField->Index += "_";
				pField->Index += vFilters[1].c_str();
				pField->Index += "_";
				pField->Index += Tick.getString().c_str();
				pField->TopicID = OF_Topic_Market_Depth_25;
			}
		}
		pField->ResumeNo = -1;
		break;
	}
	case OF_Topic_Market_KLine:
	{
		if (pField->Token != m_sGateWayKey.c_str())
		{
			if ((m_SubMap[pSession->sessionID]++) >= 10)
				return m_pErrorEngine->reportError(ERROR_NO_TRADING_RIGHT, "MoreThan3TimesPerSecond");
		}

		if (vFilters.size() == 0)	//全量订阅，对Index没有要求，全部按照没有填来
		{
			if (pField->ResumeNo == 0)
			{
				pField->ResumeNo = -1;
			}
		}
		break;
	}
	case OF_Topic_Market_LastKLine:
	{
		if (pField->Token != m_sGateWayKey.c_str())
		{
			if ((m_SubMap[pSession->sessionID]++) >= 10)
				return m_pErrorEngine->reportError(ERROR_NO_TRADING_RIGHT, "MoreThan3TimesPerSecond");
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

int CMyEngine::RspTopicAction(CTopicActionField* pField, TSessionRef* pSession)
{
	int nTopicNo = atoi(pField->TopicID.getValue());
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
	case OF_Topic_Market_OverView:
	case OF_Topic_Market_OverView_Delay:
	{
		switch (vFilters.size())
		{
		case 0:
		{
			if (pField->ResumeNo == 0)
				RspMarketData(m_pMDB, m_pServiceResponser, pSession, nTopicNo);
			break;
		}
		case 1://Spot这种，不能降频
			RspMarketData(m_pMDB, m_pServiceResponser, pField->Index, pSession, nTopicNo);
			break;
		case 3://降频,3和2共用发送流程哦
			nReaderTimes = atoi(vFilters[2].c_str());
		case 2://不降频
		{
			break;
		}
		}
		break;
	}
	case OF_Topic_Market_SnapShot:
	{
		if (pField->Index == m_sGateWayKey.c_str() || pField->Token == m_sGateWayKey.c_str())
		{
			pField->Index = "";
			pField->ResumeNo = -1;
			break;
		}

		nReaderTimes = atoi(vFilters[2].c_str());
		pField->Index = vFilters[0].c_str();
		pField->Index += "_";
		pField->Index += vFilters[1].c_str();
		if (vFilters.size() == 4)
		{
			map<CExchangeIDType, InstDescMap>::iterator iter = m_pMarketDataService->m_InstDescMap.find(vFilters[0].c_str());
			if (iter == m_pMarketDataService->m_InstDescMap.end())
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "NotFoundExchangeID");
			InstDescMap::iterator iter1 = iter->second.find(vFilters[1].c_str());
			if (iter1 == iter->second.end())
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "NotFoundInstrumentID");

			//InstDescInfo& descInfo = m_pMarketDataService->m_InstDescMap[vFilters[0].c_str()][vFilters[1].c_str()];
			//if (descInfo.desc && descInfo.desc != atof(vFilters[3].c_str()))
			CPriceType Tick = vFilters[3].c_str();
			if (Tick != iter1->second.desc)
			{
				pField->Index += "_";
				pField->Index += Tick.getString().c_str();
			}
		}
		pField->ResumeNo = -1;
		break;
	}
	case OF_Topic_Market_Depth_25:
	{
		nDepth = 25;
	}
	case OF_Topic_Market_Depth_50:
	{
		if (nDepth == 0)
			nDepth = 50;
		if (nTopicNo == OF_Topic_Market_Depth_50)
			pField->TopicID = OF_Topic_Market_Depth_Delay;
	}
	case OF_Topic_Market_Depth_100:
	{
		if (nDepth == 0)
			nDepth = 100;
		if (nTopicNo == OF_Topic_Market_Depth_100)
			pField->TopicID = OF_Topic_Market_Depth_Delay;
	}
	case OF_Topic_Market_Depth_All:
	{
		if (nDepth == 0)
			nDepth = -pField->ResumeNo;

		if (nTopicNo == OF_Topic_Market_Depth_All)
			pField->TopicID = OF_Topic_Market_Depth_Delay;
	}
	case OF_Topic_Market_Depth_Delay:
	{
		if (pField->Token != m_sGateWayKey.c_str())
		{
			if ((m_SubMap[pSession->sessionID]++) >= 10)
				return m_pErrorEngine->reportError(ERROR_NO_TRADING_RIGHT, "MoreThan3TimesPerSecond");
		}

		if (pField->Index.isNull())
		{
			if (pField->Token != m_sGateWayKey.c_str())
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Filters.size!=2 && Filters.size!=3");
			if (pField->ResumeNo == 0)
				RspMarketOrder(m_pMDB, m_pServiceResponser, pSession);
			break;
		}

		if (vFilters.size() != 2 && vFilters.size() != 3)
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Filters.size!=2 && Filters.size!=3");

		map<CExchangeIDType, InstDescMap>::iterator iter = m_pMarketDataService->m_InstDescMap.find(vFilters[0].c_str());
		if (iter == m_pMarketDataService->m_InstDescMap.end())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "NotFoundExchangeID");
		InstDescMap::iterator iter1 = iter->second.find(vFilters[1].c_str());
		if (iter1 == iter->second.end())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "NotFoundInstrumentID");

		//InstDescInfo& descInfo = m_pMarketDataService->m_InstDescMap[vFilters[0].c_str()][vFilters[1].c_str()];
		//if (descInfo.desc && vFilters.size() == 3)
		if (vFilters.size() == 3)
		{
			CPriceType Tick = vFilters[2].c_str();
			if (Tick == iter1->second.desc)
				//if (iter1->second.desc == atof(vFilters[2].c_str()))
			{
				pField->Index = vFilters[0].c_str();
				pField->Index += "_";
				pField->Index += vFilters[1].c_str();
				RspMarketOrder(m_pMDB, m_pServiceResponser, vFilters[0].c_str(), vFilters[1].c_str(), nDepth, pSession);
			}
			else
			{
				pField->Index = vFilters[0].c_str();
				pField->Index += "_";
				pField->Index += vFilters[1].c_str();
				pField->Index += "_";
				pField->Index += Tick.getString().c_str();
				pField->TopicID = OF_Topic_Market_Depth_25;
				RspMarketOrder(m_pMDB, m_pServiceResponser, vFilters[0].c_str(), vFilters[1].c_str(), vFilters[2].c_str(), nDepth, pSession);
			}
		}
		else
		{
			RspMarketOrder(m_pMDB, m_pServiceResponser, vFilters[0].c_str(), vFilters[1].c_str(), nDepth, pSession);
		}
		pField->ResumeNo = -1;
		break;
	}
	case OF_Topic_Market_KLine:
	{
		if (pField->Token != m_sGateWayKey.c_str())
		{
			if ((m_SubMap[pSession->sessionID]++) >= 10)
				return m_pErrorEngine->reportError(ERROR_NO_TRADING_RIGHT, "MoreThan3TimesPerSecond");
		}

		if (vFilters.size() == 0)	//全量订阅，对Index没有要求，全部按照没有填来
		{
			if (pField->ResumeNo == 0)
			{
				RspKLine(m_pMDB, m_pServiceResponser, pSession);
				pField->ResumeNo = -1;
			}
		}
		break;
	}
	case OF_Topic_Market_LastKLine:
	{
		if (pField->Token != m_sGateWayKey.c_str())
		{
			if ((m_SubMap[pSession->sessionID]++) >= 10)
				return m_pErrorEngine->reportError(ERROR_NO_TRADING_RIGHT, "MoreThan3TimesPerSecond");
		}

		if (vFilters.size() >= 3)
		{
			CExchangeIDType ExchangeID = vFilters[0].c_str();
			CInstrumentIDType InstrumentID = vFilters[1].c_str();
			CBarType Bar = vFilters[2].c_str();

			char baseBar[3];
			baseBar[2] = '\0';
			baseBar[1] = Bar.getBuffer()[Bar.getSize() - 1];
			Bar.getBuffer()[Bar.getSize() - 1] = '\0';
			int nBarTimes = atoi(Bar);
			baseBar[0] = '1';

			if (baseBar[1] == 'm' && nBarTimes % 5 == 0)
			{
				nBarTimes = nBarTimes / 5;
				baseBar[0] = '5';
			}

			CLastKLine* pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(ExchangeID, InstrumentID, baseBar);
			if (pLastKLine == NULL)
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, pField->Index.getValue());

			//先取到历史的K线，内存中暂时存放的
			m_pServiceResponser->PrepareResponse(TID_PushKLine, 0, "Success", pSession);

			CKLineBaseField field;
			CopyKLineBaseEntity(&field, pLastKLine);
			int nLimit = 0;
			CPublishKLine* pKLine = m_pMDB->m_PublishKLineFactory->findFirstByBar(ExchangeID, InstrumentID, baseBar);
			while (pKLine != NULL)
			{
				if (nLimit >= pField->ResumeNo.getValue())
					break;
				if (field.InsertTime.isNull())
				{
					CopyKLineBaseEntity(&field, pKLine);
				}
				else
				{
					if (pKLine->HighestPrice > field.HighestPrice)
						field.HighestPrice = pKLine->HighestPrice;
					if (pKLine->LowestPrice < field.LowestPrice)
						field.LowestPrice = pKLine->LowestPrice;
					field.Turnover += pKLine->Turnover;
					field.Volume += pKLine->Volume;
				}

				if (GetSerialNo(pKLine->Bar, field.InsertTime.getValue()) % nBarTimes == 0)
				{
					if (pKLine->HighestPrice > field.HighestPrice)
						field.HighestPrice = pKLine->HighestPrice;
					if (pKLine->LowestPrice < field.LowestPrice)
						field.LowestPrice = pKLine->LowestPrice;
					field.Turnover += pKLine->Turnover;
					field.Volume += pKLine->Volume;
					field.OpenPrice = pKLine->OpenPrice;
					m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
					nLimit++;
					field.InsertTime.clear();
				}
				else
				{
					if (field.InsertTime.isNull())
					{
						CopyKLineBaseEntity(&field, pKLine);
					}
					else
					{
						if (pKLine->HighestPrice > field.HighestPrice)
							field.HighestPrice = pKLine->HighestPrice;
						if (pKLine->LowestPrice < field.LowestPrice)
							field.LowestPrice = pKLine->LowestPrice;
						field.Turnover += pKLine->Turnover;
						field.Volume += pKLine->Volume;
						field.ClosePrice = pKLine->ClosePrice;
					}
				}
				pKLine = m_pMDB->m_PublishKLineFactory->findNextByBar();
			}
			m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_LAST);


			pField->Index = vFilters[0].c_str();
			pField->Index += "_";
			pField->Index += vFilters[1].c_str();
			pField->Index += "_1m";
			pField->ResumeNo = -1;
		}
		else
		{
			m_pServiceResponser->PrepareResponse(TID_PushKLine, 0, "Success", pSession);
			CLastKLine* pLastKLine = m_pMDB->m_LastKLineFactory->getFirst();
			while (pLastKLine != NULL)
			{
				CLastKLineField field;
				CopyLastKLineEntity(&field, pLastKLine);
				m_pServiceResponser->SendNext(&field, &field.m_Describe, OUTFOG_CHAIN_CONTINUE);
				pLastKLine = m_pMDB->m_LastKLineFactory->getNext();
			}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
			pField->ResumeNo = -1;
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


void CMyEngine::Init24(CPublishKLine* pKLine, double now)
{
	if (pKLine->Bar == "1m" && pKLine->InsertTime >= now - 24 * 60 * 60 * 1000)
	{
		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pKLine->ExchangeID, pKLine->InstrumentID);
		CWriteableMarketData theMarketData;
		if (pMarketData == NULL)
		{
			theMarketData.init();
			theMarketData.ExchangeID = pKLine->ExchangeID;
			theMarketData.InstrumentID = pKLine->InstrumentID;
			theMarketData.LowestPrice24 = pKLine->LowestPrice;
			theMarketData.HighestPrice24 = pKLine->HighestPrice;
			theMarketData.Volume24 = pKLine->Volume;
			theMarketData.Turnover24 = pKLine->Turnover;
			if (pKLine->InsertTime < now - 24 * 60 * 60 * 1000 - 60 * 1000)
				theMarketData.OpenPrice24 = pKLine->OpenPrice;
			m_pMDB->m_MarketDataFactory->add(&theMarketData);
		}
		else
		{
			m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
			if (theMarketData.LowestPrice24 > pKLine->LowestPrice)
				theMarketData.LowestPrice24 = pKLine->LowestPrice;
			if (theMarketData.HighestPrice24 < pKLine->HighestPrice)
				theMarketData.HighestPrice24 = pKLine->HighestPrice;
			theMarketData.Volume24 += pKLine->Volume;
			theMarketData.Turnover24 += pKLine->Turnover;
			if (pKLine->InsertTime < now - 24 * 60 * 60 * 1000 - 60 * 1000)
				theMarketData.OpenPrice24 = pKLine->OpenPrice;
			m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
		}
	}
}

void CMyEngine::SubTopics(CLinkField linkField)
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

	if (g_nMDSnapDepth >= 0)
	{
		if (g_nMDSnapDepth == 0)
		{
			field.TopicID = OF_Topic_Market_Depth_25;
			field.ResumeNo = -1000;
			CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
			m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

			field.ResumeNo = 0;
			field.TopicID = OF_Topic_Market_Depth_Delay;
			CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
			m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
			field.ResumeNo = -1;

			field.TopicID = OF_Topic_Market_SnapShot;
			CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
			m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
		}
		else
		{
			field.ResumeNo = 0;
			field.TopicID = OF_Topic_Market_Depth_All;
			CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
			m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
			field.ResumeNo = -1;
		}

		field.TopicID = OF_Topic_Market_LastTrade;
		CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
		m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

		field.TopicID = OF_Topic_Market_OverView;
		field.ResumeNo = 0;
		CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
		m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
		field.ResumeNo = -1;

		field.TopicID = OF_Topic_Market_LastKLine;
		CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
		m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

		field.TopicID = OF_Topic_Market_KLine;
		//为了防止在加载数据库K线的时候，交易系统产生的新K线无法获取到，这里提前
		field.ResumeNo = 0;
		CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
		m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
		field.ResumeNo = -1;
	}

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
	sprintf(sLink, "Public_%s_Links", sKernelid.c_str());
	NetWorkIni.ReadString(sLink, "Links", "", slinks, sizeof(slinks) - 1);
	if (slinks[0] == '\0')
	{
		sKernelid = "*";
		sprintf(sLink, "Public_%s_Links", sKernelid.c_str());
		NetWorkIni.ReadString(sLink, "Links", "", slinks, sizeof(slinks) - 1);
	}

	g_nMDSnapDepth = NetWorkIni.ReadInt(sLink, "MDSnapDepth", 0);
	PRINT_TO_STD("%s:MDSnapDepth:%d", sLink, g_nMDSnapDepth);

	CMyEngine* pApp = new CMyEngine();
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
		sprintf(sLink, "Public_%s_Link%d", sKernelid.c_str(), link.LinkNo.getValue());
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

