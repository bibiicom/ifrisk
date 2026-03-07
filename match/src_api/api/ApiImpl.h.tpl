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
///定义了客户端接口实现
/////////////////////////////////////////////////////////////////////////

#ifndef _MYAPIIMPL_H
#define _MYAPIIMPL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "!!@apiname!!!!@apiid!!Api.h"
#include "OutFogServiceApi.h"
#include "Mutex.h"

class CApiImpl :public C!!@apiname!!!!@apiid!!Api, public COutFogServiceCallback
{
public:	
	CApiImpl();
	virtual void Connect();
	virtual void DisConnect();
	virtual void SetAddress(const char *pAddress);
	virtual void SetCallback(C!!@apiname!!!!@apiid!!Callback *pCallback);
	virtual int  SetCertificateFile(const char *pCertFileName, const char *pKeyFileName, const char *pCaFileName, const char *pKeyFilePassword);

	virtual bool RspField(const TOutFogMetaRef* pRspRef, const void *pRspBuffer);
!!enter FTD!!
!!travel packages!!
!!if InUse(@name)!!
	!!if (!strcmp(@usage,"request"))!!

	///!!@comment!!
	virtual int !!@name!!(!!travel self!!!!if @pumpid!=0!!, !!endif!!!!@apiid!!!!@name!!Struct *p!!@name!!!!next!!, int nRequestID);
	!!endif!!
!!endif!!
!!next!!
!!leave!!

private:
	C!!@apiname!!!!@apiid!!Callback* m_pCallback;
	COutFogServiceApi* m_pOutFogServiceApi;
	TOutFogMetaRef m_OutFogMetaRef;
	char m_pStreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
};

#endif
