#ifndef __CLink_H_
#define __CLink_H_

#include "platform.h"
#include "OneMegaApi.h"
#include "json/json.h"
#include "HttpClient.h"
#include "BaseFunction.h"
#include "Thread.h"
#include "UFDataType.h"

using namespace OneMegaByte;

#ifndef WINDOWS
typedef long long INT_8;
#else
typedef __int64 INT_8;
#endif
typedef CUFFloatType<28, 15> CPriceType;

class CHttpLoop :public CThread
{
public:
	CHttpLoop()
	{
	}

	~CHttpLoop()
	{

	}

	virtual void Run()
	{
		while (true)
		{
			if (m_httpClient.GetBaseUrl().length() != 0)
			{
				PRINT_TO_STD("LinkNo[%d]:%s", m_nLinkNo, m_httpClient.GetBaseUrl().c_str());
				bool ret = m_httpClient.Get("", NULL, "bug", m_sRsp, 10239);

				if (ret)
					PRINT_TO_STD("LinkNo[%d]:http_ok:%s", m_nLinkNo, m_sRsp);
				else
					PRINT_TO_STD("LinkNo[%d]:http_error:%s", m_nLinkNo, m_sRsp);

				OnMarketOrder(m_sRsp);
			}
#ifdef WIN32
			::Sleep(5000);
#else
			::usleep(5000000);
#endif
		}
		return;
	}

	void OnMarketOrder(const char* Rsp)
	{
		//curl "https://quote.eastmoney.com/center/api/qqzq.js?_=1700117557601"
		//var data = [
		//	"#area,code,name,date,time,new,high,low,change,percent",
		//		"0,CN10Y_B,中国10年期国债,20231116,153127,2.672,2.703,2.655,0.001,0.04",
		//		"0,CN2Y_B,中国2年期国债,20231116,152515,2.322,2.387,2.323,-0.028,-1.19",
		//		"0,CN5Y_B,中国5年期国债,20231116,152548,2.497,2.545,2.493,-0.012,-0.48",
		//		"0,EU_B,长期欧元债券,20231116,153221,130.78,130.84,130.47,0.31,0.24",
		//		"0,FH10Y_B,法国10年期国债,20231116,153228,3.171,3.178,3.168,-0.026,-0.8",
		//		"0,GM10Y_B,德国10年期国债,20231116,153218,2.608,2.61,2.604,-0.023,-0.87",
		//		"0,GRC10Y_B,希腊10年期国债,20231116,153214,3.858,4.009,3.893,-0.017,-0.44",
		//		"0,ID10Y_B,印度10年期国债,20231116,153111,7.222,7.236,7.22,-0.003,-0.04",
		//		"0,ITA10Y_B,意大利10年期国债,20231116,153219,4.401,4.455,4.363,-0.035,-0.79",
		//		"0,JP10Y_B,日本10年期国债,20231116,132428,0.788,0.806,0.786,-0.012,-1.44",
		//		"0,JP_B,日本政府债券,20231116,153232,145.39,145.41,145.11,0.2,0.14",
		//		"0,SP10Y_B,西班牙10年期国债,20231116,153214,3.63,3.677,3.596,-0.03,-0.83",
		//		"0,UK10Y_B,英国10年期国债,20231116,150000,4.2425,4.237,4.122,0.0125,0.3",
		//		"0,UK_B,英国公债,20231116,15955,96.05,97.12,95.93,-0.7,-0.72",
		//		"0,US10YT_N,美国10年期T-Note,20231116,152145,108.47,108.5,108.22,0.25,0.23",
		//		"0,US10Y_B,美国10年期国债,20231116,153200,4.499,4.541,4.495,-0.039,-0.85",
		//		"0,US2YT_N,美国2年期T-Note,20231116,152056,101.52,101.52,101.47,0.04,0.04",
		//		"0,US2Y_B,美国2年期国债,20231116,152949,4.893,4.91,4.89,-0.023,-0.48",
		//		"0,US30YT_B,美国30年期T-Bond,20231116,152135,114.69,114.81,114.19,0.44,0.38",
		//		"0,US5YT_N,美国5年期T-Note,20231116,152200,105.72,105.73,105.58,0.15,0.14",
		//		"0,US5Y_B,美国5年期国债,20231116,152948,4.487,4.512,4.484,-0.039,-0.86"
		//]

//curl "https://push2.eastmoney.com/api/qt/ulist.np/get?secids=0.300059,0.000002&fields=f1,f2,f3,f4,f12,f13,f14,f107"
	//{"rc":0,"rt":11,"svr":182996292,"lt":1,"full":1,"dlmkts":"","data":{"total":2,"diff":[{"f1":2,"f2":1529,"f3":-161,"f4":-25,"f12":"300059","f13":0,"f14":"东方财富","f107":5},{"f1":2,"f2":1180,"f3":-76,"f4":-9,"f12":"000002","f13":0,"f14":"万  科Ａ","f107":5}]}}
//curl "https://push2.eastmoney.com/api/qt/ulist.np/get?secids=1.000004'
//curl "https://push2.eastmoney.com/api/qt/stock/details/get?ut=fa5fd1943c7b386f172d6893dbfba10b&fields1=f1,f2,f3,f4&fields2=f51,f52,f53,f54,f55&pos=-11&secid=0.000004"

		vector<string> splits = Txtsplit(Rsp, "\",");

		MegaMarketOrderStruct field;
		memset(&field, 0, sizeof(MegaMarketOrderStruct));
		MegaMarketTradeStruct trade;
		memset(&trade, 0, sizeof(MegaMarketTradeStruct));

		vector<int> select;
		select.push_back(1);
		select.push_back(2);
		select.push_back(3);
		select.push_back(16);
		select.push_back(18);
		select.push_back(21);

		for (int x = 0; x < select.size(); x++)
		{
			vector<string> eachSplit = Txtsplit(splits[x], ",");
			strcpy(field.InstrumentID, eachSplit[1].c_str());
			strcpy(trade.InstrumentID, field.InstrumentID);
			EX(&field, &trade, atof(eachSplit[5].c_str()));
		}
	}

	void EX(MegaMarketOrderStruct* field, MegaMarketTradeStruct* trade, double price)
	{
		trade->Volume = 1000000;
		trade->Price = price;

		double LastPrice = trade->Price;
		map<string, double>::iterator iter = m_mapPrice.find(trade->InstrumentID);
		if (iter == m_mapPrice.end())
		{
			m_mapPrice.insert(map<string, double>::value_type(trade->InstrumentID, trade->Price));
		}
		else
		{
			LastPrice = iter->second;
			iter->second = trade->Price;
		}

		if (LastPrice > trade->Price)
			trade->Direction = Mega_Direction_Sell;
		else
			trade->Direction = Mega_Direction_Buy;

		double tick = 0.0001;

		field->Direction = Mega_Direction_Sell;
		field->Price = trade->Price + tick;
		field->Volume = trade->Volume * 100;
		m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);

		field->Direction = Mega_Direction_Buy;
		field->Price = trade->Price - tick;
		field->Volume = trade->Volume * 100;
		m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);

		m_pCallback->PushMarketTrade(trade, 0, "", 0, 0, ONE_CHAIN_SINGLE);
	}

	CHttpClient m_httpClient;
	COneMegaCallback* m_pCallback;
	string m_strWSBaseUrl;
	char m_sRsp[10240];
	map<string, double> m_mapVolume;
	map<string, double> m_mapPrice;
	int m_nLinkNo;
};

class CLink : public COneMegaApi
{
public:
	CLink();
	~CLink();

	///创建UserApi
	///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
	///@param pszUserApiType UserAPI类型
	///@return 创建出的Api实例
	static COneMegaApi* CreateApi();

	///获取版本号
	static const char* GetVersion();

	///启动接口线程,只有调用后,API才开始连接服务器
	virtual void Connect();

	virtual void DisConnect() { return; };

	///设置前置服务器网络地址
	///@param pAddress：前置服务器网络地址
	///@remark 网络地址的格式为："protocol://ip:port"，如："tcp://192.168.1.8:8888"
	///@remark “tcp|shm|multi”代表传输协议，"192.168.1.8"代表服务器地址。"8888"代表服务器端口号
	virtual void SetAddress(const char* pAddress);

	///注册回调接口
	///@param pCallback 派生自回调接口类的实例
	virtual void SetCallback(COneMegaCallback* pCallback);

	///以下函数共同点说明：
	///@param nRequestNo:用户自定义请求号,在对应的RecvSend或者RecvSendQry函数处原值返回
	///@return： 0:发送成功
	///@return：-1:发送线程错误
	///@return：-2:服务端未回消息超标
	///@return：-3:发送端发送消息数超标		
	///创建上层连接请求
	virtual int SendInitLink(MegaLinkStruct* pLink, int nRequestNo);
	///用户登录请求
	virtual int SendUserLogin(MegaUserLoginStruct* pUserLogin, int nRequestNo);
	///订阅主题请求
	virtual int SendTopicAction(MegaTopicActionStruct* pTopicAction, int nRequestNo);
	////////////////////////////////////////////////////////////////////////////////////////////////

protected:
	int m_nLinkNo;

private:
	int m_nId;
	CHttpLoop m_HttpLoop;
};


#endif
