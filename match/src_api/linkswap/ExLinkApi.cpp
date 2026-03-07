#include "ExLinkApi.h"

CExLinkApi::CExLinkApi(COutFogServiceCallback *pCallback):COutFogServiceApi(pCallback)
{
	m_bLogin = false;
	m_bConnect = false;	
	
	m_bRevRtn = false;
	m_bCheckLinked =false;
	memset(&m_LinkField,0,sizeof(CLinkField));
	memset(m_sIdInfo,0,sizeof(m_sIdInfo));
	m_bLogin = false;
	
	m_iReqSeqNo = 0;
	m_DisconnectNum = 0;

	memset(&m_OutFogMetaRef,0,sizeof(TOutFogMetaRef));
	m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
}

bool CExLinkApi::setPassword(CLinkField *pLinkField)
{
	switch(pLinkField->EncryptType)
	{
	case ET_Base64:
		{
			int len = strlen(pLinkField->LinkPassword);
			pLinkField->LinkPassword = base64_decode(pLinkField->LinkPassword.getValue(), len);
			break;
		}
	default:
		{
//			REPORT_EVENT(LOG_CRITICAL, "getPassword", "no PasswordEnType %c ",pLinkField->EncryptType);
		}
	}
	return true;
}

void CExLinkApi::InitLink(CLinkField *pLinkField)
{
	memcpy(&m_LinkField,pLinkField,sizeof(CLinkField));
	setPassword(&m_LinkField);
	m_nLinkNo = m_LinkField.LinkNo.getValue();
	sprintf(m_sIdInfo,"LinkNo[%d],LinkUserID[%s]",m_LinkField.LinkNo.getValue(),m_LinkField.LinkUserID.getValue());
	m_OutFogMetaRef.GlobalNo = pLinkField->LinkNo.getValue();
}

void CExLinkApi::setLinkField(CUserLoginField *pField)
{
	m_LinkField.LinkPassword = pField->Password;
	m_LinkField.LinkUserID = pField->UserID;
	m_LinkField.EncryptType = ET_None;
}