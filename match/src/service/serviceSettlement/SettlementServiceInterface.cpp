#include "SettlementServiceInterface.h"
#include "CopyField.h"
#include "FieldData.h"
#include "mdbStruct.h"

CSettlementServiceInterface::CSettlementServiceInterface(CSettlementService *pService, CServiceResponser *pServiceResponser)
{
	m_pService = pService;
	m_pServiceResponser = pServiceResponser;
}

CSettlementServiceInterface::~CSettlementServiceInterface()
{
}

bool CSettlementServiceInterface::handleMessage(const TOutFogMetaRef* pOutFogReqRef, const  void *pStream, TSessionRef* pSession)
{
	bool success = true;
	int nRecvTNo = 0;

	switch (pOutFogReqRef->MessageNo)
	{
	case TID_SendAccountAction:
	{
		nRecvTNo = TID_RecvAccountAction;
		if (!CheckManagerFront(m_pErrorEngine, pSession->pToken))
			return RspError(nRecvTNo, pSession);
		CAccountActionField field;
		CAccountActionField::m_Describe.getSingleField(&field, pStream);

		if (field.IsReserve.isNull())
			field.IsReserve = false;

		if (!CheckUserLocalID(m_pMDB, m_pErrorEngine, field.LocalID, pSession->pUserID, m_pTransaction))
		{
			if (!CheckAccountLocalID(m_pMDB, m_pErrorEngine, field.LocalID, field.MemberID, field.AccountID, field.SettlementGroup, field.Currency, m_pTransaction))
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}

		CAccountField outfield;

		switch (field.ActionType)
		{
		case AAT_SetCanReduce:
		{
			CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(field.MemberID, field.AccountID, field.SettlementGroup, field.Currency);
			if (pAccount == NULL)
			{
				m_pErrorEngine->reportError(ERROR_AccountNotFound, "[AccountID:%s]IsNull", field.AccountID.getValue());
				success = false;
			}
			CWriteableAccount theAccount;
			m_pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
			theAccount.CanReduce = field.Amount;
			m_pMDB->m_AccountFactory->update(pAccount, &theAccount);
			CopyAccountEntity(&outfield, &theAccount);
			break;
		}
		default:
			success = m_pService->ReqAccountAction(&field, &outfield, m_pTransaction);
		}
		
		//success = m_pService->ReqAccountAction(&field, &outfield, m_pTransaction);

		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		//m_pServiceResponser->SendResponse(TID_RecvAccountAction, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_FIRST);
		m_pServiceResponser->SendNext(&field, &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
		break;
	}
	case TID_SendDeposit:
	{
		nRecvTNo = TID_RecvDeposit;
		CDepositField field;
		CDepositField::m_Describe.getSingleField(&field, pStream);

		CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		//开始创建订单
		//增加订单，用于记录冻结的大小
		CWriteableOrder theOrder;
		theOrder.init(m_pMDB);
		theOrder.MemberID = field.MemberID;
		theOrder.TradeUnitID = field.AccountID;
		theOrder.UserID = pSession->pUserID;
		theOrder.LocalID = field.LocalID;
		theOrder.Direction = D_Buy;
		theOrder.DeriveSource = DS_FromAccountAction;
		theOrder.Price = field.Amount;
		theOrder.Volume = 1;
		theOrder.VolumeRemain = 1;
		theOrder.Cost = field.Amount;
		theOrder.ClearCurrency = field.Currency;
		theOrder.SettlementGroup = field.SettlementGroup;
		theOrder.AccountID = field.AccountID;
		theOrder.OrderID = m_gOrderSequence.getNextValueByString();
		theOrder.Remark = field.Remark;
		m_pMDB->m_OrderFactory->add(&theOrder, m_pTransaction);

		COrderField outfield;
		CopyOrderEntity(&outfield, &theOrder);
		m_pServiceResponser->SendResponse(TID_RecvDeposit, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendWithdraw:
	{
		nRecvTNo = TID_RecvWithdraw;
		CWithdrawField field;
		CWithdrawField::m_Describe.getSingleField(&field, pStream);

		CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		//如果是0x开头的用户，需要签名，用来证实是用户的行为

		CAccountActionField accountActionField;
		//accountActionField.UserID = field.UserID;%
		accountActionField.MemberID = field.MemberID;
		accountActionField.AccountID = field.AccountID;
		accountActionField.SettlementGroup = field.SettlementGroup;
		accountActionField.OtherSettlementGroup = field.OtherSettlementGroup;
		accountActionField.ExchangeID = field.OtherSettlementGroup;
		accountActionField.InstrumentID = field.Currency;
		accountActionField.Currency = field.Currency;
		accountActionField.LocalID = field.LocalID;
		accountActionField.IsReserve = false;
		accountActionField.ActionLocalID = "";

		COrder* pOrder = m_pMDB->m_OrderFactory->findByOrderID(field.OrderID);
		if (pOrder == NULL)
		{
			accountActionField.ActionType = AAT_FrozenMoneyByOrder;
			accountActionField.Amount = field.Amount;
		}
		else
		{
			accountActionField.ActionType = AAT_FrozenToWithdraw;
			accountActionField.OrderID = field.OrderID;
		}
		CAccountField outfield;
		success = m_pService->ReqAccountAction(&accountActionField, &outfield, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		m_pServiceResponser->SendResponse(nRecvTNo, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendTransfer:
	{
		nRecvTNo = TID_RecvTransfer;
		CTransferField field;
		CTransferField::m_Describe.getSingleField(&field, pStream);

		CMember* pMember = CheckUserSessionLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction, field.MemberID, NULL);
		if (pMember == NULL)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		CAccountActionField accountActionField;
		accountActionField.IsReserve = false;
		accountActionField.ActionType = AAT_InnerTransfer;
		accountActionField.ActionLocalID = "";
		CopyTransferEntity(&accountActionField, &field);

		CAccountField outfield;
		success = m_pService->ReqAccountAction(&accountActionField, &outfield, m_pTransaction);
		if (!success)
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		m_pServiceResponser->SendResponse(TID_RecvDeposit, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		break;
	}
	case TID_SendSettlement:
	{
		nRecvTNo = TID_RecvSettlement;
		CSettlementField field;
		field.m_Describe.getSingleField(&field, pStream);

		if (!CheckManagerLocalID(m_pMDB, m_pErrorEngine, pSession, field.LocalID, m_pTransaction))
				return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));

		CWriteableSettleDetail theSettleDetail;
		theSettleDetail.init();
		theSettleDetail.SettleDetailID = m_gTradeSequence.getNextValueByString();
		theSettleDetail.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		CopySettlementEntity(&theSettleDetail, &field);
		success = m_pService->Settlement(&theSettleDetail, pOutFogReqRef->SequenceNo);
		if (success)
		{
			CSettleDetailField outfield;
			CopySettleDetailEntity(&outfield, &theSettleDetail);
			m_pServiceResponser->SendResponse(TID_RecvSettlement, &outfield, &(outfield.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
		}
		else
		{
			return RspError(nRecvTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	default:
		return true;
	}

	if (success)
		m_pTransaction->commit();
	else
		m_pTransaction->rollback();

	return false;
}

bool CSettlementServiceInterface::RspError(int nRecvTNo, TSessionRef* pSession, void *pField, CFieldDescribe *pFieldDescribe)
{
	char* errMsg = NULL;
	int errorCode = 0;
	m_pErrorEngine->getLastError(&errorCode, &errMsg);
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

bool CSettlementServiceInterface::handleRequest(const TOutFogMetaRef* pOutFogReqRef, const void* pStream, TSessionRef* pSession)
{
	return true;
}
