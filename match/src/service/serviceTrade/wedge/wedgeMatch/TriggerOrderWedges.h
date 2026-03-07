#ifndef StopOrderWedgeS_H
#define StopOrderWedgeS_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"
#include "monitorIndex.h"
#include "CDate.h"

class CPositionCloseAllTrigger : public CTriggerOrderCommitTrigger
{
public:
	///构造方法，创建限价行情的报单触发器
	///@param	pMDB	使用的内存数据库
	CPositionCloseAllTrigger(CMDB *pMDB);

	///析构方法，将负责释放所占用的空间
	virtual ~CPositionCloseAllTrigger();
	virtual void commitAdd(CTriggerOrder *pTriggerOrder);
	virtual void commitUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pOldTriggerOrder);
	virtual void commitRemove(CWriteableTriggerOrder *pTriggerOrder);

protected:
	///使用的内存数据库
	CMDB *m_pMDB;
};

//class CUserTriggerOrdersTrigger : public CTriggerOrderCommitTrigger
//{
//public:
//	///构造方法
//	///@param	pMDB	使用的内存数据库
//	CUserTriggerOrdersTrigger(CMDB *pMDB);
//
//	///析构方法，将负责释放所占用的空间
//	virtual ~CUserTriggerOrdersTrigger();
//	virtual void commitAdd(CTriggerOrder *pTriggerOrder);
//	virtual void commitUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pOldTriggerOrder);
//	virtual void commitRemove(CWriteableTriggerOrder *pTriggerOrder);
//
//protected:
//	///使用的内存数据库
//	CMDB *m_pMDB;
//};

class CUserOpenOrdersTrigger : public COrderCommitTrigger
{
public:
	///构造方法
	///@param	pMDB	使用的内存数据库
	CUserOpenOrdersTrigger(CMDB *pMDB);

	///析构方法，将负责释放所占用的空间
	virtual ~CUserOpenOrdersTrigger();
	virtual void commitAdd(COrder *pOrder);
	virtual void commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder);
	//virtual void commitRemove(CWriteableOrder *pOrder);
protected:
	///使用的内存数据库
	void ChangeTriggerOrder(COrder *pOrder);
	CMDB *m_pMDB;
};

class CUserOpenPositionsTrigger : public CPositionCommitTrigger
{
public:
	///构造方法
	///@param	pMDB	使用的内存数据库
	CUserOpenPositionsTrigger(CMDB *pMDB);

	///析构方法，将负责释放所占用的空间
	virtual ~CUserOpenPositionsTrigger();
	virtual void commitAdd(CPosition *pPosition);
	virtual void commitUpdate(CPosition *pPosition, CWriteablePosition *pOldPosition);
	//virtual void commitRemove(CWriteablePosition *pPosition);

protected:
	///使用的内存数据库
	CMDB *m_pMDB;
};


//class CTriggerOrderStatusTrigger : public COrderCommitTrigger
//{
//public:
//	///构造方法
//	///@param	pMDB	使用的内存数据库
//	CTriggerOrderStatusTrigger(CMDB *pMDB);
//
//	///析构方法，将负责释放所占用的空间
//	virtual ~CTriggerOrderStatusTrigger();
//	virtual void commitAdd(COrder *pOrder);
//	virtual void commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder);
//	virtual void commitRemove(CWriteableOrder *pOrder);
//
//protected:
//	///使用的内存数据库
//	CMDB *m_pMDB;
//};

/////////////////////////////////////////////////////////////////////////
///CTriggerOrderWedge是实现止损单激活的交易规则
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderWedge : public CWedge
{
public:
	///构造方法，创建止损单激活的交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
//	CTriggerOrderWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine, int iTriggerOrderMode);
	CTriggerOrderWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	///析构方法
	~CTriggerOrderWedge(void);

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_doAfterOrderChange;
		result |= USE_changePosition;
		//result |= USE_timeSync;
		return result;
	}

	virtual bool doAfterOrderChange(COrder *pOrder);
	virtual bool changePosition(CPositionChangeList *pChangeList,CTransaction *pTransaction);
	//virtual bool timeSync(CWriteableCurrentTime *pCurrentTime,CTransaction *pTransaction);
	//bool doTriggerOrder(CTriggerOrder *pTriggerOrder, CPriceType Price, CTransaction *pTransaction);

private:
	CTradeService* m_pTradeService;
	vector<CTriggerOrderActionField> m_NextTriggerOrderActionList;
//	int m_iTriggerOrderMode;
};

#endif
