/////////////////////////////////////////////////////////////////////////
///定义了客户端接口实现
/////////////////////////////////////////////////////////////////////////

#ifndef _MYAPIIMPL_H
#define _MYAPIIMPL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "OneMegaApi.h"
#include "OutFogServiceApi.h"
#include "Mutex.h"

class CApiImpl :public COneMegaApi, public COutFogServiceCallback
{
public:	
	CApiImpl();
	virtual void Connect();
	virtual void DisConnect();
	virtual void SetAddress(const char *pAddress);
	virtual void SetCallback(COneMegaCallback *pCallback);
	virtual int  SetCertificateFile(const char *pCertFileName, const char *pKeyFileName, const char *pCaFileName, const char *pKeyFilePassword);

	virtual bool RspField(const TOutFogMetaRef* pRspRef, const void *pRspBuffer);

	///一般消息请求
	virtual int SendCommand(MegaCommandStruct *pCommand, int nRequestID);

	///创建上层连接请求
	virtual int SendInitLink(MegaLinkStruct *pLink, int nRequestID);

	///用户登录请求
	virtual int SendUserLogin(MegaUserLoginStruct *pUserLogin, int nRequestID);

	///用户退出请求
	virtual int SendUserLogout(MegaUserLogoutStruct *pUserLogout, int nRequestID);

	///报单录入请求
	virtual int SendOrderInsert(MegaOrderInsertStruct *pOrderInsert, int nRequestID);

	///报单操作请求
	virtual int SendOrderAction(MegaOrderActionStruct *pOrderAction, int nRequestID);

	///触发单录入请求
	virtual int SendTriggerOrderInsert(MegaTriggerOrderInsertStruct *pTriggerOrderInsert, int nRequestID);

	///触发单操作请求
	virtual int SendTriggerOrderAction(MegaTriggerOrderActionStruct *pTriggerOrderAction, int nRequestID);

	///止盈止损报单录入请求
	virtual int SendCloseOrderInsert(MegaCloseOrderInsertStruct *pCloseOrderInsert, int nRequestID);

	///止盈止损报单操作请求
	virtual int SendCloseOrderAction(MegaCloseOrderActionStruct *pCloseOrderAction, int nRequestID);

	///报价录入请求
	virtual int SendQuoteInsert(MegaQuoteInsertStruct *pQuoteInsert, int nRequestID);

	///报价操作请求
	virtual int SendQuoteAction(MegaQuoteActionStruct *pQuoteAction, int nRequestID);

	///订阅主题请求
	virtual int SendTopicAction(MegaTopicActionStruct *pTopicAction, int nRequestID);

	///设置行情请求
	virtual int SendMarketDataUpdate(MegaMarketDataUpdateStruct *pMarketDataUpdate, int nRequestID);

	///调整持仓请求
	virtual int SendPositionAction(MegaPositionActionStruct *pPositionAction, int nRequestID);

	///用户出入金请求
	virtual int SendAccountAction(MegaAccountActionStruct *pAccountAction, int nRequestID);

	///成员设置请求
	virtual int SendMemberAction(MegaMemberActionStruct *pMemberAction, int nRequestID);

	///内部做市商申报请求
	virtual int SendRobotOrder(MegaRobotOrderStruct *pRobotOrder, int nRequestID);

	///场外成交申报请求
	virtual int SendOTCTradeInsert(MegaOTCTradeInsertStruct *pOTCTradeInsert, int nRequestID);

	///资金结算请求
	virtual int SendSettlement(MegaSettlementStruct *pSettlement, int nRequestID);

	///清理行情数据请求
	virtual int SendClearMarket(MegaClearMarketStruct *pClearMarket, int nRequestID);

	///清理最新K线请求
	virtual int SendClearLastKLine(MegaClearLastKLineStruct *pClearLastKLine, int nRequestID);

	///重置LocalID请求
	virtual int SendResetLocalID(MegaResetLocalIDStruct *pResetLocalID, int nRequestID);

	///OToken录入请求
	virtual int SendOTokenInsert(MegaOTokenInsertStruct *pOTokenInsert, int nRequestID);

	///OToken操作请求
	virtual int SendOTokenAction(MegaOTokenActionStruct *pOTokenAction, int nRequestID);

	///APIKEY录入请求
	virtual int SendAPIKeyInsert(MegaAPIKeyInsertStruct *pAPIKeyInsert, int nRequestID);

	///APIKEY操作请求
	virtual int SendAPIKeyAction(MegaAPIKeyActionStruct *pAPIKeyAction, int nRequestID);

	///仓位合并操作请求
	virtual int SendPositionMerge(MegaPositionMergeStruct *pPositionMerge, int nRequestID);

	///用户入金操作请求
	virtual int SendDeposit(MegaDepositStruct *pDeposit, int nRequestID);

	///用户出金操作请求
	virtual int SendWithdraw(MegaWithdrawStruct *pWithdraw, int nRequestID);

	///用户转账操作请求
	virtual int SendTransfer(MegaTransferStruct *pTransfer, int nRequestID);

	///注册用户请求
	virtual int SendUserRegister(MegaUserSessionStruct *pUserSession, int nRequestID);

	///反向开仓录入请求
	virtual int SendPositionReverse(MegaPositionReverseStruct *pPositionReverse, int nRequestID);

	///避免自成交报单录入请求
	virtual int SendCancelOrderInsert(MegaOrderInsertStruct *pOrderInsert, int nRequestID);

	///私有主题分类请求
	virtual int SendPrivateDispatch(MegaPrivateDispatchStruct *pPrivateDispatch, int nRequestID);

	///调整多空杠杠请求
	virtual int SendPositionLeverage(MegaPositionActionStruct *pPositionAction, int nRequestID);

	///设置合约请求
	virtual int SendInstrumentUpdate(MegaInstrumentUpdateStruct *pInstrumentUpdate, int nRequestID);

	///充币兑换请求
	virtual int SendChangeCurrency(MegaChangeCurrencyStruct *pChangeCurrency, int nRequestID);

	///钱包登录获取随机数请求
	virtual int SendWalletNonce(MegaWalletNonceStruct *pWalletNonce, int nRequestID);

	///钱包登录获取随机数请求
	virtual int SendWalletLogin(MegaWalletLoginStruct *pWalletLogin, int nRequestID);

	///资金查询请求
	virtual int SendQryAccount(MegaQryAccountStruct *pQryAccount, int nRequestID);

	///报单查询请求
	virtual int SendQryOrder(MegaQryOrderStruct *pQryOrder, int nRequestID);

	///持仓汇总查询请求
	virtual int SendQryMemberPosition(MegaQryMemberPositionStruct *pQryMemberPosition, int nRequestID);

	///持仓查询请求
	virtual int SendQryPosition(MegaQryPositionStruct *pQryPosition, int nRequestID);

	///标的查询请求
	virtual int SendQryInstrument(MegaQryInstrumentStruct *pQryInstrument, int nRequestID);

	///币种查询请求
	virtual int SendQryCurrency(MegaQryCurrencyStruct *pQryCurrency, int nRequestID);

	///成员查询请求
	virtual int SendQryMember(MegaQryMemberStruct *pQryMember, int nRequestID);

	///价位查询请求
	virtual int SendQryMarketOrder(MegaQryMarketOrderStruct *pQryMarketOrder, int nRequestID);

	///APIKEY查询请求
	virtual int SendQryAPIKey(MegaQryAPIKeyStruct *pQryAPIKey, int nRequestID);

	///保证金率查询请求
	virtual int SendQryMarginRate(MegaQryMarginRateStruct *pQryMarginRate, int nRequestID);

	///手续费率查询请求
	virtual int SendQryFee(MegaQryFeeStruct *pQryFee, int nRequestID);

	///触发订单查询请求
	virtual int SendQryTriggerOrder(MegaQryTriggerOrderStruct *pQryTriggerOrder, int nRequestID);

	///杠杆倍数查询请求
	virtual int SendQryLeverage(MegaQryLeverageStruct *pQryLeverage, int nRequestID);

	///行情查询请求
	virtual int SendQryMarketData(MegaQryMarketDataStruct *pQryMarketData, int nRequestID);

	///成交查询请求
	virtual int SendQryTrade(MegaQryTradeStruct *pQryTrade, int nRequestID);

	///历史委托查询请求
	virtual int SendQryFinishOrder(MegaQryFinishOrderStruct *pQryFinishOrder, int nRequestID);

	///历史条件单查询请求
	virtual int SendQryFinishTriggerOrder(MegaQryFinishTriggerOrderStruct *pQryFinishTriggerOrder, int nRequestID);

	///指定杠杆倍数查询请求
	virtual int SendQryWithLeverage(MegaQryWithLeverageStruct *pQryWithLeverage, int nRequestID);

	///K线查询请求
	virtual int SendQryKLine(MegaQryKLineStruct *pQryKLine, int nRequestID);

	///线查询请求
	virtual int SendQryLine(MegaQryLineStruct *pQryLine, int nRequestID);

	///资金流水查询请求
	virtual int SendQryAccountDetail(MegaQryAccountDetailStruct *pQryAccountDetail, int nRequestID);

	///结算流水查询请求
	virtual int SendQrySettleDetail(MegaQrySettleDetailStruct *pQrySettleDetail, int nRequestID);

	///检查TOKEN请求
	virtual int SendCheckToken(MegaCheckTokenStruct *pCheckToken, int nRequestID);

	///批量操作请求(HTTP接口专用)
	virtual int SendBatch(MegaCommandHeadStruct *pCommandHead, int nRequestID);

private:
	COneMegaCallback* m_pCallback;
	COutFogServiceApi* m_pOutFogServiceApi;
	TOutFogMetaRef m_OutFogMetaRef;
	char m_pStreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
};

#endif
