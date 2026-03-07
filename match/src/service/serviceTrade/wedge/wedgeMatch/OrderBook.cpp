#include "OrderBook.h"
#include "monitorIndex.h"
#include "tradingTools.h"

CMemoryStack<CBuyOrderIteratorByDirection> CBuyOrderIteratorByDirection::iteratorList;
CMemoryStack<CSellOrderIteratorByDirection> CSellOrderIteratorByDirection::iteratorList;

bool BuyOrderBookLessPred::operator() (const COrder* lValue,const COrder* rValue) const
{
	if (lValue->Price > rValue->Price)
		return true;
	if (lValue->Price < rValue->Price)
		return false;
	//if (lValue->Priority > rValue->Priority)
	//	return true;
	//if (lValue->Priority < rValue->Priority)
	//	return false;
//	return lValue->TimeSortNo < rValue->TimeSortNo;
//	if (lValue->TimeSortNo < rValue->TimeSortNo)
//		return true;
//	if (lValue->TimeSortNo > rValue->TimeSortNo)
//		return false;
//	return lValue->ImplySortNo < rValue->ImplySortNo;

	if (lValue->TimeSortNo.getValue() < rValue->TimeSortNo.getValue())
		return true;
	return false;
}

bool SellOrderBookLessPred::operator() (const COrder* lValue,const COrder* rValue) const
{
	if (lValue->Price < rValue->Price)
		return true;
	if (lValue->Price > rValue->Price)
		return false;
	//if (lValue->Priority > rValue->Priority)
	//	return true;
	//if (lValue->Priority < rValue->Priority)
	//	return false;
	//return lValue->TimeSortNo < rValue->TimeSortNo;
//	if (lValue->TimeSortNo < rValue->TimeSortNo)
//		return true;
//	if (lValue->TimeSortNo > rValue->TimeSortNo)
//		return false;
//	return lValue->ImplySortNo < rValue->ImplySortNo;

	if (lValue->TimeSortNo.getValue() < rValue->TimeSortNo.getValue())
		return true;
	return false;
}

///********** OrderBookList *****************///
COrderBookList::COrderBookList(CMDB *pMDB)
{
	m_pMDB = pMDB;
}

COrderBookList::~COrderBookList()
{
	ORDERBOOKMAP::iterator it = m_OrderBookList.begin();
	while(it != m_OrderBookList.end())
	{
		COrderBook* pBook = it->second;
		delete pBook;
		it++;
	}
	m_OrderBookList.clear();
}

///********** COrderBook *****************///
void COrderBook::InsertOrder(COrder *pOrder)
{
	if(pOrder->Direction == D_Buy)
		m_BuyBook.insert(pOrder);
	else
		m_SellBook.insert(pOrder);
}

void COrderBook::RemoveOrder(COrder *pOrder)
{
	if(pOrder->Direction == D_Buy)
	{
		TBuyOrderBook::iterator it = m_BuyBook.find(pOrder);
		if (it != m_BuyBook.end())
		{
			m_BuyBook.erase(it);
		}
		else
		{
			REPORT_EVENT(LOG_CRITICAL, "InternalError", "BuyBook[%s][%0.f] can not find order to be removed", pOrder->OrderID.getValue(), g_nSequenceNo);
			return;
		}
	}
	else
	{
		TSellOrderBook::iterator it = m_SellBook.find(pOrder);
		if (it != m_SellBook.end())
		{
			m_SellBook.erase(it);
		}
		else
		{
			REPORT_EVENT(LOG_CRITICAL, "InternalError", "SellBook[%s][%0.f] can not find order to be removed", pOrder->OrderID.getValue(), g_nSequenceNo);
			return;
		}
	}
}

//void COrderBook::UpdateOrder(COrder *pOrder,COrder *pNewOrder)
//{
//	RemoveOrder(pOrder);
//	InsertOrder(pNewOrder);
//}

///********** COrderBookList *****************///

COrderBook* COrderBookList::getOrderBook(CInstrument* pInstrument)
{
	ORDERBOOKMAP::iterator it = m_OrderBookList.find(pInstrument);
	if(it != m_OrderBookList.end())
		return it->second;
	return NULL;
}

void COrderBookList::InsertOrder(COrder *pOrder)
{
	//从中间流水反演的情况下，需要找一下合约
	if (pOrder->getInstrument() == NULL)
		pOrder->linkInstrument(m_pMDB->m_InstrumentFactory->findByInstrumentID(pOrder->ExchangeID,pOrder->InstrumentID));

	COrderBook *pBook = getOrderBook(pOrder->getInstrument());
	if (pBook != NULL)
		pBook->InsertOrder(pOrder);
}

void COrderBookList::removeOrder(COrder *pOrder)
{
	COrderBook* pBook = getOrderBook(pOrder->getInstrument());
	if (pBook != NULL)
		pBook->RemoveOrder(pOrder);
}

//void COrderBookList::updateOrder(COrder *pOrder,COrder *pNewOrder)
//{
//	COrderBook* pBook = getOrderBook(pOrder->getInstrument());
//	if (pBook != NULL)
//		pBook->UpdateOrder(pOrder,pNewOrder);
//}

void COrderBookList::addInstrument(CInstrument* pInstrument)
{
	///原来已经有此合约了，这出现在一天多次结算的情况下
	if (getOrderBook(pInstrument)!=NULL)
		return;
	COrderBook* pNewOrderBook = new COrderBook();
	m_OrderBookList.insert(ORDERBOOKMAP::value_type(pInstrument,pNewOrderBook));
}

void COrderBookList::removeInstrument(CInstrument* pInstrument)
{
	ORDERBOOKMAP::iterator it = m_OrderBookList.find(pInstrument);
	if (it == m_OrderBookList.end())		///无法删除
		return;

	COrderBook* pOrderBook=it->second;
	m_OrderBookList.erase(it);
	delete pOrderBook;
}

COrderIterator* COrderBookList::getOrderIterator(CInstrument* pInstrument,CDirectionType direction)
{
	COrderBook* pOrderBook = getOrderBook(pInstrument);
	if(pOrderBook == NULL)
		return NULL;

	if(direction == D_Buy)
		return pOrderBook->getBuyOrderIterator(m_pMDB->m_OrderFactory);
	else
		return pOrderBook->getSellOrderIterator(m_pMDB->m_OrderFactory);
}


CInstrumentTriggerForOrderBook::CInstrumentTriggerForOrderBook(COrderBookList *pOrderBookList)
	:m_pOrderBookList(pOrderBookList)
{

}

//void CInstrumentTriggerForOrderBook::afterAdd(CInstrument *pInstrument)
//{
//	m_pOrderBookList->addInstrument(pInstrument);
//}
//
//void CInstrumentTriggerForOrderBook::beforeRemove(CInstrument *pInstrument)
//{
//	m_pOrderBookList->removeInstrument(pInstrument);
//}

/////加入后触发
///@param	pInstrument	已经加入的Instrument
void CInstrumentTriggerForOrderBook::commitAdd(CInstrument *pInstrument)
{
	m_pOrderBookList->addInstrument(pInstrument);
}

void CInstrumentTriggerForOrderBook::commitRemove(CWriteableInstrument *pInstrument)
{
	m_pOrderBookList->removeInstrument(pInstrument);
}


COrderTriggerForOrderBook::COrderTriggerForOrderBook(COrderBookList *pOrderBookList):m_pOrderBookList(pOrderBookList)
{
}

//void COrderTriggerForOrderBook::afterAdd(COrder *pOrder)
//{
//	if (needInOrderBook(pOrder))
//		m_pOrderBookList->InsertOrder(pOrder);
//}
//
//void COrderTriggerForOrderBook::beforeRemove(COrder *pOrder)
//{
//	if (needInOrderBook(pOrder))
//		m_pOrderBookList->removeOrder(pOrder);
//}
//
//void COrderTriggerForOrderBook::beforeUpdate(COrder *pOrder, CWriteableOrder *pNewOrder)
//{
////	if (needInOrderBook(pOrder))
////		m_pOrderBookList->removeOrder(pOrder);
//	m_TimeSortNo = pOrder->TimeSortNo;
//	if (!needInOrderBook(pNewOrder))
//	{
//		m_pOrderBookList->removeOrder(pOrder);
//	}
//	else
//	{
//		if (pNewOrder->TimeSortNo != pOrder->TimeSortNo)
//			m_pOrderBookList->removeOrder(pOrder);
//	}
//}
//
//void COrderTriggerForOrderBook::afterUpdate(COrder *pOrder)
//{
////	if (needInOrderBook(pOrder))
////		m_pOrderBookList->InsertOrder(pOrder);
//	if (pOrder->TimeSortNo != m_TimeSortNo)
//		m_pOrderBookList->InsertOrder(pOrder);
//}

void COrderTriggerForOrderBook::commitAdd(COrder *pOrder)
{
	if (needInOrderBook(pOrder))
		m_pOrderBookList->InsertOrder(pOrder);
	return;
}

void COrderTriggerForOrderBook::commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder)
{
	if (needInOrderBook(pOldOrder))
	{
		if (needInOrderBook(pOrder))
		{
			if (pOrder->TimeSortNo != pOldOrder->TimeSortNo)
			{
				//这个操作实在是处于无奈，各种原因难以表述
				CPriceType Price = pOrder->Price;
				CLargeNumberType TimeSortNo = pOrder->TimeSortNo;
				((CWriteableOrder *)pOrder)->Price = pOldOrder->Price;
				((CWriteableOrder *)pOrder)->TimeSortNo = pOldOrder->TimeSortNo;
				m_pOrderBookList->removeOrder(pOrder);
				((CWriteableOrder *)pOrder)->Price = Price;
				((CWriteableOrder *)pOrder)->TimeSortNo = TimeSortNo;

				m_pOrderBookList->InsertOrder(pOrder);
			}
		}			
		else
		{
			//这个操作实在是处于无奈，各种原因难以表述
			CPriceType Price = pOrder->Price;
			CLargeNumberType TimeSortNo = pOrder->TimeSortNo;
			((CWriteableOrder *)pOrder)->Price = pOldOrder->Price;
			((CWriteableOrder *)pOrder)->TimeSortNo = pOldOrder->TimeSortNo;
			m_pOrderBookList->removeOrder(pOrder);
			((CWriteableOrder *)pOrder)->Price = Price;
			((CWriteableOrder *)pOrder)->TimeSortNo = TimeSortNo;
		}
	}
	else
	{
		if (needInOrderBook(pOrder))
			m_pOrderBookList->InsertOrder(pOrder);
	}
	return;
}
