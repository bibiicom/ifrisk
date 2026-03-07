#include "TableTrigger.h"
#include "CopyField.h"
#include "FieldData.h"

//////---------业务配置
CTableServiceConfigTrigger::CTableServiceConfigTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableServiceConfigTrigger::commitAdd(CServiceConfig* pServiceConfig)
{
	if(m_pRsp->PrepareMetaRef(&CServiceConfigField::m_Describe,(void*)pServiceConfig, (void*)pServiceConfig, OUTFOG_TYPE_INSERT))
		commitDo(pServiceConfig);
}

void CTableServiceConfigTrigger::commitUpdate(CServiceConfig* pServiceConfig, CWriteableServiceConfig* pOldServiceConfig)
{
	if(m_pRsp->PrepareMetaRef(&CServiceConfigField::m_Describe,(void*)pServiceConfig, (void*)pOldServiceConfig, OUTFOG_TYPE_UPDATE))
		commitDo(pServiceConfig);
}

void CTableServiceConfigTrigger::commitRemove(CWriteableServiceConfig* pServiceConfig)
{
	if(m_pRsp->PrepareMetaRef(&CServiceConfigField::m_Describe, (void*)pServiceConfig, (void*)pServiceConfig, OUTFOG_TYPE_DELETE))
		commitDo(pServiceConfig);
}

void CTableServiceConfigTrigger::commitDo(CServiceConfig* pServiceConfig)
{
	CopyServiceConfigEntity(&field,pServiceConfig);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CServiceConfigField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------币种信息
CTableCurrencyTrigger::CTableCurrencyTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableCurrencyTrigger::commitAdd(CCurrency* pCurrency)
{
	if(m_pRsp->PrepareMetaRef(&CCurrencyField::m_Describe,(void*)pCurrency, (void*)pCurrency, OUTFOG_TYPE_INSERT))
		commitDo(pCurrency);
}

void CTableCurrencyTrigger::commitUpdate(CCurrency* pCurrency, CWriteableCurrency* pOldCurrency)
{
	if(m_pRsp->PrepareMetaRef(&CCurrencyField::m_Describe,(void*)pCurrency, (void*)pOldCurrency, OUTFOG_TYPE_UPDATE))
		commitDo(pCurrency);
}

void CTableCurrencyTrigger::commitRemove(CWriteableCurrency* pCurrency)
{
	if(m_pRsp->PrepareMetaRef(&CCurrencyField::m_Describe, (void*)pCurrency, (void*)pCurrency, OUTFOG_TYPE_DELETE))
		commitDo(pCurrency);
}

void CTableCurrencyTrigger::commitDo(CCurrency* pCurrency)
{
	CopyCurrencyEntity(&field,pCurrency);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CCurrencyField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------市场行情
CTableMarketDataTrigger::CTableMarketDataTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableMarketDataTrigger::commitAdd(CMarketData* pMarketData)
{
	if(m_pRsp->PrepareMetaRef(&CMarketDataField::m_Describe,(void*)pMarketData, (void*)pMarketData, OUTFOG_TYPE_INSERT))
		commitDo(pMarketData);
}

void CTableMarketDataTrigger::commitUpdate(CMarketData* pMarketData, CWriteableMarketData* pOldMarketData)
{
	if(m_pRsp->PrepareMetaRef(&CMarketDataField::m_Describe,(void*)pMarketData, (void*)pOldMarketData, OUTFOG_TYPE_UPDATE))
		commitDo(pMarketData);
}

void CTableMarketDataTrigger::commitRemove(CWriteableMarketData* pMarketData)
{
	if(m_pRsp->PrepareMetaRef(&CMarketDataField::m_Describe, (void*)pMarketData, (void*)pMarketData, OUTFOG_TYPE_DELETE))
		commitDo(pMarketData);
}

void CTableMarketDataTrigger::commitDo(CMarketData* pMarketData)
{
	CopyMarketDataEntity(&field,pMarketData);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CMarketDataField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------交易标的
CTableInstrumentTrigger::CTableInstrumentTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableInstrumentTrigger::commitAdd(CInstrument* pInstrument)
{
	if(m_pRsp->PrepareMetaRef(&CInstrumentField::m_Describe,(void*)pInstrument, (void*)pInstrument, OUTFOG_TYPE_INSERT))
		commitDo(pInstrument);
}

void CTableInstrumentTrigger::commitUpdate(CInstrument* pInstrument, CWriteableInstrument* pOldInstrument)
{
	if(m_pRsp->PrepareMetaRef(&CInstrumentField::m_Describe,(void*)pInstrument, (void*)pOldInstrument, OUTFOG_TYPE_UPDATE))
		commitDo(pInstrument);
}

void CTableInstrumentTrigger::commitRemove(CWriteableInstrument* pInstrument)
{
	if(m_pRsp->PrepareMetaRef(&CInstrumentField::m_Describe, (void*)pInstrument, (void*)pInstrument, OUTFOG_TYPE_DELETE))
		commitDo(pInstrument);
}

void CTableInstrumentTrigger::commitDo(CInstrument* pInstrument)
{
	CopyInstrumentEntity(&field,pInstrument);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CInstrumentField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------当前时间
CTableCurrentTimeTrigger::CTableCurrentTimeTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableCurrentTimeTrigger::commitAdd(CCurrentTime* pCurrentTime)
{
	if(m_pRsp->PrepareMetaRef(&CCurrentTimeField::m_Describe,(void*)pCurrentTime, (void*)pCurrentTime, OUTFOG_TYPE_INSERT))
		commitDo(pCurrentTime);
}

void CTableCurrentTimeTrigger::commitUpdate(CCurrentTime* pCurrentTime, CWriteableCurrentTime* pOldCurrentTime)
{
	if(m_pRsp->PrepareMetaRef(&CCurrentTimeField::m_Describe,(void*)pCurrentTime, (void*)pOldCurrentTime, OUTFOG_TYPE_UPDATE))
		commitDo(pCurrentTime);
}

void CTableCurrentTimeTrigger::commitRemove(CWriteableCurrentTime* pCurrentTime)
{
	if(m_pRsp->PrepareMetaRef(&CCurrentTimeField::m_Describe, (void*)pCurrentTime, (void*)pCurrentTime, OUTFOG_TYPE_DELETE))
		commitDo(pCurrentTime);
}

void CTableCurrentTimeTrigger::commitDo(CCurrentTime* pCurrentTime)
{
	CopyCurrentTimeEntity(&field,pCurrentTime);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CCurrentTimeField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------成员
CTableMemberTrigger::CTableMemberTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableMemberTrigger::commitAdd(CMember* pMember)
{
	if(m_pRsp->PrepareMetaRef(&CMemberField::m_Describe,(void*)pMember, (void*)pMember, OUTFOG_TYPE_INSERT))
		commitDo(pMember);
}

void CTableMemberTrigger::commitUpdate(CMember* pMember, CWriteableMember* pOldMember)
{
	if(m_pRsp->PrepareMetaRef(&CMemberField::m_Describe,(void*)pMember, (void*)pOldMember, OUTFOG_TYPE_UPDATE))
		commitDo(pMember);
}

void CTableMemberTrigger::commitRemove(CWriteableMember* pMember)
{
	if(m_pRsp->PrepareMetaRef(&CMemberField::m_Describe, (void*)pMember, (void*)pMember, OUTFOG_TYPE_DELETE))
		commitDo(pMember);
}

void CTableMemberTrigger::commitDo(CMember* pMember)
{
	CopyMemberEntity(&field,pMember);
	int nDispatchNo = pMember->MemberID.getValue()[strlen(pMember->MemberID.getValue())-1] - '0';
	if(nDispatchNo < 0 )
		nDispatchNo = -nDispatchNo;
	nDispatchNo = abs(nDispatchNo);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 5 + nDispatchNo % 10;
	else
		m_pRsp->GetMetaRef()->RequestNo += nDispatchNo % 10;
	CMemberField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------用户会话
CTableUserSessionTrigger::CTableUserSessionTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableUserSessionTrigger::commitAdd(CUserSession* pUserSession)
{
	if(m_pRsp->PrepareMetaRef(&CUserSessionField::m_Describe,(void*)pUserSession, (void*)pUserSession, OUTFOG_TYPE_INSERT))
		commitDo(pUserSession);
}

void CTableUserSessionTrigger::commitUpdate(CUserSession* pUserSession, CWriteableUserSession* pOldUserSession)
{
	if(m_pRsp->PrepareMetaRef(&CUserSessionField::m_Describe,(void*)pUserSession, (void*)pOldUserSession, OUTFOG_TYPE_UPDATE))
		commitDo(pUserSession);
}

void CTableUserSessionTrigger::commitRemove(CWriteableUserSession* pUserSession)
{
	if(m_pRsp->PrepareMetaRef(&CUserSessionField::m_Describe, (void*)pUserSession, (void*)pUserSession, OUTFOG_TYPE_DELETE))
		commitDo(pUserSession);
}

void CTableUserSessionTrigger::commitDo(CUserSession* pUserSession)
{
	CopyUserSessionEntity(&field,pUserSession);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CUserSessionField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------最新K线
CTableLastKLineTrigger::CTableLastKLineTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableLastKLineTrigger::commitAdd(CLastKLine* pLastKLine)
{
	if(m_pRsp->PrepareMetaRef(&CLastKLineField::m_Describe,(void*)pLastKLine, (void*)pLastKLine, OUTFOG_TYPE_INSERT))
		commitDo(pLastKLine);
}

void CTableLastKLineTrigger::commitUpdate(CLastKLine* pLastKLine, CWriteableLastKLine* pOldLastKLine)
{
	if(m_pRsp->PrepareMetaRef(&CLastKLineField::m_Describe,(void*)pLastKLine, (void*)pOldLastKLine, OUTFOG_TYPE_UPDATE))
		commitDo(pLastKLine);
}

void CTableLastKLineTrigger::commitRemove(CWriteableLastKLine* pLastKLine)
{
	if(m_pRsp->PrepareMetaRef(&CLastKLineField::m_Describe, (void*)pLastKLine, (void*)pLastKLine, OUTFOG_TYPE_DELETE))
		commitDo(pLastKLine);
}

void CTableLastKLineTrigger::commitDo(CLastKLine* pLastKLine)
{
	CopyLastKLineEntity(&field,pLastKLine);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CLastKLineField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------CFD溢价率
CTableCFDRateTrigger::CTableCFDRateTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableCFDRateTrigger::commitAdd(CCFDRate* pCFDRate)
{
	if(m_pRsp->PrepareMetaRef(&CCFDRateField::m_Describe,(void*)pCFDRate, (void*)pCFDRate, OUTFOG_TYPE_INSERT))
		commitDo(pCFDRate);
}

void CTableCFDRateTrigger::commitUpdate(CCFDRate* pCFDRate, CWriteableCFDRate* pOldCFDRate)
{
	if(m_pRsp->PrepareMetaRef(&CCFDRateField::m_Describe,(void*)pCFDRate, (void*)pOldCFDRate, OUTFOG_TYPE_UPDATE))
		commitDo(pCFDRate);
}

void CTableCFDRateTrigger::commitRemove(CWriteableCFDRate* pCFDRate)
{
	if(m_pRsp->PrepareMetaRef(&CCFDRateField::m_Describe, (void*)pCFDRate, (void*)pCFDRate, OUTFOG_TYPE_DELETE))
		commitDo(pCFDRate);
}

void CTableCFDRateTrigger::commitDo(CCFDRate* pCFDRate)
{
	CopyCFDRateEntity(&field,pCFDRate);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CCFDRateField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------交易权限
CTableTradingRightTrigger::CTableTradingRightTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableTradingRightTrigger::commitAdd(CTradingRight* pTradingRight)
{
	if(m_pRsp->PrepareMetaRef(&CTradingRightField::m_Describe,(void*)pTradingRight, (void*)pTradingRight, OUTFOG_TYPE_INSERT))
		commitDo(pTradingRight);
}

void CTableTradingRightTrigger::commitUpdate(CTradingRight* pTradingRight, CWriteableTradingRight* pOldTradingRight)
{
	if(m_pRsp->PrepareMetaRef(&CTradingRightField::m_Describe,(void*)pTradingRight, (void*)pOldTradingRight, OUTFOG_TYPE_UPDATE))
		commitDo(pTradingRight);
}

void CTableTradingRightTrigger::commitRemove(CWriteableTradingRight* pTradingRight)
{
	if(m_pRsp->PrepareMetaRef(&CTradingRightField::m_Describe, (void*)pTradingRight, (void*)pTradingRight, OUTFOG_TYPE_DELETE))
		commitDo(pTradingRight);
}

void CTableTradingRightTrigger::commitDo(CTradingRight* pTradingRight)
{
	CopyTradingRightEntity(&field,pTradingRight);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CTradingRightField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------保证金率
CTableMarginRateTrigger::CTableMarginRateTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableMarginRateTrigger::commitAdd(CMarginRate* pMarginRate)
{
	if(m_pRsp->PrepareMetaRef(&CMarginRateField::m_Describe,(void*)pMarginRate, (void*)pMarginRate, OUTFOG_TYPE_INSERT))
		commitDo(pMarginRate);
}

void CTableMarginRateTrigger::commitUpdate(CMarginRate* pMarginRate, CWriteableMarginRate* pOldMarginRate)
{
	if(m_pRsp->PrepareMetaRef(&CMarginRateField::m_Describe,(void*)pMarginRate, (void*)pOldMarginRate, OUTFOG_TYPE_UPDATE))
		commitDo(pMarginRate);
}

void CTableMarginRateTrigger::commitRemove(CWriteableMarginRate* pMarginRate)
{
	if(m_pRsp->PrepareMetaRef(&CMarginRateField::m_Describe, (void*)pMarginRate, (void*)pMarginRate, OUTFOG_TYPE_DELETE))
		commitDo(pMarginRate);
}

void CTableMarginRateTrigger::commitDo(CMarginRate* pMarginRate)
{
	CopyMarginRateEntity(&field,pMarginRate);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CMarginRateField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------手续费率
CTableFeeTrigger::CTableFeeTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableFeeTrigger::commitAdd(CFee* pFee)
{
	if(m_pRsp->PrepareMetaRef(&CFeeField::m_Describe,(void*)pFee, (void*)pFee, OUTFOG_TYPE_INSERT))
		commitDo(pFee);
}

void CTableFeeTrigger::commitUpdate(CFee* pFee, CWriteableFee* pOldFee)
{
	if(m_pRsp->PrepareMetaRef(&CFeeField::m_Describe,(void*)pFee, (void*)pOldFee, OUTFOG_TYPE_UPDATE))
		commitDo(pFee);
}

void CTableFeeTrigger::commitRemove(CWriteableFee* pFee)
{
	if(m_pRsp->PrepareMetaRef(&CFeeField::m_Describe, (void*)pFee, (void*)pFee, OUTFOG_TYPE_DELETE))
		commitDo(pFee);
}

void CTableFeeTrigger::commitDo(CFee* pFee)
{
	CopyFeeEntity(&field,pFee);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CFeeField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------流动性设置表
CTableLiquidityTrigger::CTableLiquidityTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableLiquidityTrigger::commitAdd(CLiquidity* pLiquidity)
{
	if(m_pRsp->PrepareMetaRef(&CLiquidityField::m_Describe,(void*)pLiquidity, (void*)pLiquidity, OUTFOG_TYPE_INSERT))
		commitDo(pLiquidity);
}

void CTableLiquidityTrigger::commitUpdate(CLiquidity* pLiquidity, CWriteableLiquidity* pOldLiquidity)
{
	if(m_pRsp->PrepareMetaRef(&CLiquidityField::m_Describe,(void*)pLiquidity, (void*)pOldLiquidity, OUTFOG_TYPE_UPDATE))
		commitDo(pLiquidity);
}

void CTableLiquidityTrigger::commitRemove(CWriteableLiquidity* pLiquidity)
{
	if(m_pRsp->PrepareMetaRef(&CLiquidityField::m_Describe, (void*)pLiquidity, (void*)pLiquidity, OUTFOG_TYPE_DELETE))
		commitDo(pLiquidity);
}

void CTableLiquidityTrigger::commitDo(CLiquidity* pLiquidity)
{
	CopyLiquidityEntity(&field,pLiquidity);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 1;
	CLiquidityField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------账户资金
CTableAccountTrigger::CTableAccountTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableAccountTrigger::commitAdd(CAccount* pAccount)
{
	if(m_pRsp->PrepareMetaRef(&CAccountField::m_Describe,(void*)pAccount, (void*)pAccount, OUTFOG_TYPE_INSERT))
		commitDo(pAccount);
}

void CTableAccountTrigger::commitUpdate(CAccount* pAccount, CWriteableAccount* pOldAccount)
{
	if(m_pRsp->PrepareMetaRef(&CAccountField::m_Describe,(void*)pAccount, (void*)pOldAccount, OUTFOG_TYPE_UPDATE))
		commitDo(pAccount);
}

void CTableAccountTrigger::commitRemove(CWriteableAccount* pAccount)
{
	if(m_pRsp->PrepareMetaRef(&CAccountField::m_Describe, (void*)pAccount, (void*)pAccount, OUTFOG_TYPE_DELETE))
		commitDo(pAccount);
}

void CTableAccountTrigger::commitDo(CAccount* pAccount)
{
	CopyAccountEntity(&field,pAccount);
	int nDispatchNo = pAccount->MemberID.getValue()[strlen(pAccount->MemberID.getValue())-1] - '0';
	if(nDispatchNo < 0 )
		nDispatchNo = -nDispatchNo;
	nDispatchNo = abs(nDispatchNo);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 5 + nDispatchNo % 10;
	else
		m_pRsp->GetMetaRef()->RequestNo += nDispatchNo % 10;
	CAccountField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------成员持仓汇总
CTableMemberPositionTrigger::CTableMemberPositionTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableMemberPositionTrigger::commitAdd(CMemberPosition* pMemberPosition)
{
	if(m_pRsp->PrepareMetaRef(&CMemberPositionField::m_Describe,(void*)pMemberPosition, (void*)pMemberPosition, OUTFOG_TYPE_INSERT))
		commitDo(pMemberPosition);
}

void CTableMemberPositionTrigger::commitUpdate(CMemberPosition* pMemberPosition, CWriteableMemberPosition* pOldMemberPosition)
{
	if(m_pRsp->PrepareMetaRef(&CMemberPositionField::m_Describe,(void*)pMemberPosition, (void*)pOldMemberPosition, OUTFOG_TYPE_UPDATE))
		commitDo(pMemberPosition);
}

void CTableMemberPositionTrigger::commitRemove(CWriteableMemberPosition* pMemberPosition)
{
	if(m_pRsp->PrepareMetaRef(&CMemberPositionField::m_Describe, (void*)pMemberPosition, (void*)pMemberPosition, OUTFOG_TYPE_DELETE))
		commitDo(pMemberPosition);
}

void CTableMemberPositionTrigger::commitDo(CMemberPosition* pMemberPosition)
{
	CopyMemberPositionEntity(&field,pMemberPosition);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 15;
	CMemberPositionField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------标的持仓
CTablePositionTrigger::CTablePositionTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTablePositionTrigger::commitAdd(CPosition* pPosition)
{
	if(m_pRsp->PrepareMetaRef(&CPositionField::m_Describe,(void*)pPosition, (void*)pPosition, OUTFOG_TYPE_INSERT))
		commitDo(pPosition);
}

void CTablePositionTrigger::commitUpdate(CPosition* pPosition, CWriteablePosition* pOldPosition)
{
	if(m_pRsp->PrepareMetaRef(&CPositionField::m_Describe,(void*)pPosition, (void*)pOldPosition, OUTFOG_TYPE_UPDATE))
		commitDo(pPosition);
}

void CTablePositionTrigger::commitRemove(CWriteablePosition* pPosition)
{
	if(m_pRsp->PrepareMetaRef(&CPositionField::m_Describe, (void*)pPosition, (void*)pPosition, OUTFOG_TYPE_DELETE))
		commitDo(pPosition);
}

void CTablePositionTrigger::commitDo(CPosition* pPosition)
{
	CopyPositionEntity(&field,pPosition);
	int nDispatchNo = pPosition->MemberID.getValue()[strlen(pPosition->MemberID.getValue())-1] - '0';
	if(nDispatchNo < 0 )
		nDispatchNo = -nDispatchNo;
	nDispatchNo = abs(nDispatchNo);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 15 + nDispatchNo % 10;
	else
		m_pRsp->GetMetaRef()->RequestNo += nDispatchNo % 10;
	CPositionField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------报单
CTableOrderTrigger::CTableOrderTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableOrderTrigger::commitAdd(COrder* pOrder)
{
	if(m_pRsp->PrepareMetaRef(&COrderField::m_Describe,(void*)pOrder, (void*)pOrder, OUTFOG_TYPE_INSERT))
		commitDo(pOrder);
}

void CTableOrderTrigger::commitUpdate(COrder* pOrder, CWriteableOrder* pOldOrder)
{
	if(m_pRsp->PrepareMetaRef(&COrderField::m_Describe,(void*)pOrder, (void*)pOldOrder, OUTFOG_TYPE_UPDATE))
		commitDo(pOrder);
}

void CTableOrderTrigger::commitRemove(CWriteableOrder* pOrder)
{
	if(m_pRsp->PrepareMetaRef(&COrderField::m_Describe, (void*)pOrder, (void*)pOrder, OUTFOG_TYPE_DELETE))
		commitDo(pOrder);
}

void CTableOrderTrigger::commitDo(COrder* pOrder)
{
	CopyOrderEntity(&field,pOrder);
	int nDispatchNo = pOrder->OrderID.getValue()[strlen(pOrder->OrderID.getValue())-1] - '0';
	if(nDispatchNo < 0 )
		nDispatchNo = -nDispatchNo;
	nDispatchNo = abs(nDispatchNo);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 25 + nDispatchNo % 10;
	else
		m_pRsp->GetMetaRef()->RequestNo += nDispatchNo % 10;
	COrderField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------条件报单
CTableTriggerOrderTrigger::CTableTriggerOrderTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableTriggerOrderTrigger::commitAdd(CTriggerOrder* pTriggerOrder)
{
	if(m_pRsp->PrepareMetaRef(&CTriggerOrderField::m_Describe,(void*)pTriggerOrder, (void*)pTriggerOrder, OUTFOG_TYPE_INSERT))
		commitDo(pTriggerOrder);
}

void CTableTriggerOrderTrigger::commitUpdate(CTriggerOrder* pTriggerOrder, CWriteableTriggerOrder* pOldTriggerOrder)
{
	if(m_pRsp->PrepareMetaRef(&CTriggerOrderField::m_Describe,(void*)pTriggerOrder, (void*)pOldTriggerOrder, OUTFOG_TYPE_UPDATE))
		commitDo(pTriggerOrder);
}

void CTableTriggerOrderTrigger::commitRemove(CWriteableTriggerOrder* pTriggerOrder)
{
	if(m_pRsp->PrepareMetaRef(&CTriggerOrderField::m_Describe, (void*)pTriggerOrder, (void*)pTriggerOrder, OUTFOG_TYPE_DELETE))
		commitDo(pTriggerOrder);
}

void CTableTriggerOrderTrigger::commitDo(CTriggerOrder* pTriggerOrder)
{
	CopyTriggerOrderEntity(&field,pTriggerOrder);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 2;
	CTriggerOrderField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------持仓订单组成
CTablePositionOrderTrigger::CTablePositionOrderTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTablePositionOrderTrigger::commitAdd(CPositionOrder* pPositionOrder)
{
	if(m_pRsp->PrepareMetaRef(&CPositionOrderField::m_Describe,(void*)pPositionOrder, (void*)pPositionOrder, OUTFOG_TYPE_INSERT))
		commitDo(pPositionOrder);
}

void CTablePositionOrderTrigger::commitUpdate(CPositionOrder* pPositionOrder, CWriteablePositionOrder* pOldPositionOrder)
{
	if(m_pRsp->PrepareMetaRef(&CPositionOrderField::m_Describe,(void*)pPositionOrder, (void*)pOldPositionOrder, OUTFOG_TYPE_UPDATE))
		commitDo(pPositionOrder);
}

void CTablePositionOrderTrigger::commitRemove(CWriteablePositionOrder* pPositionOrder)
{
	if(m_pRsp->PrepareMetaRef(&CPositionOrderField::m_Describe, (void*)pPositionOrder, (void*)pPositionOrder, OUTFOG_TYPE_DELETE))
		commitDo(pPositionOrder);
}

void CTablePositionOrderTrigger::commitDo(CPositionOrder* pPositionOrder)
{
	CopyPositionOrderEntity(&field,pPositionOrder);
	int nDispatchNo = pPositionOrder->OrderID.getValue()[strlen(pPositionOrder->OrderID.getValue())-1] - '0';
	if(nDispatchNo < 0 )
		nDispatchNo = -nDispatchNo;
	nDispatchNo = abs(nDispatchNo);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 15 + nDispatchNo % 10;
	else
		m_pRsp->GetMetaRef()->RequestNo += nDispatchNo % 10;
	CPositionOrderField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------成交
CTableTradeTrigger::CTableTradeTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableTradeTrigger::commitAdd(CTrade* pTrade)
{
	if(m_pRsp->PrepareMetaRef(&CTradeField::m_Describe,(void*)pTrade, (void*)pTrade, OUTFOG_TYPE_INSERT))
		commitDo(pTrade);
}

void CTableTradeTrigger::commitUpdate(CTrade* pTrade, CWriteableTrade* pOldTrade)
{
	if(m_pRsp->PrepareMetaRef(&CTradeField::m_Describe,(void*)pTrade, (void*)pOldTrade, OUTFOG_TYPE_UPDATE))
		commitDo(pTrade);
}

void CTableTradeTrigger::commitRemove(CWriteableTrade* pTrade)
{
	if(m_pRsp->PrepareMetaRef(&CTradeField::m_Describe, (void*)pTrade, (void*)pTrade, OUTFOG_TYPE_DELETE))
		commitDo(pTrade);
}

void CTableTradeTrigger::commitDo(CTrade* pTrade)
{
	CopyTradeEntity(&field,pTrade);
	int nDispatchNo = pTrade->TradeID.getValue()[strlen(pTrade->TradeID.getValue())-1] - '0';
	if(nDispatchNo < 0 )
		nDispatchNo = -nDispatchNo;
	nDispatchNo = abs(nDispatchNo);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 110 + nDispatchNo % 10;
	else
		m_pRsp->GetMetaRef()->RequestNo += nDispatchNo % 10;
	CTradeField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------账户流水
CTableAccountDetailTrigger::CTableAccountDetailTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableAccountDetailTrigger::commitAdd(CAccountDetail* pAccountDetail)
{
	if(m_pRsp->PrepareMetaRef(&CAccountDetailField::m_Describe,(void*)pAccountDetail, (void*)pAccountDetail, OUTFOG_TYPE_INSERT))
		commitDo(pAccountDetail);
}

void CTableAccountDetailTrigger::commitUpdate(CAccountDetail* pAccountDetail, CWriteableAccountDetail* pOldAccountDetail)
{
	if(m_pRsp->PrepareMetaRef(&CAccountDetailField::m_Describe,(void*)pAccountDetail, (void*)pOldAccountDetail, OUTFOG_TYPE_UPDATE))
		commitDo(pAccountDetail);
}

void CTableAccountDetailTrigger::commitRemove(CWriteableAccountDetail* pAccountDetail)
{
	if(m_pRsp->PrepareMetaRef(&CAccountDetailField::m_Describe, (void*)pAccountDetail, (void*)pAccountDetail, OUTFOG_TYPE_DELETE))
		commitDo(pAccountDetail);
}

void CTableAccountDetailTrigger::commitDo(CAccountDetail* pAccountDetail)
{
	CopyAccountDetailEntity(&field,pAccountDetail);
	int nDispatchNo = pAccountDetail->AccountDetailID.getValue()[strlen(pAccountDetail->AccountDetailID.getValue())-1] - '0';
	if(nDispatchNo < 0 )
		nDispatchNo = -nDispatchNo;
	nDispatchNo = abs(nDispatchNo);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 120 + nDispatchNo % 10;
	else
		m_pRsp->GetMetaRef()->RequestNo += nDispatchNo % 10;
	CAccountDetailField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------历史K线
CTableKLineTrigger::CTableKLineTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableKLineTrigger::commitAdd(CKLine* pKLine)
{
	if(m_pRsp->PrepareMetaRef(&CKLineField::m_Describe,(void*)pKLine, (void*)pKLine, OUTFOG_TYPE_INSERT))
		commitDo(pKLine);
}

void CTableKLineTrigger::commitUpdate(CKLine* pKLine, CWriteableKLine* pOldKLine)
{
	if(m_pRsp->PrepareMetaRef(&CKLineField::m_Describe,(void*)pKLine, (void*)pOldKLine, OUTFOG_TYPE_UPDATE))
		commitDo(pKLine);
}

void CTableKLineTrigger::commitRemove(CWriteableKLine* pKLine)
{
	if(m_pRsp->PrepareMetaRef(&CKLineField::m_Describe, (void*)pKLine, (void*)pKLine, OUTFOG_TYPE_DELETE))
		commitDo(pKLine);
}

void CTableKLineTrigger::commitDo(CKLine* pKLine)
{
	CopyKLineEntity(&field,pKLine);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 130;
	CKLineField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------操作流水
CTableSettleDetailTrigger::CTableSettleDetailTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableSettleDetailTrigger::commitAdd(CSettleDetail* pSettleDetail)
{
	if(m_pRsp->PrepareMetaRef(&CSettleDetailField::m_Describe,(void*)pSettleDetail, (void*)pSettleDetail, OUTFOG_TYPE_INSERT))
		commitDo(pSettleDetail);
}

void CTableSettleDetailTrigger::commitUpdate(CSettleDetail* pSettleDetail, CWriteableSettleDetail* pOldSettleDetail)
{
	if(m_pRsp->PrepareMetaRef(&CSettleDetailField::m_Describe,(void*)pSettleDetail, (void*)pOldSettleDetail, OUTFOG_TYPE_UPDATE))
		commitDo(pSettleDetail);
}

void CTableSettleDetailTrigger::commitRemove(CWriteableSettleDetail* pSettleDetail)
{
	if(m_pRsp->PrepareMetaRef(&CSettleDetailField::m_Describe, (void*)pSettleDetail, (void*)pSettleDetail, OUTFOG_TYPE_DELETE))
		commitDo(pSettleDetail);
}

void CTableSettleDetailTrigger::commitDo(CSettleDetail* pSettleDetail)
{
	CopySettleDetailEntity(&field,pSettleDetail);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = 130;
	CSettleDetailField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------订单簿变化
CTableMarketOrderTrigger::CTableMarketOrderTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTableMarketOrderTrigger::commitAdd(CMarketOrder* pMarketOrder)
{
	if(m_pRsp->PrepareMetaRef(&CMarketOrderField::m_Describe,(void*)pMarketOrder, (void*)pMarketOrder, OUTFOG_TYPE_INSERT))
		commitDo(pMarketOrder);
}

void CTableMarketOrderTrigger::commitUpdate(CMarketOrder* pMarketOrder, CWriteableMarketOrder* pOldMarketOrder)
{
	if(m_pRsp->PrepareMetaRef(&CMarketOrderField::m_Describe,(void*)pMarketOrder, (void*)pOldMarketOrder, OUTFOG_TYPE_UPDATE))
		commitDo(pMarketOrder);
}

void CTableMarketOrderTrigger::commitRemove(CWriteableMarketOrder* pMarketOrder)
{
	if(m_pRsp->PrepareMetaRef(&CMarketOrderField::m_Describe, (void*)pMarketOrder, (void*)pMarketOrder, OUTFOG_TYPE_DELETE))
		commitDo(pMarketOrder);
}

void CTableMarketOrderTrigger::commitDo(CMarketOrder* pMarketOrder)
{
	CopyMarketOrderEntity(&field,pMarketOrder);
	CMarketOrderField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
//////---------发布K线
CTablePublishKLineTrigger::CTablePublishKLineTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTablePublishKLineTrigger::commitAdd(CPublishKLine* pPublishKLine)
{
	if(m_pRsp->PrepareMetaRef(&CPublishKLineField::m_Describe,(void*)pPublishKLine, (void*)pPublishKLine, OUTFOG_TYPE_INSERT))
		commitDo(pPublishKLine);
}

void CTablePublishKLineTrigger::commitUpdate(CPublishKLine* pPublishKLine, CWriteablePublishKLine* pOldPublishKLine)
{
	if(m_pRsp->PrepareMetaRef(&CPublishKLineField::m_Describe,(void*)pPublishKLine, (void*)pOldPublishKLine, OUTFOG_TYPE_UPDATE))
		commitDo(pPublishKLine);
}

void CTablePublishKLineTrigger::commitRemove(CWriteablePublishKLine* pPublishKLine)
{
	if(m_pRsp->PrepareMetaRef(&CPublishKLineField::m_Describe, (void*)pPublishKLine, (void*)pPublishKLine, OUTFOG_TYPE_DELETE))
		commitDo(pPublishKLine);
}

void CTablePublishKLineTrigger::commitDo(CPublishKLine* pPublishKLine)
{
	CopyPublishKLineEntity(&field,pPublishKLine);
	CPublishKLineField::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}

CSyncTriggers::CSyncTriggers(CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pMDB = pMDB;

	m_pServiceConfigTrigger = new CTableServiceConfigTrigger("ServiceConfig",m_pMDB,pResponser);
	m_pMDB->m_ServiceConfigFactory->addCommitTrigger(m_pServiceConfigTrigger);
	m_pCurrencyTrigger = new CTableCurrencyTrigger("Currency",m_pMDB,pResponser);
	m_pMDB->m_CurrencyFactory->addCommitTrigger(m_pCurrencyTrigger);
	m_pMarketDataTrigger = new CTableMarketDataTrigger("MarketData",m_pMDB,pResponser);
	m_pMDB->m_MarketDataFactory->addCommitTrigger(m_pMarketDataTrigger);
	m_pInstrumentTrigger = new CTableInstrumentTrigger("Instrument",m_pMDB,pResponser);
	m_pMDB->m_InstrumentFactory->addCommitTrigger(m_pInstrumentTrigger);
	m_pCurrentTimeTrigger = new CTableCurrentTimeTrigger("CurrentTime",m_pMDB,pResponser);
	m_pMDB->m_CurrentTimeFactory->addCommitTrigger(m_pCurrentTimeTrigger);
	m_pMemberTrigger = new CTableMemberTrigger("Member",m_pMDB,pResponser);
	m_pMDB->m_MemberFactory->addCommitTrigger(m_pMemberTrigger);
	m_pUserSessionTrigger = new CTableUserSessionTrigger("UserSession",m_pMDB,pResponser);
	m_pMDB->m_UserSessionFactory->addCommitTrigger(m_pUserSessionTrigger);
	m_pLastKLineTrigger = new CTableLastKLineTrigger("LastKLine",m_pMDB,pResponser);
	m_pMDB->m_LastKLineFactory->addCommitTrigger(m_pLastKLineTrigger);
	m_pCFDRateTrigger = new CTableCFDRateTrigger("CFDRate",m_pMDB,pResponser);
	m_pMDB->m_CFDRateFactory->addCommitTrigger(m_pCFDRateTrigger);
	m_pTradingRightTrigger = new CTableTradingRightTrigger("TradingRight",m_pMDB,pResponser);
	m_pMDB->m_TradingRightFactory->addCommitTrigger(m_pTradingRightTrigger);
	m_pMarginRateTrigger = new CTableMarginRateTrigger("MarginRate",m_pMDB,pResponser);
	m_pMDB->m_MarginRateFactory->addCommitTrigger(m_pMarginRateTrigger);
	m_pFeeTrigger = new CTableFeeTrigger("Fee",m_pMDB,pResponser);
	m_pMDB->m_FeeFactory->addCommitTrigger(m_pFeeTrigger);
	m_pLiquidityTrigger = new CTableLiquidityTrigger("Liquidity",m_pMDB,pResponser);
	m_pMDB->m_LiquidityFactory->addCommitTrigger(m_pLiquidityTrigger);
	m_pAccountTrigger = new CTableAccountTrigger("Account",m_pMDB,pResponser);
	m_pMDB->m_AccountFactory->addCommitTrigger(m_pAccountTrigger);
	m_pMemberPositionTrigger = new CTableMemberPositionTrigger("MemberPosition",m_pMDB,pResponser);
	m_pMDB->m_MemberPositionFactory->addCommitTrigger(m_pMemberPositionTrigger);
	m_pPositionTrigger = new CTablePositionTrigger("Position",m_pMDB,pResponser);
	m_pMDB->m_PositionFactory->addCommitTrigger(m_pPositionTrigger);
	m_pOrderTrigger = new CTableOrderTrigger("Order",m_pMDB,pResponser);
	m_pMDB->m_OrderFactory->addCommitTrigger(m_pOrderTrigger);
	m_pTriggerOrderTrigger = new CTableTriggerOrderTrigger("TriggerOrder",m_pMDB,pResponser);
	m_pMDB->m_TriggerOrderFactory->addCommitTrigger(m_pTriggerOrderTrigger);
	m_pPositionOrderTrigger = new CTablePositionOrderTrigger("PositionOrder",m_pMDB,pResponser);
	m_pMDB->m_PositionOrderFactory->addCommitTrigger(m_pPositionOrderTrigger);
	m_pTradeTrigger = new CTableTradeTrigger("Trade",m_pMDB,pResponser);
	m_pMDB->m_TradeFactory->addCommitTrigger(m_pTradeTrigger);
	m_pAccountDetailTrigger = new CTableAccountDetailTrigger("AccountDetail",m_pMDB,pResponser);
	m_pMDB->m_AccountDetailFactory->addCommitTrigger(m_pAccountDetailTrigger);
	m_pKLineTrigger = new CTableKLineTrigger("KLine",m_pMDB,pResponser);
	m_pMDB->m_KLineFactory->addCommitTrigger(m_pKLineTrigger);
	m_pSettleDetailTrigger = new CTableSettleDetailTrigger("SettleDetail",m_pMDB,pResponser);
	m_pMDB->m_SettleDetailFactory->addCommitTrigger(m_pSettleDetailTrigger);
	m_pMarketOrderTrigger = new CTableMarketOrderTrigger("MarketOrder",m_pMDB,pResponser);
	m_pMDB->m_MarketOrderFactory->addCommitTrigger(m_pMarketOrderTrigger);
	m_pPublishKLineTrigger = new CTablePublishKLineTrigger("PublishKLine",m_pMDB,pResponser);
	m_pMDB->m_PublishKLineFactory->addCommitTrigger(m_pPublishKLineTrigger);
}

CSyncTriggers::~CSyncTriggers()
{
	m_pMDB->m_ServiceConfigFactory->removeCommitTrigger(m_pServiceConfigTrigger);
	delete m_pServiceConfigTrigger;
	m_pMDB->m_CurrencyFactory->removeCommitTrigger(m_pCurrencyTrigger);
	delete m_pCurrencyTrigger;
	m_pMDB->m_MarketDataFactory->removeCommitTrigger(m_pMarketDataTrigger);
	delete m_pMarketDataTrigger;
	m_pMDB->m_InstrumentFactory->removeCommitTrigger(m_pInstrumentTrigger);
	delete m_pInstrumentTrigger;
	m_pMDB->m_CurrentTimeFactory->removeCommitTrigger(m_pCurrentTimeTrigger);
	delete m_pCurrentTimeTrigger;
	m_pMDB->m_MemberFactory->removeCommitTrigger(m_pMemberTrigger);
	delete m_pMemberTrigger;
	m_pMDB->m_UserSessionFactory->removeCommitTrigger(m_pUserSessionTrigger);
	delete m_pUserSessionTrigger;
	m_pMDB->m_LastKLineFactory->removeCommitTrigger(m_pLastKLineTrigger);
	delete m_pLastKLineTrigger;
	m_pMDB->m_CFDRateFactory->removeCommitTrigger(m_pCFDRateTrigger);
	delete m_pCFDRateTrigger;
	m_pMDB->m_TradingRightFactory->removeCommitTrigger(m_pTradingRightTrigger);
	delete m_pTradingRightTrigger;
	m_pMDB->m_MarginRateFactory->removeCommitTrigger(m_pMarginRateTrigger);
	delete m_pMarginRateTrigger;
	m_pMDB->m_FeeFactory->removeCommitTrigger(m_pFeeTrigger);
	delete m_pFeeTrigger;
	m_pMDB->m_LiquidityFactory->removeCommitTrigger(m_pLiquidityTrigger);
	delete m_pLiquidityTrigger;
	m_pMDB->m_AccountFactory->removeCommitTrigger(m_pAccountTrigger);
	delete m_pAccountTrigger;
	m_pMDB->m_MemberPositionFactory->removeCommitTrigger(m_pMemberPositionTrigger);
	delete m_pMemberPositionTrigger;
	m_pMDB->m_PositionFactory->removeCommitTrigger(m_pPositionTrigger);
	delete m_pPositionTrigger;
	m_pMDB->m_OrderFactory->removeCommitTrigger(m_pOrderTrigger);
	delete m_pOrderTrigger;
	m_pMDB->m_TriggerOrderFactory->removeCommitTrigger(m_pTriggerOrderTrigger);
	delete m_pTriggerOrderTrigger;
	m_pMDB->m_PositionOrderFactory->removeCommitTrigger(m_pPositionOrderTrigger);
	delete m_pPositionOrderTrigger;
	m_pMDB->m_TradeFactory->removeCommitTrigger(m_pTradeTrigger);
	delete m_pTradeTrigger;
	m_pMDB->m_AccountDetailFactory->removeCommitTrigger(m_pAccountDetailTrigger);
	delete m_pAccountDetailTrigger;
	m_pMDB->m_KLineFactory->removeCommitTrigger(m_pKLineTrigger);
	delete m_pKLineTrigger;
	m_pMDB->m_SettleDetailFactory->removeCommitTrigger(m_pSettleDetailTrigger);
	delete m_pSettleDetailTrigger;
	m_pMDB->m_MarketOrderFactory->removeCommitTrigger(m_pMarketOrderTrigger);
	delete m_pMarketOrderTrigger;
	m_pMDB->m_PublishKLineFactory->removeCommitTrigger(m_pPublishKLineTrigger);
	delete m_pPublishKLineTrigger;
}
