/////////////////////////////////////////////////////////////////////////
///实现了内存数据库的iterator
/////////////////////////////////////////////////////////////////////////

#include "mdbIter.h"
#include "monitorIndex.h"

//#define COUNT_SEARCH

CMemoryStack<CServiceConfigIteratorByIndex1> CServiceConfigIteratorByIndex1::iteratorList;
void CServiceConfigIteratorByIndex1::init(CServiceConfigFactory *pFactory, const CReadOnlyNameType& ConfigName, const CReadOnlyNameType& Index1)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ConfigName=ConfigName.getValue();
	m_compareObject.Index1=Index1.getValue();
	m_compareObject.Index2="";
}

CServiceConfigIteratorByIndex1 *CServiceConfigIteratorByIndex1::alloc(CServiceConfigFactory *pFactory, const CReadOnlyNameType& ConfigName, const CReadOnlyNameType& Index1)
{
	CServiceConfigIteratorByIndex1 *it;
	it=iteratorList.alloc();
	it->init(pFactory,ConfigName,Index1);
	return it;
}

CServiceConfigIteratorByIndex1::~CServiceConfigIteratorByIndex1(void)
{
}

void CServiceConfigIteratorByIndex1::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CServiceConfigIteratorByIndex1::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pConfigIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pConfigIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CServiceConfigIteratorByIndex1::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CServiceConfig *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CServiceConfigIteratorByIndex1::UpdateCurrent(CWriteableServiceConfig * pByIndex1,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CServiceConfig *)(m_pCurrent->pObject),pByIndex1,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByIndex1ForServiceConfig;
#endif

CServiceConfig *CServiceConfigIteratorByIndex1::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByIndex1ForServiceConfig++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CServiceConfig *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CServiceConfig *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CServiceConfigIteratorByIndex1::match(CServiceConfig *pServiceConfig)
{
	if (compare(pServiceConfig->ConfigName.getValue(),m_compareObject.ConfigName.getValue())!=0)
		return 0;
	if (compare(pServiceConfig->Index1.getValue(),m_compareObject.Index1.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CInstrumentIteratorBySettlementGroup> CInstrumentIteratorBySettlementGroup::iteratorList;
void CInstrumentIteratorBySettlementGroup::init(CInstrumentFactory *pFactory, const CReadOnlyShortIndexIDType& SettlementGroup)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.SettlementGroup=SettlementGroup.getValue();
}

CInstrumentIteratorBySettlementGroup *CInstrumentIteratorBySettlementGroup::alloc(CInstrumentFactory *pFactory, const CReadOnlyShortIndexIDType& SettlementGroup)
{
	CInstrumentIteratorBySettlementGroup *it;
	it=iteratorList.alloc();
	it->init(pFactory,SettlementGroup);
	return it;
}

CInstrumentIteratorBySettlementGroup::~CInstrumentIteratorBySettlementGroup(void)
{
}

void CInstrumentIteratorBySettlementGroup::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CInstrumentIteratorBySettlementGroup::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pSettlementGroupIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pSettlementGroupIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CInstrumentIteratorBySettlementGroup::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CInstrument *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CInstrumentIteratorBySettlementGroup::UpdateCurrent(CWriteableInstrument * pBySettlementGroup,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CInstrument *)(m_pCurrent->pObject),pBySettlementGroup,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindBySettlementGroupForInstrument;
#endif

CInstrument *CInstrumentIteratorBySettlementGroup::next(void)
{
#ifdef COUNT_SEARCH
	multiFindBySettlementGroupForInstrument++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CInstrument *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CInstrument *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CInstrumentIteratorBySettlementGroup::match(CInstrument *pInstrument)
{
	if (compare(pInstrument->SettlementGroup.getValue(),m_compareObject.SettlementGroup.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CInstrumentIteratorByProductGroup> CInstrumentIteratorByProductGroup::iteratorList;
void CInstrumentIteratorByProductGroup::init(CInstrumentFactory *pFactory, const CReadOnlyShortIndexIDType& ProductGroup)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ProductGroup=ProductGroup.getValue();
}

CInstrumentIteratorByProductGroup *CInstrumentIteratorByProductGroup::alloc(CInstrumentFactory *pFactory, const CReadOnlyShortIndexIDType& ProductGroup)
{
	CInstrumentIteratorByProductGroup *it;
	it=iteratorList.alloc();
	it->init(pFactory,ProductGroup);
	return it;
}

CInstrumentIteratorByProductGroup::~CInstrumentIteratorByProductGroup(void)
{
}

void CInstrumentIteratorByProductGroup::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CInstrumentIteratorByProductGroup::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pProductGroupIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pProductGroupIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CInstrumentIteratorByProductGroup::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CInstrument *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CInstrumentIteratorByProductGroup::UpdateCurrent(CWriteableInstrument * pByProductGroup,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CInstrument *)(m_pCurrent->pObject),pByProductGroup,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByProductGroupForInstrument;
#endif

CInstrument *CInstrumentIteratorByProductGroup::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByProductGroupForInstrument++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CInstrument *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CInstrument *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CInstrumentIteratorByProductGroup::match(CInstrument *pInstrument)
{
	if (compare(pInstrument->ProductGroup.getValue(),m_compareObject.ProductGroup.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CCurrencyIteratorBySettlementGroup> CCurrencyIteratorBySettlementGroup::iteratorList;
void CCurrencyIteratorBySettlementGroup::init(CCurrencyFactory *pFactory, const CReadOnlyShortIndexIDType& SettlementGroup)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.SettlementGroup=SettlementGroup.getValue();
}

CCurrencyIteratorBySettlementGroup *CCurrencyIteratorBySettlementGroup::alloc(CCurrencyFactory *pFactory, const CReadOnlyShortIndexIDType& SettlementGroup)
{
	CCurrencyIteratorBySettlementGroup *it;
	it=iteratorList.alloc();
	it->init(pFactory,SettlementGroup);
	return it;
}

CCurrencyIteratorBySettlementGroup::~CCurrencyIteratorBySettlementGroup(void)
{
}

void CCurrencyIteratorBySettlementGroup::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CCurrencyIteratorBySettlementGroup::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pSettlementGroupIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pSettlementGroupIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CCurrencyIteratorBySettlementGroup::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CCurrency *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CCurrencyIteratorBySettlementGroup::UpdateCurrent(CWriteableCurrency * pBySettlementGroup,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CCurrency *)(m_pCurrent->pObject),pBySettlementGroup,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindBySettlementGroupForCurrency;
#endif

CCurrency *CCurrencyIteratorBySettlementGroup::next(void)
{
#ifdef COUNT_SEARCH
	multiFindBySettlementGroupForCurrency++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CCurrency *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CCurrency *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CCurrencyIteratorBySettlementGroup::match(CCurrency *pCurrency)
{
	if (compare(pCurrency->SettlementGroup.getValue(),m_compareObject.SettlementGroup.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CAccountIteratorByMemberID> CAccountIteratorByMemberID::iteratorList;
void CAccountIteratorByMemberID::init(CAccountFactory *pFactory, const CReadOnlyLongIndexIDType& MemberID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.MemberID=MemberID.getValue();
	m_compareObject.AccountID="";
	m_compareObject.SettlementGroup="";
	m_compareObject.Currency="";
}

CAccountIteratorByMemberID *CAccountIteratorByMemberID::alloc(CAccountFactory *pFactory, const CReadOnlyLongIndexIDType& MemberID)
{
	CAccountIteratorByMemberID *it;
	it=iteratorList.alloc();
	it->init(pFactory,MemberID);
	return it;
}

CAccountIteratorByMemberID::~CAccountIteratorByMemberID(void)
{
}

void CAccountIteratorByMemberID::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CAccountIteratorByMemberID::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pMemberIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pMemberIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CAccountIteratorByMemberID::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CAccount *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CAccountIteratorByMemberID::UpdateCurrent(CWriteableAccount * pByMemberID,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CAccount *)(m_pCurrent->pObject),pByMemberID,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByMemberIDForAccount;
#endif

CAccount *CAccountIteratorByMemberID::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByMemberIDForAccount++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CAccount *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CAccount *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CAccountIteratorByMemberID::match(CAccount *pAccount)
{
	if (compare(pAccount->MemberID.getValue(),m_compareObject.MemberID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CAccountIteratorByAccountID> CAccountIteratorByAccountID::iteratorList;
void CAccountIteratorByAccountID::init(CAccountFactory *pFactory, const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.MemberID=MemberID.getValue();
	m_compareObject.AccountID=AccountID.getValue();
	m_compareObject.SettlementGroup="";
	m_compareObject.Currency="";
}

CAccountIteratorByAccountID *CAccountIteratorByAccountID::alloc(CAccountFactory *pFactory, const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID)
{
	CAccountIteratorByAccountID *it;
	it=iteratorList.alloc();
	it->init(pFactory,MemberID,AccountID);
	return it;
}

CAccountIteratorByAccountID::~CAccountIteratorByAccountID(void)
{
}

void CAccountIteratorByAccountID::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CAccountIteratorByAccountID::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pMemberIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pMemberIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CAccountIteratorByAccountID::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CAccount *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CAccountIteratorByAccountID::UpdateCurrent(CWriteableAccount * pByAccountID,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CAccount *)(m_pCurrent->pObject),pByAccountID,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByAccountIDForAccount;
#endif

CAccount *CAccountIteratorByAccountID::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByAccountIDForAccount++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CAccount *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CAccount *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CAccountIteratorByAccountID::match(CAccount *pAccount)
{
	if (compare(pAccount->MemberID.getValue(),m_compareObject.MemberID.getValue())!=0)
		return 0;
	if (compare(pAccount->AccountID.getValue(),m_compareObject.AccountID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CAccountIteratorBySettlementGroup> CAccountIteratorBySettlementGroup::iteratorList;
void CAccountIteratorBySettlementGroup::init(CAccountFactory *pFactory, const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID, const CReadOnlyShortIndexIDType& SettlementGroup)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.MemberID=MemberID.getValue();
	m_compareObject.AccountID=AccountID.getValue();
	m_compareObject.SettlementGroup=SettlementGroup.getValue();
	m_compareObject.Currency="";
}

CAccountIteratorBySettlementGroup *CAccountIteratorBySettlementGroup::alloc(CAccountFactory *pFactory, const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID, const CReadOnlyShortIndexIDType& SettlementGroup)
{
	CAccountIteratorBySettlementGroup *it;
	it=iteratorList.alloc();
	it->init(pFactory,MemberID,AccountID,SettlementGroup);
	return it;
}

CAccountIteratorBySettlementGroup::~CAccountIteratorBySettlementGroup(void)
{
}

void CAccountIteratorBySettlementGroup::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CAccountIteratorBySettlementGroup::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pMemberIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pMemberIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CAccountIteratorBySettlementGroup::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CAccount *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CAccountIteratorBySettlementGroup::UpdateCurrent(CWriteableAccount * pBySettlementGroup,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CAccount *)(m_pCurrent->pObject),pBySettlementGroup,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindBySettlementGroupForAccount;
#endif

CAccount *CAccountIteratorBySettlementGroup::next(void)
{
#ifdef COUNT_SEARCH
	multiFindBySettlementGroupForAccount++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CAccount *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CAccount *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CAccountIteratorBySettlementGroup::match(CAccount *pAccount)
{
	if (compare(pAccount->MemberID.getValue(),m_compareObject.MemberID.getValue())!=0)
		return 0;
	if (compare(pAccount->AccountID.getValue(),m_compareObject.AccountID.getValue())!=0)
		return 0;
	if (compare(pAccount->SettlementGroup.getValue(),m_compareObject.SettlementGroup.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CAccountIteratorByAll> CAccountIteratorByAll::iteratorList;
void CAccountIteratorByAll::init(CAccountFactory *pFactory)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.MemberID="";
	m_compareObject.AccountID="";
	m_compareObject.SettlementGroup="";
	m_compareObject.Currency="";
}

CAccountIteratorByAll *CAccountIteratorByAll::alloc(CAccountFactory *pFactory)
{
	CAccountIteratorByAll *it;
	it=iteratorList.alloc();
	it->init(pFactory);
	return it;
}

CAccountIteratorByAll::~CAccountIteratorByAll(void)
{
}

void CAccountIteratorByAll::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CAccountIteratorByAll::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pMemberIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pMemberIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CAccountIteratorByAll::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CAccount *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CAccountIteratorByAll::UpdateCurrent(CWriteableAccount * pByAll,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CAccount *)(m_pCurrent->pObject),pByAll,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByAllForAccount;
#endif

CAccount *CAccountIteratorByAll::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByAllForAccount++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CAccount *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CAccount *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CAccountIteratorByAll::match(CAccount *pAccount)
{
	return 1;
}

CMemoryStack<CAccountIteratorByCurrency> CAccountIteratorByCurrency::iteratorList;
void CAccountIteratorByCurrency::init(CAccountFactory *pFactory, const CReadOnlyShortIndexIDType& SettlementGroup, const CReadOnlyCurrencyType& Currency)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.SettlementGroup=SettlementGroup.getValue();
	m_compareObject.Currency=Currency.getValue();
	m_compareObject.Balance=999999999999.0;
}

CAccountIteratorByCurrency *CAccountIteratorByCurrency::alloc(CAccountFactory *pFactory, const CReadOnlyShortIndexIDType& SettlementGroup, const CReadOnlyCurrencyType& Currency)
{
	CAccountIteratorByCurrency *it;
	it=iteratorList.alloc();
	it->init(pFactory,SettlementGroup,Currency);
	return it;
}

CAccountIteratorByCurrency::~CAccountIteratorByCurrency(void)
{
}

void CAccountIteratorByCurrency::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CAccountIteratorByCurrency::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pCurrencyIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pCurrencyIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CAccountIteratorByCurrency::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CAccount *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CAccountIteratorByCurrency::UpdateCurrent(CWriteableAccount * pByCurrency,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CAccount *)(m_pCurrent->pObject),pByCurrency,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByCurrencyForAccount;
#endif

CAccount *CAccountIteratorByCurrency::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByCurrencyForAccount++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CAccount *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CAccount *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CAccountIteratorByCurrency::match(CAccount *pAccount)
{
	if (compare(pAccount->SettlementGroup.getValue(),m_compareObject.SettlementGroup.getValue())!=0)
		return 0;
	if (compare(pAccount->Currency.getValue(),m_compareObject.Currency.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CPositionIteratorByAccountInstrument> CPositionIteratorByAccountInstrument::iteratorList;
void CPositionIteratorByAccountInstrument::init(CPositionFactory *pFactory, const CReadOnlyAccountIDType& AccountID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.AccountID=AccountID.getValue();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.TradeUnitID="";
	m_compareObject.PosiDirection="";
}

CPositionIteratorByAccountInstrument *CPositionIteratorByAccountInstrument::alloc(CPositionFactory *pFactory, const CReadOnlyAccountIDType& AccountID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	CPositionIteratorByAccountInstrument *it;
	it=iteratorList.alloc();
	it->init(pFactory,AccountID,ExchangeID,InstrumentID);
	return it;
}

CPositionIteratorByAccountInstrument::~CPositionIteratorByAccountInstrument(void)
{
}

void CPositionIteratorByAccountInstrument::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CPositionIteratorByAccountInstrument::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pAccountIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pAccountIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CPositionIteratorByAccountInstrument::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CPosition *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CPositionIteratorByAccountInstrument::UpdateCurrent(CWriteablePosition * pByAccountInstrument,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CPosition *)(m_pCurrent->pObject),pByAccountInstrument,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByAccountInstrumentForPosition;
#endif

CPosition *CPositionIteratorByAccountInstrument::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByAccountInstrumentForPosition++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CPosition *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CPosition *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CPositionIteratorByAccountInstrument::match(CPosition *pPosition)
{
	if (compare(pPosition->AccountID.getValue(),m_compareObject.AccountID.getValue())!=0)
		return 0;
	if (compare(pPosition->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pPosition->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CPositionIteratorByAccountID> CPositionIteratorByAccountID::iteratorList;
void CPositionIteratorByAccountID::init(CPositionFactory *pFactory, const CReadOnlyAccountIDType& AccountID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.AccountID=AccountID.getValue();
	m_compareObject.ExchangeID="";
	m_compareObject.InstrumentID="";
	m_compareObject.TradeUnitID="";
	m_compareObject.PosiDirection="";
}

CPositionIteratorByAccountID *CPositionIteratorByAccountID::alloc(CPositionFactory *pFactory, const CReadOnlyAccountIDType& AccountID)
{
	CPositionIteratorByAccountID *it;
	it=iteratorList.alloc();
	it->init(pFactory,AccountID);
	return it;
}

CPositionIteratorByAccountID::~CPositionIteratorByAccountID(void)
{
}

void CPositionIteratorByAccountID::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CPositionIteratorByAccountID::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pAccountIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pAccountIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CPositionIteratorByAccountID::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CPosition *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CPositionIteratorByAccountID::UpdateCurrent(CWriteablePosition * pByAccountID,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CPosition *)(m_pCurrent->pObject),pByAccountID,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByAccountIDForPosition;
#endif

CPosition *CPositionIteratorByAccountID::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByAccountIDForPosition++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CPosition *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CPosition *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CPositionIteratorByAccountID::match(CPosition *pPosition)
{
	if (compare(pPosition->AccountID.getValue(),m_compareObject.AccountID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CPositionIteratorByLiquidPriceLong> CPositionIteratorByLiquidPriceLong::iteratorList;
void CPositionIteratorByLiquidPriceLong::init(CPositionFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& LiquidPrice)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.LiquidPrice=LiquidPrice.getValue();
}

CPositionIteratorByLiquidPriceLong *CPositionIteratorByLiquidPriceLong::alloc(CPositionFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& LiquidPrice)
{
	CPositionIteratorByLiquidPriceLong *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID,LiquidPrice);
	return it;
}

CPositionIteratorByLiquidPriceLong::~CPositionIteratorByLiquidPriceLong(void)
{
}

void CPositionIteratorByLiquidPriceLong::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CPositionIteratorByLiquidPriceLong::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pLiquidPriceLongIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pLiquidPriceLongIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CPositionIteratorByLiquidPriceLong::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CPosition *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CPositionIteratorByLiquidPriceLong::UpdateCurrent(CWriteablePosition * pByLiquidPriceLong,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CPosition *)(m_pCurrent->pObject),pByLiquidPriceLong,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByLiquidPriceLongForPosition;
#endif

CPosition *CPositionIteratorByLiquidPriceLong::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByLiquidPriceLongForPosition++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CPosition *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CPosition *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CPositionIteratorByLiquidPriceLong::match(CPosition *pPosition)
{
	if (compare(pPosition->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pPosition->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CPositionIteratorByLiquidPriceShort> CPositionIteratorByLiquidPriceShort::iteratorList;
void CPositionIteratorByLiquidPriceShort::init(CPositionFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& LiquidPrice)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.LiquidPrice=LiquidPrice.getValue();
}

CPositionIteratorByLiquidPriceShort *CPositionIteratorByLiquidPriceShort::alloc(CPositionFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& LiquidPrice)
{
	CPositionIteratorByLiquidPriceShort *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID,LiquidPrice);
	return it;
}

CPositionIteratorByLiquidPriceShort::~CPositionIteratorByLiquidPriceShort(void)
{
}

void CPositionIteratorByLiquidPriceShort::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CPositionIteratorByLiquidPriceShort::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pLiquidPriceShortIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pLiquidPriceShortIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CPositionIteratorByLiquidPriceShort::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CPosition *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CPositionIteratorByLiquidPriceShort::UpdateCurrent(CWriteablePosition * pByLiquidPriceShort,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CPosition *)(m_pCurrent->pObject),pByLiquidPriceShort,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByLiquidPriceShortForPosition;
#endif

CPosition *CPositionIteratorByLiquidPriceShort::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByLiquidPriceShortForPosition++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CPosition *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CPosition *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CPositionIteratorByLiquidPriceShort::match(CPosition *pPosition)
{
	if (compare(pPosition->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pPosition->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CMarketDataIteratorByProductGroup> CMarketDataIteratorByProductGroup::iteratorList;
void CMarketDataIteratorByProductGroup::init(CMarketDataFactory *pFactory, const CReadOnlyShortIndexIDType& ProductGroup)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ProductGroup=ProductGroup.getValue();
}

CMarketDataIteratorByProductGroup *CMarketDataIteratorByProductGroup::alloc(CMarketDataFactory *pFactory, const CReadOnlyShortIndexIDType& ProductGroup)
{
	CMarketDataIteratorByProductGroup *it;
	it=iteratorList.alloc();
	it->init(pFactory,ProductGroup);
	return it;
}

CMarketDataIteratorByProductGroup::~CMarketDataIteratorByProductGroup(void)
{
}

void CMarketDataIteratorByProductGroup::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CMarketDataIteratorByProductGroup::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pProductGroupIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pProductGroupIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CMarketDataIteratorByProductGroup::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CMarketData *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CMarketDataIteratorByProductGroup::UpdateCurrent(CWriteableMarketData * pByProductGroup,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CMarketData *)(m_pCurrent->pObject),pByProductGroup,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByProductGroupForMarketData;
#endif

CMarketData *CMarketDataIteratorByProductGroup::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByProductGroupForMarketData++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CMarketData *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CMarketData *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CMarketDataIteratorByProductGroup::match(CMarketData *pMarketData)
{
	if (compare(pMarketData->ProductGroup.getValue(),m_compareObject.ProductGroup.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<COrderIteratorByAccountID> COrderIteratorByAccountID::iteratorList;
void COrderIteratorByAccountID::init(COrderFactory *pFactory, const CReadOnlyAccountIDType& AccountID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.AccountID=AccountID.getValue();
	m_compareObject.LocalID="";
}

COrderIteratorByAccountID *COrderIteratorByAccountID::alloc(COrderFactory *pFactory, const CReadOnlyAccountIDType& AccountID)
{
	COrderIteratorByAccountID *it;
	it=iteratorList.alloc();
	it->init(pFactory,AccountID);
	return it;
}

COrderIteratorByAccountID::~COrderIteratorByAccountID(void)
{
}

void COrderIteratorByAccountID::free(void)
{
	iteratorList.free(this);
	delete this;
}

void COrderIteratorByAccountID::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pLocalIDIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pLocalIDIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void COrderIteratorByAccountID::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((COrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void COrderIteratorByAccountID::UpdateCurrent(CWriteableOrder * pByAccountID,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((COrder *)(m_pCurrent->pObject),pByAccountID,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByAccountIDForOrder;
#endif

COrder *COrderIteratorByAccountID::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByAccountIDForOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((COrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (COrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int COrderIteratorByAccountID::match(COrder *pOrder)
{
	if (compare(pOrder->AccountID.getValue(),m_compareObject.AccountID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<COrderIteratorByPositionID> COrderIteratorByPositionID::iteratorList;
void COrderIteratorByPositionID::init(COrderFactory *pFactory, const CReadOnlyLongIndexIDType& PositionID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.PositionID=PositionID.getValue();
}

COrderIteratorByPositionID *COrderIteratorByPositionID::alloc(COrderFactory *pFactory, const CReadOnlyLongIndexIDType& PositionID)
{
	COrderIteratorByPositionID *it;
	it=iteratorList.alloc();
	it->init(pFactory,PositionID);
	return it;
}

COrderIteratorByPositionID::~COrderIteratorByPositionID(void)
{
}

void COrderIteratorByPositionID::free(void)
{
	iteratorList.free(this);
	delete this;
}

void COrderIteratorByPositionID::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pPositionIDIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pPositionIDIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void COrderIteratorByPositionID::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((COrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void COrderIteratorByPositionID::UpdateCurrent(CWriteableOrder * pByPositionID,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((COrder *)(m_pCurrent->pObject),pByPositionID,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByPositionIDForOrder;
#endif

COrder *COrderIteratorByPositionID::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByPositionIDForOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((COrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (COrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int COrderIteratorByPositionID::match(COrder *pOrder)
{
	if (compare(pOrder->PositionID.getValue(),m_compareObject.PositionID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<COrderIteratorByAccountIDPrivate> COrderIteratorByAccountIDPrivate::iteratorList;
void COrderIteratorByAccountIDPrivate::init(COrderFactory *pFactory, const CReadOnlyAccountIDType& AccountID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.AccountID=AccountID.getValue();
	m_compareObject.InstrumentID="";
	m_compareObject.OrderID="";
}

COrderIteratorByAccountIDPrivate *COrderIteratorByAccountIDPrivate::alloc(COrderFactory *pFactory, const CReadOnlyAccountIDType& AccountID)
{
	COrderIteratorByAccountIDPrivate *it;
	it=iteratorList.alloc();
	it->init(pFactory,AccountID);
	return it;
}

COrderIteratorByAccountIDPrivate::~COrderIteratorByAccountIDPrivate(void)
{
}

void COrderIteratorByAccountIDPrivate::free(void)
{
	iteratorList.free(this);
	delete this;
}

void COrderIteratorByAccountIDPrivate::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pInstrumentIDIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pInstrumentIDIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void COrderIteratorByAccountIDPrivate::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((COrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void COrderIteratorByAccountIDPrivate::UpdateCurrent(CWriteableOrder * pByAccountIDPrivate,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((COrder *)(m_pCurrent->pObject),pByAccountIDPrivate,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByAccountIDPrivateForOrder;
#endif

COrder *COrderIteratorByAccountIDPrivate::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByAccountIDPrivateForOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((COrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (COrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int COrderIteratorByAccountIDPrivate::match(COrder *pOrder)
{
	if (compare(pOrder->AccountID.getValue(),m_compareObject.AccountID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<COrderIteratorByInstrumentID> COrderIteratorByInstrumentID::iteratorList;
void COrderIteratorByInstrumentID::init(COrderFactory *pFactory, const CReadOnlyAccountIDType& AccountID, const CReadOnlyInstrumentIDType& InstrumentID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.AccountID=AccountID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.OrderID="";
}

COrderIteratorByInstrumentID *COrderIteratorByInstrumentID::alloc(COrderFactory *pFactory, const CReadOnlyAccountIDType& AccountID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	COrderIteratorByInstrumentID *it;
	it=iteratorList.alloc();
	it->init(pFactory,AccountID,InstrumentID);
	return it;
}

COrderIteratorByInstrumentID::~COrderIteratorByInstrumentID(void)
{
}

void COrderIteratorByInstrumentID::free(void)
{
	iteratorList.free(this);
	delete this;
}

void COrderIteratorByInstrumentID::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pInstrumentIDIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pInstrumentIDIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void COrderIteratorByInstrumentID::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((COrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void COrderIteratorByInstrumentID::UpdateCurrent(CWriteableOrder * pByInstrumentID,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((COrder *)(m_pCurrent->pObject),pByInstrumentID,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByInstrumentIDForOrder;
#endif

COrder *COrderIteratorByInstrumentID::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByInstrumentIDForOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((COrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (COrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int COrderIteratorByInstrumentID::match(COrder *pOrder)
{
	if (compare(pOrder->AccountID.getValue(),m_compareObject.AccountID.getValue())!=0)
		return 0;
	if (compare(pOrder->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CPositionOrderIteratorByInsertTime> CPositionOrderIteratorByInsertTime::iteratorList;
void CPositionOrderIteratorByInsertTime::init(CPositionOrderFactory *pFactory, const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyIndexIDType& TradeUnitID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPosiDirectionType& PosiDirection)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.MemberID=MemberID.getValue();
	m_compareObject.TradeUnitID=TradeUnitID.getValue();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.PosiDirection=PosiDirection.getValue();
	m_compareObject.InsertTime=0;
}

CPositionOrderIteratorByInsertTime *CPositionOrderIteratorByInsertTime::alloc(CPositionOrderFactory *pFactory, const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyIndexIDType& TradeUnitID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPosiDirectionType& PosiDirection)
{
	CPositionOrderIteratorByInsertTime *it;
	it=iteratorList.alloc();
	it->init(pFactory,MemberID,TradeUnitID,ExchangeID,InstrumentID,PosiDirection);
	return it;
}

CPositionOrderIteratorByInsertTime::~CPositionOrderIteratorByInsertTime(void)
{
}

void CPositionOrderIteratorByInsertTime::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CPositionOrderIteratorByInsertTime::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pInsertTimeIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pInsertTimeIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CPositionOrderIteratorByInsertTime::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CPositionOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CPositionOrderIteratorByInsertTime::UpdateCurrent(CWriteablePositionOrder * pByInsertTime,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CPositionOrder *)(m_pCurrent->pObject),pByInsertTime,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByInsertTimeForPositionOrder;
#endif

CPositionOrder *CPositionOrderIteratorByInsertTime::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByInsertTimeForPositionOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CPositionOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CPositionOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CPositionOrderIteratorByInsertTime::match(CPositionOrder *pPositionOrder)
{
	if (compare(pPositionOrder->MemberID.getValue(),m_compareObject.MemberID.getValue())!=0)
		return 0;
	if (compare(pPositionOrder->TradeUnitID.getValue(),m_compareObject.TradeUnitID.getValue())!=0)
		return 0;
	if (compare(pPositionOrder->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pPositionOrder->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	if (compare(pPositionOrder->PosiDirection.getValue(),m_compareObject.PosiDirection.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CTriggerOrderIteratorByBuySLTriggerPrice> CTriggerOrderIteratorByBuySLTriggerPrice::iteratorList;
void CTriggerOrderIteratorByBuySLTriggerPrice::init(CTriggerOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& SLTriggerPrice)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.SLTriggerPrice=SLTriggerPrice.getValue();
	m_compareObject.Direction=D_Buy;
	m_compareObject.TimeSortNo=0;
}

CTriggerOrderIteratorByBuySLTriggerPrice *CTriggerOrderIteratorByBuySLTriggerPrice::alloc(CTriggerOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& SLTriggerPrice)
{
	CTriggerOrderIteratorByBuySLTriggerPrice *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID,SLTriggerPrice);
	return it;
}

CTriggerOrderIteratorByBuySLTriggerPrice::~CTriggerOrderIteratorByBuySLTriggerPrice(void)
{
}

void CTriggerOrderIteratorByBuySLTriggerPrice::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CTriggerOrderIteratorByBuySLTriggerPrice::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pBuySLTriggerPriceIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pBuySLTriggerPriceIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CTriggerOrderIteratorByBuySLTriggerPrice::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CTriggerOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CTriggerOrderIteratorByBuySLTriggerPrice::UpdateCurrent(CWriteableTriggerOrder * pByBuySLTriggerPrice,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CTriggerOrder *)(m_pCurrent->pObject),pByBuySLTriggerPrice,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByBuySLTriggerPriceForTriggerOrder;
#endif

CTriggerOrder *CTriggerOrderIteratorByBuySLTriggerPrice::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByBuySLTriggerPriceForTriggerOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CTriggerOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CTriggerOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CTriggerOrderIteratorByBuySLTriggerPrice::match(CTriggerOrder *pTriggerOrder)
{
	if (compare(pTriggerOrder->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pTriggerOrder->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	if (compare(pTriggerOrder->Direction.getValue(),D_Buy)!=0)
		return 0;
	return 1;
}

CMemoryStack<CTriggerOrderIteratorBySellSLTriggerPrice> CTriggerOrderIteratorBySellSLTriggerPrice::iteratorList;
void CTriggerOrderIteratorBySellSLTriggerPrice::init(CTriggerOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& SLTriggerPrice)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.SLTriggerPrice=SLTriggerPrice.getValue();
	m_compareObject.Direction=D_Sell;
	m_compareObject.TimeSortNo=0;
}

CTriggerOrderIteratorBySellSLTriggerPrice *CTriggerOrderIteratorBySellSLTriggerPrice::alloc(CTriggerOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& SLTriggerPrice)
{
	CTriggerOrderIteratorBySellSLTriggerPrice *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID,SLTriggerPrice);
	return it;
}

CTriggerOrderIteratorBySellSLTriggerPrice::~CTriggerOrderIteratorBySellSLTriggerPrice(void)
{
}

void CTriggerOrderIteratorBySellSLTriggerPrice::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CTriggerOrderIteratorBySellSLTriggerPrice::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pSellSLTriggerPriceIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pSellSLTriggerPriceIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CTriggerOrderIteratorBySellSLTriggerPrice::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CTriggerOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CTriggerOrderIteratorBySellSLTriggerPrice::UpdateCurrent(CWriteableTriggerOrder * pBySellSLTriggerPrice,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CTriggerOrder *)(m_pCurrent->pObject),pBySellSLTriggerPrice,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindBySellSLTriggerPriceForTriggerOrder;
#endif

CTriggerOrder *CTriggerOrderIteratorBySellSLTriggerPrice::next(void)
{
#ifdef COUNT_SEARCH
	multiFindBySellSLTriggerPriceForTriggerOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CTriggerOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CTriggerOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CTriggerOrderIteratorBySellSLTriggerPrice::match(CTriggerOrder *pTriggerOrder)
{
	if (compare(pTriggerOrder->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pTriggerOrder->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	if (compare(pTriggerOrder->Direction.getValue(),D_Sell)!=0)
		return 0;
	return 1;
}

CMemoryStack<CTriggerOrderIteratorByBuyTPTriggerPrice> CTriggerOrderIteratorByBuyTPTriggerPrice::iteratorList;
void CTriggerOrderIteratorByBuyTPTriggerPrice::init(CTriggerOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& TPTriggerPrice)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.TPTriggerPrice=TPTriggerPrice.getValue();
	m_compareObject.Direction=D_Buy;
	m_compareObject.TimeSortNo=0;
}

CTriggerOrderIteratorByBuyTPTriggerPrice *CTriggerOrderIteratorByBuyTPTriggerPrice::alloc(CTriggerOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& TPTriggerPrice)
{
	CTriggerOrderIteratorByBuyTPTriggerPrice *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID,TPTriggerPrice);
	return it;
}

CTriggerOrderIteratorByBuyTPTriggerPrice::~CTriggerOrderIteratorByBuyTPTriggerPrice(void)
{
}

void CTriggerOrderIteratorByBuyTPTriggerPrice::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CTriggerOrderIteratorByBuyTPTriggerPrice::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pBuyTPTriggerPriceIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pBuyTPTriggerPriceIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CTriggerOrderIteratorByBuyTPTriggerPrice::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CTriggerOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CTriggerOrderIteratorByBuyTPTriggerPrice::UpdateCurrent(CWriteableTriggerOrder * pByBuyTPTriggerPrice,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CTriggerOrder *)(m_pCurrent->pObject),pByBuyTPTriggerPrice,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByBuyTPTriggerPriceForTriggerOrder;
#endif

CTriggerOrder *CTriggerOrderIteratorByBuyTPTriggerPrice::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByBuyTPTriggerPriceForTriggerOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CTriggerOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CTriggerOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CTriggerOrderIteratorByBuyTPTriggerPrice::match(CTriggerOrder *pTriggerOrder)
{
	if (compare(pTriggerOrder->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pTriggerOrder->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	if (compare(pTriggerOrder->Direction.getValue(),D_Buy)!=0)
		return 0;
	return 1;
}

CMemoryStack<CTriggerOrderIteratorBySellTPTriggerPrice> CTriggerOrderIteratorBySellTPTriggerPrice::iteratorList;
void CTriggerOrderIteratorBySellTPTriggerPrice::init(CTriggerOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& TPTriggerPrice)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.TPTriggerPrice=TPTriggerPrice.getValue();
	m_compareObject.Direction=D_Sell;
	m_compareObject.TimeSortNo=0;
}

CTriggerOrderIteratorBySellTPTriggerPrice *CTriggerOrderIteratorBySellTPTriggerPrice::alloc(CTriggerOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& TPTriggerPrice)
{
	CTriggerOrderIteratorBySellTPTriggerPrice *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID,TPTriggerPrice);
	return it;
}

CTriggerOrderIteratorBySellTPTriggerPrice::~CTriggerOrderIteratorBySellTPTriggerPrice(void)
{
}

void CTriggerOrderIteratorBySellTPTriggerPrice::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CTriggerOrderIteratorBySellTPTriggerPrice::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pSellTPTriggerPriceIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pSellTPTriggerPriceIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CTriggerOrderIteratorBySellTPTriggerPrice::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CTriggerOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CTriggerOrderIteratorBySellTPTriggerPrice::UpdateCurrent(CWriteableTriggerOrder * pBySellTPTriggerPrice,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CTriggerOrder *)(m_pCurrent->pObject),pBySellTPTriggerPrice,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindBySellTPTriggerPriceForTriggerOrder;
#endif

CTriggerOrder *CTriggerOrderIteratorBySellTPTriggerPrice::next(void)
{
#ifdef COUNT_SEARCH
	multiFindBySellTPTriggerPriceForTriggerOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CTriggerOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CTriggerOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CTriggerOrderIteratorBySellTPTriggerPrice::match(CTriggerOrder *pTriggerOrder)
{
	if (compare(pTriggerOrder->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pTriggerOrder->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	if (compare(pTriggerOrder->Direction.getValue(),D_Sell)!=0)
		return 0;
	return 1;
}

CMemoryStack<CTriggerOrderIteratorByAccountID> CTriggerOrderIteratorByAccountID::iteratorList;
void CTriggerOrderIteratorByAccountID::init(CTriggerOrderFactory *pFactory, const CReadOnlyAccountIDType& AccountID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.AccountID=AccountID.getValue();
}

CTriggerOrderIteratorByAccountID *CTriggerOrderIteratorByAccountID::alloc(CTriggerOrderFactory *pFactory, const CReadOnlyAccountIDType& AccountID)
{
	CTriggerOrderIteratorByAccountID *it;
	it=iteratorList.alloc();
	it->init(pFactory,AccountID);
	return it;
}

CTriggerOrderIteratorByAccountID::~CTriggerOrderIteratorByAccountID(void)
{
}

void CTriggerOrderIteratorByAccountID::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CTriggerOrderIteratorByAccountID::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pAccountIDLocalIDIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pAccountIDLocalIDIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CTriggerOrderIteratorByAccountID::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CTriggerOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CTriggerOrderIteratorByAccountID::UpdateCurrent(CWriteableTriggerOrder * pByAccountID,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CTriggerOrder *)(m_pCurrent->pObject),pByAccountID,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByAccountIDForTriggerOrder;
#endif

CTriggerOrder *CTriggerOrderIteratorByAccountID::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByAccountIDForTriggerOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CTriggerOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CTriggerOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CTriggerOrderIteratorByAccountID::match(CTriggerOrder *pTriggerOrder)
{
	if (compare(pTriggerOrder->AccountID.getValue(),m_compareObject.AccountID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CTriggerOrderIteratorByActivePositionClose> CTriggerOrderIteratorByActivePositionClose::iteratorList;
void CTriggerOrderIteratorByActivePositionClose::init(CTriggerOrderFactory *pFactory, const CReadOnlyLongIndexIDType& PositionID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.PositionID=PositionID.getValue();
}

CTriggerOrderIteratorByActivePositionClose *CTriggerOrderIteratorByActivePositionClose::alloc(CTriggerOrderFactory *pFactory, const CReadOnlyLongIndexIDType& PositionID)
{
	CTriggerOrderIteratorByActivePositionClose *it;
	it=iteratorList.alloc();
	it->init(pFactory,PositionID);
	return it;
}

CTriggerOrderIteratorByActivePositionClose::~CTriggerOrderIteratorByActivePositionClose(void)
{
}

void CTriggerOrderIteratorByActivePositionClose::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CTriggerOrderIteratorByActivePositionClose::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pActivePositionCloseIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pActivePositionCloseIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CTriggerOrderIteratorByActivePositionClose::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CTriggerOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CTriggerOrderIteratorByActivePositionClose::UpdateCurrent(CWriteableTriggerOrder * pByActivePositionClose,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CTriggerOrder *)(m_pCurrent->pObject),pByActivePositionClose,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByActivePositionCloseForTriggerOrder;
#endif

CTriggerOrder *CTriggerOrderIteratorByActivePositionClose::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByActivePositionCloseForTriggerOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CTriggerOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CTriggerOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CTriggerOrderIteratorByActivePositionClose::match(CTriggerOrder *pTriggerOrder)
{
	if (compare(pTriggerOrder->PositionID.getValue(),m_compareObject.PositionID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CMarketOrderIteratorByBuyMBLData> CMarketOrderIteratorByBuyMBLData::iteratorList;
void CMarketOrderIteratorByBuyMBLData::init(CMarketOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.Price=999999999;
}

CMarketOrderIteratorByBuyMBLData *CMarketOrderIteratorByBuyMBLData::alloc(CMarketOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	CMarketOrderIteratorByBuyMBLData *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID);
	return it;
}

CMarketOrderIteratorByBuyMBLData::~CMarketOrderIteratorByBuyMBLData(void)
{
}

void CMarketOrderIteratorByBuyMBLData::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CMarketOrderIteratorByBuyMBLData::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pBuyMBLIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pBuyMBLIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CMarketOrderIteratorByBuyMBLData::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CMarketOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CMarketOrderIteratorByBuyMBLData::UpdateCurrent(CWriteableMarketOrder * pByBuyMBLData,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CMarketOrder *)(m_pCurrent->pObject),pByBuyMBLData,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByBuyMBLDataForMarketOrder;
#endif

CMarketOrder *CMarketOrderIteratorByBuyMBLData::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByBuyMBLDataForMarketOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CMarketOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CMarketOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CMarketOrderIteratorByBuyMBLData::match(CMarketOrder *pMarketOrder)
{
	if (compare(pMarketOrder->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pMarketOrder->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CMarketOrderIteratorBySellMBLData> CMarketOrderIteratorBySellMBLData::iteratorList;
void CMarketOrderIteratorBySellMBLData::init(CMarketOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.Price=-999999999;
}

CMarketOrderIteratorBySellMBLData *CMarketOrderIteratorBySellMBLData::alloc(CMarketOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	CMarketOrderIteratorBySellMBLData *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID);
	return it;
}

CMarketOrderIteratorBySellMBLData::~CMarketOrderIteratorBySellMBLData(void)
{
}

void CMarketOrderIteratorBySellMBLData::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CMarketOrderIteratorBySellMBLData::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pSellMBLIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pSellMBLIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CMarketOrderIteratorBySellMBLData::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CMarketOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CMarketOrderIteratorBySellMBLData::UpdateCurrent(CWriteableMarketOrder * pBySellMBLData,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CMarketOrder *)(m_pCurrent->pObject),pBySellMBLData,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindBySellMBLDataForMarketOrder;
#endif

CMarketOrder *CMarketOrderIteratorBySellMBLData::next(void)
{
#ifdef COUNT_SEARCH
	multiFindBySellMBLDataForMarketOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CMarketOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CMarketOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CMarketOrderIteratorBySellMBLData::match(CMarketOrder *pMarketOrder)
{
	if (compare(pMarketOrder->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pMarketOrder->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CTickMarketOrderIteratorByBuyMBLData> CTickMarketOrderIteratorByBuyMBLData::iteratorList;
void CTickMarketOrderIteratorByBuyMBLData::init(CTickMarketOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Tick)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.Tick=Tick.getValue();
	m_compareObject.Price=999999999;
}

CTickMarketOrderIteratorByBuyMBLData *CTickMarketOrderIteratorByBuyMBLData::alloc(CTickMarketOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Tick)
{
	CTickMarketOrderIteratorByBuyMBLData *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID,Tick);
	return it;
}

CTickMarketOrderIteratorByBuyMBLData::~CTickMarketOrderIteratorByBuyMBLData(void)
{
}

void CTickMarketOrderIteratorByBuyMBLData::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CTickMarketOrderIteratorByBuyMBLData::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pBuyMBLIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pBuyMBLIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CTickMarketOrderIteratorByBuyMBLData::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CTickMarketOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CTickMarketOrderIteratorByBuyMBLData::UpdateCurrent(CWriteableTickMarketOrder * pByBuyMBLData,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CTickMarketOrder *)(m_pCurrent->pObject),pByBuyMBLData,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByBuyMBLDataForTickMarketOrder;
#endif

CTickMarketOrder *CTickMarketOrderIteratorByBuyMBLData::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByBuyMBLDataForTickMarketOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CTickMarketOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CTickMarketOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CTickMarketOrderIteratorByBuyMBLData::match(CTickMarketOrder *pTickMarketOrder)
{
	if (compare(pTickMarketOrder->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pTickMarketOrder->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	if (compare(pTickMarketOrder->Tick.getValue(),m_compareObject.Tick.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CTickMarketOrderIteratorBySellMBLData> CTickMarketOrderIteratorBySellMBLData::iteratorList;
void CTickMarketOrderIteratorBySellMBLData::init(CTickMarketOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Tick)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.Tick=Tick.getValue();
	m_compareObject.Price=-999999999;
}

CTickMarketOrderIteratorBySellMBLData *CTickMarketOrderIteratorBySellMBLData::alloc(CTickMarketOrderFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Tick)
{
	CTickMarketOrderIteratorBySellMBLData *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID,Tick);
	return it;
}

CTickMarketOrderIteratorBySellMBLData::~CTickMarketOrderIteratorBySellMBLData(void)
{
}

void CTickMarketOrderIteratorBySellMBLData::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CTickMarketOrderIteratorBySellMBLData::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pSellMBLIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pSellMBLIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CTickMarketOrderIteratorBySellMBLData::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CTickMarketOrder *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CTickMarketOrderIteratorBySellMBLData::UpdateCurrent(CWriteableTickMarketOrder * pBySellMBLData,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CTickMarketOrder *)(m_pCurrent->pObject),pBySellMBLData,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindBySellMBLDataForTickMarketOrder;
#endif

CTickMarketOrder *CTickMarketOrderIteratorBySellMBLData::next(void)
{
#ifdef COUNT_SEARCH
	multiFindBySellMBLDataForTickMarketOrder++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CTickMarketOrder *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CTickMarketOrder *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CTickMarketOrderIteratorBySellMBLData::match(CTickMarketOrder *pTickMarketOrder)
{
	if (compare(pTickMarketOrder->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pTickMarketOrder->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	if (compare(pTickMarketOrder->Tick.getValue(),m_compareObject.Tick.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CPublishKLineIteratorByBar> CPublishKLineIteratorByBar::iteratorList;
void CPublishKLineIteratorByBar::init(CPublishKLineFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.Bar=Bar.getValue();
	m_compareObject.InsertTime=99999999999999.0;
}

CPublishKLineIteratorByBar *CPublishKLineIteratorByBar::alloc(CPublishKLineFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar)
{
	CPublishKLineIteratorByBar *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID,Bar);
	return it;
}

CPublishKLineIteratorByBar::~CPublishKLineIteratorByBar(void)
{
}

void CPublishKLineIteratorByBar::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CPublishKLineIteratorByBar::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pBarInsertTimeIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pBarInsertTimeIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CPublishKLineIteratorByBar::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CPublishKLine *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CPublishKLineIteratorByBar::UpdateCurrent(CWriteablePublishKLine * pByBar,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CPublishKLine *)(m_pCurrent->pObject),pByBar,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByBarForPublishKLine;
#endif

CPublishKLine *CPublishKLineIteratorByBar::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByBarForPublishKLine++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CPublishKLine *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CPublishKLine *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CPublishKLineIteratorByBar::match(CPublishKLine *pPublishKLine)
{
	if (compare(pPublishKLine->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pPublishKLine->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	if (compare(pPublishKLine->Bar.getValue(),m_compareObject.Bar.getValue())!=0)
		return 0;
	return 1;
}

CMemoryStack<CPublishKLineIteratorByBarInsertTime> CPublishKLineIteratorByBarInsertTime::iteratorList;
void CPublishKLineIteratorByBarInsertTime::init(CPublishKLineFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar, const CReadOnlyMilliSecondsType& InsertTime)
{				
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
		break;
	default:
		EMERGENCY_EXIT("Can not use CAllowIterator while run level = %c", g_cMDBRunLevel);
	}
		
	m_Factory=pFactory;
	m_isFirstTime=1;
	m_gotNext=false;
	m_pCurrent=NULL;
	m_pNext=NULL;

	///根据参数和缺省值，设置用于比较的对象的各个分量
	m_compareObject.init();
	m_compareObject.ExchangeID=ExchangeID.getValue();
	m_compareObject.InstrumentID=InstrumentID.getValue();
	m_compareObject.Bar=Bar.getValue();
	m_compareObject.InsertTime=InsertTime.getValue();
}

CPublishKLineIteratorByBarInsertTime *CPublishKLineIteratorByBarInsertTime::alloc(CPublishKLineFactory *pFactory, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar, const CReadOnlyMilliSecondsType& InsertTime)
{
	CPublishKLineIteratorByBarInsertTime *it;
	it=iteratorList.alloc();
	it->init(pFactory,ExchangeID,InstrumentID,Bar,InsertTime);
	return it;
}

CPublishKLineIteratorByBarInsertTime::~CPublishKLineIteratorByBarInsertTime(void)
{
}

void CPublishKLineIteratorByBarInsertTime::free(void)
{
	iteratorList.free(this);
	delete this;
}

void CPublishKLineIteratorByBarInsertTime::internalGetNext()
{
	if (m_gotNext) //已经取过下一个了
		return;
	if (m_isFirstTime)
	{
		//第一次进入，在索引中寻找第一个满足条件的
		m_pNext=m_Factory->pBarInsertTimeIndex->searchFirstGreatEqual(&m_compareObject);
		//设置非第一次标志
		m_isFirstTime=0;
	}
	else
	{
		if (m_pCurrent==NULL)  //已经找到最后一个了			
			m_pNext=NULL;
		else 					//在索引找下一个节点			
			m_pNext=m_Factory->pBarInsertTimeIndex->getNextNode(m_pCurrent);
	}
	m_gotNext=true;
}

void CPublishKLineIteratorByBarInsertTime::DeleteCurrent(CTransaction *pTransaction)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法删除
		return;
	internalGetNext();
	m_Factory->remove((CPublishKLine *)(m_pCurrent->pObject),pTransaction);
	m_pCurrent=NULL;
}
	

void CPublishKLineIteratorByBarInsertTime::UpdateCurrent(CWriteablePublishKLine * pByBarInsertTime,CTransaction* pTransaction, bool updateIndex)
{
	if (m_pCurrent==NULL) //当前节点不存在，所以也无法修改
		return;
	if (updateIndex) //要更新索引，那就必须预取下一个
		internalGetNext();
	m_Factory->update((CPublishKLine *)(m_pCurrent->pObject),pByBarInsertTime,pTransaction,updateIndex);
}

#ifdef COUNT_SEARCH
extern int multiFindByBarInsertTimeForPublishKLine;
#endif

CPublishKLine *CPublishKLineIteratorByBarInsertTime::next(void)
{
#ifdef COUNT_SEARCH
	multiFindByBarInsertTimeForPublishKLine++;
#endif
	internalGetNext();
	m_gotNext = false;
	m_pCurrent=m_pNext;
	if (m_pCurrent==NULL)//如果没有下一个对象了，则失败返回
		return NULL;
	if (match((CPublishKLine *)(m_pCurrent->pObject))) //符合条件，那就返回它
		return (CPublishKLine *)(m_pCurrent->pObject);
	
	//不符合条件，那就不用做了
	m_pCurrent=NULL;
	return NULL;
}

int CPublishKLineIteratorByBarInsertTime::match(CPublishKLine *pPublishKLine)
{
	if (compare(pPublishKLine->ExchangeID.getValue(),m_compareObject.ExchangeID.getValue())!=0)
		return 0;
	if (compare(pPublishKLine->InstrumentID.getValue(),m_compareObject.InstrumentID.getValue())!=0)
		return 0;
	if (compare(pPublishKLine->Bar.getValue(),m_compareObject.Bar.getValue())!=0)
		return 0;
	return 1;
}

