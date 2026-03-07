#include "CBaseObject.h"
#include "public.h"

FILE *errorOutput=stderr;
char g_cMDBRunLevel = '0';

CBaseObject::CBaseObject(void)
{
}

CBaseObject::~CBaseObject(void)
{
}

int CBaseObject::isA(char *objectType)
{
	if (!strcmp(objectType,"CBaseObject"))
		return 1;
	return 0;
}

const char *CBaseObject::getType(void)
{
	return "CBaseObject";
}

void CBaseObject::checkType(char *objectType, char *filename, int line)
{
	if (!isA(objectType))
	{
//		throw (new CDesignError("Invalid Object",filename,line));
		EMERGENCY_EXIT("Invalid Object");
	}
}
/*
void CBaseObject::output(CLogger *pLogger,int indent)
{
	pLogger->output(indent,0,"CBaseObject");
}
*/
