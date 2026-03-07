/////////////////////////////////////////////////////////////////////////
///OneMegaApiImpl.cpp
///实现了客户端接口
/////////////////////////////////////////////////////////////////////////

#include "public.h"
#include "ApiImpl.h"
#include "FieldData.h"
#include "OutFogService.h"

CApiImpl::CApiImpl()
{
	//m_pOutFogServiceApi = CreateOutFogServiceApi(this, 0);
	m_pOutFogServiceApi = COutFogService::LoadOutFogApi("OneMegaApiX", this, 0);
	memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
}

COneMegaApi *COneMegaApi::CreateApi()
{
	return new CApiImpl();
}

const char *COneMegaApi::GetVersion()
{
	return ""; 
}

void CApiImpl::DisConnect()
{
	//DisconnectAll(0);
}

void CApiImpl::Connect()
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_START;
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, NULL);
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
}

void CApiImpl::SetAddress(const char *pAddress)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_INITLINK;
	//不能超过头外面的字符能占用的字节
	strncpy(m_OutFogMetaRef.Remark, pAddress, 124);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, NULL);
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
}

int CApiImpl::SetCertificateFile(const char *pszCertFileName, const char *pszKeyFileName,
	const char *pszCaFileName, const char *pszKeyFilePassword)
{
	//	return CSslNetworkFactory::m_instance.RegisterCertificateFile(true, pszCertFileName, pszKeyFileName, 
	//		pszCaFileName, pszKeyFilePassword);
	return 0;
}

void CApiImpl::SetCallback(COneMegaCallback *pCallback)
{
	m_pCallback = pCallback;
}

///一般消息请求
int CApiImpl::SendCommand(MegaCommandStruct *pCommand, int nRequestID)
{
	CCommandField CommandField;
	memcpy(&CommandField, pCommand, sizeof(CCommandField));
	if (! CommandField.UserID.isValid())
	{
		PRINT_TO_STD("CommandField.UserID[%s]isInValid", CommandField.UserID.getString().c_str());
		return -4;
	}
	if (! CommandField.LocalID.isValid())
	{
		PRINT_TO_STD("CommandField.LocalID[%s]isInValid", CommandField.LocalID.getString().c_str());
		return -4;
	}
	if (! CommandField.Command.isValid())
	{
		PRINT_TO_STD("CommandField.Command[%s]isInValid", CommandField.Command.getString().c_str());
		return -4;
	}
	if (! CommandField.Content.isValid())
	{
		PRINT_TO_STD("CommandField.Content[%s]isInValid", CommandField.Content.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(CommandField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendCommand;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &CommandField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///创建上层连接请求
int CApiImpl::SendInitLink(MegaLinkStruct *pLink, int nRequestID)
{
	CLinkField LinkField;
	memcpy(&LinkField, pLink, sizeof(CLinkField));
	if (! LinkField.LinkNo.isValid())
	{
		PRINT_TO_STD("LinkField.LinkNo[%s]isInValid", LinkField.LinkNo.getString().c_str());
		return -4;
	}
	if (! LinkField.LinkUserID.isValid())
	{
		PRINT_TO_STD("LinkField.LinkUserID[%s]isInValid", LinkField.LinkUserID.getString().c_str());
		return -4;
	}
	if (! LinkField.LinkPassword.isValid())
	{
		PRINT_TO_STD("LinkField.LinkPassword[%s]isInValid", LinkField.LinkPassword.getString().c_str());
		return -4;
	}
	if (! LinkField.EncryptType.isValid())
	{
		PRINT_TO_STD("LinkField.EncryptType[%s]isInValid", LinkField.EncryptType.getString().c_str());
		return -4;
	}
	if (! LinkField.LinkDllName.isValid())
	{
		PRINT_TO_STD("LinkField.LinkDllName[%s]isInValid", LinkField.LinkDllName.getString().c_str());
		return -4;
	}
	if (! LinkField.LinkDllLoad.isValid())
	{
		PRINT_TO_STD("LinkField.LinkDllLoad[%s]isInValid", LinkField.LinkDllLoad.getString().c_str());
		return -4;
	}
	if (! LinkField.LinkAddress.isValid())
	{
		PRINT_TO_STD("LinkField.LinkAddress[%s]isInValid", LinkField.LinkAddress.getString().c_str());
		return -4;
	}
	if (! LinkField.LinkStatus.isValid())
	{
		PRINT_TO_STD("LinkField.LinkStatus[%s]isInValid", LinkField.LinkStatus.getString().c_str());
		return -4;
	}
	if (! LinkField.IsActive.isValid())
	{
		PRINT_TO_STD("LinkField.IsActive[%s]isInValid", LinkField.IsActive.getString().c_str());
		return -4;
	}
	if (! LinkField.Config.isValid())
	{
		PRINT_TO_STD("LinkField.Config[%s]isInValid", LinkField.Config.getString().c_str());
		return -4;
	}
	if (! LinkField.Remark.isValid())
	{
		PRINT_TO_STD("LinkField.Remark[%s]isInValid", LinkField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(LinkField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendInitLink;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &LinkField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///用户登录请求
int CApiImpl::SendUserLogin(MegaUserLoginStruct *pUserLogin, int nRequestID)
{
	CUserLoginField UserLoginField;
	memcpy(&UserLoginField, pUserLogin, sizeof(CUserLoginField));
	if (! UserLoginField.UserID.isValid())
	{
		PRINT_TO_STD("UserLoginField.UserID[%s]isInValid", UserLoginField.UserID.getString().c_str());
		return -4;
	}
	if (! UserLoginField.Password.isValid())
	{
		PRINT_TO_STD("UserLoginField.Password[%s]isInValid", UserLoginField.Password.getString().c_str());
		return -4;
	}
	if (! UserLoginField.UserProductID.isValid())
	{
		PRINT_TO_STD("UserLoginField.UserProductID[%s]isInValid", UserLoginField.UserProductID.getString().c_str());
		return -4;
	}
	if (! UserLoginField.MacAddress.isValid())
	{
		PRINT_TO_STD("UserLoginField.MacAddress[%s]isInValid", UserLoginField.MacAddress.getString().c_str());
		return -4;
	}
	if (! UserLoginField.ClientIPAddress.isValid())
	{
		PRINT_TO_STD("UserLoginField.ClientIPAddress[%s]isInValid", UserLoginField.ClientIPAddress.getString().c_str());
		return -4;
	}
	if (! UserLoginField.HDSerialID.isValid())
	{
		PRINT_TO_STD("UserLoginField.HDSerialID[%s]isInValid", UserLoginField.HDSerialID.getString().c_str());
		return -4;
	}
	if (! UserLoginField.AuthCode.isValid())
	{
		PRINT_TO_STD("UserLoginField.AuthCode[%s]isInValid", UserLoginField.AuthCode.getString().c_str());
		return -4;
	}
	if (! UserLoginField.APPID.isValid())
	{
		PRINT_TO_STD("UserLoginField.APPID[%s]isInValid", UserLoginField.APPID.getString().c_str());
		return -4;
	}
	if (! UserLoginField.Token.isValid())
	{
		PRINT_TO_STD("UserLoginField.Token[%s]isInValid", UserLoginField.Token.getString().c_str());
		return -4;
	}
	if (! UserLoginField.Remark.isValid())
	{
		PRINT_TO_STD("UserLoginField.Remark[%s]isInValid", UserLoginField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(UserLoginField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendUserLogin;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &UserLoginField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///用户退出请求
int CApiImpl::SendUserLogout(MegaUserLogoutStruct *pUserLogout, int nRequestID)
{
	CUserLogoutField UserLogoutField;
	memcpy(&UserLogoutField, pUserLogout, sizeof(CUserLogoutField));
	if (! UserLogoutField.UserID.isValid())
	{
		PRINT_TO_STD("UserLogoutField.UserID[%s]isInValid", UserLogoutField.UserID.getString().c_str());
		return -4;
	}
	if (! UserLogoutField.APPID.isValid())
	{
		PRINT_TO_STD("UserLogoutField.APPID[%s]isInValid", UserLogoutField.APPID.getString().c_str());
		return -4;
	}
	if (! UserLogoutField.Token.isValid())
	{
		PRINT_TO_STD("UserLogoutField.Token[%s]isInValid", UserLogoutField.Token.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(UserLogoutField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendUserLogout;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &UserLogoutField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///报单录入请求
int CApiImpl::SendOrderInsert(MegaOrderInsertStruct *pOrderInsert, int nRequestID)
{
	COrderInsertField OrderInsertField;
	memcpy(&OrderInsertField, pOrderInsert, sizeof(COrderInsertField));
	if (! OrderInsertField.MemberID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.MemberID[%s]isInValid", OrderInsertField.MemberID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.TradeUnitID[%s]isInValid", OrderInsertField.TradeUnitID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.AccountID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.AccountID[%s]isInValid", OrderInsertField.AccountID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.LocalID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.LocalID[%s]isInValid", OrderInsertField.LocalID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.ExchangeID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.ExchangeID[%s]isInValid", OrderInsertField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.InstrumentID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.InstrumentID[%s]isInValid", OrderInsertField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.OrderPriceType.isValid())
	{
		PRINT_TO_STD("OrderInsertField.OrderPriceType[%s]isInValid", OrderInsertField.OrderPriceType.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Direction.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Direction[%s]isInValid", OrderInsertField.Direction.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.OffsetFlag.isValid())
	{
		PRINT_TO_STD("OrderInsertField.OffsetFlag[%s]isInValid", OrderInsertField.OffsetFlag.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Price.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Price[%s]isInValid", OrderInsertField.Price.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Volume.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Volume[%s]isInValid", OrderInsertField.Volume.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.VolumeDisplay.isValid())
	{
		PRINT_TO_STD("OrderInsertField.VolumeDisplay[%s]isInValid", OrderInsertField.VolumeDisplay.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.VolumeMode.isValid())
	{
		PRINT_TO_STD("OrderInsertField.VolumeMode[%s]isInValid", OrderInsertField.VolumeMode.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Cost.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Cost[%s]isInValid", OrderInsertField.Cost.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.OrderType.isValid())
	{
		PRINT_TO_STD("OrderInsertField.OrderType[%s]isInValid", OrderInsertField.OrderType.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.GTDTime.isValid())
	{
		PRINT_TO_STD("OrderInsertField.GTDTime[%s]isInValid", OrderInsertField.GTDTime.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.MinVolume.isValid())
	{
		PRINT_TO_STD("OrderInsertField.MinVolume[%s]isInValid", OrderInsertField.MinVolume.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.BusinessType.isValid())
	{
		PRINT_TO_STD("OrderInsertField.BusinessType[%s]isInValid", OrderInsertField.BusinessType.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.BusinessValue.isValid())
	{
		PRINT_TO_STD("OrderInsertField.BusinessValue[%s]isInValid", OrderInsertField.BusinessValue.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.CloseOrderID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.CloseOrderID[%s]isInValid", OrderInsertField.CloseOrderID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.IsCrossMargin.isValid())
	{
		PRINT_TO_STD("OrderInsertField.IsCrossMargin[%s]isInValid", OrderInsertField.IsCrossMargin.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Remark.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Remark[%s]isInValid", OrderInsertField.Remark.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.OrderID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.OrderID[%s]isInValid", OrderInsertField.OrderID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.CopyMemberID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.CopyMemberID[%s]isInValid", OrderInsertField.CopyMemberID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.CopyOrderID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.CopyOrderID[%s]isInValid", OrderInsertField.CopyOrderID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Leverage.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Leverage[%s]isInValid", OrderInsertField.Leverage.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.CopyProfitRate.isValid())
	{
		PRINT_TO_STD("OrderInsertField.CopyProfitRate[%s]isInValid", OrderInsertField.CopyProfitRate.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.APPID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.APPID[%s]isInValid", OrderInsertField.APPID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.PositionID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.PositionID[%s]isInValid", OrderInsertField.PositionID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.TriggerPrice.isValid())
	{
		PRINT_TO_STD("OrderInsertField.TriggerPrice[%s]isInValid", OrderInsertField.TriggerPrice.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Reserve.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Reserve[%s]isInValid", OrderInsertField.Reserve.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(OrderInsertField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendOrderInsert;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &OrderInsertField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///报单操作请求
int CApiImpl::SendOrderAction(MegaOrderActionStruct *pOrderAction, int nRequestID)
{
	COrderActionField OrderActionField;
	memcpy(&OrderActionField, pOrderAction, sizeof(COrderActionField));
	if (! OrderActionField.OrderID.isValid())
	{
		PRINT_TO_STD("OrderActionField.OrderID[%s]isInValid", OrderActionField.OrderID.getString().c_str());
		return -4;
	}
	if (! OrderActionField.OrderLocalID.isValid())
	{
		PRINT_TO_STD("OrderActionField.OrderLocalID[%s]isInValid", OrderActionField.OrderLocalID.getString().c_str());
		return -4;
	}
	if (! OrderActionField.ActionFlag.isValid())
	{
		PRINT_TO_STD("OrderActionField.ActionFlag[%s]isInValid", OrderActionField.ActionFlag.getString().c_str());
		return -4;
	}
	if (! OrderActionField.LocalID.isValid())
	{
		PRINT_TO_STD("OrderActionField.LocalID[%s]isInValid", OrderActionField.LocalID.getString().c_str());
		return -4;
	}
	if (! OrderActionField.MemberID.isValid())
	{
		PRINT_TO_STD("OrderActionField.MemberID[%s]isInValid", OrderActionField.MemberID.getString().c_str());
		return -4;
	}
	if (! OrderActionField.AccountID.isValid())
	{
		PRINT_TO_STD("OrderActionField.AccountID[%s]isInValid", OrderActionField.AccountID.getString().c_str());
		return -4;
	}
	if (! OrderActionField.Price.isValid())
	{
		PRINT_TO_STD("OrderActionField.Price[%s]isInValid", OrderActionField.Price.getString().c_str());
		return -4;
	}
	if (! OrderActionField.Volume.isValid())
	{
		PRINT_TO_STD("OrderActionField.Volume[%s]isInValid", OrderActionField.Volume.getString().c_str());
		return -4;
	}
	if (! OrderActionField.Direction.isValid())
	{
		PRINT_TO_STD("OrderActionField.Direction[%s]isInValid", OrderActionField.Direction.getString().c_str());
		return -4;
	}
	if (! OrderActionField.VolumeDisplay.isValid())
	{
		PRINT_TO_STD("OrderActionField.VolumeDisplay[%s]isInValid", OrderActionField.VolumeDisplay.getString().c_str());
		return -4;
	}
	if (! OrderActionField.ProductGroup.isValid())
	{
		PRINT_TO_STD("OrderActionField.ProductGroup[%s]isInValid", OrderActionField.ProductGroup.getString().c_str());
		return -4;
	}
	if (! OrderActionField.ExchangeID.isValid())
	{
		PRINT_TO_STD("OrderActionField.ExchangeID[%s]isInValid", OrderActionField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! OrderActionField.InstrumentID.isValid())
	{
		PRINT_TO_STD("OrderActionField.InstrumentID[%s]isInValid", OrderActionField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! OrderActionField.Cost.isValid())
	{
		PRINT_TO_STD("OrderActionField.Cost[%s]isInValid", OrderActionField.Cost.getString().c_str());
		return -4;
	}
	if (! OrderActionField.Remark.isValid())
	{
		PRINT_TO_STD("OrderActionField.Remark[%s]isInValid", OrderActionField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(OrderActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendOrderAction;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &OrderActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///触发单录入请求
int CApiImpl::SendTriggerOrderInsert(MegaTriggerOrderInsertStruct *pTriggerOrderInsert, int nRequestID)
{
	CTriggerOrderInsertField TriggerOrderInsertField;
	memcpy(&TriggerOrderInsertField, pTriggerOrderInsert, sizeof(CTriggerOrderInsertField));
	if (! TriggerOrderInsertField.MemberID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.MemberID[%s]isInValid", TriggerOrderInsertField.MemberID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.TradeUnitID[%s]isInValid", TriggerOrderInsertField.TradeUnitID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.AccountID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.AccountID[%s]isInValid", TriggerOrderInsertField.AccountID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.LocalID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.LocalID[%s]isInValid", TriggerOrderInsertField.LocalID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.ExchangeID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.ExchangeID[%s]isInValid", TriggerOrderInsertField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.InstrumentID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.InstrumentID[%s]isInValid", TriggerOrderInsertField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.OrderPriceType.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.OrderPriceType[%s]isInValid", TriggerOrderInsertField.OrderPriceType.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.Direction.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.Direction[%s]isInValid", TriggerOrderInsertField.Direction.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.OffsetFlag.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.OffsetFlag[%s]isInValid", TriggerOrderInsertField.OffsetFlag.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.Price.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.Price[%s]isInValid", TriggerOrderInsertField.Price.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.Volume.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.Volume[%s]isInValid", TriggerOrderInsertField.Volume.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.VolumeDisplay.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.VolumeDisplay[%s]isInValid", TriggerOrderInsertField.VolumeDisplay.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.VolumeMode.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.VolumeMode[%s]isInValid", TriggerOrderInsertField.VolumeMode.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.Cost.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.Cost[%s]isInValid", TriggerOrderInsertField.Cost.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.OrderType.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.OrderType[%s]isInValid", TriggerOrderInsertField.OrderType.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.GTDTime.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.GTDTime[%s]isInValid", TriggerOrderInsertField.GTDTime.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.MinVolume.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.MinVolume[%s]isInValid", TriggerOrderInsertField.MinVolume.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.BusinessType.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.BusinessType[%s]isInValid", TriggerOrderInsertField.BusinessType.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.BusinessValue.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.BusinessValue[%s]isInValid", TriggerOrderInsertField.BusinessValue.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.CloseOrderID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.CloseOrderID[%s]isInValid", TriggerOrderInsertField.CloseOrderID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.IsCrossMargin.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.IsCrossMargin[%s]isInValid", TriggerOrderInsertField.IsCrossMargin.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.Remark.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.Remark[%s]isInValid", TriggerOrderInsertField.Remark.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.OrderID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.OrderID[%s]isInValid", TriggerOrderInsertField.OrderID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.CopyMemberID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.CopyMemberID[%s]isInValid", TriggerOrderInsertField.CopyMemberID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.CopyOrderID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.CopyOrderID[%s]isInValid", TriggerOrderInsertField.CopyOrderID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.Leverage.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.Leverage[%s]isInValid", TriggerOrderInsertField.Leverage.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.CopyProfitRate.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.CopyProfitRate[%s]isInValid", TriggerOrderInsertField.CopyProfitRate.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.APPID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.APPID[%s]isInValid", TriggerOrderInsertField.APPID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.PositionID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.PositionID[%s]isInValid", TriggerOrderInsertField.PositionID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.TriggerPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.TriggerPrice[%s]isInValid", TriggerOrderInsertField.TriggerPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.Reserve.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.Reserve[%s]isInValid", TriggerOrderInsertField.Reserve.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.SLPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.SLPrice[%s]isInValid", TriggerOrderInsertField.SLPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.SLTriggerPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.SLTriggerPrice[%s]isInValid", TriggerOrderInsertField.SLTriggerPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.TPPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.TPPrice[%s]isInValid", TriggerOrderInsertField.TPPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.TPTriggerPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.TPTriggerPrice[%s]isInValid", TriggerOrderInsertField.TPTriggerPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.RiskBefore.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.RiskBefore[%s]isInValid", TriggerOrderInsertField.RiskBefore.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.TriggerOrderType.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.TriggerOrderType[%s]isInValid", TriggerOrderInsertField.TriggerOrderType.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.TriggerDetail.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.TriggerDetail[%s]isInValid", TriggerOrderInsertField.TriggerDetail.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.TriggerPriceType.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.TriggerPriceType[%s]isInValid", TriggerOrderInsertField.TriggerPriceType.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.TriggerValue.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.TriggerValue[%s]isInValid", TriggerOrderInsertField.TriggerValue.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.CloseSLPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.CloseSLPrice[%s]isInValid", TriggerOrderInsertField.CloseSLPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.CloseSLTriggerPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.CloseSLTriggerPrice[%s]isInValid", TriggerOrderInsertField.CloseSLTriggerPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.CloseTPPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.CloseTPPrice[%s]isInValid", TriggerOrderInsertField.CloseTPPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.CloseTPTriggerPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.CloseTPTriggerPrice[%s]isInValid", TriggerOrderInsertField.CloseTPTriggerPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.CloseOrderPriceType.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.CloseOrderPriceType[%s]isInValid", TriggerOrderInsertField.CloseOrderPriceType.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.ClosePrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.ClosePrice[%s]isInValid", TriggerOrderInsertField.ClosePrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.CloseTriggerPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.CloseTriggerPrice[%s]isInValid", TriggerOrderInsertField.CloseTriggerPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.RelatedOrderID.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.RelatedOrderID[%s]isInValid", TriggerOrderInsertField.RelatedOrderID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.ActiveTime.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.ActiveTime[%s]isInValid", TriggerOrderInsertField.ActiveTime.getString().c_str());
		return -4;
	}
	if (! TriggerOrderInsertField.TriggerTime.isValid())
	{
		PRINT_TO_STD("TriggerOrderInsertField.TriggerTime[%s]isInValid", TriggerOrderInsertField.TriggerTime.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(TriggerOrderInsertField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendTriggerOrderInsert;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &TriggerOrderInsertField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///触发单操作请求
int CApiImpl::SendTriggerOrderAction(MegaTriggerOrderActionStruct *pTriggerOrderAction, int nRequestID)
{
	CTriggerOrderActionField TriggerOrderActionField;
	memcpy(&TriggerOrderActionField, pTriggerOrderAction, sizeof(CTriggerOrderActionField));
	if (! TriggerOrderActionField.OrderID.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.OrderID[%s]isInValid", TriggerOrderActionField.OrderID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.OrderLocalID.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.OrderLocalID[%s]isInValid", TriggerOrderActionField.OrderLocalID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.ActionFlag.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.ActionFlag[%s]isInValid", TriggerOrderActionField.ActionFlag.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.LocalID.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.LocalID[%s]isInValid", TriggerOrderActionField.LocalID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.MemberID.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.MemberID[%s]isInValid", TriggerOrderActionField.MemberID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.AccountID.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.AccountID[%s]isInValid", TriggerOrderActionField.AccountID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.Price.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.Price[%s]isInValid", TriggerOrderActionField.Price.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.Volume.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.Volume[%s]isInValid", TriggerOrderActionField.Volume.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.Direction.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.Direction[%s]isInValid", TriggerOrderActionField.Direction.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.VolumeDisplay.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.VolumeDisplay[%s]isInValid", TriggerOrderActionField.VolumeDisplay.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.ProductGroup.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.ProductGroup[%s]isInValid", TriggerOrderActionField.ProductGroup.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.ExchangeID.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.ExchangeID[%s]isInValid", TriggerOrderActionField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.InstrumentID.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.InstrumentID[%s]isInValid", TriggerOrderActionField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.Cost.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.Cost[%s]isInValid", TriggerOrderActionField.Cost.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.Remark.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.Remark[%s]isInValid", TriggerOrderActionField.Remark.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.SLPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.SLPrice[%s]isInValid", TriggerOrderActionField.SLPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.SLTriggerPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.SLTriggerPrice[%s]isInValid", TriggerOrderActionField.SLTriggerPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.TPPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.TPPrice[%s]isInValid", TriggerOrderActionField.TPPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.TPTriggerPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.TPTriggerPrice[%s]isInValid", TriggerOrderActionField.TPTriggerPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.TriggerPrice.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.TriggerPrice[%s]isInValid", TriggerOrderActionField.TriggerPrice.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.TriggerOrderType.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.TriggerOrderType[%s]isInValid", TriggerOrderActionField.TriggerOrderType.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.TriggerValue.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.TriggerValue[%s]isInValid", TriggerOrderActionField.TriggerValue.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.TriggerDetail.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.TriggerDetail[%s]isInValid", TriggerOrderActionField.TriggerDetail.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.TriggerPriceType.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.TriggerPriceType[%s]isInValid", TriggerOrderActionField.TriggerPriceType.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.ErrorNo.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.ErrorNo[%s]isInValid", TriggerOrderActionField.ErrorNo.getString().c_str());
		return -4;
	}
	if (! TriggerOrderActionField.ErrorMsg.isValid())
	{
		PRINT_TO_STD("TriggerOrderActionField.ErrorMsg[%s]isInValid", TriggerOrderActionField.ErrorMsg.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(TriggerOrderActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendTriggerOrderAction;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &TriggerOrderActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///止盈止损报单录入请求
int CApiImpl::SendCloseOrderInsert(MegaCloseOrderInsertStruct *pCloseOrderInsert, int nRequestID)
{
	CCloseOrderInsertField CloseOrderInsertField;
	memcpy(&CloseOrderInsertField, pCloseOrderInsert, sizeof(CCloseOrderInsertField));
	if (! CloseOrderInsertField.MemberID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.MemberID[%s]isInValid", CloseOrderInsertField.MemberID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.TradeUnitID[%s]isInValid", CloseOrderInsertField.TradeUnitID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.AccountID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.AccountID[%s]isInValid", CloseOrderInsertField.AccountID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.LocalID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.LocalID[%s]isInValid", CloseOrderInsertField.LocalID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.ExchangeID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.ExchangeID[%s]isInValid", CloseOrderInsertField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.InstrumentID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.InstrumentID[%s]isInValid", CloseOrderInsertField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.OrderPriceType.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.OrderPriceType[%s]isInValid", CloseOrderInsertField.OrderPriceType.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.Direction.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.Direction[%s]isInValid", CloseOrderInsertField.Direction.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.OffsetFlag.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.OffsetFlag[%s]isInValid", CloseOrderInsertField.OffsetFlag.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.Price.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.Price[%s]isInValid", CloseOrderInsertField.Price.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.Volume.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.Volume[%s]isInValid", CloseOrderInsertField.Volume.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.VolumeDisplay.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.VolumeDisplay[%s]isInValid", CloseOrderInsertField.VolumeDisplay.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.VolumeMode.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.VolumeMode[%s]isInValid", CloseOrderInsertField.VolumeMode.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.Cost.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.Cost[%s]isInValid", CloseOrderInsertField.Cost.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.OrderType.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.OrderType[%s]isInValid", CloseOrderInsertField.OrderType.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.GTDTime.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.GTDTime[%s]isInValid", CloseOrderInsertField.GTDTime.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.MinVolume.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.MinVolume[%s]isInValid", CloseOrderInsertField.MinVolume.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.BusinessType.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.BusinessType[%s]isInValid", CloseOrderInsertField.BusinessType.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.BusinessValue.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.BusinessValue[%s]isInValid", CloseOrderInsertField.BusinessValue.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CloseOrderID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CloseOrderID[%s]isInValid", CloseOrderInsertField.CloseOrderID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.IsCrossMargin.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.IsCrossMargin[%s]isInValid", CloseOrderInsertField.IsCrossMargin.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.Remark.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.Remark[%s]isInValid", CloseOrderInsertField.Remark.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.OrderID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.OrderID[%s]isInValid", CloseOrderInsertField.OrderID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CopyMemberID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CopyMemberID[%s]isInValid", CloseOrderInsertField.CopyMemberID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CopyOrderID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CopyOrderID[%s]isInValid", CloseOrderInsertField.CopyOrderID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.Leverage.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.Leverage[%s]isInValid", CloseOrderInsertField.Leverage.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CopyProfitRate.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CopyProfitRate[%s]isInValid", CloseOrderInsertField.CopyProfitRate.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.APPID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.APPID[%s]isInValid", CloseOrderInsertField.APPID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.PositionID.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.PositionID[%s]isInValid", CloseOrderInsertField.PositionID.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.TriggerPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.TriggerPrice[%s]isInValid", CloseOrderInsertField.TriggerPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.Reserve.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.Reserve[%s]isInValid", CloseOrderInsertField.Reserve.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CloseSLPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CloseSLPrice[%s]isInValid", CloseOrderInsertField.CloseSLPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.SLTriggerPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.SLTriggerPrice[%s]isInValid", CloseOrderInsertField.SLTriggerPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CloseSLTriggerPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CloseSLTriggerPrice[%s]isInValid", CloseOrderInsertField.CloseSLTriggerPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CloseTPPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CloseTPPrice[%s]isInValid", CloseOrderInsertField.CloseTPPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.TPTriggerPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.TPTriggerPrice[%s]isInValid", CloseOrderInsertField.TPTriggerPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CloseTPTriggerPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CloseTPTriggerPrice[%s]isInValid", CloseOrderInsertField.CloseTPTriggerPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CloseOrderPriceType.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CloseOrderPriceType[%s]isInValid", CloseOrderInsertField.CloseOrderPriceType.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.ClosePrice.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.ClosePrice[%s]isInValid", CloseOrderInsertField.ClosePrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CloseTriggerPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CloseTriggerPrice[%s]isInValid", CloseOrderInsertField.CloseTriggerPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderInsertField.CloseTriggerPriceType.isValid())
	{
		PRINT_TO_STD("CloseOrderInsertField.CloseTriggerPriceType[%s]isInValid", CloseOrderInsertField.CloseTriggerPriceType.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(CloseOrderInsertField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendCloseOrderInsert;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &CloseOrderInsertField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///止盈止损报单操作请求
int CApiImpl::SendCloseOrderAction(MegaCloseOrderActionStruct *pCloseOrderAction, int nRequestID)
{
	CCloseOrderActionField CloseOrderActionField;
	memcpy(&CloseOrderActionField, pCloseOrderAction, sizeof(CCloseOrderActionField));
	if (! CloseOrderActionField.OrderID.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.OrderID[%s]isInValid", CloseOrderActionField.OrderID.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.OrderLocalID.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.OrderLocalID[%s]isInValid", CloseOrderActionField.OrderLocalID.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.ActionFlag.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.ActionFlag[%s]isInValid", CloseOrderActionField.ActionFlag.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.LocalID.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.LocalID[%s]isInValid", CloseOrderActionField.LocalID.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.MemberID.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.MemberID[%s]isInValid", CloseOrderActionField.MemberID.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.AccountID.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.AccountID[%s]isInValid", CloseOrderActionField.AccountID.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.Price.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.Price[%s]isInValid", CloseOrderActionField.Price.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.Volume.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.Volume[%s]isInValid", CloseOrderActionField.Volume.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.Direction.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.Direction[%s]isInValid", CloseOrderActionField.Direction.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.VolumeDisplay.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.VolumeDisplay[%s]isInValid", CloseOrderActionField.VolumeDisplay.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.ProductGroup.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.ProductGroup[%s]isInValid", CloseOrderActionField.ProductGroup.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.ExchangeID.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.ExchangeID[%s]isInValid", CloseOrderActionField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.InstrumentID.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.InstrumentID[%s]isInValid", CloseOrderActionField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.Cost.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.Cost[%s]isInValid", CloseOrderActionField.Cost.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.Remark.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.Remark[%s]isInValid", CloseOrderActionField.Remark.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.SLPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.SLPrice[%s]isInValid", CloseOrderActionField.SLPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.SLTriggerPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.SLTriggerPrice[%s]isInValid", CloseOrderActionField.SLTriggerPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.TPPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.TPPrice[%s]isInValid", CloseOrderActionField.TPPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.TPTriggerPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.TPTriggerPrice[%s]isInValid", CloseOrderActionField.TPTriggerPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.TriggerPrice.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.TriggerPrice[%s]isInValid", CloseOrderActionField.TriggerPrice.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.TriggerOrderType.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.TriggerOrderType[%s]isInValid", CloseOrderActionField.TriggerOrderType.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.TriggerValue.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.TriggerValue[%s]isInValid", CloseOrderActionField.TriggerValue.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.TriggerDetail.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.TriggerDetail[%s]isInValid", CloseOrderActionField.TriggerDetail.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.TriggerPriceType.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.TriggerPriceType[%s]isInValid", CloseOrderActionField.TriggerPriceType.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.ErrorNo.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.ErrorNo[%s]isInValid", CloseOrderActionField.ErrorNo.getString().c_str());
		return -4;
	}
	if (! CloseOrderActionField.ErrorMsg.isValid())
	{
		PRINT_TO_STD("CloseOrderActionField.ErrorMsg[%s]isInValid", CloseOrderActionField.ErrorMsg.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(CloseOrderActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendCloseOrderAction;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &CloseOrderActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///报价录入请求
int CApiImpl::SendQuoteInsert(MegaQuoteInsertStruct *pQuoteInsert, int nRequestID)
{
	CQuoteInsertField QuoteInsertField;
	memcpy(&QuoteInsertField, pQuoteInsert, sizeof(CQuoteInsertField));
	if (! QuoteInsertField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.ExchangeID[%s]isInValid", QuoteInsertField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QuoteInsertField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.InstrumentID[%s]isInValid", QuoteInsertField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QuoteInsertField.MemberID.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.MemberID[%s]isInValid", QuoteInsertField.MemberID.getString().c_str());
		return -4;
	}
	if (! QuoteInsertField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.TradeUnitID[%s]isInValid", QuoteInsertField.TradeUnitID.getString().c_str());
		return -4;
	}
	if (! QuoteInsertField.LocalID.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.LocalID[%s]isInValid", QuoteInsertField.LocalID.getString().c_str());
		return -4;
	}
	if (! QuoteInsertField.Volume.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.Volume[%s]isInValid", QuoteInsertField.Volume.getString().c_str());
		return -4;
	}
	if (! QuoteInsertField.BuyOffsetFlag.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.BuyOffsetFlag[%s]isInValid", QuoteInsertField.BuyOffsetFlag.getString().c_str());
		return -4;
	}
	if (! QuoteInsertField.BuyPrice.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.BuyPrice[%s]isInValid", QuoteInsertField.BuyPrice.getString().c_str());
		return -4;
	}
	if (! QuoteInsertField.SellOffsetFlag.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.SellOffsetFlag[%s]isInValid", QuoteInsertField.SellOffsetFlag.getString().c_str());
		return -4;
	}
	if (! QuoteInsertField.SellPrice.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.SellPrice[%s]isInValid", QuoteInsertField.SellPrice.getString().c_str());
		return -4;
	}
	if (! QuoteInsertField.Remark.isValid())
	{
		PRINT_TO_STD("QuoteInsertField.Remark[%s]isInValid", QuoteInsertField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QuoteInsertField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQuoteInsert;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QuoteInsertField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///报价操作请求
int CApiImpl::SendQuoteAction(MegaQuoteActionStruct *pQuoteAction, int nRequestID)
{
	CQuoteActionField QuoteActionField;
	memcpy(&QuoteActionField, pQuoteAction, sizeof(CQuoteActionField));
	if (! QuoteActionField.OrderID.isValid())
	{
		PRINT_TO_STD("QuoteActionField.OrderID[%s]isInValid", QuoteActionField.OrderID.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.OrderLocalID.isValid())
	{
		PRINT_TO_STD("QuoteActionField.OrderLocalID[%s]isInValid", QuoteActionField.OrderLocalID.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.ActionFlag.isValid())
	{
		PRINT_TO_STD("QuoteActionField.ActionFlag[%s]isInValid", QuoteActionField.ActionFlag.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.LocalID.isValid())
	{
		PRINT_TO_STD("QuoteActionField.LocalID[%s]isInValid", QuoteActionField.LocalID.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.MemberID.isValid())
	{
		PRINT_TO_STD("QuoteActionField.MemberID[%s]isInValid", QuoteActionField.MemberID.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.AccountID.isValid())
	{
		PRINT_TO_STD("QuoteActionField.AccountID[%s]isInValid", QuoteActionField.AccountID.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.Price.isValid())
	{
		PRINT_TO_STD("QuoteActionField.Price[%s]isInValid", QuoteActionField.Price.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.Volume.isValid())
	{
		PRINT_TO_STD("QuoteActionField.Volume[%s]isInValid", QuoteActionField.Volume.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.Direction.isValid())
	{
		PRINT_TO_STD("QuoteActionField.Direction[%s]isInValid", QuoteActionField.Direction.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.VolumeDisplay.isValid())
	{
		PRINT_TO_STD("QuoteActionField.VolumeDisplay[%s]isInValid", QuoteActionField.VolumeDisplay.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.ProductGroup.isValid())
	{
		PRINT_TO_STD("QuoteActionField.ProductGroup[%s]isInValid", QuoteActionField.ProductGroup.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QuoteActionField.ExchangeID[%s]isInValid", QuoteActionField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QuoteActionField.InstrumentID[%s]isInValid", QuoteActionField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.Cost.isValid())
	{
		PRINT_TO_STD("QuoteActionField.Cost[%s]isInValid", QuoteActionField.Cost.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.Remark.isValid())
	{
		PRINT_TO_STD("QuoteActionField.Remark[%s]isInValid", QuoteActionField.Remark.getString().c_str());
		return -4;
	}
	if (! QuoteActionField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("QuoteActionField.TradeUnitID[%s]isInValid", QuoteActionField.TradeUnitID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QuoteActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQuoteAction;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QuoteActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///订阅主题请求
int CApiImpl::SendTopicAction(MegaTopicActionStruct *pTopicAction, int nRequestID)
{
	CTopicActionField TopicActionField;
	memcpy(&TopicActionField, pTopicAction, sizeof(CTopicActionField));
	if (! TopicActionField.Action.isValid())
	{
		PRINT_TO_STD("TopicActionField.Action[%s]isInValid", TopicActionField.Action.getString().c_str());
		return -4;
	}
	if (! TopicActionField.TopicID.isValid())
	{
		PRINT_TO_STD("TopicActionField.TopicID[%s]isInValid", TopicActionField.TopicID.getString().c_str());
		return -4;
	}
	if (! TopicActionField.Index.isValid())
	{
		PRINT_TO_STD("TopicActionField.Index[%s]isInValid", TopicActionField.Index.getString().c_str());
		return -4;
	}
	if (! TopicActionField.ResumeNo.isValid())
	{
		PRINT_TO_STD("TopicActionField.ResumeNo[%s]isInValid", TopicActionField.ResumeNo.getString().c_str());
		return -4;
	}
	if (! TopicActionField.BusinessNo.isValid())
	{
		PRINT_TO_STD("TopicActionField.BusinessNo[%s]isInValid", TopicActionField.BusinessNo.getString().c_str());
		return -4;
	}
	if (! TopicActionField.Token.isValid())
	{
		PRINT_TO_STD("TopicActionField.Token[%s]isInValid", TopicActionField.Token.getString().c_str());
		return -4;
	}
	if (! TopicActionField.Zip.isValid())
	{
		PRINT_TO_STD("TopicActionField.Zip[%s]isInValid", TopicActionField.Zip.getString().c_str());
		return -4;
	}
	if (! TopicActionField.Frequency.isValid())
	{
		PRINT_TO_STD("TopicActionField.Frequency[%s]isInValid", TopicActionField.Frequency.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(TopicActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendTopicAction;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &TopicActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///设置行情请求
int CApiImpl::SendMarketDataUpdate(MegaMarketDataUpdateStruct *pMarketDataUpdate, int nRequestID)
{
	CMarketDataUpdateField MarketDataUpdateField;
	memcpy(&MarketDataUpdateField, pMarketDataUpdate, sizeof(CMarketDataUpdateField));
	if (! MarketDataUpdateField.ExchangeID.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.ExchangeID[%s]isInValid", MarketDataUpdateField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.InstrumentID.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.InstrumentID[%s]isInValid", MarketDataUpdateField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.MarkedPrice.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.MarkedPrice[%s]isInValid", MarketDataUpdateField.MarkedPrice.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.TheoryPrice.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.TheoryPrice[%s]isInValid", MarketDataUpdateField.TheoryPrice.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.PreSettlementPrice.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.PreSettlementPrice[%s]isInValid", MarketDataUpdateField.PreSettlementPrice.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.SettlementPrice.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.SettlementPrice[%s]isInValid", MarketDataUpdateField.SettlementPrice.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.UnderlyingPrice.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.UnderlyingPrice[%s]isInValid", MarketDataUpdateField.UnderlyingPrice.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.PrePositionFeeRate.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.PrePositionFeeRate[%s]isInValid", MarketDataUpdateField.PrePositionFeeRate.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.PositionFeeRate.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.PositionFeeRate[%s]isInValid", MarketDataUpdateField.PositionFeeRate.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.InstrumentStatus.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.InstrumentStatus[%s]isInValid", MarketDataUpdateField.InstrumentStatus.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.Volume.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.Volume[%s]isInValid", MarketDataUpdateField.Volume.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.Turnover.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.Turnover[%s]isInValid", MarketDataUpdateField.Turnover.getString().c_str());
		return -4;
	}
	if (! MarketDataUpdateField.Remark.isValid())
	{
		PRINT_TO_STD("MarketDataUpdateField.Remark[%s]isInValid", MarketDataUpdateField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(MarketDataUpdateField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendMarketDataUpdate;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &MarketDataUpdateField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///调整持仓请求
int CApiImpl::SendPositionAction(MegaPositionActionStruct *pPositionAction, int nRequestID)
{
	CPositionActionField PositionActionField;
	memcpy(&PositionActionField, pPositionAction, sizeof(CPositionActionField));
	if (! PositionActionField.LocalID.isValid())
	{
		PRINT_TO_STD("PositionActionField.LocalID[%s]isInValid", PositionActionField.LocalID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.PositionID.isValid())
	{
		PRINT_TO_STD("PositionActionField.PositionID[%s]isInValid", PositionActionField.PositionID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.ExchangeID.isValid())
	{
		PRINT_TO_STD("PositionActionField.ExchangeID[%s]isInValid", PositionActionField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.InstrumentID.isValid())
	{
		PRINT_TO_STD("PositionActionField.InstrumentID[%s]isInValid", PositionActionField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.ProductGroup.isValid())
	{
		PRINT_TO_STD("PositionActionField.ProductGroup[%s]isInValid", PositionActionField.ProductGroup.getString().c_str());
		return -4;
	}
	if (! PositionActionField.MemberID.isValid())
	{
		PRINT_TO_STD("PositionActionField.MemberID[%s]isInValid", PositionActionField.MemberID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("PositionActionField.TradeUnitID[%s]isInValid", PositionActionField.TradeUnitID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.AccountID.isValid())
	{
		PRINT_TO_STD("PositionActionField.AccountID[%s]isInValid", PositionActionField.AccountID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.PosiDirection.isValid())
	{
		PRINT_TO_STD("PositionActionField.PosiDirection[%s]isInValid", PositionActionField.PosiDirection.getString().c_str());
		return -4;
	}
	if (! PositionActionField.Amount.isValid())
	{
		PRINT_TO_STD("PositionActionField.Amount[%s]isInValid", PositionActionField.Amount.getString().c_str());
		return -4;
	}
	if (! PositionActionField.LongLeverage.isValid())
	{
		PRINT_TO_STD("PositionActionField.LongLeverage[%s]isInValid", PositionActionField.LongLeverage.getString().c_str());
		return -4;
	}
	if (! PositionActionField.ShortLeverage.isValid())
	{
		PRINT_TO_STD("PositionActionField.ShortLeverage[%s]isInValid", PositionActionField.ShortLeverage.getString().c_str());
		return -4;
	}
	if (! PositionActionField.NetLeverage.isValid())
	{
		PRINT_TO_STD("PositionActionField.NetLeverage[%s]isInValid", PositionActionField.NetLeverage.getString().c_str());
		return -4;
	}
	if (! PositionActionField.ActionType.isValid())
	{
		PRINT_TO_STD("PositionActionField.ActionType[%s]isInValid", PositionActionField.ActionType.getString().c_str());
		return -4;
	}
	if (! PositionActionField.IsCrossMargin.isValid())
	{
		PRINT_TO_STD("PositionActionField.IsCrossMargin[%s]isInValid", PositionActionField.IsCrossMargin.getString().c_str());
		return -4;
	}
	if (! PositionActionField.IsAutoAddMargin.isValid())
	{
		PRINT_TO_STD("PositionActionField.IsAutoAddMargin[%s]isInValid", PositionActionField.IsAutoAddMargin.getString().c_str());
		return -4;
	}
	if (! PositionActionField.Remark.isValid())
	{
		PRINT_TO_STD("PositionActionField.Remark[%s]isInValid", PositionActionField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(PositionActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendPositionAction;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &PositionActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///用户出入金请求
int CApiImpl::SendAccountAction(MegaAccountActionStruct *pAccountAction, int nRequestID)
{
	CAccountActionField AccountActionField;
	memcpy(&AccountActionField, pAccountAction, sizeof(CAccountActionField));
	if (! AccountActionField.ActionLocalID.isValid())
	{
		PRINT_TO_STD("AccountActionField.ActionLocalID[%s]isInValid", AccountActionField.ActionLocalID.getString().c_str());
		return -4;
	}
	if (! AccountActionField.LocalID.isValid())
	{
		PRINT_TO_STD("AccountActionField.LocalID[%s]isInValid", AccountActionField.LocalID.getString().c_str());
		return -4;
	}
	if (! AccountActionField.OrderID.isValid())
	{
		PRINT_TO_STD("AccountActionField.OrderID[%s]isInValid", AccountActionField.OrderID.getString().c_str());
		return -4;
	}
	if (! AccountActionField.MemberID.isValid())
	{
		PRINT_TO_STD("AccountActionField.MemberID[%s]isInValid", AccountActionField.MemberID.getString().c_str());
		return -4;
	}
	if (! AccountActionField.AccountID.isValid())
	{
		PRINT_TO_STD("AccountActionField.AccountID[%s]isInValid", AccountActionField.AccountID.getString().c_str());
		return -4;
	}
	if (! AccountActionField.SettlementGroup.isValid())
	{
		PRINT_TO_STD("AccountActionField.SettlementGroup[%s]isInValid", AccountActionField.SettlementGroup.getString().c_str());
		return -4;
	}
	if (! AccountActionField.OtherAccountID.isValid())
	{
		PRINT_TO_STD("AccountActionField.OtherAccountID[%s]isInValid", AccountActionField.OtherAccountID.getString().c_str());
		return -4;
	}
	if (! AccountActionField.OtherSettlementGroup.isValid())
	{
		PRINT_TO_STD("AccountActionField.OtherSettlementGroup[%s]isInValid", AccountActionField.OtherSettlementGroup.getString().c_str());
		return -4;
	}
	if (! AccountActionField.OtherMemberID.isValid())
	{
		PRINT_TO_STD("AccountActionField.OtherMemberID[%s]isInValid", AccountActionField.OtherMemberID.getString().c_str());
		return -4;
	}
	if (! AccountActionField.Amount.isValid())
	{
		PRINT_TO_STD("AccountActionField.Amount[%s]isInValid", AccountActionField.Amount.getString().c_str());
		return -4;
	}
	if (! AccountActionField.FrozenMoney.isValid())
	{
		PRINT_TO_STD("AccountActionField.FrozenMoney[%s]isInValid", AccountActionField.FrozenMoney.getString().c_str());
		return -4;
	}
	if (! AccountActionField.Currency.isValid())
	{
		PRINT_TO_STD("AccountActionField.Currency[%s]isInValid", AccountActionField.Currency.getString().c_str());
		return -4;
	}
	if (! AccountActionField.ActionType.isValid())
	{
		PRINT_TO_STD("AccountActionField.ActionType[%s]isInValid", AccountActionField.ActionType.getString().c_str());
		return -4;
	}
	if (! AccountActionField.IsReserve.isValid())
	{
		PRINT_TO_STD("AccountActionField.IsReserve[%s]isInValid", AccountActionField.IsReserve.getString().c_str());
		return -4;
	}
	if (! AccountActionField.AccountType.isValid())
	{
		PRINT_TO_STD("AccountActionField.AccountType[%s]isInValid", AccountActionField.AccountType.getString().c_str());
		return -4;
	}
	if (! AccountActionField.MarginRateGrade.isValid())
	{
		PRINT_TO_STD("AccountActionField.MarginRateGrade[%s]isInValid", AccountActionField.MarginRateGrade.getString().c_str());
		return -4;
	}
	if (! AccountActionField.TradingRightGrade.isValid())
	{
		PRINT_TO_STD("AccountActionField.TradingRightGrade[%s]isInValid", AccountActionField.TradingRightGrade.getString().c_str());
		return -4;
	}
	if (! AccountActionField.Price.isValid())
	{
		PRINT_TO_STD("AccountActionField.Price[%s]isInValid", AccountActionField.Price.getString().c_str());
		return -4;
	}
	if (! AccountActionField.Volume.isValid())
	{
		PRINT_TO_STD("AccountActionField.Volume[%s]isInValid", AccountActionField.Volume.getString().c_str());
		return -4;
	}
	if (! AccountActionField.ExchangeID.isValid())
	{
		PRINT_TO_STD("AccountActionField.ExchangeID[%s]isInValid", AccountActionField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! AccountActionField.InstrumentID.isValid())
	{
		PRINT_TO_STD("AccountActionField.InstrumentID[%s]isInValid", AccountActionField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! AccountActionField.Source.isValid())
	{
		PRINT_TO_STD("AccountActionField.Source[%s]isInValid", AccountActionField.Source.getString().c_str());
		return -4;
	}
	if (! AccountActionField.RelatedID.isValid())
	{
		PRINT_TO_STD("AccountActionField.RelatedID[%s]isInValid", AccountActionField.RelatedID.getString().c_str());
		return -4;
	}
	if (! AccountActionField.FeeDeduceRate.isValid())
	{
		PRINT_TO_STD("AccountActionField.FeeDeduceRate[%s]isInValid", AccountActionField.FeeDeduceRate.getString().c_str());
		return -4;
	}
	if (! AccountActionField.Remark.isValid())
	{
		PRINT_TO_STD("AccountActionField.Remark[%s]isInValid", AccountActionField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(AccountActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendAccountAction;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &AccountActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///成员设置请求
int CApiImpl::SendMemberAction(MegaMemberActionStruct *pMemberAction, int nRequestID)
{
	CMemberActionField MemberActionField;
	memcpy(&MemberActionField, pMemberAction, sizeof(CMemberActionField));
	if (! MemberActionField.LocalID.isValid())
	{
		PRINT_TO_STD("MemberActionField.LocalID[%s]isInValid", MemberActionField.LocalID.getString().c_str());
		return -4;
	}
	if (! MemberActionField.MemberID.isValid())
	{
		PRINT_TO_STD("MemberActionField.MemberID[%s]isInValid", MemberActionField.MemberID.getString().c_str());
		return -4;
	}
	if (! MemberActionField.RiskLevel.isValid())
	{
		PRINT_TO_STD("MemberActionField.RiskLevel[%s]isInValid", MemberActionField.RiskLevel.getString().c_str());
		return -4;
	}
	if (! MemberActionField.PositionType.isValid())
	{
		PRINT_TO_STD("MemberActionField.PositionType[%s]isInValid", MemberActionField.PositionType.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(MemberActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendMemberAction;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &MemberActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///内部做市商申报请求
int CApiImpl::SendRobotOrder(MegaRobotOrderStruct *pRobotOrder, int nRequestID)
{
	CRobotOrderField RobotOrderField;
	memcpy(&RobotOrderField, pRobotOrder, sizeof(CRobotOrderField));
	if (! RobotOrderField.MemberID.isValid())
	{
		PRINT_TO_STD("RobotOrderField.MemberID[%s]isInValid", RobotOrderField.MemberID.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("RobotOrderField.TradeUnitID[%s]isInValid", RobotOrderField.TradeUnitID.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.AccountID.isValid())
	{
		PRINT_TO_STD("RobotOrderField.AccountID[%s]isInValid", RobotOrderField.AccountID.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.UserID.isValid())
	{
		PRINT_TO_STD("RobotOrderField.UserID[%s]isInValid", RobotOrderField.UserID.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.LocalID.isValid())
	{
		PRINT_TO_STD("RobotOrderField.LocalID[%s]isInValid", RobotOrderField.LocalID.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.ExchangeID.isValid())
	{
		PRINT_TO_STD("RobotOrderField.ExchangeID[%s]isInValid", RobotOrderField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.InstrumentID.isValid())
	{
		PRINT_TO_STD("RobotOrderField.InstrumentID[%s]isInValid", RobotOrderField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.OrderPriceType.isValid())
	{
		PRINT_TO_STD("RobotOrderField.OrderPriceType[%s]isInValid", RobotOrderField.OrderPriceType.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.Direction.isValid())
	{
		PRINT_TO_STD("RobotOrderField.Direction[%s]isInValid", RobotOrderField.Direction.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.OffsetFlag.isValid())
	{
		PRINT_TO_STD("RobotOrderField.OffsetFlag[%s]isInValid", RobotOrderField.OffsetFlag.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.Price.isValid())
	{
		PRINT_TO_STD("RobotOrderField.Price[%s]isInValid", RobotOrderField.Price.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.Volume.isValid())
	{
		PRINT_TO_STD("RobotOrderField.Volume[%s]isInValid", RobotOrderField.Volume.getString().c_str());
		return -4;
	}
	if (! RobotOrderField.VolumeDisplay.isValid())
	{
		PRINT_TO_STD("RobotOrderField.VolumeDisplay[%s]isInValid", RobotOrderField.VolumeDisplay.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(RobotOrderField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendRobotOrder;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &RobotOrderField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///场外成交申报请求
int CApiImpl::SendOTCTradeInsert(MegaOTCTradeInsertStruct *pOTCTradeInsert, int nRequestID)
{
	COTCTradeInsertField OTCTradeInsertField;
	memcpy(&OTCTradeInsertField, pOTCTradeInsert, sizeof(COTCTradeInsertField));
	if (! OTCTradeInsertField.LocalID.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.LocalID[%s]isInValid", OTCTradeInsertField.LocalID.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.ExchangeID.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.ExchangeID[%s]isInValid", OTCTradeInsertField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.InstrumentID.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.InstrumentID[%s]isInValid", OTCTradeInsertField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.Price.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.Price[%s]isInValid", OTCTradeInsertField.Price.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.Volume.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.Volume[%s]isInValid", OTCTradeInsertField.Volume.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.BuyMemberID.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.BuyMemberID[%s]isInValid", OTCTradeInsertField.BuyMemberID.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.BuyAccountID.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.BuyAccountID[%s]isInValid", OTCTradeInsertField.BuyAccountID.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.BuyTradeUnitID.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.BuyTradeUnitID[%s]isInValid", OTCTradeInsertField.BuyTradeUnitID.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.BuyOffsetFlag.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.BuyOffsetFlag[%s]isInValid", OTCTradeInsertField.BuyOffsetFlag.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.SellMemberID.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.SellMemberID[%s]isInValid", OTCTradeInsertField.SellMemberID.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.SellAccountID.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.SellAccountID[%s]isInValid", OTCTradeInsertField.SellAccountID.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.SellTradeUnitID.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.SellTradeUnitID[%s]isInValid", OTCTradeInsertField.SellTradeUnitID.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.SellOffsetFlag.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.SellOffsetFlag[%s]isInValid", OTCTradeInsertField.SellOffsetFlag.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.OTCType.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.OTCType[%s]isInValid", OTCTradeInsertField.OTCType.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.TakerDirection.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.TakerDirection[%s]isInValid", OTCTradeInsertField.TakerDirection.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.TriggerPrice.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.TriggerPrice[%s]isInValid", OTCTradeInsertField.TriggerPrice.getString().c_str());
		return -4;
	}
	if (! OTCTradeInsertField.Remark.isValid())
	{
		PRINT_TO_STD("OTCTradeInsertField.Remark[%s]isInValid", OTCTradeInsertField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(OTCTradeInsertField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendOTCTradeInsert;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &OTCTradeInsertField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///资金结算请求
int CApiImpl::SendSettlement(MegaSettlementStruct *pSettlement, int nRequestID)
{
	CSettlementField SettlementField;
	memcpy(&SettlementField, pSettlement, sizeof(CSettlementField));
	if (! SettlementField.APPID.isValid())
	{
		PRINT_TO_STD("SettlementField.APPID[%s]isInValid", SettlementField.APPID.getString().c_str());
		return -4;
	}
	if (! SettlementField.LocalID.isValid())
	{
		PRINT_TO_STD("SettlementField.LocalID[%s]isInValid", SettlementField.LocalID.getString().c_str());
		return -4;
	}
	if (! SettlementField.ExchangeID.isValid())
	{
		PRINT_TO_STD("SettlementField.ExchangeID[%s]isInValid", SettlementField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! SettlementField.InstrumentID.isValid())
	{
		PRINT_TO_STD("SettlementField.InstrumentID[%s]isInValid", SettlementField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! SettlementField.SettlementGroup.isValid())
	{
		PRINT_TO_STD("SettlementField.SettlementGroup[%s]isInValid", SettlementField.SettlementGroup.getString().c_str());
		return -4;
	}
	if (! SettlementField.SettleAction.isValid())
	{
		PRINT_TO_STD("SettlementField.SettleAction[%s]isInValid", SettlementField.SettleAction.getString().c_str());
		return -4;
	}
	if (! SettlementField.Value.isValid())
	{
		PRINT_TO_STD("SettlementField.Value[%s]isInValid", SettlementField.Value.getString().c_str());
		return -4;
	}
	if (! SettlementField.Value1.isValid())
	{
		PRINT_TO_STD("SettlementField.Value1[%s]isInValid", SettlementField.Value1.getString().c_str());
		return -4;
	}
	if (! SettlementField.Value2.isValid())
	{
		PRINT_TO_STD("SettlementField.Value2[%s]isInValid", SettlementField.Value2.getString().c_str());
		return -4;
	}
	if (! SettlementField.Value3.isValid())
	{
		PRINT_TO_STD("SettlementField.Value3[%s]isInValid", SettlementField.Value3.getString().c_str());
		return -4;
	}
	if (! SettlementField.Remark.isValid())
	{
		PRINT_TO_STD("SettlementField.Remark[%s]isInValid", SettlementField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(SettlementField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendSettlement;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &SettlementField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///清理行情数据请求
int CApiImpl::SendClearMarket(MegaClearMarketStruct *pClearMarket, int nRequestID)
{
	CClearMarketField ClearMarketField;
	memcpy(&ClearMarketField, pClearMarket, sizeof(CClearMarketField));
	if (! ClearMarketField.ExchangeID.isValid())
	{
		PRINT_TO_STD("ClearMarketField.ExchangeID[%s]isInValid", ClearMarketField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! ClearMarketField.InstrumentID.isValid())
	{
		PRINT_TO_STD("ClearMarketField.InstrumentID[%s]isInValid", ClearMarketField.InstrumentID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(ClearMarketField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendClearMarket;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &ClearMarketField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///清理最新K线请求
int CApiImpl::SendClearLastKLine(MegaClearLastKLineStruct *pClearLastKLine, int nRequestID)
{
	CClearLastKLineField ClearLastKLineField;
	memcpy(&ClearLastKLineField, pClearLastKLine, sizeof(CClearLastKLineField));
	if (! ClearLastKLineField.ExchangeID.isValid())
	{
		PRINT_TO_STD("ClearLastKLineField.ExchangeID[%s]isInValid", ClearLastKLineField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! ClearLastKLineField.InstrumentID.isValid())
	{
		PRINT_TO_STD("ClearLastKLineField.InstrumentID[%s]isInValid", ClearLastKLineField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! ClearLastKLineField.Bar.isValid())
	{
		PRINT_TO_STD("ClearLastKLineField.Bar[%s]isInValid", ClearLastKLineField.Bar.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(ClearLastKLineField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendClearLastKLine;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &ClearLastKLineField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///重置LocalID请求
int CApiImpl::SendResetLocalID(MegaResetLocalIDStruct *pResetLocalID, int nRequestID)
{
	CResetLocalIDField ResetLocalIDField;
	memcpy(&ResetLocalIDField, pResetLocalID, sizeof(CResetLocalIDField));
	if (! ResetLocalIDField.Token.isValid())
	{
		PRINT_TO_STD("ResetLocalIDField.Token[%s]isInValid", ResetLocalIDField.Token.getString().c_str());
		return -4;
	}
	if (! ResetLocalIDField.UserID.isValid())
	{
		PRINT_TO_STD("ResetLocalIDField.UserID[%s]isInValid", ResetLocalIDField.UserID.getString().c_str());
		return -4;
	}
	if (! ResetLocalIDField.APPID.isValid())
	{
		PRINT_TO_STD("ResetLocalIDField.APPID[%s]isInValid", ResetLocalIDField.APPID.getString().c_str());
		return -4;
	}
	if (! ResetLocalIDField.MemberID.isValid())
	{
		PRINT_TO_STD("ResetLocalIDField.MemberID[%s]isInValid", ResetLocalIDField.MemberID.getString().c_str());
		return -4;
	}
	if (! ResetLocalIDField.SettlementGroup.isValid())
	{
		PRINT_TO_STD("ResetLocalIDField.SettlementGroup[%s]isInValid", ResetLocalIDField.SettlementGroup.getString().c_str());
		return -4;
	}
	if (! ResetLocalIDField.AccountID.isValid())
	{
		PRINT_TO_STD("ResetLocalIDField.AccountID[%s]isInValid", ResetLocalIDField.AccountID.getString().c_str());
		return -4;
	}
	if (! ResetLocalIDField.Currency.isValid())
	{
		PRINT_TO_STD("ResetLocalIDField.Currency[%s]isInValid", ResetLocalIDField.Currency.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(ResetLocalIDField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendResetLocalID;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &ResetLocalIDField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///OToken录入请求
int CApiImpl::SendOTokenInsert(MegaOTokenInsertStruct *pOTokenInsert, int nRequestID)
{
	COTokenInsertField OTokenInsertField;
	memcpy(&OTokenInsertField, pOTokenInsert, sizeof(COTokenInsertField));
	if (! OTokenInsertField.UserID.isValid())
	{
		PRINT_TO_STD("OTokenInsertField.UserID[%s]isInValid", OTokenInsertField.UserID.getString().c_str());
		return -4;
	}
	if (! OTokenInsertField.APPID.isValid())
	{
		PRINT_TO_STD("OTokenInsertField.APPID[%s]isInValid", OTokenInsertField.APPID.getString().c_str());
		return -4;
	}
	if (! OTokenInsertField.Token.isValid())
	{
		PRINT_TO_STD("OTokenInsertField.Token[%s]isInValid", OTokenInsertField.Token.getString().c_str());
		return -4;
	}
	if (! OTokenInsertField.MemberID.isValid())
	{
		PRINT_TO_STD("OTokenInsertField.MemberID[%s]isInValid", OTokenInsertField.MemberID.getString().c_str());
		return -4;
	}
	if (! OTokenInsertField.LoginTime.isValid())
	{
		PRINT_TO_STD("OTokenInsertField.LoginTime[%s]isInValid", OTokenInsertField.LoginTime.getString().c_str());
		return -4;
	}
	if (! OTokenInsertField.ExpireTime.isValid())
	{
		PRINT_TO_STD("OTokenInsertField.ExpireTime[%s]isInValid", OTokenInsertField.ExpireTime.getString().c_str());
		return -4;
	}
	if (! OTokenInsertField.AccessLimit.isValid())
	{
		PRINT_TO_STD("OTokenInsertField.AccessLimit[%s]isInValid", OTokenInsertField.AccessLimit.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(OTokenInsertField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendOTokenInsert;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &OTokenInsertField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///OToken操作请求
int CApiImpl::SendOTokenAction(MegaOTokenActionStruct *pOTokenAction, int nRequestID)
{
	COTokenActionField OTokenActionField;
	memcpy(&OTokenActionField, pOTokenAction, sizeof(COTokenActionField));
	if (! OTokenActionField.UserID.isValid())
	{
		PRINT_TO_STD("OTokenActionField.UserID[%s]isInValid", OTokenActionField.UserID.getString().c_str());
		return -4;
	}
	if (! OTokenActionField.APPID.isValid())
	{
		PRINT_TO_STD("OTokenActionField.APPID[%s]isInValid", OTokenActionField.APPID.getString().c_str());
		return -4;
	}
	if (! OTokenActionField.ActionFlag.isValid())
	{
		PRINT_TO_STD("OTokenActionField.ActionFlag[%s]isInValid", OTokenActionField.ActionFlag.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(OTokenActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendOTokenAction;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &OTokenActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///APIKEY录入请求
int CApiImpl::SendAPIKeyInsert(MegaAPIKeyInsertStruct *pAPIKeyInsert, int nRequestID)
{
	CAPIKeyInsertField APIKeyInsertField;
	memcpy(&APIKeyInsertField, pAPIKeyInsert, sizeof(CAPIKeyInsertField));
	if (! APIKeyInsertField.Name.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.Name[%s]isInValid", APIKeyInsertField.Name.getString().c_str());
		return -4;
	}
	if (! APIKeyInsertField.UserID.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.UserID[%s]isInValid", APIKeyInsertField.UserID.getString().c_str());
		return -4;
	}
	if (! APIKeyInsertField.APPID.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.APPID[%s]isInValid", APIKeyInsertField.APPID.getString().c_str());
		return -4;
	}
	if (! APIKeyInsertField.MemberID.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.MemberID[%s]isInValid", APIKeyInsertField.MemberID.getString().c_str());
		return -4;
	}
	if (! APIKeyInsertField.AccessKey.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.AccessKey[%s]isInValid", APIKeyInsertField.AccessKey.getString().c_str());
		return -4;
	}
	if (! APIKeyInsertField.SecretKey.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.SecretKey[%s]isInValid", APIKeyInsertField.SecretKey.getString().c_str());
		return -4;
	}
	if (! APIKeyInsertField.Auth.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.Auth[%s]isInValid", APIKeyInsertField.Auth.getString().c_str());
		return -4;
	}
	if (! APIKeyInsertField.AccessLimit.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.AccessLimit[%s]isInValid", APIKeyInsertField.AccessLimit.getString().c_str());
		return -4;
	}
	if (! APIKeyInsertField.CreateTime.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.CreateTime[%s]isInValid", APIKeyInsertField.CreateTime.getString().c_str());
		return -4;
	}
	if (! APIKeyInsertField.ExpireTime.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.ExpireTime[%s]isInValid", APIKeyInsertField.ExpireTime.getString().c_str());
		return -4;
	}
	if (! APIKeyInsertField.IPAddress.isValid())
	{
		PRINT_TO_STD("APIKeyInsertField.IPAddress[%s]isInValid", APIKeyInsertField.IPAddress.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(APIKeyInsertField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendAPIKeyInsert;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &APIKeyInsertField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///APIKEY操作请求
int CApiImpl::SendAPIKeyAction(MegaAPIKeyActionStruct *pAPIKeyAction, int nRequestID)
{
	CAPIKeyActionField APIKeyActionField;
	memcpy(&APIKeyActionField, pAPIKeyAction, sizeof(CAPIKeyActionField));
	if (! APIKeyActionField.Name.isValid())
	{
		PRINT_TO_STD("APIKeyActionField.Name[%s]isInValid", APIKeyActionField.Name.getString().c_str());
		return -4;
	}
	if (! APIKeyActionField.AccessKey.isValid())
	{
		PRINT_TO_STD("APIKeyActionField.AccessKey[%s]isInValid", APIKeyActionField.AccessKey.getString().c_str());
		return -4;
	}
	if (! APIKeyActionField.Auth.isValid())
	{
		PRINT_TO_STD("APIKeyActionField.Auth[%s]isInValid", APIKeyActionField.Auth.getString().c_str());
		return -4;
	}
	if (! APIKeyActionField.AccessLimit.isValid())
	{
		PRINT_TO_STD("APIKeyActionField.AccessLimit[%s]isInValid", APIKeyActionField.AccessLimit.getString().c_str());
		return -4;
	}
	if (! APIKeyActionField.IPAddress.isValid())
	{
		PRINT_TO_STD("APIKeyActionField.IPAddress[%s]isInValid", APIKeyActionField.IPAddress.getString().c_str());
		return -4;
	}
	if (! APIKeyActionField.ExpireTime.isValid())
	{
		PRINT_TO_STD("APIKeyActionField.ExpireTime[%s]isInValid", APIKeyActionField.ExpireTime.getString().c_str());
		return -4;
	}
	if (! APIKeyActionField.ActionFlag.isValid())
	{
		PRINT_TO_STD("APIKeyActionField.ActionFlag[%s]isInValid", APIKeyActionField.ActionFlag.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(APIKeyActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendAPIKeyAction;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &APIKeyActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///仓位合并操作请求
int CApiImpl::SendPositionMerge(MegaPositionMergeStruct *pPositionMerge, int nRequestID)
{
	CPositionMergeField PositionMergeField;
	memcpy(&PositionMergeField, pPositionMerge, sizeof(CPositionMergeField));
	if (! PositionMergeField.LocalID.isValid())
	{
		PRINT_TO_STD("PositionMergeField.LocalID[%s]isInValid", PositionMergeField.LocalID.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.MemberID.isValid())
	{
		PRINT_TO_STD("PositionMergeField.MemberID[%s]isInValid", PositionMergeField.MemberID.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID[%s]isInValid", PositionMergeField.PositionID.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID1.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID1[%s]isInValid", PositionMergeField.PositionID1.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID2.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID2[%s]isInValid", PositionMergeField.PositionID2.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID3.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID3[%s]isInValid", PositionMergeField.PositionID3.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID4.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID4[%s]isInValid", PositionMergeField.PositionID4.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID5.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID5[%s]isInValid", PositionMergeField.PositionID5.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID6.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID6[%s]isInValid", PositionMergeField.PositionID6.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID7.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID7[%s]isInValid", PositionMergeField.PositionID7.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID8.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID8[%s]isInValid", PositionMergeField.PositionID8.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID9.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID9[%s]isInValid", PositionMergeField.PositionID9.getString().c_str());
		return -4;
	}
	if (! PositionMergeField.PositionID10.isValid())
	{
		PRINT_TO_STD("PositionMergeField.PositionID10[%s]isInValid", PositionMergeField.PositionID10.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(PositionMergeField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendPositionMerge;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &PositionMergeField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///用户入金操作请求
int CApiImpl::SendDeposit(MegaDepositStruct *pDeposit, int nRequestID)
{
	CDepositField DepositField;
	memcpy(&DepositField, pDeposit, sizeof(CDepositField));
	if (! DepositField.LocalID.isValid())
	{
		PRINT_TO_STD("DepositField.LocalID[%s]isInValid", DepositField.LocalID.getString().c_str());
		return -4;
	}
	if (! DepositField.MemberID.isValid())
	{
		PRINT_TO_STD("DepositField.MemberID[%s]isInValid", DepositField.MemberID.getString().c_str());
		return -4;
	}
	if (! DepositField.AccountID.isValid())
	{
		PRINT_TO_STD("DepositField.AccountID[%s]isInValid", DepositField.AccountID.getString().c_str());
		return -4;
	}
	if (! DepositField.SettlementGroup.isValid())
	{
		PRINT_TO_STD("DepositField.SettlementGroup[%s]isInValid", DepositField.SettlementGroup.getString().c_str());
		return -4;
	}
	if (! DepositField.Amount.isValid())
	{
		PRINT_TO_STD("DepositField.Amount[%s]isInValid", DepositField.Amount.getString().c_str());
		return -4;
	}
	if (! DepositField.Currency.isValid())
	{
		PRINT_TO_STD("DepositField.Currency[%s]isInValid", DepositField.Currency.getString().c_str());
		return -4;
	}
	if (! DepositField.Remark.isValid())
	{
		PRINT_TO_STD("DepositField.Remark[%s]isInValid", DepositField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(DepositField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendDeposit;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &DepositField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///用户出金操作请求
int CApiImpl::SendWithdraw(MegaWithdrawStruct *pWithdraw, int nRequestID)
{
	CWithdrawField WithdrawField;
	memcpy(&WithdrawField, pWithdraw, sizeof(CWithdrawField));
	if (! WithdrawField.LocalID.isValid())
	{
		PRINT_TO_STD("WithdrawField.LocalID[%s]isInValid", WithdrawField.LocalID.getString().c_str());
		return -4;
	}
	if (! WithdrawField.OrderID.isValid())
	{
		PRINT_TO_STD("WithdrawField.OrderID[%s]isInValid", WithdrawField.OrderID.getString().c_str());
		return -4;
	}
	if (! WithdrawField.MemberID.isValid())
	{
		PRINT_TO_STD("WithdrawField.MemberID[%s]isInValid", WithdrawField.MemberID.getString().c_str());
		return -4;
	}
	if (! WithdrawField.AccountID.isValid())
	{
		PRINT_TO_STD("WithdrawField.AccountID[%s]isInValid", WithdrawField.AccountID.getString().c_str());
		return -4;
	}
	if (! WithdrawField.SettlementGroup.isValid())
	{
		PRINT_TO_STD("WithdrawField.SettlementGroup[%s]isInValid", WithdrawField.SettlementGroup.getString().c_str());
		return -4;
	}
	if (! WithdrawField.OtherSettlementGroup.isValid())
	{
		PRINT_TO_STD("WithdrawField.OtherSettlementGroup[%s]isInValid", WithdrawField.OtherSettlementGroup.getString().c_str());
		return -4;
	}
	if (! WithdrawField.Amount.isValid())
	{
		PRINT_TO_STD("WithdrawField.Amount[%s]isInValid", WithdrawField.Amount.getString().c_str());
		return -4;
	}
	if (! WithdrawField.Currency.isValid())
	{
		PRINT_TO_STD("WithdrawField.Currency[%s]isInValid", WithdrawField.Currency.getString().c_str());
		return -4;
	}
	if (! WithdrawField.Remark.isValid())
	{
		PRINT_TO_STD("WithdrawField.Remark[%s]isInValid", WithdrawField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(WithdrawField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendWithdraw;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &WithdrawField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///用户转账操作请求
int CApiImpl::SendTransfer(MegaTransferStruct *pTransfer, int nRequestID)
{
	CTransferField TransferField;
	memcpy(&TransferField, pTransfer, sizeof(CTransferField));
	if (! TransferField.LocalID.isValid())
	{
		PRINT_TO_STD("TransferField.LocalID[%s]isInValid", TransferField.LocalID.getString().c_str());
		return -4;
	}
	if (! TransferField.MemberID.isValid())
	{
		PRINT_TO_STD("TransferField.MemberID[%s]isInValid", TransferField.MemberID.getString().c_str());
		return -4;
	}
	if (! TransferField.AccountID.isValid())
	{
		PRINT_TO_STD("TransferField.AccountID[%s]isInValid", TransferField.AccountID.getString().c_str());
		return -4;
	}
	if (! TransferField.SettlementGroup.isValid())
	{
		PRINT_TO_STD("TransferField.SettlementGroup[%s]isInValid", TransferField.SettlementGroup.getString().c_str());
		return -4;
	}
	if (! TransferField.OtherAccountID.isValid())
	{
		PRINT_TO_STD("TransferField.OtherAccountID[%s]isInValid", TransferField.OtherAccountID.getString().c_str());
		return -4;
	}
	if (! TransferField.OtherSettlementGroup.isValid())
	{
		PRINT_TO_STD("TransferField.OtherSettlementGroup[%s]isInValid", TransferField.OtherSettlementGroup.getString().c_str());
		return -4;
	}
	if (! TransferField.OtherMemberID.isValid())
	{
		PRINT_TO_STD("TransferField.OtherMemberID[%s]isInValid", TransferField.OtherMemberID.getString().c_str());
		return -4;
	}
	if (! TransferField.Amount.isValid())
	{
		PRINT_TO_STD("TransferField.Amount[%s]isInValid", TransferField.Amount.getString().c_str());
		return -4;
	}
	if (! TransferField.Currency.isValid())
	{
		PRINT_TO_STD("TransferField.Currency[%s]isInValid", TransferField.Currency.getString().c_str());
		return -4;
	}
	if (! TransferField.Remark.isValid())
	{
		PRINT_TO_STD("TransferField.Remark[%s]isInValid", TransferField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(TransferField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendTransfer;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &TransferField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///注册用户请求
int CApiImpl::SendUserRegister(MegaUserSessionStruct *pUserSession, int nRequestID)
{
	CUserSessionField UserSessionField;
	memcpy(&UserSessionField, pUserSession, sizeof(CUserSessionField));
	if (! UserSessionField.UserID.isValid())
	{
		PRINT_TO_STD("UserSessionField.UserID[%s]isInValid", UserSessionField.UserID.getString().c_str());
		return -4;
	}
	if (! UserSessionField.Password.isValid())
	{
		PRINT_TO_STD("UserSessionField.Password[%s]isInValid", UserSessionField.Password.getString().c_str());
		return -4;
	}
	if (! UserSessionField.UserProductID.isValid())
	{
		PRINT_TO_STD("UserSessionField.UserProductID[%s]isInValid", UserSessionField.UserProductID.getString().c_str());
		return -4;
	}
	if (! UserSessionField.MacAddress.isValid())
	{
		PRINT_TO_STD("UserSessionField.MacAddress[%s]isInValid", UserSessionField.MacAddress.getString().c_str());
		return -4;
	}
	if (! UserSessionField.ClientIPAddress.isValid())
	{
		PRINT_TO_STD("UserSessionField.ClientIPAddress[%s]isInValid", UserSessionField.ClientIPAddress.getString().c_str());
		return -4;
	}
	if (! UserSessionField.HDSerialID.isValid())
	{
		PRINT_TO_STD("UserSessionField.HDSerialID[%s]isInValid", UserSessionField.HDSerialID.getString().c_str());
		return -4;
	}
	if (! UserSessionField.AuthCode.isValid())
	{
		PRINT_TO_STD("UserSessionField.AuthCode[%s]isInValid", UserSessionField.AuthCode.getString().c_str());
		return -4;
	}
	if (! UserSessionField.APPID.isValid())
	{
		PRINT_TO_STD("UserSessionField.APPID[%s]isInValid", UserSessionField.APPID.getString().c_str());
		return -4;
	}
	if (! UserSessionField.Token.isValid())
	{
		PRINT_TO_STD("UserSessionField.Token[%s]isInValid", UserSessionField.Token.getString().c_str());
		return -4;
	}
	if (! UserSessionField.Remark.isValid())
	{
		PRINT_TO_STD("UserSessionField.Remark[%s]isInValid", UserSessionField.Remark.getString().c_str());
		return -4;
	}
	if (! UserSessionField.APIID.isValid())
	{
		PRINT_TO_STD("UserSessionField.APIID[%s]isInValid", UserSessionField.APIID.getString().c_str());
		return -4;
	}
	if (! UserSessionField.LoginTime.isValid())
	{
		PRINT_TO_STD("UserSessionField.LoginTime[%s]isInValid", UserSessionField.LoginTime.getString().c_str());
		return -4;
	}
	if (! UserSessionField.ExpireTime.isValid())
	{
		PRINT_TO_STD("UserSessionField.ExpireTime[%s]isInValid", UserSessionField.ExpireTime.getString().c_str());
		return -4;
	}
	if (! UserSessionField.IPAddress.isValid())
	{
		PRINT_TO_STD("UserSessionField.IPAddress[%s]isInValid", UserSessionField.IPAddress.getString().c_str());
		return -4;
	}
	if (! UserSessionField.MaxLocalID.isValid())
	{
		PRINT_TO_STD("UserSessionField.MaxLocalID[%s]isInValid", UserSessionField.MaxLocalID.getString().c_str());
		return -4;
	}
	if (! UserSessionField.SessionNo.isValid())
	{
		PRINT_TO_STD("UserSessionField.SessionNo[%s]isInValid", UserSessionField.SessionNo.getString().c_str());
		return -4;
	}
	if (! UserSessionField.FrontNo.isValid())
	{
		PRINT_TO_STD("UserSessionField.FrontNo[%s]isInValid", UserSessionField.FrontNo.getString().c_str());
		return -4;
	}
	if (! UserSessionField.AccessLimit.isValid())
	{
		PRINT_TO_STD("UserSessionField.AccessLimit[%s]isInValid", UserSessionField.AccessLimit.getString().c_str());
		return -4;
	}
	if (! UserSessionField.UserType.isValid())
	{
		PRINT_TO_STD("UserSessionField.UserType[%s]isInValid", UserSessionField.UserType.getString().c_str());
		return -4;
	}
	if (! UserSessionField.MemberID.isValid())
	{
		PRINT_TO_STD("UserSessionField.MemberID[%s]isInValid", UserSessionField.MemberID.getString().c_str());
		return -4;
	}
	if (! UserSessionField.LimitAccesses.isValid())
	{
		PRINT_TO_STD("UserSessionField.LimitAccesses[%s]isInValid", UserSessionField.LimitAccesses.getString().c_str());
		return -4;
	}
	if (! UserSessionField.SinkType.isValid())
	{
		PRINT_TO_STD("UserSessionField.SinkType[%s]isInValid", UserSessionField.SinkType.getString().c_str());
		return -4;
	}
	if (! UserSessionField.InsertTime.isValid())
	{
		PRINT_TO_STD("UserSessionField.InsertTime[%s]isInValid", UserSessionField.InsertTime.getString().c_str());
		return -4;
	}
	if (! UserSessionField.UpdateTime.isValid())
	{
		PRINT_TO_STD("UserSessionField.UpdateTime[%s]isInValid", UserSessionField.UpdateTime.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(UserSessionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendUserRegister;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &UserSessionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///反向开仓录入请求
int CApiImpl::SendPositionReverse(MegaPositionReverseStruct *pPositionReverse, int nRequestID)
{
	CPositionReverseField PositionReverseField;
	memcpy(&PositionReverseField, pPositionReverse, sizeof(CPositionReverseField));
	if (! PositionReverseField.LocalID.isValid())
	{
		PRINT_TO_STD("PositionReverseField.LocalID[%s]isInValid", PositionReverseField.LocalID.getString().c_str());
		return -4;
	}
	if (! PositionReverseField.MemberID.isValid())
	{
		PRINT_TO_STD("PositionReverseField.MemberID[%s]isInValid", PositionReverseField.MemberID.getString().c_str());
		return -4;
	}
	if (! PositionReverseField.PositionID.isValid())
	{
		PRINT_TO_STD("PositionReverseField.PositionID[%s]isInValid", PositionReverseField.PositionID.getString().c_str());
		return -4;
	}
	if (! PositionReverseField.Volume.isValid())
	{
		PRINT_TO_STD("PositionReverseField.Volume[%s]isInValid", PositionReverseField.Volume.getString().c_str());
		return -4;
	}
	if (! PositionReverseField.Remark.isValid())
	{
		PRINT_TO_STD("PositionReverseField.Remark[%s]isInValid", PositionReverseField.Remark.getString().c_str());
		return -4;
	}
	if (! PositionReverseField.CopyMemberID.isValid())
	{
		PRINT_TO_STD("PositionReverseField.CopyMemberID[%s]isInValid", PositionReverseField.CopyMemberID.getString().c_str());
		return -4;
	}
	if (! PositionReverseField.APPID.isValid())
	{
		PRINT_TO_STD("PositionReverseField.APPID[%s]isInValid", PositionReverseField.APPID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(PositionReverseField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendPositionReverse;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &PositionReverseField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///避免自成交报单录入请求
int CApiImpl::SendCancelOrderInsert(MegaOrderInsertStruct *pOrderInsert, int nRequestID)
{
	COrderInsertField OrderInsertField;
	memcpy(&OrderInsertField, pOrderInsert, sizeof(COrderInsertField));
	if (! OrderInsertField.MemberID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.MemberID[%s]isInValid", OrderInsertField.MemberID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.TradeUnitID[%s]isInValid", OrderInsertField.TradeUnitID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.AccountID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.AccountID[%s]isInValid", OrderInsertField.AccountID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.LocalID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.LocalID[%s]isInValid", OrderInsertField.LocalID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.ExchangeID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.ExchangeID[%s]isInValid", OrderInsertField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.InstrumentID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.InstrumentID[%s]isInValid", OrderInsertField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.OrderPriceType.isValid())
	{
		PRINT_TO_STD("OrderInsertField.OrderPriceType[%s]isInValid", OrderInsertField.OrderPriceType.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Direction.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Direction[%s]isInValid", OrderInsertField.Direction.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.OffsetFlag.isValid())
	{
		PRINT_TO_STD("OrderInsertField.OffsetFlag[%s]isInValid", OrderInsertField.OffsetFlag.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Price.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Price[%s]isInValid", OrderInsertField.Price.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Volume.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Volume[%s]isInValid", OrderInsertField.Volume.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.VolumeDisplay.isValid())
	{
		PRINT_TO_STD("OrderInsertField.VolumeDisplay[%s]isInValid", OrderInsertField.VolumeDisplay.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.VolumeMode.isValid())
	{
		PRINT_TO_STD("OrderInsertField.VolumeMode[%s]isInValid", OrderInsertField.VolumeMode.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Cost.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Cost[%s]isInValid", OrderInsertField.Cost.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.OrderType.isValid())
	{
		PRINT_TO_STD("OrderInsertField.OrderType[%s]isInValid", OrderInsertField.OrderType.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.GTDTime.isValid())
	{
		PRINT_TO_STD("OrderInsertField.GTDTime[%s]isInValid", OrderInsertField.GTDTime.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.MinVolume.isValid())
	{
		PRINT_TO_STD("OrderInsertField.MinVolume[%s]isInValid", OrderInsertField.MinVolume.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.BusinessType.isValid())
	{
		PRINT_TO_STD("OrderInsertField.BusinessType[%s]isInValid", OrderInsertField.BusinessType.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.BusinessValue.isValid())
	{
		PRINT_TO_STD("OrderInsertField.BusinessValue[%s]isInValid", OrderInsertField.BusinessValue.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.CloseOrderID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.CloseOrderID[%s]isInValid", OrderInsertField.CloseOrderID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.IsCrossMargin.isValid())
	{
		PRINT_TO_STD("OrderInsertField.IsCrossMargin[%s]isInValid", OrderInsertField.IsCrossMargin.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Remark.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Remark[%s]isInValid", OrderInsertField.Remark.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.OrderID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.OrderID[%s]isInValid", OrderInsertField.OrderID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.CopyMemberID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.CopyMemberID[%s]isInValid", OrderInsertField.CopyMemberID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.CopyOrderID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.CopyOrderID[%s]isInValid", OrderInsertField.CopyOrderID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Leverage.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Leverage[%s]isInValid", OrderInsertField.Leverage.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.CopyProfitRate.isValid())
	{
		PRINT_TO_STD("OrderInsertField.CopyProfitRate[%s]isInValid", OrderInsertField.CopyProfitRate.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.APPID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.APPID[%s]isInValid", OrderInsertField.APPID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.PositionID.isValid())
	{
		PRINT_TO_STD("OrderInsertField.PositionID[%s]isInValid", OrderInsertField.PositionID.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.TriggerPrice.isValid())
	{
		PRINT_TO_STD("OrderInsertField.TriggerPrice[%s]isInValid", OrderInsertField.TriggerPrice.getString().c_str());
		return -4;
	}
	if (! OrderInsertField.Reserve.isValid())
	{
		PRINT_TO_STD("OrderInsertField.Reserve[%s]isInValid", OrderInsertField.Reserve.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(OrderInsertField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendCancelOrderInsert;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &OrderInsertField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///私有主题分类请求
int CApiImpl::SendPrivateDispatch(MegaPrivateDispatchStruct *pPrivateDispatch, int nRequestID)
{
	CPrivateDispatchField PrivateDispatchField;
	memcpy(&PrivateDispatchField, pPrivateDispatch, sizeof(CPrivateDispatchField));
	if (! PrivateDispatchField.TopicID.isValid())
	{
		PRINT_TO_STD("PrivateDispatchField.TopicID[%s]isInValid", PrivateDispatchField.TopicID.getString().c_str());
		return -4;
	}
	if (! PrivateDispatchField.MemberID.isValid())
	{
		PRINT_TO_STD("PrivateDispatchField.MemberID[%s]isInValid", PrivateDispatchField.MemberID.getString().c_str());
		return -4;
	}
	if (! PrivateDispatchField.PrivateBase.isValid())
	{
		PRINT_TO_STD("PrivateDispatchField.PrivateBase[%s]isInValid", PrivateDispatchField.PrivateBase.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(PrivateDispatchField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendPrivateDispatch;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &PrivateDispatchField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///调整多空杠杠请求
int CApiImpl::SendPositionLeverage(MegaPositionActionStruct *pPositionAction, int nRequestID)
{
	CPositionActionField PositionActionField;
	memcpy(&PositionActionField, pPositionAction, sizeof(CPositionActionField));
	if (! PositionActionField.LocalID.isValid())
	{
		PRINT_TO_STD("PositionActionField.LocalID[%s]isInValid", PositionActionField.LocalID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.PositionID.isValid())
	{
		PRINT_TO_STD("PositionActionField.PositionID[%s]isInValid", PositionActionField.PositionID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.ExchangeID.isValid())
	{
		PRINT_TO_STD("PositionActionField.ExchangeID[%s]isInValid", PositionActionField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.InstrumentID.isValid())
	{
		PRINT_TO_STD("PositionActionField.InstrumentID[%s]isInValid", PositionActionField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.ProductGroup.isValid())
	{
		PRINT_TO_STD("PositionActionField.ProductGroup[%s]isInValid", PositionActionField.ProductGroup.getString().c_str());
		return -4;
	}
	if (! PositionActionField.MemberID.isValid())
	{
		PRINT_TO_STD("PositionActionField.MemberID[%s]isInValid", PositionActionField.MemberID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("PositionActionField.TradeUnitID[%s]isInValid", PositionActionField.TradeUnitID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.AccountID.isValid())
	{
		PRINT_TO_STD("PositionActionField.AccountID[%s]isInValid", PositionActionField.AccountID.getString().c_str());
		return -4;
	}
	if (! PositionActionField.PosiDirection.isValid())
	{
		PRINT_TO_STD("PositionActionField.PosiDirection[%s]isInValid", PositionActionField.PosiDirection.getString().c_str());
		return -4;
	}
	if (! PositionActionField.Amount.isValid())
	{
		PRINT_TO_STD("PositionActionField.Amount[%s]isInValid", PositionActionField.Amount.getString().c_str());
		return -4;
	}
	if (! PositionActionField.LongLeverage.isValid())
	{
		PRINT_TO_STD("PositionActionField.LongLeverage[%s]isInValid", PositionActionField.LongLeverage.getString().c_str());
		return -4;
	}
	if (! PositionActionField.ShortLeverage.isValid())
	{
		PRINT_TO_STD("PositionActionField.ShortLeverage[%s]isInValid", PositionActionField.ShortLeverage.getString().c_str());
		return -4;
	}
	if (! PositionActionField.NetLeverage.isValid())
	{
		PRINT_TO_STD("PositionActionField.NetLeverage[%s]isInValid", PositionActionField.NetLeverage.getString().c_str());
		return -4;
	}
	if (! PositionActionField.ActionType.isValid())
	{
		PRINT_TO_STD("PositionActionField.ActionType[%s]isInValid", PositionActionField.ActionType.getString().c_str());
		return -4;
	}
	if (! PositionActionField.IsCrossMargin.isValid())
	{
		PRINT_TO_STD("PositionActionField.IsCrossMargin[%s]isInValid", PositionActionField.IsCrossMargin.getString().c_str());
		return -4;
	}
	if (! PositionActionField.IsAutoAddMargin.isValid())
	{
		PRINT_TO_STD("PositionActionField.IsAutoAddMargin[%s]isInValid", PositionActionField.IsAutoAddMargin.getString().c_str());
		return -4;
	}
	if (! PositionActionField.Remark.isValid())
	{
		PRINT_TO_STD("PositionActionField.Remark[%s]isInValid", PositionActionField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(PositionActionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendPositionLeverage;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &PositionActionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///设置合约请求
int CApiImpl::SendInstrumentUpdate(MegaInstrumentUpdateStruct *pInstrumentUpdate, int nRequestID)
{
	CInstrumentUpdateField InstrumentUpdateField;
	memcpy(&InstrumentUpdateField, pInstrumentUpdate, sizeof(CInstrumentUpdateField));
	if (! InstrumentUpdateField.ExchangeID.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.ExchangeID[%s]isInValid", InstrumentUpdateField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! InstrumentUpdateField.InstrumentID.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.InstrumentID[%s]isInValid", InstrumentUpdateField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! InstrumentUpdateField.MinOrderVolume.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.MinOrderVolume[%s]isInValid", InstrumentUpdateField.MinOrderVolume.getString().c_str());
		return -4;
	}
	if (! InstrumentUpdateField.MinOrderCost.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.MinOrderCost[%s]isInValid", InstrumentUpdateField.MinOrderCost.getString().c_str());
		return -4;
	}
	if (! InstrumentUpdateField.MaxOrderVolume.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.MaxOrderVolume[%s]isInValid", InstrumentUpdateField.MaxOrderVolume.getString().c_str());
		return -4;
	}
	if (! InstrumentUpdateField.PriceTick.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.PriceTick[%s]isInValid", InstrumentUpdateField.PriceTick.getString().c_str());
		return -4;
	}
	if (! InstrumentUpdateField.VolumeTick.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.VolumeTick[%s]isInValid", InstrumentUpdateField.VolumeTick.getString().c_str());
		return -4;
	}
	if (! InstrumentUpdateField.MaxOpenInterest.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.MaxOpenInterest[%s]isInValid", InstrumentUpdateField.MaxOpenInterest.getString().c_str());
		return -4;
	}
	if (! InstrumentUpdateField.BasisPrice.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.BasisPrice[%s]isInValid", InstrumentUpdateField.BasisPrice.getString().c_str());
		return -4;
	}
	if (! InstrumentUpdateField.UpperLimitPrice.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.UpperLimitPrice[%s]isInValid", InstrumentUpdateField.UpperLimitPrice.getString().c_str());
		return -4;
	}
	if (! InstrumentUpdateField.LowerLimitPrice.isValid())
	{
		PRINT_TO_STD("InstrumentUpdateField.LowerLimitPrice[%s]isInValid", InstrumentUpdateField.LowerLimitPrice.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(InstrumentUpdateField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendInstrumentUpdate;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &InstrumentUpdateField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///充币兑换请求
int CApiImpl::SendChangeCurrency(MegaChangeCurrencyStruct *pChangeCurrency, int nRequestID)
{
	CChangeCurrencyField ChangeCurrencyField;
	memcpy(&ChangeCurrencyField, pChangeCurrency, sizeof(CChangeCurrencyField));
	if (! ChangeCurrencyField.MemberID.isValid())
	{
		PRINT_TO_STD("ChangeCurrencyField.MemberID[%s]isInValid", ChangeCurrencyField.MemberID.getString().c_str());
		return -4;
	}
	if (! ChangeCurrencyField.AccountID.isValid())
	{
		PRINT_TO_STD("ChangeCurrencyField.AccountID[%s]isInValid", ChangeCurrencyField.AccountID.getString().c_str());
		return -4;
	}
	if (! ChangeCurrencyField.LocalID.isValid())
	{
		PRINT_TO_STD("ChangeCurrencyField.LocalID[%s]isInValid", ChangeCurrencyField.LocalID.getString().c_str());
		return -4;
	}
	if (! ChangeCurrencyField.ExchangeID.isValid())
	{
		PRINT_TO_STD("ChangeCurrencyField.ExchangeID[%s]isInValid", ChangeCurrencyField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! ChangeCurrencyField.InstrumentID1.isValid())
	{
		PRINT_TO_STD("ChangeCurrencyField.InstrumentID1[%s]isInValid", ChangeCurrencyField.InstrumentID1.getString().c_str());
		return -4;
	}
	if (! ChangeCurrencyField.InstrumentID2.isValid())
	{
		PRINT_TO_STD("ChangeCurrencyField.InstrumentID2[%s]isInValid", ChangeCurrencyField.InstrumentID2.getString().c_str());
		return -4;
	}
	if (! ChangeCurrencyField.Currency.isValid())
	{
		PRINT_TO_STD("ChangeCurrencyField.Currency[%s]isInValid", ChangeCurrencyField.Currency.getString().c_str());
		return -4;
	}
	if (! ChangeCurrencyField.Amount.isValid())
	{
		PRINT_TO_STD("ChangeCurrencyField.Amount[%s]isInValid", ChangeCurrencyField.Amount.getString().c_str());
		return -4;
	}
	if (! ChangeCurrencyField.Remark.isValid())
	{
		PRINT_TO_STD("ChangeCurrencyField.Remark[%s]isInValid", ChangeCurrencyField.Remark.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(ChangeCurrencyField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendChangeCurrency;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &ChangeCurrencyField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///钱包登录获取随机数请求
int CApiImpl::SendWalletNonce(MegaWalletNonceStruct *pWalletNonce, int nRequestID)
{
	CWalletNonceField WalletNonceField;
	memcpy(&WalletNonceField, pWalletNonce, sizeof(CWalletNonceField));
	if (! WalletNonceField.UserID.isValid())
	{
		PRINT_TO_STD("WalletNonceField.UserID[%s]isInValid", WalletNonceField.UserID.getString().c_str());
		return -4;
	}
	if (! WalletNonceField.MemberID.isValid())
	{
		PRINT_TO_STD("WalletNonceField.MemberID[%s]isInValid", WalletNonceField.MemberID.getString().c_str());
		return -4;
	}
	if (! WalletNonceField.Nonce.isValid())
	{
		PRINT_TO_STD("WalletNonceField.Nonce[%s]isInValid", WalletNonceField.Nonce.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(WalletNonceField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendWalletNonce;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &WalletNonceField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///钱包登录获取随机数请求
int CApiImpl::SendWalletLogin(MegaWalletLoginStruct *pWalletLogin, int nRequestID)
{
	CWalletLoginField WalletLoginField;
	memcpy(&WalletLoginField, pWalletLogin, sizeof(CWalletLoginField));
	if (! WalletLoginField.UserID.isValid())
	{
		PRINT_TO_STD("WalletLoginField.UserID[%s]isInValid", WalletLoginField.UserID.getString().c_str());
		return -4;
	}
	if (! WalletLoginField.Signature.isValid())
	{
		PRINT_TO_STD("WalletLoginField.Signature[%s]isInValid", WalletLoginField.Signature.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(WalletLoginField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendWalletLogin;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &WalletLoginField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///资金查询请求
int CApiImpl::SendQryAccount(MegaQryAccountStruct *pQryAccount, int nRequestID)
{
	CQryAccountField QryAccountField;
	memcpy(&QryAccountField, pQryAccount, sizeof(CQryAccountField));
	if (! QryAccountField.MemberID.isValid())
	{
		PRINT_TO_STD("QryAccountField.MemberID[%s]isInValid", QryAccountField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryAccountField.AccountID.isValid())
	{
		PRINT_TO_STD("QryAccountField.AccountID[%s]isInValid", QryAccountField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryAccountField.SettlementGroup.isValid())
	{
		PRINT_TO_STD("QryAccountField.SettlementGroup[%s]isInValid", QryAccountField.SettlementGroup.getString().c_str());
		return -4;
	}
	if (! QryAccountField.Currency.isValid())
	{
		PRINT_TO_STD("QryAccountField.Currency[%s]isInValid", QryAccountField.Currency.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryAccountField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryAccount;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryAccountField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///报单查询请求
int CApiImpl::SendQryOrder(MegaQryOrderStruct *pQryOrder, int nRequestID)
{
	CQryOrderField QryOrderField;
	memcpy(&QryOrderField, pQryOrder, sizeof(CQryOrderField));
	if (! QryOrderField.MemberID.isValid())
	{
		PRINT_TO_STD("QryOrderField.MemberID[%s]isInValid", QryOrderField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryOrderField.AccountID.isValid())
	{
		PRINT_TO_STD("QryOrderField.AccountID[%s]isInValid", QryOrderField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryOrderField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryOrderField.ExchangeID[%s]isInValid", QryOrderField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryOrderField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryOrderField.InstrumentID[%s]isInValid", QryOrderField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryOrderField.LocalID.isValid())
	{
		PRINT_TO_STD("QryOrderField.LocalID[%s]isInValid", QryOrderField.LocalID.getString().c_str());
		return -4;
	}
	if (! QryOrderField.OrderID.isValid())
	{
		PRINT_TO_STD("QryOrderField.OrderID[%s]isInValid", QryOrderField.OrderID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryOrderField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryOrder;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryOrderField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///持仓汇总查询请求
int CApiImpl::SendQryMemberPosition(MegaQryMemberPositionStruct *pQryMemberPosition, int nRequestID)
{
	CQryMemberPositionField QryMemberPositionField;
	memcpy(&QryMemberPositionField, pQryMemberPosition, sizeof(CQryMemberPositionField));
	if (! QryMemberPositionField.MemberID.isValid())
	{
		PRINT_TO_STD("QryMemberPositionField.MemberID[%s]isInValid", QryMemberPositionField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryMemberPositionField.AccountID.isValid())
	{
		PRINT_TO_STD("QryMemberPositionField.AccountID[%s]isInValid", QryMemberPositionField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryMemberPositionField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryMemberPositionField.ExchangeID[%s]isInValid", QryMemberPositionField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryMemberPositionField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryMemberPositionField.InstrumentID[%s]isInValid", QryMemberPositionField.InstrumentID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryMemberPositionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryMemberPosition;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryMemberPositionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///持仓查询请求
int CApiImpl::SendQryPosition(MegaQryPositionStruct *pQryPosition, int nRequestID)
{
	CQryPositionField QryPositionField;
	memcpy(&QryPositionField, pQryPosition, sizeof(CQryPositionField));
	if (! QryPositionField.MemberID.isValid())
	{
		PRINT_TO_STD("QryPositionField.MemberID[%s]isInValid", QryPositionField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryPositionField.AccountID.isValid())
	{
		PRINT_TO_STD("QryPositionField.AccountID[%s]isInValid", QryPositionField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryPositionField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryPositionField.ExchangeID[%s]isInValid", QryPositionField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryPositionField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryPositionField.InstrumentID[%s]isInValid", QryPositionField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryPositionField.PositionClass.isValid())
	{
		PRINT_TO_STD("QryPositionField.PositionClass[%s]isInValid", QryPositionField.PositionClass.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryPositionField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryPosition;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryPositionField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///标的查询请求
int CApiImpl::SendQryInstrument(MegaQryInstrumentStruct *pQryInstrument, int nRequestID)
{
	CQryInstrumentField QryInstrumentField;
	memcpy(&QryInstrumentField, pQryInstrument, sizeof(CQryInstrumentField));
	if (! QryInstrumentField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryInstrumentField.ExchangeID[%s]isInValid", QryInstrumentField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryInstrumentField.ProductGroup.isValid())
	{
		PRINT_TO_STD("QryInstrumentField.ProductGroup[%s]isInValid", QryInstrumentField.ProductGroup.getString().c_str());
		return -4;
	}
	if (! QryInstrumentField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryInstrumentField.InstrumentID[%s]isInValid", QryInstrumentField.InstrumentID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryInstrumentField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryInstrument;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryInstrumentField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///币种查询请求
int CApiImpl::SendQryCurrency(MegaQryCurrencyStruct *pQryCurrency, int nRequestID)
{
	CQryCurrencyField QryCurrencyField;
	memcpy(&QryCurrencyField, pQryCurrency, sizeof(CQryCurrencyField));
	if (! QryCurrencyField.SettlementGroup.isValid())
	{
		PRINT_TO_STD("QryCurrencyField.SettlementGroup[%s]isInValid", QryCurrencyField.SettlementGroup.getString().c_str());
		return -4;
	}
	if (! QryCurrencyField.Currency.isValid())
	{
		PRINT_TO_STD("QryCurrencyField.Currency[%s]isInValid", QryCurrencyField.Currency.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryCurrencyField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryCurrency;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryCurrencyField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///成员查询请求
int CApiImpl::SendQryMember(MegaQryMemberStruct *pQryMember, int nRequestID)
{
	CQryMemberField QryMemberField;
	memcpy(&QryMemberField, pQryMember, sizeof(CQryMemberField));
	if (! QryMemberField.MemberType.isValid())
	{
		PRINT_TO_STD("QryMemberField.MemberType[%s]isInValid", QryMemberField.MemberType.getString().c_str());
		return -4;
	}
	if (! QryMemberField.MemberID.isValid())
	{
		PRINT_TO_STD("QryMemberField.MemberID[%s]isInValid", QryMemberField.MemberID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryMemberField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryMember;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryMemberField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///价位查询请求
int CApiImpl::SendQryMarketOrder(MegaQryMarketOrderStruct *pQryMarketOrder, int nRequestID)
{
	CQryMarketOrderField QryMarketOrderField;
	memcpy(&QryMarketOrderField, pQryMarketOrder, sizeof(CQryMarketOrderField));
	if (! QryMarketOrderField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryMarketOrderField.ExchangeID[%s]isInValid", QryMarketOrderField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryMarketOrderField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryMarketOrderField.InstrumentID[%s]isInValid", QryMarketOrderField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryMarketOrderField.Direction.isValid())
	{
		PRINT_TO_STD("QryMarketOrderField.Direction[%s]isInValid", QryMarketOrderField.Direction.getString().c_str());
		return -4;
	}
	if (! QryMarketOrderField.Depth.isValid())
	{
		PRINT_TO_STD("QryMarketOrderField.Depth[%s]isInValid", QryMarketOrderField.Depth.getString().c_str());
		return -4;
	}
	if (! QryMarketOrderField.Tick.isValid())
	{
		PRINT_TO_STD("QryMarketOrderField.Tick[%s]isInValid", QryMarketOrderField.Tick.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryMarketOrderField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryMarketOrder;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryMarketOrderField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///APIKEY查询请求
int CApiImpl::SendQryAPIKey(MegaQryAPIKeyStruct *pQryAPIKey, int nRequestID)
{
	CQryAPIKeyField QryAPIKeyField;
	memcpy(&QryAPIKeyField, pQryAPIKey, sizeof(CQryAPIKeyField));
	if (! QryAPIKeyField.UserID.isValid())
	{
		PRINT_TO_STD("QryAPIKeyField.UserID[%s]isInValid", QryAPIKeyField.UserID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryAPIKeyField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryAPIKey;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryAPIKeyField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///保证金率查询请求
int CApiImpl::SendQryMarginRate(MegaQryMarginRateStruct *pQryMarginRate, int nRequestID)
{
	CQryMarginRateField QryMarginRateField;
	memcpy(&QryMarginRateField, pQryMarginRate, sizeof(CQryMarginRateField));
	if (! QryMarginRateField.MemberID.isValid())
	{
		PRINT_TO_STD("QryMarginRateField.MemberID[%s]isInValid", QryMarginRateField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryMarginRateField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryMarginRateField.ExchangeID[%s]isInValid", QryMarginRateField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryMarginRateField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryMarginRateField.InstrumentID[%s]isInValid", QryMarginRateField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryMarginRateField.AccountID.isValid())
	{
		PRINT_TO_STD("QryMarginRateField.AccountID[%s]isInValid", QryMarginRateField.AccountID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryMarginRateField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryMarginRate;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryMarginRateField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///手续费率查询请求
int CApiImpl::SendQryFee(MegaQryFeeStruct *pQryFee, int nRequestID)
{
	CQryFeeField QryFeeField;
	memcpy(&QryFeeField, pQryFee, sizeof(CQryFeeField));
	if (! QryFeeField.MemberID.isValid())
	{
		PRINT_TO_STD("QryFeeField.MemberID[%s]isInValid", QryFeeField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryFeeField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryFeeField.ExchangeID[%s]isInValid", QryFeeField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryFeeField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryFeeField.InstrumentID[%s]isInValid", QryFeeField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryFeeField.MatchRole.isValid())
	{
		PRINT_TO_STD("QryFeeField.MatchRole[%s]isInValid", QryFeeField.MatchRole.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryFeeField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryFee;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryFeeField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///触发订单查询请求
int CApiImpl::SendQryTriggerOrder(MegaQryTriggerOrderStruct *pQryTriggerOrder, int nRequestID)
{
	CQryTriggerOrderField QryTriggerOrderField;
	memcpy(&QryTriggerOrderField, pQryTriggerOrder, sizeof(CQryTriggerOrderField));
	if (! QryTriggerOrderField.MemberID.isValid())
	{
		PRINT_TO_STD("QryTriggerOrderField.MemberID[%s]isInValid", QryTriggerOrderField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryTriggerOrderField.AccountID.isValid())
	{
		PRINT_TO_STD("QryTriggerOrderField.AccountID[%s]isInValid", QryTriggerOrderField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryTriggerOrderField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryTriggerOrderField.ExchangeID[%s]isInValid", QryTriggerOrderField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryTriggerOrderField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryTriggerOrderField.InstrumentID[%s]isInValid", QryTriggerOrderField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryTriggerOrderField.LocalID.isValid())
	{
		PRINT_TO_STD("QryTriggerOrderField.LocalID[%s]isInValid", QryTriggerOrderField.LocalID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryTriggerOrderField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryTriggerOrder;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryTriggerOrderField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///杠杆倍数查询请求
int CApiImpl::SendQryLeverage(MegaQryLeverageStruct *pQryLeverage, int nRequestID)
{
	CQryLeverageField QryLeverageField;
	memcpy(&QryLeverageField, pQryLeverage, sizeof(CQryLeverageField));
	if (! QryLeverageField.MemberID.isValid())
	{
		PRINT_TO_STD("QryLeverageField.MemberID[%s]isInValid", QryLeverageField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryLeverageField.AccountID.isValid())
	{
		PRINT_TO_STD("QryLeverageField.AccountID[%s]isInValid", QryLeverageField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryLeverageField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("QryLeverageField.TradeUnitID[%s]isInValid", QryLeverageField.TradeUnitID.getString().c_str());
		return -4;
	}
	if (! QryLeverageField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryLeverageField.ExchangeID[%s]isInValid", QryLeverageField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryLeverageField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryLeverageField.InstrumentID[%s]isInValid", QryLeverageField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryLeverageField.PosiDirection.isValid())
	{
		PRINT_TO_STD("QryLeverageField.PosiDirection[%s]isInValid", QryLeverageField.PosiDirection.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryLeverageField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryLeverage;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryLeverageField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///行情查询请求
int CApiImpl::SendQryMarketData(MegaQryMarketDataStruct *pQryMarketData, int nRequestID)
{
	CQryMarketDataField QryMarketDataField;
	memcpy(&QryMarketDataField, pQryMarketData, sizeof(CQryMarketDataField));
	if (! QryMarketDataField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryMarketDataField.ExchangeID[%s]isInValid", QryMarketDataField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryMarketDataField.ProductGroup.isValid())
	{
		PRINT_TO_STD("QryMarketDataField.ProductGroup[%s]isInValid", QryMarketDataField.ProductGroup.getString().c_str());
		return -4;
	}
	if (! QryMarketDataField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryMarketDataField.InstrumentID[%s]isInValid", QryMarketDataField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryMarketDataField.From.isValid())
	{
		PRINT_TO_STD("QryMarketDataField.From[%s]isInValid", QryMarketDataField.From.getString().c_str());
		return -4;
	}
	if (! QryMarketDataField.Limit.isValid())
	{
		PRINT_TO_STD("QryMarketDataField.Limit[%s]isInValid", QryMarketDataField.Limit.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryMarketDataField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryMarketData;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryMarketDataField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///成交查询请求
int CApiImpl::SendQryTrade(MegaQryTradeStruct *pQryTrade, int nRequestID)
{
	CQryTradeField QryTradeField;
	memcpy(&QryTradeField, pQryTrade, sizeof(CQryTradeField));
	if (! QryTradeField.OrderID.isValid())
	{
		PRINT_TO_STD("QryTradeField.OrderID[%s]isInValid", QryTradeField.OrderID.getString().c_str());
		return -4;
	}
	if (! QryTradeField.MemberID.isValid())
	{
		PRINT_TO_STD("QryTradeField.MemberID[%s]isInValid", QryTradeField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryTradeField.AccountID.isValid())
	{
		PRINT_TO_STD("QryTradeField.AccountID[%s]isInValid", QryTradeField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryTradeField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryTradeField.ExchangeID[%s]isInValid", QryTradeField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryTradeField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryTradeField.InstrumentID[%s]isInValid", QryTradeField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryTradeField.Limit.isValid())
	{
		PRINT_TO_STD("QryTradeField.Limit[%s]isInValid", QryTradeField.Limit.getString().c_str());
		return -4;
	}
	if (! QryTradeField.Before.isValid())
	{
		PRINT_TO_STD("QryTradeField.Before[%s]isInValid", QryTradeField.Before.getString().c_str());
		return -4;
	}
	if (! QryTradeField.After.isValid())
	{
		PRINT_TO_STD("QryTradeField.After[%s]isInValid", QryTradeField.After.getString().c_str());
		return -4;
	}
	if (! QryTradeField.StartTime.isValid())
	{
		PRINT_TO_STD("QryTradeField.StartTime[%s]isInValid", QryTradeField.StartTime.getString().c_str());
		return -4;
	}
	if (! QryTradeField.EndTime.isValid())
	{
		PRINT_TO_STD("QryTradeField.EndTime[%s]isInValid", QryTradeField.EndTime.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryTradeField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryTrade;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryTradeField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///历史委托查询请求
int CApiImpl::SendQryFinishOrder(MegaQryFinishOrderStruct *pQryFinishOrder, int nRequestID)
{
	CQryFinishOrderField QryFinishOrderField;
	memcpy(&QryFinishOrderField, pQryFinishOrder, sizeof(CQryFinishOrderField));
	if (! QryFinishOrderField.MemberID.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.MemberID[%s]isInValid", QryFinishOrderField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryFinishOrderField.OrderID.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.OrderID[%s]isInValid", QryFinishOrderField.OrderID.getString().c_str());
		return -4;
	}
	if (! QryFinishOrderField.AccountID.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.AccountID[%s]isInValid", QryFinishOrderField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryFinishOrderField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.ExchangeID[%s]isInValid", QryFinishOrderField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryFinishOrderField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.InstrumentID[%s]isInValid", QryFinishOrderField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryFinishOrderField.OrderStatus.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.OrderStatus[%s]isInValid", QryFinishOrderField.OrderStatus.getString().c_str());
		return -4;
	}
	if (! QryFinishOrderField.Limit.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.Limit[%s]isInValid", QryFinishOrderField.Limit.getString().c_str());
		return -4;
	}
	if (! QryFinishOrderField.Before.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.Before[%s]isInValid", QryFinishOrderField.Before.getString().c_str());
		return -4;
	}
	if (! QryFinishOrderField.After.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.After[%s]isInValid", QryFinishOrderField.After.getString().c_str());
		return -4;
	}
	if (! QryFinishOrderField.StartTime.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.StartTime[%s]isInValid", QryFinishOrderField.StartTime.getString().c_str());
		return -4;
	}
	if (! QryFinishOrderField.EndTime.isValid())
	{
		PRINT_TO_STD("QryFinishOrderField.EndTime[%s]isInValid", QryFinishOrderField.EndTime.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryFinishOrderField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryFinishOrder;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryFinishOrderField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///历史条件单查询请求
int CApiImpl::SendQryFinishTriggerOrder(MegaQryFinishTriggerOrderStruct *pQryFinishTriggerOrder, int nRequestID)
{
	CQryFinishTriggerOrderField QryFinishTriggerOrderField;
	memcpy(&QryFinishTriggerOrderField, pQryFinishTriggerOrder, sizeof(CQryFinishTriggerOrderField));
	if (! QryFinishTriggerOrderField.MemberID.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.MemberID[%s]isInValid", QryFinishTriggerOrderField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.OrderID.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.OrderID[%s]isInValid", QryFinishTriggerOrderField.OrderID.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.AccountID.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.AccountID[%s]isInValid", QryFinishTriggerOrderField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.ExchangeID[%s]isInValid", QryFinishTriggerOrderField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.InstrumentID[%s]isInValid", QryFinishTriggerOrderField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.TriggerStatus.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.TriggerStatus[%s]isInValid", QryFinishTriggerOrderField.TriggerStatus.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.TriggerOrderType.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.TriggerOrderType[%s]isInValid", QryFinishTriggerOrderField.TriggerOrderType.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.Limit.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.Limit[%s]isInValid", QryFinishTriggerOrderField.Limit.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.Before.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.Before[%s]isInValid", QryFinishTriggerOrderField.Before.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.After.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.After[%s]isInValid", QryFinishTriggerOrderField.After.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.StartTime.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.StartTime[%s]isInValid", QryFinishTriggerOrderField.StartTime.getString().c_str());
		return -4;
	}
	if (! QryFinishTriggerOrderField.EndTime.isValid())
	{
		PRINT_TO_STD("QryFinishTriggerOrderField.EndTime[%s]isInValid", QryFinishTriggerOrderField.EndTime.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryFinishTriggerOrderField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryFinishTriggerOrder;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryFinishTriggerOrderField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///指定杠杆倍数查询请求
int CApiImpl::SendQryWithLeverage(MegaQryWithLeverageStruct *pQryWithLeverage, int nRequestID)
{
	CQryWithLeverageField QryWithLeverageField;
	memcpy(&QryWithLeverageField, pQryWithLeverage, sizeof(CQryWithLeverageField));
	if (! QryWithLeverageField.MemberID.isValid())
	{
		PRINT_TO_STD("QryWithLeverageField.MemberID[%s]isInValid", QryWithLeverageField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryWithLeverageField.AccountID.isValid())
	{
		PRINT_TO_STD("QryWithLeverageField.AccountID[%s]isInValid", QryWithLeverageField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryWithLeverageField.TradeUnitID.isValid())
	{
		PRINT_TO_STD("QryWithLeverageField.TradeUnitID[%s]isInValid", QryWithLeverageField.TradeUnitID.getString().c_str());
		return -4;
	}
	if (! QryWithLeverageField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryWithLeverageField.ExchangeID[%s]isInValid", QryWithLeverageField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryWithLeverageField.PosiDirection.isValid())
	{
		PRINT_TO_STD("QryWithLeverageField.PosiDirection[%s]isInValid", QryWithLeverageField.PosiDirection.getString().c_str());
		return -4;
	}
	if (! QryWithLeverageField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryWithLeverageField.InstrumentID[%s]isInValid", QryWithLeverageField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryWithLeverageField.LongLeverage.isValid())
	{
		PRINT_TO_STD("QryWithLeverageField.LongLeverage[%s]isInValid", QryWithLeverageField.LongLeverage.getString().c_str());
		return -4;
	}
	if (! QryWithLeverageField.ShortLeverage.isValid())
	{
		PRINT_TO_STD("QryWithLeverageField.ShortLeverage[%s]isInValid", QryWithLeverageField.ShortLeverage.getString().c_str());
		return -4;
	}
	if (! QryWithLeverageField.NetLeverage.isValid())
	{
		PRINT_TO_STD("QryWithLeverageField.NetLeverage[%s]isInValid", QryWithLeverageField.NetLeverage.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryWithLeverageField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryWithLeverage;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryWithLeverageField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///K线查询请求
int CApiImpl::SendQryKLine(MegaQryKLineStruct *pQryKLine, int nRequestID)
{
	CQryKLineField QryKLineField;
	memcpy(&QryKLineField, pQryKLine, sizeof(CQryKLineField));
	if (! QryKLineField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryKLineField.ExchangeID[%s]isInValid", QryKLineField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryKLineField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryKLineField.InstrumentID[%s]isInValid", QryKLineField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryKLineField.Bar.isValid())
	{
		PRINT_TO_STD("QryKLineField.Bar[%s]isInValid", QryKLineField.Bar.getString().c_str());
		return -4;
	}
	if (! QryKLineField.EndTime.isValid())
	{
		PRINT_TO_STD("QryKLineField.EndTime[%s]isInValid", QryKLineField.EndTime.getString().c_str());
		return -4;
	}
	if (! QryKLineField.Limit.isValid())
	{
		PRINT_TO_STD("QryKLineField.Limit[%s]isInValid", QryKLineField.Limit.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryKLineField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryKLine;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryKLineField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///线查询请求
int CApiImpl::SendQryLine(MegaQryLineStruct *pQryLine, int nRequestID)
{
	CQryLineField QryLineField;
	memcpy(&QryLineField, pQryLine, sizeof(CQryLineField));
	if (! QryLineField.ExchangeID.isValid())
	{
		PRINT_TO_STD("QryLineField.ExchangeID[%s]isInValid", QryLineField.ExchangeID.getString().c_str());
		return -4;
	}
	if (! QryLineField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryLineField.InstrumentID[%s]isInValid", QryLineField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryLineField.Bar.isValid())
	{
		PRINT_TO_STD("QryLineField.Bar[%s]isInValid", QryLineField.Bar.getString().c_str());
		return -4;
	}
	if (! QryLineField.Limit.isValid())
	{
		PRINT_TO_STD("QryLineField.Limit[%s]isInValid", QryLineField.Limit.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryLineField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryLine;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryLineField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///资金流水查询请求
int CApiImpl::SendQryAccountDetail(MegaQryAccountDetailStruct *pQryAccountDetail, int nRequestID)
{
	CQryAccountDetailField QryAccountDetailField;
	memcpy(&QryAccountDetailField, pQryAccountDetail, sizeof(CQryAccountDetailField));
	if (! QryAccountDetailField.ProductGroup.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.ProductGroup[%s]isInValid", QryAccountDetailField.ProductGroup.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.AccountID.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.AccountID[%s]isInValid", QryAccountDetailField.AccountID.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.MemberID.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.MemberID[%s]isInValid", QryAccountDetailField.MemberID.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.Currency.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.Currency[%s]isInValid", QryAccountDetailField.Currency.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.SettlementGroup.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.SettlementGroup[%s]isInValid", QryAccountDetailField.SettlementGroup.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.InstrumentID[%s]isInValid", QryAccountDetailField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.Source.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.Source[%s]isInValid", QryAccountDetailField.Source.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.Limit.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.Limit[%s]isInValid", QryAccountDetailField.Limit.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.Before.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.Before[%s]isInValid", QryAccountDetailField.Before.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.After.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.After[%s]isInValid", QryAccountDetailField.After.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.StartTime.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.StartTime[%s]isInValid", QryAccountDetailField.StartTime.getString().c_str());
		return -4;
	}
	if (! QryAccountDetailField.EndTime.isValid())
	{
		PRINT_TO_STD("QryAccountDetailField.EndTime[%s]isInValid", QryAccountDetailField.EndTime.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QryAccountDetailField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQryAccountDetail;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QryAccountDetailField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///结算流水查询请求
int CApiImpl::SendQrySettleDetail(MegaQrySettleDetailStruct *pQrySettleDetail, int nRequestID)
{
	CQrySettleDetailField QrySettleDetailField;
	memcpy(&QrySettleDetailField, pQrySettleDetail, sizeof(CQrySettleDetailField));
	if (! QrySettleDetailField.ProductGroup.isValid())
	{
		PRINT_TO_STD("QrySettleDetailField.ProductGroup[%s]isInValid", QrySettleDetailField.ProductGroup.getString().c_str());
		return -4;
	}
	if (! QrySettleDetailField.InstrumentID.isValid())
	{
		PRINT_TO_STD("QrySettleDetailField.InstrumentID[%s]isInValid", QrySettleDetailField.InstrumentID.getString().c_str());
		return -4;
	}
	if (! QrySettleDetailField.SettleAction.isValid())
	{
		PRINT_TO_STD("QrySettleDetailField.SettleAction[%s]isInValid", QrySettleDetailField.SettleAction.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(QrySettleDetailField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendQrySettleDetail;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &QrySettleDetailField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///检查TOKEN请求
int CApiImpl::SendCheckToken(MegaCheckTokenStruct *pCheckToken, int nRequestID)
{
	CCheckTokenField CheckTokenField;
	memcpy(&CheckTokenField, pCheckToken, sizeof(CCheckTokenField));
	if (! CheckTokenField.Token.isValid())
	{
		PRINT_TO_STD("CheckTokenField.Token[%s]isInValid", CheckTokenField.Token.getString().c_str());
		return -4;
	}
	if (! CheckTokenField.UserID.isValid())
	{
		PRINT_TO_STD("CheckTokenField.UserID[%s]isInValid", CheckTokenField.UserID.getString().c_str());
		return -4;
	}
	if (! CheckTokenField.MaxLocalID.isValid())
	{
		PRINT_TO_STD("CheckTokenField.MaxLocalID[%s]isInValid", CheckTokenField.MaxLocalID.getString().c_str());
		return -4;
	}
	if (! CheckTokenField.MemberID.isValid())
	{
		PRINT_TO_STD("CheckTokenField.MemberID[%s]isInValid", CheckTokenField.MemberID.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(CheckTokenField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendCheckToken;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &CheckTokenField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}
///批量操作请求(HTTP接口专用)
int CApiImpl::SendBatch(MegaCommandHeadStruct *pCommandHead, int nRequestID)
{
	CCommandHeadField CommandHeadField;
	memcpy(&CommandHeadField, pCommandHead, sizeof(CCommandHeadField));
	if (! CommandHeadField.IsLast.isValid())
	{
		PRINT_TO_STD("CommandHeadField.IsLast[%s]isInValid", CommandHeadField.IsLast.getString().c_str());
		return -4;
	}
	CFieldDescribe *pFieldDescribe = &(CommandHeadField.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_SendBatch;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &CommandHeadField);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);

	return 0;
}

bool CApiImpl::RspField(const TOutFogMetaRef* pRspRef, const void *pRspBuffer)
{
	switch (pRspRef->Type)
	{
	case OUTFOG_TYPE_ADDSESSION:
	{
		m_pCallback->OnConnected();
		return true;
	}
	case OUTFOG_TYPE_DELSESSION:
	{
		m_pCallback->OnDisconnected(pRspRef->SubjectNo);
		return true;
	}
	}
	switch (pRspRef->MessageNo)
	{
	case TID_RecvCommand: //一般消息应答
	{
		MegaCommandStruct* pCommand = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Command:
				mSubjectNo = -mSubjectNo;
			case FID_Command:
			{
				CCommandField CommandField;
				if (pRspBuffer != NULL)
				{
					CCommandField::m_Describe.getSingleField(&CommandField, pRspBuffer);
					pCommand = (MegaCommandStruct*)&CommandField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvCommand(pCommand, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvInitLink: //创建上层连接应答
	{
		MegaLinkStruct* pLink = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Link:
				mSubjectNo = -mSubjectNo;
			case FID_Link:
			{
				CLinkField LinkField;
				if (pRspBuffer != NULL)
				{
					CLinkField::m_Describe.getSingleField(&LinkField, pRspBuffer);
					pLink = (MegaLinkStruct*)&LinkField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvInitLink(pLink, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvUserLogin: //用户登录应答
	{
		MegaUserLoginStruct* pUserLogin = NULL;
		MegaUserSessionStruct* pUserSession = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_UserLogin:
				mSubjectNo = -mSubjectNo;
			case FID_UserLogin:
			{
				CUserLoginField UserLoginField;
				if (pRspBuffer != NULL)
				{
					CUserLoginField::m_Describe.getSingleField(&UserLoginField, pRspBuffer);
					pUserLogin = (MegaUserLoginStruct*)&UserLoginField;
				}
				break;
			}
			case -FID_UserSession:
				mSubjectNo = -mSubjectNo;
			case FID_UserSession:
			{
				CUserSessionField UserSessionField;
				if (pRspBuffer != NULL)
				{
					CUserSessionField::m_Describe.getSingleField(&UserSessionField, pRspBuffer);
					pUserSession = (MegaUserSessionStruct*)&UserSessionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvUserLogin(pUserLogin,pUserSession, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvUserLogout: //用户退出应答
	{
		MegaUserLogoutStruct* pUserLogout = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_UserLogout:
				mSubjectNo = -mSubjectNo;
			case FID_UserLogout:
			{
				CUserLogoutField UserLogoutField;
				if (pRspBuffer != NULL)
				{
					CUserLogoutField::m_Describe.getSingleField(&UserLogoutField, pRspBuffer);
					pUserLogout = (MegaUserLogoutStruct*)&UserLogoutField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvUserLogout(pUserLogout, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvOrderInsert: //报单录入应答
	{
		MegaOrderInsertStruct* pOrderInsert = NULL;
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_OrderInsert:
				mSubjectNo = -mSubjectNo;
			case FID_OrderInsert:
			{
				COrderInsertField OrderInsertField;
				if (pRspBuffer != NULL)
				{
					COrderInsertField::m_Describe.getSingleField(&OrderInsertField, pRspBuffer);
					pOrderInsert = (MegaOrderInsertStruct*)&OrderInsertField;
				}
				break;
			}
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvOrderInsert(pOrderInsert,pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvOrderAction: //报单操作应答
	{
		MegaOrderActionStruct* pOrderAction = NULL;
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_OrderAction:
				mSubjectNo = -mSubjectNo;
			case FID_OrderAction:
			{
				COrderActionField OrderActionField;
				if (pRspBuffer != NULL)
				{
					COrderActionField::m_Describe.getSingleField(&OrderActionField, pRspBuffer);
					pOrderAction = (MegaOrderActionStruct*)&OrderActionField;
				}
				break;
			}
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvOrderAction(pOrderAction,pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvTriggerOrderInsert: //触发单录入应答
	{
		MegaTriggerOrderInsertStruct* pTriggerOrderInsert = NULL;
		MegaTriggerOrderStruct* pTriggerOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_TriggerOrderInsert:
				mSubjectNo = -mSubjectNo;
			case FID_TriggerOrderInsert:
			{
				CTriggerOrderInsertField TriggerOrderInsertField;
				if (pRspBuffer != NULL)
				{
					CTriggerOrderInsertField::m_Describe.getSingleField(&TriggerOrderInsertField, pRspBuffer);
					pTriggerOrderInsert = (MegaTriggerOrderInsertStruct*)&TriggerOrderInsertField;
				}
				break;
			}
			case -FID_TriggerOrder:
				mSubjectNo = -mSubjectNo;
			case FID_TriggerOrder:
			{
				CTriggerOrderField TriggerOrderField;
				if (pRspBuffer != NULL)
				{
					CTriggerOrderField::m_Describe.getSingleField(&TriggerOrderField, pRspBuffer);
					pTriggerOrder = (MegaTriggerOrderStruct*)&TriggerOrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvTriggerOrderInsert(pTriggerOrderInsert,pTriggerOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvTriggerOrderAction: //触发单操作应答
	{
		MegaTriggerOrderActionStruct* pTriggerOrderAction = NULL;
		MegaTriggerOrderStruct* pTriggerOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_TriggerOrderAction:
				mSubjectNo = -mSubjectNo;
			case FID_TriggerOrderAction:
			{
				CTriggerOrderActionField TriggerOrderActionField;
				if (pRspBuffer != NULL)
				{
					CTriggerOrderActionField::m_Describe.getSingleField(&TriggerOrderActionField, pRspBuffer);
					pTriggerOrderAction = (MegaTriggerOrderActionStruct*)&TriggerOrderActionField;
				}
				break;
			}
			case -FID_TriggerOrder:
				mSubjectNo = -mSubjectNo;
			case FID_TriggerOrder:
			{
				CTriggerOrderField TriggerOrderField;
				if (pRspBuffer != NULL)
				{
					CTriggerOrderField::m_Describe.getSingleField(&TriggerOrderField, pRspBuffer);
					pTriggerOrder = (MegaTriggerOrderStruct*)&TriggerOrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvTriggerOrderAction(pTriggerOrderAction,pTriggerOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvCloseOrderInsert: //止盈止损报单录入应答
	{
		MegaCloseOrderInsertStruct* pCloseOrderInsert = NULL;
		MegaOrderStruct* pOrder = NULL;
		MegaTriggerOrderStruct* pTriggerOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_CloseOrderInsert:
				mSubjectNo = -mSubjectNo;
			case FID_CloseOrderInsert:
			{
				CCloseOrderInsertField CloseOrderInsertField;
				if (pRspBuffer != NULL)
				{
					CCloseOrderInsertField::m_Describe.getSingleField(&CloseOrderInsertField, pRspBuffer);
					pCloseOrderInsert = (MegaCloseOrderInsertStruct*)&CloseOrderInsertField;
				}
				break;
			}
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
			case -FID_TriggerOrder:
				mSubjectNo = -mSubjectNo;
			case FID_TriggerOrder:
			{
				CTriggerOrderField TriggerOrderField;
				if (pRspBuffer != NULL)
				{
					CTriggerOrderField::m_Describe.getSingleField(&TriggerOrderField, pRspBuffer);
					pTriggerOrder = (MegaTriggerOrderStruct*)&TriggerOrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvCloseOrderInsert(pCloseOrderInsert,pOrder,pTriggerOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvCloseOrderAction: //止盈止损报单操作应答
	{
		MegaCloseOrderActionStruct* pCloseOrderAction = NULL;
		MegaTriggerOrderStruct* pTriggerOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_CloseOrderAction:
				mSubjectNo = -mSubjectNo;
			case FID_CloseOrderAction:
			{
				CCloseOrderActionField CloseOrderActionField;
				if (pRspBuffer != NULL)
				{
					CCloseOrderActionField::m_Describe.getSingleField(&CloseOrderActionField, pRspBuffer);
					pCloseOrderAction = (MegaCloseOrderActionStruct*)&CloseOrderActionField;
				}
				break;
			}
			case -FID_TriggerOrder:
				mSubjectNo = -mSubjectNo;
			case FID_TriggerOrder:
			{
				CTriggerOrderField TriggerOrderField;
				if (pRspBuffer != NULL)
				{
					CTriggerOrderField::m_Describe.getSingleField(&TriggerOrderField, pRspBuffer);
					pTriggerOrder = (MegaTriggerOrderStruct*)&TriggerOrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvCloseOrderAction(pCloseOrderAction,pTriggerOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQuoteInsert: //报价录入应答
	{
		MegaQuoteInsertStruct* pQuoteInsert = NULL;
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_QuoteInsert:
				mSubjectNo = -mSubjectNo;
			case FID_QuoteInsert:
			{
				CQuoteInsertField QuoteInsertField;
				if (pRspBuffer != NULL)
				{
					CQuoteInsertField::m_Describe.getSingleField(&QuoteInsertField, pRspBuffer);
					pQuoteInsert = (MegaQuoteInsertStruct*)&QuoteInsertField;
				}
				break;
			}
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQuoteInsert(pQuoteInsert,pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQuoteAction: //报价操作应答
	{
		MegaQuoteActionStruct* pQuoteAction = NULL;
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_QuoteAction:
				mSubjectNo = -mSubjectNo;
			case FID_QuoteAction:
			{
				CQuoteActionField QuoteActionField;
				if (pRspBuffer != NULL)
				{
					CQuoteActionField::m_Describe.getSingleField(&QuoteActionField, pRspBuffer);
					pQuoteAction = (MegaQuoteActionStruct*)&QuoteActionField;
				}
				break;
			}
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQuoteAction(pQuoteAction,pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvTopicAction: //订阅主题应答
	{
		MegaTopicActionStruct* pTopicAction = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_TopicAction:
				mSubjectNo = -mSubjectNo;
			case FID_TopicAction:
			{
				CTopicActionField TopicActionField;
				if (pRspBuffer != NULL)
				{
					CTopicActionField::m_Describe.getSingleField(&TopicActionField, pRspBuffer);
					pTopicAction = (MegaTopicActionStruct*)&TopicActionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvTopicAction(pTopicAction, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvMarketDataUpdate: //设置行情应答
	{
		MegaMarketDataUpdateStruct* pMarketDataUpdate = NULL;
		MegaMarketDataStruct* pMarketData = NULL;
		MegaInstrumentStruct* pInstrument = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MarketDataUpdate:
				mSubjectNo = -mSubjectNo;
			case FID_MarketDataUpdate:
			{
				CMarketDataUpdateField MarketDataUpdateField;
				if (pRspBuffer != NULL)
				{
					CMarketDataUpdateField::m_Describe.getSingleField(&MarketDataUpdateField, pRspBuffer);
					pMarketDataUpdate = (MegaMarketDataUpdateStruct*)&MarketDataUpdateField;
				}
				break;
			}
			case -FID_MarketData:
				mSubjectNo = -mSubjectNo;
			case FID_MarketData:
			{
				CMarketDataField MarketDataField;
				if (pRspBuffer != NULL)
				{
					CMarketDataField::m_Describe.getSingleField(&MarketDataField, pRspBuffer);
					pMarketData = (MegaMarketDataStruct*)&MarketDataField;
				}
				break;
			}
			case -FID_Instrument:
				mSubjectNo = -mSubjectNo;
			case FID_Instrument:
			{
				CInstrumentField InstrumentField;
				if (pRspBuffer != NULL)
				{
					CInstrumentField::m_Describe.getSingleField(&InstrumentField, pRspBuffer);
					pInstrument = (MegaInstrumentStruct*)&InstrumentField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvMarketDataUpdate(pMarketDataUpdate,pMarketData,pInstrument, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvPositionAction: //调整持仓应答
	{
		MegaPositionActionStruct* pPositionAction = NULL;
		MegaPositionStruct* pPosition = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_PositionAction:
				mSubjectNo = -mSubjectNo;
			case FID_PositionAction:
			{
				CPositionActionField PositionActionField;
				if (pRspBuffer != NULL)
				{
					CPositionActionField::m_Describe.getSingleField(&PositionActionField, pRspBuffer);
					pPositionAction = (MegaPositionActionStruct*)&PositionActionField;
				}
				break;
			}
			case -FID_Position:
				mSubjectNo = -mSubjectNo;
			case FID_Position:
			{
				CPositionField PositionField;
				if (pRspBuffer != NULL)
				{
					CPositionField::m_Describe.getSingleField(&PositionField, pRspBuffer);
					pPosition = (MegaPositionStruct*)&PositionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvPositionAction(pPositionAction,pPosition, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvAccountAction: //用户出入金应答
	{
		MegaAccountActionStruct* pAccountAction = NULL;
		MegaAccountStruct* pAccount = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_AccountAction:
				mSubjectNo = -mSubjectNo;
			case FID_AccountAction:
			{
				CAccountActionField AccountActionField;
				if (pRspBuffer != NULL)
				{
					CAccountActionField::m_Describe.getSingleField(&AccountActionField, pRspBuffer);
					pAccountAction = (MegaAccountActionStruct*)&AccountActionField;
				}
				break;
			}
			case -FID_Account:
				mSubjectNo = -mSubjectNo;
			case FID_Account:
			{
				CAccountField AccountField;
				if (pRspBuffer != NULL)
				{
					CAccountField::m_Describe.getSingleField(&AccountField, pRspBuffer);
					pAccount = (MegaAccountStruct*)&AccountField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvAccountAction(pAccountAction,pAccount, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvMemberAction: //成员设置应答
	{
		MegaMemberActionStruct* pMemberAction = NULL;
		MegaMemberStruct* pMember = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MemberAction:
				mSubjectNo = -mSubjectNo;
			case FID_MemberAction:
			{
				CMemberActionField MemberActionField;
				if (pRspBuffer != NULL)
				{
					CMemberActionField::m_Describe.getSingleField(&MemberActionField, pRspBuffer);
					pMemberAction = (MegaMemberActionStruct*)&MemberActionField;
				}
				break;
			}
			case -FID_Member:
				mSubjectNo = -mSubjectNo;
			case FID_Member:
			{
				CMemberField MemberField;
				if (pRspBuffer != NULL)
				{
					CMemberField::m_Describe.getSingleField(&MemberField, pRspBuffer);
					pMember = (MegaMemberStruct*)&MemberField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvMemberAction(pMemberAction,pMember, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvRobotOrder: //内部做市商申报应答
	{
		MegaRobotOrderStruct* pRobotOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_RobotOrder:
				mSubjectNo = -mSubjectNo;
			case FID_RobotOrder:
			{
				CRobotOrderField RobotOrderField;
				if (pRspBuffer != NULL)
				{
					CRobotOrderField::m_Describe.getSingleField(&RobotOrderField, pRspBuffer);
					pRobotOrder = (MegaRobotOrderStruct*)&RobotOrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvRobotOrder(pRobotOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvOTCTradeInsert: //场外成交申报应答
	{
		MegaOTCTradeInsertStruct* pOTCTradeInsert = NULL;
		MegaOTCTradeStruct* pOTCTrade = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_OTCTradeInsert:
				mSubjectNo = -mSubjectNo;
			case FID_OTCTradeInsert:
			{
				COTCTradeInsertField OTCTradeInsertField;
				if (pRspBuffer != NULL)
				{
					COTCTradeInsertField::m_Describe.getSingleField(&OTCTradeInsertField, pRspBuffer);
					pOTCTradeInsert = (MegaOTCTradeInsertStruct*)&OTCTradeInsertField;
				}
				break;
			}
			case -FID_OTCTrade:
				mSubjectNo = -mSubjectNo;
			case FID_OTCTrade:
			{
				COTCTradeField OTCTradeField;
				if (pRspBuffer != NULL)
				{
					COTCTradeField::m_Describe.getSingleField(&OTCTradeField, pRspBuffer);
					pOTCTrade = (MegaOTCTradeStruct*)&OTCTradeField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvOTCTradeInsert(pOTCTradeInsert,pOTCTrade, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvSettlement: //资金结算应答
	{
		MegaSettlementStruct* pSettlement = NULL;
		MegaSettleDetailStruct* pSettleDetail = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Settlement:
				mSubjectNo = -mSubjectNo;
			case FID_Settlement:
			{
				CSettlementField SettlementField;
				if (pRspBuffer != NULL)
				{
					CSettlementField::m_Describe.getSingleField(&SettlementField, pRspBuffer);
					pSettlement = (MegaSettlementStruct*)&SettlementField;
				}
				break;
			}
			case -FID_SettleDetail:
				mSubjectNo = -mSubjectNo;
			case FID_SettleDetail:
			{
				CSettleDetailField SettleDetailField;
				if (pRspBuffer != NULL)
				{
					CSettleDetailField::m_Describe.getSingleField(&SettleDetailField, pRspBuffer);
					pSettleDetail = (MegaSettleDetailStruct*)&SettleDetailField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvSettlement(pSettlement,pSettleDetail, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvClearMarket: //清理行情数据应答
	{
		MegaClearMarketStruct* pClearMarket = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_ClearMarket:
				mSubjectNo = -mSubjectNo;
			case FID_ClearMarket:
			{
				CClearMarketField ClearMarketField;
				if (pRspBuffer != NULL)
				{
					CClearMarketField::m_Describe.getSingleField(&ClearMarketField, pRspBuffer);
					pClearMarket = (MegaClearMarketStruct*)&ClearMarketField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvClearMarket(pClearMarket, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvClearLastKLine: //清理最新K线应答
	{
		MegaClearLastKLineStruct* pClearLastKLine = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_ClearLastKLine:
				mSubjectNo = -mSubjectNo;
			case FID_ClearLastKLine:
			{
				CClearLastKLineField ClearLastKLineField;
				if (pRspBuffer != NULL)
				{
					CClearLastKLineField::m_Describe.getSingleField(&ClearLastKLineField, pRspBuffer);
					pClearLastKLine = (MegaClearLastKLineStruct*)&ClearLastKLineField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvClearLastKLine(pClearLastKLine, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvResetLocalID: //重置LocalID应答
	{
		MegaResetLocalIDStruct* pResetLocalID = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_ResetLocalID:
				mSubjectNo = -mSubjectNo;
			case FID_ResetLocalID:
			{
				CResetLocalIDField ResetLocalIDField;
				if (pRspBuffer != NULL)
				{
					CResetLocalIDField::m_Describe.getSingleField(&ResetLocalIDField, pRspBuffer);
					pResetLocalID = (MegaResetLocalIDStruct*)&ResetLocalIDField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvResetLocalID(pResetLocalID, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvOTokenInsert: //OToken录入应答
	{
		MegaOTokenInsertStruct* pOTokenInsert = NULL;
		MegaUserSessionStruct* pUserSession = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_OTokenInsert:
				mSubjectNo = -mSubjectNo;
			case FID_OTokenInsert:
			{
				COTokenInsertField OTokenInsertField;
				if (pRspBuffer != NULL)
				{
					COTokenInsertField::m_Describe.getSingleField(&OTokenInsertField, pRspBuffer);
					pOTokenInsert = (MegaOTokenInsertStruct*)&OTokenInsertField;
				}
				break;
			}
			case -FID_UserSession:
				mSubjectNo = -mSubjectNo;
			case FID_UserSession:
			{
				CUserSessionField UserSessionField;
				if (pRspBuffer != NULL)
				{
					CUserSessionField::m_Describe.getSingleField(&UserSessionField, pRspBuffer);
					pUserSession = (MegaUserSessionStruct*)&UserSessionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvOTokenInsert(pOTokenInsert,pUserSession, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvOTokenAction: //OToken操作应答
	{
		MegaOTokenActionStruct* pOTokenAction = NULL;
		MegaUserSessionStruct* pUserSession = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_OTokenAction:
				mSubjectNo = -mSubjectNo;
			case FID_OTokenAction:
			{
				COTokenActionField OTokenActionField;
				if (pRspBuffer != NULL)
				{
					COTokenActionField::m_Describe.getSingleField(&OTokenActionField, pRspBuffer);
					pOTokenAction = (MegaOTokenActionStruct*)&OTokenActionField;
				}
				break;
			}
			case -FID_UserSession:
				mSubjectNo = -mSubjectNo;
			case FID_UserSession:
			{
				CUserSessionField UserSessionField;
				if (pRspBuffer != NULL)
				{
					CUserSessionField::m_Describe.getSingleField(&UserSessionField, pRspBuffer);
					pUserSession = (MegaUserSessionStruct*)&UserSessionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvOTokenAction(pOTokenAction,pUserSession, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvAPIKeyInsert: //APIKEY录入应答
	{
		MegaAPIKeyInsertStruct* pAPIKeyInsert = NULL;
		MegaAPIKeyStruct* pAPIKey = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_APIKeyInsert:
				mSubjectNo = -mSubjectNo;
			case FID_APIKeyInsert:
			{
				CAPIKeyInsertField APIKeyInsertField;
				if (pRspBuffer != NULL)
				{
					CAPIKeyInsertField::m_Describe.getSingleField(&APIKeyInsertField, pRspBuffer);
					pAPIKeyInsert = (MegaAPIKeyInsertStruct*)&APIKeyInsertField;
				}
				break;
			}
			case -FID_APIKey:
				mSubjectNo = -mSubjectNo;
			case FID_APIKey:
			{
				CAPIKeyField APIKeyField;
				if (pRspBuffer != NULL)
				{
					CAPIKeyField::m_Describe.getSingleField(&APIKeyField, pRspBuffer);
					pAPIKey = (MegaAPIKeyStruct*)&APIKeyField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvAPIKeyInsert(pAPIKeyInsert,pAPIKey, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvAPIKeyAction: //APIKEY操作应答
	{
		MegaAPIKeyActionStruct* pAPIKeyAction = NULL;
		MegaAPIKeyStruct* pAPIKey = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_APIKeyAction:
				mSubjectNo = -mSubjectNo;
			case FID_APIKeyAction:
			{
				CAPIKeyActionField APIKeyActionField;
				if (pRspBuffer != NULL)
				{
					CAPIKeyActionField::m_Describe.getSingleField(&APIKeyActionField, pRspBuffer);
					pAPIKeyAction = (MegaAPIKeyActionStruct*)&APIKeyActionField;
				}
				break;
			}
			case -FID_APIKey:
				mSubjectNo = -mSubjectNo;
			case FID_APIKey:
			{
				CAPIKeyField APIKeyField;
				if (pRspBuffer != NULL)
				{
					CAPIKeyField::m_Describe.getSingleField(&APIKeyField, pRspBuffer);
					pAPIKey = (MegaAPIKeyStruct*)&APIKeyField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvAPIKeyAction(pAPIKeyAction,pAPIKey, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvPositionMerge: //仓位合并操作应答
	{
		MegaPositionMergeStruct* pPositionMerge = NULL;
		MegaPositionStruct* pPosition = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_PositionMerge:
				mSubjectNo = -mSubjectNo;
			case FID_PositionMerge:
			{
				CPositionMergeField PositionMergeField;
				if (pRspBuffer != NULL)
				{
					CPositionMergeField::m_Describe.getSingleField(&PositionMergeField, pRspBuffer);
					pPositionMerge = (MegaPositionMergeStruct*)&PositionMergeField;
				}
				break;
			}
			case -FID_Position:
				mSubjectNo = -mSubjectNo;
			case FID_Position:
			{
				CPositionField PositionField;
				if (pRspBuffer != NULL)
				{
					CPositionField::m_Describe.getSingleField(&PositionField, pRspBuffer);
					pPosition = (MegaPositionStruct*)&PositionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvPositionMerge(pPositionMerge,pPosition, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvDeposit: //用户入金操作应答
	{
		MegaDepositStruct* pDeposit = NULL;
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Deposit:
				mSubjectNo = -mSubjectNo;
			case FID_Deposit:
			{
				CDepositField DepositField;
				if (pRspBuffer != NULL)
				{
					CDepositField::m_Describe.getSingleField(&DepositField, pRspBuffer);
					pDeposit = (MegaDepositStruct*)&DepositField;
				}
				break;
			}
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvDeposit(pDeposit,pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvWithdraw: //用户出金操作应答
	{
		MegaWithdrawStruct* pWithdraw = NULL;
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Withdraw:
				mSubjectNo = -mSubjectNo;
			case FID_Withdraw:
			{
				CWithdrawField WithdrawField;
				if (pRspBuffer != NULL)
				{
					CWithdrawField::m_Describe.getSingleField(&WithdrawField, pRspBuffer);
					pWithdraw = (MegaWithdrawStruct*)&WithdrawField;
				}
				break;
			}
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvWithdraw(pWithdraw,pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvTransfer: //用户转账操作应答
	{
		MegaTransferStruct* pTransfer = NULL;
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Transfer:
				mSubjectNo = -mSubjectNo;
			case FID_Transfer:
			{
				CTransferField TransferField;
				if (pRspBuffer != NULL)
				{
					CTransferField::m_Describe.getSingleField(&TransferField, pRspBuffer);
					pTransfer = (MegaTransferStruct*)&TransferField;
				}
				break;
			}
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvTransfer(pTransfer,pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvUserRegister: //注册用户应答
	{
		MegaUserSessionStruct* pUserSession = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_UserSession:
				mSubjectNo = -mSubjectNo;
			case FID_UserSession:
			{
				CUserSessionField UserSessionField;
				if (pRspBuffer != NULL)
				{
					CUserSessionField::m_Describe.getSingleField(&UserSessionField, pRspBuffer);
					pUserSession = (MegaUserSessionStruct*)&UserSessionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvUserRegister(pUserSession, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvPositionReverse: //反向开仓录入应答
	{
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvPositionReverse(pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvCancelOrderInsert: //避免自成交报单录入应答
	{
		MegaOrderInsertStruct* pOrderInsert = NULL;
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_OrderInsert:
				mSubjectNo = -mSubjectNo;
			case FID_OrderInsert:
			{
				COrderInsertField OrderInsertField;
				if (pRspBuffer != NULL)
				{
					COrderInsertField::m_Describe.getSingleField(&OrderInsertField, pRspBuffer);
					pOrderInsert = (MegaOrderInsertStruct*)&OrderInsertField;
				}
				break;
			}
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvCancelOrderInsert(pOrderInsert,pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvPrivateDispatch: //私有主题分类应答
	{
		MegaPrivateDispatchStruct* pPrivateDispatch = NULL;
		MegaMemberStruct* pMember = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_PrivateDispatch:
				mSubjectNo = -mSubjectNo;
			case FID_PrivateDispatch:
			{
				CPrivateDispatchField PrivateDispatchField;
				if (pRspBuffer != NULL)
				{
					CPrivateDispatchField::m_Describe.getSingleField(&PrivateDispatchField, pRspBuffer);
					pPrivateDispatch = (MegaPrivateDispatchStruct*)&PrivateDispatchField;
				}
				break;
			}
			case -FID_Member:
				mSubjectNo = -mSubjectNo;
			case FID_Member:
			{
				CMemberField MemberField;
				if (pRspBuffer != NULL)
				{
					CMemberField::m_Describe.getSingleField(&MemberField, pRspBuffer);
					pMember = (MegaMemberStruct*)&MemberField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvPrivateDispatch(pPrivateDispatch,pMember, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvPositionLeverage: //调整多空杠杠应答
	{
		MegaPositionActionStruct* pPositionAction = NULL;
		MegaPositionStruct* pPosition = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_PositionAction:
				mSubjectNo = -mSubjectNo;
			case FID_PositionAction:
			{
				CPositionActionField PositionActionField;
				if (pRspBuffer != NULL)
				{
					CPositionActionField::m_Describe.getSingleField(&PositionActionField, pRspBuffer);
					pPositionAction = (MegaPositionActionStruct*)&PositionActionField;
				}
				break;
			}
			case -FID_Position:
				mSubjectNo = -mSubjectNo;
			case FID_Position:
			{
				CPositionField PositionField;
				if (pRspBuffer != NULL)
				{
					CPositionField::m_Describe.getSingleField(&PositionField, pRspBuffer);
					pPosition = (MegaPositionStruct*)&PositionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvPositionLeverage(pPositionAction,pPosition, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvInstrumentUpdate: //设置合约应答
	{
		MegaInstrumentUpdateStruct* pInstrumentUpdate = NULL;
		MegaInstrumentStruct* pInstrument = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_InstrumentUpdate:
				mSubjectNo = -mSubjectNo;
			case FID_InstrumentUpdate:
			{
				CInstrumentUpdateField InstrumentUpdateField;
				if (pRspBuffer != NULL)
				{
					CInstrumentUpdateField::m_Describe.getSingleField(&InstrumentUpdateField, pRspBuffer);
					pInstrumentUpdate = (MegaInstrumentUpdateStruct*)&InstrumentUpdateField;
				}
				break;
			}
			case -FID_Instrument:
				mSubjectNo = -mSubjectNo;
			case FID_Instrument:
			{
				CInstrumentField InstrumentField;
				if (pRspBuffer != NULL)
				{
					CInstrumentField::m_Describe.getSingleField(&InstrumentField, pRspBuffer);
					pInstrument = (MegaInstrumentStruct*)&InstrumentField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvInstrumentUpdate(pInstrumentUpdate,pInstrument, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvChangeCurrency: //充币兑换应答
	{
		MegaAccountStruct* pAccount = NULL;
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Account:
				mSubjectNo = -mSubjectNo;
			case FID_Account:
			{
				CAccountField AccountField;
				if (pRspBuffer != NULL)
				{
					CAccountField::m_Describe.getSingleField(&AccountField, pRspBuffer);
					pAccount = (MegaAccountStruct*)&AccountField;
				}
				break;
			}
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvChangeCurrency(pAccount,pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvWalletNonce: //钱包登录获取随机数应答
	{
		MegaWalletNonceStruct* pWalletNonce = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_WalletNonce:
				mSubjectNo = -mSubjectNo;
			case FID_WalletNonce:
			{
				CWalletNonceField WalletNonceField;
				if (pRspBuffer != NULL)
				{
					CWalletNonceField::m_Describe.getSingleField(&WalletNonceField, pRspBuffer);
					pWalletNonce = (MegaWalletNonceStruct*)&WalletNonceField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvWalletNonce(pWalletNonce, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvWalletLogin: //钱包登录获取随机数应答
	{
		MegaUserSessionStruct* pUserSession = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_UserSession:
				mSubjectNo = -mSubjectNo;
			case FID_UserSession:
			{
				CUserSessionField UserSessionField;
				if (pRspBuffer != NULL)
				{
					CUserSessionField::m_Describe.getSingleField(&UserSessionField, pRspBuffer);
					pUserSession = (MegaUserSessionStruct*)&UserSessionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvWalletLogin(pUserSession, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryAccount: //资金查询应答
	{
		MegaAccountStruct* pAccount = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Account:
				mSubjectNo = -mSubjectNo;
			case FID_Account:
			{
				CAccountField AccountField;
				if (pRspBuffer != NULL)
				{
					CAccountField::m_Describe.getSingleField(&AccountField, pRspBuffer);
					pAccount = (MegaAccountStruct*)&AccountField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryAccount(pAccount, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryOrder: //报单查询应答
	{
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryOrder(pOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryMemberPosition: //持仓汇总查询应答
	{
		MegaMemberPositionStruct* pMemberPosition = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MemberPosition:
				mSubjectNo = -mSubjectNo;
			case FID_MemberPosition:
			{
				CMemberPositionField MemberPositionField;
				if (pRspBuffer != NULL)
				{
					CMemberPositionField::m_Describe.getSingleField(&MemberPositionField, pRspBuffer);
					pMemberPosition = (MegaMemberPositionStruct*)&MemberPositionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryMemberPosition(pMemberPosition, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryPosition: //持仓查询应答
	{
		MegaPositionStruct* pPosition = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Position:
				mSubjectNo = -mSubjectNo;
			case FID_Position:
			{
				CPositionField PositionField;
				if (pRspBuffer != NULL)
				{
					CPositionField::m_Describe.getSingleField(&PositionField, pRspBuffer);
					pPosition = (MegaPositionStruct*)&PositionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryPosition(pPosition, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryInstrument: //标的查询应答
	{
		MegaInstrumentStruct* pInstrument = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Instrument:
				mSubjectNo = -mSubjectNo;
			case FID_Instrument:
			{
				CInstrumentField InstrumentField;
				if (pRspBuffer != NULL)
				{
					CInstrumentField::m_Describe.getSingleField(&InstrumentField, pRspBuffer);
					pInstrument = (MegaInstrumentStruct*)&InstrumentField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryInstrument(pInstrument, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryCurrency: //币种查询应答
	{
		MegaCurrencyStruct* pCurrency = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Currency:
				mSubjectNo = -mSubjectNo;
			case FID_Currency:
			{
				CCurrencyField CurrencyField;
				if (pRspBuffer != NULL)
				{
					CCurrencyField::m_Describe.getSingleField(&CurrencyField, pRspBuffer);
					pCurrency = (MegaCurrencyStruct*)&CurrencyField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryCurrency(pCurrency, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryMember: //成员查询应答
	{
		MegaMemberStruct* pMember = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Member:
				mSubjectNo = -mSubjectNo;
			case FID_Member:
			{
				CMemberField MemberField;
				if (pRspBuffer != NULL)
				{
					CMemberField::m_Describe.getSingleField(&MemberField, pRspBuffer);
					pMember = (MegaMemberStruct*)&MemberField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryMember(pMember, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryMarketOrder: //价位查询应答
	{
		MegaMarketOrderStruct* pMarketOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MarketOrder:
				mSubjectNo = -mSubjectNo;
			case FID_MarketOrder:
			{
				CMarketOrderField MarketOrderField;
				if (pRspBuffer != NULL)
				{
					CMarketOrderField::m_Describe.getSingleField(&MarketOrderField, pRspBuffer);
					pMarketOrder = (MegaMarketOrderStruct*)&MarketOrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryMarketOrder(pMarketOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryAPIKey: //APIKEY查询应答
	{
		MegaAPIKeyStruct* pAPIKey = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_APIKey:
				mSubjectNo = -mSubjectNo;
			case FID_APIKey:
			{
				CAPIKeyField APIKeyField;
				if (pRspBuffer != NULL)
				{
					CAPIKeyField::m_Describe.getSingleField(&APIKeyField, pRspBuffer);
					pAPIKey = (MegaAPIKeyStruct*)&APIKeyField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryAPIKey(pAPIKey, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryMarginRate: //保证金率查询应答
	{
		MegaMarginRateStruct* pMarginRate = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MarginRate:
				mSubjectNo = -mSubjectNo;
			case FID_MarginRate:
			{
				CMarginRateField MarginRateField;
				if (pRspBuffer != NULL)
				{
					CMarginRateField::m_Describe.getSingleField(&MarginRateField, pRspBuffer);
					pMarginRate = (MegaMarginRateStruct*)&MarginRateField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryMarginRate(pMarginRate, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryFee: //手续费率查询应答
	{
		MegaFeeStruct* pFee = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Fee:
				mSubjectNo = -mSubjectNo;
			case FID_Fee:
			{
				CFeeField FeeField;
				if (pRspBuffer != NULL)
				{
					CFeeField::m_Describe.getSingleField(&FeeField, pRspBuffer);
					pFee = (MegaFeeStruct*)&FeeField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryFee(pFee, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryTriggerOrder: //触发订单查询应答
	{
		MegaTriggerOrderStruct* pTriggerOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_TriggerOrder:
				mSubjectNo = -mSubjectNo;
			case FID_TriggerOrder:
			{
				CTriggerOrderField TriggerOrderField;
				if (pRspBuffer != NULL)
				{
					CTriggerOrderField::m_Describe.getSingleField(&TriggerOrderField, pRspBuffer);
					pTriggerOrder = (MegaTriggerOrderStruct*)&TriggerOrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryTriggerOrder(pTriggerOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryLeverage: //杠杆倍数查询应答
	{
		MegaLeverageStruct* pLeverage = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Leverage:
				mSubjectNo = -mSubjectNo;
			case FID_Leverage:
			{
				CLeverageField LeverageField;
				if (pRspBuffer != NULL)
				{
					CLeverageField::m_Describe.getSingleField(&LeverageField, pRspBuffer);
					pLeverage = (MegaLeverageStruct*)&LeverageField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryLeverage(pLeverage, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryMarketData: //行情查询应答
	{
		MegaMarketDataStruct* pMarketData = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MarketData:
				mSubjectNo = -mSubjectNo;
			case FID_MarketData:
			{
				CMarketDataField MarketDataField;
				if (pRspBuffer != NULL)
				{
					CMarketDataField::m_Describe.getSingleField(&MarketDataField, pRspBuffer);
					pMarketData = (MegaMarketDataStruct*)&MarketDataField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryMarketData(pMarketData, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryTrade: //成交查询应答
	{
		MegaTradeStruct* pTrade = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Trade:
				mSubjectNo = -mSubjectNo;
			case FID_Trade:
			{
				CTradeField TradeField;
				if (pRspBuffer != NULL)
				{
					CTradeField::m_Describe.getSingleField(&TradeField, pRspBuffer);
					pTrade = (MegaTradeStruct*)&TradeField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryTrade(pTrade, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryFinishOrder: //历史委托查询应答
	{
		MegaFinishOrderStruct* pFinishOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_FinishOrder:
				mSubjectNo = -mSubjectNo;
			case FID_FinishOrder:
			{
				CFinishOrderField FinishOrderField;
				if (pRspBuffer != NULL)
				{
					CFinishOrderField::m_Describe.getSingleField(&FinishOrderField, pRspBuffer);
					pFinishOrder = (MegaFinishOrderStruct*)&FinishOrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryFinishOrder(pFinishOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryFinishTriggerOrder: //历史条件单查询应答
	{
		MegaFinishTriggerOrderStruct* pFinishTriggerOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_FinishTriggerOrder:
				mSubjectNo = -mSubjectNo;
			case FID_FinishTriggerOrder:
			{
				CFinishTriggerOrderField FinishTriggerOrderField;
				if (pRspBuffer != NULL)
				{
					CFinishTriggerOrderField::m_Describe.getSingleField(&FinishTriggerOrderField, pRspBuffer);
					pFinishTriggerOrder = (MegaFinishTriggerOrderStruct*)&FinishTriggerOrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryFinishTriggerOrder(pFinishTriggerOrder, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryWithLeverage: //指定杠杆倍数查询应答
	{
		MegaLeverageStruct* pLeverage = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Leverage:
				mSubjectNo = -mSubjectNo;
			case FID_Leverage:
			{
				CLeverageField LeverageField;
				if (pRspBuffer != NULL)
				{
					CLeverageField::m_Describe.getSingleField(&LeverageField, pRspBuffer);
					pLeverage = (MegaLeverageStruct*)&LeverageField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryWithLeverage(pLeverage, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryKLine: //K线查询应答
	{
		MegaKLineBaseStruct* pKLineBase = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_KLineBase:
				mSubjectNo = -mSubjectNo;
			case FID_KLineBase:
			{
				CKLineBaseField KLineBaseField;
				if (pRspBuffer != NULL)
				{
					CKLineBaseField::m_Describe.getSingleField(&KLineBaseField, pRspBuffer);
					pKLineBase = (MegaKLineBaseStruct*)&KLineBaseField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryKLine(pKLineBase, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryLine: //线查询应答
	{
		MegaLineStruct* pLine = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Line:
				mSubjectNo = -mSubjectNo;
			case FID_Line:
			{
				CLineField LineField;
				if (pRspBuffer != NULL)
				{
					CLineField::m_Describe.getSingleField(&LineField, pRspBuffer);
					pLine = (MegaLineStruct*)&LineField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryLine(pLine, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQryAccountDetail: //资金流水查询应答
	{
		MegaAccountDetailStruct* pAccountDetail = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_AccountDetail:
				mSubjectNo = -mSubjectNo;
			case FID_AccountDetail:
			{
				CAccountDetailField AccountDetailField;
				if (pRspBuffer != NULL)
				{
					CAccountDetailField::m_Describe.getSingleField(&AccountDetailField, pRspBuffer);
					pAccountDetail = (MegaAccountDetailStruct*)&AccountDetailField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQryAccountDetail(pAccountDetail, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvQrySettleDetail: //结算流水查询应答
	{
		MegaSettleDetailStruct* pSettleDetail = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_SettleDetail:
				mSubjectNo = -mSubjectNo;
			case FID_SettleDetail:
			{
				CSettleDetailField SettleDetailField;
				if (pRspBuffer != NULL)
				{
					CSettleDetailField::m_Describe.getSingleField(&SettleDetailField, pRspBuffer);
					pSettleDetail = (MegaSettleDetailStruct*)&SettleDetailField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvQrySettleDetail(pSettleDetail, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_RecvCheckToken: //检查TOKEN应答
	{
		MegaCheckTokenStruct* pCheckToken = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_CheckToken:
				mSubjectNo = -mSubjectNo;
			case FID_CheckToken:
			{
				CCheckTokenField CheckTokenField;
				if (pRspBuffer != NULL)
				{
					CCheckTokenField::m_Describe.getSingleField(&CheckTokenField, pRspBuffer);
					pCheckToken = (MegaCheckTokenStruct*)&CheckTokenField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->RecvCheckToken(pCheckToken, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
		break;
	}
	case TID_PushTimeSync: //时间同步通知
	{
		MegaCurrentTimeStruct* pCurrentTime = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_CurrentTime:
				mSubjectNo = -mSubjectNo;
			case FID_CurrentTime:
			{
				CCurrentTimeField CurrentTimeField;
				if (pRspBuffer != NULL)
				{
					CCurrentTimeField::m_Describe.getSingleField(&CurrentTimeField, pRspBuffer);
					pCurrentTime = (MegaCurrentTimeStruct*)&CurrentTimeField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushTimeSync(pCurrentTime, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushCommand: //消息通知
	{
		MegaCommandStruct* pCommand = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Command:
				mSubjectNo = -mSubjectNo;
			case FID_Command:
			{
				CCommandField CommandField;
				if (pRspBuffer != NULL)
				{
					CCommandField::m_Describe.getSingleField(&CommandField, pRspBuffer);
					pCommand = (MegaCommandStruct*)&CommandField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushCommand(pCommand, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushTrade: //成交通知
	{
		MegaTradeStruct* pTrade = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Trade:
				mSubjectNo = -mSubjectNo;
			case FID_Trade:
			{
				CTradeField TradeField;
				if (pRspBuffer != NULL)
				{
					CTradeField::m_Describe.getSingleField(&TradeField, pRspBuffer);
					pTrade = (MegaTradeStruct*)&TradeField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushTrade(pTrade, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushOrder: //报单通知
	{
		MegaOrderStruct* pOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Order:
				mSubjectNo = -mSubjectNo;
			case FID_Order:
			{
				COrderField OrderField;
				if (pRspBuffer != NULL)
				{
					COrderField::m_Describe.getSingleField(&OrderField, pRspBuffer);
					pOrder = (MegaOrderStruct*)&OrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushOrder(pOrder, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushMemberPosition: //持仓汇总通知
	{
		MegaMemberPositionStruct* pMemberPosition = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MemberPosition:
				mSubjectNo = -mSubjectNo;
			case FID_MemberPosition:
			{
				CMemberPositionField MemberPositionField;
				if (pRspBuffer != NULL)
				{
					CMemberPositionField::m_Describe.getSingleField(&MemberPositionField, pRspBuffer);
					pMemberPosition = (MegaMemberPositionStruct*)&MemberPositionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushMemberPosition(pMemberPosition, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushPosition: //持仓通知
	{
		MegaPositionStruct* pPosition = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Position:
				mSubjectNo = -mSubjectNo;
			case FID_Position:
			{
				CPositionField PositionField;
				if (pRspBuffer != NULL)
				{
					CPositionField::m_Describe.getSingleField(&PositionField, pRspBuffer);
					pPosition = (MegaPositionStruct*)&PositionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushPosition(pPosition, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushAccount: //资金通知
	{
		MegaAccountStruct* pAccount = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Account:
				mSubjectNo = -mSubjectNo;
			case FID_Account:
			{
				CAccountField AccountField;
				if (pRspBuffer != NULL)
				{
					CAccountField::m_Describe.getSingleField(&AccountField, pRspBuffer);
					pAccount = (MegaAccountStruct*)&AccountField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushAccount(pAccount, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushMarketTrade: //市场成交通知
	{
		MegaMarketTradeStruct* pMarketTrade = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MarketTrade:
				mSubjectNo = -mSubjectNo;
			case FID_MarketTrade:
			{
				CMarketTradeField MarketTradeField;
				if (pRspBuffer != NULL)
				{
					CMarketTradeField::m_Describe.getSingleField(&MarketTradeField, pRspBuffer);
					pMarketTrade = (MegaMarketTradeStruct*)&MarketTradeField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushMarketTrade(pMarketTrade, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushMarketOrder: //订单簿变化通知
	{
		MegaMarketOrderStruct* pMarketOrder = NULL;
		MegaTickMarketOrderStruct* pTickMarketOrder = NULL;
		MegaCurrentTimeStruct* pCurrentTime = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MarketOrder:
				mSubjectNo = -mSubjectNo;
			case FID_MarketOrder:
			{
				CMarketOrderField MarketOrderField;
				if (pRspBuffer != NULL)
				{
					CMarketOrderField::m_Describe.getSingleField(&MarketOrderField, pRspBuffer);
					pMarketOrder = (MegaMarketOrderStruct*)&MarketOrderField;
				}
				break;
			}
			case -FID_TickMarketOrder:
				mSubjectNo = -mSubjectNo;
			case FID_TickMarketOrder:
			{
				CTickMarketOrderField TickMarketOrderField;
				if (pRspBuffer != NULL)
				{
					CTickMarketOrderField::m_Describe.getSingleField(&TickMarketOrderField, pRspBuffer);
					pTickMarketOrder = (MegaTickMarketOrderStruct*)&TickMarketOrderField;
				}
				break;
			}
			case -FID_CurrentTime:
				mSubjectNo = -mSubjectNo;
			case FID_CurrentTime:
			{
				CCurrentTimeField CurrentTimeField;
				if (pRspBuffer != NULL)
				{
					CCurrentTimeField::m_Describe.getSingleField(&CurrentTimeField, pRspBuffer);
					pCurrentTime = (MegaCurrentTimeStruct*)&CurrentTimeField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushMarketOrder(pMarketOrder,pTickMarketOrder,pCurrentTime, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushKLine: //K线通知
	{
		MegaKLineStruct* pKLine = NULL;
		MegaLastKLineStruct* pLastKLine = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_KLine:
				mSubjectNo = -mSubjectNo;
			case FID_KLine:
			{
				CKLineField KLineField;
				if (pRspBuffer != NULL)
				{
					CKLineField::m_Describe.getSingleField(&KLineField, pRspBuffer);
					pKLine = (MegaKLineStruct*)&KLineField;
				}
				break;
			}
			case -FID_LastKLine:
				mSubjectNo = -mSubjectNo;
			case FID_LastKLine:
			{
				CLastKLineField LastKLineField;
				if (pRspBuffer != NULL)
				{
					CLastKLineField::m_Describe.getSingleField(&LastKLineField, pRspBuffer);
					pLastKLine = (MegaLastKLineStruct*)&LastKLineField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushKLine(pKLine,pLastKLine, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushMarketOverView: //普通行情通知
	{
		MegaMarketOverViewStruct* pMarketOverView = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MarketOverView:
				mSubjectNo = -mSubjectNo;
			case FID_MarketOverView:
			{
				CMarketOverViewField MarketOverViewField;
				if (pRspBuffer != NULL)
				{
					CMarketOverViewField::m_Describe.getSingleField(&MarketOverViewField, pRspBuffer);
					pMarketOverView = (MegaMarketOverViewStruct*)&MarketOverViewField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushMarketOverView(pMarketOverView, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushDelayMarketOrder: //全量延时订单簿变化通知
	{
		MegaMarketOrderStruct* pMarketOrder = NULL;
		MegaCurrentTimeStruct* pCurrentTime = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MarketOrder:
				mSubjectNo = -mSubjectNo;
			case FID_MarketOrder:
			{
				CMarketOrderField MarketOrderField;
				if (pRspBuffer != NULL)
				{
					CMarketOrderField::m_Describe.getSingleField(&MarketOrderField, pRspBuffer);
					pMarketOrder = (MegaMarketOrderStruct*)&MarketOrderField;
				}
				break;
			}
			case -FID_CurrentTime:
				mSubjectNo = -mSubjectNo;
			case FID_CurrentTime:
			{
				CCurrentTimeField CurrentTimeField;
				if (pRspBuffer != NULL)
				{
					CCurrentTimeField::m_Describe.getSingleField(&CurrentTimeField, pRspBuffer);
					pCurrentTime = (MegaCurrentTimeStruct*)&CurrentTimeField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushDelayMarketOrder(pMarketOrder,pCurrentTime, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushFiveDepthMarketData: //五档深度行情通知
	{
		MegaFiveDepthMarketDataStruct* pFiveDepthMarketData = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_FiveDepthMarketData:
				mSubjectNo = -mSubjectNo;
			case FID_FiveDepthMarketData:
			{
				CFiveDepthMarketDataField FiveDepthMarketDataField;
				if (pRspBuffer != NULL)
				{
					CFiveDepthMarketDataField::m_Describe.getSingleField(&FiveDepthMarketDataField, pRspBuffer);
					pFiveDepthMarketData = (MegaFiveDepthMarketDataStruct*)&FiveDepthMarketDataField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushFiveDepthMarketData(pFiveDepthMarketData, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushTenDepthMarketData: //十档深度行情通知
	{
		MegaTenDepthMarketDataStruct* pTenDepthMarketData = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_TenDepthMarketData:
				mSubjectNo = -mSubjectNo;
			case FID_TenDepthMarketData:
			{
				CTenDepthMarketDataField TenDepthMarketDataField;
				if (pRspBuffer != NULL)
				{
					CTenDepthMarketDataField::m_Describe.getSingleField(&TenDepthMarketDataField, pRspBuffer);
					pTenDepthMarketData = (MegaTenDepthMarketDataStruct*)&TenDepthMarketDataField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushTenDepthMarketData(pTenDepthMarketData, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushOptionsMarketData: //期权行情通知
	{
		MegaOptionsMarketDataStruct* pOptionsMarketData = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_OptionsMarketData:
				mSubjectNo = -mSubjectNo;
			case FID_OptionsMarketData:
			{
				COptionsMarketDataField OptionsMarketDataField;
				if (pRspBuffer != NULL)
				{
					COptionsMarketDataField::m_Describe.getSingleField(&OptionsMarketDataField, pRspBuffer);
					pOptionsMarketData = (MegaOptionsMarketDataStruct*)&OptionsMarketDataField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushOptionsMarketData(pOptionsMarketData, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushFee: //手续费通知
	{
		MegaFeeStruct* pFee = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Fee:
				mSubjectNo = -mSubjectNo;
			case FID_Fee:
			{
				CFeeField FeeField;
				if (pRspBuffer != NULL)
				{
					CFeeField::m_Describe.getSingleField(&FeeField, pRspBuffer);
					pFee = (MegaFeeStruct*)&FeeField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushFee(pFee, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushMarginRate: //保证金率通知
	{
		MegaMarginRateStruct* pMarginRate = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_MarginRate:
				mSubjectNo = -mSubjectNo;
			case FID_MarginRate:
			{
				CMarginRateField MarginRateField;
				if (pRspBuffer != NULL)
				{
					CMarginRateField::m_Describe.getSingleField(&MarginRateField, pRspBuffer);
					pMarginRate = (MegaMarginRateStruct*)&MarginRateField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushMarginRate(pMarginRate, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushMember: //成员通知
	{
		MegaMemberStruct* pMember = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Member:
				mSubjectNo = -mSubjectNo;
			case FID_Member:
			{
				CMemberField MemberField;
				if (pRspBuffer != NULL)
				{
					CMemberField::m_Describe.getSingleField(&MemberField, pRspBuffer);
					pMember = (MegaMemberStruct*)&MemberField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushMember(pMember, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushTriggerOrder: //触发单通知
	{
		MegaTriggerOrderStruct* pTriggerOrder = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_TriggerOrder:
				mSubjectNo = -mSubjectNo;
			case FID_TriggerOrder:
			{
				CTriggerOrderField TriggerOrderField;
				if (pRspBuffer != NULL)
				{
					CTriggerOrderField::m_Describe.getSingleField(&TriggerOrderField, pRspBuffer);
					pTriggerOrder = (MegaTriggerOrderStruct*)&TriggerOrderField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushTriggerOrder(pTriggerOrder, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushInstrument: //合约通知
	{
		MegaInstrumentStruct* pInstrument = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Instrument:
				mSubjectNo = -mSubjectNo;
			case FID_Instrument:
			{
				CInstrumentField InstrumentField;
				if (pRspBuffer != NULL)
				{
					CInstrumentField::m_Describe.getSingleField(&InstrumentField, pRspBuffer);
					pInstrument = (MegaInstrumentStruct*)&InstrumentField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushInstrument(pInstrument, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushUserSession: //用户登陆通知
	{
		MegaUserSessionStruct* pUserSession = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_UserSession:
				mSubjectNo = -mSubjectNo;
			case FID_UserSession:
			{
				CUserSessionField UserSessionField;
				if (pRspBuffer != NULL)
				{
					CUserSessionField::m_Describe.getSingleField(&UserSessionField, pRspBuffer);
					pUserSession = (MegaUserSessionStruct*)&UserSessionField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushUserSession(pUserSession, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	case TID_PushCurrency: //币种通知
	{
		MegaCurrencyStruct* pCurrency = NULL;
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			case -FID_Currency:
				mSubjectNo = -mSubjectNo;
			case FID_Currency:
			{
				CCurrencyField CurrencyField;
				if (pRspBuffer != NULL)
				{
					CCurrencyField::m_Describe.getSingleField(&CurrencyField, pRspBuffer);
					pCurrency = (MegaCurrencyStruct*)&CurrencyField;
				}
				break;
			}
		}
		if (m_pCallback != NULL)
			m_pCallback->PushCurrency(pCurrency, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
		break;
	}
	}
	return true;
}

extern "C" COneMegaApi* CreateOneMegaApi()
{
	return new CApiImpl();
}
