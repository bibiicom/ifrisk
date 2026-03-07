!!function!!
#include <set>
#include <string>
using namespace std;

typedef set<string> CPackageSet;
CPackageSet g_setUsePackage;

bool InUse(char *name)
{
	if(g_setUsePackage.find("*")!=g_setUsePackage.end())
		return true;
	return (g_setUsePackage.find(name)!=g_setUsePackage.end());
}
!!endfunction!!
!!enter api!!
!!travel self!!
!!command g_setUsePackage.insert(@name);!!
!!next!!
!!leave!!
!!enter project!!
!!let apiname=@apiname!!
!!let APINAME=@APINAME!!
!!let apiid=@apiid!!
!!let APIID=@APIID!!
!!leave!!
/////////////////////////////////////////////////////////////////////////
///!!@apiname!!!!@apiid!!ApiImpl.cpp
///实现了客户端接口
/////////////////////////////////////////////////////////////////////////

#include "public.h"
#include "ApiImpl.h"
#include "FieldData.h"
#include "OutFogService.h"

CApiImpl::CApiImpl()
{
	//m_pOutFogServiceApi = CreateOutFogServiceApi(this, 0);
	m_pOutFogServiceApi = COutFogService::LoadOutFogApi("!!@apiname!!!!@apiid!!ApiX", this, 0);
	memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
}

C!!@apiname!!!!@apiid!!Api *C!!@apiname!!!!@apiid!!Api::CreateApi()
{
	return new CApiImpl();
}

const char *C!!@apiname!!!!@apiid!!Api::GetVersion()
{
	return ""; 
}

void CApiImpl::DisConnect()
{
	//DisconnectAll(0);
}

void CApiImpl::Connect()
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_START;
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, NULL);
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
}

void CApiImpl::SetAddress(const char *pAddress)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_INITLINK;
	//不能超过头外面的字符能占用的字节
	strncpy(m_OutFogMetaRef.Remark, pAddress, 124);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, NULL);
	m_OutFogMetaRef.Type = OUTFOG_TYPE_REQUEST;
}

int CApiImpl::SetCertificateFile(const char *pszCertFileName, const char *pszKeyFileName,
	const char *pszCaFileName, const char *pszKeyFilePassword)
{
	//	return CSslNetworkFactory::m_instance.RegisterCertificateFile(true, pszCertFileName, pszKeyFileName, 
	//		pszCaFileName, pszKeyFilePassword);
	return 0;
}

void CApiImpl::SetCallback(C!!@apiname!!!!@apiid!!Callback *pCallback)
{
	m_pCallback = pCallback;
}

!!enter FTD!!
!!travel packages!!
!!if InUse(@name)!!
!!let package_name=@name!!
!!if !strcmp(@usage,"request") !!
///!!@comment!!
int CApiImpl::!!@name!!(!!travel self!!!!if @pumpid!=0!!, !!endif!!!!@apiid!!!!@name!!Struct *p!!@name!!!!next!!, int nRequestID)
{
!!travel self!!
	C!!@name!!Field !!@name!!Field;
	memcpy(&!!@name!!Field, p!!@name!!, sizeof(C!!@name!!Field));
	!!let fieldname=@name!!
	!!enter FTD!!
		!!travel fields!!
			!!if !strcmp(@fieldname,@name)!!
				!!travel self!!
	if (! !!@fieldname!!Field.!!@name!!.isValid())
	{
		PRINT_TO_STD("!!@fieldname!!Field.!!@name!![%s]isInValid", !!@fieldname!!Field.!!@name!!.getString().c_str());
		return -4;
	}
				!!next!!
			!!endif!!
		!!next!!
	!!leave!!	
	CFieldDescribe *pFieldDescribe = &(!!@fieldname!!Field.m_Describe);
	m_OutFogMetaRef.MessageNo = TID_!!@package_name!!;
	m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
	m_OutFogMetaRef.RequestNo = nRequestID;
	m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
	char StreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	pFieldDescribe->setSingleField(StreamBuf, &!!@fieldname!!Field);
	m_pOutFogServiceApi->ReqField(&m_OutFogMetaRef, StreamBuf);
!!next!!

	return 0;
}
!!endif!!
!!endif!!
!!next!!
!!leave!!

bool CApiImpl::RspField(const TOutFogMetaRef* pRspRef, const void *pRspBuffer)
{
	switch (pRspRef->Type)
	{
	case OUTFOG_TYPE_ADDSESSION:
	{
		m_pCallback->OnConnected();
		return true;
	}
	case OUTFOG_TYPE_DELSESSION:
	{
		m_pCallback->OnDisconnected(pRspRef->SubjectNo);
		return true;
	}
	}
	switch (pRspRef->MessageNo)
	{
!!enter FTD!!	
!!travel packages!!
!!if InUse(@name)!!
!!let package_name=@name!!
!!let package_usage=@usage!!	
!!if (!strcmp(@usage,"Push") || !strcmp(@usage,"response"))!!	
	case TID_!!@package_name!!: //!!@comment!!
	{
	!!travel self!!
		!!@apiid!!!!@name!!Struct* p!!@name!! = NULL;
		!!if @pumpid==0!!
			!!let paramAll=multiaddstring(2,"p",@name)!!
		!!else!!
			!!let paramAll=multiaddstring(3,@paramAll,",p",@name)!!
		!!endif!!
	!!next!!
		int mSubjectNo = pRspRef->SubjectNo;
		short nFieldNo = pRspRef->FieldNo;
		switch(nFieldNo)
		{
			!!travel self!!
			case -FID_!!@name!!:
				mSubjectNo = -mSubjectNo;
			case FID_!!@name!!:
			{
				C!!@name!!Field !!@name!!Field;
				if (pRspBuffer != NULL)
				{
					C!!@name!!Field::m_Describe.getSingleField(&!!@name!!Field, pRspBuffer);
					p!!@name!! = (!!@apiid!!!!@name!!Struct*)&!!@name!!Field;
				}
				break;
			}
			!!next!!
		}
		if (m_pCallback != NULL)
			!!if !strcmp(@package_usage,"response")!!
			m_pCallback->!!@package_name!!(!!@paramAll!!, mSubjectNo, pRspRef->Remark, pRspRef->RequestNo, pRspRef->Chain);
			!!elseif !strcmp(@package_usage,"Push")!!
			m_pCallback->!!@package_name!!(!!@paramAll!!, mSubjectNo, pRspRef->Remark, pRspRef->SequenceNo, pRspRef->BusinessNo, pRspRef->Chain);
			!!endif!!
		break;
	}
!!endif!!
!!endif!!
!!next!!
!!leave!!
	}
	return true;
}

extern "C" C!!@apiname!!!!@apiid!!Api* Create!!@apiname!!!!@apiid!!Api()
{
	return new CApiImpl();
}
