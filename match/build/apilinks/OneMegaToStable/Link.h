#ifndef __CLink_H_
#define __CLink_H_

#include "platform.h"
#include "OneMegaApi.h"
#include "BaseFunction.h"
#include "Thread.h"
#include "UFDataType.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

using namespace OneMegaByte;

typedef set<string> CTopicsSet;

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
		int nCount = 0;
		while (true)
		{
			std::cout << "nCount: " << nCount <<"\n";
			OnTime(nCount);
			nCount++;
			if (nCount > 100 )
				nCount = 0;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	void OnTime(int nCount)
	{
		//读取最新行情信息
		std::ifstream fin("libOneMegaToStable.market");
		if (!fin.is_open()) {
			return;
		}

		std::string line;
		std::string symbol;
		double price;
		double volume;
		std::string sRemark;

		// 逐行读取
		while (std::getline(fin, line)) {
			// 跳过空行
			if (line.empty()) continue;
			std::istringstream iss(line);
			if (iss >> symbol >> price >> volume >> sRemark) {
				std::cout << "Symbol: " << symbol << ", Price: " << price << ", Volume: " << volume << ", Remark: " << sRemark << std::endl;
			
				CTopicsSet::iterator iter = m_TopicsSet.find(symbol);
				if (iter != m_TopicsSet.end())
				{
					std::srand(time(NULL));

					//PRINT_TO_STD("LinkNo[%d]:%s", m_nLinkNo, iter->c_str());
					MegaMarketOrderStruct order;
					memset(&order, 0, sizeof(MegaMarketOrderStruct));
					strcpy(order.InstrumentID, iter->c_str());

					order.Direction = Mega_Direction_Buy;
					CUFFloatType<28, 15> priceX = price;
					double tick = priceX.getTick();
					for (int x = 1; x < 6; x++)
					{
						order.Volume = volume * x;
						order.Price = price - tick * x;
						m_pCallback->PushMarketOrder(&order, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
					}
					order.Direction = Mega_Direction_Sell;
					for (int x = 1; x < 6; x++)
					{
						order.Volume = volume * x;
						order.Price = price + tick * x;
						m_pCallback->PushMarketOrder(&order, NULL, NULL, 0, "", 0, 0, ONE_CHAIN_SINGLE);
					}

					MegaMarketTradeStruct trade;
					memset(&trade, 0, sizeof(MegaMarketTradeStruct));
					strcpy(trade.InstrumentID, iter->c_str());

					trade.Volume = volume;
					if (std::rand() % 2 == 0)
					{
						trade.Direction = Mega_Direction_Buy;
						trade.Price = price + 0.001;
					}
					else
					{
						trade.Direction = Mega_Direction_Sell;
						trade.Price = price - 0.001;
					}

					if (std::rand() % 3 == 0)
						trade.Price = price;

					m_pCallback->PushMarketTrade(&trade, 0, "", 0, 0, ONE_CHAIN_SINGLE);
				}
			}
			else {
				std::cerr << "格式错误: " << line << "\n";
			}
		}
	}

	COneMegaCallback* m_pCallback;
	int m_nLinkNo;
	CTopicsSet m_TopicsSet;
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
