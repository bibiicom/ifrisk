/////////////////////////////////////////////////////////////////////////
///@system 交易服务
///CTradeService.h
///定义了类CTradeService
/////////////////////////////////////////////////////////////////////////

#ifndef CTradeService_H
#define CTradeService_H

#include "errorDefine.h"
#include "DataType.h"
#include "CPositionChangeList.h"
#include "OrderBook.h"
#include "CSequence.h"
#include "FieldData.h"
#include "ServiceInterface.h"
#include "tradingTools.h"
//#include "MarketDataTriggers.h"
#include "KlineTriggers.h"

//typedef struct SMaretPriceGap
//{
//	double GapPrice;
//	double Volume;
//} SMaretPriceGap;

//typedef queue<SMaretPriceGap> CMarkedPriceQueue;
//typedef map<string, CMarkedPriceQueue*> CMarkedPriceQueues;

class CWedge;
class ITradeMaker;
class COrderMBLTrigger;
/////////////////////////////////////////////////////////////////////////
///CTradeService是一个交易功能的类
/////////////////////////////////////////////////////////////////////////
class CTradeService
{
public:
	CTradeService(CMDB *pMDB, CErrorEngine *pErrorEngine);
	virtual ~CTradeService(void);

	bool insertOrder(CWriteableOrder *pOrder, CTransaction *pTransaction);
	bool insertCloseTriggerOrder(CWriteableOrder *pOrder, CWriteableTriggerOrder *pTriggerOrder, CTransaction *pTransaction);

	//bool insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction *pTransaction);

	//	bool doAfterOrderChange(COrder *pOrder, TSessionRef* pSession, CTransaction *pTransaction);

	//	virtual bool tryTrade(COrder *pOrder,TSessionRef* pSession,	CTransaction *pTransaction);

		///尝试进行成交
		///@param	pCombOrder	要成交的组合报单
		///@param	pSession	会话编号
		///@param	pTransaction	本操作属于的事务
		///@return	true表示成功，false表示失败
	//	virtual bool tryTrade(CWriteableCombOrder *pCombOrder,TSessionRef* pSession,CTransaction *pTransaction);

		///插入报价
		///@param	pQuote	要插入的报价
		///@param	pSession	会话编号
		///@param	pTransaction	本操作属于的事务
		///@return	true表示成功，false表示失败
	//virtual bool insertQuote(CWriteableQuote *pQuote, CTransaction *pTransaction);

	///报单操作
	///@param	pOrderAction	要进行的报单操作
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);
	bool orderActionBatch(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);
	bool orderActionBatchPrice(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);

	bool orderActionBatchManager(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);

	bool triggerOrderInsert(CWriteableTriggerOrder *pTriggerOrder, CTriggerStatusType triggerStatus, CTransaction *pTransaction, CReadOnlyPriceType *pBasePrice = NULL);
	CTriggerOrder* triggerOrderAction(CTriggerOrderActionField *pTriggerOrderAction, CTransaction *pTransaction);
	bool triggerOrderActionBatch(CTriggerOrderActionField *pTriggerOrderAction, CTransaction *pTransaction);

	COrder* closeOrderAction(CCloseOrderActionField *pCloseOrderAction, CTransaction *pTransaction);
	bool closeOrderActionBatch(CCloseOrderActionField *pCloseOrderAction, CTransaction *pTransaction);

//	bool changePositionType(CPositionActionField *pPositionActionField, CTransaction *pTransaction);
	///报价操作
	///@param	pQuoteAction	要进行的报价操作
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	//virtual bool quoteAction(CWriteableQuoteAction *pQuoteAction, CTransaction *pTransaction);

	///场外成交对申报操作
	///@param	pOTCTrade	要进行的成交对申报操作
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool insertOTCTrade(COTCTradeField *pOTCTrade, CTransaction *pTransaction);

	virtual bool insertWashOnly(COTCTradeField *pOTCTrade, CTransaction *pTransaction);

	virtual bool ReqPositionMerge(CPositionMergeField *pPositionMerge,CPositionField *pPositionOut, CTransaction *pTransaction);
	
	virtual CPosition* PositionMerge(CPositionMergeField *pPositionMerge, CPosition *pPosition, CLongIndexIDType PositionID, CTransaction *pTransaction);
	virtual CPosition* PositionAdd(CPosition *pPosition1, CPosition *pPosition2, CTransaction *pTransaction);
	virtual CPosition* PositionSub(CPositionMergeField *pPositionMerge, CPosition *pPosition1, CPosition *pPosition2, CTransaction *pTransaction);

	virtual bool ReqPositionReverse(CPositionReverseField *pPositionReverse, CWriteableOrder *pOutOrder, CMember *pMember, TSessionRef* pSession, CTransaction *pTransaction);
	///插入单边成交
	///@param	pTrade	要插入的成交
	///@param	pOrder	本成交对应的报单，NULL表示本成交不对应报单
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool insertSingleTrade(CWriteableTrade *pTrade, CWriteableOrder *pOrder);

	///时间提醒
	///@param	CurrTime	当前时间
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool timeSync(CWriteableCurrentTime *pCurrentTime);

	///合约状态变化
	///@param	pInstrumentStatus	合约状态信息
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction);

	///变更客户持仓
	///@param	pChangeList	客户持仓变更列表
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示操作成功，false表示操作失败
	virtual bool changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction);

	///设置交易规则
	///@param	pWedge	要设置的交易规则
	void setWedge(CWedge *pWedge);

	///获取OrderBookList
	COrderBookList* getOrderBookList();

	//virtual bool snapMDB(UF_INT8 nSequenceNo, const char* dumpFileName = NULL);
	//virtual bool snapAllMDB(UF_INT8 nSequenceNo, const char* dumpFileName = NULL);

	//标的物行情
	virtual CMarketData* ReqMarketDataUpdate(CInstrument* pInstrument, CMarketDataUpdateField *pMarketDataField, CTransaction *pTransaction);

	bool SetLastKLine(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CPriceType Price, CMilliSecondsType UpdateTime);

	bool SetBasisPrice(CWriteableMarketData *pMarketDataField, CInstrument* pInstrument);

	//持仓保证金操作
	virtual bool ReqPositionAction(CPositionActionField *pPositionActionField, CPositionField *pPositionField, CTransaction *pTransaction);
	virtual bool ReqPositionActionBatch(CPositionActionField *pPositionActionField, CTransaction *pTransaction);
	virtual bool ReqPositionCloseBatch(CPositionActionField *pPositionAction, CTransaction *pTransaction);
	virtual bool ChangeMemberPosition(CPositionActionField *pPositionActionField, CMemberPositionField *pMemberPositionField, CTransaction *pTransaction);

	virtual bool ReqMemberAction(CMemberActionField* pMemberActionField, CMember* pMember, CTransaction* pTransaction);

	//清理行情数据操作
	virtual bool ClearMarket(CClearMarketField *pClearMarketField, CTransaction *pTransaction);

	//撤单
	bool CancelAll(CInstrument *pInstrument, CTransaction *pTransaction);
	//撤销触发单
	bool CancelAllTriggerOrder(CInstrument *pInstrument, CTransaction *pTransaction);
	//清理LastKLine
	//bool ClearAllLastKLine(CInstrument *pInstrument, CTransaction *pTransaction);
	//清理KLine
	//bool ClearAllKLine(CInstrument *pInstrument, CTransaction *pTransaction);

	bool ClearLastKLine(CClearLastKLineField *pClearLastKLineField, CTransaction *pTransaction);

	bool UpdateSequence();

	CErrorEngine *m_pErrorEngine;
	COrderBookList* m_pOrderBookList;

private:

	CMDB *m_pMDB;

	//标记价格使用
	//CMarkedPriceQueues m_MarkedPriceQueues;

	bool triggerOrderPriceCheck(CWriteableTriggerOrder *pTriggerOrder, CReadOnlyPriceType *pBasePrice = NULL);

public:
	ITradeMaker *m_pTradeMaker;

	//临时为了WashOnly的变量
	CMarketTradeTrigger* m_pMarketTradeTrigger;
	CKLineTrigger* m_pKLineTrigger;
	CWedge *m_Wedge;

	//需要被外部调用的Wedges
	CWedge *m_pActionTargetCheckWedge;
};

#endif
