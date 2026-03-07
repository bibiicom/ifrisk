#ifndef POSITIONDEPOSITWedgeS_H
#define POSITIONDEPOSITWedgeS_H

#include "CWedge.h"
#include "errorDefine.h"
#include "tradingTools.h"
#include "positionControlers.h"
#include "accountAlgorithms.h"


/////////////////////////////////////////////////////////////////////////
//CAccountProductGroupTrigger是完成账户初始化ProductGroup的触发器
/////////////////////////////////////////////////////////////////////////
class CAccountProductGroupTrigger :public CAccountActionTrigger
{
public:
	///构造方法
	///@param	pMDB	使用的内存数据库
	CAccountProductGroupTrigger(CMDB *pMDB)
	{
		m_pMDB = pMDB;
	}

	///加入前触发
	///@param	pAccount	要加入的Account
	virtual void beforeAdd(CWriteableAccount *pAccount);
private:
	///使用的内存数据库
	CMDB *m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
//CTableCurrencyAccumulationTrigger是Account表发生资金变化时更新Currency表数据的触发器
/////////////////////////////////////////////////////////////////////////
//class CTableCurrencyAccumulationTrigger :public CAccountCommitTrigger
//{
//public:
//	CTableCurrencyAccumulationTrigger(CMDB *pMDB);
//	virtual ~CTableCurrencyAccumulationTrigger(void) {};
//	virtual void commitAdd(CAccount *pAccount);
//	virtual void commitUpdate(CAccount *pAccount, CWriteableAccount *pOldAccount);
//	virtual void commitRemove(CWriteableAccount *pAccount);
//protected:
//	CMDB *m_pMDB;
//};

/////////////////////////////////////////////////////////////////////////
///pd02:CDepositCheckWedge是资金检查的交易规则
/////////////////////////////////////////////////////////////////////////
class CDepositCheckWedge : public CWedge
{
public:
	///构造方法，创建资金检查交易规则类
	///@param	pTradeService	使用的交易服务
	///@param	pMDB	使用的内存数据库
	///@param	pErrorEngine	报告错误的引擎
	///@param	pAlgo	使用的资金算法，由本对象负责释放
	///@param	useCloseProfit	是否计算平仓盈亏
	CDepositCheckWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine);

	///析构方法
	~CDepositCheckWedge(void);

	virtual int getUse(void)
	{
		int result = 0;
		result |= USE_changePosition;
		return result;
	}

	virtual bool changePosition(CPositionChangeList *pChangeList,CTransaction *pTransaction);

	virtual void applyChange(CWriteableAccount *pAccount, CDepositChange *pChange);

	virtual void applyVolumeChange(CWriteableAccount *pAccount, CDepositChange *pChange);

protected:
	///对客户持仓实施变化
	///@param	pPosition	需要变化的客户持仓
	///@param	pDepositChange	资金的变化量
	virtual void applyChange(CWriteablePosition *pPosition, CDepositChange *pDepositChange);

	///是否需要检查资金
	///@param	pChangeList	客户持仓变更列表
	///@return	true表示需要，false表示不需要
	virtual bool needCheckDeposit(CPositionChangeList *pChangeList);

	virtual bool needCheckDeposit(COrder* pOrder);

	///检查并改变资金
	///@param	pChangeList	客户持仓变更列表
	///@param	pDepositChange	资金的变化量
	///@param	needCheckThisDeposit	是否需要检查此资金
	///@param	pTransaction	本操作属于的事务
	///@return	true表示检查合格，false表示检查不合格
	virtual bool changeDeposit(CPositionChangeList *pChangeList, CDepositChange *pDepositChange, CTransaction *pTransaction);
	
	//币币交换的时候改变资金的方法
	virtual bool changeDepositAccountExchange(CPositionChangeList *pChangeList, CDepositChange *pDepositChange, CTransaction *pTransaction);
	//币币交换的时候改变持仓的方法
	virtual void applyChange(CWriteablePosition *pPosition, CDepositChange *pDepositChange, CDirectionType Direction);
	//币币交换的时候改变账户的方法
	virtual void applyChange(CWriteableAccount *pAccount, CDepositChange *pChange, CDirectionType Direction);

	// 手续费抵扣金时，改变手续费的方法
	virtual CMoneyType changeDepositFee(CPositionChangeList *pChangeList, CDepositChange *pDepositChange, CMoneyType TotalTradeFee);
	bool changeDepositFeeEach(CPositionChangeList *pChangeList, CAccount *pAccount, CMoneyType RemainFee, CRatioType FeeDeduceDisCount);

	// 币币交换手续费抵扣金时，改变仓位的方法
	//virtual bool deducePosition(CPositionChangeList *pChangeList, CDepositChange *pDepositChange, CAccount *pAccount, CMoneyType DeduceFee);
	CExchangeDepositAlgorithms* m_pExchangeAlgo;
	CBaseDepositAlgorithms* m_pBaseAlgo;
	CWriteableAccount theAccount;
	CWriteableAccount theOtherAccount;
	CWriteablePosition thePosition;
};


#endif
