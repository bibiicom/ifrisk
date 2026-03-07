#ifndef __OFSWAPLINK_H_
#define __OFSWAPLINK_H_

#include "OneMegaApi.h"
#include "platform.h"
#include "OutFogServiceApi.h"
#include "ExLinkApi.h"
#include "versionValue.h"

using namespace OneMegaByte;

//交易席位接口
class COFSwapLink : public COneMegaCallback, public CExLinkApi
{
public:
	COFSwapLink(COutFogServiceCallback *pCallback);
	~COFSwapLink();

	//	void InitLink(CLinkField *pLinkField);

	virtual bool ReqField(const TOutFogMetaRef* pOutFogReqRef, const void *pStream);
	virtual bool SyncReqField(const TOutFogMetaRef* pOutFogReqRef, const void *pReqStream, TOutFogMetaRef* pOutFogRspRef, void *pRspStream) { return true; };
	//连接
	void OnConnected();
	void OnDisconnected(int nReason);
	///一般消息应答
	virtual void RecvCommand(const MegaCommandStruct *pCommand, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///创建上层连接应答
	virtual void RecvInitLink(const MegaLinkStruct *pLink, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///用户登录应答
	virtual void RecvUserLogin(const MegaUserLoginStruct *pUserLogin, const MegaUserSessionStruct *pUserSession, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///用户退出应答
	virtual void RecvUserLogout(const MegaUserLogoutStruct *pUserLogout, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///报单录入应答
	virtual void RecvOrderInsert(const MegaOrderInsertStruct *pOrderInsert, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///报单操作应答
	virtual void RecvOrderAction(const MegaOrderActionStruct *pOrderAction, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///触发单录入应答
	virtual void RecvTriggerOrderInsert(const MegaTriggerOrderInsertStruct *pTriggerOrderInsert, const MegaTriggerOrderStruct *pTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///触发单操作应答
	virtual void RecvTriggerOrderAction(const MegaTriggerOrderActionStruct *pTriggerOrderAction, const MegaTriggerOrderStruct *pTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///止盈止损报单录入应答
	virtual void RecvCloseOrderInsert(const MegaCloseOrderInsertStruct *pCloseOrderInsert, const MegaOrderStruct *pOrder, const MegaTriggerOrderStruct *pTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///止盈止损报单操作应答
	virtual void RecvCloseOrderAction(const MegaCloseOrderActionStruct *pCloseOrderAction, const MegaTriggerOrderStruct *pTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///报价录入应答
	virtual void RecvQuoteInsert(const MegaQuoteInsertStruct *pQuoteInsert, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///报价操作应答
	virtual void RecvQuoteAction(const MegaQuoteActionStruct *pQuoteAction, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///订阅主题应答
	virtual void RecvTopicAction(const MegaTopicActionStruct *pTopicAction, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///设置行情应答
	virtual void RecvMarketDataUpdate(const MegaMarketDataUpdateStruct *pMarketDataUpdate, const MegaMarketDataStruct *pMarketData, const MegaInstrumentStruct *pInstrument, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///调整持仓应答
	virtual void RecvPositionAction(const MegaPositionActionStruct *pPositionAction, const MegaPositionStruct *pPosition, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///用户出入金应答
	virtual void RecvAccountAction(const MegaAccountActionStruct *pAccountAction, const MegaAccountStruct *pAccount, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///成员设置应答
	virtual void RecvMemberAction(const MegaMemberActionStruct *pMemberAction, const MegaMemberStruct *pMember, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///内部做市商申报应答
	virtual void RecvRobotOrder(const MegaRobotOrderStruct *pRobotOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///场外成交申报应答
	virtual void RecvOTCTradeInsert(const MegaOTCTradeInsertStruct *pOTCTradeInsert, const MegaOTCTradeStruct *pOTCTrade, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///资金结算应答
	virtual void RecvSettlement(const MegaSettlementStruct *pSettlement, const MegaSettleDetailStruct *pSettleDetail, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///清理行情数据应答
	virtual void RecvClearMarket(const MegaClearMarketStruct *pClearMarket, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///清理最新K线应答
	virtual void RecvClearLastKLine(const MegaClearLastKLineStruct *pClearLastKLine, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///重置LocalID应答
	virtual void RecvResetLocalID(const MegaResetLocalIDStruct *pResetLocalID, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///OToken录入应答
	virtual void RecvOTokenInsert(const MegaOTokenInsertStruct *pOTokenInsert, const MegaUserSessionStruct *pUserSession, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///OToken操作应答
	virtual void RecvOTokenAction(const MegaOTokenActionStruct *pOTokenAction, const MegaUserSessionStruct *pUserSession, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///APIKEY录入应答
	virtual void RecvAPIKeyInsert(const MegaAPIKeyInsertStruct *pAPIKeyInsert, const MegaAPIKeyStruct *pAPIKey, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///APIKEY操作应答
	virtual void RecvAPIKeyAction(const MegaAPIKeyActionStruct *pAPIKeyAction, const MegaAPIKeyStruct *pAPIKey, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///仓位合并操作应答
	virtual void RecvPositionMerge(const MegaPositionMergeStruct *pPositionMerge, const MegaPositionStruct *pPosition, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///用户入金操作应答
	virtual void RecvDeposit(const MegaDepositStruct *pDeposit, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///用户出金操作应答
	virtual void RecvWithdraw(const MegaWithdrawStruct *pWithdraw, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///用户转账操作应答
	virtual void RecvTransfer(const MegaTransferStruct *pTransfer, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///注册用户应答
	virtual void RecvUserRegister(const MegaUserSessionStruct *pUserSession, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///反向开仓录入应答
	virtual void RecvPositionReverse(const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///避免自成交报单录入应答
	virtual void RecvCancelOrderInsert(const MegaOrderInsertStruct *pOrderInsert, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///私有主题分类应答
	virtual void RecvPrivateDispatch(const MegaPrivateDispatchStruct *pPrivateDispatch, const MegaMemberStruct *pMember, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///调整多空杠杠应答
	virtual void RecvPositionLeverage(const MegaPositionActionStruct *pPositionAction, const MegaPositionStruct *pPosition, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///设置合约应答
	virtual void RecvInstrumentUpdate(const MegaInstrumentUpdateStruct *pInstrumentUpdate, const MegaInstrumentStruct *pInstrument, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///充币兑换应答
	virtual void RecvChangeCurrency(const MegaAccountStruct *pAccount, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///钱包登录获取随机数应答
	virtual void RecvWalletNonce(const MegaWalletNonceStruct *pWalletNonce, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///钱包登录获取随机数应答
	virtual void RecvWalletLogin(const MegaUserSessionStruct *pUserSession, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///资金查询应答
	virtual void RecvQryAccount(const MegaAccountStruct *pAccount, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///报单查询应答
	virtual void RecvQryOrder(const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///持仓汇总查询应答
	virtual void RecvQryMemberPosition(const MegaMemberPositionStruct *pMemberPosition, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///持仓查询应答
	virtual void RecvQryPosition(const MegaPositionStruct *pPosition, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///标的查询应答
	virtual void RecvQryInstrument(const MegaInstrumentStruct *pInstrument, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///币种查询应答
	virtual void RecvQryCurrency(const MegaCurrencyStruct *pCurrency, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///成员查询应答
	virtual void RecvQryMember(const MegaMemberStruct *pMember, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///价位查询应答
	virtual void RecvQryMarketOrder(const MegaMarketOrderStruct *pMarketOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///APIKEY查询应答
	virtual void RecvQryAPIKey(const MegaAPIKeyStruct *pAPIKey, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///保证金率查询应答
	virtual void RecvQryMarginRate(const MegaMarginRateStruct *pMarginRate, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///手续费率查询应答
	virtual void RecvQryFee(const MegaFeeStruct *pFee, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///触发订单查询应答
	virtual void RecvQryTriggerOrder(const MegaTriggerOrderStruct *pTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///杠杆倍数查询应答
	virtual void RecvQryLeverage(const MegaLeverageStruct *pLeverage, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///行情查询应答
	virtual void RecvQryMarketData(const MegaMarketDataStruct *pMarketData, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///成交查询应答
	virtual void RecvQryTrade(const MegaTradeStruct *pTrade, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///历史委托查询应答
	virtual void RecvQryFinishOrder(const MegaFinishOrderStruct *pFinishOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///历史条件单查询应答
	virtual void RecvQryFinishTriggerOrder(const MegaFinishTriggerOrderStruct *pFinishTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///指定杠杆倍数查询应答
	virtual void RecvQryWithLeverage(const MegaLeverageStruct *pLeverage, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///K线查询应答
	virtual void RecvQryKLine(const MegaKLineBaseStruct *pKLineBase, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///线查询应答
	virtual void RecvQryLine(const MegaLineStruct *pLine, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///资金流水查询应答
	virtual void RecvQryAccountDetail(const MegaAccountDetailStruct *pAccountDetail, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///结算流水查询应答
	virtual void RecvQrySettleDetail(const MegaSettleDetailStruct *pSettleDetail, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///检查TOKEN应答
	virtual void RecvCheckToken(const MegaCheckTokenStruct *pCheckToken, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
	///时间同步通知
	virtual void PushTimeSync(const MegaCurrentTimeStruct *pCurrentTime, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///消息通知
	virtual void PushCommand(const MegaCommandStruct *pCommand, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///成交通知
	virtual void PushTrade(const MegaTradeStruct *pTrade, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///报单通知
	virtual void PushOrder(const MegaOrderStruct *pOrder, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///持仓汇总通知
	virtual void PushMemberPosition(const MegaMemberPositionStruct *pMemberPosition, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///持仓通知
	virtual void PushPosition(const MegaPositionStruct *pPosition, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///资金通知
	virtual void PushAccount(const MegaAccountStruct *pAccount, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///市场成交通知
	virtual void PushMarketTrade(const MegaMarketTradeStruct *pMarketTrade, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///订单簿变化通知
	virtual void PushMarketOrder(const MegaMarketOrderStruct *pMarketOrder, const MegaTickMarketOrderStruct *pTickMarketOrder, const MegaCurrentTimeStruct *pCurrentTime, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///K线通知
	virtual void PushKLine(const MegaKLineStruct *pKLine, const MegaLastKLineStruct *pLastKLine, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///普通行情通知
	virtual void PushMarketOverView(const MegaMarketOverViewStruct *pMarketOverView, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///全量延时订单簿变化通知
	virtual void PushDelayMarketOrder(const MegaMarketOrderStruct *pMarketOrder, const MegaCurrentTimeStruct *pCurrentTime, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///五档深度行情通知
	virtual void PushFiveDepthMarketData(const MegaFiveDepthMarketDataStruct *pFiveDepthMarketData, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///十档深度行情通知
	virtual void PushTenDepthMarketData(const MegaTenDepthMarketDataStruct *pTenDepthMarketData, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///期权行情通知
	virtual void PushOptionsMarketData(const MegaOptionsMarketDataStruct *pOptionsMarketData, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///手续费通知
	virtual void PushFee(const MegaFeeStruct *pFee, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///保证金率通知
	virtual void PushMarginRate(const MegaMarginRateStruct *pMarginRate, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///成员通知
	virtual void PushMember(const MegaMemberStruct *pMember, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///触发单通知
	virtual void PushTriggerOrder(const MegaTriggerOrderStruct *pTriggerOrder, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///合约通知
	virtual void PushInstrument(const MegaInstrumentStruct *pInstrument, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///用户登陆通知
	virtual void PushUserSession(const MegaUserSessionStruct *pUserSession, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
	///币种通知
	virtual void PushCurrency(const MegaCurrencyStruct *pCurrency, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);

	virtual const char* getVersion()
	{
		return INNER_BASE_VERSION;
	}

protected:
	void QueryFinish();
	void Login();
	bool InitLink(CLinkField *pLinkField, int nRequestNo);
	COneMegaApi* GetDll(const char * pName);

private:
	COneMegaApi *m_pApi;
};

#endif
