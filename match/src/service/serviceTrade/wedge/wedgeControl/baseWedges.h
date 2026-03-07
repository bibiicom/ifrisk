#ifndef BASEWedgeS_H
#define BASEWedgeS_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"
#include "monitorIndex.h"
#include "CDate.h"
#include "CTradeService.h"

/////////////////////////////////////////////////////////////////////////
///base02:CFieldCheckWedge是进行基本的字段检查的交易规则
/////////////////////////////////////////////////////////////////////////
class CFieldCheckWedge : public CWedge
{
public:
	///构造方法，创建字段检查交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	CFieldCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "FieldCheck";
	}

	///获取本规则实际使用的操作
	///@return	各个操作使用标志的或
	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		result |= USE_updateInstrumentStatus;
//		result |= USE_insertCombOrder;
		return result;
	}

	///插入报单所需的操作
	///@param	pOrder	要保存的报单
	///@param	pSession	会话编号
	///@param	pTransaction 事务
	///@return	true表示成功，false表示失败
	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);

	///插入组合报单所需的操作
	///@param	pCombOrder	要保存的组合报单
	///@param	pSession	会话编号
	///@param	pTransaction 事务
	///@return	true表示成功，false表示失败
//	virtual bool insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction* pTransaction);


	///检查报单是否可以做指定操作
	///@param	pOrderAction	要进行的报单操作
	///@param	pSession	会话号
	///@param	pTransaction	当前操作所属的事务
	///@return	true表示成功，false表示失败
	virtual bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);

	///合约状态变化
	///@param	pInstrumentStatus	合约状态信息
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction);
private:
	///检查组合开平仓标志是否合法
	///@param	pInstrument	指定的合约
	///@param	pCombOffsetFlag	组合开平仓标志
	///@return	true表示合法，false表示非法
	//virtual bool validCombOffsetFlag(CInstrument *pInstrument, CReadOnlyCombOffsetFlagType &combOffsetFlag);
};

/////////////////////////////////////////////////////////////////////////
///base10:CActionTargetCheckWedge是操作对象检查的交易规则
/////////////////////////////////////////////////////////////////////////
class CActionTargetCheckWedge : public CWedge
{
public:
	///构造方法，创建操作对象检查交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	CActionTargetCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "ActionTarget";
	}

	///获取本规则实际使用的操作
	///@return	各个操作使用标志的或
	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		return result;
	}

	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);
	virtual bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);
	CWriteablePosition thePosition;
};

//////---------Instrument表触发Currency表
class CTableCurrencyCurrencyTrigger :public CInstrumentCommitTrigger
{
public:
	CTableCurrencyCurrencyTrigger(CMDB *pMDB);
	virtual ~CTableCurrencyCurrencyTrigger(void) {};
	virtual void commitAdd(CInstrument *pInstrument);
	virtual void commitUpdate(CInstrument *pInstrument, CWriteableInstrument *pOldInstrument);
	virtual void commitRemove(CWriteableInstrument *pInstrument);
protected:
	CCurrency* UpdateCurrency(CInstrument *pInstrument, CCurrency *pCurrency, CCurrencyType Currency);
	CMDB *m_pMDB;
};

//////---------UserSession表触发User表
//class CTableUserAPIKeyTrigger :public CUserSessionCommitTrigger
//{
//public:
//	CTableUserAPIKeyTrigger(CMDB *pMDB);
//	virtual ~CTableUserAPIKeyTrigger(void) {};
//	virtual void commitAdd(CUserSession *pUserSession);
//	virtual void commitRemove(CWriteableUserSession *pUserSession);
//protected:
//	CMDB *m_pMDB;
//};

//////---------UserSession表触发发送
class CUserSessionSendTrigger :public CUserSessionCommitTrigger
{
public:
	CUserSessionSendTrigger(CServiceResponser *pServiceResponser);
	virtual ~CUserSessionSendTrigger(void) {};
	virtual void commitAdd(CUserSession *pUserSession);
	virtual void commitUpdate(CUserSession *pUserSession, CWriteableUserSession *pOldUserSession);
	virtual void commitRemove(CWriteableUserSession *pUserSession);
protected:
	CServiceResponser *m_pServiceResponser;
};

/////////////////////////////////////////////////////////////////////////
///base11:CInstrumentCheckWedge是合约检查的交易规则
/////////////////////////////////////////////////////////////////////////
class CInstrumentCheckWedge : public CWedge
{
public:
	///构造方法，创建合约检查交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	CInstrumentCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine) :CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_pMDB = pMDB;
		m_sName = "InstrumentCheck";
	}

	~CInstrumentCheckWedge(void)
	{
	}

	///获取本规则实际使用的操作
	///@return	各个操作使用标志的或
	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_updateInstrumentStatus;
//		result |= USE_insertCombOrder;
		return result;
	}

	///插入报单所需的操作
	///@param	pOrder	要保存的报单
	///@param	pSession	会话编号
	///@param	pTransaction 事务
	///@return	true表示成功，false表示失败
	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);

	///插入组合报单所需的操作
	///@param	pCombOrder	要保存的组合报单
	///@param	pSession	会话编号
	///@param	pTransaction 事务
	///@return	true表示成功，false表示失败
//	virtual bool insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction* pTransaction);

	///合约状态变化
	///@param	pInstrumentStatus	合约状态信息
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction);

};

/////////////////////////////////////////////////////////////////////////
///base13:CVolumeCheckWedge是进行数量检查的交易规则
/////////////////////////////////////////////////////////////////////////
class CVolumeCheckWedge : public CWedge
{
public:
	///构造方法，创建数量检查交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	CVolumeCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine) :CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "VolumeCheck";
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		//result |= USE_insertCombOrder;
		result |= USE_changePosition;
		return result;
	}

	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);

	//virtual bool insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction* pTransaction);

	virtual bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);

	virtual bool changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction);

private:
	///判断数量是否合理
	///@param	volume	要判断的数量
	///@param	pOrder	对应的报单
	///@return	true表示合理，false表示不合理
	//bool validVolume(CVolumeType volume, COrder *pOrder);
};

/////////////////////////////////////////////////////////////////////////
///base15:CRightCheckWedge是进行交易权限检查的规则
/////////////////////////////////////////////////////////////////////////
class CRightCheckWedge : public CWedge
{
public:
	///构造方法，创建会员交易权限检查交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	CRightCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "RightCheck";
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_changePosition;
		return result;
	}

	virtual bool changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction);

private:
	virtual CTradingRightType findTradingRight(CReadOnlyShortIndexIDType &AccountTradingRight, CReadOnlyShortIndexIDType &MemberTradingRight,
		CReadOnlyShortIndexIDType& InstrumentTradingRight, CReadOnlyDirectionType Direction);
};

/////////////////////////////////////////////////////////////////////////
///base16:CPersonOpenCheckWedge是进行自然人开仓检查的规则
/////////////////////////////////////////////////////////////////////////
class CPersonOpenCheckWedge : public CWedge
{
public:
	CPersonOpenCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine, bool backendControlProperty)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_backendControlProperty = backendControlProperty;
		m_sName = "CPersonOpenCheck";
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_changePosition;
		return result;
	}

	virtual bool changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction);

private:
	bool m_backendControlProperty;
};

/////////////////////////////////////////////////////////////////////////
///base18:CUserRightWedge是进行用于权限检查的交易规则
/////////////////////////////////////////////////////////////////////////
class CUserRightWedge : public CWedge
{
public:
	///构造方法，创建进行用于权限检查的交易规
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	CUserRightWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "UserRight";
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		return result;
	}
	virtual bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);
	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);
};


#endif
