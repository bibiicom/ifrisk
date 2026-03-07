#ifndef _DATATYPE_H
#define _DATATYPE_H

#include "UFDataType.h"

/////////////////////////////////////////////////////////////////////////
///CCharType是一个自定义类型类
/////////////////////////////////////////////////////////////////////////
typedef CUFCharType CDefineTypeType;
typedef CReadOnlyUFCharType CReadOnlyDefineTypeType;

/////////////////////////////////////////////////////////////////////////
///CRangeIntType是一个年份类
/////////////////////////////////////////////////////////////////////////
typedef CUFRangeIntType<1900,9999> CYearType;
typedef const CUFRangeIntType<1900,9999> CReadOnlyYearType;
	
/////////////////////////////////////////////////////////////////////////
///CRangeIntType是一个月份类
/////////////////////////////////////////////////////////////////////////
typedef CUFRangeIntType<1,12> CMonthType;
typedef const CUFRangeIntType<1,12> CReadOnlyMonthType;
	
/////////////////////////////////////////////////////////////////////////
///CRangeIntType是一个日期类
/////////////////////////////////////////////////////////////////////////
typedef CUFRangeIntType<1,31> CDayType;
typedef const CUFRangeIntType<1,31> CReadOnlyDayType;
	
/////////////////////////////////////////////////////////////////////////
///CRangeIntType是一个单腿编号类
/////////////////////////////////////////////////////////////////////////
typedef CUFRangeIntType<0,4> CLegIDType;
typedef const CUFRangeIntType<0,4> CReadOnlyLegIDType;
	
/////////////////////////////////////////////////////////////////////////
///CRangeIntType是一个布尔型类
/////////////////////////////////////////////////////////////////////////
typedef CUFRangeIntType<0,1> CBoolType;
typedef const CUFRangeIntType<0,1> CReadOnlyBoolType;
	
/////////////////////////////////////////////////////////////////////////
///CIntType是一个数字类
/////////////////////////////////////////////////////////////////////////
typedef CUFIntType CNumberType;
typedef CReadOnlyUFIntType CReadOnlyNumberType;
	
/////////////////////////////////////////////////////////////////////////
///CFloatType是一个累积量(只加不减)类
/////////////////////////////////////////////////////////////////////////
typedef CUFFloatType<36,8> CCollectType;
typedef const CUFFloatType<36,8> CReadOnlyCollectType;
	
/////////////////////////////////////////////////////////////////////////
///CFloatType是一个汇总(加加减减)类
/////////////////////////////////////////////////////////////////////////
typedef CUFFloatType<36,6> CTotalType;
typedef const CUFFloatType<36,6> CReadOnlyTotalType;
	
/////////////////////////////////////////////////////////////////////////
///CFloatType是一个成本(为了计算更加精确)类
/////////////////////////////////////////////////////////////////////////
typedef CUFFloatType<36,15> CCostType;
typedef const CUFFloatType<36,15> CReadOnlyCostType;
	
/////////////////////////////////////////////////////////////////////////
///CFloatType是一个杠杆(不累加的精度控制)类
/////////////////////////////////////////////////////////////////////////
typedef CUFFloatType<20,8> CLeverageType;
typedef const CUFFloatType<20,8> CReadOnlyLeverageType;
	
/////////////////////////////////////////////////////////////////////////
///CFloatType是一个价格类
/////////////////////////////////////////////////////////////////////////
typedef CUFFloatType<28,15> CPriceType;
typedef const CUFFloatType<28,15> CReadOnlyPriceType;
	
/////////////////////////////////////////////////////////////////////////
///CFloatType是一个比率类
/////////////////////////////////////////////////////////////////////////
typedef CUFFloatType<26,10> CRatioType;
typedef const CUFFloatType<26,10> CReadOnlyRatioType;
	
/////////////////////////////////////////////////////////////////////////
///CFloatType是一个资金类
/////////////////////////////////////////////////////////////////////////
typedef CUFFloatType<38,10> CMoneyType;
typedef const CUFFloatType<38,10> CReadOnlyMoneyType;
	
/////////////////////////////////////////////////////////////////////////
///CFloatType是一个数量类
/////////////////////////////////////////////////////////////////////////
typedef CUFFloatType<38,10> CVolumeType;
typedef const CUFFloatType<38,10> CReadOnlyVolumeType;
	
/////////////////////////////////////////////////////////////////////////
///CLongType是一个精确到毫秒的时间戳类
/////////////////////////////////////////////////////////////////////////
typedef CUFLongType CMilliSecondsType;
typedef CReadOnlyUFLongType CReadOnlyMilliSecondsType;

/////////////////////////////////////////////////////////////////////////
///CLongType是一个大额数字类
/////////////////////////////////////////////////////////////////////////
typedef CUFLongType CLargeNumberType;
typedef CReadOnlyUFLongType CReadOnlyLargeNumberType;

/////////////////////////////////////////////////////////////////////////
///CLongType是一个秒类
/////////////////////////////////////////////////////////////////////////
typedef CUFLongType CSecondsType;
typedef CReadOnlyUFLongType CReadOnlySecondsType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个K线周期类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<3> CBarType;
typedef const CUFStringType<3> CReadOnlyBarType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个小短唯一标识类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<4> CSmallIndexIDType;
typedef const CUFStringType<4> CReadOnlySmallIndexIDType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个日期类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<8> CDateType;
typedef const CUFStringType<8> CReadOnlyDateType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个时间类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<8> CTimeType;
typedef const CUFStringType<8> CReadOnlyTimeType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个币种类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<12> CCurrencyType;
typedef const CUFStringType<12> CReadOnlyCurrencyType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个短唯一标识类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<8> CShortIndexIDType;
typedef const CUFStringType<8> CReadOnlyShortIndexIDType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个交易所ID类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<8> CExchangeIDType;
typedef const CUFStringType<8> CReadOnlyExchangeIDType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个IP地址类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<15> CIPAddressType;
typedef const CUFStringType<15> CReadOnlyIPAddressType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个唯一编号类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<20> CIndexIDType;
typedef const CUFStringType<20> CReadOnlyIndexIDType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个Mac地址类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<20> CMacAddressType;
typedef const CUFStringType<20> CReadOnlyMacAddressType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个名字类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<32> CNameType;
typedef const CUFStringType<32> CReadOnlyNameType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个标的ID类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<26> CInstrumentIDType;
typedef const CUFStringType<26> CReadOnlyInstrumentIDType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个长唯一编号类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<42> CLongIndexIDType;
typedef const CUFStringType<42> CReadOnlyLongIndexIDType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个密码类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<32> CPasswordType;
typedef const CUFStringType<32> CReadOnlyPasswordType;

/////////////////////////////////////////////////////////////////////////
///CStringType是一个资金账号类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<44> CAccountIDType;
typedef const CUFStringType<44> CReadOnlyAccountIDType;

/////////////////////////////////////////////////////////////////////////
///CVStringType是一个组播地址类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<64> CMultiIPAddressType;
typedef const CUFStringType<64> CReadOnlyMultiIPAddressType;

/////////////////////////////////////////////////////////////////////////
///CVStringType是一个长名字类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<64> CLongNameType;
typedef const CUFStringType<64> CReadOnlyLongNameType;

/////////////////////////////////////////////////////////////////////////
///CVStringType是一个超短内容类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<128> CShortContentType;
typedef const CUFStringType<128> CReadOnlyShortContentType;

/////////////////////////////////////////////////////////////////////////
///CVStringType是一个短内容类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<256> CContentType;
typedef const CUFStringType<256> CReadOnlyContentType;

/////////////////////////////////////////////////////////////////////////
///CVStringType是一个中内容类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<512> CMiddleContentType;
typedef const CUFStringType<512> CReadOnlyMiddleContentType;

/////////////////////////////////////////////////////////////////////////
///CVStringType是一个长内容类
/////////////////////////////////////////////////////////////////////////
typedef CUFStringType<1024> CLongContentType;
typedef const CUFStringType<1024> CReadOnlyLongContentType;

/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个交易用户类型类
/////////////////////////////////////////////////////////////////////////
///普通用户(订阅自己/可下自己订单)
#define UT_Trader '1'
///观察用户OB(订阅其他用户/不可下单)
#define UT_Observer '2'
///超级管理员(订阅所有/可给所有用户下单/可爆仓)
#define UT_SuperManager '5'

class CUserTypeType : public CUFCharType
{
public:							
	CUserTypeType(void)
	{
	}
	CUserTypeType(const CUserTypeType &v)
	{
		setValue(v.getValue());
	}
	CUserTypeType(const char v)
	{
		setValue(v);
	}			
	CUserTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CUserTypeType& operator=(const CUserTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CUserTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("125"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("125",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("125",ch))
			return true;
		else
			return false;
	}
};
typedef const CUserTypeType CReadOnlyUserTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个产品大类类
/////////////////////////////////////////////////////////////////////////
///保证金交易
#define PC_Margin '1'
///权利金交易
#define PC_Premium '2'
///资产转让（股票/基金/国债等）
#define PC_AssetsExchange '3'
///兑换交易（资金之间的互换）
#define PC_AccountExchange '4'
///组合
#define PC_Combination '5'
///行情
#define PC_Market '6'

class CProductClassType : public CUFCharType
{
public:							
	CProductClassType(void)
	{
	}
	CProductClassType(const CProductClassType &v)
	{
		setValue(v.getValue());
	}
	CProductClassType(const char v)
	{
		setValue(v);
	}			
	CProductClassType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CProductClassType& operator=(const CProductClassType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CProductClassType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("123456"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("123456",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("123456",ch))
			return true;
		else
			return false;
	}
};
typedef const CProductClassType CReadOnlyProductClassType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个产品类型类
/////////////////////////////////////////////////////////////////////////
///交割期货
#define PT_Delivery '1'
///永续、利率互换期货
#define PT_Swap '2'
///即期/日交割期货
#define PT_Spot '3'
///递延交割期货
#define PT_Delay '4'
///权证
#define PT_Warrants '5'
///美式期权
#define PT_AmericanOptions '6'
///欧式期权
#define PT_EuropeanOptions '7'
///币币交易
#define PT_CryptoExchange '8'
///外汇兑换
#define PT_ForeignExchange '9'
///股票
#define PT_Stock 'a'
///债券
#define PT_Bond 'b'
///基金
#define PT_ETF 'c'

class CProductTypeType : public CUFCharType
{
public:							
	CProductTypeType(void)
	{
	}
	CProductTypeType(const CProductTypeType &v)
	{
		setValue(v.getValue());
	}
	CProductTypeType(const char v)
	{
		setValue(v);
	}			
	CProductTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CProductTypeType& operator=(const CProductTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CProductTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("123456789abc"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("123456789abc",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("123456789abc",ch))
			return true;
		else
			return false;
	}
};
typedef const CProductTypeType CReadOnlyProductTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个业务类别类
/////////////////////////////////////////////////////////////////////////
///普通
#define BT_Normal '0'
///申赎
#define BT_PurchaseOrRedeem '1'
///行权
#define BT_OptionExercise '2'
///中立仓申报
#define BT_MiddlePositionApply '3'
///递延交割申报
#define BT_DeferDeliverApply '4'
///互换定单
#define BT_Swap '5'
///质押
#define BT_Mortgage '6'
///合并分拆
#define BT_MergeOrSplit '7'
///转股
#define BT_SwapEquity '8'
///回售回购
#define BT_SaleOrBuyBack '9'
///投票
#define BT_Vote 'a'
///融资融券
#define BT_MarginTrading 'b'
///期权执行
#define BT_OptionsExecution 'c'
///传递被平仓订单BusinessValue
#define BT_CloseOrderBusinessValue 'd'
///自定义11
#define BT_R11 'P'
///自定义10
#define BT_R10 'Q'
///自定义9
#define BT_R9 'R'
///连续交易
#define BT_R8 'S'
///自定义7
#define BT_R7 'T'
///画线委托
#define BT_R6 'U'
///限价市价
#define BT_R5 'V'
///条件
#define BT_R4 'W'
///止盈止损
#define BT_R3 'X'
///追踪出场
#define BT_R2 'Y'
///指标类型
#define BT_R1 'Z'

class CBusinessTypeType : public CUFCharType
{
public:							
	CBusinessTypeType(void)
	{
	}
	CBusinessTypeType(const CBusinessTypeType &v)
	{
		setValue(v.getValue());
	}
	CBusinessTypeType(const char v)
	{
		setValue(v);
	}			
	CBusinessTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CBusinessTypeType& operator=(const CBusinessTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CBusinessTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("0123456789abcdPQRSTUVWXYZ"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("0123456789abcdPQRSTUVWXYZ",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("0123456789abcdPQRSTUVWXYZ",ch))
			return true;
		else
			return false;
	}
};
typedef const CBusinessTypeType CReadOnlyBusinessTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个期权类型类
/////////////////////////////////////////////////////////////////////////
///非期权
#define OT_NotOptions '0'
///看涨
#define OT_CallOptions '1'
///看跌
#define OT_PutOptions '2'

class COptionsTypeType : public CUFCharType
{
public:							
	COptionsTypeType(void)
	{
	}
	COptionsTypeType(const COptionsTypeType &v)
	{
		setValue(v.getValue());
	}
	COptionsTypeType(const char v)
	{
		setValue(v);
	}			
	COptionsTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	COptionsTypeType& operator=(const COptionsTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	COptionsTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const COptionsTypeType CReadOnlyOptionsTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个标的交易状态类
/////////////////////////////////////////////////////////////////////////
///开盘前
#define IS_BeforeTrading '0'
///非交易
#define IS_NoTrading '1'
///连续交易
#define IS_Continous '2'
///集合竞价报单
#define IS_AuctionOrdering '3'
///集合竞价价格平衡
#define IS_AuctionBalance '4'
///集合竞价撮合
#define IS_AuctionMatch '5'
///收盘
#define IS_Closed '6'
///不活跃
#define IS_NotActive '7'

class CInstrumentStatusType : public CUFCharType
{
public:							
	CInstrumentStatusType(void)
	{
	}
	CInstrumentStatusType(const CInstrumentStatusType &v)
	{
		setValue(v.getValue());
	}
	CInstrumentStatusType(const char v)
	{
		setValue(v);
	}			
	CInstrumentStatusType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CInstrumentStatusType& operator=(const CInstrumentStatusType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CInstrumentStatusType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("01234567"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("01234567",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("01234567",ch))
			return true;
		else
			return false;
	}
};
typedef const CInstrumentStatusType CReadOnlyInstrumentStatusType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个买卖方向类
/////////////////////////////////////////////////////////////////////////
///买
#define D_Buy '0'
///卖
#define D_Sell '1'

class CDirectionType : public CUFCharType
{
public:							
	CDirectionType(void)
	{
	}
	CDirectionType(const CDirectionType &v)
	{
		setValue(v.getValue());
	}
	CDirectionType(const char v)
	{
		setValue(v);
	}			
	CDirectionType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CDirectionType& operator=(const CDirectionType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CDirectionType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("01"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("01",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("01",ch))
			return true;
		else
			return false;
	}
};
typedef const CDirectionType CReadOnlyDirectionType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个持仓类型类
/////////////////////////////////////////////////////////////////////////
///不设置
#define PT_None '0'
///综合持仓(双边持仓)
#define PT_Gross '1'
///净持仓(单边持仓)
#define PT_Net '2'

class CPositionTypeType : public CUFCharType
{
public:							
	CPositionTypeType(void)
	{
	}
	CPositionTypeType(const CPositionTypeType &v)
	{
		setValue(v.getValue());
	}
	CPositionTypeType(const char v)
	{
		setValue(v);
	}			
	CPositionTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CPositionTypeType& operator=(const CPositionTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CPositionTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CPositionTypeType CReadOnlyPositionTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个持仓大类类
/////////////////////////////////////////////////////////////////////////
///不设置
#define PC_None '0'
///保证金交易
#define PC_Risk '1'
///资产
#define PC_Assets '3'
///持币成本
#define PC_AccountCost '4'

class CPositionClassType : public CUFCharType
{
public:							
	CPositionClassType(void)
	{
	}
	CPositionClassType(const CPositionClassType &v)
	{
		setValue(v.getValue());
	}
	CPositionClassType(const char v)
	{
		setValue(v);
	}			
	CPositionClassType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CPositionClassType& operator=(const CPositionClassType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CPositionClassType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("0134"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("0134",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("0134",ch))
			return true;
		else
			return false;
	}
};
typedef const CPositionClassType CReadOnlyPositionClassType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个成员类型类
/////////////////////////////////////////////////////////////////////////
///普通
#define PT_Normal 'a'
///内部做市商
#define PT_InnerMarketMaker 'b'
///系统手续费使用
#define PT_Fee 'c'
///风控使用
#define PT_Risk 'd'
///运营使用
#define PT_ExchangeUse 'e'
///运营保留资金使用
#define PT_Reserve 'f'
///外部做市商
#define PT_OuterMarketMaker 'g'

class CMemberTypeType : public CUFCharType
{
public:							
	CMemberTypeType(void)
	{
	}
	CMemberTypeType(const CMemberTypeType &v)
	{
		setValue(v.getValue());
	}
	CMemberTypeType(const char v)
	{
		setValue(v);
	}			
	CMemberTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CMemberTypeType& operator=(const CMemberTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CMemberTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("abcdefg"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("abcdefg",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("abcdefg",ch))
			return true;
		else
			return false;
	}
};
typedef const CMemberTypeType CReadOnlyMemberTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个持仓多空方向类
/////////////////////////////////////////////////////////////////////////
///多头
#define PD_Long '0'
///空头
#define PD_Short '1'
///净
#define PD_Net '2'

class CPosiDirectionType : public CUFCharType
{
public:							
	CPosiDirectionType(void)
	{
	}
	CPosiDirectionType(const CPosiDirectionType &v)
	{
		setValue(v.getValue());
	}
	CPosiDirectionType(const char v)
	{
		setValue(v);
	}			
	CPosiDirectionType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CPosiDirectionType& operator=(const CPosiDirectionType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CPosiDirectionType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CPosiDirectionType CReadOnlyPosiDirectionType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个取值方式类
/////////////////////////////////////////////////////////////////////////
///百分比
#define VM_Percentage '0'
///绝对值
#define VM_Absolute '1'

class CValueModeType : public CUFCharType
{
public:							
	CValueModeType(void)
	{
	}
	CValueModeType(const CValueModeType &v)
	{
		setValue(v.getValue());
	}
	CValueModeType(const char v)
	{
		setValue(v);
	}			
	CValueModeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CValueModeType& operator=(const CValueModeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CValueModeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("01"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("01",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("01",ch))
			return true;
		else
			return false;
	}
};
typedef const CValueModeType CReadOnlyValueModeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个流动性基准价方式类
/////////////////////////////////////////////////////////////////////////
///买一和买一中间值
#define SPT_Bid1Ask1 '0'
///标记价
#define SPT_MarkedPrice '1'
///最新价
#define SPT_LastPrice '2'

class CLiquidityPriceTypeType : public CUFCharType
{
public:							
	CLiquidityPriceTypeType(void)
	{
	}
	CLiquidityPriceTypeType(const CLiquidityPriceTypeType &v)
	{
		setValue(v.getValue());
	}
	CLiquidityPriceTypeType(const char v)
	{
		setValue(v);
	}			
	CLiquidityPriceTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CLiquidityPriceTypeType& operator=(const CLiquidityPriceTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CLiquidityPriceTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CLiquidityPriceTypeType CReadOnlyLiquidityPriceTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个舍入方式类
/////////////////////////////////////////////////////////////////////////
///舍出
#define RM_Out '1'
///四舍五入
#define RM_Round '2'
///舍入
#define RM_In '3'
///截断
#define RM_Trunc '4'

class CRoundingModeType : public CUFCharType
{
public:							
	CRoundingModeType(void)
	{
	}
	CRoundingModeType(const CRoundingModeType &v)
	{
		setValue(v.getValue());
	}
	CRoundingModeType(const char v)
	{
		setValue(v);
	}			
	CRoundingModeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CRoundingModeType& operator=(const CRoundingModeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CRoundingModeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("1234"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("1234",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("1234",ch))
			return true;
		else
			return false;
	}
};
typedef const CRoundingModeType CReadOnlyRoundingModeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个帐号操作类型类
/////////////////////////////////////////////////////////////////////////
///入金
#define AAT_Deposit '1'
///出金
#define AAT_Withdraw '2'
///冻结资金
#define AAT_FrozenMoney '3'
///释放冻结资金
#define AAT_FreeFrozenMoney '4'
///释放冻结资金,同时出金
#define AAT_FrozenToWithdraw '5'
///内部转账
#define AAT_InnerTransfer '6'
///设置保留资金限制
#define AAT_SetReserveLimit '7'
///切换保留资金类型
#define AAT_SetReserveType '8'
///保留资金激活失效
#define AAT_SetReserveActive '9'
///使用订单冻结资金
#define AAT_FrozenMoneyByOrder 'a'
///切换是否可以出金
#define AAT_SetCanReduce 'b'

class CAccountActionTypeType : public CUFCharType
{
public:							
	CAccountActionTypeType(void)
	{
	}
	CAccountActionTypeType(const CAccountActionTypeType &v)
	{
		setValue(v.getValue());
	}
	CAccountActionTypeType(const char v)
	{
		setValue(v);
	}			
	CAccountActionTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CAccountActionTypeType& operator=(const CAccountActionTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CAccountActionTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("123456789ab"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("123456789ab",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("123456789ab",ch))
			return true;
		else
			return false;
	}
};
typedef const CAccountActionTypeType CReadOnlyAccountActionTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个资金渠道类
/////////////////////////////////////////////////////////////////////////
///银行
#define CC_Bank '1'
///支付宝
#define CC_Alipay '2'
///微信
#define CC_WeChat '3'
///区块链
#define CC_BlockChain '4'

class CCapitalChannelType : public CUFCharType
{
public:							
	CCapitalChannelType(void)
	{
	}
	CCapitalChannelType(const CCapitalChannelType &v)
	{
		setValue(v.getValue());
	}
	CCapitalChannelType(const char v)
	{
		setValue(v);
	}			
	CCapitalChannelType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CCapitalChannelType& operator=(const CCapitalChannelType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CCapitalChannelType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("1234"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("1234",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("1234",ch))
			return true;
		else
			return false;
	}
};
typedef const CCapitalChannelType CReadOnlyCapitalChannelType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个持仓操作类型类
/////////////////////////////////////////////////////////////////////////
///调整杠杆倍数
#define PAT_ChangeLeverage '1'
///增加保证金
#define PAT_AddMargin '2'
///减少保证金
#define PAT_CutMargin '3'
///全仓逐仓模式调换
#define PAT_ChangeCross '4'
///调整不超过最大杠杆倍数
#define PAT_ChangeMaxLeverage '5'
///调整默认杠杆倍数
#define PAT_ChangeDefaultLeverage '6'
///调整默认全逐仓
#define PAT_ChangeDefaultCross '7'
///调整成本价
#define PAT_ChangeCostPrice '8'
///全平
#define PAT_CloseAll 'a'
///最大平仓
#define PAT_CloseMax 'b'
///清理之后才能切换的全逐仓切换
#define PAT_ChangeAllCross 'c'

class CPositionActionTypeType : public CUFCharType
{
public:							
	CPositionActionTypeType(void)
	{
	}
	CPositionActionTypeType(const CPositionActionTypeType &v)
	{
		setValue(v.getValue());
	}
	CPositionActionTypeType(const char v)
	{
		setValue(v);
	}			
	CPositionActionTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CPositionActionTypeType& operator=(const CPositionActionTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CPositionActionTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("12345678abc"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("12345678abc",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("12345678abc",ch))
			return true;
		else
			return false;
	}
};
typedef const CPositionActionTypeType CReadOnlyPositionActionTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个报单价格条件类
/////////////////////////////////////////////////////////////////////////
///限价(手动指定Price值)
#define OPT_LimitPrice '0'
///任意价(Price按最高最低限价取值)
#define OPT_AnyPrice '1'
///最优价/对手价(Price按对方1档价取值)
#define OPT_BestPrice '2'
///五档价(Price按对方5档价取值)
#define OPT_FiveLevelPrice '3'
///十档价(类同上)
#define OPT_TenLevelPrice '4'
///最优Marker价(Price按本方1档价或其+1Tick取值)
#define OPT_BestMakerPrice '7'
///Price按委托额Cost和订单簿情况确定值
#define OPT_ByCost '8'
///保底点差价:成交价格Price不会劣于系统设置差价成交
#define OPT_CFDPrice '9'

class COrderPriceTypeType : public CUFCharType
{
public:							
	COrderPriceTypeType(void)
	{
	}
	COrderPriceTypeType(const COrderPriceTypeType &v)
	{
		setValue(v.getValue());
	}
	COrderPriceTypeType(const char v)
	{
		setValue(v);
	}			
	COrderPriceTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	COrderPriceTypeType& operator=(const COrderPriceTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	COrderPriceTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("01234789"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("01234789",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("01234789",ch))
			return true;
		else
			return false;
	}
};
typedef const COrderPriceTypeType CReadOnlyOrderPriceTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个开平标志类
/////////////////////////////////////////////////////////////////////////
///开仓
#define OF_Open '0'
///平仓
#define OF_Close '1'
///强平
#define OF_ForceClose '2'
///平今
#define OF_CloseToday '3'
///平昨
#define OF_ClosePrevious '4'
///全平
#define OF_CloseAll '5'
///指定订单平仓
#define OF_CloseOrderID '6'
///指定成交平仓
#define OF_CloseTradeID '7'
///最大平仓
#define OF_CloseMax '8'

class COffsetFlagType : public CUFCharType
{
public:							
	COffsetFlagType(void)
	{
	}
	COffsetFlagType(const COffsetFlagType &v)
	{
		setValue(v.getValue());
	}
	COffsetFlagType(const char v)
	{
		setValue(v);
	}			
	COffsetFlagType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	COffsetFlagType& operator=(const COffsetFlagType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	COffsetFlagType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012345678"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012345678",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012345678",ch))
			return true;
		else
			return false;
	}
};
typedef const COffsetFlagType CReadOnlyOffsetFlagType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个报单状态类
/////////////////////////////////////////////////////////////////////////
///未设置状态
#define OS_None '0'
///全部成交
#define OS_AllTraded '1'
///部分成交未撤单
#define OS_PartTraded '2'
///部分成交已撤单
#define OS_PartCanceled '3'
///未成交未撤单
#define OS_NoTraded '4'
///无成交已撤单
#define OS_AllCanceled '6'

class COrderStatusType : public CUFCharType
{
public:							
	COrderStatusType(void)
	{
	}
	COrderStatusType(const COrderStatusType &v)
	{
		setValue(v.getValue());
	}
	COrderStatusType(const char v)
	{
		setValue(v);
	}			
	COrderStatusType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	COrderStatusType& operator=(const COrderStatusType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	COrderStatusType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012346"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012346",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012346",ch))
			return true;
		else
			return false;
	}
};
typedef const COrderStatusType CReadOnlyOrderStatusType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个触发报单状态类
/////////////////////////////////////////////////////////////////////////
///未设置状态
#define TS_None '0'
///活跃(运行)状态
#define TS_Active '1'
///已经触发
#define TS_TriggerSuccess '2'
///触发失败
#define TS_TriggerFailed '3'
///撤单
#define TS_Canceled '4'

class CTriggerStatusType : public CUFCharType
{
public:							
	CTriggerStatusType(void)
	{
	}
	CTriggerStatusType(const CTriggerStatusType &v)
	{
		setValue(v.getValue());
	}
	CTriggerStatusType(const char v)
	{
		setValue(v);
	}			
	CTriggerStatusType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CTriggerStatusType& operator=(const CTriggerStatusType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CTriggerStatusType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("01234"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("01234",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("01234",ch))
			return true;
		else
			return false;
	}
};
typedef const CTriggerStatusType CReadOnlyTriggerStatusType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个业务来源类
/////////////////////////////////////////////////////////////////////////
///正常
#define DS_Normal '0'
///报价衍生
#define DS_FromQuote '1'
///期权执行
#define DS_OptionsExecution '2'
///组合衍生
#define DS_FromCombination '3'
///场外成交强平衍生
#define DS_FromOTCTrade_Liquidation '4'
///场外成交大宗交易衍生
#define DS_FromOTCTrade_BlockTrade '5'
///场外成交期转现衍生
#define DS_FromOTCTrade_EFP '6'
///场外成交只刷单衍生
#define DS_FromOTCTrade_WashOnly '7'
///触发订单衍生
#define DS_FromTriggerOrder '8'
///止损触发订单衍生
#define DS_FromTriggerOrder_SL '9'
///止盈触发订单衍生
#define DS_FromTriggerOrder_TP 'a'
///资金操作衍生
#define DS_FromAccountAction 'b'
///仓位合并衍生
#define DS_FromOTCTrade_PositionMerge 'c'
///CFD订单衍生
#define DS_FromOTCTrade_CFD 'd'
///场外成交ADL衍生
#define DS_FromOTCTrade_ADL 'e'
///回购手续费不能是币
#define DS_FromOTCTrade_Private 'f'

class CDeriveSourceType : public CUFCharType
{
public:							
	CDeriveSourceType(void)
	{
	}
	CDeriveSourceType(const CDeriveSourceType &v)
	{
		setValue(v.getValue());
	}
	CDeriveSourceType(const char v)
	{
		setValue(v);
	}			
	CDeriveSourceType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CDeriveSourceType& operator=(const CDeriveSourceType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CDeriveSourceType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("0123456789abcdef"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("0123456789abcdef",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("0123456789abcdef",ch))
			return true;
		else
			return false;
	}
};
typedef const CDeriveSourceType CReadOnlyDeriveSourceType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个订单类型类
/////////////////////////////////////////////////////////////////////////
///普通订单
#define OT_Normal '0'
///(FillAndKill/IOC)立即完成并且剩余撤销
#define OT_FAK '1'
///(FillOrKill)立即完成全部或者最小订单要求否则全部撤销
#define OT_FOK '2'
///需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly)
#define OT_PostOnly '3'
///不需要撮合直接成交
#define OT_FlashTrade '4'

class COrderTypeType : public CUFCharType
{
public:							
	COrderTypeType(void)
	{
	}
	COrderTypeType(const COrderTypeType &v)
	{
		setValue(v.getValue());
	}
	COrderTypeType(const char v)
	{
		setValue(v);
	}			
	COrderTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	COrderTypeType& operator=(const COrderTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	COrderTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("01234"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("01234",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("01234",ch))
			return true;
		else
			return false;
	}
};
typedef const COrderTypeType CReadOnlyOrderTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个触发的订单类型类
/////////////////////////////////////////////////////////////////////////
///未定义
#define TOT_None '0'
///持仓止盈止损
#define TOT_PositionTPSL '1'
///订单止盈止损
#define TOT_OrderTPSL '2'
///条件单
#define TOT_Condition '3'
///追踪(跟踪)订单
#define TOT_Trailing '4'
///其他算法交易
#define TOT_Algo '5'
///指标类型
#define TOT_Indicator '6'
///指标类型止盈止损
#define TOT_IndicatorTPSL '7'
///跟单类型
#define TOT_CopyTrade '8'
///划线下单类型
#define TOT_LineOrder '9'
///其他类型1
#define TOT_Other1 'a'
///其他类型2
#define TOT_Other2 'b'
///其他类型3
#define TOT_Other3 'c'
///其他类型4
#define TOT_Other4 'd'
///其他类型5
#define TOT_Other5 'e'

class CTriggerOrderTypeType : public CUFCharType
{
public:							
	CTriggerOrderTypeType(void)
	{
	}
	CTriggerOrderTypeType(const CTriggerOrderTypeType &v)
	{
		setValue(v.getValue());
	}
	CTriggerOrderTypeType(const char v)
	{
		setValue(v);
	}			
	CTriggerOrderTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CTriggerOrderTypeType& operator=(const CTriggerOrderTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CTriggerOrderTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("0123456789abcde"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("0123456789abcde",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("0123456789abcde",ch))
			return true;
		else
			return false;
	}
};
typedef const CTriggerOrderTypeType CReadOnlyTriggerOrderTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个数据库操作类型类
/////////////////////////////////////////////////////////////////////////
///插入
#define DAT_Insert '1'
///修改
#define DAT_Update '2'
///删除
#define DAT_Delete '3'
///插入或者更新
#define DAT_InsUpd '4'
///初始化
#define DAT_Init '5'

class CDBActionTypeType : public CUFCharType
{
public:							
	CDBActionTypeType(void)
	{
	}
	CDBActionTypeType(const CDBActionTypeType &v)
	{
		setValue(v.getValue());
	}
	CDBActionTypeType(const char v)
	{
		setValue(v);
	}			
	CDBActionTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CDBActionTypeType& operator=(const CDBActionTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CDBActionTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("12345"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("12345",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("12345",ch))
			return true;
		else
			return false;
	}
};
typedef const CDBActionTypeType CReadOnlyDBActionTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个操作标志类
/////////////////////////////////////////////////////////////////////////
///删除
#define AF_Delete '1'
///挂起
#define AF_Suspend '2'
///激活
#define AF_Active '3'
///修改
#define AF_Modify '4'
///触发
#define AF_Trigger '5'
///增减
#define AF_Change '6'

class CActionFlagType : public CUFCharType
{
public:							
	CActionFlagType(void)
	{
	}
	CActionFlagType(const CActionFlagType &v)
	{
		setValue(v.getValue());
	}
	CActionFlagType(const char v)
	{
		setValue(v);
	}			
	CActionFlagType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CActionFlagType& operator=(const CActionFlagType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CActionFlagType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("123456"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("123456",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("123456",ch))
			return true;
		else
			return false;
	}
};
typedef const CActionFlagType CReadOnlyActionFlagType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个交易权限类
/////////////////////////////////////////////////////////////////////////
///可以交易
#define TR_Allow '0'
///只能平仓
#define TR_CloseOnly '1'
///不能交易
#define TR_Forbidden '2'

class CTradingRightType : public CUFCharType
{
public:							
	CTradingRightType(void)
	{
	}
	CTradingRightType(const CTradingRightType &v)
	{
		setValue(v.getValue());
	}
	CTradingRightType(const char v)
	{
		setValue(v);
	}			
	CTradingRightType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CTradingRightType& operator=(const CTradingRightType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CTradingRightType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CTradingRightType CReadOnlyTradingRightType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个交易模式类
/////////////////////////////////////////////////////////////////////////
///价格优先时间优先
#define TM_PriceTime '0'
///价格优先按比例分配
#define TM_Prorata '1'
///只能和做市商报价成交
#define TM_OnlyQuote '2'
///没有撮合
#define TM_NoMatch '3'

class CTradingModelType : public CUFCharType
{
public:							
	CTradingModelType(void)
	{
	}
	CTradingModelType(const CTradingModelType &v)
	{
		setValue(v.getValue());
	}
	CTradingModelType(const char v)
	{
		setValue(v);
	}			
	CTradingModelType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CTradingModelType& operator=(const CTradingModelType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CTradingModelType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("0123"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("0123",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("0123",ch))
			return true;
		else
			return false;
	}
};
typedef const CTradingModelType CReadOnlyTradingModelType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个成交角色类
/////////////////////////////////////////////////////////////////////////
///不区分
#define MR_None '0'
///主动成交方
#define MR_Taker '1'
///被动成交方
#define MR_Maker '2'
///集合竞价
#define MR_Auction '3'
///交割
#define MR_Delivery '4'
///场外交易
#define MR_OTC '5'

class CMatchRoleType : public CUFCharType
{
public:							
	CMatchRoleType(void)
	{
	}
	CMatchRoleType(const CMatchRoleType &v)
	{
		setValue(v.getValue());
	}
	CMatchRoleType(const char v)
	{
		setValue(v);
	}			
	CMatchRoleType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CMatchRoleType& operator=(const CMatchRoleType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CMatchRoleType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012345"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012345",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012345",ch))
			return true;
		else
			return false;
	}
};
typedef const CMatchRoleType CReadOnlyMatchRoleType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个成交价来源类
/////////////////////////////////////////////////////////////////////////
///无
#define PS_None '0'
///前成交价
#define PS_LastPrice '1'
///买委托价
#define PS_Buy '2'
///卖委托价
#define PS_Sell '3'

class CPriceSourceType : public CUFCharType
{
public:							
	CPriceSourceType(void)
	{
	}
	CPriceSourceType(const CPriceSourceType &v)
	{
		setValue(v.getValue());
	}
	CPriceSourceType(const char v)
	{
		setValue(v);
	}			
	CPriceSourceType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CPriceSourceType& operator=(const CPriceSourceType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CPriceSourceType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("0123"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("0123",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("0123",ch))
			return true;
		else
			return false;
	}
};
typedef const CPriceSourceType CReadOnlyPriceSourceType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个OTC类型类
/////////////////////////////////////////////////////////////////////////
///强平
#define OTCT_Liquidation '0'
///大宗交易
#define OTCT_BlockTrade '1'
///期转现
#define OTCT_EFP '2'
///只刷单
#define OTCT_WashOnly '3'
///仓位合并
#define OTCT_PositionMerge '4'
///差价交易
#define OTCT_CFD '5'
///ADL减仓
#define OTCT_ADL '6'
///回购手续费不一样
#define OTCT_Private '7'

class COTCTypeType : public CUFCharType
{
public:							
	COTCTypeType(void)
	{
	}
	COTCTypeType(const COTCTypeType &v)
	{
		setValue(v.getValue());
	}
	COTCTypeType(const char v)
	{
		setValue(v);
	}			
	COTCTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	COTCTypeType& operator=(const COTCTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	COTCTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("01234567"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("01234567",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("01234567",ch))
			return true;
		else
			return false;
	}
};
typedef const COTCTypeType CReadOnlyOTCTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个保证金价格类型类
/////////////////////////////////////////////////////////////////////////
///不按比例按固定值,无需价格
#define MPT_None '0'
///昨结算价
#define MPT_PreSettlementPrice '1'
///开仓价
#define MPT_OpenPrice '2'
///标记价
#define MPT_MarkedPrice '3'
///最新价
#define MPT_LastPrice '4'

class CMarginPriceTypeType : public CUFCharType
{
public:							
	CMarginPriceTypeType(void)
	{
	}
	CMarginPriceTypeType(const CMarginPriceTypeType &v)
	{
		setValue(v.getValue());
	}
	CMarginPriceTypeType(const char v)
	{
		setValue(v);
	}			
	CMarginPriceTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CMarginPriceTypeType& operator=(const CMarginPriceTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CMarginPriceTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("01234"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("01234",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("01234",ch))
			return true;
		else
			return false;
	}
};
typedef const CMarginPriceTypeType CReadOnlyMarginPriceTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个触发价格类型类
/////////////////////////////////////////////////////////////////////////
///最新价
#define TPT_LastPrice '0'
///标记价
#define TPT_MarkedPrice '1'
///标的指数价
#define TPT_UnderlyingPrice '2'

class CTriggerPriceTypeType : public CUFCharType
{
public:							
	CTriggerPriceTypeType(void)
	{
	}
	CTriggerPriceTypeType(const CTriggerPriceTypeType &v)
	{
		setValue(v.getValue());
	}
	CTriggerPriceTypeType(const char v)
	{
		setValue(v);
	}			
	CTriggerPriceTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CTriggerPriceTypeType& operator=(const CTriggerPriceTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CTriggerPriceTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CTriggerPriceTypeType CReadOnlyTriggerPriceTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个限价价格类型类
/////////////////////////////////////////////////////////////////////////
///昨结算价
#define LPT_PreSettlementPrice '1'
///最新价
#define LPT_LastPrice '2'
///基础标的价
#define LPT_UnderlyingPrice '3'
///标记价
#define LPT_MarkedPrice '4'

class CLimitPriceTypeType : public CUFCharType
{
public:							
	CLimitPriceTypeType(void)
	{
	}
	CLimitPriceTypeType(const CLimitPriceTypeType &v)
	{
		setValue(v.getValue());
	}
	CLimitPriceTypeType(const char v)
	{
		setValue(v);
	}			
	CLimitPriceTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CLimitPriceTypeType& operator=(const CLimitPriceTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CLimitPriceTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("1234"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("1234",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("1234",ch))
			return true;
		else
			return false;
	}
};
typedef const CLimitPriceTypeType CReadOnlyLimitPriceTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个加密类型类
/////////////////////////////////////////////////////////////////////////
///不加密
#define ET_None '0'
///MD5加密
#define ET_MD5 '1'
///Base64加密
#define ET_Base64 '2'

class CEncryptTypeType : public CUFCharType
{
public:							
	CEncryptTypeType(void)
	{
	}
	CEncryptTypeType(const CEncryptTypeType &v)
	{
		setValue(v.getValue());
	}
	CEncryptTypeType(const char v)
	{
		setValue(v);
	}			
	CEncryptTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CEncryptTypeType& operator=(const CEncryptTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CEncryptTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CEncryptTypeType CReadOnlyEncryptTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个席位状态类
/////////////////////////////////////////////////////////////////////////
///不活跃
#define LS_NotActive '0'
///已登录
#define LS_Login '1'
///已登出
#define LS_Logout '2'

class CLinkStatusType : public CUFCharType
{
public:							
	CLinkStatusType(void)
	{
	}
	CLinkStatusType(const CLinkStatusType &v)
	{
		setValue(v.getValue());
	}
	CLinkStatusType(const char v)
	{
		setValue(v);
	}			
	CLinkStatusType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CLinkStatusType& operator=(const CLinkStatusType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CLinkStatusType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CLinkStatusType CReadOnlyLinkStatusType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个控制访问类型类
/////////////////////////////////////////////////////////////////////////
///使用IP地址
#define AT_IPAddress '1'
///使用MAC地址
#define AT_MAC '2'
///使用硬盘序列号地址
#define AT_HardDisk '3'
///使用授权码验证
#define AT_AuthCode '4'
///业务使用授权
#define AT_FuncCode '5'
///是否能操作相关成员
#define AT_Member '6'
///是否能操作相关交易单元
#define AT_TradeUnit '7'
///流量控制
#define AT_CommFlux '8'
///主题订阅
#define AT_TopicSub '9'
///登录错误限制
#define AT_LoginErrorLimit 'a'
///出入金权限
#define AT_Deposit 'b'
///用户APIKey使用
#define AT_APIKey 'c'
///手机号
#define AT_CellPhone 'd'
///邮箱号
#define AT_Email 'e'
///银行支付账号
#define AT_Bank 'f'
///微信支付账号
#define AT_WeChat 'g'
///支付宝支付账号
#define AT_Alipay 'h'
///充值地址
#define AT_DepositAddress 'i'
///提币地址
#define AT_WithDrawAddress 'j'
///OTC信息
#define AT_OTCData 'k'

class CAccessTypeType : public CUFCharType
{
public:							
	CAccessTypeType(void)
	{
	}
	CAccessTypeType(const CAccessTypeType &v)
	{
		setValue(v.getValue());
	}
	CAccessTypeType(const char v)
	{
		setValue(v);
	}			
	CAccessTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CAccessTypeType& operator=(const CAccessTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CAccessTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("123456789abcdefghijk"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("123456789abcdefghijk",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("123456789abcdefghijk",ch))
			return true;
		else
			return false;
	}
};
typedef const CAccessTypeType CReadOnlyAccessTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个控制访问类型类
/////////////////////////////////////////////////////////////////////////
///手机号
#define AT_CellPhone 'd'
///邮箱号
#define AT_Email 'e'
///银行支付账号
#define AT_Bank 'f'
///微信支付账号
#define AT_WeChat 'g'
///支付宝支付账号
#define AT_Alipay 'h'
///充值地址
#define AT_DepositAddress 'i'
///提币地址
#define AT_WithDrawAddress 'j'

class CUserSettingTypeType : public CUFCharType
{
public:							
	CUserSettingTypeType(void)
	{
	}
	CUserSettingTypeType(const CUserSettingTypeType &v)
	{
		setValue(v.getValue());
	}
	CUserSettingTypeType(const char v)
	{
		setValue(v);
	}			
	CUserSettingTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CUserSettingTypeType& operator=(const CUserSettingTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CUserSettingTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("defghij"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("defghij",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("defghij",ch))
			return true;
		else
			return false;
	}
};
typedef const CUserSettingTypeType CReadOnlyUserSettingTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个成交价生成方式类
/////////////////////////////////////////////////////////////////////////
///对手价(订单簿内的订单价格)
#define CP_OrderBookPrice '1'
///最新价、买价、卖价三价取中,市价单和组合单使用对手价
#define CP_MiddlePrice '2'

class CTradePriceModeType : public CUFCharType
{
public:							
	CTradePriceModeType(void)
	{
	}
	CTradePriceModeType(const CTradePriceModeType &v)
	{
		setValue(v.getValue());
	}
	CTradePriceModeType(const char v)
	{
		setValue(v);
	}			
	CTradePriceModeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CTradePriceModeType& operator=(const CTradePriceModeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CTradePriceModeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("12"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("12",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("12",ch))
			return true;
		else
			return false;
	}
};
typedef const CTradePriceModeType CReadOnlyTradePriceModeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个订阅操作类
/////////////////////////////////////////////////////////////////////////
///全部退订
#define SA_Clear '0'
///订阅
#define SA_Sub '1'
///退订
#define SA_UnSub '2'

class CSubActionType : public CUFCharType
{
public:							
	CSubActionType(void)
	{
	}
	CSubActionType(const CSubActionType &v)
	{
		setValue(v.getValue());
	}
	CSubActionType(const char v)
	{
		setValue(v);
	}			
	CSubActionType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CSubActionType& operator=(const CSubActionType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CSubActionType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CSubActionType CReadOnlySubActionType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个资金变化类型类
/////////////////////////////////////////////////////////////////////////
///普通
#define ACT_None '0'
///盈亏
#define ACT_ClostProfit '1'
///资金收支
#define ACT_MoneyChange '2'
///入金
#define ACT_Deposit '3'
///出金
#define ACT_Withdraw '4'
///手续费
#define ACT_Fee '5'
///递延费
#define ACT_KeepFee '6'
///资金费率
#define ACT_FundingRate '7'
///结算
#define ACT_Settlement '8'
///交割(行权)收益
#define ACT_Delivery '9'
///强平接管
#define ACT_Liquidation 'a'
///保留资金获取
#define ACT_ReserveDeposit 'b'
///保留资金转出
#define ACT_ReserveWithdraw 'c'
///保留资金盈利回收
#define ACT_ReserveProfit 'd'
///同步报文
#define ACT_Sync 'e'
///ADL减仓
#define ACT_ADL 'f'
///分润(预计分润)
#define ACT_CopyProfit 'g'
///跟单退还
#define ACT_FollowerReturn 'h'
///带单收入
#define ACT_LeaderIncome 'i'
///借款利息收支
#define ACT_LoanInterest 'j'

class CAccountChangeTypeType : public CUFCharType
{
public:							
	CAccountChangeTypeType(void)
	{
	}
	CAccountChangeTypeType(const CAccountChangeTypeType &v)
	{
		setValue(v.getValue());
	}
	CAccountChangeTypeType(const char v)
	{
		setValue(v);
	}			
	CAccountChangeTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CAccountChangeTypeType& operator=(const CAccountChangeTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CAccountChangeTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("0123456789abcdefghij"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("0123456789abcdefghij",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("0123456789abcdefghij",ch))
			return true;
		else
			return false;
	}
};
typedef const CAccountChangeTypeType CReadOnlyAccountChangeTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个结算操作类
/////////////////////////////////////////////////////////////////////////
///结算操作开始
#define SAT_SettlementBegin '0'
///定期无负债结算
#define SAT_Settlement '1'
///到期交割(行权)
#define SAT_Delivery '2'
///资金费率交换
#define SAT_FundingRate '3'
///隔夜费划转
#define SAT_OverNightFee '4'
///分摊
#define SAT_Clawback '5'
///资金初始化
#define SAT_AccountInit '6'
///结算操作结束
#define SAT_SettlementEnd '7'
///数据检查
#define SAT_DataCheck '8'
///数据修复
#define SAT_DataRepair '9'
///平账
#define SAT_AccountBalance 'a'
///清理
#define SAT_DataClear 'b'
///清空没有持仓的产品
#define SAT_ClearNoPosition 'c'
///借贷利息
#define SAT_LoanInterest 'd'

class CSettleActionTypeType : public CUFCharType
{
public:							
	CSettleActionTypeType(void)
	{
	}
	CSettleActionTypeType(const CSettleActionTypeType &v)
	{
		setValue(v.getValue());
	}
	CSettleActionTypeType(const char v)
	{
		setValue(v);
	}			
	CSettleActionTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CSettleActionTypeType& operator=(const CSettleActionTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CSettleActionTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("0123456789abcd"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("0123456789abcd",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("0123456789abcd",ch))
			return true;
		else
			return false;
	}
};
typedef const CSettleActionTypeType CReadOnlySettleActionTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个下游系统同步类型类
/////////////////////////////////////////////////////////////////////////
///同步到数据库
#define ST_SinkToDB '0'
///同步到Redis不到数据库
#define ST_SinkTillRedis '1'
///不同步
#define ST_NoSink '2'

class CSinkTypeType : public CUFCharType
{
public:							
	CSinkTypeType(void)
	{
	}
	CSinkTypeType(const CSinkTypeType &v)
	{
		setValue(v.getValue());
	}
	CSinkTypeType(const char v)
	{
		setValue(v);
	}			
	CSinkTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CSinkTypeType& operator=(const CSinkTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CSinkTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CSinkTypeType CReadOnlySinkTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个保留资金使用类型类
/////////////////////////////////////////////////////////////////////////
///先亏损保留资金再亏损真钱(劣后)
#define RT_Inferior '0'
///先亏损真钱再亏损保留资金(优先)
#define RT_Prior '1'
///保留资金不做亏损(抵押金)
#define RT_Guarantee '2'

class CReserveTypeType : public CUFCharType
{
public:							
	CReserveTypeType(void)
	{
	}
	CReserveTypeType(const CReserveTypeType &v)
	{
		setValue(v.getValue());
	}
	CReserveTypeType(const char v)
	{
		setValue(v);
	}			
	CReserveTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CReserveTypeType& operator=(const CReserveTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CReserveTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CReserveTypeType CReadOnlyReserveTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个浮盈控制方式类
/////////////////////////////////////////////////////////////////////////
///不考虑盈利和亏损
#define PPU_NoUse '0'
///盈利可以开仓,亏损不计算
#define PPU_OnlyProfit '1'
///亏损计算,盈利不可以开仓
#define PPU_OnlyLoss '2'
///盈利可以开仓,亏损也计算
#define PPU_All '3'

class CPositionProfitUseType : public CUFCharType
{
public:							
	CPositionProfitUseType(void)
	{
	}
	CPositionProfitUseType(const CPositionProfitUseType &v)
	{
		setValue(v.getValue());
	}
	CPositionProfitUseType(const char v)
	{
		setValue(v);
	}			
	CPositionProfitUseType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CPositionProfitUseType& operator=(const CPositionProfitUseType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CPositionProfitUseType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("0123"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("0123",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("0123",ch))
			return true;
		else
			return false;
	}
};
typedef const CPositionProfitUseType CReadOnlyPositionProfitUseType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个平仓订单顺序(和CloseOrderID一个字段)类
/////////////////////////////////////////////////////////////////////////
///按盈利多先平
#define COM_ProfitFirst '1'
///按亏损多先平
#define COM_LossFirst '2'
///先插入先平
#define COM_FirstInFirst '3'
///后插入先平
#define COM_lastInFirst '4'

class CCloseOrderModeType : public CUFCharType
{
public:							
	CCloseOrderModeType(void)
	{
	}
	CCloseOrderModeType(const CCloseOrderModeType &v)
	{
		setValue(v.getValue());
	}
	CCloseOrderModeType(const char v)
	{
		setValue(v);
	}			
	CCloseOrderModeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CCloseOrderModeType& operator=(const CCloseOrderModeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CCloseOrderModeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("1234"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("1234",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("1234",ch))
			return true;
		else
			return false;
	}
};
typedef const CCloseOrderModeType CReadOnlyCloseOrderModeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个数列类型类
/////////////////////////////////////////////////////////////////////////
///等差数列
#define RA_Arithmetical '1'
///等比数列
#define RA_Geometrical '2'

class CSequenceTypeType : public CUFCharType
{
public:							
	CSequenceTypeType(void)
	{
	}
	CSequenceTypeType(const CSequenceTypeType &v)
	{
		setValue(v.getValue());
	}
	CSequenceTypeType(const char v)
	{
		setValue(v);
	}			
	CSequenceTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CSequenceTypeType& operator=(const CSequenceTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CSequenceTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("12"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("12",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("12",ch))
			return true;
		else
			return false;
	}
};
typedef const CSequenceTypeType CReadOnlySequenceTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个点差接管类型类
/////////////////////////////////////////////////////////////////////////
///先撮合后点差接管
#define CFDT_Normal '0'
///只撮合不点差接管
#define CFDT_OnlyMatch '1'
///不撮合只点差接管
#define CFDT_OnlyCFD '2'

class CCFDTypeType : public CUFCharType
{
public:							
	CCFDTypeType(void)
	{
	}
	CCFDTypeType(const CCFDTypeType &v)
	{
		setValue(v.getValue());
	}
	CCFDTypeType(const char v)
	{
		setValue(v);
	}			
	CCFDTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CCFDTypeType& operator=(const CCFDTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CCFDTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("012"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("012",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("012",ch))
			return true;
		else
			return false;
	}
};
typedef const CCFDTypeType CReadOnlyCFDTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个CFD取值类型类
/////////////////////////////////////////////////////////////////////////
///委托张数
#define CFDVT_Volume '0'
///委托额
#define CFDVT_Cost '1'

class CCFDValueTypeType : public CUFCharType
{
public:							
	CCFDValueTypeType(void)
	{
	}
	CCFDValueTypeType(const CCFDValueTypeType &v)
	{
		setValue(v.getValue());
	}
	CCFDValueTypeType(const char v)
	{
		setValue(v);
	}			
	CCFDValueTypeType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CCFDValueTypeType& operator=(const CCFDValueTypeType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CCFDValueTypeType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("01"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("01",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("01",ch))
			return true;
		else
			return false;
	}
};
typedef const CCFDValueTypeType CReadOnlyCFDValueTypeType;


/////////////////////////////////////////////////////////////////////////
///CEnumCharType是一个账户风控界别类
/////////////////////////////////////////////////////////////////////////
///本币种资金权益
#define RL_Currency '0'
///结算组所有资金权益
#define RL_Settlement '1'
///资金账号所有资金权益
#define RL_Account '2'
///会员账号所有资金权益
#define RL_Member '3'

class CRiskLevelType : public CUFCharType
{
public:							
	CRiskLevelType(void)
	{
	}
	CRiskLevelType(const CRiskLevelType &v)
	{
		setValue(v.getValue());
	}
	CRiskLevelType(const char v)
	{
		setValue(v);
	}			
	CRiskLevelType(const CUFCharType &v)
	{						
		setValue(v.getValue());
	}
	CRiskLevelType& operator=(const CRiskLevelType &c)
	{						
		setValue(c.getValue());
		return *this;
	}
	CRiskLevelType& operator=(const CUFCharType &c)
	{
		setValue(c.getValue());
		return *this;
	}
	const char operator =(const char c)
	{
		setValue(c);
		return c;
	}
	const char* operator =(const char *v)
	{
		setValue(*v);
		return v;
	}
	void clear(void)
	{
		setValue('\0');
	}
	void init(void)
	{
		setValue("0123"[0]);
	}
	void setValue(const char v)
	{
		value=v;
	}
	void setValue(const char* s)
	{
		value=s[0];
	}
	bool setJsonValue(const char* s, const int nLength)
	{
		if (nLength <= 0)
		{
			value = '\0';
			return true;
		}
		value = *s;
		return isValid();
	}	
	bool isNull(void) const
	{
		if (value == '\0' || value == ' ')
			return true;
		return false;
	}
	bool isValid(void) const
	{
		if (isNull())
			return true;
		if (strchr("0123",value))
			return true;
		else
			return false;
	}
	static bool isValid(const char ch)
	{
		if (strchr("0123",ch))
			return true;
		else
			return false;
	}
};
typedef const CRiskLevelType CReadOnlyRiskLevelType;



#endif
