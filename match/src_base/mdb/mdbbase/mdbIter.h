/////////////////////////////////////////////////////////////////////////
///mdbIter.h
///定义了若干个内存数据库的iterator
/////////////////////////////////////////////////////////////////////////

#ifndef DATABASEITERATORS_H
#define DATABASEITERATORS_H

#include "mdbStruct.h"
#include "mdbTable.h"
#include "MemoryStack.h"



/////////////////////////////////////////////////////////////////////////
///CServiceConfigIteratorByIndex1是一个对业务配置使用ByIndex1方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CServiceConfigIteratorByIndex1: public CServiceConfigIterator
{
	friend class CServiceConfigFactory;
private:
	///检查此对象是否满足条件
	///@param	pServiceConfig		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CServiceConfig *pServiceConfig);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableServiceConfig m_compareObject;
	
	static CMemoryStack<CServiceConfigIteratorByIndex1> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CServiceConfigIteratorByIndex1(CServiceConfigFactory *pFactory
		,const CReadOnlyNameType& ConfigName,const CReadOnlyNameType& Index1)
	{
		init(pFactory,ConfigName,Index1);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CServiceConfigIteratorByIndex1 *alloc(CServiceConfigFactory *pFactory
		,const CReadOnlyNameType& ConfigName,const CReadOnlyNameType& Index1);

	///构造方法
	CServiceConfigIteratorByIndex1(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CServiceConfigFactory *pFactory
		,const CReadOnlyNameType& ConfigName,const CReadOnlyNameType& Index1);

	///析构方法，将负责释放所占用的空间
	virtual ~CServiceConfigIteratorByIndex1(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CServiceConfig，必须在startFindByIndex1以后，endFindByIndex1之前叫用
	///@return	下一个满足条件CServiceConfig，如果已经没有一个满足要求了，则返回NULL
	virtual CServiceConfig *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByIndex1 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableServiceConfig * pByIndex1,CTransaction *pTransaction = NULL, bool updateIndex=true);
};


/////////////////////////////////////////////////////////////////////////
///CInstrumentIteratorBySettlementGroup是一个对标的使用BySettlementGroup方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CInstrumentIteratorBySettlementGroup: public CInstrumentIterator
{
	friend class CInstrumentFactory;
private:
	///检查此对象是否满足条件
	///@param	pInstrument		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CInstrument *pInstrument);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableInstrument m_compareObject;
	
	static CMemoryStack<CInstrumentIteratorBySettlementGroup> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CInstrumentIteratorBySettlementGroup(CInstrumentFactory *pFactory
		,const CReadOnlyShortIndexIDType& SettlementGroup)
	{
		init(pFactory,SettlementGroup);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CInstrumentIteratorBySettlementGroup *alloc(CInstrumentFactory *pFactory
		,const CReadOnlyShortIndexIDType& SettlementGroup);

	///构造方法
	CInstrumentIteratorBySettlementGroup(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CInstrumentFactory *pFactory
		,const CReadOnlyShortIndexIDType& SettlementGroup);

	///析构方法，将负责释放所占用的空间
	virtual ~CInstrumentIteratorBySettlementGroup(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CInstrument，必须在startFindBySettlementGroup以后，endFindBySettlementGroup之前叫用
	///@return	下一个满足条件CInstrument，如果已经没有一个满足要求了，则返回NULL
	virtual CInstrument *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pBySettlementGroup 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableInstrument * pBySettlementGroup,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CInstrumentIteratorByProductGroup是一个对标的使用ByProductGroup方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CInstrumentIteratorByProductGroup: public CInstrumentIterator
{
	friend class CInstrumentFactory;
private:
	///检查此对象是否满足条件
	///@param	pInstrument		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CInstrument *pInstrument);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableInstrument m_compareObject;
	
	static CMemoryStack<CInstrumentIteratorByProductGroup> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CInstrumentIteratorByProductGroup(CInstrumentFactory *pFactory
		,const CReadOnlyShortIndexIDType& ProductGroup)
	{
		init(pFactory,ProductGroup);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CInstrumentIteratorByProductGroup *alloc(CInstrumentFactory *pFactory
		,const CReadOnlyShortIndexIDType& ProductGroup);

	///构造方法
	CInstrumentIteratorByProductGroup(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CInstrumentFactory *pFactory
		,const CReadOnlyShortIndexIDType& ProductGroup);

	///析构方法，将负责释放所占用的空间
	virtual ~CInstrumentIteratorByProductGroup(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CInstrument，必须在startFindByProductGroup以后，endFindByProductGroup之前叫用
	///@return	下一个满足条件CInstrument，如果已经没有一个满足要求了，则返回NULL
	virtual CInstrument *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByProductGroup 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableInstrument * pByProductGroup,CTransaction *pTransaction = NULL, bool updateIndex=true);
};





/////////////////////////////////////////////////////////////////////////
///CCurrencyIteratorBySettlementGroup是一个对资金账户币种信息使用BySettlementGroup方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCurrencyIteratorBySettlementGroup: public CCurrencyIterator
{
	friend class CCurrencyFactory;
private:
	///检查此对象是否满足条件
	///@param	pCurrency		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CCurrency *pCurrency);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableCurrency m_compareObject;
	
	static CMemoryStack<CCurrencyIteratorBySettlementGroup> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CCurrencyIteratorBySettlementGroup(CCurrencyFactory *pFactory
		,const CReadOnlyShortIndexIDType& SettlementGroup)
	{
		init(pFactory,SettlementGroup);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CCurrencyIteratorBySettlementGroup *alloc(CCurrencyFactory *pFactory
		,const CReadOnlyShortIndexIDType& SettlementGroup);

	///构造方法
	CCurrencyIteratorBySettlementGroup(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CCurrencyFactory *pFactory
		,const CReadOnlyShortIndexIDType& SettlementGroup);

	///析构方法，将负责释放所占用的空间
	virtual ~CCurrencyIteratorBySettlementGroup(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CCurrency，必须在startFindBySettlementGroup以后，endFindBySettlementGroup之前叫用
	///@return	下一个满足条件CCurrency，如果已经没有一个满足要求了，则返回NULL
	virtual CCurrency *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pBySettlementGroup 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableCurrency * pBySettlementGroup,CTransaction *pTransaction = NULL, bool updateIndex=true);
};


/////////////////////////////////////////////////////////////////////////
///CAccountIteratorByMemberID是一个对资金账户使用ByMemberID方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountIteratorByMemberID: public CAccountIterator
{
	friend class CAccountFactory;
private:
	///检查此对象是否满足条件
	///@param	pAccount		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CAccount *pAccount);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableAccount m_compareObject;
	
	static CMemoryStack<CAccountIteratorByMemberID> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CAccountIteratorByMemberID(CAccountFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID)
	{
		init(pFactory,MemberID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CAccountIteratorByMemberID *alloc(CAccountFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID);

	///构造方法
	CAccountIteratorByMemberID(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CAccountFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID);

	///析构方法，将负责释放所占用的空间
	virtual ~CAccountIteratorByMemberID(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CAccount，必须在startFindByMemberID以后，endFindByMemberID之前叫用
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	virtual CAccount *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByMemberID 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableAccount * pByMemberID,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CAccountIteratorByAccountID是一个对资金账户使用ByAccountID方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountIteratorByAccountID: public CAccountIterator
{
	friend class CAccountFactory;
private:
	///检查此对象是否满足条件
	///@param	pAccount		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CAccount *pAccount);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableAccount m_compareObject;
	
	static CMemoryStack<CAccountIteratorByAccountID> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CAccountIteratorByAccountID(CAccountFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID,const CReadOnlyAccountIDType& AccountID)
	{
		init(pFactory,MemberID,AccountID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CAccountIteratorByAccountID *alloc(CAccountFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID,const CReadOnlyAccountIDType& AccountID);

	///构造方法
	CAccountIteratorByAccountID(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CAccountFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID,const CReadOnlyAccountIDType& AccountID);

	///析构方法，将负责释放所占用的空间
	virtual ~CAccountIteratorByAccountID(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CAccount，必须在startFindByAccountID以后，endFindByAccountID之前叫用
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	virtual CAccount *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByAccountID 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableAccount * pByAccountID,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CAccountIteratorBySettlementGroup是一个对资金账户使用BySettlementGroup方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountIteratorBySettlementGroup: public CAccountIterator
{
	friend class CAccountFactory;
private:
	///检查此对象是否满足条件
	///@param	pAccount		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CAccount *pAccount);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableAccount m_compareObject;
	
	static CMemoryStack<CAccountIteratorBySettlementGroup> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CAccountIteratorBySettlementGroup(CAccountFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID,const CReadOnlyAccountIDType& AccountID,const CReadOnlyShortIndexIDType& SettlementGroup)
	{
		init(pFactory,MemberID,AccountID,SettlementGroup);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CAccountIteratorBySettlementGroup *alloc(CAccountFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID,const CReadOnlyAccountIDType& AccountID,const CReadOnlyShortIndexIDType& SettlementGroup);

	///构造方法
	CAccountIteratorBySettlementGroup(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CAccountFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID,const CReadOnlyAccountIDType& AccountID,const CReadOnlyShortIndexIDType& SettlementGroup);

	///析构方法，将负责释放所占用的空间
	virtual ~CAccountIteratorBySettlementGroup(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CAccount，必须在startFindBySettlementGroup以后，endFindBySettlementGroup之前叫用
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	virtual CAccount *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pBySettlementGroup 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableAccount * pBySettlementGroup,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CAccountIteratorByAll是一个对资金账户使用ByAll方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountIteratorByAll: public CAccountIterator
{
	friend class CAccountFactory;
private:
	///检查此对象是否满足条件
	///@param	pAccount		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CAccount *pAccount);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableAccount m_compareObject;
	
	static CMemoryStack<CAccountIteratorByAll> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CAccountIteratorByAll(CAccountFactory *pFactory
		)
	{
		init(pFactory);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CAccountIteratorByAll *alloc(CAccountFactory *pFactory
		);

	///构造方法
	CAccountIteratorByAll(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CAccountFactory *pFactory
		);

	///析构方法，将负责释放所占用的空间
	virtual ~CAccountIteratorByAll(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CAccount，必须在startFindByAll以后，endFindByAll之前叫用
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	virtual CAccount *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByAll 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableAccount * pByAll,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CAccountIteratorByCurrency是一个对资金账户使用ByCurrency方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountIteratorByCurrency: public CAccountIterator
{
	friend class CAccountFactory;
private:
	///检查此对象是否满足条件
	///@param	pAccount		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CAccount *pAccount);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableAccount m_compareObject;
	
	static CMemoryStack<CAccountIteratorByCurrency> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CAccountIteratorByCurrency(CAccountFactory *pFactory
		,const CReadOnlyShortIndexIDType& SettlementGroup,const CReadOnlyCurrencyType& Currency)
	{
		init(pFactory,SettlementGroup,Currency);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CAccountIteratorByCurrency *alloc(CAccountFactory *pFactory
		,const CReadOnlyShortIndexIDType& SettlementGroup,const CReadOnlyCurrencyType& Currency);

	///构造方法
	CAccountIteratorByCurrency(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CAccountFactory *pFactory
		,const CReadOnlyShortIndexIDType& SettlementGroup,const CReadOnlyCurrencyType& Currency);

	///析构方法，将负责释放所占用的空间
	virtual ~CAccountIteratorByCurrency(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CAccount，必须在startFindByCurrency以后，endFindByCurrency之前叫用
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	virtual CAccount *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByCurrency 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableAccount * pByCurrency,CTransaction *pTransaction = NULL, bool updateIndex=true);
};


/////////////////////////////////////////////////////////////////////////
///CPositionIteratorByAccountInstrument是一个对交易单元持仓使用ByAccountInstrument方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionIteratorByAccountInstrument: public CPositionIterator
{
	friend class CPositionFactory;
private:
	///检查此对象是否满足条件
	///@param	pPosition		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CPosition *pPosition);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteablePosition m_compareObject;
	
	static CMemoryStack<CPositionIteratorByAccountInstrument> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CPositionIteratorByAccountInstrument(CPositionFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID)
	{
		init(pFactory,AccountID,ExchangeID,InstrumentID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CPositionIteratorByAccountInstrument *alloc(CPositionFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID);

	///构造方法
	CPositionIteratorByAccountInstrument(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CPositionFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID);

	///析构方法，将负责释放所占用的空间
	virtual ~CPositionIteratorByAccountInstrument(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CPosition，必须在startFindByAccountInstrument以后，endFindByAccountInstrument之前叫用
	///@return	下一个满足条件CPosition，如果已经没有一个满足要求了，则返回NULL
	virtual CPosition *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByAccountInstrument 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteablePosition * pByAccountInstrument,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CPositionIteratorByAccountID是一个对交易单元持仓使用ByAccountID方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionIteratorByAccountID: public CPositionIterator
{
	friend class CPositionFactory;
private:
	///检查此对象是否满足条件
	///@param	pPosition		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CPosition *pPosition);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteablePosition m_compareObject;
	
	static CMemoryStack<CPositionIteratorByAccountID> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CPositionIteratorByAccountID(CPositionFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID)
	{
		init(pFactory,AccountID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CPositionIteratorByAccountID *alloc(CPositionFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID);

	///构造方法
	CPositionIteratorByAccountID(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CPositionFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID);

	///析构方法，将负责释放所占用的空间
	virtual ~CPositionIteratorByAccountID(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CPosition，必须在startFindByAccountID以后，endFindByAccountID之前叫用
	///@return	下一个满足条件CPosition，如果已经没有一个满足要求了，则返回NULL
	virtual CPosition *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByAccountID 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteablePosition * pByAccountID,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CPositionIteratorByLiquidPriceLong是一个对交易单元持仓使用ByLiquidPriceLong方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionIteratorByLiquidPriceLong: public CPositionIterator
{
	friend class CPositionFactory;
private:
	///检查此对象是否满足条件
	///@param	pPosition		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CPosition *pPosition);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteablePosition m_compareObject;
	
	static CMemoryStack<CPositionIteratorByLiquidPriceLong> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CPositionIteratorByLiquidPriceLong(CPositionFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& LiquidPrice)
	{
		init(pFactory,ExchangeID,InstrumentID,LiquidPrice);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CPositionIteratorByLiquidPriceLong *alloc(CPositionFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& LiquidPrice);

	///构造方法
	CPositionIteratorByLiquidPriceLong(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CPositionFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& LiquidPrice);

	///析构方法，将负责释放所占用的空间
	virtual ~CPositionIteratorByLiquidPriceLong(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CPosition，必须在startFindByLiquidPriceLong以后，endFindByLiquidPriceLong之前叫用
	///@return	下一个满足条件CPosition，如果已经没有一个满足要求了，则返回NULL
	virtual CPosition *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByLiquidPriceLong 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteablePosition * pByLiquidPriceLong,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CPositionIteratorByLiquidPriceShort是一个对交易单元持仓使用ByLiquidPriceShort方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionIteratorByLiquidPriceShort: public CPositionIterator
{
	friend class CPositionFactory;
private:
	///检查此对象是否满足条件
	///@param	pPosition		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CPosition *pPosition);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteablePosition m_compareObject;
	
	static CMemoryStack<CPositionIteratorByLiquidPriceShort> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CPositionIteratorByLiquidPriceShort(CPositionFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& LiquidPrice)
	{
		init(pFactory,ExchangeID,InstrumentID,LiquidPrice);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CPositionIteratorByLiquidPriceShort *alloc(CPositionFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& LiquidPrice);

	///构造方法
	CPositionIteratorByLiquidPriceShort(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CPositionFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& LiquidPrice);

	///析构方法，将负责释放所占用的空间
	virtual ~CPositionIteratorByLiquidPriceShort(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CPosition，必须在startFindByLiquidPriceShort以后，endFindByLiquidPriceShort之前叫用
	///@return	下一个满足条件CPosition，如果已经没有一个满足要求了，则返回NULL
	virtual CPosition *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByLiquidPriceShort 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteablePosition * pByLiquidPriceShort,CTransaction *pTransaction = NULL, bool updateIndex=true);
};

/////////////////////////////////////////////////////////////////////////
///CMarketDataIteratorByProductGroup是一个对行情使用ByProductGroup方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketDataIteratorByProductGroup: public CMarketDataIterator
{
	friend class CMarketDataFactory;
private:
	///检查此对象是否满足条件
	///@param	pMarketData		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CMarketData *pMarketData);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableMarketData m_compareObject;
	
	static CMemoryStack<CMarketDataIteratorByProductGroup> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CMarketDataIteratorByProductGroup(CMarketDataFactory *pFactory
		,const CReadOnlyShortIndexIDType& ProductGroup)
	{
		init(pFactory,ProductGroup);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CMarketDataIteratorByProductGroup *alloc(CMarketDataFactory *pFactory
		,const CReadOnlyShortIndexIDType& ProductGroup);

	///构造方法
	CMarketDataIteratorByProductGroup(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CMarketDataFactory *pFactory
		,const CReadOnlyShortIndexIDType& ProductGroup);

	///析构方法，将负责释放所占用的空间
	virtual ~CMarketDataIteratorByProductGroup(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CMarketData，必须在startFindByProductGroup以后，endFindByProductGroup之前叫用
	///@return	下一个满足条件CMarketData，如果已经没有一个满足要求了，则返回NULL
	virtual CMarketData *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByProductGroup 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableMarketData * pByProductGroup,CTransaction *pTransaction = NULL, bool updateIndex=true);
};

/////////////////////////////////////////////////////////////////////////
///COrderIteratorByAccountID是一个对报单使用ByAccountID方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class COrderIteratorByAccountID: public COrderIterator
{
	friend class COrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(COrder *pOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableOrder m_compareObject;
	
	static CMemoryStack<COrderIteratorByAccountID> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	COrderIteratorByAccountID(COrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID)
	{
		init(pFactory,AccountID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static COrderIteratorByAccountID *alloc(COrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID);

	///构造方法
	COrderIteratorByAccountID(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(COrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID);

	///析构方法，将负责释放所占用的空间
	virtual ~COrderIteratorByAccountID(void);
	
	virtual void free(void);

	///寻找下一个符合条件的COrder，必须在startFindByAccountID以后，endFindByAccountID之前叫用
	///@return	下一个满足条件COrder，如果已经没有一个满足要求了，则返回NULL
	virtual COrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByAccountID 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableOrder * pByAccountID,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///COrderIteratorByPositionID是一个对报单使用ByPositionID方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class COrderIteratorByPositionID: public COrderIterator
{
	friend class COrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(COrder *pOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableOrder m_compareObject;
	
	static CMemoryStack<COrderIteratorByPositionID> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	COrderIteratorByPositionID(COrderFactory *pFactory
		,const CReadOnlyLongIndexIDType& PositionID)
	{
		init(pFactory,PositionID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static COrderIteratorByPositionID *alloc(COrderFactory *pFactory
		,const CReadOnlyLongIndexIDType& PositionID);

	///构造方法
	COrderIteratorByPositionID(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(COrderFactory *pFactory
		,const CReadOnlyLongIndexIDType& PositionID);

	///析构方法，将负责释放所占用的空间
	virtual ~COrderIteratorByPositionID(void);
	
	virtual void free(void);

	///寻找下一个符合条件的COrder，必须在startFindByPositionID以后，endFindByPositionID之前叫用
	///@return	下一个满足条件COrder，如果已经没有一个满足要求了，则返回NULL
	virtual COrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByPositionID 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableOrder * pByPositionID,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///COrderIteratorByAccountIDPrivate是一个对报单使用ByAccountIDPrivate方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class COrderIteratorByAccountIDPrivate: public COrderIterator
{
	friend class COrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(COrder *pOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableOrder m_compareObject;
	
	static CMemoryStack<COrderIteratorByAccountIDPrivate> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	COrderIteratorByAccountIDPrivate(COrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID)
	{
		init(pFactory,AccountID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static COrderIteratorByAccountIDPrivate *alloc(COrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID);

	///构造方法
	COrderIteratorByAccountIDPrivate(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(COrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID);

	///析构方法，将负责释放所占用的空间
	virtual ~COrderIteratorByAccountIDPrivate(void);
	
	virtual void free(void);

	///寻找下一个符合条件的COrder，必须在startFindByAccountIDPrivate以后，endFindByAccountIDPrivate之前叫用
	///@return	下一个满足条件COrder，如果已经没有一个满足要求了，则返回NULL
	virtual COrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByAccountIDPrivate 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableOrder * pByAccountIDPrivate,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///COrderIteratorByInstrumentID是一个对报单使用ByInstrumentID方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class COrderIteratorByInstrumentID: public COrderIterator
{
	friend class COrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(COrder *pOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableOrder m_compareObject;
	
	static CMemoryStack<COrderIteratorByInstrumentID> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	COrderIteratorByInstrumentID(COrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID,const CReadOnlyInstrumentIDType& InstrumentID)
	{
		init(pFactory,AccountID,InstrumentID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static COrderIteratorByInstrumentID *alloc(COrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID,const CReadOnlyInstrumentIDType& InstrumentID);

	///构造方法
	COrderIteratorByInstrumentID(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(COrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID,const CReadOnlyInstrumentIDType& InstrumentID);

	///析构方法，将负责释放所占用的空间
	virtual ~COrderIteratorByInstrumentID(void);
	
	virtual void free(void);

	///寻找下一个符合条件的COrder，必须在startFindByInstrumentID以后，endFindByInstrumentID之前叫用
	///@return	下一个满足条件COrder，如果已经没有一个满足要求了，则返回NULL
	virtual COrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByInstrumentID 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableOrder * pByInstrumentID,CTransaction *pTransaction = NULL, bool updateIndex=true);
};

/////////////////////////////////////////////////////////////////////////
///CPositionOrderIteratorByInsertTime是一个对未平仓报单使用ByInsertTime方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionOrderIteratorByInsertTime: public CPositionOrderIterator
{
	friend class CPositionOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pPositionOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CPositionOrder *pPositionOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteablePositionOrder m_compareObject;
	
	static CMemoryStack<CPositionOrderIteratorByInsertTime> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CPositionOrderIteratorByInsertTime(CPositionOrderFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID,const CReadOnlyIndexIDType& TradeUnitID,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPosiDirectionType& PosiDirection)
	{
		init(pFactory,MemberID,TradeUnitID,ExchangeID,InstrumentID,PosiDirection);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CPositionOrderIteratorByInsertTime *alloc(CPositionOrderFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID,const CReadOnlyIndexIDType& TradeUnitID,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPosiDirectionType& PosiDirection);

	///构造方法
	CPositionOrderIteratorByInsertTime(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CPositionOrderFactory *pFactory
		,const CReadOnlyLongIndexIDType& MemberID,const CReadOnlyIndexIDType& TradeUnitID,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPosiDirectionType& PosiDirection);

	///析构方法，将负责释放所占用的空间
	virtual ~CPositionOrderIteratorByInsertTime(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CPositionOrder，必须在startFindByInsertTime以后，endFindByInsertTime之前叫用
	///@return	下一个满足条件CPositionOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CPositionOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByInsertTime 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteablePositionOrder * pByInsertTime,CTransaction *pTransaction = NULL, bool updateIndex=true);
};

/////////////////////////////////////////////////////////////////////////
///CTriggerOrderIteratorByBuySLTriggerPrice是一个对报单使用ByBuySLTriggerPrice方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderIteratorByBuySLTriggerPrice: public CTriggerOrderIterator
{
	friend class CTriggerOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pTriggerOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CTriggerOrder *pTriggerOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableTriggerOrder m_compareObject;
	
	static CMemoryStack<CTriggerOrderIteratorByBuySLTriggerPrice> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CTriggerOrderIteratorByBuySLTriggerPrice(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& SLTriggerPrice)
	{
		init(pFactory,ExchangeID,InstrumentID,SLTriggerPrice);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CTriggerOrderIteratorByBuySLTriggerPrice *alloc(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& SLTriggerPrice);

	///构造方法
	CTriggerOrderIteratorByBuySLTriggerPrice(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& SLTriggerPrice);

	///析构方法，将负责释放所占用的空间
	virtual ~CTriggerOrderIteratorByBuySLTriggerPrice(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CTriggerOrder，必须在startFindByBuySLTriggerPrice以后，endFindByBuySLTriggerPrice之前叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CTriggerOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByBuySLTriggerPrice 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTriggerOrder * pByBuySLTriggerPrice,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CTriggerOrderIteratorBySellSLTriggerPrice是一个对报单使用BySellSLTriggerPrice方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderIteratorBySellSLTriggerPrice: public CTriggerOrderIterator
{
	friend class CTriggerOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pTriggerOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CTriggerOrder *pTriggerOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableTriggerOrder m_compareObject;
	
	static CMemoryStack<CTriggerOrderIteratorBySellSLTriggerPrice> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CTriggerOrderIteratorBySellSLTriggerPrice(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& SLTriggerPrice)
	{
		init(pFactory,ExchangeID,InstrumentID,SLTriggerPrice);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CTriggerOrderIteratorBySellSLTriggerPrice *alloc(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& SLTriggerPrice);

	///构造方法
	CTriggerOrderIteratorBySellSLTriggerPrice(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& SLTriggerPrice);

	///析构方法，将负责释放所占用的空间
	virtual ~CTriggerOrderIteratorBySellSLTriggerPrice(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CTriggerOrder，必须在startFindBySellSLTriggerPrice以后，endFindBySellSLTriggerPrice之前叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CTriggerOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pBySellSLTriggerPrice 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTriggerOrder * pBySellSLTriggerPrice,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CTriggerOrderIteratorByBuyTPTriggerPrice是一个对报单使用ByBuyTPTriggerPrice方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderIteratorByBuyTPTriggerPrice: public CTriggerOrderIterator
{
	friend class CTriggerOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pTriggerOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CTriggerOrder *pTriggerOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableTriggerOrder m_compareObject;
	
	static CMemoryStack<CTriggerOrderIteratorByBuyTPTriggerPrice> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CTriggerOrderIteratorByBuyTPTriggerPrice(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& TPTriggerPrice)
	{
		init(pFactory,ExchangeID,InstrumentID,TPTriggerPrice);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CTriggerOrderIteratorByBuyTPTriggerPrice *alloc(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& TPTriggerPrice);

	///构造方法
	CTriggerOrderIteratorByBuyTPTriggerPrice(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& TPTriggerPrice);

	///析构方法，将负责释放所占用的空间
	virtual ~CTriggerOrderIteratorByBuyTPTriggerPrice(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CTriggerOrder，必须在startFindByBuyTPTriggerPrice以后，endFindByBuyTPTriggerPrice之前叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CTriggerOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByBuyTPTriggerPrice 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTriggerOrder * pByBuyTPTriggerPrice,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CTriggerOrderIteratorBySellTPTriggerPrice是一个对报单使用BySellTPTriggerPrice方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderIteratorBySellTPTriggerPrice: public CTriggerOrderIterator
{
	friend class CTriggerOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pTriggerOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CTriggerOrder *pTriggerOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableTriggerOrder m_compareObject;
	
	static CMemoryStack<CTriggerOrderIteratorBySellTPTriggerPrice> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CTriggerOrderIteratorBySellTPTriggerPrice(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& TPTriggerPrice)
	{
		init(pFactory,ExchangeID,InstrumentID,TPTriggerPrice);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CTriggerOrderIteratorBySellTPTriggerPrice *alloc(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& TPTriggerPrice);

	///构造方法
	CTriggerOrderIteratorBySellTPTriggerPrice(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CTriggerOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& TPTriggerPrice);

	///析构方法，将负责释放所占用的空间
	virtual ~CTriggerOrderIteratorBySellTPTriggerPrice(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CTriggerOrder，必须在startFindBySellTPTriggerPrice以后，endFindBySellTPTriggerPrice之前叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CTriggerOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pBySellTPTriggerPrice 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTriggerOrder * pBySellTPTriggerPrice,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CTriggerOrderIteratorByAccountID是一个对报单使用ByAccountID方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderIteratorByAccountID: public CTriggerOrderIterator
{
	friend class CTriggerOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pTriggerOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CTriggerOrder *pTriggerOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableTriggerOrder m_compareObject;
	
	static CMemoryStack<CTriggerOrderIteratorByAccountID> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CTriggerOrderIteratorByAccountID(CTriggerOrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID)
	{
		init(pFactory,AccountID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CTriggerOrderIteratorByAccountID *alloc(CTriggerOrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID);

	///构造方法
	CTriggerOrderIteratorByAccountID(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CTriggerOrderFactory *pFactory
		,const CReadOnlyAccountIDType& AccountID);

	///析构方法，将负责释放所占用的空间
	virtual ~CTriggerOrderIteratorByAccountID(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CTriggerOrder，必须在startFindByAccountID以后，endFindByAccountID之前叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CTriggerOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByAccountID 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTriggerOrder * pByAccountID,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CTriggerOrderIteratorByActivePositionClose是一个对报单使用ByActivePositionClose方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderIteratorByActivePositionClose: public CTriggerOrderIterator
{
	friend class CTriggerOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pTriggerOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CTriggerOrder *pTriggerOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableTriggerOrder m_compareObject;
	
	static CMemoryStack<CTriggerOrderIteratorByActivePositionClose> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CTriggerOrderIteratorByActivePositionClose(CTriggerOrderFactory *pFactory
		,const CReadOnlyLongIndexIDType& PositionID)
	{
		init(pFactory,PositionID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CTriggerOrderIteratorByActivePositionClose *alloc(CTriggerOrderFactory *pFactory
		,const CReadOnlyLongIndexIDType& PositionID);

	///构造方法
	CTriggerOrderIteratorByActivePositionClose(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CTriggerOrderFactory *pFactory
		,const CReadOnlyLongIndexIDType& PositionID);

	///析构方法，将负责释放所占用的空间
	virtual ~CTriggerOrderIteratorByActivePositionClose(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CTriggerOrder，必须在startFindByActivePositionClose以后，endFindByActivePositionClose之前叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CTriggerOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByActivePositionClose 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTriggerOrder * pByActivePositionClose,CTransaction *pTransaction = NULL, bool updateIndex=true);
};

/////////////////////////////////////////////////////////////////////////
///CMarketOrderIteratorByBuyMBLData是一个对分价表使用ByBuyMBLData方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketOrderIteratorByBuyMBLData: public CMarketOrderIterator
{
	friend class CMarketOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pMarketOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CMarketOrder *pMarketOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableMarketOrder m_compareObject;
	
	static CMemoryStack<CMarketOrderIteratorByBuyMBLData> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CMarketOrderIteratorByBuyMBLData(CMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID)
	{
		init(pFactory,ExchangeID,InstrumentID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CMarketOrderIteratorByBuyMBLData *alloc(CMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID);

	///构造方法
	CMarketOrderIteratorByBuyMBLData(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID);

	///析构方法，将负责释放所占用的空间
	virtual ~CMarketOrderIteratorByBuyMBLData(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CMarketOrder，必须在startFindByBuyMBLData以后，endFindByBuyMBLData之前叫用
	///@return	下一个满足条件CMarketOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CMarketOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByBuyMBLData 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableMarketOrder * pByBuyMBLData,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CMarketOrderIteratorBySellMBLData是一个对分价表使用BySellMBLData方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketOrderIteratorBySellMBLData: public CMarketOrderIterator
{
	friend class CMarketOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pMarketOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CMarketOrder *pMarketOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableMarketOrder m_compareObject;
	
	static CMemoryStack<CMarketOrderIteratorBySellMBLData> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CMarketOrderIteratorBySellMBLData(CMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID)
	{
		init(pFactory,ExchangeID,InstrumentID);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CMarketOrderIteratorBySellMBLData *alloc(CMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID);

	///构造方法
	CMarketOrderIteratorBySellMBLData(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID);

	///析构方法，将负责释放所占用的空间
	virtual ~CMarketOrderIteratorBySellMBLData(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CMarketOrder，必须在startFindBySellMBLData以后，endFindBySellMBLData之前叫用
	///@return	下一个满足条件CMarketOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CMarketOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pBySellMBLData 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableMarketOrder * pBySellMBLData,CTransaction *pTransaction = NULL, bool updateIndex=true);
};

/////////////////////////////////////////////////////////////////////////
///CTickMarketOrderIteratorByBuyMBLData是一个对Tick聚集分价表使用ByBuyMBLData方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTickMarketOrderIteratorByBuyMBLData: public CTickMarketOrderIterator
{
	friend class CTickMarketOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pTickMarketOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CTickMarketOrder *pTickMarketOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableTickMarketOrder m_compareObject;
	
	static CMemoryStack<CTickMarketOrderIteratorByBuyMBLData> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CTickMarketOrderIteratorByBuyMBLData(CTickMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& Tick)
	{
		init(pFactory,ExchangeID,InstrumentID,Tick);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CTickMarketOrderIteratorByBuyMBLData *alloc(CTickMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& Tick);

	///构造方法
	CTickMarketOrderIteratorByBuyMBLData(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CTickMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& Tick);

	///析构方法，将负责释放所占用的空间
	virtual ~CTickMarketOrderIteratorByBuyMBLData(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CTickMarketOrder，必须在startFindByBuyMBLData以后，endFindByBuyMBLData之前叫用
	///@return	下一个满足条件CTickMarketOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CTickMarketOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByBuyMBLData 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTickMarketOrder * pByBuyMBLData,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CTickMarketOrderIteratorBySellMBLData是一个对Tick聚集分价表使用BySellMBLData方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTickMarketOrderIteratorBySellMBLData: public CTickMarketOrderIterator
{
	friend class CTickMarketOrderFactory;
private:
	///检查此对象是否满足条件
	///@param	pTickMarketOrder		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CTickMarketOrder *pTickMarketOrder);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteableTickMarketOrder m_compareObject;
	
	static CMemoryStack<CTickMarketOrderIteratorBySellMBLData> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CTickMarketOrderIteratorBySellMBLData(CTickMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& Tick)
	{
		init(pFactory,ExchangeID,InstrumentID,Tick);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CTickMarketOrderIteratorBySellMBLData *alloc(CTickMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& Tick);

	///构造方法
	CTickMarketOrderIteratorBySellMBLData(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CTickMarketOrderFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyPriceType& Tick);

	///析构方法，将负责释放所占用的空间
	virtual ~CTickMarketOrderIteratorBySellMBLData(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CTickMarketOrder，必须在startFindBySellMBLData以后，endFindBySellMBLData之前叫用
	///@return	下一个满足条件CTickMarketOrder，如果已经没有一个满足要求了，则返回NULL
	virtual CTickMarketOrder *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pBySellMBLData 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteableTickMarketOrder * pBySellMBLData,CTransaction *pTransaction = NULL, bool updateIndex=true);
};



/////////////////////////////////////////////////////////////////////////
///CPublishKLineIteratorByBar是一个对PublishK线使用ByBar方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPublishKLineIteratorByBar: public CPublishKLineIterator
{
	friend class CPublishKLineFactory;
private:
	///检查此对象是否满足条件
	///@param	pPublishKLine		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CPublishKLine *pPublishKLine);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteablePublishKLine m_compareObject;
	
	static CMemoryStack<CPublishKLineIteratorByBar> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CPublishKLineIteratorByBar(CPublishKLineFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyBarType& Bar)
	{
		init(pFactory,ExchangeID,InstrumentID,Bar);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CPublishKLineIteratorByBar *alloc(CPublishKLineFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyBarType& Bar);

	///构造方法
	CPublishKLineIteratorByBar(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CPublishKLineFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyBarType& Bar);

	///析构方法，将负责释放所占用的空间
	virtual ~CPublishKLineIteratorByBar(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CPublishKLine，必须在startFindByBar以后，endFindByBar之前叫用
	///@return	下一个满足条件CPublishKLine，如果已经没有一个满足要求了，则返回NULL
	virtual CPublishKLine *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByBar 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteablePublishKLine * pByBar,CTransaction *pTransaction = NULL, bool updateIndex=true);
};
/////////////////////////////////////////////////////////////////////////
///CPublishKLineIteratorByBarInsertTime是一个对PublishK线使用ByBarInsertTime方法进行
///查找的枚举器
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPublishKLineIteratorByBarInsertTime: public CPublishKLineIterator
{
	friend class CPublishKLineFactory;
private:
	///检查此对象是否满足条件
	///@param	pPublishKLine		要被检查的对象
	///@return	1表示满足条件，0表示不满足条件
	int match(CPublishKLine *pPublishKLine);

	///在需要的情况下，获取下一个值，如果不是第一次，而且还没有取过下一个值，
	///那么就必须保证叫用时m_pCurrent指向正确的值，否则本方法的结果不确定
	void internalGetNext(void);

	///是否是第一次查询
	int m_isFirstTime;
	
	///是否已经取了下一个对象
	bool m_gotNext;

	///存储最后一次找到的索引节点
	///当m_isFirstTime为1时，或者发生了DeleteCurrent时，本值无意义
	CAVLNode *m_pCurrent;

	///当m_gotNext为true时，本值下一次next时的开始寻找的节点对应的索引节点，其他情况下无意义
	CAVLNode *m_pNext;

	///用于存储查询参数的对象
	CWriteablePublishKLine m_compareObject;
	
	static CMemoryStack<CPublishKLineIteratorByBarInsertTime> iteratorList;
public:
	///构造方法
	///@param	pFactory	要查找的对象工厂
	CPublishKLineIteratorByBarInsertTime(CPublishKLineFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyBarType& Bar,const CReadOnlyMilliSecondsType& InsertTime)
	{
		init(pFactory,ExchangeID,InstrumentID,Bar,InsertTime);
	}

	///申请一个新的对象
	///@param	pFactory	要查找的对象工厂
	static CPublishKLineIteratorByBarInsertTime *alloc(CPublishKLineFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyBarType& Bar,const CReadOnlyMilliSecondsType& InsertTime);

	///构造方法
	CPublishKLineIteratorByBarInsertTime(void)
	{
	}

	///初始化
	///@param	pFactory	要查找的对象工厂
	void init(CPublishKLineFactory *pFactory
		,const CReadOnlyExchangeIDType& ExchangeID,const CReadOnlyInstrumentIDType& InstrumentID,const CReadOnlyBarType& Bar,const CReadOnlyMilliSecondsType& InsertTime);

	///析构方法，将负责释放所占用的空间
	virtual ~CPublishKLineIteratorByBarInsertTime(void);
	
	virtual void free(void);

	///寻找下一个符合条件的CPublishKLine，必须在startFindByBarInsertTime以后，endFindByBarInsertTime之前叫用
	///@return	下一个满足条件CPublishKLine，如果已经没有一个满足要求了，则返回NULL
	virtual CPublishKLine *next(void);
	
	///删除Iterator当前指向的纪录
	///@param	pTransaction	此次删除属于的事务，NULL表示不属于任何事务，立即执行
	virtual void DeleteCurrent(CTransaction *pTransaction = NULL);
	
	///更新当前Iterator指向的纪录
	///@param	pByBarInsertTime 需要更新成的新纪录内容
	///@param	pTransaction	此次更新属于的事务，NULL表示不属于任何事务，立即执行
	///@param	updateIndex	true表示需要更新索引，false表示不需要更新索引
	virtual void UpdateCurrent(CWriteablePublishKLine * pByBarInsertTime,CTransaction *pTransaction = NULL, bool updateIndex=true);
};




#endif
