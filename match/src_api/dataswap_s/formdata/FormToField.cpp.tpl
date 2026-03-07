#include "platform.h"
#include "FormToField.h"
#include "BaseFunction.h"
#include "FieldMap.h"

CFieldDescribe* CFormAnalysis::FormToStruct(const char* packageName, const void *pInput, int nInputLen, void *pOutput, int *nTNo)
{
	CXTPPackageDefineNameMap::iterator iter = g_XTPPackageDefineNameMap.find(packageName);
	if(iter==g_XTPPackageDefineNameMap.end())
	{
		sprintf((char*)pOutput, "[NoPackageName][%s]", packageName);
		return NULL;
	}
	map<string, string> formMap = mapSplit((char *)pInput, nInputLen, '&', '=');
	switch((*iter).second->tid)
	{
!!enter FTD!!
!!travel packages!!
!!if !strcmp(@usage,"request")!!
	case TID_!!@name!!: //!!@comment!!
		{
			*nTNo = TID_!!@name!!;
			!!let tid_name=@name!!
			string sField = "";
!!travel self!!
				!!let fieldName=@name!!
				!!enter FTD!!
				!!travel fields!!
					!!if !strcmp(@name,@fieldName)!!
						!!travel self!!
							!!let fieldtypename=@type!!
							!!let eachfieldname=@name!!
			sField = "!!@name!!";
							!!enter FTD!!
								!!travel types!!
									!!let thistypename=multiaddstring(3,"C",@typename,"Type")!!
									!!if !strcmp(@fieldtypename,@thistypename)!!
			((C!!@fieldName!!Field*)pOutput)->!!@eachfieldname!!=formMap["!!@eachfieldname!!"].c_str();
			if (!((C!!@fieldName!!Field*)pOutput)->!!@eachfieldname!!.isValid())
			{
				strcpy((char*)pOutput, "[!!@fieldName!!][!!@eachfieldname!!][isValid]");
				return NULL;
			}			
									!!endif!!
								!!next!!
							!!leave!!
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

CFieldDescribe* CFormAnalysis::FormToStruct(const char* packageName, const char* fieldName, const void *pInput, int nInputLen, void *pOutput, int *nTNo)
{
	CXTPPackageDefineNameMap::iterator iter1 = g_XTPPackageDefineNameMap.find(packageName);
	if(iter1==g_XTPPackageDefineNameMap.end())
	{
		sprintf((char*)pOutput, "[NoPackageName][%s]", packageName);
		return NULL;
	}
	*nTNo = (*iter1).second->tid;

	CFieldNameMap::iterator iter = g_FieldNameMap.find(fieldName);
	if(iter==g_FieldNameMap.end())
	{
		sprintf((char*)pOutput, "[NoFieldName][%s]", fieldName);
		return NULL;
	}
	map<string, string> formMap = mapSplit((char *)pInput, nInputLen, '&', '=');
	
	switch((*iter).second->fid)
	{
!!enter FTD!!
	!!travel fields!!
		!!if valid_name("usage") && !strcmp(@usage,"InitTable")!!
	case FID_!!@name!!: //!!@comment!!
		{
			!!let tid_name=@name!!
			string sField = "";
			!!let fieldName=@name!!
				!!if !strcmp(@name,@fieldName)!!
					!!travel self!!
						!!let fieldtypename=@type!!
						!!let eachfieldname=@name!!
			sField = "!!@name!!";
						!!enter FTD!!
							!!travel types!!
								!!let thistypename=multiaddstring(3,"C",@typename,"Type")!!
								!!if !strcmp(@fieldtypename,@thistypename)!!
			((C!!@fieldName!!Field*)pOutput)->!!@eachfieldname!!=formMap["!!@eachfieldname!!"].c_str();
			if (!((C!!@fieldName!!Field*)pOutput)->!!@eachfieldname!!.isValid())
			{
				strcpy((char*)pOutput, "[!!@fieldName!!][!!@eachfieldname!!][isValid]");
				return NULL;
			}			
								!!endif!!
							!!next!!
						!!leave!!
					!!next!!
				!!endif!!
			return &C!!@fieldName!!Field::m_Describe;
		}
		!!endif!!
	!!next!!
!!leave!!
	default:
		{
			sprintf((char*)pOutput, "no FieldFID [%d]", (*iter).second->fid);
			return NULL;
		}
	}
	return NULL;
}