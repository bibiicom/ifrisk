/////////////////////////////////////////////////////////////////////////
///OutFogServiceApi.h
///定义了类COutFogServiceApi
///@history 
///徐忠华 创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef _OUTFOG_SERVICE_API_H_
#define _OUTFOG_SERVICE_API_H_

#include "OutFogServiceApiStruct.h"

//此宏定义为统一规范，如需使用C++类里面的static或者const变量，必须定义OUTFOGSERVICE_API_EXPORT宏定义
//使用此API接口时，windows平台仅需定义WINDOWS宏定义即可，其他平台无需任何宏定义
#ifdef WINDOWS
#ifdef LIB_OUTFOGSERVICE_API_EXPORT
#ifdef OUTFOGSERVICE_API_IS_LIB
#define OUTFOGSERVICE_API_EXPORT __declspec(dllexport)
#else
#define OUTFOGSERVICE_API_EXPORT __declspec(dllimport)
#endif
#else
#define OUTFOGSERVICE_API_EXPORT
#endif
#else
#define OUTFOGSERVICE_API_EXPORT
#endif

class COutFogServiceCallback
{
public:
	//发送一个Field
	//如果cChain==OUTFOG_CHAIN_LAST,立刻发送
	//如果cChain==OUTFOG_CHAIN_CONTINUE,包满发送,否则等待下一个
	//如果cChain==OUTFOG_CHAIN_FIRST,全面的全部放弃，准备下一个报文
	virtual bool RspField(const TOutFogMetaRef* pRspRef, const void *pRspBuffer) = 0;
};

class OUTFOGSERVICE_API_EXPORT COutFogServiceApi
{
public:
	COutFogServiceApi(COutFogServiceCallback *pCallback = 0)
	{
		m_pServiceCallback = pCallback;
	}
	virtual ~COutFogServiceApi()
	{
	}

	virtual const char* getVersion() = 0;

	//异步请求接口，通过COutFogServiceCallback，可返回多条记录
	virtual bool ReqField(const TOutFogMetaRef* pReqRef, const void *pReqBuffer) = 0;

	//同步请求接口，通过参数返回，只返回一条记录
	virtual bool SyncReqField(const TOutFogMetaRef* pReqRef, const void *pReqBuffer, TOutFogMetaRef* pRspRef, void *pRspBuffer) = 0;

	virtual void setServiceCallback(COutFogServiceCallback *pCallback)
	{
		m_pServiceCallback = pCallback;
	}

	COutFogServiceCallback* getServiceCallback()
	{
		return m_pServiceCallback;
	}

protected:
	COutFogServiceCallback* m_pServiceCallback;
};

extern "C" OUTFOGSERVICE_API_EXPORT COutFogServiceApi* CreateOutFogServiceApi(COutFogServiceCallback *pCallback, int nServiceNo);

//动态库动态调用的函数
typedef COutFogServiceApi* _CreateOutFogServiceApi(COutFogServiceCallback *pCallback, int nServiceNo);

#endif

