#include "public.h"
#include "CSVToField.h"
#include "CSVParser.h"

CFieldAnalysis::CFieldAnalysis()
{
	m_nNameBufferLen = 0;
	m_nContentBufferLen = 0;
}

CFieldAnalysis::~CFieldAnalysis()
{

}

bool CFieldAnalysis::Analysis(const char *pszFieldName, const char *pszFieldContent)
{
	if (!AnalysisFieldName(pszFieldName))
	{
		return false;
	}
	return AnalysisFieldContent(pszFieldContent);
}

char *CFieldAnalysis::AppendNameToken(char *pszToken)
{
	int nTokenLen = (int)strlen(pszToken) + 1;
	if (pszToken[nTokenLen - 2] == '*')
	{
		pszToken[nTokenLen - 2] = '\0';
		nTokenLen--;
	}

	char *pszTarget = m_chNameBuffer + m_nNameBufferLen;
	memcpy(m_chNameBuffer + m_nNameBufferLen, pszToken, nTokenLen);
	m_nNameBufferLen += nTokenLen;
	return pszTarget;
}

char *CFieldAnalysis::AppendContentToken(const char *pszToken)
{
	int nTokenLen = (int)strlen(pszToken) + 1;
	char *pszTarget = m_chContentBuffer + m_nContentBufferLen;
	memcpy(m_chContentBuffer + m_nContentBufferLen, pszToken, nTokenLen);
	m_nContentBufferLen += nTokenLen;
	return pszTarget;
}

bool CFieldAnalysis::AnalysisFieldName(const char *pszFieldName)
{
	m_nNameBufferLen = 0;
	m_dbmtFields.clear();
	m_mapDbmtField.clear();

	CCSVParser csvParser(pszFieldName);
	TDbmtField field = { NULL, NULL };

	do
	{
		char *pszToken = csvParser.GetNextToken();
		if (pszToken == NULL)
		{
			break;
		}
		field.pFieldName = AppendNameToken(pszToken);
		m_dbmtFields.push_back(field);
	} while (csvParser.GetErrorCode() == CPE_HAS_NEXT);

	return true;
}

bool CFieldAnalysis::AnalysisFieldContent(const char *pszFieldContent)
{
	m_nContentBufferLen = 0;

	CCSVParser csvParser(pszFieldContent);
	for (int i = 0; i < m_dbmtFields.size(); i++)
	{
		char *pszToken = csvParser.GetNextToken();
		if (pszToken == NULL)
		{
			break;
		}
		m_dbmtFields[i].pFieldContent = AppendContentToken(pszToken);
		m_mapDbmtField[m_dbmtFields[i].pFieldName] = m_dbmtFields[i].pFieldContent;
	}

	return true;
}

const char * CFieldAnalysis::GetFieldAsString(const char *pszFieldName)
{
	CDbmtFieldMap::iterator itor = m_mapDbmtField.find(pszFieldName);
	if (itor == m_mapDbmtField.end())
		return NULL;
	return (*itor).second;
}

int CFieldAnalysis::GetFieldAsInt(const char *pszFieldName)
{
	const char *pszFieldContent = GetFieldAsString(pszFieldName);
	if (pszFieldContent == NULL)
		return INT_32_MAX;
	if (*pszFieldContent == '\0')
		return INT_32_MAX;
	return atoi(pszFieldContent);
}

double CFieldAnalysis::GetFieldAsDouble(const char *pszFieldName)
{
	const char *pszFieldContent = GetFieldAsString(pszFieldName);
	if (pszFieldContent == NULL)
		return DOUBLE_NaN;
	if (*pszFieldContent == '\0')
		return DOUBLE_NaN;
	return atof(pszFieldContent);
}

double CFieldAnalysis::GetFieldAsLong(const char *pszFieldName)
{
	const char *pszFieldContent = GetFieldAsString(pszFieldName);
	if (pszFieldContent == NULL)
		return DOUBLE_NaN;
	if (*pszFieldContent == '\0')
		return DOUBLE_NaN;
	return atof(pszFieldContent);
}

bool CFieldAnalysis::TransToStruct(CFieldDescribe *pFieldDesc, char *pBuffer)
{
	bool bResult = true;
	for (int i = 0; i < pFieldDesc->GetMemberCount(); i++)
	{
		bool bRet = true;
		TMemberDesc *pMemberDesc = pFieldDesc->GetMemberDesc(i);
		if (GetFieldAsString(pMemberDesc->szName) == NULL)
		{
			printf(" :Can't find [%s] of [%s]", pMemberDesc->szName, pFieldDesc->GetFieldName());
			bRet = false;
			bResult = false;
			//如果不做动作，默认应该是0x00
			//continue;
		}
		char *pMember = pBuffer + pMemberDesc->nStructOffset;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:
		{
			if (bRet)
				*((int *)pMember) = GetFieldAsInt(pMemberDesc->szName);
			else
				*((int *)pMember) = INT_32_MAX;
			break;
		}
		case FT_BYTE:
		{
			if (bRet)
			{
				memset(pMember, 0, pMemberDesc->nSize);
				memcpy(pMember, GetFieldAsString(pMemberDesc->szName), pMemberDesc->nSize);
				if (pMemberDesc->nSize != 1)
				{
					pMember[pMemberDesc->nSize - 1] = '\0';
					char *p = pMember + strlen(pMember);
					while (p > pMember)
					{
						p--;
						if (*p != ' ')
							break;
						*p = '\0';
					}
				}
			}
			else
			{
				memset(pMember, 0, pMemberDesc->nSize);
			}
			break;
		}
		case FT_REAL8:
		{
			if (bRet)
				*((double *)pMember) = GetFieldAsDouble(pMemberDesc->szName);
			else
				*((double *)pMember) = DOUBLE_NaN;
		}
		case FT_LONG:
		{
			if (bRet)
				*((UF_INT8 *)pMember) = GetFieldAsDouble(pMemberDesc->szName);
			else
				*((UF_INT8 *)pMember) = DOUBLE_NaN;
		}
		break;
		}
	}
	if (!bResult)
		printf(" ,Loss Some Member  ...Continue\n");

	return bResult;
}

#if 0
int main()
{
	char *pszFieldName = "name,age,money";
	char *pszFieldContent = "\"peter pan\",\"20\",\"123.5\"";
	CFieldAnalysis record;
	if (!record.Analysis(pszFieldName, pszFieldContent))
	{
		printf("Analysis fail\n");
		return -1;
	}

	int i = 0;
	for (i = 1; i <= record.GetFieldCount(); i++)
	{
		printf("[%s] : [%s]\n", record.GetFieldName(i), record.GetFieldContent(i));
	}

	printf("name = [%s]\n", record.GetFieldAsString("name"));
	printf("age = [%d]\n", record.GetFieldAsInt("age"));
	printf("money = [%lf]\n", record.GetFieldAsDouble("money"));

	printf("error str = [%s]\n", record.GetFieldAsString("error str"));
	printf("error int = [%d]\n", record.GetFieldAsInt("error int"));
	printf("error double = [%lf]\n", record.GetFieldAsDouble("error double"));

	return 0;
}
#endif