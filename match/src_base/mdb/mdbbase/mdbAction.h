/////////////////////////////////////////////////////////////////////////
///mdbResources.h
///定义了若干内存数据资源，可以放置于事务中
/////////////////////////////////////////////////////////////////////////

#ifndef DATABASERESOURCES_H
#define DATABASERESOURCES_H

#include "databaseData.h"
#include "MemoryStack.h"

/////////////////////////////////////////////////////////////////////////
///CCurrentTimeResource是一个说明在CCurrentTimeFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCurrentTimeResource : public CDatabaseResource
{
public:
	///构造方法
	CCurrentTimeResource(void);
	virtual ~CCurrentTimeResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CCurrentTimeFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CCurrentTimeFactory *pFactory, CCurrentTime *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CCurrentTimeFactory
	///@return	该CCurrentTimeFactory
	CCurrentTimeFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CCurrentTimeFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CCurrentTimeResource *alloc(int type, CCurrentTimeFactory *pFactory, CCurrentTime *pObject);
private:
	///存放本资源所在的对象工厂
	CCurrentTimeFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableCurrentTime writeableObject;
	
	static CMemoryStack<CCurrentTimeResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CMemberResource是一个说明在CMemberFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMemberResource : public CDatabaseResource
{
public:
	///构造方法
	CMemberResource(void);
	virtual ~CMemberResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CMemberFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CMemberFactory *pFactory, CMember *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CMemberFactory
	///@return	该CMemberFactory
	CMemberFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CMemberFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CMemberResource *alloc(int type, CMemberFactory *pFactory, CMember *pObject);
private:
	///存放本资源所在的对象工厂
	CMemberFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableMember writeableObject;
	
	static CMemoryStack<CMemberResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CServiceConfigResource是一个说明在CServiceConfigFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CServiceConfigResource : public CDatabaseResource
{
public:
	///构造方法
	CServiceConfigResource(void);
	virtual ~CServiceConfigResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CServiceConfigFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CServiceConfigFactory *pFactory, CServiceConfig *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CServiceConfigFactory
	///@return	该CServiceConfigFactory
	CServiceConfigFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CServiceConfigFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CServiceConfigResource *alloc(int type, CServiceConfigFactory *pFactory, CServiceConfig *pObject);
private:
	///存放本资源所在的对象工厂
	CServiceConfigFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableServiceConfig writeableObject;
	
	static CMemoryStack<CServiceConfigResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CUserSessionResource是一个说明在CUserSessionFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CUserSessionResource : public CDatabaseResource
{
public:
	///构造方法
	CUserSessionResource(void);
	virtual ~CUserSessionResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CUserSessionFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CUserSessionFactory *pFactory, CUserSession *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CUserSessionFactory
	///@return	该CUserSessionFactory
	CUserSessionFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CUserSessionFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CUserSessionResource *alloc(int type, CUserSessionFactory *pFactory, CUserSession *pObject);
private:
	///存放本资源所在的对象工厂
	CUserSessionFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableUserSession writeableObject;
	
	static CMemoryStack<CUserSessionResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CInstrumentResource是一个说明在CInstrumentFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CInstrumentResource : public CDatabaseResource
{
public:
	///构造方法
	CInstrumentResource(void);
	virtual ~CInstrumentResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CInstrumentFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CInstrumentFactory *pFactory, CInstrument *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CInstrumentFactory
	///@return	该CInstrumentFactory
	CInstrumentFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CInstrumentFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CInstrumentResource *alloc(int type, CInstrumentFactory *pFactory, CInstrument *pObject);
private:
	///存放本资源所在的对象工厂
	CInstrumentFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableInstrument writeableObject;
	
	static CMemoryStack<CInstrumentResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CTradingRightResource是一个说明在CTradingRightFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTradingRightResource : public CDatabaseResource
{
public:
	///构造方法
	CTradingRightResource(void);
	virtual ~CTradingRightResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CTradingRightFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CTradingRightFactory *pFactory, CTradingRight *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CTradingRightFactory
	///@return	该CTradingRightFactory
	CTradingRightFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CTradingRightFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CTradingRightResource *alloc(int type, CTradingRightFactory *pFactory, CTradingRight *pObject);
private:
	///存放本资源所在的对象工厂
	CTradingRightFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableTradingRight writeableObject;
	
	static CMemoryStack<CTradingRightResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CMarginRateResource是一个说明在CMarginRateFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarginRateResource : public CDatabaseResource
{
public:
	///构造方法
	CMarginRateResource(void);
	virtual ~CMarginRateResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CMarginRateFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CMarginRateFactory *pFactory, CMarginRate *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CMarginRateFactory
	///@return	该CMarginRateFactory
	CMarginRateFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CMarginRateFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CMarginRateResource *alloc(int type, CMarginRateFactory *pFactory, CMarginRate *pObject);
private:
	///存放本资源所在的对象工厂
	CMarginRateFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableMarginRate writeableObject;
	
	static CMemoryStack<CMarginRateResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CCFDRateResource是一个说明在CCFDRateFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCFDRateResource : public CDatabaseResource
{
public:
	///构造方法
	CCFDRateResource(void);
	virtual ~CCFDRateResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CCFDRateFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CCFDRateFactory *pFactory, CCFDRate *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CCFDRateFactory
	///@return	该CCFDRateFactory
	CCFDRateFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CCFDRateFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CCFDRateResource *alloc(int type, CCFDRateFactory *pFactory, CCFDRate *pObject);
private:
	///存放本资源所在的对象工厂
	CCFDRateFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableCFDRate writeableObject;
	
	static CMemoryStack<CCFDRateResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CFeeResource是一个说明在CFeeFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CFeeResource : public CDatabaseResource
{
public:
	///构造方法
	CFeeResource(void);
	virtual ~CFeeResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CFeeFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CFeeFactory *pFactory, CFee *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CFeeFactory
	///@return	该CFeeFactory
	CFeeFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CFeeFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CFeeResource *alloc(int type, CFeeFactory *pFactory, CFee *pObject);
private:
	///存放本资源所在的对象工厂
	CFeeFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableFee writeableObject;
	
	static CMemoryStack<CFeeResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CCurrencyResource是一个说明在CCurrencyFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CCurrencyResource : public CDatabaseResource
{
public:
	///构造方法
	CCurrencyResource(void);
	virtual ~CCurrencyResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CCurrencyFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CCurrencyFactory *pFactory, CCurrency *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CCurrencyFactory
	///@return	该CCurrencyFactory
	CCurrencyFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CCurrencyFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CCurrencyResource *alloc(int type, CCurrencyFactory *pFactory, CCurrency *pObject);
private:
	///存放本资源所在的对象工厂
	CCurrencyFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableCurrency writeableObject;
	
	static CMemoryStack<CCurrencyResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CLiquidityResource是一个说明在CLiquidityFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CLiquidityResource : public CDatabaseResource
{
public:
	///构造方法
	CLiquidityResource(void);
	virtual ~CLiquidityResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CLiquidityFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CLiquidityFactory *pFactory, CLiquidity *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CLiquidityFactory
	///@return	该CLiquidityFactory
	CLiquidityFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CLiquidityFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CLiquidityResource *alloc(int type, CLiquidityFactory *pFactory, CLiquidity *pObject);
private:
	///存放本资源所在的对象工厂
	CLiquidityFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableLiquidity writeableObject;
	
	static CMemoryStack<CLiquidityResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CAccountResource是一个说明在CAccountFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountResource : public CDatabaseResource
{
public:
	///构造方法
	CAccountResource(void);
	virtual ~CAccountResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CAccountFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CAccountFactory *pFactory, CAccount *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CAccountFactory
	///@return	该CAccountFactory
	CAccountFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CAccountFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CAccountResource *alloc(int type, CAccountFactory *pFactory, CAccount *pObject);
private:
	///存放本资源所在的对象工厂
	CAccountFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableAccount writeableObject;
	
	static CMemoryStack<CAccountResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CMemberPositionResource是一个说明在CMemberPositionFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMemberPositionResource : public CDatabaseResource
{
public:
	///构造方法
	CMemberPositionResource(void);
	virtual ~CMemberPositionResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CMemberPositionFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CMemberPositionFactory *pFactory, CMemberPosition *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CMemberPositionFactory
	///@return	该CMemberPositionFactory
	CMemberPositionFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CMemberPositionFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CMemberPositionResource *alloc(int type, CMemberPositionFactory *pFactory, CMemberPosition *pObject);
private:
	///存放本资源所在的对象工厂
	CMemberPositionFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableMemberPosition writeableObject;
	
	static CMemoryStack<CMemberPositionResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CPositionResource是一个说明在CPositionFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionResource : public CDatabaseResource
{
public:
	///构造方法
	CPositionResource(void);
	virtual ~CPositionResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CPositionFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CPositionFactory *pFactory, CPosition *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CPositionFactory
	///@return	该CPositionFactory
	CPositionFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CPositionFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CPositionResource *alloc(int type, CPositionFactory *pFactory, CPosition *pObject);
private:
	///存放本资源所在的对象工厂
	CPositionFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteablePosition writeableObject;
	
	static CMemoryStack<CPositionResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CMarketDataResource是一个说明在CMarketDataFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketDataResource : public CDatabaseResource
{
public:
	///构造方法
	CMarketDataResource(void);
	virtual ~CMarketDataResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CMarketDataFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CMarketDataFactory *pFactory, CMarketData *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CMarketDataFactory
	///@return	该CMarketDataFactory
	CMarketDataFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CMarketDataFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CMarketDataResource *alloc(int type, CMarketDataFactory *pFactory, CMarketData *pObject);
private:
	///存放本资源所在的对象工厂
	CMarketDataFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableMarketData writeableObject;
	
	static CMemoryStack<CMarketDataResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///COrderResource是一个说明在COrderFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class COrderResource : public CDatabaseResource
{
public:
	///构造方法
	COrderResource(void);
	virtual ~COrderResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的COrderFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, COrderFactory *pFactory, COrder *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的COrderFactory
	///@return	该COrderFactory
	COrderFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的COrderFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static COrderResource *alloc(int type, COrderFactory *pFactory, COrder *pObject);
private:
	///存放本资源所在的对象工厂
	COrderFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableOrder writeableObject;
	
	static CMemoryStack<COrderResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CPositionOrderResource是一个说明在CPositionOrderFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPositionOrderResource : public CDatabaseResource
{
public:
	///构造方法
	CPositionOrderResource(void);
	virtual ~CPositionOrderResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CPositionOrderFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CPositionOrderFactory *pFactory, CPositionOrder *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CPositionOrderFactory
	///@return	该CPositionOrderFactory
	CPositionOrderFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CPositionOrderFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CPositionOrderResource *alloc(int type, CPositionOrderFactory *pFactory, CPositionOrder *pObject);
private:
	///存放本资源所在的对象工厂
	CPositionOrderFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteablePositionOrder writeableObject;
	
	static CMemoryStack<CPositionOrderResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CTriggerOrderResource是一个说明在CTriggerOrderFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderResource : public CDatabaseResource
{
public:
	///构造方法
	CTriggerOrderResource(void);
	virtual ~CTriggerOrderResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CTriggerOrderFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CTriggerOrderFactory *pFactory, CTriggerOrder *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CTriggerOrderFactory
	///@return	该CTriggerOrderFactory
	CTriggerOrderFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CTriggerOrderFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CTriggerOrderResource *alloc(int type, CTriggerOrderFactory *pFactory, CTriggerOrder *pObject);
private:
	///存放本资源所在的对象工厂
	CTriggerOrderFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableTriggerOrder writeableObject;
	
	static CMemoryStack<CTriggerOrderResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CMarketOrderResource是一个说明在CMarketOrderFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CMarketOrderResource : public CDatabaseResource
{
public:
	///构造方法
	CMarketOrderResource(void);
	virtual ~CMarketOrderResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CMarketOrderFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CMarketOrderFactory *pFactory, CMarketOrder *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CMarketOrderFactory
	///@return	该CMarketOrderFactory
	CMarketOrderFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CMarketOrderFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CMarketOrderResource *alloc(int type, CMarketOrderFactory *pFactory, CMarketOrder *pObject);
private:
	///存放本资源所在的对象工厂
	CMarketOrderFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableMarketOrder writeableObject;
	
	static CMemoryStack<CMarketOrderResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CTickMarketOrderResource是一个说明在CTickMarketOrderFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTickMarketOrderResource : public CDatabaseResource
{
public:
	///构造方法
	CTickMarketOrderResource(void);
	virtual ~CTickMarketOrderResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CTickMarketOrderFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CTickMarketOrderFactory *pFactory, CTickMarketOrder *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CTickMarketOrderFactory
	///@return	该CTickMarketOrderFactory
	CTickMarketOrderFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CTickMarketOrderFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CTickMarketOrderResource *alloc(int type, CTickMarketOrderFactory *pFactory, CTickMarketOrder *pObject);
private:
	///存放本资源所在的对象工厂
	CTickMarketOrderFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableTickMarketOrder writeableObject;
	
	static CMemoryStack<CTickMarketOrderResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CTradeResource是一个说明在CTradeFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CTradeResource : public CDatabaseResource
{
public:
	///构造方法
	CTradeResource(void);
	virtual ~CTradeResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CTradeFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CTradeFactory *pFactory, CTrade *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=NULL;
		switch (type)
		{
		case CREATE_ACTION:
			memcpy(&writeableObject, pObject, sizeof(CTrade));
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CTradeFactory
	///@return	该CTradeFactory
	CTradeFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CTradeFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CTradeResource *alloc(int type, CTradeFactory *pFactory, CTrade *pObject);
private:
	///存放本资源所在的对象工厂
	CTradeFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableTrade writeableObject;
	
	static CMemoryStack<CTradeResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CKLineResource是一个说明在CKLineFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CKLineResource : public CDatabaseResource
{
public:
	///构造方法
	CKLineResource(void);
	virtual ~CKLineResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CKLineFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CKLineFactory *pFactory, CKLine *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=NULL;
		switch (type)
		{
		case CREATE_ACTION:
			memcpy(&writeableObject, pObject, sizeof(CKLine));
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CKLineFactory
	///@return	该CKLineFactory
	CKLineFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CKLineFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CKLineResource *alloc(int type, CKLineFactory *pFactory, CKLine *pObject);
private:
	///存放本资源所在的对象工厂
	CKLineFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableKLine writeableObject;
	
	static CMemoryStack<CKLineResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CPublishKLineResource是一个说明在CPublishKLineFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CPublishKLineResource : public CDatabaseResource
{
public:
	///构造方法
	CPublishKLineResource(void);
	virtual ~CPublishKLineResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CPublishKLineFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CPublishKLineFactory *pFactory, CPublishKLine *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CPublishKLineFactory
	///@return	该CPublishKLineFactory
	CPublishKLineFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CPublishKLineFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CPublishKLineResource *alloc(int type, CPublishKLineFactory *pFactory, CPublishKLine *pObject);
private:
	///存放本资源所在的对象工厂
	CPublishKLineFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteablePublishKLine writeableObject;
	
	static CMemoryStack<CPublishKLineResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CLastKLineResource是一个说明在CLastKLineFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CLastKLineResource : public CDatabaseResource
{
public:
	///构造方法
	CLastKLineResource(void);
	virtual ~CLastKLineResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CLastKLineFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CLastKLineFactory *pFactory, CLastKLine *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CLastKLineFactory
	///@return	该CLastKLineFactory
	CLastKLineFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CLastKLineFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CLastKLineResource *alloc(int type, CLastKLineFactory *pFactory, CLastKLine *pObject);
private:
	///存放本资源所在的对象工厂
	CLastKLineFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableLastKLine writeableObject;
	
	static CMemoryStack<CLastKLineResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CAccountDetailResource是一个说明在CAccountDetailFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CAccountDetailResource : public CDatabaseResource
{
public:
	///构造方法
	CAccountDetailResource(void);
	virtual ~CAccountDetailResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CAccountDetailFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CAccountDetailFactory *pFactory, CAccountDetail *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=NULL;
		switch (type)
		{
		case CREATE_ACTION:
			memcpy(&writeableObject, pObject, sizeof(CAccountDetail));
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CAccountDetailFactory
	///@return	该CAccountDetailFactory
	CAccountDetailFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CAccountDetailFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CAccountDetailResource *alloc(int type, CAccountDetailFactory *pFactory, CAccountDetail *pObject);
private:
	///存放本资源所在的对象工厂
	CAccountDetailFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableAccountDetail writeableObject;
	
	static CMemoryStack<CAccountDetailResource> resourceList;
};

/////////////////////////////////////////////////////////////////////////
///CSettleDetailResource是一个说明在CSettleDetailFactory中使用事务管理的原子资源
///@author	
///@version	1.0
/////////////////////////////////////////////////////////////////////////
class CSettleDetailResource : public CDatabaseResource
{
public:
	///构造方法
	CSettleDetailResource(void);
	virtual ~CSettleDetailResource(void);

	///初始化
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CSettleDetailFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	void init(int type, CSettleDetailFactory *pFactory, CSettleDetail *pObject)
	{
		m_type=type;
		this->pFactory=pFactory;
		this->ref=(void *)pObject;
		switch (type)
		{
		case CREATE_ACTION:
			break;
		case UPDATE_ACTION:
		case DELETE_ACTION:
			pFactory->retrieve(pObject,&writeableObject);
			break;
		default:
			EMERGENCY_EXIT("Invalid resource type");
		}	
	}

	///释放自己
	virtual void free(void);

	///获取本资源所在的CSettleDetailFactory
	///@return	该CSettleDetailFactory
	CSettleDetailFactory *getFactory(void)
	{
		return pFactory;
	}

	///完成提交
	virtual void commit(void);
	
	///完成滚回
	virtual void rollback(void);
	
	///获取优先级，优先级可以控制commit时的次序
	///@return	本资源的优先级，0表示最高，1表示正常，1表示最低
	virtual int getPriority(void);

	///申请一个有指定内容的资源
	///@param	type	资源类型，应当是CREATE_ACTION、DELETE_ACTION或UPDATE_ACTION
	///@param	pFactory	本资源所在的CSettleDetailFactory
	///@param	pObject	本资源的地址
	///@param	pWriteableObject	新的对象值，当type为UPDATE_ACTION时有效，在目前的实现中没有用处
	///@return	返回一个有指定内容的资源
	static CSettleDetailResource *alloc(int type, CSettleDetailFactory *pFactory, CSettleDetail *pObject);
private:
	///存放本资源所在的对象工厂
	CSettleDetailFactory *pFactory;
	
	///存放本资源修改前的值，当type为CREATE_ACTION和DELETE_ACTION时，本项无意义
	CWriteableSettleDetail writeableObject;
	
	static CMemoryStack<CSettleDetailResource> resourceList;
};

#endif
