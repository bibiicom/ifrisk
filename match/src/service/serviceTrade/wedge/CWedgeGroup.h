#ifndef CWedgeGROUP_H
#define CWedgeGROUP_H

#include "CWedge.h"

/////////////////////////////////////////////////////////////////////////
///CWedgeGroup是一组交易规则组成的交易规则，实际上就是逐一叫用这组
///交易规则的相应方法
/////////////////////////////////////////////////////////////////////////
class CWedgeGroup : public CWedge
{
public:
	///构造方法，创建该交易规则组
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	CWedgeGroup(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	///析构方法，将负责释放所占用的空间
	virtual ~CWedgeGroup(void);

	virtual int getUse(void);

	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);

//	virtual bool insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction* pTransaction);

	virtual bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);

	virtual bool insertSingleTrade(CWriteableTrade *pTrade, CWriteableOrder *pOrder);

	virtual bool updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction);

	virtual bool timeSync(CWriteableCurrentTime *pCurrentTime);

	virtual void tryTradeAfterOrderInsert(COrder *pOrder);

//	virtual bool tryTradeAfterCombOrderInsert(CWriteableCombOrder *pCombOrder);

	virtual bool doAfterOrderChange(COrder *pOrder);

	virtual bool changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction);

	///加入一条新的交易规则
	///@param	pRule	要加入的规则
	///@param	pFilter	应用本规则的过滤器，NULL表示全部应用，不进行过滤
	///@return	true表示成功，false表示失败
	bool add(CWedge *pRule);
	/*
	///删除一条交易规则
	///@param	pRule	要删除的规则
	///@param	pFilter	要删除的规则的过滤器，NULL表示原来没有设置过滤器
	///@return	true表示成功，false表示失败
	bool remove(CWedge *pRule, CWedgeFilter *pFilter=NULL);
	*/
private:
	vector<CWedge*> rules[MAX_RULE_ACTION];
};

#endif
