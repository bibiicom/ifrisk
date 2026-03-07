#include "public.h"
#include "FieldData.h"

static void DescribeMemberOfCTopicActionField() 
{
	CTopicActionField f;
	f.DescribeMembers();
}
CFieldDescribe CTopicActionField::m_Describe(FID_TopicAction,sizeof(CTopicActionField),"CTopicActionField","主题订阅操作",&DescribeMemberOfCTopicActionField);

static void DescribeMemberOfCCommandHeadField() 
{
	CCommandHeadField f;
	f.DescribeMembers();
}
CFieldDescribe CCommandHeadField::m_Describe(FID_CommandHead,sizeof(CCommandHeadField),"CCommandHeadField","操作信息",&DescribeMemberOfCCommandHeadField);

static void DescribeMemberOfCErrorField() 
{
	CErrorField f;
	f.DescribeMembers();
}
CFieldDescribe CErrorField::m_Describe(FID_Error,sizeof(CErrorField),"CErrorField","响应信息",&DescribeMemberOfCErrorField);

static void DescribeMemberOfCCheckTokenField() 
{
	CCheckTokenField f;
	f.DescribeMembers();
}
CFieldDescribe CCheckTokenField::m_Describe(FID_CheckToken,sizeof(CCheckTokenField),"CCheckTokenField","响应信息",&DescribeMemberOfCCheckTokenField);

static void DescribeMemberOfCCommandField() 
{
	CCommandField f;
	f.DescribeMembers();
}
CFieldDescribe CCommandField::m_Describe(FID_Command,sizeof(CCommandField),"CCommandField","消息",&DescribeMemberOfCCommandField);

static void DescribeMemberOfCUserLoginField() 
{
	CUserLoginField f;
	f.DescribeMembers();
}
CFieldDescribe CUserLoginField::m_Describe(FID_UserLogin,sizeof(CUserLoginField),"CUserLoginField","用户登录",&DescribeMemberOfCUserLoginField);

static void DescribeMemberOfCLinkField() 
{
	CLinkField f;
	f.DescribeMembers();
}
CFieldDescribe CLinkField::m_Describe(FID_Link,sizeof(CLinkField),"CLinkField","向上通道",&DescribeMemberOfCLinkField);

static void DescribeMemberOfCUserLogoutField() 
{
	CUserLogoutField f;
	f.DescribeMembers();
}
CFieldDescribe CUserLogoutField::m_Describe(FID_UserLogout,sizeof(CUserLogoutField),"CUserLogoutField","用户登录退出",&DescribeMemberOfCUserLogoutField);

static void DescribeMemberOfCUserPasswordUpdateField() 
{
	CUserPasswordUpdateField f;
	f.DescribeMembers();
}
CFieldDescribe CUserPasswordUpdateField::m_Describe(FID_UserPasswordUpdate,sizeof(CUserPasswordUpdateField),"CUserPasswordUpdateField","用户口令修改",&DescribeMemberOfCUserPasswordUpdateField);

static void DescribeMemberOfCTriggerOrderInsertField() 
{
	CTriggerOrderInsertField f;
	f.DescribeMembers();
}
CFieldDescribe CTriggerOrderInsertField::m_Describe(FID_TriggerOrderInsert,sizeof(CTriggerOrderInsertField),"CTriggerOrderInsertField","输入报单",&DescribeMemberOfCTriggerOrderInsertField);

static void DescribeMemberOfCCloseOrderInsertField() 
{
	CCloseOrderInsertField f;
	f.DescribeMembers();
}
CFieldDescribe CCloseOrderInsertField::m_Describe(FID_CloseOrderInsert,sizeof(CCloseOrderInsertField),"CCloseOrderInsertField","输入止盈止损报单",&DescribeMemberOfCCloseOrderInsertField);

static void DescribeMemberOfCTriggerOrderActionField() 
{
	CTriggerOrderActionField f;
	f.DescribeMembers();
}
CFieldDescribe CTriggerOrderActionField::m_Describe(FID_TriggerOrderAction,sizeof(CTriggerOrderActionField),"CTriggerOrderActionField","触发单操作",&DescribeMemberOfCTriggerOrderActionField);

static void DescribeMemberOfCOrderInsertField() 
{
	COrderInsertField f;
	f.DescribeMembers();
}
CFieldDescribe COrderInsertField::m_Describe(FID_OrderInsert,sizeof(COrderInsertField),"COrderInsertField","输入报单",&DescribeMemberOfCOrderInsertField);

static void DescribeMemberOfCOrderActionField() 
{
	COrderActionField f;
	f.DescribeMembers();
}
CFieldDescribe COrderActionField::m_Describe(FID_OrderAction,sizeof(COrderActionField),"COrderActionField","报单操作",&DescribeMemberOfCOrderActionField);

static void DescribeMemberOfCQuoteInsertField() 
{
	CQuoteInsertField f;
	f.DescribeMembers();
}
CFieldDescribe CQuoteInsertField::m_Describe(FID_QuoteInsert,sizeof(CQuoteInsertField),"CQuoteInsertField","输入报价",&DescribeMemberOfCQuoteInsertField);

static void DescribeMemberOfCQuoteActionField() 
{
	CQuoteActionField f;
	f.DescribeMembers();
}
CFieldDescribe CQuoteActionField::m_Describe(FID_QuoteAction,sizeof(CQuoteActionField),"CQuoteActionField","报价操作",&DescribeMemberOfCQuoteActionField);

static void DescribeMemberOfCOTCTradeInsertField() 
{
	COTCTradeInsertField f;
	f.DescribeMembers();
}
CFieldDescribe COTCTradeInsertField::m_Describe(FID_OTCTradeInsert,sizeof(COTCTradeInsertField),"COTCTradeInsertField","输入场外成交对",&DescribeMemberOfCOTCTradeInsertField);

static void DescribeMemberOfCOTCTradeField() 
{
	COTCTradeField f;
	f.DescribeMembers();
}
CFieldDescribe COTCTradeField::m_Describe(FID_OTCTrade,sizeof(COTCTradeField),"COTCTradeField","输入场外成交对",&DescribeMemberOfCOTCTradeField);

static void DescribeMemberOfCRobotOrderField() 
{
	CRobotOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CRobotOrderField::m_Describe(FID_RobotOrder,sizeof(CRobotOrderField),"CRobotOrderField","内部做市商报单",&DescribeMemberOfCRobotOrderField);

static void DescribeMemberOfCPositionActionField() 
{
	CPositionActionField f;
	f.DescribeMembers();
}
CFieldDescribe CPositionActionField::m_Describe(FID_PositionAction,sizeof(CPositionActionField),"CPositionActionField","仓位操作",&DescribeMemberOfCPositionActionField);

static void DescribeMemberOfCAccountActionField() 
{
	CAccountActionField f;
	f.DescribeMembers();
}
CFieldDescribe CAccountActionField::m_Describe(FID_AccountAction,sizeof(CAccountActionField),"CAccountActionField","资金账户出入金",&DescribeMemberOfCAccountActionField);

static void DescribeMemberOfCMemberActionField() 
{
	CMemberActionField f;
	f.DescribeMembers();
}
CFieldDescribe CMemberActionField::m_Describe(FID_MemberAction,sizeof(CMemberActionField),"CMemberActionField","资金账户出入金",&DescribeMemberOfCMemberActionField);

static void DescribeMemberOfCMarketOverViewField() 
{
	CMarketOverViewField f;
	f.DescribeMembers();
}
CFieldDescribe CMarketOverViewField::m_Describe(FID_MarketOverView,sizeof(CMarketOverViewField),"CMarketOverViewField","行情概要",&DescribeMemberOfCMarketOverViewField);

static void DescribeMemberOfCFiveDepthMarketDataField() 
{
	CFiveDepthMarketDataField f;
	f.DescribeMembers();
}
CFieldDescribe CFiveDepthMarketDataField::m_Describe(FID_FiveDepthMarketData,sizeof(CFiveDepthMarketDataField),"CFiveDepthMarketDataField","5档深度行情",&DescribeMemberOfCFiveDepthMarketDataField);

static void DescribeMemberOfCTenDepthMarketDataField() 
{
	CTenDepthMarketDataField f;
	f.DescribeMembers();
}
CFieldDescribe CTenDepthMarketDataField::m_Describe(FID_TenDepthMarketData,sizeof(CTenDepthMarketDataField),"CTenDepthMarketDataField","10档深度行情",&DescribeMemberOfCTenDepthMarketDataField);

static void DescribeMemberOfCOptionsMarketDataField() 
{
	COptionsMarketDataField f;
	f.DescribeMembers();
}
CFieldDescribe COptionsMarketDataField::m_Describe(FID_OptionsMarketData,sizeof(COptionsMarketDataField),"COptionsMarketDataField","期权行情",&DescribeMemberOfCOptionsMarketDataField);

static void DescribeMemberOfCInstrumentUpdateField() 
{
	CInstrumentUpdateField f;
	f.DescribeMembers();
}
CFieldDescribe CInstrumentUpdateField::m_Describe(FID_InstrumentUpdate,sizeof(CInstrumentUpdateField),"CInstrumentUpdateField","更新合约",&DescribeMemberOfCInstrumentUpdateField);

static void DescribeMemberOfCMarketDataUpdateField() 
{
	CMarketDataUpdateField f;
	f.DescribeMembers();
}
CFieldDescribe CMarketDataUpdateField::m_Describe(FID_MarketDataUpdate,sizeof(CMarketDataUpdateField),"CMarketDataUpdateField","更新行情",&DescribeMemberOfCMarketDataUpdateField);

static void DescribeMemberOfCMarketTradeField() 
{
	CMarketTradeField f;
	f.DescribeMembers();
}
CFieldDescribe CMarketTradeField::m_Describe(FID_MarketTrade,sizeof(CMarketTradeField),"CMarketTradeField","成交明细",&DescribeMemberOfCMarketTradeField);

static void DescribeMemberOfCSettlementField() 
{
	CSettlementField f;
	f.DescribeMembers();
}
CFieldDescribe CSettlementField::m_Describe(FID_Settlement,sizeof(CSettlementField),"CSettlementField","资金结算",&DescribeMemberOfCSettlementField);

static void DescribeMemberOfCAPIKeyInsertField() 
{
	CAPIKeyInsertField f;
	f.DescribeMembers();
}
CFieldDescribe CAPIKeyInsertField::m_Describe(FID_APIKeyInsert,sizeof(CAPIKeyInsertField),"CAPIKeyInsertField","输入APIKey",&DescribeMemberOfCAPIKeyInsertField);

static void DescribeMemberOfCAPIKeyField() 
{
	CAPIKeyField f;
	f.DescribeMembers();
}
CFieldDescribe CAPIKeyField::m_Describe(FID_APIKey,sizeof(CAPIKeyField),"CAPIKeyField","APIKey",&DescribeMemberOfCAPIKeyField);

static void DescribeMemberOfCAPIKeyActionField() 
{
	CAPIKeyActionField f;
	f.DescribeMembers();
}
CFieldDescribe CAPIKeyActionField::m_Describe(FID_APIKeyAction,sizeof(CAPIKeyActionField),"CAPIKeyActionField","操作APIKey",&DescribeMemberOfCAPIKeyActionField);

static void DescribeMemberOfCClearMarketField() 
{
	CClearMarketField f;
	f.DescribeMembers();
}
CFieldDescribe CClearMarketField::m_Describe(FID_ClearMarket,sizeof(CClearMarketField),"CClearMarketField","清理行情数据",&DescribeMemberOfCClearMarketField);

static void DescribeMemberOfCClearLastKLineField() 
{
	CClearLastKLineField f;
	f.DescribeMembers();
}
CFieldDescribe CClearLastKLineField::m_Describe(FID_ClearLastKLine,sizeof(CClearLastKLineField),"CClearLastKLineField","清理最新K线数据",&DescribeMemberOfCClearLastKLineField);

static void DescribeMemberOfCOTokenInsertField() 
{
	COTokenInsertField f;
	f.DescribeMembers();
}
CFieldDescribe COTokenInsertField::m_Describe(FID_OTokenInsert,sizeof(COTokenInsertField),"COTokenInsertField","输入OToken",&DescribeMemberOfCOTokenInsertField);

static void DescribeMemberOfCOTokenActionField() 
{
	COTokenActionField f;
	f.DescribeMembers();
}
CFieldDescribe COTokenActionField::m_Describe(FID_OTokenAction,sizeof(COTokenActionField),"COTokenActionField","操作OToken",&DescribeMemberOfCOTokenActionField);

static void DescribeMemberOfCResetLocalIDField() 
{
	CResetLocalIDField f;
	f.DescribeMembers();
}
CFieldDescribe CResetLocalIDField::m_Describe(FID_ResetLocalID,sizeof(CResetLocalIDField),"CResetLocalIDField","重置LocalID",&DescribeMemberOfCResetLocalIDField);

static void DescribeMemberOfCPositionMergeField() 
{
	CPositionMergeField f;
	f.DescribeMembers();
}
CFieldDescribe CPositionMergeField::m_Describe(FID_PositionMerge,sizeof(CPositionMergeField),"CPositionMergeField","合并持仓",&DescribeMemberOfCPositionMergeField);

static void DescribeMemberOfCDepositField() 
{
	CDepositField f;
	f.DescribeMembers();
}
CFieldDescribe CDepositField::m_Describe(FID_Deposit,sizeof(CDepositField),"CDepositField","用户入金",&DescribeMemberOfCDepositField);

static void DescribeMemberOfCWithdrawField() 
{
	CWithdrawField f;
	f.DescribeMembers();
}
CFieldDescribe CWithdrawField::m_Describe(FID_Withdraw,sizeof(CWithdrawField),"CWithdrawField","用户出金",&DescribeMemberOfCWithdrawField);

static void DescribeMemberOfCTransferField() 
{
	CTransferField f;
	f.DescribeMembers();
}
CFieldDescribe CTransferField::m_Describe(FID_Transfer,sizeof(CTransferField),"CTransferField","用户转账",&DescribeMemberOfCTransferField);

static void DescribeMemberOfCCloseOrderActionField() 
{
	CCloseOrderActionField f;
	f.DescribeMembers();
}
CFieldDescribe CCloseOrderActionField::m_Describe(FID_CloseOrderAction,sizeof(CCloseOrderActionField),"CCloseOrderActionField","报单止盈止损操作",&DescribeMemberOfCCloseOrderActionField);

static void DescribeMemberOfCLeverageField() 
{
	CLeverageField f;
	f.DescribeMembers();
}
CFieldDescribe CLeverageField::m_Describe(FID_Leverage,sizeof(CLeverageField),"CLeverageField","杠杆倍数",&DescribeMemberOfCLeverageField);

static void DescribeMemberOfCPositionReverseField() 
{
	CPositionReverseField f;
	f.DescribeMembers();
}
CFieldDescribe CPositionReverseField::m_Describe(FID_PositionReverse,sizeof(CPositionReverseField),"CPositionReverseField","反向开仓",&DescribeMemberOfCPositionReverseField);

static void DescribeMemberOfCPrivateDispatchField() 
{
	CPrivateDispatchField f;
	f.DescribeMembers();
}
CFieldDescribe CPrivateDispatchField::m_Describe(FID_PrivateDispatch,sizeof(CPrivateDispatchField),"CPrivateDispatchField","私有主题分类",&DescribeMemberOfCPrivateDispatchField);

static void DescribeMemberOfCTickMarketOrderField() 
{
	CTickMarketOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CTickMarketOrderField::m_Describe(FID_TickMarketOrder,sizeof(CTickMarketOrderField),"CTickMarketOrderField","TICK订单簿变化",&DescribeMemberOfCTickMarketOrderField);

static void DescribeMemberOfCKLineBaseField() 
{
	CKLineBaseField f;
	f.DescribeMembers();
}
CFieldDescribe CKLineBaseField::m_Describe(FID_KLineBase,sizeof(CKLineBaseField),"CKLineBaseField","基础K线",&DescribeMemberOfCKLineBaseField);

static void DescribeMemberOfCLineField() 
{
	CLineField f;
	f.DescribeMembers();
}
CFieldDescribe CLineField::m_Describe(FID_Line,sizeof(CLineField),"CLineField","历史行情线",&DescribeMemberOfCLineField);

static void DescribeMemberOfCChangeCurrencyField() 
{
	CChangeCurrencyField f;
	f.DescribeMembers();
}
CFieldDescribe CChangeCurrencyField::m_Describe(FID_ChangeCurrency,sizeof(CChangeCurrencyField),"CChangeCurrencyField","充币兑换",&DescribeMemberOfCChangeCurrencyField);

static void DescribeMemberOfCWalletNonceField() 
{
	CWalletNonceField f;
	f.DescribeMembers();
}
CFieldDescribe CWalletNonceField::m_Describe(FID_WalletNonce,sizeof(CWalletNonceField),"CWalletNonceField","钱包登录随机数",&DescribeMemberOfCWalletNonceField);

static void DescribeMemberOfCWalletLoginField() 
{
	CWalletLoginField f;
	f.DescribeMembers();
}
CFieldDescribe CWalletLoginField::m_Describe(FID_WalletLogin,sizeof(CWalletLoginField),"CWalletLoginField","钱包登录信息",&DescribeMemberOfCWalletLoginField);

static void DescribeMemberOfCQryOrderField() 
{
	CQryOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CQryOrderField::m_Describe(FID_QryOrder,sizeof(CQryOrderField),"CQryOrderField","报单查询",&DescribeMemberOfCQryOrderField);

static void DescribeMemberOfCQryMemberPositionField() 
{
	CQryMemberPositionField f;
	f.DescribeMembers();
}
CFieldDescribe CQryMemberPositionField::m_Describe(FID_QryMemberPosition,sizeof(CQryMemberPositionField),"CQryMemberPositionField","持仓汇总查询",&DescribeMemberOfCQryMemberPositionField);

static void DescribeMemberOfCQryPositionField() 
{
	CQryPositionField f;
	f.DescribeMembers();
}
CFieldDescribe CQryPositionField::m_Describe(FID_QryPosition,sizeof(CQryPositionField),"CQryPositionField","交易单元持仓查询",&DescribeMemberOfCQryPositionField);

static void DescribeMemberOfCQryAccountField() 
{
	CQryAccountField f;
	f.DescribeMembers();
}
CFieldDescribe CQryAccountField::m_Describe(FID_QryAccount,sizeof(CQryAccountField),"CQryAccountField","交易资金查询",&DescribeMemberOfCQryAccountField);

static void DescribeMemberOfCQryInstrumentField() 
{
	CQryInstrumentField f;
	f.DescribeMembers();
}
CFieldDescribe CQryInstrumentField::m_Describe(FID_QryInstrument,sizeof(CQryInstrumentField),"CQryInstrumentField","标的查询",&DescribeMemberOfCQryInstrumentField);

static void DescribeMemberOfCQryCurrencyField() 
{
	CQryCurrencyField f;
	f.DescribeMembers();
}
CFieldDescribe CQryCurrencyField::m_Describe(FID_QryCurrency,sizeof(CQryCurrencyField),"CQryCurrencyField","标的查询",&DescribeMemberOfCQryCurrencyField);

static void DescribeMemberOfCQryMemberField() 
{
	CQryMemberField f;
	f.DescribeMembers();
}
CFieldDescribe CQryMemberField::m_Describe(FID_QryMember,sizeof(CQryMemberField),"CQryMemberField","成员查询",&DescribeMemberOfCQryMemberField);

static void DescribeMemberOfCQryMarketOrderField() 
{
	CQryMarketOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CQryMarketOrderField::m_Describe(FID_QryMarketOrder,sizeof(CQryMarketOrderField),"CQryMarketOrderField","标的价位查询",&DescribeMemberOfCQryMarketOrderField);

static void DescribeMemberOfCQryAPIKeyField() 
{
	CQryAPIKeyField f;
	f.DescribeMembers();
}
CFieldDescribe CQryAPIKeyField::m_Describe(FID_QryAPIKey,sizeof(CQryAPIKeyField),"CQryAPIKeyField","查询APIKEY",&DescribeMemberOfCQryAPIKeyField);

static void DescribeMemberOfCQryMarginRateField() 
{
	CQryMarginRateField f;
	f.DescribeMembers();
}
CFieldDescribe CQryMarginRateField::m_Describe(FID_QryMarginRate,sizeof(CQryMarginRateField),"CQryMarginRateField","查询保证金率",&DescribeMemberOfCQryMarginRateField);

static void DescribeMemberOfCQryFeeField() 
{
	CQryFeeField f;
	f.DescribeMembers();
}
CFieldDescribe CQryFeeField::m_Describe(FID_QryFee,sizeof(CQryFeeField),"CQryFeeField","查询手续费率",&DescribeMemberOfCQryFeeField);

static void DescribeMemberOfCQryTriggerOrderField() 
{
	CQryTriggerOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CQryTriggerOrderField::m_Describe(FID_QryTriggerOrder,sizeof(CQryTriggerOrderField),"CQryTriggerOrderField","查询触发订单",&DescribeMemberOfCQryTriggerOrderField);

static void DescribeMemberOfCQryLeverageField() 
{
	CQryLeverageField f;
	f.DescribeMembers();
}
CFieldDescribe CQryLeverageField::m_Describe(FID_QryLeverage,sizeof(CQryLeverageField),"CQryLeverageField","查询杠杆倍数",&DescribeMemberOfCQryLeverageField);

static void DescribeMemberOfCQryMarketDataField() 
{
	CQryMarketDataField f;
	f.DescribeMembers();
}
CFieldDescribe CQryMarketDataField::m_Describe(FID_QryMarketData,sizeof(CQryMarketDataField),"CQryMarketDataField","查询行情",&DescribeMemberOfCQryMarketDataField);

static void DescribeMemberOfCQryTradeField() 
{
	CQryTradeField f;
	f.DescribeMembers();
}
CFieldDescribe CQryTradeField::m_Describe(FID_QryTrade,sizeof(CQryTradeField),"CQryTradeField","查询成交",&DescribeMemberOfCQryTradeField);

static void DescribeMemberOfCQryFinishOrderField() 
{
	CQryFinishOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CQryFinishOrderField::m_Describe(FID_QryFinishOrder,sizeof(CQryFinishOrderField),"CQryFinishOrderField","查询历史委托",&DescribeMemberOfCQryFinishOrderField);

static void DescribeMemberOfCQryFinishTriggerOrderField() 
{
	CQryFinishTriggerOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CQryFinishTriggerOrderField::m_Describe(FID_QryFinishTriggerOrder,sizeof(CQryFinishTriggerOrderField),"CQryFinishTriggerOrderField","查询历史条件单",&DescribeMemberOfCQryFinishTriggerOrderField);

static void DescribeMemberOfCQryWithLeverageField() 
{
	CQryWithLeverageField f;
	f.DescribeMembers();
}
CFieldDescribe CQryWithLeverageField::m_Describe(FID_QryWithLeverage,sizeof(CQryWithLeverageField),"CQryWithLeverageField","指定杠杠下梯度信息",&DescribeMemberOfCQryWithLeverageField);

static void DescribeMemberOfCQryKLineField() 
{
	CQryKLineField f;
	f.DescribeMembers();
}
CFieldDescribe CQryKLineField::m_Describe(FID_QryKLine,sizeof(CQryKLineField),"CQryKLineField","查询K线",&DescribeMemberOfCQryKLineField);

static void DescribeMemberOfCQryLineField() 
{
	CQryLineField f;
	f.DescribeMembers();
}
CFieldDescribe CQryLineField::m_Describe(FID_QryLine,sizeof(CQryLineField),"CQryLineField","查询线",&DescribeMemberOfCQryLineField);

static void DescribeMemberOfCQryAccountDetailField() 
{
	CQryAccountDetailField f;
	f.DescribeMembers();
}
CFieldDescribe CQryAccountDetailField::m_Describe(FID_QryAccountDetail,sizeof(CQryAccountDetailField),"CQryAccountDetailField","查询资金流水",&DescribeMemberOfCQryAccountDetailField);

static void DescribeMemberOfCQrySettleDetailField() 
{
	CQrySettleDetailField f;
	f.DescribeMembers();
}
CFieldDescribe CQrySettleDetailField::m_Describe(FID_QrySettleDetail,sizeof(CQrySettleDetailField),"CQrySettleDetailField","查询结算流水",&DescribeMemberOfCQrySettleDetailField);

static void DescribeMemberOfCServiceConfigField() 
{
	CServiceConfigField f;
	f.DescribeMembers();
}
CFieldDescribe CServiceConfigField::m_Describe(FID_ServiceConfig,sizeof(CServiceConfigField),"CServiceConfigField","业务配置",&DescribeMemberOfCServiceConfigField);

static void DescribeMemberOfCCurrencyField() 
{
	CCurrencyField f;
	f.DescribeMembers();
}
CFieldDescribe CCurrencyField::m_Describe(FID_Currency,sizeof(CCurrencyField),"CCurrencyField","币种信息",&DescribeMemberOfCCurrencyField);

static void DescribeMemberOfCMarketDataField() 
{
	CMarketDataField f;
	f.DescribeMembers();
}
CFieldDescribe CMarketDataField::m_Describe(FID_MarketData,sizeof(CMarketDataField),"CMarketDataField","市场行情",&DescribeMemberOfCMarketDataField);

static void DescribeMemberOfCInstrumentField() 
{
	CInstrumentField f;
	f.DescribeMembers();
}
CFieldDescribe CInstrumentField::m_Describe(FID_Instrument,sizeof(CInstrumentField),"CInstrumentField","交易标的",&DescribeMemberOfCInstrumentField);

static void DescribeMemberOfCCurrentTimeField() 
{
	CCurrentTimeField f;
	f.DescribeMembers();
}
CFieldDescribe CCurrentTimeField::m_Describe(FID_CurrentTime,sizeof(CCurrentTimeField),"CCurrentTimeField","当前时间",&DescribeMemberOfCCurrentTimeField);

static void DescribeMemberOfCMemberField() 
{
	CMemberField f;
	f.DescribeMembers();
}
CFieldDescribe CMemberField::m_Describe(FID_Member,sizeof(CMemberField),"CMemberField","成员",&DescribeMemberOfCMemberField);

static void DescribeMemberOfCUserSessionField() 
{
	CUserSessionField f;
	f.DescribeMembers();
}
CFieldDescribe CUserSessionField::m_Describe(FID_UserSession,sizeof(CUserSessionField),"CUserSessionField","用户会话",&DescribeMemberOfCUserSessionField);

static void DescribeMemberOfCLastKLineField() 
{
	CLastKLineField f;
	f.DescribeMembers();
}
CFieldDescribe CLastKLineField::m_Describe(FID_LastKLine,sizeof(CLastKLineField),"CLastKLineField","最新K线",&DescribeMemberOfCLastKLineField);

static void DescribeMemberOfCCFDRateField() 
{
	CCFDRateField f;
	f.DescribeMembers();
}
CFieldDescribe CCFDRateField::m_Describe(FID_CFDRate,sizeof(CCFDRateField),"CCFDRateField","CFD溢价率",&DescribeMemberOfCCFDRateField);

static void DescribeMemberOfCTradingRightField() 
{
	CTradingRightField f;
	f.DescribeMembers();
}
CFieldDescribe CTradingRightField::m_Describe(FID_TradingRight,sizeof(CTradingRightField),"CTradingRightField","交易权限",&DescribeMemberOfCTradingRightField);

static void DescribeMemberOfCMarginRateField() 
{
	CMarginRateField f;
	f.DescribeMembers();
}
CFieldDescribe CMarginRateField::m_Describe(FID_MarginRate,sizeof(CMarginRateField),"CMarginRateField","保证金率",&DescribeMemberOfCMarginRateField);

static void DescribeMemberOfCFeeField() 
{
	CFeeField f;
	f.DescribeMembers();
}
CFieldDescribe CFeeField::m_Describe(FID_Fee,sizeof(CFeeField),"CFeeField","手续费率",&DescribeMemberOfCFeeField);

static void DescribeMemberOfCLiquidityField() 
{
	CLiquidityField f;
	f.DescribeMembers();
}
CFieldDescribe CLiquidityField::m_Describe(FID_Liquidity,sizeof(CLiquidityField),"CLiquidityField","流动性设置表",&DescribeMemberOfCLiquidityField);

static void DescribeMemberOfCAccountField() 
{
	CAccountField f;
	f.DescribeMembers();
}
CFieldDescribe CAccountField::m_Describe(FID_Account,sizeof(CAccountField),"CAccountField","账户资金",&DescribeMemberOfCAccountField);

static void DescribeMemberOfCMemberPositionField() 
{
	CMemberPositionField f;
	f.DescribeMembers();
}
CFieldDescribe CMemberPositionField::m_Describe(FID_MemberPosition,sizeof(CMemberPositionField),"CMemberPositionField","成员持仓汇总",&DescribeMemberOfCMemberPositionField);

static void DescribeMemberOfCPositionField() 
{
	CPositionField f;
	f.DescribeMembers();
}
CFieldDescribe CPositionField::m_Describe(FID_Position,sizeof(CPositionField),"CPositionField","标的持仓",&DescribeMemberOfCPositionField);

static void DescribeMemberOfCOrderField() 
{
	COrderField f;
	f.DescribeMembers();
}
CFieldDescribe COrderField::m_Describe(FID_Order,sizeof(COrderField),"COrderField","报单",&DescribeMemberOfCOrderField);

static void DescribeMemberOfCTriggerOrderField() 
{
	CTriggerOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CTriggerOrderField::m_Describe(FID_TriggerOrder,sizeof(CTriggerOrderField),"CTriggerOrderField","条件报单",&DescribeMemberOfCTriggerOrderField);

static void DescribeMemberOfCPositionOrderField() 
{
	CPositionOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CPositionOrderField::m_Describe(FID_PositionOrder,sizeof(CPositionOrderField),"CPositionOrderField","持仓订单组成",&DescribeMemberOfCPositionOrderField);

static void DescribeMemberOfCFinishOrderField() 
{
	CFinishOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CFinishOrderField::m_Describe(FID_FinishOrder,sizeof(CFinishOrderField),"CFinishOrderField","完成的报单",&DescribeMemberOfCFinishOrderField);

static void DescribeMemberOfCTradeField() 
{
	CTradeField f;
	f.DescribeMembers();
}
CFieldDescribe CTradeField::m_Describe(FID_Trade,sizeof(CTradeField),"CTradeField","成交",&DescribeMemberOfCTradeField);

static void DescribeMemberOfCFinishPositionField() 
{
	CFinishPositionField f;
	f.DescribeMembers();
}
CFieldDescribe CFinishPositionField::m_Describe(FID_FinishPosition,sizeof(CFinishPositionField),"CFinishPositionField","持仓历史",&DescribeMemberOfCFinishPositionField);

static void DescribeMemberOfCAccountDetailField() 
{
	CAccountDetailField f;
	f.DescribeMembers();
}
CFieldDescribe CAccountDetailField::m_Describe(FID_AccountDetail,sizeof(CAccountDetailField),"CAccountDetailField","账户流水",&DescribeMemberOfCAccountDetailField);

static void DescribeMemberOfCKLineField() 
{
	CKLineField f;
	f.DescribeMembers();
}
CFieldDescribe CKLineField::m_Describe(FID_KLine,sizeof(CKLineField),"CKLineField","历史K线",&DescribeMemberOfCKLineField);

static void DescribeMemberOfCSettleDetailField() 
{
	CSettleDetailField f;
	f.DescribeMembers();
}
CFieldDescribe CSettleDetailField::m_Describe(FID_SettleDetail,sizeof(CSettleDetailField),"CSettleDetailField","操作流水",&DescribeMemberOfCSettleDetailField);

static void DescribeMemberOfCFinishTriggerOrderField() 
{
	CFinishTriggerOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CFinishTriggerOrderField::m_Describe(FID_FinishTriggerOrder,sizeof(CFinishTriggerOrderField),"CFinishTriggerOrderField","完成的条件报单",&DescribeMemberOfCFinishTriggerOrderField);

static void DescribeMemberOfCMarketOrderField() 
{
	CMarketOrderField f;
	f.DescribeMembers();
}
CFieldDescribe CMarketOrderField::m_Describe(FID_MarketOrder,sizeof(CMarketOrderField),"CMarketOrderField","订单簿变化",&DescribeMemberOfCMarketOrderField);

static void DescribeMemberOfCPublishKLineField() 
{
	CPublishKLineField f;
	f.DescribeMembers();
}
CFieldDescribe CPublishKLineField::m_Describe(FID_PublishKLine,sizeof(CPublishKLineField),"CPublishKLineField","发布K线",&DescribeMemberOfCPublishKLineField);

