#include "Link.h"
#include "public.h"
#include "BaseFunction.h"
#include "printinfo.h"
#include "utils.h"

CLink::CLink()
{
	m_nId = 1;
	m_HttpLoop.Create();
}

CLink::~CLink()
{

}

COneMegaApi* CLink::CreateApi()
{
	return new CLink();
}

const char *COneMegaApi::GetVersion()
{
	return "1.0";
}

extern "C" COneMegaApi* CreateOneMegaApi()
{
	return new CLink();
}

void CLink::Connect()
{
	PRINT_TO_STD("LinkNo[%d]:MarketAddress[%s]", m_nLinkNo, m_HttpLoop.m_strWSBaseUrl.c_str());
}

void CLink::SetAddress(const char *pAddress)
{
	m_HttpLoop.m_strWSBaseUrl = pAddress;
	m_sAddress = pAddress;
}

void CLink::SetCallback(COneMegaCallback *pCallback)
{
	m_HttpLoop.m_pCallback = pCallback;
}

int CLink::SendInitLink(MegaLinkStruct *pLink, int nRequestNo)
{
	m_nLinkNo = pLink->LinkNo;
	m_HttpLoop.m_pCallback->OnConnected();
	m_HttpLoop.m_nLinkNo = pLink->LinkNo;
	return 0;
}

int CLink::SendUserLogin(MegaUserLoginStruct *pUserLogin, int nRequestNo)
{
	MegaUserSessionStruct field;
	memset(&field, 0, sizeof(MegaUserSessionStruct));
	strncpy(field.UserID, pUserLogin->UserID, sizeof(field.UserID));
	strncpy(field.Token, pUserLogin->Token, sizeof(field.Token));
	m_HttpLoop.m_pCallback->RecvUserLogin(NULL, &field, 0, "Success", nRequestNo, ONE_CHAIN_SINGLE);
	return 0;
}

int CLink::SendTopicAction(MegaTopicActionStruct *pTopicAction, int nRequestNo)
{
	if (m_HttpLoop.m_strWSBaseUrl == m_sAddress)
	{
		m_HttpLoop.m_strWSBaseUrl += pTopicAction->Index;
	}
	else
	{
		if (strstr(m_HttpLoop.m_strWSBaseUrl.c_str(), pTopicAction->Index) == NULL)
		{
			m_HttpLoop.m_strWSBaseUrl += ",";
			m_HttpLoop.m_strWSBaseUrl += pTopicAction->Index;
		}
	}

	m_HttpLoop.m_httpClient.SetBaseUrl(m_HttpLoop.m_strWSBaseUrl.c_str());
	PRINT_TO_STD("%s \n", m_HttpLoop.m_strWSBaseUrl.c_str());
	//m_HttpLoop.m_pCallback->RecvTopicAction(pTopicAction, 0, "Sending...", nRequestNo, ONE_CHAIN_SINGLE);
	return 0;
}
