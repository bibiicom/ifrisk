
#include "CWedgeGroup.h"

//#define TRACE_RULE

CWedgeGroup::CWedgeGroup(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
	:CWedge(pTradeService, pMDB, pErrorEngine)
{
	m_sName = "Group";
}

CWedgeGroup::~CWedgeGroup(void)
{
}

int CWedgeGroup::getUse(void)
{
	int result = 0;
	for (int i = 0; i < MAX_RULE_ACTION; i++)
	{
		if (rules[i].size() > 0)
		{
			result |= 1 << i;
		}
	}
	return result;
}

bool CWedgeGroup::insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
{
	vector<CWedge *>::iterator it;

	for (it = rules[R_insertOrder].begin(); it < rules[R_insertOrder].end(); it++)
	{
		if (!(*it)->insertOrder(pOrder, pTransaction))
		{
			return false;
		}
	}
	return true;
}

//bool CWedgeGroup::insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction* pTransaction)
//{
//	vector<CWedge *>::iterator it;
//
//	for (it = rules[R_insertCombOrder].begin(); it < rules[R_insertCombOrder].end(); it++)
//	{
//		if (!(*it)->insertCombOrder(pCombOrder, pTransaction))
//		{
//			return false;
//		}
//	}
//	return true;
//}


bool CWedgeGroup::orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction)
{
	vector<CWedge *>::iterator it;

	for (it = rules[R_orderAction].begin(); it < rules[R_orderAction].end(); it++)
	{
		if (!(*it)->orderAction(pOrderAction, pTransaction))
		{
			return false;
		}
	}
	return true;
}

bool CWedgeGroup::insertSingleTrade(CWriteableTrade *pTrade, CWriteableOrder *pOrder)
{
	vector<CWedge *>::iterator it;

	for (it = rules[R_insertSingleTrade].begin(); it < rules[R_insertSingleTrade].end(); it++)
	{
		if (!(*it)->insertSingleTrade(pTrade, pOrder))
		{
			return false;
		}
	}
	return true;
}

bool CWedgeGroup::updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction)
{
	vector<CWedge *>::iterator it;

	for (it = rules[R_updateInstrumentStatus].begin(); it < rules[R_updateInstrumentStatus].end(); it++)
	{
		if (!(*it)->updateInstrumentStatus(pInstrumentStatus, pTransaction))
		{
			return false;
		}
	}
	return true;
}

bool CWedgeGroup::timeSync(CWriteableCurrentTime *pCurrentTime)
{
	vector<CWedge *>::iterator it;

	for (it = rules[R_timeSync].begin(); it < rules[R_timeSync].end(); it++)
	{
		if (!(*it)->timeSync(pCurrentTime))
		{
			return false;
		}
	}
	return true;
}

void CWedgeGroup::tryTradeAfterOrderInsert(COrder *pOrder)
{
	vector<CWedge *>::iterator it;

	for (it = rules[R_tryTradeAfterOrderInsert].begin(); it < rules[R_tryTradeAfterOrderInsert].end(); it++)
		(*it)->tryTradeAfterOrderInsert(pOrder);
}

//bool CWedgeGroup::tryTradeAfterCombOrderInsert(CWriteableCombOrder *pCombOrder)
//{
//	vector<CWedge *>::iterator it;
//
//	for (it = rules[R_tryTradeAfterCombOrderInsert].begin(); it < rules[R_tryTradeAfterCombOrderInsert].end(); it++)
//	{
//		if (!(*it)->tryTradeAfterCombOrderInsert(pCombOrder))
//			return false;
//	}
//	return true;
//}

bool CWedgeGroup::doAfterOrderChange(COrder *pOrder)
{
	vector<CWedge *>::iterator it;

	for (it = rules[R_doAfterOrderChange].begin(); it < rules[R_doAfterOrderChange].end(); it++)
	{
		if (!(*it)->doAfterOrderChange(pOrder))
			return false;
	}
	return true;
}

bool CWedgeGroup::changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction)
{
	vector<CWedge *>::iterator it;

	for (it = rules[R_changePosition].begin(); it < rules[R_changePosition].end(); it++)
	{
		if (!(*it)->changePosition(pChangeList, pTransaction))
			return false;
	}
	return true;
}

bool CWedgeGroup::add(CWedge *pRule)
{
	/*
	static bool isFirst=true;
	if (isFirst)
	{
		isFirst=false;
		timeMeters[0]=new CTimeMeter("change client position");
		timeMeters[1]=new CTimeMeter("insert trade");
		timeMeters[2]=new CTimeMeter("try trade");
		timeMeters[3]=new CTimeMeter("merge back");
	}
	*/

	int use;

	use = pRule->getUse();

	for (int i = 0; i < MAX_RULE_ACTION; i++)
	{
		if (use & (1 << i))
		{
			rules[i].push_back(pRule);
		}
	}
	/*
		for (int i=0;i<MAX_RULE_ACTION;i++)
		{
			printf("%d,",rules[i].size());
		}
		printf("\n");
	*/
	return true;
}
/*
bool CWedgeGroup::remove(CWedge *pRule, CWedgeFilter *pFilter)
{
	iterator it;

	for (it=begin();it<end();it++)
	{
		if ((it->pRule==pRule)&&(it->pFilter==pFilter))
		{
			erase(it);
			return true;
		}
	}
	return false;
}
*/
