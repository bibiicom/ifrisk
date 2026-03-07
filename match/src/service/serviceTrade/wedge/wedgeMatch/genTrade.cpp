#include "genTrade.h"
#include "monitorIndex.h"
#include "CopyField.h"

CSingleTradeMaker::CSingleTradeMaker(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
{
	m_TradeService = pTradeService;
	m_pMDB = pMDB;
	m_pErrorEngine = pErrorEngine;
}

void CSingleTradeMaker::genSingleTrade(CInstrument *pInstrument, CWriteableOrder *pOrder, CReadOnlyDirectionType &direction,
	CReadOnlyPriceType &price, CReadOnlyVolumeType &volume, CIndexIDType *pTradeID, CReadOnlyDeriveSourceType &DeriveSource, char matchRole, bool bSelfTrade)
{
	CWriteableTrade theTrade;

	theTrade.init();
	theTrade.ExchangeID = pInstrument->ExchangeID;
	theTrade.InstrumentID = pInstrument->InstrumentID;
	//theTrade.ProductGroup = pInstrument->ProductGroup;
//	theTrade.SettlementGroup = pInstrument->SettlementGroup;
	//	theTrade.ProductID = pInstrument->ProductID;
	theTrade.Direction = direction;
	theTrade.Price = price;
	theTrade.Volume = volume;
	theTrade.DeriveSource = DeriveSource;
	//	theTrade.PriceSource = priceSource;
	theTrade.MatchRole = matchRole;
	theTrade.TriggerPrice = pOrder->TriggerPrice;

	theTrade.OrderID = pOrder->OrderID;
	theTrade.MemberID = pOrder->MemberID;
	theTrade.CopyOrderID = pOrder->CopyOrderID;
	theTrade.CopyMemberID = pOrder->CopyMemberID;	
//	theTrade.TradeUnitID = pOrder->TradeUnitID;
	theTrade.OffsetFlag = pOrder->OffsetFlag;
	theTrade.OrderPrice = pOrder->Price;
	theTrade.IsSelfTrade = bSelfTrade;
	theTrade.Remark = pOrder->Remark;
	theTrade.Leverage = pOrder->Leverage;
//	theTrade.Currency = pOrder->Currency;
	theTrade.FeeCurrency = pOrder->FeeCurrency;
	theTrade.ClearCurrency = pOrder->ClearCurrency;
	theTrade.PriceCurrency = pOrder->PriceCurrency;
	theTrade.APPID = pOrder->APPID;

	if (!pTradeID->isNull())
	{
		theTrade.TradeID = *pTradeID;
	}

	///设置插入成交单的回滚保存点
	///这种实现主要是为了处理插入成交失败的情况。照道理来说，插入成交应当是不会失败的，
	///目前的规则保证了这一点。但是，如果考虑使用目前的架构，支持类似于撮合轮的实现方式，
	///就必须考虑插入成交失败的可能。因此，我们还是在这里放置了一个回滚保存点，尽管它
	///目前不会被使用到。
	//CTransactionSavePoint *pBeforeInsertTrade=CTransactionSavePoint::alloc(pTransaction);
	if (!m_TradeService->insertSingleTrade(&theTrade, pOrder))
	{
		/*
		internalWarning(m_pErrorEngine,"产生成交失败");
		pTransaction->rollbackToSavePoint(pBeforeInsertTrade);
		*/
		REPORT_EVENT(LOG_CRITICAL, "genSingleTrade", "Error:%s:%s:Failed While Generate Trade", pOrder->MemberID.getValue(), pOrder->OrderID.getValue());
		return;
	}
	*pTradeID = theTrade.TradeID;
}

//bool CSingleTradeMaker::genSingleTrade(CInstrument *pInstrument, CWriteableCombOrder *pCombOrder, CReadOnlyDirectionType &direction, CReadOnlyPriceType &price, CReadOnlyPriceSourceType &priceSource, CReadOnlyVolumeType &volume,
//	CReadOnlyOffsetFlagType &offsetFlag,CTransaction *pTransaction, CIndexIDType *pTradeID, CReadOnlyDeriveSourceType &DeriveSource, char matchRole, bool bSelfTrade)
//{
//	CWriteableTrade theTrade;
//
//	theTrade.init();
//	theTrade.ExchangeID = pInstrument->ExchangeID;
//	theTrade.InstrumentID = pInstrument->InstrumentID;
//	theTrade.ProductGroup = pInstrument->ProductGroup;
////	theTrade.ProductID = pInstrument->ProductID;
//	theTrade.Direction = direction;
//	theTrade.Price = price;
//	theTrade.Volume = volume;
//	theTrade.DeriveSource = DeriveSource;
////	theTrade.PriceSource = priceSource;
//
//	theTrade.OrderID = pCombOrder->OrderID;
//	theTrade.MemberID = pCombOrder->MemberID;
//	theTrade.TradeUnitID = pCombOrder->TradeUnitID;
//	theTrade.OffsetFlag = offsetFlag;
//	theTrade.MatchRole = matchRole;
////	theTrade.UserID = pCombOrder->UserID;
////	theTrade.OrderLocalID = pCombOrder->LocalID;
//	theTrade.Remark = pCombOrder->Remark;
//	theTrade.IsSelfTrade = bSelfTrade;
//	theTrade.FeeCurrency = pInstrument->FeeCurrency;
//
//	if (!pTradeID->isNull())
//	{
//		theTrade.TradeID = *pTradeID;
//	}
//
//	if (!m_TradeService->insertSingleTrade(&theTrade, NULL,pTransaction))
//	{
//		return false;
//	}
//	*pTradeID = theTrade.TradeID;
//	return true;
//}

void CSingleTradeMaker::genTrade(CWriteableOrder *pBuyOrder, CWriteableOrder *pSellOrder, CReadOnlyPriceType &price, CReadOnlyVolumeType &volume, char BuyOrderMatchRole)
{
	CIndexIDType tradeID = "";

	char SellOrderMatchRole = MR_None;
	bool bSelfTrade = false;
	if (pBuyOrder->MemberID == pSellOrder->MemberID && pBuyOrder->TradeUnitID == pSellOrder->TradeUnitID)
	{
		bSelfTrade = true;
	}

	if (MR_Maker == BuyOrderMatchRole)
	{
		SellOrderMatchRole = MR_Taker;
	}
	else if (MR_Taker == BuyOrderMatchRole)
	{
		SellOrderMatchRole = MR_Maker;
	}

	genSingleTrade(pBuyOrder->getInstrument(), pBuyOrder, D_Buy, price, volume, &tradeID, DS_Normal, BuyOrderMatchRole, bSelfTrade);
	genSingleTrade(pSellOrder->getInstrument(), pSellOrder, D_Sell, price, volume, &tradeID, DS_Normal, SellOrderMatchRole, bSelfTrade);

	/*
	CInstrument *pInstrument=pBuyOrder->getInstrument();

	CTransactionSavePoint *pBeforeInsertTrade;

	///准备成交中的公共值
	CWriteableTrade theTrade;
	theTrade.init();
	theTrade.InstrumentID=pInstrument->InstrumentID;
	theTrade.Price=price;
	theTrade.Volume=volume;
	theTrade.TradeType=TT_Common;
	theTrade.PriceSource=priceSource;
	theTrade.linkInstrument(pInstrument);

	///准备成交，买单
	theTrade.Direction=D_Buy;
	theTrade.OrderID=pBuyOrder->OrderID;
	theTrade.MemberID=pBuyOrder->MemberID;
	theTrade.TradeUnitID=pBuyOrder->TradeUnitID;
	theTrade.OffsetFlag=pBuyOrder->OffsetFlag[0];
	theTrade.PositionFlag=pBuyOrder->CombPositionFlag[0];

	///设置插入成交单的回滚保存点
	///这种实现主要是为了处理插入成交失败的情况。照道理来说，插入成交应当是不会失败的，
	///目前的规则保证了这一点。但是，如果考虑使用目前的架构，支持类似于撮合轮的实现方式，
	///就必须考虑插入成交失败的可能。因此，我们还是在这里放置了一个回滚保存点，尽管它
	///目前不会被使用到。
	pBeforeInsertTrade=CTransactionSavePoint::alloc(pTransaction);
	if (!m_TradeService->insertSingleTrade(&theTrade,pBuyOrder,pSession, pTransaction))
	{
		pTransaction->rollbackToSavePoint(pBeforeInsertTrade);
	}

	///准备成交，卖单
	theTrade.Direction=D_Sell;
	theTrade.OrderID=pSellOrder->OrderID;
	theTrade.MemberID=pSellOrder->MemberID;
	theTrade.TradeUnitID=pSellOrder->TradeUnitID;
	theTrade.OffsetFlag=pSellOrder->OffsetFlag[0];
	theTrade.PositionFlag=pSellOrder->CombPositionFlag[0];

	pBeforeInsertTrade=CTransactionSavePoint::alloc(pTransaction);
	if (!m_TradeService->insertSingleTrade(&theTrade,pSellOrder,pSession, pTransaction))
	{
		pTransaction->rollbackToSavePoint(pBeforeInsertTrade);
	}
	*/
}

//bool CSingleTradeMaker::genTrade(CWriteableOrder *pOrder, CWriteableCombOrder *pCombOrder, CReadOnlyVolumeType volume, CReadOnlyOffsetFlagType &offsetFlag, char BuyOrderMatchRole, CTransaction *pTransaction)
//{
//	CIndexIDType tradeID = "";
//	//CPriceSourceType priceSource;
//	//if (pOrder->Direction == D_Buy)
//	//{
//	//	priceSource = PS_Buy;
//	//}
//	//else
//	//{
//	//	priceSource = PS_Sell;
//	//}
//
//	bool bSelfTrade = false;
//	if (pOrder->MemberID == pCombOrder->MemberID && pOrder->TradeUnitID == pCombOrder->TradeUnitID)
//	{
//		bSelfTrade = true;
//	}
//
//	char SellOrderMatchRole = MR_None;
//	if (MR_Maker == BuyOrderMatchRole)
//	{
//		SellOrderMatchRole = MR_Taker;
//	}
//	else if (MR_Taker == BuyOrderMatchRole)
//	{
//		SellOrderMatchRole = MR_Maker;
//	}
//
//	if (!genSingleTrade(pOrder->getInstrument(), pCombOrder, getReverseDirection(pOrder->Direction), pOrder->Price, volume, offsetFlag, pTransaction, &tradeID, DS_Normal, BuyOrderMatchRole, bSelfTrade))
//		return false;
//
//	genSingleTrade(pOrder->getInstrument(), pOrder, pOrder->Direction, pOrder->Price, volume, pTransaction, &tradeID, DS_Normal, SellOrderMatchRole, bSelfTrade);
//	return true;
//}

