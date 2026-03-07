/////////////////////////////////////////////////////////////////////////
///QueryServiceInterface.h
///定义了类CAlterServiceInterface
/////////////////////////////////////////////////////////////////////////

#ifndef QueryServiceInterface_H
#define QueryServiceInterface_H

#include "ServiceInterface.h"
#include "CErrorEngine.h"
#include "CTransaction.h"
#include "accountAlgorithms.h"

/////////////////////////////////////////////////////////////////////////
///CQueryServiceInterface是一个说明在内存运行的交易查询服务的连接
/////////////////////////////////////////////////////////////////////////
class CQueryServiceInterface: public IServiceInterface
{
public:
	///构造函数，根据一个查询服务，创建一个内存运行的连接
	///@param	pQueryService	指定的dbmt服务
	///@param	pErrorEngine	用于返回错误的引擎
	///@param	pMDB	使用的内存数据库
	///@param	verbose	true表示需要输出结果，flase表示不必输出结果
	CQueryServiceInterface(CServiceResponser *pServiceResponser);
	
	///析构函数
	virtual ~CQueryServiceInterface();

	///处理一个XTP包
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleMessage(const TOutFogMetaRef* pOutFogReqRef,const void *pStream, TSessionRef* pSession);

	///处理一个XTP请求
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleRequest(const TOutFogMetaRef* pOutFogReqRef, const void* pStream, TSessionRef* pSession);

private:
	bool RspError(int nRecvTNo, TSessionRef* pSession, void *pField = NULL, CFieldDescribe *pFieldDescribe = NULL);
	CBaseDepositAlgorithms *m_pBaseDepositAlgorithms;
};

#endif
