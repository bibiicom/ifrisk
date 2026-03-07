/////////////////////////////////////////////////////////////////////////
///OneMegaApi.h
///定义了客户端API的C语言接口
/////////////////////////////////////////////////////////////////////////

#ifndef ONE_MEGACAPI_H
#define ONE_MEGACAPI_H

#include "OneMegaDataStruct.h"

#if defined(WINDOWS) && !defined(WIN32)
#define WIN32
#endif

#if defined(ONE_MEGACAPI_IS_LIB) && defined(WIN32)
#ifdef LIB_ONE_MEGA_CAPI_EXPORT
#define ONE_MEGA_CAPI_EXPORT __declspec(dllexport)
#else
#define ONE_MEGA_CAPI_EXPORT __declspec(dllimport)
#endif
#else
#define ONE_MEGA_CAPI_EXPORT __attribute__ ((dllimport))
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/// API容器指针
	typedef void* OneMegaApiIns;

	///////////////////////////////////////////////////
	/// 定义回调函数指针
	///////////////////////////////////////////////////

	///连接
	typedef void (*FuncOnConnected)(OneMegaApiIns Ins);
	///断连
	typedef void (*FuncOnDisconnected)(OneMegaApiIns Ins, int nReason);

	///以下函数的共同注释：
	///@param pError:错误信息,0表示正确
	///@param nRequestNo:用户自定义请求号,在对应的Recv函数放回相同值，用于定位报文
	///@param bLast:SendQy函数会返回多条记录，bLast标明是否此类回报的最后一个报文
	///@param nLocalNo:Push函数会返回的报文属于哪个本地主题，相同的nLocalNo的后续订阅会覆盖前订阅
	///@param nSequenceNo:Push函数会返回的报文的nLocalNo为Key的主题序列号
	///一般消息应答
	typedef void (*FuncRecvCommand)(OneMegaApiIns Ins,const MegaCommandStruct *pCommand,int nReqID,char cChain);
	///创建上层连接应答
	typedef void (*FuncRecvInitLink)(OneMegaApiIns Ins,const MegaLinkStruct *pLink,int nReqID,char cChain);
	///用户登录应答
	typedef void (*FuncRecvUserLogin)(OneMegaApiIns Ins,const MegaUserLoginStruct *pUserLogin,const MegaUserSessionStruct *pUserSession,int nReqID,char cChain);
	///用户退出应答
	typedef void (*FuncRecvUserLogout)(OneMegaApiIns Ins,const MegaUserLogoutStruct *pUserLogout,int nReqID,char cChain);
	///报单录入应答
	typedef void (*FuncRecvOrderInsert)(OneMegaApiIns Ins,const MegaOrderInsertStruct *pOrderInsert,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///报单操作应答
	typedef void (*FuncRecvOrderAction)(OneMegaApiIns Ins,const MegaOrderActionStruct *pOrderAction,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///触发单录入应答
	typedef void (*FuncRecvTriggerOrderInsert)(OneMegaApiIns Ins,const MegaTriggerOrderInsertStruct *pTriggerOrderInsert,const MegaTriggerOrderStruct *pTriggerOrder,int nReqID,char cChain);
	///触发单操作应答
	typedef void (*FuncRecvTriggerOrderAction)(OneMegaApiIns Ins,const MegaTriggerOrderActionStruct *pTriggerOrderAction,const MegaTriggerOrderStruct *pTriggerOrder,int nReqID,char cChain);
	///止盈止损报单录入应答
	typedef void (*FuncRecvCloseOrderInsert)(OneMegaApiIns Ins,const MegaCloseOrderInsertStruct *pCloseOrderInsert,const MegaOrderStruct *pOrder,const MegaTriggerOrderStruct *pTriggerOrder,int nReqID,char cChain);
	///止盈止损报单操作应答
	typedef void (*FuncRecvCloseOrderAction)(OneMegaApiIns Ins,const MegaCloseOrderActionStruct *pCloseOrderAction,const MegaTriggerOrderStruct *pTriggerOrder,int nReqID,char cChain);
	///报价录入应答
	typedef void (*FuncRecvQuoteInsert)(OneMegaApiIns Ins,const MegaQuoteInsertStruct *pQuoteInsert,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///报价操作应答
	typedef void (*FuncRecvQuoteAction)(OneMegaApiIns Ins,const MegaQuoteActionStruct *pQuoteAction,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///订阅主题应答
	typedef void (*FuncRecvTopicAction)(OneMegaApiIns Ins,const MegaTopicActionStruct *pTopicAction,int nReqID,char cChain);
	///设置行情应答
	typedef void (*FuncRecvMarketDataUpdate)(OneMegaApiIns Ins,const MegaMarketDataUpdateStruct *pMarketDataUpdate,const MegaMarketDataStruct *pMarketData,const MegaInstrumentStruct *pInstrument,int nReqID,char cChain);
	///调整持仓应答
	typedef void (*FuncRecvPositionAction)(OneMegaApiIns Ins,const MegaPositionActionStruct *pPositionAction,const MegaPositionStruct *pPosition,int nReqID,char cChain);
	///用户出入金应答
	typedef void (*FuncRecvAccountAction)(OneMegaApiIns Ins,const MegaAccountActionStruct *pAccountAction,const MegaAccountStruct *pAccount,int nReqID,char cChain);
	///成员设置应答
	typedef void (*FuncRecvMemberAction)(OneMegaApiIns Ins,const MegaMemberActionStruct *pMemberAction,const MegaMemberStruct *pMember,int nReqID,char cChain);
	///内部做市商申报应答
	typedef void (*FuncRecvRobotOrder)(OneMegaApiIns Ins,const MegaRobotOrderStruct *pRobotOrder,int nReqID,char cChain);
	///场外成交申报应答
	typedef void (*FuncRecvOTCTradeInsert)(OneMegaApiIns Ins,const MegaOTCTradeInsertStruct *pOTCTradeInsert,const MegaOTCTradeStruct *pOTCTrade,int nReqID,char cChain);
	///资金结算应答
	typedef void (*FuncRecvSettlement)(OneMegaApiIns Ins,const MegaSettlementStruct *pSettlement,const MegaSettleDetailStruct *pSettleDetail,int nReqID,char cChain);
	///清理行情数据应答
	typedef void (*FuncRecvClearMarket)(OneMegaApiIns Ins,const MegaClearMarketStruct *pClearMarket,int nReqID,char cChain);
	///清理最新K线应答
	typedef void (*FuncRecvClearLastKLine)(OneMegaApiIns Ins,const MegaClearLastKLineStruct *pClearLastKLine,int nReqID,char cChain);
	///重置LocalID应答
	typedef void (*FuncRecvResetLocalID)(OneMegaApiIns Ins,const MegaResetLocalIDStruct *pResetLocalID,int nReqID,char cChain);
	///OToken录入应答
	typedef void (*FuncRecvOTokenInsert)(OneMegaApiIns Ins,const MegaOTokenInsertStruct *pOTokenInsert,const MegaUserSessionStruct *pUserSession,int nReqID,char cChain);
	///OToken操作应答
	typedef void (*FuncRecvOTokenAction)(OneMegaApiIns Ins,const MegaOTokenActionStruct *pOTokenAction,const MegaUserSessionStruct *pUserSession,int nReqID,char cChain);
	///APIKEY录入应答
	typedef void (*FuncRecvAPIKeyInsert)(OneMegaApiIns Ins,const MegaAPIKeyInsertStruct *pAPIKeyInsert,const MegaAPIKeyStruct *pAPIKey,int nReqID,char cChain);
	///APIKEY操作应答
	typedef void (*FuncRecvAPIKeyAction)(OneMegaApiIns Ins,const MegaAPIKeyActionStruct *pAPIKeyAction,const MegaAPIKeyStruct *pAPIKey,int nReqID,char cChain);
	///仓位合并操作应答
	typedef void (*FuncRecvPositionMerge)(OneMegaApiIns Ins,const MegaPositionMergeStruct *pPositionMerge,const MegaPositionStruct *pPosition,int nReqID,char cChain);
	///用户入金操作应答
	typedef void (*FuncRecvDeposit)(OneMegaApiIns Ins,const MegaDepositStruct *pDeposit,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///用户出金操作应答
	typedef void (*FuncRecvWithdraw)(OneMegaApiIns Ins,const MegaWithdrawStruct *pWithdraw,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///用户转账操作应答
	typedef void (*FuncRecvTransfer)(OneMegaApiIns Ins,const MegaTransferStruct *pTransfer,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///注册用户应答
	typedef void (*FuncRecvUserRegister)(OneMegaApiIns Ins,const MegaUserSessionStruct *pUserSession,int nReqID,char cChain);
	///反向开仓录入应答
	typedef void (*FuncRecvPositionReverse)(OneMegaApiIns Ins,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///避免自成交报单录入应答
	typedef void (*FuncRecvCancelOrderInsert)(OneMegaApiIns Ins,const MegaOrderInsertStruct *pOrderInsert,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///私有主题分类应答
	typedef void (*FuncRecvPrivateDispatch)(OneMegaApiIns Ins,const MegaPrivateDispatchStruct *pPrivateDispatch,const MegaMemberStruct *pMember,int nReqID,char cChain);
	///调整多空杠杠应答
	typedef void (*FuncRecvPositionLeverage)(OneMegaApiIns Ins,const MegaPositionActionStruct *pPositionAction,const MegaPositionStruct *pPosition,int nReqID,char cChain);
	///设置合约应答
	typedef void (*FuncRecvInstrumentUpdate)(OneMegaApiIns Ins,const MegaInstrumentUpdateStruct *pInstrumentUpdate,const MegaInstrumentStruct *pInstrument,int nReqID,char cChain);
	///充币兑换应答
	typedef void (*FuncRecvChangeCurrency)(OneMegaApiIns Ins,const MegaAccountStruct *pAccount,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///钱包登录获取随机数应答
	typedef void (*FuncRecvWalletNonce)(OneMegaApiIns Ins,const MegaWalletNonceStruct *pWalletNonce,int nReqID,char cChain);
	///钱包登录获取随机数应答
	typedef void (*FuncRecvWalletLogin)(OneMegaApiIns Ins,const MegaUserSessionStruct *pUserSession,int nReqID,char cChain);
	///资金查询应答
	typedef void (*FuncRecvQryAccount)(OneMegaApiIns Ins,const MegaAccountStruct *pAccount,int nReqID,char cChain);
	///报单查询应答
	typedef void (*FuncRecvQryOrder)(OneMegaApiIns Ins,const MegaOrderStruct *pOrder,int nReqID,char cChain);
	///持仓汇总查询应答
	typedef void (*FuncRecvQryMemberPosition)(OneMegaApiIns Ins,const MegaMemberPositionStruct *pMemberPosition,int nReqID,char cChain);
	///持仓查询应答
	typedef void (*FuncRecvQryPosition)(OneMegaApiIns Ins,const MegaPositionStruct *pPosition,int nReqID,char cChain);
	///标的查询应答
	typedef void (*FuncRecvQryInstrument)(OneMegaApiIns Ins,const MegaInstrumentStruct *pInstrument,int nReqID,char cChain);
	///币种查询应答
	typedef void (*FuncRecvQryCurrency)(OneMegaApiIns Ins,const MegaCurrencyStruct *pCurrency,int nReqID,char cChain);
	///成员查询应答
	typedef void (*FuncRecvQryMember)(OneMegaApiIns Ins,const MegaMemberStruct *pMember,int nReqID,char cChain);
	///价位查询应答
	typedef void (*FuncRecvQryMarketOrder)(OneMegaApiIns Ins,const MegaMarketOrderStruct *pMarketOrder,int nReqID,char cChain);
	///APIKEY查询应答
	typedef void (*FuncRecvQryAPIKey)(OneMegaApiIns Ins,const MegaAPIKeyStruct *pAPIKey,int nReqID,char cChain);
	///保证金率查询应答
	typedef void (*FuncRecvQryMarginRate)(OneMegaApiIns Ins,const MegaMarginRateStruct *pMarginRate,int nReqID,char cChain);
	///手续费率查询应答
	typedef void (*FuncRecvQryFee)(OneMegaApiIns Ins,const MegaFeeStruct *pFee,int nReqID,char cChain);
	///触发订单查询应答
	typedef void (*FuncRecvQryTriggerOrder)(OneMegaApiIns Ins,const MegaTriggerOrderStruct *pTriggerOrder,int nReqID,char cChain);
	///杠杆倍数查询应答
	typedef void (*FuncRecvQryLeverage)(OneMegaApiIns Ins,const MegaLeverageStruct *pLeverage,int nReqID,char cChain);
	///行情查询应答
	typedef void (*FuncRecvQryMarketData)(OneMegaApiIns Ins,const MegaMarketDataStruct *pMarketData,int nReqID,char cChain);
	///成交查询应答
	typedef void (*FuncRecvQryTrade)(OneMegaApiIns Ins,const MegaTradeStruct *pTrade,int nReqID,char cChain);
	///历史委托查询应答
	typedef void (*FuncRecvQryFinishOrder)(OneMegaApiIns Ins,const MegaFinishOrderStruct *pFinishOrder,int nReqID,char cChain);
	///历史条件单查询应答
	typedef void (*FuncRecvQryFinishTriggerOrder)(OneMegaApiIns Ins,const MegaFinishTriggerOrderStruct *pFinishTriggerOrder,int nReqID,char cChain);
	///指定杠杆倍数查询应答
	typedef void (*FuncRecvQryWithLeverage)(OneMegaApiIns Ins,const MegaLeverageStruct *pLeverage,int nReqID,char cChain);
	///K线查询应答
	typedef void (*FuncRecvQryKLine)(OneMegaApiIns Ins,const MegaKLineBaseStruct *pKLineBase,int nReqID,char cChain);
	///线查询应答
	typedef void (*FuncRecvQryLine)(OneMegaApiIns Ins,const MegaLineStruct *pLine,int nReqID,char cChain);
	///资金流水查询应答
	typedef void (*FuncRecvQryAccountDetail)(OneMegaApiIns Ins,const MegaAccountDetailStruct *pAccountDetail,int nReqID,char cChain);
	///结算流水查询应答
	typedef void (*FuncRecvQrySettleDetail)(OneMegaApiIns Ins,const MegaSettleDetailStruct *pSettleDetail,int nReqID,char cChain);
	///检查TOKEN应答
	typedef void (*FuncRecvCheckToken)(OneMegaApiIns Ins,const MegaCheckTokenStruct *pCheckToken,int nReqID,char cChain);
	///时间同步通知
	typedef void (*FuncPushTimeSync)(OneMegaApiIns Ins,const MegaCurrentTimeStruct *pCurrentTime,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///消息通知
	typedef void (*FuncPushCommand)(OneMegaApiIns Ins,const MegaCommandStruct *pCommand,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///成交通知
	typedef void (*FuncPushTrade)(OneMegaApiIns Ins,const MegaTradeStruct *pTrade,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///报单通知
	typedef void (*FuncPushOrder)(OneMegaApiIns Ins,const MegaOrderStruct *pOrder,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///持仓汇总通知
	typedef void (*FuncPushMemberPosition)(OneMegaApiIns Ins,const MegaMemberPositionStruct *pMemberPosition,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///持仓通知
	typedef void (*FuncPushPosition)(OneMegaApiIns Ins,const MegaPositionStruct *pPosition,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///资金通知
	typedef void (*FuncPushAccount)(OneMegaApiIns Ins,const MegaAccountStruct *pAccount,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///市场成交通知
	typedef void (*FuncPushMarketTrade)(OneMegaApiIns Ins,const MegaMarketTradeStruct *pMarketTrade,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///订单簿变化通知
	typedef void (*FuncPushMarketOrder)(OneMegaApiIns Ins,const MegaMarketOrderStruct *pMarketOrder,const MegaTickMarketOrderStruct *pTickMarketOrder,const MegaCurrentTimeStruct *pCurrentTime,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///K线通知
	typedef void (*FuncPushKLine)(OneMegaApiIns Ins,const MegaKLineStruct *pKLine,const MegaLastKLineStruct *pLastKLine,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///普通行情通知
	typedef void (*FuncPushMarketOverView)(OneMegaApiIns Ins,const MegaMarketOverViewStruct *pMarketOverView,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///全量延时订单簿变化通知
	typedef void (*FuncPushDelayMarketOrder)(OneMegaApiIns Ins,const MegaMarketOrderStruct *pMarketOrder,const MegaCurrentTimeStruct *pCurrentTime,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///五档深度行情通知
	typedef void (*FuncPushFiveDepthMarketData)(OneMegaApiIns Ins,const MegaFiveDepthMarketDataStruct *pFiveDepthMarketData,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///十档深度行情通知
	typedef void (*FuncPushTenDepthMarketData)(OneMegaApiIns Ins,const MegaTenDepthMarketDataStruct *pTenDepthMarketData,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///期权行情通知
	typedef void (*FuncPushOptionsMarketData)(OneMegaApiIns Ins,const MegaOptionsMarketDataStruct *pOptionsMarketData,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///手续费通知
	typedef void (*FuncPushFee)(OneMegaApiIns Ins,const MegaFeeStruct *pFee,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///保证金率通知
	typedef void (*FuncPushMarginRate)(OneMegaApiIns Ins,const MegaMarginRateStruct *pMarginRate,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///成员通知
	typedef void (*FuncPushMember)(OneMegaApiIns Ins,const MegaMemberStruct *pMember,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///触发单通知
	typedef void (*FuncPushTriggerOrder)(OneMegaApiIns Ins,const MegaTriggerOrderStruct *pTriggerOrder,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///合约通知
	typedef void (*FuncPushInstrument)(OneMegaApiIns Ins,const MegaInstrumentStruct *pInstrument,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///用户登陆通知
	typedef void (*FuncPushUserSession)(OneMegaApiIns Ins,const MegaUserSessionStruct *pUserSession,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);
	///币种通知
	typedef void (*FuncPushCurrency)(OneMegaApiIns Ins,const MegaCurrencyStruct *pCurrency,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain);

	////////////////////////////////////////////////
	/// 回调函数虚表
	////////////////////////////////////////////////

	typedef struct Callbacks
	{
		FuncOnConnected ptrOnConnected; ///连接
		FuncOnDisconnected ptrOnDisconnected; ///断线
		FuncRecvCommand ptrRecvCommand; ///一般消息应答
		FuncRecvInitLink ptrRecvInitLink; ///创建上层连接应答
		FuncRecvUserLogin ptrRecvUserLogin; ///用户登录应答
		FuncRecvUserLogout ptrRecvUserLogout; ///用户退出应答
		FuncRecvOrderInsert ptrRecvOrderInsert; ///报单录入应答
		FuncRecvOrderAction ptrRecvOrderAction; ///报单操作应答
		FuncRecvTriggerOrderInsert ptrRecvTriggerOrderInsert; ///触发单录入应答
		FuncRecvTriggerOrderAction ptrRecvTriggerOrderAction; ///触发单操作应答
		FuncRecvCloseOrderInsert ptrRecvCloseOrderInsert; ///止盈止损报单录入应答
		FuncRecvCloseOrderAction ptrRecvCloseOrderAction; ///止盈止损报单操作应答
		FuncRecvQuoteInsert ptrRecvQuoteInsert; ///报价录入应答
		FuncRecvQuoteAction ptrRecvQuoteAction; ///报价操作应答
		FuncRecvTopicAction ptrRecvTopicAction; ///订阅主题应答
		FuncRecvMarketDataUpdate ptrRecvMarketDataUpdate; ///设置行情应答
		FuncRecvPositionAction ptrRecvPositionAction; ///调整持仓应答
		FuncRecvAccountAction ptrRecvAccountAction; ///用户出入金应答
		FuncRecvMemberAction ptrRecvMemberAction; ///成员设置应答
		FuncRecvRobotOrder ptrRecvRobotOrder; ///内部做市商申报应答
		FuncRecvOTCTradeInsert ptrRecvOTCTradeInsert; ///场外成交申报应答
		FuncRecvSettlement ptrRecvSettlement; ///资金结算应答
		FuncRecvClearMarket ptrRecvClearMarket; ///清理行情数据应答
		FuncRecvClearLastKLine ptrRecvClearLastKLine; ///清理最新K线应答
		FuncRecvResetLocalID ptrRecvResetLocalID; ///重置LocalID应答
		FuncRecvOTokenInsert ptrRecvOTokenInsert; ///OToken录入应答
		FuncRecvOTokenAction ptrRecvOTokenAction; ///OToken操作应答
		FuncRecvAPIKeyInsert ptrRecvAPIKeyInsert; ///APIKEY录入应答
		FuncRecvAPIKeyAction ptrRecvAPIKeyAction; ///APIKEY操作应答
		FuncRecvPositionMerge ptrRecvPositionMerge; ///仓位合并操作应答
		FuncRecvDeposit ptrRecvDeposit; ///用户入金操作应答
		FuncRecvWithdraw ptrRecvWithdraw; ///用户出金操作应答
		FuncRecvTransfer ptrRecvTransfer; ///用户转账操作应答
		FuncRecvUserRegister ptrRecvUserRegister; ///注册用户应答
		FuncRecvPositionReverse ptrRecvPositionReverse; ///反向开仓录入应答
		FuncRecvCancelOrderInsert ptrRecvCancelOrderInsert; ///避免自成交报单录入应答
		FuncRecvPrivateDispatch ptrRecvPrivateDispatch; ///私有主题分类应答
		FuncRecvPositionLeverage ptrRecvPositionLeverage; ///调整多空杠杠应答
		FuncRecvInstrumentUpdate ptrRecvInstrumentUpdate; ///设置合约应答
		FuncRecvChangeCurrency ptrRecvChangeCurrency; ///充币兑换应答
		FuncRecvWalletNonce ptrRecvWalletNonce; ///钱包登录获取随机数应答
		FuncRecvWalletLogin ptrRecvWalletLogin; ///钱包登录获取随机数应答
		FuncRecvQryAccount ptrRecvQryAccount; ///资金查询应答
		FuncRecvQryOrder ptrRecvQryOrder; ///报单查询应答
		FuncRecvQryMemberPosition ptrRecvQryMemberPosition; ///持仓汇总查询应答
		FuncRecvQryPosition ptrRecvQryPosition; ///持仓查询应答
		FuncRecvQryInstrument ptrRecvQryInstrument; ///标的查询应答
		FuncRecvQryCurrency ptrRecvQryCurrency; ///币种查询应答
		FuncRecvQryMember ptrRecvQryMember; ///成员查询应答
		FuncRecvQryMarketOrder ptrRecvQryMarketOrder; ///价位查询应答
		FuncRecvQryAPIKey ptrRecvQryAPIKey; ///APIKEY查询应答
		FuncRecvQryMarginRate ptrRecvQryMarginRate; ///保证金率查询应答
		FuncRecvQryFee ptrRecvQryFee; ///手续费率查询应答
		FuncRecvQryTriggerOrder ptrRecvQryTriggerOrder; ///触发订单查询应答
		FuncRecvQryLeverage ptrRecvQryLeverage; ///杠杆倍数查询应答
		FuncRecvQryMarketData ptrRecvQryMarketData; ///行情查询应答
		FuncRecvQryTrade ptrRecvQryTrade; ///成交查询应答
		FuncRecvQryFinishOrder ptrRecvQryFinishOrder; ///历史委托查询应答
		FuncRecvQryFinishTriggerOrder ptrRecvQryFinishTriggerOrder; ///历史条件单查询应答
		FuncRecvQryWithLeverage ptrRecvQryWithLeverage; ///指定杠杆倍数查询应答
		FuncRecvQryKLine ptrRecvQryKLine; ///K线查询应答
		FuncRecvQryLine ptrRecvQryLine; ///线查询应答
		FuncRecvQryAccountDetail ptrRecvQryAccountDetail; ///资金流水查询应答
		FuncRecvQrySettleDetail ptrRecvQrySettleDetail; ///结算流水查询应答
		FuncRecvCheckToken ptrRecvCheckToken; ///检查TOKEN应答
		FuncPushTimeSync ptrPushTimeSync; ///时间同步通知
		FuncPushCommand ptrPushCommand; ///消息通知
		FuncPushTrade ptrPushTrade; ///成交通知
		FuncPushOrder ptrPushOrder; ///报单通知
		FuncPushMemberPosition ptrPushMemberPosition; ///持仓汇总通知
		FuncPushPosition ptrPushPosition; ///持仓通知
		FuncPushAccount ptrPushAccount; ///资金通知
		FuncPushMarketTrade ptrPushMarketTrade; ///市场成交通知
		FuncPushMarketOrder ptrPushMarketOrder; ///订单簿变化通知
		FuncPushKLine ptrPushKLine; ///K线通知
		FuncPushMarketOverView ptrPushMarketOverView; ///普通行情通知
		FuncPushDelayMarketOrder ptrPushDelayMarketOrder; ///全量延时订单簿变化通知
		FuncPushFiveDepthMarketData ptrPushFiveDepthMarketData; ///五档深度行情通知
		FuncPushTenDepthMarketData ptrPushTenDepthMarketData; ///十档深度行情通知
		FuncPushOptionsMarketData ptrPushOptionsMarketData; ///期权行情通知
		FuncPushFee ptrPushFee; ///手续费通知
		FuncPushMarginRate ptrPushMarginRate; ///保证金率通知
		FuncPushMember ptrPushMember; ///成员通知
		FuncPushTriggerOrder ptrPushTriggerOrder; ///触发单通知
		FuncPushInstrument ptrPushInstrument; ///合约通知
		FuncPushUserSession ptrPushUserSession; ///用户登陆通知
		FuncPushCurrency ptrPushCurrency; ///币种通知
	} Callbacks;


	///////////////////////////////////////////////
	/// 主动请求函数
	///////////////////////////////////////////////

	///创建Api
	///@param pFlowPath 存贮订阅信息文件的目录，默认为当前目录
	///@return 创建出的Api
	ONE_MEGA_CAPI_EXPORT OneMegaApiIns CreateOneMegaCApi();

	///注册回调函数虚表
	///@param Ins CreateMegaApi函数返回的API实例地址
	///@param callbacks 回调函数虚表的地址
	ONE_MEGA_CAPI_EXPORT void SetCallback(OneMegaApiIns Ins, Callbacks *pCallback);

	///获取系统版本号
	///@return 版本号
	ONE_MEGA_CAPI_EXPORT const char *GetVersion(OneMegaApiIns Ins);

	///删除接口对象本身
	///@param Ins CreateMegaApi函数返回的API实例地址
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	ONE_MEGA_CAPI_EXPORT void Release(OneMegaApiIns Ins);

	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	ONE_MEGA_CAPI_EXPORT void Start(OneMegaApiIns Ins);

	///等待接口线程结束运行
	///@param Ins CreateMegaApi函数返回的API实例地址
	///@return 线程退出代码
	ONE_MEGA_CAPI_EXPORT int Join(OneMegaApiIns Ins);

	///主动断开和服务器的连接
	///@return 
	ONE_MEGA_CAPI_EXPORT void DisConnect(OneMegaApiIns Ins);

	///注册前置机网络地址
	///@param Ins CreateMegaApi函数返回的API实例地址
	///@param pAddress：前置服务器网络地址
	///@remark 网络地址的格式为："protocol://ip:port"，如："tcp://192.168.1.8:8888"
	///@remark “tcp|shm|multi”代表传输协议，"192.168.1.8"代表服务器地址。"8888"代表服务器端口号
	ONE_MEGA_CAPI_EXPORT void SetFrontAddress(OneMegaApiIns Ins,const char *pszFrontAddress);

	///加载证书
	///@param Ins CreateMegaApi函数返回的API实例地址
	///@param pszCertFileName 用户证书文件名
	///@param pszKeyFileName 用户私钥文件名
	///@param pszCaFileName 可信任CA证书文件名
	///@param pszKeyFilePassword 用户私钥文件密码
	///@return 0 操作成功;-1 可信任CA证书载入失败;-2 用户证书载入失败;-3 用户私钥载入失败;-4 用户证书校验失败
	ONE_MEGA_CAPI_EXPORT int SetCertificateFile(OneMegaApiIns Ins, const char *pszCertFileName, const char *pszKeyFileName, const char *pszCaFileName, const char *pszKeyFilePassword);

	///设置心跳超时时间。
	///@param Ins CreateMegaApi函数返回的API实例地址
	///@param timeout 心跳超时时间(秒)
	ONE_MEGA_CAPI_EXPORT void SetHeartbeatTimeout(OneMegaApiIns Ins, unsigned int nTimeout);

	///以下函数共同点说明：
	///@param nReqID:用户自定义请求号,在对应的RecvSend或者RecvQuery函数处原值返回
	///@return： 0:发送成功;-1 发送线程错误;-2 服务端未回消息超标;-3 发送端发送消息数超标
	///一般消息请求
	ONE_MEGA_CAPI_EXPORT int SendCommand(OneMegaApiIns Ins, MegaCommandStruct *pCommand, int nReqID);
	///创建上层连接请求
	ONE_MEGA_CAPI_EXPORT int SendInitLink(OneMegaApiIns Ins, MegaLinkStruct *pLink, int nReqID);
	///用户登录请求
	ONE_MEGA_CAPI_EXPORT int SendUserLogin(OneMegaApiIns Ins, MegaUserLoginStruct *pUserLogin, int nReqID);
	///用户退出请求
	ONE_MEGA_CAPI_EXPORT int SendUserLogout(OneMegaApiIns Ins, MegaUserLogoutStruct *pUserLogout, int nReqID);
	///报单录入请求
	ONE_MEGA_CAPI_EXPORT int SendOrderInsert(OneMegaApiIns Ins, MegaOrderInsertStruct *pOrderInsert, int nReqID);
	///报单操作请求
	ONE_MEGA_CAPI_EXPORT int SendOrderAction(OneMegaApiIns Ins, MegaOrderActionStruct *pOrderAction, int nReqID);
	///触发单录入请求
	ONE_MEGA_CAPI_EXPORT int SendTriggerOrderInsert(OneMegaApiIns Ins, MegaTriggerOrderInsertStruct *pTriggerOrderInsert, int nReqID);
	///触发单操作请求
	ONE_MEGA_CAPI_EXPORT int SendTriggerOrderAction(OneMegaApiIns Ins, MegaTriggerOrderActionStruct *pTriggerOrderAction, int nReqID);
	///止盈止损报单录入请求
	ONE_MEGA_CAPI_EXPORT int SendCloseOrderInsert(OneMegaApiIns Ins, MegaCloseOrderInsertStruct *pCloseOrderInsert, int nReqID);
	///止盈止损报单操作请求
	ONE_MEGA_CAPI_EXPORT int SendCloseOrderAction(OneMegaApiIns Ins, MegaCloseOrderActionStruct *pCloseOrderAction, int nReqID);
	///报价录入请求
	ONE_MEGA_CAPI_EXPORT int SendQuoteInsert(OneMegaApiIns Ins, MegaQuoteInsertStruct *pQuoteInsert, int nReqID);
	///报价操作请求
	ONE_MEGA_CAPI_EXPORT int SendQuoteAction(OneMegaApiIns Ins, MegaQuoteActionStruct *pQuoteAction, int nReqID);
	///订阅主题请求
	ONE_MEGA_CAPI_EXPORT int SendTopicAction(OneMegaApiIns Ins, MegaTopicActionStruct *pTopicAction, int nReqID);
	///设置行情请求
	ONE_MEGA_CAPI_EXPORT int SendMarketDataUpdate(OneMegaApiIns Ins, MegaMarketDataUpdateStruct *pMarketDataUpdate, int nReqID);
	///调整持仓请求
	ONE_MEGA_CAPI_EXPORT int SendPositionAction(OneMegaApiIns Ins, MegaPositionActionStruct *pPositionAction, int nReqID);
	///用户出入金请求
	ONE_MEGA_CAPI_EXPORT int SendAccountAction(OneMegaApiIns Ins, MegaAccountActionStruct *pAccountAction, int nReqID);
	///成员设置请求
	ONE_MEGA_CAPI_EXPORT int SendMemberAction(OneMegaApiIns Ins, MegaMemberActionStruct *pMemberAction, int nReqID);
	///内部做市商申报请求
	ONE_MEGA_CAPI_EXPORT int SendRobotOrder(OneMegaApiIns Ins, MegaRobotOrderStruct *pRobotOrder, int nReqID);
	///场外成交申报请求
	ONE_MEGA_CAPI_EXPORT int SendOTCTradeInsert(OneMegaApiIns Ins, MegaOTCTradeInsertStruct *pOTCTradeInsert, int nReqID);
	///资金结算请求
	ONE_MEGA_CAPI_EXPORT int SendSettlement(OneMegaApiIns Ins, MegaSettlementStruct *pSettlement, int nReqID);
	///清理行情数据请求
	ONE_MEGA_CAPI_EXPORT int SendClearMarket(OneMegaApiIns Ins, MegaClearMarketStruct *pClearMarket, int nReqID);
	///清理最新K线请求
	ONE_MEGA_CAPI_EXPORT int SendClearLastKLine(OneMegaApiIns Ins, MegaClearLastKLineStruct *pClearLastKLine, int nReqID);
	///重置LocalID请求
	ONE_MEGA_CAPI_EXPORT int SendResetLocalID(OneMegaApiIns Ins, MegaResetLocalIDStruct *pResetLocalID, int nReqID);
	///OToken录入请求
	ONE_MEGA_CAPI_EXPORT int SendOTokenInsert(OneMegaApiIns Ins, MegaOTokenInsertStruct *pOTokenInsert, int nReqID);
	///OToken操作请求
	ONE_MEGA_CAPI_EXPORT int SendOTokenAction(OneMegaApiIns Ins, MegaOTokenActionStruct *pOTokenAction, int nReqID);
	///APIKEY录入请求
	ONE_MEGA_CAPI_EXPORT int SendAPIKeyInsert(OneMegaApiIns Ins, MegaAPIKeyInsertStruct *pAPIKeyInsert, int nReqID);
	///APIKEY操作请求
	ONE_MEGA_CAPI_EXPORT int SendAPIKeyAction(OneMegaApiIns Ins, MegaAPIKeyActionStruct *pAPIKeyAction, int nReqID);
	///仓位合并操作请求
	ONE_MEGA_CAPI_EXPORT int SendPositionMerge(OneMegaApiIns Ins, MegaPositionMergeStruct *pPositionMerge, int nReqID);
	///用户入金操作请求
	ONE_MEGA_CAPI_EXPORT int SendDeposit(OneMegaApiIns Ins, MegaDepositStruct *pDeposit, int nReqID);
	///用户出金操作请求
	ONE_MEGA_CAPI_EXPORT int SendWithdraw(OneMegaApiIns Ins, MegaWithdrawStruct *pWithdraw, int nReqID);
	///用户转账操作请求
	ONE_MEGA_CAPI_EXPORT int SendTransfer(OneMegaApiIns Ins, MegaTransferStruct *pTransfer, int nReqID);
	///注册用户请求
	ONE_MEGA_CAPI_EXPORT int SendUserRegister(OneMegaApiIns Ins, MegaUserSessionStruct *pUserSession, int nReqID);
	///反向开仓录入请求
	ONE_MEGA_CAPI_EXPORT int SendPositionReverse(OneMegaApiIns Ins, MegaPositionReverseStruct *pPositionReverse, int nReqID);
	///避免自成交报单录入请求
	ONE_MEGA_CAPI_EXPORT int SendCancelOrderInsert(OneMegaApiIns Ins, MegaOrderInsertStruct *pOrderInsert, int nReqID);
	///私有主题分类请求
	ONE_MEGA_CAPI_EXPORT int SendPrivateDispatch(OneMegaApiIns Ins, MegaPrivateDispatchStruct *pPrivateDispatch, int nReqID);
	///调整多空杠杠请求
	ONE_MEGA_CAPI_EXPORT int SendPositionLeverage(OneMegaApiIns Ins, MegaPositionActionStruct *pPositionAction, int nReqID);
	///设置合约请求
	ONE_MEGA_CAPI_EXPORT int SendInstrumentUpdate(OneMegaApiIns Ins, MegaInstrumentUpdateStruct *pInstrumentUpdate, int nReqID);
	///充币兑换请求
	ONE_MEGA_CAPI_EXPORT int SendChangeCurrency(OneMegaApiIns Ins, MegaChangeCurrencyStruct *pChangeCurrency, int nReqID);
	///钱包登录获取随机数请求
	ONE_MEGA_CAPI_EXPORT int SendWalletNonce(OneMegaApiIns Ins, MegaWalletNonceStruct *pWalletNonce, int nReqID);
	///钱包登录获取随机数请求
	ONE_MEGA_CAPI_EXPORT int SendWalletLogin(OneMegaApiIns Ins, MegaWalletLoginStruct *pWalletLogin, int nReqID);
	///资金查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryAccount(OneMegaApiIns Ins, MegaQryAccountStruct *pQryAccount, int nReqID);
	///报单查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryOrder(OneMegaApiIns Ins, MegaQryOrderStruct *pQryOrder, int nReqID);
	///持仓汇总查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryMemberPosition(OneMegaApiIns Ins, MegaQryMemberPositionStruct *pQryMemberPosition, int nReqID);
	///持仓查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryPosition(OneMegaApiIns Ins, MegaQryPositionStruct *pQryPosition, int nReqID);
	///标的查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryInstrument(OneMegaApiIns Ins, MegaQryInstrumentStruct *pQryInstrument, int nReqID);
	///币种查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryCurrency(OneMegaApiIns Ins, MegaQryCurrencyStruct *pQryCurrency, int nReqID);
	///成员查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryMember(OneMegaApiIns Ins, MegaQryMemberStruct *pQryMember, int nReqID);
	///价位查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryMarketOrder(OneMegaApiIns Ins, MegaQryMarketOrderStruct *pQryMarketOrder, int nReqID);
	///APIKEY查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryAPIKey(OneMegaApiIns Ins, MegaQryAPIKeyStruct *pQryAPIKey, int nReqID);
	///保证金率查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryMarginRate(OneMegaApiIns Ins, MegaQryMarginRateStruct *pQryMarginRate, int nReqID);
	///手续费率查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryFee(OneMegaApiIns Ins, MegaQryFeeStruct *pQryFee, int nReqID);
	///触发订单查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryTriggerOrder(OneMegaApiIns Ins, MegaQryTriggerOrderStruct *pQryTriggerOrder, int nReqID);
	///杠杆倍数查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryLeverage(OneMegaApiIns Ins, MegaQryLeverageStruct *pQryLeverage, int nReqID);
	///行情查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryMarketData(OneMegaApiIns Ins, MegaQryMarketDataStruct *pQryMarketData, int nReqID);
	///成交查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryTrade(OneMegaApiIns Ins, MegaQryTradeStruct *pQryTrade, int nReqID);
	///历史委托查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryFinishOrder(OneMegaApiIns Ins, MegaQryFinishOrderStruct *pQryFinishOrder, int nReqID);
	///历史条件单查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryFinishTriggerOrder(OneMegaApiIns Ins, MegaQryFinishTriggerOrderStruct *pQryFinishTriggerOrder, int nReqID);
	///指定杠杆倍数查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryWithLeverage(OneMegaApiIns Ins, MegaQryWithLeverageStruct *pQryWithLeverage, int nReqID);
	///K线查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryKLine(OneMegaApiIns Ins, MegaQryKLineStruct *pQryKLine, int nReqID);
	///线查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryLine(OneMegaApiIns Ins, MegaQryLineStruct *pQryLine, int nReqID);
	///资金流水查询请求
	ONE_MEGA_CAPI_EXPORT int SendQryAccountDetail(OneMegaApiIns Ins, MegaQryAccountDetailStruct *pQryAccountDetail, int nReqID);
	///结算流水查询请求
	ONE_MEGA_CAPI_EXPORT int SendQrySettleDetail(OneMegaApiIns Ins, MegaQrySettleDetailStruct *pQrySettleDetail, int nReqID);
	///检查TOKEN请求
	ONE_MEGA_CAPI_EXPORT int SendCheckToken(OneMegaApiIns Ins, MegaCheckTokenStruct *pCheckToken, int nReqID);
	///批量操作请求(HTTP接口专用)
	ONE_MEGA_CAPI_EXPORT int SendBatch(OneMegaApiIns Ins, MegaCommandHeadStruct *pCommandHead, int nReqID);

#ifdef __cplusplus
}
#endif

#endif
