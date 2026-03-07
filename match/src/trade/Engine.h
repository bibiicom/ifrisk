/////////////////////////////////////////////////////////////////////////
///Engine.h
///定义了类CEngine
///@history 
/////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_H
#define ENGINE_H

#include "BaseEngine.h"
#include "ServiceInterface.h"

class CEngine: protected vector<IServiceInterface *>, protected CBaseEngine
{
public:
	///构造函数
	///@param	pConfig	创建引擎需要使用的配置
	CEngine();
	
	virtual ~CEngine(void);
	
	///处理一个内存XTP包
	///@param	pPackage	要处理的XTP包
	virtual void handleMessage(const TOutFogMetaRef* pOutFogReqRef,const void *pStream, TSessionRef* pSession);
	
	///处理一个请求
	///@param	pPackage	要处理的请求
	///@param	nSessionID	会话编号
	virtual void handleRequest(const TOutFogMetaRef* pOutFogReqRef,const void *pStream, TSessionRef* pSession);

protected:
	///增加一个内存服务连接
	///@param	pLink	要增加的内存服务连接
	void addServiceInterface(IServiceInterface *pLink);

	///删除一个内存服务连接
	///@param	pLink	要删除的内存服务连接
	///@return	true表示删除成功，flase表示删除失败
	bool removeServiceInterface(IServiceInterface *pLink);

	int m_seqID;
};

#endif
