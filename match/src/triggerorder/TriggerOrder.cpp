#include "TriggerOrder.h"
#include "public.h"
#include "CopyField.h"
#include "monitorIndex.h"
#include "CFilelogService.h"

void CmdLineArgError()
{
	printf("Usage: triggerorder \n");
	printf("       triggerorder -v      :	Show Version\n");
	exit(0);
}

CTriggerOrders::CTriggerOrders()
{
	//使用更多的索引,在mdbSearch.cpp中添加Level
	g_cMDBRunLevel = 't';

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

	m_WorkerMap.insert(CWorkerMap::value_type("StopOrder", new CStopOrderWorker(m_pMDB, this)));
	//m_WorkerMap.insert(CWorkerMap::value_type("Callback", new CCallbackWorker(m_pMDB, this)));

	new CIntPtrMonitorIndex("TriggerOrderSize", m_pMDB->m_TriggerOrderFactory->getCountPtr(), 10);
}

CTriggerOrders::~CTriggerOrders()
{
}

bool CTriggerOrders::RspField(const TOutFogMetaRef* pOutFogRspRef, const void* pRspBuffer)
{
	if (pRspBuffer == NULL)
	{
		//REPORT_EVENT(LOG_CRITICAL, "RspField", "Error:pRspBufferIsNull");
		return false;
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

		//char key[64] = { 0 };
		//sprintf(key, "%s:%s", field.ExchangeID.getValue(), field.InstrumentID.getValue());

		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		CWriteableMarketData theMarketData;
		if (pMarketData != NULL)
		{
			if (field.LastPrice != pMarketData->LastPrice)
			{
				REPORT_EVENT(LOG_INFO, "PushMarketData", "[%s][Last:%s][Time:%0.f]", field.InstrumentID.getValue(),
					field.LastPrice.getString().c_str(),  field.UpdateTime.getValue());

				CWorkerMap::iterator iter = m_WorkerMap.begin();
				while (iter != m_WorkerMap.end())
				{
					iter->second->TriggerByPrice(field.ExchangeID, field.InstrumentID, field.LastPrice, TPT_LastPrice);
					iter++;
				}
			}

			if (field.MarkedPrice != pMarketData->MarkedPrice)
			{
				REPORT_EVENT(LOG_INFO, "PushMarketData", "[%s][Marked:%s][Time:%0.f]", field.InstrumentID.getValue(),
					 field.MarkedPrice.getString().c_str(), field.UpdateTime.getValue());

				CWorkerMap::iterator iter = m_WorkerMap.begin();
				while (iter != m_WorkerMap.end())
				{
					iter->second->TriggerByPrice(field.ExchangeID, field.InstrumentID, field.MarkedPrice, TPT_MarkedPrice);
					iter++;
				}
			}

			m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
			theMarketData.LastPrice = field.LastPrice;
			theMarketData.UpdateTime = field.UpdateTime;
			theMarketData.MarkedPrice = field.MarkedPrice;
			m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
		}
		else
		{
			theMarketData.init();
			theMarketData.ExchangeID = field.ExchangeID;
			theMarketData.InstrumentID = field.InstrumentID;
			theMarketData.LastPrice = field.LastPrice;
			theMarketData.UpdateTime = field.UpdateTime;
			theMarketData.MarkedPrice = field.MarkedPrice;
			m_pMDB->m_MarketDataFactory->add(&theMarketData);
		}

		//map<string, CMilliSecondsType>::iterator timeIt = m_InstrumentTradeTimeMap.find(key);
		//if (timeIt == m_InstrumentTradeTimeMap.end())
		//{
		//	m_InstrumentTradeTimeMap.insert(pair<string, CMilliSecondsType>(key, field.UpdateTime));
		//}
		//else
		//{
		//	// 时间间隔 <= 1s, 不做处理
		//	if (field.UpdateTime <= timeIt->second)
		//		break;
		//	else
		//		m_InstrumentTradeTimeMap[key] = field.UpdateTime;
		//}

		//map<string, CPriceType>::iterator it = m_InstrumentPriceMap.find(key);
		//if (it == m_InstrumentPriceMap.end())
		//{
		//	m_InstrumentPriceMap.insert(pair<string, CPriceType>(key, field.LastPrice));
		//}
		//else
		//{
		//	// 价格没发生变化，不作处理
		//	if (field.LastPrice == it->second)
		//		break;
		//	else
		//		m_InstrumentPriceMap[key] = field.LastPrice;
		//}

		//REPORT_EVENT(LOG_INFO, "PushMarketData", "[%s_%s][Last:%s][Marked:%s][Ask1:%s][Bid1:%s][Time:%d]",
		//	field.ExchangeID.getValue(), field.InstrumentID.getValue(),
		//	field.LastPrice.getString().c_str(), field.MarkedPrice.getString().c_str(), field.AskPrice1.getString().c_str(),
		//	field.BidPrice1.getString().c_str(), field.UpdateTime.getValue());

		break;
	}
	case TID_PushMarketTrade:
	{
		CMarketTradeField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		//char key[64] = { 0 };
		//sprintf(key, "%s:%s", field.ExchangeID.getValue(), field.InstrumentID.getValue());
		//map<string, CPriceType>::iterator it = m_InstrumentPriceMap.find(key);
		//if (it == m_InstrumentPriceMap.end())
		//{
			//m_InstrumentPriceMap.insert(pair<string, CPriceType>(key, field.Price));
			//m_InstrumentTradeTimeMap.insert(pair<string, CMilliSecondsType>(key, field.TradeTime));
	//	}
		//else
		//{
		//	// 价格没发生变化，不作处理
		//	if (field.Price == it->second)
		//	{
		//		m_InstrumentTradeTimeMap[key] = field.TradeTime;
		//		break;
		//	}
		//	else
		//	{
		//		m_InstrumentPriceMap[key] = field.Price;
		//		m_InstrumentTradeTimeMap[key] = field.TradeTime;
		//	}
		//}

		CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		CWriteableMarketData theMarketData;
		if (pMarketData != NULL)
		{
			if (field.Price != pMarketData->LastPrice)
			{
				REPORT_EVENT(LOG_INFO, "PushMarketTrade", "[%s][Price:%s][Time:%0.f]",
					field.InstrumentID.getValue(), field.Price.getString().c_str(), field.InsertTime.getValue());
				CWorkerMap::iterator iter = m_WorkerMap.begin();
				while (iter != m_WorkerMap.end())
				{
					iter->second->TriggerByPrice(field.ExchangeID, field.InstrumentID, field.Price, TPT_LastPrice);
					iter++;
				}
			}
			m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
			theMarketData.LastPrice = field.Price;
			theMarketData.UpdateTime = field.InsertTime;
			m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
		}
		break;
	}
	case TID_PushTriggerOrder:
	{
		CTriggerOrderField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		// 过滤指标类型条件单，由其他模块进行触发
		if (field.TriggerOrderType >= TOT_Indicator)
			break;

		REPORT_EVENT(LOG_INFO, "PushTriggerOrder", "[OrderID:%s][InstrumentID:%s][TriggerStatus:%c]",
			field.OrderID.getValue(), field.InstrumentID.getValue(), field.TriggerStatus.getValue());

		if (field.TriggerStatus == TS_Active)
		{
			CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
			if (pMarketData != NULL)
			{
				CPriceType TriggerPrice = pMarketData->LastPrice;
				bool needDelete = false;
				int errNo = 0; //0表示ok
				char errMsg[128] = { 0 }; //128是ErrorMsg的最大长度
				if (field.TriggerPriceType == TPT_MarkedPrice)
					TriggerPrice = pMarketData->MarkedPrice;

				//卖出开空，止盈价应该小于市价/标记价，止损价应该大于市价/标记价，反了必须直接返回错误
				if (field.Direction == D_Buy)
				{
					if (!field.SLTriggerPrice.isNull() && field.SLTriggerPrice <= TriggerPrice) {
						needDelete = true;
						errNo = -1;
						sprintf(errMsg, "Buy_SLTriggerPrice:%s<=TriggerPrice:%s", field.SLTriggerPrice.getString().c_str(), TriggerPrice.getString().c_str());
					}
					if (!field.TPTriggerPrice.isNull() && field.TPTriggerPrice >= TriggerPrice) {
						needDelete = true;
						errNo = -2;
						sprintf(errMsg, "Buy_TPTriggerPrice:%s>=TriggerPrice:%s", field.TPTriggerPrice.getString().c_str(), TriggerPrice.getString().c_str());
					}
				}
				//买入开多，止损价应该小于市价/标记价，止盈价应该大于市价/标记价，反了必须直接返回错误
				else
				{
					if (!field.SLTriggerPrice.isNull() && field.SLTriggerPrice >= TriggerPrice)
					{
						needDelete = true;
						errNo = -3;
						sprintf(errMsg, "Sell_SLTriggerPrice:%s>=TriggerPrice:%s", field.SLTriggerPrice.getString().c_str(), TriggerPrice.getString().c_str());
					}
					if (!field.TPTriggerPrice.isNull() && field.TPTriggerPrice <= TriggerPrice)
					{
						needDelete = true;
						errNo = -4;
						sprintf(errMsg, "Sell_TPTriggerPrice:%s<=TriggerPrice:%s", field.TPTriggerPrice.getString().c_str(), TriggerPrice.getString().c_str());
					}
				}
				if (needDelete)
				{
					REPORT_EVENT(LOG_INFO, "TID_PushTriggerOrder", "%s", errMsg);
					CWriteableTriggerOrder theTriggerOrder;
					theTriggerOrder.init();
					CopyTriggerOrderEntity(&theTriggerOrder, &field);
					theTriggerOrder.ErrorNo = errNo;
					theTriggerOrder.ErrorMsg = errMsg;
					SendTriggerOrder(&theTriggerOrder, TriggerPrice, TriggerPrice, AF_Delete);
				}
			}
		}

		if (field.TriggerStatus >= TS_TriggerSuccess)
		{
			CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findByPK(field.OrderID);
			if (pTriggerOrder != NULL)
			{
				//删除本地triggerorder
				REPORT_EVENT(LOG_INFO, "TriggerOrderSync", "remove local TriggerOrder:[OrderID:%s][InstrumentID:%s]",
					pTriggerOrder->OrderID.getValue(), pTriggerOrder->InstrumentID.getValue());
				m_pMDB->m_TriggerOrderFactory->remove(pTriggerOrder, NULL);
			}
			else
			{
				REPORT_EVENT(LOG_ERROR, "TriggerOrder", "can't find OrderID, please check runLevel");
			}
		}
		else
		{
			CWriteableTriggerOrder theTriggerOrder;
			theTriggerOrder.init();
			CopyTriggerOrderEntity(&theTriggerOrder, &field);
			m_pMDB->m_TriggerOrderFactory->addOrUpdateByPK(&theTriggerOrder, NULL);
		}

		break;
	}
	case TID_PushTimeSync:
	{
		CCurrentTimeField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		REPORT_EVENT(LOG_INFO, "PushTimeSync", "[CurrTime:%d][ZoneDate:%s][ZoneTime:%s]",
			field.CurrTime.getValue(), field.ZoneDate.getValue(), field.ZoneTime.getValue());

		//CWorkerMap::iterator iter = m_WorkerMap.begin();
		//while (iter != m_WorkerMap.end())
		//{
		//	iter->second->TimeSync(&field);
		//	iter++;
		//}
		break;
	}
	case TID_RecvTriggerOrderAction:
	{
		if (pOutFogRspRef->SubjectNo != 0)
		{
			CTriggerOrderActionField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);
			REPORT_EVENT(LOG_INFO, "RecvTriggerOrderAction Error", "[ErrorID:%d][ErrorMsg:%s][OrderID:%s][InstrumentID:%s]",
				pOutFogRspRef->SubjectNo, pOutFogRspRef->Remark, field.OrderID.getValue(), field.InstrumentID.getValue());
		}
		else
		{
			CTriggerOrderField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);
			REPORT_EVENT(LOG_INFO, "RecvTriggerOrderAction Success", "[ErrorID:%d][ErrorMsg:%s][OrderID:%s][InstrumentID:%s][TriggerStatus:%c]",
				pOutFogRspRef->SubjectNo, pOutFogRspRef->Remark, field.OrderID.getValue(), field.InstrumentID.getValue(), field.TriggerStatus.getValue());
		}
		break;
	}
	case TID_PushKLine:
	{
		/*CLastKLineField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		REPORT_EVENT(LOG_INFO, "RspField", "TID_PushKLine:[InstrumentID:%d][Bar:%s][BeginTime:%d][OpenPrice:%0.4f][ClosePrice:%0.4f][HighestPrice:%0.4f][LowestPrice:%0.4f][Volume:%0.2f]",
			field.InstrumentID.getValue(), field.Bar.getValue(), field.BeginTime.getValue(),
			field.OpenPrice.getValue(), field.ClosePrice.getValue(), field.HighestPrice.getValue(), field.LowestPrice.getValue(), field.Volume.getValue());
		*/
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

bool CTriggerOrders::SendTriggerOrder(CTriggerOrder* pTriggerOrder, CPriceType Price, CPriceType LastPrice, CActionFlagType ActionFlag)
{
	REPORT_EVENT(LOG_INFO, "SendTriggerOrderAction", "activate TriggerOrder[%c]:[OrderID:%s][InstrumentID:%s][TriggerPriceType:%c][TriggerPrice:%s][BasePrice:%s]",
		ActionFlag.getValue(), pTriggerOrder->OrderID.getValue(), pTriggerOrder->InstrumentID.getValue(), pTriggerOrder->TriggerPriceType.getValue(),
		Price.getString().c_str(), LastPrice.getString().c_str());

	CTriggerOrderActionField theStruct;
	theStruct.ActionFlag = ActionFlag;
	theStruct.MemberID = pTriggerOrder->MemberID;
	theStruct.OrderID = pTriggerOrder->OrderID;
	theStruct.TriggerPrice = Price;
	theStruct.TriggerPriceType = pTriggerOrder->TriggerPriceType;
	//TODO：止盈止损赋值，虽然后面的使用是通过retrieve获取到pTriggerOrder，再得到这两个值，但是如果streamBuf中包含这两个值，会不会判断更容易？
	//theStruct.TPTriggerPrice = pTriggerOrder->TPTriggerPrice;
	//theStruct.SLTriggerPrice = pTriggerOrder->SLTriggerPrice;
	theStruct.ExchangeID = pTriggerOrder->ExchangeID;
	theStruct.InstrumentID = pTriggerOrder->InstrumentID;
	//theStruct.ProductGroup = pTriggerOrder->ProductGroup;
	if (ActionFlag == AF_Delete)
	{
		//把Delete的原因返回回去
		theStruct.ErrorNo = pTriggerOrder->ErrorNo;
		theStruct.ErrorMsg = pTriggerOrder->ErrorMsg;
	}
	// 发送TriggerOrderAction
	CTriggerOrderActionField::m_Describe.setSingleField(m_streamBuf, &theStruct);
	//m_OutFogMetaRef.ServiceNo = m_LinkTo.LinkNo;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	m_OutFogMetaRef.MessageNo = TID_SendTriggerOrderAction;
	m_OutFogMetaRef.FieldNo = CTriggerOrderActionField::m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = CTriggerOrderActionField::m_Describe.m_nStreamSize;
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
	return true;
}

void CTriggerOrders::SubTopics(CLinkField linkField)
{
	CTopicActionField field;
	m_OutFogMetaRef.FieldNo = field.m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = field.m_Describe.m_nStreamSize;
	m_OutFogMetaRef.GlobalNo = linkField.LinkNo;

	field.Action = SA_Sub;
	field.ResumeNo = 0;
	int nLocalNo = 2;

	// 最新成交
	field.TopicID = "2";
	field.Index = "";
	m_OutFogMetaRef.MessageNo = TID_SendTopicAction;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

	// 最新K线
	/*field.TopicID = "4";
	field.LocalNo = nLocalNo++;
	field.FilterValue = "Test_BTCUSD_1m";
	m_OutFogMetaRef.TNo = TID_SendTopicAction;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);*/

	// 时钟心跳
	/*field.TopicID = "6";
	field.LocalNo = nLocalNo++;
	field.FilterValue = "";
	m_OutFogMetaRef.TNo = TID_SendTopicAction;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);*/

	// 实时行情
	field.TopicID = "7";
	field.Index = "";
	m_OutFogMetaRef.MessageNo = TID_SendTopicAction;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

	// 策略单
	field.TopicID = "14";
	field.Index = "";
	m_OutFogMetaRef.MessageNo = TID_SendTopicAction;
	CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);

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

	CTriggerOrders* pApp = new CTriggerOrders();
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
	sprintf(sLink, "TriggerOrder_%d_Links", nKernelid);
	NetWorkIni.ReadString(sLink, "Links", "", slinks, sizeof(slinks) - 1);
	vector<string> SLinks = Txtsplit(slinks, ",");
	for (int i = 0; i < SLinks.size(); i++)
	{
		link.LinkNo = atoi(SLinks[i].c_str());
		sprintf(sLink, "TriggerOrder_%d_Link%d", nKernelid, link.LinkNo.getValue());
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

	while (true)
	{
		SLEEP_SECONDS(10);
	}

	return 0;
}

