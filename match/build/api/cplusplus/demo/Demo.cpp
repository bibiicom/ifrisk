//  一个简单的例子
// 介绍接口的使用
// 本例将演示一个报单录入操作的过程

#include <stdio.h>
#include <windows.h>
#include "OfexSuperApi.h"
using namespace OUTFOG;

class CSimpleHandler : public COfexSuperCallback
{
public:
	// 构造函数，需要一个有效的Api实例的指针
	CSimpleHandler(COfexSuperApi *pApi):m_pApi(pApi) {}
	~CSimpleHandler () {}

	// 当客户端与量投科技建立起通信连接，客户端需要进行登录
	virtual void OnFrontConnected()
	{
		OfexUserLoginStruct sendStruct;
		memset(&sendStruct, 0, sizeof(sendStruct));
		strcpy(sendStruct.UserID, "0001");
		strcpy(sendStruct.Password, "111111");
		int nRtn = m_pApi->SendUserLogin(&sendStruct, 0);
		printf("m_pApi->SendUserLogin = [%d] \n", nRtn);
	}

	// 当客户端与量投科技通信连接断开时，该方法被调用
	virtual void OnFrontDisconnected(int nReason)
	{
		// 当发生这个情况后，API会自动重新连接，客户端可不做处理
		printf("OnFrontDisconnected\n");
	}

	// 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
	virtual void RecvSendUserLogin(const OfexRspUserLoginStruct *pRspUserLogin,const OfexErrorStruct *pError,int nRequestNo)
	{
		printf("OnRspUserLogin: \n");
		if(pError)
		{
			printf("ErrorCode=[%d],ErrorMsg=[%s] ", pError->ErrorNo, pError->ErrorMsg);
			if (pError->ErrorNo != 0) 
			{
				// 登录登失败，客户端需进行错误处理，重新登录
				return;
			}
		}

		printf("RequestID=[%d],UserID=[%s],maxlocalid=[%s]\n", nRequestNo, pRspUserLogin->UserID,pRspUserLogin->MaxLocalID);

		//订阅用户相关主题
		OfexSubscribeTopicStruct sendStruct;
		sendStruct.ResumeType = OfexResumeType_Quick;
		sendStruct.TopicNo = Ofex_TOPICNO_USER;
		m_pApi->SendSubscribeTopic(&sendStruct,0);

		//订阅公共主题
		sendStruct.TopicNo = Ofex_TOPICNO_PUBLIC;
		m_pApi->SendSubscribeTopic(&sendStruct,0);

		//订阅行情主题
		sendStruct.TopicNo = Ofex_TOPICNO_MARKET_5_DEPTH;
		strcpy(sendStruct.SubIndexName,"btc2008");
		m_pApi->SendSubscribeTopic(&sendStruct,0);
	}

	virtual void RecvPushDepthMarketData(const OfexDepthMarketDataStruct *pDepthMarketData,int nTopicNo,double nSequenceNo)
	{
		if(pDepthMarketData==NULL)
		{
			printf("pDepthMarketData is Null");
			return;
		}
		printf("ExchangeID=[%s],InstrumentID=[%s],LastpPrice=[%f]\n",pDepthMarketData->ExchangeID,pDepthMarketData->InstrumentID,pDepthMarketData->LastPrice);
	}

private:
	COfexSuperApi *m_pApi;
};

int main()
{
	printf("ApiVersion=[%s]\n", COfexSuperApi::GetVersion());
	COfexSuperApi *pApi = COfexSuperApi::CreateApi();

	CSimpleHandler sh(pApi);
	pApi->SetCallback(&sh);


	// 设置域名服务器的前置地址，可以注册多个地址
	pApi->SetNameServerAddress("tcp://127.0.0.1:22701");
	pApi->SetNameServerAddress("tcp://127.0.0.1:22702");
	
	// 设置服务器的前置地址，可以注册多个地址
	// 前置地址的优先级大于域名服务其地址
	pApi->SetFrontAddress("tcp://127.0.0.1:12701");
	pApi->SetFrontAddress("tcp://127.0.0.1:12702");

	// 使客户端开始与后台服务建立连接
	pApi->Connect();
	pApi->Join();
	// 释放API实例
	pApi->Release();
	return 0;
}
