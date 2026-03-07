#include "marketDataWedges.h"
#include "monitorIndex.h"
#include "CopyField.h"

COrderMBLTrigger::COrderMBLTrigger(CMDB *pMDB)
{
	m_pMDB = pMDB;
}

COrderMBLTrigger::~COrderMBLTrigger(void)
{
}

void COrderMBLTrigger::commitAdd(COrder *pOrder)
{
	if (!needInOrderBook(pOrder))
		return;

	//从中间流水反演的情况下，需要找一下合约
	CInstrument* pInstrument = pOrder->getInstrument();

	if (pInstrument->TradingModel == TM_OnlyQuote)
	{
		if (pOrder->DeriveSource != DS_FromQuote)
			return;
	}

	CVolumeType volumeChange = pOrder->VolumeRemain;
	if (!pOrder->VolumeDisplay.isNull())
		volumeChange = pOrder->VolumeDisplay;

	alterMBLData(pInstrument, pOrder->Direction, pOrder->Price, volumeChange, pOrder->VolumeRemain, 1);

	//if (!pInstrument->MarketTick1.isNull() && pInstrument->MarketTick1 > 0.0)
	//{
	//	CPriceType tickPrice = pOrder->Price;
	//	if (pOrder->Direction == D_Buy)
	//		tickPrice.toLowerTick(pInstrument->MarketTick1.getValue());
	//	else
	//		tickPrice.toUpperTick(pInstrument->MarketTick1.getValue());
	//	alterTickMBLData(pInstrument, pOrder->Direction, tickPrice, pInstrument->MarketTick1, volumeChange, 1);
	//}

	//if (!pInstrument->MarketTick2.isNull() && pInstrument->MarketTick2 > 0.0)
	//{
	//	CPriceType tickPrice = pOrder->Price;
	//	if (pOrder->Direction == D_Buy)
	//		tickPrice.toLowerTick(pInstrument->MarketTick2.getValue());
	//	else
	//		tickPrice.toUpperTick(pInstrument->MarketTick2.getValue());
	//	alterTickMBLData(pInstrument, pOrder->Direction, tickPrice, pInstrument->MarketTick2, volumeChange, 1);
	//}
}

void COrderMBLTrigger::commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder)
{
	//VolumeDisplay的设计逻辑是不管订单数量怎么变，行情订单簿显示的字段都不变
	//此时如果订单发生变化，只要不是数量的变化，显示是不会变化的
	//if ((!needInOrderBook(pOldOrder)) && (!needInOrderBook(pOrder)))
	//	return;

	CInstrument* pInstrument = pOrder->getInstrument();
	if (pInstrument == NULL)
		return;

	if (pInstrument->TradingModel == TM_OnlyQuote)
	{
		if (pOrder->DeriveSource != DS_FromQuote)
			return;
	}

	///价格没有发生变化，那就修改数量即可
	if (pOrder->Price == pOldOrder->Price)
	{
		CVolumeType trueVolumeChange;
		CVolumeType volumeChange;
		CNumberType orderNumberChange;

		if (needInOrderBook(pOldOrder))
		{
			if (needInOrderBook(pOrder))
			{
				trueVolumeChange = pOrder->VolumeRemain - pOldOrder->VolumeRemain;
				if (pOrder->VolumeDisplay.isNull())
					volumeChange = pOrder->VolumeRemain - pOldOrder->VolumeRemain;
				else
					volumeChange = pOrder->VolumeDisplay - pOldOrder->VolumeDisplay;
				orderNumberChange = 0;
			}
			else
			{
				trueVolumeChange = -pOldOrder->VolumeRemain;
				if (pOrder->VolumeDisplay.isNull())
					volumeChange = -pOldOrder->VolumeRemain;
				else
					volumeChange = -pOldOrder->VolumeDisplay;
				orderNumberChange = -1;
			}
		}
		else
		{
			if (needInOrderBook(pOrder))
			{
				trueVolumeChange = pOrder->VolumeRemain;
				if (pOrder->VolumeDisplay.isNull())
					volumeChange = pOrder->VolumeRemain;
				else
					volumeChange = pOrder->VolumeDisplay;
				orderNumberChange = 1;
			}
			else
			{
				return;
			}
		}
		///调整MBL行情
		alterMBLData(pOrder->getInstrument(), pOrder->Direction, pOrder->Price, volumeChange, trueVolumeChange, orderNumberChange);

		//if (!pInstrument->MarketTick1.isNull() && pInstrument->MarketTick1 > 0.0)
		//{
		//	CPriceType tickPrice = pOrder->Price;
		//	if (pOrder->Direction == D_Buy)
		//		tickPrice.toLowerTick(pInstrument->MarketTick1.getValue());
		//	else
		//		tickPrice.toUpperTick(pInstrument->MarketTick1.getValue());
		//	alterTickMBLData(pInstrument, pOrder->Direction, tickPrice, pInstrument->MarketTick1, volumeChange, 0);
		//}
		//if (!pInstrument->MarketTick2.isNull() && pInstrument->MarketTick2 > 0.0)
		//{
		//	CPriceType tickPrice = pOrder->Price;
		//	if (pOrder->Direction == D_Buy)
		//		tickPrice.toLowerTick(pInstrument->MarketTick2.getValue());
		//	else
		//		tickPrice.toUpperTick(pInstrument->MarketTick2.getValue());
		//	alterTickMBLData(pInstrument, pOrder->Direction, tickPrice, pInstrument->MarketTick2, volumeChange, 0);
		//}
	}
	else
	{		///价格发生了变化，那就分别处理两个价位上的变化
		if (needInOrderBook(pOldOrder))
		{
			CVolumeType trueVolumeChange = -pOldOrder->VolumeRemain;
			CVolumeType volumeChange = -pOldOrder->VolumeRemain;

			if (!pOrder->VolumeDisplay.isNull())
				volumeChange = -pOldOrder->VolumeDisplay;

			alterMBLData(pInstrument, pOldOrder->Direction, pOldOrder->Price, volumeChange, trueVolumeChange, -1);

			//if (!pInstrument->MarketTick1.isNull() && pInstrument->MarketTick1 > 0.0)
			//{
			//	CPriceType tickPrice = pOrder->Price;
			//	if (pOrder->Direction == D_Buy)
			//		tickPrice.toLowerTick(pInstrument->MarketTick1.getValue());
			//	else
			//		tickPrice.toUpperTick(pInstrument->MarketTick1.getValue());
			//	alterTickMBLData(pInstrument, pOrder->Direction, tickPrice, pInstrument->MarketTick1, volumeChange, -1);
			//}
			//if (!pInstrument->MarketTick2.isNull() && pInstrument->MarketTick2 > 0.0)
			//{
			//	CPriceType tickPrice = pOrder->Price;
			//	if (pOrder->Direction == D_Buy)
			//		tickPrice.toLowerTick(pInstrument->MarketTick2.getValue());
			//	else
			//		tickPrice.toUpperTick(pInstrument->MarketTick2.getValue());
			//	alterTickMBLData(pInstrument, pOrder->Direction, tickPrice, pInstrument->MarketTick2, volumeChange, -1);
			//}
		}

		if (needInOrderBook(pOrder))
		{
			CVolumeType trueVolumeChange = pOrder->VolumeRemain;
			CVolumeType volumeChange = pOrder->VolumeRemain;
			if (!pOrder->VolumeDisplay.isNull())
				volumeChange = pOrder->VolumeDisplay;

			alterMBLData(pInstrument, pOrder->Direction, pOrder->Price, volumeChange, trueVolumeChange, 1);
			//if (!pInstrument->MarketTick1.isNull() && pInstrument->MarketTick1 > 0.0)
			//{
			//	CPriceType tickPrice = pOrder->Price;
			//	if (pOrder->Direction == D_Buy)
			//		tickPrice.toLowerTick(pInstrument->MarketTick1.getValue());
			//	else
			//		tickPrice.toUpperTick(pInstrument->MarketTick1.getValue());
			//	alterTickMBLData(pInstrument, pOrder->Direction, tickPrice, pInstrument->MarketTick1, volumeChange, 1);
			//}
			//if (!pInstrument->MarketTick2.isNull() && pInstrument->MarketTick2 > 0.0)
			//{
			//	CPriceType tickPrice = pOrder->Price;
			//	if (pOrder->Direction == D_Buy)
			//		tickPrice.toLowerTick(pInstrument->MarketTick2.getValue());
			//	else
			//		tickPrice.toUpperTick(pInstrument->MarketTick2.getValue());
			//	alterTickMBLData(pInstrument, pOrder->Direction, tickPrice, pInstrument->MarketTick2, volumeChange, 1);
			//}
		}
	}
}

void COrderMBLTrigger::commitRemove(CWriteableOrder *pOrder)
{
	//if (!needInOrderBook(pOrder))
	//	return;

	//CInstrument* pInstrument = pOrder->getInstrument();
	//if (pInstrument->TradingModel == TM_OnlyQuote)
	//{
	//	if (pOrder->DeriveSource != DS_FromQuote)
	//		return;
	//}
	//CVolumeType trueVolumeChange = -pOrder->VolumeRemain;
	//CVolumeType volumeChange = -pOrder->VolumeRemain;
	//if (!pOrder->VolumeDisplay.isNull())
	//	volumeChange = -pOrder->VolumeDisplay;

	//alterMBLData(pOrder->getInstrument(), pOrder->Direction, pOrder->Price, volumeChange, trueVolumeChange, -1);
	////if (!pInstrument->MarketTick1.isNull() && pInstrument->MarketTick1 > 0.0)
	////{
	////	CPriceType tickPrice = pOrder->Price;
	////	if (pOrder->Direction == D_Buy)
	////		tickPrice.toLowerTick(pInstrument->MarketTick1.getValue());
	////	else
	////		tickPrice.toUpperTick(pInstrument->MarketTick1.getValue());
	////	alterTickMBLData(pInstrument, pOrder->Direction, tickPrice, pInstrument->MarketTick1, volumeChange, -1);
	////}
	////if (!pInstrument->MarketTick2.isNull() && pInstrument->MarketTick2 > 0.0)
	////{
	////	CPriceType tickPrice = pOrder->Price;
	////	if (pOrder->Direction == D_Buy)
	////		tickPrice.toLowerTick(pInstrument->MarketTick2.getValue());
	////	else
	////		tickPrice.toUpperTick(pInstrument->MarketTick2.getValue());
	////	alterTickMBLData(pInstrument, pOrder->Direction, tickPrice, pInstrument->MarketTick2, volumeChange, -1);
	////}
}

void COrderMBLTrigger::alterMBLData(CInstrument *pInstrument, CReadOnlyDirectionType &direction, CReadOnlyPriceType &price, CReadOnlyVolumeType &volumeChange, CReadOnlyVolumeType &trueVolumeChange, CReadOnlyNumberType &orderNumberChange)
{
	//if (volumeChange == 0.0)
	//	return;

	//CVolumeType newVolume = volumeChange;
	//CVolumeType newTrueVolume = trueVolumeChange;
	//newVolume.toTick(pInstrument->VolumeTick.getValue());

	CMarketOrder *pMarketOrder;

	pMarketOrder = m_pMDB->m_MarketOrderFactory->findByPrice(pInstrument->ExchangeID, pInstrument->InstrumentID, price);
	if (pMarketOrder == NULL)
	{
		///原来没有此项行情，则加入此项行情
		theWriteableMarketOrder.ExchangeID = pInstrument->ExchangeID;
		theWriteableMarketOrder.InstrumentID = pInstrument->InstrumentID;
		theWriteableMarketOrder.Direction = direction;
		theWriteableMarketOrder.Price = price;
		theWriteableMarketOrder.Volume = volumeChange;
		theWriteableMarketOrder.TrueVolume = trueVolumeChange;
		theWriteableMarketOrder.Orders = orderNumberChange;
		theWriteableMarketOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_MarketOrderFactory->add(&theWriteableMarketOrder);

		//		if (pInstrument->InstrumentID == "ETH/USDT" && price == 395.0)
		//			printf("%f => 0.0,%f,%f | %d \n", price.getValue(), volumeChange, theWriteableMarketOrder.Volume.getValue(), theWriteableMarketOrder.OrderNumber.getValue());
	}
	else
	{
		//if (pInstrument->InstrumentID == "BTCUSD" && price == 592.5 &&pMarketOrder->Volume== 21)
		//if (pInstrument->InstrumentID == "ETHUSD" && price == 592.5)
		//{
		//	int a = 0;
		//}
		///原来有此项行情

		CNumberType orderNumber = pMarketOrder->Orders.getValue() + orderNumberChange.getValue();

		///计算新的数量
		//newVolume += pMarketOrder->Volume;
		//newTrueVolume += pMarketOrder->TrueVolume;
		//newVolume.toTick(pInstrument->VolumeTick.getValue());

		//if (pInstrument->InstrumentID == "ETHUSD" && price == 592.5 && newVolume == 21)
		//{
		//	int a = 0;
		//}

		///不需要此项行情，所以将其删除
		if (orderNumber <= 0)
		{
			//			if (pInstrument->InstrumentID == "ETH/USDT" && price == 395.0)
			//				printf("%f => %f,%f,0.0 | 0 \n", price.getValue(), pMarketOrder->Volume.getValue(), volumeChange.getValue());
			m_pMDB->m_MarketOrderFactory->remove(pMarketOrder);
		}
		else
		{

			if (pMarketOrder->Direction != direction)
			{
				m_pMDB->m_MarketOrderFactory->remove(pMarketOrder);
				theWriteableMarketOrder.ExchangeID = pInstrument->ExchangeID;
				theWriteableMarketOrder.InstrumentID = pInstrument->InstrumentID;
				theWriteableMarketOrder.Direction = direction;
				theWriteableMarketOrder.Price = price;
				theWriteableMarketOrder.Volume = volumeChange;
				theWriteableMarketOrder.TrueVolume = trueVolumeChange;
				theWriteableMarketOrder.Orders = orderNumberChange;
				theWriteableMarketOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
				m_pMDB->m_MarketOrderFactory->add(&theWriteableMarketOrder);
			}
			else
			{
				m_pMDB->m_MarketOrderFactory->retrieve(pMarketOrder, &theWriteableMarketOrder);
				theWriteableMarketOrder.Volume += volumeChange;
				theWriteableMarketOrder.TrueVolume += trueVolumeChange;
				theWriteableMarketOrder.Orders += orderNumberChange;
				theWriteableMarketOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
				//			if (pInstrument->InstrumentID == "ETH/USDT" && price == 395.0)
				//				printf("%f => %f,%f,%f | %d \n", price.getValue(), pMarketOrder->Volume.getValue(), volumeChange.getValue(), theWriteableMarketOrder.Volume.getValue(), theWriteableMarketOrder.OrderNumber.getValue());
				m_pMDB->m_MarketOrderFactory->update(pMarketOrder, &theWriteableMarketOrder);
			}
		}
	}

	//CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID);
	//if (pMarketData != NULL)
	//{
	//	CWriteableMarketData MarketData;
	//	m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &MarketData);
	//	MarketData.TNO = m_gBusinessSequence.getCurrentValueByDouble();
	//	m_pMDB->m_MarketDataFactory->update(pMarketData, &MarketData);
	//}
}

//void COrderMBLTrigger::alterTickMBLData(CInstrument *pInstrument, CReadOnlyDirectionType &direction, CReadOnlyPriceType &price, CReadOnlyPriceType &tick, CReadOnlyVolumeType &volumeChange, CReadOnlyNumberType &orderNumberChange)
//{
//	//if (volumeChange == 0.0)
//	//	return;
//
//	//volumeChange.toTick(pInstrument->VolumeTick.getValue());
//
//	CTickMarketOrder *pMarketOrder;
//	CWriteableTickMarketOrder theWriteableMarketOrder;
//
//	pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findByPrice(pInstrument->ExchangeID, pInstrument->InstrumentID, tick, price);
//	if (pMarketOrder == NULL)
//	{
//		///原来没有此项行情，则加入此项行情
//		theWriteableMarketOrder.ExchangeID = pInstrument->ExchangeID;
//		theWriteableMarketOrder.InstrumentID = pInstrument->InstrumentID;
//		theWriteableMarketOrder.Direction = direction;
//		theWriteableMarketOrder.Price = price;
//		theWriteableMarketOrder.Tick = tick;
//		theWriteableMarketOrder.Volume = volumeChange;
//		theWriteableMarketOrder.linkInstrument(pInstrument);
//		theWriteableMarketOrder.Orders = orderNumberChange;
//		theWriteableMarketOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
//		m_pMDB->m_TickMarketOrderFactory->add(&theWriteableMarketOrder);
//	}
//	else
//	{
//		CVolumeType newVolume = pMarketOrder->Volume + volumeChange;
//		//newVolume.toTick(pInstrument->VolumeTick.getValue());
//
//		//如果新的Volume是0，必须删除，避免内存中TickMarketData数目过大导致内存dump时间过长和内存过大
//		if (newVolume <= 0.0)
//		{
//			m_pMDB->m_TickMarketOrderFactory->remove(pMarketOrder);
//		}
//		else
//		{
//			m_pMDB->m_TickMarketOrderFactory->retrieve(pMarketOrder, &theWriteableMarketOrder);
//			theWriteableMarketOrder.Volume += volumeChange;
//			theWriteableMarketOrder.Orders += orderNumberChange;
//			theWriteableMarketOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
//			m_pMDB->m_TickMarketOrderFactory->update(pMarketOrder, &theWriteableMarketOrder);
//		}
//	}
//}

CMarketDataGenerateWedge::CMarketDataGenerateWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
	:CWedge(pTradeService, pMDB, pErrorEngine)
{
	m_sName = "MarketDataGenerate";
}

CMarketDataGenerateWedge::~CMarketDataGenerateWedge(void)
{
}

///是否达到涨停板条件，只能在收盘时间使用
///@param	pMDB	使用的内存数据库
///@param	pUniPressure	单边市情况
///@return	true表示达到涨停板，false表示没有达到
// bool isUpperPressure(CMDB *pMDB, CUniPressure *pUniPressure)
// {
// 	if (pUniPressure==NULL)
// 	{
// 		return false;
// 	}
// 	if (pUniPressure->UpperPressureStartTime.isNull())
// 	{
// 		return false;
// 	}
// 	int startTime=getSecondOfDay(pUniPressure->UpperPressureStartTime);
// 	int currentTime=getSecondOfDay(getCurrentTime(pMDB));
// 	if (currentTime-startTime>=300)
// 	{
// 		return true;
// 	}
// 	return false;
// }

///是否达到跌停板条件，只能在收盘时间使用
///@param	pMDB	使用的内存数据库
///@param	pUniPressure	单边市情况
///@return	true表示达到跌停板，false表示没有达到
// bool isLowerPressure(CMDB *pMDB, CUniPressure *pUniPressure)
// {
// 	if (pUniPressure==NULL)
// 	{
// 		return false;
// 	}
// 	if (pUniPressure->LowerPressureStartTime.isNull())
// 	{
// 		return false;
// 	}
// 	int startTime=getSecondOfDay(pUniPressure->LowerPressureStartTime);
// 	int currentTime=getSecondOfDay(getCurrentTime(pMDB));
// 	if (currentTime-startTime>=300)
// 	{
// 		return true;
// 	}
// 	return false;
// }

bool CMarketDataGenerateWedge::updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction)
{
	m_pMDB->m_MarketDataFactory->retrieve(pInstrumentStatus, &theMarketData);

	bool modified = false;

	///合约开盘需要设置一些字段的值，这段代码目前也不必要了，已经交给初始化触发器了
	if (pInstrumentStatus->InstrumentStatus == IS_BeforeTrading)
	{
		modified = true;
		theMarketData.LastPrice = theMarketData.PreClosePrice;
		if (theMarketData.Turnover.isNull())
		{
			theMarketData.Turnover = 0.0;
		}
	}
	//合约收盘需要设置的一些字段的值
	if (pInstrumentStatus->InstrumentStatus == IS_Closed)
	{
		modified = true;
		theMarketData.ClosePrice = theMarketData.LastPrice;
	}

	if (modified)
	{
		m_pMDB->m_MarketDataFactory->update(pInstrumentStatus, &theMarketData, pTransaction, 1);
	}
	return true;
}

bool CMarketDataGenerateWedge::changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction)
{
	if (pChangeList->pInstrument->TradingModel == TM_OnlyQuote)
		return true;

	CMarketData *pMarketData;

	if (pChangeList->pTrade == NULL)
		return true;

	CPriceType tradePrice = pChangeList->pTrade->Price;

	//CPriceType tradePrice;
	//if (!pChangeList->hasTrade(&tradePrice))
	//{
	//	///不是成交引起的持仓变化，不必修改行情
	//	return true;
	//}

	pMarketData = pChangeList->pInstrument->getMarketData();
	m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);

	if (needShowPrice(m_pMDB, pChangeList->pTrade))
	{
		///修改最新成交价
		theMarketData.LastPrice = tradePrice;

		///有可能需要修改开盘价
		if (theMarketData.OpenPrice.isNull())
			theMarketData.OpenPrice = tradePrice;

		///有可能改变最高价
		if ((theMarketData.HighestPrice.isNull()) || (theMarketData.HighestPrice < tradePrice))
			theMarketData.HighestPrice = tradePrice;

		///有可能修改最低价
		if ((theMarketData.LowestPrice.isNull()) || (theMarketData.LowestPrice > tradePrice))
			theMarketData.LowestPrice = tradePrice;
	}

	///计算持仓变化量和成交量
	CVolumeType positionChange = 0, tradeVolume = 0;
	int i;

	for (i = 0;; i++)
	{
		CPositionChange *pPositionChange = pChangeList->get(i);
		if (pPositionChange == NULL)
		{
			break;
		}
		if (pPositionChange->positionChangeType == IS_TRADE)
		{
			positionChange += pPositionChange->volumeChange;
			tradeVolume += fabs(pPositionChange->volumeChange);
		}
	}

	//if (needShowVolumeAmount(pChangeList->pTrade))
	{
		///修改成交量
		theMarketData.Volume += tradeVolume;
		theMarketData.Volume.toTick(pChangeList->pInstrument->VolumeTick);

		///修改成交金额
		double Turnover = 0.0;
		if (pChangeList->pInstrument->IsInverse)
			Turnover = tradeVolume * pChangeList->pInstrument->VolumeMultiple / tradePrice;
		else
			Turnover = tradeVolume * pChangeList->pInstrument->VolumeMultiple*tradePrice;

		theMarketData.Turnover += Turnover;
	}

	//if (needShowPosition(pChangeList->pTrade))
	{
		///修改总持仓量
		theMarketData.OpenInterest += positionChange;
		if (theMarketData.OpenInterest < 0.0)
			theMarketData.OpenInterest = 0.0;
		theMarketData.OpenInterest.toTick(pChangeList->pInstrument->VolumeTick);
	}

	theMarketData.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	///写回
	m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);

	/**
	在修改持仓，而非插入成交时计算行情，主要是考虑到对总持仓量的修改，插入成交
	时不能完全计算（净持仓的情况）。如果分在两个地方分别计算，那效率就会下降。
	**/

	return true;
}

bool CMarketDataGenerateWedge::timeSync(CWriteableCurrentTime *pCurrentTime)
{
	if (pCurrentTime->ZoneDate == m_CurrDate)
		return true;

	if (m_CurrDate == "")
	{
		m_CurrDate = pCurrentTime->ZoneDate;
		return true;
	}
	CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	while (pMarketData != NULL)
	{
		m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
		theMarketData.OpenPrice = pMarketData->LastPrice;
		theMarketData.HighestPrice.clear();
		theMarketData.LowestPrice.clear();
		theMarketData.ClosePrice.clear();
		theMarketData.Volume = 0.0;
		theMarketData.Turnover = 0.0;
		theMarketData.PreClosePrice = pMarketData->LastPrice;
		m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);

		pMarketData = m_pMDB->m_MarketDataFactory->getNext();
	}
	m_CurrDate = pCurrentTime->ZoneDate;
	return true;
}

//bool CMarketDataGenerateWedge::needShowVolumeAmount(CTrade *pTrade)
//{
//	if (pTrade->IsSelfTrade)
//	{
//		if (!needSyncDB(m_pMDB, pTrade->MemberID))
//			return false;
//		else
//			return true;
//	}
//	return true;
//}

//bool CMarketDataGenerateWedge::needShowPosition(CTrade *pTrade)
//{
//	if (pTrade->IsSelfTrade)
//	{
//		if (!needSyncDB(m_pMDB, pTrade->MemberID))
//			return false;
//		else
//			return true;
//	}
//	return true;
//}

CMarketOverViewTrigger::CMarketOverViewTrigger(CServiceResponser *pServiceResponser, CMDB *pMDB)
{
	m_pServiceResponser = pServiceResponser;
	m_pMDB = pMDB;
}

CMarketOverViewTrigger::~CMarketOverViewTrigger(void)
{
}

void CMarketOverViewTrigger::commitAdd(CMarketData *pMarketData)
{
	CMarketOverViewField field;
	CopyMarketOverViewEntity(&field, pMarketData);
	sprintf(m_pServiceResponser->GetMetaRef()->SubIndexName, "%s_%s,%s", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.ProductGroup.getValue());
	//m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
	m_pServiceResponser->SendToSubjectID(TID_PushMarketOverView, &field, &(CMarketOverViewField::m_Describe), OF_Topic_Market_OverView, 3);
}

void CMarketOverViewTrigger::commitUpdate(CMarketData *pMarketData, CWriteableMarketData *pOldMarketData)
{
	//一秒钟发一次
	//最新价变化就一定发送ProductGroup

	char * pIndex = NULL;
	//if ((pOldMarketData->LastPrice != pMarketData->LastPrice)
	//	|| (pOldMarketData->MarkedPrice != pMarketData->MarkedPrice))
	//{
	//	m_pServiceResponser->GetMetaRef()->SubIndexName[0] = '\0';
	//	pIndex = fastStrcat(m_pServiceResponser->GetMetaRef()->SubIndexName, pMarketData->ProductGroup.getValue());
	//	if (pMarketData->UpdateTime != pOldMarketData->UpdateTime)
	//	{
	//		pIndex = fastStrcat(pIndex,",");
	//		pIndex = fastStrcat(pIndex, pMarketData->ExchangeID.getValue());
	//		pIndex = fastStrcat(pIndex, "_");
	//		pIndex = fastStrcat(pIndex, pMarketData->InstrumentID.getValue());
	//	}
	//}
	//else
	//{
	//	if (pMarketData->UpdateTime != pOldMarketData->UpdateTime)
	//	{
	//		m_pServiceResponser->GetMetaRef()->SubIndexName[0] = '\0';
	//		pIndex = fastStrcat(m_pServiceResponser->GetMetaRef()->SubIndexName, pMarketData->ExchangeID.getValue());
	//		pIndex = fastStrcat(pIndex, "_");
	//		pIndex = fastStrcat(pIndex, pMarketData->InstrumentID.getValue());
	//	}
	//}
	if (pMarketData->UpdateTime / 1000 != pOldMarketData->UpdateTime / 1000)
	{
		m_pServiceResponser->GetMetaRef()->SubIndexName[0] = '\0';
		pIndex = fastStrcat(m_pServiceResponser->GetMetaRef()->SubIndexName, pMarketData->ProductGroup.getValue());
		pIndex = fastStrcat(pIndex, ",");
		pIndex = fastStrcat(m_pServiceResponser->GetMetaRef()->SubIndexName, pMarketData->ExchangeID.getValue());
		pIndex = fastStrcat(pIndex, "_");
		pIndex = fastStrcat(pIndex, pMarketData->InstrumentID.getValue());
	}

	if (pIndex != NULL || pMarketData->InstrumentStatus != pOldMarketData->InstrumentStatus)
	{
		CMarketOverViewField field;
		CopyMarketOverViewEntity(&field, pMarketData);
		m_pServiceResponser->SendToSubjectID(TID_PushMarketOverView, &field, &(CMarketOverViewField::m_Describe), OF_Topic_Market_OverView, 3);
	}
}
