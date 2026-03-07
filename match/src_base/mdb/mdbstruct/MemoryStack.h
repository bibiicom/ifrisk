/////////////////////////////////////////////////////////////////////////
///MemoryStack.h
///定义了以堆栈方式申请和释放的某种特定类型的内存
/////////////////////////////////////////////////////////////////////////

#ifndef MEMORYSTACK_H
#define MEMORYSTACK_H

//#include "CBaseObject.h"
//#include "utility.h"

#include "platform.h"

/////////////////////////////////////////////////////////////////////////
///CMemoryStack<T>是一个类型T的以堆栈方式申请和释放的内存管理器。也就是说，
///类型T的申请和释放将使用这个类的alloc和free方法，而且，先alloc出去的，
///一定后free。这里的T必须有缺省的构造子，而且不能在内部再申请其他空间，
///其析构方法必须无事可做
///@author	tom
///@version	1.0,20060513
/////////////////////////////////////////////////////////////////////////
template <class T> class CMemoryStack: public vector<T *>
{
private:
	int allocCount;
public:
	///构造函数
	CMemoryStack(void)
	{
		allocCount=0;
	}

	///申请一个
	///@return	申请得到的对象
	T *alloc(void)
	{
		if (allocCount==vector<T *>::size())
		{
			//	push_back(new T);  g++ -fpermissive 编译不通过
			//	如果当前数组vector中的指针都用光了，则生成一个新的对象，并把指针存放在数组中。
			this->push_back(new T);
		}
		return vector<T *>::at(allocCount++);	// 直接重复使用前面分配的对象
	}

	///释放一个
	///@param	p	要释放的对象
	/// 对象的索引（指针）是存放在数组vector中，本类中的释放指的是索引无效了。
	/// 这里只是把数组的索引总数量减少一个。即这个指针无效了。
	/// 真正的对象本身，并没有释放
	void free(T *p)
	{
		allocCount--;
	}
	
	/// 释放所有对象
	/// 表示数组vector中，所有的指针都无效了。
	void clear(void)
	{
		allocCount=0;
	}
}; 

#endif
