#include "platform.h"
#include "JsonToField.h"
#include "BaseFunction.h"
#include "FieldMap.h"

CFieldDescribe* CJsonAnalysis::FromJsonToStruct(const char* packageName, const void *pInput, int nInputLen, void *pOutput,int *nTNo)
{
	CXTPPackageDefineNameMap::iterator iter1 = g_XTPPackageDefineNameMap.find(packageName);
	if(iter1==g_XTPPackageDefineNameMap.end())
	{
		sprintf((char*)pOutput, "[NoPackageName][%s]", packageName);
		return NULL;
	}
	map<string, json_str> jsonMap = jsonSplit((char *)pInput + 1, nInputLen - 1);
	map<string, json_str>::iterator iter;
	switch((*iter1).second->tid)
	{
	case TID_SendCommand: //一般消息请求
		{
			*nTNo = TID_SendCommand;
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CCommandField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Command][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCommandField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CCommandField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Command][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCommandField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("Command");
			if(iter != jsonMap.end())
			{
				if(!((CCommandField*)pOutput)->Command.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Command][Command][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCommandField*)pOutput)->Command.clear();
			}
			iter = jsonMap.find("Content");
			if(iter != jsonMap.end())
			{
				if(!((CCommandField*)pOutput)->Content.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Command][Content][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCommandField*)pOutput)->Content.clear();
			}
			return &CCommandField::m_Describe;
		
		}
	case TID_SendInitLink: //创建上层连接请求
		{
			*nTNo = TID_SendInitLink;
			iter = jsonMap.find("LinkNo");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->LinkNo.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][LinkNo][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->LinkNo.clear();
			}
			iter = jsonMap.find("LinkUserID");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->LinkUserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][LinkUserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->LinkUserID.clear();
			}
			iter = jsonMap.find("LinkPassword");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->LinkPassword.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][LinkPassword][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->LinkPassword.clear();
			}
			iter = jsonMap.find("EncryptType");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->EncryptType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][EncryptType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->EncryptType.clear();
			}
			iter = jsonMap.find("LinkDllName");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->LinkDllName.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][LinkDllName][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->LinkDllName.clear();
			}
			iter = jsonMap.find("LinkDllLoad");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->LinkDllLoad.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][LinkDllLoad][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->LinkDllLoad.clear();
			}
			iter = jsonMap.find("LinkAddress");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->LinkAddress.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][LinkAddress][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->LinkAddress.clear();
			}
			iter = jsonMap.find("LinkStatus");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->LinkStatus.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][LinkStatus][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->LinkStatus.clear();
			}
			iter = jsonMap.find("IsActive");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->IsActive.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][IsActive][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->IsActive.clear();
			}
			iter = jsonMap.find("Config");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->Config.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][Config][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->Config.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CLinkField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Link][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CLinkField*)pOutput)->Remark.clear();
			}
			return &CLinkField::m_Describe;
		
		}
	case TID_SendUserLogin: //用户登录请求
		{
			*nTNo = TID_SendUserLogin;
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CUserLoginField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogin][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLoginField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("Password");
			if(iter != jsonMap.end())
			{
				if(!((CUserLoginField*)pOutput)->Password.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogin][Password][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLoginField*)pOutput)->Password.clear();
			}
			iter = jsonMap.find("UserProductID");
			if(iter != jsonMap.end())
			{
				if(!((CUserLoginField*)pOutput)->UserProductID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogin][UserProductID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLoginField*)pOutput)->UserProductID.clear();
			}
			iter = jsonMap.find("MacAddress");
			if(iter != jsonMap.end())
			{
				if(!((CUserLoginField*)pOutput)->MacAddress.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogin][MacAddress][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLoginField*)pOutput)->MacAddress.clear();
			}
			iter = jsonMap.find("ClientIPAddress");
			if(iter != jsonMap.end())
			{
				if(!((CUserLoginField*)pOutput)->ClientIPAddress.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogin][ClientIPAddress][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLoginField*)pOutput)->ClientIPAddress.clear();
			}
			iter = jsonMap.find("HDSerialID");
			if(iter != jsonMap.end())
			{
				if(!((CUserLoginField*)pOutput)->HDSerialID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogin][HDSerialID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLoginField*)pOutput)->HDSerialID.clear();
			}
			iter = jsonMap.find("AuthCode");
			if(iter != jsonMap.end())
			{
				if(!((CUserLoginField*)pOutput)->AuthCode.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogin][AuthCode][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLoginField*)pOutput)->AuthCode.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((CUserLoginField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogin][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLoginField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("Token");
			if(iter != jsonMap.end())
			{
				if(!((CUserLoginField*)pOutput)->Token.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogin][Token][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLoginField*)pOutput)->Token.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CUserLoginField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogin][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLoginField*)pOutput)->Remark.clear();
			}
			return &CUserLoginField::m_Describe;
		
		}
	case TID_SendUserLogout: //用户退出请求
		{
			*nTNo = TID_SendUserLogout;
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CUserLogoutField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogout][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLogoutField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((CUserLogoutField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogout][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLogoutField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("Token");
			if(iter != jsonMap.end())
			{
				if(!((CUserLogoutField*)pOutput)->Token.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserLogout][Token][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserLogoutField*)pOutput)->Token.clear();
			}
			return &CUserLogoutField::m_Describe;
		
		}
	case TID_SendOrderInsert: //报单录入请求
		{
			*nTNo = TID_SendOrderInsert;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->TradeUnitID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("OrderPriceType");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->OrderPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][OrderPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->OrderPriceType.clear();
			}
			iter = jsonMap.find("Direction");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Direction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Direction.clear();
			}
			iter = jsonMap.find("OffsetFlag");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->OffsetFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][OffsetFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->OffsetFlag.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("VolumeDisplay");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][VolumeDisplay][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->VolumeDisplay.clear();
			}
			iter = jsonMap.find("VolumeMode");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->VolumeMode.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][VolumeMode][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->VolumeMode.clear();
			}
			iter = jsonMap.find("Cost");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Cost][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Cost.clear();
			}
			iter = jsonMap.find("OrderType");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->OrderType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][OrderType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->OrderType.clear();
			}
			iter = jsonMap.find("GTDTime");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->GTDTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][GTDTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->GTDTime.clear();
			}
			iter = jsonMap.find("MinVolume");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->MinVolume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][MinVolume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->MinVolume.clear();
			}
			iter = jsonMap.find("BusinessType");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->BusinessType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][BusinessType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->BusinessType.clear();
			}
			iter = jsonMap.find("BusinessValue");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->BusinessValue.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][BusinessValue][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->BusinessValue.clear();
			}
			iter = jsonMap.find("CloseOrderID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->CloseOrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][CloseOrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->CloseOrderID.clear();
			}
			iter = jsonMap.find("IsCrossMargin");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->IsCrossMargin.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][IsCrossMargin][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->IsCrossMargin.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Remark.clear();
			}
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("CopyMemberID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->CopyMemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][CopyMemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->CopyMemberID.clear();
			}
			iter = jsonMap.find("CopyOrderID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->CopyOrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][CopyOrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->CopyOrderID.clear();
			}
			iter = jsonMap.find("Leverage");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Leverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Leverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Leverage.clear();
			}
			iter = jsonMap.find("CopyProfitRate");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->CopyProfitRate.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][CopyProfitRate][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->CopyProfitRate.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("PositionID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][PositionID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->PositionID.clear();
			}
			iter = jsonMap.find("TriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->TriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][TriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->TriggerPrice.clear();
			}
			iter = jsonMap.find("Reserve");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Reserve.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Reserve][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Reserve.clear();
			}
			return &COrderInsertField::m_Describe;
		
		}
	case TID_SendOrderAction: //报单操作请求
		{
			*nTNo = TID_SendOrderAction;
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("OrderLocalID");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->OrderLocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][OrderLocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->OrderLocalID.clear();
			}
			iter = jsonMap.find("ActionFlag");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->ActionFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][ActionFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->ActionFlag.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("Direction");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][Direction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->Direction.clear();
			}
			iter = jsonMap.find("VolumeDisplay");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][VolumeDisplay][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->VolumeDisplay.clear();
			}
			iter = jsonMap.find("ProductGroup");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][ProductGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->ProductGroup.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Cost");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][Cost][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->Cost.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((COrderActionField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderAction][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderActionField*)pOutput)->Remark.clear();
			}
			return &COrderActionField::m_Describe;
		
		}
	case TID_SendTriggerOrderInsert: //触发单录入请求
		{
			*nTNo = TID_SendTriggerOrderInsert;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->TradeUnitID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("OrderPriceType");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->OrderPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][OrderPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->OrderPriceType.clear();
			}
			iter = jsonMap.find("Direction");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][Direction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->Direction.clear();
			}
			iter = jsonMap.find("OffsetFlag");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->OffsetFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][OffsetFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->OffsetFlag.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("VolumeDisplay");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][VolumeDisplay][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->VolumeDisplay.clear();
			}
			iter = jsonMap.find("VolumeMode");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->VolumeMode.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][VolumeMode][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->VolumeMode.clear();
			}
			iter = jsonMap.find("Cost");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][Cost][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->Cost.clear();
			}
			iter = jsonMap.find("OrderType");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->OrderType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][OrderType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->OrderType.clear();
			}
			iter = jsonMap.find("GTDTime");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->GTDTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][GTDTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->GTDTime.clear();
			}
			iter = jsonMap.find("MinVolume");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->MinVolume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][MinVolume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->MinVolume.clear();
			}
			iter = jsonMap.find("BusinessType");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->BusinessType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][BusinessType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->BusinessType.clear();
			}
			iter = jsonMap.find("BusinessValue");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->BusinessValue.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][BusinessValue][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->BusinessValue.clear();
			}
			iter = jsonMap.find("CloseOrderID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->CloseOrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][CloseOrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->CloseOrderID.clear();
			}
			iter = jsonMap.find("IsCrossMargin");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->IsCrossMargin.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][IsCrossMargin][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->IsCrossMargin.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->Remark.clear();
			}
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("CopyMemberID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->CopyMemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][CopyMemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->CopyMemberID.clear();
			}
			iter = jsonMap.find("CopyOrderID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->CopyOrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][CopyOrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->CopyOrderID.clear();
			}
			iter = jsonMap.find("Leverage");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->Leverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][Leverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->Leverage.clear();
			}
			iter = jsonMap.find("CopyProfitRate");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->CopyProfitRate.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][CopyProfitRate][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->CopyProfitRate.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("PositionID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][PositionID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->PositionID.clear();
			}
			iter = jsonMap.find("TriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->TriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->TriggerPrice.clear();
			}
			iter = jsonMap.find("Reserve");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->Reserve.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][Reserve][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->Reserve.clear();
			}
			iter = jsonMap.find("SLPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->SLPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][SLPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->SLPrice.clear();
			}
			iter = jsonMap.find("SLTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->SLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][SLTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->SLTriggerPrice.clear();
			}
			iter = jsonMap.find("TPPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->TPPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][TPPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->TPPrice.clear();
			}
			iter = jsonMap.find("TPTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->TPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][TPTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->TPTriggerPrice.clear();
			}
			iter = jsonMap.find("RiskBefore");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->RiskBefore.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][RiskBefore][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->RiskBefore.clear();
			}
			iter = jsonMap.find("TriggerOrderType");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->TriggerOrderType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerOrderType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->TriggerOrderType.clear();
			}
			iter = jsonMap.find("TriggerDetail");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->TriggerDetail.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerDetail][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->TriggerDetail.clear();
			}
			iter = jsonMap.find("TriggerPriceType");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->TriggerPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->TriggerPriceType.clear();
			}
			iter = jsonMap.find("TriggerValue");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->TriggerValue.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerValue][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->TriggerValue.clear();
			}
			iter = jsonMap.find("CloseSLPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->CloseSLPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][CloseSLPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->CloseSLPrice.clear();
			}
			iter = jsonMap.find("CloseSLTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->CloseSLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][CloseSLTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->CloseSLTriggerPrice.clear();
			}
			iter = jsonMap.find("CloseTPPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->CloseTPPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][CloseTPPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->CloseTPPrice.clear();
			}
			iter = jsonMap.find("CloseTPTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->CloseTPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][CloseTPTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->CloseTPTriggerPrice.clear();
			}
			iter = jsonMap.find("CloseOrderPriceType");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->CloseOrderPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][CloseOrderPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->CloseOrderPriceType.clear();
			}
			iter = jsonMap.find("ClosePrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->ClosePrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][ClosePrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->ClosePrice.clear();
			}
			iter = jsonMap.find("CloseTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->CloseTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][CloseTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->CloseTriggerPrice.clear();
			}
			iter = jsonMap.find("RelatedOrderID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->RelatedOrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][RelatedOrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->RelatedOrderID.clear();
			}
			iter = jsonMap.find("ActiveTime");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->ActiveTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][ActiveTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->ActiveTime.clear();
			}
			iter = jsonMap.find("TriggerTime");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderInsertField*)pOutput)->TriggerTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderInsert][TriggerTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderInsertField*)pOutput)->TriggerTime.clear();
			}
			return &CTriggerOrderInsertField::m_Describe;
		
		}
	case TID_SendTriggerOrderAction: //触发单操作请求
		{
			*nTNo = TID_SendTriggerOrderAction;
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("OrderLocalID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->OrderLocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][OrderLocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->OrderLocalID.clear();
			}
			iter = jsonMap.find("ActionFlag");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->ActionFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][ActionFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->ActionFlag.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("Direction");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][Direction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->Direction.clear();
			}
			iter = jsonMap.find("VolumeDisplay");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][VolumeDisplay][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->VolumeDisplay.clear();
			}
			iter = jsonMap.find("ProductGroup");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][ProductGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->ProductGroup.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Cost");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][Cost][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->Cost.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->Remark.clear();
			}
			iter = jsonMap.find("SLPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->SLPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][SLPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->SLPrice.clear();
			}
			iter = jsonMap.find("SLTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->SLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][SLTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->SLTriggerPrice.clear();
			}
			iter = jsonMap.find("TPPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->TPPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][TPPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->TPPrice.clear();
			}
			iter = jsonMap.find("TPTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->TPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][TPTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->TPTriggerPrice.clear();
			}
			iter = jsonMap.find("TriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->TriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][TriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->TriggerPrice.clear();
			}
			iter = jsonMap.find("TriggerOrderType");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->TriggerOrderType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][TriggerOrderType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->TriggerOrderType.clear();
			}
			iter = jsonMap.find("TriggerValue");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->TriggerValue.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][TriggerValue][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->TriggerValue.clear();
			}
			iter = jsonMap.find("TriggerDetail");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->TriggerDetail.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][TriggerDetail][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->TriggerDetail.clear();
			}
			iter = jsonMap.find("TriggerPriceType");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->TriggerPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][TriggerPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->TriggerPriceType.clear();
			}
			iter = jsonMap.find("ErrorNo");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->ErrorNo.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][ErrorNo][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->ErrorNo.clear();
			}
			iter = jsonMap.find("ErrorMsg");
			if(iter != jsonMap.end())
			{
				if(!((CTriggerOrderActionField*)pOutput)->ErrorMsg.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TriggerOrderAction][ErrorMsg][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTriggerOrderActionField*)pOutput)->ErrorMsg.clear();
			}
			return &CTriggerOrderActionField::m_Describe;
		
		}
	case TID_SendCloseOrderInsert: //止盈止损报单录入请求
		{
			*nTNo = TID_SendCloseOrderInsert;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->TradeUnitID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("OrderPriceType");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->OrderPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][OrderPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->OrderPriceType.clear();
			}
			iter = jsonMap.find("Direction");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][Direction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->Direction.clear();
			}
			iter = jsonMap.find("OffsetFlag");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->OffsetFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][OffsetFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->OffsetFlag.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("VolumeDisplay");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][VolumeDisplay][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->VolumeDisplay.clear();
			}
			iter = jsonMap.find("VolumeMode");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->VolumeMode.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][VolumeMode][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->VolumeMode.clear();
			}
			iter = jsonMap.find("Cost");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][Cost][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->Cost.clear();
			}
			iter = jsonMap.find("OrderType");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->OrderType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][OrderType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->OrderType.clear();
			}
			iter = jsonMap.find("GTDTime");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->GTDTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][GTDTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->GTDTime.clear();
			}
			iter = jsonMap.find("MinVolume");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->MinVolume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][MinVolume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->MinVolume.clear();
			}
			iter = jsonMap.find("BusinessType");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->BusinessType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][BusinessType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->BusinessType.clear();
			}
			iter = jsonMap.find("BusinessValue");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->BusinessValue.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][BusinessValue][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->BusinessValue.clear();
			}
			iter = jsonMap.find("CloseOrderID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CloseOrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CloseOrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CloseOrderID.clear();
			}
			iter = jsonMap.find("IsCrossMargin");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->IsCrossMargin.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][IsCrossMargin][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->IsCrossMargin.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->Remark.clear();
			}
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("CopyMemberID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CopyMemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CopyMemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CopyMemberID.clear();
			}
			iter = jsonMap.find("CopyOrderID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CopyOrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CopyOrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CopyOrderID.clear();
			}
			iter = jsonMap.find("Leverage");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->Leverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][Leverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->Leverage.clear();
			}
			iter = jsonMap.find("CopyProfitRate");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CopyProfitRate.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CopyProfitRate][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CopyProfitRate.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("PositionID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][PositionID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->PositionID.clear();
			}
			iter = jsonMap.find("TriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->TriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][TriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->TriggerPrice.clear();
			}
			iter = jsonMap.find("Reserve");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->Reserve.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][Reserve][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->Reserve.clear();
			}
			iter = jsonMap.find("CloseSLPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CloseSLPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CloseSLPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CloseSLPrice.clear();
			}
			iter = jsonMap.find("SLTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->SLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][SLTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->SLTriggerPrice.clear();
			}
			iter = jsonMap.find("CloseSLTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CloseSLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CloseSLTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CloseSLTriggerPrice.clear();
			}
			iter = jsonMap.find("CloseTPPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CloseTPPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CloseTPPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CloseTPPrice.clear();
			}
			iter = jsonMap.find("TPTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->TPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][TPTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->TPTriggerPrice.clear();
			}
			iter = jsonMap.find("CloseTPTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CloseTPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CloseTPTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CloseTPTriggerPrice.clear();
			}
			iter = jsonMap.find("CloseOrderPriceType");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CloseOrderPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CloseOrderPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CloseOrderPriceType.clear();
			}
			iter = jsonMap.find("ClosePrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->ClosePrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][ClosePrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->ClosePrice.clear();
			}
			iter = jsonMap.find("CloseTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CloseTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CloseTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CloseTriggerPrice.clear();
			}
			iter = jsonMap.find("CloseTriggerPriceType");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderInsertField*)pOutput)->CloseTriggerPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderInsert][CloseTriggerPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderInsertField*)pOutput)->CloseTriggerPriceType.clear();
			}
			return &CCloseOrderInsertField::m_Describe;
		
		}
	case TID_SendCloseOrderAction: //止盈止损报单操作请求
		{
			*nTNo = TID_SendCloseOrderAction;
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("OrderLocalID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->OrderLocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][OrderLocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->OrderLocalID.clear();
			}
			iter = jsonMap.find("ActionFlag");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->ActionFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][ActionFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->ActionFlag.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("Direction");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][Direction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->Direction.clear();
			}
			iter = jsonMap.find("VolumeDisplay");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][VolumeDisplay][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->VolumeDisplay.clear();
			}
			iter = jsonMap.find("ProductGroup");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][ProductGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->ProductGroup.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Cost");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][Cost][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->Cost.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->Remark.clear();
			}
			iter = jsonMap.find("SLPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->SLPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][SLPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->SLPrice.clear();
			}
			iter = jsonMap.find("SLTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->SLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][SLTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->SLTriggerPrice.clear();
			}
			iter = jsonMap.find("TPPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->TPPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][TPPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->TPPrice.clear();
			}
			iter = jsonMap.find("TPTriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->TPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][TPTriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->TPTriggerPrice.clear();
			}
			iter = jsonMap.find("TriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->TriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][TriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->TriggerPrice.clear();
			}
			iter = jsonMap.find("TriggerOrderType");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->TriggerOrderType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][TriggerOrderType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->TriggerOrderType.clear();
			}
			iter = jsonMap.find("TriggerValue");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->TriggerValue.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][TriggerValue][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->TriggerValue.clear();
			}
			iter = jsonMap.find("TriggerDetail");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->TriggerDetail.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][TriggerDetail][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->TriggerDetail.clear();
			}
			iter = jsonMap.find("TriggerPriceType");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->TriggerPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][TriggerPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->TriggerPriceType.clear();
			}
			iter = jsonMap.find("ErrorNo");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->ErrorNo.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][ErrorNo][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->ErrorNo.clear();
			}
			iter = jsonMap.find("ErrorMsg");
			if(iter != jsonMap.end())
			{
				if(!((CCloseOrderActionField*)pOutput)->ErrorMsg.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CloseOrderAction][ErrorMsg][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCloseOrderActionField*)pOutput)->ErrorMsg.clear();
			}
			return &CCloseOrderActionField::m_Describe;
		
		}
	case TID_SendQuoteInsert: //报价录入请求
		{
			*nTNo = TID_SendQuoteInsert;
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->TradeUnitID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("BuyOffsetFlag");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->BuyOffsetFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][BuyOffsetFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->BuyOffsetFlag.clear();
			}
			iter = jsonMap.find("BuyPrice");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->BuyPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][BuyPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->BuyPrice.clear();
			}
			iter = jsonMap.find("SellOffsetFlag");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->SellOffsetFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][SellOffsetFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->SellOffsetFlag.clear();
			}
			iter = jsonMap.find("SellPrice");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->SellPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][SellPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->SellPrice.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteInsertField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteInsert][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteInsertField*)pOutput)->Remark.clear();
			}
			return &CQuoteInsertField::m_Describe;
		
		}
	case TID_SendQuoteAction: //报价操作请求
		{
			*nTNo = TID_SendQuoteAction;
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("OrderLocalID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->OrderLocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][OrderLocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->OrderLocalID.clear();
			}
			iter = jsonMap.find("ActionFlag");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->ActionFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][ActionFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->ActionFlag.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("Direction");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][Direction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->Direction.clear();
			}
			iter = jsonMap.find("VolumeDisplay");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][VolumeDisplay][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->VolumeDisplay.clear();
			}
			iter = jsonMap.find("ProductGroup");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][ProductGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->ProductGroup.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Cost");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][Cost][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->Cost.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->Remark.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((CQuoteActionField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QuoteAction][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQuoteActionField*)pOutput)->TradeUnitID.clear();
			}
			return &CQuoteActionField::m_Describe;
		
		}
	case TID_SendTopicAction: //订阅主题请求
		{
			*nTNo = TID_SendTopicAction;
			iter = jsonMap.find("Action");
			if(iter != jsonMap.end())
			{
				if(!((CTopicActionField*)pOutput)->Action.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TopicAction][Action][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTopicActionField*)pOutput)->Action.clear();
			}
			iter = jsonMap.find("TopicID");
			if(iter != jsonMap.end())
			{
				if(!((CTopicActionField*)pOutput)->TopicID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TopicAction][TopicID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTopicActionField*)pOutput)->TopicID.clear();
			}
			iter = jsonMap.find("Index");
			if(iter != jsonMap.end())
			{
				if(!((CTopicActionField*)pOutput)->Index.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TopicAction][Index][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTopicActionField*)pOutput)->Index.clear();
			}
			iter = jsonMap.find("ResumeNo");
			if(iter != jsonMap.end())
			{
				if(!((CTopicActionField*)pOutput)->ResumeNo.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TopicAction][ResumeNo][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTopicActionField*)pOutput)->ResumeNo.clear();
			}
			iter = jsonMap.find("BusinessNo");
			if(iter != jsonMap.end())
			{
				if(!((CTopicActionField*)pOutput)->BusinessNo.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TopicAction][BusinessNo][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTopicActionField*)pOutput)->BusinessNo.clear();
			}
			iter = jsonMap.find("Token");
			if(iter != jsonMap.end())
			{
				if(!((CTopicActionField*)pOutput)->Token.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TopicAction][Token][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTopicActionField*)pOutput)->Token.clear();
			}
			iter = jsonMap.find("Zip");
			if(iter != jsonMap.end())
			{
				if(!((CTopicActionField*)pOutput)->Zip.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TopicAction][Zip][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTopicActionField*)pOutput)->Zip.clear();
			}
			iter = jsonMap.find("Frequency");
			if(iter != jsonMap.end())
			{
				if(!((CTopicActionField*)pOutput)->Frequency.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[TopicAction][Frequency][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTopicActionField*)pOutput)->Frequency.clear();
			}
			return &CTopicActionField::m_Describe;
		
		}
	case TID_SendMarketDataUpdate: //设置行情请求
		{
			*nTNo = TID_SendMarketDataUpdate;
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("MarkedPrice");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->MarkedPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][MarkedPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->MarkedPrice.clear();
			}
			iter = jsonMap.find("TheoryPrice");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->TheoryPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][TheoryPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->TheoryPrice.clear();
			}
			iter = jsonMap.find("PreSettlementPrice");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->PreSettlementPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][PreSettlementPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->PreSettlementPrice.clear();
			}
			iter = jsonMap.find("SettlementPrice");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->SettlementPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][SettlementPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->SettlementPrice.clear();
			}
			iter = jsonMap.find("UnderlyingPrice");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->UnderlyingPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][UnderlyingPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->UnderlyingPrice.clear();
			}
			iter = jsonMap.find("PrePositionFeeRate");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->PrePositionFeeRate.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][PrePositionFeeRate][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->PrePositionFeeRate.clear();
			}
			iter = jsonMap.find("PositionFeeRate");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->PositionFeeRate.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][PositionFeeRate][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->PositionFeeRate.clear();
			}
			iter = jsonMap.find("InstrumentStatus");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->InstrumentStatus.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][InstrumentStatus][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->InstrumentStatus.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("Turnover");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->Turnover.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][Turnover][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->Turnover.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CMarketDataUpdateField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MarketDataUpdate][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMarketDataUpdateField*)pOutput)->Remark.clear();
			}
			return &CMarketDataUpdateField::m_Describe;
		
		}
	case TID_SendPositionAction: //调整持仓请求
		{
			*nTNo = TID_SendPositionAction;
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("PositionID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][PositionID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->PositionID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("ProductGroup");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][ProductGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->ProductGroup.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->TradeUnitID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("PosiDirection");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->PosiDirection.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][PosiDirection][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->PosiDirection.clear();
			}
			iter = jsonMap.find("Amount");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->Amount.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][Amount][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->Amount.clear();
			}
			iter = jsonMap.find("LongLeverage");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->LongLeverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][LongLeverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->LongLeverage.clear();
			}
			iter = jsonMap.find("ShortLeverage");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->ShortLeverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][ShortLeverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->ShortLeverage.clear();
			}
			iter = jsonMap.find("NetLeverage");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->NetLeverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][NetLeverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->NetLeverage.clear();
			}
			iter = jsonMap.find("ActionType");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->ActionType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][ActionType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->ActionType.clear();
			}
			iter = jsonMap.find("IsCrossMargin");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->IsCrossMargin.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][IsCrossMargin][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->IsCrossMargin.clear();
			}
			iter = jsonMap.find("IsAutoAddMargin");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->IsAutoAddMargin.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][IsAutoAddMargin][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->IsAutoAddMargin.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->Remark.clear();
			}
			return &CPositionActionField::m_Describe;
		
		}
	case TID_SendAccountAction: //用户出入金请求
		{
			*nTNo = TID_SendAccountAction;
			iter = jsonMap.find("ActionLocalID");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->ActionLocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][ActionLocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->ActionLocalID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("SettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][SettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->SettlementGroup.clear();
			}
			iter = jsonMap.find("OtherAccountID");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->OtherAccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][OtherAccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->OtherAccountID.clear();
			}
			iter = jsonMap.find("OtherSettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->OtherSettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][OtherSettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->OtherSettlementGroup.clear();
			}
			iter = jsonMap.find("OtherMemberID");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->OtherMemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][OtherMemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->OtherMemberID.clear();
			}
			iter = jsonMap.find("Amount");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->Amount.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][Amount][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->Amount.clear();
			}
			iter = jsonMap.find("FrozenMoney");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->FrozenMoney.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][FrozenMoney][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->FrozenMoney.clear();
			}
			iter = jsonMap.find("Currency");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][Currency][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->Currency.clear();
			}
			iter = jsonMap.find("ActionType");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->ActionType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][ActionType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->ActionType.clear();
			}
			iter = jsonMap.find("IsReserve");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->IsReserve.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][IsReserve][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->IsReserve.clear();
			}
			iter = jsonMap.find("AccountType");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->AccountType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][AccountType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->AccountType.clear();
			}
			iter = jsonMap.find("MarginRateGrade");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->MarginRateGrade.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][MarginRateGrade][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->MarginRateGrade.clear();
			}
			iter = jsonMap.find("TradingRightGrade");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->TradingRightGrade.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][TradingRightGrade][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->TradingRightGrade.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Source");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->Source.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][Source][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->Source.clear();
			}
			iter = jsonMap.find("RelatedID");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->RelatedID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][RelatedID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->RelatedID.clear();
			}
			iter = jsonMap.find("FeeDeduceRate");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->FeeDeduceRate.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][FeeDeduceRate][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->FeeDeduceRate.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CAccountActionField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[AccountAction][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAccountActionField*)pOutput)->Remark.clear();
			}
			return &CAccountActionField::m_Describe;
		
		}
	case TID_SendMemberAction: //成员设置请求
		{
			*nTNo = TID_SendMemberAction;
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CMemberActionField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MemberAction][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMemberActionField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CMemberActionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MemberAction][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMemberActionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("RiskLevel");
			if(iter != jsonMap.end())
			{
				if(!((CMemberActionField*)pOutput)->RiskLevel.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MemberAction][RiskLevel][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMemberActionField*)pOutput)->RiskLevel.clear();
			}
			iter = jsonMap.find("PositionType");
			if(iter != jsonMap.end())
			{
				if(!((CMemberActionField*)pOutput)->PositionType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[MemberAction][PositionType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CMemberActionField*)pOutput)->PositionType.clear();
			}
			return &CMemberActionField::m_Describe;
		
		}
	case TID_SendRobotOrder: //内部做市商申报请求
		{
			*nTNo = TID_SendRobotOrder;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->TradeUnitID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("OrderPriceType");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->OrderPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][OrderPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->OrderPriceType.clear();
			}
			iter = jsonMap.find("Direction");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][Direction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->Direction.clear();
			}
			iter = jsonMap.find("OffsetFlag");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->OffsetFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][OffsetFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->OffsetFlag.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("VolumeDisplay");
			if(iter != jsonMap.end())
			{
				if(!((CRobotOrderField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[RobotOrder][VolumeDisplay][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CRobotOrderField*)pOutput)->VolumeDisplay.clear();
			}
			return &CRobotOrderField::m_Describe;
		
		}
	case TID_SendOTCTradeInsert: //场外成交申报请求
		{
			*nTNo = TID_SendOTCTradeInsert;
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("BuyMemberID");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->BuyMemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][BuyMemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->BuyMemberID.clear();
			}
			iter = jsonMap.find("BuyAccountID");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->BuyAccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][BuyAccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->BuyAccountID.clear();
			}
			iter = jsonMap.find("BuyTradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->BuyTradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][BuyTradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->BuyTradeUnitID.clear();
			}
			iter = jsonMap.find("BuyOffsetFlag");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->BuyOffsetFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][BuyOffsetFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->BuyOffsetFlag.clear();
			}
			iter = jsonMap.find("SellMemberID");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->SellMemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][SellMemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->SellMemberID.clear();
			}
			iter = jsonMap.find("SellAccountID");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->SellAccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][SellAccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->SellAccountID.clear();
			}
			iter = jsonMap.find("SellTradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->SellTradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][SellTradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->SellTradeUnitID.clear();
			}
			iter = jsonMap.find("SellOffsetFlag");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->SellOffsetFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][SellOffsetFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->SellOffsetFlag.clear();
			}
			iter = jsonMap.find("OTCType");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->OTCType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][OTCType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->OTCType.clear();
			}
			iter = jsonMap.find("TakerDirection");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->TakerDirection.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][TakerDirection][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->TakerDirection.clear();
			}
			iter = jsonMap.find("TriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->TriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][TriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->TriggerPrice.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((COTCTradeInsertField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTCTradeInsert][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTCTradeInsertField*)pOutput)->Remark.clear();
			}
			return &COTCTradeInsertField::m_Describe;
		
		}
	case TID_SendSettlement: //资金结算请求
		{
			*nTNo = TID_SendSettlement;
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("SettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][SettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->SettlementGroup.clear();
			}
			iter = jsonMap.find("SettleAction");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->SettleAction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][SettleAction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->SettleAction.clear();
			}
			iter = jsonMap.find("Value");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->Value.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][Value][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->Value.clear();
			}
			iter = jsonMap.find("Value1");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->Value1.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][Value1][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->Value1.clear();
			}
			iter = jsonMap.find("Value2");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->Value2.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][Value2][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->Value2.clear();
			}
			iter = jsonMap.find("Value3");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->Value3.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][Value3][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->Value3.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CSettlementField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Settlement][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CSettlementField*)pOutput)->Remark.clear();
			}
			return &CSettlementField::m_Describe;
		
		}
	case TID_SendClearMarket: //清理行情数据请求
		{
			*nTNo = TID_SendClearMarket;
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CClearMarketField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ClearMarket][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CClearMarketField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CClearMarketField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ClearMarket][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CClearMarketField*)pOutput)->InstrumentID.clear();
			}
			return &CClearMarketField::m_Describe;
		
		}
	case TID_SendClearLastKLine: //清理最新K线请求
		{
			*nTNo = TID_SendClearLastKLine;
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CClearLastKLineField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ClearLastKLine][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CClearLastKLineField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CClearLastKLineField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ClearLastKLine][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CClearLastKLineField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Bar");
			if(iter != jsonMap.end())
			{
				if(!((CClearLastKLineField*)pOutput)->Bar.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ClearLastKLine][Bar][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CClearLastKLineField*)pOutput)->Bar.clear();
			}
			return &CClearLastKLineField::m_Describe;
		
		}
	case TID_SendResetLocalID: //重置LocalID请求
		{
			*nTNo = TID_SendResetLocalID;
			iter = jsonMap.find("Token");
			if(iter != jsonMap.end())
			{
				if(!((CResetLocalIDField*)pOutput)->Token.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ResetLocalID][Token][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CResetLocalIDField*)pOutput)->Token.clear();
			}
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CResetLocalIDField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ResetLocalID][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CResetLocalIDField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((CResetLocalIDField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ResetLocalID][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CResetLocalIDField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CResetLocalIDField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ResetLocalID][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CResetLocalIDField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("SettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CResetLocalIDField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ResetLocalID][SettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CResetLocalIDField*)pOutput)->SettlementGroup.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CResetLocalIDField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ResetLocalID][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CResetLocalIDField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("Currency");
			if(iter != jsonMap.end())
			{
				if(!((CResetLocalIDField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ResetLocalID][Currency][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CResetLocalIDField*)pOutput)->Currency.clear();
			}
			return &CResetLocalIDField::m_Describe;
		
		}
	case TID_SendOTokenInsert: //OToken录入请求
		{
			*nTNo = TID_SendOTokenInsert;
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((COTokenInsertField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTokenInsert][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTokenInsertField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((COTokenInsertField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTokenInsert][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTokenInsertField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("Token");
			if(iter != jsonMap.end())
			{
				if(!((COTokenInsertField*)pOutput)->Token.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTokenInsert][Token][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTokenInsertField*)pOutput)->Token.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((COTokenInsertField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTokenInsert][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTokenInsertField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("LoginTime");
			if(iter != jsonMap.end())
			{
				if(!((COTokenInsertField*)pOutput)->LoginTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTokenInsert][LoginTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTokenInsertField*)pOutput)->LoginTime.clear();
			}
			iter = jsonMap.find("ExpireTime");
			if(iter != jsonMap.end())
			{
				if(!((COTokenInsertField*)pOutput)->ExpireTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTokenInsert][ExpireTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTokenInsertField*)pOutput)->ExpireTime.clear();
			}
			iter = jsonMap.find("AccessLimit");
			if(iter != jsonMap.end())
			{
				if(!((COTokenInsertField*)pOutput)->AccessLimit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTokenInsert][AccessLimit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTokenInsertField*)pOutput)->AccessLimit.clear();
			}
			return &COTokenInsertField::m_Describe;
		
		}
	case TID_SendOTokenAction: //OToken操作请求
		{
			*nTNo = TID_SendOTokenAction;
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((COTokenActionField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTokenAction][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTokenActionField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((COTokenActionField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTokenAction][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTokenActionField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("ActionFlag");
			if(iter != jsonMap.end())
			{
				if(!((COTokenActionField*)pOutput)->ActionFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OTokenAction][ActionFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COTokenActionField*)pOutput)->ActionFlag.clear();
			}
			return &COTokenActionField::m_Describe;
		
		}
	case TID_SendAPIKeyInsert: //APIKEY录入请求
		{
			*nTNo = TID_SendAPIKeyInsert;
			iter = jsonMap.find("Name");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->Name.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][Name][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->Name.clear();
			}
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccessKey");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->AccessKey.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][AccessKey][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->AccessKey.clear();
			}
			iter = jsonMap.find("SecretKey");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->SecretKey.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][SecretKey][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->SecretKey.clear();
			}
			iter = jsonMap.find("Auth");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->Auth.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][Auth][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->Auth.clear();
			}
			iter = jsonMap.find("AccessLimit");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->AccessLimit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][AccessLimit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->AccessLimit.clear();
			}
			iter = jsonMap.find("CreateTime");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->CreateTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][CreateTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->CreateTime.clear();
			}
			iter = jsonMap.find("ExpireTime");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->ExpireTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][ExpireTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->ExpireTime.clear();
			}
			iter = jsonMap.find("IPAddress");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyInsertField*)pOutput)->IPAddress.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyInsert][IPAddress][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyInsertField*)pOutput)->IPAddress.clear();
			}
			return &CAPIKeyInsertField::m_Describe;
		
		}
	case TID_SendAPIKeyAction: //APIKEY操作请求
		{
			*nTNo = TID_SendAPIKeyAction;
			iter = jsonMap.find("Name");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyActionField*)pOutput)->Name.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyAction][Name][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyActionField*)pOutput)->Name.clear();
			}
			iter = jsonMap.find("AccessKey");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyActionField*)pOutput)->AccessKey.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyAction][AccessKey][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyActionField*)pOutput)->AccessKey.clear();
			}
			iter = jsonMap.find("Auth");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyActionField*)pOutput)->Auth.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyAction][Auth][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyActionField*)pOutput)->Auth.clear();
			}
			iter = jsonMap.find("AccessLimit");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyActionField*)pOutput)->AccessLimit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyAction][AccessLimit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyActionField*)pOutput)->AccessLimit.clear();
			}
			iter = jsonMap.find("IPAddress");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyActionField*)pOutput)->IPAddress.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyAction][IPAddress][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyActionField*)pOutput)->IPAddress.clear();
			}
			iter = jsonMap.find("ExpireTime");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyActionField*)pOutput)->ExpireTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyAction][ExpireTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyActionField*)pOutput)->ExpireTime.clear();
			}
			iter = jsonMap.find("ActionFlag");
			if(iter != jsonMap.end())
			{
				if(!((CAPIKeyActionField*)pOutput)->ActionFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[APIKeyAction][ActionFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CAPIKeyActionField*)pOutput)->ActionFlag.clear();
			}
			return &CAPIKeyActionField::m_Describe;
		
		}
	case TID_SendPositionMerge: //仓位合并操作请求
		{
			*nTNo = TID_SendPositionMerge;
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("PositionID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID.clear();
			}
			iter = jsonMap.find("PositionID1");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID1.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID1][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID1.clear();
			}
			iter = jsonMap.find("PositionID2");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID2.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID2][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID2.clear();
			}
			iter = jsonMap.find("PositionID3");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID3.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID3][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID3.clear();
			}
			iter = jsonMap.find("PositionID4");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID4.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID4][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID4.clear();
			}
			iter = jsonMap.find("PositionID5");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID5.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID5][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID5.clear();
			}
			iter = jsonMap.find("PositionID6");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID6.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID6][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID6.clear();
			}
			iter = jsonMap.find("PositionID7");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID7.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID7][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID7.clear();
			}
			iter = jsonMap.find("PositionID8");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID8.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID8][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID8.clear();
			}
			iter = jsonMap.find("PositionID9");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID9.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID9][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID9.clear();
			}
			iter = jsonMap.find("PositionID10");
			if(iter != jsonMap.end())
			{
				if(!((CPositionMergeField*)pOutput)->PositionID10.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionMerge][PositionID10][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionMergeField*)pOutput)->PositionID10.clear();
			}
			return &CPositionMergeField::m_Describe;
		
		}
	case TID_SendDeposit: //用户入金操作请求
		{
			*nTNo = TID_SendDeposit;
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CDepositField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Deposit][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CDepositField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CDepositField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Deposit][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CDepositField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CDepositField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Deposit][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CDepositField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("SettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CDepositField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Deposit][SettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CDepositField*)pOutput)->SettlementGroup.clear();
			}
			iter = jsonMap.find("Amount");
			if(iter != jsonMap.end())
			{
				if(!((CDepositField*)pOutput)->Amount.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Deposit][Amount][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CDepositField*)pOutput)->Amount.clear();
			}
			iter = jsonMap.find("Currency");
			if(iter != jsonMap.end())
			{
				if(!((CDepositField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Deposit][Currency][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CDepositField*)pOutput)->Currency.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CDepositField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Deposit][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CDepositField*)pOutput)->Remark.clear();
			}
			return &CDepositField::m_Describe;
		
		}
	case TID_SendWithdraw: //用户出金操作请求
		{
			*nTNo = TID_SendWithdraw;
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CWithdrawField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Withdraw][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWithdrawField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CWithdrawField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Withdraw][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWithdrawField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CWithdrawField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Withdraw][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWithdrawField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CWithdrawField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Withdraw][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWithdrawField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("SettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CWithdrawField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Withdraw][SettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWithdrawField*)pOutput)->SettlementGroup.clear();
			}
			iter = jsonMap.find("OtherSettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CWithdrawField*)pOutput)->OtherSettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Withdraw][OtherSettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWithdrawField*)pOutput)->OtherSettlementGroup.clear();
			}
			iter = jsonMap.find("Amount");
			if(iter != jsonMap.end())
			{
				if(!((CWithdrawField*)pOutput)->Amount.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Withdraw][Amount][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWithdrawField*)pOutput)->Amount.clear();
			}
			iter = jsonMap.find("Currency");
			if(iter != jsonMap.end())
			{
				if(!((CWithdrawField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Withdraw][Currency][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWithdrawField*)pOutput)->Currency.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CWithdrawField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Withdraw][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWithdrawField*)pOutput)->Remark.clear();
			}
			return &CWithdrawField::m_Describe;
		
		}
	case TID_SendTransfer: //用户转账操作请求
		{
			*nTNo = TID_SendTransfer;
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CTransferField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Transfer][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTransferField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CTransferField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Transfer][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTransferField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CTransferField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Transfer][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTransferField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("SettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CTransferField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Transfer][SettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTransferField*)pOutput)->SettlementGroup.clear();
			}
			iter = jsonMap.find("OtherAccountID");
			if(iter != jsonMap.end())
			{
				if(!((CTransferField*)pOutput)->OtherAccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Transfer][OtherAccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTransferField*)pOutput)->OtherAccountID.clear();
			}
			iter = jsonMap.find("OtherSettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CTransferField*)pOutput)->OtherSettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Transfer][OtherSettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTransferField*)pOutput)->OtherSettlementGroup.clear();
			}
			iter = jsonMap.find("OtherMemberID");
			if(iter != jsonMap.end())
			{
				if(!((CTransferField*)pOutput)->OtherMemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Transfer][OtherMemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTransferField*)pOutput)->OtherMemberID.clear();
			}
			iter = jsonMap.find("Amount");
			if(iter != jsonMap.end())
			{
				if(!((CTransferField*)pOutput)->Amount.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Transfer][Amount][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTransferField*)pOutput)->Amount.clear();
			}
			iter = jsonMap.find("Currency");
			if(iter != jsonMap.end())
			{
				if(!((CTransferField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Transfer][Currency][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTransferField*)pOutput)->Currency.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CTransferField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[Transfer][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CTransferField*)pOutput)->Remark.clear();
			}
			return &CTransferField::m_Describe;
		
		}
	case TID_SendUserRegister: //注册用户请求
		{
			*nTNo = TID_SendUserRegister;
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("Password");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->Password.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][Password][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->Password.clear();
			}
			iter = jsonMap.find("UserProductID");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->UserProductID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][UserProductID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->UserProductID.clear();
			}
			iter = jsonMap.find("MacAddress");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->MacAddress.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][MacAddress][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->MacAddress.clear();
			}
			iter = jsonMap.find("ClientIPAddress");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->ClientIPAddress.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][ClientIPAddress][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->ClientIPAddress.clear();
			}
			iter = jsonMap.find("HDSerialID");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->HDSerialID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][HDSerialID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->HDSerialID.clear();
			}
			iter = jsonMap.find("AuthCode");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->AuthCode.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][AuthCode][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->AuthCode.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("Token");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->Token.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][Token][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->Token.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->Remark.clear();
			}
			iter = jsonMap.find("APIID");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->APIID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][APIID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->APIID.clear();
			}
			iter = jsonMap.find("LoginTime");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->LoginTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][LoginTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->LoginTime.clear();
			}
			iter = jsonMap.find("ExpireTime");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->ExpireTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][ExpireTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->ExpireTime.clear();
			}
			iter = jsonMap.find("IPAddress");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->IPAddress.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][IPAddress][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->IPAddress.clear();
			}
			iter = jsonMap.find("MaxLocalID");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->MaxLocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][MaxLocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->MaxLocalID.clear();
			}
			iter = jsonMap.find("SessionNo");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->SessionNo.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][SessionNo][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->SessionNo.clear();
			}
			iter = jsonMap.find("FrontNo");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->FrontNo.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][FrontNo][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->FrontNo.clear();
			}
			iter = jsonMap.find("AccessLimit");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->AccessLimit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][AccessLimit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->AccessLimit.clear();
			}
			iter = jsonMap.find("UserType");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->UserType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][UserType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->UserType.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("LimitAccesses");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->LimitAccesses.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][LimitAccesses][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->LimitAccesses.clear();
			}
			iter = jsonMap.find("SinkType");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->SinkType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][SinkType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->SinkType.clear();
			}
			iter = jsonMap.find("InsertTime");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][InsertTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->InsertTime.clear();
			}
			iter = jsonMap.find("UpdateTime");
			if(iter != jsonMap.end())
			{
				if(!((CUserSessionField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[UserSession][UpdateTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CUserSessionField*)pOutput)->UpdateTime.clear();
			}
			return &CUserSessionField::m_Describe;
		
		}
	case TID_SendPositionReverse: //反向开仓录入请求
		{
			*nTNo = TID_SendPositionReverse;
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionReverseField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionReverse][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionReverseField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionReverseField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionReverse][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionReverseField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("PositionID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionReverseField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionReverse][PositionID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionReverseField*)pOutput)->PositionID.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((CPositionReverseField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionReverse][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionReverseField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CPositionReverseField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionReverse][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionReverseField*)pOutput)->Remark.clear();
			}
			iter = jsonMap.find("CopyMemberID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionReverseField*)pOutput)->CopyMemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionReverse][CopyMemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionReverseField*)pOutput)->CopyMemberID.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionReverseField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionReverse][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionReverseField*)pOutput)->APPID.clear();
			}
			return &CPositionReverseField::m_Describe;
		
		}
	case TID_SendCancelOrderInsert: //避免自成交报单录入请求
		{
			*nTNo = TID_SendCancelOrderInsert;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->TradeUnitID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("OrderPriceType");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->OrderPriceType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][OrderPriceType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->OrderPriceType.clear();
			}
			iter = jsonMap.find("Direction");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Direction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Direction.clear();
			}
			iter = jsonMap.find("OffsetFlag");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->OffsetFlag.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][OffsetFlag][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->OffsetFlag.clear();
			}
			iter = jsonMap.find("Price");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Price][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Price.clear();
			}
			iter = jsonMap.find("Volume");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Volume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Volume.clear();
			}
			iter = jsonMap.find("VolumeDisplay");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][VolumeDisplay][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->VolumeDisplay.clear();
			}
			iter = jsonMap.find("VolumeMode");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->VolumeMode.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][VolumeMode][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->VolumeMode.clear();
			}
			iter = jsonMap.find("Cost");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Cost][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Cost.clear();
			}
			iter = jsonMap.find("OrderType");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->OrderType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][OrderType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->OrderType.clear();
			}
			iter = jsonMap.find("GTDTime");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->GTDTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][GTDTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->GTDTime.clear();
			}
			iter = jsonMap.find("MinVolume");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->MinVolume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][MinVolume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->MinVolume.clear();
			}
			iter = jsonMap.find("BusinessType");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->BusinessType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][BusinessType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->BusinessType.clear();
			}
			iter = jsonMap.find("BusinessValue");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->BusinessValue.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][BusinessValue][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->BusinessValue.clear();
			}
			iter = jsonMap.find("CloseOrderID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->CloseOrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][CloseOrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->CloseOrderID.clear();
			}
			iter = jsonMap.find("IsCrossMargin");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->IsCrossMargin.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][IsCrossMargin][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->IsCrossMargin.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Remark.clear();
			}
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("CopyMemberID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->CopyMemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][CopyMemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->CopyMemberID.clear();
			}
			iter = jsonMap.find("CopyOrderID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->CopyOrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][CopyOrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->CopyOrderID.clear();
			}
			iter = jsonMap.find("Leverage");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Leverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Leverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Leverage.clear();
			}
			iter = jsonMap.find("CopyProfitRate");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->CopyProfitRate.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][CopyProfitRate][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->CopyProfitRate.clear();
			}
			iter = jsonMap.find("APPID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][APPID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->APPID.clear();
			}
			iter = jsonMap.find("PositionID");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][PositionID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->PositionID.clear();
			}
			iter = jsonMap.find("TriggerPrice");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->TriggerPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][TriggerPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->TriggerPrice.clear();
			}
			iter = jsonMap.find("Reserve");
			if(iter != jsonMap.end())
			{
				if(!((COrderInsertField*)pOutput)->Reserve.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[OrderInsert][Reserve][isInValid]");
					return NULL;
				}
			}
			else
			{
				((COrderInsertField*)pOutput)->Reserve.clear();
			}
			return &COrderInsertField::m_Describe;
		
		}
	case TID_SendPrivateDispatch: //私有主题分类请求
		{
			*nTNo = TID_SendPrivateDispatch;
			iter = jsonMap.find("TopicID");
			if(iter != jsonMap.end())
			{
				if(!((CPrivateDispatchField*)pOutput)->TopicID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PrivateDispatch][TopicID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPrivateDispatchField*)pOutput)->TopicID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CPrivateDispatchField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PrivateDispatch][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPrivateDispatchField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("PrivateBase");
			if(iter != jsonMap.end())
			{
				if(!((CPrivateDispatchField*)pOutput)->PrivateBase.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PrivateDispatch][PrivateBase][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPrivateDispatchField*)pOutput)->PrivateBase.clear();
			}
			return &CPrivateDispatchField::m_Describe;
		
		}
	case TID_SendPositionLeverage: //调整多空杠杠请求
		{
			*nTNo = TID_SendPositionLeverage;
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("PositionID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][PositionID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->PositionID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("ProductGroup");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][ProductGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->ProductGroup.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->TradeUnitID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("PosiDirection");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->PosiDirection.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][PosiDirection][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->PosiDirection.clear();
			}
			iter = jsonMap.find("Amount");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->Amount.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][Amount][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->Amount.clear();
			}
			iter = jsonMap.find("LongLeverage");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->LongLeverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][LongLeverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->LongLeverage.clear();
			}
			iter = jsonMap.find("ShortLeverage");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->ShortLeverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][ShortLeverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->ShortLeverage.clear();
			}
			iter = jsonMap.find("NetLeverage");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->NetLeverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][NetLeverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->NetLeverage.clear();
			}
			iter = jsonMap.find("ActionType");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->ActionType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][ActionType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->ActionType.clear();
			}
			iter = jsonMap.find("IsCrossMargin");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->IsCrossMargin.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][IsCrossMargin][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->IsCrossMargin.clear();
			}
			iter = jsonMap.find("IsAutoAddMargin");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->IsAutoAddMargin.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][IsAutoAddMargin][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->IsAutoAddMargin.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CPositionActionField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[PositionAction][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CPositionActionField*)pOutput)->Remark.clear();
			}
			return &CPositionActionField::m_Describe;
		
		}
	case TID_SendInstrumentUpdate: //设置合约请求
		{
			*nTNo = TID_SendInstrumentUpdate;
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("MinOrderVolume");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->MinOrderVolume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][MinOrderVolume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->MinOrderVolume.clear();
			}
			iter = jsonMap.find("MinOrderCost");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->MinOrderCost.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][MinOrderCost][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->MinOrderCost.clear();
			}
			iter = jsonMap.find("MaxOrderVolume");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->MaxOrderVolume.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][MaxOrderVolume][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->MaxOrderVolume.clear();
			}
			iter = jsonMap.find("PriceTick");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->PriceTick.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][PriceTick][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->PriceTick.clear();
			}
			iter = jsonMap.find("VolumeTick");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->VolumeTick.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][VolumeTick][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->VolumeTick.clear();
			}
			iter = jsonMap.find("MaxOpenInterest");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->MaxOpenInterest.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][MaxOpenInterest][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->MaxOpenInterest.clear();
			}
			iter = jsonMap.find("BasisPrice");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->BasisPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][BasisPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->BasisPrice.clear();
			}
			iter = jsonMap.find("UpperLimitPrice");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->UpperLimitPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][UpperLimitPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->UpperLimitPrice.clear();
			}
			iter = jsonMap.find("LowerLimitPrice");
			if(iter != jsonMap.end())
			{
				if(!((CInstrumentUpdateField*)pOutput)->LowerLimitPrice.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[InstrumentUpdate][LowerLimitPrice][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CInstrumentUpdateField*)pOutput)->LowerLimitPrice.clear();
			}
			return &CInstrumentUpdateField::m_Describe;
		
		}
	case TID_SendChangeCurrency: //充币兑换请求
		{
			*nTNo = TID_SendChangeCurrency;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CChangeCurrencyField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ChangeCurrency][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CChangeCurrencyField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CChangeCurrencyField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ChangeCurrency][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CChangeCurrencyField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CChangeCurrencyField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ChangeCurrency][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CChangeCurrencyField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CChangeCurrencyField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ChangeCurrency][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CChangeCurrencyField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID1");
			if(iter != jsonMap.end())
			{
				if(!((CChangeCurrencyField*)pOutput)->InstrumentID1.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ChangeCurrency][InstrumentID1][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CChangeCurrencyField*)pOutput)->InstrumentID1.clear();
			}
			iter = jsonMap.find("InstrumentID2");
			if(iter != jsonMap.end())
			{
				if(!((CChangeCurrencyField*)pOutput)->InstrumentID2.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ChangeCurrency][InstrumentID2][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CChangeCurrencyField*)pOutput)->InstrumentID2.clear();
			}
			iter = jsonMap.find("Currency");
			if(iter != jsonMap.end())
			{
				if(!((CChangeCurrencyField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ChangeCurrency][Currency][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CChangeCurrencyField*)pOutput)->Currency.clear();
			}
			iter = jsonMap.find("Amount");
			if(iter != jsonMap.end())
			{
				if(!((CChangeCurrencyField*)pOutput)->Amount.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ChangeCurrency][Amount][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CChangeCurrencyField*)pOutput)->Amount.clear();
			}
			iter = jsonMap.find("Remark");
			if(iter != jsonMap.end())
			{
				if(!((CChangeCurrencyField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[ChangeCurrency][Remark][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CChangeCurrencyField*)pOutput)->Remark.clear();
			}
			return &CChangeCurrencyField::m_Describe;
		
		}
	case TID_SendWalletNonce: //钱包登录获取随机数请求
		{
			*nTNo = TID_SendWalletNonce;
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CWalletNonceField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[WalletNonce][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWalletNonceField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CWalletNonceField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[WalletNonce][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWalletNonceField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("Nonce");
			if(iter != jsonMap.end())
			{
				if(!((CWalletNonceField*)pOutput)->Nonce.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[WalletNonce][Nonce][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWalletNonceField*)pOutput)->Nonce.clear();
			}
			return &CWalletNonceField::m_Describe;
		
		}
	case TID_SendWalletLogin: //钱包登录获取随机数请求
		{
			*nTNo = TID_SendWalletLogin;
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CWalletLoginField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[WalletLogin][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWalletLoginField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("Signature");
			if(iter != jsonMap.end())
			{
				if(!((CWalletLoginField*)pOutput)->Signature.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[WalletLogin][Signature][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CWalletLoginField*)pOutput)->Signature.clear();
			}
			return &CWalletLoginField::m_Describe;
		
		}
	case TID_SendQryAccount: //资金查询请求
		{
			*nTNo = TID_SendQryAccount;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccount][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccount][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("SettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccount][SettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountField*)pOutput)->SettlementGroup.clear();
			}
			iter = jsonMap.find("Currency");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccount][Currency][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountField*)pOutput)->Currency.clear();
			}
			return &CQryAccountField::m_Describe;
		
		}
	case TID_SendQryOrder: //报单查询请求
		{
			*nTNo = TID_SendQryOrder;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryOrderField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryOrder][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryOrderField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryOrderField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryOrder][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryOrderField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryOrderField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryOrder][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryOrderField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryOrderField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryOrder][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryOrderField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CQryOrderField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryOrder][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryOrderField*)pOutput)->LocalID.clear();
			}
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CQryOrderField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryOrder][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryOrderField*)pOutput)->OrderID.clear();
			}
			return &CQryOrderField::m_Describe;
		
		}
	case TID_SendQryMemberPosition: //持仓汇总查询请求
		{
			*nTNo = TID_SendQryMemberPosition;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMemberPositionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMemberPosition][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMemberPositionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMemberPositionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMemberPosition][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMemberPositionField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMemberPositionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMemberPosition][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMemberPositionField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMemberPositionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMemberPosition][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMemberPositionField*)pOutput)->InstrumentID.clear();
			}
			return &CQryMemberPositionField::m_Describe;
		
		}
	case TID_SendQryPosition: //持仓查询请求
		{
			*nTNo = TID_SendQryPosition;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryPositionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryPosition][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryPositionField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryPositionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryPosition][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryPositionField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryPositionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryPosition][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryPositionField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryPositionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryPosition][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryPositionField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("PositionClass");
			if(iter != jsonMap.end())
			{
				if(!((CQryPositionField*)pOutput)->PositionClass.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryPosition][PositionClass][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryPositionField*)pOutput)->PositionClass.clear();
			}
			return &CQryPositionField::m_Describe;
		
		}
	case TID_SendQryInstrument: //标的查询请求
		{
			*nTNo = TID_SendQryInstrument;
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryInstrumentField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryInstrument][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryInstrumentField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("ProductGroup");
			if(iter != jsonMap.end())
			{
				if(!((CQryInstrumentField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryInstrument][ProductGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryInstrumentField*)pOutput)->ProductGroup.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryInstrumentField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryInstrument][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryInstrumentField*)pOutput)->InstrumentID.clear();
			}
			return &CQryInstrumentField::m_Describe;
		
		}
	case TID_SendQryCurrency: //币种查询请求
		{
			*nTNo = TID_SendQryCurrency;
			iter = jsonMap.find("SettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CQryCurrencyField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryCurrency][SettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryCurrencyField*)pOutput)->SettlementGroup.clear();
			}
			iter = jsonMap.find("Currency");
			if(iter != jsonMap.end())
			{
				if(!((CQryCurrencyField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryCurrency][Currency][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryCurrencyField*)pOutput)->Currency.clear();
			}
			return &CQryCurrencyField::m_Describe;
		
		}
	case TID_SendQryMember: //成员查询请求
		{
			*nTNo = TID_SendQryMember;
			iter = jsonMap.find("MemberType");
			if(iter != jsonMap.end())
			{
				if(!((CQryMemberField*)pOutput)->MemberType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMember][MemberType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMemberField*)pOutput)->MemberType.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMemberField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMember][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMemberField*)pOutput)->MemberID.clear();
			}
			return &CQryMemberField::m_Describe;
		
		}
	case TID_SendQryMarketOrder: //价位查询请求
		{
			*nTNo = TID_SendQryMarketOrder;
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarketOrderField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarketOrder][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarketOrderField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarketOrderField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarketOrder][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarketOrderField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Direction");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarketOrderField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarketOrder][Direction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarketOrderField*)pOutput)->Direction.clear();
			}
			iter = jsonMap.find("Depth");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarketOrderField*)pOutput)->Depth.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarketOrder][Depth][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarketOrderField*)pOutput)->Depth.clear();
			}
			iter = jsonMap.find("Tick");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarketOrderField*)pOutput)->Tick.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarketOrder][Tick][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarketOrderField*)pOutput)->Tick.clear();
			}
			return &CQryMarketOrderField::m_Describe;
		
		}
	case TID_SendQryAPIKey: //APIKEY查询请求
		{
			*nTNo = TID_SendQryAPIKey;
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CQryAPIKeyField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAPIKey][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAPIKeyField*)pOutput)->UserID.clear();
			}
			return &CQryAPIKeyField::m_Describe;
		
		}
	case TID_SendQryMarginRate: //保证金率查询请求
		{
			*nTNo = TID_SendQryMarginRate;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarginRateField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarginRate][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarginRateField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarginRateField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarginRate][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarginRateField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarginRateField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarginRate][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarginRateField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarginRateField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarginRate][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarginRateField*)pOutput)->AccountID.clear();
			}
			return &CQryMarginRateField::m_Describe;
		
		}
	case TID_SendQryFee: //手续费率查询请求
		{
			*nTNo = TID_SendQryFee;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFeeField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFee][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFeeField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFeeField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFee][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFeeField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFeeField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFee][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFeeField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("MatchRole");
			if(iter != jsonMap.end())
			{
				if(!((CQryFeeField*)pOutput)->MatchRole.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFee][MatchRole][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFeeField*)pOutput)->MatchRole.clear();
			}
			return &CQryFeeField::m_Describe;
		
		}
	case TID_SendQryTriggerOrder: //触发订单查询请求
		{
			*nTNo = TID_SendQryTriggerOrder;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryTriggerOrderField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTriggerOrder][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTriggerOrderField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryTriggerOrderField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTriggerOrder][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTriggerOrderField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryTriggerOrderField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTriggerOrder][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTriggerOrderField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryTriggerOrderField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTriggerOrder][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTriggerOrderField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("LocalID");
			if(iter != jsonMap.end())
			{
				if(!((CQryTriggerOrderField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTriggerOrder][LocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTriggerOrderField*)pOutput)->LocalID.clear();
			}
			return &CQryTriggerOrderField::m_Describe;
		
		}
	case TID_SendQryLeverage: //杠杆倍数查询请求
		{
			*nTNo = TID_SendQryLeverage;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryLeverageField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryLeverage][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryLeverageField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryLeverageField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryLeverage][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryLeverageField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((CQryLeverageField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryLeverage][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryLeverageField*)pOutput)->TradeUnitID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryLeverageField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryLeverage][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryLeverageField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryLeverageField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryLeverage][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryLeverageField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("PosiDirection");
			if(iter != jsonMap.end())
			{
				if(!((CQryLeverageField*)pOutput)->PosiDirection.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryLeverage][PosiDirection][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryLeverageField*)pOutput)->PosiDirection.clear();
			}
			return &CQryLeverageField::m_Describe;
		
		}
	case TID_SendQryMarketData: //行情查询请求
		{
			*nTNo = TID_SendQryMarketData;
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarketDataField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarketData][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarketDataField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("ProductGroup");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarketDataField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarketData][ProductGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarketDataField*)pOutput)->ProductGroup.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarketDataField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarketData][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarketDataField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("From");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarketDataField*)pOutput)->From.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarketData][From][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarketDataField*)pOutput)->From.clear();
			}
			iter = jsonMap.find("Limit");
			if(iter != jsonMap.end())
			{
				if(!((CQryMarketDataField*)pOutput)->Limit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryMarketData][Limit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryMarketDataField*)pOutput)->Limit.clear();
			}
			return &CQryMarketDataField::m_Describe;
		
		}
	case TID_SendQryTrade: //成交查询请求
		{
			*nTNo = TID_SendQryTrade;
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CQryTradeField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTrade][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTradeField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryTradeField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTrade][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTradeField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryTradeField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTrade][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTradeField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryTradeField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTrade][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTradeField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryTradeField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTrade][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTradeField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Limit");
			if(iter != jsonMap.end())
			{
				if(!((CQryTradeField*)pOutput)->Limit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTrade][Limit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTradeField*)pOutput)->Limit.clear();
			}
			iter = jsonMap.find("Before");
			if(iter != jsonMap.end())
			{
				if(!((CQryTradeField*)pOutput)->Before.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTrade][Before][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTradeField*)pOutput)->Before.clear();
			}
			iter = jsonMap.find("After");
			if(iter != jsonMap.end())
			{
				if(!((CQryTradeField*)pOutput)->After.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTrade][After][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTradeField*)pOutput)->After.clear();
			}
			iter = jsonMap.find("StartTime");
			if(iter != jsonMap.end())
			{
				if(!((CQryTradeField*)pOutput)->StartTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTrade][StartTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTradeField*)pOutput)->StartTime.clear();
			}
			iter = jsonMap.find("EndTime");
			if(iter != jsonMap.end())
			{
				if(!((CQryTradeField*)pOutput)->EndTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryTrade][EndTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryTradeField*)pOutput)->EndTime.clear();
			}
			return &CQryTradeField::m_Describe;
		
		}
	case TID_SendQryFinishOrder: //历史委托查询请求
		{
			*nTNo = TID_SendQryFinishOrder;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("OrderStatus");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->OrderStatus.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][OrderStatus][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->OrderStatus.clear();
			}
			iter = jsonMap.find("Limit");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->Limit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][Limit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->Limit.clear();
			}
			iter = jsonMap.find("Before");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->Before.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][Before][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->Before.clear();
			}
			iter = jsonMap.find("After");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->After.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][After][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->After.clear();
			}
			iter = jsonMap.find("StartTime");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->StartTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][StartTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->StartTime.clear();
			}
			iter = jsonMap.find("EndTime");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishOrderField*)pOutput)->EndTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishOrder][EndTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishOrderField*)pOutput)->EndTime.clear();
			}
			return &CQryFinishOrderField::m_Describe;
		
		}
	case TID_SendQryFinishTriggerOrder: //历史条件单查询请求
		{
			*nTNo = TID_SendQryFinishTriggerOrder;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("OrderID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][OrderID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->OrderID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("TriggerStatus");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->TriggerStatus.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][TriggerStatus][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->TriggerStatus.clear();
			}
			iter = jsonMap.find("TriggerOrderType");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->TriggerOrderType.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][TriggerOrderType][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->TriggerOrderType.clear();
			}
			iter = jsonMap.find("Limit");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->Limit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][Limit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->Limit.clear();
			}
			iter = jsonMap.find("Before");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->Before.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][Before][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->Before.clear();
			}
			iter = jsonMap.find("After");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->After.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][After][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->After.clear();
			}
			iter = jsonMap.find("StartTime");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->StartTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][StartTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->StartTime.clear();
			}
			iter = jsonMap.find("EndTime");
			if(iter != jsonMap.end())
			{
				if(!((CQryFinishTriggerOrderField*)pOutput)->EndTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryFinishTriggerOrder][EndTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryFinishTriggerOrderField*)pOutput)->EndTime.clear();
			}
			return &CQryFinishTriggerOrderField::m_Describe;
		
		}
	case TID_SendQryWithLeverage: //指定杠杆倍数查询请求
		{
			*nTNo = TID_SendQryWithLeverage;
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryWithLeverageField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryWithLeverage][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryWithLeverageField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryWithLeverageField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryWithLeverage][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryWithLeverageField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("TradeUnitID");
			if(iter != jsonMap.end())
			{
				if(!((CQryWithLeverageField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryWithLeverage][TradeUnitID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryWithLeverageField*)pOutput)->TradeUnitID.clear();
			}
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryWithLeverageField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryWithLeverage][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryWithLeverageField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("PosiDirection");
			if(iter != jsonMap.end())
			{
				if(!((CQryWithLeverageField*)pOutput)->PosiDirection.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryWithLeverage][PosiDirection][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryWithLeverageField*)pOutput)->PosiDirection.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryWithLeverageField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryWithLeverage][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryWithLeverageField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("LongLeverage");
			if(iter != jsonMap.end())
			{
				if(!((CQryWithLeverageField*)pOutput)->LongLeverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryWithLeverage][LongLeverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryWithLeverageField*)pOutput)->LongLeverage.clear();
			}
			iter = jsonMap.find("ShortLeverage");
			if(iter != jsonMap.end())
			{
				if(!((CQryWithLeverageField*)pOutput)->ShortLeverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryWithLeverage][ShortLeverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryWithLeverageField*)pOutput)->ShortLeverage.clear();
			}
			iter = jsonMap.find("NetLeverage");
			if(iter != jsonMap.end())
			{
				if(!((CQryWithLeverageField*)pOutput)->NetLeverage.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryWithLeverage][NetLeverage][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryWithLeverageField*)pOutput)->NetLeverage.clear();
			}
			return &CQryWithLeverageField::m_Describe;
		
		}
	case TID_SendQryKLine: //K线查询请求
		{
			*nTNo = TID_SendQryKLine;
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryKLineField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryKLine][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryKLineField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryKLineField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryKLine][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryKLineField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Bar");
			if(iter != jsonMap.end())
			{
				if(!((CQryKLineField*)pOutput)->Bar.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryKLine][Bar][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryKLineField*)pOutput)->Bar.clear();
			}
			iter = jsonMap.find("EndTime");
			if(iter != jsonMap.end())
			{
				if(!((CQryKLineField*)pOutput)->EndTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryKLine][EndTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryKLineField*)pOutput)->EndTime.clear();
			}
			iter = jsonMap.find("Limit");
			if(iter != jsonMap.end())
			{
				if(!((CQryKLineField*)pOutput)->Limit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryKLine][Limit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryKLineField*)pOutput)->Limit.clear();
			}
			return &CQryKLineField::m_Describe;
		
		}
	case TID_SendQryLine: //线查询请求
		{
			*nTNo = TID_SendQryLine;
			iter = jsonMap.find("ExchangeID");
			if(iter != jsonMap.end())
			{
				if(!((CQryLineField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryLine][ExchangeID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryLineField*)pOutput)->ExchangeID.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryLineField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryLine][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryLineField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Bar");
			if(iter != jsonMap.end())
			{
				if(!((CQryLineField*)pOutput)->Bar.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryLine][Bar][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryLineField*)pOutput)->Bar.clear();
			}
			iter = jsonMap.find("Limit");
			if(iter != jsonMap.end())
			{
				if(!((CQryLineField*)pOutput)->Limit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryLine][Limit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryLineField*)pOutput)->Limit.clear();
			}
			return &CQryLineField::m_Describe;
		
		}
	case TID_SendQryAccountDetail: //资金流水查询请求
		{
			*nTNo = TID_SendQryAccountDetail;
			iter = jsonMap.find("ProductGroup");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][ProductGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->ProductGroup.clear();
			}
			iter = jsonMap.find("AccountID");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][AccountID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->AccountID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->MemberID.clear();
			}
			iter = jsonMap.find("Currency");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][Currency][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->Currency.clear();
			}
			iter = jsonMap.find("SettlementGroup");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][SettlementGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->SettlementGroup.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("Source");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->Source.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][Source][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->Source.clear();
			}
			iter = jsonMap.find("Limit");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->Limit.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][Limit][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->Limit.clear();
			}
			iter = jsonMap.find("Before");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->Before.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][Before][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->Before.clear();
			}
			iter = jsonMap.find("After");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->After.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][After][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->After.clear();
			}
			iter = jsonMap.find("StartTime");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->StartTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][StartTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->StartTime.clear();
			}
			iter = jsonMap.find("EndTime");
			if(iter != jsonMap.end())
			{
				if(!((CQryAccountDetailField*)pOutput)->EndTime.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QryAccountDetail][EndTime][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQryAccountDetailField*)pOutput)->EndTime.clear();
			}
			return &CQryAccountDetailField::m_Describe;
		
		}
	case TID_SendQrySettleDetail: //结算流水查询请求
		{
			*nTNo = TID_SendQrySettleDetail;
			iter = jsonMap.find("ProductGroup");
			if(iter != jsonMap.end())
			{
				if(!((CQrySettleDetailField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QrySettleDetail][ProductGroup][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQrySettleDetailField*)pOutput)->ProductGroup.clear();
			}
			iter = jsonMap.find("InstrumentID");
			if(iter != jsonMap.end())
			{
				if(!((CQrySettleDetailField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QrySettleDetail][InstrumentID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQrySettleDetailField*)pOutput)->InstrumentID.clear();
			}
			iter = jsonMap.find("SettleAction");
			if(iter != jsonMap.end())
			{
				if(!((CQrySettleDetailField*)pOutput)->SettleAction.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[QrySettleDetail][SettleAction][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CQrySettleDetailField*)pOutput)->SettleAction.clear();
			}
			return &CQrySettleDetailField::m_Describe;
		
		}
	case TID_SendCheckToken: //检查TOKEN请求
		{
			*nTNo = TID_SendCheckToken;
			iter = jsonMap.find("Token");
			if(iter != jsonMap.end())
			{
				if(!((CCheckTokenField*)pOutput)->Token.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CheckToken][Token][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCheckTokenField*)pOutput)->Token.clear();
			}
			iter = jsonMap.find("UserID");
			if(iter != jsonMap.end())
			{
				if(!((CCheckTokenField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CheckToken][UserID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCheckTokenField*)pOutput)->UserID.clear();
			}
			iter = jsonMap.find("MaxLocalID");
			if(iter != jsonMap.end())
			{
				if(!((CCheckTokenField*)pOutput)->MaxLocalID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CheckToken][MaxLocalID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCheckTokenField*)pOutput)->MaxLocalID.clear();
			}
			iter = jsonMap.find("MemberID");
			if(iter != jsonMap.end())
			{
				if(!((CCheckTokenField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CheckToken][MemberID][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCheckTokenField*)pOutput)->MemberID.clear();
			}
			return &CCheckTokenField::m_Describe;
		
		}
	case TID_SendBatch: //批量操作请求(HTTP接口专用)
		{
			*nTNo = TID_SendBatch;
			iter = jsonMap.find("IsLast");
			if(iter != jsonMap.end())
			{
				if(!((CCommandHeadField*)pOutput)->IsLast.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[CommandHead][IsLast][isInValid]");
					return NULL;
				}
			}
			else
			{
				((CCommandHeadField*)pOutput)->IsLast.clear();
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

CFieldDescribe* CJsonAnalysis::FromJsonToStruct(const char* packageName, const char* fieldName, const void *pInput, int nInputLen, void *pOutput,int *nTNo)
{
	CXTPPackageDefineNameMap::iterator iter1 = g_XTPPackageDefineNameMap.find(packageName);
	if(iter1==g_XTPPackageDefineNameMap.end())
	{
		sprintf((char*)pOutput, "[NoPackageName][%s]", packageName);
		return NULL;
	}
	*nTNo = (*iter1).second->tid;

	CFieldNameMap::iterator iter2 = g_FieldNameMap.find(fieldName);
	if(iter2==g_FieldNameMap.end())
	{
		sprintf((char*)pOutput, "[NoFieldName][%s]", fieldName);
		return NULL;
	}
	map<string, json_str> jsonMap = jsonSplit((char *)pInput + 1, nInputLen - 1);
	map<string, json_str>::iterator iter;
	switch((*iter2).second->fid)
	{
	case FID_ServiceConfig: //业务配置
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("ConfigName");
				if(iter != jsonMap.end())
				{
					if(!((CServiceConfigField*)pOutput)->ConfigName.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[ServiceConfig][ConfigName][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CServiceConfigField*)pOutput)->ConfigName.clear();
				}
				iter = jsonMap.find("Index1");
				if(iter != jsonMap.end())
				{
					if(!((CServiceConfigField*)pOutput)->Index1.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[ServiceConfig][Index1][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CServiceConfigField*)pOutput)->Index1.clear();
				}
				iter = jsonMap.find("Index2");
				if(iter != jsonMap.end())
				{
					if(!((CServiceConfigField*)pOutput)->Index2.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[ServiceConfig][Index2][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CServiceConfigField*)pOutput)->Index2.clear();
				}
				iter = jsonMap.find("ConfigValue");
				if(iter != jsonMap.end())
				{
					if(!((CServiceConfigField*)pOutput)->ConfigValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[ServiceConfig][ConfigValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CServiceConfigField*)pOutput)->ConfigValue.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CServiceConfigField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[ServiceConfig][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CServiceConfigField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CServiceConfigField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[ServiceConfig][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CServiceConfigField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CServiceConfigField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[ServiceConfig][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CServiceConfigField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[ServiceConfig][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CServiceConfigField::m_Describe;		
		}
	case FID_Currency: //币种信息
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("SettlementGroup");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][SettlementGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->SettlementGroup.clear();
				}
				iter = jsonMap.find("Currency");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][Currency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->Currency.clear();
				}
				iter = jsonMap.find("Deposit");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->Deposit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][Deposit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->Deposit.clear();
				}
				iter = jsonMap.find("Withdraw");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->Withdraw.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][Withdraw][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->Withdraw.clear();
				}
				iter = jsonMap.find("Balance");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->Balance.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][Balance][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->Balance.clear();
				}
				iter = jsonMap.find("PreBalance");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->PreBalance.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][PreBalance][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->PreBalance.clear();
				}
				iter = jsonMap.find("SortNumber");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->SortNumber.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][SortNumber][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->SortNumber.clear();
				}
				iter = jsonMap.find("CanReduce");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->CanReduce.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][CanReduce][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->CanReduce.clear();
				}
				iter = jsonMap.find("IsTrading");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->IsTrading.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][IsTrading][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->IsTrading.clear();
				}
				iter = jsonMap.find("SettleSegment");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->SettleSegment.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][SettleSegment][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->SettleSegment.clear();
				}
				iter = jsonMap.find("MoneyTick");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->MoneyTick.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][MoneyTick][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->MoneyTick.clear();
				}
				iter = jsonMap.find("ExchangeID");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][ExchangeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->ExchangeID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("LoanUse");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->LoanUse.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][LoanUse][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->LoanUse.clear();
				}
				iter = jsonMap.find("LoanLimit");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->LoanLimit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][LoanLimit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->LoanLimit.clear();
				}
				iter = jsonMap.find("LoanBorrow");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->LoanBorrow.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][LoanBorrow][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->LoanBorrow.clear();
				}
				iter = jsonMap.find("LoanDeposit");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->LoanDeposit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][LoanDeposit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->LoanDeposit.clear();
				}
				iter = jsonMap.find("LoanBase");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->LoanBase.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][LoanBase][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->LoanBase.clear();
				}
				iter = jsonMap.find("LoanRate");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->LoanRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][LoanRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->LoanRate.clear();
				}
				iter = jsonMap.find("LoanHighBase");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->LoanHighBase.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][LoanHighBase][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->LoanHighBase.clear();
				}
				iter = jsonMap.find("LoanHighRate");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->LoanHighRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][LoanHighRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->LoanHighRate.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("CurrValue");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->CurrValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][CurrValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->CurrValue.clear();
				}
				iter = jsonMap.find("HairCut");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->HairCut.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][HairCut][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->HairCut.clear();
				}
				iter = jsonMap.find("MaxValue");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->MaxValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][MaxValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->MaxValue.clear();
				}
				iter = jsonMap.find("MarginRateGroup");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->MarginRateGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][MarginRateGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->MarginRateGroup.clear();
				}
				iter = jsonMap.find("PrivateEnd");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->PrivateEnd.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][PrivateEnd][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->PrivateEnd.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CCurrencyField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Currency][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrencyField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[Currency][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CCurrencyField::m_Describe;		
		}
	case FID_MarketData: //市场行情
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("ExchangeID");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][ExchangeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->ExchangeID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("ProductGroup");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][ProductGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->ProductGroup.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->UpdateTime.clear();
				}
				iter = jsonMap.find("PreClosePrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->PreClosePrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][PreClosePrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->PreClosePrice.clear();
				}
				iter = jsonMap.find("PreSettlementPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->PreSettlementPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][PreSettlementPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->PreSettlementPrice.clear();
				}
				iter = jsonMap.find("PreOpenInterest");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->PreOpenInterest.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][PreOpenInterest][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->PreOpenInterest.clear();
				}
				iter = jsonMap.find("PrePositionFeeRate");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->PrePositionFeeRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][PrePositionFeeRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->PrePositionFeeRate.clear();
				}
				iter = jsonMap.find("ClosePrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->ClosePrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][ClosePrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->ClosePrice.clear();
				}
				iter = jsonMap.find("PositionFeeTime");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->PositionFeeTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][PositionFeeTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->PositionFeeTime.clear();
				}
				iter = jsonMap.find("SettlementTime");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->SettlementTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][SettlementTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->SettlementTime.clear();
				}
				iter = jsonMap.find("UpperLimitPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->UpperLimitPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][UpperLimitPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->UpperLimitPrice.clear();
				}
				iter = jsonMap.find("LowerLimitPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->LowerLimitPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][LowerLimitPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->LowerLimitPrice.clear();
				}
				iter = jsonMap.find("UnderlyingPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->UnderlyingPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][UnderlyingPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->UnderlyingPrice.clear();
				}
				iter = jsonMap.find("MarkedPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->MarkedPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][MarkedPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->MarkedPrice.clear();
				}
				iter = jsonMap.find("PositionFeeRate");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->PositionFeeRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][PositionFeeRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->PositionFeeRate.clear();
				}
				iter = jsonMap.find("HighestPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->HighestPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][HighestPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->HighestPrice.clear();
				}
				iter = jsonMap.find("LowestPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->LowestPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][LowestPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->LowestPrice.clear();
				}
				iter = jsonMap.find("LastPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->LastPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][LastPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->LastPrice.clear();
				}
				iter = jsonMap.find("Volume");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][Volume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->Volume.clear();
				}
				iter = jsonMap.find("Turnover");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->Turnover.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][Turnover][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->Turnover.clear();
				}
				iter = jsonMap.find("OpenInterest");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->OpenInterest.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][OpenInterest][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->OpenInterest.clear();
				}
				iter = jsonMap.find("SettlementPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->SettlementPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][SettlementPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->SettlementPrice.clear();
				}
				iter = jsonMap.find("OpenPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->OpenPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][OpenPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->OpenPrice.clear();
				}
				iter = jsonMap.find("IsPubMarketData");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->IsPubMarketData.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][IsPubMarketData][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->IsPubMarketData.clear();
				}
				iter = jsonMap.find("InstrumentStatus");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->InstrumentStatus.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][InstrumentStatus][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->InstrumentStatus.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("BusinessNo");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->BusinessNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][BusinessNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->BusinessNo.clear();
				}
				iter = jsonMap.find("MarkedPriceTotal");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->MarkedPriceTotal.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][MarkedPriceTotal][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->MarkedPriceTotal.clear();
				}
				iter = jsonMap.find("MarkedPriceWeight");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->MarkedPriceWeight.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][MarkedPriceWeight][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->MarkedPriceWeight.clear();
				}
				iter = jsonMap.find("L25UpperPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->L25UpperPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][L25UpperPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->L25UpperPrice.clear();
				}
				iter = jsonMap.find("L25LowerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->L25LowerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][L25LowerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->L25LowerPrice.clear();
				}
				iter = jsonMap.find("L25UpperPrice1");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->L25UpperPrice1.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][L25UpperPrice1][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->L25UpperPrice1.clear();
				}
				iter = jsonMap.find("L25LowerPrice1");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->L25LowerPrice1.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][L25LowerPrice1][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->L25LowerPrice1.clear();
				}
				iter = jsonMap.find("L25UpperPrice2");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->L25UpperPrice2.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][L25UpperPrice2][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->L25UpperPrice2.clear();
				}
				iter = jsonMap.find("L25LowerPrice2");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->L25LowerPrice2.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][L25LowerPrice2][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->L25LowerPrice2.clear();
				}
				iter = jsonMap.find("BidPrice1");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->BidPrice1.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][BidPrice1][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->BidPrice1.clear();
				}
				iter = jsonMap.find("BidVolume1");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->BidVolume1.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][BidVolume1][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->BidVolume1.clear();
				}
				iter = jsonMap.find("AskPrice1");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->AskPrice1.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][AskPrice1][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->AskPrice1.clear();
				}
				iter = jsonMap.find("AskVolume1");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->AskVolume1.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][AskVolume1][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->AskVolume1.clear();
				}
				iter = jsonMap.find("TheoryPrice");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->TheoryPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][TheoryPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->TheoryPrice.clear();
				}
				iter = jsonMap.find("HighestPrice24");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->HighestPrice24.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][HighestPrice24][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->HighestPrice24.clear();
				}
				iter = jsonMap.find("LowestPrice24");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->LowestPrice24.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][LowestPrice24][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->LowestPrice24.clear();
				}
				iter = jsonMap.find("Volume24");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->Volume24.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][Volume24][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->Volume24.clear();
				}
				iter = jsonMap.find("Turnover24");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->Turnover24.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][Turnover24][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->Turnover24.clear();
				}
				iter = jsonMap.find("OpenPrice24");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->OpenPrice24.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][OpenPrice24][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->OpenPrice24.clear();
				}
				iter = jsonMap.find("Frequency");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->Frequency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][Frequency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->Frequency.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CMarketDataField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarketData][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarketDataField*)pOutput)->InsertTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[MarketData][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CMarketDataField::m_Describe;		
		}
	case FID_Instrument: //交易标的
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("ExchangeID");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][ExchangeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->ExchangeID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("InstrumentName");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->InstrumentName.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][InstrumentName][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->InstrumentName.clear();
				}
				iter = jsonMap.find("SettlementGroup");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][SettlementGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->SettlementGroup.clear();
				}
				iter = jsonMap.find("ProductGroup");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->ProductGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][ProductGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->ProductGroup.clear();
				}
				iter = jsonMap.find("UnderlyingID");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->UnderlyingID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][UnderlyingID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->UnderlyingID.clear();
				}
				iter = jsonMap.find("ProductClass");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->ProductClass.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][ProductClass][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->ProductClass.clear();
				}
				iter = jsonMap.find("ProductType");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->ProductType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][ProductType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->ProductType.clear();
				}
				iter = jsonMap.find("StrikePrice");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->StrikePrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][StrikePrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->StrikePrice.clear();
				}
				iter = jsonMap.find("OptionsType");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->OptionsType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][OptionsType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->OptionsType.clear();
				}
				iter = jsonMap.find("VolumeMultiple");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->VolumeMultiple.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][VolumeMultiple][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->VolumeMultiple.clear();
				}
				iter = jsonMap.find("StartTime");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->StartTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][StartTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->StartTime.clear();
				}
				iter = jsonMap.find("ExpireTime");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->ExpireTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][ExpireTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->ExpireTime.clear();
				}
				iter = jsonMap.find("PriceCurrency");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->PriceCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][PriceCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->PriceCurrency.clear();
				}
				iter = jsonMap.find("ClearCurrency");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->ClearCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][ClearCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->ClearCurrency.clear();
				}
				iter = jsonMap.find("IsInverse");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->IsInverse.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][IsInverse][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->IsInverse.clear();
				}
				iter = jsonMap.find("BaseCurrency");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->BaseCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][BaseCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->BaseCurrency.clear();
				}
				iter = jsonMap.find("MarginPriceType");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->MarginPriceType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][MarginPriceType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->MarginPriceType.clear();
				}
				iter = jsonMap.find("BasisPrice");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->BasisPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][BasisPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->BasisPrice.clear();
				}
				iter = jsonMap.find("MinOrderVolume");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->MinOrderVolume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][MinOrderVolume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->MinOrderVolume.clear();
				}
				iter = jsonMap.find("MinOrderCost");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->MinOrderCost.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][MinOrderCost][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->MinOrderCost.clear();
				}
				iter = jsonMap.find("MaxOrderVolume");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->MaxOrderVolume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][MaxOrderVolume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->MaxOrderVolume.clear();
				}
				iter = jsonMap.find("PriceTick");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->PriceTick.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][PriceTick][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->PriceTick.clear();
				}
				iter = jsonMap.find("VolumeTick");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->VolumeTick.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][VolumeTick][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->VolumeTick.clear();
				}
				iter = jsonMap.find("ShowVolumeTick");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->ShowVolumeTick.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][ShowVolumeTick][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->ShowVolumeTick.clear();
				}
				iter = jsonMap.find("TradingModel");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->TradingModel.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][TradingModel][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->TradingModel.clear();
				}
				iter = jsonMap.find("DayStartTime");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->DayStartTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][DayStartTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->DayStartTime.clear();
				}
				iter = jsonMap.find("SortNumber");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->SortNumber.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][SortNumber][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->SortNumber.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("DefaultLeverage");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->DefaultLeverage.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][DefaultLeverage][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->DefaultLeverage.clear();
				}
				iter = jsonMap.find("PriceLimitValueMode");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->PriceLimitValueMode.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][PriceLimitValueMode][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->PriceLimitValueMode.clear();
				}
				iter = jsonMap.find("PriceLimitPrice");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->PriceLimitPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][PriceLimitPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->PriceLimitPrice.clear();
				}
				iter = jsonMap.find("PriceLimitUpperValue");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->PriceLimitUpperValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][PriceLimitUpperValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->PriceLimitUpperValue.clear();
				}
				iter = jsonMap.find("PriceLimitLowerValue");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->PriceLimitLowerValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][PriceLimitLowerValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->PriceLimitLowerValue.clear();
				}
				iter = jsonMap.find("MarginRateGroup");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->MarginRateGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][MarginRateGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->MarginRateGroup.clear();
				}
				iter = jsonMap.find("CFDGroup");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->CFDGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][CFDGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->CFDGroup.clear();
				}
				iter = jsonMap.find("FeeGroup");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->FeeGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][FeeGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->FeeGroup.clear();
				}
				iter = jsonMap.find("TradingRightGroup");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->TradingRightGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][TradingRightGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->TradingRightGroup.clear();
				}
				iter = jsonMap.find("MaxOpenInterest");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->MaxOpenInterest.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][MaxOpenInterest][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->MaxOpenInterest.clear();
				}
				iter = jsonMap.find("FundingRateGroup");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->FundingRateGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][FundingRateGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->FundingRateGroup.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->UpdateTime.clear();
				}
				iter = jsonMap.find("MarkedPriceTimes");
				if(iter != jsonMap.end())
				{
					if(!((CInstrumentField*)pOutput)->MarkedPriceTimes.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Instrument][MarkedPriceTimes][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CInstrumentField*)pOutput)->MarkedPriceTimes.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[Instrument][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CInstrumentField::m_Describe;		
		}
	case FID_CurrentTime: //当前时间
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("SystemID");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->SystemID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][SystemID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->SystemID.clear();
				}
				iter = jsonMap.find("ZoneDate");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->ZoneDate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][ZoneDate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->ZoneDate.clear();
				}
				iter = jsonMap.find("ZoneTime");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->ZoneTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][ZoneTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->ZoneTime.clear();
				}
				iter = jsonMap.find("ZoneDays");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->ZoneDays.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][ZoneDays][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->ZoneDays.clear();
				}
				iter = jsonMap.find("CurrTime");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->CurrTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][CurrTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->CurrTime.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->UpdateTime.clear();
				}
				iter = jsonMap.find("TimeZone");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->TimeZone.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][TimeZone][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->TimeZone.clear();
				}
				iter = jsonMap.find("LastTradeID");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->LastTradeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][LastTradeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->LastTradeID.clear();
				}
				iter = jsonMap.find("LastOrderID");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->LastOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][LastOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->LastOrderID.clear();
				}
				iter = jsonMap.find("LastAccountDetailID");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->LastAccountDetailID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][LastAccountDetailID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->LastAccountDetailID.clear();
				}
				iter = jsonMap.find("LastTimeSortNo");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->LastTimeSortNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][LastTimeSortNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->LastTimeSortNo.clear();
				}
				iter = jsonMap.find("LastBusinessNo");
				if(iter != jsonMap.end())
				{
					if(!((CCurrentTimeField*)pOutput)->LastBusinessNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CurrentTime][LastBusinessNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCurrentTimeField*)pOutput)->LastBusinessNo.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[CurrentTime][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CCurrentTimeField::m_Describe;		
		}
	case FID_Member: //成员
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("MemberType");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->MemberType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][MemberType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->MemberType.clear();
				}
				iter = jsonMap.find("Grade");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->Grade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][Grade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->Grade.clear();
				}
				iter = jsonMap.find("Priority");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->Priority.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][Priority][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->Priority.clear();
				}
				iter = jsonMap.find("FeeGrade");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->FeeGrade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][FeeGrade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->FeeGrade.clear();
				}
				iter = jsonMap.find("MarginRateGrade");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->MarginRateGrade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][MarginRateGrade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->MarginRateGrade.clear();
				}
				iter = jsonMap.find("TradingRightGrade");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->TradingRightGrade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][TradingRightGrade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->TradingRightGrade.clear();
				}
				iter = jsonMap.find("CFDGrade");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->CFDGrade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][CFDGrade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->CFDGrade.clear();
				}
				iter = jsonMap.find("LiquidityGrade");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->LiquidityGrade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][LiquidityGrade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->LiquidityGrade.clear();
				}
				iter = jsonMap.find("RiskLevel");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->RiskLevel.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][RiskLevel][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->RiskLevel.clear();
				}
				iter = jsonMap.find("PositionType");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->PositionType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][PositionType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->PositionType.clear();
				}
				iter = jsonMap.find("TriggerOrders");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->TriggerOrders.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][TriggerOrders][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->TriggerOrders.clear();
				}
				iter = jsonMap.find("OpenOrders");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->OpenOrders.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][OpenOrders][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->OpenOrders.clear();
				}
				iter = jsonMap.find("OpenPositions");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->OpenPositions.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][OpenPositions][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->OpenPositions.clear();
				}
				iter = jsonMap.find("MaxLocalID");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->MaxLocalID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][MaxLocalID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->MaxLocalID.clear();
				}
				iter = jsonMap.find("SinkType");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->SinkType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][SinkType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->SinkType.clear();
				}
				iter = jsonMap.find("CopyMemberID");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->CopyMemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][CopyMemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->CopyMemberID.clear();
				}
				iter = jsonMap.find("Region");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->Region.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][Region][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->Region.clear();
				}
				iter = jsonMap.find("FeeDeduceDisCount");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->FeeDeduceDisCount.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][FeeDeduceDisCount][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->FeeDeduceDisCount.clear();
				}
				iter = jsonMap.find("FeeDeduceCurrency");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->FeeDeduceCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][FeeDeduceCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->FeeDeduceCurrency.clear();
				}
				iter = jsonMap.find("CanBorrow");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->CanBorrow.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][CanBorrow][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->CanBorrow.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CMemberField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Member][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[Member][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CMemberField::m_Describe;		
		}
	case FID_UserSession: //用户会话
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("UserID");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][UserID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->UserID.clear();
				}
				iter = jsonMap.find("Password");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->Password.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][Password][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->Password.clear();
				}
				iter = jsonMap.find("UserProductID");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->UserProductID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][UserProductID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->UserProductID.clear();
				}
				iter = jsonMap.find("MacAddress");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->MacAddress.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][MacAddress][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->MacAddress.clear();
				}
				iter = jsonMap.find("ClientIPAddress");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->ClientIPAddress.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][ClientIPAddress][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->ClientIPAddress.clear();
				}
				iter = jsonMap.find("HDSerialID");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->HDSerialID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][HDSerialID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->HDSerialID.clear();
				}
				iter = jsonMap.find("AuthCode");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->AuthCode.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][AuthCode][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->AuthCode.clear();
				}
				iter = jsonMap.find("APPID");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][APPID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->APPID.clear();
				}
				iter = jsonMap.find("Token");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->Token.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][Token][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->Token.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("APIID");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->APIID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][APIID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->APIID.clear();
				}
				iter = jsonMap.find("LoginTime");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->LoginTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][LoginTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->LoginTime.clear();
				}
				iter = jsonMap.find("ExpireTime");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->ExpireTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][ExpireTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->ExpireTime.clear();
				}
				iter = jsonMap.find("IPAddress");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->IPAddress.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][IPAddress][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->IPAddress.clear();
				}
				iter = jsonMap.find("MaxLocalID");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->MaxLocalID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][MaxLocalID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->MaxLocalID.clear();
				}
				iter = jsonMap.find("SessionNo");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->SessionNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][SessionNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->SessionNo.clear();
				}
				iter = jsonMap.find("FrontNo");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->FrontNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][FrontNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->FrontNo.clear();
				}
				iter = jsonMap.find("AccessLimit");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->AccessLimit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][AccessLimit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->AccessLimit.clear();
				}
				iter = jsonMap.find("UserType");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->UserType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][UserType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->UserType.clear();
				}
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("LimitAccesses");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->LimitAccesses.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][LimitAccesses][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->LimitAccesses.clear();
				}
				iter = jsonMap.find("SinkType");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->SinkType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][SinkType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->SinkType.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CUserSessionField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[UserSession][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CUserSessionField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[UserSession][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CUserSessionField::m_Describe;		
		}
	case FID_LastKLine: //最新K线
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("ExchangeID");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][ExchangeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->ExchangeID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("Bar");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->Bar.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][Bar][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->Bar.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("OpenPrice");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->OpenPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][OpenPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->OpenPrice.clear();
				}
				iter = jsonMap.find("HighestPrice");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->HighestPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][HighestPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->HighestPrice.clear();
				}
				iter = jsonMap.find("LowestPrice");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->LowestPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][LowestPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->LowestPrice.clear();
				}
				iter = jsonMap.find("ClosePrice");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->ClosePrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][ClosePrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->ClosePrice.clear();
				}
				iter = jsonMap.find("Volume");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][Volume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->Volume.clear();
				}
				iter = jsonMap.find("Turnover");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->Turnover.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][Turnover][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->Turnover.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CLastKLineField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[LastKLine][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLastKLineField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[LastKLine][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CLastKLineField::m_Describe;		
		}
	case FID_CFDRate: //CFD溢价率
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("TriggerOrderType");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->TriggerOrderType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][TriggerOrderType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->TriggerOrderType.clear();
				}
				iter = jsonMap.find("RateBegin");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->RateBegin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][RateBegin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->RateBegin.clear();
				}
				iter = jsonMap.find("RateEnd");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->RateEnd.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][RateEnd][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->RateEnd.clear();
				}
				iter = jsonMap.find("CompVolume");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->CompVolume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][CompVolume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->CompVolume.clear();
				}
				iter = jsonMap.find("CompRate");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->CompRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][CompRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->CompRate.clear();
				}
				iter = jsonMap.find("HardRate");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->HardRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][HardRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->HardRate.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CCFDRateField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[CFDRate][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CCFDRateField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[CFDRate][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CCFDRateField::m_Describe;		
		}
	case FID_TradingRight: //交易权限
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CTradingRightField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TradingRight][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTradingRightField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CTradingRightField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TradingRight][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTradingRightField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("Direction");
				if(iter != jsonMap.end())
				{
					if(!((CTradingRightField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TradingRight][Direction][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTradingRightField*)pOutput)->Direction.clear();
				}
				iter = jsonMap.find("TradingRight");
				if(iter != jsonMap.end())
				{
					if(!((CTradingRightField*)pOutput)->TradingRight.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TradingRight][TradingRight][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTradingRightField*)pOutput)->TradingRight.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CTradingRightField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TradingRight][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTradingRightField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CTradingRightField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TradingRight][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTradingRightField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CTradingRightField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TradingRight][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTradingRightField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[TradingRight][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CTradingRightField::m_Describe;		
		}
	case FID_MarginRate: //保证金率
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("ValueMode");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->ValueMode.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][ValueMode][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->ValueMode.clear();
				}
				iter = jsonMap.find("MarginSequenceType");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->MarginSequenceType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][MarginSequenceType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->MarginSequenceType.clear();
				}
				iter = jsonMap.find("RiskSequenceType");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->RiskSequenceType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][RiskSequenceType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->RiskSequenceType.clear();
				}
				iter = jsonMap.find("LongMarginRatio");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->LongMarginRatio.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][LongMarginRatio][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->LongMarginRatio.clear();
				}
				iter = jsonMap.find("LongMaintMarginRatio");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->LongMaintMarginRatio.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][LongMaintMarginRatio][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->LongMaintMarginRatio.clear();
				}
				iter = jsonMap.find("LongMarginStep");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->LongMarginStep.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][LongMarginStep][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->LongMarginStep.clear();
				}
				iter = jsonMap.find("LongMaintMarginStep");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->LongMaintMarginStep.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][LongMaintMarginStep][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->LongMaintMarginStep.clear();
				}
				iter = jsonMap.find("LongRiskLimit");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->LongRiskLimit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][LongRiskLimit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->LongRiskLimit.clear();
				}
				iter = jsonMap.find("LongRiskStep");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->LongRiskStep.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][LongRiskStep][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->LongRiskStep.clear();
				}
				iter = jsonMap.find("ShortMarginRatio");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->ShortMarginRatio.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][ShortMarginRatio][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->ShortMarginRatio.clear();
				}
				iter = jsonMap.find("ShortMaintMarginRatio");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->ShortMaintMarginRatio.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][ShortMaintMarginRatio][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->ShortMaintMarginRatio.clear();
				}
				iter = jsonMap.find("ShortMarginStep");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->ShortMarginStep.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][ShortMarginStep][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->ShortMarginStep.clear();
				}
				iter = jsonMap.find("ShortMaintMarginStep");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->ShortMaintMarginStep.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][ShortMaintMarginStep][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->ShortMaintMarginStep.clear();
				}
				iter = jsonMap.find("ShortRiskLimit");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->ShortRiskLimit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][ShortRiskLimit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->ShortRiskLimit.clear();
				}
				iter = jsonMap.find("ShortRiskStep");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->ShortRiskStep.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][ShortRiskStep][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->ShortRiskStep.clear();
				}
				iter = jsonMap.find("DefaultLeverage");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->DefaultLeverage.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][DefaultLeverage][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->DefaultLeverage.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CMarginRateField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MarginRate][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMarginRateField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[MarginRate][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CMarginRateField::m_Describe;		
		}
	case FID_Fee: //手续费率
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("MatchRole");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->MatchRole.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][MatchRole][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->MatchRole.clear();
				}
				iter = jsonMap.find("OpenFeeRate");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->OpenFeeRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][OpenFeeRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->OpenFeeRate.clear();
				}
				iter = jsonMap.find("OpenFeeAmount");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->OpenFeeAmount.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][OpenFeeAmount][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->OpenFeeAmount.clear();
				}
				iter = jsonMap.find("CloseFeeRate");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->CloseFeeRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][CloseFeeRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->CloseFeeRate.clear();
				}
				iter = jsonMap.find("CloseFeeAmount");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->CloseFeeAmount.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][CloseFeeAmount][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->CloseFeeAmount.clear();
				}
				iter = jsonMap.find("CloseTodayFeeRate");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->CloseTodayFeeRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][CloseTodayFeeRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->CloseTodayFeeRate.clear();
				}
				iter = jsonMap.find("CloseTodayFeeAmount");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->CloseTodayFeeAmount.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][CloseTodayFeeAmount][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->CloseTodayFeeAmount.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CFeeField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Fee][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CFeeField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[Fee][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CFeeField::m_Describe;		
		}
	case FID_Liquidity: //流动性设置表
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("LiquidityGrade");
				if(iter != jsonMap.end())
				{
					if(!((CLiquidityField*)pOutput)->LiquidityGrade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Liquidity][LiquidityGrade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLiquidityField*)pOutput)->LiquidityGrade.clear();
				}
				iter = jsonMap.find("LiquidityGroup");
				if(iter != jsonMap.end())
				{
					if(!((CLiquidityField*)pOutput)->LiquidityGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Liquidity][LiquidityGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLiquidityField*)pOutput)->LiquidityGroup.clear();
				}
				iter = jsonMap.find("BasePriceType");
				if(iter != jsonMap.end())
				{
					if(!((CLiquidityField*)pOutput)->BasePriceType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Liquidity][BasePriceType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLiquidityField*)pOutput)->BasePriceType.clear();
				}
				iter = jsonMap.find("VolumeBegin");
				if(iter != jsonMap.end())
				{
					if(!((CLiquidityField*)pOutput)->VolumeBegin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Liquidity][VolumeBegin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLiquidityField*)pOutput)->VolumeBegin.clear();
				}
				iter = jsonMap.find("BuyOpen");
				if(iter != jsonMap.end())
				{
					if(!((CLiquidityField*)pOutput)->BuyOpen.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Liquidity][BuyOpen][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLiquidityField*)pOutput)->BuyOpen.clear();
				}
				iter = jsonMap.find("BuyClose");
				if(iter != jsonMap.end())
				{
					if(!((CLiquidityField*)pOutput)->BuyClose.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Liquidity][BuyClose][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLiquidityField*)pOutput)->BuyClose.clear();
				}
				iter = jsonMap.find("SellOpen");
				if(iter != jsonMap.end())
				{
					if(!((CLiquidityField*)pOutput)->SellOpen.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Liquidity][SellOpen][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLiquidityField*)pOutput)->SellOpen.clear();
				}
				iter = jsonMap.find("SellClose");
				if(iter != jsonMap.end())
				{
					if(!((CLiquidityField*)pOutput)->SellClose.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Liquidity][SellClose][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLiquidityField*)pOutput)->SellClose.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CLiquidityField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Liquidity][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLiquidityField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CLiquidityField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Liquidity][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CLiquidityField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[Liquidity][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CLiquidityField::m_Describe;		
		}
	case FID_Account: //账户资金
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("SettlementGroup");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][SettlementGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->SettlementGroup.clear();
				}
				iter = jsonMap.find("AccountID");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][AccountID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->AccountID.clear();
				}
				iter = jsonMap.find("Currency");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Currency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Currency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Currency.clear();
				}
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("Available");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Available.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Available][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Available.clear();
				}
				iter = jsonMap.find("Withdrawable");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Withdrawable.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Withdrawable][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Withdrawable.clear();
				}
				iter = jsonMap.find("PreBalance");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->PreBalance.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][PreBalance][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->PreBalance.clear();
				}
				iter = jsonMap.find("Balance");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Balance.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Balance][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Balance.clear();
				}
				iter = jsonMap.find("PositionCost");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->PositionCost.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][PositionCost][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->PositionCost.clear();
				}
				iter = jsonMap.find("UseMargin");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->UseMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][UseMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->UseMargin.clear();
				}
				iter = jsonMap.find("CrossMargin");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->CrossMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][CrossMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->CrossMargin.clear();
				}
				iter = jsonMap.find("CloseProfit");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->CloseProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][CloseProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->CloseProfit.clear();
				}
				iter = jsonMap.find("Deposit");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Deposit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Deposit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Deposit.clear();
				}
				iter = jsonMap.find("Withdraw");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Withdraw.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Withdraw][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Withdraw.clear();
				}
				iter = jsonMap.find("FrozenMargin");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->FrozenMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][FrozenMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->FrozenMargin.clear();
				}
				iter = jsonMap.find("FrozenMoney");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->FrozenMoney.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][FrozenMoney][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->FrozenMoney.clear();
				}
				iter = jsonMap.find("FrozenFee");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->FrozenFee.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][FrozenFee][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->FrozenFee.clear();
				}
				iter = jsonMap.find("Fee");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Fee.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Fee][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Fee.clear();
				}
				iter = jsonMap.find("MoneyChange");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->MoneyChange.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][MoneyChange][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->MoneyChange.clear();
				}
				iter = jsonMap.find("Reserve");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Reserve.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Reserve][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Reserve.clear();
				}
				iter = jsonMap.find("ReserveAvailable");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->ReserveAvailable.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][ReserveAvailable][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->ReserveAvailable.clear();
				}
				iter = jsonMap.find("ReserveLimit");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->ReserveLimit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][ReserveLimit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->ReserveLimit.clear();
				}
				iter = jsonMap.find("ReserveProfit");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->ReserveProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][ReserveProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->ReserveProfit.clear();
				}
				iter = jsonMap.find("ReserveType");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->ReserveType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][ReserveType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->ReserveType.clear();
				}
				iter = jsonMap.find("ReserveActive");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->ReserveActive.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][ReserveActive][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->ReserveActive.clear();
				}
				iter = jsonMap.find("TotalCloseProfit");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->TotalCloseProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][TotalCloseProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->TotalCloseProfit.clear();
				}
				iter = jsonMap.find("TotalDeposit");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->TotalDeposit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][TotalDeposit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->TotalDeposit.clear();
				}
				iter = jsonMap.find("TotalWithdraw");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->TotalWithdraw.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][TotalWithdraw][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->TotalWithdraw.clear();
				}
				iter = jsonMap.find("TotalFee");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->TotalFee.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][TotalFee][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->TotalFee.clear();
				}
				iter = jsonMap.find("TotalMoneyChange");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->TotalMoneyChange.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][TotalMoneyChange][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->TotalMoneyChange.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->UpdateTime.clear();
				}
				iter = jsonMap.find("AccountType");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->AccountType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][AccountType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->AccountType.clear();
				}
				iter = jsonMap.find("BusinessNo");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->BusinessNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][BusinessNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->BusinessNo.clear();
				}
				iter = jsonMap.find("MarginRateGrade");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->MarginRateGrade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][MarginRateGrade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->MarginRateGrade.clear();
				}
				iter = jsonMap.find("TradingRightGrade");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->TradingRightGrade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][TradingRightGrade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->TradingRightGrade.clear();
				}
				iter = jsonMap.find("Frequency");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Frequency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Frequency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Frequency.clear();
				}
				iter = jsonMap.find("MaintMargin");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->MaintMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][MaintMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->MaintMargin.clear();
				}
				iter = jsonMap.find("UnRealProfit");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->UnRealProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][UnRealProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->UnRealProfit.clear();
				}
				iter = jsonMap.find("CopyProfit");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->CopyProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][CopyProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->CopyProfit.clear();
				}
				iter = jsonMap.find("CFDType");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->CFDType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][CFDType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->CFDType.clear();
				}
				iter = jsonMap.find("FeeGrade");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->FeeGrade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][FeeGrade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->FeeGrade.clear();
				}
				iter = jsonMap.find("CFDGrade");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->CFDGrade.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][CFDGrade][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->CFDGrade.clear();
				}
				iter = jsonMap.find("Borrow");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Borrow.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Borrow][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Borrow.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("MaxLocalID");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->MaxLocalID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][MaxLocalID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->MaxLocalID.clear();
				}
				iter = jsonMap.find("CurrValue");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->CurrValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][CurrValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->CurrValue.clear();
				}
				iter = jsonMap.find("CanReduce");
				if(iter != jsonMap.end())
				{
					if(!((CAccountField*)pOutput)->CanReduce.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Account][CanReduce][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CAccountField*)pOutput)->CanReduce.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[Account][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CAccountField::m_Describe;		
		}
	case FID_MemberPosition: //成员持仓汇总
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("AccountID");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][AccountID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->AccountID.clear();
				}
				iter = jsonMap.find("ExchangeID");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][ExchangeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->ExchangeID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("Position");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->Position.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][Position][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->Position.clear();
				}
				iter = jsonMap.find("LongFrozen");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->LongFrozen.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][LongFrozen][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->LongFrozen.clear();
				}
				iter = jsonMap.find("ShortFrozen");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->ShortFrozen.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][ShortFrozen][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->ShortFrozen.clear();
				}
				iter = jsonMap.find("DefaultLeverage");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->DefaultLeverage.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][DefaultLeverage][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->DefaultLeverage.clear();
				}
				iter = jsonMap.find("DefaultCrossMargin");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->DefaultCrossMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][DefaultCrossMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->DefaultCrossMargin.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CMemberPositionField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[MemberPosition][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CMemberPositionField*)pOutput)->UpdateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[MemberPosition][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CMemberPositionField::m_Describe;		
		}
	case FID_Position: //标的持仓
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("PositionID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][PositionID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->PositionID.clear();
				}
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("TradeUnitID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][TradeUnitID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->TradeUnitID.clear();
				}
				iter = jsonMap.find("ExchangeID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][ExchangeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->ExchangeID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("PositionClass");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->PositionClass.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][PositionClass][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->PositionClass.clear();
				}
				iter = jsonMap.find("PosiDirection");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->PosiDirection.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][PosiDirection][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->PosiDirection.clear();
				}
				iter = jsonMap.find("PrePosition");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->PrePosition.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][PrePosition][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->PrePosition.clear();
				}
				iter = jsonMap.find("Position");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->Position.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][Position][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->Position.clear();
				}
				iter = jsonMap.find("LongFrozen");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->LongFrozen.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][LongFrozen][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->LongFrozen.clear();
				}
				iter = jsonMap.find("ShortFrozen");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->ShortFrozen.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][ShortFrozen][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->ShortFrozen.clear();
				}
				iter = jsonMap.find("PreLongFrozen");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->PreLongFrozen.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][PreLongFrozen][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->PreLongFrozen.clear();
				}
				iter = jsonMap.find("PreShortFrozen");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->PreShortFrozen.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][PreShortFrozen][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->PreShortFrozen.clear();
				}
				iter = jsonMap.find("HighestPosition");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->HighestPosition.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][HighestPosition][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->HighestPosition.clear();
				}
				iter = jsonMap.find("ClosePosition");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->ClosePosition.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][ClosePosition][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->ClosePosition.clear();
				}
				iter = jsonMap.find("PositionCost");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->PositionCost.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][PositionCost][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->PositionCost.clear();
				}
				iter = jsonMap.find("CostPrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->CostPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][CostPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->CostPrice.clear();
				}
				iter = jsonMap.find("UseMargin");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->UseMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][UseMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->UseMargin.clear();
				}
				iter = jsonMap.find("FrozenMargin");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->FrozenMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][FrozenMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->FrozenMargin.clear();
				}
				iter = jsonMap.find("LongFrozenMargin");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->LongFrozenMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][LongFrozenMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->LongFrozenMargin.clear();
				}
				iter = jsonMap.find("ShortFrozenMargin");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->ShortFrozenMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][ShortFrozenMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->ShortFrozenMargin.clear();
				}
				iter = jsonMap.find("CloseProfit");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->CloseProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][CloseProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->CloseProfit.clear();
				}
				iter = jsonMap.find("TotalPositionCost");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->TotalPositionCost.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][TotalPositionCost][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->TotalPositionCost.clear();
				}
				iter = jsonMap.find("TotalCloseProfit");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->TotalCloseProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][TotalCloseProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->TotalCloseProfit.clear();
				}
				iter = jsonMap.find("OpenPrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->OpenPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][OpenPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->OpenPrice.clear();
				}
				iter = jsonMap.find("ClosePrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->ClosePrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][ClosePrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->ClosePrice.clear();
				}
				iter = jsonMap.find("TradeFee");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->TradeFee.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][TradeFee][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->TradeFee.clear();
				}
				iter = jsonMap.find("PositionFee");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->PositionFee.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][PositionFee][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->PositionFee.clear();
				}
				iter = jsonMap.find("Leverage");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->Leverage.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][Leverage][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->Leverage.clear();
				}
				iter = jsonMap.find("AccountID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][AccountID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->AccountID.clear();
				}
				iter = jsonMap.find("PriceCurrency");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->PriceCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][PriceCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->PriceCurrency.clear();
				}
				iter = jsonMap.find("ClearCurrency");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->ClearCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][ClearCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->ClearCurrency.clear();
				}
				iter = jsonMap.find("SettlementGroup");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][SettlementGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->SettlementGroup.clear();
				}
				iter = jsonMap.find("IsCrossMargin");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->IsCrossMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][IsCrossMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->IsCrossMargin.clear();
				}
				iter = jsonMap.find("CloseOrderID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->CloseOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][CloseOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->CloseOrderID.clear();
				}
				iter = jsonMap.find("SLTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->SLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][SLTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->SLTriggerPrice.clear();
				}
				iter = jsonMap.find("TPTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->TPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][TPTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->TPTriggerPrice.clear();
				}
				iter = jsonMap.find("BeginTime");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->BeginTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][BeginTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->BeginTime.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("LastOpenTime");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->LastOpenTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][LastOpenTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->LastOpenTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->UpdateTime.clear();
				}
				iter = jsonMap.find("BusinessNo");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->BusinessNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][BusinessNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->BusinessNo.clear();
				}
				iter = jsonMap.find("IsAutoAddMargin");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->IsAutoAddMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][IsAutoAddMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->IsAutoAddMargin.clear();
				}
				iter = jsonMap.find("Frequency");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->Frequency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][Frequency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->Frequency.clear();
				}
				iter = jsonMap.find("MaintMargin");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->MaintMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][MaintMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->MaintMargin.clear();
				}
				iter = jsonMap.find("UnRealProfit");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->UnRealProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][UnRealProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->UnRealProfit.clear();
				}
				iter = jsonMap.find("LiquidPrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->LiquidPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][LiquidPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->LiquidPrice.clear();
				}
				iter = jsonMap.find("CreateTime");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->CreateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][CreateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->CreateTime.clear();
				}
				iter = jsonMap.find("CopyMemberID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->CopyMemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][CopyMemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->CopyMemberID.clear();
				}
				iter = jsonMap.find("CopyProfitRate");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->CopyProfitRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][CopyProfitRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->CopyProfitRate.clear();
				}
				iter = jsonMap.find("CopyProfit");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->CopyProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][CopyProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->CopyProfit.clear();
				}
				iter = jsonMap.find("FirstTradeID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->FirstTradeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][FirstTradeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->FirstTradeID.clear();
				}
				iter = jsonMap.find("LastTradeID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->LastTradeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][LastTradeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->LastTradeID.clear();
				}
				iter = jsonMap.find("BusinessType");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->BusinessType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][BusinessType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->BusinessType.clear();
				}
				iter = jsonMap.find("BusinessValue");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->BusinessValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][BusinessValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->BusinessValue.clear();
				}
				iter = jsonMap.find("Reserve");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->Reserve.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][Reserve][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->Reserve.clear();
				}
				iter = jsonMap.find("ReserveProfit");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->ReserveProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][ReserveProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->ReserveProfit.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CPositionField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Position][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionField*)pOutput)->Remark.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[Position][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CPositionField::m_Describe;		
		}
	case FID_Order: //报单
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("TradeUnitID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][TradeUnitID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->TradeUnitID.clear();
				}
				iter = jsonMap.find("AccountID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][AccountID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->AccountID.clear();
				}
				iter = jsonMap.find("LocalID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][LocalID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->LocalID.clear();
				}
				iter = jsonMap.find("ExchangeID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][ExchangeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->ExchangeID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("OrderPriceType");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->OrderPriceType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][OrderPriceType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->OrderPriceType.clear();
				}
				iter = jsonMap.find("Direction");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Direction][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Direction.clear();
				}
				iter = jsonMap.find("OffsetFlag");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->OffsetFlag.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][OffsetFlag][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->OffsetFlag.clear();
				}
				iter = jsonMap.find("Price");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Price][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Price.clear();
				}
				iter = jsonMap.find("Volume");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Volume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Volume.clear();
				}
				iter = jsonMap.find("VolumeDisplay");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][VolumeDisplay][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->VolumeDisplay.clear();
				}
				iter = jsonMap.find("VolumeMode");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->VolumeMode.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][VolumeMode][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->VolumeMode.clear();
				}
				iter = jsonMap.find("Cost");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Cost][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Cost.clear();
				}
				iter = jsonMap.find("OrderType");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->OrderType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][OrderType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->OrderType.clear();
				}
				iter = jsonMap.find("GTDTime");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->GTDTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][GTDTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->GTDTime.clear();
				}
				iter = jsonMap.find("MinVolume");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->MinVolume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][MinVolume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->MinVolume.clear();
				}
				iter = jsonMap.find("BusinessType");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->BusinessType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][BusinessType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->BusinessType.clear();
				}
				iter = jsonMap.find("BusinessValue");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->BusinessValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][BusinessValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->BusinessValue.clear();
				}
				iter = jsonMap.find("CloseOrderID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->CloseOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][CloseOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->CloseOrderID.clear();
				}
				iter = jsonMap.find("IsCrossMargin");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->IsCrossMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][IsCrossMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->IsCrossMargin.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("OrderID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][OrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->OrderID.clear();
				}
				iter = jsonMap.find("CopyMemberID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->CopyMemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][CopyMemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->CopyMemberID.clear();
				}
				iter = jsonMap.find("CopyOrderID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->CopyOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][CopyOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->CopyOrderID.clear();
				}
				iter = jsonMap.find("Leverage");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Leverage.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Leverage][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Leverage.clear();
				}
				iter = jsonMap.find("CopyProfitRate");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->CopyProfitRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][CopyProfitRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->CopyProfitRate.clear();
				}
				iter = jsonMap.find("APPID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][APPID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->APPID.clear();
				}
				iter = jsonMap.find("PositionID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][PositionID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->PositionID.clear();
				}
				iter = jsonMap.find("TriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->TriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][TriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->TriggerPrice.clear();
				}
				iter = jsonMap.find("Reserve");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Reserve.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Reserve][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Reserve.clear();
				}
				iter = jsonMap.find("OrderStatus");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->OrderStatus.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][OrderStatus][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->OrderStatus.clear();
				}
				iter = jsonMap.find("DeriveSource");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->DeriveSource.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][DeriveSource][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->DeriveSource.clear();
				}
				iter = jsonMap.find("DeriveDetail");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->DeriveDetail.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][DeriveDetail][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->DeriveDetail.clear();
				}
				iter = jsonMap.find("VolumeTraded");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->VolumeTraded.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][VolumeTraded][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->VolumeTraded.clear();
				}
				iter = jsonMap.find("VolumeRemain");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->VolumeRemain.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][VolumeRemain][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->VolumeRemain.clear();
				}
				iter = jsonMap.find("VolumeCancled");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->VolumeCancled.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][VolumeCancled][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->VolumeCancled.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->UpdateTime.clear();
				}
				iter = jsonMap.find("Priority");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Priority.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Priority][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Priority.clear();
				}
				iter = jsonMap.find("TimeSortNo");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->TimeSortNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][TimeSortNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->TimeSortNo.clear();
				}
				iter = jsonMap.find("FrontNo");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->FrontNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][FrontNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->FrontNo.clear();
				}
				iter = jsonMap.find("PriceCurrency");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->PriceCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][PriceCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->PriceCurrency.clear();
				}
				iter = jsonMap.find("FeeCurrency");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->FeeCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][FeeCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->FeeCurrency.clear();
				}
				iter = jsonMap.find("ClearCurrency");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->ClearCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][ClearCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->ClearCurrency.clear();
				}
				iter = jsonMap.find("FrozenMoney");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->FrozenMoney.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][FrozenMoney][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->FrozenMoney.clear();
				}
				iter = jsonMap.find("FrozenFee");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->FrozenFee.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][FrozenFee][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->FrozenFee.clear();
				}
				iter = jsonMap.find("FrozenMargin");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->FrozenMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][FrozenMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->FrozenMargin.clear();
				}
				iter = jsonMap.find("Fee");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Fee.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Fee][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Fee.clear();
				}
				iter = jsonMap.find("CloseProfit");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->CloseProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][CloseProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->CloseProfit.clear();
				}
				iter = jsonMap.find("Turnover");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Turnover.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Turnover][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Turnover.clear();
				}
				iter = jsonMap.find("RelatedOrderID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->RelatedOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][RelatedOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->RelatedOrderID.clear();
				}
				iter = jsonMap.find("BusinessResult");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->BusinessResult.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][BusinessResult][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->BusinessResult.clear();
				}
				iter = jsonMap.find("BusinessNo");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->BusinessNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][BusinessNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->BusinessNo.clear();
				}
				iter = jsonMap.find("Tradable");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Tradable.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Tradable][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Tradable.clear();
				}
				iter = jsonMap.find("SettlementGroup");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][SettlementGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->SettlementGroup.clear();
				}
				iter = jsonMap.find("PosiDirection");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->PosiDirection.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][PosiDirection][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->PosiDirection.clear();
				}
				iter = jsonMap.find("TradePrice");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->TradePrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][TradePrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->TradePrice.clear();
				}
				iter = jsonMap.find("OpenPrice");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->OpenPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][OpenPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->OpenPrice.clear();
				}
				iter = jsonMap.find("TriggerOrderID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->TriggerOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][TriggerOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->TriggerOrderID.clear();
				}
				iter = jsonMap.find("SLTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->SLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][SLTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->SLTriggerPrice.clear();
				}
				iter = jsonMap.find("TPTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->TPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][TPTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->TPTriggerPrice.clear();
				}
				iter = jsonMap.find("CopyProfit");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->CopyProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][CopyProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->CopyProfit.clear();
				}
				iter = jsonMap.find("Position");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Position.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Position][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Position.clear();
				}
				iter = jsonMap.find("UserID");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][UserID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->UserID.clear();
				}
				iter = jsonMap.find("LastPriceByInsert");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->LastPriceByInsert.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][LastPriceByInsert][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->LastPriceByInsert.clear();
				}
				iter = jsonMap.find("BidPrice1ByInsert");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->BidPrice1ByInsert.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][BidPrice1ByInsert][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->BidPrice1ByInsert.clear();
				}
				iter = jsonMap.find("AskPrice1ByInsert");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->AskPrice1ByInsert.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][AskPrice1ByInsert][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->AskPrice1ByInsert.clear();
				}
				iter = jsonMap.find("Available");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->Available.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][Available][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->Available.clear();
				}
				iter = jsonMap.find("CreateTime");
				if(iter != jsonMap.end())
				{
					if(!((COrderField*)pOutput)->CreateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[Order][CreateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((COrderField*)pOutput)->CreateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[Order][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &COrderField::m_Describe;		
		}
	case FID_TriggerOrder: //条件报单
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("TradeUnitID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TradeUnitID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TradeUnitID.clear();
				}
				iter = jsonMap.find("AccountID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][AccountID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->AccountID.clear();
				}
				iter = jsonMap.find("LocalID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][LocalID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->LocalID.clear();
				}
				iter = jsonMap.find("ExchangeID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][ExchangeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->ExchangeID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("OrderPriceType");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->OrderPriceType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][OrderPriceType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->OrderPriceType.clear();
				}
				iter = jsonMap.find("Direction");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][Direction][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->Direction.clear();
				}
				iter = jsonMap.find("OffsetFlag");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->OffsetFlag.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][OffsetFlag][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->OffsetFlag.clear();
				}
				iter = jsonMap.find("Price");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][Price][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->Price.clear();
				}
				iter = jsonMap.find("Volume");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][Volume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->Volume.clear();
				}
				iter = jsonMap.find("VolumeDisplay");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][VolumeDisplay][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->VolumeDisplay.clear();
				}
				iter = jsonMap.find("VolumeMode");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->VolumeMode.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][VolumeMode][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->VolumeMode.clear();
				}
				iter = jsonMap.find("Cost");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][Cost][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->Cost.clear();
				}
				iter = jsonMap.find("OrderType");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->OrderType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][OrderType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->OrderType.clear();
				}
				iter = jsonMap.find("GTDTime");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->GTDTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][GTDTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->GTDTime.clear();
				}
				iter = jsonMap.find("MinVolume");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->MinVolume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][MinVolume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->MinVolume.clear();
				}
				iter = jsonMap.find("BusinessType");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->BusinessType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][BusinessType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->BusinessType.clear();
				}
				iter = jsonMap.find("BusinessValue");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->BusinessValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][BusinessValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->BusinessValue.clear();
				}
				iter = jsonMap.find("CloseOrderID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CloseOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CloseOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CloseOrderID.clear();
				}
				iter = jsonMap.find("IsCrossMargin");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->IsCrossMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][IsCrossMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->IsCrossMargin.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("OrderID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][OrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->OrderID.clear();
				}
				iter = jsonMap.find("CopyMemberID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CopyMemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CopyMemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CopyMemberID.clear();
				}
				iter = jsonMap.find("CopyOrderID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CopyOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CopyOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CopyOrderID.clear();
				}
				iter = jsonMap.find("Leverage");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->Leverage.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][Leverage][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->Leverage.clear();
				}
				iter = jsonMap.find("CopyProfitRate");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CopyProfitRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CopyProfitRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CopyProfitRate.clear();
				}
				iter = jsonMap.find("APPID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][APPID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->APPID.clear();
				}
				iter = jsonMap.find("PositionID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][PositionID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->PositionID.clear();
				}
				iter = jsonMap.find("TriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TriggerPrice.clear();
				}
				iter = jsonMap.find("Reserve");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->Reserve.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][Reserve][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->Reserve.clear();
				}
				iter = jsonMap.find("SLPrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->SLPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][SLPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->SLPrice.clear();
				}
				iter = jsonMap.find("SLTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->SLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][SLTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->SLTriggerPrice.clear();
				}
				iter = jsonMap.find("TPPrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TPPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TPPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TPPrice.clear();
				}
				iter = jsonMap.find("TPTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TPTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TPTriggerPrice.clear();
				}
				iter = jsonMap.find("RiskBefore");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->RiskBefore.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][RiskBefore][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->RiskBefore.clear();
				}
				iter = jsonMap.find("TriggerOrderType");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TriggerOrderType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TriggerOrderType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TriggerOrderType.clear();
				}
				iter = jsonMap.find("TriggerDetail");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TriggerDetail.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TriggerDetail][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TriggerDetail.clear();
				}
				iter = jsonMap.find("TriggerPriceType");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TriggerPriceType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TriggerPriceType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TriggerPriceType.clear();
				}
				iter = jsonMap.find("TriggerValue");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TriggerValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TriggerValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TriggerValue.clear();
				}
				iter = jsonMap.find("CloseSLPrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CloseSLPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CloseSLPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CloseSLPrice.clear();
				}
				iter = jsonMap.find("CloseSLTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CloseSLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CloseSLTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CloseSLTriggerPrice.clear();
				}
				iter = jsonMap.find("CloseTPPrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CloseTPPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CloseTPPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CloseTPPrice.clear();
				}
				iter = jsonMap.find("CloseTPTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CloseTPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CloseTPTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CloseTPTriggerPrice.clear();
				}
				iter = jsonMap.find("CloseOrderPriceType");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CloseOrderPriceType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CloseOrderPriceType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CloseOrderPriceType.clear();
				}
				iter = jsonMap.find("ClosePrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->ClosePrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][ClosePrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->ClosePrice.clear();
				}
				iter = jsonMap.find("CloseTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CloseTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CloseTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CloseTriggerPrice.clear();
				}
				iter = jsonMap.find("RelatedOrderID");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->RelatedOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][RelatedOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->RelatedOrderID.clear();
				}
				iter = jsonMap.find("ActiveTime");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->ActiveTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][ActiveTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->ActiveTime.clear();
				}
				iter = jsonMap.find("TriggerTime");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TriggerTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TriggerTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TriggerTime.clear();
				}
				iter = jsonMap.find("TimeSortNo");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TimeSortNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TimeSortNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TimeSortNo.clear();
				}
				iter = jsonMap.find("TriggerStatus");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->TriggerStatus.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][TriggerStatus][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->TriggerStatus.clear();
				}
				iter = jsonMap.find("PosiDirection");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->PosiDirection.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][PosiDirection][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->PosiDirection.clear();
				}
				iter = jsonMap.find("FrontNo");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->FrontNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][FrontNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->FrontNo.clear();
				}
				iter = jsonMap.find("ErrorNo");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->ErrorNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][ErrorNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->ErrorNo.clear();
				}
				iter = jsonMap.find("ErrorMsg");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->ErrorMsg.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][ErrorMsg][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->ErrorMsg.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->UpdateTime.clear();
				}
				iter = jsonMap.find("BusinessNo");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->BusinessNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][BusinessNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->BusinessNo.clear();
				}
				iter = jsonMap.find("CreateTime");
				if(iter != jsonMap.end())
				{
					if(!((CTriggerOrderField*)pOutput)->CreateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[TriggerOrder][CreateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CTriggerOrderField*)pOutput)->CreateTime.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[TriggerOrder][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CTriggerOrderField::m_Describe;		
		}
	case FID_PositionOrder: //持仓订单组成
		{
			string sField = "";
			try
			{
				iter = jsonMap.find("MemberID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->MemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][MemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->MemberID.clear();
				}
				iter = jsonMap.find("TradeUnitID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->TradeUnitID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][TradeUnitID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->TradeUnitID.clear();
				}
				iter = jsonMap.find("AccountID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->AccountID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][AccountID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->AccountID.clear();
				}
				iter = jsonMap.find("LocalID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->LocalID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][LocalID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->LocalID.clear();
				}
				iter = jsonMap.find("ExchangeID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->ExchangeID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][ExchangeID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->ExchangeID.clear();
				}
				iter = jsonMap.find("InstrumentID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->InstrumentID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][InstrumentID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->InstrumentID.clear();
				}
				iter = jsonMap.find("OrderPriceType");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->OrderPriceType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][OrderPriceType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->OrderPriceType.clear();
				}
				iter = jsonMap.find("Direction");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Direction.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Direction][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Direction.clear();
				}
				iter = jsonMap.find("OffsetFlag");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->OffsetFlag.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][OffsetFlag][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->OffsetFlag.clear();
				}
				iter = jsonMap.find("Price");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Price.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Price][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Price.clear();
				}
				iter = jsonMap.find("Volume");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Volume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Volume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Volume.clear();
				}
				iter = jsonMap.find("VolumeDisplay");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->VolumeDisplay.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][VolumeDisplay][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->VolumeDisplay.clear();
				}
				iter = jsonMap.find("VolumeMode");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->VolumeMode.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][VolumeMode][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->VolumeMode.clear();
				}
				iter = jsonMap.find("Cost");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Cost.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Cost][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Cost.clear();
				}
				iter = jsonMap.find("OrderType");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->OrderType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][OrderType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->OrderType.clear();
				}
				iter = jsonMap.find("GTDTime");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->GTDTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][GTDTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->GTDTime.clear();
				}
				iter = jsonMap.find("MinVolume");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->MinVolume.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][MinVolume][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->MinVolume.clear();
				}
				iter = jsonMap.find("BusinessType");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->BusinessType.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][BusinessType][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->BusinessType.clear();
				}
				iter = jsonMap.find("BusinessValue");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->BusinessValue.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][BusinessValue][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->BusinessValue.clear();
				}
				iter = jsonMap.find("CloseOrderID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->CloseOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][CloseOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->CloseOrderID.clear();
				}
				iter = jsonMap.find("IsCrossMargin");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->IsCrossMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][IsCrossMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->IsCrossMargin.clear();
				}
				iter = jsonMap.find("Remark");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Remark.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Remark][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Remark.clear();
				}
				iter = jsonMap.find("OrderID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->OrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][OrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->OrderID.clear();
				}
				iter = jsonMap.find("CopyMemberID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->CopyMemberID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][CopyMemberID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->CopyMemberID.clear();
				}
				iter = jsonMap.find("CopyOrderID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->CopyOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][CopyOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->CopyOrderID.clear();
				}
				iter = jsonMap.find("Leverage");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Leverage.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Leverage][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Leverage.clear();
				}
				iter = jsonMap.find("CopyProfitRate");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->CopyProfitRate.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][CopyProfitRate][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->CopyProfitRate.clear();
				}
				iter = jsonMap.find("APPID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->APPID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][APPID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->APPID.clear();
				}
				iter = jsonMap.find("PositionID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->PositionID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][PositionID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->PositionID.clear();
				}
				iter = jsonMap.find("TriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->TriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][TriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->TriggerPrice.clear();
				}
				iter = jsonMap.find("Reserve");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Reserve.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Reserve][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Reserve.clear();
				}
				iter = jsonMap.find("OrderStatus");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->OrderStatus.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][OrderStatus][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->OrderStatus.clear();
				}
				iter = jsonMap.find("DeriveSource");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->DeriveSource.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][DeriveSource][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->DeriveSource.clear();
				}
				iter = jsonMap.find("DeriveDetail");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->DeriveDetail.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][DeriveDetail][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->DeriveDetail.clear();
				}
				iter = jsonMap.find("VolumeTraded");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->VolumeTraded.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][VolumeTraded][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->VolumeTraded.clear();
				}
				iter = jsonMap.find("VolumeRemain");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->VolumeRemain.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][VolumeRemain][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->VolumeRemain.clear();
				}
				iter = jsonMap.find("VolumeCancled");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->VolumeCancled.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][VolumeCancled][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->VolumeCancled.clear();
				}
				iter = jsonMap.find("InsertTime");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->InsertTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][InsertTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->InsertTime.clear();
				}
				iter = jsonMap.find("UpdateTime");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->UpdateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][UpdateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->UpdateTime.clear();
				}
				iter = jsonMap.find("Priority");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Priority.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Priority][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Priority.clear();
				}
				iter = jsonMap.find("TimeSortNo");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->TimeSortNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][TimeSortNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->TimeSortNo.clear();
				}
				iter = jsonMap.find("FrontNo");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->FrontNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][FrontNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->FrontNo.clear();
				}
				iter = jsonMap.find("PriceCurrency");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->PriceCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][PriceCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->PriceCurrency.clear();
				}
				iter = jsonMap.find("FeeCurrency");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->FeeCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][FeeCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->FeeCurrency.clear();
				}
				iter = jsonMap.find("ClearCurrency");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->ClearCurrency.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][ClearCurrency][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->ClearCurrency.clear();
				}
				iter = jsonMap.find("FrozenMoney");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->FrozenMoney.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][FrozenMoney][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->FrozenMoney.clear();
				}
				iter = jsonMap.find("FrozenFee");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->FrozenFee.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][FrozenFee][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->FrozenFee.clear();
				}
				iter = jsonMap.find("FrozenMargin");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->FrozenMargin.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][FrozenMargin][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->FrozenMargin.clear();
				}
				iter = jsonMap.find("Fee");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Fee.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Fee][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Fee.clear();
				}
				iter = jsonMap.find("CloseProfit");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->CloseProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][CloseProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->CloseProfit.clear();
				}
				iter = jsonMap.find("Turnover");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Turnover.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Turnover][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Turnover.clear();
				}
				iter = jsonMap.find("RelatedOrderID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->RelatedOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][RelatedOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->RelatedOrderID.clear();
				}
				iter = jsonMap.find("BusinessResult");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->BusinessResult.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][BusinessResult][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->BusinessResult.clear();
				}
				iter = jsonMap.find("BusinessNo");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->BusinessNo.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][BusinessNo][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->BusinessNo.clear();
				}
				iter = jsonMap.find("Tradable");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Tradable.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Tradable][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Tradable.clear();
				}
				iter = jsonMap.find("SettlementGroup");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->SettlementGroup.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][SettlementGroup][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->SettlementGroup.clear();
				}
				iter = jsonMap.find("PosiDirection");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->PosiDirection.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][PosiDirection][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->PosiDirection.clear();
				}
				iter = jsonMap.find("TradePrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->TradePrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][TradePrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->TradePrice.clear();
				}
				iter = jsonMap.find("OpenPrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->OpenPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][OpenPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->OpenPrice.clear();
				}
				iter = jsonMap.find("TriggerOrderID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->TriggerOrderID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][TriggerOrderID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->TriggerOrderID.clear();
				}
				iter = jsonMap.find("SLTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->SLTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][SLTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->SLTriggerPrice.clear();
				}
				iter = jsonMap.find("TPTriggerPrice");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->TPTriggerPrice.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][TPTriggerPrice][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->TPTriggerPrice.clear();
				}
				iter = jsonMap.find("CopyProfit");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->CopyProfit.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][CopyProfit][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->CopyProfit.clear();
				}
				iter = jsonMap.find("Position");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Position.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Position][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Position.clear();
				}
				iter = jsonMap.find("UserID");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->UserID.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][UserID][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->UserID.clear();
				}
				iter = jsonMap.find("LastPriceByInsert");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->LastPriceByInsert.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][LastPriceByInsert][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->LastPriceByInsert.clear();
				}
				iter = jsonMap.find("BidPrice1ByInsert");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->BidPrice1ByInsert.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][BidPrice1ByInsert][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->BidPrice1ByInsert.clear();
				}
				iter = jsonMap.find("AskPrice1ByInsert");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->AskPrice1ByInsert.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][AskPrice1ByInsert][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->AskPrice1ByInsert.clear();
				}
				iter = jsonMap.find("Available");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->Available.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][Available][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->Available.clear();
				}
				iter = jsonMap.find("CreateTime");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->CreateTime.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][CreateTime][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->CreateTime.clear();
				}
				iter = jsonMap.find("VolumeToClose");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->VolumeToClose.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][VolumeToClose][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->VolumeToClose.clear();
				}
				iter = jsonMap.find("VolumeClosed");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->VolumeClosed.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][VolumeClosed][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->VolumeClosed.clear();
				}
				iter = jsonMap.find("VolumeOnClose");
				if(iter != jsonMap.end())
				{
					if(!((CPositionOrderField*)pOutput)->VolumeOnClose.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[PositionOrder][VolumeOnClose][isInValid]");
						return NULL;
					}
				}
				else
				{
					((CPositionOrderField*)pOutput)->VolumeOnClose.clear();
				}
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[PositionOrder][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &CPositionOrderField::m_Describe;		
		}
	default:
		{
			sprintf((char*)pOutput, "no FieldFID [%d]", (*iter2).second->fid);
			return NULL;
		}		
	}
	return NULL;
}
