#ifndef __COneFtdToOkex_H_
#define __COneFtdToOkex_H_

#include "WSClient.h"
#include "platform.h"
#include "OneMegaApi.h"
#include "json/json.h"
#include "BaseFunction.h"
#include "UFDataType.h"

using namespace OneMegaByte;

typedef CUFFloatType<38, 10> CVolumeType;

class CLink : public COneMegaApi, public CWSClientSpi
{
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

	////////////////////////////////////////////////////////////////////////////////////////////////
protected:
	virtual void OnWSConnected() override;
	virtual void OnWSDisConnected() override;
	virtual void OnData(const char* data, size_t length) override;

protected:
	void OnTrade(Json::Value &Value);
	void OnFundingRate(Json::Value &Value);
	void OnIndex(Json::Value &Value);
	void OnOrder(Json::Value &Value);
	int m_nLinkNo;

private:
	COneMegaCallback *m_pCallback;
	CWSClient m_wsClient;
	string m_strWSBaseUrl;
	//set<string> m_sTopics;
	double m_nMessages;
	double m_VolumeRatio;
	int m_nId;
};

#endif
