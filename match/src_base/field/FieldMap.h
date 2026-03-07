#ifndef AFX_XTPPACKAGEDESC_H__
#define AFX_XTPPACKAGEDESC_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "FieldData.h"
#include "HashMap.h"

///获取报单的名称
///@param	tid	报单编号
///@return	报单名称
const char *getPackageName(int tid);
const char *getShortPackageName(int tid);

#define UNLIMITED_FIELD_OCCUR 1000
#define MAX_PACKAGE_NAME_LEN 32
#define MAX_COMMENT_LEN		100
typedef struct
{
	int fid;
	CFieldDescribe *pFieldDescribe;
	char fieldName[MAX_PACKAGE_NAME_LEN];
	char fieldShortName[MAX_PACKAGE_NAME_LEN];
	char fieldApiName[MAX_PACKAGE_NAME_LEN];
	char comment[MAX_COMMENT_LEN];
}	TFieldUse;

typedef struct
{
	int tid;
	char packageName[MAX_PACKAGE_NAME_LEN];
	char comment[MAX_COMMENT_LEN];
}	TPackageDefine;

//extern TPackageDefine XTPPackageDefines[];
class CXTPPackageDefineMap: public CHashMap<int, TPackageDefine *, HashInt>
{
public:
	CXTPPackageDefineMap(TPackageDefine *pXTPPackageDefines, int nDefineCount)
	{
		for(int i=0; i<nDefineCount; i++)
		{
			Insert(pXTPPackageDefines->tid, pXTPPackageDefines);
			pXTPPackageDefines++;
		}
	}
};
class CXTPPackageDefineNameMap: public map<string, TPackageDefine *>
{
public:
	CXTPPackageDefineNameMap(TPackageDefine *pXTPPackageDefines, int nDefineCount)
	{
		for(int i=0; i<nDefineCount; i++)
		{
			insert(CXTPPackageDefineNameMap::value_type(pXTPPackageDefines->packageName, pXTPPackageDefines));
			pXTPPackageDefines++;
		}
	}
};

//extern TFieldUse FieldUses[];
class CFieldUseMap: public CHashMap<int, TFieldUse *, HashInt>
{
public:
	CFieldUseMap(TFieldUse *pFieldUses, int nDefineCount)
	{
		for(int i=0; i<nDefineCount; i++)
		{
			Insert(pFieldUses->fid, pFieldUses);
			pFieldUses++;
		}
	}
};

class CFieldNameMap : public map<string, TFieldUse *>
{
public:
	CFieldNameMap(TFieldUse *pFieldUses, int nDefineCount)
	{
		for (int i = 0; i < nDefineCount; i++)
		{
			insert(CFieldNameMap::value_type(pFieldUses->fieldName, pFieldUses));
			pFieldUses++;
		}
	}
};

extern CXTPPackageDefineMap g_XTPPackageDefineMap;
extern CXTPPackageDefineNameMap g_XTPPackageDefineNameMap;
extern CFieldUseMap g_FieldUseMap;
extern CFieldNameMap g_FieldNameMap;

#endif
