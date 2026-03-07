#include "Allocator.h"
#include <memory.h>
#include <stdio.h>
#include <iostream>
//#include "platform.h"
#include "CBaseObject.h"
//#include "CConfig.h"
#include "monitorIndex.h"
#include "public.h"

// 32位下，缺省使用512M的空间
#define DEFAULT_MEMORY_SIZE_32 (512)

// 64位下，缺省使用4G的空间
#define DEFAULT_MEMORY_SIZE_64 (4*1024)

/// 32位下，缺省的最大可分配的块数
#define DEFAULT_MAX_BLOCK_COUNT_32		(16*1024)

/// 64位下，缺省的最大可分配的块数
#define DEFAULT_MAX_BLOCK_COUNT_64		(128*1024)

/// 共享内存时，使用的内存编号
#define SHARE_MEMORY_START_ID	15

/// 申请的内存大小
size_t dwMemorySize;

/// 最大可分配块数
int maxBlockCount;

///表示内存块的使用情况
struct UsedBlockStruct
{
	void* startPosition;	///内存块的起始地址
};

///分配的内存的头
struct MemoryAllocatorHead
{
	void* m_pthisPoint;	///指向自己的指针，用来进行检验
	int version;		///版本号
	int usedBlockNo;	///已经被占用的块数
	void* m_firstEmptyAddress;		///指向首先可用的内存区域
	UsedBlockStruct blockVect[1];	///内存块的使用状况，实际的数组大小应当由maxBlockCount控制
};

static CIntUsageMonitorIndex *pMemoryIndex, *pBlockCountIndex;

CMemoryAllocator::CMemoryAllocator(int memorySize, int blockCount)
{
	if (sizeof(void *) == 4)
	{
		/// 32位程序
		dwMemorySize = (size_t)1024 * 1024 * DEFAULT_MEMORY_SIZE_32;
		maxBlockCount = DEFAULT_MAX_BLOCK_COUNT_32;
	}
	else if (sizeof(void *) == 8)
	{
		/// 64位程序
		dwMemorySize = (size_t)1024 * 1024 * DEFAULT_MEMORY_SIZE_64;
		maxBlockCount = DEFAULT_MAX_BLOCK_COUNT_64;
	}
	else
	{
		EMERGENCY_EXIT("Unknown machine word size");
	}

	if (memorySize > 0)
		dwMemorySize = (size_t)1024 * 1024 * memorySize;

	if (blockCount > 0)
		maxBlockCount = blockCount;

	pMemoryIndex = new CIntUsageMonitorIndex("MemoryDatabase", dwMemorySize / 1024l / 1024l, 20);
	pBlockCountIndex = new CIntUsageMonitorIndex("MDBBlock", maxBlockCount, 20);
}

CMemoryAllocator::~CMemoryAllocator()
{
}

void CMemoryAllocator::UpdateUseCount(int value)
{
	m_pHead->usedBlockNo = value;
}

int	CMemoryAllocator::GetUseCount()
{
	return m_pHead->usedBlockNo;
}

void CMemoryAllocator::UpdateEmptyAddress(char *Address)
{
	m_pHead->m_firstEmptyAddress = (void *)Address;
}

char* CMemoryAllocator::GetEmptyAddress()
{
	return (char *)m_pHead->m_firstEmptyAddress;
}

bool CMemoryAllocator::IsBlockUsed(int blockID)
{
	if (blockID >= maxBlockCount)
		return true;
	if (m_pHead->blockVect[blockID].startPosition != NULL)
		return true;
	return false;
}

bool CMemoryAllocator::isValid()
{
	if (m_pHead->m_pthisPoint != &m_pHead)
		return false;
	return true;
}

void CMemoryAllocator::writeLog()
{
	pBlockCountIndex->setValue(GetUseCount());
	pMemoryIndex->setValue(((char *)GetEmptyAddress() - (char *)m_pHead) / 1024l / 1024l);
}

void* CMemoryAllocator::alloc(int size, int &reuseID, bool isReuse)
{
	if (!isReuse)		///如果不是重用
	{
		if (reuseID == -1)	///由Allocator负责分配ID号
		{
			int totalBlock = GetUseCount();
			while (IsBlockUsed(totalBlock))		///找到一个未分配的可用的编号
			{
				if (totalBlock >= maxBlockCount)
				{
					EMERGENCY_EXIT("Insufficient meomry space controled by allocator(block count overflow)");
				}
				totalBlock++;
			}
			reuseID = totalBlock;
			UpdateUseCount(totalBlock);

			void *pEmpty = m_pHead->m_firstEmptyAddress;
			m_pHead->m_firstEmptyAddress = (char *)pEmpty + size;
			if (m_pHead->m_firstEmptyAddress > m_maxEndPosition)	///如果分配的内存地址已大于最大可用的内存,出错!
				EMERGENCY_EXIT("Insufficient meomry space controled by allocator");
			m_pHead->blockVect[totalBlock].startPosition = pEmpty;
			writeLog();
			return pEmpty;
		}
		else		///根据传入的ID号申请内存
		{
			if (IsBlockUsed(reuseID))
				return 0;
			void *pEmpty = m_pHead->m_firstEmptyAddress;
			m_pHead->m_firstEmptyAddress = (char *)pEmpty + size;
			m_pHead->blockVect[reuseID].startPosition = pEmpty;
			writeLog();
			return pEmpty;
		}
	}
	else		///如果是重用的情况
	{
		if (IsBlockUsed(reuseID))
		{
			return m_pHead->blockVect[reuseID].startPosition;
		}
	}
	return 0;
}

void CMemoryAllocator::init(bool reuse)
{
	if (!reuse)
	{
		if (m_startPosition == NULL)
			EMERGENCY_EXIT("Not Enough Memory!");
		m_pHead = (MemoryAllocatorHead *)m_startPosition;
		m_pHead->m_pthisPoint = (void *)m_startPosition;
		m_pHead->version = 1;
		m_pHead->m_firstEmptyAddress = m_startPosition + sizeof(MemoryAllocatorHead) + sizeof(UsedBlockStruct)*(maxBlockCount - 1);
		m_pHead->usedBlockNo = RESERVE_BLOCK_ID;
		for (int i = 0; i < maxBlockCount; i++)
		{
			m_pHead->blockVect[i].startPosition = NULL;
		}
		m_maxEndPosition = m_startPosition + dwMemorySize;	///计算出最大分配内存的位置
	}
	else
	{
		if (!isValid())
			EMERGENCY_EXIT("Invalid Memory Reuse!");
		m_pHead = (MemoryAllocatorHead *)m_startPosition;
		if (m_pHead == NULL)
			EMERGENCY_EXIT("Reuse Memory Error!");
	}
}

CNormalMemoryAllocator::CNormalMemoryAllocator(int memorySize, int blockCount) :CMemoryAllocator(memorySize, blockCount)
{
}

void CNormalMemoryAllocator::init(bool reuse)
{
	if (!reuse)
	{
		m_startPosition = new char[dwMemorySize];
		CMemoryAllocator::init(reuse);
	}
	else
	{
		EMERGENCY_EXIT("Normal memory can not reuse");
	}
}


CShareMemoryAllocator::CShareMemoryAllocator(int memorySize, int blockCount) :CMemoryAllocator(memorySize, blockCount)
{
}

void CShareMemoryAllocator::init(bool reuse)
{
	if (!reuse)
	{
		ALLOC_SHARE_MEMORY(&m_startPosition, SHARE_MEMORY_START_ID, dwMemorySize);
	}
	else
	{
		REUSE_SHARE_MEMORY(&m_startPosition, SHARE_MEMORY_START_ID, dwMemorySize);
	}
	CMemoryAllocator::init(reuse);
}



