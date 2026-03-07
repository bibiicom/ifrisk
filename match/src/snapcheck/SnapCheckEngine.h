#ifndef TRADEENGINE_H
#define TRADEENGINE_H

#include "Engine.h"
#include "AlterServiceInterface.h"
#include "AlterService.h"
#include "OutFogServiceApi.h"
#include "TableDefine.h"


/////////////////////////////////////////////////////////////////////////
///CMemoryTradeEngine是一个基于内存消息传送的交易引擎，包含了交易用的内存服务连接
/////////////////////////////////////////////////////////////////////////
class CSnapCheckEngine : public CEngine, public CTinitTableCallback
{
public:
	///构造函数
	///@param	pConfig	创建引擎需要使用的配置
	///@param	pTradeResult	用于放置输出结果的流
	///@param	recovery	是否使用恢复模式
	///@param	verbose	true表示要输出结果，flase表示不要输出结果
	CSnapCheckEngine(const char *pszOutFilePath);

	virtual ~CSnapCheckEngine();
	virtual bool SendTable(CFieldDescribe *pFieldDesc, const void *pRspBuffer);
	void handleMessage(const TOutFogMetaRef* pOutFogReqRef, const void *pStream);
	virtual const char* getVersion();
	bool SnapCheck();
	bool LoadFromCSV(const char *pszCSVPath);

	bool checkMemberPostion();
	bool checkMember();
	bool checkPostion();
	bool checkLongEqualShort();

	//bool checkCount();
	bool checkAccount();
	bool checkOrder();

	bool checkSpotTick();
	bool checkTriggerOrder();

	void OutPutClose();
	void checkLink();
	void checkNull();
	void checkValid();
private:
	CAlterService  *m_pAlterService;
	CAlterServiceInterface *m_pAlterServiceInterface;

	TOutFogMetaRef m_OutFogMetaRef;
	char m_pStreamBuf[OUTFOG_MAX_PACKAGE_SIZE * 3];
	TSessionRef m_SessionRef;
	FILE *m_OutPut;

	//map<string, CRatioType> m_Dvalue;
};

#endif
