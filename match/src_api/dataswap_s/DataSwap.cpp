#include "DataSwap.h"
#include "BaseFunction.h"
#include "FieldMap.h"

CDataSwitchEngine::CDataSwitchEngine()
{
}

CDataSwitchEngine::~CDataSwitchEngine(void)
{
}

bool CDataSwitchEngine::ReqField(const TOutFogMetaRef* pReqRef, const void *pStream)
{
	return true;
}

bool CDataSwitchEngine::SyncReqField(const TOutFogMetaRef* pReqRef, const void *pReqBuffer, TOutFogMetaRef* pRspRef, void *pRspBuffer)
{
	pRspRef->Length = 0;
	switch (pReqRef->IsReplay)
	{
	case 1:
	{
		const char* pData = Replay1(pReqRef, (const char*)pReqBuffer, pRspRef);
		TOutFogMetaRef theNext;
		memcpy(&theNext, pReqRef, sizeof(TOutFogMetaRef));
		theNext.Length = pRspRef->Length;
		return Swap(&theNext, pData, pRspRef, pRspBuffer);
	}
	case 2:
	{
		Replay2(pReqRef, (const char*)pReqBuffer, pRspRef, (char*)pRspBuffer);
		break;
	}
	case 3:
	{
		switch (pReqRef->FrontNo)
		{
		case 1:
		case 2:
		case 3:
			sprintf((char*)pRspBuffer, "{\"a\":\"%s\",\"n\":%d,\"c\":%d,\"m\":\"\%s\"}", pReqRef->SubIndexName, pReqRef->RequestNo, pReqRef->SubjectNo, pReqBuffer);
			break;
		case 4:
			sprintf((char*)pRspBuffer, "{a:%s,n:%d,c:%d,m:%s}", pReqRef->SubIndexName, pReqRef->RequestNo, pReqRef->SubjectNo, pReqBuffer);
			break;
		default:
			sprintf((char*)pRspBuffer, "{\"action\":\"%s\",\"requestNo\":%d,\"errorCode\":%d,\"errorMsg\":\"%s\"}", pReqRef->SubIndexName, pReqRef->RequestNo, pReqRef->SubjectNo, pReqBuffer);
		}
		pRspRef->Length = strlen((char*)pRspBuffer);
		break;
	}
	default:
	{
		return Swap(pReqRef, (const char*)pReqBuffer, pRspRef, pRspBuffer);
	}
	}
	return true;
}

bool CDataSwitchEngine::Swap(const TOutFogMetaRef* pReqRef, const void *pReqBuffer, TOutFogMetaRef* pRspRef, void *pRspBuffer)
{
	switch (pReqRef->Direction)
	{
	case OUTFOG_DATAFORMAT_JSON:
	{
		switch (pRspRef->Direction)
		{
		case OUTFOG_DATAFORMAT_STREAM:
		{
			//memcpy(m_pStreamBuf,pReqBuffer,pReqRef->Length);
			//memset(m_pStreamBuf, 0, sizeof(m_pStreamBuf));
			char m_pStreamBuf[4 * OUTFOG_MAX_PACKAGE_SIZE] = { 0 };
			//m_pStreamBuf[pReqRef->Length]='\0';
			CFieldDescribe* pFieldDescribe = NULL;
			if (pReqRef->Type == OUTFOG_TYPE_REQUEST)
				pFieldDescribe = m_JsonAnalysis.FromJsonToStruct(pRspRef->SubIndexName, pReqBuffer, pReqRef->Length, m_pStreamBuf, &pRspRef->MessageNo);
			else
			{
				const char* fieldName = pReqRef->UserID;
				pFieldDescribe = m_JsonAnalysis.FromJsonToStruct(pRspRef->SubIndexName, fieldName, pReqBuffer, pReqRef->Length, m_pStreamBuf, &pRspRef->MessageNo);
			}
			if (pFieldDescribe != NULL)
			{
				pRspRef->FieldNo = pFieldDescribe->m_FieldID;
				pRspRef->Length = pFieldDescribe->GetStreamSize();
				pFieldDescribe->setSingleField(pRspBuffer, &m_pStreamBuf);
				return true;
			}
			else
			{
				sprintf((char*)pRspBuffer, "Error:%s", (char *)m_pStreamBuf);
				return false;
			}
		}
		default:
		{
			return false;
		}
		}
	}
	case OUTFOG_DATAFORMAT_FORMDATA:
	{
		switch (pRspRef->Direction)
		{
		case OUTFOG_DATAFORMAT_STREAM:
		{
			//memcpy(m_pStreamBuf,pReqBuffer,pReqRef->Length);
			//memset(m_pStreamBuf, 0, sizeof(m_pStreamBuf));
			char m_pStreamBuf[4 * OUTFOG_MAX_PACKAGE_SIZE] = { 0 };
			//m_pStreamBuf[pReqRef->Length]='\0';
			CFieldDescribe* pFieldDescribe = NULL;
			if (pReqRef->Type == OUTFOG_TYPE_REQUEST)
				pFieldDescribe = m_FormAnalysis.FormToStruct(pRspRef->SubIndexName, pReqBuffer, pReqRef->Length, m_pStreamBuf, &pRspRef->MessageNo);
			else
			{
				const char* fieldName = pReqRef->UserID;
				pFieldDescribe = m_FormAnalysis.FormToStruct(pRspRef->SubIndexName, fieldName, pReqBuffer, pReqRef->Length, m_pStreamBuf, &pRspRef->MessageNo);
			}
			if (pFieldDescribe != NULL)
			{
				pRspRef->FieldNo = pFieldDescribe->m_FieldID;
				pRspRef->Length = pFieldDescribe->GetStreamSize();
				pFieldDescribe->setSingleField(pRspBuffer, &m_pStreamBuf);
				return true;
			}
			else
			{
				sprintf((char*)pRspBuffer, "Error:%s", (char *)m_pStreamBuf);
				return false;
			}
		}
		default:
		{
			return false;
		}
		}
	}
	case OUTFOG_DATAFORMAT_STREAM:
	{
		switch (pRspRef->Direction)
		{
		case OUTFOG_DATAFORMAT_JSON:
		{
			short nFieldNo = pReqRef->FieldNo;
			if (nFieldNo < 0)
			{
				if (pReqRef->FrontNo != 0)
					return false;
				nFieldNo = -nFieldNo;
			}
			TFieldUse **pFieldUse = g_FieldUseMap.Find(nFieldNo);
			if (pFieldUse == NULL)
				return false;
			char m_pStreamBuf[4 * OUTFOG_MAX_PACKAGE_SIZE] = { 0 };
			CFieldDescribe *pFieldDescribe = (*pFieldUse)->pFieldDescribe;		
			pFieldDescribe->StreamToStruct(m_pStreamBuf, (char *)pReqBuffer);
			switch (pReqRef->FrontNo)
			{
			case 1:
			case 2:
			{
				pFieldDescribe->StructToShortJsonS((char *)pRspBuffer, m_pStreamBuf);
				strncpy(pRspRef->SubIndexName, (*pFieldUse)->fieldShortName, sizeof(pRspRef->SubIndexName));
				break;
			}
			case 3:
			{
				pFieldDescribe->StructToJsonArray((char *)pRspBuffer, m_pStreamBuf);
				strncpy(pRspRef->SubIndexName, (*pFieldUse)->fieldShortName, sizeof(pRspRef->SubIndexName));
				break;
			}
			case 4:
			{
				pFieldDescribe->StructToJsonArrayC((char *)pRspBuffer, m_pStreamBuf);
				strncpy(pRspRef->SubIndexName, (*pFieldUse)->fieldShortName, sizeof(pRspRef->SubIndexName));
				break;
			}
			default:
			{
				pFieldDescribe->StructToJsonS((char *)pRspBuffer, m_pStreamBuf);
				strncpy(pRspRef->SubIndexName, (*pFieldUse)->fieldApiName, sizeof(pRspRef->SubIndexName));
			}
			}
			return true;
		}
		case OUTFOG_DATAFORMAT_OUTPUT:
		{
			TFieldUse **pFieldUse = g_FieldUseMap.Find(pReqRef->FieldNo);
			if (pFieldUse == NULL)
			{
				sprintf((char *)pRspBuffer, "FIELD[%d],SIZE[%d] NO FOUND", pReqRef->FieldNo, pReqRef->Length);
				return false;
			}
			CFieldDescribe *pFieldDescribe = (*pFieldUse)->pFieldDescribe;
			pFieldDescribe->StreamToOutput((char *)pRspBuffer, (char *)pReqBuffer);
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
	case OUTFOG_DATAFORMAT_STRUCT:
	{
		switch (pRspRef->Direction)
		{
		case OUTFOG_DATAFORMAT_JSON:
		{
			short nFieldNo = pReqRef->FieldNo;
			if (nFieldNo < 0)
			{
				if (pReqRef->FrontNo != 0)
					return false;
				nFieldNo = -nFieldNo;
			}

			TFieldUse **pFieldUse = g_FieldUseMap.Find(nFieldNo);
			if (pFieldUse == NULL)
				return false;
			CFieldDescribe *pFieldDescribe = (*pFieldUse)->pFieldDescribe;
			int nZip = pReqRef->FrontNo % 10;
			switch (nZip)
			{
			case 1:
			{
				pFieldDescribe->StructToShortJsonS((char *)pRspBuffer, (char *)pReqBuffer);
				strncpy(pRspRef->SubIndexName, (*pFieldUse)->fieldShortName, sizeof(pRspRef->SubIndexName));
				break;
			}
			case 3:
			{
				pFieldDescribe->StructToJsonArray((char *)pRspBuffer, (char *)pReqBuffer);
				strncpy(pRspRef->SubIndexName, (*pFieldUse)->fieldShortName, sizeof(pRspRef->SubIndexName));
				break;
			}
			case 4:
			{
				pFieldDescribe->StructToJsonArrayC((char *)pRspBuffer, (char *)pReqBuffer);
				strncpy(pRspRef->SubIndexName, (*pFieldUse)->fieldShortName, sizeof(pRspRef->SubIndexName));
				break;
			}
			default:
			{
				pFieldDescribe->StructToJsonS((char *)pRspBuffer, (char *)pReqBuffer);
				strncpy(pRspRef->SubIndexName, (*pFieldUse)->fieldApiName, sizeof(pRspRef->SubIndexName));
			}
			}
			return true;
		}
		case OUTFOG_DATAFORMAT_OUTPUT:
		{
			short nFieldNo = pReqRef->FieldNo;
			if (nFieldNo < 0)
				nFieldNo = -nFieldNo;

			TFieldUse **pFieldUse = g_FieldUseMap.Find(nFieldNo);

			if (pFieldUse == NULL)
			{
				sprintf((char *)pRspBuffer, "FIELD[%d],SIZE[%d] NO FOUND", nFieldNo, pReqRef->Length);
				return false;
			}
			CFieldDescribe *pFieldDescribe = (*pFieldUse)->pFieldDescribe;
			pFieldDescribe->StructToOutput((char *)pRspBuffer, (char *)pReqBuffer);
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
	default:
	{
		return false;
	}
	}
}

const char* CDataSwitchEngine::Replay1(const TOutFogMetaRef* pReqRef, const char *pReqBuffer, TOutFogMetaRef* pRspRef)
{
	//传入的是一个json格式: {"requestNo":1,"A":{"a":1,"b":2,...}}，标记序号，用于区别
	//或者 {"A":{"a":1,"b":2,...}}，不标记序号，用于区别
	//先提取A 和 {"a":1,"b":2,...}
	//A作为操作类型，{"a":1,"b":2,...}作为操作数据

	int i = 1;
	int j = 0;
	pRspRef->RequestNo = 0;
	if (pReqBuffer[0] == '\'')
		i++;
	while (pReqBuffer[i] != ':')
	{
		if (pReqBuffer[i] == '\0')
			break;
		if (pReqBuffer[i] != '\"')
		{
			pRspRef->SubIndexName[j] = pReqBuffer[i];
			j++;
		}
		i++;
	}
	pRspRef->SubIndexName[j] = '\0';

	if (!strcmp(pRspRef->SubIndexName, "requestNo") || !strcmp(pRspRef->SubIndexName, "RequestNo"))
	{
		i++;
		j = 0;
		char sRequestNo[256] = "";
		while (pReqBuffer[i] != ',')
		{
			if (pReqBuffer[i] == '\0')
				break;
			sRequestNo[j] = pReqBuffer[i];
			j++;
			i++;
		}
		pRspRef->RequestNo = atoi(sRequestNo);
		i++;
		j = 0;
		while (pReqBuffer[i] != ':')
		{
			if (pReqBuffer[i] == '\0')
				break;
			if (pReqBuffer[i] != '\"')
			{
				pRspRef->SubIndexName[j] = pReqBuffer[i];
				j++;
			}
			i++;
		}
		pRspRef->SubIndexName[j] = '\0';
	}

	if (!strcmp(pRspRef->SubIndexName, "TimeGap") || !strcmp(pRspRef->SubIndexName, "timeGap"))
	{
		i++;
		j = 0;
		char sTimeGap[256] = "";
		while (pReqBuffer[i] != ',')
		{
			if (pReqBuffer[i] == '\0')
				break;
			sTimeGap[j] = pReqBuffer[i];
			j++;
			i++;
		}
		pRspRef->SubjectNo = atoi(sTimeGap);
		i++;
		j = 0;
		while (pReqBuffer[i] != ':')
		{
			if (pReqBuffer[i] == '\0')
				break;
			if (pReqBuffer[i] != '\"')
			{
				pRspRef->SubIndexName[j] = pReqBuffer[i];
				j++;
			}
			i++;
		}
		pRspRef->SubIndexName[j] = '\0';
	}

	if (!strcmp(pRspRef->SubIndexName, "TimeStamp") || !strcmp(pRspRef->SubIndexName, "timeStamp"))
	{
		i++;
		j = 0;
		char sTimeStamp[256] = "";
		while (pReqBuffer[i] != ',')
		{
			if (pReqBuffer[i] == '\0')
				break;
			sTimeStamp[j] = pReqBuffer[i];
			j++;
			i++;
		}
		pRspRef->SessionNo = atoi(sTimeStamp);

		i++;
		j = 0;
		while (pReqBuffer[i] != ':')
		{
			if (pReqBuffer[i] == '\0')
				break;
			if (pReqBuffer[i] != '\"')
			{
				pRspRef->SubIndexName[j] = pReqBuffer[i];
				j++;
			}
			i++;
		}
		pRspRef->SubIndexName[j] = '\0';
	}
	pRspRef->Length = pReqRef->Length - i - 2;
	return pReqBuffer + i + 1;
}

void* CDataSwitchEngine::Replay2(const TOutFogMetaRef* pReqRef, const void *pReqBuffer, TOutFogMetaRef* pRspRef, char *pRspBuffer)
{
	switch (pReqRef->FieldNo)
	{
	case -1:
	{
		switch (pReqRef->FrontNo)
		{
		case 1:
		case 2:
		case 3:
			sprintf(pRspBuffer, "{\"a\":\"%s\",\"n\":%d,\"c\":%d,\"m\":\"%s\",\"r\":[",
				getShortPackageName(pReqRef->MessageNo), pReqRef->RequestNo, pReqRef->SubjectNo, pReqRef->Remark);
			break;
		case 4:
			sprintf(pRspBuffer, "{a:%s,n:%d,c:%d,m:%s,r:[",
				getShortPackageName(pReqRef->MessageNo), pReqRef->RequestNo, pReqRef->SubjectNo, pReqRef->Remark);
			break;
		default:
			sprintf(pRspBuffer, "{\"action\":\"%s\",\"requestNo\":%d,\"errorCode\":%d,\"errorMsg\":\"%s\",\"result\":[",
				getPackageName(pReqRef->MessageNo), pReqRef->RequestNo, pReqRef->SubjectNo, pReqRef->Remark);
			break;
		}
		break;
	}
	case -2:
	{
		double BusinessNo = 0;
		if (pReqRef->BusinessNo != DOUBLE_NaN)
			BusinessNo = pReqRef->BusinessNo;
		switch (pReqRef->FrontNo)
		{
		case 1:
		case 2:
		case 3:
			sprintf(pRspBuffer, "{\"a\":\"%s\",\"b\":%0.f,\"i\":\"%s\",\"r\":[", getShortPackageName(pReqRef->MessageNo), BusinessNo, pReqRef->Remark);
			break;
		case 4:
			sprintf(pRspBuffer, "{a:%s,b:%0.f,i:%s,r:[", getShortPackageName(pReqRef->MessageNo), BusinessNo, pReqRef->Remark);
			break;
		default:
		{
			char cChangeType = pReqRef->RequestNo;
			sprintf(pRspBuffer, "{\"action\":\"%s\",\"index\":\"%s\",\"bNo\":%0.f,\"changeType\":\"%c\",\"result\":[",
				getPackageName(pReqRef->MessageNo), pReqRef->Remark, BusinessNo, cChangeType);
			break;
		}
		}
		break;
	}
	case -3:
	{
		if (pReqRef->GlobalNo == 0.0)
		{
			strcpy(pRspBuffer, "]}");
		}
		else
		{
			switch (pReqRef->FrontNo)
			{
			case 2:
			case 3:
			case 4:
				strcpy(pRspBuffer, "]}]}");
				break;
			default:
				strcpy(pRspBuffer, "]}");
			}
		}
		break;
	}
	default:
	{
		if (pReqRef->GlobalNo == 0.0)
		{
			char m_fieldStruct[4098 * 4];
			Swap(pReqRef, pReqBuffer, pRspRef, m_fieldStruct);
			switch (pReqRef->FrontNo)
			{
			case 1:
				sprintf(pRspBuffer, "{\"t\":\"%s\",\"d\":{%s}}", pRspRef->SubIndexName, m_fieldStruct);
				break;
			case 2:
				sprintf(pRspBuffer, "{\"t\":\"%s\",\"d\":[{%s}", pRspRef->SubIndexName, m_fieldStruct);
				break;
			case 3:
				sprintf(pRspBuffer, "{\"%s\":[[%s]", pRspRef->SubIndexName, m_fieldStruct);
				break;
			case 4:
				sprintf(pRspBuffer, "{%s:[[%s]", pRspRef->SubIndexName, m_fieldStruct);
				break;
			default:
				sprintf(pRspBuffer, "{\"table\":\"%s\",\"data\":{%s}}", pRspRef->SubIndexName, m_fieldStruct);
				break;
			}
		}
		else
		{
			char m_fieldStruct[4098 * 4];
			Swap(pReqRef, pReqBuffer, pRspRef, m_fieldStruct);
			switch (pReqRef->FrontNo)
			{
			case 1:
				sprintf(pRspBuffer, ",{\"t\":\"%s\",\"d\":{%s}}", pRspRef->SubIndexName, m_fieldStruct);
				break;
			case 2:
			{
				if (pRspRef->FieldNo == pReqRef->FieldNo)
					sprintf(pRspBuffer, ",{%s}", m_fieldStruct);
				else
					sprintf(pRspBuffer, "]},{\"t\":\"%s\",\"d\":[{%s}", pRspRef->SubIndexName, m_fieldStruct);
				break;
			}
			case 3:
			{
				if (pRspRef->FieldNo == pReqRef->FieldNo)
					sprintf(pRspBuffer, ",[%s]", m_fieldStruct);
				else
					sprintf(pRspBuffer, "]},{\"%s\":[[%s]", pRspRef->SubIndexName, m_fieldStruct);
				break;
			}
			case 4:
			{
				if (pRspRef->FieldNo == pReqRef->FieldNo)
					sprintf(pRspBuffer, ",[%s]", m_fieldStruct);
				else
					sprintf(pRspBuffer, "]},{%s:[[%s]", pRspRef->SubIndexName, m_fieldStruct);
				break;
			}
			default:
			{
				sprintf(pRspBuffer, ",{\"table\":\"%s\",\"data\":{%s}}", pRspRef->SubIndexName, m_fieldStruct);
			}
			}
		}
	}
	}
	pRspRef->FieldNo = pReqRef->FieldNo;
	pRspRef->Length = strlen(pRspBuffer);
	return NULL;
}

const char* CDataSwitchEngine::getVersion()
{
	static char version[256];
	sprintf(version, "Service version: %s", "dataswitch");
	return version;
}

void CDataSwitchEngine::setServiceCallback(COutFogServiceCallback *pCallback)
{
	m_pServiceCallback = pCallback;
}

extern "C" COutFogServiceApi* CreateOutFogServiceApi(COutFogServiceCallback *pCallback, int nServiceID)
{
	return new CDataSwitchEngine();
}
