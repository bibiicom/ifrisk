#include "printinfo.h"
//#include "time.h"
#include "BaseFunction.h"

int loglevel;
bool printstd;

//char * getCurrDateTime(void)
//{
//	time_t ltime;
//	struct tm *now;
//	static char timeBuffer[21];
//
//	time(&ltime);
//	now = localtime(&ltime);
//	sprintf(timeBuffer, "%04d%02d%02d-%02d:%02d:%02d",
//		now->tm_year + 1900,
//		now->tm_mon + 1,
//		now->tm_mday,
//		now->tm_hour,
//		now->tm_min,
//		now->tm_sec);
//	return timeBuffer;
//}

bool formatTime(char* pTime)
{
	if (pTime[0] == '\0')
	{
		sprintf(pTime, "00:00:00");
		return true;
	}

	vector<string> sTimes = Txtsplit(pTime, ":");
	if (sTimes.size() == 1)
		sTimes = Txtsplit(pTime, "-");
	if (sTimes.size() == 1)
		sTimes = Txtsplit(pTime, "|");

	if (sTimes.size() >= 3)
	{
		sprintf(pTime, "%02d:%02d:%02d", atoi(sTimes[0].c_str()), atoi(sTimes[1].c_str()), atoi(sTimes[2].c_str()));
	}
	else
	{
		if (sTimes.size() == 0)
		{
			sprintf(pTime, "00:00:00");
		}
		else
		{
			if (sTimes.size() == 2)
			{
				sprintf(pTime, "%02d:%02d:00", atoi(sTimes[0].c_str()), atoi(sTimes[1].c_str()));
			}
			else
			{
				if (sTimes.size() == 1)
					sprintf(pTime, "%02d:00:00", atoi(sTimes[0].c_str()));
				else
					sprintf(pTime, "00:00:00");
			}
		}
	}
	return pTime;
}

int getHourMinus(const char* sFirstTime, const char * sSencondTime)
{
	const char *pTime = sFirstTime;
	int hour = (pTime[0] - '0') * 10 + (pTime[1] - '0');
	int start = hour;

	pTime = sSencondTime;
	hour = (pTime[0] - '0') * 10 + (pTime[1] - '0');
	int end = hour;

	int minus = start - end;
	if (minus < 0)
		minus += 24;
	return minus;
}

int minusHour(const char* sFirstTime, const char * sSencondTime)
{
	const char *pTime = sFirstTime;
	int hour = (pTime[0] - '0') * 10 + (pTime[1] - '0');
	int start = hour;

	pTime = sSencondTime;
	hour = (pTime[0] - '0') * 10 + (pTime[1] - '0');
	int end = hour;

	int minus = start - end;
	if (minus < 0)
		minus += 24;
	return minus;
}

int getMiniteMinus(const char* sFirstTime, const char * sSencondTime)
{
	const char *pTime = sFirstTime;
	int hour = (pTime[0] - '0') * 10 + (pTime[1] - '0');
	int minute = (pTime[3] - '0') * 10 + (pTime[4] - '0');
	int start = hour * 60 + minute;

	pTime = sSencondTime;
	hour = (pTime[0] - '0') * 10 + (pTime[1] - '0');
	minute = (pTime[3] - '0') * 10 + (pTime[4] - '0');
	int end = hour * 60 + minute;

	int minus = start - end;
	if (minus < 0)
		minus += 24 * 60;
	return minus;
}

int minusMinute(const char* sFirstTime, const char * sSencondTime)
{
	const char* pTime = sFirstTime;
	int minute = (pTime[3] - '0') * 10 + (pTime[4] - '0');
	int start = minute;

	pTime = sSencondTime;
	minute = (pTime[3] - '0') * 10 + (pTime[4] - '0');
	int end = minute;

	int minus = start - end;
	if (minus < 0)
		minus += 60;
	return minus;
}

int getSecondMinus(const char* sFirstTime, const char * sSencondTime)
{
	const char *pTime = sFirstTime;
	int hour = (pTime[0] - '0') * 10 + (pTime[1] - '0');
	int minute = (pTime[3] - '0') * 10 + (pTime[4] - '0');
	int second = (pTime[6] - '0') * 10 + (pTime[7] - '0');
	int start = hour * 3600 + minute * 60 + second;

	pTime = sSencondTime;
	hour = (pTime[0] - '0') * 10 + (pTime[1] - '0');
	minute = (pTime[3] - '0') * 10 + (pTime[4] - '0');
	second = (pTime[6] - '0') * 10 + (pTime[7] - '0');
	int end = hour * 3600 + minute * 60 + second;

	int minus = start - end;
	if (minus < 0)
		minus += 24 * 60 * 60;
	return minus;
}

//char * getCurrDateMinute()
//{
//	time_t ltime;
//	struct tm *now;
//	static char timeBuffer[18];
//
//	time(&ltime);
//	now = localtime(&ltime);
//	sprintf(timeBuffer, "%04d%02d%02d-%02d:%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min);
//	return timeBuffer;
//}

char* getLocalTime(char* pBuffer)
{
	time_t now = time(NULL);
	struct tm tt;
	LOCALTIME(&tt, &now);

	sprintf(pBuffer, "%02d:%02d:%02d", tt.tm_hour, tt.tm_min, tt.tm_sec);
	return pBuffer;
}

//char* getCurrTimeNoGap()
//{
//	time_t ltime;
//	struct tm *now;
//	static char timeBuffer[10];
//
//	time(&ltime);
//	now = localtime(&ltime);
//	sprintf(timeBuffer, "%02d%02d%02d", now->tm_hour, now->tm_min, now->tm_sec);
//	return timeBuffer;
//}

//char* getMachineName()
//{
//#ifdef WIN32
//	static char nn[100];
//	//gethostname(nn,100);
//	//return nn;
//	return "WIN32";
//#else
//	static char nn[100];
//	gethostname(nn, 100);
//	return nn;
//#endif
//}
//
//char * getCurrDate()
//{
//	time_t ltime;
//	struct tm *now;
//	static char timeBuffer1[21];
//
//	time(&ltime);
//	now = localtime(&ltime);
//	sprintf(timeBuffer1, "%04d%02d%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
//	return timeBuffer1;
//}

// void printInfo(int level,char *sCppName,char* pFormat, ...)
// {
// 	if (level < loglevel)
// 	{
// 		return;
// 	}
// 
// 	char chMsg[32*1024];
// 	va_list pArg;
// 	
// 	static FILE *g_logfp=NULL;
// 	if (g_logfp==NULL)
// 	{
// 		g_logfp=mfopen("Syslog.log","at");	
// 		if (!g_logfp)
// 		{
// 			printf("not open %s.log\n");
// 			exit(0);
// 		}
// 		fprintf(g_logfp,"%s begin start\n",getCurrTime());
// 		fflush(g_logfp);
// 	}
// 	
// 	memset(chMsg,0,sizeof(chMsg));
// 	va_start(pArg, pFormat);
// 	vsprintf(chMsg+strlen(chMsg), pFormat, pArg);
// 	va_end(pArg);
// 	
// 	if(printstd)
// 		printf("%s",chMsg);	
// 	fprintf(g_logfp,"%s%c%s%c%s%c%02d%c%s: %s",getCurrDate(),\
// 		SEP_CHAR,getTime(),SEP_CHAR,getMachineName(),SEP_CHAR,level,SEP_CHAR,sCppName,chMsg);
// 	
// 	fflush(g_logfp);
// }

// 替换字符串中特征字符串为指定字符串
int ReplaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr)
{
	int  StringLen;
	char caNewString[2000];

	char *FindPos = strstr(sSrc, sMatchStr);
	if ((!FindPos) || (!sMatchStr))
		return -1;

	while (FindPos)
	{
		memset(caNewString, 0, sizeof(caNewString));
		StringLen = (int)(FindPos - sSrc);
		strncpy(caNewString, sSrc, StringLen);
		strcat(caNewString, sReplaceStr);
		strcat(caNewString, FindPos + strlen(sMatchStr));
		strcpy(sSrc, caNewString);

		FindPos = strstr(sSrc, sMatchStr);
	}

	return 0;
}

int CheckTime(char* localtime, char *sTime)
{
	int lTime1, lTime2;
	char strTime[20];

	sprintf(strTime, "%c%c%c%c%c%c", localtime[0], localtime[1], localtime[3], localtime[4], localtime[6], localtime[7]);
	lTime1 = atol(strTime);

	sprintf(strTime, "%c%c%c%c%c%c", sTime[0], sTime[1], sTime[3], sTime[4], sTime[6], sTime[7]);

	lTime2 = atol(strTime);

	if (lTime1 < lTime2) return -1;
	if (lTime1 == lTime2) return 0;
	if (lTime1 > lTime2) return 1;

	return 0;
}

//int getLongTime(char* sTime, char *pattern)
//{
//	tm tm_;                                    // 定义tm结构体
//	int year, month, day, hour, minute, second;// 定义时间的各个int临时变量
//
//	if (pattern == NULL)
//		sscanf(sTime, "%d:%d:%d", &year, &month, &day, &hour, &minute, &second);
//	else
//		sscanf(sTime, pattern, &year, &month, &day, &hour, &minute, &second);
//
//	//日期取本地日期
//	time_t ltime;
//	time(&ltime);
//#ifdef WINDOWS
//	tm* this_tm_ = localtime(&ltime);
//	tm_.tm_year = this_tm_->tm_year;
//	tm_.tm_yday = this_tm_->tm_yday;
//	tm_.tm_wday = this_tm_->tm_wday;
//	tm_.tm_mday = this_tm_->tm_mday;
//	tm_.tm_mon = this_tm_->tm_mon;
//#else
//	localtime_r(&ltime, &tm_);
//#endif
//
//	tm_.tm_hour = hour;                        // 时
//	tm_.tm_min = minute;                       // 分
//	tm_.tm_sec = second;                       // 秒
//	tm_.tm_isdst = 0;                          // 非夏令时。
//	int t_ = mktime(&tm_);					  // 将tm结构体转换成time_t格式
//	return t_;                                 // 返回值
//}

//int getLongDayTime(const char* sDay, const char* sTime, const char * patternDay, const char * patternTime)
//{
//	tm tm_;                                    // 定义tm结构体
//	int year, month, day, hour, minute, second;// 定义时间的各个int临时变量
//	if (patternDay == NULL)
//		sscanf(sDay, "%4d%2d%2d", &year, &month, &day);
//	else
//		sscanf(sDay, patternDay, &year, &month, &day);
//
//	if (patternTime == NULL)
//		sscanf(sTime, "%d:%d:%d", &hour, &minute, &second);
//	else
//		sscanf(sTime, patternTime, &hour, &minute, &second);
//
//	tm_.tm_year = year - 1900;                 // 年，由于tm结构体存储的是从1900年开始的时间，所以tm_year为int临时变量减去1900
//	tm_.tm_mon = month - 1;                    // 月，由于tm结构体的月份存储范围为0-11，所以tm_mon为int临时变量减去1
//	tm_.tm_mday = day;                         // 日
//	tm_.tm_hour = hour;                        // 时
//	tm_.tm_min = minute;                       // 分
//	tm_.tm_sec = second;                       // 秒
//	tm_.tm_isdst = 0;                          // 非夏令时。
//	int t_ = mktime(&tm_);					  // 将tm结构体转换成time_t格式
//	return t_;                                 // 返回值
//}

//char* getLogTime(char* pszDate)
//{
//	time_t now = time(NULL);
//	struct tm *tt = localtime(&now);
//	sprintf(pszDate, "%02d%02d%02d%02d", tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour);
//	return pszDate;
//}

char* getLogMinTime(char* pszDateMin)
{
	time_t now = time(NULL);
	struct tm tt;
	LOCALTIME(&tt, &now);
	sprintf(pszDateMin, "%02d%02d%02d%02d%02d", tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min);
	return pszDateMin;
}
//
//char* convertTimestampToStr(time_t ts)
//{
//	struct tm *tt = localtime(&ts);
//
//	static char timeBuffer[32];
//	sprintf(timeBuffer, "%04d%02d%02d,%02d:%02d:%02d,%d:%d",
//		tt->tm_year + 1900,
//		tt->tm_mon + 1,
//		tt->tm_mday,
//		tt->tm_hour,
//		tt->tm_min,
//		tt->tm_sec,
//		tt->tm_wday,
//		tt->tm_yday);
//	return timeBuffer;
//}
