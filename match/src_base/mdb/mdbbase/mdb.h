/////////////////////////////////////////////////////////////////////////
///mdb.h
///定义了类CMDB
/////////////////////////////////////////////////////////////////////////

#ifndef CMEMORYDB_H
#define CMEMORYDB_H

#include "CBaseObject.h"
#include "mdbStruct.h"
#include "mdbTable.h"
#include "mdbIter.h"

struct TableStruct
{
	char tableName[100];
	int  reuseID;
};

class CMDBSnap
{
public:
	CMDBSnap()
	{
		m_Status = 0;
	}
	virtual ~CMDBSnap() {};
	//0:初始化状态(快照同步)，可以生成快照
	//1:快照生成完成，可以同步快照到磁盘
	volatile int m_Status;
	vector<CWriteableCurrentTime> m_CurrentTime;	//当前时间表
	vector<CWriteableMember> m_Member;	//成员表
	vector<CWriteableServiceConfig> m_ServiceConfig;	//业务配置表
	vector<CWriteableUserSession> m_UserSession;	//交易员在线会话表
	vector<CWriteableInstrument> m_Instrument;	//标的表
	vector<CWriteableTradingRight> m_TradingRight;	//交易权限表
	vector<CWriteableMarginRate> m_MarginRate;	//标的保证金率表
	vector<CWriteableCFDRate> m_CFDRate;	//CFD溢价率表
	vector<CWriteableFee> m_Fee;	//手续费率表
	vector<CWriteableCurrency> m_Currency;	//资金账户币种信息表
	vector<CWriteableLiquidity> m_Liquidity;	//流动性设置表表
	vector<CWriteableAccount> m_Account;	//资金账户表
	vector<CWriteableMemberPosition> m_MemberPosition;	//成员总持仓表
	vector<CWriteablePosition> m_Position;	//交易单元持仓表
	vector<CWriteableMarketData> m_MarketData;	//行情表
	vector<CWriteableOrder> m_Order;	//报单表
	vector<CWriteablePositionOrder> m_PositionOrder;	//未平仓报单表
	vector<CWriteableTriggerOrder> m_TriggerOrder;	//报单表
	vector<CWriteableMarketOrder> m_MarketOrder;	//分价表表
	vector<CWriteableTickMarketOrder> m_TickMarketOrder;	//Tick聚集分价表表
	vector<CWriteableTrade> m_Trade;	//成交表
	vector<CWriteableKLine> m_KLine;	//K线表
	vector<CWriteablePublishKLine> m_PublishKLine;	//PublishK线表
	vector<CWriteableLastKLine> m_LastKLine;	//最新K线表
	vector<CWriteableAccountDetail> m_AccountDetail;	//资金明细表
	vector<CWriteableSettleDetail> m_SettleDetail;	//结算明细表
};

/////////////////////////////////////////////////////////////////////////
///CMDB是一个内存数据库类，包含了所有定义的表信息
///@author	xuzh
///@version	1.0,20050824
/////////////////////////////////////////////////////////////////////////
class CMDB : public CBaseObject
{
public:
	///构造方法，创建内存数据库
	CMDB();
	
	///析构方法，将负责释放所占用的空间
	virtual ~CMDB();
	
	virtual int isA(char *objectType);
	virtual const char *getType();

	///进行初始化工作
	///@param	pConfig	初始化时使用的配置信息
	///@return	1表示成功，0表示失败
	virtual int init(int memorySize, int blockCount, IMemoryAllocator *pAllocator=NULL, bool reuse=false);
	
	///进行数据载入工作
	///@param	path	载入指定路径的数据
	///@return	1表示成功，0表示失败
	virtual int load(const char *path);

	///进行WriteableTable数据存储工作
	///@param	path	保存到指定路径
	///@return	1表示成功，0表示失败
	virtual int save(const char *path, UF_INT8 nSequenceNo=0.0);
	
	//存储前一个版本的数据
	//用于比较和前一个版本的数据
	///@param	path	保存到指定路径
	///@return	1表示成功，0表示失败	
	virtual int savePreVersion(const char *path, UF_INT8 nSequenceNo=0.0);
	
	///进行所有数据存储工作
	///@param	path	保存到指定路径
	///@return	1表示成功，0表示失败
	virtual int saveAll(const char *path, UF_INT8 nSequenceNo=0.0);
	virtual int saveAllPreVersion(const char *path, UF_INT8 nSequenceNo=0.0);	

	///清空所有的数据
	///@return	1表示成功，0表示失败
	virtual void clear();
	
	///完成所有外键的连接
	virtual void linkAll();
	
	///Link数据检查
	///@param	pFile	检查结果输出文件
	virtual void checkLink(FILE *pFile) const;
	
	///Link数据清理
	virtual void clearLink();	
	
	///Null数据检查
	///@param	pFile	检查结果输出文件
	virtual void checkNull(FILE *pFile) const;
	
	///Valid数据检查
	///@param	pFile	检查结果输出文件
	virtual void checkValid(FILE *pFile) const;	
	
	//为了异步写文件快照
	bool snap(const char *path, UF_INT8 nSequenceNo = 0.0);
	bool snapAll(const char *path, UF_INT8 nSequenceNo = 0.0);	
	bool saveSnap();
	bool clearSnap();
	bool canSnap();

	CCurrentTimeFactory *m_CurrentTimeFactory;		//当前时间表
	CMemberFactory *m_MemberFactory;		//成员表
	CServiceConfigFactory *m_ServiceConfigFactory;		//业务配置表
	CUserSessionFactory *m_UserSessionFactory;		//交易员在线会话表
	CInstrumentFactory *m_InstrumentFactory;		//标的表
	CTradingRightFactory *m_TradingRightFactory;		//交易权限表
	CMarginRateFactory *m_MarginRateFactory;		//标的保证金率表
	CCFDRateFactory *m_CFDRateFactory;		//CFD溢价率表
	CFeeFactory *m_FeeFactory;		//手续费率表
	CCurrencyFactory *m_CurrencyFactory;		//资金账户币种信息表
	CLiquidityFactory *m_LiquidityFactory;		//流动性设置表表
	CAccountFactory *m_AccountFactory;		//资金账户表
	CMemberPositionFactory *m_MemberPositionFactory;		//成员总持仓表
	CPositionFactory *m_PositionFactory;		//交易单元持仓表
	CMarketDataFactory *m_MarketDataFactory;		//行情表
	COrderFactory *m_OrderFactory;		//报单表
	CPositionOrderFactory *m_PositionOrderFactory;		//未平仓报单表
	CTriggerOrderFactory *m_TriggerOrderFactory;		//报单表
	CMarketOrderFactory *m_MarketOrderFactory;		//分价表表
	CTickMarketOrderFactory *m_TickMarketOrderFactory;		//Tick聚集分价表表
	CTradeFactory *m_TradeFactory;		//成交表
	CKLineFactory *m_KLineFactory;		//K线表
	CPublishKLineFactory *m_PublishKLineFactory;		//PublishK线表
	CLastKLineFactory *m_LastKLineFactory;		//最新K线表
	CAccountDetailFactory *m_AccountDetailFactory;		//资金明细表
	CSettleDetailFactory *m_SettleDetailFactory;		//结算明细表
private:
	CFixMem *pMem;		///用来存放表的信息
	CMDBSnap m_MDBLastSnap;
	char m_sLastSnapPath[128];
	UF_INT8 m_nLastSequenceNo;
};

#endif
