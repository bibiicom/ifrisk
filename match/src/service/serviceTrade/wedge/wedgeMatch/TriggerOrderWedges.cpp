#include "TriggerOrderWedges.h"
#include "CopyField.h"
#include "tradingTools.h"

CPositionCloseAllTrigger::CPositionCloseAllTrigger(CMDB *pMDB)
{
	m_pMDB = pMDB;
}

CPositionCloseAllTrigger::~CPositionCloseAllTrigger()
{
}

void CPositionCloseAllTrigger::commitAdd(CTriggerOrder *pTriggerOrder)
{
	if (pTriggerOrder->OffsetFlag == OF_CloseAll)
	{
		CPosition *pPosition = m_pMDB->m_PositionFactory->findByPositionID(pTriggerOrder->PositionID);
		if (pPosition != NULL)
		{
			//如果有止盈止损订单，需要删除
			if (!pPosition->CloseOrderID.isNull())
			{
				CTriggerOrder* pTriggerOrderOld = m_pMDB->m_TriggerOrderFactory->findByOrderID(pPosition->CloseOrderID);
				if (pTriggerOrderOld != NULL)
				{
					//添加到FinishTriggerOrder
					CWriteableTriggerOrder theTriggerOrder;
					m_pMDB->m_TriggerOrderFactory->retrieve(pTriggerOrderOld, &theTriggerOrder);
					theTriggerOrder.TriggerStatus = TS_Canceled;
					theTriggerOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
					m_pMDB->m_TriggerOrderFactory->update(pTriggerOrderOld, &theTriggerOrder);
					//保存TriggerOrder，remove统一在commit之后做
					gDeleteTriggerOrderSet.insert(pTriggerOrderOld);
				}
			}
			CWriteablePosition thePosition;
			m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
			thePosition.CloseOrderID = pTriggerOrder->OrderID;
			thePosition.SLTriggerPrice = pTriggerOrder->SLTriggerPrice;
			thePosition.TPTriggerPrice = pTriggerOrder->TPTriggerPrice;
			thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
		}
	}
	if (pTriggerOrder->TriggerStatus >= TS_TriggerSuccess)
	{
		//保存TriggerOrder，remove统一在commit之后做
		gDeleteTriggerOrderSet.insert(pTriggerOrder);
	}
}

void CPositionCloseAllTrigger::commitUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pOldTriggerOrder)
{
	if (pTriggerOrder->TriggerStatus >= TS_TriggerSuccess)
	{
		//保存TriggerOrder，remove统一在commit之后做
		gDeleteTriggerOrderSet.insert(pTriggerOrder);

		if (pTriggerOrder->OffsetFlag == OF_CloseAll)
		{
			CPosition *pPosition = m_pMDB->m_PositionFactory->findByPositionID(pTriggerOrder->PositionID);
			if (pPosition != NULL)
			{
				//由于pPosition在一定情况下会删除，如果trigger更新多次会出问题，所以在删除的情况下，就不更新了
				//if (pPosition->TradeUnitID != pPosition->AccountID && pPosition->TradeUnitID != getIsolateMergeTradeUnitID(pPosition->AccountID))
				if (!pPosition->TradeUnitID.like(pPosition->AccountID))
				{
					if (pPosition->Position == 0.0 && pPosition->LongFrozen == 0.0 && pPosition->ShortFrozen == 0.0)
						return;
				}

				if (!pPosition->CloseOrderID.isNull() && pPosition->CloseOrderID == pTriggerOrder->OrderID) {
					CWriteablePosition thePosition;
					m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
					thePosition.CloseOrderID.clear();
					thePosition.SLTriggerPrice.clear();
					thePosition.TPTriggerPrice.clear();
					thePosition.Frequency.clear();
					thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
					m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
				}
			}
		}
	}
}

void CPositionCloseAllTrigger::commitRemove(CWriteableTriggerOrder *pTriggerOrder)
{

}

//CUserTriggerOrdersTrigger::CUserTriggerOrdersTrigger(CMDB *pMDB)
//{
//	m_pMDB = pMDB;
//}
//
//CUserTriggerOrdersTrigger::~CUserTriggerOrdersTrigger()
//{
//}
//
//void CUserTriggerOrdersTrigger::commitAdd(CTriggerOrder *pTriggerOrder)
//{
//	//CMember *pMember = pTriggerOrder->getMember(m_pMDB->m_MemberFactory);
//	//if (pMember != NULL)
//	//{
//	//	CWriteableMember theMember;
//	//	m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
//	//	theMember.TriggerOrders += 1;
//	//	theMember.SinkType = ST_SinkToDB;
//	//	m_pMDB->m_MemberFactory->update(pMember, &theMember);
//	//}
//}
//
//void CUserTriggerOrdersTrigger::commitUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pOldTriggerOrder)
//{
//
//}
//
//void CUserTriggerOrdersTrigger::commitRemove(CWriteableTriggerOrder *pTriggerOrder)
//{
//	//CMember *pMember = pTriggerOrder->getMember(m_pMDB->m_MemberFactory);
//	//if (pMember != NULL)
//	//{
//	//	CWriteableMember theMember;
//	//	m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
//	//	theMember.TriggerOrders -= 1;
//	//	theMember.SinkType = ST_SinkToDB;
//	//	m_pMDB->m_MemberFactory->update(pMember, &theMember);
//	//}
//}

CUserOpenOrdersTrigger::CUserOpenOrdersTrigger(CMDB *pMDB)
{
	m_pMDB = pMDB;
}

CUserOpenOrdersTrigger::~CUserOpenOrdersTrigger()
{
}

void CUserOpenOrdersTrigger::commitAdd(COrder *pOrder)
{
	//CMember *pMember = pOrder->getMember(m_pMDB->m_MemberFactory);
	//if (pMember != NULL)
	//{
	//	CWriteableMember theMember;
	//	m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
	//	theMember.OpenOrders += 1;
	//	theMember.SinkType = ST_NoSink;
	//	m_pMDB->m_MemberFactory->update(pMember, &theMember);
	//}
	//这里就非常有技巧和容易忽略的地方，由于Transaction的合并操作，导致add+update 变成了add,后面的update不会继续调用了
	//所以这里需要判断add进来的是不是已经改变了状态的订单
	if (pOrder->OrderStatus == OS_AllCanceled || pOrder->OrderStatus == OS_PartCanceled || pOrder->OrderStatus == OS_AllTraded)
	{
		//保存Order，remove统一在commit之后做
		gDeleteOrderSet.insert(pOrder);
		ChangeTriggerOrder(pOrder);
	}
}

void CUserOpenOrdersTrigger::commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder)
{
	//投机取巧，为了不新增Triiger
	if (pOrder->OrderStatus == OS_AllCanceled || pOrder->OrderStatus == OS_PartCanceled || pOrder->OrderStatus == OS_AllTraded)
	{
		//保存Order，remove统一在commit之后做
		gDeleteOrderSet.insert(pOrder);
		ChangeTriggerOrder(pOrder);
	}
}

void CUserOpenOrdersTrigger::ChangeTriggerOrder(COrder *pOrder)
{
	// 处理委托对应的触发单
	CTriggerOrder *pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findByOrderID(pOrder->RelatedOrderID.getValue());

	if (pTriggerOrder != NULL)
	{
		//这里需要考虑如果TriggerOrder订单状态发生了改变，不能再update，会导致重复删除
		if (pTriggerOrder->TriggerStatus >= TS_TriggerSuccess)
			return;

		if (pOrder->OrderStatus == OS_AllTraded && pTriggerOrder->TriggerStatus < TS_Active)//只处理“未设置状态”的触发单
		{
			if (pOrder->PosiDirection == PD_Net)
			{
				CPosition *pPosition = pOrder->getPosition();
				if (pPosition != NULL)
				{
					if (pPosition->Position == 0 || (pPosition->Position > 0 && pTriggerOrder->Direction == D_Buy) || (pPosition->Position < 0 && pTriggerOrder->Direction == D_Sell))
					{
						CWriteableTriggerOrder theTriggerOrder;
						m_pMDB->m_TriggerOrderFactory->retrieve(pTriggerOrder, &theTriggerOrder);
						theTriggerOrder.TriggerStatus = TS_Canceled;
						theTriggerOrder.ErrorMsg = "NoPositionToClose";
						theTriggerOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
						m_pMDB->m_TriggerOrderFactory->update(pTriggerOrder, &theTriggerOrder);
						return;
					}
				}

			}
			// 委托全部成交，将对应的触发单激活
			//CTriggerOrder* pTobeUpdate = pTriggerOrder;
			CWriteableTriggerOrder theTriggerOrder;
			m_pMDB->m_TriggerOrderFactory->retrieve(pTriggerOrder, &theTriggerOrder);
			CPriceType basePrice = pOrder->TradePrice;
			bool needDelete = false;
			// 判断止盈止损价格与成交价格比较
			if (pTriggerOrder->Direction == D_Buy)
			{
				if (!pTriggerOrder->SLTriggerPrice.isNull() && pTriggerOrder->SLTriggerPrice <= basePrice)
					needDelete = true;
				if (!pTriggerOrder->TPTriggerPrice.isNull() && pTriggerOrder->TPTriggerPrice >= basePrice)
					needDelete = true;
			}
			//买入开多，止损价应该小于市价/标记价，止盈价应该大于市价/标记价，反了必须直接返回错误
			else
			{
				if (!pTriggerOrder->SLTriggerPrice.isNull() && pTriggerOrder->SLTriggerPrice >= basePrice)
					needDelete = true;
				if (!pTriggerOrder->TPTriggerPrice.isNull() && pTriggerOrder->TPTriggerPrice <= basePrice)
					needDelete = true;
			}

			if (needDelete) {
				CWriteableTriggerOrder theTriggerOrder;
				m_pMDB->m_TriggerOrderFactory->retrieve(pTriggerOrder, &theTriggerOrder);
				theTriggerOrder.TriggerStatus = TS_Canceled;
				theTriggerOrder.ErrorMsg = "SLTriggerPrice/TPTriggerPrice outof TradePrice";
				theTriggerOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
				m_pMDB->m_TriggerOrderFactory->update(pTriggerOrder, &theTriggerOrder);
			}
			else {
				theTriggerOrder.TriggerStatus = TS_Active;
				theTriggerOrder.Volume = pOrder->VolumeTraded;
				theTriggerOrder.TradeUnitID = pOrder->TradeUnitID;
				theTriggerOrder.ActiveTime = g_nCurrentTime;
				theTriggerOrder.PositionID = pOrder->PositionID;
				theTriggerOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
				//后续TriggerOrder激活之后会产生新的Order，这里需要给其腾出空间
				theTriggerOrder.RelatedOrderID.clear();
				theTriggerOrder.linkOrder((COrder*)NULL);
				m_pMDB->m_TriggerOrderFactory->update(pTriggerOrder, &theTriggerOrder);
			}
		}
		else if (pOrder->OrderStatus == OS_PartCanceled || pOrder->OrderStatus == OS_AllCanceled)
		{
			// 委托撤单，将对应的触发单撤单
			CWriteableTriggerOrder theTriggerOrder;
			m_pMDB->m_TriggerOrderFactory->retrieve(pTriggerOrder, &theTriggerOrder);
			theTriggerOrder.TriggerStatus = TS_Canceled;
			theTriggerOrder.ErrorMsg = "OriginalOrderCanceled";
			theTriggerOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_TriggerOrderFactory->update(pTriggerOrder, &theTriggerOrder);
		}
	}

}

//void CUserOpenOrdersTrigger::commitRemove(CWriteableOrder *pOrder)
//{
//	//CMember *pMember = pOrder->getMember(m_pMDB->m_MemberFactory);
//	//if (pMember != NULL)
//	//{
//	//	CWriteableMember theMember;
//	//	m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
//	//	theMember.OpenOrders -= 1;
//	//	theMember.SinkType = ST_NoSink;
//	//	m_pMDB->m_MemberFactory->update(pMember, &theMember);
//	//}
//}
//

CUserOpenPositionsTrigger::CUserOpenPositionsTrigger(CMDB *pMDB)
{
	m_pMDB = pMDB;
}

CUserOpenPositionsTrigger::~CUserOpenPositionsTrigger()
{
}

void CUserOpenPositionsTrigger::commitAdd(CPosition *pPosition)
{
	//if (pPosition->Position != 0)
	//{
	//	CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(pPosition->MemberID);
	//	if (pMember != NULL)
	//	{
	//		CWriteableMember theMember;
	//		m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
	//		theMember.OpenPositions += 1;
	//		theMember.SinkType = ST_NoSink;
	//		m_pMDB->m_MemberFactory->update(pMember, &theMember);
	//	}
	//}
	// 将分仓模式下的对应持仓删除
	//if (pPosition->TradeUnitID != pPosition->AccountID && pPosition->TradeUnitID != getIsolateMergeTradeUnitID(pPosition->AccountID))
	//if (!pPosition->TradeUnitID.like(pPosition->AccountID))
	//{
	//	if (pPosition->Position == 0.0 && pPosition->LongFrozen == 0.0 && pPosition->ShortFrozen == 0.0)
	//		m_pMDB->m_PositionFactory->remove(pPosition);
	//}

	if (pPosition->Position == 0.0 && pPosition->ShortFrozen == 0.0 && pPosition->LongFrozen == 0.0)
	{
		if(!pPosition->TradeUnitID.like(pPosition->AccountID))
			gDeletePositionSet.insert(pPosition);
	}
}

void CUserOpenPositionsTrigger::commitUpdate(CPosition *pPosition, CWriteablePosition *pOldPosition)
{
	if (pPosition->Position == 0.0 && pPosition->ShortFrozen == 0.0 && pPosition->LongFrozen == 0.0)
	{
		if (!pPosition->TradeUnitID.like(pPosition->AccountID))
			gDeletePositionSet.insert(pPosition);
	}

	// 将分仓模式下的对应持仓删除
	//if (pPosition->TradeUnitID != pPosition->AccountID && pPosition->TradeUnitID != getIsolateMergeTradeUnitID(pPosition->AccountID))
	//if (!pPosition->TradeUnitID.like(pPosition->AccountID))
	//{
	//	if (pPosition->Position == 0.0 && pPosition->LongFrozen == 0.0 && pPosition->ShortFrozen == 0.0)
	//		m_pMDB->m_PositionFactory->remove(pPosition);
	//}

	//if (pOldPosition->Position != 0)
	//{
	//	if (pPosition->Position == 0)
	//	{
	//		CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(pPosition->MemberID);
	//		if (pMember != NULL)
	//		{
	//			CWriteableMember theMember;
	//			m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
	//			theMember.OpenPositions -= 1;
	//			theMember.SinkType = ST_NoSink;
	//			m_pMDB->m_MemberFactory->update(pMember, &theMember);
	//		}
	//	}
	//}

	//if (pOldPosition->Position == 0)
	//{
	//	if (pPosition->Position != 0)
	//	{
	//		CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(pPosition->MemberID);
	//		if (pMember != NULL)
	//		{
	//			CWriteableMember theMember;
	//			m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
	//			theMember.OpenPositions += 1;
	//			theMember.SinkType = ST_NoSink;
	//			m_pMDB->m_MemberFactory->update(pMember, &theMember);
	//		}
	//	}
	//}
}
//
//void CUserOpenPositionsTrigger::commitRemove(CWriteablePosition *pPosition)
//{
//	//if (pPosition->Position != 0)
//	//{
//	//	CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(pPosition->MemberID);
//	//	if (pMember != NULL)
//	//	{
//	//		CWriteableMember theMember;
//	//		m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
//	//		theMember.OpenPositions -= 1;
//	//		theMember.SinkType = ST_NoSink;
//	//		m_pMDB->m_MemberFactory->update(pMember, &theMember);
//	//	}
//	//}
//}
//
//CTriggerOrderStatusTrigger::CTriggerOrderStatusTrigger(CMDB *pMDB)
//{
//	m_pMDB = pMDB;
//}
//
//CTriggerOrderStatusTrigger::~CTriggerOrderStatusTrigger()
//{
//}

//void CTriggerOrderStatusTrigger::commitAdd(COrder *pOrder)
//{
//}
//
//void CTriggerOrderStatusTrigger::commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder)
//{
//}
//
//void CTriggerOrderStatusTrigger::commitRemove(CWriteableOrder *pOrder)
//{
//}

CTriggerOrderWedge::CTriggerOrderWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
	:CWedge(pTradeService, pMDB, pErrorEngine)
{
	m_pTradeService = pTradeService;
	//	m_iTriggerOrderMode = iTriggerOrderMode;
	m_sName = "TriggerOrder";
}

CTriggerOrderWedge::~CTriggerOrderWedge(void)
{
}

bool CTriggerOrderWedge::changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction)
{
	//这里要注意，如果修改订单的数目会导致成交，成交还会导致修改订单
	//这样m_NextTriggerOrderActionList的clear就会出问题
	//这种情况下，应该撤单
	if (pChangeList->pNewPosition != NULL)
	{
		if (pChangeList->pNewPosition->Position != 0.0)
		{
			if (pChangeList->pNewPosition->PosiDirection != PD_Net)
				return true;

			if (pChangeList->pOldPosition == NULL)
				return true;

			//净仓方向没有反转
			//if (pChangeList->pNewPosition->Position*pChangeList->pOldPosition->Position > 0)
			//	return true;

			if (pChangeList->pOldPosition->Position == 0)
				return true;

			if (pChangeList->pOldPosition->Position > 0)
			{
				if (pChangeList->pNewPosition->Position > 0)
					return true;
			}
			else
			{
				if (pChangeList->pNewPosition->Position < 0)
					return true;
			}
		}

		//如果是报单不需要，如果是撤单或者成交才需要撤掉所有的触发单
		if (pChangeList->pTrade == NULL && pChangeList->pOrderCancle == NULL)
			return true;

		CTriggerOrderActionField theTriggerOrderAction;
		//theTriggerOrderAction.init();
		//theTriggerOrderAction.UserID = pChangeList->pNewPosition->UserID;
		theTriggerOrderAction.ActionFlag = AF_Delete;

		CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByActivePositionClose(pChangeList->pNewPosition->PositionID);
		while (pTriggerOrder != NULL)
		{
			//自己不能删除自己,如果是CloseAll的需要删除的话，会进入死循环
			if (pTriggerOrder->OrderID != pChangeList->pOrder->RelatedOrderID)
			{
				theTriggerOrderAction.OrderID = pTriggerOrder->OrderID;
				m_NextTriggerOrderActionList.push_back(theTriggerOrderAction);
			}
			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByActivePositionClose();
		}
	}
	return true;
}

bool CTriggerOrderWedge::doAfterOrderChange(COrder *pOrder)
{
	if (m_NextTriggerOrderActionList.size() > 0)
	{
		for (size_t i = 0; i < m_NextTriggerOrderActionList.size(); i++)
		{
			CTriggerOrderActionField* pOrderTriggerAction = &(m_NextTriggerOrderActionList[i]);
			CTransaction theTransaction;
			bool success = m_TradeService->triggerOrderAction(pOrderTriggerAction, &theTransaction);
			if (success)
				theTransaction.commit();
			else
				theTransaction.rollback();
		}
		m_NextTriggerOrderActionList.clear();
	}
	return true;
}

//bool CTriggerOrderWedge::timeSync(CWriteableCurrentTime *pCurrentTime, CTransaction *pTransaction)
//{
//	return true;
//	// 内部运行时才执行，0：内部运行，1：外部运行
//	if (m_iTriggerOrderMode != 0)
//		return true;
//
//	//优化:2秒触发
//	if (pCurrentTime->ZoneTime[7] % 2 == 0)
//		return true;
//
//	//规定每次触发的条数
//	int nTriggerCount = 0;
//
//	CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
//	while (pMarketData != NULL)
//	{
//		CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByBuySLTriggerPrice(pMarketData->ExchangeID, pMarketData->InstrumentID, pMarketData->LastPrice);
//		while (pTriggerOrder != NULL)
//		{
//			nTriggerCount++;
//			if (nTriggerCount > g_nMaxTriggerCount)
//			{
//				m_pMDB->m_TriggerOrderFactory->endFindByBuySLTriggerPrice();
//				m_pMDB->m_MarketDataFactory->endGet();
//				return true;
//			}
//			CTriggerOrder* pTobeRemove = pTriggerOrder;
//			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByBuySLTriggerPrice();
//			doTriggerOrder(pTobeRemove, pTobeRemove->SLTriggerPrice, pTransaction);
//		}
//		m_pMDB->m_TriggerOrderFactory->endFindByBuySLTriggerPrice();
//
//		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstBySellSLTriggerPrice(pMarketData->ExchangeID, pMarketData->InstrumentID, pMarketData->LastPrice);
//		while (pTriggerOrder != NULL)
//		{
//			nTriggerCount++;
//			if (nTriggerCount > g_nMaxTriggerCount)
//			{
//				m_pMDB->m_TriggerOrderFactory->endFindBySellSLTriggerPrice();
//				m_pMDB->m_MarketDataFactory->endGet();
//				return true;
//			}
//			CTriggerOrder* pTobeRemove = pTriggerOrder;
//			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextBySellSLTriggerPrice();
//			doTriggerOrder(pTobeRemove, pTobeRemove->SLTriggerPrice, pTransaction);
//		}
//		m_pMDB->m_TriggerOrderFactory->endFindBySellSLTriggerPrice();
//
//		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByBuyTPTriggerPrice(pMarketData->ExchangeID, pMarketData->InstrumentID, pMarketData->LastPrice);
//		while (pTriggerOrder != NULL)
//		{
//			nTriggerCount++;
//			if (nTriggerCount > g_nMaxTriggerCount)
//			{
//				m_pMDB->m_TriggerOrderFactory->endFindByBuyTPTriggerPrice();
//				m_pMDB->m_MarketDataFactory->endGet();
//				return true;
//			}
//			CTriggerOrder* pTobeRemove = pTriggerOrder;
//			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByBuyTPTriggerPrice();
//			doTriggerOrder(pTobeRemove, pTobeRemove->TPTriggerPrice, pTransaction);
//		}
//		m_pMDB->m_TriggerOrderFactory->endFindByBuyTPTriggerPrice();
//
//		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstBySellTPTriggerPrice(pMarketData->ExchangeID, pMarketData->InstrumentID, pMarketData->LastPrice);
//		while (pTriggerOrder != NULL)
//		{
//			nTriggerCount++;
//			if (nTriggerCount > g_nMaxTriggerCount)
//			{
//				m_pMDB->m_TriggerOrderFactory->endFindBySellTPTriggerPrice();
//				m_pMDB->m_MarketDataFactory->endGet();
//				return true;
//			}
//			CTriggerOrder* pTobeRemove = pTriggerOrder;
//			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextBySellTPTriggerPrice();
//			doTriggerOrder(pTobeRemove, pTobeRemove->TPTriggerPrice, pTransaction);
//		}
//		m_pMDB->m_TriggerOrderFactory->endFindBySellTPTriggerPrice();
//
//		pMarketData = m_pMDB->m_MarketDataFactory->getNext();
//	}
//	m_pMDB->m_MarketDataFactory->endGet();
//	return true;
//}

//bool CTriggerOrderWedge::doTriggerOrder(CTriggerOrder *pTriggerOrder, CPriceType Price, CTransaction *pTransaction)
//{
//	///总是按照超级会话的方式操作
//	TpSession pSession;
//	pSession->frontID = 0;
//	pSession->sessionID = 0;
//	pSession->pToken = NULL;
//	CWriteableTriggerOrderAction theTriggerOrderAction;
//	theTriggerOrderAction.init();
//	theTriggerOrderAction.OrderID = pTriggerOrder->OrderID;
//	theTriggerOrderAction.Price = Price;
//	theTriggerOrderAction.ActionFlag = AF_Trigger;
//	CTransaction Transaction;
//	if (m_pTradeService->triggerOrderAction(&theTriggerOrderAction, &Transaction))
//		Transaction.commit();
//	else
//		Transaction.rollback();
//
//	return true;
//}
