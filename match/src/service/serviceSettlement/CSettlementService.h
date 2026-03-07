#ifndef _CSettlementService_H
#define _CSettlementService_H

#include "errorDefine.h"
#include "DataType.h"
#include "CSequence.h"
#include "FieldData.h"
#include "ServiceInterface.h"
#include "tradingTools.h"
//#include "MarketDataTriggers.h"

class CSettlementService
{
public:
	CSettlementService(CMDB *pMDB, CErrorEngine *pErrorEngine);
	virtual ~CSettlementService();
	//出入金
	virtual bool ReqAccountAction(CAccountActionField *pAccountAction, CAccountField *pOutAccount, CTransaction *pTransaction);

	//virtual bool Loan(CAccountActionField *pAccountAction, CAccountField *pOutAccount, CTransaction *pTransaction);

	//结算--总控,失败的代价太大，所以结算不会回退
	bool Settlement(CWriteableSettleDetail *pSettlementDetail, UF_INT8 nSequenceNo);

	//资金费率清算
	bool FundingRate(CWriteableSettleDetail *pSettlementDetail);
	//结算
	bool Settlement(CWriteableSettleDetail *pSettlementDetail);
	//交割
	bool Delivery(CWriteableSettleDetail *pSettlementDetail);

	bool LoanInterest(CWriteableSettleDetail *pSettlementDetail, double InterestGap);

	void AccountCollect(map<string, double> Map, CAccountIDType AccountID, CShortIndexIDType SettlementGroup, map<string, double> BanlanceMap, CDefineTypeType Source, CLongNameType Remark);
	double Fee(double dFee, CAccountIDType AccountID, CShortIndexIDType SettlementGroup, CCurrencyType Currency, CDefineTypeType Source, CLongNameType Remark);

//	void WriteErrorLog(const char * pName, const char * pLog);

	double AccountInit(CAccount *pAccount);

	bool DataRepair(CWriteableSettleDetail *pSettlementDetail);
	bool DataClear(CWriteableSettleDetail *pSettlementDetail);

	void DataRepairMemberPosition();
	void DataRepairMember();
	void DataRepairOrder();
	void DataRepairTriggerOrder();
	void DataRepairPosition();
	void DataRepairSpotPosition();
	void DataRepairAccount();

	void DataClearMemberPosition();
	void DataClearUserSession();
	void DataClearPosition();
	void DataClearAccount();
	void DataClearUser();

	CErrorEngine *m_pErrorEngine;
private:
	CMDB *m_pMDB;
	bool AccountWithdraw(CWriteableAccount *pAccount, CAccountActionField *pAccountAction);
	string sSnapPath; // Sanp文件的路径
	FILE* m_pErrorFile;
};

#endif
