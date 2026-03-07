/////////////////////////////////////////////////////////////////////////
///OneMegaApi.h
///定义了客户端的C++语言的API接口
/////////////////////////////////////////////////////////////////////////

#ifndef _ONE_MEGAAPI_H_
#define _ONE_MEGAAPI_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "OneMegaDataStruct.h"

#if defined(ONE_MEGAAPI_IS_LIB) && defined(WIN32)
#ifdef LIB_ONE_MEGA_API_EXPORT
#define ONE_MEGA_API_EXPORT __declspec(dllexport)
#else
#define ONE_MEGA_API_EXPORT __declspec(dllimport)
#endif
#else
#define ONE_MEGA_API_EXPORT 
#endif

namespace OneMegaByte
{
    //Callback是交易数据的回调处理类，当有数据到达客户端时，API会回调本类中对应的接口
    //客户程序需要继承这个类，并实现回调函数，实现时候回调函数内不能有阻塞性操作
    //Send开头的函数为操作命令请求函数
	//Recv开头的函数为操作命令的应答的回调
	//SendQry开头的函数为查询请求函数
	//RecvQry开头的函数为查询请求应答的回调
	//Push开头的函数为服务器主动推送回调，前提是必须通过订阅函数订阅成功主题

	class COneMegaCallback
	{
	public:
		virtual ~COneMegaCallback(){};
		///当客户端与服务端建立连接时，该方法被调用。客户端后续需发起登录请求
		virtual void OnConnected(){};
		
		///当客户端与服务端连接断开时，该方法被调用。客户端打印日志即可，可不做处理，API会自动重新连接
		///@param nReason：4907:网络读失败;4098:网络写失败;8193:接收心跳超时;8194:发送心跳失败;8195:收到错误报文
		virtual void OnDisconnected(int nReason){};

		///以下函数的共同注释：
		///@param pError:错误信息,0表示正确
		///@param nRequestNo:用户自定义请求号,在对应的Recv函数放回相同值，用于定位报文
		///@param bLast:SendQy函数会返回多条记录，bLast标明是否此类回报的最后一个报文
		///@param nLocalNo:Push函数会返回的报文属于哪个本地主题，相同的nLocalNo的后续订阅会覆盖前订阅
		///@param nSequenceNo:Push函数会返回的报文的nLocalNo为Key的主题序列号
		///一般消息应答
		virtual void RecvCommand(const MegaCommandStruct *pCommand,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///创建上层连接应答
		virtual void RecvInitLink(const MegaLinkStruct *pLink,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///用户登录应答
		virtual void RecvUserLogin(const MegaUserLoginStruct *pUserLogin,const MegaUserSessionStruct *pUserSession,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///用户退出应答
		virtual void RecvUserLogout(const MegaUserLogoutStruct *pUserLogout,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///报单录入应答
		virtual void RecvOrderInsert(const MegaOrderInsertStruct *pOrderInsert,const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///报单操作应答
		virtual void RecvOrderAction(const MegaOrderActionStruct *pOrderAction,const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///触发单录入应答
		virtual void RecvTriggerOrderInsert(const MegaTriggerOrderInsertStruct *pTriggerOrderInsert,const MegaTriggerOrderStruct *pTriggerOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///触发单操作应答
		virtual void RecvTriggerOrderAction(const MegaTriggerOrderActionStruct *pTriggerOrderAction,const MegaTriggerOrderStruct *pTriggerOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///止盈止损报单录入应答
		virtual void RecvCloseOrderInsert(const MegaCloseOrderInsertStruct *pCloseOrderInsert,const MegaOrderStruct *pOrder,const MegaTriggerOrderStruct *pTriggerOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///止盈止损报单操作应答
		virtual void RecvCloseOrderAction(const MegaCloseOrderActionStruct *pCloseOrderAction,const MegaTriggerOrderStruct *pTriggerOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///报价录入应答
		virtual void RecvQuoteInsert(const MegaQuoteInsertStruct *pQuoteInsert,const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///报价操作应答
		virtual void RecvQuoteAction(const MegaQuoteActionStruct *pQuoteAction,const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///订阅主题应答
		virtual void RecvTopicAction(const MegaTopicActionStruct *pTopicAction,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///设置行情应答
		virtual void RecvMarketDataUpdate(const MegaMarketDataUpdateStruct *pMarketDataUpdate,const MegaMarketDataStruct *pMarketData,const MegaInstrumentStruct *pInstrument,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///调整持仓应答
		virtual void RecvPositionAction(const MegaPositionActionStruct *pPositionAction,const MegaPositionStruct *pPosition,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///用户出入金应答
		virtual void RecvAccountAction(const MegaAccountActionStruct *pAccountAction,const MegaAccountStruct *pAccount,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///成员设置应答
		virtual void RecvMemberAction(const MegaMemberActionStruct *pMemberAction,const MegaMemberStruct *pMember,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///内部做市商申报应答
		virtual void RecvRobotOrder(const MegaRobotOrderStruct *pRobotOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///场外成交申报应答
		virtual void RecvOTCTradeInsert(const MegaOTCTradeInsertStruct *pOTCTradeInsert,const MegaOTCTradeStruct *pOTCTrade,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///资金结算应答
		virtual void RecvSettlement(const MegaSettlementStruct *pSettlement,const MegaSettleDetailStruct *pSettleDetail,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///清理行情数据应答
		virtual void RecvClearMarket(const MegaClearMarketStruct *pClearMarket,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///清理最新K线应答
		virtual void RecvClearLastKLine(const MegaClearLastKLineStruct *pClearLastKLine,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///重置LocalID应答
		virtual void RecvResetLocalID(const MegaResetLocalIDStruct *pResetLocalID,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///OToken录入应答
		virtual void RecvOTokenInsert(const MegaOTokenInsertStruct *pOTokenInsert,const MegaUserSessionStruct *pUserSession,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///OToken操作应答
		virtual void RecvOTokenAction(const MegaOTokenActionStruct *pOTokenAction,const MegaUserSessionStruct *pUserSession,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///APIKEY录入应答
		virtual void RecvAPIKeyInsert(const MegaAPIKeyInsertStruct *pAPIKeyInsert,const MegaAPIKeyStruct *pAPIKey,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///APIKEY操作应答
		virtual void RecvAPIKeyAction(const MegaAPIKeyActionStruct *pAPIKeyAction,const MegaAPIKeyStruct *pAPIKey,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///仓位合并操作应答
		virtual void RecvPositionMerge(const MegaPositionMergeStruct *pPositionMerge,const MegaPositionStruct *pPosition,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///用户入金操作应答
		virtual void RecvDeposit(const MegaDepositStruct *pDeposit,const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///用户出金操作应答
		virtual void RecvWithdraw(const MegaWithdrawStruct *pWithdraw,const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///用户转账操作应答
		virtual void RecvTransfer(const MegaTransferStruct *pTransfer,const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///注册用户应答
		virtual void RecvUserRegister(const MegaUserSessionStruct *pUserSession,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///反向开仓录入应答
		virtual void RecvPositionReverse(const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///避免自成交报单录入应答
		virtual void RecvCancelOrderInsert(const MegaOrderInsertStruct *pOrderInsert,const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///私有主题分类应答
		virtual void RecvPrivateDispatch(const MegaPrivateDispatchStruct *pPrivateDispatch,const MegaMemberStruct *pMember,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///调整多空杠杠应答
		virtual void RecvPositionLeverage(const MegaPositionActionStruct *pPositionAction,const MegaPositionStruct *pPosition,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///设置合约应答
		virtual void RecvInstrumentUpdate(const MegaInstrumentUpdateStruct *pInstrumentUpdate,const MegaInstrumentStruct *pInstrument,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///充币兑换应答
		virtual void RecvChangeCurrency(const MegaAccountStruct *pAccount,const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///钱包登录获取随机数应答
		virtual void RecvWalletNonce(const MegaWalletNonceStruct *pWalletNonce,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///钱包登录获取随机数应答
		virtual void RecvWalletLogin(const MegaUserSessionStruct *pUserSession,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///资金查询应答
		virtual void RecvQryAccount(const MegaAccountStruct *pAccount,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///报单查询应答
		virtual void RecvQryOrder(const MegaOrderStruct *pOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///持仓汇总查询应答
		virtual void RecvQryMemberPosition(const MegaMemberPositionStruct *pMemberPosition,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///持仓查询应答
		virtual void RecvQryPosition(const MegaPositionStruct *pPosition,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///标的查询应答
		virtual void RecvQryInstrument(const MegaInstrumentStruct *pInstrument,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///币种查询应答
		virtual void RecvQryCurrency(const MegaCurrencyStruct *pCurrency,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///成员查询应答
		virtual void RecvQryMember(const MegaMemberStruct *pMember,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///价位查询应答
		virtual void RecvQryMarketOrder(const MegaMarketOrderStruct *pMarketOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///APIKEY查询应答
		virtual void RecvQryAPIKey(const MegaAPIKeyStruct *pAPIKey,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///保证金率查询应答
		virtual void RecvQryMarginRate(const MegaMarginRateStruct *pMarginRate,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///手续费率查询应答
		virtual void RecvQryFee(const MegaFeeStruct *pFee,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///触发订单查询应答
		virtual void RecvQryTriggerOrder(const MegaTriggerOrderStruct *pTriggerOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///杠杆倍数查询应答
		virtual void RecvQryLeverage(const MegaLeverageStruct *pLeverage,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///行情查询应答
		virtual void RecvQryMarketData(const MegaMarketDataStruct *pMarketData,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///成交查询应答
		virtual void RecvQryTrade(const MegaTradeStruct *pTrade,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///历史委托查询应答
		virtual void RecvQryFinishOrder(const MegaFinishOrderStruct *pFinishOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///历史条件单查询应答
		virtual void RecvQryFinishTriggerOrder(const MegaFinishTriggerOrderStruct *pFinishTriggerOrder,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///指定杠杆倍数查询应答
		virtual void RecvQryWithLeverage(const MegaLeverageStruct *pLeverage,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///K线查询应答
		virtual void RecvQryKLine(const MegaKLineBaseStruct *pKLineBase,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///线查询应答
		virtual void RecvQryLine(const MegaLineStruct *pLine,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///资金流水查询应答
		virtual void RecvQryAccountDetail(const MegaAccountDetailStruct *pAccountDetail,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///结算流水查询应答
		virtual void RecvQrySettleDetail(const MegaSettleDetailStruct *pSettleDetail,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///检查TOKEN应答
		virtual void RecvCheckToken(const MegaCheckTokenStruct *pCheckToken,int nErrorNo,const char* pErrorMsg,int nRequestNo,char cChain){};
		///时间同步通知
		virtual void PushTimeSync(const MegaCurrentTimeStruct *pCurrentTime,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///消息通知
		virtual void PushCommand(const MegaCommandStruct *pCommand,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///成交通知
		virtual void PushTrade(const MegaTradeStruct *pTrade,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///报单通知
		virtual void PushOrder(const MegaOrderStruct *pOrder,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///持仓汇总通知
		virtual void PushMemberPosition(const MegaMemberPositionStruct *pMemberPosition,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///持仓通知
		virtual void PushPosition(const MegaPositionStruct *pPosition,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///资金通知
		virtual void PushAccount(const MegaAccountStruct *pAccount,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///市场成交通知
		virtual void PushMarketTrade(const MegaMarketTradeStruct *pMarketTrade,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///订单簿变化通知
		virtual void PushMarketOrder(const MegaMarketOrderStruct *pMarketOrder,const MegaTickMarketOrderStruct *pTickMarketOrder,const MegaCurrentTimeStruct *pCurrentTime,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///K线通知
		virtual void PushKLine(const MegaKLineStruct *pKLine,const MegaLastKLineStruct *pLastKLine,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///普通行情通知
		virtual void PushMarketOverView(const MegaMarketOverViewStruct *pMarketOverView,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///全量延时订单簿变化通知
		virtual void PushDelayMarketOrder(const MegaMarketOrderStruct *pMarketOrder,const MegaCurrentTimeStruct *pCurrentTime,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///五档深度行情通知
		virtual void PushFiveDepthMarketData(const MegaFiveDepthMarketDataStruct *pFiveDepthMarketData,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///十档深度行情通知
		virtual void PushTenDepthMarketData(const MegaTenDepthMarketDataStruct *pTenDepthMarketData,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///期权行情通知
		virtual void PushOptionsMarketData(const MegaOptionsMarketDataStruct *pOptionsMarketData,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///手续费通知
		virtual void PushFee(const MegaFeeStruct *pFee,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///保证金率通知
		virtual void PushMarginRate(const MegaMarginRateStruct *pMarginRate,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///成员通知
		virtual void PushMember(const MegaMemberStruct *pMember,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///触发单通知
		virtual void PushTriggerOrder(const MegaTriggerOrderStruct *pTriggerOrder,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///合约通知
		virtual void PushInstrument(const MegaInstrumentStruct *pInstrument,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///用户登陆通知
		virtual void PushUserSession(const MegaUserSessionStruct *pUserSession,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
		///币种通知
		virtual void PushCurrency(const MegaCurrencyStruct *pCurrency,int nLocalNo,const char* FilterValue,double nSequenceNo,double nBusinessNo,char cChain) {};
	};

	class ONE_MEGA_API_EXPORT COneMegaApi
	{
	public:
		///创建UserApi
		///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
		///@param pszUserApiType UserAPI类型
		///@return 创建出的Api实例
		static COneMegaApi *CreateApi();
		
		///获取版本号
		static const char *GetVersion();
		
		///启动接口线程,只有调用后,API才开始连接服务器
		virtual void Connect() = 0;
		
		///主动断开和服务器的连接
		virtual void DisConnect() = 0;
		
		///设置前置服务器网络地址
		///@param pAddress：前置服务器网络地址
		///@remark 网络地址的格式为："protocol://ip:port"，如："tcp://192.168.1.8:8888"
		///@remark “tcp|shm|multi”代表传输协议，"192.168.1.8"代表服务器地址。"8888"代表服务器端口号
		virtual void SetAddress(const char *pAddress) = 0;
		
		///注册回调接口
		///@param pCallback 派生自回调接口类的实例
		virtual void SetCallback(COneMegaCallback *pCallback) = 0;
		
		///加载证书
		///@param pCertFileName 用户证书文件名
		///@param pKeyFileName 用户私钥文件名
		///@param pCaFileName 可信任CA证书文件名
		///@param pKeyFilePassword 用户私钥文件密码
		///@return 0 操作成功;-1 可信任CA证书载入失败;-2 用户证书载入失败;-3 用户私钥载入失败;-4 用户证书校验失败
		virtual int SetCertificateFile(const char *pCertFileName, const char *pKeyFileName, const char *pCaFileName, const char *pKeyFilePassword) { return 0; };
		
		///以下函数共同点说明：
		///@param nRequestNo:用户自定义请求号,在对应的Recv函数处原值返回
		///@return： 0:发送成功;-1 发送线程错误;-2 服务端未回消息超标;-3 发送端发送消息数超标;-4 字段值不合法
		///一般消息请求
		virtual int SendCommand(MegaCommandStruct *pCommand, int nRequestNo) { return 0; };
		///创建上层连接请求
		virtual int SendInitLink(MegaLinkStruct *pLink, int nRequestNo) { return 0; };
		///用户登录请求
		virtual int SendUserLogin(MegaUserLoginStruct *pUserLogin, int nRequestNo) { return 0; };
		///用户退出请求
		virtual int SendUserLogout(MegaUserLogoutStruct *pUserLogout, int nRequestNo) { return 0; };
		///报单录入请求
		virtual int SendOrderInsert(MegaOrderInsertStruct *pOrderInsert, int nRequestNo) { return 0; };
		///报单操作请求
		virtual int SendOrderAction(MegaOrderActionStruct *pOrderAction, int nRequestNo) { return 0; };
		///触发单录入请求
		virtual int SendTriggerOrderInsert(MegaTriggerOrderInsertStruct *pTriggerOrderInsert, int nRequestNo) { return 0; };
		///触发单操作请求
		virtual int SendTriggerOrderAction(MegaTriggerOrderActionStruct *pTriggerOrderAction, int nRequestNo) { return 0; };
		///止盈止损报单录入请求
		virtual int SendCloseOrderInsert(MegaCloseOrderInsertStruct *pCloseOrderInsert, int nRequestNo) { return 0; };
		///止盈止损报单操作请求
		virtual int SendCloseOrderAction(MegaCloseOrderActionStruct *pCloseOrderAction, int nRequestNo) { return 0; };
		///报价录入请求
		virtual int SendQuoteInsert(MegaQuoteInsertStruct *pQuoteInsert, int nRequestNo) { return 0; };
		///报价操作请求
		virtual int SendQuoteAction(MegaQuoteActionStruct *pQuoteAction, int nRequestNo) { return 0; };
		///订阅主题请求
		virtual int SendTopicAction(MegaTopicActionStruct *pTopicAction, int nRequestNo) { return 0; };
		///设置行情请求
		virtual int SendMarketDataUpdate(MegaMarketDataUpdateStruct *pMarketDataUpdate, int nRequestNo) { return 0; };
		///调整持仓请求
		virtual int SendPositionAction(MegaPositionActionStruct *pPositionAction, int nRequestNo) { return 0; };
		///用户出入金请求
		virtual int SendAccountAction(MegaAccountActionStruct *pAccountAction, int nRequestNo) { return 0; };
		///成员设置请求
		virtual int SendMemberAction(MegaMemberActionStruct *pMemberAction, int nRequestNo) { return 0; };
		///内部做市商申报请求
		virtual int SendRobotOrder(MegaRobotOrderStruct *pRobotOrder, int nRequestNo) { return 0; };
		///场外成交申报请求
		virtual int SendOTCTradeInsert(MegaOTCTradeInsertStruct *pOTCTradeInsert, int nRequestNo) { return 0; };
		///资金结算请求
		virtual int SendSettlement(MegaSettlementStruct *pSettlement, int nRequestNo) { return 0; };
		///清理行情数据请求
		virtual int SendClearMarket(MegaClearMarketStruct *pClearMarket, int nRequestNo) { return 0; };
		///清理最新K线请求
		virtual int SendClearLastKLine(MegaClearLastKLineStruct *pClearLastKLine, int nRequestNo) { return 0; };
		///重置LocalID请求
		virtual int SendResetLocalID(MegaResetLocalIDStruct *pResetLocalID, int nRequestNo) { return 0; };
		///OToken录入请求
		virtual int SendOTokenInsert(MegaOTokenInsertStruct *pOTokenInsert, int nRequestNo) { return 0; };
		///OToken操作请求
		virtual int SendOTokenAction(MegaOTokenActionStruct *pOTokenAction, int nRequestNo) { return 0; };
		///APIKEY录入请求
		virtual int SendAPIKeyInsert(MegaAPIKeyInsertStruct *pAPIKeyInsert, int nRequestNo) { return 0; };
		///APIKEY操作请求
		virtual int SendAPIKeyAction(MegaAPIKeyActionStruct *pAPIKeyAction, int nRequestNo) { return 0; };
		///仓位合并操作请求
		virtual int SendPositionMerge(MegaPositionMergeStruct *pPositionMerge, int nRequestNo) { return 0; };
		///用户入金操作请求
		virtual int SendDeposit(MegaDepositStruct *pDeposit, int nRequestNo) { return 0; };
		///用户出金操作请求
		virtual int SendWithdraw(MegaWithdrawStruct *pWithdraw, int nRequestNo) { return 0; };
		///用户转账操作请求
		virtual int SendTransfer(MegaTransferStruct *pTransfer, int nRequestNo) { return 0; };
		///注册用户请求
		virtual int SendUserRegister(MegaUserSessionStruct *pUserSession, int nRequestNo) { return 0; };
		///反向开仓录入请求
		virtual int SendPositionReverse(MegaPositionReverseStruct *pPositionReverse, int nRequestNo) { return 0; };
		///避免自成交报单录入请求
		virtual int SendCancelOrderInsert(MegaOrderInsertStruct *pOrderInsert, int nRequestNo) { return 0; };
		///私有主题分类请求
		virtual int SendPrivateDispatch(MegaPrivateDispatchStruct *pPrivateDispatch, int nRequestNo) { return 0; };
		///调整多空杠杠请求
		virtual int SendPositionLeverage(MegaPositionActionStruct *pPositionAction, int nRequestNo) { return 0; };
		///设置合约请求
		virtual int SendInstrumentUpdate(MegaInstrumentUpdateStruct *pInstrumentUpdate, int nRequestNo) { return 0; };
		///充币兑换请求
		virtual int SendChangeCurrency(MegaChangeCurrencyStruct *pChangeCurrency, int nRequestNo) { return 0; };
		///钱包登录获取随机数请求
		virtual int SendWalletNonce(MegaWalletNonceStruct *pWalletNonce, int nRequestNo) { return 0; };
		///钱包登录获取随机数请求
		virtual int SendWalletLogin(MegaWalletLoginStruct *pWalletLogin, int nRequestNo) { return 0; };
		///资金查询请求
		virtual int SendQryAccount(MegaQryAccountStruct *pQryAccount, int nRequestNo) { return 0; };
		///报单查询请求
		virtual int SendQryOrder(MegaQryOrderStruct *pQryOrder, int nRequestNo) { return 0; };
		///持仓汇总查询请求
		virtual int SendQryMemberPosition(MegaQryMemberPositionStruct *pQryMemberPosition, int nRequestNo) { return 0; };
		///持仓查询请求
		virtual int SendQryPosition(MegaQryPositionStruct *pQryPosition, int nRequestNo) { return 0; };
		///标的查询请求
		virtual int SendQryInstrument(MegaQryInstrumentStruct *pQryInstrument, int nRequestNo) { return 0; };
		///币种查询请求
		virtual int SendQryCurrency(MegaQryCurrencyStruct *pQryCurrency, int nRequestNo) { return 0; };
		///成员查询请求
		virtual int SendQryMember(MegaQryMemberStruct *pQryMember, int nRequestNo) { return 0; };
		///价位查询请求
		virtual int SendQryMarketOrder(MegaQryMarketOrderStruct *pQryMarketOrder, int nRequestNo) { return 0; };
		///APIKEY查询请求
		virtual int SendQryAPIKey(MegaQryAPIKeyStruct *pQryAPIKey, int nRequestNo) { return 0; };
		///保证金率查询请求
		virtual int SendQryMarginRate(MegaQryMarginRateStruct *pQryMarginRate, int nRequestNo) { return 0; };
		///手续费率查询请求
		virtual int SendQryFee(MegaQryFeeStruct *pQryFee, int nRequestNo) { return 0; };
		///触发订单查询请求
		virtual int SendQryTriggerOrder(MegaQryTriggerOrderStruct *pQryTriggerOrder, int nRequestNo) { return 0; };
		///杠杆倍数查询请求
		virtual int SendQryLeverage(MegaQryLeverageStruct *pQryLeverage, int nRequestNo) { return 0; };
		///行情查询请求
		virtual int SendQryMarketData(MegaQryMarketDataStruct *pQryMarketData, int nRequestNo) { return 0; };
		///成交查询请求
		virtual int SendQryTrade(MegaQryTradeStruct *pQryTrade, int nRequestNo) { return 0; };
		///历史委托查询请求
		virtual int SendQryFinishOrder(MegaQryFinishOrderStruct *pQryFinishOrder, int nRequestNo) { return 0; };
		///历史条件单查询请求
		virtual int SendQryFinishTriggerOrder(MegaQryFinishTriggerOrderStruct *pQryFinishTriggerOrder, int nRequestNo) { return 0; };
		///指定杠杆倍数查询请求
		virtual int SendQryWithLeverage(MegaQryWithLeverageStruct *pQryWithLeverage, int nRequestNo) { return 0; };
		///K线查询请求
		virtual int SendQryKLine(MegaQryKLineStruct *pQryKLine, int nRequestNo) { return 0; };
		///线查询请求
		virtual int SendQryLine(MegaQryLineStruct *pQryLine, int nRequestNo) { return 0; };
		///资金流水查询请求
		virtual int SendQryAccountDetail(MegaQryAccountDetailStruct *pQryAccountDetail, int nRequestNo) { return 0; };
		///结算流水查询请求
		virtual int SendQrySettleDetail(MegaQrySettleDetailStruct *pQrySettleDetail, int nRequestNo) { return 0; };
		///检查TOKEN请求
		virtual int SendCheckToken(MegaCheckTokenStruct *pCheckToken, int nRequestNo) { return 0; };
		///批量操作请求(HTTP接口专用)
		virtual int SendBatch(MegaCommandHeadStruct *pCommandHead, int nRequestNo) { return 0; };

	protected:
		~COneMegaApi(){};
	};
}

using namespace OneMegaByte;

//动态调用函数
extern "C" ONE_MEGA_API_EXPORT COneMegaApi* CreateOneMegaApi();

//动态库动态调用的函数
typedef COneMegaApi* _CreateOneMegaApi();

#endif
