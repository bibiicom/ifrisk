#ifndef _MDTRIGGERS_H_
#define _MDTRIGGERS_H_

#include "mdbStruct.h"
#include "mdb.h"
#include "ServiceInterface.h"
#include "FieldData.h"
#include "MarketDataService.h"

const int TOPIC_MARKETDATA_VERSION_INTERVAL = 1000;


class CMarketOrderNotifyTrigger :public CMarketOrderCommitTrigger
{
public:
	CMarketOrderNotifyTrigger(CMarketDataService *pMarketDataService);

	///加入后触发
	///@param	pMarketOrder	已经加入的MarketOrder
	virtual void commitAdd(CMarketOrder *pMarketOrder);

	///更新后触发
	///@param	pMarketOrder	被刷新的CMarketOrder
	///@param	poldMarketOrder	原来的值
	virtual void commitUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pOldMarketOrder);

	///删除后触发
	///@param	pMarketOrder	已经删除的CMarketOrder
	virtual void commitRemove(CWriteableMarketOrder *pMarketOrder);

private:

	CMarketDataService *m_pMarketDataService;
};

class CTickMarketOrderNotifyTrigger :public CTickMarketOrderCommitTrigger
{
public:
	CTickMarketOrderNotifyTrigger(CMarketDataService* pMarketDataService);

	///加入后触发
	///@param	pMarketOrder	已经加入的MarketOrder
	virtual void commitAdd(CTickMarketOrder* pMarketOrder);

	///更新后触发
	///@param	pMarketOrder	被刷新的CMarketOrder
	///@param	poldMarketOrder	原来的值
	virtual void commitUpdate(CTickMarketOrder* pMarketOrder, CWriteableTickMarketOrder* pOldMarketOrder);

	///删除后触发
	///@param	pMarketOrder	已经删除的CMarketOrder
	virtual void commitRemove(CWriteableTickMarketOrder* pMarketOrder);

private:

	CMarketDataService* m_pMarketDataService;
};

#endif