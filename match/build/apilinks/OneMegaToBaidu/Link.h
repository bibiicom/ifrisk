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
				bool ret = m_httpClient.Get("", NULL, "https://gushitong.baidu.com/", m_sRsp, 10239);

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
		//curl "https://finance.pae.baidu.com/api/getrevforeigndata?query=CNYHKD"

		//{
		//	"QueryID":"3891262057",
		//		"ResultCode" : "0",
		//		"Result" : {
		//		"revCode": [
		//		{
		//			"code":"CNYUSD",
		//				"name" : "\u4eba\u6c11\u5e01\u5151\u7f8e\u5143",
		//				"market" : "global",
		//				"ratio" : "0.09",
		//				"price" : "0.138",
		//				"increase" : "0.0001",
		//				"status" : "1",
		//				"list" : [
		//			{
		//				"text":"\u6700\u65b0\u4ef7",
		//					"value" : "0.1380"
		//			},
		//			{
		//				"text":"\u6da8\u8dcc\u989d",
		//				"value" : "+0.0001"
		//			},
		//			{
		//				"text":"\u6da8\u8dcc\u5e45",
		//				"value" : "+0.0900%"
		//			}
		//				],
		//				"icon1":"https:\/\/baidu-finance.cdn.bcebos.com\/imgs\/money_icon_v2\/01\u4eba\u6c11\u5e01.png",
		//					"icon2" : "https:\/\/baidu-finance.cdn.bcebos.com\/imgs\/money_icon_v2\/02\u7f8e\u5143.png"
		//		}
		//		],
		//			"corrCode":{
		//			"front": [
		//			{
		//				"code":"USDGBP",
		//					"name" : "\u7f8e\u5143\u82f1\u9551",
		//					"price" : {
		//					"value":"0.8024"
		//				},
		//					"increase" : "0.0023",
		//						"ratio" : {
		//						"value":"+0.2900%",
		//							"status" : "up"
		//					},
		//						"status" : "1"
		//			},
		//		{
		//			"code":"USDEUR",
		//			"name" : "\u7f8e\u5143\u6b27\u5143",
		//			"price" : {
		//				"value":"0.9213"
		//			},
		//			"increase" : "0.0022",
		//			"ratio" : {
		//				"value":"+0.2400%",
		//				"status" : "up"
		//			},
		//			"status" : "1"
		//		},
		//		{
		//			"code":"USDHKD",
		//			"name" : "\u7f8e\u5143\u6e2f\u5143",
		//			"price" : {
		//				"value":"7.8068"
		//			},
		//			"increase" : "0.0021",
		//			"ratio" : {
		//				"value":"+0.0300%",
		//				"status" : "up"
		//			},
		//			"status" : "1"
		//		},
		//		{
		//			"code":"USDJPY",
		//			"name" : "\u7f8e\u5143\u65e5\u5143",
		//			"price" : {
		//				"value":"150.3800"
		//			},
		//			"increase" : "-0.0100",
		//			"ratio" : {
		//				"value":"-0.0100%",
		//				"status" : "down"
		//			},
		//			"status" : "-1"
		//		},
		//		{
		//			"code":"USDCNY",
		//			"name" : "\u7f8e\u5143\u4eba\u6c11\u5e01",
		//			"price" : {
		//				"value":"7.2473"
		//			},
		//			"increase" : "-0.0065",
		//			"ratio" : {
		//				"value":"-0.0900%",
		//				"status" : "down"
		//			},
		//			"status" : "-1"
		//		},
		//		{
		//			"code":"USDTHB",
		//			"name" : "\u7f8e\u5143\u6cf0\u94e2",
		//			"price" : {
		//				"value":"35.4700"
		//			},
		//			"increase" : "-0.0400",
		//			"ratio" : {
		//				"value":"-0.1100%",
		//				"status" : "down"
		//			},
		//			"status" : "-1"
		//		}
		//			],
		//				"back": [
		//				{
		//					"code":"CNYGBP",
		//						"name" : "\u4eba\u6c11\u5e01\u82f1\u9551",
		//						"price" : {
		//						"value":"0.1107"
		//					},
		//						"increase" : "0.0004",
		//							"ratio" : {
		//							"value":"+0.3800%",
		//								"status" : "up"
		//						},
		//							"status" : "1"
		//				},
		//		{
		//			"code":"CNYEUR",
		//			"name" : "\u4eba\u6c11\u5e01\u6b27\u5143",
		//			"price" : {
		//				"value":"0.1271"
		//			},
		//			"increase" : "0.0004",
		//			"ratio" : {
		//				"value":"+0.3200%",
		//				"status" : "up"
		//			},
		//			"status" : "1"
		//		},
		//		{
		//			"code":"CNYHKD",
		//			"name" : "\u4eba\u6c11\u5e01\u6e2f\u5143",
		//			"price" : {
		//				"value":"1.0772"
		//			},
		//			"increase" : "0.0013",
		//			"ratio" : {
		//				"value":"+0.1200%",
		//				"status" : "up"
		//			},
		//			"status" : "1"
		//		},
		//		{
		//			"code":"CNYUSD",
		//			"name" : "\u4eba\u6c11\u5e01\u7f8e\u5143",
		//			"price" : {
		//				"value":"0.1380"
		//			},
		//			"increase" : "0.0001",
		//			"ratio" : {
		//				"value":"+0.0900%",
		//				"status" : "up"
		//			},
		//			"status" : "1"
		//		},
		//		{
		//			"code":"CNYJPY",
		//			"name" : "\u4eba\u6c11\u5e01\u65e5\u5143",
		//			"price" : {
		//				"value":"20.7505"
		//			},
		//			"increase" : "0.0184",
		//			"ratio" : {
		//				"value":"+0.0900%",
		//				"status" : "up"
		//			},
		//			"status" : "1"
		//		},
		//		{
		//			"code":"CNYTHB",
		//			"name" : "\u4eba\u6c11\u5e01\u6cf0\u94e2",
		//			"price" : {
		//				"value":"4.8929"
		//			},
		//			"increase" : "-0.0025",
		//			"ratio" : {
		//				"value":"-0.0500%",
		//				"status" : "down"
		//			},
		//			"status" : "-1"
		//		}
		//				],
		//					"front_title":"\u7f8e\u5143\u76f8\u5173",
		//					"back_title" : "\u4eba\u6c11\u5e01\u76f8\u5173"
		//		}
		//	}
		//}


		MegaMarketOrderStruct field;
		memset(&field, 0, sizeof(MegaMarketOrderStruct));
		MegaMarketTradeStruct trade;
		memset(&trade, 0, sizeof(MegaMarketTradeStruct));

		string strData(Rsp);
		//printf("%s \n", strData.c_str());
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(strData, root))
			return;
		try
		{
			//Json::Value& data = root["Result"]["revCode"];
			//for (int i = 0; i < data.size(); i++)
			//{
			//	Json::Value& eachdata = data[i];
			//	string instrumntid = eachdata["code"].asCString();
			//	strcpy(field.InstrumentID, instrumntid.c_str());
			//	strcpy(trade.InstrumentID, instrumntid.c_str());
			//	CPriceType Price = 1 / eachdata["price"].asDouble();
			//	Price.toLowerTick(0.0001);
			//	EX(&field, &trade, Price.getValue());
			//}

			//Json::Value& data = root["Result"]["corrCode"]["front"];
			//for (int i = 0; i < data.size(); i++)
			//{
			//	Json::Value& eachdata = data[i];
			//	string instrumntid = eachdata["code"].asCString();
			//	if (instrumntid == "USDCNY")
			//	{
			//		strcpy(field.InstrumentID, "USDCNY");
			//		strcpy(trade.InstrumentID, "USDCNY");
			//		EX(&field, &trade, eachdata["price"]["value"].asDouble());
			//	}
			//}

			Json::Value& data = root["Result"]["corrCode"]["back"];
			for (int i = 0; i < data.size(); i++)
			{
				Json::Value& eachdata = data[i];
				if (eachdata["price"]["value"].asDouble() == 0.0)
					continue;

				string instrumntid = eachdata["code"].asCString();
				if (instrumntid == "CNYHKD")
				{
					strcpy(field.InstrumentID, "HKDCNY");
					strcpy(trade.InstrumentID, "HKDCNY");
					CPriceType Price = 1 / eachdata["price"]["value"].asDouble();
					Price.toLowerTick(0.0001);
					EX(&field, &trade, Price.getValue());
				}
				if (instrumntid == "CNYUSD")
				{
					strcpy(field.InstrumentID, "USDCNY");
					strcpy(trade.InstrumentID, "USDCNY");
					CPriceType Price = 1 / eachdata["price"]["value"].asDouble();
					Price.toLowerTick(0.0001);
					EX(&field, &trade, Price.getValue());
				}
			}
		}
		catch (...)
		{
			printf("execption\n");
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
	char m_sRsp[10240];

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
