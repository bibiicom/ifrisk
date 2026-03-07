#include "Engine.h"
#include "FieldData.h"

CEngine::CEngine() :CBaseEngine()
{
	m_seqID = 1;
}

CEngine::~CEngine(void)
{
}

void CEngine::handleMessage(const TOutFogMetaRef* pOutFogReqRef, const void *pStream, TSessionRef* pSession)
{
	//if (pSession->frontID.getValue()!=0 || pSession->sessionID.getValue()!=0)
	//printf("Engine::FrontID=%d sessionID=%ld\n",pSession->frontID.getValue(),pSession->sessionID.getValue());

	iterator it;

	for (it = begin(); it < end(); it++)
	{
		if (!(*it)->handleMessage(pOutFogReqRef, pStream, pSession))
		{
			break;
		}
	}

	//	m_pMDB->save(m_pConfig);
}

void CEngine::handleRequest(const TOutFogMetaRef* pOutFogReqRef, const void *pStream, TSessionRef* pSession)
{
	iterator it;
	for (it = begin(); it < end(); it++)
	{
		(*it)->handleRequest(pOutFogReqRef, pStream, pSession);
	}
}

void CEngine::addServiceInterface(IServiceInterface *pLink)
{
	pLink->m_pTransaction = &m_Transaction;
	pLink->m_pMDB = m_pMDB;
	pLink->m_pErrorEngine = m_pErrorEngine;
	push_back(pLink);
}

bool CEngine::removeServiceInterface(IServiceInterface *pLink)
{
	iterator it;
	for (it = begin(); it < end(); it++)
	{
		if (*it == pLink)
		{
			erase(it);
			return true;
		}
	}
	return false;
}

