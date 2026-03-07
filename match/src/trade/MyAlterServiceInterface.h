#ifndef MYAlterServiceInterface_H
#define MYAlterServiceInterface_H

#include "AlterServiceInterface.h"
#include "MyAlterService.h"

/////////////////////////////////////////////////////////////////////////
///CAlterServiceInterface是一个说明在内存运行的Dbmt服务的连接
/////////////////////////////////////////////////////////////////////////
class CMyAlterServiceInterface: public CAlterServiceInterface
{
public:
	///构造函数，根据一个Dbmt服务，创建一个内存运行的连接
	///@param	ppDbmtService	指定的dbmt服务
	///@param	pErrorEngine	用于返回错误的引擎
	///@param	verbose	true表示需要输出结果，flase表示不必输出结果
	CMyAlterServiceInterface(CMyAlterService *pDbmtService,CServiceResponser *pServiceResponser);
	
	///析构函数
	virtual ~CMyAlterServiceInterface();

	///处理一个XTP包
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleMessage(TOutFogMetaRef* pOutFogReqRef, void *pStream, TSessionRef* pSession);
	
private:
	CMyAlterService *m_pMyService;
	char m_pDbmtSteam[10000];
};

#endif
