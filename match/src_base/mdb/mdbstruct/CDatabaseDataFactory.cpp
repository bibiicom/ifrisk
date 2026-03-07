#include "CDatabaseDataFactory.h"


CDatabaseDataFactory::CDatabaseDataFactory(int unitSize, int maxTradeUnit,IMemoryAllocator *pAllocator,bool reuse, int reuseID, int readOnly)
{
	if(maxTradeUnit == 0)	///如果为0，则认为是缺省值
		maxTradeUnit = MDB_DEFAULT_BLOCK_SIZE;
	pMem=new CFixMem(unitSize,maxTradeUnit,pAllocator,reuse,reuseID,readOnly);
	if (pMem==NULL)
		EMERGENCY_EXIT("Insufficient memory space");
	this->maxTradeUnit=maxTradeUnit;
}

CDatabaseDataFactory::~CDatabaseDataFactory(void)
{
	CHECK_TYPE("CDatabaseDataFactory");
	delete pMem;
}

int CDatabaseDataFactory::isA(char *objectType)
{
	if (!strcmp(objectType,"CDatabaseDataFactory"))
		return 1;
	return CBaseObject::isA(objectType);
}

const char *CDatabaseDataFactory::getType(void)
{
	return "CDatabaseDataFactory";
}

//void CDatabaseDataFactory::output(CLogger *pLogger,int indent)
//{
//	CHECK_TYPE("CDatabaseDataFactory");
//	pLogger->output(indent,0,"CDatabaseDataFactory:max_unit=%d",maxTradeUnit);
//	pMem->output(pLogger,indent+1);
//}

void CDatabaseDataFactory::clearAll(void)
{
	pMem->freeAll();
}

int	CDatabaseDataFactory::getReuseID()
{
	return pMem->GetReuseID();
}
