/////////////////////////////////////////////////////////////////////////
///@system 交易引擎系统
///@company 上海兆百网络科技有限公司
///CWedge.cpp
///实现了类CWedge
/////////////////////////////////////////////////////////////////////////

#include "CWedge.h"

CWedge::CWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
{
	m_TradeService = pTradeService;
	m_pMDB = pMDB;
	m_pErrorEngine = pErrorEngine;
}

CWedge::~CWedge(void)
{
}

//void CWedge::internalWarning(char *msg)
//{
//	::internalWarning(m_pErrorEngine,msg);
//}

int CWedge::getUse(void)
{
	int result = 0;
	/*
	if (this->insertOrder!=CWedge::insertOrder)
	{
		result|=USE_insertOrder;
	}
	if (this->orderAction!=CWedge::orderAction)
	{
		result|=USE_orderAction;
	}
	if (this->insertSingleTrade!=CWedge::insertSingleTrade)
	{
		result|=USE_insertSingleTrade;
	}
	if (this->updateInstrumentStatus!=CWedge::updateInstrumentStatus)
	{
		result|=USE_updateInstrumentStatus;
	}
	if (this->timeSync!=CWedge::timeSync)
	{
		result|=USE_timeSync;
	}
	if (this->tryTradeAfterOrderInsert!=CWedge::tryTradeAfterOrderInsert)
	{
		result|=USE_tryTradeAfterOrderInsert;
	}
	if (this->doAfterOrderChange!=CWedge::doAfterOrderChange)
	{
		result|=USE_doAfterOrderChange;
	}
	if (this->changePosition!=CWedge::changePosition)
	{
		result|=USE_changePosition;
	}
	*/
	result |= USE_insertOrder;
	result |= USE_orderAction;
	result |= USE_insertSingleTrade;
	result |= USE_updateInstrumentStatus;
	result |= USE_timeSync;
	result |= USE_tryTradeAfterOrderInsert;
	result |= USE_doAfterOrderChange;
	result |= USE_changePosition;
	return result;
}

const char * CWedge::getName(void)
{
	return m_sName.c_str();
}

void internalWarning(CErrorEngine *pErrorEngine, char *msg)
{
	int errorID;
	char *errorMsg;
	pErrorEngine->getLastError(&errorID, &errorMsg);
	//	BIZ_EXCEPTION_LOG3("(trading engine warning:%s:%d:%s)",msg,errorID,errorMsg);
}

