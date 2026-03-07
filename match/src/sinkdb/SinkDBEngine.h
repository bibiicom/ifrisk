#ifndef _SINKDBENGINE_H
#define _SINKDBENGINE_H

#include "OutFogServiceApi.h"
#include "SinkDBBase.h"

class CSinkDBEngine :public COutFogServiceApi, public CSinkDBBase, public CTinitTableCallback
{
public:
	CSinkDBEngine(COutFogServiceCallback *pCallback, int nKernelID);
	
	virtual ~CSinkDBEngine();

	virtual bool ReqField(const TOutFogMetaRef* pOutFogReqRef,const void *pStream);
	virtual bool SyncReqField(const TOutFogMetaRef* pOutFogReqRef,const void *pReqStream,TOutFogMetaRef* pOutFogRspRef,void *pRspStream)
	{
		return true;
	}
	virtual bool SendTable(CFieldDescribe *pFieldDesc, const void *pRspBuffer);
	virtual const char* getVersion();

private:
	bool m_bRealDo;
	int m_nLastTNO;
	char m_pStreamBuf[OUTFOG_MAX_PACKAGE_SIZE * 3];
	FILE* m_fDumpPhase;
};

#endif
