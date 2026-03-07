#include "Link.h"
#include "public.h"
#include "BaseFunction.h"
#include "printinfo.h"
#include "utils.h"
//#include "zlibuse.h"

// subscribe channel without login
/*
 swap/ticker // 行情数据频道
 swap/candle60s // 1分钟k线数据频道
 swap/candle180s // 3分钟k线数据频道
 swap/candle300s // 5分钟k线数据频道
 swap/candle900s // 15分钟k线数据频道
 swap/candle1800s // 30分钟k线数据频道
 swap/candle3600s // 1小时k线数据频道
 swap/candle7200s // 2小时k线数据频道
 swap/candle14400s // 4小时k线数据频道
 swap/candle21600 // 6小时k线数据频道
 swap/candle43200s // 12小时线数据频道
 swap/candle180s // 3分钟k线数据频道
 swap/candle300s // 5分钟k线数据频道
 swap/candle900s // 15分钟k线数据频道
 swap/candle1800s // 30分钟k线数据频道
 swap/candle3600s // 1小时k线数据频道
 swap/candle7200s // k线数据频道
 swap/candle86400s // 1day k线数据频道
 swap/candle604800s // 1week k线数据频道
 swap/trade // 交易信息频道
 swap/funding_rate//资金费率频道
 swap/price_range//限价范围频道
 swap/depth //深度数据频道，首次200档，后续增量
 swap/depth5 //深度数据频道，每次返回前5档
 swap/mark_price// 标记价格频道
*/

CLink::CLink()
{
	m_pCallback = NULL;
	m_nMessages = 0;
}

CLink::~CLink()
{

}

COneMegaApi* CLink::CreateApi()
{
	return new CLink();
}

const char *CLink::GetVersion()
{
	return "1.0";
}

extern "C" COneMegaApi* CreateOneMegaApi()
{
	return new CLink();
}

void CLink::Connect()
{
	m_wsClient.RegisterSpi(this);
	//int ret = m_wsClient.Connect(m_strWSBaseUrl.c_str());
	m_wsClient.SetUrl(m_strWSBaseUrl.c_str());
	m_wsClient.Start();
}

void CLink::SetAddress(const char *pAddress)
{
	m_strWSBaseUrl = pAddress;
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
	//为了解决TopicID不够的问题	
	string sTopicID = pTopicAction->TopicID;
	if (sTopicID == "fr")
		sTopicID = "funding-rate";
	else if(sTopicID=="it")
		sTopicID = "index-tickers";

	//{"op": "subscribe","args":[{"channel":"trades","instId":"BTC-USD-SWAP"}]}
	char buff[512] = {};
	snprintf(buff, sizeof(buff), "{\"op\": \"%s\",\"args\":[{\"channel\":\"%s\",\"instId\":\"%s\"}]}",
		pTopicAction->Action == Mega_SubAction_Sub ? "subscribe" : "unsubscribe", sTopicID.c_str(), pTopicAction->Index);
	PRINT_TO_STD("LinkNo[%d]:%s", m_nLinkNo, buff);
	m_sTopics.insert(buff);
	return m_wsClient.Send(buff);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLink::OnWSConnected()
{
	m_pCallback->OnConnected();
	
	set<string>::iterator iter = m_sTopics.begin();
	while (iter != m_sTopics.end())
	{
		PRINT_TO_STD("LinkNo[%d]:ReConnect:%s", m_nLinkNo, iter->c_str());
		m_wsClient.Send(*iter);
		iter++;
	}
}

void CLink::OnWSDisConnected()
{
	PRINT_TO_STD("LinkNo[%d]:OnWSDisConnected",m_nLinkNo);
	m_pCallback->OnDisconnected(0);
}

void CLink::OnData(const char* data, size_t length)
{
	//unsigned char realData[1024] = {};
	//uLong realDatalen = sizeof(realData);
	//gzDecompress(data, length, realData, &realDatalen);
	//string strData((char*)realData, realDatalen);

	string strData((char*)data, length);
	//printf("%s \n", strData.c_str());
	//PRINT_TO_STD("LinkNo[%d]:%s", m_nLinkNo, strData.c_str());
	m_nMessages++;
	if (fmod(m_nMessages, 100000) == 0)
		PRINT_TO_STD("LinkNo[%d]:m_nMessages[%0.f]", m_nLinkNo, m_nMessages);
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(strData, root))
		return;
	try
	{
		if (!root["event"].isNull())
		{
			string sEvent = root["event"].asString();
			if (sEvent == "error")
				printf("Error: %s \n", root["msg"].asCString());
			//else
				//printf("%s: %s \n", sEvent.c_str(), root["arg"].asCString());
		}
		else
		{
			string szChannel = root["arg"]["channel"].asCString();
			if (szChannel == "trades")
			{
				Json::Value& data = root["data"];
				for (int i = 0; i < data.size(); i++)
					OnTrade(data[i]);
			}
			else if (szChannel == "books" || szChannel == "books5")
			{
				Json::Value& data = root["data"];
				string sInstrumentID = root["arg"]["instId"].asString();
				for (int i = 0; i < data.size(); i++)
					OnOrder(sInstrumentID.c_str(), data[i]);
			}
			else if (szChannel == "ticks")
			{
				Json::Value& data = root["data"];
				for (int i = 0; i < data.size(); i++)
				{
					//OnIndex(szTable, payload[i]);
				}
			}
			else if (szChannel == "funding-rate")
			{
				Json::Value& data = root["data"];
				for (int i = 0; i < data.size(); i++)
				{
					OnFundingRate(data[i]);
				}
			}
			else if (szChannel == "index-tickers")
			{
				Json::Value& data = root["data"];
				for (int i = 0; i < data.size(); i++)
				{
					OnIndex(data[i]);
				}
			}
			else if (szChannel == "mark-price")
			{
				Json::Value& data = root["data"];
				for (int i = 0; i < data.size(); i++)
				{
					OnMarkPrice(data[i]);
				}
			}
		}
	}
	catch (...)
	{
		printf("execption\n");
	}
}

void CLink::OnTrade(Json::Value &Value)
{
	//{
	//	"arg":{
	//		"channel":"trades",
	//		"instId" : "BTC-USD-SWAP"
	//	},
	//	"data":[{
	//		"instId":"BTC-USD-SWAP",
	//		"tradeId" : "109807921",
	//		"px" : "54330",
	//		"sz" : "70",
	//		"side" : "buy",
	//		"ts" : "1619765480854"
	//	}]
	//}
	MegaMarketTradeStruct field;
	memset(&field, 0, sizeof(MegaMarketTradeStruct));
	strncpy(field.InstrumentID, Value["instId"].asCString(), sizeof(field.InstrumentID));
	if (Value["side"].asString() == "buy")
		field.Direction = Mega_Direction_Buy;
	else
		field.Direction = Mega_Direction_Sell;
	field.Price = atof(Value["px"].asCString());
	//strncpy(field.TradeID, Value["tradeId"].asCString(), sizeof(field.TradeID));
	field.Volume = atof(Value["sz"].asCString());
	field.InsertTime = atof(Value["ts"].asCString()) / 1000;
	m_pCallback->PushMarketTrade(&field, 0, "", 0, 0, ONE_CHAIN_SINGLE);
}

void CLink::OnFundingRate(Json::Value &Value)
{
	//{
	//	"arg":{
	//		"channel":"funding-rate",
	//		"instId" : "ICP-USDT-SWAP"
	//	},
	//	"data" : [{
	//			"fundingRate":"0.00000915",
	//			"fundingTime" : "1642147200000", 
	//			"instId" : "ICP-USDT-SWAP",
	//			"instType" : "SWAP",
	//			"nextFundingRate" : "0.00037"
	//	}]
	//}

	MegaMarketOverViewStruct field;
	memset(&field, 0, sizeof(MegaMarketOverViewStruct));
	field.UnderlyingPrice = DOUBLE_NaN;
	field.MarkedPrice = DOUBLE_NaN;
	strncpy(field.InstrumentID, Value["instId"].asCString(), sizeof(field.InstrumentID));
	field.PositionFeeRate = atof(Value["nextFundingRate"].asCString());
	field.PrePositionFeeRate = atof(Value["fundingRate"].asCString());
	m_pCallback->PushMarketOverView(&field, 0, "", 0, 0, ONE_CHAIN_SINGLE);
}

void CLink::OnIndex(Json::Value &Value)
{
	//{
	//	"arg":{
	//		"channel":"index-tickers", 
	//		"instId" : "ICP-USDT"
	//	},
	//	"data" : [{
	//		"instId":"ICP-USDT",
	//			"idxPx" : "31.93",
	//			"open24h" : "34.03",
	//			"high24h" : "34.4", 
	//			"low24h" : "31.06",
	//			"sodUtc0" : "31.39", 
	//			"sodUtc8" : "32.72",
	//			"ts" : "1642126140071"
	//	}]
	//}

	MegaMarketOverViewStruct field;
	memset(&field, 0, sizeof(MegaMarketOverViewStruct));
	field.PositionFeeRate = DOUBLE_NaN;
	field.MarkedPrice = DOUBLE_NaN;
	strncpy(field.InstrumentID, Value["instId"].asCString(), sizeof(field.InstrumentID));
	field.UnderlyingPrice = atof(Value["idxPx"].asCString());;
	m_pCallback->PushMarketOverView(&field, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	//PRINT_TO_STD("LinkNo[%d]:OnIndex:%f", m_nLinkNo, field.UnderlyingPrice);
}

void CLink::OnMarkPrice(Json::Value &Value)
{
	//{
	//	"arg":{
	//		"channel":"mark-price",
	//			"instId" : "ETH-USD-240329-1800-C"
	//	},
	//		"data" : [
	//		{
	//			"instId":"ETH-USD-240329-1800-C",
	//				"instType" : "OPTION",
	//				"markPx" : "0.13905415600075935",
	//				"ts" : "1698631827771"
	//		}
	//		]
	//}

	MegaMarketOverViewStruct field;
	memset(&field, 0, sizeof(MegaMarketOverViewStruct));
	field.UnderlyingPrice = DOUBLE_NaN;
	field.PositionFeeRate = DOUBLE_NaN;
	strncpy(field.InstrumentID, Value["instId"].asCString(), sizeof(field.InstrumentID));
	//double MarkedPrice = atof(Value["markPx"].asCString());
	field.MarkedPrice = atof(Value["markPx"].asCString());
	m_pCallback->PushMarketOverView(&field, 0, "", 0, 0, ONE_CHAIN_SINGLE);
}

void CLink::OnOrder(const char* sInstrumentID, Json::Value &Value)
{
	//{
	//	"arg":{
	//		"channel":"books", 
	//		"instId" : "BTC-USD-SWAP"
	//	},
	//	"action" : "update",
	//	"data" : [{
	//		"asks":[
	//			["54462.8", "5229", "0", "2"], 
	//			["54462.9", "50", "0", "1"],
	//			["54502.2", "0", "0", "0"],
	//			["56498", "22", "0", "1"]
	//		],
	//		"bids":[
	//			["54442.7", "336", "0", "6"], 
	//			["54439.1", "0", "0", "0"],
	//			["54436.8", "2", "0", "1"],
	//			["54417.8", "1613", "0", "2"], 
	//			["54417.7", "1229", "0", "1"],
	//			["54415.4", "2498", "0", "2"],
	//			["54414.2", "0", "0", "0"], 
	//			["52050", "184", "0", "3"]
	//		],
	//		"ts" : "1619765597503", 
	//		"checksum" : 528129311
	//	}]
	//}

	MegaMarketOrderStruct field;
	memset(&field, 0, sizeof(MegaMarketOrderStruct));
	strncpy(field.InstrumentID, sInstrumentID, sizeof(field.InstrumentID));

	field.Direction = Mega_Direction_Sell;
	Json::Value& asks = Value["asks"];
	for (int i = 0; i < asks.size(); i++)
	{
		int k = 0;
		field.Price = atof(asks[i][k].asCString());
		field.Volume = atof(asks[i][1].asCString());
		field.Orders = atof(asks[i][3].asCString());
		m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	}

	field.Direction = Mega_Direction_Buy;
	Json::Value& bids = Value["bids"];
	for (int i = 0; i < bids.size(); i++)
	{
		int k = 0;
		field.Price = atof(bids[i][k].asCString());
		field.Volume = atof(bids[i][1].asCString());
		field.Orders = atof(bids[i][3].asCString());
		m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	}
}