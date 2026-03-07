!!function!!
#include <set>
#include <string>
using namespace std;

typedef set<string> CPackageSet;
CPackageSet g_setUsePackage;

bool InUse(char *name)
{
	if(g_setUsePackage.find("*")!=g_setUsePackage.end())
		return true;
	return (g_setUsePackage.find(name)!=g_setUsePackage.end());
}
!!endfunction!!
!!enter api!!
!!travel self!!
!!command g_setUsePackage.insert(@name);!!
!!next!!
!!leave!!
!!enter project!!
!!let apiname=@apiname!!
!!let APINAME=@APINAME!!
!!let apiid=@apiid!!
!!let APIID=@APIID!!
!!leave!!
/////////////////////////////////////////////////////////////////////////
///!!@apiname!!!!@apiid!!Api.h
///定义了客户端的C++语言的API接口
/////////////////////////////////////////////////////////////////////////

#ifndef _!!@APINAME!!_!!@APIID!!API_H_
#define _!!@APINAME!!_!!@APIID!!API_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "!!@apiname!!!!@apiid!!DataStruct.h"

#if defined(!!@APINAME!!_!!@APIID!!API_IS_LIB) && defined(WIN32)
#ifdef LIB_!!@APINAME!!_!!@APIID!!_API_EXPORT
#define !!@APINAME!!_!!@APIID!!_API_EXPORT __declspec(dllexport)
#else
#define !!@APINAME!!_!!@APIID!!_API_EXPORT __declspec(dllimport)
#endif
#else
#define !!@APINAME!!_!!@APIID!!_API_EXPORT 
#endif

namespace !!@apiname!!!!@apiid!!Byte
{
    //Callback是交易数据的回调处理类，当有数据到达客户端时，API会回调本类中对应的接口
    //客户程序需要继承这个类，并实现回调函数，实现时候回调函数内不能有阻塞性操作
    //Send开头的函数为操作命令请求函数
	//Recv开头的函数为操作命令的应答的回调
	//SendQry开头的函数为查询请求函数
	//RecvQry开头的函数为查询请求应答的回调
	//Push开头的函数为服务器主动推送回调，前提是必须通过订阅函数订阅成功主题

	class C!!@apiname!!!!@apiid!!Callback
	{
	public:
		virtual ~C!!@apiname!!!!@apiid!!Callback(){};
		///当客户端与服务端建立连接时，该方法被调用。客户端后续需发起登录请求
		virtual void OnConnected(){};
		
		///当客户端与服务端连接断开时，该方法被调用。客户端打印日志即可，可不做处理，API会自动重新连接
		///@param nReason：4907:网络读失败;4098:网络写失败;8193:接收心跳超时;8194:发送心跳失败;8195:收到错误报文
		virtual void OnDisconnected(int nReason){};

		///以下函数的共同注释：
		///@param pError:错误信息,0表示正确
		///@param nRequestNo:用户自定义请求号,在对应的Recv函数放回相同值，用于定位报文
		///@param bLast:SendQy函数会返回多条记录，bLast标明是否此类回报的最后一个报文
		///@param nLocalNo:Push函数会返回的报文属于哪个本地主题，相同的nLocalNo的后续订阅会覆盖前订阅
		///@param nSequenceNo:Push函数会返回的报文的nLocalNo为Key的主题序列号
	!!enter FTD!!
	!!travel packages!!
!!if InUse(@name)!!
	!!if !strcmp(@usage,"response") !!
		///!!@comment!!
		virtual void !!@name!!(!!travel self!!!!if @pumpid!=0!!,!!endif!!const !!@apiid!!!!@name!!Struct *p!!@name!!!!next!!,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
	!!elseif !strcmp(@usage,"Push")!!
		///!!@comment!!
		virtual void !!@name!!(!!travel self!!!!if @pumpid!=0!!,!!endif!!const !!@apiid!!!!@name!!Struct *p!!@name!!!!next!!,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
	!!endif!!
!!endif!!
	!!next!!
	!!leave!!
	};

	class !!@APINAME!!_!!@APIID!!_API_EXPORT C!!@apiname!!!!@apiid!!Api
	{
	public:
		///创建UserApi
		///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
		///@param pszUserApiType UserAPI类型
		///@return 创建出的Api实例
		static C!!@apiname!!!!@apiid!!Api *CreateApi();
		
		///获取版本号
		static const char *GetVersion();
		
		///启动接口线程,只有调用后,API才开始连接服务器
		virtual void Connect() = 0;
		
		///主动断开和服务器的连接
		virtual void DisConnect() = 0;
		
		///设置前置服务器网络地址
		///@param pAddress：前置服务器网络地址
		///@remark 网络地址的格式为："protocol://ip:port"，如："tcp://192.168.1.8:8888"
		///@remark “tcp|shm|multi”代表传输协议，"192.168.1.8"代表服务器地址。"8888"代表服务器端口号
		virtual void SetAddress(const char *pAddress) = 0;
		
		///注册回调接口
		///@param pCallback 派生自回调接口类的实例
		virtual void SetCallback(C!!@apiname!!!!@apiid!!Callback *pCallback) = 0;
		
		///加载证书
		///@param pCertFileName 用户证书文件名
		///@param pKeyFileName 用户私钥文件名
		///@param pCaFileName 可信任CA证书文件名
		///@param pKeyFilePassword 用户私钥文件密码
		///@return 0 操作成功;-1 可信任CA证书载入失败;-2 用户证书载入失败;-3 用户私钥载入失败;-4 用户证书校验失败
		virtual int SetCertificateFile(const char *pCertFileName, const char *pKeyFileName, const char *pCaFileName, const char *pKeyFilePassword) { return 0; };
		
		///以下函数共同点说明：
		///@param nRequestNo:用户自定义请求号,在对应的Recv函数处原值返回
		///@return： 0:发送成功;-1 发送线程错误;-2 服务端未回消息超标;-3 发送端发送消息数超标;-4 字段值不合法
	!!enter FTD!!
	!!travel packages!!
!!if InUse(@name)!!
	!!if !strcmp(@usage,"request")!!
		///!!@comment!!
		virtual int !!@name!!(!!travel self!!!!if @pumpid!=0!!, !!endif!!!!@apiid!!!!@name!!Struct *p!!@name!!!!next!!, int nRequestNo) { return 0; };
	!!endif!!
!!endif!!
	!!next!!
	!!leave!!

	protected:
		~C!!@apiname!!!!@apiid!!Api(){};
	};
}

using namespace !!@apiname!!!!@apiid!!Byte;

//动态调用函数
extern "C" !!@APINAME!!_!!@APIID!!_API_EXPORT C!!@apiname!!!!@apiid!!Api* Create!!@apiname!!!!@apiid!!Api();

//动态库动态调用的函数
typedef C!!@apiname!!!!@apiid!!Api* _Create!!@apiname!!!!@apiid!!Api();

#endif
