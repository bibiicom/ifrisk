#include "CSettlementService.h"
#include "tradingTools.h"
#include "monitorIndex.h"
#include "CopyField.h"
#include "printinfo.h"
#include "monitorIndex.h"
#include "mdbStruct.h"
#include "Config.h"

CSettlementService::CSettlementService(CMDB* pMDB, CErrorEngine* pErrorEngine)
{
	m_pMDB = pMDB;
	m_pErrorEngine = pErrorEngine;
	sSnapPath = GetConfigString(INI_FILE_NAME, "SnapPath");
	m_pErrorFile = NULL;
}

CSettlementService::~CSettlementService(void)
{
}

bool CSettlementService::AccountWithdraw(CWriteableAccount* pAccount, CAccountActionField* pAccountAction)
{
	pAccount->Withdraw += pAccountAction->Amount;
	pAccount->TotalWithdraw += pAccountAction->Amount;
	if (pAccountAction->IsReserve)
	{
		pAccount->Reserve -= pAccountAction->Amount;

		if (pAccount->Reserve < 0.0)
		{
			REPORT_EVENT(LOG_CRITICAL, "Account", "[%s] Reserve[%0.f] < 0 ", pAccount->AccountID.getValue(), pAccount->Reserve.getValue());
			m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY);
			return false;
		}
	}
	applyAccountChange(pAccount);

	//出金需要计算浮动盈亏
	double PositionProfit = getPositionProfit(m_pMDB, pAccount);
	if (PositionProfit > 0.0)
		PositionProfit = 0.0;
	//这里一定要重载使用<，要不然就会不能double方式判断
	if (pAccount->Withdrawable < -PositionProfit)
		return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY);
	return true;
}

//bool CSettlementService::Loan(CAccountActionField* pAccountAction, CAccountField* pOutAccount, CTransaction* pTransaction)
//{
//	if (pAccountAction->Amount.isNull())
//		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "AmountIsNull");
//
//	if (pAccountAction->Amount <= 0.0)
//		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "Amount<=0");
//
//	if (pAccountAction->AccountID.isNull())
//		return m_pErrorEngine->reportError(ERROR_AccountNotFound, "[AccountID:%s]IsNull", pAccountAction->AccountID.getValue());
//
//	//寻找币种在充值的列表中
//	CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pAccountAction->Currency, pAccountAction->SettlementGroup);
//	if (pCurrency == NULL)
//		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "Currency[%s]NotFound", pAccountAction->Currency.getValue());
//	if (!pCurrency->CanChange)
//		return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS, "CurrencyCanNotChange");
//
//	CWriteableCurrency theCurrency;
//	m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
//
//	CWriteableAccount theAccount;
//	CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pAccountAction->MemberID, pAccountAction->AccountID, pAccountAction->SettlementGroup, pAccountAction->Currency);
//	if (pAccount == NULL)
//		return m_pErrorEngine->reportError(ERROR_AccountNotFound, "[AccountID:%s]", pAccountAction->AccountID.getValue());
//	m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
//	switch (pAccountAction->ActionType)
//	{
//	case AAT_LoanLend:
//	{
//		theAccount.FrozenMoney += pAccountAction->Amount;
//		theAccount.Available -= pAccountAction->Amount;
//		theAccount.Withdrawable -= pAccountAction->Amount;
//		double PositionProfit = getPositionProfit(m_pMDB, &theAccount);
//		if (PositionProfit > 0.0)
//			PositionProfit = 0.0;
//		if (theAccount.Withdrawable < -PositionProfit)
//			return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "Withdrawable(%s)<-PositionProfit(%0.f)", theAccount.Withdrawable.getString().c_str(), PositionProfit);
//
//		CPosition *pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(theAccount.AccountID, theAccount.SettlementGroup.getValue(),
//			theAccount.Currency.getValue(), theAccount.AccountID.getValue(), PD_Long);
//		CWriteablePosition thePosition;
//		if (pPosition == NULL)
//		{
//			thePosition.init();
//			thePosition.PositionClass = PC_Loan;
//			thePosition.Position = pAccountAction->Amount;
//			thePosition.PositionID = m_gOrderSequence.getNextValueByString();
//			thePosition.InstrumentID = theAccount.Currency;
//			thePosition.ExchangeID = theAccount.SettlementGroup;
//			thePosition.SettlementGroup = theAccount.SettlementGroup;
//			thePosition.MemberID = theAccount.AccountID;
//			thePosition.PosiDirection = PD_Long;
//			thePosition.ClearCurrency = theAccount.Currency;
//			thePosition.AccountID = theAccount.AccountID;
//			thePosition.TradeUnitID = theAccount.AccountID.getValue();
//			thePosition.linkAccount(pAccount);
//			m_pMDB->m_PositionFactory->add(&thePosition, pTransaction);
//		}
//		else
//		{
//			m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
//			thePosition.Position += pAccountAction->Amount;
//			m_pMDB->m_PositionFactory->update(pPosition, &thePosition, pTransaction);
//		}
//		theCurrency.LoanTotal += pAccountAction->Amount;
//		LoanLoanInterest(&theCurrency);
//		break;
//	}
//	case AAT_LoanBack:
//	{
//		theAccount.FrozenMoney -= pAccountAction->Amount;
//		theAccount.Available += pAccountAction->Amount;
//		theAccount.Withdrawable += pAccountAction->Amount;
//		CPosition* pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(theAccount.AccountID, theAccount.SettlementGroup.getValue(),
//			theAccount.Currency.getValue(), theAccount.AccountID.getValue(), PD_Long);
//		if (pPosition == NULL)
//			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "PositionNotFound");
//		CWriteablePosition thePosition;
//		m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
//		thePosition.Position -= pAccountAction->Amount;
//		if (thePosition.Position == 0.0)
//			m_pMDB->m_PositionFactory->remove(pPosition, pTransaction);
//		else
//			m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
//		theCurrency.LoanTotal -= pAccountAction->Amount;
//		LoanLoanInterest(&theCurrency);
//		break;
//	}
//	}
//	m_pMDB->m_AccountFactory->update(pAccount, &theAccount, pTransaction);
//	m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency, pTransaction);
//
//	if (pOutAccount != NULL)
//		CopyAccountEntity(pOutAccount, &theAccount);
//	return true;
//}

bool CSettlementService::ReqAccountAction(CAccountActionField* pAccountAction, CAccountField* pOutAccount, CTransaction* pTransaction)
{
	//ToDo:由于double数据过大导致数据不精确
	//这里预备的方案是 如果prebalance过大，能入金的小数点就会小，这样来避免流水账务对不上的情况

	COrder* pOrder = NULL;
	//先找出要操作的订单，为了后续检查使用
	if (!pAccountAction->ActionLocalID.isNull() )
	{
		pOrder = m_pMDB->m_OrderFactory->findByLocalID(pAccountAction->AccountID, pAccountAction->ActionLocalID);
		if (pOrder == NULL)
			return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND);
	}

	if (!pAccountAction->OrderID.isNull())
	{
		pOrder = m_pMDB->m_OrderFactory->findByOrderID(pAccountAction->OrderID);
		if (pOrder == NULL)
			return m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND);
	}

	if (pOrder != NULL)
	{
		pAccountAction->AccountID = pOrder->AccountID;
		pAccountAction->SettlementGroup = pOrder->SettlementGroup;
		pAccountAction->Currency = pOrder->ClearCurrency;
		pAccountAction->MemberID = pOrder->MemberID;
	}

	if (pAccountAction->AccountID.isNull())
		return m_pErrorEngine->reportError(ERROR_AccountNotFound, "[AccountID:%s]IsNull", pAccountAction->AccountID.getValue());

	//寻找币种在充值的列表中
	CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pAccountAction->Currency, pAccountAction->SettlementGroup);
	if (pCurrency == NULL)
		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "Currency[%s]NotFound", pAccountAction->Currency.getValue());
	
	CWriteableCurrency theCurrency;
	m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);

	CWriteableAccount theAccount;
	CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pAccountAction->MemberID, pAccountAction->AccountID, pAccountAction->SettlementGroup, pAccountAction->Currency);
	if (pAccount == NULL)
	{
		if (pAccountAction->MemberID.isNull())
			pAccountAction->MemberID = pAccountAction->AccountID;
		CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(pAccountAction->MemberID);
		if (pMember == NULL)
			return m_pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);

		//如果没有，就需要增加一个
		theAccount.init(m_pMDB);
		theAccount.CanReduce = true;
		theAccount.AccountID = pAccountAction->AccountID;
		theAccount.AccountType = pAccountAction->AccountType;
		theAccount.Currency = pAccountAction->Currency;
		theAccount.MemberID = pAccountAction->MemberID;
		theAccount.MaxLocalID = pAccountAction->LocalID;
		theAccount.SettlementGroup = pAccountAction->SettlementGroup;
		//theAccount.ProductGroup = pCurrency->SettlementGroup;
		theAccount.linkCurrency(pCurrency);
		CServiceConfig* pServiceConfig = m_pMDB->m_ServiceConfigFactory->findByConfig("DefaultReserveType", "default", "default");
		if (pServiceConfig != NULL)
			theAccount.ReserveType = pServiceConfig->ConfigValue[0];
		else
			theAccount.ReserveType = RT_Inferior;
	}
	else
	{
		m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
	}

	switch (pAccountAction->ActionType)
	{
	case AAT_Withdraw:
	case AAT_FrozenMoney:
	case AAT_FrozenToWithdraw:
	case AAT_FrozenMoneyByOrder:
		if (!pCurrency->CanReduce)
			return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS, "CurrencyCanNotChange");
	case AAT_InnerTransfer:
		if (!theAccount.CanReduce)
			return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "AccountCanNotChange");
	}

	if (pAccountAction->Amount.isNull())
	{
		switch (pAccountAction->ActionType)
		{
		case AAT_Withdraw:
		case AAT_InnerTransfer:
		{
			if (pAccountAction->IsReserve)
			{
				if (theAccount.Balance < theAccount.Reserve)
				{
					//这种情况下，体验金要归零的，因为全部把体验金提走了
					pAccountAction->Amount = theAccount.Balance;
					theAccount.Reserve = theAccount.Balance;
				}
				else
				{
					pAccountAction->Amount = theAccount.Reserve;
				}
			}
			else
				pAccountAction->Amount = theAccount.Withdrawable;
			break;
		}
		case AAT_FrozenMoney:
		{
			if (pAccountAction->IsReserve)
				pAccountAction->Amount = theAccount.Reserve;
			else
				pAccountAction->Amount = theAccount.Withdrawable;
			break;
		}
		case AAT_FreeFrozenMoney:
		case AAT_FrozenToWithdraw:
		{
			if (pOrder != NULL)
				pAccountAction->Amount = pOrder->Cost;
			else
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "AmountIsNull");
			break;
		}
		case AAT_FrozenMoneyByOrder:
		case AAT_Deposit:
		default:
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "AmountIsNull");
		}
	}

	if (pAccountAction->Amount < 0.0)
	{
		REPORT_EVENT(LOG_CRITICAL, "Account", "[%s]Amount[%f]IsNegtive", pAccount->AccountID.getValue(), pAccountAction->Amount.getValue());
		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "AmountIsNegtive[%f]", pAccountAction->Amount.getValue());
	}

	//if (!pAccountAction->Amount.isTick(theAccount.Balance.getCompareTick()))
	//{
	//	CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(theAccount.MemberID);
	//	if (pMember == NULL || pMember->MemberType == PT_Normal)
	//		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "[%s]NotOnAccountMoneyTick[%s]",
	//			pAccountAction->Amount.getString().c_str(), doubleToStringFormat(theAccount.Balance.getCompareTick()).c_str());
	//}

	if (!(pAccountAction->ActionType == AAT_SetReserveType || pAccountAction->ActionType == AAT_SetReserveType))
	{
		if (!pAccountAction->Amount.isTick(pCurrency->MoneyTick.getValue()))
		{
			if (pAccountAction->ActionType != AAT_Deposit)
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "[%s]NotOnCurrencyMoneyTick[%s]",
					pAccountAction->Amount.getString().c_str(), theAccount.getCurrency()->MoneyTick.getString().c_str());

			double smallTick = doubleCompareTick(pAccountAction->Amount.getValue(), pAccountAction->Amount.getPrecision());
			if (pCurrency->MoneyTick > smallTick)
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "[%s]NotOnCurrencyMoneyTick[%s]",
					pAccountAction->Amount.getString().c_str(), theAccount.getCurrency()->MoneyTick.getString().c_str());
			theCurrency.MoneyTick = smallTick;
		}
	}

	switch (pAccountAction->ActionType)
	{
	case AAT_Deposit://入金
	{
		if (pOrder != NULL)
		{
			CWriteableOrder theOrder;
			m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);
			theOrder.OrderStatus = OS_AllTraded;
			theOrder.VolumeTraded = theOrder.Volume;
			theOrder.VolumeRemain = 0.0;
			theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_OrderFactory->update(pOrder, &theOrder, pTransaction);
		}
		theAccount.Deposit += pAccountAction->Amount;
		theAccount.TotalDeposit += pAccountAction->Amount;
		if (pAccountAction->IsReserve)
		{
			theAccount.Reserve += pAccountAction->Amount;
			theAccount.ReserveActive = true;
		}

		if (!pAccountAction->Remark.isNull())
			theAccount.Remark = pAccountAction->Remark;

		//如果指定了合约号，说明需要增加持仓成本
		if (!pAccountAction->InstrumentID.isNull())
		{
			CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pAccountAction->ExchangeID, pAccountAction->InstrumentID);
			if (pInstrument == NULL || pInstrument->ProductClass != PC_AccountExchange || pInstrument->BaseCurrency != pAccountAction->Currency || pInstrument->SettlementGroup != pAccountAction->SettlementGroup)
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "InstrumentIDOrProductClassOrCurrencyOrSettlementGroupError");

			CWriteablePosition thePosition;
			thePosition.init(m_pMDB);
			CPosition* pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(pAccountAction->AccountID, pAccountAction->ExchangeID, pAccountAction->InstrumentID, pAccountAction->AccountID.getValue(), PD_Long);
			if (pPosition == NULL)
			{
				if (!InitPosition(m_pMDB, m_pErrorEngine, pInstrument, &thePosition, pAccountAction->MemberID, pAccountAction->MemberID.getValue(), pAccountAction->AccountID, PD_Long, INT_32_MAX, 1))
					return false;
			}
			else
				m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
			CVolumeType OpenVolume = pAccountAction->Amount / pInstrument->VolumeMultiple;
			if (!OpenVolume.isTick(pInstrument->VolumeTick))
				return m_pErrorEngine->reportError(ERROR_VOLUME_NOT_ON_TICK, "PositionOpen[%s],InstrumentVolumeTick[%s]", OpenVolume.getString().c_str(), pInstrument->VolumeTick.getString().c_str());
			thePosition.Position += OpenVolume;

			if (pAccountAction->Price.isNull())
			{
				if (pPosition == NULL || pPosition->Position == 0.0)// 全部出金的情况下，有持仓记录但持仓量为0
				{
					thePosition.OpenPrice = pInstrument->getMarketData()->LastPrice;
					thePosition.TotalPositionCost = thePosition.Position * thePosition.OpenPrice * pInstrument->VolumeMultiple;
				}
				else
				{
					if (pPosition->OpenPrice.isNull() || pPosition->OpenPrice == 0.0) // 有持仓，但OpenPrice为空或为0，需要取最新价
						thePosition.OpenPrice = pInstrument->getMarketData()->LastPrice;
					thePosition.TotalPositionCost = thePosition.Position * pPosition->OpenPrice * pInstrument->VolumeMultiple;
				}
			}
			else
			{
				thePosition.OpenPrice = pAccountAction->Price;
				thePosition.TotalPositionCost = thePosition.Position * thePosition.OpenPrice * pInstrument->VolumeMultiple;
			}

			thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			if (thePosition.Position > thePosition.HighestPosition)
				thePosition.HighestPosition = thePosition.Position;
			m_pMDB->m_PositionFactory->addOrUpdate(pPosition, &thePosition, pTransaction);
		}
		applyAccountChange(&theAccount);
		theCurrency.Deposit += pAccountAction->Amount;
		theCurrency.Deposit.toLowerTick(theCurrency.MoneyTick.getValue());
		break;
	}
	case AAT_Withdraw://出金，从可取扣除
	{
		if (!pAccountAction->InstrumentID.isNull())
		{
			CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pAccountAction->ExchangeID, pAccountAction->InstrumentID);
			if (pInstrument == NULL || pInstrument->ProductClass != PC_AccountExchange || pInstrument->BaseCurrency != pAccountAction->Currency || pInstrument->SettlementGroup != pAccountAction->SettlementGroup)
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "InstrumentIDOrProductClassOrCurrencyOrSettlementGroupError");

			CWriteablePosition thePosition;
			CPosition* pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(pAccountAction->AccountID, pAccountAction->ExchangeID, pAccountAction->InstrumentID,  pAccountAction->AccountID.getValue(), PD_Long);
			if (pPosition != NULL)
			{
				m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
				double TotalOldTradePrice = pPosition->TotalPositionCost / pPosition->Position / pInstrument->VolumeMultiple;
				CVolumeType CloseProfitPosition = min(pAccountAction->Amount.getValue(), pPosition->Position.getValue() / pInstrument->VolumeMultiple);
				if (!CloseProfitPosition.isTick(pInstrument->VolumeTick))
					return m_pErrorEngine->reportError(ERROR_VOLUME_NOT_ON_TICK, "PositionClose[%s],InstrumentVolumeTick[%s]", CloseProfitPosition.getString().c_str(), pInstrument->VolumeTick.getString().c_str());

				thePosition.Position -= CloseProfitPosition;
				thePosition.TotalPositionCost -= pInstrument->VolumeMultiple * CloseProfitPosition * TotalOldTradePrice;
				if (thePosition.Position == 0.0)
				{
					thePosition.TotalPositionCost = 0.0;
					thePosition.OpenPrice.clear();
					thePosition.TotalCloseProfit = 0.0;
					thePosition.PositionCost = 0.0;
					thePosition.CostPrice.clear();
				}
				thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
				m_pMDB->m_PositionFactory->addOrUpdate(pPosition, &thePosition, pTransaction);
			}
		}
		if (!AccountWithdraw(&theAccount, pAccountAction))
			return false;
		theCurrency.Withdraw += pAccountAction->Amount;
		theCurrency.Withdraw.toLowerTick(theCurrency.MoneyTick.getValue());
		break;
	}
	case AAT_FrozenMoneyByOrder:
	{
		if (!pAccountAction->LocalID.isNull())
		{
			COrder* pOrder = m_pMDB->m_OrderFactory->findByLocalID(pAccountAction->AccountID, pAccountAction->LocalID);
			if (pOrder != NULL)
				return m_pErrorEngine->reportError(ERROR_DUPLICATE_ACTION, "%s_%s", pAccountAction->MemberID.getValue(), pAccountAction->LocalID.getValue());
		}

		//增加订单，用于记录冻结的大小
		CWriteableOrder theOrder;
		theOrder.init(m_pMDB);
		theOrder.MemberID = pAccountAction->MemberID;
		theOrder.TradeUnitID = pAccountAction->AccountID;
		//		theOrder.UserID = pAccountAction->UserID;
		theOrder.LocalID = pAccountAction->LocalID;
		theOrder.DeriveSource = DS_FromAccountAction;
		theOrder.Direction = D_Sell;
		theOrder.Price = pAccountAction->Price;
		theOrder.Volume = pAccountAction->Volume;
		theOrder.VolumeRemain = pAccountAction->Volume;
		theOrder.Cost = pAccountAction->Amount;
		theOrder.ClearCurrency = pAccountAction->Currency;
		//		theOrder.ProductGroup = theAccount.ProductGroup;
		theOrder.SettlementGroup = pAccountAction->SettlementGroup;
		theOrder.InstrumentID = pAccountAction->InstrumentID;
		theOrder.ExchangeID = pAccountAction->ExchangeID;
		theOrder.AccountID = pAccountAction->AccountID;
		theOrder.OrderID = m_gOrderSequence.getNextValueByString();
		theOrder.FrozenMoney = pAccountAction->Amount;
		theOrder.Remark = pAccountAction->Remark;
		pAccountAction->OrderID = theOrder.OrderID;
		m_pMDB->m_OrderFactory->add(&theOrder, pTransaction);
	}
	case AAT_FrozenMoney://冻结,从可用中冻结，可取可能为负
	{
		//增加冻结资金
		theAccount.FrozenMoney += pAccountAction->Amount;
		theAccount.Available -= pAccountAction->Amount;
		theAccount.Withdrawable -= pAccountAction->Amount;

		//冻结后可用不能小于0
		if (pAccountAction->IsReserve)
		{
			if (theAccount.Reserve < pAccountAction->Amount)
				return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "Reserve(%s)<(%s)", theAccount.Reserve.getString().c_str(), pAccountAction->Amount.getString().c_str());
		}
		else
		{
			double PositionProfit = getPositionProfit(m_pMDB, &theAccount);
			if (PositionProfit > 0.0)
				PositionProfit = 0.0;
			if (theAccount.Withdrawable < -PositionProfit)
				return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "Withdrawable(%s)<-PositionProfit(%0.f)", theAccount.Withdrawable.getString().c_str(), PositionProfit);
		}
		break;
	}
	case AAT_FreeFrozenMoney://解除冻结
	{
		if (pOrder != NULL)
		{
			CWriteableOrder theOrder;
			m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);
			theOrder.OrderStatus = OS_AllCanceled;
			theOrder.VolumeTraded = 0.0;
			theOrder.VolumeRemain = 0.0;
			pAccountAction->Amount = theOrder.FrozenMoney;
			theOrder.FrozenMoney = 0.0;
			theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_OrderFactory->update(pOrder, &theOrder, pTransaction);
		}

		//解冻资金不能大于冻结资金
		if (pAccountAction->Amount > theAccount.FrozenMoney)
			//REPORT_EVENT(LOG_CRITICAL, "Account", "[%s]:Amount[%0.f]>FrozenMoney[%0.f]", theAccount.AccountID.getValue(), pAccountAction->Amount.getValue(), theAccount.FrozenMoney.getValue());
			return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY);

		theAccount.FrozenMoney -= pAccountAction->Amount;
		theAccount.Withdrawable += pAccountAction->Amount;
		theAccount.Available += pAccountAction->Amount;
		break;
	}
	case AAT_FrozenToWithdraw:	//将冻结转出
	{
		if (pOrder != NULL)
		{
			CWriteableOrder theOrder;
			m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);
			theOrder.OrderStatus = OS_AllTraded;
			theOrder.VolumeTraded = theOrder.Volume;
			theOrder.VolumeRemain = 0.0;
			pAccountAction->FrozenMoney = pOrder->FrozenMoney;
			theOrder.FrozenMoney = 0.0;
			theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_OrderFactory->update(pOrder, &theOrder, pTransaction);
		}

		if (!pAccountAction->InstrumentID.isNull())
		{
			CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pAccountAction->ExchangeID, pAccountAction->InstrumentID);
			if (pInstrument == NULL || pInstrument->ProductClass != PC_AccountExchange || pInstrument->BaseCurrency != pAccountAction->Currency || pInstrument->SettlementGroup != pAccountAction->SettlementGroup)
				return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "InstrumentIDOrProductClassOrCurrencyOrSettlementGroupError");

			CWriteablePosition thePosition;
			CPosition* pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(pAccountAction->AccountID, pAccountAction->ExchangeID, pAccountAction->InstrumentID, pAccountAction->AccountID.getValue(), PD_Long);
			if (pPosition != NULL)
			{
				m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
				double TotalOldTradePrice = pPosition->TotalPositionCost / pPosition->Position;
				CVolumeType CloseProfitPosition = min(pAccountAction->Amount.getValue(), pPosition->Position.getValue() / pInstrument->VolumeMultiple);
				if (!CloseProfitPosition.isTick(pInstrument->VolumeTick))
					return m_pErrorEngine->reportError(ERROR_VOLUME_NOT_ON_TICK, "PositionClose[%s],InstrumentVolumeTick[%s]", CloseProfitPosition.getString().c_str(), pInstrument->VolumeTick.getString().c_str());
				thePosition.Position -= CloseProfitPosition;
				//平仓盈亏
				theAccount.TotalCloseProfit += CloseProfitPosition * (pInstrument->getMarketData()->LastPrice.getValue() - TotalOldTradePrice);
				thePosition.TotalPositionCost -= pInstrument->VolumeMultiple * CloseProfitPosition * TotalOldTradePrice;
				if (thePosition.Position == 0.0)
				{
					thePosition.Position = 0.0;
					thePosition.TotalPositionCost = 0.0;
					thePosition.OpenPrice.clear();
					thePosition.TotalCloseProfit = 0.0;
					thePosition.PositionCost = 0.0;
					thePosition.CostPrice.clear();
				}
				thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
				m_pMDB->m_PositionFactory->addOrUpdate(pPosition, &thePosition, pTransaction);
			}
		}

		//判断解冻资金是否大于冻结资金
		//if (pAccountAction->Amount + theAccount.Reserve > theAccount.FrozenMoney)
		//	//REPORT_EVENT(LOG_CRITICAL, "Account", "[%s]:Amount[%0.f]>FrozenMoney[%0.f]", theAccount.AccountID.getValue(), pAccountAction->Amount.getValue(), theAccount.FrozenMoney.getValue());
		//	return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY);

		//theAccount.FrozenMoney -= pAccountAction->Amount + theAccount.Reserve;
		//theAccount.Withdraw += pAccountAction->Amount + theAccount.Reserve;
		//theAccount.TotalWithdraw += pAccountAction->Amount + theAccount.Reserve;
		//theAccount.Reserve = 0.0;

		if (pAccountAction->Amount > theAccount.FrozenMoney)
			//REPORT_EVENT(LOG_CRITICAL, "Account", "[%s]:Amount[%0.f]>FrozenMoney[%0.f]", theAccount.AccountID.getValue(), pAccountAction->Amount.getValue(), theAccount.FrozenMoney.getValue());
			return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY,"Amount>FrozenMoney");

		if (pAccountAction->FrozenMoney > theAccount.FrozenMoney)
			//REPORT_EVENT(LOG_CRITICAL, "Account", "[%s]:Amount[%0.f]>FrozenMoney[%0.f]", theAccount.AccountID.getValue(), pAccountAction->Amount.getValue(), theAccount.FrozenMoney.getValue());
			return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY);

		theAccount.FrozenMoney -= pAccountAction->FrozenMoney;
		theAccount.Withdraw += pAccountAction->Amount;
		theAccount.TotalWithdraw += pAccountAction->Amount;

		if (pAccountAction->IsReserve)
		{
			theAccount.Reserve -= pAccountAction->Amount;
			if (pAccount->Reserve < 0.0)
			{
				REPORT_EVENT(LOG_CRITICAL, "Account", "[%s] Reserve[%0.f] < 0 ", pAccount->AccountID.getValue(), pAccount->Reserve.getValue());
				m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY);
				return false;
			}
		}

		//		if (pAccountAction->IsReserve)
		//		{
		//			theAccount.Reserve -= pAccountAction->Amount;
		//			if (theAccount.Reserve < 0.0)
		//			{
		//				REPORT_EVENT(LOG_CRITICAL, "Account", "[%s] Reserve[%0.f] < 0 ", pAccount->AccountID.getValue(), pAccount->Reserve.getValue());
		//				m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY);
		//				return false;
		//			}
					//			theAccount.ReserveLimit -= pAccountAction->Amount;
		//			applyAccountChange(&theAccount);
		//		}
		//		else
		//		{
					//			theAccount.ReserveActive = false;
		applyAccountChange(&theAccount);
		//		}
		theCurrency.Withdraw += pAccountAction->Amount;
		theCurrency.Withdraw.toLowerTick(theCurrency.MoneyTick.getValue());
		break;
	}
	case AAT_InnerTransfer:
	{
		if (!AccountWithdraw(&theAccount, pAccountAction))
			return false;

		theCurrency.Withdraw += pAccountAction->Amount;
		theCurrency.Withdraw.toLowerTick(theCurrency.MoneyTick.getValue());

		//另外的账户入金
		CAccountActionField otherAccountAction;
		//		otherAccountAction.UserID = pAccountAction->OtherAccountID;
		otherAccountAction.LocalID.clear();
		otherAccountAction.MemberID = pAccountAction->OtherMemberID;
		otherAccountAction.AccountID = pAccountAction->OtherAccountID;
		otherAccountAction.Currency = pAccountAction->Currency;
		otherAccountAction.SettlementGroup = pAccountAction->OtherSettlementGroup;
		otherAccountAction.Amount = pAccountAction->Amount;
		otherAccountAction.IsReserve = pAccountAction->IsReserve;
		otherAccountAction.ActionType = AAT_Deposit;
		otherAccountAction.Remark = pAccountAction->Remark;
		//内部转账，把Source/RelatedID透传到otherAccountAction中，注意ActionType有变化
		otherAccountAction.Source = pAccountAction->Source;
		otherAccountAction.RelatedID = pAccountAction->RelatedID;
		if (!ReqAccountAction(&otherAccountAction, NULL, pTransaction))
			return false;
		break;
	}
	//case AAT_CutReserveProfit:
	//{
	//	if (pAccountAction->Amount > theAccount.ReserveProfit)
	//	{
	//		//REPORT_EVENT(LOG_CRITICAL, "Account", "[%s]:Amount[%0.f]>ReserveProfit[%0.f]", theAccount.AccountID.getValue(), pAccountAction->Amount.getValue(), theAccount.ReserveProfit.getValue());
	//		m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY);
	//		return false;
	//	}

	//	theAccount.Withdraw += pAccountAction->Amount;
	//	theAccount.ReserveProfit -= pAccountAction->Amount;
	//	applyAccountChange(&theAccount);

	//	//另外的账户入金
	//	CWriteableAccountAction otherAccountAction;
	//	otherAccountAction.init();
	//	otherAccountAction.UserID = pAccountAction->OtherAccountID;
	//	otherAccountAction.LocalID.clear();
	//	otherAccountAction.AccountID = pAccountAction->OtherAccountID;
	//	otherAccountAction.Currency = pAccountAction->Currency;
	//	otherAccountAction.SettlementGroup = pAccountAction->OtherSettlementGroup;
	//	otherAccountAction.Amount = pAccountAction->Amount;
	//	otherAccountAction.IsReserve = false;
	//	otherAccountAction.ActionType = AAT_Deposit;
	//	otherAccountAction.Remark = pAccountAction->Remark;
	//	ReqAccountAction(&otherAccountAction, NULL, pSession, pTransaction);
	//	break;
	//}
	case AAT_SetReserveType:
	{
		theAccount.ReserveType = '0' + (int)pAccountAction->Amount.getValue();
		break;
	}
	case AAT_SetReserveActive:
	{
		theAccount.ReserveActive = pAccountAction->Amount;
		applyAccountChange(&theAccount);
		break;
	}
	case AAT_SetReserveLimit:
	{
		theAccount.ReserveLimit = pAccountAction->Amount;
		applyAccountChange(&theAccount);
		break;
	}
	//case AAT_ReserveLoss:
	//{
	//	//体验金过期需要条件控制，有保证金和冻结保证金的情况下，不能过期
	//	if (theAccount.UseMargin > 0.0 || theAccount.FrozenMargin > 0.0)
	//		return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY);

	//	//theAccount.ReserveActive = (int)pAccountAction->Amount.getValue();
	//	//删除体验金
	//	pAccountAction->Amount = 0;
	//	pAccountAction->IsReserve = false;
	//	if (!AccountWithdraw(&theAccount, pAccountAction))
	//		return false;
	//	//applyAccountChange(&theAccount);
	//	theCurrency.Withdraw += pAccountAction->Amount;
	//	break;
	//}
	//case AAT_CutReserveProfit:
	//case AAT_LoanLend:
	//{
	//	theAccount.FrozenMoney += pAccountAction->Amount;
	//	theAccount.Available -= pAccountAction->Amount;
	//	theAccount.Withdrawable -= pAccountAction->Amount;
	//	double PositionProfit = getPositionProfit(m_pMDB, &theAccount);
	//	if (PositionProfit > 0.0)
	//		PositionProfit = 0.0;
	//	if (theAccount.Withdrawable < -PositionProfit)
	//		return m_pErrorEngine->reportError(ERROR_INSUFFICIENT_MONEY, "Withdrawable(%s)<-PositionProfit(%0.f)", theAccount.Withdrawable.getString().c_str(), PositionProfit);

	//	CPosition *pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(theAccount.AccountID, theAccount.SettlementGroup.getValue(),
	//		theAccount.Currency.getValue(), theAccount.AccountID.getValue(), PD_Long);
	//	CWriteablePosition thePosition;
	//	if (pPosition == NULL)
	//	{
	//		thePosition.init();
	//		thePosition.PositionClass = PC_Loan;
	//		thePosition.Position = pAccountAction->Amount;
	//		thePosition.PositionID = m_gOrderSequence.getNextValueByString();
	//		thePosition.InstrumentID = theAccount.Currency;
	//		thePosition.ExchangeID = theAccount.SettlementGroup;
	//		thePosition.SettlementGroup = theAccount.SettlementGroup;
	//		thePosition.MemberID = theAccount.AccountID;
	//		thePosition.PosiDirection = PD_Long;
	//		thePosition.ClearCurrency = theAccount.Currency;
	//		thePosition.AccountID = theAccount.AccountID;
	//		thePosition.TradeUnitID = theAccount.AccountID.getValue();
	//		thePosition.linkAccount(pAccount);
	//		m_pMDB->m_PositionFactory->add(&thePosition, pTransaction);
	//	}
	//	else
	//	{
	//		m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
	//		thePosition.Position += pAccountAction->Amount;
	//		m_pMDB->m_PositionFactory->update(pPosition, &thePosition, pTransaction);
	//	}
	//	theCurrency.LoanTotal += thePosition.Position;
	//	break;
	//}
	//case AAT_LoanBack:
	//{
	//	theAccount.FrozenMoney -= pAccountAction->Amount;
	//	theAccount.Available += pAccountAction->Amount;
	//	theAccount.Withdrawable += pAccountAction->Amount;
	//	CPosition* pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(theAccount.AccountID, theAccount.SettlementGroup.getValue(),
	//		theAccount.Currency.getValue(), theAccount.AccountID.getValue(), PD_Long);
	//	if (pPosition == NULL)
	//		return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND, "PositionNotFound");
	//	CWriteablePosition thePosition;
	//	m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
	//	thePosition.Position -= pAccountAction->Amount;
	//	if (thePosition.Position == 0.0)
	//		m_pMDB->m_PositionFactory->remove(pPosition, pTransaction);
	//	else
	//		m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
	//	theCurrency.LoanTotal -= thePosition.Position;
	//	break;
	//}
	default:
	{
		return m_pErrorEngine->reportError(ERROR_BAD_FIELD, "ActionType");
	}
	}

	double PreBalance = 0.0;
	if (pAccount != NULL)
		PreBalance = pAccount->Balance;

	if (theAccount.Balance != PreBalance)
	{
		//出入金成功,写入出入金流水表
		CWriteableAccountDetail theAccountDetail;
		theAccountDetail.init();
		theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
		theAccountDetail.AccountID = theAccount.AccountID;
		theAccountDetail.SettlementGroup = theAccount.SettlementGroup;
		theAccountDetail.Currency = theAccount.Currency;
		theAccountDetail.MemberID = theAccount.MemberID;
		theAccountDetail.SettleSegment = theAccount.getCurrency()->SettleSegment;
		//theAccountDetail.TradeUnitID.clear();
		theAccountDetail.ExchangeID.clear();
		theAccountDetail.InstrumentID.clear();
		//theAccountDetail.ProductGroup = theAccount.ProductGroup;
		theAccountDetail.Remark = pAccountAction->Remark;
		theAccountDetail.PreBalance = PreBalance;
		if (pAccountAction->LocalID.isNull() && !pAccountAction->ActionLocalID.isNull())
			theAccountDetail.LocalID = pAccountAction->ActionLocalID;
		else
			theAccountDetail.LocalID = pAccountAction->LocalID;

		if (pAccountAction->ActionType == AAT_Deposit)
		{
			theAccountDetail.Amount = pAccountAction->Amount;
			if (pAccountAction->IsReserve)
			{
				theAccountDetail.Source = ACT_ReserveDeposit;
				theAccountDetail.ReserveAmount = theAccountDetail.Amount;
			}
			else
			{
				theAccountDetail.Source = ACT_Deposit;
			}
		}
		else
		{
			theAccountDetail.Amount = -pAccountAction->Amount;
			if (pAccountAction->IsReserve)
			{
				theAccountDetail.Source = ACT_ReserveWithdraw;
				theAccountDetail.ReserveAmount = theAccountDetail.Amount;
			}
			else
			{
				theAccountDetail.Source = ACT_Withdraw;
			}
		}

		theAccountDetail.ReserveBalance = theAccount.Reserve;
		//无论出入金，如果传入Source,以传入的Source为准
		if (!pAccountAction->Source.isNull())
			theAccountDetail.Source = pAccountAction->Source;

		if (!pAccountAction->RelatedID.isNull())
			theAccountDetail.RelatedID = pAccountAction->RelatedID;

		theAccountDetail.Balance = theAccount.Balance;
		//theAccountDetail.Balance = theAccountDetail.PreBalance + theAccountDetail.Amount;
		//if (theAccountDetail.Amount != 0.0)
		m_pMDB->m_AccountDetailFactory->add(&theAccountDetail, pTransaction);

		//double Reserve = 0.0;
		//if (pAccount != NULL)
		//	Reserve = pAccount->Reserve;

		//判断是否是出金了体验金
		//if (theAccount.Reserve != Reserve && (!pAccountAction->IsReserve))
		//{
		//	theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
		//	theAccountDetail.PreBalance = theAccountDetail.Balance;
		//	theAccountDetail.Balance = theAccount.Balance;
		//	theAccountDetail.Source = ACT_ReserveWithdraw;
		//	if (theAccountDetail.Amount != 0.0)
		//		theAccountDetail.Remark = "move";
		//	if (pAccount == NULL)
		//		return m_pErrorEngine->reportError(ERROR_AccountNotFound);

		//	theAccountDetail.Amount = -pAccount->Reserve;
		//	m_pMDB->m_AccountDetailFactory->add(&theAccountDetail, pTransaction);
		//}
	}

	theAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_AccountFactory->addOrUpdate(pAccount, &theAccount, pTransaction);

	theCurrency.Balance = theCurrency.PreBalance + theCurrency.Deposit - theCurrency.Withdraw;
	m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency, pTransaction);

	if (pOutAccount != NULL)
		CopyAccountEntity(pOutAccount, &theAccount);
	return true;
}

bool CSettlementService::FundingRate(CWriteableSettleDetail* pSettlementDetail)
{
	CInstrument* pInstrumentAll = m_pMDB->m_InstrumentFactory->getFirst();
	for (; pInstrumentAll != NULL; pInstrumentAll = m_pMDB->m_InstrumentFactory->getNext())
	{
		if (pInstrumentAll->ProductType != PT_Swap)
			continue;

		if (pSettlementDetail->FundingRateGroup != pInstrumentAll->FundingRateGroup)
			continue;

		if (pInstrumentAll->getMarketData()->PrePositionFeeRate.isNull())
		{
			if (pInstrumentAll->getMarketData()->PositionFeeRate.isNull())
				continue;

			//如果资金费用更没有确定，就用新的资金费用代替，同时新资金费用归零
			CWriteableMarketData theMarketData;
			m_pMDB->m_MarketDataFactory->retrieve(pInstrumentAll->getMarketData(), &theMarketData);
			theMarketData.PrePositionFeeRate = theMarketData.PositionFeeRate;
			theMarketData.PositionFeeRate = 0.0;
			//得到下一次资金结算的费用的时间
			if (!pSettlementDetail->Value2.isNull())
			{
				theMarketData.PositionFeeTime = pSettlementDetail->Value2.getValue();
			}
			else
			{
				if (!pSettlementDetail->Value3.isNull())
					theMarketData.PositionFeeTime = g_nCurrentTime + pSettlementDetail->Value3.getValue();
			}
			m_pMDB->m_MarketDataFactory->update(pInstrumentAll->getMarketData(), &theMarketData);
		}
		if (pInstrumentAll->getMarketData()->MarkedPrice.isNull())
			continue;

		if (pInstrumentAll->IsInverse)
		{
			if (pInstrumentAll->getMarketData()->MarkedPrice == 0.0)
				continue;
		}

		CWriteableSettleDetail theSettlementDetail;
		CopySettleDetailEntity(&theSettlementDetail, pSettlementDetail);
		theSettlementDetail.ExchangeID = pInstrumentAll->ExchangeID;
		theSettlementDetail.InstrumentID = pInstrumentAll->InstrumentID;
		theSettlementDetail.SettlementGroup = pInstrumentAll->SettlementGroup;
		//theSettlementDetail.ProductGroup = pInstrumentAll->ProductGroup;
		theSettlementDetail.Value = pInstrumentAll->getMarketData()->PrePositionFeeRate.getValue();
		theSettlementDetail.Value1 = pInstrumentAll->getMarketData()->MarkedPrice.getValue();
		theSettlementDetail.Value2 = pSettlementDetail->Value2;
		theSettlementDetail.Value3 = pSettlementDetail->Value3;
		m_pMDB->m_SettleDetailFactory->add(&theSettlementDetail);
	}

	//做一个循环，完成资金和持仓的转换
	CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		if (pPosition->Position == 0)
		{
			if (pPosition->LongFrozen == 0 && pPosition->ShortFrozen == 0 && pPosition->CloseOrderID.isNull())
			{
				if (pPosition->TradeUnitID.like(pPosition->AccountID))
				{
					if (pPosition->HighestPosition > 0 && pPosition->UpdateTime > g_nCurrentTime - 7 * 24 * 60 * 60 * 1000)
					{
						pPosition = m_pMDB->m_PositionFactory->getNext();
						continue;
					}
					bool bDelete = true;
					CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findFirstByAccountID(pPosition->AccountID);
					while (pTriggerOrder != NULL)
					{
						if (pTriggerOrder->InstrumentID == pPosition->InstrumentID && pTriggerOrder->ExchangeID == pPosition->ExchangeID && pTriggerOrder->AccountID == pPosition->AccountID)
						{
							bDelete = false;
							break;
						}
						pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findNextByAccountID();
					}
					if (!bDelete)
					{
						pPosition = m_pMDB->m_PositionFactory->getNext();
						continue;
					}

					CPosition* pOtherPosition = m_pMDB->m_PositionFactory->findFirstByAccountInstrument(pPosition->AccountID, pPosition->ExchangeID, pPosition->InstrumentID);

					while (pOtherPosition != NULL)
					{
						if (!(pOtherPosition->Position == 0 && pOtherPosition->LongFrozen == 0 && pOtherPosition->ShortFrozen == 0 && pOtherPosition->CloseOrderID.isNull()
							&& pOtherPosition->UpdateTime < g_nCurrentTime - 30 * 24 * 60 * 60 * 1000))
						{
							bDelete = false;
							break;
						}
						pOtherPosition = m_pMDB->m_PositionFactory->findNextByAccountInstrument();
					}
					if (!bDelete)
					{
						pPosition = m_pMDB->m_PositionFactory->getNext();
						continue;
					}
				}
				CPosition* pPositionToRemove = pPosition;
				pPosition = m_pMDB->m_PositionFactory->getNext();
				m_pMDB->m_PositionFactory->remove(pPositionToRemove);
				continue;
			}
			else
			{
				pPosition = m_pMDB->m_PositionFactory->getNext();
				continue;
			}
		}

		CSettleDetail* pSettleDetail = m_pMDB->m_SettleDetailFactory->findByInstrument(pSettlementDetail->SettleDetailID, pPosition->ExchangeID, pPosition->InstrumentID);
		if (pSettleDetail == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		CInstrument* pInstrument = pPosition->getInstrument();
		if (pInstrument == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		CAccount* pAccount = pPosition->getAccount();
		if (pAccount == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		CMoneyType thisPremium;
		if (pInstrument->IsInverse)
			thisPremium = abs(pPosition->Position) * pInstrument->VolumeMultiple * pSettleDetail->Value.getValue() / pInstrument->getMarketData()->MarkedPrice;
		else
			thisPremium = abs(pPosition->Position) * pInstrument->VolumeMultiple * pInstrument->getMarketData()->MarkedPrice * pSettleDetail->Value.getValue();

		// 对资金费用进行tick值处理
		CCurrency *pCurrency = pAccount->getCurrency();
		if (pCurrency == NULL)
		{
			REPORT_EVENT(LOG_CRITICAL, "FundingRate", "AccountID:%s, SettlementGroup:%s, pCurrency:%s not found", pAccount->AccountID.getValue(), pAccount->SettlementGroup.getValue(), pAccount->Currency.getValue());
			continue;
		}
		thisPremium.toLowerTick(pCurrency->MoneyTick.getValue());

		CWriteableAccount theWriteableAccount;
		m_pMDB->m_AccountFactory->retrieve(pAccount, &theWriteableAccount);
		CMoneyType changePremium = 0.0;
		switch (pPosition->PosiDirection)
		{
		case PD_Long:
		{
			changePremium = -thisPremium;
			break;
		}
		case PD_Short:
		{
			changePremium = thisPremium;
			break;
		}
		case PD_Net:
		{
			if (pPosition->Position > 0)
				changePremium = -thisPremium;
			else
				changePremium = thisPremium;
			break;
		}
		}
		theWriteableAccount.MoneyChange += changePremium;

		//追仓模式先扣除资金，如果可用资金不够需要扣除逐仓保证金
		if (!pPosition->IsCrossMargin)
		{
			CWriteablePosition theWriteablePosition;
			m_pMDB->m_PositionFactory->retrieve(pPosition, &theWriteablePosition);
			theWriteablePosition.UseMargin += changePremium;
			if (theWriteablePosition.UseMargin < 0.0)
			{
				theWriteablePosition.UseMargin = 0.0;
				theWriteableAccount.UseMargin -= pPosition->UseMargin;
			}
			else
			{
				theWriteableAccount.UseMargin += changePremium;
			}
			m_pMDB->m_PositionFactory->update(pPosition, &theWriteablePosition);
		}

		applyAccountChange(&theWriteableAccount);
		applyReserveChange(pAccount, &theWriteableAccount, changePremium);

		//增加资金明细
		CWriteableAccountDetail theAccountDetail;
		theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
		theAccountDetail.AccountID = theWriteableAccount.AccountID;
		theAccountDetail.SettlementGroup = theWriteableAccount.SettlementGroup;
		theAccountDetail.Currency = theWriteableAccount.Currency;
		theAccountDetail.SettleSegment = theWriteableAccount.getCurrency()->SettleSegment;
		theAccountDetail.MemberID = pPosition->MemberID;
		//		theAccountDetail.TradeUnitID = pPosition->TradeUnitID;
		theAccountDetail.ExchangeID = pPosition->ExchangeID;
		theAccountDetail.InstrumentID = pPosition->InstrumentID;
		//theAccountDetail.ProductGroup = pPosition->ProductGroup;
		theAccountDetail.Amount = changePremium;
		theAccountDetail.PreBalance = pAccount->Balance;
		theAccountDetail.Balance = theWriteableAccount.Balance;
		//theAccountDetail.Amount.cutByBigFloat(theAccountDetail.Balance.getValue());
		theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
		theAccountDetail.Source = ACT_FundingRate;
		theAccountDetail.ReserveBalance = theWriteableAccount.Reserve;
		theAccountDetail.LocalID = pSettlementDetail->LocalID;

		if (changePremium < 0.0)
		{
			CMoneyType ReserveLoss = pAccount->Reserve - theWriteableAccount.Reserve;
			if (ReserveLoss >= -changePremium)
				ReserveLoss = -changePremium;
			//sprintf(theAccountDetail.Remark.getBuffer(), "ReserveProfit:-%s", ReserveLoss.getString().c_str());	
			if (ReserveLoss > 0.0)
			{
				theAccountDetail.ReserveAmount = -ReserveLoss;
				theAccountDetail.ReserveBalance = theWriteableAccount.Reserve;
			}
		}

		m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);
		theWriteableAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_AccountFactory->update(pAccount, &theWriteableAccount);

		pPosition = m_pMDB->m_PositionFactory->getNext();
	}
	m_pMDB->m_SettleDetailFactory->clearAll();
	return true;
}

double CSettlementService::Fee(double dFee, CAccountIDType AccountID, CShortIndexIDType SettlementGroup, CCurrencyType Currency, CDefineTypeType Source, CLongNameType Remark)
{
	CWriteableAccount theAccount;
	CAccount* pFeeAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(AccountID.getValue(), AccountID, SettlementGroup, Currency);
	if (pFeeAccount == NULL)
	{
		theAccount.init(m_pMDB);
		theAccount.CanReduce = true;
		theAccount.AccountID = AccountID;
		theAccount.SettlementGroup = SettlementGroup;
		theAccount.Currency = Currency;
		theAccount.MemberID = AccountID;
		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(AccountID.getValue());
		if (pMember == NULL)
		{
			CWriteableMember theMember;
			theMember.init();
			theMember.MemberID = AccountID;
			theMember.MemberType = PT_Fee;
			m_pMDB->m_MemberFactory->add(&theMember);
		}
	}
	else
		m_pMDB->m_AccountFactory->retrieve(pFeeAccount, &theAccount);

	CMoneyType Fee = dFee;
	Fee.toLowerTick(theAccount.getCurrency()->MoneyTick.getValue());
	theAccount.PreBalance = theAccount.Balance + Fee;
	applyAccountChange(&theAccount);
	theAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_AccountFactory->addOrUpdate(pFeeAccount, &theAccount);

	CWriteableAccountDetail theAccountDetail;
	theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
	theAccountDetail.AccountID = theAccount.AccountID;
	theAccountDetail.MemberID = theAccount.MemberID;
	theAccountDetail.SettlementGroup = theAccount.SettlementGroup;
	theAccountDetail.Currency = theAccount.Currency;
	theAccountDetail.SettleSegment = theAccount.getCurrency()->SettleSegment;
	//theAccountDetail.ProductGroup = theAccount.ProductGroup;
	theAccountDetail.Amount = Fee;
	if (pFeeAccount == NULL)
		theAccountDetail.PreBalance = 0.0;
	else
		theAccountDetail.PreBalance = pFeeAccount->Balance;
	theAccountDetail.Balance = theAccount.Balance;
	//theAccountDetail.Amount.cutByBigFloat(theAccountDetail.Balance.getValue());
	theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
	theAccountDetail.Source = Source;
	theAccountDetail.Remark = Remark;
	theAccountDetail.ReserveBalance = theAccount.Reserve;
	theAccountDetail.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();

	m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);
	return theAccount.Balance.getValue();
}

void CSettlementService::AccountCollect(map<string, double> Map, CAccountIDType AccountID, CShortIndexIDType SettlementGroup, map<string, double> BanlanceMap, CDefineTypeType Source, CLongNameType Remark)
{
	map<string, double>::iterator iter = Map.begin();
	while (iter != Map.end())
	{
		CCurrencyType Currency = iter->first.c_str();
		double Balance = Fee(iter->second, AccountID, SettlementGroup, Currency, Source, Remark);
		map<string, double>::iterator iterBanlance = BanlanceMap.find(Currency.getValue());
		if (iterBanlance == BanlanceMap.end())
			BanlanceMap.insert(map<string, double>::value_type(Currency.getValue(), Balance));
		else
			iterBanlance->second += Balance;
		iter++;
	}
}


bool CSettlementService::Settlement(CWriteableSettleDetail* pSettlementField, UF_INT8 nSequenceNo)
{
	switch (pSettlementField->SettleAction)
	{
	case SAT_SettlementBegin:
	{
		if (pSettlementField->SettlementGroup.isNull())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "SettlementGroupIsNull");

		if (!m_pMDB->canSnap())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "MDBCanNotSnap");

		string sSettleSegment;
		CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findFirstBySettlementGroup(pSettlementField->SettlementGroup);
		while (pCurrency != NULL)
		{
			sSettleSegment = pCurrency->SettleSegment.getValue();
			CWriteableCurrency theCurrency;
			m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
			theCurrency.IsTrading = false;
			theCurrency.CanReduce = false;
			m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
			pCurrency = m_pMDB->m_CurrencyFactory->findNextBySettlementGroup();
		}
		//		char dumpName[256];
		//		sprintf(dumpName, "settle_%s_%s_1", pSettlementField->SettlementGroup.getValue(), sSettleSegment.c_str());
		//		snapMDB(nSequenceNo, dumpName);
		break;
	}
	case SAT_Settlement:
	{
		//char szPath[256];
		//sprintf(szPath, "%s/snap.%.0f.settle_%s.1/", sSnapPath.c_str(), nSequenceNo, pSettlementField->SettlementGroup.getValue());
		//m_pMDB->save(szPath, nSequenceNo);

		if (pSettlementField->SettlementGroup.isNull())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "SettlementGroupIsNull");

		Settlement(pSettlementField);

		//如果是0，说明一个合约都没有结算，这个不能算成功
		//if (pSettlementField->SettleNo == 0)
		//	return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
		//break; 如果是根据SettlementGroup做结算，后续还需要做初始化动作

		//char szPath[256];
		//sprintf(szPath, "%s/snap.%.0f.settle_%s.2/", sSnapPath.c_str(), nSequenceNo, pSettlementField->SettlementGroup.getValue());
		//m_pMDB->save(szPath, nSequenceNo);
	}
	case SAT_AccountInit:
	{
		//结算带着做掉，这里就只能做SettlementGroup的操作，不能返回错误，要不然就失败了
		if (!pSettlementField->InstrumentID.isNull())
			return true;

		//char szPath[256];
		//sprintf(szPath, "%s/snap.%.0f.init_%s.1/", sSnapPath.c_str(), nSequenceNo, pSettlementField->SettlementGroup.getValue());
		//m_pMDB->save(szPath, nSequenceNo);

		map<string, double> FeeMap;
		map<string, double> CopyProfitMap;
		//CIndexIDType ProductGroup = "";
		CAccountIDType FeeAccount = "FeeAccount";
		CServiceConfig* pServiceConfig = m_pMDB->m_ServiceConfigFactory->findByConfig("FeeAccount", "default", "default");
		if (pServiceConfig != NULL)
			FeeAccount = pServiceConfig->ConfigValue.getValue();
		CAccountIDType CopyProfitAccount = "CopyProfitAccount";
		pServiceConfig = m_pMDB->m_ServiceConfigFactory->findByConfig("CopyProfitAccount", "default", "default");
		if (pServiceConfig != NULL)
			CopyProfitAccount = pServiceConfig->ConfigValue.getValue();

		map<string, double> BanlanceMap;
		map<string, double> PreBanlanceMap;
		CAccount* pAccount = m_pMDB->m_AccountFactory->getFirst();
		while (pAccount != NULL)
		{
			if (pAccount->SettlementGroup != pSettlementField->SettlementGroup)
			{
				pAccount = m_pMDB->m_AccountFactory->getNext();
				continue;
			}

			//计算Account的原始总额信息，在修改之前做，要不然被改变之后值就不对了
			map<string, double>::iterator iterPreBanlance = PreBanlanceMap.find(pAccount->Currency.getValue());
			if (iterPreBanlance == PreBanlanceMap.end())
				PreBanlanceMap.insert(map<string, double>::value_type(pAccount->Currency.getValue(), pAccount->PreBalance + pAccount->Deposit - pAccount->Withdraw));
			else
				iterPreBanlance->second += pAccount->PreBalance + pAccount->Deposit - pAccount->Withdraw;

			if (pAccount->Fee != 0.0)
			{
				map<string, double>::iterator iter = FeeMap.find(pAccount->Currency.getValue());
				if (iter == FeeMap.end())
					FeeMap.insert(map<string, double>::value_type(pAccount->Currency.getValue(), pAccount->Fee.getValue()));
				else
					iter->second += pAccount->Fee.getValue();
			}

			if (pAccount->CopyProfit != 0.0)
			{
				map<string, double>::iterator iter = CopyProfitMap.find(pAccount->Currency.getValue());
				if (iter == CopyProfitMap.end())
					CopyProfitMap.insert(map<string, double>::value_type(pAccount->Currency.getValue(), pAccount->CopyProfit.getValue()));
				else
					iter->second += pAccount->CopyProfit.getValue();
			}

			AccountInit(pAccount);

			//if (ProductGroup.isNull())
			//	ProductGroup = pAccount->ProductGroup;

			//计算Account的现在的总额信息
			if (pAccount->AccountID != FeeAccount && pAccount->AccountID != CopyProfitAccount)
			{
				map<string, double>::iterator iterBanlance = BanlanceMap.find(pAccount->Currency.getValue());
				if (iterBanlance == BanlanceMap.end())
					BanlanceMap.insert(map<string, double>::value_type(pAccount->Currency.getValue(), pAccount->Balance));
				else
					iterBanlance->second += pAccount->Balance;
			}

			pAccount = m_pMDB->m_AccountFactory->getNext();
		}

		//归拢手续费
		AccountCollect(FeeMap, FeeAccount, pSettlementField->SettlementGroup, BanlanceMap, ACT_Fee, "Fee");
		//归拢分润费
		AccountCollect(CopyProfitMap, CopyProfitAccount, pSettlementField->SettlementGroup, BanlanceMap, ACT_CopyProfit, "CopyProfit");
		//WriteErrorLog(pSettlementField->SettlementGroup.getValue(), NULL);

		if (m_pErrorFile != NULL)
			fclose(m_pErrorFile);

		string filename = sSnapPath;
		filename += "init.";
		filename += pSettlementField->SettlementGroup.getValue();
		filename += ".";
		filename += LargeNumberToString(g_nSequenceNo);
		m_pErrorFile = mfopen(filename.c_str(), "wt");

		CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findFirstBySettlementGroup(pSettlementField->SettlementGroup);
		while (pCurrency != NULL)
		{
			CWriteableCurrency theCurrency;
			m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);

			map<string, double>::iterator iterPreBanlance = PreBanlanceMap.find(theCurrency.Currency.getValue());
			//if (iterPreBanlance != PreBanlanceMap.end())
			//	REPORT_EVENT(LOG_CRITICAL, "BalanceCheck", "[%s/%s][Account.PreBalance:%s]", theCurrency.SettlementGroup.getValue(), theCurrency.Currency.getValue(),
			//		doubleToString(iterPreBanlance->second).c_str());

			map<string, double>::iterator iter = BanlanceMap.find(pCurrency->Currency.getValue());
			if (iter != BanlanceMap.end())
			{
				if (!theCurrency.Balance.equal(iter->second, 1048576))
				{
					char buffer[256];
					sprintf(buffer, "[%s][Currency:%s]!=[Account:%s][Diff:%s] --> Error!", theCurrency.Currency.getValue(),
						theCurrency.Balance.getString().c_str(), doubleToStringFormat(iter->second).c_str(), doubleToStringFormat(theCurrency.Balance.getValue() - iter->second).c_str());
					//REPORT_EVENT(LOG_CRITICAL, "BalanceCheck", buffer);
					//WriteErrorLog(pSettlementField->SettlementGroup.getValue(), buffer);
					if (m_pErrorFile != NULL)
					{
						fprintf(m_pErrorFile, "%s\n", buffer);
						fflush(m_pErrorFile);
					}
				}
				//else
				//{
				//	REPORT_EVENT(LOG_CRITICAL, "BalanceCheck", "[%s/%s][Currency:%s]==[Account:%s]", theCurrency.SettlementGroup.getValue(), theCurrency.Currency.getValue(),
				//		theCurrency.Balance.getString().c_str(), doubleToString(iter->second).c_str());
				//}

				theCurrency.PreBalance = theCurrency.Balance;
				theCurrency.Balance = theCurrency.Balance;
			}
			else
			{
				theCurrency.PreBalance.init();
				theCurrency.Balance.init();
			}
			theCurrency.Deposit.init();
			theCurrency.Withdraw.init();
			m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
			pCurrency = m_pMDB->m_CurrencyFactory->findNextBySettlementGroup();
		}

		//如果存在就删除掉，如果在调用函数内部处理，来回关闭就会导致性能低下
		if (m_pErrorFile != NULL)
		{
			fclose(m_pErrorFile);
			m_pErrorFile = NULL;
		}

		CWriteableSettleDetail theSettlementDetail;
		CopySettleDetailEntity(&theSettlementDetail, pSettlementField);
		m_pMDB->m_SettleDetailFactory->add(&theSettlementDetail);

		//char szPath[256];
		//sprintf(szPath, "%s/snap.%.0f.init_%s.2/", sSnapPath.c_str(), nSequenceNo, pSettlementField->SettlementGroup.getValue());
		//m_pMDB->save(szPath, nSequenceNo);
		break;
	}
	case SAT_SettlementEnd:
	{
		if (!m_pMDB->canSnap())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "MDBCanNotSnap");
		if (pSettlementField->SettlementGroup.isNull())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "SettlementGroupIsNull");

		CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findFirstBySettlementGroup(pSettlementField->SettlementGroup);
		while (pCurrency != NULL)
		{
			pSettlementField->SettleSegment = pCurrency->SettleSegment;
			CWriteableCurrency theCurrency;
			m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
			theCurrency.CanReduce = true;
			theCurrency.IsTrading = true;
			theCurrency.SettleSegment = m_gAccountDetailSequence.getCurrentValueByString();
			m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
			pCurrency = m_pMDB->m_CurrencyFactory->findNextBySettlementGroup();
		}
		char dumpName[256];
		sprintf(dumpName, "settle_%s_%s_2", pSettlementField->SettlementGroup.getValue(), pSettlementField->SettleSegment.getValue());
		g_bSnapName = dumpName;
		break;
	}
	case SAT_Delivery:
	{
		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pSettlementField->ExchangeID, pSettlementField->InstrumentID);
		if (pInstrument == NULL)
			return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);

		CMarketData* pMarketData = pInstrument->getMarketData();
		if (pMarketData != NULL)
		{
			if (pMarketData->InstrumentStatus != IS_NotActive)
				return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS, "MarketDataExsit");
		}

		if (pInstrument->ProductType == PT_CryptoExchange)
		{
			CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
			while (pPosition != NULL)
			{
				CPosition* pPositionToRemove = NULL;
				if(pPosition->getInstrument()==pInstrument)
					pPositionToRemove = pPosition;
				pPosition = m_pMDB->m_PositionFactory->getNext();
				if (pPositionToRemove != NULL)
					m_pMDB->m_PositionFactory->remove(pPositionToRemove);
			}
		}

		m_pMDB->m_MarketDataFactory->remove(pMarketData);

		//CWriteableSettleDetail theSettlementDetail;
		//CopySettleDetailEntity(&theSettlementDetail, pSettlementField);
		//theSettlementDetail.ExchangeID = pSettlementField->ExchangeID;
		//theSettlementDetail.InstrumentID = pSettlementField->InstrumentID;
		//m_pMDB->m_SettleDetailFactory->add(&theSettlementDetail);

		CLastKLine* pLastKLine = NULL;
		pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "1m");
		if (pLastKLine != NULL)
			m_pMDB->m_LastKLineFactory->remove(pLastKLine);
		pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "5m");
		if (pLastKLine != NULL)
			m_pMDB->m_LastKLineFactory->remove(pLastKLine);
		pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "1h");
		if (pLastKLine != NULL)
			m_pMDB->m_LastKLineFactory->remove(pLastKLine);
		pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "1d");
		if (pLastKLine != NULL)
			m_pMDB->m_LastKLineFactory->remove(pLastKLine);
		pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "1w");
		if (pLastKLine != NULL)
			m_pMDB->m_LastKLineFactory->remove(pLastKLine);
		pLastKLine = m_pMDB->m_LastKLineFactory->findByInstrumentBar(pInstrument->ExchangeID, pInstrument->InstrumentID, "1o");
		if (pLastKLine != NULL)
			m_pMDB->m_LastKLineFactory->remove(pLastKLine);

		CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pInstrument->BaseCurrency, pInstrument->SettlementGroup);
		if (pCurrency != NULL && pCurrency->Balance == 0.0)
		{
			if (pCurrency->Balance == 0.0)
			{
				CAccount* pAccount = m_pMDB->m_AccountFactory->getFirst();
				while (pAccount != NULL)
				{
					CAccount* pAccountToRemove = pAccount;
					pAccount = m_pMDB->m_AccountFactory->getNext();
					if (pAccountToRemove->getCurrency() == pCurrency)
						m_pMDB->m_AccountFactory->remove(pAccountToRemove);
				}
				m_pMDB->m_CurrencyFactory->remove(pCurrency);
			}
			else
			{
				if (pCurrency->ExchangeID == pInstrument->ExchangeID && pCurrency->InstrumentID == pInstrument->InstrumentID)
				{
					CWriteableCurrency theCurrency;
					m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
					theCurrency.ExchangeID.clear();
					theCurrency.InstrumentID.clear();
					CInstrument* pInstrumentNext = m_pMDB->m_InstrumentFactory->getFirst();
					while (pInstrumentNext != NULL)
					{
						if (pInstrumentNext != pInstrument && pInstrumentNext->BaseCurrency == pCurrency->Currency)
						{
							theCurrency.ExchangeID = pInstrumentNext->ExchangeID;
							theCurrency.InstrumentID = pInstrumentNext->InstrumentID;
							theCurrency.linkMarketData(pInstrumentNext->getMarketData());
						}
						pInstrumentNext = m_pMDB->m_InstrumentFactory->getNext();
					}
					m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
				}
			}

		}
		m_pMDB->m_InstrumentFactory->remove(pInstrument);
		break;
	}
	case SAT_DataRepair:
	{
		DataRepair(pSettlementField);
		break;
	}
	case SAT_DataClear:
	{
		DataClear(pSettlementField);
		break;
	}
	case SAT_AccountBalance:
	{
		if (pSettlementField->SettlementGroup.isNull())
			return m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "SettlementGroupIsNull");
		CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findFirstBySettlementGroup(pSettlementField->SettlementGroup);
		while (pCurrency != NULL)
		{
			CWriteableCurrency theCurrency;
			m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
			theCurrency.PreBalance.init();
			theCurrency.Deposit.init();
			theCurrency.Withdraw.init();

			CAccount* pAccount = m_pMDB->m_AccountFactory->getFirst();
			while (pAccount != NULL)
			{
				if (pAccount->SettlementGroup != pSettlementField->SettlementGroup)
				{
					pAccount = m_pMDB->m_AccountFactory->getNext();
					continue;
				}
				if (pAccount->Currency == theCurrency.Currency)
				{
					theCurrency.PreBalance += pAccount->PreBalance;
					theCurrency.Deposit += pAccount->Deposit;
					theCurrency.Withdraw += pAccount->Withdraw;
					theCurrency.PreBalance += pAccount->Fee;
				}
				pAccount = m_pMDB->m_AccountFactory->getNext();
			}
			theCurrency.Balance = theCurrency.PreBalance + theCurrency.Deposit - theCurrency.Withdraw;
			//REPORT_EVENT(LOG_CRITICAL, "AccountBalance", "[%s/%s][PreBalance:%s][Balance:%s][Deposit:%s][Withdraw:%s]", theCurrency.SettlementGroup.getValue(), theCurrency.Currency.getValue(),
			//	theCurrency.PreBalance.getString().c_str(), theCurrency.Balance.getString().c_str(), theCurrency.Deposit.getString().c_str(), theCurrency.Withdraw.getString().c_str());

			m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
			pCurrency = m_pMDB->m_CurrencyFactory->findNextBySettlementGroup();
		}
		break;
	}
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
		break;
	}
	}
	return true;
}

//void CSettlementService::WriteErrorLog(const char * pName, const char * pLog)
//{
//	if (m_pErrorFile == NULL)
//	{
//		string filename = sSnapPath;
//		filename += "init.";
//		filename += pName;
//		filename += ".";
//		filename += LargeNumberToString(g_nSequenceNo);
//		m_pErrorFile = mfopen(filename.c_str(), "at");
//	}
//	if (m_pErrorFile != NULL)
//	{
//		if (pLog != NULL)
//		{
//			fprintf(m_pErrorFile, "%s\n", pLog);
//			fflush(m_pErrorFile);
//		}
//	}
//}

bool CSettlementService::Settlement(CWriteableSettleDetail* pSettlementDetail)
{
	//获取各个合约的结算信息
	CInstrument* pInstrumentAll = m_pMDB->m_InstrumentFactory->findFirstBySettlementGroup(pSettlementDetail->SettlementGroup);
	for (; pInstrumentAll != NULL; pInstrumentAll = m_pMDB->m_InstrumentFactory->findNextBySettlementGroup())
	{
		if (pInstrumentAll->ProductClass != PC_Margin)
			continue;

		CPriceType SettlementPrice;
		SettlementPrice.clear();

		SettlementPrice = pInstrumentAll->getMarketData()->SettlementPrice;

		if (SettlementPrice.isNull())
			SettlementPrice = pInstrumentAll->getMarketData()->MarkedPrice;

		if (SettlementPrice.isNull())
			SettlementPrice = pInstrumentAll->getMarketData()->LastPrice;

		if (SettlementPrice.isNull())
			continue;

		// 修复反向合约结算时，PositionCost计算不对问题
		if (pInstrumentAll->IsInverse && SettlementPrice == 0.0)
			continue;

		CWriteableMarketData theMarketData;
		m_pMDB->m_MarketDataFactory->retrieve(pInstrumentAll->getMarketData(), &theMarketData);
		//得到下一次结算的时间
		theMarketData.PositionFeeTime = g_nCurrentTime + pSettlementDetail->Value3.getValue();
		m_pMDB->m_MarketDataFactory->update(pInstrumentAll->getMarketData(), &theMarketData);

		CWriteableSettleDetail theSettlementDetail;
		CopySettleDetailEntity(&theSettlementDetail, pSettlementDetail);
		theSettlementDetail.ExchangeID = pInstrumentAll->ExchangeID;
		theSettlementDetail.InstrumentID = pInstrumentAll->InstrumentID;
		//theSettlementDetail.ProductGroup = pInstrumentAll->ProductGroup;
		theSettlementDetail.Value = SettlementPrice;
		theSettlementDetail.Value1 = pSettlementDetail->Value1;
		theSettlementDetail.Value2 = pSettlementDetail->Value2;
		theSettlementDetail.Value3 = pSettlementDetail->Value3;
		m_pMDB->m_SettleDetailFactory->add(&theSettlementDetail);
	}

	//清空保证金信息
	CAccount* pAccount = m_pMDB->m_AccountFactory->getFirst();
	while (pAccount != NULL)
	{
		if (pAccount->SettlementGroup == pSettlementDetail->SettlementGroup &&  pAccount->UseMargin != 0.0)
		{
			CWriteableAccount theWriteableAccount;
			m_pMDB->m_AccountFactory->retrieve(pAccount, &theWriteableAccount);
			theWriteableAccount.UseMargin = 0.0;
			theWriteableAccount.CrossMargin = 0.0;
			applyAccountChange(&theWriteableAccount);
			theWriteableAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_AccountFactory->update(pAccount, &theWriteableAccount);
		}
		pAccount = m_pMDB->m_AccountFactory->getNext();
	}

	CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		if (pPosition->Position == 0)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		CSettleDetail* pSettleDetail = m_pMDB->m_SettleDetailFactory->findByInstrument(pSettlementDetail->SettleDetailID, pPosition->ExchangeID, pPosition->InstrumentID);
		if (pSettleDetail == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		CInstrument* pInstrument = pPosition->getInstrument();
		if (pInstrument == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		CAccount* pAccount = pPosition->getAccount();
		if (pAccount == NULL)
		{
			REPORT_EVENT(LOG_CRITICAL, "Settlement", "pPosition AccountID:%s,ClearCurrency:%s no Account", pPosition->AccountID.getValue(), pPosition->getInstrument()->ClearCurrency.getValue());
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		CWriteablePosition theWriteablePosition;
		m_pMDB->m_PositionFactory->retrieve(pPosition, &theWriteablePosition);

		CWriteableAccount theWriteableAccount;
		m_pMDB->m_AccountFactory->retrieve(pAccount, &theWriteableAccount);

		theWriteablePosition.CostPrice = pSettleDetail->Value;
		if (pInstrument->IsInverse)
			theWriteablePosition.PositionCost = fabs(theWriteablePosition.Position) * pInstrument->VolumeMultiple / pSettleDetail->Value.getValue();
		else
			theWriteablePosition.PositionCost = fabs(theWriteablePosition.Position) * pSettleDetail->Value.getValue() * pInstrument->VolumeMultiple;

		CCostType Profit = theWriteablePosition.PositionCost - pPosition->PositionCost;
		if (pPosition->PosiDirection == PD_Short || pPosition->Position < 0.0)
			Profit = -Profit;
		if (pInstrument->IsInverse)
			Profit = -Profit;

		theWriteableAccount.CloseProfit += Profit.getValue();

		theWriteablePosition.UseMargin = theWriteablePosition.PositionCost / theWriteablePosition.Leverage;
		//逐仓的盈亏到保证金、全仓的盈亏到余额
		if (pPosition->IsCrossMargin)
		{
			theWriteableAccount.UseMargin += theWriteablePosition.UseMargin;
			theWriteableAccount.CrossMargin += theWriteablePosition.UseMargin;
		}
		else
		{
			theWriteablePosition.UseMargin += Profit.getValue();
			if (theWriteablePosition.UseMargin <= 0)
				theWriteablePosition.UseMargin = 0.0;
			theWriteableAccount.UseMargin += theWriteablePosition.UseMargin;
		}
		applyAccountChange(&theWriteableAccount);
		if (Profit != 0.0)
		{
			CWriteableAccountDetail theAccountDetail;
			theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
			theAccountDetail.AccountID = theWriteableAccount.AccountID;
			theAccountDetail.SettlementGroup = theWriteableAccount.SettlementGroup;
			//theAccountDetail.ProductGroup = pPosition->ProductGroup;
			theAccountDetail.Currency = theWriteableAccount.Currency;
			theAccountDetail.SettleSegment = theWriteableAccount.getCurrency()->SettleSegment;
			theAccountDetail.MemberID = pPosition->MemberID;
			//			theAccountDetail.TradeUnitID = pPosition->TradeUnitID;
			theAccountDetail.InstrumentID = pPosition->InstrumentID;
			theAccountDetail.Amount = Profit;
			theAccountDetail.PreBalance = pAccount->Balance;
			theAccountDetail.Balance = theWriteableAccount.Balance;
			theAccountDetail.ReserveBalance = theWriteableAccount.Reserve;
			//theAccountDetail.Amount.cutByBigFloat(theAccountDetail.Balance.getValue());
			theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
			theAccountDetail.Source = ACT_Settlement;
			//sprintf(theAccountDetail.Remark.getString(), "Settlement And Price is %lf", SettlementPrice);
			theAccountDetail.Remark = "SettlementPrice:";
			theAccountDetail.Remark += pSettleDetail->Value.getAllString().c_str();
			theAccountDetail.LocalID = pSettlementDetail->LocalID;
			theAccountDetail.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);
		}
		theWriteablePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_PositionFactory->update(pPosition, &theWriteablePosition);

		theWriteableAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_AccountFactory->update(pAccount, &theWriteableAccount);
		pPosition = m_pMDB->m_PositionFactory->getNext();
	}
	return true;
}

double CSettlementService::AccountInit(CAccount* pAccount)
{
	double Fee = pAccount->Fee;
	if (pAccount->Balance != pAccount->PreBalance || pAccount->MoneyChange != 0.0 || pAccount->Deposit != 0.0 || pAccount->Withdraw != 0.0 || pAccount->CloseProfit != 0.0 || pAccount->Fee != 0.0)
	{
		CWriteableAccount theWriteableAccount;
		m_pMDB->m_AccountFactory->retrieve(pAccount, &theWriteableAccount);
		theWriteableAccount.PreBalance = theWriteableAccount.Balance;
		theWriteableAccount.MoneyChange = 0.0;
		theWriteableAccount.Deposit = 0.0;
		theWriteableAccount.Withdraw = 0.0;
		theWriteableAccount.CloseProfit = 0.0;
		theWriteableAccount.Fee = 0.0;
		theWriteableAccount.CopyProfit = 0.0;
		applyAccountChange(&theWriteableAccount);
		theWriteableAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_AccountFactory->update(pAccount, &theWriteableAccount);
	}
	return Fee;
}

bool CSettlementService::DataClear(CWriteableSettleDetail* pSettlementDetail)
{
	//清理持仓
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "Position")
		DataClearPosition();

	//清理MemberPosition
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "MemberPosition")
		DataClearMemberPosition();

	//清理过期的session
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "UserSession")
		DataClearUserSession();

	//清理资金
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "Account")
		DataClearAccount();

	//清理用户
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "User")
		DataClearUser();

	//m_pMDB->snap("./snap/snap.1000", 1000);
	return true;
}

bool CSettlementService::DataRepair(CWriteableSettleDetail* pSettlementDetail)
{
	//不管做什么，需要把数据库表的外键进行一次整理
	m_pMDB->linkAll();

	//对User表的统计字段进行重新统计
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "Member")
		DataRepairMember();

	// 对订单的冻结重新计算
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "Order")
		DataRepairOrder();

	//修复现货持仓冻结只对现货，同时写死了交易所
	//SpotPosition是新增持仓，RepairPosition也会新增持仓，这里需要
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "SpotPosition")
		DataRepairSpotPosition();

	//修复持仓冻结
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "Position")
		DataRepairPosition();

	//修复MemberPosition
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "MemberPosition")
		DataRepairMemberPosition();

	//修复资金冻结
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "Account")
		DataRepairAccount();

	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "TriggerOrder")
		DataRepairTriggerOrder();

	//如果找不到外键，将自己的主键置为空
	if (pSettlementDetail->Remark.isNull() || pSettlementDetail->Remark == "ClearLink")
		m_pMDB->clearLink();

	//修复完成之后需要link以下，让新增的纪律能够找到数据
	m_pMDB->linkAll();
	//m_pMDB->snap("./snap/snap.1000", 1000);
	return true;
}

void CSettlementService::DataRepairMemberPosition()
{
	CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->getFirst();
	while (pMemberPosition != NULL)
	{
		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pMemberPosition->ExchangeID, pMemberPosition->InstrumentID);
		if (pInstrument == NULL || pInstrument->ProductClass == PC_AccountExchange || pMemberPosition->AccountID.isNull())
		{
			CMemberPosition* pOldMemberPosition = pMemberPosition;
			pMemberPosition = m_pMDB->m_MemberPositionFactory->getNext();
			m_pMDB->m_MemberPositionFactory->remove(pOldMemberPosition);
		}
		else
		{
			CWriteableMemberPosition theMemberPosition;
			m_pMDB->m_MemberPositionFactory->retrieve(pMemberPosition, &theMemberPosition);
			theMemberPosition.Position.init();
			theMemberPosition.LongFrozen.init();
			theMemberPosition.ShortFrozen.init();
			m_pMDB->m_MemberPositionFactory->update(pMemberPosition, &theMemberPosition);
			pMemberPosition = m_pMDB->m_MemberPositionFactory->getNext();
		}
	}

	CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pPosition->ExchangeID, pPosition->InstrumentID);
		if (pInstrument != NULL && pInstrument->ProductClass != PC_AccountExchange)
		{
			CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(pPosition->AccountID,
				pPosition->ExchangeID, pPosition->InstrumentID);
			CWriteableMemberPosition theMemberPosition;
			if (pMemberPosition == NULL)
			{
				theMemberPosition.init();
				theMemberPosition.Position = fabs(pPosition->Position.getValue());
				theMemberPosition.LongFrozen = pPosition->LongFrozen;
				theMemberPosition.ShortFrozen = pPosition->ShortFrozen;
				theMemberPosition.AccountID = pPosition->AccountID;
				theMemberPosition.ExchangeID = pPosition->ExchangeID;
				theMemberPosition.InstrumentID = pPosition->InstrumentID;
				theMemberPosition.DefaultCrossMargin = true;
				theMemberPosition.DefaultLeverage = pPosition->Leverage;
				m_pMDB->m_MemberPositionFactory->add(&theMemberPosition);
			}
			else
			{
				m_pMDB->m_MemberPositionFactory->retrieve(pMemberPosition, &theMemberPosition);
				theMemberPosition.Position += pPosition->Position;
				theMemberPosition.Position.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				//theMemberPosition.PrePosition += pPosition->PrePosition;
				theMemberPosition.LongFrozen += pPosition->LongFrozen;
				theMemberPosition.LongFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				theMemberPosition.ShortFrozen += pPosition->ShortFrozen;
				theMemberPosition.ShortFrozen.toLowerTick(pPosition->getInstrument()->VolumeTick.getValue());
				//theMemberPosition.PreLongFrozen += pPosition->PreLongFrozen;
				//theMemberPosition.PreShortFrozen += pPosition->PreShortFrozen;
				/*if (theMemberPosition.DefaultLeverage < pPosition->Leverage)
					theMemberPosition.DefaultLeverage = pPosition->Leverage;*/
				m_pMDB->m_MemberPositionFactory->update(pMemberPosition, &theMemberPosition);
			}
		}
		pPosition = m_pMDB->m_PositionFactory->getNext();
	}
}

void CSettlementService::DataRepairMember()
{
	map<string, int> triggerOrdersMap;
	map<string, int> openOrdersMap;
	map<string, int> openPositionsMap;

	CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getFirst();
	while (pTriggerOrder != NULL)
	{
		map<string, int>::iterator iter = triggerOrdersMap.find(pTriggerOrder->MemberID.getValue());
		if (iter == triggerOrdersMap.end())
			triggerOrdersMap.insert(map<string, int>::value_type(pTriggerOrder->MemberID.getValue(), 1));
		else
			iter->second += 1;
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getNext();
	}

	COrder* pOrder = m_pMDB->m_OrderFactory->getFirst();
	while (pOrder != NULL)
	{
		map<string, int>::iterator iter = openOrdersMap.find(pOrder->MemberID.getValue());
		if (iter == openOrdersMap.end())
			openOrdersMap.insert(map<string, int>::value_type(pOrder->MemberID.getValue(), 1));
		else
			iter->second += 1;
		pOrder = m_pMDB->m_OrderFactory->getNext();
	}

	CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		if (pPosition->getInstrument() == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}
		if (pPosition->getInstrument()->ProductClass == PC_Premium || pPosition->getInstrument()->ProductClass == PC_Margin)
		{
			if (pPosition->Position != 0)
			{
				map<string, int>::iterator iter = openPositionsMap.find(pPosition->MemberID.getValue());
				if (iter == openPositionsMap.end())
					openPositionsMap.insert(map<string, int>::value_type(pPosition->MemberID.getValue(), 1));
				else
					iter->second += 1;
			}
		}
		pPosition = m_pMDB->m_PositionFactory->getNext();
	}

	CMember* pMember = m_pMDB->m_MemberFactory->getFirst();
	while (pMember != NULL)
	{
		CWriteableMember theMember;
		m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
		theMember.TriggerOrders.init();
		theMember.OpenOrders.init();
		theMember.OpenPositions.init();
		map<string, int>::iterator iter = triggerOrdersMap.find(pMember->MemberID.getValue());
		if (iter != triggerOrdersMap.end())
			theMember.TriggerOrders = iter->second;

		iter = openOrdersMap.find(pMember->MemberID.getValue());
		if (iter != openOrdersMap.end())
			theMember.OpenOrders = iter->second;

		iter = openPositionsMap.find(pMember->MemberID.getValue());
		if (iter != openPositionsMap.end())
			theMember.OpenPositions = iter->second;

		m_pMDB->m_MemberFactory->update(pMember, &theMember);
		pMember = m_pMDB->m_MemberFactory->getNext();
	}
}

void CSettlementService::DataRepairTriggerOrder()
{
	CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getFirst();
	while (pTriggerOrder != NULL)
	{
		if (!pTriggerOrder->RelatedOrderID.isNull())
		{
			if (pTriggerOrder->linkOrder() == NULL)
			{
				CWriteableTriggerOrder theTriggerOrder;
				m_pMDB->m_TriggerOrderFactory->retrieve(pTriggerOrder, &theTriggerOrder);
				theTriggerOrder.RelatedOrderID.clear();
				m_pMDB->m_TriggerOrderFactory->update(pTriggerOrder, &theTriggerOrder);
			}
		}
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getNext();
	}
}

void CSettlementService::DataRepairOrder()
{
	COrder* pOrder = m_pMDB->m_OrderFactory->getFirst();
	while (pOrder != NULL)
	{
		if (pOrder->Volume == 0)
		{
			COrder* pOrderToRemove = pOrder;
			pOrder = m_pMDB->m_OrderFactory->getNext();
			m_pMDB->m_OrderFactory->remove(pOrderToRemove);
			continue;
		}
		CInstrument* pInstrument = pOrder->getInstrument();
		if (pInstrument == NULL)
		{
			pOrder = m_pMDB->m_OrderFactory->getNext();
			continue;
		}

		CWriteableOrder theOrder;
		m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);

		theOrder.PosiDirection = getOrderPosiDirection(pOrder);
		if (theOrder.TradeUnitID.isNull())
			theOrder.TradeUnitID = theOrder.AccountID;

		switch (pInstrument->ProductClass)
		{
		case PC_Margin:
		case PC_Premium:
		{
			CFee* pFee = getFee(m_pMDB, MR_Taker, pInstrument, pOrder->getMember());

			CPriceType ClearPrice = pOrder->Price;
			if (pInstrument->IsInverse)
				ClearPrice = 1.0 / pOrder->Price;

			if (pOrder->OffsetFlag == OF_Open)
			{
				//冻结手续费
				theOrder.FrozenFee = pFee->OpenFeeAmount + pFee->OpenFeeRate * theOrder.VolumeRemain * ClearPrice * pInstrument->VolumeMultiple;
				//冻结保证金
				if (pOrder->Leverage > 0.0)
					theOrder.FrozenMargin = theOrder.VolumeRemain * pInstrument->VolumeMultiple * ClearPrice / pOrder->Leverage;
				else
					theOrder.FrozenMargin = -theOrder.VolumeRemain * pInstrument->VolumeMultiple * pOrder->Leverage;
			}
			else
			{
				double fee1 = pFee->CloseTodayFeeAmount + pFee->CloseTodayFeeRate * theOrder.VolumeRemain * ClearPrice * pInstrument->VolumeMultiple;
				double fee2 = pFee->CloseFeeAmount + pFee->CloseFeeRate * theOrder.VolumeRemain * ClearPrice * pInstrument->VolumeMultiple;
				theOrder.FrozenFee = MAX(fee1, fee2);
			}
			if (pInstrument->IsInverse)
				theOrder.ClearCurrency = pInstrument->BaseCurrency;
			else
				theOrder.ClearCurrency = pInstrument->ClearCurrency;
			theOrder.FeeCurrency = pInstrument->ClearCurrency;
			break;
		}
		case PC_AccountExchange:
		{
			if (pOrder->Direction == D_Buy)
			{
				theOrder.FrozenMoney = theOrder.VolumeRemain * theOrder.Price * pInstrument->VolumeMultiple;
				theOrder.ClearCurrency = pInstrument->ClearCurrency;
				theOrder.FeeCurrency = pInstrument->BaseCurrency;
			}
			else
			{
				theOrder.FrozenMoney = theOrder.VolumeRemain;
				theOrder.ClearCurrency = pInstrument->BaseCurrency;
				theOrder.FeeCurrency = pInstrument->ClearCurrency;
			}
			theOrder.OffsetFlag = OF_Open;
		}
		default:
		{
			theOrder.FrozenFee = 0.0;
			theOrder.FrozenMargin = 0.0;
		}
		}

		theOrder.PriceCurrency = pInstrument->PriceCurrency;
		//theOrder.FeeCurrency = pInstrument->ClearCurrency;
		//theOrder.ClearCurrency = theOrder.ClearCurrency;

		theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		CPosition* pPosition = theOrder.linkPosition();
		if (pPosition != NULL)
			theOrder.PositionID = pPosition->PositionID;
		m_pMDB->m_OrderFactory->update(pOrder, &theOrder);
		pOrder = m_pMDB->m_OrderFactory->getNext();
	}
}

void CSettlementService::DataRepairPosition()
{
	//CInstrument
	CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	while (pMarketData != NULL)
	{
		CWriteableMarketData theMarketData;
		m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
		theMarketData.OpenInterest = 0.0;
		m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
		pMarketData = m_pMDB->m_MarketDataFactory->getNext();
	}

	CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		if (pPosition->getInstrument() == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}
		if (pPosition->getInstrument()->getClearCurrency() == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}
		CMarketData *pMarketData = pPosition->getInstrument()->getMarketData();
		if (pMarketData == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		if (pPosition->getInstrument()->ProductClass == PC_Margin)
		{
			CWriteableMarketData theMarketData;
			m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);

			switch (pPosition->PosiDirection)
			{
			case PD_Long:
			{
				theMarketData.OpenInterest += pPosition->Position;
				break;
			}
			case PD_Short:
			{
				theMarketData.OpenInterest -= pPosition->Position;
				break;
			}
			case PD_Net:
			{
				theMarketData.OpenInterest += pPosition->Position;
				break;
			}
			}
			m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
		}

		CWriteablePosition thePosition;
		m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
		if (thePosition.PositionID.isNull())
			thePosition.PositionID = m_gOrderSequence.getNextValueByString();

		if (thePosition.AccountID.isNull())
			thePosition.AccountID = thePosition.MemberID;

		if (thePosition.TradeUnitID.isNull())
			thePosition.TradeUnitID = thePosition.AccountID;

		if (thePosition.Leverage <= 0)
			thePosition.Leverage = 1;

		//整理由于修改了tick导致的tick值不一致问题
		thePosition.Position.toLowerTick(pPosition->getInstrument()->VolumeTick);
		double MoneyTick = pPosition->getInstrument()->getClearCurrency()->MoneyTick;

		thePosition.CloseProfit.toLowerTick(MoneyTick);
		thePosition.TotalCloseProfit.toLowerTick(MoneyTick);
		m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
		pPosition = m_pMDB->m_PositionFactory->getNext();
	}

	//对不平的持仓进行补充
	CServiceConfig* pServiceConfig = m_pMDB->m_ServiceConfigFactory->findByConfig("RiskAccount", "default", "default");
	if (pServiceConfig != NULL)
	{
		CLongIndexIDType RiskMemberID = pServiceConfig->ConfigValue.getValue();
		CAccountIDType RiskAccountID = RiskMemberID.getValue();
		CIndexIDType TradeUnitID = RiskAccountID.getValue();
		CBoolType IsCrossMargin;
		IsCrossMargin.clear();
		CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(RiskMemberID);
		if (pMember != NULL)
		{
			CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
			while (pMarketData != NULL)
			{
				CWriteableMarketData theMarketData;
				if (pMarketData->OpenInterest != 0)
				{
					bool bOK = true;
					CPosition *pRiskPosition = NULL;
					CWriteablePosition thePosition;
					if (pMember->PositionType == PT_Net)
					{
						pRiskPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(RiskMemberID.getValue(), pMarketData->ExchangeID, pMarketData->InstrumentID, TradeUnitID, PD_Net);
						if (pRiskPosition != NULL)
						{
							m_pMDB->m_PositionFactory->retrieve(pRiskPosition, &thePosition);
							thePosition.Position -= pMarketData->OpenInterest;
						}
						else
						{
							thePosition.init();
							bOK = InitPosition(m_pMDB, m_pErrorEngine, pMarketData->getInstrument(), &thePosition, RiskMemberID, TradeUnitID, RiskAccountID, PD_Net, IsCrossMargin, DOUBLE_NaN);
							thePosition.Position -= pMarketData->OpenInterest;
						}
					}
					else
					{
						if (pMarketData->OpenInterest < 0)
						{
							pRiskPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(RiskMemberID.getValue(), pMarketData->ExchangeID, pMarketData->InstrumentID, TradeUnitID, PD_Long);
							if (pRiskPosition != NULL)
							{
								m_pMDB->m_PositionFactory->retrieve(pRiskPosition, &thePosition);
								thePosition.Position += -pMarketData->OpenInterest;
							}
							else
							{
								thePosition.init();
								bOK = InitPosition(m_pMDB, m_pErrorEngine, pMarketData->getInstrument(), &thePosition, RiskMemberID, TradeUnitID, RiskAccountID, PD_Long, IsCrossMargin, DOUBLE_NaN);
								thePosition.Position += -pMarketData->OpenInterest;
							}
						}
						else
						{
							pRiskPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(RiskMemberID.getValue(), pMarketData->ExchangeID, pMarketData->InstrumentID,  TradeUnitID, PD_Short);

							if (pRiskPosition != NULL)
							{
								m_pMDB->m_PositionFactory->retrieve(pRiskPosition, &thePosition);
								thePosition.Position += pMarketData->OpenInterest;
							}
							else
							{
								thePosition.init();
								bOK = InitPosition(m_pMDB, m_pErrorEngine, pMarketData->getInstrument(), &thePosition, RiskMemberID, TradeUnitID, RiskAccountID, PD_Short, IsCrossMargin, DOUBLE_NaN);
								thePosition.Position += pMarketData->OpenInterest;
							}
						}
					}

					if (bOK)
					{
						CPriceType clearPrice = pMarketData->MarkedPrice.getValue();
						if (clearPrice.isNull())
							clearPrice = pMarketData->LastPrice;

						if (pMarketData->getInstrument()->IsInverse)
							clearPrice = 1 / clearPrice.getValue();
						thePosition.PositionCost += pMarketData->getInstrument()->VolumeMultiple * abs(pMarketData->OpenInterest.getValue()) * clearPrice.getValue();
						m_pMDB->m_PositionFactory->addOrUpdate(pRiskPosition, &thePosition);
					}
				}
				pMarketData = m_pMDB->m_MarketDataFactory->getNext();
			}
		}
	}

	//补充有订单没有持仓的持仓
	COrder* pOrder = m_pMDB->m_OrderFactory->getFirst();
	while (pOrder != NULL)
	{
		if (pOrder->getInstrument() == NULL)
		{
			pOrder = m_pMDB->m_OrderFactory->getNext();
			continue;
		}

		CWriteablePosition thePosition;
		thePosition.init();
		CPosition* pPosition = pOrder->getPosition();
		//如果没有持仓需要新增的时候，第一个Order创建，第二个Order就不需要重复创建了
		if (pPosition == NULL)
			pPosition = pOrder->linkPosition();

		if (pPosition == NULL)
		{
			if (!InitPosition(m_pMDB, m_pErrorEngine, pOrder->getInstrument(), &thePosition, pOrder->MemberID, pOrder->TradeUnitID.getValue(), pOrder->AccountID, pOrder->PosiDirection, pOrder->IsCrossMargin, pOrder->Leverage))
			{
				pOrder = m_pMDB->m_OrderFactory->getNext();
				continue;
			}
		}
		else
		{
			m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
			//第一次需要清零
			if (pPosition->BusinessNo != m_gBusinessSequence.getCurrentValueByDouble())
			{
				thePosition.FrozenMargin = 0.0;
				thePosition.LongFrozenMargin = 0.0;
				thePosition.ShortFrozenMargin = 0.0;
				thePosition.LongFrozen = 0.0;
				thePosition.ShortFrozen = 0.0;
			}
		}

		switch (pOrder->getInstrument()->ProductClass)
		{
		case PC_Margin:
		case PC_Premium:
		{
			//thePosition.FrozenFee += pOrder->FrozenFee;
			if (pOrder->OffsetFlag == OF_Open)
			{
				//以后组合保证金上线之后，这个地方需要改
				//thePosition.FrozenMargin += pOrder->FrozenMargin;
				if (pOrder->Direction == D_Buy)
				{
					thePosition.LongFrozenMargin += pOrder->FrozenMargin;
					thePosition.LongFrozen += pOrder->VolumeRemain;
					//thePosition.LongFrozen.toLowerTick(thePosition.getInstrument()->VolumeTick.getValue());
					//thePosition.FrozenMoney += pOrder->FrozenMoney;
				}
				else
				{
					thePosition.ShortFrozenMargin += pOrder->FrozenMargin;
					thePosition.ShortFrozen += pOrder->VolumeRemain;
					//thePosition.ShortFrozen.toLowerTick(thePosition.getInstrument()->VolumeTick.getValue());
				}

				switch (thePosition.PosiDirection)
				{
				case PD_Long:
				case PD_Short:
				{
					thePosition.FrozenMargin += pOrder->FrozenMargin;
					break;
				}
				default:
					break;
				}
			}
			else
			{
				if (pOrder->Direction == D_Buy)
				{
					thePosition.LongFrozen += pOrder->VolumeRemain;
					//thePosition.LongFrozen.toLowerTick(thePosition.getInstrument()->VolumeTick.getValue());
				}
				else
				{
					thePosition.ShortFrozen += pOrder->VolumeRemain;
					//thePosition.ShortFrozen.toLowerTick(thePosition.getInstrument()->VolumeTick.getValue());
				}
			}
			break;
		}
		case PC_AccountExchange:
		{
			if (pOrder->Direction == D_Buy)
			{
				thePosition.LongFrozen += pOrder->VolumeRemain;
				//thePosition.LongFrozen.toLowerTick(thePosition.getInstrument()->VolumeTick.getValue());
				//thePosition.FrozenMoney += pOrder->FrozenMoney;
			}
			else
			{
				thePosition.ShortFrozen += pOrder->VolumeRemain;
				//thePosition.ShortFrozen.toLowerTick(thePosition.getInstrument()->VolumeTick.getValue());
			}
			break;
		}
		default:
			break;
		}

		thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();

		if (pPosition == NULL)
			pOrder->linkPosition(m_pMDB->m_PositionFactory->add(&thePosition));
		else
			m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
		pOrder = m_pMDB->m_OrderFactory->getNext();
	}

	pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	while (pMarketData != NULL)
	{
		CWriteableMarketData theMarketData;
		m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);
		theMarketData.OpenInterest = 0.0;
		m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);
		pMarketData = m_pMDB->m_MarketDataFactory->getNext();
	}

	// 单独计算净仓模式的FrozenMargin
	//这里一定会很疑惑，为什么还需要再来一次
	//有些持仓由于各种原因，有订单数据，但是缺没有订单等修改，说明是废持仓，所以需要清理
	//如果放到前面清理，由于持仓归零会自动删除导致后续无法有持仓依据
	pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		if (pPosition->getInstrument() == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}
		CMarketData *pMarketData = pPosition->getInstrument()->getMarketData();
		if (pMarketData == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		CWriteableMarketData theMarketData;
		m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theMarketData);

		switch (pPosition->PosiDirection)
		{
		case PD_Long:
		{
			theMarketData.OpenInterest += pPosition->Position;
			break;
		}
		case PD_Net:
		{
			if (pPosition->Position > 0)
				theMarketData.OpenInterest += pPosition->Position;
			break;
		}
		}
		m_pMDB->m_MarketDataFactory->update(pMarketData, &theMarketData);

		//持仓等于0，又没有订单让其变化，说明这个订单的数目需要清零
		if (pPosition->Position == 0 && pPosition->BusinessNo != m_gBusinessSequence.getCurrentValueByDouble())
		{
			CWriteablePosition thePosition;
			m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
			thePosition.FrozenMargin = 0.0;
			thePosition.LongFrozenMargin = 0.0;
			thePosition.ShortFrozenMargin = 0.0;
			thePosition.LongFrozen = 0.0;
			thePosition.ShortFrozen = 0.0;
			//是因为这里有可能导致删除，所以需先保留索引
			CPosition *pOldPosition = pPosition;
			pPosition = m_pMDB->m_PositionFactory->getNext();
			m_pMDB->m_PositionFactory->update(pOldPosition, &thePosition);
			continue;
		}

		CInstrument* pInstrument = pPosition->getInstrument();
		switch (pInstrument->ProductClass)
		{
		case PC_Margin:
		case PC_Premium:
		{
			switch (pPosition->PosiDirection)
			{
			case PD_Net:
			{
				CWriteablePosition thePosition;
				m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
				CMoneyType allLongSideMargin = thePosition.LongFrozenMargin;
				CMoneyType allShortSideMargin = thePosition.ShortFrozenMargin;
				CMoneyType newMargin = thePosition.UseMargin;
				if (thePosition.Position > 0)
					allLongSideMargin += newMargin;
				else if (thePosition.Position < 0)
					allShortSideMargin += newMargin;
				CMoneyType newFrozen = MAX(allLongSideMargin, allShortSideMargin) - newMargin;
				thePosition.FrozenMargin += newFrozen;
				m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
		pPosition = m_pMDB->m_PositionFactory->getNext();
	}

	CPositionOrder* pPositionOrder = m_pMDB->m_PositionOrderFactory->getFirst();
	while (pPositionOrder != NULL)
	{
		if (pPositionOrder->PositionID.isNull())
		{
			CPosition* pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(pPositionOrder->AccountID, pPositionOrder->ExchangeID, pPositionOrder->InstrumentID,  pPositionOrder->TradeUnitID, pPositionOrder->PosiDirection);
			if (pPosition != NULL && pPosition->Position != 0.0)
			{
				CWriteablePositionOrder thePositionOrder;
				m_pMDB->m_PositionOrderFactory->retrieve(pPositionOrder, &thePositionOrder);
				thePositionOrder.PositionID = pPosition->PositionID;
				m_pMDB->m_PositionOrderFactory->update(pPositionOrder, &thePositionOrder);
			}
			else
			{
				CPositionOrder* pTobeRemove = pPositionOrder;
				pPositionOrder = m_pMDB->m_PositionOrderFactory->getNext();
				m_pMDB->m_PositionOrderFactory->remove(pTobeRemove);
				continue;
			}
		}
		pPositionOrder = m_pMDB->m_PositionOrderFactory->getNext();
	}

	CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getFirst();
	while (pTriggerOrder != NULL)
	{
		if (pTriggerOrder->TriggerStatus != TS_None && pTriggerOrder->OffsetFlag != OF_Open)
		{
			CPosition* pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(pTriggerOrder->AccountID, pTriggerOrder->ExchangeID, pTriggerOrder->InstrumentID, pTriggerOrder->TradeUnitID, pTriggerOrder->PosiDirection);
			if (pPosition != NULL)
			{
				CWriteableTriggerOrder theTriggerOrder;
				m_pMDB->m_TriggerOrderFactory->retrieve(pTriggerOrder, &theTriggerOrder);
				theTriggerOrder.PositionID = pPosition->PositionID;
				m_pMDB->m_TriggerOrderFactory->update(pTriggerOrder, &theTriggerOrder);

				if (pTriggerOrder->OffsetFlag == OF_CloseAll)
				{
					CWriteablePosition thePosition;
					m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
					thePosition.CloseOrderID = pTriggerOrder->OrderID;
					thePosition.SLTriggerPrice = pTriggerOrder->SLTriggerPrice;
					thePosition.TPTriggerPrice = pTriggerOrder->TPTriggerPrice;
					m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
				}
			}
		}
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getNext();
	}
}

void CSettlementService::DataRepairSpotPosition()
{
	//CExchangeIDType ExchangeID = m_pMDB->m_InstrumentFactory->getFirst()->ExchangeID;
	//m_pMDB->m_InstrumentFactory->endGet();

	//CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
	//while (pPosition != NULL)
	//{
	//	if (pPosition->getInstrument() == NULL)
	//	{
	//		pPosition = m_pMDB->m_PositionFactory->getNext();
	//		continue;
	//	}
	//	if (pPosition->getInstrument()->ProductClass == PC_AccountExchange)
	//	{
	//		CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pPosition->AccountID, pPosition->SettlementGroup,pPosition->getInstrument()->BaseCurrency);
	//		if (pAccount != NULL)
	//		{
	//			CInstrumentIDType InstrumentID = pAccount->Currency.getValue();
	//			InstrumentID += "/USDT";
	//			CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(ExchangeID, InstrumentID);

	//			CWriteablePosition thePosition;
	//			m_pMDB->m_PositionFactory->retrieve(pPosition, &thePosition);
	//			thePosition.ClearCurrency = pAccount->Currency;
	//			thePosition.PriceCurrency = pInstrument->PriceCurrency;
	//			thePosition.Position = pAccount->Balance;
	//			thePosition.HighestPosition = thePosition.Position;
	//			thePosition.ShortFrozen = pAccount->FrozenMoney;
	//			thePosition.OpenPrice = pInstrument->getMarketData()->LastPrice;
	//			thePosition.TotalPositionCost = thePosition.Position * thePosition.OpenPrice * pInstrument->VolumeMultiple;
	//			thePosition.BeginTime = g_nCurrentTime;
	//			thePosition.InsertTime = g_nCurrentTime;
	//			thePosition.UpdateTime = g_nCurrentTime;
	//			thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	//			m_pMDB->m_PositionFactory->update(pPosition, &thePosition);
	//		}
	//	}
	//	pPosition = m_pMDB->m_PositionFactory->getNext();
	//}

	//CAccount* pAccount = m_pMDB->m_AccountFactory->getFirst();
	//while (pAccount != NULL)
	//{
	//	if (pAccount->Balance > 0 && pAccount->SettlementGroup == "Spot" && pAccount->Currency != "USDT")
	//	{
	//		CInstrumentIDType InstrumentID = pAccount->Currency.getValue();
	//		InstrumentID += "/USDT";
	//		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(ExchangeID, InstrumentID);

	//		bool bOK = true;
	//		CWriteablePosition thePosition;
	//		thePosition.init();
	//		CPosition* pPositionLong = m_pMDB->m_PositionFactory->findByTradeUnitAndInstrument(pAccount->MemberID, pAccount->AccountID.getValue(), ExchangeID, InstrumentID, PD_Long);
	//		if (pPositionLong == NULL)
	//			bOK = InitPosition(m_pMDB, m_pErrorEngine, pInstrument, &thePosition, pAccount->MemberID, pAccount->AccountID.getValue(), pAccount->AccountID, PD_Long, INT_32_MAX, DOUBLE_NaN);
	//		else
	//			m_pMDB->m_PositionFactory->retrieve(pPositionLong, &thePosition);

	//		if (bOK)
	//		{
	//			thePosition.Position = pAccount->Balance;
	//			thePosition.ShortFrozen = pAccount->FrozenMoney;
	//			thePosition.OpenPrice = pInstrument->getMarketData()->LastPrice;
	//			thePosition.TotalPositionCost = thePosition.Position * thePosition.OpenPrice * pInstrument->VolumeMultiple;
	//			thePosition.BeginTime = g_nCurrentTime;
	//			thePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	//			m_pMDB->m_PositionFactory->addOrUpdate(pPositionLong, &thePosition);
	//		}
	//	}
	//	pAccount = m_pMDB->m_AccountFactory->getNext();
	//}
}

void CSettlementService::DataRepairAccount()
{
	CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->getFirst();
	while (pCurrency != NULL)
	{
		CWriteableCurrency theCurrency;
		m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
		double MoneyTick = pCurrency->MoneyTick.getValue();

		theCurrency.Deposit.toLowerTick(MoneyTick);
		theCurrency.Withdraw.toLowerTick(MoneyTick);
		theCurrency.PreBalance.toLowerTick(MoneyTick);
		theCurrency.Balance = theCurrency.PreBalance + theCurrency.Deposit - theCurrency.Withdraw;
		theCurrency.LoanUse = 0.0;
		m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
		pCurrency = m_pMDB->m_CurrencyFactory->getNext();
	}

	CAccount* pAccount = m_pMDB->m_AccountFactory->getFirst();
	while (pAccount != NULL)
	{
		//必须删除币种没有的，防止之前的数据影响结果
		if (pAccount->getCurrency() == NULL)
		{
			CAccount* pToRemove = pAccount;
			pAccount = m_pMDB->m_AccountFactory->getNext();
			m_pMDB->m_AccountFactory->remove(pToRemove);
			continue;
		}
		CWriteableAccount theAccount;
		m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
		theAccount.FrozenFee = 0.0;
		theAccount.FrozenMargin = 0.0;
		theAccount.FrozenMoney = 0.0;
		theAccount.UseMargin = 0.0;
		theAccount.CrossMargin = 0.0;
		theAccount.PositionCost = 0.0;
		theAccount.UnRealProfit = 0.0;

		CCurrency *pCurrency = pAccount->getCurrency();
		CWriteableCurrency theCurrency;
		m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
		theCurrency.LoanUse += theAccount.Borrow;
		m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);

		double MoneyTick = theAccount.getCurrency()->MoneyTick;

		theAccount.PreBalance.toLowerTick(MoneyTick);
		theAccount.MoneyChange.toLowerTick(MoneyTick);
		theAccount.Deposit.toLowerTick(MoneyTick);
		theAccount.Withdraw.toUpperTick(MoneyTick);
		theAccount.CloseProfit.toLowerTick(MoneyTick);
		theAccount.Fee.toUpperTick(MoneyTick);

		theAccount.TotalCloseProfit.toLowerTick(MoneyTick);
		theAccount.TotalDeposit.toLowerTick(MoneyTick);
		theAccount.TotalWithdraw.toUpperTick(MoneyTick);
		theAccount.TotalFee.toUpperTick(MoneyTick);
		theAccount.TotalMoneyChange.toLowerTick(MoneyTick);
		theAccount.Reserve.toLowerTick(MoneyTick);
		applyAccountChange(&theAccount);

		//printf("[%s]theAccount[%s].FrozenMoney[%f]\n", theAccount.Currency, theAccount.AccountID, theAccount.FrozenMoney);

		theAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_AccountFactory->update(pAccount, &theAccount);
		pAccount = m_pMDB->m_AccountFactory->getNext();
	}

	CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		//if (pPosition->PositionClass == PC_Loan)
		//{
		//	CAccount* pAccount = pPosition->getAccount();
		//	if (pAccount == NULL)
		//		pAccount = pPosition->linkAccount(m_pMDB->m_AccountFactory);
		//	CCurrency *pCurrency = pAccount->getCurrency();
		//	CWriteableCurrency theCurrency;
		//	m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
		//	theCurrency.LoanTotal += pPosition->Position;
		//	m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
		//}

		if (pPosition->FrozenMargin != 0.0 || pPosition->UseMargin != 0.0)
		{
			CAccount* pAccount = pPosition->getAccount();
			if (pAccount == NULL)
				pAccount = pPosition->linkAccount();
			CWriteableAccount theAccount;
			if (pAccount != NULL)
			{
				m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
				theAccount.FrozenMargin += pPosition->FrozenMargin;
				theAccount.UseMargin += pPosition->UseMargin;
				theAccount.PositionCost += pPosition->PositionCost;
				if (pPosition->IsCrossMargin)
					theAccount.CrossMargin += pPosition->UseMargin;
				applyAccountChange(&theAccount);
				theAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
				m_pMDB->m_AccountFactory->update(pAccount, &theAccount);
			}
			else
			{
				theAccount.init(m_pMDB);
				theAccount.CanReduce = true;
				theAccount.AccountID = pPosition->AccountID;
				theAccount.Currency = pPosition->ClearCurrency;
				theAccount.MemberID = pPosition->MemberID;
				theAccount.SettlementGroup = pPosition->SettlementGroup;
				pAccount = m_pMDB->m_AccountFactory->add(&theAccount);
				pPosition->linkAccount(pAccount);
			}
		}
		pPosition = m_pMDB->m_PositionFactory->getNext();
	}


	COrder* pOrder = m_pMDB->m_OrderFactory->getFirst();
	while (pOrder != NULL)
	{
		//		CAccount *pAccount = pOrder->getAccount();
		CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pOrder->MemberID, pOrder->AccountID, pOrder->SettlementGroup, pOrder->ClearCurrency);
		if (pAccount != NULL)
		{
			CWriteableAccount theAccount;
			m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
			theAccount.FrozenMoney += pOrder->FrozenMoney;
			theAccount.FrozenFee += pOrder->FrozenFee;
			applyAccountChange(&theAccount);
			//printf("[%s]OrderFrozenMoney[%f]:theAccount.FrozenMoney[%f]\n", theAccount.Currency, pOrder->FrozenMoney, theAccount.FrozenMoney);
			theAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_AccountFactory->update(pAccount, &theAccount);
		}
		pOrder = m_pMDB->m_OrderFactory->getNext();
	}
}

bool CSettlementService::Delivery(CWriteableSettleDetail* pSettlementDetail)
{
	//撤单ToDo
	//CancelAll(pInstrument, pTransaction);
	//CancelAllTriggerOrder(pInstrument, pTransaction);
	//全仓持仓按照交割价格平仓
	CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		CInstrument* pInstrument = pPosition->getInstrument();
		if (pInstrument == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}
		if (pSettlementDetail->ExchangeID != pInstrument->ExchangeID)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		if (pSettlementDetail->InstrumentID != pInstrument->InstrumentID)
		{
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}

		CWriteablePosition theWriteablePosition;
		m_pMDB->m_PositionFactory->retrieve(pPosition, &theWriteablePosition);

		CAccount* pAccount = pPosition->getAccount();
		if (pAccount == NULL)
		{
			REPORT_EVENT(LOG_CRITICAL, "Delivery", "pPosition AccountID:%s,ClearCurrency:%s no Account", pPosition->AccountID.getValue(), pPosition->getInstrument()->ClearCurrency.getValue());
			pPosition = m_pMDB->m_PositionFactory->getNext();
			continue;
		}
		CWriteableAccount theWriteableAccount;
		m_pMDB->m_AccountFactory->retrieve(pAccount, &theWriteableAccount);

		if (pInstrument->IsInverse)
			theWriteablePosition.PositionCost = theWriteablePosition.Position * pInstrument->getMarketData()->SettlementPrice * pInstrument->VolumeMultiple;
		else
			theWriteablePosition.PositionCost = theWriteablePosition.Position * pInstrument->VolumeMultiple / pInstrument->getMarketData()->SettlementPrice;

		double profit = theWriteablePosition.PositionCost - pPosition->PositionCost;
		theWriteableAccount.CloseProfit += profit;
		applyAccountChange(&theWriteableAccount);

		theWriteableAccount.PreBalance = theWriteableAccount.Balance;
		theWriteableAccount.MoneyChange = 0.0;
		theWriteableAccount.Deposit = 0.0;
		theWriteableAccount.Withdraw = 0.0;
		theWriteableAccount.CloseProfit = 0.0;
		theWriteableAccount.Fee = 0.0;

		theWriteableAccount.TotalMoneyChange = 0.0;
		theWriteableAccount.TotalDeposit = 0.0;
		theWriteableAccount.TotalWithdraw = 0.0;
		theWriteableAccount.TotalCloseProfit = 0.0;
		theWriteableAccount.TotalFee = 0.0;

		theWriteableAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_AccountFactory->update(pAccount, &theWriteableAccount);

		theWriteablePosition.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_PositionFactory->update(pPosition, &theWriteablePosition);
		pPosition = m_pMDB->m_PositionFactory->getNext();
	}

	//删除合约和其他的设置

	//toDo

	return true;
}

bool CSettlementService::LoanInterest(CWriteableSettleDetail *pSettlementDetail, double InterestGap)
{
	CWriteableSettleDetail theSettlementDetail;
	CopySettleDetailEntity(&theSettlementDetail, pSettlementDetail);
	theSettlementDetail.ExchangeID = pSettlementDetail->ExchangeID;
	theSettlementDetail.InstrumentID = pSettlementDetail->InstrumentID;
	theSettlementDetail.SettlementGroup = pSettlementDetail->SettlementGroup;
	//theSettlementDetail.ProductGroup = pSettlementDetail->ProductGroup;
	theSettlementDetail.Value = pSettlementDetail->Value;
	theSettlementDetail.Value1 = pSettlementDetail->Value1;
	theSettlementDetail.Value2 = pSettlementDetail->Value2;
	theSettlementDetail.Value3 = pSettlementDetail->Value3;
	m_pMDB->m_SettleDetailFactory->add(&theSettlementDetail);

	CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->getFirst();
	while (pCurrency != NULL)
	{
		CWriteableCurrency theCurrency;
		m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
		LoanLoanInterest(&theCurrency);
		theCurrency.LoanUse = 0.0;
		theCurrency.CurrValue = 0.0;
		m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
		pCurrency = m_pMDB->m_CurrencyFactory->getNext();
	}

	CWriteableAccountDetail theAccountDetail;
	CAccount* pAccount = m_pMDB->m_AccountFactory->getFirst();
	while (pAccount != NULL)
	{
		CCurrency *pCurrency = pAccount->getCurrency();
		if (pCurrency == NULL)
		{
			pAccount = m_pMDB->m_AccountFactory->getNext();
			continue;
		}

		if (pAccount->Borrow > 0.0)
		{
			CWriteableAccount theAccount;
			m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);

			double LoanInterest = theAccount.Borrow * pCurrency->LoanBorrow * InterestGap;
			if (LoanInterest > 0)
			{
				theAccount.MoneyChange -= LoanInterest;
				applyAccountChange(&theAccount);
				applyReserveChange(pAccount, &theAccount, -LoanInterest);

				theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
				theAccountDetail.AccountID = theAccount.AccountID;
				theAccountDetail.SettlementGroup = theAccount.SettlementGroup;
				theAccountDetail.Currency = theAccount.Currency;
				theAccountDetail.SettleSegment = pCurrency->SettleSegment;
				theAccountDetail.ExchangeID = pCurrency->SettlementGroup;
				theAccountDetail.InstrumentID = pCurrency->Currency;
				//		theAccountDetail.ProductGroup = pCurrency->SettlementGroup;
				theAccountDetail.Amount = -LoanInterest;
				theAccountDetail.PreBalance = pAccount->Balance;
				theAccountDetail.Balance = theAccount.Balance;
				theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
				theAccountDetail.Source = ACT_LoanInterest;
				theAccountDetail.ReserveBalance = theAccount.Reserve;
				theAccountDetail.Remark = theAccount.Borrow.getString().c_str();
				theAccountDetail.Remark += ";";
				theAccountDetail.Remark += pCurrency->LoanBorrow.getString().c_str();
				theAccountDetail.LocalID = pSettlementDetail->LocalID;
				m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);
			}

			theAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			if (theAccount.Balance < 0.0)
				theAccount.Borrow = -theAccount.Balance;
			else
				theAccount.Borrow = 0.0;
			m_pMDB->m_AccountFactory->update(pAccount, &theAccount);

			CWriteableCurrency theCurrency;
			m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
			theCurrency.LoanUse += theAccount.Borrow;
			theCurrency.CurrValue += LoanInterest;
			m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
			pAccount = m_pMDB->m_AccountFactory->getNext();
		}
		else
		{
			CCurrency* pCurrency = pAccount->getCurrency();
			double LoanInterest = pAccount->Balance.getValue() * pCurrency->LoanDeposit.getValue() * InterestGap * (1 - g_dLoanInterestFeeRate);
			if (LoanInterest > 0)
			{
				CWriteableAccount theAccount;
				m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
				theAccount.MoneyChange += LoanInterest;
				applyAccountChange(&theAccount);
				theAccount.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
				m_pMDB->m_AccountFactory->update(pAccount, &theAccount);

				CWriteableCurrency theCurrency;
				m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
				theCurrency.CurrValue -= LoanInterest;
				m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);

				theAccountDetail.AccountDetailID = m_gAccountDetailSequence.getNextValueByString();
				theAccountDetail.AccountID = theAccount.AccountID;
				theAccountDetail.SettlementGroup = theAccount.SettlementGroup;
				theAccountDetail.Currency = theAccount.Currency;
				theAccountDetail.SettleSegment = theAccount.getCurrency()->SettleSegment;
				theAccountDetail.ExchangeID = pCurrency->SettlementGroup;
				theAccountDetail.InstrumentID = pCurrency->Currency;
				theAccountDetail.Amount = LoanInterest;
				theAccountDetail.PreBalance = pAccount->Balance;
				theAccountDetail.Balance = theAccount.Balance;
				theAccountDetail.Amount.toRoundTick(min(theAccountDetail.Balance.getCompareTick(), theAccountDetail.PreBalance.getCompareTick()));
				theAccountDetail.Source = ACT_LoanInterest;
				theAccountDetail.ReserveBalance = theAccount.Reserve;
				theAccountDetail.LocalID = pSettlementDetail->LocalID;
				m_pMDB->m_AccountDetailFactory->add(&theAccountDetail);
			}
		}
		pAccount = m_pMDB->m_AccountFactory->getNext();
	}

	CAccountIDType FeeAccount = "FeeAccount";
	CServiceConfig* pServiceConfig = m_pMDB->m_ServiceConfigFactory->findByConfig("FeeAccount", "default", "default");
	if (pServiceConfig != NULL)
		FeeAccount = pServiceConfig->ConfigValue.getValue();

	pCurrency = m_pMDB->m_CurrencyFactory->getFirst();
	while (pCurrency != NULL)
	{
		CWriteableCurrency theCurrency;
		m_pMDB->m_CurrencyFactory->retrieve(pCurrency, &theCurrency);
		double dFee = theCurrency.CurrValue;
		theCurrency.CurrValue = 0.0;
		Fee(dFee, FeeAccount, pCurrency->SettlementGroup, pCurrency->Currency, ACT_LoanInterest, "LoanInterestFee");
		m_pMDB->m_CurrencyFactory->update(pCurrency, &theCurrency);
		pCurrency = m_pMDB->m_CurrencyFactory->getNext();
	}

	return true;
}

//------------------------------------------------------------------------------------------------------

void CSettlementService::DataClearMemberPosition()
{
	CMemberPosition* pIndex = m_pMDB->m_MemberPositionFactory->getFirst();
	while (pIndex != NULL)
	{
		if (pIndex->Position == 0 && pIndex->LongFrozen == 0 && pIndex->ShortFrozen == 0)
		{
			if (pIndex->UpdateTime.isNull() && pIndex->UpdateTime < g_nCurrentTime - 180 * 24 * 60 * 60 * 1000)
			{
				CMemberPosition* pToRemove = pIndex;
				pIndex = m_pMDB->m_MemberPositionFactory->getNext();
				m_pMDB->m_MemberPositionFactory->remove(pToRemove);
				continue;
			}
		}
		pIndex = m_pMDB->m_MemberPositionFactory->getNext();
	}
}

void CSettlementService::DataClearUserSession()
{
	CUserSession* pIndex = m_pMDB->m_UserSessionFactory->getFirst();
	while (pIndex != NULL)
	{
		if (!pIndex->ExpireTime.isNull() && pIndex->ExpireTime < g_nCurrentTime)
		{
			CUserSession* pToRemove = pIndex;
			pIndex = m_pMDB->m_UserSessionFactory->getNext();
			m_pMDB->m_UserSessionFactory->remove(pToRemove);
			continue;
		}
		else
		{
			pIndex = m_pMDB->m_UserSessionFactory->getNext();
		}
	}
}

void CSettlementService::DataClearPosition()
{
	CPosition* pIndex = m_pMDB->m_PositionFactory->getFirst();
	while (pIndex != NULL)
	{
		if (pIndex->Position == 0 && pIndex->LongFrozen == 0 && pIndex->ShortFrozen == 0)
		{
			if (pIndex->UpdateTime < g_nCurrentTime - 30 * 24 * 60 * 60 * 1000)
			{
				CPosition* pToRemove = pIndex;
				pIndex = m_pMDB->m_PositionFactory->getNext();
				m_pMDB->m_PositionFactory->remove(pToRemove);
				continue;
			}
		}
		pIndex = m_pMDB->m_PositionFactory->getNext();
	}
}


void CSettlementService::DataClearAccount()
{
	CAccount* pIndex = m_pMDB->m_AccountFactory->getFirst();
	while (pIndex != NULL)
	{
		if (pIndex->Balance == 0.0 && pIndex->PreBalance == 0.0)
		{
			if (pIndex->UpdateTime < g_nCurrentTime - 30 * 24 * 60 * 60 * 1000)
			{
				CAccount* pToRemove = pIndex;
				pIndex = m_pMDB->m_AccountFactory->getNext();
				m_pMDB->m_AccountFactory->remove(pToRemove);
				continue;
			}
		}

		/*if (pIndex->getCurrency() == NULL)
		{
			CAccount* pToRemove = pIndex;
			pIndex = m_pMDB->m_AccountFactory->getNext();
			m_pMDB->m_AccountFactory->remove(pToRemove);
			continue;
		}*/

		pIndex = m_pMDB->m_AccountFactory->getNext();
	}
}

void CSettlementService::DataClearUser()
{

}
