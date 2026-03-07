#include "OutFogServiceApi.h"
#include "Config.h"
#include "utility.h"
#include "SinkDBEngine.h"
#include "OutFogService.h"
#include "version.h"
#include "DbHelper.h"

void CmdLineArgError()
{
	printf("Usage: sinkdb \n");
	printf("        sinkdb 0...n   :设置启动号\n");
	printf("        sinkdb -r      :加载csv启动\n");
	printf("        sinkdb -v      :显示版本信息\n");
	exit(0);
}

TDBMethod g_HelperTaskMethod[]
{
	TASK_METHOD_DEFINE(CKLineField, TDB_TABLE, "t_D_KLine", "ExchangeID,InstrumentID,Bar,InsertTime", new CDataContainer<CKLineField>()),
	TASK_METHOD_END
};

bool g_bLoadCSV = false;

/////////////////////////////////////////////////////////////////////////////////////
// 大道至简
/////////////////////////////////////////////////////////////////////////////////////

class CSinkDB:public COutFogServiceCallback
{
public:
	CSinkDB(int nKernelid, UF_INT8 nBeginNo = 0)
	{
		m_nKernelid = nKernelid;
		m_nBeginNo = nBeginNo;
	}

	~CSinkDB()
	{

	}

	void setServiceApi(COutFogServiceApi* pServiceApi)
	{
		m_pServiceApi = pServiceApi;
		m_pEngine = new CSinkDBEngine(this, m_nKernelid);
	}

	virtual bool RspField(const TOutFogMetaRef* pOutFogRspRef,const void *pField)
	{	
		return m_pEngine->ReqField(pOutFogRspRef,pField);
	}

private:
	CSinkDBEngine *m_pEngine;
	COutFogServiceCallback* m_pCallback;
	COutFogServiceApi* m_pServiceApi;
	UF_INT8 m_nBeginNo;
	int m_nKernelid;
};

int startBegin = 0;
int startEnd = 0;
int main(int argc, char *argv[])
{
	GET_ACCURATE_MSEC_TIME(startBegin);
#ifdef UNIX
	signal(SIGABRT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, SIG_IGN);
#endif

	showVersion(argc,argv);

	if (argc > 4)
	{
		CmdLineArgError();
	}

	int nKernelid = 0;

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-r"))
			g_bLoadCSV = true;

		if (isValidNumber(argv[i]))
			nKernelid = atoi(argv[i]);
	}

	CSinkDB* pApp = new CSinkDB(nKernelid);
	COutFogService*	m_pService = new COutFogService();
	COutFogServiceApi *pOutFogServiceApi = m_pService->LoadOutFogApi("omqsapi",(COutFogServiceCallback*)pApp,nKernelid);
	pApp->setServiceApi(pOutFogServiceApi);

	TOutFogMetaRef OutFogMetaRef;
	OutFogMetaRef.Type = OUTFOG_TYPE_START;
	pOutFogServiceApi->ReqField(&OutFogMetaRef,NULL);
	
	while(true)
	{
		if (startBegin != 0)
		{
			GET_ACCURATE_MSEC_TIME(startEnd);
			PRINT_TO_STD("startCost %d ms", startEnd - startBegin);
			startBegin = 0;
			startEnd = 0;
		}
		SLEEP_SECONDS(10);
	}

	return 0;
}
