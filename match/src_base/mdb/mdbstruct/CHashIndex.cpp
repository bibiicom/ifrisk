#include "CHashIndex.h"
//#include "HashMap.h"

CHashIndex::CHashIndex(int hashKeySize, IMemoryAllocator *pAllocator, bool reuse, int reuseID, int readOnly)
{
	int i = 0;
	for (; i < __stl_num_primes_hashindex; i++)
	{
		if (__stl_prime_list_hashindex[i] >= hashKeySize)
		{
			m_hashKeySize = __stl_prime_list_hashindex[i];
			break;
		}
	}
	if (i == __stl_num_primes_hashindex)
		EMERGENCY_EXIT("Too large hash size");
	pIndexMem = new CFixMem(sizeof(CHashIndexNode), 4096 * 8, pAllocator, reuse, reuseID, readOnly);
	if (pIndexMem == NULL)
		EMERGENCY_EXIT("Insufficient memory space");
	if (!reuse)
		init();
}

CHashIndex::~CHashIndex(void)
{
	delete pIndexMem;
}

void CHashIndex::init(void)
{
	for (int i = 0; i < m_hashKeySize; i++)
	{
		CHashIndexNode *pNode = (CHashIndexNode *)(pIndexMem->alloc());
		pNode->pObject = NULL;
		pNode->next = NULL;
	}
}
