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
				bool ret = m_httpClient.Get("", NULL, "referer=https://finance.sina.com.cn", m_sRsp, 10239);

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
		//curl -H "referer=https://finance.sina.com.cn" "https://hq.sinajs.cn/rn=1700030658310list=DINIW,fx_susdcny,fx_seurcny,fx_scnyjpy,fx_sgbpcny"	
		//var hq_str_HKDCNY = "14:12:13,0.9306,0.9311,0.9304,15,0.9304,0.9306,0.9291,0.9306,港元人民币,2023-11-08";
		//var hq_str_USDCNY = "14:12:01,7.2763,7.2766,7.2811,123,7.2765,7.2780,7.2657,7.2763,美元人民币,2023-11-08";
		//var hq_str_hfCHA50CFD = "";
		//var hq_str_hk00700 = "TENCENT,腾讯控股,309.400,308.600,314.600,308.600,309.800,1.200,0.389,309.60001,309.79999,2508566329,8041889,0.000,0.000,413.658,215.963,2023/11/08,13:56";
		//var hq_str_sh512170 = "医疗ETF,0.423,0.424,0.425,0.431,0.422,0.425,0.426,1354878615,580143388.000,96308100,0.425,62662400,0.424,19378000,0.423,13639000,0.422,10123500,0.421,35243300,0.426,67324800,0.427,67027700,0.428,61798500,0.429,55656800,0.430,2023-11-08,14:12:48,00,";
		//var hq_str_sh513050 = "中概互联网ETF,1.010,1.010,1.006,1.018,1.005,1.006,1.007,496422262,502382823.000,8205300,1.006,10506620,1.005,9096100,1.004,5057900,1.003,4165100,1.002,7167300,1.007,8959500,1.008,6482000,1.009,5190400,1.010,3718400,1.011,2023-11-08,14:12:48,00,";
		//var hq_str_sh600519 = "贵州茅台,1784.050,1791.170,1791.440,1803.800,1782.870,1791.280,1791.450,1145489,2054994907.000,700,1791.280,200,1791.270,100,1791.260,100,1791.130,100,1790.030,200,1791.450,100,1791.460,200,1791.470,100,1791.540,100,1791.880,2023-11-08,14:12:49,00,";
		//var hq_str_sz000002 = "万 科Ａ,11.830,11.930,12.020,12.270,11.800,12.010,12.020,113796697,1377528312.700,104000,12.010,1134000,12.000,151200,11.990,149000,11.980,95100,11.970,204700,12.020,446000,12.030,227400,12.040,166100,12.050,518300,12.060,2023-11-08,14:12:48,00";
	
		vector<string> Instruments = Txtsplit(Rsp, ";");

		MegaMarketOrderStruct field;
		memset(&field, 0, sizeof(MegaMarketOrderStruct));
		MegaMarketTradeStruct trade;
		memset(&trade, 0, sizeof(MegaMarketTradeStruct));
		for (int i = 0; i < Instruments.size(); i++)
		{
			int k = 0;
			for (int j = 12; j < Instruments[i].size(); j++)
			{
				if (Instruments[i][j] == '=')
				{
					field.InstrumentID[k] = '\0';
					trade.InstrumentID[k] = '\0';
					break;
				}					
				field.InstrumentID[k] = Instruments[i][j];
				trade.InstrumentID[k] = Instruments[i][j];
				k++;
			}

			vector<string> MarketOrders = Txtsplit(Instruments[i], ",");

			switch (MarketOrders.size())
			{
			case 11:
			{
				break;
			}
			case 19:
			{
				break;
			}
			case 34:
			{
				field.Direction = Mega_Direction_Buy;
				for (int x = 10; x < 20; x++)
				{
					field.Volume = atof(MarketOrders[x].c_str());
					x++;
					field.Price = atof(MarketOrders[x].c_str());
					m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				}

				field.Direction = Mega_Direction_Sell;
				for (int x = 20; x < 30; x++)
				{
					field.Volume = atof(MarketOrders[x].c_str());
					x++;
					field.Price = atof(MarketOrders[x].c_str());
					m_pCallback->PushMarketOrder(&field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				}

				trade.Volume = atof(MarketOrders[8].c_str());

				map<string, double>::iterator iter = m_mapVolume.find(trade.InstrumentID);
				if (iter == m_mapVolume.end())
				{
					m_mapVolume.insert(map<string, double>::value_type(trade.InstrumentID, trade.Volume));
					trade.Volume = 0.0;
				}
				else
				{
					if (trade.Volume <= iter->second)
						iter->second = trade.Volume;
					else
						trade.Volume = trade.Volume - iter->second;
				}

				if (trade.Volume > 0)
				{
					trade.Price = atof(MarketOrders[3].c_str());
					if (trade.Price == atof(MarketOrders[6].c_str()))
						trade.Direction = Mega_Direction_Sell;
					else
						trade.Direction = Mega_Direction_Buy;
					m_pCallback->PushMarketTrade(&trade, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				}					

				break;
			}
			}

			int a = 0;
		}
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
	string m_sAddress;
};


#endif
