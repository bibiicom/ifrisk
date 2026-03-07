/////////////////////////////////////////////////////////////////////////
///CHashIndex.h
///定义了类CHashIndex
/////////////////////////////////////////////////////////////////////////

#ifndef CHASHINDEX_H
#define CHASHINDEX_H

//#include "platform.h"
#include "CBaseObject.h"
#include "CFixMem.h"
#include "Allocator.h"

/////////////////////////////////////////////////////////////////////////
///CHashIndexNode是一个Hash节点对象
///@author	tom
///@version	1.0,20020611
/////////////////////////////////////////////////////////////////////////

enum { __stl_num_primes_hashindex = 28 };

static const unsigned long __stl_prime_list_hashindex[__stl_num_primes_hashindex] =
{
		53ul,         97ul,         193ul,       389ul,       769ul,
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
		1610612741ul, 3221225473ul, 4294967291ul
};

class CHashIndexNode
{
public:
	///保存本节点中包含的对象
	const void *pObject;
	///保存下一个节点
	CHashIndexNode *next;
};

/////////////////////////////////////////////////////////////////////////
///CHashIndex是一个Hash索引对象。索引对象是对某种类型的数据结构，以及其上的一种比较函数，
///生成的Hash表。
///@author	tom
///@version	1.0,20020611
/////////////////////////////////////////////////////////////////////////
class CHashIndex
{
public:
	///构造函数，申请空间，建立一棵空的Hash索引
	///@param	hashKeySize	HashKey的大小
	///@param	pAllocator	负责内存分配的分配器
	///@param	reuse	说明这个存储是否需要重用
	///@param	reuseID	当重用时，表明重用的编号，不重用则应当使用-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CHashIndex(int hashKeySize, IMemoryAllocator *pAllocator,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///析构函数，负责释放空间
	virtual ~CHashIndex(void);

	///初始化，将所有的桶清空
	void init(void);

	///在本Hash索引中加入一个新的对象节点
	///@param	pObject	新的对象的首地址
	///@param	hashKey	对应的Hash键
	///@return	加入Hash索引后包含此对象的节点
	CHashIndexNode *addObject(const void *pObject, unsigned int hashKey)
	{
		int pos=hashKey%m_hashKeySize;
		CHashIndexNode *pNode=(CHashIndexNode *)(pIndexMem->getObject(pos));
		if (pNode->pObject==NULL)
		{
			///本桶内还没有对象，那就放在第一个位置
			pNode->pObject=pObject;
			return pNode;
		}
		else
		{
			///本桶内已经有对象了，那就放在第二个位置
			CHashIndexNode *pNewNode=(CHashIndexNode *)(pIndexMem->alloc());
			pNewNode->pObject=pObject;
			pNewNode->next=pNode->next;
			pNode->next=pNewNode;
			return pNewNode;
		}
	}

	///在本Hash索引中删除一个对象节点
	///@param	pObject	要删除的对象的首地址
	///@param	hashKey	对应的Hash键
	///@return	true表示删除成功，false表示删除失败
	bool removeObject(const void *pObject, unsigned int hashKey)
	{
		int pos=hashKey%m_hashKeySize;
		CHashIndexNode *pNode=(CHashIndexNode *)(pIndexMem->getObject(pos));
		CHashIndexNode *pNextNode;
		if (pNode->pObject==pObject)
		{
			///要删除的是第一个对象
			if (pNode->next==NULL)
			{
				///没有第二个对象了，那把第一个对象删除了即可
				pNode->pObject=NULL;
				return true;
			}
			else
			{
				///后面还有第二个对象，那就需要把下一个对象升上来，并删除后面一个对象
				pNextNode=pNode->next;
				pNode->pObject=pNextNode->pObject;
				pNode->next=pNextNode->next;
				pIndexMem->free(pNextNode);
				return true;
			}
		}
		else
		{
			///要删除的不是第一个对象
			while ((pNextNode=pNode->next)!=NULL)
			{
				if (pNextNode->pObject==pObject)
				{
					///下一个节点就是要删除的对象，那就删除它
					pNode->next=pNextNode->next;
					pIndexMem->free(pNextNode);
					return true;
				}
				///依次寻找下去
				pNode=pNextNode;
			}
			///没有找到，那就失败返回
			return false;
		}
	}

	///全部删除
	void removeAll(void)
	{
		pIndexMem->freeAll();
		///tom，20070328
		///修正了清空HashIndex后，没有把初始的桶重新建出来的问题
		init();
	}

	///寻找第一个等于某键值的节点
	///@param hashKey	对应的Hash键
	///@return	第一个等于此键值的节点，如果不存在此节点，则返回NULL
	CHashIndexNode *searchFirstEqual(unsigned int hashKey)
	{
		int pos=hashKey%m_hashKeySize;
		CHashIndexNode *pNode=(CHashIndexNode *)(pIndexMem->getObject(pos));
		if (pNode->pObject==NULL)
			return NULL;
		else
			return pNode;
	}

	///寻找下一个等于某键值的节点
	///@param	pNode	相对的键值
	///@return	下一个等于此键值的节点，如果不存在此节点，则返回NULL
	CHashIndexNode *searchNextEqual(CHashIndexNode *pNode)
	{
		return pNode->next;
	}

	///获取重用编号
	///@param	本索引的重用编号
	int getReuseID(void)
	{
		return pIndexMem->GetReuseID();
	}

private:
	///存储存储Hash索引的CFixMem
	CFixMem *pIndexMem;
	
	///HashKey的大小
	int m_hashKeySize;
};

#endif
