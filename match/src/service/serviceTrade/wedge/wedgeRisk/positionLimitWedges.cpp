#include "positionLimitWedges.h"
#include "monitorIndex.h"
#include "tradingTools.h"
#include "CopyField.h"

CPositionCheckWedge::CPositionCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
	:CWedge(pTradeService, pMDB, pErrorEngine)
{
	m_grossControler = new CGrossPositionTypeControler(pMDB);
	m_netControler = new CNetPositionTypeControler(pMDB);
	m_exchangeControler = new CExchangePositionTypeControler(pMDB);
	//	m_algo = new CBasePositionLimitAlgorithms(pMDB);
	m_sName = "PositionCheck";
}

CPositionCheckWedge::~CPositionCheckWedge(void)
{
}

bool CPositionCheckWedge::enoughPosition(CInstrument *pInstrument, CPosition *pOldPosition, CWriteablePosition *pNewPosition, COrder *pOrder)
{
	if (pInstrument->MaxOpenInterest.isNull() || pInstrument->MaxOpenInterest == 0.0)
		return true;

	if (pInstrument->getMarketData()->OpenInterest < pInstrument->MaxOpenInterest)
		return true;

	//如果是平仓订单，不需要检查
	bool bOpen = false;
	switch (pNewPosition->PosiDirection)
	{
	case PD_Long:
	{
		if (pNewPosition->LongFrozen > pOldPosition->LongFrozen)
			bOpen = true;
		break;
	}
	case PD_Short:
	{
		if (pNewPosition->ShortFrozen > pOldPosition->ShortFrozen)
			bOpen = true;
		break;
	}
	case PD_Net:
	{
		if (pNewPosition->Position > 0)
		{
			if (pNewPosition->LongFrozen > pOldPosition->LongFrozen)
				bOpen = true;
		}
		else if (pNewPosition->Position < 0)
		{
			if (pNewPosition->ShortFrozen > pOldPosition->ShortFrozen)
				bOpen = true;
		}
		else
		{
			if (pNewPosition->LongFrozen > pOldPosition->LongFrozen)
				bOpen = true;
			if (pNewPosition->ShortFrozen > pOldPosition->ShortFrozen)
				bOpen = true;
		}
		break;
	}
	}

	if (!bOpen)
		return true;

	switch (pInstrument->ProductClass)
	{
	case PC_Margin:
	case PC_Premium:
		break;
	default:
		return true;
	}

	////接管的情况不进行限制检查
	switch (pOrder->DeriveSource)
	{
	case DS_FromOTCTrade_Liquidation:
	case DS_FromOTCTrade_ADL:
		return true;
	}

	if (pOrder->Priority == 0)
		return true;

	return m_pErrorEngine->reportError(ERROR_POSITION_LIMIT_EXCEED, "Market[%s]>=Max[%s]",
		pInstrument->getMarketData()->OpenInterest.getString().c_str(), pInstrument->MaxOpenInterest.getString().c_str());
}

IPositionTypeControler* CPositionCheckWedge::getControler(CInstrument *pInstrument, COrder *pOrder)
{
	//switch (pInstrument->ProductClass)
	//{
	//case PC_Exchange:
	//	return m_exchangeControler;
	//default:
	//{
	//	switch (pOrder->getMember()->PositionType)
	//		//switch (getPositionType(m_pMDB, pOrder->MemberID))
	//	{
	//	case PT_Net:
	//		return m_netControler;
	//	default:
	//		return m_grossControler;
	//	}
	//}
	//}

	switch (pInstrument->ProductClass)
	{
	case PC_AccountExchange:
		return m_exchangeControler;
	default:
	{
		switch (pOrder->PosiDirection)
			//switch (getPositionType(m_pMDB, pOrder->MemberID))
		{
		case PD_Net:
			return m_netControler;
		default:
			return m_grossControler;
		}
	}
	}
}

bool CPositionCheckWedge::insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
{
	IPositionTypeControler *m_controler = getControler(pOrder->getInstrument(), pOrder);

	///找到对应的持仓记录
	CPosition *pPosition = pOrder->getPosition();
	m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
	m_controler->initControler(pOrder, &thePosition);

	CPriceType orderPrice = pOrder->Price;
	if (m_controler != m_exchangeControler)
		orderPrice = getFrozenMarginPrice(m_pMDB, pOrder->getInstrument(), pOrder->Price, pOrder->Direction);

	if (!m_controler->calculateNewFrozen(&thePosition, pOrder->Direction, pOrder->OffsetFlag, pOrder->Volume, orderPrice))
		return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE);

	///设置各种引用
	m_controler->getPositionChangeList()->pOrder = pOrder;
	m_controler->getPositionChangeList()->pTrade = NULL;
	m_controler->getPositionChangeList()->pOrderCancle = NULL;
	//	m_controler->getPositionChangeList()->pTradeUnit = pOrder->getTradeUnit();

	//pOrder->OrderID = m_TradeService->m_pOrderSequence->getNextValue();

	///看看其他规则是否不同意进行此项变化
	if (!m_TradeService->changePosition(m_controler->getPositionChangeList(), pTransaction))
		return false;

	if (!enoughPosition(pOrder->getInstrument(), pPosition, &thePosition, pOrder))
		return false;

	//记录要平仓数量
	/*if (pOrder->OffsetFlag != OF_Open) {
		if (thePosition.ClosePosition.isNull())
			thePosition.ClosePosition = 0;
		thePosition.ClosePosition += pOrder->Volume;
	}*/
//	thePosition.UserID = pOrder->UserID;
	thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_PositionFactory->update(pPosition, &thePosition, pTransaction);
	pOrder->Leverage = thePosition.Leverage;
	pOrder->IsCrossMargin = thePosition.IsCrossMargin;
	return true;
}

bool CPositionCheckWedge::orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction)
{
	///获取报单
	COrder *pOrder = pOrderAction->getOrder();

	IPositionTypeControler *m_controler = getControler(pOrder->getInstrument(), pOrder);
	m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);

	CPosition *pPosition = pOrder->getPosition();
	thePosition.init(m_pMDB);
	if (pPosition == NULL)
	{
		if (!InitPosition(m_pMDB, m_pErrorEngine, pOrder->getInstrument(), &thePosition, pOrder->MemberID, pOrder->TradeUnitID.getValue(), pOrder->AccountID, pOrder->PosiDirection, pOrder->IsCrossMargin, pOrder->Leverage))
			return false;
	}
	else
		m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);

	m_controler->initControler(&theOrder, &thePosition);

	///计算新的冻结持仓量
	CVolumeType volumeChange;
	CPriceType orderPrice;

	switch (pOrderAction->ActionFlag)
	{
	case AF_Delete:
	{
		//撤单或者挂起，则需要返还冻结量
		//if (theOrder.VolumeTraded == 0)
		//	theOrder.OrderStatus = OS_AllCanceled;
		//else
		//	theOrder.OrderStatus = OS_PartCanceled;
		theOrder.VolumeRemain = 0.0;
		theOrder.VolumeCancled = theOrder.Volume - theOrder.VolumeRemain - theOrder.VolumeTraded;
		
		//删除订单根据订单已经发生额来决定释放，不需要价格，直接使用订单价格即可
		if (!m_controler->calculateNewFrozen(&thePosition, pOrder->Direction, pOrder->OffsetFlag, -pOrder->VolumeRemain, pOrder->Price))
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE);
		m_controler->getPositionChangeList()->pOrderCancle = pOrderAction;
		break;
	}
	case AF_Suspend:
	{
		theOrder.Tradable = false;
		break;
	}
	case AF_Active:
	{
		//修改持仓中的冻结量
		theOrder.TimeSortNo = m_gSortSequence.getNextValueByDouble();
		theOrder.Tradable = true;
		orderPrice = getFrozenMarginPrice(m_pMDB, pOrder->getInstrument(), pOrder->Price, pOrder->Direction);
		if (!m_controler->calculateNewFrozen(&thePosition, pOrder->Direction, pOrder->OffsetFlag, theOrder.VolumeRemain, orderPrice))
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE);
		break;
	}
	case AF_Modify:
	{
		//改单，需要返还原来的冻结量，计算新的冻结量
		if (pOrderAction->Volume <= theOrder.VolumeTraded)
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_VOLUME_TO_MODIFY);

		if (!m_controler->calculateNewFrozen(&thePosition, pOrder->Direction, pOrder->OffsetFlag, -pOrder->VolumeRemain, pOrder->Price))
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE);

		//完成数量的修改
		theOrder.Volume = pOrderAction->Volume;
		theOrder.VolumeRemain = theOrder.Volume - theOrder.VolumeTraded;

		if (theOrder.Volume > pOrder->Volume)
			theOrder.TimeSortNo = m_gSortSequence.getNextValueByDouble();

		if (pOrderAction->Price != theOrder.Price)
		{
			theOrder.Price = pOrderAction->Price;
			theOrder.TimeSortNo = m_gSortSequence.getNextValueByDouble();
		}
		if (!pOrderAction->VolumeDisplay.isNull())
		{
			theOrder.VolumeDisplay = pOrderAction->VolumeDisplay;
		}

		orderPrice = getFrozenMarginPrice(m_pMDB, pOrder->getInstrument(), pOrderAction->Price, pOrder->Direction);
		if (!m_controler->calculateNewFrozen(&thePosition, pOrder->Direction, pOrder->OffsetFlag, theOrder.VolumeRemain, orderPrice))
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE);
		//进行限仓检查
		if (!enoughPosition(pOrder->getInstrument(), pPosition, &thePosition, pOrder))
			return false;
		break;
	}
	case AF_Change:
	{
		//改单，需要返还原来的冻结量，计算新的冻结量
		if (pOrder->VolumeRemain <= -pOrderAction->Volume)
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_VOLUME_TO_MODIFY);

		if (!m_controler->calculateNewFrozen(&thePosition, pOrder->Direction, pOrder->OffsetFlag, -pOrder->VolumeRemain, pOrder->Price))
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE);

		//完成数量的修改
		theOrder.Volume += pOrderAction->Volume;
		theOrder.VolumeRemain += pOrderAction->Volume;

		//数量增加，需要重新排队
		if (pOrderAction->Volume > 0.0)
			theOrder.TimeSortNo = m_gSortSequence.getNextValueByDouble();
		///价格变化，需要重新排队
		if (pOrderAction->Price != theOrder.Price)
		{
			theOrder.Price = pOrderAction->Price;
			theOrder.TimeSortNo = m_gSortSequence.getNextValueByDouble();
		}

		orderPrice = getFrozenMarginPrice(m_pMDB, pOrder->getInstrument(), pOrderAction->Price, pOrder->Direction);
		if (!m_controler->calculateNewFrozen(&thePosition, pOrder->Direction, pOrder->OffsetFlag, theOrder.VolumeRemain, orderPrice))
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE);
		//进行限仓检查
		if (!enoughPosition(pOrder->getInstrument(), pPosition, &thePosition, pOrder))
			return false;
		break;
	}
	}

	//设置各种引用
	m_controler->getPositionChangeList()->pOrder = &theOrder;
	m_controler->getPositionChangeList()->pOldOrder = pOrder;
	m_controler->getPositionChangeList()->pTrade = NULL;
	m_controler->getPositionChangeList()->pOldPosition = pPosition;
	m_controler->getPositionChangeList()->pNewPosition = &thePosition;
	//	m_controler->getPositionChangeList()->pTradeUnit = pOrder->getTradeUnit();

	//看看其他规则是否不同意进行此项变化
	if (!m_TradeService->changePosition(m_controler->getPositionChangeList(), pTransaction))
		return false;


//	thePosition.UserID = theOrder.UserID;
	//if (m_controler != m_exchangeControler)
	//{
		//double tick = pOrder->getInstrument()->VolumeTick.getValue();
		//thePosition.LongFrozen.toTick(tick);
		//thePosition.ShortFrozen.toTick(tick);
		//thePosition.PreLongFrozen.toTick(tick);
		//thePosition.PreShortFrozen.toTick(tick);
//	}
	thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_PositionFactory->update(pPosition, &thePosition, pTransaction);

	theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_OrderFactory->update(pOrder, &theOrder, pTransaction);
	return true;
}

bool CPositionCheckWedge::insertSingleTrade(CWriteableTrade *pTrade, CWriteableOrder *pOrder)
{
	pOrder->VolumeTraded += pTrade->Volume;
	IPositionTypeControler *m_controler = getControler(pOrder->getInstrument(), pOrder);
	///找到对应的持仓记录
	CPosition *pPosition = pOrder->getPosition();
	thePosition.init();
	if (pPosition == NULL)
	{
		if (!InitPosition(m_pMDB, m_pErrorEngine, pOrder->getInstrument(), &thePosition, pOrder->MemberID, pOrder->TradeUnitID.getValue(), pOrder->AccountID, pOrder->PosiDirection, pOrder->IsCrossMargin, pOrder->Leverage))
			return false;
		pPosition = m_pMDB->m_PositionFactory->add(&thePosition);
		pOrder->linkPosition(pPosition);
	}
	else
	{
		m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
	}

	m_controler->initControler(pOrder, &thePosition);

	///计算新的持仓
	CReadOnlyPriceType *pOrderPrice;
	if (pOrder == NULL)
		pOrderPrice = NULL;
	else
		pOrderPrice = &(getFrozenMarginPrice(m_pMDB, pOrder->getInstrument(), pOrder->Price, pOrder->Direction));

	CPriceType tradePrice = getMarginPrice(pOrder->getInstrument(), pTrade);

	//if (!m_controler->calculateNewPosition(&thePosition, pTrade->Direction, pTrade->OffsetFlag, pTrade->Volume, tradePrice, pOrderPrice))
	//	return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE);

	m_controler->calculateNewPosition(&thePosition, pTrade->Direction, pTrade->OffsetFlag, pTrade->Volume, tradePrice, pOrderPrice);

	/////计算成交量
	//if (pTrade->Direction == D_Buy)
	//	thePosition.BuyVolume += pTrade->Volume;
	//else
	//	thePosition.SellVolume += pTrade->Volume;

	///用掉和返还限仓
	CVolumeType longOpenVolume = 0, shortOpenVolume = 0, longCloseVolume = 0, shortCloseVolume = 0;
	m_controler->getPositionChangeList()->calculateOpenVolume(&longOpenVolume, &shortOpenVolume);
	m_controler->getPositionChangeList()->calculateCloseVolume(&longCloseVolume, &shortCloseVolume);
	//if (m_algo != NULL)
	//{
	//	m_algo->usePositionLimit(pOrder->getInstrument(), &thePosition, longOpenVolume, shortOpenVolume, pTransaction);
	//	m_algo->returnPositionLimit(pOrder->getInstrument(), &thePosition, longCloseVolume, shortCloseVolume, pTransaction);
	//}

	///设置各种引用
	m_controler->getPositionChangeList()->pOrder = pOrder;
	m_controler->getPositionChangeList()->pTrade = pTrade;
	m_controler->getPositionChangeList()->pOldOrder = m_pMDB->m_OrderFactory->findByOrderID(pOrder->OrderID);
	m_controler->getPositionChangeList()->pOrderCancle = NULL;
	//	m_controler->getPositionChangeList()->pTradeUnit = pTrade->getTradeUnit();

	//看看其他规则是否不同意进行此项变化
	m_TradeService->changePosition(m_controler->getPositionChangeList(), NULL);

	//记录成交后自减仓数量变更
	if (thePosition.PosiDirection == PD_Net)
	{
		if (pOrder->OffsetFlag != OF_Open)
		{
			thePosition.ClosePosition = thePosition.ClosePosition - longCloseVolume - shortCloseVolume;
			if (thePosition.ClosePosition < pOrder->getInstrument()->VolumeTick)
				thePosition.ClosePosition = 0.0;
		}
	}

//	thePosition.UserID = pOrder->UserID;
	//if (m_controler != m_exchangeControler)
	//{
	//	double tick = pOrder->getInstrument()->VolumeTick.getValue();
	//	thePosition.PrePosition.toTick(tick);
	//	thePosition.Position.toTick(tick);
	//}
	thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	if (fabs(thePosition.Position) > fabs(thePosition.HighestPosition))
		thePosition.HighestPosition = thePosition.Position;

	//if (pPosition == NULL)
	//	pOrder->linkPosition(m_pMDB->m_PositionFactory->add(&thePosition, pTransaction));
	//else
	//	m_pMDB->m_PositionFactory->addOrUpdate(pPosition, &thePosition, pTransaction);

	if (pTrade->MatchRole == MR_Taker)
		m_pMDB->m_PositionFactory->update(pPosition, &thePosition, NULL, false);
	else
		m_pMDB->m_PositionFactory->update(pPosition, &thePosition);

	return true;
}

CMemberPositionCheckWedge::CMemberPositionCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
	:CWedge(pTradeService, pMDB, pErrorEngine)
{
	//m_algo = new CBaseMemberPositionLimitAlgorithms(pMDB);
	m_sName = "MemberPositionCheck";
}

CMemberPositionCheckWedge::~CMemberPositionCheckWedge(void)
{
	//delete m_algo;
}

CMemberPosition* CMemberPositionCheckWedge::setMemberPosition(CAccountIDType AccountID, CPositionChangeList *pChangeList, CTransaction *pTransaction)
{
	CMemberPosition *pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(AccountID, pChangeList->pOrder->ExchangeID,
		pChangeList->pOrder->InstrumentID);
	if (pMemberPosition == NULL)
	{
		theMemberPosition.ExchangeID = pChangeList->pOrder->ExchangeID;
		theMemberPosition.AccountID = AccountID;
		theMemberPosition.InstrumentID = pChangeList->pOrder->InstrumentID;
		//theMemberPosition.DefaultLeverage = pChangeList->pInstrument->DefaultLeverage;
		theMemberPosition.DefaultLeverage = getDefaultLvgWithMargin(m_pMDB, pChangeList->pInstrument, pChangeList->pOrder->MemberID, AccountID, pChangeList->pOrder->PosiDirection, pChangeList->pRate);
		theMemberPosition.DefaultCrossMargin = true;
		theMemberPosition.Position = pChangeList->pNewPosition->Position.getValueAbs();
		theMemberPosition.LongFrozen = pChangeList->pNewPosition->LongFrozen;
		theMemberPosition.ShortFrozen = pChangeList->pNewPosition->ShortFrozen;
		if (pChangeList->pTrade != NULL)
			pMemberPosition = m_pMDB->m_MemberPositionFactory->add(&theMemberPosition);
		else
			pMemberPosition = m_pMDB->m_MemberPositionFactory->add(&theMemberPosition, pTransaction);
	}
	else
	{
		m_pMDB->m_MemberPositionFactory->retrieve(pMemberPosition, &theMemberPosition);
		if (pChangeList->pOldPosition == NULL)
		{
			theMemberPosition.Position += pChangeList->pNewPosition->Position;
			theMemberPosition.LongFrozen += pChangeList->pNewPosition->LongFrozen;
			theMemberPosition.ShortFrozen += pChangeList->pNewPosition->ShortFrozen;
			//theMemberPosition.PrePosition += pChangeList->pNewPosition->PrePosition;
			//theMemberPosition.PreLongFrozen += pChangeList->pNewPosition->PreLongFrozen;
			//theMemberPosition.PreShortFrozen += pChangeList->pNewPosition->PreShortFrozen;
		}
		else
		{
			theMemberPosition.Position += pChangeList->pNewPosition->Position - pChangeList->pOldPosition->Position;
			theMemberPosition.LongFrozen += pChangeList->pNewPosition->LongFrozen - pChangeList->pOldPosition->LongFrozen;
			theMemberPosition.ShortFrozen += pChangeList->pNewPosition->ShortFrozen - pChangeList->pOldPosition->ShortFrozen;
			//theMemberPosition.PrePosition += pChangeList->pNewPosition->PrePosition - pChangeList->pOldPosition->PrePosition;
			//theMemberPosition.PreLongFrozen += pChangeList->pNewPosition->PreLongFrozen - pChangeList->pOldPosition->PreLongFrozen;
			//theMemberPosition.PreShortFrozen += pChangeList->pNewPosition->PreShortFrozen - pChangeList->pOldPosition->PreShortFrozen;
		}

		double VolumeTick = pChangeList->pOrder->getInstrument()->VolumeTick.getValue();
		theMemberPosition.Position.toLowerTick(VolumeTick);
		theMemberPosition.LongFrozen.toLowerTick(VolumeTick);
		theMemberPosition.ShortFrozen.toLowerTick(VolumeTick);
		if (pChangeList->pTrade != NULL)
			m_pMDB->m_MemberPositionFactory->addOrUpdate(pMemberPosition, &theMemberPosition);
		else
			m_pMDB->m_MemberPositionFactory->addOrUpdate(pMemberPosition, &theMemberPosition, pTransaction);
	}
	return pMemberPosition;
}

bool CMemberPositionCheckWedge::changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction)
{
	//合约和期权需要，其他不需要
	switch (pChangeList->pInstrument->ProductClass)
	{
	case PC_Margin:
	case PC_Premium:
		break;
	default:
		return true;
	}

	//CMemberPosition* pMemberPosition = setMemberPosition("", pChangeList, pTransaction);
	//if (pMemberPosition == NULL)
	//	m_pErrorEngine->reportError(ERROR_INVALID_VALUE); //不太可能出现，这里为了程序不能core而已

	///对于报单产生成交的情况，不进行限仓检查
	//if (pChangeList->pTrade != NULL)
	//	return true;

	///进行限仓检查
	//if (!enoughPosition(pOrder->getInstrument(), pPosition, &thePosition, pOrder, pTransaction))
	//{
	//	return false;
	//}

	////平仓、撤单不需要检查限仓
	//if (pChangeList->isOpen() && pChangeList->pOrderCancle == NULL && pChangeList->pTrade == NULL)
	//{
	//	CPositionLimit *pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(pChangeList->pOrder->MemberID, defaultTradeUnitID, pChangeList->pOrder->ExchangeID, pChangeList->pInstrument->InstrumentID);
	//	if (pPositionLimit == NULL)
	//		pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(pChangeList->pOrder->MemberID, defaultTradeUnitID, pChangeList->pOrder->ExchangeID, pChangeList->pInstrument->ProductID.getValue());
	//	if (pPositionLimit == NULL)
	//		pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(pChangeList->pOrder->MemberID, defaultTradeUnitID, pChangeList->pOrder->ExchangeID, pChangeList->pInstrument->ProductGroup.getValue());
	//	if (pPositionLimit == NULL)
	//		pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(pChangeList->pOrder->MemberID, defaultTradeUnitID, pChangeList->pOrder->ExchangeID, defaultInstrumentID);
	//	if (pPositionLimit == NULL)
	//		pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(pChangeList->pOrder->MemberID, defaultTradeUnitID, defaultExchangeID, defaultInstrumentID);

	//	//----------------------
	//	if (pPositionLimit == NULL)
	//	{
	//		CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(pChangeList->pOrder->MemberID);
	//		if (pMember != NULL)
	//		{
	//			pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(pMember->Grade.getValue(), defaultTradeUnitID, pChangeList->pOrder->ExchangeID, pChangeList->pInstrument->InstrumentID);
	//			if (pPositionLimit == NULL)
	//				pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(pMember->Grade.getValue(), defaultTradeUnitID, pChangeList->pOrder->ExchangeID, pChangeList->pInstrument->ProductID.getValue());
	//			if (pPositionLimit == NULL)
	//				pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(pMember->Grade.getValue(), defaultTradeUnitID, pChangeList->pOrder->ExchangeID, pChangeList->pInstrument->ProductGroup.getValue());
	//			if (pPositionLimit == NULL)
	//				pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(pMember->Grade.getValue(), defaultTradeUnitID, pChangeList->pOrder->ExchangeID, defaultInstrumentID);
	//			if (pPositionLimit == NULL)
	//				pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(pMember->Grade.getValue(), defaultTradeUnitID, defaultExchangeID, defaultInstrumentID);
	//		}
	//	}

	//	//----------------------
	//	if (pPositionLimit == NULL)
	//		pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(defaultMemberID, defaultTradeUnitID, pChangeList->pOrder->ExchangeID, pChangeList->pInstrument->InstrumentID);
	//	if (pPositionLimit == NULL)
	//		pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(defaultMemberID, defaultTradeUnitID, pChangeList->pOrder->ExchangeID, pChangeList->pInstrument->ProductID.getValue());
	//	if (pPositionLimit == NULL)
	//		pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(defaultMemberID, defaultTradeUnitID, pChangeList->pOrder->ExchangeID, pChangeList->pInstrument->ProductGroup.getValue());
	//	if (pPositionLimit == NULL)
	//		pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(defaultMemberID, defaultTradeUnitID, pChangeList->pOrder->ExchangeID, defaultInstrumentID);
	//	if (pPositionLimit == NULL)
	//		pPositionLimit = m_pMDB->m_PositionLimitFactory->findByTradeUnitAndInstrument(defaultMemberID, defaultTradeUnitID, defaultExchangeID, defaultInstrumentID);

	//	if (pPositionLimit != NULL)
	//	{
	//		CVolumeType TotalPosition = pMemberPosition->Position;
	//		if (TotalPosition < 0.0)
	//			TotalPosition = -TotalPosition;
	//		if (pMemberPosition->PosiDirection == PD_Long || pMemberPosition->PosiDirection == PD_Net)
	//		{
	//			if ((TotalPosition + pMemberPosition->LongFrozen) > pPositionLimit->LongPosLimit)
	//				return m_pErrorEngine->reportError(ERROR_POSITION_LIMIT_EXCEED, "[%0.4f]+[%0.4f]>[%0.4f]", TotalPosition.getValue(), pMemberPosition->LongFrozen.getValue(), pPositionLimit->LongPosLimit.getValue());
	//		}
	//		if (pMemberPosition->PosiDirection == PD_Short || pMemberPosition->PosiDirection == PD_Net)
	//		{
	//			if ((TotalPosition + pMemberPosition->ShortFrozen) > pPositionLimit->ShortPosLimit)
	//				return m_pErrorEngine->reportError(ERROR_POSITION_LIMIT_EXCEED, "[%0.4f]+[%0.4f]>[%0.4f]", TotalPosition.getValue(), pMemberPosition->ShortFrozen.getValue(), pPositionLimit->ShortPosLimit.getValue());
	//		}
	//	}
	//}

	if (pChangeList->pRate == NULL)
	{
		CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pChangeList->pOrder->MemberID, pChangeList->pOrder->AccountID, pChangeList->pOrder->SettlementGroup, pChangeList->pOrder->ClearCurrency);
		pChangeList->pRate = getMarginRate(m_pMDB, pChangeList->pInstrument->MarginRateGroup.getValue(), pAccount);
		if (pChangeList->pRate == NULL)
			return m_pErrorEngine->reportError(ERROR_CAN_NOT_FIND_MARGIN_RATE);
	}

	CMemberPosition* pMemberPosition = setMemberPosition(pChangeList->pOrder->AccountID, pChangeList, pTransaction);

	if (pChangeList->isOpen() && pChangeList->pOrderCancle == NULL && pChangeList->pTrade == NULL)
	{
		//接管账户开仓不需要限制杠杆
		if (pChangeList->pOrder->DeriveSource != DS_FromOTCTrade_Liquidation)
		{
			CRatioType minMarginRatio = getMinMarginRatio(pMemberPosition->Position, pMemberPosition->LongFrozen, pMemberPosition->ShortFrozen, pChangeList->pInstrument, pChangeList->pRate, pChangeList->pNewPosition->PosiDirection);
			double Leverage = 1.0;
			if (pChangeList->pRate->ValueMode == VM_Absolute)
				Leverage = -minMarginRatio;
			else
				Leverage = minMarginRatio.getInverse();

			//这里需要找到所有订单的Position的最大杠杆
			if (pChangeList->pNewPosition->Leverage > Leverage)
				return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "PositionLeveage[%.4f]>MaxLeveage[%.4f]", pChangeList->pNewPosition->Leverage.getValue(), Leverage);

			//这里不能用全局的遍历，不确定上层是否使用了全局遍历，如果上层使用了，这里使用就会冲突，导致无法预料的结果
			CPositionIteratorByAccountInstrument iter(m_pMDB->m_PositionFactory, pChangeList->pNewPosition->AccountID, pChangeList->pNewPosition->ExchangeID, pChangeList->pNewPosition->InstrumentID);
			CPosition* pPosition = iter.next();
			while (pPosition != NULL)
			{
				if (pPosition->Position > 0 || pPosition->LongFrozen > 0 || pPosition->ShortFrozen > 0)
				{
					if (pPosition->Leverage > Leverage && pPosition->PosiDirection == pChangeList->pNewPosition->PosiDirection)
						return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "PositionLeveage[%.4f]>MaxLeveage[%.4f]", pPosition->Leverage.getValue(), Leverage);
				}
				pPosition = iter.next();
			}
		}
	}
	return true;
	//return enoughPosition(pChangeList, pLongMemberPosition, pShortMemberPosition);
}

CMemberPosition *CMemberPositionCheckWedge::findMemberPosition(CInstrument *pInstrument,
	CReadOnlyLongIndexIDType &MemberID,
	CPosiDirectionType posiDirection,
	CWriteableMemberPosition *pMemberPosition)
{
	//CMemberPosition *pOriginalMemberPosition;
	//pOriginalMemberPosition = m_pMDB->m_MemberPositionFactory->findByPartAndInstrument(
	//	MemberID,
	//	pInstrument->ExchangeID,
	//	pInstrument->InstrumentID,
	//	posiDirection);
	//if (pMemberPosition != NULL)
	//{
	//	if (pOriginalMemberPosition != NULL)
	//	{
	//		m_pMDB->m_MemberPositionFactory->retrieve(pOriginalMemberPosition, pMemberPosition);
	//	}
	//	else
	//	{
	//		pMemberPosition->init();
	//		pMemberPosition->PosiDirection = posiDirection;
	//		pMemberPosition->InstrumentID = pInstrument->InstrumentID;
	//		pMemberPosition->ProductGroup = pInstrument->ProductGroup;
	//		pMemberPosition->MemberID = MemberID;
	//	}
	//}
	//return pOriginalMemberPosition;
	return NULL;
}

CVolumeType CMemberPositionCheckWedge::getTotalMemberPosition(CMemberPosition *pMemberPosition, CPosiDirectionType posiDirection)
{
	CVolumeType result;
	result = pMemberPosition->Position;
	if (posiDirection == PD_Long)
		result += pMemberPosition->LongFrozen;
	else
		result += pMemberPosition->ShortFrozen;
	return result;
}

CPositionOrderWedge1::CPositionOrderWedge1(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
	:CWedge(pTradeService, pMDB, pErrorEngine)
{
	m_sName = "PositionOrder";
}

CPositionOrderWedge1::~CPositionOrderWedge1()
{
}
bool CPositionOrderWedge1::insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
{
	//指定平仓ID的情况下，数量不能大于可平量,未填数量赋值为最大可平数量
	if (!pOrder->CloseOrderID.isNull())
	{
		if (pOrder->OffsetFlag == OF_Open)
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "OrderNotCloseWhenHasCloseOrderID[%s]", pOrder->CloseOrderID.getValue());

		CPositionOrder *pPositionOrder = m_pMDB->m_PositionOrderFactory->findByOrderID(pOrder->CloseOrderID);
		if (pPositionOrder == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "PositionOrder[%s]NotFound", pOrder->CloseOrderID.getValue());

		if (pPositionOrder->Direction == pOrder->Direction)
			return m_pErrorEngine->reportError(ERROR_BAD_FIELD, "DirectionError:PositionOrder[%c]==Order[%c]", pPositionOrder->Direction.getValue(),
				pOrder->Direction.getValue());

		if (pOrder->Volume.isNull())
		{
			pOrder->Volume = pPositionOrder->VolumeToClose - pPositionOrder->VolumeClosed - pPositionOrder->VolumeOnClose;
		}
		else
		{
			if (pOrder->Volume > pPositionOrder->VolumeToClose - pPositionOrder->VolumeClosed - pPositionOrder->VolumeOnClose)
				return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE, "PositionOrder[%s]", pOrder->CloseOrderID.getValue());
		}
		if (pOrder->BusinessType == BT_CloseOrderBusinessValue)
			pOrder->BusinessResult = pPositionOrder->BusinessResult;

		m_pMDB->m_PositionOrderFactory->retrieve(pPositionOrder, &thePositionOrder);
		thePositionOrder.VolumeOnClose += pOrder->Volume;
		m_pMDB->m_PositionOrderFactory->update(pPositionOrder, &thePositionOrder, pTransaction);
	}
	return true;
}

bool CPositionOrderWedge1::orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction)
{
	if (!pOrderAction->getOrder()->CloseOrderID.isNull())
	{
		CPositionOrder *pPositionOrder = m_pMDB->m_PositionOrderFactory->findByOrderID(pOrderAction->getOrder()->CloseOrderID);
		if (pPositionOrder != NULL)
		{
			m_pMDB->m_PositionOrderFactory->retrieve(pPositionOrder, &thePositionOrder);
			thePositionOrder.VolumeOnClose -= pOrderAction->getOrder()->VolumeRemain;
			m_pMDB->m_PositionOrderFactory->update(pPositionOrder, &thePositionOrder, pTransaction);
		}
	}
	return true;
}


CPositionOrderWedge::CPositionOrderWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
	:CWedge(pTradeService, pMDB, pErrorEngine)
{
	m_sName = "PositionOrder";
}

CPositionOrderWedge::~CPositionOrderWedge()
{
}

bool CPositionOrderWedge::changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction)
{
	for (int i = 0;; i++)
	{
		CPositionChange *pPositionChange = pChangeList->get(i);
		if (pPositionChange == NULL)
			break;

		if (pPositionChange->positionChangeType != IS_TRADE)
			continue;

		if (pPositionChange->isOpen)
		{
			CPositionOrder *pPositionOrder = m_pMDB->m_PositionOrderFactory->findByOrderID(pChangeList->pOrder->OrderID);
			if (pPositionOrder == NULL)
			{
				thePositionOrder.init();
				CopyOrderEntity(&thePositionOrder, pChangeList->pOrder);
				thePositionOrder.VolumeToClose = pPositionChange->volumeChange;
				m_pMDB->m_PositionOrderFactory->add(&thePositionOrder);
			}
			else
			{
				m_pMDB->m_PositionOrderFactory->retrieve(pPositionOrder, &thePositionOrder);
				CopyOrderEntity(&thePositionOrder, pChangeList->pOrder);
				thePositionOrder.VolumeToClose += pPositionChange->volumeChange;
				m_pMDB->m_PositionOrderFactory->update(pPositionOrder, &thePositionOrder);
			}
		}
		else
		{
			if (pChangeList->pOrder->CloseOrderID.isNull())	//没有指定平仓ID的情况下，按照先进先出，不记录VolumeOnClose在途订单
			{
				CPositionOrder *pPositionOrder = m_pMDB->m_PositionOrderFactory->findFirstByInsertTime(pChangeList->pOrder->MemberID,
					pChangeList->pOrder->TradeUnitID, pChangeList->pOrder->ExchangeID, pChangeList->pOrder->InstrumentID, pChangeList->pOrder->PosiDirection);
				CVolumeType ToCloseVolume = -pPositionChange->volumeChange;
				while (pPositionOrder != NULL)
				{
					if (pPositionOrder->VolumeToClose > ToCloseVolume + pPositionOrder->VolumeClosed)
					{
						m_pMDB->m_PositionOrderFactory->retrieve(pPositionOrder, &thePositionOrder);
						thePositionOrder.VolumeClosed += ToCloseVolume;
						m_pMDB->m_PositionOrderFactory->update(pPositionOrder, &thePositionOrder);
						pPositionOrder = m_pMDB->m_PositionOrderFactory->findNextByInsertTime();
						break;
					}
					else
					{
						ToCloseVolume -= pPositionOrder->VolumeToClose - pPositionOrder->VolumeClosed;
						CPositionOrder *pPositionOrderTobeRemove = pPositionOrder;
						pPositionOrder = m_pMDB->m_PositionOrderFactory->findNextByInsertTime();
						m_pMDB->m_PositionOrderFactory->remove(pPositionOrderTobeRemove);
						if (ToCloseVolume <= 0.0)
							break;
					}
				}
			}
			else
			{
				CPositionOrder *pPositionOrder = m_pMDB->m_PositionOrderFactory->findByOrderID(pChangeList->pOrder->CloseOrderID);
				if (pPositionOrder != NULL)
				{
					if (pPositionOrder->VolumeClosed >= pPositionOrder->VolumeToClose + pPositionChange->volumeChange)
					{
						m_pMDB->m_PositionOrderFactory->remove(pPositionOrder);
					}
					else
					{
						m_pMDB->m_PositionOrderFactory->retrieve(pPositionOrder, &thePositionOrder);
						thePositionOrder.VolumeClosed -= pPositionChange->volumeChange;
						thePositionOrder.VolumeOnClose += pPositionChange->volumeChange;
						m_pMDB->m_PositionOrderFactory->update(pPositionOrder, &thePositionOrder);
					}
				}
			}
		}
	}
	return true;
}

CPositionWedge::CPositionWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
	:CWedge(pTradeService, pMDB, pErrorEngine)
{
	m_sName = "Position";
}

CPositionWedge::~CPositionWedge()
{
}

bool CPositionWedge::changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction)
{
	if (pChangeList->pOrderCancle != NULL)
		return true;

	// OldPosition为空或持仓量为0的前提下
	if (pChangeList->pOldPosition == NULL || pChangeList->pOldPosition->Position.isNull() || pChangeList->pOldPosition->Position == 0)
	{
		//NewPosition持仓数量大于0，则更新BeginTime
		if (pChangeList->pNewPosition->Position > 0)
			pChangeList->pNewPosition->BeginTime = g_nCurrentTime;
	}

	if (pChangeList->pTrade != NULL)
		return true;

	return true;
}
