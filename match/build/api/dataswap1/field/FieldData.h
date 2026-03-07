#ifndef _FIELDDATA_H__
#define _FIELDDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "FieldDescribe.h"
#include "DataType.h"

#define FINISHORDER

///主题订阅操作
class CTopicActionField
{
public:
	///订阅操作
	CSubActionType	Action;
	///请参考TOPICID类型定义
	CLongIndexIDType	TopicID;
	///过滤值
	CShortContentType	Index;
	///续传号:0是从头开始,-1是从服务端最新位置续传
	CLargeNumberType	ResumeNo;
	///续传号:0是从头开始,不能为负数
	CLargeNumberType	BusinessNo;
	///令牌
	CShortContentType	Token;
	///压缩方式
	CNumberType	Zip;
	///每间隔多少频率读取一次
	CNumberType	Frequency;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(Action,(int)((char *)&Action-(char *)this),"Action",sizeof(Action),"Action","Action",0);
		m_Describe.SetupMember(TopicID,(int)((char *)&TopicID-(char *)this),"TopicID",sizeof(TopicID),"TopicID","TopicID",0);
		m_Describe.SetupMember(Index,(int)((char *)&Index-(char *)this),"Index",sizeof(Index),"Index","Index",0);
		m_Describe.SetupMember(ResumeNo,(int)((char *)&ResumeNo-(char *)this),"ResumeNo",sizeof(ResumeNo),"ResumeNo","ResumeNo",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(Token,(int)((char *)&Token-(char *)this),"Token",sizeof(Token),"Token","Token",0);
		m_Describe.SetupMember(Zip,(int)((char *)&Zip-(char *)this),"Zip",sizeof(Zip),"Zip","Zip",0);
		m_Describe.SetupMember(Frequency,(int)((char *)&Frequency-(char *)this),"Frequency",sizeof(Frequency),"Frequency","Frequency",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_TopicAction=0x2001;

///操作信息
class CCommandHeadField
{
public:
	///是否最后
	CBoolType	IsLast;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(IsLast,(int)((char *)&IsLast-(char *)this),"IsLast",sizeof(IsLast),"IsLast","IsLast",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_CommandHead=0x2002;

///响应信息
class CErrorField
{
public:
	///错误代码
	CNumberType	ErrorNo;
	///错误信息
	CShortContentType	ErrorMsg;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ErrorNo,(int)((char *)&ErrorNo-(char *)this),"ErrorNo",sizeof(ErrorNo),"ErrorNo","ErrorNo",0);
		m_Describe.SetupMember(ErrorMsg,(int)((char *)&ErrorMsg-(char *)this),"ErrorMsg",sizeof(ErrorMsg),"ErrorMsg","ErrorMsg",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Error=0x2003;

///响应信息
class CCheckTokenField
{
public:
	///令牌
	CShortContentType	Token;
	///交易用户代码
	CLongIndexIDType	UserID;
	///最大本地标识
	CIndexIDType	MaxLocalID;
	///成员代码
	CLongIndexIDType	MemberID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(Token,(int)((char *)&Token-(char *)this),"Token",sizeof(Token),"Token","Token",0);
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(MaxLocalID,(int)((char *)&MaxLocalID-(char *)this),"MaxLocalID",sizeof(MaxLocalID),"MaxLocalID","MaxLocalID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_CheckToken=0x2004;

///消息
class CCommandField
{
public:
	///交易用户代码
	CLongIndexIDType	UserID;
	///命令本地号
	CIndexIDType	LocalID;
	///命令
	CNameType	Command;
	///字段内容
	CLongContentType	Content;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(Command,(int)((char *)&Command-(char *)this),"Command",sizeof(Command),"Command","Command",0);
		m_Describe.SetupMember(Content,(int)((char *)&Content-(char *)this),"Content",sizeof(Content),"Content","Content",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Command=0x2100;

///用户登录
class CUserLoginField
{
public:
	///交易用户代码
	CLongIndexIDType	UserID;
	///密码
	CPasswordType	Password;
	///用户端产品信息
	CNameType	UserProductID;
	///Mac地址
	CMacAddressType	MacAddress;
	///终端IP地址
	CIPAddressType	ClientIPAddress;
	///硬盘序列号
	CLongNameType	HDSerialID;
	///授权编码
	CShortContentType	AuthCode;
	///应用编号
	CShortIndexIDType	APPID;
	///令牌
	CShortContentType	Token;
	///备注
	CLongNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(Password,(int)((char *)&Password-(char *)this),"Password",sizeof(Password),"Password","Password",0);
		m_Describe.SetupMember(UserProductID,(int)((char *)&UserProductID-(char *)this),"UserProductID",sizeof(UserProductID),"UserProductID","UserProductID",0);
		m_Describe.SetupMember(MacAddress,(int)((char *)&MacAddress-(char *)this),"MacAddress",sizeof(MacAddress),"MacAddress","MacAddress",0);
		m_Describe.SetupMember(ClientIPAddress,(int)((char *)&ClientIPAddress-(char *)this),"ClientIPAddress",sizeof(ClientIPAddress),"ClientIPAddress","ClientIPAddress",0);
		m_Describe.SetupMember(HDSerialID,(int)((char *)&HDSerialID-(char *)this),"HDSerialID",sizeof(HDSerialID),"HDSerialID","HDSerialID",0);
		m_Describe.SetupMember(AuthCode,(int)((char *)&AuthCode-(char *)this),"AuthCode",sizeof(AuthCode),"AuthCode","AuthCode",0);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(Token,(int)((char *)&Token-(char *)this),"Token",sizeof(Token),"Token","Token",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_UserLogin=0x2101;

///向上通道
class CLinkField
{
public:
	///通道号
	CNumberType	LinkNo;
	///通道用户
	CLongIndexIDType	LinkUserID;
	///通道用户密码
	CPasswordType	LinkPassword;
	///密码加密算法
	CEncryptTypeType	EncryptType;
	///LinkDll名
	CNameType	LinkDllName;
	///LinkDll加载库
	CNameType	LinkDllLoad;
	///Link连接地址
	CContentType	LinkAddress;
	///Link状态
	CLinkStatusType	LinkStatus;
	///是否活跃
	CBoolType	IsActive;
	///报盘自定义的配置
	CContentType	Config;
	///备注
	CLongNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LinkNo,(int)((char *)&LinkNo-(char *)this),"LinkNo",sizeof(LinkNo),"LinkNo","LinkNo",0);
		m_Describe.SetupMember(LinkUserID,(int)((char *)&LinkUserID-(char *)this),"LinkUserID",sizeof(LinkUserID),"LinkUserID","LinkUserID",0);
		m_Describe.SetupMember(LinkPassword,(int)((char *)&LinkPassword-(char *)this),"LinkPassword",sizeof(LinkPassword),"LinkPassword","LinkPassword",0);
		m_Describe.SetupMember(EncryptType,(int)((char *)&EncryptType-(char *)this),"EncryptType",sizeof(EncryptType),"EncryptType","EncryptType",0);
		m_Describe.SetupMember(LinkDllName,(int)((char *)&LinkDllName-(char *)this),"LinkDllName",sizeof(LinkDllName),"LinkDllName","LinkDllName",0);
		m_Describe.SetupMember(LinkDllLoad,(int)((char *)&LinkDllLoad-(char *)this),"LinkDllLoad",sizeof(LinkDllLoad),"LinkDllLoad","LinkDllLoad",0);
		m_Describe.SetupMember(LinkAddress,(int)((char *)&LinkAddress-(char *)this),"LinkAddress",sizeof(LinkAddress),"LinkAddress","LinkAddress",0);
		m_Describe.SetupMember(LinkStatus,(int)((char *)&LinkStatus-(char *)this),"LinkStatus",sizeof(LinkStatus),"LinkStatus","LinkStatus",0);
		m_Describe.SetupMember(IsActive,(int)((char *)&IsActive-(char *)this),"IsActive",sizeof(IsActive),"IsActive","IsActive",0);
		m_Describe.SetupMember(Config,(int)((char *)&Config-(char *)this),"Config",sizeof(Config),"Config","Config",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Link=0x2102;

///用户登录退出
class CUserLogoutField
{
public:
	///交易用户代码
	CLongIndexIDType	UserID;
	///应用编号
	CShortIndexIDType	APPID;
	///令牌
	CShortContentType	Token;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(Token,(int)((char *)&Token-(char *)this),"Token",sizeof(Token),"Token","Token",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_UserLogout=0x2103;

///用户口令修改
class CUserPasswordUpdateField
{
public:
	///交易用户代码
	CLongIndexIDType	UserID;
	///旧密码
	CPasswordType	OldPassword;
	///新密码
	CPasswordType	NewPassword;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(OldPassword,(int)((char *)&OldPassword-(char *)this),"OldPassword",sizeof(OldPassword),"OldPassword","OldPassword",0);
		m_Describe.SetupMember(NewPassword,(int)((char *)&NewPassword-(char *)this),"NewPassword",sizeof(NewPassword),"NewPassword","NewPassword",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_UserPasswordUpdate=0x2104;

///输入报单
class CTriggerOrderInsertField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///资金账号
	CAccountIDType	AccountID;
	///报单本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单价格类型
	COrderPriceTypeType	OrderPriceType;
	///买卖方向
	CDirectionType	Direction;
	///开平标志
	COffsetFlagType	OffsetFlag;
	///报单价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///显示数量
	CVolumeType	VolumeDisplay;
	///数量取值方式
	CValueModeType	VolumeMode;
	///委托额
	CMoneyType	Cost;
	///订单类型
	COrderTypeType	OrderType;
	///GTD时间
	CMilliSecondsType	GTDTime;
	///最小成交量
	CVolumeType	MinVolume;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType	CloseOrderID;
	///是否全仓
	CBoolType	IsCrossMargin;
	///备注
	CNameType	Remark;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单员报单号
	CIndexIDType	CopyOrderID;
	///委托单杠杆倍数
	CLeverageType	Leverage;
	///带单分配比例
	CRatioType	CopyProfitRate;
	///应用编号
	CShortIndexIDType	APPID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///触发价
	CPriceType	TriggerPrice;
	///保留资金
	CMoneyType	Reserve;
	///止损价
	CPriceType	SLPrice;
	///止损触发价
	CPriceType	SLTriggerPrice;
	///止盈价
	CPriceType	TPPrice;
	///止盈触发价
	CPriceType	TPTriggerPrice;
	///是否提前检查风控
	CBoolType	RiskBefore;
	///触发的订单类型
	CTriggerOrderTypeType	TriggerOrderType;
	///触发类型明细
	CIndexIDType	TriggerDetail;
	///触发价类型
	CTriggerPriceTypeType	TriggerPriceType;
	///触发单具体设置信息
	CLongContentType	TriggerValue;
	///平仓止损价
	CPriceType	CloseSLPrice;
	///平仓止损触发价
	CPriceType	CloseSLTriggerPrice;
	///平仓止盈价
	CPriceType	CloseTPPrice;
	///平仓止盈触发价
	CPriceType	CloseTPTriggerPrice;
	///报单价格类型
	COrderPriceTypeType	CloseOrderPriceType;
	///平仓价
	CPriceType	ClosePrice;
	///平仓触发价
	CPriceType	CloseTriggerPrice;
	///关联报单号
	CIndexIDType	RelatedOrderID;
	///激活时间
	CMilliSecondsType	ActiveTime;
	///触发时间
	CMilliSecondsType	TriggerTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OrderPriceType,(int)((char *)&OrderPriceType-(char *)this),"OrderPriceType",sizeof(OrderPriceType),"OrderPriceType","OrderPriceType",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(OffsetFlag,(int)((char *)&OffsetFlag-(char *)this),"OffsetFlag",sizeof(OffsetFlag),"OffsetFlag","OffsetFlag",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(VolumeMode,(int)((char *)&VolumeMode-(char *)this),"VolumeMode",sizeof(VolumeMode),"VolumeMode","VolumeMode",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(OrderType,(int)((char *)&OrderType-(char *)this),"OrderType",sizeof(OrderType),"OrderType","OrderType",0);
		m_Describe.SetupMember(GTDTime,(int)((char *)&GTDTime-(char *)this),"GTDTime",sizeof(GTDTime),"GTDTime","GTDTime",3);
		m_Describe.SetupMember(MinVolume,(int)((char *)&MinVolume-(char *)this),"MinVolume",sizeof(MinVolume),"MinVolume","MinVolume",10);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
		m_Describe.SetupMember(CloseOrderID,(int)((char *)&CloseOrderID-(char *)this),"CloseOrderID",sizeof(CloseOrderID),"CloseOrderID","CloseOrderID",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyOrderID,(int)((char *)&CopyOrderID-(char *)this),"CopyOrderID",sizeof(CopyOrderID),"CopyOrderID","CopyOrderID",0);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(CopyProfitRate,(int)((char *)&CopyProfitRate-(char *)this),"CopyProfitRate",sizeof(CopyProfitRate),"CopyProfitRate","CopyProfitRate",10);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
		m_Describe.SetupMember(SLPrice,(int)((char *)&SLPrice-(char *)this),"SLPrice",sizeof(SLPrice),"SLPrice","SLPrice",15);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(TPPrice,(int)((char *)&TPPrice-(char *)this),"TPPrice",sizeof(TPPrice),"TPPrice","TPPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(RiskBefore,(int)((char *)&RiskBefore-(char *)this),"RiskBefore",sizeof(RiskBefore),"RiskBefore","RiskBefore",0);
		m_Describe.SetupMember(TriggerOrderType,(int)((char *)&TriggerOrderType-(char *)this),"TriggerOrderType",sizeof(TriggerOrderType),"TriggerOrderType","TriggerOrderType",0);
		m_Describe.SetupMember(TriggerDetail,(int)((char *)&TriggerDetail-(char *)this),"TriggerDetail",sizeof(TriggerDetail),"TriggerDetail","TriggerDetail",0);
		m_Describe.SetupMember(TriggerPriceType,(int)((char *)&TriggerPriceType-(char *)this),"TriggerPriceType",sizeof(TriggerPriceType),"TriggerPriceType","TriggerPriceType",0);
		m_Describe.SetupMember(TriggerValue,(int)((char *)&TriggerValue-(char *)this),"TriggerValue",sizeof(TriggerValue),"TriggerValue","TriggerValue",0);
		m_Describe.SetupMember(CloseSLPrice,(int)((char *)&CloseSLPrice-(char *)this),"CloseSLPrice",sizeof(CloseSLPrice),"CloseSLPrice","CloseSLPrice",15);
		m_Describe.SetupMember(CloseSLTriggerPrice,(int)((char *)&CloseSLTriggerPrice-(char *)this),"CloseSLTriggerPrice",sizeof(CloseSLTriggerPrice),"CloseSLTriggerPrice","CloseSLTriggerPrice",15);
		m_Describe.SetupMember(CloseTPPrice,(int)((char *)&CloseTPPrice-(char *)this),"CloseTPPrice",sizeof(CloseTPPrice),"CloseTPPrice","CloseTPPrice",15);
		m_Describe.SetupMember(CloseTPTriggerPrice,(int)((char *)&CloseTPTriggerPrice-(char *)this),"CloseTPTriggerPrice",sizeof(CloseTPTriggerPrice),"CloseTPTriggerPrice","CloseTPTriggerPrice",15);
		m_Describe.SetupMember(CloseOrderPriceType,(int)((char *)&CloseOrderPriceType-(char *)this),"CloseOrderPriceType",sizeof(CloseOrderPriceType),"CloseOrderPriceType","CloseOrderPriceType",0);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(CloseTriggerPrice,(int)((char *)&CloseTriggerPrice-(char *)this),"CloseTriggerPrice",sizeof(CloseTriggerPrice),"CloseTriggerPrice","CloseTriggerPrice",15);
		m_Describe.SetupMember(RelatedOrderID,(int)((char *)&RelatedOrderID-(char *)this),"RelatedOrderID",sizeof(RelatedOrderID),"RelatedOrderID","RelatedOrderID",0);
		m_Describe.SetupMember(ActiveTime,(int)((char *)&ActiveTime-(char *)this),"ActiveTime",sizeof(ActiveTime),"ActiveTime","ActiveTime",3);
		m_Describe.SetupMember(TriggerTime,(int)((char *)&TriggerTime-(char *)this),"TriggerTime",sizeof(TriggerTime),"TriggerTime","TriggerTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_TriggerOrderInsert=0x2109;

///输入止盈止损报单
class CCloseOrderInsertField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///资金账号
	CAccountIDType	AccountID;
	///报单本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单价格类型
	COrderPriceTypeType	OrderPriceType;
	///买卖方向
	CDirectionType	Direction;
	///开平标志
	COffsetFlagType	OffsetFlag;
	///报单价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///显示数量
	CVolumeType	VolumeDisplay;
	///数量取值方式
	CValueModeType	VolumeMode;
	///委托额
	CMoneyType	Cost;
	///订单类型
	COrderTypeType	OrderType;
	///GTD时间
	CMilliSecondsType	GTDTime;
	///最小成交量
	CVolumeType	MinVolume;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType	CloseOrderID;
	///是否全仓
	CBoolType	IsCrossMargin;
	///备注
	CNameType	Remark;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单员报单号
	CIndexIDType	CopyOrderID;
	///委托单杠杆倍数
	CLeverageType	Leverage;
	///带单分配比例
	CRatioType	CopyProfitRate;
	///应用编号
	CShortIndexIDType	APPID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///触发价
	CPriceType	TriggerPrice;
	///保留资金
	CMoneyType	Reserve;
	///平仓止损价
	CPriceType	CloseSLPrice;
	///平仓止损触发价
	CPriceType	SLTriggerPrice;
	///平仓止损触发价
	CPriceType	CloseSLTriggerPrice;
	///平仓止盈价
	CPriceType	CloseTPPrice;
	///平仓止盈触发价
	CPriceType	TPTriggerPrice;
	///平仓止盈触发价
	CPriceType	CloseTPTriggerPrice;
	///报单价格类型
	COrderPriceTypeType	CloseOrderPriceType;
	///平仓价
	CPriceType	ClosePrice;
	///平仓触发价
	CPriceType	CloseTriggerPrice;
	///触发价类型
	CTriggerPriceTypeType	CloseTriggerPriceType;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OrderPriceType,(int)((char *)&OrderPriceType-(char *)this),"OrderPriceType",sizeof(OrderPriceType),"OrderPriceType","OrderPriceType",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(OffsetFlag,(int)((char *)&OffsetFlag-(char *)this),"OffsetFlag",sizeof(OffsetFlag),"OffsetFlag","OffsetFlag",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(VolumeMode,(int)((char *)&VolumeMode-(char *)this),"VolumeMode",sizeof(VolumeMode),"VolumeMode","VolumeMode",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(OrderType,(int)((char *)&OrderType-(char *)this),"OrderType",sizeof(OrderType),"OrderType","OrderType",0);
		m_Describe.SetupMember(GTDTime,(int)((char *)&GTDTime-(char *)this),"GTDTime",sizeof(GTDTime),"GTDTime","GTDTime",3);
		m_Describe.SetupMember(MinVolume,(int)((char *)&MinVolume-(char *)this),"MinVolume",sizeof(MinVolume),"MinVolume","MinVolume",10);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
		m_Describe.SetupMember(CloseOrderID,(int)((char *)&CloseOrderID-(char *)this),"CloseOrderID",sizeof(CloseOrderID),"CloseOrderID","CloseOrderID",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyOrderID,(int)((char *)&CopyOrderID-(char *)this),"CopyOrderID",sizeof(CopyOrderID),"CopyOrderID","CopyOrderID",0);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(CopyProfitRate,(int)((char *)&CopyProfitRate-(char *)this),"CopyProfitRate",sizeof(CopyProfitRate),"CopyProfitRate","CopyProfitRate",10);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
		m_Describe.SetupMember(CloseSLPrice,(int)((char *)&CloseSLPrice-(char *)this),"CloseSLPrice",sizeof(CloseSLPrice),"CloseSLPrice","CloseSLPrice",15);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(CloseSLTriggerPrice,(int)((char *)&CloseSLTriggerPrice-(char *)this),"CloseSLTriggerPrice",sizeof(CloseSLTriggerPrice),"CloseSLTriggerPrice","CloseSLTriggerPrice",15);
		m_Describe.SetupMember(CloseTPPrice,(int)((char *)&CloseTPPrice-(char *)this),"CloseTPPrice",sizeof(CloseTPPrice),"CloseTPPrice","CloseTPPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(CloseTPTriggerPrice,(int)((char *)&CloseTPTriggerPrice-(char *)this),"CloseTPTriggerPrice",sizeof(CloseTPTriggerPrice),"CloseTPTriggerPrice","CloseTPTriggerPrice",15);
		m_Describe.SetupMember(CloseOrderPriceType,(int)((char *)&CloseOrderPriceType-(char *)this),"CloseOrderPriceType",sizeof(CloseOrderPriceType),"CloseOrderPriceType","CloseOrderPriceType",0);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(CloseTriggerPrice,(int)((char *)&CloseTriggerPrice-(char *)this),"CloseTriggerPrice",sizeof(CloseTriggerPrice),"CloseTriggerPrice","CloseTriggerPrice",15);
		m_Describe.SetupMember(CloseTriggerPriceType,(int)((char *)&CloseTriggerPriceType-(char *)this),"CloseTriggerPriceType",sizeof(CloseTriggerPriceType),"CloseTriggerPriceType","CloseTriggerPriceType",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_CloseOrderInsert=0x210A;

///触发单操作
class CTriggerOrderActionField
{
public:
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///报单本地标识
	CIndexIDType	OrderLocalID;
	///报单操作标志
	CActionFlagType	ActionFlag;
	///操作本地标识
	CIndexIDType	LocalID;
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///价格
	CPriceType	Price;
	///数量变化
	CVolumeType	Volume;
	///买卖方向
	CDirectionType	Direction;
	///显示数量
	CVolumeType	VolumeDisplay;
	///产品组
	CShortIndexIDType	ProductGroup;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///委托额
	CMoneyType	Cost;
	///备注
	CNameType	Remark;
	///止损价
	CPriceType	SLPrice;
	///止损触发价
	CPriceType	SLTriggerPrice;
	///止盈价
	CPriceType	TPPrice;
	///止盈触发价
	CPriceType	TPTriggerPrice;
	///触发价
	CPriceType	TriggerPrice;
	///触发的订单类型
	CShortIndexIDType	TriggerOrderType;
	///触发单具体设置信息
	CLongContentType	TriggerValue;
	///触发类型明细
	CIndexIDType	TriggerDetail;
	///触发价类型
	CTriggerPriceTypeType	TriggerPriceType;
	///错误代码
	CNumberType	ErrorNo;
	///错误信息
	CShortContentType	ErrorMsg;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(OrderLocalID,(int)((char *)&OrderLocalID-(char *)this),"OrderLocalID",sizeof(OrderLocalID),"OrderLocalID","OrderLocalID",0);
		m_Describe.SetupMember(ActionFlag,(int)((char *)&ActionFlag-(char *)this),"ActionFlag",sizeof(ActionFlag),"ActionFlag","ActionFlag",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(SLPrice,(int)((char *)&SLPrice-(char *)this),"SLPrice",sizeof(SLPrice),"SLPrice","SLPrice",15);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(TPPrice,(int)((char *)&TPPrice-(char *)this),"TPPrice",sizeof(TPPrice),"TPPrice","TPPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(TriggerOrderType,(int)((char *)&TriggerOrderType-(char *)this),"TriggerOrderType",sizeof(TriggerOrderType),"TriggerOrderType","TriggerOrderType",0);
		m_Describe.SetupMember(TriggerValue,(int)((char *)&TriggerValue-(char *)this),"TriggerValue",sizeof(TriggerValue),"TriggerValue","TriggerValue",0);
		m_Describe.SetupMember(TriggerDetail,(int)((char *)&TriggerDetail-(char *)this),"TriggerDetail",sizeof(TriggerDetail),"TriggerDetail","TriggerDetail",0);
		m_Describe.SetupMember(TriggerPriceType,(int)((char *)&TriggerPriceType-(char *)this),"TriggerPriceType",sizeof(TriggerPriceType),"TriggerPriceType","TriggerPriceType",0);
		m_Describe.SetupMember(ErrorNo,(int)((char *)&ErrorNo-(char *)this),"ErrorNo",sizeof(ErrorNo),"ErrorNo","ErrorNo",0);
		m_Describe.SetupMember(ErrorMsg,(int)((char *)&ErrorMsg-(char *)this),"ErrorMsg",sizeof(ErrorMsg),"ErrorMsg","ErrorMsg",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_TriggerOrderAction=0x211C;

///输入报单
class COrderInsertField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///资金账号
	CAccountIDType	AccountID;
	///报单本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单价格类型
	COrderPriceTypeType	OrderPriceType;
	///买卖方向
	CDirectionType	Direction;
	///开平标志
	COffsetFlagType	OffsetFlag;
	///报单价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///显示数量
	CVolumeType	VolumeDisplay;
	///数量取值方式
	CValueModeType	VolumeMode;
	///委托额
	CMoneyType	Cost;
	///订单类型
	COrderTypeType	OrderType;
	///GTD时间
	CMilliSecondsType	GTDTime;
	///最小成交量
	CVolumeType	MinVolume;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType	CloseOrderID;
	///是否全仓
	CBoolType	IsCrossMargin;
	///备注
	CNameType	Remark;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单员报单号
	CIndexIDType	CopyOrderID;
	///委托单杠杆倍数
	CLeverageType	Leverage;
	///带单分配比例
	CRatioType	CopyProfitRate;
	///应用编号
	CShortIndexIDType	APPID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///触发价
	CPriceType	TriggerPrice;
	///保留资金
	CMoneyType	Reserve;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OrderPriceType,(int)((char *)&OrderPriceType-(char *)this),"OrderPriceType",sizeof(OrderPriceType),"OrderPriceType","OrderPriceType",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(OffsetFlag,(int)((char *)&OffsetFlag-(char *)this),"OffsetFlag",sizeof(OffsetFlag),"OffsetFlag","OffsetFlag",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(VolumeMode,(int)((char *)&VolumeMode-(char *)this),"VolumeMode",sizeof(VolumeMode),"VolumeMode","VolumeMode",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(OrderType,(int)((char *)&OrderType-(char *)this),"OrderType",sizeof(OrderType),"OrderType","OrderType",0);
		m_Describe.SetupMember(GTDTime,(int)((char *)&GTDTime-(char *)this),"GTDTime",sizeof(GTDTime),"GTDTime","GTDTime",3);
		m_Describe.SetupMember(MinVolume,(int)((char *)&MinVolume-(char *)this),"MinVolume",sizeof(MinVolume),"MinVolume","MinVolume",10);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
		m_Describe.SetupMember(CloseOrderID,(int)((char *)&CloseOrderID-(char *)this),"CloseOrderID",sizeof(CloseOrderID),"CloseOrderID","CloseOrderID",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyOrderID,(int)((char *)&CopyOrderID-(char *)this),"CopyOrderID",sizeof(CopyOrderID),"CopyOrderID","CopyOrderID",0);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(CopyProfitRate,(int)((char *)&CopyProfitRate-(char *)this),"CopyProfitRate",sizeof(CopyProfitRate),"CopyProfitRate","CopyProfitRate",10);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_OrderInsert=0x2110;

///报单操作
class COrderActionField
{
public:
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///报单本地标识
	CIndexIDType	OrderLocalID;
	///报单操作标志
	CActionFlagType	ActionFlag;
	///操作本地标识
	CIndexIDType	LocalID;
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///价格
	CPriceType	Price;
	///数量变化
	CVolumeType	Volume;
	///买卖方向
	CDirectionType	Direction;
	///显示数量
	CVolumeType	VolumeDisplay;
	///产品组
	CShortIndexIDType	ProductGroup;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///委托额
	CMoneyType	Cost;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(OrderLocalID,(int)((char *)&OrderLocalID-(char *)this),"OrderLocalID",sizeof(OrderLocalID),"OrderLocalID","OrderLocalID",0);
		m_Describe.SetupMember(ActionFlag,(int)((char *)&ActionFlag-(char *)this),"ActionFlag",sizeof(ActionFlag),"ActionFlag","ActionFlag",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_OrderAction=0x2111;

///输入报价
class CQuoteInsertField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///报价本地标识
	CIndexIDType	LocalID;
	///数量
	CVolumeType	Volume;
	///买方组合开平标志
	COffsetFlagType	BuyOffsetFlag;
	///买方价格
	CPriceType	BuyPrice;
	///卖方组合开平标志
	COffsetFlagType	SellOffsetFlag;
	///卖方价格
	CPriceType	SellPrice;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(BuyOffsetFlag,(int)((char *)&BuyOffsetFlag-(char *)this),"BuyOffsetFlag",sizeof(BuyOffsetFlag),"BuyOffsetFlag","BuyOffsetFlag",0);
		m_Describe.SetupMember(BuyPrice,(int)((char *)&BuyPrice-(char *)this),"BuyPrice",sizeof(BuyPrice),"BuyPrice","BuyPrice",15);
		m_Describe.SetupMember(SellOffsetFlag,(int)((char *)&SellOffsetFlag-(char *)this),"SellOffsetFlag",sizeof(SellOffsetFlag),"SellOffsetFlag","SellOffsetFlag",0);
		m_Describe.SetupMember(SellPrice,(int)((char *)&SellPrice-(char *)this),"SellPrice",sizeof(SellPrice),"SellPrice","SellPrice",15);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QuoteInsert=0x2112;

///报价操作
class CQuoteActionField
{
public:
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///报单本地标识
	CIndexIDType	OrderLocalID;
	///报单操作标志
	CActionFlagType	ActionFlag;
	///操作本地标识
	CIndexIDType	LocalID;
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///价格
	CPriceType	Price;
	///数量变化
	CVolumeType	Volume;
	///买卖方向
	CDirectionType	Direction;
	///显示数量
	CVolumeType	VolumeDisplay;
	///产品组
	CShortIndexIDType	ProductGroup;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///委托额
	CMoneyType	Cost;
	///备注
	CNameType	Remark;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(OrderLocalID,(int)((char *)&OrderLocalID-(char *)this),"OrderLocalID",sizeof(OrderLocalID),"OrderLocalID","OrderLocalID",0);
		m_Describe.SetupMember(ActionFlag,(int)((char *)&ActionFlag-(char *)this),"ActionFlag",sizeof(ActionFlag),"ActionFlag","ActionFlag",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QuoteAction=0x2113;

///输入场外成交对
class COTCTradeInsertField
{
public:
	///成交对本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///买方成员代码
	CLongIndexIDType	BuyMemberID;
	///买方交易单元代码
	CAccountIDType	BuyAccountID;
	///买方交易单元代码
	CIndexIDType	BuyTradeUnitID;
	///买方组合开平标志
	COffsetFlagType	BuyOffsetFlag;
	///卖方方成员代码
	CLongIndexIDType	SellMemberID;
	///卖方交易单元代码
	CAccountIDType	SellAccountID;
	///卖方交易单元代码
	CIndexIDType	SellTradeUnitID;
	///卖方组合开平标志
	COffsetFlagType	SellOffsetFlag;
	///OTC类型
	COTCTypeType	OTCType;
	///Taker的方向
	CDirectionType	TakerDirection;
	///触发价
	CPriceType	TriggerPrice;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(BuyMemberID,(int)((char *)&BuyMemberID-(char *)this),"BuyMemberID",sizeof(BuyMemberID),"BuyMemberID","BuyMemberID",0);
		m_Describe.SetupMember(BuyAccountID,(int)((char *)&BuyAccountID-(char *)this),"BuyAccountID",sizeof(BuyAccountID),"BuyAccountID","BuyAccountID",0);
		m_Describe.SetupMember(BuyTradeUnitID,(int)((char *)&BuyTradeUnitID-(char *)this),"BuyTradeUnitID",sizeof(BuyTradeUnitID),"BuyTradeUnitID","BuyTradeUnitID",0);
		m_Describe.SetupMember(BuyOffsetFlag,(int)((char *)&BuyOffsetFlag-(char *)this),"BuyOffsetFlag",sizeof(BuyOffsetFlag),"BuyOffsetFlag","BuyOffsetFlag",0);
		m_Describe.SetupMember(SellMemberID,(int)((char *)&SellMemberID-(char *)this),"SellMemberID",sizeof(SellMemberID),"SellMemberID","SellMemberID",0);
		m_Describe.SetupMember(SellAccountID,(int)((char *)&SellAccountID-(char *)this),"SellAccountID",sizeof(SellAccountID),"SellAccountID","SellAccountID",0);
		m_Describe.SetupMember(SellTradeUnitID,(int)((char *)&SellTradeUnitID-(char *)this),"SellTradeUnitID",sizeof(SellTradeUnitID),"SellTradeUnitID","SellTradeUnitID",0);
		m_Describe.SetupMember(SellOffsetFlag,(int)((char *)&SellOffsetFlag-(char *)this),"SellOffsetFlag",sizeof(SellOffsetFlag),"SellOffsetFlag","SellOffsetFlag",0);
		m_Describe.SetupMember(OTCType,(int)((char *)&OTCType-(char *)this),"OTCType",sizeof(OTCType),"OTCType","OTCType",0);
		m_Describe.SetupMember(TakerDirection,(int)((char *)&TakerDirection-(char *)this),"TakerDirection",sizeof(TakerDirection),"TakerDirection","TakerDirection",0);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_OTCTradeInsert=0x2115;

///输入场外成交对
class COTCTradeField
{
public:
	///成交对本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///买方成员代码
	CLongIndexIDType	BuyMemberID;
	///买方交易单元代码
	CAccountIDType	BuyAccountID;
	///买方交易单元代码
	CIndexIDType	BuyTradeUnitID;
	///买方组合开平标志
	COffsetFlagType	BuyOffsetFlag;
	///卖方方成员代码
	CLongIndexIDType	SellMemberID;
	///卖方交易单元代码
	CAccountIDType	SellAccountID;
	///卖方交易单元代码
	CIndexIDType	SellTradeUnitID;
	///卖方组合开平标志
	COffsetFlagType	SellOffsetFlag;
	///OTC类型
	COTCTypeType	OTCType;
	///Taker的方向
	CDirectionType	TakerDirection;
	///触发价
	CPriceType	TriggerPrice;
	///备注
	CNameType	Remark;
	///成交代码
	CIndexIDType	TradeID;
	///成交时间
	CMilliSecondsType	TradeTime;
	///插入时间
	CMilliSecondsType	InsertTime;
	///成交类型
	CDeriveSourceType	DeriveSource;
	///买方报单系统唯一代码
	CIndexIDType	BuyOrderID;
	///卖方报单系统唯一代码
	CIndexIDType	SellOrderID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(BuyMemberID,(int)((char *)&BuyMemberID-(char *)this),"BuyMemberID",sizeof(BuyMemberID),"BuyMemberID","BuyMemberID",0);
		m_Describe.SetupMember(BuyAccountID,(int)((char *)&BuyAccountID-(char *)this),"BuyAccountID",sizeof(BuyAccountID),"BuyAccountID","BuyAccountID",0);
		m_Describe.SetupMember(BuyTradeUnitID,(int)((char *)&BuyTradeUnitID-(char *)this),"BuyTradeUnitID",sizeof(BuyTradeUnitID),"BuyTradeUnitID","BuyTradeUnitID",0);
		m_Describe.SetupMember(BuyOffsetFlag,(int)((char *)&BuyOffsetFlag-(char *)this),"BuyOffsetFlag",sizeof(BuyOffsetFlag),"BuyOffsetFlag","BuyOffsetFlag",0);
		m_Describe.SetupMember(SellMemberID,(int)((char *)&SellMemberID-(char *)this),"SellMemberID",sizeof(SellMemberID),"SellMemberID","SellMemberID",0);
		m_Describe.SetupMember(SellAccountID,(int)((char *)&SellAccountID-(char *)this),"SellAccountID",sizeof(SellAccountID),"SellAccountID","SellAccountID",0);
		m_Describe.SetupMember(SellTradeUnitID,(int)((char *)&SellTradeUnitID-(char *)this),"SellTradeUnitID",sizeof(SellTradeUnitID),"SellTradeUnitID","SellTradeUnitID",0);
		m_Describe.SetupMember(SellOffsetFlag,(int)((char *)&SellOffsetFlag-(char *)this),"SellOffsetFlag",sizeof(SellOffsetFlag),"SellOffsetFlag","SellOffsetFlag",0);
		m_Describe.SetupMember(OTCType,(int)((char *)&OTCType-(char *)this),"OTCType",sizeof(OTCType),"OTCType","OTCType",0);
		m_Describe.SetupMember(TakerDirection,(int)((char *)&TakerDirection-(char *)this),"TakerDirection",sizeof(TakerDirection),"TakerDirection","TakerDirection",0);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(TradeID,(int)((char *)&TradeID-(char *)this),"TradeID",sizeof(TradeID),"TradeID","TradeID",0);
		m_Describe.SetupMember(TradeTime,(int)((char *)&TradeTime-(char *)this),"TradeTime",sizeof(TradeTime),"TradeTime","TradeTime",3);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(DeriveSource,(int)((char *)&DeriveSource-(char *)this),"DeriveSource",sizeof(DeriveSource),"DeriveSource","DeriveSource",0);
		m_Describe.SetupMember(BuyOrderID,(int)((char *)&BuyOrderID-(char *)this),"BuyOrderID",sizeof(BuyOrderID),"BuyOrderID","BuyOrderID",0);
		m_Describe.SetupMember(SellOrderID,(int)((char *)&SellOrderID-(char *)this),"SellOrderID",sizeof(SellOrderID),"SellOrderID","SellOrderID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_OTCTrade=0x2116;

///内部做市商报单
class CRobotOrderField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///资金账号
	CAccountIDType	AccountID;
	///交易用户代码
	CLongIndexIDType	UserID;
	///报单本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单价格类型
	COrderPriceTypeType	OrderPriceType;
	///买卖方向
	CDirectionType	Direction;
	///开平标志
	COffsetFlagType	OffsetFlag;
	///报单价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///显示数量
	CVolumeType	VolumeDisplay;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OrderPriceType,(int)((char *)&OrderPriceType-(char *)this),"OrderPriceType",sizeof(OrderPriceType),"OrderPriceType","OrderPriceType",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(OffsetFlag,(int)((char *)&OffsetFlag-(char *)this),"OffsetFlag",sizeof(OffsetFlag),"OffsetFlag","OffsetFlag",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_RobotOrder=0x2117;

///仓位操作
class CPositionActionField
{
public:
	///出入金本地号
	CIndexIDType	LocalID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///产品组
	CShortIndexIDType	ProductGroup;
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///资金账号
	CAccountIDType	AccountID;
	///持仓多空方向，0:多;1:空;2:净;空值:多空方向同时操作
	CPosiDirectionType	PosiDirection;
	///发生额
	CMoneyType	Amount;
	///杠杆倍数
	CLeverageType	LongLeverage;
	///杠杆倍数
	CLeverageType	ShortLeverage;
	///杠杆倍数
	CLeverageType	NetLeverage;
	///操作类型
	CPositionActionTypeType	ActionType;
	///是否全仓
	CBoolType	IsCrossMargin;
	///是否自动追加保证金
	CBoolType	IsAutoAddMargin;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
		m_Describe.SetupMember(Amount,(int)((char *)&Amount-(char *)this),"Amount",sizeof(Amount),"Amount","Amount",10);
		m_Describe.SetupMember(LongLeverage,(int)((char *)&LongLeverage-(char *)this),"LongLeverage",sizeof(LongLeverage),"LongLeverage","LongLeverage",8);
		m_Describe.SetupMember(ShortLeverage,(int)((char *)&ShortLeverage-(char *)this),"ShortLeverage",sizeof(ShortLeverage),"ShortLeverage","ShortLeverage",8);
		m_Describe.SetupMember(NetLeverage,(int)((char *)&NetLeverage-(char *)this),"NetLeverage",sizeof(NetLeverage),"NetLeverage","NetLeverage",8);
		m_Describe.SetupMember(ActionType,(int)((char *)&ActionType-(char *)this),"ActionType",sizeof(ActionType),"ActionType","ActionType",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(IsAutoAddMargin,(int)((char *)&IsAutoAddMargin-(char *)this),"IsAutoAddMargin",sizeof(IsAutoAddMargin),"IsAutoAddMargin","IsAutoAddMargin",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_PositionAction=0x2118;

///资金账户出入金
class CAccountActionField
{
public:
	///关联的之前的资金操作本地号
	CIndexIDType	ActionLocalID;
	///出入金本地号
	CIndexIDType	LocalID;
	///关联订单号
	CIndexIDType	OrderID;
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///内部转账资金账号
	CAccountIDType	OtherAccountID;
	///内部转账结算组编号
	CShortIndexIDType	OtherSettlementGroup;
	///内部转账成员代码
	CLongIndexIDType	OtherMemberID;
	///发生额
	CMoneyType	Amount;
	///冻结资金
	CMoneyType	FrozenMoney;
	///币种
	CCurrencyType	Currency;
	///资金操作类型
	CAccountActionTypeType	ActionType;
	///是否保留资金
	CBoolType	IsReserve;
	///账户类型
	CIndexIDType	AccountType;
	///保证金等级
	CShortIndexIDType	MarginRateGrade;
	///权限等级
	CShortIndexIDType	TradingRightGrade;
	///价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///财务流水类型
	CDefineTypeType	Source;
	///内外对账ID
	CLongNameType	RelatedID;
	///作为手续费的抵扣比例
	CRatioType	FeeDeduceRate;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ActionLocalID,(int)((char *)&ActionLocalID-(char *)this),"ActionLocalID",sizeof(ActionLocalID),"ActionLocalID","ActionLocalID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(OtherAccountID,(int)((char *)&OtherAccountID-(char *)this),"OtherAccountID",sizeof(OtherAccountID),"OtherAccountID","OtherAccountID",0);
		m_Describe.SetupMember(OtherSettlementGroup,(int)((char *)&OtherSettlementGroup-(char *)this),"OtherSettlementGroup",sizeof(OtherSettlementGroup),"OtherSettlementGroup","OtherSettlementGroup",0);
		m_Describe.SetupMember(OtherMemberID,(int)((char *)&OtherMemberID-(char *)this),"OtherMemberID",sizeof(OtherMemberID),"OtherMemberID","OtherMemberID",0);
		m_Describe.SetupMember(Amount,(int)((char *)&Amount-(char *)this),"Amount",sizeof(Amount),"Amount","Amount",10);
		m_Describe.SetupMember(FrozenMoney,(int)((char *)&FrozenMoney-(char *)this),"FrozenMoney",sizeof(FrozenMoney),"FrozenMoney","FrozenMoney",10);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
		m_Describe.SetupMember(ActionType,(int)((char *)&ActionType-(char *)this),"ActionType",sizeof(ActionType),"ActionType","ActionType",0);
		m_Describe.SetupMember(IsReserve,(int)((char *)&IsReserve-(char *)this),"IsReserve",sizeof(IsReserve),"IsReserve","IsReserve",0);
		m_Describe.SetupMember(AccountType,(int)((char *)&AccountType-(char *)this),"AccountType",sizeof(AccountType),"AccountType","AccountType",0);
		m_Describe.SetupMember(MarginRateGrade,(int)((char *)&MarginRateGrade-(char *)this),"MarginRateGrade",sizeof(MarginRateGrade),"MarginRateGrade","MarginRateGrade",0);
		m_Describe.SetupMember(TradingRightGrade,(int)((char *)&TradingRightGrade-(char *)this),"TradingRightGrade",sizeof(TradingRightGrade),"TradingRightGrade","TradingRightGrade",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Source,(int)((char *)&Source-(char *)this),"Source",sizeof(Source),"Source","Source",0);
		m_Describe.SetupMember(RelatedID,(int)((char *)&RelatedID-(char *)this),"RelatedID",sizeof(RelatedID),"RelatedID","RelatedID",0);
		m_Describe.SetupMember(FeeDeduceRate,(int)((char *)&FeeDeduceRate-(char *)this),"FeeDeduceRate",sizeof(FeeDeduceRate),"FeeDeduceRate","FeeDeduceRate",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_AccountAction=0x2120;

///资金账户出入金
class CMemberActionField
{
public:
	///出入金本地号
	CIndexIDType	LocalID;
	///成员代码
	CLongIndexIDType	MemberID;
	///风控类型
	CRiskLevelType	RiskLevel;
	///持仓类型
	CPositionTypeType	PositionType;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(RiskLevel,(int)((char *)&RiskLevel-(char *)this),"RiskLevel",sizeof(RiskLevel),"RiskLevel","RiskLevel",0);
		m_Describe.SetupMember(PositionType,(int)((char *)&PositionType-(char *)this),"PositionType",sizeof(PositionType),"PositionType","PositionType",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_MemberAction=0x2122;

///行情概要
class CMarketOverViewField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///产品组
	CShortIndexIDType	ProductGroup;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///最高限价
	CPriceType	UpperLimitPrice;
	///最低限价
	CPriceType	LowerLimitPrice;
	///基础标的价格
	CPriceType	UnderlyingPrice;
	///标记价格
	CPriceType	MarkedPrice;
	///预计持仓费按比例
	CRatioType	PositionFeeRate;
	///当日最高价
	CPriceType	HighestPrice;
	///当日最低价
	CPriceType	LowestPrice;
	///最新价
	CPriceType	LastPrice;
	///当日数量
	CCollectType	Volume;
	///当日成交金额
	CCollectType	Turnover;
	///持仓量
	CVolumeType	OpenInterest;
	///预估结算价
	CPriceType	SettlementPrice;
	///当日开盘价
	CPriceType	OpenPrice;
	///标的交易状态
	CInstrumentStatusType	InstrumentStatus;
	///上次持仓费按比例
	CRatioType	PrePositionFeeRate;
	///收取持仓费用时间
	CMilliSecondsType	PositionFeeTime;
	///申买价一
	CPriceType	BidPrice1;
	///申卖价一
	CPriceType	AskPrice1;
	///24小时最高价
	CPriceType	HighestPrice24;
	///24小时最低价
	CPriceType	LowestPrice24;
	///24小时数量
	CCollectType	Volume24;
	///24小时成交金额
	CCollectType	Turnover24;
	///24小时开盘价
	CPriceType	OpenPrice24;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(UpperLimitPrice,(int)((char *)&UpperLimitPrice-(char *)this),"UpperLimitPrice",sizeof(UpperLimitPrice),"UpperLimitPrice","UpperLimitPrice",15);
		m_Describe.SetupMember(LowerLimitPrice,(int)((char *)&LowerLimitPrice-(char *)this),"LowerLimitPrice",sizeof(LowerLimitPrice),"LowerLimitPrice","LowerLimitPrice",15);
		m_Describe.SetupMember(UnderlyingPrice,(int)((char *)&UnderlyingPrice-(char *)this),"UnderlyingPrice",sizeof(UnderlyingPrice),"UnderlyingPrice","UnderlyingPrice",15);
		m_Describe.SetupMember(MarkedPrice,(int)((char *)&MarkedPrice-(char *)this),"MarkedPrice",sizeof(MarkedPrice),"MarkedPrice","MarkedPrice",15);
		m_Describe.SetupMember(PositionFeeRate,(int)((char *)&PositionFeeRate-(char *)this),"PositionFeeRate",sizeof(PositionFeeRate),"PositionFeeRate","PositionFeeRate",10);
		m_Describe.SetupMember(HighestPrice,(int)((char *)&HighestPrice-(char *)this),"HighestPrice",sizeof(HighestPrice),"HighestPrice","HighestPrice",15);
		m_Describe.SetupMember(LowestPrice,(int)((char *)&LowestPrice-(char *)this),"LowestPrice",sizeof(LowestPrice),"LowestPrice","LowestPrice",15);
		m_Describe.SetupMember(LastPrice,(int)((char *)&LastPrice-(char *)this),"LastPrice",sizeof(LastPrice),"LastPrice","LastPrice",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",8);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",8);
		m_Describe.SetupMember(OpenInterest,(int)((char *)&OpenInterest-(char *)this),"OpenInterest",sizeof(OpenInterest),"OpenInterest","OpenInterest",10);
		m_Describe.SetupMember(SettlementPrice,(int)((char *)&SettlementPrice-(char *)this),"SettlementPrice",sizeof(SettlementPrice),"SettlementPrice","SettlementPrice",15);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(InstrumentStatus,(int)((char *)&InstrumentStatus-(char *)this),"InstrumentStatus",sizeof(InstrumentStatus),"InstrumentStatus","InstrumentStatus",0);
		m_Describe.SetupMember(PrePositionFeeRate,(int)((char *)&PrePositionFeeRate-(char *)this),"PrePositionFeeRate",sizeof(PrePositionFeeRate),"PrePositionFeeRate","PrePositionFeeRate",10);
		m_Describe.SetupMember(PositionFeeTime,(int)((char *)&PositionFeeTime-(char *)this),"PositionFeeTime",sizeof(PositionFeeTime),"PositionFeeTime","PositionFeeTime",3);
		m_Describe.SetupMember(BidPrice1,(int)((char *)&BidPrice1-(char *)this),"BidPrice1",sizeof(BidPrice1),"BidPrice1","BidPrice1",15);
		m_Describe.SetupMember(AskPrice1,(int)((char *)&AskPrice1-(char *)this),"AskPrice1",sizeof(AskPrice1),"AskPrice1","AskPrice1",15);
		m_Describe.SetupMember(HighestPrice24,(int)((char *)&HighestPrice24-(char *)this),"HighestPrice24",sizeof(HighestPrice24),"HighestPrice24","HighestPrice24",15);
		m_Describe.SetupMember(LowestPrice24,(int)((char *)&LowestPrice24-(char *)this),"LowestPrice24",sizeof(LowestPrice24),"LowestPrice24","LowestPrice24",15);
		m_Describe.SetupMember(Volume24,(int)((char *)&Volume24-(char *)this),"Volume24",sizeof(Volume24),"Volume24","Volume24",8);
		m_Describe.SetupMember(Turnover24,(int)((char *)&Turnover24-(char *)this),"Turnover24",sizeof(Turnover24),"Turnover24","Turnover24",8);
		m_Describe.SetupMember(OpenPrice24,(int)((char *)&OpenPrice24-(char *)this),"OpenPrice24",sizeof(OpenPrice24),"OpenPrice24","OpenPrice24",15);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_MarketOverView=0x2125;

///5档深度行情
class CFiveDepthMarketDataField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///产品组
	CShortIndexIDType	ProductGroup;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///申买价一
	CPriceType	BidPrice1;
	///申买量一
	CVolumeType	BidVolume1;
	///申卖价一
	CPriceType	AskPrice1;
	///申卖量一
	CVolumeType	AskVolume1;
	///申买价二
	CPriceType	BidPrice2;
	///申买量二
	CVolumeType	BidVolume2;
	///申卖价二
	CPriceType	AskPrice2;
	///申卖量二
	CVolumeType	AskVolume2;
	///申买价三
	CPriceType	BidPrice3;
	///申买量三
	CVolumeType	BidVolume3;
	///申卖价三
	CPriceType	AskPrice3;
	///申卖量三
	CVolumeType	AskVolume3;
	///申买价四
	CPriceType	BidPrice4;
	///申买量四
	CVolumeType	BidVolume4;
	///申卖价四
	CPriceType	AskPrice4;
	///申卖量四
	CVolumeType	AskVolume4;
	///申买价五
	CPriceType	BidPrice5;
	///申买量五
	CVolumeType	BidVolume5;
	///申卖价五
	CPriceType	AskPrice5;
	///申卖量五
	CVolumeType	AskVolume5;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(BidPrice1,(int)((char *)&BidPrice1-(char *)this),"BidPrice1",sizeof(BidPrice1),"BidPrice1","BidPrice1",15);
		m_Describe.SetupMember(BidVolume1,(int)((char *)&BidVolume1-(char *)this),"BidVolume1",sizeof(BidVolume1),"BidVolume1","BidVolume1",10);
		m_Describe.SetupMember(AskPrice1,(int)((char *)&AskPrice1-(char *)this),"AskPrice1",sizeof(AskPrice1),"AskPrice1","AskPrice1",15);
		m_Describe.SetupMember(AskVolume1,(int)((char *)&AskVolume1-(char *)this),"AskVolume1",sizeof(AskVolume1),"AskVolume1","AskVolume1",10);
		m_Describe.SetupMember(BidPrice2,(int)((char *)&BidPrice2-(char *)this),"BidPrice2",sizeof(BidPrice2),"BidPrice2","BidPrice2",15);
		m_Describe.SetupMember(BidVolume2,(int)((char *)&BidVolume2-(char *)this),"BidVolume2",sizeof(BidVolume2),"BidVolume2","BidVolume2",10);
		m_Describe.SetupMember(AskPrice2,(int)((char *)&AskPrice2-(char *)this),"AskPrice2",sizeof(AskPrice2),"AskPrice2","AskPrice2",15);
		m_Describe.SetupMember(AskVolume2,(int)((char *)&AskVolume2-(char *)this),"AskVolume2",sizeof(AskVolume2),"AskVolume2","AskVolume2",10);
		m_Describe.SetupMember(BidPrice3,(int)((char *)&BidPrice3-(char *)this),"BidPrice3",sizeof(BidPrice3),"BidPrice3","BidPrice3",15);
		m_Describe.SetupMember(BidVolume3,(int)((char *)&BidVolume3-(char *)this),"BidVolume3",sizeof(BidVolume3),"BidVolume3","BidVolume3",10);
		m_Describe.SetupMember(AskPrice3,(int)((char *)&AskPrice3-(char *)this),"AskPrice3",sizeof(AskPrice3),"AskPrice3","AskPrice3",15);
		m_Describe.SetupMember(AskVolume3,(int)((char *)&AskVolume3-(char *)this),"AskVolume3",sizeof(AskVolume3),"AskVolume3","AskVolume3",10);
		m_Describe.SetupMember(BidPrice4,(int)((char *)&BidPrice4-(char *)this),"BidPrice4",sizeof(BidPrice4),"BidPrice4","BidPrice4",15);
		m_Describe.SetupMember(BidVolume4,(int)((char *)&BidVolume4-(char *)this),"BidVolume4",sizeof(BidVolume4),"BidVolume4","BidVolume4",10);
		m_Describe.SetupMember(AskPrice4,(int)((char *)&AskPrice4-(char *)this),"AskPrice4",sizeof(AskPrice4),"AskPrice4","AskPrice4",15);
		m_Describe.SetupMember(AskVolume4,(int)((char *)&AskVolume4-(char *)this),"AskVolume4",sizeof(AskVolume4),"AskVolume4","AskVolume4",10);
		m_Describe.SetupMember(BidPrice5,(int)((char *)&BidPrice5-(char *)this),"BidPrice5",sizeof(BidPrice5),"BidPrice5","BidPrice5",15);
		m_Describe.SetupMember(BidVolume5,(int)((char *)&BidVolume5-(char *)this),"BidVolume5",sizeof(BidVolume5),"BidVolume5","BidVolume5",10);
		m_Describe.SetupMember(AskPrice5,(int)((char *)&AskPrice5-(char *)this),"AskPrice5",sizeof(AskPrice5),"AskPrice5","AskPrice5",15);
		m_Describe.SetupMember(AskVolume5,(int)((char *)&AskVolume5-(char *)this),"AskVolume5",sizeof(AskVolume5),"AskVolume5","AskVolume5",10);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_FiveDepthMarketData=0x2126;

///10档深度行情
class CTenDepthMarketDataField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///产品组
	CShortIndexIDType	ProductGroup;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///申买价一
	CPriceType	BidPrice1;
	///申买量一
	CVolumeType	BidVolume1;
	///申卖价一
	CPriceType	AskPrice1;
	///申卖量一
	CVolumeType	AskVolume1;
	///申买价二
	CPriceType	BidPrice2;
	///申买量二
	CVolumeType	BidVolume2;
	///申卖价二
	CPriceType	AskPrice2;
	///申卖量二
	CVolumeType	AskVolume2;
	///申买价三
	CPriceType	BidPrice3;
	///申买量三
	CVolumeType	BidVolume3;
	///申卖价三
	CPriceType	AskPrice3;
	///申卖量三
	CVolumeType	AskVolume3;
	///申买价四
	CPriceType	BidPrice4;
	///申买量四
	CVolumeType	BidVolume4;
	///申卖价四
	CPriceType	AskPrice4;
	///申卖量四
	CVolumeType	AskVolume4;
	///申买价五
	CPriceType	BidPrice5;
	///申买量五
	CVolumeType	BidVolume5;
	///申卖价五
	CPriceType	AskPrice5;
	///申卖量五
	CVolumeType	AskVolume5;
	///申买价六
	CPriceType	BidPrice6;
	///申买量六
	CVolumeType	BidVolume6;
	///申卖价六
	CPriceType	AskPrice6;
	///申卖量六
	CVolumeType	AskVolume6;
	///申买价七
	CPriceType	BidPrice7;
	///申买量七
	CVolumeType	BidVolume7;
	///申卖价七
	CPriceType	AskPrice7;
	///申卖量七
	CVolumeType	AskVolume7;
	///申买价八
	CPriceType	BidPrice8;
	///申买量八
	CVolumeType	BidVolume8;
	///申卖价八
	CPriceType	AskPrice8;
	///申卖量八
	CVolumeType	AskVolume8;
	///申买价九
	CPriceType	BidPrice9;
	///申买量九
	CVolumeType	BidVolume9;
	///申卖价九
	CPriceType	AskPrice9;
	///申卖量九
	CVolumeType	AskVolume9;
	///申买价十
	CPriceType	BidPrice10;
	///申买量十
	CVolumeType	BidVolume10;
	///申卖价十
	CPriceType	AskPrice10;
	///申卖量十
	CVolumeType	AskVolume10;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(BidPrice1,(int)((char *)&BidPrice1-(char *)this),"BidPrice1",sizeof(BidPrice1),"BidPrice1","BidPrice1",15);
		m_Describe.SetupMember(BidVolume1,(int)((char *)&BidVolume1-(char *)this),"BidVolume1",sizeof(BidVolume1),"BidVolume1","BidVolume1",10);
		m_Describe.SetupMember(AskPrice1,(int)((char *)&AskPrice1-(char *)this),"AskPrice1",sizeof(AskPrice1),"AskPrice1","AskPrice1",15);
		m_Describe.SetupMember(AskVolume1,(int)((char *)&AskVolume1-(char *)this),"AskVolume1",sizeof(AskVolume1),"AskVolume1","AskVolume1",10);
		m_Describe.SetupMember(BidPrice2,(int)((char *)&BidPrice2-(char *)this),"BidPrice2",sizeof(BidPrice2),"BidPrice2","BidPrice2",15);
		m_Describe.SetupMember(BidVolume2,(int)((char *)&BidVolume2-(char *)this),"BidVolume2",sizeof(BidVolume2),"BidVolume2","BidVolume2",10);
		m_Describe.SetupMember(AskPrice2,(int)((char *)&AskPrice2-(char *)this),"AskPrice2",sizeof(AskPrice2),"AskPrice2","AskPrice2",15);
		m_Describe.SetupMember(AskVolume2,(int)((char *)&AskVolume2-(char *)this),"AskVolume2",sizeof(AskVolume2),"AskVolume2","AskVolume2",10);
		m_Describe.SetupMember(BidPrice3,(int)((char *)&BidPrice3-(char *)this),"BidPrice3",sizeof(BidPrice3),"BidPrice3","BidPrice3",15);
		m_Describe.SetupMember(BidVolume3,(int)((char *)&BidVolume3-(char *)this),"BidVolume3",sizeof(BidVolume3),"BidVolume3","BidVolume3",10);
		m_Describe.SetupMember(AskPrice3,(int)((char *)&AskPrice3-(char *)this),"AskPrice3",sizeof(AskPrice3),"AskPrice3","AskPrice3",15);
		m_Describe.SetupMember(AskVolume3,(int)((char *)&AskVolume3-(char *)this),"AskVolume3",sizeof(AskVolume3),"AskVolume3","AskVolume3",10);
		m_Describe.SetupMember(BidPrice4,(int)((char *)&BidPrice4-(char *)this),"BidPrice4",sizeof(BidPrice4),"BidPrice4","BidPrice4",15);
		m_Describe.SetupMember(BidVolume4,(int)((char *)&BidVolume4-(char *)this),"BidVolume4",sizeof(BidVolume4),"BidVolume4","BidVolume4",10);
		m_Describe.SetupMember(AskPrice4,(int)((char *)&AskPrice4-(char *)this),"AskPrice4",sizeof(AskPrice4),"AskPrice4","AskPrice4",15);
		m_Describe.SetupMember(AskVolume4,(int)((char *)&AskVolume4-(char *)this),"AskVolume4",sizeof(AskVolume4),"AskVolume4","AskVolume4",10);
		m_Describe.SetupMember(BidPrice5,(int)((char *)&BidPrice5-(char *)this),"BidPrice5",sizeof(BidPrice5),"BidPrice5","BidPrice5",15);
		m_Describe.SetupMember(BidVolume5,(int)((char *)&BidVolume5-(char *)this),"BidVolume5",sizeof(BidVolume5),"BidVolume5","BidVolume5",10);
		m_Describe.SetupMember(AskPrice5,(int)((char *)&AskPrice5-(char *)this),"AskPrice5",sizeof(AskPrice5),"AskPrice5","AskPrice5",15);
		m_Describe.SetupMember(AskVolume5,(int)((char *)&AskVolume5-(char *)this),"AskVolume5",sizeof(AskVolume5),"AskVolume5","AskVolume5",10);
		m_Describe.SetupMember(BidPrice6,(int)((char *)&BidPrice6-(char *)this),"BidPrice6",sizeof(BidPrice6),"BidPrice6","BidPrice6",15);
		m_Describe.SetupMember(BidVolume6,(int)((char *)&BidVolume6-(char *)this),"BidVolume6",sizeof(BidVolume6),"BidVolume6","BidVolume6",10);
		m_Describe.SetupMember(AskPrice6,(int)((char *)&AskPrice6-(char *)this),"AskPrice6",sizeof(AskPrice6),"AskPrice6","AskPrice6",15);
		m_Describe.SetupMember(AskVolume6,(int)((char *)&AskVolume6-(char *)this),"AskVolume6",sizeof(AskVolume6),"AskVolume6","AskVolume6",10);
		m_Describe.SetupMember(BidPrice7,(int)((char *)&BidPrice7-(char *)this),"BidPrice7",sizeof(BidPrice7),"BidPrice7","BidPrice7",15);
		m_Describe.SetupMember(BidVolume7,(int)((char *)&BidVolume7-(char *)this),"BidVolume7",sizeof(BidVolume7),"BidVolume7","BidVolume7",10);
		m_Describe.SetupMember(AskPrice7,(int)((char *)&AskPrice7-(char *)this),"AskPrice7",sizeof(AskPrice7),"AskPrice7","AskPrice7",15);
		m_Describe.SetupMember(AskVolume7,(int)((char *)&AskVolume7-(char *)this),"AskVolume7",sizeof(AskVolume7),"AskVolume7","AskVolume7",10);
		m_Describe.SetupMember(BidPrice8,(int)((char *)&BidPrice8-(char *)this),"BidPrice8",sizeof(BidPrice8),"BidPrice8","BidPrice8",15);
		m_Describe.SetupMember(BidVolume8,(int)((char *)&BidVolume8-(char *)this),"BidVolume8",sizeof(BidVolume8),"BidVolume8","BidVolume8",10);
		m_Describe.SetupMember(AskPrice8,(int)((char *)&AskPrice8-(char *)this),"AskPrice8",sizeof(AskPrice8),"AskPrice8","AskPrice8",15);
		m_Describe.SetupMember(AskVolume8,(int)((char *)&AskVolume8-(char *)this),"AskVolume8",sizeof(AskVolume8),"AskVolume8","AskVolume8",10);
		m_Describe.SetupMember(BidPrice9,(int)((char *)&BidPrice9-(char *)this),"BidPrice9",sizeof(BidPrice9),"BidPrice9","BidPrice9",15);
		m_Describe.SetupMember(BidVolume9,(int)((char *)&BidVolume9-(char *)this),"BidVolume9",sizeof(BidVolume9),"BidVolume9","BidVolume9",10);
		m_Describe.SetupMember(AskPrice9,(int)((char *)&AskPrice9-(char *)this),"AskPrice9",sizeof(AskPrice9),"AskPrice9","AskPrice9",15);
		m_Describe.SetupMember(AskVolume9,(int)((char *)&AskVolume9-(char *)this),"AskVolume9",sizeof(AskVolume9),"AskVolume9","AskVolume9",10);
		m_Describe.SetupMember(BidPrice10,(int)((char *)&BidPrice10-(char *)this),"BidPrice10",sizeof(BidPrice10),"BidPrice10","BidPrice10",15);
		m_Describe.SetupMember(BidVolume10,(int)((char *)&BidVolume10-(char *)this),"BidVolume10",sizeof(BidVolume10),"BidVolume10","BidVolume10",10);
		m_Describe.SetupMember(AskPrice10,(int)((char *)&AskPrice10-(char *)this),"AskPrice10",sizeof(AskPrice10),"AskPrice10","AskPrice10",15);
		m_Describe.SetupMember(AskVolume10,(int)((char *)&AskVolume10-(char *)this),"AskVolume10",sizeof(AskVolume10),"AskVolume10","AskVolume10",10);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_TenDepthMarketData=0x2127;

///期权行情
class COptionsMarketDataField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///产品组
	CShortIndexIDType	ProductGroup;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///上次收盘
	CPriceType	PreClosePrice;
	///上次结算价
	CPriceType	PreSettlementPrice;
	///上次持仓量
	CVolumeType	PreOpenInterest;
	///上次持仓费按比例
	CRatioType	PrePositionFeeRate;
	///收盘价
	CPriceType	ClosePrice;
	///收取持仓费用时间
	CMilliSecondsType	PositionFeeTime;
	///结算时间
	CMilliSecondsType	SettlementTime;
	///最高限价
	CPriceType	UpperLimitPrice;
	///最低限价
	CPriceType	LowerLimitPrice;
	///基础标的价格
	CPriceType	UnderlyingPrice;
	///标记价格
	CPriceType	MarkedPrice;
	///预计持仓费按比例
	CRatioType	PositionFeeRate;
	///当日最高价
	CPriceType	HighestPrice;
	///当日最低价
	CPriceType	LowestPrice;
	///最新价
	CPriceType	LastPrice;
	///当日数量
	CCollectType	Volume;
	///当日成交金额
	CCollectType	Turnover;
	///持仓量
	CVolumeType	OpenInterest;
	///预估结算价
	CPriceType	SettlementPrice;
	///当日开盘价
	CPriceType	OpenPrice;
	///虚实度
	CRatioType	Delta;
	///曲线凸度
	CRatioType	Gamma;
	///波动率度
	CRatioType	Vega;
	///时间价值度
	CRatioType	Theta;
	///利率度
	CRatioType	Rho;
	///最新波动率
	CRatioType	LastVolatility;
	///买一波动率
	CRatioType	Bid1Volatility;
	///卖一波动率
	CRatioType	Ask1Volatility;
	///标记波动率
	CRatioType	MaskedVolatility;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(PreClosePrice,(int)((char *)&PreClosePrice-(char *)this),"PreClosePrice",sizeof(PreClosePrice),"PreClosePrice","PreClosePrice",15);
		m_Describe.SetupMember(PreSettlementPrice,(int)((char *)&PreSettlementPrice-(char *)this),"PreSettlementPrice",sizeof(PreSettlementPrice),"PreSettlementPrice","PreSettlementPrice",15);
		m_Describe.SetupMember(PreOpenInterest,(int)((char *)&PreOpenInterest-(char *)this),"PreOpenInterest",sizeof(PreOpenInterest),"PreOpenInterest","PreOpenInterest",10);
		m_Describe.SetupMember(PrePositionFeeRate,(int)((char *)&PrePositionFeeRate-(char *)this),"PrePositionFeeRate",sizeof(PrePositionFeeRate),"PrePositionFeeRate","PrePositionFeeRate",10);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(PositionFeeTime,(int)((char *)&PositionFeeTime-(char *)this),"PositionFeeTime",sizeof(PositionFeeTime),"PositionFeeTime","PositionFeeTime",3);
		m_Describe.SetupMember(SettlementTime,(int)((char *)&SettlementTime-(char *)this),"SettlementTime",sizeof(SettlementTime),"SettlementTime","SettlementTime",3);
		m_Describe.SetupMember(UpperLimitPrice,(int)((char *)&UpperLimitPrice-(char *)this),"UpperLimitPrice",sizeof(UpperLimitPrice),"UpperLimitPrice","UpperLimitPrice",15);
		m_Describe.SetupMember(LowerLimitPrice,(int)((char *)&LowerLimitPrice-(char *)this),"LowerLimitPrice",sizeof(LowerLimitPrice),"LowerLimitPrice","LowerLimitPrice",15);
		m_Describe.SetupMember(UnderlyingPrice,(int)((char *)&UnderlyingPrice-(char *)this),"UnderlyingPrice",sizeof(UnderlyingPrice),"UnderlyingPrice","UnderlyingPrice",15);
		m_Describe.SetupMember(MarkedPrice,(int)((char *)&MarkedPrice-(char *)this),"MarkedPrice",sizeof(MarkedPrice),"MarkedPrice","MarkedPrice",15);
		m_Describe.SetupMember(PositionFeeRate,(int)((char *)&PositionFeeRate-(char *)this),"PositionFeeRate",sizeof(PositionFeeRate),"PositionFeeRate","PositionFeeRate",10);
		m_Describe.SetupMember(HighestPrice,(int)((char *)&HighestPrice-(char *)this),"HighestPrice",sizeof(HighestPrice),"HighestPrice","HighestPrice",15);
		m_Describe.SetupMember(LowestPrice,(int)((char *)&LowestPrice-(char *)this),"LowestPrice",sizeof(LowestPrice),"LowestPrice","LowestPrice",15);
		m_Describe.SetupMember(LastPrice,(int)((char *)&LastPrice-(char *)this),"LastPrice",sizeof(LastPrice),"LastPrice","LastPrice",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",8);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",8);
		m_Describe.SetupMember(OpenInterest,(int)((char *)&OpenInterest-(char *)this),"OpenInterest",sizeof(OpenInterest),"OpenInterest","OpenInterest",10);
		m_Describe.SetupMember(SettlementPrice,(int)((char *)&SettlementPrice-(char *)this),"SettlementPrice",sizeof(SettlementPrice),"SettlementPrice","SettlementPrice",15);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(Delta,(int)((char *)&Delta-(char *)this),"Delta",sizeof(Delta),"Delta","Delta",10);
		m_Describe.SetupMember(Gamma,(int)((char *)&Gamma-(char *)this),"Gamma",sizeof(Gamma),"Gamma","Gamma",10);
		m_Describe.SetupMember(Vega,(int)((char *)&Vega-(char *)this),"Vega",sizeof(Vega),"Vega","Vega",10);
		m_Describe.SetupMember(Theta,(int)((char *)&Theta-(char *)this),"Theta",sizeof(Theta),"Theta","Theta",10);
		m_Describe.SetupMember(Rho,(int)((char *)&Rho-(char *)this),"Rho",sizeof(Rho),"Rho","Rho",10);
		m_Describe.SetupMember(LastVolatility,(int)((char *)&LastVolatility-(char *)this),"LastVolatility",sizeof(LastVolatility),"LastVolatility","LastVolatility",10);
		m_Describe.SetupMember(Bid1Volatility,(int)((char *)&Bid1Volatility-(char *)this),"Bid1Volatility",sizeof(Bid1Volatility),"Bid1Volatility","Bid1Volatility",10);
		m_Describe.SetupMember(Ask1Volatility,(int)((char *)&Ask1Volatility-(char *)this),"Ask1Volatility",sizeof(Ask1Volatility),"Ask1Volatility","Ask1Volatility",10);
		m_Describe.SetupMember(MaskedVolatility,(int)((char *)&MaskedVolatility-(char *)this),"MaskedVolatility",sizeof(MaskedVolatility),"MaskedVolatility","MaskedVolatility",10);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_OptionsMarketData=0x2128;

///更新合约
class CInstrumentUpdateField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///最小下单量
	CVolumeType	MinOrderVolume;
	///最小下单额
	CVolumeType	MinOrderCost;
	///最大下单量
	CVolumeType	MaxOrderVolume;
	///最小变动价位
	CPriceType	PriceTick;
	///最小变动量
	CVolumeType	VolumeTick;
	///最大持仓量
	CVolumeType	MaxOpenInterest;
	///挂牌基准价
	CPriceType	BasisPrice;
	///最高限价
	CPriceType	UpperLimitPrice;
	///最低限价
	CPriceType	LowerLimitPrice;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(MinOrderVolume,(int)((char *)&MinOrderVolume-(char *)this),"MinOrderVolume",sizeof(MinOrderVolume),"MinOrderVolume","MinOrderVolume",10);
		m_Describe.SetupMember(MinOrderCost,(int)((char *)&MinOrderCost-(char *)this),"MinOrderCost",sizeof(MinOrderCost),"MinOrderCost","MinOrderCost",10);
		m_Describe.SetupMember(MaxOrderVolume,(int)((char *)&MaxOrderVolume-(char *)this),"MaxOrderVolume",sizeof(MaxOrderVolume),"MaxOrderVolume","MaxOrderVolume",10);
		m_Describe.SetupMember(PriceTick,(int)((char *)&PriceTick-(char *)this),"PriceTick",sizeof(PriceTick),"PriceTick","PriceTick",15);
		m_Describe.SetupMember(VolumeTick,(int)((char *)&VolumeTick-(char *)this),"VolumeTick",sizeof(VolumeTick),"VolumeTick","VolumeTick",10);
		m_Describe.SetupMember(MaxOpenInterest,(int)((char *)&MaxOpenInterest-(char *)this),"MaxOpenInterest",sizeof(MaxOpenInterest),"MaxOpenInterest","MaxOpenInterest",10);
		m_Describe.SetupMember(BasisPrice,(int)((char *)&BasisPrice-(char *)this),"BasisPrice",sizeof(BasisPrice),"BasisPrice","BasisPrice",15);
		m_Describe.SetupMember(UpperLimitPrice,(int)((char *)&UpperLimitPrice-(char *)this),"UpperLimitPrice",sizeof(UpperLimitPrice),"UpperLimitPrice","UpperLimitPrice",15);
		m_Describe.SetupMember(LowerLimitPrice,(int)((char *)&LowerLimitPrice-(char *)this),"LowerLimitPrice",sizeof(LowerLimitPrice),"LowerLimitPrice","LowerLimitPrice",15);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_InstrumentUpdate=0x212F;

///更新行情
class CMarketDataUpdateField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///合理价格
	CPriceType	MarkedPrice;
	///理论价
	CPriceType	TheoryPrice;
	///上次结算价
	CPriceType	PreSettlementPrice;
	///结算价
	CPriceType	SettlementPrice;
	///基础标的价格
	CPriceType	UnderlyingPrice;
	///持仓费按比例
	CRatioType	PrePositionFeeRate;
	///预计持仓费按比例
	CRatioType	PositionFeeRate;
	///标的交易状态
	CInstrumentStatusType	InstrumentStatus;
	///数量
	CCollectType	Volume;
	///成交金额
	CCollectType	Turnover;
	///备注
	CLongNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(MarkedPrice,(int)((char *)&MarkedPrice-(char *)this),"MarkedPrice",sizeof(MarkedPrice),"MarkedPrice","MarkedPrice",15);
		m_Describe.SetupMember(TheoryPrice,(int)((char *)&TheoryPrice-(char *)this),"TheoryPrice",sizeof(TheoryPrice),"TheoryPrice","TheoryPrice",15);
		m_Describe.SetupMember(PreSettlementPrice,(int)((char *)&PreSettlementPrice-(char *)this),"PreSettlementPrice",sizeof(PreSettlementPrice),"PreSettlementPrice","PreSettlementPrice",15);
		m_Describe.SetupMember(SettlementPrice,(int)((char *)&SettlementPrice-(char *)this),"SettlementPrice",sizeof(SettlementPrice),"SettlementPrice","SettlementPrice",15);
		m_Describe.SetupMember(UnderlyingPrice,(int)((char *)&UnderlyingPrice-(char *)this),"UnderlyingPrice",sizeof(UnderlyingPrice),"UnderlyingPrice","UnderlyingPrice",15);
		m_Describe.SetupMember(PrePositionFeeRate,(int)((char *)&PrePositionFeeRate-(char *)this),"PrePositionFeeRate",sizeof(PrePositionFeeRate),"PrePositionFeeRate","PrePositionFeeRate",10);
		m_Describe.SetupMember(PositionFeeRate,(int)((char *)&PositionFeeRate-(char *)this),"PositionFeeRate",sizeof(PositionFeeRate),"PositionFeeRate","PositionFeeRate",10);
		m_Describe.SetupMember(InstrumentStatus,(int)((char *)&InstrumentStatus-(char *)this),"InstrumentStatus",sizeof(InstrumentStatus),"InstrumentStatus","InstrumentStatus",0);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",8);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",8);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_MarketDataUpdate=0x2130;

///成交明细
class CMarketTradeField
{
public:
	///成交代码
	CIndexIDType	TradeID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///Taker方买卖方向
	CDirectionType	Direction;
	///成交价格
	CPriceType	Price;
	///成交数量
	CVolumeType	Volume;
	///插入时间
	CMilliSecondsType	InsertTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(TradeID,(int)((char *)&TradeID-(char *)this),"TradeID",sizeof(TradeID),"TradeID","TradeID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_MarketTrade=0x2131;

///资金结算
class CSettlementField
{
public:
	///应用编号
	CShortIndexIDType	APPID;
	///成交对本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///结算操作
	CSettleActionTypeType	SettleAction;
	///操作使用值
	CRatioType	Value;
	///发生值1
	CRatioType	Value1;
	///发生值2
	CRatioType	Value2;
	///发生值3
	CRatioType	Value3;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(SettleAction,(int)((char *)&SettleAction-(char *)this),"SettleAction",sizeof(SettleAction),"SettleAction","SettleAction",0);
		m_Describe.SetupMember(Value,(int)((char *)&Value-(char *)this),"Value",sizeof(Value),"Value","Value",10);
		m_Describe.SetupMember(Value1,(int)((char *)&Value1-(char *)this),"Value1",sizeof(Value1),"Value1","Value1",10);
		m_Describe.SetupMember(Value2,(int)((char *)&Value2-(char *)this),"Value2",sizeof(Value2),"Value2","Value2",10);
		m_Describe.SetupMember(Value3,(int)((char *)&Value3-(char *)this),"Value3",sizeof(Value3),"Value3","Value3",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Settlement=0x2133;

///输入APIKey
class CAPIKeyInsertField
{
public:
	///备注
	CNameType	Name;
	///交易用户代码
	CLongIndexIDType	UserID;
	///应用编号
	CShortIndexIDType	APPID;
	///成员代码
	CLongIndexIDType	MemberID;
	///访问密钥
	CShortContentType	AccessKey;
	///签名加密密钥
	CShortContentType	SecretKey;
	///权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭
	CNameType	Auth;
	///每秒访问的次数限制
	CNumberType	AccessLimit;
	///创建时间
	CMilliSecondsType	CreateTime;
	///失效时间
	CMilliSecondsType	ExpireTime;
	///绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2
	CContentType	IPAddress;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(Name,(int)((char *)&Name-(char *)this),"Name",sizeof(Name),"Name","Name",0);
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccessKey,(int)((char *)&AccessKey-(char *)this),"AccessKey",sizeof(AccessKey),"AccessKey","AccessKey",0);
		m_Describe.SetupMember(SecretKey,(int)((char *)&SecretKey-(char *)this),"SecretKey",sizeof(SecretKey),"SecretKey","SecretKey",0);
		m_Describe.SetupMember(Auth,(int)((char *)&Auth-(char *)this),"Auth",sizeof(Auth),"Auth","Auth",0);
		m_Describe.SetupMember(AccessLimit,(int)((char *)&AccessLimit-(char *)this),"AccessLimit",sizeof(AccessLimit),"AccessLimit","AccessLimit",0);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",3);
		m_Describe.SetupMember(ExpireTime,(int)((char *)&ExpireTime-(char *)this),"ExpireTime",sizeof(ExpireTime),"ExpireTime","ExpireTime",3);
		m_Describe.SetupMember(IPAddress,(int)((char *)&IPAddress-(char *)this),"IPAddress",sizeof(IPAddress),"IPAddress","IPAddress",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_APIKeyInsert=0x2134;

///APIKey
class CAPIKeyField
{
public:
	///备注
	CNameType	Name;
	///交易用户代码
	CLongIndexIDType	UserID;
	///应用编号
	CShortIndexIDType	APPID;
	///成员代码
	CLongIndexIDType	MemberID;
	///访问密钥
	CShortContentType	AccessKey;
	///签名加密密钥
	CShortContentType	SecretKey;
	///权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭
	CNameType	Auth;
	///每秒访问的次数限制
	CNumberType	AccessLimit;
	///创建时间
	CMilliSecondsType	CreateTime;
	///失效时间
	CMilliSecondsType	ExpireTime;
	///绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2
	CContentType	IPAddress;
	///剩余有效天数
	CNumberType	RemainDays;
	///状态，1：正常，0：失效
	CNumberType	Status;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(Name,(int)((char *)&Name-(char *)this),"Name",sizeof(Name),"Name","Name",0);
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccessKey,(int)((char *)&AccessKey-(char *)this),"AccessKey",sizeof(AccessKey),"AccessKey","AccessKey",0);
		m_Describe.SetupMember(SecretKey,(int)((char *)&SecretKey-(char *)this),"SecretKey",sizeof(SecretKey),"SecretKey","SecretKey",0);
		m_Describe.SetupMember(Auth,(int)((char *)&Auth-(char *)this),"Auth",sizeof(Auth),"Auth","Auth",0);
		m_Describe.SetupMember(AccessLimit,(int)((char *)&AccessLimit-(char *)this),"AccessLimit",sizeof(AccessLimit),"AccessLimit","AccessLimit",0);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",3);
		m_Describe.SetupMember(ExpireTime,(int)((char *)&ExpireTime-(char *)this),"ExpireTime",sizeof(ExpireTime),"ExpireTime","ExpireTime",3);
		m_Describe.SetupMember(IPAddress,(int)((char *)&IPAddress-(char *)this),"IPAddress",sizeof(IPAddress),"IPAddress","IPAddress",0);
		m_Describe.SetupMember(RemainDays,(int)((char *)&RemainDays-(char *)this),"RemainDays",sizeof(RemainDays),"RemainDays","RemainDays",0);
		m_Describe.SetupMember(Status,(int)((char *)&Status-(char *)this),"Status",sizeof(Status),"Status","Status",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_APIKey=0x2135;

///操作APIKey
class CAPIKeyActionField
{
public:
	///备注
	CNameType	Name;
	///访问密钥
	CShortContentType	AccessKey;
	///权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭
	CNameType	Auth;
	///每秒访问的次数限制
	CNumberType	AccessLimit;
	///绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2
	CContentType	IPAddress;
	///失效时间
	CMilliSecondsType	ExpireTime;
	///操作标志
	CActionFlagType	ActionFlag;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(Name,(int)((char *)&Name-(char *)this),"Name",sizeof(Name),"Name","Name",0);
		m_Describe.SetupMember(AccessKey,(int)((char *)&AccessKey-(char *)this),"AccessKey",sizeof(AccessKey),"AccessKey","AccessKey",0);
		m_Describe.SetupMember(Auth,(int)((char *)&Auth-(char *)this),"Auth",sizeof(Auth),"Auth","Auth",0);
		m_Describe.SetupMember(AccessLimit,(int)((char *)&AccessLimit-(char *)this),"AccessLimit",sizeof(AccessLimit),"AccessLimit","AccessLimit",0);
		m_Describe.SetupMember(IPAddress,(int)((char *)&IPAddress-(char *)this),"IPAddress",sizeof(IPAddress),"IPAddress","IPAddress",0);
		m_Describe.SetupMember(ExpireTime,(int)((char *)&ExpireTime-(char *)this),"ExpireTime",sizeof(ExpireTime),"ExpireTime","ExpireTime",3);
		m_Describe.SetupMember(ActionFlag,(int)((char *)&ActionFlag-(char *)this),"ActionFlag",sizeof(ActionFlag),"ActionFlag","ActionFlag",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_APIKeyAction=0x2136;

///清理行情数据
class CClearMarketField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_ClearMarket=0x2137;

///清理最新K线数据
class CClearLastKLineField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///K线周期代码(n[m/h/d/o])
	CBarType	Bar;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Bar,(int)((char *)&Bar-(char *)this),"Bar",sizeof(Bar),"Bar","Bar",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_ClearLastKLine=0x2138;

///输入OToken
class COTokenInsertField
{
public:
	///交易用户代码
	CLongIndexIDType	UserID;
	///应用编号
	CShortIndexIDType	APPID;
	///令牌
	CShortContentType	Token;
	///成员代码
	CLongIndexIDType	MemberID;
	///登录时间
	CMilliSecondsType	LoginTime;
	///失效时间
	CMilliSecondsType	ExpireTime;
	///每秒访问的次数限制
	CNumberType	AccessLimit;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(Token,(int)((char *)&Token-(char *)this),"Token",sizeof(Token),"Token","Token",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(LoginTime,(int)((char *)&LoginTime-(char *)this),"LoginTime",sizeof(LoginTime),"LoginTime","LoginTime",3);
		m_Describe.SetupMember(ExpireTime,(int)((char *)&ExpireTime-(char *)this),"ExpireTime",sizeof(ExpireTime),"ExpireTime","ExpireTime",3);
		m_Describe.SetupMember(AccessLimit,(int)((char *)&AccessLimit-(char *)this),"AccessLimit",sizeof(AccessLimit),"AccessLimit","AccessLimit",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_OTokenInsert=0x2139;

///操作OToken
class COTokenActionField
{
public:
	///交易用户代码
	CLongIndexIDType	UserID;
	///应用编号
	CShortIndexIDType	APPID;
	///操作标志
	CActionFlagType	ActionFlag;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(ActionFlag,(int)((char *)&ActionFlag-(char *)this),"ActionFlag",sizeof(ActionFlag),"ActionFlag","ActionFlag",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_OTokenAction=0x213A;

///重置LocalID
class CResetLocalIDField
{
public:
	///令牌
	CShortContentType	Token;
	///交易用户代码
	CLongIndexIDType	UserID;
	///应用编号
	CShortIndexIDType	APPID;
	///成员代码
	CLongIndexIDType	MemberID;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///资金账号
	CAccountIDType	AccountID;
	///币种
	CCurrencyType	Currency;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(Token,(int)((char *)&Token-(char *)this),"Token",sizeof(Token),"Token","Token",0);
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_ResetLocalID=0x213B;

///合并持仓
class CPositionMergeField
{
public:
	///操作本地号
	CIndexIDType	LocalID;
	///成员代码
	CLongIndexIDType	MemberID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///持仓代码1
	CLongIndexIDType	PositionID1;
	///持仓代码2
	CLongIndexIDType	PositionID2;
	///持仓代码3
	CLongIndexIDType	PositionID3;
	///持仓代码4
	CLongIndexIDType	PositionID4;
	///持仓代码5
	CLongIndexIDType	PositionID5;
	///持仓代码6
	CLongIndexIDType	PositionID6;
	///持仓代码7
	CLongIndexIDType	PositionID7;
	///持仓代码8
	CLongIndexIDType	PositionID8;
	///持仓代码9
	CLongIndexIDType	PositionID9;
	///持仓代码10
	CLongIndexIDType	PositionID10;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(PositionID1,(int)((char *)&PositionID1-(char *)this),"PositionID1",sizeof(PositionID1),"PositionID1","PositionID1",0);
		m_Describe.SetupMember(PositionID2,(int)((char *)&PositionID2-(char *)this),"PositionID2",sizeof(PositionID2),"PositionID2","PositionID2",0);
		m_Describe.SetupMember(PositionID3,(int)((char *)&PositionID3-(char *)this),"PositionID3",sizeof(PositionID3),"PositionID3","PositionID3",0);
		m_Describe.SetupMember(PositionID4,(int)((char *)&PositionID4-(char *)this),"PositionID4",sizeof(PositionID4),"PositionID4","PositionID4",0);
		m_Describe.SetupMember(PositionID5,(int)((char *)&PositionID5-(char *)this),"PositionID5",sizeof(PositionID5),"PositionID5","PositionID5",0);
		m_Describe.SetupMember(PositionID6,(int)((char *)&PositionID6-(char *)this),"PositionID6",sizeof(PositionID6),"PositionID6","PositionID6",0);
		m_Describe.SetupMember(PositionID7,(int)((char *)&PositionID7-(char *)this),"PositionID7",sizeof(PositionID7),"PositionID7","PositionID7",0);
		m_Describe.SetupMember(PositionID8,(int)((char *)&PositionID8-(char *)this),"PositionID8",sizeof(PositionID8),"PositionID8","PositionID8",0);
		m_Describe.SetupMember(PositionID9,(int)((char *)&PositionID9-(char *)this),"PositionID9",sizeof(PositionID9),"PositionID9","PositionID9",0);
		m_Describe.SetupMember(PositionID10,(int)((char *)&PositionID10-(char *)this),"PositionID10",sizeof(PositionID10),"PositionID10","PositionID10",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_PositionMerge=0x213C;

///用户入金
class CDepositField
{
public:
	///出入金本地号
	CIndexIDType	LocalID;
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CIndexIDType	AccountID;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///发生额
	CMoneyType	Amount;
	///币种
	CCurrencyType	Currency;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(Amount,(int)((char *)&Amount-(char *)this),"Amount",sizeof(Amount),"Amount","Amount",10);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Deposit=0x213D;

///用户出金
class CWithdrawField
{
public:
	///出入金本地号
	CIndexIDType	LocalID;
	///冻结无需填,真正出金的订单号
	CIndexIDType	OrderID;
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///转出结算组编号
	CShortIndexIDType	SettlementGroup;
	///转入结算组编号
	CShortIndexIDType	OtherSettlementGroup;
	///发生额
	CMoneyType	Amount;
	///币种
	CCurrencyType	Currency;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(OtherSettlementGroup,(int)((char *)&OtherSettlementGroup-(char *)this),"OtherSettlementGroup",sizeof(OtherSettlementGroup),"OtherSettlementGroup","OtherSettlementGroup",0);
		m_Describe.SetupMember(Amount,(int)((char *)&Amount-(char *)this),"Amount",sizeof(Amount),"Amount","Amount",10);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Withdraw=0x213E;

///用户转账
class CTransferField
{
public:
	///出入金本地号
	CIndexIDType	LocalID;
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///内部转账资金账号
	CAccountIDType	OtherAccountID;
	///内部转账结算组编号
	CShortIndexIDType	OtherSettlementGroup;
	///内部转账成员代码
	CLongIndexIDType	OtherMemberID;
	///发生额
	CMoneyType	Amount;
	///币种
	CCurrencyType	Currency;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(OtherAccountID,(int)((char *)&OtherAccountID-(char *)this),"OtherAccountID",sizeof(OtherAccountID),"OtherAccountID","OtherAccountID",0);
		m_Describe.SetupMember(OtherSettlementGroup,(int)((char *)&OtherSettlementGroup-(char *)this),"OtherSettlementGroup",sizeof(OtherSettlementGroup),"OtherSettlementGroup","OtherSettlementGroup",0);
		m_Describe.SetupMember(OtherMemberID,(int)((char *)&OtherMemberID-(char *)this),"OtherMemberID",sizeof(OtherMemberID),"OtherMemberID","OtherMemberID",0);
		m_Describe.SetupMember(Amount,(int)((char *)&Amount-(char *)this),"Amount",sizeof(Amount),"Amount","Amount",10);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Transfer=0x213F;

///报单止盈止损操作
class CCloseOrderActionField
{
public:
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///报单本地标识
	CIndexIDType	OrderLocalID;
	///报单操作标志
	CActionFlagType	ActionFlag;
	///操作本地标识
	CIndexIDType	LocalID;
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///价格
	CPriceType	Price;
	///数量变化
	CVolumeType	Volume;
	///买卖方向
	CDirectionType	Direction;
	///显示数量
	CVolumeType	VolumeDisplay;
	///产品组
	CShortIndexIDType	ProductGroup;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///委托额
	CMoneyType	Cost;
	///备注
	CNameType	Remark;
	///止损价
	CPriceType	SLPrice;
	///止损触发价
	CPriceType	SLTriggerPrice;
	///止盈价
	CPriceType	TPPrice;
	///止盈触发价
	CPriceType	TPTriggerPrice;
	///触发价
	CPriceType	TriggerPrice;
	///触发的订单类型
	CShortIndexIDType	TriggerOrderType;
	///触发单具体设置信息
	CLongContentType	TriggerValue;
	///触发类型明细
	CIndexIDType	TriggerDetail;
	///触发价类型
	CTriggerPriceTypeType	TriggerPriceType;
	///错误代码
	CNumberType	ErrorNo;
	///错误信息
	CShortContentType	ErrorMsg;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(OrderLocalID,(int)((char *)&OrderLocalID-(char *)this),"OrderLocalID",sizeof(OrderLocalID),"OrderLocalID","OrderLocalID",0);
		m_Describe.SetupMember(ActionFlag,(int)((char *)&ActionFlag-(char *)this),"ActionFlag",sizeof(ActionFlag),"ActionFlag","ActionFlag",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(SLPrice,(int)((char *)&SLPrice-(char *)this),"SLPrice",sizeof(SLPrice),"SLPrice","SLPrice",15);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(TPPrice,(int)((char *)&TPPrice-(char *)this),"TPPrice",sizeof(TPPrice),"TPPrice","TPPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(TriggerOrderType,(int)((char *)&TriggerOrderType-(char *)this),"TriggerOrderType",sizeof(TriggerOrderType),"TriggerOrderType","TriggerOrderType",0);
		m_Describe.SetupMember(TriggerValue,(int)((char *)&TriggerValue-(char *)this),"TriggerValue",sizeof(TriggerValue),"TriggerValue","TriggerValue",0);
		m_Describe.SetupMember(TriggerDetail,(int)((char *)&TriggerDetail-(char *)this),"TriggerDetail",sizeof(TriggerDetail),"TriggerDetail","TriggerDetail",0);
		m_Describe.SetupMember(TriggerPriceType,(int)((char *)&TriggerPriceType-(char *)this),"TriggerPriceType",sizeof(TriggerPriceType),"TriggerPriceType","TriggerPriceType",0);
		m_Describe.SetupMember(ErrorNo,(int)((char *)&ErrorNo-(char *)this),"ErrorNo",sizeof(ErrorNo),"ErrorNo","ErrorNo",0);
		m_Describe.SetupMember(ErrorMsg,(int)((char *)&ErrorMsg-(char *)this),"ErrorMsg",sizeof(ErrorMsg),"ErrorMsg","ErrorMsg",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_CloseOrderAction=0x2141;

///杠杆倍数
class CLeverageField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///持仓多空方向
	CPosiDirectionType	PosiDirection;
	///默认最大杠杆倍数
	CLeverageType	DefaultMaxLeverage;
	///多头杠杆倍数
	CLeverageType	LongLeverage;
	///当前多头梯度最大杠杆倍数
	CLeverageType	LongMaxLeverage;
	///当前多头梯度最大张数
	CVolumeType	LongMaxVolume;
	///当前多头梯度剩余可开张数
	CVolumeType	LongLastVolume;
	///多头梯度档位
	CNumberType	LongLevel;
	///当前多头维持保证金率
	CRatioType	LongMaintRatio;
	///空头杠杆倍数
	CLeverageType	ShortLeverage;
	///当前空头梯度最大杠杆倍数
	CLeverageType	ShortMaxLeverage;
	///当前空头梯度最大张数
	CVolumeType	ShortMaxVolume;
	///当前空头梯度剩余可开张数
	CVolumeType	ShortLastVolume;
	///空头梯度档位
	CNumberType	ShortLevel;
	///当前空头维持保证金率
	CRatioType	ShortMaintRatio;
	///是否全仓
	CBoolType	IsCrossMargin;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
		m_Describe.SetupMember(DefaultMaxLeverage,(int)((char *)&DefaultMaxLeverage-(char *)this),"DefaultMaxLeverage",sizeof(DefaultMaxLeverage),"DefaultMaxLeverage","DefaultMaxLeverage",8);
		m_Describe.SetupMember(LongLeverage,(int)((char *)&LongLeverage-(char *)this),"LongLeverage",sizeof(LongLeverage),"LongLeverage","LongLeverage",8);
		m_Describe.SetupMember(LongMaxLeverage,(int)((char *)&LongMaxLeverage-(char *)this),"LongMaxLeverage",sizeof(LongMaxLeverage),"LongMaxLeverage","LongMaxLeverage",8);
		m_Describe.SetupMember(LongMaxVolume,(int)((char *)&LongMaxVolume-(char *)this),"LongMaxVolume",sizeof(LongMaxVolume),"LongMaxVolume","LongMaxVolume",10);
		m_Describe.SetupMember(LongLastVolume,(int)((char *)&LongLastVolume-(char *)this),"LongLastVolume",sizeof(LongLastVolume),"LongLastVolume","LongLastVolume",10);
		m_Describe.SetupMember(LongLevel,(int)((char *)&LongLevel-(char *)this),"LongLevel",sizeof(LongLevel),"LongLevel","LongLevel",0);
		m_Describe.SetupMember(LongMaintRatio,(int)((char *)&LongMaintRatio-(char *)this),"LongMaintRatio",sizeof(LongMaintRatio),"LongMaintRatio","LongMaintRatio",10);
		m_Describe.SetupMember(ShortLeverage,(int)((char *)&ShortLeverage-(char *)this),"ShortLeverage",sizeof(ShortLeverage),"ShortLeverage","ShortLeverage",8);
		m_Describe.SetupMember(ShortMaxLeverage,(int)((char *)&ShortMaxLeverage-(char *)this),"ShortMaxLeverage",sizeof(ShortMaxLeverage),"ShortMaxLeverage","ShortMaxLeverage",8);
		m_Describe.SetupMember(ShortMaxVolume,(int)((char *)&ShortMaxVolume-(char *)this),"ShortMaxVolume",sizeof(ShortMaxVolume),"ShortMaxVolume","ShortMaxVolume",10);
		m_Describe.SetupMember(ShortLastVolume,(int)((char *)&ShortLastVolume-(char *)this),"ShortLastVolume",sizeof(ShortLastVolume),"ShortLastVolume","ShortLastVolume",10);
		m_Describe.SetupMember(ShortLevel,(int)((char *)&ShortLevel-(char *)this),"ShortLevel",sizeof(ShortLevel),"ShortLevel","ShortLevel",0);
		m_Describe.SetupMember(ShortMaintRatio,(int)((char *)&ShortMaintRatio-(char *)this),"ShortMaintRatio",sizeof(ShortMaintRatio),"ShortMaintRatio","ShortMaintRatio",10);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Leverage=0x2142;

///反向开仓
class CPositionReverseField
{
public:
	///操作本地号
	CIndexIDType	LocalID;
	///成员代码
	CLongIndexIDType	MemberID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///反向开仓数量
	CVolumeType	Volume;
	///备注
	CNameType	Remark;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///应用编号
	CShortIndexIDType	APPID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_PositionReverse=0x2143;

///私有主题分类
class CPrivateDispatchField
{
public:
	///请参考TOPICID类型定义
	CLongIndexIDType	TopicID;
	///成员代码
	CLongIndexIDType	MemberID;
	///私有主题起点
	CLongIndexIDType	PrivateBase;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(TopicID,(int)((char *)&TopicID-(char *)this),"TopicID",sizeof(TopicID),"TopicID","TopicID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(PrivateBase,(int)((char *)&PrivateBase-(char *)this),"PrivateBase",sizeof(PrivateBase),"PrivateBase","PrivateBase",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_PrivateDispatch=0x2144;

///TICK订单簿变化
class CTickMarketOrderField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///买卖方向
	CDirectionType	Direction;
	///价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///订单数量
	CNumberType	Orders;
	///最后变化序列号
	CLargeNumberType	BusinessNo;
	///真实报单数量
	CVolumeType	TrueVolume;
	///最小变动价位
	CPriceType	Tick;
	///索引
	CShortContentType	SubIndex;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(Orders,(int)((char *)&Orders-(char *)this),"Orders",sizeof(Orders),"Orders","Orders",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"","BusinessNo",0);
		m_Describe.SetupMember(TrueVolume,(int)((char *)&TrueVolume-(char *)this),"TrueVolume",sizeof(TrueVolume),"TrueVolume","TrueVolume",10);
		m_Describe.SetupMember(Tick,(int)((char *)&Tick-(char *)this),"Tick",sizeof(Tick),"Tick","Tick",15);
		m_Describe.SetupMember(SubIndex,(int)((char *)&SubIndex-(char *)this),"SubIndex",sizeof(SubIndex),"SubIndex","SubIndex",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_TickMarketOrder=0x2146;

///基础K线
class CKLineBaseField
{
public:
	///插入时间
	CMilliSecondsType	InsertTime;
	///开盘价
	CPriceType	OpenPrice;
	///最高价
	CPriceType	HighestPrice;
	///最低价
	CPriceType	LowestPrice;
	///收盘价
	CPriceType	ClosePrice;
	///数量
	CCollectType	Volume;
	///成交金额
	CCollectType	Turnover;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(HighestPrice,(int)((char *)&HighestPrice-(char *)this),"HighestPrice",sizeof(HighestPrice),"HighestPrice","HighestPrice",15);
		m_Describe.SetupMember(LowestPrice,(int)((char *)&LowestPrice-(char *)this),"LowestPrice",sizeof(LowestPrice),"LowestPrice","LowestPrice",15);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",8);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",8);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_KLineBase=0x2147;

///历史行情线
class CLineField
{
public:
	///插入时间
	CMilliSecondsType	InsertTime;
	///收盘价
	CPriceType	ClosePrice;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Line=0x2148;

///充币兑换
class CChangeCurrencyField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///报单本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///第一个标的代码
	CInstrumentIDType	InstrumentID1;
	///第二个标的代码
	CInstrumentIDType	InstrumentID2;
	///币种
	CCurrencyType	Currency;
	///发生额
	CMoneyType	Amount;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID1,(int)((char *)&InstrumentID1-(char *)this),"InstrumentID1",sizeof(InstrumentID1),"InstrumentID1","InstrumentID1",0);
		m_Describe.SetupMember(InstrumentID2,(int)((char *)&InstrumentID2-(char *)this),"InstrumentID2",sizeof(InstrumentID2),"InstrumentID2","InstrumentID2",0);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
		m_Describe.SetupMember(Amount,(int)((char *)&Amount-(char *)this),"Amount",sizeof(Amount),"Amount","Amount",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_ChangeCurrency=0x2149;

///钱包登录随机数
class CWalletNonceField
{
public:
	///交易用户代码
	CLongIndexIDType	UserID;
	///成员代码
	CLongIndexIDType	MemberID;
	///随机数
	CLongIndexIDType	Nonce;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(Nonce,(int)((char *)&Nonce-(char *)this),"Nonce",sizeof(Nonce),"Nonce","Nonce",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_WalletNonce=0x214A;

///钱包登录信息
class CWalletLoginField
{
public:
	///交易用户代码
	CLongIndexIDType	UserID;
	///签名
	CContentType	Signature;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(Signature,(int)((char *)&Signature-(char *)this),"Signature",sizeof(Signature),"Signature","Signature",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_WalletLogin=0x214B;

///报单查询
class CQryOrderField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单本地标识
	CIndexIDType	LocalID;
	///订单号
	CIndexIDType	OrderID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryOrder=0x2201;

///持仓汇总查询
class CQryMemberPositionField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryMemberPosition=0x2206;

///交易单元持仓查询
class CQryPositionField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///持仓大类
	CPositionClassType	PositionClass;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(PositionClass,(int)((char *)&PositionClass-(char *)this),"PositionClass",sizeof(PositionClass),"PositionClass","PositionClass",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryPosition=0x2207;

///交易资金查询
class CQryAccountField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///结算组
	CShortIndexIDType	SettlementGroup;
	///币种
	CCurrencyType	Currency;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryAccount=0x2208;

///标的查询
class CQryInstrumentField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///产品组
	CShortIndexIDType	ProductGroup;
	///标的代码
	CInstrumentIDType	InstrumentID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryInstrument=0x2209;

///标的查询
class CQryCurrencyField
{
public:
	///结算组
	CShortIndexIDType	SettlementGroup;
	///币种
	CCurrencyType	Currency;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryCurrency=0x2210;

///成员查询
class CQryMemberField
{
public:
	///成员类型
	CMemberTypeType	MemberType;
	///成员代码
	CLongIndexIDType	MemberID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberType,(int)((char *)&MemberType-(char *)this),"MemberType",sizeof(MemberType),"MemberType","MemberType",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryMember=0x2212;

///标的价位查询
class CQryMarketOrderField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///买卖方向
	CDirectionType	Direction;
	///深度
	CNumberType	Depth;
	///最小变动价位
	CPriceType	Tick;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(Depth,(int)((char *)&Depth-(char *)this),"Depth",sizeof(Depth),"Depth","Depth",0);
		m_Describe.SetupMember(Tick,(int)((char *)&Tick-(char *)this),"Tick",sizeof(Tick),"Tick","Tick",15);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryMarketOrder=0x2213;

///查询APIKEY
class CQryAPIKeyField
{
public:
	///交易用户代码
	CLongIndexIDType	UserID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryAPIKey=0x2214;

///查询保证金率
class CQryMarginRateField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///资金账号
	CAccountIDType	AccountID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryMarginRate=0x2215;

///查询手续费率
class CQryFeeField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///成交角色
	CMatchRoleType	MatchRole;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(MatchRole,(int)((char *)&MatchRole-(char *)this),"MatchRole",sizeof(MatchRole),"MatchRole","MatchRole",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryFee=0x2215;

///查询触发订单
class CQryTriggerOrderField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单本地标识
	CIndexIDType	LocalID;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryTriggerOrder=0x2216;

///查询杠杆倍数
class CQryLeverageField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///持仓多空方向
	CPosiDirectionType	PosiDirection;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryLeverage=0x2217;

///查询行情
class CQryMarketDataField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///产品组
	CShortIndexIDType	ProductGroup;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///开始序号
	CNumberType	From;
	///限制数量
	CNumberType	Limit;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(From,(int)((char *)&From-(char *)this),"From",sizeof(From),"From","From",0);
		m_Describe.SetupMember(Limit,(int)((char *)&Limit-(char *)this),"Limit",sizeof(Limit),"Limit","Limit",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryMarketData=0x2218;

///查询成交
class CQryTradeField
{
public:
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///限制数量
	CNumberType	Limit;
	///请求此 ID 之前（更旧的数据）的分页内容
	CIndexIDType	Before;
	///请求此 ID 之后（更新的数据）的分页内容
	CIndexIDType	After;
	///起始时间
	CMilliSecondsType	StartTime;
	///结束时间
	CMilliSecondsType	EndTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Limit,(int)((char *)&Limit-(char *)this),"Limit",sizeof(Limit),"Limit","Limit",0);
		m_Describe.SetupMember(Before,(int)((char *)&Before-(char *)this),"Before",sizeof(Before),"Before","Before",0);
		m_Describe.SetupMember(After,(int)((char *)&After-(char *)this),"After",sizeof(After),"After","After",0);
		m_Describe.SetupMember(StartTime,(int)((char *)&StartTime-(char *)this),"StartTime",sizeof(StartTime),"StartTime","StartTime",3);
		m_Describe.SetupMember(EndTime,(int)((char *)&EndTime-(char *)this),"EndTime",sizeof(EndTime),"EndTime","EndTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryTrade=0x2219;

///查询历史委托
class CQryFinishOrderField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///资金账号
	CAccountIDType	AccountID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单状态
	COrderStatusType	OrderStatus;
	///限制数量
	CNumberType	Limit;
	///请求此 ID 之前（更旧的数据）的分页内容
	CIndexIDType	Before;
	///请求此 ID 之后（更新的数据）的分页内容
	CIndexIDType	After;
	///起始时间
	CMilliSecondsType	StartTime;
	///结束时间
	CMilliSecondsType	EndTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OrderStatus,(int)((char *)&OrderStatus-(char *)this),"OrderStatus",sizeof(OrderStatus),"OrderStatus","OrderStatus",0);
		m_Describe.SetupMember(Limit,(int)((char *)&Limit-(char *)this),"Limit",sizeof(Limit),"Limit","Limit",0);
		m_Describe.SetupMember(Before,(int)((char *)&Before-(char *)this),"Before",sizeof(Before),"Before","Before",0);
		m_Describe.SetupMember(After,(int)((char *)&After-(char *)this),"After",sizeof(After),"After","After",0);
		m_Describe.SetupMember(StartTime,(int)((char *)&StartTime-(char *)this),"StartTime",sizeof(StartTime),"StartTime","StartTime",3);
		m_Describe.SetupMember(EndTime,(int)((char *)&EndTime-(char *)this),"EndTime",sizeof(EndTime),"EndTime","EndTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryFinishOrder=0x221A;

///查询历史条件单
class CQryFinishTriggerOrderField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///资金账号
	CAccountIDType	AccountID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///触发报单状态
	CTriggerStatusType	TriggerStatus;
	///触发的订单类型
	CTriggerOrderTypeType	TriggerOrderType;
	///限制数量
	CNumberType	Limit;
	///请求此 ID 之前（更旧的数据）的分页内容
	CIndexIDType	Before;
	///请求此 ID 之后（更新的数据）的分页内容
	CIndexIDType	After;
	///起始时间
	CMilliSecondsType	StartTime;
	///结束时间
	CMilliSecondsType	EndTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(TriggerStatus,(int)((char *)&TriggerStatus-(char *)this),"TriggerStatus",sizeof(TriggerStatus),"TriggerStatus","TriggerStatus",0);
		m_Describe.SetupMember(TriggerOrderType,(int)((char *)&TriggerOrderType-(char *)this),"TriggerOrderType",sizeof(TriggerOrderType),"TriggerOrderType","TriggerOrderType",0);
		m_Describe.SetupMember(Limit,(int)((char *)&Limit-(char *)this),"Limit",sizeof(Limit),"Limit","Limit",0);
		m_Describe.SetupMember(Before,(int)((char *)&Before-(char *)this),"Before",sizeof(Before),"Before","Before",0);
		m_Describe.SetupMember(After,(int)((char *)&After-(char *)this),"After",sizeof(After),"After","After",0);
		m_Describe.SetupMember(StartTime,(int)((char *)&StartTime-(char *)this),"StartTime",sizeof(StartTime),"StartTime","StartTime",3);
		m_Describe.SetupMember(EndTime,(int)((char *)&EndTime-(char *)this),"EndTime",sizeof(EndTime),"EndTime","EndTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryFinishTriggerOrder=0x221B;

///指定杠杠下梯度信息
class CQryWithLeverageField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///持仓多空方向
	CPosiDirectionType	PosiDirection;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///多头杠杆倍数
	CLeverageType	LongLeverage;
	///空头杠杆倍数
	CLeverageType	ShortLeverage;
	///净仓杠杆倍数
	CLeverageType	NetLeverage;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(LongLeverage,(int)((char *)&LongLeverage-(char *)this),"LongLeverage",sizeof(LongLeverage),"LongLeverage","LongLeverage",8);
		m_Describe.SetupMember(ShortLeverage,(int)((char *)&ShortLeverage-(char *)this),"ShortLeverage",sizeof(ShortLeverage),"ShortLeverage","ShortLeverage",8);
		m_Describe.SetupMember(NetLeverage,(int)((char *)&NetLeverage-(char *)this),"NetLeverage",sizeof(NetLeverage),"NetLeverage","NetLeverage",8);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryWithLeverage=0x221C;

///查询K线
class CQryKLineField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///K线周期代码(n[m/h/d/o])
	CBarType	Bar;
	///结束时间
	CMilliSecondsType	EndTime;
	///数量
	CNumberType	Limit;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Bar,(int)((char *)&Bar-(char *)this),"Bar",sizeof(Bar),"Bar","Bar",0);
		m_Describe.SetupMember(EndTime,(int)((char *)&EndTime-(char *)this),"EndTime",sizeof(EndTime),"EndTime","EndTime",3);
		m_Describe.SetupMember(Limit,(int)((char *)&Limit-(char *)this),"Limit",sizeof(Limit),"Limit","Limit",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryKLine=0x221D;

///查询线
class CQryLineField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///K线周期代码(n[m/h/d/o])
	CBarType	Bar;
	///数量
	CNumberType	Limit;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Bar,(int)((char *)&Bar-(char *)this),"Bar",sizeof(Bar),"Bar","Bar",0);
		m_Describe.SetupMember(Limit,(int)((char *)&Limit-(char *)this),"Limit",sizeof(Limit),"Limit","Limit",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryLine=0x221E;

///查询资金流水
class CQryAccountDetailField
{
public:
	///产品组
	CShortIndexIDType	ProductGroup;
	///资金账号
	CAccountIDType	AccountID;
	///成员代码
	CLongIndexIDType	MemberID;
	///币种
	CCurrencyType	Currency;
	///结算组
	CShortIndexIDType	SettlementGroup;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///财务流水类型
	CIndexIDType	Source;
	///限制数量
	CNumberType	Limit;
	///请求此 ID 之前（更旧的数据）的分页内容
	CIndexIDType	Before;
	///请求此 ID 之后（更新的数据）的分页内容
	CIndexIDType	After;
	///起始时间
	CMilliSecondsType	StartTime;
	///结束时间
	CMilliSecondsType	EndTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Source,(int)((char *)&Source-(char *)this),"Source",sizeof(Source),"Source","Source",0);
		m_Describe.SetupMember(Limit,(int)((char *)&Limit-(char *)this),"Limit",sizeof(Limit),"Limit","Limit",0);
		m_Describe.SetupMember(Before,(int)((char *)&Before-(char *)this),"Before",sizeof(Before),"Before","Before",0);
		m_Describe.SetupMember(After,(int)((char *)&After-(char *)this),"After",sizeof(After),"After","After",0);
		m_Describe.SetupMember(StartTime,(int)((char *)&StartTime-(char *)this),"StartTime",sizeof(StartTime),"StartTime","StartTime",3);
		m_Describe.SetupMember(EndTime,(int)((char *)&EndTime-(char *)this),"EndTime",sizeof(EndTime),"EndTime","EndTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QryAccountDetail=0x2220;

///查询结算流水
class CQrySettleDetailField
{
public:
	///产品组
	CShortIndexIDType	ProductGroup;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///结算操作
	CSettleActionTypeType	SettleAction;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(SettleAction,(int)((char *)&SettleAction-(char *)this),"SettleAction",sizeof(SettleAction),"SettleAction","SettleAction",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_QrySettleDetail=0x2220;

///业务配置
class CServiceConfigField
{
public:
	///配置名称
	CNameType	ConfigName;
	///索引1
	CNameType	Index1;
	///索引2
	CNameType	Index2;
	///配置值
	CLongNameType	ConfigValue;
	///备注
	CContentType	Remark;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ConfigName,(int)((char *)&ConfigName-(char *)this),"ConfigName",sizeof(ConfigName),"ConfigName","ConfigName",0);
		m_Describe.SetupMember(Index1,(int)((char *)&Index1-(char *)this),"Index1",sizeof(Index1),"Index1","Index1",0);
		m_Describe.SetupMember(Index2,(int)((char *)&Index2-(char *)this),"Index2",sizeof(Index2),"Index2","Index2",0);
		m_Describe.SetupMember(ConfigValue,(int)((char *)&ConfigValue-(char *)this),"ConfigValue",sizeof(ConfigValue),"ConfigValue","ConfigValue",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_ServiceConfig=0x1001;

///币种信息
class CCurrencyField
{
public:
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///币种
	CCurrencyType	Currency;
	///入金金额
	CMoneyType	Deposit;
	///出金金额
	CMoneyType	Withdraw;
	///静态权益
	CMoneyType	Balance;
	///上次静态权益
	CMoneyType	PreBalance;
	///排序字段
	CNumberType	SortNumber;
	///是否可以出金
	CBoolType	CanReduce;
	///是否允许相关交易
	CBoolType	IsTrading;
	///结算段
	CIndexIDType	SettleSegment;
	///最小单位
	CMoneyType	MoneyTick;
	///价格参考交易所代码
	CExchangeIDType	ExchangeID;
	///价格参考标的代码
	CInstrumentIDType	InstrumentID;
	///借款使用量
	CMoneyType	LoanUse;
	///最大可借比例
	CRatioType	LoanLimit;
	///借款利息
	CRatioType	LoanBorrow;
	///存款利息
	CRatioType	LoanDeposit;
	///利息起点
	CRatioType	LoanBase;
	///利息斜率
	CRatioType	LoanRate;
	///高利息起点
	CRatioType	LoanHighBase;
	///高利息斜率
	CRatioType	LoanHighRate;
	///备注
	CLongNameType	Remark;
	///当前价值
	CMoneyType	CurrValue;
	///抵押比例
	CRatioType	HairCut;
	///最大抵押数额
	CMoneyType	MaxValue;
	///保证金组
	CShortIndexIDType	MarginRateGroup;
	///私有化停止交易线
	CRatioType	PrivateEnd;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
		m_Describe.SetupMember(Deposit,(int)((char *)&Deposit-(char *)this),"Deposit",sizeof(Deposit),"Deposit","Deposit",10);
		m_Describe.SetupMember(Withdraw,(int)((char *)&Withdraw-(char *)this),"Withdraw",sizeof(Withdraw),"Withdraw","Withdraw",10);
		m_Describe.SetupMember(Balance,(int)((char *)&Balance-(char *)this),"Balance",sizeof(Balance),"Balance","Balance",10);
		m_Describe.SetupMember(PreBalance,(int)((char *)&PreBalance-(char *)this),"PreBalance",sizeof(PreBalance),"PreBalance","PreBalance",10);
		m_Describe.SetupMember(SortNumber,(int)((char *)&SortNumber-(char *)this),"SortNumber",sizeof(SortNumber),"SortNumber","SortNumber",0);
		m_Describe.SetupMember(CanReduce,(int)((char *)&CanReduce-(char *)this),"CanReduce",sizeof(CanReduce),"CanReduce","CanReduce",0);
		m_Describe.SetupMember(IsTrading,(int)((char *)&IsTrading-(char *)this),"IsTrading",sizeof(IsTrading),"IsTrading","IsTrading",0);
		m_Describe.SetupMember(SettleSegment,(int)((char *)&SettleSegment-(char *)this),"SettleSegment",sizeof(SettleSegment),"SettleSegment","SettleSegment",0);
		m_Describe.SetupMember(MoneyTick,(int)((char *)&MoneyTick-(char *)this),"MoneyTick",sizeof(MoneyTick),"MoneyTick","MoneyTick",10);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(LoanUse,(int)((char *)&LoanUse-(char *)this),"LoanUse",sizeof(LoanUse),"LoanUse","LoanUse",10);
		m_Describe.SetupMember(LoanLimit,(int)((char *)&LoanLimit-(char *)this),"LoanLimit",sizeof(LoanLimit),"LoanLimit","LoanLimit",10);
		m_Describe.SetupMember(LoanBorrow,(int)((char *)&LoanBorrow-(char *)this),"LoanBorrow",sizeof(LoanBorrow),"LoanBorrow","LoanBorrow",10);
		m_Describe.SetupMember(LoanDeposit,(int)((char *)&LoanDeposit-(char *)this),"LoanDeposit",sizeof(LoanDeposit),"LoanDeposit","LoanDeposit",10);
		m_Describe.SetupMember(LoanBase,(int)((char *)&LoanBase-(char *)this),"LoanBase",sizeof(LoanBase),"LoanBase","LoanBase",10);
		m_Describe.SetupMember(LoanRate,(int)((char *)&LoanRate-(char *)this),"LoanRate",sizeof(LoanRate),"LoanRate","LoanRate",10);
		m_Describe.SetupMember(LoanHighBase,(int)((char *)&LoanHighBase-(char *)this),"LoanHighBase",sizeof(LoanHighBase),"LoanHighBase","LoanHighBase",10);
		m_Describe.SetupMember(LoanHighRate,(int)((char *)&LoanHighRate-(char *)this),"LoanHighRate",sizeof(LoanHighRate),"LoanHighRate","LoanHighRate",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(CurrValue,(int)((char *)&CurrValue-(char *)this),"CurrValue",sizeof(CurrValue),"CurrValue","CurrValue",10);
		m_Describe.SetupMember(HairCut,(int)((char *)&HairCut-(char *)this),"HairCut",sizeof(HairCut),"HairCut","HairCut",10);
		m_Describe.SetupMember(MaxValue,(int)((char *)&MaxValue-(char *)this),"MaxValue",sizeof(MaxValue),"MaxValue","MaxValue",10);
		m_Describe.SetupMember(MarginRateGroup,(int)((char *)&MarginRateGroup-(char *)this),"MarginRateGroup",sizeof(MarginRateGroup),"MarginRateGroup","MarginRateGroup",0);
		m_Describe.SetupMember(PrivateEnd,(int)((char *)&PrivateEnd-(char *)this),"PrivateEnd",sizeof(PrivateEnd),"PrivateEnd","PrivateEnd",10);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Currency=0x1108;

///市场行情
class CMarketDataField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///产品组
	CShortIndexIDType	ProductGroup;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///上次收盘
	CPriceType	PreClosePrice;
	///上次结算价
	CPriceType	PreSettlementPrice;
	///上次持仓量
	CVolumeType	PreOpenInterest;
	///上次持仓费按比例
	CRatioType	PrePositionFeeRate;
	///收盘价
	CPriceType	ClosePrice;
	///收取持仓费用时间
	CMilliSecondsType	PositionFeeTime;
	///结算时间
	CMilliSecondsType	SettlementTime;
	///最高限价
	CPriceType	UpperLimitPrice;
	///最低限价
	CPriceType	LowerLimitPrice;
	///基础标的价格
	CPriceType	UnderlyingPrice;
	///标记价格
	CPriceType	MarkedPrice;
	///预计持仓费按比例
	CRatioType	PositionFeeRate;
	///当日最高价
	CPriceType	HighestPrice;
	///当日最低价
	CPriceType	LowestPrice;
	///最新价
	CPriceType	LastPrice;
	///当日数量
	CCollectType	Volume;
	///当日成交金额
	CCollectType	Turnover;
	///持仓量
	CVolumeType	OpenInterest;
	///预估结算价
	CPriceType	SettlementPrice;
	///当日开盘价
	CPriceType	OpenPrice;
	///行情发布状态
	CBoolType	IsPubMarketData;
	///标的交易状态
	CInstrumentStatusType	InstrumentStatus;
	///备注
	CLongNameType	Remark;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///标记价格汇总
	CTotalType	MarkedPriceTotal;
	///标记价格权重
	CTotalType	MarkedPriceWeight;
	///25档行情最高价
	CPriceType	L25UpperPrice;
	///25档行情最低价
	CPriceType	L25LowerPrice;
	///25档行情最高价1
	CPriceType	L25UpperPrice1;
	///25档行情最低价1
	CPriceType	L25LowerPrice1;
	///25档行情最高价2
	CPriceType	L25UpperPrice2;
	///25档行情最低价2
	CPriceType	L25LowerPrice2;
	///申买价一
	CPriceType	BidPrice1;
	///申买量一
	CVolumeType	BidVolume1;
	///申卖价一
	CPriceType	AskPrice1;
	///申卖量一
	CVolumeType	AskVolume1;
	///理论价
	CPriceType	TheoryPrice;
	///24小时最高价
	CPriceType	HighestPrice24;
	///24小时最低价
	CPriceType	LowestPrice24;
	///24小时数量
	CCollectType	Volume24;
	///24小时成交金额
	CCollectType	Turnover24;
	///24小时开盘价
	CPriceType	OpenPrice24;
	///每秒更新的次数
	CNumberType	Frequency;
	///插入时间
	CMilliSecondsType	InsertTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(PreClosePrice,(int)((char *)&PreClosePrice-(char *)this),"PreClosePrice",sizeof(PreClosePrice),"PreClosePrice","PreClosePrice",15);
		m_Describe.SetupMember(PreSettlementPrice,(int)((char *)&PreSettlementPrice-(char *)this),"PreSettlementPrice",sizeof(PreSettlementPrice),"PreSettlementPrice","PreSettlementPrice",15);
		m_Describe.SetupMember(PreOpenInterest,(int)((char *)&PreOpenInterest-(char *)this),"PreOpenInterest",sizeof(PreOpenInterest),"PreOpenInterest","PreOpenInterest",10);
		m_Describe.SetupMember(PrePositionFeeRate,(int)((char *)&PrePositionFeeRate-(char *)this),"PrePositionFeeRate",sizeof(PrePositionFeeRate),"PrePositionFeeRate","PrePositionFeeRate",10);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(PositionFeeTime,(int)((char *)&PositionFeeTime-(char *)this),"PositionFeeTime",sizeof(PositionFeeTime),"PositionFeeTime","PositionFeeTime",3);
		m_Describe.SetupMember(SettlementTime,(int)((char *)&SettlementTime-(char *)this),"SettlementTime",sizeof(SettlementTime),"SettlementTime","SettlementTime",3);
		m_Describe.SetupMember(UpperLimitPrice,(int)((char *)&UpperLimitPrice-(char *)this),"UpperLimitPrice",sizeof(UpperLimitPrice),"UpperLimitPrice","UpperLimitPrice",15);
		m_Describe.SetupMember(LowerLimitPrice,(int)((char *)&LowerLimitPrice-(char *)this),"LowerLimitPrice",sizeof(LowerLimitPrice),"LowerLimitPrice","LowerLimitPrice",15);
		m_Describe.SetupMember(UnderlyingPrice,(int)((char *)&UnderlyingPrice-(char *)this),"UnderlyingPrice",sizeof(UnderlyingPrice),"UnderlyingPrice","UnderlyingPrice",15);
		m_Describe.SetupMember(MarkedPrice,(int)((char *)&MarkedPrice-(char *)this),"MarkedPrice",sizeof(MarkedPrice),"MarkedPrice","MarkedPrice",15);
		m_Describe.SetupMember(PositionFeeRate,(int)((char *)&PositionFeeRate-(char *)this),"PositionFeeRate",sizeof(PositionFeeRate),"PositionFeeRate","PositionFeeRate",10);
		m_Describe.SetupMember(HighestPrice,(int)((char *)&HighestPrice-(char *)this),"HighestPrice",sizeof(HighestPrice),"HighestPrice","HighestPrice",15);
		m_Describe.SetupMember(LowestPrice,(int)((char *)&LowestPrice-(char *)this),"LowestPrice",sizeof(LowestPrice),"LowestPrice","LowestPrice",15);
		m_Describe.SetupMember(LastPrice,(int)((char *)&LastPrice-(char *)this),"LastPrice",sizeof(LastPrice),"LastPrice","LastPrice",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",8);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",8);
		m_Describe.SetupMember(OpenInterest,(int)((char *)&OpenInterest-(char *)this),"OpenInterest",sizeof(OpenInterest),"OpenInterest","OpenInterest",10);
		m_Describe.SetupMember(SettlementPrice,(int)((char *)&SettlementPrice-(char *)this),"SettlementPrice",sizeof(SettlementPrice),"SettlementPrice","SettlementPrice",15);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(IsPubMarketData,(int)((char *)&IsPubMarketData-(char *)this),"IsPubMarketData",sizeof(IsPubMarketData),"IsPubMarketData","IsPubMarketData",0);
		m_Describe.SetupMember(InstrumentStatus,(int)((char *)&InstrumentStatus-(char *)this),"InstrumentStatus",sizeof(InstrumentStatus),"InstrumentStatus","InstrumentStatus",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(MarkedPriceTotal,(int)((char *)&MarkedPriceTotal-(char *)this),"MarkedPriceTotal",sizeof(MarkedPriceTotal),"MarkedPriceTotal","MarkedPriceTotal",6);
		m_Describe.SetupMember(MarkedPriceWeight,(int)((char *)&MarkedPriceWeight-(char *)this),"MarkedPriceWeight",sizeof(MarkedPriceWeight),"MarkedPriceWeight","MarkedPriceWeight",6);
		m_Describe.SetupMember(L25UpperPrice,(int)((char *)&L25UpperPrice-(char *)this),"L25UpperPrice",sizeof(L25UpperPrice),"L25UpperPrice","L25UpperPrice",15);
		m_Describe.SetupMember(L25LowerPrice,(int)((char *)&L25LowerPrice-(char *)this),"L25LowerPrice",sizeof(L25LowerPrice),"L25LowerPrice","L25LowerPrice",15);
		m_Describe.SetupMember(L25UpperPrice1,(int)((char *)&L25UpperPrice1-(char *)this),"L25UpperPrice1",sizeof(L25UpperPrice1),"L25UpperPrice1","L25UpperPrice1",15);
		m_Describe.SetupMember(L25LowerPrice1,(int)((char *)&L25LowerPrice1-(char *)this),"L25LowerPrice1",sizeof(L25LowerPrice1),"L25LowerPrice1","L25LowerPrice1",15);
		m_Describe.SetupMember(L25UpperPrice2,(int)((char *)&L25UpperPrice2-(char *)this),"L25UpperPrice2",sizeof(L25UpperPrice2),"L25UpperPrice2","L25UpperPrice2",15);
		m_Describe.SetupMember(L25LowerPrice2,(int)((char *)&L25LowerPrice2-(char *)this),"L25LowerPrice2",sizeof(L25LowerPrice2),"L25LowerPrice2","L25LowerPrice2",15);
		m_Describe.SetupMember(BidPrice1,(int)((char *)&BidPrice1-(char *)this),"BidPrice1",sizeof(BidPrice1),"BidPrice1","BidPrice1",15);
		m_Describe.SetupMember(BidVolume1,(int)((char *)&BidVolume1-(char *)this),"BidVolume1",sizeof(BidVolume1),"BidVolume1","BidVolume1",10);
		m_Describe.SetupMember(AskPrice1,(int)((char *)&AskPrice1-(char *)this),"AskPrice1",sizeof(AskPrice1),"AskPrice1","AskPrice1",15);
		m_Describe.SetupMember(AskVolume1,(int)((char *)&AskVolume1-(char *)this),"AskVolume1",sizeof(AskVolume1),"AskVolume1","AskVolume1",10);
		m_Describe.SetupMember(TheoryPrice,(int)((char *)&TheoryPrice-(char *)this),"TheoryPrice",sizeof(TheoryPrice),"TheoryPrice","TheoryPrice",15);
		m_Describe.SetupMember(HighestPrice24,(int)((char *)&HighestPrice24-(char *)this),"HighestPrice24",sizeof(HighestPrice24),"HighestPrice24","HighestPrice24",15);
		m_Describe.SetupMember(LowestPrice24,(int)((char *)&LowestPrice24-(char *)this),"LowestPrice24",sizeof(LowestPrice24),"LowestPrice24","LowestPrice24",15);
		m_Describe.SetupMember(Volume24,(int)((char *)&Volume24-(char *)this),"Volume24",sizeof(Volume24),"Volume24","Volume24",8);
		m_Describe.SetupMember(Turnover24,(int)((char *)&Turnover24-(char *)this),"Turnover24",sizeof(Turnover24),"Turnover24","Turnover24",8);
		m_Describe.SetupMember(OpenPrice24,(int)((char *)&OpenPrice24-(char *)this),"OpenPrice24",sizeof(OpenPrice24),"OpenPrice24","OpenPrice24",15);
		m_Describe.SetupMember(Frequency,(int)((char *)&Frequency-(char *)this),"Frequency",sizeof(Frequency),"Frequency","Frequency",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_MarketData=0x1101;

///交易标的
class CInstrumentField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///标的名称
	CLongNameType	InstrumentName;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///产品组
	CShortIndexIDType	ProductGroup;
	///基础标的代码
	CInstrumentIDType	UnderlyingID;
	///产品大类
	CProductClassType	ProductClass;
	///产品类型
	CProductTypeType	ProductType;
	///执行价
	CPriceType	StrikePrice;
	///期权类型
	COptionsTypeType	OptionsType;
	///数量乘数
	CRatioType	VolumeMultiple;
	///启动时间
	CMilliSecondsType	StartTime;
	///到期时间
	CMilliSecondsType	ExpireTime;
	///计价币种
	CCurrencyType	PriceCurrency;
	///清算币种
	CCurrencyType	ClearCurrency;
	///是否反向标的
	CBoolType	IsInverse;
	///标的基础货币
	CCurrencyType	BaseCurrency;
	///保证金价格类型
	CMarginPriceTypeType	MarginPriceType;
	///挂牌基准价
	CPriceType	BasisPrice;
	///最小下单量
	CVolumeType	MinOrderVolume;
	///最小下单额
	CVolumeType	MinOrderCost;
	///最大下单量
	CVolumeType	MaxOrderVolume;
	///最小变动价位
	CPriceType	PriceTick;
	///最小变动量
	CVolumeType	VolumeTick;
	///前端行情显示最小变动量(服务端不用)
	CVolumeType	ShowVolumeTick;
	///交易模式
	CTradingModelType	TradingModel;
	///每日开始交易时间
	CShortContentType	DayStartTime;
	///排序字段
	CNumberType	SortNumber;
	///备注
	CContentType	Remark;
	///默认杠杆倍数
	CLeverageType	DefaultLeverage;
	///取值方式
	CValueModeType	PriceLimitValueMode;
	///限价价格类型
	CLimitPriceTypeType	PriceLimitPrice;
	///上限
	CPriceType	PriceLimitUpperValue;
	///下限
	CPriceType	PriceLimitLowerValue;
	///保证金组
	CShortIndexIDType	MarginRateGroup;
	///差价组
	CShortIndexIDType	CFDGroup;
	///手续费组
	CShortIndexIDType	FeeGroup;
	///权限组
	CShortIndexIDType	TradingRightGroup;
	///最大持仓量
	CVolumeType	MaxOpenInterest;
	///资金费用组
	CShortIndexIDType	FundingRateGroup;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价)
	CNumberType	MarkedPriceTimes;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(InstrumentName,(int)((char *)&InstrumentName-(char *)this),"InstrumentName",sizeof(InstrumentName),"InstrumentName","InstrumentName",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(ProductGroup,(int)((char *)&ProductGroup-(char *)this),"ProductGroup",sizeof(ProductGroup),"ProductGroup","ProductGroup",0);
		m_Describe.SetupMember(UnderlyingID,(int)((char *)&UnderlyingID-(char *)this),"UnderlyingID",sizeof(UnderlyingID),"UnderlyingID","UnderlyingID",0);
		m_Describe.SetupMember(ProductClass,(int)((char *)&ProductClass-(char *)this),"ProductClass",sizeof(ProductClass),"ProductClass","ProductClass",0);
		m_Describe.SetupMember(ProductType,(int)((char *)&ProductType-(char *)this),"ProductType",sizeof(ProductType),"ProductType","ProductType",0);
		m_Describe.SetupMember(StrikePrice,(int)((char *)&StrikePrice-(char *)this),"StrikePrice",sizeof(StrikePrice),"StrikePrice","StrikePrice",15);
		m_Describe.SetupMember(OptionsType,(int)((char *)&OptionsType-(char *)this),"OptionsType",sizeof(OptionsType),"OptionsType","OptionsType",0);
		m_Describe.SetupMember(VolumeMultiple,(int)((char *)&VolumeMultiple-(char *)this),"VolumeMultiple",sizeof(VolumeMultiple),"VolumeMultiple","VolumeMultiple",10);
		m_Describe.SetupMember(StartTime,(int)((char *)&StartTime-(char *)this),"StartTime",sizeof(StartTime),"StartTime","StartTime",3);
		m_Describe.SetupMember(ExpireTime,(int)((char *)&ExpireTime-(char *)this),"ExpireTime",sizeof(ExpireTime),"ExpireTime","ExpireTime",3);
		m_Describe.SetupMember(PriceCurrency,(int)((char *)&PriceCurrency-(char *)this),"PriceCurrency",sizeof(PriceCurrency),"PriceCurrency","PriceCurrency",0);
		m_Describe.SetupMember(ClearCurrency,(int)((char *)&ClearCurrency-(char *)this),"ClearCurrency",sizeof(ClearCurrency),"ClearCurrency","ClearCurrency",0);
		m_Describe.SetupMember(IsInverse,(int)((char *)&IsInverse-(char *)this),"IsInverse",sizeof(IsInverse),"IsInverse","IsInverse",0);
		m_Describe.SetupMember(BaseCurrency,(int)((char *)&BaseCurrency-(char *)this),"BaseCurrency",sizeof(BaseCurrency),"BaseCurrency","BaseCurrency",0);
		m_Describe.SetupMember(MarginPriceType,(int)((char *)&MarginPriceType-(char *)this),"MarginPriceType",sizeof(MarginPriceType),"MarginPriceType","MarginPriceType",0);
		m_Describe.SetupMember(BasisPrice,(int)((char *)&BasisPrice-(char *)this),"BasisPrice",sizeof(BasisPrice),"BasisPrice","BasisPrice",15);
		m_Describe.SetupMember(MinOrderVolume,(int)((char *)&MinOrderVolume-(char *)this),"MinOrderVolume",sizeof(MinOrderVolume),"MinOrderVolume","MinOrderVolume",10);
		m_Describe.SetupMember(MinOrderCost,(int)((char *)&MinOrderCost-(char *)this),"MinOrderCost",sizeof(MinOrderCost),"MinOrderCost","MinOrderCost",10);
		m_Describe.SetupMember(MaxOrderVolume,(int)((char *)&MaxOrderVolume-(char *)this),"MaxOrderVolume",sizeof(MaxOrderVolume),"MaxOrderVolume","MaxOrderVolume",10);
		m_Describe.SetupMember(PriceTick,(int)((char *)&PriceTick-(char *)this),"PriceTick",sizeof(PriceTick),"PriceTick","PriceTick",15);
		m_Describe.SetupMember(VolumeTick,(int)((char *)&VolumeTick-(char *)this),"VolumeTick",sizeof(VolumeTick),"VolumeTick","VolumeTick",10);
		m_Describe.SetupMember(ShowVolumeTick,(int)((char *)&ShowVolumeTick-(char *)this),"ShowVolumeTick",sizeof(ShowVolumeTick),"ShowVolumeTick","ShowVolumeTick",10);
		m_Describe.SetupMember(TradingModel,(int)((char *)&TradingModel-(char *)this),"TradingModel",sizeof(TradingModel),"TradingModel","TradingModel",0);
		m_Describe.SetupMember(DayStartTime,(int)((char *)&DayStartTime-(char *)this),"DayStartTime",sizeof(DayStartTime),"DayStartTime","DayStartTime",0);
		m_Describe.SetupMember(SortNumber,(int)((char *)&SortNumber-(char *)this),"SortNumber",sizeof(SortNumber),"SortNumber","SortNumber",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(DefaultLeverage,(int)((char *)&DefaultLeverage-(char *)this),"DefaultLeverage",sizeof(DefaultLeverage),"DefaultLeverage","DefaultLeverage",8);
		m_Describe.SetupMember(PriceLimitValueMode,(int)((char *)&PriceLimitValueMode-(char *)this),"PriceLimitValueMode",sizeof(PriceLimitValueMode),"PriceLimitValueMode","PriceLimitValueMode",0);
		m_Describe.SetupMember(PriceLimitPrice,(int)((char *)&PriceLimitPrice-(char *)this),"PriceLimitPrice",sizeof(PriceLimitPrice),"PriceLimitPrice","PriceLimitPrice",0);
		m_Describe.SetupMember(PriceLimitUpperValue,(int)((char *)&PriceLimitUpperValue-(char *)this),"PriceLimitUpperValue",sizeof(PriceLimitUpperValue),"PriceLimitUpperValue","PriceLimitUpperValue",15);
		m_Describe.SetupMember(PriceLimitLowerValue,(int)((char *)&PriceLimitLowerValue-(char *)this),"PriceLimitLowerValue",sizeof(PriceLimitLowerValue),"PriceLimitLowerValue","PriceLimitLowerValue",15);
		m_Describe.SetupMember(MarginRateGroup,(int)((char *)&MarginRateGroup-(char *)this),"MarginRateGroup",sizeof(MarginRateGroup),"MarginRateGroup","MarginRateGroup",0);
		m_Describe.SetupMember(CFDGroup,(int)((char *)&CFDGroup-(char *)this),"CFDGroup",sizeof(CFDGroup),"CFDGroup","CFDGroup",0);
		m_Describe.SetupMember(FeeGroup,(int)((char *)&FeeGroup-(char *)this),"FeeGroup",sizeof(FeeGroup),"FeeGroup","FeeGroup",0);
		m_Describe.SetupMember(TradingRightGroup,(int)((char *)&TradingRightGroup-(char *)this),"TradingRightGroup",sizeof(TradingRightGroup),"TradingRightGroup","TradingRightGroup",0);
		m_Describe.SetupMember(MaxOpenInterest,(int)((char *)&MaxOpenInterest-(char *)this),"MaxOpenInterest",sizeof(MaxOpenInterest),"MaxOpenInterest","MaxOpenInterest",10);
		m_Describe.SetupMember(FundingRateGroup,(int)((char *)&FundingRateGroup-(char *)this),"FundingRateGroup",sizeof(FundingRateGroup),"FundingRateGroup","FundingRateGroup",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(MarkedPriceTimes,(int)((char *)&MarkedPriceTimes-(char *)this),"MarkedPriceTimes",sizeof(MarkedPriceTimes),"MarkedPriceTimes","MarkedPriceTimes",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Instrument=0x1102;

///当前时间
class CCurrentTimeField
{
public:
	///系统名
	CShortIndexIDType	SystemID;
	///本地日期
	CDateType	ZoneDate;
	///本地时间
	CTimeType	ZoneTime;
	///本地天数
	CDateType	ZoneDays;
	///当前时间（秒）
	CMilliSecondsType	CurrTime;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间（毫秒）
	CMilliSecondsType	UpdateTime;
	///本地时区
	CNumberType	TimeZone;
	///最新最新成交序列号
	CIndexIDType	LastTradeID;
	///最新订单序列号
	CIndexIDType	LastOrderID;
	///最新资金明细序列号
	CIndexIDType	LastAccountDetailID;
	///最新排序序列号
	CLargeNumberType	LastTimeSortNo;
	///最新事务序列号
	CLargeNumberType	LastBusinessNo;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(SystemID,(int)((char *)&SystemID-(char *)this),"SystemID",sizeof(SystemID),"SystemID","SystemID",0);
		m_Describe.SetupMember(ZoneDate,(int)((char *)&ZoneDate-(char *)this),"ZoneDate",sizeof(ZoneDate),"ZoneDate","ZoneDate",0);
		m_Describe.SetupMember(ZoneTime,(int)((char *)&ZoneTime-(char *)this),"ZoneTime",sizeof(ZoneTime),"ZoneTime","ZoneTime",0);
		m_Describe.SetupMember(ZoneDays,(int)((char *)&ZoneDays-(char *)this),"ZoneDays",sizeof(ZoneDays),"ZoneDays","ZoneDays",0);
		m_Describe.SetupMember(CurrTime,(int)((char *)&CurrTime-(char *)this),"CurrTime",sizeof(CurrTime),"CurrTime","CurrTime",3);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(TimeZone,(int)((char *)&TimeZone-(char *)this),"TimeZone",sizeof(TimeZone),"TimeZone","TimeZone",0);
		m_Describe.SetupMember(LastTradeID,(int)((char *)&LastTradeID-(char *)this),"LastTradeID",sizeof(LastTradeID),"LastTradeID","LastTradeID",0);
		m_Describe.SetupMember(LastOrderID,(int)((char *)&LastOrderID-(char *)this),"LastOrderID",sizeof(LastOrderID),"LastOrderID","LastOrderID",0);
		m_Describe.SetupMember(LastAccountDetailID,(int)((char *)&LastAccountDetailID-(char *)this),"LastAccountDetailID",sizeof(LastAccountDetailID),"LastAccountDetailID","LastAccountDetailID",0);
		m_Describe.SetupMember(LastTimeSortNo,(int)((char *)&LastTimeSortNo-(char *)this),"LastTimeSortNo",sizeof(LastTimeSortNo),"LastTimeSortNo","LastTimeSortNo",0);
		m_Describe.SetupMember(LastBusinessNo,(int)((char *)&LastBusinessNo-(char *)this),"LastBusinessNo",sizeof(LastBusinessNo),"LastBusinessNo","LastBusinessNo",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_CurrentTime=0x1002;

///成员
class CMemberField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///成员类型
	CMemberTypeType	MemberType;
	///等级
	CShortIndexIDType	Grade;
	///优先权
	CNumberType	Priority;
	///手续费等级
	CShortIndexIDType	FeeGrade;
	///保证金等级
	CShortIndexIDType	MarginRateGrade;
	///权限等级
	CShortIndexIDType	TradingRightGrade;
	///差价等级
	CShortIndexIDType	CFDGrade;
	///流动性等级
	CShortIndexIDType	LiquidityGrade;
	///风控类型
	CRiskLevelType	RiskLevel;
	///持仓类型
	CPositionTypeType	PositionType;
	///触发订单个数
	CNumberType	TriggerOrders;
	///未成交订单数
	CNumberType	OpenOrders;
	///用户持仓个数
	CNumberType	OpenPositions;
	///最大本地标识
	CIndexIDType	MaxLocalID;
	///下游系统同步类型
	CSinkTypeType	SinkType;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///
	CShortIndexIDType	Region;
	///作为手续费的抵扣优惠比例
	CRatioType	FeeDeduceDisCount;
	///作为手续费的抵扣比例
	CCurrencyType	FeeDeduceCurrency;
	///是否需要借钱交易
	CBoolType	CanBorrow;
	///备注
	CNameType	Remark;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(MemberType,(int)((char *)&MemberType-(char *)this),"MemberType",sizeof(MemberType),"MemberType","MemberType",0);
		m_Describe.SetupMember(Grade,(int)((char *)&Grade-(char *)this),"Grade",sizeof(Grade),"Grade","Grade",0);
		m_Describe.SetupMember(Priority,(int)((char *)&Priority-(char *)this),"Priority",sizeof(Priority),"Priority","Priority",0);
		m_Describe.SetupMember(FeeGrade,(int)((char *)&FeeGrade-(char *)this),"FeeGrade",sizeof(FeeGrade),"FeeGrade","FeeGrade",0);
		m_Describe.SetupMember(MarginRateGrade,(int)((char *)&MarginRateGrade-(char *)this),"MarginRateGrade",sizeof(MarginRateGrade),"MarginRateGrade","MarginRateGrade",0);
		m_Describe.SetupMember(TradingRightGrade,(int)((char *)&TradingRightGrade-(char *)this),"TradingRightGrade",sizeof(TradingRightGrade),"TradingRightGrade","TradingRightGrade",0);
		m_Describe.SetupMember(CFDGrade,(int)((char *)&CFDGrade-(char *)this),"CFDGrade",sizeof(CFDGrade),"CFDGrade","CFDGrade",0);
		m_Describe.SetupMember(LiquidityGrade,(int)((char *)&LiquidityGrade-(char *)this),"LiquidityGrade",sizeof(LiquidityGrade),"LiquidityGrade","LiquidityGrade",0);
		m_Describe.SetupMember(RiskLevel,(int)((char *)&RiskLevel-(char *)this),"RiskLevel",sizeof(RiskLevel),"RiskLevel","RiskLevel",0);
		m_Describe.SetupMember(PositionType,(int)((char *)&PositionType-(char *)this),"PositionType",sizeof(PositionType),"PositionType","PositionType",0);
		m_Describe.SetupMember(TriggerOrders,(int)((char *)&TriggerOrders-(char *)this),"TriggerOrders",sizeof(TriggerOrders),"TriggerOrders","TriggerOrders",0);
		m_Describe.SetupMember(OpenOrders,(int)((char *)&OpenOrders-(char *)this),"OpenOrders",sizeof(OpenOrders),"OpenOrders","OpenOrders",0);
		m_Describe.SetupMember(OpenPositions,(int)((char *)&OpenPositions-(char *)this),"OpenPositions",sizeof(OpenPositions),"OpenPositions","OpenPositions",0);
		m_Describe.SetupMember(MaxLocalID,(int)((char *)&MaxLocalID-(char *)this),"MaxLocalID",sizeof(MaxLocalID),"MaxLocalID","MaxLocalID",0);
		m_Describe.SetupMember(SinkType,(int)((char *)&SinkType-(char *)this),"SinkType",sizeof(SinkType),"SinkType","SinkType",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(Region,(int)((char *)&Region-(char *)this),"Region",sizeof(Region),"Region","Region",0);
		m_Describe.SetupMember(FeeDeduceDisCount,(int)((char *)&FeeDeduceDisCount-(char *)this),"FeeDeduceDisCount",sizeof(FeeDeduceDisCount),"FeeDeduceDisCount","FeeDeduceDisCount",10);
		m_Describe.SetupMember(FeeDeduceCurrency,(int)((char *)&FeeDeduceCurrency-(char *)this),"FeeDeduceCurrency",sizeof(FeeDeduceCurrency),"FeeDeduceCurrency","FeeDeduceCurrency",0);
		m_Describe.SetupMember(CanBorrow,(int)((char *)&CanBorrow-(char *)this),"CanBorrow",sizeof(CanBorrow),"CanBorrow","CanBorrow",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Member=0x1006;

///用户会话
class CUserSessionField
{
public:
	///交易用户代码
	CLongIndexIDType	UserID;
	///密码
	CPasswordType	Password;
	///用户端产品信息
	CNameType	UserProductID;
	///Mac地址
	CMacAddressType	MacAddress;
	///终端IP地址
	CIPAddressType	ClientIPAddress;
	///硬盘序列号
	CLongNameType	HDSerialID;
	///授权编码
	CShortContentType	AuthCode;
	///应用编号
	CShortIndexIDType	APPID;
	///令牌
	CShortContentType	Token;
	///备注
	CLongNameType	Remark;
	///接口端产品信息
	CNameType	APIID;
	///登录时间
	CMilliSecondsType	LoginTime;
	///失效时间
	CMilliSecondsType	ExpireTime;
	///允许登录的IP地址
	CContentType	IPAddress;
	///最大本地标识
	CIndexIDType	MaxLocalID;
	///会话编号
	CNumberType	SessionNo;
	///前置编号
	CNumberType	FrontNo;
	///每秒访问的次数限制
	CNumberType	AccessLimit;
	///用户类型
	CUserTypeType	UserType;
	///成员代码
	CLongIndexIDType	MemberID;
	///该秒已经访问的次数
	CNumberType	LimitAccesses;
	///下游系统同步类型
	CSinkTypeType	SinkType;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(Password,(int)((char *)&Password-(char *)this),"Password",sizeof(Password),"Password","Password",0);
		m_Describe.SetupMember(UserProductID,(int)((char *)&UserProductID-(char *)this),"UserProductID",sizeof(UserProductID),"UserProductID","UserProductID",0);
		m_Describe.SetupMember(MacAddress,(int)((char *)&MacAddress-(char *)this),"MacAddress",sizeof(MacAddress),"MacAddress","MacAddress",0);
		m_Describe.SetupMember(ClientIPAddress,(int)((char *)&ClientIPAddress-(char *)this),"ClientIPAddress",sizeof(ClientIPAddress),"ClientIPAddress","ClientIPAddress",0);
		m_Describe.SetupMember(HDSerialID,(int)((char *)&HDSerialID-(char *)this),"HDSerialID",sizeof(HDSerialID),"HDSerialID","HDSerialID",0);
		m_Describe.SetupMember(AuthCode,(int)((char *)&AuthCode-(char *)this),"AuthCode",sizeof(AuthCode),"AuthCode","AuthCode",0);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(Token,(int)((char *)&Token-(char *)this),"Token",sizeof(Token),"Token","Token",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(APIID,(int)((char *)&APIID-(char *)this),"APIID",sizeof(APIID),"APIID","APIID",0);
		m_Describe.SetupMember(LoginTime,(int)((char *)&LoginTime-(char *)this),"LoginTime",sizeof(LoginTime),"LoginTime","LoginTime",3);
		m_Describe.SetupMember(ExpireTime,(int)((char *)&ExpireTime-(char *)this),"ExpireTime",sizeof(ExpireTime),"ExpireTime","ExpireTime",3);
		m_Describe.SetupMember(IPAddress,(int)((char *)&IPAddress-(char *)this),"IPAddress",sizeof(IPAddress),"IPAddress","IPAddress",0);
		m_Describe.SetupMember(MaxLocalID,(int)((char *)&MaxLocalID-(char *)this),"MaxLocalID",sizeof(MaxLocalID),"MaxLocalID","MaxLocalID",0);
		m_Describe.SetupMember(SessionNo,(int)((char *)&SessionNo-(char *)this),"SessionNo",sizeof(SessionNo),"SessionNo","SessionNo",0);
		m_Describe.SetupMember(FrontNo,(int)((char *)&FrontNo-(char *)this),"FrontNo",sizeof(FrontNo),"FrontNo","FrontNo",0);
		m_Describe.SetupMember(AccessLimit,(int)((char *)&AccessLimit-(char *)this),"AccessLimit",sizeof(AccessLimit),"AccessLimit","AccessLimit",0);
		m_Describe.SetupMember(UserType,(int)((char *)&UserType-(char *)this),"UserType",sizeof(UserType),"UserType","UserType",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(LimitAccesses,(int)((char *)&LimitAccesses-(char *)this),"LimitAccesses",sizeof(LimitAccesses),"LimitAccesses","LimitAccesses",0);
		m_Describe.SetupMember(SinkType,(int)((char *)&SinkType-(char *)this),"SinkType",sizeof(SinkType),"SinkType","SinkType",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_UserSession=0x1005;

///最新K线
class CLastKLineField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///K线周期代码(n[m/h/d/o])
	CBarType	Bar;
	///插入时间
	CMilliSecondsType	InsertTime;
	///开盘价
	CPriceType	OpenPrice;
	///最高价
	CPriceType	HighestPrice;
	///最低价
	CPriceType	LowestPrice;
	///收盘价
	CPriceType	ClosePrice;
	///数量
	CCollectType	Volume;
	///成交金额
	CCollectType	Turnover;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Bar,(int)((char *)&Bar-(char *)this),"Bar",sizeof(Bar),"Bar","Bar",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(HighestPrice,(int)((char *)&HighestPrice-(char *)this),"HighestPrice",sizeof(HighestPrice),"HighestPrice","HighestPrice",15);
		m_Describe.SetupMember(LowestPrice,(int)((char *)&LowestPrice-(char *)this),"LowestPrice",sizeof(LowestPrice),"LowestPrice","LowestPrice",15);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",8);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",8);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_LastKLine=0x1103;

///CFD溢价率
class CCFDRateField
{
public:
	///成员代码或组
	CLongIndexIDType	MemberID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单价格类型
	CLongIndexIDType	TriggerOrderType;
	///多头起始溢价率
	CRatioType	RateBegin;
	///多头最大溢价率
	CRatioType	RateEnd;
	///点差比较基准
	CRatioType	CompVolume;
	///点差比较基准率
	CRatioType	CompRate;
	///硬顶率
	CRatioType	HardRate;
	///备注
	CLongNameType	Remark;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(TriggerOrderType,(int)((char *)&TriggerOrderType-(char *)this),"TriggerOrderType",sizeof(TriggerOrderType),"TriggerOrderType","TriggerOrderType",0);
		m_Describe.SetupMember(RateBegin,(int)((char *)&RateBegin-(char *)this),"RateBegin",sizeof(RateBegin),"RateBegin","RateBegin",10);
		m_Describe.SetupMember(RateEnd,(int)((char *)&RateEnd-(char *)this),"RateEnd",sizeof(RateEnd),"RateEnd","RateEnd",10);
		m_Describe.SetupMember(CompVolume,(int)((char *)&CompVolume-(char *)this),"CompVolume",sizeof(CompVolume),"CompVolume","CompVolume",10);
		m_Describe.SetupMember(CompRate,(int)((char *)&CompRate-(char *)this),"CompRate",sizeof(CompRate),"CompRate","CompRate",10);
		m_Describe.SetupMember(HardRate,(int)((char *)&HardRate-(char *)this),"HardRate",sizeof(HardRate),"HardRate","HardRate",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_CFDRate=0x1106;

///交易权限
class CTradingRightField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///买卖方向
	CDirectionType	Direction;
	///交易权限
	CTradingRightType	TradingRight;
	///备注
	CNameType	Remark;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(TradingRight,(int)((char *)&TradingRight-(char *)this),"TradingRight",sizeof(TradingRight),"TradingRight","TradingRight",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_TradingRight=0x1201;

///保证金率
class CMarginRateField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///取值方式
	CValueModeType	ValueMode;
	///保证金数列
	CSequenceTypeType	MarginSequenceType;
	///风险限额数列
	CSequenceTypeType	RiskSequenceType;
	///多头起始保证金率
	CRatioType	LongMarginRatio;
	///多头起始维持保证金率
	CRatioType	LongMaintMarginRatio;
	///多头保证金率步长
	CRatioType	LongMarginStep;
	///多头维持保证金率步长
	CRatioType	LongMaintMarginStep;
	///多头起始风险限额
	CMoneyType	LongRiskLimit;
	///多头风险限额步长
	CMoneyType	LongRiskStep;
	///空头起始保证金率
	CRatioType	ShortMarginRatio;
	///空头起始维持保证金率
	CRatioType	ShortMaintMarginRatio;
	///空头保证金率步长
	CRatioType	ShortMarginStep;
	///空头维持保证金率步长
	CRatioType	ShortMaintMarginStep;
	///空头起始风险限额
	CMoneyType	ShortRiskLimit;
	///空头风险限额步长
	CMoneyType	ShortRiskStep;
	///默认杠杆倍数
	CLeverageType	DefaultLeverage;
	///备注
	CLongNameType	Remark;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(ValueMode,(int)((char *)&ValueMode-(char *)this),"ValueMode",sizeof(ValueMode),"ValueMode","ValueMode",0);
		m_Describe.SetupMember(MarginSequenceType,(int)((char *)&MarginSequenceType-(char *)this),"MarginSequenceType",sizeof(MarginSequenceType),"MarginSequenceType","MarginSequenceType",0);
		m_Describe.SetupMember(RiskSequenceType,(int)((char *)&RiskSequenceType-(char *)this),"RiskSequenceType",sizeof(RiskSequenceType),"RiskSequenceType","RiskSequenceType",0);
		m_Describe.SetupMember(LongMarginRatio,(int)((char *)&LongMarginRatio-(char *)this),"LongMarginRatio",sizeof(LongMarginRatio),"LongMarginRatio","LongMarginRatio",10);
		m_Describe.SetupMember(LongMaintMarginRatio,(int)((char *)&LongMaintMarginRatio-(char *)this),"LongMaintMarginRatio",sizeof(LongMaintMarginRatio),"LongMaintMarginRatio","LongMaintMarginRatio",10);
		m_Describe.SetupMember(LongMarginStep,(int)((char *)&LongMarginStep-(char *)this),"LongMarginStep",sizeof(LongMarginStep),"LongMarginStep","LongMarginStep",10);
		m_Describe.SetupMember(LongMaintMarginStep,(int)((char *)&LongMaintMarginStep-(char *)this),"LongMaintMarginStep",sizeof(LongMaintMarginStep),"LongMaintMarginStep","LongMaintMarginStep",10);
		m_Describe.SetupMember(LongRiskLimit,(int)((char *)&LongRiskLimit-(char *)this),"LongRiskLimit",sizeof(LongRiskLimit),"LongRiskLimit","LongRiskLimit",10);
		m_Describe.SetupMember(LongRiskStep,(int)((char *)&LongRiskStep-(char *)this),"LongRiskStep",sizeof(LongRiskStep),"LongRiskStep","LongRiskStep",10);
		m_Describe.SetupMember(ShortMarginRatio,(int)((char *)&ShortMarginRatio-(char *)this),"ShortMarginRatio",sizeof(ShortMarginRatio),"ShortMarginRatio","ShortMarginRatio",10);
		m_Describe.SetupMember(ShortMaintMarginRatio,(int)((char *)&ShortMaintMarginRatio-(char *)this),"ShortMaintMarginRatio",sizeof(ShortMaintMarginRatio),"ShortMaintMarginRatio","ShortMaintMarginRatio",10);
		m_Describe.SetupMember(ShortMarginStep,(int)((char *)&ShortMarginStep-(char *)this),"ShortMarginStep",sizeof(ShortMarginStep),"ShortMarginStep","ShortMarginStep",10);
		m_Describe.SetupMember(ShortMaintMarginStep,(int)((char *)&ShortMaintMarginStep-(char *)this),"ShortMaintMarginStep",sizeof(ShortMaintMarginStep),"ShortMaintMarginStep","ShortMaintMarginStep",10);
		m_Describe.SetupMember(ShortRiskLimit,(int)((char *)&ShortRiskLimit-(char *)this),"ShortRiskLimit",sizeof(ShortRiskLimit),"ShortRiskLimit","ShortRiskLimit",10);
		m_Describe.SetupMember(ShortRiskStep,(int)((char *)&ShortRiskStep-(char *)this),"ShortRiskStep",sizeof(ShortRiskStep),"ShortRiskStep","ShortRiskStep",10);
		m_Describe.SetupMember(DefaultLeverage,(int)((char *)&DefaultLeverage-(char *)this),"DefaultLeverage",sizeof(DefaultLeverage),"DefaultLeverage","DefaultLeverage",8);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_MarginRate=0x1202;

///手续费率
class CFeeField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///成交角色
	CMatchRoleType	MatchRole;
	///开仓手续费按比例
	CRatioType	OpenFeeRate;
	///开仓手续费按手数
	CRatioType	OpenFeeAmount;
	///平仓手续费按比例
	CRatioType	CloseFeeRate;
	///平仓手续费按手数
	CRatioType	CloseFeeAmount;
	///平今仓手续费按比例
	CRatioType	CloseTodayFeeRate;
	///平今仓手续费按手数
	CRatioType	CloseTodayFeeAmount;
	///备注
	CLongNameType	Remark;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(MatchRole,(int)((char *)&MatchRole-(char *)this),"MatchRole",sizeof(MatchRole),"MatchRole","MatchRole",0);
		m_Describe.SetupMember(OpenFeeRate,(int)((char *)&OpenFeeRate-(char *)this),"OpenFeeRate",sizeof(OpenFeeRate),"OpenFeeRate","OpenFeeRate",10);
		m_Describe.SetupMember(OpenFeeAmount,(int)((char *)&OpenFeeAmount-(char *)this),"OpenFeeAmount",sizeof(OpenFeeAmount),"OpenFeeAmount","OpenFeeAmount",10);
		m_Describe.SetupMember(CloseFeeRate,(int)((char *)&CloseFeeRate-(char *)this),"CloseFeeRate",sizeof(CloseFeeRate),"CloseFeeRate","CloseFeeRate",10);
		m_Describe.SetupMember(CloseFeeAmount,(int)((char *)&CloseFeeAmount-(char *)this),"CloseFeeAmount",sizeof(CloseFeeAmount),"CloseFeeAmount","CloseFeeAmount",10);
		m_Describe.SetupMember(CloseTodayFeeRate,(int)((char *)&CloseTodayFeeRate-(char *)this),"CloseTodayFeeRate",sizeof(CloseTodayFeeRate),"CloseTodayFeeRate","CloseTodayFeeRate",10);
		m_Describe.SetupMember(CloseTodayFeeAmount,(int)((char *)&CloseTodayFeeAmount-(char *)this),"CloseTodayFeeAmount",sizeof(CloseTodayFeeAmount),"CloseTodayFeeAmount","CloseTodayFeeAmount",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Fee=0x1203;

///流动性设置表
class CLiquidityField
{
public:
	///流动性等级
	CShortIndexIDType	LiquidityGrade;
	///标的代码
	CLongIndexIDType	LiquidityGroup;
	///基于的价格
	CLiquidityPriceTypeType	BasePriceType;
	///起始量
	CVolumeType	VolumeBegin;
	///买开流动性比例
	CRatioType	BuyOpen;
	///买平流动性比例
	CRatioType	BuyClose;
	///卖开流动性比例
	CRatioType	SellOpen;
	///卖平流动性比例
	CRatioType	SellClose;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(LiquidityGrade,(int)((char *)&LiquidityGrade-(char *)this),"LiquidityGrade",sizeof(LiquidityGrade),"LiquidityGrade","LiquidityGrade",0);
		m_Describe.SetupMember(LiquidityGroup,(int)((char *)&LiquidityGroup-(char *)this),"LiquidityGroup",sizeof(LiquidityGroup),"LiquidityGroup","LiquidityGroup",0);
		m_Describe.SetupMember(BasePriceType,(int)((char *)&BasePriceType-(char *)this),"BasePriceType",sizeof(BasePriceType),"BasePriceType","BasePriceType",0);
		m_Describe.SetupMember(VolumeBegin,(int)((char *)&VolumeBegin-(char *)this),"VolumeBegin",sizeof(VolumeBegin),"VolumeBegin","VolumeBegin",10);
		m_Describe.SetupMember(BuyOpen,(int)((char *)&BuyOpen-(char *)this),"BuyOpen",sizeof(BuyOpen),"BuyOpen","BuyOpen",10);
		m_Describe.SetupMember(BuyClose,(int)((char *)&BuyClose-(char *)this),"BuyClose",sizeof(BuyClose),"BuyClose","BuyClose",10);
		m_Describe.SetupMember(SellOpen,(int)((char *)&SellOpen-(char *)this),"SellOpen",sizeof(SellOpen),"SellOpen","SellOpen",10);
		m_Describe.SetupMember(SellClose,(int)((char *)&SellClose-(char *)this),"SellClose",sizeof(SellClose),"SellClose","SellClose",10);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Liquidity=0x1204;

///账户资金
class CAccountField
{
public:
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///资金账号
	CAccountIDType	AccountID;
	///币种
	CCurrencyType	Currency;
	///成员代码
	CLongIndexIDType	MemberID;
	///可用资金
	CMoneyType	Available;
	///可取资金
	CMoneyType	Withdrawable;
	///上次静态权益
	CMoneyType	PreBalance;
	///静态权益
	CMoneyType	Balance;
	///持仓成本
	CMoneyType	PositionCost;
	///所有占用保证金
	CMoneyType	UseMargin;
	///全仓保证金
	CMoneyType	CrossMargin;
	///平仓盈亏
	CMoneyType	CloseProfit;
	///入金金额
	CMoneyType	Deposit;
	///出金金额
	CMoneyType	Withdraw;
	///冻结保证金
	CMoneyType	FrozenMargin;
	///冻结资金
	CMoneyType	FrozenMoney;
	///冻结手续费
	CMoneyType	FrozenFee;
	///手续费
	CMoneyType	Fee;
	///资金变化
	CMoneyType	MoneyChange;
	///保留资金
	CMoneyType	Reserve;
	///保留资金可用
	CMoneyType	ReserveAvailable;
	///保留资金限制
	CMoneyType	ReserveLimit;
	///保留资金盈亏
	CMoneyType	ReserveProfit;
	///保留资金类型
	CReserveTypeType	ReserveType;
	///保留资金是否活跃
	CBoolType	ReserveActive;
	///总平仓盈亏
	CTotalType	TotalCloseProfit;
	///总入金金额
	CTotalType	TotalDeposit;
	///总出金金额
	CTotalType	TotalWithdraw;
	///总手续费
	CTotalType	TotalFee;
	///总资金变化
	CTotalType	TotalMoneyChange;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///账户类型
	CShortIndexIDType	AccountType;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///保证金等级
	CShortIndexIDType	MarginRateGrade;
	///权限等级
	CShortIndexIDType	TradingRightGrade;
	///每秒更新的次数
	CNumberType	Frequency;
	///维持保证金
	CMoneyType	MaintMargin;
	///未实现盈亏
	CMoneyType	UnRealProfit;
	///带单盈利分配
	CMoneyType	CopyProfit;
	///点差接管类型
	CCFDTypeType	CFDType;
	///手续费等级
	CShortIndexIDType	FeeGrade;
	///差价等级
	CShortIndexIDType	CFDGrade;
	///借款周期结算量
	CMoneyType	Borrow;
	///备注
	CNameType	Remark;
	///最大本地标识
	CIndexIDType	MaxLocalID;
	///当前价值
	CMoneyType	CurrValue;
	///是否可以出金
	CBoolType	CanReduce;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(Available,(int)((char *)&Available-(char *)this),"Available",sizeof(Available),"Available","Available",10);
		m_Describe.SetupMember(Withdrawable,(int)((char *)&Withdrawable-(char *)this),"Withdrawable",sizeof(Withdrawable),"Withdrawable","Withdrawable",10);
		m_Describe.SetupMember(PreBalance,(int)((char *)&PreBalance-(char *)this),"PreBalance",sizeof(PreBalance),"PreBalance","PreBalance",10);
		m_Describe.SetupMember(Balance,(int)((char *)&Balance-(char *)this),"Balance",sizeof(Balance),"Balance","Balance",10);
		m_Describe.SetupMember(PositionCost,(int)((char *)&PositionCost-(char *)this),"PositionCost",sizeof(PositionCost),"PositionCost","PositionCost",10);
		m_Describe.SetupMember(UseMargin,(int)((char *)&UseMargin-(char *)this),"UseMargin",sizeof(UseMargin),"UseMargin","UseMargin",10);
		m_Describe.SetupMember(CrossMargin,(int)((char *)&CrossMargin-(char *)this),"CrossMargin",sizeof(CrossMargin),"CrossMargin","CrossMargin",10);
		m_Describe.SetupMember(CloseProfit,(int)((char *)&CloseProfit-(char *)this),"CloseProfit",sizeof(CloseProfit),"CloseProfit","CloseProfit",10);
		m_Describe.SetupMember(Deposit,(int)((char *)&Deposit-(char *)this),"Deposit",sizeof(Deposit),"Deposit","Deposit",10);
		m_Describe.SetupMember(Withdraw,(int)((char *)&Withdraw-(char *)this),"Withdraw",sizeof(Withdraw),"Withdraw","Withdraw",10);
		m_Describe.SetupMember(FrozenMargin,(int)((char *)&FrozenMargin-(char *)this),"FrozenMargin",sizeof(FrozenMargin),"FrozenMargin","FrozenMargin",10);
		m_Describe.SetupMember(FrozenMoney,(int)((char *)&FrozenMoney-(char *)this),"FrozenMoney",sizeof(FrozenMoney),"FrozenMoney","FrozenMoney",10);
		m_Describe.SetupMember(FrozenFee,(int)((char *)&FrozenFee-(char *)this),"FrozenFee",sizeof(FrozenFee),"FrozenFee","FrozenFee",10);
		m_Describe.SetupMember(Fee,(int)((char *)&Fee-(char *)this),"Fee",sizeof(Fee),"Fee","Fee",10);
		m_Describe.SetupMember(MoneyChange,(int)((char *)&MoneyChange-(char *)this),"MoneyChange",sizeof(MoneyChange),"MoneyChange","MoneyChange",10);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
		m_Describe.SetupMember(ReserveAvailable,(int)((char *)&ReserveAvailable-(char *)this),"ReserveAvailable",sizeof(ReserveAvailable),"ReserveAvailable","ReserveAvailable",10);
		m_Describe.SetupMember(ReserveLimit,(int)((char *)&ReserveLimit-(char *)this),"ReserveLimit",sizeof(ReserveLimit),"ReserveLimit","ReserveLimit",10);
		m_Describe.SetupMember(ReserveProfit,(int)((char *)&ReserveProfit-(char *)this),"ReserveProfit",sizeof(ReserveProfit),"ReserveProfit","ReserveProfit",10);
		m_Describe.SetupMember(ReserveType,(int)((char *)&ReserveType-(char *)this),"ReserveType",sizeof(ReserveType),"ReserveType","ReserveType",0);
		m_Describe.SetupMember(ReserveActive,(int)((char *)&ReserveActive-(char *)this),"ReserveActive",sizeof(ReserveActive),"ReserveActive","ReserveActive",0);
		m_Describe.SetupMember(TotalCloseProfit,(int)((char *)&TotalCloseProfit-(char *)this),"TotalCloseProfit",sizeof(TotalCloseProfit),"TotalCloseProfit","TotalCloseProfit",6);
		m_Describe.SetupMember(TotalDeposit,(int)((char *)&TotalDeposit-(char *)this),"TotalDeposit",sizeof(TotalDeposit),"TotalDeposit","TotalDeposit",6);
		m_Describe.SetupMember(TotalWithdraw,(int)((char *)&TotalWithdraw-(char *)this),"TotalWithdraw",sizeof(TotalWithdraw),"TotalWithdraw","TotalWithdraw",6);
		m_Describe.SetupMember(TotalFee,(int)((char *)&TotalFee-(char *)this),"TotalFee",sizeof(TotalFee),"TotalFee","TotalFee",6);
		m_Describe.SetupMember(TotalMoneyChange,(int)((char *)&TotalMoneyChange-(char *)this),"TotalMoneyChange",sizeof(TotalMoneyChange),"TotalMoneyChange","TotalMoneyChange",6);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(AccountType,(int)((char *)&AccountType-(char *)this),"AccountType",sizeof(AccountType),"AccountType","AccountType",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(MarginRateGrade,(int)((char *)&MarginRateGrade-(char *)this),"MarginRateGrade",sizeof(MarginRateGrade),"MarginRateGrade","MarginRateGrade",0);
		m_Describe.SetupMember(TradingRightGrade,(int)((char *)&TradingRightGrade-(char *)this),"TradingRightGrade",sizeof(TradingRightGrade),"TradingRightGrade","TradingRightGrade",0);
		m_Describe.SetupMember(Frequency,(int)((char *)&Frequency-(char *)this),"Frequency",sizeof(Frequency),"Frequency","Frequency",0);
		m_Describe.SetupMember(MaintMargin,(int)((char *)&MaintMargin-(char *)this),"MaintMargin",sizeof(MaintMargin),"MaintMargin","MaintMargin",10);
		m_Describe.SetupMember(UnRealProfit,(int)((char *)&UnRealProfit-(char *)this),"UnRealProfit",sizeof(UnRealProfit),"UnRealProfit","UnRealProfit",10);
		m_Describe.SetupMember(CopyProfit,(int)((char *)&CopyProfit-(char *)this),"CopyProfit",sizeof(CopyProfit),"CopyProfit","CopyProfit",10);
		m_Describe.SetupMember(CFDType,(int)((char *)&CFDType-(char *)this),"CFDType",sizeof(CFDType),"CFDType","CFDType",0);
		m_Describe.SetupMember(FeeGrade,(int)((char *)&FeeGrade-(char *)this),"FeeGrade",sizeof(FeeGrade),"FeeGrade","FeeGrade",0);
		m_Describe.SetupMember(CFDGrade,(int)((char *)&CFDGrade-(char *)this),"CFDGrade",sizeof(CFDGrade),"CFDGrade","CFDGrade",0);
		m_Describe.SetupMember(Borrow,(int)((char *)&Borrow-(char *)this),"Borrow",sizeof(Borrow),"Borrow","Borrow",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(MaxLocalID,(int)((char *)&MaxLocalID-(char *)this),"MaxLocalID",sizeof(MaxLocalID),"MaxLocalID","MaxLocalID",0);
		m_Describe.SetupMember(CurrValue,(int)((char *)&CurrValue-(char *)this),"CurrValue",sizeof(CurrValue),"CurrValue","CurrValue",10);
		m_Describe.SetupMember(CanReduce,(int)((char *)&CanReduce-(char *)this),"CanReduce",sizeof(CanReduce),"CanReduce","CanReduce",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Account=0x1205;

///成员持仓汇总
class CMemberPositionField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///资金账号
	CAccountIDType	AccountID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///总持仓
	CVolumeType	Position;
	///多头冻结
	CVolumeType	LongFrozen;
	///空头冻结
	CVolumeType	ShortFrozen;
	///默认杠杆倍数
	CLeverageType	DefaultLeverage;
	///默认是否全仓
	CBoolType	DefaultCrossMargin;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Position,(int)((char *)&Position-(char *)this),"Position",sizeof(Position),"Position","Position",10);
		m_Describe.SetupMember(LongFrozen,(int)((char *)&LongFrozen-(char *)this),"LongFrozen",sizeof(LongFrozen),"LongFrozen","LongFrozen",10);
		m_Describe.SetupMember(ShortFrozen,(int)((char *)&ShortFrozen-(char *)this),"ShortFrozen",sizeof(ShortFrozen),"ShortFrozen","ShortFrozen",10);
		m_Describe.SetupMember(DefaultLeverage,(int)((char *)&DefaultLeverage-(char *)this),"DefaultLeverage",sizeof(DefaultLeverage),"DefaultLeverage","DefaultLeverage",8);
		m_Describe.SetupMember(DefaultCrossMargin,(int)((char *)&DefaultCrossMargin-(char *)this),"DefaultCrossMargin",sizeof(DefaultCrossMargin),"DefaultCrossMargin","DefaultCrossMargin",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_MemberPosition=0x1209;

///标的持仓
class CPositionField
{
public:
	///持仓代码
	CLongIndexIDType	PositionID;
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///持仓类型
	CPositionClassType	PositionClass;
	///持仓多空方向
	CPosiDirectionType	PosiDirection;
	///上次持仓
	CVolumeType	PrePosition;
	///总持仓
	CVolumeType	Position;
	///多头冻结
	CVolumeType	LongFrozen;
	///空头冻结
	CVolumeType	ShortFrozen;
	///昨日多头冻结
	CVolumeType	PreLongFrozen;
	///昨日空头冻结
	CVolumeType	PreShortFrozen;
	///最大持仓
	CVolumeType	HighestPosition;
	///可平持仓
	CVolumeType	ClosePosition;
	///持仓成本
	CCostType	PositionCost;
	///成本价
	CPriceType	CostPrice;
	///占用保证金
	CMoneyType	UseMargin;
	///冻结保证金
	CMoneyType	FrozenMargin;
	///多头冻结保证金
	CMoneyType	LongFrozenMargin;
	///空头冻结保证金
	CMoneyType	ShortFrozenMargin;
	///平仓盈亏
	CMoneyType	CloseProfit;
	///开仓成本
	CTotalType	TotalPositionCost;
	///总平仓盈亏
	CTotalType	TotalCloseProfit;
	///开仓均价
	CPriceType	OpenPrice;
	///平仓均价
	CPriceType	ClosePrice;
	///交易费用
	CMoneyType	TradeFee;
	///持仓费用或者股票分红等
	CMoneyType	PositionFee;
	///杠杆倍数
	CLeverageType	Leverage;
	///资金账号
	CAccountIDType	AccountID;
	///计价币种
	CCurrencyType	PriceCurrency;
	///清算币种
	CCurrencyType	ClearCurrency;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///是否全仓
	CBoolType	IsCrossMargin;
	///平仓单代码
	CIndexIDType	CloseOrderID;
	///止损触发价
	CPriceType	SLTriggerPrice;
	///止盈触发价
	CPriceType	TPTriggerPrice;
	///持仓不为0的开始时间
	CMilliSecondsType	BeginTime;
	///插入时间(新持仓时间)
	CMilliSecondsType	InsertTime;
	///最后开仓时间
	CMilliSecondsType	LastOpenTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///是否自动追加保证金
	CBoolType	IsAutoAddMargin;
	///每秒更新的次数
	CNumberType	Frequency;
	///维持保证金
	CMoneyType	MaintMargin;
	///未实现盈亏
	CMoneyType	UnRealProfit;
	///清算价格
	CPriceType	LiquidPrice;
	///创建时间
	CIndexIDType	CreateTime;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单分配比例
	CRatioType	CopyProfitRate;
	///带单盈利分配
	CMoneyType	CopyProfit;
	///初始成交订单号
	CLongIndexIDType	FirstTradeID;
	///结束成交订单号
	CLongIndexIDType	LastTradeID;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	///保留资金
	CMoneyType	Reserve;
	///保留资金盈亏
	CMoneyType	ReserveProfit;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(PositionClass,(int)((char *)&PositionClass-(char *)this),"PositionClass",sizeof(PositionClass),"PositionClass","PositionClass",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
		m_Describe.SetupMember(PrePosition,(int)((char *)&PrePosition-(char *)this),"PrePosition",sizeof(PrePosition),"PrePosition","PrePosition",10);
		m_Describe.SetupMember(Position,(int)((char *)&Position-(char *)this),"Position",sizeof(Position),"Position","Position",10);
		m_Describe.SetupMember(LongFrozen,(int)((char *)&LongFrozen-(char *)this),"LongFrozen",sizeof(LongFrozen),"LongFrozen","LongFrozen",10);
		m_Describe.SetupMember(ShortFrozen,(int)((char *)&ShortFrozen-(char *)this),"ShortFrozen",sizeof(ShortFrozen),"ShortFrozen","ShortFrozen",10);
		m_Describe.SetupMember(PreLongFrozen,(int)((char *)&PreLongFrozen-(char *)this),"PreLongFrozen",sizeof(PreLongFrozen),"PreLongFrozen","PreLongFrozen",10);
		m_Describe.SetupMember(PreShortFrozen,(int)((char *)&PreShortFrozen-(char *)this),"PreShortFrozen",sizeof(PreShortFrozen),"PreShortFrozen","PreShortFrozen",10);
		m_Describe.SetupMember(HighestPosition,(int)((char *)&HighestPosition-(char *)this),"HighestPosition",sizeof(HighestPosition),"HighestPosition","HighestPosition",10);
		m_Describe.SetupMember(ClosePosition,(int)((char *)&ClosePosition-(char *)this),"ClosePosition",sizeof(ClosePosition),"ClosePosition","ClosePosition",10);
		m_Describe.SetupMember(PositionCost,(int)((char *)&PositionCost-(char *)this),"PositionCost",sizeof(PositionCost),"PositionCost","PositionCost",15);
		m_Describe.SetupMember(CostPrice,(int)((char *)&CostPrice-(char *)this),"CostPrice",sizeof(CostPrice),"CostPrice","CostPrice",15);
		m_Describe.SetupMember(UseMargin,(int)((char *)&UseMargin-(char *)this),"UseMargin",sizeof(UseMargin),"UseMargin","UseMargin",10);
		m_Describe.SetupMember(FrozenMargin,(int)((char *)&FrozenMargin-(char *)this),"FrozenMargin",sizeof(FrozenMargin),"FrozenMargin","FrozenMargin",10);
		m_Describe.SetupMember(LongFrozenMargin,(int)((char *)&LongFrozenMargin-(char *)this),"LongFrozenMargin",sizeof(LongFrozenMargin),"LongFrozenMargin","LongFrozenMargin",10);
		m_Describe.SetupMember(ShortFrozenMargin,(int)((char *)&ShortFrozenMargin-(char *)this),"ShortFrozenMargin",sizeof(ShortFrozenMargin),"ShortFrozenMargin","ShortFrozenMargin",10);
		m_Describe.SetupMember(CloseProfit,(int)((char *)&CloseProfit-(char *)this),"CloseProfit",sizeof(CloseProfit),"CloseProfit","CloseProfit",10);
		m_Describe.SetupMember(TotalPositionCost,(int)((char *)&TotalPositionCost-(char *)this),"TotalPositionCost",sizeof(TotalPositionCost),"TotalPositionCost","TotalPositionCost",6);
		m_Describe.SetupMember(TotalCloseProfit,(int)((char *)&TotalCloseProfit-(char *)this),"TotalCloseProfit",sizeof(TotalCloseProfit),"TotalCloseProfit","TotalCloseProfit",6);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(TradeFee,(int)((char *)&TradeFee-(char *)this),"TradeFee",sizeof(TradeFee),"TradeFee","TradeFee",10);
		m_Describe.SetupMember(PositionFee,(int)((char *)&PositionFee-(char *)this),"PositionFee",sizeof(PositionFee),"PositionFee","PositionFee",10);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(PriceCurrency,(int)((char *)&PriceCurrency-(char *)this),"PriceCurrency",sizeof(PriceCurrency),"PriceCurrency","PriceCurrency",0);
		m_Describe.SetupMember(ClearCurrency,(int)((char *)&ClearCurrency-(char *)this),"ClearCurrency",sizeof(ClearCurrency),"ClearCurrency","ClearCurrency",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(CloseOrderID,(int)((char *)&CloseOrderID-(char *)this),"CloseOrderID",sizeof(CloseOrderID),"CloseOrderID","CloseOrderID",0);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(BeginTime,(int)((char *)&BeginTime-(char *)this),"BeginTime",sizeof(BeginTime),"BeginTime","BeginTime",3);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(LastOpenTime,(int)((char *)&LastOpenTime-(char *)this),"LastOpenTime",sizeof(LastOpenTime),"LastOpenTime","LastOpenTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(IsAutoAddMargin,(int)((char *)&IsAutoAddMargin-(char *)this),"IsAutoAddMargin",sizeof(IsAutoAddMargin),"IsAutoAddMargin","IsAutoAddMargin",0);
		m_Describe.SetupMember(Frequency,(int)((char *)&Frequency-(char *)this),"Frequency",sizeof(Frequency),"Frequency","Frequency",0);
		m_Describe.SetupMember(MaintMargin,(int)((char *)&MaintMargin-(char *)this),"MaintMargin",sizeof(MaintMargin),"MaintMargin","MaintMargin",10);
		m_Describe.SetupMember(UnRealProfit,(int)((char *)&UnRealProfit-(char *)this),"UnRealProfit",sizeof(UnRealProfit),"UnRealProfit","UnRealProfit",10);
		m_Describe.SetupMember(LiquidPrice,(int)((char *)&LiquidPrice-(char *)this),"LiquidPrice",sizeof(LiquidPrice),"LiquidPrice","LiquidPrice",15);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyProfitRate,(int)((char *)&CopyProfitRate-(char *)this),"CopyProfitRate",sizeof(CopyProfitRate),"CopyProfitRate","CopyProfitRate",10);
		m_Describe.SetupMember(CopyProfit,(int)((char *)&CopyProfit-(char *)this),"CopyProfit",sizeof(CopyProfit),"CopyProfit","CopyProfit",10);
		m_Describe.SetupMember(FirstTradeID,(int)((char *)&FirstTradeID-(char *)this),"FirstTradeID",sizeof(FirstTradeID),"FirstTradeID","FirstTradeID",0);
		m_Describe.SetupMember(LastTradeID,(int)((char *)&LastTradeID-(char *)this),"LastTradeID",sizeof(LastTradeID),"LastTradeID","LastTradeID",0);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
		m_Describe.SetupMember(ReserveProfit,(int)((char *)&ReserveProfit-(char *)this),"ReserveProfit",sizeof(ReserveProfit),"ReserveProfit","ReserveProfit",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Position=0x1206;

///报单
class COrderField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///资金账号
	CAccountIDType	AccountID;
	///报单本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单价格类型
	COrderPriceTypeType	OrderPriceType;
	///买卖方向
	CDirectionType	Direction;
	///开平标志
	COffsetFlagType	OffsetFlag;
	///报单价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///显示数量
	CVolumeType	VolumeDisplay;
	///数量取值方式
	CValueModeType	VolumeMode;
	///委托额
	CMoneyType	Cost;
	///订单类型
	COrderTypeType	OrderType;
	///GTD时间
	CMilliSecondsType	GTDTime;
	///最小成交量
	CVolumeType	MinVolume;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType	CloseOrderID;
	///是否全仓
	CBoolType	IsCrossMargin;
	///备注
	CNameType	Remark;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单员报单号
	CIndexIDType	CopyOrderID;
	///委托单杠杆倍数
	CLeverageType	Leverage;
	///带单分配比例
	CRatioType	CopyProfitRate;
	///应用编号
	CShortIndexIDType	APPID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///触发价
	CPriceType	TriggerPrice;
	///保留资金
	CMoneyType	Reserve;
	///报单状态
	COrderStatusType	OrderStatus;
	///衍生来源
	CDeriveSourceType	DeriveSource;
	///衍生明细
	CIndexIDType	DeriveDetail;
	///成交数量
	CVolumeType	VolumeTraded;
	///剩余数量
	CVolumeType	VolumeRemain;
	///已经撤单数量
	CVolumeType	VolumeCancled;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///优先权
	CNumberType	Priority;
	///按时间排队的序号
	CLargeNumberType	TimeSortNo;
	///前置编号
	CNumberType	FrontNo;
	///计价币种
	CCurrencyType	PriceCurrency;
	///手续费币种
	CCurrencyType	FeeCurrency;
	///清算币种
	CCurrencyType	ClearCurrency;
	///冻结资金
	CMoneyType	FrozenMoney;
	///冻结手续费
	CMoneyType	FrozenFee;
	///冻结保证金
	CMoneyType	FrozenMargin;
	///手续费
	CMoneyType	Fee;
	///平仓盈亏
	CMoneyType	CloseProfit;
	///成交金额
	CMoneyType	Turnover;
	///关联报单号
	CIndexIDType	RelatedOrderID;
	///业务执行结果
	CNameType	BusinessResult;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///是否可以参加交易
	CBoolType	Tradable;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///持仓多空方向
	CPosiDirectionType	PosiDirection;
	///成交均价
	CPriceType	TradePrice;
	///平仓成交时的开仓均价
	CPriceType	OpenPrice;
	///触发报单号
	CIndexIDType	TriggerOrderID;
	///开仓报单成交之后的止损触发价
	CPriceType	SLTriggerPrice;
	///开仓报单成交之后的止盈触发价
	CPriceType	TPTriggerPrice;
	///带单盈利分配
	CMoneyType	CopyProfit;
	///成交时的持仓量
	CVolumeType	Position;
	///交易用户代码
	CLongIndexIDType	UserID;
	///插入时的最新价
	CPriceType	LastPriceByInsert;
	///插入时的买一价
	CPriceType	BidPrice1ByInsert;
	///插入时的卖一价
	CPriceType	AskPrice1ByInsert;
	///可用资金
	CMoneyType	Available;
	///创建时间
	CIndexIDType	CreateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OrderPriceType,(int)((char *)&OrderPriceType-(char *)this),"OrderPriceType",sizeof(OrderPriceType),"OrderPriceType","OrderPriceType",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(OffsetFlag,(int)((char *)&OffsetFlag-(char *)this),"OffsetFlag",sizeof(OffsetFlag),"OffsetFlag","OffsetFlag",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(VolumeMode,(int)((char *)&VolumeMode-(char *)this),"VolumeMode",sizeof(VolumeMode),"VolumeMode","VolumeMode",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(OrderType,(int)((char *)&OrderType-(char *)this),"OrderType",sizeof(OrderType),"OrderType","OrderType",0);
		m_Describe.SetupMember(GTDTime,(int)((char *)&GTDTime-(char *)this),"GTDTime",sizeof(GTDTime),"GTDTime","GTDTime",3);
		m_Describe.SetupMember(MinVolume,(int)((char *)&MinVolume-(char *)this),"MinVolume",sizeof(MinVolume),"MinVolume","MinVolume",10);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
		m_Describe.SetupMember(CloseOrderID,(int)((char *)&CloseOrderID-(char *)this),"CloseOrderID",sizeof(CloseOrderID),"CloseOrderID","CloseOrderID",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyOrderID,(int)((char *)&CopyOrderID-(char *)this),"CopyOrderID",sizeof(CopyOrderID),"CopyOrderID","CopyOrderID",0);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(CopyProfitRate,(int)((char *)&CopyProfitRate-(char *)this),"CopyProfitRate",sizeof(CopyProfitRate),"CopyProfitRate","CopyProfitRate",10);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
		m_Describe.SetupMember(OrderStatus,(int)((char *)&OrderStatus-(char *)this),"OrderStatus",sizeof(OrderStatus),"OrderStatus","OrderStatus",0);
		m_Describe.SetupMember(DeriveSource,(int)((char *)&DeriveSource-(char *)this),"DeriveSource",sizeof(DeriveSource),"DeriveSource","DeriveSource",0);
		m_Describe.SetupMember(DeriveDetail,(int)((char *)&DeriveDetail-(char *)this),"DeriveDetail",sizeof(DeriveDetail),"DeriveDetail","DeriveDetail",0);
		m_Describe.SetupMember(VolumeTraded,(int)((char *)&VolumeTraded-(char *)this),"VolumeTraded",sizeof(VolumeTraded),"VolumeTraded","VolumeTraded",10);
		m_Describe.SetupMember(VolumeRemain,(int)((char *)&VolumeRemain-(char *)this),"VolumeRemain",sizeof(VolumeRemain),"VolumeRemain","VolumeRemain",10);
		m_Describe.SetupMember(VolumeCancled,(int)((char *)&VolumeCancled-(char *)this),"VolumeCancled",sizeof(VolumeCancled),"VolumeCancled","VolumeCancled",10);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(Priority,(int)((char *)&Priority-(char *)this),"Priority",sizeof(Priority),"Priority","Priority",0);
		m_Describe.SetupMember(TimeSortNo,(int)((char *)&TimeSortNo-(char *)this),"TimeSortNo",sizeof(TimeSortNo),"TimeSortNo","TimeSortNo",0);
		m_Describe.SetupMember(FrontNo,(int)((char *)&FrontNo-(char *)this),"FrontNo",sizeof(FrontNo),"FrontNo","FrontNo",0);
		m_Describe.SetupMember(PriceCurrency,(int)((char *)&PriceCurrency-(char *)this),"PriceCurrency",sizeof(PriceCurrency),"PriceCurrency","PriceCurrency",0);
		m_Describe.SetupMember(FeeCurrency,(int)((char *)&FeeCurrency-(char *)this),"FeeCurrency",sizeof(FeeCurrency),"FeeCurrency","FeeCurrency",0);
		m_Describe.SetupMember(ClearCurrency,(int)((char *)&ClearCurrency-(char *)this),"ClearCurrency",sizeof(ClearCurrency),"ClearCurrency","ClearCurrency",0);
		m_Describe.SetupMember(FrozenMoney,(int)((char *)&FrozenMoney-(char *)this),"FrozenMoney",sizeof(FrozenMoney),"FrozenMoney","FrozenMoney",10);
		m_Describe.SetupMember(FrozenFee,(int)((char *)&FrozenFee-(char *)this),"FrozenFee",sizeof(FrozenFee),"FrozenFee","FrozenFee",10);
		m_Describe.SetupMember(FrozenMargin,(int)((char *)&FrozenMargin-(char *)this),"FrozenMargin",sizeof(FrozenMargin),"FrozenMargin","FrozenMargin",10);
		m_Describe.SetupMember(Fee,(int)((char *)&Fee-(char *)this),"Fee",sizeof(Fee),"Fee","Fee",10);
		m_Describe.SetupMember(CloseProfit,(int)((char *)&CloseProfit-(char *)this),"CloseProfit",sizeof(CloseProfit),"CloseProfit","CloseProfit",10);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",10);
		m_Describe.SetupMember(RelatedOrderID,(int)((char *)&RelatedOrderID-(char *)this),"RelatedOrderID",sizeof(RelatedOrderID),"RelatedOrderID","RelatedOrderID",0);
		m_Describe.SetupMember(BusinessResult,(int)((char *)&BusinessResult-(char *)this),"BusinessResult",sizeof(BusinessResult),"BusinessResult","BusinessResult",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(Tradable,(int)((char *)&Tradable-(char *)this),"Tradable",sizeof(Tradable),"Tradable","Tradable",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
		m_Describe.SetupMember(TradePrice,(int)((char *)&TradePrice-(char *)this),"TradePrice",sizeof(TradePrice),"TradePrice","TradePrice",15);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(TriggerOrderID,(int)((char *)&TriggerOrderID-(char *)this),"TriggerOrderID",sizeof(TriggerOrderID),"TriggerOrderID","TriggerOrderID",0);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(CopyProfit,(int)((char *)&CopyProfit-(char *)this),"CopyProfit",sizeof(CopyProfit),"CopyProfit","CopyProfit",10);
		m_Describe.SetupMember(Position,(int)((char *)&Position-(char *)this),"Position",sizeof(Position),"Position","Position",10);
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(LastPriceByInsert,(int)((char *)&LastPriceByInsert-(char *)this),"LastPriceByInsert",sizeof(LastPriceByInsert),"LastPriceByInsert","LastPriceByInsert",15);
		m_Describe.SetupMember(BidPrice1ByInsert,(int)((char *)&BidPrice1ByInsert-(char *)this),"BidPrice1ByInsert",sizeof(BidPrice1ByInsert),"BidPrice1ByInsert","BidPrice1ByInsert",15);
		m_Describe.SetupMember(AskPrice1ByInsert,(int)((char *)&AskPrice1ByInsert-(char *)this),"AskPrice1ByInsert",sizeof(AskPrice1ByInsert),"AskPrice1ByInsert","AskPrice1ByInsert",15);
		m_Describe.SetupMember(Available,(int)((char *)&Available-(char *)this),"Available",sizeof(Available),"Available","Available",10);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Order=0x1207;

///条件报单
class CTriggerOrderField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///资金账号
	CAccountIDType	AccountID;
	///报单本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单价格类型
	COrderPriceTypeType	OrderPriceType;
	///买卖方向
	CDirectionType	Direction;
	///开平标志
	COffsetFlagType	OffsetFlag;
	///报单价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///显示数量
	CVolumeType	VolumeDisplay;
	///数量取值方式
	CValueModeType	VolumeMode;
	///委托额
	CMoneyType	Cost;
	///订单类型
	COrderTypeType	OrderType;
	///GTD时间
	CMilliSecondsType	GTDTime;
	///最小成交量
	CVolumeType	MinVolume;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType	CloseOrderID;
	///是否全仓
	CBoolType	IsCrossMargin;
	///备注
	CNameType	Remark;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单员报单号
	CIndexIDType	CopyOrderID;
	///委托单杠杆倍数
	CLeverageType	Leverage;
	///带单分配比例
	CRatioType	CopyProfitRate;
	///应用编号
	CShortIndexIDType	APPID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///触发价
	CPriceType	TriggerPrice;
	///保留资金
	CMoneyType	Reserve;
	///止损价
	CPriceType	SLPrice;
	///止损触发价
	CPriceType	SLTriggerPrice;
	///止盈价
	CPriceType	TPPrice;
	///止盈触发价
	CPriceType	TPTriggerPrice;
	///是否提前检查风控
	CBoolType	RiskBefore;
	///触发的订单类型
	CTriggerOrderTypeType	TriggerOrderType;
	///触发类型明细
	CIndexIDType	TriggerDetail;
	///触发价类型
	CTriggerPriceTypeType	TriggerPriceType;
	///触发单具体设置信息
	CLongContentType	TriggerValue;
	///平仓止损价
	CPriceType	CloseSLPrice;
	///平仓止损触发价
	CPriceType	CloseSLTriggerPrice;
	///平仓止盈价
	CPriceType	CloseTPPrice;
	///平仓止盈触发价
	CPriceType	CloseTPTriggerPrice;
	///报单价格类型
	COrderPriceTypeType	CloseOrderPriceType;
	///平仓价
	CPriceType	ClosePrice;
	///平仓触发价
	CPriceType	CloseTriggerPrice;
	///关联报单号
	CIndexIDType	RelatedOrderID;
	///激活时间
	CMilliSecondsType	ActiveTime;
	///触发时间
	CMilliSecondsType	TriggerTime;
	///按时间排队的序号
	CLargeNumberType	TimeSortNo;
	///触发报单状态
	CTriggerStatusType	TriggerStatus;
	///持仓多空方向
	CPosiDirectionType	PosiDirection;
	///前置编号
	CNumberType	FrontNo;
	///错误代码
	CNumberType	ErrorNo;
	///错误信息
	CShortContentType	ErrorMsg;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///创建时间
	CIndexIDType	CreateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OrderPriceType,(int)((char *)&OrderPriceType-(char *)this),"OrderPriceType",sizeof(OrderPriceType),"OrderPriceType","OrderPriceType",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(OffsetFlag,(int)((char *)&OffsetFlag-(char *)this),"OffsetFlag",sizeof(OffsetFlag),"OffsetFlag","OffsetFlag",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(VolumeMode,(int)((char *)&VolumeMode-(char *)this),"VolumeMode",sizeof(VolumeMode),"VolumeMode","VolumeMode",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(OrderType,(int)((char *)&OrderType-(char *)this),"OrderType",sizeof(OrderType),"OrderType","OrderType",0);
		m_Describe.SetupMember(GTDTime,(int)((char *)&GTDTime-(char *)this),"GTDTime",sizeof(GTDTime),"GTDTime","GTDTime",3);
		m_Describe.SetupMember(MinVolume,(int)((char *)&MinVolume-(char *)this),"MinVolume",sizeof(MinVolume),"MinVolume","MinVolume",10);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
		m_Describe.SetupMember(CloseOrderID,(int)((char *)&CloseOrderID-(char *)this),"CloseOrderID",sizeof(CloseOrderID),"CloseOrderID","CloseOrderID",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyOrderID,(int)((char *)&CopyOrderID-(char *)this),"CopyOrderID",sizeof(CopyOrderID),"CopyOrderID","CopyOrderID",0);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(CopyProfitRate,(int)((char *)&CopyProfitRate-(char *)this),"CopyProfitRate",sizeof(CopyProfitRate),"CopyProfitRate","CopyProfitRate",10);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
		m_Describe.SetupMember(SLPrice,(int)((char *)&SLPrice-(char *)this),"SLPrice",sizeof(SLPrice),"SLPrice","SLPrice",15);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(TPPrice,(int)((char *)&TPPrice-(char *)this),"TPPrice",sizeof(TPPrice),"TPPrice","TPPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(RiskBefore,(int)((char *)&RiskBefore-(char *)this),"RiskBefore",sizeof(RiskBefore),"RiskBefore","RiskBefore",0);
		m_Describe.SetupMember(TriggerOrderType,(int)((char *)&TriggerOrderType-(char *)this),"TriggerOrderType",sizeof(TriggerOrderType),"TriggerOrderType","TriggerOrderType",0);
		m_Describe.SetupMember(TriggerDetail,(int)((char *)&TriggerDetail-(char *)this),"TriggerDetail",sizeof(TriggerDetail),"TriggerDetail","TriggerDetail",0);
		m_Describe.SetupMember(TriggerPriceType,(int)((char *)&TriggerPriceType-(char *)this),"TriggerPriceType",sizeof(TriggerPriceType),"TriggerPriceType","TriggerPriceType",0);
		m_Describe.SetupMember(TriggerValue,(int)((char *)&TriggerValue-(char *)this),"TriggerValue",sizeof(TriggerValue),"TriggerValue","TriggerValue",0);
		m_Describe.SetupMember(CloseSLPrice,(int)((char *)&CloseSLPrice-(char *)this),"CloseSLPrice",sizeof(CloseSLPrice),"CloseSLPrice","CloseSLPrice",15);
		m_Describe.SetupMember(CloseSLTriggerPrice,(int)((char *)&CloseSLTriggerPrice-(char *)this),"CloseSLTriggerPrice",sizeof(CloseSLTriggerPrice),"CloseSLTriggerPrice","CloseSLTriggerPrice",15);
		m_Describe.SetupMember(CloseTPPrice,(int)((char *)&CloseTPPrice-(char *)this),"CloseTPPrice",sizeof(CloseTPPrice),"CloseTPPrice","CloseTPPrice",15);
		m_Describe.SetupMember(CloseTPTriggerPrice,(int)((char *)&CloseTPTriggerPrice-(char *)this),"CloseTPTriggerPrice",sizeof(CloseTPTriggerPrice),"CloseTPTriggerPrice","CloseTPTriggerPrice",15);
		m_Describe.SetupMember(CloseOrderPriceType,(int)((char *)&CloseOrderPriceType-(char *)this),"CloseOrderPriceType",sizeof(CloseOrderPriceType),"CloseOrderPriceType","CloseOrderPriceType",0);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(CloseTriggerPrice,(int)((char *)&CloseTriggerPrice-(char *)this),"CloseTriggerPrice",sizeof(CloseTriggerPrice),"CloseTriggerPrice","CloseTriggerPrice",15);
		m_Describe.SetupMember(RelatedOrderID,(int)((char *)&RelatedOrderID-(char *)this),"RelatedOrderID",sizeof(RelatedOrderID),"RelatedOrderID","RelatedOrderID",0);
		m_Describe.SetupMember(ActiveTime,(int)((char *)&ActiveTime-(char *)this),"ActiveTime",sizeof(ActiveTime),"ActiveTime","ActiveTime",3);
		m_Describe.SetupMember(TriggerTime,(int)((char *)&TriggerTime-(char *)this),"TriggerTime",sizeof(TriggerTime),"TriggerTime","TriggerTime",3);
		m_Describe.SetupMember(TimeSortNo,(int)((char *)&TimeSortNo-(char *)this),"TimeSortNo",sizeof(TimeSortNo),"TimeSortNo","TimeSortNo",0);
		m_Describe.SetupMember(TriggerStatus,(int)((char *)&TriggerStatus-(char *)this),"TriggerStatus",sizeof(TriggerStatus),"TriggerStatus","TriggerStatus",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
		m_Describe.SetupMember(FrontNo,(int)((char *)&FrontNo-(char *)this),"FrontNo",sizeof(FrontNo),"FrontNo","FrontNo",0);
		m_Describe.SetupMember(ErrorNo,(int)((char *)&ErrorNo-(char *)this),"ErrorNo",sizeof(ErrorNo),"ErrorNo","ErrorNo",0);
		m_Describe.SetupMember(ErrorMsg,(int)((char *)&ErrorMsg-(char *)this),"ErrorMsg",sizeof(ErrorMsg),"ErrorMsg","ErrorMsg",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_TriggerOrder=0x120A;

///持仓订单组成
class CPositionOrderField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///资金账号
	CAccountIDType	AccountID;
	///报单本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单价格类型
	COrderPriceTypeType	OrderPriceType;
	///买卖方向
	CDirectionType	Direction;
	///开平标志
	COffsetFlagType	OffsetFlag;
	///报单价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///显示数量
	CVolumeType	VolumeDisplay;
	///数量取值方式
	CValueModeType	VolumeMode;
	///委托额
	CMoneyType	Cost;
	///订单类型
	COrderTypeType	OrderType;
	///GTD时间
	CMilliSecondsType	GTDTime;
	///最小成交量
	CVolumeType	MinVolume;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType	CloseOrderID;
	///是否全仓
	CBoolType	IsCrossMargin;
	///备注
	CNameType	Remark;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单员报单号
	CIndexIDType	CopyOrderID;
	///委托单杠杆倍数
	CLeverageType	Leverage;
	///带单分配比例
	CRatioType	CopyProfitRate;
	///应用编号
	CShortIndexIDType	APPID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///触发价
	CPriceType	TriggerPrice;
	///保留资金
	CMoneyType	Reserve;
	///报单状态
	COrderStatusType	OrderStatus;
	///衍生来源
	CDeriveSourceType	DeriveSource;
	///衍生明细
	CIndexIDType	DeriveDetail;
	///成交数量
	CVolumeType	VolumeTraded;
	///剩余数量
	CVolumeType	VolumeRemain;
	///已经撤单数量
	CVolumeType	VolumeCancled;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///优先权
	CNumberType	Priority;
	///按时间排队的序号
	CLargeNumberType	TimeSortNo;
	///前置编号
	CNumberType	FrontNo;
	///计价币种
	CCurrencyType	PriceCurrency;
	///手续费币种
	CCurrencyType	FeeCurrency;
	///清算币种
	CCurrencyType	ClearCurrency;
	///冻结资金
	CMoneyType	FrozenMoney;
	///冻结手续费
	CMoneyType	FrozenFee;
	///冻结保证金
	CMoneyType	FrozenMargin;
	///手续费
	CMoneyType	Fee;
	///平仓盈亏
	CMoneyType	CloseProfit;
	///成交金额
	CMoneyType	Turnover;
	///关联报单号
	CIndexIDType	RelatedOrderID;
	///业务执行结果
	CNameType	BusinessResult;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///是否可以参加交易
	CBoolType	Tradable;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///持仓多空方向
	CPosiDirectionType	PosiDirection;
	///成交均价
	CPriceType	TradePrice;
	///平仓成交时的开仓均价
	CPriceType	OpenPrice;
	///触发报单号
	CIndexIDType	TriggerOrderID;
	///开仓报单成交之后的止损触发价
	CPriceType	SLTriggerPrice;
	///开仓报单成交之后的止盈触发价
	CPriceType	TPTriggerPrice;
	///带单盈利分配
	CMoneyType	CopyProfit;
	///成交时的持仓量
	CVolumeType	Position;
	///交易用户代码
	CLongIndexIDType	UserID;
	///插入时的最新价
	CPriceType	LastPriceByInsert;
	///插入时的买一价
	CPriceType	BidPrice1ByInsert;
	///插入时的卖一价
	CPriceType	AskPrice1ByInsert;
	///可用资金
	CMoneyType	Available;
	///创建时间
	CIndexIDType	CreateTime;
	///需要平仓的数量
	CVolumeType	VolumeToClose;
	///已经平仓的数量
	CVolumeType	VolumeClosed;
	///正在平仓未成交的数量
	CVolumeType	VolumeOnClose;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OrderPriceType,(int)((char *)&OrderPriceType-(char *)this),"OrderPriceType",sizeof(OrderPriceType),"OrderPriceType","OrderPriceType",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(OffsetFlag,(int)((char *)&OffsetFlag-(char *)this),"OffsetFlag",sizeof(OffsetFlag),"OffsetFlag","OffsetFlag",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(VolumeMode,(int)((char *)&VolumeMode-(char *)this),"VolumeMode",sizeof(VolumeMode),"VolumeMode","VolumeMode",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(OrderType,(int)((char *)&OrderType-(char *)this),"OrderType",sizeof(OrderType),"OrderType","OrderType",0);
		m_Describe.SetupMember(GTDTime,(int)((char *)&GTDTime-(char *)this),"GTDTime",sizeof(GTDTime),"GTDTime","GTDTime",3);
		m_Describe.SetupMember(MinVolume,(int)((char *)&MinVolume-(char *)this),"MinVolume",sizeof(MinVolume),"MinVolume","MinVolume",10);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
		m_Describe.SetupMember(CloseOrderID,(int)((char *)&CloseOrderID-(char *)this),"CloseOrderID",sizeof(CloseOrderID),"CloseOrderID","CloseOrderID",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyOrderID,(int)((char *)&CopyOrderID-(char *)this),"CopyOrderID",sizeof(CopyOrderID),"CopyOrderID","CopyOrderID",0);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(CopyProfitRate,(int)((char *)&CopyProfitRate-(char *)this),"CopyProfitRate",sizeof(CopyProfitRate),"CopyProfitRate","CopyProfitRate",10);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
		m_Describe.SetupMember(OrderStatus,(int)((char *)&OrderStatus-(char *)this),"OrderStatus",sizeof(OrderStatus),"OrderStatus","OrderStatus",0);
		m_Describe.SetupMember(DeriveSource,(int)((char *)&DeriveSource-(char *)this),"DeriveSource",sizeof(DeriveSource),"DeriveSource","DeriveSource",0);
		m_Describe.SetupMember(DeriveDetail,(int)((char *)&DeriveDetail-(char *)this),"DeriveDetail",sizeof(DeriveDetail),"DeriveDetail","DeriveDetail",0);
		m_Describe.SetupMember(VolumeTraded,(int)((char *)&VolumeTraded-(char *)this),"VolumeTraded",sizeof(VolumeTraded),"VolumeTraded","VolumeTraded",10);
		m_Describe.SetupMember(VolumeRemain,(int)((char *)&VolumeRemain-(char *)this),"VolumeRemain",sizeof(VolumeRemain),"VolumeRemain","VolumeRemain",10);
		m_Describe.SetupMember(VolumeCancled,(int)((char *)&VolumeCancled-(char *)this),"VolumeCancled",sizeof(VolumeCancled),"VolumeCancled","VolumeCancled",10);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(Priority,(int)((char *)&Priority-(char *)this),"Priority",sizeof(Priority),"Priority","Priority",0);
		m_Describe.SetupMember(TimeSortNo,(int)((char *)&TimeSortNo-(char *)this),"TimeSortNo",sizeof(TimeSortNo),"TimeSortNo","TimeSortNo",0);
		m_Describe.SetupMember(FrontNo,(int)((char *)&FrontNo-(char *)this),"FrontNo",sizeof(FrontNo),"FrontNo","FrontNo",0);
		m_Describe.SetupMember(PriceCurrency,(int)((char *)&PriceCurrency-(char *)this),"PriceCurrency",sizeof(PriceCurrency),"PriceCurrency","PriceCurrency",0);
		m_Describe.SetupMember(FeeCurrency,(int)((char *)&FeeCurrency-(char *)this),"FeeCurrency",sizeof(FeeCurrency),"FeeCurrency","FeeCurrency",0);
		m_Describe.SetupMember(ClearCurrency,(int)((char *)&ClearCurrency-(char *)this),"ClearCurrency",sizeof(ClearCurrency),"ClearCurrency","ClearCurrency",0);
		m_Describe.SetupMember(FrozenMoney,(int)((char *)&FrozenMoney-(char *)this),"FrozenMoney",sizeof(FrozenMoney),"FrozenMoney","FrozenMoney",10);
		m_Describe.SetupMember(FrozenFee,(int)((char *)&FrozenFee-(char *)this),"FrozenFee",sizeof(FrozenFee),"FrozenFee","FrozenFee",10);
		m_Describe.SetupMember(FrozenMargin,(int)((char *)&FrozenMargin-(char *)this),"FrozenMargin",sizeof(FrozenMargin),"FrozenMargin","FrozenMargin",10);
		m_Describe.SetupMember(Fee,(int)((char *)&Fee-(char *)this),"Fee",sizeof(Fee),"Fee","Fee",10);
		m_Describe.SetupMember(CloseProfit,(int)((char *)&CloseProfit-(char *)this),"CloseProfit",sizeof(CloseProfit),"CloseProfit","CloseProfit",10);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",10);
		m_Describe.SetupMember(RelatedOrderID,(int)((char *)&RelatedOrderID-(char *)this),"RelatedOrderID",sizeof(RelatedOrderID),"RelatedOrderID","RelatedOrderID",0);
		m_Describe.SetupMember(BusinessResult,(int)((char *)&BusinessResult-(char *)this),"BusinessResult",sizeof(BusinessResult),"BusinessResult","BusinessResult",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(Tradable,(int)((char *)&Tradable-(char *)this),"Tradable",sizeof(Tradable),"Tradable","Tradable",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
		m_Describe.SetupMember(TradePrice,(int)((char *)&TradePrice-(char *)this),"TradePrice",sizeof(TradePrice),"TradePrice","TradePrice",15);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(TriggerOrderID,(int)((char *)&TriggerOrderID-(char *)this),"TriggerOrderID",sizeof(TriggerOrderID),"TriggerOrderID","TriggerOrderID",0);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(CopyProfit,(int)((char *)&CopyProfit-(char *)this),"CopyProfit",sizeof(CopyProfit),"CopyProfit","CopyProfit",10);
		m_Describe.SetupMember(Position,(int)((char *)&Position-(char *)this),"Position",sizeof(Position),"Position","Position",10);
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(LastPriceByInsert,(int)((char *)&LastPriceByInsert-(char *)this),"LastPriceByInsert",sizeof(LastPriceByInsert),"LastPriceByInsert","LastPriceByInsert",15);
		m_Describe.SetupMember(BidPrice1ByInsert,(int)((char *)&BidPrice1ByInsert-(char *)this),"BidPrice1ByInsert",sizeof(BidPrice1ByInsert),"BidPrice1ByInsert","BidPrice1ByInsert",15);
		m_Describe.SetupMember(AskPrice1ByInsert,(int)((char *)&AskPrice1ByInsert-(char *)this),"AskPrice1ByInsert",sizeof(AskPrice1ByInsert),"AskPrice1ByInsert","AskPrice1ByInsert",15);
		m_Describe.SetupMember(Available,(int)((char *)&Available-(char *)this),"Available",sizeof(Available),"Available","Available",10);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",0);
		m_Describe.SetupMember(VolumeToClose,(int)((char *)&VolumeToClose-(char *)this),"VolumeToClose",sizeof(VolumeToClose),"VolumeToClose","VolumeToClose",10);
		m_Describe.SetupMember(VolumeClosed,(int)((char *)&VolumeClosed-(char *)this),"VolumeClosed",sizeof(VolumeClosed),"VolumeClosed","VolumeClosed",10);
		m_Describe.SetupMember(VolumeOnClose,(int)((char *)&VolumeOnClose-(char *)this),"VolumeOnClose",sizeof(VolumeOnClose),"VolumeOnClose","VolumeOnClose",10);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_PositionOrder=0x120B;

///完成的报单
class CFinishOrderField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///资金账号
	CAccountIDType	AccountID;
	///报单本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单价格类型
	COrderPriceTypeType	OrderPriceType;
	///买卖方向
	CDirectionType	Direction;
	///开平标志
	COffsetFlagType	OffsetFlag;
	///报单价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///显示数量
	CVolumeType	VolumeDisplay;
	///数量取值方式
	CValueModeType	VolumeMode;
	///委托额
	CMoneyType	Cost;
	///订单类型
	COrderTypeType	OrderType;
	///GTD时间
	CMilliSecondsType	GTDTime;
	///最小成交量
	CVolumeType	MinVolume;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType	CloseOrderID;
	///是否全仓
	CBoolType	IsCrossMargin;
	///备注
	CNameType	Remark;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单员报单号
	CIndexIDType	CopyOrderID;
	///委托单杠杆倍数
	CLeverageType	Leverage;
	///带单分配比例
	CRatioType	CopyProfitRate;
	///应用编号
	CShortIndexIDType	APPID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///触发价
	CPriceType	TriggerPrice;
	///保留资金
	CMoneyType	Reserve;
	///报单状态
	COrderStatusType	OrderStatus;
	///衍生来源
	CDeriveSourceType	DeriveSource;
	///衍生明细
	CIndexIDType	DeriveDetail;
	///成交数量
	CVolumeType	VolumeTraded;
	///剩余数量
	CVolumeType	VolumeRemain;
	///已经撤单数量
	CVolumeType	VolumeCancled;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///优先权
	CNumberType	Priority;
	///按时间排队的序号
	CLargeNumberType	TimeSortNo;
	///前置编号
	CNumberType	FrontNo;
	///计价币种
	CCurrencyType	PriceCurrency;
	///手续费币种
	CCurrencyType	FeeCurrency;
	///清算币种
	CCurrencyType	ClearCurrency;
	///冻结资金
	CMoneyType	FrozenMoney;
	///冻结手续费
	CMoneyType	FrozenFee;
	///冻结保证金
	CMoneyType	FrozenMargin;
	///手续费
	CMoneyType	Fee;
	///平仓盈亏
	CMoneyType	CloseProfit;
	///成交金额
	CMoneyType	Turnover;
	///关联报单号
	CIndexIDType	RelatedOrderID;
	///业务执行结果
	CNameType	BusinessResult;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///是否可以参加交易
	CBoolType	Tradable;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///持仓多空方向
	CPosiDirectionType	PosiDirection;
	///成交均价
	CPriceType	TradePrice;
	///平仓成交时的开仓均价
	CPriceType	OpenPrice;
	///触发报单号
	CIndexIDType	TriggerOrderID;
	///开仓报单成交之后的止损触发价
	CPriceType	SLTriggerPrice;
	///开仓报单成交之后的止盈触发价
	CPriceType	TPTriggerPrice;
	///带单盈利分配
	CMoneyType	CopyProfit;
	///成交时的持仓量
	CVolumeType	Position;
	///交易用户代码
	CLongIndexIDType	UserID;
	///插入时的最新价
	CPriceType	LastPriceByInsert;
	///插入时的买一价
	CPriceType	BidPrice1ByInsert;
	///插入时的卖一价
	CPriceType	AskPrice1ByInsert;
	///可用资金
	CMoneyType	Available;
	///创建时间
	CIndexIDType	CreateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OrderPriceType,(int)((char *)&OrderPriceType-(char *)this),"OrderPriceType",sizeof(OrderPriceType),"OrderPriceType","OrderPriceType",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(OffsetFlag,(int)((char *)&OffsetFlag-(char *)this),"OffsetFlag",sizeof(OffsetFlag),"OffsetFlag","OffsetFlag",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(VolumeMode,(int)((char *)&VolumeMode-(char *)this),"VolumeMode",sizeof(VolumeMode),"VolumeMode","VolumeMode",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(OrderType,(int)((char *)&OrderType-(char *)this),"OrderType",sizeof(OrderType),"OrderType","OrderType",0);
		m_Describe.SetupMember(GTDTime,(int)((char *)&GTDTime-(char *)this),"GTDTime",sizeof(GTDTime),"GTDTime","GTDTime",3);
		m_Describe.SetupMember(MinVolume,(int)((char *)&MinVolume-(char *)this),"MinVolume",sizeof(MinVolume),"MinVolume","MinVolume",10);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
		m_Describe.SetupMember(CloseOrderID,(int)((char *)&CloseOrderID-(char *)this),"CloseOrderID",sizeof(CloseOrderID),"CloseOrderID","CloseOrderID",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyOrderID,(int)((char *)&CopyOrderID-(char *)this),"CopyOrderID",sizeof(CopyOrderID),"CopyOrderID","CopyOrderID",0);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(CopyProfitRate,(int)((char *)&CopyProfitRate-(char *)this),"CopyProfitRate",sizeof(CopyProfitRate),"CopyProfitRate","CopyProfitRate",10);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
		m_Describe.SetupMember(OrderStatus,(int)((char *)&OrderStatus-(char *)this),"OrderStatus",sizeof(OrderStatus),"OrderStatus","OrderStatus",0);
		m_Describe.SetupMember(DeriveSource,(int)((char *)&DeriveSource-(char *)this),"DeriveSource",sizeof(DeriveSource),"DeriveSource","DeriveSource",0);
		m_Describe.SetupMember(DeriveDetail,(int)((char *)&DeriveDetail-(char *)this),"DeriveDetail",sizeof(DeriveDetail),"DeriveDetail","DeriveDetail",0);
		m_Describe.SetupMember(VolumeTraded,(int)((char *)&VolumeTraded-(char *)this),"VolumeTraded",sizeof(VolumeTraded),"VolumeTraded","VolumeTraded",10);
		m_Describe.SetupMember(VolumeRemain,(int)((char *)&VolumeRemain-(char *)this),"VolumeRemain",sizeof(VolumeRemain),"VolumeRemain","VolumeRemain",10);
		m_Describe.SetupMember(VolumeCancled,(int)((char *)&VolumeCancled-(char *)this),"VolumeCancled",sizeof(VolumeCancled),"VolumeCancled","VolumeCancled",10);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(Priority,(int)((char *)&Priority-(char *)this),"Priority",sizeof(Priority),"Priority","Priority",0);
		m_Describe.SetupMember(TimeSortNo,(int)((char *)&TimeSortNo-(char *)this),"TimeSortNo",sizeof(TimeSortNo),"TimeSortNo","TimeSortNo",0);
		m_Describe.SetupMember(FrontNo,(int)((char *)&FrontNo-(char *)this),"FrontNo",sizeof(FrontNo),"FrontNo","FrontNo",0);
		m_Describe.SetupMember(PriceCurrency,(int)((char *)&PriceCurrency-(char *)this),"PriceCurrency",sizeof(PriceCurrency),"PriceCurrency","PriceCurrency",0);
		m_Describe.SetupMember(FeeCurrency,(int)((char *)&FeeCurrency-(char *)this),"FeeCurrency",sizeof(FeeCurrency),"FeeCurrency","FeeCurrency",0);
		m_Describe.SetupMember(ClearCurrency,(int)((char *)&ClearCurrency-(char *)this),"ClearCurrency",sizeof(ClearCurrency),"ClearCurrency","ClearCurrency",0);
		m_Describe.SetupMember(FrozenMoney,(int)((char *)&FrozenMoney-(char *)this),"FrozenMoney",sizeof(FrozenMoney),"FrozenMoney","FrozenMoney",10);
		m_Describe.SetupMember(FrozenFee,(int)((char *)&FrozenFee-(char *)this),"FrozenFee",sizeof(FrozenFee),"FrozenFee","FrozenFee",10);
		m_Describe.SetupMember(FrozenMargin,(int)((char *)&FrozenMargin-(char *)this),"FrozenMargin",sizeof(FrozenMargin),"FrozenMargin","FrozenMargin",10);
		m_Describe.SetupMember(Fee,(int)((char *)&Fee-(char *)this),"Fee",sizeof(Fee),"Fee","Fee",10);
		m_Describe.SetupMember(CloseProfit,(int)((char *)&CloseProfit-(char *)this),"CloseProfit",sizeof(CloseProfit),"CloseProfit","CloseProfit",10);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",10);
		m_Describe.SetupMember(RelatedOrderID,(int)((char *)&RelatedOrderID-(char *)this),"RelatedOrderID",sizeof(RelatedOrderID),"RelatedOrderID","RelatedOrderID",0);
		m_Describe.SetupMember(BusinessResult,(int)((char *)&BusinessResult-(char *)this),"BusinessResult",sizeof(BusinessResult),"BusinessResult","BusinessResult",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(Tradable,(int)((char *)&Tradable-(char *)this),"Tradable",sizeof(Tradable),"Tradable","Tradable",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
		m_Describe.SetupMember(TradePrice,(int)((char *)&TradePrice-(char *)this),"TradePrice",sizeof(TradePrice),"TradePrice","TradePrice",15);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(TriggerOrderID,(int)((char *)&TriggerOrderID-(char *)this),"TriggerOrderID",sizeof(TriggerOrderID),"TriggerOrderID","TriggerOrderID",0);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(CopyProfit,(int)((char *)&CopyProfit-(char *)this),"CopyProfit",sizeof(CopyProfit),"CopyProfit","CopyProfit",10);
		m_Describe.SetupMember(Position,(int)((char *)&Position-(char *)this),"Position",sizeof(Position),"Position","Position",10);
		m_Describe.SetupMember(UserID,(int)((char *)&UserID-(char *)this),"UserID",sizeof(UserID),"UserID","UserID",0);
		m_Describe.SetupMember(LastPriceByInsert,(int)((char *)&LastPriceByInsert-(char *)this),"LastPriceByInsert",sizeof(LastPriceByInsert),"LastPriceByInsert","LastPriceByInsert",15);
		m_Describe.SetupMember(BidPrice1ByInsert,(int)((char *)&BidPrice1ByInsert-(char *)this),"BidPrice1ByInsert",sizeof(BidPrice1ByInsert),"BidPrice1ByInsert","BidPrice1ByInsert",15);
		m_Describe.SetupMember(AskPrice1ByInsert,(int)((char *)&AskPrice1ByInsert-(char *)this),"AskPrice1ByInsert",sizeof(AskPrice1ByInsert),"AskPrice1ByInsert","AskPrice1ByInsert",15);
		m_Describe.SetupMember(Available,(int)((char *)&Available-(char *)this),"Available",sizeof(Available),"Available","Available",10);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_FinishOrder=0x1301;

///成交
class CTradeField
{
public:
	///成交代码
	CIndexIDType	TradeID;
	///买卖方向
	CDirectionType	Direction;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///成员代码
	CLongIndexIDType	MemberID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///资金账号
	CAccountIDType	AccountID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///开平标志
	COffsetFlagType	OffsetFlag;
	///成交价格
	CPriceType	Price;
	///成交数量
	CVolumeType	Volume;
	///衍生类型
	CDeriveSourceType	DeriveSource;
	///成交角色
	CMatchRoleType	MatchRole;
	///计价币种
	CCurrencyType	PriceCurrency;
	///清算币种
	CCurrencyType	ClearCurrency;
	///手续费
	CMoneyType	Fee;
	///手续费币种
	CCurrencyType	FeeCurrency;
	///平仓盈亏
	CMoneyType	CloseProfit;
	///成交金额
	CMoneyType	Turnover;
	///占用或者减少的保证金
	CMoneyType	UseMargin;
	///杠杆倍数
	CLeverageType	Leverage;
	///委托价格
	CPriceType	OrderPrice;
	///触发价
	CPriceType	TriggerPrice;
	///是否自成交
	CBoolType	IsSelfTrade;
	///备注
	CNameType	Remark;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///开仓均价
	CPriceType	OpenPrice;
	///应用编号
	CShortIndexIDType	APPID;
	///插入时间
	CMilliSecondsType	InsertTime;
	///创建时间
	CIndexIDType	CreateTime;
	///带单员报单号
	CIndexIDType	CopyOrderID;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单盈利分配
	CMoneyType	CopyProfit;
	///成交时的持仓量
	CVolumeType	Position;
	///保留资金盈亏
	CMoneyType	ReserveProfit;
	///保留资金手续费
	CMoneyType	ReserveFee;
	///Trade备注
	CNameType	TradeRemark;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(TradeID,(int)((char *)&TradeID-(char *)this),"TradeID",sizeof(TradeID),"TradeID","TradeID",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OffsetFlag,(int)((char *)&OffsetFlag-(char *)this),"OffsetFlag",sizeof(OffsetFlag),"OffsetFlag","OffsetFlag",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(DeriveSource,(int)((char *)&DeriveSource-(char *)this),"DeriveSource",sizeof(DeriveSource),"DeriveSource","DeriveSource",0);
		m_Describe.SetupMember(MatchRole,(int)((char *)&MatchRole-(char *)this),"MatchRole",sizeof(MatchRole),"MatchRole","MatchRole",0);
		m_Describe.SetupMember(PriceCurrency,(int)((char *)&PriceCurrency-(char *)this),"PriceCurrency",sizeof(PriceCurrency),"PriceCurrency","PriceCurrency",0);
		m_Describe.SetupMember(ClearCurrency,(int)((char *)&ClearCurrency-(char *)this),"ClearCurrency",sizeof(ClearCurrency),"ClearCurrency","ClearCurrency",0);
		m_Describe.SetupMember(Fee,(int)((char *)&Fee-(char *)this),"Fee",sizeof(Fee),"Fee","Fee",10);
		m_Describe.SetupMember(FeeCurrency,(int)((char *)&FeeCurrency-(char *)this),"FeeCurrency",sizeof(FeeCurrency),"FeeCurrency","FeeCurrency",0);
		m_Describe.SetupMember(CloseProfit,(int)((char *)&CloseProfit-(char *)this),"CloseProfit",sizeof(CloseProfit),"CloseProfit","CloseProfit",10);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",10);
		m_Describe.SetupMember(UseMargin,(int)((char *)&UseMargin-(char *)this),"UseMargin",sizeof(UseMargin),"UseMargin","UseMargin",10);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(OrderPrice,(int)((char *)&OrderPrice-(char *)this),"OrderPrice",sizeof(OrderPrice),"OrderPrice","OrderPrice",15);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(IsSelfTrade,(int)((char *)&IsSelfTrade-(char *)this),"IsSelfTrade",sizeof(IsSelfTrade),"IsSelfTrade","IsSelfTrade",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",0);
		m_Describe.SetupMember(CopyOrderID,(int)((char *)&CopyOrderID-(char *)this),"CopyOrderID",sizeof(CopyOrderID),"CopyOrderID","CopyOrderID",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyProfit,(int)((char *)&CopyProfit-(char *)this),"CopyProfit",sizeof(CopyProfit),"CopyProfit","CopyProfit",10);
		m_Describe.SetupMember(Position,(int)((char *)&Position-(char *)this),"Position",sizeof(Position),"Position","Position",10);
		m_Describe.SetupMember(ReserveProfit,(int)((char *)&ReserveProfit-(char *)this),"ReserveProfit",sizeof(ReserveProfit),"ReserveProfit","ReserveProfit",10);
		m_Describe.SetupMember(ReserveFee,(int)((char *)&ReserveFee-(char *)this),"ReserveFee",sizeof(ReserveFee),"ReserveFee","ReserveFee",10);
		m_Describe.SetupMember(TradeRemark,(int)((char *)&TradeRemark-(char *)this),"TradeRemark",sizeof(TradeRemark),"TradeRemark","TradeRemark",0);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_Trade=0x1302;

///持仓历史
class CFinishPositionField
{
public:
	///持仓代码
	CLongIndexIDType	PositionID;
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///持仓类型
	CPositionClassType	PositionClass;
	///持仓多空方向
	CPosiDirectionType	PosiDirection;
	///上次持仓
	CVolumeType	PrePosition;
	///总持仓
	CVolumeType	Position;
	///多头冻结
	CVolumeType	LongFrozen;
	///空头冻结
	CVolumeType	ShortFrozen;
	///昨日多头冻结
	CVolumeType	PreLongFrozen;
	///昨日空头冻结
	CVolumeType	PreShortFrozen;
	///最大持仓
	CVolumeType	HighestPosition;
	///可平持仓
	CVolumeType	ClosePosition;
	///持仓成本
	CCostType	PositionCost;
	///成本价
	CPriceType	CostPrice;
	///占用保证金
	CMoneyType	UseMargin;
	///冻结保证金
	CMoneyType	FrozenMargin;
	///多头冻结保证金
	CMoneyType	LongFrozenMargin;
	///空头冻结保证金
	CMoneyType	ShortFrozenMargin;
	///平仓盈亏
	CMoneyType	CloseProfit;
	///开仓成本
	CTotalType	TotalPositionCost;
	///总平仓盈亏
	CTotalType	TotalCloseProfit;
	///开仓均价
	CPriceType	OpenPrice;
	///平仓均价
	CPriceType	ClosePrice;
	///交易费用
	CMoneyType	TradeFee;
	///持仓费用或者股票分红等
	CMoneyType	PositionFee;
	///杠杆倍数
	CLeverageType	Leverage;
	///资金账号
	CAccountIDType	AccountID;
	///计价币种
	CCurrencyType	PriceCurrency;
	///清算币种
	CCurrencyType	ClearCurrency;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///是否全仓
	CBoolType	IsCrossMargin;
	///平仓单代码
	CIndexIDType	CloseOrderID;
	///止损触发价
	CPriceType	SLTriggerPrice;
	///止盈触发价
	CPriceType	TPTriggerPrice;
	///持仓不为0的开始时间
	CMilliSecondsType	BeginTime;
	///插入时间(新持仓时间)
	CMilliSecondsType	InsertTime;
	///最后开仓时间
	CMilliSecondsType	LastOpenTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///是否自动追加保证金
	CBoolType	IsAutoAddMargin;
	///每秒更新的次数
	CNumberType	Frequency;
	///维持保证金
	CMoneyType	MaintMargin;
	///未实现盈亏
	CMoneyType	UnRealProfit;
	///清算价格
	CPriceType	LiquidPrice;
	///创建时间
	CIndexIDType	CreateTime;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单分配比例
	CRatioType	CopyProfitRate;
	///带单盈利分配
	CMoneyType	CopyProfit;
	///初始成交订单号
	CLongIndexIDType	FirstTradeID;
	///结束成交订单号
	CLongIndexIDType	LastTradeID;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	///保留资金
	CMoneyType	Reserve;
	///保留资金盈亏
	CMoneyType	ReserveProfit;
	///备注
	CNameType	Remark;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(PositionClass,(int)((char *)&PositionClass-(char *)this),"PositionClass",sizeof(PositionClass),"PositionClass","PositionClass",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
		m_Describe.SetupMember(PrePosition,(int)((char *)&PrePosition-(char *)this),"PrePosition",sizeof(PrePosition),"PrePosition","PrePosition",10);
		m_Describe.SetupMember(Position,(int)((char *)&Position-(char *)this),"Position",sizeof(Position),"Position","Position",10);
		m_Describe.SetupMember(LongFrozen,(int)((char *)&LongFrozen-(char *)this),"LongFrozen",sizeof(LongFrozen),"LongFrozen","LongFrozen",10);
		m_Describe.SetupMember(ShortFrozen,(int)((char *)&ShortFrozen-(char *)this),"ShortFrozen",sizeof(ShortFrozen),"ShortFrozen","ShortFrozen",10);
		m_Describe.SetupMember(PreLongFrozen,(int)((char *)&PreLongFrozen-(char *)this),"PreLongFrozen",sizeof(PreLongFrozen),"PreLongFrozen","PreLongFrozen",10);
		m_Describe.SetupMember(PreShortFrozen,(int)((char *)&PreShortFrozen-(char *)this),"PreShortFrozen",sizeof(PreShortFrozen),"PreShortFrozen","PreShortFrozen",10);
		m_Describe.SetupMember(HighestPosition,(int)((char *)&HighestPosition-(char *)this),"HighestPosition",sizeof(HighestPosition),"HighestPosition","HighestPosition",10);
		m_Describe.SetupMember(ClosePosition,(int)((char *)&ClosePosition-(char *)this),"ClosePosition",sizeof(ClosePosition),"ClosePosition","ClosePosition",10);
		m_Describe.SetupMember(PositionCost,(int)((char *)&PositionCost-(char *)this),"PositionCost",sizeof(PositionCost),"PositionCost","PositionCost",15);
		m_Describe.SetupMember(CostPrice,(int)((char *)&CostPrice-(char *)this),"CostPrice",sizeof(CostPrice),"CostPrice","CostPrice",15);
		m_Describe.SetupMember(UseMargin,(int)((char *)&UseMargin-(char *)this),"UseMargin",sizeof(UseMargin),"UseMargin","UseMargin",10);
		m_Describe.SetupMember(FrozenMargin,(int)((char *)&FrozenMargin-(char *)this),"FrozenMargin",sizeof(FrozenMargin),"FrozenMargin","FrozenMargin",10);
		m_Describe.SetupMember(LongFrozenMargin,(int)((char *)&LongFrozenMargin-(char *)this),"LongFrozenMargin",sizeof(LongFrozenMargin),"LongFrozenMargin","LongFrozenMargin",10);
		m_Describe.SetupMember(ShortFrozenMargin,(int)((char *)&ShortFrozenMargin-(char *)this),"ShortFrozenMargin",sizeof(ShortFrozenMargin),"ShortFrozenMargin","ShortFrozenMargin",10);
		m_Describe.SetupMember(CloseProfit,(int)((char *)&CloseProfit-(char *)this),"CloseProfit",sizeof(CloseProfit),"CloseProfit","CloseProfit",10);
		m_Describe.SetupMember(TotalPositionCost,(int)((char *)&TotalPositionCost-(char *)this),"TotalPositionCost",sizeof(TotalPositionCost),"TotalPositionCost","TotalPositionCost",6);
		m_Describe.SetupMember(TotalCloseProfit,(int)((char *)&TotalCloseProfit-(char *)this),"TotalCloseProfit",sizeof(TotalCloseProfit),"TotalCloseProfit","TotalCloseProfit",6);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(TradeFee,(int)((char *)&TradeFee-(char *)this),"TradeFee",sizeof(TradeFee),"TradeFee","TradeFee",10);
		m_Describe.SetupMember(PositionFee,(int)((char *)&PositionFee-(char *)this),"PositionFee",sizeof(PositionFee),"PositionFee","PositionFee",10);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(PriceCurrency,(int)((char *)&PriceCurrency-(char *)this),"PriceCurrency",sizeof(PriceCurrency),"PriceCurrency","PriceCurrency",0);
		m_Describe.SetupMember(ClearCurrency,(int)((char *)&ClearCurrency-(char *)this),"ClearCurrency",sizeof(ClearCurrency),"ClearCurrency","ClearCurrency",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(CloseOrderID,(int)((char *)&CloseOrderID-(char *)this),"CloseOrderID",sizeof(CloseOrderID),"CloseOrderID","CloseOrderID",0);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(BeginTime,(int)((char *)&BeginTime-(char *)this),"BeginTime",sizeof(BeginTime),"BeginTime","BeginTime",3);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(LastOpenTime,(int)((char *)&LastOpenTime-(char *)this),"LastOpenTime",sizeof(LastOpenTime),"LastOpenTime","LastOpenTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(IsAutoAddMargin,(int)((char *)&IsAutoAddMargin-(char *)this),"IsAutoAddMargin",sizeof(IsAutoAddMargin),"IsAutoAddMargin","IsAutoAddMargin",0);
		m_Describe.SetupMember(Frequency,(int)((char *)&Frequency-(char *)this),"Frequency",sizeof(Frequency),"Frequency","Frequency",0);
		m_Describe.SetupMember(MaintMargin,(int)((char *)&MaintMargin-(char *)this),"MaintMargin",sizeof(MaintMargin),"MaintMargin","MaintMargin",10);
		m_Describe.SetupMember(UnRealProfit,(int)((char *)&UnRealProfit-(char *)this),"UnRealProfit",sizeof(UnRealProfit),"UnRealProfit","UnRealProfit",10);
		m_Describe.SetupMember(LiquidPrice,(int)((char *)&LiquidPrice-(char *)this),"LiquidPrice",sizeof(LiquidPrice),"LiquidPrice","LiquidPrice",15);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyProfitRate,(int)((char *)&CopyProfitRate-(char *)this),"CopyProfitRate",sizeof(CopyProfitRate),"CopyProfitRate","CopyProfitRate",10);
		m_Describe.SetupMember(CopyProfit,(int)((char *)&CopyProfit-(char *)this),"CopyProfit",sizeof(CopyProfit),"CopyProfit","CopyProfit",10);
		m_Describe.SetupMember(FirstTradeID,(int)((char *)&FirstTradeID-(char *)this),"FirstTradeID",sizeof(FirstTradeID),"FirstTradeID","FirstTradeID",0);
		m_Describe.SetupMember(LastTradeID,(int)((char *)&LastTradeID-(char *)this),"LastTradeID",sizeof(LastTradeID),"LastTradeID","LastTradeID",0);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
		m_Describe.SetupMember(ReserveProfit,(int)((char *)&ReserveProfit-(char *)this),"ReserveProfit",sizeof(ReserveProfit),"ReserveProfit","ReserveProfit",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_FinishPosition=0x1309;

///账户流水
class CAccountDetailField
{
public:
	///资金明细号
	CIndexIDType	AccountDetailID;
	///成员代码
	CLongIndexIDType	MemberID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///资金账号
	CAccountIDType	AccountID;
	///币种
	CCurrencyType	Currency;
	///发生额
	CMoneyType	Amount;
	///上次静态权益
	CMoneyType	PreBalance;
	///静态权益
	CMoneyType	Balance;
	///体验金发生额
	CMoneyType	ReserveAmount;
	///体验金静态权益
	CMoneyType	ReserveBalance;
	///财务流水类型
	CDefineTypeType	Source;
	///备注
	CNameType	Remark;
	///本地标识
	CIndexIDType	LocalID;
	///结算段
	CIndexIDType	SettleSegment;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///内外对账ID
	CLongNameType	RelatedID;
	///插入时间
	CMilliSecondsType	InsertTime;
	///创建时间
	CIndexIDType	CreateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(AccountDetailID,(int)((char *)&AccountDetailID-(char *)this),"AccountDetailID",sizeof(AccountDetailID),"AccountDetailID","AccountDetailID",0);
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(Currency,(int)((char *)&Currency-(char *)this),"Currency",sizeof(Currency),"Currency","Currency",0);
		m_Describe.SetupMember(Amount,(int)((char *)&Amount-(char *)this),"Amount",sizeof(Amount),"Amount","Amount",10);
		m_Describe.SetupMember(PreBalance,(int)((char *)&PreBalance-(char *)this),"PreBalance",sizeof(PreBalance),"PreBalance","PreBalance",10);
		m_Describe.SetupMember(Balance,(int)((char *)&Balance-(char *)this),"Balance",sizeof(Balance),"Balance","Balance",10);
		m_Describe.SetupMember(ReserveAmount,(int)((char *)&ReserveAmount-(char *)this),"ReserveAmount",sizeof(ReserveAmount),"ReserveAmount","ReserveAmount",10);
		m_Describe.SetupMember(ReserveBalance,(int)((char *)&ReserveBalance-(char *)this),"ReserveBalance",sizeof(ReserveBalance),"ReserveBalance","ReserveBalance",10);
		m_Describe.SetupMember(Source,(int)((char *)&Source-(char *)this),"Source",sizeof(Source),"Source","Source",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(SettleSegment,(int)((char *)&SettleSegment-(char *)this),"SettleSegment",sizeof(SettleSegment),"SettleSegment","SettleSegment",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(RelatedID,(int)((char *)&RelatedID-(char *)this),"RelatedID",sizeof(RelatedID),"RelatedID","RelatedID",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_AccountDetail=0x1303;

///历史K线
class CKLineField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///K线周期代码(n[m/h/d/o])
	CBarType	Bar;
	///插入时间
	CMilliSecondsType	InsertTime;
	///开盘价
	CPriceType	OpenPrice;
	///最高价
	CPriceType	HighestPrice;
	///最低价
	CPriceType	LowestPrice;
	///收盘价
	CPriceType	ClosePrice;
	///数量
	CCollectType	Volume;
	///成交金额
	CCollectType	Turnover;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Bar,(int)((char *)&Bar-(char *)this),"Bar",sizeof(Bar),"Bar","Bar",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(HighestPrice,(int)((char *)&HighestPrice-(char *)this),"HighestPrice",sizeof(HighestPrice),"HighestPrice","HighestPrice",15);
		m_Describe.SetupMember(LowestPrice,(int)((char *)&LowestPrice-(char *)this),"LowestPrice",sizeof(LowestPrice),"LowestPrice","LowestPrice",15);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",8);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",8);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_KLine=0x1304;

///操作流水
class CSettleDetailField
{
public:
	///操作明细号
	CIndexIDType	SettleDetailID;
	///应用编号
	CShortIndexIDType	APPID;
	///成交对本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///结算组编号
	CShortIndexIDType	SettlementGroup;
	///结算操作
	CSettleActionTypeType	SettleAction;
	///操作使用值
	CRatioType	Value;
	///发生值1
	CRatioType	Value1;
	///发生值2
	CRatioType	Value2;
	///发生值3
	CRatioType	Value3;
	///备注
	CNameType	Remark;
	///资金费用组
	CShortIndexIDType	FundingRateGroup;
	///结算段
	CIndexIDType	SettleSegment;
	///创建时间
	CIndexIDType	InsertTime;
	///业务序列号
	CLargeNumberType	BusinessNo;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(SettleDetailID,(int)((char *)&SettleDetailID-(char *)this),"SettleDetailID",sizeof(SettleDetailID),"SettleDetailID","SettleDetailID",0);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(SettlementGroup,(int)((char *)&SettlementGroup-(char *)this),"SettlementGroup",sizeof(SettlementGroup),"SettlementGroup","SettlementGroup",0);
		m_Describe.SetupMember(SettleAction,(int)((char *)&SettleAction-(char *)this),"SettleAction",sizeof(SettleAction),"SettleAction","SettleAction",0);
		m_Describe.SetupMember(Value,(int)((char *)&Value-(char *)this),"Value",sizeof(Value),"Value","Value",10);
		m_Describe.SetupMember(Value1,(int)((char *)&Value1-(char *)this),"Value1",sizeof(Value1),"Value1","Value1",10);
		m_Describe.SetupMember(Value2,(int)((char *)&Value2-(char *)this),"Value2",sizeof(Value2),"Value2","Value2",10);
		m_Describe.SetupMember(Value3,(int)((char *)&Value3-(char *)this),"Value3",sizeof(Value3),"Value3","Value3",10);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(FundingRateGroup,(int)((char *)&FundingRateGroup-(char *)this),"FundingRateGroup",sizeof(FundingRateGroup),"FundingRateGroup","FundingRateGroup",0);
		m_Describe.SetupMember(SettleSegment,(int)((char *)&SettleSegment-(char *)this),"SettleSegment",sizeof(SettleSegment),"SettleSegment","SettleSegment",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_SettleDetail=0x1305;

///完成的条件报单
class CFinishTriggerOrderField
{
public:
	///成员代码
	CLongIndexIDType	MemberID;
	///交易单元代码
	CIndexIDType	TradeUnitID;
	///资金账号
	CAccountIDType	AccountID;
	///报单本地标识
	CIndexIDType	LocalID;
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///报单价格类型
	COrderPriceTypeType	OrderPriceType;
	///买卖方向
	CDirectionType	Direction;
	///开平标志
	COffsetFlagType	OffsetFlag;
	///报单价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///显示数量
	CVolumeType	VolumeDisplay;
	///数量取值方式
	CValueModeType	VolumeMode;
	///委托额
	CMoneyType	Cost;
	///订单类型
	COrderTypeType	OrderType;
	///GTD时间
	CMilliSecondsType	GTDTime;
	///最小成交量
	CVolumeType	MinVolume;
	///业务类别
	CBusinessTypeType	BusinessType;
	///业务值
	CNameType	BusinessValue;
	///平仓指定开仓的订单号
	CIndexIDType	CloseOrderID;
	///是否全仓
	CBoolType	IsCrossMargin;
	///备注
	CNameType	Remark;
	///报单系统唯一代码
	CIndexIDType	OrderID;
	///带单员代码
	CLongIndexIDType	CopyMemberID;
	///带单员报单号
	CIndexIDType	CopyOrderID;
	///委托单杠杆倍数
	CLeverageType	Leverage;
	///带单分配比例
	CRatioType	CopyProfitRate;
	///应用编号
	CShortIndexIDType	APPID;
	///持仓代码
	CLongIndexIDType	PositionID;
	///触发价
	CPriceType	TriggerPrice;
	///保留资金
	CMoneyType	Reserve;
	///止损价
	CPriceType	SLPrice;
	///止损触发价
	CPriceType	SLTriggerPrice;
	///止盈价
	CPriceType	TPPrice;
	///止盈触发价
	CPriceType	TPTriggerPrice;
	///是否提前检查风控
	CBoolType	RiskBefore;
	///触发的订单类型
	CTriggerOrderTypeType	TriggerOrderType;
	///触发类型明细
	CIndexIDType	TriggerDetail;
	///触发价类型
	CTriggerPriceTypeType	TriggerPriceType;
	///触发单具体设置信息
	CLongContentType	TriggerValue;
	///平仓止损价
	CPriceType	CloseSLPrice;
	///平仓止损触发价
	CPriceType	CloseSLTriggerPrice;
	///平仓止盈价
	CPriceType	CloseTPPrice;
	///平仓止盈触发价
	CPriceType	CloseTPTriggerPrice;
	///报单价格类型
	COrderPriceTypeType	CloseOrderPriceType;
	///平仓价
	CPriceType	ClosePrice;
	///平仓触发价
	CPriceType	CloseTriggerPrice;
	///关联报单号
	CIndexIDType	RelatedOrderID;
	///激活时间
	CMilliSecondsType	ActiveTime;
	///触发时间
	CMilliSecondsType	TriggerTime;
	///按时间排队的序号
	CLargeNumberType	TimeSortNo;
	///触发报单状态
	CTriggerStatusType	TriggerStatus;
	///持仓多空方向
	CPosiDirectionType	PosiDirection;
	///前置编号
	CNumberType	FrontNo;
	///错误代码
	CNumberType	ErrorNo;
	///错误信息
	CShortContentType	ErrorMsg;
	///插入时间
	CMilliSecondsType	InsertTime;
	///更新时间
	CMilliSecondsType	UpdateTime;
	///业务序列号
	CLargeNumberType	BusinessNo;
	///创建时间
	CIndexIDType	CreateTime;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(MemberID,(int)((char *)&MemberID-(char *)this),"MemberID",sizeof(MemberID),"MemberID","MemberID",0);
		m_Describe.SetupMember(TradeUnitID,(int)((char *)&TradeUnitID-(char *)this),"TradeUnitID",sizeof(TradeUnitID),"TradeUnitID","TradeUnitID",0);
		m_Describe.SetupMember(AccountID,(int)((char *)&AccountID-(char *)this),"AccountID",sizeof(AccountID),"AccountID","AccountID",0);
		m_Describe.SetupMember(LocalID,(int)((char *)&LocalID-(char *)this),"LocalID",sizeof(LocalID),"LocalID","LocalID",0);
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(OrderPriceType,(int)((char *)&OrderPriceType-(char *)this),"OrderPriceType",sizeof(OrderPriceType),"OrderPriceType","OrderPriceType",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(OffsetFlag,(int)((char *)&OffsetFlag-(char *)this),"OffsetFlag",sizeof(OffsetFlag),"OffsetFlag","OffsetFlag",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(VolumeDisplay,(int)((char *)&VolumeDisplay-(char *)this),"VolumeDisplay",sizeof(VolumeDisplay),"VolumeDisplay","VolumeDisplay",10);
		m_Describe.SetupMember(VolumeMode,(int)((char *)&VolumeMode-(char *)this),"VolumeMode",sizeof(VolumeMode),"VolumeMode","VolumeMode",0);
		m_Describe.SetupMember(Cost,(int)((char *)&Cost-(char *)this),"Cost",sizeof(Cost),"Cost","Cost",10);
		m_Describe.SetupMember(OrderType,(int)((char *)&OrderType-(char *)this),"OrderType",sizeof(OrderType),"OrderType","OrderType",0);
		m_Describe.SetupMember(GTDTime,(int)((char *)&GTDTime-(char *)this),"GTDTime",sizeof(GTDTime),"GTDTime","GTDTime",3);
		m_Describe.SetupMember(MinVolume,(int)((char *)&MinVolume-(char *)this),"MinVolume",sizeof(MinVolume),"MinVolume","MinVolume",10);
		m_Describe.SetupMember(BusinessType,(int)((char *)&BusinessType-(char *)this),"BusinessType",sizeof(BusinessType),"BusinessType","BusinessType",0);
		m_Describe.SetupMember(BusinessValue,(int)((char *)&BusinessValue-(char *)this),"BusinessValue",sizeof(BusinessValue),"BusinessValue","BusinessValue",0);
		m_Describe.SetupMember(CloseOrderID,(int)((char *)&CloseOrderID-(char *)this),"CloseOrderID",sizeof(CloseOrderID),"CloseOrderID","CloseOrderID",0);
		m_Describe.SetupMember(IsCrossMargin,(int)((char *)&IsCrossMargin-(char *)this),"IsCrossMargin",sizeof(IsCrossMargin),"IsCrossMargin","IsCrossMargin",0);
		m_Describe.SetupMember(Remark,(int)((char *)&Remark-(char *)this),"Remark",sizeof(Remark),"Remark","Remark",0);
		m_Describe.SetupMember(OrderID,(int)((char *)&OrderID-(char *)this),"OrderID",sizeof(OrderID),"OrderID","OrderID",0);
		m_Describe.SetupMember(CopyMemberID,(int)((char *)&CopyMemberID-(char *)this),"CopyMemberID",sizeof(CopyMemberID),"CopyMemberID","CopyMemberID",0);
		m_Describe.SetupMember(CopyOrderID,(int)((char *)&CopyOrderID-(char *)this),"CopyOrderID",sizeof(CopyOrderID),"CopyOrderID","CopyOrderID",0);
		m_Describe.SetupMember(Leverage,(int)((char *)&Leverage-(char *)this),"Leverage",sizeof(Leverage),"Leverage","Leverage",8);
		m_Describe.SetupMember(CopyProfitRate,(int)((char *)&CopyProfitRate-(char *)this),"CopyProfitRate",sizeof(CopyProfitRate),"CopyProfitRate","CopyProfitRate",10);
		m_Describe.SetupMember(APPID,(int)((char *)&APPID-(char *)this),"APPID",sizeof(APPID),"APPID","APPID",0);
		m_Describe.SetupMember(PositionID,(int)((char *)&PositionID-(char *)this),"PositionID",sizeof(PositionID),"PositionID","PositionID",0);
		m_Describe.SetupMember(TriggerPrice,(int)((char *)&TriggerPrice-(char *)this),"TriggerPrice",sizeof(TriggerPrice),"TriggerPrice","TriggerPrice",15);
		m_Describe.SetupMember(Reserve,(int)((char *)&Reserve-(char *)this),"Reserve",sizeof(Reserve),"Reserve","Reserve",10);
		m_Describe.SetupMember(SLPrice,(int)((char *)&SLPrice-(char *)this),"SLPrice",sizeof(SLPrice),"SLPrice","SLPrice",15);
		m_Describe.SetupMember(SLTriggerPrice,(int)((char *)&SLTriggerPrice-(char *)this),"SLTriggerPrice",sizeof(SLTriggerPrice),"SLTriggerPrice","SLTriggerPrice",15);
		m_Describe.SetupMember(TPPrice,(int)((char *)&TPPrice-(char *)this),"TPPrice",sizeof(TPPrice),"TPPrice","TPPrice",15);
		m_Describe.SetupMember(TPTriggerPrice,(int)((char *)&TPTriggerPrice-(char *)this),"TPTriggerPrice",sizeof(TPTriggerPrice),"TPTriggerPrice","TPTriggerPrice",15);
		m_Describe.SetupMember(RiskBefore,(int)((char *)&RiskBefore-(char *)this),"RiskBefore",sizeof(RiskBefore),"RiskBefore","RiskBefore",0);
		m_Describe.SetupMember(TriggerOrderType,(int)((char *)&TriggerOrderType-(char *)this),"TriggerOrderType",sizeof(TriggerOrderType),"TriggerOrderType","TriggerOrderType",0);
		m_Describe.SetupMember(TriggerDetail,(int)((char *)&TriggerDetail-(char *)this),"TriggerDetail",sizeof(TriggerDetail),"TriggerDetail","TriggerDetail",0);
		m_Describe.SetupMember(TriggerPriceType,(int)((char *)&TriggerPriceType-(char *)this),"TriggerPriceType",sizeof(TriggerPriceType),"TriggerPriceType","TriggerPriceType",0);
		m_Describe.SetupMember(TriggerValue,(int)((char *)&TriggerValue-(char *)this),"TriggerValue",sizeof(TriggerValue),"TriggerValue","TriggerValue",0);
		m_Describe.SetupMember(CloseSLPrice,(int)((char *)&CloseSLPrice-(char *)this),"CloseSLPrice",sizeof(CloseSLPrice),"CloseSLPrice","CloseSLPrice",15);
		m_Describe.SetupMember(CloseSLTriggerPrice,(int)((char *)&CloseSLTriggerPrice-(char *)this),"CloseSLTriggerPrice",sizeof(CloseSLTriggerPrice),"CloseSLTriggerPrice","CloseSLTriggerPrice",15);
		m_Describe.SetupMember(CloseTPPrice,(int)((char *)&CloseTPPrice-(char *)this),"CloseTPPrice",sizeof(CloseTPPrice),"CloseTPPrice","CloseTPPrice",15);
		m_Describe.SetupMember(CloseTPTriggerPrice,(int)((char *)&CloseTPTriggerPrice-(char *)this),"CloseTPTriggerPrice",sizeof(CloseTPTriggerPrice),"CloseTPTriggerPrice","CloseTPTriggerPrice",15);
		m_Describe.SetupMember(CloseOrderPriceType,(int)((char *)&CloseOrderPriceType-(char *)this),"CloseOrderPriceType",sizeof(CloseOrderPriceType),"CloseOrderPriceType","CloseOrderPriceType",0);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(CloseTriggerPrice,(int)((char *)&CloseTriggerPrice-(char *)this),"CloseTriggerPrice",sizeof(CloseTriggerPrice),"CloseTriggerPrice","CloseTriggerPrice",15);
		m_Describe.SetupMember(RelatedOrderID,(int)((char *)&RelatedOrderID-(char *)this),"RelatedOrderID",sizeof(RelatedOrderID),"RelatedOrderID","RelatedOrderID",0);
		m_Describe.SetupMember(ActiveTime,(int)((char *)&ActiveTime-(char *)this),"ActiveTime",sizeof(ActiveTime),"ActiveTime","ActiveTime",3);
		m_Describe.SetupMember(TriggerTime,(int)((char *)&TriggerTime-(char *)this),"TriggerTime",sizeof(TriggerTime),"TriggerTime","TriggerTime",3);
		m_Describe.SetupMember(TimeSortNo,(int)((char *)&TimeSortNo-(char *)this),"TimeSortNo",sizeof(TimeSortNo),"TimeSortNo","TimeSortNo",0);
		m_Describe.SetupMember(TriggerStatus,(int)((char *)&TriggerStatus-(char *)this),"TriggerStatus",sizeof(TriggerStatus),"TriggerStatus","TriggerStatus",0);
		m_Describe.SetupMember(PosiDirection,(int)((char *)&PosiDirection-(char *)this),"PosiDirection",sizeof(PosiDirection),"PosiDirection","PosiDirection",0);
		m_Describe.SetupMember(FrontNo,(int)((char *)&FrontNo-(char *)this),"FrontNo",sizeof(FrontNo),"FrontNo","FrontNo",0);
		m_Describe.SetupMember(ErrorNo,(int)((char *)&ErrorNo-(char *)this),"ErrorNo",sizeof(ErrorNo),"ErrorNo","ErrorNo",0);
		m_Describe.SetupMember(ErrorMsg,(int)((char *)&ErrorMsg-(char *)this),"ErrorMsg",sizeof(ErrorMsg),"ErrorMsg","ErrorMsg",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(UpdateTime,(int)((char *)&UpdateTime-(char *)this),"UpdateTime",sizeof(UpdateTime),"UpdateTime","UpdateTime",3);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"BusinessNo","BusinessNo",0);
		m_Describe.SetupMember(CreateTime,(int)((char *)&CreateTime-(char *)this),"CreateTime",sizeof(CreateTime),"CreateTime","CreateTime",0);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_FinishTriggerOrder=0x1306;

///订单簿变化
class CMarketOrderField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///买卖方向
	CDirectionType	Direction;
	///价格
	CPriceType	Price;
	///数量
	CVolumeType	Volume;
	///订单数量
	CNumberType	Orders;
	///最后变化序列号
	CLargeNumberType	BusinessNo;
	///真实报单数量
	CVolumeType	TrueVolume;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Direction,(int)((char *)&Direction-(char *)this),"Direction",sizeof(Direction),"Direction","Direction",0);
		m_Describe.SetupMember(Price,(int)((char *)&Price-(char *)this),"Price",sizeof(Price),"Price","Price",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",10);
		m_Describe.SetupMember(Orders,(int)((char *)&Orders-(char *)this),"Orders",sizeof(Orders),"Orders","Orders",0);
		m_Describe.SetupMember(BusinessNo,(int)((char *)&BusinessNo-(char *)this),"BusinessNo",sizeof(BusinessNo),"","BusinessNo",0);
		m_Describe.SetupMember(TrueVolume,(int)((char *)&TrueVolume-(char *)this),"TrueVolume",sizeof(TrueVolume),"TrueVolume","TrueVolume",10);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_MarketOrder=0x1307;

///发布K线
class CPublishKLineField
{
public:
	///交易所代码
	CExchangeIDType	ExchangeID;
	///标的代码
	CInstrumentIDType	InstrumentID;
	///K线周期代码(n[m/h/d/o])
	CBarType	Bar;
	///插入时间
	CMilliSecondsType	InsertTime;
	///开盘价
	CPriceType	OpenPrice;
	///最高价
	CPriceType	HighestPrice;
	///最低价
	CPriceType	LowestPrice;
	///收盘价
	CPriceType	ClosePrice;
	///数量
	CCollectType	Volume;
	///成交金额
	CCollectType	Turnover;
	
	void DescribeMembers()
	{
		m_Describe.SetupMember(ExchangeID,(int)((char *)&ExchangeID-(char *)this),"ExchangeID",sizeof(ExchangeID),"ExchangeID","ExchangeID",0);
		m_Describe.SetupMember(InstrumentID,(int)((char *)&InstrumentID-(char *)this),"InstrumentID",sizeof(InstrumentID),"InstrumentID","InstrumentID",0);
		m_Describe.SetupMember(Bar,(int)((char *)&Bar-(char *)this),"Bar",sizeof(Bar),"Bar","Bar",0);
		m_Describe.SetupMember(InsertTime,(int)((char *)&InsertTime-(char *)this),"InsertTime",sizeof(InsertTime),"InsertTime","InsertTime",3);
		m_Describe.SetupMember(OpenPrice,(int)((char *)&OpenPrice-(char *)this),"OpenPrice",sizeof(OpenPrice),"OpenPrice","OpenPrice",15);
		m_Describe.SetupMember(HighestPrice,(int)((char *)&HighestPrice-(char *)this),"HighestPrice",sizeof(HighestPrice),"HighestPrice","HighestPrice",15);
		m_Describe.SetupMember(LowestPrice,(int)((char *)&LowestPrice-(char *)this),"LowestPrice",sizeof(LowestPrice),"LowestPrice","LowestPrice",15);
		m_Describe.SetupMember(ClosePrice,(int)((char *)&ClosePrice-(char *)this),"ClosePrice",sizeof(ClosePrice),"ClosePrice","ClosePrice",15);
		m_Describe.SetupMember(Volume,(int)((char *)&Volume-(char *)this),"Volume",sizeof(Volume),"Volume","Volume",8);
		m_Describe.SetupMember(Turnover,(int)((char *)&Turnover-(char *)this),"Turnover",sizeof(Turnover),"Turnover","Turnover",8);
	}
	
	static CFieldDescribe m_Describe;
};
const int FID_PublishKLine=0x1308;



//一般消息请求
const int TID_SendCommand=0x00002001;
//一般消息应答
const int TID_RecvCommand=0x00002002;
//创建上层连接请求
const int TID_SendInitLink=0x00002003;
//创建上层连接应答
const int TID_RecvInitLink=0x00002004;
//用户登录请求
const int TID_SendUserLogin=0x00002005;
//用户登录应答
const int TID_RecvUserLogin=0x00002006;
//用户退出请求
const int TID_SendUserLogout=0x00002007;
//用户退出应答
const int TID_RecvUserLogout=0x00002008;
//报单录入请求
const int TID_SendOrderInsert=0x00002009;
//报单录入应答
const int TID_RecvOrderInsert=0x0000200A;
//报单操作请求
const int TID_SendOrderAction=0x0000200B;
//报单操作应答
const int TID_RecvOrderAction=0x0000200C;
//触发单录入请求
const int TID_SendTriggerOrderInsert=0x0000200D;
//触发单录入应答
const int TID_RecvTriggerOrderInsert=0x0000200E;
//触发单操作请求
const int TID_SendTriggerOrderAction=0x00002010;
//触发单操作应答
const int TID_RecvTriggerOrderAction=0x00002011;
//止盈止损报单录入请求
const int TID_SendCloseOrderInsert=0x00002012;
//止盈止损报单录入应答
const int TID_RecvCloseOrderInsert=0x00002013;
//止盈止损报单操作请求
const int TID_SendCloseOrderAction=0x0000201A;
//止盈止损报单操作应答
const int TID_RecvCloseOrderAction=0x0000201B;
//报价录入请求
const int TID_SendQuoteInsert=0x00002014;
//报价录入应答
const int TID_RecvQuoteInsert=0x00002015;
//报价操作请求
const int TID_SendQuoteAction=0x00002016;
//报价操作应答
const int TID_RecvQuoteAction=0x00002017;
//订阅主题请求
const int TID_SendTopicAction=0x00002025;
//订阅主题应答
const int TID_RecvTopicAction=0x00002026;
//设置行情请求
const int TID_SendMarketDataUpdate=0x00002035;
//设置行情应答
const int TID_RecvMarketDataUpdate=0x00002036;
//调整持仓请求
const int TID_SendPositionAction=0x00002037;
//调整持仓应答
const int TID_RecvPositionAction=0x00002038;
//用户出入金请求
const int TID_SendAccountAction=0x00002039;
//用户出入金应答
const int TID_RecvAccountAction=0x0000203A;
//成员设置请求
const int TID_SendMemberAction=0x0000203B;
//成员设置应答
const int TID_RecvMemberAction=0x0000203C;
//内部做市商申报请求
const int TID_SendRobotOrder=0x00002041;
//内部做市商申报应答
const int TID_RecvRobotOrder=0x00002042;
//场外成交申报请求
const int TID_SendOTCTradeInsert=0x00002043;
//场外成交申报应答
const int TID_RecvOTCTradeInsert=0x00002044;
//资金结算请求
const int TID_SendSettlement=0x00002045;
//资金结算应答
const int TID_RecvSettlement=0x00002046;
//清理行情数据请求
const int TID_SendClearMarket=0x00002047;
//清理行情数据应答
const int TID_RecvClearMarket=0x00002048;
//清理最新K线请求
const int TID_SendClearLastKLine=0x00002049;
//清理最新K线应答
const int TID_RecvClearLastKLine=0x0000204A;
//重置LocalID请求
const int TID_SendResetLocalID=0x0000204B;
//重置LocalID应答
const int TID_RecvResetLocalID=0x0000204C;
//OToken录入请求
const int TID_SendOTokenInsert=0x0000204D;
//OToken录入应答
const int TID_RecvOTokenInsert=0x0000204E;
//OToken操作请求
const int TID_SendOTokenAction=0x0000204F;
//OToken操作应答
const int TID_RecvOTokenAction=0x00002050;
//APIKEY录入请求
const int TID_SendAPIKeyInsert=0x00002051;
//APIKEY录入应答
const int TID_RecvAPIKeyInsert=0x00002052;
//APIKEY操作请求
const int TID_SendAPIKeyAction=0x00002053;
//APIKEY操作应答
const int TID_RecvAPIKeyAction=0x00002054;
//仓位合并操作请求
const int TID_SendPositionMerge=0x00002055;
//仓位合并操作应答
const int TID_RecvPositionMerge=0x00002056;
//用户入金操作请求
const int TID_SendDeposit=0x00002057;
//用户入金操作应答
const int TID_RecvDeposit=0x00002058;
//用户出金操作请求
const int TID_SendWithdraw=0x00002059;
//用户出金操作应答
const int TID_RecvWithdraw=0x0000205A;
//用户转账操作请求
const int TID_SendTransfer=0x0000205B;
//用户转账操作应答
const int TID_RecvTransfer=0x0000205C;
//注册用户请求
const int TID_SendUserRegister=0x0000205D;
//注册用户应答
const int TID_RecvUserRegister=0x0000205E;
//反向开仓录入请求
const int TID_SendPositionReverse=0x0000205F;
//反向开仓录入应答
const int TID_RecvPositionReverse=0x00002060;
//避免自成交报单录入请求
const int TID_SendCancelOrderInsert=0x00002061;
//避免自成交报单录入应答
const int TID_RecvCancelOrderInsert=0x00002062;
//私有主题分类请求
const int TID_SendPrivateDispatch=0x00002063;
//私有主题分类应答
const int TID_RecvPrivateDispatch=0x00002064;
//调整多空杠杠请求
const int TID_SendPositionLeverage=0x00002065;
//调整多空杠杠应答
const int TID_RecvPositionLeverage=0x00002066;
//设置合约请求
const int TID_SendInstrumentUpdate=0x00002067;
//设置合约应答
const int TID_RecvInstrumentUpdate=0x00002068;
//充币兑换请求
const int TID_SendChangeCurrency=0x00002069;
//充币兑换应答
const int TID_RecvChangeCurrency=0x0000206A;
//钱包登录获取随机数请求
const int TID_SendWalletNonce=0x0000206B;
//钱包登录获取随机数应答
const int TID_RecvWalletNonce=0x0000206C;
//钱包登录获取随机数请求
const int TID_SendWalletLogin=0x0000206D;
//钱包登录获取随机数应答
const int TID_RecvWalletLogin=0x0000206E;
//资金查询请求
const int TID_SendQryAccount=0x00002101;
//资金查询应答
const int TID_RecvQryAccount=0x00002102;
//报单查询请求
const int TID_SendQryOrder=0x00002103;
//报单查询应答
const int TID_RecvQryOrder=0x00002104;
//持仓汇总查询请求
const int TID_SendQryMemberPosition=0x00002109;
//持仓汇总查询应答
const int TID_RecvQryMemberPosition=0x0000210A;
//持仓查询请求
const int TID_SendQryPosition=0x00002111;
//持仓查询应答
const int TID_RecvQryPosition=0x00002112;
//标的查询请求
const int TID_SendQryInstrument=0x00002113;
//标的查询应答
const int TID_RecvQryInstrument=0x00002114;
//币种查询请求
const int TID_SendQryCurrency=0x00002115;
//币种查询应答
const int TID_RecvQryCurrency=0x00002116;
//成员查询请求
const int TID_SendQryMember=0x00002125;
//成员查询应答
const int TID_RecvQryMember=0x00002126;
//价位查询请求
const int TID_SendQryMarketOrder=0x00002127;
//价位查询应答
const int TID_RecvQryMarketOrder=0x00002128;
//APIKEY查询请求
const int TID_SendQryAPIKey=0x00002132;
//APIKEY查询应答
const int TID_RecvQryAPIKey=0x00002133;
//保证金率查询请求
const int TID_SendQryMarginRate=0x00002134;
//保证金率查询应答
const int TID_RecvQryMarginRate=0x00002135;
//手续费率查询请求
const int TID_SendQryFee=0x00002136;
//手续费率查询应答
const int TID_RecvQryFee=0x00002137;
//触发订单查询请求
const int TID_SendQryTriggerOrder=0x00002140;
//触发订单查询应答
const int TID_RecvQryTriggerOrder=0x00002141;
//杠杆倍数查询请求
const int TID_SendQryLeverage=0x00002142;
//杠杆倍数查询应答
const int TID_RecvQryLeverage=0x00002143;
//行情查询请求
const int TID_SendQryMarketData=0x00002144;
//行情查询应答
const int TID_RecvQryMarketData=0x00002145;
//成交查询请求
const int TID_SendQryTrade=0x00002146;
//成交查询应答
const int TID_RecvQryTrade=0x00002147;
//历史委托查询请求
const int TID_SendQryFinishOrder=0x00002148;
//历史委托查询应答
const int TID_RecvQryFinishOrder=0x00002149;
//历史条件单查询请求
const int TID_SendQryFinishTriggerOrder=0x0000214A;
//历史条件单查询应答
const int TID_RecvQryFinishTriggerOrder=0x0000214B;
//指定杠杆倍数查询请求
const int TID_SendQryWithLeverage=0x0000214C;
//指定杠杆倍数查询应答
const int TID_RecvQryWithLeverage=0x0000214D;
//K线查询请求
const int TID_SendQryKLine=0x0000214E;
//K线查询应答
const int TID_RecvQryKLine=0x0000214F;
//线查询请求
const int TID_SendQryLine=0x00002150;
//线查询应答
const int TID_RecvQryLine=0x00002151;
//资金流水查询请求
const int TID_SendQryAccountDetail=0x00002152;
//资金流水查询应答
const int TID_RecvQryAccountDetail=0x00002153;
//结算流水查询请求
const int TID_SendQrySettleDetail=0x00002154;
//结算流水查询应答
const int TID_RecvQrySettleDetail=0x00002155;
//检查TOKEN请求
const int TID_SendCheckToken=0x000021FD;
//检查TOKEN应答
const int TID_RecvCheckToken=0x000021FE;
//批量操作请求(HTTP接口专用)
const int TID_SendBatch=0x000021FF;
//时间同步通知
const int TID_PushTimeSync=0x00002200;
//消息通知
const int TID_PushCommand=0x00002201;
//成交通知
const int TID_PushTrade=0x00002202;
//报单通知
const int TID_PushOrder=0x00002203;
//持仓汇总通知
const int TID_PushMemberPosition=0x00002207;
//持仓通知
const int TID_PushPosition=0x00002208;
//资金通知
const int TID_PushAccount=0x00002209;
//市场成交通知
const int TID_PushMarketTrade=0x00002212;
//订单簿变化通知
const int TID_PushMarketOrder=0x00002213;
//K线通知
const int TID_PushKLine=0x00002214;
//普通行情通知
const int TID_PushMarketOverView=0x00002215;
//全量延时订单簿变化通知
const int TID_PushDelayMarketOrder=0x00002216;
//五档深度行情通知
const int TID_PushFiveDepthMarketData=0x00002217;
//十档深度行情通知
const int TID_PushTenDepthMarketData=0x00002218;
//期权行情通知
const int TID_PushOptionsMarketData=0x00002219;
//手续费通知
const int TID_PushFee=0x0000221B;
//保证金率通知
const int TID_PushMarginRate=0x0000221C;
//成员通知
const int TID_PushMember=0x0000221D;
//触发单通知
const int TID_PushTriggerOrder=0x00002220;
//合约通知
const int TID_PushInstrument=0x00002221;
//用户登陆通知
const int TID_PushUserSession=0x00002222;
//币种通知
const int TID_PushCurrency=0x00002223;
//数据同步开始
const int TID_DataSyncStart=0x00001001;
//数据同步结束
const int TID_DataSyncEnd=0x00001002;
//插入操作请求
const int TID_SendInsert=0x00001003;
//插入操作应答
const int TID_RecvInsert=0x00001004;
//更新操作请求
const int TID_SendUpdate=0x00001005;
//更新操作应答
const int TID_RecvUpdate=0x00001006;
//删除操作请求
const int TID_SendDelete=0x00001007;
//删除操作应答
const int TID_RecvDelete=0x00001008;
//插入没有就更新请求
const int TID_SendInsUpd=0x00001009;
//插入没有就更新应答
const int TID_RecvInsUpd=0x0000100A;
//查询唯一索引记录请求
const int TID_SendQryOne=0x0000100B;
//查询唯一索引记录应答
const int TID_RecvQryOne=0x0000100C;
//设置字段值清空请求
const int TID_SendClear=0x0000100D;
//设置字段值清空应答
const int TID_RecvClear=0x0000100E;
//初始化操作请求
const int TID_SendInit=0x00001010;
//初始化操作应答
const int TID_RecvInit=0x00001011;


#define OF_Topic_Private_Order 3	//私有交易主题,私有交易主题
#define OF_Topic_Private_SnapShot 12	//私有快照主题,私有快照主题
#define OF_Topic_Market_LastTrade 2	//最近成交,最近成交
#define OF_Topic_Market_LastKLine 11	//最近K线(主要是1分钟)推送主题,最近K线(主要是1分钟)推送主题
#define OF_Topic_Market_KLine 4	//历史K线主题,历史K线主题
#define OF_Topic_Market_SnapShot 5	//5档行情快照主题[可通过Numbers降频],5档行情快照主题[可通过Numbers降频]
#define OF_Topic_Market_Broad 6	//广播主题,合约变化,时钟心跳等,广播主题,合约变化,时钟心跳等
#define OF_Topic_Market_OverView 7	//行情概要快照主题,行情概要快照主题
#define OF_Topic_Market_OverView_Delay 17	//行情概要延时快照主题,行情概要延时快照主题
#define OF_Topic_Market_Depth_All 8	//全档行情增量主题,全档行情增量主题
#define OF_Topic_Market_Depth_Delay 18	//全档行情增量延时发送主题,全档行情增量延时发送主题
#define OF_Topic_Market_Depth_25 25	//25档行情增量主题,25档行情增量主题
#define OF_Topic_Market_Depth_50 50	//50档行情增量主题,50档行情增量主题
#define OF_Topic_Market_Depth_100 100	//100档行情增量主题,100档行情增量主题
#define OF_Topic_Private_Risk 9	//风控数据(持仓、资金、行情、时间),分控用户才能订阅,风控数据(持仓、资金、行情、时间),分控用户才能订阅
#define OF_Topic_Private_Trade 13	//私有成交主题,私有成交主题
#define OF_Topic_Private_TriggerOrder 14	//策略触发程序使用的私有主题,策略触发程序使用的私有主题
#define OF_Topic_Private_FinishOrder 16	//管理后台私有完成订单主题,管理后台私有完成订单主题
#define OF_Topic_Private_Copy_Finish 10	//跟单完成订单主题,跟单完成订单主题
#define OF_Topic_Private_UserSession 99	//用户Session主题,用户Session主题

#define S_Topic_Private_Order "3"	//私有交易主题,私有交易主题
#define S_Topic_Private_SnapShot "12"	//私有快照主题,私有快照主题
#define S_Topic_Market_LastTrade "2"	//最近成交,最近成交
#define S_Topic_Market_LastKLine "11"	//最近K线(主要是1分钟)推送主题,最近K线(主要是1分钟)推送主题
#define S_Topic_Market_KLine "4"	//历史K线主题,历史K线主题
#define S_Topic_Market_SnapShot "5"	//5档行情快照主题[可通过Numbers降频],5档行情快照主题[可通过Numbers降频]
#define S_Topic_Market_Broad "6"	//广播主题,合约变化,时钟心跳等,广播主题,合约变化,时钟心跳等
#define S_Topic_Market_OverView "7"	//行情概要快照主题,行情概要快照主题
#define S_Topic_Market_OverView_Delay "17"	//行情概要延时快照主题,行情概要延时快照主题
#define S_Topic_Market_Depth_All "8"	//全档行情增量主题,全档行情增量主题
#define S_Topic_Market_Depth_Delay "18"	//全档行情增量延时发送主题,全档行情增量延时发送主题
#define S_Topic_Market_Depth_25 "25"	//25档行情增量主题,25档行情增量主题
#define S_Topic_Market_Depth_50 "50"	//50档行情增量主题,50档行情增量主题
#define S_Topic_Market_Depth_100 "100"	//100档行情增量主题,100档行情增量主题
#define S_Topic_Private_Risk "9"	//风控数据(持仓、资金、行情、时间),分控用户才能订阅,风控数据(持仓、资金、行情、时间),分控用户才能订阅
#define S_Topic_Private_Trade "13"	//私有成交主题,私有成交主题
#define S_Topic_Private_TriggerOrder "14"	//策略触发程序使用的私有主题,策略触发程序使用的私有主题
#define S_Topic_Private_FinishOrder "16"	//管理后台私有完成订单主题,管理后台私有完成订单主题
#define S_Topic_Private_Copy_Finish "10"	//跟单完成订单主题,跟单完成订单主题
#define S_Topic_Private_UserSession "99"	//用户Session主题,用户Session主题

#endif
