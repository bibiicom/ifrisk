#ifndef __CMarketWorker_H__
#define __CMarketWorker_H__

#include "utility.h"
#include "FieldData.h"
#include "WorkerInterface.h"

#define MaxOrders 50

typedef map<double, COrderField, less<double> > CSellOrderMap;
typedef map<double, COrderField, greater<double> > CBuyOrderMap;

typedef set<double> COrderPriceSet;

#define WorkerTypeMaker 'M'

class CMarketWorker :public CWorkerInterface
{
public:
	CMarketWorker(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CWorkerCallback *pMarketMaker);
	~CMarketWorker();

	//一般处理（逐笔行情）
	bool SaveOrder(COrderField *pField) { return true; };
	bool InitOrder(COrderField *pField);	//查询之后的订单，全量初始化
	bool SaveMarketTrade(CMarketTradeField *pField); //主要处理成交
	bool SaveMarketOrder(CMarketOrderField *pField);
	bool Init();

	bool SaveMarketOverView(CMarketOverViewField *pField);

	virtual CExchangeIDType GetExchangeID()
	{
		return m_OrderInsertField.ExchangeID;
	};
	virtual CInstrumentIDType GetInstrumentID()
	{
		return m_OrderInsertField.InstrumentID;
	};
	virtual char GetWorkerType()
	{
		return WorkerTypeMaker;
	}
	//比较此价格的订单和行情的差距，做出调整
	bool ChangeOrder(CPriceType LastOrderPrice, CVolumeType Volume, CDirectionType Direction);

private:

	CBuyOrderMap m_BuyMarketOrders;//存储所有的买订单快照
	CSellOrderMap m_SellMarketOrders;//存储所有的卖订单快照

	CWorkerCallback *m_pMarketMaker;
	COrderInsertField m_OrderInsertField;
	//COrderPriceSet m_TodoOrderPrice;//需要处理的OrderPrice集合

	//由于使用快照行情需要特殊的算法，以下是算法需要的临时变量
	//CTenDepthMarketDataField m_DepthMarketDataField;
	//CMarketDataField m_MarketDataField;
	COTCTradeInsertField m_OTCTradeInsertField;
	CQryOrderField m_QryOrderField;
	char m_PrintInfo[128];
	CPriceType m_PriceTick;
	CMarketDataUpdateField m_MarketDataUpdateField;
	int m_nPriceTickCount;
};

#endif
