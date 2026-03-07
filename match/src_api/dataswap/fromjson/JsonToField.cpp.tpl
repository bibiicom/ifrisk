#include "platform.h"
#include "JsonToField.h"
#include "BaseFunction.h"
#include "FieldMap.h"

CFieldDescribe* CJsonAnalysis::FromJsonToStruct(const char* packageName, const void *pInput, int nInputLen, void *pOutput,int *nTNo)
{
	CXTPPackageDefineNameMap::iterator iter1 = g_XTPPackageDefineNameMap.find(packageName);
	if(iter1==g_XTPPackageDefineNameMap.end())
	{
		sprintf((char*)pOutput, "[NoPackageName][%s]", packageName);
		return NULL;
	}
	map<string, json_str> jsonMap = jsonSplit((char *)pInput + 1, nInputLen - 1);
	map<string, json_str>::iterator iter;
	switch((*iter1).second->tid)
	{
!!enter FTD!!
!!travel packages!!
!!if !strcmp(@usage,"request")!!
	case TID_!!@name!!: //!!@comment!!
		{
			*nTNo = TID_!!@name!!;
			!!let tid_name=@name!!
!!travel self!!
				!!let fieldName=@name!!
				!!enter FTD!!
				!!travel fields!!
					!!if !strcmp(@name,@fieldName)!!
						!!travel self!!
			iter = jsonMap.find("!!@name!!");
			if(iter != jsonMap.end())
			{
				if(!((C!!@fieldName!!Field*)pOutput)->!!@name!!.setJsonValue(iter->second.p, iter->second.len))
				{
					strcpy((char*)pOutput, "[!!@fieldName!!][!!@name!!][isInValid]");
					return NULL;
				}
			}
			else
			{
				((C!!@fieldName!!Field*)pOutput)->!!@name!!.clear();
			}
						!!next!!
					!!endif!!
				!!next!!
				!!leave!!
			return &C!!@fieldName!!Field::m_Describe;
!!next!!
		
		}
!!endif!!
!!next!!
!!leave!!	
	default:
		{
			return NULL;
		}		
	}
	return NULL;
}

CFieldDescribe* CJsonAnalysis::FromJsonToStruct(const char* packageName, const char* fieldName, const void *pInput, int nInputLen, void *pOutput,int *nTNo)
{
	CXTPPackageDefineNameMap::iterator iter1 = g_XTPPackageDefineNameMap.find(packageName);
	if(iter1==g_XTPPackageDefineNameMap.end())
	{
		sprintf((char*)pOutput, "[NoPackageName][%s]", packageName);
		return NULL;
	}
	*nTNo = (*iter1).second->tid;

	CFieldNameMap::iterator iter2 = g_FieldNameMap.find(fieldName);
	if(iter2==g_FieldNameMap.end())
	{
		sprintf((char*)pOutput, "[NoFieldName][%s]", fieldName);
		return NULL;
	}
	map<string, json_str> jsonMap = jsonSplit((char *)pInput + 1, nInputLen - 1);
	map<string, json_str>::iterator iter;
	switch((*iter2).second->fid)
	{
!!enter FTD!!
	!!travel fields!!
		!!if valid_name("usage") && !strcmp(@usage,"InitTable")!!
	case FID_!!@name!!: //!!@comment!!
		{
			!!let tid_name=@name!!
			string sField = "";
			try
			{
			!!let fieldName=@name!!
				!!if !strcmp(@name,@fieldName)!!
					!!travel self!!
				iter = jsonMap.find("!!@name!!");
				if(iter != jsonMap.end())
				{
					if(!((C!!@fieldName!!Field*)pOutput)->!!@name!!.setJsonValue(iter->second.p, iter->second.len))
					{
						strcpy((char*)pOutput, "[!!@fieldName!!][!!@name!!][isInValid]");
						return NULL;
					}
				}
				else
				{
					((C!!@fieldName!!Field*)pOutput)->!!@name!!.clear();
				}
					!!next!!				
				!!endif!!
			}
			catch(exception& e)
			{
				sprintf((char*)pOutput,"[!!@tid_name!!][%s][%s]",sField.c_str(),e.what());
				return NULL;
			}
			return &C!!@fieldName!!Field::m_Describe;		
		}
		!!endif!!
	!!next!!
!!leave!!	
	default:
		{
			sprintf((char*)pOutput, "no FieldFID [%d]", (*iter2).second->fid);
			return NULL;
		}		
	}
	return NULL;
}