#include "public.h"
#include "FieldMap.h"

const char *getPackageName(int tid)
{
	switch (tid)
	{
	case TID_SendCommand:
		return "SendCommand";
	case TID_RecvCommand:
		return "RecvCommand";
	case TID_SendInitLink:
		return "SendInitLink";
	case TID_RecvInitLink:
		return "RecvInitLink";
	case TID_SendUserLogin:
		return "SendUserLogin";
	case TID_RecvUserLogin:
		return "RecvUserLogin";
	case TID_SendUserLogout:
		return "SendUserLogout";
	case TID_RecvUserLogout:
		return "RecvUserLogout";
	case TID_SendOrderInsert:
		return "SendOrderInsert";
	case TID_RecvOrderInsert:
		return "RecvOrderInsert";
	case TID_SendOrderAction:
		return "SendOrderAction";
	case TID_RecvOrderAction:
		return "RecvOrderAction";
	case TID_SendTriggerOrderInsert:
		return "SendTriggerOrderInsert";
	case TID_RecvTriggerOrderInsert:
		return "RecvTriggerOrderInsert";
	case TID_SendTriggerOrderAction:
		return "SendTriggerOrderAction";
	case TID_RecvTriggerOrderAction:
		return "RecvTriggerOrderAction";
	case TID_SendCloseOrderInsert:
		return "SendCloseOrderInsert";
	case TID_RecvCloseOrderInsert:
		return "RecvCloseOrderInsert";
	case TID_SendCloseOrderAction:
		return "SendCloseOrderAction";
	case TID_RecvCloseOrderAction:
		return "RecvCloseOrderAction";
	case TID_SendQuoteInsert:
		return "SendQuoteInsert";
	case TID_RecvQuoteInsert:
		return "RecvQuoteInsert";
	case TID_SendQuoteAction:
		return "SendQuoteAction";
	case TID_RecvQuoteAction:
		return "RecvQuoteAction";
	case TID_SendTopicAction:
		return "SendTopicAction";
	case TID_RecvTopicAction:
		return "RecvTopicAction";
	case TID_SendMarketDataUpdate:
		return "SendMarketDataUpdate";
	case TID_RecvMarketDataUpdate:
		return "RecvMarketDataUpdate";
	case TID_SendPositionAction:
		return "SendPositionAction";
	case TID_RecvPositionAction:
		return "RecvPositionAction";
	case TID_SendAccountAction:
		return "SendAccountAction";
	case TID_RecvAccountAction:
		return "RecvAccountAction";
	case TID_SendMemberAction:
		return "SendMemberAction";
	case TID_RecvMemberAction:
		return "RecvMemberAction";
	case TID_SendRobotOrder:
		return "SendRobotOrder";
	case TID_RecvRobotOrder:
		return "RecvRobotOrder";
	case TID_SendOTCTradeInsert:
		return "SendOTCTradeInsert";
	case TID_RecvOTCTradeInsert:
		return "RecvOTCTradeInsert";
	case TID_SendSettlement:
		return "SendSettlement";
	case TID_RecvSettlement:
		return "RecvSettlement";
	case TID_SendClearMarket:
		return "SendClearMarket";
	case TID_RecvClearMarket:
		return "RecvClearMarket";
	case TID_SendClearLastKLine:
		return "SendClearLastKLine";
	case TID_RecvClearLastKLine:
		return "RecvClearLastKLine";
	case TID_SendResetLocalID:
		return "SendResetLocalID";
	case TID_RecvResetLocalID:
		return "RecvResetLocalID";
	case TID_SendOTokenInsert:
		return "SendOTokenInsert";
	case TID_RecvOTokenInsert:
		return "RecvOTokenInsert";
	case TID_SendOTokenAction:
		return "SendOTokenAction";
	case TID_RecvOTokenAction:
		return "RecvOTokenAction";
	case TID_SendAPIKeyInsert:
		return "SendAPIKeyInsert";
	case TID_RecvAPIKeyInsert:
		return "RecvAPIKeyInsert";
	case TID_SendAPIKeyAction:
		return "SendAPIKeyAction";
	case TID_RecvAPIKeyAction:
		return "RecvAPIKeyAction";
	case TID_SendPositionMerge:
		return "SendPositionMerge";
	case TID_RecvPositionMerge:
		return "RecvPositionMerge";
	case TID_SendDeposit:
		return "SendDeposit";
	case TID_RecvDeposit:
		return "RecvDeposit";
	case TID_SendWithdraw:
		return "SendWithdraw";
	case TID_RecvWithdraw:
		return "RecvWithdraw";
	case TID_SendTransfer:
		return "SendTransfer";
	case TID_RecvTransfer:
		return "RecvTransfer";
	case TID_SendUserRegister:
		return "SendUserRegister";
	case TID_RecvUserRegister:
		return "RecvUserRegister";
	case TID_SendPositionReverse:
		return "SendPositionReverse";
	case TID_RecvPositionReverse:
		return "RecvPositionReverse";
	case TID_SendCancelOrderInsert:
		return "SendCancelOrderInsert";
	case TID_RecvCancelOrderInsert:
		return "RecvCancelOrderInsert";
	case TID_SendPrivateDispatch:
		return "SendPrivateDispatch";
	case TID_RecvPrivateDispatch:
		return "RecvPrivateDispatch";
	case TID_SendPositionLeverage:
		return "SendPositionLeverage";
	case TID_RecvPositionLeverage:
		return "RecvPositionLeverage";
	case TID_SendInstrumentUpdate:
		return "SendInstrumentUpdate";
	case TID_RecvInstrumentUpdate:
		return "RecvInstrumentUpdate";
	case TID_SendChangeCurrency:
		return "SendChangeCurrency";
	case TID_RecvChangeCurrency:
		return "RecvChangeCurrency";
	case TID_SendWalletNonce:
		return "SendWalletNonce";
	case TID_RecvWalletNonce:
		return "RecvWalletNonce";
	case TID_SendWalletLogin:
		return "SendWalletLogin";
	case TID_RecvWalletLogin:
		return "RecvWalletLogin";
	case TID_SendQryAccount:
		return "SendQryAccount";
	case TID_RecvQryAccount:
		return "RecvQryAccount";
	case TID_SendQryOrder:
		return "SendQryOrder";
	case TID_RecvQryOrder:
		return "RecvQryOrder";
	case TID_SendQryMemberPosition:
		return "SendQryMemberPosition";
	case TID_RecvQryMemberPosition:
		return "RecvQryMemberPosition";
	case TID_SendQryPosition:
		return "SendQryPosition";
	case TID_RecvQryPosition:
		return "RecvQryPosition";
	case TID_SendQryInstrument:
		return "SendQryInstrument";
	case TID_RecvQryInstrument:
		return "RecvQryInstrument";
	case TID_SendQryCurrency:
		return "SendQryCurrency";
	case TID_RecvQryCurrency:
		return "RecvQryCurrency";
	case TID_SendQryMember:
		return "SendQryMember";
	case TID_RecvQryMember:
		return "RecvQryMember";
	case TID_SendQryMarketOrder:
		return "SendQryMarketOrder";
	case TID_RecvQryMarketOrder:
		return "RecvQryMarketOrder";
	case TID_SendQryAPIKey:
		return "SendQryAPIKey";
	case TID_RecvQryAPIKey:
		return "RecvQryAPIKey";
	case TID_SendQryMarginRate:
		return "SendQryMarginRate";
	case TID_RecvQryMarginRate:
		return "RecvQryMarginRate";
	case TID_SendQryFee:
		return "SendQryFee";
	case TID_RecvQryFee:
		return "RecvQryFee";
	case TID_SendQryTriggerOrder:
		return "SendQryTriggerOrder";
	case TID_RecvQryTriggerOrder:
		return "RecvQryTriggerOrder";
	case TID_SendQryLeverage:
		return "SendQryLeverage";
	case TID_RecvQryLeverage:
		return "RecvQryLeverage";
	case TID_SendQryMarketData:
		return "SendQryMarketData";
	case TID_RecvQryMarketData:
		return "RecvQryMarketData";
	case TID_SendQryTrade:
		return "SendQryTrade";
	case TID_RecvQryTrade:
		return "RecvQryTrade";
	case TID_SendQryFinishOrder:
		return "SendQryFinishOrder";
	case TID_RecvQryFinishOrder:
		return "RecvQryFinishOrder";
	case TID_SendQryFinishTriggerOrder:
		return "SendQryFinishTriggerOrder";
	case TID_RecvQryFinishTriggerOrder:
		return "RecvQryFinishTriggerOrder";
	case TID_SendQryWithLeverage:
		return "SendQryWithLeverage";
	case TID_RecvQryWithLeverage:
		return "RecvQryWithLeverage";
	case TID_SendQryKLine:
		return "SendQryKLine";
	case TID_RecvQryKLine:
		return "RecvQryKLine";
	case TID_SendQryLine:
		return "SendQryLine";
	case TID_RecvQryLine:
		return "RecvQryLine";
	case TID_SendQryAccountDetail:
		return "SendQryAccountDetail";
	case TID_RecvQryAccountDetail:
		return "RecvQryAccountDetail";
	case TID_SendQrySettleDetail:
		return "SendQrySettleDetail";
	case TID_RecvQrySettleDetail:
		return "RecvQrySettleDetail";
	case TID_SendCheckToken:
		return "SendCheckToken";
	case TID_RecvCheckToken:
		return "RecvCheckToken";
	case TID_SendBatch:
		return "SendBatch";
	case TID_PushTimeSync:
		return "PushTimeSync";
	case TID_PushCommand:
		return "PushCommand";
	case TID_PushTrade:
		return "PushTrade";
	case TID_PushOrder:
		return "PushOrder";
	case TID_PushMemberPosition:
		return "PushMemberPosition";
	case TID_PushPosition:
		return "PushPosition";
	case TID_PushAccount:
		return "PushAccount";
	case TID_PushMarketTrade:
		return "PushMarketTrade";
	case TID_PushMarketOrder:
		return "PushMarketOrder";
	case TID_PushKLine:
		return "PushKLine";
	case TID_PushMarketOverView:
		return "PushMarketOverView";
	case TID_PushDelayMarketOrder:
		return "PushDelayMarketOrder";
	case TID_PushFiveDepthMarketData:
		return "PushFiveDepthMarketData";
	case TID_PushTenDepthMarketData:
		return "PushTenDepthMarketData";
	case TID_PushOptionsMarketData:
		return "PushOptionsMarketData";
	case TID_PushFee:
		return "PushFee";
	case TID_PushMarginRate:
		return "PushMarginRate";
	case TID_PushMember:
		return "PushMember";
	case TID_PushTriggerOrder:
		return "PushTriggerOrder";
	case TID_PushInstrument:
		return "PushInstrument";
	case TID_PushUserSession:
		return "PushUserSession";
	case TID_PushCurrency:
		return "PushCurrency";
	case TID_DataSyncStart:
		return "DataSyncStart";
	case TID_DataSyncEnd:
		return "DataSyncEnd";
	case TID_SendInsert:
		return "SendInsert";
	case TID_RecvInsert:
		return "RecvInsert";
	case TID_SendUpdate:
		return "SendUpdate";
	case TID_RecvUpdate:
		return "RecvUpdate";
	case TID_SendDelete:
		return "SendDelete";
	case TID_RecvDelete:
		return "RecvDelete";
	case TID_SendInsUpd:
		return "SendInsUpd";
	case TID_RecvInsUpd:
		return "RecvInsUpd";
	case TID_SendQryOne:
		return "SendQryOne";
	case TID_RecvQryOne:
		return "RecvQryOne";
	case TID_SendClear:
		return "SendClear";
	case TID_RecvClear:
		return "RecvClear";
	case TID_SendInit:
		return "SendInit";
	case TID_RecvInit:
		return "RecvInit";
	}
	return "unknown";
}

const char *getShortPackageName(int tid)
{
	switch (tid)
	{
	case TID_SendCommand:
		return "SendCommand";
	case TID_RecvCommand:
		return "RC";
	case TID_SendInitLink:
		return "SendInitLink";
	case TID_RecvInitLink:
		return "RecvInitLink";
	case TID_SendUserLogin:
		return "SendUserLogin";
	case TID_RecvUserLogin:
		return "RecvUserLogin";
	case TID_SendUserLogout:
		return "SendUserLogout";
	case TID_RecvUserLogout:
		return "RecvUserLogout";
	case TID_SendOrderInsert:
		return "SendOrderInsert";
	case TID_RecvOrderInsert:
		return "ROI";
	case TID_SendOrderAction:
		return "SendOrderAction";
	case TID_RecvOrderAction:
		return "RecvOrderAction";
	case TID_SendTriggerOrderInsert:
		return "SendTriggerOrderInsert";
	case TID_RecvTriggerOrderInsert:
		return "RecvTriggerOrderInsert";
	case TID_SendTriggerOrderAction:
		return "SendTriggerOrderAction";
	case TID_RecvTriggerOrderAction:
		return "RecvTriggerOrderAction";
	case TID_SendCloseOrderInsert:
		return "SendCloseOrderInsert";
	case TID_RecvCloseOrderInsert:
		return "RecvCloseOrderInsert";
	case TID_SendCloseOrderAction:
		return "SendCloseOrderAction";
	case TID_RecvCloseOrderAction:
		return "RecvCloseOrderAction";
	case TID_SendQuoteInsert:
		return "SendQuoteInsert";
	case TID_RecvQuoteInsert:
		return "RecvQuoteInsert";
	case TID_SendQuoteAction:
		return "SendQuoteAction";
	case TID_RecvQuoteAction:
		return "RecvQuoteAction";
	case TID_SendTopicAction:
		return "SendTopicAction";
	case TID_RecvTopicAction:
		return "RecvTopicAction";
	case TID_SendMarketDataUpdate:
		return "SendMarketDataUpdate";
	case TID_RecvMarketDataUpdate:
		return "RecvMarketDataUpdate";
	case TID_SendPositionAction:
		return "SendPositionAction";
	case TID_RecvPositionAction:
		return "RecvPositionAction";
	case TID_SendAccountAction:
		return "SendAccountAction";
	case TID_RecvAccountAction:
		return "RecvAccountAction";
	case TID_SendMemberAction:
		return "SendMemberAction";
	case TID_RecvMemberAction:
		return "RecvMemberAction";
	case TID_SendRobotOrder:
		return "SendRobotOrder";
	case TID_RecvRobotOrder:
		return "RecvRobotOrder";
	case TID_SendOTCTradeInsert:
		return "SendOTCTradeInsert";
	case TID_RecvOTCTradeInsert:
		return "RecvOTCTradeInsert";
	case TID_SendSettlement:
		return "SendSettlement";
	case TID_RecvSettlement:
		return "RecvSettlement";
	case TID_SendClearMarket:
		return "SendClearMarket";
	case TID_RecvClearMarket:
		return "RecvClearMarket";
	case TID_SendClearLastKLine:
		return "SendClearLastKLine";
	case TID_RecvClearLastKLine:
		return "RecvClearLastKLine";
	case TID_SendResetLocalID:
		return "SendResetLocalID";
	case TID_RecvResetLocalID:
		return "RecvResetLocalID";
	case TID_SendOTokenInsert:
		return "SendOTokenInsert";
	case TID_RecvOTokenInsert:
		return "RecvOTokenInsert";
	case TID_SendOTokenAction:
		return "SendOTokenAction";
	case TID_RecvOTokenAction:
		return "RecvOTokenAction";
	case TID_SendAPIKeyInsert:
		return "SendAPIKeyInsert";
	case TID_RecvAPIKeyInsert:
		return "RecvAPIKeyInsert";
	case TID_SendAPIKeyAction:
		return "SendAPIKeyAction";
	case TID_RecvAPIKeyAction:
		return "RecvAPIKeyAction";
	case TID_SendPositionMerge:
		return "SendPositionMerge";
	case TID_RecvPositionMerge:
		return "RecvPositionMerge";
	case TID_SendDeposit:
		return "SendDeposit";
	case TID_RecvDeposit:
		return "RecvDeposit";
	case TID_SendWithdraw:
		return "SendWithdraw";
	case TID_RecvWithdraw:
		return "RecvWithdraw";
	case TID_SendTransfer:
		return "SendTransfer";
	case TID_RecvTransfer:
		return "RecvTransfer";
	case TID_SendUserRegister:
		return "SendUserRegister";
	case TID_RecvUserRegister:
		return "RecvUserRegister";
	case TID_SendPositionReverse:
		return "SendPositionReverse";
	case TID_RecvPositionReverse:
		return "RPR";
	case TID_SendCancelOrderInsert:
		return "SendCancelOrderInsert";
	case TID_RecvCancelOrderInsert:
		return "RecvCancelOrderInsert";
	case TID_SendPrivateDispatch:
		return "SendPrivateDispatch";
	case TID_RecvPrivateDispatch:
		return "RecvPrivateDispatch";
	case TID_SendPositionLeverage:
		return "SendPositionLeverage";
	case TID_RecvPositionLeverage:
		return "RecvPositionLeverage";
	case TID_SendInstrumentUpdate:
		return "SendInstrumentUpdate";
	case TID_RecvInstrumentUpdate:
		return "RecvInstrumentUpdate";
	case TID_SendChangeCurrency:
		return "SendChangeCurrency";
	case TID_RecvChangeCurrency:
		return "RecvChangeCurrency";
	case TID_SendWalletNonce:
		return "SendWalletNonce";
	case TID_RecvWalletNonce:
		return "RecvWalletNonce";
	case TID_SendWalletLogin:
		return "SendWalletLogin";
	case TID_RecvWalletLogin:
		return "RecvWalletLogin";
	case TID_SendQryAccount:
		return "SendQryAccount";
	case TID_RecvQryAccount:
		return "RecvQryAccount";
	case TID_SendQryOrder:
		return "SendQryOrder";
	case TID_RecvQryOrder:
		return "RecvQryOrder";
	case TID_SendQryMemberPosition:
		return "SendQryMemberPosition";
	case TID_RecvQryMemberPosition:
		return "RecvQryMemberPosition";
	case TID_SendQryPosition:
		return "SendQryPosition";
	case TID_RecvQryPosition:
		return "RecvQryPosition";
	case TID_SendQryInstrument:
		return "SendQryInstrument";
	case TID_RecvQryInstrument:
		return "RecvQryInstrument";
	case TID_SendQryCurrency:
		return "SendQryCurrency";
	case TID_RecvQryCurrency:
		return "RecvQryCurrency";
	case TID_SendQryMember:
		return "SendQryMember";
	case TID_RecvQryMember:
		return "RecvQryMember";
	case TID_SendQryMarketOrder:
		return "SendQryMarketOrder";
	case TID_RecvQryMarketOrder:
		return "RecvQryMarketOrder";
	case TID_SendQryAPIKey:
		return "SendQryAPIKey";
	case TID_RecvQryAPIKey:
		return "RecvQryAPIKey";
	case TID_SendQryMarginRate:
		return "SendQryMarginRate";
	case TID_RecvQryMarginRate:
		return "RecvQryMarginRate";
	case TID_SendQryFee:
		return "SendQryFee";
	case TID_RecvQryFee:
		return "RecvQryFee";
	case TID_SendQryTriggerOrder:
		return "SendQryTriggerOrder";
	case TID_RecvQryTriggerOrder:
		return "RecvQryTriggerOrder";
	case TID_SendQryLeverage:
		return "SendQryLeverage";
	case TID_RecvQryLeverage:
		return "RecvQryLeverage";
	case TID_SendQryMarketData:
		return "SendQryMarketData";
	case TID_RecvQryMarketData:
		return "RecvQryMarketData";
	case TID_SendQryTrade:
		return "SendQryTrade";
	case TID_RecvQryTrade:
		return "RecvQryTrade";
	case TID_SendQryFinishOrder:
		return "SendQryFinishOrder";
	case TID_RecvQryFinishOrder:
		return "RecvQryFinishOrder";
	case TID_SendQryFinishTriggerOrder:
		return "SendQryFinishTriggerOrder";
	case TID_RecvQryFinishTriggerOrder:
		return "RecvQryFinishTriggerOrder";
	case TID_SendQryWithLeverage:
		return "SendQryWithLeverage";
	case TID_RecvQryWithLeverage:
		return "RecvQryWithLeverage";
	case TID_SendQryKLine:
		return "SendQryKLine";
	case TID_RecvQryKLine:
		return "RecvQryKLine";
	case TID_SendQryLine:
		return "SendQryLine";
	case TID_RecvQryLine:
		return "RecvQryLine";
	case TID_SendQryAccountDetail:
		return "SendQryAccountDetail";
	case TID_RecvQryAccountDetail:
		return "RecvQryAccountDetail";
	case TID_SendQrySettleDetail:
		return "SendQrySettleDetail";
	case TID_RecvQrySettleDetail:
		return "RecvQrySettleDetail";
	case TID_SendCheckToken:
		return "SendCheckToken";
	case TID_RecvCheckToken:
		return "RecvCheckToken";
	case TID_SendBatch:
		return "SendBatch";
	case TID_PushTimeSync:
		return "PS";
	case TID_PushCommand:
		return "PC";
	case TID_PushTrade:
		return "PT";
	case TID_PushOrder:
		return "PO";
	case TID_PushMemberPosition:
		return "PMP";
	case TID_PushPosition:
		return "PP";
	case TID_PushAccount:
		return "PA";
	case TID_PushMarketTrade:
		return "PMT";
	case TID_PushMarketOrder:
		return "PMO";
	case TID_PushKLine:
		return "PK";
	case TID_PushMarketOverView:
		return "PO";
	case TID_PushDelayMarketOrder:
		return "PMO";
	case TID_PushFiveDepthMarketData:
		return "PM5";
	case TID_PushTenDepthMarketData:
		return "PMT";
	case TID_PushOptionsMarketData:
		return "POM";
	case TID_PushFee:
		return "PF";
	case TID_PushMarginRate:
		return "PMR";
	case TID_PushMember:
		return "PM";
	case TID_PushTriggerOrder:
		return "PTO";
	case TID_PushInstrument:
		return "PI";
	case TID_PushUserSession:
		return "PI";
	case TID_PushCurrency:
		return "PI";
	case TID_DataSyncStart:
		return "DataSyncStart";
	case TID_DataSyncEnd:
		return "DataSyncEnd";
	case TID_SendInsert:
		return "SendInsert";
	case TID_RecvInsert:
		return "RecvInsert";
	case TID_SendUpdate:
		return "SendUpdate";
	case TID_RecvUpdate:
		return "RecvUpdate";
	case TID_SendDelete:
		return "SendDelete";
	case TID_RecvDelete:
		return "RecvDelete";
	case TID_SendInsUpd:
		return "SendInsUpd";
	case TID_RecvInsUpd:
		return "RecvInsUpd";
	case TID_SendQryOne:
		return "SendQryOne";
	case TID_RecvQryOne:
		return "RecvQryOne";
	case TID_SendClear:
		return "SendClear";
	case TID_RecvClear:
		return "RecvClear";
	case TID_SendInit:
		return "SendInit";
	case TID_RecvInit:
		return "RecvInit";
	}
	return "unknown";
}

TFieldUse FieldUses[]=
{
	{FID_TopicAction, &CTopicActionField::m_Describe, "TopicAction", "TopicAction", "TopicAction", "主题订阅操作"}
	,{FID_CommandHead, &CCommandHeadField::m_Describe, "CommandHead", "CommandHead", "CommandHead", "操作信息"}
	,{FID_Error, &CErrorField::m_Describe, "Error", "Error", "Error", "响应信息"}
	,{FID_CheckToken, &CCheckTokenField::m_Describe, "CheckToken", "CheckToken", "CheckToken", "响应信息"}
	,{FID_Command, &CCommandField::m_Describe, "Command", "Command", "Command", "消息"}
	,{FID_UserLogin, &CUserLoginField::m_Describe, "UserLogin", "UserLogin", "UserLogin", "用户登录"}
	,{FID_Link, &CLinkField::m_Describe, "Link", "Link", "Link", "向上通道"}
	,{FID_UserLogout, &CUserLogoutField::m_Describe, "UserLogout", "UO", "UserLogout", "用户登录退出"}
	,{FID_UserPasswordUpdate, &CUserPasswordUpdateField::m_Describe, "UserPasswordUpdate", "UserPasswordUpdate", "UserPasswordUpdate", "用户口令修改"}
	,{FID_TriggerOrderInsert, &CTriggerOrderInsertField::m_Describe, "TriggerOrderInsert", "TOI", "TriggerOrderInsert", "输入报单"}
	,{FID_CloseOrderInsert, &CCloseOrderInsertField::m_Describe, "CloseOrderInsert", "COI", "CloseOrderInsert", "输入止盈止损报单"}
	,{FID_TriggerOrderAction, &CTriggerOrderActionField::m_Describe, "TriggerOrderAction", "TOA", "TriggerOrderAction", "触发单操作"}
	,{FID_OrderInsert, &COrderInsertField::m_Describe, "OrderInsert", "OI", "OrderInsert", "输入报单"}
	,{FID_OrderAction, &COrderActionField::m_Describe, "OrderAction", "OA", "OrderAction", "报单操作"}
	,{FID_QuoteInsert, &CQuoteInsertField::m_Describe, "QuoteInsert", "QI", "QuoteInsert", "输入报价"}
	,{FID_QuoteAction, &CQuoteActionField::m_Describe, "QuoteAction", "QA", "QuoteAction", "报价操作"}
	,{FID_OTCTradeInsert, &COTCTradeInsertField::m_Describe, "OTCTradeInsert", "OTCTradeInsert", "OTCTradeInsert", "输入场外成交对"}
	,{FID_OTCTrade, &COTCTradeField::m_Describe, "OTCTrade", "OTCTrade", "OTCTrade", "输入场外成交对"}
	,{FID_RobotOrder, &CRobotOrderField::m_Describe, "RobotOrder", "RobotOrder", "RobotOrder", "内部做市商报单"}
	,{FID_PositionAction, &CPositionActionField::m_Describe, "PositionAction", "PositionAction", "PositionAction", "仓位操作"}
	,{FID_AccountAction, &CAccountActionField::m_Describe, "AccountAction", "AccountAction", "AccountAction", "资金账户出入金"}
	,{FID_MemberAction, &CMemberActionField::m_Describe, "MemberAction", "MemberAction", "MemberAction", "资金账户出入金"}
	,{FID_MarketOverView, &CMarketOverViewField::m_Describe, "MarketOverView", "M", "MarketOverView", "行情概要"}
	,{FID_FiveDepthMarketData, &CFiveDepthMarketDataField::m_Describe, "FiveDepthMarketData", "M5", "FiveDepthMarketData", "5档深度行情"}
	,{FID_TenDepthMarketData, &CTenDepthMarketDataField::m_Describe, "TenDepthMarketData", "MT", "TenDepthMarketData", "10档深度行情"}
	,{FID_OptionsMarketData, &COptionsMarketDataField::m_Describe, "OptionsMarketData", "OptionsMarketData", "OptionsMarketData", "期权行情"}
	,{FID_InstrumentUpdate, &CInstrumentUpdateField::m_Describe, "InstrumentUpdate", "InstrumentUpdate", "InstrumentUpdate", "更新合约"}
	,{FID_MarketDataUpdate, &CMarketDataUpdateField::m_Describe, "MarketDataUpdate", "MarketDataUpdate", "MarketDataUpdate", "更新行情"}
	,{FID_MarketTrade, &CMarketTradeField::m_Describe, "MarketTrade", "T", "MarketTrade", "成交明细"}
	,{FID_Settlement, &CSettlementField::m_Describe, "Settlement", "Settlement", "Settlement", "资金结算"}
	,{FID_APIKeyInsert, &CAPIKeyInsertField::m_Describe, "APIKeyInsert", "APIKeyInsert", "APIKeyInsert", "输入APIKey"}
	,{FID_APIKey, &CAPIKeyField::m_Describe, "APIKey", "APIKey", "APIKey", "APIKey"}
	,{FID_APIKeyAction, &CAPIKeyActionField::m_Describe, "APIKeyAction", "APIKeyAction", "APIKeyAction", "操作APIKey"}
	,{FID_ClearMarket, &CClearMarketField::m_Describe, "ClearMarket", "ClearMarket", "ClearMarket", "清理行情数据"}
	,{FID_ClearLastKLine, &CClearLastKLineField::m_Describe, "ClearLastKLine", "ClearLastKLine", "ClearLastKLine", "清理最新K线数据"}
	,{FID_OTokenInsert, &COTokenInsertField::m_Describe, "OTokenInsert", "OTokenInsert", "OTokenInsert", "输入OToken"}
	,{FID_OTokenAction, &COTokenActionField::m_Describe, "OTokenAction", "OTokenAction", "OTokenAction", "操作OToken"}
	,{FID_ResetLocalID, &CResetLocalIDField::m_Describe, "ResetLocalID", "ResetLocalID", "ResetLocalID", "重置LocalID"}
	,{FID_PositionMerge, &CPositionMergeField::m_Describe, "PositionMerge", "PositionMerge", "PositionMerge", "合并持仓"}
	,{FID_Deposit, &CDepositField::m_Describe, "Deposit", "Deposit", "Deposit", "用户入金"}
	,{FID_Withdraw, &CWithdrawField::m_Describe, "Withdraw", "Withdraw", "Withdraw", "用户出金"}
	,{FID_Transfer, &CTransferField::m_Describe, "Transfer", "Transfer", "Transfer", "用户转账"}
	,{FID_CloseOrderAction, &CCloseOrderActionField::m_Describe, "CloseOrderAction", "CloseOrderAction", "CloseOrderAction", "报单止盈止损操作"}
	,{FID_Leverage, &CLeverageField::m_Describe, "Leverage", "Leverage", "Leverage", "杠杆倍数"}
	,{FID_PositionReverse, &CPositionReverseField::m_Describe, "PositionReverse", "PR", "PositionReverse", "反向开仓"}
	,{FID_PrivateDispatch, &CPrivateDispatchField::m_Describe, "PrivateDispatch", "PrivateDispatch", "PrivateDispatch", "私有主题分类"}
	,{FID_TickMarketOrder, &CTickMarketOrderField::m_Describe, "TickMarketOrder", "O", "TickMarketOrder", "TICK订单簿变化"}
	,{FID_KLineBase, &CKLineBaseField::m_Describe, "KLineBase", "K", "KLineBase", "基础K线"}
	,{FID_Line, &CLineField::m_Describe, "Line", "L", "Line", "历史行情线"}
	,{FID_ChangeCurrency, &CChangeCurrencyField::m_Describe, "ChangeCurrency", "CC", "ChangeCurrency", "充币兑换"}
	,{FID_WalletNonce, &CWalletNonceField::m_Describe, "WalletNonce", "WalletNonce", "WalletNonce", "钱包登录随机数"}
	,{FID_WalletLogin, &CWalletLoginField::m_Describe, "WalletLogin", "WalletLogin", "WalletLogin", "钱包登录信息"}
	,{FID_QryOrder, &CQryOrderField::m_Describe, "QryOrder", "QryOrder", "QryOrder", "报单查询"}
	,{FID_QryMemberPosition, &CQryMemberPositionField::m_Describe, "QryMemberPosition", "QryMemberPosition", "QryMemberPosition", "持仓汇总查询"}
	,{FID_QryPosition, &CQryPositionField::m_Describe, "QryPosition", "QryPosition", "QryPosition", "交易单元持仓查询"}
	,{FID_QryAccount, &CQryAccountField::m_Describe, "QryAccount", "QryAccount", "QryAccount", "交易资金查询"}
	,{FID_QryInstrument, &CQryInstrumentField::m_Describe, "QryInstrument", "QryInstrument", "QryInstrument", "标的查询"}
	,{FID_QryCurrency, &CQryCurrencyField::m_Describe, "QryCurrency", "QryCurrency", "QryCurrency", "标的查询"}
	,{FID_QryMember, &CQryMemberField::m_Describe, "QryMember", "QryMember", "QryMember", "成员查询"}
	,{FID_QryMarketOrder, &CQryMarketOrderField::m_Describe, "QryMarketOrder", "QryMarketOrder", "QryMarketOrder", "标的价位查询"}
	,{FID_QryAPIKey, &CQryAPIKeyField::m_Describe, "QryAPIKey", "QryAPIKey", "QryAPIKey", "查询APIKEY"}
	,{FID_QryMarginRate, &CQryMarginRateField::m_Describe, "QryMarginRate", "QryMarginRate", "QryMarginRate", "查询保证金率"}
	,{FID_QryFee, &CQryFeeField::m_Describe, "QryFee", "QryFee", "QryFee", "查询手续费率"}
	,{FID_QryTriggerOrder, &CQryTriggerOrderField::m_Describe, "QryTriggerOrder", "QryTriggerOrder", "QryTriggerOrder", "查询触发订单"}
	,{FID_QryLeverage, &CQryLeverageField::m_Describe, "QryLeverage", "QryLeverage", "QryLeverage", "查询杠杆倍数"}
	,{FID_QryMarketData, &CQryMarketDataField::m_Describe, "QryMarketData", "QryMarketData", "QryMarketData", "查询行情"}
	,{FID_QryTrade, &CQryTradeField::m_Describe, "QryTrade", "QryTrade", "QryTrade", "查询成交"}
	,{FID_QryFinishOrder, &CQryFinishOrderField::m_Describe, "QryFinishOrder", "QryFinishOrder", "QryFinishOrder", "查询历史委托"}
	,{FID_QryFinishTriggerOrder, &CQryFinishTriggerOrderField::m_Describe, "QryFinishTriggerOrder", "QryFinishTriggerOrder", "QryFinishTriggerOrder", "查询历史条件单"}
	,{FID_QryWithLeverage, &CQryWithLeverageField::m_Describe, "QryWithLeverage", "QryWithLeverage", "QryWithLeverage", "指定杠杠下梯度信息"}
	,{FID_QryKLine, &CQryKLineField::m_Describe, "QryKLine", "QryKLine", "QryKLine", "查询K线"}
	,{FID_QryLine, &CQryLineField::m_Describe, "QryLine", "QryLine", "QryLine", "查询线"}
	,{FID_QryAccountDetail, &CQryAccountDetailField::m_Describe, "QryAccountDetail", "QryAccountDetail", "QryAccountDetail", "查询资金流水"}
	,{FID_QrySettleDetail, &CQrySettleDetailField::m_Describe, "QrySettleDetail", "QrySettleDetail", "QrySettleDetail", "查询结算流水"}
	,{FID_ServiceConfig, &CServiceConfigField::m_Describe, "ServiceConfig", "ServiceConfig", "ServiceConfig", "业务配置"}
	,{FID_Currency, &CCurrencyField::m_Describe, "Currency", "Currency", "Currency", "币种信息"}
	,{FID_MarketData, &CMarketDataField::m_Describe, "MarketData", "MarketData", "MarketData", "市场行情"}
	,{FID_Instrument, &CInstrumentField::m_Describe, "Instrument", "Instrument", "Instrument", "交易标的"}
	,{FID_CurrentTime, &CCurrentTimeField::m_Describe, "CurrentTime", "T", "CurrentTime", "当前时间"}
	,{FID_Member, &CMemberField::m_Describe, "Member", "Member", "Member", "成员"}
	,{FID_UserSession, &CUserSessionField::m_Describe, "UserSession", "UserSession", "UserSession", "用户会话"}
	,{FID_LastKLine, &CLastKLineField::m_Describe, "LastKLine", "LK", "LastKLine", "最新K线"}
	,{FID_CFDRate, &CCFDRateField::m_Describe, "CFDRate", "CFDRate", "CFDRate", "CFD溢价率"}
	,{FID_TradingRight, &CTradingRightField::m_Describe, "TradingRight", "TradingRight", "TradingRight", "交易权限"}
	,{FID_MarginRate, &CMarginRateField::m_Describe, "MarginRate", "MarginRate", "MarginRate", "保证金率"}
	,{FID_Fee, &CFeeField::m_Describe, "Fee", "Fee", "Fee", "手续费率"}
	,{FID_Liquidity, &CLiquidityField::m_Describe, "Liquidity", "Liquidity", "Liquidity", "流动性设置表"}
	,{FID_Account, &CAccountField::m_Describe, "Account", "Account", "Account", "账户资金"}
	,{FID_MemberPosition, &CMemberPositionField::m_Describe, "MemberPosition", "MemberPosition", "MemberPosition", "成员持仓汇总"}
	,{FID_Position, &CPositionField::m_Describe, "Position", "Position", "Position", "标的持仓"}
	,{FID_Order, &COrderField::m_Describe, "Order", "Order", "Order", "报单"}
	,{FID_TriggerOrder, &CTriggerOrderField::m_Describe, "TriggerOrder", "TriggerOrder", "TriggerOrder", "条件报单"}
	,{FID_PositionOrder, &CPositionOrderField::m_Describe, "PositionOrder", "PositionOrder", "PositionOrder", "持仓订单组成"}
	,{FID_FinishOrder, &CFinishOrderField::m_Describe, "FinishOrder", "FinishOrder", "FinishOrder", "完成的报单"}
	,{FID_Trade, &CTradeField::m_Describe, "Trade", "Trade", "Trade", "成交"}
	,{FID_FinishPosition, &CFinishPositionField::m_Describe, "FinishPosition", "FinishPosition", "FinishPosition", "持仓历史"}
	,{FID_AccountDetail, &CAccountDetailField::m_Describe, "AccountDetail", "AccountDetail", "AccountDetail", "账户流水"}
	,{FID_KLine, &CKLineField::m_Describe, "KLine", "K", "KLine", "历史K线"}
	,{FID_SettleDetail, &CSettleDetailField::m_Describe, "SettleDetail", "SettleDetail", "SettleDetail", "操作流水"}
	,{FID_FinishTriggerOrder, &CFinishTriggerOrderField::m_Describe, "FinishTriggerOrder", "FinishTriggerOrder", "FinishTriggerOrder", "完成的条件报单"}
	,{FID_MarketOrder, &CMarketOrderField::m_Describe, "MarketOrder", "O", "MarketOrder", "订单簿变化"}
	,{FID_PublishKLine, &CPublishKLineField::m_Describe, "PublishKLine", "PK", "PublishKLine", "发布K线"}
};

TPackageDefine XTPPackageDefines[]=
{
	{TID_SendCommand, "SendCommand", "一般消息请求" }
	,{TID_RecvCommand, "RecvCommand", "一般消息应答" }
	,{TID_SendInitLink, "SendInitLink", "创建上层连接请求" }
	,{TID_RecvInitLink, "RecvInitLink", "创建上层连接应答" }
	,{TID_SendUserLogin, "SendUserLogin", "用户登录请求" }
	,{TID_RecvUserLogin, "RecvUserLogin", "用户登录应答" }
	,{TID_SendUserLogout, "SendUserLogout", "用户退出请求" }
	,{TID_RecvUserLogout, "RecvUserLogout", "用户退出应答" }
	,{TID_SendOrderInsert, "SendOrderInsert", "报单录入请求" }
	,{TID_RecvOrderInsert, "RecvOrderInsert", "报单录入应答" }
	,{TID_SendOrderAction, "SendOrderAction", "报单操作请求" }
	,{TID_RecvOrderAction, "RecvOrderAction", "报单操作应答" }
	,{TID_SendTriggerOrderInsert, "SendTriggerOrderInsert", "触发单录入请求" }
	,{TID_RecvTriggerOrderInsert, "RecvTriggerOrderInsert", "触发单录入应答" }
	,{TID_SendTriggerOrderAction, "SendTriggerOrderAction", "触发单操作请求" }
	,{TID_RecvTriggerOrderAction, "RecvTriggerOrderAction", "触发单操作应答" }
	,{TID_SendCloseOrderInsert, "SendCloseOrderInsert", "止盈止损报单录入请求" }
	,{TID_RecvCloseOrderInsert, "RecvCloseOrderInsert", "止盈止损报单录入应答" }
	,{TID_SendCloseOrderAction, "SendCloseOrderAction", "止盈止损报单操作请求" }
	,{TID_RecvCloseOrderAction, "RecvCloseOrderAction", "止盈止损报单操作应答" }
	,{TID_SendQuoteInsert, "SendQuoteInsert", "报价录入请求" }
	,{TID_RecvQuoteInsert, "RecvQuoteInsert", "报价录入应答" }
	,{TID_SendQuoteAction, "SendQuoteAction", "报价操作请求" }
	,{TID_RecvQuoteAction, "RecvQuoteAction", "报价操作应答" }
	,{TID_SendTopicAction, "SendTopicAction", "订阅主题请求" }
	,{TID_RecvTopicAction, "RecvTopicAction", "订阅主题应答" }
	,{TID_SendMarketDataUpdate, "SendMarketDataUpdate", "设置行情请求" }
	,{TID_RecvMarketDataUpdate, "RecvMarketDataUpdate", "设置行情应答" }
	,{TID_SendPositionAction, "SendPositionAction", "调整持仓请求" }
	,{TID_RecvPositionAction, "RecvPositionAction", "调整持仓应答" }
	,{TID_SendAccountAction, "SendAccountAction", "用户出入金请求" }
	,{TID_RecvAccountAction, "RecvAccountAction", "用户出入金应答" }
	,{TID_SendMemberAction, "SendMemberAction", "成员设置请求" }
	,{TID_RecvMemberAction, "RecvMemberAction", "成员设置应答" }
	,{TID_SendRobotOrder, "SendRobotOrder", "内部做市商申报请求" }
	,{TID_RecvRobotOrder, "RecvRobotOrder", "内部做市商申报应答" }
	,{TID_SendOTCTradeInsert, "SendOTCTradeInsert", "场外成交申报请求" }
	,{TID_RecvOTCTradeInsert, "RecvOTCTradeInsert", "场外成交申报应答" }
	,{TID_SendSettlement, "SendSettlement", "资金结算请求" }
	,{TID_RecvSettlement, "RecvSettlement", "资金结算应答" }
	,{TID_SendClearMarket, "SendClearMarket", "清理行情数据请求" }
	,{TID_RecvClearMarket, "RecvClearMarket", "清理行情数据应答" }
	,{TID_SendClearLastKLine, "SendClearLastKLine", "清理最新K线请求" }
	,{TID_RecvClearLastKLine, "RecvClearLastKLine", "清理最新K线应答" }
	,{TID_SendResetLocalID, "SendResetLocalID", "重置LocalID请求" }
	,{TID_RecvResetLocalID, "RecvResetLocalID", "重置LocalID应答" }
	,{TID_SendOTokenInsert, "SendOTokenInsert", "OToken录入请求" }
	,{TID_RecvOTokenInsert, "RecvOTokenInsert", "OToken录入应答" }
	,{TID_SendOTokenAction, "SendOTokenAction", "OToken操作请求" }
	,{TID_RecvOTokenAction, "RecvOTokenAction", "OToken操作应答" }
	,{TID_SendAPIKeyInsert, "SendAPIKeyInsert", "APIKEY录入请求" }
	,{TID_RecvAPIKeyInsert, "RecvAPIKeyInsert", "APIKEY录入应答" }
	,{TID_SendAPIKeyAction, "SendAPIKeyAction", "APIKEY操作请求" }
	,{TID_RecvAPIKeyAction, "RecvAPIKeyAction", "APIKEY操作应答" }
	,{TID_SendPositionMerge, "SendPositionMerge", "仓位合并操作请求" }
	,{TID_RecvPositionMerge, "RecvPositionMerge", "仓位合并操作应答" }
	,{TID_SendDeposit, "SendDeposit", "用户入金操作请求" }
	,{TID_RecvDeposit, "RecvDeposit", "用户入金操作应答" }
	,{TID_SendWithdraw, "SendWithdraw", "用户出金操作请求" }
	,{TID_RecvWithdraw, "RecvWithdraw", "用户出金操作应答" }
	,{TID_SendTransfer, "SendTransfer", "用户转账操作请求" }
	,{TID_RecvTransfer, "RecvTransfer", "用户转账操作应答" }
	,{TID_SendUserRegister, "SendUserRegister", "注册用户请求" }
	,{TID_RecvUserRegister, "RecvUserRegister", "注册用户应答" }
	,{TID_SendPositionReverse, "SendPositionReverse", "反向开仓录入请求" }
	,{TID_RecvPositionReverse, "RecvPositionReverse", "反向开仓录入应答" }
	,{TID_SendCancelOrderInsert, "SendCancelOrderInsert", "避免自成交报单录入请求" }
	,{TID_RecvCancelOrderInsert, "RecvCancelOrderInsert", "避免自成交报单录入应答" }
	,{TID_SendPrivateDispatch, "SendPrivateDispatch", "私有主题分类请求" }
	,{TID_RecvPrivateDispatch, "RecvPrivateDispatch", "私有主题分类应答" }
	,{TID_SendPositionLeverage, "SendPositionLeverage", "调整多空杠杠请求" }
	,{TID_RecvPositionLeverage, "RecvPositionLeverage", "调整多空杠杠应答" }
	,{TID_SendInstrumentUpdate, "SendInstrumentUpdate", "设置合约请求" }
	,{TID_RecvInstrumentUpdate, "RecvInstrumentUpdate", "设置合约应答" }
	,{TID_SendChangeCurrency, "SendChangeCurrency", "充币兑换请求" }
	,{TID_RecvChangeCurrency, "RecvChangeCurrency", "充币兑换应答" }
	,{TID_SendWalletNonce, "SendWalletNonce", "钱包登录获取随机数请求" }
	,{TID_RecvWalletNonce, "RecvWalletNonce", "钱包登录获取随机数应答" }
	,{TID_SendWalletLogin, "SendWalletLogin", "钱包登录获取随机数请求" }
	,{TID_RecvWalletLogin, "RecvWalletLogin", "钱包登录获取随机数应答" }
	,{TID_SendQryAccount, "SendQryAccount", "资金查询请求" }
	,{TID_RecvQryAccount, "RecvQryAccount", "资金查询应答" }
	,{TID_SendQryOrder, "SendQryOrder", "报单查询请求" }
	,{TID_RecvQryOrder, "RecvQryOrder", "报单查询应答" }
	,{TID_SendQryMemberPosition, "SendQryMemberPosition", "持仓汇总查询请求" }
	,{TID_RecvQryMemberPosition, "RecvQryMemberPosition", "持仓汇总查询应答" }
	,{TID_SendQryPosition, "SendQryPosition", "持仓查询请求" }
	,{TID_RecvQryPosition, "RecvQryPosition", "持仓查询应答" }
	,{TID_SendQryInstrument, "SendQryInstrument", "标的查询请求" }
	,{TID_RecvQryInstrument, "RecvQryInstrument", "标的查询应答" }
	,{TID_SendQryCurrency, "SendQryCurrency", "币种查询请求" }
	,{TID_RecvQryCurrency, "RecvQryCurrency", "币种查询应答" }
	,{TID_SendQryMember, "SendQryMember", "成员查询请求" }
	,{TID_RecvQryMember, "RecvQryMember", "成员查询应答" }
	,{TID_SendQryMarketOrder, "SendQryMarketOrder", "价位查询请求" }
	,{TID_RecvQryMarketOrder, "RecvQryMarketOrder", "价位查询应答" }
	,{TID_SendQryAPIKey, "SendQryAPIKey", "APIKEY查询请求" }
	,{TID_RecvQryAPIKey, "RecvQryAPIKey", "APIKEY查询应答" }
	,{TID_SendQryMarginRate, "SendQryMarginRate", "保证金率查询请求" }
	,{TID_RecvQryMarginRate, "RecvQryMarginRate", "保证金率查询应答" }
	,{TID_SendQryFee, "SendQryFee", "手续费率查询请求" }
	,{TID_RecvQryFee, "RecvQryFee", "手续费率查询应答" }
	,{TID_SendQryTriggerOrder, "SendQryTriggerOrder", "触发订单查询请求" }
	,{TID_RecvQryTriggerOrder, "RecvQryTriggerOrder", "触发订单查询应答" }
	,{TID_SendQryLeverage, "SendQryLeverage", "杠杆倍数查询请求" }
	,{TID_RecvQryLeverage, "RecvQryLeverage", "杠杆倍数查询应答" }
	,{TID_SendQryMarketData, "SendQryMarketData", "行情查询请求" }
	,{TID_RecvQryMarketData, "RecvQryMarketData", "行情查询应答" }
	,{TID_SendQryTrade, "SendQryTrade", "成交查询请求" }
	,{TID_RecvQryTrade, "RecvQryTrade", "成交查询应答" }
	,{TID_SendQryFinishOrder, "SendQryFinishOrder", "历史委托查询请求" }
	,{TID_RecvQryFinishOrder, "RecvQryFinishOrder", "历史委托查询应答" }
	,{TID_SendQryFinishTriggerOrder, "SendQryFinishTriggerOrder", "历史条件单查询请求" }
	,{TID_RecvQryFinishTriggerOrder, "RecvQryFinishTriggerOrder", "历史条件单查询应答" }
	,{TID_SendQryWithLeverage, "SendQryWithLeverage", "指定杠杆倍数查询请求" }
	,{TID_RecvQryWithLeverage, "RecvQryWithLeverage", "指定杠杆倍数查询应答" }
	,{TID_SendQryKLine, "SendQryKLine", "K线查询请求" }
	,{TID_RecvQryKLine, "RecvQryKLine", "K线查询应答" }
	,{TID_SendQryLine, "SendQryLine", "线查询请求" }
	,{TID_RecvQryLine, "RecvQryLine", "线查询应答" }
	,{TID_SendQryAccountDetail, "SendQryAccountDetail", "资金流水查询请求" }
	,{TID_RecvQryAccountDetail, "RecvQryAccountDetail", "资金流水查询应答" }
	,{TID_SendQrySettleDetail, "SendQrySettleDetail", "结算流水查询请求" }
	,{TID_RecvQrySettleDetail, "RecvQrySettleDetail", "结算流水查询应答" }
	,{TID_SendCheckToken, "SendCheckToken", "检查TOKEN请求" }
	,{TID_RecvCheckToken, "RecvCheckToken", "检查TOKEN应答" }
	,{TID_SendBatch, "SendBatch", "批量操作请求(HTTP接口专用)" }
	,{TID_PushTimeSync, "PushTimeSync", "时间同步通知" }
	,{TID_PushCommand, "PushCommand", "消息通知" }
	,{TID_PushTrade, "PushTrade", "成交通知" }
	,{TID_PushOrder, "PushOrder", "报单通知" }
	,{TID_PushMemberPosition, "PushMemberPosition", "持仓汇总通知" }
	,{TID_PushPosition, "PushPosition", "持仓通知" }
	,{TID_PushAccount, "PushAccount", "资金通知" }
	,{TID_PushMarketTrade, "PushMarketTrade", "市场成交通知" }
	,{TID_PushMarketOrder, "PushMarketOrder", "订单簿变化通知" }
	,{TID_PushKLine, "PushKLine", "K线通知" }
	,{TID_PushMarketOverView, "PushMarketOverView", "普通行情通知" }
	,{TID_PushDelayMarketOrder, "PushDelayMarketOrder", "全量延时订单簿变化通知" }
	,{TID_PushFiveDepthMarketData, "PushFiveDepthMarketData", "五档深度行情通知" }
	,{TID_PushTenDepthMarketData, "PushTenDepthMarketData", "十档深度行情通知" }
	,{TID_PushOptionsMarketData, "PushOptionsMarketData", "期权行情通知" }
	,{TID_PushFee, "PushFee", "手续费通知" }
	,{TID_PushMarginRate, "PushMarginRate", "保证金率通知" }
	,{TID_PushMember, "PushMember", "成员通知" }
	,{TID_PushTriggerOrder, "PushTriggerOrder", "触发单通知" }
	,{TID_PushInstrument, "PushInstrument", "合约通知" }
	,{TID_PushUserSession, "PushUserSession", "用户登陆通知" }
	,{TID_PushCurrency, "PushCurrency", "币种通知" }
	,{TID_DataSyncStart, "DataSyncStart", "数据同步开始" }
	,{TID_DataSyncEnd, "DataSyncEnd", "数据同步结束" }
	,{TID_SendInsert, "SendInsert", "插入操作请求" }
	,{TID_RecvInsert, "RecvInsert", "插入操作应答" }
	,{TID_SendUpdate, "SendUpdate", "更新操作请求" }
	,{TID_RecvUpdate, "RecvUpdate", "更新操作应答" }
	,{TID_SendDelete, "SendDelete", "删除操作请求" }
	,{TID_RecvDelete, "RecvDelete", "删除操作应答" }
	,{TID_SendInsUpd, "SendInsUpd", "插入没有就更新请求" }
	,{TID_RecvInsUpd, "RecvInsUpd", "插入没有就更新应答" }
	,{TID_SendQryOne, "SendQryOne", "查询唯一索引记录请求" }
	,{TID_RecvQryOne, "RecvQryOne", "查询唯一索引记录应答" }
	,{TID_SendClear, "SendClear", "设置字段值清空请求" }
	,{TID_RecvClear, "RecvClear", "设置字段值清空应答" }
	,{TID_SendInit, "SendInit", "初始化操作请求" }
	,{TID_RecvInit, "RecvInit", "初始化操作应答" }
};

CXTPPackageDefineMap g_XTPPackageDefineMap(XTPPackageDefines, sizeof(XTPPackageDefines)/ sizeof(TPackageDefine));
CXTPPackageDefineNameMap g_XTPPackageDefineNameMap(XTPPackageDefines, sizeof(XTPPackageDefines)/ sizeof(TPackageDefine));
CFieldUseMap g_FieldUseMap(FieldUses, sizeof(FieldUses)/ sizeof(TFieldUse));
CFieldNameMap g_FieldNameMap(FieldUses, sizeof(FieldUses) / sizeof(TFieldUse));
