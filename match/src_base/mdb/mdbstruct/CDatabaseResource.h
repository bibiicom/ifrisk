/////////////////////////////////////////////////////////////////////////
///CDatabaseResource.h
///定义了类CDatabaseResource
/////////////////////////////////////////////////////////////////////////

#ifndef CDATABASERESOURCE_H
#define CDATABASERESOURCE_H

#include "platform.h"
#include "CBaseObject.h"
#include "CResource.h"

#define CREATE_ACTION 1
#define DELETE_ACTION 2
#define UPDATE_ACTION 4
#define NONE_ACTION 0

/////////////////////////////////////////////////////////////////////////
///CDatabaseResource是一个说明在数据库中使用的事务
///@author	tom
///@version	1.0,20060126
/////////////////////////////////////////////////////////////////////////
class CDatabaseResource: public CResource
{
public:
	///构造方法
	CDatabaseResource(void)
	{
	}

	///构造方法
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	updateIndex	true表示需要更新索引，false表示不需要
	CDatabaseResource(int type);

	///获取本资源的类型
	///@return	CREATE_ACTION、DELTE_ACTION、UPDATE_ACTION或者NONE_ACTION
	int getResourceType(void)
	{
		return m_type;
	}

	///与另一个资源合并
	///@param	pResource	要合并的另一个资源
	///@return	RESOURCE_MERGE_OK表示合并成功，
	///			RESOURCE_MERGE_FAIL_THIS表示本条合并失败，但是还可以尝试其他的
	///			RESOURCE_MERGE_FAIL_ALL表示合并全部失败，其他的也不用尝试了
	virtual int merge(CResource *pResource);
protected:
	///存放资源类型
	///对于不同类型的资源，需要指定不同的引用（getRef的返回值），
	///操作				保存时
	///CREATE_ACTION	创建的对象
	///UPDATE_ACTION	修改的对象
	///DELETE_ACTION	删除的对象
	///NONE_ACTION		NULL
	int m_type;
};

#endif
