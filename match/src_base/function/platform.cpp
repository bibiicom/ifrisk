#include "platform.h"

UF_INT8 g_nCurrentTime = 0;

#ifdef WIN32
char	*optarg;		// global argument pointer
int		optind = 0; 	// global argv index

int getopt(int argc, char *argv[], char *optstring)
{
	static char *next = NULL;
	if (optind == 0)
		next = NULL;

	optarg = NULL;

	if (next == NULL || *next == ('\0'))
	{
		if (optind == 0)
			optind++;

		if (optind >= argc || argv[optind][0] != ('-') || argv[optind][1] == ('\0'))
		{
			optarg = NULL;
			if (optind < argc)
				optarg = argv[optind];
			return EOF;
		}

		if (strcmp(argv[optind], ("--")) == 0)
		{
			optind++;
			optarg = NULL;
			if (optind < argc)
				optarg = argv[optind];
			return EOF;
		}

		next = argv[optind];
		next++;		// skip past -
		optind++;
	}

	char c = *next++;
	char *cp = strchr(optstring, c);

	if (cp == NULL || c == (':'))
		return ('?');

	cp++;
	if (*cp == (':'))
	{
		if (*next != ('\0'))
		{
			optarg = next;
			next = NULL;
		}
		else if (optind < argc)
		{
			optarg = argv[optind];
			optind++;
		}
		else
		{
			return ('?');
		}
	}

	return c;
}

#else

char* strupr(char *str)
{
	if (str == NULL)
		return NULL;

	char *p = str;
	while (*p != '\0')
	{
		if (*p >= 'a' && *p <= 'z')
			*p = (*p) - 32;
		p++;
	}
	return str;
}

char* strlwr(char *str)
{
	if (str == NULL)
		return NULL;

	char *p = str;
	while (*p != '\0')
	{
		if (*p >= 'A' && *p <= 'Z')
			*p = (*p) + 32;
		p++;
	}
	return str;
}
#endif

// I64S print64(UF_INT8 a)
// {
// 	I64S s;
// #ifdef WINDOWS
// 	sprintf(s.cs, "%I64d", a);
// #else
// 	sprintf(s.cs, "%lld", a);
// #endif
// 	return s;
// }
// 
// I64S print64x(UF_INT8 a)
// {
// 	I64S s;
// #ifdef WINDOWS
// 	sprintf(s.cs, "%I64X", a);
// #else
// 	sprintf(s.cs, "%llX", a);
// #endif
// 	return s;
// }

//static unsigned char source8[8] = { 12,1,5,13,7,2,9,11 };
//static unsigned char source4[4] = { 9,1,3,7 };
//static unsigned char gene8[8];
//static unsigned char gene4[4];

//void init_gene(long gene)
//{
//	if (gene <= 0)
//		return;
//	int i;
//	for (i = 0; i < 8; i++)
//	{
//		gene8[i] = source8[i];
//	}
//
//	for (i = 0; i < 4; i++)
//	{
//		gene4[i] = source4[i];
//	}
//
//	int g1 = gene % 8;
//	int g2 = gene % 4;
//
//	gene8[0] = source8[g1];
//	gene8[g1] = source8[0];
//
//	gene4[0] = source4[g2];
//	gene4[g2] = source4[0];
//}

//void doodle8(unsigned char *p)
//{
//	for (int i = 0; i < 8; i++)
//	{
//#ifndef _BIG_ENDIAN_
//		p[i] = p[i] + gene8[i];
//#else
//		p[i] = p[i] + gene8[7 - i];
//#endif
//	}
//}
//void undoodle8(unsigned char *p)
//{
//	for (int i = 0; i < 8; i++)
//	{
//#ifndef _BIG_ENDIAN_
//		p[i] = p[i] - gene8[i];
//#else
//		p[i] = p[i] - gene8[7 - i];
//#endif
//	}
//}
//
//void doodle4(unsigned char *p)
//{
//	for (int i = 0; i < 4; i++)
//	{
//#ifndef _BIG_ENDIAN_
//		p[i] = p[i] + gene4[i];
//#else
//		p[i] = p[i] + gene4[3 - i];
//#endif
//	}
//}
//void undoodle4(unsigned char *p)
//{
//	for (int i = 0; i < 4; i++)
//	{
//#ifndef _BIG_ENDIAN_
//		p[i] = p[i] - gene4[i];
//#else
//		p[i] = p[i] - gene4[3 - i];
//#endif
//	}
//}


#ifdef WINDOWS
#include <direct.h>
#include <io.h>
#define MKDIR(x,y) mkdir(x)
#else
#include <unistd.h>
#include <sys/stat.h>
#include <fnmatch.h>
#define MKDIR(x,y) mkdir(x,y)
#endif

void convertPath(char *target, const char *source)
{
	const char *s;
	char *t;
	for (s = source, t = target; ((s - source) < MAX_PATH_LEN) && (*s != '\0'); s++, t++)
	{
		if (strchr(ALL_SPLITS, *s) != NULL)
		{
			*t = PATH_SPLIT;
		}
		else
		{
			*t = *s;
		}
	}
	*t = '\0';
}

FILE *mfopen(const char *filename, const char *mode, bool bExit)
{
	char actualName[MAX_PATH_LEN + 1];
	convertPath(actualName, filename);
	FILE *pFile = fopen(actualName, mode);

	if (pFile == NULL)
	{
		char m_sError[512];
#ifdef WINDOWS
		sprintf(m_sError, "Can not open file[%s]:ErrorID[%d]", filename, GetLastError());
#else
		sprintf(m_sError, "Can not open file[%s]:ErrorMsg[%s]", filename, strerror(errno));
#endif
		if (bExit)
			EMERGENCY_EXIT(m_sError);
	}
	return pFile;
}

bool makedir(const char *path)
{
	char str[512];
	convertPath(str, path);
	int len = (int)strlen(str);
	for (int i = 1; i < len; i++)
	{
		if (str[i] == PATH_SPLIT)
		{
			str[i] = '\0';
			if (access(str, 0) != 0)
			{
				MKDIR(str, 0775);
			}
			str[i] = PATH_SPLIT;
		}
	}
	if (len > 0 && access(str, 0) != 0)
	{
		MKDIR(str, 0775);
	}
	return (access(str, 0) == 0);
}

void getRealFolder(char *All_Name, char *FolderName)
{
	sprintf(FolderName, "%s", All_Name);
	for (int i = (int)strlen(FolderName); i > 0; i--)
	{
		if (FolderName[i] != PATH_SPLIT)
		{
			FolderName[i] = '\0';
		}
		else
		{
			break;
		}
	}
}

void getFileName(char *All_Name, char *FileName)
{
	size_t i = 0;
	size_t j = 0; //这是给FileName赋值的起点
	for (i = 0; i <= strlen(All_Name); i++)
	{
		if (All_Name[i] == PATH_SPLIT)
		{
			j = 0;
			FileName[j] = '\0';
		}
		else
		{
			FileName[j] = All_Name[i];
			j++;
		}

		if (All_Name[i] == '\0')
		{
			return;
		}
	}
}

vector<string> findNames(const char* preFileName, int nAttribute)
{
	vector<string> files;
	char actualPreFileName[MAX_PATH_LEN + 1];
	convertPath(actualPreFileName, preFileName);

	//由于支持 ../app/*.dll 和 ../app*.dll的模式，preFileName给出来的不一定是真正的名字
	//所以这里获取正在的文件夹的名字和文件的名字，便于后续使用和比对
	char actualFileName[MAX_PATH_LEN + 1];
	getFileName(actualPreFileName, actualFileName);

	char actualFolderPath[MAX_PATH_LEN + 1];
	getRealFolder(actualPreFileName, actualFolderPath);
	//	printf("%s : %s",actualFolderPath,FileName);

#ifdef WINDOWS
	struct _finddata_t fileInfo;
#ifdef _WIN64
	__int64 findResult;
#else
	int findResult;
#endif
	findResult = _findfirst(actualPreFileName, &fileInfo);
	if (findResult == -1)
	{
		_findclose(findResult);
		return files;
	}
	bool flag = 0;

	char fileName[MAX_PATH_LEN + 1];
	do
	{
		sprintf(fileName, "%s%s", actualFolderPath, fileInfo.name);
		//if (fileInfo.attrib == _A_ARCH)
		if (nAttribute >= 0)
		{
			if (!fileInfo.attrib & nAttribute)
				continue;
		}
		files.push_back(fileName);
	} while (_findnext(findResult, &fileInfo) != -1);

	_findclose(findResult);
#else
	DIR *dir = opendir(actualFolderPath);
	if (NULL == dir)
	{
		return files;
	}

	struct dirent * filename;
	char fileName[MAX_PATH_LEN + 1];

	while ((filename = readdir(dir)) != NULL)
	{
		if (strcmp(filename->d_name, ".") == 0)
			continue;
		if (strcmp(filename->d_name, "..") == 0)
			continue;

		if (nAttribute >= 0)
		{
			if (filename->d_type != nAttribute)
				continue;
		}
		//if( strncmp(filename->d_name,actualFileName,strlen(actualFileName))==0 ) 
		if (fnmatch(actualFileName, filename->d_name, FNM_PATHNAME | FNM_PERIOD) == 0)
		{
			sprintf(fileName, "%s%s", actualFolderPath, filename->d_name);
			files.push_back(fileName);
		}
	}
	closedir(dir);
#endif

	return files;
}

void f_print_to_std(const char *format, ...)
{
	char buffer[4096] = { 0 };

	va_list v;
	va_start(v, format);
	vsnprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), format, v);
	va_end(v);

	time_t now = time(NULL);
	char timeBuffer[100];
	struct tm tt;
	LOCALTIME(&tt, &now);
	strftime(timeBuffer, 100, "%m-%d %H:%M:%S", &tt);
	printf("[%s]: %s \n", timeBuffer, buffer);
	fflush(stdout);
}

void f_print_to_std_us(const char *format, ...)
{
	char buffer[4096] = { 0 };

	va_list v;
	va_start(v, format);
	vsnprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), format, v);
	va_end(v);

	char sTimeBuffer[4098];
#ifdef WIN32	
	struct _timeb timeout;
	_ftime(&timeout);
	struct tm tt;
	LOCALTIME(&tt, &(timeout.time));
	int m_nSendUsec = tt.tm_sec * 1000 + timeout.millitm;
	sprintf(sTimeBuffer, "[%02d-%02d %02d:%02d:%02d %06d]", tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, timeout.millitm);
#else
	struct timeval timeout;
	gettimeofday(&timeout, 0);
	struct tm tt;
	LOCALTIME(&tt, &(timeout.tv_sec));
	int m_nSendUsec = tt.tm_sec * 1000000 + timeout.tv_usec;
	sprintf(sTimeBuffer, "[%02d-%02d %02d:%02d:%02d %06d]", tt.tm_mon, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, timeout.tv_usec);
#endif

	printf("%s: %s \n", sTimeBuffer, buffer);
	fflush(stdout);
}

void f_emergency_exit(const char *format, ...)
{
	char buffer[4096] = { 0 };

	va_list v;
	va_start(v, format);
	vsnprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), format, v);
	va_end(v);

	time_t now = time(NULL);
	char timeBuffer[100];
	struct tm tt;
	LOCALTIME(&tt, &now);
	strftime(timeBuffer, 100, "%m-%d %H:%M:%S", &tt);
	printf("[%s]: %s\n", timeBuffer, buffer);
	fflush(stdout);
	char *__pNull = NULL; *__pNull = 0;
}

//const char* getTick(double value)
//{
//	if (value < 0.0000000001 && value >-0.0000000001)
//		return "0.0000000001";
//	if (value <= 450359.9627370496 && value >= -450359.9627370496)
//		return "0.0000000001";
//	else if (value <= 4503599.627370496 && value >= -4503599.627370496)
//		return "0.000000001";
//	else if (value <= 45035996.27370496 && value >= -45035996.27370496)
//		return "0.00000001";
//	else if (value <= 450359962.7370496 && value >= -450359962.7370496)
//		return "0.0000001";
//	else if (value <= 4503599627.370496 && value >= -4503599627.370496)
//		return "0.000001";
//	else if (value <= 45035996273.70496 && value >= -45035996273.70496)
//		return "0.00001";
//	else if (value <= 450359962737.0496 && value >= -450359962737.0496)
//		return "0.0001";
//	else if (value <= 4503599627370.496 && value >= -4503599627370.496)
//		return "0.001";
//	else if (value <= 45035996273704.96 && value >= -45035996273704.96)
//		return "0.01";
//	else if (value <= 450359962737049.6 && value >= -450359962737049.6)
//		return "0.1";
//	else
//		return "1";
//}

string LargeNumberToString(double value)
{
	if (value < 0.9 && value >-0.9)
		return "0";

	if (value <= 4503599627370496 && value >= -4503599627370496)
	{
		char buf[32] = { 0 };
		sprintf(buf, "%.0lf", value);
		return buf;
	}
	return "";
}

//string LargeNumberToStringJson(double value)
//{
//	if (value < 0.0000000001 && value >-0.0000000001)
//		return "0";
//
//	if (value <= 4503599627370496 && value >= -4503599627370496)
//	{
//		char buf[32] = { 0 };
//		sprintf(buf, "%.0lf", value);
//		return buf;
//	}
//	return "null";
//}


//double a = 0.5;
//for (int i = 0; i <= 52; i++)
//{
//	a *= 2;
//	double p = 1;
//	int j = 52;
//	for (; j > i; j--)
//	{
//		p /= 2;
//	}
//	printf("<%0.f -> %d -> %d -> %s\n", a, i, 52 - i, doubleToStringFormat(p).c_str());
//}

//	<1 -> 0 -> 52 -> 2.2204460492503131e-16
//	< 2 -> 1 -> 51 -> 4.4408920985006262e-16
//	< 4 -> 2 -> 50 -> 8.8817841970012523e-16
//	< 8 -> 3 -> 49 -> 1.7763568394002505e-15
//	< 16 -> 4 -> 48 -> 3.5527136788005009e-15
//	< 32 -> 5 -> 47 -> 7.1054273576010019e-15
//	< 64 -> 6 -> 46 -> 1.4210854715202004e-14
//	< 128 -> 7 -> 45 -> 2.8421709430404007e-14
//	< 256 -> 8 -> 44 -> 5.6843418860808015e-14
//	< 512 -> 9 -> 43 -> 1.1368683772161603e-13
//	< 1024 -> 10 -> 42 -> 2.2737367544323206e-13
//	< 2048 -> 11 -> 41 -> 4.5474735088646412e-13
//	< 4096 -> 12 -> 40 -> 9.0949470177292824e-13
//	< 8192 -> 13 -> 39 -> 1.8189894035458565e-12
//	< 16384 -> 14 -> 38 -> 3.6379788070917130e-12
//	< 32768 -> 15 -> 37 -> 7.2759576141834259e-12
//	< 65536 -> 16 -> 36 -> 1.4551915228366852e-11
//	< 131072 -> 17 -> 35 -> 2.9103830456733704e-11
//	< 262144 -> 18 -> 34 -> 5.8207660913467407e-11
//	< 524288 -> 19 -> 33 -> 1.1641532182693481e-10
//	< 1048576 -> 20 -> 32 -> 2.3283064365386963e-10
//	< 2097152 -> 21 -> 31 -> 4.6566128730773926e-10
//	< 4194304 -> 22 -> 30 -> 9.3132257461547852e-10
//	< 8388608 -> 23 -> 29 -> 1.8626451492309570e-09
//	< 16777216 -> 24 -> 28 -> 3.7252902984619141e-09
//	< 33554432 -> 25 -> 27 -> 7.4505805969238281e-09
//	< 67108864 -> 26 -> 26 -> 1.4901161193847656e-08
//	< 134217728 -> 27 -> 25 -> 2.9802322387695313e-08
//	< 268435456 -> 28 -> 24 -> 5.9604644775390625e-08
//	< 536870912 -> 29 -> 23 -> 1.1920928955078125e-07
//	< 1073741824 -> 30 -> 22 -> 2.3841857910156250e-07
//	< 2147483648 -> 31 -> 21 -> 4.7683715820312500e-07
//	< 4294967296 -> 32 -> 20 -> 9.5367431640625000e-07
//	< 8589934592 -> 33 -> 19 -> 1.9073486328125000e-06
//	< 17179869184 -> 34 -> 18 -> 3.8146972656250000e-06
//	< 34359738368 -> 35 -> 17 -> 7.6293945312500000e-06
//	< 68719476736 -> 36 -> 16 -> 1.5258789062500000e-05
//	< 137438953472 -> 37 -> 15 -> 3.0517578125000000e-05
//	< 274877906944 -> 38 -> 14 -> 6.1035156250000000e-05
//	< 549755813888 -> 39 -> 13 -> 0.00012207031250
//	< 1099511627776 -> 40 -> 12 -> 0.0002441406250
//	< 2199023255552 -> 41 -> 11 -> 0.000488281250
//	< 4398046511104 -> 42 -> 10 -> 0.00097656250
//	< 8796093022208 -> 43 -> 9 -> 0.0019531250
//	< 17592186044416 -> 44 -> 8 -> 0.003906250
//	< 35184372088832 -> 45 -> 7 -> 0.00781250
//	< 70368744177664 -> 46 -> 6 -> 0.0156250
//	< 140737488355328 -> 47 -> 5 -> 0.031250
//	< 281474976710656 -> 48 -> 4 -> 0.06250
//	< 562949953421312 -> 49 -> 3 -> 0.1250
//	< 1125899906842624 -> 50 -> 2 -> 0.250
//	< 2251799813685248 -> 51 -> 1 -> 0.50
//	< 4503599627370496 -> 52 -> 0 -> 1.0
//最后一列的最小变化单位小于tick值的位数，如果接近1也可以用

string doubleToString(double value, int precision)
{
	//保留十位小数，用于存储csv文件和数据库
	if (value == DOUBLE_NaN)
		return "";

	char buf[32] = { 0 };
	switch (precision)
	{
	case 15:
		if (value < 4 && value > -4)
		{
			if (value < 0.9e-15 && value > -0.9e-15)
				return "0.0";
			sprintf(buf, "%.15lf", value);
			break;
		}
	case 14:
		if (value < 32 && value > -32)
		{
			if (value < 0.9e-14 && value > -0.9e-14)
				return "0.0";
			sprintf(buf, "%.14lf", value);
			break;
		}
	case 13:
		if (value < 256 && value > -256)//1.13很小可以
		{
			if (value < 0.9e-13 && value > -0.9e-13)
				return "0.0";
			sprintf(buf, "%.13lf", value);
			break;
		}
	case 12:
		if (value < 4096 && value > -4096)
		{
			if (value < 0.9e-12 && value > -0.9e-12)
				return "0.0";
			sprintf(buf, "%.12lf", value);
			break;
		}
	case 11:
		if (value < 32768 && value > -32768)
		{
			if (value < 0.9e-11 && value > -0.9e-11)
				return "0.0";
			sprintf(buf, "%.11lf", value);
			break;
		}
	case 10:
		if (value < 262144 && value > -262144)
		{
			if (value < 0.9e-10 && value > -0.9e-10)
				return "0.0";
			sprintf(buf, "%.10lf", value);
			break;
		}
	case 9:
		if (value < 4194304 && value > -4194304)
		{
			if (value < 0.9e-9 && value > -0.9e-9)
				return "0.0";
			sprintf(buf, "%.9lf", value);
			break;
		}
	case 8:
		if (value < 33554432 && value > -33554432)
		{
			if (value < 0.9e-8 && value > -0.9e-8)
				return "0.0";
			sprintf(buf, "%.8lf", value);
			break;
		}
	case 7:
		if (value < 268435456 && value > -268435456) //1.19很小可以
		{
			if (value < 0.9e-7 && value > -0.9e-7)
				return "0.0";
			sprintf(buf, "%.7lf", value);
			break;
		}
	case 6:
		if (value < 4294967296 && value > -4294967296)
		{
			if (value < 0.9e-6 && value > -0.9e-6)
				return "0.0";
			sprintf(buf, "%.6lf", value);
			break;
		}
	case 5:
		if (value < 34359738368 && value > -34359738368)
		{
			if (value < 0.9e-5 && value > -0.9e-5)
				return "0.0";
			sprintf(buf, "%.5lf", value);
			break;
		}
	case 4:
		if (value < 274877906944 && value > -274877906944)
		{
			if (value < 0.9e-4 && value > -0.9e-4)
				return "0.0";
			sprintf(buf, "%.4lf", value);
			break;
		}
	case 3:
		if (value < 4398046511104 && value > -4398046511104)
		{
			if (value < 0.9e-3 && value > -0.9e-3)
				return "0.0";
			sprintf(buf, "%.3lf", value);
			break;
		}
	case 2:
		if (value < 35184372088832 && value > -35184372088832)
		{
			if (value < 0.9e-2 && value > -0.9e-2)
				return "0.0";
			sprintf(buf, "%.2lf", value);
			break;
		}
	case 1:
		if (value < 281474976710656 && value > -281474976710656)
		{
			if (value < 0.9e-1 && value > -0.9e-1)
				return "0.0";
			sprintf(buf, "%.1lf", value);
			break;
		}
	default:
		return doubleToStringFormat(value);
	}

	char *p = buf + strlen(buf) - 1;
	while (*(p) == '0' && p >= buf)
		p--;
	if (*(p) == '.')
		*(p + 2) = '\0';
	else
		*(p + 1) = '\0';
	return buf;
}

string doubleToStringFormat(double value)
{
	if (value == DOUBLE_NaN)
		return "";

	char buffer[32];
#if defined(_MSC_VER) && defined(__STDC_SECURE_LIB__) // Use secure version with visual studio 2005 to avoid warning. 
	sprintf_s(buffer, sizeof(buffer), "%#.17g", value);
#else	
	sprintf(buffer, "%#.17g", value);
#endif
	char* ch = buffer + strlen(buffer) - 1;

	if (*ch == '.')
	{
		//防止出现有小数点没有0的情况 20074574067650760. 这里补一个0
		*(ch + 1) = '0';
		*(ch + 2) = '\0';
		return buffer;
	}

	if (*ch != '0') return buffer; // nothing to truncate, so save time

	while (ch > buffer && *ch == '0')
	{
		--ch;
	}
	char* last_nonzero = ch;
	while (ch >= buffer)
	{
		switch (*ch)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			--ch;
			continue;
		case '.':
			// Truncate zeroes to save bytes in output, but keep one.
			*(last_nonzero + 2) = '\0';
			return buffer;
		default:
			return buffer;
		}
	}
	return buffer;
}

double getMaxValue(int precision)
{
	switch (precision)
	{
	case 15:
		return 4;
	case 14:
		return 32;
	case 13:
		return 256;
	case 12:
		return 4096;
	case 11:
		return 32768;
	case 10:
		return 262144;
	case 9:
		return 4194304;
	case 8:
		return 33554432;
	case 7:
		return 268435456;
	case 6:
		return 4294967296;
	case 5:
		return 34359738368;
	case 4:
		return 274877906944;
	case 3:
		return 4398046511104;
	case 2:
		return 35184372088832;
	case 1:
		return 281474976710656;
	default:
		return 4503599627370496;
	}
}

double doubleCompareSmall(double value, int precision)
{
	switch (precision)
	{
	case 15:
		if (value < 4 && value > -4)
			return 8.8817841970012523e-16;
	case 14:
		if (value < 32 && value > -32)
			return 7.1054273576010019e-15;
	case 13:
		if (value < 256 && value > -256)
			return 5.6843418860808015e-14;
	case 12:
		if (value < 4096 && value > -4096)
			return 9.0949470177292824e-13;
	case 11:
		if (value < 32768 && value > -32768)
			return 7.2759576141834259e-12;
	case 10:
		if (value < 262144 && value > -262144)
			return 5.8207660913467407e-11;
	case 9:
		if (value < 4194304 && value > -4194304)
			return 9.3132257461547852e-10;
	case 8:
		if (value < 33554432 && value > -33554432)
			return 7.4505805969238281e-09;
	case 7:
		if (value < 268435456 && value > -268435456)
			return 5.9604644775390625e-08;
	case 6:
		if (value < 4294967296 && value > -4294967296)
			return 9.5367431640625000e-07;
	case 5:
		if (value < 34359738368 && value > -34359738368)
			return 7.6293945312500000e-06;
	case 4:
		if (value < 274877906944 && value > -274877906944)
			return 6.1035156250000000e-05;
	case 3:
		if (value < 4398046511104 && value > -4398046511104)
			return 0.00097656250;
	case 2:
		if (value < 35184372088832 && value > -35184372088832)
			return 0.00781250;
	case 1:
		if (value < 281474976710656 && value > -281474976710656)
			return 0.06250;
	default:
	{
		double base = 1;
		double basevalue = 4503599627370496;

		for (int i = 0; i < 100; i++)
		{
			if (value < basevalue && value > -basevalue)
				return base;
			basevalue *= 2;
			base *= 2;
		}
		return base;
	}
	}
}

double doubleCompareTick(double value, int precision)
{
	switch (precision)
	{
	case 15:
		if (value < 4 && value > -4)
			return 1e-15;
	case 14:
		if (value < 32 && value > -32)
			return 1e-14;
	case 13:
		if (value < 256 && value > -256)
			return 1e-13;
	case 12:
		if (value < 4096 && value > -4096)
			return 1e-12;
	case 11:
		if (value < 32768 && value > -32768)
			return 1e-11;
	case 10:
		if (value < 262144 && value > -262144)
			return 1e-10;
	case 9:
		if (value < 4194304 && value > -4194304)
			return 1e-09;
	case 8:
		if (value < 33554432 && value > -33554432)
			return 1e-08;
	case 7:
		if (value < 268435456 && value > -268435456)
			return 1e-07;
	case 6:
		if (value < 4294967296 && value > -4294967296)
			return 1e-06;
	case 5:
		if (value < 34359738368 && value > -34359738368)
			return 1e-05;
	case 4:
		if (value < 274877906944 && value > -274877906944)
			return 1e-04;
	case 3:
		if (value < 4398046511104 && value > -4398046511104)
			return 1e-03;
	case 2:
		if (value < 35184372088832 && value > -35184372088832)
			return 1e-02;
	case 1:
		if (value < 281474976710656 && value > -281474976710656)
			return 1e-01;
	default:
	{
		return 1;
		//if (value < 4503599627370496 && value > -4503599627370496)
		//	return 1;
		//if (value < 36028797018963968 && value > -36028797018963968)
		//	return 10;
		//if (value < 2.8823037615171174e+17 && value > -2.8823037615171174e+17)
		//	return 100;
		//if (value < 2.3058430092136940e+18 && value > -2.3058430092136940e+18)
		//	return 1000;
		//if (value < 3.6893488147419103e+19 && value > -3.6893488147419103e+19)
		//	return 10000;
		//if (value < 2.9514790517935283e+20 && value > -2.9514790517935283e+20)
		//	return 100000;
		//if (value < 2.3611832414348226e+21 && value > -2.3611832414348226e+21)
		//	return 1000000;
		//if (value < 3.7778931862957162e+22 && value > -3.7778931862957162e+22)
		//	return 10000000;
		//if (value < 3.0223145490365729e+23 && value > -3.0223145490365729e+23)
		//	return 100000000;
		//if (value < 2.4178516392292583e+24 && value > -2.4178516392292583e+24)
		//	return 1000000000;
		//if (value < 3.8685626227668134e+25 && value > -3.8685626227668134e+25)
		//	return 10000000000;
		//if (value < 3.0948500982134507e+26 && value > -3.0948500982134507e+26)
		//	return 100000000000;
		//if (value < 2.4758800785707605e+27 && value > -2.4758800785707605e+27)
		//	return 1000000000000;
		//if (value < 3.9614081257132169e+28 && value > -3.9614081257132169e+28)
		//	return 10000000000000;
	}
	}
	return 1;
}

void str_print(const char* str, int str_len)
{
	int i = 0;
	for (; i < str_len; i++)
		printf("%c", str[i]);
}

#ifdef LINUX
//将整数转化为字符串：
//value，转换对象
//str，转换后存储字符串的数组
//radix，转换（给于的字符串）的长度
char* itoa(int value, char *str, int radix)
{
	static char dig[] =	"0123456789abcdefghijklmnopqrstuvwxyz";
	int n = 0, neg = 0;
	unsigned int v;
	char* p, *q;
	char c;
	/* 负数：十进制直接在前面加负号，其它进制用补码 */
	if (radix == 10 && value < 0) 
	{
		v = -value;
		neg = 1;
	}
	else
	{
		v = value;
	}
	/* 其它进制的负数转为unsigned int类型，即取了补码 */
	do {
		str[n++] = dig[v%radix];
		v /= radix;
	} while (v);
	if (neg)
		str[n++] = '-';
	str[n] = '\0';
	/* 字符串反转 */
	for (p = str, q = p + (n - 1); p < q; ++p, --q)
		c = *p, *p = *q, *q = c;
	return str;
}
#endif
