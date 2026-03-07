#ifndef _DATA_SWAP_H_
#define _DATA_SWAP_H_

#include "OutFogServiceApi.h"
#include "json/json.h"

class CDataSwitchEngine: public COutFogServiceApi
{
public:
	CDataSwitchEngine();
	virtual ~CDataSwitchEngine(void);	

	virtual void setServiceCallback(COutFogServiceCallback *pCallback);
	virtual const char* getVersion();
	virtual bool ReqField(const TOutFogMetaRef* pReqRef,const void *pReqBuffer);
	virtual bool Start(){return true;};
	virtual bool SyncReqField(const TOutFogMetaRef* pReqRef,const void *pReqBuffer,TOutFogMetaRef* pRspRef,void *pRspBuffer);

	const char* Replay0(const TOutFogMetaRef* pReqRef, const char* pReqBuffer, TOutFogMetaRef* pRspRef, void* pRspBuffer);
	const char* Replay1(const TOutFogMetaRef* pReqRef, const char *pReqBuffer, TOutFogMetaRef* pRspRef, void *pRspBuffer);
	void* Replay2(const TOutFogMetaRef* pReqRef, const void *pReqBuffer, TOutFogMetaRef* pRspRef, char *pRspBuffer);
	bool DC(const TOutFogMetaRef* pReqRef, const void *pReqBuffer, TOutFogMetaRef* pRspRef, char *pRspBuffer);

private:
};

#endif
