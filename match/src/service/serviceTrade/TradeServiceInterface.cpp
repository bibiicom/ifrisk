/////////////////////////////////////////////////////////////////////////
///@system 交易引擎系统
///@company 上海兆百网络科技有限公司
///TradeServiceInterface.h
/////////////////////////////////////////////////////////////////////////

#include "TradeServiceInterface.h"
#include "CopyField.h"
#include "FieldData.h"
#include "CWedge.h"
#include "Config.h"

CTradeServiceInterface::CTradeServiceInterface(CTradeService *pService, CServiceResponser *pServiceResponser)
{
	m_pService = pService;
	m_pServiceResponser = pServiceResponser;

	m_handleOrderInsertIndex = new CUFINT8TotalMonitorIndex("HandleOrderInsert", 10);
	m_handleOrderInsertErrorIndex = new CUFINT8TotalMonitorIndex("HandleOrderInsertError", 10);
	m_handleOrderActionIndex = new CUFINT8TotalMonitorIndex("HandleOrderAction", 10);
	m_handleOrderActionErrorIndex = new CUFINT8TotalMonitorIndex("HandleOrderActionError", 10);

	m_InsertFailedLog = NULL;
	m_ActionFailedLog = NULL;

	string fileName = GetConfigString(INI_FILE_NAME, "ErrorPath");
	if (fileName != "")
	{
		makedir(fileName.c_str());
		string fileName1 = fileName + "InsertFailed.csv";
		m_InsertFailedLog = mfopen(fileName1.c_str(), "at");

		string fileName2 = fileName + "ActionFailed.csv";
		m_ActionFailedLog = mfopen(fileName2.c_str(), "at");
	}
}

CTradeServiceInterface::~CTradeServiceInterface()
{
}

bool CTradeServiceInterface::handleMessage(const TOutFogMetaRef* pOutFogReqRef, const  void *pStream, TSessionRef* pSession)
{
	bool success = true;
	int nRecvTNo = 0;

	switch (pOutFogReqRef->MessageNo)
	{
	case TID_SendChangeCurrency:
	{
		nRecvTNo = TID_RecvChangeCurrency;
		CChangeCurrencyField field;
		CChangeCurrencyField::m_Describe.getSingleField(&field, pStream);
		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
		if (pMember == NULL)
		{
			CWriteableMember theMemeber;
			theMemeber.init();
			theMemeber.MemberID = field.MemberID;
			theMemeber.MemberType = PT_Normal;
			theMemeber.SinkType = ST_SinkToDB;
			theMemeber.Priority.clear();
			m_pMDB->m_MemberFactory->add(&theMemeber, m_pTransaction);
		}
			
		if (field.AccountID.isNull())
			field.AccountID = field.MemberID;

		CInstrument* pInstrument1 = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID1);
		if (pInstrument1 == NULL)
		{
			m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}

		//先入金
		CAccountActionField accountActionField;
		accountActionField.MemberID = field.MemberID;
		accountActionField.AccountID = field.AccountID;
		accountActionField.SettlementGroup = pInstrument1->SettlementGroup;
		accountActionField.Currency = field.Currency;
		accountActionField.LocalID = field.LocalID;
		accountActionField.Amount = field.Amount;
		accountActionField.Remark = field.Remark;
		accountActionField.ActionType = AAT_Deposit;
		accountActionField.IsReserve = false;
		accountActionField.ActionLocalID = "";

		CAccountField outfield;
		success = m_pSettlementService->ReqAccountAction(&accountActionField, &outfield, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		//交换
		CWriteableOrder Order1;
		Order1.init();
		Order1.InstrumentID = field.InstrumentID1;
		Order1.linkInstrument(pInstrument1);
		Order1.ExchangeID = field.ExchangeID;
		Order1.MemberID = field.MemberID;
		Order1.AccountID = field.AccountID;
		Order1.Remark = field.LocalID;
		Order1.Tradable = true;
		if (pInstrument1->ClearCurrency == field.Currency)
		{
			Order1.Direction = D_Buy;
			Order1.Cost = field.Amount;
			Order1.OrderPriceType = OPT_ByCost;
		}			
		else
		{
			Order1.Direction = D_Sell;
			Order1.Volume = field.Amount;
			if (!field.Amount.isTick(pInstrument1->VolumeTick))
			{
				m_pErrorEngine->reportError(ERROR_VOLUME_NOT_ON_TICK);
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
			Order1.OrderPriceType = OPT_TenLevelPrice;
		}
		success = m_pService->insertOrder(&Order1, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		if (Order1.VolumeTraded != Order1.Volume)
		{
			m_pErrorEngine->reportError(ERROR_PRICE_OUT_OF_BIDPRICE1);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		accountActionField.AccountType = AAT_FrozenMoneyByOrder;
		if (!field.InstrumentID2.isNull())
		{
			CInstrument* pInstrument2 = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID2);
			if (pInstrument2 == NULL)
			{
				m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
				return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
			}
			CWriteableOrder Order2;
			Order2.init();
			Order2.InstrumentID = field.InstrumentID2;
			Order2.linkInstrument(pInstrument2);
			Order2.ExchangeID = field.ExchangeID;
			Order2.MemberID = field.MemberID;
			Order2.AccountID = field.AccountID;
			Order2.Tradable = true;

			if (Order1.Direction == D_Buy)
			{
				if (pInstrument1->BaseCurrency == pInstrument2->ClearCurrency)
				{
					Order2.Direction = D_Buy;
					Order2.Cost = Order1.VolumeTraded;
					if (Order1.FeeCurrency == pInstrument1->BaseCurrency)
						Order2.Cost -= Order1.Fee;
					Order2.OrderPriceType = OPT_ByCost;
				}
				else
				{
					Order2.Direction = D_Sell;
					Order2.OrderPriceType = OPT_TenLevelPrice;
					Order2.Volume = Order1.VolumeTraded;
					if (Order1.FeeCurrency == pInstrument1->BaseCurrency)
						Order2.Volume -= Order1.Fee;
					Order2.Volume.toTick(pInstrument2->VolumeTick);
				}
			}
			else
			{
				if (pInstrument1->ClearCurrency == pInstrument2->ClearCurrency)
				{
					Order2.Direction = D_Buy;
					Order2.Cost = Order1.Turnover;
					if (Order1.FeeCurrency == pInstrument1->ClearCurrency)
						Order2.Cost -= Order1.Fee;
					Order2.OrderPriceType = OPT_ByCost;
				}
				else
				{
					Order2.Direction = D_Sell;
					Order2.OrderPriceType = OPT_TenLevelPrice;
					Order2.Volume = Order1.Turnover;
					if (Order1.FeeCurrency == pInstrument1->ClearCurrency)
						Order2.Volume -= Order1.Fee;
					Order2.Volume.toTick(pInstrument2->VolumeTick);
				}
			}
			success = m_pService->insertOrder(&Order2, m_pTransaction);
			if (!success)
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

			if (Order1.VolumeTraded != Order1.Volume)
			{
				m_pErrorEngine->reportError(ERROR_PRICE_OUT_OF_BIDPRICE1);
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
			if (Order2.Direction == D_Buy)
			{
				accountActionField.Amount = Order2.VolumeTraded;
				accountActionField.Currency = pInstrument2->BaseCurrency;
				if (Order2.FeeCurrency == pInstrument2->BaseCurrency)
					accountActionField.Amount -= Order2.Fee;
			}
			else
			{
				accountActionField.Amount = Order2.Turnover;
				accountActionField.Currency = pInstrument2->ClearCurrency;
				if (Order2.FeeCurrency == pInstrument2->ClearCurrency)
					accountActionField.Amount -= Order2.Fee;
			}
		}
		else
		{
			if (Order1.Direction == D_Buy)
			{
				accountActionField.Amount = Order1.VolumeTraded;
				accountActionField.Currency = pInstrument1->BaseCurrency;
				if (Order1.FeeCurrency == pInstrument1->BaseCurrency)
					accountActionField.Amount -= Order1.Fee;
			}
			else
			{
				accountActionField.Amount = Order1.Turnover;
				accountActionField.Currency = pInstrument1->ClearCurrency;
				if (Order1.FeeCurrency == pInstrument1->ClearCurrency)
					accountActionField.Amount -= Order1.Fee;
			}
		}

		//冻结资金，方便出金
		accountActionField.ActionType = AAT_FrozenMoneyByOrder;
		success = m_pSettlementService->ReqAccountAction(&accountActionField, &outfield, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		COrder* pOrder = m_pMDB->m_OrderFactory->findByOrderID(accountActionField.OrderID);
		memcpy(&m_OrderField, pOrder, (int)((char*)&(pOrder->CreateTime) - (char*)pOrder));
		m_pServiceResponser->SendResponse(nRecvTNo, &m_OrderField, &(m_OrderField.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
		m_pServiceResponser->SendNext(&outfield, &(outfield.m_Describe), OUTFOG_CHAIN_CONTINUE);
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendOrderInsert:
	{
		nRecvTNo = TID_RecvOrderInsert;
		m_handleOrderInsertIndex->incValue();
		//COrderInsertField field;
		COrderInsertField::m_Describe.getSingleField(&m_OrderInsertField, pStream);

		//不支持用户自己使用 FlashTrade CFDPrice的
		if (m_OrderInsertField.OrderPriceType == OPT_CFDPrice)
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "OrderPriceType=9_NotAllowed");
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
		}
		if (m_OrderInsertField.OrderType == OT_FlashTrade)
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "OrderType=4_NotAllowed");
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
		}
		if (m_OrderInsertField.OffsetFlag == OF_ForceClose)
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "OffsetFlag=2_NotAllowed");
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
		}

		InitOrder(m_OrderInsertField);
		if (m_OrderInsertField.OrderPriceType == OPT_LimitPrice && m_OrderInsertField.Price.isNull())
		{
			m_pErrorEngine->reportError(ERROR_NO_LIMITPRICE);
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
		}

		//CWriteableOrder theStruct;
		//m_Order.init();
		//为了TradeServce的服务能被不同的服务调用，所以检查和Session相关的操作需要在外面做掉

		CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, "", m_pTransaction, m_OrderInsertField.MemberID, &(m_OrderInsertField.APPID));

		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));

		if (m_OrderInsertField.AccountID.isNull())
			m_OrderInsertField.AccountID = m_OrderInsertField.MemberID;

		if (!m_OrderInsertField.LocalID.isNull())
		{
			COrder* pOrder = m_pMDB->m_OrderFactory->findByLocalID(m_OrderInsertField.AccountID, m_OrderInsertField.LocalID);
			if (pOrder != NULL)
			{
				m_pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);
				return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
			}
		}

		if (!m_OrderInsertField.CopyProfitRate.isNull())
		{
			if (!copyProfitRateIsValid(m_OrderInsertField.CopyProfitRate))
			{
				m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
				return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
			}
		}

		//CopyInputOrderEntity(&m_Order, &m_OrderInsertField);
		memcpy(&m_Order, &m_OrderInsertField, (int)((char *)&(m_OrderInsertField.Reserve) - (char *)&m_OrderInsertField));
		memset(&(m_Order.OrderStatus), 0, (int)((char *)&(m_Order.CreateTime) - (char *)&m_Order.OrderStatus));

		m_Order.SLTriggerPrice.clear();
		m_Order.TPTriggerPrice.clear();
		m_Order.TriggerPrice.clear();
		m_Order.OpenPrice.clear();
		m_Order.TradePrice.clear();
		m_Order.DeriveSource.init();
		m_Order.InsertTime.clear();
		m_Order.PositionID.clear();

		//Todo:如果是管理员的强平报单，可以使用更高级别的
		m_Order.Tradable = true;
		m_Order.linkMember(pMember);
		m_Order.linkInstrument((CInstrument*)NULL);
		m_Order.linkPosition((CPosition*)NULL);
		m_Order.linkTriggerOrder((CTriggerOrder*)NULL);
		m_Order.FrontNo = pSession->frontID;
		//		m_Order.SessionNo = pSession->sessionID;
		m_Order.UserID = pSession->pUserID;

		success = m_pService->insertOrder(&m_Order, m_pTransaction);

		if (!success)
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));

		//CopyOrderEntity(&m_OrderField, &m_Order);
		memcpy(&m_OrderField, &m_Order, (int)((char *)&(m_Order.CreateTime) - (char *)&m_Order));

		if (m_Order.APPID.isNull())
		{
			//CopyInputOrderEntity(&field, &theStruct);
			//m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			m_pServiceResponser->SendResponse(nRecvTNo, &m_OrderField, &(m_OrderField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		else
		{
			m_pServiceResponser->SendResponse(nRecvTNo, &m_OrderField, &(m_OrderField.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
			if (m_Order.getPosition() != NULL)
			{
				//CPositionField positionField;
				//CopyPositionEntity(&m_PositionField, m_Order.getPosition());
				memcpy(&m_PositionField, m_Order.getPosition(), (int)((char *)&(m_Order.getPosition()->Remark) - (char *)m_Order.getPosition()));
				m_pServiceResponser->SendNext(&m_PositionField, &(m_PositionField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		}
		break;
	}
	case TID_SendCancelOrderInsert:
	{
		nRecvTNo = TID_RecvCancelOrderInsert;
		m_handleOrderInsertIndex->incValue();
		//COrderInsertField field;
		COrderInsertField::m_Describe.getSingleField(&m_OrderInsertField, pStream);

		//不支持用户自己使用 FlashTrade CFDPrice的
		if (m_OrderInsertField.OrderPriceType == OPT_CFDPrice)
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "OrderPriceType=9_NotAllowed");
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
		}
		if (m_OrderInsertField.OrderType == OT_FlashTrade)
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "OrderType=4_NotAllowed");
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
		}
		if (m_OrderInsertField.OffsetFlag == OF_ForceClose)
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "OffsetFlag=2_NotAllowed");
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
		}

		InitOrder(m_OrderInsertField);
		if (m_OrderInsertField.OrderPriceType != OPT_LimitPrice || m_OrderInsertField.Price.isNull())
		{
			m_pErrorEngine->reportError(ERROR_NO_LIMITPRICE);
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
		}

		CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, "", m_pTransaction, m_OrderInsertField.MemberID, &(m_OrderInsertField.APPID));
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
		if (m_OrderInsertField.AccountID.isNull())
			m_OrderInsertField.AccountID = m_OrderInsertField.MemberID;

		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(m_OrderInsertField.ExchangeID, m_OrderInsertField.InstrumentID);
		if (pInstrument == NULL)
		{
			m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
		}
		m_OrderAction.ActionFlag = AF_Delete;
		if (!m_OrderInsertField.LocalID.isNull())
		{
			COrder* pOrder = m_pMDB->m_OrderFactory->findByLocalID(m_OrderInsertField.AccountID, m_OrderInsertField.LocalID);
			if (pOrder != NULL)
			{
				//m_pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);
				//return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));
				CTransaction theTransaction;
				m_OrderAction.OrderID = pOrder->OrderID;
				m_OrderAction.linkMember(pOrder->getMember());
				m_OrderAction.linkOrder(pOrder);
				m_OrderAction.linkTriggerOrder((CTriggerOrder*)NULL);
				if (m_pService->orderAction(&m_OrderAction, &theTransaction))
					theTransaction.commit();
				else
					theTransaction.rollback();
			}
		}

		CancleOtherOrder(pInstrument, m_OrderInsertField.Price, m_OrderInsertField.Direction);

		//CopyInputOrderEntity(&m_Order, &m_OrderInsertField);
		memcpy(&m_Order, &m_OrderInsertField, (int)((char *)&(m_OrderInsertField.PositionID) - (char *)&m_OrderInsertField));
		memset(&(m_Order.OrderStatus), 0, (int)((char *)&(m_Order.CreateTime) - (char *)&m_Order.OrderStatus));

		m_Order.SLTriggerPrice.clear();
		m_Order.TPTriggerPrice.clear();
		m_Order.TriggerPrice.clear();
		m_Order.OpenPrice.clear();
		m_Order.TradePrice.clear();
		m_Order.DeriveSource.init();
		m_Order.InsertTime.clear();
		m_Order.PositionID.clear();

		//Todo:如果是管理员的强平报单，可以使用更高级别的
		m_Order.Tradable = true;
		m_Order.linkMember(pMember);
		m_Order.linkInstrument((CInstrument*)NULL);
		m_Order.linkPosition((CPosition*)NULL);
		m_Order.linkTriggerOrder((CTriggerOrder*)NULL);
		m_Order.FrontNo = pSession->frontID;
		//		m_Order.SessionNo = pSession->sessionID;
		m_Order.UserID = pSession->pUserID;

		success = m_pService->insertOrder(&m_Order, m_pTransaction);

		if (!success)
			return RspError(nRecvTNo, pSession, &m_OrderInsertField, &(m_OrderInsertField.m_Describe));

		//COrderField outfield;
		//CopyOrderEntity(&m_OrderField, &m_Order);
		memcpy(&m_OrderField, &m_Order, (int)((char *)&(m_Order.CreateTime) - (char *)&m_Order));

		if (m_Order.APPID.isNull())
		{
			m_pServiceResponser->SendResponse(nRecvTNo, &m_OrderField, &(m_OrderField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		else
		{
			m_pServiceResponser->SendResponse(nRecvTNo, &m_OrderField, &(m_OrderField.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
			if (m_Order.getPosition() != NULL)
			{
				//CPositionField positionField;
				//CopyPositionEntity(&positionField, m_Order.getPosition());
				memcpy(&m_PositionField, m_Order.getPosition(), (int)((char *)&(m_Order.getPosition()->Remark) - (char *)m_Order.getPosition()));
				m_pServiceResponser->SendNext(&m_PositionField, &(m_PositionField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		}
		break;
	}
	case TID_SendCloseOrderInsert:
	{
		nRecvTNo = TID_RecvCloseOrderInsert;
		m_handleOrderInsertIndex->incValue();
		CCloseOrderInsertField field;
		CCloseOrderInsertField::m_Describe.getSingleField(&field, pStream);

		InitOrder(field);

		if (field.OffsetFlag != OF_Open)
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "OffsetFlagMustBe0");
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		CWriteableOrder theStruct;
		theStruct.init();

		CMember* pMember;
		if (!field.APPID.isNull())
		{
			theStruct.APPID = field.APPID;
			pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
		}
		else
		{
			pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, &(theStruct.APPID));
			if (!theStruct.APPID.isNull()) {
				field.APPID = theStruct.APPID;
			}
		}

		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		if (!field.CopyProfitRate.isNull())
		{
			if (!copyProfitRateIsValid(field.CopyProfitRate))
			{
				m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
		}

		//OrderID有值，表示报单止盈止损的改单：先撤单后下单
		if (!field.OrderID.isNull())
		{
			CWriteableOrderAction theOrderAction;
			theOrderAction.init();
			theOrderAction.OrderID = field.OrderID;
			theOrderAction.OrderLocalID = field.LocalID;
			//			theOrderAction.UserID = field.UserID;
			theOrderAction.MemberID = field.MemberID;
			theOrderAction.ExchangeID = field.ExchangeID;
			theOrderAction.InstrumentID = field.InstrumentID;
			theOrderAction.ActionFlag = AF_Delete;
			theOrderAction.linkMember(pMember);
			success = m_pService->orderAction(&theOrderAction, m_pTransaction);
			if (!success)
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}

		CopyInputOrderEntity(&theStruct, &field);
		theStruct.FrontNo = pSession->frontID;
		//		theStruct.SessionNo = pSession->sessionID;
				//Todo:如果是管理员的强平报单，可以使用更高级别的
		theStruct.Tradable = true;
		theStruct.linkMember(pMember);

		//需要把触发订单的信息去掉和保存起来
		CWriteableTriggerOrder theTriggerOrder;
		theTriggerOrder.init(m_pMDB);
		//这里没有使用CopyTriggerOrderInsertEntity，因为CCloseOrderInsertField并不是完全匹配
		CopyInputOrderEntity(&theTriggerOrder, &field);

		theTriggerOrder.OrderType = OT_Normal;
		//theTriggerOrder因为只拷贝了PositionId,其他需要单独赋值。
		theTriggerOrder.SLPrice = field.CloseSLPrice;
		if (!field.SLTriggerPrice.isNull())
			theTriggerOrder.SLTriggerPrice = field.SLTriggerPrice;
		else
			theTriggerOrder.SLTriggerPrice = field.CloseSLTriggerPrice;
		theTriggerOrder.TPPrice = field.CloseTPPrice;
		if (!field.TPTriggerPrice.isNull())
			theTriggerOrder.TPTriggerPrice = field.TPTriggerPrice;
		else
			theTriggerOrder.TPTriggerPrice = field.CloseTPTriggerPrice;
		theTriggerOrder.TriggerPrice = field.CloseTriggerPrice;
		theTriggerOrder.Price = field.ClosePrice;
		theTriggerOrder.OrderPriceType = field.CloseOrderPriceType;
		theTriggerOrder.TriggerPriceType = field.CloseTriggerPriceType;
		theTriggerOrder.BusinessType = BT_R3;

		success = m_pService->insertCloseTriggerOrder(&theStruct, &theTriggerOrder, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		success = m_pService->insertOrder(&theStruct, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		CTriggerOrderField triggerOrderField;
		//由于订单下单之后，TriggerOrder的状态会发生变化，这里取最新的TriggerOrder来回报,由于普遍更新设置为在commit的时候做
		//客户端要第一时间得到反馈，这里必须要先设置一下
		CopyTriggerOrderEntity(&triggerOrderField, &theTriggerOrder);
		if (theStruct.OrderStatus == OS_AllTraded)
		{
			theTriggerOrder.TriggerStatus = TS_Active;
			theTriggerOrder.Volume = theStruct.VolumeTraded;
			theTriggerOrder.TradeUnitID = theStruct.TradeUnitID;
			theTriggerOrder.ActiveTime = g_nCurrentTime;
		}
		else if (theStruct.OrderStatus == OS_AllCanceled || theStruct.OrderStatus == OS_PartCanceled)
		{
			theTriggerOrder.TriggerStatus = TS_Canceled;
			theTriggerOrder.ErrorMsg = "OriginalOrderCanceled";
		}
		m_pServiceResponser->SendResponse(nRecvTNo, &triggerOrderField, &(triggerOrderField.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
		if (theStruct.getPosition() != NULL)
		{
			CPositionField positionField;
			CopyPositionEntity(&positionField, theStruct.getPosition());
			m_pServiceResponser->SendNext(&positionField, &(positionField.m_Describe), OUTFOG_CHAIN_CONTINUE);
		}
		COrderField orderField;
		CopyOrderEntity(&orderField, &theStruct);
		m_pServiceResponser->SendNext(&orderField, &(orderField.m_Describe), OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendTriggerOrderInsert:
	{
		nRecvTNo = TID_RecvTriggerOrderInsert;
		m_handleOrderInsertIndex->incValue();
		CTriggerOrderInsertField field;
		CTriggerOrderInsertField::m_Describe.getSingleField(&field, pStream);

		if (field.OffsetFlag == OF_ForceClose)
		{
			m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "OffsetFlag=2_NotAllowed");
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		InitOrder(field);

		if (field.RiskBefore.isNull())
			field.RiskBefore.init();
		if (field.TriggerPriceType.isNull())
			field.TriggerPriceType.init();
		if (field.TriggerOrderType.isNull())
			field.TriggerOrderType.init();

		CWriteableTriggerOrder theTriggerOrder;
		theTriggerOrder.init(m_pMDB);

		CMember* pMember;
		if (!field.APPID.isNull())
		{
			theTriggerOrder.APPID = field.APPID;
			pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, "", m_pTransaction, field.MemberID, NULL);
		}
		else
		{
			pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, "", m_pTransaction, field.MemberID, &(theTriggerOrder.APPID));
			if (!theTriggerOrder.APPID.isNull())
				field.APPID = theTriggerOrder.APPID;
		}
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		if (!field.LocalID.isNull())
		{
			COrder* pOrder = m_pMDB->m_OrderFactory->findByLocalID(field.AccountID, field.LocalID);
			if (pOrder != NULL)
			{
				m_pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
		}

		if (!field.CopyProfitRate.isNull())
		{
			if (!copyProfitRateIsValid(field.CopyProfitRate))
			{
				m_pErrorEngine->reportError(ERROR_INVALID_VALUE);
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
		}

		//OrderID有值，表示条件单止盈止损的改单：先撤单后下单
		if (!field.OrderID.isNull())
		{
			CTriggerOrderActionField theTriggerOrderAction;
			//theTriggerOrderAction.init();
			theTriggerOrderAction.OrderID = field.OrderID;
			theTriggerOrderAction.OrderLocalID = field.LocalID;
			//theTriggerOrderAction.UserID = field.UserID;
			theTriggerOrderAction.MemberID = field.MemberID;
			theTriggerOrderAction.ExchangeID = field.ExchangeID;
			theTriggerOrderAction.InstrumentID = field.InstrumentID;
			theTriggerOrderAction.ActionFlag = AF_Delete;
			//theTriggerOrderAction.linkMember(pMember);
			if (m_pService->triggerOrderAction(&theTriggerOrderAction, m_pTransaction) == NULL)
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}

		CopyTriggerOrderInsertEntity(&theTriggerOrder, &field);
		theTriggerOrder.linkMember(pMember);

		//先注释掉，用于测试净仓的分仓逐仓是否符合预期
		////净仓剔除全仓分仓，逐仓
		//if (pMember->PositionType == PT_Net)
		//{
		//	//净仓不支持分仓
		//	if (field.TradeUnitID != field.AccountID)
		//	{
		//		m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitID[%s]", field.TradeUnitID.getValue());
		//		return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		//	}
		//	//净仓不支持逐仓
		//	if (!field.IsCrossMargin.isNull() && !field.IsCrossMargin)
		//	{
		//		m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitID[%s],IsCrossMargin[%d]", field.TradeUnitID.getValue(), field.IsCrossMargin.getValue());
		//		return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		//	}
		//}

		if (!theTriggerOrder.RelatedOrderID.isNull())
		{
			//根据TriggerOrder的RelatedOrderID找到Order
			COrder *pOrder = m_pMDB->m_OrderFactory->findByOrderID(theTriggerOrder.RelatedOrderID);
			if (pOrder == NULL)
			{
				m_pErrorEngine->reportError(ERROR_ORDER_NOT_FOUND);
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
			if (!pOrder->TriggerOrderID.isNull())
			{
				m_pErrorEngine->reportError(ERROR_ORDER_EXIST, "TriggerOrderIDExsit");
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
			CWriteableOrder theOrder;
			m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);
			if (!m_pService->insertCloseTriggerOrder(&theOrder, &theTriggerOrder, m_pTransaction))
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
			m_pMDB->m_OrderFactory->update(pOrder, &theOrder);
		}
		else
		{
			success = m_pService->triggerOrderInsert(&theTriggerOrder, TS_Active, m_pTransaction);
		}

		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		CTriggerOrderField outField;
		CopyTriggerOrderEntity(&outField, &theTriggerOrder);
		m_pServiceResponser->SendResponse(nRecvTNo, &outField, &(outField.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
		if (theTriggerOrder.getOrder() != NULL)
		{
			COrderField orderField;
			CopyOrderEntity(&orderField, theTriggerOrder.getOrder());
			m_pServiceResponser->SendNext(&orderField, &(orderField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			if (theTriggerOrder.getOrder()->getPosition() != NULL)
			{
				CPositionField positionField;
				CopyPositionEntity(&positionField, theTriggerOrder.getOrder()->getPosition());
				m_pServiceResponser->SendNext(&positionField, &(positionField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			}
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendTriggerOrderAction:
	{
		nRecvTNo = TID_RecvTriggerOrderAction;
		CTriggerOrderActionField field;
		CTriggerOrderActionField::m_Describe.getSingleField(&field, pStream);

		if (field.AccountID.isNull())
			field.AccountID = field.MemberID;

		if (field.MemberID.isNull())
			field.MemberID = field.AccountID;

		CTriggerOrderActionField theTriggerOrderAction;
		//theTriggerOrderAction.init();
		CopyTriggerOrderActionEntity(&theTriggerOrderAction, &field);

		switch (theTriggerOrderAction.ActionFlag)
		{
		case AF_Delete:
		{
			CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
			if (pMember == NULL)
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			//theTriggerOrderAction.linkMember(pMember);
			break;
		}
		case AF_Active:
		case AF_Trigger:
		{
			if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			break;
		}
		case AF_Modify:
		{
			if (field.OrderID.isNull())
			{
				m_pErrorEngine->reportError(ERROR_BAD_FIELD, "OrderID cannot be null");
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
			break;
		}
		default:
		{
			m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		}

		if (field.OrderID.isNull() && field.OrderLocalID.isNull())
		{
			success = m_pService->triggerOrderActionBatch(&theTriggerOrderAction, m_pTransaction);
			if (!success)
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		else
		{
			CTriggerOrder* pTriggerOrder = m_pService->triggerOrderAction(&theTriggerOrderAction, m_pTransaction);
			if (pTriggerOrder == NULL)
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			CTriggerOrderField outfield;
			CopyTriggerOrderEntity(&outfield, pTriggerOrder);
			m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
			//if (theTriggerOrderAction.getTriggerOrder()->getOrder() != NULL)
			//{
			//	COrderField orderField;
			//	CopyOrderEntity(&orderField, theTriggerOrderAction.getTriggerOrder()->getOrder());
			//	m_pServiceResponser->SendNext(&orderField, &(orderField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			//	if (theTriggerOrderAction.getTriggerOrder()->getOrder()->getPosition() != NULL)
			//	{
			//		CPositionField positionField;
			//		CopyPositionEntity(&positionField, theTriggerOrderAction.getTriggerOrder()->getOrder()->getPosition());
			//		m_pServiceResponser->SendNext(&positionField, &(positionField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			//	}
			//}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		}
		break;
	}
	//case TID_SendQuoteInsert:
	//{
	//	nRecvTNo = TID_RecvQuoteInsert;
	//	CQuoteInsertField field;
	//	CQuoteInsertField::m_Describe.getSingleField(&field, pStream);
	//	CWriteableQuote theStruct;
	//	theStruct.init();
	//	CopyInputQuoteEntity(&theStruct, &field);
	//	if (!m_pService->insertQuote(&theStruct, m_pTransaction))
	//		return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
	//	CQuoteField outfield;
	//	CopyQuoteEntity(&outfield, &theStruct);
	//	m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
	//	break;
	//}
	case TID_SendOrderAction:
	{
		nRecvTNo = TID_RecvOrderAction;
		m_handleOrderActionIndex->incValue();
		//COrderActionField field;
		COrderActionField::m_Describe.getSingleField(&m_OrderActionField, pStream);

		if (!(m_OrderActionField.ActionFlag == AF_Delete || m_OrderActionField.ActionFlag == AF_Modify))
		{
			m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
			return RspError(nRecvTNo, pSession, &m_OrderActionField, &(m_OrderActionField.m_Describe));
		}

		switch (m_OrderActionField.ActionFlag)
		{
		case AF_Delete:
		{
			break;
		}
		case AF_Modify:
		{
			if (m_OrderActionField.OrderID.isNull())
			{
				m_pErrorEngine->reportError(ERROR_BAD_FIELD, "OrderID cannot be null");
				return RspError(nRecvTNo, pSession, &m_OrderActionField, &(m_OrderActionField.m_Describe));
			}
			break;
		}
		default:
		{
			break;
		}
		}

		//CWriteableOrderAction theStruct;
		//CopyOrderActionEntity(&m_OrderAction, &m_OrderActionField);
		memcpy(&m_OrderAction, &m_OrderActionField, (int)((char *)&(m_OrderActionField.Remark) - (char *)&m_OrderActionField));
		m_OrderAction.linkMember((CMember*)NULL);
		m_OrderAction.linkOrder((COrder*)NULL);
		m_OrderAction.linkTriggerOrder((CTriggerOrder*)NULL);

		if (m_OrderAction.OrderID.isNull() && m_OrderAction.OrderLocalID.isNull())
		{
			if (m_OrderAction.MemberID.isNull())
			{
				if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
					success = m_pErrorEngine->reportError(ERROR_ONLY_TRADE_MANAGER_CAN_DO_THIS, "MemberIDisNull");
				else
					success = m_pService->orderActionBatchManager(&m_OrderAction, m_pTransaction);
			}
			else
			{
				CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, m_OrderActionField.LocalID, m_pTransaction, m_OrderActionField.MemberID, NULL);
				if (pMember == NULL)
					return RspError(nRecvTNo, pSession, &m_OrderActionField, &(m_OrderActionField.m_Describe));
				m_OrderAction.linkMember(pMember);

				if (pMember->MemberType == PT_InnerMarketMaker)
					success = m_pService->orderActionBatchManager(&m_OrderAction, m_pTransaction);
				else
				{
					if (m_OrderAction.ActionFlag == AF_Delete && !m_OrderAction.Price.isNull())
						success = m_pService->orderActionBatchPrice(&m_OrderAction, m_pTransaction);
					else
						success = m_pService->orderActionBatch(&m_OrderAction, m_pTransaction);
				}
			}

			if (!success)
				return RspError(nRecvTNo, pSession, &m_OrderActionField, &(m_OrderActionField.m_Describe));

			m_pServiceResponser->SendResponse(nRecvTNo, &m_OrderActionField, &(m_OrderActionField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		else
		{
			CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, m_OrderActionField.LocalID, m_pTransaction, m_OrderActionField.MemberID, NULL);
			if (pMember == NULL)
				return RspError(nRecvTNo, pSession, &m_OrderActionField, &(m_OrderActionField.m_Describe));
			m_OrderAction.linkMember(pMember);

			success = m_pService->orderAction(&m_OrderAction, m_pTransaction);
			if (!success)
				return RspError(nRecvTNo, pSession, &m_OrderActionField, &(m_OrderActionField.m_Describe));
			//COrderField outfield;
			//CopyOrderEntity(&m_OrderField, theStruct.getOrder());
			memcpy(&m_OrderField, m_OrderAction.getOrder(), (int)((char *)&(m_OrderAction.getOrder()->CreateTime) - (char *)m_OrderAction.getOrder()));

			if (m_OrderAction.getOrder()->APPID.isNull())
			{
				m_pServiceResponser->SendResponse(TID_RecvOrderAction, &m_OrderField, &(m_OrderField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			}
			else
			{
				m_pServiceResponser->SendResponse(TID_RecvOrderAction, &m_OrderField, &(m_OrderField.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
				if (m_OrderAction.getOrder()->getPosition() != NULL)
				{
					//CPositionField positionField;
					//CopyPositionEntity(&m_PositionField, m_OrderAction.getOrder()->getPosition());
					memcpy(&m_PositionField, m_OrderAction.getOrder()->getPosition(), (int)((char *)&(m_OrderAction.getOrder()->getPosition()->Remark) - (char *)m_OrderAction.getOrder()->getPosition()));
					m_pServiceResponser->SendNext(&m_PositionField, &(m_PositionField.m_Describe), OUTFOG_CHAIN_CONTINUE);
				}
			}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		}
		break;
	}
	//case TID_SendQuoteAction:
	//{
	//	nRecvTNo = TID_RecvQuoteAction;
	//	CQuoteActionField field;
	//	CQuoteActionField::m_Describe.getSingleField(&field, pStream);
	//	CWriteableQuoteAction theStruct;
	//	theStruct.init();
	//	CopyQuoteActionEntity(&theStruct, &field);
	//	success = m_pService->quoteAction(&theStruct, &m_Transaction);
	//	if (!success)
	//		return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

	//	CQuoteField outfield;
	//	CQuote *pQuote = theStruct.getQuote();
	//	if (pQuote != NULL)
	//		CopyQuoteEntity(&outfield, pQuote);
	//	m_pServiceResponser->SendResponse(TID_RecvQuoteAction, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
	//	break;
	//}
	case TID_PushTimeSync:
	{
		CCurrentTimeField field;
		CCurrentTimeField::m_Describe.getSingleField(&field, pStream);
		CWriteableCurrentTime theStruct;
		theStruct.init();
		CopyCurrentTimeEntity(&theStruct, &field);
		success = m_pService->timeSync(&theStruct);
		break;
	}
	case TID_SendPositionAction:
	{
		nRecvTNo = TID_RecvPositionAction;
		CPositionActionField field;
		CPositionActionField::m_Describe.getSingleField(&field, pStream);

		CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		if (field.ActionType == PAT_CloseAll || field.ActionType == PAT_CloseMax)
		{
			success = m_pService->ReqPositionCloseBatch(&field, m_pTransaction);
			if (!success)
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		else
		{
			//先注释掉，用于测试净仓的分仓逐仓是否符合预期
			//净仓剔除全仓分仓，逐仓
			//if (pMember->PositionType == PT_Net)
			//{
			//	//净仓全仓不支持分仓
			//	if (field.TradeUnitID != field.AccountID)
			//	{
			//		m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitID[%s]", field.TradeUnitID.getValue());
			//		return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			//	}
			//	//净仓不支持逐仓
			//	if (!field.IsCrossMargin.isNull() && !field.IsCrossMargin)
			//	{
			//		m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitID[%s],IsCrossMargin[%d]", field.TradeUnitID.getValue(), field.IsCrossMargin.getValue());
			//		return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			//	}
			//}
			//如果指定了持仓，直接调用，如果没有指定持仓则使用批量操作+持仓汇总修改
			if (field.TradeUnitID.isNull() || field.TradeUnitID == "default")
			{
				if (field.AccountID.isNull())
					field.AccountID = field.MemberID;
				if (field.ActionType == PAT_ChangeDefaultLeverage)
				{
					CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
					if (pInstrument != NULL)
					{
						CLeverageType maxLeverage = 1.0;
						if (pMember->PositionType == PT_Net)
						{
							maxLeverage = queryMaxLeverage(m_pMDB, pInstrument, field.MemberID, field.AccountID, field.TradeUnitID, PD_Net);
						}
						else {
							CLeverageType maxLongLeverage = queryMaxLeverage(m_pMDB, pInstrument, field.MemberID, field.AccountID, field.TradeUnitID, PD_Long);
							CLeverageType maxShortLeverage = queryMaxLeverage(m_pMDB, pInstrument, field.MemberID, field.AccountID, field.TradeUnitID, PD_Short);
							maxLeverage = maxLongLeverage < maxShortLeverage ? maxLongLeverage : maxShortLeverage;
						}
						if (maxLeverage < field.Amount)
						{
							m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "maxLeverage[%s]<Amount[%s]", maxLeverage.getString().c_str(), field.Amount.getString().c_str());
							return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
						}
					}
				}

				//if (field.ActionType == PAT_ChangeMaxLeverage || field.ActionType == PAT_ChangeLeverage || field.ActionType == PAT_ChangeAllCross)
				//{
				if (!m_pService->ReqPositionActionBatch(&field, m_pTransaction))
					return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
				//}

				if (field.PosiDirection.isNull() && pMember->PositionType == PT_Net)
					field.PosiDirection = PD_Net;

				CMemberPositionField outfield;
				if (field.PosiDirection.isNull())
				{
					field.PosiDirection = PD_Long;
					if (!m_pService->ChangeMemberPosition(&field, &outfield, m_pTransaction))
						return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
					field.PosiDirection = PD_Short;
					CMemberPositionField outfield2;
					if (!m_pService->ChangeMemberPosition(&field, &outfield2, m_pTransaction))
						return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
					m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
					m_pServiceResponser->SendNext(&outfield2, &(outfield2.m_Describe), OUTFOG_CHAIN_CONTINUE);
				}
				else
				{
					if (!m_pService->ChangeMemberPosition(&field, &outfield, m_pTransaction))
						return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
					m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
				}
				//m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
			}
			else
			{
				if (field.PosiDirection.isNull() && pMember->PositionType == PT_Net)
					field.PosiDirection = PD_Net;

				CPositionField outfield;
				if (field.PosiDirection.isNull())
				{
					field.PosiDirection = PD_Long;
					if (!m_pService->ReqPositionAction(&field, &outfield, m_pTransaction))
						return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
					field.PosiDirection = PD_Short;
					CPositionField outfield2;
					if (!m_pService->ReqPositionAction(&field, &outfield2, m_pTransaction))
						return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
					m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
					m_pServiceResponser->SendNext(&outfield2, &(outfield2.m_Describe), OUTFOG_CHAIN_CONTINUE);
				}
				else
				{
					if (!m_pService->ReqPositionAction(&field, &outfield, m_pTransaction))
						return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
					m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
				}
				//m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
			}
			switch (field.ActionType)
			{
			case PAT_ChangeDefaultLeverage:
			case PAT_ChangeLeverage:
			case PAT_ChangeMaxLeverage:
			{
				CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
				if (pInstrument != NULL)
				{
					CLeverageField rspField;
					CopyQryLeverageEntity(&rspField, &field);
					queryMarginAndLeverage(m_pMDB, m_pErrorEngine, pInstrument, field.MemberID, field.AccountID, field.TradeUnitID, 0, &rspField, 0, 0, 0);
					m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
				}
			}
			}

			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		}
		break;
	}
	case TID_SendOTCTradeInsert:
	{
		nRecvTNo = TID_RecvOTCTradeInsert;
		COTCTradeInsertField field;
		field.m_Describe.getSingleField(&field, pStream);
		if (field.OTCType.isNull())
			field.OTCType = OTCT_WashOnly;

		if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		COTCTradeField outfield;
		CopyOTCTradeInsertEntity(&outfield, &field);
		if (outfield.OTCType == OTCT_WashOnly)
			success = m_pService->insertWashOnly(&outfield, m_pTransaction);
		else
			success = m_pService->insertOTCTrade(&outfield, m_pTransaction);

		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		m_pServiceResponser->SendResponse(TID_RecvOTCTradeInsert, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendMarketDataUpdate:
	{
		nRecvTNo = TID_RecvMarketDataUpdate;
		CMarketDataUpdateField field;
		field.m_Describe.getSingleField(&field, pStream);
		if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if (pInstrument == NULL)
			return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND, field.InstrumentID.getValue());

		CMarketData* pMarketData = m_pService->ReqMarketDataUpdate(pInstrument, &field, m_pTransaction);
		if (pMarketData == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		CMarketDataField outfield;
		CopyMarketDataEntity(&outfield, pMarketData);
		m_pServiceResponser->SendResponse(TID_RecvMarketDataUpdate, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendInstrumentUpdate:
	{
		nRecvTNo = TID_RecvInstrumentUpdate;
		CInstrumentUpdateField field;
		field.m_Describe.getSingleField(&field, pStream);
		if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if (pInstrument == NULL)
			return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND, field.InstrumentID.getValue());

		CWriteableInstrument theInstrument;
		m_pMDB->m_InstrumentFactory->retrieve(pInstrument, &theInstrument);
		if (!field.PriceTick.isNull())
			theInstrument.PriceTick = field.PriceTick;
		if (!field.VolumeTick.isNull())
			theInstrument.VolumeTick = field.VolumeTick;
		if (!field.MinOrderCost.isNull())
			theInstrument.MinOrderCost = field.MinOrderCost;
		if (!field.MinOrderVolume.isNull())
			theInstrument.MinOrderVolume = field.MinOrderVolume;
		if (!field.MaxOpenInterest.isNull())
			theInstrument.MaxOpenInterest = field.MaxOpenInterest;
		if (!field.MaxOrderVolume.isNull())
			theInstrument.MaxOrderVolume = field.MaxOrderVolume;
		if (!field.BasisPrice.isNull())
			theInstrument.BasisPrice = field.BasisPrice;
		m_pMDB->m_InstrumentFactory->update(pInstrument, &theInstrument);
		CInstrumentField outfield;
		CopyInstrumentEntity(&outfield, &theInstrument);
		m_pServiceResponser->SendResponse(TID_RecvMarketDataUpdate, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);

		break;
	}
	case TID_SendPositionMerge:
	{
		nRecvTNo = TID_RecvPositionMerge;
		CPositionMergeField field;
		CPositionMergeField::m_Describe.getSingleField(&field, pStream);

		CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		CPositionField outfield;
		success = m_pService->ReqPositionMerge(&field, &outfield, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendResetLocalID:
	{
		nRecvTNo = TID_RecvResetLocalID;
		CResetLocalIDField field;
		CResetLocalIDField::m_Describe.getSingleField(&field, pStream);

		if (!field.MemberID.isNull())
		{
			CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
			if (pMember != NULL)
			{
				CWriteableMember theMember;
				m_pMDB->m_MemberFactory->retrieve(pMember, &theMember);
				theMember.MaxLocalID.clear();
				m_pMDB->m_MemberFactory->update(pMember, &theMember);
			}
		}

		if (!field.AccountID.isNull())
		{
			CAccount *pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(field.MemberID, field.AccountID, field.SettlementGroup, field.Currency);
			if (pAccount != NULL)
			{
				CWriteableAccount theAccount;
				m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
				theAccount.MaxLocalID.clear();
				m_pMDB->m_AccountFactory->update(pAccount, &theAccount);
			}
		}

		if (!field.Token.isNull())
		{
			CUserSession *pUserSession = m_pMDB->m_UserSessionFactory->findByToken(field.Token);
			if (pUserSession != NULL)
			{
				CWriteableUserSession theUserSession;
				m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
				theUserSession.MaxLocalID.clear();
				m_pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession);
			}
		}

		if (!field.UserID.isNull())
		{
			CUserSession *pUserSession = m_pMDB->m_UserSessionFactory->findByUserIDAndAppID(field.UserID, field.APPID);
			if (pUserSession != NULL)
			{
				CWriteableUserSession theUserSession;
				m_pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
				theUserSession.MaxLocalID.clear();
				m_pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession);
			}
		}

		m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		success = true;
		break;
	}
	case TID_SendClearMarket:
	{
		nRecvTNo = TID_RecvClearMarket;
		CClearMarketField field;
		CClearMarketField::m_Describe.getSingleField(&field, pStream);
		if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		success = m_pService->ClearMarket(&field, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendClearLastKLine:
	{
		nRecvTNo = TID_RecvClearLastKLine;
		CClearLastKLineField field;
		CClearLastKLineField::m_Describe.getSingleField(&field, pStream);
		if (!CheckManagerSession(m_pMDB, m_pErrorEngine, pSession))
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		success = m_pService->ClearLastKLine(&field, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendCloseOrderAction:
	{
		nRecvTNo = TID_RecvCloseOrderAction;
		CCloseOrderActionField field;
		CCloseOrderActionField::m_Describe.getSingleField(&field, pStream);

		CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		switch (field.ActionFlag)
		{
		case AF_Delete:
		{
			break;
		}
		case AF_Modify:
		{
			if (field.OrderID.isNull())
			{
				m_pErrorEngine->reportError(ERROR_BAD_FIELD, "OrderID cannot be null");
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			}
			break;
		}
		default:
		{
			m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		}

		CCloseOrderActionField theCloseOrderAction;
		//theCloseOrderAction.init();
		CopyCloseOrderActionEntity(&theCloseOrderAction, &field);
		//theCloseOrderAction.linkMember(pMember);

		if (field.OrderID.isNull() && field.OrderLocalID.isNull())
		{
			success = m_pService->closeOrderActionBatch(&theCloseOrderAction, m_pTransaction);
			if (!success)
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
			m_pServiceResponser->SendResponse(nRecvTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		else
		{
			COrder* pOrder = m_pService->closeOrderAction(&theCloseOrderAction, m_pTransaction);
			if (pOrder == NULL)
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

			COrderField outfield;
			CopyOrderEntity(&outfield, pOrder);

			m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
			//if (theCloseOrderAction.getTriggerOrder() != NULL)
			//{
			//	CTriggerOrderField triggerOrderField;
			//	CopyTriggerOrderEntity(&triggerOrderField, theCloseOrderAction.getTriggerOrder());
			//	m_pServiceResponser->SendNext(&triggerOrderField, &(triggerOrderField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			//}
			//if (theCloseOrderAction.getOrder()->getPosition() != NULL)
			//{
			//	CPositionField positionField;
			//	CopyPositionEntity(&positionField, theCloseOrderAction.getOrder()->getPosition());
			//	m_pServiceResponser->SendNext(&positionField, &(positionField.m_Describe), OUTFOG_CHAIN_CONTINUE);
			//}
			m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		}
		break;
	}
	case TID_SendPositionReverse:
	{
		nRecvTNo = TID_RecvPositionReverse;
		CPositionReverseField field;
		CPositionReverseField::m_Describe.getSingleField(&field, pStream);

		//为了TradeServce的服务能被不同的服务调用，所以检查和Session相关的操作需要在外面做掉
		CMember* pMember;

		CWriteableOrder theOutOrder;
		theOutOrder.init(m_pMDB);
		if (!field.APPID.isNull())
		{
			theOutOrder.APPID = field.APPID;
			pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
		}
		else
		{
			pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, &(theOutOrder.APPID));
			if (!theOutOrder.APPID.isNull()) {
				field.APPID = theOutOrder.APPID;
			}
		}
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		success = m_pService->ReqPositionReverse(&field, &theOutOrder, pMember, pSession, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		//
		COrderField outfield;
		CopyOrderEntity(&outfield, &theOutOrder);
		m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
		if (theOutOrder.getPosition() != NULL)
		{
			CPositionField positionField;
			CopyPositionEntity(&positionField, theOutOrder.getPosition());
			m_pServiceResponser->SendNext(&positionField, &(positionField.m_Describe), OUTFOG_CHAIN_CONTINUE);
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendMemberAction:
	{
		nRecvTNo = TID_RecvMemberAction;
		CMemberActionField field;
		CMemberActionField::m_Describe.getSingleField(&field, pStream);

		CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		success = m_pService->ReqMemberAction(&field, pMember, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		CMemberField theMember;
		CopyMemberEntity(&theMember, pMember);
		m_pServiceResponser->SendResponse(nRecvTNo, &theMember, &(theMember.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendPositionLeverage:
	{
		nRecvTNo = TID_RecvPositionLeverage;
		CPositionActionField field;
		CPositionActionField::m_Describe.getSingleField(&field, pStream);

		CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		if (pMember->PositionType == PT_Net) {
			m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "PT_Net is not support");
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		switch (field.ActionType)
		{
		case PAT_ChangeDefaultLeverage:
		case PAT_ChangeLeverage:
		case PAT_ChangeMaxLeverage:
		{
			break;
		}
		default:
			m_pErrorEngine->reportError(ERROR_INVALID_ACTION, "ActionTypeNot[1/5/6]");
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		//如果指定了持仓，直接调用，如果没有指定持仓则使用批量操作+持仓汇总修改
		if (field.NetLeverage.isNull())
			field.NetLeverage = 0;
		if (field.LongLeverage.isNull())
			field.LongLeverage = 0;
		if (field.ShortLeverage.isNull())
			field.ShortLeverage = 0;

		if (field.TradeUnitID.isNull() || field.TradeUnitID == "default")
		{
			if (field.AccountID.isNull())
				field.AccountID = field.MemberID;
			if (field.ActionType == PAT_ChangeDefaultLeverage)
			{
				CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
				if (pInstrument != NULL)
				{
					//CLeverageType maxLeverage = 1.0;
					if (field.LongLeverage >= 1)
					{
						CLeverageType maxLongLeverage = queryMaxLeverage(m_pMDB, pInstrument, field.MemberID, field.AccountID, field.TradeUnitID, PD_Long);
						if (maxLongLeverage < field.LongLeverage)
						{
							m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "maxLongLeverage[%s]<LongLeverage[%s]", maxLongLeverage.getString().c_str(), field.LongLeverage.getString().c_str());
							return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
						}
					}
					if (field.ShortLeverage >= 1)
					{
						CLeverageType maxShortLeverage = queryMaxLeverage(m_pMDB, pInstrument, field.MemberID, field.AccountID, field.TradeUnitID, PD_Short);
						if (maxShortLeverage < field.ShortLeverage)
						{
							m_pErrorEngine->reportError(ERROR_LEVERAGE_TOO_HIGH, "maxShortLeverage[%s]<ShortLeverage[%s]", maxShortLeverage.getString().c_str(), field.ShortLeverage.getString().c_str());
							return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
						}
					}
				}
			}

			if (field.LongLeverage >= 1)
			{
				field.Amount = field.LongLeverage;
				field.PosiDirection = PD_Long;
				if (!m_pService->ReqPositionActionBatch(&field, m_pTransaction))
					return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
				CMemberPositionField outfield;
				if (!m_pService->ChangeMemberPosition(&field, &outfield, m_pTransaction))
					return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
				m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
			}
			if (field.ShortLeverage >= 1)
			{
				field.Amount = field.ShortLeverage;
				field.PosiDirection = PD_Short;
				if (!m_pService->ReqPositionActionBatch(&field, m_pTransaction))
					return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
				CMemberPositionField outfield;
				if (!m_pService->ChangeMemberPosition(&field, &outfield, m_pTransaction))
					return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
				if (field.LongLeverage >= 1)
				{
					m_pServiceResponser->SendNext(&outfield, &(outfield.m_Describe), OUTFOG_CHAIN_CONTINUE);
				}
				else
				{
					m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
				}
			}
		}
		else
		{
			if (field.LongLeverage >= 1)
			{
				field.Amount = field.LongLeverage;
				field.PosiDirection = PD_Long;
				// if (!m_pService->ReqPositionActionBatch(&field, m_pTransaction))
				// 	return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
				CPositionField outfield;
				if (!m_pService->ReqPositionAction(&field, &outfield, m_pTransaction))
					return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
				m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
			}
			if (field.ShortLeverage >= 1)
			{
				field.Amount = field.ShortLeverage;
				field.PosiDirection = PD_Short;
				// if (!m_pService->ReqPositionActionBatch(&field, &m_Transaction))
				// 	return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
				CPositionField outfield;
				if (!m_pService->ReqPositionAction(&field, &outfield, m_pTransaction))
					return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
				if (field.LongLeverage >= 1)
				{
					m_pServiceResponser->SendNext(&outfield, &(outfield.m_Describe), OUTFOG_CHAIN_CONTINUE);
				}
				else
				{
					m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
				}
			}
		}

		CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(field.ExchangeID, field.InstrumentID);
		if (pInstrument != NULL)
		{
			CLeverageField rspField;
			CopyQryLeverageEntity(&rspField, &field);
			queryMarginAndLeverage(m_pMDB, m_pErrorEngine, pInstrument, field.MemberID, field.AccountID, field.TradeUnitID, 0, &rspField, 0, 0, 0);
			m_pServiceResponser->SendNext(&rspField, &(rspField.m_Describe), OUTFOG_CHAIN_CONTINUE);
		}
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		//}
		break;
	}
	default:
		return true;
	}

	if (success)
		m_pTransaction->commit();
	else
		m_pTransaction->rollback();

	//m_pService->AdditionalPost(m_pTransaction);
	//为了让上层退出，不继续其他Service了
	return false;
}

bool CTradeServiceInterface::RspError(int nRecvTNo, TSessionRef* pSession, void *pField, CFieldDescribe *pFieldDescribe)
{
	char* errMsg = NULL;
	int errorCode = 0;
	m_pErrorEngine->getLastError(&errorCode, &errMsg);

	switch (nRecvTNo)
	{
	case TID_RecvCloseOrderInsert:
	case TID_RecvOrderInsert:
	case TID_RecvTriggerOrderInsert:
	case TID_RecvOTCTradeInsert:
	{
		m_handleOrderInsertErrorIndex->incValue();
		if (m_InsertFailedLog != NULL)
		{
			fprintf(m_InsertFailedLog, "%s,%0.lf,%s:", pFieldDescribe->GetFieldName(), g_nCurrentTime, errMsg);
			pFieldDescribe->StructToJson(BufferTemp, (char*)pField);
			fprintf(m_InsertFailedLog, "%s\n", BufferTemp);
			fflush(m_InsertFailedLog);
		}
		break;
	}
	case TID_RecvOrderAction:
	case TID_RecvTriggerOrderAction:
	{
		m_handleOrderActionErrorIndex->incValue();
		//if (m_ActionFailedLog != NULL)
		//{
		//	fprintf(m_ActionFailedLog, "%s,%0.lf,%s:", pFieldDescribe->GetFieldName(), g_nCurrentTime, errMsg);
		//	pFieldDescribe->StructToJson(BufferTemp, (char*)pField);
		//	fprintf(m_ActionFailedLog, "%s\n", BufferTemp);
		//	fflush(m_ActionFailedLog);
		//}
		break;
	}
	}

	if (pFieldDescribe == NULL)
	{
		CErrorField responseField;
		responseField.ErrorNo = errorCode;
		responseField.ErrorMsg = errMsg;
		m_pServiceResponser->SendResponse(nRecvTNo, &responseField, &(responseField.m_Describe), pSession, OUTFOG_CHAIN_SINGLE, errorCode, errMsg);
	}
	else
	{
		m_pServiceResponser->SendResponse(nRecvTNo, pField, pFieldDescribe, pSession, OUTFOG_CHAIN_SINGLE, errorCode, errMsg);
	}
	m_pTransaction->rollback();
	return false;
}

bool CTradeServiceInterface::handleRequest(const TOutFogMetaRef* pOutFogReqRef, const void *pStream, TSessionRef* pSession)
{
	return true;
}

void CTradeServiceInterface::CancleOtherOrder(CInstrument *pInstrument, CPriceType Price, CDirectionType Direction)
{
	m_OrderAction.ActionFlag = AF_Delete;
	if (Direction == D_Buy)
	{
		COrderIterator* pCountPartyIterator = m_pService->m_pOrderBookList->getOrderIterator(pInstrument, D_Sell);
		COrder* pCountParty = pCountPartyIterator->next();
		while (pCountParty != NULL)
		{
			if (pCountParty->Price > Price)
				break;

			if (pCountParty->Priority != 0) {
				pCountParty = pCountPartyIterator->next();
				continue;
			}
			CTransaction theTransaction;
			m_OrderAction.OrderID = pCountParty->OrderID;
			m_OrderAction.linkMember(pCountParty->getMember());
			m_OrderAction.linkOrder(pCountParty);
			m_OrderAction.linkTriggerOrder((CTriggerOrder*)NULL);
			pCountParty = pCountPartyIterator->next();
			if (m_pService->orderAction(&m_OrderAction, &theTransaction))
				theTransaction.commit();
			else
				theTransaction.rollback();
		}
		pCountPartyIterator->free();
	}
	else
	{
		COrderIterator* pCountPartyIterator = m_pService->m_pOrderBookList->getOrderIterator(pInstrument, D_Buy);
		COrder* pCountParty = pCountPartyIterator->next();
		while (pCountParty != NULL)
		{
			if (pCountParty->Price < Price)
				break;

			if (pCountParty->Priority != 0) {
				pCountParty = pCountPartyIterator->next();
				continue;
			}
			CTransaction theTransaction;
			m_OrderAction.OrderID = pCountParty->OrderID;
			m_OrderAction.linkMember(pCountParty->getMember());
			m_OrderAction.linkOrder(pCountParty);
			m_OrderAction.linkTriggerOrder((CTriggerOrder*)NULL);
			pCountParty = pCountPartyIterator->next();
			if (m_pService->orderAction(&m_OrderAction, &theTransaction))
				theTransaction.commit();
			else
				theTransaction.rollback();
		}
		pCountPartyIterator->free();
	}
}

//bool CTradeServiceInterface::CancleMyOrder(CInstrument *pInstrument, CLongIndexIDType MemberID, CPriceType Price, CDirectionType Direction)
//{
//	bool bSuccess = false;
//	m_OrderAction.ActionFlag = AF_Delete;
//	if (Direction == D_Buy)
//	{
//		COrderIterator* pCountPartyIterator = m_pService->m_pOrderBookList->getOrderIterator(pInstrument, Direction);
//		COrder* pCountParty = pCountPartyIterator->next();
//		while (pCountParty != NULL)
//		{
//			if (pCountParty->Price < Price)
//				break;
//			if (pCountParty->Price != Price) {
//				pCountParty = pCountPartyIterator->next();
//				continue;
//			}
//			if (pCountParty->MemberID != MemberID) {
//				pCountParty = pCountPartyIterator->next();
//				continue;
//			}
//			CTransaction theTransaction;
//			m_OrderAction.OrderID = pCountParty->OrderID;
//			m_OrderAction.linkMember(pCountParty->getMember());
//			m_OrderAction.linkOrder(pCountParty);
//			m_OrderAction.linkTriggerOrder((CTriggerOrder*)NULL);
//			pCountParty = pCountPartyIterator->next();
//			if (m_pService->orderAction(&m_OrderAction, &theTransaction))
//				theTransaction.commit();
//			else 
//				theTransaction.rollback();
//			bSuccess = true;
//		}
//		pCountPartyIterator->free();
//	}
//	else
//	{
//		COrderIterator* pCountPartyIterator = m_pService->m_pOrderBookList->getOrderIterator(pInstrument, Direction);
//		COrder* pCountParty = pCountPartyIterator->next();
//		while (pCountParty != NULL)
//		{
//			if (pCountParty->Price > Price)
//				break;
//			if (pCountParty->Price != Price) {
//				pCountParty = pCountPartyIterator->next();
//				continue;
//			}
//			if (pCountParty->MemberID != MemberID) {
//				pCountParty = pCountPartyIterator->next();
//				continue;
//			}
//			CTransaction theTransaction;
//			m_OrderAction.OrderID = pCountParty->OrderID;
//			m_OrderAction.linkMember(pCountParty->getMember());
//			m_OrderAction.linkOrder(pCountParty);
//			m_OrderAction.linkTriggerOrder((CTriggerOrder*)NULL);
//			pCountParty = pCountPartyIterator->next();
//			if (m_pService->orderAction(&m_OrderAction, &theTransaction)) 
//				theTransaction.commit();
//			else
//				theTransaction.rollback();
//			bSuccess = true;
//		}
//		pCountPartyIterator->free();
//	}
//	return bSuccess;
//}
