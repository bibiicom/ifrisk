#include "positionControlers.h"
#include "monitorIndex.h"

void CGrossPositionTypeControler::initControler(CWriteableOrder *pOrder, CWriteablePosition *pPosition)
{
	changeList.clear();
	changeList.pInstrument = pOrder->getInstrument();
	changeList.direction = pOrder->Direction;
	changeList.pOldPosition = pOrder->getPosition();
	changeList.pNewPosition = pPosition;
}

//CPosition *CGrossPositionTypeControler::findPosition(CWriteableOrder *pOrder, CWriteablePosition *pPosition, CPosition *pPossiblePosition)
//{
//	changeList.clear();
//
//	CPosition *pOriginalPosition = NULL;
//
//	if (pOrder->OffsetFlag[0] == OF_ClosePrevious)
//		changeList.isToday = false;
//
//	///找出对应的客户持仓
//	if (pPossiblePosition == NULL)
//		pOriginalPosition = getPosition(m_pMDB, pOrder);
//	else
//		pOriginalPosition = pPossiblePosition;
//
//	if (pOriginalPosition != NULL)
//		m_pMDB->m_PositionFactory->retrieve(pOriginalPosition, pPosition);
//	else
//		InitPosition(m_pMDB, pOrder, pPosition);
//
//	//	changeList.pMemberID = &MemberID;
//	//	changeList.pTradeUnitID = &TradeUnitID;
//	changeList.pInstrument = pOrder->getInstrument();
//	changeList.direction = pOrder->Direction;
//	changeList.pOldPosition = pOriginalPosition;
//	changeList.pNewPosition = pPosition;
//
//	return pOriginalPosition;
//}

bool CGrossPositionTypeControler::calculateNewFrozen(
	CWriteablePosition *pPosition,
	CReadOnlyDirectionType &direction,
	CReadOnlyOffsetFlagType &offsetFlag,
	CReadOnlyVolumeType &volumeChange,
	CReadOnlyPriceType &orderPrice)
{
	if (volumeChange == 0)
		return true;
	CVolumeType volumeRemain = 0;
	//由于持仓的变化 持仓、冻结 部分会对资金产生影响，平仓冻结部分只对持仓数量有影响
	//所以changeList不会对平仓部分进行计算，不需要加入到changeList

	switch (pPosition->PosiDirection)
	{
	case PD_Long:
	{	///根据开平标志，决定修改今日还是昨日的冻结量
		///这里将平仓与平今不予区分，因为我们不会同时使用这两个标记
		if (offsetFlag == OF_Open)
		{
			pPosition->LongFrozen += volumeChange;
			pPosition->LongFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
			changeList.m_LongOpenVolume = volumeChange;
			changeList.add(PD_Long, IS_FROZEN, volumeChange, true);
		}
		else
		{
			pPosition->ShortFrozen += volumeChange;
			pPosition->ShortFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
			volumeRemain = pPosition->Position - pPosition->ShortFrozen;
			changeList.add(PD_Short, IS_FROZEN, volumeChange, false);
		}
		break;
	}
	case PD_Short:
	{	///根据开平标志，决定修改今日还是昨日的冻结量
		///这里将平仓与平今不予区分，因为我们不会同时使用这两个标记
		if (offsetFlag == OF_Open)
		{
			pPosition->ShortFrozen += volumeChange;
			pPosition->ShortFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
			//pPosition->ShortFrozen.toLowerTick()
			changeList.m_ShortOpenVolume = volumeChange;
			changeList.add(PD_Short, IS_FROZEN, volumeChange, true);
		}
		else
		{
			pPosition->LongFrozen += volumeChange;
			pPosition->LongFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
			volumeRemain = pPosition->Position - pPosition->LongFrozen;
			changeList.add(PD_Long, IS_FROZEN, volumeChange, false);
		}
		break;
	}
	}
	if (volumeRemain < 0)
		return false;
	else
		return true;
}

bool CGrossPositionTypeControler::calculateNewPosition(CWriteablePosition *pPosition,
	CReadOnlyDirectionType &direction,
	CReadOnlyOffsetFlagType &offsetFlag,
	CReadOnlyVolumeType &volumeChange,
	CReadOnlyPriceType &tradePrice,
	CReadOnlyPriceType *pOrderPrice)
{
	if (volumeChange == 0)
		return true;

	CVolumeType volumeRemain = 0;
	bool isOpen = false;

	switch (pPosition->PosiDirection)
	{
	case PD_Long:
	{
		///根据开平标志，决定修改今日还是昨日的持仓量
		///这里将平仓与平今不予区分，因为我们不会同时使用这两个标记
		switch (offsetFlag)
		{
		case OF_Open:
		{
			pPosition->Position += volumeChange;
			pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
			if (pOrderPrice != NULL)
			{
				pPosition->LongFrozen -= volumeChange;
				pPosition->LongFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.add(PD_Long, IS_FROZEN, -volumeChange, true);
			}
			changeList.m_LongOpenVolume = volumeChange;
			changeList.add(pPosition->PosiDirection, IS_TRADE, volumeChange, true);
			break;
		}
		case OF_Close:
		{
			pPosition->Position -= volumeChange;
			pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
			if (pOrderPrice != NULL)
			{
				pPosition->ShortFrozen -= volumeChange;
				pPosition->ShortFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.add(PD_Short, IS_FROZEN, -volumeChange, false);
			}
			volumeRemain = pPosition->Position - pPosition->ShortFrozen;
			changeList.add(pPosition->PosiDirection, IS_TRADE, -volumeChange, false);
			break;
		}
		//case OF_ClosePrevious:
		//{
		//	pPosition->PrePosition -= volumeChange;
		//	pPosition->PrePosition.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
		//	if (pOrderPrice != NULL)
		//	{
		//		pPosition->PreShortFrozen -= volumeChange;
		//		pPosition->PreShortFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
		//		changeList.add(PD_Short, IS_FROZEN, -volumeChange, *pOrderPrice, false, pPosition->getInstrument()->IsInverse);
		//	}
		//	volumeRemain = pPosition->PrePosition - pPosition->PreShortFrozen;
		//	changeList.add(pPosition->PosiDirection, IS_TRADE, -volumeChange, tradePrice, false, pPosition->getInstrument()->IsInverse);
		//	break;
		//}
		}
		break;
	}
	case PD_Short:
	{	///根据开平标志，决定修改今日还是昨日的持仓量
		///这里将平仓与平今不予区分，因为我们不会同时使用这两个标记
		switch (offsetFlag)
		{
		case OF_Open:
		{
			pPosition->Position += volumeChange;
			pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
			if (pOrderPrice != NULL)
			{
				pPosition->ShortFrozen -= volumeChange;
				pPosition->ShortFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.add(PD_Short, IS_FROZEN, -volumeChange, true);
			}
			changeList.m_ShortOpenVolume = volumeChange;
			changeList.add(pPosition->PosiDirection, IS_TRADE, volumeChange, true);
			break;
		}
		case OF_Close:
		{
			pPosition->Position -= volumeChange;
			pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
			if (pOrderPrice != NULL)
			{
				pPosition->LongFrozen -= volumeChange;
				pPosition->LongFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.add(PD_Long, IS_FROZEN, -volumeChange, false);
			}
			volumeRemain = pPosition->Position - pPosition->LongFrozen;
			changeList.add(pPosition->PosiDirection, IS_TRADE, -volumeChange, false);
			break;
		}
		//case OF_ClosePrevious:
		//{
		//	pPosition->PrePosition -= volumeChange;
		//	pPosition->PrePosition.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
		//	if (pOrderPrice != NULL)
		//	{
		//		pPosition->PreLongFrozen -= volumeChange;
		//		pPosition->PreLongFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
		//		changeList.add(PD_Long, IS_FROZEN, -volumeChange, *pOrderPrice, false, pPosition->getInstrument()->IsInverse);
		//	}
		//	volumeRemain = pPosition->PrePosition - pPosition->PreLongFrozen;
		//	changeList.add(pPosition->PosiDirection, IS_TRADE, -volumeChange, tradePrice, false, pPosition->getInstrument()->IsInverse);
		//	break;
		//}
		}
		break;
	}
	}

	if (volumeRemain < 0)
		return false;
	else
		return true;
}

void CNetPositionTypeControler::initControler(CWriteableOrder *pOrder, CWriteablePosition *pPosition)
{
	changeList.clear();

	changeList.pInstrument = pOrder->getInstrument();
	changeList.direction = pOrder->Direction;
	changeList.isToday = true;
	changeList.pOldPosition = pOrder->getPosition();
	changeList.pNewPosition = pPosition;
}

//CPosition *CNetPositionTypeControler::findPosition(CWriteableOrder *pOrder, CWriteablePosition *pPosition, CPosition *pPossiblePosition)
//{
//	changeList.clear();
//
//	CPosiDirectionType posiDirection = PD_Net;
//	pOrder->PosiDirection = PD_Net;
//	///找出对应的客户持仓
//	CPosition *pOriginalPosition;
//	if (pPossiblePosition == NULL)
//		pOriginalPosition = pOrder->linkPosition(m_pMDB->m_PositionFactory);
//	else
//		pOriginalPosition = pPossiblePosition;
//
//	///找到原来的客户持仓了，那么就把它取出来
//	if (pOriginalPosition != NULL)
//		m_pMDB->m_PositionFactory->retrieve(pOriginalPosition, pPosition);
//	else
//		InitPosition(m_pMDB, pOrder, pPosition);
//
//	//	changeList.pMemberID = &MemberID;
//	//	changeList.pTradeUnitID = &TradeUnitID;
//	changeList.pInstrument = pOrder->getInstrument();
//	changeList.direction = pOrder->Direction;
//	changeList.isToday = true;
//	changeList.pOldPosition = pOriginalPosition;
//	changeList.pNewPosition = pPosition;
//
//	return pOriginalPosition;
//}

void CNetPositionTypeControler::makeChanges(
	CVolumeType oldPosition,
	CVolumeType oldLongFrozen,
	CVolumeType oldShortFrozen,
	CVolumeType newPosition,
	CVolumeType newLongFrozen,
	CVolumeType newShortFrozen,
	CReadOnlyPriceType *pOrderPrice,
	CReadOnlyPriceType *pTradePrice,
	CBoolType isInverse)
{
	CVolumeType oldLongPosition, oldShortPosition;
	CVolumeType newLongPosition, newShortPosition;

	if (oldPosition >= 0.0)
	{
		oldLongPosition = oldPosition;
		oldShortPosition = 0.0;
	}
	else
	{
		oldLongPosition = 0.0;
		oldShortPosition = -oldPosition;
	}

	if (newPosition >= 0.0)
	{
		newLongPosition = newPosition;
		newShortPosition = 0.0;
	}
	else
	{
		newLongPosition = 0.0;
		newShortPosition = -newPosition;
	}

	if (newLongPosition != oldLongPosition)
	{
		///多头仓位减少，说明发生了多头平仓
		if (newLongPosition > oldLongPosition)
			changeList.add(PD_Long, IS_TRADE, newLongPosition - oldLongPosition, true);
		else
			changeList.add(PD_Long, IS_TRADE, newLongPosition - oldLongPosition, false);
	}
	if (newShortPosition != oldShortPosition)
	{
		///空头仓位建少，说明发生了空头平仓
		if (newShortPosition > oldShortPosition)
			changeList.add(PD_Short, IS_TRADE, newShortPosition - oldShortPosition, true);
		else
			changeList.add(PD_Short, IS_TRADE, newShortPosition - oldShortPosition, false);
	}

	if (newLongFrozen != oldLongFrozen)
	{
		changeList.add(PD_Long, IS_FROZEN, newLongFrozen - oldLongFrozen, true);
	}

	if (newShortFrozen != oldShortFrozen)
	{
		changeList.add(PD_Short, IS_FROZEN, newShortFrozen - oldShortFrozen, true);
	}
}

bool CNetPositionTypeControler::calculateNewFrozen(
	CWriteablePosition *pPosition,
	CReadOnlyDirectionType &direction,
	CReadOnlyOffsetFlagType &offsetFlag,
	CReadOnlyVolumeType &volumeChange,
	CReadOnlyPriceType &orderPrice)
{
	//只减仓操作
	if (offsetFlag != OF_Open) {
		if (pPosition->ClosePosition.isNull())
			pPosition->ClosePosition = 0;
		if (pPosition->Position > 0)
		{
			//说明已经没有仓位了，仓位反向了
			if (direction != D_Sell)
				return false;
			if (pPosition->Position - pPosition->ShortFrozen < volumeChange)
				return false;
		}
		else
		{
			if (direction != D_Buy)
				return false;
			if (abs(pPosition->Position) - pPosition->LongFrozen < volumeChange)
				return false;
		}
		pPosition->ClosePosition += volumeChange;
	}
	else
	{
		//存在只减仓订单
		if (pPosition->ClosePosition > 0)
		{
			if (pPosition->Position > 0)
			{
				if (direction != D_Buy)
				{
					if (pPosition->Position < pPosition->ShortFrozen + volumeChange)
						return false;
				}
			}
			else
			{
				if (direction != D_Sell)
				{
					if (abs(pPosition->Position) < pPosition->LongFrozen + volumeChange)
						return false;
				}
			}
		}
	}
	if (direction == D_Buy)
	{
		makeChanges(pPosition->Position,
			pPosition->LongFrozen,
			pPosition->ShortFrozen,
			pPosition->Position,
			pPosition->LongFrozen + volumeChange,
			pPosition->ShortFrozen,
			&orderPrice,
			NULL,
			pPosition->getInstrument()->IsInverse);
		pPosition->LongFrozen += volumeChange;
		pPosition->LongFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
		if (pPosition->Position >= 0)
			changeList.m_LongOpenVolume = volumeChange;
		else
			changeList.m_LongOpenVolume = pPosition->Position + pPosition->LongFrozen;
	}
	else
	{
		makeChanges(pPosition->Position,
			pPosition->LongFrozen,
			pPosition->ShortFrozen,
			pPosition->Position,
			pPosition->LongFrozen,
			pPosition->ShortFrozen + volumeChange,
			&orderPrice,
			NULL,
			pPosition->getInstrument()->IsInverse);
		pPosition->ShortFrozen += volumeChange;
		pPosition->ShortFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
		if (pPosition->Position <= 0)
			changeList.m_ShortOpenVolume = volumeChange;
		else
			changeList.m_ShortOpenVolume = pPosition->ShortFrozen - pPosition->Position;
	}
	return true;
}

bool CNetPositionTypeControler::calculateNewPosition(CWriteablePosition *pPosition,
	CReadOnlyDirectionType &direction,
	CReadOnlyOffsetFlagType	&offsetFlag,
	CReadOnlyVolumeType &volumeChange,
	CReadOnlyPriceType &tradePrice,
	CReadOnlyPriceType *pOrderPrice)
{
	if (pOrderPrice == NULL)
	{
		///是插入OTC报单的情况
		if (direction == D_Buy)
		{
			makeChanges(pPosition->Position,
				pPosition->LongFrozen,
				pPosition->ShortFrozen,
				pPosition->Position + volumeChange,
				pPosition->LongFrozen,
				pPosition->ShortFrozen,
				pOrderPrice,
				&tradePrice,
				pPosition->getInstrument()->IsInverse);
			if (pPosition->Position >= 0)
			{
				pPosition->Position += volumeChange;
				pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.m_LongOpenVolume = volumeChange;
			}
			else
			{
				pPosition->Position += volumeChange;
				pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.m_LongOpenVolume = pPosition->Position;
			}
		}
		else
		{
			makeChanges(pPosition->Position,
				pPosition->LongFrozen,
				pPosition->ShortFrozen,
				pPosition->Position - volumeChange,
				pPosition->LongFrozen,
				pPosition->ShortFrozen,
				pOrderPrice,
				&tradePrice,
				pPosition->getInstrument()->IsInverse);
			if (pPosition->Position <= 0)
			{
				pPosition->Position -= volumeChange;
				pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.m_ShortOpenVolume = volumeChange;
			}
			else
			{
				pPosition->Position -= volumeChange;
				pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.m_ShortOpenVolume = -pPosition->Position;
			}
		}
	}
	else
	{
		///是报单成交的情况
		if (direction == D_Buy)
		{
			makeChanges(pPosition->Position,
				pPosition->LongFrozen,
				pPosition->ShortFrozen,
				pPosition->Position + volumeChange,
				pPosition->LongFrozen - volumeChange,
				pPosition->ShortFrozen,
				pOrderPrice,
				&tradePrice,
				pPosition->getInstrument()->IsInverse);
			pPosition->LongFrozen -= volumeChange;
			if (pPosition->Position >= 0)
			{
				pPosition->Position += volumeChange;
				pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.m_LongOpenVolume = volumeChange;
			}
			else
			{
				pPosition->Position += volumeChange;
				pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.m_LongOpenVolume = pPosition->Position;
			}
		}
		else
		{
			makeChanges(pPosition->Position,
				pPosition->LongFrozen,
				pPosition->ShortFrozen,
				pPosition->Position - volumeChange,
				pPosition->LongFrozen,
				pPosition->ShortFrozen - volumeChange,
				pOrderPrice,
				&tradePrice,
				pPosition->getInstrument()->IsInverse);
			pPosition->ShortFrozen -= volumeChange;
			if (pPosition->Position <= 0)
			{
				pPosition->Position -= volumeChange;
				pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.m_ShortOpenVolume = volumeChange;
			}
			else
			{
				pPosition->Position -= volumeChange;
				pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				changeList.m_ShortOpenVolume = -pPosition->Position;
			}
		}
	}
	return true;
}

bool CExchangePositionTypeControler::calculateNewFrozen(
	CWriteablePosition *pPosition,
	CReadOnlyDirectionType &direction,
	CReadOnlyOffsetFlagType &offsetFlag,
	CReadOnlyVolumeType &volumeChange,
	CReadOnlyPriceType &orderPrice)
{
	if (volumeChange == 0)
		return true;

	if (direction == D_Buy)
	{
		pPosition->LongFrozen += volumeChange;
		pPosition->LongFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
		changeList.m_LongOpenVolume = volumeChange;
	}
	else
	{
		pPosition->ShortFrozen += volumeChange;
		pPosition->ShortFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
		changeList.m_ShortOpenVolume = volumeChange;
	}

	changeList.add(PD_Long, IS_FROZEN, volumeChange, true);
	return true;
}

bool CExchangePositionTypeControler::calculateNewPosition(
	CWriteablePosition *pPosition,
	CReadOnlyDirectionType &direction,
	CReadOnlyOffsetFlagType &offsetFlag,
	CReadOnlyVolumeType &volumeChange,
	CReadOnlyPriceType &tradePrice,
	CReadOnlyPriceType *pOrderPrice)
{
	if (volumeChange == 0)
		return true;

	if (pOrderPrice != NULL)
	{
		if (direction == D_Buy)
		{
			pPosition->LongFrozen -= volumeChange;
			pPosition->LongFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
		}			
		else
		{
			pPosition->ShortFrozen -= volumeChange;
			pPosition->ShortFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
		}			
		changeList.add(PD_Long, IS_FROZEN, -volumeChange, true);
	}

	if (direction == D_Buy)
		pPosition->Position += volumeChange;
	else
		pPosition->Position -= volumeChange;

	if (pPosition->Position < 0.0)
		pPosition->Position = 0.0;
	else
		pPosition->Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
	changeList.add(PD_Long, IS_TRADE, volumeChange, true);

	//if (pPosition->Position <= 0.0)
	//	pPosition->Position = 0.0;
	return true;
}

