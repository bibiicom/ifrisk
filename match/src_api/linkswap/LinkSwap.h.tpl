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
#ifndef __OFSWAPLINK_H_
#define __OFSWAPLINK_H_

#include "!!@apiname!!!!@apiid!!Api.h"
#include "platform.h"
#include "OutFogServiceApi.h"
#include "ExLinkApi.h"
#include "versionValue.h"

using namespace !!@apiname!!!!@apiid!!Byte;

//交易席位接口
class COFSwapLink : public C!!@apiname!!!!@apiid!!Callback, public CExLinkApi
{
public:
	COFSwapLink(COutFogServiceCallback *pCallback);
	~COFSwapLink();

	//	void InitLink(CLinkField *pLinkField);

	virtual bool ReqField(const TOutFogMetaRef* pOutFogReqRef, const void *pStream);
	virtual bool SyncReqField(const TOutFogMetaRef* pOutFogReqRef, const void *pReqStream, TOutFogMetaRef* pOutFogRspRef, void *pRspStream) { return true; };
	//连接
	void OnConnected();
	void OnDisconnected(int nReason);
!!enter FTD!!
!!travel packages!!
	!!if InUse(@name)!!
		!!if !strcmp(@usage,"response") !!
	///!!@comment!!
	virtual void !!@name!!(!!travel self!!!!if @pumpid!=0!!, !!endif!!const !!@apiid!!!!@name!!Struct *p!!@name!!!!next!!, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain);
!!elseif !strcmp(@usage,"Push")!!
	///!!@comment!!
	virtual void !!@name!!(!!travel self!!!!if @pumpid!=0!!, !!endif!!const !!@apiid!!!!@name!!Struct *p!!@name!!!!next!!, int nLocalNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain);
		!!endif!!
	!!endif!!	
!!next!!
!!leave!!

	virtual const char* getVersion()
	{
		return INNER_BASE_VERSION;
	}

protected:
	void QueryFinish();
	void Login();
	bool InitLink(CLinkField *pLinkField, int nRequestNo);
	C!!@apiname!!!!@apiid!!Api* GetDll(const char * pName);

private:
	C!!@apiname!!!!@apiid!!Api *m_pApi;
};

#endif
