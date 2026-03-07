#ifndef _DATA_JsonAnalysis_H_
#define _DATA_JsonAnalysis_H_

#include "FieldMap.h"

class CJsonAnalysis
{
public:
	CJsonAnalysis() {};
	virtual ~CJsonAnalysis() {};
	CFieldDescribe* FromJsonToStruct(const char* packageName, const void *pInput, int nInputLen, void *pOutput, int *nTNo);
	CFieldDescribe* FromJsonToStruct(const char* packageName, const char* fieldName, const void *pInput, int nInputLen, void *pOutput, int *nTNo);
};

#endif
