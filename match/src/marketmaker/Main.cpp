#include "Main.h"
#include "MarketWorker.h"
#include "public.h"
#include "CopyField.h"
#include "monitorIndex.h"
#include "CFilelogService.h"
#include "Config.h"
#include "errorDefine.h"

int g_nKernelID = 0;

void CmdLineArgError()
{
	printf("Usage: marketmaker \n");
	printf("       marketmaker -v      :	Show Version\n");
	exit(0);
}

CMarketMaker::CMarketMaker()
{
	//m_nCount = 0;
	//m_nSkipCount = 0;
	//m_nTime = 0;
	//m_LastPrice.clear();
	CMonitorIndex::init(CFilelogService::CreateInstance(GetConfigString(INI_FILE_NAME, "SyslogPath").c_str()), GetConfigString(INI_FILE_NAME, "LogLevel").c_str());
	m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	m_OutFogMetaRef.RequestNo = 0;
	m_bReady = false;
	m_nLocalID = 0;
	m_Ini.Open(NETWORK_FILE_NAME);
	m_nInitOrders = 0;
}

CMarketMaker::~CMarketMaker()
{
}

bool CMarketMaker::RspField(const TOutFogMetaRef* pOutFogRspRef, const void* pRspBuffer)
{
	switch (pOutFogRspRef->MessageNo)
	{
	case TID_RecvUserLogin:
	{
		PRINT_TO_STD("LinkID[%0.f],Login[%s]", pOutFogRspRef->GlobalNo, pOutFogRspRef->Remark);
		//如果是错误的登录，需要提醒注意
		if (pOutFogRspRef->SubjectNo != 0)
			break;

		if (pOutFogRspRef->GlobalNo == m_LinkTo.LinkNo)
		{
			m_LinkTo.LinkStatus = LS_Login;
			CWorkerMap::iterator iter = m_WorkerMap.begin();
			while (iter != m_WorkerMap.end())
			{
				iter->second->Init();
				iter++;
			}

			//查询合约
			CQryInstrumentField field0;
			memset(&field0, 0, sizeof(CQryInstrumentField));
			m_OutFogMetaRef.GlobalNo = m_LinkTo.LinkNo;
			m_OutFogMetaRef.MessageNo = TID_SendQryInstrument;
			m_OutFogMetaRef.FieldNo = field0.m_Describe.m_FieldID;
			m_OutFogMetaRef.Length = field0.m_Describe.m_nStreamSize;
			m_OutFogMetaRef.RequestNo = 0;
			CQryInstrumentField::m_Describe.setSingleField(m_streamBuf, &field0);
			m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
			////查询资金
			//CQryAccountField field3;
			//memset(&field3, 0, sizeof(CQryAccountField));
			//field3.MemberID = m_MemberID;
			//m_OutFogMetaRef.GlobalNo = m_LinkTo.LinkNo;
			//m_OutFogMetaRef.MessageNo = TID_SendQryAccount;
			//m_OutFogMetaRef.FieldNo = field3.m_Describe.m_FieldID;
			//m_OutFogMetaRef.Length = field3.m_Describe.m_nStreamSize;
			//m_OutFogMetaRef.RequestNo = 0;
			//CQryAccountField::m_Describe.setSingleField(m_streamBuf, &field3);
			//m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
			//查询订单
			CQryOrderField field1;
			memset(&field1, 0, sizeof(CQryOrderField));
			field1.MemberID = m_MemberID;
			m_OutFogMetaRef.GlobalNo = m_LinkTo.LinkNo;
			m_OutFogMetaRef.MessageNo = TID_SendQryOrder;
			m_OutFogMetaRef.FieldNo = field1.m_Describe.m_FieldID;
			m_OutFogMetaRef.Length = field1.m_Describe.m_nStreamSize;
			m_OutFogMetaRef.RequestNo = 0;
			CQryOrderField::m_Describe.setSingleField(m_streamBuf, &field1);
			m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
			//清空服务器端的MaxLocalID
			CResetLocalIDField field2;
			memset(&field2, 0, sizeof(CResetLocalIDField));
			field2.UserID = m_LinkTo.LinkUserID;
			field2.MemberID = m_MemberID;
			m_OutFogMetaRef.GlobalNo = m_LinkTo.LinkNo;
			m_OutFogMetaRef.MessageNo = TID_SendResetLocalID;
			m_OutFogMetaRef.FieldNo = field2.m_Describe.m_FieldID;
			m_OutFogMetaRef.Length = field2.m_Describe.m_nStreamSize;
			CResetLocalIDField::m_Describe.setSingleField(m_streamBuf, &field2);
			m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
			m_nLocalID = 0;
		}
		else
		{
			CLinkMap::iterator iter = m_LinkFrom.find((int)pOutFogRspRef->GlobalNo);
			if (iter != m_LinkFrom.end())
				iter->second.LinkStatus = LS_Login;
			//SubTopics(iter->second);
		}
		break;
	}
	case TID_RecvTopicAction:
	{
		if (pOutFogRspRef->SubjectNo != 0)
		{
			PRINT_TO_STD("LinkID[%0.f]:Sub Error[%s]", pOutFogRspRef->GlobalNo, pOutFogRspRef->Remark);
			break;
		}
		CTopicActionField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		REPORT_EVENT(LOG_ERROR, "RspField", "LinkID[%0.f]:Sub[%s_%s] %s", pOutFogRspRef->GlobalNo, field.TopicID.getValue(), field.Index.getValue(), pOutFogRspRef->Remark);
		break;
	}
	case TID_RecvQryInstrument:
	{
		if (pRspBuffer == NULL)
			break;
		CInstrumentField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		string instrumentname = field.ExchangeID.getString();
		instrumentname += "_";
		instrumentname += field.InstrumentID;
		REPORT_EVENT(LOG_ERROR, "RspField", "LinkID[%0.f]:Instrument:[%s],[%s]", pOutFogRspRef->GlobalNo, instrumentname.c_str(), field.Remark.getValue());
		vector<string> configs = Txtsplit(field.Remark.getBuffer(), ";");
		for (int i = 0; i < configs.size(); i++)
		{
			vector<string> pairs = Txtsplit(configs[i], "|");
			if (pairs.size() < 2)
				continue;

			char cWorkerType = pairs[0].c_str()[0];

			string sInstrumentname = instrumentname;
			sInstrumentname += "_";
			sInstrumentname += cWorkerType;

			CPairMap::iterator iter = m_PairMap.find(pairs[1]);
			if (iter == m_PairMap.end())
			{
				m_PairMap.insert(CPairMap::value_type(pairs[1], sInstrumentname));
			}
			else
			{
				iter->second += ",";
				iter->second += sInstrumentname;
			}

			if (cWorkerType == WorkerTypeMaker)
				m_WorkerMap.insert(CWorkerMap::value_type(sInstrumentname, new CMarketWorker(field.ExchangeID, field.InstrumentID, this)));
			else if (cWorkerType == WorkerTypeIndex)
				m_WorkerMap.insert(CWorkerMap::value_type(sInstrumentname, new CIndexWorker(field.ExchangeID, field.InstrumentID, this)));
			else if (cWorkerType == WorkerTypeFast)
				m_WorkerMap.insert(CWorkerMap::value_type(sInstrumentname, new CFastWorker(field.ExchangeID, field.InstrumentID, this)));
		}

		//if (pOutFogRspRef->Chain == OUTFOG_CHAIN_LAST)
		//{
		//	bool bLinkFromLogin = true;
		//	CLinkMap::iterator iter = m_LinkFrom.begin();
		//	while (iter != m_LinkFrom.end())
		//	{
		//		if (iter->second.LinkStatus != LS_Login)
		//			bLinkFromLogin = false;
		//		iter++;
		//	}
		//	if (bLinkFromLogin)
		//	{
		//		CLinkMap::iterator iter = m_LinkFrom.begin();
		//		while (iter != m_LinkFrom.end())
		//		{
		//			SubTopics(iter->second);
		//			iter++;
		//		}
		//	}
		//}
		break;
	}
	case TID_PushMarketOverView:
	{
		CMarketOverViewField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		//PRINT_TO_STD("LinkID[%0.f]:MarketOverView:[%s_%s],[%s/%s]", pOutFogRspRef->GlobalNo, field.ExchangeID.getValue(), field.InstrumentID.getValue(),
		//	field.PositionFeeRate.getString().c_str(), field.UnderlyingPrice.getString().c_str());
		REPORT_EVENT(LOG_DEBUG, "MarketOverView", "LinkID[%0.f]:[%s_%s],[%s/%s/%s]", pOutFogRspRef->GlobalNo, field.ExchangeID.getValue(), field.InstrumentID.getValue(),
			field.PositionFeeRate.getString().c_str(), field.UnderlyingPrice.getString().c_str(), field.MarkedPrice.getString().c_str());
		string key = getWorkerKey((int)pOutFogRspRef->GlobalNo, field.ExchangeID, field.InstrumentID);
		vector<string> workers = Txtsplit(key, ",");
		for (int i = 0; i < workers.size(); i++)
		{
			CWorkerMap::iterator iter = m_WorkerMap.find(workers[i]);
			if (iter != m_WorkerMap.end())
				iter->second->SaveMarketOverView(&field);
		}
		break;
	}
	case TID_PushTenDepthMarketData:
	{
		if (!m_bReady)
			break;
		CMarketOverViewField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		//PRINT_TO_STD("TenMarketData:[%s_%s],[%0.4f/%0.2f|%0.4f/%0.2f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(),
		//	field.AskPrice1.getValue(), field.AskVolume1.getValue(), field.BidPrice1.getValue(), field.BidVolume1.getValue());
		//PRINT_TO_STD("TenMarketData:[%s:%s],[%0.4f/%0.2f|%0.4f/%0.2f||%0.4f/%0.2f|%0.4f/%0.2f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(),
		//	field.AskPrice2.getValue(), field.AskVolume2.getValue(), field.AskPrice1.getValue(), field.AskVolume1.getValue(),
		//	field.BidPrice1.getValue(), field.BidVolume1.getValue(), field.BidPrice2.getValue(), field.BidVolume2.getValue());
		string key = getWorkerKey((int)pOutFogRspRef->GlobalNo, field.ExchangeID, field.InstrumentID);
		vector<string> workers = Txtsplit(key, ",");
		for (int i = 0; i < workers.size(); i++)
		{
			CWorkerMap::iterator iter = m_WorkerMap.find(workers[i]);
			if (iter != m_WorkerMap.end())
				iter->second->SaveMarketOverView(&field);
		}
		break;
	}
	case TID_PushMarketOrder:
	{
		if (!m_bReady)
			break;
		CMarketOrderField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);

		////没有基准价，就不处理订单
		//if (m_LastPrice.isNull())
		//	break;

		REPORT_EVENT(LOG_DEBUG, "PushMarketOrder", "LinkID[%0.f][%s_%s],[%c/%0.4f/%0.2f]", pOutFogRspRef->GlobalNo, field.ExchangeID.getValue(), field.InstrumentID.getValue(),
			field.Direction.getValue(), field.Price.getValue(), field.Volume.getValue());
		//PRINT_TO_STD("MarketOrder:[%s_%s],[%c/%0.4f/%0.2f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(),
		//	field.Direction.getValue(), field.Price.getValue(), field.Volume.getValue());
		//if (field.Price > m_LastPrice.getValue()*1.01 || field.Price < m_LastPrice.getValue()*0.99)
		//{
		//	m_nSkipCount++;
		//}
		//else
		//{

		string key = getWorkerKey((int)pOutFogRspRef->GlobalNo, field.ExchangeID, field.InstrumentID);
		vector<string> workers = Txtsplit(key, ",");
		for (int i = 0; i < workers.size(); i++)
		{
			CWorkerMap::iterator iter = m_WorkerMap.find(workers[i]);
			if (iter != m_WorkerMap.end())
				iter->second->SaveMarketOrder(&field);
		}
		//}

		//m_nCount++;
		//CSecondsType thisTime = time(NULL);
		//if (thisTime != m_nTime)
		//{
		//	PRINT_TO_STD("MarketOrder: Recv  %d Messages in 1s , Skip %d Messages", m_nCount, m_nSkipCount);
		//	m_nCount = 0;
		//	m_nSkipCount = 0;
		//	m_nTime = thisTime;
		//}

		break;
	}
	case TID_PushMarketTrade:
	{
		if (!m_bReady)
			break;
		CMarketTradeField field;
		field.m_Describe.getSingleField(&field, pRspBuffer);
		//m_LastPrice = field.Price;
		//PRINT_TO_STD("MarketTrade:ID[%s_%s],Price[%0.5f],Volume[%0.5f]", field.ExchangeID.getValue(),
		//	field.InstrumentID.getValue(), field.Price.getValue(), field.Volume.getValue());

		REPORT_EVENT(LOG_DEBUG, "PushMarketTrade", "LinkID[%0.f]ID[%s_%s], Price[%0.5f], Volume[%0.5f]", pOutFogRspRef->GlobalNo, field.ExchangeID.getValue(),
			field.InstrumentID.getValue(), field.Price.getValue(), field.Volume.getValue());

		string key = getWorkerKey((int)pOutFogRspRef->GlobalNo, field.ExchangeID, field.InstrumentID);
		vector<string> workers = Txtsplit(key, ",");
		for (int i = 0; i < workers.size(); i++)
		{
			CWorkerMap::iterator iter = m_WorkerMap.find(workers[i]);
			if (iter != m_WorkerMap.end())
				iter->second->SaveMarketTrade(&field);
		}
		break;
	}
	case TID_RecvOTCTradeInsert:
	{
		if (pOutFogRspRef->SubjectNo == 0)
		{
			break;
			COTCTradeField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);
			REPORT_EVENT(LOG_ERROR, "RspField", "LinkID[%0.f]:RecvOTCTradeInsert:[%s_%s][%s/%s,%0.4f,%0.4f]", pOutFogRspRef->GlobalNo, field.ExchangeID.getValue(), field.InstrumentID.getValue(),
				field.LocalID.getValue(), field.TradeID.getValue(), field.Price.getValue(), field.Volume.getValue());
		}
		else
		{
			COTCTradeInsertField field;
			if (pRspBuffer != NULL)
				field.m_Describe.getSingleField(&field, pRspBuffer);
			REPORT_EVENT(LOG_ERROR, "RspField", "LinkID[%0.f]:RecvOTCTradeInsert Error:[%d,%s][%s_%s][%s,%0.4f,%0.4f]", pOutFogRspRef->GlobalNo, pOutFogRspRef->SubjectNo, pOutFogRspRef->Remark, field.ExchangeID.getValue(), field.InstrumentID.getValue(),
				field.LocalID.getValue(), field.Price.getValue(), field.Volume.getValue());

			CInstrumentUpdateField outfield;
			outfield.ExchangeID = field.ExchangeID;
			outfield.InstrumentID = field.InstrumentID;
			outfield.VolumeTick.clear();
			outfield.PriceTick.clear();
			outfield.MaxOpenInterest.clear();
			outfield.MaxOrderVolume.clear();
			outfield.MinOrderCost.clear();
			outfield.MinOrderVolume.clear();
			outfield.BasisPrice.clear();

			switch (pOutFogRspRef->SubjectNo)
			{
			case ERROR_PRICE_NOT_ON_TICK:
				outfield.PriceTick = field.Price.getTick();
				break;
			case ERROR_VOLUME_NOT_ON_TICK:
				outfield.VolumeTick = field.Volume.getTick();
				break;
			case ERROR_OutOfMaxVolume:
				outfield.MaxOrderVolume = field.Volume;
				break;
			case ERROR_LessThanMinVolume:
				outfield.MinOrderVolume = field.Volume;
				break;
			}
			SendInstrumentUpdate(&outfield);
		}
		break;
	}
	case TID_PushTrade:
	{
		CTradeField field;
		CTradeField::m_Describe.getSingleField(&field, pRspBuffer);
		if (field.DeriveSource == DS_Normal)
		{
			REPORT_EVENT(LOG_ERROR, "RspField", "LinkID[%0.f]:RecvPushTrade:[%s_%s][%s,%c,%0.4f,%0.4f]", pOutFogRspRef->GlobalNo, field.ExchangeID.getValue(), field.InstrumentID.getValue(),
				field.OrderID.getValue(), field.Direction.getValue(), field.Price.getValue(), field.Volume.getValue());
		}
		break;
	}
	case TID_RecvCancelOrderInsert:
	case TID_RecvOrderInsert:
	{
		if (pOutFogRspRef->SubjectNo == 0)
		{
			break;
			COrderField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);
			REPORT_EVENT(LOG_ERROR, "RspField", "LinkID[%0.f]:RecvOrderInsert:[%s_%s][%s,%c,%0.4f,%0.4f/%0.4f]", pOutFogRspRef->GlobalNo, field.ExchangeID.getValue(), field.InstrumentID.getValue(),
				field.OrderID.getValue(), field.Direction.getValue(), field.Price.getValue(), field.VolumeTraded.getValue(), field.Volume.getValue());
		}
		else
		{
			COrderInsertField field;
			if (pRspBuffer != NULL)
				field.m_Describe.getSingleField(&field, pRspBuffer);
			REPORT_EVENT(LOG_CRITICAL, "RspField", "LinkID[%0.f]:RecvOrderInsert Error:[%d,%s][%s_%s][%s,%c,%0.4f]", pOutFogRspRef->GlobalNo, pOutFogRspRef->SubjectNo, pOutFogRspRef->Remark, field.ExchangeID.getValue(), field.InstrumentID.getValue(),
				field.LocalID.getValue(), field.Direction.getValue(), field.Price.getValue());

			CInstrumentUpdateField outfield;
			outfield.ExchangeID = field.ExchangeID;
			outfield.InstrumentID = field.InstrumentID;
			outfield.VolumeTick.clear();
			outfield.PriceTick.clear();
			outfield.MaxOpenInterest.clear();
			outfield.MaxOrderVolume.clear();
			outfield.MinOrderCost.clear();
			outfield.MinOrderVolume.clear();
			outfield.BasisPrice.clear();

			switch (pOutFogRspRef->SubjectNo)
			{
			case ERROR_PRICE_NOT_ON_TICK:
				outfield.PriceTick = field.Price.getTick();
				break;
			case ERROR_VOLUME_NOT_ON_TICK:
				outfield.VolumeTick = field.Volume.getTick();
				break;
			case ERROR_OutOfMaxVolume:
				outfield.MaxOrderVolume = field.Volume;
				break;
			case ERROR_LessThanMinVolume:
				outfield.MinOrderVolume = field.Volume;
				break;
			}
			SendInstrumentUpdate(&outfield);
		}
		break;
	}
	case TID_RecvOrderAction:
	{
		if (pOutFogRspRef->SubjectNo == 0)
		{
			break;
			COrderField field;
			field.m_Describe.getSingleField(&field, pRspBuffer);
			REPORT_EVENT(LOG_ERROR, "RspField", "LinkID[%0.f]:RecvOrderAction:[%s_%s][%s/%s,%c,%0.4f,%0.4f/%0.4f]", pOutFogRspRef->GlobalNo, field.ExchangeID.getValue(), field.InstrumentID.getValue(),
				field.LocalID.getValue(), field.OrderID.getValue(), field.Direction.getValue(), field.Price.getValue(), field.VolumeTraded.getValue(), field.Volume.getValue());
		}
		else
		{
			break;
			COrderActionField field;
			if (pRspBuffer != NULL)
				field.m_Describe.getSingleField(&field, pRspBuffer);
			REPORT_EVENT(LOG_ERROR, "RspField", "LinkID[%0.f]:RecvOrderAction Error:[%d,%s][%s/%s]", pOutFogRspRef->GlobalNo, pOutFogRspRef->SubjectNo, pOutFogRspRef->Remark, field.OrderLocalID.getValue(), field.OrderID.getValue());
		}
		break;
	}
	case TID_PushOrder:
	{
		break;
		COrderField field;
		COrderField::m_Describe.getSingleField(&field, pRspBuffer);
		//这里确保万无一失
		if (field.OrderStatus == OS_AllTraded || field.OrderStatus == OS_AllCanceled || field.OrderStatus == OS_PartCanceled)
			field.VolumeRemain = 0.0;

		//PRINT_TO_STD("RecvPushOrder:[%s_%s][%s/%s,%c,%0.4f,%0.4f/%0.4f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(),
		//	field.LocalID.getValue(), field.OrderID.getValue(), field.Direction.getValue(), field.Price.getValue(), field.VolumeRemain.getValue(), field.Volume.getValue());

		string key = field.ExchangeID.getValue();
		key += "_";
		key += field.InstrumentID.getValue();
		CWorkerMap::iterator iter = m_WorkerMap.find(key);
		if (iter != m_WorkerMap.end())
			iter->second->SaveOrder(&field);
		break;
	}
	case TID_RecvQryOrder:
	{
		if (pRspBuffer != NULL)
		{
			m_nInitOrders++;
			COrderField field;
			COrderField::m_Describe.getSingleField(&field, pRspBuffer);

			//对本地编号做优化
			if (field.LocalID.getInt() > m_nLocalID)
				m_nLocalID = field.LocalID.getInt();

			//printf("%s \n", field.InstrumentID.getValue());
			string key = field.ExchangeID.getValue();
			key += "_";
			key += field.InstrumentID.getValue();
			key += "_";

			string key1 = key + WorkerTypeMaker;

			CWorkerMap::iterator iter = m_WorkerMap.find(key);
			if (iter != m_WorkerMap.end())
				iter->second->InitOrder(&field);

			string key2 = key + WorkerTypeFast;
			iter = m_WorkerMap.find(key);
			if (iter != m_WorkerMap.end())
				iter->second->InitOrder(&field);

		}

		if ((pOutFogRspRef->Chain == OUTFOG_CHAIN_LAST || pOutFogRspRef->Chain == OUTFOG_CHAIN_SINGLE) && m_bReady == false)
		{
			//PRINT_TO_STD("LinkID[%0.f]:RecvQryOrder End,Begin To SubTopics", pOutFogRspRef->GlobalNo);
			////订阅2号交易流水
			//SubTopics(m_LinkTo);
			m_bReady = true;

			CWorkerMap::iterator iter = m_WorkerMap.begin();
			while (iter != m_WorkerMap.end())
			{
				iter->second->InitOrder(NULL);
				iter++;
			}
			REPORT_EVENT(LOG_ERROR, "RecvQryOrder", "m_nInitOrders:%d", m_nInitOrders);
			m_nInitOrders = 0;
		}
		break;
	}
	//case TID_PushInstrumentStatus:
	//{
	//	CInstrumentStatusField field;
	//	CInstrumentStatusField::m_Describe.getSingleField(&field, pRspBuffer);

	//	PRINT_TO_STD("RecvPushInstrumentStatus1:[%s:%s][%c]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.InstrumentStatus.getValue());
	//	if (field.InstrumentStatus != IS_Continous)
	//		field.InstrumentStatus = IS_NoTrading;
	//	PRINT_TO_STD("RecvPushInstrumentStatus2:[%s:%s][%c]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.InstrumentStatus.getValue());

	//	//string key = getWorkerKey(field.ExchangeID, field.InstrumentID);
	//	CWorkerMap::iterator iter = m_WorkerMap.begin();
	//	while (iter != m_WorkerMap.end())
	//	{
	//		CWorkerInterface *pWorker = iter->second;
	//		if (!strncmp(field.InstrumentID, pWorker->GetInstrumentID(), strlen(field.InstrumentID)))
	//		{
	//			CWorkerInterface *pWorker = iter->second;
	//			CInstrumentStatusField newfield;
	//			CopyInstrumentStatusEntity(&newfield, &field);
	//			newfield.InstrumentID = pWorker->GetInstrumentID();
	//			newfield.ExchangeID = pWorker->GetExchangeID();

	//			CInstrumentStatusField::m_Describe.setSingleField(m_streamBuf, &newfield);

	//			m_OutFogMetaRef.ServiceNo = m_LinkTo.LinkNo;
	//			m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	//			m_OutFogMetaRef.TNo = TID_SendInstrumentStatusUpdate;
	//			m_OutFogMetaRef.FieldNo = newfield.m_Describe.m_FieldID;
	//			m_OutFogMetaRef.Length = newfield.m_Describe.m_nStreamSize;
	//			m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	//			m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
	//		}
	//		iter++;
	//	}
	//	break;
	//}
	default:
	{
	}
	}
	return true;
}

string CMarketMaker::getWorkerKey(int nLinkNo, CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID)
{
	string key = "";
	char sLinkNo[8];
	if (ExchangeID.isNull())
	{
		key = InstrumentID.getValue();
		key += "_";
		sprintf(sLinkNo, "%d", nLinkNo);
		key += sLinkNo;
	}
	else
	{
		key = ExchangeID.getValue();
		key += "_";
		key += InstrumentID.getValue();
		key += "_";
		sprintf(sLinkNo, "%d", nLinkNo);
		key += sLinkNo;
	}

	CPairMap::iterator iter = m_PairMap.find(key);
	if (iter == m_PairMap.end())
	{
		return key;
	}
	else
	{
		return iter->second;
	}
}

bool CMarketMaker::SendOTCTrade(COTCTradeInsertField* pField)
{
	pField->BuyTradeUnitID = m_TradeUnitID;
	pField->BuyMemberID = m_MemberID;
	pField->SellTradeUnitID = m_TradeUnitID;
	pField->SellMemberID = m_MemberID;

	//查找持仓，决定是开平
	pField->BuyOffsetFlag = "0";
	pField->SellOffsetFlag = "0";
	//pField->OTCType = OTCT_WashOnly;
	pField->OTCType = OTCT_WashOnly;

	COTCTradeInsertField::m_Describe.setSingleField(m_streamBuf, pField);
	m_OutFogMetaRef.GlobalNo = m_LinkTo.LinkNo;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	m_OutFogMetaRef.MessageNo = TID_SendOTCTradeInsert;
	m_OutFogMetaRef.FieldNo = pField->m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = pField->m_Describe.m_nStreamSize;
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;

	//printf("%s:%s \n", pField->InstrumentID.getString().c_str(),pField->Price.getString().c_str());

	m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
	return true;
}

bool CMarketMaker::SendOrder(COrderInsertField* pField)
{
	m_OutFogMetaRef.MessageNo = TID_SendCancelOrderInsert;
	//pField->TradeUnitID = m_TradeUnitID;
	pField->MemberID = m_MemberID;
	pField->LocalID = GetLocalID();

	//查找持仓，决定是开平
	pField->OffsetFlag = "0";

	COrderInsertField::m_Describe.setSingleField(m_streamBuf, pField);
	m_OutFogMetaRef.GlobalNo = m_LinkTo.LinkNo;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	m_OutFogMetaRef.FieldNo = pField->m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = pField->m_Describe.m_nStreamSize;
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	m_OutFogMetaRef.RequestNo = -2;

	//PRINT_TO_STD("SendOrder:[%s_%s],[%s],[%c,%0.4f,%0.2f]", pField->ExchangeID.getValue(), pField->InstrumentID.getValue(),
	//	pField->LocalID.getValue(),pField->Direction.getValue(),pField->Price.getValue(), pField->Volume.getValue());

	return m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
}

char* CMarketMaker::GetLocalID()
{
	sprintf(m_sLocalID, "%020d", m_nLocalID);
	m_nLocalID++;
	return m_sLocalID;
}

bool CMarketMaker::CancelOrder(COrderField* pField)
{
	COrderActionField field;
	memset(&field, 0, sizeof(COrderActionField));
	field.ActionFlag = AF_Delete;
	//field.LocalID = GetLocalID();
	//field.OrderID = pField->OrderID;
	field.OrderLocalID = pField->LocalID;

	if (field.OrderID.isNull() && field.OrderLocalID.isNull())
	{
		//这种情况非常危险
		PRINT_TO_STD("CancleOrder Error:[%s_%s][%s/%s][%c,%0.4f,%0.2f]", pField->ExchangeID.getValue(), pField->InstrumentID.getValue(),
			field.OrderLocalID.getValue(), pField->OrderID.getValue(), pField->Direction.getValue(), pField->Price.getValue(), pField->Volume.getValue());
		return false;
	}

	COrderActionField::m_Describe.setSingleField(m_streamBuf, &field);
	m_OutFogMetaRef.GlobalNo = m_LinkTo.LinkNo;
	m_OutFogMetaRef.FieldNo = field.m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = field.m_Describe.m_nStreamSize;
	m_OutFogMetaRef.MessageNo = TID_SendOrderAction;
	//不要求回复正确结果，这样可以减少服务器压力
	m_OutFogMetaRef.RequestNo = -1;

	//PRINT_TO_STD("CancleOrder:[%s_%s][%s/%s][%c,%0.4f,%0.2f]", pField->ExchangeID.getValue(), pField->InstrumentID.getValue(),
	//	field.OrderLocalID.getValue(),pField->OrderID.getValue(), pField->Direction.getValue(), pField->Price.getValue(), pField->Volume.getValue());

	return m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
}

bool CMarketMaker::SendMarketUpdate(CMarketDataUpdateField* pField)
{
	CMarketDataUpdateField::m_Describe.setSingleField(m_streamBuf, pField);
	m_OutFogMetaRef.GlobalNo = m_LinkTo.LinkNo;
	m_OutFogMetaRef.FieldNo = pField->m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = pField->m_Describe.m_nStreamSize;
	m_OutFogMetaRef.MessageNo = TID_SendMarketDataUpdate;
	//不要求回复正确结果，这样可以减少服务器压力
	m_OutFogMetaRef.RequestNo = -1;
	return m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
}

bool CMarketMaker::SendInstrumentUpdate(CInstrumentUpdateField* pField)
{
	CInstrumentUpdateField::m_Describe.setSingleField(m_streamBuf, pField);
	m_OutFogMetaRef.GlobalNo = m_LinkTo.LinkNo;
	m_OutFogMetaRef.FieldNo = pField->m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = pField->m_Describe.m_nStreamSize;
	m_OutFogMetaRef.MessageNo = TID_SendInstrumentUpdate;
	//不要求回复正确结果，这样可以减少服务器压力
	m_OutFogMetaRef.RequestNo = -1;
	return m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
}

bool CMarketMaker::GetOrder(CQryOrderField* pField)
{
	pField->MemberID = m_MemberID;
	m_OutFogMetaRef.GlobalNo = m_LinkTo.LinkNo;
	m_OutFogMetaRef.MessageNo = TID_SendQryOrder;
	m_OutFogMetaRef.FieldNo = pField->m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = pField->m_Describe.m_nStreamSize;
	CQryOrderField::m_Describe.setSingleField(m_streamBuf, pField);
	return m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
}

void CMarketMaker::SubTopics(CLinkField linkField)
{
	CTopicActionField field;
	TOutFogMetaRef OutFogMetaRef;
	OutFogMetaRef.FieldNo = field.m_Describe.m_FieldID;
	OutFogMetaRef.Length = field.m_Describe.m_nStreamSize;
	OutFogMetaRef.GlobalNo = linkField.LinkNo;

	OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
	OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
	OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	OutFogMetaRef.RequestNo = 0;

	field.Action = SA_Sub;
	field.ResumeNo = -1;

	if (linkField.LinkNo == m_LinkTo.LinkNo)
	{
		//目前不需要订阅私有流水，算法通过本地处理来加速报价
		//field.TopicID = 3;
		//field.FilterValue = m_LinkTo.LinkUserID;
		//m_OutFogMetaRef.GlobalNo = linkField.LinkNo;
		//m_OutFogMetaRef.MessageNo = TID_SendTopicAction;
		//CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
		//m_pServiceApi->ReqField(&m_OutFogMetaRef, m_streamBuf);
	}
	else
	{
		CPairMap::iterator iter1 = m_PairMap.begin();
		while (iter1 != m_PairMap.end())
		{
			field.Index.clear();
			vector<string> sFilterValue = Txtsplit(iter1->first, "_");
			for (int i = 0; i < sFilterValue.size() - 1; i++)
			{
				field.Index += sFilterValue[i].c_str();
			}

			if (sFilterValue[sFilterValue.size() - 1] != linkField.LinkNo.getString())
			{
				iter1++;
				continue;
			}

			OutFogMetaRef.MessageNo = TID_SendTopicAction;
			CTopicActionField::m_Describe.setSingleField(m_streamBuf, &field);
			m_pServiceApi->ReqField(&OutFogMetaRef, m_streamBuf);
			iter1++;
		}
	}

	//开放push流水
	OutFogMetaRef.Type = OUTFOG_TYPE_SUBTOPIC;
	m_pServiceApi->ReqField(&OutFogMetaRef, NULL);
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

	if (argc > 1)
	{
		if (isValidNumber(argv[1]))
		{
			g_nKernelID = atoi(argv[1]);
		}
	}

	TIniFile Ini;
	Ini.Open(NETWORK_FILE_NAME);

	CMarketMaker* pApp = new CMarketMaker();
	COutFogService* m_pService = new COutFogService();
	COutFogServiceApi* pOutFogServiceApi = m_pService->LoadOutFogApi("omqsapi", pApp, g_nKernelID);
	pApp->setServiceApi(pOutFogServiceApi);

	TOutFogMetaRef OutFogMetaRef;
	OutFogMetaRef.Type = OUTFOG_TYPE_START;
	pOutFogServiceApi->ReqField(&OutFogMetaRef, NULL);

	char sLink[64];
	sprintf(sLink, "MarketMaker_%d_Links", g_nKernelID);
	CLinkField link;
	link.LinkNo = Ini.ReadInt(sLink, "To", 1);

	char slinks[1024];
	Ini.ReadString(sLink, "From", "", slinks, sizeof(slinks) - 1);

	sprintf(sLink, "MarketMaker_%d_Link%d", g_nKernelID, link.LinkNo.getValue());
	Ini.ReadString(sLink, "LinkAddress", "", link.LinkAddress.getBuffer(), sizeof(link.LinkAddress) - 1);
	Ini.ReadString(sLink, "LinkDllName", "", link.LinkDllName.getBuffer(), sizeof(link.LinkDllName) - 1);
	Ini.ReadString(sLink, "LinkDllLoad", "", link.LinkDllLoad.getBuffer(), sizeof(link.LinkDllLoad) - 1);
	Ini.ReadString(sLink, "LinkUserID", "", link.LinkUserID.getBuffer(), sizeof(link.LinkUserID) - 1);
	Ini.ReadString(sLink, "LinkPassword", "", link.LinkPassword.getBuffer(), sizeof(link.LinkPassword) - 1);
	Ini.ReadString(sLink, "Config", "", link.Config.getBuffer(), sizeof(link.Config) - 1);

	memcpy(&(pApp->m_LinkTo), &link, sizeof(CLinkField));

	char StreamBuf[4096];
	CLinkField::m_Describe.setSingleField(StreamBuf, &link);

	OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
	OutFogMetaRef.Type = OUTFOG_TYPE_INITLINK;
	strncpy(OutFogMetaRef.SubIndexName, link.LinkDllName.getValue(), sizeof(OutFogMetaRef.SubIndexName));
	OutFogMetaRef.GlobalNo = link.LinkNo.getValue();
	OutFogMetaRef.MessageNo = TID_SendInitLink;
	OutFogMetaRef.FieldNo = link.m_Describe.m_FieldID;
	OutFogMetaRef.Length = link.m_Describe.m_nStreamSize;
	OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	pOutFogServiceApi->ReqField(&OutFogMetaRef, StreamBuf);

	vector<string> SLinks = Txtsplit(slinks, ",");
	for (int i = 0; i < SLinks.size(); i++)
	{
		link.LinkNo = atoi(SLinks[i].c_str());
		sprintf(sLink, "MarketMaker_%d_Link%d", g_nKernelID, link.LinkNo.getValue());
		Ini.ReadString(sLink, "LinkAddress", "", link.LinkAddress.getBuffer(), sizeof(link.LinkAddress) - 1);
		Ini.ReadString(sLink, "LinkDllName", "", link.LinkDllName.getBuffer(), sizeof(link.LinkDllName) - 1);
		Ini.ReadString(sLink, "LinkDllLoad", "", link.LinkDllLoad.getBuffer(), sizeof(link.LinkDllLoad) - 1);
		Ini.ReadString(sLink, "LinkUserID", "", link.LinkUserID.getBuffer(), sizeof(link.LinkUserID) - 1);
		Ini.ReadString(sLink, "LinkPassword", "", link.LinkPassword.getBuffer(), sizeof(link.LinkPassword) - 1);
		Ini.ReadString(sLink, "Config", "", link.Config.getBuffer(), sizeof(link.Config) - 1);

		CLinkField::m_Describe.setSingleField(StreamBuf, &link);

		strncpy(OutFogMetaRef.SubIndexName, link.LinkDllName.getValue(), sizeof(OutFogMetaRef.SubIndexName));
		OutFogMetaRef.GlobalNo = link.LinkNo.getValue();
		pOutFogServiceApi->ReqField(&OutFogMetaRef, StreamBuf);
		link.LinkStatus = LS_NotActive;
		pApp->m_LinkFrom.insert(CLinkMap::value_type(link.LinkNo.getValue(), link));
	}

	while (true)
	{
		SLEEP_SECONDS(3);
		if (pApp->m_bReady)
		{
			CLinkMap::iterator iter = pApp->m_LinkFrom.begin();
			while (iter != pApp->m_LinkFrom.end())
			{
				if (iter->second.LinkStatus == LS_Login)
				{
					pApp->SubTopics(iter->second);
					iter->second.LinkStatus = '3';
				}
				iter++;
			}
		}
	}
	return 0;
}

