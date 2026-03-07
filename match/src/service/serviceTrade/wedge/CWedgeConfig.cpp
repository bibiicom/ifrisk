#include "CWedgeConfig.h"
#include "monitorIndex.h"

CWedgeConfig::CWedgeConfig(CMDB *pMDB, CErrorEngine *pErrorEngine):CTradeService(pMDB, pErrorEngine)
{
	CWedgeGroup *pWedgeGroup;
	CWedge *pWedge;

	pWedgeGroup = new CWedgeGroup(this, pMDB, pErrorEngine);

	//加入时间同步规则
	pWedge = new CTimeSyncWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入合约检查规则
	pWedge = new CInstrumentCheckWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入操作对象检查规则
	m_pActionTargetCheckWedge = new CActionTargetCheckWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(m_pActionTargetCheckWedge);

	//加入字段检查规则
	pWedge = new CFieldCheckWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入组合合约检查规则
	//pWedge = new CCombinationInstrumentCheckWedge(this, pMDB, pErrorEngine);
	//pWedgeGroup->add(pWedge);

	//加入组合合约字段检查规则
	//pWedge = new CCombinationFieldCheckWedge(this, pMDB, pErrorEngine);
	//pWedgeGroup->add(pWedge);

	//加入会话检查规则
//	pWedge = new CSessionCheckWedge(this, pMDB, pErrorEngine);
//	pWedgeGroup->add(pWedge);

	//加入合约状态控制规则
	pWedge = new CInstrumentStatusCheckWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入强平单控制规则
	//pWedge = new CForceCloseControlWedge(this, pMDB, pErrorEngine);
	//pWedgeGroup->add(pWedge);

	//加入合约类型检查规则
	//这里有doAfterOrderChange，确保这个规则在CStatusControlWedge之后，要不然会死锁core
	pWedge = new COrderTypeCheckWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入合约类型控制检查规则
	pWedge = new COrderTypeControlWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入自然人开仓检查控制规则
	//	pWedge = new CPersonOpenCheckWedge(this, pMDB, pErrorEngine, BackendControlProperty);
	//	pWedgeGroup->add(pWedge);

	//加入数量检查规则,Price决定的过程中，币币的TurnOver会决定数量，所以需要再决定之后检查
	pWedge = new CVolumeCheckWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//pWedge = new CPositionOrderWedge1(this, pMDB, pErrorEngine);
	//pWedgeGroup->add(pWedge);

	//以上两个会决定数量，而价格里面有需求是依赖余数量的，所以需要在价格之前完成
	//加入价格检查规则
	pWedge = new CPriceCheckWedge(this, pMDB, pErrorEngine);
	//		IPriceLimitAlgorithms *pPriceLimitAlgorithms = new CCombinationPriceLimitAlgorithms(pMDB, new CBasePriceLimitAlgorithms(pMDB));
	//		pWedge = new CPriceCheckWedge(this, pMDB, pErrorEngine, pPriceLimitAlgorithms);
	pWedgeGroup->add(pWedge);

	//pWedge = new CFuseWedge(this, pMDB, pErrorEngine, pPriceCheckWedge, CancelFuseForNoFuse);
	//pWedgeGroup->add(pWedge);

	//加入交易权限控制规则
	pWedge = new CRightCheckWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入用户交易权限控制规则
	pWedge = new CUserRightWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入客户持仓控制规则
	pWedge = new CPositionCheckWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	///简单合约投机净持仓普通投资者的情况
	//pPositionLimitAlgo = new CBasePositionLimitAlgorithms(pMDB);
	//pPositionTypeControler = new CNetPositionTypeControler(pMDB);
	//pWedge = new COptionsPositionCheckWedge(this, pMDB, pErrorEngine, pPositionLimitAlgo, pPositionTypeControler);
	//pWedgeFilter = &(
	//	(*new CTradableOrderWedgeFilter(pMDB)) &
	//	(*new CPositionTypeWedgeFilter(pMDB, PT_Net)) &
	//	((*new CProductClassWedgeFilter(pMDB, PC_Futures)) |
	//	(*new CProductClassWedgeFilter(pMDB, PC_Options))));
	//pPositionWedgeGroup->add(pWedge, pWedgeFilter);

	//pWedge = new CCombinationWedge(this, pMDB, pErrorEngine, pPositionWedgeGroup);
	//pWedgeFilter = &(			(*new CTradableOrderWedgeFilter(pMDB)) &			(*new CProductClassWedgeFilter(pMDB, PC_Combination)));
	//pWedgeGroup->add(pWedge, pWedgeFilter);

	//加入会员持仓控制规则，还能支持期权的部分
	///会员持仓检查部分修改，做市商部分不做修改
	///普通投资者的情况
	///期权
	//pMemberPositionLimitAlgo = new CBaseMemberPositionLimitAlgorithms(pMDB);
	//pWedge = new COptionsMemberPositionCheckWedge(this, pMDB, pErrorEngine, pMemberPositionLimitAlgo);
	//pWedgeFilter = &(*new CProductClassWedgeFilter(pMDB, PC_Options));
	//pWedgeGroup->add(pWedge);
	///期货

	//加入资金控制规则，还能支持期权的部分
	///普通投资者普通合约的情况
	pWedge = new CDepositCheckWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//MemberPosition需要用到正确的AccountID，这里需要放到资金检查之后，才能得到正确的AccountID
	pWedge = new CMemberPositionCheckWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//pWedge = new CPositionOrderWedge(this, pMDB, pErrorEngine);
	//pWedgeGroup->add(pWedge);

	pWedge = new CPositionWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入止损单激活的交易规则
	//int iTriggerOrderMode = 0;// 0：内部运行，1：外部运行
	//char *sTriggerOrderMode = pConfig->getConfig("TriggerOrderMode");
	//if (sTriggerOrderMode[0] != '\0')
	//{
	//	iTriggerOrderMode = atoi(sTriggerOrderMode);// 如果无法转换为int类型的值，则返回0
	//}

	//pWedge = new CTriggerOrderWedge(this, pMDB, pErrorEngine, iTriggerOrderMode);
	pWedge = new CTriggerOrderWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入自动挂起的交易规则
	//	pWedge = new CAutoSuspendWedge(this, pMDB, pErrorEngine);
	//	pWedgeGroup->add(pWedge);

	//加入集合竞价的撮合规则
	ITradeMaker *pTradeMaker;
	pTradeMaker = new CSingleTradeMaker(this, pMDB, pErrorEngine);
	//pTradeMaker = new CCombinationTradeMaker(this, pMDB, pErrorEngine);
	//pWedge = new CAuctionWedge(this, pMDB, pErrorEngine, pTradeMaker);
	//pWedgeGroup->add(pWedge);
	//加入价格优先时间优先的撮合规则
	//pTradeMaker = new CCombinationTradeMaker(this, pMDB, pErrorEngine);
	m_pTradeMaker = pTradeMaker;
	pWedge = new CPriceTimeWedge(this, pMDB, pErrorEngine, pTradeMaker);
	pWedgeGroup->add(pWedge);
	//pWedge = new COnlyQuoteWedge(this, pMDB, pErrorEngine, pTradeMaker);
	//pWedgeGroup->add(pWedge);
	pWedge = new CCFDWedge(this, pMDB, pErrorEngine, pTradeMaker);
	pWedgeGroup->add(pWedge);

		//加入优先权规则
	pWedge = new CPriorityWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入行情生成控制规则
	pWedge = new CMarketDataGenerateWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入状态控制规则
	pWedge = new CStatusControlWedge(this, pMDB, pErrorEngine);
	//pWedge = new CCombinationStatusControlWedge(this, pMDB, pErrorEngine);
	pWedgeGroup->add(pWedge);

	//加入执行规则
	pWedge = new CExecutionWedge(this, pMDB, pErrorEngine);
	//pWedge = new CImplyWedge(this, pMDB, pErrorEngine, m_pOrderSequence, m_pTradeSequence, m_pSortSequence);
	pWedgeGroup->add(pWedge);

	setWedge(pWedgeGroup);
}

CWedgeConfig::~CWedgeConfig(void)
{
}

