#ifndef CPositionChangeList_H
#define CPositionChangeList_H

#include "databaseData.h"

///持仓变化的类型
typedef enum
{
	IS_FROZEN,	///冻结发生变化
	IS_TRADE	///成交造成的真实持仓变化
}TPositionChangeType;

/////////////////////////////////////////////////////////////////////////
///CPositionChange标识一个按照综合持仓方式理解的客户持仓变化
/////////////////////////////////////////////////////////////////////////
class CPositionChange
{
public:
	///持仓方向
	CPosiDirectionType posiDirection;

	///持仓变化类型
	TPositionChangeType positionChangeType;

	///持仓变化量
	CVolumeType volumeChange;

	///造成持仓变化时使用的清算价格
	///tom 这里使用的价格是清算货币的价格
	//CPriceType clearPrice;

	///造成持仓变化时使用的报价价格
	//CPriceType price;

	///是否开仓
	bool isOpen;

};

/////////////////////////////////////////////////////////////////////////
///CPositionChangeList标识一个按照综合方式理解的客户持仓变化列表
/////////////////////////////////////////////////////////////////////////
class CPositionChangeList
{
public:
	///会员号
//	CReadOnlyIndexIDType *pMemberID;

	///客户号
//	CReadOnlyIndexIDType *pTradeUnitID;

	///合约
	CInstrument *pInstrument;

	///是否是今日仓
	bool isToday;
	
	///买卖方向
	CDirectionType direction;

	///老的持仓记录，可能为NULL
	CPosition *pOldPosition;

	///新的持仓记录
	CWriteablePosition *pNewPosition;
	
	///造成变化的报单，如果不是报单造成的，则为NULL
	CWriteableOrder *pOrder;

	COrder *pOldOrder;

	///造成变化的成交，如果不是成交造成的，则为NULL
	CWriteableTrade *pTrade;
	
	///会员客户纪录
//	CTradeUnit *pTradeUnit;

	//造成变化的撤单，如果不是撤单造成的，则为NULL
	COrderAction *pOrderCancle;

	COrderAction *pOrderAction;
	
	//多头开仓数量，负数为平仓数量
	CVolumeType m_LongOpenVolume;

	//空头开仓梳理，负数为平仓数量
	CVolumeType m_ShortOpenVolume;

	CMarginRate *pRate;

	///加入一个持仓变化
	///@param	posiDirection	变化方向
	///@param	isOpen	是否开仓
	///@param	positionChangeType	持仓变化类型
	///@param	volumeChange	持仓变化量
	///@param	price	造成持仓变化的价格
	//void add(CPosiDirectionType posiDirection, TPositionChangeType positionChangeType, CVolumeType volumeChange, CReadOnlyPriceType &price, bool isOpen, CBoolType isInverse);
	void add(CPosiDirectionType posiDirection, TPositionChangeType positionChangeType, CVolumeType volumeChange, bool isOpen);

	///获取某个编号的持仓变化
	///@param	id	持仓变化的编号
	///@return	得到的持仓变化，如果没有此编号，则返回NULL
	CPositionChange *get(int id);

	///清空所有的持仓变化
	void clear();

	///检查是否发生了变化
	///@param	pLongChanged	返回多头是否发生了变化，NULL表示不需要返回
	///@param	pShortChanged	返回空头是否发生了变化，NULL表示不需要返回
	///@param	true表示发生了变化，false表示没有发生变化
	bool isChanged(bool *pLongChanged=NULL, bool *pShortChanged=NULL);

	///是否发生了开仓
	///@param	pLongOpen	返回多头是否发生了开仓，NULL表示不需要返回
	///@param	pShortOpen	返回空头是否发生了开仓，NULL表示不需要返回
	///@return	true表示发生了开仓，false表示没有发生开仓
	bool isOpen(bool *pLongOpen=NULL, bool *pShortOpen=NULL);

	///计算开仓量
	///@param	pLongVolume	返回多头开仓量
	///@param	pShortVolume	返回空头开仓量
	void calculateOpenVolume(CVolumeType *pLongVolume, CVolumeType *pShortVolume);

	///计算平仓量
	///@param	pLongVolume	返回多头平仓量
	///@param	pShortVolume	返回空头平仓量
	void calculateCloseVolume(CVolumeType *pLongVolume, CVolumeType *pShortVolume);

	///是否有成交
	///@param	pTradePrice	如果有成交，则返回成交的价格
	///@return	true表示有成交，false表示没有成交
	//bool hasTrade(CPriceType *pTradePrice);
private:
	///变化数量
	int changeCount;

	///所有变化的数组
	CPositionChange changes[4];
};

#endif
