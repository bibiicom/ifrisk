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
		//curl "http://qt.gtimg.cn/q=hfCHA50CFD,hk00700,sh512170,sh513050,sh600519,sz000002"	
		//v_HKDCNY = "12:36:00,0.7900,0.7910,0.7899,8,0.7899,0.7900,0.7892,0.7900,港元人民币";
		//v_USDCNY = "12:25:02,6.1264,6.1268,6.1295,54,6.1296,6.1305,6.1251,6.1264,美元人民币";
		//v_hk00700 = "100~腾讯控股~00700~306.200~308.400~305.000~8249501.0~0~0~306.200~0~0~0~0~0~0~0~0~0~306.200~0~0~0~0~0~0~0~0~0~8249501.0~2023/11/09 15:07:49~-2.200~-0.71~308.800~305.000~306.200~8249501.0~2528064823.870~0~13.82~~0~0~1.23~29113.2459~29113.2459~TENCENT~6.75~414.200~208.470~0.57~40.37~0~0~0~0~0~13.54~3.53~0.09~100~-2.32~6.10~GP~25.82~13.13~6.47~-1.54~-8.81~9507918306.00~9507918306.00~25.80~20.666~306.451~-19.46~HKD~1~30";
		//v_sh512170 = "1~医疗ETF~512170~0.422~0.427~0.427~8930549~4227293~4703256~0.422~115452~0.421~108048~0.420~187022~0.419~56445~0.418~106314~0.423~235511~0.424~269697~0.425~170190~0.426~152875~0.427~123947~~20231109152307~-0.005~-1.17~0.431~0.422~0.422/8930549/380189458~8930549~38019~1.36~~~0.431~0.422~2.11~277.98~277.98~0.00~0.470~0.384~0.70~-378939~0.426~~~~~~38018.9458~0.0000~0~ ~ETF~-16.93~2.93~~~~0.571~0.386~7.65~-0.24~0.72~65872820500~65872820500~-24.84~-22.71~65872820500~-0.07~0.422~-15.94~0.00~0.4281~CNY~0~___D__F__N~0.430~-383241";
		//v_sh513050 = "1~中概互联网ETF~513050~1.005~1.007~1.007~4805657~2277333~2528324~1.005~25449~1.004~42782~1.003~47539~1.002~53076~1.001~19838~1.006~71401~1.007~64590~1.008~55244~1.009~58043~1.010~30881~~20231109152306~-0.002~-0.20~1.012~1.002~1.005/4805657/483741599~4805657~48374~1.34~~~1.012~1.002~0.99~359.91~359.91~0.00~1.108~0.906~0.70~-91475~1.007~~~~~~48374.1599~0.0000~0~ ~ETF~-3.46~2.55~~~~1.206~0.882~2.87~-3.46~-7.12~35812432700~35812432700~-19.51~-9.21~35812432700~3.62~0.970~12.17~0.00~0.9720~CNY~0~___D__F__Y~1.012~-72853";
		//v_sh600519 = "1~贵州茅台~600519~1794.11~1798.34~1790.11~12800~5869~6931~1794.10~2~1794.05~3~1794.03~4~1794.00~101~1793.83~4~1794.11~4~1794.30~17~1794.33~2~1794.38~2~1794.39~8~~20231109152218~-4.23~-0.24~1799.00~1783.00~1794.11/12800/2291996819~12800~229200~0.10~31.66~~1799.00~1783.00~0.89~22537.57~22537.57~10.35~1978.17~1618.51~0.54~81~1790.62~31.97~35.94~~~1.02~229199.6819~0.0000~0~ ~GP-A~5.47~0.82~2.67~32.69~28.19~1935.00~1387.33~7.29~0.45~-5.09~1256197800~1256197800~55.10~-1.31~1256197800~~~27.05~0.01~~CNY~0~___D__F__N~1793.80~32";
		//v_sz000002 = "51~万  科Ａ~000002~11.96~12.13~12.10~814956~398753~416203~11.96~1937~11.95~12598~11.94~2864~11.93~5039~11.92~5166~11.97~1480~11.98~1858~11.99~3221~12.00~3635~12.01~1504~~20231109152209~-0.17~-1.40~12.15~11.91~11.96/814956/978437341~814956~97844~0.84~7.45~~12.15~11.91~1.98~1162.11~1426.91~0.56~13.34~10.92~0.64~15906~12.01~7.86~6.31~~~1.38~97843.7341~0.0000~0~ ~GP-A~-31.75~6.60~5.65~7.57~1.90~19.95~11.07~3.37~-6.85~-14.72~9716667215~11930709471~40.47~-29.79~9716667215~~~-13.80~-0.08~~CNY~0~~11.90~10722";

		MegaMarketOrderStruct field;
		memset(&field, 0, sizeof(MegaMarketOrderStruct));
		MegaMarketTradeStruct trade;
		memset(&trade, 0, sizeof(MegaMarketTradeStruct));

		vector<string> Instruments = Txtsplit(Rsp, ";\n");

		for (int i = 0; i < Instruments.size(); i++)
		{
			int k = 0;
			for (int j = 2; j < Instruments[i].size(); j++)
			{
				if (Instruments[i][j] == '=' || Instruments[i][j] == ' ')
				{
					field.InstrumentID[k] = '\0';
					trade.InstrumentID[k] = '\0';
					break;
				}
				field.InstrumentID[k] = Instruments[i][j];
				trade.InstrumentID[k] = Instruments[i][j];
				k++;
			}

			vector<string> MarketOrders = Txtsplit(Instruments[i], "~");
			//PRINT_TO_STD("--> %d", MarketOrders.size());
			switch (MarketOrders.size())
			{
			case 78:
			{
				HK(&MarketOrders, &field, &trade);
				break;
			}
			case 87:
			{
				CN(&MarketOrders, &field, &trade);
				break;
			}
			}
		}
	}

	void HK(vector<string>* MarketOrders, MegaMarketOrderStruct* field, MegaMarketTradeStruct* trade)
	{
		trade->Volume = 0.0;
		map<string, double>::iterator iter = m_mapVolume.find(trade->InstrumentID);
		double volume = atof((*MarketOrders)[6].c_str()) / 2;
		if (iter == m_mapVolume.end())
		{
			m_mapVolume.insert(map<string, double>::value_type(trade->InstrumentID, volume));
		}
		else
		{
			if (volume <= iter->second)
				iter->second = volume;
			else
				trade->Volume = volume - iter->second;
		}

		if (trade->Volume > 0.0)
		{
			trade->Price = atof((*MarketOrders)[3].c_str());
			double LastPrice = trade->Price;
			iter = m_mapPrice.find(trade->InstrumentID);
			if (iter == m_mapPrice.end())
			{
				m_mapPrice.insert(map<string, double>::value_type(trade->InstrumentID, trade->Price));
			}
			else
			{
				LastPrice = iter->second;
				iter->second = trade->Price;
			}

			if (LastPrice != trade->Price)
			{
				if (LastPrice > trade->Price)
					trade->Direction = Mega_Direction_Sell;
				else
					trade->Direction = Mega_Direction_Buy;

				CPriceType TickPrice = trade->Price;
				double tick = TickPrice.getTick();

				field->Direction = Mega_Direction_Sell;
				field->Price = trade->Price + tick * 5;
				field->Volume = trade->Volume * 50;
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				field->Price = trade->Price + tick * 4;
				field->Volume = trade->Volume * 40;
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				field->Price = trade->Price + tick * 3;
				field->Volume = trade->Volume * 30;
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				field->Price = trade->Price + tick * 2;
				field->Volume = trade->Volume * 20;
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				field->Price = trade->Price + tick * 1;
				field->Volume = trade->Volume * 30;
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);

				field->Direction = Mega_Direction_Buy;
				field->Price = trade->Price - tick * 1;
				field->Volume = trade->Volume * 40;
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				field->Price = trade->Price - tick * 2;
				field->Volume = trade->Volume * 30;
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				field->Price = trade->Price - tick * 3;
				field->Volume = trade->Volume * 20;
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				field->Price = trade->Price - tick * 4;
				field->Volume = trade->Volume * 30;
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				field->Price = trade->Price - tick * 5;
				field->Volume = trade->Volume * 50;
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
			}
			m_pCallback->PushMarketTrade(trade, 0, "", 0, 0, ONE_CHAIN_SINGLE);
		}
	}

	void CN(vector<string>* MarketOrders, MegaMarketOrderStruct* field, MegaMarketTradeStruct* trade)
	{
		field->Direction = Mega_Direction_Buy;
		for (int x = 9; x < 19; x++)
		{
			field->Price = atof((*MarketOrders)[x].c_str());
			x++;
			field->Volume = atof((*MarketOrders)[x].c_str());
			if (field->Volume > 0.0)
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
		}

		field->Direction = Mega_Direction_Sell;
		for (int x = 19; x < 29; x++)
		{
			field->Price = atof((*MarketOrders)[x].c_str());
			x++;
			field->Volume = atof((*MarketOrders)[x].c_str());
			if (field->Volume > 0.0)
				m_pCallback->PushMarketOrder(field, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
		}

		trade->Volume = atof((*MarketOrders)[6].c_str());

		map<string, double>::iterator iter = m_mapVolume.find(trade->InstrumentID);
		if (iter == m_mapVolume.end())
		{
			m_mapVolume.insert(map<string, double>::value_type(trade->InstrumentID, trade->Volume));
			trade->Volume = 0.0;
		}
		else
		{
			if (trade->Volume <= iter->second)
				iter->second = trade->Volume;
			else
				trade->Volume = trade->Volume - iter->second;
		}

		if (trade->Volume > 0.0)
		{
			trade->Price = atof((*MarketOrders)[3].c_str());
			if (trade->Price == atof((*MarketOrders)[9].c_str()))
				trade->Direction = Mega_Direction_Sell;
			else
				trade->Direction = Mega_Direction_Buy;
			m_pCallback->PushMarketTrade(trade, 0, "", 0, 0, ONE_CHAIN_SINGLE);
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
