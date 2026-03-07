#include "public.h"
#include "SimpleConnect.h"
#include "version.h"

int main(int argc, char *argv[])
{
	showVersion(argc, argv);

	CSelectReactor *pReactor = new CSelectReactor();
	CSimpleConnecter *pConnect = new CSimpleConnecter(pReactor);

	pConnect->RegisterConnecter("tcp://127.0.0.1:12701");
	pReactor->Create();
	pConnect->Start();

	int nCount = 0;

#ifdef WINDOWS
	SYSTEMTIME t;
	GetLocalTime(&t);
	int first_s = t.wSecond;
	int first_ms = t.wMilliseconds;
#else
	struct timeval t;
	gettimeofday(&t, NULL);	
	int first_s = t.tv_sec;
	int first_ms = t.tv_usec/1000;
#endif

	while (true)
	{
		char message[1024];
		memset(message, 'c', sizeof(message));
		if (pConnect->RequestDirectly(message) == 0)
		{
			nCount++;
		}
		else
		{
			printf("Disconnected,sleep 1 second \n");
			SLEEP_SECONDS(1);
			continue;
		}
		
		if (nCount % 8 == 0)
		{
			SLEEP(1);
		}

		if (nCount == 16000)
		{
			int this_mSecond = 0;
#ifdef WINDOWS
			SYSTEMTIME t;
			GetLocalTime(&t);
			int second_s = t.wSecond;
			int second_ms = t.wMilliseconds;
#else
			struct timeval t;
			gettimeofday(&t, NULL);
			int second_s = t.tv_sec;
			int second_ms = t.tv_usec / 1000;
#endif
			//printf("this sencond %d \n", this_mSecond);
			//printf("mSecond %d \n", mSecond);
			int sGap = second_s - first_s;
			if (sGap <= 0)
			{
				sGap += 60;
			}
			int msGap = sGap * 1000 + second_ms - first_ms;
			first_ms = second_ms;
			first_s = second_s;
			printf("speed: %d/s \n", (nCount * 1000) / msGap);
			nCount = 0;
		}
	}

	return 0;
}
