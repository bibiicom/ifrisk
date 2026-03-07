#include "Link.h"
#include "public.h"
#include "BaseFunction.h"
#include "printinfo.h"
#include "utils.h"
#include "ApiKeyUtil.h"
#include <inttypes.h>
#include <openssl/hmac.h>


//#include "zlibuse.h"

// subscribe channel without login
/*
 
*/

CUserDataSpi::CUserDataSpi(CLink* pLink)
	: m_pLink(pLink)
{
}

void CUserDataSpi::OnWSConnected()
{
	m_pLink->m_bUserDataStreamReady = true;
	PRINT_TO_STD("LinkNo[%d]:OnWSConnected", m_pLink->m_nLinkNo);
}

void CUserDataSpi::OnWSDisConnected()
{
	m_pLink->m_bUserDataStreamReady = false;
	PRINT_TO_STD("LinkNo[%d]:OnWSDisConnected", m_pLink->m_nLinkNo);
}

void CUserDataSpi::OnData(const char* data, size_t length)
{
	string strData(data, length);
	//printf("%s \n", strData.c_str());
	Json::Reader reader(true);
	Json::Value root;
	if (!reader.parse(strData, root))
		return;

	try
	{
		const char* dataType = root["e"].asCString();
		if (0 == strcmp(dataType, "executionReport"))
		{
			PRINT_TO_STD("executionReport:%s", strData.c_str());
			m_pLink->OnOrderUpdate(root);
		}

	}
	catch (...)
	{
		PRINT_TO_STD("收到非法数据:%s", data);
	}
}

void CUserDataSpi::OnPoll()
{
	if (m_pLink->m_expiredOrders.empty())
		return;

	time_t tNow = time(NULL);
	for (CLink::CExpiredOrderMap::iterator itUnavailable = m_pLink->m_expiredOrders.begin()
		; itUnavailable != m_pLink->m_expiredOrders.end(); )
	{
		CLink::CExpiredOrderMap::iterator it = itUnavailable++;
		if (tNow - it->second.insertTime > 2)
		{
			m_pLink->m_pCallback->PushOrder(&(it->second.order), 0, "", 0, 0, ONE_CHAIN_SINGLE);
			m_pLink->m_expiredOrders.erase(it);
		}
	}
}

CLink::CLink()
	: m_userDataSpi(this)
	, m_pCallback(NULL)
	, m_bUserDataStreamReady(false)
	, m_bHasTrade(false)
	, m_lastListenKeyTime(0)
	, m_nPeriodSubCnt(0)
	, m_lastSubTime(0)
	, m_nLinkNo(0)
{
	memset(&m_userLogin, 0, sizeof(m_userLogin));
	m_nId = 1;
}

CLink::~CLink()
{
	if (m_pCallback != NULL)
	{
		delete m_pCallback;
		m_pCallback = NULL;
	}
}

COneMegaApi* CLink::CreateApi()
{
	return new CLink();
}

const char *COneMegaApi::GetVersion()
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
	snprintf(marketDataWSUrl, sizeof(marketDataWSUrl), "%s/stream", m_strWSBaseUrl.c_str());
	PRINT_TO_STD("LinkNo[%d]:MarketAddress[%s]", m_nLinkNo, marketDataWSUrl);
	/*int ret = m_wsClient.Connect(marketDataWSUrl);
	if (0 == ret)*/
	m_wsClient.SetUrl(marketDataWSUrl);
	m_wsClient.Start();

	if (m_bHasTrade)
	{
		while (1)
		{
			if (GenListenKey(m_listenKey))
				break;

			SLEEP(1000);
		}

		char userDataWSUrl[1024] = {};
		snprintf(userDataWSUrl, sizeof(userDataWSUrl), "%s/ws/%s", m_strWSBaseUrl.c_str(), m_listenKey.c_str());
		m_wsUserDataClient.RegisterSpi(&m_userDataSpi);
		PRINT_TO_STD("LinkNo[%d]:TradeAddress[%s]", m_nLinkNo, userDataWSUrl);
		//if (0 == m_wsUserDataClient.Connect(userDataWSUrl))
		m_wsUserDataClient.SetUrl(userDataWSUrl);
		m_wsUserDataClient.Start();

		if (m_userLogin.UserID[0] != 0)
		{
			OnUserLogon(&m_userLogin);
		}
	}
}

void CLink::SetAddress(const char *pAddress)
{
	m_bHasTrade = false;
	vector< string > urls = Txtsplit(pAddress, ";");
	for (size_t i = 0; i < urls.size(); i++)
	{
		string& url = urls[i];
		if (0 == strncmp(url.c_str(), "ws", 2))
			m_strWSBaseUrl = url;
		else if (0 == strncmp(url.c_str(), "http", 4))
		{
			m_httpClient.SetBaseUrl(url.c_str());
			m_bHasTrade = true;
		}
		else if (0 == strncmp(url.c_str(), "APIKey", 6))
		{
			m_extraHeader = "X-MBX-APIKEY: ";
			m_extraHeader += url.c_str() + 7;
		}
		else if (0 == strncmp(url.c_str(), "APISecret", 9))
			m_secretKey = url.c_str() + 10;
	}
	
}

void CLink::SetCallback(COneMegaCallback *pCallback)
{
	m_pCallback = new COneMegaCallbackWrap(pCallback);
}

int CLink::SendInitLink(MegaLinkStruct *pLink, int nRequestNo)
{
	m_nLinkNo = pLink->LinkNo;
	return 0;
}

int CLink::SendUserLogin(MegaUserLoginStruct *pUserLogin, int nRequestNo)
{
	m_userLogin = *pUserLogin;
	if (!m_bHasTrade)
	{
		OnUserLogon(pUserLogin);
	}

	return 0;
}

int CLink::SendTopicAction(MegaTopicActionStruct *pTopicAction, int nRequestNo)
{
	//为了解决TopicID不够的问题

	//{"op": "subscribe","args":[{"channel":"trades","instId":"BTC-USD-SWAP"}]}

	//{"method":"SUBSCRIBE", "params" : ["btcusdt@aggSnap", "!markPriceMini@arr", "!contractInfo", "!miniTicker@arr", "btcusdt@markPrice", "btcusdt@depth@500ms", "btcusdt_perpetual@continuousKline_1d"], "id" : 1}

	// Websocket服务器每秒最多接受10个订阅消息
	// 单个连接最多可以订阅 200 个Streams
	time_t tNow = time(NULL);
	if (0 == m_lastSubTime)
		m_lastSubTime = tNow;
	else
	{
		int tGap = tNow - m_lastSubTime;
		if (tGap >= 2)
		{
			m_lastSubTime = tNow;
			m_nPeriodSubCnt = 0;
		}
		else if (m_nPeriodSubCnt >= 5)
		{
			SLEEP(1000);
			m_nPeriodSubCnt = 0;
			m_lastSubTime = time(NULL);
		}
	}

	m_nPeriodSubCnt++;

	char buff[512] = {};
	snprintf(buff, sizeof(buff) - 1, "{\"method\":\"%s\",\"params\":[\"%s@%s\"],\"id\":%d}",
		pTopicAction->Action == Mega_SubAction_Sub ? "SUBSCRIBE" : "UNSUBSCRIBE", pTopicAction->Index, pTopicAction->TopicID, m_nId++);
	//不能回复，应该是m_wsClient回调中回复，这里回复会有多线程调用问题
	//m_pCallback->RecvTopicAction(pTopicAction, 0, "Sending...", nRequestNo, ONE_CHAIN_SINGLE);
	PRINT_TO_STD("LinkNo[%d]:%s", m_nLinkNo, buff);
	return m_wsClient.Send(buff);
}

int CLink::SendOrderInsert(MegaOrderInsertStruct* pOrderInsert, int nRequestNo)
{
	const char* type = "LIMIT";
	if (Mega_OrderPriceType_AnyPrice == pOrderInsert->OrderPriceType)
		type = "MARKET";

	string errMsg;
	int errorID = SendOrder(pOrderInsert, type, errMsg, nRequestNo);
	if (errorID != 0)
		m_pCallback->RecvOrderInsert(pOrderInsert, NULL, errorID, errMsg.c_str(), nRequestNo, ONE_CHAIN_SINGLE);

	return errorID;
}

int CLink::SendTriggerOrderInsert(MegaTriggerOrderInsertStruct* pTriggerOrderInsert, int nRequestNo)
{
	const char* type = pTriggerOrderInsert->TriggerDetail;
	string errMsg;
	int errorID = SendOrder(pTriggerOrderInsert, type, errMsg, nRequestNo);
	if (errorID != 0)
		m_pCallback->RecvTriggerOrderInsert(pTriggerOrderInsert, NULL, errorID, errMsg.c_str(), nRequestNo, ONE_CHAIN_SINGLE);

	return errorID;
}

int CLink::SendOrderAction(MegaOrderActionStruct* pOrderAction, int nRequestNo)
{
	string errMsg;
	int errorID = OrderAction(pOrderAction, errMsg, nRequestNo);
	if (errorID != 0)
		m_pCallback->RecvOrderAction(pOrderAction, NULL, errorID, errMsg.c_str(), nRequestNo, ONE_CHAIN_SINGLE);

	return errorID;
}

int CLink::SendPositionAction(MegaPositionActionStruct* pPositionAction, int nRequestNo)
{
	char buff[1024] = {};
	strupr(pPositionAction->InstrumentID);
	AppendQueryData(buff, "symbol", pPositionAction->InstrumentID, true);
	AppendQueryData(buff, "leverage", (int)pPositionAction->NetLeverage);

	timeb t;
	ftime(&t);
	INT_8 timestamp = t.time * 1000 + t.millitm;
	AppendQueryData(buff, "timestamp", timestamp);

	string signature = HmacShaEncode2(m_secretKey.c_str(), buff);

	AppendQueryData(buff, "signature", signature.c_str());

	const int resp_len = 2 * 1024 * 1024;
	char* resp = new char[resp_len];
	memset(resp, 0, resp_len);

	int ret = 0;
	do
	{
		if (!m_httpClient.Post("/fapi/v1/leverage", buff, m_extraHeader.c_str(), resp, resp_len - 1))
			break;

	} while (0);

	PRINT_TO_STD("SendPositionAction:param:%s,resp:%s", buff, resp);

	delete[] resp;

	return ret;
}

int CLink::SendTriggerOrderAction(MegaTriggerOrderActionStruct* pTriggerOrderAction, int nRequestNo)
{
	string errMsg;
	int errorID = OrderAction(pTriggerOrderAction, errMsg, nRequestNo);

	if (errorID != 0)
		m_pCallback->RecvTriggerOrderAction(pTriggerOrderAction, NULL, errorID, errMsg.c_str(), nRequestNo, ONE_CHAIN_SINGLE);

	return errorID;
}

int CLink::SendQryInstrument(MegaQryInstrumentStruct* pQryInstrument, int nRequestNo)
{
	const int resp_len = 10 * 1024 * 1024;
	char* resp = new char[resp_len];
	memset(resp, 0, resp_len);

	int ret = -1;
	do
	{
		if (!m_httpClient.Get("/api/v3/exchangeInfo", NULL, m_extraHeader.c_str(), resp, resp_len - 1))
			break;

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(resp, root))
			break;

		try
		{
			Json::Value& symbols = root["symbols"];

			for (int i = 0; i < symbols.size(); i++)
			{
				OnQryInst(symbols[i], (i == symbols.size() - 1));
			}

			ret = 0;
		}
		catch (...)
		{
			PRINT_TO_STD("parse inst execption:%s", resp);
		}

	} while (0);

	if (0 != ret)
	{
		PRINT_TO_STD("SendQryInstrument fail:%d,%s", ret, resp);
	}

	delete[] resp;
	
	return ret;
}

int CLink::SendQryKLine(MegaQryKLineStruct* pQryKLine, int nRequestNo)
{
	char buff[1024] = {};
	strupr(pQryKLine->InstrumentID);
	AppendQueryData(buff, "symbol", pQryKLine->InstrumentID, true);
	AppendQueryData(buff, "interval", pQryKLine->Bar);
	AppendQueryData(buff, "limit", pQryKLine->Limit);

	const int resp_len = 2 * 1024 * 1024;
	char* resp = new char[resp_len];
	memset(resp, 0, resp_len);

	int ret = -1;
	do
	{
		if (!m_httpClient.Get("/fapi/v1/klines", buff, m_extraHeader.c_str(), resp, resp_len - 1))
			break;

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(resp, root))
			break;

		try
		{
			for (int i = 0; i < root.size(); i++)
			{
				OnQryKline(root[i], (i == root.size() - 1));
			}

			ret = 0;
		}
		catch (...)
		{
			PRINT_TO_STD("parse klines execption,%s", resp);
		}

	} while (0);

	if (ret != 0)
	{
		PRINT_TO_STD("SendQryKLine faild:%d,%s", ret, resp);
	}

	delete[] resp;

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLink::OnWSConnected()
{
	m_pCallback->OnConnected();
}

void CLink::OnWSDisConnected()
{
	PRINT_TO_STD("market data disconnected");
	m_pCallback->OnDisconnected(0);
}

void CLink::OnData(const char* data, size_t length)
{
	//unsigned char realData[1024] = {};
	//uLong realDatalen = sizeof(realData);
	//gzDecompress(data, length, realData, &realDatalen);
	//string strData((char*)realData, realDatalen);

	string strData(data, length);
	//printf("%s \n", strData.c_str());
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(strData, root))
		return;
	try
	{
		if (root.isMember("id") && root.isMember("result"))
		{
			//m_pCallback->RecvTopicAction(pTopicAction, 0, "Sending...", nRequestNo, ONE_CHAIN_SINGLE);
			return;
		}

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
			else if (szChannel == "kline")
			{
				OnKline(root["data"]["k"]);
			}
		}
	}
	catch (...)
	{
		PRINT_TO_STD("格式错误,%s", strData.c_str());
	}
}

void CLink::OnPoll()
{
	if (0 == m_lastListenKeyTime || m_listenKey.empty())
		return;

	time_t tNow = time(NULL);
	if (tNow - m_lastListenKeyTime < 30 * 60)
		return;

	char buff[1024] = {};
	AppendQueryData(buff, "listenKey", m_listenKey.c_str(), true);

	char resp[1024] = {};
	bool res = m_httpClient.Put("/api/v3/userDataStream", buff, m_extraHeader.c_str(), resp, sizeof(resp) - 1);
	if (res)
	{
		m_lastListenKeyTime = tNow;
	}

	PRINT_TO_STD("Put listenKey:%s,%s", res ? "成功" : "失败", resp);
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
	field.Volume = atof(Value["q"].asCString());
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
		field.Volume = atof(asks[i][1].asCString());
		field.Orders = 1;
		m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	}

	field.Direction = Mega_Direction_Buy;
	Json::Value& bids = Value["b"];
	for (int i = 0; i < bids.size(); i++)
	{
		int k = 0;
		field.Price = atof(bids[i][k].asCString());
		field.Volume = atof(bids[i][1].asCString());
		field.Orders = 1;
		m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	}
}

template<typename T>
void InitKline(T& data, Json::Value& json)
{
	strncpy(data.InstrumentID, json["s"].asCString(), sizeof(data.InstrumentID));
	strlwr(data.InstrumentID);
	strncpy(data.Bar, json["i"].asCString(), sizeof(data.Bar));
	data.InsertTime = json["t"].asDouble();
	data.HighestPrice = json["h"].asDouble();
	data.LowestPrice = json["l"].asDouble();
	data.OpenPrice = json["o"].asDouble();
	data.ClosePrice = json["c"].asDouble();
	data.Volume = atof(json["v"].asCString());
	data.Turnover = atof(json["q"].asCString());
}

void CLink::OnKline(Json::Value& Value)
{
	MegaLastKLineStruct lastKline;
	memset(&lastKline, 0, sizeof(lastKline));
	InitKline(lastKline, Value);

	bool bEnd = Value["x"].asBool();
	if (bEnd)
		lastKline.UpdateTime = -1;
	else
		lastKline.UpdateTime = Value["T"].asDouble();

	m_pCallback->PushKLine(NULL, &lastKline, 0, "", 0, 0, ONE_CHAIN_SINGLE);
}

void CLink::OnQryKline(Json::Value& Value, bool bIsLast)
{
	MegaKLineBaseStruct kline;
	memset(&kline, 0, sizeof(kline));

	Json::Value::UInt idx = 0;
	kline.InsertTime = Value.get(idx++, Json::Value::null).asDouble();
	kline.OpenPrice = Value.get(idx++, Json::Value::null).asDouble();
	kline.HighestPrice = Value.get(idx++, Json::Value::null).asDouble();
	kline.LowestPrice = Value.get(idx++, Json::Value::null).asDouble();
	kline.ClosePrice = Value.get(idx++, Json::Value::null).asDouble();
	kline.Volume = Value.get(idx++, Json::Value::null).asDouble();
	
	m_pCallback->RecvQryKLine(&kline, 0, "", 0, bIsLast ? ONE_CHAIN_LAST : ONE_CHAIN_CONTINUE);
}

void CLink::OnOrderUpdate(Json::Value& Value)
{
	MegaOrderStruct order;
	memset(&order, 0, sizeof(order));

	strncpy(order.InstrumentID, Value["s"].asCString(), sizeof(order.InstrumentID));
	strlwr(order.InstrumentID);
	strncpy(order.LocalID, Value["c"].asCString(), sizeof(order.LocalID));
	const char* szDir = Value["S"].asCString();
	order.Direction = (szDir != NULL && 0 == strcmp("SELL", szDir)) ? Mega_Direction_Sell : Mega_Direction_Buy;
	order.OrderPriceType = Mega_OrderPriceType_LimitPrice;
	TransOrderPriceType(Value["o"].asCString(), order.OrderPriceType);

	order.Volume = Value["q"].asDouble();
	order.Price = Value["p"].asDouble();

	Json::Value& orderSysIDValue = Value["i"];
	if (!orderSysIDValue.isString())
		snprintf(order.OrderID, sizeof(order.OrderID) - 1, "%.0lf", orderSysIDValue.asDouble());
	else
		strncpy(order.OrderID, orderSysIDValue.asCString(), sizeof(order.OrderID));

	order.OrderStatus = Mega_OrderStatus_None;
	bool bExpired = false;
	TransOrderStatus(Value["X"].asCString(), order.OrderStatus, bExpired);
	
	order.VolumeTraded = Value["z"].asDouble();

	if (bExpired)
	{
		ExpiredOrderInfo expiredOrderInfo = { order, time(NULL) };
		m_expiredOrders[order.OrderID] = expiredOrderInfo;
	}
	else
	{
		m_pCallback->PushOrder(&order, 0, "", 0, 0, ONE_CHAIN_SINGLE);

		CExpiredOrderMap::iterator it = m_expiredOrders.find(order.OrderID);
		if (it != m_expiredOrders.end())
			m_expiredOrders.erase(it);
	}

	const char* currType = Value["x"].asCString();
	if (0 == strcmp(currType, "TRADE"))
	{
		MegaTradeStruct trade;
		memset(&trade, 0, sizeof(trade));
		InitTrade(order, trade);

		trade.Volume = Value["l"].asDouble();
		trade.Price = Value["L"].asDouble();
		snprintf(trade.TradeID, sizeof(trade.TradeID) - 1, "%.0lf", Value["t"].asDouble());

		m_pCallback->PushTrade(&trade, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	}
}

void CLink::OnQryInst(Json::Value& Value, bool bIsLast)
{
	MegaInstrumentStruct inst;
	memset(&inst, 0, sizeof(inst));

	strncpy(inst.InstrumentID, Value["symbol"].asCString(), sizeof(inst.InstrumentID));
	strlwr(inst.InstrumentID);

	snprintf(inst.Remark, sizeof(inst.Remark) - 1, "p:%d,v:%d", Value["quotePrecision"].asInt(), Value["baseAssetPrecision"].asInt());

	Json::Value filters = Value["filters"];
	for (int i = 0; i < filters.size(); i++)
	{
		const Json::Value& filter = filters[i];
		const char* filterType = filter["filterType"].asCString();
		if (0 == strcmp(filterType, "PRICE_FILTER"))
		{
			const Json::Value& tickSize = filter["tickSize"];
			if (tickSize.isString())
				inst.PriceTick = atof(tickSize.asCString());
		}
		else if (0 == strcmp(filterType, "LOT_SIZE"))
		{
			const Json::Value& minQty = filter["minQty"];
			if (minQty.isString())
				inst.MinOrderVolume = atof(minQty.asCString());
		}
	}

	m_pCallback->RecvQryInstrument(&inst, 0, "", 0, bIsLast ? ONE_CHAIN_LAST : ONE_CHAIN_CONTINUE);
}

void CLink::GetJsonValue(int value, char* szValue, int len)
{
	snprintf(szValue, len - 1, "\"%d\"", value);
}

void CLink::GetJsonValue(double value, char* szValue, int len)
{
	snprintf(szValue, len - 1, "\"%.5lf\"", value);
}

void CLink::GetJsonValue(const char* value, char* szValue, int len)
{
	snprintf(szValue, len - 1, "\"%s\"", value);
}

void CLink::GetQueryValue(int value, char* szValue, int len)
{
	snprintf(szValue, len - 1, "%d", value);
}

void CLink::GetQueryValue(INT_8 value, char* szValue, int len)
{
	snprintf(szValue, len - 1, "%" PRId64, value);
}

void CLink::GetQueryValue(double value, char* szValue, int len)
{
	snprintf(szValue, len - 1, "%.5lf", value);
}

void CLink::GetQueryValue(const char* value, char* szValue, int len)
{
	snprintf(szValue, len - 1, "%s", value);
}

void CLink::TransOrderPriceType(const char* source, MegaOrderPriceTypeType& target)
{
	if (NULL == source)
		return;

	if (0 == strcmp("MARKET", source))
		target = Mega_OrderPriceType_AnyPrice;
	else if (0 == strcmp("LIMIT", source))
		target = Mega_OrderPriceType_LimitPrice;
}

void CLink::TransOrderStatus(const char* source, MegaOrderStatusType& target, bool& bExpired)
{
	if (NULL == source)
		return;

	bExpired = false;
	if (0 == strcmp("NEW", source))
		target = Mega_OrderStatus_NoTraded;
	else if (0 == strcmp("PARTIALLY_FILLED", source))
		target = Mega_OrderStatus_PartTraded;
	else if (0 == strcmp("FILLED", source))
		target = Mega_OrderStatus_AllTraded;
	else if (0 == strcmp("CANCELED", source))
		target = Mega_OrderStatus_AllCanceled;
	else if (0 == strcmp("EXPIRED", source))
	{
		target = Mega_OrderStatus_AllCanceled;
		bExpired = true;
	}
}

void CLink::TransTriggertatus(const char* source, MegaTriggerStatusType& target, bool& bExpired)
{
	if (NULL == source)
		return;

	if (0 == strcmp("NEW", source))
		target = Mega_TriggerStatus_Active;
	else if (0 == strcmp("CANCELED", source))
		target = Mega_TriggerStatus_Canceled;
	else if (0 == strcmp("EXPIRED", source))
	{
		target = Mega_OrderStatus_AllCanceled;
		bExpired = true;
	}
}

void CLink::InitTrade(const MegaOrderStruct& order, MegaTradeStruct& trade)
{
	strncpy(trade.InstrumentID, order.InstrumentID, sizeof(trade.InstrumentID));
	strncpy(trade.OrderID, order.OrderID, sizeof(trade.OrderID));
	trade.Direction = order.Direction;
	

}

bool CLink::GenListenKey(string& listenKey)
{
	char resp[1024] = {};
	bool res = m_httpClient.Post("/api/v3/userDataStream", NULL, m_extraHeader.c_str(), resp, sizeof(resp) - 1);
	if (!res)
	{
		PRINT_TO_STD("GenListenKey fail:%s", resp);
		return false;
	}

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(resp, root))
	{
		PRINT_TO_STD("GenListenKey fail:%s", resp);
		return false;
	}

	try
	{
		listenKey = root["listenKey"].asCString();
	}
	catch (...)
	{
		PRINT_TO_STD("GenListenKey execption,resp:%s", resp);
		return false;
	}

	PRINT_TO_STD("gen listenkey:%s", listenKey.c_str());
	m_lastListenKeyTime = time(NULL);
	return true;
}

int CLink::ParseErrorMsg(Json::Value& data, string& errMsg)
{
	const Json::Value& code = data["code"];
	const Json::Value& msg = data["msg"];

	if (code.isNull() || msg.isNull())
		return 0;

	errMsg = msg.asCString();
	return code.asInt();
}

void CLink::OnUserLogon(MegaUserLoginStruct* pUserLogin)
{
	MegaUserSessionStruct field;
	memset(&field, 0, sizeof(MegaUserSessionStruct));
	strncpy(field.UserID, pUserLogin->UserID, sizeof(field.UserID));
	strncpy(field.Token, pUserLogin->Token, sizeof(field.Token));
	m_pCallback->RecvUserLogin(NULL, &field, 0, "Success", 0, ONE_CHAIN_SINGLE);
}
