#ifndef DEPOSITALGORITHMS_H
#define DEPOSITALGORITHMS_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"

class CDepositChange
{
public:
	///构造函数
	CDepositChange()
	{
		marginChange = 0.0;
		maintMarginChange = 0.0;
		longFrozenChange = 0.0;
		shortFrozenChange = 0.0;
		frozenMarginChange = 0.0;
		closeProfit = 0.0;
		totalCloseProfit = 0.0;
		positionCostChange = 0.0;
		ydPositionCostChange = 0.0;
		totalPositionCostChange = 0.0;
		frozenFeeChange = 0.0;
		feeChange = 0.0;
		moneyChange = 0.0;
		frozenMoneyChange = 0.0;
		feeVolumeChange = 0.0;
		volumeChange = 0.0;
		frozenVolumeChange = 0.0;
		toCloseProfit = 0.0;
		isCrossMargin = true;
		copyProfit = 0.0;
	}

	//保证金变化
	CMoneyType marginChange;
	//维持保证金变化
	CMoneyType maintMarginChange;
	//多头冻结资金变化
	CMoneyType longFrozenChange;
	//空头冻结资金变化
	CMoneyType shortFrozenChange;
	//冻结保证金变化
	CMoneyType frozenMarginChange;
	//平仓盈亏
	CMoneyType closeProfit;
	//全部的平仓盈亏
	CMoneyType totalCloseProfit;
	//今日持仓成本变化
	CMoneyType positionCostChange;
	//昨日持仓成本变化
	CMoneyType ydPositionCostChange;
	//所有持仓成本变化
	CMoneyType totalPositionCostChange;
	//手续费的变化
	CMoneyType feeChange;
	//冻结手续费的变化
	CMoneyType frozenFeeChange;
	//账户增加的资金
	CMoneyType moneyChange;
	//冻结资金的变化
	CMoneyType frozenMoneyChange;
	//数量手续费的变化
	CVolumeType feeVolumeChange;
	//减少的数量
	CVolumeType volumeChange;
	//冻结的数量
	CVolumeType frozenVolumeChange;
	//预计亏损
	CMoneyType toCloseProfit;
	//是否全仓
	bool isCrossMargin;
	CMoneyType copyProfit; 
};

/////////////////////////////////////////////////////////////////////////
///IDepositAlgorithms是资金算法的抽象类
/////////////////////////////////////////////////////////////////////////
class IDepositAlgorithms
{
public:
	IDepositAlgorithms(CMDB *pMDB)
	{
		m_pMDB = pMDB;
	}
	virtual ~IDepositAlgorithms()
	{
	}

	virtual void calculateDepositDifference1(CPositionChangeList *pList, CDepositChange *pDepositChange, CTransaction *pTransaction) = 0;
	virtual void calculateDepositDifference2(CPositionChangeList *pList, CDepositChange *pDepositChange, CTransaction *pTransaction) = 0;

	CMDB *m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CBaseDepositAlgorithms是基础的资金算法类
/////////////////////////////////////////////////////////////////////////
class CBaseDepositAlgorithms : public IDepositAlgorithms
{
public:

	CBaseDepositAlgorithms(CMDB *pMDB) :IDepositAlgorithms(pMDB){}

	virtual void calculateDepositDifference1(CPositionChangeList *pList, CDepositChange *pDepositChange, CTransaction *pTransaction);
	virtual void calculateDepositDifference2(CPositionChangeList *pList, CDepositChange *pDepositChange, CTransaction *pTransaction);

	//virtual void calculateMarginChange(CPositionChangeList *pList, CDepositChange *pDepositChange);
	void calculateMarginChangeEach(CPositionChange *pPositionChange, CPositionChangeList *pList, CDepositChange *pDepositChange);

	//virtual void calculateMarginFrozenChange(CPositionChangeList *pList, CDepositChange *pDepositChange);
	void calculateMarginFrozenChangeEach(CPositionChange *pPositionChange,  CPositionChangeList *pList, CDepositChange *pDepositChange);

	virtual void calculateCloseProfitAndPositionCost(CPositionChange *pPositionChange, CPositionChangeList *pList, CDepositChange *pDepositChange);
	virtual void calculateToLoss(CPositionChange *pPositionChange, CPositionChangeList *pList, CDepositChange *pDepositChange);

	virtual void calculateFee(CPositionChange *pPositionChange, CPositionChangeList *pList, CDepositChange *pDepositChange);
	virtual void calculatePremiumChange(CPositionChange *pPositionChange, CPositionChangeList *pList, CDepositChange *pDepositChange);
	virtual void calculateUnFrozen(CPositionChange *pPositionChange, CPositionChangeList *pList, CDepositChange *pDepositChange);

};


class CExchangeDepositAlgorithms : public IDepositAlgorithms
{
public:
	CExchangeDepositAlgorithms(CMDB *pMDB) :IDepositAlgorithms(pMDB){}

	virtual void calculateDepositDifference1(CPositionChangeList *pList,  CDepositChange *pDepositChange, CTransaction *pTransaction);
	virtual void calculateDepositDifference2(CPositionChangeList *pList, CDepositChange *pDepositChange, CTransaction *pTransaction);
	virtual void calculateFee(CPositionChange *pPositionChange, CPositionChangeList *pList, CDepositChange *pDepositChange);
	virtual void calculateUnFrozen(CPositionChange *pPositionChange, CPositionChangeList *pList, CDepositChange *pDepositChange);
	virtual void calculateMoneyChange(CPositionChange *pPositionChange, CPositionChangeList *pList, CDepositChange *pDepositChange);
	virtual void calculateCloseProfitAndPositionCost(CPositionChange *pPositionChange, CPositionChangeList *pList, CDepositChange *pDepositChange);
};
#endif
