///CDatabaseDataFactory.h
///定义了类CDatabaseDataFactory
/////////////////////////////////////////////////////////////////////////

#ifndef CDATABASEDATAFACTORY_H
#define CDATABASEDATAFACTORY_H

#include "CBaseObject.h"
#include "CFixMem.h"
//#include "CConfig.h"
#include "Allocator.h"

const int MDB_DEFAULT_BLOCK_SIZE = 4096;

/////////////////////////////////////////////////////////////////////////
///CDatabaseDataFactory是一个抽象的交易所内部数据对象工厂，将使用CFixMem管理其
///全部内容，负责内部数据对象的创建、删除、更改和替换功能。如果把一项内部数据内容
///认为是数据库中的一行，那么内部数据对象工厂就相当于一个表。各个具体种类的交易所
///内部数据需要分别继承这个类，实现自己的对象工厂。
///@author	tom
///@version	1.0,20020214
/////////////////////////////////////////////////////////////////////////
class CDatabaseDataFactory : public CBaseObject
{
public:
	///构造方法，将创建自己的CFixMem
	///@param	unitSize	每个数据对象的大小
	///@param	maxTradeUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CDatabaseDataFactory(int unitSize, int maxTradeUnit,IMemoryAllocator *pAllocator,bool reuse=false, int reuseID = -1, int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CDatabaseDataFactory(void);
	
	virtual int isA(char *objectType);
	virtual const char *getType(void);
//	virtual void output(CLogger *pLogger,int indent=0);

	///获取本CDatabaseDataFactory中已经有的纪录个数
	///@return	纪录个数
	int getCount(void)
	{
		return pMem->getCount();
	}

	///获取指向本CDatabaseDataFactory中已有记录个数的指针
	///@return	指向本CDatabaseDataFactory中已有记录个数的指针
	const int *getCountPtr(void)
	{
		return pMem->GetCountPtr();
	}

	
	///清空所有的记录
	virtual void clearAll(void);

	///从CSV文件中读取所有的对象
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename)=0;
	
	///向CSV文件中写出所有的对象
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename)=0;

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *)=0;

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename)=0;

	int	getReuseID();

protected:
	///创建一个对象
	///@return	指向这个对象位置的指针
	void *createObject(void)
	{
		return pMem->alloc();
	}
	
	///删除一个对象
	///@param	pObject	指向这个对象的指针
	void deleteObject(void *pObject)
	{
		pMem->free(pObject);
	}

	///用于存放数据的CFixMem
	CFixMem *pMem;
	
	///最多对象个数
	int maxTradeUnit;
};

#endif
