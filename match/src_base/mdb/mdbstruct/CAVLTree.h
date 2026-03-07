#ifndef CAVLTREE_H
#define CAVLTREE_H

//#include "platform.h"
#include "CBaseObject.h"
#include "CFixMem.h"
#include "Allocator.h"

///TCompareFunc是一个比较函数指针类型
typedef int (* TCompareFunc)(const void *,const void *);

/////////////////////////////////////////////////////////////////////////
///CAVLNode是一个平衡数节点对象
/////////////////////////////////////////////////////////////////////////
class CAVLNode
{
public:
	///保存本节点中包含的对象
	const void *pObject;
	///保存本节点的父节点、左儿子节点和右儿子节点
	CAVLNode *father, *left, *right;
	///平衡状况
	int depth;
};

/////////////////////////////////////////////////////////////////////////
///CAVLTree是一个索引对象。索引对象是对某种类型的数据结构，以及其上的一种比较函数，
///生成的平衡树。
/////////////////////////////////////////////////////////////////////////
class CAVLTree : public CBaseObject
{
public:
	///构造函数，申请空间，建立一棵空的平衡树
	///@param	maxTradeUnit	最多单位数
	///@param	compareFunc	比较函数指针
	///@param	pAllocator	负责内存分配的分配器
	///@param	reuse	说明这个存储是否需要重用
	///@param	reuseID	当重用时，表明重用的编号，不重用则应当使用-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CAVLTree(int maxTradeUnit, TCompareFunc compareFunc,IMemoryAllocator *pAllocator,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///析构函数，负责释放空间
	virtual ~CAVLTree(void);
	
	virtual int isA(char *objectType);
	virtual const char *getType(void);
//	virtual void output(CLogger *pLogger,int indent=0);
	
	///在本平衡树中加入一个新的对象节点
	///@param	pObject	新的对象的首地址
	///@return	加入树后包含此对象的节点
	CAVLNode *addObject(const void *pObject);
	
	///获取最小的一个节点
	///@return	最小的一个节点，如果还没有节点，则返回NULL
	CAVLNode *getSmallest(void);
	
	///获取最大的一个节点
	///@return	最大的一个节点，如果还没有节点，则返回NULL
	CAVLNode *getLargest(void);

	///寻找第一个大于等于此对象的节点
	///@param pObject	比较时用的对象
	///@return	第一个大于等于此对象的节点，如果不存在此节点，则返回NULL
	CAVLNode *searchFirstGreatEqual(const void *pObject);
	
	///寻找第一个大于此对象的节点
	///@param pObject	比较时用的对象
	///@return	第一个大于此对象的节点，如果不存在此节点，则返回NULL
	CAVLNode *searchFirstGreat(const void *pObject);
	
	///寻找最后一个小于等于此对象的节点
	///@param pObject	比较时用的对象
	///@return	最后一个小于等于此对象的节点，如果不存在此节点，则返回NULL
	CAVLNode *searchLastLessEqual(const void *pObject);
	
	///寻找最后一个小于此对象的节点
	///@param pObject	比较时用的对象
	///@return	最后一个小于此对象的节点，如果不存在此节点，则返回NULL
	CAVLNode *searchLastLess(const void *pObject);
	
	///寻找第一个等于此对象的节点
	///@param pObject	比较时用的对象
	///@return	第一个等于此对象的节点，如果不存在此节点，则返回NULL
	CAVLNode *searchFirstEqual(const void *pObject);
	
	///寻找最后一个等于此对象的节点
	///@param pObject	比较时用的对象
	///@return	最后一个等于此对象的节点，如果不存在此节点，则返回NULL
	CAVLNode *searchLastEqual(const void *pObject);
	
	///寻找下一个节点，即大于此节点的第一个节点
	///@param pNode	目前节点
	///@return	大于此节点的第一个节点，如果不存在此节点，则返回NULL
	CAVLNode *getNextNode(CAVLNode *pNode);
	
	///寻找前一个节点，即小于此节点的最后一个节点
	///@param pNode	目前节点
	///@return	小于此节点的最后一个节点，如果不存在此节点，则返回NULL
	CAVLNode *getPrevNode(CAVLNode *pNode);
	
	///寻找等于此对象的节点，这里的相等是指地址上相等
	///@param pObject	比较时用的对象
	///@return	等于此对象的节点，如果不存在此节点，则返回NULL
	CAVLNode *findObject(const void *pObject);

	///删除指定节点
	///@param	pNode	要删除的节点
	void removeNode(CAVLNode *pNode);
	
	///删除包含指定对象的节点
	///@param	pObject	要删除的对象
	void removeObject(const void *pObject);
	
	///全部删除
	void removeAll(void);

	///指定节点中的对象发生了变化，需要重新计算其在平衡树中的位置
	///@param	pNode	发生变化的节点
	///@return	新的节点
	CAVLNode *updateNode(CAVLNode *pNode);
	
	///指定对象发生了变化，需要重新计算其在平衡树中的位置
	///@param pObject	发生变化的对象
	///@return	新的节点
	CAVLNode *updateObject(const void *pObject);
	
	///判断目前的平衡树是否合法，此函数仅用作调试
	///@param	count	表示要检查此平衡树的节点个数是否为count，如果为-1就表示不检查
	///@return	NULL表示合法，否则输出表示错误情况的信息
	char *isValid(int count=-1);

	///获取重用编号
	///@param	本索引的重用编号
	int getReuseID(void);
private:
	///存储存储平衡树的CFixMem
	CFixMem *pIndexMem;
	
	///存储比较函数指针
	TCompareFunc compareFunc;
	
	///调整树的平衡结构，从指定节点开始向上调整
	///@param	thisNode	开始调整的节点
	void alterTree(CAVLNode *thisNode);
	
	///设置本平衡树的根
	///@param	pNode	要设置为根的节点
	void setRoot(CAVLNode *pNode);
	
	///获取本平衡树的根
	///@return	当前的根节点
	CAVLNode *getRoot(void);
};

#endif
