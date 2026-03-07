/////////////////////////////////////////////////////////////////////////
///Trade系统的main文件
///@company 上海兆百网络科技有限公司
///20190808	徐忠华		创建该文件
///////////////////////////////////////////////////////////////////////

#include "OutFogServiceApi.h"
#include "Config.h"
#include "utility.h"
#include "TradeEngine.h"
#include "OutFogService.h"
#include "version.h"

void CmdLineArgError()
{
	printf("Usage: trade 0 1 ... n/(-r/-c) [from] [to]/-v/-h/-s \n");
	printf("       trade          :不设置启动号默认为0,只能支持单机运行\n");
	printf("       trade 1	      :设置启动号1\n");
	printf("       trade -v       :显示版本信息\n");
	printf("       trade -h       :显示使用方法\n");
	printf("       trade -s       :并且生成snap后继续，用于抛弃最后一个报文重启并且生成snap\n");
	printf("       trade -b       :用于抛弃最后一个报文重启并且生成snap,之后退出\n");
	printf("       trade -r 5 10  :仅仅从序号5号到10做redo,到10或者没有流水了就结束\n");
	printf("       trade -c 5 10  :仅仅从序号5号到10做和前一个版本redo输出,到10或者没有流水了就结束\n");
	exit(0);
}

CTradeEngine* g_pTradeEngine;

#ifdef UNIX
void segfalut(int dummy)
{
	if (!g_pTradeEngine->m_bFirstField)
	{
		g_bSnapName = "Restart";
		g_pTradeEngine->RollBack();
		g_pTradeEngine->snapMDB(g_nSequenceNo + 1, true);
		PRINT_TO_STD("SnapTo[%0.f]BeforeExit", g_nSequenceNo + 1);
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////////////
// 大道至简
/////////////////////////////////////////////////////////////////////////////////////

class CExchangeApp :public COutFogServiceCallback
{
public:
	CExchangeApp()
	{
	}

	~CExchangeApp()
	{

	}

	void setServiceApi(COutFogServiceApi* pServiceApi)
	{
		m_pServiceApi = pServiceApi;
		g_pTradeEngine = new CTradeEngine(this);
	}

	virtual bool RspField(const TOutFogMetaRef* pOutFogRspRef, const void *pField)
	{
		switch (pOutFogRspRef->Direction)
		{
		case OUTFOG_DIRECTION_TO_FRONT:
		case OUTFOG_DIRECTION_TO_OFFER:
		{
			return m_pServiceApi->ReqField(pOutFogRspRef, pField);
		}
		case OUTFOG_DIRECTION_TO_KERNEL:
		case OUTFOG_DIRECTION_TO_NULL:
		default:
		{
			if (pOutFogRspRef->Type == OUTFOG_TYPE_INIT)
			{
				return m_pServiceApi->ReqField(pOutFogRspRef, pField);
			}
			else
			{
				return g_pTradeEngine->ReqField(pOutFogRspRef, pField);
			}
		}
		}
	}

	//CTradeEngine *m_pTradeEngine;

private:
	COutFogServiceCallback* m_pCallback;
	COutFogServiceApi* m_pServiceApi;
};

int startBegin = 0;
int startEnd = 0;
int main(int argc, char *argv[])
{
	GET_ACCURATE_MSEC_TIME(startBegin);

#ifdef UNIX

//https ://blog.csdn.net/m0_59938453/article/details/127479926

//SIGINT             2		 Ctrl + C时OS送给前台进程组中每个进程。 有点像软件中断
//SIGABRT            6		 调用abort函数，进程异常终止。 
//SIGPOLL和SIGIO     8       指示一个异步IO事件，在高级IO中提及
//SIGKILL            9		 杀死进程的终极办法  例如我们之前 kill - 9
//SIGSEGV            11		无效存储访问时OS发出该信号    段错误就是在这个信号产生的
//SIGPIPE            13		  异步通讯  涉及管道和socket
//SIGALRM            14		  涉及alarm函数的实现
//SIGTERM            15     kill命令发送的OS默认终止信号
//SIGUSR1            10        用户自定义信号，作用和意义由应用自己定义
//SIGUSR2            12		 用户自定义信号，作用和意义由应用自己定义

	signal(SIGABRT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	//signal(SIGSEGV, segfalut);

	struct sigaction act;
	act.sa_handler = segfalut;

	//mask : 这个字段主要指定在信号处理函数正在运行的过程中，如果连续产生多个信号，需要屏蔽哪些信号。也就是说当进程收到屏蔽的信号时，正在进行的信号处理函数不会被
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGSEGV);

	//为了更加精细化的控制信号处理逻辑，这个字段保存了一些控制信号处理行为的选项集合。常见的选项有:
	//SA_ONESHOT/SA_RESETHAND: 意思是我们注册的信号处理函数，仅仅只起一次作用。响应完一次后，就设置回默认行为。
	//SA_NOMASK/SA_NODEFER: 表示信号处理函数在执行的过程中会被中断。比如我们进程捕获到一个感兴趣的信号，随后
	// 会执行注册的信号处理函数，但是此时进程又收到其他的信号或者和上次相同的信号，此时正在执行的信号处理函数会被中断，
	// 从而转去执行最新到来的信号处理函数。如果连续产生多个相同的信号，那么我们的信号处理函数就要做好同步，幂等等措施。
	//SA_INTERRUPT: 当进程正在执行一个非常耗时的系统调用时，如果此时进程接收到了信号，那么这个系统调用将会被信号中断，
	// 进程转去执行相应的信号处理函数。那么当信号处理函数执行完时，如果这里设置了 SA_INTERRUPT，那么系统调用将不会继续
	// 执行并且会返回一个 -EINTR常量，告诉调用方，这个系统调用被信号中断了，怎么处理你看着办吧。
	//SA RESTART : 当系统调用被信号中断后，相应的信号处理函数执行完毕后，如果这里设置了SA_RESTART 系统调用将会被自动重新启动
	act.sa_flags = SA_ONESHOT;
	// act.sa_flags = SA_ONESHOT;
	// act.sa_flags = SA_RESETHAND;
	// act.sa_flags = SA_NOMASK;
	// act.sa_flags = SA_NODEFER;
	// act.sa_flags = SA_INTERRUPT;
	// act.sa_flags = SA_RESTART;

	sigaction(SIGSEGV, &act, NULL);

#endif

	showVersion(argc, argv);

	if ((argc == 2) && (!strcmp(argv[1], "-h")))
		CmdLineArgError();

	if (argc > 4)
	{
		CmdLineArgError();
	}

	int nKernelid = 0;

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-r"))
			g_bOnlyRedo = true;

		if (!strcmp(argv[i], "-s"))
			g_bSnapFirst = true;

		if (!strcmp(argv[i], "-b"))
		{
			g_bSnapFirst = true;
			g_bRoolback = true;
		}

		if (!strcmp(argv[i], "-c"))
		{
			g_bOnlyRedo = true;
			g_bCompare = true;
		}

		if (isValidNumber(argv[i]))
		{
			if (g_bOnlyRedo)
			{
				if (g_nBeginID < 0)
					g_nBeginID = atof(argv[i]);				
				else
					g_nEndID = atof(argv[i]);
			}
			else
			{
				nKernelid = atoi(argv[i]);
			}
		}			
	}

	CExchangeApp* pExchange = new CExchangeApp();
	COutFogService*	m_pService = new COutFogService();
	COutFogServiceApi *pOutFogServiceApi = m_pService->LoadOutFogApi("omqsapi", (COutFogServiceCallback*)pExchange, nKernelid);
	pExchange->setServiceApi(pOutFogServiceApi);

	TOutFogMetaRef OutFogMetaRef;
	OutFogMetaRef.Type = OUTFOG_TYPE_START;
	pOutFogServiceApi->ReqField(&OutFogMetaRef, NULL);

	while (true)
	{
		if (startBegin != 0)
		{
			GET_ACCURATE_MSEC_TIME(startEnd);
			PRINT_TO_STD("------StartCost %d ms------", startEnd - startBegin);
			startBegin = 0;
			startEnd = 0;
		}
		g_pTradeEngine->SnapSave();
		SLEEP_SECONDS(2);
	}

	return 0;
}
