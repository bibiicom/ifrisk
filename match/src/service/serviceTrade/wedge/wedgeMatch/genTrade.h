#ifndef TRADINGMODELS_H
#define TRADINGMODELS_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"

class ITradeMaker
{
public:
	///析构方法
	virtual ~ITradeMaker(void)
	{
	}

	///根据两张普通报单，产生成交
	///@param	pBuyOrder	买入报单
	///@param	pSellOrder	卖出报单
	///@param	price	成交价格
	///@param	priceSource	价格来源
	///@param	volume	成交数量
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	virtual void genTrade(CWriteableOrder *pBuyOrder, CWriteableOrder *pSellOrder, CReadOnlyPriceType &price, CReadOnlyVolumeType &volume, char BuyOrderMatchRole) = 0;

	///根据一张普通报单和一张非标准组合报单，产生成交
	///@param	pOrder	指定的普通报单
	///@param	pCombOrder	指定的非标准组合
	///@param	volume	成交量
	///@param	offsetFlag	非标组合的开平标记
	///@param	PositionFlag	非标组合的投保标记
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示产生成交成功，false表示产生成交失败
	//virtual bool genTrade(CWriteableOrder *pOrder, CWriteableCombOrder *pCombOrder,
	//	CReadOnlyVolumeType volume, CReadOnlyOffsetFlagType &offsetFlag, char BuyOrderMatchRole,CTransaction *pTransaction) = 0;

	///根据普通报单，产生单边成交
	///@param	pInstrument	指定的合约
	///@param	pOrder	指定报单
	///@param	direction	买卖方向
	///@param	price	成交价格
	///@param	priceSource	价格来源
	///@param	volume	成交数量
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@param	pTradeID	使用的成交编号，在输入时，如果为""，则表示自由编号，如果其他值，则作为编号。返回时放置实际的成交编号
	///@param	tradeType	成交类型
	///@param	matchRole	成交角色，taker还是maker
	///@param	legID	分腿编号
	virtual void genSingleTrade(CInstrument *pInstrument, CWriteableOrder *pOrder, CReadOnlyDirectionType &direction, CReadOnlyPriceType &price,
		CReadOnlyVolumeType &volume, CIndexIDType *pTradeID, CReadOnlyDeriveSourceType &DeriveSource, char matchRole, bool bSelfTrade) = 0;

	///根据非标准组合报单，产生单边成交
	///@param	pInstrument	指定的合约
	///@param	pCombOrder	指定的非标准报单
	///@param	direction	买卖方向
	///@param	price	成交价格
	///@param	priceSource	价格来源
	///@param	volume	成交数量
	///@param	offsetFlag	非标组合的开平标记
	///@param	PositionFlag	非标组合的投保标记
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@param	pTradeID	使用的成交编号，在输入时，如果为""，则表示自由编号，如果其他值，则作为编号。返回时放置实际的成交编号
	///@param	tradeType	成交类型
	///@param	matchRole	成交角色，taker还是maker
	///@return	true表示产生成交成功，false表示产生成交失败
	//virtual bool genSingleTrade(CInstrument *pInstrument, CWriteableCombOrder *pCombOrder, CReadOnlyDirectionType &direction, CReadOnlyPriceType &price, CReadOnlyVolumeType &volume,
	//	CReadOnlyOffsetFlagType &offsetFlag,CTransaction *pTransaction, CIndexIDType *pTradeID, CReadOnlyDeriveSourceType &DeriveSource, char matchRole, bool bSelfTrade) = 0;

};

/////////////////////////////////////////////////////////////////////////
///CSingleTradeMaker是简单合约成交产生器
/////////////////////////////////////////////////////////////////////////
class CSingleTradeMaker : public ITradeMaker
{
public:
	///构造方法，创建简单合约成交产生器
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	CSingleTradeMaker(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	///产生成交
	///@param	pBuyOrder	买入报单
	///@param	pSellOrder	卖出报单
	///@param	price	成交价格
	///@param	priceSource	价格来源
	///@param	volume	成交数量
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	virtual void genTrade(CWriteableOrder *pBuyOrder, CWriteableOrder *pSellOrder, CReadOnlyPriceType &price, CReadOnlyVolumeType &volume, char BuyOrderMatchRole);

	///根据一张普通报单和一张非标准组合报单，产生成交
	///@param	pOrder	指定的普通报单
	///@param	pCombOrder	指定的非标准组合
	///@param	volume	成交量
	///@param	offsetFlag	非标组合的开平标记
	///@param	PositionFlag	非标组合的投保标记
	///@param	pSession	会话编号
	///@param	pTransaction	本操作属于的事务
	///@return	true表示产生成交成功，false表示产生成交失败
	//virtual bool genTrade(CWriteableOrder *pOrder, CWriteableCombOrder *pCombOrder,	CReadOnlyVolumeType volume, CReadOnlyOffsetFlagType &offsetFlag, 
	//	char BuyOrderMatchRole,CTransaction *pTransaction);

	virtual void genSingleTrade(CInstrument *pInstrument, CWriteableOrder *pOrder, CReadOnlyDirectionType &direction, CReadOnlyPriceType &price,
		CReadOnlyVolumeType &volume, CIndexIDType *pTradeID, CReadOnlyDeriveSourceType &DeriveSource, char matchRole, bool bSelfTrade);

	//virtual bool genSingleTrade(CInstrument *pInstrument, CWriteableCombOrder *pCombOrder, CReadOnlyDirectionType &direction, CReadOnlyPriceType &price, CReadOnlyPriceSourceType &priceSource, CReadOnlyVolumeType &volume,
	//	CReadOnlyOffsetFlagType &offsetFlag,CTransaction *pTransaction, CIndexIDType *pTradeID, CReadOnlyDeriveSourceType &DeriveSource, char matchRole, bool bSelfTrade);

protected:
	///交易服务
	CTradeService *m_TradeService;

	///使用的交易数据
	CMDB *m_pMDB;

	///使用的错误引擎
	CErrorEngine *m_pErrorEngine;
};


#endif
