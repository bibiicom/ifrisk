/////////////////////////////////////////////////////////////////////////
///CResource.h
///定义了类CResource
/////////////////////////////////////////////////////////////////////////

#ifndef CRESOURCE_H
#define CRESOURCE_H

#define RESOURCE_MERGE_OK	1
#define RESOURCE_MERGE_FAIL_THIS 2
#define RESOURCE_MERGE_FAIL_ALL 3
#define RESOURCE_MERGE_STOP 4            //增加一种合并情况

/////////////////////////////////////////////////////////////////////////
///CResource是一个抽象类，用于说明一个可以使用事务管理的资源
///@author	tom
///@version	1.0,20020228
/////////////////////////////////////////////////////////////////////////
class CResource
{
protected:
	void *ref;
public:
	///析构函数
	virtual ~CResource(void)
	{
	}
	
	///完成提交
	virtual void commit(void)=0;
	
	///完成滚回
	virtual void rollback(void)=0;

	///获取本资源的引用，用于唯一标识一个资源
	///@return	用于标识一个资源
	const void *getRef(void)
	{
		return ref;
	}
	
	///释放自己
	virtual void free(void)
	{
		delete this;
	}

	///与另一个资源合并
	///@param	pResource	要合并的另一个资源
	///@return	RESOURCE_MERGE_OK表示合并成功，
	///			RESOURCE_MERGE_FAIL_THIS表示本条合并失败，但是还可以尝试其他的
	///			RESOURCE_MERGE_FAIL_ALL表示合并全部失败，其他的也不用尝试了
	virtual int merge(CResource *pResource)=0;

	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void)
	{
		return 1;
	}

	///是否控制资源，目前只有savepoint是控制资源
	///@return	true表示是，false表示否
	virtual bool isControl(void)
	{
		return false;
	}
};

#endif
