#include "CTransaction.h"
//#include "CTimeMeter.h"
#include "CDatabaseResource.h"

#define MAX_TRANSACTION_PRIORITY 3

CMemoryStack<CTransactionSavePoint> CTransactionSavePoint::resourceList;
/*
CTransactionSavePoint::CTransactionSavePoint(CTransaction *pTransaction)
{
	m_Transaction=pTransaction;
	pTransaction->setSavePoint(this);
	pTransaction->m_SavePointCount++;
}
*/
CTransactionSavePoint *CTransactionSavePoint::alloc(CTransaction *pTransaction)
{
	CTransactionSavePoint *pSavePoint = resourceList.alloc();
	pSavePoint->m_Transaction = pTransaction;
	pTransaction->setSavePoint(pSavePoint);
	pTransaction->m_SavePointCount++;
	return pSavePoint;
}

CTransactionSavePoint::~CTransactionSavePoint(void)
{
	m_Transaction->m_SavePointCount--;
}

void CTransactionSavePoint::free(void)
{
	m_Transaction->m_SavePointCount--;
	resourceList.free(this);
}

CTransaction::CTransaction(void)
{
	m_SavePointCount = 0;
	ref = NULL;
}

CTransaction::~CTransaction(void)
{
	//rollback();
}
/*
void CTransaction::addResource(CResource *pResource)
{
	if (mergeBack(size(),pResource,false))
	{
		///合并成功，那就不用加了
		pResource->free();
	}
	else
	{
		///合并不成功，那就要加了
		push_back(pResource);
	}
}
*/
void CTransaction::commit(void)
{
	vector<CResource *> toBeCommit[MAX_TRANSACTION_PRIORITY + 1];
	int priority;
	//这里需要使用使用有符号性，因为判断了大小
	int pos = (int)size() - 1;
	///从前向后，将需要commit的资源放入toBeCommit队列中
	for (; pos >= 0; pos--)
	{
		if (at(pos)->isControl())
		{
			///如果是控制性资源，就直接释放了
			at(pos)->free();
			continue;
		}
		//如果该资源是NONE_ACTION，就直接释放
		CDatabaseResource* pDatabaseResource = (CDatabaseResource *)at(pos);
		if (NONE_ACTION == pDatabaseResource->getResourceType())
		{
			///如果是NONE_ACTION资源，就直接释放了
			at(pos)->free();
			continue;
		}

		if (m_SavePointCount > 0)
		{
			if (mergeBack(pos, at(pos), true))
			{
				///被合并掉了，那就不用commit了
				at(pos)->free();
				continue;
			}
		}
		///合并不掉，那就只好放入等待commit的队列中了
		priority = at(pos)->getPriority();
		if (priority < 0)
		{
			priority = 0;
		}
		else if (priority > MAX_TRANSACTION_PRIORITY)
		{
			priority = MAX_TRANSACTION_PRIORITY;
		}
		toBeCommit[priority].push_back(at(pos));
		//这里的效率不高，会在资源的二次方的阶上完成合并和commit。
		//虽然没有降低整体的阶，但是还是对系数有一定的影响。我们采用
		//m_SavePointCount来控制是否需要mergeBack，这会使大部分没有
		//使用savepoint的事务效率提高。
	}
	//按照优先级从低到高，位置从后往前，完成所有资源的commit和释放
	for (priority = 0; priority <= MAX_TRANSACTION_PRIORITY; priority++)
	{
		vector<CResource *>::reverse_iterator it;
		for (it = toBeCommit[priority].rbegin(); it < toBeCommit[priority].rend(); it++)
			//vector<CResource *>::iterator it;
			//for (it=toBeCommit[priority].begin();it<toBeCommit[priority].end();it++)
		{
			(*it)->commit();
			(*it)->free();
		}
	}
	clear();
	m_SavePointCount = 0;
}
/*
bool CTransaction::mergeBack(int pos,CResource *pResource,bool forceMerge)
{
	METER_START(3);
	for (pos--;pos>=0;pos--)
	{
		switch (at(pos)->merge(pResource))
		{
		case RESOURCE_MERGE_OK:
			///合并成功
			METER_END(3);
			return true;
		case RESOURCE_MERGE_FAIL_THIS:
			///这个没有合并成功，应当尝试下一个
			break;
		case RESOURCE_MERGE_FAIL_ALL:
			///全部失败
			if (!forceMerge)
			{
				//不用再试了
				METER_END(3);
				return false;
			}
			break;
		}
	}
	METER_END(3);
	return false;
}
*/
void CTransaction::rollback(void)
{
	rollbackToSavePoint(NULL);
	m_SavePointCount = 0;
}

void CTransaction::rollbackToSavePoint(CTransactionSavePoint *pSavePoint)
{
	int pos = (int)size(); //需要使用int，size_t是无符号数，不能比较>=0
	///从后向前，rollback所有指定保存点以前的资源
	for (pos--; pos >= 0; pos--)
	{
		if (at(pos) == pSavePoint)
		{
			///找到该保存点了，那就删除后面的所有资源，但是要保存目前的这个保存点，然后返回		
			//之前为什么要保持目前的保存点呢？
			//之前回退SavePoint的时候没有释放回退点，也没有删除回退点
			at(pos)->free();
			erase(begin() + pos, end());
			//erase(begin()+pos+1,end());
			return;
		}
		///本资源是保存点以后的资源，那就将其rollback，并予以释放
		at(pos)->rollback();
		at(pos)->free();
	}
	///所有资源都已经rollback了，还是没有找到指定的保存点，那就等于全部rollback
	clear();
}

void CTransaction::setSavePoint(CTransactionSavePoint *pSavePoint)
{
	push_back(pSavePoint);
}
