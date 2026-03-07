#include "version.h"
#include "versionValue.h"

void showVersion(int argc, char *argv[])
{
	if ((argc == 2) && (!strcmp(argv[1], "-v")))
	{
		printf("%s version: %s\n", argv[0], INNER_BASE_VERSION);
		exit(0);
	}
	checkVersion();
}

void checkVersion()
{
	time_t dwCurrTime = time(NULL);
	char szLocalDate[9] = { '\0' };
	struct tm tt;
	LOCALTIME(&tt, &dwCurrTime);
	strftime(szLocalDate, sizeof(szLocalDate), "%Y%m%d", &tt);
	if (strcmp(szLocalDate,INNER_DUE_DATE) > 0)
	{
		printf("expire,exit!\n");
		exit(0);
	}
}