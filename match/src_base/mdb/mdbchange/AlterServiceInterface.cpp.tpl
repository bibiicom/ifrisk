!!function!!
int isDbmtPackage(char *usage)
{
	if (strcmp(usage,"InitTable") == 0)
		return 1;
	return 0;
}

!!endfunction!!
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
!!enter XTP!!	
!!travel fields!!
!!if valid_name("usage") && isDbmtPackage(@usage)!!
	!!let tableName=@name!!
	case FID_!!@name!!:
	{
		C!!@name!!Field field;
		C!!@name!!Field::m_Describe.getSingleField(&field, pStream);
		success = m_pService->Sync!!@tableName!!Impl(pOutFogReqRef->MessageNo, &field, m_pTransaction);
		if (pSession != NULL)
		{
			if(success)
				m_pServiceResponser->SendResponse(RspTNo, &field, &(field.m_Describe), pSession, OUTFOG_CHAIN_SINGLE);
			else
				RspError(m_pErrorEngine, m_pServiceResponser, RspTNo, pSession, &field, &(field.m_Describe));
		}
		break;
	}
!!endif!!
!!next!!
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
