#include "priceControlWedges.h"
#include "monitorIndex.h"

bool CPriceCheckWedge::insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
{
	switch (pOrder->DeriveSource)
	{
	case DS_FromOTCTrade_Liquidation:
	case DS_FromOTCTrade_ADL:
	case DS_FromOTCTrade_Private:
	{
		if (pOrder->Price.isNull())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "PriceIsNull");
		return true;
	}
	}

	CInstrument* pInstrument = pOrder->getInstrument();
	if (pInstrument == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);

	CMarketData *pMarketData = pInstrument->getMarketData();
	if (pMarketData == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);

	pOrder->LastPriceByInsert = pMarketData->LastPrice;

	double nMaxToTradeVolume = 0.0;
	if (pOrder->OrderPriceType == OPT_LimitPrice)
	{
		if (pOrder->Price.isNull())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PriceIsNull");
		//只做maker的时候，判断价格是否符合要求
		if (pOrder->OrderType == OT_PostOnly)
		{
			if (pOrder->Direction == D_Buy)
			{
				CMarketOrder *pSellMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(pOrder->ExchangeID, pOrder->InstrumentID);
				if (pSellMarketOrder != NULL)
				{
					if (pOrder->Price >= pSellMarketOrder->Price)
						return m_pErrorEngine->reportError(ERROR_PRICE_OUT_OF_ASKPRICE1);
				}
			}
			else
			{
				CMarketOrder *pBuyMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(pOrder->ExchangeID, pOrder->InstrumentID);
				if (pBuyMarketOrder != NULL)
				{
					if (pOrder->Price <= pBuyMarketOrder->Price)
						return m_pErrorEngine->reportError(ERROR_PRICE_OUT_OF_BIDPRICE1);
				}
			}
		}
		//if (pOrder->DeriveSource != DS_FromOTCTrade_Liquidation)
		//{
		//	if (!pMarketData->UpperLimitPrice.isNull() && pOrder->Direction == D_Buy && pOrder->Price > pMarketData->UpperLimitPrice)
		//		return m_pErrorEngine->reportError(ERROR_PriceOutOfUpperLimit, "OrderPrice[%.4lf]>UpperLimitPrice[%.4lf]", pOrder->Price.getValue(), pMarketData->UpperLimitPrice.getValue());
		//	if (!pMarketData->LowerLimitPrice.isNull() && pOrder->Direction == D_Sell && pOrder->Price < pMarketData->LowerLimitPrice)
		//		return m_pErrorEngine->reportError(ERROR_PriceOutOfLowerLimit, "OrderPrice[%.4lf]<LowerLimitPrice[%.4lf]", pOrder->Price.getValue(), pMarketData->LowerLimitPrice.getValue());
		//}

		//if (!(pOrder->Price.isIntTick(pInstrument->PriceTick.getValue())))
		//	return m_pErrorEngine->reportError(ERROR_PRICE_NOT_ON_TICK, "Price[%.4lf]:PriceTick[%.4lf]", pOrder->Price.getValue(), pInstrument->PriceTick.getValue());
	}
	else
	{
		int nLevel = 0;
		CPriceType LimitPrice = pOrder->Price;
		switch (pOrder->OrderPriceType)
		{
		case OPT_AnyPrice:
		{
			//市价单的价格需要在计算资金的时候得出
			//m_pErrorEngine->reportError(ERROR_BAD_FIELD);
			if (pOrder->Direction == D_Buy)
				pOrder->Price = pMarketData->UpperLimitPrice;
			else
				pOrder->Price = pMarketData->LowerLimitPrice;
			if (pOrder->Price.isNull())
				return m_pErrorEngine->reportError(ERROR_NO_LIMITPRICE);
			break;
		}
		case OPT_ByCost:
		{
			//为了防止在资金不足的情况下，由于没有提前检测资金，造成订单簿恶意深度攻击，这里就可以检查资金
			CAccount* pAccount = NULL;
			pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pOrder->MemberID, pOrder->AccountID, pOrder->SettlementGroup, pOrder->ClearCurrency);
			if (pAccount == NULL)
				return m_pErrorEngine->reportError(ERROR_AccountNotFound, "[%s][%s][%s]", pOrder->AccountID.getValue(), pOrder->ClearCurrency.getValue(), pOrder->SettlementGroup.getValue());

			if (pAccount->Available < pOrder->Cost.getValue())
				return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "%s,Cost[%s]", pAccount->Available.getString().c_str(), pOrder->Cost.getString().c_str());

			//如果已经确定了价格是多少，那就不需要到订单簿去找了
			if (pOrder->Price.isNull())
				nLevel = INT_32_MAX;
			break;
		}
		case OPT_BestPrice:
		{
			nLevel = 1;
			break;
		}
		case OPT_FiveLevelPrice:
		{
			nLevel = 5;
			break;
		}
		case OPT_TenLevelPrice:
		{
			nLevel = 10;
			break;
		}
		//case OPT_TwentyLevelPrice:
		//{
		//	nLevel = 20;
		//	break;
		//}
		//case OPT_ThirtyLevelPrice:
		//{
		//	nLevel = 30;
		//	break;
		//}
		case OPT_BestMakerPrice:
		{
			//保持Marker，尽量排到第一个
			if (pOrder->Direction == D_Buy)
			{
				if (pOrder->BidPrice1ByInsert.isNull())
				{
					if (pOrder->Price.isNull())
						pOrder->Price = pMarketData->LastPrice;
					if (!pOrder->AskPrice1ByInsert.isNull())
					{
						if (pOrder->Price >= pOrder->AskPrice1ByInsert)
							pOrder->Price = pOrder->AskPrice1ByInsert - pInstrument->PriceTick;
					}
				}
				else
				{
					pOrder->Price = pOrder->BidPrice1ByInsert + pInstrument->PriceTick;
					if (!pOrder->AskPrice1ByInsert.isNull())
					{
						if (pOrder->Price >= pOrder->AskPrice1ByInsert)
							pOrder->Price = pOrder->BidPrice1ByInsert;
					}
				}
			}
			else
			{
				if (pOrder->AskPrice1ByInsert.isNull())
				{
					if (pOrder->Price.isNull())
						pOrder->Price = pMarketData->LastPrice;
					if (!pOrder->BidPrice1ByInsert.isNull())
					{
						if (pOrder->Price <= pOrder->BidPrice1ByInsert)
							pOrder->Price = pOrder->BidPrice1ByInsert + pInstrument->PriceTick;
					}
				}
				else
				{
					pOrder->Price = pOrder->AskPrice1ByInsert - pInstrument->PriceTick;
					if (!pOrder->BidPrice1ByInsert.isNull())
					{
						if (pOrder->Price <= pOrder->BidPrice1ByInsert)
							pOrder->Price = pOrder->AskPrice1ByInsert;
					}
				}
			}
			break;
		}
		case OPT_CFDPrice:
		{
			bool bGet = getCFDPrice(m_pMDB, pOrder->getMember(), pOrder, pInstrument, pMarketData);
			if (bGet)
			{
				nLevel = 0;
				break;
			}
			else
			{
				// 找不到点差组时，还原OrderPriceType
				pOrder->OrderPriceType = OPT_TenLevelPrice;
				nLevel = 10;
				break;
			}

			/*
			CMember *pMember = pOrder->getMember();
			if (pMember == NULL)
			{
				pMember = m_pMDB->m_MemberFactory->findByMemberID(pOrder->MemberID);
				if (pMember == NULL)
					return m_pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND, "[%s]", pOrder->MemberID.getValue());
				pOrder->linkMember(pMember);
			}
			if (!pMember->CFDGrade.isNull())
			{
				CTriggerOrder *pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findByOrderID(pOrder->RelatedOrderID);
				if (pTriggerOrder != NULL && !pTriggerOrder->TriggerOrderType.isNull())
				{
				CCFDRate *rate = NULL;
				CCFDRate *pRate = m_pMDB->m_CFDRateFactory->findFirstByMemberAndInstrumentIndex(pMember->CFDGrade.getString().c_str(), pOrder->ExchangeID, pOrder->InstrumentID);
				while (pRate != NULL)
				{
						if (pRate->TriggerOrderType.isNull() || pRate->TriggerOrderType == "default")
					{
						rate = pRate;
					}
						else if (pRate->TriggerOrderType.contains(pTriggerOrder->TriggerOrderType))
					{
						rate = pRate;
						break;
					}
					pRate = m_pMDB->m_CFDRateFactory->findNextByMemberAndInstrumentIndex();
				}
				if (rate != NULL)
				{
					CRatioType lastrate = (pOrder->Volume / rate->CompVolume*rate->CompRate).getValue();
					if (lastrate < rate->RateBegin) {
						lastrate = rate->RateBegin;
					}
					else if (lastrate > rate->RateEnd)
					{
						lastrate = rate->RateEnd;
					}
					if (lastrate > 0)
					{
						CPriceType dp = pOrder->TriggerPrice*lastrate;
						if (pOrder->Direction == D_Buy)
						{
							pOrder->Price = pOrder->TriggerPrice + dp;
							if (!pMarketData->UpperLimitPrice.isNull() && pOrder->Price > pMarketData->UpperLimitPrice)
							{
								pOrder->Price = pMarketData->UpperLimitPrice;
							}
						}
						else
						{
							pOrder->Price = pOrder->TriggerPrice - dp;
							if (!pMarketData->LowerLimitPrice.isNull() && pOrder->Price < pMarketData->LowerLimitPrice)
							{
								pOrder->Price = pMarketData->LowerLimitPrice;
							}
						}
						//格式化tick
						pOrder->Price.toTick(pInstrument->PriceTick.getValue());
						//PRINT_TO_STD("uid:%s,instrumetnID:%s,ratio:%s,price:%.4lf,dp:%.4lf,triprice:%.4lf", pOrder->MemberID.getValue(), pOrder->InstrumentID.getValue(), lastrate.getString().c_str(), pOrder->Price.getValue(), dp.getValue(), pOrder->TriggerPrice.getValue());
						break;
					}
				}
			}
				
			}
			//防止数据配置已经更新不一致
			nLevel = 10;
			break;*/
		}
			
		}

		if (nLevel > 0)
		{
			pOrder->Price.clear();
			if (pOrder->Direction == D_Buy)
			{
				double ByTurnoverTurnOverRemain = pOrder->Cost;//OPT_ByTurnover使用
				double ByTurnoverVolume = 0.0;
				CMarketOrder *pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(pOrder->ExchangeID, pOrder->InstrumentID);
				int nThisLevel = 1;
				while (pMarketOrder != NULL)
				{
					if (!pMarketData->UpperLimitPrice.isNull() && pMarketOrder->Price > pMarketData->UpperLimitPrice)
						break;

					pOrder->Price = pMarketOrder->Price;
					nMaxToTradeVolume += pMarketOrder->TrueVolume;
					if (pOrder->OrderPriceType == OPT_ByCost)
					{
						double ThisTurnOver = pMarketOrder->Price * pMarketOrder->TrueVolume * pOrder->getInstrument()->VolumeMultiple;
						if (ByTurnoverTurnOverRemain > ThisTurnOver)
						{
							ByTurnoverVolume += pMarketOrder->TrueVolume;
							pOrder->Volume = ByTurnoverVolume + (ByTurnoverTurnOverRemain - ThisTurnOver) / (pMarketOrder->Price * pOrder->getInstrument()->VolumeMultiple);
							//pOrder->Volume.toLowerTick(pOrder->getInstrument()->VolumeTick.getValue());
							pOrder->Volume.toLowerTick(pOrder->getInstrument()->VolumeTick.getValue(), pOrder->Volume.getCompareTick());
							ByTurnoverTurnOverRemain -= ThisTurnOver;
						}
						else
						{
							pOrder->Volume = ByTurnoverVolume + ByTurnoverTurnOverRemain / (pMarketOrder->Price*pOrder->getInstrument()->VolumeMultiple);
							pOrder->Volume.toLowerTick(pOrder->getInstrument()->VolumeTick.getValue(), pOrder->Volume.getCompareTick());
							break;
						}
					}
					if (nThisLevel >= nLevel)
						break;
					pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextBySellMBLData();
					nThisLevel++;
				}
				//如果找不到，尽量找一个第一个的位置
				if (pOrder->Price.isNull())
				{
					pOrder->Price = pMarketData->LastPrice;
					if (pOrder->OrderPriceType == OPT_ByCost)
					{
						pOrder->Volume = ByTurnoverTurnOverRemain / (pOrder->Price * pOrder->getInstrument()->VolumeMultiple);
						pOrder->Volume.toLowerTick(pOrder->getInstrument()->VolumeTick.getValue(), pOrder->Volume.getCompareTick());
					}
				}
			}
			else
			{
				double ByTurnoverTurnOverRemain = pOrder->Cost;//OPT_ByTurnover使用
				double ByTurnoverVolume = 0.0;
				CMarketOrder *pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(pOrder->ExchangeID, pOrder->InstrumentID);
				int nThisLevel = 1;
				while (pMarketOrder != NULL)
				{
					if (!pMarketData->LowerLimitPrice.isNull() && pMarketOrder->Price < pMarketData->LowerLimitPrice)
						break;

					pOrder->Price = pMarketOrder->Price;
					nMaxToTradeVolume += pMarketOrder->TrueVolume;
					if (pOrder->OrderPriceType == OPT_ByCost)
					{
						double ThisTurnOver = pMarketOrder->Price * pMarketOrder->TrueVolume * pOrder->getInstrument()->VolumeMultiple;
						if (ByTurnoverTurnOverRemain > ThisTurnOver)
						{
							ByTurnoverVolume += pMarketOrder->TrueVolume;
							pOrder->Volume = ByTurnoverVolume + (ByTurnoverTurnOverRemain - ThisTurnOver) / (pMarketOrder->Price * pOrder->getInstrument()->VolumeMultiple);
							pOrder->Volume.toLowerTick(pOrder->getInstrument()->VolumeTick.getValue());
							ByTurnoverTurnOverRemain -= ThisTurnOver;
						}
						else
						{
							pOrder->Volume = ByTurnoverVolume + ByTurnoverTurnOverRemain / (pMarketOrder->Price*pOrder->getInstrument()->VolumeMultiple);
							pOrder->Volume.toLowerTick(pOrder->getInstrument()->VolumeTick.getValue(), pOrder->Volume.getCompareTick());
							break;
						}
					}
					if (nThisLevel >= nLevel)
						break;
					pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextByBuyMBLData();
					nThisLevel++;
				}
				//如果找不到，尽量找一个第一个的位置
				if (pOrder->Price.isNull())
				{
					pOrder->Price = pMarketData->LastPrice;
					if (pOrder->OrderPriceType == OPT_ByCost)
					{
						pOrder->Volume = ByTurnoverTurnOverRemain / (pOrder->Price * pOrder->getInstrument()->VolumeMultiple);
						pOrder->Volume.toLowerTick(pOrder->getInstrument()->VolumeTick.getValue(), pOrder->Volume.getCompareTick());
					}
				}
			}
		}

		//没有价格的情况下，Price可以作为限价的最后防线
		if (!LimitPrice.isNull())
		{
			if (pOrder->Direction == D_Buy)
			{
				if (pOrder->Price > LimitPrice)
					pOrder->Price = LimitPrice;
			}
			else
			{
				if (pOrder->Price < LimitPrice)
					pOrder->Price = LimitPrice;
			}
		}
	}

	//检查反向合约价格应该大于0
	//if (pInstrument->IsInverse)
	//{
	//	if (pOrder->Price <= 0.0)
	//	{
	//		m_pErrorEngine->reportError(ERROR_PRICE_MUST_GREATER_THAN_ZERO);
	//		return false;
	//	}
	//}
	//else
	//{
	//	switch (pInstrument->ProductClass)
	//	{
	//	case PC_Options:
	//	case PC_Security:
	//	case PC_Exchange:
	//	{
	//		if (pOrder->Price <= 0.0)
	//		{
	//			m_pErrorEngine->reportError(ERROR_PRICE_MUST_GREATER_THAN_ZERO);
	//			return false;
	//		}
	//	}
	//	default:
	//	{}
	//	}
	//}

	if (pOrder->OrderType == OT_FOK)
	{
		if (pOrder->OrderPriceType == OPT_LimitPrice || pOrder->OrderPriceType == OPT_AnyPrice)
			nMaxToTradeVolume = getMaxToTradeVolume(pOrder->Price, pOrder->Direction, pOrder->ExchangeID.getValue(), pOrder->InstrumentID.getValue());

		if (pOrder->MinVolume > nMaxToTradeVolume)
			return m_pErrorEngine->reportError(ERROR_MV_ORDER_EXCEED_VOLUMN);
	}

	return true;
}

bool CPriceCheckWedge::changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction)
{
	//成交不检查
	if (pChangeList->pTrade != NULL)
		return true;
	//撤单不检查
	if (pChangeList->pOrderCancle != NULL)
		return true;

	switch (pChangeList->pOrder->DeriveSource)
	{
	case DS_FromOTCTrade_Liquidation:
	case DS_FromOTCTrade_ADL:
		return true;
	}

	CInstrument *pInstrument = pChangeList->pOrder->getInstrument();
	CMarketData *pMarketData = pInstrument->getMarketData();

	if (pChangeList->pOrder->Price.isNull())
		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "PriceIsNull");

	//这里需要优化，正向合约允许小于零的价格
	if (pChangeList->pOrder->Price <= 0.0)
		return m_pErrorEngine->reportError(ERROR_PRICE_MUST_GREATER_THAN_ZERO, pChangeList->pOrder->Price.getString().c_str());

	//不能超过一定的值，要不然tick值无效的
	if (!pChangeList->pOrder->Price.isValid(pInstrument->PriceTick.getValue()))
		return m_pErrorEngine->reportError(ERROR_PriceOutOfUpperLimit, "TickMaxValue");

	if (!(pChangeList->pOrder->Price.isTick(pInstrument->PriceTick.getValue())))
		return m_pErrorEngine->reportError(ERROR_PRICE_NOT_ON_TICK);

	//if (!(pChangeList->pOrder->Price.isIntTick(pInstrument->PriceTick.getValue())))
	//	return m_pErrorEngine->reportError(ERROR_PRICE_NOT_ON_TICK);

	if (pChangeList->pOrder->Priority == 0)
		return true;

	if (!pMarketData->UpperLimitPrice.isNull() && pChangeList->pOrder->Direction == D_Buy && pChangeList->pOrder->Price > pMarketData->UpperLimitPrice)
		return m_pErrorEngine->reportError(ERROR_PriceOutOfUpperLimit,"Price>UpperLimitPrice[%s]", pMarketData->UpperLimitPrice.getString().c_str());

	if (!pMarketData->LowerLimitPrice.isNull() && pChangeList->pOrder->Direction == D_Sell && pChangeList->pOrder->Price < pMarketData->LowerLimitPrice)
		return m_pErrorEngine->reportError(ERROR_PriceOutOfLowerLimit,"Price<LowerLimitPrice[%s]", pMarketData->LowerLimitPrice.getString().c_str());
	return true;
}

double CPriceCheckWedge::getMaxToTradeVolume(double Price, char Direction, const char* ExchangeID, const char* InstrumentID)
{
	double nMaxToTradeVolume = 0.0;
	if (Direction == D_Sell)
	{
		CMarketOrder *pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(ExchangeID, InstrumentID);
		while (pMarketOrder != NULL)
		{
			if (pMarketOrder->Price < Price)
				break;
			nMaxToTradeVolume += pMarketOrder->TrueVolume;
			pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextByBuyMBLData();
		}
	}
	else
	{
		CMarketOrder *pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(ExchangeID, InstrumentID);
		while (pMarketOrder != NULL)
		{
			if (pMarketOrder->Price > Price)
				break;
			nMaxToTradeVolume += pMarketOrder->TrueVolume;
			pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextBySellMBLData();
		}
	}
	return nMaxToTradeVolume;
}

bool CPriceCheckWedge::orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction)
{
	if (pOrderAction->ActionFlag == AF_Modify)
	{
		if (pOrderAction->Price.isNull())
			pOrderAction->Price = pOrderAction->getOrder()->Price;

		//CInstrument *pInstrument = pOrderAction->getOrder()->getInstrument();
		//CMarketData *pMarketData = pInstrument->getMarketData();

		//if (!validPrice(pOrderAction->Price))
		//	return false;

		//if (!(pOrderAction->Price.isIntTick(pInstrument->PriceTick.getValue())))
		//	return m_pErrorEngine->reportError(ERROR_PRICE_NOT_ON_TICK);

		//if (!pMarketData->UpperLimitPrice.isNull() && pOrderAction->getOrder()->Direction == D_Buy && pOrderAction->Price > pMarketData->UpperLimitPrice)
		//	return m_pErrorEngine->reportError(ERROR_PriceOutOfUpperLimit);

		//if (!pMarketData->LowerLimitPrice.isNull() && pOrderAction->getOrder()->Direction == D_Sell && pOrderAction->Price < pMarketData->LowerLimitPrice)
		//	return m_pErrorEngine->reportError(ERROR_PriceOutOfLowerLimit);
	}
	return true;
}

//bool CPriceCheckWedge::validPrice(CPriceType price)
//{
//	if (price.isNull())
//		return m_pErrorEngine->reportError(ERROR_PRICE_NOT_ON_TICK, price.getString().c_str());
//
//	if (price <= 0)
//		return m_pErrorEngine->reportError(ERROR_PRICE_MUST_GREATER_THAN_ZERO, price.getString().c_str());
//
//	return true;
//}