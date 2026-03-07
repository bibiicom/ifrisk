#ifndef _SettlementINTERFACE_H_
#define _SettlementINTERFACE_H_

#include "ServiceInterface.h"
#include "CErrorEngine.h"
#include "CTransaction.h"
#include "monitorIndex.h"
#include "CSettlementService.h"

class CSettlementServiceInterface : public IServiceInterface
{
public:
	///构造函数
	///@param	pService	指定的Settlement服务
	///@param	pErrorEngine	用于返回错误的引擎
	///@param	verbose	true表示需要输出结果，flase表示不必输出结果
	CSettlementServiceInterface(CSettlementService* pService, CServiceResponser* pServiceResponser);

	///析构函数
	virtual ~CSettlementServiceInterface();

	///处理一个XTP包
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleMessage(const TOutFogMetaRef* pOutFogReqRef, const void* pStream, TSessionRef* pSession);

	///处理一个XTP请求
	///@param	pPackage	要处理的XTP包
	///@param	pSession	会话引用
	///@return	true表示处理成功，false表示处理失败
	virtual bool handleRequest(const TOutFogMetaRef* pOutFogReqRef, const void* pStream, TSessionRef* pSession);

private:
	bool RspError(int nRecvTNo, TSessionRef* pSession, void* pField = NULL, CFieldDescribe* pFieldDescribe = NULL);
	CSettlementService* m_pService;
};

#endif
