#ifndef __COneFtdToBinanceSpot_H_
#define __COneFtdToBinanceSpot_H_

#include "WSClient.h"
#include "platform.h"
#include "OneMegaApi.h"
#include "json/json.h"
#include "HttpClient.h"
#include "ApiKeyUtil.h"
#include "tool.h"

using namespace OneMegaByte;

#ifndef WINDOWS
typedef long long INT_8;
#else
typedef __int64 INT_8;
#endif

class COneMegaCallbackWrap
{
public:
	COneMegaCallbackWrap(COneMegaCallback* pCb)
		: m_pCallback(pCb)
	{

	}

	void OnConnected()
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->OnConnected();
	}

	void OnDisconnected(int nReason)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->OnDisconnected(nReason);
	}

	void PushOrder(const MegaOrderStruct* pOrder, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->PushOrder(pOrder, nLocalNo, FilterValue, nSequenceNo, nBusinessNo, cChain);
	}

	void PushTrade(const MegaTradeStruct* pTrade, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->PushTrade(pTrade, nLocalNo, FilterValue, nSequenceNo, nBusinessNo, cChain);
	}

	void RecvTopicAction(const MegaTopicActionStruct* pTopicAction, int nErrorNo, const char* pErrorMsg, int nRequestNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->RecvTopicAction(pTopicAction, nErrorNo, pErrorMsg, nRequestNo, cChain);
	}

	void RecvOrderInsert(const MegaOrderInsertStruct* pOrderInsert, const MegaOrderStruct* pOrder, int nErrorNo, const char* pErrorMsg, int nRequestNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->RecvOrderInsert(pOrderInsert, pOrder, nErrorNo, pErrorMsg, nRequestNo, cChain);
	}

	void RecvTriggerOrderInsert(const MegaTriggerOrderInsertStruct* pTriggerOrderInsert, const MegaTriggerOrderStruct* pTriggerOrder, int nErrorNo, const char* pErrorMsg, int nRequestNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->RecvTriggerOrderInsert(pTriggerOrderInsert, pTriggerOrder, nErrorNo, pErrorMsg, nRequestNo, cChain);
	}

	void RecvOrderAction(const MegaOrderActionStruct* pOrderAction, const MegaOrderStruct* pOrder, int nErrorNo, const char* pErrorMsg, int nRequestNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->RecvOrderAction(pOrderAction, pOrder, nErrorNo, pErrorMsg, nRequestNo, cChain);
	}

	void RecvTriggerOrderAction(const MegaTriggerOrderActionStruct* pTriggerOrderAction, const MegaTriggerOrderStruct* pTriggerOrder, int nErrorNo, const char* pErrorMsg, int nRequestNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->RecvTriggerOrderAction(pTriggerOrderAction, pTriggerOrder, nErrorNo, pErrorMsg, nRequestNo, cChain);
	}

	void PushMarketTrade(const MegaMarketTradeStruct* pMarketTrade, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->PushMarketTrade(pMarketTrade, nLocalNo, FilterValue, nSequenceNo, nBusinessNo, cChain);
	}

	void PushMarketOverView(const MegaMarketOverViewStruct* pMarketOverView, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->PushMarketOverView(pMarketOverView, nLocalNo, FilterValue, nSequenceNo, nBusinessNo, cChain);
	}

	void PushMarketOrder(const MegaMarketOrderStruct* pMarketOrder, const MegaTickMarketOrderStruct* pTickMarketOrder, const MegaCurrentTimeStruct* pCurrentTime, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->PushMarketOrder(pMarketOrder, pTickMarketOrder, pCurrentTime, nLocalNo, FilterValue, nSequenceNo, nBusinessNo, cChain);
	}

	void PushKLine(const MegaKLineStruct* pKLine, const MegaLastKLineStruct* pLastKLine, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->PushKLine(pKLine, pLastKLine, nLocalNo, FilterValue, nSequenceNo, nBusinessNo, cChain);
	}

	void RecvQryKLine(const MegaKLineBaseStruct* pKLine, int nErrorNo, const char* pErrorMsg, int nRequestNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->RecvQryKLine(pKLine, nErrorNo, pErrorMsg, nRequestNo, cChain);
	}

	void RecvQryInstrument(const MegaInstrumentStruct* pInstrument, int nErrorNo, const char* pErrorMsg, int nRequestNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->RecvQryInstrument(pInstrument, nErrorNo, pErrorMsg, nRequestNo, cChain);
	}

	void RecvUserLogin(const MegaUserLoginStruct* pUserLogin, const MegaUserSessionStruct* pUserSession, int nErrorNo, const char* pErrorMsg, int nRequestNo, char cChain)
	{
		CAutoLock< ManualLock > autoLock(m_lock);
		m_pCallback->RecvUserLogin(pUserLogin, pUserSession, nErrorNo, pErrorMsg, nRequestNo, cChain);
	}

private:
	COneMegaCallback* m_pCallback;
	ManualLock m_lock;

};

class CLink;
class CUserDataSpi : public CWSClientSpi
{
public:
	CUserDataSpi(CLink* pLink);

protected:
	virtual void OnWSConnected();
	virtual void OnWSDisConnected();
	virtual void OnData(const char* data, size_t length);
	virtual void OnPoll();

private:
	CLink* m_pLink;
};

class CLink : public COneMegaApi, public CWSClientSpi
{
	friend class CUserDataSpi;

	typedef struct tagExpiredOrderInfo
	{
		MegaOrderStruct order;
		time_t insertTime;
	} ExpiredOrderInfo;

	typedef map< string, ExpiredOrderInfo > CExpiredOrderMap;

public:
	CLink();
	~CLink();

	///创建UserApi
	///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
	///@param pszUserApiType UserAPI类型
	///@return 创建出的Api实例
	static COneMegaApi *CreateApi();

	///获取版本号
	static const char *GetVersion();

	///启动接口线程,只有调用后,API才开始连接服务器
	virtual void Connect();

	virtual void DisConnect() { return; };

	///设置前置服务器网络地址
	///@param pAddress：前置服务器网络地址
	///@remark 网络地址的格式为："protocol://ip:port"，如："tcp://192.168.1.8:8888"
	///@remark “tcp|shm|multi”代表传输协议，"192.168.1.8"代表服务器地址。"8888"代表服务器端口号
	virtual void SetAddress(const char *pAddress);

	///注册回调接口
	///@param pCallback 派生自回调接口类的实例
	virtual void SetCallback(COneMegaCallback *pCallback);

	///以下函数共同点说明：
	///@param nRequestNo:用户自定义请求号,在对应的RecvSend或者RecvSendQry函数处原值返回
	///@return： 0:发送成功
	///@return：-1:发送线程错误
	///@return：-2:服务端未回消息超标
	///@return：-3:发送端发送消息数超标		
	///创建上层连接请求
	virtual int SendInitLink(MegaLinkStruct *pLink, int nRequestNo);
	///用户登录请求
	virtual int SendUserLogin(MegaUserLoginStruct *pUserLogin, int nRequestNo);
	///订阅主题请求
	virtual int SendTopicAction(MegaTopicActionStruct *pTopicAction, int nRequestNo);
	///报单录入请求
	virtual int SendOrderInsert(MegaOrderInsertStruct* pOrderInsert, int nRequestNo);
	///触发单录入请求
	virtual int SendTriggerOrderInsert(MegaTriggerOrderInsertStruct* pTriggerOrderInsert, int nRequestNo);

	///报单操作请求
	virtual int SendOrderAction(MegaOrderActionStruct* pOrderAction, int nRequestNo);

	///调整持仓请求
	virtual int SendPositionAction(MegaPositionActionStruct* pPositionAction, int nRequestNo);

	///触发单操作请求
	virtual int SendTriggerOrderAction(MegaTriggerOrderActionStruct* pTriggerOrderAction, int nRequestNo);

	///标的查询请求
	virtual int SendQryInstrument(MegaQryInstrumentStruct* pQryInstrument, int nRequestNo);

	///K线查询请求
	virtual int SendQryKLine(MegaQryKLineStruct* pQryKLine, int nRequestNo);

	////////////////////////////////////////////////////////////////////////////////////////////////
protected:
	virtual void OnWSConnected();
	virtual void OnWSDisConnected();
	virtual void OnData(const char* data, size_t length);
	virtual void OnPoll() override;

protected:
	void OnTrade(Json::Value &Value);
	void OnFundingRate(Json::Value &Value);
	void OnIndex(Json::Value &Value);
	void OnOrder(Json::Value &Value);
	void OnKline(Json::Value& Value);
	void OnQryKline(Json::Value& Value, bool bIsLast);

	void OnOrderUpdate(Json::Value& Value);

	void OnQryInst(Json::Value& Value, bool bIsLast);

	void GetJsonValue(int value, char* szValue, int len);
	void GetJsonValue(double value, char* szValue, int len);
	void GetJsonValue(const char* value, char* szValue, int len);

	template<typename T>
	void AppendJsonData(char* json, const char* key, T value, bool first = false);

	void GetQueryValue(int value, char* szValue, int len);
	void GetQueryValue(INT_8 value, char* szValue, int len);
	void GetQueryValue(double value, char* szValue, int len);
	void GetQueryValue(const char* value, char* szValue, int len);

	template<typename T>
	void AppendQueryData(char* query, const char* key, T value, bool first = false);

	void TransOrderPriceType(const char* source, MegaOrderPriceTypeType& target);
	void TransOrderStatus(const char* source, MegaOrderStatusType& target, bool& bExpired);
	void TransTriggertatus(const char* source, MegaTriggerStatusType& target, bool& bExpired);

	void InitTrade(const MegaOrderStruct& order, MegaTradeStruct& trade);

	bool GenListenKey(string& listenKey);

	template<typename T1>
	int SendOrder(T1* pOrder, const char* type, string& errMsg, int nRequestNo);

	template<typename T1>
	int OrderAction(T1* pOrder, string& errMsg, int nRequestNo);

	template<typename T1, typename T2>
	void CopyOrder(const T1& source, T2& target);

	void AppendTriggerPrice(const char* key, MegaOrderInsertStruct* pOrderInsert, char* target) {}
	void AppendTriggerPrice(const char* key, MegaTriggerOrderInsertStruct* pOrderInsert, char* target)
	{
		AppendQueryData(target, key, pOrderInsert->TriggerPrice);
	}

	void AppendSLTriggerPrice(const char* key, MegaOrderInsertStruct* pOrderInsert, char* target) {}
	void AppendSLTriggerPrice(const char* key, MegaTriggerOrderInsertStruct* pOrderInsert, char* target)
	{
		AppendQueryData(target, key, pOrderInsert->SLTriggerPrice);
	}

	void SetStatus(const char* status, MegaOrderStruct& order, bool& bExpired)
	{
		TransOrderStatus(status, order.OrderStatus, bExpired);
	}

	void SetStatus(const char* status, MegaTriggerOrderStruct& order, bool& bExpired)
	{
		TransTriggertatus(status, order.TriggerStatus, bExpired);
	}

	int ParseErrorMsg(Json::Value& data, string& errMsg);

	template<typename T>
	void ParseOrder(Json::Value& data, T& order);

	void OnUserLogon(MegaUserLoginStruct* pUserLogin);

	int m_nLinkNo;

private:
	COneMegaCallbackWrap* m_pCallback;
	CWSClient m_wsClient;
	CWSClient m_wsUserDataClient;
	string m_strWSBaseUrl;
	CHttpClient m_httpClient;

	string m_extraHeader;
	string m_secretKey;
	int m_nId;

	CUserDataSpi m_userDataSpi;
	string m_listenKey;
	bool m_bUserDataStreamReady;

	MegaUserLoginStruct m_userLogin;
	bool m_bHasTrade;
	CExpiredOrderMap m_expiredOrders;

	time_t m_lastListenKeyTime;

	int m_nPeriodSubCnt;
	time_t m_lastSubTime;
};

template<typename T>
void CLink::AppendJsonData(char* json, const char* key, T value, bool first/* = false*/)
{
	char szValue[128] = {};
	GetJsonValue(value, szValue, sizeof(szValue));

	char buff[256] = {};
	if (first)
		snprintf(buff, sizeof(buff) - 1, "\"\%s\":%s", key, szValue);
	else
		snprintf(buff, sizeof(buff) - 1, ",\"\%s\":%s", key, szValue);

	strcat(json, buff);
}

template<typename T>
void CLink::AppendQueryData(char* query, const char* key, T value, bool first/* = false*/)
{
	char szValue[128] = {};
	GetQueryValue(value, szValue, sizeof(szValue));

	char buff[256] = {};
	if (first)
		snprintf(buff, sizeof(buff) - 1, "%s=%s", key, szValue);
	else
		snprintf(buff, sizeof(buff) - 1, "&%s=%s", key, szValue);

	strcat(query, buff);
}

template<typename T1>
int CLink::SendOrder(T1* pOrderInsert, const char* type, string& errMsg, int nRequestNo)
{
	char buff[1024] = {};
	strupr(pOrderInsert->InstrumentID);
	AppendQueryData(buff, "symbol", pOrderInsert->InstrumentID, true);
	AppendQueryData(buff, "newClientOrderId", pOrderInsert->LocalID);
	AppendQueryData(buff, "side", pOrderInsert->Direction == Mega_Direction_Buy ? "BUY" : "SELL");
	AppendQueryData(buff, "quantity", pOrderInsert->Volume);
	AppendQueryData(buff, "type", type);

	if (0 == strcmp(type, "LIMIT")
		|| 0 == strcmp(type, "STOP")
		|| 0 == strcmp(type, "TAKE_PROFIT"))
	{
		AppendQueryData(buff, "price", pOrderInsert->Price);
		AppendQueryData(buff, "timeInForce", "GTC");
	}

	if (0 == strcmp(type, "STOP_MARKET") 
		|| 0 == strcmp(type, "STOP")
		|| 0 == strcmp(type, "TAKE_PROFIT_MARKET")
		|| 0 == strcmp(type, "TAKE_PROFIT"))
	{
		AppendTriggerPrice("stopPrice", pOrderInsert, buff);
	}

	AppendQueryData(buff, "recvWindow", "5000");

	timeb t;
	ftime(&t);
	INT_8 timestamp = t.time * 1000 + t.millitm;
	AppendQueryData(buff, "timestamp", timestamp);

	string signature = HmacShaEncode2(m_secretKey.c_str(), buff);

	AppendQueryData(buff, "signature", signature.c_str());

	char resp[1024] = {};
	bool res = m_httpClient.Post("/api/v3/order", buff, m_extraHeader.c_str(), resp, sizeof(resp) - 1);

	PRINT_TO_STD("SendOrderInsert,param:%s,result:%s,%s", buff, res ? "成功" : "失败", resp);

	if (!res)
	{
		errMsg = resp;
		return -1;
	}

	if (0 == resp[0])
		return 0;

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(resp, root))
	{
		PRINT_TO_STD("parse失败");
		return 0;
	}

	try
	{
		int errID = ParseErrorMsg(root, errMsg);
		if (errID != 0)
			return errID;
	}
	catch (...)
	{
		PRINT_TO_STD("execption");
		return 0;
	}
	return 0;
}

template<typename T1, typename T2>
void CLink::CopyOrder(const T1& source, T2& target)
{
	strncpy(target.InstrumentID, source.InstrumentID, sizeof(target.InstrumentID) - 1);
	strncpy(target.LocalID, source.LocalID, sizeof(target.LocalID) - 1);

	target.Direction = source.Direction;
	target.Price = source.Price;
}

template<typename T1>
int CLink::OrderAction(T1* pOrderAction, string& errMsg, int nRequestNo)
{
	if (pOrderAction->ActionFlag != Mega_ActionFlag_Delete)
		return -1;

	char buff[1024] = {};
	strupr(pOrderAction->InstrumentID);
	AppendQueryData(buff, "symbol", pOrderAction->InstrumentID, true);
	AppendQueryData(buff, "newClientOrderId", pOrderAction->OrderLocalID);
	AppendQueryData(buff, "orderId", pOrderAction->OrderID);
	timeb t;
	ftime(&t);
	INT_8 timestamp = t.time * 1000 + t.millitm;
	AppendQueryData(buff, "timestamp", timestamp);

	AppendQueryData(buff, "recvWindow", "5000");

	string signature = HmacShaEncode2(m_secretKey.c_str(), buff);

	AppendQueryData(buff, "signature", signature.c_str());
	char resp[1024] = {};
	bool res = m_httpClient.Delete("/api/v3/order", buff, m_extraHeader.c_str(), resp, sizeof(resp) - 1);

	PRINT_TO_STD("SendOrderAction,param:%s,result:%s,%s", buff, res ? "成功" : "失败", resp);

	if (!res)
	{
		errMsg = resp;
		return -1;
	}

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(resp, root))
	{
		errMsg = "解析异常";
		return -1;
	}

	try
	{
		int errID = ParseErrorMsg(root, errMsg);
		if (errID != 0)
			return errID;
	}
	catch (...)
	{
		printf("execption\n");
		errMsg = "execption";
		return -1;
	}
	return 0;
}

template<typename T>
void CLink::ParseOrder(Json::Value& root, T& order)
{
	snprintf(order.OrderID, sizeof(order.OrderID) - 1, "%.0lf", root["orderId"].asDouble());
	strncpy(order.LocalID, root["clientOrderId"].asCString(), sizeof(order.LocalID) - 1);
	order.Direction = (0 == strcmp("buy", root["side"].asCString())) ? Mega_Direction_Buy : Mega_Direction_Sell;
	order.Price = atof(root["price"].asCString());

	const char* status = root["status"].asCString();
	SetStatus(status, order);
}

#endif
