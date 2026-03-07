/////////////////////////////////////////////////////////////////////////
///CLoginServiceInterface.h
///定义了类 CLoginServiceInterface
///@history 
/////////////////////////////////////////////////////////////////////////

#ifndef LOGINServiceInterface_H
#define LOGINServiceInterface_H

#include "ServiceInterface.h"
#include "CErrorEngine.h"
#include "CTransaction.h"
#include "monitorIndex.h"
#include "LoginService.h"

/////////////////////////////////////////////////////////////////////////
///CLoginServiceInterface是一个说明在Login服务的连接
/////////////////////////////////////////////////////////////////////////

class CLoginServiceInterface: public IServiceInterface
{
public:
	///构造函数，根据一个login服务，创建一个内存运行的连接
	///@param	pLoginService	指定的trade服务
	///@param	pErrorEngine	用于返回错误的引擎
	///@param	verbose	true表示需要输出结果，flase表示不必输出结果
	CLoginServiceInterface(CLoginService *pLoginService, CServiceResponser *pServiceResponser);
	
	///析构函数
	virtual ~CLoginServiceInterface();

	///处理一个XTP包
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleMessage(const TOutFogMetaRef* pOutFogReqRef,const void *pStream, TSessionRef* pSession);

	///处理一个XTP请求
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleRequest(const TOutFogMetaRef* pOutFogReqRef,const void *pStream, TSessionRef* pSession);
	
private:
	CLoginService *m_pLoginService;
};

#endif
