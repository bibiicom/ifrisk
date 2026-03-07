#ifndef BASEMatchWedgeS_H
#define BASEMatchWedgeS_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"
#include "monitorIndex.h"
#include "CDate.h"
#include "genTrade.h"

class CExecutionWedge : public CWedge
{
public:
	CExecutionWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	///析构方法，将负责释放所占用的空间
	virtual ~CExecutionWedge(void);

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_insertSingleTrade;
		result |= USE_updateInstrumentStatus;
//		result |= USE_insertCombOrder;
		return result;
	}

	virtual bool insertOrder(CWriteableOrder *pOrder,CTransaction* pTransaction);
	//virtual bool insertCombOrder(CWriteableCombOrder *pCombOrder,CTransaction* pTransaction);
	virtual bool insertSingleTrade(CWriteableTrade *pTrade, CWriteableOrder *pOrder);
	virtual bool updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus,CTransaction *pTransaction);
protected:

	CInstrumentTriggerForOrderBook* m_pInstrumentTrigger;
	COrderTriggerForOrderBook*		m_pOrderTrigger;
	CWriteableMarketData WritebleMarketData;
	//CEventMonitor m_eventMonitor;
};

/////////////////////////////////////////////////////////////////////////
///base08:CTimeSyncWedge是完成时间同步的交易规则，应当放在各个交易规则的前面使用
/////////////////////////////////////////////////////////////////////////
class CTimeSyncWedge : public CWedge
{
public:
	CTimeSyncWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "TimeSync";
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_timeSync;
		return result;
	}

	virtual bool timeSync(CWriteableCurrentTime *pCurrentTime);
};

/////////////////////////////////////////////////////////////////////////
///base12:CInstrumentStatusCheckWedge是合约状态检查的交易规则
/////////////////////////////////////////////////////////////////////////
class CInstrumentStatusCheckWedge : public CWedge
{
public:
	CInstrumentStatusCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		///时间初始化为空
		m_OldTime.clear();
		m_iInitialDay = CDate::DateToLong("19000101");
		m_iCurrTradingDay = m_iInitialDay;
		m_sName = "InstrumentStatusCheck";
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		result |= USE_updateInstrumentStatus;
		result |= USE_timeSync;
//		result |= USE_insertCombOrder;
		return result;
	}

	virtual bool insertOrder(CWriteableOrder *pOrder,CTransaction* pTransaction);
//	virtual bool insertCombOrder(CWriteableCombOrder *pCombOrder,CTransaction* pTransaction);
	virtual bool orderAction(CWriteableOrderAction *pOrderAction,CTransaction *pTransaction);
	virtual bool updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus,CTransaction *pTransaction);
	virtual bool timeSync(CWriteableCurrentTime *pCurrentTime);

	//为优化交易阶段切换逻辑，记录下上一次切换成功时，距离最近的交易阶段StartTime和
	//切换时的交易大段编号，从而避免每一个时钟报文都执行一遍切换逻辑；同时为了避免频繁
	//从内存表中读取日期和时间，采用成员变量来记录交易日和时间，以提高效率。
	//tom 切换改为按照交易阶段切换之后，每个合约的交易阶段可以从状态表获取，这里只记录time
private:

	//当前交易日，由于CDate的构造函数中判断逻辑较为复杂，所以此处直接使用CDate作为成员
	//变量的类型，而不使用CDateType，避免在后面使用处频繁调用CDate的构造函数以提高效率，
	//由于内存表中，给交易日初始化为19000101，故此处也会在构造函数中将其初始化为该值
	//CDate的LongToDate线程不安全，这里尽量避免使用，另外也可以提高效率	
	int     m_iCurrTradingDay;
	///默认交易日19000101折算成的日期数
	int     m_iInitialDay;

	///为避免频繁从内存表中读取OldTime，此处换成成员变量记录上一次的时间（不含毫秒）
	CTimeType m_OldTime;
};

/////////////////////////////////////////////////////////////////////////
///base17:CPriorityWedge是进行优先级设置的交易规则
/////////////////////////////////////////////////////////////////////////
class CPriorityWedge : public CWedge
{
public:
	///构造方法，创建优先级设置的交易规则
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	///@param	useYdPosition	使用昨持仓
	CPriorityWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "Priority";
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		return result;
	}

	virtual bool insertOrder(CWriteableOrder *pOrder,CTransaction* pTransaction);

	virtual bool orderAction(CWriteableOrderAction *pOrderAction,CTransaction *pTransaction);

protected:
	///设置优先级
	///@param	pOrder	要设置优先级的报单
	void setPriority(CWriteableOrder *pOrder);
	CWriteableOrder theOrder;
};

/////////////////////////////////////////////////////////////////////////
///tmodel01:CPriceTimeWedge是按照价格时间优先规则成交的交易规则
/////////////////////////////////////////////////////////////////////////
class CPriceTimeWedge : public CWedge
{
public:
	CPriceTimeWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine, ITradeMaker *pTradeMaker)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_TradeMaker = pTradeMaker;
		m_sName = "PriceTime";
	}

	~CPriceTimeWedge(void)
	{
		delete m_TradeMaker;
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_tryTradeAfterOrderInsert;
//		result |= USE_tryTradeAfterCombOrderInsert;
		return result;
	}

	virtual void tryTradeAfterOrderInsert(COrder *pOrder);

	//virtual bool tryTradeAfterCombOrderInsert(CWriteableCombOrder *pCombOrder,CTransaction *pTransaction);

private:
	ITradeMaker *m_TradeMaker;
	CWriteableOrder theOrder;
	CWriteableOrder theCountParty;
};

/////////////////////////////////////////////////////////////////////////
///tmodel02:CAuctionWedge是进行集合竞价的交易规则
/////////////////////////////////////////////////////////////////////////
class CAuctionWedge : public CWedge
{
public:
	CAuctionWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine, ITradeMaker *pTradeMaker)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_TradeMaker = pTradeMaker;
		m_sName = "Auction";
	}

	~CAuctionWedge(void)
	{
		delete m_TradeMaker;
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_updateInstrumentStatus;
		return result;
	}

	virtual bool updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus,CTransaction *pTransaction);

private:
	///成交生成器
	ITradeMaker *m_TradeMaker;
	CWriteableOrder theBuyOrder, theSellOrder;
};

class CCFDWedge : public CWedge
{
public:
	CCFDWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine, ITradeMaker *pTradeMaker)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_TradeMaker = pTradeMaker;
		m_sName = "CFD";
	}

	~CCFDWedge(void)
	{
		delete m_TradeMaker;
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_tryTradeAfterOrderInsert;
		return result;
	}

	virtual void tryTradeAfterOrderInsert(COrder *pOrder);

private:

	///成交生成器
	ITradeMaker *m_TradeMaker;
	CWriteableOrder theOrder;
};


#endif
