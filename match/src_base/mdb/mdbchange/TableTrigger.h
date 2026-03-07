#ifndef _TAABLETRIGGER_H_
#define _TAABLETRIGGER_H_

#include "mdb.h"
#include "ServiceResponser.h"

//////---------业务配置	
class CTableServiceConfigTrigger:public CServiceConfigCommitTrigger
{
public:
	CTableServiceConfigTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableServiceConfigTrigger(void){};
	virtual void commitAdd(CServiceConfig* pServiceConfig);
	virtual void commitUpdate(CServiceConfig* pServiceConfig, CWriteableServiceConfig* pOldServiceConfig);
	virtual void commitRemove(CWriteableServiceConfig* pServiceConfig);
	void commitDo(CServiceConfig* pServiceConfig);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CServiceConfigField field;
};
//////---------币种信息	
class CTableCurrencyTrigger:public CCurrencyCommitTrigger
{
public:
	CTableCurrencyTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableCurrencyTrigger(void){};
	virtual void commitAdd(CCurrency* pCurrency);
	virtual void commitUpdate(CCurrency* pCurrency, CWriteableCurrency* pOldCurrency);
	virtual void commitRemove(CWriteableCurrency* pCurrency);
	void commitDo(CCurrency* pCurrency);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CCurrencyField field;
};
//////---------市场行情	
class CTableMarketDataTrigger:public CMarketDataCommitTrigger
{
public:
	CTableMarketDataTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableMarketDataTrigger(void){};
	virtual void commitAdd(CMarketData* pMarketData);
	virtual void commitUpdate(CMarketData* pMarketData, CWriteableMarketData* pOldMarketData);
	virtual void commitRemove(CWriteableMarketData* pMarketData);
	void commitDo(CMarketData* pMarketData);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CMarketDataField field;
};
//////---------交易标的	
class CTableInstrumentTrigger:public CInstrumentCommitTrigger
{
public:
	CTableInstrumentTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableInstrumentTrigger(void){};
	virtual void commitAdd(CInstrument* pInstrument);
	virtual void commitUpdate(CInstrument* pInstrument, CWriteableInstrument* pOldInstrument);
	virtual void commitRemove(CWriteableInstrument* pInstrument);
	void commitDo(CInstrument* pInstrument);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CInstrumentField field;
};
//////---------当前时间	
class CTableCurrentTimeTrigger:public CCurrentTimeCommitTrigger
{
public:
	CTableCurrentTimeTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableCurrentTimeTrigger(void){};
	virtual void commitAdd(CCurrentTime* pCurrentTime);
	virtual void commitUpdate(CCurrentTime* pCurrentTime, CWriteableCurrentTime* pOldCurrentTime);
	virtual void commitRemove(CWriteableCurrentTime* pCurrentTime);
	void commitDo(CCurrentTime* pCurrentTime);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CCurrentTimeField field;
};
//////---------成员	
class CTableMemberTrigger:public CMemberCommitTrigger
{
public:
	CTableMemberTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableMemberTrigger(void){};
	virtual void commitAdd(CMember* pMember);
	virtual void commitUpdate(CMember* pMember, CWriteableMember* pOldMember);
	virtual void commitRemove(CWriteableMember* pMember);
	void commitDo(CMember* pMember);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CMemberField field;
};
//////---------用户会话	
class CTableUserSessionTrigger:public CUserSessionCommitTrigger
{
public:
	CTableUserSessionTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableUserSessionTrigger(void){};
	virtual void commitAdd(CUserSession* pUserSession);
	virtual void commitUpdate(CUserSession* pUserSession, CWriteableUserSession* pOldUserSession);
	virtual void commitRemove(CWriteableUserSession* pUserSession);
	void commitDo(CUserSession* pUserSession);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CUserSessionField field;
};
//////---------最新K线	
class CTableLastKLineTrigger:public CLastKLineCommitTrigger
{
public:
	CTableLastKLineTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableLastKLineTrigger(void){};
	virtual void commitAdd(CLastKLine* pLastKLine);
	virtual void commitUpdate(CLastKLine* pLastKLine, CWriteableLastKLine* pOldLastKLine);
	virtual void commitRemove(CWriteableLastKLine* pLastKLine);
	void commitDo(CLastKLine* pLastKLine);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CLastKLineField field;
};
//////---------CFD溢价率	
class CTableCFDRateTrigger:public CCFDRateCommitTrigger
{
public:
	CTableCFDRateTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableCFDRateTrigger(void){};
	virtual void commitAdd(CCFDRate* pCFDRate);
	virtual void commitUpdate(CCFDRate* pCFDRate, CWriteableCFDRate* pOldCFDRate);
	virtual void commitRemove(CWriteableCFDRate* pCFDRate);
	void commitDo(CCFDRate* pCFDRate);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CCFDRateField field;
};
//////---------交易权限	
class CTableTradingRightTrigger:public CTradingRightCommitTrigger
{
public:
	CTableTradingRightTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableTradingRightTrigger(void){};
	virtual void commitAdd(CTradingRight* pTradingRight);
	virtual void commitUpdate(CTradingRight* pTradingRight, CWriteableTradingRight* pOldTradingRight);
	virtual void commitRemove(CWriteableTradingRight* pTradingRight);
	void commitDo(CTradingRight* pTradingRight);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CTradingRightField field;
};
//////---------保证金率	
class CTableMarginRateTrigger:public CMarginRateCommitTrigger
{
public:
	CTableMarginRateTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableMarginRateTrigger(void){};
	virtual void commitAdd(CMarginRate* pMarginRate);
	virtual void commitUpdate(CMarginRate* pMarginRate, CWriteableMarginRate* pOldMarginRate);
	virtual void commitRemove(CWriteableMarginRate* pMarginRate);
	void commitDo(CMarginRate* pMarginRate);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CMarginRateField field;
};
//////---------手续费率	
class CTableFeeTrigger:public CFeeCommitTrigger
{
public:
	CTableFeeTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableFeeTrigger(void){};
	virtual void commitAdd(CFee* pFee);
	virtual void commitUpdate(CFee* pFee, CWriteableFee* pOldFee);
	virtual void commitRemove(CWriteableFee* pFee);
	void commitDo(CFee* pFee);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CFeeField field;
};
//////---------流动性设置表	
class CTableLiquidityTrigger:public CLiquidityCommitTrigger
{
public:
	CTableLiquidityTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableLiquidityTrigger(void){};
	virtual void commitAdd(CLiquidity* pLiquidity);
	virtual void commitUpdate(CLiquidity* pLiquidity, CWriteableLiquidity* pOldLiquidity);
	virtual void commitRemove(CWriteableLiquidity* pLiquidity);
	void commitDo(CLiquidity* pLiquidity);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CLiquidityField field;
};
//////---------账户资金	
class CTableAccountTrigger:public CAccountCommitTrigger
{
public:
	CTableAccountTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableAccountTrigger(void){};
	virtual void commitAdd(CAccount* pAccount);
	virtual void commitUpdate(CAccount* pAccount, CWriteableAccount* pOldAccount);
	virtual void commitRemove(CWriteableAccount* pAccount);
	void commitDo(CAccount* pAccount);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CAccountField field;
};
//////---------成员持仓汇总	
class CTableMemberPositionTrigger:public CMemberPositionCommitTrigger
{
public:
	CTableMemberPositionTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableMemberPositionTrigger(void){};
	virtual void commitAdd(CMemberPosition* pMemberPosition);
	virtual void commitUpdate(CMemberPosition* pMemberPosition, CWriteableMemberPosition* pOldMemberPosition);
	virtual void commitRemove(CWriteableMemberPosition* pMemberPosition);
	void commitDo(CMemberPosition* pMemberPosition);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CMemberPositionField field;
};
//////---------标的持仓	
class CTablePositionTrigger:public CPositionCommitTrigger
{
public:
	CTablePositionTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTablePositionTrigger(void){};
	virtual void commitAdd(CPosition* pPosition);
	virtual void commitUpdate(CPosition* pPosition, CWriteablePosition* pOldPosition);
	virtual void commitRemove(CWriteablePosition* pPosition);
	void commitDo(CPosition* pPosition);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CPositionField field;
};
//////---------报单	
class CTableOrderTrigger:public COrderCommitTrigger
{
public:
	CTableOrderTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableOrderTrigger(void){};
	virtual void commitAdd(COrder* pOrder);
	virtual void commitUpdate(COrder* pOrder, CWriteableOrder* pOldOrder);
	virtual void commitRemove(CWriteableOrder* pOrder);
	void commitDo(COrder* pOrder);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	COrderField field;
};
//////---------条件报单	
class CTableTriggerOrderTrigger:public CTriggerOrderCommitTrigger
{
public:
	CTableTriggerOrderTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableTriggerOrderTrigger(void){};
	virtual void commitAdd(CTriggerOrder* pTriggerOrder);
	virtual void commitUpdate(CTriggerOrder* pTriggerOrder, CWriteableTriggerOrder* pOldTriggerOrder);
	virtual void commitRemove(CWriteableTriggerOrder* pTriggerOrder);
	void commitDo(CTriggerOrder* pTriggerOrder);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CTriggerOrderField field;
};
//////---------持仓订单组成	
class CTablePositionOrderTrigger:public CPositionOrderCommitTrigger
{
public:
	CTablePositionOrderTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTablePositionOrderTrigger(void){};
	virtual void commitAdd(CPositionOrder* pPositionOrder);
	virtual void commitUpdate(CPositionOrder* pPositionOrder, CWriteablePositionOrder* pOldPositionOrder);
	virtual void commitRemove(CWriteablePositionOrder* pPositionOrder);
	void commitDo(CPositionOrder* pPositionOrder);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CPositionOrderField field;
};
//////---------成交	
class CTableTradeTrigger:public CTradeCommitTrigger
{
public:
	CTableTradeTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableTradeTrigger(void){};
	virtual void commitAdd(CTrade* pTrade);
	virtual void commitUpdate(CTrade* pTrade, CWriteableTrade* pOldTrade);
	virtual void commitRemove(CWriteableTrade* pTrade);
	void commitDo(CTrade* pTrade);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CTradeField field;
};
//////---------账户流水	
class CTableAccountDetailTrigger:public CAccountDetailCommitTrigger
{
public:
	CTableAccountDetailTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableAccountDetailTrigger(void){};
	virtual void commitAdd(CAccountDetail* pAccountDetail);
	virtual void commitUpdate(CAccountDetail* pAccountDetail, CWriteableAccountDetail* pOldAccountDetail);
	virtual void commitRemove(CWriteableAccountDetail* pAccountDetail);
	void commitDo(CAccountDetail* pAccountDetail);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CAccountDetailField field;
};
//////---------历史K线	
class CTableKLineTrigger:public CKLineCommitTrigger
{
public:
	CTableKLineTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableKLineTrigger(void){};
	virtual void commitAdd(CKLine* pKLine);
	virtual void commitUpdate(CKLine* pKLine, CWriteableKLine* pOldKLine);
	virtual void commitRemove(CWriteableKLine* pKLine);
	void commitDo(CKLine* pKLine);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CKLineField field;
};
//////---------操作流水	
class CTableSettleDetailTrigger:public CSettleDetailCommitTrigger
{
public:
	CTableSettleDetailTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableSettleDetailTrigger(void){};
	virtual void commitAdd(CSettleDetail* pSettleDetail);
	virtual void commitUpdate(CSettleDetail* pSettleDetail, CWriteableSettleDetail* pOldSettleDetail);
	virtual void commitRemove(CWriteableSettleDetail* pSettleDetail);
	void commitDo(CSettleDetail* pSettleDetail);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CSettleDetailField field;
};
//////---------订单簿变化	
class CTableMarketOrderTrigger:public CMarketOrderCommitTrigger
{
public:
	CTableMarketOrderTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTableMarketOrderTrigger(void){};
	virtual void commitAdd(CMarketOrder* pMarketOrder);
	virtual void commitUpdate(CMarketOrder* pMarketOrder, CWriteableMarketOrder* pOldMarketOrder);
	virtual void commitRemove(CWriteableMarketOrder* pMarketOrder);
	void commitDo(CMarketOrder* pMarketOrder);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CMarketOrderField field;
};
//////---------发布K线	
class CTablePublishKLineTrigger:public CPublishKLineCommitTrigger
{
public:
	CTablePublishKLineTrigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTablePublishKLineTrigger(void){};
	virtual void commitAdd(CPublishKLine* pPublishKLine);
	virtual void commitUpdate(CPublishKLine* pPublishKLine, CWriteablePublishKLine* pOldPublishKLine);
	virtual void commitRemove(CWriteablePublishKLine* pPublishKLine);
	void commitDo(CPublishKLine* pPublishKLine);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
	CPublishKLineField field;
};

class CSyncTriggers
{
public:
	CSyncTriggers(CMDB* pMDB,CServiceResponser *pResponser);
	virtual ~CSyncTriggers();

	CTableServiceConfigTrigger *m_pServiceConfigTrigger;
	CTableCurrencyTrigger *m_pCurrencyTrigger;
	CTableMarketDataTrigger *m_pMarketDataTrigger;
	CTableInstrumentTrigger *m_pInstrumentTrigger;
	CTableCurrentTimeTrigger *m_pCurrentTimeTrigger;
	CTableMemberTrigger *m_pMemberTrigger;
	CTableUserSessionTrigger *m_pUserSessionTrigger;
	CTableLastKLineTrigger *m_pLastKLineTrigger;
	CTableCFDRateTrigger *m_pCFDRateTrigger;
	CTableTradingRightTrigger *m_pTradingRightTrigger;
	CTableMarginRateTrigger *m_pMarginRateTrigger;
	CTableFeeTrigger *m_pFeeTrigger;
	CTableLiquidityTrigger *m_pLiquidityTrigger;
	CTableAccountTrigger *m_pAccountTrigger;
	CTableMemberPositionTrigger *m_pMemberPositionTrigger;
	CTablePositionTrigger *m_pPositionTrigger;
	CTableOrderTrigger *m_pOrderTrigger;
	CTableTriggerOrderTrigger *m_pTriggerOrderTrigger;
	CTablePositionOrderTrigger *m_pPositionOrderTrigger;
	CTableTradeTrigger *m_pTradeTrigger;
	CTableAccountDetailTrigger *m_pAccountDetailTrigger;
	CTableKLineTrigger *m_pKLineTrigger;
	CTableSettleDetailTrigger *m_pSettleDetailTrigger;
	CTableMarketOrderTrigger *m_pMarketOrderTrigger;
	CTablePublishKLineTrigger *m_pPublishKLineTrigger;

private:
	CMDB *m_pMDB;
};

#endif
