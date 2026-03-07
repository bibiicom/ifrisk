#include "CTradeService.h"
#include "CWedge.h"
#include "tradingTools.h"
#include "monitorIndex.h"
#include "CopyField.h"
#include "printinfo.h"
#include "monitorIndex.h"
#include "CopyField.h"

CTradeService::CTradeService(CMDB* pMDB, CErrorEngine* pErrorEngine)
{
	m_pMDB = pMDB;
	m_pErrorEngine = pErrorEngine;
	m_Wedge = NULL;
	m_pOrderBookList = new COrderBookList(pMDB);
}

CTradeService::~CTradeService(void)
{
	if (m_Wedge != NULL)
	{
		delete m_Wedge;
	}
	delete m_pOrderBookList;
}

void CTradeService::setWedge(CWedge* pWedge)
{
	if (m_Wedge != NULL)
	{
		delete m_Wedge;
	}
	m_Wedge = pWedge;
}

bool CTradeService::triggerOrderInsert(CWriteableTriggerOrder* pTriggerOrder, CTriggerStatusType triggerStatus, CTransaction* pTransaction, CReadOnlyPriceType* pBasePrice)
{
	pTriggerOrder->m_pMDB = m_pMDB;
	//开始进行OrderID的赋值，为了确保反演不一致的情况下，撤单ID就乱了
	//如果预先给的话，Service的不一致不会影响OrderID的撤单
	pTriggerOrder->OrderID = m_gOrderSequence.getNextValueByString();

	//检查用户的当前triggerOrder报单量是否超过MaxTriggerOrders限定值
	CMember* pMember = pTriggerOrder->getMember();
	if (pMember == NULL)
		return m_pErrorEngine->reportError(ERROR_USER_NOT_FOUND, pTriggerOrder->MemberID.getValue());

	if (!pMember->TriggerOrders.isNull() && pMember->TriggerOrders >= g_nMaxTriggerOrders)
		return m_pErrorEngine->reportError(ERROR_ORDER_LIMIT_EXCEED, "%d>=%d", pMember->TriggerOrders.getValue(), g_nMaxTriggerOrders);

	// 条件单的带单，跟单判断逻辑保持与Order一致, 下单接口中传入了CopyMemberID值
	if (!pTriggerOrder->CopyMemberID.isNull()) {
		if (pTriggerOrder->MemberID == pTriggerOrder->CopyMemberID) {
			REPORT_EVENT(LOG_DEBUG, "TLeaderOrder V3.0", "MID:%s, CMID:%s, OrderID:%s",
				pTriggerOrder->MemberID.getValue(), pMember->CopyMemberID.getValue(), pTriggerOrder->OrderID.getValue());
			pTriggerOrder->CopyMemberID = pTriggerOrder->MemberID;
			pTriggerOrder->CopyOrderID = pTriggerOrder->OrderID;
		}
		else if (pTriggerOrder->CopyMemberID == "0") {
			REPORT_EVENT(LOG_DEBUG, "TOrder V3.0", "MID:%s, CMID:%s, OrderID:%s",
				pTriggerOrder->MemberID.getValue(), pMember->CopyMemberID.getValue(), pTriggerOrder->OrderID.getValue());
		}
		else {
			REPORT_EVENT(LOG_DEBUG, "TFollowerOrder", "MID:%s, CMID:%s, OrderID:%s, CopyOrderID:%s",
				pTriggerOrder->MemberID.getValue(), pTriggerOrder->CopyMemberID.getValue(), pTriggerOrder->OrderID.getValue(),
				pTriggerOrder->CopyOrderID.getValue());
			//检查跟单者的CopyMemberID是否在Member表中
			CMember* leaderMember = m_pMDB->m_MemberFactory->findByMemberID(pTriggerOrder->CopyMemberID);
			if (leaderMember == NULL) {
				REPORT_EVENT(LOG_ERROR, "TLeader MID error, not exist", "CMID:%s", pTriggerOrder->CopyMemberID.getValue());
				pTriggerOrder->CopyMemberID.clear();
				pTriggerOrder->CopyOrderID.clear();
			}
		}
	}
	else {
		if (!pMember->CopyMemberID.isNull()) {
			if (pMember->CopyMemberID == pMember->MemberID) {
				REPORT_EVENT(LOG_DEBUG, "TLeaderOrder V2.0", "MID:%s, CMID:%s, OrderID:%s, InstrumentID:%s",
					pTriggerOrder->MemberID.getValue(), pMember->CopyMemberID.getValue(),
					pTriggerOrder->OrderID.getValue(), pTriggerOrder->InstrumentID.getValue());
				//添加交易对判断逻辑
				if ((pTriggerOrder->InstrumentID == "BTCUSDT") || (pTriggerOrder->InstrumentID == "ETHUSDT")) {
					pTriggerOrder->CopyMemberID = pTriggerOrder->MemberID;
					pTriggerOrder->CopyOrderID = pTriggerOrder->OrderID;
				}
			}
			else {
				REPORT_EVENT(LOG_ERROR, "TLeader V2.0 CMID error", "MID:%s, CMID:%s",
					pTriggerOrder->MemberID.getValue(), pMember->CopyMemberID.getValue());
			}
		}
	}

	CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pTriggerOrder->ExchangeID, pTriggerOrder->InstrumentID);
	if (pMarketData == NULL)
		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "%s+%s", pTriggerOrder->ExchangeID.getValue(), pTriggerOrder->InstrumentID.getValue());
	pTriggerOrder->linkMarketData(pMarketData);
	pTriggerOrder->linkInstrument(pMarketData->getInstrument());

	//pTriggerOrder->ProductGroup = pMarketData->ProductGroup;
	pTriggerOrder->TriggerStatus = triggerStatus;

	//if (pTriggerOrder->Price.isNull())
	//	pTriggerOrder->OrderPriceType = OPT_TenLevelPrice;

	//if (pTriggerOrder->MemberID.isNull())
	//	pTriggerOrder->MemberID = pMember->DefaultMemberID;

	if (pTriggerOrder->AccountID.isNull())
		pTriggerOrder->AccountID = pTriggerOrder->MemberID;

	InitTriggerOrderInfo(m_pMDB, pTriggerOrder, pTriggerOrder->Leverage, pTriggerOrder->IsCrossMargin);

	if (pTriggerOrder->TradeUnitID.isNull())
	{
		pTriggerOrder->TradeUnitID = pTriggerOrder->AccountID;
	}
	else
	{
		//// 开仓时检查全仓逐仓的TradeUnitID和IsCrossMargin是否匹配
		//if (pTriggerOrder->OffsetFlag == OF_Open && pTriggerOrder->TradeUnitID != defaultTradeUnitID)
		//{
		//	if (pTriggerOrder->TradeUnitID == pTriggerOrder->AccountID)
		//	{
		//		if (!pTriggerOrder->IsCrossMargin.isNull() && !pTriggerOrder->IsCrossMargin)
		//			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitID[%s],IsCrossMargin[%d]", pTriggerOrder->TradeUnitID.getValue(), pTriggerOrder->IsCrossMargin.getValue());
		//	}
		//	else if (pTriggerOrder->TradeUnitID == getIsolateMergeTradeUnitID(pTriggerOrder->AccountID))
		//	{
		//		if (!pTriggerOrder->IsCrossMargin.isNull() && pTriggerOrder->IsCrossMargin)
		//			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitID[%s],IsCrossMargin[%d]", pTriggerOrder->TradeUnitID.getValue(), pTriggerOrder->IsCrossMargin.getValue());
		//	}
		//}
	}

	CWriteableOrder theTrueOrder;
	theTrueOrder.init(m_pMDB);
	theTrueOrder.linkMember(pMember);
	CopyInputOrderEntity(&theTrueOrder, pTriggerOrder);
	theTrueOrder.linkInstrument(pTriggerOrder->getInstrument());

	//如果是全平，意味着是仓位上面的止盈止损，需要配合仓位的属性来
	CPosition* pPosition = getPosition(m_pMDB, &theTrueOrder);
	pTriggerOrder->PosiDirection = theTrueOrder.PosiDirection;

	if (pPosition != NULL)
	{
		pTriggerOrder->AccountID = pPosition->AccountID;
		pTriggerOrder->Leverage = pPosition->Leverage;
		pTriggerOrder->IsCrossMargin = pPosition->IsCrossMargin;
	}

	if (triggerStatus != TS_None && pTriggerOrder->OffsetFlag != OF_Open)
	{
		if (pPosition == NULL)
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE, "PositionNotFound");
		if (pPosition->Position == 0.0)
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE, "Position=0");
		pTriggerOrder->PositionID = pPosition->PositionID;

		//仓位上的CopyMemberID需要带回TriggerOrder里面来
		if ((!pPosition->CopyMemberID.isNull()) && (pPosition->CopyMemberID != "0") && (pPosition->Position != 0))
		{
			pTriggerOrder->CopyMemberID = pPosition->CopyMemberID;
			pTriggerOrder->CopyOrderID = pTriggerOrder->OrderID;
		}
		//仓位上的CopyProfitRate需要带回TriggerOrder里面来
		if ((!(pPosition->CopyProfitRate.isNull())) && (pPosition->CopyProfitRate > 0))
			pTriggerOrder->CopyProfitRate = pPosition->CopyProfitRate;
	}

	//全平，持仓止盈止损
	if (pTriggerOrder->OffsetFlag == OF_CloseAll)
		pTriggerOrder->TriggerOrderType = TOT_PositionTPSL;

	if (pTriggerOrder->TriggerOrderType == TOT_None)
		pTriggerOrder->TriggerOrderType = TOT_Condition;

	if (!triggerOrderPriceCheck(pTriggerOrder, pBasePrice))
		return false;

	if (pTriggerOrder->RiskBefore)
	{
		//如果需要提前做，不是所有的都能支持的，需要限价订单并且不是止盈止损才可以
		if (pTriggerOrder->OrderPriceType != OPT_LimitPrice || pTriggerOrder->TriggerPrice.isNull())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
		theTrueOrder.DeriveSource = DS_FromTriggerOrder;
		theTrueOrder.Tradable = false;
		theTrueOrder.RelatedOrderID = pTriggerOrder->OrderID;
		theTrueOrder.BusinessValue = pTriggerOrder->OrderID;
		insertOrder(&theTrueOrder, pTransaction);
		pTriggerOrder->RelatedOrderID = theTrueOrder.OrderID;
		pTriggerOrder->linkOrder(m_pMDB->m_OrderFactory->findByOrderID(pTriggerOrder->RelatedOrderID));
	}
	m_pMDB->m_TriggerOrderFactory->add(pTriggerOrder, pTransaction);
	return true;
}

bool CTradeService::triggerOrderPriceCheck(CWriteableTriggerOrder* pTriggerOrder, CReadOnlyPriceType* pBasePrice)
{
	if (pTriggerOrder->TriggerOrderType >= TOT_Indicator) {
		return true;
	}

	//这三个价格，至少有一个价格有值，否者出错
	if (pTriggerOrder->TriggerPrice.isNull() && pTriggerOrder->SLTriggerPrice.isNull() && pTriggerOrder->TPTriggerPrice.isNull())
		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TriggerPrice/SLTriggerPrice/TPTriggerPrice=NULL");

	if (pTriggerOrder->getMarketData() == NULL)
		return m_pErrorEngine->reportError(ERROR_MARKETDATA_NOT_FOUND, "TriggerOrderMarketData");

	CPriceType basePrice = pTriggerOrder->getMarketData()->LastPrice;
	//没有传入的标记价，就用LatestPrice
	if (pBasePrice != NULL)
		basePrice = *pBasePrice;

	//提前判断，当TPTriggerPrice与SLTriggerPrice有值的情况下，先进行判断。
	//不然，TPTriggerPrice与SLTriggerPrice会由于TriggerPrice判断而被改变，甚至清空
	//卖出开空，止盈价应该小于市价/标记价，止损价应该大于市价/标记价，反了必须直接返回错误
	if (pTriggerOrder->Direction == D_Buy)
	{
		if (!pTriggerOrder->SLTriggerPrice.isNull() && pTriggerOrder->SLTriggerPrice <= basePrice)
			return m_pErrorEngine->reportError(ERROR_PriceOutOfLowerLimit, "SLTriggerPrice[%.4lf]<=BasePrice[%.4lf]", pTriggerOrder->SLTriggerPrice.getValue(), basePrice.getValue());
		if (!pTriggerOrder->TPTriggerPrice.isNull() && pTriggerOrder->TPTriggerPrice >= basePrice)
			return m_pErrorEngine->reportError(ERROR_PriceOutOfUpperLimit, "TPTriggerPrice[%.4lf]>=BasePrice[%.4lf]", pTriggerOrder->TPTriggerPrice.getValue(), basePrice.getValue());
	}
	//买入开多，止损价应该小于市价/标记价，止盈价应该大于市价/标记价，反了必须直接返回错误
	else
	{
		if (!pTriggerOrder->SLTriggerPrice.isNull() && pTriggerOrder->SLTriggerPrice >= basePrice)
			return m_pErrorEngine->reportError(ERROR_PriceOutOfUpperLimit, "SLTriggerPrice[%.4lf]>=BasePrice[%.4lf]", pTriggerOrder->SLTriggerPrice.getValue(), basePrice.getValue());
		if (!pTriggerOrder->TPTriggerPrice.isNull() && pTriggerOrder->TPTriggerPrice <= basePrice)
			return m_pErrorEngine->reportError(ERROR_PriceOutOfLowerLimit, "TPTriggerPrice[%.4lf]<=BasePrice[%.4lf]", pTriggerOrder->TPTriggerPrice.getValue(), basePrice.getValue());
	}

	if (!pTriggerOrder->TriggerPrice.isNull())
	{
		//卖出开空，止盈价应该小于市价/标记价，止损价应该大于市价/标记价
		if (pTriggerOrder->Direction == D_Buy)
		{
			if (pTriggerOrder->TriggerPrice <= basePrice)
			{
				if (pTriggerOrder->TPTriggerPrice.isNull()) {
					pTriggerOrder->TPTriggerPrice = pTriggerOrder->TriggerPrice;
				}
				if (pTriggerOrder->TPPrice.isNull())
					pTriggerOrder->TPPrice = pTriggerOrder->Price;
				pTriggerOrder->SLTriggerPrice.clear();
				pTriggerOrder->SLPrice.clear();
			}
			else
			{
				if (pTriggerOrder->SLTriggerPrice.isNull())
					pTriggerOrder->SLTriggerPrice = pTriggerOrder->TriggerPrice;
				if (pTriggerOrder->SLPrice.isNull())
					pTriggerOrder->SLPrice = pTriggerOrder->Price;
				pTriggerOrder->TPTriggerPrice.clear();
				pTriggerOrder->TPPrice.clear();
			}
		} /*pTriggerOrder->Direction == D_Buy*/
		//买入开多，止损价应该小于市价/标记价，止盈价应该大于市价/标记价
		else
		{
			if (pTriggerOrder->TriggerPrice <= basePrice)
			{
				if (pTriggerOrder->SLTriggerPrice.isNull())
					pTriggerOrder->SLTriggerPrice = pTriggerOrder->TriggerPrice;
				if (pTriggerOrder->SLPrice.isNull())
					pTriggerOrder->SLPrice = pTriggerOrder->Price;
				pTriggerOrder->TPTriggerPrice.clear();
				pTriggerOrder->TPPrice.clear();
			}
			else
			{
				if (pTriggerOrder->TPTriggerPrice.isNull())
					pTriggerOrder->TPTriggerPrice = pTriggerOrder->TriggerPrice;
				if (pTriggerOrder->TPPrice.isNull())
					pTriggerOrder->TPPrice = pTriggerOrder->Price;
				pTriggerOrder->SLTriggerPrice.clear();
				pTriggerOrder->SLPrice.clear();
			}
		}
	} /*!pTriggerOrder->TriggerPrice.isNull()*/
	return true;
}

CTriggerOrder* CTradeService::triggerOrderAction(CTriggerOrderActionField* pTriggerOrderAction, CTransaction* pTransaction)
{
	//触发订单不存在回退的可能性，就算失败，也是生成历史触发订单表，用于记录失败原因
	CTriggerOrder* pTriggerOrder = NULL;
	if (!pTriggerOrderAction->OrderID.isNull())
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findByOrderID(pTriggerOrderAction->OrderID);
	else
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findByLocalID(pTriggerOrderAction->AccountID, pTriggerOrderAction->OrderLocalID);

	if (pTriggerOrder == NULL)
	{
		m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND);
		return NULL;
	}

	CWriteableTriggerOrder theTriggerOrder;
	m_pMDB->m_TriggerOrderFactory->retrieve(pTriggerOrder, &theTriggerOrder);
	switch (pTriggerOrderAction->ActionFlag)
	{
	case AF_Delete:
	{
		if (pTriggerOrder->TriggerStatus == TS_Canceled)
			m_pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);
		//如果之前做了风控检测，需要撤单
		if (pTriggerOrder->RiskBefore)
		{
			CWriteableOrderAction theOrderAction;
			theOrderAction.init(m_pMDB);
			theOrderAction.OrderID = pTriggerOrder->RelatedOrderID;
			theOrderAction.ActionFlag = AF_Delete;
			orderAction(&theOrderAction, pTransaction);
		}
		theTriggerOrder.TriggerStatus = TS_Canceled;
		//查找是否有订单的止盈止损单，做一定清理工作
		if (!theTriggerOrder.RelatedOrderID.isNull())
		{
			COrder* pOrder = m_pMDB->m_OrderFactory->findByOrderID(theTriggerOrder.RelatedOrderID);
			if (pOrder != NULL)
			{
				CWriteableOrder theOrder;
				m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);
				theOrder.RelatedOrderID.clear();
				theOrder.TriggerOrderID.clear();
				theOrder.TriggerPrice.clear();
				theOrder.SLTriggerPrice.clear();
				theOrder.TPTriggerPrice.clear();
				theOrder.linkTriggerOrder((CTriggerOrder*)NULL);
				theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
				m_pMDB->m_OrderFactory->update(pOrder, &theOrder, pTransaction);
			}
		}
		//CFinishTriggerOrder* pFinishOrder = m_pMDB->m_FinishTriggerOrderFactory->add((CWriteableFinishTriggerOrder*)(&theTriggerOrder), pTransaction);
		//pTriggerOrderAction->linkTriggerOrder((CTriggerOrder*)pFinishOrder);
		//m_pMDB->m_TriggerOrderFactory->remove(pTriggerOrder, pTransaction);
		theTriggerOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_TriggerOrderFactory->update(pTriggerOrder, &theTriggerOrder, pTransaction);
		break;
	}
	case AF_Trigger:
	{
		if (pTriggerOrder->TriggerStatus == TS_TriggerSuccess)
			m_pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);
		if (theTriggerOrder.RiskBefore)
		{
			COrder* pOrder = theTriggerOrder.getOrder();
			if (pOrder == NULL)
			{
				COrder* pOrder = m_pMDB->m_OrderFactory->findByOrderID(theTriggerOrder.RelatedOrderID);
				if (pOrder == NULL)
					break;
			}
			CWriteableOrder theOrder;
			m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);

			if (!theTriggerOrder.CloseSLTriggerPrice.isNull() || !theTriggerOrder.CloseTPTriggerPrice.isNull() || !theTriggerOrder.CloseTriggerPrice.isNull())
			{
				CWriteableTriggerOrder newTriggerOrder;
				memcpy(&newTriggerOrder, &theTriggerOrder, sizeof(CWriteableTriggerOrder));
				newTriggerOrder.SLPrice = theTriggerOrder.CloseSLPrice;
				newTriggerOrder.SLTriggerPrice = theTriggerOrder.CloseSLTriggerPrice;
				newTriggerOrder.TPPrice = theTriggerOrder.CloseTPPrice;
				newTriggerOrder.TPTriggerPrice = theTriggerOrder.CloseTPTriggerPrice;
				newTriggerOrder.TriggerPrice = theTriggerOrder.CloseTriggerPrice;
				newTriggerOrder.Price = theTriggerOrder.ClosePrice;
				newTriggerOrder.OrderPriceType = theTriggerOrder.CloseOrderPriceType;
				newTriggerOrder.RelatedOrderID = pOrder->OrderID;
				newTriggerOrder.Remark = "2";//条件单开仓止盈止损，触发之后生成的止盈止损单 remark要等于2
				newTriggerOrder.BusinessType = BT_R3;
				newTriggerOrder.IsCrossMargin = theTriggerOrder.IsCrossMargin;
				// 清空三个Close触发价，避免循环触发
				newTriggerOrder.CloseSLTriggerPrice.clear();
				newTriggerOrder.CloseTPTriggerPrice.clear();
				newTriggerOrder.CloseTriggerPrice.clear();
				CTransactionSavePoint* pBeforeTransaction = CTransactionSavePoint::alloc(pTransaction);
				if (!insertCloseTriggerOrder(&theOrder, &newTriggerOrder, pTransaction))
					pTransaction->rollbackToSavePoint(pBeforeTransaction);
			}
			theOrder.Tradable = true;
			theOrder.TriggerPrice = pTriggerOrderAction->TriggerPrice;
			theOrder.SLTriggerPrice = theTriggerOrder.SLTriggerPrice;
			theOrder.TPTriggerPrice = theTriggerOrder.TPTriggerPrice;
			theOrder.IsCrossMargin = theTriggerOrder.IsCrossMargin;
			theOrder.DeriveDetail = pTriggerOrderAction->TriggerDetail;
			theTriggerOrder.TriggerPrice = pTriggerOrderAction->TriggerPrice;
			theTriggerOrder.TriggerDetail = pTriggerOrderAction->TriggerDetail;
			theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_OrderFactory->update(pOrder, &theOrder, pTransaction);
			if (needTryTrade(pOrder))
			{
				m_Wedge->tryTradeAfterOrderInsert(pOrder);
				m_Wedge->doAfterOrderChange(&theOrder);
				theTriggerOrder.TriggerStatus = TS_TriggerSuccess;
			}
		}
		else
		{
			CWriteableOrder theOrder;
			theOrder.init(m_pMDB);
			CopyInputOrderEntity(&theOrder, &theTriggerOrder);
			theOrder.Tradable = true;
			theOrder.DeriveSource = DS_FromTriggerOrder;
			theOrder.TriggerPrice = pTriggerOrderAction->TriggerPrice;
			theOrder.SLTriggerPrice = theTriggerOrder.SLTriggerPrice;
			theOrder.TPTriggerPrice = theTriggerOrder.TPTriggerPrice;
			theOrder.RelatedOrderID = theTriggerOrder.OrderID;
			theOrder.BusinessType = theTriggerOrder.BusinessType;
			theOrder.BusinessValue = theTriggerOrder.OrderID;
			theOrder.IsCrossMargin = theTriggerOrder.IsCrossMargin;
			theOrder.Leverage = theTriggerOrder.Leverage;
			theOrder.DeriveDetail = pTriggerOrderAction->TriggerDetail;
			theTriggerOrder.TriggerPrice = pTriggerOrderAction->TriggerPrice;
			theTriggerOrder.TriggerDetail = pTriggerOrderAction->TriggerDetail;
			//由于Action的Stream没有包含TP/SLTriggerPrice,仅仅是在前面用止盈/止损触发价赋值给TriggerPrice
			//导致这里还需要判断到底是止盈还是止损触发的
			if (pTriggerOrderAction->TriggerPrice == theTriggerOrder.SLTriggerPrice)
			{
				theOrder.DeriveSource = DS_FromTriggerOrder_SL;
				if (!theTriggerOrder.SLPrice.isNull())
				{
					theOrder.OrderPriceType = OPT_LimitPrice;
					theOrder.Price = theTriggerOrder.SLPrice;
				}
			}

			if (pTriggerOrderAction->TriggerPrice == theTriggerOrder.TPTriggerPrice)
			{
				theOrder.DeriveSource = DS_FromTriggerOrder_TP;
				if (!pTriggerOrder->TPPrice.isNull())
				{
					theOrder.OrderPriceType = OPT_LimitPrice;
					theOrder.Price = theTriggerOrder.TPPrice;
				}
			}

			//平仓止盈止损判断是否是点差单; ===临时判断代码,后续开放客户端 OPT_CFDPrice 类型可以删除===
			// ByCost类型不应该走点差，判断逻辑由TriggerOrder改为Order
			if (theOrder.OrderPriceType == OPT_TenLevelPrice)
			{
				// CMember* pMember = theOrder.getMember();
				// if (pMember == NULL)
				// {
				// 	pMember = m_pMDB->m_MemberFactory->findByMemberID(theOrder.MemberID);
				// 	if (pMember == NULL)
				// 		return m_pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND, "[%s]", theOrder.MemberID.getValue());
				// 	theOrder.linkMember(pMember);
				// }
				// if (!pMember->CFDGrade.isNull())
				// {
				// 	CCFDRate* rate = getCFDRate(m_pMDB, pMember->CFDGrade.getString().c_str(), pTriggerOrder->ExchangeID, pTriggerOrder->InstrumentID);
				// 	if (rate != NULL)
				// 		theOrder.OrderPriceType = OPT_CFDPrice;
				// }
				theOrder.OrderPriceType = OPT_CFDPrice;
			}

			CTransactionSavePoint* pBeforeTransaction = CTransactionSavePoint::alloc(pTransaction);
			if (!theTriggerOrder.CloseSLTriggerPrice.isNull() || !theTriggerOrder.CloseTPTriggerPrice.isNull() || !theTriggerOrder.CloseTriggerPrice.isNull())
			{
				CWriteableTriggerOrder newTriggerOrder;
				memcpy(&newTriggerOrder, &theTriggerOrder, sizeof(CWriteableTriggerOrder));
				newTriggerOrder.SLPrice = theTriggerOrder.CloseSLPrice;
				newTriggerOrder.SLTriggerPrice = theTriggerOrder.CloseSLTriggerPrice;
				newTriggerOrder.TPPrice = theTriggerOrder.CloseTPPrice;
				newTriggerOrder.TPTriggerPrice = theTriggerOrder.CloseTPTriggerPrice;
				newTriggerOrder.TriggerPrice = theTriggerOrder.CloseTriggerPrice;
				newTriggerOrder.Price = theTriggerOrder.ClosePrice;
				newTriggerOrder.OrderPriceType = theTriggerOrder.CloseOrderPriceType;
				newTriggerOrder.Remark = "2";//条件单开仓止盈止损，触发之后生成的止盈止损单 remark要等于2
				newTriggerOrder.BusinessType = BT_R3;
				newTriggerOrder.IsCrossMargin = theTriggerOrder.IsCrossMargin;
				newTriggerOrder.Leverage = theTriggerOrder.Leverage;
				// 清空三个Close触发价，避免循环触发
				newTriggerOrder.CloseSLTriggerPrice.clear();
				newTriggerOrder.CloseTPTriggerPrice.clear();
				newTriggerOrder.CloseTriggerPrice.clear();
				//触发单插入失败，不能影响后续的订单操作
				if (!insertCloseTriggerOrder(&theOrder, &newTriggerOrder, pTransaction))
				{
					char* errMsg = NULL;
					int errorCode = 0;
					m_pErrorEngine->getLastError(&errorCode, &errMsg);
					theTriggerOrder.ErrorNo = errorCode;
					theTriggerOrder.ErrorMsg = "InsertTriggerOrderError:";
					theTriggerOrder.ErrorMsg += errMsg;
					pTransaction->rollbackToSavePoint(pBeforeTransaction);
				}
			}
			//订单失败不能影响TriggerOrder的触发动作
			if (insertOrder(&theOrder, pTransaction))
			{
				theTriggerOrder.TriggerStatus = TS_TriggerSuccess;
				theTriggerOrder.RelatedOrderID = theOrder.OrderID;
				theTriggerOrder.Leverage = theOrder.Leverage;
				theTriggerOrder.PositionID = theOrder.PositionID;
			}
			else
			{
				pTransaction->rollbackToSavePoint(pBeforeTransaction);
				char* errMsg = NULL;
				int errorCode = 0;
				m_pErrorEngine->getLastError(&errorCode, &errMsg);

				if (errorCode != ERROR_NONE)
				{
					theTriggerOrder.ErrorNo = errorCode;
					theTriggerOrder.ErrorMsg = errMsg;
					theTriggerOrder.TriggerStatus = TS_TriggerFailed;
					//在非限价单的情况下，需要得出价格以便查询具体哪个价格导致的错误
					if (theTriggerOrder.Price != theOrder.Price)
						theTriggerOrder.Price = theOrder.Price;
				}
			}
			// 触发成功之后重新赋值，失败则保留之前的杠杆倍数
			// theTriggerOrder.Leverage = theOrder.Leverage;
		}
		//CFinishTriggerOrder* pFinishOrder = m_pMDB->m_FinishTriggerOrderFactory->add((CWriteableFinishTriggerOrder*)(&theTriggerOrder), pTransaction);
		//pTriggerOrderAction->linkTriggerOrder((CTriggerOrder*)pFinishOrder);	
		theTriggerOrder.TriggerTime = g_nCurrentTime;
		theTriggerOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_TriggerOrderFactory->update(pTriggerOrder, &theTriggerOrder, pTransaction);
		break;
	}
	case AF_Active:
	{
		break;
	}
	case AF_Modify:
	{
		// 触发价需要重新计算，清空原有的
		theTriggerOrder.TriggerPrice.clear();
		theTriggerOrder.SLTriggerPrice.clear();
		theTriggerOrder.TPTriggerPrice.clear();

		if (!theTriggerOrder.RiskBefore)
		{
			if (!pTriggerOrderAction->Price.isNull())
				theTriggerOrder.Price = pTriggerOrderAction->Price;
			if (!pTriggerOrderAction->Volume.isNull())
				theTriggerOrder.Volume = pTriggerOrderAction->Volume;
			if (!pTriggerOrderAction->Cost.isNull())
				theTriggerOrder.Cost = pTriggerOrderAction->Cost;
		}
		if (!pTriggerOrderAction->TriggerPrice.isNull())
			theTriggerOrder.TriggerPrice = pTriggerOrderAction->TriggerPrice;
		if (!pTriggerOrderAction->SLTriggerPrice.isNull())
			theTriggerOrder.SLTriggerPrice = pTriggerOrderAction->SLTriggerPrice;
		if (!pTriggerOrderAction->TPTriggerPrice.isNull())
			theTriggerOrder.TPTriggerPrice = pTriggerOrderAction->TPTriggerPrice;
		if (!pTriggerOrderAction->SLPrice.isNull())
			theTriggerOrder.SLPrice = pTriggerOrderAction->SLPrice;
		if (!pTriggerOrderAction->TPPrice.isNull())
			theTriggerOrder.TPPrice = pTriggerOrderAction->TPPrice;
		if (!pTriggerOrderAction->TriggerValue.isNull())
			theTriggerOrder.TriggerValue = pTriggerOrderAction->TriggerValue;
		if (!pTriggerOrderAction->TriggerPriceType.isNull())
			theTriggerOrder.TriggerPriceType = pTriggerOrderAction->TriggerPriceType;

		if (!theTriggerOrder.RelatedOrderID.isNull())
		{
			COrder* pOrder = m_pMDB->m_OrderFactory->findByOrderID(theTriggerOrder.RelatedOrderID);
			if (pOrder != NULL)
			{
				if (!triggerOrderPriceCheck(&theTriggerOrder, &(pOrder->Price)))
					return NULL;
				CWriteableOrder theOrder;
				m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);
				theOrder.TriggerPrice = theTriggerOrder.TriggerPrice;
				theOrder.SLTriggerPrice = theTriggerOrder.SLTriggerPrice;
				theOrder.TPTriggerPrice = theTriggerOrder.TPTriggerPrice;
				theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
				m_pMDB->m_OrderFactory->update(pOrder, &theOrder, pTransaction);
			}
		}
		else
		{
			if (!triggerOrderPriceCheck(&theTriggerOrder))
				return NULL;
		}
		m_pMDB->m_TriggerOrderFactory->update(pTriggerOrder, &theTriggerOrder, pTransaction);
		break;
	}
	default:
	{
	}
	}
	return pTriggerOrder;
}

bool CTradeService::insertOrder(CWriteableOrder* pOrder, CTransaction* pTransaction)
{
	pOrder->m_pMDB = m_pMDB;
	//开始进行OrderID的赋值，为了确保反演不一致的情况下，撤单ID就乱了
	//如果预先给的话，Service的不一致不会影响OrderID的撤单
	pOrder->OrderID = m_gOrderSequence.getNextValueByString();

	if (pOrder->LocalID.isNull())
		pOrder->LocalID = pOrder->OrderID.getValue();

	//pOrder->VolumeRemain = pOrder->Volume;
	//pOrder->OrderStatus = OS_NoTraded;
	//pOrder->TimeSortNo = m_gSortSequence.getNextValueByDouble();
	//pOrder->UpdateTime = g_nCurrentTime;
	//pOrder->InsertTime = g_nCurrentTime;
	//m_pMDB->m_OrderFactory->add(pOrder,pTransaction);
	//return true;

	//检查用户的当前Order报单量是否超过MaxOrderCount限定值
	CMember* pMember = pOrder->getMember();
	if (pMember == NULL)
		return m_pErrorEngine->reportError(ERROR_USER_NOT_FOUND, "[%s]", pOrder->MemberID.getValue());

	pOrder->Priority = pOrder->getMember()->Priority;
	if (pOrder->Priority.isNull())
		pOrder->Priority = 100;

	//if (!pUser->OpenOrders.isNull() && pUser->OpenOrders >= g_nMaxOpenOrders)
	//	return m_pErrorEngine->reportError(ERROR_ORDER_LIMIT_EXCEED,"[%d]>=[%d]", pUser->OpenOrders.getValue(), g_nMaxOpenOrders);

	//if (!pUser->OpenPositions.isNull() && pUser->OpenPositions >= g_nMaxOpenPositions)
	//{
	//	CPosition *pPosition = getPosition(m_pMDB, pOrder);
	//	if (pPosition == NULL) // 当前订单会导致新增持仓个数
	//		return m_pErrorEngine->reportError(ERROR_POSITION_COUNT_LIMIT_EXCEED, "PositionCountLimit[%d]", g_nMaxOpenPositions);
	//}

	CMarketOrder* pBuyMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(pOrder->ExchangeID, pOrder->InstrumentID);
	if (pBuyMarketOrder != NULL)
		pOrder->BidPrice1ByInsert = pBuyMarketOrder->Price;

	CMarketOrder* pSellMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(pOrder->ExchangeID, pOrder->InstrumentID);
	if (pSellMarketOrder != NULL)
		pOrder->AskPrice1ByInsert = pSellMarketOrder->Price;

	//takerskip计算
	CLiquidity* pLiquidity = NULL;
	if (pMember->Priority == 100)
	{
		//		pLiquidity = m_pMDB->m_LiquidityFactory->findByLiquidity(pMember->LiquidityGrade, pOrder->getInstrument()->LiquidityGroup.getValue());
		//		if (pLiquidity == NULL)
		pLiquidity = m_pMDB->m_LiquidityFactory->findByLiquidity(pMember->LiquidityGrade, pOrder->InstrumentID.getValue());

		if (pLiquidity == NULL)
		{
			pLiquidity = m_pMDB->m_LiquidityFactory->findByLiquidity(pMember->LiquidityGrade, defaultInstrumentID.getValue());
			if (pLiquidity == NULL)
			{
				pLiquidity = m_pMDB->m_LiquidityFactory->findByLiquidity(defaultMemberID.getValue(), pOrder->InstrumentID.getValue());
				if (pLiquidity == NULL)
				{
					pLiquidity = m_pMDB->m_LiquidityFactory->findByLiquidity(defaultMemberID.getValue(), defaultInstrumentID.getValue());
				}
			}
		}

		if (pLiquidity != NULL)
		{
			CInstrument* pInstrument = pOrder->getInstrument();
			CMarketData* pMarketData = pInstrument->getMarketData();
			//CMarketData *pMarketData = pOrder->getInstrument()->getMarketData();
			CPriceType BasePrice = pMarketData->MarkedPrice;
			switch (pLiquidity->BasePriceType)
			{
			case SPT_Bid1Ask1:
			{
				if (!pOrder->BidPrice1ByInsert.isNull() && !pOrder->AskPrice1ByInsert.isNull())
					BasePrice = (pOrder->BidPrice1ByInsert.getValue() + pOrder->AskPrice1ByInsert.getValue()) / 2;
				break;
			}
			case SPT_LastPrice:
			{
				BasePrice = pMarketData->LastPrice;
				break;
			}
			}
			if (!BasePrice.isNull())
			{
				if (pOrder->Direction == D_Buy)
				{
					if (pOrder->OffsetFlag == OF_Open)
						BasePrice = BasePrice * (1 + pLiquidity->BuyOpen);
					else
						BasePrice = BasePrice * (1 + pLiquidity->BuyClose);

					//比较订单和BasePrice大小
					if (!pOrder->Price.isNull() && BasePrice >= pOrder->Price)
						BasePrice = pOrder->Price;

					COrderIterator* pCountPartyIterator = m_pOrderBookList->getOrderIterator(pOrder->getInstrument(), D_Sell);
					COrder* pCountParty = pCountPartyIterator->next();
					while (pCountParty != NULL)
					{
						if (pCountParty->Price > BasePrice)
							break;

						if (pCountParty->Priority > 0)
						{
							pCountParty = pCountPartyIterator->next();
							continue;
						}

						CTransaction theTransaction;
						CWriteableOrderAction theOrderAction;
						theOrderAction.init();
						theOrderAction.OrderID = pCountParty->OrderID;
						theOrderAction.ActionFlag = AF_Delete;
						theOrderAction.linkMember(pMember);
						theOrderAction.linkOrder(pCountParty);
						pCountParty = pCountPartyIterator->next();
						if (orderAction(&theOrderAction, &theTransaction))
							theTransaction.commit();
						else
							theTransaction.rollback();
					}
					pCountPartyIterator->free();
				}
				else
				{
					if (pOrder->OffsetFlag == OF_Open)
						BasePrice = BasePrice * (1 - pLiquidity->BuyOpen);
					else
						BasePrice = BasePrice * (1 - pLiquidity->BuyClose);

					if (!pOrder->Price.isNull() && BasePrice <= pOrder->Price)
						BasePrice = pOrder->Price;

					COrderIterator* pCountPartyIterator = m_pOrderBookList->getOrderIterator(pOrder->getInstrument(), D_Buy);
					COrder* pCountParty = pCountPartyIterator->next();
					while (pCountParty != NULL)
					{
						if (pCountParty->Price < BasePrice)
							break;

						if (pCountParty->Priority > 0)
						{
							pCountParty = pCountPartyIterator->next();
							continue;
						}

						CTransaction theTransaction;
						CWriteableOrderAction theOrderAction;
						theOrderAction.init();
						theOrderAction.OrderID = pCountParty->OrderID;
						theOrderAction.ActionFlag = AF_Delete;
						theOrderAction.linkMember(pMember);
						theOrderAction.linkOrder(pCountParty);
						pCountParty = pCountPartyIterator->next();
						if (orderAction(&theOrderAction, &theTransaction))
							theTransaction.commit();
						else
							theTransaction.rollback();
					}
					pCountPartyIterator->free();
				}
			}
		}
	}

	//检查Leverage参数，这里填充CPositionActionField来设置杠杆.
	//if ((!field.TradeUnitID.isNull()) && (field.TradeUnitID != "default")) {
	//	if ((!field.Leverage.isNull()) && (field.Leverage > 0) && (field.OffsetFlag == OF_Open))
		//	if ((!field.TradeUnitID.isNull()) && (field.TradeUnitID != "default")) {
	//			if ((!field.Leverage.isNull()) && (field.Leverage > 0) && (field.OffsetFlag == OF_Open))
	//if (!pOrder->Leverage.isNull())
	//{
	//	CPositionActionField actionField;
	//	actionField.ActionType = PAT_ChangeLeverage;
	//	actionField.ExchangeID = pOrder->ExchangeID;
	//	actionField.InstrumentID = pOrder->InstrumentID;
	//	actionField.MemberID = pOrder->MemberID;
	//	actionField.AccountID = pOrder->AccountID;
	//	actionField.TradeUnitID = pOrder->TradeUnitID;
	//	actionField.Amount = pOrder->Leverage;
	//	CPosition* pPosition = getPosition(m_pMDB, pOrder);
	//	pOrder->linkPosition(pPosition);
	//	actionField.PosiDirection = pOrder->PosiDirection;

	//	bool ret = ReqPositionAction(&actionField, NULL, pTransaction);
	//	if (!ret)
	//		return false;
	//}
	//这里即便成功了也不做commit,因为下单失败，杠杆设置也不需要生效。在后面一并commit.

	// 下单接口中传入了CopyMemberID值
	if (!pOrder->CopyMemberID.isNull()) {
		if (pOrder->MemberID == pOrder->CopyMemberID) {
			REPORT_EVENT(LOG_DEBUG, "LeaderOrder V3.0", "MID:%s, CMID:%s, OrderID:%s",
				pOrder->MemberID.getValue(), pMember->CopyMemberID.getValue(), pOrder->OrderID.getValue());
			pOrder->CopyMemberID = pOrder->MemberID;
			pOrder->CopyOrderID = pOrder->OrderID;
		}
		else if (pOrder->CopyMemberID == "0") {
			REPORT_EVENT(LOG_DEBUG, "Order V3.0", "MID:%s, CMID:%s, OrderID:%s",
				pOrder->MemberID.getValue(), pMember->CopyMemberID.getValue(), pOrder->OrderID.getValue());
		}
		else {
			REPORT_EVENT(LOG_DEBUG, "FollowerOrder", "MID:%s, CMID:%s, OrderID:%s, CopyOrderID:%s",
				pOrder->MemberID.getValue(), pOrder->CopyMemberID.getValue(), pOrder->OrderID.getValue(), pOrder->CopyOrderID.getValue());
			//检查跟单者的CopyMemberID是否在Member表中
			CMember* leaderMember = m_pMDB->m_MemberFactory->findByMemberID(pOrder->CopyMemberID);
			if (leaderMember == NULL) {
				REPORT_EVENT(LOG_ERROR, "Leader MID error, not exist", "CMID:%s", pOrder->CopyMemberID.getValue());
				pOrder->CopyMemberID.clear();
				pOrder->CopyOrderID.clear();
			}
		}
	}
	else {
		if (!pMember->CopyMemberID.isNull()) {
			if (pMember->CopyMemberID == pMember->MemberID) {
				REPORT_EVENT(LOG_DEBUG, "LeaderOrder V2.0", "MID:%s, CMID:%s, OrderID:%s",
					pOrder->MemberID.getValue(), pMember->CopyMemberID.getValue(), pOrder->OrderID.getValue());
				//添加交易对判断逻辑
				if ((pOrder->InstrumentID == "BTCUSDT") || (pOrder->InstrumentID == "ETHUSDT")) {
					pOrder->CopyMemberID = pOrder->MemberID;
					pOrder->CopyOrderID = pOrder->OrderID;
				}
			}
			else {
				REPORT_EVENT(LOG_ERROR, "Leader V2.0 CMID error", "MID:%s, CMID:%s",
					pOrder->MemberID.getValue(), pMember->CopyMemberID.getValue());
			}
		}
	}

	//如果是全平，那么如果有撤单需要先处理撤单
	if (pOrder->OffsetFlag == OF_CloseAll || pOrder->OffsetFlag == OF_ForceClose)
	{
		CWriteableOrderAction theOrderAction;
		theOrderAction.init();
		theOrderAction.ActionFlag = AF_Delete;
		//	theOrderAction.UserID = pOrder->UserID;
		theOrderAction.OrderLocalID.clear();
		theOrderAction.MemberID = pOrder->MemberID;
		theOrderAction.linkMember(pMember);
		COrder* pOldOrder = m_pMDB->m_OrderFactory->findFirstByAccountID(pOrder->AccountID);
		while (pOldOrder != NULL)
		{
			if (pOldOrder->ExchangeID == pOrder->ExchangeID && pOldOrder->InstrumentID == pOrder->InstrumentID && pOldOrder->TradeUnitID == pOrder->TradeUnitID)
			{
				if (pOldOrder->Direction == pOrder->Direction)
				{
					if (pOldOrder->OffsetFlag == OF_Open && pOldOrder->getPosition()->PosiDirection != PD_Net)
					{
						pOldOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
						continue;
					}
					theOrderAction.OrderID = pOldOrder->OrderID;
					theOrderAction.linkOrder(pOldOrder);
					pOldOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
					if (!orderAction(&theOrderAction, pTransaction))
					{
						return false;
					}
					continue;
				}
			}
			pOldOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
		}
	}

	// 市价委托单，转换为点差单
	if (pOrder->OrderPriceType == OPT_TenLevelPrice)
	{
		// CMember* pMember = pOrder->getMember();
		// if (pMember == NULL)
		// {
		// 	pMember = m_pMDB->m_MemberFactory->findByMemberID(pOrder->MemberID);
		// 	if (pMember == NULL)
		// 		return m_pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND, "[%s]",pOrder->MemberID.getValue());
		// 	pOrder->linkMember(pMember);
		// }
		// if (!pMember->CFDGrade.isNull())
		// {
		// 	CCFDRate* rate = getCFDRate(m_pMDB, pMember->CFDGrade.getString().c_str(), pOrder->ExchangeID, pOrder->InstrumentID);
		// 	if (rate != NULL)
		// 		pOrder->OrderPriceType = OPT_CFDPrice;
		// }
		pOrder->OrderPriceType = OPT_CFDPrice;
	}

	///检查是否可以插入报单
	if (!m_Wedge->insertOrder(pOrder, pTransaction))
	{
		char* errMsg = NULL;
		int errorCode = 0;
		m_pErrorEngine->getLastError(&errorCode, &errMsg);

		//为了失败，资金不足的情况下，由于定价不合理造成的，可以再来一次
		if (errorCode == ERROR_INSUFFICIENT_MONEY)
		{
			CInstrument* pInstrument = pOrder->getInstrument();
			if (pInstrument != NULL && pInstrument->ProductClass == PC_Margin)
			{
				switch (pOrder->OrderPriceType)
				{
				case OPT_AnyPrice:
				case OPT_FiveLevelPrice:
				case OPT_TenLevelPrice:
				{
					pTransaction->rollback();
					pOrder->OrderPriceType = OPT_BestPrice;
					pOrder->OrderType = OT_Normal;
					pOrder->FrozenFee = 0.0;
					pOrder->FrozenMargin = 0.0;
					pOrder->FrozenMoney = 0.0;
					pOrder->linkPosition((CPosition*)NULL);
					if (!m_Wedge->insertOrder(pOrder, pTransaction))
						return false;
					break;
				}
				default:
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	////尝试进行成交
	if (needTryTrade(pOrder))
	{
		COrder* pMDBOrder = m_pMDB->m_OrderFactory->findByOrderID(pOrder->OrderID);
		if (pMDBOrder != NULL) {
			m_Wedge->tryTradeAfterOrderInsert(pMDBOrder);
			m_Wedge->doAfterOrderChange(pMDBOrder);
			//if ((!pMDBOrder->VolumeTraded.isNull()) && (pMDBOrder->VolumeTraded > 0.0))
		//	{
				//开始把Postion和Account进行资金事务处理，只需要处理一次，不需要连续处理
			pTransaction->addResource(COrderResource::alloc(UPDATE_ACTION, m_pMDB->m_OrderFactory, pMDBOrder));
			if (pMDBOrder->getPosition() != NULL)
			{
				pTransaction->addResource(CPositionResource::alloc(UPDATE_ACTION, m_pMDB->m_PositionFactory, pMDBOrder->getPosition()));
				if (pMDBOrder->getPosition()->getAccount() != NULL)
					pTransaction->addResource(CAccountResource::alloc(UPDATE_ACTION, m_pMDB->m_AccountFactory, pMDBOrder->getPosition()->getAccount()));
				else
					REPORT_EVENT(LOG_ERROR, "insertOrder", "pMDBOrder->getPosition()->getAccount is NULL");
			}
			else
				REPORT_EVENT(LOG_ERROR, "insertOrder", "pMDBOrder->getPosition is NULL");
			//	}
			memcpy(pOrder, pMDBOrder, sizeof(CWriteableOrder));
		}
	}

	return true;
}

bool CTradeService::ReqPositionCloseBatch(CPositionActionField* pPositionAction, CTransaction* pTransaction)
{
	CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByAccountID(pPositionAction->AccountID);
	int nCount = 0;
	while (pPosition != NULL)
	{
		if (!pPositionAction->ExchangeID.isNull() && pPositionAction->ExchangeID != pPosition->ExchangeID)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		if (!pPositionAction->InstrumentID.isNull() && pPositionAction->InstrumentID != pPosition->InstrumentID)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		if (!pPositionAction->TradeUnitID.isNull() && pPositionAction->TradeUnitID != pPosition->TradeUnitID)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		if (!pPositionAction->AccountID.isNull() && pPositionAction->AccountID != pPosition->AccountID)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		//if (!pPositionAction->ProductGroup.isNull() && pPositionAction->ProductGroup != pPosition->ProductGroup)
		//{
		//	pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
		//	continue;
		//}

		if (pPosition->Position == 0.0)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		CWriteableOrder theOrder;
		theOrder.init(m_pMDB);
		theOrder.MemberID = pPositionAction->MemberID;
		//		theOrder.UserID = pPositionAction->UserID;
		theOrder.LocalID = pPositionAction->LocalID;
		if (pPositionAction->ActionType == PAT_CloseAll)
			theOrder.OffsetFlag = OF_CloseAll;
		else if (pPositionAction->ActionType == PAT_CloseMax)
			theOrder.OffsetFlag = OF_CloseMax;
		theOrder.OrderPriceType = g_cOrderPriceType;
		theOrder.OrderType = OT_Normal;
		//theOrder.TimeCondition = TC_GTC;
		theOrder.Tradable = true;
		theOrder.Direction = getReverseDirection(pPosition);
		theOrder.PosiDirection = pPosition->PosiDirection;
		theOrder.ExchangeID = pPosition->ExchangeID;
		theOrder.InstrumentID = pPosition->InstrumentID;
		theOrder.MemberID = pPosition->MemberID;
		theOrder.AccountID = pPosition->AccountID;
		theOrder.TradeUnitID = pPosition->TradeUnitID;
		theOrder.IsCrossMargin = pPosition->IsCrossMargin;
		theOrder.linkPosition(pPosition);
		//theOrder.linkAccount(pPosition->getAccount());

		//Position可能会被删除，在删除之前，得到下一个地址
		pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();

		CTransaction theTransaction;
		bool success = insertOrder(&theOrder, &theTransaction);
		if (success)
		{
			nCount++;
			theTransaction.commit();
		}
		else
		{
			//char* errMsg = NULL;
			//int errorCode = 0;
			//m_pErrorEngine->getLastError(&errorCode, &errMsg);
			//REPORT_EVENT(LOG_ERROR, "ReqPositionCloseBatch", "Position[%s_%s_%s_%s_%c],ErrorID[%d],ErrorMsg[%s]", theOrder.MemberID.getValue(), theOrder.TradeUnitID.getValue(),
			//	theOrder.ExchangeID.getValue(), theOrder.InstrumentID.getValue(), theOrder.PosiDirection.getValue(), errorCode, errMsg);
			theTransaction.rollback();
		}
	}

	if (nCount == 0)
		return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE, "ZeroPositonClosed");

	return true;
}

bool CTradeService::insertCloseTriggerOrder(CWriteableOrder* pOrder, CWriteableTriggerOrder* pTriggerOrder, CTransaction* pTransaction)
{
	pTriggerOrder->OffsetFlag = OF_CloseMax;
	//如果没有标记触发其的OrderID，这里应该是order会在后面加上，取后面的id
	if (pTriggerOrder->RelatedOrderID.isNull())
		pTriggerOrder->RelatedOrderID = m_gOrderSequence.touchNextValueByString(2.0);

	if (pTriggerOrder->TriggerPriceType.isNull())
		pTriggerOrder->TriggerPriceType.init();

	//TriggerOrder的OrderPriceType去掉，
	if (pTriggerOrder->OrderPriceType.isNull())
	{
		pTriggerOrder->OrderPriceType = g_cOrderPriceType;
		pTriggerOrder->Price.clear();
	}

	//Order买入，TriggerOrder反方向
	if (pOrder->Direction == D_Buy)
		pTriggerOrder->Direction = D_Sell;
	else
		pTriggerOrder->Direction = D_Buy;

	//订单止盈止损
	pTriggerOrder->TriggerOrderType = TOT_OrderTPSL;

	bool bSuccess = false;
	//不再使用OrderPriceType来判断，只要pOrder->Price不为空
	//if (pOrder->OrderPriceType == OPT_LimitPrice)
	if (!pOrder->Price.isNull())
		bSuccess = triggerOrderInsert(pTriggerOrder, TS_None, pTransaction, &(pOrder->Price));
	else
		bSuccess = triggerOrderInsert(pTriggerOrder, TS_None, pTransaction);

	if (!bSuccess)
		return false;

	pOrder->RelatedOrderID = pTriggerOrder->OrderID;
	pOrder->TriggerOrderID = pTriggerOrder->OrderID;
	pOrder->SLTriggerPrice = pTriggerOrder->SLTriggerPrice;
	pOrder->TPTriggerPrice = pTriggerOrder->TPTriggerPrice;
	//pOrder->TriggerPrice = pTriggerOrder->TriggerPrice;
	pOrder->BusinessValue = pTriggerOrder->OrderID;
	return true;
}

//bool CTradeService::insertCombOrder(CWriteableCombOrder* pCombOrder, CTransaction* pTransaction)
//{
//	pCombOrder->OrderID = m_gOrderSequence.getNextValueByString();
//
//	///检查是否可以插入组合报单
//	if (!m_Wedge->insertCombOrder(pCombOrder, pTransaction))
//		return false;
//
//	return true;
//}

//bool CTradeService::insertQuote(CWriteableQuote* pQuote, CTransaction* pTransaction)
//{
//	///删除原来的报价
//	CQuote* pLastQuote;
//
//	pLastQuote = m_pMDB->m_QuoteFactory->findByTradeUnitAndInstrument(pQuote->MemberID, pQuote->TradeUnitID, pQuote->ExchangeID, pQuote->InstrumentID);
//
//	if (pLastQuote != NULL)
//	{
//		CWriteableQuoteAction theQuoteAction;
//
//		theQuoteAction.init();
//		theQuoteAction.ExchangeID = pLastQuote->ExchangeID;
//		theQuoteAction.InstrumentID = pLastQuote->InstrumentID;
//		theQuoteAction.MemberID = pLastQuote->MemberID;
//		theQuoteAction.TradeUnitID = pLastQuote->TradeUnitID;
//		theQuoteAction.ActionFlag = AF_Delete;
//		theQuoteAction.UserID = pQuote->UserID;
//		theQuoteAction.linkQuote(pLastQuote);
//
//		CTransactionSavePoint* pBeforeCancelQuote = CTransactionSavePoint::alloc(pTransaction);
//		if (!quoteAction(&theQuoteAction, pTransaction))
//		{
//			//internalWarning("插入报价时删除原报价失败");
//			pTransaction->rollbackToSavePoint(pBeforeCancelQuote);
//		}
//	}
//
//	CTransactionSavePoint* pBeforeInsert = CTransactionSavePoint::alloc(pTransaction);
//
//	if (pQuote->InsertTime.isNull())
//		pQuote->InsertTime = g_nCurrentTime;
//
//	CWriteableOrder theOrder;
//
//	///公共的部分
//	theOrder.init();
//	theOrder.MemberID = pQuote->MemberID;
//	theOrder.TradeUnitID = pQuote->TradeUnitID;
//	theOrder.UserID = pQuote->UserID;
//	theOrder.ExchangeID = pQuote->ExchangeID;
//	theOrder.InstrumentID = pQuote->InstrumentID;
//	theOrder.OrderPriceType = OPT_LimitPrice;
//	theOrder.Volume = pQuote->Volume;
//	theOrder.TimeCondition = TC_GTC;
//	theOrder.OrderType = OT_Normal;
//	theOrder.LocalID = pQuote->LocalID;
//	//	theOrder.IsAutoCancle = false;
//	theOrder.DeriveSource = DS_FromQuote;
//	theOrder.Remark = pQuote->Remark;
//
//	///买方报单
//	theOrder.OrderID = "";
//	theOrder.Direction = D_Buy;
//	theOrder.OffsetFlag = pQuote->BuyOffsetFlag;
//	theOrder.Price = pQuote->BuyPrice;
//
//	if (insertOrder(&theOrder, pTransaction))
//	{
//		pQuote->BuyOrderID = theOrder.OrderID;
//
//		///卖方报单
//		theOrder.OrderID = "";
//		theOrder.Direction = D_Sell;
//		theOrder.OffsetFlag = pQuote->SellOffsetFlag;
//		theOrder.Price = pQuote->SellPrice;
//		if (insertOrder(&theOrder, pTransaction))
//		{
//			pQuote->SellOrderID = theOrder.OrderID;
//			///完成插入报价的操作
//			m_pMDB->m_QuoteFactory->add(pQuote, pTransaction);
//			return true;
//		}
//	}
//
//	///放弃此次报价插入操作，但是依然返回true，因为原来的报价还是要删除的
//	///叫用方应当根据QuoteSysID是否被设值来确定是否完成了报价插入工作
//	pTransaction->rollbackToSavePoint(pBeforeInsert);
//
//	return true;
//}

bool CTradeService::insertWashOnly(COTCTradeField* pOTCTrade, CTransaction* pTransaction)
{
	//移到前面来，防止合约找不到，放在下面没啥用
	CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pOTCTrade->ExchangeID, pOTCTrade->InstrumentID);
	if (pInstrument == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);

	CMarketData* pMarketData = pInstrument->getMarketData();
	if (pMarketData == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);

	if (pMarketData->InstrumentStatus != IS_Continous)
		return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS);

	pOTCTrade->DeriveSource = DS_FromOTCTrade_WashOnly;

	if (!pOTCTrade->Price.isTick(pInstrument->PriceTick))
		return m_pErrorEngine->reportError(ERROR_PRICE_NOT_ON_TICK);

	if (!pOTCTrade->Volume.isTick(pInstrument->VolumeTick))
		return m_pErrorEngine->reportError(ERROR_VOLUME_NOT_ON_TICK);

	if (pOTCTrade->Volume < pInstrument->MinOrderVolume)
		return m_pErrorEngine->reportError(ERROR_LessThanMinVolume, "[%s]<[%s]", pOTCTrade->Volume.getString().c_str(), pInstrument->MinOrderVolume.getString().c_str());

	if (pOTCTrade->Volume > pInstrument->MaxOrderVolume)
		return m_pErrorEngine->reportError(ERROR_OutOfMaxVolume, "[%s]>[%s]", pOTCTrade->Volume.getString().c_str(), pInstrument->MaxOrderVolume.getString().c_str());

	//如果是washonly订单，需要看订单簿是否满足要求
	CMarketOrder* pBuyMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(pOTCTrade->ExchangeID, pOTCTrade->InstrumentID);
	if (pBuyMarketOrder != NULL)
	{
		if (pBuyMarketOrder->Price > pOTCTrade->Price)
			pOTCTrade->Price = pBuyMarketOrder->Price;
		//return m_pErrorEngine->reportError(ERROR_PriceOutOfLowerLimit, "%.4lf>%.4lf", pBuyMarketOrder->Price.getValue(), pOTCTrade->Price.getValue());
	}

	CMarketOrder* pSellMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(pOTCTrade->ExchangeID, pOTCTrade->InstrumentID);
	if (pSellMarketOrder != NULL)
	{
		if (pSellMarketOrder->Price < pOTCTrade->Price)
			pOTCTrade->Price = pSellMarketOrder->Price;
		//return m_pErrorEngine->reportError(ERROR_PriceOutOfUpperLimit, "%.4lf<%.4lf", pSellMarketOrder->Price.getValue(), pOTCTrade->Price.getValue());
	}

	CWriteableMarketData theMarketData;
	m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);

	theMarketData.Volume += pOTCTrade->Volume.getValue() * 2;
	theMarketData.LastPrice = pOTCTrade->Price;
	//更新最高最低价
	if (theMarketData.OpenPrice.isNull())
		theMarketData.OpenPrice = pOTCTrade->Price;
	if ((theMarketData.HighestPrice.isNull()) || (theMarketData.HighestPrice < pOTCTrade->Price))
		theMarketData.HighestPrice = pOTCTrade->Price;
	if ((theMarketData.LowestPrice.isNull()) || (theMarketData.LowestPrice > pOTCTrade->Price))
		theMarketData.LowestPrice = pOTCTrade->Price;
	theMarketData.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	//getMDBTime(m_pMDB, &(theMarketData.UpdateTime), &(theMarketData.UpdateMilliSecond));

	CWriteableTrade theTrade;
	theTrade.init();
	theTrade.ExchangeID = pOTCTrade->ExchangeID;
	theTrade.InstrumentID = pOTCTrade->InstrumentID;
	//theTrade.ProductGroup = pInstrument->ProductGroup;
	//	theTrade.SettlementGroup = pInstrument->SettlementGroup;
		//	theTrade.ProductID = pInstrument->ProductID;
	theTrade.Price = pOTCTrade->Price;
	theTrade.Volume = pOTCTrade->Volume;
	if (pInstrument->IsInverse)
	{
		if (theTrade.Price <= 0.0)
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Price<=0.0");
		theTrade.Turnover = pInstrument->VolumeMultiple.getValue() * theTrade.Volume.getValue() / theTrade.Price.getValue();
	}
	else
	{
		theTrade.Turnover = fabs(pInstrument->VolumeMultiple.getValue() * theTrade.Volume.getValue() * theTrade.Price.getValue());
	}

	theTrade.DeriveSource = pOTCTrade->DeriveSource;
	///分别插入两张成交单

	theTrade.TradeID = m_gTradeSequence.getNextValueByString();
	theTrade.MatchRole = MR_Taker;
	if (pOTCTrade->TakerDirection == D_Buy)
		theTrade.Direction = D_Buy;
	else
		theTrade.Direction = D_Sell;
	theTrade.InsertTime = g_nCurrentTime;
	m_pMarketTradeTrigger->commitAdd(&theTrade);

	//K线只认买方，这里需要做一个特殊处理
	theTrade.Direction = D_Buy;
	m_pKLineTrigger->commitAdd(&theTrade);

	theMarketData.Turnover += theTrade.Turnover;

	m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData, pTransaction);
	return true;
}

//处理大宗交易/期转现成交对申报
bool CTradeService::insertOTCTrade(COTCTradeField* pOTCTrade, CTransaction* pTransaction)
{
	//移到前面来，防止合约找不到，放在下面没啥用
	CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pOTCTrade->ExchangeID, pOTCTrade->InstrumentID);
	if (pInstrument == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND, "%s:%s", pOTCTrade->ExchangeID.getValue(), pOTCTrade->InstrumentID.getValue());

	//设置成交类型,报入时,OTCType应该不应超出这三种情况
	switch (pOTCTrade->OTCType)
	{
	case OTCT_BlockTrade:
	{
		pOTCTrade->DeriveSource = DS_FromOTCTrade_BlockTrade;
		break;
	}
	case OTCT_EFP:
	{
		pOTCTrade->DeriveSource = DS_FromOTCTrade_EFP;
		break;
	}
	case OTCT_Liquidation:
	{
		pOTCTrade->DeriveSource = DS_FromOTCTrade_Liquidation;
		break;
	}
	case OTCT_WashOnly:
	{
		pOTCTrade->DeriveSource = DS_FromOTCTrade_WashOnly;
		break;
	}
	case OTCT_PositionMerge:
	{
		pOTCTrade->DeriveSource = DS_FromOTCTrade_PositionMerge;
		break;
	}
	case OTCT_ADL:
	{
		pOTCTrade->DeriveSource = DS_FromOTCTrade_ADL;
		break;
	}
	case OTCT_Private:
	{
		pOTCTrade->DeriveSource = DS_FromOTCTrade_Private;
		break;
	}
	default:
	{
		m_pErrorEngine->reportError(ERROR_OTC_TYPE_ERROR);
		return false;
	}
	}

	///生成买卖两笔报单，分别插入报单

	//	bool bInsertOrderOK = false;

	CWriteableOrder theWriteableBuyOrder;
	///公共的部分
	theWriteableBuyOrder.init(m_pMDB);
	theWriteableBuyOrder.ExchangeID = pOTCTrade->ExchangeID;
	theWriteableBuyOrder.InstrumentID = pOTCTrade->InstrumentID;
	theWriteableBuyOrder.Volume = pOTCTrade->Volume;
	theWriteableBuyOrder.OrderPriceType = OPT_LimitPrice;
	theWriteableBuyOrder.Price = pOTCTrade->Price;
	//	theWriteableBuyOrder.TimeCondition = TC_GTC;
	theWriteableBuyOrder.OrderType = OT_Normal;
	//	theOrder.ContingentCondition = CC_Immediately;
		//	theOrder.ForceCloseReason = FCR_NotForceClose;
//	theOrder.IsAutoCancle = false;
	theWriteableBuyOrder.Remark = pOTCTrade->Remark;
	//设置订单类型,报入时,OTCType应该不应超出这三种情况
	theWriteableBuyOrder.DeriveSource = pOTCTrade->DeriveSource;
	theWriteableBuyOrder.BusinessResult = pOTCTrade->SellMemberID;
	///买方报单
	theWriteableBuyOrder.MemberID = pOTCTrade->BuyMemberID;
	theWriteableBuyOrder.TradeUnitID = pOTCTrade->BuyTradeUnitID;
	//	theWriteableBuyOrder.UserID = pOTCTrade->UserID;
	theWriteableBuyOrder.AccountID = pOTCTrade->BuyAccountID;
	theWriteableBuyOrder.Direction = D_Buy;
	theWriteableBuyOrder.OffsetFlag = pOTCTrade->BuyOffsetFlag;
	theWriteableBuyOrder.TriggerPrice = pOTCTrade->TriggerPrice;
	theWriteableBuyOrder.Tradable = false;
	theWriteableBuyOrder.RelatedOrderID = m_gOrderSequence.touchNextValueByString(2);

	if (!insertOrder(&theWriteableBuyOrder, pTransaction))
		return false;


	CWriteableOrder theWriteableSellOrder;
	///卖方报单
	theWriteableSellOrder.init(m_pMDB);
	theWriteableSellOrder.ExchangeID = pOTCTrade->ExchangeID;
	theWriteableSellOrder.InstrumentID = pOTCTrade->InstrumentID;
	theWriteableSellOrder.Volume = pOTCTrade->Volume;
	theWriteableSellOrder.OrderPriceType = OPT_LimitPrice;
	theWriteableSellOrder.Price = pOTCTrade->Price;
	//	theWriteableSellOrder.TimeCondition = TC_GTC;
	theWriteableSellOrder.OrderType = OT_Normal;
	//	theOrder.ContingentCondition = CC_Immediately;
		//	theOrder.ForceCloseReason = FCR_NotForceClose;
//	theOrder.IsAutoCancle = false;
	theWriteableSellOrder.Remark = pOTCTrade->Remark;
	//设置订单类型,报入时,OTCType应该不应超出这三种情况
	theWriteableSellOrder.DeriveSource = pOTCTrade->DeriveSource;
	theWriteableSellOrder.BusinessResult = pOTCTrade->BuyMemberID;
	///买方报单
	theWriteableSellOrder.MemberID = pOTCTrade->SellMemberID;
	theWriteableSellOrder.TradeUnitID = pOTCTrade->SellTradeUnitID;
	//	theWriteableSellOrder.UserID = pOTCTrade->UserID;
	theWriteableSellOrder.AccountID = pOTCTrade->SellAccountID;
	theWriteableSellOrder.Direction = D_Sell;
	theWriteableSellOrder.OffsetFlag = pOTCTrade->SellOffsetFlag;
	theWriteableSellOrder.TriggerPrice = pOTCTrade->TriggerPrice;
	theWriteableSellOrder.Tradable = false;
	theWriteableSellOrder.RelatedOrderID = m_gOrderSequence.getCurrentValueByString();

	if (!insertOrder(&theWriteableSellOrder, pTransaction))
		return false;

	CWriteableTrade theTrade;

	///准备成交中的公共值
	theTrade.init();
	theTrade.ExchangeID = pOTCTrade->ExchangeID;
	theTrade.InstrumentID = pOTCTrade->InstrumentID;
	//theTrade.ProductGroup = pInstrument->ProductGroup;
	//	theTrade.SettlementGroup = pInstrument->SettlementGroup;
		//	theTrade.ProductID = pInstrument->ProductID;
	theTrade.Price = pOTCTrade->Price;
	theTrade.Volume = pOTCTrade->Volume;
	theTrade.DeriveSource = pOTCTrade->DeriveSource;
	//if (pOTCTrade->TakerDirection == D_Buy)
	//	theTrade.PriceSource = PS_Sell;
	//else
	//	theTrade.PriceSource = PS_Buy;

	///分别插入两张成交单
	theTrade.Direction = D_Buy;
	theTrade.MemberID = pOTCTrade->BuyMemberID;
	//	theTrade.TradeUnitID = pOTCTrade->BuyTradeUnitID;
	theTrade.OffsetFlag = pOTCTrade->BuyOffsetFlag;
	theTrade.AccountID = pOTCTrade->BuyAccountID;
	theTrade.OrderID = theWriteableBuyOrder.OrderID;//买单的系统号
	//将Trade中的UserID填大宗交易控制员的席位号
	//theTrade.UserID = pOTCTrade->BuyUserID;
//	theTrade.UserID = pOTCTrade->UserID;
//	theTrade.OrderLocalID = pOTCTrade->LocalID;
	theTrade.MatchRole = MR_None;
	theTrade.OrderPrice = theWriteableSellOrder.Price;
	theTrade.TriggerPrice = pOTCTrade->TriggerPrice;
	COrder* pBuyOrder = m_pMDB->m_OrderFactory->findByOrderID(theWriteableBuyOrder.OrderID);
	if (pBuyOrder->getPosition() == NULL)
		pBuyOrder->linkPosition();
	COrder* pSellOrder = m_pMDB->m_OrderFactory->findByOrderID(theWriteableSellOrder.OrderID);
	if (pSellOrder->getPosition() == NULL)
		pSellOrder->linkPosition();
	//CWriteableFinishOrder finishBuyOrder;
	//CWriteableFinishOrder finishSellOrder;

	theTrade.Leverage = pBuyOrder->getPosition()->Leverage;
	theTrade.FeeCurrency = pBuyOrder->FeeCurrency;
	theTrade.ClearCurrency = pBuyOrder->ClearCurrency;
	theTrade.PriceCurrency = pBuyOrder->PriceCurrency;
	m_pMDB->m_OrderFactory->retrieve(pBuyOrder, &theWriteableBuyOrder);
	insertSingleTrade(&theTrade, &theWriteableBuyOrder);

	//CopyOrderEntity(&finishBuyOrder, &theWriteableBuyOrder);
	theTrade.Turnover = 0.0;
	theTrade.Fee = 0.0;
	theTrade.CloseProfit = 0.0;

	theTrade.Direction = D_Sell;
	theTrade.MemberID = pOTCTrade->SellMemberID;
	//	theTrade.TradeUnitID = pOTCTrade->SellTradeUnitID;
	theTrade.OffsetFlag = pOTCTrade->SellOffsetFlag;
	theTrade.AccountID = pOTCTrade->SellAccountID;
	theTrade.OrderID = theWriteableSellOrder.OrderID;//卖单的系统号
	//将Trade中的UserID填大宗交易控制员的席位号
	//theTrade.UserID = pOTCTrade->SellUserID;
//		theTrade.UserID = pOTCTrade->UserID;
	//	theTrade.OrderLocalID = pOTCTrade->LocalID;
	theTrade.MatchRole = MR_None;
	theTrade.Leverage = pSellOrder->getPosition()->Leverage;
	//	theTrade.ClearCurrency = pSellOrder->Currency;
	theTrade.ClearCurrency = pSellOrder->ClearCurrency;
	theTrade.PriceCurrency = pSellOrder->PriceCurrency;
	theTrade.FeeCurrency = pSellOrder->FeeCurrency;

	m_pMDB->m_OrderFactory->retrieve(pSellOrder, &theWriteableSellOrder);
	insertSingleTrade(&theTrade, &theWriteableSellOrder);

	//设置TradeID
	pOTCTrade->TradeID = theTrade.TradeID;

	//!!!由于该规则被大宗交易和强平公用,所以最终成交对插入到不同的表
	//大宗交易成交插入OTCTrade表,强平成交插入ForceCloseTrade表,插入操作在CTradeServiceImp里实现
	///完成插入场外成交的操作
	//m_pMDB->m_OTCTradeFactory->add(pOTCTrade,pTransaction);
	//m_pMDB->m_FinishOrderFactory->add(&finishBuyOrder, pTransaction);
	//m_pMDB->m_OrderFactory->remove(pBuyOrder, pTransaction);

	theWriteableBuyOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_OrderFactory->update(pBuyOrder, &theWriteableBuyOrder, pTransaction);
	pTransaction->addResource(CPositionResource::alloc(UPDATE_ACTION, m_pMDB->m_PositionFactory, pBuyOrder->getPosition()));
	if (pBuyOrder->getPosition()->getAccount() != NULL)
		pTransaction->addResource(CAccountResource::alloc(UPDATE_ACTION, m_pMDB->m_AccountFactory, pBuyOrder->getPosition()->getAccount()));
	else
		REPORT_EVENT(LOG_ERROR, "insertOTCTrade", "pBuyOrder->getPosition()->getAccount() is NULL");

	//m_pMDB->m_FinishOrderFactory->add(&finishSellOrder, pTransaction);
	//m_pMDB->m_OrderFactory->remove(pSellOrder, pTransaction);
	theWriteableSellOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_OrderFactory->update(pSellOrder, &theWriteableSellOrder, pTransaction);
	pTransaction->addResource(CPositionResource::alloc(UPDATE_ACTION, m_pMDB->m_PositionFactory, pSellOrder->getPosition()));
	if (pSellOrder->getPosition()->getAccount() != NULL)
		pTransaction->addResource(CAccountResource::alloc(UPDATE_ACTION, m_pMDB->m_AccountFactory, pSellOrder->getPosition()->getAccount()));
	else
		REPORT_EVENT(LOG_ERROR, "insertOTCTrade", "pSellOrder->getPosition()->getAccount() is NULL");

	return true;
}

bool CTradeService::triggerOrderActionBatch(CTriggerOrderActionField* pTriggerOrderAction, CTransaction* pTransaction)
{
	CTriggerOrderActionField theTriggerOrderAction;
	//theTriggerOrderAction.init();
	theTriggerOrderAction.ActionFlag = AF_Delete;
	theTriggerOrderAction.OrderLocalID.clear();

	CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByAccountID(pTriggerOrderAction->AccountID);

	int nCount = 0;

	while (pTriggerOrder != NULL)
	{
		if (!pTriggerOrderAction->ExchangeID.isNull() && pTriggerOrder->ExchangeID != pTriggerOrderAction->ExchangeID)
		{
			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByAccountID();
			continue;
		}

		if (!pTriggerOrderAction->AccountID.isNull() && pTriggerOrder->AccountID != pTriggerOrderAction->AccountID)
		{
			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByAccountID();
			continue;
		}

		//如果指定了合约，只获取合约部分
		if (!pTriggerOrderAction->InstrumentID.isNull() && pTriggerOrder->InstrumentID != pTriggerOrderAction->InstrumentID)
		{
			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByAccountID();
			continue;
		}

		if (!pTriggerOrderAction->AccountID.isNull() && pTriggerOrder->AccountID != pTriggerOrderAction->AccountID)
		{
			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByAccountID();
			continue;
		}

		//if (!pTriggerOrderAction->ProductGroup.isNull() && pTriggerOrder->ProductGroup != pTriggerOrderAction->ProductGroup)
		//{
		//	pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByAccountID();
		//	continue;
		//}

		if (!pTriggerOrderAction->TriggerOrderType.isNull() && !pTriggerOrderAction->TriggerOrderType.contains(pTriggerOrder->TriggerOrderType.getValue()))
		{
			pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByAccountID();
			continue;
		}

		theTriggerOrderAction.OrderID = pTriggerOrder->OrderID;
		//如果这里不制定TriggerOrder，我们后续的处理肯定由于处理之后LinkTriggerOrder了，导致core
		//theTriggerOrderAction.linkTriggerOrder(pTriggerOrder);

		//Order会被删除，在删除之前，得到下一个地址
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByAccountID();

		CTransaction theTransaction;
		if (triggerOrderAction(&theTriggerOrderAction, &theTransaction) != NULL)
		{
			nCount++;
			theTransaction.commit();
		}
		else
		{
			theTransaction.rollback();
		}
	}
	if (nCount == 0)
		return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND, "ZeroTriggerOrderActioned");

	return true;
}

bool CTradeService::orderActionBatch(CWriteableOrderAction* pOrderAction, CTransaction* pTransaction)
{
	CWriteableOrderAction theOrderAction;
	theOrderAction.init();
	theOrderAction.ActionFlag = AF_Delete;
	//theOrderAction.UserID = pOrderAction->UserID;
	theOrderAction.MemberID = pOrderAction->MemberID;
	if (theOrderAction.AccountID.isNull())
		theOrderAction.AccountID = pOrderAction->MemberID;
	theOrderAction.OrderLocalID.clear();
	COrder* pOrder = m_pMDB->m_OrderFactory->findFirstByAccountID(theOrderAction.AccountID);
	int nCount = 0;
	while (pOrder != NULL)
	{
		if (!pOrderAction->ExchangeID.isNull() && pOrder->ExchangeID != pOrderAction->ExchangeID)
		{
			pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
			continue;
		}

		//如果指定了合约，只获取合约部分
		if (!pOrderAction->InstrumentID.isNull() && pOrder->InstrumentID != pOrderAction->InstrumentID)
		{
			pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
			continue;
		}

		//if (!pOrderAction->ProductGroup.isNull() && pOrder->ProductGroup != pOrderAction->ProductGroup)
		//{
		//	pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
		//	continue;
		//}

		theOrderAction.OrderID = pOrder->OrderID;
		theOrderAction.linkMember(pOrderAction->getMember());
		theOrderAction.linkOrder(pOrder);

		//Order会被删除，在删除之前，得到下一个地址
		pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();

		CTransaction theTransaction;
		if (orderAction(&theOrderAction, &theTransaction))
		{
			nCount++;
			theTransaction.commit();
		}
		else
		{
			theTransaction.rollback();
		}
	}

	if (nCount == 0)
		return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND, "ZeroOrderActioned");

	return true;
}

bool CTradeService::orderActionBatchPrice(CWriteableOrderAction* pOrderAction, CTransaction* pTransaction)
{
	if (pOrderAction->Direction.isNull())
		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "DirectionIsNull");

	CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pOrderAction->ExchangeID, pOrderAction->InstrumentID);
	if (pInstrument == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);

	CWriteableOrderAction theOrderAction;
	theOrderAction.init();
	theOrderAction.ActionFlag = AF_Delete;
	//theOrderAction.UserID = pOrderAction->UserID;
	theOrderAction.MemberID = pOrderAction->MemberID;
	theOrderAction.OrderLocalID.clear();
	int nCount = 0;

	if (pOrderAction->Direction == D_Buy)
	{
		COrderIterator* pCountPartyIterator = m_pOrderBookList->getOrderIterator(pInstrument, D_Buy);
		COrder* pCountParty = pCountPartyIterator->next();
		while (pCountParty != NULL)
		{
			if (!pOrderAction->MemberID.isNull() && pCountParty->MemberID != pOrderAction->MemberID)
			{
				pCountParty = pCountPartyIterator->next();
				continue;
			}
			if (!pOrderAction->AccountID.isNull() && pCountParty->AccountID != pOrderAction->AccountID)
			{
				pCountParty = pCountPartyIterator->next();
				continue;
			}

			theOrderAction.OrderID = pCountParty->OrderID;
			theOrderAction.linkMember(pCountParty->getMember());
			theOrderAction.linkOrder(pCountParty);

			//Order会被删除，在删除之前，得到下一个地址
			pCountParty = pCountPartyIterator->next();

			CTransaction theTransaction;
			if (orderAction(&theOrderAction, &theTransaction))
			{
				nCount++;
				theTransaction.commit();
			}
			else
			{
				theTransaction.rollback();
			}
		}
		pCountPartyIterator->free();
	}
	else
	{
		COrderIterator* pCountPartyIterator = m_pOrderBookList->getOrderIterator(pInstrument, D_Sell);
		COrder* pCountParty = pCountPartyIterator->next();
		while (pCountParty != NULL)
		{
			if (!pOrderAction->MemberID.isNull() && pCountParty->MemberID != pOrderAction->MemberID)
			{
				pCountParty = pCountPartyIterator->next();
				continue;
			}
			if (!pOrderAction->AccountID.isNull() && pCountParty->AccountID != pOrderAction->AccountID)
			{
				pCountParty = pCountPartyIterator->next();
				continue;
			}

			theOrderAction.OrderID = pCountParty->OrderID;
			theOrderAction.linkMember(pCountParty->getMember());
			theOrderAction.linkOrder(pCountParty);

			//Order会被删除，在删除之前，得到下一个地址
			pCountParty = pCountPartyIterator->next();

			CTransaction theTransaction;
			if (orderAction(&theOrderAction, &theTransaction))
			{
				nCount++;
				theTransaction.commit();
			}
			else
			{
				theTransaction.rollback();
			}
		}
		pCountPartyIterator->free();
	}

	if (nCount == 0)
		return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND, "ZeroOrderActioned");

	return true;
}
bool CTradeService::orderActionBatchManager(CWriteableOrderAction* pOrderAction, CTransaction* pTransaction)
{
	CWriteableOrderAction theOrderAction;
	theOrderAction.init();
	theOrderAction.ActionFlag = AF_Delete;
	theOrderAction.OrderLocalID.clear();
	COrder* pOrder = m_pMDB->m_OrderFactory->getFirst();
	int nCount = 0;
	while (pOrder != NULL)
	{
		if (!pOrderAction->ExchangeID.isNull() && pOrder->ExchangeID != pOrderAction->ExchangeID)
		{
			pOrder = m_pMDB->m_OrderFactory->getNext();
			continue;
		}

		//如果指定了合约，只获取合约部分
		if (!pOrderAction->InstrumentID.isNull() && pOrder->InstrumentID != pOrderAction->InstrumentID)
		{
			pOrder = m_pMDB->m_OrderFactory->getNext();
			continue;
		}

		if (!pOrderAction->MemberID.isNull() && pOrder->MemberID != pOrderAction->MemberID)
		{
			pOrder = m_pMDB->m_OrderFactory->getNext();
			continue;
		}

		if (!pOrderAction->AccountID.isNull() && pOrder->AccountID != pOrderAction->AccountID)
		{
			pOrder = m_pMDB->m_OrderFactory->getNext();
			continue;
		}

		theOrderAction.OrderID = pOrder->OrderID;
		theOrderAction.linkMember(pOrder->getMember());
		theOrderAction.linkOrder(pOrder);

		//Order会被删除，在删除之前，得到下一个地址
		pOrder = m_pMDB->m_OrderFactory->getNext();

		CTransaction theTransaction;
		if (orderAction(&theOrderAction, &theTransaction))
		{
			theTransaction.commit();
			nCount++;
		}
		else
		{
			theTransaction.rollback();
		}
	}

	if (nCount == 0)
		return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND, "ZeroOrderActioned");

	return true;
}

bool CTradeService::orderAction(CWriteableOrderAction* pOrderAction, CTransaction* pTransaction)
{
	pOrderAction->m_pMDB = m_pMDB;
	COrder* pOrder = pOrderAction->getOrder();
	if (pOrder == NULL)
	{
		if (!pOrderAction->OrderID.isNull())
		{
			pOrder = pOrderAction->linkOrder();
		}
		else
		{
			if (pOrderAction->MemberID.isNull())
			{
				if (pOrderAction->getMember() == NULL)
					return m_pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);
				pOrderAction->MemberID = pOrderAction->getMember()->MemberID;
			}

			if (pOrderAction->AccountID.isNull())
				pOrderAction->AccountID = pOrderAction->MemberID;

			pOrder = pOrderAction->linkOrder(m_pMDB->m_OrderFactory->findByLocalID(pOrderAction->AccountID, pOrderAction->OrderLocalID));
			if (pOrder != NULL)
				pOrderAction->OrderID = pOrder->OrderID;
		}
		if (pOrder == NULL)
			return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND, pOrderAction->OrderID.getValue());
	}

	//CWriteableOrder aOrder;
	//m_pMDB->m_OrderFactory->retrieve(pOrder,&aOrder);
	//aOrder.VolumeRemain = 0;
	//aOrder.OrderStatus = OS_AllCanceled;
	//m_pMDB->m_OrderFactory->update(pOrder,&aOrder,pTransaction);
	//return true;

	// 以下字段填写原有的值，避免被误传
//	pOrderAction->UserID = pOrder->UserID;
//	pOrderAction->LocalID = pOrder->LocalID;
	pOrderAction->MemberID = pOrder->MemberID;
	//pOrderAction->ProductGroup = pOrder->ProductGroup;
	pOrderAction->ExchangeID = pOrder->ExchangeID;
	pOrderAction->InstrumentID = pOrder->InstrumentID;
	pOrderAction->AccountID = pOrder->AccountID;

	// 如果没传值，填写原有的值
	if (pOrderAction->Price.isNull())
		pOrderAction->Price = pOrder->Price;
	if (pOrderAction->Volume.isNull())
		pOrderAction->Volume = pOrder->Volume;

	if (pOrderAction->getMember() == NULL)
		pOrderAction->linkMember();

	//进过OrderAciont之后，pOrder的价格会发送变化，所以这里需要预先记录一下价格，便于后面判断是否需要尝试成交
	CPriceType ActionPrice = pOrder->Price;

	if (!m_Wedge->orderAction(pOrderAction, pTransaction))
		return false;

	//Todo:如果是修改订单，判断是否需要重新撮合
	switch (pOrderAction->ActionFlag)
	{
	case AF_Modify:
	{
		if (ActionPrice != pOrderAction->Price)
		{
			if (needTryTrade(pOrderAction->getOrder()))
			{
				m_Wedge->tryTradeAfterOrderInsert(pOrderAction->getOrder());
				m_Wedge->doAfterOrderChange(pOrderAction->getOrder());
				//开始把Postion和Account进行资金事务处理，只需要处理一次，不需要连续处理
				//pOrderAction->getOrder()前面已经检测了不为空
				pTransaction->addResource(COrderResource::alloc(UPDATE_ACTION, m_pMDB->m_OrderFactory, pOrderAction->getOrder()));
				if (pOrderAction->getOrder()->getPosition() != NULL)
				{
					pTransaction->addResource(CPositionResource::alloc(UPDATE_ACTION, m_pMDB->m_PositionFactory, pOrderAction->getOrder()->getPosition()));
					if (pOrderAction->getOrder()->getPosition()->getAccount() != NULL)
						pTransaction->addResource(CAccountResource::alloc(UPDATE_ACTION, m_pMDB->m_AccountFactory, pOrderAction->getOrder()->getPosition()->getAccount()));
					else
						REPORT_EVENT(LOG_ERROR, "orderAction modify", "pOrderAction->getOrder()->getPosition()->getAccount() is NULL");
					return true; //直接返回，否则会再调用一次doAfterChange
				}
				else
				{
					REPORT_EVENT(LOG_ERROR, "orderAction modify", "pOrderAction->getOrder()->getPosition() is NULL");
					return true;
				}
			}
		}
		break;
	}
	case AF_Active:
	{
		if (needTryTrade(pOrderAction->getOrder()))
		{
			m_Wedge->tryTradeAfterOrderInsert(pOrderAction->getOrder());
			m_Wedge->doAfterOrderChange(pOrderAction->getOrder());
			//开始把Postion和Account进行资金事务处理，只需要处理一次，不需要连续处理
			//pOrderAction->getOrder()前面已经检测了不为空
			pTransaction->addResource(COrderResource::alloc(UPDATE_ACTION, m_pMDB->m_OrderFactory, pOrderAction->getOrder()));
			if (pOrderAction->getOrder()->getPosition() != NULL)
			{
				pTransaction->addResource(CPositionResource::alloc(UPDATE_ACTION, m_pMDB->m_PositionFactory, pOrderAction->getOrder()->getPosition()));
				if (pOrderAction->getOrder()->getPosition()->getAccount() != NULL)
					pTransaction->addResource(CAccountResource::alloc(UPDATE_ACTION, m_pMDB->m_AccountFactory, pOrderAction->getOrder()->getPosition()->getAccount()));
				else
					REPORT_EVENT(LOG_ERROR, "orderAction active", "pOrderAction->getOrder()->getPosition()->getAccount() is NULL");
				return true; //直接返回，否则会再调用一次doAfterChange
			}
			else
			{
				REPORT_EVENT(LOG_ERROR, "orderAction active", "pOrderAction->getOrder()->getPosition() is NULL");
				return true;
			}
		}
		break;
	}
	}

	m_Wedge->doAfterOrderChange(pOrderAction->getOrder());

	return true;
}

//bool CTradeService::quoteAction(CWriteableQuoteAction* pQuoteAction, CTransaction* pTransaction)
//{
//	CQuote* pQuote = m_pMDB->m_QuoteFactory->findByTradeUnitAndInstrument(pQuoteAction->MemberID, pQuoteAction->TradeUnitID, pQuoteAction->ExchangeID, pQuoteAction->InstrumentID);
//	if (pQuote == NULL)
//		return m_pErrorEngine->reportError(ERROR_QUOTE_NOT_FOUND);
//
//	///取消对应的买卖报单
//	CWriteableOrderAction theOrderAction;
//	theOrderAction.init();
//
//	///设置公共的部分
//	theOrderAction.ActionFlag = AF_Delete;
//	theOrderAction.UserID = pQuoteAction->UserID;
//
//	///取消买报单
//	theOrderAction.OrderID = pQuoteAction->getQuote()->BuyOrderID;
//
//	CTransactionSavePoint* pBeforeCancelBidOrder = CTransactionSavePoint::alloc(pTransaction);
//	if (!orderAction(&theOrderAction, pTransaction))
//	{
//		//internalWarning("删除报价时删除买入报单失败");
//		pTransaction->rollbackToSavePoint(pBeforeCancelBidOrder);
//	}
//
//	///取消卖报单
//	theOrderAction.OrderID = pQuoteAction->getQuote()->SellOrderID;
//
//	CTransactionSavePoint* pBeforeCancelAskOrder = CTransactionSavePoint::alloc(pTransaction);
//	if (!orderAction(&theOrderAction, pTransaction))
//	{
//		//internalWarning("删除报价时删除卖出报单失败");
//		pTransaction->rollbackToSavePoint(pBeforeCancelAskOrder);
//	}
//	m_pMDB->m_QuoteFactory->remove(pQuote, pTransaction);
//	return true;
//}

bool CTradeService::insertSingleTrade(CWriteableTrade* pTrade, CWriteableOrder* pOrder)
{
	///是否可以插入单边成交
	if (!m_Wedge->insertSingleTrade(pTrade, pOrder))
		return false;
	return true;
}

bool CTradeService::timeSync(CWriteableCurrentTime* pCurrentTime)
{
	CMonitorIndex::handleOne();
	if (!m_Wedge->timeSync(pCurrentTime))
	{
		return false;
	}
	return true;
}

bool CTradeService::updateInstrumentStatus(CWriteableMarketData* pInstrumentStatus, CTransaction* pTransaction)
{
	if (!m_Wedge->updateInstrumentStatus(pInstrumentStatus, pTransaction))
	{
		return false;
	}
	return true;
}

bool CTradeService::changePosition(CPositionChangeList* pChangeList, CTransaction* pTransaction)
{
	//	if (pChangeList->pOrder->Priority == 0)
	//	{
	//		if (pChangeList->pTrade == NULL)
	//			return true;
	//	}

		///检查是否可以变更客户持仓
	if (!m_Wedge->changePosition(pChangeList, pTransaction))
	{
		return false;
	}
	return true;
}

COrderBookList* CTradeService::getOrderBookList()
{
	return m_pOrderBookList;
}

//bool CTradeService::snapMDB(UF_INT8 nSequenceNo, const char* dumpFileName)
//{
//	char szPath[256];
//
//	if (dumpFileName == NULL)
//		sprintf(szPath, "%s/snap.%.0f/", m_pConfig->getConfig("SnapPath"), nSequenceNo);
//	else
//		sprintf(szPath, "%s/snap.%.0f.%s/", m_pConfig->getConfig("SnapPath"), nSequenceNo, dumpFileName);
//
//	UpdateSequence();
//
//	//m_pMDB->save(szPath, nSequenceNo);
//	if (!m_pMDB->snap(szPath, nSequenceNo))
//		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "MDBSnapStatusIsNot0");
//
//	ResetSequence();
//	return true;
//}
//
//bool CTradeService::snapAllMDB(UF_INT8 nSequenceNo, const char* dumpFileName)
//{
//	char szPath[256];
//
//	if (dumpFileName == NULL)
//		sprintf(szPath, "%s/snap.%.0f/", m_pConfig->getConfig("SnapPath"), nSequenceNo);
//	else
//		sprintf(szPath, "%s/snap.%.0f.%s/", m_pConfig->getConfig("SnapPath"), nSequenceNo, dumpFileName);
//
//	UpdateSequence();
//
//	//m_pMDB->save(szPath, nSequenceNo);
//	if (!m_pMDB->snapAll(szPath, nSequenceNo))
//		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "MDBSnapStatusIsNot0");
//
//	ResetSequence();
//	return true;
//}

bool CTradeService::ReqPositionActionBatch(CPositionActionField* pPositionActionField, CTransaction* pTransaction)
{
	CPositionActionField eachPositionActionField;
	CopyPositionActionEntity(&eachPositionActionField, pPositionActionField);
	//int nCount = 0;
	if (pPositionActionField->AccountID.isNull())
		pPositionActionField->AccountID = pPositionActionField->MemberID;
	CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByAccountInstrument(pPositionActionField->AccountID, pPositionActionField->ExchangeID, pPositionActionField->InstrumentID);
	while (pPosition != NULL)
	{
		if (!pPositionActionField->PosiDirection.isNull() && pPositionActionField->PosiDirection != pPosition->PosiDirection)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountInstrument();
			continue;
		}
		if (!pPositionActionField->TradeUnitID.isNull() && pPositionActionField->TradeUnitID != pPosition->TradeUnitID)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountInstrument();
			continue;
		}
		eachPositionActionField.PosiDirection = pPosition->PosiDirection;
		eachPositionActionField.TradeUnitID = pPosition->TradeUnitID;

		//由于支持逐仓全仓切换，只能允许一个的场景，这里做要么全部成功要么全部失败的处理
		if (!ReqPositionAction(&eachPositionActionField, NULL, pTransaction))
			return false;
		//nCount++;
		pPosition = m_pMDB->m_PositionFactory->findNextByAccountInstrument();
	}

	//	if (nCount == 0)
		//	return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND, "ZeroPositionActioned");
	return true;
}

bool CTradeService::ChangeMemberPosition(CPositionActionField* pPositionActionField, CMemberPositionField* pMemberPositionField, CTransaction* pTransaction)
{
	CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pPositionActionField->ExchangeID, pPositionActionField->InstrumentID);
	if (pInstrument == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND, "%s:%s", pPositionActionField->ExchangeID.getValue(), pPositionActionField->InstrumentID.getValue());

	CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(pPositionActionField->AccountID,
		pPositionActionField->ExchangeID, pPositionActionField->InstrumentID);
	CWriteableMemberPosition theMemberPosition;
	if (pMemberPosition == NULL)
	{
		theMemberPosition.init();
		theMemberPosition.AccountID = pPositionActionField->AccountID;
		theMemberPosition.ExchangeID = pPositionActionField->ExchangeID;
		theMemberPosition.InstrumentID = pPositionActionField->InstrumentID;
		theMemberPosition.DefaultCrossMargin = true;
		//theMemberPosition.DefaultLeverage = pInstrument->DefaultLeverage;
		CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pPositionActionField->MemberID, pPositionActionField->AccountID, pInstrument->SettlementGroup, pInstrument->ClearCurrency);
		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(pPositionActionField->MemberID);
		CMarginRate* rate = getMarginRate(m_pMDB, pInstrument->MarginRateGroup.getValue(), pAccount);
		theMemberPosition.DefaultLeverage = getDefaultLvgWithMargin(m_pMDB, pInstrument, pPositionActionField->MemberID, pPositionActionField->AccountID, pPositionActionField->PosiDirection, rate);
	}
	else
	{
		m_pMDB->m_MemberPositionFactory->retrieve(pMemberPosition, &theMemberPosition);
	}

	//最后的MemberPositon也需要修改
	switch (pPositionActionField->ActionType)
	{
	case PAT_ChangeDefaultLeverage:
	case PAT_ChangeLeverage:
	case PAT_ChangeMaxLeverage:
	{
		if (pPositionActionField->Amount <= 0.0)
			return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "Amount<=0");
		theMemberPosition.DefaultLeverage = pPositionActionField->Amount;
		break;
	}
	case PAT_ChangeAllCross:
	case PAT_ChangeCross:
	case PAT_ChangeDefaultCross:
	{
		if (pPositionActionField->Amount != 0.0 && pPositionActionField->Amount != 1.0)
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Amount should be 0 or 1");
		if (pPositionActionField->Amount != 0.0)
			theMemberPosition.DefaultCrossMargin = true;
		else
			theMemberPosition.DefaultCrossMargin = false;
		break;
	}
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "ActionTypeNot[1/4/5/6/7]");
	}
	}
	if (pMemberPositionField != NULL)
		CopyMemberPositionEntity(pMemberPositionField, &theMemberPosition);
	return m_pMDB->m_MemberPositionFactory->addOrUpdate(pMemberPosition, &theMemberPosition);
}

//持仓保证金操作
bool CTradeService::ReqPositionAction(CPositionActionField* pPositionActionField, CPositionField* pPositionField, CTransaction* pTransaction)
{
	//根据持仓调整
	CPosition* pPosition = m_pMDB->m_PositionFactory->findByPositionID(pPositionActionField->PositionID);
	if (pPosition == NULL)
		pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(pPositionActionField->AccountID, pPositionActionField->ExchangeID, pPositionActionField->InstrumentID, pPositionActionField->TradeUnitID, pPositionActionField->PosiDirection);

	CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pPositionActionField->ExchangeID, pPositionActionField->InstrumentID);
	if (pInstrument == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND, "%s:%s", pPositionActionField->ExchangeID.getValue(), pPositionActionField->InstrumentID.getValue());
	if (pInstrument->getMarketData()->InstrumentStatus != IS_Continous)
		return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS, "%s:%s", pPositionActionField->ExchangeID.getValue(), pPositionActionField->InstrumentID.getValue());

	//判断持仓的保证金是否符合最低保证金要求
	CWriteablePosition theWriteablePosition;
	theWriteablePosition.init();
	if (pPosition == NULL)
	{
		if (pPositionActionField->ActionType == PAT_AddMargin || pPositionActionField->ActionType == PAT_CutMargin)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		if (pPositionActionField->AccountID.isNull())
			pPositionActionField->AccountID = pPositionActionField->MemberID;
		if (pPositionActionField->TradeUnitID.isNull())
			pPositionActionField->TradeUnitID = pPositionActionField->AccountID;
		if (!InitPosition(m_pMDB, m_pErrorEngine, pInstrument, &theWriteablePosition, pPositionActionField->MemberID, pPositionActionField->TradeUnitID.getValue(), pPositionActionField->AccountID,
			pPositionActionField->PosiDirection, pPositionActionField->IsCrossMargin, DOUBLE_NaN))
			return false;
	}
	else
	{
		m_pMDB->m_PositionFactory->retrieve(pPosition, &theWriteablePosition);
	}
	CAccount* pAccount = theWriteablePosition.getAccount();
	CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(pPositionActionField->MemberID);
	CMarginRate* pRate = getMarginRate(m_pMDB, pInstrument->MarginRateGroup.getValue(), pAccount);
	if (pRate == NULL)
		return m_pErrorEngine->reportError(ERROR_CAN_NOT_FIND_MARGIN_RATE);

	CRatioType minMarginRatio = 1.0;
	CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(theWriteablePosition.AccountID,
		pPositionActionField->ExchangeID, pPositionActionField->InstrumentID);
	if (pMemberPosition != NULL)
		minMarginRatio = getMinMarginRatio(pMemberPosition->Position, pMemberPosition->LongFrozen, pMemberPosition->ShortFrozen, pInstrument, pRate, pPositionActionField->PosiDirection);
	else
		minMarginRatio = getMinMarginRatio(theWriteablePosition.Position, theWriteablePosition.LongFrozen, theWriteablePosition.ShortFrozen, pInstrument, pRate, pPositionActionField->PosiDirection);
	//double minMarginRatio = getMinMarginRatio(theWriteablePosition.Position, theWriteablePosition.LongFrozen, theWriteablePosition.ShortFrozen, pRate, pPositionActionField->PosiDirection);
	//theWriteablePosition.Leverage = getPositionLeverage(pRate, pInstrument, NULL, pPositionActionField->PosiDirection, minMarginRatio);

	CMoneyType changeMargin = 0.0;
	//CMoneyType changeForzenMargin = 0.0;

	switch (pPositionActionField->ActionType)
	{
	case PAT_ChangeLeverage:
	case PAT_ChangeMaxLeverage:
	{
		if (theWriteablePosition.LongFrozen > 0.0 || theWriteablePosition.ShortFrozen > 0.0)
			return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "OpenOrder:Frozen!=0");

		if (pPositionActionField->Amount <= 0.0)
			return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "Amount<=0");

		if (theWriteablePosition.Leverage == 0.0)
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_LEVERAGE, "Position.Leverage=0");

		CMoneyType newMargin = 0.0;
		if (pRate->ValueMode == VM_Absolute)
		{
			if (pPositionActionField->Amount > -minMarginRatio || pPositionActionField->Amount >= 0.0)
				return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "Amount[%s]>-minMarginRatio[%s]", pPositionActionField->Amount.getString().c_str(), minMarginRatio.getString().c_str());
			// return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_LEVERAGE);
			if (pPositionActionField->ActionType == PAT_ChangeMaxLeverage)
			{
				//要么全部成功，要么全部失败的情况下，这里只能选择成功
				if (theWriteablePosition.Leverage > pPositionActionField->Amount)
					//return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_LEVERAGE,"ChangeMaxLeverage:[%0.4f]<[%0.4f]NotNecessary", theWriteablePosition.Leverage.getValue(), pPositionActionField->Amount.getValue());
					return true;
			}
			newMargin = abs(theWriteablePosition.Position * pPositionActionField->Amount * pInstrument->VolumeMultiple);
			changeMargin = newMargin - theWriteablePosition.UseMargin;
			//changeForzenMargin = theWriteablePosition.FrozenMargin*pPositionActionField->Amount / theWriteablePosition.Leverage;
			//theWriteablePosition.LongFrozenMargin = theWriteablePosition.LongFrozenMargin*pPositionActionField->Amount / theWriteablePosition.Leverage;
			//theWriteablePosition.ShortFrozenMargin = theWriteablePosition.ShortFrozenMargin*pPositionActionField->Amount / theWriteablePosition.Leverage;
		}
		else
		{
			if (pPositionActionField->Amount > minMarginRatio.getInverse() || pPositionActionField->Amount <= 0.0)
				return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "Amount[%s]>minMarginRatio[%s]", pPositionActionField->Amount.getString().c_str(), minMarginRatio.getString().c_str());
			// return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_LEVERAGE);
			if (pPositionActionField->ActionType == PAT_ChangeMaxLeverage)
			{
				//要么全部成功，要么全部失败的情况下，这里只能选择成功
				if (theWriteablePosition.Leverage < pPositionActionField->Amount)
					//return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_LEVERAGE,"ChangeMaxLeverage:[%0.4f]<[%0.4f]NotNecessary", theWriteablePosition.Leverage.getValue(), pPositionActionField->Amount.getValue());
					return true;
			}

			/*switch (pInstrument->MarginPriceType)
			{
			case MPT_MarkedPrice:
			{
				newMargin = abs(theWriteablePosition.Position*pInstrument->VolumeMultiple*pInstrument->getMarketData()->MarkedPrice) / pPositionActionField->Amount;
				changeMargin = newMargin - theWriteablePosition.UseMargin;
				break;
			}
			case MPT_LastPrice:
			{
				newMargin = abs(theWriteablePosition.Position*pInstrument->VolumeMultiple*pInstrument->getMarketData()->LastPrice) / pPositionActionField->Amount;
				changeMargin = newMargin - theWriteablePosition.UseMargin;
				break;
			}
			default:
			{
				newMargin = theWriteablePosition.PositionCost / pPositionActionField->Amount;
				changeMargin = newMargin - theWriteablePosition.UseMargin;
			}
			}*/
			newMargin = getPositionNewMargin(m_pMDB, &theWriteablePosition, pInstrument, pPositionActionField->Amount);
			changeMargin = newMargin - theWriteablePosition.UseMargin;

			//changeForzenMargin = theWriteablePosition.FrozenMargin*theWriteablePosition.Leverage / pPositionActionField->Amount
			//	- theWriteablePosition.FrozenMargin;
			//theWriteablePosition.LongFrozenMargin = theWriteablePosition.LongFrozenMargin*theWriteablePosition.Leverage / pPositionActionField->Amount;
			//theWriteablePosition.ShortFrozenMargin = theWriteablePosition.ShortFrozenMargin*theWriteablePosition.Leverage / pPositionActionField->Amount;
		}
		theWriteablePosition.Leverage = pPositionActionField->Amount;
		if (theWriteablePosition.IsCrossMargin)
			theWriteablePosition.UseMargin += changeMargin;
		else
		{
			// 杠杆调整后需要的保证金 > 仓位保证金，则失败
			//if (newMargin > theWriteablePosition.UseMargin)
			//	return m_pErrorEngine->reportError(ERROR_NotEnoughMargin, "[%.4f]>[%.4f]", newMargin.getValue(), theWriteablePosition.UseMargin.getValue());
			// 调整杠杆，不影响保证金
			//changeMargin = 0.0;

			double PositionProfit = 0.0;
			if (changeMargin > 0)
			{
				// 从外部划入保证金时，需要补齐未实现亏损部分
				PositionProfit = getSinglePositionProfit(m_pMDB, &theWriteablePosition);
				changeMargin -= MIN(PositionProfit, 0.0);
			}
			else
			{
				// 释放的保证金，保留在仓位上
				changeMargin = 0.0;
			}
			theWriteablePosition.UseMargin += changeMargin;
		}
		break;
	}
	case PAT_CutMargin:
	{
		// 全仓模式不支持增减保证金
		if (theWriteablePosition.IsCrossMargin)
			return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "CrossPositionIsNotAllowed");
		if (theWriteablePosition.Position == 0.0)
			return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "PositionIsEmpty");
		if (pPositionActionField->Amount < 0.0)
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Amount<0");
		changeMargin = -pPositionActionField->Amount;
		theWriteablePosition.UseMargin += changeMargin;
		if (!pPositionActionField->IsAutoAddMargin.isNull())
			theWriteablePosition.IsAutoAddMargin = pPositionActionField->IsAutoAddMargin;
		break;
	}
	case PAT_AddMargin:
	{
		// 全仓模式不支持增减保证金
		if (theWriteablePosition.IsCrossMargin)
			return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "CrossPositionIsNotAllowed");
		if (theWriteablePosition.Position == 0.0)
			return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "PositionIsEmpty");
		if (pPositionActionField->Amount < 0.0)
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Amount<0");
		changeMargin = pPositionActionField->Amount;
		theWriteablePosition.UseMargin += changeMargin;
		if (!pPositionActionField->IsAutoAddMargin.isNull())
			theWriteablePosition.IsAutoAddMargin = pPositionActionField->IsAutoAddMargin;
		break;
	}
	case PAT_ChangeCross:
	case PAT_ChangeAllCross:
	{
		//return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "ChangeCrossIsNotAllowed");
		if (pPositionActionField->Amount != 0.0 && pPositionActionField->Amount != 1.0)
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Amount should be 0 or 1");
		//加上有持仓不让调整
		if (theWriteablePosition.Position != 0.0)
			return m_pErrorEngine->reportError(ERROR_POSITION_EXIST);

		//如果有订单，不允许切换持仓类型
		if (theWriteablePosition.LongFrozen != 0 || theWriteablePosition.ShortFrozen != 0)
			return m_pErrorEngine->reportError(ERROR_ORDER_EXIST);
		//如果有条件委托，不允许切换持仓类型

		//如果全仓模式下的某个仓位处于亏损，不允许切换持仓类型
		if (theWriteablePosition.IsCrossMargin && theWriteablePosition.Position > 0.0)
		{
			CAccount* pAccount = theWriteablePosition.getAccount();
			if (pAccount == NULL)
				return m_pErrorEngine->reportError(ERROR_AccountNotFound);

			if (!checkCrossPositionProfit(m_pMDB, pAccount))
				return m_pErrorEngine->reportError(ERROR_POSITION_UNDER_DEFICIT);
		}

		if (pPositionActionField->Amount != 0.0)
			theWriteablePosition.IsCrossMargin = true;
		else
			theWriteablePosition.IsCrossMargin = false;
		theWriteablePosition.Frequency.clear();
		break;
	}
	case PAT_ChangeCostPrice:
	{
		// 只有Spot现货才支持修改成本价
		if (pInstrument->ProductClass != PC_AccountExchange)
			return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
		if (pPosition == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		if (pPositionActionField->Amount <= 0.0)
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Amount must be greater than 0");

		theWriteablePosition.OpenPrice = pPositionActionField->Amount;
		theWriteablePosition.TotalPositionCost = theWriteablePosition.Position * theWriteablePosition.OpenPrice * pInstrument->VolumeMultiple;
		break;
	}
	default:
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "ActionTypeMustIn[1/2/3/4/5/8]");
	}

	//	theWriteablePosition.FrozenMargin += changeForzenMargin;
//	theWriteablePosition.UserID = pPositionActionField->UserID;

	//如果是逐仓的话，需要判断 (保证金+未显示盈亏)/PositionCost > minMarginRatio
	if (changeMargin < 0.0)
	{
		if (theWriteablePosition.IsCrossMargin)
		{
			CRatioType newRatio = (theWriteablePosition.UseMargin / theWriteablePosition.PositionCost).getValue();
			if (newRatio < minMarginRatio)
				return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "[%.4f]>[%.4f]", 1 / newRatio.getValue(), 1 / minMarginRatio);
		}
		else
		{
			/*CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(theWriteablePosition.ExchangeID, theWriteablePosition.InstrumentID);
			double PositionProfit = 0.0;
			if (pInstrument->IsInverse)
				PositionProfit = theWriteablePosition.PositionCost - theWriteablePosition.Position * pInstrument->VolumeMultiple / pMarketData->MarkedPrice;
			else
				PositionProfit = theWriteablePosition.Position * pMarketData->MarkedPrice * pInstrument->VolumeMultiple - theWriteablePosition.PositionCost;
			PositionProfit = MIN(PositionProfit, 0);
			CRatioType newLeverage = theWriteablePosition.PositionCost / (theWriteablePosition.UseMargin + PositionProfit);
			if (theWriteablePosition.UseMargin < 0 || newLeverage > theWriteablePosition.Leverage)
				return m_pErrorEngine->reportError(ERROR_NotEnoughMargin, "[%.4f+%0.4f]<[%.4f/%0.4f]",
					theWriteablePosition.UseMargin.getValue(), PositionProfit, theWriteablePosition.PositionCost.getValue(), theWriteablePosition.Leverage.getValue());*/
			double PositionProfit = getSinglePositionProfit(m_pMDB, &theWriteablePosition);
			PositionProfit = MIN(PositionProfit, 0);
			if (theWriteablePosition.UseMargin < theWriteablePosition.PositionCost / theWriteablePosition.Leverage - PositionProfit)
				return m_pErrorEngine->reportError(ERROR_NotEnoughMargin, "[%.4f<%.4f]", theWriteablePosition.UseMargin.getValue(), (theWriteablePosition.PositionCost / theWriteablePosition.Leverage - PositionProfit).getValue());
		}
	}

	//if (changeMargin != 0.0 || changeForzenMargin != 0.0)
	if (changeMargin != 0.0)
	{
		// 只处理与Account相关的操作
		//if (theWriteablePosition.IsCrossMargin || pPositionActionField->ActionType == PAT_CutMargin || pPositionActionField->ActionType == PAT_AddMargin)
		//{
		if (pAccount == NULL)
			return m_pErrorEngine->reportError(ERROR_AccountNotFound);

		CWriteableAccount theWriteableAccount;
		m_pMDB->m_AccountFactory->retrieve(pAccount, &theWriteableAccount);
		//		theWriteableAccount.UserID = pPositionActionField->UserID;
		if (theWriteablePosition.IsCrossMargin)
			theWriteableAccount.CrossMargin += changeMargin;
		theWriteableAccount.UseMargin += changeMargin;
		//theWriteableAccount.FrozenMargin += changeForzenMargin;

		applyAccountChange(&theWriteableAccount);

		double PositionProfit = getPositionProfit(m_pMDB, pAccount);
		if (PositionProfit > 0.0)
			PositionProfit = 0.0;

		if (theWriteableAccount.Available < -PositionProfit || theWriteablePosition.UseMargin < 0.0)
			//	if (theWriteableAccount.Available < 0.0 || theWriteablePosition.UseMargin < 0.0)
		{
			if (pPositionActionField->ActionType != PAT_CutMargin)
				return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY);
		}
		theWriteableAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_AccountFactory->update(pAccount, &theWriteableAccount, pTransaction);
		//}
	}

	theWriteablePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_PositionFactory->addOrUpdate(pPosition, &theWriteablePosition, pTransaction);
	if (pPositionField != NULL)
		CopyPositionEntity(pPositionField, &theWriteablePosition);
	return true;
}

CMarketData* CTradeService::ReqMarketDataUpdate(CInstrument* pInstrument, CMarketDataUpdateField* pMarketDataField, CTransaction* pTransaction)
{
	CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID);
	if (pMarketData == NULL)
	{
		m_pErrorEngine->reportError(ERROR_MARKETDATA_NOT_FOUND, pMarketDataField->InstrumentID.getValue());
		return NULL;
	}

	CWriteableMarketData theWritebleMarketData;
	m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theWritebleMarketData);

	//更新合理价格和涨跌停
// 	if(!pMarketDataUpdateField->MarkedPrice.isNull())
// 	{
// 		theWritebleMarketData.MarkedPrice = pMarketDataUpdateField->MarkedPrice;
//
// 		CInstrumentStatus *pInstrumentStatus=pInstrument->getInstrumentStatus();
// 		if (pInstrumentStatus==NULL)
// 		{
// 			///没有合约状态，这是不可能的，不过为了鲁棒性，就放过了
// 			REPORT_EVENT(LOG_CRITICAL,"ReqSetUnderlyingPrice","can not find instrument status");
// 			return false;
// 		}
// 		CPriceBanding *pPriceBanding=pInstrumentStatus->getPriceBanding();
// 		if (pPriceBanding==NULL)
// 		{
// 			///找不到价格绑定，那就是没有价格绑定
// 			REPORT_EVENT(LOG_CRITICAL,"ReqSetUnderlyingPrice","can not find PriceBanding");
// 			return false;
// 		}
// 		calculatePriceBanding(pMarketDataUpdateField->MarkedPrice,pInstrument->getInstrumentProperty()->PriceTick,
// 			pPriceBanding->ValueMode,pPriceBanding->RoundingMode,
// 			pPriceBanding->UpperValue,pPriceBanding->LowerValue,
// 		&(theWritebleMarketData.UpperLimitPrice),&(theWritebleMarketData.LowerLimitPrice));
// 	}

	if (!pMarketDataField->InstrumentStatus.isNull())
	{
		theWritebleMarketData.InstrumentStatus = pMarketDataField->InstrumentStatus;
		if (pMarketDataField->InstrumentStatus == IS_NotActive)
		{
			CancelAll(pInstrument, NULL);
			CancelAllTriggerOrder(pInstrument, NULL);
		}
	}

	if (!pMarketDataField->PrePositionFeeRate.isNull())
		theWritebleMarketData.PrePositionFeeRate = pMarketDataField->PrePositionFeeRate;

	if (!pMarketDataField->PositionFeeRate.isNull())
		theWritebleMarketData.PositionFeeRate = pMarketDataField->PositionFeeRate;

	if (!pMarketDataField->PreSettlementPrice.isNull())
		theWritebleMarketData.PreSettlementPrice = pMarketDataField->PreSettlementPrice;

	if (!pMarketDataField->SettlementPrice.isNull())
		theWritebleMarketData.SettlementPrice = pMarketDataField->SettlementPrice;

	if (!pMarketDataField->MarkedPrice.isNull())
	{
		theWritebleMarketData.MarkedPrice = pMarketDataField->MarkedPrice;
		if (pInstrument->PriceLimitPrice == LPT_MarkedPrice)
		{
			calculatePriceBanding(theWritebleMarketData.MarkedPrice, pInstrument->PriceTick,
				pInstrument->PriceLimitValueMode, RM_In, pInstrument->PriceLimitUpperValue, pInstrument->PriceLimitLowerValue,
				&(theWritebleMarketData.UpperLimitPrice), &(theWritebleMarketData.LowerLimitPrice));
		}
		SetLastKLine(pMarketData->ExchangeID, getMarkedInstrumentID(pMarketDataField->InstrumentID), theWritebleMarketData.MarkedPrice, theWritebleMarketData.UpdateTime);
	}

	if (!pMarketDataField->TheoryPrice.isNull())
		theWritebleMarketData.TheoryPrice = pMarketDataField->TheoryPrice;

	theWritebleMarketData.UpdateTime = g_nCurrentTime;

	if (!pMarketDataField->UnderlyingPrice.isNull())
	{
		theWritebleMarketData.UnderlyingPrice = pMarketDataField->UnderlyingPrice;
		CInstrumentIDType UnderlyingInstrumentID = "i";
		UnderlyingInstrumentID += pMarketDataField->InstrumentID;
		SetLastKLine(pMarketData->ExchangeID, UnderlyingInstrumentID, theWritebleMarketData.UnderlyingPrice, theWritebleMarketData.UpdateTime);

		//如果是指数行情，需要K线并且最新价需要更新
		if (pInstrument->ProductClass == PC_Market)
		{
			theWritebleMarketData.LastPrice = pMarketDataField->UnderlyingPrice;
			SetLastKLine(pMarketData->ExchangeID, pMarketDataField->InstrumentID, theWritebleMarketData.UnderlyingPrice, theWritebleMarketData.UpdateTime);
		}
		else
		{
			//计算MarketGap的时候需要用到限价，这里就优先计算限价
			//if (!pInstrument->PriceLimitValueMode.isNull())
			if (pInstrument->PriceLimitPrice == LPT_UnderlyingPrice)
			{
				calculatePriceBanding(theWritebleMarketData.UnderlyingPrice, pInstrument->PriceTick,
					pInstrument->PriceLimitValueMode, RM_In, pInstrument->PriceLimitUpperValue, pInstrument->PriceLimitLowerValue,
					&(theWritebleMarketData.UpperLimitPrice), &(theWritebleMarketData.LowerLimitPrice));
			}

			SetBasisPrice(&theWritebleMarketData, pInstrument);
			GenMarkPrice(&theWritebleMarketData);

			if (pInstrument->PriceLimitPrice == LPT_MarkedPrice)
			{
				calculatePriceBanding(theWritebleMarketData.MarkedPrice, pInstrument->PriceTick,
					pInstrument->PriceLimitValueMode, RM_In, pInstrument->PriceLimitUpperValue, pInstrument->PriceLimitLowerValue,
					&(theWritebleMarketData.UpperLimitPrice), &(theWritebleMarketData.LowerLimitPrice));
			}

			if (!theWritebleMarketData.MarkedPrice.isNull())
				SetLastKLine(pMarketData->ExchangeID, getMarkedInstrumentID(pMarketDataField->InstrumentID), theWritebleMarketData.MarkedPrice, theWritebleMarketData.UpdateTime);
		}
	}

	theWritebleMarketData.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	//theWritebleMarketData.UpdateTime = getMDBTime(m_pMDB);
	m_pMDB->m_MarketDataFactory->update(pMarketData, &theWritebleMarketData, pTransaction);
	return pMarketData;
}

bool CTradeService::SetLastKLine(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CPriceType Price, CMilliSecondsType UpdateTime)
{
	if (Price.isNull())
		return false;

	CWriteableTrade theTrade;
	theTrade.init();
	theTrade.InstrumentID = InstrumentID;
	theTrade.ExchangeID = ExchangeID;
	theTrade.Price = Price;
	m_pKLineTrigger->UpdateLastKLine(&theTrade, "1m");
	m_pKLineTrigger->UpdateLastKLine(&theTrade, "5m");
	m_pKLineTrigger->UpdateLastKLine(&theTrade, "1h");
	m_pKLineTrigger->UpdateLastKLine(&theTrade, "1d");
	m_pKLineTrigger->UpdateLastKLine(&theTrade, "1w");
	m_pKLineTrigger->UpdateLastKLine(&theTrade, "1o");
	return true;
}

bool CTradeService::SetBasisPrice(CWriteableMarketData* pMarketDataField, CInstrument* pInstrument)
{
	//string InstrumentIndex = pMarketDataField->ExchangeID.getValue();
	//InstrumentIndex += "_";
	//InstrumentIndex += pMarketDataField->InstrumentID.getValue();

	CMarketOrder* pBuyMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(pMarketDataField->ExchangeID, pMarketDataField->InstrumentID);
	if (pBuyMarketOrder == NULL)
		return false;

	CMarketOrder* pSellMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(pMarketDataField->ExchangeID, pMarketDataField->InstrumentID);
	if (pSellMarketOrder == NULL)
		return false;

	//SMaretPriceGap SMaretPriceGap;
	//SMaretPriceGap.GapPrice = (pBuyMarketOrder->Price.getValue() + pSellMarketOrder->Price.getValue()) / 2 - pMarketDataField->UnderlyingPrice;
	double OrderBookPrice = (pBuyMarketOrder->Price.getValue() + pSellMarketOrder->Price.getValue()) / 2;
	//判断Gap是否合理
	//if (!pMarketDataField->UpperLimitPrice.isNull() && !pMarketDataField->LowerLimitPrice.isNull())
	//{
	//	if (pMarketDataField->UpperLimitPrice < OrderBookPrice || pMarketDataField->LowerLimitPrice > OrderBookPrice)
	//	{
	//		REPORT_EVENT(LOG_CRITICAL, "MarkedPrice", "[%s_%s]OrderBookPrice[%s]NotIn[%s]And[%s]", pMarketDataField->ExchangeID.getValue(), pMarketDataField->InstrumentID.getValue(),
	//			doubleToStringFormat(OrderBookPrice).c_str(), pMarketDataField->LowerLimitPrice.getString().c_str(), pMarketDataField->UpperLimitPrice.getString().c_str());
	//		return false;
	//	}
	//}

	double GapPrice = OrderBookPrice - pMarketDataField->UnderlyingPrice;

	//第一次，需要清零
	if (pMarketDataField->MarkedPriceWeight.isNull() || pMarketDataField->MarkedPriceTotal.isNull())
	{
		pMarketDataField->MarkedPriceWeight = 0;
		pMarketDataField->MarkedPriceTotal = 0;
	}

	while (pMarketDataField->MarkedPriceWeight >= pInstrument->MarkedPriceTimes)
	{
		pMarketDataField->MarkedPriceTotal -= pMarketDataField->MarkedPriceTotal.getValue() / pMarketDataField->MarkedPriceWeight.getValue();
		pMarketDataField->MarkedPriceWeight -= 1;
	}
	//pMarketDataField->MarkedPriceTotal += SMaretPriceGap.GapPrice;
	//pMarketDataField->MarkedPriceWeight += SMaretPriceGap.Volume;
	pMarketDataField->MarkedPriceTotal += GapPrice;
	pMarketDataField->MarkedPriceWeight += 1;

	//CMarkedPriceQueues::iterator iter = m_MarkedPriceQueues.find(InstrumentIndex);
	//if (iter == m_MarkedPriceQueues.end())
	//{
	//	CMarkedPriceQueue* pMarkedPriceQueue = new CMarkedPriceQueue();
	//	pMarkedPriceQueue->push(SMaretPriceGap);
	//	pMarketDataField->MarkedPriceTotal = SMaretPriceGap.GapPrice;
	//	pMarketDataField->MarkedPriceWeight = SMaretPriceGap.Volume;
	//	m_MarkedPriceQueues.insert(CMarkedPriceQueues::value_type(InstrumentIndex, pMarkedPriceQueue));
	//	REPORT_EVENT(LOG_DEBUG, "MarkedPrice", "[%s][%s]->[In][GapPrice:%lf][New]", pMarketDataField->ExchangeID.getValue(), pMarketDataField->InstrumentID.getValue(), SMaretPriceGap.GapPrice);
	//	pMarkedPriceQueue->push(SMaretPriceGap);
	//}
	//else
	//{
	//	CMarkedPriceQueue* pMarkedPriceQueue = iter->second;
	//	while (pMarkedPriceQueue->size() > g_nMarkedPriceTimes)
	//	{
	//		pMarketDataField->MarkedPriceTotal -= pMarkedPriceQueue->front().GapPrice;
	//		pMarketDataField->MarkedPriceWeight -= pMarkedPriceQueue->front().Volume;
	//		REPORT_EVENT(LOG_DEBUG, "MarkedPrice", "[%s][%s]->[Out][GapPrice:%lf]", pMarketDataField->ExchangeID.getValue(), pMarketDataField->InstrumentID.getValue(), pMarkedPriceQueue->front().GapPrice);
	//		pMarkedPriceQueue->pop();
	//	}
	//	////如果之前的样本不足，或者pMarkedPriceQueue被清理了，这里需要做平滑处理，不能做删除处理
	//	//if (pMarkedPriceQueue->size() < g_nMarkedPriceTimes)
	//	//{
	//	//	double avg = pMarketDataField->MarkedPriceTotal.getValue() / pMarketDataField->MarkedPriceWeight.getValue();
	//	//	double weight = pMarketDataField->MarkedPriceWeight.getValue() / g_nMarkedPriceTimes;
	//	//	pMarketDataField->MarkedPriceTotal -= avg * weight;
	//	//	pMarketDataField->MarkedPriceWeight -= weight;
	//	//}
	//	pMarketDataField->MarkedPriceTotal += SMaretPriceGap.GapPrice;
	//	pMarketDataField->MarkedPriceWeight += SMaretPriceGap.Volume;
	//	REPORT_EVENT(LOG_DEBUG, "MarkedPrice", "[%s][%s]->[In][GapPrice:%lf]", pMarketDataField->ExchangeID.getValue(), pMarketDataField->InstrumentID.getValue(), SMaretPriceGap.GapPrice);
	//	pMarkedPriceQueue->push(SMaretPriceGap);
	//}

	//REPORT_EVENT(LOG_DEBUG, "MarkedPrice", "[%s][%s]->[GapPrice:%lf][Total:%lf]/[Weight:%lf]=[%lf][Ask:%lf][Bid:%lf]",
	//	pMarketDataField->ExchangeID.getValue(), pMarketDataField->InstrumentID.getValue(), pMarketDataField->GapPrice.getValue(),
	//	pMarketDataField->MarkedPriceTotal.getValue(), pMarketDataField->MarkedPriceWeight.getValue(),
	//	pMarketDataField->MarkedPriceTotal.getValue() / pMarketDataField->MarkedPriceWeight.getValue(),
	//	pSellMarketOrder->Price.getValue(), pSellMarketOrder->Price.getValue());

	REPORT_EVENT(LOG_DEBUG, "MarkedPrice", "[%s_%s]->[%lf|%lf] [(%lf+%lf)/2-%lf|%lf/%0.f]",
		pMarketDataField->ExchangeID.getValue(), pMarketDataField->InstrumentID.getValue(),
		GapPrice, pMarketDataField->MarkedPriceTotal.getValue() / pMarketDataField->MarkedPriceWeight.getValue(),
		pSellMarketOrder->Price.getValue(), pBuyMarketOrder->Price.getValue(), pMarketDataField->UnderlyingPrice.getValue(),
		pMarketDataField->MarkedPriceTotal.getValue(), pMarketDataField->MarkedPriceWeight.getValue());

	return true;
}

bool CTradeService::CancelAll(CInstrument* pInstrument, CTransaction* pTransaction)
{
	COrder* pOrder = m_pMDB->m_OrderFactory->getFirst();
	while (pOrder != NULL)
	{
		if (pOrder->ExchangeID == pInstrument->ExchangeID && pOrder->InstrumentID == pInstrument->InstrumentID)
		{
			CWriteableOrderAction theOrderAction;
			theOrderAction.init();
			theOrderAction.ActionFlag = AF_Delete;
			theOrderAction.OrderID = pOrder->OrderID;
			//			theOrderAction.UserID = pOrder->UserID;
			orderAction(&theOrderAction, pTransaction);
		}
		pOrder = m_pMDB->m_OrderFactory->getNext();
	}
	return true;
}

bool CTradeService::UpdateSequence()
{
	//情况的目的是在反演或者重启之后，保持内存的一致性
	//CMarkedPriceQueues::iterator iter = m_MarkedPriceQueues.begin();
	//while (iter != m_MarkedPriceQueues.end())
	//{
	//	CMarkedPriceQueue* pMarkedPriceQueue = iter->second;
	//	iter++;
	//	delete pMarkedPriceQueue;
	//}
	//m_MarkedPriceQueues.clear();
	return true;
}

bool CTradeService::ClearMarket(CClearMarketField* pClearMarketField, CTransaction* pTransaction)
{
	CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pClearMarketField->ExchangeID, pClearMarketField->InstrumentID);
	if (pInstrument == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);

	//SortNumber不是-1时，不作任何处理
	if (pInstrument->SortNumber != -1)
		return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS);

	//Order撤单
	CancelAll(pInstrument, pTransaction);

	//TriggerOrder撤单
	CancelAllTriggerOrder(pInstrument, pTransaction);

	//清理LastKLine
	//ClearAllLastKLine(pInstrument, pTransaction);

	//清理KLine
	//ClearAllKLine(pInstrument, pTransaction);

	//更新MarketData
	CMarketData* pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID);
	if (pMarketData != NULL)
	{
		CWriteableMarketData theWritebleMarketData;
		m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theWritebleMarketData);
		theWritebleMarketData.OpenPrice = theWritebleMarketData.MarkedPrice;
		theWritebleMarketData.ClosePrice = theWritebleMarketData.MarkedPrice;
		theWritebleMarketData.HighestPrice = theWritebleMarketData.MarkedPrice;
		theWritebleMarketData.LowestPrice = theWritebleMarketData.MarkedPrice;
		m_pMDB->m_MarketDataFactory->update(pMarketData, &theWritebleMarketData, pTransaction);
	}

	return true;
}

bool CTradeService::CancelAllTriggerOrder(CInstrument* pInstrument, CTransaction* pTransaction)
{
	CTriggerOrder* pOrder = m_pMDB->m_TriggerOrderFactory->getFirst();
	while (pOrder != NULL)
	{
		if (pOrder->ExchangeID == pInstrument->ExchangeID && pOrder->InstrumentID == pInstrument->InstrumentID)
		{
			CTriggerOrderActionField theTriggerOrderAction;
			//theTriggerOrderAction.init();
			theTriggerOrderAction.ActionFlag = AF_Delete;
			theTriggerOrderAction.OrderID = pOrder->OrderID;
			//theTriggerOrderAction.UserID = pOrder->UserID;
			//theTriggerOrderAction.linkTriggerOrder(pOrder);
			pOrder = m_pMDB->m_TriggerOrderFactory->getNext();
			triggerOrderAction(&theTriggerOrderAction, pTransaction);
		}
		else
		{
			pOrder = m_pMDB->m_TriggerOrderFactory->getNext();
		}
	}
	return true;
}

//bool CTradeService::ClearAllLastKLine(CInstrument *pInstrument, CTransaction *pTransaction)
//{
//	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID);
//	while (pLastKLine != NULL)
//	{
//		CLastKLine *pToBeRemove = pLastKLine;
//		pLastKLine = m_pMDB->m_LastKLineFactory->findNextByInstrumentID();
//		m_pMDB->m_LastKLineFactory->remove(pToBeRemove, pTransaction);
//	}
//	m_pMDB->m_LastKLineFactory->endFindByInstrumentID();
//	return true;
//}

//bool CTradeService::ClearAllKLine(CInstrument *pInstrument, CTransaction *pTransaction)
//{
//	CKLine *pKLine = m_pMDB->m_KLineFactory->findFirstByInstrumentID(pInstrument->ExchangeID, pInstrument->InstrumentID);
//	while (pKLine != NULL)
//	{
//		CKLine *pToBeRemove = pKLine;
//		pKLine = m_pMDB->m_KLineFactory->findNextByInstrumentID();
//		m_pMDB->m_KLineFactory->remove(pToBeRemove, pTransaction);
//	}
//	m_pMDB->m_KLineFactory->endFindByInstrumentID();
//	return true;
//}

bool CTradeService::ClearLastKLine(CClearLastKLineField* pClearLastKLineField, CTransaction* pTransaction)
{
	CLastKLine* pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pClearLastKLineField->ExchangeID, pClearLastKLineField->InstrumentID, pClearLastKLineField->Bar);
	if (pLastKLine == NULL)
		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);

	m_pMDB->m_LastKLineFactory->remove(pLastKLine, pTransaction);
	return true;
}

bool CTradeService::ReqPositionMerge(CPositionMergeField* pPositionMerge, CPositionField* pPositionOut, CTransaction* pTransaction)
{
	CPosition* pPosition = m_pMDB->m_PositionFactory->findByPositionID(pPositionMerge->PositionID);
	if (pPosition == NULL || pPosition->Position == 0.0)
		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "PositionNotFound;%s", pPositionMerge->PositionID.getValue());

	// 目前仅支持全仓仓位的合并
	if (!pPosition->IsCrossMargin) {
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "IsolatedPositionNotSupported;%s", pPositionMerge->PositionID.getValue());
	}

	CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByActivePositionClose(pPositionMerge->PositionID);
	if (pTriggerOrder != NULL)
		return m_pErrorEngine->reportError(ERROR_RECORD_EXIST, "TriggerOrderExist;%s", pPosition->PositionID.getValue());

	if (pPosition->LongFrozen > -pPosition->ShortFrozen)
		return m_pErrorEngine->reportError(ERROR_RECORD_EXIST, "OrderExist;%s_%s_%s_%s_%c[LongFrozen:%s][ShortFrozen:%s]", pPosition->MemberID.getValue(), pPosition->TradeUnitID.getValue(),
			pPosition->ExchangeID.getValue(), pPosition->InstrumentID.getValue(), pPosition->PosiDirection.getValue(),
			pPosition->LongFrozen.getString().c_str(), pPosition->ShortFrozen.getString().c_str());

	if (pPositionMerge->PositionID1.isNull())
		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "PositionID1IsNULL");

	pPosition = PositionMerge(pPositionMerge, pPosition, pPositionMerge->PositionID1, pTransaction);
	if (pPosition == NULL)
		return false;

	if (!pPositionMerge->PositionID2.isNull())
		pPosition = PositionMerge(pPositionMerge, pPosition, pPositionMerge->PositionID2, pTransaction);
	if (pPosition == NULL)
		return false;

	if (!pPositionMerge->PositionID3.isNull())
		pPosition = PositionMerge(pPositionMerge, pPosition, pPositionMerge->PositionID3, pTransaction);
	if (pPosition == NULL)
		return false;

	if (!pPositionMerge->PositionID4.isNull())
		pPosition = PositionMerge(pPositionMerge, pPosition, pPositionMerge->PositionID4, pTransaction);
	if (pPosition == NULL)
		return false;

	if (!pPositionMerge->PositionID5.isNull())
		pPosition = PositionMerge(pPositionMerge, pPosition, pPositionMerge->PositionID5, pTransaction);
	if (pPosition == NULL)
		return false;

	if (!pPositionMerge->PositionID6.isNull())
		pPosition = PositionMerge(pPositionMerge, pPosition, pPositionMerge->PositionID6, pTransaction);
	if (pPosition == NULL)
		return false;

	if (!pPositionMerge->PositionID7.isNull())
		pPosition = PositionMerge(pPositionMerge, pPosition, pPositionMerge->PositionID7, pTransaction);
	if (pPosition == NULL)
		return false;

	if (!pPositionMerge->PositionID8.isNull())
		pPosition = PositionMerge(pPositionMerge, pPosition, pPositionMerge->PositionID8, pTransaction);
	if (pPosition == NULL)
		return false;

	if (!pPositionMerge->PositionID9.isNull())
		pPosition = PositionMerge(pPositionMerge, pPosition, pPositionMerge->PositionID9, pTransaction);
	if (pPosition == NULL)
		return false;

	if (!pPositionMerge->PositionID10.isNull())
		pPosition = PositionMerge(pPositionMerge, pPosition, pPositionMerge->PositionID10, pTransaction);
	if (pPosition == NULL)
		return false;

	CopyPositionEntity(pPositionOut, pPosition);
	return true;
}

CPosition* CTradeService::PositionMerge(CPositionMergeField* pPositionMerge, CPosition* pPosition, CLongIndexIDType PositionID, CTransaction* pTransaction)
{
	CPosition* pPositionToMerge = m_pMDB->m_PositionFactory->findByPositionID(PositionID);
	if (pPositionToMerge == NULL || pPositionToMerge->Position == 0.0)
	{
		m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "Position1NotFound;%s", PositionID.getValue());
		return NULL;
	}

	// 目前仅支持全仓仓位的合并
	if (!pPositionToMerge->IsCrossMargin) {
		m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "IsolatedPositionNotSupported;%s", PositionID.getValue());
		return NULL;
	}

	if (pPositionToMerge == pPosition)
	{
		m_pErrorEngine->reportError(ERROR_DuplicateRecord, "PositionMerge:TheSamePosition:%s", PositionID.getValue());
		return NULL;
	}

	CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByActivePositionClose(PositionID);
	if (pTriggerOrder != NULL)
	{
		m_pErrorEngine->reportError(ERROR_RECORD_EXIST, "TriggerOrderExist;%s", PositionID.getValue());
		return NULL;
	}

	if (pPositionToMerge->LongFrozen > -pPositionToMerge->ShortFrozen)
	{
		m_pErrorEngine->reportError(ERROR_RECORD_EXIST, "OrderExist;%s[LongFrozen:%s][ShortFrozen:%s]", PositionID.getValue(),
			pPositionToMerge->LongFrozen.getString().c_str(), pPositionToMerge->ShortFrozen.getString().c_str());
		return NULL;
	}

	//如果两个持仓资金不一样，不能操作
	if (pPositionToMerge->AccountID != pPosition->AccountID)
	{
		m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "AccountIDNotSame;%s_[Account:%s]!=[Account:%s]", PositionID.getValue(),
			pPositionToMerge->AccountID.getValue(), pPosition->AccountID.getValue());
		return NULL;
	}

	switch (pPosition->PosiDirection)
	{
	case PD_Long:
	{
		switch (pPositionToMerge->PosiDirection)
		{
		case PD_Long:
		{
			return PositionAdd(pPosition, pPositionToMerge, pTransaction);
			break;
		}
		case PD_Short:
		{
			return PositionSub(pPositionMerge, pPosition, pPositionToMerge, pTransaction);
			break;
		}
		case PD_Net:
		{
			if (pPositionToMerge->Position > 0)
				return PositionAdd(pPosition, pPositionToMerge, pTransaction);
			else
				return PositionSub(pPositionMerge, pPosition, pPositionToMerge, pTransaction);
			break;
		}
		}
		break;
	}
	case PD_Short:
	{
		switch (pPositionToMerge->PosiDirection)
		{
		case PD_Long:
		{
			return PositionSub(pPositionMerge, pPosition, pPositionToMerge, pTransaction);
			break;
		}
		case PD_Short:
		{
			return PositionAdd(pPosition, pPositionToMerge, pTransaction);
			break;
		}
		case PD_Net:
		{
			if (pPositionToMerge->Position > 0)
				return PositionSub(pPositionMerge, pPosition, pPositionToMerge, pTransaction);
			else
				return PositionAdd(pPosition, pPositionToMerge, pTransaction);
			break;
		}
		}
		break;
	}
	case PD_Net:
	{
		switch (pPositionToMerge->PosiDirection)
		{
		case PD_Long:
		{
			if (pPosition > 0)
				return PositionAdd(pPosition, pPositionToMerge, pTransaction);
			else
				return PositionSub(pPositionMerge, pPosition, pPositionToMerge, pTransaction);
			break;
		}
		case PD_Short:
		{
			if (pPosition > 0)
				return PositionSub(pPositionMerge, pPosition, pPositionToMerge, pTransaction);
			else
				return PositionAdd(pPosition, pPositionToMerge, pTransaction);
			break;
		}
		case PD_Net:
		{
			if (pPosition > 0)
			{
				if (pPositionToMerge->Position > 0)
					return PositionAdd(pPosition, pPositionToMerge, pTransaction);
				else
					return PositionSub(pPositionMerge, pPosition, pPositionToMerge, pTransaction);
			}
			else
			{
				if (pPositionToMerge->Position > 0)
					return PositionSub(pPositionMerge, pPosition, pPositionToMerge, pTransaction);
				else
					return PositionAdd(pPosition, pPositionToMerge, pTransaction);
			}
			break;
		}
		}
		break;
	}
	}
	return NULL;
}

CPosition* CTradeService::PositionAdd(CPosition* pPosition1, CPosition* pPosition2, CTransaction* pTransaction)
{
	CWriteablePosition thePosition;
	if (pPosition2->AccountID == pPosition2->TradeUnitID) //合仓占据主要
	{
		m_pMDB->m_PositionFactory->retrieve(pPosition2, &thePosition);
		if (pPosition1->IsCrossMargin)	//全仓占据主要
			thePosition.IsCrossMargin = true;
		thePosition.Position += pPosition1->Position;
		thePosition.UseMargin += pPosition1->UseMargin;
		thePosition.PositionCost += pPosition1->PositionCost;
		thePosition.TotalPositionCost += pPosition1->TotalPositionCost;
		thePosition.TotalCloseProfit += pPosition1->TotalCloseProfit;
		thePosition.TradeFee += pPosition1->TradeFee;
		thePosition.PositionFee += pPosition1->PositionFee;

		thePosition.Leverage = thePosition.PositionCost / thePosition.UseMargin;
		if (thePosition.getInstrument()->IsInverse)
		{
			if (thePosition.PositionCost == 0.0)
				thePosition.CostPrice.clear();
			else
				thePosition.CostPrice = thePosition.Position * thePosition.getInstrument()->VolumeMultiple / thePosition.PositionCost;

			if (thePosition.TotalPositionCost == 0.0)
				thePosition.OpenPrice.clear();
			else
				thePosition.OpenPrice = thePosition.Position * thePosition.getInstrument()->VolumeMultiple / thePosition.TotalPositionCost;
		}
		else
		{
			thePosition.CostPrice = thePosition.PositionCost / (thePosition.Position * thePosition.getInstrument()->VolumeMultiple);
			thePosition.OpenPrice = thePosition.TotalPositionCost / (thePosition.Position * thePosition.getInstrument()->VolumeMultiple);
		}
		thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_PositionFactory->update(pPosition2, &thePosition, pTransaction);
		m_pMDB->m_PositionFactory->remove(pPosition1, pTransaction);
		return pPosition2;
	}
	else
	{
		m_pMDB->m_PositionFactory->retrieve(pPosition1, &thePosition);
		if (pPosition2->IsCrossMargin)	//全仓占据主要
			thePosition.IsCrossMargin = true;
		thePosition.Position += pPosition2->Position;
		thePosition.UseMargin += pPosition2->UseMargin;
		thePosition.PositionCost += pPosition2->PositionCost;
		thePosition.TotalPositionCost += pPosition2->TotalPositionCost;
		thePosition.TotalCloseProfit += pPosition2->TotalCloseProfit;
		thePosition.TradeFee += pPosition2->TradeFee;
		thePosition.PositionFee += pPosition2->PositionFee;

		if (thePosition.getInstrument()->IsInverse)
		{
			if (thePosition.PositionCost == 0.0)
				thePosition.CostPrice.clear();
			else
				thePosition.CostPrice = thePosition.PositionCost / (thePosition.Position * thePosition.getInstrument()->VolumeMultiple);
			if (thePosition.TotalPositionCost == 0.0)
				thePosition.OpenPrice.clear();
			else
				thePosition.OpenPrice = thePosition.Position * thePosition.getInstrument()->VolumeMultiple / thePosition.TotalPositionCost;
		}
		else
		{
			thePosition.CostPrice = thePosition.PositionCost / thePosition.Position * thePosition.getInstrument()->VolumeMultiple;
			thePosition.OpenPrice = thePosition.TotalPositionCost / (thePosition.Position * thePosition.getInstrument()->VolumeMultiple);
		}
		if (thePosition.UseMargin != 0.0)
			thePosition.Leverage = thePosition.PositionCost / thePosition.UseMargin;

		thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_PositionFactory->update(pPosition1, &thePosition, pTransaction);
		m_pMDB->m_PositionFactory->remove(pPosition2, pTransaction);
		return pPosition1;
	}
}

CPosition* CTradeService::PositionSub(CPositionMergeField* pPositionMerge, CPosition* pPosition1, CPosition* pPosition2, CTransaction* pTransaction)
{
	COTCTradeField outfield;
	CPosition* pPosition = NULL;
	CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pPosition1->ExchangeID, pPosition1->InstrumentID);
	if (pInstrument == NULL)
	{
		m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND, "PositionSub[%s_%s]", pPosition1->ExchangeID.getValue(), pPosition1->InstrumentID.getValue());
		return NULL;
	}

	//确保Positon不能被删除，如果删除了，后续放回即将出现问题，导致后续core
	//一旦return NULL，后续操作暂停，这样就无法实现全部仓位合并的操作
	if (fabs(pPosition1->Position) == fabs(pPosition2->Position))
	{
		outfield.Volume = fabs(pPosition1->Position.getValue());
		pPosition = pPosition1;
		outfield.Remark = pPosition1->TradeUnitID;
	}
	else
	{
		if (fabs(pPosition1->Position) > fabs(pPosition2->Position))
		{
			outfield.Volume = fabs(pPosition2->Position.getValue());
			pPosition = pPosition1;
		}
		else
		{
			outfield.Volume = fabs(pPosition1->Position.getValue());
			pPosition = pPosition2;
		}
	}

	outfield.Price = pInstrument->getMarketData()->LastPrice;
	//	outfield.UserID = pPositionMerge->UserID;
	outfield.LocalID = pPositionMerge->LocalID;
	outfield.ExchangeID = pPosition1->ExchangeID;
	outfield.InstrumentID = pPosition1->InstrumentID;
	outfield.BuyMemberID = pPosition1->MemberID;
	outfield.BuyOffsetFlag = OF_Close;
	outfield.SellMemberID = pPosition1->MemberID;
	outfield.SellOffsetFlag = OF_Close;
	outfield.TakerDirection = D_Buy;
	outfield.TriggerPrice.clear();
	outfield.OTCType = OTCT_PositionMerge;

	switch (pPosition1->PosiDirection)
	{
	case PD_Long:
	{
		outfield.BuyTradeUnitID = pPosition2->TradeUnitID;
		outfield.SellTradeUnitID = pPosition1->TradeUnitID;
		break;
	}
	case PD_Short:
	{
		outfield.BuyTradeUnitID = pPosition1->TradeUnitID;
		outfield.SellTradeUnitID = pPosition2->TradeUnitID;
		break;
	}
	case PD_Net:
	{
		if (pPosition1->Position > 0)
		{
			outfield.BuyTradeUnitID = pPosition2->TradeUnitID;
			outfield.SellTradeUnitID = pPosition1->TradeUnitID;
		}
		else
		{
			outfield.BuyTradeUnitID = pPosition1->TradeUnitID;
			outfield.SellTradeUnitID = pPosition2->TradeUnitID;
		}
		break;
	}
	}

	if (!insertOTCTrade(&outfield, pTransaction))
		return NULL;

	return pPosition;
}

bool CTradeService::closeOrderActionBatch(CCloseOrderActionField* pCloseOrderAction, CTransaction* pTransaction)
{
	CCloseOrderActionField theCloseOrderAction;
	//theCloseOrderAction.init();
	theCloseOrderAction.ActionFlag = AF_Delete;
	//theCloseOrderAction.UserID = pCloseOrderAction->UserID;
	theCloseOrderAction.MemberID = pCloseOrderAction->MemberID;
	theCloseOrderAction.OrderLocalID.clear();

	COrder* pOrder = m_pMDB->m_OrderFactory->findFirstByAccountID(pCloseOrderAction->AccountID);
	int nCount = 0;
	while (pOrder != NULL)
	{
		if (!pCloseOrderAction->ExchangeID.isNull() && pOrder->ExchangeID != pCloseOrderAction->ExchangeID)
		{
			pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
			continue;
		}

		//如果指定了合约，只获取合约部分
		if (!pCloseOrderAction->InstrumentID.isNull() && pOrder->InstrumentID != pCloseOrderAction->InstrumentID)
		{
			pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
			continue;
		}

		if (!pCloseOrderAction->ProductGroup.isNull() && pOrder->getInstrument()->ProductGroup != pCloseOrderAction->ProductGroup)
		{
			pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
			continue;
		}

		// 只处理有关联triggerOrder的报单
		if (pOrder->RelatedOrderID.isNull())
		{
			pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
			continue;
		}

		theCloseOrderAction.OrderID = pOrder->OrderID;
		//		theCloseOrderAction.linkMember(pCloseOrderAction->getMember());
			//	theCloseOrderAction.linkOrder(pOrder);

				//Order会被删除，在删除之前，得到下一个地址
		pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();

		CTransaction theTransaction;
		if (closeOrderAction(&theCloseOrderAction, &theTransaction))
		{
			nCount++;
			theTransaction.commit();
		}
		else
		{
			theTransaction.rollback();
		}
	}
	if (nCount == 0)
		return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND, "ZeroOrderActioned");
	return true;
}

COrder* CTradeService::closeOrderAction(CCloseOrderActionField* pCloseOrderAction, CTransaction* pTransaction)
{
	switch (pCloseOrderAction->ActionFlag)
	{
	case AF_Modify:
	{
		// 处理报单
		CWriteableOrderAction theOrderAction;
		theOrderAction.init();
		//		theOrderAction.UserID = pCloseOrderAction->UserID;
		theOrderAction.ActionFlag = pCloseOrderAction->ActionFlag;
		theOrderAction.OrderID = pCloseOrderAction->OrderID;
		theOrderAction.Price = pCloseOrderAction->Price;
		theOrderAction.Volume = pCloseOrderAction->Volume;
		//		theOrderAction.linkMember(pCloseOrderAction->getMember());
		if (!orderAction(&theOrderAction, pTransaction))
			return NULL;

		// 处理条件单
		if (theOrderAction.getOrder()->RelatedOrderID.isNull())
		{
			m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "TriggerOrder not found, OrderAction should be used instead");
			return NULL;
		}

		CTriggerOrderActionField theTriggerOrderAction;
		//theTriggerOrderAction.init();
		if (pCloseOrderAction->TriggerPrice.isNull() && pCloseOrderAction->SLTriggerPrice.isNull() && pCloseOrderAction->TPTriggerPrice.isNull())
		{
			// 触发价为空，对原有的条件单进行撤单
		//	theTriggerOrderAction.UserID = pCloseOrderAction->UserID;
			theTriggerOrderAction.ActionFlag = AF_Delete;
			theTriggerOrderAction.OrderID = theOrderAction.getOrder()->RelatedOrderID;
			triggerOrderAction(&theTriggerOrderAction, pTransaction);
		}
		else
		{
			// 触发价不为空，对触发价重新计算
			//theTriggerOrderAction.UserID = pCloseOrderAction->UserID;
			theTriggerOrderAction.ActionFlag = pCloseOrderAction->ActionFlag;
			theTriggerOrderAction.OrderID = theOrderAction.getOrder()->RelatedOrderID;
			theTriggerOrderAction.SLTriggerPrice = pCloseOrderAction->SLTriggerPrice;
			theTriggerOrderAction.TPTriggerPrice = pCloseOrderAction->TPTriggerPrice;
			theTriggerOrderAction.TriggerPrice = pCloseOrderAction->TriggerPrice;
			// TPPrice与SLPrice,无论是否有值都必须要传给theTriggerOrderAction，后续会根据这两个值来确定止盈止损价格
			theTriggerOrderAction.TPPrice = pCloseOrderAction->TPPrice;
			theTriggerOrderAction.SLPrice = pCloseOrderAction->SLPrice;
			theTriggerOrderAction.TriggerPriceType = pCloseOrderAction->TriggerPriceType;
			triggerOrderAction(&theTriggerOrderAction, pTransaction);
		}
		//if (success)
		//{
		//	pCloseOrderAction->linkOrder(theOrderAction.getOrder());
		//	pCloseOrderAction->linkTriggerOrder(theTriggerOrderAction.getTriggerOrder());
		//}
		return theOrderAction.getOrder();
	}
	case AF_Delete:
	{
		CWriteableOrderAction theOrderAction;
		theOrderAction.init();
		//theOrderAction.UserID = pCloseOrderAction->UserID;
		theOrderAction.ActionFlag = pCloseOrderAction->ActionFlag;
		theOrderAction.OrderID = pCloseOrderAction->OrderID;
		//theOrderAction.linkMember(pCloseOrderAction->getMember());
		if (!orderAction(&theOrderAction, pTransaction))
			return NULL;
		//pCloseOrderAction->linkOrder(theOrderAction.getOrder());
		return theOrderAction.getOrder();
	}
	default:
		break;
	}

	return NULL;
}

bool CTradeService::ReqPositionReverse(CPositionReverseField* pPositionReverse, CWriteableOrder* pOutOrder, CMember* pMember, TSessionRef* pSession, CTransaction* pTransaction)
{
	bool success = false;
	CPosition* pPosition = m_pMDB->m_PositionFactory->findByPositionID(pPositionReverse->PositionID);
	if (pPosition == NULL || pPosition->Position == 0.0)
		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "PositionNotFound;%s", pPositionReverse->PositionID.getValue());

	//净仓不支持反向操作
	if (pPosition->PosiDirection == PD_Net)
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "PD_Net is not supported");
	}

	CWriteablePosition thePosition;
	m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);

	CInstrument* pInstrument = pPosition->getInstrument();
	if (pInstrument == NULL)
	{
		pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pPosition->ExchangeID, pPosition->InstrumentID);
		if (pInstrument == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "InstrumentNotFound;%s_%s", pPosition->ExchangeID.getValue(), pPosition->InstrumentID.getValue());
		pPosition->linkInstrument(pInstrument);
	}
	if (pInstrument->ProductClass == PC_AccountExchange)
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "Spot is not supported");

	CDirectionType Direction;
	if (pPosition->PosiDirection == PD_Long)
		Direction = D_Sell;
	else if (pPosition->PosiDirection == PD_Short)
		Direction = D_Buy;
	else
		Direction = pPosition->Position >= 0.0 ? D_Sell : D_Buy;

	// CloseAll
	CWriteableOrder theCloseOrder;
	theCloseOrder.init(m_pMDB);
	theCloseOrder.UserID = pSession->pUserID;
	theCloseOrder.MemberID = pPositionReverse->MemberID;
	theCloseOrder.AccountID = thePosition.AccountID;
	theCloseOrder.TradeUnitID = pPosition->TradeUnitID;
	theCloseOrder.ExchangeID = pPosition->ExchangeID;
	theCloseOrder.InstrumentID = pPosition->InstrumentID;
	theCloseOrder.LocalID = pPositionReverse->LocalID;
	theCloseOrder.Remark = pPositionReverse->Remark;
	theCloseOrder.CopyMemberID = pPositionReverse->CopyMemberID;
	theCloseOrder.APPID = pPositionReverse->APPID;
	theCloseOrder.Tradable = true;
	theCloseOrder.FrontNo = pSession->frontID;
	//theCloseOrder.SessionNo = pSession->sessionID;
	theCloseOrder.Direction = Direction;
	theCloseOrder.PosiDirection = thePosition.PosiDirection;
	theCloseOrder.OffsetFlag = OF_CloseAll;
	theCloseOrder.OrderType = OT_Normal;
	theCloseOrder.OrderPriceType = g_cOrderPriceType;
	theCloseOrder.Cost.clear();
	theCloseOrder.Price.clear();
	theCloseOrder.Volume.clear();
	//theCloseOrder.TimeCondition = TC_GTC;
	theCloseOrder.DeriveSource = DS_Normal;
	theCloseOrder.IsCrossMargin = thePosition.IsCrossMargin;
	theCloseOrder.linkMember(pMember);
	theCloseOrder.linkInstrument(pInstrument);
	theCloseOrder.linkPosition(pPosition);
	success = insertOrder(&theCloseOrder, pTransaction);
	if (!success)
		return success;

	//if (theCloseOrder.getPosition()->Position != 0.0)
	//	return m_pErrorEngine->reportError(ERROR_POSITION_EXIST, "Position still exists while closing all positions");

	// 平仓不受后面开仓的影响
	pTransaction->commit();

	// 要求开平仓杠杆一致，分仓仓位反向开仓前需要调整默认杠杆
	if (!thePosition.TradeUnitID.like(thePosition.AccountID))
	{
		CPositionActionField field;
		field.ActionType = PAT_ChangeDefaultLeverage;
		field.ExchangeID = thePosition.ExchangeID;
		field.InstrumentID = thePosition.InstrumentID;
		//field.ProductGroup = thePosition.ProductGroup;
		field.MemberID = thePosition.MemberID;
		field.AccountID = thePosition.AccountID;
		field.TradeUnitID = "";
		field.Amount = thePosition.Leverage;
		CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(thePosition.AccountID,
			thePosition.ExchangeID, thePosition.InstrumentID);
		if (pMemberPosition == NULL || (pMemberPosition != NULL && pMemberPosition->DefaultLeverage != thePosition.Leverage))
		{
			if (!ChangeMemberPosition(&field, NULL, pTransaction))
				return false;
		}
	}

	// 按平仓量或指定量反向开仓
	CWriteableOrder theOpenOrder;
	theOpenOrder.init(m_pMDB);
	//	theOpenOrder.UserID = pPositionReverse->UserID;
	theOpenOrder.MemberID = pPositionReverse->MemberID;
	theOpenOrder.AccountID = thePosition.AccountID;
	if (!thePosition.TradeUnitID.like(thePosition.AccountID))
		theOpenOrder.TradeUnitID = defaultTradeUnitID;
	else
		theOpenOrder.TradeUnitID = thePosition.TradeUnitID;
	theOpenOrder.ExchangeID = pPosition->ExchangeID;
	theOpenOrder.InstrumentID = pPosition->InstrumentID;
	theOpenOrder.LocalID = pPositionReverse->LocalID;
	theOpenOrder.Remark = pPositionReverse->Remark;
	theOpenOrder.CopyMemberID = pPositionReverse->CopyMemberID;
	theOpenOrder.APPID = pPositionReverse->APPID;
	theOpenOrder.Tradable = true;
	theOpenOrder.FrontNo = pSession->frontID;
	//	theOpenOrder.SessionNo = pSession->sessionID;
	theOpenOrder.Direction = Direction;
	theOpenOrder.PosiDirection = thePosition.PosiDirection == PD_Long ? PD_Short : PD_Long;
	theOpenOrder.OffsetFlag = OF_Open;
	theOpenOrder.OrderType = OT_Normal;
	theOpenOrder.OrderPriceType = g_cOrderPriceType;
	theOpenOrder.Cost.clear();
	theOpenOrder.Price.clear();
	theOpenOrder.Volume = pPositionReverse->Volume.isNull() ? theCloseOrder.Volume : pPositionReverse->Volume;
	//	theOpenOrder.TimeCondition = TC_GTC;
	theOpenOrder.DeriveSource = DS_Normal;
	theOpenOrder.IsCrossMargin = thePosition.IsCrossMargin;
	theOpenOrder.linkMember(pMember);
	theOpenOrder.linkInstrument(pInstrument);

	// 记录反手前的反向仓位的持仓数量
	CVolumeType theOldPosition = 0.0;
	theOpenOrder.m_pMDB = m_pMDB;
	CPosition* pOldPosition = theOpenOrder.linkPosition();
	if (pOldPosition != NULL)
		theOldPosition = pOldPosition->Position;

	// 反手下单
	success = insertOrder(&theOpenOrder, pTransaction);

	//if (!success)
	//{
	//	// 按可用资金反向开仓
	//	CAccount *pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(thePosition.AccountID, thePosition.SettlementGroup,pInstrument->ClearCurrency);
	//	if (pAccount == NULL)
	//		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "AccountNotFound;%s_%s_%s", thePosition.AccountID.getValue(), pInstrument->ClearCurrency.getValue(),
	//			thePosition.SettlementGroup.getValue());

	//	// TODO
	//	return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	//	//theOpenOrder.OrderPriceType = OPT_ByCost;
	//	//theOpenOrder.Cost = pAccount->Available;
	//	//if (!pAccount->getCurrency()->MoneyTick.isNull())
	//	//	theOpenOrder.Cost.toLowerTick(pAccount->getCurrency()->MoneyTick);
	//	//theOpenOrder.Volume.clear();
	//	//success = insertOrder(&theOpenOrder, pTransaction);
	//}

	if (success)
	{
		// 反向仓位在反手前的仓位为0时，继承原有仓位的自动追加保证金属性
		if (theOldPosition == 0.0)
		{
			CWriteablePosition theWriteablePosition;
			m_pMDB->m_PositionFactory->retrieve(theOpenOrder.getPosition(), &theWriteablePosition);
			theWriteablePosition.IsAutoAddMargin = thePosition.IsAutoAddMargin;
			m_pMDB->m_PositionFactory->update(theOpenOrder.getPosition(), &theWriteablePosition, pTransaction);
		}

		if (pOutOrder != NULL)
		{
			CopyOrderEntity(pOutOrder, &theOpenOrder);
			pOutOrder->linkPosition(theOpenOrder.getPosition());
		}
	}
	return success;
}

bool CTradeService::ReqMemberAction(CMemberActionField* pMemberActionField, CMember* pMember, CTransaction* pTransaction)
{
	CWriteableMember theWriteableMember;
	m_pMDB->m_MemberFactory->retrieve(pMember, &theWriteableMember);

	if (!pMemberActionField->PositionType.isNull())
	{
		switch (pMemberActionField->PositionType)
		{
		case PT_Net:
		case PT_Gross:
			break;
		default:
		{
			m_pErrorEngine->reportError(ERROR_CAN_NOT_CHANGE_POSTYPE, "DefaultPositionTypeIsInValid");
			return false;
		}
		}
		//CAccount* pAccount = m_pMDB->m_AccountFactory->findFirstByMemberID(pMemberActionField->MemberID);
		//while (pAccount != NULL)
		//{
		//	if (pAccount->UseMargin > 0 || pAccount->FrozenMargin > 0 || pAccount->FrozenFee > 0 || pAccount->CrossMargin > 0)
		//		return m_pErrorEngine->reportError(ERROR_POSITION_EXIST, "PositionOrOrderExistOnAllAccount");
		//	pAccount = m_pMDB->m_AccountFactory->findNextByMemberID();
		//}
		//if (pMember->OpenPositions > 0)
		//	return m_pErrorEngine->reportError(ERROR_POSITION_EXIST, "OnThisMember");
		//if (pMember->OpenOrders > 0)
		//	return m_pErrorEngine->reportError(ERROR_POSITION_EXIST, "OnThisMember");
		//if (pMember->TriggerOrders > 0)
		//	return m_pErrorEngine->reportError(ERROR_POSITION_EXIST, "TriggerOrderExistOnThisMember");

		//CAccount* pAccount = m_pMDB->m_AccountFactory->findFirstByMemberID(pMemberActionField->MemberID);
		//while (pAccount != NULL)
		//{
		//	CPosition *pPosition = m_pMDB->m_PositionFactory->findFirstByAccountID(pAccount->AccountID);
		//	while (pPosition != NULL)
		//	{
		//		if (pPosition->Position != 0)
		//		{
		//			if (pPosition->getInstrument() != NULL && pPosition->getInstrument()->ProductClass == PC_Margin)
		//				return m_pErrorEngine->reportError(ERROR_POSITION_EXIST, "Position(%s)", pPosition->PositionID.getValue());
		//		}
		//		pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
		//	}
		//	COrder *pOrder = m_pMDB->m_OrderFactory->findFirstByPositionID(pMemberActionField->MemberID);
		//	if (pOrder != NULL)
		//		return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "Order(%s)", pOrder->OrderID.getValue());
		//	CTriggerOrder *pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByMemberID(pMemberActionField->MemberID);
		//	if (pTriggerOrder != NULL)
		//		return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "TriggerOrder(%s)", pTriggerOrder->OrderID.getValue());
		//	pAccount = m_pMDB->m_AccountFactory->findNextByMemberID();
		//}

		theWriteableMember.PositionType = pMemberActionField->PositionType;

		return true;
	}

	if (!pMemberActionField->RiskLevel.isNull())
	{
		if (pMember->RiskLevel > pMemberActionField->RiskLevel)
		{
			CAccount* pAllAccount = m_pMDB->m_AccountFactory->findFirstByMemberID(pMemberActionField->MemberID);
			while (pAllAccount != NULL)
			{
				if (pAllAccount->Balance < 0.0 || pAllAccount->Borrow != 0.0)
					return m_pErrorEngine->reportError(ERROR_InvalidStatus, "AcountID[%s][Currency%s][Balance:%s]<0or[Borrow:%s]!=0",
						pAllAccount->AccountID.getValue(), pAllAccount->Currency.getValue(), pAllAccount->Balance.getString().c_str(), pAllAccount->Borrow.getString().c_str());
				pAllAccount = m_pMDB->m_AccountFactory->findNextByMemberID();
			}
		}
		theWriteableMember.RiskLevel = pMemberActionField->RiskLevel;
	}
	theWriteableMember.SinkType = ST_SinkToDB;
	m_pMDB->m_MemberFactory->update(pMember, &theWriteableMember, pTransaction);
	return true;
}
