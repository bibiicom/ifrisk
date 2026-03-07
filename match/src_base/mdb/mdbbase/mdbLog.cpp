/////////////////////////////////////////////////////////////////////////
///CMDBLog.cpp
///实现了类CMDBLog
/////////////////////////////////////////////////////////////////////////

#include "mdbLog.h"
#include "monitorIndex.h"

///本宏控制是否使用单行方式进行输出
#define SINGLE_LINE

CCurrentTimeLogTrigger::CCurrentTimeLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CCurrentTimeLogTrigger::~CCurrentTimeLogTrigger(void)
{
}

void CCurrentTimeLogTrigger::beforeAdd(CWriteableCurrentTime *pCurrentTime)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"CurrentTime,add");
		pCurrentTime->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add CurrentTime\n");
		pCurrentTime->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CCurrentTimeLogTrigger::beforeUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pNewCurrentTime)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"CurrentTime,upd");
		pNewCurrentTime->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update CurrentTime\n");
		pNewCurrentTime->dumpDiff(m_output,pCurrentTime);
#endif
		fflush(m_output);
	}
}
	
void CCurrentTimeLogTrigger::beforeRemove(CCurrentTime *pCurrentTime)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"CurrentTime,del");
		pCurrentTime->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove CurrentTime\n");
		pCurrentTime->dump(m_output);
#endif
		fflush(m_output);
	}
}

CMemberLogTrigger::CMemberLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CMemberLogTrigger::~CMemberLogTrigger(void)
{
}

void CMemberLogTrigger::beforeAdd(CWriteableMember *pMember)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Member,add");
		pMember->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add Member\n");
		pMember->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CMemberLogTrigger::beforeUpdate(CMember *pMember, CWriteableMember *pNewMember)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Member,upd");
		pNewMember->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update Member\n");
		pNewMember->dumpDiff(m_output,pMember);
#endif
		fflush(m_output);
	}
}
	
void CMemberLogTrigger::beforeRemove(CMember *pMember)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Member,del");
		pMember->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove Member\n");
		pMember->dump(m_output);
#endif
		fflush(m_output);
	}
}

CServiceConfigLogTrigger::CServiceConfigLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CServiceConfigLogTrigger::~CServiceConfigLogTrigger(void)
{
}

void CServiceConfigLogTrigger::beforeAdd(CWriteableServiceConfig *pServiceConfig)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"ServiceConfig,add");
		pServiceConfig->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add ServiceConfig\n");
		pServiceConfig->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CServiceConfigLogTrigger::beforeUpdate(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pNewServiceConfig)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"ServiceConfig,upd");
		pNewServiceConfig->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update ServiceConfig\n");
		pNewServiceConfig->dumpDiff(m_output,pServiceConfig);
#endif
		fflush(m_output);
	}
}
	
void CServiceConfigLogTrigger::beforeRemove(CServiceConfig *pServiceConfig)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"ServiceConfig,del");
		pServiceConfig->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove ServiceConfig\n");
		pServiceConfig->dump(m_output);
#endif
		fflush(m_output);
	}
}

CUserSessionLogTrigger::CUserSessionLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CUserSessionLogTrigger::~CUserSessionLogTrigger(void)
{
}

void CUserSessionLogTrigger::beforeAdd(CWriteableUserSession *pUserSession)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"UserSession,add");
		pUserSession->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add UserSession\n");
		pUserSession->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CUserSessionLogTrigger::beforeUpdate(CUserSession *pUserSession, CWriteableUserSession *pNewUserSession)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"UserSession,upd");
		pNewUserSession->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update UserSession\n");
		pNewUserSession->dumpDiff(m_output,pUserSession);
#endif
		fflush(m_output);
	}
}
	
void CUserSessionLogTrigger::beforeRemove(CUserSession *pUserSession)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"UserSession,del");
		pUserSession->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove UserSession\n");
		pUserSession->dump(m_output);
#endif
		fflush(m_output);
	}
}

CInstrumentLogTrigger::CInstrumentLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CInstrumentLogTrigger::~CInstrumentLogTrigger(void)
{
}

void CInstrumentLogTrigger::beforeAdd(CWriteableInstrument *pInstrument)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Instrument,add");
		pInstrument->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add Instrument\n");
		pInstrument->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CInstrumentLogTrigger::beforeUpdate(CInstrument *pInstrument, CWriteableInstrument *pNewInstrument)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Instrument,upd");
		pNewInstrument->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update Instrument\n");
		pNewInstrument->dumpDiff(m_output,pInstrument);
#endif
		fflush(m_output);
	}
}
	
void CInstrumentLogTrigger::beforeRemove(CInstrument *pInstrument)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Instrument,del");
		pInstrument->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove Instrument\n");
		pInstrument->dump(m_output);
#endif
		fflush(m_output);
	}
}

CTradingRightLogTrigger::CTradingRightLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CTradingRightLogTrigger::~CTradingRightLogTrigger(void)
{
}

void CTradingRightLogTrigger::beforeAdd(CWriteableTradingRight *pTradingRight)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"TradingRight,add");
		pTradingRight->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add TradingRight\n");
		pTradingRight->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CTradingRightLogTrigger::beforeUpdate(CTradingRight *pTradingRight, CWriteableTradingRight *pNewTradingRight)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"TradingRight,upd");
		pNewTradingRight->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update TradingRight\n");
		pNewTradingRight->dumpDiff(m_output,pTradingRight);
#endif
		fflush(m_output);
	}
}
	
void CTradingRightLogTrigger::beforeRemove(CTradingRight *pTradingRight)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"TradingRight,del");
		pTradingRight->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove TradingRight\n");
		pTradingRight->dump(m_output);
#endif
		fflush(m_output);
	}
}

CMarginRateLogTrigger::CMarginRateLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CMarginRateLogTrigger::~CMarginRateLogTrigger(void)
{
}

void CMarginRateLogTrigger::beforeAdd(CWriteableMarginRate *pMarginRate)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MarginRate,add");
		pMarginRate->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add MarginRate\n");
		pMarginRate->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CMarginRateLogTrigger::beforeUpdate(CMarginRate *pMarginRate, CWriteableMarginRate *pNewMarginRate)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MarginRate,upd");
		pNewMarginRate->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update MarginRate\n");
		pNewMarginRate->dumpDiff(m_output,pMarginRate);
#endif
		fflush(m_output);
	}
}
	
void CMarginRateLogTrigger::beforeRemove(CMarginRate *pMarginRate)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MarginRate,del");
		pMarginRate->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove MarginRate\n");
		pMarginRate->dump(m_output);
#endif
		fflush(m_output);
	}
}

CCFDRateLogTrigger::CCFDRateLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CCFDRateLogTrigger::~CCFDRateLogTrigger(void)
{
}

void CCFDRateLogTrigger::beforeAdd(CWriteableCFDRate *pCFDRate)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"CFDRate,add");
		pCFDRate->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add CFDRate\n");
		pCFDRate->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CCFDRateLogTrigger::beforeUpdate(CCFDRate *pCFDRate, CWriteableCFDRate *pNewCFDRate)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"CFDRate,upd");
		pNewCFDRate->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update CFDRate\n");
		pNewCFDRate->dumpDiff(m_output,pCFDRate);
#endif
		fflush(m_output);
	}
}
	
void CCFDRateLogTrigger::beforeRemove(CCFDRate *pCFDRate)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"CFDRate,del");
		pCFDRate->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove CFDRate\n");
		pCFDRate->dump(m_output);
#endif
		fflush(m_output);
	}
}

CFeeLogTrigger::CFeeLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CFeeLogTrigger::~CFeeLogTrigger(void)
{
}

void CFeeLogTrigger::beforeAdd(CWriteableFee *pFee)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Fee,add");
		pFee->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add Fee\n");
		pFee->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CFeeLogTrigger::beforeUpdate(CFee *pFee, CWriteableFee *pNewFee)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Fee,upd");
		pNewFee->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update Fee\n");
		pNewFee->dumpDiff(m_output,pFee);
#endif
		fflush(m_output);
	}
}
	
void CFeeLogTrigger::beforeRemove(CFee *pFee)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Fee,del");
		pFee->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove Fee\n");
		pFee->dump(m_output);
#endif
		fflush(m_output);
	}
}

CCurrencyLogTrigger::CCurrencyLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CCurrencyLogTrigger::~CCurrencyLogTrigger(void)
{
}

void CCurrencyLogTrigger::beforeAdd(CWriteableCurrency *pCurrency)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Currency,add");
		pCurrency->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add Currency\n");
		pCurrency->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CCurrencyLogTrigger::beforeUpdate(CCurrency *pCurrency, CWriteableCurrency *pNewCurrency)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Currency,upd");
		pNewCurrency->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update Currency\n");
		pNewCurrency->dumpDiff(m_output,pCurrency);
#endif
		fflush(m_output);
	}
}
	
void CCurrencyLogTrigger::beforeRemove(CCurrency *pCurrency)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Currency,del");
		pCurrency->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove Currency\n");
		pCurrency->dump(m_output);
#endif
		fflush(m_output);
	}
}

CLiquidityLogTrigger::CLiquidityLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CLiquidityLogTrigger::~CLiquidityLogTrigger(void)
{
}

void CLiquidityLogTrigger::beforeAdd(CWriteableLiquidity *pLiquidity)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Liquidity,add");
		pLiquidity->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add Liquidity\n");
		pLiquidity->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CLiquidityLogTrigger::beforeUpdate(CLiquidity *pLiquidity, CWriteableLiquidity *pNewLiquidity)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Liquidity,upd");
		pNewLiquidity->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update Liquidity\n");
		pNewLiquidity->dumpDiff(m_output,pLiquidity);
#endif
		fflush(m_output);
	}
}
	
void CLiquidityLogTrigger::beforeRemove(CLiquidity *pLiquidity)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Liquidity,del");
		pLiquidity->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove Liquidity\n");
		pLiquidity->dump(m_output);
#endif
		fflush(m_output);
	}
}

CAccountLogTrigger::CAccountLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CAccountLogTrigger::~CAccountLogTrigger(void)
{
}

void CAccountLogTrigger::beforeAdd(CWriteableAccount *pAccount)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Account,add");
		pAccount->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add Account\n");
		pAccount->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CAccountLogTrigger::beforeUpdate(CAccount *pAccount, CWriteableAccount *pNewAccount)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Account,upd");
		pNewAccount->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update Account\n");
		pNewAccount->dumpDiff(m_output,pAccount);
#endif
		fflush(m_output);
	}
}
	
void CAccountLogTrigger::beforeRemove(CAccount *pAccount)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Account,del");
		pAccount->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove Account\n");
		pAccount->dump(m_output);
#endif
		fflush(m_output);
	}
}

CMemberPositionLogTrigger::CMemberPositionLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CMemberPositionLogTrigger::~CMemberPositionLogTrigger(void)
{
}

void CMemberPositionLogTrigger::beforeAdd(CWriteableMemberPosition *pMemberPosition)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MemberPosition,add");
		pMemberPosition->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add MemberPosition\n");
		pMemberPosition->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CMemberPositionLogTrigger::beforeUpdate(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pNewMemberPosition)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MemberPosition,upd");
		pNewMemberPosition->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update MemberPosition\n");
		pNewMemberPosition->dumpDiff(m_output,pMemberPosition);
#endif
		fflush(m_output);
	}
}
	
void CMemberPositionLogTrigger::beforeRemove(CMemberPosition *pMemberPosition)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MemberPosition,del");
		pMemberPosition->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove MemberPosition\n");
		pMemberPosition->dump(m_output);
#endif
		fflush(m_output);
	}
}

CPositionLogTrigger::CPositionLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CPositionLogTrigger::~CPositionLogTrigger(void)
{
}

void CPositionLogTrigger::beforeAdd(CWriteablePosition *pPosition)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Position,add");
		pPosition->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add Position\n");
		pPosition->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CPositionLogTrigger::beforeUpdate(CPosition *pPosition, CWriteablePosition *pNewPosition)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Position,upd");
		pNewPosition->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update Position\n");
		pNewPosition->dumpDiff(m_output,pPosition);
#endif
		fflush(m_output);
	}
}
	
void CPositionLogTrigger::beforeRemove(CPosition *pPosition)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Position,del");
		pPosition->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove Position\n");
		pPosition->dump(m_output);
#endif
		fflush(m_output);
	}
}

CMarketDataLogTrigger::CMarketDataLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CMarketDataLogTrigger::~CMarketDataLogTrigger(void)
{
}

void CMarketDataLogTrigger::beforeAdd(CWriteableMarketData *pMarketData)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MarketData,add");
		pMarketData->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add MarketData\n");
		pMarketData->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CMarketDataLogTrigger::beforeUpdate(CMarketData *pMarketData, CWriteableMarketData *pNewMarketData)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MarketData,upd");
		pNewMarketData->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update MarketData\n");
		pNewMarketData->dumpDiff(m_output,pMarketData);
#endif
		fflush(m_output);
	}
}
	
void CMarketDataLogTrigger::beforeRemove(CMarketData *pMarketData)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MarketData,del");
		pMarketData->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove MarketData\n");
		pMarketData->dump(m_output);
#endif
		fflush(m_output);
	}
}

COrderLogTrigger::COrderLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

COrderLogTrigger::~COrderLogTrigger(void)
{
}

void COrderLogTrigger::beforeAdd(CWriteableOrder *pOrder)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Order,add");
		pOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add Order\n");
		pOrder->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void COrderLogTrigger::beforeUpdate(COrder *pOrder, CWriteableOrder *pNewOrder)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Order,upd");
		pNewOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update Order\n");
		pNewOrder->dumpDiff(m_output,pOrder);
#endif
		fflush(m_output);
	}
}
	
void COrderLogTrigger::beforeRemove(COrder *pOrder)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Order,del");
		pOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove Order\n");
		pOrder->dump(m_output);
#endif
		fflush(m_output);
	}
}

CPositionOrderLogTrigger::CPositionOrderLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CPositionOrderLogTrigger::~CPositionOrderLogTrigger(void)
{
}

void CPositionOrderLogTrigger::beforeAdd(CWriteablePositionOrder *pPositionOrder)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"PositionOrder,add");
		pPositionOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add PositionOrder\n");
		pPositionOrder->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CPositionOrderLogTrigger::beforeUpdate(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pNewPositionOrder)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"PositionOrder,upd");
		pNewPositionOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update PositionOrder\n");
		pNewPositionOrder->dumpDiff(m_output,pPositionOrder);
#endif
		fflush(m_output);
	}
}
	
void CPositionOrderLogTrigger::beforeRemove(CPositionOrder *pPositionOrder)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"PositionOrder,del");
		pPositionOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove PositionOrder\n");
		pPositionOrder->dump(m_output);
#endif
		fflush(m_output);
	}
}

CTriggerOrderLogTrigger::CTriggerOrderLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CTriggerOrderLogTrigger::~CTriggerOrderLogTrigger(void)
{
}

void CTriggerOrderLogTrigger::beforeAdd(CWriteableTriggerOrder *pTriggerOrder)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"TriggerOrder,add");
		pTriggerOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add TriggerOrder\n");
		pTriggerOrder->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CTriggerOrderLogTrigger::beforeUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pNewTriggerOrder)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"TriggerOrder,upd");
		pNewTriggerOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update TriggerOrder\n");
		pNewTriggerOrder->dumpDiff(m_output,pTriggerOrder);
#endif
		fflush(m_output);
	}
}
	
void CTriggerOrderLogTrigger::beforeRemove(CTriggerOrder *pTriggerOrder)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"TriggerOrder,del");
		pTriggerOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove TriggerOrder\n");
		pTriggerOrder->dump(m_output);
#endif
		fflush(m_output);
	}
}

CMarketOrderLogTrigger::CMarketOrderLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CMarketOrderLogTrigger::~CMarketOrderLogTrigger(void)
{
}

void CMarketOrderLogTrigger::beforeAdd(CWriteableMarketOrder *pMarketOrder)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MarketOrder,add");
		pMarketOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add MarketOrder\n");
		pMarketOrder->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CMarketOrderLogTrigger::beforeUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pNewMarketOrder)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MarketOrder,upd");
		pNewMarketOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update MarketOrder\n");
		pNewMarketOrder->dumpDiff(m_output,pMarketOrder);
#endif
		fflush(m_output);
	}
}
	
void CMarketOrderLogTrigger::beforeRemove(CMarketOrder *pMarketOrder)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"MarketOrder,del");
		pMarketOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove MarketOrder\n");
		pMarketOrder->dump(m_output);
#endif
		fflush(m_output);
	}
}

CTickMarketOrderLogTrigger::CTickMarketOrderLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CTickMarketOrderLogTrigger::~CTickMarketOrderLogTrigger(void)
{
}

void CTickMarketOrderLogTrigger::beforeAdd(CWriteableTickMarketOrder *pTickMarketOrder)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"TickMarketOrder,add");
		pTickMarketOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add TickMarketOrder\n");
		pTickMarketOrder->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CTickMarketOrderLogTrigger::beforeUpdate(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pNewTickMarketOrder)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"TickMarketOrder,upd");
		pNewTickMarketOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update TickMarketOrder\n");
		pNewTickMarketOrder->dumpDiff(m_output,pTickMarketOrder);
#endif
		fflush(m_output);
	}
}
	
void CTickMarketOrderLogTrigger::beforeRemove(CTickMarketOrder *pTickMarketOrder)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"TickMarketOrder,del");
		pTickMarketOrder->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove TickMarketOrder\n");
		pTickMarketOrder->dump(m_output);
#endif
		fflush(m_output);
	}
}

CTradeLogTrigger::CTradeLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CTradeLogTrigger::~CTradeLogTrigger(void)
{
}

void CTradeLogTrigger::beforeAdd(CWriteableTrade *pTrade)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Trade,add");
		pTrade->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add Trade\n");
		pTrade->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CTradeLogTrigger::beforeUpdate(CTrade *pTrade, CWriteableTrade *pNewTrade)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Trade,upd");
		pNewTrade->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update Trade\n");
		pNewTrade->dumpDiff(m_output,pTrade);
#endif
		fflush(m_output);
	}
}
	
void CTradeLogTrigger::beforeRemove(CTrade *pTrade)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"Trade,del");
		pTrade->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove Trade\n");
		pTrade->dump(m_output);
#endif
		fflush(m_output);
	}
}

CKLineLogTrigger::CKLineLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CKLineLogTrigger::~CKLineLogTrigger(void)
{
}

void CKLineLogTrigger::beforeAdd(CWriteableKLine *pKLine)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"KLine,add");
		pKLine->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add KLine\n");
		pKLine->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CKLineLogTrigger::beforeUpdate(CKLine *pKLine, CWriteableKLine *pNewKLine)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"KLine,upd");
		pNewKLine->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update KLine\n");
		pNewKLine->dumpDiff(m_output,pKLine);
#endif
		fflush(m_output);
	}
}
	
void CKLineLogTrigger::beforeRemove(CKLine *pKLine)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"KLine,del");
		pKLine->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove KLine\n");
		pKLine->dump(m_output);
#endif
		fflush(m_output);
	}
}

CPublishKLineLogTrigger::CPublishKLineLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CPublishKLineLogTrigger::~CPublishKLineLogTrigger(void)
{
}

void CPublishKLineLogTrigger::beforeAdd(CWriteablePublishKLine *pPublishKLine)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"PublishKLine,add");
		pPublishKLine->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add PublishKLine\n");
		pPublishKLine->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CPublishKLineLogTrigger::beforeUpdate(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pNewPublishKLine)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"PublishKLine,upd");
		pNewPublishKLine->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update PublishKLine\n");
		pNewPublishKLine->dumpDiff(m_output,pPublishKLine);
#endif
		fflush(m_output);
	}
}
	
void CPublishKLineLogTrigger::beforeRemove(CPublishKLine *pPublishKLine)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"PublishKLine,del");
		pPublishKLine->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove PublishKLine\n");
		pPublishKLine->dump(m_output);
#endif
		fflush(m_output);
	}
}

CLastKLineLogTrigger::CLastKLineLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CLastKLineLogTrigger::~CLastKLineLogTrigger(void)
{
}

void CLastKLineLogTrigger::beforeAdd(CWriteableLastKLine *pLastKLine)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"LastKLine,add");
		pLastKLine->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add LastKLine\n");
		pLastKLine->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CLastKLineLogTrigger::beforeUpdate(CLastKLine *pLastKLine, CWriteableLastKLine *pNewLastKLine)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"LastKLine,upd");
		pNewLastKLine->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update LastKLine\n");
		pNewLastKLine->dumpDiff(m_output,pLastKLine);
#endif
		fflush(m_output);
	}
}
	
void CLastKLineLogTrigger::beforeRemove(CLastKLine *pLastKLine)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"LastKLine,del");
		pLastKLine->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove LastKLine\n");
		pLastKLine->dump(m_output);
#endif
		fflush(m_output);
	}
}

CAccountDetailLogTrigger::CAccountDetailLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CAccountDetailLogTrigger::~CAccountDetailLogTrigger(void)
{
}

void CAccountDetailLogTrigger::beforeAdd(CWriteableAccountDetail *pAccountDetail)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"AccountDetail,add");
		pAccountDetail->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add AccountDetail\n");
		pAccountDetail->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CAccountDetailLogTrigger::beforeUpdate(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pNewAccountDetail)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"AccountDetail,upd");
		pNewAccountDetail->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update AccountDetail\n");
		pNewAccountDetail->dumpDiff(m_output,pAccountDetail);
#endif
		fflush(m_output);
	}
}
	
void CAccountDetailLogTrigger::beforeRemove(CAccountDetail *pAccountDetail)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"AccountDetail,del");
		pAccountDetail->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove AccountDetail\n");
		pAccountDetail->dump(m_output);
#endif
		fflush(m_output);
	}
}

CSettleDetailLogTrigger::CSettleDetailLogTrigger(FILE *output, int flag)
{
	m_output=output;
	m_flag=flag;
}

CSettleDetailLogTrigger::~CSettleDetailLogTrigger(void)
{
}

void CSettleDetailLogTrigger::beforeAdd(CWriteableSettleDetail *pSettleDetail)
{
	if (m_flag&CREATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"SettleDetail,add");
		pSettleDetail->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Add SettleDetail\n");
		pSettleDetail->dump(m_output);
#endif
		fflush(m_output);
	}
}
	
void CSettleDetailLogTrigger::beforeUpdate(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pNewSettleDetail)
{
	if (m_flag&UPDATE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"SettleDetail,upd");
		pNewSettleDetail->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Update SettleDetail\n");
		pNewSettleDetail->dumpDiff(m_output,pSettleDetail);
#endif
		fflush(m_output);
	}
}
	
void CSettleDetailLogTrigger::beforeRemove(CSettleDetail *pSettleDetail)
{
	if (m_flag&DELETE_ACTION)
	{
#ifdef SINGLE_LINE
		fprintf(m_output,"SettleDetail,del");
		pSettleDetail->dumpInLine(m_output);
		fprintf(m_output,"\n");
#else
		fprintf(m_output,"Remove SettleDetail\n");
		pSettleDetail->dump(m_output);
#endif
		fflush(m_output);
	}
}

CMDBLog::CMDBLog(CMDB *pMDB, const char *fileName)
{
	//把下面这行提到前面来了。之前有可能会没有初始化就使用。
	m_Started=0;

	if (*fileName=='\0')
	{
		m_DumpOutput=NULL;
		return;
	}
	if (!strcmp(fileName,"stdout"))
	{
		m_DumpOutput=stdout;
	}
	else if (!strcmp(fileName,"stderr"))
	{
		m_DumpOutput=stderr;
	}
	else
	{
		m_DumpOutput=mfopen(fileName,"wt");
	}
	if (m_DumpOutput==NULL)
	{
		REPORT_EVENT(LOG_INFO,"EnvError","write dump file failed");
		return;
	}
	m_pMDB=pMDB;
	// m_Started=0;

	m_CurrentTimeTrigger=new CCurrentTimeLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_MemberTrigger=new CMemberLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_ServiceConfigTrigger=new CServiceConfigLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_UserSessionTrigger=new CUserSessionLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_InstrumentTrigger=new CInstrumentLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_TradingRightTrigger=new CTradingRightLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_MarginRateTrigger=new CMarginRateLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_CFDRateTrigger=new CCFDRateLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_FeeTrigger=new CFeeLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_CurrencyTrigger=new CCurrencyLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_LiquidityTrigger=new CLiquidityLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_AccountTrigger=new CAccountLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_MemberPositionTrigger=new CMemberPositionLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_PositionTrigger=new CPositionLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_MarketDataTrigger=new CMarketDataLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_OrderTrigger=new COrderLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_PositionOrderTrigger=new CPositionOrderLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_TriggerOrderTrigger=new CTriggerOrderLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_MarketOrderTrigger=new CMarketOrderLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_TickMarketOrderTrigger=new CTickMarketOrderLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_TradeTrigger=new CTradeLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_KLineTrigger=new CKLineLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_PublishKLineTrigger=new CPublishKLineLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_LastKLineTrigger=new CLastKLineLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_AccountDetailTrigger=new CAccountDetailLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
	m_SettleDetailTrigger=new CSettleDetailLogTrigger(m_DumpOutput,CREATE_ACTION|UPDATE_ACTION|DELETE_ACTION);
}

CMDBLog::~CMDBLog(void)
{
	stop();
	if (m_DumpOutput==NULL)
	{
		return;
	}
	delete m_CurrentTimeTrigger;
	delete m_MemberTrigger;
	delete m_ServiceConfigTrigger;
	delete m_UserSessionTrigger;
	delete m_InstrumentTrigger;
	delete m_TradingRightTrigger;
	delete m_MarginRateTrigger;
	delete m_CFDRateTrigger;
	delete m_FeeTrigger;
	delete m_CurrencyTrigger;
	delete m_LiquidityTrigger;
	delete m_AccountTrigger;
	delete m_MemberPositionTrigger;
	delete m_PositionTrigger;
	delete m_MarketDataTrigger;
	delete m_OrderTrigger;
	delete m_PositionOrderTrigger;
	delete m_TriggerOrderTrigger;
	delete m_MarketOrderTrigger;
	delete m_TickMarketOrderTrigger;
	delete m_TradeTrigger;
	delete m_KLineTrigger;
	delete m_PublishKLineTrigger;
	delete m_LastKLineTrigger;
	delete m_AccountDetailTrigger;
	delete m_SettleDetailTrigger;
	if ((m_DumpOutput!=NULL)&&(m_DumpOutput!=stdout)&&(m_DumpOutput!=stderr))
	{
		fclose(m_DumpOutput);
	}
}

int CMDBLog::isStarted(void)
{
	return m_Started;
}

void CMDBLog::start(void)
{
	if (m_Started)
	{
		return;
	}
	if (m_DumpOutput==NULL)
	{
		return;
	}
	m_pMDB->m_CurrentTimeFactory->addActionTrigger(m_CurrentTimeTrigger);
	m_pMDB->m_MemberFactory->addActionTrigger(m_MemberTrigger);
	m_pMDB->m_ServiceConfigFactory->addActionTrigger(m_ServiceConfigTrigger);
	m_pMDB->m_UserSessionFactory->addActionTrigger(m_UserSessionTrigger);
	m_pMDB->m_InstrumentFactory->addActionTrigger(m_InstrumentTrigger);
	m_pMDB->m_TradingRightFactory->addActionTrigger(m_TradingRightTrigger);
	m_pMDB->m_MarginRateFactory->addActionTrigger(m_MarginRateTrigger);
	m_pMDB->m_CFDRateFactory->addActionTrigger(m_CFDRateTrigger);
	m_pMDB->m_FeeFactory->addActionTrigger(m_FeeTrigger);
	m_pMDB->m_CurrencyFactory->addActionTrigger(m_CurrencyTrigger);
	m_pMDB->m_LiquidityFactory->addActionTrigger(m_LiquidityTrigger);
	m_pMDB->m_AccountFactory->addActionTrigger(m_AccountTrigger);
	m_pMDB->m_MemberPositionFactory->addActionTrigger(m_MemberPositionTrigger);
	m_pMDB->m_PositionFactory->addActionTrigger(m_PositionTrigger);
	m_pMDB->m_MarketDataFactory->addActionTrigger(m_MarketDataTrigger);
	m_pMDB->m_OrderFactory->addActionTrigger(m_OrderTrigger);
	m_pMDB->m_PositionOrderFactory->addActionTrigger(m_PositionOrderTrigger);
	m_pMDB->m_TriggerOrderFactory->addActionTrigger(m_TriggerOrderTrigger);
	m_pMDB->m_MarketOrderFactory->addActionTrigger(m_MarketOrderTrigger);
	m_pMDB->m_TickMarketOrderFactory->addActionTrigger(m_TickMarketOrderTrigger);
	m_pMDB->m_TradeFactory->addActionTrigger(m_TradeTrigger);
	m_pMDB->m_KLineFactory->addActionTrigger(m_KLineTrigger);
	m_pMDB->m_PublishKLineFactory->addActionTrigger(m_PublishKLineTrigger);
	m_pMDB->m_LastKLineFactory->addActionTrigger(m_LastKLineTrigger);
	m_pMDB->m_AccountDetailFactory->addActionTrigger(m_AccountDetailTrigger);
	m_pMDB->m_SettleDetailFactory->addActionTrigger(m_SettleDetailTrigger);

	//添加下面一行，相当改变其状态后，给这个参数赋正确的值。
	m_Started=1;
}

void CMDBLog::stop()
{
	if (!m_Started)
	{
		return;
	}
	if (m_DumpOutput==NULL)
	{
		return;
	}
	m_pMDB->m_CurrentTimeFactory->removeActionTrigger(m_CurrentTimeTrigger);
	m_pMDB->m_MemberFactory->removeActionTrigger(m_MemberTrigger);
	m_pMDB->m_ServiceConfigFactory->removeActionTrigger(m_ServiceConfigTrigger);
	m_pMDB->m_UserSessionFactory->removeActionTrigger(m_UserSessionTrigger);
	m_pMDB->m_InstrumentFactory->removeActionTrigger(m_InstrumentTrigger);
	m_pMDB->m_TradingRightFactory->removeActionTrigger(m_TradingRightTrigger);
	m_pMDB->m_MarginRateFactory->removeActionTrigger(m_MarginRateTrigger);
	m_pMDB->m_CFDRateFactory->removeActionTrigger(m_CFDRateTrigger);
	m_pMDB->m_FeeFactory->removeActionTrigger(m_FeeTrigger);
	m_pMDB->m_CurrencyFactory->removeActionTrigger(m_CurrencyTrigger);
	m_pMDB->m_LiquidityFactory->removeActionTrigger(m_LiquidityTrigger);
	m_pMDB->m_AccountFactory->removeActionTrigger(m_AccountTrigger);
	m_pMDB->m_MemberPositionFactory->removeActionTrigger(m_MemberPositionTrigger);
	m_pMDB->m_PositionFactory->removeActionTrigger(m_PositionTrigger);
	m_pMDB->m_MarketDataFactory->removeActionTrigger(m_MarketDataTrigger);
	m_pMDB->m_OrderFactory->removeActionTrigger(m_OrderTrigger);
	m_pMDB->m_PositionOrderFactory->removeActionTrigger(m_PositionOrderTrigger);
	m_pMDB->m_TriggerOrderFactory->removeActionTrigger(m_TriggerOrderTrigger);
	m_pMDB->m_MarketOrderFactory->removeActionTrigger(m_MarketOrderTrigger);
	m_pMDB->m_TickMarketOrderFactory->removeActionTrigger(m_TickMarketOrderTrigger);
	m_pMDB->m_TradeFactory->removeActionTrigger(m_TradeTrigger);
	m_pMDB->m_KLineFactory->removeActionTrigger(m_KLineTrigger);
	m_pMDB->m_PublishKLineFactory->removeActionTrigger(m_PublishKLineTrigger);
	m_pMDB->m_LastKLineFactory->removeActionTrigger(m_LastKLineTrigger);
	m_pMDB->m_AccountDetailFactory->removeActionTrigger(m_AccountDetailTrigger);
	m_pMDB->m_SettleDetailFactory->removeActionTrigger(m_SettleDetailTrigger);
	
	//添加下面一行，相当改变其状态后，给这个参数赋正确的值。
	m_Started=0;
}

void CMDBLog::dumpMsg(char *format,...)
{
	va_list v;
	
	if (!m_Started)
	{
		return;
	}
	
	va_start(v,format);
	vfprintf(m_DumpOutput,format,v);
	va_end(v);
	fflush(m_DumpOutput);
}

