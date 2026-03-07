#ifndef _CSVTOFIEDL_H__
#define _CSVTOFIEDL_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "FieldDescribe.h"

const int DBMT_RECORD_MAX_SIZE = MAX_FIELD_SIZE * 2;

class CFieldAnalysis  
{
public:
	CFieldAnalysis();
	virtual ~CFieldAnalysis();
	bool Analysis(const char *pszFieldName, const char *pszFieldContent);
	inline int GetFieldCount();
	inline const char *GetFieldName(int nIndex);
	inline const char *GetFieldContent(int nIndex);

	bool AnalysisFieldName(const char *pszFieldName);
	bool AnalysisFieldContent(const char *pszFieldContent);

	bool TransToStruct(CFieldDescribe *pFieldDesc, char *pBuffer);

	const char *GetFieldAsString(const char *pszFieldName);
	int GetFieldAsInt(const char *pszFieldName);
	double GetFieldAsDouble(const char *pszFieldName);
	double GetFieldAsLong(const char *pszFieldName);
private:
	char *AppendNameToken(char *pszToken);
	char *AppendContentToken(const char *pszToken);
private:
	struct TDbmtField
	{
		char *pFieldName;
		char *pFieldContent;
	};

	vector<TDbmtField> m_dbmtFields;

	struct ltstr
	{
	  bool operator()(const char* s1, const char* s2) const
	  {
		return strcmp(s1, s2) < 0;
	  }
	};
	typedef map<const char*, const char *, ltstr> CDbmtFieldMap;
	CDbmtFieldMap m_mapDbmtField;

	char m_chNameBuffer[DBMT_RECORD_MAX_SIZE];
	int m_nNameBufferLen;
	char m_chContentBuffer[DBMT_RECORD_MAX_SIZE];
	int m_nContentBufferLen;
};

inline int CFieldAnalysis::GetFieldCount()
{
	return (int)m_dbmtFields.size();
}

inline const char * CFieldAnalysis::GetFieldName(int nIndex)
{
	return m_dbmtFields[nIndex-1].pFieldName;
}

inline const char * CFieldAnalysis::GetFieldContent(int nIndex)
{
	return m_dbmtFields[nIndex-1].pFieldContent;
}

#endif
