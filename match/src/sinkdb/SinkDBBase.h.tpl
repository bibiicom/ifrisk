!!function!!
int isInitPackage(char *usage)
{
	if (strcmp(usage,"InitTable")==0)
	{
		return 1;
	}
	return 0;
}
int isDownPackage(char *usage)
{
	if (strcmp(usage,"DownTable")==0)
	{
		return 1;
	}
	return 0;
}
!!endfunction!!
#ifndef _CSinkDBBase_H
#define _CSinkDBBase_H

#include "Config.h"
#include "SinkDBDispatcher.h"

extern bool g_bLoadCSV;

//这个类的出现就是为了能够自动生成代码，不影响主类
//所以这个类不能单独使用

class CSinkDBBase
{
public:
	CSinkDBBase(){};
	virtual ~CSinkDBBase(){};	
	bool DisPatch(const TOutFogMetaRef* pOutFogReqRef,const void *pStream)
	{
		switch(pOutFogReqRef->SessionNo)
		{
!!enter XTP!!
!!travel fields!!
!!if valid_name("usage")!!
	!!if isInitPackage(@usage) || isDownPackage(@usage)!!	
		case FID_!!@name!!:
			{
				C!!@name!!Field::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(C!!@name!!Field::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
	!!endif!!
!!endif!!
!!next!!
!!leave!!
		default:
			{

			}
		}
		return true;
	}

	bool DisPatch(CFieldDescribe *pFieldDesc, const void *pRspBuffer)
	{
		//过滤从csvload进来的数据，不需要入库filter的数据
		switch(pFieldDesc->m_FieldID)
		{
!!enter XTP!!
!!travel fields!!
!!if valid_name("usage")!!
	!!if isInitPackage(@usage) || isDownPackage(@usage)!!
			!!if valid_name("filter")!!
			case FID_!!@name!!:
			{
				C!!@name!!Field* pField = (C!!@name!!Field*)pRspBuffer;
				if(!!@filter!!)
					return true;
				break;
			}			
			!!endif!!
	!!endif!!
!!endif!!
!!next!!
!!leave!!
		default:
			{
			}
		}
		
		m_ActionType = TSL_INSERT;
		switch(pFieldDesc->m_FieldID)
		{
!!enter XTP!!
!!travel fields!!
!!if valid_name("usage")!!
	!!if isInitPackage(@usage) || isDownPackage(@usage)!!	
			case FID_!!@name!!:
			{
		!!if valid_name("multi")!!
			!!if !strcmp(get_string("multiType"),"char")!!
				int nDispatchNo = ((C!!@name!!Field*)pRspBuffer)->!!@multiField!!.getValue()[strlen(((C!!@name!!Field*)pRspBuffer)->!!@multiField!!.getValue())-1] - '0';
			!!endif!!
			!!if !strcmp(get_string("multiType"),"int")!!
				int nDispatchNo = ((C!!@name!!Field*)pRspBuffer)->!!@multiField!!.getValue();
			!!endif!!
			!!if !strcmp(get_string("multiType"),"double")!!
				int nDispatchNo = (int)(((C!!@name!!Field*)pRspBuffer)->!!@multiField!!.getValue());
			!!endif!!
				nDispatchNo = abs(nDispatchNo);
				int RequestNo = !!@syncno!! + nDispatchNo % !!@multi!!;
		!!else!!
				int RequestNo = !!@syncno!!;
		!!endif!!				
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(C!!@name!!Field::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
	!!endif!!
!!endif!!
!!next!!
!!leave!!
		default:
			{

			}
		}
		return true;
	}

protected:
	CSinkDBDispatcher *m_pSinkDBDispatcher;
	TSQLType m_ActionType;
	UF_INT8 m_nSeriesNo;
	char m_pBuffer[OUTFOG_MAX_PACKAGE_SIZE * 3];
};

#endif
