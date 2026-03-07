#include "matchWedges.h"
#include "CopyField.h"
#include "monitorIndex.h"
#include "tradingTools.h"
#include "CSequence.h"

CExecutionWedge::CExecutionWedge(CTradeService *pTradeService, CMDB *pMDB, CErrorEngine *pErrorEngine)
	:CWedge(pTradeService, pMDB, pErrorEngine)
{
	m_sName = "Execution";
}

CExecutionWedge::~CExecutionWedge(void)
{
	//if (m_pInstrumentTrigger)
	//{
	//	m_pMDB->m_InstrumentFactory->removeActionTrigger(m_pInstrumentTrigger);
	//	delete m_pInstrumentTrigger;
	//}

	//if (m_pOrderTrigger)
	//{
	//	m_pMDB->m_OrderFactory->removeActionTrigger(m_pOrderTrigger);
	//	delete m_pOrderTrigger;
	//}
}

bool CExecutionWedge::insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
{
	pOrder->VolumeRemain = pOrder->Volume - pOrder->VolumeTraded;

	//pOrder->OrderID = m_TradeService->m_pOrderSequence->getNextValue();

	//	pOrder->UpdateUserID = pOrder->UserID;
	pOrder->TimeSortNo = m_gSortSequence.getNextValueByDouble();
//	pOrder->ImplySortNo = 0.0;
	pOrder->BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();

	// 记录插入时的行情价格
	//CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(pOrder->ExchangeID, pOrder->InstrumentID);
	//if (pMarketData != NULL)
	//{
	//	pOrder->LastPriceByInsert = pMarketData->LastPrice;
	//	pOrder->BidPrice1ByInsert = pMarketData->BidPrice1;
	//	pOrder->AskPrice1ByInsert = pMarketData->AskPrice1;
	//}

	//完成插入报单的操作
	//COrder *pSavedOrder = m_pMDB->m_OrderFactory->add(pOrder, pTransaction);

	if (m_pMDB->m_OrderFactory->add(pOrder, pTransaction) == NULL)
		return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);

	////尝试进行成交
	//if (needTryTrade(pOrder))
	//{
	//	m_TradeService->tryTrade(pSavedOrder,pTransaction);
	//	memcpy(pOrder, pSavedOrder, sizeof(CWriteableOrder));
	//}
	return true;
}

//bool CExecutionWedge::insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction* pTransaction)
//{
//	///设置字段
//	//if (pCombOrder->InsertDate.isNull())
//	//{
//	//	pCombOrder->InsertDate = getMDBDay(m_pMDB);
//	//}
//
//	///设置组合报单号
//	//pCombOrder->CombOrderID = m_TradeService->m_pOrderSequence->getNextValue();
//
//	///尝试进行成交
//	//if (!m_TradeService->tryTrade(pCombOrder,pTransaction))
//	//{
//	//	return false;
//	//}
//
//	pCombOrder->InsertTime = g_nCurrentTime;
//	m_pMDB->m_CombOrderFactory->add(pCombOrder, pTransaction);
//
//	return true;
//}

bool CExecutionWedge::insertSingleTrade(CWriteableTrade *pTrade, CWriteableOrder *pOrder)
{
	///设置成交号
	if (pTrade->TradeID.isNull())
	{
		pTrade->TradeID = m_gTradeSequence.getNextValueByString();
		///这里，只在成交号不存在的时候，寻找成交编号。这是因为需要买卖双方共用一个成交
		///编号，以便监察。在此种逻辑下，插入成交的地方总是连续的双边一起插入的。如果在
		///插入第1个成交时，不设置成交编号，而在插入第2个成交时，沿用原来的成交编号，那
		///就可以实现这两笔成交共用一个号了。
	}

	///设置时间字段

	//	pTrade->TradeDate = getMDBDay(m_pMDB);

		///设置交易员和本地报单号
//	pTrade->UserID = pOrder->UserID;
//	pTrade->OrderLocalID = pOrder->LocalID;

	if (pTrade->AccountID.isNull())
		pTrade->AccountID = pOrder->AccountID;

	pTrade->BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_TradeFactory->add(pTrade);

	return true;
}

bool CExecutionWedge::updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction)
{
	m_pMDB->m_MarketDataFactory->retrieve(pInstrumentStatus, &WritebleMarketData);
	WritebleMarketData.InstrumentStatus = pInstrumentStatus->InstrumentStatus;
	m_pMDB->m_MarketDataFactory->update(pInstrumentStatus, &WritebleMarketData, pTransaction);

	REPORT_EVENT(LOG_CRITICAL, "InstrumentStatusUpdate", "%s:%c", pInstrumentStatus->InstrumentID.getValue(), pInstrumentStatus->InstrumentStatus.getValue());

	return true;
}

bool CTimeSyncWedge::timeSync(CWriteableCurrentTime *pCurrentTime)
{
	setMDBTime(m_pMDB, pCurrentTime);
	return true;
}

bool CInstrumentStatusCheckWedge::insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
{
	CInstrumentStatusType curStatus = pOrder->getInstrument()->getMarketData()->InstrumentStatus;
	if ((curStatus == IS_Continous) || (curStatus == IS_AuctionOrdering) || (curStatus == IS_AuctionBalance))
	{
		return true;
	}

	if (!(pOrder->DeriveSource == DS_Normal || pOrder->DeriveSource == DS_FromQuote || pOrder->DeriveSource == DS_FromOTCTrade_WashOnly))
	{
		if (curStatus == IS_Closed)
			return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS);
		else
			return true;
	}
	return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS);
}

//bool CInstrumentStatusCheckWedge::insertCombOrder(CWriteableCombOrder *pCombOrder, CTransaction* pTransaction)
//{
//	CInstrumentStatusType curStatus;
//#define checkInstrumentStatus(id)											\
//	if (pCombOrder->getInstrument ## id())									\
//	{																		\
//		if (pCombOrder->getInstrument ## id()->getMarketData()->InstrumentStatus!=IS_Continous)		\
//			return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS);	\
//	}																		\
//	else																	\
//	{																		\
//		return true;														\
//	}
//
//	checkInstrumentStatus(1);
//	checkInstrumentStatus(2);
//	checkInstrumentStatus(3);
//	checkInstrumentStatus(4);
//
//	return true;
//}

bool CInstrumentStatusCheckWedge::orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction)
{
	CInstrumentStatusType curStatus = pOrderAction->getOrder()->getInstrument()->getMarketData()->InstrumentStatus;
	if (pOrderAction->ActionFlag == AF_Active)
	{
		if ((curStatus == IS_Continous) || (curStatus == IS_AuctionOrdering) || (curStatus == IS_AuctionBalance))
			return true;
	}
	else
	{
		if ((curStatus == IS_Continous) || (curStatus == IS_AuctionOrdering || curStatus == IS_NoTrading))
			return true;
	}

	return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS);
}

bool CInstrumentStatusCheckWedge::updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction)
{
	CInstrument *pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pInstrumentStatus->ExchangeID, pInstrumentStatus->InstrumentID);

	if (pInstrument == NULL)
		return m_pErrorEngine->reportError(ERROR_INSTRUMENT_NOT_FOUND);

	pInstrumentStatus->linkInstrument(pInstrument);

	CInstrumentStatusType curStatus = pInstrument->getMarketData()->InstrumentStatus;
	CInstrumentStatusType newStatus = pInstrumentStatus->InstrumentStatus;

	///实际上不切换状态，总是允许的	
	if (curStatus == newStatus)
	{
		return true;
	}

	///检查是否合理的状态切换
	switch (curStatus)
	{
	case IS_BeforeTrading:
		if (newStatus == IS_NoTrading)
		{
			return true;
		}
		break;
	case IS_NoTrading:
		if ((newStatus == IS_Continous) || (newStatus == IS_AuctionOrdering) || (newStatus == IS_Closed))
		{
			return true;
		}
		break;
	case IS_Continous:
		if (newStatus == IS_NoTrading)
		{
			return true;
		}
		break;
	case IS_AuctionOrdering:
		if ((newStatus == IS_AuctionBalance) || (newStatus == IS_AuctionMatch))
		{
			return true;
		}
		break;
	case IS_AuctionBalance:
		if (newStatus == IS_AuctionMatch)
		{
			return true;
		}
		break;
	case IS_AuctionMatch:
		if (newStatus == IS_NoTrading)
		{
			return true;
		}
		break;
	default:
		break;
	}

	return m_pErrorEngine->reportError(ERROR_INVALID_INSTRUMENT_STATUS_SHIFT);
}

bool CInstrumentStatusCheckWedge::timeSync(CWriteableCurrentTime *pCurrentTime)
{
	//tom 20190911 建议手动切换，自动切换暂时不做了
	return true;

	//	CInstrument *pInstrument;
	//	CTradingSegment *pTradingSegment;

		///获取原来的时间
		///给m_OldTime赋初始化值，不使用原来每次都从内存表中获取时间的方法，提高效率
	//if (m_OldTime.isNull())
	//{
	//	m_OldTime = getMDBOldTime(m_pMDB);
	//}
	/////临时变量，用在下面的判断逻辑中
	//CTimeType oldTime = m_OldTime;

	/////先检查是否可以在当前时间进行合约状态，此举纯属优化措施，过滤毫秒部分
	//if (pCurrentTime->CurrTime == oldTime)
	//{
	//	return true;
	//}
	/////更新m_OldTime为当前的时间（不含毫秒信息）
	//m_OldTime = pCurrentTime->CurrTime;

	/////取当前交易日，结算组未完成时，内存表中的交易日CurrTradingDay默认值为"19000101"，
	/////此时，应该取交易日，直到所取到的交易日为结算组初始化完成后的正常交易日为止。
	//if (m_iCurrTradingDay == m_iInitialDay)
	//{
	//	m_iCurrTradingDay = CDate::DateToLong(getMDBDay(m_pMDB).getValue());
	//}

	///根据交易日和实际自然日计算交易大段编号，计算公式为：交易大段编号 = 交易日 - 自然日
	///交易大段编号为两位数
	///自然日来自排队机时钟包内的CurrDate提供, 历史数据反演时，CurrDate字段均为空值，
	///当判断CurrDate为空时，就取交易日作为当日的自然日，交易大段默认为0. 
	/// nCurrPrimeTradingSegSN是交易日减去排队机日期
	//int nCurrPrimeTradingSegSN = 0;
	//if (!pCurrentTime->CurrDate.isNull())
	//{
	//	///根据交易日和实际自然日计算交易大段编号，计算公式为：交易大段编号 = 交易日 - 自然日
	//	nCurrPrimeTradingSegSN = m_iCurrTradingDay - CDate::DateToLong(pCurrentTime->CurrDate.getValue());
	//}

	///大段号小于0，有两种场景：
	/// a)自然日在交易日后，此时要切换这是不符合业务规则的，故直接拒绝继续执行切换逻辑；
	/// b)在结算组数据还没有初始化完成的时候，交易日默认为19000101,自然日默认为19700101，故此时大段号
	///   计算出来的结果为一个负数，故此时也不执行切换逻辑。
	///	如果出现上述情况，不做切换，此时有一种情况要注意：
	/// 如果在交易收盘之前系统暂停，如果在当天收盘之后启动，是要切换的，但是如果过了一个自然天启动，就不会做切换到收盘的操作。
	//if (nCurrPrimeTradingSegSN < 0)
	//{
	//	//日志输出
	//	REPORT_EVENT(LOG_INFO, "TradingSegment", "nCurrPrimeTradingSegSN is %d,is below 0 , No change", nCurrPrimeTradingSegSN);
	//	return true;
	//}

	///REPORT_EVENT(LOG_CRITICAL,"TradingSegment test","currsegment:%d,oldtime:%s,currtime:%s",nCurrPrimeTradingSegSN,oldTime.getValue(),pCurrentTime->CurrTime.getValue());

	///夜盘系统开始后，合约的交易阶段不一样了，所以必须按照每个合约进行切换
	// tom 20150116 修改为按照合约切换
// 	CInstrumentStatus *pInstrumentStatus;
// 	CInstrumentStatusIteratorByInstrumentIDAll iteratorinstrument(m_pMDB->m_InstrumentStatusFactory);
// 	while ((pInstrumentStatus = iteratorinstrument.next())!=NULL)
// 	{
// 		///找到在从老时间点到新时间点需要进行状态切换的合约，逐一切换
// 		///tom 这里注意，原来的逻辑是老时间之后的所有阶段全部切换，但是有夜盘之后，就不行了，时间小的但是阶段在前的不在之中，导致无法切换
// 		///所以，这里这里需要按照合约进行选择
// 		///CTradingSegmentIteratorByTime iterator(m_pMDB->m_TradingSegmentFactory, oldTime);
// 		///这里的索引合约号相同，阶段号大于参数阶段号的全部取出来
// 		CTradingSegmentIteratorByInstrumentAndTradingSegementSN iterator(m_pMDB->m_TradingSegmentFactory,pInstrumentStatus->ExchangeID,pInstrumentStatus->InstrumentID,pInstrumentStatus->TradingSegmentNo.getValue());
// 		///tom 第一个阶段号应该是合约的最后状态，此状态不需要切换
// 		///如果后面没有状态，说明合约到了最后状态，下一个合约切换
// 		if(iterator.next()==NULL)
// 		{
// 			continue;
// 		}
// 		while((pTradingSegment = iterator.next())!=NULL)
// 		{
// 			///REPORT_EVENT(LOG_CRITICAL,"TradingSegment begin","seg:%d,starttime:%s ",pTradingSegment->TradingSegmentNo.getValue(),pTradingSegment->StartTime.getValue());
// 			///根据合约状态切换中TradingSegmentNo的最高位来计算其所属的交易大段，该值目前定义为4位整数，最高位为交易大段，xiechun 20140909
// 			///nTmpPrimeTradingSegSN是从表里面找出来的大段号
// 			int nTmpPrimeTradingSegSN = 0;
// 			if(pTradingSegment->TradingSegmentNo.getValue() >= 100)
// 			{
// 				nTmpPrimeTradingSegSN = (pTradingSegment->TradingSegmentNo.getValue())%100;
// 			}
// 			///REPORT_EVENT(LOG_CRITICAL,"last segement","instrument:%s,lastsegement:%d ",pInstrumentStatus->InstrumentID.getValue(),pInstrumentStatus->TradingSegmentNo.getValue());
// 			///nLastPrimeTrdSegSN是此合约上次的大段号
// 			///int nLastPrimeTrdSegSN = 0;
// 			///if (pInstrumentStatus->TradingSegmentNo.getValue() >= 100)
// 			///{
// 			///		nLastPrimeTrdSegSN = (pInstrumentStatus->TradingSegmentNo.getValue())%100;
// 			///}
// 
// 			///检查合约状态切换中的交易大段是否与当前实际的交易大段相符，
// 			///如果相同看时间是否满足条件，只切换旧时间到目前时间阶段号
// 			///如果切换的交易阶段编号在上次阶段编号之外，也不做切换
// 			
// 			///从索引来说，nTmpPrimeTradingSegSN是在nLastPrimeTrdSegSN之后
// 			///这里无需判断nLastPrimeTrdSegSN小于nTmpPrimeTradingSegSN
// 			if(nTmpPrimeTradingSegSN < nCurrPrimeTradingSegSN)
// 			{
// 				//如果大段很小，后续无需切换
// 				break;
// 			}
// 
// 			///tom 20141219 在大阶段号小于不需要切换，等于时如果时间大于也不需要切换
// 			if ((nTmpPrimeTradingSegSN == nCurrPrimeTradingSegSN))
// 			{
// 				/// nTmpPrimeTradingSegSN和nCurrPrimeTradingSegSN相同的情况下，说明此时是否切换就看时间的大小
// 				///已经超出了本次的时间，由于是按照时间对交易阶段进行升序排列，				
// 				///只处理在oldtime和currtime之间的状态
// 				///如果大于现在的时间，下面的就可以不用切换了
// 				if (pTradingSegment->StartTime > pCurrentTime->CurrTime)
// 				{
// 					break;
// 				}
// 				///现在是按照交易阶段排序进行切换，此时上一个交易阶段的时间和上一个交易阶段的时间
// 				///这里就不和oldtime进行比较了，明显的例子就是如果隔天重启交易系统，前天的停止时间18:00:00,切换时间是2:00:00
// 				///这个逻辑的判断下就不会切换,修改之后就会有问题，比如同一个大阶段号下，阶段号大的时间比阶段号小的时间小，此时
// 				///系统对大阶段号依然会切换，老系统的逻辑是不切换
// 				///如果比老时间要早，说明已经切换过了，不用切换
// 				//if ((pTradingSegment->StartTime <= oldTime))
// 				//{
// 				//	continue;
// 				//}
// 			}
// 
// 			pInstrument=m_pMDB->m_InstrumentFactory->findByInstrumentID(pTradingSegment->ExchangeID,pTradingSegment->InstrumentID);
// 			if (pInstrument==NULL)
// 			{
// 				///合约找不到，忽略它
// 				continue;
// 			}
// 
// 			if ((pInstrument->getInstrumentStatus()!=NULL)&&(pInstrument->getInstrumentStatus()->IsManual))
// 			{
// 				///如果该合约在手动切换的情况，则不予处理
// 				continue;
// 			}
// 
// 			if (!pInstrument->IsTrading)
// 			{
// 				///如果该合约属于不交易合约，则不予处理
// 				continue;
// 			}
// 			///REPORT_EVENT(LOG_CRITICAL,"TradingSegment change","each:%d ",pTradingSegment->TradingSegmentNo.getValue());
// 			///否则进行状态切换
// 			CWriteableInstrumentStatus theInstrumentStatus;
// 			theInstrumentStatus.init();
// 			theInstrumentStatus.InstrumentID=pTradingSegment->InstrumentID;
// 			theInstrumentStatus.InstrumentStatus=pTradingSegment->InstrumentStatus;
// 			theInstrumentStatus.TradingSegmentNo=pTradingSegment->TradingSegmentNo;
// 			theInstrumentStatus.EnterTime=pCurrentTime->CurrTime;
// 			theInstrumentStatus.EnterDate=pCurrentTime->CurrDate;
// 			if (pInstrument->getInstrumentStatus()!=NULL)
// 			{
// 				theInstrumentStatus.IsManual=pInstrument->getInstrumentStatus()->IsManual;
// 			}
// 			else
// 			{
// 				theInstrumentStatus.IsManual=false;
// 			}
// 
// 			///这里使用回滚保存点的实现方式，因为某个合约的状态切换不成功，应当不影响其他合约的
// 			///状态切换。
// 			CTransactionSavePoint *pBeforeUpdateInstrumentStatus=CTransactionSavePoint::alloc(pTransaction);
// 			if (!m_TradeService->updateInstrumentStatus(&theInstrumentStatus,pSession,pTransaction))
// 			{
// 				internalWarning("自动合约状态切换失败");
// 				pTransaction->rollbackToSavePoint(pBeforeUpdateInstrumentStatus);
// 			}
// 		}
// 	}

	m_pErrorEngine->getLastError();

	return true;
}

bool CPriorityWedge::insertOrder(CWriteableOrder *pOrder, CTransaction* pTransaction)
{
	setPriority(pOrder);

	return true;
}

bool CPriorityWedge::orderAction(CWriteableOrderAction *pOrderAction, CTransaction *pTransaction)
{
	if (pOrderAction->ActionFlag == AF_Active)
	{
		COrder *pOrder;
		pOrder = pOrderAction->getOrder();
		m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);

		///在激活报单时，需要重新设置优先级
		setPriority(&theOrder);

		m_pMDB->m_OrderFactory->update(pOrder, &theOrder, pTransaction);
	}
	return true;
}

void CPriorityWedge::setPriority(CWriteableOrder *pOrder)
{
	//if (pOrder->Priority.isNull())
	//{
	//	pOrder->Priority = pOrder->getMember()->Priority.getInt();
	//}		
	//else
	//{
	//	if(pOrder->Priority > pOrder->getMember()->Priority.getInt())
	//		pOrder->Priority = pOrder->getMember()->Priority.getInt();
	//}

	//pOrder->Priority = pOrder->getMember()->Priority;
	//if (pOrder->Priority.isNull())
	//	pOrder->Priority = 100;
	return;

	///只考虑停板价平仓优先
	if (pOrder->OffsetFlag != OF_Open)
	{
		///只需要考虑静态价格绑定的情况
		if (pOrder->Direction == D_Buy)
		{
			CReadOnlyPriceType &upperLimitPrice = pOrder->getInstrument()->getMarketData()->UpperLimitPrice;
			if (!upperLimitPrice.isNull())
			{
				if (pOrder->Price == pOrder->getInstrument()->getMarketData()->UpperLimitPrice)
				{
					///买单等于涨停板价，优先级为1
					pOrder->Priority = 1;
					return;
				}
			}
		}
		else
		{
			CReadOnlyPriceType &lowerLimitPrice = pOrder->getInstrument()->getMarketData()->LowerLimitPrice;
			if (!lowerLimitPrice.isNull())
			{
				if (pOrder->Price == pOrder->getInstrument()->getMarketData()->LowerLimitPrice)
				{
					///卖单等于跌停板价，优先级为1
					pOrder->Priority = 1;
					return;
				}
			}
		}
	}

	///其他情况的优先级为0
	pOrder->Priority = 0;
}


void CPriceTimeWedge::tryTradeAfterOrderInsert(COrder *pOrder)
{
	if (pOrder->getInstrument()->TradingModel != TM_PriceTime)
		return;

	if (pOrder->OrderType == OT_FlashTrade)
		return;

	COrderIterator	*pCountPartyIterator;
	bool hasTrade = false;

	///只在连续交易状态尝试成交
	if (pOrder->getInstrument()->getMarketData()->InstrumentStatus != IS_Continous)
		return;

	m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);

	///按照价格优先时间优先的规则排队
	if (pOrder->Direction == D_Buy)
		pCountPartyIterator = m_TradeService->getOrderBookList()->getOrderIterator(pOrder->getInstrument(), D_Sell);
	else
		pCountPartyIterator = m_TradeService->getOrderBookList()->getOrderIterator(pOrder->getInstrument(), D_Buy);

	while (theOrder.VolumeRemain > 0.0)
	{
		COrder *pCountParty = pCountPartyIterator->next();
		if (pCountParty == NULL)
		{
			///找不到交易对手了
			break;
		}

		///判断价格是否符合
		if (theOrder.OrderPriceType != OPT_AnyPrice)
		{
			if (theOrder.Direction == D_Buy)
			{
				if (theOrder.Price < pCountParty->Price)
					break;
			}
			else
			{
				if (pCountParty->Price < theOrder.Price)
					break;
			}
		}

		hasTrade = true;
		m_pMDB->m_OrderFactory->retrieve(pCountParty, &theCountParty);

		///确认成交价和成交量
		//CPriceSourceType priceSource;
		//CPriceType tradePrice = getTradePrice(pOrder, pCountParty, &priceSource);
		CPriceType tradePrice = pCountParty->Price;
		CVolumeType tradeVolume = MIN(theOrder.VolumeRemain, theCountParty.VolumeRemain);

		///产生成交
		if (pOrder->Direction == D_Buy)
			m_TradeMaker->genTrade(&theOrder, &theCountParty, tradePrice, tradeVolume, MR_Taker);
		else
			m_TradeMaker->genTrade(&theCountParty, &theOrder, tradePrice, tradeVolume, MR_Maker);

		///修改对手方的TID
		theCountParty.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		///更新对手方报单
		//pCountPartyIterator->UpdateCurrent(&theCountParty, pTransaction, true);
		pCountPartyIterator->UpdateCurrent(&theCountParty);
	}
	pCountPartyIterator->free();	///删除叠代器

	///更新本方报单
	if (hasTrade)
	{
		theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_OrderFactory->update(pOrder, &theOrder, NULL, false);
	}
}

///说明一个分支的信息
class CLegInfo
{
public:
	///该分支的合约
	CInstrument *m_pInstrument;

	///该分支的乘数
	CRatioType m_legMultiple;

	///该分支的买卖方向
	CDirectionType m_direction;

	///该分支的开平标志
	COffsetFlagType m_offsetFlag;

private:
	///该分支的分价枚举器
	CMarketOrderIterator *m_pMarketOrderIterator;

	///该分支的当前正在分价
	CMarketOrder *m_pMarketOrder;

	///该分支的分价中的剩余量
	CVolumeType m_remainVolume;
public:
	///创建一个分支信息
	///@param	pMDB	使用的内存数据库
	///@param	pInstrument	指定的合约
	///@param	legMultiple	分支乘数
	///@param	direction	分支买卖方向
	CLegInfo(CMDB *pMDB, CInstrument *pInstrument, CRatioType legMultiple, CDirectionType direction, COffsetFlagType offsetFlag)
	{
		m_pInstrument = pInstrument;
		m_legMultiple = legMultiple;
		m_direction = direction;
		m_offsetFlag = offsetFlag;
		if (direction == D_Buy)
		{
			m_pMarketOrderIterator = new CMarketOrderIteratorBySellMBLData(pMDB->m_MarketOrderFactory, pInstrument->ExchangeID, pInstrument->InstrumentID);
		}
		else
		{
			m_pMarketOrderIterator = new CMarketOrderIteratorByBuyMBLData(pMDB->m_MarketOrderFactory, pInstrument->ExchangeID, pInstrument->InstrumentID);
		}
		m_pMarketOrder = m_pMarketOrderIterator->next();
		if (m_pMarketOrder != NULL)
		{
			m_remainVolume = m_pMarketOrder->TrueVolume;
		}
		else
		{
			m_remainVolume = 0;
		}
	}

	~CLegInfo(void)
	{
		delete m_pMarketOrderIterator;
	}

	///按照该分支的定义获取下一个价格和数量
	///@param	price	返回下一个价格
	///@param	volume	返回下一个数量
	///@return	true表示存在下一个价格数量，false表示不存在
	bool getNextPriceVolume(CPriceType &price, CVolumeType &volume)
	{
		if (m_remainVolume >= m_legMultiple)
		{
			///当前价位上就有足够的量，那就返回当前价位的即可
			price = m_pMarketOrder->Price*m_legMultiple;
			if (m_direction == D_Sell)
			{
				///本分支是卖出的，那就用负价格
				price = -price;
			}

			///最大可以产生的数量
			volume = m_remainVolume / m_legMultiple;

			///隐含地自动用掉一个单位的数量，这是为了和下面的一种情况统一
			m_remainVolume -= m_legMultiple;

			return true;
		}
		///当前价位上的数量不够，那就从后面的价格中找
		CVolumeType legRemainVolume;
		CVolumeType thisLevelVolume;
		legRemainVolume = m_legMultiple;
		price.clear();
		while (legRemainVolume > 0)
		{
			if (m_remainVolume == 0)
			{
				///本价位的没有了，那就再找一个价位
				m_pMarketOrder = m_pMarketOrderIterator->next();
				if (m_pMarketOrder != NULL)
				{
					///下一个价位有
					m_remainVolume = m_pMarketOrder->TrueVolume;
				}
				else
				{
					///下一个价位没有，那就失败了
					m_remainVolume = 0;
					return false;
				}
			}
			if (m_remainVolume >= legRemainVolume)
			{
				///本价位的数量就够了，就凑满本分支即可
				thisLevelVolume = legRemainVolume;
			}
			else
			{
				///本价位的数量不够，就用光本价位的数量
				thisLevelVolume = m_remainVolume;
			}
			///调整总价格
			price += thisLevelVolume * m_pMarketOrder->Price;
			///调整剩余数量
			m_remainVolume -= thisLevelVolume;
			legRemainVolume -= thisLevelVolume;
		}
		if (m_direction == D_Sell)
		{
			///本分支是卖出的，那就用负价格
			price = -price;
		}

		///最大可以产生的数量为1，因为下一个的价格不同
		volume = 1;

		///上述操作实际上已经把这个单位的数量用掉了，保持了一致
		return true;
	}

	///用掉一定的价格和数量
	///@param	volume	要用掉的数量
	void useVolume(CVolumeType volume)
	{
		///用掉数量为1，那就不用做了，隐含已经做掉了
		m_remainVolume -= m_legMultiple * (volume.getValue() - 1);
	}
};

//bool CPriceTimeWedge::tryTradeAfterCombOrderInsert(CWriteableCombOrder *pCombOrder, CTransaction *pTransaction)
//{
//	///对于各个分支，准备相关的数据
//	int legCount = 0;
//	CLegInfo *legInfos[4];
//
//#define makeLegInfo(id)																\
//	if (pCombOrder->getInstrument ## id())											\
//	{																				\
//		legInfos[legCount++]=new CLegInfo(m_pMDB,pCombOrder->getInstrument ## id(),	\
//			pCombOrder->LegMultiple ## id, pCombOrder->Direction ## id,				\
//			pCombOrder->OffsetFlag ## id);				\
//	}
//
//	makeLegInfo(1);
//	makeLegInfo(2);
//	makeLegInfo(3);
//	makeLegInfo(4);
//
//	///计算可以交易的总成交量
//	CVolumeType totalVolume = 0;
//	CVolumeType thisVolume;
//	CPriceType thisPrice;
//	CVolumeType legVolume;
//	CPriceType legPrice;
//	int i;
//
//	for (;;)
//	{
//		thisVolume = 10000;	///一个很大的，不可能产生成交的数量
//		thisPrice.clear();
//		for (i = 0; i < legCount; i++)
//		{
//			if (legInfos[i]->getNextPriceVolume(legPrice, legVolume))
//			{
//				///本分支可以产生价格和数量，那就加入
//				thisPrice += legPrice;
//				if (legVolume < thisVolume)
//				{
//					thisVolume = legVolume;
//				}
//			}
//			else
//			{
//				///本分支已经不能产生价格和数量了，那就不再做了
//				thisVolume = 0;
//				break;
//			}
//		}
//		if (thisVolume == 0)
//		{
//			///不能产生下一个数量，那就结束了
//			break;
//		}
//
//		///产生了下一个数量，那就检查价格是否合理
//		if (pCombOrder->Direction1 == D_Buy)
//		{
//			///是买入单，那报单价必须大于等于成交价
//			if (pCombOrder->Price < thisPrice)
//			{
//				///小于，那就结束了
//				break;
//			}
//		}
//		else
//		{
//			///是卖出单，那报单价必须小于等于成交价
//			if (pCombOrder->Price > -thisPrice)
//			{
//				///大于，那就结束了
//				break;
//			}
//		}
//
//		///看来这些数量是可以成交的了
//		totalVolume += thisVolume;
//		if (totalVolume >= pCombOrder->Volume)
//		{
//			///看来可以全部成交了，那就成交这些
//			totalVolume = pCombOrder->Volume;
//			break;
//		}
//
//		///标示用掉的量
//		for (i = 0; i < legCount; i++)
//		{
//			legInfos[i]->useVolume(thisVolume);
//		}
//	}
//
//	///产生所有的成交
//	for (i = 0; i < legCount; i++)
//	{
//		///找到本分支对手方的报单
//		COrderIterator	*pOrderIterator;
//		if (legInfos[i]->m_direction == D_Buy)
//			pOrderIterator = m_TradeService->getOrderBookList()->getOrderIterator(legInfos[i]->m_pInstrument, D_Sell);
//		else
//			pOrderIterator = m_TradeService->getOrderBookList()->getOrderIterator(legInfos[i]->m_pInstrument, D_Buy);
//
//		CVolumeType remainVolume = totalVolume * legInfos[i]->m_legMultiple;
//		CVolumeType thisVolume;
//		COrder *pOrder;
//		CWriteableOrder theOrder;
//
//		while (remainVolume > 0)
//		{
//			pOrder = pOrderIterator->next();
//			if (pOrder == NULL)
//			{
//				///怎么可能找不到报单呢？
//				REPORT_EVENT(LOG_CRITICAL, "InternalError", "can not find order");
//				break;
//			}
//			m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);
//
//			///本报单的成交量是剩余成交量和对手方报单量的较小值
//			if (remainVolume < pOrder->VolumeRemain)
//			{
//				thisVolume = remainVolume;
//			}
//			else
//			{
//				thisVolume = pOrder->VolumeRemain;
//			}
//
//			///产生成交
//			if (!m_TradeMaker->genTrade(&theOrder, pCombOrder, thisVolume, legInfos[i]->m_offsetFlag, MR_Maker, pTransaction))
//			{
//				///不能产生成交，那就整体失败
//				///清理环境
//				for (i = 0; i < legCount; i++)
//				{
//					delete legInfos[i];
//				}
//				pOrderIterator->free();
//				return false;
//			}
//
//			///修改对手方报单
//			pOrderIterator->UpdateCurrent(&theOrder, pTransaction, true);
//
//			///调整剩余数量
//			remainVolume -= thisVolume;
//		}
//
//		pOrderIterator->free();
//	}
//
//	///设置组合报单中的数据
//	pCombOrder->VolumeTraded = totalVolume;
//	pCombOrder->VolumeRemain = pCombOrder->Volume - totalVolume;
//
//	///清理环境
//	for (i = 0; i < legCount; i++)
//	{
//		delete legInfos[i];
//	}
//
//	return true;
//}

///用于计算集合竞价时的项
typedef struct
{
	///买入量
	CVolumeType buyVolume;
	///卖出量
	CVolumeType sellVolume;
	///累计买入量
	CVolumeType accumulateBuyVolume;
	///累计卖出量
	CVolumeType accumulateSellVolume;
	///成交量
	CVolumeType tradeVolume;
	///剩余量
	CVolumeType remainVolume;
}	TMBLItem;

bool CAuctionWedge::updateInstrumentStatus(CWriteableMarketData *pInstrumentStatus, CTransaction *pTransaction)
{
	///只有进入竞价撮合状态，才进行撮合工作
	if (pInstrumentStatus->InstrumentStatus != IS_AuctionMatch)
		return true;

	///获取价格的两边
	CMarketOrderIteratorByBuyMBLData buyMBLIterator(m_pMDB->m_MarketOrderFactory, pInstrumentStatus->ExchangeID, pInstrumentStatus->InstrumentID);
	CMarketOrderIteratorBySellMBLData sellMBLIterator(m_pMDB->m_MarketOrderFactory, pInstrumentStatus->ExchangeID, pInstrumentStatus->InstrumentID);
	CMarketOrder *pBuyMarketOrder, *pSellMarketOrder;
	CPriceType highestBidPrice, lowestAskPrice;

	pBuyMarketOrder = buyMBLIterator.next();
	pSellMarketOrder = sellMBLIterator.next();

	if (pBuyMarketOrder == NULL)
		return true;///如果没有买入单，那就不用做了
	else
		highestBidPrice = pBuyMarketOrder->Price;

	if (pSellMarketOrder == NULL)
		return true;///如果没有卖出单，那就不用做了
	else
		lowestAskPrice = pSellMarketOrder->Price;

	if (highestBidPrice < lowestAskPrice)
		return true;///买价和卖价不相交，那就不用做了

	CPriceType tick;
	tick = pInstrumentStatus->getInstrument()->PriceTick;

	time_t lowTick, highTick;
	size_t size, i;

	lowTick = lowestAskPrice.getTicks(tick);
	highTick = highestBidPrice.getTicks(tick);
	size = (size_t)(highTick - lowTick) + 1;

	TMBLItem *pMBLList = new TMBLItem[size];

	for (i = 0; i < size; i++)
	{
		pMBLList[i].buyVolume = 0;
		pMBLList[i].sellVolume = 0;
	}

	while (pBuyMarketOrder != NULL)
	{
		if (pBuyMarketOrder->Price < lowestAskPrice)
		{
			///超出范围了，那就不用考虑了
			break;
		}
		pMBLList[(pBuyMarketOrder->Price.getTicks(tick)) - lowTick].buyVolume = pBuyMarketOrder->TrueVolume;
		pBuyMarketOrder = buyMBLIterator.next();
	}

	while (pSellMarketOrder != NULL)
	{
		if (pSellMarketOrder->Price > highestBidPrice)
		{
			///超出范围了，那就不用考虑了
			break;
		}
		pMBLList[(pSellMarketOrder->Price.getTicks(tick)) - lowTick].sellVolume = pSellMarketOrder->TrueVolume;
		pSellMarketOrder = sellMBLIterator.next();
	}

	///计算累计的买入和卖出量

	pMBLList[size - 1].accumulateBuyVolume = pMBLList[size - 1].buyVolume;
	for (i = size - 2; i >= 0; i--)
	{
		pMBLList[i].accumulateBuyVolume = pMBLList[i + 1].accumulateBuyVolume + pMBLList[i].buyVolume;
	}
	pMBLList[0].accumulateSellVolume = pMBLList[0].sellVolume;
	for (i = 1; i < size; i++)
	{
		pMBLList[i].accumulateSellVolume = pMBLList[i - 1].accumulateSellVolume + pMBLList[i].sellVolume;
	}

	///计算成交量和剩余量
	for (i = 0; i < size; i++)
	{
		pMBLList[i].tradeVolume = MIN(pMBLList[i].accumulateBuyVolume, pMBLList[i].accumulateSellVolume);
		pMBLList[i].remainVolume = abs(pMBLList[i].accumulateBuyVolume - pMBLList[i].accumulateSellVolume);
	}

	///计算最大成交量
	CPriceType tradePrice;
	CVolumeType maxTradeVolume = 0;
	size_t fromTick = 0, toTick = size - 1, tradePriceTick;

	for (i = 0; i < size; i++)
	{
		if (pMBLList[i].tradeVolume > maxTradeVolume)
		{
			maxTradeVolume = pMBLList[i].tradeVolume;
			fromTick = toTick = i;
		}
		else if (pMBLList[i].tradeVolume == maxTradeVolume)
		{
			toTick = i;
		}
		///注意，这里实际上可以优化，可以加上下面的代码
		///else
		///{
		///		break;
		///}
	}

	if (fromTick == toTick)
	{
		///有唯一的最大成交量，那就找到成交价了
		tradePriceTick = fromTick;
		tradePrice = (tradePriceTick + lowTick)*tick;
	}
	else
	{
		///按照最小剩余量计算
		CVolumeType	minRemainVolume;
		size_t remainFromTick, remainToTick;

		minRemainVolume = pMBLList[fromTick].remainVolume;
		remainFromTick = remainToTick = fromTick;

		for (i = fromTick + 1; i <= toTick; i++)
		{
			if (pMBLList[i].remainVolume < minRemainVolume)
			{
				minRemainVolume = pMBLList[i].remainVolume;
				remainFromTick = remainToTick = i;
			}
			else if (pMBLList[i].remainVolume == minRemainVolume)
			{
				remainToTick = i;
			}
			///注意，这里实际上可以优化，可以加上下面的代码
			///else
			///{
			///		break;
			///}
		}
		if (remainFromTick == remainToTick)
		{
			///有唯一的最小剩余量，那也找到成交价了
			tradePriceTick = remainFromTick;
			tradePrice = (tradePriceTick + lowTick)*tick;
		}
		else
		{
			///当全部价位上买量大时，取最高价位
			if (pMBLList[remainToTick].accumulateBuyVolume > pMBLList[remainToTick].accumulateSellVolume)
			{
				tradePriceTick = remainToTick;
				tradePrice = (tradePriceTick + lowTick)*tick;
			}
			///当全部价位卖量大时，取最低价位
			else if (pMBLList[remainFromTick].accumulateBuyVolume < pMBLList[remainFromTick].accumulateSellVolume)
			{
				tradePriceTick = remainFromTick;
				tradePrice = (tradePriceTick + lowTick)*tick;
			}
			else
			{
				///看来只能使用最接近上一成交价原则
				if (minRemainVolume != 0)
				{
					///当不是全部价位上买量卖量相等时，从最高的大买量价位到最低的大卖量价位求中间价
					while (pMBLList[remainFromTick].accumulateBuyVolume > pMBLList[remainFromTick].accumulateSellVolume)
					{
						remainFromTick++;
					}
					remainFromTick--;
					remainToTick = remainFromTick + 1;
				}
				CPriceType lastPrice, lowPrice, highPrice;
				lowPrice = (remainFromTick + lowTick)*tick;
				highPrice = (remainToTick + lowTick)*tick;
				lastPrice = pInstrumentStatus->LastPrice;
				if (lastPrice.isNull())
				{
					///今天还没有成交，则采用昨收盘
					lastPrice = pInstrumentStatus->PreClosePrice;
				}
				if (lastPrice > highPrice)
				{
					tradePrice = highPrice;
				}
				else if (lastPrice < lowPrice)
				{
					tradePrice = lowPrice;
				}
				else
				{
					tradePrice = lastPrice;
				}
			}
		}
	}

	delete pMBLList;

	///产生所有的成交
	COrder *pBuyOrder, *pSellOrder;
	CVolumeType remainVolume;
	int genCount = 0;

	COrderIterator* pBuyOrderIterator = m_TradeService->getOrderBookList()->getOrderIterator(pInstrumentStatus->getInstrument(), D_Buy);
	COrderIterator* pSellOrderIterator = m_TradeService->getOrderBookList()->getOrderIterator(pInstrumentStatus->getInstrument(), D_Sell);
	//COrderIteratorInBuyOrderBook buyOrderIterator(m_pMDB->m_OrderFactory,pInstrumentStatus->InstrumentID);
	//COrderIteratorInSellOrderBook sellOrderIterator(m_pMDB->m_OrderFactory,pInstrumentStatus->InstrumentID);

	pBuyOrder = pBuyOrderIterator->next();
	if (pBuyOrder == NULL)
	{
		REPORT_EVENT(LOG_CRITICAL, "InternalError", "can not find buy order during action");
		return true;
	}
	m_pMDB->m_OrderFactory->retrieve(pBuyOrder, &theBuyOrder);

	pSellOrder = pSellOrderIterator->next();
	if (pSellOrder == NULL)
	{
		REPORT_EVENT(LOG_CRITICAL, "InternalError", "can not find sell order during action");
		return true;
	}
	m_pMDB->m_OrderFactory->retrieve(pSellOrder, &theSellOrder);

	remainVolume = maxTradeVolume;

	while (remainVolume > 0)
	{
		if (theBuyOrder.VolumeRemain == 0)
		{
			pBuyOrderIterator->UpdateCurrent(&theBuyOrder, pTransaction, true);
			pBuyOrder = pBuyOrderIterator->next();
			if (pBuyOrder == NULL)
			{
				REPORT_EVENT(LOG_CRITICAL, "InternalError", "can not find buy order during action");
				break;
			}
			m_pMDB->m_OrderFactory->retrieve(pBuyOrder, &theBuyOrder);
		}
		if (theSellOrder.VolumeRemain == 0)
		{
			pSellOrderIterator->UpdateCurrent(&theSellOrder, pTransaction, true);
			pSellOrder = pSellOrderIterator->next();
			if (pSellOrder == NULL)
			{
				REPORT_EVENT(LOG_CRITICAL, "InternalError", "can not find sell order during action");
				break;
			}
			m_pMDB->m_OrderFactory->retrieve(pSellOrder, &theSellOrder);
		}

		CVolumeType tradeVolume = MIN(theBuyOrder.VolumeRemain, theSellOrder.VolumeRemain);

		///计算剩余成交量
		remainVolume -= tradeVolume;

		///产生成交
		m_TradeMaker->genTrade(&theBuyOrder, &theSellOrder, tradePrice, tradeVolume, MR_None);

		///这里的代码是为了缩小事务的大小，以提高系统的效率
		genCount++;
		if (genCount >= 5)
		{
			genCount = 0;
			pTransaction->commit();
		}
	}
	//解决集合竞价撮合行情发送错误的缺陷
	if (pBuyOrder != NULL)
	{
		theBuyOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_OrderFactory->update(pBuyOrder, &theBuyOrder, pTransaction);
	}

	if (pSellOrder != NULL)
	{
		theSellOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
		m_pMDB->m_OrderFactory->update(pSellOrder, &theSellOrder, pTransaction);
	}

	pTransaction->commit();

	pSellOrderIterator->free();
	pBuyOrderIterator->free();

	m_pErrorEngine->getLastError();

	return true;
}

void CCFDWedge::tryTradeAfterOrderInsert(COrder *pOrder)
{
	if (!(pOrder->OrderPriceType == OPT_CFDPrice || pOrder->OrderType == OT_FlashTrade))
		return;

	if (pOrder->VolumeRemain <= 0)
		return;

	if (g_sCFDTaker.isNull())
	{
		REPORT_EVENT(LOG_CRITICAL, "CCFDWedge", "CFDTakerIsNull");
		return;
	}
	CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(g_sCFDTaker);
	if (pMember == NULL)
	{
		REPORT_EVENT(LOG_CRITICAL, "CCFDWedge", "MemberNotFound[%s]", g_sCFDTaker.getValue());
		return;
	}

	//接管账户控制使用
	//CTransactionSavePoint *pBeforeInsert = CTransactionSavePoint::alloc(pTransaction);
	CTransaction theTransaction;
	theOrder.init();
	theOrder.ExchangeID = pOrder->ExchangeID;
	theOrder.InstrumentID = pOrder->InstrumentID;
	theOrder.Volume = pOrder->VolumeRemain;
	theOrder.OrderPriceType = OPT_LimitPrice;
	theOrder.Price = pOrder->Price;
//	theOrder.TimeCondition = TC_GTC;
	theOrder.OrderType = OT_Normal;
	theOrder.DeriveSource = DS_FromOTCTrade_CFD;
	theOrder.Remark = pOrder->MemberID;
	theOrder.MemberID = g_sCFDTaker;
	theOrder.TradeUnitID = g_sCFDTaker;
	theOrder.UserID = pOrder->UserID;
	theOrder.AccountID = g_sCFDTaker;
	theOrder.linkMember(pMember);
	if (pOrder->Direction == D_Buy)
		theOrder.Direction = D_Sell;
	else
		theOrder.Direction = D_Buy;
	theOrder.OffsetFlag = OF_Open;
	theOrder.Tradable = false;
	theOrder.RelatedOrderID = pOrder->OrderID;
	theOrder.IsCrossMargin.clear();
	theOrder.linkInstrument(pOrder->getInstrument());
	theOrder.linkPosition((CPosition*)NULL);
	theOrder.linkTriggerOrder((CTriggerOrder*)NULL);

	if (!m_TradeService->insertOrder(&theOrder, &theTransaction))
	{
		char* errMsg = NULL;
		int errorCode = 0;
		m_pErrorEngine->getLastError(&errorCode, &errMsg);
		REPORT_EVENT(LOG_CRITICAL, "CCFDWedge:tryTradeAfterOrderInsert", "ErrorID[%d],ErrorMsg[%s]", errorCode, errMsg);
		theTransaction.rollback();
		return;
	}

	CIndexIDType tradeID = "";
	m_TradeMaker->genSingleTrade(pOrder->getInstrument(), &theOrder, theOrder.Direction, theOrder.Price, theOrder.VolumeRemain, &tradeID, DS_FromOTCTrade_CFD, MR_Maker, false);
	COrder *pMDBOrder = m_pMDB->m_OrderFactory->findByOrderID(theOrder.OrderID);
	theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	if (pMDBOrder != NULL) 
	{
		m_pMDB->m_OrderFactory->update(pMDBOrder, &theOrder, &theTransaction);
		if (pMDBOrder->getPosition() != NULL)
		{
			theTransaction.addResource(CPositionResource::alloc(UPDATE_ACTION, m_pMDB->m_PositionFactory, pMDBOrder->getPosition()));
			if (pMDBOrder->getPosition()->getAccount() != NULL)
				theTransaction.addResource(CAccountResource::alloc(UPDATE_ACTION, m_pMDB->m_AccountFactory, pMDBOrder->getPosition()->getAccount()));
			else
				REPORT_EVENT(LOG_ERROR, "CCFDWedge:tryTradeAfterOrderInsert", "pMDBOrder->getPosition()->getAccount() is NULL");
		}
		else
		{
			REPORT_EVENT(LOG_ERROR, "CCFDWedge:tryTradeAfterOrderInsert", "pMDBOrder->getPosition() is NULL");
		}			
		theTransaction.commit();
	}

	m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);
	m_TradeMaker->genSingleTrade(pOrder->getInstrument(), &theOrder, theOrder.Direction, theOrder.Price, theOrder.VolumeRemain, &tradeID, DS_FromOTCTrade_CFD, MR_Taker, false);
	theOrder.BusinessNo = m_gBusinessSequence.getCurrentValueByDouble();
	m_pMDB->m_OrderFactory->update(pOrder, &theOrder, NULL, false);
}
