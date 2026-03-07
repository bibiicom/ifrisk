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
!!let apiid=@apiid!!
!!let APINAME=@APINAME!!
!!let APIID=@APIID!!
!!leave!!
/////////////////////////////////////////////////////////////////////////
///!!@apiname!!!!@apiid!!Api.h
///定义了客户端API的C语言接口
/////////////////////////////////////////////////////////////////////////

#ifndef !!@APINAME!!_!!@APIID!!CAPI_H
#define !!@APINAME!!_!!@APIID!!CAPI_H

#include "!!@apiname!!!!@apiid!!DataStruct.h"

#if defined(WINDOWS) && !defined(WIN32)
#define WIN32
#endif

#if defined(!!@APINAME!!_!!@APIID!!CAPI_IS_LIB) && defined(WIN32)
#ifdef LIB_!!@APINAME!!_!!@APIID!!_CAPI_EXPORT
#define !!@APINAME!!_!!@APIID!!_CAPI_EXPORT __declspec(dllexport)
#else
#define !!@APINAME!!_!!@APIID!!_CAPI_EXPORT __declspec(dllimport)
#endif
#else
#define !!@APINAME!!_!!@APIID!!_CAPI_EXPORT __attribute__ ((dllimport))
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/// API容器指针
	typedef void* !!@apiname!!!!@apiid!!ApiIns;

	///////////////////////////////////////////////////
	/// 定义回调函数指针
	///////////////////////////////////////////////////

	///连接
	typedef void (*FuncOnConnected)(!!@apiname!!!!@apiid!!ApiIns Ins);
	///断连
	typedef void (*FuncOnDisconnected)(!!@apiname!!!!@apiid!!ApiIns Ins, int nReason);

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
	typedef void (*Func!!@name!!)(!!@apiname!!!!@apiid!!ApiIns Ins,!!travel self!!!!if @pumpid!=0!!,!!endif!!const !!@apiid!!!!@name!!Struct *p!!@name!!!!next!!,int nReqID,char cChain);
			!!elseif !strcmp(@usage,"Push")!!
	///!!@comment!!
	typedef void (*Func!!@name!!)(!!@apiname!!!!@apiid!!ApiIns Ins,!!travel self!!!!if @pumpid!=0!!,!!endif!!const !!@apiid!!!!@name!!Struct *p!!@name!!!!next!!,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
			!!endif!!
		!!endif!!	
	!!next!!
	!!leave!!

	////////////////////////////////////////////////
	/// 回调函数虚表
	////////////////////////////////////////////////

	typedef struct Callbacks
	{
		FuncOnConnected ptrOnConnected; ///连接
		FuncOnDisconnected ptrOnDisconnected; ///断线
	!!enter FTD!!
	!!travel packages!!
		!!if InUse(@name)!!
			!!if !strcmp(@usage,"response") || !strcmp(@usage,"Push")!!
		Func!!@name!! ptr!!@name!!; ///!!@comment!!
			!!endif!!
		!!endif!!	
	!!next!!
	!!leave!!
	} Callbacks;


	///////////////////////////////////////////////
	/// 主动请求函数
	///////////////////////////////////////////////

	///创建Api
	///@param pFlowPath 存贮订阅信息文件的目录，默认为当前目录
	///@return 创建出的Api
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT !!@apiname!!!!@apiid!!ApiIns Create!!@apiname!!!!@apiid!!CApi();

	///注册回调函数虚表
	///@param Ins Create!!@apiid!!Api函数返回的API实例地址
	///@param callbacks 回调函数虚表的地址
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT void SetCallback(!!@apiname!!!!@apiid!!ApiIns Ins, Callbacks *pCallback);

	///获取系统版本号
	///@return 版本号
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT const char *GetVersion(!!@apiname!!!!@apiid!!ApiIns Ins);

	///删除接口对象本身
	///@param Ins Create!!@apiid!!Api函数返回的API实例地址
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT void Release(!!@apiname!!!!@apiid!!ApiIns Ins);

	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT void Start(!!@apiname!!!!@apiid!!ApiIns Ins);

	///等待接口线程结束运行
	///@param Ins Create!!@apiid!!Api函数返回的API实例地址
	///@return 线程退出代码
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT int Join(!!@apiname!!!!@apiid!!ApiIns Ins);

	///主动断开和服务器的连接
	///@return 
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT void DisConnect(!!@apiname!!!!@apiid!!ApiIns Ins);

	///注册前置机网络地址
	///@param Ins Create!!@apiid!!Api函数返回的API实例地址
	///@param pAddress：前置服务器网络地址
	///@remark 网络地址的格式为："protocol://ip:port"，如："tcp://192.168.1.8:8888"
	///@remark “tcp|shm|multi”代表传输协议，"192.168.1.8"代表服务器地址。"8888"代表服务器端口号
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT void SetFrontAddress(!!@apiname!!!!@apiid!!ApiIns Ins,const char *pszFrontAddress);

	///加载证书
	///@param Ins Create!!@apiid!!Api函数返回的API实例地址
	///@param pszCertFileName 用户证书文件名
	///@param pszKeyFileName 用户私钥文件名
	///@param pszCaFileName 可信任CA证书文件名
	///@param pszKeyFilePassword 用户私钥文件密码
	///@return 0 操作成功;-1 可信任CA证书载入失败;-2 用户证书载入失败;-3 用户私钥载入失败;-4 用户证书校验失败
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT int SetCertificateFile(!!@apiname!!!!@apiid!!ApiIns Ins, const char *pszCertFileName, const char *pszKeyFileName, const char *pszCaFileName, const char *pszKeyFilePassword);

	///设置心跳超时时间。
	///@param Ins Create!!@apiid!!Api函数返回的API实例地址
	///@param timeout 心跳超时时间(秒)
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT void SetHeartbeatTimeout(!!@apiname!!!!@apiid!!ApiIns Ins, unsigned int nTimeout);

	///以下函数共同点说明：
	///@param nReqID:用户自定义请求号,在对应的RecvSend或者RecvQuery函数处原值返回
	///@return： 0:发送成功;-1 发送线程错误;-2 服务端未回消息超标;-3 发送端发送消息数超标
	!!enter FTD!!
		!!travel packages!!
			!!if InUse(@name)!!
				!!if !strcmp(@usage,"request")!!
	///!!@comment!!
	!!@APINAME!!_!!@APIID!!_CAPI_EXPORT int !!@name!!(!!@apiname!!!!@apiid!!ApiIns Ins, !!travel self!!!!if @pumpid!=0!!, !!endif!!!!@apiid!!!!@name!!Struct *p!!@name!!!!next!!, int nReqID);
				!!endif!!
			!!endif!!
		!!next!!
	!!leave!!

#ifdef __cplusplus
}
#endif

#endif
