#include "CDatabaseResource.h"
#include "public.h"

CDatabaseResource::CDatabaseResource(int type)
{
	m_type = type;
	if ((type != CREATE_ACTION) && (type != UPDATE_ACTION) && (type != DELETE_ACTION))
		EMERGENCY_EXIT("Invalid resource type");
}

int CDatabaseResource::merge(CResource *pResource)
{
	///本资源和新的资源的标识相同的资源，那一定是数据库资源，而且可能需要合并
	CDatabaseResource *pDatabaseResource;
	pDatabaseResource = (CDatabaseResource *)pResource;

	//对NONE_ACTION动作不合并
	if (NONE_ACTION == pDatabaseResource->m_type)
		return RESOURCE_MERGE_STOP;

	switch (m_type)
	{
	case CREATE_ACTION:
		switch (pDatabaseResource->m_type)
		{
		case CREATE_ACTION:
			EMERGENCY_EXIT("impossible create action after create action");
			break;
		case UPDATE_ACTION:
			///原来是CREATE，那新的还是CREATE，不需要进行任何处理
			///插入后修改，还是插入
			return RESOURCE_MERGE_OK;
		case DELETE_ACTION:
			///如果原来是CREATE，那任何其他值已无意义，所以不用处理
			///插入后删除，结果是空操作
			m_type = NONE_ACTION;
			return RESOURCE_MERGE_OK;
		default:
			EMERGENCY_EXIT("Invalid resource action");
		}
		break;
	case UPDATE_ACTION:
		switch (pDatabaseResource->m_type)
		{
		case CREATE_ACTION:
			EMERGENCY_EXIT("impossible create action after update action");
			break;
		case UPDATE_ACTION:
			///修改后修改，还是修改
			return RESOURCE_MERGE_OK;
		case DELETE_ACTION:
			///修改后删除，改为删除操作
			m_type = DELETE_ACTION;
			return RESOURCE_MERGE_OK;
		default:
			EMERGENCY_EXIT("Invalid resource action");
		}
		break;
	case DELETE_ACTION:
		switch (pDatabaseResource->m_type)
		{
		case CREATE_ACTION:
			//在已经删除的位置上又创建了一个新对象，这是允许的，但不需要合并
			return RESOURCE_MERGE_FAIL_THIS;
		case UPDATE_ACTION:
			EMERGENCY_EXIT("impossible update after delete action");
			break;
		case DELETE_ACTION:
			EMERGENCY_EXIT("impossible delete after delete action");
			break;
		default:
			EMERGENCY_EXIT("Invalid resource action");
		}
		break;
	case NONE_ACTION:	//注意，NONE只有一种情况下会出现，那就是在创建后删除
		switch (pDatabaseResource->m_type)
		{
		case CREATE_ACTION:
			//在已经删除的位置上又创建了一个新对象，这是允许的，但不需要合并
			return RESOURCE_MERGE_FAIL_THIS;
		case UPDATE_ACTION:
			EMERGENCY_EXIT("impossible update after delete action");
			break;
		case DELETE_ACTION:
			EMERGENCY_EXIT("impossible delete after delete action");
			break;
		default:
			EMERGENCY_EXIT("Invalid resource action");
		}
		break;
	}
	EMERGENCY_EXIT("Invalid datebase resource type");
	return RESOURCE_MERGE_FAIL_THIS;
}
