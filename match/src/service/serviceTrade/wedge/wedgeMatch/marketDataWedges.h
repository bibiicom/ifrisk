#ifndef MARKETDATAWedgeS_H
#define MARKETDATAWedgeS_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"

class COrderMBLTrigger : public COrderCommitTrigger
{
public:
	///构造方法，创建限价行情的报单触发器
	///@param	pMDB	使用的内存数据库
	COrderMBLTrigger(CMDB *pMDB);

	///析构方法，将负责释放所占用的空间
	virtual ~COrderMBLTrigger(void);

	///确认加入后触发
	///@param	pOrder	已经加入的Order
	virtual void commitAdd(COrder *pOrder);

	///确认更新后触发
	///@param	pOrder	被刷新的COrder
	///@param	pOldOrder	原来的值
	virtual void commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder);

	///确认删除后触发
	///@param	pOrder	被删除前的COrder
	virtual void commitRemove(CWriteableOrder *pOrder);

protected:
	///使用的内存数据库
	CMDB *m_pMDB;

//	CTradeService* m_pTradeService;

	void alterMBLData(CInstrument *pInstrument, CReadOnlyDirectionType &direction, CReadOnlyPriceType &price, CReadOnlyVolumeType &volumeChange, CReadOnlyVolumeType &trueVolumeChange, CReadOnlyNumberType &orderNumberChange);
	//void alterTickMBLData(CInstrument *pInstrument, CReadOnlyDirectionType &direction, CReadOnlyPriceType &price, CReadOnlyPriceType &tick, CReadOnlyVolumeType &volumeChange, CReadOnlyNumberType &orderNumberChange);
	CWriteableMarketOrder theWriteableMarketOrder;
};

/////////////////////////////////////////////////////////////////////////
///md01:CMarketDataGenerateWedge是产生行情的交易规则
/////////////////////////////////////////////////////////////////////////
class CMarketDataGenerateWedge : public CWedge
{
public:
	///构造方法，创建行情生成交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	///@param	genSettlementPrice	是否生成结算价
	CMarketDataGenerateWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	~CMarketDataGenerateWedge(void);

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_updateInstrumentStatus;
		result |= USE_changePosition;
		result |= USE_timeSync;
		return result;
	}

	virtual bool updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction);

	virtual bool changePosition(CPositionChangeList *pChangeList,CTransaction *pTransaction);

	virtual bool timeSync(CWriteableCurrentTime *pCurrentTime);
protected:

	///判断是否需要在行情中显示成交数量和金额
	///@param	pTrade	指定的成交
	///@return	true表示需要，false表示不需要
	//virtual bool needShowVolumeAmount(CTrade *pTrade);

	///判断是否需要在行情中显示总持仓量
	///@param	pTrade	指定的成交
	///@return	true表示需要，false表示不需要
//	virtual bool needShowPosition(CTrade *pTrade);

	///时间
	CDateType m_CurrDate;
	CWriteableMarketData theMarketData;
};

class CMarketOverViewTrigger : public CMarketDataCommitTrigger
{
public:
	CMarketOverViewTrigger(CServiceResponser *pServiceResponser, CMDB *pMDB);
	virtual ~CMarketOverViewTrigger();
	virtual void commitAdd(CMarketData *pMarketData);
	virtual void commitUpdate(CMarketData *pMarketData, CWriteableMarketData *pOldMarketData);

private:
	CMDB *m_pMDB;
	CServiceResponser *m_pServiceResponser;
};

#endif
