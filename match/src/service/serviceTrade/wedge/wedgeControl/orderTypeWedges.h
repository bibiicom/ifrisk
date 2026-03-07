/////////////////////////////////////////////////////////////////////////
///orderTypeWedges.h
///	定义了多个与报单类型相关的交易规则类
/////////////////////////////////////////////////////////////////////////

#ifndef ORDERTYPEWedgeS_H
#define ORDERTYPEWedgeS_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"

/////////////////////////////////////////////////////////////////////////
///otype01:CStatusControlWedge是对状态进行控制的交易规则
/////////////////////////////////////////////////////////////////////////
class CStatusControlWedge : public CWedge
{
public:
	CStatusControlWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "StatusControl";
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		result |= USE_changePosition;
		result |= USE_insertSingleTrade;
		result |= USE_doAfterOrderChange;
		return result;
	}

	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);
	virtual bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);
	virtual bool changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction);
	virtual bool insertSingleTrade(CWriteableTrade *pTrade, CWriteableOrder *pOrder);
	virtual bool doAfterOrderChange(COrder *pOrder);
};

///定义某个合约的报单列表
typedef vector<COrder *> TInstrumentOrderList;

/////////////////////////////////////////////////////////////////////////
///COrderList是一个按照合约划分的报单表
/////////////////////////////////////////////////////////////////////////
//class COrderList : public map<CInstrument *, TInstrumentOrderList>
//{
//public:
//	///构造方法
//	COrderList(void);
//
//	///析构方法
//	~COrderList(void);
//
//	///插入报单
//	///@param	pOrder	要插入的报单
//	void insertOrder(COrder *pOrder);
//
//	///删除指定合约的所有报单
//	///@param	pInstrument	指定的合约
//	void removeOrders(CInstrument *pInstrument);
//
//	///获取指定合约的报单表
//	///@param	pInstrument	指定的合约
//	///@return	该合约的所有报单表
//	TInstrumentOrderList *getInstrumentOrders(CInstrument *pInstrument);
//};

/////////////////////////////////////////////////////////////////////////
///otype02:COrderTypeCheckWedge是对报单类型进行检查的规则
/////////////////////////////////////////////////////////////////////////
class COrderTypeCheckWedge : public CWedge
{
public:
	///构造方法，创建报单类型检查交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	COrderTypeCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "OrderTypeCheck";
	}

	///获取本规则实际使用的操作
	///@return	各个操作使用标志的或
	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		result |= USE_updateInstrumentStatus;
		result |= USE_doAfterOrderChange;
		return result;
	}

	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);

	virtual bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);

	virtual bool updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction);

	virtual bool doAfterOrderChange(COrder *pOrder);
private:

	///时间条件的处理，只有在真插入撮合队列时使用
	///@param	pOrder	要检查的报单
	///@return	true表示合法，false表示非法
	bool timeCheck(COrder *pOrder);

	///所有在最近一节中插入的GFS和GFA报单
//	COrderList m_GFSOrderList;
};

/////////////////////////////////////////////////////////////////////////
///otype05:COrderTypeControlWedge是对报单类型控制的规则
/////////////////////////////////////////////////////////////////////////
class COrderTypeControlWedge : public CWedge
{
public:
	COrderTypeControlWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "OrderTypeControl";
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
//		result |= USE_insertCombOrder;
		return result;
	}

	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);

//	virtual bool insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction* pTransaction);
};

/////////////////////////////////////////////////////////////////////////
///otype06:CAutoSuspendWedge是自动挂起的交易规则
/////////////////////////////////////////////////////////////////////////
class CAutoSuspendWedge : public CWedge, public CUserSessionCommitTrigger
{
public:
	CAutoSuspendWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	///析构方法
	~CAutoSuspendWedge(void);

	///获取本规则实际使用的操作
	///@return	各个操作使用标志的或
	virtual int getUse(void)
	{
		int result = 0;
		return result;
	}

	///删除后触发
	///@param	pUserSession	已经删除的CUserSession
	virtual void commitRemove(CWriteableUserSession *pUserSession);
};

/////////////////////////////////////////////////////////////////////////
///otype07:CForceCloseControlWedge是对强平单控制的规则
/////////////////////////////////////////////////////////////////////////
//class CForceCloseControlWedge : public CWedge, public CMarketDataCommitTrigger
//{
//public:
//	///构造方法，创建强平单控制规则类
//	///@param	pTradeService	使用的交易服务
//	///@param	pMDB	使用的内存数据库
//	///@param	pErrorEngine	报告错误的引擎
//	CForceCloseControlWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
//		:CWedge(pTradeService, pMDB, pErrorEngine)
//	{
//	}
//
//	///获取本交易规则的名称
//	///@return	本交易规则的名称
//	virtual char *getName(void)
//	{
//		return "ForceCloseControl";
//	}
//
//	///获取本规则实际使用的操作
//	///@return	各个操作使用标志的或
//	virtual int getUse(void)
//	{
//		int result = 0;
//		result |= USE_insertOrder;
//		result |= USE_orderAction;
//		return result;
//	}
//
//	///插入报单所需的操作
//	///@param	pOrder	要保存的报单
//	///@param	pSession	会话编号
//	///@param	pTransaction 事务
//	///@return	true表示成功，false表示失败
//	virtual bool insertOrder(CWriteableOrder *pOrder,CTransaction* pTransaction);
//
//	///检查报单是否可以做指定操作
//	///@param	pOrderAction	要进行的报单操作
//	///@param	pSession	会话号
//	///@param	pTransaction	当前操作所属的事务
//	///@return	true表示成功，false表示失败
//	virtual bool orderAction(CWriteableOrderAction *pOrderAction,CTransaction *pTransaction);
//};


#endif
