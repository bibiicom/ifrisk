#include "accountAlgorithms.h"
#include "monitorIndex.h"
#include "tradingTools.h"

void CBaseDepositAlgorithms::calculateMarginChangeEach(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (pPositionChange->positionChangeType != IS_TRADE)
		return;

	//如果使用动态保证金，需要重新全部计算，每个增加的保证金就不需要了
	if (pList->pInstrument->MarginPriceType == MPT_MarkedPrice || pList->pInstrument->MarginPriceType == MPT_LastPrice)
		return;


	if (pPositionChange->volumeChange > 0)
	{	//开仓

		if (pList->pNewPosition->Leverage > 0.0)
		{
			double clearPrice = pList->pTrade->Price.getValue();
			if (pList->pInstrument->IsInverse)
				clearPrice = 1 / pList->pTrade->Price.getValue();
			pDepositChange->marginChange += pPositionChange->volumeChange * pList->pInstrument->VolumeMultiple * clearPrice / pList->pNewPosition->Leverage;

		}
		else if (pList->pNewPosition->Leverage < 0.0)
		{
			pDepositChange->marginChange -= pPositionChange->volumeChange * pList->pInstrument->VolumeMultiple * pList->pNewPosition->Leverage;
		}
	}
	else
	{	//平仓
		if (pList->pNewPosition->Leverage > 0.0)
		{
			double newMargin = pList->pOldPosition->UseMargin * (pList->pOldPosition->Leverage / pList->pNewPosition->Leverage);
			if (pPositionChange->volumeChange == -abs(pList->pOldPosition->Position))
				pDepositChange->marginChange -= newMargin;
			else
				pDepositChange->marginChange += pPositionChange->volumeChange * newMargin / abs(pList->pOldPosition->Position);
		}
		else if (pList->pNewPosition->Leverage < 0.0)
		{
			if (pPositionChange->volumeChange == -abs(pList->pOldPosition->Position))
			{
				if (pList->pOldPosition->Leverage != 0.0)
					pDepositChange->marginChange -= pList->pOldPosition->UseMargin * pList->pNewPosition->Leverage / pList->pOldPosition->Leverage;
			}
			else
				pDepositChange->marginChange -= pPositionChange->volumeChange * pList->pInstrument->VolumeMultiple * pList->pNewPosition->Leverage;
		}
	}
}

//void CBaseDepositAlgorithms::calculateMarginChange(CPositionChangeList *pList, CDepositChange *pDepositChange)
//{
//	if (pList->pOldPosition == NULL)
//		return;
//
//	if (pList->pNewPosition->Leverage == pList->pNewPosition->Leverage)
//		return;
//
//	if (pList->pNewPosition->Leverage > 0.0)
//	{
//		switch (pList->pInstrument->MarginPriceType)
//		{
//		case MPT_MarkedPrice:
//			pDepositChange->marginChange += abs(pList->pNewPosition->Position)*pList->pInstrument->VolumeMultiple*pList->pInstrument->getMarketData()->MarkedPrice / pList->pNewPosition->Leverage - pList->pOldPosition->UseMargin;
//			break;
//		case MPT_LastPrice:
//			pDepositChange->marginChange += abs(pList->pNewPosition->Position)*pList->pInstrument->VolumeMultiple*pList->pInstrument->getMarketData()->LastPrice / pList->pNewPosition->Leverage - pList->pOldPosition->UseMargin;
//			break;
//		default:
//			pDepositChange->marginChange += pList->pOldPosition->PositionCost / pList->pNewPosition->Leverage - pList->pOldPosition->UseMargin;
//		}
//	}
//	else if (pList->pNewPosition->Leverage < 0.0)
//		pDepositChange->marginChange += abs(pList->pOldPosition->Position*pList->pInstrument->VolumeMultiple*pList->pNewPosition->Leverage) - pList->pOldPosition->UseMargin;
//	else
//		pDepositChange->marginChange -= pList->pOldPosition->UseMargin;
//}

void CBaseDepositAlgorithms::calculateMarginFrozenChangeEach(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (!(pPositionChange->positionChangeType == IS_FROZEN))
		return;

	//冻结保证金按照比例释放的基数应该在于持仓，而非基于订单，由于净持仓的出现导致按比例释放的原则很难适用
	//需要在持仓层面进行综合考虑才能决定释放保证金的比例

	//如果方向不一样，说明是平仓单
	if (!pPositionChange->isOpen)
		return;

	if (pPositionChange->posiDirection == PD_Long)
	{
		if (pList->pOldPosition == NULL || pPositionChange->volumeChange > 0)
		{
			CMoneyType frozenMarginChange = 0.0;
			if (pList->pNewPosition->Leverage > 0.0)
			{
				double clearPrice = pList->pOrder->Price.getValue();
				if (pList->pInstrument->IsInverse)
					clearPrice = 1 / pList->pOrder->Price.getValue();
				frozenMarginChange = pPositionChange->volumeChange * pList->pInstrument->VolumeMultiple * clearPrice / pList->pNewPosition->Leverage;
			}
			else if (pList->pNewPosition->Leverage < 0.0)
			{
				frozenMarginChange = -pPositionChange->volumeChange * pList->pInstrument->VolumeMultiple * pList->pNewPosition->Leverage;
			}

			pDepositChange->longFrozenChange += frozenMarginChange;
			//这里需要归零处理，防止改单的时候，订单重新计算保证金的时候，导致问题
			//if(pList->pOrder->Volume == pPositionChange->volumeChange)
			//	pList->pOrder->FrozenMargin = frozenMarginChange;
			//else
			//	pList->pOrder->FrozenMargin += frozenMarginChange;
		}
	}
	else
	{
		if (pList->pOldPosition == NULL || pPositionChange->volumeChange > 0)
		{
			CMoneyType frozenMarginChange = 0.0;
			if (pList->pNewPosition->Leverage > 0.0)
			{
				double clearPrice = pList->pOrder->Price.getValue();
				if (pList->pInstrument->IsInverse)
					clearPrice = 1 / pList->pOrder->Price.getValue();
				frozenMarginChange = pPositionChange->volumeChange * pList->pInstrument->VolumeMultiple * clearPrice / pList->pNewPosition->Leverage;
			}
			else if (pList->pNewPosition->Leverage < 0.0)
			{
				frozenMarginChange = -pPositionChange->volumeChange * pList->pInstrument->VolumeMultiple * pList->pNewPosition->Leverage;
			}

			pDepositChange->shortFrozenChange += frozenMarginChange;
			//这里需要归零处理，防止改单的时候，订单重新计算保证金的时候，导致问题
			//if (pList->pOrder->Volume == pPositionChange->volumeChange)
			//	pList->pOrder->FrozenMargin = frozenMarginChange;
			//else
			//	pList->pOrder->FrozenMargin += frozenMarginChange;
		}
	}
}

//void CBaseDepositAlgorithms::calculateMarginFrozenChange(CPositionChangeList *pList, CDepositChange *pDepositChange)
//{
//	if (pList->pOldPosition == NULL)
//		return;
//
//	if (pList->pOldPosition->Leverage == pList->pNewPosition->Leverage)
//		return;
//
//	if (pList->pNewPosition->PosiDirection == PD_Long || pList->pNewPosition->PosiDirection == PD_Net)
//	{
//		if (pList->pNewPosition->Leverage > 0.0)
//			pDepositChange->longFrozenChange += pList->pOldPosition->LongFrozenMargin*pList->pOldPosition->Leverage / pList->pNewPosition->Leverage - pList->pOldPosition->LongFrozenMargin;
//		else if (pList->pNewPosition->Leverage < 0.0)
//			pDepositChange->longFrozenChange += -pList->pOldPosition->LongFrozen*pList->pInstrument->VolumeMultiple*pList->pNewPosition->Leverage - pList->pOldPosition->LongFrozenMargin;
//		else
//			pDepositChange->longFrozenChange -= pList->pOldPosition->LongFrozenMargin;
//	}
//
//	if (pList->pNewPosition->PosiDirection == PD_Short || pList->pNewPosition->PosiDirection == PD_Net)
//	{
//		if (pList->pNewPosition->Leverage > 0.0)
//			pDepositChange->shortFrozenChange += pList->pOldPosition->ShortFrozenMargin*pList->pOldPosition->Leverage / pList->pNewPosition->Leverage - pList->pOldPosition->ShortFrozenMargin;
//		else if (pList->pNewPosition->Leverage < 0.0)
//			pDepositChange->shortFrozenChange += -pList->pOldPosition->ShortFrozen*pList->pInstrument->VolumeMultiple*pList->pNewPosition->Leverage - pList->pOldPosition->ShortFrozenMargin;
//		else
//			pDepositChange->shortFrozenChange -= pList->pOldPosition->ShortFrozenMargin;
//	}
//}

void CBaseDepositAlgorithms::calculateToLoss(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (!(pList->pInstrument->ProductClass == PC_Margin || pList->pInstrument->ProductClass == PC_Premium))
		return;

	//单向持仓是无法区分订单是开平的，所以订单冻结的情况下
	//开仓也需要计算预计标记价格的亏损
	//平仓的冻结需要计算预计盈亏
	if (pPositionChange->positionChangeType == IS_FROZEN && pPositionChange->volumeChange > 0)
	{
		CMarketData* pMarketData = pList->pInstrument->getMarketData();
		CPriceType basePrice = pMarketData->LastPrice.getValue();
		if (!pMarketData->MarkedPrice.isNull())
			basePrice = pMarketData->MarkedPrice.getValue();
		if (pList->pInstrument->IsInverse && basePrice == 0.0)
			return;

		CPriceType closePrice = pList->pOrder->Price;

		CReadOnlyRatioType& volumeMultiple = pList->pInstrument->VolumeMultiple;
		if (pPositionChange->posiDirection == PD_Long)
		{
			if (pList->pInstrument->IsInverse)
				pDepositChange->toCloseProfit += volumeMultiple * pPositionChange->volumeChange * (1 / closePrice.getValue() - 1 / basePrice.getValue());
			else
				pDepositChange->toCloseProfit += volumeMultiple * pPositionChange->volumeChange * (basePrice.getValue() - closePrice.getValue());
		}
		else
		{
			if (pList->pInstrument->IsInverse)
				pDepositChange->toCloseProfit += volumeMultiple * pPositionChange->volumeChange * (1 / basePrice.getValue() - 1 / closePrice.getValue());
			else
				pDepositChange->toCloseProfit += volumeMultiple * pPositionChange->volumeChange * (closePrice.getValue() - basePrice.getValue());
		}
	}
}

void CBaseDepositAlgorithms::calculateCloseProfitAndPositionCost(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (pList->pInstrument->ProductClass != PC_Margin)
		return;

	if (pPositionChange->positionChangeType == IS_TRADE)
	{
		CReadOnlyRatioType& volumeMultiple = pList->pInstrument->VolumeMultiple;
		if (pPositionChange->volumeChange < 0)
		{
			///计算平均的成交价格
			///这里不需要判断pOldPosition是否为NULL，因为如果发生了平仓行为，那原来的持仓记录一定存在
			CPriceType oldTradePrice = fabs((double)pList->pOldPosition->PositionCost / volumeMultiple / pList->pOldPosition->Position);
			CPriceType TotalOldTradePrice = fabs((double)pList->pOldPosition->TotalPositionCost / volumeMultiple / pList->pOldPosition->Position);

			CMoneyType closeProfit = 0.0;
			CMoneyType totalCloseProfit = 0.0;
			///平仓盈亏
			if (pPositionChange->posiDirection == PD_Long)
			{
				if (pList->pInstrument->IsInverse)
				{
					//closeProfit += volumeMultiple * pPositionChange->volumeChange*(pPositionChange->clearPrice - oldTradePrice);
					//totalCloseProfit += volumeMultiple * pPositionChange->volumeChange*(pPositionChange->clearPrice - TotalOldTradePrice);
					closeProfit += volumeMultiple * pPositionChange->volumeChange * (1 / pList->pTrade->Price.getValue() - oldTradePrice);
					totalCloseProfit += volumeMultiple * pPositionChange->volumeChange * (1 / pList->pTrade->Price.getValue() - TotalOldTradePrice);
				}
				else
				{
					//closeProfit += volumeMultiple * pPositionChange->volumeChange*(oldTradePrice - pPositionChange->clearPrice);
					//totalCloseProfit += volumeMultiple * pPositionChange->volumeChange*(TotalOldTradePrice - pPositionChange->clearPrice);
					closeProfit += volumeMultiple * pPositionChange->volumeChange * (oldTradePrice - pList->pTrade->Price.getValue());
					totalCloseProfit += volumeMultiple * pPositionChange->volumeChange * (TotalOldTradePrice - pList->pTrade->Price.getValue());
				}
			}
			else
			{
				if (pList->pInstrument->IsInverse)
				{
					closeProfit += volumeMultiple * pPositionChange->volumeChange * (oldTradePrice - 1 / pList->pTrade->Price.getValue());
					totalCloseProfit += volumeMultiple * pPositionChange->volumeChange * (TotalOldTradePrice - 1 / pList->pTrade->Price.getValue());
				}
				else
				{
					closeProfit += volumeMultiple * pPositionChange->volumeChange * (pList->pTrade->Price.getValue() - oldTradePrice);
					totalCloseProfit += volumeMultiple * pPositionChange->volumeChange * (pList->pTrade->Price.getValue() - TotalOldTradePrice);
				}
			}

			double MoneyTick = pList->pInstrument->getClearCurrency()->MoneyTick;
			closeProfit.toLowerTick(MoneyTick);
			totalCloseProfit.toLowerTick(MoneyTick);
			pDepositChange->closeProfit += closeProfit;
			pDepositChange->totalCloseProfit += totalCloseProfit;

			///平仓造成的持仓成本变化volumeMultiple
			if (pList->isToday)
				pDepositChange->positionCostChange += volumeMultiple * pPositionChange->volumeChange * oldTradePrice;
			else
				pDepositChange->ydPositionCostChange += volumeMultiple * pPositionChange->volumeChange * oldTradePrice;
			pDepositChange->totalPositionCostChange += volumeMultiple * pPositionChange->volumeChange * TotalOldTradePrice;
		}
		else
		{
			//开仓造成的持仓成本变化

			double clearPrice = pList->pTrade->Price.getValue();
			if (pList->pInstrument->IsInverse)
				clearPrice = 1 / pList->pTrade->Price.getValue();

			double thisPositionCostChange = volumeMultiple * pPositionChange->volumeChange * clearPrice;

			if (pList->isToday)
				pDepositChange->positionCostChange += thisPositionCostChange;
			else
				pDepositChange->ydPositionCostChange += thisPositionCostChange;
			pDepositChange->totalPositionCostChange += thisPositionCostChange;
		}
	}
}

void CBaseDepositAlgorithms::calculateFee(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (pPositionChange->positionChangeType == IS_FROZEN)
	{
		if (pPositionChange->volumeChange > 0)
		{
			CFee* pFee = getFee(m_pMDB, MR_Taker, pList->pInstrument, pList->pOrder->getMember());
			if (pFee == NULL)
				return;

			double clearPrice = pList->pOrder->Price.getValue();
			if (pList->pInstrument->IsInverse)
				clearPrice = 1 / pList->pOrder->Price.getValue();

			if (pPositionChange->isOpen)
			{
				pDepositChange->frozenFeeChange += pFee->OpenFeeAmount + pFee->OpenFeeRate * pPositionChange->volumeChange * clearPrice * pList->pInstrument->VolumeMultiple;
			}
			else
			{
				if (pList->isToday)
					pDepositChange->frozenFeeChange += pFee->CloseTodayFeeAmount + pFee->CloseTodayFeeRate * pPositionChange->volumeChange * clearPrice * pList->pInstrument->VolumeMultiple;
				else
					pDepositChange->frozenFeeChange += pFee->CloseFeeAmount + pFee->CloseFeeRate * pPositionChange->volumeChange * clearPrice * pList->pInstrument->VolumeMultiple;
			}
		}
	}
	else
	{
		CFee* pFee = getFee(m_pMDB, pList->pTrade->MatchRole, pList->pInstrument, pList->pOrder->getMember());
		if (pFee == NULL)
			return;
		CMoneyType feeChange = 0.0;
		double clearPrice = pList->pTrade->Price.getValue();
		if (pList->pInstrument->IsInverse)
			clearPrice = 1 / pList->pTrade->Price.getValue();

		if (pPositionChange->volumeChange > 0)
		{
			feeChange = pFee->OpenFeeAmount + pFee->OpenFeeRate * abs(pPositionChange->volumeChange) * clearPrice * pList->pInstrument->VolumeMultiple;
		}
		else
		{
			if (pList->isToday)
				feeChange = pFee->CloseTodayFeeAmount + pFee->CloseTodayFeeRate * abs(pPositionChange->volumeChange) * clearPrice * pList->pInstrument->VolumeMultiple;
			else
				feeChange = pFee->CloseFeeAmount + pFee->CloseFeeRate * abs(pPositionChange->volumeChange) * clearPrice * pList->pInstrument->VolumeMultiple;
		}
		double MoneyTick = pList->pInstrument->getClearCurrency()->MoneyTick;
		feeChange.toUpperTick(MoneyTick);
		pDepositChange->feeChange += feeChange;
	}
}

void CBaseDepositAlgorithms::calculatePremiumChange(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (pList->pInstrument->ProductClass != PC_Premium)
		return;

	if (pPositionChange->positionChangeType == IS_TRADE)
	{
		double MoneyTick = pList->pInstrument->getClearCurrency()->MoneyTick;
		CMoneyType moneyChange = pPositionChange->volumeChange * pList->pTrade->Price.getValue() * pList->pInstrument->VolumeMultiple;
		if (pList->direction == D_Buy)
		{
			moneyChange.toUpperTick(MoneyTick);
			pDepositChange->moneyChange -= moneyChange;
		}
		else
		{
			moneyChange.toLowerTick(MoneyTick);
			pDepositChange->moneyChange += moneyChange;
		}
	}
	else
	{
		///权利金冻结: 期权买平仓 不再冻结权利金
		if (pList->direction == D_Buy && pPositionChange->volumeChange > 0)
			pDepositChange->frozenMoneyChange += pPositionChange->volumeChange * pList->pTrade->Price.getValue() * pList->pInstrument->VolumeMultiple;
	}
}

void CBaseDepositAlgorithms::calculateUnFrozen(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (pPositionChange->positionChangeType != IS_FROZEN)
		return;

	if (pPositionChange->volumeChange < 0 && pList->pOldOrder != NULL)
	{
		if (pPositionChange->volumeChange == -pList->pOldOrder->VolumeRemain)
		{
			pDepositChange->frozenMoneyChange -= pList->pOldOrder->FrozenMoney;
			pDepositChange->frozenFeeChange -= pList->pOldOrder->FrozenFee;
			if (pPositionChange->posiDirection == PD_Long)
				pDepositChange->longFrozenChange -= pList->pOldOrder->FrozenMargin;
			else
				pDepositChange->shortFrozenChange -= pList->pOldOrder->FrozenMargin;
		}
		else
		{
			pDepositChange->frozenMoneyChange += pPositionChange->volumeChange * pList->pOldOrder->FrozenMoney / pList->pOldOrder->VolumeRemain;
			pDepositChange->frozenFeeChange += pPositionChange->volumeChange * pList->pOldOrder->FrozenFee / pList->pOldOrder->VolumeRemain;

			if (pPositionChange->posiDirection == PD_Long)
				pDepositChange->longFrozenChange += pPositionChange->volumeChange * pList->pOldOrder->FrozenMargin / pList->pOldOrder->VolumeRemain;
			else
				pDepositChange->shortFrozenChange += pPositionChange->volumeChange * pList->pOldOrder->FrozenMargin / pList->pOldOrder->VolumeRemain;
		}
	}
}

void CBaseDepositAlgorithms::calculateDepositDifference1(CPositionChangeList* pList, CDepositChange* pDepositChange, CTransaction* pTransaction)
{
	if (pList->pTrade == NULL)
		return;

	int i;
	for (i = 0;; i++)
	{
		CPositionChange* pPositionChange = pList->get(i);
		if (pPositionChange == NULL)
		{
			break;
		}
		//计算期货合约的平仓盈亏和持仓成本变化
		calculateCloseProfitAndPositionCost(pPositionChange, pList, pDepositChange);
		//计算期权权利金收支和冻结
		calculatePremiumChange(pPositionChange, pList, pDepositChange);
	}

	//计算成交额

	pList->pTrade->CloseProfit = pDepositChange->closeProfit;
	pList->pOrder->CloseProfit += pDepositChange->closeProfit;

	//仓位有可能从非带单仓位变为带单仓位，此时需要把Copy信息给到Trade与Order中
	if ((!pList->pNewPosition->CopyMemberID.isNull()) && (pList->pNewPosition->CopyMemberID != "0"))
	{
		pList->pTrade->CopyMemberID = pList->pNewPosition->CopyMemberID;
		if (pList->pNewPosition->CopyMemberID == pList->pNewPosition->MemberID)
		{
			pList->pTrade->CopyOrderID = pList->pOrder->OrderID;
			pList->pOrder->CopyOrderID = pList->pOrder->OrderID;
		}
		else
		{
			pList->pTrade->CopyOrderID = pList->pOrder->CopyOrderID;
		}
		pList->pOrder->CopyMemberID = pList->pNewPosition->CopyMemberID;
	}

	//这里不要加跟单者的强判断，只要持仓上有分润比例，就应该进行分润, 从盈利中得到分润额
	if ((!pList->pNewPosition->CopyProfitRate.isNull()) && (pList->pNewPosition->CopyProfitRate > 0))
	{
		pDepositChange->copyProfit = pDepositChange->closeProfit * pList->pNewPosition->CopyProfitRate;
		//只有Trade中会统计亏损的金额
		pList->pTrade->CopyProfit = pDepositChange->copyProfit;

		if (pDepositChange->copyProfit > 0)
		{
			//Order是多笔Trade的累加，如果把负的统计进来是错误的
			pList->pOrder->CopyProfit += pDepositChange->copyProfit;
		}
	}

	if (pList->pInstrument->IsInverse)
	{
		pList->pTrade->Turnover = (pList->pTrade->Volume * pList->pInstrument->VolumeMultiple) / pList->pTrade->Price;
		if (pList->pTrade->Direction == D_Buy)
			pList->pTrade->Turnover.toUpperTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
		else
			pList->pTrade->Turnover.toLowerTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
		pList->pOrder->Turnover += pList->pTrade->Turnover;
		pList->pOrder->TradePrice = (pList->pOrder->VolumeTraded * pList->pInstrument->VolumeMultiple) / pList->pOrder->Turnover;
	}
	else
	{
		pList->pTrade->Turnover = pList->pTrade->Volume * pList->pInstrument->VolumeMultiple * pList->pTrade->Price;
		if (pList->pTrade->Direction == D_Buy)
			pList->pTrade->Turnover.toUpperTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
		else
			pList->pTrade->Turnover.toLowerTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
		pList->pOrder->Turnover += pList->pTrade->Turnover;
		pList->pOrder->TradePrice = pList->pOrder->Turnover / (pList->pOrder->VolumeTraded * pList->pInstrument->VolumeMultiple);
	}
}
void CBaseDepositAlgorithms::calculateDepositDifference2(CPositionChangeList* pList, CDepositChange* pDepositChange, CTransaction* pTransaction)
{
	//按照结果计算保证金
	/*CMoneyType useMargin = 0.0;
	CMoneyType longFrozenChange = 0.0;
	CMoneyType shortFrozenChange = 0.0;*/

	///顺序处理每个持仓变化指令,净持仓开平需要分每个指令分别处理
	int i;
	for (i = 0;; i++)
	{
		CPositionChange* pPositionChange = pList->get(i);
		if (pPositionChange == NULL)
		{
			break;
		}

		//计算变化的保证金
		calculateMarginChangeEach(pPositionChange, pList, pDepositChange);
		//计算变化的冻结保证金	
		calculateMarginFrozenChangeEach(pPositionChange, pList, pDepositChange);
		//计算期货合约的平仓盈亏和持仓成本变化
		calculateCloseProfitAndPositionCost(pPositionChange, pList, pDepositChange);
		//计算期货合约的预计平仓盈亏
		calculateToLoss(pPositionChange, pList, pDepositChange);
		//计算期权权利金收支和冻结
		calculatePremiumChange(pPositionChange, pList, pDepositChange);
		//计算手续费
		calculateFee(pPositionChange, pList, pDepositChange);
		//计算冻结释放
		calculateUnFrozen(pPositionChange, pList, pDepositChange);
	}

	pList->pOrder->FrozenMargin += pDepositChange->longFrozenChange + pDepositChange->shortFrozenChange;
	pList->pOrder->FrozenFee += pDepositChange->frozenFeeChange;
	pList->pOrder->FrozenMoney += pDepositChange->frozenMoneyChange;
	//pList->pOrder->Leverage = pList->pNewPosition->Leverage;

	//计算成交额
	if (pList->pTrade != NULL)
	{
		//pList->pTrade->Fee = pDepositChange->feeChange;
		pList->pOrder->Fee += pDepositChange->feeChange;
		pList->pTrade->CloseProfit = pDepositChange->closeProfit;
		pList->pOrder->CloseProfit += pDepositChange->closeProfit;

		//仓位有可能从非带单仓位变为带单仓位，此时需要把Copy信息给到Trade与Order中
		if ((!pList->pNewPosition->CopyMemberID.isNull()) && (pList->pNewPosition->CopyMemberID != "0"))
		{
			pList->pTrade->CopyMemberID = pList->pNewPosition->CopyMemberID;
			if (pList->pNewPosition->CopyMemberID == pList->pNewPosition->MemberID)
			{
				pList->pTrade->CopyOrderID = pList->pOrder->OrderID;
				pList->pOrder->CopyOrderID = pList->pOrder->OrderID;
			}
			else
			{
				pList->pTrade->CopyOrderID = pList->pOrder->CopyOrderID;
			}
			pList->pOrder->CopyMemberID = pList->pNewPosition->CopyMemberID;
		}

		//这里不要加跟单者的强判断，只要持仓上有分润比例，就应该进行分润, 从盈利中得到分润额
		if ((!pList->pNewPosition->CopyProfitRate.isNull()) && (pList->pNewPosition->CopyProfitRate > 0))
		{
			pDepositChange->copyProfit = pDepositChange->closeProfit * pList->pNewPosition->CopyProfitRate;
			//只有Trade中会统计亏损的金额
			pList->pTrade->CopyProfit = pDepositChange->copyProfit;

			if (pDepositChange->copyProfit > 0)
			{
				//Order是多笔Trade的累加，如果把负的统计进来是错误的
				pList->pOrder->CopyProfit += pDepositChange->copyProfit;
			}
		}

		if (pList->pInstrument->IsInverse)
		{
			pList->pTrade->Turnover = (pList->pTrade->Volume * pList->pInstrument->VolumeMultiple) / pList->pTrade->Price;
			if (pList->pTrade->Direction == D_Buy)
				pList->pTrade->Turnover.toUpperTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
			else
				pList->pTrade->Turnover.toLowerTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
			pList->pOrder->Turnover += pList->pTrade->Turnover;
			pList->pOrder->TradePrice = (pList->pOrder->VolumeTraded * pList->pInstrument->VolumeMultiple) / pList->pOrder->Turnover;
		}
		else
		{
			pList->pTrade->Turnover = pList->pTrade->Volume * pList->pInstrument->VolumeMultiple * pList->pTrade->Price;
			if (pList->pTrade->Direction == D_Buy)
				pList->pTrade->Turnover.toUpperTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
			else
				pList->pTrade->Turnover.toLowerTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
			pList->pOrder->Turnover += pList->pTrade->Turnover;
			pList->pOrder->TradePrice = pList->pOrder->Turnover / (pList->pOrder->VolumeTraded * pList->pInstrument->VolumeMultiple);
		}
		pList->pNewPosition->PositionCost = pList->pOldPosition->PositionCost + pDepositChange->positionCostChange;
		pList->pNewPosition->UseMargin += pDepositChange->marginChange;
		double MaintMarginRatio = getMinMaintMarginRatio(pList->pNewPosition->Position, pList->pInstrument, pList->pRate, pList->pNewPosition->PosiDirection);
		pList->pNewPosition->MaintMargin = pList->pNewPosition->PositionCost * MaintMarginRatio;

		if (pList->pNewPosition->getInstrument()->IsInverse)
		{
			if (pList->pNewPosition->PositionCost == 0.0)
				pList->pNewPosition->CostPrice.clear();
			else
			{
				pList->pNewPosition->CostPrice = fabs(pList->pNewPosition->getInstrument()->VolumeMultiple * pList->pNewPosition->Position / pList->pNewPosition->PositionCost);
				if (!pList->pNewPosition->IsCrossMargin)
					pList->pNewPosition->LiquidPrice = pList->pNewPosition->getInstrument()->VolumeMultiple * fabs(pList->pNewPosition->Position)
					/ (pList->pNewPosition->PositionCost - pList->pNewPosition->UseMargin + pList->pNewPosition->MaintMargin);
			}
		}
		else
		{
			pList->pNewPosition->CostPrice = fabs(pList->pNewPosition->PositionCost / pList->pNewPosition->getInstrument()->VolumeMultiple / pList->pNewPosition->Position);
			if (!pList->pNewPosition->IsCrossMargin)
				pList->pNewPosition->LiquidPrice = (pList->pNewPosition->PositionCost - pList->pNewPosition->UseMargin + pList->pNewPosition->MaintMargin)
				/ pList->pNewPosition->getInstrument()->VolumeMultiple / fabs(pList->pNewPosition->Position);
		}
		//pList->pTrade->Leverage = pList->pNewPosition->Leverage;
	}

	////计算保证金的变化
	//calculateMarginChange(pList, pDepositChange);

	////计算冻结资金的变化
	//calculateMarginFrozenChange(pList, pDepositChange);

	//计算总的冻结保证金情况
	CMoneyType oldFrozen, newFrozen;
	if (pList->pOldPosition != NULL)
	{
		oldFrozen = pList->pOldPosition->FrozenMargin;
		switch (pList->pOldPosition->PosiDirection)
		{
		case PD_Net:
		{
			CMoneyType allLongSideMargin = pList->pOldPosition->LongFrozenMargin + pDepositChange->longFrozenChange;
			CMoneyType allShortSideMargin = pList->pOldPosition->ShortFrozenMargin + pDepositChange->shortFrozenChange;
			CMoneyType newMargin = pList->pOldPosition->UseMargin + pDepositChange->marginChange;
			if (pList->pNewPosition->Position > 0)
				allLongSideMargin += newMargin;
			else if (pList->pNewPosition->Position < 0)
				allShortSideMargin += newMargin;
			newFrozen = MAX(allLongSideMargin, allShortSideMargin) - newMargin;
			break;
		}
		case PD_Long:
		{
			newFrozen = pList->pOldPosition->LongFrozenMargin + pDepositChange->longFrozenChange;
			break;
		}
		case PD_Short:
		{
			newFrozen = pList->pOldPosition->ShortFrozenMargin + pDepositChange->shortFrozenChange;
			break;
		}
		}
	}
	else
	{
		oldFrozen = 0.0;
		newFrozen = MAX(pDepositChange->longFrozenChange, pDepositChange->shortFrozenChange);
	}
	pDepositChange->frozenMarginChange = newFrozen - oldFrozen;
	//订单的保证金变化统一，这里会出现一个订单上加冻结，另外一个订单减冻结的异象
	//在净持仓为成交订单交换开平属性时
	//在调整保证金率时
	//pList->pOrder->FrozenMargin += pDepositChange->frozenMarginChange;
}

void CExchangeDepositAlgorithms::calculateFee(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (pPositionChange->positionChangeType == IS_TRADE)
	{
		CMoneyType feeChange = 0.0;
		CFee* pFee = getFee(m_pMDB, pList->pTrade->MatchRole, pList->pInstrument, pList->pOrder->getMember());
		if (pFee == NULL)
			return;
		if (pList->direction == D_Buy)
		{
			if (pList->pTrade->DeriveSource == DS_FromOTCTrade_Private)
			{
				feeChange = pFee->OpenFeeAmount + pFee->OpenFeeRate * pPositionChange->volumeChange * pList->pTrade->Price * pList->pInstrument->VolumeMultiple;
				feeChange.toUpperTick(pList->pInstrument->getClearCurrency()->MoneyTick);
				pDepositChange->feeChange += feeChange;
			}
			else
			{
				feeChange = pFee->OpenFeeAmount + pFee->OpenFeeRate * pPositionChange->volumeChange;
				feeChange.toUpperTick(pList->pInstrument->getBaseCurrency()->MoneyTick);
				pDepositChange->feeVolumeChange += feeChange;
				//买由于会得到标的，成本需要发生变化
				pDepositChange->totalPositionCostChange -= feeChange * pList->pTrade->Price * pList->pInstrument->VolumeMultiple;
			}
		}
		else
		{
			feeChange = pFee->CloseFeeAmount + pFee->CloseFeeRate * pPositionChange->volumeChange * pList->pTrade->Price * pList->pInstrument->VolumeMultiple;
			feeChange.toUpperTick(pList->pInstrument->getClearCurrency()->MoneyTick);
			pDepositChange->feeChange += feeChange;
		}
	}
}

void CExchangeDepositAlgorithms::calculateUnFrozen(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (pPositionChange->volumeChange < 0.0 && pList->pOldOrder != NULL)
	{
		//按照比例释放
		if (pList->direction == D_Buy)
		{
			if (pPositionChange->volumeChange == -pList->pOldOrder->VolumeRemain)
			{
				pDepositChange->frozenMoneyChange -= pList->pOldOrder->FrozenMoney;
			}
			else
			{
				CMoneyType frozenChange = pPositionChange->volumeChange * pList->pOldOrder->FrozenMoney / pList->pOldOrder->VolumeRemain;
				pDepositChange->frozenMoneyChange += frozenChange;
			}
		}
		else
		{
			pDepositChange->frozenVolumeChange += pPositionChange->volumeChange;
		}
	}
}

void CExchangeDepositAlgorithms::calculateMoneyChange(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (pPositionChange->positionChangeType == IS_TRADE)
	{
		double MoneyTick = pList->pInstrument->getClearCurrency()->MoneyTick;
		CMoneyType moneyChange = pPositionChange->volumeChange * pList->pTrade->Price * pList->pInstrument->VolumeMultiple;
		if (pList->direction == D_Buy)
		{
			moneyChange.toUpperTick(MoneyTick);
			pDepositChange->moneyChange -= moneyChange;
			pDepositChange->volumeChange += pPositionChange->volumeChange;
		}
		else
		{
			moneyChange.toLowerTick(MoneyTick);
			pDepositChange->moneyChange += moneyChange;
			pDepositChange->volumeChange -= pPositionChange->volumeChange;
		}
	}
	else
	{
		if (pPositionChange->volumeChange <= 0.0)
			return;
		if (pList->direction == D_Buy)
		{
			if (pList->pOrder->OrderPriceType == OPT_ByCost)
				pDepositChange->frozenMoneyChange += pList->pOrder->Cost;
			else
				pDepositChange->frozenMoneyChange += pPositionChange->volumeChange * pList->pOrder->Price * pList->pInstrument->VolumeMultiple;
		}
		else
		{
			pDepositChange->frozenVolumeChange += pPositionChange->volumeChange;
		}
	}
}

void CExchangeDepositAlgorithms::calculateCloseProfitAndPositionCost(CPositionChange* pPositionChange, CPositionChangeList* pList, CDepositChange* pDepositChange)
{
	if (pPositionChange->positionChangeType == IS_TRADE)
	{
		if (pList->direction == D_Sell)
		{
			if (pList->pOldPosition->Position != 0)
			{
				//计算平均的成交价格
				//这里不需要判断pOldPosition是否为NULL，因为如果发生了平仓行为，那原来的持仓记录一定存在
				double TotalOldTradePrice = fabs((double)pList->pOldPosition->TotalPositionCost / pList->pInstrument->VolumeMultiple / pList->pOldPosition->Position);
				//需要考虑有Account里面的volumeChange之前没有生成持仓的情况，这些不能计算盈亏，需要手动生成持仓计算后续盈亏
				//但是没有生成盈亏，就不能计算盈亏
				double CloseProfitPosition = min(pPositionChange->volumeChange.getValue(), pList->pOldPosition->Position.getValue());
				//卖盈亏
				CMoneyType closeProfit = pList->pInstrument->VolumeMultiple.getValue() * CloseProfitPosition * (pList->pTrade->Price.getValue() - TotalOldTradePrice);
				double MoneyTick = pList->pInstrument->getClearCurrency()->MoneyTick;
				closeProfit.toLowerTick(MoneyTick);
				pDepositChange->totalCloseProfit += closeProfit;
				pDepositChange->totalPositionCostChange -= pList->pInstrument->VolumeMultiple * CloseProfitPosition * TotalOldTradePrice;
			}
		}
		else
		{
			//买造成的持仓成本变化
			CMoneyType thisPositionCostChange = pList->pInstrument->VolumeMultiple.getValue() * pPositionChange->volumeChange * pList->pTrade->Price;
			pDepositChange->totalPositionCostChange += thisPositionCostChange;
		}
	}
}

void CExchangeDepositAlgorithms::calculateDepositDifference1(CPositionChangeList* pList, CDepositChange* pDepositChange, CTransaction* pTransaction)
{
	if (pList->pTrade == NULL)
		return;

	int i;
	for (i = 0;; i++)
	{
		CPositionChange* pPositionChange = pList->get(i);
		if (pPositionChange == NULL)
			break;
		//计算收支和冻结
		calculateMoneyChange(pPositionChange, pList, pDepositChange);
		//计算持仓盈亏
		calculateCloseProfitAndPositionCost(pPositionChange, pList, pDepositChange);
	}

	pList->pTrade->Turnover = pList->pTrade->Volume * pList->pInstrument->VolumeMultiple * pList->pTrade->Price;
	if (pList->pTrade->Direction == D_Buy)
		pList->pTrade->Turnover.toUpperTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
	else
		pList->pTrade->Turnover.toLowerTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
	pList->pOrder->Turnover += pList->pTrade->Turnover;
	pList->pOrder->TradePrice = pList->pOrder->Turnover / (pList->pOrder->VolumeTraded * pList->pInstrument->VolumeMultiple);

	pList->pTrade->CloseProfit = pDepositChange->totalCloseProfit;
	pList->pOrder->CloseProfit += pDepositChange->totalCloseProfit;
}

void CExchangeDepositAlgorithms::calculateDepositDifference2(CPositionChangeList* pList, CDepositChange* pDepositChange, CTransaction* pTransaction)
{
	int i;
	for (i = 0;; i++)
	{
		CPositionChange* pPositionChange = pList->get(i);
		if (pPositionChange == NULL)
			break;
		//计算收支和冻结
		calculateMoneyChange(pPositionChange, pList, pDepositChange);
		//计算手续费
		calculateFee(pPositionChange, pList, pDepositChange);
		//计算冻结释放
		calculateUnFrozen(pPositionChange, pList, pDepositChange);
		//计算持仓盈亏
		calculateCloseProfitAndPositionCost(pPositionChange, pList, pDepositChange);
	}

	if (pList->direction == D_Buy)
		pList->pOrder->FrozenMoney += pDepositChange->frozenMoneyChange;
	else
		pList->pOrder->FrozenMoney += pDepositChange->frozenVolumeChange;
	//pList->pOrder->Leverage = pList->pNewPosition->Leverage;
	if (pList->pTrade != NULL)
	{
		if (pList->pTrade->Direction == D_Buy)
		{
			//pList->pTrade->Fee = pDepositChange->feeVolumeChange;
			pList->pOrder->Fee += pDepositChange->feeVolumeChange;
		}
		else
		{
			//pList->pTrade->Fee = pDepositChange->feeChange;
			pList->pOrder->Fee += pDepositChange->feeChange;
		}

		pList->pTrade->Turnover = pList->pTrade->Volume * pList->pInstrument->VolumeMultiple * pList->pTrade->Price;
		if (pList->pTrade->Direction == D_Buy)
			pList->pTrade->Turnover.toUpperTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
		else
			pList->pTrade->Turnover.toLowerTick(pList->pInstrument->getClearCurrency()->MoneyTick.getValue());
		pList->pOrder->Turnover += pList->pTrade->Turnover;
		pList->pOrder->TradePrice = pList->pOrder->Turnover / (pList->pOrder->VolumeTraded * pList->pInstrument->VolumeMultiple);

		pList->pTrade->CloseProfit = pDepositChange->totalCloseProfit;
		pList->pOrder->CloseProfit += pDepositChange->totalCloseProfit;
		//pList->pTrade->Leverage = pList->pNewPosition->Leverage;
	}
}
