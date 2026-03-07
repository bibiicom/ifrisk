#ifndef CDOUBLEHASH_H
#define CDOUBLEHASH_H

#include "CBaseObject.h"
#include "CFixMem.h"
#include "Allocator.h"

class CDoubleHash
{
public:
	CDoubleHash();
	
	///析构函数，负责释放空间
	virtual ~CDoubleHash(void);
	
	///加入一个新的对象节点
	///@param	pObject	新的对象的首地址
	///@return	加入树后包含此对象的节点
	 bool addObject(const void *pObject, string sHashKey1, string sHashKey2);

	///寻找第一个等于此对象的节点
	///@param pObject	比较时用的对象
	///@return	第一个等于此对象的节点，如果不存在此节点，则返回NULL
	void *searchFirstEqual(string sHashKey1);
	
	///寻找下一个节点，即大于此节点的第一个节点
	///@param pNode	目前节点
	///@return	大于此节点的第一个节点，如果不存在此节点，则返回NULL
	void *getNextNode();
	
	///寻找等于此对象的节点，这里的相等是指地址上相等
	///@param pObject	比较时用的对象
	///@return	等于此对象的节点，如果不存在此节点，则返回NULL
	void *findObject(string sHashKey1, string sHashKey2);
	
	///删除包含指定对象的节点
	///@param	pObject	要删除的对象
	bool removeObject(string sHashKey1, string sHashKey2);
	
	///全部删除
	void removeAll();

private:

};

#endif
