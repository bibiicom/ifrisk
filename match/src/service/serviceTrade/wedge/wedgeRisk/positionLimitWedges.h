#ifndef POSITIONLIMIT_H
#define POSITIONLIMIT_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"
//#include "positionLimitAlgorithms.h"
#include "positionControlers.h"

/////////////////////////////////////////////////////////////////////////
///pd01:CPositionCheckWedge是客户持仓检查的交易规则
/////////////////////////////////////////////////////////////////////////
class CPositionCheckWedge : public CWedge
{
public:
	///构造方法，创建客户持仓检查交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	///@param	pAlgo	使用的客户限仓算法，由本对象负责释放
	///@param	pControler	使用的客户持仓控制器（综合持仓或者净持仓）,由本对象负责释放
	///@param	useYdPosition	是否使用昨仓位
	CPositionCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	///析构方法
	~CPositionCheckWedge(void);

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		result |= USE_insertSingleTrade;
		return result;
	}

	virtual bool insertOrder(CWriteableOrder *pOrder,CTransaction* pTransaction);
	virtual bool orderAction(CWriteableOrderAction *pOrderAction,CTransaction *pTransaction);
	virtual bool insertSingleTrade(CWriteableTrade *pTrade, CWriteableOrder *pOrder);

protected:

	virtual bool enoughPosition(CInstrument *pInstrument, CPosition *pOldPosition, CWriteablePosition *pNewPosition, COrder *pOrder);
	IPositionTypeControler* getControler(CInstrument *pInstrument, COrder *pOrder);

	CGrossPositionTypeControler *m_grossControler;
	CNetPositionTypeControler *m_netControler;
	CExchangePositionTypeControler *m_exchangeControler;
	//IPositionLimitAlgorithms *m_algo;
	CWriteableOrder theOrder;
	CWriteablePosition thePosition;
};

/////////////////////////////////////////////////////////////////////////
///pd03:CMemberPositionCheckWedge是会员持仓检查的交易规则
/////////////////////////////////////////////////////////////////////////
class CMemberPositionCheckWedge : public CWedge
{
public:
	CMemberPositionCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	~CMemberPositionCheckWedge(void);

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_changePosition;
		return result;
	}

	virtual bool changePosition(CPositionChangeList *pChangeList,CTransaction *pTransaction);

protected:

	CMemberPosition* setMemberPosition(CAccountIDType AccountID, CPositionChangeList *pChangeList, CTransaction *pTransaction);

	///获取对应的持仓记录
	///@param	pInstrument	指定的合约
	///@param	particiapntID	指定的会员号
	///@param	tradingRole	指定的交易角色
	///@param	posiDirection	持仓方向
	///@param	PositionFlag	投保标志
	///@param	pMemberPosition	返回的持仓记录,NULL表示不需要返回
	///@return	得到的客户持仓记录，如果找不到，则返回NULL
	virtual CMemberPosition *findMemberPosition(CInstrument *pInstrument, CReadOnlyLongIndexIDType &MemberID, CPosiDirectionType posiDirection, CWriteableMemberPosition *pMemberPosition = NULL);

	///获取指定会员持仓的总持仓量，包括冻结
	///@param	pMemberPosition	指定的会员持仓
	///@param	posiDirection	持仓方向
	///@return	总持仓量
	virtual CVolumeType getTotalMemberPosition(CMemberPosition *pMemberPosition, CPosiDirectionType posiDirection);

	///计算会员限仓的算法
	//IMemberPositionLimitAlgorithms *m_algo;
	CWriteableMemberPosition theMemberPosition;
};

class CPositionOrderWedge1 : public CWedge
{
public:
	CPositionOrderWedge1(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	~CPositionOrderWedge1();

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		return result;
	}
	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);
	virtual bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);
	CWriteablePositionOrder thePositionOrder;
};

class CPositionOrderWedge : public CWedge
{
public:
	CPositionOrderWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	~CPositionOrderWedge();

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_changePosition;
		return result;
	}
	virtual bool changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction);
	CWriteablePositionOrder thePositionOrder;
};

class CPositionWedge : public CWedge
{
public:
	CPositionWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	~CPositionWedge();

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_changePosition;
		return result;
	}
	virtual bool changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction);
};

#endif
