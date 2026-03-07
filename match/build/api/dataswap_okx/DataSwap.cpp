#include "DataSwap.h"
#include "BaseFunction.h"
#include "FieldMap.h"

CDataSwitchEngine::CDataSwitchEngine()
{
}

CDataSwitchEngine::~CDataSwitchEngine(void)
{
}

bool CDataSwitchEngine::ReqField(const TOutFogMetaRef* pReqRef, const void *pStream)
{
	return true;
}

bool CDataSwitchEngine::SyncReqField(const TOutFogMetaRef* pReqRef, const void *pReqBuffer, TOutFogMetaRef* pRspRef, void *pRspBuffer)
{
	switch (pReqRef->IsReplay)
	{
	case 0: //request
	{
		Replay0(pReqRef, (const char*)pReqBuffer, pRspRef, pRspBuffer);
		break;
	}
	case 1: //response
	{
		Replay1(pReqRef, (const char*)pReqBuffer, pRspRef, pRspBuffer);
		break;
	}
	case 2://response
	{
		Replay2(pReqRef, (const char*)pReqBuffer, pRspRef, (char*)pRspBuffer);
		break;
	}
	case 3://error
	{
		switch (pReqRef->FrontNo)
		{
		case 1:
		case 2:
		case 3:
			sprintf((char*)pRspBuffer, "{\"a\":\"%s\",\"n\":%d,\"c\":%d,\"m\":\"\%s\"}", pReqRef->SubIndexName, pReqRef->RequestNo, pReqRef->SubjectNo, pReqBuffer);
			break;
		case 4:
			sprintf((char*)pRspBuffer, "{a:%s,n:%d,c:%d,m:%s}", pReqRef->SubIndexName, pReqRef->RequestNo, pReqRef->SubjectNo, pReqBuffer);
			break;
		default:
			sprintf((char*)pRspBuffer, "{\"action\":\"%s\",\"requestNo\":%d,\"errorCode\":%d,\"errorMsg\":\"%s\"}", pReqRef->SubIndexName, pReqRef->RequestNo, pReqRef->SubjectNo, pReqBuffer);
		}
		break;
	}
	default:
	{
	}
	}
	return true;
}

void* CDataSwitchEngine::Replay2(const TOutFogMetaRef* pReqRef, const void *pReqBuffer, TOutFogMetaRef* pRspRef, char *pRspBuffer)
{
	switch (pReqRef->FieldNo)
	{
	case -1:
	{
		sprintf(pRspBuffer, "{\"type\":\"snapshot\",\"topic\":\"orderbook.10.%s\",\"ts\":%d,\"data\":",
			pReqRef->Remark, pReqRef->SequenceNo);
		break;
	}
	case -2:
	{
		sprintf(pRspBuffer, "{\"type\":\"snapshot\",\"topic\":\"orderbook.10.%s\",\"ts\":%d,\"data\":",
			pReqRef->Remark, pReqRef->SequenceNo);
		break;
	}
	case -3:
	{
		if (pReqRef->GlobalNo == 0.0)
		{
			switch (pReqRef->MessageNo)
			{
			case TID_PushMarketOrder:
			case TID_RecvQryMarketOrder:
				strcpy(pRspBuffer, "{}");
				break;
			}
		}
		else
		{
			switch (pReqRef->MessageNo)
			{
			case TID_PushMarketOrder:
			case TID_RecvQryMarketOrder:
				strcpy(pRspBuffer, "]}}");
				break;
			}
		}
		break;
	}
	default:
	{
		DC(pReqRef, pReqBuffer, pRspRef, pRspBuffer);
	}
	}
	return NULL;
}

bool CDataSwitchEngine::DC(const TOutFogMetaRef* pReqRef, const void *pReqBuffer, TOutFogMetaRef* pRspRef, char *pRspBuffer)
{
	switch (pReqRef->FieldNo)
	{
	case FID_MarketOrder:
	{
		CMarketOrderField field;
		CMarketOrderField::m_Describe.getSingleField(&field, pReqBuffer);
		if (field.Direction == D_Buy)
		{
			strcpy(pRspRef->SubIndexName, "b");
			if (pRspRef->FieldNo == 1)
				pRspRef->RequestNo = 1;
			else
				pRspRef->RequestNo = 0;
			pRspRef->FieldNo = 1;
		}
		else
		{
			strcpy(pRspRef->SubIndexName, "a");
			if (pRspRef->FieldNo == 2)
				pRspRef->RequestNo = 1;
			else
				pRspRef->RequestNo = 0;
			pRspRef->FieldNo = 2;
		}

		if (pReqRef->GlobalNo == 0.0)
		{
			sprintf(pRspBuffer, "{\"s\":\"%s\",\"%s\":[[%s,%s]", pReqRef->Remark, pRspRef->SubIndexName, field.Price.getString().c_str(), field.Volume.getString().c_str());
		}
		else
		{
			if (pRspRef->RequestNo == 0)
				sprintf(pRspBuffer, "],\"%s\":[[%s]", pRspRef->SubIndexName, field.Price.getString().c_str(), field.Volume.getString().c_str());
			else
				sprintf(pRspBuffer, ",[%s,%s]", field.Price.getString().c_str(), field.Volume.getString().c_str());
			break;
		}
	}
	}
	return true;
}

const char* CDataSwitchEngine::Replay0(const TOutFogMetaRef* pReqRef, const char* pReqBuffer, TOutFogMetaRef* pRspRef, void* pRspBuffer)
{
	string strData((char*)pReqBuffer, pReqRef->Length);
	////printf("%s \n", strData.c_str());
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(strData, root))
		return NULL;

	try
	{
		if (root["instId"].isNull())
			return NULL;
		((COrderInsertField*)pRspBuffer)->InstrumentID = root["instId"].asCString();
		((COrderInsertField*)pRspBuffer)->LocalID = root["clOrdId"].asCString();
		if (root["side"]=="long")
			((COrderInsertField*)pRspBuffer)->Direction = D_Buy;
		else
			((COrderInsertField*)pRspBuffer)->Direction = D_Sell;

		pRspRef->FieldNo = COrderInsertField::m_Describe.m_FieldID;
	}
	catch (...)
	{
		sprintf((char*)pRspBuffer, "Error:eed");
		return NULL;
	}
}


const char* CDataSwitchEngine::Replay1(const TOutFogMetaRef* pReqRef, const char *pReqBuffer, TOutFogMetaRef* pRspRef, void *pRspBuffer)
{
	//string strData((char*)pReqBuffer, pReqRef->Length);
	////printf("%s \n", strData.c_str());
	//Json::Reader reader;
	//Json::Value root;
	//if (!reader.parse(strData, root))
	//	return NULL;
	try
	{
	//	if (root["args"].isNull())
	//		return NULL;


		CQryMarketOrderField field;
		field.ExchangeID = "Test";
		field.InstrumentID = "BTCUSDT";
		pRspRef->FieldNo = field.m_Describe.m_FieldID;
		pRspRef->MessageNo = TID_SendQryMarketOrder;
		pRspRef->RequestNo = 0;
		pRspRef->Length = field.m_Describe.GetStreamSize();
		field.m_Describe.setSingleField(pRspBuffer, &field);
		return NULL;

		//Json::Value& topics = root["args"];
		//for (int i = 0; i < topics.size(); i++)
		//{

		//}
		//CTopicActionField field;
		//field.Action = SA_Sub;
		//field.FilterValue = "Test_BTCUSDT";
		//field.ResumeNo = -1;
		//pRspRef->FieldNo = field.m_Describe.m_FieldID;
		//pRspRef->Length = field.m_Describe.GetStreamSize();
		//CTopicActionField::m_Describe.setSingleField(pRspBuffer, &field);
	}
	catch (...)
	{
		printf("execption\n");
	}
	return NULL;
}

const char* CDataSwitchEngine::getVersion()
{
	static char version[256];
	sprintf(version, "Service version: %s", "dataswitch");
	return version;
}

void CDataSwitchEngine::setServiceCallback(COutFogServiceCallback *pCallback)
{
	m_pServiceCallback = pCallback;
}

extern "C" COutFogServiceApi* CreateOutFogServiceApi(COutFogServiceCallback *pCallback, int nServiceID)
{
	return new CDataSwitchEngine();
}
