#include "FieldDescribe.h"
#include "BaseFunction.h"

CFieldDescribe::CFieldDescribe(int FieldID, int nStructSize, const char *pszFieldName, const char *pszComment, describeFunc func)
{
	m_FieldID = FieldID;
	strncpy(m_szFieldName, pszFieldName, sizeof(m_szFieldName) - 1);
	//strcpy(m_szComment, pszComment);
	m_nStructSize = nStructSize;
	m_nStreamSize = 0;
	m_nTotalMember = 0;
	func();
}

CFieldDescribe::~CFieldDescribe()
{

}

//将对象转换为字节流
//@param pStruct 要转换的对象
//@param pStream 转换出的字节流
//@remark 字节流中的成员变量是高位在前
void CFieldDescribe::StructToStream(char *pStream, const char *pStruct)
{
	//	if(bInSys)
	//	{
	//		memcpy(pStream,pStruct,m_nStructSize);
	//		return;
	//	}

	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *p = &m_MemberDesc[i];
#ifndef _BIG_ENDIAN_						//低位在前
		switch (p->nType)
		{
		case FT_DWORD:						//int
			ChangeEndianCopy4(pStream + p->nStreamOffset, pStruct + p->nStructOffset);
			break;
		case FT_LONG:
		case FT_REAL8:						//double
			ChangeEndianCopy8(pStream + p->nStreamOffset, pStruct + p->nStructOffset);
			break;
		case FT_BYTE:
			memcpy(pStream + p->nStreamOffset, pStruct + p->nStructOffset, p->nSize);
			break;
		}
#else										//高位在前
		memcpy(pStream + p->nStreamOffset, pStruct + p->nStructOffset, p->nSize);
#endif
	}
}

//将字节流转换为对象
//@param pStruct 转换的出对象
//@param pStream 要转换的字节流
//@remark 字节流中的成员变量必须是高位在前
void CFieldDescribe::StreamToStruct(char *pStruct, const char *pStream)
{
	//	if(bInSys)
	//	{
	//		memcpy(pStruct,pStream,m_nStructSize);
	//		return;
	//	}
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *p = &m_MemberDesc[i];
#ifndef _BIG_ENDIAN_						//低位在前
		switch (p->nType)
		{
		case FT_DWORD:						//int
			ChangeEndianCopy4(pStruct + p->nStructOffset, pStream + p->nStreamOffset);
			break;
		case FT_LONG:
		case FT_REAL8:						//double
			ChangeEndianCopy8(pStruct + p->nStructOffset, pStream + p->nStreamOffset);
			break;
		case FT_BYTE:
			memcpy(pStruct + p->nStructOffset, pStream + p->nStreamOffset, p->nSize);
			break;
		}
#else										//高位在前
		memcpy(pStruct + p->nStructOffset, pStream + p->nStreamOffset, p->nSize);
#endif
	}
}

bool CFieldDescribe::StreamToOutput(char* pOutput, const char *pStream)
{
#ifdef _BIG_ENDIAN_
	return StructToOutput(pOutput, pStream);
#else
	StreamToStruct(m_szFieldTmp, pStream);
	return StructToOutput(pOutput, m_szFieldTmp);
#endif
}

bool CFieldDescribe::StreamToCSV(char* pCSV, const char *pStream)
{
#ifdef _BIG_ENDIAN_
	return StructToCSV(pCSV, pStream);
#else
	StreamToStruct(m_szFieldTmp, pStream);
	return StructToCSV(pCSV, m_szFieldTmp);
#endif
}

bool CFieldDescribe::StreamToSingleCSV(char* pCSV, const char *pStream)
{
#ifdef _BIG_ENDIAN_
	return StructToSingleCSV(pCSV, pStream);
#else
	StreamToStruct(m_szFieldTmp, pStream);
	return StructToSingleCSV(pCSV, m_szFieldTmp);
#endif
}

bool CFieldDescribe::StreamToJson(char* pJson, const char *pStream)
{
#ifdef _BIG_ENDIAN_
	return StructToJson(pJson, pStream);
#else
	StreamToStruct(m_szFieldTmp, pStream);
	return StructToJson(pJson, m_szFieldTmp);
#endif
}

bool CFieldDescribe::StructToOutput(char* pOutput, const char *pStruct)
{
	sprintf(pOutput, "\tFIELD [%s] START\n", m_szFieldName);
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		sprintf(pOutput + strlen(pOutput), "\t\t[%s][%d]", pMemberDesc->szName, pMemberDesc->nType & 0xFF);
		const char *pMember = pStruct + pMemberDesc->nStructOffset;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:
		{
			if (*((int *)pMember) != INT_32_MAX)
				sprintf(pOutput + strlen(pOutput), "[%d]\n", *((int *)pMember));
			else
				sprintf(pOutput + strlen(pOutput), "[]\n");
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				if (isprint(*pMember))
					sprintf(pOutput + strlen(pOutput), "[%c]\n", *pMember);
				else
					sprintf(pOutput + strlen(pOutput), "[0X%02X]\n", *pMember & 0xFF);
			}
			else
			{
				sprintf(pOutput + strlen(pOutput), "[%s]\n", pMember);
			}
			break;
		}
		case FT_REAL8:
		{
			string value = doubleToStringFormat(*((double*)pMember));
			sprintf(pOutput + strlen(pOutput), "[%s]\n", value.c_str());
			break;
		}
		case FT_LONG:
		{
			string value = LargeNumberToString(*((double*)pMember));
			sprintf(pOutput + strlen(pOutput), "[%s]\n", value.c_str());
			break;
		}
		}
	}
	sprintf(pOutput + strlen(pOutput), "\tFIELD [%s] END", m_szFieldName);
	return true;
}

bool CFieldDescribe::StructToCSV(char* pCSV, const char *pStruct)
{
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		strcat(pCSV, "\"");
		strcat(pCSV, pMemberDesc->szName);
		strcat(pCSV, "\",\"");
		char strTemp[2] = { '\0', '\0' };
		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:					//long
		{
			if (*((int *)pMemberData) != INT_32_MAX)
				sprintf(pCSV + strlen(pCSV), "%ld", *((int *)pMemberData));
			break;
		}
		case FT_REAL8:						//double
		{
			string value = doubleToStringFormat(*((double*)pMemberData));
			strcat(pCSV, value.c_str());
			break;
		}
		case FT_LONG:						//double
		{
			string value = LargeNumberToString(*((double*)pMemberData));
			strcat(pCSV, value.c_str());
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				if (isprint(*pMemberData))
					strTemp[0] = *pMemberData;
				else
					strTemp[0] = '\0';
				strcat(pCSV, strTemp);
			}
			else
			{
				strcat(pCSV, pMemberData);
			}
			break;
		}
		}
		strcat(pCSV, "\"");
	}
	return true;
}

bool CFieldDescribe::StructToSingleCSV(char* pCSV, const char *pStruct)
{
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		strcat(pCSV, "\"");
		char strTemp[2] = { '\0', '\0' };
		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:					//long
		{
			if (*((int *)pMemberData) != INT_32_MAX)
				sprintf(pCSV + strlen(pCSV), "%ld", *((int *)pMemberData));
			break;
		}
		case FT_REAL8:						//double
		{
			string value = doubleToStringFormat(*((double*)pMemberData));
			strcat(pCSV, value.c_str());
			break;
		}
		case FT_LONG:						//double
		{
			string value = LargeNumberToString(*((double*)pMemberData));
			strcat(pCSV, value.c_str());
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				if (isprint(*pMemberData))
					strTemp[0] = *pMemberData;
				else
					strTemp[0] = '\0';
				strcat(pCSV, strTemp);
			}
			else
			{
				strcat(pCSV, pMemberData);
			}
			break;
		}
		}
		strcat(pCSV, "\"");
	}
	return true;
}

bool CFieldDescribe::StructToShortJsonS(char* pJson, const char *pStruct)
{
	pJson[0] = '\0';
	char* fastNext = pJson;
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
		if (pMemberDesc->sShort[0] == '\0')
			continue;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:						//long
		{
			if (*((int *)pMemberData) != INT_32_MAX)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
				fastNext = fastStrcat(fastNext, "\":\"");
				fastNext = fastStrcat(fastNext, *((int *)pMemberData));
				fastNext = fastStrcat(fastNext, "\"");
			}
			break;
		}
		case FT_REAL8:						//double
		{
			if ((*((double*)pMemberData)) != DOUBLE_NaN)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
				fastNext = fastStrcat(fastNext, "\":\"");
				fastNext = fastStrcat(fastNext, doubleToString(*((double*)pMemberData), pMemberDesc->nPrecision).c_str());
				fastNext = fastStrcat(fastNext, "\"");
			}
			break;
		}
		case FT_LONG:						//double
		{
			if ((*((double*)pMemberData)) != DOUBLE_NaN)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
				fastNext = fastStrcat(fastNext, "\":\"");
				fastNext = fastStrcat(fastNext, LargeNumberToString(*((double*)pMemberData)).c_str());
				fastNext = fastStrcat(fastNext, "\"");
			}
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				const char cData = *pMemberData;
				if (*pMemberData != '\0' && isprint(cData))
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
					fastNext = fastStrcat(fastNext, "\":\"");
					fastNext = fastStrcat(fastNext, cData);
					fastNext = fastStrcat(fastNext, "\"");
				}
			}
			else
			{
				if (*pMemberData != '\0')
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
					fastNext = fastStrcat(fastNext, "\":\"");
					fastNext = fastStrcat(fastNext, pMemberData);
					fastNext = fastStrcat(fastNext, "\"");
				}
			}
			break;
		}
		}
	}
	return true;
}

bool CFieldDescribe::StructToShortJson(char* pJson, const char *pStruct)
{
	pJson[0] = '\0';
	char* fastNext = pJson;
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
		if (pMemberDesc->sShort[0] == '\0')
			continue;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:						//long
		{
			if (*((int *)pMemberData) != INT_32_MAX)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
				fastNext = fastStrcat(fastNext, "\":");
				fastNext = fastStrcat(fastNext, *((int *)pMemberData));
			}
			break;
		}
		case FT_REAL8:						//double
		{
			if ((*((double*)pMemberData)) != DOUBLE_NaN)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
				fastNext = fastStrcat(fastNext, "\":");
				fastNext = fastStrcat(fastNext, doubleToString(*((double*)pMemberData), pMemberDesc->nPrecision).c_str());
			}
			break;
		}
		case FT_LONG:						//double
		{
			if ((*((double*)pMemberData)) != DOUBLE_NaN)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
				fastNext = fastStrcat(fastNext, "\":");
				fastNext = fastStrcat(fastNext, LargeNumberToString(*((double*)pMemberData)).c_str());
			}
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				const char cData = *pMemberData;
				if (*pMemberData != '\0' && isprint(cData))
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
					fastNext = fastStrcat(fastNext, "\":\"");
					fastNext = fastStrcat(fastNext, cData);
					fastNext = fastStrcat(fastNext, "\"");
				}
			}
			else
			{
				if (*pMemberData != '\0')
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
					fastNext = fastStrcat(fastNext, "\":\"");
					fastNext = fastStrcat(fastNext, pMemberData);
					fastNext = fastStrcat(fastNext, "\"");
				}
			}
			break;
		}
		}
	}
	return true;
}

bool CFieldDescribe::StructToShortJsonC(char* pJson, const char *pStruct)
{
	pJson[0] = '\0';
	char* fastNext = pJson;
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
		if (pMemberDesc->sShort[0] == '\0')
			continue;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:						//long
		{
			if (*((int *)pMemberData) != INT_32_MAX)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
				fastNext = fastStrcat(fastNext, ":");
				fastNext = fastStrcat(fastNext, *((int *)pMemberData));
			}
			break;
		}
		case FT_REAL8:						//double
		{
			if ((*((double*)pMemberData)) != DOUBLE_NaN)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
				fastNext = fastStrcat(fastNext, ":");
				fastNext = fastStrcat(fastNext, doubleToString(*((double*)pMemberData), pMemberDesc->nPrecision).c_str());
			}
			break;
		}
		case FT_LONG:						//double
		{
			if ((*((double*)pMemberData)) != DOUBLE_NaN)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
				fastNext = fastStrcat(fastNext, ":");
				fastNext = fastStrcat(fastNext, LargeNumberToString(*((double*)pMemberData)).c_str());
			}
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				const char cData = *pMemberData;
				if (*pMemberData != '\0' && isprint(cData))
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
					fastNext = fastStrcat(fastNext, ":");
					fastNext = fastStrcat(fastNext, cData);
				}
			}
			else
			{
				if (*pMemberData != '\0')
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, pMemberDesc->sShort);
					fastNext = fastStrcat(fastNext, ":");
					fastNext = fastStrcat(fastNext, pMemberData);
				}
			}
			break;
		}
		}
	}
	return true;
}

bool CFieldDescribe::StructToJsonS(char* pJson, const char *pStruct)
{
	pJson[0] = '\0';
	char* fastNext = pJson;
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
		if (pMemberDesc->sApi[0] == '\0')
			continue;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:						//long
		{
			if (*((int *)pMemberData) != INT_32_MAX)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":\"");
				fastNext = fastStrcat(fastNext, *((int *)pMemberData));
				fastNext = fastStrcat(fastNext, "\"");
			}
			else
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":null");
			}
			break;
		}
		case FT_REAL8:						//double
		{
			string value = doubleToString(*((double*)pMemberData), pMemberDesc->nPrecision);
			if (value.size() > 0)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":\"");
				fastNext = fastStrcat(fastNext, value.c_str());
				fastNext = fastStrcat(fastNext, "\"");
			}
			else
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":null");
			}
			break;
		}
		case FT_LONG:						//double
		{
			string value = LargeNumberToString(*((double*)pMemberData));
			if (value.size() > 0)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":\"");
				fastNext = fastStrcat(fastNext, value.c_str());
				fastNext = fastStrcat(fastNext, "\"");
			}
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":null");
			}
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				const char cData = *pMemberData;
				if (isprint(cData))
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
					fastNext = fastStrcat(fastNext, "\":\"");
					fastNext = fastStrcat(fastNext, cData);
					fastNext = fastStrcat(fastNext, "\"");
				}
				else
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
					fastNext = fastStrcat(fastNext, "\":\"\"");
				}
			}
			else
			{
				if (*pMemberData != '\0')
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
					fastNext = fastStrcat(fastNext, "\":\"");
					fastNext = fastStrcat(fastNext, pMemberData);
					fastNext = fastStrcat(fastNext, "\"");
				}
				else
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
					fastNext = fastStrcat(fastNext, "\":\"\"");
				}
			}
			break;
		}
		}
	}
	return true;
}

//bool CFieldDescribe::StructToJson(char* pJson, const char *pStruct)
//{
//	pJson[0] = '\0';
//	for (int i = 0; i < m_nTotalMember; i++)
//	{
//		TMemberDesc *pMemberDesc = GetMemberDesc(i);
//		if (i > 0)
//			strcat(pJson, ",");
//
//		strcat(pJson, "\"");
//		strcat(pJson, pMemberDesc->szName);
//		strcat(pJson, "\":");
//
//		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
//		switch (pMemberDesc->nType)
//		{
//		case FT_DWORD:						//long
//		{
//			if (*((int *)pMemberData) != INT_32_MAX)
//				sprintf(pJson + strlen(pJson), "%d", *((int *)pMemberData));
//			else
//				sprintf(pJson + strlen(pJson), "null");
//			break;
//		}
//		case FT_REAL8:						//double
//		{
//			if ((*((double*)pMemberData)) != DOUBLE_NaN)
//				strcat(pJson, doubleToString(*((double*)pMemberData), pMemberDesc->nPrecision).c_str());
//			else
//				strcat(pJson, "null");
//			break;
//		}
//		case FT_LONG:						//double
//		{
//			if ((*((double*)pMemberData)) != DOUBLE_NaN)
//				strcat(pJson, LargeNumberToString(*((double*)pMemberData)).c_str());
//			else
//				strcat(pJson, "null");
//			break;
//		}
//		case FT_BYTE:
//		{
//			if (pMemberDesc->nSize == 1)
//			{
//				//char tmp = *pMemberData;
//				//sprintf(pJson + strlen(pJson), "%d", (int)tmp);
//
//				if (isprint(*pMemberData))
//					sprintf(pJson + strlen(pJson), "\"%c\"", *pMemberData);
//				else
//					sprintf(pJson + strlen(pJson), "null");
//			}
//			else
//			{
//				strcat(pJson, "\"");
//				strcat(pJson, pMemberData);
//				strcat(pJson, "\"");
//			}
//			break;
//		}
//		}
//	}
//	return true;
//}

bool CFieldDescribe::StructToJson(char* pJson, const char *pStruct)
{
	pJson[0] = '\0';
	char* fastNext = pJson;
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
		if (pMemberDesc->sApi[0] == '\0')
			continue;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:						//long
		{
			if (*((int *)pMemberData) != INT_32_MAX)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":");
				fastNext = fastStrcat(fastNext, *((int *)pMemberData));
			}
			else
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":null");
			}
			break;
		}
		case FT_REAL8:						//double
		{
			string value = doubleToString(*((double*)pMemberData), pMemberDesc->nPrecision);
			if (value.size() > 0)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":");
				fastNext = fastStrcat(fastNext, value.c_str());
			}
			else
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":null");
			}
			break;
		}
		case FT_LONG:						//double
		{
			string value = LargeNumberToString(*((double*)pMemberData));
			if (value.size() > 0)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":");
				fastNext = fastStrcat(fastNext, value.c_str());
			}
			else
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
				fastNext = fastStrcat(fastNext, "\":null");
			}
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				char cData = *pMemberData;
				if (isprint(cData))
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
					fastNext = fastStrcat(fastNext, "\":\"");
					fastNext = fastStrcat(fastNext, cData);
					fastNext = fastStrcat(fastNext, "\"");
				}
				else
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
					fastNext = fastStrcat(fastNext, "\":\"\"");
				}
			}
			else
			{
				if (*pMemberData != '\0')
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
					fastNext = fastStrcat(fastNext, "\":\"");
					fastNext = fastStrcat(fastNext, pMemberData);
					fastNext = fastStrcat(fastNext, "\"");
				}
				else
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberDesc->sApi);
					fastNext = fastStrcat(fastNext, "\":\"\"");
				}
			}
			break;
		}
		}
	}
	return true;
}

bool CFieldDescribe::StructToJsonArray(char* pJson, const char *pStruct)
{
	pJson[0] = '\0';
	char* fastNext = pJson;
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
		if (pMemberDesc->sShort[0] == '\0')
			continue;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:						//long
		{
			if (*((int *)pMemberData) != INT_32_MAX)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, *((int *)pMemberData));
			}
			break;
		}
		case FT_REAL8:						//double
		{
			string value = doubleToString(*((double*)pMemberData), pMemberDesc->nPrecision);
			if (value.size() > 0)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, value.c_str());
			}
			break;
		}
		case FT_LONG:						//double
		{
			string value = LargeNumberToString(*((double*)pMemberData));
			if (value.size() > 0)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, value.c_str());
			}
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				char cData = *pMemberData;
				if (isprint(cData))
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, cData);
					fastNext = fastStrcat(fastNext, "\"");
				}
			}
			else
			{
				if (*pMemberData != '\0')
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberData);
					fastNext = fastStrcat(fastNext, "\"");
				}
			}
			break;
		}
		}
	}
	return true;
}

bool CFieldDescribe::StructToJsonArrayS(char* pJson, const char *pStruct)
{
	pJson[0] = '\0';
	char* fastNext = pJson;
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
		if (pMemberDesc->sShort[0] == '\0')
			continue;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:						//long
		{
			if (*((int *)pMemberData) != INT_32_MAX)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, *((int *)pMemberData));
				fastNext = fastStrcat(fastNext, "\"");
			}
			break;
		}
		case FT_REAL8:						//double
		{
			string value = doubleToString(*((double*)pMemberData), pMemberDesc->nPrecision);
			if (value.size() > 0)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, value.c_str());
				fastNext = fastStrcat(fastNext, "\"");
			}
			break;
		}
		case FT_LONG:						//double
		{
			string value = LargeNumberToString(*((double*)pMemberData));
			if (value.size() > 0)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, "\"");
				fastNext = fastStrcat(fastNext, value.c_str());
				fastNext = fastStrcat(fastNext, "\"");
			}
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				char cData = *pMemberData;
				if (isprint(cData))
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, cData);
					fastNext = fastStrcat(fastNext, "\"");
				}
			}
			else
			{
				if (*pMemberData != '\0')
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, "\"");
					fastNext = fastStrcat(fastNext, pMemberData);
					fastNext = fastStrcat(fastNext, "\"");
				}
			}
			break;
		}
		}
	}
	return true;
}

bool CFieldDescribe::StructToJsonArrayC(char* pJson, const char *pStruct)
{
	pJson[0] = '\0';
	char* fastNext = pJson;
	for (int i = 0; i < m_nTotalMember; i++)
	{
		TMemberDesc *pMemberDesc = GetMemberDesc(i);
		char *pMemberData = (char *)pStruct + pMemberDesc->nStructOffset;
		if (pMemberDesc->sShort[0] == '\0')
			continue;
		switch (pMemberDesc->nType)
		{
		case FT_DWORD:						//long
		{
			if (*((int *)pMemberData) != INT_32_MAX)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, *((int *)pMemberData));
			}
			break;
		}
		case FT_REAL8:						//double
		{
			string value = doubleToString(*((double*)pMemberData), pMemberDesc->nPrecision);
			if (value.size() > 0)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, value.c_str());
			}
			break;
		}
		case FT_LONG:						//double
		{
			string value = LargeNumberToString(*((double*)pMemberData));
			if (value.size() > 0)
			{
				if (fastNext != pJson)
					fastNext = fastStrcat(fastNext, ",");
				fastNext = fastStrcat(fastNext, value.c_str());
			}
			break;
		}
		case FT_BYTE:
		{
			if (pMemberDesc->nSize == 1)
			{
				char cData = *pMemberData;
				if (isprint(cData))
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, cData);
				}
			}
			else
			{
				if (*pMemberData != '\0')
				{
					if (fastNext != pJson)
						fastNext = fastStrcat(fastNext, ",");
					fastNext = fastStrcat(fastNext, pMemberData);
				}
			}
			break;
		}
		}
	}
	return true;
}

int CFieldDescribe::getSingleField(void *pField, const void *pStream)
{
	//memset(pField,0,sizeof(pField));
	StreamToStruct((char *)(pField), (char *)pStream);
	return m_nStreamSize;
}

int CFieldDescribe::setSingleField(void *pStream, const void *pField)
{
	//memset(pStream,0,sizeof(pStream));
	StructToStream((char *)pStream, (char *)(pField));
	return m_nStreamSize;
}