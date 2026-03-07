#ifndef __CMarketMakerInterface_H__
#define __CMarketMakerInterface_H__

#include "FieldData.h"
#include "mdbStruct.h"

class CWorkerCallback
{
public:
	virtual bool SendForceClose(CAccount* pAccount) = 0;
	virtual bool SendForceClose(CPosition* pPosition) = 0;
};

class CWorkerInterface
{
public:
	CWorkerInterface() {};
	~CWorkerInterface() {};

	//时钟处理
	virtual bool TimeSync(CCurrentTimeField *pField) = 0;
	//行情处理
	virtual bool MarketOverViewSync(CMarketOverViewField *pField) = 0;
	//订单触发处理
	virtual bool TriggerOrderSync(CTriggerOrderField *pField) = 0;
	//订单触发结果处理
	virtual bool TriggerOrderActionSync(CTriggerOrderField *pField) = 0;
	//行情最新成交处理
	virtual bool MarketTradeSync(CMarketTradeField *pField) = 0;

private:

};

#endif