/////////////////////////////////////////////////////////////////////////
///@system OutFogServiceApiWrapper
///OutFogServiceApiWrapper.h
///定义了C++实现C的api的一个简单的脚手架
///@history 
///20190909	徐忠华	创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef _OUTFOG_SERVICE_API_WRAPPER_H_
#define _OUTFOG_SERVICE_API_WRAPPER_H_

#include "OutFogServiceApi.h"
#include "OutFogServiceCApi.h"
#include "platform.h"

void _OutFogRspField(void* pInstance,TOutFogMetaRef* pOutFogRspRef,void *pStream);

class COutFogServiceApiWrapper:public COutFogServiceApi
{
public:
	COutFogServiceApiWrapper(const char* pName,COutFogServiceCallback *pCallback=0,int nServiceID=0)
	{
		m_pServiceCallback = pCallback;
		m_pInstance = NULL;
		getInstance(pName,nServiceID);
		if(m_pInstance!=NULL)
		{
			m_Callbacks.ptrOutFogRspField = &_OutFogRspField;
			m_pRegisterCallbacks(m_pInstance,&m_Callbacks);
		}
	}
	virtual ~COutFogServiceApiWrapper()
	{
	}

	const char* getVersion()
	{
		if(m_pInstance==NULL)
		{
			return "no instance!";
		}
		return m_pGetVersion(m_pInstance);
	}
	bool Start()
	{
		return m_pStart(m_pInstance);
	}
	bool ReqField(const TOutFogMetaRef* pOutFogReqRef,const void *pStream)
	{
		return m_pReqField(m_pInstance,pOutFogReqRef,pStream);
	}
	bool SyncReqField(const TOutFogMetaRef* pOutFogReqRef,const void *pReqStream,TOutFogMetaRef* pOutFogRspRef,void *pRspStream)
	{
		return m_pSyncReqField(m_pInstance,pOutFogReqRef,pReqStream,pOutFogRspRef,pRspStream);
	};
	
	void setServiceCallback(COutFogServiceCallback *pCallback)
	{
		m_pServiceCallback = pCallback;
	}

	int getServiceID()
	{
		m_pGetServiceID(m_pInstance);
	}

	bool getInstance(const char* pName,int nServiceID)
	{
		char ServiceDllName[128];
#ifdef WINDOWS
		snprintf(ServiceDllName, 128, "%s.dll", pName);
		HINSTANCE hDll=LoadLibrary(ServiceDllName);
		if(hDll!=NULL)
		{
			m_pCreateServiceApi = (_CreateOutFogServiceCApi*)GetProcAddress(hDll, "CreateOutFogServiceCApi");
			m_pGetServiceID = (_OutFogGetServiceID*)GetProcAddress(hDll, "OutFogGetServiceID");
			m_pReqField = (_OutFogReqField*)GetProcAddress(hDll, "OutFogReqField");
			if(m_pCreateServiceApi!=NULL)
			{
				m_pInstance = m_pCreateServiceApi(nServiceID);
				printf("kernel: Load Library:%s \n",ServiceDllName);
			}
		}
		if(m_pInstance==NULL)
		{
			int errorid = GetLastError();
			printf("error: Can not Load Library:%s errorid:%d\n",ServiceDllName,errorid);
			return false;
		}
#else
		sprintf(ServiceDllName,"lib%s.so",pName);
		void *handle = dlopen (ServiceDllName, RTLD_NOW);
		if(handle)
		{
			m_pCreateServiceApi = (_CreateOutFogServiceCApi*)dlsym(handle, "CreateOutFogServiceApi");
			m_pGetServiceID = (_OutFogGetServiceID*)dlsym(handle, "OutFogGetServiceID");
			m_pReqField = (_OutFogReqField*)dlsym(handle, "OutFogReqField");
			if(m_pCreateServiceApi!=NULL)
			{
				m_pInstance = m_pCreateServiceApi(nServiceID);
				printf("kernel: Load Library:%s\n",ServiceDllName);
			}
		}
		if(m_pInstance==NULL)
		{
			printf("error: Can not Load Library:%s msg:%s\n",ServiceDllName,dlerror());
			return false;
		}
#endif
		return true;
	}

protected:
	void* m_pInstance;
	TOutFogCallbacks m_Callbacks;
	COutFogServiceCallback* m_pServiceCallback;
	int m_nDllType;

	_CreateOutFogServiceCApi* m_pCreateServiceApi;
	_OutFogRegisterCallbacks* m_pRegisterCallbacks;
	_OutFogGetVersion* m_pGetVersion;
	_OutFogStart* m_pStart;
	_OutFogReqField* m_pReqField;
	_OutFogSyncReqField* m_pSyncReqField;
	_OutFogGetServiceID* m_pGetServiceID;
};

#endif

