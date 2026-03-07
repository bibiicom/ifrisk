/////////////////////////////////////////////////////////////////////////
///utility.h
///定义若干系统工具性的函数
/////////////////////////////////////////////////////////////////////////

#ifndef UTILITY_H
#define UTILITY_H

#include "platform.h"

///清除一个字符串后面的空格
///@param	s	要清楚空格的字符串，会被破坏
///@return	清除完空格的字符串，一般地址就是s
char *trim(char *s);

///获取当前的系统时间，以长格式方式给出
///@return	以长格式方式给出的当前时间
//char *getLongTime(void);
//
/////获取当前的系统时间，以短格式方式给出
/////@return	以短格式方式给出的当前时间
//char *getShortTime(void);
//
/////获取今天的时间 hh:mm:ss 格式
//char *getTodayTime(void);

///获取当前日期 yyyymmdd
struct tm GetNowDate(char *pszDate);

struct tm GetSlogDate(char *pszDate);

struct tm GetTM(time_t *pTime);

/*
///以下是宫耀东添加

///打开filename,作为DEBUG文件输出
///返回0:成功
///其他:失败
int dbgOpen(char *strFileName); 

///输出strBuffer信息到DEBUG文件中
int dbgPrint(char *strBuffer);

///关闭debug文件
int dbgClose();

///得到debug的文件
FILE *getFpDebugOutput();


///在定义DEBUG_LOG的情况下定义下列方法 ,否则定义为NULL
#ifdef DEBUG_LOG
#define DBG_OPEN(s)     dbgOpen(s)
#define DBG_STRING(s)	\
	{					\
		FILE *fp = getFpDebugOutput();\
		fprintf(fp, "[%s]:%s:%d\n", getLongTime(),__FILE__,__LINE__);\
		fprintf(fp,"%s",s!=NULL?s:"The String is NULL, Please check your program\n");\
		fflush(fp);\
	}	
				
#define DBG_CLASS(p) 	\
	{					\
		FILE *fp = getFpDebugOutput();\
		fprintf(fp, "[%s]:%s:%d\n", getLongTime(),__FILE__,__LINE__);\
		if(p!=NULL)   \
			(p)->dump(fp);\
		else \
			fprintf(fp,"The Class Pointer is NULL, Please check your program\n");	\
		fflush(fp);\
	}	
#define DBG_CLOSE	dbgClose	
#else
#define DBG_OPEN(s)		NULL
#define DBG_STRING(s)	NULL				
#define DBG_CLASS(p) 	NULL
#define DGB_CLOSE		NULL
#endif

#ifdef _DEBUG
#define TRACE0(sz)				{printf("[%s][%d] ",__FILE__,__LINE__);printf("%s",sz);}
#define TRACE1(sz, p1)          {printf("[%s][%d] ",__FILE__,__LINE__);printf(sz, p1);}
#define TRACE2(sz, p1, p2)      {printf("[%s][%d] ",__FILE__,__LINE__);printf(sz, p1, p2);}
#define TRACE3(sz, p1, p2, p3)  {printf("[%s][%d] ",__FILE__,__LINE__);printf(sz, p1, p2, p3);}
#else
#define TRACE0(sz)
#define TRACE1(sz, p1)
#define TRACE2(sz, p1, p2)
#define TRACE3(sz, p1, p2, p3)
#endif

#define TRACE	TRACE0
*/

/*
功能： 检查给定的str是否是符合如下标准的数值：
    没有小数点的整数
    或带有一个小数点的浮点数
    不带有TRIM功能，如果有空格，返回否
return 0: 否
rerurn 1: 是
*/
int isValidNumber(char *s);

/*
功能：检测字符串是否合法的日期格式yyyymmdd
    注意的确是合法的日期（包括对闰年229的考虑）
返回：
    return 0: 否
    rerurn 1: 是
 
*/
int isValidDate(char *s);

/*
功能：检测字符串是否合法的时间格式hhmmss
返回：
    return 0: 否
    rerurn 1: 是
*/
int isValidTime(char *s);

/**检查一个IP地址是否与指定IP地址匹配
*@param pIpAddress 字符串表示的IP地址("xxx.xxx.xxx.xxx")
*@param pIpMatch 字符串表示的要匹配的IP地址("xxx.xxx.xxx.xxx")
*@param pIpMask 字符串表示的掩码("xxx.xxx.xxx.xxx")
*@return true 地址匹配 false 地址不匹配
*@remark
*/
bool CheckIpAddressMatch(const char *pIpAddress, 
						 const char *pIpMatch, 
						 const char *pIpMask);

/**检查一个IP地址是否与指定IP地址匹配
*@param pIpAddress 整数表示的IP地址
*@param pIpMatch 整数表示的要匹配的IP地址
*@param pIpMask 整数表示的掩码
*@return true 地址匹配 false 地址不匹配
*@remark
*/
bool CheckIpAddressMatch(const unsigned long ulIpAddress, 
						 const unsigned long ulIpMatch, 
						 const unsigned long ulpMask);

///判断命令行参数是否说明应当采用恢复模式
///@param	argc	参数个数
///@param	argv	参数值表
///@return	true表示是恢复模式，false表示不是恢复模式
bool isRecovery(int argc, char *argv[]);

char * StrCCopy(char * dest, const char * source, size_t count);

#ifdef WIN32
//*********************************************************************************************************
//下面是硬盘序列号获取
//*********************************************************************************************************
#include <iostream>  
#include <windows.h>  

#define DFP_GET_VERSION         0x00074080   
#define DFP_SEND_DRIVE_COMMAND  0x0007c084   
#define DFP_RECEIVE_DRIVE_DATA  0x0007c088   

typedef struct _GETVERSIONOUTPARAMS   
{   
    char bVersion; // Binary driver version.   
    char bRevision; // Binary driver revision.   
    char bReserved; // Not used.   
    char bIDEDeviceMap; // Bit map of IDE devices.   
    int fCapabilities; // Bit mask of driver capabilities.   
    int dwReserved[4]; // For future use.   
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;   

#if _MSC_VER <= 1300

typedef struct _IDEREGS   
{   
    char bFeaturesReg; // Used for specifying SMART "commands".   
    char bSectorCountReg; // IDE sector count register   
    char bSectorNumberReg; // IDE sector number register   
    char bCylLowReg; // IDE low order cylinder value   
    char bCylHighReg; // IDE high order cylinder value   
    char bDriveHeadReg; // IDE drive/head register   
    char bCommandReg; // Actual IDE command.   
    char bReserved; // reserved for future use. Must be zero.   
} IDEREGS, *PIDEREGS, *LPIDEREGS;   

typedef struct _SENDCMDINPARAMS   
{   
    int cBufferSize; // Buffer size in bytes   
    IDEREGS irDriveRegs; // Structure with drive register values.   
    char bDriveNumber; // Physical drive number to send command to (0,1,2,3).   
    char bReserved[3]; // Reserved for future expansion.   
    int dwReserved[4]; // For future use.   
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;   

typedef struct _DRIVERSTATUS   
{   
    char bDriverError; // Error code from driver,   
    // or 0 if no error.   
    char bIDEStatus; // Contents of IDE Error register.   
    // Only valid when bDriverError   
    // is SMART_IDE_ERROR.   
    char bReserved[2]; // Reserved for future expansion.   
    int dwReserved[2]; // Reserved for future expansion.   
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;   

typedef struct _SENDCMDOUTPARAMS   
{   
    int cBufferSize; // Size of bBuffer in bytes   
    DRIVERSTATUS DriverStatus; // Driver status structure.   
    char bBuffer[512]; // Buffer of arbitrary length   
    // in which to store the data read from the drive.   
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;   
#endif _MSC_VER <= 1300

typedef struct _IDSECTOR   
{   
    USHORT wGenConfig;   
    USHORT wNumCyls;   
    USHORT wReserved;   
    USHORT wNumHeads;   
    USHORT wBytesPerTrack;   
    USHORT wBytesPerSector;   
    USHORT wSectorsPerTrack;   
    USHORT wVendorUnique[3];   
    CHAR sSerialNumber[20];   
    USHORT wBufferType;   
    USHORT wBufferSize;   
    USHORT wECCSize;   
    CHAR sFirmwareRev[8];   
    CHAR sModelNumber[40];   
    USHORT wMoreVendorUnique;   
    USHORT wDoubleWordIO;   
    USHORT wCapabilities;   
    USHORT wReserved1;   
    USHORT wPIOTiming;   
    USHORT wDMATiming;   
    USHORT wBS;   
    USHORT wNumCurrentCyls;   
    USHORT wNumCurrentHeads;   
    USHORT wNumCurrentSectorsPerTrack;   
    ULONG ulCurrentSectorCapacity;   
    USHORT wMultSectorStuff;   
    ULONG ulTotalAddressableSectors;   
    USHORT wSingleWordDMA;   
    USHORT wMultiWordDMA;   
    BYTE bReserved[128];   
} IDSECTOR, *PIDSECTOR;   

void funccloseHandle(HANDLE  hDrive);
//--------------------------------------------------------------  
//                      硬盘序列号  
//--------------------------------------------------------------  
BOOL GetHDSerial(char *lpszHD, int len=128);  

#endif

#endif
