#ifndef TRADING_TOOLS
#define TRADING_TOOLS

#include "errorDefine.h"
#include "mdb.h"
#include "DataType.h"
#include "CSequence.h"
#include "mdbStruct.h"
#include "mdbAction.h"
//#include "CPositionChangeList.h"

#define InitOrder(field)						\
	{											\
		if (field.VolumeMode.isNull())			\
			field.VolumeMode = VM_Absolute;		\
		if (field.OrderPriceType.isNull())		\
		{										\
			if(field.Price.isNull())			\
				field.OrderPriceType = OPT_TenLevelPrice;\
			else								\
				field.OrderPriceType = OPT_LimitPrice; \
		}										\
		if (field.OrderType.isNull())			\
			field.OrderType.init();				\
		if (field.BusinessType.isNull())		\
			field.BusinessType.init();			\
		if (field.OffsetFlag.isNull())			\
			field.OffsetFlag = OF_Open;			\
		if (field.Leverage == 0.0)				\
			field.Leverage.clear();				\
		if (field.AccountID == 0.0)				\
			field.AccountID = field.MemberID;	\
	}

//		if (!field.IsAutoCancle.isValid())		\
//			field.IsAutoCancle.init();			\

extern const char *INI_FILE_NAME;

///缺省的会员号
extern const CLongIndexIDType defaultMemberID;

///缺省的客户号
extern const CIndexIDType defaultTradeUnitID;

///缺省的合约号
extern const CExchangeIDType defaultExchangeID;

///缺省的合约号
extern const CInstrumentIDType defaultInstrumentID;

///缺省的TriggerOrderType
extern const CLongIndexIDType defaultTriggerOrderType;

/////缺省的产品号
//extern const CIndexIDType defaultProductID;
//
/////缺省的用户编码
//extern const CIndexIDType defaultUserID;

extern CLongIndexIDType g_sCFDTaker;

extern bool g_bKLineGap;

extern bool g_bDoubleMarketVolume;

extern int g_nSessionExpireTime;

extern string g_sBaseClearCurrency;

extern int g_nMaxTriggerOrders;

extern int g_nMaxOpenOrders;

extern int g_nMaxOpenPositions;

extern int g_nPushTimeSeconds;

extern int g_nMaxLastKLineNo;

extern int g_nDefaultAccessLimit;

extern bool g_bNoTradeFinishTo;

extern char g_cOrderPriceType;

extern UF_INT8 g_nBeginID; //从哪个序号开始做

extern UF_INT8 g_nEndID; //Redo到哪个序号结束

//系统运行状态，是否是redo模式
extern bool g_bOnlyRedo;

extern bool g_bSnapFirst;

extern bool g_bRoolback;

extern bool g_bCompare;

extern UF_INT8 g_nSequenceNo;

extern double g_dLoanInterestFeeRate;

//extern bool g_bNeedPushAll;

//各个系统决定是否snap，并且决定Sanp的名字
extern string g_bSnapName;

extern CSequence m_gAccountDetailSequence;
extern CSequence m_gTradeSequence;
extern CSequence m_gBusinessSequence;
extern CSequence m_gOrderSequence;
extern CSequence m_gSortSequence;

//typedef struct TpSession
//{
//	int frontID;
//	int sessionID;
//	int requestID;
//	const char* pToken;
//	const char* pUserID;
//}TpSession;

extern set<COrder*> gDeleteOrderSet;
extern set<CTriggerOrder*> gDeleteTriggerOrderSet;
extern set<CPosition*> gDeletePositionSet;

extern CAccountIDType g_sCopyProfitAccountID;

#define REDIS_MAX_COUNT 500

//extern const CDateType defaultDate;

///获取今天的交易日期
///@param	pMDB	指定的内存数据库
///@return	今天的交易日期，如果没有今天的日期，则返回缺省日期
//CReadOnlyDateType &getMDBDay(CMDB *pMDB);

///获取今天的自然日期
///@param	pMDB	指定的内存数据库
///@return	今天的自然日期，如果没有今天的日期，则返回缺省日期
//CReadOnlyDateType &getCalendarDay(CMDB *pMDB);

///获取今天的交易日期
///@param	pMDB	指定的内存数据库
///@param	pYear	返回的年份
///@param	pMonth	返回的月份
///@param	pDay	返回的日期
//void getMDBDay(CMDB *pMDB, int *pYear, int *pMonth, int *pDay);

//double getInverse(double beforeInverse);

///获取当前时间
///@param	pMDB	指定的内存数据库，如果不指定，则使用上次指定的内存数据库
///@return	当前时间
//CReadOnlyTimeType &getMDBTime(CMDB *pMDB = NULL);
//UF_INT8 getMDBTime(CMDB *pMDB);

///获取上次时间
///@param	pMDB	指定的内存数据库
///@return	上次时间
//CReadOnlyTimeType &getMDBOldTime(CMDB *pMDB);

//获取当前的精确时间
///@param	pMDB	指定的内存数据库
///@param	pTime	用于返回时间（精确到秒）
///@param	pMillisec	用于返回时间（精确到毫秒）
//void getMDBTime(CMDB *pMDB, CMilliSecondsType *pTime, CNumberType *pMillisec);

///设置当前时间
///@param	pMDB	指定的内存数据库
///@return	当前时间
void setMDBTime(CMDB *pMDB, CWriteableCurrentTime *pTime);
//void setMDBTime(CMDB *pMDB,const int nSeconds, CTransaction *pTransaction);

///得到指定时间中的秒钟数
///@param	theTime	指定时间
///@return	秒钟数
//int getSecond(CReadOnlyTimeType &theTime);

///得到时间表示的在今天中的秒钟数，即该时间是今天的第几秒
///@param	theTime	指定时间
///@return	今天的秒钟数
//int getSecondOfDay(CReadOnlyTimeType &theTime);


///获取相反的买卖方向
///@param	direction	原买卖方向
///@return	相反的买卖方向
CDirectionType getReverseDirection(CReadOnlyDirectionType direction);
CDirectionType getReverseDirection(CPosition *pPosition);


///设置合约是否可以交易的标志
///@param	pOrder	要设置标志的合约
//void setTradable(CWriteableOrder *pOrder);

///设置止损单状态
///@param	pOrder	指定的止损单
///@param	pMarketData	对应的行情
void setStopOrderStatus(CWriteableOrder *pOrder, CMarketData *pMarketData);

///设置报单的剩余数量和状态
///@param	pOrder	指定的报单
void setOrderStatus(CWriteableOrder *pOrder);

///获取某个合约的前结算价
///@param	pInstrument	指定的合约
CReadOnlyPriceType &getPreSettlementPrice(CInstrument *pInstrument);

///获取冻结保证金的价格
///@param	pInstrument	指定的合约
CReadOnlyPriceType &getFrozenMarginPrice(CMDB *pMDB, CInstrument *pInstrument, CReadOnlyPriceType& Price, CDirectionType Direction);

///获取占用保证金的价格
///@param	pInstrument	指定的合约
CReadOnlyPriceType &getMarginPrice(CInstrument *pInstrument, CTrade *pTrade);

///判断指定用户是否拥有指定权限
///@param	pMDB	内存数据库
///@param	UserID	用户编码
///@param	FunctionCode	功能编号
///@return	true表示有指定权限，false表示没有指定权限
//bool hasRight(CMDB *pMDB, CReadOnlyShortContentType &AllowID, CIndexIDType FunctionCode);

//CUser* checkLocalID(CMDB *pMDB, CErrorEngine *pErrorEngine, CReadOnlyLongIndexIDType &userID, CReadOnlyIndexIDType &LocalID, CLongIndexIDType* pMemberID = NULL);


bool CheckUserLocalID(CMDB *pMDB, CErrorEngine *pErrorEngine, CReadOnlyIndexIDType &LocalID, const char* UserID, CTransaction *pTransaction);
CMember* CheckMemberLocalID(CMDB *pMDB, CErrorEngine *pErrorEngine, CReadOnlyIndexIDType &LocalID, CLongIndexIDType &MemberID, CTransaction *pTransaction);
bool CheckAccountLocalID(CMDB *pMDB, CErrorEngine *pErrorEngine, CIndexIDType &LocalID, CLongIndexIDType &MemberID, CAccountIDType &AccountID, CShortIndexIDType &SettlementGroup, CCurrencyType &Currency, CTransaction *pTransaction);

CMember* CheckUserSessionLocalID(CMDB *pMDB, CErrorEngine *pErrorEngine, TSessionRef* pSession, CReadOnlyIndexIDType &LocalID, CTransaction *pTransaction, CLongIndexIDType &pMemberID, CShortIndexIDType* pAPPID = NULL);
CMember* CheckManagerSessionLocalID(CMDB *pMDB, CErrorEngine *pErrorEngine, TSessionRef* pSession, CReadOnlyIndexIDType &LocalID, CTransaction *pTransaction, CLongIndexIDType* pMemberID = NULL, CShortIndexIDType* pAPPID = NULL);
bool CheckManagerLocalID(CMDB *pMDB, CErrorEngine *pErrorEngine, TSessionRef* pSession, CReadOnlyIndexIDType &LocalID, CTransaction *pTransaction, CShortIndexIDType* pAPPID = NULL);

bool CheckManagerSession(CMDB *pMDB, CErrorEngine *pErrorEngine, TSessionRef* pSession);
bool CheckSession(CMDB *pMDB, CErrorEngine *pErrorEngine, TSessionRef* pSession, CLongIndexIDType &MemberID);
bool CheckManagerFront(CErrorEngine *pErrorEngine, const char* pToken);

void getRisk(CMDB* m_pMDB, CAccount* pAccount, double& PositionProfit, double& MaintMargin, double BasePrice);
double getPositionProfit(CMDB* m_pMDB, CAccount* pAccount);
double getHairCut(CAccount* pAccount, double price);

double getMarkedPrice(CMarketData* pMarketData);
double getMarkedPrice(CCurrency* pCurrency);

double getMaintMargin(CPosition* pPosition, CInstrument* pInstrument, double basePrice, CMarginRate* pRate);
double getPositionProfit(CPosition* pPosition, CInstrument* pInstrument, double basePrice);

///计算单个仓位的未实现盈亏
double getSinglePositionProfit(CMDB* m_pMDB, CPosition *pPosition);

///根据新的杠杆，计算仓位新的仓位保证金
double getPositionNewMargin(CMDB* m_pMDB, CPosition *pPosition, CInstrument *pInstrument, double newLeverage);

///判断全仓的所有仓位是否都盈利
///@param	pMDB	内存数据库
///@param	pAccount	用户账户
///@return	true表示所有仓位都盈利，false表示存在没有盈利的仓位
bool checkCrossPositionProfit(CMDB* m_pMDB, CAccount *pAccount);

///计算涨跌停板值
///@param	refPrice	参考价格
///@param	tick	最小价格变动单位
///@param	valueMode	取值方式
///@param	roundingMode	舍入方式
///@param	upperValue	上限
///@param	lowerValue	下限
///@param	pUpperLimit	返回的涨停板价
///@param	pLowerLimit	返回的跌停板价
///@return	true表示有涨跌停板值，false表示没有涨跌停板值
bool calculatePriceBanding(CReadOnlyPriceType &refPrice, CReadOnlyPriceType &tick,
	CReadOnlyValueModeType &valueMode, CReadOnlyRoundingModeType &roundingMode,
	CReadOnlyPriceType	&upperValue, CReadOnlyPriceType &lowerValue,
	CPriceType *pUpperLimit, CPriceType *pLowerLimit);

//重新计算资金信息
void applyAccountChange(CWriteableAccount *pAccount);
//清理资金
void applyAccountClear(CWriteableAccount *pAccount);

//保留资金盈亏算法
void applyReserveChange(CAccount *pOldAccount, CWriteableAccount *pNewAccount, CMoneyType Profit);

//bool validSession(CMDB *pMDB,CErrorEngine *pErrorEngine,CReadOnlyNumberType &sessionID,CReadOnlyNumberType &frontID,
//				  CIndexIDType *pMemberID,CReadOnlyIndexIDType *pUserID, bool managerOnly);

//double getMemberPosition(CPositionChangeList *pChangeList);
double getMinMarginRatio(CVolumeType Position, CVolumeType LongFrozen, CVolumeType ShortFrozen, CInstrument *pInstrument, CMarginRate *pRate, CPosiDirectionType posiDirection);
//double getPositionLeverage(CMarginRate *pRate, CInstrument *pInstrument, CPosition *pOldPosition, CPosiDirectionType PosiDirection, double minMarginRatio);
double getLongMarginRateValue(CVolumeType nPosition, CMarginRate* pRate);
double getShortMarginRateValue(CVolumeType nPosition, CMarginRate* pRate);

double getMinMaintMarginRatio(CVolumeType Position, CInstrument *pInstrument, CMarginRate *pRate, CPosiDirectionType posiDirection);
//double getPositionLeverage(CMarginRate *pRate, CInstrument *pInstrument, CPosition *pOldPosition, CPosiDirectionType PosiDirection, double minMarginRatio);
double getLongMaintMarginRateValue(CVolumeType nPosition, CMarginRate* pRate);
double getShortMaintMarginRateValue(CVolumeType nPosition, CMarginRate* pRate);

//COffsetFlagType getLogicOffsetFlag(CReadOnlyOffsetFlagType &offsetFlag);
//CPositionTypeType getPositionType(CMDB* m_pMDB, CLongIndexIDType MemberID);
//CPositionTypeType getMemberPositionType(CMDB* m_pMDB, CLongIndexIDType MemberID);
CPosiDirectionType getOrderPosiDirection(COrder *pOrder);
CPosiDirectionType getTriggerOrderPosiDirection(CTriggerOrder *pTriggerOrder);
CPosiDirectionType getPosiDirection(CMember* pMember, CProductClassType ProductClass, CDirectionType Direction, COffsetFlagType OffsetFlag);
CPosition* getPosition(CMDB* m_pMDB, CWriteableOrder *pOrder);

///获取保证金率
///@param	pInstrument	指定的合约
///@param	pTradeUnit	指定的会员客户
///@return	得到的保证金率，如果找不到，则返回NULL
CMarginRate *getMarginRate(CMDB* m_pMDB, const char* MarginRateGroup, CAccount* pAccount);

///获取手续费率
CFee* getFee(CMDB* m_pMDB, CReadOnlyMatchRoleType &matchrole, CInstrument *pInstrument, CMember* pMember);
CFee* getEachFee(CMDB* m_pMDB, CReadOnlyMatchRoleType &matchrole, CInstrument *pInstrument, CLongIndexIDType MemberID);

//CReadOnlyPriceType& getTradePrice(COrder *pNewOrder, COrder *pOldOrder, CPriceSourceType *pPriceSource);

//bool InitPosition(CMDB* m_pMDB, CWriteableOrder* pOrder, CWriteablePosition *pPosition);

bool InitPosition(CMDB* m_pMDB, CErrorEngine *pErrorEngine, CInstrument* pInstrument, CWriteablePosition *pPosition, CLongIndexIDType MemberID, CIndexIDType TradeUnitID,
	CAccountIDType AccountID, CPosiDirectionType PosiDirection, CBoolType IsCrossMargin, CLeverageType Leverage);

bool needShowPrice(CMDB* m_pMDB, CTrade *pTrade);

//bool getLastKLine(CMDB* m_pMDB, CWriteableKLine *pLastKLine, CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, const char* Period);
//bool getLastKLineByBeginTime(CMDB* m_pMDB, CWriteableKLine *pLastKLine, CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, const char* Period);

bool needInOrderBook(COrder *pOrder);

bool needTryTrade(COrder *pOrder);

bool GenMarkPrice(CWriteableMarketData *pMarketData);

//bool needSyncDB(CMDB* m_pMDB, CMember *pMember);

int GetSerialNo(CBarType Bar, double InsertTime);

//int GetKLineNo(const CMilliSecondsType &BeginTime, const char *pPeriod);
CInstrumentIDType getMarkedInstrumentID(const CInstrumentIDType &InstrumentID);
CInstrumentIDType getUnderlyingInstrumentID(const CInstrumentIDType &InstrumentID);

CCurrentTime& convertTimestampToCurrentTime(CMilliSecondsType timestamp);

bool setTrade(CWriteableTrade *pTrade, COrder *pOrder, CReadOnlyPriceType &price, CReadOnlyVolumeType &volume, CReadOnlyDeriveSourceType &DeriveSource, char matchRole, bool bSelfTrade);

bool setOrderPrice(CMDB* m_pMDB, CErrorEngine *pErrorEngine, CWriteableOrder *pOrder);

void InitTriggerOrderInfo(CMDB* m_pMDB, CTriggerOrder *pTriggerOrder, CLeverageType &Leverage, CBoolType &IsCrossMargin);

void LoanLoanInterest(CWriteableCurrency *pCurrencyn);

//bool getAccountID(CMDB* m_pMDB, CErrorEngine *pErrorEngine, CLongIndexIDType MemberID, CAccountIDType& AccountID);
//查看默认杠杠
CLeverageType queryDefaultLeverage(CMDB* m_pMDB, CInstrument *pInstrument, CLongIndexIDType MemberID, CAccountIDType AccountID, CIndexIDType TradeUnitID, CPosiDirectionType PosiDirection, CMarginRate *Rate);
//默认marginradio leverage
CLeverageType getDefaultLvgWithMargin(CMDB* m_pMDB, CInstrument* pInstrument, CLongIndexIDType MemberID, CAccountIDType AccountID, CPosiDirectionType PosiDirection, CMarginRate *Rate);

bool queryMarginAndLeverage(CMDB* m_pMDB, CErrorEngine *pErrorEngine, CInstrument *pInstrument, CLongIndexIDType MemberID, CAccountIDType AccountID, CIndexIDType TradeUnitID, CPosiDirectionType PosiDirection, CLeverageField *Cleverage, CLeverageType LongLeverage, CLeverageType ShortLeverage, CLeverageType NetLeverage);

CLeverageType queryMaxLeverage(CMDB* m_pMDB, CInstrument *pInstrument, CLongIndexIDType MemberID, CAccountIDType AccountID, CIndexIDType TradeUnitID, CPosiDirectionType PosiDirection);

//double queryDefaultMaxLeverage(CMDB* m_pMDB, CInstrument *pInstrument, CLongIndexIDType MemberID, CAccountIDType AccountID, CIndexIDType TradeUnitID, CPosiDirectionType PosiDirection);

//CIndexIDType getIsolateMergeTradeUnitID(CAccountIDType AccountID);
CPositionTypeType transPositionType(CPositionTypeType positionType);

//CMoneyType calcuateDeduceFee(CMDB* m_pMDB, CCurrencyType FeeDeduceCurreny, CRatioType FeeDeducePercent,
//	CCurrencyType FeeCurrency, CShortIndexIDType FeeSettlementGroup, CPriceType TradeExchangeRate, CMoneyType RemainFee);

class CServiceConfigTrigger : public CServiceConfigCommitTrigger
{
public:
	CServiceConfigTrigger() {};
	virtual ~CServiceConfigTrigger() {};
	virtual void commitAdd(CServiceConfig *pServiceConfig);
	virtual void commitUpdate(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pOldServiceConfig);
	virtual void commitRemove(CWriteableServiceConfig *pServiceConfig) {};
};

bool copyProfitRateIsValid(CRatioType copyProfitRate);

CCFDRate* getCFDRate(CMDB* m_pMDB, const CMember *pMember, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyLongIndexIDType& TriggerOrderType);
bool getCFDPrice(CMDB* m_pMDB, CMember *pMember, CWriteableOrder *pOrder, CInstrument *pInstrument, CMarketData *pMarketData);

#endif
