#include "Config.h"
#include "platform.h"
#include "public.h"

#define CONFIG_MAX_LEN			256

void GetConfig(const char *filename, char *key, char *val, int size)
{
	char    buffer[CONFIG_MAX_LEN+10];
	char 	*tkey,*tval;

	FILE  *fp=mfopen(filename, "r");
	if (fp == NULL)
		EMERGENCY_EXIT("config file [%s] not found", filename);

	while (fgets(buffer, CONFIG_MAX_LEN, fp)!=NULL)
	{
#ifdef LINUX
		// WIndows 平台没有函数 strtok_r，用条件编译 
		char *safeptr;
		if (strtok_r(buffer,"\n\r",&safeptr)==NULL)
#else
		if (strtok(buffer,"\n\r")==NULL)
#endif
			continue;

		if (buffer[0]=='#')
			continue;
		

#ifdef LINUX
		tkey = strtok_r(buffer, "\n\r=",&safeptr);
		tval = strtok_r(NULL, "\n\r",&safeptr);
#else
		tkey = strtok(buffer, "\n\r=");
		tval = strtok(NULL, "\n\r");
#endif

		if (tkey == NULL)
			EMERGENCY_EXIT("config file [%s] not found", filename);
	
		if (strcmp(key, tkey) == 0)
		{
			fclose(fp);	
			if (tval == NULL)
				strncpy(val, "", size);
			else
				strncpy(val, tval, size);
			return;
		}
	}
	
	fclose(fp);
}

int GetConfigInt(const char *filename, char *key)
{
	char buf[CONFIG_MAX_LEN] = {};
	GetConfig(filename, key, buf, sizeof(buf));
	return atoi(buf);
}

double GetConfigDouble(const char *filename, char *key)
{
	char buf[CONFIG_MAX_LEN] = {};
	GetConfig(filename, key, buf, sizeof(buf));
	return atof(buf);
}

string GetConfigString(const char *filename, char *key)
{
	char buf[CONFIG_MAX_LEN] = {};
	GetConfig(filename, key, buf, sizeof(buf));
	return buf;
}
