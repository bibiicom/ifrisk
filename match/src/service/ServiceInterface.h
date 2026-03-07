/////////////////////////////////////////////////////////////////////////
///IServiceInterface.h
///定义了接口IServiceInterface
/////////////////////////////////////////////////////////////////////////

#ifndef _IServiceInterface_H
#define _IServiceInterface_H

#include "DataType.h"
#include "OutFogServiceApi.h"
#include "ServiceResponser.h"
#include "tradingTools.h"

/////////////////////////////////////////////////////////////////////////
///IServiceMemoryLink是一个说明在内存运行的服务的接口
/////////////////////////////////////////////////////////////////////////
class IServiceInterface
{
public:
	///处理一个XTP包
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleMessage(const TOutFogMetaRef* pOutFogReqRef, const void* pStream, TSessionRef* pSession) = 0;
	///处理一个XTP请求
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleRequest(const TOutFogMetaRef* pOutFogReqRef, const void* pStream, TSessionRef* pSession) = 0;

	CServiceResponser* m_pServiceResponser;
	CTransaction* m_pTransaction;
	CErrorEngine* m_pErrorEngine;
	CMDB* m_pMDB;
};

#endif
