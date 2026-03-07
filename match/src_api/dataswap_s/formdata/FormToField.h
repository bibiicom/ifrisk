#ifndef _DATA_FormToField_H_
#define _DATA_FormToField_H_

#include "FieldMap.h"

class CFormAnalysis
{
public:
	CFormAnalysis(){};
	virtual ~CFormAnalysis(){};
	CFieldDescribe* FormToStruct(const char* packageName,const void *pInput, int nInputLen, void *pOutput,int *nTNo);
	CFieldDescribe* FormToStruct(const char* packageName,const char* fieldName,const void *pInput, int nInputLen, void *pOutput,int *nTNo);
};

#endif
