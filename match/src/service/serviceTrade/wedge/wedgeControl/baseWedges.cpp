#include "baseWedges.h"
#include "CopyField.h"
#include "monitorIndex.h"
#include "tradingTools.h"
#include "CSequence.h"

//bool CFieldCheckWedge::validCombOffsetFlag(CInstrument *pInstrument, CReadOnlyCombOffsetFlagType &combOffsetFlag)
//{
//	const char *pCombOffsetFlag = combOffsetFlag;
//	if (*pCombOffsetFlag == '\0')
//	{
//		return false;
//	}
//	while (*pCombOffsetFlag != '\0')
//	{
//		if (!COffsetFlagType::isValid(*pCombOffsetFlag))
//		{
//			return false;
//		}
//		pCombOffsetFlag++;
//	}
//	return true;
//}

bool CFieldCheckWedge::insertOrder(CWriteableOrder* pOrder, CTransaction* pTransaction)
{
	if (!pOrder->Direction.isValid())
		return m_pErrorEngine->reportError(ERROR_BAD_FIELD, "Direction");
	return true;
	///检查基本字段是否合法
	string sInValid = pOrder->GetInValid();
	if (sInValid != "")
		return m_pErrorEngine->reportError(ERROR_BAD_FIELD, sInValid.c_str());

	/////检查数组字段是否合法
	//if (!validCombOffsetFlag(pOrder->getInstrument(), pOrder->OffsetFlag))
	//{
	//	m_pErrorEngine->reportError(ERROR_BAD_FIELD, "OffsetFlag");
	//	return false;
	//}

	///update by wang.xp 20070316
	///如果不是强平单,不允许填写强平原因为非整数倍强平
// 		char *pCombOffsetFlag=(char *)&pOrder->OffsetFlag;
// 		if  (*pCombOffsetFlag!=OF_ForceClose && (pOrder->ForceCloseReason==FCR_NotMultiple))
// 		{
// 			m_pErrorEngine->reportError(ERROR_BAD_FIELD);
// 			return false;
// 		}
	///update over 20070316

	return true;
}

//bool CFieldCheckWedge::insertCombOrder(CWriteableCombOrder* pCombOrder, CTransaction* pTransaction)
//{
//#define checkField(id)												\
//	if (!pCombOrder->InstrumentID ## id.isNull())					\
//	{																\
//		if (!pCombOrder->Direction ## id.isValid() ||				\
//			!pCombOrder->OffsetFlag ## id.isValid())				\
//			return m_pErrorEngine->reportError(ERROR_BAD_FIELD);		\
//	}
//
//	checkField(1);
//	checkField(2);
//	checkField(3);
//	checkField(4);
//
//	return true;
//}

bool CFieldCheckWedge::orderAction(CWriteableOrderAction* pOrderAction, CTransaction* pTransaction)
{
	return true;
	///检查基本字段是否合法
	string sInValid = pOrderAction->GetInValid();
	if (sInValid != "")
		return m_pErrorEngine->reportError(ERROR_BAD_FIELD, sInValid.c_str());
	return true;
}

bool CFieldCheckWedge::updateInstrumentStatus(CWriteableMarketData* pInstrumentStatus, CTransaction* pTransaction)
{
	///检查基本字段是否合法
	if (pInstrumentStatus->InstrumentStatus.isNull())
		return m_pErrorEngine->reportError(ERROR_BAD_FIELD, "InstrumentStatus");

	return true;
}

bool CActionTargetCheckWedge::insertOrder(CWriteableOrder* pOrder, CTransaction* pTransaction)
{
	if (pOrder->getMember() == NULL)
		pOrder->linkMember();

	CMember* pMember = pOrder->getMember();
	bool isInnerMarketMaker = false;
	if (pMember == NULL)
		return m_pErrorEngine->reportError(ERROR_USER_NOT_ACTIVE, pOrder->MemberID.getValue());
	else
		isInnerMarketMaker = (pMember->MemberType == PT_InnerMarketMaker) ? true : false;

	if (pOrder->MemberID.isNull())
		pOrder->MemberID = pOrder->getMember()->MemberID;

	if (pOrder->AccountID.isNull())
		pOrder->AccountID = pOrder->MemberID;

	if (pOrder->TradeUnitID.isNull())
	{
		pOrder->TradeUnitID = pOrder->AccountID;
	}
	else
	{
		if (pOrder->TradeUnitID == defaultTradeUnitID)
			pOrder->TradeUnitID = pOrder->OrderID;
	}

	//决定持仓方向
	//switch (getPositionType(m_pMDB, pOrder))
	//{
	//case PT_Net:
	//	pOrder->PosiDirection = PD_Net;
	//default:
	//	pOrder->PosiDirection = pOrder->Direction;
	//}

	CPosition* pPosition = getPosition(m_pMDB, pOrder);
	if (pPosition == NULL)
	{
		// 分仓加仓时检查仓位是否存在
		/*if (pOrder->OffsetFlag == OF_Open &&
			pOrder->TradeUnitID != defaultTradeUnitID && pOrder->TradeUnitID != pOrder->AccountID && pOrder->TradeUnitID != getIsolateMergeTradeUnitID(pOrder->AccountID))
		{
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "PositionNotExist");
		}*/
		thePosition.init(m_pMDB);
		thePosition.PositionID = pOrder->OrderID;
		thePosition.CopyMemberID = pOrder->CopyMemberID;
		thePosition.CopyProfitRate = pOrder->CopyProfitRate;
		thePosition.Remark = pOrder->Remark;
		if (isInnerMarketMaker)
		{
			thePosition.CopyMemberID.clear();
			thePosition.CopyProfitRate.clear();
		}
		thePosition.m_pMDB = m_pMDB;
		if (!InitPosition(m_pMDB, m_pErrorEngine, pOrder->getInstrument(), &thePosition, pOrder->MemberID, pOrder->TradeUnitID.getValue(), pOrder->AccountID, pOrder->PosiDirection, pOrder->IsCrossMargin, pOrder->Leverage))
			return false;
		thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		pPosition = m_pMDB->m_PositionFactory->add(&thePosition, pTransaction);
		pOrder->Position = pPosition->Position;
		pOrder->linkPosition(pPosition);
	}
	else
	{
		pOrder->Position = pPosition->Position;
		//如果发现已经有了，并且Positon之前已经结束了，此时需要重新定义PositionID，避免和之前冲突
		//为了补救，比如调整杠杆之后是没有PositionID的，持仓删除之后，需要讲PositionID清零
		m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
		if (pPosition->PositionID.isNull() || (pPosition->Position == 0.0 && pPosition->ShortFrozen == 0.0 && pPosition->LongFrozen == 0.0))
		{
			thePosition.PositionID = pOrder->OrderID;
			thePosition.HighestPosition = 0.0;
		}
		// 仓位不为空的带/跟单仓位上的操作，都必须带回CopyMemberID
		if ((!pPosition->CopyMemberID.isNull()) && (pPosition->CopyMemberID != "0") && (pPosition->Position != 0))
		{
			REPORT_EVENT(LOG_DEBUG, "Keep Position CMID to Order", "MID:%s, CMID:%s, Position:%s",
				pPosition->MemberID.getValue(), pPosition->CopyMemberID.getValue(), pPosition->Position.getString().c_str());
			pOrder->CopyMemberID = pPosition->CopyMemberID;
			if (pPosition->CopyMemberID == pPosition->MemberID)
			{
				pOrder->CopyOrderID = pOrder->OrderID;
			}
		}
		else
		{
			thePosition.CopyMemberID = pOrder->CopyMemberID;
		}
		//如果仓位上已经有分润比例，不允许Order的分润比例来进行修改,除非position变为0
		if ((!pPosition->CopyProfitRate.isNull()) && (pPosition->CopyProfitRate > 0) && (pPosition->Position != 0))
		{
			//pOrder的分润比例，被pPosition中的替代
			pOrder->CopyProfitRate = pPosition->CopyProfitRate;
		}
		else
		{
			thePosition.CopyProfitRate = pOrder->CopyProfitRate;
		}

		if (isInnerMarketMaker)
		{
			thePosition.CopyMemberID.clear();
			thePosition.CopyProfitRate.clear();
		}

		if (pOrder->Remark.isNull())
			pOrder->Remark = thePosition.Remark;
		else
			thePosition.Remark = pOrder->Remark;

		thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		pPosition = m_pMDB->m_PositionFactory->update(pPosition, &thePosition, pTransaction);
	}

	pOrder->PositionID = pPosition->PositionID;

	// 剔除做市，剔除净仓下逐仓和分仓
	//if ((pOrder->getMember()->Grade != "X" && pOrder->getMember()->MarginRateGrade != "X") && pOrder->PosiDirection == PD_Net)
	//{
	//	if (pOrder->TradeUnitID.contains(pOrder->AccountID))
	//	{
	//		if (!pPosition->IsCrossMargin.isNull() && !pPosition->IsCrossMargin)
	//			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitID[%s],IsCrossMargin[%d]", pOrder->TradeUnitID.getValue(), pPosition->IsCrossMargin.getValue());
	//	}
	//	else
	//	{
	//		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitID[%s]", pOrder->TradeUnitID.getValue());
	//	}
	//	/*if (pOrder->TradeUnitID == pOrder->AccountID)
	//	{
	//		if (!pOrder->IsCrossMargin.isNull() && !pOrder->IsCrossMargin)
	//			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitID[%s],IsCrossMargin[%d]", pOrder->TradeUnitID.getValue(), pOrder->IsCrossMargin.getValue());
	//	}
	//	else if (pOrder->TradeUnitID == getIsolateMergeTradeUnitID(pOrder->AccountID))
	//	{
	//		if (!pOrder->IsCrossMargin.isNull() && pOrder->IsCrossMargin)
	//			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitID[%s],IsCrossMargin[%d]", pOrder->TradeUnitID.getValue(), pOrder->IsCrossMargin.getValue());
	//	}*/
	//}
	return true;
}

bool CActionTargetCheckWedge::orderAction(CWriteableOrderAction* pOrderAction, CTransaction* pTransaction)
{
	if (pOrderAction->getMember() == NULL)
		return m_pErrorEngine->reportError(ERROR_USER_NOT_ACTIVE, pOrderAction->MemberID.getValue());

	COrder* pOrder = pOrderAction->getOrder();
	//if (pOrder == NULL)
	//{
	//	if (!pOrderAction->OrderID.isNull())
	//	{
	//		pOrder = pOrderAction->linkOrder(m_pMDB->m_OrderFactory);
	//	}
	//	else
	//	{
	//		pOrder = pOrderAction->linkOrder(m_pMDB->m_OrderFactory->findByLocalID(pOrderAction->UserID, pOrderAction->OrderLocalID));
	//		if (pOrder != NULL)
	//			pOrderAction->OrderID = pOrder->OrderID;
	//	}
	//}

	if (pOrder == NULL)
		return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND);

	//资金变化的AccountAction导致的冻结资金的订单的撤单应该需要AccountAction来撤单
	//但是不能保证这个接口不会被调用，所以这里做预防
	if (pOrder->DeriveSource == DS_FromAccountAction)
		return m_pErrorEngine->reportError(ERROR_UnSupportOrderType);

	if (pOrderAction->MemberID.isNull())
		pOrderAction->MemberID = pOrderAction->getMember()->MemberID;

	return true;
}

bool CInstrumentCheckWedge::insertOrder(CWriteableOrder* pOrder, CTransaction* pTransaction)
{
	CInstrument* pInstrument = pOrder->getInstrument();
	if (pInstrument == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND, "%s:%s", pOrder->ExchangeID.getValue(), pOrder->InstrumentID.getValue());

	//pOrder->ProductGroup = pInstrument->ProductGroup;
	//	pOrder->ProductID = pInstrument->ProductID;
	pOrder->SettlementGroup = pInstrument->SettlementGroup;

	if (pInstrument->ProductClass == PC_AccountExchange)
	{
		if (pOrder->Direction == D_Buy)
		{
			pOrder->ClearCurrency = pInstrument->ClearCurrency;
			pOrder->FeeCurrency = pInstrument->BaseCurrency;
		}
		else
		{
			pOrder->ClearCurrency = pInstrument->BaseCurrency;
			pOrder->FeeCurrency = pInstrument->ClearCurrency;
		}
	}
	else
	{
		if (pInstrument->IsInverse)
			pOrder->ClearCurrency = pInstrument->BaseCurrency;
		else
			pOrder->ClearCurrency = pInstrument->ClearCurrency;
		pOrder->FeeCurrency = pInstrument->ClearCurrency;
	}
	pOrder->PriceCurrency = pInstrument->PriceCurrency;
	//pOrder->FeeCurrency = pInstrument->ClearCurrency;
	//pOrder->ClearCurrency = pOrder->Currency;
	return true;
}
//bool CInstrumentCheckWedge::insertCombOrder(CWriteableCombOrder* pCombOrder, CTransaction* pTransaction)
//{
//	if (!pCombOrder->InstrumentID1.isNull())
//	{
//		if (!pCombOrder->linkInstrument1(m_pMDB->m_InstrumentFactory))
//			return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
//	}
//	else
//	{
//		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
//	}
//
//	if (!pCombOrder->InstrumentID2.isNull())
//	{
//		if (!pCombOrder->linkInstrument2(m_pMDB->m_InstrumentFactory))
//			return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
//
//		if (pCombOrder->InstrumentID1 == pCombOrder->InstrumentID2)
//			return m_pErrorEngine->reportError(ERROR_DuplicateRecord);
//	}
//	else
//	{
//		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
//	}
//
//	if (!pCombOrder->InstrumentID3.isNull())
//	{
//		if (!pCombOrder->linkInstrument3(m_pMDB->m_InstrumentFactory))
//			return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
//		if ((pCombOrder->InstrumentID1 == pCombOrder->InstrumentID3) ||
//			(pCombOrder->InstrumentID2 == pCombOrder->InstrumentID3))
//			return m_pErrorEngine->reportError(ERROR_DuplicateRecord);
//
//		if (!pCombOrder->InstrumentID4.isNull())
//		{
//			if (!pCombOrder->linkInstrument4(m_pMDB->m_InstrumentFactory))
//				return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
//
//			if ((pCombOrder->InstrumentID1 == pCombOrder->InstrumentID4) ||
//				(pCombOrder->InstrumentID2 == pCombOrder->InstrumentID4) ||
//				(pCombOrder->InstrumentID3 == pCombOrder->InstrumentID4))
//				return m_pErrorEngine->reportError(ERROR_DuplicateRecord);
//		}
//	}
//
//	//	CProductClassType productClass;
//
//	//#define checkProductClass(id)														\
//	//	if (pCombOrder->getInstrument ## id())											\
//	//	{																				\
//	//		productClass=pCombOrder->getInstrument ## id()->ProductClass;				\
//	//		if ((productClass!=PC_Futures)&&(productClass!=PC_Options))					\
//	//			return m_pErrorEngine->reportError(ERROR_COMBINATION_ON_SINGLE_INSTRUMENT);	\
//	//	}																				\
//	//	else																			\
//	//	{																				\
//	//		return true;																\
//	//	}
//	//
//	//	checkProductClass(1);
//	//	checkProductClass(2);
//	//	checkProductClass(3);
//	//	checkProductClass(4);
//
//	return true;
//}

bool CInstrumentCheckWedge::updateInstrumentStatus(CWriteableMarketData* pInstrumentStatus, CTransaction* pTransaction)
{
	return true;
}

bool CVolumeCheckWedge::insertOrder(CWriteableOrder* pOrder, CTransaction* pTransaction)
{
	//Price决定的过程中，币币的TurnOver会决定数量，所以需要再决定之后检
	//VolumeDisplay这个比较特殊，不可以设置为0.0
	if (pOrder->VolumeDisplay == 0.0)
		pOrder->VolumeDisplay.clear();

	//if (!pOrder->VolumeDisplay.isNull())
	//{
		//只有做市商X才有资格设置VolumeDisplay
		//if (pOrder->getMember() != NULL && pOrder->getMember()->Grade == "X")
		//{
		//	//这种情况下才允许设置pOrder->VolumeDisplay
		//}
		//else
		//{
		//	pOrder->VolumeDisplay.clear();
		//}

	CMember* pMember = pOrder->getMember();
	if (pMember != NULL)
	{
		if (pMember->MemberType != PT_InnerMarketMaker)
			pOrder->VolumeDisplay.clear();
	}
	//}
	if (!pOrder->VolumeDisplay.isNull())
	{
		if (!pOrder->VolumeDisplay.isTick(pOrder->getInstrument()->VolumeTick.getValue()))
			return m_pErrorEngine->reportError(ERROR_VOLUME_NOT_ON_TICK);
		/*if (pOrder->VolumeDisplay < pOrder->getInstrument()->MinOrderVolume)
			return m_pErrorEngine->reportError(ERROR_LessThanMinVolume);*/
		if (pOrder->VolumeDisplay <= 0)
			return m_pErrorEngine->reportError(ERROR_INVALID_VOLUME, "VolumeDisplayLessThan0");

		//允许大于最大量下ERROR_INVALID_VOLUME
	}

	if (pOrder->OffsetFlag == OF_CloseAll)
	{
		CPosition* pPosition = pOrder->getPosition();
		if (pPosition->Position == 0)
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE, "Position=0");

		if (pPosition->Position > 0)
			pOrder->Volume = pPosition->Position;
		else
			pOrder->Volume = -pPosition->Position;

		CInstrument* pInstrument = pOrder->getInstrument();
		if (!pInstrument->MaxOrderVolume.isNull()) {
			if ((pOrder->Volume) > (pInstrument->MaxOrderVolume)) {
				//提前控制Volume的值
				pOrder->Volume = pInstrument->MaxOrderVolume;
			}
		}
		pOrder->VolumeRemain = pOrder->Volume;
	}
	else if (pOrder->OffsetFlag == OF_CloseMax)
	{
		CPosition* pPosition = pOrder->getPosition();
		if (pPosition->Position == 0)
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE, "Position=0");

		CVolumeType PositionCanCloseVolume = 0.0;
		switch (pPosition->PosiDirection)
		{
		case PD_Long:
			PositionCanCloseVolume = pPosition->Position - pPosition->ShortFrozen;
			break;
		case PD_Short:
			PositionCanCloseVolume = pPosition->Position - pPosition->LongFrozen;
			break;
		case PD_Net:
		{
			if (pPosition->Position > 0)
				PositionCanCloseVolume = pPosition->Position - pPosition->ShortFrozen;
			else
				PositionCanCloseVolume = abs(pPosition->Position) - pPosition->LongFrozen;
		}
		break;
		}

		if (PositionCanCloseVolume <= 0.0)
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE, "Position[%s]LongFrozen[%s]ShortFrozen[%s]",
				pPosition->Position.getString().c_str(), pPosition->LongFrozen.getString().c_str(), pPosition->ShortFrozen.getString().c_str());

		if (pOrder->Volume > PositionCanCloseVolume)
			pOrder->Volume = PositionCanCloseVolume;

		//对于平最大可平量的情况 offFlag = 8, 检查MaxOrderVolume的设置，提前给出错误提示
		CInstrument* pInstrument = pOrder->getInstrument();
		if (!pInstrument->MaxOrderVolume.isNull()) {
			if (pOrder->Volume > pInstrument->MaxOrderVolume.getValue()) {
				return m_pErrorEngine->reportError(ERROR_OutOfMaxVolume, "PositionCanCloseVolume[%s] > MaxOrderVolume[%s]",
					pOrder->Volume.getString().c_str(), pInstrument->MaxOrderVolume.getString().c_str());
			}
		}
		pOrder->VolumeRemain = pOrder->Volume;
	}
	else if (pOrder->OffsetFlag == OF_Close)
	{
		CPosition* pPosition = pOrder->getPosition();
		if (pPosition->Position == 0)
			return m_pErrorEngine->reportError(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE, "Position=0");

		//对于平最大可平量的情况 offFlag = 1， 检查MaxOrderVolume的设置，提前给出错误提示
		CInstrument* pInstrument = pOrder->getInstrument();
		if (!pInstrument->MaxOrderVolume.isNull()) {
			if (pOrder->Volume > pInstrument->MaxOrderVolume.getValue()) {
				return m_pErrorEngine->reportError(ERROR_OutOfMaxVolume, "Volume[%s] > MaxOrderVolume[%s]",
					pOrder->Volume.getString().c_str(), pInstrument->MaxOrderVolume.getString().c_str());
			}
		}
		pOrder->VolumeRemain = pOrder->Volume;
	}

	//不能小于0，是铁律
	if (pOrder->Volume <= 0)
		return m_pErrorEngine->reportError(ERROR_INVALID_VOLUME, "<=0");

	return true;
}

//bool CVolumeCheckWedge::insertCombOrder(CWriteableCombOrder* pCombOrder, CTransaction* pTransaction)
//{
//	CVolumeType volume, totalVolume;
//#define checkVolume(id)																	\
//	if (pCombOrder->getInstrument ## id())												\
//	{																					\
//		volume=pCombOrder->LegMultiple ## id;											\
//		totalVolume=volume*pCombOrder->Volume;											\
//		if((totalVolume < pCombOrder->getInstrument ## id()->MinOrderVolume) ||			\
//			(totalVolume > pCombOrder->getInstrument ## id()->MaxOrderVolume) ||		\
//			(fmod(volume, pCombOrder->getInstrument ## id()->VolumeTick) !=0))			\
//			return m_pErrorEngine->reportError(ERROR_INVALID_VOLUME);							\
//	}																					\
//	else																				\
//	{																					\
//		return true;																	\
//	}
//
//	checkVolume(1);
//	checkVolume(2);
//	checkVolume(3);
//	checkVolume(4);
//	return true;
//}

bool CVolumeCheckWedge::orderAction(CWriteableOrderAction* pOrderAction, CTransaction* pTransaction)
{
	//if (pOrderAction->ActionFlag == AF_Modify)
	//{
	//	return validVolume(pOrderAction->Volume, pOrder);
	//}
	//else if (pOrderAction->ActionFlag == AF_Change)
	//{
	//	//修改报单，不能造成数量不对,只有当新的数量不为0时检查，为0是允许的
	//	CVolumeType newVolume = pOrder->VolumeRemain + pOrderAction->Volume;
	//	return validVolume(newVolume, pOrder);
	//}
	COrder* pOrder = pOrderAction->getOrder();

	//剩余订单为零，说明已经完成了处理，不能重复处理
	if (pOrder->VolumeRemain == 0.0)
		return m_pErrorEngine->reportError(ERROR_INVALID_VOLUME);

	if (pOrderAction->ActionFlag == AF_Modify || pOrderAction->ActionFlag == AF_Change)
	{

		if (pOrder != NULL)
		{
			if (!pOrderAction->VolumeDisplay.isNull())
			{
				CMember* pMember = pOrderAction->getMember();
				if (pMember != NULL && (pMember->MemberType == PT_InnerMarketMaker))
				{
					if (!pOrderAction->VolumeDisplay.isTick(pOrder->getInstrument()->VolumeTick.getValue()))
						return m_pErrorEngine->reportError(ERROR_VOLUME_NOT_ON_TICK);
					if (pOrderAction->VolumeDisplay <= 0)
						return m_pErrorEngine->reportError(ERROR_INVALID_VOLUME, "VolumeDisplayLessThan0");
				}
				else
				{
					pOrderAction->VolumeDisplay.clear();
				}
			}
		}
	}
	return true;
}

//bool CVolumeCheckWedge::validVolume(CVolumeType volume, COrder *pOrder)
//{
//	if (pOrder->OffsetFlag[0] == OF_CloseAll)
//		return true;
//
//	if (!volume.isTick(pOrder->getInstrument()->VolumeTick.getValue()))
//		return m_pErrorEngine->reportError(ERROR_VOLUME_NOT_ON_TICK);
//
//	if (pOrder->DeriveSource == DS_FromOTCTrade_Liquidation)
//		return true;
//
//	if (pOrder->OffsetFlag[0] == OF_ForceClose)
//		return true;
//
//	//确认最大下单量
//	if (!volume.isNull() && volume > pOrder->getInstrument()->MaxOrderVolume)
//		return m_pErrorEngine->reportError(ERROR_OutOfMaxVolume);
//
//	if (volume <= 0.0)
//		return m_pErrorEngine->reportError(ERROR_LessThanMinVolume);
//
//	//最小下单量确保开仓不能超过,在ChangePosition中控制
//	//最小下单量不是每个都精确控制，而是确保剩余的持仓不能小于最小下单量
//	//同时确保如果剩余为0的情况下，不控制最小下单量
//	return true;
//}

bool CVolumeCheckWedge::changePosition(CPositionChangeList* pChangeList, CTransaction* pTransaction)
{
	//成交不检查
	if (pChangeList->pTrade != NULL)
		return true;

	//撤单不检查
	if (pChangeList->pOrderCancle != NULL)
		return true;

	if (pChangeList->pOrder->Volume.isNull())
		return m_pErrorEngine->reportError(ERROR_INVALID_VOLUME);

	if (pChangeList->pOrder->Volume <= 0.0)
		return m_pErrorEngine->reportError(ERROR_LessThanMinVolume);

	//强平和ADL不检查
	switch (pChangeList->pOrder->DeriveSource)
	{
	case DS_FromOTCTrade_Liquidation:
	case DS_FromOTCTrade_ADL:
	case DS_FromOTCTrade_Private:
		return true;
	}

	if (!pChangeList->pOrder->Volume.isTick(pChangeList->pInstrument->VolumeTick.getValue()))
		return m_pErrorEngine->reportError(ERROR_VOLUME_NOT_ON_TICK);

	if (pChangeList->pOrder->OffsetFlag == OF_ForceClose)
		return true;

	//做市不检查订单的数目，要不然无法模拟订单簿
	CMember* pMember = pChangeList->pOrder->getMember();
	if (pMember == NULL)
		return  m_pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);

	if (pMember->MemberType == PT_InnerMarketMaker)
		return true;

	//如果保证金率分组设置为一个数字，标明是做市用户，不检查资金和特殊需求
	if (pMember->TradingRightGrade.getSize() == 1)
		return true;

	//确认最大下单量
	if (!pChangeList->pInstrument->MaxOrderVolume.isNull())
	{
		//Volume会在CVolumeCheckWedge::insertOrder的时候根据OffsetFlag来取值
		//当pInstrument的MaxOrderVolume被设置后，一旦平仓量大于最大委托量，以最大委托量平仓
		if (pChangeList->pOrder->Volume > pChangeList->pInstrument->MaxOrderVolume)
		{
			if (pChangeList->pOrder->OffsetFlag == OF_CloseAll) {
				pChangeList->pOrder->Volume = pChangeList->pInstrument->MaxOrderVolume;
			}
			else {
				return m_pErrorEngine->reportError(ERROR_OutOfMaxVolume);
			}
		}
	}

	//最小下单量确保开仓不能超过,在ChangePosition中控制
	//最小下单量不是每个都精确控制，而是确保剩余的持仓不能小于最小下单量
	//同时确保如果剩余为0的情况下，不控制最小下单量

	if (!pChangeList->pInstrument->MinOrderVolume.isNull())
	{
		switch (pChangeList->pInstrument->ProductClass)
		{
		case PC_Margin:
		case PC_Premium:
		{
			if (pChangeList->isOpen())
			{
				// 只有开仓单才需要控制最小下单额
				if (pChangeList->pOrder->Volume < pChangeList->pInstrument->MinOrderVolume)
					return m_pErrorEngine->reportError(ERROR_LessThanMinVolume);
				//净持仓情况下，只需要计算存开仓的部分
				if (pChangeList->m_LongOpenVolume > 0 && pChangeList->m_LongOpenVolume < pChangeList->pInstrument->MinOrderVolume)
					return m_pErrorEngine->reportError(ERROR_LessThanMinVolume);
				if (pChangeList->m_ShortOpenVolume > 0 && pChangeList->m_ShortOpenVolume < pChangeList->pInstrument->MinOrderVolume)
					return m_pErrorEngine->reportError(ERROR_LessThanMinVolume);
			}
			//else
			//{
			//	//结果为0的全部通过,如果剩余小于MinVolume,不能通过
			//	switch (pChangeList->pNewPosition->PosiDirection)
			//	{
			//	case PD_Long:
			//	{
			//		//结果为0的全部通过
			//		if (pChangeList->pNewPosition->Position == pChangeList->pNewPosition->ShortFrozen)
			//			return true;
			//		if (pChangeList->pNewPosition->Position < pChangeList->pInstrument->MinOrderVolume + pChangeList->pNewPosition->ShortFrozen)
			//			return m_pErrorEngine->reportError(ERROR_PositionLessThanMinVolume);
			//		break;
			//	}
			//	case PD_Short:
			//	{
			//		if (pChangeList->pNewPosition->Position == pChangeList->pNewPosition->LongFrozen)
			//			return true;
			//		if (pChangeList->pNewPosition->Position < pChangeList->pInstrument->MinOrderVolume + pChangeList->pNewPosition->LongFrozen)
			//			return m_pErrorEngine->reportError(ERROR_PositionLessThanMinVolume);
			//		break;
			//	}
			//	case PD_Net:
			//	{
			//		if (pChangeList->pNewPosition->Position > 0)
			//		{
			//			if (pChangeList->pNewPosition->Position == pChangeList->pNewPosition->ShortFrozen)
			//				return true;
			//			if (pChangeList->pNewPosition->Position < pChangeList->pInstrument->MinOrderVolume + pChangeList->pNewPosition->ShortFrozen)
			//				return m_pErrorEngine->reportError(ERROR_PositionLessThanMinVolume,"Left[%s]<MinOrderVolume[%s]+ShortFrozen[%s]",
			//					pChangeList->pNewPosition->Position.getString().c_str(), pChangeList->pInstrument->MinOrderVolume.getString().c_str(), pChangeList->pNewPosition->ShortFrozen.getString().c_str());
			//		}
			//		else
			//		{
			//			if (pChangeList->pNewPosition->LongFrozen == -pChangeList->pNewPosition->Position)
			//				return true;
			//			if (pChangeList->pNewPosition->Position > -pChangeList->pNewPosition->LongFrozen - pChangeList->pInstrument->MinOrderVolume)
			//				return m_pErrorEngine->reportError(ERROR_PositionLessThanMinVolume);
			//		}
			//		break;
			//	}
			//	}
			//}
			break;
		}
		case PC_AccountExchange:
		{
			if (pChangeList->pOrder->Direction == D_Buy)
			{
				if (pChangeList->pOrder->Volume < pChangeList->pInstrument->MinOrderVolume)
					return m_pErrorEngine->reportError(ERROR_LessThanMinVolume);
			}
			else
			{
				if (pChangeList->pNewPosition != NULL)
				{
					if (pChangeList->pNewPosition->Position <= pChangeList->pNewPosition->ShortFrozen)
						return true;
					if (pChangeList->pNewPosition->Position < pChangeList->pInstrument->MinOrderVolume + pChangeList->pNewPosition->ShortFrozen)
						return m_pErrorEngine->reportError(ERROR_PositionLessThanMinVolume);
					if (pChangeList->pOrder->Volume < pChangeList->pInstrument->MinOrderVolume)
						return m_pErrorEngine->reportError(ERROR_LessThanMinVolume);
				}
			}
			break;
		}
		default:
		{
		}
		}
	}

	if (!pChangeList->pInstrument->MinOrderCost.isNull())
	{
		switch (pChangeList->pInstrument->ProductClass)
		{
		case PC_Margin:
		case PC_Premium:
		{
			if (pChangeList->isOpen())
			{
				CMoneyType Cost;
				if (pChangeList->pOrder->OrderPriceType == OPT_ByCost)
					Cost = pChangeList->pOrder->Cost;
				else
				{
					if (pChangeList->pInstrument->IsInverse)
						Cost = pChangeList->pOrder->Volume * pChangeList->pInstrument->VolumeMultiple;
					else
						Cost = pChangeList->pOrder->Volume * pChangeList->pOrder->Price * pChangeList->pInstrument->VolumeMultiple;
				}

				if (Cost < pChangeList->pInstrument->MinOrderCost)
					return m_pErrorEngine->reportError(ERROR_LessThanMinVolume, "Cost[%s]<MinOrderCost[%s]",
						Cost.getString().c_str(), pChangeList->pInstrument->MinOrderCost.getString().c_str());
			}
			break;
		}
		case PC_AccountExchange:
		{
			if (pChangeList->pOrder->Direction == D_Buy)
			{
				CMoneyType Cost;
				if (pChangeList->pOrder->OrderPriceType == OPT_ByCost)
					Cost = pChangeList->pOrder->Cost;
				else
					Cost = pChangeList->pOrder->Volume * pChangeList->pOrder->Price * pChangeList->pInstrument->VolumeMultiple;

				if (Cost < pChangeList->pInstrument->MinOrderCost)
					return m_pErrorEngine->reportError(ERROR_LessThanMinVolume, "Cost[%s]<MinOrderCost[%s]",
						Cost.getString().c_str(), pChangeList->pInstrument->MinOrderCost.getString().c_str());
			}
			break;
		}
		default:
		{
		}
		}
	}

	return true;
}

CTradingRightType CRightCheckWedge::findTradingRight(CReadOnlyShortIndexIDType& AccountTradingRight, CReadOnlyShortIndexIDType& MemberTradingRight,
	CReadOnlyShortIndexIDType& InstrumentTradingRight, CReadOnlyDirectionType Direction)
{
	CTradingRight* pTradingRight = NULL;
	if (InstrumentTradingRight.isNull())
	{
		if (!AccountTradingRight.isNull())
			pTradingRight = m_pMDB->m_TradingRightFactory->findByMemberIDAndInstrument(AccountTradingRight.getValue(), defaultInstrumentID, Direction);
		else if (!MemberTradingRight.isNull())
			pTradingRight = m_pMDB->m_TradingRightFactory->findByMemberIDAndInstrument(MemberTradingRight.getValue(), defaultInstrumentID, Direction);
		else
			pTradingRight = m_pMDB->m_TradingRightFactory->findByMemberIDAndInstrument(defaultMemberID, defaultInstrumentID, Direction);
	}
	else
	{
		if (!AccountTradingRight.isNull())
			pTradingRight = m_pMDB->m_TradingRightFactory->findByMemberIDAndInstrument(AccountTradingRight.getValue(), InstrumentTradingRight.getValue(), Direction);
		else if (!MemberTradingRight.isNull())
			pTradingRight = m_pMDB->m_TradingRightFactory->findByMemberIDAndInstrument(MemberTradingRight.getValue(), InstrumentTradingRight.getValue(), Direction);
		else
			pTradingRight = m_pMDB->m_TradingRightFactory->findByMemberIDAndInstrument(defaultMemberID, InstrumentTradingRight.getValue(), Direction);
	}
	if (pTradingRight != NULL)
		return pTradingRight->TradingRight;
	return TR_Allow;
}

bool CRightCheckWedge::changePosition(CPositionChangeList* pChangeList, CTransaction* pTransaction)
{
	if (pChangeList->pTrade != NULL)
		return true;

	////如果是OTC的报单，也不需要检查权限
	//if ((pChangeList->pOrder != NULL) && (pChangeList->pOrder->DeriveSource == DS_FromOTCTrade_Liquidation))
	//	return true;

	switch (pChangeList->pOrder->DeriveSource)
	{
	case DS_FromOTCTrade_Liquidation:
	case DS_FromOTCTrade_ADL:
	case DS_FromOTCTrade_Private:
		return true;
	}

	//撤单一定是会允许的
	if (pChangeList->pOrderCancle != NULL)
		return true;

	//做市账户不需要检查
	if (pChangeList->pOrder->Priority == 0)
		return true;

	//CFD账户不检查
	if (pChangeList->pOrder->MemberID == g_sCFDTaker)
		return true;

	//风控强平不检查
	//if (pChangeList->pOrder->DeriveSource == DS_FromOTCTrade_Liquidation)
	//	return true;

	CTradingRightType TradingRight;
	CAccount* pAccount = pChangeList->pOrder->getPosition()->getAccount();
	if (pAccount == NULL)
		TradingRight = findTradingRight("", pChangeList->pOrder->getMember()->TradingRightGrade, pChangeList->pInstrument->TradingRightGroup, pChangeList->pOrder->Direction);
	else
		TradingRight = findTradingRight(pAccount->TradingRightGrade, pChangeList->pOrder->getMember()->TradingRightGrade, pChangeList->pInstrument->TradingRightGroup, pChangeList->pOrder->Direction);

	switch (TradingRight)
	{
	case TR_Forbidden:
		return m_pErrorEngine->reportError(ERROR_TRADE_FORBIDDEN);
	case TR_CloseOnly:
	{
		if (pChangeList->isOpen())
			return m_pErrorEngine->reportError(ERROR_CLOSE_ONLY);
	}
	case TR_Allow:
	default:
		return true;
	}
	return true;
}

bool CPersonOpenCheckWedge::changePosition(CPositionChangeList* pChangeList, CTransaction* pTransaction)
{
	return true;
	if (!m_backendControlProperty)
	{
		// 		///不是由后台控制合约属性，那就要按照交割前几月进行判断
		// 		int allowCount=pInstrumentProperty->AllowDelivPersonOpen;
		// 		if (allowCount<=0)
		// 		{
		// 			///没有限制
		// 			return true;
		// 		}
		// 	
		// 		int year,month,day;
		// 		getTradingDay(m_pMDB,&year,&month,&day);
		// 		int curMonth=year*12+month-1;
		// 		int delMonth=pChangeList->pInstrument->DeliveryYear*12+pChangeList->pInstrument->DeliveryMonth-1;
		// 	
		// 		if (delMonth-curMonth>allowCount-1)
		// 		{
		// 			///还没到时间
		// 			return true;
		// 		}
	}
	else
	{
		// 		///由后台控制，那就直接判断是否允许
		// 		if (pInstrumentProperty->AllowDelivPersonOpen)
		// 		{
		// 			return true;
		// 		}
	}

	///看来目前不允许自然人开仓，那是否是这种情况呢

	//if (pChangeList->pTradeUnit->TradeUnitType != CT_Person)
	//{
	//	///不是自然人
	//	return true;
	//}

	if (!pChangeList->isOpen())
	{
		///没有开仓
		return true;
	}

	//m_pErrorEngine->reportError(ERROR_NOT_ALLOW_PERSON_OPEN);
	return false;
}

bool CUserRightWedge::insertOrder(CWriteableOrder* pOrder, CTransaction* pTransaction)
{
	//if (pOrder->MemberID != pOrder->getMember()->MemberID)
	//{
	//	if (pOrder->MemberID == g_sCFDTaker)
	//		return true;
	//	return m_pErrorEngine->reportError(ERROR_CanNotActionForOtherUser, "%s:Action:%s", pOrder->MemberID.getValue(), pOrder->getUser()->DefaultMemberID.getValue());
	//}
	return true;
}

bool CUserRightWedge::orderAction(CWriteableOrderAction* pOrderAction, CTransaction* pTransaction)
{
	/////如果报单操作用户就是原报单插入用户，那就可以通过
	//if (pOrderAction->UserID != pOrderAction->getOrder()->UserID)
	//{
	//	if (!(pOrderAction->getUser()->UserType == UT_SuperManager))
	//		return m_pErrorEngine->reportError(ERROR_CanNotActionForOtherUser, "%s:Action:%s",
	//			pOrderAction->UserID.getValue(), pOrderAction->getOrder()->UserID.getValue());
	//}

	if (pOrderAction->MemberID != pOrderAction->getOrder()->MemberID)
		return m_pErrorEngine->reportError(ERROR_CanNotActionForOtherUser, "%s:ActionOrder:%s",
			pOrderAction->MemberID.getValue(), pOrderAction->getOrder()->MemberID.getValue());

	//if (pOrderAction->MemberID != pOrderAction->getUser()->DefaultMemberID)
	//{
	//	if (pOrderAction->getUser()->UserType != UT_SuperManager)
	//		return m_pErrorEngine->reportError(ERROR_CanNotActionForOtherUser, "%s:Action:%s",
	//			pOrderAction->MemberID.getValue(), pOrderAction->getUser()->DefaultMemberID.getValue());
	//}

	return true;
}

//////---------Instrument表触发Currency表
CTableCurrencyCurrencyTrigger::CTableCurrencyCurrencyTrigger(CMDB* pMDB)
{
	m_pMDB = pMDB;
}

void CTableCurrencyCurrencyTrigger::commitAdd(CInstrument* pInstrument)
{
	if (pInstrument->ProductClass == PC_AccountExchange)
	{
		CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pInstrument->BaseCurrency, pInstrument->SettlementGroup);
		UpdateCurrency(pInstrument, pCurrency, pInstrument->BaseCurrency);
	}

	CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pInstrument->ClearCurrency, pInstrument->SettlementGroup);
	pCurrency = UpdateCurrency(pInstrument, pCurrency, pInstrument->ClearCurrency);
	pInstrument->linkClearCurrency(pCurrency);
}

void CTableCurrencyCurrencyTrigger::commitUpdate(CInstrument* pInstrument, CWriteableInstrument* pOldInstrument)
{
	if (pInstrument->ProductClass == PC_AccountExchange)
	{
		CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pInstrument->BaseCurrency, pInstrument->SettlementGroup);
		UpdateCurrency(pInstrument, pCurrency, pInstrument->BaseCurrency);
		pInstrument->linkBaseCurrency(pCurrency);
	}
	CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pInstrument->ClearCurrency, pInstrument->SettlementGroup);
	pCurrency = UpdateCurrency(pInstrument, pCurrency, pInstrument->ClearCurrency);
	pInstrument->linkClearCurrency(pCurrency);
}

void CTableCurrencyCurrencyTrigger::commitRemove(CWriteableInstrument* pInstrument)
{
}

CCurrency* CTableCurrencyCurrencyTrigger::UpdateCurrency(CInstrument* pInstrument, CCurrency* pCurrency, CCurrencyType Currency)
{
	if (pCurrency == NULL)
	{
		CWriteableCurrency theCurrency;
		theCurrency.init();
		theCurrency.SettlementGroup = pInstrument->SettlementGroup;
		theCurrency.Currency = Currency;
		theCurrency.SortNumber = pInstrument->SortNumber;
		//从相同的SettlmentGroup里面找到SettleSegment
		CCurrency* pNewCurrency = m_pMDB->m_CurrencyFactory->findFirstBySettlementGroup(pInstrument->SettlementGroup);
		while (pNewCurrency != NULL)
		{
			if (!pNewCurrency->SettleSegment.isNull())
			{
				theCurrency.SettleSegment = pNewCurrency->SettleSegment;
				break;
			}
			pNewCurrency = m_pMDB->m_CurrencyFactory->findNextBySettlementGroup();
		}
		if (pInstrument->ProductClass == PC_AccountExchange && pInstrument->BaseCurrency == theCurrency.Currency)
		{
			if (pInstrument->ClearCurrency == g_sBaseClearCurrency.c_str())
			{
				theCurrency.InstrumentID = pInstrument->InstrumentID;
				theCurrency.ExchangeID = pInstrument->ExchangeID;
				theCurrency.linkMarketData(pInstrument->getMarketData());
			}
			else
			{
				CCurrency* pNextCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pInstrument->ClearCurrency, pInstrument->SettlementGroup);
				if (pNextCurrency != NULL)
				{
					theCurrency.InstrumentID = pInstrument->InstrumentID;
					theCurrency.ExchangeID = pInstrument->ExchangeID;
					theCurrency.linkMarketData(pInstrument->getMarketData());
				}
			}
		}
		if (theCurrency.Currency == g_sBaseClearCurrency.c_str())
		{
			theCurrency.InstrumentID.clear();
			theCurrency.ExchangeID.clear();
			theCurrency.linkMarketData((CWriteableMarketData*)NULL);
		}
		m_pMDB->m_CurrencyFactory->add(&theCurrency);
	}
	else
	{
		CWriteableCurrency theCurrency;
		m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
		//		theCurrency.ProductGroup = pInstrument->ProductGroup;
		theCurrency.SortNumber = pInstrument->SortNumber;
		if (pInstrument->ProductClass == PC_AccountExchange && pInstrument->ClearCurrency == g_sBaseClearCurrency.c_str() && pInstrument->BaseCurrency == theCurrency.Currency)
		{
			if (pInstrument->ClearCurrency == g_sBaseClearCurrency.c_str())
			{
				theCurrency.InstrumentID = pInstrument->InstrumentID;
				theCurrency.ExchangeID = pInstrument->ExchangeID;
				theCurrency.linkMarketData(pInstrument->getMarketData());
			}
			else
			{
				CCurrency* pNextCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pInstrument->ClearCurrency, pInstrument->SettlementGroup);
				if (pNextCurrency != NULL)
				{
					theCurrency.InstrumentID = pInstrument->InstrumentID;
					theCurrency.ExchangeID = pInstrument->ExchangeID;
					theCurrency.linkMarketData(pInstrument->getMarketData());
				}
			}
		}
		if (theCurrency.Currency == g_sBaseClearCurrency.c_str())
		{
			theCurrency.InstrumentID.clear();
			theCurrency.ExchangeID.clear();
			theCurrency.linkMarketData((CWriteableMarketData*)NULL);
		}
		m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
	}
	return pCurrency;
}

//////---------UserSession表触发User表
//CTableUserAPIKeyTrigger::CTableUserAPIKeyTrigger(CMDB *pMDB)
//{
//	m_pMDB = pMDB;
//}
//
//void CTableUserAPIKeyTrigger::commitAdd(CUserSession * pUserSession)
//{
//	如果是申请APIKey，申请数量+1
//	if (pUserSession->APPID.isNull())
//	{
//		if (!pUserSession->Token.isNull() && !pUserSession->AuthCode.isNull())
//		{
//			CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(pUserSession->MemberID);
//			if (pMember != NULL)
//			{
//				CWriteableMember theMember;
//				m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
//				theMember.ApiKeys += 1;
//				theMember.SinkType = ST_SinkToDB;
//				m_pMDB->m_MemberFactory->update(pMember, &theMember);
//			}
//		}
//	}
//}
//
//void CTableUserAPIKeyTrigger::commitRemove(CWriteableUserSession * pUserSession)
//{
//	如果是删除APIKey，申请数量-1
//	if (pUserSession->APPID.isNull())
//	{
//		if (!pUserSession->Token.isNull() && !pUserSession->AuthCode.isNull())
//		{
//			CUser *pUser = m_pMDB->m_UserFactory->findByUserID(pUserSession->UserID);
//			if (pUser != NULL)
//			{
//				CWriteableUser theUser;
//				m_pMDB->m_UserFactory->retrieve(pUser, &theUser);
//				theUser.ApiKeyCount -= 1;
//				theUser.SinkType = ST_SinkToDB;
//				m_pMDB->m_UserFactory->update(pUser, &theUser);
//			}
//		}
//	}
//}

//////---------UserSession表触发发送
CUserSessionSendTrigger::CUserSessionSendTrigger(CServiceResponser* pServiceResponser)
{
	m_pServiceResponser = pServiceResponser;
}

void CUserSessionSendTrigger::commitAdd(CUserSession* pUserSession)
{
	m_pServiceResponser->GetMetaRef()->SessionNo = pUserSession->SessionNo;
	m_pServiceResponser->GetMetaRef()->FrontNo = pUserSession->FrontNo;
	m_pServiceResponser->AddApiKey(pUserSession->Token.getValue(), pUserSession->AuthCode.getValue(), pUserSession->UserID.getValue(),
		pUserSession->ExpireTime.getValue(), pUserSession->AccessLimit.getValue());
}

void CUserSessionSendTrigger::commitUpdate(CUserSession* pUserSession, CWriteableUserSession* pOldUserSession)
{
	if (pUserSession->SinkType == ST_NoSink)
		return;
	m_pServiceResponser->AddApiKey(pUserSession->Token.getValue(), pUserSession->AuthCode.getValue(), pUserSession->UserID.getValue(),
		pUserSession->ExpireTime.getValue(), pUserSession->AccessLimit.getValue());
}

void CUserSessionSendTrigger::commitRemove(CWriteableUserSession* pUserSession)
{
	//删除两个，要不然导致原有的长连接无法断开
	m_pServiceResponser->DelSession(pUserSession->FrontNo.getValue(), pUserSession->SessionNo.getValue());
	m_pServiceResponser->DelApiKey(pUserSession->Token.getValue());
}
