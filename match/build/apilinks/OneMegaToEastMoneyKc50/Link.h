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
		//https://47.push2.eastmoney.com/api/qt/clist/get?pn=1&pz=20&po=1&np=1&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&invt=2&wbp2u=|0|0|0|web&fid=f108&fs=m:10+c:588000&fields=f108,f12,f161&_=1703068855030

		//{
		//	"rc":0,
		//		"rt" : 6,
		//		"svr" : 181669449,
		//		"lt" : 1,
		//		"full" : 1,
		//		"dlmkts" : "",
		//		"data" : {
		//		"total":86,
		//			"diff" : [
		//		{
		//			"f12":"10005532",
		//				"f108" : 154514,
		//				"f161" : 0.95
		//		},
		//	{
		//		"f12":"10005531",
		//		"f108" : 141986,
		//		"f161" : 0.9
		//	},
		//	{
		//		"f12":"10005823",
		//		"f108" : 35280,
		//		"f161" : 1
		//	},
		//	{
		//		"f12":"10005824",
		//		"f108" : 28096,
		//		"f161" : 1.05
		//	},
		//	{
		//		"f12":"10006340",
		//		"f108" : 27510,
		//		"f161" : 0.9
		//	},
		//	{
		//		"f12":"10005629",
		//		"f108" : 19421,
		//		"f161" : 0.85
		//	}
		//	]
		//	}
		//}

		//vector<string> splits = Txtsplit(Rsp, "},{");

		MegaMarketOrderStruct field;
		memset(&field, 0, sizeof(MegaMarketOrderStruct));
		MegaMarketTradeStruct trade;
		memset(&trade, 0, sizeof(MegaMarketTradeStruct));

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(Rsp, root))
			return;
		try
		{
			Json::Value& data = root["data"]["diff"];
			for (int i = 0; i < data.size(); i++)
			{
				strcpy(field.InstrumentID, data[i]["f12"].asCString());
				strcpy(trade.InstrumentID, field.InstrumentID);
				EX(&field, &trade, data[i]["f161"].asDouble());
			}
		}
		catch (...)
		{
			printf("execption\n");
		}

		//for (int x = 0; x < splits.size(); x++)
		//{
		//	vector<string> eachSplit = Txtsplit(splits[x], "\",\"");
		//	strcpy(field.InstrumentID, eachSplit[1].c_str());
		//	EX(&field, &trade, atof(eachSplit[5].c_str()));
		//}
	}

	void EX(MegaMarketOrderStruct* field, MegaMarketTradeStruct* trade, double price)
	{
		trade->Volume = 1;
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
