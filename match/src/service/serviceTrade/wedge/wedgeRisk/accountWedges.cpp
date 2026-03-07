#include "accountWedges.h"
#include "monitorIndex.h"
#include "CopyField.h"

void CAccountProductGroupTrigger::beforeAdd(CWriteableAccount* pAccount)
{
	//CCurrency *pCurrency = m_pMDB->m_CurrencyFactory->findByPK(pAccount->Currency, pAccount->SettlementGroup);
	//if (pCurrency != NULL)
	//	pAccount->ProductGroup = pCurrency->ProductGroup;
}

/////////////////////////////////////////////////////////////////////////
//CTableCurrencyAccumulationTrigger是Account表发生资金变化时更新Currency表数据的触发器
/////////////////////////////////////////////////////////////////////////
//CTableCurrencyAccumulationTrigger::CTableCurrencyAccumulationTrigger(CMDB *pMDB)
//{
//	m_pMDB = pMDB;
//}
//
//void CTableCurrencyAccumulationTrigger::commitAdd(CAccount *pAccount)
//{
//	// 汇总资金数据到Currency表
//	CCurrency *pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pAccount->Currency, pAccount->SettlementGroup);
//	if (pCurrency != NULL)
//	{
//		CWriteableCurrency theCurrency;
//		m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
//		theCurrency.Withdraw += pAccount->Withdraw;
//		theCurrency.Deposit += pAccount->Deposit;
//		theCurrency.Balance += pAccount->Balance;
//		theCurrency.PreBalance += pAccount->PreBalance;
//		m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
//	}
//}
//
//void CTableCurrencyAccumulationTrigger::commitUpdate(CAccount *pAccount, CWriteableAccount *pOldAccount)
//{
//	//当出入金发生变化的时候，才更新
//	if (pOldAccount->Withdraw != pAccount->Withdraw || pOldAccount->Deposit != pAccount->Deposit)
//	{
//		CCurrency *pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pAccount->Currency, pAccount->SettlementGroup);
//		if (pCurrency != NULL)
//		{
//			CWriteableCurrency theCurrency;
//			m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
//			theCurrency.Withdraw += (pAccount->Withdraw - pOldAccount->Withdraw);
//			theCurrency.Deposit += (pAccount->Deposit - pOldAccount->Deposit);
//			//theCurrency.Balance += (pAccount->Balance - pOldAccount->Balance);
//			//theCurrency.PreBalance += (pAccount->PreBalance - pOldAccount->PreBalance);
//			theCurrency.Balance = pAccount->PreBalance + theCurrency.Deposit - theCurrency.Withdraw;
//			m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
//		}
//	}
//}
//
//void CTableCurrencyAccumulationTrigger::commitRemove(CWriteableAccount *pAccount)
//{
//	CCurrency *pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pAccount->Currency, pAccount->SettlementGroup);
//	if (pCurrency != NULL)
//	{
//		CWriteableCurrency theCurrency;
//		m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
//		theCurrency.Withdraw -= pAccount->Withdraw;
//		theCurrency.Deposit -= pAccount->Deposit;
//		theCurrency.Balance -= pAccount->Balance;
//		theCurrency.PreBalance -= pAccount->PreBalance;
//		m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
//	}
//}

CDepositCheckWedge::CDepositCheckWedge(CTradeService* pTradeService, CMDB* pMDB, CErrorEngine* pErrorEngine)
	:CWedge(pTradeService, pMDB, pErrorEngine)
{
	m_pExchangeAlgo = new CExchangeDepositAlgorithms(pMDB);
	m_pBaseAlgo = new CBaseDepositAlgorithms(pMDB);
	m_sName = "DepositCheck";
}

CDepositCheckWedge::~CDepositCheckWedge(void)
{
	delete m_pExchangeAlgo;
	delete m_pBaseAlgo;
}


bool CDepositCheckWedge::needCheckDeposit(CPositionChangeList* pChangeList)
{
	///是否资金不足，检查的规则包括：
	///	期货的开仓报单要检查
	///	期权的报单要检查
	///	OTC报单产生成交要检查
// 	if ((pChangeList->pTrade==NULL)&&(pChangeList->pInstrument->ProductClass==PC_Futures)&&pChangeList->isOpen())
// 	{
// 		return true;
// 	}
// 	//期权买平仓不再检查资金（平仓均不检查资金）
// 	else if ((pChangeList->pTrade==NULL)&&(pChangeList->pInstrument->ProductClass==PC_Options)&&pChangeList->isOpen())
// 	{
// 		return true;
// 	}
// 	//tom OTC报单不检查资金
// 	else if ((pChangeList->pOrder!=NULL))
// 	{
// 		if(pChangeList->pOrder->OrderType==ORDT_DeriveFromOTCTrade)
// 		{
// 			return true;
// 		}
// 	}
// 	return false;

	//撤单不检查资金
	if (pChangeList->pOrderCancle != NULL)
		return false;

	//这里是其他报单的情况
// 	if (pChangeList->pTrade==NULL)
// 	{
// 		//开仓肯定检查资金
// 		if(pChangeList->isOpen())
// 		{
// 			return true;
// 		}
// 	}

	//成交不检查资金，不回退
	if (pChangeList->pTrade != NULL)
		return false;

	if (pChangeList->pOrder->Priority == 0)
		return false;

	//接管报单全部不检查资金，为了方便接管
	switch (pChangeList->pOrder->DeriveSource)
	{
	case DS_FromOTCTrade_Liquidation:
	case DS_FromOTCTrade_ADL:
	case DS_FromOTCTrade_Private:
		return false;
	}

	//开仓、平仓都需要检查资金
	//if (pChangeList->isOpen())
	//{
	//	return true;
	//}

	//if (!pChangeList->isOpen())
	//{
	//	return false;
	//}

	//强平不需要检查资金
	if (pChangeList->pOrder->OffsetFlag == OF_ForceClose)
		return false;

	//平仓的检查资金是需要计算浮动盈亏，使用浮动盈亏计算出最差平仓价

	//市价单成交需要检查资金，但是这里冻结的情况全部考虑了，不会出现资金不足的情况，所以不检查
	return true;
}

bool CDepositCheckWedge::needCheckDeposit(COrder* pOrder)
{
	//内部做市不检查资金
	if (pOrder->getMember()->MemberType == PT_InnerMarketMaker)
		return false;
	//CFD接管账户不需要检查资金
	if (pOrder->MemberID == g_sCFDTaker)
		return false;
	return true;
}

//bool CDepositCheckWedge::deducePosition(CPositionChangeList *pChangeList, CDepositChange *pDepositChange, CAccount *pAccount, CMoneyType DeduceFee)
//{
//	CInstrumentIDType InstrumentID = "";
//	sprintf(InstrumentID.getBuffer(), "%s/%s", pAccount->Currency.getValue(), pChangeList->pTrade->PriceCurrency.getValue());
//	CPosition* pPosition = m_pMDB->m_PositionFactory->findByPositionID(pChangeList->pOrder->PositionID);
//	if (pPosition == NULL || DeduceFee > pPosition->Position - pPosition->ShortFrozen)
//		return false;
//
//	CVolumeType VolumeTick = DEFAULT_MOENEY_TICK;
//	if (!pPosition->getInstrument()->VolumeTick.isNull())
//		VolumeTick = pPosition->getInstrument()->VolumeTick;
//
//	m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
//	thePosition.Position -= DeduceFee;
//	if (thePosition.Position < VolumeTick && thePosition.Position > -VolumeTick)
//		thePosition.Position = 0.0;
//	if (thePosition.Position == 0.0)
//	{
//		thePosition.TotalPositionCost = 0.0;
//		thePosition.OpenPrice.clear();
//		thePosition.TotalCloseProfit = 0.0;
//		thePosition.PositionCost = 0.0;
//		thePosition.CostPrice.clear();
//		thePosition.CloseProfit = 0.0;
//		thePosition.UseMargin = 0.0;
//	}
//	else
//	{
//		thePosition.TotalPositionCost -= DeduceFee * pPosition->getInstrument()->VolumeMultiple * pPosition->OpenPrice;
//	}
//
//	thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
//	m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
//	return true;
//}

bool CDepositCheckWedge::changeDepositFeeEach(CPositionChangeList* pChangeList, CAccount* pAccount, CMoneyType RemainFee, CRatioType FeeDeduceDisCount)
{
	CCurrency* pCurrency = pAccount->getCurrency();
	if (pCurrency->getMarketData() == NULL || pCurrency->getMarketData()->LastPrice.isNull())
		return false;

	CMoneyType deduceFee = FeeDeduceDisCount.getValue() * RemainFee.getValue() / pCurrency->getMarketData()->LastPrice.getValue();

	if (pAccount->Available < deduceFee)
		return false;

	// 扣除账户手续费
	m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
	theAccount.Fee += deduceFee;
	theAccount.Fee.toUpperTick(pCurrency->MoneyTick.getValue());
	theAccount.TotalFee += deduceFee;
	applyAccountChange(&theAccount);

	// 增加资金流水
	CWriteableAccountDetail theAccountDetail;
	theAccountDetail.PreBalance = pAccount->Balance;
	theAccountDetail.AccountID = theAccount.AccountID;
	theAccountDetail.MemberID = theAccount.MemberID;
	//		theAccountDetail.TradeUnitID = pChangeList->pTrade->TradeUnitID;
	theAccountDetail.Currency = theAccount.Currency;
	theAccountDetail.SettlementGroup = theAccount.SettlementGroup;
	theAccountDetail.ExchangeID = pChangeList->pTrade->ExchangeID;
	theAccountDetail.InstrumentID = pChangeList->pTrade->InstrumentID;
	//theAccountDetail.ProductGroup = pChangeList->pTrade->ProductGroup;
	theAccountDetail.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	theAccountDetail.LocalID = pChangeList->pOrder->LocalID;
	theAccountDetail.ReserveBalance = theAccount.Reserve;
	theAccountDetail.Amount = -deduceFee;
	theAccountDetail.Balance = theAccount.Balance;
	theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
	theAccountDetail.Source = ACT_Fee;
	theAccountDetail.RelatedID = pChangeList->pTrade->TradeID;
	theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
	m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);

	theAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_AccountFactory->update(pAccount, &theAccount);

	if (pChangeList->pTrade->Remark.isNull())
		sprintf(pChangeList->pTrade->Remark.getBuffer(), "%s:%s:%s", pAccount->Currency.getString().c_str(), deduceFee.getString().c_str(), pCurrency->getMarketData()->LastPrice.getString().c_str());
	else
		sprintf(pChangeList->pTrade->Remark.getBuffer(), "%s;%s:%s:%s", pChangeList->pTrade->Remark.getString().c_str(),
			pAccount->Currency.getString().c_str(), deduceFee.getString().c_str(), pCurrency->getMarketData()->LastPrice.getString().c_str());
	return true;
}

CMoneyType CDepositCheckWedge::changeDepositFee(CPositionChangeList* pChangeList, CDepositChange* pDepositChange, CMoneyType TotalTradeFee)
{
	if (pChangeList->pNewPosition->getAccount() == NULL)
		return TotalTradeFee;

	//换算成系统统一币种
	CMarketData* pMarketData = pChangeList->pNewPosition->getAccount()->getCurrency()->getMarketData();
	if (pMarketData == NULL || pMarketData->LastPrice.isNull())
		return TotalTradeFee;
	CMoneyType DeduceFee = TotalTradeFee * pMarketData->LastPrice;

	// 用户手续费币种
	CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pChangeList->pOrder->MemberID, pChangeList->pOrder->AccountID, pChangeList->pInstrument->SettlementGroup, pChangeList->pOrder->getMember()->FeeDeduceCurrency);
	if (pAccount != NULL)
	{
		if (changeDepositFeeEach(pChangeList, pAccount, DeduceFee, pChangeList->pOrder->getMember()->FeeDeduceDisCount))
			return TotalTradeFee - DeduceFee;
	}

	// 系统抵扣金币种
	CServiceConfig* pServiceConfig = m_pMDB->m_ServiceConfigFactory->findByConfig("FeeDeduce", "default", "default");
	if (pServiceConfig == NULL || pServiceConfig->ConfigValue.isNull())
		return TotalTradeFee;
	CCurrencyType FeeDeduceCurreny = pServiceConfig->ConfigValue.getValue();
	CRatioType FeeDeduceDisCount = pServiceConfig->Remark.getDouble();

	pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pChangeList->pOrder->MemberID, pChangeList->pOrder->AccountID, pChangeList->pInstrument->SettlementGroup, FeeDeduceCurreny);
	if (pAccount != NULL)
	{
		if (changeDepositFeeEach(pChangeList, pAccount, DeduceFee, FeeDeduceDisCount))
			return TotalTradeFee - DeduceFee;
	}
	return TotalTradeFee;
}

bool CDepositCheckWedge::changeDeposit(CPositionChangeList* pChangeList, CDepositChange* pDepositChange, CTransaction* pTransaction)
{
	CAccount* pAccount = NULL;

	applyChange(pChangeList->pNewPosition, pDepositChange);
	if (pChangeList->pTrade != NULL)
	{
		pChangeList->pTrade->Position = pChangeList->pNewPosition->Position;
		pChangeList->pTrade->PositionID = pChangeList->pNewPosition->PositionID;
	}

	// 如果是成交，首先走手续费抵扣金逻辑
	if (pChangeList->pTrade != NULL && pChangeList->pOrder->AccountID == pChangeList->pOrder->MemberID)
	{
		CMoneyType totalFee = pDepositChange->feeChange;
		CMoneyType remainFee = changeDepositFee(pChangeList, pDepositChange, totalFee);
		pDepositChange->feeChange = remainFee;
	}

	pAccount = pChangeList->pNewPosition->getAccount();
	if (pAccount == NULL)
	{
		theAccount.init(m_pMDB);
		theAccount.CanReduce = true;
		theAccount.AccountID = pChangeList->pOrder->AccountID;
		theAccount.Currency = pChangeList->pOrder->ClearCurrency;
		theAccount.MemberID = pChangeList->pOrder->MemberID;
		theAccount.SettlementGroup = pChangeList->pOrder->SettlementGroup;
		pAccount = m_pMDB->m_AccountFactory->add(&theAccount);
		pChangeList->pNewPosition->linkAccount(pAccount);
	}

	//	if (pChangeList->pOrder->getAccount() == NULL)
	//		pChangeList->pOrder->linkAccount(pAccount);

	m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);

	CCurrency* pCurrency = theAccount.getCurrency();
	if (!pCurrency->IsTrading)
		return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS, "CurrencyIsNoTrading", pCurrency->Currency.getValue());

	CWriteableAccountDetail theAccountDetail;

	if (pChangeList->pTrade != NULL)
	{
		pChangeList->pTrade->Fee = pDepositChange->feeChange;
		pChangeList->pTrade->AccountID = pAccount->AccountID;
		theAccountDetail.PreBalance = pAccount->Balance;
		theAccountDetail.SettleSegment = pAccount->getCurrency()->SettleSegment;
		//爆仓之后，体验金不能使用
//		if (pChangeList->pTrade->DeriveSource == DS_FromOTCTrade_Liquidation)
//			theAccount.ReserveActive = false;
		pChangeList->pTrade->UseMargin = pDepositChange->marginChange;
		//简单第一步手续费同步
//		pChangeList->pTrade->ReserveFee = 0.0;

		//if (OF_Open == pChangeList->pTrade->OffsetFlag)
		//{
		//	pChangeList->pTrade->OpenPrice = pChangeList->pNewPosition->OpenPrice;
		//	pChangeList->pOrder->OpenPrice = pChangeList->pNewPosition->OpenPrice;
		//}
		//else
		//{
		//	if (pChangeList->pOldPosition != NULL)
		//	{
		//		pChangeList->pTrade->OpenPrice = pChangeList->pOldPosition->OpenPrice;
		//		pChangeList->pOrder->OpenPrice = pChangeList->pOldPosition->OpenPrice;
		//	}
		//}

		if (pChangeList->pNewPosition->OpenPrice.isNull())
		{
			if (pChangeList->pOldPosition != NULL)
			{
				pChangeList->pTrade->OpenPrice = pChangeList->pOldPosition->OpenPrice;
				pChangeList->pOrder->OpenPrice = pChangeList->pOldPosition->OpenPrice;
			}
		}
		else
		{
			pChangeList->pTrade->OpenPrice = pChangeList->pNewPosition->OpenPrice;
			pChangeList->pOrder->OpenPrice = pChangeList->pNewPosition->OpenPrice;
		}

		//if (!pCurrency->MoneyTick.isNull())
		//{
		//	pDepositChange->feeChange.toUpperTick(pCurrency->MoneyTick);
		//	pDepositChange->moneyChange.toLowerTick(pCurrency->MoneyTick);
		//	pDepositChange->closeProfit.toLowerTick(pCurrency->MoneyTick);
		//}
	}

	//释放保证金的时候，统计释放的保留资金是多少
	//平仓看资金使用之前的情况
	//if (pDepositChange->marginChange < 0.0)
	//{
		//pChangeList->pTrade->ReserveMargin = -MIN(pAccount->Reserve - pAccount->ReserveLimit, (-pDepositChange->marginChange.getValue()));
		//pChangeList->pTrade->ReserveFee = pChangeList->pTrade->Fee * (pChangeList->pTrade->ReserveMargin / pChangeList->pTrade->UseMargin);
		//if (pDepositChange->closeProfit > 0)
		//{
		//	//赚钱之后，一定比例之下可以使用，一定比例需要冻结，后续转走
		//	double cutProfit = -pChangeList->pTrade->ReserveMargin / fabs(pChangeList->pTrade->UseMargin);
		//	pChangeList->pTrade->ReserveProfit = pDepositChange->closeProfit * cutProfit;
		//	if (pChangeList->pTrade->ReserveProfit > (-pChangeList->pTrade->ReserveMargin))
		//	{
		//		double cutProfit = pChangeList->pTrade->ReserveProfit + pChangeList->pTrade->ReserveMargin;
		//		//theAccount.ReserveProfit += cutProfit;
		//		//theAccount.ReserveLimit -= pChangeList->pTrade->ReserveMargin;
		//		pChangeList->pTrade->ReserveProfit = cutProfit;
		//	}
		//	else
		//	{
		//		//theAccount.ReserveLimit += pChangeList->pTrade->ReserveProfit;
		//		pChangeList->pTrade->ReserveProfit = 0.0;
		//	}
		//}
	//}
	//else if (pDepositChange->marginChange > 0)
	//{
	//	pChangeList->pTrade->ReserveMargin = MIN(pAccount->Reserve.getValue(), pDepositChange->marginChange.getValue());
	//}

	///修改帐户的资金信息
	applyChange(&theAccount, pDepositChange);

	CMoneyType ReserveLoss = 0.0;
	if (pDepositChange->closeProfit < 0.0)
		ReserveLoss += pDepositChange->closeProfit;
	if (pDepositChange->feeChange > 0.0)
		ReserveLoss -= pDepositChange->feeChange;
	if (pDepositChange->moneyChange < 0.0)
		ReserveLoss += pDepositChange->moneyChange;
	applyReserveChange(pAccount, &theAccount, ReserveLoss);

	///检查帐号中的资金是否足够
	if (needCheckDeposit(pChangeList))
	{
		if (pCurrency->LoanLimit > 0.0)
		{
			double LoanUse = pCurrency->LoanUse + theAccount.Borrow - pAccount->Borrow;
			if (LoanUse / pCurrency->Balance.getValue() > pCurrency->LoanLimit.getValue())
				return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "LoanBorrow>LoanLimit");
		}
		if (needCheckDeposit(pChangeList->pOrder))
		{
			if (!theAccount.CanReduce)
				return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "AccountCanNotChange");

			if (pChangeList->pOrder->getMember()->RiskLevel != RL_Currency)
			{
				if (pCurrency->Balance < (pCurrency->LoanUse + theAccount.Borrow - pAccount->Borrow))
					return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "LoanTotal[%s]<LoanUse[%s]",
						pCurrency->Balance.getString().c_str(), pCurrency->LoanUse.getString().c_str());
				return true;
			}
			else
			{
				if (pDepositChange->isCrossMargin)
				{
					double PositionProfit = getPositionProfit(m_pMDB, &theAccount);
					pChangeList->pOrder->Available = theAccount.Available + PositionProfit + pDepositChange->frozenMarginChange + pDepositChange->frozenFeeChange;
					if (pChangeList->isOpen())
					{
						if (theAccount.Available < -PositionProfit)
							return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "AV[%s]+PP[%s]<0",
								theAccount.Available.getString().c_str(), doubleToStringFormat(PositionProfit).c_str());
					}
					CMoneyType NewAvailable = theAccount.Available + theAccount.FrozenFee + theAccount.FrozenMargin + theAccount.CrossMargin + PositionProfit + pDepositChange->toCloseProfit;
					if (NewAvailable < 0.0)
						return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "%s=AV[%s]+FF[%s]+FM[%s]+UM[%s]+PP[%s]+CP[%s]<0", NewAvailable.getString().c_str(),
							theAccount.Available.getString().c_str(), theAccount.FrozenFee.getString().c_str(), theAccount.FrozenMargin.getString().c_str(), theAccount.CrossMargin.getString().c_str(),
							doubleToStringFormat(PositionProfit).c_str(), pDepositChange->toCloseProfit.getString().c_str());
				}
				else
				{
					if (pChangeList->isOpen())
					{
						double PositionProfit = getPositionProfit(m_pMDB, &theAccount);
						pChangeList->pOrder->Available = theAccount.Available + PositionProfit + pDepositChange->frozenMarginChange + pDepositChange->frozenFeeChange;
						if (theAccount.Available < -PositionProfit)
							return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "AV[%s]+PP[%s]<0",
								theAccount.Available.getString().c_str(), doubleToStringFormat(PositionProfit).c_str());
					}
					double PositionProfit = 0.0;
					CPriceType basePrice = pChangeList->pInstrument->getMarketData()->MarkedPrice.getValue();
					if (basePrice.isNull())
						basePrice = pChangeList->pInstrument->getMarketData()->LastPrice.getValue();
					if (!basePrice.isNull())
						PositionProfit = getPositionProfit(pChangeList->pNewPosition, pChangeList->pInstrument, basePrice.getValue());

					CMoneyType NewMargin = pDepositChange->frozenMarginChange + pChangeList->pNewPosition->UseMargin + PositionProfit + pDepositChange->toCloseProfit;
					if (NewMargin < 0.0)
						return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "%s=FM[%s]+UM[%s]+PP[%s]+CP[%s]<0", NewMargin.getString().c_str(),
							pDepositChange->frozenMarginChange.getString().c_str(), pChangeList->pNewPosition->UseMargin.getString().c_str(),
							doubleToStringFormat(PositionProfit).c_str(), pDepositChange->toCloseProfit.getString().c_str());
				}
			}
		}
	}
	//	theAccount.UserID = pChangeList->pOrder->UserID;

	if (pChangeList->pTrade != NULL)
	{
		theAccountDetail.AccountID = theAccount.AccountID;
		theAccountDetail.MemberID = theAccount.MemberID;
		theAccountDetail.Currency = theAccount.Currency;
		theAccountDetail.SettlementGroup = theAccount.SettlementGroup;
		theAccountDetail.ExchangeID = pChangeList->pTrade->ExchangeID;
		theAccountDetail.InstrumentID = pChangeList->pTrade->InstrumentID;
		//theAccountDetail.ProductGroup = pChangeList->pTrade->ProductGroup;
		theAccountDetail.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		theAccountDetail.LocalID = pChangeList->pOrder->LocalID;
		theAccountDetail.PreBalance = pAccount->Balance;
		theAccountDetail.ReserveAmount = 0.0;
		theAccountDetail.ReserveBalance = theAccount.Reserve;

		pChangeList->pTrade->ReserveFee = 0.0;
		pChangeList->pTrade->ReserveProfit = 0.0;

		CMoneyType ReserveLoss = pAccount->Reserve - theAccount.Reserve;
		CMoneyType RealLoss = pAccount->Balance - theAccount.Balance - ReserveLoss;
		if (pDepositChange->feeChange > 0.0)
		{
			theAccountDetail.Amount = -pDepositChange->feeChange;
			theAccountDetail.Balance = theAccountDetail.PreBalance - pDepositChange->feeChange;
			//theAccountDetail.Amount.cutByBigFloat(theAccountDetail.Balance.getValue());
			theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
			theAccountDetail.Source = ACT_Fee;
			if (ReserveLoss > 0.0 && pDepositChange->feeChange > 0.0)
			{
				if (theAccount.ReserveType == RT_Inferior)
				{
					pChangeList->pTrade->ReserveFee = ReserveLoss;
					if (pChangeList->pTrade->ReserveFee >= pDepositChange->feeChange)
						pChangeList->pTrade->ReserveFee = pDepositChange->feeChange;
					ReserveLoss -= pChangeList->pTrade->ReserveFee;
					//sprintf(theAccountDetail.Remark.getBuffer(), "ReserveFee:%s", pChangeList->pTrade->ReserveFee.getString().c_str());
					theAccountDetail.ReserveAmount = -pChangeList->pTrade->ReserveFee;
					theAccountDetail.ReserveBalance = pAccount->Reserve - pChangeList->pTrade->ReserveFee;
				}
				else if (theAccount.ReserveType == RT_Prior)
				{
					RealLoss -= pDepositChange->feeChange;
					if (RealLoss < 0)
					{
						pChangeList->pTrade->ReserveFee = -RealLoss;
						if (pChangeList->pTrade->ReserveFee > 0.0)
						{
							theAccountDetail.ReserveAmount = -pChangeList->pTrade->ReserveFee;
							theAccountDetail.ReserveBalance = pAccount->Reserve - pChangeList->pTrade->ReserveFee;
						}
						//sprintf(theAccountDetail.Remark.getBuffer(), "ReserveFee:%s", pChangeList->pTrade->ReserveFee.getString().c_str());
					}
				}
			}

			theAccountDetail.RelatedID = pChangeList->pTrade->TradeID;
			theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();

			//m_pMDB->m_AccountDetailFactory->add(&theAccountDetail, pTransaction);
			m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);
			theAccountDetail.PreBalance = theAccountDetail.Balance;
		}

		if (pDepositChange->closeProfit != 0.0)
		{
			theAccountDetail.Amount = pDepositChange->closeProfit;
			theAccountDetail.Balance = theAccountDetail.PreBalance + pDepositChange->closeProfit;
			//theAccountDetail.Amount.cutByBigFloat(theAccountDetail.Balance.getValue());
			theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));

			switch (pChangeList->pTrade->DeriveSource)
			{
			case DS_FromOTCTrade_Liquidation:
				theAccountDetail.Source = ACT_Liquidation;
				break;
			case DS_FromOTCTrade_ADL:
				theAccountDetail.Source = ACT_ADL;
				break;
			default:
				theAccountDetail.Source = ACT_ClostProfit;
			}

			if (ReserveLoss > 0.0 && pDepositChange->closeProfit < 0.0)
			{
				if (theAccount.ReserveType == RT_Prior)
				{
					CMoneyType ReserveProfit = ReserveLoss;
					if (ReserveProfit >= -pDepositChange->closeProfit)
						ReserveProfit = -pDepositChange->closeProfit;
					ReserveLoss += ReserveProfit;
					//snprintf(theAccountDetail.Remark.getBuffer(), 64, "ReserveProfit:-%s", ReserveProfit.getString().c_str());
					pChangeList->pTrade->ReserveProfit = -ReserveProfit;
					theAccountDetail.ReserveAmount = -ReserveProfit;
					theAccountDetail.ReserveBalance = theAccount.Reserve;
				}
				else if (theAccount.ReserveType == RT_Inferior)
				{
					RealLoss += pDepositChange->closeProfit;
					if (RealLoss < 0)
					{
						CMoneyType ReserveProfit = RealLoss;
						if (ReserveProfit < pDepositChange->closeProfit)
							ReserveProfit = pDepositChange->closeProfit;
						//snprintf(theAccountDetail.Remark.getBuffer(), 64, "ReserveProfit:%s", ReserveProfit.getString().c_str());
						pChangeList->pTrade->ReserveProfit = ReserveProfit;
						theAccountDetail.ReserveAmount = -ReserveProfit;
						theAccountDetail.ReserveBalance = theAccount.Reserve;
					}
				}
			}
			theAccountDetail.RelatedID = pChangeList->pTrade->TradeID;
			theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
			//m_pMDB->m_AccountDetailFactory->add(&theAccountDetail, pTransaction);
			m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);
			theAccountDetail.PreBalance = theAccountDetail.Balance;
		}

		if (pDepositChange->moneyChange != 0.0)
		{
			theAccountDetail.Amount = pDepositChange->moneyChange;
			theAccountDetail.Balance = theAccountDetail.PreBalance + pDepositChange->moneyChange;
			//theAccountDetail.Amount.cutByBigFloat(theAccountDetail.Balance.getValue());
			theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
			theAccountDetail.Source = ACT_MoneyChange;
			if (pDepositChange->moneyChange < 0.0)
			{
				if (theAccount.ReserveType == RT_Prior)
				{
					CMoneyType ReserveProfit = ReserveLoss;
					if (ReserveProfit >= -pDepositChange->moneyChange)
						ReserveProfit = -pDepositChange->moneyChange;
					if (ReserveProfit > 0.0)
					{
						//snprintf(theAccountDetail.Remark.getBuffer(), 64, "ReserveProfit:-%s", ReserveProfit.getString().c_str());
						pChangeList->pTrade->ReserveProfit = -ReserveProfit;
						theAccountDetail.ReserveAmount = -ReserveProfit;
						theAccountDetail.ReserveBalance = theAccount.Reserve;
					}
				}
				else if (theAccount.ReserveType == RT_Inferior)
				{
					RealLoss += pDepositChange->moneyChange;
					if (RealLoss < 0)
					{
						CMoneyType ReserveProfit = RealLoss;
						if (ReserveProfit < pDepositChange->moneyChange)
							ReserveProfit = pDepositChange->moneyChange;
						//snprintf(theAccountDetail.Remark.getBuffer(), 64, "ReserveProfit:%s", ReserveProfit.getString().c_str());
						pChangeList->pTrade->ReserveProfit = -ReserveProfit;
						theAccountDetail.ReserveAmount = -ReserveProfit;
						theAccountDetail.ReserveBalance = theAccount.Reserve;
					}
				}
			}
			theAccountDetail.RelatedID = pChangeList->pTrade->TradeID;
			theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
			//m_pMDB->m_AccountDetailFactory->add(&theAccountDetail, pTransaction);
			m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);
			theAccountDetail.PreBalance = theAccountDetail.Balance;
		}

		if (pDepositChange->copyProfit > 0.0)
		{
			theAccountDetail.Amount = -pDepositChange->copyProfit;
			theAccountDetail.Balance = theAccountDetail.PreBalance - pDepositChange->copyProfit;
			theAccountDetail.Source = ACT_CopyProfit;
			theAccountDetail.RelatedID = pChangeList->pTrade->TradeID;
			theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
			m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);
		}
	}

	theAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	if (pChangeList->pTrade != NULL)
	{
		m_pMDB->m_AccountFactory->update(pAccount, &theAccount);
		if (theAccount.Balance < 0)
			theAccount.Borrow = -theAccount.Balance;

		if (theAccount.Borrow > pAccount->Borrow)
		{
			CWriteableCurrency theCurrency;
			m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
			theCurrency.LoanUse += theAccount.Borrow - pAccount->Borrow;
			m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
		}
	}
	else
	{
		m_pMDB->m_AccountFactory->update(pAccount, &theAccount, pTransaction);
	}

	return true;
}

bool CDepositCheckWedge::changeDepositAccountExchange(CPositionChangeList* pChangeList, CDepositChange* pDepositChange, CTransaction* pTransaction)
{
	// 如果是成交，首先走手续费抵扣金逻辑
	if (pChangeList->pTrade != NULL && pChangeList->pOrder->AccountID == pChangeList->pOrder->MemberID)
	{
		CMoneyType totalFee = 0.0;
		if (pChangeList->pTrade->Direction == D_Buy)
		{
			totalFee = pDepositChange->feeVolumeChange;
			CMoneyType remainFee = changeDepositFee(pChangeList, pDepositChange, totalFee);
			pDepositChange->feeVolumeChange = remainFee;
		}
		else
		{
			totalFee = pDepositChange->feeChange;
			CMoneyType remainFee = changeDepositFee(pChangeList, pDepositChange, totalFee);
			pDepositChange->feeChange = remainFee;
		}
	}

	//由于手续费收的是标的，持仓量必然会收到影响
	pChangeList->pNewPosition->Position -= pDepositChange->feeVolumeChange;
	applyChange(pChangeList->pNewPosition, pDepositChange, pChangeList->pOrder->Direction);
	if (pChangeList->pTrade != NULL)
	{
		pChangeList->pTrade->Position = pChangeList->pNewPosition->Position;
		pChangeList->pTrade->PositionID = pChangeList->pNewPosition->PositionID;
	}

	CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pChangeList->pOrder->MemberID, pChangeList->pOrder->AccountID, pChangeList->pOrder->SettlementGroup, pChangeList->pOrder->ClearCurrency);
	if (pAccount == NULL)
	{
		theAccount.init(m_pMDB);
		theAccount.AccountID = pChangeList->pOrder->AccountID;
		theAccount.Currency = pChangeList->pOrder->ClearCurrency;
		theAccount.MemberID = pChangeList->pOrder->MemberID;
		theAccount.SettlementGroup = pChangeList->pOrder->SettlementGroup;
		pAccount = m_pMDB->m_AccountFactory->add(&theAccount);
	}

	m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);

	if (!theAccount.getCurrency()->IsTrading)
		return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS, "CurrencyIsNoTrading", theAccount.getCurrency()->Currency.getValue());
	if (pChangeList->pOrder->Direction == D_Buy)
		applyChange(&theAccount, pDepositChange, pChangeList->pOrder->Direction);
	else
		applyVolumeChange(&theAccount, pDepositChange);

	CCurrency* pCurrency = theAccount.getCurrency();


	if (needCheckDeposit(pChangeList))
	{
		if (!theAccount.CanReduce)
			return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "AccountCanNotChange");
		if (pChangeList->pOrder->getMember()->RiskLevel != RL_Currency)
		{
			//if (pCurrency->LoanTotal < (pCurrency->LoanUse + theAccount.Borrow - pAccount->Borrow - pCurrency->LoanOverflow))
			//	return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "LoanTotal[%s]<LoanUse[%s]+New",
			//		pCurrency->LoanTotal.getString().c_str(), pCurrency->LoanUse.getString().c_str());
		}
		else
		{
			if (theAccount.Available < 0.0)
				return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, theAccount.Available.getString().c_str());
		}

		if (pChangeList->pOrder->Direction == D_Buy)
		{
			CCurrency* pBaseCurrency = pChangeList->pOrder->getInstrument()->getBaseCurrency();
			if (pBaseCurrency != NULL && !pBaseCurrency->PrivateEnd.isNull())
			{
				CVolumeType longVolume = pChangeList->pNewPosition->LongFrozen.getValue();
				CVolumeType mustVolume = pBaseCurrency->Balance.getValue();
				CAccount* pOtherAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pChangeList->pOrder->MemberID, pChangeList->pOrder->AccountID, pChangeList->pOrder->SettlementGroup, pBaseCurrency->Currency);
				if (pOtherAccount != NULL)
				{
					longVolume += pOtherAccount->Balance.getValue();
					mustVolume -= pOtherAccount->Balance.getValue();
				}

				if (longVolume >= pBaseCurrency->Balance.getValue() * pBaseCurrency->PrivateEnd.getValue())
				{
					if (pChangeList->pOrder->OrderType != OT_Normal)
						return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Percent>%s,OrderTypeMustGTC", pBaseCurrency->PrivateEnd.getString().c_str());
					if (pChangeList->pNewPosition->LongFrozen > pChangeList->pOrder->Volume.getValue())
						return m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "Percent>%s,CancleOtherOrders", pBaseCurrency->PrivateEnd.getString().c_str());
					if (longVolume < pBaseCurrency->Balance.getValue())
						return m_pErrorEngine->reportError(ERROR_INVALID_VOLUME, "Percent>%s,VolumeMust>=%s", pBaseCurrency->PrivateEnd.getString().c_str(), mustVolume.getString().c_str());
				}
			}
		}
		pChangeList->pOrder->Available = theAccount.Available;
	}

	//如果是成交，需要改变本方比重和对方币种资金
	//财务记录：本方扣掉，对方得到，对方手续费
	if (pChangeList->pTrade != NULL)
	{
		// 更新trade的成本价
		pChangeList->pTrade->OpenPrice = pChangeList->pNewPosition->OpenPrice;
		if (pChangeList->pTrade->Direction == D_Buy)
			pChangeList->pTrade->Fee = pDepositChange->feeVolumeChange;
		else
			pChangeList->pTrade->Fee = pDepositChange->feeChange;

		CWriteableAccountDetail theAccountDetail;
		//		theAccountDetail.TradeUnitID = pChangeList->pTrade->TradeUnitID;
		theAccountDetail.ExchangeID = pChangeList->pTrade->ExchangeID;
		theAccountDetail.InstrumentID = pChangeList->pTrade->InstrumentID;
		//theAccountDetail.ProductGroup = pChangeList->pTrade->ProductGroup;
		theAccountDetail.MemberID = pChangeList->pTrade->MemberID;
		theAccountDetail.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		theAccountDetail.LocalID = pChangeList->pOrder->LocalID;
		theAccountDetail.AccountID = pChangeList->pOrder->AccountID;
		theAccountDetail.SettlementGroup = pChangeList->pOrder->SettlementGroup;
		theAccountDetail.Source = ACT_MoneyChange;
		//本方扣掉
		theAccountDetail.Currency = theAccount.Currency;
		theAccountDetail.SettleSegment = theAccount.getCurrency()->SettleSegment;
		theAccountDetail.PreBalance = pAccount->Balance;
		theAccountDetail.RelatedID = pChangeList->pTrade->TradeID;
		theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
		pChangeList->pTrade->AccountID = pChangeList->pOrder->AccountID;
		if (pChangeList->pOrder->Direction == D_Buy)
			theAccountDetail.Amount = pDepositChange->moneyChange;
		else
			theAccountDetail.Amount = pDepositChange->volumeChange;
		//theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
		theAccountDetail.Balance = theAccountDetail.PreBalance + theAccountDetail.Amount;
		m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);

		//另一个币种账户得到
		if (pChangeList->pOrder->Direction == D_Buy)
			theAccountDetail.Currency = pChangeList->pOrder->getInstrument()->BaseCurrency;
		else
			theAccountDetail.Currency = pChangeList->pOrder->getInstrument()->ClearCurrency;

		CAccount* pOtherAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pChangeList->pOrder->MemberID, pChangeList->pOrder->AccountID, pChangeList->pOrder->SettlementGroup, theAccountDetail.Currency);
		if (pOtherAccount == NULL)
		{
			theOtherAccount.init(m_pMDB);
			theOtherAccount.AccountID = pChangeList->pOrder->AccountID;
			theOtherAccount.MemberID = pChangeList->pOrder->MemberID;
			theOtherAccount.Currency = theAccountDetail.Currency;
			theOtherAccount.SettlementGroup = pChangeList->pOrder->SettlementGroup;
		}
		else
			m_pMDB->m_AccountFactory->retrieve(pOtherAccount, &theOtherAccount);
		theAccountDetail.PreBalance = theOtherAccount.Balance;
		if (pChangeList->pOrder->Direction == D_Buy)
		{
			applyVolumeChange(&theOtherAccount, pDepositChange);
			CCurrency* pCurrency = theOtherAccount.getCurrency();
			if (pCurrency != NULL && !pCurrency->PrivateEnd.isNull())
			{
				if (theOtherAccount.Balance >= pCurrency->Balance.getValue() * pCurrency->PrivateEnd.getValue())
				{
					CMarketData* pMarketData = pChangeList->pInstrument->getMarketData();
					CWriteableMarketData theMarketData;
					m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
					theMarketData.InstrumentStatus = IS_NoTrading;
					m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
				}
			}
			theAccountDetail.Amount = pDepositChange->volumeChange;
			theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
			theAccountDetail.Balance = theAccountDetail.PreBalance + theAccountDetail.Amount;
		}
		else
		{
			applyChange(&theOtherAccount, pDepositChange, pChangeList->pOrder->Direction);
			theAccountDetail.Amount = pDepositChange->moneyChange;
			theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
			theAccountDetail.Balance = theAccountDetail.PreBalance + theAccountDetail.Amount;
		}

		//现货不结算，要及时清理
		//applyAccountClear(&theOtherAccount);

		applyAccountChange(&theOtherAccount);
		if (pOtherAccount == NULL)
		{
			theOtherAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			pOtherAccount = m_pMDB->m_AccountFactory->add(&theOtherAccount);
		}
		else
		{
			theOtherAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_AccountFactory->update(pOtherAccount, &theOtherAccount);
		}

		if (theOtherAccount.Currency == pChangeList->pNewPosition->ClearCurrency && pChangeList->pNewPosition->getAccount() == NULL)
			pChangeList->pNewPosition->linkAccount(pOtherAccount);

		theAccountDetail.SettleSegment = theOtherAccount.getCurrency()->SettleSegment;
		theAccountDetail.RelatedID = pChangeList->pTrade->TradeID;
		theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
		if (pDepositChange->closeProfit != 0)
			sprintf(theAccountDetail.Remark.getBuffer(), "CloseProfit:%s", pDepositChange->closeProfit.getString().c_str());
		m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);

		//对方币种手续费
		if (pChangeList->pTrade->Fee > 0.0)
		{
			theAccountDetail.PreBalance = theAccountDetail.Balance;
			if (pChangeList->pOrder->Direction == D_Buy)
				theAccountDetail.Amount = -pDepositChange->feeVolumeChange;
			else
				theAccountDetail.Amount = -pDepositChange->feeChange;
			theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
			theAccountDetail.Balance = theAccountDetail.PreBalance + theAccountDetail.Amount;
			theAccountDetail.Source = ACT_Fee;
			theAccountDetail.RelatedID = pChangeList->pTrade->TradeID;
			theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
			m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);
		}
	}

	// 为了保留流水中的PreBalance，需要在流水之后再更新pAccount
	//现货不结算，要及时清理
	//applyAccountClear(&theAccount);
	applyAccountChange(&theAccount);
	//	theAccount.UserID = pChangeList->pOrder->UserID;
	theAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	if (pChangeList->pTrade != NULL)
	{
		if (theAccount.Balance < 0 && pAccount->Borrow < -theAccount.Balance)
		{
			theAccount.Borrow = -theAccount.Balance;
			CWriteableCurrency theCurrency;
			m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
			theCurrency.LoanUse += theAccount.Borrow - pAccount->Borrow;
			LoanLoanInterest(&theCurrency);
			m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
		}
		if (pChangeList->pTrade->MatchRole == MR_Taker)
			m_pMDB->m_AccountFactory->update(pAccount, &theAccount, NULL, false);
		else
			m_pMDB->m_AccountFactory->update(pAccount, &theAccount);
	}
	else
	{
		m_pMDB->m_AccountFactory->update(pAccount, &theAccount, pTransaction);
	}
	return true;
}

bool CDepositCheckWedge::changePosition(CPositionChangeList* pChangeList, CTransaction* pTransaction)
{
	///计算资金的变化量
	CDepositChange change;

	switch (pChangeList->pInstrument->ProductClass)
	{
	case PC_AccountExchange:
	{
		if (pChangeList->pOrder->getMember()->MemberType == PT_InnerMarketMaker)
			m_pExchangeAlgo->calculateDepositDifference1(pChangeList, &change, pTransaction);
		else
			m_pExchangeAlgo->calculateDepositDifference2(pChangeList, &change, pTransaction);

		return changeDepositAccountExchange(pChangeList, &change, pTransaction);
	}
	case PC_Premium:
	case PC_Margin:
	{
		if (pChangeList->pTrade != NULL && pChangeList->pOrderCancle != NULL)
		{
			change.isCrossMargin = pChangeList->pNewPosition->IsCrossMargin.getValue();
			if (pChangeList->pNewPosition->Leverage == 0.0)
				return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "Leverage=0.0");
		}

		//平仓、撤单不需要检查杠杆
		//if (pChangeList->isOpen() && pChangeList->pOrderCancle == NULL && pChangeList->pTrade == NULL)
		//{
		//	//强平接管账户开仓不需要限制杠杆
		//	if (pChangeList->pOrder->DeriveSource != DS_FromOTCTrade_Liquidation)
		//	{
		//		CMarginRate *pRate;
		//		pRate = getMarginRate(m_pMDB, pChangeList->pInstrument, pChangeList->pOrder->MemberID);
		//		if (pRate == NULL)
		//			return m_pErrorEngine->reportError(ERROR_CAN_NOT_FIND_MARGIN_RATE);

		//		double minMarginRatio = getMinMarginRatio(pChangeList->pNewPosition->Position, pChangeList->pNewPosition->LongFrozen, pChangeList->pNewPosition->ShortFrozen, pRate, pChangeList->pNewPosition->PosiDirection);
		//		double Leverage = 1.0;
		//		if (pRate->ValueMode == VM_Absolute)
		//			Leverage = -minMarginRatio;
		//		else
		//			Leverage = getInverse(minMarginRatio);

		//		if ( pChangeList->pNewPosition->Leverage > Leverage)
		//		{
		//			return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "PositionLeveage[%.4f]>MaxLeveage[%.4f]", pChangeList->pNewPosition->Leverage.getValue(), Leverage);
		//			//如果杠杆不够，合约里面的默认杠杆不符合要求，需要自动降低杠杆
		//			//if (pChangeList->pOldPosition != NULL)
		//			//	return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH);
		//			//else
		//			//	pChangeList->pNewPosition->Leverage = Leverage;
		//		}
		//		//CPosition *pPositon = m_pMDB->m_PositionFactory->findFirstByMemberIDAndLeverage(pChangeList->pOrder->MemberID, pChangeList->pOrder->AccountID, pChangeList->pOrder->ExchangeID,
		//		//	pChangeList->pOrder->InstrumentID, pChangeList->pOrder->PosiDirection);
		//		//m_pMDB->m_PositionFactory->endFindByMemberIDAndLeverage();
		//		//if (pPositon != NULL)
		//		//{
		//		//	if (pPositon->Leverage > Leverage)
		//		//		return m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "AllPositionMaxLeveage[%.4f]>MaxLeveage[%.4f]", pPositon->Leverage.getValue(), Leverage);
		//		//}
		//	}
		//}
		if (pChangeList->pOrder->getMember()->MemberType == PT_InnerMarketMaker)
			m_pBaseAlgo->calculateDepositDifference1(pChangeList, &change, pTransaction);
		else
			m_pBaseAlgo->calculateDepositDifference2(pChangeList, &change, pTransaction);

		return changeDeposit(pChangeList, &change, pTransaction);
	}
	default:
	{
	}
	}
	return true;
}

void CDepositCheckWedge::applyChange(CWriteablePosition* pPosition, CDepositChange* pChange)
{
	CVolumeType VolumeTick = pPosition->getInstrument()->VolumeTick;
	if (pPosition->Position < VolumeTick && pPosition->Position > -VolumeTick)
		pPosition->Position = 0.0;
	//if (pPosition->PrePosition < VolumeTick && pPosition->PrePosition > -VolumeTick)
	//	pPosition->PrePosition = 0.0;
	if (pPosition->LongFrozen < VolumeTick)
		pPosition->LongFrozen = 0.0;
	if (pPosition->ShortFrozen < VolumeTick)
		pPosition->ShortFrozen = 0.0;

	if (pPosition->Position == 0.0)
	{
		pPosition->TotalPositionCost = 0.0;
		pPosition->OpenPrice.clear();
		pPosition->TotalCloseProfit = 0.0;
		pPosition->PositionCost = 0.0;
		pPosition->CostPrice.clear();
		pPosition->CloseProfit = 0.0;
		pPosition->UseMargin = 0.0;
		pPosition->CopyProfit = 0.0;
	}
	else
	{
		pPosition->TotalPositionCost += pChange->totalPositionCostChange;
		if (pPosition->getInstrument()->IsInverse)
			if (pPosition->TotalPositionCost == 0.0)
				pPosition->OpenPrice = 0.0;
			else
				pPosition->OpenPrice = fabs(pPosition->getInstrument()->VolumeMultiple * pPosition->Position / pPosition->TotalPositionCost);
		else
			pPosition->OpenPrice = fabs(pPosition->TotalPositionCost / pPosition->getInstrument()->VolumeMultiple / pPosition->Position);

		pPosition->TotalCloseProfit += pChange->totalCloseProfit;

		pPosition->CloseProfit += pChange->closeProfit;

		if (pChange->copyProfit > 0)
			pPosition->CopyProfit += pChange->copyProfit;
	}

	if (pPosition->LongFrozen == 0.0)
		pPosition->LongFrozenMargin = 0.0;
	else
		pPosition->LongFrozenMargin += pChange->longFrozenChange;

	if (pPosition->ShortFrozen == 0.0)
		pPosition->ShortFrozenMargin = 0.0;
	else
		pPosition->ShortFrozenMargin += pChange->shortFrozenChange;

	if (pPosition->LongFrozen == 0.0 && pPosition->ShortFrozen == 0.0)
		pPosition->FrozenMargin = 0.0;
	else
		pPosition->FrozenMargin += pChange->frozenMarginChange;

	if (pPosition->FrozenMargin <= 0.0)
		pPosition->FrozenMargin = 0.0;
}

void CDepositCheckWedge::applyChange(CWriteablePosition* pPosition, CDepositChange* pChange, CDirectionType Direction)
{
	CVolumeType VolumeTick = pPosition->getInstrument()->VolumeTick;
	if (pPosition->Position < VolumeTick && pPosition->Position > -VolumeTick)
		pPosition->Position = 0.0;
	if (pPosition->ClosePosition < VolumeTick)
		pPosition->ClosePosition = 0.0;
	//if (pPosition->PrePosition < VolumeTick && pPosition->PrePosition > -VolumeTick)
	//	pPosition->PrePosition = 0.0;
	if (pPosition->LongFrozen < VolumeTick)
		pPosition->LongFrozen = 0.0;
	if (pPosition->ShortFrozen < VolumeTick)
		pPosition->ShortFrozen = 0.0;

	if (pPosition->Position == 0.0)
	{
		pPosition->TotalPositionCost = 0.0;
		pPosition->OpenPrice.clear();
		pPosition->TotalCloseProfit = 0.0;
		pPosition->PositionCost = 0.0;
		pPosition->CostPrice.clear();
		pPosition->CloseProfit = 0.0;
		pPosition->UseMargin = 0.0;
	}
	else
	{
		pPosition->TotalPositionCost += pChange->totalPositionCostChange;
		if (Direction == D_Buy)
		{
			if (pPosition->getInstrument()->IsInverse)
				if (pPosition->TotalPositionCost == 0.0)
					pPosition->OpenPrice.clear();
				else
					pPosition->OpenPrice = fabs(pPosition->getInstrument()->VolumeMultiple * pPosition->Position / pPosition->TotalPositionCost);
			else
				pPosition->OpenPrice = fabs(pPosition->TotalPositionCost / pPosition->getInstrument()->VolumeMultiple / pPosition->Position);
		}

		pPosition->TotalCloseProfit += pChange->totalCloseProfit;
		//pPosition->PositionCost = 0.0;
		//pPosition->CostPrice = 0.0;
		pPosition->CloseProfit += pChange->closeProfit;
		pPosition->UseMargin += pChange->marginChange;
	}

	if (pPosition->LongFrozen == 0.0)
		pPosition->LongFrozenMargin = 0.0;
	else
		pPosition->LongFrozenMargin += pChange->longFrozenChange;

	if (pPosition->ShortFrozen == 0.0)
		pPosition->ShortFrozenMargin = 0.0;
	else
		pPosition->ShortFrozenMargin += pChange->shortFrozenChange;

	if (pPosition->LongFrozen == 0.0 && pPosition->ShortFrozen == 0.0)
	{
		pPosition->FrozenMargin = 0.0;
	}
	else
	{
		pPosition->FrozenMargin += pChange->frozenMarginChange;
	}
}

void CDepositCheckWedge::applyChange(CWriteableAccount* pAccount, CDepositChange* pChange)
{
	double MoneyTick = pAccount->getCurrency()->MoneyTick;

	pAccount->MoneyChange += pChange->moneyChange;
	pAccount->MoneyChange.toLowerTick(MoneyTick);
	pAccount->CloseProfit += pChange->closeProfit;
	pAccount->CloseProfit.toLowerTick(MoneyTick);
	pAccount->Fee += pChange->feeChange;
	pAccount->Fee.toUpperTick(MoneyTick);

	pAccount->TotalMoneyChange += pChange->moneyChange;
	pAccount->TotalCloseProfit += pChange->totalCloseProfit;
	pAccount->TotalFee += pChange->feeChange;

	pAccount->PositionCost += pChange->positionCostChange;
	if (pAccount->PositionCost < MoneyTick)
		pAccount->PositionCost = 0.0;

	pAccount->MaintMargin += pChange->maintMarginChange;
	if (pAccount->MaintMargin < MoneyTick)
		pAccount->MaintMargin = 0.0;

	pAccount->UseMargin += pChange->marginChange;
	if (pAccount->UseMargin < MoneyTick)
		pAccount->UseMargin = 0.0;
	if (pChange->isCrossMargin)
	{
		pAccount->CrossMargin += pChange->marginChange;
		if (pAccount->CrossMargin < MoneyTick)
			pAccount->CrossMargin = 0.0;
	}

	pAccount->FrozenMargin += pChange->frozenMarginChange;
	if (pAccount->FrozenMargin < MoneyTick)
		pAccount->FrozenMargin = 0.0;

	pAccount->FrozenMoney += pChange->frozenMoneyChange;
	if (pAccount->FrozenMoney < MoneyTick)
		pAccount->FrozenMoney = 0.0;

	pAccount->FrozenFee += pChange->frozenFeeChange;
	if (pAccount->FrozenFee < MoneyTick)
		pAccount->FrozenFee = 0.0;

	if (pChange->copyProfit > 0)
		pAccount->CopyProfit += pChange->copyProfit;
	if (pAccount->CopyProfit < MoneyTick)
		pAccount->CopyProfit = 0.0;

	applyAccountChange(pAccount);
}

void CDepositCheckWedge::applyChange(CWriteableAccount* pAccount, CDepositChange* pChange, CDirectionType Direction)
{
	CMoneyType MoneyTick = pAccount->getCurrency()->MoneyTick;

	pAccount->MoneyChange += pChange->moneyChange;
	pAccount->MoneyChange.toLowerTick(MoneyTick);
	pAccount->Fee += pChange->feeChange;
	pAccount->Fee.toUpperTick(MoneyTick);

	pAccount->TotalFee += pChange->feeChange;
	pAccount->TotalMoneyChange += pChange->moneyChange;

	pAccount->FrozenMoney += pChange->frozenMoneyChange;
	if (pAccount->FrozenMoney < MoneyTick)
		pAccount->FrozenMoney = 0.0;

	applyAccountChange(pAccount);
}

void CDepositCheckWedge::applyVolumeChange(CWriteableAccount* pAccount, CDepositChange* pChange)
{
	CMoneyType MoneyTick = pAccount->getCurrency()->MoneyTick;

	pAccount->CloseProfit += pChange->closeProfit;
	pAccount->CloseProfit.toLowerTick(MoneyTick);
	pAccount->MoneyChange += pChange->volumeChange;
	pAccount->MoneyChange.toLowerTick(MoneyTick);
	pAccount->Fee += pChange->feeVolumeChange;
	pAccount->Fee.toUpperTick(MoneyTick);

	pAccount->TotalMoneyChange += pChange->volumeChange;
	pAccount->TotalCloseProfit += pChange->totalCloseProfit;
	pAccount->TotalFee += pChange->feeVolumeChange;

	pAccount->FrozenMoney += pChange->frozenVolumeChange;
	if (pAccount->FrozenMoney < MoneyTick)
		pAccount->FrozenMoney = 0.0;

	applyAccountChange(pAccount);
}
