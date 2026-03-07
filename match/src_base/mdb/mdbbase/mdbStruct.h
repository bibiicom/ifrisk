/////////////////////////////////////////////////////////////////////////
///mdbStruct.h
///定义了内存数据库用的基本数据结构类，以及iterator、trigger的基类
/////////////////////////////////////////////////////////////////////////

#ifndef BASEDATASTRUCT_H
#define BASEDATASTRUCT_H

#include "platform.h"
#include "DataType.h"
#include "CTransaction.h"

///判断结果是否满足条件
extern bool satisfy(int value,int type);

///字符串比较函数
///@param	s1	字符串1
///@param	s2	字符串2
///@return	1表示大于，0表示等于，-1表示小于
int compare(const char *s1, const char *s2);

///整数比较函数
///@param	v1	整数1
///@param	v2	整数2
///@return	1表示大于，0表示等于，-1表示小于
int compare(const int v1,const int v2);

///长整数比较函数
///@param	v1	长整数1
///@param	v2	长整数2
///@return	1表示大于，0表示等于，-1表示小于
//int compare(const UF_INT8 v1,const UF_INT8 v2);

///浮点数比较函数
///@param	v1	浮点数1
///@param	v2	浮点数2
///@return	1表示大于，0表示等于，-1表示小于
int compare(const double v1,const double v2);

///从buffer中获取下一个token，这里的token是以逗号或者换行分隔的
///在使用时，第一次应当传入指定的buffer，以后应当反复传入NULL，
///这样可以依次取出所有的token。本函数将改变buffer中的内容，叫用
///方有责任自己保护数据。本函数有重入问题，在一个取token的序列结
///束前，开始新的一个取token序列，将影响前面一个的结果。此问题和
///strtok的问题类似
///@param	buffer	要取token的buffer，NULL表示从上次取的buffer中取下一个
///@return	得到的token
char *getNextToken(char *buffer);

class CWriteableCurrentTime;
class CCurrentTimeFactory;
class CWriteableMember;
class CMemberFactory;
class CWriteableServiceConfig;
class CServiceConfigFactory;
class CWriteableUserSession;
class CUserSessionFactory;
class CWriteableInstrument;
class CInstrumentFactory;
class CWriteableTradingRight;
class CTradingRightFactory;
class CWriteableMarginRate;
class CMarginRateFactory;
class CWriteableCFDRate;
class CCFDRateFactory;
class CWriteableFee;
class CFeeFactory;
class CWriteableCurrency;
class CCurrencyFactory;
class CWriteableLiquidity;
class CLiquidityFactory;
class CWriteableAccount;
class CAccountFactory;
class CWriteableMemberPosition;
class CMemberPositionFactory;
class CWriteablePosition;
class CPositionFactory;
class CWriteableMarketData;
class CMarketDataFactory;
class CWriteableOrder;
class COrderFactory;
class CWriteablePositionOrder;
class CPositionOrderFactory;
class CWriteableTriggerOrder;
class CTriggerOrderFactory;
class CWriteableMarketOrder;
class CMarketOrderFactory;
class CWriteableTickMarketOrder;
class CTickMarketOrderFactory;
class CWriteableTrade;
class CTradeFactory;
class CWriteableKLine;
class CKLineFactory;
class CWriteablePublishKLine;
class CPublishKLineFactory;
class CWriteableLastKLine;
class CLastKLineFactory;
class CWriteableAccountDetail;
class CAccountDetailFactory;
class CWriteableSettleDetail;
class CSettleDetailFactory;
class CWriteableOrderAction;
class CMDB;


/////////////////////////////////////////////////////////////////////////
///CWriteableCurrentTime是一个存储当前时间的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableCurrentTime
{
public:
	CWriteableCurrentTime()
	{
	}
	
	CWriteableCurrentTime(const CWriteableCurrentTime& c)
	{
		memcpy(this, &c, sizeof(CWriteableCurrentTime));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! SystemID.isValid())
			return "SystemID";
		if ( ! ZoneDate.isValid())
			return "ZoneDate";
		if ( ! ZoneTime.isValid())
			return "ZoneTime";
		if ( ! ZoneDays.isValid())
			return "ZoneDays";
		if ( ! CurrTime.isValid())
			return "CurrTime";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		if ( ! TimeZone.isValid())
			return "TimeZone";
		if ( ! LastTradeID.isValid())
			return "LastTradeID";
		if ( ! LastOrderID.isValid())
			return "LastOrderID";
		if ( ! LastAccountDetailID.isValid())
			return "LastAccountDetailID";
		if ( ! LastTimeSortNo.isValid())
			return "LastTimeSortNo";
		if ( ! LastBusinessNo.isValid())
			return "LastBusinessNo";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableCurrentTime *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///系统名
	CShortIndexIDType SystemID;
	///本地日期
	CDateType ZoneDate;
	///本地时间
	CTimeType ZoneTime;
	///本地天数
	CDateType ZoneDays;
	///当前时间（秒）
	CMilliSecondsType CurrTime;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间（毫秒）
	CMilliSecondsType UpdateTime;
	///本地时区
	CNumberType TimeZone;
	///最新最新成交序列号
	CIndexIDType LastTradeID;
	///最新订单序列号
	CIndexIDType LastOrderID;
	///最新资金明细序列号
	CIndexIDType LastAccountDetailID;
	///最新排序序列号
	CLargeNumberType LastTimeSortNo;
	///最新事务序列号
	CLargeNumberType LastBusinessNo;
	
	
	///计算所有的hash值
	void calAllHash()
	{
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableCurrentTime CCurrentTime;

/////////////////////////////////////////////////////////////////////////
///CCurrentTimeActionTrigger是操作当前时间时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCurrentTimeActionTrigger
{
public:
	///构造方法
	CCurrentTimeActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CCurrentTimeActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CCurrentTimeFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pCurrentTime	要加入的CurrentTime
	virtual void beforeAdd(CWriteableCurrentTime *pCurrentTime)
	{
		return;
	}
	
	///加入后触发
	///@param	pCurrentTime	已经加入的CurrentTime
	virtual void afterAdd(CCurrentTime *pCurrentTime)
	{
		return;
	}

	///更新前触发	
	///@param	pCurrentTime	被刷新的CCurrentTime
	///@param	pNewCurrentTime	新的值
	virtual void beforeUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pNewCurrentTime)
	{
		return;
	}
	
	///更新后触发
	///@param	pCurrentTime	被刷新的CCurrentTime
	virtual void afterUpdate(CCurrentTime *pCurrentTime)
	{
		return;
	}
	
	///删除前触发
	///@param	pCurrentTime	要删除的CCurrentTime
	virtual void beforeRemove(CCurrentTime *pCurrentTime)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CCurrentTimeFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CCurrentTimeCommitTrigger是确认当前时间时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCurrentTimeCommitTrigger
{
public:
	///构造方法
	CCurrentTimeCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CCurrentTimeCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CCurrentTimeFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pCurrentTime	已经加入的CurrentTime
	virtual void commitAdd(CCurrentTime *pCurrentTime)
	{
		return;
	}

	///更新后触发
	///@param	pCurrentTime	被刷新的CCurrentTime
	///@param	poldCurrentTime	原来的值
	virtual void commitUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pOldCurrentTime)
	{
		return;
	}
	
	///删除后触发
	///@param	pCurrentTime	已经删除的CCurrentTime
	virtual void commitRemove(CWriteableCurrentTime *pCurrentTime)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CCurrentTimeFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CCurrentTimeIterator是一个对当前时间的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCurrentTimeIterator
{
public:
	///构造方法
	CCurrentTimeIterator()
	{
	}

	///构造方法
	CCurrentTimeIterator(CCurrentTimeFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CCurrentTimeIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个CurrentTime
	///@return	下一个满足条件CCurrentTime，如果已经没有一个满足要求了，则返回NULL
	virtual CCurrentTime *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CCurrentTimeFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pCurrentTime 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableCurrentTime * pCurrentTime,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CCurrentTimeFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableMember是一个存储成员的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableMember
{
public:
	CWriteableMember()
	{
	}
	
	CWriteableMember(const CWriteableMember& c)
	{
		memcpy(this, &c, sizeof(CWriteableMember));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! MemberType.isValid())
			return "MemberType";
		if ( ! Grade.isValid())
			return "Grade";
		if ( ! Priority.isValid())
			return "Priority";
		if ( ! FeeGrade.isValid())
			return "FeeGrade";
		if ( ! MarginRateGrade.isValid())
			return "MarginRateGrade";
		if ( ! TradingRightGrade.isValid())
			return "TradingRightGrade";
		if ( ! CFDGrade.isValid())
			return "CFDGrade";
		if ( ! LiquidityGrade.isValid())
			return "LiquidityGrade";
		if ( ! RiskLevel.isValid())
			return "RiskLevel";
		if ( ! PositionType.isValid())
			return "PositionType";
		if ( ! TriggerOrders.isValid())
			return "TriggerOrders";
		if ( ! OpenOrders.isValid())
			return "OpenOrders";
		if ( ! OpenPositions.isValid())
			return "OpenPositions";
		if ( ! MaxLocalID.isValid())
			return "MaxLocalID";
		if ( ! SinkType.isValid())
			return "SinkType";
		if ( ! CopyMemberID.isValid())
			return "CopyMemberID";
		if ( ! Region.isValid())
			return "Region";
		if ( ! FeeDeduceDisCount.isValid())
			return "FeeDeduceDisCount";
		if ( ! FeeDeduceCurrency.isValid())
			return "FeeDeduceCurrency";
		if ( ! CanBorrow.isValid())
			return "CanBorrow";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableMember *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///成员代码
	CLongIndexIDType MemberID;
	///成员类型
	CMemberTypeType MemberType;
	///等级
	CShortIndexIDType Grade;
	///优先权
	CNumberType Priority;
	///手续费等级
	CShortIndexIDType FeeGrade;
	///保证金等级
	CShortIndexIDType MarginRateGrade;
	///权限等级
	CShortIndexIDType TradingRightGrade;
	///差价等级
	CShortIndexIDType CFDGrade;
	///流动性等级
	CShortIndexIDType LiquidityGrade;
	///风控类型
	CRiskLevelType RiskLevel;
	///持仓类型
	CPositionTypeType PositionType;
	///触发订单个数
	CNumberType TriggerOrders;
	///未成交订单数
	CNumberType OpenOrders;
	///用户持仓个数
	CNumberType OpenPositions;
	///最大本地标识
	CIndexIDType MaxLocalID;
	///下游系统同步类型
	CSinkTypeType SinkType;
	///带单员代码
	CLongIndexIDType CopyMemberID;
	CShortIndexIDType Region;
	///作为手续费的抵扣优惠比例
	CRatioType FeeDeduceDisCount;
	///作为手续费的抵扣比例
	CCurrencyType FeeDeduceCurrency;
	///是否需要借钱交易
	CBoolType CanBorrow;
	///备注
	CNameType Remark;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	unsigned int HashMemberID;
	
	///计算MemberID的hash值
	void calHashMemberID()
	{
		unsigned key=0;
		key=MemberID.hash(key);
		HashMemberID=key;
	}
	
	///根据需要情况，更新MemberID的hash值
	void updateHashMemberID()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashMemberID();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashMemberID();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableMember CMember;

/////////////////////////////////////////////////////////////////////////
///CMemberActionTrigger是操作成员时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMemberActionTrigger
{
public:
	///构造方法
	CMemberActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CMemberActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CMemberFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pMember	要加入的Member
	virtual void beforeAdd(CWriteableMember *pMember)
	{
		return;
	}
	
	///加入后触发
	///@param	pMember	已经加入的Member
	virtual void afterAdd(CMember *pMember)
	{
		return;
	}

	///更新前触发	
	///@param	pMember	被刷新的CMember
	///@param	pNewMember	新的值
	virtual void beforeUpdate(CMember *pMember, CWriteableMember *pNewMember)
	{
		return;
	}
	
	///更新后触发
	///@param	pMember	被刷新的CMember
	virtual void afterUpdate(CMember *pMember)
	{
		return;
	}
	
	///删除前触发
	///@param	pMember	要删除的CMember
	virtual void beforeRemove(CMember *pMember)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CMemberFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CMemberCommitTrigger是确认成员时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMemberCommitTrigger
{
public:
	///构造方法
	CMemberCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CMemberCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CMemberFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pMember	已经加入的Member
	virtual void commitAdd(CMember *pMember)
	{
		return;
	}

	///更新后触发
	///@param	pMember	被刷新的CMember
	///@param	poldMember	原来的值
	virtual void commitUpdate(CMember *pMember, CWriteableMember *pOldMember)
	{
		return;
	}
	
	///删除后触发
	///@param	pMember	已经删除的CMember
	virtual void commitRemove(CWriteableMember *pMember)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CMemberFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CMemberIterator是一个对成员的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMemberIterator
{
public:
	///构造方法
	CMemberIterator()
	{
	}

	///构造方法
	CMemberIterator(CMemberFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CMemberIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个Member
	///@return	下一个满足条件CMember，如果已经没有一个满足要求了，则返回NULL
	virtual CMember *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CMemberFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pMember 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableMember * pMember,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CMemberFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableServiceConfig是一个存储业务配置的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableServiceConfig
{
public:
	CWriteableServiceConfig()
	{
	}
	
	CWriteableServiceConfig(const CWriteableServiceConfig& c)
	{
		memcpy(this, &c, sizeof(CWriteableServiceConfig));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! ConfigName.isValid())
			return "ConfigName";
		if ( ! Index1.isValid())
			return "Index1";
		if ( ! Index2.isValid())
			return "Index2";
		if ( ! ConfigValue.isValid())
			return "ConfigValue";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableServiceConfig *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///配置名称
	CNameType ConfigName;
	///索引1
	CNameType Index1;
	///索引2
	CNameType Index2;
	///配置值
	CLongNameType ConfigValue;
	///备注
	CContentType Remark;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	
	///计算所有的hash值
	void calAllHash()
	{
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableServiceConfig CServiceConfig;

/////////////////////////////////////////////////////////////////////////
///CServiceConfigActionTrigger是操作业务配置时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CServiceConfigActionTrigger
{
public:
	///构造方法
	CServiceConfigActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CServiceConfigActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CServiceConfigFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pServiceConfig	要加入的ServiceConfig
	virtual void beforeAdd(CWriteableServiceConfig *pServiceConfig)
	{
		return;
	}
	
	///加入后触发
	///@param	pServiceConfig	已经加入的ServiceConfig
	virtual void afterAdd(CServiceConfig *pServiceConfig)
	{
		return;
	}

	///更新前触发	
	///@param	pServiceConfig	被刷新的CServiceConfig
	///@param	pNewServiceConfig	新的值
	virtual void beforeUpdate(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pNewServiceConfig)
	{
		return;
	}
	
	///更新后触发
	///@param	pServiceConfig	被刷新的CServiceConfig
	virtual void afterUpdate(CServiceConfig *pServiceConfig)
	{
		return;
	}
	
	///删除前触发
	///@param	pServiceConfig	要删除的CServiceConfig
	virtual void beforeRemove(CServiceConfig *pServiceConfig)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CServiceConfigFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CServiceConfigCommitTrigger是确认业务配置时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CServiceConfigCommitTrigger
{
public:
	///构造方法
	CServiceConfigCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CServiceConfigCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CServiceConfigFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pServiceConfig	已经加入的ServiceConfig
	virtual void commitAdd(CServiceConfig *pServiceConfig)
	{
		return;
	}

	///更新后触发
	///@param	pServiceConfig	被刷新的CServiceConfig
	///@param	poldServiceConfig	原来的值
	virtual void commitUpdate(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pOldServiceConfig)
	{
		return;
	}
	
	///删除后触发
	///@param	pServiceConfig	已经删除的CServiceConfig
	virtual void commitRemove(CWriteableServiceConfig *pServiceConfig)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CServiceConfigFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CServiceConfigIterator是一个对业务配置的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CServiceConfigIterator
{
public:
	///构造方法
	CServiceConfigIterator()
	{
	}

	///构造方法
	CServiceConfigIterator(CServiceConfigFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CServiceConfigIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个ServiceConfig
	///@return	下一个满足条件CServiceConfig，如果已经没有一个满足要求了，则返回NULL
	virtual CServiceConfig *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CServiceConfigFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pServiceConfig 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableServiceConfig * pServiceConfig,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CServiceConfigFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableUserSession是一个存储交易员在线会话的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableUserSession
{
public:
	CWriteableUserSession()
	{
	}
	
	CWriteableUserSession(const CWriteableUserSession& c)
	{
		memcpy(this, &c, sizeof(CWriteableUserSession));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! UserID.isValid())
			return "UserID";
		if ( ! Password.isValid())
			return "Password";
		if ( ! UserProductID.isValid())
			return "UserProductID";
		if ( ! MacAddress.isValid())
			return "MacAddress";
		if ( ! ClientIPAddress.isValid())
			return "ClientIPAddress";
		if ( ! HDSerialID.isValid())
			return "HDSerialID";
		if ( ! AuthCode.isValid())
			return "AuthCode";
		if ( ! APPID.isValid())
			return "APPID";
		if ( ! Token.isValid())
			return "Token";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! APIID.isValid())
			return "APIID";
		if ( ! LoginTime.isValid())
			return "LoginTime";
		if ( ! ExpireTime.isValid())
			return "ExpireTime";
		if ( ! IPAddress.isValid())
			return "IPAddress";
		if ( ! MaxLocalID.isValid())
			return "MaxLocalID";
		if ( ! SessionNo.isValid())
			return "SessionNo";
		if ( ! FrontNo.isValid())
			return "FrontNo";
		if ( ! AccessLimit.isValid())
			return "AccessLimit";
		if ( ! UserType.isValid())
			return "UserType";
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! LimitAccesses.isValid())
			return "LimitAccesses";
		if ( ! SinkType.isValid())
			return "SinkType";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableUserSession *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///交易用户代码
	CLongIndexIDType UserID;
	///密码
	CPasswordType Password;
	///用户端产品信息
	CNameType UserProductID;
	///Mac地址
	CMacAddressType MacAddress;
	///终端IP地址
	CIPAddressType ClientIPAddress;
	///硬盘序列号
	CLongNameType HDSerialID;
	///授权编码
	CShortContentType AuthCode;
	///应用编号
	CShortIndexIDType APPID;
	///令牌
	CShortContentType Token;
	///备注
	CLongNameType Remark;
	///接口端产品信息
	CNameType APIID;
	///登录时间
	CMilliSecondsType LoginTime;
	///失效时间
	CMilliSecondsType ExpireTime;
	///允许登录的IP地址
	CContentType IPAddress;
	///最大本地标识
	CIndexIDType MaxLocalID;
	///会话编号
	CNumberType SessionNo;
	///前置编号
	CNumberType FrontNo;
	///每秒访问的次数限制
	CNumberType AccessLimit;
	///用户类型
	CUserTypeType UserType;
	///成员代码
	CLongIndexIDType MemberID;
	///该秒已经访问的次数
	CNumberType LimitAccesses;
	///下游系统同步类型
	CSinkTypeType SinkType;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	unsigned int HashToken;
	
	///计算Token的hash值
	void calHashToken()
	{
		unsigned key=0;
		key=Token.hash(key);
		HashToken=key;
	}
	
	///根据需要情况，更新Token的hash值
	void updateHashToken()
	{
		calHashToken();
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashToken();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashToken();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableUserSession CUserSession;

/////////////////////////////////////////////////////////////////////////
///CUserSessionActionTrigger是操作交易员在线会话时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CUserSessionActionTrigger
{
public:
	///构造方法
	CUserSessionActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CUserSessionActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CUserSessionFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pUserSession	要加入的UserSession
	virtual void beforeAdd(CWriteableUserSession *pUserSession)
	{
		return;
	}
	
	///加入后触发
	///@param	pUserSession	已经加入的UserSession
	virtual void afterAdd(CUserSession *pUserSession)
	{
		return;
	}

	///更新前触发	
	///@param	pUserSession	被刷新的CUserSession
	///@param	pNewUserSession	新的值
	virtual void beforeUpdate(CUserSession *pUserSession, CWriteableUserSession *pNewUserSession)
	{
		return;
	}
	
	///更新后触发
	///@param	pUserSession	被刷新的CUserSession
	virtual void afterUpdate(CUserSession *pUserSession)
	{
		return;
	}
	
	///删除前触发
	///@param	pUserSession	要删除的CUserSession
	virtual void beforeRemove(CUserSession *pUserSession)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CUserSessionFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CUserSessionCommitTrigger是确认交易员在线会话时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CUserSessionCommitTrigger
{
public:
	///构造方法
	CUserSessionCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CUserSessionCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CUserSessionFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pUserSession	已经加入的UserSession
	virtual void commitAdd(CUserSession *pUserSession)
	{
		return;
	}

	///更新后触发
	///@param	pUserSession	被刷新的CUserSession
	///@param	poldUserSession	原来的值
	virtual void commitUpdate(CUserSession *pUserSession, CWriteableUserSession *pOldUserSession)
	{
		return;
	}
	
	///删除后触发
	///@param	pUserSession	已经删除的CUserSession
	virtual void commitRemove(CWriteableUserSession *pUserSession)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CUserSessionFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CUserSessionIterator是一个对交易员在线会话的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CUserSessionIterator
{
public:
	///构造方法
	CUserSessionIterator()
	{
	}

	///构造方法
	CUserSessionIterator(CUserSessionFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CUserSessionIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个UserSession
	///@return	下一个满足条件CUserSession，如果已经没有一个满足要求了，则返回NULL
	virtual CUserSession *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CUserSessionFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pUserSession 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableUserSession * pUserSession,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CUserSessionFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableInstrument是一个存储标的的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableInstrument
{
public:
	CWriteableInstrument()
	{
		pMarketData=NULL;
		pClearCurrency=NULL;
		pBaseCurrency=NULL;
	}
	
	CWriteableInstrument(const CWriteableInstrument& c)
	{
		memcpy(this, &c, sizeof(CWriteableInstrument));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! InstrumentName.isValid())
			return "InstrumentName";
		if ( ! SettlementGroup.isValid())
			return "SettlementGroup";
		if ( ! ProductGroup.isValid())
			return "ProductGroup";
		if ( ! UnderlyingID.isValid())
			return "UnderlyingID";
		if ( ! ProductClass.isValid())
			return "ProductClass";
		if ( ! ProductType.isValid())
			return "ProductType";
		if ( ! StrikePrice.isValid())
			return "StrikePrice";
		if ( ! OptionsType.isValid())
			return "OptionsType";
		if ( ! VolumeMultiple.isValid())
			return "VolumeMultiple";
		if ( ! StartTime.isValid())
			return "StartTime";
		if ( ! ExpireTime.isValid())
			return "ExpireTime";
		if ( ! PriceCurrency.isValid())
			return "PriceCurrency";
		if ( ! ClearCurrency.isValid())
			return "ClearCurrency";
		if ( ! IsInverse.isValid())
			return "IsInverse";
		if ( ! BaseCurrency.isValid())
			return "BaseCurrency";
		if ( ! MarginPriceType.isValid())
			return "MarginPriceType";
		if ( ! BasisPrice.isValid())
			return "BasisPrice";
		if ( ! MinOrderVolume.isValid())
			return "MinOrderVolume";
		if ( ! MinOrderCost.isValid())
			return "MinOrderCost";
		if ( ! MaxOrderVolume.isValid())
			return "MaxOrderVolume";
		if ( ! PriceTick.isValid())
			return "PriceTick";
		if ( ! VolumeTick.isValid())
			return "VolumeTick";
		if ( ! ShowVolumeTick.isValid())
			return "ShowVolumeTick";
		if ( ! TradingModel.isValid())
			return "TradingModel";
		if ( ! DayStartTime.isValid())
			return "DayStartTime";
		if ( ! SortNumber.isValid())
			return "SortNumber";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! DefaultLeverage.isValid())
			return "DefaultLeverage";
		if ( ! PriceLimitValueMode.isValid())
			return "PriceLimitValueMode";
		if ( ! PriceLimitPrice.isValid())
			return "PriceLimitPrice";
		if ( ! PriceLimitUpperValue.isValid())
			return "PriceLimitUpperValue";
		if ( ! PriceLimitLowerValue.isValid())
			return "PriceLimitLowerValue";
		if ( ! MarginRateGroup.isValid())
			return "MarginRateGroup";
		if ( ! CFDGroup.isValid())
			return "CFDGroup";
		if ( ! FeeGroup.isValid())
			return "FeeGroup";
		if ( ! TradingRightGroup.isValid())
			return "TradingRightGroup";
		if ( ! MaxOpenInterest.isValid())
			return "MaxOpenInterest";
		if ( ! FundingRateGroup.isValid())
			return "FundingRateGroup";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		if ( ! MarkedPriceTimes.isValid())
			return "MarkedPriceTimes";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableInstrument *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///标的名称
	CLongNameType InstrumentName;
	///结算组编号
	CShortIndexIDType SettlementGroup;
	///产品组
	CShortIndexIDType ProductGroup;
	///基础标的代码
	CInstrumentIDType UnderlyingID;
	///产品大类
	CProductClassType ProductClass;
	///产品类型
	CProductTypeType ProductType;
	///执行价
	CPriceType StrikePrice;
	///期权类型
	COptionsTypeType OptionsType;
	///数量乘数
	CRatioType VolumeMultiple;
	///启动时间
	CMilliSecondsType StartTime;
	///到期时间
	CMilliSecondsType ExpireTime;
	///计价币种
	CCurrencyType PriceCurrency;
	///清算币种
	CCurrencyType ClearCurrency;
	///是否反向标的
	CBoolType IsInverse;
	///标的基础货币
	CCurrencyType BaseCurrency;
	///保证金价格类型
	CMarginPriceTypeType MarginPriceType;
	///挂牌基准价
	CPriceType BasisPrice;
	///最小下单量
	CVolumeType MinOrderVolume;
	///最小下单额
	CVolumeType MinOrderCost;
	///最大下单量
	CVolumeType MaxOrderVolume;
	///最小变动价位
	CPriceType PriceTick;
	///最小变动量
	CVolumeType VolumeTick;
	///前端行情显示最小变动量(服务端不用)
	CVolumeType ShowVolumeTick;
	///交易模式
	CTradingModelType TradingModel;
	///每日开始交易时间
	CShortContentType DayStartTime;
	///排序字段
	CNumberType SortNumber;
	///备注
	CContentType Remark;
	///默认杠杆倍数
	CLeverageType DefaultLeverage;
	///取值方式
	CValueModeType PriceLimitValueMode;
	///限价价格类型
	CLimitPriceTypeType PriceLimitPrice;
	///上限
	CPriceType PriceLimitUpperValue;
	///下限
	CPriceType PriceLimitLowerValue;
	///保证金组
	CShortIndexIDType MarginRateGroup;
	///差价组
	CShortIndexIDType CFDGroup;
	///手续费组
	CShortIndexIDType FeeGroup;
	///权限组
	CShortIndexIDType TradingRightGroup;
	///最大持仓量
	CVolumeType MaxOpenInterest;
	///资金费用组
	CShortIndexIDType FundingRateGroup;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	///标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价)
	CNumberType MarkedPriceTimes;
	
	unsigned int HashInstrumentID;
	
	///计算InstrumentID的hash值
	void calHashInstrumentID()
	{
		unsigned key=0;
		key=ExchangeID.hash(key);
		key=InstrumentID.hash(key);
		HashInstrumentID=key;
	}
	
	///根据需要情况，更新InstrumentID的hash值
	void updateHashInstrumentID()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashInstrumentID();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashInstrumentID();
	}
	
	///构造对应的行情的指针
	///@param	pFactory	构造对应的行情的指针时，寻找的对象工厂
	///@return	找到的对应的行情的指针
	const CWriteableMarketData* linkMarketData() const;

	///构造对应的行情指针
	///@param	pMarketData	要连接到的对象
	///@return	输入的pMarketData
	const CWriteableMarketData* linkMarketData(const CWriteableMarketData *pMarketData) const
	{
		void *target=(void *)(&(this->pMarketData));
		*((const CWriteableMarketData **)target)=pMarketData;
		return pMarketData;
	}
	
	///获取对应的行情的指针
	///@return	找到的对应的行情的指针
	const CWriteableMarketData* getMarketData() const
	{
		if(pMarketData==NULL)
			return linkMarketData();
		return pMarketData;
	}
	
	///构造对应的清算币种的指针
	///@param	pFactory	构造对应的清算币种的指针时，寻找的对象工厂
	///@return	找到的对应的清算币种的指针
	const CWriteableCurrency* linkClearCurrency() const;

	///构造对应的清算币种指针
	///@param	pCurrency	要连接到的对象
	///@return	输入的pCurrency
	const CWriteableCurrency* linkClearCurrency(const CWriteableCurrency *pCurrency) const
	{
		void *target=(void *)(&(this->pClearCurrency));
		*((const CWriteableCurrency **)target)=pCurrency;
		return pCurrency;
	}
	
	///获取对应的清算币种的指针
	///@return	找到的对应的清算币种的指针
	const CWriteableCurrency* getClearCurrency() const
	{
		if(pClearCurrency==NULL)
			return linkClearCurrency();
		return pClearCurrency;
	}
	
	///构造对应的基础币种的指针
	///@param	pFactory	构造对应的基础币种的指针时，寻找的对象工厂
	///@return	找到的对应的基础币种的指针
	const CWriteableCurrency* linkBaseCurrency() const;

	///构造对应的基础币种指针
	///@param	pCurrency	要连接到的对象
	///@return	输入的pCurrency
	const CWriteableCurrency* linkBaseCurrency(const CWriteableCurrency *pCurrency) const
	{
		void *target=(void *)(&(this->pBaseCurrency));
		*((const CWriteableCurrency **)target)=pCurrency;
		return pCurrency;
	}
	
	///获取对应的基础币种的指针
	///@return	找到的对应的基础币种的指针
	const CWriteableCurrency* getBaseCurrency() const
	{
		if(pBaseCurrency==NULL)
			return linkBaseCurrency();
		return pBaseCurrency;
	}
	
	CMDB* m_pMDB;
private:
	///对应的行情
	const CWriteableMarketData *pMarketData;
	///对应的清算币种
	const CWriteableCurrency *pClearCurrency;
	///对应的基础币种
	const CWriteableCurrency *pBaseCurrency;
};

typedef const CWriteableInstrument CInstrument;

/////////////////////////////////////////////////////////////////////////
///CInstrumentActionTrigger是操作标的时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CInstrumentActionTrigger
{
public:
	///构造方法
	CInstrumentActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CInstrumentActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CInstrumentFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pInstrument	要加入的Instrument
	virtual void beforeAdd(CWriteableInstrument *pInstrument)
	{
		return;
	}
	
	///加入后触发
	///@param	pInstrument	已经加入的Instrument
	virtual void afterAdd(CInstrument *pInstrument)
	{
		return;
	}

	///更新前触发	
	///@param	pInstrument	被刷新的CInstrument
	///@param	pNewInstrument	新的值
	virtual void beforeUpdate(CInstrument *pInstrument, CWriteableInstrument *pNewInstrument)
	{
		return;
	}
	
	///更新后触发
	///@param	pInstrument	被刷新的CInstrument
	virtual void afterUpdate(CInstrument *pInstrument)
	{
		return;
	}
	
	///删除前触发
	///@param	pInstrument	要删除的CInstrument
	virtual void beforeRemove(CInstrument *pInstrument)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CInstrumentFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CInstrumentCommitTrigger是确认标的时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CInstrumentCommitTrigger
{
public:
	///构造方法
	CInstrumentCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CInstrumentCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CInstrumentFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pInstrument	已经加入的Instrument
	virtual void commitAdd(CInstrument *pInstrument)
	{
		return;
	}

	///更新后触发
	///@param	pInstrument	被刷新的CInstrument
	///@param	poldInstrument	原来的值
	virtual void commitUpdate(CInstrument *pInstrument, CWriteableInstrument *pOldInstrument)
	{
		return;
	}
	
	///删除后触发
	///@param	pInstrument	已经删除的CInstrument
	virtual void commitRemove(CWriteableInstrument *pInstrument)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CInstrumentFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CInstrumentIterator是一个对标的的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CInstrumentIterator
{
public:
	///构造方法
	CInstrumentIterator()
	{
	}

	///构造方法
	CInstrumentIterator(CInstrumentFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CInstrumentIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个Instrument
	///@return	下一个满足条件CInstrument，如果已经没有一个满足要求了，则返回NULL
	virtual CInstrument *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CInstrumentFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pInstrument 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableInstrument * pInstrument,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CInstrumentFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableTradingRight是一个存储交易权限的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableTradingRight
{
public:
	CWriteableTradingRight()
	{
	}
	
	CWriteableTradingRight(const CWriteableTradingRight& c)
	{
		memcpy(this, &c, sizeof(CWriteableTradingRight));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! Direction.isValid())
			return "Direction";
		if ( ! TradingRight.isValid())
			return "TradingRight";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableTradingRight *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///成员代码
	CLongIndexIDType MemberID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///买卖方向
	CDirectionType Direction;
	///交易权限
	CTradingRightType TradingRight;
	///备注
	CNameType Remark;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	unsigned int HashMemberIDAndInstrument;
	
	///计算MemberIDAndInstrument的hash值
	void calHashMemberIDAndInstrument()
	{
		unsigned key=0;
		key=MemberID.hash(key);
		key=InstrumentID.hash(key);
		key=Direction.hash(key);
		HashMemberIDAndInstrument=key;
	}
	
	///根据需要情况，更新MemberIDAndInstrument的hash值
	void updateHashMemberIDAndInstrument()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashMemberIDAndInstrument();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashMemberIDAndInstrument();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableTradingRight CTradingRight;

/////////////////////////////////////////////////////////////////////////
///CTradingRightActionTrigger是操作交易权限时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTradingRightActionTrigger
{
public:
	///构造方法
	CTradingRightActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CTradingRightActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CTradingRightFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pTradingRight	要加入的TradingRight
	virtual void beforeAdd(CWriteableTradingRight *pTradingRight)
	{
		return;
	}
	
	///加入后触发
	///@param	pTradingRight	已经加入的TradingRight
	virtual void afterAdd(CTradingRight *pTradingRight)
	{
		return;
	}

	///更新前触发	
	///@param	pTradingRight	被刷新的CTradingRight
	///@param	pNewTradingRight	新的值
	virtual void beforeUpdate(CTradingRight *pTradingRight, CWriteableTradingRight *pNewTradingRight)
	{
		return;
	}
	
	///更新后触发
	///@param	pTradingRight	被刷新的CTradingRight
	virtual void afterUpdate(CTradingRight *pTradingRight)
	{
		return;
	}
	
	///删除前触发
	///@param	pTradingRight	要删除的CTradingRight
	virtual void beforeRemove(CTradingRight *pTradingRight)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CTradingRightFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CTradingRightCommitTrigger是确认交易权限时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTradingRightCommitTrigger
{
public:
	///构造方法
	CTradingRightCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CTradingRightCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CTradingRightFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pTradingRight	已经加入的TradingRight
	virtual void commitAdd(CTradingRight *pTradingRight)
	{
		return;
	}

	///更新后触发
	///@param	pTradingRight	被刷新的CTradingRight
	///@param	poldTradingRight	原来的值
	virtual void commitUpdate(CTradingRight *pTradingRight, CWriteableTradingRight *pOldTradingRight)
	{
		return;
	}
	
	///删除后触发
	///@param	pTradingRight	已经删除的CTradingRight
	virtual void commitRemove(CWriteableTradingRight *pTradingRight)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CTradingRightFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CTradingRightIterator是一个对交易权限的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTradingRightIterator
{
public:
	///构造方法
	CTradingRightIterator()
	{
	}

	///构造方法
	CTradingRightIterator(CTradingRightFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CTradingRightIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个TradingRight
	///@return	下一个满足条件CTradingRight，如果已经没有一个满足要求了，则返回NULL
	virtual CTradingRight *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CTradingRightFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pTradingRight 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTradingRight * pTradingRight,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CTradingRightFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableMarginRate是一个存储标的保证金率的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableMarginRate
{
public:
	CWriteableMarginRate()
	{
	}
	
	CWriteableMarginRate(const CWriteableMarginRate& c)
	{
		memcpy(this, &c, sizeof(CWriteableMarginRate));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! ValueMode.isValid())
			return "ValueMode";
		if ( ! MarginSequenceType.isValid())
			return "MarginSequenceType";
		if ( ! RiskSequenceType.isValid())
			return "RiskSequenceType";
		if ( ! LongMarginRatio.isValid())
			return "LongMarginRatio";
		if ( ! LongMaintMarginRatio.isValid())
			return "LongMaintMarginRatio";
		if ( ! LongMarginStep.isValid())
			return "LongMarginStep";
		if ( ! LongMaintMarginStep.isValid())
			return "LongMaintMarginStep";
		if ( ! LongRiskLimit.isValid())
			return "LongRiskLimit";
		if ( ! LongRiskStep.isValid())
			return "LongRiskStep";
		if ( ! ShortMarginRatio.isValid())
			return "ShortMarginRatio";
		if ( ! ShortMaintMarginRatio.isValid())
			return "ShortMaintMarginRatio";
		if ( ! ShortMarginStep.isValid())
			return "ShortMarginStep";
		if ( ! ShortMaintMarginStep.isValid())
			return "ShortMaintMarginStep";
		if ( ! ShortRiskLimit.isValid())
			return "ShortRiskLimit";
		if ( ! ShortRiskStep.isValid())
			return "ShortRiskStep";
		if ( ! DefaultLeverage.isValid())
			return "DefaultLeverage";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableMarginRate *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///成员代码
	CLongIndexIDType MemberID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///取值方式
	CValueModeType ValueMode;
	///保证金数列
	CSequenceTypeType MarginSequenceType;
	///风险限额数列
	CSequenceTypeType RiskSequenceType;
	///多头起始保证金率
	CRatioType LongMarginRatio;
	///多头起始维持保证金率
	CRatioType LongMaintMarginRatio;
	///多头保证金率步长
	CRatioType LongMarginStep;
	///多头维持保证金率步长
	CRatioType LongMaintMarginStep;
	///多头起始风险限额
	CMoneyType LongRiskLimit;
	///多头风险限额步长
	CMoneyType LongRiskStep;
	///空头起始保证金率
	CRatioType ShortMarginRatio;
	///空头起始维持保证金率
	CRatioType ShortMaintMarginRatio;
	///空头保证金率步长
	CRatioType ShortMarginStep;
	///空头维持保证金率步长
	CRatioType ShortMaintMarginStep;
	///空头起始风险限额
	CMoneyType ShortRiskLimit;
	///空头风险限额步长
	CMoneyType ShortRiskStep;
	///默认杠杆倍数
	CLeverageType DefaultLeverage;
	///备注
	CLongNameType Remark;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	unsigned int HashMemberAndInstrument;
	
	///计算MemberAndInstrument的hash值
	void calHashMemberAndInstrument()
	{
		unsigned key=0;
		key=MemberID.hash(key);
		key=InstrumentID.hash(key);
		HashMemberAndInstrument=key;
	}
	
	///根据需要情况，更新MemberAndInstrument的hash值
	void updateHashMemberAndInstrument()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashMemberAndInstrument();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashMemberAndInstrument();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableMarginRate CMarginRate;

/////////////////////////////////////////////////////////////////////////
///CMarginRateActionTrigger是操作标的保证金率时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarginRateActionTrigger
{
public:
	///构造方法
	CMarginRateActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CMarginRateActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CMarginRateFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pMarginRate	要加入的MarginRate
	virtual void beforeAdd(CWriteableMarginRate *pMarginRate)
	{
		return;
	}
	
	///加入后触发
	///@param	pMarginRate	已经加入的MarginRate
	virtual void afterAdd(CMarginRate *pMarginRate)
	{
		return;
	}

	///更新前触发	
	///@param	pMarginRate	被刷新的CMarginRate
	///@param	pNewMarginRate	新的值
	virtual void beforeUpdate(CMarginRate *pMarginRate, CWriteableMarginRate *pNewMarginRate)
	{
		return;
	}
	
	///更新后触发
	///@param	pMarginRate	被刷新的CMarginRate
	virtual void afterUpdate(CMarginRate *pMarginRate)
	{
		return;
	}
	
	///删除前触发
	///@param	pMarginRate	要删除的CMarginRate
	virtual void beforeRemove(CMarginRate *pMarginRate)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CMarginRateFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CMarginRateCommitTrigger是确认标的保证金率时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarginRateCommitTrigger
{
public:
	///构造方法
	CMarginRateCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CMarginRateCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CMarginRateFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pMarginRate	已经加入的MarginRate
	virtual void commitAdd(CMarginRate *pMarginRate)
	{
		return;
	}

	///更新后触发
	///@param	pMarginRate	被刷新的CMarginRate
	///@param	poldMarginRate	原来的值
	virtual void commitUpdate(CMarginRate *pMarginRate, CWriteableMarginRate *pOldMarginRate)
	{
		return;
	}
	
	///删除后触发
	///@param	pMarginRate	已经删除的CMarginRate
	virtual void commitRemove(CWriteableMarginRate *pMarginRate)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CMarginRateFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CMarginRateIterator是一个对标的保证金率的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarginRateIterator
{
public:
	///构造方法
	CMarginRateIterator()
	{
	}

	///构造方法
	CMarginRateIterator(CMarginRateFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CMarginRateIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个MarginRate
	///@return	下一个满足条件CMarginRate，如果已经没有一个满足要求了，则返回NULL
	virtual CMarginRate *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CMarginRateFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pMarginRate 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableMarginRate * pMarginRate,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CMarginRateFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableCFDRate是一个存储CFD溢价率的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableCFDRate
{
public:
	CWriteableCFDRate()
	{
	}
	
	CWriteableCFDRate(const CWriteableCFDRate& c)
	{
		memcpy(this, &c, sizeof(CWriteableCFDRate));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! TriggerOrderType.isValid())
			return "TriggerOrderType";
		if ( ! RateBegin.isValid())
			return "RateBegin";
		if ( ! RateEnd.isValid())
			return "RateEnd";
		if ( ! CompVolume.isValid())
			return "CompVolume";
		if ( ! CompRate.isValid())
			return "CompRate";
		if ( ! HardRate.isValid())
			return "HardRate";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableCFDRate *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///成员代码或组
	CLongIndexIDType MemberID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///报单价格类型
	CLongIndexIDType TriggerOrderType;
	///多头起始溢价率
	CRatioType RateBegin;
	///多头最大溢价率
	CRatioType RateEnd;
	///点差比较基准
	CRatioType CompVolume;
	///点差比较基准率
	CRatioType CompRate;
	///硬顶率
	CRatioType HardRate;
	///备注
	CLongNameType Remark;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	unsigned int HashMemberAndInstrument;
	
	///计算MemberAndInstrument的hash值
	void calHashMemberAndInstrument()
	{
		unsigned key=0;
		key=MemberID.hash(key);
		key=InstrumentID.hash(key);
		key=TriggerOrderType.hash(key);
		HashMemberAndInstrument=key;
	}
	
	///根据需要情况，更新MemberAndInstrument的hash值
	void updateHashMemberAndInstrument()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashMemberAndInstrument();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashMemberAndInstrument();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableCFDRate CCFDRate;

/////////////////////////////////////////////////////////////////////////
///CCFDRateActionTrigger是操作CFD溢价率时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCFDRateActionTrigger
{
public:
	///构造方法
	CCFDRateActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CCFDRateActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CCFDRateFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pCFDRate	要加入的CFDRate
	virtual void beforeAdd(CWriteableCFDRate *pCFDRate)
	{
		return;
	}
	
	///加入后触发
	///@param	pCFDRate	已经加入的CFDRate
	virtual void afterAdd(CCFDRate *pCFDRate)
	{
		return;
	}

	///更新前触发	
	///@param	pCFDRate	被刷新的CCFDRate
	///@param	pNewCFDRate	新的值
	virtual void beforeUpdate(CCFDRate *pCFDRate, CWriteableCFDRate *pNewCFDRate)
	{
		return;
	}
	
	///更新后触发
	///@param	pCFDRate	被刷新的CCFDRate
	virtual void afterUpdate(CCFDRate *pCFDRate)
	{
		return;
	}
	
	///删除前触发
	///@param	pCFDRate	要删除的CCFDRate
	virtual void beforeRemove(CCFDRate *pCFDRate)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CCFDRateFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CCFDRateCommitTrigger是确认CFD溢价率时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCFDRateCommitTrigger
{
public:
	///构造方法
	CCFDRateCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CCFDRateCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CCFDRateFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pCFDRate	已经加入的CFDRate
	virtual void commitAdd(CCFDRate *pCFDRate)
	{
		return;
	}

	///更新后触发
	///@param	pCFDRate	被刷新的CCFDRate
	///@param	poldCFDRate	原来的值
	virtual void commitUpdate(CCFDRate *pCFDRate, CWriteableCFDRate *pOldCFDRate)
	{
		return;
	}
	
	///删除后触发
	///@param	pCFDRate	已经删除的CCFDRate
	virtual void commitRemove(CWriteableCFDRate *pCFDRate)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CCFDRateFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CCFDRateIterator是一个对CFD溢价率的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCFDRateIterator
{
public:
	///构造方法
	CCFDRateIterator()
	{
	}

	///构造方法
	CCFDRateIterator(CCFDRateFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CCFDRateIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个CFDRate
	///@return	下一个满足条件CCFDRate，如果已经没有一个满足要求了，则返回NULL
	virtual CCFDRate *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CCFDRateFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pCFDRate 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableCFDRate * pCFDRate,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CCFDRateFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableFee是一个存储手续费率的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableFee
{
public:
	CWriteableFee()
	{
	}
	
	CWriteableFee(const CWriteableFee& c)
	{
		memcpy(this, &c, sizeof(CWriteableFee));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! MatchRole.isValid())
			return "MatchRole";
		if ( ! OpenFeeRate.isValid())
			return "OpenFeeRate";
		if ( ! OpenFeeAmount.isValid())
			return "OpenFeeAmount";
		if ( ! CloseFeeRate.isValid())
			return "CloseFeeRate";
		if ( ! CloseFeeAmount.isValid())
			return "CloseFeeAmount";
		if ( ! CloseTodayFeeRate.isValid())
			return "CloseTodayFeeRate";
		if ( ! CloseTodayFeeAmount.isValid())
			return "CloseTodayFeeAmount";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableFee *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///成员代码
	CLongIndexIDType MemberID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///成交角色
	CMatchRoleType MatchRole;
	///开仓手续费按比例
	CRatioType OpenFeeRate;
	///开仓手续费按手数
	CRatioType OpenFeeAmount;
	///平仓手续费按比例
	CRatioType CloseFeeRate;
	///平仓手续费按手数
	CRatioType CloseFeeAmount;
	///平今仓手续费按比例
	CRatioType CloseTodayFeeRate;
	///平今仓手续费按手数
	CRatioType CloseTodayFeeAmount;
	///备注
	CLongNameType Remark;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	unsigned int HashMemberIDAndInstrument;
	
	///计算MemberIDAndInstrument的hash值
	void calHashMemberIDAndInstrument()
	{
		unsigned key=0;
		key=MemberID.hash(key);
		key=InstrumentID.hash(key);
		key=MatchRole.hash(key);
		HashMemberIDAndInstrument=key;
	}
	
	///根据需要情况，更新MemberIDAndInstrument的hash值
	void updateHashMemberIDAndInstrument()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashMemberIDAndInstrument();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashMemberIDAndInstrument();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableFee CFee;

/////////////////////////////////////////////////////////////////////////
///CFeeActionTrigger是操作手续费率时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CFeeActionTrigger
{
public:
	///构造方法
	CFeeActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CFeeActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CFeeFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pFee	要加入的Fee
	virtual void beforeAdd(CWriteableFee *pFee)
	{
		return;
	}
	
	///加入后触发
	///@param	pFee	已经加入的Fee
	virtual void afterAdd(CFee *pFee)
	{
		return;
	}

	///更新前触发	
	///@param	pFee	被刷新的CFee
	///@param	pNewFee	新的值
	virtual void beforeUpdate(CFee *pFee, CWriteableFee *pNewFee)
	{
		return;
	}
	
	///更新后触发
	///@param	pFee	被刷新的CFee
	virtual void afterUpdate(CFee *pFee)
	{
		return;
	}
	
	///删除前触发
	///@param	pFee	要删除的CFee
	virtual void beforeRemove(CFee *pFee)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CFeeFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CFeeCommitTrigger是确认手续费率时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CFeeCommitTrigger
{
public:
	///构造方法
	CFeeCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CFeeCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CFeeFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pFee	已经加入的Fee
	virtual void commitAdd(CFee *pFee)
	{
		return;
	}

	///更新后触发
	///@param	pFee	被刷新的CFee
	///@param	poldFee	原来的值
	virtual void commitUpdate(CFee *pFee, CWriteableFee *pOldFee)
	{
		return;
	}
	
	///删除后触发
	///@param	pFee	已经删除的CFee
	virtual void commitRemove(CWriteableFee *pFee)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CFeeFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CFeeIterator是一个对手续费率的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CFeeIterator
{
public:
	///构造方法
	CFeeIterator()
	{
	}

	///构造方法
	CFeeIterator(CFeeFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CFeeIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个Fee
	///@return	下一个满足条件CFee，如果已经没有一个满足要求了，则返回NULL
	virtual CFee *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CFeeFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pFee 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableFee * pFee,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CFeeFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableCurrency是一个存储资金账户币种信息的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableCurrency
{
public:
	CWriteableCurrency()
	{
		pMarketData=NULL;
	}
	
	CWriteableCurrency(const CWriteableCurrency& c)
	{
		memcpy(this, &c, sizeof(CWriteableCurrency));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! SettlementGroup.isValid())
			return "SettlementGroup";
		if ( ! Currency.isValid())
			return "Currency";
		if ( ! Deposit.isValid())
			return "Deposit";
		if ( ! Withdraw.isValid())
			return "Withdraw";
		if ( ! Balance.isValid())
			return "Balance";
		if ( ! PreBalance.isValid())
			return "PreBalance";
		if ( ! SortNumber.isValid())
			return "SortNumber";
		if ( ! CanReduce.isValid())
			return "CanReduce";
		if ( ! IsTrading.isValid())
			return "IsTrading";
		if ( ! SettleSegment.isValid())
			return "SettleSegment";
		if ( ! MoneyTick.isValid())
			return "MoneyTick";
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! LoanUse.isValid())
			return "LoanUse";
		if ( ! LoanLimit.isValid())
			return "LoanLimit";
		if ( ! LoanBorrow.isValid())
			return "LoanBorrow";
		if ( ! LoanDeposit.isValid())
			return "LoanDeposit";
		if ( ! LoanBase.isValid())
			return "LoanBase";
		if ( ! LoanRate.isValid())
			return "LoanRate";
		if ( ! LoanHighBase.isValid())
			return "LoanHighBase";
		if ( ! LoanHighRate.isValid())
			return "LoanHighRate";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! CurrValue.isValid())
			return "CurrValue";
		if ( ! HairCut.isValid())
			return "HairCut";
		if ( ! MaxValue.isValid())
			return "MaxValue";
		if ( ! MarginRateGroup.isValid())
			return "MarginRateGroup";
		if ( ! PrivateEnd.isValid())
			return "PrivateEnd";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableCurrency *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///结算组编号
	CShortIndexIDType SettlementGroup;
	///币种
	CCurrencyType Currency;
	///入金金额
	CMoneyType Deposit;
	///出金金额
	CMoneyType Withdraw;
	///静态权益
	CMoneyType Balance;
	///上次静态权益
	CMoneyType PreBalance;
	///排序字段
	CNumberType SortNumber;
	///是否可以出金
	CBoolType CanReduce;
	///是否允许相关交易
	CBoolType IsTrading;
	///结算段
	CIndexIDType SettleSegment;
	///最小单位
	CMoneyType MoneyTick;
	///价格参考交易所代码
	CExchangeIDType ExchangeID;
	///价格参考标的代码
	CInstrumentIDType InstrumentID;
	///借款使用量
	CMoneyType LoanUse;
	///最大可借比例
	CRatioType LoanLimit;
	///借款利息
	CRatioType LoanBorrow;
	///存款利息
	CRatioType LoanDeposit;
	///利息起点
	CRatioType LoanBase;
	///利息斜率
	CRatioType LoanRate;
	///高利息起点
	CRatioType LoanHighBase;
	///高利息斜率
	CRatioType LoanHighRate;
	///备注
	CLongNameType Remark;
	///当前价值
	CMoneyType CurrValue;
	///抵押比例
	CRatioType HairCut;
	///最大抵押数额
	CMoneyType MaxValue;
	///保证金组
	CShortIndexIDType MarginRateGroup;
	///私有化停止交易线
	CRatioType PrivateEnd;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	unsigned int HashCurrency;
	
	///计算Currency的hash值
	void calHashCurrency()
	{
		unsigned key=0;
		key=Currency.hash(key);
		key=SettlementGroup.hash(key);
		HashCurrency=key;
	}
	
	///根据需要情况，更新Currency的hash值
	void updateHashCurrency()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashCurrency();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashCurrency();
	}
	
	///构造对应的行情的指针
	///@param	pFactory	构造对应的行情的指针时，寻找的对象工厂
	///@return	找到的对应的行情的指针
	const CWriteableMarketData* linkMarketData() const;

	///构造对应的行情指针
	///@param	pMarketData	要连接到的对象
	///@return	输入的pMarketData
	const CWriteableMarketData* linkMarketData(const CWriteableMarketData *pMarketData) const
	{
		void *target=(void *)(&(this->pMarketData));
		*((const CWriteableMarketData **)target)=pMarketData;
		return pMarketData;
	}
	
	///获取对应的行情的指针
	///@return	找到的对应的行情的指针
	const CWriteableMarketData* getMarketData() const
	{
		if(pMarketData==NULL)
			return linkMarketData();
		return pMarketData;
	}
	
	CMDB* m_pMDB;
private:
	///对应的行情
	const CWriteableMarketData *pMarketData;
};

typedef const CWriteableCurrency CCurrency;

/////////////////////////////////////////////////////////////////////////
///CCurrencyActionTrigger是操作资金账户币种信息时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCurrencyActionTrigger
{
public:
	///构造方法
	CCurrencyActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CCurrencyActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CCurrencyFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pCurrency	要加入的Currency
	virtual void beforeAdd(CWriteableCurrency *pCurrency)
	{
		return;
	}
	
	///加入后触发
	///@param	pCurrency	已经加入的Currency
	virtual void afterAdd(CCurrency *pCurrency)
	{
		return;
	}

	///更新前触发	
	///@param	pCurrency	被刷新的CCurrency
	///@param	pNewCurrency	新的值
	virtual void beforeUpdate(CCurrency *pCurrency, CWriteableCurrency *pNewCurrency)
	{
		return;
	}
	
	///更新后触发
	///@param	pCurrency	被刷新的CCurrency
	virtual void afterUpdate(CCurrency *pCurrency)
	{
		return;
	}
	
	///删除前触发
	///@param	pCurrency	要删除的CCurrency
	virtual void beforeRemove(CCurrency *pCurrency)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CCurrencyFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CCurrencyCommitTrigger是确认资金账户币种信息时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCurrencyCommitTrigger
{
public:
	///构造方法
	CCurrencyCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CCurrencyCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CCurrencyFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pCurrency	已经加入的Currency
	virtual void commitAdd(CCurrency *pCurrency)
	{
		return;
	}

	///更新后触发
	///@param	pCurrency	被刷新的CCurrency
	///@param	poldCurrency	原来的值
	virtual void commitUpdate(CCurrency *pCurrency, CWriteableCurrency *pOldCurrency)
	{
		return;
	}
	
	///删除后触发
	///@param	pCurrency	已经删除的CCurrency
	virtual void commitRemove(CWriteableCurrency *pCurrency)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CCurrencyFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CCurrencyIterator是一个对资金账户币种信息的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCurrencyIterator
{
public:
	///构造方法
	CCurrencyIterator()
	{
	}

	///构造方法
	CCurrencyIterator(CCurrencyFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CCurrencyIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个Currency
	///@return	下一个满足条件CCurrency，如果已经没有一个满足要求了，则返回NULL
	virtual CCurrency *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CCurrencyFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pCurrency 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableCurrency * pCurrency,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CCurrencyFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableLiquidity是一个存储流动性设置表的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableLiquidity
{
public:
	CWriteableLiquidity()
	{
	}
	
	CWriteableLiquidity(const CWriteableLiquidity& c)
	{
		memcpy(this, &c, sizeof(CWriteableLiquidity));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! LiquidityGrade.isValid())
			return "LiquidityGrade";
		if ( ! LiquidityGroup.isValid())
			return "LiquidityGroup";
		if ( ! BasePriceType.isValid())
			return "BasePriceType";
		if ( ! VolumeBegin.isValid())
			return "VolumeBegin";
		if ( ! BuyOpen.isValid())
			return "BuyOpen";
		if ( ! BuyClose.isValid())
			return "BuyClose";
		if ( ! SellOpen.isValid())
			return "SellOpen";
		if ( ! SellClose.isValid())
			return "SellClose";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableLiquidity *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///流动性等级
	CShortIndexIDType LiquidityGrade;
	///标的代码
	CLongIndexIDType LiquidityGroup;
	///基于的价格
	CLiquidityPriceTypeType BasePriceType;
	///起始量
	CVolumeType VolumeBegin;
	///买开流动性比例
	CRatioType BuyOpen;
	///买平流动性比例
	CRatioType BuyClose;
	///卖开流动性比例
	CRatioType SellOpen;
	///卖平流动性比例
	CRatioType SellClose;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	unsigned int HashLiquidity;
	
	///计算Liquidity的hash值
	void calHashLiquidity()
	{
		unsigned key=0;
		key=LiquidityGrade.hash(key);
		key=LiquidityGroup.hash(key);
		HashLiquidity=key;
	}
	
	///根据需要情况，更新Liquidity的hash值
	void updateHashLiquidity()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashLiquidity();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashLiquidity();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableLiquidity CLiquidity;

/////////////////////////////////////////////////////////////////////////
///CLiquidityActionTrigger是操作流动性设置表时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CLiquidityActionTrigger
{
public:
	///构造方法
	CLiquidityActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CLiquidityActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CLiquidityFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pLiquidity	要加入的Liquidity
	virtual void beforeAdd(CWriteableLiquidity *pLiquidity)
	{
		return;
	}
	
	///加入后触发
	///@param	pLiquidity	已经加入的Liquidity
	virtual void afterAdd(CLiquidity *pLiquidity)
	{
		return;
	}

	///更新前触发	
	///@param	pLiquidity	被刷新的CLiquidity
	///@param	pNewLiquidity	新的值
	virtual void beforeUpdate(CLiquidity *pLiquidity, CWriteableLiquidity *pNewLiquidity)
	{
		return;
	}
	
	///更新后触发
	///@param	pLiquidity	被刷新的CLiquidity
	virtual void afterUpdate(CLiquidity *pLiquidity)
	{
		return;
	}
	
	///删除前触发
	///@param	pLiquidity	要删除的CLiquidity
	virtual void beforeRemove(CLiquidity *pLiquidity)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CLiquidityFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CLiquidityCommitTrigger是确认流动性设置表时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CLiquidityCommitTrigger
{
public:
	///构造方法
	CLiquidityCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CLiquidityCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CLiquidityFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pLiquidity	已经加入的Liquidity
	virtual void commitAdd(CLiquidity *pLiquidity)
	{
		return;
	}

	///更新后触发
	///@param	pLiquidity	被刷新的CLiquidity
	///@param	poldLiquidity	原来的值
	virtual void commitUpdate(CLiquidity *pLiquidity, CWriteableLiquidity *pOldLiquidity)
	{
		return;
	}
	
	///删除后触发
	///@param	pLiquidity	已经删除的CLiquidity
	virtual void commitRemove(CWriteableLiquidity *pLiquidity)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CLiquidityFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CLiquidityIterator是一个对流动性设置表的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CLiquidityIterator
{
public:
	///构造方法
	CLiquidityIterator()
	{
	}

	///构造方法
	CLiquidityIterator(CLiquidityFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CLiquidityIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个Liquidity
	///@return	下一个满足条件CLiquidity，如果已经没有一个满足要求了，则返回NULL
	virtual CLiquidity *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CLiquidityFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pLiquidity 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableLiquidity * pLiquidity,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CLiquidityFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableAccount是一个存储资金账户的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableAccount
{
public:
	CWriteableAccount()
	{
		pCurrency=NULL;
		pMember=NULL;
	}
	
	CWriteableAccount(const CWriteableAccount& c)
	{
		memcpy(this, &c, sizeof(CWriteableAccount));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! SettlementGroup.isValid())
			return "SettlementGroup";
		if ( ! AccountID.isValid())
			return "AccountID";
		if ( ! Currency.isValid())
			return "Currency";
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! Available.isValid())
			return "Available";
		if ( ! Withdrawable.isValid())
			return "Withdrawable";
		if ( ! PreBalance.isValid())
			return "PreBalance";
		if ( ! Balance.isValid())
			return "Balance";
		if ( ! PositionCost.isValid())
			return "PositionCost";
		if ( ! UseMargin.isValid())
			return "UseMargin";
		if ( ! CrossMargin.isValid())
			return "CrossMargin";
		if ( ! CloseProfit.isValid())
			return "CloseProfit";
		if ( ! Deposit.isValid())
			return "Deposit";
		if ( ! Withdraw.isValid())
			return "Withdraw";
		if ( ! FrozenMargin.isValid())
			return "FrozenMargin";
		if ( ! FrozenMoney.isValid())
			return "FrozenMoney";
		if ( ! FrozenFee.isValid())
			return "FrozenFee";
		if ( ! Fee.isValid())
			return "Fee";
		if ( ! MoneyChange.isValid())
			return "MoneyChange";
		if ( ! Reserve.isValid())
			return "Reserve";
		if ( ! ReserveAvailable.isValid())
			return "ReserveAvailable";
		if ( ! ReserveLimit.isValid())
			return "ReserveLimit";
		if ( ! ReserveProfit.isValid())
			return "ReserveProfit";
		if ( ! ReserveType.isValid())
			return "ReserveType";
		if ( ! ReserveActive.isValid())
			return "ReserveActive";
		if ( ! TotalCloseProfit.isValid())
			return "TotalCloseProfit";
		if ( ! TotalDeposit.isValid())
			return "TotalDeposit";
		if ( ! TotalWithdraw.isValid())
			return "TotalWithdraw";
		if ( ! TotalFee.isValid())
			return "TotalFee";
		if ( ! TotalMoneyChange.isValid())
			return "TotalMoneyChange";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		if ( ! AccountType.isValid())
			return "AccountType";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		if ( ! MarginRateGrade.isValid())
			return "MarginRateGrade";
		if ( ! TradingRightGrade.isValid())
			return "TradingRightGrade";
		if ( ! Frequency.isValid())
			return "Frequency";
		if ( ! MaintMargin.isValid())
			return "MaintMargin";
		if ( ! UnRealProfit.isValid())
			return "UnRealProfit";
		if ( ! CopyProfit.isValid())
			return "CopyProfit";
		if ( ! CFDType.isValid())
			return "CFDType";
		if ( ! FeeGrade.isValid())
			return "FeeGrade";
		if ( ! CFDGrade.isValid())
			return "CFDGrade";
		if ( ! Borrow.isValid())
			return "Borrow";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! MaxLocalID.isValid())
			return "MaxLocalID";
		if ( ! CurrValue.isValid())
			return "CurrValue";
		if ( ! CanReduce.isValid())
			return "CanReduce";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableAccount *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///结算组编号
	CShortIndexIDType SettlementGroup;
	///资金账号
	CAccountIDType AccountID;
	///币种
	CCurrencyType Currency;
	///成员代码
	CLongIndexIDType MemberID;
	///可用资金
	CMoneyType Available;
	///可取资金
	CMoneyType Withdrawable;
	///上次静态权益
	CMoneyType PreBalance;
	///静态权益
	CMoneyType Balance;
	///持仓成本
	CMoneyType PositionCost;
	///所有占用保证金
	CMoneyType UseMargin;
	///全仓保证金
	CMoneyType CrossMargin;
	///平仓盈亏
	CMoneyType CloseProfit;
	///入金金额
	CMoneyType Deposit;
	///出金金额
	CMoneyType Withdraw;
	///冻结保证金
	CMoneyType FrozenMargin;
	///冻结资金
	CMoneyType FrozenMoney;
	///冻结手续费
	CMoneyType FrozenFee;
	///手续费
	CMoneyType Fee;
	///资金变化
	CMoneyType MoneyChange;
	///保留资金
	CMoneyType Reserve;
	///保留资金可用
	CMoneyType ReserveAvailable;
	///保留资金限制
	CMoneyType ReserveLimit;
	///保留资金盈亏
	CMoneyType ReserveProfit;
	///保留资金类型
	CReserveTypeType ReserveType;
	///保留资金是否活跃
	CBoolType ReserveActive;
	///总平仓盈亏
	CTotalType TotalCloseProfit;
	///总入金金额
	CTotalType TotalDeposit;
	///总出金金额
	CTotalType TotalWithdraw;
	///总手续费
	CTotalType TotalFee;
	///总资金变化
	CTotalType TotalMoneyChange;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	///账户类型
	CShortIndexIDType AccountType;
	///业务序列号
	CLargeNumberType BusinessNo;
	///保证金等级
	CShortIndexIDType MarginRateGrade;
	///权限等级
	CShortIndexIDType TradingRightGrade;
	///每秒更新的次数
	CNumberType Frequency;
	///维持保证金
	CMoneyType MaintMargin;
	///未实现盈亏
	CMoneyType UnRealProfit;
	///带单盈利分配
	CMoneyType CopyProfit;
	///点差接管类型
	CCFDTypeType CFDType;
	///手续费等级
	CShortIndexIDType FeeGrade;
	///差价等级
	CShortIndexIDType CFDGrade;
	///借款周期结算量
	CMoneyType Borrow;
	///备注
	CNameType Remark;
	///最大本地标识
	CIndexIDType MaxLocalID;
	///当前价值
	CMoneyType CurrValue;
	///是否可以出金
	CBoolType CanReduce;
	
	
	///计算所有的hash值
	void calAllHash()
	{
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
	}
	
	///构造币种的指针
	///@param	pFactory	构造币种的指针时，寻找的对象工厂
	///@return	找到的币种的指针
	const CWriteableCurrency* linkCurrency() const;

	///构造币种指针
	///@param	pCurrency	要连接到的对象
	///@return	输入的pCurrency
	const CWriteableCurrency* linkCurrency(const CWriteableCurrency *pCurrency) const
	{
		void *target=(void *)(&(this->pCurrency));
		*((const CWriteableCurrency **)target)=pCurrency;
		return pCurrency;
	}
	
	///获取币种的指针
	///@return	找到的币种的指针
	const CWriteableCurrency* getCurrency() const
	{
		if(pCurrency==NULL)
			return linkCurrency();
		return pCurrency;
	}
	
	///构造对应的用户的指针
	///@param	pFactory	构造对应的用户的指针时，寻找的对象工厂
	///@return	找到的对应的用户的指针
	const CWriteableMember* linkMember() const;

	///构造对应的用户指针
	///@param	pMember	要连接到的对象
	///@return	输入的pMember
	const CWriteableMember* linkMember(const CWriteableMember *pMember) const
	{
		void *target=(void *)(&(this->pMember));
		*((const CWriteableMember **)target)=pMember;
		return pMember;
	}
	
	///获取对应的用户的指针
	///@return	找到的对应的用户的指针
	const CWriteableMember* getMember() const
	{
		if(pMember==NULL)
			return linkMember();
		return pMember;
	}
	
	CMDB* m_pMDB;
private:
	///币种
	const CWriteableCurrency *pCurrency;
	///对应的用户
	const CWriteableMember *pMember;
};

typedef const CWriteableAccount CAccount;

/////////////////////////////////////////////////////////////////////////
///CAccountActionTrigger是操作资金账户时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountActionTrigger
{
public:
	///构造方法
	CAccountActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CAccountActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CAccountFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pAccount	要加入的Account
	virtual void beforeAdd(CWriteableAccount *pAccount)
	{
		return;
	}
	
	///加入后触发
	///@param	pAccount	已经加入的Account
	virtual void afterAdd(CAccount *pAccount)
	{
		return;
	}

	///更新前触发	
	///@param	pAccount	被刷新的CAccount
	///@param	pNewAccount	新的值
	virtual void beforeUpdate(CAccount *pAccount, CWriteableAccount *pNewAccount)
	{
		return;
	}
	
	///更新后触发
	///@param	pAccount	被刷新的CAccount
	virtual void afterUpdate(CAccount *pAccount)
	{
		return;
	}
	
	///删除前触发
	///@param	pAccount	要删除的CAccount
	virtual void beforeRemove(CAccount *pAccount)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CAccountFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CAccountCommitTrigger是确认资金账户时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountCommitTrigger
{
public:
	///构造方法
	CAccountCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CAccountCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CAccountFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pAccount	已经加入的Account
	virtual void commitAdd(CAccount *pAccount)
	{
		return;
	}

	///更新后触发
	///@param	pAccount	被刷新的CAccount
	///@param	poldAccount	原来的值
	virtual void commitUpdate(CAccount *pAccount, CWriteableAccount *pOldAccount)
	{
		return;
	}
	
	///删除后触发
	///@param	pAccount	已经删除的CAccount
	virtual void commitRemove(CWriteableAccount *pAccount)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CAccountFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CAccountIterator是一个对资金账户的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountIterator
{
public:
	///构造方法
	CAccountIterator()
	{
	}

	///构造方法
	CAccountIterator(CAccountFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CAccountIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个Account
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	virtual CAccount *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CAccountFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pAccount 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableAccount * pAccount,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CAccountFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableMemberPosition是一个存储成员总持仓的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableMemberPosition
{
public:
	CWriteableMemberPosition()
	{
	}
	
	CWriteableMemberPosition(const CWriteableMemberPosition& c)
	{
		memcpy(this, &c, sizeof(CWriteableMemberPosition));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! AccountID.isValid())
			return "AccountID";
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! Position.isValid())
			return "Position";
		if ( ! LongFrozen.isValid())
			return "LongFrozen";
		if ( ! ShortFrozen.isValid())
			return "ShortFrozen";
		if ( ! DefaultLeverage.isValid())
			return "DefaultLeverage";
		if ( ! DefaultCrossMargin.isValid())
			return "DefaultCrossMargin";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableMemberPosition *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///成员代码
	CLongIndexIDType MemberID;
	///资金账号
	CAccountIDType AccountID;
	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///总持仓
	CVolumeType Position;
	///多头冻结
	CVolumeType LongFrozen;
	///空头冻结
	CVolumeType ShortFrozen;
	///默认杠杆倍数
	CLeverageType DefaultLeverage;
	///默认是否全仓
	CBoolType DefaultCrossMargin;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	unsigned int HashInstrument;
	
	///计算Instrument的hash值
	void calHashInstrument()
	{
		unsigned key=0;
		key=AccountID.hash(key);
		key=ExchangeID.hash(key);
		key=InstrumentID.hash(key);
		HashInstrument=key;
	}
	
	///根据需要情况，更新Instrument的hash值
	void updateHashInstrument()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashInstrument();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashInstrument();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableMemberPosition CMemberPosition;

/////////////////////////////////////////////////////////////////////////
///CMemberPositionActionTrigger是操作成员总持仓时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMemberPositionActionTrigger
{
public:
	///构造方法
	CMemberPositionActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CMemberPositionActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CMemberPositionFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pMemberPosition	要加入的MemberPosition
	virtual void beforeAdd(CWriteableMemberPosition *pMemberPosition)
	{
		return;
	}
	
	///加入后触发
	///@param	pMemberPosition	已经加入的MemberPosition
	virtual void afterAdd(CMemberPosition *pMemberPosition)
	{
		return;
	}

	///更新前触发	
	///@param	pMemberPosition	被刷新的CMemberPosition
	///@param	pNewMemberPosition	新的值
	virtual void beforeUpdate(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pNewMemberPosition)
	{
		return;
	}
	
	///更新后触发
	///@param	pMemberPosition	被刷新的CMemberPosition
	virtual void afterUpdate(CMemberPosition *pMemberPosition)
	{
		return;
	}
	
	///删除前触发
	///@param	pMemberPosition	要删除的CMemberPosition
	virtual void beforeRemove(CMemberPosition *pMemberPosition)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CMemberPositionFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CMemberPositionCommitTrigger是确认成员总持仓时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMemberPositionCommitTrigger
{
public:
	///构造方法
	CMemberPositionCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CMemberPositionCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CMemberPositionFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pMemberPosition	已经加入的MemberPosition
	virtual void commitAdd(CMemberPosition *pMemberPosition)
	{
		return;
	}

	///更新后触发
	///@param	pMemberPosition	被刷新的CMemberPosition
	///@param	poldMemberPosition	原来的值
	virtual void commitUpdate(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pOldMemberPosition)
	{
		return;
	}
	
	///删除后触发
	///@param	pMemberPosition	已经删除的CMemberPosition
	virtual void commitRemove(CWriteableMemberPosition *pMemberPosition)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CMemberPositionFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CMemberPositionIterator是一个对成员总持仓的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMemberPositionIterator
{
public:
	///构造方法
	CMemberPositionIterator()
	{
	}

	///构造方法
	CMemberPositionIterator(CMemberPositionFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CMemberPositionIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个MemberPosition
	///@return	下一个满足条件CMemberPosition，如果已经没有一个满足要求了，则返回NULL
	virtual CMemberPosition *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CMemberPositionFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pMemberPosition 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableMemberPosition * pMemberPosition,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CMemberPositionFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteablePosition是一个存储交易单元持仓的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteablePosition
{
public:
	CWriteablePosition()
	{
		pInstrument=NULL;
		pAccount=NULL;
	}
	
	CWriteablePosition(const CWriteablePosition& c)
	{
		memcpy(this, &c, sizeof(CWriteablePosition));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! PositionID.isValid())
			return "PositionID";
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! TradeUnitID.isValid())
			return "TradeUnitID";
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! PositionClass.isValid())
			return "PositionClass";
		if ( ! PosiDirection.isValid())
			return "PosiDirection";
		if ( ! PrePosition.isValid())
			return "PrePosition";
		if ( ! Position.isValid())
			return "Position";
		if ( ! LongFrozen.isValid())
			return "LongFrozen";
		if ( ! ShortFrozen.isValid())
			return "ShortFrozen";
		if ( ! PreLongFrozen.isValid())
			return "PreLongFrozen";
		if ( ! PreShortFrozen.isValid())
			return "PreShortFrozen";
		if ( ! HighestPosition.isValid())
			return "HighestPosition";
		if ( ! ClosePosition.isValid())
			return "ClosePosition";
		if ( ! PositionCost.isValid())
			return "PositionCost";
		if ( ! CostPrice.isValid())
			return "CostPrice";
		if ( ! UseMargin.isValid())
			return "UseMargin";
		if ( ! FrozenMargin.isValid())
			return "FrozenMargin";
		if ( ! LongFrozenMargin.isValid())
			return "LongFrozenMargin";
		if ( ! ShortFrozenMargin.isValid())
			return "ShortFrozenMargin";
		if ( ! CloseProfit.isValid())
			return "CloseProfit";
		if ( ! TotalPositionCost.isValid())
			return "TotalPositionCost";
		if ( ! TotalCloseProfit.isValid())
			return "TotalCloseProfit";
		if ( ! OpenPrice.isValid())
			return "OpenPrice";
		if ( ! ClosePrice.isValid())
			return "ClosePrice";
		if ( ! TradeFee.isValid())
			return "TradeFee";
		if ( ! PositionFee.isValid())
			return "PositionFee";
		if ( ! Leverage.isValid())
			return "Leverage";
		if ( ! AccountID.isValid())
			return "AccountID";
		if ( ! PriceCurrency.isValid())
			return "PriceCurrency";
		if ( ! ClearCurrency.isValid())
			return "ClearCurrency";
		if ( ! SettlementGroup.isValid())
			return "SettlementGroup";
		if ( ! IsCrossMargin.isValid())
			return "IsCrossMargin";
		if ( ! CloseOrderID.isValid())
			return "CloseOrderID";
		if ( ! SLTriggerPrice.isValid())
			return "SLTriggerPrice";
		if ( ! TPTriggerPrice.isValid())
			return "TPTriggerPrice";
		if ( ! BeginTime.isValid())
			return "BeginTime";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! LastOpenTime.isValid())
			return "LastOpenTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		if ( ! IsAutoAddMargin.isValid())
			return "IsAutoAddMargin";
		if ( ! Frequency.isValid())
			return "Frequency";
		if ( ! MaintMargin.isValid())
			return "MaintMargin";
		if ( ! UnRealProfit.isValid())
			return "UnRealProfit";
		if ( ! LiquidPrice.isValid())
			return "LiquidPrice";
		if ( ! CreateTime.isValid())
			return "CreateTime";
		if ( ! CopyMemberID.isValid())
			return "CopyMemberID";
		if ( ! CopyProfitRate.isValid())
			return "CopyProfitRate";
		if ( ! CopyProfit.isValid())
			return "CopyProfit";
		if ( ! FirstTradeID.isValid())
			return "FirstTradeID";
		if ( ! LastTradeID.isValid())
			return "LastTradeID";
		if ( ! BusinessType.isValid())
			return "BusinessType";
		if ( ! BusinessValue.isValid())
			return "BusinessValue";
		if ( ! Reserve.isValid())
			return "Reserve";
		if ( ! ReserveProfit.isValid())
			return "ReserveProfit";
		if ( ! Remark.isValid())
			return "Remark";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteablePosition *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///持仓代码
	CLongIndexIDType PositionID;
	///成员代码
	CLongIndexIDType MemberID;
	///交易单元代码
	CIndexIDType TradeUnitID;
	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///持仓类型
	CPositionClassType PositionClass;
	///持仓多空方向
	CPosiDirectionType PosiDirection;
	///上次持仓
	CVolumeType PrePosition;
	///总持仓
	CVolumeType Position;
	///多头冻结
	CVolumeType LongFrozen;
	///空头冻结
	CVolumeType ShortFrozen;
	///昨日多头冻结
	CVolumeType PreLongFrozen;
	///昨日空头冻结
	CVolumeType PreShortFrozen;
	///最大持仓
	CVolumeType HighestPosition;
	///可平持仓
	CVolumeType ClosePosition;
	///持仓成本
	CCostType PositionCost;
	///成本价
	CPriceType CostPrice;
	///占用保证金
	CMoneyType UseMargin;
	///冻结保证金
	CMoneyType FrozenMargin;
	///多头冻结保证金
	CMoneyType LongFrozenMargin;
	///空头冻结保证金
	CMoneyType ShortFrozenMargin;
	///平仓盈亏
	CMoneyType CloseProfit;
	///开仓成本
	CTotalType TotalPositionCost;
	///总平仓盈亏
	CTotalType TotalCloseProfit;
	///开仓均价
	CPriceType OpenPrice;
	///平仓均价
	CPriceType ClosePrice;
	///交易费用
	CMoneyType TradeFee;
	///持仓费用或者股票分红等
	CMoneyType PositionFee;
	///杠杆倍数
	CLeverageType Leverage;
	///资金账号
	CAccountIDType AccountID;
	///计价币种
	CCurrencyType PriceCurrency;
	///清算币种
	CCurrencyType ClearCurrency;
	///结算组编号
	CShortIndexIDType SettlementGroup;
	///是否全仓
	CBoolType IsCrossMargin;
	///平仓单代码
	CIndexIDType CloseOrderID;
	///止损触发价
	CPriceType SLTriggerPrice;
	///止盈触发价
	CPriceType TPTriggerPrice;
	///持仓不为0的开始时间
	CMilliSecondsType BeginTime;
	///插入时间(新持仓时间)
	CMilliSecondsType InsertTime;
	///最后开仓时间
	CMilliSecondsType LastOpenTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	///业务序列号
	CLargeNumberType BusinessNo;
	///是否自动追加保证金
	CBoolType IsAutoAddMargin;
	///每秒更新的次数
	CNumberType Frequency;
	///维持保证金
	CMoneyType MaintMargin;
	///未实现盈亏
	CMoneyType UnRealProfit;
	///清算价格
	CPriceType LiquidPrice;
	///创建时间
	CIndexIDType CreateTime;
	///带单员代码
	CLongIndexIDType CopyMemberID;
	///带单分配比例
	CRatioType CopyProfitRate;
	///带单盈利分配
	CMoneyType CopyProfit;
	///初始成交订单号
	CLongIndexIDType FirstTradeID;
	///结束成交订单号
	CLongIndexIDType LastTradeID;
	///业务类别
	CBusinessTypeType BusinessType;
	///业务值
	CNameType BusinessValue;
	///保留资金
	CMoneyType Reserve;
	///保留资金盈亏
	CMoneyType ReserveProfit;
	///备注
	CNameType Remark;
	
	unsigned int HashPositionID;
	
	///计算PositionID的hash值
	void calHashPositionID()
	{
		unsigned key=0;
		key=PositionID.hash(key);
		HashPositionID=key;
	}
	
	///根据需要情况，更新PositionID的hash值
	void updateHashPositionID()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashPositionID();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashPositionID();
	}
	
	///构造对应的标的的指针
	///@param	pFactory	构造对应的标的的指针时，寻找的对象工厂
	///@return	找到的对应的标的的指针
	const CWriteableInstrument* linkInstrument() const;

	///构造对应的标的指针
	///@param	pInstrument	要连接到的对象
	///@return	输入的pInstrument
	const CWriteableInstrument* linkInstrument(const CWriteableInstrument *pInstrument) const
	{
		void *target=(void *)(&(this->pInstrument));
		*((const CWriteableInstrument **)target)=pInstrument;
		return pInstrument;
	}
	
	///获取对应的标的的指针
	///@return	找到的对应的标的的指针
	const CWriteableInstrument* getInstrument() const
	{
		if(pInstrument==NULL)
			return linkInstrument();
		return pInstrument;
	}
	
	///构造对应的自己账号的指针
	///@param	pFactory	构造对应的自己账号的指针时，寻找的对象工厂
	///@return	找到的对应的自己账号的指针
	const CWriteableAccount* linkAccount() const;

	///构造对应的自己账号指针
	///@param	pAccount	要连接到的对象
	///@return	输入的pAccount
	const CWriteableAccount* linkAccount(const CWriteableAccount *pAccount) const
	{
		void *target=(void *)(&(this->pAccount));
		*((const CWriteableAccount **)target)=pAccount;
		return pAccount;
	}
	
	///获取对应的自己账号的指针
	///@return	找到的对应的自己账号的指针
	const CWriteableAccount* getAccount() const
	{
		if(pAccount==NULL)
			return linkAccount();
		return pAccount;
	}
	
	CMDB* m_pMDB;
private:
	///对应的标的
	const CWriteableInstrument *pInstrument;
	///对应的自己账号
	const CWriteableAccount *pAccount;
};

typedef const CWriteablePosition CPosition;

/////////////////////////////////////////////////////////////////////////
///CPositionActionTrigger是操作交易单元持仓时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionActionTrigger
{
public:
	///构造方法
	CPositionActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CPositionActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CPositionFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pPosition	要加入的Position
	virtual void beforeAdd(CWriteablePosition *pPosition)
	{
		return;
	}
	
	///加入后触发
	///@param	pPosition	已经加入的Position
	virtual void afterAdd(CPosition *pPosition)
	{
		return;
	}

	///更新前触发	
	///@param	pPosition	被刷新的CPosition
	///@param	pNewPosition	新的值
	virtual void beforeUpdate(CPosition *pPosition, CWriteablePosition *pNewPosition)
	{
		return;
	}
	
	///更新后触发
	///@param	pPosition	被刷新的CPosition
	virtual void afterUpdate(CPosition *pPosition)
	{
		return;
	}
	
	///删除前触发
	///@param	pPosition	要删除的CPosition
	virtual void beforeRemove(CPosition *pPosition)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CPositionFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CPositionCommitTrigger是确认交易单元持仓时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionCommitTrigger
{
public:
	///构造方法
	CPositionCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CPositionCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CPositionFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pPosition	已经加入的Position
	virtual void commitAdd(CPosition *pPosition)
	{
		return;
	}

	///更新后触发
	///@param	pPosition	被刷新的CPosition
	///@param	poldPosition	原来的值
	virtual void commitUpdate(CPosition *pPosition, CWriteablePosition *pOldPosition)
	{
		return;
	}
	
	///删除后触发
	///@param	pPosition	已经删除的CPosition
	virtual void commitRemove(CWriteablePosition *pPosition)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CPositionFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CPositionIterator是一个对交易单元持仓的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionIterator
{
public:
	///构造方法
	CPositionIterator()
	{
	}

	///构造方法
	CPositionIterator(CPositionFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CPositionIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个Position
	///@return	下一个满足条件CPosition，如果已经没有一个满足要求了，则返回NULL
	virtual CPosition *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CPositionFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pPosition 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteablePosition * pPosition,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CPositionFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableMarketData是一个存储行情的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableMarketData
{
public:
	CWriteableMarketData()
	{
		pInstrument=NULL;
	}
	
	CWriteableMarketData(const CWriteableMarketData& c)
	{
		memcpy(this, &c, sizeof(CWriteableMarketData));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! ProductGroup.isValid())
			return "ProductGroup";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		if ( ! PreClosePrice.isValid())
			return "PreClosePrice";
		if ( ! PreSettlementPrice.isValid())
			return "PreSettlementPrice";
		if ( ! PreOpenInterest.isValid())
			return "PreOpenInterest";
		if ( ! PrePositionFeeRate.isValid())
			return "PrePositionFeeRate";
		if ( ! ClosePrice.isValid())
			return "ClosePrice";
		if ( ! PositionFeeTime.isValid())
			return "PositionFeeTime";
		if ( ! SettlementTime.isValid())
			return "SettlementTime";
		if ( ! UpperLimitPrice.isValid())
			return "UpperLimitPrice";
		if ( ! LowerLimitPrice.isValid())
			return "LowerLimitPrice";
		if ( ! UnderlyingPrice.isValid())
			return "UnderlyingPrice";
		if ( ! MarkedPrice.isValid())
			return "MarkedPrice";
		if ( ! PositionFeeRate.isValid())
			return "PositionFeeRate";
		if ( ! HighestPrice.isValid())
			return "HighestPrice";
		if ( ! LowestPrice.isValid())
			return "LowestPrice";
		if ( ! LastPrice.isValid())
			return "LastPrice";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! Turnover.isValid())
			return "Turnover";
		if ( ! OpenInterest.isValid())
			return "OpenInterest";
		if ( ! SettlementPrice.isValid())
			return "SettlementPrice";
		if ( ! OpenPrice.isValid())
			return "OpenPrice";
		if ( ! IsPubMarketData.isValid())
			return "IsPubMarketData";
		if ( ! InstrumentStatus.isValid())
			return "InstrumentStatus";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		if ( ! MarkedPriceTotal.isValid())
			return "MarkedPriceTotal";
		if ( ! MarkedPriceWeight.isValid())
			return "MarkedPriceWeight";
		if ( ! L25UpperPrice.isValid())
			return "L25UpperPrice";
		if ( ! L25LowerPrice.isValid())
			return "L25LowerPrice";
		if ( ! L25UpperPrice1.isValid())
			return "L25UpperPrice1";
		if ( ! L25LowerPrice1.isValid())
			return "L25LowerPrice1";
		if ( ! L25UpperPrice2.isValid())
			return "L25UpperPrice2";
		if ( ! L25LowerPrice2.isValid())
			return "L25LowerPrice2";
		if ( ! BidPrice1.isValid())
			return "BidPrice1";
		if ( ! BidVolume1.isValid())
			return "BidVolume1";
		if ( ! AskPrice1.isValid())
			return "AskPrice1";
		if ( ! AskVolume1.isValid())
			return "AskVolume1";
		if ( ! TheoryPrice.isValid())
			return "TheoryPrice";
		if ( ! HighestPrice24.isValid())
			return "HighestPrice24";
		if ( ! LowestPrice24.isValid())
			return "LowestPrice24";
		if ( ! Volume24.isValid())
			return "Volume24";
		if ( ! Turnover24.isValid())
			return "Turnover24";
		if ( ! OpenPrice24.isValid())
			return "OpenPrice24";
		if ( ! Frequency.isValid())
			return "Frequency";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableMarketData *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///产品组
	CShortIndexIDType ProductGroup;
	///更新时间
	CMilliSecondsType UpdateTime;
	///上次收盘
	CPriceType PreClosePrice;
	///上次结算价
	CPriceType PreSettlementPrice;
	///上次持仓量
	CVolumeType PreOpenInterest;
	///上次持仓费按比例
	CRatioType PrePositionFeeRate;
	///收盘价
	CPriceType ClosePrice;
	///收取持仓费用时间
	CMilliSecondsType PositionFeeTime;
	///结算时间
	CMilliSecondsType SettlementTime;
	///最高限价
	CPriceType UpperLimitPrice;
	///最低限价
	CPriceType LowerLimitPrice;
	///基础标的价格
	CPriceType UnderlyingPrice;
	///标记价格
	CPriceType MarkedPrice;
	///预计持仓费按比例
	CRatioType PositionFeeRate;
	///当日最高价
	CPriceType HighestPrice;
	///当日最低价
	CPriceType LowestPrice;
	///最新价
	CPriceType LastPrice;
	///当日数量
	CCollectType Volume;
	///当日成交金额
	CCollectType Turnover;
	///持仓量
	CVolumeType OpenInterest;
	///预估结算价
	CPriceType SettlementPrice;
	///当日开盘价
	CPriceType OpenPrice;
	///行情发布状态
	CBoolType IsPubMarketData;
	///标的交易状态
	CInstrumentStatusType InstrumentStatus;
	///备注
	CLongNameType Remark;
	///业务序列号
	CLargeNumberType BusinessNo;
	///标记价格汇总
	CTotalType MarkedPriceTotal;
	///标记价格权重
	CTotalType MarkedPriceWeight;
	///25档行情最高价
	CPriceType L25UpperPrice;
	///25档行情最低价
	CPriceType L25LowerPrice;
	///25档行情最高价1
	CPriceType L25UpperPrice1;
	///25档行情最低价1
	CPriceType L25LowerPrice1;
	///25档行情最高价2
	CPriceType L25UpperPrice2;
	///25档行情最低价2
	CPriceType L25LowerPrice2;
	///申买价一
	CPriceType BidPrice1;
	///申买量一
	CVolumeType BidVolume1;
	///申卖价一
	CPriceType AskPrice1;
	///申卖量一
	CVolumeType AskVolume1;
	///理论价
	CPriceType TheoryPrice;
	///24小时最高价
	CPriceType HighestPrice24;
	///24小时最低价
	CPriceType LowestPrice24;
	///24小时数量
	CCollectType Volume24;
	///24小时成交金额
	CCollectType Turnover24;
	///24小时开盘价
	CPriceType OpenPrice24;
	///每秒更新的次数
	CNumberType Frequency;
	///插入时间
	CMilliSecondsType InsertTime;
	
	unsigned int HashInstrumentIDIndex;
	
	///计算InstrumentIDIndex的hash值
	void calHashInstrumentIDIndex()
	{
		unsigned key=0;
		key=ExchangeID.hash(key);
		key=InstrumentID.hash(key);
		HashInstrumentIDIndex=key;
	}
	
	///根据需要情况，更新InstrumentIDIndex的hash值
	void updateHashInstrumentIDIndex()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashInstrumentIDIndex();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashInstrumentIDIndex();
	}
	
	///构造对应的标的的指针
	///@param	pFactory	构造对应的标的的指针时，寻找的对象工厂
	///@return	找到的对应的标的的指针
	const CWriteableInstrument* linkInstrument() const;

	///构造对应的标的指针
	///@param	pInstrument	要连接到的对象
	///@return	输入的pInstrument
	const CWriteableInstrument* linkInstrument(const CWriteableInstrument *pInstrument) const
	{
		void *target=(void *)(&(this->pInstrument));
		*((const CWriteableInstrument **)target)=pInstrument;
		return pInstrument;
	}
	
	///获取对应的标的的指针
	///@return	找到的对应的标的的指针
	const CWriteableInstrument* getInstrument() const
	{
		if(pInstrument==NULL)
			return linkInstrument();
		return pInstrument;
	}
	
	CMDB* m_pMDB;
private:
	///对应的标的
	const CWriteableInstrument *pInstrument;
};

typedef const CWriteableMarketData CMarketData;

/////////////////////////////////////////////////////////////////////////
///CMarketDataActionTrigger是操作行情时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketDataActionTrigger
{
public:
	///构造方法
	CMarketDataActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CMarketDataActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CMarketDataFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pMarketData	要加入的MarketData
	virtual void beforeAdd(CWriteableMarketData *pMarketData)
	{
		return;
	}
	
	///加入后触发
	///@param	pMarketData	已经加入的MarketData
	virtual void afterAdd(CMarketData *pMarketData)
	{
		return;
	}

	///更新前触发	
	///@param	pMarketData	被刷新的CMarketData
	///@param	pNewMarketData	新的值
	virtual void beforeUpdate(CMarketData *pMarketData, CWriteableMarketData *pNewMarketData)
	{
		return;
	}
	
	///更新后触发
	///@param	pMarketData	被刷新的CMarketData
	virtual void afterUpdate(CMarketData *pMarketData)
	{
		return;
	}
	
	///删除前触发
	///@param	pMarketData	要删除的CMarketData
	virtual void beforeRemove(CMarketData *pMarketData)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CMarketDataFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CMarketDataCommitTrigger是确认行情时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketDataCommitTrigger
{
public:
	///构造方法
	CMarketDataCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CMarketDataCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CMarketDataFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pMarketData	已经加入的MarketData
	virtual void commitAdd(CMarketData *pMarketData)
	{
		return;
	}

	///更新后触发
	///@param	pMarketData	被刷新的CMarketData
	///@param	poldMarketData	原来的值
	virtual void commitUpdate(CMarketData *pMarketData, CWriteableMarketData *pOldMarketData)
	{
		return;
	}
	
	///删除后触发
	///@param	pMarketData	已经删除的CMarketData
	virtual void commitRemove(CWriteableMarketData *pMarketData)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CMarketDataFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CMarketDataIterator是一个对行情的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketDataIterator
{
public:
	///构造方法
	CMarketDataIterator()
	{
	}

	///构造方法
	CMarketDataIterator(CMarketDataFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CMarketDataIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个MarketData
	///@return	下一个满足条件CMarketData，如果已经没有一个满足要求了，则返回NULL
	virtual CMarketData *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CMarketDataFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pMarketData 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableMarketData * pMarketData,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CMarketDataFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableOrder是一个存储报单的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableOrder
{
public:
	CWriteableOrder()
	{
		pInstrument=NULL;
		pPosition=NULL;
		pMember=NULL;
		pTriggerOrder=NULL;
	}
	
	CWriteableOrder(const CWriteableOrder& c)
	{
		memcpy(this, &c, sizeof(CWriteableOrder));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! TradeUnitID.isValid())
			return "TradeUnitID";
		if ( ! AccountID.isValid())
			return "AccountID";
		if ( ! LocalID.isValid())
			return "LocalID";
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! OrderPriceType.isValid())
			return "OrderPriceType";
		if ( ! Direction.isValid())
			return "Direction";
		if ( ! OffsetFlag.isValid())
			return "OffsetFlag";
		if ( ! Price.isValid())
			return "Price";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! VolumeDisplay.isValid())
			return "VolumeDisplay";
		if ( ! VolumeMode.isValid())
			return "VolumeMode";
		if ( ! Cost.isValid())
			return "Cost";
		if ( ! OrderType.isValid())
			return "OrderType";
		if ( ! GTDTime.isValid())
			return "GTDTime";
		if ( ! MinVolume.isValid())
			return "MinVolume";
		if ( ! BusinessType.isValid())
			return "BusinessType";
		if ( ! BusinessValue.isValid())
			return "BusinessValue";
		if ( ! CloseOrderID.isValid())
			return "CloseOrderID";
		if ( ! IsCrossMargin.isValid())
			return "IsCrossMargin";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! OrderID.isValid())
			return "OrderID";
		if ( ! CopyMemberID.isValid())
			return "CopyMemberID";
		if ( ! CopyOrderID.isValid())
			return "CopyOrderID";
		if ( ! Leverage.isValid())
			return "Leverage";
		if ( ! CopyProfitRate.isValid())
			return "CopyProfitRate";
		if ( ! APPID.isValid())
			return "APPID";
		if ( ! PositionID.isValid())
			return "PositionID";
		if ( ! TriggerPrice.isValid())
			return "TriggerPrice";
		if ( ! Reserve.isValid())
			return "Reserve";
		if ( ! OrderStatus.isValid())
			return "OrderStatus";
		if ( ! DeriveSource.isValid())
			return "DeriveSource";
		if ( ! DeriveDetail.isValid())
			return "DeriveDetail";
		if ( ! VolumeTraded.isValid())
			return "VolumeTraded";
		if ( ! VolumeRemain.isValid())
			return "VolumeRemain";
		if ( ! VolumeCancled.isValid())
			return "VolumeCancled";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		if ( ! Priority.isValid())
			return "Priority";
		if ( ! TimeSortNo.isValid())
			return "TimeSortNo";
		if ( ! FrontNo.isValid())
			return "FrontNo";
		if ( ! PriceCurrency.isValid())
			return "PriceCurrency";
		if ( ! FeeCurrency.isValid())
			return "FeeCurrency";
		if ( ! ClearCurrency.isValid())
			return "ClearCurrency";
		if ( ! FrozenMoney.isValid())
			return "FrozenMoney";
		if ( ! FrozenFee.isValid())
			return "FrozenFee";
		if ( ! FrozenMargin.isValid())
			return "FrozenMargin";
		if ( ! Fee.isValid())
			return "Fee";
		if ( ! CloseProfit.isValid())
			return "CloseProfit";
		if ( ! Turnover.isValid())
			return "Turnover";
		if ( ! RelatedOrderID.isValid())
			return "RelatedOrderID";
		if ( ! BusinessResult.isValid())
			return "BusinessResult";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		if ( ! Tradable.isValid())
			return "Tradable";
		if ( ! SettlementGroup.isValid())
			return "SettlementGroup";
		if ( ! PosiDirection.isValid())
			return "PosiDirection";
		if ( ! TradePrice.isValid())
			return "TradePrice";
		if ( ! OpenPrice.isValid())
			return "OpenPrice";
		if ( ! TriggerOrderID.isValid())
			return "TriggerOrderID";
		if ( ! SLTriggerPrice.isValid())
			return "SLTriggerPrice";
		if ( ! TPTriggerPrice.isValid())
			return "TPTriggerPrice";
		if ( ! CopyProfit.isValid())
			return "CopyProfit";
		if ( ! Position.isValid())
			return "Position";
		if ( ! UserID.isValid())
			return "UserID";
		if ( ! LastPriceByInsert.isValid())
			return "LastPriceByInsert";
		if ( ! BidPrice1ByInsert.isValid())
			return "BidPrice1ByInsert";
		if ( ! AskPrice1ByInsert.isValid())
			return "AskPrice1ByInsert";
		if ( ! Available.isValid())
			return "Available";
		if ( ! CreateTime.isValid())
			return "CreateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableOrder *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///成员代码
	CLongIndexIDType MemberID;
	///交易单元代码
	CIndexIDType TradeUnitID;
	///资金账号
	CAccountIDType AccountID;
	///报单本地标识
	CIndexIDType LocalID;
	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///报单价格类型
	COrderPriceTypeType OrderPriceType;
	///买卖方向
	CDirectionType Direction;
	///开平标志
	COffsetFlagType OffsetFlag;
	///报单价格
	CPriceType Price;
	///数量
	CVolumeType Volume;
	///显示数量
	CVolumeType VolumeDisplay;
	///数量取值方式
	CValueModeType VolumeMode;
	///委托额
	CMoneyType Cost;
	///订单类型
	COrderTypeType OrderType;
	///GTD时间
	CMilliSecondsType GTDTime;
	///最小成交量
	CVolumeType MinVolume;
	///业务类别
	CBusinessTypeType BusinessType;
	///业务值
	CNameType BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType CloseOrderID;
	///是否全仓
	CBoolType IsCrossMargin;
	///备注
	CNameType Remark;
	///报单系统唯一代码
	CIndexIDType OrderID;
	///带单员代码
	CLongIndexIDType CopyMemberID;
	///带单员报单号
	CIndexIDType CopyOrderID;
	///委托单杠杆倍数
	CLeverageType Leverage;
	///带单分配比例
	CRatioType CopyProfitRate;
	///应用编号
	CShortIndexIDType APPID;
	///持仓代码
	CLongIndexIDType PositionID;
	///触发价
	CPriceType TriggerPrice;
	///保留资金
	CMoneyType Reserve;
	///报单状态
	COrderStatusType OrderStatus;
	///衍生来源
	CDeriveSourceType DeriveSource;
	///衍生明细
	CIndexIDType DeriveDetail;
	///成交数量
	CVolumeType VolumeTraded;
	///剩余数量
	CVolumeType VolumeRemain;
	///已经撤单数量
	CVolumeType VolumeCancled;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	///优先权
	CNumberType Priority;
	///按时间排队的序号
	CLargeNumberType TimeSortNo;
	///前置编号
	CNumberType FrontNo;
	///计价币种
	CCurrencyType PriceCurrency;
	///手续费币种
	CCurrencyType FeeCurrency;
	///清算币种
	CCurrencyType ClearCurrency;
	///冻结资金
	CMoneyType FrozenMoney;
	///冻结手续费
	CMoneyType FrozenFee;
	///冻结保证金
	CMoneyType FrozenMargin;
	///手续费
	CMoneyType Fee;
	///平仓盈亏
	CMoneyType CloseProfit;
	///成交金额
	CMoneyType Turnover;
	///关联报单号
	CIndexIDType RelatedOrderID;
	///业务执行结果
	CNameType BusinessResult;
	///业务序列号
	CLargeNumberType BusinessNo;
	///是否可以参加交易
	CBoolType Tradable;
	///结算组编号
	CShortIndexIDType SettlementGroup;
	///持仓多空方向
	CPosiDirectionType PosiDirection;
	///成交均价
	CPriceType TradePrice;
	///平仓成交时的开仓均价
	CPriceType OpenPrice;
	///触发报单号
	CIndexIDType TriggerOrderID;
	///开仓报单成交之后的止损触发价
	CPriceType SLTriggerPrice;
	///开仓报单成交之后的止盈触发价
	CPriceType TPTriggerPrice;
	///带单盈利分配
	CMoneyType CopyProfit;
	///成交时的持仓量
	CVolumeType Position;
	///交易用户代码
	CLongIndexIDType UserID;
	///插入时的最新价
	CPriceType LastPriceByInsert;
	///插入时的买一价
	CPriceType BidPrice1ByInsert;
	///插入时的卖一价
	CPriceType AskPrice1ByInsert;
	///可用资金
	CMoneyType Available;
	///创建时间
	CIndexIDType CreateTime;
	
	unsigned int HashOrderID;
	
	///计算OrderID的hash值
	void calHashOrderID()
	{
		unsigned key=0;
		key=OrderID.hash(key);
		HashOrderID=key;
	}
	
	///根据需要情况，更新OrderID的hash值
	void updateHashOrderID()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashOrderID();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashOrderID();
	}
	
	///构造对应的标的的指针
	///@param	pFactory	构造对应的标的的指针时，寻找的对象工厂
	///@return	找到的对应的标的的指针
	const CWriteableInstrument* linkInstrument() const;

	///构造对应的标的指针
	///@param	pInstrument	要连接到的对象
	///@return	输入的pInstrument
	const CWriteableInstrument* linkInstrument(const CWriteableInstrument *pInstrument) const
	{
		void *target=(void *)(&(this->pInstrument));
		*((const CWriteableInstrument **)target)=pInstrument;
		return pInstrument;
	}
	
	///获取对应的标的的指针
	///@return	找到的对应的标的的指针
	const CWriteableInstrument* getInstrument() const
	{
		if(pInstrument==NULL)
			return linkInstrument();
		return pInstrument;
	}
	
	///构造对应的交易单元持仓的指针
	///@param	pFactory	构造对应的交易单元持仓的指针时，寻找的对象工厂
	///@return	找到的对应的交易单元持仓的指针
	const CWriteablePosition* linkPosition() const;

	///构造对应的交易单元持仓指针
	///@param	pPosition	要连接到的对象
	///@return	输入的pPosition
	const CWriteablePosition* linkPosition(const CWriteablePosition *pPosition) const
	{
		void *target=(void *)(&(this->pPosition));
		*((const CWriteablePosition **)target)=pPosition;
		return pPosition;
	}
	
	///获取对应的交易单元持仓的指针
	///@return	找到的对应的交易单元持仓的指针
	const CWriteablePosition* getPosition() const
	{
		if(pPosition==NULL)
			return linkPosition();
		return pPosition;
	}
	
	///构造对应的用户的指针
	///@param	pFactory	构造对应的用户的指针时，寻找的对象工厂
	///@return	找到的对应的用户的指针
	const CWriteableMember* linkMember() const;

	///构造对应的用户指针
	///@param	pMember	要连接到的对象
	///@return	输入的pMember
	const CWriteableMember* linkMember(const CWriteableMember *pMember) const
	{
		void *target=(void *)(&(this->pMember));
		*((const CWriteableMember **)target)=pMember;
		return pMember;
	}
	
	///获取对应的用户的指针
	///@return	找到的对应的用户的指针
	const CWriteableMember* getMember() const
	{
		if(pMember==NULL)
			return linkMember();
		return pMember;
	}
	
	///构造关联的触发报单的指针
	///@param	pFactory	构造关联的触发报单的指针时，寻找的对象工厂
	///@return	找到的关联的触发报单的指针
	const CWriteableTriggerOrder* linkTriggerOrder() const;

	///构造关联的触发报单指针
	///@param	pTriggerOrder	要连接到的对象
	///@return	输入的pTriggerOrder
	const CWriteableTriggerOrder* linkTriggerOrder(const CWriteableTriggerOrder *pTriggerOrder) const
	{
		void *target=(void *)(&(this->pTriggerOrder));
		*((const CWriteableTriggerOrder **)target)=pTriggerOrder;
		return pTriggerOrder;
	}
	
	///获取关联的触发报单的指针
	///@return	找到的关联的触发报单的指针
	const CWriteableTriggerOrder* getTriggerOrder() const
	{
		if(pTriggerOrder==NULL)
			return linkTriggerOrder();
		return pTriggerOrder;
	}
	
	CMDB* m_pMDB;
private:
	///对应的标的
	const CWriteableInstrument *pInstrument;
	///对应的交易单元持仓
	const CWriteablePosition *pPosition;
	///对应的用户
	const CWriteableMember *pMember;
	///关联的触发报单
	const CWriteableTriggerOrder *pTriggerOrder;
};

typedef const CWriteableOrder COrder;

/////////////////////////////////////////////////////////////////////////
///COrderActionTrigger是操作报单时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class COrderActionTrigger
{
public:
	///构造方法
	COrderActionTrigger()
	{
	}
	
	///析构方法
	virtual ~COrderActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(COrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pOrder	要加入的Order
	virtual void beforeAdd(CWriteableOrder *pOrder)
	{
		return;
	}
	
	///加入后触发
	///@param	pOrder	已经加入的Order
	virtual void afterAdd(COrder *pOrder)
	{
		return;
	}

	///更新前触发	
	///@param	pOrder	被刷新的COrder
	///@param	pNewOrder	新的值
	virtual void beforeUpdate(COrder *pOrder, CWriteableOrder *pNewOrder)
	{
		return;
	}
	
	///更新后触发
	///@param	pOrder	被刷新的COrder
	virtual void afterUpdate(COrder *pOrder)
	{
		return;
	}
	
	///删除前触发
	///@param	pOrder	要删除的COrder
	virtual void beforeRemove(COrder *pOrder)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	COrderFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///COrderCommitTrigger是确认报单时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class COrderCommitTrigger
{
public:
	///构造方法
	COrderCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~COrderCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(COrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pOrder	已经加入的Order
	virtual void commitAdd(COrder *pOrder)
	{
		return;
	}

	///更新后触发
	///@param	pOrder	被刷新的COrder
	///@param	poldOrder	原来的值
	virtual void commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder)
	{
		return;
	}
	
	///删除后触发
	///@param	pOrder	已经删除的COrder
	virtual void commitRemove(CWriteableOrder *pOrder)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	COrderFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///COrderIterator是一个对报单的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class COrderIterator
{
public:
	///构造方法
	COrderIterator()
	{
	}

	///构造方法
	COrderIterator(COrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~COrderIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个Order
	///@return	下一个满足条件COrder，如果已经没有一个满足要求了，则返回NULL
	virtual COrder *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	COrderFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pOrder 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableOrder * pOrder,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	COrderFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteablePositionOrder是一个存储未平仓报单的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteablePositionOrder
{
public:
	CWriteablePositionOrder()
	{
	}
	
	CWriteablePositionOrder(const CWriteablePositionOrder& c)
	{
		memcpy(this, &c, sizeof(CWriteablePositionOrder));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! TradeUnitID.isValid())
			return "TradeUnitID";
		if ( ! AccountID.isValid())
			return "AccountID";
		if ( ! LocalID.isValid())
			return "LocalID";
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! OrderPriceType.isValid())
			return "OrderPriceType";
		if ( ! Direction.isValid())
			return "Direction";
		if ( ! OffsetFlag.isValid())
			return "OffsetFlag";
		if ( ! Price.isValid())
			return "Price";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! VolumeDisplay.isValid())
			return "VolumeDisplay";
		if ( ! VolumeMode.isValid())
			return "VolumeMode";
		if ( ! Cost.isValid())
			return "Cost";
		if ( ! OrderType.isValid())
			return "OrderType";
		if ( ! GTDTime.isValid())
			return "GTDTime";
		if ( ! MinVolume.isValid())
			return "MinVolume";
		if ( ! BusinessType.isValid())
			return "BusinessType";
		if ( ! BusinessValue.isValid())
			return "BusinessValue";
		if ( ! CloseOrderID.isValid())
			return "CloseOrderID";
		if ( ! IsCrossMargin.isValid())
			return "IsCrossMargin";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! OrderID.isValid())
			return "OrderID";
		if ( ! CopyMemberID.isValid())
			return "CopyMemberID";
		if ( ! CopyOrderID.isValid())
			return "CopyOrderID";
		if ( ! Leverage.isValid())
			return "Leverage";
		if ( ! CopyProfitRate.isValid())
			return "CopyProfitRate";
		if ( ! APPID.isValid())
			return "APPID";
		if ( ! PositionID.isValid())
			return "PositionID";
		if ( ! TriggerPrice.isValid())
			return "TriggerPrice";
		if ( ! Reserve.isValid())
			return "Reserve";
		if ( ! OrderStatus.isValid())
			return "OrderStatus";
		if ( ! DeriveSource.isValid())
			return "DeriveSource";
		if ( ! DeriveDetail.isValid())
			return "DeriveDetail";
		if ( ! VolumeTraded.isValid())
			return "VolumeTraded";
		if ( ! VolumeRemain.isValid())
			return "VolumeRemain";
		if ( ! VolumeCancled.isValid())
			return "VolumeCancled";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		if ( ! Priority.isValid())
			return "Priority";
		if ( ! TimeSortNo.isValid())
			return "TimeSortNo";
		if ( ! FrontNo.isValid())
			return "FrontNo";
		if ( ! PriceCurrency.isValid())
			return "PriceCurrency";
		if ( ! FeeCurrency.isValid())
			return "FeeCurrency";
		if ( ! ClearCurrency.isValid())
			return "ClearCurrency";
		if ( ! FrozenMoney.isValid())
			return "FrozenMoney";
		if ( ! FrozenFee.isValid())
			return "FrozenFee";
		if ( ! FrozenMargin.isValid())
			return "FrozenMargin";
		if ( ! Fee.isValid())
			return "Fee";
		if ( ! CloseProfit.isValid())
			return "CloseProfit";
		if ( ! Turnover.isValid())
			return "Turnover";
		if ( ! RelatedOrderID.isValid())
			return "RelatedOrderID";
		if ( ! BusinessResult.isValid())
			return "BusinessResult";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		if ( ! Tradable.isValid())
			return "Tradable";
		if ( ! SettlementGroup.isValid())
			return "SettlementGroup";
		if ( ! PosiDirection.isValid())
			return "PosiDirection";
		if ( ! TradePrice.isValid())
			return "TradePrice";
		if ( ! OpenPrice.isValid())
			return "OpenPrice";
		if ( ! TriggerOrderID.isValid())
			return "TriggerOrderID";
		if ( ! SLTriggerPrice.isValid())
			return "SLTriggerPrice";
		if ( ! TPTriggerPrice.isValid())
			return "TPTriggerPrice";
		if ( ! CopyProfit.isValid())
			return "CopyProfit";
		if ( ! Position.isValid())
			return "Position";
		if ( ! UserID.isValid())
			return "UserID";
		if ( ! LastPriceByInsert.isValid())
			return "LastPriceByInsert";
		if ( ! BidPrice1ByInsert.isValid())
			return "BidPrice1ByInsert";
		if ( ! AskPrice1ByInsert.isValid())
			return "AskPrice1ByInsert";
		if ( ! Available.isValid())
			return "Available";
		if ( ! CreateTime.isValid())
			return "CreateTime";
		if ( ! VolumeToClose.isValid())
			return "VolumeToClose";
		if ( ! VolumeClosed.isValid())
			return "VolumeClosed";
		if ( ! VolumeOnClose.isValid())
			return "VolumeOnClose";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteablePositionOrder *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///成员代码
	CLongIndexIDType MemberID;
	///交易单元代码
	CIndexIDType TradeUnitID;
	///资金账号
	CAccountIDType AccountID;
	///报单本地标识
	CIndexIDType LocalID;
	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///报单价格类型
	COrderPriceTypeType OrderPriceType;
	///买卖方向
	CDirectionType Direction;
	///开平标志
	COffsetFlagType OffsetFlag;
	///报单价格
	CPriceType Price;
	///数量
	CVolumeType Volume;
	///显示数量
	CVolumeType VolumeDisplay;
	///数量取值方式
	CValueModeType VolumeMode;
	///委托额
	CMoneyType Cost;
	///订单类型
	COrderTypeType OrderType;
	///GTD时间
	CMilliSecondsType GTDTime;
	///最小成交量
	CVolumeType MinVolume;
	///业务类别
	CBusinessTypeType BusinessType;
	///业务值
	CNameType BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType CloseOrderID;
	///是否全仓
	CBoolType IsCrossMargin;
	///备注
	CNameType Remark;
	///报单系统唯一代码
	CIndexIDType OrderID;
	///带单员代码
	CLongIndexIDType CopyMemberID;
	///带单员报单号
	CIndexIDType CopyOrderID;
	///委托单杠杆倍数
	CLeverageType Leverage;
	///带单分配比例
	CRatioType CopyProfitRate;
	///应用编号
	CShortIndexIDType APPID;
	///持仓代码
	CLongIndexIDType PositionID;
	///触发价
	CPriceType TriggerPrice;
	///保留资金
	CMoneyType Reserve;
	///报单状态
	COrderStatusType OrderStatus;
	///衍生来源
	CDeriveSourceType DeriveSource;
	///衍生明细
	CIndexIDType DeriveDetail;
	///成交数量
	CVolumeType VolumeTraded;
	///剩余数量
	CVolumeType VolumeRemain;
	///已经撤单数量
	CVolumeType VolumeCancled;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	///优先权
	CNumberType Priority;
	///按时间排队的序号
	CLargeNumberType TimeSortNo;
	///前置编号
	CNumberType FrontNo;
	///计价币种
	CCurrencyType PriceCurrency;
	///手续费币种
	CCurrencyType FeeCurrency;
	///清算币种
	CCurrencyType ClearCurrency;
	///冻结资金
	CMoneyType FrozenMoney;
	///冻结手续费
	CMoneyType FrozenFee;
	///冻结保证金
	CMoneyType FrozenMargin;
	///手续费
	CMoneyType Fee;
	///平仓盈亏
	CMoneyType CloseProfit;
	///成交金额
	CMoneyType Turnover;
	///关联报单号
	CIndexIDType RelatedOrderID;
	///业务执行结果
	CNameType BusinessResult;
	///业务序列号
	CLargeNumberType BusinessNo;
	///是否可以参加交易
	CBoolType Tradable;
	///结算组编号
	CShortIndexIDType SettlementGroup;
	///持仓多空方向
	CPosiDirectionType PosiDirection;
	///成交均价
	CPriceType TradePrice;
	///平仓成交时的开仓均价
	CPriceType OpenPrice;
	///触发报单号
	CIndexIDType TriggerOrderID;
	///开仓报单成交之后的止损触发价
	CPriceType SLTriggerPrice;
	///开仓报单成交之后的止盈触发价
	CPriceType TPTriggerPrice;
	///带单盈利分配
	CMoneyType CopyProfit;
	///成交时的持仓量
	CVolumeType Position;
	///交易用户代码
	CLongIndexIDType UserID;
	///插入时的最新价
	CPriceType LastPriceByInsert;
	///插入时的买一价
	CPriceType BidPrice1ByInsert;
	///插入时的卖一价
	CPriceType AskPrice1ByInsert;
	///可用资金
	CMoneyType Available;
	///创建时间
	CIndexIDType CreateTime;
	///需要平仓的数量
	CVolumeType VolumeToClose;
	///已经平仓的数量
	CVolumeType VolumeClosed;
	///正在平仓未成交的数量
	CVolumeType VolumeOnClose;
	
	unsigned int HashOrderID;
	
	///计算OrderID的hash值
	void calHashOrderID()
	{
		unsigned key=0;
		key=OrderID.hash(key);
		HashOrderID=key;
	}
	
	///根据需要情况，更新OrderID的hash值
	void updateHashOrderID()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashOrderID();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashOrderID();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteablePositionOrder CPositionOrder;

/////////////////////////////////////////////////////////////////////////
///CPositionOrderActionTrigger是操作未平仓报单时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionOrderActionTrigger
{
public:
	///构造方法
	CPositionOrderActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CPositionOrderActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CPositionOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pPositionOrder	要加入的PositionOrder
	virtual void beforeAdd(CWriteablePositionOrder *pPositionOrder)
	{
		return;
	}
	
	///加入后触发
	///@param	pPositionOrder	已经加入的PositionOrder
	virtual void afterAdd(CPositionOrder *pPositionOrder)
	{
		return;
	}

	///更新前触发	
	///@param	pPositionOrder	被刷新的CPositionOrder
	///@param	pNewPositionOrder	新的值
	virtual void beforeUpdate(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pNewPositionOrder)
	{
		return;
	}
	
	///更新后触发
	///@param	pPositionOrder	被刷新的CPositionOrder
	virtual void afterUpdate(CPositionOrder *pPositionOrder)
	{
		return;
	}
	
	///删除前触发
	///@param	pPositionOrder	要删除的CPositionOrder
	virtual void beforeRemove(CPositionOrder *pPositionOrder)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CPositionOrderFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CPositionOrderCommitTrigger是确认未平仓报单时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionOrderCommitTrigger
{
public:
	///构造方法
	CPositionOrderCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CPositionOrderCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CPositionOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pPositionOrder	已经加入的PositionOrder
	virtual void commitAdd(CPositionOrder *pPositionOrder)
	{
		return;
	}

	///更新后触发
	///@param	pPositionOrder	被刷新的CPositionOrder
	///@param	poldPositionOrder	原来的值
	virtual void commitUpdate(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pOldPositionOrder)
	{
		return;
	}
	
	///删除后触发
	///@param	pPositionOrder	已经删除的CPositionOrder
	virtual void commitRemove(CWriteablePositionOrder *pPositionOrder)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CPositionOrderFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CPositionOrderIterator是一个对未平仓报单的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionOrderIterator
{
public:
	///构造方法
	CPositionOrderIterator()
	{
	}

	///构造方法
	CPositionOrderIterator(CPositionOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CPositionOrderIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个PositionOrder
	///@return	下一个满足条件CPositionOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CPositionOrder *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CPositionOrderFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pPositionOrder 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteablePositionOrder * pPositionOrder,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CPositionOrderFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableTriggerOrder是一个存储报单的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableTriggerOrder
{
public:
	CWriteableTriggerOrder()
	{
		pOrder=NULL;
		pMember=NULL;
		pInstrument=NULL;
		pMarketData=NULL;
	}
	
	CWriteableTriggerOrder(const CWriteableTriggerOrder& c)
	{
		memcpy(this, &c, sizeof(CWriteableTriggerOrder));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! TradeUnitID.isValid())
			return "TradeUnitID";
		if ( ! AccountID.isValid())
			return "AccountID";
		if ( ! LocalID.isValid())
			return "LocalID";
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! OrderPriceType.isValid())
			return "OrderPriceType";
		if ( ! Direction.isValid())
			return "Direction";
		if ( ! OffsetFlag.isValid())
			return "OffsetFlag";
		if ( ! Price.isValid())
			return "Price";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! VolumeDisplay.isValid())
			return "VolumeDisplay";
		if ( ! VolumeMode.isValid())
			return "VolumeMode";
		if ( ! Cost.isValid())
			return "Cost";
		if ( ! OrderType.isValid())
			return "OrderType";
		if ( ! GTDTime.isValid())
			return "GTDTime";
		if ( ! MinVolume.isValid())
			return "MinVolume";
		if ( ! BusinessType.isValid())
			return "BusinessType";
		if ( ! BusinessValue.isValid())
			return "BusinessValue";
		if ( ! CloseOrderID.isValid())
			return "CloseOrderID";
		if ( ! IsCrossMargin.isValid())
			return "IsCrossMargin";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! OrderID.isValid())
			return "OrderID";
		if ( ! CopyMemberID.isValid())
			return "CopyMemberID";
		if ( ! CopyOrderID.isValid())
			return "CopyOrderID";
		if ( ! Leverage.isValid())
			return "Leverage";
		if ( ! CopyProfitRate.isValid())
			return "CopyProfitRate";
		if ( ! APPID.isValid())
			return "APPID";
		if ( ! PositionID.isValid())
			return "PositionID";
		if ( ! TriggerPrice.isValid())
			return "TriggerPrice";
		if ( ! Reserve.isValid())
			return "Reserve";
		if ( ! SLPrice.isValid())
			return "SLPrice";
		if ( ! SLTriggerPrice.isValid())
			return "SLTriggerPrice";
		if ( ! TPPrice.isValid())
			return "TPPrice";
		if ( ! TPTriggerPrice.isValid())
			return "TPTriggerPrice";
		if ( ! RiskBefore.isValid())
			return "RiskBefore";
		if ( ! TriggerOrderType.isValid())
			return "TriggerOrderType";
		if ( ! TriggerDetail.isValid())
			return "TriggerDetail";
		if ( ! TriggerPriceType.isValid())
			return "TriggerPriceType";
		if ( ! TriggerValue.isValid())
			return "TriggerValue";
		if ( ! CloseSLPrice.isValid())
			return "CloseSLPrice";
		if ( ! CloseSLTriggerPrice.isValid())
			return "CloseSLTriggerPrice";
		if ( ! CloseTPPrice.isValid())
			return "CloseTPPrice";
		if ( ! CloseTPTriggerPrice.isValid())
			return "CloseTPTriggerPrice";
		if ( ! CloseOrderPriceType.isValid())
			return "CloseOrderPriceType";
		if ( ! ClosePrice.isValid())
			return "ClosePrice";
		if ( ! CloseTriggerPrice.isValid())
			return "CloseTriggerPrice";
		if ( ! RelatedOrderID.isValid())
			return "RelatedOrderID";
		if ( ! ActiveTime.isValid())
			return "ActiveTime";
		if ( ! TriggerTime.isValid())
			return "TriggerTime";
		if ( ! TimeSortNo.isValid())
			return "TimeSortNo";
		if ( ! TriggerStatus.isValid())
			return "TriggerStatus";
		if ( ! PosiDirection.isValid())
			return "PosiDirection";
		if ( ! FrontNo.isValid())
			return "FrontNo";
		if ( ! ErrorNo.isValid())
			return "ErrorNo";
		if ( ! ErrorMsg.isValid())
			return "ErrorMsg";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		if ( ! CreateTime.isValid())
			return "CreateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableTriggerOrder *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///成员代码
	CLongIndexIDType MemberID;
	///交易单元代码
	CIndexIDType TradeUnitID;
	///资金账号
	CAccountIDType AccountID;
	///报单本地标识
	CIndexIDType LocalID;
	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///报单价格类型
	COrderPriceTypeType OrderPriceType;
	///买卖方向
	CDirectionType Direction;
	///开平标志
	COffsetFlagType OffsetFlag;
	///报单价格
	CPriceType Price;
	///数量
	CVolumeType Volume;
	///显示数量
	CVolumeType VolumeDisplay;
	///数量取值方式
	CValueModeType VolumeMode;
	///委托额
	CMoneyType Cost;
	///订单类型
	COrderTypeType OrderType;
	///GTD时间
	CMilliSecondsType GTDTime;
	///最小成交量
	CVolumeType MinVolume;
	///业务类别
	CBusinessTypeType BusinessType;
	///业务值
	CNameType BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType CloseOrderID;
	///是否全仓
	CBoolType IsCrossMargin;
	///备注
	CNameType Remark;
	///报单系统唯一代码
	CIndexIDType OrderID;
	///带单员代码
	CLongIndexIDType CopyMemberID;
	///带单员报单号
	CIndexIDType CopyOrderID;
	///委托单杠杆倍数
	CLeverageType Leverage;
	///带单分配比例
	CRatioType CopyProfitRate;
	///应用编号
	CShortIndexIDType APPID;
	///持仓代码
	CLongIndexIDType PositionID;
	///触发价
	CPriceType TriggerPrice;
	///保留资金
	CMoneyType Reserve;
	///止损价
	CPriceType SLPrice;
	///止损触发价
	CPriceType SLTriggerPrice;
	///止盈价
	CPriceType TPPrice;
	///止盈触发价
	CPriceType TPTriggerPrice;
	///是否提前检查风控
	CBoolType RiskBefore;
	///触发的订单类型
	CTriggerOrderTypeType TriggerOrderType;
	///触发类型明细
	CIndexIDType TriggerDetail;
	///触发价类型
	CTriggerPriceTypeType TriggerPriceType;
	///触发单具体设置信息
	CLongContentType TriggerValue;
	///平仓止损价
	CPriceType CloseSLPrice;
	///平仓止损触发价
	CPriceType CloseSLTriggerPrice;
	///平仓止盈价
	CPriceType CloseTPPrice;
	///平仓止盈触发价
	CPriceType CloseTPTriggerPrice;
	///报单价格类型
	COrderPriceTypeType CloseOrderPriceType;
	///平仓价
	CPriceType ClosePrice;
	///平仓触发价
	CPriceType CloseTriggerPrice;
	///关联报单号
	CIndexIDType RelatedOrderID;
	///激活时间
	CMilliSecondsType ActiveTime;
	///触发时间
	CMilliSecondsType TriggerTime;
	///按时间排队的序号
	CLargeNumberType TimeSortNo;
	///触发报单状态
	CTriggerStatusType TriggerStatus;
	///持仓多空方向
	CPosiDirectionType PosiDirection;
	///前置编号
	CNumberType FrontNo;
	///错误代码
	CNumberType ErrorNo;
	///错误信息
	CShortContentType ErrorMsg;
	///插入时间
	CMilliSecondsType InsertTime;
	///更新时间
	CMilliSecondsType UpdateTime;
	///业务序列号
	CLargeNumberType BusinessNo;
	///创建时间
	CIndexIDType CreateTime;
	
	unsigned int HashOrderID;
	
	///计算OrderID的hash值
	void calHashOrderID()
	{
		unsigned key=0;
		key=OrderID.hash(key);
		HashOrderID=key;
	}
	
	///根据需要情况，更新OrderID的hash值
	void updateHashOrderID()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashOrderID();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashOrderID();
	}
	
	///构造对应的报单的指针
	///@param	pFactory	构造对应的报单的指针时，寻找的对象工厂
	///@return	找到的对应的报单的指针
	const CWriteableOrder* linkOrder() const;

	///构造对应的报单指针
	///@param	pOrder	要连接到的对象
	///@return	输入的pOrder
	const CWriteableOrder* linkOrder(const CWriteableOrder *pOrder) const
	{
		void *target=(void *)(&(this->pOrder));
		*((const CWriteableOrder **)target)=pOrder;
		return pOrder;
	}
	
	///获取对应的报单的指针
	///@return	找到的对应的报单的指针
	const CWriteableOrder* getOrder() const
	{
		if(pOrder==NULL)
			return linkOrder();
		return pOrder;
	}
	
	///构造对应的用户的指针
	///@param	pFactory	构造对应的用户的指针时，寻找的对象工厂
	///@return	找到的对应的用户的指针
	const CWriteableMember* linkMember() const;

	///构造对应的用户指针
	///@param	pMember	要连接到的对象
	///@return	输入的pMember
	const CWriteableMember* linkMember(const CWriteableMember *pMember) const
	{
		void *target=(void *)(&(this->pMember));
		*((const CWriteableMember **)target)=pMember;
		return pMember;
	}
	
	///获取对应的用户的指针
	///@return	找到的对应的用户的指针
	const CWriteableMember* getMember() const
	{
		if(pMember==NULL)
			return linkMember();
		return pMember;
	}
	
	///构造对应的标的的指针
	///@param	pFactory	构造对应的标的的指针时，寻找的对象工厂
	///@return	找到的对应的标的的指针
	const CWriteableInstrument* linkInstrument() const;

	///构造对应的标的指针
	///@param	pInstrument	要连接到的对象
	///@return	输入的pInstrument
	const CWriteableInstrument* linkInstrument(const CWriteableInstrument *pInstrument) const
	{
		void *target=(void *)(&(this->pInstrument));
		*((const CWriteableInstrument **)target)=pInstrument;
		return pInstrument;
	}
	
	///获取对应的标的的指针
	///@return	找到的对应的标的的指针
	const CWriteableInstrument* getInstrument() const
	{
		if(pInstrument==NULL)
			return linkInstrument();
		return pInstrument;
	}
	
	///构造对应的行情的指针
	///@param	pFactory	构造对应的行情的指针时，寻找的对象工厂
	///@return	找到的对应的行情的指针
	const CWriteableMarketData* linkMarketData() const;

	///构造对应的行情指针
	///@param	pMarketData	要连接到的对象
	///@return	输入的pMarketData
	const CWriteableMarketData* linkMarketData(const CWriteableMarketData *pMarketData) const
	{
		void *target=(void *)(&(this->pMarketData));
		*((const CWriteableMarketData **)target)=pMarketData;
		return pMarketData;
	}
	
	///获取对应的行情的指针
	///@return	找到的对应的行情的指针
	const CWriteableMarketData* getMarketData() const
	{
		if(pMarketData==NULL)
			return linkMarketData();
		return pMarketData;
	}
	
	CMDB* m_pMDB;
private:
	///对应的报单
	const CWriteableOrder *pOrder;
	///对应的用户
	const CWriteableMember *pMember;
	///对应的标的
	const CWriteableInstrument *pInstrument;
	///对应的行情
	const CWriteableMarketData *pMarketData;
};

typedef const CWriteableTriggerOrder CTriggerOrder;

/////////////////////////////////////////////////////////////////////////
///CTriggerOrderActionTrigger是操作报单时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderActionTrigger
{
public:
	///构造方法
	CTriggerOrderActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CTriggerOrderActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CTriggerOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pTriggerOrder	要加入的TriggerOrder
	virtual void beforeAdd(CWriteableTriggerOrder *pTriggerOrder)
	{
		return;
	}
	
	///加入后触发
	///@param	pTriggerOrder	已经加入的TriggerOrder
	virtual void afterAdd(CTriggerOrder *pTriggerOrder)
	{
		return;
	}

	///更新前触发	
	///@param	pTriggerOrder	被刷新的CTriggerOrder
	///@param	pNewTriggerOrder	新的值
	virtual void beforeUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pNewTriggerOrder)
	{
		return;
	}
	
	///更新后触发
	///@param	pTriggerOrder	被刷新的CTriggerOrder
	virtual void afterUpdate(CTriggerOrder *pTriggerOrder)
	{
		return;
	}
	
	///删除前触发
	///@param	pTriggerOrder	要删除的CTriggerOrder
	virtual void beforeRemove(CTriggerOrder *pTriggerOrder)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CTriggerOrderFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CTriggerOrderCommitTrigger是确认报单时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderCommitTrigger
{
public:
	///构造方法
	CTriggerOrderCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CTriggerOrderCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CTriggerOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pTriggerOrder	已经加入的TriggerOrder
	virtual void commitAdd(CTriggerOrder *pTriggerOrder)
	{
		return;
	}

	///更新后触发
	///@param	pTriggerOrder	被刷新的CTriggerOrder
	///@param	poldTriggerOrder	原来的值
	virtual void commitUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pOldTriggerOrder)
	{
		return;
	}
	
	///删除后触发
	///@param	pTriggerOrder	已经删除的CTriggerOrder
	virtual void commitRemove(CWriteableTriggerOrder *pTriggerOrder)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CTriggerOrderFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CTriggerOrderIterator是一个对报单的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderIterator
{
public:
	///构造方法
	CTriggerOrderIterator()
	{
	}

	///构造方法
	CTriggerOrderIterator(CTriggerOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CTriggerOrderIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个TriggerOrder
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CTriggerOrder *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CTriggerOrderFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pTriggerOrder 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTriggerOrder * pTriggerOrder,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CTriggerOrderFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableMarketOrder是一个存储分价表的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableMarketOrder
{
public:
	CWriteableMarketOrder()
	{
	}
	
	CWriteableMarketOrder(const CWriteableMarketOrder& c)
	{
		memcpy(this, &c, sizeof(CWriteableMarketOrder));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! Direction.isValid())
			return "Direction";
		if ( ! Price.isValid())
			return "Price";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! Orders.isValid())
			return "Orders";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		if ( ! TrueVolume.isValid())
			return "TrueVolume";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableMarketOrder *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///买卖方向
	CDirectionType Direction;
	///价格
	CPriceType Price;
	///数量
	CVolumeType Volume;
	///订单数量
	CNumberType Orders;
	///最后变化序列号
	CLargeNumberType BusinessNo;
	///真实报单数量
	CVolumeType TrueVolume;
	
	unsigned int HashPriceHashKey;
	
	///计算PriceHashKey的hash值
	void calHashPriceHashKey()
	{
		unsigned key=0;
		key=ExchangeID.hash(key);
		key=InstrumentID.hash(key);
		key=Price.hash(key);
		HashPriceHashKey=key;
	}
	
	///根据需要情况，更新PriceHashKey的hash值
	void updateHashPriceHashKey()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashPriceHashKey();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashPriceHashKey();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableMarketOrder CMarketOrder;

/////////////////////////////////////////////////////////////////////////
///CMarketOrderActionTrigger是操作分价表时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketOrderActionTrigger
{
public:
	///构造方法
	CMarketOrderActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CMarketOrderActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CMarketOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pMarketOrder	要加入的MarketOrder
	virtual void beforeAdd(CWriteableMarketOrder *pMarketOrder)
	{
		return;
	}
	
	///加入后触发
	///@param	pMarketOrder	已经加入的MarketOrder
	virtual void afterAdd(CMarketOrder *pMarketOrder)
	{
		return;
	}

	///更新前触发	
	///@param	pMarketOrder	被刷新的CMarketOrder
	///@param	pNewMarketOrder	新的值
	virtual void beforeUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pNewMarketOrder)
	{
		return;
	}
	
	///更新后触发
	///@param	pMarketOrder	被刷新的CMarketOrder
	virtual void afterUpdate(CMarketOrder *pMarketOrder)
	{
		return;
	}
	
	///删除前触发
	///@param	pMarketOrder	要删除的CMarketOrder
	virtual void beforeRemove(CMarketOrder *pMarketOrder)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CMarketOrderFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CMarketOrderCommitTrigger是确认分价表时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketOrderCommitTrigger
{
public:
	///构造方法
	CMarketOrderCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CMarketOrderCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CMarketOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pMarketOrder	已经加入的MarketOrder
	virtual void commitAdd(CMarketOrder *pMarketOrder)
	{
		return;
	}

	///更新后触发
	///@param	pMarketOrder	被刷新的CMarketOrder
	///@param	poldMarketOrder	原来的值
	virtual void commitUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pOldMarketOrder)
	{
		return;
	}
	
	///删除后触发
	///@param	pMarketOrder	已经删除的CMarketOrder
	virtual void commitRemove(CWriteableMarketOrder *pMarketOrder)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CMarketOrderFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CMarketOrderIterator是一个对分价表的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketOrderIterator
{
public:
	///构造方法
	CMarketOrderIterator()
	{
	}

	///构造方法
	CMarketOrderIterator(CMarketOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CMarketOrderIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个MarketOrder
	///@return	下一个满足条件CMarketOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CMarketOrder *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CMarketOrderFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pMarketOrder 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableMarketOrder * pMarketOrder,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CMarketOrderFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableTickMarketOrder是一个存储Tick聚集分价表的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableTickMarketOrder
{
public:
	CWriteableTickMarketOrder()
	{
	}
	
	CWriteableTickMarketOrder(const CWriteableTickMarketOrder& c)
	{
		memcpy(this, &c, sizeof(CWriteableTickMarketOrder));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! Direction.isValid())
			return "Direction";
		if ( ! Price.isValid())
			return "Price";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! Orders.isValid())
			return "Orders";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		if ( ! TrueVolume.isValid())
			return "TrueVolume";
		if ( ! Tick.isValid())
			return "Tick";
		if ( ! SubIndex.isValid())
			return "SubIndex";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableTickMarketOrder *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///买卖方向
	CDirectionType Direction;
	///价格
	CPriceType Price;
	///数量
	CVolumeType Volume;
	///订单数量
	CNumberType Orders;
	///最后变化序列号
	CLargeNumberType BusinessNo;
	///真实报单数量
	CVolumeType TrueVolume;
	///最小变动价位
	CPriceType Tick;
	///索引
	CShortContentType SubIndex;
	
	unsigned int HashPriceHashKey;
	
	///计算PriceHashKey的hash值
	void calHashPriceHashKey()
	{
		unsigned key=0;
		key=ExchangeID.hash(key);
		key=InstrumentID.hash(key);
		key=Tick.hash(key);
		key=Price.hash(key);
		HashPriceHashKey=key;
	}
	
	///根据需要情况，更新PriceHashKey的hash值
	void updateHashPriceHashKey()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashPriceHashKey();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashPriceHashKey();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableTickMarketOrder CTickMarketOrder;

/////////////////////////////////////////////////////////////////////////
///CTickMarketOrderActionTrigger是操作Tick聚集分价表时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTickMarketOrderActionTrigger
{
public:
	///构造方法
	CTickMarketOrderActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CTickMarketOrderActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CTickMarketOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pTickMarketOrder	要加入的TickMarketOrder
	virtual void beforeAdd(CWriteableTickMarketOrder *pTickMarketOrder)
	{
		return;
	}
	
	///加入后触发
	///@param	pTickMarketOrder	已经加入的TickMarketOrder
	virtual void afterAdd(CTickMarketOrder *pTickMarketOrder)
	{
		return;
	}

	///更新前触发	
	///@param	pTickMarketOrder	被刷新的CTickMarketOrder
	///@param	pNewTickMarketOrder	新的值
	virtual void beforeUpdate(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pNewTickMarketOrder)
	{
		return;
	}
	
	///更新后触发
	///@param	pTickMarketOrder	被刷新的CTickMarketOrder
	virtual void afterUpdate(CTickMarketOrder *pTickMarketOrder)
	{
		return;
	}
	
	///删除前触发
	///@param	pTickMarketOrder	要删除的CTickMarketOrder
	virtual void beforeRemove(CTickMarketOrder *pTickMarketOrder)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CTickMarketOrderFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CTickMarketOrderCommitTrigger是确认Tick聚集分价表时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTickMarketOrderCommitTrigger
{
public:
	///构造方法
	CTickMarketOrderCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CTickMarketOrderCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CTickMarketOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pTickMarketOrder	已经加入的TickMarketOrder
	virtual void commitAdd(CTickMarketOrder *pTickMarketOrder)
	{
		return;
	}

	///更新后触发
	///@param	pTickMarketOrder	被刷新的CTickMarketOrder
	///@param	poldTickMarketOrder	原来的值
	virtual void commitUpdate(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pOldTickMarketOrder)
	{
		return;
	}
	
	///删除后触发
	///@param	pTickMarketOrder	已经删除的CTickMarketOrder
	virtual void commitRemove(CWriteableTickMarketOrder *pTickMarketOrder)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CTickMarketOrderFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CTickMarketOrderIterator是一个对Tick聚集分价表的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTickMarketOrderIterator
{
public:
	///构造方法
	CTickMarketOrderIterator()
	{
	}

	///构造方法
	CTickMarketOrderIterator(CTickMarketOrderFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CTickMarketOrderIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个TickMarketOrder
	///@return	下一个满足条件CTickMarketOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CTickMarketOrder *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CTickMarketOrderFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pTickMarketOrder 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTickMarketOrder * pTickMarketOrder,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CTickMarketOrderFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableTrade是一个存储成交的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableTrade
{
public:
	CWriteableTrade()
	{
	}
	
	CWriteableTrade(const CWriteableTrade& c)
	{
		memcpy(this, &c, sizeof(CWriteableTrade));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! TradeID.isValid())
			return "TradeID";
		if ( ! Direction.isValid())
			return "Direction";
		if ( ! OrderID.isValid())
			return "OrderID";
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! PositionID.isValid())
			return "PositionID";
		if ( ! AccountID.isValid())
			return "AccountID";
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! OffsetFlag.isValid())
			return "OffsetFlag";
		if ( ! Price.isValid())
			return "Price";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! DeriveSource.isValid())
			return "DeriveSource";
		if ( ! MatchRole.isValid())
			return "MatchRole";
		if ( ! PriceCurrency.isValid())
			return "PriceCurrency";
		if ( ! ClearCurrency.isValid())
			return "ClearCurrency";
		if ( ! Fee.isValid())
			return "Fee";
		if ( ! FeeCurrency.isValid())
			return "FeeCurrency";
		if ( ! CloseProfit.isValid())
			return "CloseProfit";
		if ( ! Turnover.isValid())
			return "Turnover";
		if ( ! UseMargin.isValid())
			return "UseMargin";
		if ( ! Leverage.isValid())
			return "Leverage";
		if ( ! OrderPrice.isValid())
			return "OrderPrice";
		if ( ! TriggerPrice.isValid())
			return "TriggerPrice";
		if ( ! IsSelfTrade.isValid())
			return "IsSelfTrade";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		if ( ! OpenPrice.isValid())
			return "OpenPrice";
		if ( ! APPID.isValid())
			return "APPID";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! CreateTime.isValid())
			return "CreateTime";
		if ( ! CopyOrderID.isValid())
			return "CopyOrderID";
		if ( ! CopyMemberID.isValid())
			return "CopyMemberID";
		if ( ! CopyProfit.isValid())
			return "CopyProfit";
		if ( ! Position.isValid())
			return "Position";
		if ( ! ReserveProfit.isValid())
			return "ReserveProfit";
		if ( ! ReserveFee.isValid())
			return "ReserveFee";
		if ( ! TradeRemark.isValid())
			return "TradeRemark";
		if ( ! BusinessType.isValid())
			return "BusinessType";
		if ( ! BusinessValue.isValid())
			return "BusinessValue";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableTrade *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///成交代码
	CIndexIDType TradeID;
	///买卖方向
	CDirectionType Direction;
	///报单系统唯一代码
	CIndexIDType OrderID;
	///成员代码
	CLongIndexIDType MemberID;
	///持仓代码
	CLongIndexIDType PositionID;
	///资金账号
	CAccountIDType AccountID;
	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///开平标志
	COffsetFlagType OffsetFlag;
	///成交价格
	CPriceType Price;
	///成交数量
	CVolumeType Volume;
	///衍生类型
	CDeriveSourceType DeriveSource;
	///成交角色
	CMatchRoleType MatchRole;
	///计价币种
	CCurrencyType PriceCurrency;
	///清算币种
	CCurrencyType ClearCurrency;
	///手续费
	CMoneyType Fee;
	///手续费币种
	CCurrencyType FeeCurrency;
	///平仓盈亏
	CMoneyType CloseProfit;
	///成交金额
	CMoneyType Turnover;
	///占用或者减少的保证金
	CMoneyType UseMargin;
	///杠杆倍数
	CLeverageType Leverage;
	///委托价格
	CPriceType OrderPrice;
	///触发价
	CPriceType TriggerPrice;
	///是否自成交
	CBoolType IsSelfTrade;
	///备注
	CNameType Remark;
	///业务序列号
	CLargeNumberType BusinessNo;
	///开仓均价
	CPriceType OpenPrice;
	///应用编号
	CShortIndexIDType APPID;
	///插入时间
	CMilliSecondsType InsertTime;
	///创建时间
	CIndexIDType CreateTime;
	///带单员报单号
	CIndexIDType CopyOrderID;
	///带单员代码
	CLongIndexIDType CopyMemberID;
	///带单盈利分配
	CMoneyType CopyProfit;
	///成交时的持仓量
	CVolumeType Position;
	///保留资金盈亏
	CMoneyType ReserveProfit;
	///保留资金手续费
	CMoneyType ReserveFee;
	///Trade备注
	CNameType TradeRemark;
	///业务类别
	CBusinessTypeType BusinessType;
	///业务值
	CNameType BusinessValue;
	
	
	///计算所有的hash值
	void calAllHash()
	{
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableTrade CTrade;

/////////////////////////////////////////////////////////////////////////
///CTradeActionTrigger是操作成交时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTradeActionTrigger
{
public:
	///构造方法
	CTradeActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CTradeActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CTradeFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pTrade	要加入的Trade
	virtual void beforeAdd(CWriteableTrade *pTrade)
	{
		return;
	}
	
	///加入后触发
	///@param	pTrade	已经加入的Trade
	virtual void afterAdd(CTrade *pTrade)
	{
		return;
	}

	///更新前触发	
	///@param	pTrade	被刷新的CTrade
	///@param	pNewTrade	新的值
	virtual void beforeUpdate(CTrade *pTrade, CWriteableTrade *pNewTrade)
	{
		return;
	}
	
	///更新后触发
	///@param	pTrade	被刷新的CTrade
	virtual void afterUpdate(CTrade *pTrade)
	{
		return;
	}
	
	///删除前触发
	///@param	pTrade	要删除的CTrade
	virtual void beforeRemove(CTrade *pTrade)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CTradeFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CTradeCommitTrigger是确认成交时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTradeCommitTrigger
{
public:
	///构造方法
	CTradeCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CTradeCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CTradeFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pTrade	已经加入的Trade
	virtual void commitAdd(CTrade *pTrade)
	{
		return;
	}

	///更新后触发
	///@param	pTrade	被刷新的CTrade
	///@param	poldTrade	原来的值
	virtual void commitUpdate(CTrade *pTrade, CWriteableTrade *pOldTrade)
	{
		return;
	}
	
	///删除后触发
	///@param	pTrade	已经删除的CTrade
	virtual void commitRemove(CWriteableTrade *pTrade)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CTradeFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CTradeIterator是一个对成交的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTradeIterator
{
public:
	///构造方法
	CTradeIterator()
	{
	}

	///构造方法
	CTradeIterator(CTradeFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CTradeIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个Trade
	///@return	下一个满足条件CTrade，如果已经没有一个满足要求了，则返回NULL
	virtual CTrade *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CTradeFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pTrade 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTrade * pTrade,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CTradeFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableKLine是一个存储K线的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableKLine
{
public:
	CWriteableKLine()
	{
	}
	
	CWriteableKLine(const CWriteableKLine& c)
	{
		memcpy(this, &c, sizeof(CWriteableKLine));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! Bar.isValid())
			return "Bar";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! OpenPrice.isValid())
			return "OpenPrice";
		if ( ! HighestPrice.isValid())
			return "HighestPrice";
		if ( ! LowestPrice.isValid())
			return "LowestPrice";
		if ( ! ClosePrice.isValid())
			return "ClosePrice";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! Turnover.isValid())
			return "Turnover";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableKLine *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///K线周期代码(n[m/h/d/o])
	CBarType Bar;
	///插入时间
	CMilliSecondsType InsertTime;
	///开盘价
	CPriceType OpenPrice;
	///最高价
	CPriceType HighestPrice;
	///最低价
	CPriceType LowestPrice;
	///收盘价
	CPriceType ClosePrice;
	///数量
	CCollectType Volume;
	///成交金额
	CCollectType Turnover;
	
	
	///计算所有的hash值
	void calAllHash()
	{
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableKLine CKLine;

/////////////////////////////////////////////////////////////////////////
///CKLineActionTrigger是操作K线时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CKLineActionTrigger
{
public:
	///构造方法
	CKLineActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CKLineActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CKLineFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pKLine	要加入的KLine
	virtual void beforeAdd(CWriteableKLine *pKLine)
	{
		return;
	}
	
	///加入后触发
	///@param	pKLine	已经加入的KLine
	virtual void afterAdd(CKLine *pKLine)
	{
		return;
	}

	///更新前触发	
	///@param	pKLine	被刷新的CKLine
	///@param	pNewKLine	新的值
	virtual void beforeUpdate(CKLine *pKLine, CWriteableKLine *pNewKLine)
	{
		return;
	}
	
	///更新后触发
	///@param	pKLine	被刷新的CKLine
	virtual void afterUpdate(CKLine *pKLine)
	{
		return;
	}
	
	///删除前触发
	///@param	pKLine	要删除的CKLine
	virtual void beforeRemove(CKLine *pKLine)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CKLineFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CKLineCommitTrigger是确认K线时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CKLineCommitTrigger
{
public:
	///构造方法
	CKLineCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CKLineCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CKLineFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pKLine	已经加入的KLine
	virtual void commitAdd(CKLine *pKLine)
	{
		return;
	}

	///更新后触发
	///@param	pKLine	被刷新的CKLine
	///@param	poldKLine	原来的值
	virtual void commitUpdate(CKLine *pKLine, CWriteableKLine *pOldKLine)
	{
		return;
	}
	
	///删除后触发
	///@param	pKLine	已经删除的CKLine
	virtual void commitRemove(CWriteableKLine *pKLine)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CKLineFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CKLineIterator是一个对K线的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CKLineIterator
{
public:
	///构造方法
	CKLineIterator()
	{
	}

	///构造方法
	CKLineIterator(CKLineFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CKLineIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个KLine
	///@return	下一个满足条件CKLine，如果已经没有一个满足要求了，则返回NULL
	virtual CKLine *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CKLineFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pKLine 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableKLine * pKLine,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CKLineFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteablePublishKLine是一个存储PublishK线的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteablePublishKLine
{
public:
	CWriteablePublishKLine()
	{
	}
	
	CWriteablePublishKLine(const CWriteablePublishKLine& c)
	{
		memcpy(this, &c, sizeof(CWriteablePublishKLine));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! Bar.isValid())
			return "Bar";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! OpenPrice.isValid())
			return "OpenPrice";
		if ( ! HighestPrice.isValid())
			return "HighestPrice";
		if ( ! LowestPrice.isValid())
			return "LowestPrice";
		if ( ! ClosePrice.isValid())
			return "ClosePrice";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! Turnover.isValid())
			return "Turnover";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteablePublishKLine *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///K线周期代码(n[m/h/d/o])
	CBarType Bar;
	///插入时间
	CMilliSecondsType InsertTime;
	///开盘价
	CPriceType OpenPrice;
	///最高价
	CPriceType HighestPrice;
	///最低价
	CPriceType LowestPrice;
	///收盘价
	CPriceType ClosePrice;
	///数量
	CCollectType Volume;
	///成交金额
	CCollectType Turnover;
	
	
	///计算所有的hash值
	void calAllHash()
	{
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteablePublishKLine CPublishKLine;

/////////////////////////////////////////////////////////////////////////
///CPublishKLineActionTrigger是操作PublishK线时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPublishKLineActionTrigger
{
public:
	///构造方法
	CPublishKLineActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CPublishKLineActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CPublishKLineFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pPublishKLine	要加入的PublishKLine
	virtual void beforeAdd(CWriteablePublishKLine *pPublishKLine)
	{
		return;
	}
	
	///加入后触发
	///@param	pPublishKLine	已经加入的PublishKLine
	virtual void afterAdd(CPublishKLine *pPublishKLine)
	{
		return;
	}

	///更新前触发	
	///@param	pPublishKLine	被刷新的CPublishKLine
	///@param	pNewPublishKLine	新的值
	virtual void beforeUpdate(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pNewPublishKLine)
	{
		return;
	}
	
	///更新后触发
	///@param	pPublishKLine	被刷新的CPublishKLine
	virtual void afterUpdate(CPublishKLine *pPublishKLine)
	{
		return;
	}
	
	///删除前触发
	///@param	pPublishKLine	要删除的CPublishKLine
	virtual void beforeRemove(CPublishKLine *pPublishKLine)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CPublishKLineFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CPublishKLineCommitTrigger是确认PublishK线时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPublishKLineCommitTrigger
{
public:
	///构造方法
	CPublishKLineCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CPublishKLineCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CPublishKLineFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pPublishKLine	已经加入的PublishKLine
	virtual void commitAdd(CPublishKLine *pPublishKLine)
	{
		return;
	}

	///更新后触发
	///@param	pPublishKLine	被刷新的CPublishKLine
	///@param	poldPublishKLine	原来的值
	virtual void commitUpdate(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pOldPublishKLine)
	{
		return;
	}
	
	///删除后触发
	///@param	pPublishKLine	已经删除的CPublishKLine
	virtual void commitRemove(CWriteablePublishKLine *pPublishKLine)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CPublishKLineFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CPublishKLineIterator是一个对PublishK线的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPublishKLineIterator
{
public:
	///构造方法
	CPublishKLineIterator()
	{
	}

	///构造方法
	CPublishKLineIterator(CPublishKLineFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CPublishKLineIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个PublishKLine
	///@return	下一个满足条件CPublishKLine，如果已经没有一个满足要求了，则返回NULL
	virtual CPublishKLine *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CPublishKLineFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pPublishKLine 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteablePublishKLine * pPublishKLine,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CPublishKLineFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableLastKLine是一个存储最新K线的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableLastKLine
{
public:
	CWriteableLastKLine()
	{
	}
	
	CWriteableLastKLine(const CWriteableLastKLine& c)
	{
		memcpy(this, &c, sizeof(CWriteableLastKLine));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! Bar.isValid())
			return "Bar";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! OpenPrice.isValid())
			return "OpenPrice";
		if ( ! HighestPrice.isValid())
			return "HighestPrice";
		if ( ! LowestPrice.isValid())
			return "LowestPrice";
		if ( ! ClosePrice.isValid())
			return "ClosePrice";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! Turnover.isValid())
			return "Turnover";
		if ( ! UpdateTime.isValid())
			return "UpdateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableLastKLine *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///K线周期代码(n[m/h/d/o])
	CBarType Bar;
	///插入时间
	CMilliSecondsType InsertTime;
	///开盘价
	CPriceType OpenPrice;
	///最高价
	CPriceType HighestPrice;
	///最低价
	CPriceType LowestPrice;
	///收盘价
	CPriceType ClosePrice;
	///数量
	CCollectType Volume;
	///成交金额
	CCollectType Turnover;
	///更新时间
	CMilliSecondsType UpdateTime;
	
	unsigned int HashInstrumentBarHashKey;
	
	///计算InstrumentBarHashKey的hash值
	void calHashInstrumentBarHashKey()
	{
		unsigned key=0;
		key=ExchangeID.hash(key);
		key=InstrumentID.hash(key);
		key=Bar.hash(key);
		HashInstrumentBarHashKey=key;
	}
	
	///根据需要情况，更新InstrumentBarHashKey的hash值
	void updateHashInstrumentBarHashKey()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashInstrumentBarHashKey();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashInstrumentBarHashKey();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableLastKLine CLastKLine;

/////////////////////////////////////////////////////////////////////////
///CLastKLineActionTrigger是操作最新K线时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CLastKLineActionTrigger
{
public:
	///构造方法
	CLastKLineActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CLastKLineActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CLastKLineFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pLastKLine	要加入的LastKLine
	virtual void beforeAdd(CWriteableLastKLine *pLastKLine)
	{
		return;
	}
	
	///加入后触发
	///@param	pLastKLine	已经加入的LastKLine
	virtual void afterAdd(CLastKLine *pLastKLine)
	{
		return;
	}

	///更新前触发	
	///@param	pLastKLine	被刷新的CLastKLine
	///@param	pNewLastKLine	新的值
	virtual void beforeUpdate(CLastKLine *pLastKLine, CWriteableLastKLine *pNewLastKLine)
	{
		return;
	}
	
	///更新后触发
	///@param	pLastKLine	被刷新的CLastKLine
	virtual void afterUpdate(CLastKLine *pLastKLine)
	{
		return;
	}
	
	///删除前触发
	///@param	pLastKLine	要删除的CLastKLine
	virtual void beforeRemove(CLastKLine *pLastKLine)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CLastKLineFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CLastKLineCommitTrigger是确认最新K线时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CLastKLineCommitTrigger
{
public:
	///构造方法
	CLastKLineCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CLastKLineCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CLastKLineFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pLastKLine	已经加入的LastKLine
	virtual void commitAdd(CLastKLine *pLastKLine)
	{
		return;
	}

	///更新后触发
	///@param	pLastKLine	被刷新的CLastKLine
	///@param	poldLastKLine	原来的值
	virtual void commitUpdate(CLastKLine *pLastKLine, CWriteableLastKLine *pOldLastKLine)
	{
		return;
	}
	
	///删除后触发
	///@param	pLastKLine	已经删除的CLastKLine
	virtual void commitRemove(CWriteableLastKLine *pLastKLine)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CLastKLineFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CLastKLineIterator是一个对最新K线的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CLastKLineIterator
{
public:
	///构造方法
	CLastKLineIterator()
	{
	}

	///构造方法
	CLastKLineIterator(CLastKLineFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CLastKLineIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个LastKLine
	///@return	下一个满足条件CLastKLine，如果已经没有一个满足要求了，则返回NULL
	virtual CLastKLine *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CLastKLineFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pLastKLine 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableLastKLine * pLastKLine,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CLastKLineFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableAccountDetail是一个存储资金明细的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableAccountDetail
{
public:
	CWriteableAccountDetail()
	{
	}
	
	CWriteableAccountDetail(const CWriteableAccountDetail& c)
	{
		memcpy(this, &c, sizeof(CWriteableAccountDetail));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! AccountDetailID.isValid())
			return "AccountDetailID";
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! SettlementGroup.isValid())
			return "SettlementGroup";
		if ( ! AccountID.isValid())
			return "AccountID";
		if ( ! Currency.isValid())
			return "Currency";
		if ( ! Amount.isValid())
			return "Amount";
		if ( ! PreBalance.isValid())
			return "PreBalance";
		if ( ! Balance.isValid())
			return "Balance";
		if ( ! ReserveAmount.isValid())
			return "ReserveAmount";
		if ( ! ReserveBalance.isValid())
			return "ReserveBalance";
		if ( ! Source.isValid())
			return "Source";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! LocalID.isValid())
			return "LocalID";
		if ( ! SettleSegment.isValid())
			return "SettleSegment";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		if ( ! RelatedID.isValid())
			return "RelatedID";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! CreateTime.isValid())
			return "CreateTime";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableAccountDetail *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///资金明细号
	CIndexIDType AccountDetailID;
	///成员代码
	CLongIndexIDType MemberID;
	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///结算组编号
	CShortIndexIDType SettlementGroup;
	///资金账号
	CAccountIDType AccountID;
	///币种
	CCurrencyType Currency;
	///发生额
	CMoneyType Amount;
	///上次静态权益
	CMoneyType PreBalance;
	///静态权益
	CMoneyType Balance;
	///体验金发生额
	CMoneyType ReserveAmount;
	///体验金静态权益
	CMoneyType ReserveBalance;
	///财务流水类型
	CDefineTypeType Source;
	///备注
	CNameType Remark;
	///本地标识
	CIndexIDType LocalID;
	///结算段
	CIndexIDType SettleSegment;
	///业务序列号
	CLargeNumberType BusinessNo;
	///内外对账ID
	CLongNameType RelatedID;
	///插入时间
	CMilliSecondsType InsertTime;
	///创建时间
	CIndexIDType CreateTime;
	
	
	///计算所有的hash值
	void calAllHash()
	{
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableAccountDetail CAccountDetail;

/////////////////////////////////////////////////////////////////////////
///CAccountDetailActionTrigger是操作资金明细时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountDetailActionTrigger
{
public:
	///构造方法
	CAccountDetailActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CAccountDetailActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CAccountDetailFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pAccountDetail	要加入的AccountDetail
	virtual void beforeAdd(CWriteableAccountDetail *pAccountDetail)
	{
		return;
	}
	
	///加入后触发
	///@param	pAccountDetail	已经加入的AccountDetail
	virtual void afterAdd(CAccountDetail *pAccountDetail)
	{
		return;
	}

	///更新前触发	
	///@param	pAccountDetail	被刷新的CAccountDetail
	///@param	pNewAccountDetail	新的值
	virtual void beforeUpdate(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pNewAccountDetail)
	{
		return;
	}
	
	///更新后触发
	///@param	pAccountDetail	被刷新的CAccountDetail
	virtual void afterUpdate(CAccountDetail *pAccountDetail)
	{
		return;
	}
	
	///删除前触发
	///@param	pAccountDetail	要删除的CAccountDetail
	virtual void beforeRemove(CAccountDetail *pAccountDetail)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CAccountDetailFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CAccountDetailCommitTrigger是确认资金明细时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountDetailCommitTrigger
{
public:
	///构造方法
	CAccountDetailCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CAccountDetailCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CAccountDetailFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pAccountDetail	已经加入的AccountDetail
	virtual void commitAdd(CAccountDetail *pAccountDetail)
	{
		return;
	}

	///更新后触发
	///@param	pAccountDetail	被刷新的CAccountDetail
	///@param	poldAccountDetail	原来的值
	virtual void commitUpdate(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pOldAccountDetail)
	{
		return;
	}
	
	///删除后触发
	///@param	pAccountDetail	已经删除的CAccountDetail
	virtual void commitRemove(CWriteableAccountDetail *pAccountDetail)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CAccountDetailFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CAccountDetailIterator是一个对资金明细的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountDetailIterator
{
public:
	///构造方法
	CAccountDetailIterator()
	{
	}

	///构造方法
	CAccountDetailIterator(CAccountDetailFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CAccountDetailIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个AccountDetail
	///@return	下一个满足条件CAccountDetail，如果已经没有一个满足要求了，则返回NULL
	virtual CAccountDetail *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CAccountDetailFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pAccountDetail 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableAccountDetail * pAccountDetail,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CAccountDetailFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableSettleDetail是一个存储结算明细的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableSettleDetail
{
public:
	CWriteableSettleDetail()
	{
	}
	
	CWriteableSettleDetail(const CWriteableSettleDetail& c)
	{
		memcpy(this, &c, sizeof(CWriteableSettleDetail));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! SettleDetailID.isValid())
			return "SettleDetailID";
		if ( ! APPID.isValid())
			return "APPID";
		if ( ! LocalID.isValid())
			return "LocalID";
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! SettlementGroup.isValid())
			return "SettlementGroup";
		if ( ! SettleAction.isValid())
			return "SettleAction";
		if ( ! Value.isValid())
			return "Value";
		if ( ! Value1.isValid())
			return "Value1";
		if ( ! Value2.isValid())
			return "Value2";
		if ( ! Value3.isValid())
			return "Value3";
		if ( ! Remark.isValid())
			return "Remark";
		if ( ! FundingRateGroup.isValid())
			return "FundingRateGroup";
		if ( ! SettleSegment.isValid())
			return "SettleSegment";
		if ( ! InsertTime.isValid())
			return "InsertTime";
		if ( ! BusinessNo.isValid())
			return "BusinessNo";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableSettleDetail *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///操作明细号
	CIndexIDType SettleDetailID;
	///应用编号
	CShortIndexIDType APPID;
	///成交对本地标识
	CIndexIDType LocalID;
	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///结算组编号
	CShortIndexIDType SettlementGroup;
	///结算操作
	CSettleActionTypeType SettleAction;
	///操作使用值
	CRatioType Value;
	///发生值1
	CRatioType Value1;
	///发生值2
	CRatioType Value2;
	///发生值3
	CRatioType Value3;
	///备注
	CNameType Remark;
	///资金费用组
	CShortIndexIDType FundingRateGroup;
	///结算段
	CIndexIDType SettleSegment;
	///创建时间
	CIndexIDType InsertTime;
	///业务序列号
	CLargeNumberType BusinessNo;
	
	unsigned int HashInstrumentHashKey;
	
	///计算InstrumentHashKey的hash值
	void calHashInstrumentHashKey()
	{
		unsigned key=0;
		key=SettleDetailID.hash(key);
		key=ExchangeID.hash(key);
		key=InstrumentID.hash(key);
		HashInstrumentHashKey=key;
	}
	
	///根据需要情况，更新InstrumentHashKey的hash值
	void updateHashInstrumentHashKey()
	{
	}
	
	///计算所有的hash值
	void calAllHash()
	{
		calHashInstrumentHashKey();
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
		updateHashInstrumentHashKey();
	}
	
	CMDB* m_pMDB;
private:
};

typedef const CWriteableSettleDetail CSettleDetail;

/////////////////////////////////////////////////////////////////////////
///CSettleDetailActionTrigger是操作结算明细时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CSettleDetailActionTrigger
{
public:
	///构造方法
	CSettleDetailActionTrigger()
	{
	}
	
	///析构方法
	virtual ~CSettleDetailActionTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CSettleDetailFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入前触发
	///@param	pSettleDetail	要加入的SettleDetail
	virtual void beforeAdd(CWriteableSettleDetail *pSettleDetail)
	{
		return;
	}
	
	///加入后触发
	///@param	pSettleDetail	已经加入的SettleDetail
	virtual void afterAdd(CSettleDetail *pSettleDetail)
	{
		return;
	}

	///更新前触发	
	///@param	pSettleDetail	被刷新的CSettleDetail
	///@param	pNewSettleDetail	新的值
	virtual void beforeUpdate(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pNewSettleDetail)
	{
		return;
	}
	
	///更新后触发
	///@param	pSettleDetail	被刷新的CSettleDetail
	virtual void afterUpdate(CSettleDetail *pSettleDetail)
	{
		return;
	}
	
	///删除前触发
	///@param	pSettleDetail	要删除的CSettleDetail
	virtual void beforeRemove(CSettleDetail *pSettleDetail)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CSettleDetailFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CSettleDetailCommitTrigger是确认结算明细时的触发器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CSettleDetailCommitTrigger
{
public:
	///构造方法
	CSettleDetailCommitTrigger()
	{
	}
	
	///析构方法
	virtual ~CSettleDetailCommitTrigger()
	{
	}

	///设定对应的对象工厂
	///@param	pFactory	指定的对象工厂
	void setFactory(CSettleDetailFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///加入后触发
	///@param	pSettleDetail	已经加入的SettleDetail
	virtual void commitAdd(CSettleDetail *pSettleDetail)
	{
		return;
	}

	///更新后触发
	///@param	pSettleDetail	被刷新的CSettleDetail
	///@param	poldSettleDetail	原来的值
	virtual void commitUpdate(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pOldSettleDetail)
	{
		return;
	}
	
	///删除后触发
	///@param	pSettleDetail	已经删除的CSettleDetail
	virtual void commitRemove(CWriteableSettleDetail *pSettleDetail)
	{
		return;
	}
protected:
	///该触发器对应的对象工厂
	CSettleDetailFactory *m_Factory;
};

/////////////////////////////////////////////////////////////////////////
///CSettleDetailIterator是一个对结算明细的枚举器接口
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CSettleDetailIterator
{
public:
	///构造方法
	CSettleDetailIterator()
	{
	}

	///构造方法
	CSettleDetailIterator(CSettleDetailFactory *pFactory)
	{
		m_Factory=pFactory;
	}

	///析构方法，将负责释放所占用的空间
	virtual ~CSettleDetailIterator(){};
	
	///释放自己
	virtual void free()
	{
		delete this;
	}

	///获得下一个SettleDetail
	///@return	下一个满足条件CSettleDetail，如果已经没有一个满足要求了，则返回NULL
	virtual CSettleDetail *next()=0;
	
	///获得对应的对象工厂
	///@return	对应的对象工厂
	CSettleDetailFactory *getFactory()
	{
		return m_Factory;
	}
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL) = 0;
	
	///更新当前Iterator指向的纪录
	///@param	pSettleDetail 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableSettleDetail * pSettleDetail,CTransaction *pTransaction = NULL, bool updateIndex=true) = 0;
	
	///将本枚举器中所有内容全部输出
	///@param	output	要输出的位置
	void dump(FILE *output);

protected:
	///进行查询时使用的对象工厂
	CSettleDetailFactory *m_Factory;	
};


/////////////////////////////////////////////////////////////////////////
///CWriteableOrderAction是一个存储报单操作的对象，可以写出。
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CWriteableOrderAction
{
public:
	CWriteableOrderAction()
	{
		pOrder=NULL;
		pTriggerOrder=NULL;
		pMember=NULL;
	}
	
	CWriteableOrderAction(const CWriteableOrderAction& c)
	{
		memcpy(this, &c, sizeof(CWriteableOrderAction));
	}
		
	///向CSV文件中写出标题行
	///@param	output	要写出的文件
	static void writeCSVHead(FILE *output);
	static void writeCSVHeadPreVersion(FILE *output);	
	
	///初始化操作，将所有的字段根据是否为空初始化相应的值
	void init();
	void init(CMDB *pMDB);
	///从文件读入
	///@param	input	读入的文件
	///@return	1表示读入成功，0表示读入失败
	int read(FILE *input);
	
	///向文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int write(FILE *output) const;
	
	///从CSV文件读入
	///@param	input	读入的文件
	///@param	pNames	各个字段的名称，如果为NULL，则顺序读出
	///@return	1表示读入成功，0表示读入失败
	int readCSV(FILE *input,vector<char *> *pNames=NULL);
	
	///向CSV文件写出
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSV(FILE *output) const;
	
	///向CSV文件写出上个版本
	///@param	output 写出的文件
	///@return	1表时写出成功，0表示写出失败
	int writeCSVPreVersion(FILE *output) const;	
	
	///检查数据的合法性
	///@return	1表示合法，0表示不合法
	const char* GetInValid() const
	{
		if ( ! OrderID.isValid())
			return "OrderID";
		if ( ! OrderLocalID.isValid())
			return "OrderLocalID";
		if ( ! ActionFlag.isValid())
			return "ActionFlag";
		if ( ! LocalID.isValid())
			return "LocalID";
		if ( ! MemberID.isValid())
			return "MemberID";
		if ( ! AccountID.isValid())
			return "AccountID";
		if ( ! Price.isValid())
			return "Price";
		if ( ! Volume.isValid())
			return "Volume";
		if ( ! Direction.isValid())
			return "Direction";
		if ( ! VolumeDisplay.isValid())
			return "VolumeDisplay";
		if ( ! ProductGroup.isValid())
			return "ProductGroup";
		if ( ! ExchangeID.isValid())
			return "ExchangeID";
		if ( ! InstrumentID.isValid())
			return "InstrumentID";
		if ( ! Cost.isValid())
			return "Cost";
		if ( ! Remark.isValid())
			return "Remark";
		return "";
	}

	///将内容写到文件中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	index	要在前面显示的下标值，一般用于数组分量的显示，用-1表示不显示
	void dump(FILE *fp=NULL,int index=-1) const;
	
	///将内容写入文件，突出显示变化的内容
	///@param	fp	要写出的文件,NULL表示stdout
	///@param	pOriginal	要比较的原来内容
	void dumpDiff(FILE *fp, const CWriteableOrderAction *pOriginal) const;
	
	///将内容写到文件的一行中,用于DEBUG
	///@param	fp	要写出的文件,NULL表示stdout
	void dumpInLine(FILE *fp=NULL) const;

	///将内容写到一个字符串中
	///@param	target	要写的字符串，叫用方应当保证其有足够的空间
	void dumpString(char *target) const;

	///报单系统唯一代码
	CIndexIDType OrderID;
	///报单本地标识
	CIndexIDType OrderLocalID;
	///报单操作标志
	CActionFlagType ActionFlag;
	///操作本地标识
	CIndexIDType LocalID;
	///成员代码
	CLongIndexIDType MemberID;
	///资金账号
	CAccountIDType AccountID;
	///价格
	CPriceType Price;
	///数量变化
	CVolumeType Volume;
	///买卖方向
	CDirectionType Direction;
	///显示数量
	CVolumeType VolumeDisplay;
	///产品组
	CShortIndexIDType ProductGroup;
	///交易所代码
	CExchangeIDType ExchangeID;
	///标的代码
	CInstrumentIDType InstrumentID;
	///委托额
	CMoneyType Cost;
	///备注
	CNameType Remark;
	
	
	///计算所有的hash值
	void calAllHash()
	{
	}
	
	///更新所有的hash值
	void updateAllHash()
	{
	}
	
	///构造对应的报单的指针
	///@param	pFactory	构造对应的报单的指针时，寻找的对象工厂
	///@return	找到的对应的报单的指针
	const CWriteableOrder* linkOrder() const;

	///构造对应的报单指针
	///@param	pOrder	要连接到的对象
	///@return	输入的pOrder
	const CWriteableOrder* linkOrder(const CWriteableOrder *pOrder) const
	{
		void *target=(void *)(&(this->pOrder));
		*((const CWriteableOrder **)target)=pOrder;
		return pOrder;
	}
	
	///获取对应的报单的指针
	///@return	找到的对应的报单的指针
	const CWriteableOrder* getOrder() const
	{
		if(pOrder==NULL)
			return linkOrder();
		return pOrder;
	}
	
	///构造对应的报单的指针
	///@param	pFactory	构造对应的报单的指针时，寻找的对象工厂
	///@return	找到的对应的报单的指针
	const CWriteableTriggerOrder* linkTriggerOrder() const;

	///构造对应的报单指针
	///@param	pTriggerOrder	要连接到的对象
	///@return	输入的pTriggerOrder
	const CWriteableTriggerOrder* linkTriggerOrder(const CWriteableTriggerOrder *pTriggerOrder) const
	{
		void *target=(void *)(&(this->pTriggerOrder));
		*((const CWriteableTriggerOrder **)target)=pTriggerOrder;
		return pTriggerOrder;
	}
	
	///获取对应的报单的指针
	///@return	找到的对应的报单的指针
	const CWriteableTriggerOrder* getTriggerOrder() const
	{
		if(pTriggerOrder==NULL)
			return linkTriggerOrder();
		return pTriggerOrder;
	}
	
	///构造对应的用户的指针
	///@param	pFactory	构造对应的用户的指针时，寻找的对象工厂
	///@return	找到的对应的用户的指针
	const CWriteableMember* linkMember() const;

	///构造对应的用户指针
	///@param	pMember	要连接到的对象
	///@return	输入的pMember
	const CWriteableMember* linkMember(const CWriteableMember *pMember) const
	{
		void *target=(void *)(&(this->pMember));
		*((const CWriteableMember **)target)=pMember;
		return pMember;
	}
	
	///获取对应的用户的指针
	///@return	找到的对应的用户的指针
	const CWriteableMember* getMember() const
	{
		if(pMember==NULL)
			return linkMember();
		return pMember;
	}
	
	CMDB* m_pMDB;
private:
	///对应的报单
	const CWriteableOrder *pOrder;
	///对应的报单
	const CWriteableTriggerOrder *pTriggerOrder;
	///对应的用户
	const CWriteableMember *pMember;
};

typedef const CWriteableOrderAction COrderAction;


#endif
