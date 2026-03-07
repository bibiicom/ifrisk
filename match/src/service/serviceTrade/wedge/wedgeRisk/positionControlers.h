#ifndef POSITIONCONTROLERS_H
#define POSITIONCONTROLERS_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"
//#include "positionLimitAlgorithms.h"

/////////////////////////////////////////////////////////////////////////
///IPositionTypeControler是客户持仓类型控制器的接口
/////////////////////////////////////////////////////////////////////////
class IPositionTypeControler
{
public:
	IPositionTypeControler(CMDB *pMDB)
	{
		m_pMDB = pMDB;
	}

	///析构方法
	virtual ~IPositionTypeControler()
	{
	}

	//virtual CPosition *findPosition(CWriteableOrder *pOrder, CWriteablePosition *pPosition, CPosition *pPossiblePosition) = 0;

	virtual void initControler(CWriteableOrder *pOrder, CWriteablePosition *pPosition) = 0;

	virtual bool calculateNewFrozen(CWriteablePosition *pPosition,
		CReadOnlyDirectionType &direction,
		CReadOnlyOffsetFlagType	&offsetFlag,
		CReadOnlyVolumeType &volumeChange,
		CReadOnlyPriceType &orderPrice) = 0;

	virtual bool calculateNewPosition(CWriteablePosition *pPosition,
		CReadOnlyDirectionType &direction,
		CReadOnlyOffsetFlagType	&offsetFlag,
		CReadOnlyVolumeType &volumeChange,
		CReadOnlyPriceType &tradePrice,
		CReadOnlyPriceType *pOrderPrice) = 0;

	virtual CPositionChangeList *getPositionChangeList()
	{
		return &changeList;
	}

protected:
	CMDB *m_pMDB;
	CPositionChangeList changeList;
};

/////////////////////////////////////////////////////////////////////////
///CGrossPositionTypeControler是客户综合持仓类型的控制器，定义了客户
///综合持仓的若干基本方法，供交易规则叫用
/////////////////////////////////////////////////////////////////////////
class CGrossPositionTypeControler : public IPositionTypeControler
{
public:
	///构造函数
	///@param	pMDB	指定的内存数据库
	///@param	totalAllTradeUnit	在计算总持仓时，是否需要累加所有会员中的此客户
	CGrossPositionTypeControler(CMDB *pMDB) :IPositionTypeControler(pMDB)
	{
	}

	virtual void initControler(CWriteableOrder *pOrder, CWriteablePosition *pPosition);

	//virtual CPosition *findPosition(CWriteableOrder *pOrder, CWriteablePosition *pPosition, CPosition *pPossiblePosition);

	virtual bool calculateNewFrozen(CWriteablePosition *pPosition,
		CReadOnlyDirectionType &direction,
		CReadOnlyOffsetFlagType	&offsetFlag,
		CReadOnlyVolumeType &volumeChange,
		CReadOnlyPriceType &orderPrice);

	virtual bool calculateNewPosition(CWriteablePosition *pPosition,
		CReadOnlyDirectionType &direction,
		CReadOnlyOffsetFlagType	&offsetFlag,
		CReadOnlyVolumeType &volumeChange,
		CReadOnlyPriceType &tradePrice,
		CReadOnlyPriceType *pOrderPrice);

};

/////////////////////////////////////////////////////////////////////////
///CNetPositionTypeControler是客户净持仓类型的控制器，定义了客户
///净持仓的若干基本方法，供交易规则叫用
/////////////////////////////////////////////////////////////////////////
class CNetPositionTypeControler : public IPositionTypeControler
{
public:
	///构造函数
	///@param	pMDB	指定的内存数据库
	///@param	totalAllTradeUnit	在计算总持仓时，是否需要累加所有会员中的此客户
	CNetPositionTypeControler(CMDB *pMDB) :IPositionTypeControler(pMDB)
	{
	}

	virtual void initControler(CWriteableOrder *pOrder, CWriteablePosition *pPosition);

	//virtual CPosition *findPosition(CWriteableOrder *pOrder, CWriteablePosition *pPosition, CPosition *pPossiblePosition);

	virtual bool calculateNewFrozen(CWriteablePosition *pPosition,
		CReadOnlyDirectionType &direction,
		CReadOnlyOffsetFlagType	&offsetFlag,
		CReadOnlyVolumeType &volumeChange,
		CReadOnlyPriceType &orderPrice);

	virtual bool calculateNewPosition(CWriteablePosition *pPosition,
		CReadOnlyDirectionType &direction,
		CReadOnlyOffsetFlagType	&offsetFlag,
		CReadOnlyVolumeType &volumeChange,
		CReadOnlyPriceType &tradePrice,
		CReadOnlyPriceType *pOrderPrice);

protected:
	void makeChanges(CVolumeType oldPosition,
		CVolumeType oldLongFrozen,
		CVolumeType oldShortFrozen,
		CVolumeType newPosition,
		CVolumeType newLongFrozen,
		CVolumeType newShortFrozen,
		CReadOnlyPriceType *pOrderPrice,
		CReadOnlyPriceType *pTradePrice,
		CBoolType isInverse);
};

class CExchangePositionTypeControler : public IPositionTypeControler
{
public:
	CExchangePositionTypeControler(CMDB *pMDB) :IPositionTypeControler(pMDB)
	{
	}

	virtual void initControler(CWriteableOrder *pOrder, CWriteablePosition *pPosition)
	{
		changeList.clear();
		changeList.pInstrument = pOrder->getInstrument();
		changeList.direction = pOrder->Direction;
		changeList.pOldPosition = pOrder->getPosition();
		changeList.pNewPosition = pPosition;
	}

	//virtual CPosition *findPosition(CWriteableOrder *pOrder, CWriteablePosition *pPosition, CPosition *pPossiblePosition)
	//{
	//	changeList.clear();
	//	changeList.pInstrument = pOrder->getInstrument();
	//	changeList.direction = pOrder->Direction;

	//	if (pPossiblePosition == NULL)
	//		pPossiblePosition = pOrder->linkPosition(m_pMDB->m_PositionFactory);

	//	if (pPossiblePosition != NULL)
	//		m_pMDB->m_PositionFactory->retrieve(pPossiblePosition, pPosition);
	//	else
	//		InitPosition(m_pMDB, pOrder->getInstrument(), pPosition, pOrder->MemberID, pOrder->TradeUnitID.getValue(), pOrder->AccountID, pOrder->PosiDirection);
	//	changeList.pOldPosition = pPossiblePosition;
	//	changeList.pNewPosition = pPosition;
	//	return pPossiblePosition;
	//}

	virtual bool calculateNewFrozen(CWriteablePosition *pPosition,
		CReadOnlyDirectionType &direction,
		CReadOnlyOffsetFlagType	&offsetFlag,
		CReadOnlyVolumeType &volumeChange,
		CReadOnlyPriceType &orderPrice);

	virtual bool calculateNewPosition(CWriteablePosition *pPosition,
		CReadOnlyDirectionType &direction,
		CReadOnlyOffsetFlagType	&offsetFlag,
		CReadOnlyVolumeType &volumeChange,
		CReadOnlyPriceType &tradePrice,
		CReadOnlyPriceType *pOrderPrice);
};

#endif
