#ifndef INSTRUMENTORDERBOOK_H
#define INSTRUMENTORDERBOOK_H
#include "platform.h"
#include "mdb.h"
#include "MemoryStack.h"

///买报单序列排序方法，价格从大到小，优先级从大到小，时间从小到大
class BuyOrderBookLessPred :public binary_function<COrder*, COrder*, bool>
{
public:
	///判断小于的谓词
	///@param	lValue	左边的值
	///@param	rValue	右边的值
	///@return	true表示小于，false表示不小于
	bool operator() (const COrder* lValue, const COrder* rValue) const;
};

///卖报单序列排序方法，价格从小到大，优先级从大到小，时间从小到大
class SellOrderBookLessPred
{
public:
	///判断小于的谓词
	///@param	lValue	左边的值
	///@param	rValue	右边的值
	///@return	true表示小于，false表示不小于
	bool operator() (const COrder* lValue, const COrder* rValue) const;
};

typedef std::set<COrder*, BuyOrderBookLessPred, std::allocator<COrder *> > TBuyOrderBook;
typedef std::set<COrder*, SellOrderBookLessPred, std::allocator<COrder *> > TSellOrderBook;

///这是一个从撮合队列中返回报单的Iterator。这里的T必须是set<COrder*,某谓词>
class CBuyOrderIteratorByDirection :public COrderIterator
{
private:
	///指向当前节点的迭代器，当m_bFirst为flase时有效，等于m_pSet->end()表示已经走完了
	TBuyOrderBook::iterator m_it;

	///是否已经进行了预取
	bool m_bPreget;

	///是否是第一次取
	bool m_bFisrt;

	///使用的set
	TBuyOrderBook* m_pSet;

	static CMemoryStack<CBuyOrderIteratorByDirection> iteratorList;
public:
	///构造函数
	CBuyOrderIteratorByDirection(void)
	{
	}

	///初始化
	///@param	pSet	指定的set
	///@param	pFactory	指定的对象工厂
	void init(TBuyOrderBook* pSet, COrderFactory *pFactory)
	{
		m_pSet = pSet;
		m_bFisrt = true;
		m_bPreget = false;
		m_Factory = pFactory;
	}

	///申请一个迭代器
	///@param	pSet	指定的set
	///@param	pFactory	指定的对象工厂
	///@return	得到的迭代器
	static CBuyOrderIteratorByDirection *alloc(TBuyOrderBook* pSet, COrderFactory *pFactory)
	{
		CBuyOrderIteratorByDirection *it;
		it = iteratorList.alloc();
		it->init(pSet, pFactory);
		return it;
	}

	///删除当前记录
	///@param	pTransaction	指定的事务
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL)
	{
		TBuyOrderBook::iterator last = m_it;
		m_it++;
		m_bPreget = true;
		m_Factory->remove(*last, pTransaction);
	}

	///更新当前记录
	///@param	pOrder	要修改成的新纪录
	///@param	pTransaction	指定的事务
	///@param	updateIndex	是否要更新索引
	virtual void UpdateCurrent(CWriteableOrder * pOrder, CTransaction *pTransaction = NULL, bool updateIndex = true)
	{
		TBuyOrderBook::iterator last = m_it;
		m_it++;
		m_bPreget = true;
		m_Factory->update(*last, pOrder, pTransaction, updateIndex);
	}

	///按次序获取下一条记录
	///@return	返回下一条记录
	virtual COrder* next(void)
	{
		if (m_bPreget)
		{
			m_bPreget = false;
			if (m_it == m_pSet->end())
			{
				return NULL;
			}
			return *m_it;
		}
		if (m_bFisrt)
		{
			m_bFisrt = false;
			m_it = m_pSet->begin();
			if (m_it == m_pSet->end())
			{
				return NULL;
			}
		}
		else
		{
			if (m_it == m_pSet->end())
			{
				return NULL;
			}
			m_it++;
			if (m_it == m_pSet->end())
			{
				return NULL;
			}
		}
		return *m_it;
	}

	///释放自己
	virtual void free(void)
	{
		iteratorList.free(this);
	}
};

///这是一个从撮合队列中返回报单的Iterator。这里的T必须是set<COrder*,某谓词>
class CSellOrderIteratorByDirection :public COrderIterator
{
private:
	///指向当前节点的迭代器，当m_bFirst为flase时有效，等于m_pSet->end()表示已经走完了
	TSellOrderBook::iterator m_it;

	///是否已经进行了预取
	bool m_bPreget;

	///是否是第一次取
	bool m_bFisrt;

	///使用的set
	TSellOrderBook* m_pSet;

	static CMemoryStack<CSellOrderIteratorByDirection> iteratorList;
public:
	///构造函数
	CSellOrderIteratorByDirection(void)
	{
	}

	///初始化
	///@param	pSet	指定的set
	///@param	pFactory	指定的对象工厂
	void init(TSellOrderBook* pSet, COrderFactory *pFactory)
	{
		m_pSet = pSet;
		m_bFisrt = true;
		m_bPreget = false;
		m_Factory = pFactory;
	}

	///申请一个迭代器
	///@param	pSet	指定的set
	///@param	pFactory	指定的对象工厂
	///@return	得到的迭代器
	static CSellOrderIteratorByDirection *alloc(TSellOrderBook* pSet, COrderFactory *pFactory)
	{
		CSellOrderIteratorByDirection *it;
		it = iteratorList.alloc();
		it->init(pSet, pFactory);
		return it;
	}

	///删除当前记录
	///@param	pTransaction	指定的事务
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL)
	{
		TSellOrderBook::iterator last = m_it;
		m_it++;
		m_bPreget = true;
		m_Factory->remove(*last, pTransaction);
	}

	///更新当前记录
	///@param	pOrder	要修改成的新纪录
	///@param	pTransaction	指定的事务
	///@param	updateIndex	是否要更新索引
	virtual void UpdateCurrent(CWriteableOrder * pOrder, CTransaction *pTransaction = NULL, bool updateIndex = true)
	{
		TSellOrderBook::iterator last = m_it;
		m_it++;
		m_bPreget = true;
		m_Factory->update(*last, pOrder, pTransaction, updateIndex);
	}

	///按次序获取下一条记录
	///@return	返回下一条记录
	virtual COrder* next(void)
	{
		if (m_bPreget)
		{
			m_bPreget = false;
			if (m_it == m_pSet->end())
			{
				return NULL;
			}
			return *m_it;
		}
		if (m_bFisrt)
		{
			m_bFisrt = false;
			m_it = m_pSet->begin();
			if (m_it == m_pSet->end())
			{
				return NULL;
			}
		}
		else
		{
			if (m_it == m_pSet->end())
			{
				return NULL;
			}
			m_it++;
			if (m_it == m_pSet->end())
			{
				return NULL;
			}
		}
		return *m_it;
	}

	///释放自己
	virtual void free(void)
	{
		iteratorList.free(this);
	}
};

///某个合约的报单簿
class COrderBook
{
	friend class COrderBookList;
public:
	///买报单簿
	TBuyOrderBook m_BuyBook;

	///卖报单簿
	TSellOrderBook m_SellBook;
private:
	///获取买报单迭代器
	///@param	pFactory	指定的报单对象工厂
	///@return	返回买入报单的迭代器
	COrderIterator* getBuyOrderIterator(COrderFactory *pFactory)
	{
		return CBuyOrderIteratorByDirection::alloc(&m_BuyBook, pFactory);
	}

	///获取卖报单迭代器
	///@param	pFactory	指定的报单对象工厂
	///@return	返回卖出报单的迭代器
	COrderIterator* getSellOrderIterator(COrderFactory *pFactory)
	{
		return CSellOrderIteratorByDirection::alloc(&m_SellBook, pFactory);
	}

	///插入报单
	///@param	pOrder	新插入的报单
	void InsertOrder(COrder *pOrder);

	///删除报单
	///@param	pOrder	要删除的报单
	void RemoveOrder(COrder *pOrder);

	///更新报单
	///@param	pOrder	原来的报单
	///@param	pNewOrder	要修改成的新报单
	//void UpdateOrder(COrder *pOrder, COrder* pNewOrder);
};


///一个负责将Order存贮在不同OrderBook中的类
class COrderBookList
{
public:
	COrderBookList();
	COrderBookList(CMDB *pMDB);
	~COrderBookList();

	///插入报单
	///@param	pOrder	新插入的报单
	void InsertOrder(COrder *pOrder);

	///删除报单
	///@param	pOrder	要删除的报单
	void removeOrder(COrder *pOrder);

	///更新报单
	///@param	pOrder	原来的报单
	///@param	pNewOrder	要修改成的新报单
	//void updateOrder(COrder *pOrder, COrder* pNewOrder);

	///插入合约
	///@param	pInstrument	要增加的合约
	void addInstrument(CInstrument* pInstrument);

	///删除合约
	///@param	pInstrument	要删除的合约
	void removeInstrument(CInstrument* pInstrument);

	///根据合约，买卖方向取报单迭代器
	///@param	pInstrument	指定的合约
	///@param	direction	指定的买卖方向
	///@return	得到的报单迭代器
	COrderIterator* getOrderIterator(CInstrument* pInstrument, CDirectionType direction);

private:
	///根据合约取对应的报单簿
	///@param	pInstrument	指定的合约
	///@return	对应的报单簿，找不到则返回NULL
	COrderBook* getOrderBook(CInstrument* pInstrument);

private:
	typedef map<CInstrument *, COrderBook* > ORDERBOOKMAP;	///OrderBook列表
	ORDERBOOKMAP m_OrderBookList;
	CMDB *m_pMDB;
};

///为OrderBook准备的合约触发器
class CInstrumentTriggerForOrderBook :public  CInstrumentCommitTrigger// CInstrumentActionTrigger
{
public:
	///构造函数
	///@param	pOrderBookList	所有OrderBook的清单
	CInstrumentTriggerForOrderBook(COrderBookList *pOrderBookList);

	/////加入后触发
	/////@param	pInstrument	已经加入的CInstrument
	//virtual void afterAdd(CInstrument *pInstrument);

	/////删除前触发
	/////@param	pInstrument	要删除的CInstrument
	//virtual void beforeRemove(CInstrument *pInstrument);

	///加入后触发
	///@param	pInstrument	已经加入的Instrument
	virtual void commitAdd(CInstrument *pInstrument);

	virtual void commitRemove(CWriteableInstrument *pInstrument);

private:
	COrderBookList *m_pOrderBookList;
};

///报单修改时，影响报单簿的触发器
class COrderTriggerForOrderBook :public COrderCommitTrigger //COrderActionTrigger
{
public:
	///构造函数
	///@param	pOrderBookList	所有OrderBook的清单
	COrderTriggerForOrderBook(COrderBookList *pOrderBookList);

	/////加入后触发
	/////@param	pOrder	已经加入的Order
	//virtual void afterAdd(COrder *pOrder);

	/////删除前触发
	/////@param	pOrder	要删除的COrder
	//virtual void beforeRemove(COrder *pOrder);

	/////更新前触发	
	/////@param	pOrder	被刷新的COrder
	/////@param	pNewOrder	新的值
	//virtual void beforeUpdate(COrder *pOrder, CWriteableOrder *pNewOrder);

	/////更新后触发
	/////@param	pOrder	被刷新的COrder
	//virtual void afterUpdate(COrder *pOrder);

	virtual void commitAdd(COrder *pOrder);

	virtual void commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder);

private:

	COrderBookList *m_pOrderBookList;
	//UF_INT8 m_TimeSortNo;
};

#endif
