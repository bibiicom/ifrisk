/////////////////////////////////////////////////////////////////////////
///Trade系统的main文件
///@company 上海兆百网络科技有限公司
///20190808	徐忠华		创建该文件
///////////////////////////////////////////////////////////////////////

#include "OutFogServiceApi.h"
#include "utility.h"
#include "SnapCheckEngine.h"
#include "OutFogService.h"
#include "version.h"

void CmdLineArgError()
{
	printf("Usage: snapcheck snappath [checkresult]/-v/-h \n");
	printf("       snapcheck snap/snap.0 snap/check.snap.0     :检查snap.0里面的snap数据\n");
	printf("       snapcheck snap/snap.0                       :检查snap.0里面的snap数据,输出为snap/snap.0.check\n");
	printf("       snapcheck -v                                :显示版本信息\n");
	printf("       snapcheck -h                                :显示使用方法\n");
	exit(0);
}

/////////////////////////////////////////////////////////////////////////////////////
// 大道至简
/////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{

#ifdef UNIX
	signal(SIGABRT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, SIG_IGN);
#endif

	showVersion(argc, argv);

	string sOutFile = "snapcheck.out";

	switch (argc)
	{
	case 1:
	{
		CmdLineArgError();
		break;
	}
	case 2:
	{
		if (!strcmp(argv[1], "-h"))
			CmdLineArgError();
		char sOutFilePre[128];
		memset(sOutFilePre, 0, sizeof(sOutFilePre));
		int sLength = strlen(argv[1]);
		if (argv[1][sLength - 1] == '/' || argv[1][sLength - 1] == '\\')
			strncpy(sOutFilePre, argv[1], sLength - 1);
		else
			strncpy(sOutFilePre, argv[1], sLength);
		sOutFile = sOutFilePre;
		sOutFile += ".check";
		break;
	}
	case 3:
	{
		sOutFile = argv[2];
		break;
	}
	default:
	{
		break;
	}
	}

	printf("output check result::%s\n", sOutFile.c_str());

	CSnapCheckEngine SnapCheckEngine(sOutFile.c_str());
	SnapCheckEngine.LoadFromCSV(argv[1]);
	SnapCheckEngine.SnapCheck();
}
