#include "errorDefine.h"
#include "tradingTools.h"
#include "monitorIndex.h"
#include "CDate.h"
#include "CopyField.h"
#include "BaseFunction.h"
#include "printinfo.h"
#include "OutFogServiceApiStruct.h"
#include "utility.h"

const char* INI_FILE_NAME = "trade.ini";

const CLongIndexIDType defaultMemberID = "default";
const CIndexIDType defaultTradeUnitID = "default";
const CInstrumentIDType defaultInstrumentID = "default";
const CLongIndexIDType defaultTriggerOrderType = "default";
//const CIndexIDType defaultUserID = "default";

CLongIndexIDType g_sCFDTaker = "";

//行情配置
bool g_bKLineGap = true;

//默认单边
bool g_bDoubleMarketVolume = false;

int g_nSessionExpireTime = 3 * 24 * 60 * 60;

string g_sBaseClearCurrency = "USDT";

int g_nAPIKeyExpireTime = 90 * 24 * 60 * 60;

// 每个用户最大触发订单数量
int g_nMaxTriggerOrders = 50;

// 每个用户最大订单数量
int g_nMaxOpenOrders = 10000;

// 每个用户最大持仓个数
int g_nMaxOpenPositions = 200;

//多少秒推送一次功能时钟报文
int g_nPushTimeSeconds = 5;

int g_nMaxLastKLineNo = 100;

//接口每秒访问的默认次数限制
int g_nDefaultAccessLimit = 20;

//"0(false):ToRedis,1(ture):ToDB"
bool g_bNoTradeFinishTo = false;

//默认为最优十档价
char g_cOrderPriceType = OPT_TenLevelPrice;

//redo模式运行
bool g_bOnlyRedo = false;

//和前一个版本作比较的redo模式
bool g_bCompare = false;

bool g_bSnapFirst = false;

bool g_bRoolback = false;

UF_INT8 g_nSequenceNo = 0.0;

double g_dLoanInterestFeeRate = 0.02;

set<COrder*> gDeleteOrderSet;
set<CTriggerOrder*> gDeleteTriggerOrderSet;
set<CPosition*> gDeletePositionSet;

CAccountIDType g_sCopyProfitAccountID = "";

void CServiceConfigTrigger::commitAdd(CServiceConfig* pServiceConfig)
{
	if (pServiceConfig->ConfigName == "CFDTaker")
	{
		g_sCFDTaker = pServiceConfig->ConfigValue.getValue();
		return;
	}
	if (pServiceConfig->ConfigName == "KLineGap")
	{
		g_bKLineGap = pServiceConfig->ConfigValue.getValue()[0] - '0';
		return;
	}
	if (pServiceConfig->ConfigName == "DoubleMarketVolume")
	{
		g_bDoubleMarketVolume = pServiceConfig->ConfigValue.getValue()[0] - '0';
		return;
	}
	if (pServiceConfig->ConfigName == "SessionExpireTime")
	{
		g_nSessionExpireTime = atoi((pServiceConfig->ConfigValue.getValue()));
		return;
	}
	if (pServiceConfig->ConfigName == "BaseClearCurrency")
	{
		g_sBaseClearCurrency = pServiceConfig->ConfigValue.getValue();
		return;
	}
	if (pServiceConfig->ConfigName == "MaxTriggerOrders")
	{
		g_nMaxTriggerOrders = atoi((pServiceConfig->ConfigValue.getValue()));
		return;
	}
	if (pServiceConfig->ConfigName == "MaxOpenOrders")
	{
		g_nMaxOpenOrders = atoi((pServiceConfig->ConfigValue.getValue()));
		return;
	}
	if (pServiceConfig->ConfigName == "MaxOpenPositions")
	{
		g_nMaxOpenPositions = atoi((pServiceConfig->ConfigValue.getValue()));
		return;
	}
	if (pServiceConfig->ConfigName == "PushTimeSeconds")
	{
		g_nPushTimeSeconds = atoi((pServiceConfig->ConfigValue.getValue()));
		return;
	}
	if (pServiceConfig->ConfigName == "NoTradeFinishTo")
	{
		g_bNoTradeFinishTo = pServiceConfig->ConfigValue.getValue()[0] - '0';
		return;
	}
	if (pServiceConfig->ConfigName == "OrderPriceType")
	{
		g_cOrderPriceType = pServiceConfig->ConfigValue.getValue()[0];
		return;
	}
	if (pServiceConfig->ConfigName == "LoanInterestFeeRate")
	{
		g_dLoanInterestFeeRate = atof(pServiceConfig->ConfigValue.getValue());
		return;
	}
	if (pServiceConfig->ConfigName == "CopyProfitAccountID")
	{
		g_sCopyProfitAccountID = pServiceConfig->ConfigValue.getValue();
	}
}

void CServiceConfigTrigger::commitUpdate(CServiceConfig* pServiceConfig, CWriteableServiceConfig* pOldServiceConfig)
{
	commitAdd(pServiceConfig);
}


//从哪个序号开始做
UF_INT8 g_nBeginID = -1;
UF_INT8 g_nEndID = -1;

string g_bSnapName = "";

//是否需要把所有的内存数据库全部Push出去
//bool g_bNeedPushAll = false;

CSequence m_gAccountDetailSequence;
CSequence m_gTradeSequence;
CSequence m_gBusinessSequence;
CSequence m_gOrderSequence;
CSequence m_gSortSequence;

//tom 定位为全局可用，在baseWedge里面引用
//#define TEST_RULE
//#ifdef TEST_RULE
//const CDateType defaultDate = "20060101";
//#else
//const CDateType defaultDate = "19000101";
//#endif

// CReadOnlyDateType &getTradingDay(CMDB *pMDB)
// {
// // 	CDataSyncStatus *pStatus;
// // 	pStatus=pMDB->m_DataSyncStatusFactory->getFirst();
// // 	pMDB->m_DataSyncStatusFactory->endGet();
// // 	if (pStatus==NULL)
// // 	{
// // 		return defaultDate;
// // 	}
// // 	return pStatus->TradingDay;
// 
// 	return getCalendarDay(pMDB);
// }

//double getInverse(double beforeInverse)
//{
//	if (fabs(beforeInverse - 0.0) < doubleCompareTick(beforeInverse, SMALLEST_PRECISION))
//		return 1.0;
//	else
//		return 1.0 / beforeInverse;
//}

//CReadOnlyDateType &getMDBDay(CMDB *pMDB)
//{
//	static CMDB *pLastDB = NULL;
//	if (pMDB != NULL)
//	{
//		pLastDB = pMDB;
//	}
//	else
//	{
//		pMDB = pLastDB;
//	}
//
//	if (pMDB == NULL)
//	{
//		return defaultDate;
//	}
//
//	CCurrentTime *pCurrTime;
//	pCurrTime = pMDB->m_CurrentTimeFactory->getFirst();
//	pMDB->m_CurrentTimeFactory->endGet();
//	if (pCurrTime == NULL)
//	{
//		///没有设过日期
//		return defaultDate;
//	}
//	else
//	{
//		///设过日期
//		return pCurrTime->CurrDate;
//	}
//}
//
//void getMDBDay(CMDB *pMDB, int *pYear, int *pMonth, int *pDay)
//{
//	CReadOnlyDateType &today = getMDBDay(pMDB);
//	*pYear = (today[0] - '0') * 1000 +
//		(today[1] - '0') * 100 +
//		(today[2] - '0') * 10 +
//		(today[3] - '0');
//	*pMonth = (today[4] - '0') * 10 +
//		(today[5] - '0');
//	*pDay = (today[6] - '0') * 10 +
//		(today[7] - '0');
//}

//CReadOnlyTimeType &getMDBTime(CMDB *pMDB)
//{
//	static CTimeType defaultTime = "00:00:00";
//
//	if (pMDB == NULL)
//	{
//		return defaultTime;
//	}
//
//	CCurrentTime *pCurrTime;
//	pCurrTime = pMDB->m_CurrentTimeFactory->getFirst();
//	pMDB->m_CurrentTimeFactory->endGet();
//	if (pCurrTime == NULL)
//	{
//		///没有设过时间
//		return defaultTime;
//	}
//	else
//	{
//		///设过时间
//		return pCurrTime->CurrTime;
//	}
//}

//CReadOnlyTimeType &getMDBOldTime(CMDB *pMDB)
//{
//	CCurrentTime *pCurrTime;
//	pCurrTime = pMDB->m_CurrentTimeFactory->getFirst();
//	pMDB->m_CurrentTimeFactory->endGet();
//	if (pCurrTime == NULL)
//	{
//		///没有设过时间
//		static CTimeType defaultTime = "00:00:00";
//		return defaultTime;
//	}
//	else
//	{
//		///设过时间
//		return pCurrTime->OldTime;
//	}
//}

//void getMDBTime(CMDB *pMDB, CMilliSecondsType *pTime, CNumberType *pMillisec)
//{
//	CCurrentTime *pCurrTime;
//	pCurrTime = pMDB->m_CurrentTimeFactory->getFirst();
//	pMDB->m_CurrentTimeFactory->endGet();
//	if (pCurrTime == NULL)
//	{
//		///没有设过时间
//		*pTime = 0;
//		*pMillisec = 0;
//		return;
//	}
//	else
//	{
//		///设过时间
//		*pTime = pCurrTime->CurrTime;
//		*pMillisec = pCurrTime->CurrMilliSecond;
//		return;
//	}
//}

//UF_INT8 getMDBTime(CMDB *pMDB)
//{
//	CCurrentTime *pCurrTime;
//	pCurrTime = pMDB->m_CurrentTimeFactory->getFirst();
//	pMDB->m_CurrentTimeFactory->endGet();
//	if (pCurrTime == NULL)
//		return 0;
//	else
//		return pCurrTime->CurrTime.getValue();
//}

void setMDBTime(CMDB* pMDB, CWriteableCurrentTime* pTime)
{
	CCurrentTime* pOldTime;
	pOldTime = pMDB->m_CurrentTimeFactory->getFirst();
	if (pOldTime == NULL)
	{
		pOldTime = pMDB->m_CurrentTimeFactory->add(pTime);
		new CStringMonitorIndex("CurrentTime", pOldTime->ZoneTime, 10);
		///tom 增加日期的输出，方便tkernel的日期区别
		new CStringMonitorIndex("CurrentDate", pOldTime->ZoneDate, 10);
	}
	else
	{
		///非第一次设时间
		pMDB->m_CurrentTimeFactory->update(pOldTime, pTime);
	}
}

//void setMDBTime(CMDB *pMDB, const int nSeconds, CTransaction *pTransaction)
//{
//	CCurrentTime *pOldTime = pMDB->m_CurrentTimeFactory->getFirst();
//	pMDB->m_CurrentTimeFactory->endGet();
//	if (pOldTime == NULL)
//	{
//		CWriteableCurrentTime CurrTime;
//		CurrTime.SystemID = SystemName;
//		CurrTime.OldTime = 0;
//		CurrTime.OldMillisec = 0;
//
//		CurrTime.CurrTime = nSeconds;
//		time_t dwCurrTime = (time_t)nSeconds;
//		strftime(CurrTime.ZoneDate.getString(), 11, "%Y%m%d", localtime(&dwCurrTime));
//		strftime(CurrTime.ZoneTime.getString(), 11, "%H:%M:%S", localtime(&dwCurrTime));
//
//		pOldTime = pMDB->m_CurrentTimeFactory->add(&CurrTime, pTransaction);
//		new CStringMonitorIndex("CurrentTime", pOldTime->ZoneTime, 10);
//		///tom 增加日期的输出
//		new CStringMonitorIndex("CurrentDate", pOldTime->ZoneDate, 10);
//	}
//	else
//	{
//		///非第一次设时间
//		CWriteableCurrentTime CurrTime;
//		pMDB->m_CurrentTimeFactory->retrieve(pOldTime, &CurrTime);
//		CurrTime.OldMillisec = pOldTime->CurrMillisec;
//		CurrTime.OldTime = pOldTime->CurrTime;
//
//		CurrTime.CurrTime = nSeconds;
//		time_t dwCurrTime = (time_t)nSeconds;
//		strftime(CurrTime.ZoneDate.getString(), 11, "%Y%m%d", localtime(&dwCurrTime));
//		strftime(CurrTime.ZoneTime.getString(), 11, "%H:%M:%S", localtime(&dwCurrTime));
//
//		pMDB->m_CurrentTimeFactory->update(pOldTime, &CurrTime, pTransaction);
//	}
//}

//int getSecond(CReadOnlyTimeType &theTime)
//{
//	return (theTime[6] - '0') * 10 + theTime[7] - '0';
//}

//int getSecondOfDay(CReadOnlyTimeType &theTime)
//{
//	const char *pTime = theTime;
//	int hour = (pTime[0] - '0') * 10 + (pTime[1] - '0');
//	int minute = (pTime[3] - '0') * 10 + (pTime[4] - '0');
//	int second = (pTime[6] - '0') * 10 + (pTime[7] - '0');
//	return (hour * 60 + minute) * 60 + second;
//}

CDirectionType getReverseDirection(CReadOnlyDirectionType direction)
{
	if (direction == D_Buy)
		return D_Sell;
	else
		return D_Buy;
}

CDirectionType getReverseDirection(CPosition* pPosition)
{
	switch (pPosition->PosiDirection)
	{
	case PD_Long:
		return D_Sell;
	case PD_Short:
		return D_Buy;
	case PD_Net:
	{
		if (pPosition->Position > 0)
			return D_Sell;
		else
			return D_Buy;
	}
	}
	return D_Buy;
}

CPosiDirectionType getPositionDirection(CPosition* pPosition)
{
	if (pPosition->PosiDirection == PD_Net)
	{
		if (pPosition->Position > 0)
			return PD_Long;
		else
			return PD_Short;
	}
	else
	{
		return pPosition->PosiDirection;
	}
}

void setOrderStatus(CWriteableOrder* pOrder)
{
	pOrder->VolumeRemain = pOrder->Volume - pOrder->VolumeTraded - pOrder->VolumeCancled;

	if (pOrder->VolumeRemain <= 0.0)
	{
		if (pOrder->VolumeCancled > 0.0)
		{
			if (pOrder->VolumeTraded > 0.0)
				pOrder->OrderStatus = OS_PartCanceled;
			else
				pOrder->OrderStatus = OS_AllCanceled;
		}
		else
		{
			//已经全部成交
			pOrder->OrderStatus = OS_AllTraded;
		}
	}
	else if (pOrder->VolumeRemain == pOrder->Volume)
	{
		//一点都没有成交
		pOrder->OrderStatus = OS_NoTraded;
	}
	else
	{
		//成交了部分
		pOrder->OrderStatus = OS_PartTraded;
	}
}

CReadOnlyPriceType& getPreSettlementPrice(CInstrument* pInstrument)
{
	CMarketData* pMarketData = pInstrument->getMarketData();
	if (pMarketData != NULL)
	{
		if (!pMarketData->PreSettlementPrice.isNull())
		{
			return pMarketData->PreSettlementPrice;
		}
		if (!pMarketData->PreClosePrice.isNull())
		{
			return pMarketData->PreClosePrice;
		}
	}
	return pInstrument->BasisPrice;
}

CReadOnlyPriceType& getFrozenMarginPrice(CMDB* pMDB, CInstrument* pInstrument, CReadOnlyPriceType& Price, CDirectionType Direction)
{
	CMarketData* pMarketData = pInstrument->getMarketData();
	if (pMarketData == NULL)
		return pInstrument->BasisPrice;

	//表明使用昨结算价的情况下，使用昨结算价
	if (pInstrument->MarginPriceType == MPT_PreSettlementPrice)
		return getPreSettlementPrice(pInstrument);

	//市价单没有委托价的情况下，按照结算价
	if (Price.isNull())
		return pMarketData->LastPrice;

	//有委托价的情况下，委托价和对手盘的不同选择
	if (pInstrument->IsInverse)
	{
		if (Direction == D_Buy)
		{
			CMarketOrder* pMarketOrder = pMDB->m_MarketOrderFactory->findFirstBySellMBLData(pInstrument->ExchangeID, pInstrument->InstrumentID);
			if (pMarketOrder != NULL)
			{
				if (Price > pMarketOrder->Price)
					return pMarketOrder->Price;
			}
		}
	}
	else
	{
		if (Direction == D_Sell)
		{
			CMarketOrder* pMarketOrder = pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(pInstrument->ExchangeID, pInstrument->InstrumentID);
			if (pMarketOrder != NULL)
			{
				if (Price < pMarketOrder->Price)
					return pMarketOrder->Price;
			}
		}
	}
	return Price;
}

CReadOnlyPriceType& getMarginPrice(CInstrument* pInstrument, CTrade* pTrade)
{
	//原则是按照设定，选择成交价、前结算价、合理价格选择一个
	static CPriceType defaultMarginPrice = 0.0;
	switch (pInstrument->MarginPriceType)
	{
	case MPT_MarkedPrice:
	{
		CMarketData* pMarketData = pInstrument->getMarketData();
		if (pMarketData != NULL)
		{
			defaultMarginPrice = pMarketData->MarkedPrice;
		}
		break;
	}
	case MPT_PreSettlementPrice:
	{
		defaultMarginPrice = getPreSettlementPrice(pInstrument);
		break;
	}
	case MPT_LastPrice:
	case MPT_OpenPrice:
	case MPT_None:
	default:
	{
		defaultMarginPrice = pTrade->Price;
	}
	}

	return defaultMarginPrice;
}

//bool hasRight(CMDB *pMDB, CReadOnlyShortContentType &AllowID, CIndexIDType FunctionCode)
//{
//	if (pMDB->m_AllowFactory->getCount() == 0)
//		return true;
//	if (pMDB->m_AllowFactory->findByAllowIDAccessTypeValue(AllowID, AT_FuncCode, FunctionCode.getValue()))
//		return true;
//	return false;
//}

//CUser* checkLocalID(CMDB *pMDB, CErrorEngine *pErrorEngine, CReadOnlyLongIndexIDType &userID, CReadOnlyIndexIDType &LocalID, CLongIndexIDType* pMemberID)
//{
//	CUser *pUser = NULL;
//	pUser = pMDB->m_UserFactory->findByPK(userID);
//	CWriteableUser theUser;
//
//	if (pUser == NULL)
//	{
//		pErrorEngine->reportError(ERROR_USER_NOT_FOUND);
//		return NULL;
//	}
//	else
//	{
//		pMDB->m_UserFactory->retrieve(pUser, &theUser);
//	}
//
//	///如果不设置localid，是允许的。
//	//这是一种特殊情况，如果设置了null,不检查localID
//	if (!LocalID.isNull())
//	{
//		if (theUser.MaxLocalID >= LocalID)
//		{
//			pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);
//			return NULL;
//		}
//		theUser.MaxLocalID = LocalID;
//		theUser.SinkType = ST_NoSink;
//		pMDB->m_UserFactory->update(pUser, &theUser);
//	}
//
//	if (pMemberID != NULL && (*pMemberID).isNull())
//		*pMemberID = pUser->MemberID;
//
//	return pUser;
//}

bool CheckManagerFront(CErrorEngine* pErrorEngine, const char* pToken)
{
	if (pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && pToken[1] == '\0')
		return true;

	return pErrorEngine->reportError(ERROR_ONLY_TRADE_MANAGER_CAN_DO_THIS);
}

bool CheckManagerSession(CMDB* pMDB, CErrorEngine* pErrorEngine, TSessionRef* pSession)
{
	if (pSession->pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && pSession->pToken[1] == '\0')
		return true;

	CUserSession* pUserSession = NULL;
	//if (pSession->pToken[0] == '\0')
	//	pUserSession = pMDB->m_UserSessionFactory->findBySessionNo(pSession->frontID, pSession->sessionID);
	//else
	pUserSession = pMDB->m_UserSessionFactory->findByToken(pSession->pToken);

	if (pUserSession == NULL)
		return pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);

	//如果session时间过期，需要重新生成，并且返回错误
	if (pUserSession->ExpireTime < g_nCurrentTime)
		return pErrorEngine->reportError(ERROR_APIKEY_EXPIRED);

	if (pUserSession->UserType == UT_SuperManager)
		return true;

	return pErrorEngine->reportError(ERROR_ONLY_TRADE_MANAGER_CAN_DO_THIS);
}

bool CheckAccountLocalID(CMDB* pMDB, CErrorEngine* pErrorEngine, CIndexIDType& LocalID, CLongIndexIDType& MemberID, CAccountIDType& AccountID, CShortIndexIDType& SettlementGroup, CCurrencyType& Currency, CTransaction* pTransaction)
{
	CAccount* pAccount = pMDB->m_AccountFactory->findByAccountIDAndCurrency(MemberID, AccountID, SettlementGroup, Currency);
	//没有资金也可以入金，所以没有资金的情况是可以检测通过的
	if (pAccount == NULL)
		return true;

	if (!LocalID.isNull())
	{
		if (pAccount->MaxLocalID >= LocalID)
			return pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);

		CWriteableAccount theAccount;
		pMDB->m_AccountFactory->retrieve(pAccount, &theAccount);
		theAccount.MaxLocalID = LocalID;
		pMDB->m_AccountFactory->update(pAccount, &theAccount, pTransaction);
	}
	return pAccount;
}

bool CheckUserLocalID(CMDB* pMDB, CErrorEngine* pErrorEngine, CReadOnlyIndexIDType& LocalID, const char* UserID, CTransaction* pTransaction)
{
	//CUser* pUser = pMDB->m_UserFactory->findByUserID(UserID);
	//if (pUser == NULL)
	//	return pErrorEngine->reportError(ERROR_USER_NOT_FOUND);

	CUserSession* pUserSession = pMDB->m_UserSessionFactory->findByUserIDAndAppID(UserID, "");
	if (pUserSession == NULL)
		return pErrorEngine->reportError(ERROR_USER_NOT_FOUND);

	if (!LocalID.isNull())
	{
		if (pUserSession->MaxLocalID >= LocalID)
			return pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);

		//CWriteableUser theUser;
		//pMDB->m_UserFactory->retrieve(pUser, &theUser);
		//theUser.MaxLocalID = LocalID;
		//pMDB->m_UserFactory->update(pUser, &theUser, pTransaction);

		CWriteableUserSession theUserSession;
		pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
		theUserSession.MaxLocalID = LocalID;
		pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession, pTransaction);
	}
	return true;
}

CMember* CheckMemberLocalID(CMDB* pMDB, CErrorEngine* pErrorEngine, CReadOnlyIndexIDType& LocalID, CLongIndexIDType& MemberID, CTransaction* pTransaction)
{
	//如果是管理员的报单，不需要UserSession的控制，此时需要通过MemberID来控制
	CMember* pMember = pMDB->m_MemberFactory->findByMemberID(MemberID);
	if (pMember == NULL)
	{
		pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);
		return NULL;
	}

	if (!LocalID.isNull())
	{
		if (pMember->MaxLocalID >= LocalID)
		{
			pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);
			return NULL;
		}

		CWriteableMember theMember;
		pMDB->m_MemberFactory->retrieve(pMember, &theMember);
		theMember.MaxLocalID = LocalID;
		theMember.SinkType = ST_NoSink;
		pMDB->m_MemberFactory->update(pMember, &theMember, pTransaction);
	}
	return pMember;
}

CMember* CheckUserSessionLocalID(CMDB* pMDB, CErrorEngine* pErrorEngine, TSessionRef* pSession, CReadOnlyIndexIDType& LocalID, CTransaction* pTransaction, CLongIndexIDType& MemberID, CShortIndexIDType* pAPPID)
{
	if (pSession->pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && pSession->pToken[1] == '\0')
	{
		return CheckMemberLocalID(pMDB, pErrorEngine, LocalID, MemberID, pTransaction);
	}

	CUserSession* pUserSession = NULL;
	//if (pSession->pToken[0] == '\0')
	//	pUserSession = pMDB->m_UserSessionFactory->findBySessionNo(pSession->frontID, pSession->sessionID);
	//else
	pUserSession = pMDB->m_UserSessionFactory->findByToken(pSession->pToken);

	if (pUserSession == NULL)
	{
		pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);
		return NULL;
	}

	//如果session时间过期，需要重新生成，并且返回错误
	if (pUserSession->ExpireTime < g_nCurrentTime)
	{
		pErrorEngine->reportError(ERROR_APIKEY_EXPIRED);
		return NULL;
	}

	if (pAPPID != NULL && *pAPPID == "")
		*pAPPID = pUserSession->APPID;


	if (pUserSession->UserType == UT_Observer)
	{
		pErrorEngine->reportError(ERROR_TRADE_FORBIDDEN);
		return NULL;
	}

	//普通用户，文不对其，就直接返回错误

	if (MemberID.isNull())
		MemberID = pUserSession->MemberID;

	CMember* pMember = pMDB->m_MemberFactory->findByMemberID(MemberID);
	if (pMember == NULL)
	{
		pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);
		return NULL;
	}

	if (pUserSession->MemberID != MemberID)
	{
		//还有什么事情是管理员不能干的吗
		if (pUserSession->UserType != UT_SuperManager)
		{
			pErrorEngine->reportError(ERROR_TRADE_FORBIDDEN);
			return NULL;
		}
	}

	if (!LocalID.isNull())
	{
		if (pUserSession->MaxLocalID >= LocalID)
		{
			pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);
			return NULL;
		}

		CWriteableUserSession theUserSession;
		pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
		theUserSession.MaxLocalID = LocalID;
		theUserSession.SinkType = ST_NoSink;
		pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession, pTransaction);
	}
	return pMember;
}

CMember* CheckMangerSessionLocalID(CMDB* pMDB, CErrorEngine* pErrorEngine, TSessionRef* pSession, CReadOnlyIndexIDType& LocalID, CTransaction* pTransaction, CLongIndexIDType* pMemberID, CShortIndexIDType* pAPPID)
{
	if (pSession->pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && pSession->pToken[1] == '\0')
	{
		if (pMemberID == NULL || pMemberID->isNull())
		{
			pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);
			return NULL;
		}
		return CheckMemberLocalID(pMDB, pErrorEngine, LocalID, *pMemberID, pTransaction);
	}

	CUserSession* pUserSession = NULL;
	//if (pSession->pToken[0] == '\0')
	//	pUserSession = pMDB->m_UserSessionFactory->findBySessionNo(pSession->frontID, pSession->sessionID);
	//else
	pUserSession = pMDB->m_UserSessionFactory->findByToken(pSession->pToken);

	if (pUserSession == NULL)
	{
		pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);
		return NULL;
	}

	//如果session时间过期，需要重新生成，并且返回错误
	if (pUserSession->ExpireTime < g_nCurrentTime)
	{
		pErrorEngine->reportError(ERROR_APIKEY_EXPIRED);
		return NULL;
	}

	if (pAPPID != NULL)
		*pAPPID = pUserSession->APPID;

	if (pUserSession->UserType != UT_SuperManager)
	{
		pErrorEngine->reportError(ERROR_TRADE_FORBIDDEN);
		return NULL;
	}

	//普通用户，文不对其，就直接返回错误
	if (pMemberID != NULL)
	{
		if (pMemberID->isNull())
			*pMemberID = pUserSession->MemberID;
	}

	if (!LocalID.isNull())
	{
		if (pUserSession->MaxLocalID >= LocalID)
		{
			pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);
			return NULL;
		}

		CWriteableUserSession theUserSession;
		pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
		theUserSession.MaxLocalID = LocalID;
		theUserSession.SinkType = ST_NoSink;
		pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession, pTransaction);
	}

	return pMDB->m_MemberFactory->findByMemberID(*pMemberID);
}

bool CheckManagerLocalID(CMDB* pMDB, CErrorEngine* pErrorEngine, TSessionRef* pSession, CReadOnlyIndexIDType& LocalID, CTransaction* pTransaction, CShortIndexIDType* pAPPID)
{
	if (pSession->pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && pSession->pToken[1] == '\0')
		return true;

	CUserSession* pUserSession = NULL;
	//if (pSession->pToken[0] == '\0')
	//	pUserSession = pMDB->m_UserSessionFactory->findBySessionNo(pSession->frontID, pSession->sessionID);
	//else
	pUserSession = pMDB->m_UserSessionFactory->findByToken(pSession->pToken);

	if (pUserSession == NULL)
		return pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);

	//如果session时间过期，需要重新生成，并且返回错误
	if (pUserSession->ExpireTime < g_nCurrentTime)
		return pErrorEngine->reportError(ERROR_APIKEY_EXPIRED);

	if (pAPPID != NULL)
		*pAPPID = pUserSession->APPID;

	if (pUserSession->UserType != UT_SuperManager)
		return pErrorEngine->reportError(ERROR_TRADE_FORBIDDEN);

	if (!LocalID.isNull())
	{
		if (pUserSession->MaxLocalID >= LocalID)
			return pErrorEngine->reportError(ERROR_DUPLICATE_ACTION);

		CWriteableUserSession theUserSession;
		pMDB->m_UserSessionFactory->retrieve(pUserSession, &theUserSession);
		theUserSession.MaxLocalID = LocalID;
		theUserSession.SinkType = ST_NoSink;
		pMDB->m_UserSessionFactory->update(pUserSession, &theUserSession, pTransaction);
	}

	return true;
}

bool CheckSession(CMDB* pMDB, CErrorEngine* pErrorEngine, TSessionRef* pSession, CLongIndexIDType& MemberID)
{
	if (pSession->pToken[0] == OUTFOG_REMARK_TOKEN_MANAGER && pSession->pToken[1] == '\0')
		return true;

	CUserSession* pUserSession = NULL;
	//if (pSession->pToken[0] == '\0')
	//	pUserSession = pMDB->m_UserSessionFactory->findBySessionNo(pSession->frontID, pSession->sessionID);
	//else
	pUserSession = pMDB->m_UserSessionFactory->findByToken(pSession->pToken);

	if (pUserSession == NULL)
		return pErrorEngine->reportError(ERROR_USER_NOT_LOGIN);

	//如果session时间过期，需要重新生成，并且返回错误
	if (pUserSession->ExpireTime < g_nCurrentTime)
		return pErrorEngine->reportError(ERROR_APIKEY_EXPIRED);

	//普通用户，文不对其，就直接返回错误
	if (!MemberID.isNull())
	{
		if (MemberID != pUserSession->MemberID)
		{
			if (pUserSession->UserType != UT_SuperManager)
				return pErrorEngine->reportError(ERROR_TRADE_FORBIDDEN);
		}
	}
	else
	{
		MemberID = pUserSession->MemberID;
	}
	return true;
}

//static int fceil(double v)
//{
//	return int(ceil(v - SMALL_DOUBLE));
//}
//
//static int ffloor(double v)
//{
//	return int(floor(v + SMALL_DOUBLE));
//}

bool calculatePriceBanding(CReadOnlyPriceType& refPrice, CReadOnlyPriceType& tick,
	CReadOnlyValueModeType& valueMode, CReadOnlyRoundingModeType& roundingMode,
	CReadOnlyPriceType& upperValue, CReadOnlyPriceType& lowerValue,
	CPriceType* pUpperLimit, CPriceType* pLowerLimit)
{
	if (refPrice.isNull())
		return false;
	CPriceType tmp;

	if (!upperValue.isNull())
	{
		if (valueMode == VM_Absolute)
			*pUpperLimit = refPrice + upperValue;
		else
			*pUpperLimit = refPrice * (1 + upperValue);

		switch (roundingMode)
		{
		case RM_Out:
			(*pUpperLimit).toUpperTick(tick);
			break;
		case RM_In:
			(*pUpperLimit).toLowerTick(tick);
			break;
		case RM_Round:
			(*pUpperLimit).toRoundTick(tick);
			break;
		default:
			(*pUpperLimit).toLowerTick(tick);
			break;
		}
	}

	if (!lowerValue.isNull())
	{
		///先直接计算价格绑定
		if (valueMode == VM_Absolute)
			*pLowerLimit = refPrice - lowerValue;
		else
			*pLowerLimit = refPrice * (1 - lowerValue);

		switch (roundingMode)
		{
		case RM_Out:
			(*pLowerLimit).toLowerTick(tick);
			break;
		case RM_In:
			(*pLowerLimit).toUpperTick(tick);
			break;
		case RM_Round:
			(*pLowerLimit).toRoundTick(tick);
			break;
		default:
			(*pLowerLimit).toLowerTick(tick);
			break;
		}
	}
	return true;
}

void applyReserveChange(CAccount* pOldAccount, CWriteableAccount* pNewAccount, CMoneyType Profit)
{
	switch (pNewAccount->ReserveType)
	{
	case RT_Inferior:
	{
		//保留资金亏完，亏真钱
		if (Profit < 0.0)
			pNewAccount->Reserve += Profit;
		//		if (pNewAccount->Reserve < pNewAccount->ReserveLimit)
		//			pNewAccount->Reserve = pNewAccount->ReserveLimit;
		applyAccountChange(pNewAccount);
		break;
	}
	case RT_Prior:
	{
		//先亏真钱，再亏保留资金
		if (pOldAccount->Reserve > pNewAccount->Balance)
		{
			pNewAccount->Reserve = pNewAccount->Balance;
			//			if (pNewAccount->Reserve < pNewAccount->ReserveLimit)
			//				pNewAccount->Reserve = pNewAccount->ReserveLimit;
			applyAccountChange(pNewAccount);
		}
		break;
	}
	case RT_Guarantee:
	default:
	{
	}
	}
}

void applyAccountChange(CWriteableAccount* pAccount)
{
	//pAccount->ReserveActive = true;
	//pAccount->ReserveLimit = 0.0;
	//pAccount->ReserveProfit = 0.0;

	pAccount->Balance =
		pAccount->PreBalance
		+ pAccount->MoneyChange
		+ pAccount->Deposit
		- pAccount->Withdraw
		+ pAccount->CloseProfit
		- pAccount->Fee
		- pAccount->CopyProfit;

	////这样可以利用+=来处理精度的归零问题
	//pAccount->Balance = pAccount->PreBalance;
	//pAccount->Balance += pAccount->MoneyChange
	//	+ pAccount->Deposit
	//	- pAccount->Withdraw
	//	+ pAccount->CloseProfit
	//	- pAccount->Fee;

	//pAccount->Balance.toLowerTick(pAccount->getCurrency()->MoneyTick);

	////这样可以利用+=来处理精度的归零问题
	//pAccount->Available = pAccount->Balance;
	//pAccount->Available -=(pAccount->UseMargin
	//	+pAccount->FrozenMargin
	//	+pAccount->FrozenMoney
	//	+pAccount->FrozenFee);
	pAccount->Available =
		pAccount->Balance
		- pAccount->UseMargin
		- pAccount->FrozenMargin
		- pAccount->FrozenMoney
		- pAccount->FrozenFee;

	//reserve不可能小于0
	if (pAccount->Reserve < 0.0)
		pAccount->Reserve = 0.0;

	//	if (pAccount->ReserveLimit < 0.0)
	//		pAccount->ReserveLimit = 0.0;

		//ReserveLimit 不可能 大于 Reserve
	//	if (pAccount->ReserveLimit > pAccount->Reserve)
	//		pAccount->ReserveLimit = pAccount->Reserve;

		//任何一笔转出，意味着体验金转出，所以可转就是 加上体验金的转出之后的可用为0
	pAccount->Withdrawable = pAccount->Available - pAccount->Reserve;
	if (pAccount->ReserveActive)
	{
		if (pAccount->ReserveLimit > pAccount->Reserve)
			pAccount->Available -= pAccount->Reserve;
		else
			pAccount->Available -= pAccount->ReserveLimit;
	}
	else
	{
		pAccount->Available = pAccount->Withdrawable;
		pAccount->ReserveAvailable = 0.0;
	}
}

void applyAccountClear(CWriteableAccount* pAccount)
{
	pAccount->PreBalance = pAccount->Balance;
	double MoneyTick = pAccount->getCurrency()->MoneyTick.getValue();
	pAccount->PreBalance.toLowerTick(MoneyTick);
	pAccount->MoneyChange.init();
	pAccount->Deposit.init();
	pAccount->Withdraw.init();
	pAccount->CloseProfit.init();
	pAccount->Fee.init();
	pAccount->CopyProfit.init();
}


// bool validSession(CMDB *pMDB,CErrorEngine *pErrorEngine,CReadOnlyNumberType &sessionID,CReadOnlyNumberType &frontID,
// 				  CIndexIDType *pMemberID,CReadOnlyIndexIDType *pUserID, bool managerOnly)
// {
// 	if (sessionID==0)
// 	{
// 		///超级会话
// 		return true;
// 	}
// 	
// 	CUserSession *pUserSession;
// 	pUserSession=pMDB->m_UserSessionFactory->findBySessionNo(frontID,SessionID);
// 	if (pUserSession==NULL)
// 	{
// 		///找不到会话
// 		pErrorEngine->reportError(ERROR_SESSION_NOT_FOUND);
// 		return false;
// 	}
// /*	
// 	if (!pUserSession->IsLogin)
// 	{
// 		///会话还没有登录
// 		m_pErrorEngine->reportError(ERROR_NOT_LOGIN_YET);
// 		return false;
// 	}
// */
// 	if (pUserSession->UserType==UT_TradeManager)
// 	{
// 		///是管理员，那什么都可以做
// 		return true;
// 	}
// 	if (managerOnly)
// 	{
// 		///必要要求是管理员才可以做此项操作
// 		pErrorEngine->reportError(ERROR_ONLY_TRADE_MANAGER_CAN_DO_THIS);
// 		return false;
// 	}
// 	
// 	if (pUserID!=NULL)
// 	{
// 		if (*pUserID!=pUserSession->UserID)
// 		{
// 			///用户不符合
// 			pErrorEngine->reportError(ERROR_USER_MISMATCH);
// 			return false;
// 		}
// 	}
// 
// 	if (pMemberID!=NULL)
// 	{
// 		if (*pMemberID==pUserSession->MemberID)
// 		{
// 			return true;
// 		}
// 
// 		CMember *pMember;
// 		pMember=pMDB->m_MemberFactory->findByMemberID(*pMemberID);
// 		if (pMember!=NULL)
// 		{
// 			CReadOnlyIndexIDType *pClearingMemberID=getClearingMemberID(pMember);
// 			if ((pClearingMemberID!=NULL)&&(*pClearingMemberID==pUserSession->MemberID))
// 			{
// 				///结算会员的用户可以处理所属的交易会员
// 				return true;
// 			}
// 		}
// 	}
// 	
// 	///会员不符合
// 	pErrorEngine->reportError(ERROR_CAN_NOT_OPERATE_FOR_OTHER_MEMBER);
// 	///将会员号设置为正确的会员号，以便错误响应能够准确地返回到该会员的私有流中
// 	*pMemberID=pUserSession->MemberID;
// 	return false;
// }

//double getPositionLeverage(CMarginRate *pRate, CInstrument *pInstrument, CPosition *pOldPosition, CPosiDirectionType PosiDirection, double minMarginRatio)
//{
//	if (minMarginRatio == 0.0)
//		return 0.0;
//
//	if (pOldPosition == NULL)
//	{
//		if (pRate != NULL && pRate->ValueMode == VM_Absolute)
//			return -minMarginRatio;
//		else
//			return 1.0 / minMarginRatio;
//	}
//	else
//	{
//		if (pRate != NULL && pRate->ValueMode == VM_Absolute)
//		{
//			if (minMarginRatio > -pOldPosition->Leverage)
//				return -minMarginRatio;
//			else
//				return pOldPosition->Leverage;
//		}
//		else
//		{
//			double Leverage = getInverse(minMarginRatio);
//			if (Leverage < pOldPosition->Leverage)
//				return Leverage;
//			else
//				return pOldPosition->Leverage;
//		}
//	}
//}

double getMinMarginRatio(CVolumeType Position, CVolumeType LongFrozen, CVolumeType ShortFrozen, CInstrument* pInstrument, CMarginRate* pRate, CPosiDirectionType posiDirection)
{
	double marginRatio = 1.0;
	double nMarginPosition = 0;
	double price = pInstrument->getMarketData()->MarkedPrice.getValue();
	if (price == DOUBLE_NaN)
		price = pInstrument->getMarketData()->LastPrice.getValue();
	switch (posiDirection)
	{
	case PD_Long:
	{
		nMarginPosition = Position + LongFrozen;
		if (pInstrument->IsInverse)
			marginRatio = getLongMarginRateValue(nMarginPosition, pRate);
		else
			marginRatio = getLongMarginRateValue(nMarginPosition * price, pRate);
		break;
	}
	case PD_Short:
	{
		nMarginPosition = Position + ShortFrozen;
		if (pInstrument->IsInverse)
			marginRatio = getShortMarginRateValue(nMarginPosition, pRate);
		else
			marginRatio = getShortMarginRateValue(nMarginPosition * price, pRate);
		break;
	}
	case PD_Net:
	{
		if (Position >= 0)
		{
			if (pInstrument->IsInverse)
				marginRatio = MAX(getLongMarginRateValue(Position + LongFrozen, pRate), getShortMarginRateValue(ShortFrozen, pRate));
			else
				marginRatio = MAX(getLongMarginRateValue((Position + LongFrozen) * price, pRate),
					getShortMarginRateValue(ShortFrozen * price, pRate));
		}
		else
		{
			if (pInstrument->IsInverse)
				marginRatio = MAX(getLongMarginRateValue(LongFrozen, pRate), getShortMarginRateValue(-Position + ShortFrozen, pRate));
			else
				marginRatio = MAX(getLongMarginRateValue(LongFrozen * price, pRate),
					getShortMarginRateValue((-Position + ShortFrozen) * price, pRate));
		}
		break;
	}
	default:
	{
		REPORT_EVENT(LOG_CRITICAL, "getMaxLeverage", "invalid position direction while get getMaxLeverage!");
	}
	}
	return marginRatio;
}

double getMinMaintMarginRatio(CVolumeType Position, CInstrument* pInstrument, CMarginRate* pRate, CPosiDirectionType posiDirection)
{
	double marginRatio = 1.0;
	double nMarginPosition = 0;
	double price = pInstrument->getMarketData()->MarkedPrice.getValue();
	if (price == DOUBLE_NaN)
		price = pInstrument->getMarketData()->LastPrice.getValue();
	switch (posiDirection)
	{
	case PD_Long:
	{
		if (pInstrument->IsInverse)
			marginRatio = getLongMaintMarginRateValue(Position, pRate);
		else
			marginRatio = getLongMaintMarginRateValue(Position * price, pRate);
		break;
	}
	case PD_Short:
	{
		if (pInstrument->IsInverse)
			marginRatio = getShortMaintMarginRateValue(Position, pRate);
		else
			marginRatio = getShortMaintMarginRateValue(Position * price, pRate);
		break;
	}
	case PD_Net:
	{
		if (Position >= 0)
		{
			if (pInstrument->IsInverse)
				marginRatio = getLongMaintMarginRateValue(Position, pRate);
			else
				marginRatio = getLongMaintMarginRateValue(Position * price, pRate);
		}
		else
		{
			if (pInstrument->IsInverse)
				marginRatio = getShortMaintMarginRateValue(-Position, pRate);
			else
				marginRatio = getShortMaintMarginRateValue(-Position * price, pRate);
		}
		break;
	}
	default:
	{
		REPORT_EVENT(LOG_CRITICAL, "getMinMaintMarginRatio", "invalid position direction while get getMaxLeverage!");
	}
	}
	return marginRatio;
}


double getLongMarginRateValue(CMoneyType PositionCost, CMarginRate* pRate)
{
	if (pRate == NULL)
	{
		return 1.0;
	}
	double nOutPositon = PositionCost - pRate->LongRiskLimit.getValue();
	if (nOutPositon <= 0)
	{
		return pRate->LongMarginRatio;
	}
	else
	{
		int nStep = 0;
		if (pRate->LongRiskStep != 0)
			nStep = (int)ceil(nOutPositon / pRate->LongRiskStep.getValue());
		return pRate->LongMarginRatio + nStep * pRate->LongMarginStep.getValue();
	}
}

double getLongMaintMarginRateValue(CMoneyType nPosition, CMarginRate* pRate)
{
	if (pRate == NULL)
	{
		return 1.0;
	}
	double nOutPositon = nPosition - pRate->LongRiskLimit.getValue();
	if (nOutPositon <= 0)
	{
		return pRate->LongMaintMarginRatio;
	}
	else
	{
		int nStep = 0;
		if (pRate->LongRiskStep != 0)
			nStep = (int)ceil(nOutPositon / pRate->LongRiskStep.getValue());
		return pRate->LongMaintMarginRatio + nStep * pRate->LongMarginStep.getValue();
	}
}

double getShortMarginRateValue(CVolumeType nPosition, CMarginRate* pRate)
{
	if (pRate == NULL)
	{
		return 1.0;
	}
	double nOutPositon = nPosition - pRate->ShortRiskLimit.getValue();
	if (nOutPositon <= 0)
	{
		return pRate->ShortMarginRatio.getValue();
	}
	else
	{
		int nStep = 0;
		if (pRate->ShortRiskStep != 0)
			nStep = (int)ceil(nOutPositon / pRate->ShortRiskStep.getValue());

		return pRate->ShortMarginRatio.getValue() + nStep * pRate->ShortMarginStep.getValue();
	}
}

double getShortMaintMarginRateValue(CVolumeType nPosition, CMarginRate* pRate)
{
	if (pRate == NULL)
	{
		return 1.0;
	}
	double nOutPositon = nPosition - pRate->ShortRiskLimit.getValue();
	if (nOutPositon <= 0)
	{
		return pRate->ShortMaintMarginRatio.getValue();
	}
	else
	{
		int nStep = 0;
		if (pRate->ShortRiskStep != 0)
			nStep = (int)ceil(nOutPositon / pRate->ShortRiskStep.getValue());
		return pRate->ShortMaintMarginRatio.getValue() + nStep * pRate->ShortMarginStep.getValue();
	}
}


CMarginRate* getMarginRate(CMDB* m_pMDB, const char* MarginRateGroup, CAccount* pAccount)
{
	CMarginRate* pRate = NULL;

	if (pAccount == NULL)
		return NULL;

	if (!pAccount->MarginRateGrade.isNull())
	{
		CReadOnlyLongIndexIDType marginRateGrade = pAccount->MarginRateGrade.getString().c_str();
		pRate = m_pMDB->m_MarginRateFactory->findByMemberAndInstrument(marginRateGrade, MarginRateGroup);
		if (pRate != NULL)
			return pRate;
		pRate = m_pMDB->m_MarginRateFactory->findByMemberAndInstrument(marginRateGrade, defaultInstrumentID);
		if (pRate != NULL)
			return pRate;
	}

	if (pAccount->getMember() != NULL)
	{
		pRate = m_pMDB->m_MarginRateFactory->findByMemberAndInstrument(pAccount->getMember()->MemberID, MarginRateGroup);
		if (pRate != NULL)
			return pRate;

		pRate = m_pMDB->m_MarginRateFactory->findByMemberAndInstrument(pAccount->getMember()->MemberID, defaultInstrumentID);
		if (pRate != NULL)
			return pRate;

		if (!pAccount->getMember()->MarginRateGrade.isNull())
		{
			CReadOnlyLongIndexIDType marginRateGrade = pAccount->getMember()->MarginRateGrade.getString().c_str();
			pRate = m_pMDB->m_MarginRateFactory->findByMemberAndInstrument(marginRateGrade, MarginRateGroup);
			if (pRate != NULL)
				return pRate;
			pRate = m_pMDB->m_MarginRateFactory->findByMemberAndInstrument(marginRateGrade, defaultInstrumentID);
			if (pRate != NULL)
				return pRate;
		}
	}

	pRate = m_pMDB->m_MarginRateFactory->findByMemberAndInstrument(defaultMemberID, MarginRateGroup);
	if (pRate != NULL)
		return pRate;
	pRate = m_pMDB->m_MarginRateFactory->findByMemberAndInstrument(defaultMemberID, defaultInstrumentID);
	if (pRate != NULL)
		return pRate;

	return pRate;
}

CFee* getFee(CMDB* m_pMDB, CReadOnlyMatchRoleType& matchrole, CInstrument* pInstrument, CMember* pMember)
{
	if (NULL == pInstrument)
		return NULL;

	if (NULL == pMember)
		return NULL;

	CFee* pFee = getEachFee(m_pMDB, matchrole, pInstrument, pMember->MemberID);
	if (NULL != pFee)
		return pFee;

	if (!pMember->FeeGrade.isNull())
	{
		pFee = getEachFee(m_pMDB, matchrole, pInstrument, pMember->FeeGrade.getString().c_str());
		if (NULL != pFee)
			return pFee;
	}

	//// 检查user表的FeeGrade
	//CUser *pUser = m_pMDB->m_UserFactory->findByMemberID(MemberID);
	//if (pUser != NULL && pUser->FeeGrade.isNull())
	//{
	//	CFee *pFee = getEachFee(m_pMDB, matchrole, pInstrument, pUser->FeeGrade.getString().c_str());
	//	if (NULL != pFee)
	//		return pFee;
	//}

	pFee = getEachFee(m_pMDB, matchrole, pInstrument, defaultMemberID);
	if (NULL != pFee)
		return pFee;

	//这里再没有找到，就要报错了
	if (NULL == pFee)
		REPORT_EVENT(LOG_CRITICAL, "getTradeUnitFee", "Cann't Found Fee Rate:InstrumentID:[%s],MemberID:[%s]", pInstrument->InstrumentID.getValue(), pMember->MemberID.getValue());
	return pFee;
}

CFee* getEachFee(CMDB* m_pMDB, CReadOnlyMatchRoleType& matchrole, CInstrument* pInstrument, CLongIndexIDType MemberID)
{
	//使用特殊的会员号和客户号查找，此为特殊交易编码的设置
	CFee* pFee = m_pMDB->m_FeeFactory->findByMemberIDAndInstrument(MemberID, pInstrument->InstrumentID, matchrole);
	if (NULL != pFee)
		return pFee;

	pFee = m_pMDB->m_FeeFactory->findByMemberIDAndInstrument(MemberID, pInstrument->FeeGroup.getValue(), matchrole);
	if (NULL != pFee)
		return pFee;

	pFee = m_pMDB->m_FeeFactory->findByMemberIDAndInstrument(MemberID, defaultInstrumentID, matchrole);
	if (NULL != pFee)
		return pFee;

	return NULL;
}

//CPositionTypeType getPositionType(CMDB* m_pMDB, CLongIndexIDType MemberID)
//{
//	CPositionTypeType positionType;
//	positionType.clear();
//	CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(MemberID);
//
//	if (pMember != NULL)
//		positionType = pMember->DefaultPositionType;
//
//	/*if (positionType.isNull())
//		positionType = PT_None;*/
//	//if (positionType.isNull())
//	//{
//	//	positionType = PT_None;
//	//	CUser *pUser = m_pMDB->m_UserFactory->findByUserID(MemberID);
//	//	if (pUser != NULL)
//	//	{
//	//		positionType = pUser->DefaultPositionType;
//	//		if (positionType.isNull())
//	//			positionType = PT_None;
//	//	}
//	//}
//
//	if (positionType.isNull())
//		positionType = PT_Gross;
//
//	return positionType;
//}

//CPositionTypeType getMemberPositionType(CMDB* m_pMDB, CLongIndexIDType MemberID)
//{
//	CPositionTypeType positionType;
//	positionType.clear();
//	CMember *pMember = m_pMDB->m_MemberFactory->findByMemberID(MemberID);
//
//	if (pMember != NULL)
//		positionType = pMember->DefaultPositionType;
//
//	if (positionType.isNull())
//		positionType = PT_None;
//	return positionType;
//}

CPosiDirectionType getOrderPosiDirection(COrder* pOrder)
{
	if (pOrder->getInstrument() == NULL)
		pOrder->linkInstrument();

	return getPosiDirection(pOrder->getMember(), pOrder->getInstrument()->ProductClass, pOrder->Direction, pOrder->OffsetFlag);
}

CPosiDirectionType getTriggerOrderPosiDirection(CTriggerOrder* pTriggerOrder)
{
	if (pTriggerOrder->getInstrument() == NULL)
		pTriggerOrder->linkInstrument();

	return getPosiDirection(pTriggerOrder->getMember(), pTriggerOrder->getInstrument()->ProductClass, pTriggerOrder->Direction, pTriggerOrder->OffsetFlag);
}

CPosiDirectionType getPosiDirection(CMember* pMember, CProductClassType ProductClass, CDirectionType Direction, COffsetFlagType OffsetFlag)
{
	switch (ProductClass)
	{
	case PC_AccountExchange:
	case PC_AssetsExchange:
	case PC_Premium:
	{
		return PD_Long;
	}
	case PC_Margin:
	{
		switch (pMember->PositionType)
		{
		case PT_Net:
		{
			return PD_Net;
		}
		default:
		{
			//对于买卖方向有两种情况，开平标志有5种情况，需要分别讨论
			if (OffsetFlag == OF_Open)
			{
				if (Direction == PD_Long)
					return PD_Long;
				else
					return PD_Short;
			}
			else
			{
				if (Direction == PD_Long)
					return PD_Short;
				else
					return PD_Long;
			}
		}
		}
	}
	}
}

//CPosition* getPosition(CMDB* m_pMDB, CWriteableOrder *pOrder)
//{
//	if (pOrder->getPosition() != NULL)
//		return pOrder->getPosition();
//
//	pOrder->PosiDirection = getOrderPosiDirection(m_pMDB, pOrder);
//	return pOrder->linkPosition(m_pMDB->m_PositionFactory);
//}

CPosition* getPosition(CMDB* m_pMDB, CWriteableOrder* pOrder)
{
	if (pOrder->getPosition() != NULL)
		return pOrder->getPosition();
	pOrder->PosiDirection = getPosiDirection(pOrder->getMember(), pOrder->getInstrument()->ProductClass, pOrder->Direction, pOrder->OffsetFlag);

	CPosition* pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(pOrder->AccountID, pOrder->ExchangeID, pOrder->InstrumentID, pOrder->TradeUnitID, pOrder->PosiDirection);
	pOrder->linkPosition(pPosition);
	return pPosition;
}

//bool InitPosition(CMDB* m_pMDB, CWriteableOrder* pOrder, CWriteablePosition *pPosition)
//{
//	CInstrument *pInstrument = pOrder->getInstrument();
//	pPosition->init();
//	pPosition->ExchangeID = pInstrument->ExchangeID;
//	pPosition->InstrumentID = pInstrument->InstrumentID;
//	pPosition->ProductGroup = pInstrument->ProductGroup;
//	pPosition->MemberID = pOrder->MemberID;
//	pPosition->TradeUnitID = pOrder->TradeUnitID;
//	pPosition->PosiDirection = pOrder->PosiDirection;
//	pPosition->ProductID = pInstrument->ProductID;
//	pPosition->SettlementGroup = pInstrument->SettlementGroup;
//	if (pInstrument->IsInverse)
//		pPosition->Currency = pInstrument->BaseCurrency;
//	else
//		pPosition->Currency = pInstrument->ClearCurrency;
//
//	pPosition->linkInstrument(pInstrument);
//	pPosition->AccountID = pOrder->AccountID;
//	pPosition->linkAccount(m_pMDB->m_AccountFactory);
//	pPosition->TPTriggerPrice.clear();
//	pPosition->SLTriggerPrice.clear();
//
//	CMemberPosition *pMemberPosition = m_pMDB->m_MemberPositionFactory->findByMemberAndInstrument(pOrder->MemberID, pOrder->AccountID, pOrder->ExchangeID, pOrder->InstrumentID, pOrder->PosiDirection);
//	if (pMemberPosition != NULL)
//	{
//		pPosition->IsCrossMargin = pMemberPosition->DefaultCrossMargin;
//		pPosition->Leverage = pMemberPosition->DefaultLeverage;
//	}
//	else
//	{
//		CMemberPosition *pMemberPosition = m_pMDB->m_MemberPositionFactory->findByMemberAndInstrument(pOrder->MemberID, "", pOrder->ExchangeID, pOrder->InstrumentID, pOrder->PosiDirection);
//		if (pMemberPosition != NULL)
//		{
//			pPosition->IsCrossMargin = pMemberPosition->DefaultCrossMargin;
//			pPosition->Leverage = pMemberPosition->DefaultLeverage;
//		}
//		else
//		{
//			pPosition->Leverage = pOrder->getInstrument()->DefaultLeverage;
//			pPosition->IsCrossMargin = true;
//		}
//	}
//	return true;
//}

bool InitPosition(CMDB* m_pMDB, CErrorEngine* pErrorEngine, CInstrument* pInstrument, CWriteablePosition* pPosition, CLongIndexIDType MemberID, CIndexIDType TradeUnitID,
	CAccountIDType AccountID, CPosiDirectionType PosiDirection, CBoolType IsCrossMargin, CLeverageType Leverage)
{
	if (MemberID.isNull())
		return pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);
	if (AccountID.isNull())
		return pErrorEngine->reportError(ERROR_AccountNotFound);
	if (TradeUnitID.isNull())
		return pErrorEngine->reportError(ERROR_INVALID_VALUE, "TradeUnitIDIsNull");
	if (PosiDirection.isNull())
		return pErrorEngine->reportError(ERROR_INVALID_VALUE, "PosiDirectionIsNull");

	//pPosition->init();
	pPosition->ExchangeID = pInstrument->ExchangeID;
	pPosition->InstrumentID = pInstrument->InstrumentID;
	pPosition->m_pMDB = m_pMDB;
	//pPosition->ProductGroup = pInstrument->ProductGroup;
	pPosition->MemberID = MemberID;
	pPosition->TradeUnitID = TradeUnitID;
	pPosition->PosiDirection = PosiDirection;
	//	pPosition->ProductID = pInstrument->ProductID;
	pPosition->SettlementGroup = pInstrument->SettlementGroup;
	pPosition->AccountID = AccountID;
	pPosition->ClosePosition = 0;
	if (pPosition->PositionID.isNull())
		pPosition->PositionID = m_gOrderSequence.getNextValueByString();
	pPosition->ClearCurrency = pInstrument->ClearCurrency;
	pPosition->PriceCurrency = pInstrument->PriceCurrency;
	pPosition->linkInstrument(pInstrument);
	pPosition->TPTriggerPrice.clear();
	pPosition->SLTriggerPrice.clear();
	pPosition->IsCrossMargin = IsCrossMargin;
	pPosition->Leverage = Leverage;

	if (pPosition->IsCrossMargin.isNull() || pPosition->Leverage.isNull())
	{
		CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID);
		if (pMemberPosition != NULL)
		{
			if (pPosition->IsCrossMargin.isNull())
				pPosition->IsCrossMargin = pMemberPosition->DefaultCrossMargin;
			if (pPosition->Leverage.isNull())
				pPosition->Leverage = pMemberPosition->DefaultLeverage;
		}
		else
		{
			CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID);
			if (pMemberPosition != NULL)
			{
				if (pPosition->IsCrossMargin.isNull())
					pPosition->IsCrossMargin = pMemberPosition->DefaultCrossMargin;
				if (pPosition->Leverage.isNull())
					pPosition->Leverage = pMemberPosition->DefaultLeverage;
			}
		}
	}

	if (pPosition->Leverage.isNull())
	{
		CAccount* pAccount = pPosition->getAccount();
		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(pPosition->MemberID);
		CMarginRate* rate = getMarginRate(m_pMDB, pInstrument->MarginRateGroup.getValue(), pAccount);
		if (rate == NULL)
			pPosition->Leverage = 1;
		else
			pPosition->Leverage = getDefaultLvgWithMargin(m_pMDB, pInstrument, MemberID, AccountID, PosiDirection, rate);
	}

	if (pPosition->IsCrossMargin.isNull())
		pPosition->IsCrossMargin = true;

	//// 合仓情况下，仓位杠杆需要保持一致，如果反方向仓位已存在，直接取值作为新开仓位的杠杆
	//if (TradeUnitID.like(AccountID) && PosiDirection != PD_Net)
	//{
	//	CPosiDirectionType ReversePosiDirection = (PosiDirection == PD_Long ? PD_Short : PD_Long);
	//	CPosition *pReversePosition = m_pMDB->m_PositionFactory->findByTradeUnitAndInstrument(MemberID, TradeUnitID, pInstrument->ExchangeID, pInstrument->InstrumentID, ReversePosiDirection);
	//	if (pReversePosition != NULL)
	//	{
	//		pPosition->Leverage = pReversePosition->Leverage;
	//		pPosition->IsCrossMargin = pReversePosition->IsCrossMargin;
	//	}
	//}

	return true;
}

//CReadOnlyPriceType& getTradePrice(COrder *pNewOrder, COrder *pOldOrder, CPriceSourceType *pPriceSource)
//{
	//根据合约情况改为合适的成交价生成方式
//	switch (pNewOrder->getInstrument()->TradePriceMode)
//	{
//	case CP_MiddlePrice:
	//{
	//	///如果新报单是不是限价单或者老报单是组合派生报单，则使用先入价
	//	if ((pNewOrder->OrderPriceType != OPT_LimitPrice) || (pOldOrder->DeriveSource == DS_FromCombination))
	//	{
	//		if (pOldOrder->Direction == D_Buy)
	//			*pPriceSource = PS_Buy;
	//		else
	//			*pPriceSource = PS_Sell;
	//		return pOldOrder->Price;
	//	}
	//	///获取前一价
	//	CReadOnlyPriceType *pLastPrice = &(pOldOrder->getInstrument()->getMarketData()->LastPrice);
	//	if (pLastPrice->isNull())
	//	{
	//		///今天还没有成交，则采用昨收盘
	//		pLastPrice = &(pOldOrder->getInstrument()->getMarketData()->PreClosePrice);
	//	}
	//	if (pLastPrice->isNull())
	//	{
	//		if (pOldOrder->Direction == D_Buy)
	//			*pPriceSource = PS_Buy;
	//		else
	//			*pPriceSource = PS_Sell;
	//		///如果还是没有前一价，则使用先入价
	//		return pOldOrder->Price;
	//	}
	//	CReadOnlyPriceType *pBuyPrice, *pSellPrice;
	//	if (pNewOrder->Direction == D_Buy)
	//	{
	//		pBuyPrice = &(pNewOrder->Price);
	//		pSellPrice = &(pOldOrder->Price);
	//	}
	//	else
	//	{
	//		pBuyPrice = &(pOldOrder->Price);
	//		pSellPrice = &(pNewOrder->Price);
	//	}
	//	if (*pLastPrice > *pBuyPrice)
	//	{
	//		*pPriceSource = PS_Buy;
	//		return *pBuyPrice;
	//	}
	//	else if (*pLastPrice < *pSellPrice)
	//	{
	//		*pPriceSource = PS_Sell;
	//		return *pSellPrice;
	//	}
	//	else
	//	{
	//		*pPriceSource = PS_LastPrice;
	//		return *pLastPrice;
	//	}
	//	break;
	//}
	//case CP_OrderBookPrice:
	//default:
	//{
//		if (pOldOrder->Direction == D_Buy)
//			*pPriceSource = PS_Buy;
//		else
//			*pPriceSource = PS_Sell;
//		return pOldOrder->Price;
//	}
//	}


	// 	///如果新报单是不是限价单或者老报单是组合派生报单，则使用先入价
	// 	if ((pNewOrder->OrderPriceType!=OPT_LimitPrice)||(pOldOrder->OrderType==ORDT_DeriveFromCombination))
	// 	{
	// 		if (pOldOrder->Direction==D_Buy)
	// 		{
	// 			*pPriceSource=PS_Buy;
	// 		}
	// 		else
	// 		{
	// 			*pPriceSource=PS_Sell;
	// 		}
	// 		return pOldOrder->Price;
	// 	}
	// 	///否则使用中间价
	// 	else
	// 	{
	// 		///获取前一价
	// 		CReadOnlyPriceType *pLastPrice;
	// 
	// 		pLastPrice=&(pOldOrder->getInstrument()->getMarketData()->LastPrice);
	// 		if (pLastPrice->isNull())
	// 		{
	// 			///今天还没有成交，则采用昨收盘
	// 			pLastPrice=&(pOldOrder->getInstrument()->getMarketData()->PreClosePrice);
	// 		}
	// 		if (pLastPrice->isNull())
	// 		{
	// 			if (pOldOrder->Direction==D_Buy)
	// 			{
	// 				*pPriceSource=PS_Buy;
	// 			}
	// 			else
	// 			{
	// 				*pPriceSource=PS_Sell;
	// 			}
	// 			///如果还是没有前一价，则使用先入价
	// 			return pOldOrder->Price;
	// 		}
	// 		CReadOnlyPriceType *pBuyPrice,*pSellPrice;
	// 		if (pNewOrder->Direction==D_Buy)
	// 		{
	// 			pBuyPrice=&(pNewOrder->Price);
	// 			pSellPrice=&(pOldOrder->Price);
	// 		}
	// 		else
	// 		{
	// 			pBuyPrice=&(pOldOrder->Price);
	// 			pSellPrice=&(pNewOrder->Price);
	// 		}
	// 		if (*pLastPrice>*pBuyPrice)
	// 		{
	// 			*pPriceSource=PS_Buy;
	// 			return *pBuyPrice;
	// 		}
	// 		else if (*pLastPrice<*pSellPrice)
	// 		{
	// 			*pPriceSource=PS_Sell;
	// 			return *pSellPrice;
	// 		}
	// 		else
	// 		{
	// 			*pPriceSource=PS_LastPrice;
	// 			return *pLastPrice;
	// 		}
	// 	}
//}

bool needShowPrice(CMDB* m_pMDB, CTrade* pTrade)
{
	switch (pTrade->DeriveSource)
	{
	case DS_Normal:
	case DS_FromTriggerOrder:
	case DS_FromTriggerOrder_SL:
	case DS_FromTriggerOrder_TP:
		//case DS_FromOTCTrade_CFD:
	{
		return true;
	}
	case DS_FromOTCTrade_WashOnly:
	{
		return true;
	}
	default:
	{
		return false;
	}
	}
	return false;
}

//bool getLastKLine(CMDB* m_pMDB, CWriteableKLine *pKLine, CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, const char* pPeriod)
//{
//	char Period = '\0'; char sPeriod[4] = { 0 }; int i = 0;
//	while (pPeriod[i] != '\0')
//	{
//		if (pPeriod[i] <= '9')
//			sPeriod[i] = pPeriod[i];
//		else
//			Period = pPeriod[i];
//		i++;
//	}
//	if (i > 3)
//		return false;
//
//	sPeriod[i] = '\0';
//	char basePeriod[3];
//	basePeriod[0] = '1';
//	basePeriod[1] = Period;
//	basePeriod[2] = '\0';
//	int nSearchTimes = atoi(sPeriod);
//	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriod(ExchangeID, InstrumentID, basePeriod);
//	if (pLastKLine == NULL)
//	{
//		m_pMDB->m_LastKLineFactory->endFindByHisPeriod();
//		return false;
//	}
//
//	pKLine->Bar = pPeriod;
//	pKLine->KLineNo = pLastKLine->KLineNo.getValue() / nSearchTimes;
//
//	pKLine->ExchangeID = pLastKLine->ExchangeID;
//	pKLine->InstrumentID = pLastKLine->InstrumentID;
//	pKLine->UpdateTime = pLastKLine->UpdateTime;
//	pKLine->HighestPrice = pLastKLine->HighestPrice;
//	pKLine->LowestPrice = pLastKLine->LowestPrice;
//	pKLine->Turnover = pLastKLine->Turnover;
//	pKLine->Volume = pLastKLine->Volume;
//	pKLine->BeginTime = pLastKLine->BeginTime;
//	pKLine->OpenPrice = pLastKLine->OpenPrice;
//	pKLine->ClosePrice = pLastKLine->ClosePrice;
//	pKLine->LastTradeID = pLastKLine->LastTradeID;
//
//	if (pLastKLine->KLineNo.getValue() % nSearchTimes != 0)
//	{
//		pLastKLine = m_pMDB->m_LastKLineFactory->findNextByHisPeriod();
//		while (pLastKLine != NULL)
//		{
//			if (pLastKLine->HighestPrice > pKLine->HighestPrice)
//				pKLine->HighestPrice = pLastKLine->HighestPrice;
//			if (pLastKLine->LowestPrice < pKLine->LowestPrice)
//				pKLine->LowestPrice = pLastKLine->LowestPrice;
//			pKLine->Turnover += pLastKLine->Turnover;
//			pKLine->Volume += pLastKLine->Volume;
//			pKLine->BeginTime = pLastKLine->BeginTime;
//			pKLine->OpenPrice = pLastKLine->OpenPrice;
//			if (pLastKLine->KLineNo.getValue() % nSearchTimes == 0)
//				break;
//			pLastKLine = m_pMDB->m_LastKLineFactory->findNextByHisPeriod();
//		}
//	}
//	m_pMDB->m_LastKLineFactory->endFindByHisPeriod();
//	return true;
//}

//bool getLastKLineByBeginTime(CMDB* m_pMDB, CWriteableKLine *pKLine, CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, const char* pPeriod)
//{
//	char Period = '\0'; char sPeriod[4] = { 0 }; int i = 0;
//	while (pPeriod[i] != '\0')
//	{
//		if (pPeriod[i] <= '9')
//			sPeriod[i] = pPeriod[i];
//		else
//			Period = pPeriod[i];
//		i++;
//	}
//	if (i > 3)
//		return false;
//
//	sPeriod[i] = '\0';
//	char basePeriod[3];
//	basePeriod[0] = '1';
//	basePeriod[1] = Period;
//	basePeriod[2] = '\0';
//	int nSearchTimes = atoi(sPeriod);
//	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriodBeginTime(ExchangeID, InstrumentID, basePeriod);
//	if (pLastKLine == NULL)
//	{
//		m_pMDB->m_LastKLineFactory->endFindByHisPeriodBeginTime();
//		return false;
//	}
//
//	pKLine->Bar = pPeriod;
//	pKLine->KLineNo = GetKLineNo3(pLastKLine->BeginTime, pPeriod);
//	pKLine->SerialNo = pLastKLine->SerialNo.getValue() / nSearchTimes;
//
//	pKLine->ExchangeID = pLastKLine->ExchangeID;
//	pKLine->InstrumentID = pLastKLine->InstrumentID;
//	pKLine->UpdateTime = pLastKLine->UpdateTime;
//	pKLine->HighestPrice = pLastKLine->HighestPrice;
//	pKLine->LowestPrice = pLastKLine->LowestPrice;
//	pKLine->Turnover = pLastKLine->Turnover;
//	pKLine->Volume = pLastKLine->Volume;
//	pKLine->BeginTime = pLastKLine->BeginTime;
//	pKLine->OpenPrice = pLastKLine->OpenPrice;
//	pKLine->ClosePrice = pLastKLine->ClosePrice;
//	pKLine->LastTradeID = pLastKLine->LastTradeID;
//
//	if (pLastKLine->SerialNo.getValue() % nSearchTimes != 0)
//	{
//		pLastKLine = m_pMDB->m_LastKLineFactory->findNextByHisPeriodBeginTime();
//		while (pLastKLine != NULL)
//		{
//			if (pLastKLine->HighestPrice > pKLine->HighestPrice)
//				pKLine->HighestPrice = pLastKLine->HighestPrice;
//			if (pLastKLine->LowestPrice < pKLine->LowestPrice)
//				pKLine->LowestPrice = pLastKLine->LowestPrice;
//			pKLine->Turnover += pLastKLine->Turnover;
//			pKLine->Volume += pLastKLine->Volume;
//			pKLine->BeginTime = pLastKLine->BeginTime;
//			pKLine->OpenPrice = pLastKLine->OpenPrice;
//			if (pLastKLine->SerialNo.getValue() % nSearchTimes == 0)
//				break;
//			pLastKLine = m_pMDB->m_LastKLineFactory->findNextByHisPeriodBeginTime();
//		}
//	}
//	m_pMDB->m_LastKLineFactory->endFindByHisPeriodBeginTime();
//	return true;
//}

bool needInOrderBook(COrder* pOrder)
{
	if (pOrder->VolumeRemain <= 0.0)
		return false;

	switch (pOrder->DeriveSource)
	{
	case DS_OptionsExecution:
	case DS_FromOTCTrade_Liquidation:
	case DS_FromOTCTrade_ADL:
	case DS_FromOTCTrade_Private:
	case DS_FromOTCTrade_BlockTrade:
	case DS_FromOTCTrade_EFP:
	case DS_FromOTCTrade_WashOnly:
	case DS_FromAccountAction:
		return false;
	}

	switch (pOrder->OrderType)
	{
	case OT_FAK:
	case OT_FOK:
	case OT_FlashTrade:
		return false;
	}

	switch (pOrder->OrderStatus)
	{
	case OS_PartCanceled:
	case OS_AllCanceled:
	case OS_AllTraded:
		return false;
	}

	return pOrder->Tradable;
}

bool needTryTrade(COrder* pOrder)
{
	if (pOrder->VolumeRemain <= 0.0)
		return false;

	switch (pOrder->DeriveSource)
	{
	case DS_OptionsExecution:
	case DS_FromOTCTrade_Liquidation:
	case DS_FromOTCTrade_ADL:
	case DS_FromOTCTrade_Private:
	case DS_FromOTCTrade_BlockTrade:
	case DS_FromOTCTrade_EFP:
	case DS_FromOTCTrade_WashOnly:
		return false;
	}

	switch (pOrder->OrderStatus)
	{
	case OS_PartCanceled:
	case OS_AllCanceled:
	case OS_AllTraded:
		return false;
	}

	return pOrder->Tradable;
}

bool GenMarkPrice(CWriteableMarketData* pMarketData)
{
	if (pMarketData->MarkedPriceTotal.isNull())
		return false;

	if (pMarketData->MarkedPriceWeight == 0.0 || pMarketData->MarkedPriceWeight.isNull())
		return false;

	//pMarketDataField->MarkedPriceTotal.toTick(pInstrument->PriceTick.getValue());
	//pMarketDataField->MarkedPriceWeight.toTick(pInstrument->VolumeTick.getValue());
	double MarkedPrice = pMarketData->MarkedPriceTotal.getValue() / pMarketData->MarkedPriceWeight.getValue() + pMarketData->UnderlyingPrice;
	//pMarketDataField->MarkedPrice.toTick(pInstrument->PriceTick.getValue());

	////MarketPrice每次变化比上次限制在一定范围之类
	//if (!pMarketDataField->MarkedPrice.isValid() || pMarketDataField->MarkedPrice == 0.0 || pMarketDataField->MarkedPrice.isNull())
	//{
	//	pMarketDataField->MarkedPrice = MarkedPrice;
	//}
	//else
	//{
	//	if (MarkedPrice > pMarketDataField->MarkedPrice * 1.1)
	//		pMarketDataField->MarkedPrice = pMarketDataField->MarkedPrice*1.1;
	//	else if (MarkedPrice < pMarketDataField->MarkedPrice * 0.9)
	//		pMarketDataField->MarkedPrice = pMarketDataField->MarkedPrice*0.9;
	//	else
	//		pMarketDataField->MarkedPrice = MarkedPrice;
	//}

	//按照PriceTick进行四舍五入
	pMarketData->MarkedPrice = MarkedPrice;
	CInstrument* pInstrument = pMarketData->getInstrument();
	if (pInstrument != NULL && !pInstrument->PriceTick.isNull())
		pMarketData->MarkedPrice.toRoundTick(pInstrument->PriceTick.getValue());

	//REPORT_EVENT(LOG_DEBUG, "MarkedPrice", "[%s][%s]->[MarkedPrice:%lf][UnderlyingPrice:%lf][Total:%lf][Weight:%lf]=[%lf]",
	//	pMarketDataField->ExchangeID.getValue(), pMarketDataField->InstrumentID.getValue(),
	//	pMarketDataField->MarkedPrice.getValue(), pMarketDataField->UnderlyingPrice.getValue(),
	//	pMarketDataField->MarkedPriceTotal.getValue(), pMarketDataField->MarkedPriceWeight.getValue(),
	//	pMarketDataField->MarkedPriceTotal.getValue() / pMarketDataField->MarkedPriceWeight.getValue());

	return true;
}

//bool needSyncDB(CMDB* m_pMDB, CMember *pMember)
//{
//	if (pMember == NULL)
//		return false;
//	if (pMember->MemberType == PT_InnerMarketMaker)
//		return false;
//	else
//		return true;
//
//	return true;
//}

double getMaintMargin(CPosition* pPosition, CInstrument* pInstrument, double basePrice, CMarginRate* pRate)
{
	if (pRate == NULL)
		return 0.0;
	double MaintMarginRatio = getMinMaintMarginRatio(pPosition->Position, pInstrument, pRate, pPosition->PosiDirection);
	if (pInstrument->IsInverse)
	{
		if (basePrice == 0.0)
			return 0.0;
		return pPosition->Position * pInstrument->VolumeMultiple * MaintMarginRatio / basePrice;
	}
	else
	{
		return pPosition->Position * pInstrument->VolumeMultiple * MaintMarginRatio * basePrice;
	}
}

double getPositionProfit(CPosition* pPosition, CInstrument* pInstrument, double basePrice)
{
	double openPrice = fabs(pPosition->PositionCost.getValue() / pInstrument->VolumeMultiple.getValue() / pPosition->Position.getValue());
	if (pPosition->PosiDirection == PD_Long || pPosition->PosiDirection == PD_Net)
	{
		if (pInstrument->IsInverse)
		{
			if (basePrice == 0.0)
				return 0.0;
			return pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (openPrice - 1 / basePrice);
		}
		else
		{
			return pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (basePrice - openPrice);
		}
	}
	else
	{
		if (pInstrument->IsInverse)
		{
			if (basePrice == 0.0)
				return 0.0;
			return pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (1 / basePrice - openPrice);
		}
		else
		{
			return pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (openPrice - basePrice);
		}
	}
}

double getMarkedPrice(CCurrency* pCurrency)
{
	CCurrency* pThisCurrency = pCurrency;
	double price = 1.0;

	while (true)
	{
		if (pThisCurrency == NULL)
			return price;
		CMarketData* pMarketData = pThisCurrency->getMarketData();
		if (pMarketData == NULL)
			return price;
		price *= getMarkedPrice(pMarketData);
		pThisCurrency = pMarketData->getInstrument()->getClearCurrency();
	}
}

double getMarkedPrice(CMarketData* pMarketData)
{
	double price = 1.0;
	if (pMarketData != NULL)
	{
		price = pMarketData->MarkedPrice.getValue();
		if (price == DOUBLE_NaN)
			price = pMarketData->LastPrice.getValue();
		if (price == DOUBLE_NaN)
			price = 1.0;
	}
	return price;
}

double getHairCut(CAccount* pAccount, double price)
{
	CCurrency* pCurrency = pAccount->getCurrency();
	if (pAccount->Balance > 0.0)
	{
		double hairCut = pAccount->Balance.getValue() * pCurrency->HairCut.getValue();
		
		hairCut = hairCut * price;
		if (pCurrency->MaxValue < hairCut)
			hairCut = pCurrency->MaxValue.getValue();
		return hairCut;
	}
	if (pAccount->Balance < 0.0)
	{
		double hairCut = pAccount->Balance.getValue();
		hairCut = hairCut * price;
		return hairCut;
	}
	return 0.0;
}

void getRisk(CMDB* m_pMDB, CAccount* pAccount, double& PositionProfit, double& MaintMargin, double BasePrice)
{
	//这里如果findFirstByAccountID其他地方使用了，则需要使用iterator
	CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByAccountID(pAccount->AccountID);
	while (pPosition != NULL)
	{
		if (pPosition->Position == 0.0)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		if (!pPosition->IsCrossMargin)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		if (pPosition->ClearCurrency != pAccount->Currency)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		if (pPosition->SettlementGroup != pAccount->SettlementGroup)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		CInstrument* pInstrument = pPosition->getInstrument();
		if (pInstrument == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		if (pInstrument->ProductClass != PC_Margin)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		if (pInstrument->getMarketData() == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		CMarginRate* pRate = getMarginRate(m_pMDB, pInstrument->MarginRateGroup.getValue(), pAccount);
		MaintMargin += getMaintMargin(pPosition, pInstrument, BasePrice, pRate);
		PositionProfit += getPositionProfit(pPosition, pInstrument, BasePrice);
		pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
	}
}

double getPositionProfit(CMDB* m_pMDB, CAccount* pAccount)
{
	double PositionProfit = 0.0;
	//这里如果findFirstByAccountID其他地方使用了，则需要使用iterator
	CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByAccountID(pAccount->AccountID);
	while (pPosition != NULL)
	{
		if (pPosition->Position == 0.0)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		if (!pPosition->IsCrossMargin)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		if (pPosition->ClearCurrency != pAccount->Currency)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		if (pPosition->SettlementGroup != pAccount->SettlementGroup)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		CInstrument* pInstrument = pPosition->getInstrument();
		if (pInstrument == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		if (pInstrument->ProductClass != PC_Margin)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		if (pInstrument->getMarketData() == NULL)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		CPriceType basePrice = pInstrument->getMarketData()->MarkedPrice.getValue();
		if (basePrice.isNull())
			basePrice = pInstrument->getMarketData()->LastPrice.getValue();
		if (basePrice.isNull())
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}
		PositionProfit += getPositionProfit(pPosition, pInstrument, basePrice.getValue());
		pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
	}
	return PositionProfit;
}

bool checkCrossPositionProfit(CMDB* m_pMDB, CAccount* pAccount)
{
	double PositionProfit = 0.0;
	CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByAccountID(pAccount->AccountID);
	while (pPosition != NULL)
	{
		if (pPosition->Position == 0.0)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		if (!pPosition->IsCrossMargin)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		if (pPosition->ClearCurrency != pAccount->Currency)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		if (pPosition->SettlementGroup != pAccount->SettlementGroup)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		CInstrument* pInstrument = pPosition->getInstrument();
		if (pInstrument == NULL)
		{
			pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pPosition->ExchangeID, pPosition->InstrumentID);
			if (pInstrument == NULL)
			{
				pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
				continue;
			}
		}

		if (pInstrument->ProductClass != PC_Margin)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		CMarketData* pMarketData = pInstrument->getMarketData();
		if (pMarketData == NULL)
		{
			pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pPosition->ExchangeID, pPosition->InstrumentID);
			if (pMarketData == NULL)
			{
				pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
				continue;
			}
		}

		CPriceType basePrice = getMarkedPrice(pInstrument->getMarketData());
		if (pInstrument->IsInverse && basePrice == 0.0)
		{
			pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
			continue;
		}

		double openPrice = fabs(pPosition->PositionCost.getValue() / pInstrument->VolumeMultiple.getValue() / pPosition->Position.getValue());
		if (pPosition->PosiDirection == PD_Long || pPosition->PosiDirection == PD_Net)
		{
			if (pInstrument->IsInverse)
				PositionProfit = pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (openPrice - 1 / basePrice.getValue());
			else
				PositionProfit = pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (basePrice.getValue() - openPrice);
		}
		else
		{
			if (pInstrument->IsInverse)
				PositionProfit = pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (1 / basePrice.getValue() - openPrice);
			else
				PositionProfit = pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (openPrice - basePrice.getValue());
		}

		if (PositionProfit < 0.0)//出现亏损
		{
			return false;
		}
		pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
	}

	return true;
}

int GetSerialNo(CBarType Bar, double InsertTime)
{
	if (Bar == "1m")
	{
		//分钟：取UTC时间的起点作为标准
		//return (int)fmod(pKLine->BeginTime.getValue() / 60, 60);
		return (int)(InsertTime / (60 * 1000));
	}
	else if (Bar == "5m")
	{
		//return (int)fmod(pKLine->BeginTime.getValue() / (60 * 5), 12);
		return (int)(InsertTime / (5 * 60 * 1000));
	}
	else if (Bar == "1h")
	{
		//return (int)fmod((pKLine->BeginTime.getValue() - pKLine->TimeZone.getValue()) / (60 * 60), 24);
		return (int)(InsertTime / (60 * 60 * 1000));
	}
	else if (Bar == "1d")
	{
		return (int)floor(InsertTime / (24 * 60 * 60 * 1000));
	}
	else if (Bar == "1w")
	{
		//1970.01.04是星期日
		return (int)floor((InsertTime + 3 * 24 * 60 * 60 * 1000) / (7 * 24 * 60 * 60 * 1000));
	}
	else if (Bar == "1o")
	{
		//12月线就是年线
		time_t KLineTime = (time_t)(InsertTime / 1000);
		struct tm timeinfo = GetTM(&KLineTime);
		return timeinfo.tm_mon;
	}
	else
	{
		return 0;
	}
}

//int GetKLineNo(const CMilliSecondsType &BeginTime, const char *pPeriod)
//{
//	if (strcmp(pPeriod, "1m") == 0)
//	{
//		return int((BeginTime / 60) % g_nMaxLastKLineNo);
//	}
//	else if (strcmp(pPeriod, "5m") == 0)
//	{
//		return int((BeginTime / 300) % g_nMaxLastKLineNo);
//	}
//	else if (strcmp(pPeriod, "15m") == 0)
//	{
//		return int((BeginTime / 900) % g_nMaxLastKLineNo);
//	}
//	else if (strcmp(pPeriod, "30m") == 0)
//	{
//		return int((BeginTime / 1800) % g_nMaxLastKLineNo);
//	}
//	else if (strcmp(pPeriod, "1h") == 0)
//	{
//		return int((BeginTime / 3600) % g_nMaxLastKLineNo);
//	}
//	else
//		return BeginTime;
//}

CInstrumentIDType getMarkedInstrumentID(const CInstrumentIDType& InstrumentID)
{
	CInstrumentIDType MarkInstrumentID = "m";
	MarkInstrumentID += InstrumentID;
	return MarkInstrumentID;
}

CInstrumentIDType getUnderlyingInstrumentID(const CInstrumentIDType& InstrumentID)
{
	CInstrumentIDType MarkInstrumentID = "i";
	MarkInstrumentID += InstrumentID;
	return MarkInstrumentID;
}

//CCurrentTime& convertTimestampToCurrentTime(CMilliSecondsType timestamp)
//{
//	static CWriteableCurrentTime theCurrentTime;
//	theCurrentTime.init();
//	char *timeStr = convertTimestampToStr(timestamp);
//	vector<string> zoneStr = Txtsplit(timeStr, ",");
//	if (zoneStr.size() == 3)
//	{
//		theCurrentTime.ZoneDate = zoneStr[0].c_str();
//		theCurrentTime.ZoneTime = zoneStr[1].c_str();
//		theCurrentTime.ZoneDays = zoneStr[2].c_str();
//		theCurrentTime.CurrTime = timestamp;
//	}
//	return theCurrentTime;
//}

bool setTrade(CWriteableTrade* pTrade, COrder* pOrder, CReadOnlyPriceType& price, CReadOnlyVolumeType& volume, CReadOnlyDeriveSourceType& DeriveSource, char matchRole, bool bSelfTrade)
{
	pTrade->init();
	pTrade->ExchangeID = pOrder->getInstrument()->ExchangeID;
	pTrade->InstrumentID = pOrder->getInstrument()->InstrumentID;
	//pTrade->ProductGroup = pOrder->getInstrument()->ProductGroup;
	//	pTrade->SettlementGroup = pOrder->getInstrument()->SettlementGroup;
		//	pTrade->ProductID = pOrder->getInstrument()->ProductID;
	pTrade->Direction = pOrder->Direction;
	pTrade->Price = price;
	pTrade->Volume = volume;
	pTrade->DeriveSource = DeriveSource;
	//	pTrade->PriceSource = priceSource;
	pTrade->MatchRole = matchRole;

	pTrade->OrderID = pOrder->OrderID;
	pTrade->MemberID = pOrder->MemberID;
	//	pTrade->TradeUnitID = pOrder->TradeUnitID;
	pTrade->OffsetFlag = pOrder->OffsetFlag;
	pTrade->OrderPrice = pOrder->Price;
	pTrade->IsSelfTrade = bSelfTrade;
	pTrade->Remark = pOrder->Remark;
	return true;
}

//如果没有价格，说明设置价格的模块有问题，此时需要启动内部价格机制，使用指数价格进行调节
bool setOrderPrice(CMDB* pMDB, CErrorEngine* pErrorEngine, CWriteableOrder* pOrder)
{
	if (!pOrder->Price.isNull())
		return true;

	CMarketData* pMarketData = pMDB->m_MarketDataFactory->findByInstrumentID(pOrder->ExchangeID, pOrder->InstrumentID);
	if (pMarketData == NULL || pMarketData->UnderlyingPrice.isNull())
	{
		pErrorEngine->reportError(ERROR_UnderlyingPriceError);
		return false;
	}
	pOrder->linkInstrument(pMarketData->getInstrument());
	if (pOrder->Direction == D_Buy)
	{
		pOrder->Price = pMarketData->UnderlyingPrice * 1.01;
		pOrder->Price.toUpperTick(pMarketData->getInstrument()->PriceTick.getValue());
	}
	else
	{
		pOrder->Price = pMarketData->UnderlyingPrice * 0.99;
		pOrder->Price.toLowerTick(pMarketData->getInstrument()->PriceTick.getValue());
	}
	return true;
}

//COffsetFlagType getLogicOffsetFlag(CReadOnlyOffsetFlagType &offsetFlag)
//{
//	switch (offsetFlag)
//	{
//	case OF_Open:
//		return OF_Open;
//		//case OF_ClosePrevious:
//		//	return OF_ClosePrevious;
//	default:
//		return OF_Close;
//	}
//	return OF_Open;
//}

void LoanLoanInterest(CWriteableCurrency* pCurrency)
{
	double LoanRatio = 0.0;
	if (pCurrency->Balance != 0.0)
		LoanRatio = pCurrency->LoanUse / pCurrency->Balance;
	if (LoanRatio < pCurrency->LoanHighBase)
		pCurrency->LoanBorrow = pCurrency->LoanBase + pCurrency->LoanRate * LoanRatio;
	else
		pCurrency->LoanBorrow = pCurrency->LoanBase + pCurrency->LoanHighRate * LoanRatio;

	pCurrency->LoanDeposit = pCurrency->LoanBorrow * LoanRatio;
}

void InitTriggerOrderInfo(CMDB* m_pMDB, CTriggerOrder* pTriggerOrder, CLeverageType& Leverage, CBoolType& IsCrossMargin)
{
	CPosiDirectionType posiDirection = getTriggerOrderPosiDirection(pTriggerOrder);
	CPosition* pPosition = m_pMDB->m_PositionFactory->findByPositionID(pTriggerOrder->PositionID);
	if (pPosition != NULL)
	{
		Leverage = pPosition->Leverage;
		// 如果外部已确定全仓或逐仓，则忽略默认值
		if (IsCrossMargin.isNull())
			IsCrossMargin = pPosition->IsCrossMargin;
		return;
	}

	CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(pTriggerOrder->AccountID, pTriggerOrder->ExchangeID, pTriggerOrder->InstrumentID);
	if (pMemberPosition != NULL)
	{
		if (Leverage.isNull())
			Leverage = pMemberPosition->DefaultLeverage;
		// 如果外部已确定全仓或逐仓，则忽略默认值
		if (IsCrossMargin.isNull())
			IsCrossMargin = pMemberPosition->DefaultCrossMargin;
		return;
	}

	pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(pTriggerOrder->AccountID, pTriggerOrder->ExchangeID, pTriggerOrder->InstrumentID);
	if (pMemberPosition != NULL)
	{
		Leverage = pMemberPosition->DefaultLeverage;
		// 如果外部已确定全仓或逐仓，则忽略默认值
		if (IsCrossMargin.isNull())
			IsCrossMargin = pMemberPosition->DefaultCrossMargin;
		return;
	}

	CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pTriggerOrder->ExchangeID, pTriggerOrder->InstrumentID);
	if (pInstrument != NULL)
	{
		//Leverage = pInstrument->DefaultLeverage;
		// 如果外部已确定全仓或逐仓，则忽略默认值
		if (IsCrossMargin.isNull())
			IsCrossMargin = true;
		CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(pTriggerOrder->MemberID, pTriggerOrder->AccountID, pInstrument->SettlementGroup, pInstrument->ClearCurrency);
		CMarginRate* rate = getMarginRate(m_pMDB, pInstrument->MarginRateGroup.getValue(), pAccount);
		if (rate == NULL)
			Leverage = 1;
		else
			Leverage = getDefaultLvgWithMargin(m_pMDB, pInstrument, pTriggerOrder->MemberID, pTriggerOrder->AccountID, posiDirection, rate);
	}
}

//bool getAccountID(CMDB* m_pMDB, CErrorEngine *pErrorEngine, CLongIndexIDType MemberID, CAccountIDType& AccountID)
//{
//	if (!AccountID.isNull())
//		return true;
//
//	//如果还没有设置AccountID，只能到AccountID表里面找第一个作为AccountID
//	//但是需要使用哪个Account，还需要从Currency和Settlement里面找
//	CAccount* pAccount = m_pMDB->m_AccountFactory->findFirstByByMemberID(MemberID);
//	if (pAccount != NULL)
//	{
//		AccountID = pAccount->AccountID;
//		if (!AccountID.isNull())
//			return true;
//	}
//
//	return pErrorEngine->reportError(ERROR_AccountNotFound);
//}

//查看默认杠杠
CLeverageType queryDefaultLeverage(CMDB* m_pMDB, CInstrument* pInstrument, CLongIndexIDType MemberID, CAccountIDType AccountID, CIndexIDType TradeUnitID, CPosiDirectionType PosiDirection, CMarginRate* Rate)
{
	CPosition* pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID, TradeUnitID, PosiDirection);
	if (pPosition != NULL)
	{
		//剔除分仓已经全部为空的记录
		//if (pPosition->TradeUnitID.like(pPosition->AccountID) || (pPosition->Position + pPosition->LongFrozen + pPosition->ShortFrozen) > 0)
		//{
		//显示杠杆有仓位记录，直接返回 （为跟单需求更改）
		return pPosition->Leverage;
		//}
		//if (PosiDirection != PD_Net)
		//{
		//	// 不是异常的分仓仓位
		//	if ((pPosition->Position + pPosition->LongFrozen + pPosition->ShortFrozen) == 0)
		//		return pPosition->Leverage;
		//}
		//else
		//{
		//	if ((abs(pPosition->Position) + pPosition->LongFrozen + pPosition->ShortFrozen) == 0)
		//		return pPosition->Leverage;
		//}
	}

	CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID);
	if (pMemberPosition != NULL)
		return pMemberPosition->DefaultLeverage;

	return getDefaultLvgWithMargin(m_pMDB, pInstrument, MemberID, AccountID, PosiDirection, Rate);
	//return pInstrument->DefaultLeverage;
}

CLeverageType getDefaultLvgWithMargin(CMDB* m_pMDB, CInstrument* pInstrument, CLongIndexIDType MemberID, CAccountIDType AccountID, CPosiDirectionType PosiDirection, CMarginRate* Rate)
{
	if (Rate == NULL)
		return 1.0;
	if (Rate->DefaultLeverage.isNull() || Rate->DefaultLeverage == 0) {
		CRatioType minMarginRatio = getMinMarginRatio(0, 0, 0, pInstrument, Rate, PosiDirection);
		if (Rate->ValueMode == VM_Absolute)
		{
			return -minMarginRatio;
		}
		else
		{
			CLeverageType l = minMarginRatio.getInverse();
			return l.getInt();
		}
	}
	else
	{
		return Rate->DefaultLeverage;
	}
}

CLeverageType queryMaxLeverage(CMDB* m_pMDB, CInstrument* pInstrument, CLongIndexIDType MemberID, CAccountIDType AccountID, CIndexIDType TradeUnitID, CPosiDirectionType PosiDirection)
{
	CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(MemberID);
	CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(MemberID, AccountID, pInstrument->SettlementGroup, pInstrument->ClearCurrency);
	CMarginRate* rate = getMarginRate(m_pMDB, pInstrument->MarginRateGroup, pAccount);
	if (rate == NULL)
		return 1.0;
	CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID);
	if (pMemberPosition != NULL)
	{
		CRatioType minMarginRatio = 1.0;
		minMarginRatio = getMinMarginRatio(pMemberPosition->Position, pMemberPosition->LongFrozen, pMemberPosition->ShortFrozen, pInstrument, rate, PosiDirection);
		if (rate->ValueMode == VM_Absolute)
		{
			return -minMarginRatio;
		}
		else
		{
			return minMarginRatio.getInverse();
		}
	}
	else
	{
		pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID);
		if (pMemberPosition != NULL)
		{
			CRatioType minMarginRatio = 1.0;
			minMarginRatio = getMinMarginRatio(pMemberPosition->Position, pMemberPosition->LongFrozen, pMemberPosition->ShortFrozen, pInstrument, rate, PosiDirection);
			if (rate->ValueMode == VM_Absolute)
			{
				return -minMarginRatio;
			}
			else
			{
				return minMarginRatio.getInverse();
			}

		}
		else
		{
			//return pInstrument->DefaultLeverage;
			return getDefaultLvgWithMargin(m_pMDB, pInstrument, MemberID, AccountID, PosiDirection, rate);
		}
	}
	return 1.0;
}

//double queryDefaultMaxLeverage(CMDB* m_pMDB, CInstrument *pInstrument, CLongIndexIDType MemberID, CAccountIDType AccountID, CPosiDirectionType PosiDirection)
//{
//	CLeverageType maxleverage = 0.0, memmaxleverage = 0.0;
//	CPosition *pPosition = m_pMDB->m_PositionFactory->findFirstByMemberAndInstrument(MemberID, pInstrument->ExchangeID, pInstrument->InstrumentID);
//	while (pPosition != NULL)
//	{
//		if (pPosition->PosiDirection == PosiDirection && pPosition->AccountID == AccountID)
//		{
//			if (pPosition->Leverage > maxleverage)
//			{
//				maxleverage = pPosition->Leverage;
//			}
//		}
//		pPosition = m_pMDB->m_PositionFactory->findNextByMemberAndInstrument();
//	}
//	/*if (maxleverage != 0) {
//		return maxleverage;
//	}*/
//
//	CMemberPosition *pMemberPosition = m_pMDB->m_MemberPositionFactory->findFirstByMemberID(MemberID);
//	while (pMemberPosition != NULL)
//	{
//		if (pMemberPosition->PosiDirection == PosiDirection && pMemberPosition->AccountID == AccountID && pMemberPosition->InstrumentID == pInstrument->InstrumentID && pMemberPosition->ExchangeID == pInstrument->ExchangeID)
//		{
//			if (pMemberPosition->DefaultLeverage > memmaxleverage)
//			{
//				memmaxleverage = pMemberPosition->DefaultLeverage;
//			}
//		}
//		pMemberPosition = m_pMDB->m_MemberPositionFactory->findNextByMemberID();
//	}
//	if (memmaxleverage != 0) {
//		return memmaxleverage > maxleverage ? memmaxleverage : maxleverage;
//	}
//
//	pMemberPosition = m_pMDB->m_MemberPositionFactory->findFirstByMemberID(MemberID);
//	while (pMemberPosition != NULL)
//	{
//		if (pMemberPosition->PosiDirection == PosiDirection && pMemberPosition->AccountID == "" && pMemberPosition->InstrumentID == pInstrument->InstrumentID && pMemberPosition->ExchangeID == pInstrument->ExchangeID)
//		{
//			if (pMemberPosition->DefaultLeverage > maxleverage)
//			{
//				maxleverage = pMemberPosition->DefaultLeverage;
//			}
//		}
//		pMemberPosition = m_pMDB->m_MemberPositionFactory->findNextByMemberID();
//	}
//	if (memmaxleverage != 0) {
//		return memmaxleverage > maxleverage ? memmaxleverage : maxleverage;
//	}
//
//	if (maxleverage != 0) {
//		return maxleverage;
//	}
//
//	return pInstrument->DefaultLeverage;
//}

bool queryMarginAndLeverage(CMDB* m_pMDB, CErrorEngine* pErrorEngine, CInstrument* pInstrument, CLongIndexIDType MemberID, CAccountIDType AccountID, CIndexIDType TradeUnitID, CPosiDirectionType PosiDirection, CLeverageField* respl, CLeverageType Longl, CLeverageType Shortl, CLeverageType Netl)
{
	CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(MemberID);
	if (pMember == NULL)
		return pErrorEngine->reportError(ERROR_MEMBER_NOT_FOUND);

	if (AccountID.isNull())
		AccountID = MemberID;

	if (TradeUnitID.isNull())
		TradeUnitID = AccountID;

	CAccount* pAccount = m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(MemberID, AccountID, pInstrument->SettlementGroup, pInstrument->ClearCurrency);

	CMarginRate* rate = getMarginRate(m_pMDB, pInstrument->MarginRateGroup, pAccount);
	if (rate == NULL)
		return pErrorEngine->reportError(ERROR_CAN_NOT_FIND_MARGIN_RATE);
	CRatioType minMarginRatio = getMinMarginRatio(0, 0, 0, pInstrument, rate, PD_Long);
	CLeverageType L = minMarginRatio.getInverse();
	respl->DefaultMaxLeverage = L.getInt();

	//净仓合仓逻辑，分仓还要判断正负
	//if (pMember->PositionType == PT_Net)
	//{
	//	CLeverageType netMaxlev = 0;
	//	if (PosiDirection == 0)
	//	{
	//		respl->LongLeverage = queryDefaultLeverage(m_pMDB, pInstrument, MemberID, AccountID, TradeUnitID, PD_Net, rate);
	//		respl->ShortLeverage = respl->LongLeverage;
	//	}
	//	else {
	//		if (Netl < 1)
	//		{
	//			return pErrorEngine->reportError(ERROR_INVALID_VALUE, "Leverage should >= 1");
	//		}
	//		respl->ShortLeverage = Netl;
	//		respl->LongLeverage = Netl;
	//	}

	//	CPosition *pPosition = m_pMDB->m_PositionFactory->findFirstByMemberAndInstrument(MemberID, pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Net);
	//	while (pPosition != NULL)
	//	{
	//		//剔除异常新增分仓记录
	//		if (pPosition->Position == 0 && pPosition->LongFrozen == 0 && pPosition->ShortFrozen == 0)
	//		{
	//			pPosition = m_pMDB->m_PositionFactory->findNextByMemberAndInstrument();
	//			continue;
	//		}
	//		if (pPosition->Leverage > netMaxlev)
	//			netMaxlev = pPosition->Leverage;
	//		pPosition = m_pMDB->m_PositionFactory->findNextByByMemberInstrumentTradeUnitPD();
	//	}
	//	if (respl->LongLeverage > netMaxlev)
	//		netMaxlev = respl->LongLeverage;

	//	CVolumeType longv = 0, shortv = 0;
	//	CMemberPosition *pMemberPosition = m_pMDB->m_MemberPositionFactory->findByMemberAndInstrument(MemberID, AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Net);
	//	if (pMemberPosition != NULL)
	//	{
	//		if (pMemberPosition->Position >= 0)
	//		{
	//			longv = pMemberPosition->Position + pMemberPosition->LongFrozen;
	//			shortv = pMemberPosition->ShortFrozen;
	//		}
	//		else
	//		{
	//			longv = pMemberPosition->LongFrozen;
	//			shortv = abs(pMemberPosition->Position) + pMemberPosition->ShortFrozen;
	//		}
	//	}
	//	else
	//	{
	//		pMemberPosition = m_pMDB->m_MemberPositionFactory->findByMemberAndInstrument(MemberID, "", pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Net);
	//		if (pMemberPosition != NULL)
	//		{
	//			if (pMemberPosition->Position >= 0)
	//			{
	//				longv = pMemberPosition->Position + pMemberPosition->LongFrozen;
	//				shortv = pMemberPosition->ShortFrozen;
	//			}
	//			else
	//			{
	//				longv = pMemberPosition->LongFrozen;
	//				shortv = abs(pMemberPosition->Position) + pMemberPosition->ShortFrozen;
	//			}
	//		}
	//	}

	//	if (!TradeUnitID.isNull())
	//	{
	//		CPosition *pPosition = m_pMDB->m_PositionFactory->findByTradeUnitAndInstrument(MemberID, TradeUnitID, pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Net);
	//		if (pPosition != NULL)
	//			respl->IsCrossMargin = pPosition->IsCrossMargin;
	//	}

	//	if (pMemberPosition != NULL)
	//	{
	//		if(respl->IsCrossMargin.isNull())
	//			respl->IsCrossMargin = pMemberPosition->DefaultCrossMargin;
	//		CRatioType minMarginRatio = getMinMarginRatio(pMemberPosition->Position, pMemberPosition->LongFrozen, pMemberPosition->ShortFrozen, rate, PD_Net);
	//		respl->LongMaxLeverage = 1 / minMarginRatio.getValue();
	//		respl->ShortMaxLeverage = respl->LongMaxLeverage;
	//	}
	//	else
	//	{
	//		CRatioType minMarginRatio = getMinMarginRatio(0, 0, 0, rate, PD_Net);
	//		respl->LongMaxLeverage = 1 / minMarginRatio.getValue();
	//		respl->ShortMaxLeverage = respl->LongMaxLeverage;
	//	}

	//	if (respl->IsCrossMargin.isNull())
	//		respl->IsCrossMargin = true;

	//	respl->LongLevel = floor(MAX((1 / netMaxlev - rate->LongMarginRatio) / rate->LongMarginStep, 0)) + 1;
	//	respl->LongMaxVolume = rate->LongRiskLimit + (respl->LongLevel - 1)*rate->LongRiskStep;
	//	//respl->LongMaxLeverage = 1 / ((respl->LongLevel - 1)*rate->LongMarginStep + rate->LongMarginRatio);
	//	respl->LongLastVolume = respl->LongMaxVolume - longv;
	//	//respl->LongMaintRatio = getLongMaintMarginRateValue(longv, rate);

	//	respl->ShortLevel = respl->LongLevel;
	//	respl->ShortMaxVolume = rate->ShortRiskLimit + (respl->ShortLevel - 1)*rate->ShortRiskStep;
	//	//respl->ShortMaxLeverage = 1 / ((respl->ShortLevel - 1)*rate->ShortMarginStep + rate->ShortMarginRatio);
	//	respl->ShortLastVolume = respl->ShortMaxVolume - shortv;
	//	//respl->ShortMaintRatio = getShortMaintMarginRateValue(shortv, rate);
	//	return true;
	//}
	//else
	//{
	//	CLeverageType longMaxlev = 0, shortMaxlev = 0;
	//	if (PosiDirection == 0)
	//	{
	//		respl->LongLeverage = queryDefaultLeverage(m_pMDB, pInstrument, MemberID, AccountID, TradeUnitID, PD_Long, rate);
	//		respl->ShortLeverage = queryDefaultLeverage(m_pMDB, pInstrument, MemberID, AccountID, TradeUnitID, PD_Short, rate);
	//	}
	//	else
	//	{
	//		if (Shortl < 1 && Longl < 1)
	//		{
	//			return pErrorEngine->reportError(ERROR_INVALID_VALUE, "Leverage should >= 1");
	//		}
	//		respl->ShortLeverage = Shortl;
	//		respl->LongLeverage = Longl;
	//	}

	//	CPosition *pPosition = m_pMDB->m_PositionFactory->findFirstByMemberAndInstrumentAndPosiDirection(MemberID, AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Long);
	//	while (pPosition != NULL)
	//	{
	//		//剔除异常新增分仓0记录
	//		if ((pPosition->Position + pPosition->LongFrozen + pPosition->ShortFrozen) == 0)
	//		{
	//			pPosition = m_pMDB->m_PositionFactory->findNextByMemberAndInstrumentAndPosiDirection();
	//			continue;
	//		}
	//		if (pPosition->Leverage > longMaxlev)
	//			longMaxlev = pPosition->Leverage;
	//		pPosition = m_pMDB->m_PositionFactory->findNextByMemberAndInstrumentAndPosiDirection();
	//	}
	//	if (respl->LongLeverage > longMaxlev)
	//		longMaxlev = respl->LongLeverage;

	//	pPosition = m_pMDB->m_PositionFactory->findFirstByMemberAndInstrumentAndPosiDirection(MemberID, AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Short);
	//	while (pPosition != NULL)
	//	{
	//		//剔除异常新增分仓0记录
	//		if ((pPosition->Position + pPosition->LongFrozen + pPosition->ShortFrozen) == 0)
	//		{
	//			pPosition = m_pMDB->m_PositionFactory->findNextByMemberAndInstrumentAndPosiDirection();
	//			continue;
	//		}
	//		if (pPosition->Leverage > shortMaxlev)
	//			shortMaxlev = pPosition->Leverage;
	//		pPosition = m_pMDB->m_PositionFactory->findNextByMemberAndInstrumentAndPosiDirection();
	//	}
	//	if (respl->ShortLeverage > shortMaxlev)
	//		shortMaxlev = respl->ShortLeverage;

	//	//多空持有数量
	//	CVolumeType longv = 0;
	//	CMemberPosition *pMemberPosition = m_pMDB->m_MemberPositionFactory->findByMemberAndInstrument(MemberID, AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Long);
	//	if (pMemberPosition != NULL)
	//	{
	//		longv = pMemberPosition->Position + pMemberPosition->LongFrozen;
	//	}
	//	else
	//	{
	//		pMemberPosition = m_pMDB->m_MemberPositionFactory->findByMemberAndInstrument(MemberID, "", pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Long);
	//		if (pMemberPosition != NULL)
	//		{
	//			longv = pMemberPosition->Position + pMemberPosition->LongFrozen;
	//		}
	//	}

	//	if (!TradeUnitID.isNull())
	//	{
	//		CPosition *pPosition = m_pMDB->m_PositionFactory->findByTradeUnitAndInstrument(MemberID, TradeUnitID, pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Long);
	//		if (pPosition != NULL)
	//			respl->IsCrossMargin = pPosition->IsCrossMargin;
	//		if (respl->IsCrossMargin.isNull())
	//		{
	//			CPosition *pPosition = m_pMDB->m_PositionFactory->findByTradeUnitAndInstrument(MemberID, TradeUnitID, pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Short);
	//			if (pPosition != NULL)
	//				respl->IsCrossMargin = pPosition->IsCrossMargin;
	//		}
	//	}

	//	if (pMemberPosition != NULL)
	//	{
	//		if (respl->IsCrossMargin.isNull())
	//			respl->IsCrossMargin = pMemberPosition->DefaultCrossMargin;

	//		CRatioType minMarginRatio = getMinMarginRatio(pMemberPosition->Position, pMemberPosition->LongFrozen, pMemberPosition->ShortFrozen, rate, PD_Long);
	//		respl->LongMaxLeverage = 1 / minMarginRatio.getValue();
	//	}
	//	else
	//	{
	//		CRatioType minMarginRatio = getMinMarginRatio(0, 0, 0, rate, PD_Long);
	//		respl->LongMaxLeverage = 1 / minMarginRatio.getValue();
	//	}

	//	CVolumeType shortv = 0;
	//	pMemberPosition = m_pMDB->m_MemberPositionFactory->findByMemberAndInstrument(MemberID, AccountID, pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Short);
	//	if (pMemberPosition != NULL)
	//	{
	//		shortv = pMemberPosition->Position + pMemberPosition->ShortFrozen;
	//	}
	//	else
	//	{
	//		pMemberPosition = m_pMDB->m_MemberPositionFactory->findByMemberAndInstrument(MemberID, "", pInstrument->ExchangeID, pInstrument->InstrumentID, PD_Short);
	//		if (pMemberPosition != NULL)
	//		{
	//			shortv = pMemberPosition->Position + pMemberPosition->ShortFrozen;
	//		}
	//	}

	//	if (pMemberPosition != NULL)
	//	{
	//		if (respl->IsCrossMargin.isNull())
	//			respl->IsCrossMargin = pMemberPosition->DefaultCrossMargin;
	//		CRatioType minMarginRatio = getMinMarginRatio(pMemberPosition->Position, pMemberPosition->LongFrozen, pMemberPosition->ShortFrozen, rate, PD_Short);
	//		respl->ShortMaxLeverage = 1 / minMarginRatio.getValue();
	//	}
	//	else
	//	{
	//		CRatioType minMarginRatio = getMinMarginRatio(0, 0, 0, rate, PD_Short);
	//		respl->ShortMaxLeverage = 1 / minMarginRatio.getValue();
	//	}

	//	if (respl->IsCrossMargin.isNull())
	//		respl->IsCrossMargin = true;

	//	respl->LongLevel = floor(MAX((1 / longMaxlev - rate->LongMarginRatio) / rate->LongMarginStep, 0)) + 1;
	//	respl->LongMaxVolume = rate->LongRiskLimit + (respl->LongLevel - 1)*rate->LongRiskStep;
	//	//respl->LongMaxLeverage = 1 / ((respl->LongLevel - 1)*rate->LongMarginStep + rate->LongMarginRatio);
	//	respl->LongLastVolume = respl->LongMaxVolume - longv;
	//	//respl->LongMaintRatio = getLongMaintMarginRateValue(longv, rate);

	//	respl->ShortLevel = floor(MAX((1 / shortMaxlev - rate->ShortMarginRatio) / rate->ShortMarginStep, 0)) + 1;
	//	respl->ShortMaxVolume = rate->ShortRiskLimit + (respl->ShortLevel - 1)*rate->ShortRiskStep;
	//	//respl->ShortMaxLeverage = 1 / ((respl->ShortLevel - 1)*rate->ShortMarginStep + rate->ShortMarginRatio);
	//	respl->ShortLastVolume = respl->ShortMaxVolume - shortv;
	//	//respl->ShortMaintRatio = getShortMaintMarginRateValue(shortv, rate);
	//}
	return true;
}

double getSinglePositionProfit(CMDB* m_pMDB, CPosition* pPosition)
{
	if (pPosition == NULL)
		return 0.0;

	if (pPosition->Position == 0.0)
		return 0.0;

	/*if (!pPosition->IsCrossMargin)
	{
		pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
		continue;
	}*/

	CInstrument* pInstrument = pPosition->getInstrument();
	if (pInstrument == NULL)
	{
		pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pPosition->ExchangeID, pPosition->InstrumentID);
		if (pInstrument == NULL)
			return 0.0;
	}

	if (pInstrument->ProductClass != PC_Margin)
		return 0.0;

	CPriceType basePrice = getMarkedPrice(pInstrument->getMarketData());
	if (pInstrument->IsInverse && basePrice == 0.0)
		return 0.0;

	double PositionProfit = 0.0;
	double openPrice = fabs(pPosition->PositionCost.getValue() / pInstrument->VolumeMultiple.getValue() / pPosition->Position.getValue());
	if (pPosition->PosiDirection == PD_Long || pPosition->PosiDirection == PD_Net)
	{
		if (pInstrument->IsInverse)
			PositionProfit += pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (openPrice - 1 / basePrice.getValue());
		else
			PositionProfit += pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (basePrice.getValue() - openPrice);
	}
	else
	{
		if (pInstrument->IsInverse)
			PositionProfit += pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (1 / basePrice.getValue() - openPrice);
		else
			PositionProfit += pInstrument->VolumeMultiple.getValue() * pPosition->Position.getValue() * (openPrice - basePrice.getValue());
	}

	return PositionProfit;
}

double getPositionNewMargin(CMDB* m_pMDB, CPosition* pPosition, CInstrument* pInstrument, double newLeverage)
{
	if (pPosition == NULL)
		return 0.0;
	if (pInstrument == NULL || newLeverage == 0.0)
		return pPosition->UseMargin;

	CMoneyType newMargin = 0.0;
	switch (pInstrument->MarginPriceType)
	{
	case MPT_MarkedPrice:
	{
		if (pInstrument->getMarketData() == NULL)
		{
			pInstrument->linkMarketData();
			if (pInstrument->getMarketData() == NULL)
				return pPosition->UseMargin;
		}
		newMargin = abs(pPosition->Position * pInstrument->VolumeMultiple * pInstrument->getMarketData()->MarkedPrice) / newLeverage;
		break;
	}
	case MPT_LastPrice:
	{
		if (pInstrument->getMarketData() == NULL)
		{
			pInstrument->linkMarketData();
			if (pInstrument->getMarketData() == NULL)
				return pPosition->UseMargin;
		}
		newMargin = abs(pPosition->Position * pInstrument->VolumeMultiple * pInstrument->getMarketData()->LastPrice) / newLeverage;
		break;
	}
	default:
	{
		newMargin = pPosition->PositionCost / newLeverage;
	}
	}

	return newMargin;
}

//CIndexIDType getIsolateMergeTradeUnitID(CAccountIDType AccountID)
//{
//	// 逐仓合仓的TradeUnitID的特殊处理
//	char buffer[64];
//	sprintf(buffer, "%sHE", AccountID.getString().c_str());
//	return buffer;
//}


CPositionTypeType transPositionType(CPositionTypeType positionType)
{
	if (positionType.isNull())
	{
		positionType = PT_None;
	}
	switch (positionType)
	{
	case PT_Net:
		return PT_Net;
	default:
		return PT_Gross;
	}
}

//CMoneyType calcuateDeduceFee(CMDB* m_pMDB, CCurrencyType FeeDeduceCurreny, CRatioType FeeDeducePercent,
//	CCurrencyType FeeCurrency, CShortIndexIDType FeeSettlementGroup, CPriceType TradeFeeExchangeRate, CMoneyType RemainFee)
//{
//	if (FeeCurrency == FeeDeduceCurreny)
//	{
//		// 系统抵扣金币种抵扣
//		CCurrency* pFeeCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(FeeCurrency, FeeSettlementGroup);
//		if (pFeeCurrency == NULL || pFeeCurrency->getMarketData() == NULL || pFeeCurrency->getMarketData()->LastPrice.isNull())
//			return 0.0;
//		return RemainFee * FeeDeducePercent * TradeFeeExchangeRate / pFeeCurrency->getMarketData()->LastPrice.getValue();
//	}
//	else
//	{
//		// 用户手续费币种抵扣
//		CCurrency* pFeeCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(FeeCurrency, FeeSettlementGroup);
//		if (pFeeCurrency == NULL || pFeeCurrency->getMarketData() == NULL || pFeeCurrency->getMarketData()->LastPrice.isNull()
//			|| pFeeCurrency->FeeDisCount.isNull() || pFeeCurrency->FeeDisCount <= 0.0)
//			return 0.0;
//		return RemainFee * TradeFeeExchangeRate / (pFeeCurrency->getMarketData()->LastPrice.getValue() * pFeeCurrency->FeeDisCount);
//	}
//}

bool copyProfitRateIsValid(CRatioType copyProfitRate)
{
	if ((copyProfitRate >= 0.0) && (copyProfitRate < 1.0))
		return true;
	else
		return false;
}

CCFDRate* getCFDRate(CMDB* m_pMDB, const CMember* pMember, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyLongIndexIDType& TriggerOrderType)
{
	if (pMember == NULL)
		return NULL;
	if (!pMember->MemberType.isNull() && (pMember->MemberType == PT_InnerMarketMaker || pMember->MemberType == PT_OuterMarketMaker))
		return NULL;

	CCFDRate* pCFDRate = NULL;
	if (!pMember->CFDGrade.isNull())
	{
		CLongIndexIDType MemberID = pMember->CFDGrade.getString().c_str();
		pCFDRate = m_pMDB->m_CFDRateFactory->findByMemberAndInstrument(MemberID, InstrumentID, TriggerOrderType);
		if (NULL != pCFDRate)
			return pCFDRate;

		pCFDRate = m_pMDB->m_CFDRateFactory->findByMemberAndInstrument(MemberID, InstrumentID, defaultTriggerOrderType);
		if (NULL != pCFDRate)
			return pCFDRate;

		pCFDRate = m_pMDB->m_CFDRateFactory->findByMemberAndInstrument(MemberID, defaultInstrumentID, defaultTriggerOrderType);
		if (NULL != pCFDRate)
			return pCFDRate;
	}

	pCFDRate = m_pMDB->m_CFDRateFactory->findByMemberAndInstrument(defaultMemberID, InstrumentID, TriggerOrderType);
	if (NULL != pCFDRate)
		return pCFDRate;

	pCFDRate = m_pMDB->m_CFDRateFactory->findByMemberAndInstrument(defaultMemberID, InstrumentID, defaultTriggerOrderType);
	if (NULL != pCFDRate)
		return pCFDRate;

	pCFDRate = m_pMDB->m_CFDRateFactory->findByMemberAndInstrument(defaultMemberID, defaultInstrumentID, defaultTriggerOrderType);
	if (NULL != pCFDRate)
		return pCFDRate;

	return NULL;
}

bool getCFDPrice(CMDB* m_pMDB, CMember* pMember, CWriteableOrder* pOrder, CInstrument* pInstrument, CMarketData* pMarketData)
{
	if (pOrder->OrderPriceType != OPT_CFDPrice)
		return false;

	if (pMember == NULL)
		return false;
	if (!pMember->MemberType.isNull() && (pMember->MemberType == PT_InnerMarketMaker || pMember->MemberType == PT_OuterMarketMaker))
		return false;

	// 基准价，普通市价单->外部/场内最新价，条件单触发的市价单->触发价
	CPriceType CFDBasePrice;
	CLongIndexIDType TriggerOrderType = defaultTriggerOrderType;
	if (pOrder->DeriveSource == DS_Normal)
	{
		if (!pMarketData->TheoryPrice.isNull())
		{
			CFDBasePrice = pMarketData->TheoryPrice;
			pOrder->LastPriceByInsert = pMarketData->TheoryPrice;
		}
		else
			CFDBasePrice = pMarketData->LastPrice;
	}
	else
	{
		CFDBasePrice = pOrder->TriggerPrice;
		CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->findByOrderID(pOrder->RelatedOrderID);
		if (pTriggerOrder != NULL && !pTriggerOrder->TriggerOrderType.isNull())
			TriggerOrderType = pTriggerOrder->TriggerOrderType;
	}

	CCFDRate* rate = getCFDRate(m_pMDB, pMember, pOrder->ExchangeID, pOrder->InstrumentID, TriggerOrderType);
	if (rate != NULL)
	{
		CRatioType lastrate = (pOrder->Volume / rate->CompVolume * rate->CompRate).getValue();
		if (lastrate < rate->RateBegin) {
			lastrate = rate->RateBegin;
		}
		else if (lastrate > rate->RateEnd)
		{
			lastrate = rate->RateEnd;
		}
		if (lastrate > 0)
		{
			CPriceType dp = CFDBasePrice * lastrate;
			if (pOrder->Direction == D_Buy)
			{
				pOrder->Price = CFDBasePrice + dp;
				if (!pMarketData->UpperLimitPrice.isNull() && pOrder->Price > pMarketData->UpperLimitPrice)
				{
					pOrder->Price = pMarketData->UpperLimitPrice;
				}
			}
			else
			{
				pOrder->Price = CFDBasePrice - dp;
				if (!pMarketData->LowerLimitPrice.isNull() && pOrder->Price < pMarketData->LowerLimitPrice)
				{
					pOrder->Price = pMarketData->LowerLimitPrice;
				}
			}
			//格式化tick
			pOrder->Price.toTick(pInstrument->PriceTick.getValue());
			//PRINT_TO_STD("uid:%s,instrumetnID:%s,ratio:%s,price:%.4lf,dp:%.4lf,triprice:%.4lf", pOrder->MemberID.getValue(), pOrder->InstrumentID.getValue(), lastrate.getString().c_str(), pOrder->Price.getValue(), dp.getValue(), pOrder->TriggerPrice.getValue());
			return true;
		}
	}

	return false;
}
