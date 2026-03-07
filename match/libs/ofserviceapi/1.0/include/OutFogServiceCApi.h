/////////////////////////////////////////////////////////////////////////
///OutFogServiceCApi.h
///定义了类COutFogServiceCApi
///@history 
///徐忠华 创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef _OUTFOG_SERVICE_CAPI_H_
#define _OUTFOG_SERVICE_CAPI_H_

#include "OutFogServiceApiStruct.h"

#ifdef __cplusplus
extern "C" {
#endif

	//此宏定义为统一规范，如需使用C++类里面的static或者const变量，必须定义OUTFOGSERVICE_API_EXPORT宏定义
	//使用此API接口时，windows平台仅需定义WINDOWS宏定义即可，其他平台无需任何宏定义
#ifdef WINDOWS
#ifdef LIB_OUTFOGSERVICE_CAPI_EXPORT
#ifdef OUTFOGSERVICE_CAPI_IS_LIB
#define OUTFOGSERVICE_CAPI_EXPORT __declspec(dllexport)
#else
#define OUTFOGSERVICE_CAPI_EXPORT __declspec(dllimport)
#endif
#else
#define OUTFOGSERVICE_CAPI_EXPORT
#endif
#else
#define OUTFOGSERVICE_CAPI_EXPORT
#endif

//发送一个Field
//如果cChain==OUTFOG_CHAIN_LAST,立刻发送
//如果cChain==OUTFOG_CHAIN_CONTINUE,包满发送,否则等待下一个
//如果cChain==OUTFOG_CHAIN_FIRST,全面的全部放弃，准备下一个报文
	typedef void(*OutFogRspField)(void* pInstance, TOutFogMetaRef* pRspRef, void *pRspBuffer);

	struct TOutFogCallbacks
	{
		OutFogRspField ptrOutFogRspField;
	};

	///注册回调函数虚表
	///@param pInstance InitApi函数返回的API实例地址
	///@param callbacks 回调函数虚表的地址
	OUTFOGSERVICE_CAPI_EXPORT void OutFogRegisterCallbacks(void* pInstance, TOutFogCallbacks *pCallbacks);

	OUTFOGSERVICE_CAPI_EXPORT const char* OutFogGetVersion(void* pInstance);

	OUTFOGSERVICE_CAPI_EXPORT bool OutFogStart(void* pInstance);

	OUTFOGSERVICE_CAPI_EXPORT bool OutFogReqField(void* pInstance, const TOutFogMetaRef* pReqRef, const void *pBuffer);
	OUTFOGSERVICE_CAPI_EXPORT bool OutFogSyncReqField(const TOutFogMetaRef* pReqRef, const void *pReqBuffer, TOutFogMetaRef* pRspRef, void *pRspBuffer);

	OUTFOGSERVICE_CAPI_EXPORT int OutFogGetServiceID(void* pInstance);

	OUTFOGSERVICE_CAPI_EXPORT void* CreateOutFogServiceCApi(int nServiceNo);


	//动态库动态调用的函数
	typedef void _OutFogRegisterCallbacks(void* pInstance, TOutFogCallbacks *pCallbacks);
	typedef const char* _OutFogGetVersion(void* pInstance);
	typedef bool _OutFogStart(void* pInstance);
	typedef bool _OutFogReqField(void* pInstance, const TOutFogMetaRef* pReqRef, const void *pStream);
	typedef bool _OutFogSyncReqField(void* pInstance, const TOutFogMetaRef* pReqRef, const void *pReqStream, TOutFogMetaRef* pRspRef, void *pRspStream);

	typedef int _OutFogGetServiceID(void* pInstance);
	typedef void* _CreateOutFogServiceCApi(int nServiceNo);

#ifdef __cplusplus
}
#endif

#endif

