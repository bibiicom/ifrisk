#ifndef _TABLE_DEFINE_H_
#define _TABLE_DEFINE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "platform.h"
#include "FieldDescribe.h"
#include "FieldData.h"
#include "CSVToField.h"

#define MAX_LINE_LEGTH	50000

struct TTinitTableInfo
{
    char *pszTable;
	CFieldDescribe *pFieldDescribe;
};

extern TTinitTableInfo g_InitTableInfo[];
extern TTinitTableInfo g_AllTableInfo[];

class CTinitTableCallback
{
public:
	virtual bool SendTable(CFieldDescribe *pFieldDesc, const void *pRspBuffer) = 0;
};

bool LoadTable(const char *pszCSVFileName, CFieldDescribe *pFieldDesc, CTinitTableCallback* pTinitTableCallback);

#endif
