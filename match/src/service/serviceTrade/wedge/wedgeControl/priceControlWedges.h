/////////////////////////////////////////////////////////////////////////
///priceControlWedges.h
///定义了多个价格控制的交易规则类
/////////////////////////////////////////////////////////////////////////

#ifndef PRICECONTROLWedgeS_H
#define PRICECONTROLWedgeS_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"
#include "CopyField.h"
#include "monitorIndex.h"

/////////////////////////////////////////////////////////////////////////
///price01:CPriceCheckWedge是进行价格检查的交易规则
/////////////////////////////////////////////////////////////////////////
class CPriceCheckWedge : public CWedge
{
public:
	///构造方法，创建价格检查交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	///@param	pAlgo	停板价计算方法
	CPriceCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
		:CWedge(pTradeService, pMDB, pErrorEngine)
	{
		m_sName = "PriceCheck";
	}

	///析构方法
	~CPriceCheckWedge(void)
	{
	}

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_insertOrder;
		result |= USE_orderAction;
		result |= USE_changePosition;
		return result;
	}

	virtual bool insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction);
	virtual bool orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction);
	virtual bool changePosition(CPositionChangeList *pChangeList, CTransaction *pTransaction);
	double getMaxToTradeVolume(double Price, char Direction, const char* ExchangeID, const char* InstrumentID);

private:
	///判断价格是否合理
	///@param	price	要判断的数量
	///@return	true表示合理，false表示不合理
	//bool validPrice(CPriceType price);
};

#endif
