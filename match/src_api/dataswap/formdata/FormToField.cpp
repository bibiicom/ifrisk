#include "platform.h"
#include "FormToField.h"
#include "BaseFunction.h"
#include "FieldMap.h"

CFieldDescribe* CFormAnalysis::FormToStruct(const char* packageName, const void *pInput, int nInputLen, void *pOutput, int *nTNo)
{
	CXTPPackageDefineNameMap::iterator iter = g_XTPPackageDefineNameMap.find(packageName);
	if(iter==g_XTPPackageDefineNameMap.end())
	{
		sprintf((char*)pOutput, "[NoPackageName][%s]", packageName);
		return NULL;
	}
	map<string, string> formMap = mapSplit((char *)pInput, nInputLen, '&', '=');
	switch((*iter).second->tid)
	{
	case TID_SendCommand: //一般消息请求
		{
			*nTNo = TID_SendCommand;
			string sField = "";
			sField = "UserID";
			((CCommandField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CCommandField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[Command][UserID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CCommandField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CCommandField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[Command][LocalID][isValid]");
				return NULL;
			}			
			sField = "Command";
			((CCommandField*)pOutput)->Command=formMap["Command"].c_str();
			if (!((CCommandField*)pOutput)->Command.isValid())
			{
				strcpy((char*)pOutput, "[Command][Command][isValid]");
				return NULL;
			}			
			sField = "Content";
			((CCommandField*)pOutput)->Content=formMap["Content"].c_str();
			if (!((CCommandField*)pOutput)->Content.isValid())
			{
				strcpy((char*)pOutput, "[Command][Content][isValid]");
				return NULL;
			}			
			return &CCommandField::m_Describe;
		
		}
	case TID_SendInitLink: //创建上层连接请求
		{
			*nTNo = TID_SendInitLink;
			string sField = "";
			sField = "LinkNo";
			((CLinkField*)pOutput)->LinkNo=formMap["LinkNo"].c_str();
			if (!((CLinkField*)pOutput)->LinkNo.isValid())
			{
				strcpy((char*)pOutput, "[Link][LinkNo][isValid]");
				return NULL;
			}			
			sField = "LinkUserID";
			((CLinkField*)pOutput)->LinkUserID=formMap["LinkUserID"].c_str();
			if (!((CLinkField*)pOutput)->LinkUserID.isValid())
			{
				strcpy((char*)pOutput, "[Link][LinkUserID][isValid]");
				return NULL;
			}			
			sField = "LinkPassword";
			((CLinkField*)pOutput)->LinkPassword=formMap["LinkPassword"].c_str();
			if (!((CLinkField*)pOutput)->LinkPassword.isValid())
			{
				strcpy((char*)pOutput, "[Link][LinkPassword][isValid]");
				return NULL;
			}			
			sField = "EncryptType";
			((CLinkField*)pOutput)->EncryptType=formMap["EncryptType"].c_str();
			if (!((CLinkField*)pOutput)->EncryptType.isValid())
			{
				strcpy((char*)pOutput, "[Link][EncryptType][isValid]");
				return NULL;
			}			
			sField = "LinkDllName";
			((CLinkField*)pOutput)->LinkDllName=formMap["LinkDllName"].c_str();
			if (!((CLinkField*)pOutput)->LinkDllName.isValid())
			{
				strcpy((char*)pOutput, "[Link][LinkDllName][isValid]");
				return NULL;
			}			
			sField = "LinkDllLoad";
			((CLinkField*)pOutput)->LinkDllLoad=formMap["LinkDllLoad"].c_str();
			if (!((CLinkField*)pOutput)->LinkDllLoad.isValid())
			{
				strcpy((char*)pOutput, "[Link][LinkDllLoad][isValid]");
				return NULL;
			}			
			sField = "LinkAddress";
			((CLinkField*)pOutput)->LinkAddress=formMap["LinkAddress"].c_str();
			if (!((CLinkField*)pOutput)->LinkAddress.isValid())
			{
				strcpy((char*)pOutput, "[Link][LinkAddress][isValid]");
				return NULL;
			}			
			sField = "LinkStatus";
			((CLinkField*)pOutput)->LinkStatus=formMap["LinkStatus"].c_str();
			if (!((CLinkField*)pOutput)->LinkStatus.isValid())
			{
				strcpy((char*)pOutput, "[Link][LinkStatus][isValid]");
				return NULL;
			}			
			sField = "IsActive";
			((CLinkField*)pOutput)->IsActive=formMap["IsActive"].c_str();
			if (!((CLinkField*)pOutput)->IsActive.isValid())
			{
				strcpy((char*)pOutput, "[Link][IsActive][isValid]");
				return NULL;
			}			
			sField = "Config";
			((CLinkField*)pOutput)->Config=formMap["Config"].c_str();
			if (!((CLinkField*)pOutput)->Config.isValid())
			{
				strcpy((char*)pOutput, "[Link][Config][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CLinkField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CLinkField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Link][Remark][isValid]");
				return NULL;
			}			
			return &CLinkField::m_Describe;
		
		}
	case TID_SendUserLogin: //用户登录请求
		{
			*nTNo = TID_SendUserLogin;
			string sField = "";
			sField = "UserID";
			((CUserLoginField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CUserLoginField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[UserLogin][UserID][isValid]");
				return NULL;
			}			
			sField = "Password";
			((CUserLoginField*)pOutput)->Password=formMap["Password"].c_str();
			if (!((CUserLoginField*)pOutput)->Password.isValid())
			{
				strcpy((char*)pOutput, "[UserLogin][Password][isValid]");
				return NULL;
			}			
			sField = "UserProductID";
			((CUserLoginField*)pOutput)->UserProductID=formMap["UserProductID"].c_str();
			if (!((CUserLoginField*)pOutput)->UserProductID.isValid())
			{
				strcpy((char*)pOutput, "[UserLogin][UserProductID][isValid]");
				return NULL;
			}			
			sField = "MacAddress";
			((CUserLoginField*)pOutput)->MacAddress=formMap["MacAddress"].c_str();
			if (!((CUserLoginField*)pOutput)->MacAddress.isValid())
			{
				strcpy((char*)pOutput, "[UserLogin][MacAddress][isValid]");
				return NULL;
			}			
			sField = "ClientIPAddress";
			((CUserLoginField*)pOutput)->ClientIPAddress=formMap["ClientIPAddress"].c_str();
			if (!((CUserLoginField*)pOutput)->ClientIPAddress.isValid())
			{
				strcpy((char*)pOutput, "[UserLogin][ClientIPAddress][isValid]");
				return NULL;
			}			
			sField = "HDSerialID";
			((CUserLoginField*)pOutput)->HDSerialID=formMap["HDSerialID"].c_str();
			if (!((CUserLoginField*)pOutput)->HDSerialID.isValid())
			{
				strcpy((char*)pOutput, "[UserLogin][HDSerialID][isValid]");
				return NULL;
			}			
			sField = "AuthCode";
			((CUserLoginField*)pOutput)->AuthCode=formMap["AuthCode"].c_str();
			if (!((CUserLoginField*)pOutput)->AuthCode.isValid())
			{
				strcpy((char*)pOutput, "[UserLogin][AuthCode][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CUserLoginField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CUserLoginField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[UserLogin][APPID][isValid]");
				return NULL;
			}			
			sField = "Token";
			((CUserLoginField*)pOutput)->Token=formMap["Token"].c_str();
			if (!((CUserLoginField*)pOutput)->Token.isValid())
			{
				strcpy((char*)pOutput, "[UserLogin][Token][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CUserLoginField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CUserLoginField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[UserLogin][Remark][isValid]");
				return NULL;
			}			
			return &CUserLoginField::m_Describe;
		
		}
	case TID_SendUserLogout: //用户退出请求
		{
			*nTNo = TID_SendUserLogout;
			string sField = "";
			sField = "UserID";
			((CUserLogoutField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CUserLogoutField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[UserLogout][UserID][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CUserLogoutField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CUserLogoutField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[UserLogout][APPID][isValid]");
				return NULL;
			}			
			sField = "Token";
			((CUserLogoutField*)pOutput)->Token=formMap["Token"].c_str();
			if (!((CUserLogoutField*)pOutput)->Token.isValid())
			{
				strcpy((char*)pOutput, "[UserLogout][Token][isValid]");
				return NULL;
			}			
			return &CUserLogoutField::m_Describe;
		
		}
	case TID_SendOrderInsert: //报单录入请求
		{
			*nTNo = TID_SendOrderInsert;
			string sField = "";
			sField = "MemberID";
			((COrderInsertField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((COrderInsertField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((COrderInsertField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((COrderInsertField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((COrderInsertField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((COrderInsertField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][AccountID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((COrderInsertField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((COrderInsertField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((COrderInsertField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((COrderInsertField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((COrderInsertField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((COrderInsertField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "OrderPriceType";
			((COrderInsertField*)pOutput)->OrderPriceType=formMap["OrderPriceType"].c_str();
			if (!((COrderInsertField*)pOutput)->OrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][OrderPriceType][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((COrderInsertField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((COrderInsertField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Direction][isValid]");
				return NULL;
			}			
			sField = "OffsetFlag";
			((COrderInsertField*)pOutput)->OffsetFlag=formMap["OffsetFlag"].c_str();
			if (!((COrderInsertField*)pOutput)->OffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][OffsetFlag][isValid]");
				return NULL;
			}			
			sField = "Price";
			((COrderInsertField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((COrderInsertField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((COrderInsertField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((COrderInsertField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Volume][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((COrderInsertField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((COrderInsertField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "VolumeMode";
			((COrderInsertField*)pOutput)->VolumeMode=formMap["VolumeMode"].c_str();
			if (!((COrderInsertField*)pOutput)->VolumeMode.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][VolumeMode][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((COrderInsertField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((COrderInsertField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Cost][isValid]");
				return NULL;
			}			
			sField = "OrderType";
			((COrderInsertField*)pOutput)->OrderType=formMap["OrderType"].c_str();
			if (!((COrderInsertField*)pOutput)->OrderType.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][OrderType][isValid]");
				return NULL;
			}			
			sField = "GTDTime";
			((COrderInsertField*)pOutput)->GTDTime=formMap["GTDTime"].c_str();
			if (!((COrderInsertField*)pOutput)->GTDTime.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][GTDTime][isValid]");
				return NULL;
			}			
			sField = "MinVolume";
			((COrderInsertField*)pOutput)->MinVolume=formMap["MinVolume"].c_str();
			if (!((COrderInsertField*)pOutput)->MinVolume.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][MinVolume][isValid]");
				return NULL;
			}			
			sField = "BusinessType";
			((COrderInsertField*)pOutput)->BusinessType=formMap["BusinessType"].c_str();
			if (!((COrderInsertField*)pOutput)->BusinessType.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][BusinessType][isValid]");
				return NULL;
			}			
			sField = "BusinessValue";
			((COrderInsertField*)pOutput)->BusinessValue=formMap["BusinessValue"].c_str();
			if (!((COrderInsertField*)pOutput)->BusinessValue.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][BusinessValue][isValid]");
				return NULL;
			}			
			sField = "CloseOrderID";
			((COrderInsertField*)pOutput)->CloseOrderID=formMap["CloseOrderID"].c_str();
			if (!((COrderInsertField*)pOutput)->CloseOrderID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][CloseOrderID][isValid]");
				return NULL;
			}			
			sField = "IsCrossMargin";
			((COrderInsertField*)pOutput)->IsCrossMargin=formMap["IsCrossMargin"].c_str();
			if (!((COrderInsertField*)pOutput)->IsCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][IsCrossMargin][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((COrderInsertField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((COrderInsertField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Remark][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((COrderInsertField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((COrderInsertField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][OrderID][isValid]");
				return NULL;
			}			
			sField = "CopyMemberID";
			((COrderInsertField*)pOutput)->CopyMemberID=formMap["CopyMemberID"].c_str();
			if (!((COrderInsertField*)pOutput)->CopyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][CopyMemberID][isValid]");
				return NULL;
			}			
			sField = "CopyOrderID";
			((COrderInsertField*)pOutput)->CopyOrderID=formMap["CopyOrderID"].c_str();
			if (!((COrderInsertField*)pOutput)->CopyOrderID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][CopyOrderID][isValid]");
				return NULL;
			}			
			sField = "Leverage";
			((COrderInsertField*)pOutput)->Leverage=formMap["Leverage"].c_str();
			if (!((COrderInsertField*)pOutput)->Leverage.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Leverage][isValid]");
				return NULL;
			}			
			sField = "CopyProfitRate";
			((COrderInsertField*)pOutput)->CopyProfitRate=formMap["CopyProfitRate"].c_str();
			if (!((COrderInsertField*)pOutput)->CopyProfitRate.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][CopyProfitRate][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((COrderInsertField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((COrderInsertField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][APPID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((COrderInsertField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((COrderInsertField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][PositionID][isValid]");
				return NULL;
			}			
			sField = "TriggerPrice";
			((COrderInsertField*)pOutput)->TriggerPrice=formMap["TriggerPrice"].c_str();
			if (!((COrderInsertField*)pOutput)->TriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][TriggerPrice][isValid]");
				return NULL;
			}			
			sField = "Reserve";
			((COrderInsertField*)pOutput)->Reserve=formMap["Reserve"].c_str();
			if (!((COrderInsertField*)pOutput)->Reserve.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Reserve][isValid]");
				return NULL;
			}			
			return &COrderInsertField::m_Describe;
		
		}
	case TID_SendOrderAction: //报单操作请求
		{
			*nTNo = TID_SendOrderAction;
			string sField = "";
			sField = "OrderID";
			((COrderActionField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((COrderActionField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][OrderID][isValid]");
				return NULL;
			}			
			sField = "OrderLocalID";
			((COrderActionField*)pOutput)->OrderLocalID=formMap["OrderLocalID"].c_str();
			if (!((COrderActionField*)pOutput)->OrderLocalID.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][OrderLocalID][isValid]");
				return NULL;
			}			
			sField = "ActionFlag";
			((COrderActionField*)pOutput)->ActionFlag=formMap["ActionFlag"].c_str();
			if (!((COrderActionField*)pOutput)->ActionFlag.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][ActionFlag][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((COrderActionField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((COrderActionField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][LocalID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((COrderActionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((COrderActionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((COrderActionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((COrderActionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][AccountID][isValid]");
				return NULL;
			}			
			sField = "Price";
			((COrderActionField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((COrderActionField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((COrderActionField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((COrderActionField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][Volume][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((COrderActionField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((COrderActionField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][Direction][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((COrderActionField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((COrderActionField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "ProductGroup";
			((COrderActionField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((COrderActionField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((COrderActionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((COrderActionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((COrderActionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((COrderActionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((COrderActionField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((COrderActionField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][Cost][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((COrderActionField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((COrderActionField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[OrderAction][Remark][isValid]");
				return NULL;
			}			
			return &COrderActionField::m_Describe;
		
		}
	case TID_SendTriggerOrderInsert: //触发单录入请求
		{
			*nTNo = TID_SendTriggerOrderInsert;
			string sField = "";
			sField = "MemberID";
			((CTriggerOrderInsertField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CTriggerOrderInsertField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CTriggerOrderInsertField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][AccountID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CTriggerOrderInsertField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CTriggerOrderInsertField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CTriggerOrderInsertField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "OrderPriceType";
			((CTriggerOrderInsertField*)pOutput)->OrderPriceType=formMap["OrderPriceType"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->OrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][OrderPriceType][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((CTriggerOrderInsertField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][Direction][isValid]");
				return NULL;
			}			
			sField = "OffsetFlag";
			((CTriggerOrderInsertField*)pOutput)->OffsetFlag=formMap["OffsetFlag"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->OffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][OffsetFlag][isValid]");
				return NULL;
			}			
			sField = "Price";
			((CTriggerOrderInsertField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CTriggerOrderInsertField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][Volume][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((CTriggerOrderInsertField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "VolumeMode";
			((CTriggerOrderInsertField*)pOutput)->VolumeMode=formMap["VolumeMode"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->VolumeMode.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][VolumeMode][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((CTriggerOrderInsertField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][Cost][isValid]");
				return NULL;
			}			
			sField = "OrderType";
			((CTriggerOrderInsertField*)pOutput)->OrderType=formMap["OrderType"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->OrderType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][OrderType][isValid]");
				return NULL;
			}			
			sField = "GTDTime";
			((CTriggerOrderInsertField*)pOutput)->GTDTime=formMap["GTDTime"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->GTDTime.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][GTDTime][isValid]");
				return NULL;
			}			
			sField = "MinVolume";
			((CTriggerOrderInsertField*)pOutput)->MinVolume=formMap["MinVolume"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->MinVolume.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][MinVolume][isValid]");
				return NULL;
			}			
			sField = "BusinessType";
			((CTriggerOrderInsertField*)pOutput)->BusinessType=formMap["BusinessType"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->BusinessType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][BusinessType][isValid]");
				return NULL;
			}			
			sField = "BusinessValue";
			((CTriggerOrderInsertField*)pOutput)->BusinessValue=formMap["BusinessValue"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->BusinessValue.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][BusinessValue][isValid]");
				return NULL;
			}			
			sField = "CloseOrderID";
			((CTriggerOrderInsertField*)pOutput)->CloseOrderID=formMap["CloseOrderID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->CloseOrderID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][CloseOrderID][isValid]");
				return NULL;
			}			
			sField = "IsCrossMargin";
			((CTriggerOrderInsertField*)pOutput)->IsCrossMargin=formMap["IsCrossMargin"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->IsCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][IsCrossMargin][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CTriggerOrderInsertField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][Remark][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((CTriggerOrderInsertField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][OrderID][isValid]");
				return NULL;
			}			
			sField = "CopyMemberID";
			((CTriggerOrderInsertField*)pOutput)->CopyMemberID=formMap["CopyMemberID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->CopyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][CopyMemberID][isValid]");
				return NULL;
			}			
			sField = "CopyOrderID";
			((CTriggerOrderInsertField*)pOutput)->CopyOrderID=formMap["CopyOrderID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->CopyOrderID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][CopyOrderID][isValid]");
				return NULL;
			}			
			sField = "Leverage";
			((CTriggerOrderInsertField*)pOutput)->Leverage=formMap["Leverage"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->Leverage.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][Leverage][isValid]");
				return NULL;
			}			
			sField = "CopyProfitRate";
			((CTriggerOrderInsertField*)pOutput)->CopyProfitRate=formMap["CopyProfitRate"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->CopyProfitRate.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][CopyProfitRate][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CTriggerOrderInsertField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][APPID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((CTriggerOrderInsertField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][PositionID][isValid]");
				return NULL;
			}			
			sField = "TriggerPrice";
			((CTriggerOrderInsertField*)pOutput)->TriggerPrice=formMap["TriggerPrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->TriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerPrice][isValid]");
				return NULL;
			}			
			sField = "Reserve";
			((CTriggerOrderInsertField*)pOutput)->Reserve=formMap["Reserve"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->Reserve.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][Reserve][isValid]");
				return NULL;
			}			
			sField = "SLPrice";
			((CTriggerOrderInsertField*)pOutput)->SLPrice=formMap["SLPrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->SLPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][SLPrice][isValid]");
				return NULL;
			}			
			sField = "SLTriggerPrice";
			((CTriggerOrderInsertField*)pOutput)->SLTriggerPrice=formMap["SLTriggerPrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->SLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][SLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TPPrice";
			((CTriggerOrderInsertField*)pOutput)->TPPrice=formMap["TPPrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->TPPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][TPPrice][isValid]");
				return NULL;
			}			
			sField = "TPTriggerPrice";
			((CTriggerOrderInsertField*)pOutput)->TPTriggerPrice=formMap["TPTriggerPrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->TPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][TPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "RiskBefore";
			((CTriggerOrderInsertField*)pOutput)->RiskBefore=formMap["RiskBefore"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->RiskBefore.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][RiskBefore][isValid]");
				return NULL;
			}			
			sField = "TriggerOrderType";
			((CTriggerOrderInsertField*)pOutput)->TriggerOrderType=formMap["TriggerOrderType"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->TriggerOrderType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerOrderType][isValid]");
				return NULL;
			}			
			sField = "TriggerDetail";
			((CTriggerOrderInsertField*)pOutput)->TriggerDetail=formMap["TriggerDetail"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->TriggerDetail.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerDetail][isValid]");
				return NULL;
			}			
			sField = "TriggerPriceType";
			((CTriggerOrderInsertField*)pOutput)->TriggerPriceType=formMap["TriggerPriceType"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->TriggerPriceType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerPriceType][isValid]");
				return NULL;
			}			
			sField = "TriggerValue";
			((CTriggerOrderInsertField*)pOutput)->TriggerValue=formMap["TriggerValue"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->TriggerValue.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerValue][isValid]");
				return NULL;
			}			
			sField = "CloseSLPrice";
			((CTriggerOrderInsertField*)pOutput)->CloseSLPrice=formMap["CloseSLPrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->CloseSLPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][CloseSLPrice][isValid]");
				return NULL;
			}			
			sField = "CloseSLTriggerPrice";
			((CTriggerOrderInsertField*)pOutput)->CloseSLTriggerPrice=formMap["CloseSLTriggerPrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->CloseSLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][CloseSLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CloseTPPrice";
			((CTriggerOrderInsertField*)pOutput)->CloseTPPrice=formMap["CloseTPPrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->CloseTPPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][CloseTPPrice][isValid]");
				return NULL;
			}			
			sField = "CloseTPTriggerPrice";
			((CTriggerOrderInsertField*)pOutput)->CloseTPTriggerPrice=formMap["CloseTPTriggerPrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->CloseTPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][CloseTPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CloseOrderPriceType";
			((CTriggerOrderInsertField*)pOutput)->CloseOrderPriceType=formMap["CloseOrderPriceType"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->CloseOrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][CloseOrderPriceType][isValid]");
				return NULL;
			}			
			sField = "ClosePrice";
			((CTriggerOrderInsertField*)pOutput)->ClosePrice=formMap["ClosePrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->ClosePrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][ClosePrice][isValid]");
				return NULL;
			}			
			sField = "CloseTriggerPrice";
			((CTriggerOrderInsertField*)pOutput)->CloseTriggerPrice=formMap["CloseTriggerPrice"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->CloseTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][CloseTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "RelatedOrderID";
			((CTriggerOrderInsertField*)pOutput)->RelatedOrderID=formMap["RelatedOrderID"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->RelatedOrderID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][RelatedOrderID][isValid]");
				return NULL;
			}			
			sField = "ActiveTime";
			((CTriggerOrderInsertField*)pOutput)->ActiveTime=formMap["ActiveTime"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->ActiveTime.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][ActiveTime][isValid]");
				return NULL;
			}			
			sField = "TriggerTime";
			((CTriggerOrderInsertField*)pOutput)->TriggerTime=formMap["TriggerTime"].c_str();
			if (!((CTriggerOrderInsertField*)pOutput)->TriggerTime.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerTime][isValid]");
				return NULL;
			}			
			return &CTriggerOrderInsertField::m_Describe;
		
		}
	case TID_SendTriggerOrderAction: //触发单操作请求
		{
			*nTNo = TID_SendTriggerOrderAction;
			string sField = "";
			sField = "OrderID";
			((CTriggerOrderActionField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][OrderID][isValid]");
				return NULL;
			}			
			sField = "OrderLocalID";
			((CTriggerOrderActionField*)pOutput)->OrderLocalID=formMap["OrderLocalID"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->OrderLocalID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][OrderLocalID][isValid]");
				return NULL;
			}			
			sField = "ActionFlag";
			((CTriggerOrderActionField*)pOutput)->ActionFlag=formMap["ActionFlag"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->ActionFlag.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][ActionFlag][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CTriggerOrderActionField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][LocalID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CTriggerOrderActionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CTriggerOrderActionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][AccountID][isValid]");
				return NULL;
			}			
			sField = "Price";
			((CTriggerOrderActionField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CTriggerOrderActionField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][Volume][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((CTriggerOrderActionField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][Direction][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((CTriggerOrderActionField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "ProductGroup";
			((CTriggerOrderActionField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CTriggerOrderActionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CTriggerOrderActionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((CTriggerOrderActionField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][Cost][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CTriggerOrderActionField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][Remark][isValid]");
				return NULL;
			}			
			sField = "SLPrice";
			((CTriggerOrderActionField*)pOutput)->SLPrice=formMap["SLPrice"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->SLPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][SLPrice][isValid]");
				return NULL;
			}			
			sField = "SLTriggerPrice";
			((CTriggerOrderActionField*)pOutput)->SLTriggerPrice=formMap["SLTriggerPrice"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->SLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][SLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TPPrice";
			((CTriggerOrderActionField*)pOutput)->TPPrice=formMap["TPPrice"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->TPPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][TPPrice][isValid]");
				return NULL;
			}			
			sField = "TPTriggerPrice";
			((CTriggerOrderActionField*)pOutput)->TPTriggerPrice=formMap["TPTriggerPrice"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->TPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][TPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TriggerPrice";
			((CTriggerOrderActionField*)pOutput)->TriggerPrice=formMap["TriggerPrice"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->TriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][TriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TriggerOrderType";
			((CTriggerOrderActionField*)pOutput)->TriggerOrderType=formMap["TriggerOrderType"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->TriggerOrderType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][TriggerOrderType][isValid]");
				return NULL;
			}			
			sField = "TriggerValue";
			((CTriggerOrderActionField*)pOutput)->TriggerValue=formMap["TriggerValue"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->TriggerValue.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][TriggerValue][isValid]");
				return NULL;
			}			
			sField = "TriggerDetail";
			((CTriggerOrderActionField*)pOutput)->TriggerDetail=formMap["TriggerDetail"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->TriggerDetail.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][TriggerDetail][isValid]");
				return NULL;
			}			
			sField = "TriggerPriceType";
			((CTriggerOrderActionField*)pOutput)->TriggerPriceType=formMap["TriggerPriceType"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->TriggerPriceType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][TriggerPriceType][isValid]");
				return NULL;
			}			
			sField = "ErrorNo";
			((CTriggerOrderActionField*)pOutput)->ErrorNo=formMap["ErrorNo"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->ErrorNo.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][ErrorNo][isValid]");
				return NULL;
			}			
			sField = "ErrorMsg";
			((CTriggerOrderActionField*)pOutput)->ErrorMsg=formMap["ErrorMsg"].c_str();
			if (!((CTriggerOrderActionField*)pOutput)->ErrorMsg.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrderAction][ErrorMsg][isValid]");
				return NULL;
			}			
			return &CTriggerOrderActionField::m_Describe;
		
		}
	case TID_SendCloseOrderInsert: //止盈止损报单录入请求
		{
			*nTNo = TID_SendCloseOrderInsert;
			string sField = "";
			sField = "MemberID";
			((CCloseOrderInsertField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CCloseOrderInsertField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CCloseOrderInsertField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][AccountID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CCloseOrderInsertField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CCloseOrderInsertField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CCloseOrderInsertField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "OrderPriceType";
			((CCloseOrderInsertField*)pOutput)->OrderPriceType=formMap["OrderPriceType"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->OrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][OrderPriceType][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((CCloseOrderInsertField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][Direction][isValid]");
				return NULL;
			}			
			sField = "OffsetFlag";
			((CCloseOrderInsertField*)pOutput)->OffsetFlag=formMap["OffsetFlag"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->OffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][OffsetFlag][isValid]");
				return NULL;
			}			
			sField = "Price";
			((CCloseOrderInsertField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CCloseOrderInsertField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][Volume][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((CCloseOrderInsertField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "VolumeMode";
			((CCloseOrderInsertField*)pOutput)->VolumeMode=formMap["VolumeMode"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->VolumeMode.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][VolumeMode][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((CCloseOrderInsertField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][Cost][isValid]");
				return NULL;
			}			
			sField = "OrderType";
			((CCloseOrderInsertField*)pOutput)->OrderType=formMap["OrderType"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->OrderType.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][OrderType][isValid]");
				return NULL;
			}			
			sField = "GTDTime";
			((CCloseOrderInsertField*)pOutput)->GTDTime=formMap["GTDTime"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->GTDTime.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][GTDTime][isValid]");
				return NULL;
			}			
			sField = "MinVolume";
			((CCloseOrderInsertField*)pOutput)->MinVolume=formMap["MinVolume"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->MinVolume.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][MinVolume][isValid]");
				return NULL;
			}			
			sField = "BusinessType";
			((CCloseOrderInsertField*)pOutput)->BusinessType=formMap["BusinessType"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->BusinessType.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][BusinessType][isValid]");
				return NULL;
			}			
			sField = "BusinessValue";
			((CCloseOrderInsertField*)pOutput)->BusinessValue=formMap["BusinessValue"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->BusinessValue.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][BusinessValue][isValid]");
				return NULL;
			}			
			sField = "CloseOrderID";
			((CCloseOrderInsertField*)pOutput)->CloseOrderID=formMap["CloseOrderID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CloseOrderID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CloseOrderID][isValid]");
				return NULL;
			}			
			sField = "IsCrossMargin";
			((CCloseOrderInsertField*)pOutput)->IsCrossMargin=formMap["IsCrossMargin"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->IsCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][IsCrossMargin][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CCloseOrderInsertField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][Remark][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((CCloseOrderInsertField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][OrderID][isValid]");
				return NULL;
			}			
			sField = "CopyMemberID";
			((CCloseOrderInsertField*)pOutput)->CopyMemberID=formMap["CopyMemberID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CopyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CopyMemberID][isValid]");
				return NULL;
			}			
			sField = "CopyOrderID";
			((CCloseOrderInsertField*)pOutput)->CopyOrderID=formMap["CopyOrderID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CopyOrderID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CopyOrderID][isValid]");
				return NULL;
			}			
			sField = "Leverage";
			((CCloseOrderInsertField*)pOutput)->Leverage=formMap["Leverage"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->Leverage.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][Leverage][isValid]");
				return NULL;
			}			
			sField = "CopyProfitRate";
			((CCloseOrderInsertField*)pOutput)->CopyProfitRate=formMap["CopyProfitRate"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CopyProfitRate.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CopyProfitRate][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CCloseOrderInsertField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][APPID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((CCloseOrderInsertField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][PositionID][isValid]");
				return NULL;
			}			
			sField = "TriggerPrice";
			((CCloseOrderInsertField*)pOutput)->TriggerPrice=formMap["TriggerPrice"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->TriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][TriggerPrice][isValid]");
				return NULL;
			}			
			sField = "Reserve";
			((CCloseOrderInsertField*)pOutput)->Reserve=formMap["Reserve"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->Reserve.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][Reserve][isValid]");
				return NULL;
			}			
			sField = "CloseSLPrice";
			((CCloseOrderInsertField*)pOutput)->CloseSLPrice=formMap["CloseSLPrice"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CloseSLPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CloseSLPrice][isValid]");
				return NULL;
			}			
			sField = "SLTriggerPrice";
			((CCloseOrderInsertField*)pOutput)->SLTriggerPrice=formMap["SLTriggerPrice"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->SLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][SLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CloseSLTriggerPrice";
			((CCloseOrderInsertField*)pOutput)->CloseSLTriggerPrice=formMap["CloseSLTriggerPrice"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CloseSLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CloseSLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CloseTPPrice";
			((CCloseOrderInsertField*)pOutput)->CloseTPPrice=formMap["CloseTPPrice"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CloseTPPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CloseTPPrice][isValid]");
				return NULL;
			}			
			sField = "TPTriggerPrice";
			((CCloseOrderInsertField*)pOutput)->TPTriggerPrice=formMap["TPTriggerPrice"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->TPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][TPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CloseTPTriggerPrice";
			((CCloseOrderInsertField*)pOutput)->CloseTPTriggerPrice=formMap["CloseTPTriggerPrice"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CloseTPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CloseTPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CloseOrderPriceType";
			((CCloseOrderInsertField*)pOutput)->CloseOrderPriceType=formMap["CloseOrderPriceType"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CloseOrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CloseOrderPriceType][isValid]");
				return NULL;
			}			
			sField = "ClosePrice";
			((CCloseOrderInsertField*)pOutput)->ClosePrice=formMap["ClosePrice"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->ClosePrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][ClosePrice][isValid]");
				return NULL;
			}			
			sField = "CloseTriggerPrice";
			((CCloseOrderInsertField*)pOutput)->CloseTriggerPrice=formMap["CloseTriggerPrice"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CloseTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CloseTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CloseTriggerPriceType";
			((CCloseOrderInsertField*)pOutput)->CloseTriggerPriceType=formMap["CloseTriggerPriceType"].c_str();
			if (!((CCloseOrderInsertField*)pOutput)->CloseTriggerPriceType.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderInsert][CloseTriggerPriceType][isValid]");
				return NULL;
			}			
			return &CCloseOrderInsertField::m_Describe;
		
		}
	case TID_SendCloseOrderAction: //止盈止损报单操作请求
		{
			*nTNo = TID_SendCloseOrderAction;
			string sField = "";
			sField = "OrderID";
			((CCloseOrderActionField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][OrderID][isValid]");
				return NULL;
			}			
			sField = "OrderLocalID";
			((CCloseOrderActionField*)pOutput)->OrderLocalID=formMap["OrderLocalID"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->OrderLocalID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][OrderLocalID][isValid]");
				return NULL;
			}			
			sField = "ActionFlag";
			((CCloseOrderActionField*)pOutput)->ActionFlag=formMap["ActionFlag"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->ActionFlag.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][ActionFlag][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CCloseOrderActionField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][LocalID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CCloseOrderActionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CCloseOrderActionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][AccountID][isValid]");
				return NULL;
			}			
			sField = "Price";
			((CCloseOrderActionField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CCloseOrderActionField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][Volume][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((CCloseOrderActionField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][Direction][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((CCloseOrderActionField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "ProductGroup";
			((CCloseOrderActionField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CCloseOrderActionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CCloseOrderActionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((CCloseOrderActionField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][Cost][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CCloseOrderActionField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][Remark][isValid]");
				return NULL;
			}			
			sField = "SLPrice";
			((CCloseOrderActionField*)pOutput)->SLPrice=formMap["SLPrice"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->SLPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][SLPrice][isValid]");
				return NULL;
			}			
			sField = "SLTriggerPrice";
			((CCloseOrderActionField*)pOutput)->SLTriggerPrice=formMap["SLTriggerPrice"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->SLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][SLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TPPrice";
			((CCloseOrderActionField*)pOutput)->TPPrice=formMap["TPPrice"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->TPPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][TPPrice][isValid]");
				return NULL;
			}			
			sField = "TPTriggerPrice";
			((CCloseOrderActionField*)pOutput)->TPTriggerPrice=formMap["TPTriggerPrice"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->TPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][TPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TriggerPrice";
			((CCloseOrderActionField*)pOutput)->TriggerPrice=formMap["TriggerPrice"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->TriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][TriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TriggerOrderType";
			((CCloseOrderActionField*)pOutput)->TriggerOrderType=formMap["TriggerOrderType"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->TriggerOrderType.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][TriggerOrderType][isValid]");
				return NULL;
			}			
			sField = "TriggerValue";
			((CCloseOrderActionField*)pOutput)->TriggerValue=formMap["TriggerValue"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->TriggerValue.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][TriggerValue][isValid]");
				return NULL;
			}			
			sField = "TriggerDetail";
			((CCloseOrderActionField*)pOutput)->TriggerDetail=formMap["TriggerDetail"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->TriggerDetail.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][TriggerDetail][isValid]");
				return NULL;
			}			
			sField = "TriggerPriceType";
			((CCloseOrderActionField*)pOutput)->TriggerPriceType=formMap["TriggerPriceType"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->TriggerPriceType.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][TriggerPriceType][isValid]");
				return NULL;
			}			
			sField = "ErrorNo";
			((CCloseOrderActionField*)pOutput)->ErrorNo=formMap["ErrorNo"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->ErrorNo.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][ErrorNo][isValid]");
				return NULL;
			}			
			sField = "ErrorMsg";
			((CCloseOrderActionField*)pOutput)->ErrorMsg=formMap["ErrorMsg"].c_str();
			if (!((CCloseOrderActionField*)pOutput)->ErrorMsg.isValid())
			{
				strcpy((char*)pOutput, "[CloseOrderAction][ErrorMsg][isValid]");
				return NULL;
			}			
			return &CCloseOrderActionField::m_Describe;
		
		}
	case TID_SendQuoteInsert: //报价录入请求
		{
			*nTNo = TID_SendQuoteInsert;
			string sField = "";
			sField = "ExchangeID";
			((CQuoteInsertField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQuoteInsertField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQuoteInsertField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQuoteInsertField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CQuoteInsertField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQuoteInsertField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CQuoteInsertField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CQuoteInsertField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CQuoteInsertField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CQuoteInsertField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][LocalID][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CQuoteInsertField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CQuoteInsertField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][Volume][isValid]");
				return NULL;
			}			
			sField = "BuyOffsetFlag";
			((CQuoteInsertField*)pOutput)->BuyOffsetFlag=formMap["BuyOffsetFlag"].c_str();
			if (!((CQuoteInsertField*)pOutput)->BuyOffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][BuyOffsetFlag][isValid]");
				return NULL;
			}			
			sField = "BuyPrice";
			((CQuoteInsertField*)pOutput)->BuyPrice=formMap["BuyPrice"].c_str();
			if (!((CQuoteInsertField*)pOutput)->BuyPrice.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][BuyPrice][isValid]");
				return NULL;
			}			
			sField = "SellOffsetFlag";
			((CQuoteInsertField*)pOutput)->SellOffsetFlag=formMap["SellOffsetFlag"].c_str();
			if (!((CQuoteInsertField*)pOutput)->SellOffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][SellOffsetFlag][isValid]");
				return NULL;
			}			
			sField = "SellPrice";
			((CQuoteInsertField*)pOutput)->SellPrice=formMap["SellPrice"].c_str();
			if (!((CQuoteInsertField*)pOutput)->SellPrice.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][SellPrice][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CQuoteInsertField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CQuoteInsertField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[QuoteInsert][Remark][isValid]");
				return NULL;
			}			
			return &CQuoteInsertField::m_Describe;
		
		}
	case TID_SendQuoteAction: //报价操作请求
		{
			*nTNo = TID_SendQuoteAction;
			string sField = "";
			sField = "OrderID";
			((CQuoteActionField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CQuoteActionField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][OrderID][isValid]");
				return NULL;
			}			
			sField = "OrderLocalID";
			((CQuoteActionField*)pOutput)->OrderLocalID=formMap["OrderLocalID"].c_str();
			if (!((CQuoteActionField*)pOutput)->OrderLocalID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][OrderLocalID][isValid]");
				return NULL;
			}			
			sField = "ActionFlag";
			((CQuoteActionField*)pOutput)->ActionFlag=formMap["ActionFlag"].c_str();
			if (!((CQuoteActionField*)pOutput)->ActionFlag.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][ActionFlag][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CQuoteActionField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CQuoteActionField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][LocalID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CQuoteActionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQuoteActionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQuoteActionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQuoteActionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][AccountID][isValid]");
				return NULL;
			}			
			sField = "Price";
			((CQuoteActionField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((CQuoteActionField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CQuoteActionField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CQuoteActionField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][Volume][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((CQuoteActionField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((CQuoteActionField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][Direction][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((CQuoteActionField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((CQuoteActionField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "ProductGroup";
			((CQuoteActionField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CQuoteActionField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQuoteActionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQuoteActionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQuoteActionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQuoteActionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((CQuoteActionField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((CQuoteActionField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][Cost][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CQuoteActionField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CQuoteActionField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][Remark][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CQuoteActionField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CQuoteActionField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[QuoteAction][TradeUnitID][isValid]");
				return NULL;
			}			
			return &CQuoteActionField::m_Describe;
		
		}
	case TID_SendTopicAction: //订阅主题请求
		{
			*nTNo = TID_SendTopicAction;
			string sField = "";
			sField = "Action";
			((CTopicActionField*)pOutput)->Action=formMap["Action"].c_str();
			if (!((CTopicActionField*)pOutput)->Action.isValid())
			{
				strcpy((char*)pOutput, "[TopicAction][Action][isValid]");
				return NULL;
			}			
			sField = "TopicID";
			((CTopicActionField*)pOutput)->TopicID=formMap["TopicID"].c_str();
			if (!((CTopicActionField*)pOutput)->TopicID.isValid())
			{
				strcpy((char*)pOutput, "[TopicAction][TopicID][isValid]");
				return NULL;
			}			
			sField = "Index";
			((CTopicActionField*)pOutput)->Index=formMap["Index"].c_str();
			if (!((CTopicActionField*)pOutput)->Index.isValid())
			{
				strcpy((char*)pOutput, "[TopicAction][Index][isValid]");
				return NULL;
			}			
			sField = "ResumeNo";
			((CTopicActionField*)pOutput)->ResumeNo=formMap["ResumeNo"].c_str();
			if (!((CTopicActionField*)pOutput)->ResumeNo.isValid())
			{
				strcpy((char*)pOutput, "[TopicAction][ResumeNo][isValid]");
				return NULL;
			}			
			sField = "BusinessNo";
			((CTopicActionField*)pOutput)->BusinessNo=formMap["BusinessNo"].c_str();
			if (!((CTopicActionField*)pOutput)->BusinessNo.isValid())
			{
				strcpy((char*)pOutput, "[TopicAction][BusinessNo][isValid]");
				return NULL;
			}			
			sField = "Token";
			((CTopicActionField*)pOutput)->Token=formMap["Token"].c_str();
			if (!((CTopicActionField*)pOutput)->Token.isValid())
			{
				strcpy((char*)pOutput, "[TopicAction][Token][isValid]");
				return NULL;
			}			
			sField = "Zip";
			((CTopicActionField*)pOutput)->Zip=formMap["Zip"].c_str();
			if (!((CTopicActionField*)pOutput)->Zip.isValid())
			{
				strcpy((char*)pOutput, "[TopicAction][Zip][isValid]");
				return NULL;
			}			
			sField = "Frequency";
			((CTopicActionField*)pOutput)->Frequency=formMap["Frequency"].c_str();
			if (!((CTopicActionField*)pOutput)->Frequency.isValid())
			{
				strcpy((char*)pOutput, "[TopicAction][Frequency][isValid]");
				return NULL;
			}			
			return &CTopicActionField::m_Describe;
		
		}
	case TID_SendMarketDataUpdate: //设置行情请求
		{
			*nTNo = TID_SendMarketDataUpdate;
			string sField = "";
			sField = "ExchangeID";
			((CMarketDataUpdateField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CMarketDataUpdateField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "MarkedPrice";
			((CMarketDataUpdateField*)pOutput)->MarkedPrice=formMap["MarkedPrice"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->MarkedPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][MarkedPrice][isValid]");
				return NULL;
			}			
			sField = "TheoryPrice";
			((CMarketDataUpdateField*)pOutput)->TheoryPrice=formMap["TheoryPrice"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->TheoryPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][TheoryPrice][isValid]");
				return NULL;
			}			
			sField = "PreSettlementPrice";
			((CMarketDataUpdateField*)pOutput)->PreSettlementPrice=formMap["PreSettlementPrice"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->PreSettlementPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][PreSettlementPrice][isValid]");
				return NULL;
			}			
			sField = "SettlementPrice";
			((CMarketDataUpdateField*)pOutput)->SettlementPrice=formMap["SettlementPrice"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->SettlementPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][SettlementPrice][isValid]");
				return NULL;
			}			
			sField = "UnderlyingPrice";
			((CMarketDataUpdateField*)pOutput)->UnderlyingPrice=formMap["UnderlyingPrice"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->UnderlyingPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][UnderlyingPrice][isValid]");
				return NULL;
			}			
			sField = "PrePositionFeeRate";
			((CMarketDataUpdateField*)pOutput)->PrePositionFeeRate=formMap["PrePositionFeeRate"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->PrePositionFeeRate.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][PrePositionFeeRate][isValid]");
				return NULL;
			}			
			sField = "PositionFeeRate";
			((CMarketDataUpdateField*)pOutput)->PositionFeeRate=formMap["PositionFeeRate"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->PositionFeeRate.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][PositionFeeRate][isValid]");
				return NULL;
			}			
			sField = "InstrumentStatus";
			((CMarketDataUpdateField*)pOutput)->InstrumentStatus=formMap["InstrumentStatus"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->InstrumentStatus.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][InstrumentStatus][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CMarketDataUpdateField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][Volume][isValid]");
				return NULL;
			}			
			sField = "Turnover";
			((CMarketDataUpdateField*)pOutput)->Turnover=formMap["Turnover"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->Turnover.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][Turnover][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CMarketDataUpdateField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CMarketDataUpdateField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[MarketDataUpdate][Remark][isValid]");
				return NULL;
			}			
			return &CMarketDataUpdateField::m_Describe;
		
		}
	case TID_SendPositionAction: //调整持仓请求
		{
			*nTNo = TID_SendPositionAction;
			string sField = "";
			sField = "LocalID";
			((CPositionActionField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CPositionActionField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][LocalID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((CPositionActionField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((CPositionActionField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][PositionID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CPositionActionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CPositionActionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CPositionActionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CPositionActionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "ProductGroup";
			((CPositionActionField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CPositionActionField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CPositionActionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CPositionActionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CPositionActionField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CPositionActionField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CPositionActionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CPositionActionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][AccountID][isValid]");
				return NULL;
			}			
			sField = "PosiDirection";
			((CPositionActionField*)pOutput)->PosiDirection=formMap["PosiDirection"].c_str();
			if (!((CPositionActionField*)pOutput)->PosiDirection.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][PosiDirection][isValid]");
				return NULL;
			}			
			sField = "Amount";
			((CPositionActionField*)pOutput)->Amount=formMap["Amount"].c_str();
			if (!((CPositionActionField*)pOutput)->Amount.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][Amount][isValid]");
				return NULL;
			}			
			sField = "LongLeverage";
			((CPositionActionField*)pOutput)->LongLeverage=formMap["LongLeverage"].c_str();
			if (!((CPositionActionField*)pOutput)->LongLeverage.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][LongLeverage][isValid]");
				return NULL;
			}			
			sField = "ShortLeverage";
			((CPositionActionField*)pOutput)->ShortLeverage=formMap["ShortLeverage"].c_str();
			if (!((CPositionActionField*)pOutput)->ShortLeverage.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][ShortLeverage][isValid]");
				return NULL;
			}			
			sField = "NetLeverage";
			((CPositionActionField*)pOutput)->NetLeverage=formMap["NetLeverage"].c_str();
			if (!((CPositionActionField*)pOutput)->NetLeverage.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][NetLeverage][isValid]");
				return NULL;
			}			
			sField = "ActionType";
			((CPositionActionField*)pOutput)->ActionType=formMap["ActionType"].c_str();
			if (!((CPositionActionField*)pOutput)->ActionType.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][ActionType][isValid]");
				return NULL;
			}			
			sField = "IsCrossMargin";
			((CPositionActionField*)pOutput)->IsCrossMargin=formMap["IsCrossMargin"].c_str();
			if (!((CPositionActionField*)pOutput)->IsCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][IsCrossMargin][isValid]");
				return NULL;
			}			
			sField = "IsAutoAddMargin";
			((CPositionActionField*)pOutput)->IsAutoAddMargin=formMap["IsAutoAddMargin"].c_str();
			if (!((CPositionActionField*)pOutput)->IsAutoAddMargin.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][IsAutoAddMargin][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CPositionActionField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CPositionActionField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][Remark][isValid]");
				return NULL;
			}			
			return &CPositionActionField::m_Describe;
		
		}
	case TID_SendAccountAction: //用户出入金请求
		{
			*nTNo = TID_SendAccountAction;
			string sField = "";
			sField = "ActionLocalID";
			((CAccountActionField*)pOutput)->ActionLocalID=formMap["ActionLocalID"].c_str();
			if (!((CAccountActionField*)pOutput)->ActionLocalID.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][ActionLocalID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CAccountActionField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CAccountActionField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][LocalID][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((CAccountActionField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CAccountActionField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][OrderID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CAccountActionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CAccountActionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CAccountActionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CAccountActionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][AccountID][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CAccountActionField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CAccountActionField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "OtherAccountID";
			((CAccountActionField*)pOutput)->OtherAccountID=formMap["OtherAccountID"].c_str();
			if (!((CAccountActionField*)pOutput)->OtherAccountID.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][OtherAccountID][isValid]");
				return NULL;
			}			
			sField = "OtherSettlementGroup";
			((CAccountActionField*)pOutput)->OtherSettlementGroup=formMap["OtherSettlementGroup"].c_str();
			if (!((CAccountActionField*)pOutput)->OtherSettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][OtherSettlementGroup][isValid]");
				return NULL;
			}			
			sField = "OtherMemberID";
			((CAccountActionField*)pOutput)->OtherMemberID=formMap["OtherMemberID"].c_str();
			if (!((CAccountActionField*)pOutput)->OtherMemberID.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][OtherMemberID][isValid]");
				return NULL;
			}			
			sField = "Amount";
			((CAccountActionField*)pOutput)->Amount=formMap["Amount"].c_str();
			if (!((CAccountActionField*)pOutput)->Amount.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][Amount][isValid]");
				return NULL;
			}			
			sField = "FrozenMoney";
			((CAccountActionField*)pOutput)->FrozenMoney=formMap["FrozenMoney"].c_str();
			if (!((CAccountActionField*)pOutput)->FrozenMoney.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][FrozenMoney][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CAccountActionField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CAccountActionField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][Currency][isValid]");
				return NULL;
			}			
			sField = "ActionType";
			((CAccountActionField*)pOutput)->ActionType=formMap["ActionType"].c_str();
			if (!((CAccountActionField*)pOutput)->ActionType.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][ActionType][isValid]");
				return NULL;
			}			
			sField = "IsReserve";
			((CAccountActionField*)pOutput)->IsReserve=formMap["IsReserve"].c_str();
			if (!((CAccountActionField*)pOutput)->IsReserve.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][IsReserve][isValid]");
				return NULL;
			}			
			sField = "AccountType";
			((CAccountActionField*)pOutput)->AccountType=formMap["AccountType"].c_str();
			if (!((CAccountActionField*)pOutput)->AccountType.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][AccountType][isValid]");
				return NULL;
			}			
			sField = "MarginRateGrade";
			((CAccountActionField*)pOutput)->MarginRateGrade=formMap["MarginRateGrade"].c_str();
			if (!((CAccountActionField*)pOutput)->MarginRateGrade.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][MarginRateGrade][isValid]");
				return NULL;
			}			
			sField = "TradingRightGrade";
			((CAccountActionField*)pOutput)->TradingRightGrade=formMap["TradingRightGrade"].c_str();
			if (!((CAccountActionField*)pOutput)->TradingRightGrade.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][TradingRightGrade][isValid]");
				return NULL;
			}			
			sField = "Price";
			((CAccountActionField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((CAccountActionField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CAccountActionField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CAccountActionField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][Volume][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CAccountActionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CAccountActionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CAccountActionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CAccountActionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Source";
			((CAccountActionField*)pOutput)->Source=formMap["Source"].c_str();
			if (!((CAccountActionField*)pOutput)->Source.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][Source][isValid]");
				return NULL;
			}			
			sField = "RelatedID";
			((CAccountActionField*)pOutput)->RelatedID=formMap["RelatedID"].c_str();
			if (!((CAccountActionField*)pOutput)->RelatedID.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][RelatedID][isValid]");
				return NULL;
			}			
			sField = "FeeDeduceRate";
			((CAccountActionField*)pOutput)->FeeDeduceRate=formMap["FeeDeduceRate"].c_str();
			if (!((CAccountActionField*)pOutput)->FeeDeduceRate.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][FeeDeduceRate][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CAccountActionField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CAccountActionField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[AccountAction][Remark][isValid]");
				return NULL;
			}			
			return &CAccountActionField::m_Describe;
		
		}
	case TID_SendMemberAction: //成员设置请求
		{
			*nTNo = TID_SendMemberAction;
			string sField = "";
			sField = "LocalID";
			((CMemberActionField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CMemberActionField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[MemberAction][LocalID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CMemberActionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CMemberActionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[MemberAction][MemberID][isValid]");
				return NULL;
			}			
			sField = "RiskLevel";
			((CMemberActionField*)pOutput)->RiskLevel=formMap["RiskLevel"].c_str();
			if (!((CMemberActionField*)pOutput)->RiskLevel.isValid())
			{
				strcpy((char*)pOutput, "[MemberAction][RiskLevel][isValid]");
				return NULL;
			}			
			sField = "PositionType";
			((CMemberActionField*)pOutput)->PositionType=formMap["PositionType"].c_str();
			if (!((CMemberActionField*)pOutput)->PositionType.isValid())
			{
				strcpy((char*)pOutput, "[MemberAction][PositionType][isValid]");
				return NULL;
			}			
			return &CMemberActionField::m_Describe;
		
		}
	case TID_SendRobotOrder: //内部做市商申报请求
		{
			*nTNo = TID_SendRobotOrder;
			string sField = "";
			sField = "MemberID";
			((CRobotOrderField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CRobotOrderField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CRobotOrderField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CRobotOrderField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CRobotOrderField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CRobotOrderField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][AccountID][isValid]");
				return NULL;
			}			
			sField = "UserID";
			((CRobotOrderField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CRobotOrderField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][UserID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CRobotOrderField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CRobotOrderField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CRobotOrderField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CRobotOrderField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CRobotOrderField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CRobotOrderField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "OrderPriceType";
			((CRobotOrderField*)pOutput)->OrderPriceType=formMap["OrderPriceType"].c_str();
			if (!((CRobotOrderField*)pOutput)->OrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][OrderPriceType][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((CRobotOrderField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((CRobotOrderField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][Direction][isValid]");
				return NULL;
			}			
			sField = "OffsetFlag";
			((CRobotOrderField*)pOutput)->OffsetFlag=formMap["OffsetFlag"].c_str();
			if (!((CRobotOrderField*)pOutput)->OffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][OffsetFlag][isValid]");
				return NULL;
			}			
			sField = "Price";
			((CRobotOrderField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((CRobotOrderField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CRobotOrderField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CRobotOrderField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][Volume][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((CRobotOrderField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((CRobotOrderField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[RobotOrder][VolumeDisplay][isValid]");
				return NULL;
			}			
			return &CRobotOrderField::m_Describe;
		
		}
	case TID_SendOTCTradeInsert: //场外成交申报请求
		{
			*nTNo = TID_SendOTCTradeInsert;
			string sField = "";
			sField = "LocalID";
			((COTCTradeInsertField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((COTCTradeInsertField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((COTCTradeInsertField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Price";
			((COTCTradeInsertField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((COTCTradeInsertField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][Volume][isValid]");
				return NULL;
			}			
			sField = "BuyMemberID";
			((COTCTradeInsertField*)pOutput)->BuyMemberID=formMap["BuyMemberID"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->BuyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][BuyMemberID][isValid]");
				return NULL;
			}			
			sField = "BuyAccountID";
			((COTCTradeInsertField*)pOutput)->BuyAccountID=formMap["BuyAccountID"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->BuyAccountID.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][BuyAccountID][isValid]");
				return NULL;
			}			
			sField = "BuyTradeUnitID";
			((COTCTradeInsertField*)pOutput)->BuyTradeUnitID=formMap["BuyTradeUnitID"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->BuyTradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][BuyTradeUnitID][isValid]");
				return NULL;
			}			
			sField = "BuyOffsetFlag";
			((COTCTradeInsertField*)pOutput)->BuyOffsetFlag=formMap["BuyOffsetFlag"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->BuyOffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][BuyOffsetFlag][isValid]");
				return NULL;
			}			
			sField = "SellMemberID";
			((COTCTradeInsertField*)pOutput)->SellMemberID=formMap["SellMemberID"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->SellMemberID.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][SellMemberID][isValid]");
				return NULL;
			}			
			sField = "SellAccountID";
			((COTCTradeInsertField*)pOutput)->SellAccountID=formMap["SellAccountID"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->SellAccountID.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][SellAccountID][isValid]");
				return NULL;
			}			
			sField = "SellTradeUnitID";
			((COTCTradeInsertField*)pOutput)->SellTradeUnitID=formMap["SellTradeUnitID"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->SellTradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][SellTradeUnitID][isValid]");
				return NULL;
			}			
			sField = "SellOffsetFlag";
			((COTCTradeInsertField*)pOutput)->SellOffsetFlag=formMap["SellOffsetFlag"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->SellOffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][SellOffsetFlag][isValid]");
				return NULL;
			}			
			sField = "OTCType";
			((COTCTradeInsertField*)pOutput)->OTCType=formMap["OTCType"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->OTCType.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][OTCType][isValid]");
				return NULL;
			}			
			sField = "TakerDirection";
			((COTCTradeInsertField*)pOutput)->TakerDirection=formMap["TakerDirection"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->TakerDirection.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][TakerDirection][isValid]");
				return NULL;
			}			
			sField = "TriggerPrice";
			((COTCTradeInsertField*)pOutput)->TriggerPrice=formMap["TriggerPrice"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->TriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][TriggerPrice][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((COTCTradeInsertField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((COTCTradeInsertField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[OTCTradeInsert][Remark][isValid]");
				return NULL;
			}			
			return &COTCTradeInsertField::m_Describe;
		
		}
	case TID_SendSettlement: //资金结算请求
		{
			*nTNo = TID_SendSettlement;
			string sField = "";
			sField = "APPID";
			((CSettlementField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CSettlementField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][APPID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CSettlementField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CSettlementField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CSettlementField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CSettlementField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CSettlementField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CSettlementField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CSettlementField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CSettlementField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "SettleAction";
			((CSettlementField*)pOutput)->SettleAction=formMap["SettleAction"].c_str();
			if (!((CSettlementField*)pOutput)->SettleAction.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][SettleAction][isValid]");
				return NULL;
			}			
			sField = "Value";
			((CSettlementField*)pOutput)->Value=formMap["Value"].c_str();
			if (!((CSettlementField*)pOutput)->Value.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][Value][isValid]");
				return NULL;
			}			
			sField = "Value1";
			((CSettlementField*)pOutput)->Value1=formMap["Value1"].c_str();
			if (!((CSettlementField*)pOutput)->Value1.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][Value1][isValid]");
				return NULL;
			}			
			sField = "Value2";
			((CSettlementField*)pOutput)->Value2=formMap["Value2"].c_str();
			if (!((CSettlementField*)pOutput)->Value2.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][Value2][isValid]");
				return NULL;
			}			
			sField = "Value3";
			((CSettlementField*)pOutput)->Value3=formMap["Value3"].c_str();
			if (!((CSettlementField*)pOutput)->Value3.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][Value3][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CSettlementField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CSettlementField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Settlement][Remark][isValid]");
				return NULL;
			}			
			return &CSettlementField::m_Describe;
		
		}
	case TID_SendClearMarket: //清理行情数据请求
		{
			*nTNo = TID_SendClearMarket;
			string sField = "";
			sField = "ExchangeID";
			((CClearMarketField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CClearMarketField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[ClearMarket][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CClearMarketField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CClearMarketField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[ClearMarket][InstrumentID][isValid]");
				return NULL;
			}			
			return &CClearMarketField::m_Describe;
		
		}
	case TID_SendClearLastKLine: //清理最新K线请求
		{
			*nTNo = TID_SendClearLastKLine;
			string sField = "";
			sField = "ExchangeID";
			((CClearLastKLineField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CClearLastKLineField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[ClearLastKLine][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CClearLastKLineField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CClearLastKLineField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[ClearLastKLine][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Bar";
			((CClearLastKLineField*)pOutput)->Bar=formMap["Bar"].c_str();
			if (!((CClearLastKLineField*)pOutput)->Bar.isValid())
			{
				strcpy((char*)pOutput, "[ClearLastKLine][Bar][isValid]");
				return NULL;
			}			
			return &CClearLastKLineField::m_Describe;
		
		}
	case TID_SendResetLocalID: //重置LocalID请求
		{
			*nTNo = TID_SendResetLocalID;
			string sField = "";
			sField = "Token";
			((CResetLocalIDField*)pOutput)->Token=formMap["Token"].c_str();
			if (!((CResetLocalIDField*)pOutput)->Token.isValid())
			{
				strcpy((char*)pOutput, "[ResetLocalID][Token][isValid]");
				return NULL;
			}			
			sField = "UserID";
			((CResetLocalIDField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CResetLocalIDField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[ResetLocalID][UserID][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CResetLocalIDField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CResetLocalIDField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[ResetLocalID][APPID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CResetLocalIDField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CResetLocalIDField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[ResetLocalID][MemberID][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CResetLocalIDField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CResetLocalIDField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[ResetLocalID][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CResetLocalIDField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CResetLocalIDField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[ResetLocalID][AccountID][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CResetLocalIDField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CResetLocalIDField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[ResetLocalID][Currency][isValid]");
				return NULL;
			}			
			return &CResetLocalIDField::m_Describe;
		
		}
	case TID_SendOTokenInsert: //OToken录入请求
		{
			*nTNo = TID_SendOTokenInsert;
			string sField = "";
			sField = "UserID";
			((COTokenInsertField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((COTokenInsertField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[OTokenInsert][UserID][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((COTokenInsertField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((COTokenInsertField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[OTokenInsert][APPID][isValid]");
				return NULL;
			}			
			sField = "Token";
			((COTokenInsertField*)pOutput)->Token=formMap["Token"].c_str();
			if (!((COTokenInsertField*)pOutput)->Token.isValid())
			{
				strcpy((char*)pOutput, "[OTokenInsert][Token][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((COTokenInsertField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((COTokenInsertField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[OTokenInsert][MemberID][isValid]");
				return NULL;
			}			
			sField = "LoginTime";
			((COTokenInsertField*)pOutput)->LoginTime=formMap["LoginTime"].c_str();
			if (!((COTokenInsertField*)pOutput)->LoginTime.isValid())
			{
				strcpy((char*)pOutput, "[OTokenInsert][LoginTime][isValid]");
				return NULL;
			}			
			sField = "ExpireTime";
			((COTokenInsertField*)pOutput)->ExpireTime=formMap["ExpireTime"].c_str();
			if (!((COTokenInsertField*)pOutput)->ExpireTime.isValid())
			{
				strcpy((char*)pOutput, "[OTokenInsert][ExpireTime][isValid]");
				return NULL;
			}			
			sField = "AccessLimit";
			((COTokenInsertField*)pOutput)->AccessLimit=formMap["AccessLimit"].c_str();
			if (!((COTokenInsertField*)pOutput)->AccessLimit.isValid())
			{
				strcpy((char*)pOutput, "[OTokenInsert][AccessLimit][isValid]");
				return NULL;
			}			
			return &COTokenInsertField::m_Describe;
		
		}
	case TID_SendOTokenAction: //OToken操作请求
		{
			*nTNo = TID_SendOTokenAction;
			string sField = "";
			sField = "UserID";
			((COTokenActionField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((COTokenActionField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[OTokenAction][UserID][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((COTokenActionField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((COTokenActionField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[OTokenAction][APPID][isValid]");
				return NULL;
			}			
			sField = "ActionFlag";
			((COTokenActionField*)pOutput)->ActionFlag=formMap["ActionFlag"].c_str();
			if (!((COTokenActionField*)pOutput)->ActionFlag.isValid())
			{
				strcpy((char*)pOutput, "[OTokenAction][ActionFlag][isValid]");
				return NULL;
			}			
			return &COTokenActionField::m_Describe;
		
		}
	case TID_SendAPIKeyInsert: //APIKEY录入请求
		{
			*nTNo = TID_SendAPIKeyInsert;
			string sField = "";
			sField = "Name";
			((CAPIKeyInsertField*)pOutput)->Name=formMap["Name"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->Name.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][Name][isValid]");
				return NULL;
			}			
			sField = "UserID";
			((CAPIKeyInsertField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][UserID][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CAPIKeyInsertField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][APPID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CAPIKeyInsertField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccessKey";
			((CAPIKeyInsertField*)pOutput)->AccessKey=formMap["AccessKey"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->AccessKey.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][AccessKey][isValid]");
				return NULL;
			}			
			sField = "SecretKey";
			((CAPIKeyInsertField*)pOutput)->SecretKey=formMap["SecretKey"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->SecretKey.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][SecretKey][isValid]");
				return NULL;
			}			
			sField = "Auth";
			((CAPIKeyInsertField*)pOutput)->Auth=formMap["Auth"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->Auth.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][Auth][isValid]");
				return NULL;
			}			
			sField = "AccessLimit";
			((CAPIKeyInsertField*)pOutput)->AccessLimit=formMap["AccessLimit"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->AccessLimit.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][AccessLimit][isValid]");
				return NULL;
			}			
			sField = "CreateTime";
			((CAPIKeyInsertField*)pOutput)->CreateTime=formMap["CreateTime"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->CreateTime.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][CreateTime][isValid]");
				return NULL;
			}			
			sField = "ExpireTime";
			((CAPIKeyInsertField*)pOutput)->ExpireTime=formMap["ExpireTime"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->ExpireTime.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][ExpireTime][isValid]");
				return NULL;
			}			
			sField = "IPAddress";
			((CAPIKeyInsertField*)pOutput)->IPAddress=formMap["IPAddress"].c_str();
			if (!((CAPIKeyInsertField*)pOutput)->IPAddress.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyInsert][IPAddress][isValid]");
				return NULL;
			}			
			return &CAPIKeyInsertField::m_Describe;
		
		}
	case TID_SendAPIKeyAction: //APIKEY操作请求
		{
			*nTNo = TID_SendAPIKeyAction;
			string sField = "";
			sField = "Name";
			((CAPIKeyActionField*)pOutput)->Name=formMap["Name"].c_str();
			if (!((CAPIKeyActionField*)pOutput)->Name.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyAction][Name][isValid]");
				return NULL;
			}			
			sField = "AccessKey";
			((CAPIKeyActionField*)pOutput)->AccessKey=formMap["AccessKey"].c_str();
			if (!((CAPIKeyActionField*)pOutput)->AccessKey.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyAction][AccessKey][isValid]");
				return NULL;
			}			
			sField = "Auth";
			((CAPIKeyActionField*)pOutput)->Auth=formMap["Auth"].c_str();
			if (!((CAPIKeyActionField*)pOutput)->Auth.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyAction][Auth][isValid]");
				return NULL;
			}			
			sField = "AccessLimit";
			((CAPIKeyActionField*)pOutput)->AccessLimit=formMap["AccessLimit"].c_str();
			if (!((CAPIKeyActionField*)pOutput)->AccessLimit.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyAction][AccessLimit][isValid]");
				return NULL;
			}			
			sField = "IPAddress";
			((CAPIKeyActionField*)pOutput)->IPAddress=formMap["IPAddress"].c_str();
			if (!((CAPIKeyActionField*)pOutput)->IPAddress.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyAction][IPAddress][isValid]");
				return NULL;
			}			
			sField = "ExpireTime";
			((CAPIKeyActionField*)pOutput)->ExpireTime=formMap["ExpireTime"].c_str();
			if (!((CAPIKeyActionField*)pOutput)->ExpireTime.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyAction][ExpireTime][isValid]");
				return NULL;
			}			
			sField = "ActionFlag";
			((CAPIKeyActionField*)pOutput)->ActionFlag=formMap["ActionFlag"].c_str();
			if (!((CAPIKeyActionField*)pOutput)->ActionFlag.isValid())
			{
				strcpy((char*)pOutput, "[APIKeyAction][ActionFlag][isValid]");
				return NULL;
			}			
			return &CAPIKeyActionField::m_Describe;
		
		}
	case TID_SendPositionMerge: //仓位合并操作请求
		{
			*nTNo = TID_SendPositionMerge;
			string sField = "";
			sField = "LocalID";
			((CPositionMergeField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CPositionMergeField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][LocalID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CPositionMergeField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CPositionMergeField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][MemberID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((CPositionMergeField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID][isValid]");
				return NULL;
			}			
			sField = "PositionID1";
			((CPositionMergeField*)pOutput)->PositionID1=formMap["PositionID1"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID1.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID1][isValid]");
				return NULL;
			}			
			sField = "PositionID2";
			((CPositionMergeField*)pOutput)->PositionID2=formMap["PositionID2"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID2.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID2][isValid]");
				return NULL;
			}			
			sField = "PositionID3";
			((CPositionMergeField*)pOutput)->PositionID3=formMap["PositionID3"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID3.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID3][isValid]");
				return NULL;
			}			
			sField = "PositionID4";
			((CPositionMergeField*)pOutput)->PositionID4=formMap["PositionID4"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID4.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID4][isValid]");
				return NULL;
			}			
			sField = "PositionID5";
			((CPositionMergeField*)pOutput)->PositionID5=formMap["PositionID5"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID5.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID5][isValid]");
				return NULL;
			}			
			sField = "PositionID6";
			((CPositionMergeField*)pOutput)->PositionID6=formMap["PositionID6"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID6.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID6][isValid]");
				return NULL;
			}			
			sField = "PositionID7";
			((CPositionMergeField*)pOutput)->PositionID7=formMap["PositionID7"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID7.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID7][isValid]");
				return NULL;
			}			
			sField = "PositionID8";
			((CPositionMergeField*)pOutput)->PositionID8=formMap["PositionID8"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID8.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID8][isValid]");
				return NULL;
			}			
			sField = "PositionID9";
			((CPositionMergeField*)pOutput)->PositionID9=formMap["PositionID9"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID9.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID9][isValid]");
				return NULL;
			}			
			sField = "PositionID10";
			((CPositionMergeField*)pOutput)->PositionID10=formMap["PositionID10"].c_str();
			if (!((CPositionMergeField*)pOutput)->PositionID10.isValid())
			{
				strcpy((char*)pOutput, "[PositionMerge][PositionID10][isValid]");
				return NULL;
			}			
			return &CPositionMergeField::m_Describe;
		
		}
	case TID_SendDeposit: //用户入金操作请求
		{
			*nTNo = TID_SendDeposit;
			string sField = "";
			sField = "LocalID";
			((CDepositField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CDepositField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[Deposit][LocalID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CDepositField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CDepositField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[Deposit][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CDepositField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CDepositField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[Deposit][AccountID][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CDepositField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CDepositField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Deposit][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "Amount";
			((CDepositField*)pOutput)->Amount=formMap["Amount"].c_str();
			if (!((CDepositField*)pOutput)->Amount.isValid())
			{
				strcpy((char*)pOutput, "[Deposit][Amount][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CDepositField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CDepositField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[Deposit][Currency][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CDepositField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CDepositField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Deposit][Remark][isValid]");
				return NULL;
			}			
			return &CDepositField::m_Describe;
		
		}
	case TID_SendWithdraw: //用户出金操作请求
		{
			*nTNo = TID_SendWithdraw;
			string sField = "";
			sField = "LocalID";
			((CWithdrawField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CWithdrawField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[Withdraw][LocalID][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((CWithdrawField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CWithdrawField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[Withdraw][OrderID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CWithdrawField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CWithdrawField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[Withdraw][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CWithdrawField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CWithdrawField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[Withdraw][AccountID][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CWithdrawField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CWithdrawField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Withdraw][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "OtherSettlementGroup";
			((CWithdrawField*)pOutput)->OtherSettlementGroup=formMap["OtherSettlementGroup"].c_str();
			if (!((CWithdrawField*)pOutput)->OtherSettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Withdraw][OtherSettlementGroup][isValid]");
				return NULL;
			}			
			sField = "Amount";
			((CWithdrawField*)pOutput)->Amount=formMap["Amount"].c_str();
			if (!((CWithdrawField*)pOutput)->Amount.isValid())
			{
				strcpy((char*)pOutput, "[Withdraw][Amount][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CWithdrawField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CWithdrawField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[Withdraw][Currency][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CWithdrawField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CWithdrawField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Withdraw][Remark][isValid]");
				return NULL;
			}			
			return &CWithdrawField::m_Describe;
		
		}
	case TID_SendTransfer: //用户转账操作请求
		{
			*nTNo = TID_SendTransfer;
			string sField = "";
			sField = "LocalID";
			((CTransferField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CTransferField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[Transfer][LocalID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CTransferField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CTransferField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[Transfer][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CTransferField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CTransferField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[Transfer][AccountID][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CTransferField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CTransferField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Transfer][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "OtherAccountID";
			((CTransferField*)pOutput)->OtherAccountID=formMap["OtherAccountID"].c_str();
			if (!((CTransferField*)pOutput)->OtherAccountID.isValid())
			{
				strcpy((char*)pOutput, "[Transfer][OtherAccountID][isValid]");
				return NULL;
			}			
			sField = "OtherSettlementGroup";
			((CTransferField*)pOutput)->OtherSettlementGroup=formMap["OtherSettlementGroup"].c_str();
			if (!((CTransferField*)pOutput)->OtherSettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Transfer][OtherSettlementGroup][isValid]");
				return NULL;
			}			
			sField = "OtherMemberID";
			((CTransferField*)pOutput)->OtherMemberID=formMap["OtherMemberID"].c_str();
			if (!((CTransferField*)pOutput)->OtherMemberID.isValid())
			{
				strcpy((char*)pOutput, "[Transfer][OtherMemberID][isValid]");
				return NULL;
			}			
			sField = "Amount";
			((CTransferField*)pOutput)->Amount=formMap["Amount"].c_str();
			if (!((CTransferField*)pOutput)->Amount.isValid())
			{
				strcpy((char*)pOutput, "[Transfer][Amount][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CTransferField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CTransferField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[Transfer][Currency][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CTransferField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CTransferField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Transfer][Remark][isValid]");
				return NULL;
			}			
			return &CTransferField::m_Describe;
		
		}
	case TID_SendUserRegister: //注册用户请求
		{
			*nTNo = TID_SendUserRegister;
			string sField = "";
			sField = "UserID";
			((CUserSessionField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CUserSessionField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][UserID][isValid]");
				return NULL;
			}			
			sField = "Password";
			((CUserSessionField*)pOutput)->Password=formMap["Password"].c_str();
			if (!((CUserSessionField*)pOutput)->Password.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][Password][isValid]");
				return NULL;
			}			
			sField = "UserProductID";
			((CUserSessionField*)pOutput)->UserProductID=formMap["UserProductID"].c_str();
			if (!((CUserSessionField*)pOutput)->UserProductID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][UserProductID][isValid]");
				return NULL;
			}			
			sField = "MacAddress";
			((CUserSessionField*)pOutput)->MacAddress=formMap["MacAddress"].c_str();
			if (!((CUserSessionField*)pOutput)->MacAddress.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][MacAddress][isValid]");
				return NULL;
			}			
			sField = "ClientIPAddress";
			((CUserSessionField*)pOutput)->ClientIPAddress=formMap["ClientIPAddress"].c_str();
			if (!((CUserSessionField*)pOutput)->ClientIPAddress.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][ClientIPAddress][isValid]");
				return NULL;
			}			
			sField = "HDSerialID";
			((CUserSessionField*)pOutput)->HDSerialID=formMap["HDSerialID"].c_str();
			if (!((CUserSessionField*)pOutput)->HDSerialID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][HDSerialID][isValid]");
				return NULL;
			}			
			sField = "AuthCode";
			((CUserSessionField*)pOutput)->AuthCode=formMap["AuthCode"].c_str();
			if (!((CUserSessionField*)pOutput)->AuthCode.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][AuthCode][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CUserSessionField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CUserSessionField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][APPID][isValid]");
				return NULL;
			}			
			sField = "Token";
			((CUserSessionField*)pOutput)->Token=formMap["Token"].c_str();
			if (!((CUserSessionField*)pOutput)->Token.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][Token][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CUserSessionField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CUserSessionField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][Remark][isValid]");
				return NULL;
			}			
			sField = "APIID";
			((CUserSessionField*)pOutput)->APIID=formMap["APIID"].c_str();
			if (!((CUserSessionField*)pOutput)->APIID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][APIID][isValid]");
				return NULL;
			}			
			sField = "LoginTime";
			((CUserSessionField*)pOutput)->LoginTime=formMap["LoginTime"].c_str();
			if (!((CUserSessionField*)pOutput)->LoginTime.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][LoginTime][isValid]");
				return NULL;
			}			
			sField = "ExpireTime";
			((CUserSessionField*)pOutput)->ExpireTime=formMap["ExpireTime"].c_str();
			if (!((CUserSessionField*)pOutput)->ExpireTime.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][ExpireTime][isValid]");
				return NULL;
			}			
			sField = "IPAddress";
			((CUserSessionField*)pOutput)->IPAddress=formMap["IPAddress"].c_str();
			if (!((CUserSessionField*)pOutput)->IPAddress.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][IPAddress][isValid]");
				return NULL;
			}			
			sField = "MaxLocalID";
			((CUserSessionField*)pOutput)->MaxLocalID=formMap["MaxLocalID"].c_str();
			if (!((CUserSessionField*)pOutput)->MaxLocalID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][MaxLocalID][isValid]");
				return NULL;
			}			
			sField = "SessionNo";
			((CUserSessionField*)pOutput)->SessionNo=formMap["SessionNo"].c_str();
			if (!((CUserSessionField*)pOutput)->SessionNo.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][SessionNo][isValid]");
				return NULL;
			}			
			sField = "FrontNo";
			((CUserSessionField*)pOutput)->FrontNo=formMap["FrontNo"].c_str();
			if (!((CUserSessionField*)pOutput)->FrontNo.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][FrontNo][isValid]");
				return NULL;
			}			
			sField = "AccessLimit";
			((CUserSessionField*)pOutput)->AccessLimit=formMap["AccessLimit"].c_str();
			if (!((CUserSessionField*)pOutput)->AccessLimit.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][AccessLimit][isValid]");
				return NULL;
			}			
			sField = "UserType";
			((CUserSessionField*)pOutput)->UserType=formMap["UserType"].c_str();
			if (!((CUserSessionField*)pOutput)->UserType.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][UserType][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CUserSessionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CUserSessionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][MemberID][isValid]");
				return NULL;
			}			
			sField = "LimitAccesses";
			((CUserSessionField*)pOutput)->LimitAccesses=formMap["LimitAccesses"].c_str();
			if (!((CUserSessionField*)pOutput)->LimitAccesses.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][LimitAccesses][isValid]");
				return NULL;
			}			
			sField = "SinkType";
			((CUserSessionField*)pOutput)->SinkType=formMap["SinkType"].c_str();
			if (!((CUserSessionField*)pOutput)->SinkType.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][SinkType][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CUserSessionField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CUserSessionField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CUserSessionField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CUserSessionField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][UpdateTime][isValid]");
				return NULL;
			}			
			return &CUserSessionField::m_Describe;
		
		}
	case TID_SendPositionReverse: //反向开仓录入请求
		{
			*nTNo = TID_SendPositionReverse;
			string sField = "";
			sField = "LocalID";
			((CPositionReverseField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CPositionReverseField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[PositionReverse][LocalID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CPositionReverseField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CPositionReverseField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[PositionReverse][MemberID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((CPositionReverseField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((CPositionReverseField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[PositionReverse][PositionID][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CPositionReverseField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CPositionReverseField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[PositionReverse][Volume][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CPositionReverseField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CPositionReverseField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[PositionReverse][Remark][isValid]");
				return NULL;
			}			
			sField = "CopyMemberID";
			((CPositionReverseField*)pOutput)->CopyMemberID=formMap["CopyMemberID"].c_str();
			if (!((CPositionReverseField*)pOutput)->CopyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[PositionReverse][CopyMemberID][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CPositionReverseField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CPositionReverseField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[PositionReverse][APPID][isValid]");
				return NULL;
			}			
			return &CPositionReverseField::m_Describe;
		
		}
	case TID_SendCancelOrderInsert: //避免自成交报单录入请求
		{
			*nTNo = TID_SendCancelOrderInsert;
			string sField = "";
			sField = "MemberID";
			((COrderInsertField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((COrderInsertField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((COrderInsertField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((COrderInsertField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((COrderInsertField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((COrderInsertField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][AccountID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((COrderInsertField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((COrderInsertField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((COrderInsertField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((COrderInsertField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((COrderInsertField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((COrderInsertField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "OrderPriceType";
			((COrderInsertField*)pOutput)->OrderPriceType=formMap["OrderPriceType"].c_str();
			if (!((COrderInsertField*)pOutput)->OrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][OrderPriceType][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((COrderInsertField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((COrderInsertField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Direction][isValid]");
				return NULL;
			}			
			sField = "OffsetFlag";
			((COrderInsertField*)pOutput)->OffsetFlag=formMap["OffsetFlag"].c_str();
			if (!((COrderInsertField*)pOutput)->OffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][OffsetFlag][isValid]");
				return NULL;
			}			
			sField = "Price";
			((COrderInsertField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((COrderInsertField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((COrderInsertField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((COrderInsertField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Volume][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((COrderInsertField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((COrderInsertField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "VolumeMode";
			((COrderInsertField*)pOutput)->VolumeMode=formMap["VolumeMode"].c_str();
			if (!((COrderInsertField*)pOutput)->VolumeMode.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][VolumeMode][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((COrderInsertField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((COrderInsertField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Cost][isValid]");
				return NULL;
			}			
			sField = "OrderType";
			((COrderInsertField*)pOutput)->OrderType=formMap["OrderType"].c_str();
			if (!((COrderInsertField*)pOutput)->OrderType.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][OrderType][isValid]");
				return NULL;
			}			
			sField = "GTDTime";
			((COrderInsertField*)pOutput)->GTDTime=formMap["GTDTime"].c_str();
			if (!((COrderInsertField*)pOutput)->GTDTime.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][GTDTime][isValid]");
				return NULL;
			}			
			sField = "MinVolume";
			((COrderInsertField*)pOutput)->MinVolume=formMap["MinVolume"].c_str();
			if (!((COrderInsertField*)pOutput)->MinVolume.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][MinVolume][isValid]");
				return NULL;
			}			
			sField = "BusinessType";
			((COrderInsertField*)pOutput)->BusinessType=formMap["BusinessType"].c_str();
			if (!((COrderInsertField*)pOutput)->BusinessType.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][BusinessType][isValid]");
				return NULL;
			}			
			sField = "BusinessValue";
			((COrderInsertField*)pOutput)->BusinessValue=formMap["BusinessValue"].c_str();
			if (!((COrderInsertField*)pOutput)->BusinessValue.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][BusinessValue][isValid]");
				return NULL;
			}			
			sField = "CloseOrderID";
			((COrderInsertField*)pOutput)->CloseOrderID=formMap["CloseOrderID"].c_str();
			if (!((COrderInsertField*)pOutput)->CloseOrderID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][CloseOrderID][isValid]");
				return NULL;
			}			
			sField = "IsCrossMargin";
			((COrderInsertField*)pOutput)->IsCrossMargin=formMap["IsCrossMargin"].c_str();
			if (!((COrderInsertField*)pOutput)->IsCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][IsCrossMargin][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((COrderInsertField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((COrderInsertField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Remark][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((COrderInsertField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((COrderInsertField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][OrderID][isValid]");
				return NULL;
			}			
			sField = "CopyMemberID";
			((COrderInsertField*)pOutput)->CopyMemberID=formMap["CopyMemberID"].c_str();
			if (!((COrderInsertField*)pOutput)->CopyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][CopyMemberID][isValid]");
				return NULL;
			}			
			sField = "CopyOrderID";
			((COrderInsertField*)pOutput)->CopyOrderID=formMap["CopyOrderID"].c_str();
			if (!((COrderInsertField*)pOutput)->CopyOrderID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][CopyOrderID][isValid]");
				return NULL;
			}			
			sField = "Leverage";
			((COrderInsertField*)pOutput)->Leverage=formMap["Leverage"].c_str();
			if (!((COrderInsertField*)pOutput)->Leverage.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Leverage][isValid]");
				return NULL;
			}			
			sField = "CopyProfitRate";
			((COrderInsertField*)pOutput)->CopyProfitRate=formMap["CopyProfitRate"].c_str();
			if (!((COrderInsertField*)pOutput)->CopyProfitRate.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][CopyProfitRate][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((COrderInsertField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((COrderInsertField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][APPID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((COrderInsertField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((COrderInsertField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][PositionID][isValid]");
				return NULL;
			}			
			sField = "TriggerPrice";
			((COrderInsertField*)pOutput)->TriggerPrice=formMap["TriggerPrice"].c_str();
			if (!((COrderInsertField*)pOutput)->TriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][TriggerPrice][isValid]");
				return NULL;
			}			
			sField = "Reserve";
			((COrderInsertField*)pOutput)->Reserve=formMap["Reserve"].c_str();
			if (!((COrderInsertField*)pOutput)->Reserve.isValid())
			{
				strcpy((char*)pOutput, "[OrderInsert][Reserve][isValid]");
				return NULL;
			}			
			return &COrderInsertField::m_Describe;
		
		}
	case TID_SendPrivateDispatch: //私有主题分类请求
		{
			*nTNo = TID_SendPrivateDispatch;
			string sField = "";
			sField = "TopicID";
			((CPrivateDispatchField*)pOutput)->TopicID=formMap["TopicID"].c_str();
			if (!((CPrivateDispatchField*)pOutput)->TopicID.isValid())
			{
				strcpy((char*)pOutput, "[PrivateDispatch][TopicID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CPrivateDispatchField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CPrivateDispatchField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[PrivateDispatch][MemberID][isValid]");
				return NULL;
			}			
			sField = "PrivateBase";
			((CPrivateDispatchField*)pOutput)->PrivateBase=formMap["PrivateBase"].c_str();
			if (!((CPrivateDispatchField*)pOutput)->PrivateBase.isValid())
			{
				strcpy((char*)pOutput, "[PrivateDispatch][PrivateBase][isValid]");
				return NULL;
			}			
			return &CPrivateDispatchField::m_Describe;
		
		}
	case TID_SendPositionLeverage: //调整多空杠杠请求
		{
			*nTNo = TID_SendPositionLeverage;
			string sField = "";
			sField = "LocalID";
			((CPositionActionField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CPositionActionField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][LocalID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((CPositionActionField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((CPositionActionField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][PositionID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CPositionActionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CPositionActionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CPositionActionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CPositionActionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "ProductGroup";
			((CPositionActionField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CPositionActionField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CPositionActionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CPositionActionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CPositionActionField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CPositionActionField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CPositionActionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CPositionActionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][AccountID][isValid]");
				return NULL;
			}			
			sField = "PosiDirection";
			((CPositionActionField*)pOutput)->PosiDirection=formMap["PosiDirection"].c_str();
			if (!((CPositionActionField*)pOutput)->PosiDirection.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][PosiDirection][isValid]");
				return NULL;
			}			
			sField = "Amount";
			((CPositionActionField*)pOutput)->Amount=formMap["Amount"].c_str();
			if (!((CPositionActionField*)pOutput)->Amount.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][Amount][isValid]");
				return NULL;
			}			
			sField = "LongLeverage";
			((CPositionActionField*)pOutput)->LongLeverage=formMap["LongLeverage"].c_str();
			if (!((CPositionActionField*)pOutput)->LongLeverage.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][LongLeverage][isValid]");
				return NULL;
			}			
			sField = "ShortLeverage";
			((CPositionActionField*)pOutput)->ShortLeverage=formMap["ShortLeverage"].c_str();
			if (!((CPositionActionField*)pOutput)->ShortLeverage.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][ShortLeverage][isValid]");
				return NULL;
			}			
			sField = "NetLeverage";
			((CPositionActionField*)pOutput)->NetLeverage=formMap["NetLeverage"].c_str();
			if (!((CPositionActionField*)pOutput)->NetLeverage.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][NetLeverage][isValid]");
				return NULL;
			}			
			sField = "ActionType";
			((CPositionActionField*)pOutput)->ActionType=formMap["ActionType"].c_str();
			if (!((CPositionActionField*)pOutput)->ActionType.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][ActionType][isValid]");
				return NULL;
			}			
			sField = "IsCrossMargin";
			((CPositionActionField*)pOutput)->IsCrossMargin=formMap["IsCrossMargin"].c_str();
			if (!((CPositionActionField*)pOutput)->IsCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][IsCrossMargin][isValid]");
				return NULL;
			}			
			sField = "IsAutoAddMargin";
			((CPositionActionField*)pOutput)->IsAutoAddMargin=formMap["IsAutoAddMargin"].c_str();
			if (!((CPositionActionField*)pOutput)->IsAutoAddMargin.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][IsAutoAddMargin][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CPositionActionField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CPositionActionField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[PositionAction][Remark][isValid]");
				return NULL;
			}			
			return &CPositionActionField::m_Describe;
		
		}
	case TID_SendInstrumentUpdate: //设置合约请求
		{
			*nTNo = TID_SendInstrumentUpdate;
			string sField = "";
			sField = "ExchangeID";
			((CInstrumentUpdateField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CInstrumentUpdateField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "MinOrderVolume";
			((CInstrumentUpdateField*)pOutput)->MinOrderVolume=formMap["MinOrderVolume"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->MinOrderVolume.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][MinOrderVolume][isValid]");
				return NULL;
			}			
			sField = "MinOrderCost";
			((CInstrumentUpdateField*)pOutput)->MinOrderCost=formMap["MinOrderCost"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->MinOrderCost.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][MinOrderCost][isValid]");
				return NULL;
			}			
			sField = "MaxOrderVolume";
			((CInstrumentUpdateField*)pOutput)->MaxOrderVolume=formMap["MaxOrderVolume"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->MaxOrderVolume.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][MaxOrderVolume][isValid]");
				return NULL;
			}			
			sField = "PriceTick";
			((CInstrumentUpdateField*)pOutput)->PriceTick=formMap["PriceTick"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->PriceTick.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][PriceTick][isValid]");
				return NULL;
			}			
			sField = "VolumeTick";
			((CInstrumentUpdateField*)pOutput)->VolumeTick=formMap["VolumeTick"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->VolumeTick.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][VolumeTick][isValid]");
				return NULL;
			}			
			sField = "MaxOpenInterest";
			((CInstrumentUpdateField*)pOutput)->MaxOpenInterest=formMap["MaxOpenInterest"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->MaxOpenInterest.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][MaxOpenInterest][isValid]");
				return NULL;
			}			
			sField = "BasisPrice";
			((CInstrumentUpdateField*)pOutput)->BasisPrice=formMap["BasisPrice"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->BasisPrice.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][BasisPrice][isValid]");
				return NULL;
			}			
			sField = "UpperLimitPrice";
			((CInstrumentUpdateField*)pOutput)->UpperLimitPrice=formMap["UpperLimitPrice"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->UpperLimitPrice.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][UpperLimitPrice][isValid]");
				return NULL;
			}			
			sField = "LowerLimitPrice";
			((CInstrumentUpdateField*)pOutput)->LowerLimitPrice=formMap["LowerLimitPrice"].c_str();
			if (!((CInstrumentUpdateField*)pOutput)->LowerLimitPrice.isValid())
			{
				strcpy((char*)pOutput, "[InstrumentUpdate][LowerLimitPrice][isValid]");
				return NULL;
			}			
			return &CInstrumentUpdateField::m_Describe;
		
		}
	case TID_SendChangeCurrency: //充币兑换请求
		{
			*nTNo = TID_SendChangeCurrency;
			string sField = "";
			sField = "MemberID";
			((CChangeCurrencyField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CChangeCurrencyField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[ChangeCurrency][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CChangeCurrencyField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CChangeCurrencyField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[ChangeCurrency][AccountID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CChangeCurrencyField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CChangeCurrencyField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[ChangeCurrency][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CChangeCurrencyField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CChangeCurrencyField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[ChangeCurrency][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID1";
			((CChangeCurrencyField*)pOutput)->InstrumentID1=formMap["InstrumentID1"].c_str();
			if (!((CChangeCurrencyField*)pOutput)->InstrumentID1.isValid())
			{
				strcpy((char*)pOutput, "[ChangeCurrency][InstrumentID1][isValid]");
				return NULL;
			}			
			sField = "InstrumentID2";
			((CChangeCurrencyField*)pOutput)->InstrumentID2=formMap["InstrumentID2"].c_str();
			if (!((CChangeCurrencyField*)pOutput)->InstrumentID2.isValid())
			{
				strcpy((char*)pOutput, "[ChangeCurrency][InstrumentID2][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CChangeCurrencyField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CChangeCurrencyField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[ChangeCurrency][Currency][isValid]");
				return NULL;
			}			
			sField = "Amount";
			((CChangeCurrencyField*)pOutput)->Amount=formMap["Amount"].c_str();
			if (!((CChangeCurrencyField*)pOutput)->Amount.isValid())
			{
				strcpy((char*)pOutput, "[ChangeCurrency][Amount][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CChangeCurrencyField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CChangeCurrencyField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[ChangeCurrency][Remark][isValid]");
				return NULL;
			}			
			return &CChangeCurrencyField::m_Describe;
		
		}
	case TID_SendWalletNonce: //钱包登录获取随机数请求
		{
			*nTNo = TID_SendWalletNonce;
			string sField = "";
			sField = "UserID";
			((CWalletNonceField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CWalletNonceField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[WalletNonce][UserID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CWalletNonceField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CWalletNonceField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[WalletNonce][MemberID][isValid]");
				return NULL;
			}			
			sField = "Nonce";
			((CWalletNonceField*)pOutput)->Nonce=formMap["Nonce"].c_str();
			if (!((CWalletNonceField*)pOutput)->Nonce.isValid())
			{
				strcpy((char*)pOutput, "[WalletNonce][Nonce][isValid]");
				return NULL;
			}			
			return &CWalletNonceField::m_Describe;
		
		}
	case TID_SendWalletLogin: //钱包登录获取随机数请求
		{
			*nTNo = TID_SendWalletLogin;
			string sField = "";
			sField = "UserID";
			((CWalletLoginField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CWalletLoginField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[WalletLogin][UserID][isValid]");
				return NULL;
			}			
			sField = "Signature";
			((CWalletLoginField*)pOutput)->Signature=formMap["Signature"].c_str();
			if (!((CWalletLoginField*)pOutput)->Signature.isValid())
			{
				strcpy((char*)pOutput, "[WalletLogin][Signature][isValid]");
				return NULL;
			}			
			return &CWalletLoginField::m_Describe;
		
		}
	case TID_SendQryAccount: //资金查询请求
		{
			*nTNo = TID_SendQryAccount;
			string sField = "";
			sField = "MemberID";
			((CQryAccountField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryAccountField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryAccount][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryAccountField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryAccountField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryAccount][AccountID][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CQryAccountField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CQryAccountField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[QryAccount][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CQryAccountField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CQryAccountField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[QryAccount][Currency][isValid]");
				return NULL;
			}			
			return &CQryAccountField::m_Describe;
		
		}
	case TID_SendQryOrder: //报单查询请求
		{
			*nTNo = TID_SendQryOrder;
			string sField = "";
			sField = "MemberID";
			((CQryOrderField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryOrderField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryOrder][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryOrderField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryOrderField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryOrder][AccountID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryOrderField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryOrderField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryOrder][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryOrderField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryOrderField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryOrder][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CQryOrderField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CQryOrderField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[QryOrder][LocalID][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((CQryOrderField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CQryOrderField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[QryOrder][OrderID][isValid]");
				return NULL;
			}			
			return &CQryOrderField::m_Describe;
		
		}
	case TID_SendQryMemberPosition: //持仓汇总查询请求
		{
			*nTNo = TID_SendQryMemberPosition;
			string sField = "";
			sField = "MemberID";
			((CQryMemberPositionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryMemberPositionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryMemberPosition][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryMemberPositionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryMemberPositionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryMemberPosition][AccountID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryMemberPositionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryMemberPositionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryMemberPosition][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryMemberPositionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryMemberPositionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryMemberPosition][InstrumentID][isValid]");
				return NULL;
			}			
			return &CQryMemberPositionField::m_Describe;
		
		}
	case TID_SendQryPosition: //持仓查询请求
		{
			*nTNo = TID_SendQryPosition;
			string sField = "";
			sField = "MemberID";
			((CQryPositionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryPositionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryPosition][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryPositionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryPositionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryPosition][AccountID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryPositionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryPositionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryPosition][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryPositionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryPositionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryPosition][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "PositionClass";
			((CQryPositionField*)pOutput)->PositionClass=formMap["PositionClass"].c_str();
			if (!((CQryPositionField*)pOutput)->PositionClass.isValid())
			{
				strcpy((char*)pOutput, "[QryPosition][PositionClass][isValid]");
				return NULL;
			}			
			return &CQryPositionField::m_Describe;
		
		}
	case TID_SendQryInstrument: //标的查询请求
		{
			*nTNo = TID_SendQryInstrument;
			string sField = "";
			sField = "ExchangeID";
			((CQryInstrumentField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryInstrumentField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryInstrument][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "ProductGroup";
			((CQryInstrumentField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CQryInstrumentField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[QryInstrument][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryInstrumentField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryInstrumentField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryInstrument][InstrumentID][isValid]");
				return NULL;
			}			
			return &CQryInstrumentField::m_Describe;
		
		}
	case TID_SendQryCurrency: //币种查询请求
		{
			*nTNo = TID_SendQryCurrency;
			string sField = "";
			sField = "SettlementGroup";
			((CQryCurrencyField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CQryCurrencyField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[QryCurrency][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CQryCurrencyField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CQryCurrencyField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[QryCurrency][Currency][isValid]");
				return NULL;
			}			
			return &CQryCurrencyField::m_Describe;
		
		}
	case TID_SendQryMember: //成员查询请求
		{
			*nTNo = TID_SendQryMember;
			string sField = "";
			sField = "MemberType";
			((CQryMemberField*)pOutput)->MemberType=formMap["MemberType"].c_str();
			if (!((CQryMemberField*)pOutput)->MemberType.isValid())
			{
				strcpy((char*)pOutput, "[QryMember][MemberType][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CQryMemberField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryMemberField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryMember][MemberID][isValid]");
				return NULL;
			}			
			return &CQryMemberField::m_Describe;
		
		}
	case TID_SendQryMarketOrder: //价位查询请求
		{
			*nTNo = TID_SendQryMarketOrder;
			string sField = "";
			sField = "ExchangeID";
			((CQryMarketOrderField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryMarketOrderField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryMarketOrder][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryMarketOrderField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryMarketOrderField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryMarketOrder][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((CQryMarketOrderField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((CQryMarketOrderField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[QryMarketOrder][Direction][isValid]");
				return NULL;
			}			
			sField = "Depth";
			((CQryMarketOrderField*)pOutput)->Depth=formMap["Depth"].c_str();
			if (!((CQryMarketOrderField*)pOutput)->Depth.isValid())
			{
				strcpy((char*)pOutput, "[QryMarketOrder][Depth][isValid]");
				return NULL;
			}			
			sField = "Tick";
			((CQryMarketOrderField*)pOutput)->Tick=formMap["Tick"].c_str();
			if (!((CQryMarketOrderField*)pOutput)->Tick.isValid())
			{
				strcpy((char*)pOutput, "[QryMarketOrder][Tick][isValid]");
				return NULL;
			}			
			return &CQryMarketOrderField::m_Describe;
		
		}
	case TID_SendQryAPIKey: //APIKEY查询请求
		{
			*nTNo = TID_SendQryAPIKey;
			string sField = "";
			sField = "UserID";
			((CQryAPIKeyField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CQryAPIKeyField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[QryAPIKey][UserID][isValid]");
				return NULL;
			}			
			return &CQryAPIKeyField::m_Describe;
		
		}
	case TID_SendQryMarginRate: //保证金率查询请求
		{
			*nTNo = TID_SendQryMarginRate;
			string sField = "";
			sField = "MemberID";
			((CQryMarginRateField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryMarginRateField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryMarginRate][MemberID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryMarginRateField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryMarginRateField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryMarginRate][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryMarginRateField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryMarginRateField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryMarginRate][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryMarginRateField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryMarginRateField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryMarginRate][AccountID][isValid]");
				return NULL;
			}			
			return &CQryMarginRateField::m_Describe;
		
		}
	case TID_SendQryFee: //手续费率查询请求
		{
			*nTNo = TID_SendQryFee;
			string sField = "";
			sField = "MemberID";
			((CQryFeeField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryFeeField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryFee][MemberID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryFeeField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryFeeField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryFee][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryFeeField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryFeeField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryFee][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "MatchRole";
			((CQryFeeField*)pOutput)->MatchRole=formMap["MatchRole"].c_str();
			if (!((CQryFeeField*)pOutput)->MatchRole.isValid())
			{
				strcpy((char*)pOutput, "[QryFee][MatchRole][isValid]");
				return NULL;
			}			
			return &CQryFeeField::m_Describe;
		
		}
	case TID_SendQryTriggerOrder: //触发订单查询请求
		{
			*nTNo = TID_SendQryTriggerOrder;
			string sField = "";
			sField = "MemberID";
			((CQryTriggerOrderField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryTriggerOrderField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryTriggerOrder][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryTriggerOrderField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryTriggerOrderField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryTriggerOrder][AccountID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryTriggerOrderField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryTriggerOrderField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryTriggerOrder][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryTriggerOrderField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryTriggerOrderField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryTriggerOrder][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CQryTriggerOrderField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CQryTriggerOrderField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[QryTriggerOrder][LocalID][isValid]");
				return NULL;
			}			
			return &CQryTriggerOrderField::m_Describe;
		
		}
	case TID_SendQryLeverage: //杠杆倍数查询请求
		{
			*nTNo = TID_SendQryLeverage;
			string sField = "";
			sField = "MemberID";
			((CQryLeverageField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryLeverageField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryLeverage][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryLeverageField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryLeverageField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryLeverage][AccountID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CQryLeverageField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CQryLeverageField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[QryLeverage][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryLeverageField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryLeverageField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryLeverage][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryLeverageField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryLeverageField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryLeverage][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "PosiDirection";
			((CQryLeverageField*)pOutput)->PosiDirection=formMap["PosiDirection"].c_str();
			if (!((CQryLeverageField*)pOutput)->PosiDirection.isValid())
			{
				strcpy((char*)pOutput, "[QryLeverage][PosiDirection][isValid]");
				return NULL;
			}			
			return &CQryLeverageField::m_Describe;
		
		}
	case TID_SendQryMarketData: //行情查询请求
		{
			*nTNo = TID_SendQryMarketData;
			string sField = "";
			sField = "ExchangeID";
			((CQryMarketDataField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryMarketDataField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryMarketData][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "ProductGroup";
			((CQryMarketDataField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CQryMarketDataField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[QryMarketData][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryMarketDataField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryMarketDataField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryMarketData][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "From";
			((CQryMarketDataField*)pOutput)->From=formMap["From"].c_str();
			if (!((CQryMarketDataField*)pOutput)->From.isValid())
			{
				strcpy((char*)pOutput, "[QryMarketData][From][isValid]");
				return NULL;
			}			
			sField = "Limit";
			((CQryMarketDataField*)pOutput)->Limit=formMap["Limit"].c_str();
			if (!((CQryMarketDataField*)pOutput)->Limit.isValid())
			{
				strcpy((char*)pOutput, "[QryMarketData][Limit][isValid]");
				return NULL;
			}			
			return &CQryMarketDataField::m_Describe;
		
		}
	case TID_SendQryTrade: //成交查询请求
		{
			*nTNo = TID_SendQryTrade;
			string sField = "";
			sField = "OrderID";
			((CQryTradeField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CQryTradeField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[QryTrade][OrderID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CQryTradeField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryTradeField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryTrade][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryTradeField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryTradeField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryTrade][AccountID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryTradeField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryTradeField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryTrade][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryTradeField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryTradeField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryTrade][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Limit";
			((CQryTradeField*)pOutput)->Limit=formMap["Limit"].c_str();
			if (!((CQryTradeField*)pOutput)->Limit.isValid())
			{
				strcpy((char*)pOutput, "[QryTrade][Limit][isValid]");
				return NULL;
			}			
			sField = "Before";
			((CQryTradeField*)pOutput)->Before=formMap["Before"].c_str();
			if (!((CQryTradeField*)pOutput)->Before.isValid())
			{
				strcpy((char*)pOutput, "[QryTrade][Before][isValid]");
				return NULL;
			}			
			sField = "After";
			((CQryTradeField*)pOutput)->After=formMap["After"].c_str();
			if (!((CQryTradeField*)pOutput)->After.isValid())
			{
				strcpy((char*)pOutput, "[QryTrade][After][isValid]");
				return NULL;
			}			
			sField = "StartTime";
			((CQryTradeField*)pOutput)->StartTime=formMap["StartTime"].c_str();
			if (!((CQryTradeField*)pOutput)->StartTime.isValid())
			{
				strcpy((char*)pOutput, "[QryTrade][StartTime][isValid]");
				return NULL;
			}			
			sField = "EndTime";
			((CQryTradeField*)pOutput)->EndTime=formMap["EndTime"].c_str();
			if (!((CQryTradeField*)pOutput)->EndTime.isValid())
			{
				strcpy((char*)pOutput, "[QryTrade][EndTime][isValid]");
				return NULL;
			}			
			return &CQryTradeField::m_Describe;
		
		}
	case TID_SendQryFinishOrder: //历史委托查询请求
		{
			*nTNo = TID_SendQryFinishOrder;
			string sField = "";
			sField = "MemberID";
			((CQryFinishOrderField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][MemberID][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((CQryFinishOrderField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][OrderID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryFinishOrderField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][AccountID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryFinishOrderField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryFinishOrderField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "OrderStatus";
			((CQryFinishOrderField*)pOutput)->OrderStatus=formMap["OrderStatus"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->OrderStatus.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][OrderStatus][isValid]");
				return NULL;
			}			
			sField = "Limit";
			((CQryFinishOrderField*)pOutput)->Limit=formMap["Limit"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->Limit.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][Limit][isValid]");
				return NULL;
			}			
			sField = "Before";
			((CQryFinishOrderField*)pOutput)->Before=formMap["Before"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->Before.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][Before][isValid]");
				return NULL;
			}			
			sField = "After";
			((CQryFinishOrderField*)pOutput)->After=formMap["After"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->After.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][After][isValid]");
				return NULL;
			}			
			sField = "StartTime";
			((CQryFinishOrderField*)pOutput)->StartTime=formMap["StartTime"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->StartTime.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][StartTime][isValid]");
				return NULL;
			}			
			sField = "EndTime";
			((CQryFinishOrderField*)pOutput)->EndTime=formMap["EndTime"].c_str();
			if (!((CQryFinishOrderField*)pOutput)->EndTime.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishOrder][EndTime][isValid]");
				return NULL;
			}			
			return &CQryFinishOrderField::m_Describe;
		
		}
	case TID_SendQryFinishTriggerOrder: //历史条件单查询请求
		{
			*nTNo = TID_SendQryFinishTriggerOrder;
			string sField = "";
			sField = "MemberID";
			((CQryFinishTriggerOrderField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][MemberID][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((CQryFinishTriggerOrderField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][OrderID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryFinishTriggerOrderField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][AccountID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryFinishTriggerOrderField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryFinishTriggerOrderField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "TriggerStatus";
			((CQryFinishTriggerOrderField*)pOutput)->TriggerStatus=formMap["TriggerStatus"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->TriggerStatus.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][TriggerStatus][isValid]");
				return NULL;
			}			
			sField = "TriggerOrderType";
			((CQryFinishTriggerOrderField*)pOutput)->TriggerOrderType=formMap["TriggerOrderType"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->TriggerOrderType.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][TriggerOrderType][isValid]");
				return NULL;
			}			
			sField = "Limit";
			((CQryFinishTriggerOrderField*)pOutput)->Limit=formMap["Limit"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->Limit.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][Limit][isValid]");
				return NULL;
			}			
			sField = "Before";
			((CQryFinishTriggerOrderField*)pOutput)->Before=formMap["Before"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->Before.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][Before][isValid]");
				return NULL;
			}			
			sField = "After";
			((CQryFinishTriggerOrderField*)pOutput)->After=formMap["After"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->After.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][After][isValid]");
				return NULL;
			}			
			sField = "StartTime";
			((CQryFinishTriggerOrderField*)pOutput)->StartTime=formMap["StartTime"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->StartTime.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][StartTime][isValid]");
				return NULL;
			}			
			sField = "EndTime";
			((CQryFinishTriggerOrderField*)pOutput)->EndTime=formMap["EndTime"].c_str();
			if (!((CQryFinishTriggerOrderField*)pOutput)->EndTime.isValid())
			{
				strcpy((char*)pOutput, "[QryFinishTriggerOrder][EndTime][isValid]");
				return NULL;
			}			
			return &CQryFinishTriggerOrderField::m_Describe;
		
		}
	case TID_SendQryWithLeverage: //指定杠杆倍数查询请求
		{
			*nTNo = TID_SendQryWithLeverage;
			string sField = "";
			sField = "MemberID";
			((CQryWithLeverageField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryWithLeverageField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryWithLeverage][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryWithLeverageField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryWithLeverageField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryWithLeverage][AccountID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CQryWithLeverageField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CQryWithLeverageField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[QryWithLeverage][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CQryWithLeverageField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryWithLeverageField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryWithLeverage][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "PosiDirection";
			((CQryWithLeverageField*)pOutput)->PosiDirection=formMap["PosiDirection"].c_str();
			if (!((CQryWithLeverageField*)pOutput)->PosiDirection.isValid())
			{
				strcpy((char*)pOutput, "[QryWithLeverage][PosiDirection][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryWithLeverageField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryWithLeverageField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryWithLeverage][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "LongLeverage";
			((CQryWithLeverageField*)pOutput)->LongLeverage=formMap["LongLeverage"].c_str();
			if (!((CQryWithLeverageField*)pOutput)->LongLeverage.isValid())
			{
				strcpy((char*)pOutput, "[QryWithLeverage][LongLeverage][isValid]");
				return NULL;
			}			
			sField = "ShortLeverage";
			((CQryWithLeverageField*)pOutput)->ShortLeverage=formMap["ShortLeverage"].c_str();
			if (!((CQryWithLeverageField*)pOutput)->ShortLeverage.isValid())
			{
				strcpy((char*)pOutput, "[QryWithLeverage][ShortLeverage][isValid]");
				return NULL;
			}			
			sField = "NetLeverage";
			((CQryWithLeverageField*)pOutput)->NetLeverage=formMap["NetLeverage"].c_str();
			if (!((CQryWithLeverageField*)pOutput)->NetLeverage.isValid())
			{
				strcpy((char*)pOutput, "[QryWithLeverage][NetLeverage][isValid]");
				return NULL;
			}			
			return &CQryWithLeverageField::m_Describe;
		
		}
	case TID_SendQryKLine: //K线查询请求
		{
			*nTNo = TID_SendQryKLine;
			string sField = "";
			sField = "ExchangeID";
			((CQryKLineField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryKLineField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryKLine][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryKLineField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryKLineField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryKLine][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Bar";
			((CQryKLineField*)pOutput)->Bar=formMap["Bar"].c_str();
			if (!((CQryKLineField*)pOutput)->Bar.isValid())
			{
				strcpy((char*)pOutput, "[QryKLine][Bar][isValid]");
				return NULL;
			}			
			sField = "EndTime";
			((CQryKLineField*)pOutput)->EndTime=formMap["EndTime"].c_str();
			if (!((CQryKLineField*)pOutput)->EndTime.isValid())
			{
				strcpy((char*)pOutput, "[QryKLine][EndTime][isValid]");
				return NULL;
			}			
			sField = "Limit";
			((CQryKLineField*)pOutput)->Limit=formMap["Limit"].c_str();
			if (!((CQryKLineField*)pOutput)->Limit.isValid())
			{
				strcpy((char*)pOutput, "[QryKLine][Limit][isValid]");
				return NULL;
			}			
			return &CQryKLineField::m_Describe;
		
		}
	case TID_SendQryLine: //线查询请求
		{
			*nTNo = TID_SendQryLine;
			string sField = "";
			sField = "ExchangeID";
			((CQryLineField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CQryLineField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[QryLine][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryLineField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryLineField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryLine][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Bar";
			((CQryLineField*)pOutput)->Bar=formMap["Bar"].c_str();
			if (!((CQryLineField*)pOutput)->Bar.isValid())
			{
				strcpy((char*)pOutput, "[QryLine][Bar][isValid]");
				return NULL;
			}			
			sField = "Limit";
			((CQryLineField*)pOutput)->Limit=formMap["Limit"].c_str();
			if (!((CQryLineField*)pOutput)->Limit.isValid())
			{
				strcpy((char*)pOutput, "[QryLine][Limit][isValid]");
				return NULL;
			}			
			return &CQryLineField::m_Describe;
		
		}
	case TID_SendQryAccountDetail: //资金流水查询请求
		{
			*nTNo = TID_SendQryAccountDetail;
			string sField = "";
			sField = "ProductGroup";
			((CQryAccountDetailField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CQryAccountDetailField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][AccountID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CQryAccountDetailField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][MemberID][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CQryAccountDetailField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][Currency][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CQryAccountDetailField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQryAccountDetailField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Source";
			((CQryAccountDetailField*)pOutput)->Source=formMap["Source"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->Source.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][Source][isValid]");
				return NULL;
			}			
			sField = "Limit";
			((CQryAccountDetailField*)pOutput)->Limit=formMap["Limit"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->Limit.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][Limit][isValid]");
				return NULL;
			}			
			sField = "Before";
			((CQryAccountDetailField*)pOutput)->Before=formMap["Before"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->Before.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][Before][isValid]");
				return NULL;
			}			
			sField = "After";
			((CQryAccountDetailField*)pOutput)->After=formMap["After"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->After.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][After][isValid]");
				return NULL;
			}			
			sField = "StartTime";
			((CQryAccountDetailField*)pOutput)->StartTime=formMap["StartTime"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->StartTime.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][StartTime][isValid]");
				return NULL;
			}			
			sField = "EndTime";
			((CQryAccountDetailField*)pOutput)->EndTime=formMap["EndTime"].c_str();
			if (!((CQryAccountDetailField*)pOutput)->EndTime.isValid())
			{
				strcpy((char*)pOutput, "[QryAccountDetail][EndTime][isValid]");
				return NULL;
			}			
			return &CQryAccountDetailField::m_Describe;
		
		}
	case TID_SendQrySettleDetail: //结算流水查询请求
		{
			*nTNo = TID_SendQrySettleDetail;
			string sField = "";
			sField = "ProductGroup";
			((CQrySettleDetailField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CQrySettleDetailField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[QrySettleDetail][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CQrySettleDetailField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CQrySettleDetailField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[QrySettleDetail][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "SettleAction";
			((CQrySettleDetailField*)pOutput)->SettleAction=formMap["SettleAction"].c_str();
			if (!((CQrySettleDetailField*)pOutput)->SettleAction.isValid())
			{
				strcpy((char*)pOutput, "[QrySettleDetail][SettleAction][isValid]");
				return NULL;
			}			
			return &CQrySettleDetailField::m_Describe;
		
		}
	case TID_SendCheckToken: //检查TOKEN请求
		{
			*nTNo = TID_SendCheckToken;
			string sField = "";
			sField = "Token";
			((CCheckTokenField*)pOutput)->Token=formMap["Token"].c_str();
			if (!((CCheckTokenField*)pOutput)->Token.isValid())
			{
				strcpy((char*)pOutput, "[CheckToken][Token][isValid]");
				return NULL;
			}			
			sField = "UserID";
			((CCheckTokenField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CCheckTokenField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[CheckToken][UserID][isValid]");
				return NULL;
			}			
			sField = "MaxLocalID";
			((CCheckTokenField*)pOutput)->MaxLocalID=formMap["MaxLocalID"].c_str();
			if (!((CCheckTokenField*)pOutput)->MaxLocalID.isValid())
			{
				strcpy((char*)pOutput, "[CheckToken][MaxLocalID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CCheckTokenField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CCheckTokenField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[CheckToken][MemberID][isValid]");
				return NULL;
			}			
			return &CCheckTokenField::m_Describe;
		
		}
	case TID_SendBatch: //批量操作请求(HTTP接口专用)
		{
			*nTNo = TID_SendBatch;
			string sField = "";
			sField = "IsLast";
			((CCommandHeadField*)pOutput)->IsLast=formMap["IsLast"].c_str();
			if (!((CCommandHeadField*)pOutput)->IsLast.isValid())
			{
				strcpy((char*)pOutput, "[CommandHead][IsLast][isValid]");
				return NULL;
			}			
			return &CCommandHeadField::m_Describe;
		
		}
	default:
		{
			return NULL;
		}		
	}
	return NULL;
}

CFieldDescribe* CFormAnalysis::FormToStruct(const char* packageName, const char* fieldName, const void *pInput, int nInputLen, void *pOutput, int *nTNo)
{
	CXTPPackageDefineNameMap::iterator iter1 = g_XTPPackageDefineNameMap.find(packageName);
	if(iter1==g_XTPPackageDefineNameMap.end())
	{
		sprintf((char*)pOutput, "[NoPackageName][%s]", packageName);
		return NULL;
	}
	*nTNo = (*iter1).second->tid;

	CFieldNameMap::iterator iter = g_FieldNameMap.find(fieldName);
	if(iter==g_FieldNameMap.end())
	{
		sprintf((char*)pOutput, "[NoFieldName][%s]", fieldName);
		return NULL;
	}
	map<string, string> formMap = mapSplit((char *)pInput, nInputLen, '&', '=');
	
	switch((*iter).second->fid)
	{
	case FID_ServiceConfig: //业务配置
		{
			string sField = "";
			sField = "ConfigName";
			((CServiceConfigField*)pOutput)->ConfigName=formMap["ConfigName"].c_str();
			if (!((CServiceConfigField*)pOutput)->ConfigName.isValid())
			{
				strcpy((char*)pOutput, "[ServiceConfig][ConfigName][isValid]");
				return NULL;
			}			
			sField = "Index1";
			((CServiceConfigField*)pOutput)->Index1=formMap["Index1"].c_str();
			if (!((CServiceConfigField*)pOutput)->Index1.isValid())
			{
				strcpy((char*)pOutput, "[ServiceConfig][Index1][isValid]");
				return NULL;
			}			
			sField = "Index2";
			((CServiceConfigField*)pOutput)->Index2=formMap["Index2"].c_str();
			if (!((CServiceConfigField*)pOutput)->Index2.isValid())
			{
				strcpy((char*)pOutput, "[ServiceConfig][Index2][isValid]");
				return NULL;
			}			
			sField = "ConfigValue";
			((CServiceConfigField*)pOutput)->ConfigValue=formMap["ConfigValue"].c_str();
			if (!((CServiceConfigField*)pOutput)->ConfigValue.isValid())
			{
				strcpy((char*)pOutput, "[ServiceConfig][ConfigValue][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CServiceConfigField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CServiceConfigField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[ServiceConfig][Remark][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CServiceConfigField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CServiceConfigField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[ServiceConfig][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CServiceConfigField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CServiceConfigField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[ServiceConfig][UpdateTime][isValid]");
				return NULL;
			}			
			return &CServiceConfigField::m_Describe;
		}
	case FID_Currency: //币种信息
		{
			string sField = "";
			sField = "SettlementGroup";
			((CCurrencyField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CCurrencyField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Currency][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CCurrencyField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CCurrencyField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[Currency][Currency][isValid]");
				return NULL;
			}			
			sField = "Deposit";
			((CCurrencyField*)pOutput)->Deposit=formMap["Deposit"].c_str();
			if (!((CCurrencyField*)pOutput)->Deposit.isValid())
			{
				strcpy((char*)pOutput, "[Currency][Deposit][isValid]");
				return NULL;
			}			
			sField = "Withdraw";
			((CCurrencyField*)pOutput)->Withdraw=formMap["Withdraw"].c_str();
			if (!((CCurrencyField*)pOutput)->Withdraw.isValid())
			{
				strcpy((char*)pOutput, "[Currency][Withdraw][isValid]");
				return NULL;
			}			
			sField = "Balance";
			((CCurrencyField*)pOutput)->Balance=formMap["Balance"].c_str();
			if (!((CCurrencyField*)pOutput)->Balance.isValid())
			{
				strcpy((char*)pOutput, "[Currency][Balance][isValid]");
				return NULL;
			}			
			sField = "PreBalance";
			((CCurrencyField*)pOutput)->PreBalance=formMap["PreBalance"].c_str();
			if (!((CCurrencyField*)pOutput)->PreBalance.isValid())
			{
				strcpy((char*)pOutput, "[Currency][PreBalance][isValid]");
				return NULL;
			}			
			sField = "SortNumber";
			((CCurrencyField*)pOutput)->SortNumber=formMap["SortNumber"].c_str();
			if (!((CCurrencyField*)pOutput)->SortNumber.isValid())
			{
				strcpy((char*)pOutput, "[Currency][SortNumber][isValid]");
				return NULL;
			}			
			sField = "CanReduce";
			((CCurrencyField*)pOutput)->CanReduce=formMap["CanReduce"].c_str();
			if (!((CCurrencyField*)pOutput)->CanReduce.isValid())
			{
				strcpy((char*)pOutput, "[Currency][CanReduce][isValid]");
				return NULL;
			}			
			sField = "IsTrading";
			((CCurrencyField*)pOutput)->IsTrading=formMap["IsTrading"].c_str();
			if (!((CCurrencyField*)pOutput)->IsTrading.isValid())
			{
				strcpy((char*)pOutput, "[Currency][IsTrading][isValid]");
				return NULL;
			}			
			sField = "SettleSegment";
			((CCurrencyField*)pOutput)->SettleSegment=formMap["SettleSegment"].c_str();
			if (!((CCurrencyField*)pOutput)->SettleSegment.isValid())
			{
				strcpy((char*)pOutput, "[Currency][SettleSegment][isValid]");
				return NULL;
			}			
			sField = "MoneyTick";
			((CCurrencyField*)pOutput)->MoneyTick=formMap["MoneyTick"].c_str();
			if (!((CCurrencyField*)pOutput)->MoneyTick.isValid())
			{
				strcpy((char*)pOutput, "[Currency][MoneyTick][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CCurrencyField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CCurrencyField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[Currency][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CCurrencyField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CCurrencyField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[Currency][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "LoanUse";
			((CCurrencyField*)pOutput)->LoanUse=formMap["LoanUse"].c_str();
			if (!((CCurrencyField*)pOutput)->LoanUse.isValid())
			{
				strcpy((char*)pOutput, "[Currency][LoanUse][isValid]");
				return NULL;
			}			
			sField = "LoanLimit";
			((CCurrencyField*)pOutput)->LoanLimit=formMap["LoanLimit"].c_str();
			if (!((CCurrencyField*)pOutput)->LoanLimit.isValid())
			{
				strcpy((char*)pOutput, "[Currency][LoanLimit][isValid]");
				return NULL;
			}			
			sField = "LoanBorrow";
			((CCurrencyField*)pOutput)->LoanBorrow=formMap["LoanBorrow"].c_str();
			if (!((CCurrencyField*)pOutput)->LoanBorrow.isValid())
			{
				strcpy((char*)pOutput, "[Currency][LoanBorrow][isValid]");
				return NULL;
			}			
			sField = "LoanDeposit";
			((CCurrencyField*)pOutput)->LoanDeposit=formMap["LoanDeposit"].c_str();
			if (!((CCurrencyField*)pOutput)->LoanDeposit.isValid())
			{
				strcpy((char*)pOutput, "[Currency][LoanDeposit][isValid]");
				return NULL;
			}			
			sField = "LoanBase";
			((CCurrencyField*)pOutput)->LoanBase=formMap["LoanBase"].c_str();
			if (!((CCurrencyField*)pOutput)->LoanBase.isValid())
			{
				strcpy((char*)pOutput, "[Currency][LoanBase][isValid]");
				return NULL;
			}			
			sField = "LoanRate";
			((CCurrencyField*)pOutput)->LoanRate=formMap["LoanRate"].c_str();
			if (!((CCurrencyField*)pOutput)->LoanRate.isValid())
			{
				strcpy((char*)pOutput, "[Currency][LoanRate][isValid]");
				return NULL;
			}			
			sField = "LoanHighBase";
			((CCurrencyField*)pOutput)->LoanHighBase=formMap["LoanHighBase"].c_str();
			if (!((CCurrencyField*)pOutput)->LoanHighBase.isValid())
			{
				strcpy((char*)pOutput, "[Currency][LoanHighBase][isValid]");
				return NULL;
			}			
			sField = "LoanHighRate";
			((CCurrencyField*)pOutput)->LoanHighRate=formMap["LoanHighRate"].c_str();
			if (!((CCurrencyField*)pOutput)->LoanHighRate.isValid())
			{
				strcpy((char*)pOutput, "[Currency][LoanHighRate][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CCurrencyField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CCurrencyField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Currency][Remark][isValid]");
				return NULL;
			}			
			sField = "CurrValue";
			((CCurrencyField*)pOutput)->CurrValue=formMap["CurrValue"].c_str();
			if (!((CCurrencyField*)pOutput)->CurrValue.isValid())
			{
				strcpy((char*)pOutput, "[Currency][CurrValue][isValid]");
				return NULL;
			}			
			sField = "HairCut";
			((CCurrencyField*)pOutput)->HairCut=formMap["HairCut"].c_str();
			if (!((CCurrencyField*)pOutput)->HairCut.isValid())
			{
				strcpy((char*)pOutput, "[Currency][HairCut][isValid]");
				return NULL;
			}			
			sField = "MaxValue";
			((CCurrencyField*)pOutput)->MaxValue=formMap["MaxValue"].c_str();
			if (!((CCurrencyField*)pOutput)->MaxValue.isValid())
			{
				strcpy((char*)pOutput, "[Currency][MaxValue][isValid]");
				return NULL;
			}			
			sField = "MarginRateGroup";
			((CCurrencyField*)pOutput)->MarginRateGroup=formMap["MarginRateGroup"].c_str();
			if (!((CCurrencyField*)pOutput)->MarginRateGroup.isValid())
			{
				strcpy((char*)pOutput, "[Currency][MarginRateGroup][isValid]");
				return NULL;
			}			
			sField = "PrivateEnd";
			((CCurrencyField*)pOutput)->PrivateEnd=formMap["PrivateEnd"].c_str();
			if (!((CCurrencyField*)pOutput)->PrivateEnd.isValid())
			{
				strcpy((char*)pOutput, "[Currency][PrivateEnd][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CCurrencyField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CCurrencyField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[Currency][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CCurrencyField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CCurrencyField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[Currency][UpdateTime][isValid]");
				return NULL;
			}			
			return &CCurrencyField::m_Describe;
		}
	case FID_MarketData: //市场行情
		{
			string sField = "";
			sField = "ExchangeID";
			((CMarketDataField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CMarketDataField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CMarketDataField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CMarketDataField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "ProductGroup";
			((CMarketDataField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CMarketDataField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CMarketDataField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CMarketDataField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][UpdateTime][isValid]");
				return NULL;
			}			
			sField = "PreClosePrice";
			((CMarketDataField*)pOutput)->PreClosePrice=formMap["PreClosePrice"].c_str();
			if (!((CMarketDataField*)pOutput)->PreClosePrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][PreClosePrice][isValid]");
				return NULL;
			}			
			sField = "PreSettlementPrice";
			((CMarketDataField*)pOutput)->PreSettlementPrice=formMap["PreSettlementPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->PreSettlementPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][PreSettlementPrice][isValid]");
				return NULL;
			}			
			sField = "PreOpenInterest";
			((CMarketDataField*)pOutput)->PreOpenInterest=formMap["PreOpenInterest"].c_str();
			if (!((CMarketDataField*)pOutput)->PreOpenInterest.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][PreOpenInterest][isValid]");
				return NULL;
			}			
			sField = "PrePositionFeeRate";
			((CMarketDataField*)pOutput)->PrePositionFeeRate=formMap["PrePositionFeeRate"].c_str();
			if (!((CMarketDataField*)pOutput)->PrePositionFeeRate.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][PrePositionFeeRate][isValid]");
				return NULL;
			}			
			sField = "ClosePrice";
			((CMarketDataField*)pOutput)->ClosePrice=formMap["ClosePrice"].c_str();
			if (!((CMarketDataField*)pOutput)->ClosePrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][ClosePrice][isValid]");
				return NULL;
			}			
			sField = "PositionFeeTime";
			((CMarketDataField*)pOutput)->PositionFeeTime=formMap["PositionFeeTime"].c_str();
			if (!((CMarketDataField*)pOutput)->PositionFeeTime.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][PositionFeeTime][isValid]");
				return NULL;
			}			
			sField = "SettlementTime";
			((CMarketDataField*)pOutput)->SettlementTime=formMap["SettlementTime"].c_str();
			if (!((CMarketDataField*)pOutput)->SettlementTime.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][SettlementTime][isValid]");
				return NULL;
			}			
			sField = "UpperLimitPrice";
			((CMarketDataField*)pOutput)->UpperLimitPrice=formMap["UpperLimitPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->UpperLimitPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][UpperLimitPrice][isValid]");
				return NULL;
			}			
			sField = "LowerLimitPrice";
			((CMarketDataField*)pOutput)->LowerLimitPrice=formMap["LowerLimitPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->LowerLimitPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][LowerLimitPrice][isValid]");
				return NULL;
			}			
			sField = "UnderlyingPrice";
			((CMarketDataField*)pOutput)->UnderlyingPrice=formMap["UnderlyingPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->UnderlyingPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][UnderlyingPrice][isValid]");
				return NULL;
			}			
			sField = "MarkedPrice";
			((CMarketDataField*)pOutput)->MarkedPrice=formMap["MarkedPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->MarkedPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][MarkedPrice][isValid]");
				return NULL;
			}			
			sField = "PositionFeeRate";
			((CMarketDataField*)pOutput)->PositionFeeRate=formMap["PositionFeeRate"].c_str();
			if (!((CMarketDataField*)pOutput)->PositionFeeRate.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][PositionFeeRate][isValid]");
				return NULL;
			}			
			sField = "HighestPrice";
			((CMarketDataField*)pOutput)->HighestPrice=formMap["HighestPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->HighestPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][HighestPrice][isValid]");
				return NULL;
			}			
			sField = "LowestPrice";
			((CMarketDataField*)pOutput)->LowestPrice=formMap["LowestPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->LowestPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][LowestPrice][isValid]");
				return NULL;
			}			
			sField = "LastPrice";
			((CMarketDataField*)pOutput)->LastPrice=formMap["LastPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->LastPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][LastPrice][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CMarketDataField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CMarketDataField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][Volume][isValid]");
				return NULL;
			}			
			sField = "Turnover";
			((CMarketDataField*)pOutput)->Turnover=formMap["Turnover"].c_str();
			if (!((CMarketDataField*)pOutput)->Turnover.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][Turnover][isValid]");
				return NULL;
			}			
			sField = "OpenInterest";
			((CMarketDataField*)pOutput)->OpenInterest=formMap["OpenInterest"].c_str();
			if (!((CMarketDataField*)pOutput)->OpenInterest.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][OpenInterest][isValid]");
				return NULL;
			}			
			sField = "SettlementPrice";
			((CMarketDataField*)pOutput)->SettlementPrice=formMap["SettlementPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->SettlementPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][SettlementPrice][isValid]");
				return NULL;
			}			
			sField = "OpenPrice";
			((CMarketDataField*)pOutput)->OpenPrice=formMap["OpenPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->OpenPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][OpenPrice][isValid]");
				return NULL;
			}			
			sField = "IsPubMarketData";
			((CMarketDataField*)pOutput)->IsPubMarketData=formMap["IsPubMarketData"].c_str();
			if (!((CMarketDataField*)pOutput)->IsPubMarketData.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][IsPubMarketData][isValid]");
				return NULL;
			}			
			sField = "InstrumentStatus";
			((CMarketDataField*)pOutput)->InstrumentStatus=formMap["InstrumentStatus"].c_str();
			if (!((CMarketDataField*)pOutput)->InstrumentStatus.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][InstrumentStatus][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CMarketDataField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CMarketDataField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][Remark][isValid]");
				return NULL;
			}			
			sField = "BusinessNo";
			((CMarketDataField*)pOutput)->BusinessNo=formMap["BusinessNo"].c_str();
			if (!((CMarketDataField*)pOutput)->BusinessNo.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][BusinessNo][isValid]");
				return NULL;
			}			
			sField = "MarkedPriceTotal";
			((CMarketDataField*)pOutput)->MarkedPriceTotal=formMap["MarkedPriceTotal"].c_str();
			if (!((CMarketDataField*)pOutput)->MarkedPriceTotal.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][MarkedPriceTotal][isValid]");
				return NULL;
			}			
			sField = "MarkedPriceWeight";
			((CMarketDataField*)pOutput)->MarkedPriceWeight=formMap["MarkedPriceWeight"].c_str();
			if (!((CMarketDataField*)pOutput)->MarkedPriceWeight.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][MarkedPriceWeight][isValid]");
				return NULL;
			}			
			sField = "L25UpperPrice";
			((CMarketDataField*)pOutput)->L25UpperPrice=formMap["L25UpperPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->L25UpperPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][L25UpperPrice][isValid]");
				return NULL;
			}			
			sField = "L25LowerPrice";
			((CMarketDataField*)pOutput)->L25LowerPrice=formMap["L25LowerPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->L25LowerPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][L25LowerPrice][isValid]");
				return NULL;
			}			
			sField = "L25UpperPrice1";
			((CMarketDataField*)pOutput)->L25UpperPrice1=formMap["L25UpperPrice1"].c_str();
			if (!((CMarketDataField*)pOutput)->L25UpperPrice1.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][L25UpperPrice1][isValid]");
				return NULL;
			}			
			sField = "L25LowerPrice1";
			((CMarketDataField*)pOutput)->L25LowerPrice1=formMap["L25LowerPrice1"].c_str();
			if (!((CMarketDataField*)pOutput)->L25LowerPrice1.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][L25LowerPrice1][isValid]");
				return NULL;
			}			
			sField = "L25UpperPrice2";
			((CMarketDataField*)pOutput)->L25UpperPrice2=formMap["L25UpperPrice2"].c_str();
			if (!((CMarketDataField*)pOutput)->L25UpperPrice2.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][L25UpperPrice2][isValid]");
				return NULL;
			}			
			sField = "L25LowerPrice2";
			((CMarketDataField*)pOutput)->L25LowerPrice2=formMap["L25LowerPrice2"].c_str();
			if (!((CMarketDataField*)pOutput)->L25LowerPrice2.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][L25LowerPrice2][isValid]");
				return NULL;
			}			
			sField = "BidPrice1";
			((CMarketDataField*)pOutput)->BidPrice1=formMap["BidPrice1"].c_str();
			if (!((CMarketDataField*)pOutput)->BidPrice1.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][BidPrice1][isValid]");
				return NULL;
			}			
			sField = "BidVolume1";
			((CMarketDataField*)pOutput)->BidVolume1=formMap["BidVolume1"].c_str();
			if (!((CMarketDataField*)pOutput)->BidVolume1.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][BidVolume1][isValid]");
				return NULL;
			}			
			sField = "AskPrice1";
			((CMarketDataField*)pOutput)->AskPrice1=formMap["AskPrice1"].c_str();
			if (!((CMarketDataField*)pOutput)->AskPrice1.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][AskPrice1][isValid]");
				return NULL;
			}			
			sField = "AskVolume1";
			((CMarketDataField*)pOutput)->AskVolume1=formMap["AskVolume1"].c_str();
			if (!((CMarketDataField*)pOutput)->AskVolume1.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][AskVolume1][isValid]");
				return NULL;
			}			
			sField = "TheoryPrice";
			((CMarketDataField*)pOutput)->TheoryPrice=formMap["TheoryPrice"].c_str();
			if (!((CMarketDataField*)pOutput)->TheoryPrice.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][TheoryPrice][isValid]");
				return NULL;
			}			
			sField = "HighestPrice24";
			((CMarketDataField*)pOutput)->HighestPrice24=formMap["HighestPrice24"].c_str();
			if (!((CMarketDataField*)pOutput)->HighestPrice24.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][HighestPrice24][isValid]");
				return NULL;
			}			
			sField = "LowestPrice24";
			((CMarketDataField*)pOutput)->LowestPrice24=formMap["LowestPrice24"].c_str();
			if (!((CMarketDataField*)pOutput)->LowestPrice24.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][LowestPrice24][isValid]");
				return NULL;
			}			
			sField = "Volume24";
			((CMarketDataField*)pOutput)->Volume24=formMap["Volume24"].c_str();
			if (!((CMarketDataField*)pOutput)->Volume24.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][Volume24][isValid]");
				return NULL;
			}			
			sField = "Turnover24";
			((CMarketDataField*)pOutput)->Turnover24=formMap["Turnover24"].c_str();
			if (!((CMarketDataField*)pOutput)->Turnover24.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][Turnover24][isValid]");
				return NULL;
			}			
			sField = "OpenPrice24";
			((CMarketDataField*)pOutput)->OpenPrice24=formMap["OpenPrice24"].c_str();
			if (!((CMarketDataField*)pOutput)->OpenPrice24.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][OpenPrice24][isValid]");
				return NULL;
			}			
			sField = "Frequency";
			((CMarketDataField*)pOutput)->Frequency=formMap["Frequency"].c_str();
			if (!((CMarketDataField*)pOutput)->Frequency.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][Frequency][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CMarketDataField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CMarketDataField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[MarketData][InsertTime][isValid]");
				return NULL;
			}			
			return &CMarketDataField::m_Describe;
		}
	case FID_Instrument: //交易标的
		{
			string sField = "";
			sField = "ExchangeID";
			((CInstrumentField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CInstrumentField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CInstrumentField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CInstrumentField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "InstrumentName";
			((CInstrumentField*)pOutput)->InstrumentName=formMap["InstrumentName"].c_str();
			if (!((CInstrumentField*)pOutput)->InstrumentName.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][InstrumentName][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CInstrumentField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CInstrumentField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "ProductGroup";
			((CInstrumentField*)pOutput)->ProductGroup=formMap["ProductGroup"].c_str();
			if (!((CInstrumentField*)pOutput)->ProductGroup.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][ProductGroup][isValid]");
				return NULL;
			}			
			sField = "UnderlyingID";
			((CInstrumentField*)pOutput)->UnderlyingID=formMap["UnderlyingID"].c_str();
			if (!((CInstrumentField*)pOutput)->UnderlyingID.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][UnderlyingID][isValid]");
				return NULL;
			}			
			sField = "ProductClass";
			((CInstrumentField*)pOutput)->ProductClass=formMap["ProductClass"].c_str();
			if (!((CInstrumentField*)pOutput)->ProductClass.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][ProductClass][isValid]");
				return NULL;
			}			
			sField = "ProductType";
			((CInstrumentField*)pOutput)->ProductType=formMap["ProductType"].c_str();
			if (!((CInstrumentField*)pOutput)->ProductType.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][ProductType][isValid]");
				return NULL;
			}			
			sField = "StrikePrice";
			((CInstrumentField*)pOutput)->StrikePrice=formMap["StrikePrice"].c_str();
			if (!((CInstrumentField*)pOutput)->StrikePrice.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][StrikePrice][isValid]");
				return NULL;
			}			
			sField = "OptionsType";
			((CInstrumentField*)pOutput)->OptionsType=formMap["OptionsType"].c_str();
			if (!((CInstrumentField*)pOutput)->OptionsType.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][OptionsType][isValid]");
				return NULL;
			}			
			sField = "VolumeMultiple";
			((CInstrumentField*)pOutput)->VolumeMultiple=formMap["VolumeMultiple"].c_str();
			if (!((CInstrumentField*)pOutput)->VolumeMultiple.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][VolumeMultiple][isValid]");
				return NULL;
			}			
			sField = "StartTime";
			((CInstrumentField*)pOutput)->StartTime=formMap["StartTime"].c_str();
			if (!((CInstrumentField*)pOutput)->StartTime.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][StartTime][isValid]");
				return NULL;
			}			
			sField = "ExpireTime";
			((CInstrumentField*)pOutput)->ExpireTime=formMap["ExpireTime"].c_str();
			if (!((CInstrumentField*)pOutput)->ExpireTime.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][ExpireTime][isValid]");
				return NULL;
			}			
			sField = "PriceCurrency";
			((CInstrumentField*)pOutput)->PriceCurrency=formMap["PriceCurrency"].c_str();
			if (!((CInstrumentField*)pOutput)->PriceCurrency.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][PriceCurrency][isValid]");
				return NULL;
			}			
			sField = "ClearCurrency";
			((CInstrumentField*)pOutput)->ClearCurrency=formMap["ClearCurrency"].c_str();
			if (!((CInstrumentField*)pOutput)->ClearCurrency.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][ClearCurrency][isValid]");
				return NULL;
			}			
			sField = "IsInverse";
			((CInstrumentField*)pOutput)->IsInverse=formMap["IsInverse"].c_str();
			if (!((CInstrumentField*)pOutput)->IsInverse.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][IsInverse][isValid]");
				return NULL;
			}			
			sField = "BaseCurrency";
			((CInstrumentField*)pOutput)->BaseCurrency=formMap["BaseCurrency"].c_str();
			if (!((CInstrumentField*)pOutput)->BaseCurrency.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][BaseCurrency][isValid]");
				return NULL;
			}			
			sField = "MarginPriceType";
			((CInstrumentField*)pOutput)->MarginPriceType=formMap["MarginPriceType"].c_str();
			if (!((CInstrumentField*)pOutput)->MarginPriceType.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][MarginPriceType][isValid]");
				return NULL;
			}			
			sField = "BasisPrice";
			((CInstrumentField*)pOutput)->BasisPrice=formMap["BasisPrice"].c_str();
			if (!((CInstrumentField*)pOutput)->BasisPrice.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][BasisPrice][isValid]");
				return NULL;
			}			
			sField = "MinOrderVolume";
			((CInstrumentField*)pOutput)->MinOrderVolume=formMap["MinOrderVolume"].c_str();
			if (!((CInstrumentField*)pOutput)->MinOrderVolume.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][MinOrderVolume][isValid]");
				return NULL;
			}			
			sField = "MinOrderCost";
			((CInstrumentField*)pOutput)->MinOrderCost=formMap["MinOrderCost"].c_str();
			if (!((CInstrumentField*)pOutput)->MinOrderCost.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][MinOrderCost][isValid]");
				return NULL;
			}			
			sField = "MaxOrderVolume";
			((CInstrumentField*)pOutput)->MaxOrderVolume=formMap["MaxOrderVolume"].c_str();
			if (!((CInstrumentField*)pOutput)->MaxOrderVolume.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][MaxOrderVolume][isValid]");
				return NULL;
			}			
			sField = "PriceTick";
			((CInstrumentField*)pOutput)->PriceTick=formMap["PriceTick"].c_str();
			if (!((CInstrumentField*)pOutput)->PriceTick.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][PriceTick][isValid]");
				return NULL;
			}			
			sField = "VolumeTick";
			((CInstrumentField*)pOutput)->VolumeTick=formMap["VolumeTick"].c_str();
			if (!((CInstrumentField*)pOutput)->VolumeTick.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][VolumeTick][isValid]");
				return NULL;
			}			
			sField = "ShowVolumeTick";
			((CInstrumentField*)pOutput)->ShowVolumeTick=formMap["ShowVolumeTick"].c_str();
			if (!((CInstrumentField*)pOutput)->ShowVolumeTick.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][ShowVolumeTick][isValid]");
				return NULL;
			}			
			sField = "TradingModel";
			((CInstrumentField*)pOutput)->TradingModel=formMap["TradingModel"].c_str();
			if (!((CInstrumentField*)pOutput)->TradingModel.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][TradingModel][isValid]");
				return NULL;
			}			
			sField = "DayStartTime";
			((CInstrumentField*)pOutput)->DayStartTime=formMap["DayStartTime"].c_str();
			if (!((CInstrumentField*)pOutput)->DayStartTime.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][DayStartTime][isValid]");
				return NULL;
			}			
			sField = "SortNumber";
			((CInstrumentField*)pOutput)->SortNumber=formMap["SortNumber"].c_str();
			if (!((CInstrumentField*)pOutput)->SortNumber.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][SortNumber][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CInstrumentField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CInstrumentField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][Remark][isValid]");
				return NULL;
			}			
			sField = "DefaultLeverage";
			((CInstrumentField*)pOutput)->DefaultLeverage=formMap["DefaultLeverage"].c_str();
			if (!((CInstrumentField*)pOutput)->DefaultLeverage.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][DefaultLeverage][isValid]");
				return NULL;
			}			
			sField = "PriceLimitValueMode";
			((CInstrumentField*)pOutput)->PriceLimitValueMode=formMap["PriceLimitValueMode"].c_str();
			if (!((CInstrumentField*)pOutput)->PriceLimitValueMode.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][PriceLimitValueMode][isValid]");
				return NULL;
			}			
			sField = "PriceLimitPrice";
			((CInstrumentField*)pOutput)->PriceLimitPrice=formMap["PriceLimitPrice"].c_str();
			if (!((CInstrumentField*)pOutput)->PriceLimitPrice.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][PriceLimitPrice][isValid]");
				return NULL;
			}			
			sField = "PriceLimitUpperValue";
			((CInstrumentField*)pOutput)->PriceLimitUpperValue=formMap["PriceLimitUpperValue"].c_str();
			if (!((CInstrumentField*)pOutput)->PriceLimitUpperValue.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][PriceLimitUpperValue][isValid]");
				return NULL;
			}			
			sField = "PriceLimitLowerValue";
			((CInstrumentField*)pOutput)->PriceLimitLowerValue=formMap["PriceLimitLowerValue"].c_str();
			if (!((CInstrumentField*)pOutput)->PriceLimitLowerValue.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][PriceLimitLowerValue][isValid]");
				return NULL;
			}			
			sField = "MarginRateGroup";
			((CInstrumentField*)pOutput)->MarginRateGroup=formMap["MarginRateGroup"].c_str();
			if (!((CInstrumentField*)pOutput)->MarginRateGroup.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][MarginRateGroup][isValid]");
				return NULL;
			}			
			sField = "CFDGroup";
			((CInstrumentField*)pOutput)->CFDGroup=formMap["CFDGroup"].c_str();
			if (!((CInstrumentField*)pOutput)->CFDGroup.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][CFDGroup][isValid]");
				return NULL;
			}			
			sField = "FeeGroup";
			((CInstrumentField*)pOutput)->FeeGroup=formMap["FeeGroup"].c_str();
			if (!((CInstrumentField*)pOutput)->FeeGroup.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][FeeGroup][isValid]");
				return NULL;
			}			
			sField = "TradingRightGroup";
			((CInstrumentField*)pOutput)->TradingRightGroup=formMap["TradingRightGroup"].c_str();
			if (!((CInstrumentField*)pOutput)->TradingRightGroup.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][TradingRightGroup][isValid]");
				return NULL;
			}			
			sField = "MaxOpenInterest";
			((CInstrumentField*)pOutput)->MaxOpenInterest=formMap["MaxOpenInterest"].c_str();
			if (!((CInstrumentField*)pOutput)->MaxOpenInterest.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][MaxOpenInterest][isValid]");
				return NULL;
			}			
			sField = "FundingRateGroup";
			((CInstrumentField*)pOutput)->FundingRateGroup=formMap["FundingRateGroup"].c_str();
			if (!((CInstrumentField*)pOutput)->FundingRateGroup.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][FundingRateGroup][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CInstrumentField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CInstrumentField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CInstrumentField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CInstrumentField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][UpdateTime][isValid]");
				return NULL;
			}			
			sField = "MarkedPriceTimes";
			((CInstrumentField*)pOutput)->MarkedPriceTimes=formMap["MarkedPriceTimes"].c_str();
			if (!((CInstrumentField*)pOutput)->MarkedPriceTimes.isValid())
			{
				strcpy((char*)pOutput, "[Instrument][MarkedPriceTimes][isValid]");
				return NULL;
			}			
			return &CInstrumentField::m_Describe;
		}
	case FID_CurrentTime: //当前时间
		{
			string sField = "";
			sField = "SystemID";
			((CCurrentTimeField*)pOutput)->SystemID=formMap["SystemID"].c_str();
			if (!((CCurrentTimeField*)pOutput)->SystemID.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][SystemID][isValid]");
				return NULL;
			}			
			sField = "ZoneDate";
			((CCurrentTimeField*)pOutput)->ZoneDate=formMap["ZoneDate"].c_str();
			if (!((CCurrentTimeField*)pOutput)->ZoneDate.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][ZoneDate][isValid]");
				return NULL;
			}			
			sField = "ZoneTime";
			((CCurrentTimeField*)pOutput)->ZoneTime=formMap["ZoneTime"].c_str();
			if (!((CCurrentTimeField*)pOutput)->ZoneTime.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][ZoneTime][isValid]");
				return NULL;
			}			
			sField = "ZoneDays";
			((CCurrentTimeField*)pOutput)->ZoneDays=formMap["ZoneDays"].c_str();
			if (!((CCurrentTimeField*)pOutput)->ZoneDays.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][ZoneDays][isValid]");
				return NULL;
			}			
			sField = "CurrTime";
			((CCurrentTimeField*)pOutput)->CurrTime=formMap["CurrTime"].c_str();
			if (!((CCurrentTimeField*)pOutput)->CurrTime.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][CurrTime][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CCurrentTimeField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CCurrentTimeField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CCurrentTimeField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CCurrentTimeField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][UpdateTime][isValid]");
				return NULL;
			}			
			sField = "TimeZone";
			((CCurrentTimeField*)pOutput)->TimeZone=formMap["TimeZone"].c_str();
			if (!((CCurrentTimeField*)pOutput)->TimeZone.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][TimeZone][isValid]");
				return NULL;
			}			
			sField = "LastTradeID";
			((CCurrentTimeField*)pOutput)->LastTradeID=formMap["LastTradeID"].c_str();
			if (!((CCurrentTimeField*)pOutput)->LastTradeID.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][LastTradeID][isValid]");
				return NULL;
			}			
			sField = "LastOrderID";
			((CCurrentTimeField*)pOutput)->LastOrderID=formMap["LastOrderID"].c_str();
			if (!((CCurrentTimeField*)pOutput)->LastOrderID.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][LastOrderID][isValid]");
				return NULL;
			}			
			sField = "LastAccountDetailID";
			((CCurrentTimeField*)pOutput)->LastAccountDetailID=formMap["LastAccountDetailID"].c_str();
			if (!((CCurrentTimeField*)pOutput)->LastAccountDetailID.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][LastAccountDetailID][isValid]");
				return NULL;
			}			
			sField = "LastTimeSortNo";
			((CCurrentTimeField*)pOutput)->LastTimeSortNo=formMap["LastTimeSortNo"].c_str();
			if (!((CCurrentTimeField*)pOutput)->LastTimeSortNo.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][LastTimeSortNo][isValid]");
				return NULL;
			}			
			sField = "LastBusinessNo";
			((CCurrentTimeField*)pOutput)->LastBusinessNo=formMap["LastBusinessNo"].c_str();
			if (!((CCurrentTimeField*)pOutput)->LastBusinessNo.isValid())
			{
				strcpy((char*)pOutput, "[CurrentTime][LastBusinessNo][isValid]");
				return NULL;
			}			
			return &CCurrentTimeField::m_Describe;
		}
	case FID_Member: //成员
		{
			string sField = "";
			sField = "MemberID";
			((CMemberField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CMemberField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[Member][MemberID][isValid]");
				return NULL;
			}			
			sField = "MemberType";
			((CMemberField*)pOutput)->MemberType=formMap["MemberType"].c_str();
			if (!((CMemberField*)pOutput)->MemberType.isValid())
			{
				strcpy((char*)pOutput, "[Member][MemberType][isValid]");
				return NULL;
			}			
			sField = "Grade";
			((CMemberField*)pOutput)->Grade=formMap["Grade"].c_str();
			if (!((CMemberField*)pOutput)->Grade.isValid())
			{
				strcpy((char*)pOutput, "[Member][Grade][isValid]");
				return NULL;
			}			
			sField = "Priority";
			((CMemberField*)pOutput)->Priority=formMap["Priority"].c_str();
			if (!((CMemberField*)pOutput)->Priority.isValid())
			{
				strcpy((char*)pOutput, "[Member][Priority][isValid]");
				return NULL;
			}			
			sField = "FeeGrade";
			((CMemberField*)pOutput)->FeeGrade=formMap["FeeGrade"].c_str();
			if (!((CMemberField*)pOutput)->FeeGrade.isValid())
			{
				strcpy((char*)pOutput, "[Member][FeeGrade][isValid]");
				return NULL;
			}			
			sField = "MarginRateGrade";
			((CMemberField*)pOutput)->MarginRateGrade=formMap["MarginRateGrade"].c_str();
			if (!((CMemberField*)pOutput)->MarginRateGrade.isValid())
			{
				strcpy((char*)pOutput, "[Member][MarginRateGrade][isValid]");
				return NULL;
			}			
			sField = "TradingRightGrade";
			((CMemberField*)pOutput)->TradingRightGrade=formMap["TradingRightGrade"].c_str();
			if (!((CMemberField*)pOutput)->TradingRightGrade.isValid())
			{
				strcpy((char*)pOutput, "[Member][TradingRightGrade][isValid]");
				return NULL;
			}			
			sField = "CFDGrade";
			((CMemberField*)pOutput)->CFDGrade=formMap["CFDGrade"].c_str();
			if (!((CMemberField*)pOutput)->CFDGrade.isValid())
			{
				strcpy((char*)pOutput, "[Member][CFDGrade][isValid]");
				return NULL;
			}			
			sField = "LiquidityGrade";
			((CMemberField*)pOutput)->LiquidityGrade=formMap["LiquidityGrade"].c_str();
			if (!((CMemberField*)pOutput)->LiquidityGrade.isValid())
			{
				strcpy((char*)pOutput, "[Member][LiquidityGrade][isValid]");
				return NULL;
			}			
			sField = "RiskLevel";
			((CMemberField*)pOutput)->RiskLevel=formMap["RiskLevel"].c_str();
			if (!((CMemberField*)pOutput)->RiskLevel.isValid())
			{
				strcpy((char*)pOutput, "[Member][RiskLevel][isValid]");
				return NULL;
			}			
			sField = "PositionType";
			((CMemberField*)pOutput)->PositionType=formMap["PositionType"].c_str();
			if (!((CMemberField*)pOutput)->PositionType.isValid())
			{
				strcpy((char*)pOutput, "[Member][PositionType][isValid]");
				return NULL;
			}			
			sField = "TriggerOrders";
			((CMemberField*)pOutput)->TriggerOrders=formMap["TriggerOrders"].c_str();
			if (!((CMemberField*)pOutput)->TriggerOrders.isValid())
			{
				strcpy((char*)pOutput, "[Member][TriggerOrders][isValid]");
				return NULL;
			}			
			sField = "OpenOrders";
			((CMemberField*)pOutput)->OpenOrders=formMap["OpenOrders"].c_str();
			if (!((CMemberField*)pOutput)->OpenOrders.isValid())
			{
				strcpy((char*)pOutput, "[Member][OpenOrders][isValid]");
				return NULL;
			}			
			sField = "OpenPositions";
			((CMemberField*)pOutput)->OpenPositions=formMap["OpenPositions"].c_str();
			if (!((CMemberField*)pOutput)->OpenPositions.isValid())
			{
				strcpy((char*)pOutput, "[Member][OpenPositions][isValid]");
				return NULL;
			}			
			sField = "MaxLocalID";
			((CMemberField*)pOutput)->MaxLocalID=formMap["MaxLocalID"].c_str();
			if (!((CMemberField*)pOutput)->MaxLocalID.isValid())
			{
				strcpy((char*)pOutput, "[Member][MaxLocalID][isValid]");
				return NULL;
			}			
			sField = "SinkType";
			((CMemberField*)pOutput)->SinkType=formMap["SinkType"].c_str();
			if (!((CMemberField*)pOutput)->SinkType.isValid())
			{
				strcpy((char*)pOutput, "[Member][SinkType][isValid]");
				return NULL;
			}			
			sField = "CopyMemberID";
			((CMemberField*)pOutput)->CopyMemberID=formMap["CopyMemberID"].c_str();
			if (!((CMemberField*)pOutput)->CopyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[Member][CopyMemberID][isValid]");
				return NULL;
			}			
			sField = "Region";
			((CMemberField*)pOutput)->Region=formMap["Region"].c_str();
			if (!((CMemberField*)pOutput)->Region.isValid())
			{
				strcpy((char*)pOutput, "[Member][Region][isValid]");
				return NULL;
			}			
			sField = "FeeDeduceDisCount";
			((CMemberField*)pOutput)->FeeDeduceDisCount=formMap["FeeDeduceDisCount"].c_str();
			if (!((CMemberField*)pOutput)->FeeDeduceDisCount.isValid())
			{
				strcpy((char*)pOutput, "[Member][FeeDeduceDisCount][isValid]");
				return NULL;
			}			
			sField = "FeeDeduceCurrency";
			((CMemberField*)pOutput)->FeeDeduceCurrency=formMap["FeeDeduceCurrency"].c_str();
			if (!((CMemberField*)pOutput)->FeeDeduceCurrency.isValid())
			{
				strcpy((char*)pOutput, "[Member][FeeDeduceCurrency][isValid]");
				return NULL;
			}			
			sField = "CanBorrow";
			((CMemberField*)pOutput)->CanBorrow=formMap["CanBorrow"].c_str();
			if (!((CMemberField*)pOutput)->CanBorrow.isValid())
			{
				strcpy((char*)pOutput, "[Member][CanBorrow][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CMemberField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CMemberField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Member][Remark][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CMemberField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CMemberField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[Member][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CMemberField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CMemberField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[Member][UpdateTime][isValid]");
				return NULL;
			}			
			return &CMemberField::m_Describe;
		}
	case FID_UserSession: //用户会话
		{
			string sField = "";
			sField = "UserID";
			((CUserSessionField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CUserSessionField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][UserID][isValid]");
				return NULL;
			}			
			sField = "Password";
			((CUserSessionField*)pOutput)->Password=formMap["Password"].c_str();
			if (!((CUserSessionField*)pOutput)->Password.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][Password][isValid]");
				return NULL;
			}			
			sField = "UserProductID";
			((CUserSessionField*)pOutput)->UserProductID=formMap["UserProductID"].c_str();
			if (!((CUserSessionField*)pOutput)->UserProductID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][UserProductID][isValid]");
				return NULL;
			}			
			sField = "MacAddress";
			((CUserSessionField*)pOutput)->MacAddress=formMap["MacAddress"].c_str();
			if (!((CUserSessionField*)pOutput)->MacAddress.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][MacAddress][isValid]");
				return NULL;
			}			
			sField = "ClientIPAddress";
			((CUserSessionField*)pOutput)->ClientIPAddress=formMap["ClientIPAddress"].c_str();
			if (!((CUserSessionField*)pOutput)->ClientIPAddress.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][ClientIPAddress][isValid]");
				return NULL;
			}			
			sField = "HDSerialID";
			((CUserSessionField*)pOutput)->HDSerialID=formMap["HDSerialID"].c_str();
			if (!((CUserSessionField*)pOutput)->HDSerialID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][HDSerialID][isValid]");
				return NULL;
			}			
			sField = "AuthCode";
			((CUserSessionField*)pOutput)->AuthCode=formMap["AuthCode"].c_str();
			if (!((CUserSessionField*)pOutput)->AuthCode.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][AuthCode][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CUserSessionField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CUserSessionField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][APPID][isValid]");
				return NULL;
			}			
			sField = "Token";
			((CUserSessionField*)pOutput)->Token=formMap["Token"].c_str();
			if (!((CUserSessionField*)pOutput)->Token.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][Token][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CUserSessionField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CUserSessionField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][Remark][isValid]");
				return NULL;
			}			
			sField = "APIID";
			((CUserSessionField*)pOutput)->APIID=formMap["APIID"].c_str();
			if (!((CUserSessionField*)pOutput)->APIID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][APIID][isValid]");
				return NULL;
			}			
			sField = "LoginTime";
			((CUserSessionField*)pOutput)->LoginTime=formMap["LoginTime"].c_str();
			if (!((CUserSessionField*)pOutput)->LoginTime.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][LoginTime][isValid]");
				return NULL;
			}			
			sField = "ExpireTime";
			((CUserSessionField*)pOutput)->ExpireTime=formMap["ExpireTime"].c_str();
			if (!((CUserSessionField*)pOutput)->ExpireTime.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][ExpireTime][isValid]");
				return NULL;
			}			
			sField = "IPAddress";
			((CUserSessionField*)pOutput)->IPAddress=formMap["IPAddress"].c_str();
			if (!((CUserSessionField*)pOutput)->IPAddress.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][IPAddress][isValid]");
				return NULL;
			}			
			sField = "MaxLocalID";
			((CUserSessionField*)pOutput)->MaxLocalID=formMap["MaxLocalID"].c_str();
			if (!((CUserSessionField*)pOutput)->MaxLocalID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][MaxLocalID][isValid]");
				return NULL;
			}			
			sField = "SessionNo";
			((CUserSessionField*)pOutput)->SessionNo=formMap["SessionNo"].c_str();
			if (!((CUserSessionField*)pOutput)->SessionNo.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][SessionNo][isValid]");
				return NULL;
			}			
			sField = "FrontNo";
			((CUserSessionField*)pOutput)->FrontNo=formMap["FrontNo"].c_str();
			if (!((CUserSessionField*)pOutput)->FrontNo.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][FrontNo][isValid]");
				return NULL;
			}			
			sField = "AccessLimit";
			((CUserSessionField*)pOutput)->AccessLimit=formMap["AccessLimit"].c_str();
			if (!((CUserSessionField*)pOutput)->AccessLimit.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][AccessLimit][isValid]");
				return NULL;
			}			
			sField = "UserType";
			((CUserSessionField*)pOutput)->UserType=formMap["UserType"].c_str();
			if (!((CUserSessionField*)pOutput)->UserType.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][UserType][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CUserSessionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CUserSessionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][MemberID][isValid]");
				return NULL;
			}			
			sField = "LimitAccesses";
			((CUserSessionField*)pOutput)->LimitAccesses=formMap["LimitAccesses"].c_str();
			if (!((CUserSessionField*)pOutput)->LimitAccesses.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][LimitAccesses][isValid]");
				return NULL;
			}			
			sField = "SinkType";
			((CUserSessionField*)pOutput)->SinkType=formMap["SinkType"].c_str();
			if (!((CUserSessionField*)pOutput)->SinkType.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][SinkType][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CUserSessionField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CUserSessionField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CUserSessionField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CUserSessionField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[UserSession][UpdateTime][isValid]");
				return NULL;
			}			
			return &CUserSessionField::m_Describe;
		}
	case FID_LastKLine: //最新K线
		{
			string sField = "";
			sField = "ExchangeID";
			((CLastKLineField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CLastKLineField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CLastKLineField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CLastKLineField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Bar";
			((CLastKLineField*)pOutput)->Bar=formMap["Bar"].c_str();
			if (!((CLastKLineField*)pOutput)->Bar.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][Bar][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CLastKLineField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CLastKLineField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][InsertTime][isValid]");
				return NULL;
			}			
			sField = "OpenPrice";
			((CLastKLineField*)pOutput)->OpenPrice=formMap["OpenPrice"].c_str();
			if (!((CLastKLineField*)pOutput)->OpenPrice.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][OpenPrice][isValid]");
				return NULL;
			}			
			sField = "HighestPrice";
			((CLastKLineField*)pOutput)->HighestPrice=formMap["HighestPrice"].c_str();
			if (!((CLastKLineField*)pOutput)->HighestPrice.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][HighestPrice][isValid]");
				return NULL;
			}			
			sField = "LowestPrice";
			((CLastKLineField*)pOutput)->LowestPrice=formMap["LowestPrice"].c_str();
			if (!((CLastKLineField*)pOutput)->LowestPrice.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][LowestPrice][isValid]");
				return NULL;
			}			
			sField = "ClosePrice";
			((CLastKLineField*)pOutput)->ClosePrice=formMap["ClosePrice"].c_str();
			if (!((CLastKLineField*)pOutput)->ClosePrice.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][ClosePrice][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CLastKLineField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CLastKLineField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][Volume][isValid]");
				return NULL;
			}			
			sField = "Turnover";
			((CLastKLineField*)pOutput)->Turnover=formMap["Turnover"].c_str();
			if (!((CLastKLineField*)pOutput)->Turnover.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][Turnover][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CLastKLineField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CLastKLineField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[LastKLine][UpdateTime][isValid]");
				return NULL;
			}			
			return &CLastKLineField::m_Describe;
		}
	case FID_CFDRate: //CFD溢价率
		{
			string sField = "";
			sField = "MemberID";
			((CCFDRateField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CCFDRateField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][MemberID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CCFDRateField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CCFDRateField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "TriggerOrderType";
			((CCFDRateField*)pOutput)->TriggerOrderType=formMap["TriggerOrderType"].c_str();
			if (!((CCFDRateField*)pOutput)->TriggerOrderType.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][TriggerOrderType][isValid]");
				return NULL;
			}			
			sField = "RateBegin";
			((CCFDRateField*)pOutput)->RateBegin=formMap["RateBegin"].c_str();
			if (!((CCFDRateField*)pOutput)->RateBegin.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][RateBegin][isValid]");
				return NULL;
			}			
			sField = "RateEnd";
			((CCFDRateField*)pOutput)->RateEnd=formMap["RateEnd"].c_str();
			if (!((CCFDRateField*)pOutput)->RateEnd.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][RateEnd][isValid]");
				return NULL;
			}			
			sField = "CompVolume";
			((CCFDRateField*)pOutput)->CompVolume=formMap["CompVolume"].c_str();
			if (!((CCFDRateField*)pOutput)->CompVolume.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][CompVolume][isValid]");
				return NULL;
			}			
			sField = "CompRate";
			((CCFDRateField*)pOutput)->CompRate=formMap["CompRate"].c_str();
			if (!((CCFDRateField*)pOutput)->CompRate.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][CompRate][isValid]");
				return NULL;
			}			
			sField = "HardRate";
			((CCFDRateField*)pOutput)->HardRate=formMap["HardRate"].c_str();
			if (!((CCFDRateField*)pOutput)->HardRate.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][HardRate][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CCFDRateField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CCFDRateField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][Remark][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CCFDRateField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CCFDRateField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CCFDRateField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CCFDRateField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[CFDRate][UpdateTime][isValid]");
				return NULL;
			}			
			return &CCFDRateField::m_Describe;
		}
	case FID_TradingRight: //交易权限
		{
			string sField = "";
			sField = "MemberID";
			((CTradingRightField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CTradingRightField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[TradingRight][MemberID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CTradingRightField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CTradingRightField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[TradingRight][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((CTradingRightField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((CTradingRightField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[TradingRight][Direction][isValid]");
				return NULL;
			}			
			sField = "TradingRight";
			((CTradingRightField*)pOutput)->TradingRight=formMap["TradingRight"].c_str();
			if (!((CTradingRightField*)pOutput)->TradingRight.isValid())
			{
				strcpy((char*)pOutput, "[TradingRight][TradingRight][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CTradingRightField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CTradingRightField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[TradingRight][Remark][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CTradingRightField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CTradingRightField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[TradingRight][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CTradingRightField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CTradingRightField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[TradingRight][UpdateTime][isValid]");
				return NULL;
			}			
			return &CTradingRightField::m_Describe;
		}
	case FID_MarginRate: //保证金率
		{
			string sField = "";
			sField = "MemberID";
			((CMarginRateField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CMarginRateField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][MemberID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CMarginRateField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CMarginRateField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "ValueMode";
			((CMarginRateField*)pOutput)->ValueMode=formMap["ValueMode"].c_str();
			if (!((CMarginRateField*)pOutput)->ValueMode.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][ValueMode][isValid]");
				return NULL;
			}			
			sField = "MarginSequenceType";
			((CMarginRateField*)pOutput)->MarginSequenceType=formMap["MarginSequenceType"].c_str();
			if (!((CMarginRateField*)pOutput)->MarginSequenceType.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][MarginSequenceType][isValid]");
				return NULL;
			}			
			sField = "RiskSequenceType";
			((CMarginRateField*)pOutput)->RiskSequenceType=formMap["RiskSequenceType"].c_str();
			if (!((CMarginRateField*)pOutput)->RiskSequenceType.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][RiskSequenceType][isValid]");
				return NULL;
			}			
			sField = "LongMarginRatio";
			((CMarginRateField*)pOutput)->LongMarginRatio=formMap["LongMarginRatio"].c_str();
			if (!((CMarginRateField*)pOutput)->LongMarginRatio.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][LongMarginRatio][isValid]");
				return NULL;
			}			
			sField = "LongMaintMarginRatio";
			((CMarginRateField*)pOutput)->LongMaintMarginRatio=formMap["LongMaintMarginRatio"].c_str();
			if (!((CMarginRateField*)pOutput)->LongMaintMarginRatio.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][LongMaintMarginRatio][isValid]");
				return NULL;
			}			
			sField = "LongMarginStep";
			((CMarginRateField*)pOutput)->LongMarginStep=formMap["LongMarginStep"].c_str();
			if (!((CMarginRateField*)pOutput)->LongMarginStep.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][LongMarginStep][isValid]");
				return NULL;
			}			
			sField = "LongMaintMarginStep";
			((CMarginRateField*)pOutput)->LongMaintMarginStep=formMap["LongMaintMarginStep"].c_str();
			if (!((CMarginRateField*)pOutput)->LongMaintMarginStep.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][LongMaintMarginStep][isValid]");
				return NULL;
			}			
			sField = "LongRiskLimit";
			((CMarginRateField*)pOutput)->LongRiskLimit=formMap["LongRiskLimit"].c_str();
			if (!((CMarginRateField*)pOutput)->LongRiskLimit.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][LongRiskLimit][isValid]");
				return NULL;
			}			
			sField = "LongRiskStep";
			((CMarginRateField*)pOutput)->LongRiskStep=formMap["LongRiskStep"].c_str();
			if (!((CMarginRateField*)pOutput)->LongRiskStep.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][LongRiskStep][isValid]");
				return NULL;
			}			
			sField = "ShortMarginRatio";
			((CMarginRateField*)pOutput)->ShortMarginRatio=formMap["ShortMarginRatio"].c_str();
			if (!((CMarginRateField*)pOutput)->ShortMarginRatio.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][ShortMarginRatio][isValid]");
				return NULL;
			}			
			sField = "ShortMaintMarginRatio";
			((CMarginRateField*)pOutput)->ShortMaintMarginRatio=formMap["ShortMaintMarginRatio"].c_str();
			if (!((CMarginRateField*)pOutput)->ShortMaintMarginRatio.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][ShortMaintMarginRatio][isValid]");
				return NULL;
			}			
			sField = "ShortMarginStep";
			((CMarginRateField*)pOutput)->ShortMarginStep=formMap["ShortMarginStep"].c_str();
			if (!((CMarginRateField*)pOutput)->ShortMarginStep.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][ShortMarginStep][isValid]");
				return NULL;
			}			
			sField = "ShortMaintMarginStep";
			((CMarginRateField*)pOutput)->ShortMaintMarginStep=formMap["ShortMaintMarginStep"].c_str();
			if (!((CMarginRateField*)pOutput)->ShortMaintMarginStep.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][ShortMaintMarginStep][isValid]");
				return NULL;
			}			
			sField = "ShortRiskLimit";
			((CMarginRateField*)pOutput)->ShortRiskLimit=formMap["ShortRiskLimit"].c_str();
			if (!((CMarginRateField*)pOutput)->ShortRiskLimit.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][ShortRiskLimit][isValid]");
				return NULL;
			}			
			sField = "ShortRiskStep";
			((CMarginRateField*)pOutput)->ShortRiskStep=formMap["ShortRiskStep"].c_str();
			if (!((CMarginRateField*)pOutput)->ShortRiskStep.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][ShortRiskStep][isValid]");
				return NULL;
			}			
			sField = "DefaultLeverage";
			((CMarginRateField*)pOutput)->DefaultLeverage=formMap["DefaultLeverage"].c_str();
			if (!((CMarginRateField*)pOutput)->DefaultLeverage.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][DefaultLeverage][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CMarginRateField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CMarginRateField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][Remark][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CMarginRateField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CMarginRateField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CMarginRateField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CMarginRateField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[MarginRate][UpdateTime][isValid]");
				return NULL;
			}			
			return &CMarginRateField::m_Describe;
		}
	case FID_Fee: //手续费率
		{
			string sField = "";
			sField = "MemberID";
			((CFeeField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CFeeField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[Fee][MemberID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CFeeField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CFeeField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[Fee][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "MatchRole";
			((CFeeField*)pOutput)->MatchRole=formMap["MatchRole"].c_str();
			if (!((CFeeField*)pOutput)->MatchRole.isValid())
			{
				strcpy((char*)pOutput, "[Fee][MatchRole][isValid]");
				return NULL;
			}			
			sField = "OpenFeeRate";
			((CFeeField*)pOutput)->OpenFeeRate=formMap["OpenFeeRate"].c_str();
			if (!((CFeeField*)pOutput)->OpenFeeRate.isValid())
			{
				strcpy((char*)pOutput, "[Fee][OpenFeeRate][isValid]");
				return NULL;
			}			
			sField = "OpenFeeAmount";
			((CFeeField*)pOutput)->OpenFeeAmount=formMap["OpenFeeAmount"].c_str();
			if (!((CFeeField*)pOutput)->OpenFeeAmount.isValid())
			{
				strcpy((char*)pOutput, "[Fee][OpenFeeAmount][isValid]");
				return NULL;
			}			
			sField = "CloseFeeRate";
			((CFeeField*)pOutput)->CloseFeeRate=formMap["CloseFeeRate"].c_str();
			if (!((CFeeField*)pOutput)->CloseFeeRate.isValid())
			{
				strcpy((char*)pOutput, "[Fee][CloseFeeRate][isValid]");
				return NULL;
			}			
			sField = "CloseFeeAmount";
			((CFeeField*)pOutput)->CloseFeeAmount=formMap["CloseFeeAmount"].c_str();
			if (!((CFeeField*)pOutput)->CloseFeeAmount.isValid())
			{
				strcpy((char*)pOutput, "[Fee][CloseFeeAmount][isValid]");
				return NULL;
			}			
			sField = "CloseTodayFeeRate";
			((CFeeField*)pOutput)->CloseTodayFeeRate=formMap["CloseTodayFeeRate"].c_str();
			if (!((CFeeField*)pOutput)->CloseTodayFeeRate.isValid())
			{
				strcpy((char*)pOutput, "[Fee][CloseTodayFeeRate][isValid]");
				return NULL;
			}			
			sField = "CloseTodayFeeAmount";
			((CFeeField*)pOutput)->CloseTodayFeeAmount=formMap["CloseTodayFeeAmount"].c_str();
			if (!((CFeeField*)pOutput)->CloseTodayFeeAmount.isValid())
			{
				strcpy((char*)pOutput, "[Fee][CloseTodayFeeAmount][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CFeeField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CFeeField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Fee][Remark][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CFeeField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CFeeField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[Fee][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CFeeField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CFeeField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[Fee][UpdateTime][isValid]");
				return NULL;
			}			
			return &CFeeField::m_Describe;
		}
	case FID_Liquidity: //流动性设置表
		{
			string sField = "";
			sField = "LiquidityGrade";
			((CLiquidityField*)pOutput)->LiquidityGrade=formMap["LiquidityGrade"].c_str();
			if (!((CLiquidityField*)pOutput)->LiquidityGrade.isValid())
			{
				strcpy((char*)pOutput, "[Liquidity][LiquidityGrade][isValid]");
				return NULL;
			}			
			sField = "LiquidityGroup";
			((CLiquidityField*)pOutput)->LiquidityGroup=formMap["LiquidityGroup"].c_str();
			if (!((CLiquidityField*)pOutput)->LiquidityGroup.isValid())
			{
				strcpy((char*)pOutput, "[Liquidity][LiquidityGroup][isValid]");
				return NULL;
			}			
			sField = "BasePriceType";
			((CLiquidityField*)pOutput)->BasePriceType=formMap["BasePriceType"].c_str();
			if (!((CLiquidityField*)pOutput)->BasePriceType.isValid())
			{
				strcpy((char*)pOutput, "[Liquidity][BasePriceType][isValid]");
				return NULL;
			}			
			sField = "VolumeBegin";
			((CLiquidityField*)pOutput)->VolumeBegin=formMap["VolumeBegin"].c_str();
			if (!((CLiquidityField*)pOutput)->VolumeBegin.isValid())
			{
				strcpy((char*)pOutput, "[Liquidity][VolumeBegin][isValid]");
				return NULL;
			}			
			sField = "BuyOpen";
			((CLiquidityField*)pOutput)->BuyOpen=formMap["BuyOpen"].c_str();
			if (!((CLiquidityField*)pOutput)->BuyOpen.isValid())
			{
				strcpy((char*)pOutput, "[Liquidity][BuyOpen][isValid]");
				return NULL;
			}			
			sField = "BuyClose";
			((CLiquidityField*)pOutput)->BuyClose=formMap["BuyClose"].c_str();
			if (!((CLiquidityField*)pOutput)->BuyClose.isValid())
			{
				strcpy((char*)pOutput, "[Liquidity][BuyClose][isValid]");
				return NULL;
			}			
			sField = "SellOpen";
			((CLiquidityField*)pOutput)->SellOpen=formMap["SellOpen"].c_str();
			if (!((CLiquidityField*)pOutput)->SellOpen.isValid())
			{
				strcpy((char*)pOutput, "[Liquidity][SellOpen][isValid]");
				return NULL;
			}			
			sField = "SellClose";
			((CLiquidityField*)pOutput)->SellClose=formMap["SellClose"].c_str();
			if (!((CLiquidityField*)pOutput)->SellClose.isValid())
			{
				strcpy((char*)pOutput, "[Liquidity][SellClose][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CLiquidityField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CLiquidityField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[Liquidity][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CLiquidityField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CLiquidityField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[Liquidity][UpdateTime][isValid]");
				return NULL;
			}			
			return &CLiquidityField::m_Describe;
		}
	case FID_Account: //账户资金
		{
			string sField = "";
			sField = "SettlementGroup";
			((CAccountField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CAccountField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Account][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CAccountField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CAccountField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[Account][AccountID][isValid]");
				return NULL;
			}			
			sField = "Currency";
			((CAccountField*)pOutput)->Currency=formMap["Currency"].c_str();
			if (!((CAccountField*)pOutput)->Currency.isValid())
			{
				strcpy((char*)pOutput, "[Account][Currency][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CAccountField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CAccountField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[Account][MemberID][isValid]");
				return NULL;
			}			
			sField = "Available";
			((CAccountField*)pOutput)->Available=formMap["Available"].c_str();
			if (!((CAccountField*)pOutput)->Available.isValid())
			{
				strcpy((char*)pOutput, "[Account][Available][isValid]");
				return NULL;
			}			
			sField = "Withdrawable";
			((CAccountField*)pOutput)->Withdrawable=formMap["Withdrawable"].c_str();
			if (!((CAccountField*)pOutput)->Withdrawable.isValid())
			{
				strcpy((char*)pOutput, "[Account][Withdrawable][isValid]");
				return NULL;
			}			
			sField = "PreBalance";
			((CAccountField*)pOutput)->PreBalance=formMap["PreBalance"].c_str();
			if (!((CAccountField*)pOutput)->PreBalance.isValid())
			{
				strcpy((char*)pOutput, "[Account][PreBalance][isValid]");
				return NULL;
			}			
			sField = "Balance";
			((CAccountField*)pOutput)->Balance=formMap["Balance"].c_str();
			if (!((CAccountField*)pOutput)->Balance.isValid())
			{
				strcpy((char*)pOutput, "[Account][Balance][isValid]");
				return NULL;
			}			
			sField = "PositionCost";
			((CAccountField*)pOutput)->PositionCost=formMap["PositionCost"].c_str();
			if (!((CAccountField*)pOutput)->PositionCost.isValid())
			{
				strcpy((char*)pOutput, "[Account][PositionCost][isValid]");
				return NULL;
			}			
			sField = "UseMargin";
			((CAccountField*)pOutput)->UseMargin=formMap["UseMargin"].c_str();
			if (!((CAccountField*)pOutput)->UseMargin.isValid())
			{
				strcpy((char*)pOutput, "[Account][UseMargin][isValid]");
				return NULL;
			}			
			sField = "CrossMargin";
			((CAccountField*)pOutput)->CrossMargin=formMap["CrossMargin"].c_str();
			if (!((CAccountField*)pOutput)->CrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[Account][CrossMargin][isValid]");
				return NULL;
			}			
			sField = "CloseProfit";
			((CAccountField*)pOutput)->CloseProfit=formMap["CloseProfit"].c_str();
			if (!((CAccountField*)pOutput)->CloseProfit.isValid())
			{
				strcpy((char*)pOutput, "[Account][CloseProfit][isValid]");
				return NULL;
			}			
			sField = "Deposit";
			((CAccountField*)pOutput)->Deposit=formMap["Deposit"].c_str();
			if (!((CAccountField*)pOutput)->Deposit.isValid())
			{
				strcpy((char*)pOutput, "[Account][Deposit][isValid]");
				return NULL;
			}			
			sField = "Withdraw";
			((CAccountField*)pOutput)->Withdraw=formMap["Withdraw"].c_str();
			if (!((CAccountField*)pOutput)->Withdraw.isValid())
			{
				strcpy((char*)pOutput, "[Account][Withdraw][isValid]");
				return NULL;
			}			
			sField = "FrozenMargin";
			((CAccountField*)pOutput)->FrozenMargin=formMap["FrozenMargin"].c_str();
			if (!((CAccountField*)pOutput)->FrozenMargin.isValid())
			{
				strcpy((char*)pOutput, "[Account][FrozenMargin][isValid]");
				return NULL;
			}			
			sField = "FrozenMoney";
			((CAccountField*)pOutput)->FrozenMoney=formMap["FrozenMoney"].c_str();
			if (!((CAccountField*)pOutput)->FrozenMoney.isValid())
			{
				strcpy((char*)pOutput, "[Account][FrozenMoney][isValid]");
				return NULL;
			}			
			sField = "FrozenFee";
			((CAccountField*)pOutput)->FrozenFee=formMap["FrozenFee"].c_str();
			if (!((CAccountField*)pOutput)->FrozenFee.isValid())
			{
				strcpy((char*)pOutput, "[Account][FrozenFee][isValid]");
				return NULL;
			}			
			sField = "Fee";
			((CAccountField*)pOutput)->Fee=formMap["Fee"].c_str();
			if (!((CAccountField*)pOutput)->Fee.isValid())
			{
				strcpy((char*)pOutput, "[Account][Fee][isValid]");
				return NULL;
			}			
			sField = "MoneyChange";
			((CAccountField*)pOutput)->MoneyChange=formMap["MoneyChange"].c_str();
			if (!((CAccountField*)pOutput)->MoneyChange.isValid())
			{
				strcpy((char*)pOutput, "[Account][MoneyChange][isValid]");
				return NULL;
			}			
			sField = "Reserve";
			((CAccountField*)pOutput)->Reserve=formMap["Reserve"].c_str();
			if (!((CAccountField*)pOutput)->Reserve.isValid())
			{
				strcpy((char*)pOutput, "[Account][Reserve][isValid]");
				return NULL;
			}			
			sField = "ReserveAvailable";
			((CAccountField*)pOutput)->ReserveAvailable=formMap["ReserveAvailable"].c_str();
			if (!((CAccountField*)pOutput)->ReserveAvailable.isValid())
			{
				strcpy((char*)pOutput, "[Account][ReserveAvailable][isValid]");
				return NULL;
			}			
			sField = "ReserveLimit";
			((CAccountField*)pOutput)->ReserveLimit=formMap["ReserveLimit"].c_str();
			if (!((CAccountField*)pOutput)->ReserveLimit.isValid())
			{
				strcpy((char*)pOutput, "[Account][ReserveLimit][isValid]");
				return NULL;
			}			
			sField = "ReserveProfit";
			((CAccountField*)pOutput)->ReserveProfit=formMap["ReserveProfit"].c_str();
			if (!((CAccountField*)pOutput)->ReserveProfit.isValid())
			{
				strcpy((char*)pOutput, "[Account][ReserveProfit][isValid]");
				return NULL;
			}			
			sField = "ReserveType";
			((CAccountField*)pOutput)->ReserveType=formMap["ReserveType"].c_str();
			if (!((CAccountField*)pOutput)->ReserveType.isValid())
			{
				strcpy((char*)pOutput, "[Account][ReserveType][isValid]");
				return NULL;
			}			
			sField = "ReserveActive";
			((CAccountField*)pOutput)->ReserveActive=formMap["ReserveActive"].c_str();
			if (!((CAccountField*)pOutput)->ReserveActive.isValid())
			{
				strcpy((char*)pOutput, "[Account][ReserveActive][isValid]");
				return NULL;
			}			
			sField = "TotalCloseProfit";
			((CAccountField*)pOutput)->TotalCloseProfit=formMap["TotalCloseProfit"].c_str();
			if (!((CAccountField*)pOutput)->TotalCloseProfit.isValid())
			{
				strcpy((char*)pOutput, "[Account][TotalCloseProfit][isValid]");
				return NULL;
			}			
			sField = "TotalDeposit";
			((CAccountField*)pOutput)->TotalDeposit=formMap["TotalDeposit"].c_str();
			if (!((CAccountField*)pOutput)->TotalDeposit.isValid())
			{
				strcpy((char*)pOutput, "[Account][TotalDeposit][isValid]");
				return NULL;
			}			
			sField = "TotalWithdraw";
			((CAccountField*)pOutput)->TotalWithdraw=formMap["TotalWithdraw"].c_str();
			if (!((CAccountField*)pOutput)->TotalWithdraw.isValid())
			{
				strcpy((char*)pOutput, "[Account][TotalWithdraw][isValid]");
				return NULL;
			}			
			sField = "TotalFee";
			((CAccountField*)pOutput)->TotalFee=formMap["TotalFee"].c_str();
			if (!((CAccountField*)pOutput)->TotalFee.isValid())
			{
				strcpy((char*)pOutput, "[Account][TotalFee][isValid]");
				return NULL;
			}			
			sField = "TotalMoneyChange";
			((CAccountField*)pOutput)->TotalMoneyChange=formMap["TotalMoneyChange"].c_str();
			if (!((CAccountField*)pOutput)->TotalMoneyChange.isValid())
			{
				strcpy((char*)pOutput, "[Account][TotalMoneyChange][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CAccountField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CAccountField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[Account][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CAccountField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CAccountField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[Account][UpdateTime][isValid]");
				return NULL;
			}			
			sField = "AccountType";
			((CAccountField*)pOutput)->AccountType=formMap["AccountType"].c_str();
			if (!((CAccountField*)pOutput)->AccountType.isValid())
			{
				strcpy((char*)pOutput, "[Account][AccountType][isValid]");
				return NULL;
			}			
			sField = "BusinessNo";
			((CAccountField*)pOutput)->BusinessNo=formMap["BusinessNo"].c_str();
			if (!((CAccountField*)pOutput)->BusinessNo.isValid())
			{
				strcpy((char*)pOutput, "[Account][BusinessNo][isValid]");
				return NULL;
			}			
			sField = "MarginRateGrade";
			((CAccountField*)pOutput)->MarginRateGrade=formMap["MarginRateGrade"].c_str();
			if (!((CAccountField*)pOutput)->MarginRateGrade.isValid())
			{
				strcpy((char*)pOutput, "[Account][MarginRateGrade][isValid]");
				return NULL;
			}			
			sField = "TradingRightGrade";
			((CAccountField*)pOutput)->TradingRightGrade=formMap["TradingRightGrade"].c_str();
			if (!((CAccountField*)pOutput)->TradingRightGrade.isValid())
			{
				strcpy((char*)pOutput, "[Account][TradingRightGrade][isValid]");
				return NULL;
			}			
			sField = "Frequency";
			((CAccountField*)pOutput)->Frequency=formMap["Frequency"].c_str();
			if (!((CAccountField*)pOutput)->Frequency.isValid())
			{
				strcpy((char*)pOutput, "[Account][Frequency][isValid]");
				return NULL;
			}			
			sField = "MaintMargin";
			((CAccountField*)pOutput)->MaintMargin=formMap["MaintMargin"].c_str();
			if (!((CAccountField*)pOutput)->MaintMargin.isValid())
			{
				strcpy((char*)pOutput, "[Account][MaintMargin][isValid]");
				return NULL;
			}			
			sField = "UnRealProfit";
			((CAccountField*)pOutput)->UnRealProfit=formMap["UnRealProfit"].c_str();
			if (!((CAccountField*)pOutput)->UnRealProfit.isValid())
			{
				strcpy((char*)pOutput, "[Account][UnRealProfit][isValid]");
				return NULL;
			}			
			sField = "CopyProfit";
			((CAccountField*)pOutput)->CopyProfit=formMap["CopyProfit"].c_str();
			if (!((CAccountField*)pOutput)->CopyProfit.isValid())
			{
				strcpy((char*)pOutput, "[Account][CopyProfit][isValid]");
				return NULL;
			}			
			sField = "CFDType";
			((CAccountField*)pOutput)->CFDType=formMap["CFDType"].c_str();
			if (!((CAccountField*)pOutput)->CFDType.isValid())
			{
				strcpy((char*)pOutput, "[Account][CFDType][isValid]");
				return NULL;
			}			
			sField = "FeeGrade";
			((CAccountField*)pOutput)->FeeGrade=formMap["FeeGrade"].c_str();
			if (!((CAccountField*)pOutput)->FeeGrade.isValid())
			{
				strcpy((char*)pOutput, "[Account][FeeGrade][isValid]");
				return NULL;
			}			
			sField = "CFDGrade";
			((CAccountField*)pOutput)->CFDGrade=formMap["CFDGrade"].c_str();
			if (!((CAccountField*)pOutput)->CFDGrade.isValid())
			{
				strcpy((char*)pOutput, "[Account][CFDGrade][isValid]");
				return NULL;
			}			
			sField = "Borrow";
			((CAccountField*)pOutput)->Borrow=formMap["Borrow"].c_str();
			if (!((CAccountField*)pOutput)->Borrow.isValid())
			{
				strcpy((char*)pOutput, "[Account][Borrow][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CAccountField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CAccountField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Account][Remark][isValid]");
				return NULL;
			}			
			sField = "MaxLocalID";
			((CAccountField*)pOutput)->MaxLocalID=formMap["MaxLocalID"].c_str();
			if (!((CAccountField*)pOutput)->MaxLocalID.isValid())
			{
				strcpy((char*)pOutput, "[Account][MaxLocalID][isValid]");
				return NULL;
			}			
			sField = "CurrValue";
			((CAccountField*)pOutput)->CurrValue=formMap["CurrValue"].c_str();
			if (!((CAccountField*)pOutput)->CurrValue.isValid())
			{
				strcpy((char*)pOutput, "[Account][CurrValue][isValid]");
				return NULL;
			}			
			sField = "CanReduce";
			((CAccountField*)pOutput)->CanReduce=formMap["CanReduce"].c_str();
			if (!((CAccountField*)pOutput)->CanReduce.isValid())
			{
				strcpy((char*)pOutput, "[Account][CanReduce][isValid]");
				return NULL;
			}			
			return &CAccountField::m_Describe;
		}
	case FID_MemberPosition: //成员持仓汇总
		{
			string sField = "";
			sField = "MemberID";
			((CMemberPositionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CMemberPositionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][MemberID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CMemberPositionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CMemberPositionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][AccountID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CMemberPositionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CMemberPositionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CMemberPositionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CMemberPositionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "Position";
			((CMemberPositionField*)pOutput)->Position=formMap["Position"].c_str();
			if (!((CMemberPositionField*)pOutput)->Position.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][Position][isValid]");
				return NULL;
			}			
			sField = "LongFrozen";
			((CMemberPositionField*)pOutput)->LongFrozen=formMap["LongFrozen"].c_str();
			if (!((CMemberPositionField*)pOutput)->LongFrozen.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][LongFrozen][isValid]");
				return NULL;
			}			
			sField = "ShortFrozen";
			((CMemberPositionField*)pOutput)->ShortFrozen=formMap["ShortFrozen"].c_str();
			if (!((CMemberPositionField*)pOutput)->ShortFrozen.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][ShortFrozen][isValid]");
				return NULL;
			}			
			sField = "DefaultLeverage";
			((CMemberPositionField*)pOutput)->DefaultLeverage=formMap["DefaultLeverage"].c_str();
			if (!((CMemberPositionField*)pOutput)->DefaultLeverage.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][DefaultLeverage][isValid]");
				return NULL;
			}			
			sField = "DefaultCrossMargin";
			((CMemberPositionField*)pOutput)->DefaultCrossMargin=formMap["DefaultCrossMargin"].c_str();
			if (!((CMemberPositionField*)pOutput)->DefaultCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][DefaultCrossMargin][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CMemberPositionField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CMemberPositionField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CMemberPositionField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CMemberPositionField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[MemberPosition][UpdateTime][isValid]");
				return NULL;
			}			
			return &CMemberPositionField::m_Describe;
		}
	case FID_Position: //标的持仓
		{
			string sField = "";
			sField = "PositionID";
			((CPositionField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((CPositionField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[Position][PositionID][isValid]");
				return NULL;
			}			
			sField = "MemberID";
			((CPositionField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CPositionField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[Position][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CPositionField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CPositionField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[Position][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CPositionField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CPositionField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[Position][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CPositionField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CPositionField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[Position][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "PositionClass";
			((CPositionField*)pOutput)->PositionClass=formMap["PositionClass"].c_str();
			if (!((CPositionField*)pOutput)->PositionClass.isValid())
			{
				strcpy((char*)pOutput, "[Position][PositionClass][isValid]");
				return NULL;
			}			
			sField = "PosiDirection";
			((CPositionField*)pOutput)->PosiDirection=formMap["PosiDirection"].c_str();
			if (!((CPositionField*)pOutput)->PosiDirection.isValid())
			{
				strcpy((char*)pOutput, "[Position][PosiDirection][isValid]");
				return NULL;
			}			
			sField = "PrePosition";
			((CPositionField*)pOutput)->PrePosition=formMap["PrePosition"].c_str();
			if (!((CPositionField*)pOutput)->PrePosition.isValid())
			{
				strcpy((char*)pOutput, "[Position][PrePosition][isValid]");
				return NULL;
			}			
			sField = "Position";
			((CPositionField*)pOutput)->Position=formMap["Position"].c_str();
			if (!((CPositionField*)pOutput)->Position.isValid())
			{
				strcpy((char*)pOutput, "[Position][Position][isValid]");
				return NULL;
			}			
			sField = "LongFrozen";
			((CPositionField*)pOutput)->LongFrozen=formMap["LongFrozen"].c_str();
			if (!((CPositionField*)pOutput)->LongFrozen.isValid())
			{
				strcpy((char*)pOutput, "[Position][LongFrozen][isValid]");
				return NULL;
			}			
			sField = "ShortFrozen";
			((CPositionField*)pOutput)->ShortFrozen=formMap["ShortFrozen"].c_str();
			if (!((CPositionField*)pOutput)->ShortFrozen.isValid())
			{
				strcpy((char*)pOutput, "[Position][ShortFrozen][isValid]");
				return NULL;
			}			
			sField = "PreLongFrozen";
			((CPositionField*)pOutput)->PreLongFrozen=formMap["PreLongFrozen"].c_str();
			if (!((CPositionField*)pOutput)->PreLongFrozen.isValid())
			{
				strcpy((char*)pOutput, "[Position][PreLongFrozen][isValid]");
				return NULL;
			}			
			sField = "PreShortFrozen";
			((CPositionField*)pOutput)->PreShortFrozen=formMap["PreShortFrozen"].c_str();
			if (!((CPositionField*)pOutput)->PreShortFrozen.isValid())
			{
				strcpy((char*)pOutput, "[Position][PreShortFrozen][isValid]");
				return NULL;
			}			
			sField = "HighestPosition";
			((CPositionField*)pOutput)->HighestPosition=formMap["HighestPosition"].c_str();
			if (!((CPositionField*)pOutput)->HighestPosition.isValid())
			{
				strcpy((char*)pOutput, "[Position][HighestPosition][isValid]");
				return NULL;
			}			
			sField = "ClosePosition";
			((CPositionField*)pOutput)->ClosePosition=formMap["ClosePosition"].c_str();
			if (!((CPositionField*)pOutput)->ClosePosition.isValid())
			{
				strcpy((char*)pOutput, "[Position][ClosePosition][isValid]");
				return NULL;
			}			
			sField = "PositionCost";
			((CPositionField*)pOutput)->PositionCost=formMap["PositionCost"].c_str();
			if (!((CPositionField*)pOutput)->PositionCost.isValid())
			{
				strcpy((char*)pOutput, "[Position][PositionCost][isValid]");
				return NULL;
			}			
			sField = "CostPrice";
			((CPositionField*)pOutput)->CostPrice=formMap["CostPrice"].c_str();
			if (!((CPositionField*)pOutput)->CostPrice.isValid())
			{
				strcpy((char*)pOutput, "[Position][CostPrice][isValid]");
				return NULL;
			}			
			sField = "UseMargin";
			((CPositionField*)pOutput)->UseMargin=formMap["UseMargin"].c_str();
			if (!((CPositionField*)pOutput)->UseMargin.isValid())
			{
				strcpy((char*)pOutput, "[Position][UseMargin][isValid]");
				return NULL;
			}			
			sField = "FrozenMargin";
			((CPositionField*)pOutput)->FrozenMargin=formMap["FrozenMargin"].c_str();
			if (!((CPositionField*)pOutput)->FrozenMargin.isValid())
			{
				strcpy((char*)pOutput, "[Position][FrozenMargin][isValid]");
				return NULL;
			}			
			sField = "LongFrozenMargin";
			((CPositionField*)pOutput)->LongFrozenMargin=formMap["LongFrozenMargin"].c_str();
			if (!((CPositionField*)pOutput)->LongFrozenMargin.isValid())
			{
				strcpy((char*)pOutput, "[Position][LongFrozenMargin][isValid]");
				return NULL;
			}			
			sField = "ShortFrozenMargin";
			((CPositionField*)pOutput)->ShortFrozenMargin=formMap["ShortFrozenMargin"].c_str();
			if (!((CPositionField*)pOutput)->ShortFrozenMargin.isValid())
			{
				strcpy((char*)pOutput, "[Position][ShortFrozenMargin][isValid]");
				return NULL;
			}			
			sField = "CloseProfit";
			((CPositionField*)pOutput)->CloseProfit=formMap["CloseProfit"].c_str();
			if (!((CPositionField*)pOutput)->CloseProfit.isValid())
			{
				strcpy((char*)pOutput, "[Position][CloseProfit][isValid]");
				return NULL;
			}			
			sField = "TotalPositionCost";
			((CPositionField*)pOutput)->TotalPositionCost=formMap["TotalPositionCost"].c_str();
			if (!((CPositionField*)pOutput)->TotalPositionCost.isValid())
			{
				strcpy((char*)pOutput, "[Position][TotalPositionCost][isValid]");
				return NULL;
			}			
			sField = "TotalCloseProfit";
			((CPositionField*)pOutput)->TotalCloseProfit=formMap["TotalCloseProfit"].c_str();
			if (!((CPositionField*)pOutput)->TotalCloseProfit.isValid())
			{
				strcpy((char*)pOutput, "[Position][TotalCloseProfit][isValid]");
				return NULL;
			}			
			sField = "OpenPrice";
			((CPositionField*)pOutput)->OpenPrice=formMap["OpenPrice"].c_str();
			if (!((CPositionField*)pOutput)->OpenPrice.isValid())
			{
				strcpy((char*)pOutput, "[Position][OpenPrice][isValid]");
				return NULL;
			}			
			sField = "ClosePrice";
			((CPositionField*)pOutput)->ClosePrice=formMap["ClosePrice"].c_str();
			if (!((CPositionField*)pOutput)->ClosePrice.isValid())
			{
				strcpy((char*)pOutput, "[Position][ClosePrice][isValid]");
				return NULL;
			}			
			sField = "TradeFee";
			((CPositionField*)pOutput)->TradeFee=formMap["TradeFee"].c_str();
			if (!((CPositionField*)pOutput)->TradeFee.isValid())
			{
				strcpy((char*)pOutput, "[Position][TradeFee][isValid]");
				return NULL;
			}			
			sField = "PositionFee";
			((CPositionField*)pOutput)->PositionFee=formMap["PositionFee"].c_str();
			if (!((CPositionField*)pOutput)->PositionFee.isValid())
			{
				strcpy((char*)pOutput, "[Position][PositionFee][isValid]");
				return NULL;
			}			
			sField = "Leverage";
			((CPositionField*)pOutput)->Leverage=formMap["Leverage"].c_str();
			if (!((CPositionField*)pOutput)->Leverage.isValid())
			{
				strcpy((char*)pOutput, "[Position][Leverage][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CPositionField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CPositionField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[Position][AccountID][isValid]");
				return NULL;
			}			
			sField = "PriceCurrency";
			((CPositionField*)pOutput)->PriceCurrency=formMap["PriceCurrency"].c_str();
			if (!((CPositionField*)pOutput)->PriceCurrency.isValid())
			{
				strcpy((char*)pOutput, "[Position][PriceCurrency][isValid]");
				return NULL;
			}			
			sField = "ClearCurrency";
			((CPositionField*)pOutput)->ClearCurrency=formMap["ClearCurrency"].c_str();
			if (!((CPositionField*)pOutput)->ClearCurrency.isValid())
			{
				strcpy((char*)pOutput, "[Position][ClearCurrency][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CPositionField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CPositionField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Position][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "IsCrossMargin";
			((CPositionField*)pOutput)->IsCrossMargin=formMap["IsCrossMargin"].c_str();
			if (!((CPositionField*)pOutput)->IsCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[Position][IsCrossMargin][isValid]");
				return NULL;
			}			
			sField = "CloseOrderID";
			((CPositionField*)pOutput)->CloseOrderID=formMap["CloseOrderID"].c_str();
			if (!((CPositionField*)pOutput)->CloseOrderID.isValid())
			{
				strcpy((char*)pOutput, "[Position][CloseOrderID][isValid]");
				return NULL;
			}			
			sField = "SLTriggerPrice";
			((CPositionField*)pOutput)->SLTriggerPrice=formMap["SLTriggerPrice"].c_str();
			if (!((CPositionField*)pOutput)->SLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[Position][SLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TPTriggerPrice";
			((CPositionField*)pOutput)->TPTriggerPrice=formMap["TPTriggerPrice"].c_str();
			if (!((CPositionField*)pOutput)->TPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[Position][TPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "BeginTime";
			((CPositionField*)pOutput)->BeginTime=formMap["BeginTime"].c_str();
			if (!((CPositionField*)pOutput)->BeginTime.isValid())
			{
				strcpy((char*)pOutput, "[Position][BeginTime][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CPositionField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CPositionField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[Position][InsertTime][isValid]");
				return NULL;
			}			
			sField = "LastOpenTime";
			((CPositionField*)pOutput)->LastOpenTime=formMap["LastOpenTime"].c_str();
			if (!((CPositionField*)pOutput)->LastOpenTime.isValid())
			{
				strcpy((char*)pOutput, "[Position][LastOpenTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CPositionField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CPositionField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[Position][UpdateTime][isValid]");
				return NULL;
			}			
			sField = "BusinessNo";
			((CPositionField*)pOutput)->BusinessNo=formMap["BusinessNo"].c_str();
			if (!((CPositionField*)pOutput)->BusinessNo.isValid())
			{
				strcpy((char*)pOutput, "[Position][BusinessNo][isValid]");
				return NULL;
			}			
			sField = "IsAutoAddMargin";
			((CPositionField*)pOutput)->IsAutoAddMargin=formMap["IsAutoAddMargin"].c_str();
			if (!((CPositionField*)pOutput)->IsAutoAddMargin.isValid())
			{
				strcpy((char*)pOutput, "[Position][IsAutoAddMargin][isValid]");
				return NULL;
			}			
			sField = "Frequency";
			((CPositionField*)pOutput)->Frequency=formMap["Frequency"].c_str();
			if (!((CPositionField*)pOutput)->Frequency.isValid())
			{
				strcpy((char*)pOutput, "[Position][Frequency][isValid]");
				return NULL;
			}			
			sField = "MaintMargin";
			((CPositionField*)pOutput)->MaintMargin=formMap["MaintMargin"].c_str();
			if (!((CPositionField*)pOutput)->MaintMargin.isValid())
			{
				strcpy((char*)pOutput, "[Position][MaintMargin][isValid]");
				return NULL;
			}			
			sField = "UnRealProfit";
			((CPositionField*)pOutput)->UnRealProfit=formMap["UnRealProfit"].c_str();
			if (!((CPositionField*)pOutput)->UnRealProfit.isValid())
			{
				strcpy((char*)pOutput, "[Position][UnRealProfit][isValid]");
				return NULL;
			}			
			sField = "LiquidPrice";
			((CPositionField*)pOutput)->LiquidPrice=formMap["LiquidPrice"].c_str();
			if (!((CPositionField*)pOutput)->LiquidPrice.isValid())
			{
				strcpy((char*)pOutput, "[Position][LiquidPrice][isValid]");
				return NULL;
			}			
			sField = "CreateTime";
			((CPositionField*)pOutput)->CreateTime=formMap["CreateTime"].c_str();
			if (!((CPositionField*)pOutput)->CreateTime.isValid())
			{
				strcpy((char*)pOutput, "[Position][CreateTime][isValid]");
				return NULL;
			}			
			sField = "CopyMemberID";
			((CPositionField*)pOutput)->CopyMemberID=formMap["CopyMemberID"].c_str();
			if (!((CPositionField*)pOutput)->CopyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[Position][CopyMemberID][isValid]");
				return NULL;
			}			
			sField = "CopyProfitRate";
			((CPositionField*)pOutput)->CopyProfitRate=formMap["CopyProfitRate"].c_str();
			if (!((CPositionField*)pOutput)->CopyProfitRate.isValid())
			{
				strcpy((char*)pOutput, "[Position][CopyProfitRate][isValid]");
				return NULL;
			}			
			sField = "CopyProfit";
			((CPositionField*)pOutput)->CopyProfit=formMap["CopyProfit"].c_str();
			if (!((CPositionField*)pOutput)->CopyProfit.isValid())
			{
				strcpy((char*)pOutput, "[Position][CopyProfit][isValid]");
				return NULL;
			}			
			sField = "FirstTradeID";
			((CPositionField*)pOutput)->FirstTradeID=formMap["FirstTradeID"].c_str();
			if (!((CPositionField*)pOutput)->FirstTradeID.isValid())
			{
				strcpy((char*)pOutput, "[Position][FirstTradeID][isValid]");
				return NULL;
			}			
			sField = "LastTradeID";
			((CPositionField*)pOutput)->LastTradeID=formMap["LastTradeID"].c_str();
			if (!((CPositionField*)pOutput)->LastTradeID.isValid())
			{
				strcpy((char*)pOutput, "[Position][LastTradeID][isValid]");
				return NULL;
			}			
			sField = "BusinessType";
			((CPositionField*)pOutput)->BusinessType=formMap["BusinessType"].c_str();
			if (!((CPositionField*)pOutput)->BusinessType.isValid())
			{
				strcpy((char*)pOutput, "[Position][BusinessType][isValid]");
				return NULL;
			}			
			sField = "BusinessValue";
			((CPositionField*)pOutput)->BusinessValue=formMap["BusinessValue"].c_str();
			if (!((CPositionField*)pOutput)->BusinessValue.isValid())
			{
				strcpy((char*)pOutput, "[Position][BusinessValue][isValid]");
				return NULL;
			}			
			sField = "Reserve";
			((CPositionField*)pOutput)->Reserve=formMap["Reserve"].c_str();
			if (!((CPositionField*)pOutput)->Reserve.isValid())
			{
				strcpy((char*)pOutput, "[Position][Reserve][isValid]");
				return NULL;
			}			
			sField = "ReserveProfit";
			((CPositionField*)pOutput)->ReserveProfit=formMap["ReserveProfit"].c_str();
			if (!((CPositionField*)pOutput)->ReserveProfit.isValid())
			{
				strcpy((char*)pOutput, "[Position][ReserveProfit][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CPositionField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CPositionField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Position][Remark][isValid]");
				return NULL;
			}			
			return &CPositionField::m_Describe;
		}
	case FID_Order: //报单
		{
			string sField = "";
			sField = "MemberID";
			((COrderField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((COrderField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[Order][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((COrderField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((COrderField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[Order][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((COrderField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((COrderField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[Order][AccountID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((COrderField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((COrderField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[Order][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((COrderField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((COrderField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[Order][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((COrderField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((COrderField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[Order][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "OrderPriceType";
			((COrderField*)pOutput)->OrderPriceType=formMap["OrderPriceType"].c_str();
			if (!((COrderField*)pOutput)->OrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[Order][OrderPriceType][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((COrderField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((COrderField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[Order][Direction][isValid]");
				return NULL;
			}			
			sField = "OffsetFlag";
			((COrderField*)pOutput)->OffsetFlag=formMap["OffsetFlag"].c_str();
			if (!((COrderField*)pOutput)->OffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[Order][OffsetFlag][isValid]");
				return NULL;
			}			
			sField = "Price";
			((COrderField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((COrderField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[Order][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((COrderField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((COrderField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[Order][Volume][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((COrderField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((COrderField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[Order][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "VolumeMode";
			((COrderField*)pOutput)->VolumeMode=formMap["VolumeMode"].c_str();
			if (!((COrderField*)pOutput)->VolumeMode.isValid())
			{
				strcpy((char*)pOutput, "[Order][VolumeMode][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((COrderField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((COrderField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[Order][Cost][isValid]");
				return NULL;
			}			
			sField = "OrderType";
			((COrderField*)pOutput)->OrderType=formMap["OrderType"].c_str();
			if (!((COrderField*)pOutput)->OrderType.isValid())
			{
				strcpy((char*)pOutput, "[Order][OrderType][isValid]");
				return NULL;
			}			
			sField = "GTDTime";
			((COrderField*)pOutput)->GTDTime=formMap["GTDTime"].c_str();
			if (!((COrderField*)pOutput)->GTDTime.isValid())
			{
				strcpy((char*)pOutput, "[Order][GTDTime][isValid]");
				return NULL;
			}			
			sField = "MinVolume";
			((COrderField*)pOutput)->MinVolume=formMap["MinVolume"].c_str();
			if (!((COrderField*)pOutput)->MinVolume.isValid())
			{
				strcpy((char*)pOutput, "[Order][MinVolume][isValid]");
				return NULL;
			}			
			sField = "BusinessType";
			((COrderField*)pOutput)->BusinessType=formMap["BusinessType"].c_str();
			if (!((COrderField*)pOutput)->BusinessType.isValid())
			{
				strcpy((char*)pOutput, "[Order][BusinessType][isValid]");
				return NULL;
			}			
			sField = "BusinessValue";
			((COrderField*)pOutput)->BusinessValue=formMap["BusinessValue"].c_str();
			if (!((COrderField*)pOutput)->BusinessValue.isValid())
			{
				strcpy((char*)pOutput, "[Order][BusinessValue][isValid]");
				return NULL;
			}			
			sField = "CloseOrderID";
			((COrderField*)pOutput)->CloseOrderID=formMap["CloseOrderID"].c_str();
			if (!((COrderField*)pOutput)->CloseOrderID.isValid())
			{
				strcpy((char*)pOutput, "[Order][CloseOrderID][isValid]");
				return NULL;
			}			
			sField = "IsCrossMargin";
			((COrderField*)pOutput)->IsCrossMargin=formMap["IsCrossMargin"].c_str();
			if (!((COrderField*)pOutput)->IsCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[Order][IsCrossMargin][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((COrderField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((COrderField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[Order][Remark][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((COrderField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((COrderField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[Order][OrderID][isValid]");
				return NULL;
			}			
			sField = "CopyMemberID";
			((COrderField*)pOutput)->CopyMemberID=formMap["CopyMemberID"].c_str();
			if (!((COrderField*)pOutput)->CopyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[Order][CopyMemberID][isValid]");
				return NULL;
			}			
			sField = "CopyOrderID";
			((COrderField*)pOutput)->CopyOrderID=formMap["CopyOrderID"].c_str();
			if (!((COrderField*)pOutput)->CopyOrderID.isValid())
			{
				strcpy((char*)pOutput, "[Order][CopyOrderID][isValid]");
				return NULL;
			}			
			sField = "Leverage";
			((COrderField*)pOutput)->Leverage=formMap["Leverage"].c_str();
			if (!((COrderField*)pOutput)->Leverage.isValid())
			{
				strcpy((char*)pOutput, "[Order][Leverage][isValid]");
				return NULL;
			}			
			sField = "CopyProfitRate";
			((COrderField*)pOutput)->CopyProfitRate=formMap["CopyProfitRate"].c_str();
			if (!((COrderField*)pOutput)->CopyProfitRate.isValid())
			{
				strcpy((char*)pOutput, "[Order][CopyProfitRate][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((COrderField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((COrderField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[Order][APPID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((COrderField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((COrderField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[Order][PositionID][isValid]");
				return NULL;
			}			
			sField = "TriggerPrice";
			((COrderField*)pOutput)->TriggerPrice=formMap["TriggerPrice"].c_str();
			if (!((COrderField*)pOutput)->TriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[Order][TriggerPrice][isValid]");
				return NULL;
			}			
			sField = "Reserve";
			((COrderField*)pOutput)->Reserve=formMap["Reserve"].c_str();
			if (!((COrderField*)pOutput)->Reserve.isValid())
			{
				strcpy((char*)pOutput, "[Order][Reserve][isValid]");
				return NULL;
			}			
			sField = "OrderStatus";
			((COrderField*)pOutput)->OrderStatus=formMap["OrderStatus"].c_str();
			if (!((COrderField*)pOutput)->OrderStatus.isValid())
			{
				strcpy((char*)pOutput, "[Order][OrderStatus][isValid]");
				return NULL;
			}			
			sField = "DeriveSource";
			((COrderField*)pOutput)->DeriveSource=formMap["DeriveSource"].c_str();
			if (!((COrderField*)pOutput)->DeriveSource.isValid())
			{
				strcpy((char*)pOutput, "[Order][DeriveSource][isValid]");
				return NULL;
			}			
			sField = "DeriveDetail";
			((COrderField*)pOutput)->DeriveDetail=formMap["DeriveDetail"].c_str();
			if (!((COrderField*)pOutput)->DeriveDetail.isValid())
			{
				strcpy((char*)pOutput, "[Order][DeriveDetail][isValid]");
				return NULL;
			}			
			sField = "VolumeTraded";
			((COrderField*)pOutput)->VolumeTraded=formMap["VolumeTraded"].c_str();
			if (!((COrderField*)pOutput)->VolumeTraded.isValid())
			{
				strcpy((char*)pOutput, "[Order][VolumeTraded][isValid]");
				return NULL;
			}			
			sField = "VolumeRemain";
			((COrderField*)pOutput)->VolumeRemain=formMap["VolumeRemain"].c_str();
			if (!((COrderField*)pOutput)->VolumeRemain.isValid())
			{
				strcpy((char*)pOutput, "[Order][VolumeRemain][isValid]");
				return NULL;
			}			
			sField = "VolumeCancled";
			((COrderField*)pOutput)->VolumeCancled=formMap["VolumeCancled"].c_str();
			if (!((COrderField*)pOutput)->VolumeCancled.isValid())
			{
				strcpy((char*)pOutput, "[Order][VolumeCancled][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((COrderField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((COrderField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[Order][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((COrderField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((COrderField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[Order][UpdateTime][isValid]");
				return NULL;
			}			
			sField = "Priority";
			((COrderField*)pOutput)->Priority=formMap["Priority"].c_str();
			if (!((COrderField*)pOutput)->Priority.isValid())
			{
				strcpy((char*)pOutput, "[Order][Priority][isValid]");
				return NULL;
			}			
			sField = "TimeSortNo";
			((COrderField*)pOutput)->TimeSortNo=formMap["TimeSortNo"].c_str();
			if (!((COrderField*)pOutput)->TimeSortNo.isValid())
			{
				strcpy((char*)pOutput, "[Order][TimeSortNo][isValid]");
				return NULL;
			}			
			sField = "FrontNo";
			((COrderField*)pOutput)->FrontNo=formMap["FrontNo"].c_str();
			if (!((COrderField*)pOutput)->FrontNo.isValid())
			{
				strcpy((char*)pOutput, "[Order][FrontNo][isValid]");
				return NULL;
			}			
			sField = "PriceCurrency";
			((COrderField*)pOutput)->PriceCurrency=formMap["PriceCurrency"].c_str();
			if (!((COrderField*)pOutput)->PriceCurrency.isValid())
			{
				strcpy((char*)pOutput, "[Order][PriceCurrency][isValid]");
				return NULL;
			}			
			sField = "FeeCurrency";
			((COrderField*)pOutput)->FeeCurrency=formMap["FeeCurrency"].c_str();
			if (!((COrderField*)pOutput)->FeeCurrency.isValid())
			{
				strcpy((char*)pOutput, "[Order][FeeCurrency][isValid]");
				return NULL;
			}			
			sField = "ClearCurrency";
			((COrderField*)pOutput)->ClearCurrency=formMap["ClearCurrency"].c_str();
			if (!((COrderField*)pOutput)->ClearCurrency.isValid())
			{
				strcpy((char*)pOutput, "[Order][ClearCurrency][isValid]");
				return NULL;
			}			
			sField = "FrozenMoney";
			((COrderField*)pOutput)->FrozenMoney=formMap["FrozenMoney"].c_str();
			if (!((COrderField*)pOutput)->FrozenMoney.isValid())
			{
				strcpy((char*)pOutput, "[Order][FrozenMoney][isValid]");
				return NULL;
			}			
			sField = "FrozenFee";
			((COrderField*)pOutput)->FrozenFee=formMap["FrozenFee"].c_str();
			if (!((COrderField*)pOutput)->FrozenFee.isValid())
			{
				strcpy((char*)pOutput, "[Order][FrozenFee][isValid]");
				return NULL;
			}			
			sField = "FrozenMargin";
			((COrderField*)pOutput)->FrozenMargin=formMap["FrozenMargin"].c_str();
			if (!((COrderField*)pOutput)->FrozenMargin.isValid())
			{
				strcpy((char*)pOutput, "[Order][FrozenMargin][isValid]");
				return NULL;
			}			
			sField = "Fee";
			((COrderField*)pOutput)->Fee=formMap["Fee"].c_str();
			if (!((COrderField*)pOutput)->Fee.isValid())
			{
				strcpy((char*)pOutput, "[Order][Fee][isValid]");
				return NULL;
			}			
			sField = "CloseProfit";
			((COrderField*)pOutput)->CloseProfit=formMap["CloseProfit"].c_str();
			if (!((COrderField*)pOutput)->CloseProfit.isValid())
			{
				strcpy((char*)pOutput, "[Order][CloseProfit][isValid]");
				return NULL;
			}			
			sField = "Turnover";
			((COrderField*)pOutput)->Turnover=formMap["Turnover"].c_str();
			if (!((COrderField*)pOutput)->Turnover.isValid())
			{
				strcpy((char*)pOutput, "[Order][Turnover][isValid]");
				return NULL;
			}			
			sField = "RelatedOrderID";
			((COrderField*)pOutput)->RelatedOrderID=formMap["RelatedOrderID"].c_str();
			if (!((COrderField*)pOutput)->RelatedOrderID.isValid())
			{
				strcpy((char*)pOutput, "[Order][RelatedOrderID][isValid]");
				return NULL;
			}			
			sField = "BusinessResult";
			((COrderField*)pOutput)->BusinessResult=formMap["BusinessResult"].c_str();
			if (!((COrderField*)pOutput)->BusinessResult.isValid())
			{
				strcpy((char*)pOutput, "[Order][BusinessResult][isValid]");
				return NULL;
			}			
			sField = "BusinessNo";
			((COrderField*)pOutput)->BusinessNo=formMap["BusinessNo"].c_str();
			if (!((COrderField*)pOutput)->BusinessNo.isValid())
			{
				strcpy((char*)pOutput, "[Order][BusinessNo][isValid]");
				return NULL;
			}			
			sField = "Tradable";
			((COrderField*)pOutput)->Tradable=formMap["Tradable"].c_str();
			if (!((COrderField*)pOutput)->Tradable.isValid())
			{
				strcpy((char*)pOutput, "[Order][Tradable][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((COrderField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((COrderField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[Order][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "PosiDirection";
			((COrderField*)pOutput)->PosiDirection=formMap["PosiDirection"].c_str();
			if (!((COrderField*)pOutput)->PosiDirection.isValid())
			{
				strcpy((char*)pOutput, "[Order][PosiDirection][isValid]");
				return NULL;
			}			
			sField = "TradePrice";
			((COrderField*)pOutput)->TradePrice=formMap["TradePrice"].c_str();
			if (!((COrderField*)pOutput)->TradePrice.isValid())
			{
				strcpy((char*)pOutput, "[Order][TradePrice][isValid]");
				return NULL;
			}			
			sField = "OpenPrice";
			((COrderField*)pOutput)->OpenPrice=formMap["OpenPrice"].c_str();
			if (!((COrderField*)pOutput)->OpenPrice.isValid())
			{
				strcpy((char*)pOutput, "[Order][OpenPrice][isValid]");
				return NULL;
			}			
			sField = "TriggerOrderID";
			((COrderField*)pOutput)->TriggerOrderID=formMap["TriggerOrderID"].c_str();
			if (!((COrderField*)pOutput)->TriggerOrderID.isValid())
			{
				strcpy((char*)pOutput, "[Order][TriggerOrderID][isValid]");
				return NULL;
			}			
			sField = "SLTriggerPrice";
			((COrderField*)pOutput)->SLTriggerPrice=formMap["SLTriggerPrice"].c_str();
			if (!((COrderField*)pOutput)->SLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[Order][SLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TPTriggerPrice";
			((COrderField*)pOutput)->TPTriggerPrice=formMap["TPTriggerPrice"].c_str();
			if (!((COrderField*)pOutput)->TPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[Order][TPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CopyProfit";
			((COrderField*)pOutput)->CopyProfit=formMap["CopyProfit"].c_str();
			if (!((COrderField*)pOutput)->CopyProfit.isValid())
			{
				strcpy((char*)pOutput, "[Order][CopyProfit][isValid]");
				return NULL;
			}			
			sField = "Position";
			((COrderField*)pOutput)->Position=formMap["Position"].c_str();
			if (!((COrderField*)pOutput)->Position.isValid())
			{
				strcpy((char*)pOutput, "[Order][Position][isValid]");
				return NULL;
			}			
			sField = "UserID";
			((COrderField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((COrderField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[Order][UserID][isValid]");
				return NULL;
			}			
			sField = "LastPriceByInsert";
			((COrderField*)pOutput)->LastPriceByInsert=formMap["LastPriceByInsert"].c_str();
			if (!((COrderField*)pOutput)->LastPriceByInsert.isValid())
			{
				strcpy((char*)pOutput, "[Order][LastPriceByInsert][isValid]");
				return NULL;
			}			
			sField = "BidPrice1ByInsert";
			((COrderField*)pOutput)->BidPrice1ByInsert=formMap["BidPrice1ByInsert"].c_str();
			if (!((COrderField*)pOutput)->BidPrice1ByInsert.isValid())
			{
				strcpy((char*)pOutput, "[Order][BidPrice1ByInsert][isValid]");
				return NULL;
			}			
			sField = "AskPrice1ByInsert";
			((COrderField*)pOutput)->AskPrice1ByInsert=formMap["AskPrice1ByInsert"].c_str();
			if (!((COrderField*)pOutput)->AskPrice1ByInsert.isValid())
			{
				strcpy((char*)pOutput, "[Order][AskPrice1ByInsert][isValid]");
				return NULL;
			}			
			sField = "Available";
			((COrderField*)pOutput)->Available=formMap["Available"].c_str();
			if (!((COrderField*)pOutput)->Available.isValid())
			{
				strcpy((char*)pOutput, "[Order][Available][isValid]");
				return NULL;
			}			
			sField = "CreateTime";
			((COrderField*)pOutput)->CreateTime=formMap["CreateTime"].c_str();
			if (!((COrderField*)pOutput)->CreateTime.isValid())
			{
				strcpy((char*)pOutput, "[Order][CreateTime][isValid]");
				return NULL;
			}			
			return &COrderField::m_Describe;
		}
	case FID_TriggerOrder: //条件报单
		{
			string sField = "";
			sField = "MemberID";
			((CTriggerOrderField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CTriggerOrderField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CTriggerOrderField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][AccountID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CTriggerOrderField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CTriggerOrderField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CTriggerOrderField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "OrderPriceType";
			((CTriggerOrderField*)pOutput)->OrderPriceType=formMap["OrderPriceType"].c_str();
			if (!((CTriggerOrderField*)pOutput)->OrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][OrderPriceType][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((CTriggerOrderField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((CTriggerOrderField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][Direction][isValid]");
				return NULL;
			}			
			sField = "OffsetFlag";
			((CTriggerOrderField*)pOutput)->OffsetFlag=formMap["OffsetFlag"].c_str();
			if (!((CTriggerOrderField*)pOutput)->OffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][OffsetFlag][isValid]");
				return NULL;
			}			
			sField = "Price";
			((CTriggerOrderField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((CTriggerOrderField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CTriggerOrderField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CTriggerOrderField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][Volume][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((CTriggerOrderField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((CTriggerOrderField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "VolumeMode";
			((CTriggerOrderField*)pOutput)->VolumeMode=formMap["VolumeMode"].c_str();
			if (!((CTriggerOrderField*)pOutput)->VolumeMode.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][VolumeMode][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((CTriggerOrderField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((CTriggerOrderField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][Cost][isValid]");
				return NULL;
			}			
			sField = "OrderType";
			((CTriggerOrderField*)pOutput)->OrderType=formMap["OrderType"].c_str();
			if (!((CTriggerOrderField*)pOutput)->OrderType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][OrderType][isValid]");
				return NULL;
			}			
			sField = "GTDTime";
			((CTriggerOrderField*)pOutput)->GTDTime=formMap["GTDTime"].c_str();
			if (!((CTriggerOrderField*)pOutput)->GTDTime.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][GTDTime][isValid]");
				return NULL;
			}			
			sField = "MinVolume";
			((CTriggerOrderField*)pOutput)->MinVolume=formMap["MinVolume"].c_str();
			if (!((CTriggerOrderField*)pOutput)->MinVolume.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][MinVolume][isValid]");
				return NULL;
			}			
			sField = "BusinessType";
			((CTriggerOrderField*)pOutput)->BusinessType=formMap["BusinessType"].c_str();
			if (!((CTriggerOrderField*)pOutput)->BusinessType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][BusinessType][isValid]");
				return NULL;
			}			
			sField = "BusinessValue";
			((CTriggerOrderField*)pOutput)->BusinessValue=formMap["BusinessValue"].c_str();
			if (!((CTriggerOrderField*)pOutput)->BusinessValue.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][BusinessValue][isValid]");
				return NULL;
			}			
			sField = "CloseOrderID";
			((CTriggerOrderField*)pOutput)->CloseOrderID=formMap["CloseOrderID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CloseOrderID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CloseOrderID][isValid]");
				return NULL;
			}			
			sField = "IsCrossMargin";
			((CTriggerOrderField*)pOutput)->IsCrossMargin=formMap["IsCrossMargin"].c_str();
			if (!((CTriggerOrderField*)pOutput)->IsCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][IsCrossMargin][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CTriggerOrderField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CTriggerOrderField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][Remark][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((CTriggerOrderField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][OrderID][isValid]");
				return NULL;
			}			
			sField = "CopyMemberID";
			((CTriggerOrderField*)pOutput)->CopyMemberID=formMap["CopyMemberID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CopyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CopyMemberID][isValid]");
				return NULL;
			}			
			sField = "CopyOrderID";
			((CTriggerOrderField*)pOutput)->CopyOrderID=formMap["CopyOrderID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CopyOrderID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CopyOrderID][isValid]");
				return NULL;
			}			
			sField = "Leverage";
			((CTriggerOrderField*)pOutput)->Leverage=formMap["Leverage"].c_str();
			if (!((CTriggerOrderField*)pOutput)->Leverage.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][Leverage][isValid]");
				return NULL;
			}			
			sField = "CopyProfitRate";
			((CTriggerOrderField*)pOutput)->CopyProfitRate=formMap["CopyProfitRate"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CopyProfitRate.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CopyProfitRate][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CTriggerOrderField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][APPID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((CTriggerOrderField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][PositionID][isValid]");
				return NULL;
			}			
			sField = "TriggerPrice";
			((CTriggerOrderField*)pOutput)->TriggerPrice=formMap["TriggerPrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TriggerPrice][isValid]");
				return NULL;
			}			
			sField = "Reserve";
			((CTriggerOrderField*)pOutput)->Reserve=formMap["Reserve"].c_str();
			if (!((CTriggerOrderField*)pOutput)->Reserve.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][Reserve][isValid]");
				return NULL;
			}			
			sField = "SLPrice";
			((CTriggerOrderField*)pOutput)->SLPrice=formMap["SLPrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->SLPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][SLPrice][isValid]");
				return NULL;
			}			
			sField = "SLTriggerPrice";
			((CTriggerOrderField*)pOutput)->SLTriggerPrice=formMap["SLTriggerPrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->SLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][SLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TPPrice";
			((CTriggerOrderField*)pOutput)->TPPrice=formMap["TPPrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TPPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TPPrice][isValid]");
				return NULL;
			}			
			sField = "TPTriggerPrice";
			((CTriggerOrderField*)pOutput)->TPTriggerPrice=formMap["TPTriggerPrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "RiskBefore";
			((CTriggerOrderField*)pOutput)->RiskBefore=formMap["RiskBefore"].c_str();
			if (!((CTriggerOrderField*)pOutput)->RiskBefore.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][RiskBefore][isValid]");
				return NULL;
			}			
			sField = "TriggerOrderType";
			((CTriggerOrderField*)pOutput)->TriggerOrderType=formMap["TriggerOrderType"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TriggerOrderType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TriggerOrderType][isValid]");
				return NULL;
			}			
			sField = "TriggerDetail";
			((CTriggerOrderField*)pOutput)->TriggerDetail=formMap["TriggerDetail"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TriggerDetail.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TriggerDetail][isValid]");
				return NULL;
			}			
			sField = "TriggerPriceType";
			((CTriggerOrderField*)pOutput)->TriggerPriceType=formMap["TriggerPriceType"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TriggerPriceType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TriggerPriceType][isValid]");
				return NULL;
			}			
			sField = "TriggerValue";
			((CTriggerOrderField*)pOutput)->TriggerValue=formMap["TriggerValue"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TriggerValue.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TriggerValue][isValid]");
				return NULL;
			}			
			sField = "CloseSLPrice";
			((CTriggerOrderField*)pOutput)->CloseSLPrice=formMap["CloseSLPrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CloseSLPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CloseSLPrice][isValid]");
				return NULL;
			}			
			sField = "CloseSLTriggerPrice";
			((CTriggerOrderField*)pOutput)->CloseSLTriggerPrice=formMap["CloseSLTriggerPrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CloseSLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CloseSLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CloseTPPrice";
			((CTriggerOrderField*)pOutput)->CloseTPPrice=formMap["CloseTPPrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CloseTPPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CloseTPPrice][isValid]");
				return NULL;
			}			
			sField = "CloseTPTriggerPrice";
			((CTriggerOrderField*)pOutput)->CloseTPTriggerPrice=formMap["CloseTPTriggerPrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CloseTPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CloseTPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CloseOrderPriceType";
			((CTriggerOrderField*)pOutput)->CloseOrderPriceType=formMap["CloseOrderPriceType"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CloseOrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CloseOrderPriceType][isValid]");
				return NULL;
			}			
			sField = "ClosePrice";
			((CTriggerOrderField*)pOutput)->ClosePrice=formMap["ClosePrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->ClosePrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][ClosePrice][isValid]");
				return NULL;
			}			
			sField = "CloseTriggerPrice";
			((CTriggerOrderField*)pOutput)->CloseTriggerPrice=formMap["CloseTriggerPrice"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CloseTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CloseTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "RelatedOrderID";
			((CTriggerOrderField*)pOutput)->RelatedOrderID=formMap["RelatedOrderID"].c_str();
			if (!((CTriggerOrderField*)pOutput)->RelatedOrderID.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][RelatedOrderID][isValid]");
				return NULL;
			}			
			sField = "ActiveTime";
			((CTriggerOrderField*)pOutput)->ActiveTime=formMap["ActiveTime"].c_str();
			if (!((CTriggerOrderField*)pOutput)->ActiveTime.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][ActiveTime][isValid]");
				return NULL;
			}			
			sField = "TriggerTime";
			((CTriggerOrderField*)pOutput)->TriggerTime=formMap["TriggerTime"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TriggerTime.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TriggerTime][isValid]");
				return NULL;
			}			
			sField = "TimeSortNo";
			((CTriggerOrderField*)pOutput)->TimeSortNo=formMap["TimeSortNo"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TimeSortNo.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TimeSortNo][isValid]");
				return NULL;
			}			
			sField = "TriggerStatus";
			((CTriggerOrderField*)pOutput)->TriggerStatus=formMap["TriggerStatus"].c_str();
			if (!((CTriggerOrderField*)pOutput)->TriggerStatus.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][TriggerStatus][isValid]");
				return NULL;
			}			
			sField = "PosiDirection";
			((CTriggerOrderField*)pOutput)->PosiDirection=formMap["PosiDirection"].c_str();
			if (!((CTriggerOrderField*)pOutput)->PosiDirection.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][PosiDirection][isValid]");
				return NULL;
			}			
			sField = "FrontNo";
			((CTriggerOrderField*)pOutput)->FrontNo=formMap["FrontNo"].c_str();
			if (!((CTriggerOrderField*)pOutput)->FrontNo.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][FrontNo][isValid]");
				return NULL;
			}			
			sField = "ErrorNo";
			((CTriggerOrderField*)pOutput)->ErrorNo=formMap["ErrorNo"].c_str();
			if (!((CTriggerOrderField*)pOutput)->ErrorNo.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][ErrorNo][isValid]");
				return NULL;
			}			
			sField = "ErrorMsg";
			((CTriggerOrderField*)pOutput)->ErrorMsg=formMap["ErrorMsg"].c_str();
			if (!((CTriggerOrderField*)pOutput)->ErrorMsg.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][ErrorMsg][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CTriggerOrderField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CTriggerOrderField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CTriggerOrderField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CTriggerOrderField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][UpdateTime][isValid]");
				return NULL;
			}			
			sField = "BusinessNo";
			((CTriggerOrderField*)pOutput)->BusinessNo=formMap["BusinessNo"].c_str();
			if (!((CTriggerOrderField*)pOutput)->BusinessNo.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][BusinessNo][isValid]");
				return NULL;
			}			
			sField = "CreateTime";
			((CTriggerOrderField*)pOutput)->CreateTime=formMap["CreateTime"].c_str();
			if (!((CTriggerOrderField*)pOutput)->CreateTime.isValid())
			{
				strcpy((char*)pOutput, "[TriggerOrder][CreateTime][isValid]");
				return NULL;
			}			
			return &CTriggerOrderField::m_Describe;
		}
	case FID_PositionOrder: //持仓订单组成
		{
			string sField = "";
			sField = "MemberID";
			((CPositionOrderField*)pOutput)->MemberID=formMap["MemberID"].c_str();
			if (!((CPositionOrderField*)pOutput)->MemberID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][MemberID][isValid]");
				return NULL;
			}			
			sField = "TradeUnitID";
			((CPositionOrderField*)pOutput)->TradeUnitID=formMap["TradeUnitID"].c_str();
			if (!((CPositionOrderField*)pOutput)->TradeUnitID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][TradeUnitID][isValid]");
				return NULL;
			}			
			sField = "AccountID";
			((CPositionOrderField*)pOutput)->AccountID=formMap["AccountID"].c_str();
			if (!((CPositionOrderField*)pOutput)->AccountID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][AccountID][isValid]");
				return NULL;
			}			
			sField = "LocalID";
			((CPositionOrderField*)pOutput)->LocalID=formMap["LocalID"].c_str();
			if (!((CPositionOrderField*)pOutput)->LocalID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][LocalID][isValid]");
				return NULL;
			}			
			sField = "ExchangeID";
			((CPositionOrderField*)pOutput)->ExchangeID=formMap["ExchangeID"].c_str();
			if (!((CPositionOrderField*)pOutput)->ExchangeID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][ExchangeID][isValid]");
				return NULL;
			}			
			sField = "InstrumentID";
			((CPositionOrderField*)pOutput)->InstrumentID=formMap["InstrumentID"].c_str();
			if (!((CPositionOrderField*)pOutput)->InstrumentID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][InstrumentID][isValid]");
				return NULL;
			}			
			sField = "OrderPriceType";
			((CPositionOrderField*)pOutput)->OrderPriceType=formMap["OrderPriceType"].c_str();
			if (!((CPositionOrderField*)pOutput)->OrderPriceType.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][OrderPriceType][isValid]");
				return NULL;
			}			
			sField = "Direction";
			((CPositionOrderField*)pOutput)->Direction=formMap["Direction"].c_str();
			if (!((CPositionOrderField*)pOutput)->Direction.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Direction][isValid]");
				return NULL;
			}			
			sField = "OffsetFlag";
			((CPositionOrderField*)pOutput)->OffsetFlag=formMap["OffsetFlag"].c_str();
			if (!((CPositionOrderField*)pOutput)->OffsetFlag.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][OffsetFlag][isValid]");
				return NULL;
			}			
			sField = "Price";
			((CPositionOrderField*)pOutput)->Price=formMap["Price"].c_str();
			if (!((CPositionOrderField*)pOutput)->Price.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Price][isValid]");
				return NULL;
			}			
			sField = "Volume";
			((CPositionOrderField*)pOutput)->Volume=formMap["Volume"].c_str();
			if (!((CPositionOrderField*)pOutput)->Volume.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Volume][isValid]");
				return NULL;
			}			
			sField = "VolumeDisplay";
			((CPositionOrderField*)pOutput)->VolumeDisplay=formMap["VolumeDisplay"].c_str();
			if (!((CPositionOrderField*)pOutput)->VolumeDisplay.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][VolumeDisplay][isValid]");
				return NULL;
			}			
			sField = "VolumeMode";
			((CPositionOrderField*)pOutput)->VolumeMode=formMap["VolumeMode"].c_str();
			if (!((CPositionOrderField*)pOutput)->VolumeMode.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][VolumeMode][isValid]");
				return NULL;
			}			
			sField = "Cost";
			((CPositionOrderField*)pOutput)->Cost=formMap["Cost"].c_str();
			if (!((CPositionOrderField*)pOutput)->Cost.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Cost][isValid]");
				return NULL;
			}			
			sField = "OrderType";
			((CPositionOrderField*)pOutput)->OrderType=formMap["OrderType"].c_str();
			if (!((CPositionOrderField*)pOutput)->OrderType.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][OrderType][isValid]");
				return NULL;
			}			
			sField = "GTDTime";
			((CPositionOrderField*)pOutput)->GTDTime=formMap["GTDTime"].c_str();
			if (!((CPositionOrderField*)pOutput)->GTDTime.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][GTDTime][isValid]");
				return NULL;
			}			
			sField = "MinVolume";
			((CPositionOrderField*)pOutput)->MinVolume=formMap["MinVolume"].c_str();
			if (!((CPositionOrderField*)pOutput)->MinVolume.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][MinVolume][isValid]");
				return NULL;
			}			
			sField = "BusinessType";
			((CPositionOrderField*)pOutput)->BusinessType=formMap["BusinessType"].c_str();
			if (!((CPositionOrderField*)pOutput)->BusinessType.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][BusinessType][isValid]");
				return NULL;
			}			
			sField = "BusinessValue";
			((CPositionOrderField*)pOutput)->BusinessValue=formMap["BusinessValue"].c_str();
			if (!((CPositionOrderField*)pOutput)->BusinessValue.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][BusinessValue][isValid]");
				return NULL;
			}			
			sField = "CloseOrderID";
			((CPositionOrderField*)pOutput)->CloseOrderID=formMap["CloseOrderID"].c_str();
			if (!((CPositionOrderField*)pOutput)->CloseOrderID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][CloseOrderID][isValid]");
				return NULL;
			}			
			sField = "IsCrossMargin";
			((CPositionOrderField*)pOutput)->IsCrossMargin=formMap["IsCrossMargin"].c_str();
			if (!((CPositionOrderField*)pOutput)->IsCrossMargin.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][IsCrossMargin][isValid]");
				return NULL;
			}			
			sField = "Remark";
			((CPositionOrderField*)pOutput)->Remark=formMap["Remark"].c_str();
			if (!((CPositionOrderField*)pOutput)->Remark.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Remark][isValid]");
				return NULL;
			}			
			sField = "OrderID";
			((CPositionOrderField*)pOutput)->OrderID=formMap["OrderID"].c_str();
			if (!((CPositionOrderField*)pOutput)->OrderID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][OrderID][isValid]");
				return NULL;
			}			
			sField = "CopyMemberID";
			((CPositionOrderField*)pOutput)->CopyMemberID=formMap["CopyMemberID"].c_str();
			if (!((CPositionOrderField*)pOutput)->CopyMemberID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][CopyMemberID][isValid]");
				return NULL;
			}			
			sField = "CopyOrderID";
			((CPositionOrderField*)pOutput)->CopyOrderID=formMap["CopyOrderID"].c_str();
			if (!((CPositionOrderField*)pOutput)->CopyOrderID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][CopyOrderID][isValid]");
				return NULL;
			}			
			sField = "Leverage";
			((CPositionOrderField*)pOutput)->Leverage=formMap["Leverage"].c_str();
			if (!((CPositionOrderField*)pOutput)->Leverage.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Leverage][isValid]");
				return NULL;
			}			
			sField = "CopyProfitRate";
			((CPositionOrderField*)pOutput)->CopyProfitRate=formMap["CopyProfitRate"].c_str();
			if (!((CPositionOrderField*)pOutput)->CopyProfitRate.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][CopyProfitRate][isValid]");
				return NULL;
			}			
			sField = "APPID";
			((CPositionOrderField*)pOutput)->APPID=formMap["APPID"].c_str();
			if (!((CPositionOrderField*)pOutput)->APPID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][APPID][isValid]");
				return NULL;
			}			
			sField = "PositionID";
			((CPositionOrderField*)pOutput)->PositionID=formMap["PositionID"].c_str();
			if (!((CPositionOrderField*)pOutput)->PositionID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][PositionID][isValid]");
				return NULL;
			}			
			sField = "TriggerPrice";
			((CPositionOrderField*)pOutput)->TriggerPrice=formMap["TriggerPrice"].c_str();
			if (!((CPositionOrderField*)pOutput)->TriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][TriggerPrice][isValid]");
				return NULL;
			}			
			sField = "Reserve";
			((CPositionOrderField*)pOutput)->Reserve=formMap["Reserve"].c_str();
			if (!((CPositionOrderField*)pOutput)->Reserve.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Reserve][isValid]");
				return NULL;
			}			
			sField = "OrderStatus";
			((CPositionOrderField*)pOutput)->OrderStatus=formMap["OrderStatus"].c_str();
			if (!((CPositionOrderField*)pOutput)->OrderStatus.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][OrderStatus][isValid]");
				return NULL;
			}			
			sField = "DeriveSource";
			((CPositionOrderField*)pOutput)->DeriveSource=formMap["DeriveSource"].c_str();
			if (!((CPositionOrderField*)pOutput)->DeriveSource.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][DeriveSource][isValid]");
				return NULL;
			}			
			sField = "DeriveDetail";
			((CPositionOrderField*)pOutput)->DeriveDetail=formMap["DeriveDetail"].c_str();
			if (!((CPositionOrderField*)pOutput)->DeriveDetail.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][DeriveDetail][isValid]");
				return NULL;
			}			
			sField = "VolumeTraded";
			((CPositionOrderField*)pOutput)->VolumeTraded=formMap["VolumeTraded"].c_str();
			if (!((CPositionOrderField*)pOutput)->VolumeTraded.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][VolumeTraded][isValid]");
				return NULL;
			}			
			sField = "VolumeRemain";
			((CPositionOrderField*)pOutput)->VolumeRemain=formMap["VolumeRemain"].c_str();
			if (!((CPositionOrderField*)pOutput)->VolumeRemain.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][VolumeRemain][isValid]");
				return NULL;
			}			
			sField = "VolumeCancled";
			((CPositionOrderField*)pOutput)->VolumeCancled=formMap["VolumeCancled"].c_str();
			if (!((CPositionOrderField*)pOutput)->VolumeCancled.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][VolumeCancled][isValid]");
				return NULL;
			}			
			sField = "InsertTime";
			((CPositionOrderField*)pOutput)->InsertTime=formMap["InsertTime"].c_str();
			if (!((CPositionOrderField*)pOutput)->InsertTime.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][InsertTime][isValid]");
				return NULL;
			}			
			sField = "UpdateTime";
			((CPositionOrderField*)pOutput)->UpdateTime=formMap["UpdateTime"].c_str();
			if (!((CPositionOrderField*)pOutput)->UpdateTime.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][UpdateTime][isValid]");
				return NULL;
			}			
			sField = "Priority";
			((CPositionOrderField*)pOutput)->Priority=formMap["Priority"].c_str();
			if (!((CPositionOrderField*)pOutput)->Priority.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Priority][isValid]");
				return NULL;
			}			
			sField = "TimeSortNo";
			((CPositionOrderField*)pOutput)->TimeSortNo=formMap["TimeSortNo"].c_str();
			if (!((CPositionOrderField*)pOutput)->TimeSortNo.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][TimeSortNo][isValid]");
				return NULL;
			}			
			sField = "FrontNo";
			((CPositionOrderField*)pOutput)->FrontNo=formMap["FrontNo"].c_str();
			if (!((CPositionOrderField*)pOutput)->FrontNo.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][FrontNo][isValid]");
				return NULL;
			}			
			sField = "PriceCurrency";
			((CPositionOrderField*)pOutput)->PriceCurrency=formMap["PriceCurrency"].c_str();
			if (!((CPositionOrderField*)pOutput)->PriceCurrency.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][PriceCurrency][isValid]");
				return NULL;
			}			
			sField = "FeeCurrency";
			((CPositionOrderField*)pOutput)->FeeCurrency=formMap["FeeCurrency"].c_str();
			if (!((CPositionOrderField*)pOutput)->FeeCurrency.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][FeeCurrency][isValid]");
				return NULL;
			}			
			sField = "ClearCurrency";
			((CPositionOrderField*)pOutput)->ClearCurrency=formMap["ClearCurrency"].c_str();
			if (!((CPositionOrderField*)pOutput)->ClearCurrency.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][ClearCurrency][isValid]");
				return NULL;
			}			
			sField = "FrozenMoney";
			((CPositionOrderField*)pOutput)->FrozenMoney=formMap["FrozenMoney"].c_str();
			if (!((CPositionOrderField*)pOutput)->FrozenMoney.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][FrozenMoney][isValid]");
				return NULL;
			}			
			sField = "FrozenFee";
			((CPositionOrderField*)pOutput)->FrozenFee=formMap["FrozenFee"].c_str();
			if (!((CPositionOrderField*)pOutput)->FrozenFee.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][FrozenFee][isValid]");
				return NULL;
			}			
			sField = "FrozenMargin";
			((CPositionOrderField*)pOutput)->FrozenMargin=formMap["FrozenMargin"].c_str();
			if (!((CPositionOrderField*)pOutput)->FrozenMargin.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][FrozenMargin][isValid]");
				return NULL;
			}			
			sField = "Fee";
			((CPositionOrderField*)pOutput)->Fee=formMap["Fee"].c_str();
			if (!((CPositionOrderField*)pOutput)->Fee.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Fee][isValid]");
				return NULL;
			}			
			sField = "CloseProfit";
			((CPositionOrderField*)pOutput)->CloseProfit=formMap["CloseProfit"].c_str();
			if (!((CPositionOrderField*)pOutput)->CloseProfit.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][CloseProfit][isValid]");
				return NULL;
			}			
			sField = "Turnover";
			((CPositionOrderField*)pOutput)->Turnover=formMap["Turnover"].c_str();
			if (!((CPositionOrderField*)pOutput)->Turnover.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Turnover][isValid]");
				return NULL;
			}			
			sField = "RelatedOrderID";
			((CPositionOrderField*)pOutput)->RelatedOrderID=formMap["RelatedOrderID"].c_str();
			if (!((CPositionOrderField*)pOutput)->RelatedOrderID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][RelatedOrderID][isValid]");
				return NULL;
			}			
			sField = "BusinessResult";
			((CPositionOrderField*)pOutput)->BusinessResult=formMap["BusinessResult"].c_str();
			if (!((CPositionOrderField*)pOutput)->BusinessResult.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][BusinessResult][isValid]");
				return NULL;
			}			
			sField = "BusinessNo";
			((CPositionOrderField*)pOutput)->BusinessNo=formMap["BusinessNo"].c_str();
			if (!((CPositionOrderField*)pOutput)->BusinessNo.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][BusinessNo][isValid]");
				return NULL;
			}			
			sField = "Tradable";
			((CPositionOrderField*)pOutput)->Tradable=formMap["Tradable"].c_str();
			if (!((CPositionOrderField*)pOutput)->Tradable.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Tradable][isValid]");
				return NULL;
			}			
			sField = "SettlementGroup";
			((CPositionOrderField*)pOutput)->SettlementGroup=formMap["SettlementGroup"].c_str();
			if (!((CPositionOrderField*)pOutput)->SettlementGroup.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][SettlementGroup][isValid]");
				return NULL;
			}			
			sField = "PosiDirection";
			((CPositionOrderField*)pOutput)->PosiDirection=formMap["PosiDirection"].c_str();
			if (!((CPositionOrderField*)pOutput)->PosiDirection.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][PosiDirection][isValid]");
				return NULL;
			}			
			sField = "TradePrice";
			((CPositionOrderField*)pOutput)->TradePrice=formMap["TradePrice"].c_str();
			if (!((CPositionOrderField*)pOutput)->TradePrice.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][TradePrice][isValid]");
				return NULL;
			}			
			sField = "OpenPrice";
			((CPositionOrderField*)pOutput)->OpenPrice=formMap["OpenPrice"].c_str();
			if (!((CPositionOrderField*)pOutput)->OpenPrice.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][OpenPrice][isValid]");
				return NULL;
			}			
			sField = "TriggerOrderID";
			((CPositionOrderField*)pOutput)->TriggerOrderID=formMap["TriggerOrderID"].c_str();
			if (!((CPositionOrderField*)pOutput)->TriggerOrderID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][TriggerOrderID][isValid]");
				return NULL;
			}			
			sField = "SLTriggerPrice";
			((CPositionOrderField*)pOutput)->SLTriggerPrice=formMap["SLTriggerPrice"].c_str();
			if (!((CPositionOrderField*)pOutput)->SLTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][SLTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "TPTriggerPrice";
			((CPositionOrderField*)pOutput)->TPTriggerPrice=formMap["TPTriggerPrice"].c_str();
			if (!((CPositionOrderField*)pOutput)->TPTriggerPrice.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][TPTriggerPrice][isValid]");
				return NULL;
			}			
			sField = "CopyProfit";
			((CPositionOrderField*)pOutput)->CopyProfit=formMap["CopyProfit"].c_str();
			if (!((CPositionOrderField*)pOutput)->CopyProfit.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][CopyProfit][isValid]");
				return NULL;
			}			
			sField = "Position";
			((CPositionOrderField*)pOutput)->Position=formMap["Position"].c_str();
			if (!((CPositionOrderField*)pOutput)->Position.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Position][isValid]");
				return NULL;
			}			
			sField = "UserID";
			((CPositionOrderField*)pOutput)->UserID=formMap["UserID"].c_str();
			if (!((CPositionOrderField*)pOutput)->UserID.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][UserID][isValid]");
				return NULL;
			}			
			sField = "LastPriceByInsert";
			((CPositionOrderField*)pOutput)->LastPriceByInsert=formMap["LastPriceByInsert"].c_str();
			if (!((CPositionOrderField*)pOutput)->LastPriceByInsert.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][LastPriceByInsert][isValid]");
				return NULL;
			}			
			sField = "BidPrice1ByInsert";
			((CPositionOrderField*)pOutput)->BidPrice1ByInsert=formMap["BidPrice1ByInsert"].c_str();
			if (!((CPositionOrderField*)pOutput)->BidPrice1ByInsert.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][BidPrice1ByInsert][isValid]");
				return NULL;
			}			
			sField = "AskPrice1ByInsert";
			((CPositionOrderField*)pOutput)->AskPrice1ByInsert=formMap["AskPrice1ByInsert"].c_str();
			if (!((CPositionOrderField*)pOutput)->AskPrice1ByInsert.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][AskPrice1ByInsert][isValid]");
				return NULL;
			}			
			sField = "Available";
			((CPositionOrderField*)pOutput)->Available=formMap["Available"].c_str();
			if (!((CPositionOrderField*)pOutput)->Available.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][Available][isValid]");
				return NULL;
			}			
			sField = "CreateTime";
			((CPositionOrderField*)pOutput)->CreateTime=formMap["CreateTime"].c_str();
			if (!((CPositionOrderField*)pOutput)->CreateTime.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][CreateTime][isValid]");
				return NULL;
			}			
			sField = "VolumeToClose";
			((CPositionOrderField*)pOutput)->VolumeToClose=formMap["VolumeToClose"].c_str();
			if (!((CPositionOrderField*)pOutput)->VolumeToClose.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][VolumeToClose][isValid]");
				return NULL;
			}			
			sField = "VolumeClosed";
			((CPositionOrderField*)pOutput)->VolumeClosed=formMap["VolumeClosed"].c_str();
			if (!((CPositionOrderField*)pOutput)->VolumeClosed.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][VolumeClosed][isValid]");
				return NULL;
			}			
			sField = "VolumeOnClose";
			((CPositionOrderField*)pOutput)->VolumeOnClose=formMap["VolumeOnClose"].c_str();
			if (!((CPositionOrderField*)pOutput)->VolumeOnClose.isValid())
			{
				strcpy((char*)pOutput, "[PositionOrder][VolumeOnClose][isValid]");
				return NULL;
			}			
			return &CPositionOrderField::m_Describe;
		}
	default:
		{
			sprintf((char*)pOutput, "no FieldFID [%d]", (*iter).second->fid);
			return NULL;
		}
	}
	return NULL;
}
