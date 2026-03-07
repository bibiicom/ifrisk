/////////////////////////////////////////////////////////////////////////
///CAlterService.h
///定义了CAlterService
/////////////////////////////////////////////////////////////////////////

#ifndef CAlterService_H
#define CAlterService_H

#include "CErrorEngine.h"
#include "tradingTools.h"
#include "ServiceInterface.h"

/////////////////////////////////////////////////////////////////////////
///CDbmtServiceBase是一个完成数据同步工作的类
///内存数据库的数据
///////////////////////////////////////////////////////////////////////// 
class CAlterService
{
public:
	///构造方法
	///@param	pMDB	指定的内存数据库
	///@param	pErrorEngine	指定的错误引擎
	CAlterService(CMDB *pMDB, CErrorEngine *pErrorEngine);

	///析构方法
	virtual ~CAlterService();
	
public:
	///同步数据库表的实现
	///@param	pCurrentTime	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncCurrentTimeImpl(int nTid, CCurrentTimeField *pCurrentTime, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pMember	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncMemberImpl(int nTid, CMemberField *pMember, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pServiceConfig	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncServiceConfigImpl(int nTid, CServiceConfigField *pServiceConfig, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pUserSession	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncUserSessionImpl(int nTid, CUserSessionField *pUserSession, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pInstrument	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncInstrumentImpl(int nTid, CInstrumentField *pInstrument, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pTradingRight	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncTradingRightImpl(int nTid, CTradingRightField *pTradingRight, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pMarginRate	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncMarginRateImpl(int nTid, CMarginRateField *pMarginRate, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pCFDRate	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncCFDRateImpl(int nTid, CCFDRateField *pCFDRate, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pFee	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncFeeImpl(int nTid, CFeeField *pFee, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pCurrency	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncCurrencyImpl(int nTid, CCurrencyField *pCurrency, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pLiquidity	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncLiquidityImpl(int nTid, CLiquidityField *pLiquidity, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pAccount	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncAccountImpl(int nTid, CAccountField *pAccount, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pMemberPosition	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncMemberPositionImpl(int nTid, CMemberPositionField *pMemberPosition, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pPosition	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncPositionImpl(int nTid, CPositionField *pPosition, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pMarketData	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncMarketDataImpl(int nTid, CMarketDataField *pMarketData, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pOrder	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncOrderImpl(int nTid, COrderField *pOrder, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pPositionOrder	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncPositionOrderImpl(int nTid, CPositionOrderField *pPositionOrder, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pTriggerOrder	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncTriggerOrderImpl(int nTid, CTriggerOrderField *pTriggerOrder, CTransaction *pTransaction);
	///同步数据库表的实现
	///@param	pLastKLine	要同步的信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool SyncLastKLineImpl(int nTid, CLastKLineField *pLastKLine, CTransaction *pTransaction);
	///数据同步后处理
	///@param	pTransaction	本处理属于的事务
	virtual void afterSync(CTransaction *pTransaction);
	
	///Link数据检查
	///@param	pFile	检查结果输出文件
	virtual void checkLink(FILE *pFile) const;
	
	///Null数据检查
	///@param	pFile	检查结果输出文件
	virtual void checkNull(FILE *pFile) const;
	
	///Valid数据检查
	///@param	pFile	检查结果输出文件
	virtual void checkValid(FILE *pFile) const;	
	
protected:
	CMDB *m_pMDB;
	CErrorEngine *m_pErrorEngine;
};
#endif
