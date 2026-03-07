
#include "LinkSwap.h"
#include "public.h"

COFSwapLink::COFSwapLink(COutFogServiceCallback *pCallback):CExLinkApi(pCallback)
{
	memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
}

COFSwapLink::~COFSwapLink()
{
}

bool COFSwapLink::ReqField(const TOutFogMetaRef* pOutFogReqRef, const void *pStream)
{
	int ret = 0;
	string errorMsg;
	switch (pOutFogReqRef->MessageNo)
	{
	case TID_SendCommand: ///一般消息请求
	{
		CCommandField field;
		CCommandField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendCommand((MegaCommandStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "Command";
		break;
	}
	case TID_SendInitLink: ///创建上层连接请求
	{
		CLinkField field;
		CLinkField::m_Describe.getSingleField(&field, pStream);
		InitLink(&field, pOutFogReqRef->RequestNo);
		errorMsg = "Link";
		break;
	}
	case TID_SendUserLogin: ///用户登录请求
	{
		CUserLoginField field;
		CUserLoginField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendUserLogin((MegaUserLoginStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "UserLogin";
		break;
	}
	case TID_SendUserLogout: ///用户退出请求
	{
		CUserLogoutField field;
		CUserLogoutField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendUserLogout((MegaUserLogoutStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "UserLogout";
		break;
	}
	case TID_SendOrderInsert: ///报单录入请求
	{
		COrderInsertField field;
		COrderInsertField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendOrderInsert((MegaOrderInsertStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "OrderInsert";
		break;
	}
	case TID_SendOrderAction: ///报单操作请求
	{
		COrderActionField field;
		COrderActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendOrderAction((MegaOrderActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "OrderAction";
		break;
	}
	case TID_SendTriggerOrderInsert: ///触发单录入请求
	{
		CTriggerOrderInsertField field;
		CTriggerOrderInsertField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendTriggerOrderInsert((MegaTriggerOrderInsertStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "TriggerOrderInsert";
		break;
	}
	case TID_SendTriggerOrderAction: ///触发单操作请求
	{
		CTriggerOrderActionField field;
		CTriggerOrderActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendTriggerOrderAction((MegaTriggerOrderActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "TriggerOrderAction";
		break;
	}
	case TID_SendCloseOrderInsert: ///止盈止损报单录入请求
	{
		CCloseOrderInsertField field;
		CCloseOrderInsertField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendCloseOrderInsert((MegaCloseOrderInsertStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "CloseOrderInsert";
		break;
	}
	case TID_SendCloseOrderAction: ///止盈止损报单操作请求
	{
		CCloseOrderActionField field;
		CCloseOrderActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendCloseOrderAction((MegaCloseOrderActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "CloseOrderAction";
		break;
	}
	case TID_SendQuoteInsert: ///报价录入请求
	{
		CQuoteInsertField field;
		CQuoteInsertField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQuoteInsert((MegaQuoteInsertStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QuoteInsert";
		break;
	}
	case TID_SendQuoteAction: ///报价操作请求
	{
		CQuoteActionField field;
		CQuoteActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQuoteAction((MegaQuoteActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QuoteAction";
		break;
	}
	case TID_SendTopicAction: ///订阅主题请求
	{
		CTopicActionField field;
		CTopicActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendTopicAction((MegaTopicActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "TopicAction";
		break;
	}
	case TID_SendMarketDataUpdate: ///设置行情请求
	{
		CMarketDataUpdateField field;
		CMarketDataUpdateField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendMarketDataUpdate((MegaMarketDataUpdateStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "MarketDataUpdate";
		break;
	}
	case TID_SendPositionAction: ///调整持仓请求
	{
		CPositionActionField field;
		CPositionActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendPositionAction((MegaPositionActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "PositionAction";
		break;
	}
	case TID_SendAccountAction: ///用户出入金请求
	{
		CAccountActionField field;
		CAccountActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendAccountAction((MegaAccountActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "AccountAction";
		break;
	}
	case TID_SendMemberAction: ///成员设置请求
	{
		CMemberActionField field;
		CMemberActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendMemberAction((MegaMemberActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "MemberAction";
		break;
	}
	case TID_SendRobotOrder: ///内部做市商申报请求
	{
		CRobotOrderField field;
		CRobotOrderField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendRobotOrder((MegaRobotOrderStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "RobotOrder";
		break;
	}
	case TID_SendOTCTradeInsert: ///场外成交申报请求
	{
		COTCTradeInsertField field;
		COTCTradeInsertField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendOTCTradeInsert((MegaOTCTradeInsertStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "OTCTradeInsert";
		break;
	}
	case TID_SendSettlement: ///资金结算请求
	{
		CSettlementField field;
		CSettlementField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendSettlement((MegaSettlementStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "Settlement";
		break;
	}
	case TID_SendClearMarket: ///清理行情数据请求
	{
		CClearMarketField field;
		CClearMarketField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendClearMarket((MegaClearMarketStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "ClearMarket";
		break;
	}
	case TID_SendClearLastKLine: ///清理最新K线请求
	{
		CClearLastKLineField field;
		CClearLastKLineField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendClearLastKLine((MegaClearLastKLineStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "ClearLastKLine";
		break;
	}
	case TID_SendResetLocalID: ///重置LocalID请求
	{
		CResetLocalIDField field;
		CResetLocalIDField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendResetLocalID((MegaResetLocalIDStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "ResetLocalID";
		break;
	}
	case TID_SendOTokenInsert: ///OToken录入请求
	{
		COTokenInsertField field;
		COTokenInsertField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendOTokenInsert((MegaOTokenInsertStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "OTokenInsert";
		break;
	}
	case TID_SendOTokenAction: ///OToken操作请求
	{
		COTokenActionField field;
		COTokenActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendOTokenAction((MegaOTokenActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "OTokenAction";
		break;
	}
	case TID_SendAPIKeyInsert: ///APIKEY录入请求
	{
		CAPIKeyInsertField field;
		CAPIKeyInsertField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendAPIKeyInsert((MegaAPIKeyInsertStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "APIKeyInsert";
		break;
	}
	case TID_SendAPIKeyAction: ///APIKEY操作请求
	{
		CAPIKeyActionField field;
		CAPIKeyActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendAPIKeyAction((MegaAPIKeyActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "APIKeyAction";
		break;
	}
	case TID_SendPositionMerge: ///仓位合并操作请求
	{
		CPositionMergeField field;
		CPositionMergeField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendPositionMerge((MegaPositionMergeStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "PositionMerge";
		break;
	}
	case TID_SendDeposit: ///用户入金操作请求
	{
		CDepositField field;
		CDepositField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendDeposit((MegaDepositStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "Deposit";
		break;
	}
	case TID_SendWithdraw: ///用户出金操作请求
	{
		CWithdrawField field;
		CWithdrawField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendWithdraw((MegaWithdrawStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "Withdraw";
		break;
	}
	case TID_SendTransfer: ///用户转账操作请求
	{
		CTransferField field;
		CTransferField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendTransfer((MegaTransferStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "Transfer";
		break;
	}
	case TID_SendUserRegister: ///注册用户请求
	{
		CUserSessionField field;
		CUserSessionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendUserRegister((MegaUserSessionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "UserSession";
		break;
	}
	case TID_SendPositionReverse: ///反向开仓录入请求
	{
		CPositionReverseField field;
		CPositionReverseField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendPositionReverse((MegaPositionReverseStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "PositionReverse";
		break;
	}
	case TID_SendCancelOrderInsert: ///避免自成交报单录入请求
	{
		COrderInsertField field;
		COrderInsertField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendCancelOrderInsert((MegaOrderInsertStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "OrderInsert";
		break;
	}
	case TID_SendPrivateDispatch: ///私有主题分类请求
	{
		CPrivateDispatchField field;
		CPrivateDispatchField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendPrivateDispatch((MegaPrivateDispatchStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "PrivateDispatch";
		break;
	}
	case TID_SendPositionLeverage: ///调整多空杠杠请求
	{
		CPositionActionField field;
		CPositionActionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendPositionLeverage((MegaPositionActionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "PositionAction";
		break;
	}
	case TID_SendInstrumentUpdate: ///设置合约请求
	{
		CInstrumentUpdateField field;
		CInstrumentUpdateField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendInstrumentUpdate((MegaInstrumentUpdateStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "InstrumentUpdate";
		break;
	}
	case TID_SendChangeCurrency: ///充币兑换请求
	{
		CChangeCurrencyField field;
		CChangeCurrencyField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendChangeCurrency((MegaChangeCurrencyStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "ChangeCurrency";
		break;
	}
	case TID_SendWalletNonce: ///钱包登录获取随机数请求
	{
		CWalletNonceField field;
		CWalletNonceField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendWalletNonce((MegaWalletNonceStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "WalletNonce";
		break;
	}
	case TID_SendWalletLogin: ///钱包登录获取随机数请求
	{
		CWalletLoginField field;
		CWalletLoginField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendWalletLogin((MegaWalletLoginStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "WalletLogin";
		break;
	}
	case TID_SendQryAccount: ///资金查询请求
	{
		CQryAccountField field;
		CQryAccountField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryAccount((MegaQryAccountStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryAccount";
		break;
	}
	case TID_SendQryOrder: ///报单查询请求
	{
		CQryOrderField field;
		CQryOrderField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryOrder((MegaQryOrderStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryOrder";
		break;
	}
	case TID_SendQryMemberPosition: ///持仓汇总查询请求
	{
		CQryMemberPositionField field;
		CQryMemberPositionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryMemberPosition((MegaQryMemberPositionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryMemberPosition";
		break;
	}
	case TID_SendQryPosition: ///持仓查询请求
	{
		CQryPositionField field;
		CQryPositionField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryPosition((MegaQryPositionStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryPosition";
		break;
	}
	case TID_SendQryInstrument: ///标的查询请求
	{
		CQryInstrumentField field;
		CQryInstrumentField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryInstrument((MegaQryInstrumentStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryInstrument";
		break;
	}
	case TID_SendQryCurrency: ///币种查询请求
	{
		CQryCurrencyField field;
		CQryCurrencyField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryCurrency((MegaQryCurrencyStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryCurrency";
		break;
	}
	case TID_SendQryMember: ///成员查询请求
	{
		CQryMemberField field;
		CQryMemberField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryMember((MegaQryMemberStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryMember";
		break;
	}
	case TID_SendQryMarketOrder: ///价位查询请求
	{
		CQryMarketOrderField field;
		CQryMarketOrderField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryMarketOrder((MegaQryMarketOrderStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryMarketOrder";
		break;
	}
	case TID_SendQryAPIKey: ///APIKEY查询请求
	{
		CQryAPIKeyField field;
		CQryAPIKeyField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryAPIKey((MegaQryAPIKeyStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryAPIKey";
		break;
	}
	case TID_SendQryMarginRate: ///保证金率查询请求
	{
		CQryMarginRateField field;
		CQryMarginRateField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryMarginRate((MegaQryMarginRateStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryMarginRate";
		break;
	}
	case TID_SendQryFee: ///手续费率查询请求
	{
		CQryFeeField field;
		CQryFeeField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryFee((MegaQryFeeStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryFee";
		break;
	}
	case TID_SendQryTriggerOrder: ///触发订单查询请求
	{
		CQryTriggerOrderField field;
		CQryTriggerOrderField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryTriggerOrder((MegaQryTriggerOrderStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryTriggerOrder";
		break;
	}
	case TID_SendQryLeverage: ///杠杆倍数查询请求
	{
		CQryLeverageField field;
		CQryLeverageField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryLeverage((MegaQryLeverageStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryLeverage";
		break;
	}
	case TID_SendQryMarketData: ///行情查询请求
	{
		CQryMarketDataField field;
		CQryMarketDataField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryMarketData((MegaQryMarketDataStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryMarketData";
		break;
	}
	case TID_SendQryTrade: ///成交查询请求
	{
		CQryTradeField field;
		CQryTradeField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryTrade((MegaQryTradeStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryTrade";
		break;
	}
	case TID_SendQryFinishOrder: ///历史委托查询请求
	{
		CQryFinishOrderField field;
		CQryFinishOrderField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryFinishOrder((MegaQryFinishOrderStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryFinishOrder";
		break;
	}
	case TID_SendQryFinishTriggerOrder: ///历史条件单查询请求
	{
		CQryFinishTriggerOrderField field;
		CQryFinishTriggerOrderField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryFinishTriggerOrder((MegaQryFinishTriggerOrderStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryFinishTriggerOrder";
		break;
	}
	case TID_SendQryWithLeverage: ///指定杠杆倍数查询请求
	{
		CQryWithLeverageField field;
		CQryWithLeverageField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryWithLeverage((MegaQryWithLeverageStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryWithLeverage";
		break;
	}
	case TID_SendQryKLine: ///K线查询请求
	{
		CQryKLineField field;
		CQryKLineField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryKLine((MegaQryKLineStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryKLine";
		break;
	}
	case TID_SendQryLine: ///线查询请求
	{
		CQryLineField field;
		CQryLineField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryLine((MegaQryLineStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryLine";
		break;
	}
	case TID_SendQryAccountDetail: ///资金流水查询请求
	{
		CQryAccountDetailField field;
		CQryAccountDetailField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQryAccountDetail((MegaQryAccountDetailStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QryAccountDetail";
		break;
	}
	case TID_SendQrySettleDetail: ///结算流水查询请求
	{
		CQrySettleDetailField field;
		CQrySettleDetailField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendQrySettleDetail((MegaQrySettleDetailStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "QrySettleDetail";
		break;
	}
	case TID_SendCheckToken: ///检查TOKEN请求
	{
		CCheckTokenField field;
		CCheckTokenField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendCheckToken((MegaCheckTokenStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "CheckToken";
		break;
	}
	case TID_SendBatch: ///批量操作请求(HTTP接口专用)
	{
		CCommandHeadField field;
		CCommandHeadField::m_Describe.getSingleField(&field, pStream);
				ret = m_pApi->SendBatch((MegaCommandHeadStruct*)&field, pOutFogReqRef->RequestNo);	
		errorMsg = "CommandHead";
		break;
	}
	default:
	{
		PRINT_TO_STD("%s,Send Req [%0X] Error! no such method", m_sIdInfo, pOutFogReqRef->MessageNo);
	}
	}
	if (ret != 0)
		PRINT_TO_STD("%s,Send Req [%s] Error! ret=%d", m_sIdInfo, errorMsg.c_str(), ret);
	return true;
}

bool COFSwapLink::InitLink(CLinkField *pLinkField, int nRequestNo)
{
	CExLinkApi::InitLink(pLinkField);
	m_pApi = GetDll(pLinkField->LinkDllLoad);
	if (m_pApi == NULL)
	{
		PRINT_TO_STD("Load %s error", m_sIdInfo, pLinkField->LinkDllLoad.getValue());
		return false;
	}
	m_pApi->SetCallback(this);
	m_pApi->SendInitLink((MegaLinkStruct*)pLinkField, nRequestNo);

	PRINT_TO_STD("%s,InitLink,LinkAddress:%s", m_sIdInfo, pLinkField->LinkAddress.getValue());
	m_pApi->SetAddress(pLinkField->LinkAddress.getValue());
	m_pApi->Connect();
	return true;
}

void COFSwapLink::OnConnected()
{
	PRINT_TO_STD("%s,OnConnected:Success!", m_sIdInfo);
	m_bConnect = true;
	Login();
}

void COFSwapLink::OnDisconnected(int nReason)
{
	PRINT_TO_STD("%s,OnDisConnected,Reason=[%d]", m_sIdInfo, nReason);
	m_bConnect = false;
	m_bLogin = false;
	m_DisconnectNum++;
	//	SendLoginStatus(SS_Logout);
	//	SendDisConnetMessageNotify();
}

void COFSwapLink::Login()
{
	if (m_LinkField.LinkUserID.isNull() && m_LinkField.LinkPassword.isNull())
	{
		PRINT_TO_STD("%s,NoNeedToLogin!", m_sIdInfo);
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
		m_OutFogMetaRef.MessageNo = TID_RecvUserLogin;
		m_OutFogMetaRef.RequestNo = 0;
		m_OutFogMetaRef.SubjectNo = 0;
		strcpy(m_OutFogMetaRef.Remark, "Success");
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
		return;
	}
	MegaUserLoginStruct reqUserLogin;
	memset(&reqUserLogin, 0, sizeof(reqUserLogin));
	strncpy(reqUserLogin.UserID, m_LinkField.LinkUserID.getValue(), sizeof(reqUserLogin.UserID));
	strncpy(reqUserLogin.Password, m_LinkField.LinkPassword.getValue(), sizeof(reqUserLogin.Password));
	strcpy(reqUserLogin.UserProductID, ProductInfo);
	vector<string> Configs = Txtsplit(m_LinkField.Config.getValue(), ",");
	for (int i = 0; i < Configs.size(); i++)
	{
		vector<string> eachConfigs = Txtsplit(Configs[i], ":");
		if (eachConfigs.size()==2 && eachConfigs[0] == "APPID")
			strncpy(reqUserLogin.APPID, eachConfigs[1].c_str(), sizeof(reqUserLogin.APPID));
	}
	
	int ret = m_pApi->SendUserLogin(&reqUserLogin, 0);
	if (ret != 0)
		PRINT_TO_STD("%s,Login:Fail!,ret=%d", m_sIdInfo, ret);
	else
		PRINT_TO_STD("%s,Login:Success!", m_sIdInfo);
}

void COFSwapLink::RecvCommand(const MegaCommandStruct *pCommand, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvCommand;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pCommand != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CCommandField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CCommandField::m_Describe.m_FieldID;	
		}
		CCommandField::m_Describe.setSingleField(pStreamBuf, pCommand);
		m_OutFogMetaRef.FieldNo = CCommandField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CCommandField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pCommand == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvInitLink(const MegaLinkStruct *pLink, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvInitLink;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pLink != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CLinkField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CLinkField::m_Describe.m_FieldID;	
		}
		CLinkField::m_Describe.setSingleField(pStreamBuf, pLink);
		m_OutFogMetaRef.FieldNo = CLinkField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CLinkField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pLink == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvUserLogin(const MegaUserLoginStruct *pUserLogin, const MegaUserSessionStruct *pUserSession, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvUserLogin;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pUserLogin != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CUserLoginField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CUserLoginField::m_Describe.m_FieldID;	
		}
		CUserLoginField::m_Describe.setSingleField(pStreamBuf, pUserLogin);
		m_OutFogMetaRef.FieldNo = CUserLoginField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CUserLoginField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pUserSession != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CUserSessionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;	
		}
		CUserSessionField::m_Describe.setSingleField(pStreamBuf, pUserSession);
		m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CUserSessionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pUserLogin == NULL && pUserSession == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvUserLogout(const MegaUserLogoutStruct *pUserLogout, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvUserLogout;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pUserLogout != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CUserLogoutField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CUserLogoutField::m_Describe.m_FieldID;	
		}
		CUserLogoutField::m_Describe.setSingleField(pStreamBuf, pUserLogout);
		m_OutFogMetaRef.FieldNo = CUserLogoutField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CUserLogoutField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pUserLogout == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvOrderInsert(const MegaOrderInsertStruct *pOrderInsert, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvOrderInsert;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pOrderInsert != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderInsertField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderInsertField::m_Describe.m_FieldID;	
		}
		COrderInsertField::m_Describe.setSingleField(pStreamBuf, pOrderInsert);
		m_OutFogMetaRef.FieldNo = COrderInsertField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderInsertField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrderInsert == NULL && pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvOrderAction(const MegaOrderActionStruct *pOrderAction, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvOrderAction;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pOrderAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderActionField::m_Describe.m_FieldID;	
		}
		COrderActionField::m_Describe.setSingleField(pStreamBuf, pOrderAction);
		m_OutFogMetaRef.FieldNo = COrderActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrderAction == NULL && pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvTriggerOrderInsert(const MegaTriggerOrderInsertStruct *pTriggerOrderInsert, const MegaTriggerOrderStruct *pTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvTriggerOrderInsert;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pTriggerOrderInsert != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CTriggerOrderInsertField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CTriggerOrderInsertField::m_Describe.m_FieldID;	
		}
		CTriggerOrderInsertField::m_Describe.setSingleField(pStreamBuf, pTriggerOrderInsert);
		m_OutFogMetaRef.FieldNo = CTriggerOrderInsertField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CTriggerOrderInsertField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTriggerOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CTriggerOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;	
		}
		CTriggerOrderField::m_Describe.setSingleField(pStreamBuf, pTriggerOrder);
		m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CTriggerOrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTriggerOrderInsert == NULL && pTriggerOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvTriggerOrderAction(const MegaTriggerOrderActionStruct *pTriggerOrderAction, const MegaTriggerOrderStruct *pTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvTriggerOrderAction;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pTriggerOrderAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CTriggerOrderActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CTriggerOrderActionField::m_Describe.m_FieldID;	
		}
		CTriggerOrderActionField::m_Describe.setSingleField(pStreamBuf, pTriggerOrderAction);
		m_OutFogMetaRef.FieldNo = CTriggerOrderActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CTriggerOrderActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTriggerOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CTriggerOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;	
		}
		CTriggerOrderField::m_Describe.setSingleField(pStreamBuf, pTriggerOrder);
		m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CTriggerOrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTriggerOrderAction == NULL && pTriggerOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvCloseOrderInsert(const MegaCloseOrderInsertStruct *pCloseOrderInsert, const MegaOrderStruct *pOrder, const MegaTriggerOrderStruct *pTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvCloseOrderInsert;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pCloseOrderInsert != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CCloseOrderInsertField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CCloseOrderInsertField::m_Describe.m_FieldID;	
		}
		CCloseOrderInsertField::m_Describe.setSingleField(pStreamBuf, pCloseOrderInsert);
		m_OutFogMetaRef.FieldNo = CCloseOrderInsertField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CCloseOrderInsertField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTriggerOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CTriggerOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;	
		}
		CTriggerOrderField::m_Describe.setSingleField(pStreamBuf, pTriggerOrder);
		m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CTriggerOrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pCloseOrderInsert == NULL && pOrder == NULL && pTriggerOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvCloseOrderAction(const MegaCloseOrderActionStruct *pCloseOrderAction, const MegaTriggerOrderStruct *pTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvCloseOrderAction;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pCloseOrderAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CCloseOrderActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CCloseOrderActionField::m_Describe.m_FieldID;	
		}
		CCloseOrderActionField::m_Describe.setSingleField(pStreamBuf, pCloseOrderAction);
		m_OutFogMetaRef.FieldNo = CCloseOrderActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CCloseOrderActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTriggerOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CTriggerOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;	
		}
		CTriggerOrderField::m_Describe.setSingleField(pStreamBuf, pTriggerOrder);
		m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CTriggerOrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pCloseOrderAction == NULL && pTriggerOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQuoteInsert(const MegaQuoteInsertStruct *pQuoteInsert, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQuoteInsert;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pQuoteInsert != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CQuoteInsertField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CQuoteInsertField::m_Describe.m_FieldID;	
		}
		CQuoteInsertField::m_Describe.setSingleField(pStreamBuf, pQuoteInsert);
		m_OutFogMetaRef.FieldNo = CQuoteInsertField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CQuoteInsertField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pQuoteInsert == NULL && pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQuoteAction(const MegaQuoteActionStruct *pQuoteAction, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQuoteAction;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pQuoteAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CQuoteActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CQuoteActionField::m_Describe.m_FieldID;	
		}
		CQuoteActionField::m_Describe.setSingleField(pStreamBuf, pQuoteAction);
		m_OutFogMetaRef.FieldNo = CQuoteActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CQuoteActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pQuoteAction == NULL && pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvTopicAction(const MegaTopicActionStruct *pTopicAction, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvTopicAction;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pTopicAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CTopicActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CTopicActionField::m_Describe.m_FieldID;	
		}
		CTopicActionField::m_Describe.setSingleField(pStreamBuf, pTopicAction);
		m_OutFogMetaRef.FieldNo = CTopicActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CTopicActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTopicAction == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvMarketDataUpdate(const MegaMarketDataUpdateStruct *pMarketDataUpdate, const MegaMarketDataStruct *pMarketData, const MegaInstrumentStruct *pInstrument, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvMarketDataUpdate;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pMarketDataUpdate != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CMarketDataUpdateField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CMarketDataUpdateField::m_Describe.m_FieldID;	
		}
		CMarketDataUpdateField::m_Describe.setSingleField(pStreamBuf, pMarketDataUpdate);
		m_OutFogMetaRef.FieldNo = CMarketDataUpdateField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CMarketDataUpdateField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pMarketData != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CMarketDataField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CMarketDataField::m_Describe.m_FieldID;	
		}
		CMarketDataField::m_Describe.setSingleField(pStreamBuf, pMarketData);
		m_OutFogMetaRef.FieldNo = CMarketDataField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CMarketDataField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pInstrument != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CInstrumentField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CInstrumentField::m_Describe.m_FieldID;	
		}
		CInstrumentField::m_Describe.setSingleField(pStreamBuf, pInstrument);
		m_OutFogMetaRef.FieldNo = CInstrumentField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CInstrumentField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pMarketDataUpdate == NULL && pMarketData == NULL && pInstrument == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvPositionAction(const MegaPositionActionStruct *pPositionAction, const MegaPositionStruct *pPosition, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvPositionAction;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pPositionAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CPositionActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CPositionActionField::m_Describe.m_FieldID;	
		}
		CPositionActionField::m_Describe.setSingleField(pStreamBuf, pPositionAction);
		m_OutFogMetaRef.FieldNo = CPositionActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CPositionActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pPosition != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CPositionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;	
		}
		CPositionField::m_Describe.setSingleField(pStreamBuf, pPosition);
		m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CPositionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pPositionAction == NULL && pPosition == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvAccountAction(const MegaAccountActionStruct *pAccountAction, const MegaAccountStruct *pAccount, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvAccountAction;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pAccountAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CAccountActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CAccountActionField::m_Describe.m_FieldID;	
		}
		CAccountActionField::m_Describe.setSingleField(pStreamBuf, pAccountAction);
		m_OutFogMetaRef.FieldNo = CAccountActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CAccountActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pAccount != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CAccountField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CAccountField::m_Describe.m_FieldID;	
		}
		CAccountField::m_Describe.setSingleField(pStreamBuf, pAccount);
		m_OutFogMetaRef.FieldNo = CAccountField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CAccountField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pAccountAction == NULL && pAccount == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvMemberAction(const MegaMemberActionStruct *pMemberAction, const MegaMemberStruct *pMember, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvMemberAction;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pMemberAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CMemberActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CMemberActionField::m_Describe.m_FieldID;	
		}
		CMemberActionField::m_Describe.setSingleField(pStreamBuf, pMemberAction);
		m_OutFogMetaRef.FieldNo = CMemberActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CMemberActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pMember != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CMemberField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CMemberField::m_Describe.m_FieldID;	
		}
		CMemberField::m_Describe.setSingleField(pStreamBuf, pMember);
		m_OutFogMetaRef.FieldNo = CMemberField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CMemberField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pMemberAction == NULL && pMember == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvRobotOrder(const MegaRobotOrderStruct *pRobotOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvRobotOrder;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pRobotOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CRobotOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CRobotOrderField::m_Describe.m_FieldID;	
		}
		CRobotOrderField::m_Describe.setSingleField(pStreamBuf, pRobotOrder);
		m_OutFogMetaRef.FieldNo = CRobotOrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CRobotOrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pRobotOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvOTCTradeInsert(const MegaOTCTradeInsertStruct *pOTCTradeInsert, const MegaOTCTradeStruct *pOTCTrade, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvOTCTradeInsert;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pOTCTradeInsert != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COTCTradeInsertField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COTCTradeInsertField::m_Describe.m_FieldID;	
		}
		COTCTradeInsertField::m_Describe.setSingleField(pStreamBuf, pOTCTradeInsert);
		m_OutFogMetaRef.FieldNo = COTCTradeInsertField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COTCTradeInsertField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOTCTrade != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COTCTradeField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COTCTradeField::m_Describe.m_FieldID;	
		}
		COTCTradeField::m_Describe.setSingleField(pStreamBuf, pOTCTrade);
		m_OutFogMetaRef.FieldNo = COTCTradeField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COTCTradeField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOTCTradeInsert == NULL && pOTCTrade == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvSettlement(const MegaSettlementStruct *pSettlement, const MegaSettleDetailStruct *pSettleDetail, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvSettlement;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pSettlement != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CSettlementField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CSettlementField::m_Describe.m_FieldID;	
		}
		CSettlementField::m_Describe.setSingleField(pStreamBuf, pSettlement);
		m_OutFogMetaRef.FieldNo = CSettlementField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CSettlementField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pSettleDetail != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CSettleDetailField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CSettleDetailField::m_Describe.m_FieldID;	
		}
		CSettleDetailField::m_Describe.setSingleField(pStreamBuf, pSettleDetail);
		m_OutFogMetaRef.FieldNo = CSettleDetailField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CSettleDetailField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pSettlement == NULL && pSettleDetail == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvClearMarket(const MegaClearMarketStruct *pClearMarket, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvClearMarket;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pClearMarket != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CClearMarketField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CClearMarketField::m_Describe.m_FieldID;	
		}
		CClearMarketField::m_Describe.setSingleField(pStreamBuf, pClearMarket);
		m_OutFogMetaRef.FieldNo = CClearMarketField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CClearMarketField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pClearMarket == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvClearLastKLine(const MegaClearLastKLineStruct *pClearLastKLine, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvClearLastKLine;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pClearLastKLine != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CClearLastKLineField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CClearLastKLineField::m_Describe.m_FieldID;	
		}
		CClearLastKLineField::m_Describe.setSingleField(pStreamBuf, pClearLastKLine);
		m_OutFogMetaRef.FieldNo = CClearLastKLineField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CClearLastKLineField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pClearLastKLine == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvResetLocalID(const MegaResetLocalIDStruct *pResetLocalID, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvResetLocalID;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pResetLocalID != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CResetLocalIDField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CResetLocalIDField::m_Describe.m_FieldID;	
		}
		CResetLocalIDField::m_Describe.setSingleField(pStreamBuf, pResetLocalID);
		m_OutFogMetaRef.FieldNo = CResetLocalIDField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CResetLocalIDField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pResetLocalID == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvOTokenInsert(const MegaOTokenInsertStruct *pOTokenInsert, const MegaUserSessionStruct *pUserSession, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvOTokenInsert;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pOTokenInsert != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COTokenInsertField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COTokenInsertField::m_Describe.m_FieldID;	
		}
		COTokenInsertField::m_Describe.setSingleField(pStreamBuf, pOTokenInsert);
		m_OutFogMetaRef.FieldNo = COTokenInsertField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COTokenInsertField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pUserSession != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CUserSessionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;	
		}
		CUserSessionField::m_Describe.setSingleField(pStreamBuf, pUserSession);
		m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CUserSessionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOTokenInsert == NULL && pUserSession == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvOTokenAction(const MegaOTokenActionStruct *pOTokenAction, const MegaUserSessionStruct *pUserSession, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvOTokenAction;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pOTokenAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COTokenActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COTokenActionField::m_Describe.m_FieldID;	
		}
		COTokenActionField::m_Describe.setSingleField(pStreamBuf, pOTokenAction);
		m_OutFogMetaRef.FieldNo = COTokenActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COTokenActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pUserSession != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CUserSessionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;	
		}
		CUserSessionField::m_Describe.setSingleField(pStreamBuf, pUserSession);
		m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CUserSessionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOTokenAction == NULL && pUserSession == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvAPIKeyInsert(const MegaAPIKeyInsertStruct *pAPIKeyInsert, const MegaAPIKeyStruct *pAPIKey, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvAPIKeyInsert;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pAPIKeyInsert != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CAPIKeyInsertField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CAPIKeyInsertField::m_Describe.m_FieldID;	
		}
		CAPIKeyInsertField::m_Describe.setSingleField(pStreamBuf, pAPIKeyInsert);
		m_OutFogMetaRef.FieldNo = CAPIKeyInsertField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CAPIKeyInsertField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pAPIKey != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CAPIKeyField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CAPIKeyField::m_Describe.m_FieldID;	
		}
		CAPIKeyField::m_Describe.setSingleField(pStreamBuf, pAPIKey);
		m_OutFogMetaRef.FieldNo = CAPIKeyField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CAPIKeyField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pAPIKeyInsert == NULL && pAPIKey == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvAPIKeyAction(const MegaAPIKeyActionStruct *pAPIKeyAction, const MegaAPIKeyStruct *pAPIKey, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvAPIKeyAction;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pAPIKeyAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CAPIKeyActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CAPIKeyActionField::m_Describe.m_FieldID;	
		}
		CAPIKeyActionField::m_Describe.setSingleField(pStreamBuf, pAPIKeyAction);
		m_OutFogMetaRef.FieldNo = CAPIKeyActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CAPIKeyActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pAPIKey != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CAPIKeyField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CAPIKeyField::m_Describe.m_FieldID;	
		}
		CAPIKeyField::m_Describe.setSingleField(pStreamBuf, pAPIKey);
		m_OutFogMetaRef.FieldNo = CAPIKeyField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CAPIKeyField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pAPIKeyAction == NULL && pAPIKey == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvPositionMerge(const MegaPositionMergeStruct *pPositionMerge, const MegaPositionStruct *pPosition, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvPositionMerge;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pPositionMerge != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CPositionMergeField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CPositionMergeField::m_Describe.m_FieldID;	
		}
		CPositionMergeField::m_Describe.setSingleField(pStreamBuf, pPositionMerge);
		m_OutFogMetaRef.FieldNo = CPositionMergeField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CPositionMergeField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pPosition != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CPositionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;	
		}
		CPositionField::m_Describe.setSingleField(pStreamBuf, pPosition);
		m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CPositionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pPositionMerge == NULL && pPosition == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvDeposit(const MegaDepositStruct *pDeposit, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvDeposit;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pDeposit != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CDepositField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CDepositField::m_Describe.m_FieldID;	
		}
		CDepositField::m_Describe.setSingleField(pStreamBuf, pDeposit);
		m_OutFogMetaRef.FieldNo = CDepositField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CDepositField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pDeposit == NULL && pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvWithdraw(const MegaWithdrawStruct *pWithdraw, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvWithdraw;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pWithdraw != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CWithdrawField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CWithdrawField::m_Describe.m_FieldID;	
		}
		CWithdrawField::m_Describe.setSingleField(pStreamBuf, pWithdraw);
		m_OutFogMetaRef.FieldNo = CWithdrawField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CWithdrawField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pWithdraw == NULL && pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvTransfer(const MegaTransferStruct *pTransfer, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvTransfer;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pTransfer != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CTransferField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CTransferField::m_Describe.m_FieldID;	
		}
		CTransferField::m_Describe.setSingleField(pStreamBuf, pTransfer);
		m_OutFogMetaRef.FieldNo = CTransferField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CTransferField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTransfer == NULL && pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvUserRegister(const MegaUserSessionStruct *pUserSession, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvUserRegister;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pUserSession != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CUserSessionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;	
		}
		CUserSessionField::m_Describe.setSingleField(pStreamBuf, pUserSession);
		m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CUserSessionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pUserSession == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvPositionReverse(const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvPositionReverse;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvCancelOrderInsert(const MegaOrderInsertStruct *pOrderInsert, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvCancelOrderInsert;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pOrderInsert != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderInsertField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderInsertField::m_Describe.m_FieldID;	
		}
		COrderInsertField::m_Describe.setSingleField(pStreamBuf, pOrderInsert);
		m_OutFogMetaRef.FieldNo = COrderInsertField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderInsertField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrderInsert == NULL && pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvPrivateDispatch(const MegaPrivateDispatchStruct *pPrivateDispatch, const MegaMemberStruct *pMember, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvPrivateDispatch;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pPrivateDispatch != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CPrivateDispatchField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CPrivateDispatchField::m_Describe.m_FieldID;	
		}
		CPrivateDispatchField::m_Describe.setSingleField(pStreamBuf, pPrivateDispatch);
		m_OutFogMetaRef.FieldNo = CPrivateDispatchField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CPrivateDispatchField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pMember != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CMemberField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CMemberField::m_Describe.m_FieldID;	
		}
		CMemberField::m_Describe.setSingleField(pStreamBuf, pMember);
		m_OutFogMetaRef.FieldNo = CMemberField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CMemberField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pPrivateDispatch == NULL && pMember == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvPositionLeverage(const MegaPositionActionStruct *pPositionAction, const MegaPositionStruct *pPosition, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvPositionLeverage;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pPositionAction != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CPositionActionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CPositionActionField::m_Describe.m_FieldID;	
		}
		CPositionActionField::m_Describe.setSingleField(pStreamBuf, pPositionAction);
		m_OutFogMetaRef.FieldNo = CPositionActionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CPositionActionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pPosition != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CPositionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;	
		}
		CPositionField::m_Describe.setSingleField(pStreamBuf, pPosition);
		m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CPositionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pPositionAction == NULL && pPosition == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvInstrumentUpdate(const MegaInstrumentUpdateStruct *pInstrumentUpdate, const MegaInstrumentStruct *pInstrument, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvInstrumentUpdate;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pInstrumentUpdate != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CInstrumentUpdateField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CInstrumentUpdateField::m_Describe.m_FieldID;	
		}
		CInstrumentUpdateField::m_Describe.setSingleField(pStreamBuf, pInstrumentUpdate);
		m_OutFogMetaRef.FieldNo = CInstrumentUpdateField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CInstrumentUpdateField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pInstrument != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CInstrumentField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CInstrumentField::m_Describe.m_FieldID;	
		}
		CInstrumentField::m_Describe.setSingleField(pStreamBuf, pInstrument);
		m_OutFogMetaRef.FieldNo = CInstrumentField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CInstrumentField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pInstrumentUpdate == NULL && pInstrument == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvChangeCurrency(const MegaAccountStruct *pAccount, const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvChangeCurrency;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pAccount != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CAccountField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CAccountField::m_Describe.m_FieldID;	
		}
		CAccountField::m_Describe.setSingleField(pStreamBuf, pAccount);
		m_OutFogMetaRef.FieldNo = CAccountField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CAccountField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pAccount == NULL && pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvWalletNonce(const MegaWalletNonceStruct *pWalletNonce, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvWalletNonce;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pWalletNonce != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CWalletNonceField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CWalletNonceField::m_Describe.m_FieldID;	
		}
		CWalletNonceField::m_Describe.setSingleField(pStreamBuf, pWalletNonce);
		m_OutFogMetaRef.FieldNo = CWalletNonceField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CWalletNonceField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pWalletNonce == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvWalletLogin(const MegaUserSessionStruct *pUserSession, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvWalletLogin;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pUserSession != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CUserSessionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;	
		}
		CUserSessionField::m_Describe.setSingleField(pStreamBuf, pUserSession);
		m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CUserSessionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pUserSession == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryAccount(const MegaAccountStruct *pAccount, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryAccount;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pAccount != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CAccountField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CAccountField::m_Describe.m_FieldID;	
		}
		CAccountField::m_Describe.setSingleField(pStreamBuf, pAccount);
		m_OutFogMetaRef.FieldNo = CAccountField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CAccountField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pAccount == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryOrder(const MegaOrderStruct *pOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryOrder;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;	
		}
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryMemberPosition(const MegaMemberPositionStruct *pMemberPosition, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryMemberPosition;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pMemberPosition != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CMemberPositionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CMemberPositionField::m_Describe.m_FieldID;	
		}
		CMemberPositionField::m_Describe.setSingleField(pStreamBuf, pMemberPosition);
		m_OutFogMetaRef.FieldNo = CMemberPositionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CMemberPositionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pMemberPosition == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryPosition(const MegaPositionStruct *pPosition, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryPosition;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pPosition != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CPositionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;	
		}
		CPositionField::m_Describe.setSingleField(pStreamBuf, pPosition);
		m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CPositionField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pPosition == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryInstrument(const MegaInstrumentStruct *pInstrument, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryInstrument;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pInstrument != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CInstrumentField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CInstrumentField::m_Describe.m_FieldID;	
		}
		CInstrumentField::m_Describe.setSingleField(pStreamBuf, pInstrument);
		m_OutFogMetaRef.FieldNo = CInstrumentField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CInstrumentField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pInstrument == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryCurrency(const MegaCurrencyStruct *pCurrency, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryCurrency;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pCurrency != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CCurrencyField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CCurrencyField::m_Describe.m_FieldID;	
		}
		CCurrencyField::m_Describe.setSingleField(pStreamBuf, pCurrency);
		m_OutFogMetaRef.FieldNo = CCurrencyField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CCurrencyField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pCurrency == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryMember(const MegaMemberStruct *pMember, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryMember;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pMember != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CMemberField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CMemberField::m_Describe.m_FieldID;	
		}
		CMemberField::m_Describe.setSingleField(pStreamBuf, pMember);
		m_OutFogMetaRef.FieldNo = CMemberField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CMemberField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pMember == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryMarketOrder(const MegaMarketOrderStruct *pMarketOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryMarketOrder;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pMarketOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CMarketOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CMarketOrderField::m_Describe.m_FieldID;	
		}
		CMarketOrderField::m_Describe.setSingleField(pStreamBuf, pMarketOrder);
		m_OutFogMetaRef.FieldNo = CMarketOrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CMarketOrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pMarketOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryAPIKey(const MegaAPIKeyStruct *pAPIKey, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryAPIKey;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pAPIKey != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CAPIKeyField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CAPIKeyField::m_Describe.m_FieldID;	
		}
		CAPIKeyField::m_Describe.setSingleField(pStreamBuf, pAPIKey);
		m_OutFogMetaRef.FieldNo = CAPIKeyField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CAPIKeyField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pAPIKey == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryMarginRate(const MegaMarginRateStruct *pMarginRate, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryMarginRate;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pMarginRate != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CMarginRateField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CMarginRateField::m_Describe.m_FieldID;	
		}
		CMarginRateField::m_Describe.setSingleField(pStreamBuf, pMarginRate);
		m_OutFogMetaRef.FieldNo = CMarginRateField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CMarginRateField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pMarginRate == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryFee(const MegaFeeStruct *pFee, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryFee;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pFee != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CFeeField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CFeeField::m_Describe.m_FieldID;	
		}
		CFeeField::m_Describe.setSingleField(pStreamBuf, pFee);
		m_OutFogMetaRef.FieldNo = CFeeField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CFeeField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pFee == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryTriggerOrder(const MegaTriggerOrderStruct *pTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryTriggerOrder;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pTriggerOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CTriggerOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;	
		}
		CTriggerOrderField::m_Describe.setSingleField(pStreamBuf, pTriggerOrder);
		m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CTriggerOrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTriggerOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryLeverage(const MegaLeverageStruct *pLeverage, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryLeverage;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pLeverage != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CLeverageField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CLeverageField::m_Describe.m_FieldID;	
		}
		CLeverageField::m_Describe.setSingleField(pStreamBuf, pLeverage);
		m_OutFogMetaRef.FieldNo = CLeverageField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CLeverageField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pLeverage == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryMarketData(const MegaMarketDataStruct *pMarketData, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryMarketData;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pMarketData != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CMarketDataField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CMarketDataField::m_Describe.m_FieldID;	
		}
		CMarketDataField::m_Describe.setSingleField(pStreamBuf, pMarketData);
		m_OutFogMetaRef.FieldNo = CMarketDataField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CMarketDataField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pMarketData == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryTrade(const MegaTradeStruct *pTrade, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryTrade;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pTrade != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CTradeField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CTradeField::m_Describe.m_FieldID;	
		}
		CTradeField::m_Describe.setSingleField(pStreamBuf, pTrade);
		m_OutFogMetaRef.FieldNo = CTradeField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CTradeField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTrade == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryFinishOrder(const MegaFinishOrderStruct *pFinishOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryFinishOrder;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pFinishOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CFinishOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CFinishOrderField::m_Describe.m_FieldID;	
		}
		CFinishOrderField::m_Describe.setSingleField(pStreamBuf, pFinishOrder);
		m_OutFogMetaRef.FieldNo = CFinishOrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CFinishOrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pFinishOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryFinishTriggerOrder(const MegaFinishTriggerOrderStruct *pFinishTriggerOrder, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryFinishTriggerOrder;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pFinishTriggerOrder != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CFinishTriggerOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CFinishTriggerOrderField::m_Describe.m_FieldID;	
		}
		CFinishTriggerOrderField::m_Describe.setSingleField(pStreamBuf, pFinishTriggerOrder);
		m_OutFogMetaRef.FieldNo = CFinishTriggerOrderField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CFinishTriggerOrderField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pFinishTriggerOrder == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryWithLeverage(const MegaLeverageStruct *pLeverage, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryWithLeverage;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pLeverage != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CLeverageField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CLeverageField::m_Describe.m_FieldID;	
		}
		CLeverageField::m_Describe.setSingleField(pStreamBuf, pLeverage);
		m_OutFogMetaRef.FieldNo = CLeverageField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CLeverageField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pLeverage == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryKLine(const MegaKLineBaseStruct *pKLineBase, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryKLine;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pKLineBase != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CKLineBaseField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CKLineBaseField::m_Describe.m_FieldID;	
		}
		CKLineBaseField::m_Describe.setSingleField(pStreamBuf, pKLineBase);
		m_OutFogMetaRef.FieldNo = CKLineBaseField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CKLineBaseField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pKLineBase == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryLine(const MegaLineStruct *pLine, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryLine;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pLine != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CLineField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CLineField::m_Describe.m_FieldID;	
		}
		CLineField::m_Describe.setSingleField(pStreamBuf, pLine);
		m_OutFogMetaRef.FieldNo = CLineField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CLineField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pLine == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQryAccountDetail(const MegaAccountDetailStruct *pAccountDetail, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQryAccountDetail;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pAccountDetail != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CAccountDetailField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CAccountDetailField::m_Describe.m_FieldID;	
		}
		CAccountDetailField::m_Describe.setSingleField(pStreamBuf, pAccountDetail);
		m_OutFogMetaRef.FieldNo = CAccountDetailField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CAccountDetailField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pAccountDetail == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvQrySettleDetail(const MegaSettleDetailStruct *pSettleDetail, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvQrySettleDetail;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pSettleDetail != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CSettleDetailField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CSettleDetailField::m_Describe.m_FieldID;	
		}
		CSettleDetailField::m_Describe.setSingleField(pStreamBuf, pSettleDetail);
		m_OutFogMetaRef.FieldNo = CSettleDetailField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CSettleDetailField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pSettleDetail == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::RecvCheckToken(const MegaCheckTokenStruct *pCheckToken, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_RecvCheckToken;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
	if (pCheckToken != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -CCheckTokenField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = CCheckTokenField::m_Describe.m_FieldID;	
		}
		CCheckTokenField::m_Describe.setSingleField(pStreamBuf, pCheckToken);
		m_OutFogMetaRef.FieldNo = CCheckTokenField::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = CCheckTokenField::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pCheckToken == NULL)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}

void COFSwapLink::PushTimeSync(const MegaCurrentTimeStruct *pCurrentTime, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushTimeSync;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pCurrentTime != NULL)
	{
		CCurrentTimeField::m_Describe.setSingleField(pStreamBuf, pCurrentTime);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CCurrentTimeField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CCurrentTimeField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CCurrentTimeField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushCommand(const MegaCommandStruct *pCommand, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushCommand;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pCommand != NULL)
	{
		CCommandField::m_Describe.setSingleField(pStreamBuf, pCommand);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CCommandField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CCommandField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CCommandField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushTrade(const MegaTradeStruct *pTrade, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushTrade;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pTrade != NULL)
	{
		CTradeField::m_Describe.setSingleField(pStreamBuf, pTrade);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CTradeField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CTradeField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CTradeField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushOrder(const MegaOrderStruct *pOrder, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushOrder;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pOrder != NULL)
	{
		COrderField::m_Describe.setSingleField(pStreamBuf, pOrder);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -COrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = COrderField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = COrderField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushMemberPosition(const MegaMemberPositionStruct *pMemberPosition, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushMemberPosition;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pMemberPosition != NULL)
	{
		CMemberPositionField::m_Describe.setSingleField(pStreamBuf, pMemberPosition);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CMemberPositionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CMemberPositionField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CMemberPositionField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushPosition(const MegaPositionStruct *pPosition, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushPosition;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pPosition != NULL)
	{
		CPositionField::m_Describe.setSingleField(pStreamBuf, pPosition);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CPositionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CPositionField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushAccount(const MegaAccountStruct *pAccount, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushAccount;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pAccount != NULL)
	{
		CAccountField::m_Describe.setSingleField(pStreamBuf, pAccount);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CAccountField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CAccountField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CAccountField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushMarketTrade(const MegaMarketTradeStruct *pMarketTrade, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushMarketTrade;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pMarketTrade != NULL)
	{
		CMarketTradeField::m_Describe.setSingleField(pStreamBuf, pMarketTrade);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CMarketTradeField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CMarketTradeField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CMarketTradeField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushMarketOrder(const MegaMarketOrderStruct *pMarketOrder, const MegaTickMarketOrderStruct *pTickMarketOrder, const MegaCurrentTimeStruct *pCurrentTime, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushMarketOrder;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pMarketOrder != NULL)
	{
		CMarketOrderField::m_Describe.setSingleField(pStreamBuf, pMarketOrder);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CMarketOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CMarketOrderField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CMarketOrderField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pTickMarketOrder != NULL)
	{
		CTickMarketOrderField::m_Describe.setSingleField(pStreamBuf, pTickMarketOrder);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CTickMarketOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CTickMarketOrderField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CTickMarketOrderField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pCurrentTime != NULL)
	{
		CCurrentTimeField::m_Describe.setSingleField(pStreamBuf, pCurrentTime);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CCurrentTimeField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CCurrentTimeField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CCurrentTimeField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushKLine(const MegaKLineStruct *pKLine, const MegaLastKLineStruct *pLastKLine, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushKLine;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pKLine != NULL)
	{
		CKLineField::m_Describe.setSingleField(pStreamBuf, pKLine);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CKLineField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CKLineField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CKLineField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pLastKLine != NULL)
	{
		CLastKLineField::m_Describe.setSingleField(pStreamBuf, pLastKLine);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CLastKLineField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CLastKLineField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CLastKLineField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushMarketOverView(const MegaMarketOverViewStruct *pMarketOverView, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushMarketOverView;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pMarketOverView != NULL)
	{
		CMarketOverViewField::m_Describe.setSingleField(pStreamBuf, pMarketOverView);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CMarketOverViewField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CMarketOverViewField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CMarketOverViewField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushDelayMarketOrder(const MegaMarketOrderStruct *pMarketOrder, const MegaCurrentTimeStruct *pCurrentTime, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushDelayMarketOrder;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pMarketOrder != NULL)
	{
		CMarketOrderField::m_Describe.setSingleField(pStreamBuf, pMarketOrder);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CMarketOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CMarketOrderField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CMarketOrderField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
	if (pCurrentTime != NULL)
	{
		CCurrentTimeField::m_Describe.setSingleField(pStreamBuf, pCurrentTime);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CCurrentTimeField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CCurrentTimeField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CCurrentTimeField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushFiveDepthMarketData(const MegaFiveDepthMarketDataStruct *pFiveDepthMarketData, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushFiveDepthMarketData;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pFiveDepthMarketData != NULL)
	{
		CFiveDepthMarketDataField::m_Describe.setSingleField(pStreamBuf, pFiveDepthMarketData);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CFiveDepthMarketDataField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CFiveDepthMarketDataField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CFiveDepthMarketDataField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushTenDepthMarketData(const MegaTenDepthMarketDataStruct *pTenDepthMarketData, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushTenDepthMarketData;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pTenDepthMarketData != NULL)
	{
		CTenDepthMarketDataField::m_Describe.setSingleField(pStreamBuf, pTenDepthMarketData);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CTenDepthMarketDataField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CTenDepthMarketDataField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CTenDepthMarketDataField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushOptionsMarketData(const MegaOptionsMarketDataStruct *pOptionsMarketData, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushOptionsMarketData;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pOptionsMarketData != NULL)
	{
		COptionsMarketDataField::m_Describe.setSingleField(pStreamBuf, pOptionsMarketData);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -COptionsMarketDataField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = COptionsMarketDataField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = COptionsMarketDataField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushFee(const MegaFeeStruct *pFee, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushFee;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pFee != NULL)
	{
		CFeeField::m_Describe.setSingleField(pStreamBuf, pFee);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CFeeField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CFeeField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CFeeField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushMarginRate(const MegaMarginRateStruct *pMarginRate, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushMarginRate;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pMarginRate != NULL)
	{
		CMarginRateField::m_Describe.setSingleField(pStreamBuf, pMarginRate);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CMarginRateField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CMarginRateField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CMarginRateField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushMember(const MegaMemberStruct *pMember, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushMember;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pMember != NULL)
	{
		CMemberField::m_Describe.setSingleField(pStreamBuf, pMember);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CMemberField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CMemberField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CMemberField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushTriggerOrder(const MegaTriggerOrderStruct *pTriggerOrder, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushTriggerOrder;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pTriggerOrder != NULL)
	{
		CTriggerOrderField::m_Describe.setSingleField(pStreamBuf, pTriggerOrder);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CTriggerOrderField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CTriggerOrderField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CTriggerOrderField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushInstrument(const MegaInstrumentStruct *pInstrument, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushInstrument;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pInstrument != NULL)
	{
		CInstrumentField::m_Describe.setSingleField(pStreamBuf, pInstrument);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CInstrumentField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CInstrumentField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CInstrumentField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushUserSession(const MegaUserSessionStruct *pUserSession, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushUserSession;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pUserSession != NULL)
	{
		CUserSessionField::m_Describe.setSingleField(pStreamBuf, pUserSession);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CUserSessionField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CUserSessionField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CUserSessionField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

void COFSwapLink::PushCurrency(const MegaCurrencyStruct *pCurrency, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_PushCurrency;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
	if (pCurrency != NULL)
	{
		CCurrencyField::m_Describe.setSingleField(pStreamBuf, pCurrency);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -CCurrencyField::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = CCurrencyField::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = CCurrencyField::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
}

COneMegaApi* COFSwapLink::GetDll(const char* pName)
{
	COneMegaApi *pDll = NULL;
	char ServiceDllName[128];
#ifdef WINDOWS
	sprintf(ServiceDllName, "%s.dll", pName);
	HINSTANCE hDll = LoadLibrary(ServiceDllName);
	if (hDll != NULL)
	{
		_CreateOneMegaApi* MyCreateServiceApi = (_CreateOneMegaApi*)GetProcAddress(hDll, "CreateOneMegaApi");
		if (MyCreateServiceApi != NULL)
		{
			pDll = MyCreateServiceApi();
			if (pDll == NULL)
			{
				PRINT_TO_STD("error: %s->CreateOneMegaApi return NULL",ServiceDllName);
				return NULL;
			}
			else
			{
				PRINT_TO_STD("Load Library:%s", ServiceDllName);
			}
		}
		else
		{
			int errorid = GetLastError();
			PRINT_TO_STD("error: Can not GetProcAddress:%s->CreateOneMegaApi errorid:%d", ServiceDllName, errorid);
			return NULL;
		}
	}
	else
	{
		int errorid = GetLastError();
		PRINT_TO_STD("error: Can not LoadLibrary:%s errorid:%d", ServiceDllName, errorid);
		return NULL;
	}
#else
	sprintf(ServiceDllName, "lib%s.so", pName);
	void *handle = dlopen(ServiceDllName, RTLD_NOW);
	if (handle)
	{
		_CreateOneMegaApi* MyCreateServiceApi = (_CreateOneMegaApi*)dlsym(handle, "CreateOneMegaApi");
		if (MyCreateServiceApi != NULL)
		{
			pDll = MyCreateServiceApi();
			if (pDll == NULL)
			{
				PRINT_TO_STD("error: %s->CreateOneMegaApi return NULL",ServiceDllName);
				return NULL;
			}
			else
			{
				PRINT_TO_STD("Load Library:%s", ServiceDllName);
			}
		}
		else
		{
			PRINT_TO_STD("error: Can not dlsym:%s->CreateOneMegaApi msg:%s",ServiceDllName, dlerror());
			return NULL;
		}
	}
	else
	{
		PRINT_TO_STD("error: Can not dlopen:%s msg:%s", ServiceDllName, dlerror());
		return NULL;
	}
#endif
	return pDll;
}

extern "C" COutFogServiceApi* CreateOutFogServiceApi(COutFogServiceCallback *pCallback, int nServiceID)
{
	return new COFSwapLink(pCallback);
}
