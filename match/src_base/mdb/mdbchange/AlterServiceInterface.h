/////////////////////////////////////////////////////////////////////////
///AlterServiceInterface.h
///定义了类CAlterServiceInterface
/////////////////////////////////////////////////////////////////////////

#ifndef AlterServiceInterface_BASE_H
#define AlterServiceInterface_BASE_H

#include "ServiceInterface.h"
#include "CErrorEngine.h"
#include "CTransaction.h"
#include "DataType.h"
#include "AlterService.h"

/////////////////////////////////////////////////////////////////////////
///CAlterServiceInterface是一个说明在内存运行的Dbmt服务的连接
/////////////////////////////////////////////////////////////////////////
class CAlterServiceInterface: public IServiceInterface
{
public:
	///构造函数，根据一个Dbmt服务，创建一个内存运行的连接
	///@param	ppDbmtService	指定的dbmt服务
	///@param	pErrorEngine	用于返回错误的引擎
	///@param	verbose	true表示需要输出结果，flase表示不必输出结果
	CAlterServiceInterface(CAlterService *pDbmtService, CServiceResponser *pServiceResponser);
	
	///析构函数
	virtual ~CAlterServiceInterface();

	///处理一个XTP包
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleMessage(const TOutFogMetaRef* pOutFogReqRef, const void *pStream,TSessionRef* pSession);

	///处理一个XTP请求
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleRequest(const TOutFogMetaRef* pOutFogReqRef,const void *pStream, TSessionRef* pSession);
	
protected:
	CAlterService *m_pService;
};

#endif
