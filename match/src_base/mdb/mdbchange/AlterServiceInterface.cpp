#include "AlterServiceInterface.h"
#include "CopyField.h"
#include "monitorIndex.h"
#include "FieldData.h"
#include "ServiceShare.h"

CAlterServiceInterface::CAlterServiceInterface(CAlterService *pService, CServiceResponser *pServiceResponser)
{
	m_pService=pService;
	m_pServiceResponser = pServiceResponser;
}

CAlterServiceInterface::~CAlterServiceInterface()
{
}

bool CAlterServiceInterface::handleMessage(const TOutFogMetaRef* pOutFogReqRef,const void *pStream, TSessionRef* pSession)
{
	int RspTNo = 0;
	switch (pOutFogReqRef->MessageNo)
	{
	case TID_SendInsert:
		RspTNo = TID_RecvInsert;
		break;
	case TID_SendUpdate:
		RspTNo = TID_RecvUpdate;
		break;
	case TID_SendDelete:
		RspTNo = TID_RecvDelete;
		break;
	case TID_SendInsUpd:
		RspTNo = TID_RecvInsUpd;
		break;
	case TID_SendQryOne:
		RspTNo = TID_RecvQryOne;
		break;
	case TID_SendClear:
		RspTNo = TID_RecvClear;
		break;
	case TID_SendInit:
		RspTNo = TID_RecvInit;
		break;
	case TID_DataSyncEnd:
	{
		m_pService->afterSync(m_pTransaction);
		m_pTransaction->commit();
		return false; //为了让上层退出，不继续其他Service了
	}
	default:
	{
		return true;
	}
	}
	
	if (pSession != NULL)
	{
		if (!CheckManagerFront(m_pErrorEngine, pSession->pToken))
			return RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession);
	}
	
	bool success=false;
	switch (pOutFogReqRef->FieldNo)
	{
	case FID_ServiceConfig:
	{
		CServiceConfigField field;
		CServiceConfigField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncServiceConfigImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_Currency:
	{
		CCurrencyField field;
		CCurrencyField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncCurrencyImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_MarketData:
	{
		CMarketDataField field;
		CMarketDataField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncMarketDataImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_Instrument:
	{
		CInstrumentField field;
		CInstrumentField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncInstrumentImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_CurrentTime:
	{
		CCurrentTimeField field;
		CCurrentTimeField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncCurrentTimeImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_Member:
	{
		CMemberField field;
		CMemberField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncMemberImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_UserSession:
	{
		CUserSessionField field;
		CUserSessionField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncUserSessionImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_LastKLine:
	{
		CLastKLineField field;
		CLastKLineField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncLastKLineImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_CFDRate:
	{
		CCFDRateField field;
		CCFDRateField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncCFDRateImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_TradingRight:
	{
		CTradingRightField field;
		CTradingRightField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncTradingRightImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_MarginRate:
	{
		CMarginRateField field;
		CMarginRateField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncMarginRateImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_Fee:
	{
		CFeeField field;
		CFeeField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncFeeImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_Liquidity:
	{
		CLiquidityField field;
		CLiquidityField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncLiquidityImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_Account:
	{
		CAccountField field;
		CAccountField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncAccountImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_MemberPosition:
	{
		CMemberPositionField field;
		CMemberPositionField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncMemberPositionImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_Position:
	{
		CPositionField field;
		CPositionField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncPositionImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_Order:
	{
		COrderField field;
		COrderField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncOrderImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_TriggerOrder:
	{
		CTriggerOrderField field;
		CTriggerOrderField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncTriggerOrderImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	case FID_PositionOrder:
	{
		CPositionOrderField field;
		CPositionOrderField::m_Describe.getSingleField(&field, pStream);
		success = m_pService->SyncPositionOrderImpl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
	default:
	{
		m_pErrorEngine->reportError(ERROR_INVALID_VALUE, "FieldNo:%d", pOutFogReqRef->FieldNo);
		success = false;
	}
	}
	
	if(success)
		m_pTransaction->commit();
	else
		m_pTransaction->rollback();

	//为了让上层退出，不继续其他Service了
	return false;
}

bool CAlterServiceInterface::handleRequest(const TOutFogMetaRef* pOutFogReqRef,const void *pStream, TSessionRef* pSession)
{
	return true;
}
