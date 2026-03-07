#include "orderTypeWedges.h"
#include "CopyField.h"

bool CStatusControlWedge::insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
{
	if(pOrder->OrderStatus.isNull())
		pOrder->OrderStatus = OS_NoTraded;
	return true;
}

bool CStatusControlWedge::orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction)
{
	return true;
	COrder *pOrder = pOrderAction->getOrder();
	switch (pOrderAction->ActionFlag)
	{
	case AF_Delete:
		switch (pOrder->OrderStatus)
		{
		case OS_AllCanceled:
		case OS_PartCanceled:
		case OS_AllTraded:
			///已经撤掉的单，不能再撤单了
			return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND);
		case OS_PartTraded:
		case OS_NoTraded:
			//			pOrderAction->Tradable = false;
		{
		}
		}
		break;
	case AF_Modify:
		switch (pOrder->OrderStatus)
		{
		case OS_AllCanceled:
		case OS_PartCanceled:
		case OS_AllTraded:
			///已经撤掉的单，不能再改单了
			return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND);
		case OS_PartTraded:
		case OS_NoTraded:
		{
		}
		//			pOrderAction->Tradable = pOrderAction->getOrder()->Tradable;
		}
		break;
	case AF_Suspend:
		//switch (pOrder->OrderStatus)
		//{
		//case OS_AllTraded:
		//	///已经全部成交，不能再挂起了
		//	m_pErrorEngine->reportError(ERROR_ORDER_HAS_ALL_TRADED);
		//	return false;
		//case OS_Canceled:
		//	///已经撤掉的单，不能再挂起了
		//	m_pErrorEngine->reportError(ERROR_ORDER_HAS_CANCELED);
		//	return false;
		//case OS_PartTradedNotQueueing:
		//case OS_NoTradeNotQueueing:
		//	///正在挂起的报单不能再挂起了
		//	m_pErrorEngine->reportError(ERROR_ORDER_HAD_SUSPENDED);
		//	return false;
		//case OS_PartTraded:
		//case OS_NoTraded:
		//	pOrderAction->Tradable = 1;
		//}
		break;
	case AF_Active:
		//switch (pOrder->OrderStatus)
		//{
		//case OS_AllTraded:
		//	///已经全部成交，不能再激活了
		//	m_pErrorEngine->reportError(ERROR_ORDER_HAS_ALL_TRADED);
		//	return false;
		//case OS_Canceled:
		//	///已经撤掉的单，不能再激活了
		//	m_pErrorEngine->reportError(ERROR_ORDER_HAS_CANCELED);
		//	return false;
		//case OS_PartTraded:
		//case OS_NoTraded:
		//	///目前活跃的报单不能再激活了
		//	m_pErrorEngine->reportError(ERROR_ORDER_HAD_ACTIVED);
		//	return false;
		//case OS_PartTradedNotQueueing:
		//case OS_NoTradeNotQueueing:
		//	pOrderAction->Tradable = 1;
		//}
		break;
	}
	return true;
}
bool CStatusControlWedge::changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction)
{
	setOrderStatus(pChangeList->pOrder);
	return true;
}

bool CStatusControlWedge::insertSingleTrade(CWriteableTrade *pTrade, CWriteableOrder *pOrder)
{
	//if (pOrder != NULL)
	//{
		//pOrder->VolumeTraded += pTrade->Volume;
	//	setOrderStatus(pOrder);
		//setTradable(pOrder);
	//}
	return true;
}

bool CStatusControlWedge::doAfterOrderChange(COrder *pOrder)
{
	return true;
}

///------------
///发现原来的处理中，由于在没有切换至非交易状态时，需要
///检查所有的GFS和GFA的报单，造成效率降低。考虑到这类报单的总数量应当不多，
///因此采用了将其单独保存一个表，只在小范围内找的办法来优化。
///------------
//COrderList::COrderList(void)
//{
//}
//COrderList::~COrderList(void)
//{
//}
//void COrderList::insertOrder(COrder *pOrder)
//{
//	TInstrumentOrderList *pOrderList = getInstrumentOrders(pOrder->getInstrument());
//	if (pOrderList != NULL)
//	{
//		pOrderList->push_back(pOrder);
//	}
//	else
//	{
//		TInstrumentOrderList theOrderList;
//		insert(COrderList::value_type(pOrder->getInstrument(), theOrderList));
//	}
//}
//void COrderList::removeOrders(CInstrument *pInstrument)
//{
//	TInstrumentOrderList *pOrderList = getInstrumentOrders(pInstrument);
//	if (pOrderList != NULL)
//	{
//		pOrderList->clear();
//	}
//}

//TInstrumentOrderList *COrderList::getInstrumentOrders(CInstrument *pInstrument)
//{
//	iterator it = find(pInstrument);
//	if (it == end())
//	{
//		return NULL;
//	}
//	return &(it->second);
//}
//下面是所有相关的字段，以及允许的取值，列在这里供代码检查时的方便
//OrderPriceType:OPT_AnyPrice,OPT_LimitPrice
//Price
//TimeCondition:TC_IOC,TC_GFS,TC_GFD,TC_GTD,TC_GTC,TC_GFA
//GTDTime
//MinVolume
bool COrderTypeCheckWedge::insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
{
	if (pOrder->OrderPriceType == OPT_ByCost)
	{
		if (pOrder->Cost.isNull())
			return m_pErrorEngine->reportError(ERROR_UnSupportOrderType);

		if (pOrder->getInstrument()->ProductClass != PC_AccountExchange)
			return m_pErrorEngine->reportError(ERROR_UnSupportOrderType);
	}

	//if (pOrder->getInstrument()->TradingModel == TM_NoMatch)
	//{
	//	switch (pOrder->DeriveSource)
	//	{
	//	case DS_FromTriggerOrder:
	//	case DS_FromTriggerOrder_SL:
	//	case DS_FromTriggerOrder_TP:
	//	case DS_FromOTCTrade_Liquidation:
	//	{
	//		break;
	//	}
	//	default:
	//	{
	//		return m_pErrorEngine->reportError(ERROR_UnSupportOrderType);
	//	}
	//	}
	//}

	///GTD单必须指定GTDTime
	//if (pOrder->TimeCondition == TC_GTD)
	//{
	//	if (pOrder->GTDTime.isNull())
	//		return m_pErrorEngine->reportError(ERROR_GTD_ORDER_DATE_MISSED);
	//}
	///MV单必须指定MinVolume，且必须小于等于Volume
	if (pOrder->OrderType == OT_FOK)
	{
		return m_pErrorEngine->reportError(ERROR_UnSupportOrderType);

		if (pOrder->MinVolume <= 0.0)
			pOrder->MinVolume = pOrder->Volume;

		if (pOrder->MinVolume > pOrder->Volume)
			return m_pErrorEngine->reportError(ERROR_MV_ORDER_EXCEED_VOLUMN);

		if (pOrder->OrderPriceType == OPT_BestMakerPrice)
			return m_pErrorEngine->reportError(ERROR_MARKET_ORDER_CAN_NOT_QUEUE);
	}

	///------
	///	对于不需要用的报单字段，自动进行清理
	///------
	if (!timeCheck(pOrder))
	{
		return false;
	}

	//if (pOrder->TimeCondition != TC_GTD)
	//{
	//	pOrder->GTDTime = "";
	//}
	if (pOrder->OrderType == OT_Normal || pOrder->OrderType == OT_PostOnly)
	{
		pOrder->MinVolume = 0;
	}

	return true;
}
bool COrderTypeCheckWedge::orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction)
{
	if (pOrderAction->ActionFlag != AF_Active)
	{
		///只有激活报单时需要检查
		return true;
	}
	return timeCheck(pOrderAction->getOrder());
}

bool COrderTypeCheckWedge::timeCheck(COrder *pOrder)
{
	CInstrumentStatusType instrumentStatus;
	CMilliSecondsType today;

	today = g_nCurrentTime;
	instrumentStatus = pOrder->getInstrument()->getMarketData()->InstrumentStatus;

	///GTD单必须指定GTDTime，且该日期必须大于等于今天
	//if (pOrder->TimeCondition == TC_GTD)
	//{
	//	if (pOrder->GTDTime < today)
	//		return m_pErrorEngine->reportError(ERROR_GTD_ORDER_EXPIRED);
	//}
	return true;
}

bool COrderTypeCheckWedge::updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction)
{
	///如果新的合约状态是NoTrading，那么就取消所有GFS和GFA的报单
	while (pInstrumentStatus->InstrumentStatus == IS_NoTrading)
	{
		/*
		COrderIterator *pIterator=COrderIteratorByInstrumentID::alloc(m_pMDB->m_OrderFactory,pInstrumentStatus->InstrumentID);
		COrder *pOrder,*pNextOrder;
		pNextOrder=pIterator->next();
		while (pNextOrder!=NULL)
		{
			pOrder=pNextOrder;
			pNextOrder=pIterator->next();
			if ((pOrder->OrderStatus!=OS_AllTraded)&&(pOrder->OrderStatus!=OS_Canceled)&&
				((pOrder->TimeCondition==TC_GFS)||(pOrder->TimeCondition==TC_GFA)))
			{
				CWriteableOrderAction theOrderAction;
				theOrderAction.init();
				theOrderAction.OrderID=pOrder->OrderID;
				theOrderAction.ActionFlag=AF_Delete;
				theOrderAction.ActionSource=AS_Internal;
				theOrderAction.MemberID=pOrder->MemberID;

				CTransactionSavePoint *pBeforeCancelOrder=CTransactionSavePoint::alloc(pTransaction);
				if (!m_TradeService->orderAction(&theOrderAction,superSession,pTransaction))
				{
					internalWarning("合约状态切换时取消GFS或者GFA报单失败");
					pTransaction->rollbackToSavePoint(pBeforeCancelOrder);
				}
			}
			pIterator->free();
			*/
		//TInstrumentOrderList *pOrderList = m_GFSOrderList.getInstrumentOrders(pInstrumentStatus->getInstrument());
		//if (pOrderList == NULL)
		//{
		//	break;
		//}
		//TInstrumentOrderList::iterator it;
		//for (it = pOrderList->begin(); it < pOrderList->end(); it++)
		//{
		//	COrder *pOrder = *it;
		//	if ((pOrder->OrderStatus != OS_AllTraded) && (pOrder->OrderStatus != OS_Canceled) && (pOrder->TimeCondition == TC_GFS))
		//	{
		//		CWriteableOrderAction theOrderAction;
		//		theOrderAction.init();
		//		theOrderAction.OrderID = pOrder->OrderID;
		//		theOrderAction.ActionFlag = AF_Delete;
		//		theOrderAction.linkOrder(pOrder);
		//		theOrderAction.linkUser(pOrder->getUser());
		//		CTransactionSavePoint *pBeforeCancelOrder = CTransactionSavePoint::alloc(pTransaction);
		//		if (!m_TradeService->orderAction(&theOrderAction, pTransaction))
		//		{
		//			//internalWarning("合约状态切换时取消GFS或者GFA报单失败");
		//			pTransaction->rollbackToSavePoint(pBeforeCancelOrder);
		//		}
		//	}
		//}
	}
	return true;
}

bool COrderTypeCheckWedge::doAfterOrderChange(COrder *pOrder)
{
	if ((pOrder->OrderType == OT_FOK || pOrder->OrderType == OT_FAK) && pOrder->VolumeRemain > 0)
	{
		///如果时间条件是IOC，则进行撤单
		CWriteableOrderAction theOrderAction;
		theOrderAction.init();
		theOrderAction.OrderID = pOrder->OrderID;
		theOrderAction.ActionFlag = AF_Delete;
		//theOrderAction.UserID = pOrder->UserID;
		theOrderAction.linkMember(pOrder->getMember());
		//theOrderAction.linkOrder(pOrder);
		CTransaction theTransaction;
		bool success = m_TradeService->orderAction(&theOrderAction, &theTransaction);
		if (success)
		{
			//这里commit会提交两遍order，但是发送两遍finishorder，这里可以不commit，等到总的commit增加
			//theTransaction.commit();
		}
		else
		{
			//internalWarning("取消IOC单失败");
			theTransaction.rollback();
		}
	}
	//else if (pOrder->TimeCondition == TC_GFS)
	//{
	//	///GFS报单，加入列表中
	//	m_GFSOrderList.insertOrder(pOrder);
	//}
	return true;
}

bool COrderTypeControlWedge::insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
{
	//switch (pOrder->TimeCondition)
	//{
	//case TC_GTC:
	//	return true;
	//case TC_GFS:
	//case TC_GFD:
	//case TC_GTD:
	//default:
	//	m_pErrorEngine->reportError(ERROR_UnSupportOrderType);
	//	return false;
	//}
	return true;
}

//bool COrderTypeControlWedge::insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction* pTransaction)
//{
//	return m_pErrorEngine->reportError(ERROR_UnSupportOrderType);
//}

CAutoSuspendWedge::CAutoSuspendWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
	: CWedge(pTradeService, pMDB, pErrorEngine)
{
	m_pMDB->m_UserSessionFactory->addCommitTrigger(this);
	m_sName = "AutoSuspend";
}

CAutoSuspendWedge::~CAutoSuspendWedge(void)
{
	m_pMDB->m_UserSessionFactory->removeCommitTrigger(this);
}

void CAutoSuspendWedge::commitRemove(CWriteableUserSession *pUserSession)
{
	//TpSession pSession;

	//pSession->frontID = 0;
	//pSession->sessionID = 0;
	/////删除了用户会话，说明发生了登出操作，那就检查所有用户是该用户，而且
	/////目前是可以交易的报单,并且有自动撤单标记的,将其撤单
	//COrderIteratorAll it(m_pMDB->m_OrderFactory);
	//COrder *pOrder, *pNextOrder;
	//pNextOrder = it.next();
	//while (pNextOrder != NULL)
	//{
	//	pOrder = pNextOrder;
	//	pNextOrder = it.next();
	//	if ((pOrder->UserID == pUserSession->UserID) && (pOrder->IsAutoCancle))
	//	{
	//		if ((pOrder->getInstrument()->getMarketData()->InstrumentStatus == IS_Continous) || (pOrder->getInstrument()->getMarketData()->InstrumentStatus == IS_AuctionOrdering))
	//		{
	//			CWriteableOrderAction theOrderAction;

	//			theOrderAction.init();
	//			theOrderAction.OrderID = pOrder->OrderID;
	//			theOrderAction.ActionFlag = AF_Suspend;
	//			theOrderAction.UserID = pOrder->UserID;
	//			///这里无法将此操作合并到本事务中去。不过由于这种挂起应当总是能成功的，
	//			///所以也就无所谓				
	//			if (!m_TradeService->orderAction(&theOrderAction,NULL,NULL))
	//			{
	//				//					internalWarning("自动挂起报单失败");
	//			}
	//		}
	//	}
	//}
	//m_pErrorEngine->getLastError();
}
//
//bool CForceCloseControlWedge::insertOrder(CWriteableOrder *pOrder,CTransaction* pTransaction)
//{
//	///------
//	///	徐忠华，强平市价单将被自动转化为停板价上的限价单
//	///------
//	if (pOrder->OffsetFlag[0] == OF_ForceClose)
//	{
//		if (pOrder->ActionSource == AS_Member)
//		{
//			m_pErrorEngine->reportError(ERROR_FORCE_CLOSE_ONLY_USED_BY_ADMINISTRATOR);
//			return false;
//		}
//		if (pOrder->OrderPriceType == OPT_AnyPrice)
//		{
//			CMarketData *pMarketData;
//			pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pOrder->ExchangeID, pOrder->InstrumentID);
//			if (pMarketData != NULL)
//			{
//				CPriceType limitPrice;
//				limitPrice = "";
//				if (pOrder->Direction == D_Buy)
//				{
//					limitPrice = pMarketData->UpperLimitPrice;
//				}
//				else
//				{
//					limitPrice = pMarketData->LowerLimitPrice;
//				}
//				if (!limitPrice.isNull())
//				{
//					pOrder->OrderPriceType = OPT_LimitPrice;
//					pOrder->TimeCondition = TC_GFD;
//					pOrder->Price = limitPrice;
//				}
//			}
//		}
//	}
//	return true;
//}
//
//bool CForceCloseControlWedge::orderAction(CWriteableOrderAction *pOrderAction,CTransaction *pTransaction)
//{
//	if (pOrderAction->getOrder()->OffsetFlag[0] == OF_ForceClose)
//	{
//		if (pOrderAction->ActionSource == AS_Internal)
//		{
//			return true;
//		}
//		CUserSession *pUserSession;
//		pUserSession = m_pMDB->m_UserSessionFactory->findBySessionNo(pSession->sessionID, pSession->frontID);
//		if (pUserSession == NULL)
//		{
//			///找不到会话
//			return true;
//		}
//		if (pUserSession->UserType != UT_TradeManager)
//		{
//			m_pErrorEngine->reportError(ERROR_FORCE_CLOSE_ONLY_USED_BY_ADMINISTRATOR);
//			return false;
//		}
//	}
//	return true;
//}
