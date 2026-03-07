#include "MyAlterServiceInterface.h"
#include "CopyField.h"
#include "monitorIndex.h"
#include "FieldData.h"

CMyAlterServiceInterface::CMyAlterServiceInterface(CMyAlterService *pService, CServiceResponser *pServiceResponser)
	:CAlterServiceInterface(pService, pServiceResponser)
{
	m_pMyService = pService;
}

CMyAlterServiceInterface::~CMyAlterServiceInterface()
{
}

bool CMyAlterServiceInterface::handleMessage(TOutFogMetaRef* pOutFogReqRef, void *pStream, TSessionRef* pSession)
{
	//switch (pOutFogReqRef->MessageNo)
	//{
	//case TID_SendDBCommand:
	//{
	//	CDBCommandField field;
	//	CDBCommandField::m_Describe.getSingleField(&field, pStream);
	//	int DbmtFieldID;
	//	if (m_pMyService->ReqDBCommand(&field, m_pDbmtSteam, DbmtFieldID, pSession, &m_Transaction))
	//	{
	//		int DbmtTid = 0;
	//		switch (field.DBActionType)
	//		{
	//		case DAT_Insert:
	//		{
	//			DbmtTid = TID_SendInsert;
	//			break;
	//		}
	//		case DAT_Update:
	//		{
	//			DbmtTid = TID_SendUpdate;
	//			break;
	//		}
	//		case DAT_Delete:
	//		{
	//			DbmtTid = TID_SendDelete;
	//			break;
	//		}
	//		case DAT_InsUpd:
	//		{
	//			DbmtTid = TID_SendInsUpd;
	//			break;
	//		}
	//		}
	//		TOutFogMetaRef DbmtOutFogReqRef;
	//		DbmtOutFogReqRef.MessageNo = DbmtTid;
	//		DbmtOutFogReqRef.FieldNo = DbmtFieldID;
	//		CAlterServiceInterface::handleMessage(&DbmtOutFogReqRef, m_pDbmtSteam, pSession);
	//	}
	//	break;
	//}
	//default:
	//{
	//	return CAlterServiceInterface::handleMessage(pOutFogReqRef, pStream, pSession);
	//}
	//}

	return CAlterServiceInterface::handleMessage(pOutFogReqRef, pStream, pSession);
	//return true;
}
