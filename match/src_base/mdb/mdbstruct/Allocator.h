#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

struct MemoryAllocatorHead;

#define	RESERVE_BLOCK_ID	5				//预留的头

///内存分配的接口
class IMemoryAllocator
{
public:
	virtual ~IMemoryAllocator(void)
	{
	}

	/// 获得制定大小的快数量
	///@param size 获取指定大小的内存
	///@param reuseID 表示指定的块号 -1表示由系统分配
	///@param isReuse 是否需要重用	
	virtual void* alloc(int size,int &reuseID,bool isReuse = false) = 0;

	///初始化方法
	virtual void init(bool reuse) = 0;
};

///普通内存分配器
class CMemoryAllocator:public IMemoryAllocator
{
public:
	/// 获得制定大小的快数量
	///@param size 获取指定大小的内存
	///@param reuseID 表示指定的块号 -1表示由系统分配
	///@param isReuse 是否需要重用	
	virtual void* alloc(int size,int &reuseID,bool isReuse = false);

	///初始化内存
	///@param reuse 是否为重用
	virtual void init(bool reuse);	

	bool	isValid();
	
protected:
	CMemoryAllocator(int memorySize,int blockCount);

	char *m_startPosition;				///内存快的首地址
	MemoryAllocatorHead* m_pHead;		///分配的内存头
	char *m_maxEndPosition;

protected:
	///更新已使用的块数
	void	UpdateUseCount(int value);	
	///获取已使用的块数
	int		GetUseCount();
	///更新下一可用内存块的首地址
	void	UpdateEmptyAddress(char *Address);
	///或取下一可用内存块的首地址
	char*	GetEmptyAddress();
	///检查某一块是否已被使用
	///@param blockID 块编号
	///@return true 已被使用 false未被使用
	bool	IsBlockUsed(int blockID);	
	~CMemoryAllocator();

	void writeLog();
};

///普通内存分配器
class CNormalMemoryAllocator:public CMemoryAllocator
{
public:
	CNormalMemoryAllocator(int memorySize,int blockCount);
	///初始化内存
	///@param reuse 是否为重用
	virtual void init(bool reuse);	
};


///共享内存分配器
class CShareMemoryAllocator:public CMemoryAllocator
{
public:
	CShareMemoryAllocator(int memorySize,int blockCount);
	///初始化内存
	///@param reuse 是否为重用
	virtual void init(bool reuse);	
};

#endif
