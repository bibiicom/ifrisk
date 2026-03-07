#ifndef _TRADESERVICEINTERFACE_H_
#define _TRADESERVICEINTERFACE_H_

#include "ServiceInterface.h"
#include "CErrorEngine.h"
#include "CTransaction.h"
#include "monitorIndex.h"
#include "CTradeService.h"
#include "CSettlementService.h"

class CTradeServiceInterface : public IServiceInterface
{
public:
	///构造函数，根据一个trade服务，创建一个内存运行的连接
	///@param	pService	指定的trade服务
	///@param	pErrorEngine	用于返回错误的引擎
	///@param	verbose	true表示需要输出结果，flase表示不必输出结果
	CTradeServiceInterface(CTradeService* pService, CServiceResponser* pServiceResponser);

	///析构函数
	virtual ~CTradeServiceInterface();

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

	void CancleOtherOrder(CInstrument* pInstrument, CPriceType Price, CDirectionType Direction);
	//	bool CancleMyOrder(CInstrument *pInstrument, CLongIndexIDType MemberID, CPriceType Price, CDirectionType Direction);

	CSettlementService* m_pSettlementService;
private:

	bool RspError(int nRecvTNo, TSessionRef* pSession, void* pField = NULL, CFieldDescribe* pFieldDescribe = NULL);

	CTradeService* m_pService;

	CUFINT8TotalMonitorIndex* m_handleOrderInsertIndex, * m_handleOrderActionIndex;
	CUFINT8TotalMonitorIndex* m_handleOrderInsertErrorIndex, * m_handleOrderActionErrorIndex;
	FILE* m_InsertFailedLog, * m_ActionFailedLog;
	char BufferTemp[20480];

	COrderField m_OrderField;
	COrderInsertField m_OrderInsertField;
	COrderActionField m_OrderActionField;
	CPositionField m_PositionField;

	CWriteableOrder m_Order;
	CWriteableOrderAction m_OrderAction;

};

#endif
