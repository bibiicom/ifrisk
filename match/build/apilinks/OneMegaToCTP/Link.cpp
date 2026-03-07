#include "Link.h"
#include "public.h"
#include "BaseFunction.h"
#include "printinfo.h"
#include "utils.h"

CLink::CLink()
{
	m_pCallback = NULL;
	m_nMessages = 0;
}

CLink::~CLink()
{

}

COneMegaApi *CLink::CreateApi()
{
	return new CLink();
}

const char * CLink::GetVersion()
{
	return "1.0";
}

extern "C" COneMegaApi* CreateOneMegaApi()
{
	return new CLink();
}

void CLink::Connect()
{
	PRINT_TO_STD("LinkNo[%d]:Address[%s]", m_nLinkNo, m_sAddress.c_str());
	m_pThostFtdcMdApi = CThostFtdcMdApi::CreateFtdcMdApi();
	m_pThostFtdcMdApi->RegisterSpi(this);

	vector<string> address = Txtsplit(m_sAddress,";");
	for (int i = 0; i < address.size(); i++)
	{
		m_pThostFtdcMdApi->RegisterFront((char*)(address[i].c_str()));
	}	
	
	m_pThostFtdcMdApi->Init();
}

void CLink::SetAddress(const char *pAddress)
{
	m_sAddress = pAddress;
}

void CLink::SetCallback(COneMegaCallback *pCallback)
{
	m_pCallback = pCallback;
}

int CLink::SendInitLink(MegaLinkStruct *pLink, int nRequestNo)
{
	m_nLinkNo = pLink->LinkNo;
	return 0;
}

int CLink::SendUserLogin(MegaUserLoginStruct *pUserLogin, int nRequestNo)
{
	//先做行情，无需登录，直接返回登录成功
	MegaUserSessionStruct field;
	memset(&field, 0, sizeof(MegaUserSessionStruct));
	strncpy(field.UserID, pUserLogin->UserID, sizeof(field.UserID));
	strncpy(field.Token, pUserLogin->Token, sizeof(field.Token));
	m_pCallback->RecvUserLogin(NULL, &field, 0, "Success", nRequestNo, ONE_CHAIN_SINGLE);
	return 0;
}

int CLink::SendTopicAction(MegaTopicActionStruct *pTopicAction, int nRequestNo)
{
	char *ppInstrumentID[] = { "" };
	ppInstrumentID[0] = pTopicAction->Index;
	m_sTopics.insert(pTopicAction->Index);
	//ppInstrumentID[0] = "rb2401";
	PRINT_TO_STD("LinkNo[%d]:SendTopicAction[%s]", m_nLinkNo, ppInstrumentID[0]);
	int ret = m_pThostFtdcMdApi->SubscribeMarketData(ppInstrumentID, 1);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLink::OnFrontConnected()
{
	CThostFtdcReqUserLoginField field;
	memset(&field, 0, sizeof(field));
	int ret = m_pThostFtdcMdApi->ReqUserLogin(&field, 0);
	m_pCallback->OnConnected();
}

void CLink::OnFrontDisconnected(int nReason)
{
	PRINT_TO_STD("LinkNo[%d]:OnFrontDisconnected",m_nLinkNo);
	m_pCallback->OnDisconnected(0);
}

void CLink::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//PRINT_TO_STD("LinkNo[%d]:OnRtnDepthMarketData:%s/%0.2f", m_nLinkNo, pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice);
	m_nMessages++;
	if (fmod(m_nMessages, 100000) == 0)
		PRINT_TO_STD("LinkNo[%d]:m_nMessages[%0.f]", m_nLinkNo, m_nMessages);

	MegaMarketOrderStruct field;
	memset(&field, 0, sizeof(MegaMarketOrderStruct));
	strcpy(field.InstrumentID, pDepthMarketData->InstrumentID);
	strcpy(field.ExchangeID, pDepthMarketData->ExchangeID);

	
	field.Direction = Mega_Direction_Sell;
	field.Price = pDepthMarketData->AskPrice1;
	field.Volume = pDepthMarketData->AskVolume1;
	m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	//field.Price = pDepthMarketData->AskPrice2;
	//field.Volume = pDepthMarketData->AskVolume2;
	//m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	//field.Price = pDepthMarketData->AskPrice3;
	//field.Volume = pDepthMarketData->AskVolume3;
	//m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	//field.Price = pDepthMarketData->AskPrice4;
	//field.Volume = pDepthMarketData->AskVolume4;
	//m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	//field.Price = pDepthMarketData->AskPrice5;
	//field.Volume = pDepthMarketData->AskVolume5;
	//m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);

	field.Direction = Mega_Direction_Buy;
	field.Price = pDepthMarketData->BidPrice1;
	field.Volume = pDepthMarketData->BidVolume1;
	m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	//field.Price = pDepthMarketData->BidPrice2;
	//field.Volume = pDepthMarketData->BidVolume2;
	//m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	//field.Price = pDepthMarketData->BidPrice3;
	//field.Volume = pDepthMarketData->BidVolume3;
	//m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	//field.Price = pDepthMarketData->BidPrice4;
	//field.Volume = pDepthMarketData->BidVolume4;
	//m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	//field.Price = pDepthMarketData->BidPrice5;
	//field.Volume = pDepthMarketData->BidVolume5;
	//m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);

	MegaMarketTradeStruct trade;
	memset(&trade, 0, sizeof(MegaMarketTradeStruct));
	strcpy(trade.InstrumentID, pDepthMarketData->InstrumentID);
	trade.Volume = 0.0;
	map<string, double>::iterator iter = m_mapVolume.find(pDepthMarketData->InstrumentID);
	if (iter == m_mapVolume.end())
	{
		m_mapVolume.insert(map<string, double>::value_type(pDepthMarketData->InstrumentID, pDepthMarketData->Volume));
	}
	else
	{
		double volume = pDepthMarketData->Volume / 2;
		if (volume <= iter->second)
			iter->second = volume;
		else
			trade.Volume = volume - iter->second;
	}

	if (trade.Volume > 0.0)
	{
		trade.Price = pDepthMarketData->LastPrice;
		double LastPrice = trade.Price;
		iter = m_mapPrice.find(trade.InstrumentID);
		if (iter == m_mapPrice.end())
		{
			m_mapPrice.insert(map<string, double>::value_type(trade.InstrumentID, trade.Price));
		}
		else
		{
			LastPrice = iter->second;
			iter->second = trade.Price;
		}

		if (LastPrice != trade.Price)
		{
			if (LastPrice > trade.Price)
				trade.Direction = Mega_Direction_Sell;
			else
				trade.Direction = Mega_Direction_Buy;
		}

		m_pCallback->PushMarketTrade(&trade, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	}
}

void CLink::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	PRINT_TO_STD("LinkNo[%d]:OnRspUserLogin:%d/%s", m_nLinkNo, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	//if (pRspInfo->ErrorID == 0)
	//{
	//	MegaUserSessionStruct field;
	//	memset(&field, 0, sizeof(MegaUserSessionStruct));
	//	strncpy(field.UserID, pRspUserLogin->UserID, sizeof(field.UserID));
	//	m_pCallback->RecvUserLogin(NULL, &field, 0, "Success", nRequestID, ONE_CHAIN_SINGLE);
	//}
	set<string>::iterator iter = m_sTopics.begin();
	while (iter != m_sTopics.end())
	{
		char* ppInstrumentID[] = { "" };
		ppInstrumentID[0] = (char*)iter->c_str();
		PRINT_TO_STD("LinkNo[%d]:ReConnect:SendTopicAction[%s]", m_nLinkNo, ppInstrumentID[0]);
		int ret = m_pThostFtdcMdApi->SubscribeMarketData(ppInstrumentID, 1);
		iter++;
	}
}

void CLink::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	PRINT_TO_STD("LinkNo[%d]:OnRspError:%d/%s", m_nLinkNo, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
}

void CLink::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	PRINT_TO_STD("LinkNo[%d]:OnRspSubMarketData:%s:%d/%s", m_nLinkNo, pSpecificInstrument->InstrumentID, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
}
