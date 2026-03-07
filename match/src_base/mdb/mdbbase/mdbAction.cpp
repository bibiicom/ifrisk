/////////////////////////////////////////////////////////////////////////
///mdbResources.cpp
///实现了若干内存数据资源，可以放置于事务中
/////////////////////////////////////////////////////////////////////////

#include "mdbAction.h"


CMemoryStack<CCurrentTimeResource> CCurrentTimeResource::resourceList;

CCurrentTimeResource * CCurrentTimeResource::alloc(int type, CCurrentTimeFactory *pFactory, CCurrentTime *pObject)
{
	CCurrentTimeResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CCurrentTimeResource::free(void)
{
	resourceList.free(this);
}

CCurrentTimeResource::CCurrentTimeResource(void)
{
}

CCurrentTimeResource::~CCurrentTimeResource(void)
{
}

void CCurrentTimeResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CCurrentTime *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CCurrentTime *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CCurrentTimeResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CCurrentTime *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CCurrentTime *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CCurrentTimeResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CMemberResource> CMemberResource::resourceList;

CMemberResource * CMemberResource::alloc(int type, CMemberFactory *pFactory, CMember *pObject)
{
	CMemberResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CMemberResource::free(void)
{
	resourceList.free(this);
}

CMemberResource::CMemberResource(void)
{
}

CMemberResource::~CMemberResource(void)
{
}

void CMemberResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CMember *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CMember *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CMemberResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CMember *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CMember *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CMemberResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CServiceConfigResource> CServiceConfigResource::resourceList;

CServiceConfigResource * CServiceConfigResource::alloc(int type, CServiceConfigFactory *pFactory, CServiceConfig *pObject)
{
	CServiceConfigResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CServiceConfigResource::free(void)
{
	resourceList.free(this);
}

CServiceConfigResource::CServiceConfigResource(void)
{
}

CServiceConfigResource::~CServiceConfigResource(void)
{
}

void CServiceConfigResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CServiceConfig *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CServiceConfig *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CServiceConfigResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CServiceConfig *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CServiceConfig *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CServiceConfigResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CUserSessionResource> CUserSessionResource::resourceList;

CUserSessionResource * CUserSessionResource::alloc(int type, CUserSessionFactory *pFactory, CUserSession *pObject)
{
	CUserSessionResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CUserSessionResource::free(void)
{
	resourceList.free(this);
}

CUserSessionResource::CUserSessionResource(void)
{
}

CUserSessionResource::~CUserSessionResource(void)
{
}

void CUserSessionResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CUserSession *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CUserSession *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CUserSessionResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CUserSession *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CUserSession *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CUserSessionResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CInstrumentResource> CInstrumentResource::resourceList;

CInstrumentResource * CInstrumentResource::alloc(int type, CInstrumentFactory *pFactory, CInstrument *pObject)
{
	CInstrumentResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CInstrumentResource::free(void)
{
	resourceList.free(this);
}

CInstrumentResource::CInstrumentResource(void)
{
}

CInstrumentResource::~CInstrumentResource(void)
{
}

void CInstrumentResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CInstrument *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CInstrument *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CInstrumentResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CInstrument *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CInstrument *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CInstrumentResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CTradingRightResource> CTradingRightResource::resourceList;

CTradingRightResource * CTradingRightResource::alloc(int type, CTradingRightFactory *pFactory, CTradingRight *pObject)
{
	CTradingRightResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CTradingRightResource::free(void)
{
	resourceList.free(this);
}

CTradingRightResource::CTradingRightResource(void)
{
}

CTradingRightResource::~CTradingRightResource(void)
{
}

void CTradingRightResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CTradingRight *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CTradingRight *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CTradingRightResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CTradingRight *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CTradingRight *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CTradingRightResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CMarginRateResource> CMarginRateResource::resourceList;

CMarginRateResource * CMarginRateResource::alloc(int type, CMarginRateFactory *pFactory, CMarginRate *pObject)
{
	CMarginRateResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CMarginRateResource::free(void)
{
	resourceList.free(this);
}

CMarginRateResource::CMarginRateResource(void)
{
}

CMarginRateResource::~CMarginRateResource(void)
{
}

void CMarginRateResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CMarginRate *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CMarginRate *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CMarginRateResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CMarginRate *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CMarginRate *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CMarginRateResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CCFDRateResource> CCFDRateResource::resourceList;

CCFDRateResource * CCFDRateResource::alloc(int type, CCFDRateFactory *pFactory, CCFDRate *pObject)
{
	CCFDRateResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CCFDRateResource::free(void)
{
	resourceList.free(this);
}

CCFDRateResource::CCFDRateResource(void)
{
}

CCFDRateResource::~CCFDRateResource(void)
{
}

void CCFDRateResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CCFDRate *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CCFDRate *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CCFDRateResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CCFDRate *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CCFDRate *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CCFDRateResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CFeeResource> CFeeResource::resourceList;

CFeeResource * CFeeResource::alloc(int type, CFeeFactory *pFactory, CFee *pObject)
{
	CFeeResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CFeeResource::free(void)
{
	resourceList.free(this);
}

CFeeResource::CFeeResource(void)
{
}

CFeeResource::~CFeeResource(void)
{
}

void CFeeResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CFee *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CFee *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CFeeResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CFee *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CFee *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CFeeResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CCurrencyResource> CCurrencyResource::resourceList;

CCurrencyResource * CCurrencyResource::alloc(int type, CCurrencyFactory *pFactory, CCurrency *pObject)
{
	CCurrencyResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CCurrencyResource::free(void)
{
	resourceList.free(this);
}

CCurrencyResource::CCurrencyResource(void)
{
}

CCurrencyResource::~CCurrencyResource(void)
{
}

void CCurrencyResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CCurrency *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CCurrency *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CCurrencyResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CCurrency *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CCurrency *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CCurrencyResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CLiquidityResource> CLiquidityResource::resourceList;

CLiquidityResource * CLiquidityResource::alloc(int type, CLiquidityFactory *pFactory, CLiquidity *pObject)
{
	CLiquidityResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CLiquidityResource::free(void)
{
	resourceList.free(this);
}

CLiquidityResource::CLiquidityResource(void)
{
}

CLiquidityResource::~CLiquidityResource(void)
{
}

void CLiquidityResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CLiquidity *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CLiquidity *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CLiquidityResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CLiquidity *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CLiquidity *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CLiquidityResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CAccountResource> CAccountResource::resourceList;

CAccountResource * CAccountResource::alloc(int type, CAccountFactory *pFactory, CAccount *pObject)
{
	CAccountResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CAccountResource::free(void)
{
	resourceList.free(this);
}

CAccountResource::CAccountResource(void)
{
}

CAccountResource::~CAccountResource(void)
{
}

void CAccountResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CAccount *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CAccount *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CAccountResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CAccount *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CAccount *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CAccountResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CMemberPositionResource> CMemberPositionResource::resourceList;

CMemberPositionResource * CMemberPositionResource::alloc(int type, CMemberPositionFactory *pFactory, CMemberPosition *pObject)
{
	CMemberPositionResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CMemberPositionResource::free(void)
{
	resourceList.free(this);
}

CMemberPositionResource::CMemberPositionResource(void)
{
}

CMemberPositionResource::~CMemberPositionResource(void)
{
}

void CMemberPositionResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CMemberPosition *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CMemberPosition *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CMemberPositionResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CMemberPosition *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CMemberPosition *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CMemberPositionResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CPositionResource> CPositionResource::resourceList;

CPositionResource * CPositionResource::alloc(int type, CPositionFactory *pFactory, CPosition *pObject)
{
	CPositionResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CPositionResource::free(void)
{
	resourceList.free(this);
}

CPositionResource::CPositionResource(void)
{
}

CPositionResource::~CPositionResource(void)
{
}

void CPositionResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CPosition *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CPosition *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CPositionResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CPosition *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CPosition *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CPositionResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CMarketDataResource> CMarketDataResource::resourceList;

CMarketDataResource * CMarketDataResource::alloc(int type, CMarketDataFactory *pFactory, CMarketData *pObject)
{
	CMarketDataResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CMarketDataResource::free(void)
{
	resourceList.free(this);
}

CMarketDataResource::CMarketDataResource(void)
{
}

CMarketDataResource::~CMarketDataResource(void)
{
}

void CMarketDataResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CMarketData *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CMarketData *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CMarketDataResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CMarketData *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CMarketData *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CMarketDataResource::getPriority(void)
{
	return 2;
}


CMemoryStack<COrderResource> COrderResource::resourceList;

COrderResource * COrderResource::alloc(int type, COrderFactory *pFactory, COrder *pObject)
{
	COrderResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void COrderResource::free(void)
{
	resourceList.free(this);
}

COrderResource::COrderResource(void)
{
}

COrderResource::~COrderResource(void)
{
}

void COrderResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((COrder *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((COrder *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void COrderResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((COrder *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((COrder *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int COrderResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CPositionOrderResource> CPositionOrderResource::resourceList;

CPositionOrderResource * CPositionOrderResource::alloc(int type, CPositionOrderFactory *pFactory, CPositionOrder *pObject)
{
	CPositionOrderResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CPositionOrderResource::free(void)
{
	resourceList.free(this);
}

CPositionOrderResource::CPositionOrderResource(void)
{
}

CPositionOrderResource::~CPositionOrderResource(void)
{
}

void CPositionOrderResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CPositionOrder *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CPositionOrder *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CPositionOrderResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CPositionOrder *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CPositionOrder *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CPositionOrderResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CTriggerOrderResource> CTriggerOrderResource::resourceList;

CTriggerOrderResource * CTriggerOrderResource::alloc(int type, CTriggerOrderFactory *pFactory, CTriggerOrder *pObject)
{
	CTriggerOrderResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CTriggerOrderResource::free(void)
{
	resourceList.free(this);
}

CTriggerOrderResource::CTriggerOrderResource(void)
{
}

CTriggerOrderResource::~CTriggerOrderResource(void)
{
}

void CTriggerOrderResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CTriggerOrder *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CTriggerOrder *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CTriggerOrderResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CTriggerOrder *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CTriggerOrder *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CTriggerOrderResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CMarketOrderResource> CMarketOrderResource::resourceList;

CMarketOrderResource * CMarketOrderResource::alloc(int type, CMarketOrderFactory *pFactory, CMarketOrder *pObject)
{
	CMarketOrderResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CMarketOrderResource::free(void)
{
	resourceList.free(this);
}

CMarketOrderResource::CMarketOrderResource(void)
{
}

CMarketOrderResource::~CMarketOrderResource(void)
{
}

void CMarketOrderResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CMarketOrder *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CMarketOrder *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CMarketOrderResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CMarketOrder *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CMarketOrder *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CMarketOrderResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CTickMarketOrderResource> CTickMarketOrderResource::resourceList;

CTickMarketOrderResource * CTickMarketOrderResource::alloc(int type, CTickMarketOrderFactory *pFactory, CTickMarketOrder *pObject)
{
	CTickMarketOrderResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CTickMarketOrderResource::free(void)
{
	resourceList.free(this);
}

CTickMarketOrderResource::CTickMarketOrderResource(void)
{
}

CTickMarketOrderResource::~CTickMarketOrderResource(void)
{
}

void CTickMarketOrderResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CTickMarketOrder *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CTickMarketOrder *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CTickMarketOrderResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CTickMarketOrder *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CTickMarketOrder *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CTickMarketOrderResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CTradeResource> CTradeResource::resourceList;

CTradeResource * CTradeResource::alloc(int type, CTradeFactory *pFactory, CTrade *pObject)
{
	CTradeResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CTradeResource::free(void)
{
	resourceList.free(this);
}

CTradeResource::CTradeResource(void)
{
}

CTradeResource::~CTradeResource(void)
{
}

void CTradeResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd(&writeableObject);
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}	
}
	
void CTradeResource::rollback(void)
{
}

int CTradeResource::getPriority(void)
{
	return 3;
}


CMemoryStack<CKLineResource> CKLineResource::resourceList;

CKLineResource * CKLineResource::alloc(int type, CKLineFactory *pFactory, CKLine *pObject)
{
	CKLineResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CKLineResource::free(void)
{
	resourceList.free(this);
}

CKLineResource::CKLineResource(void)
{
}

CKLineResource::~CKLineResource(void)
{
}

void CKLineResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd(&writeableObject);
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}	
}
	
void CKLineResource::rollback(void)
{
}

int CKLineResource::getPriority(void)
{
	return 3;
}


CMemoryStack<CPublishKLineResource> CPublishKLineResource::resourceList;

CPublishKLineResource * CPublishKLineResource::alloc(int type, CPublishKLineFactory *pFactory, CPublishKLine *pObject)
{
	CPublishKLineResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CPublishKLineResource::free(void)
{
	resourceList.free(this);
}

CPublishKLineResource::CPublishKLineResource(void)
{
}

CPublishKLineResource::~CPublishKLineResource(void)
{
}

void CPublishKLineResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CPublishKLine *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CPublishKLine *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CPublishKLineResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CPublishKLine *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CPublishKLine *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CPublishKLineResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CLastKLineResource> CLastKLineResource::resourceList;

CLastKLineResource * CLastKLineResource::alloc(int type, CLastKLineFactory *pFactory, CLastKLine *pObject)
{
	CLastKLineResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CLastKLineResource::free(void)
{
	resourceList.free(this);
}

CLastKLineResource::CLastKLineResource(void)
{
}

CLastKLineResource::~CLastKLineResource(void)
{
}

void CLastKLineResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CLastKLine *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CLastKLine *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CLastKLineResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CLastKLine *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CLastKLine *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CLastKLineResource::getPriority(void)
{
	return 2;
}


CMemoryStack<CAccountDetailResource> CAccountDetailResource::resourceList;

CAccountDetailResource * CAccountDetailResource::alloc(int type, CAccountDetailFactory *pFactory, CAccountDetail *pObject)
{
	CAccountDetailResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CAccountDetailResource::free(void)
{
	resourceList.free(this);
}

CAccountDetailResource::CAccountDetailResource(void)
{
}

CAccountDetailResource::~CAccountDetailResource(void)
{
}

void CAccountDetailResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd(&writeableObject);
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}	
}
	
void CAccountDetailResource::rollback(void)
{
}

int CAccountDetailResource::getPriority(void)
{
	return 3;
}


CMemoryStack<CSettleDetailResource> CSettleDetailResource::resourceList;

CSettleDetailResource * CSettleDetailResource::alloc(int type, CSettleDetailFactory *pFactory, CSettleDetail *pObject)
{
	CSettleDetailResource *pResource=resourceList.alloc();
	pResource->init(type,pFactory,pObject);
	return pResource;
}

void CSettleDetailResource::free(void)
{
	resourceList.free(this);
}

CSettleDetailResource::CSettleDetailResource(void)
{
}

CSettleDetailResource::~CSettleDetailResource(void)
{
}

void CSettleDetailResource::commit(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->commitAdd((CSettleDetail *)ref);
		break;
	case UPDATE_ACTION:
		pFactory->commitUpdate((CSettleDetail *)ref,&writeableObject);	
		break;
	case DELETE_ACTION:
		pFactory->commitRemove(&writeableObject);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}
	
void CSettleDetailResource::rollback(void)
{
	switch (m_type)
	{
	case CREATE_ACTION:
		pFactory->internalRemove((CSettleDetail *)ref,false);
		break;
	case UPDATE_ACTION:
		pFactory->internalUpdate((CSettleDetail *)ref,&writeableObject,false);
		break;
	case DELETE_ACTION:
		pFactory->internalAdd(&writeableObject,false);
		break;
	case NONE_ACTION:
		break;
	default:
		EMERGENCY_EXIT("Invalid resource type");
	}
}

int CSettleDetailResource::getPriority(void)
{
	return 3;
}

