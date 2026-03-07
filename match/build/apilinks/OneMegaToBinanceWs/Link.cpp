#include "Link.h"
#include "public.h"
#include "BaseFunction.h"
#include "printinfo.h"
#include "utils.h"
#include <inttypes.h>
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
	m_VolumeRatio = 1.0;
	m_nId = 1;
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

	char marketDataWSUrl[1024] = {};
	snprintf(marketDataWSUrl, sizeof(marketDataWSUrl), "%s", m_strWSBaseUrl.c_str());
	PRINT_TO_STD("LinkNo[%d]:MarketAddress[%s]", m_nLinkNo, marketDataWSUrl);
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
	vector<string> configs = Txtsplit(pLink->Config, ";");
	for (int i = 0; i < configs.size(); i++)
	{
		vector<string> items = Txtsplit(configs[i], ":");
		if (items.size() == 2 && items[0] == "VolumeRatio")
			m_VolumeRatio = atof(items[1].c_str());
	}
	PRINT_TO_STD("LinkNo[%d]:VolumeRatio:%.5f", m_nLinkNo, m_VolumeRatio);
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

	//{"op": "subscribe","args":[{"channel":"trades","instId":"BTC-USD-SWAP"}]}

	//{"method":"SUBSCRIBE", "params" : ["btcusdt@aggSnap", "!markPriceMini@arr", "!contractInfo", "!miniTicker@arr", "btcusdt@markPrice", "btcusdt@depth@500ms", "btcusdt_perpetual@continuousKline_1d"], "id" : 1}

	// Websocket服务器每秒最多接受10个订阅消息
	// 单个连接最多可以订阅 200 个Streams

	//Maker|mkrusdt_5|aggTrade|depth5@500ms

	char buff[512] = {};
	snprintf(buff, sizeof(buff) - 1, "{\"method\":\"%s\",\"params\":[\"%s@aggTrade\",\"%s@depth\"],\"id\":%d}",
		pTopicAction->Action == Mega_SubAction_Sub ? "SUBSCRIBE" : "UNSUBSCRIBE", pTopicAction->Index, pTopicAction->Index,m_nId++);
	PRINT_TO_STD("LinkNo[%d]:%s", m_nLinkNo, buff);
	m_wsClient.Send(buff);
	SLEEP_SECONDS(1);
	//m_sTopics.insert(buff);
	snprintf(buff, sizeof(buff) - 1, "{\"method\":\"LIST_SUBSCRIPTIONS\",\"id\":%d}",m_nId++);
	PRINT_TO_STD("LinkNo[%d]:%s", m_nLinkNo, buff);
	return m_wsClient.Send(buff);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLink::OnWSConnected()
{
	PRINT_TO_STD("LinkNo[%d]:OnWSConnected", m_nLinkNo);
	m_pCallback->OnConnected();
	m_nId = 1;
	//set<string>::iterator iter = m_sTopics.begin();
	//while (iter != m_sTopics.end())
	//{
	//	PRINT_TO_STD("LinkNo[%d]:ReConnect:%s", m_nLinkNo, iter->c_str());
	//	m_wsClient.Send(*iter);
	//	SLEEP(500);
	//	iter++;
	//}
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
	//PRINT_TO_STD("LinkNo[%d]:%s", m_nLinkNo, strData.c_str());
	m_nMessages++;
	if (fmod(m_nMessages, 10000) == 0)
		PRINT_TO_STD("LinkNo[%d]:m_nMessages[%0.f]", m_nLinkNo, m_nMessages);
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(strData, root))
		return;
	//try
	//{
		if (root.isMember("id") && root.isMember("result"))
		{
			PRINT_TO_STD("LinkNo[%d]:%s", m_nLinkNo, strData.c_str());
			return;
		}

		if (!root["event"].isNull())
		{
			string sEvent = root["event"].asString();
			if (sEvent == "error")
				printf("Error: %s \n", root["msg"].asCString());
			PRINT_TO_STD("LinkNo[%d]:%s", m_nLinkNo, strData.c_str());
		}
		else
		{
			string szChannel = root["data"]["e"].asCString();
			if (szChannel == "aggTrade")
			{
				OnTrade(root["data"]);
			}
			else if (szChannel == "depthUpdate")
			{
				OnOrder(root["data"]);
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
			else if (szChannel == "markPriceUpdate")
			{
				OnIndex(root["data"]);
			}
			else if (szChannel == "index-tickers")
			{
				Json::Value& data = root["data"];
				for (int i = 0; i < data.size(); i++)
				{
					OnIndex(data[i]);
				}
			}
			//else if (szChannel == "kline")
			//{
			//	OnKline(root["data"]["k"]);
			//}
		}
	//}
	//catch (...)
	//{
	//	PRINT_TO_STD("format error: %s", strData.c_str());
	//}
}

void CLink::OnTrade(Json::Value &Value)
{
	//{\stream\:\btcusdt@aggTrade\, \data\:{\e\:\aggTrade\, \E\:1665406089892, \a\:1485513584, \s\:\BTCUSDT\, \p\:\19372.20\, \q\:\0.002\, \f\:2939660663, \l\:2939660663, \T\:1665406089889, \m\:true}}
	MegaMarketTradeStruct field;
	memset(&field, 0, sizeof(MegaMarketTradeStruct));
	strncpy(field.InstrumentID, Value["s"].asCString(), sizeof(field.InstrumentID));
	strlwr(field.InstrumentID);

	if (Value["m"].asBool() == true)
		field.Direction = Mega_Direction_Buy;
	else
		field.Direction = Mega_Direction_Sell;
	field.Price = atof(Value["p"].asCString());
	//strncpy(field.TradeID, Value["tradeId"].asCString(), sizeof(field.TradeID));
	CVolumeType Volume = atof(Value["q"].asCString());
	double tick = Volume.getTick();
	Volume = Volume.getValue() * m_VolumeRatio;
	Volume.toTick(tick);
	if(Volume==0.0)
		Volume=tick;
	field.Volume = Volume;
	field.InsertTime = Value["T"].asDouble() / 1000;
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
	field.PrePositionFeeRate = atof(Value["nextFundingRate"].asCString());;
	m_pCallback->PushMarketOverView(&field, 0, "", 0, 0, ONE_CHAIN_SINGLE);
}

void CLink::OnIndex(Json::Value &Value)
{
	MegaMarketOverViewStruct field;
	memset(&field, 0, sizeof(MegaMarketOverViewStruct));

	strncpy(field.InstrumentID, Value["s"].asCString(), sizeof(field.InstrumentID));
	strlwr(field.InstrumentID);

	field.PositionFeeRate = atof(Value["r"].asCString());
	field.PrePositionFeeRate = atof(Value["r"].asCString());
	field.UnderlyingPrice = atof(Value["i"].asCString());

	m_pCallback->PushMarketOverView(&field, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	//PRINT_TO_STD("LinkNo[%d]:OnIndex:%f", m_nLinkNo, field.UnderlyingPrice);

	//MegaMarketOverViewStruct field;
	//memset(&field, 0, sizeof(MegaMarketOverViewStruct));
	//field.UnderlyingPrice = DOUBLE_NaN;
	//field.PositionFeeRate = DOUBLE_NaN;
	//strncpy(field.InstrumentID, Value["instId"].asCString(), sizeof(field.InstrumentID));
	//field.UnderlyingPrice = atof(Value["idxPx"].asCString());;
	//m_pCallback->PushMarketOverView(&field, 0, "", 0, ONE_CHAIN_SINGLE);
}

void CLink::OnOrder(Json::Value &Value)
{
	//{"stream":"btcusdt@depth", "data" : {"e":"depthUpdate", "E" : 1665406091302, "T" : 1665406091296, "s" : "BTCUSDT", "U" : 2015638356323, "u" : 2015638365956, "pu" : 2015638356247, 
	//"b" : [["1937.20", "0.000"], ["11623.10", "0.002"]],
	//"a" : [["1937.20", "0.000"], ["11623.10", "0.002"]}}
	MegaMarketOrderStruct field;
	memset(&field, 0, sizeof(MegaMarketOrderStruct));
	strncpy(field.InstrumentID, Value["s"].asCString(), sizeof(field.InstrumentID));
	strlwr(field.InstrumentID);

	field.Direction = Mega_Direction_Sell;
	Json::Value& asks = Value["a"];
	for (int i = 0; i < asks.size(); i++)
	{
		int k = 0;
		field.Price = atof(asks[i][k].asCString());
		CVolumeType Volume = atof(asks[i][1].asCString());
		double tick = Volume.getTick();
		Volume = Volume.getValue() * m_VolumeRatio;
		Volume.toTick(tick);
		field.Volume = Volume.getValue();
		field.Orders = 1;
		m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	}

	field.Direction = Mega_Direction_Buy;
	Json::Value& bids = Value["b"];
	for (int i = 0; i < bids.size(); i++)
	{
		int k = 0;
		field.Price = atof(bids[i][k].asCString());
		CVolumeType Volume = atof(bids[i][1].asCString());
		double tick = Volume.getTick();
		Volume = Volume.getValue() * m_VolumeRatio;
		Volume.toTick(tick);
		field.Volume = Volume;
		field.Orders = 1;
		m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	}
}
