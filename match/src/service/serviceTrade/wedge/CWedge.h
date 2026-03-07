#ifndef CWedge_H
#define CWedge_H

//#include "CConfig.h"
#include "databaseData.h"
#include "CTransaction.h"
#include "mdb.h"
#include "CErrorEngine.h"
#include "CTradeService.h"

#define R_insertOrder 0
#define R_orderAction 1
#define R_insertSingleTrade 2
#define R_updateInstrumentStatus 3
#define R_timeSync 4
#define R_tryTradeAfterOrderInsert 5
#define R_doAfterOrderChange 6
#define R_changePosition 7
//#define R_insertCombOrder 8
//#define R_tryTradeAfterCombOrderInsert 9
#define R_UseLess 8

#define MAX_RULE_ACTION R_UseLess

#define USE_insertOrder						(1<<R_insertOrder)
#define USE_orderAction						(1<<R_orderAction)
#define USE_insertSingleTrade				(1<<R_insertSingleTrade)
#define USE_updateInstrumentStatus			(1<<R_updateInstrumentStatus)
#define USE_timeSync						(1<<R_timeSync)
#define USE_tryTradeAfterOrderInsert		(1<<R_tryTradeAfterOrderInsert)
#define USE_doAfterOrderChange				(1<<R_doAfterOrderChange)
#define USE_changePosition					(1<<R_changePosition)
//#define USE_insertCombOrder				(1<<R_insertCombOrder)
//#define USE_tryTradeAfterCombOrderInsert	(1<<R_tryTradeAfterCombOrderInsert)


#define RET_FALSE 0		//失败，不继续
#define RET_CONTINUE 1  //成功，继续
#define RET_BREAK 2		//成功，不继续


/////////////////////////////////////////////////////////////////////////
///CWedge是规则的抽象类，各个规则需要继承他
/////////////////////////////////////////////////////////////////////////
class CWedge
{
public:
	///构造方法，创建该交易引擎规则抽象类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	CWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	///析构方法，将负责释放所占用的空间
	virtual ~CWedge(void);

	///插入报单所需的操作
	///@param	pOrder	要插入的报单
	///@param	pSession	会话编号
	///@param	pTransaction 事务
	///@return	true表示成功，false表示失败
	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
	{
		return true;
	}

	///插入组合报单所需的操作
	///@param	pCombOrder	要插入的组合报单
	///@param	pSession	会话编号
	///@param	pTransaction 事务
	///@return	true表示成功，false表示失败
	//virtual bool insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction* pTransaction)
	//{
	//	return true;
	//}

	///检查报单是否可以做指定操作
	///@param	pOrderAction	要进行的报单操作
	///@param	pSession	会话号
	///@param	pTransaction	当前操作所属的事务
	///@return	true表示成功，false表示失败
	virtual bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction)
	{
		return true;
	}

	///插入单边成交所需的操作
	///@param	pTrade	要插入的单边成交
	///@param	pOrder	本成交对应的报单
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool insertSingleTrade(CWriteableTrade *pTrade, CWriteableOrder *pOrder)
	{
		return true;
	}

	///合约状态变化
	///@param	pInstrumentStatus	合约状态信息
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction)
	{
		return true;
	}

	///时间提醒
	///@param	CurrTime	当前时间
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool timeSync(CWriteableCurrentTime *pCurrentTime)
	{
		return true;
	}

	///在报单插入后尝试成交
	///@param	pOrder	要尝试成交的报单
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	virtual void tryTradeAfterOrderInsert(COrder *pOrder)
	{
		return;
	}

	///在组合报单插入后尝试成交
	///@param	pCombOrder	要尝试成交的组合报单
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	//virtual bool tryTradeAfterCombOrderInsert(CWriteableCombOrder *pCombOrder)
	//{
	//	return true;
	//}

	///成交后处理
	///@param	pOrder	完成成交后的报单内容
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	virtual bool doAfterOrderChange(COrder *pOrder)
	{
		return true;
	}

	///变更客户持仓
	///@param	pChangeList	客户持仓变更列表
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示操作成功，false表示操作失败
	virtual bool changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction)
	{
		return true;
	}

	virtual const char *getName();

	virtual int getUse();

protected:
	///报告内部错误
	///@param	msg	错误信息
	//void internalWarning(char *msg);

	///交易服务
	CTradeService *m_TradeService;

	///使用的交易数据
	CMDB *m_pMDB;

	///使用的错误引擎
	CErrorEngine *m_pErrorEngine;

	//Wedge名
	string m_sName;
};

///报告内部错误
///@param	pErrorEngine	错误引擎
///@param	msg	错误信息
//void internalWarning(CErrorEngine *pErrorEngine, char *msg);

#endif
