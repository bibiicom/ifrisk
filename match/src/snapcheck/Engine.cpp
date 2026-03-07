#include "Engine.h"
#include "FieldData.h"

CEngine::CEngine() :CBaseEngine()
{
	m_seqID = 1;
}

CEngine::~CEngine(void)
{
}

void CEngine::addServiceInterface(IServiceInterface *pLink)
{
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

