#ifndef __CMarketMakerInterface_H__
#define __CMarketMakerInterface_H__

#include "FieldData.h"

class CWorkerCallback
{
public:
	virtual bool SendOrder(COrderInsertField *pField) = 0;
	virtual bool SendOTCTrade(COTCTradeInsertField *pField) = 0;
	virtual bool CancelOrder(COrderField *pField) = 0;
	virtual bool GetOrder(CQryOrderField *pField) = 0;
	virtual bool SendMarketUpdate(CMarketDataUpdateField *pField) = 0;
	virtual bool SendInstrumentUpdate(CInstrumentUpdateField *pField) = 0;
};

class CWorkerInterface
{
public:
	CWorkerInterface(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CWorkerCallback *pMarketMaker)
	{
	};
	~CWorkerInterface() {};

	//一般处理（逐笔行情）
	virtual bool SaveOrder(COrderField *pField) = 0;	//主要处理对比行情之后的撤单
	//查询之后的订单初始化
	virtual bool InitOrder(COrderField *pField) = 0;
	 //主要处理成交
	virtual bool SaveMarketTrade(CMarketTradeField *pField) = 0;
	//订单类型
	virtual bool SaveMarketOrder(CMarketOrderField *pField) = 0;
	//主要更新的指数和资金费率等一般信息
	virtual bool SaveMarketOverView(CMarketOverViewField *pField) = 0;
	//初始化，断线重连的时候需要做
	virtual bool Init() = 0;

	virtual CExchangeIDType GetExchangeID() = 0;
	virtual CInstrumentIDType GetInstrumentID() = 0;

	virtual char GetWorkerType() = 0;

private:
};

#endif