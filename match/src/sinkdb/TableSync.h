#ifndef _TABLE_SYNC_H_
#define _TABLE_SYNC_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "platform.h"
#include "FieldDescribe.h"
#include "FieldData.h"
#include "TableDefine.h"

//#define MAX_LINE_LEGTH	10000

enum TTaskMethodType
{
	TMT_TABLE,
	TMT_PLSSP
};

enum TGroupNameType
{
	TMT_GN_INIT,
	TMT_GN_DOWN
};

const int MAX_TASK_COMMAND_LEN	= 4000;

struct TTaskMethod
{
	int nWorkerID;
	TGroupNameType nGroupNameType;
	TTaskMethodType nMethodType;
	CFieldDescribe *pDescirbe;
	const char *pszFieldName;
	const char *pszObjectName;
	const char *pszKeyName;
	char szInsertCommand[MAX_TASK_COMMAND_LEN];
	char szUpdateCommand[MAX_TASK_COMMAND_LEN];
	char szDeleteCommand[MAX_TASK_COMMAND_LEN];
	char szInsUpdCommand[MAX_TASK_COMMAND_LEN];
	char szInsertHead[MAX_TASK_COMMAND_LEN];
	char szUpdateHead[MAX_TASK_COMMAND_LEN];
	char szDeleteHead[MAX_TASK_COMMAND_LEN];
	char szInsUpdHead[MAX_TASK_COMMAND_LEN];
	int keyPosition[10];
};

//用于DisPatch记录是否处理过此类报文
//struct TSingleMethod
//{
//	int nWorkerID;
//	TGroupNameType nGroupNameType;
//	TTaskMethodType nMethodType;
//	CFieldDescribe *pDescirbe;
//	const char *pszFieldName;
//	const char *pszObjectName;
//	bool bFirst;
//};


enum TSQLType
{
	TSL_INSERT,
	TSL_UPDATE,
	TSL_DELETE,
	TSL_NOSQL
};

struct TSQLAction
{
	UF_INT8 nTaskNo;
	const char *pszFieldName;
	CFieldDescribe *pDescirbe;
	//void *pData;
	fpos_t fposData;
	TSQLType nActionType;
};


extern TTaskMethod g_TaskMethod[];
//extern TSingleMethod g_SingleMethod[];
extern TTinitTableInfo g_LoadTableInfo[];

//每TMDB_COMMIT_VOLUME个数据库操作Commit一次
const int TMDB_COMMIT_VOLUME = 1000;
//每隔TMDB_COMMIT_INTERVAL毫秒Commit
const int TMDB_COMMIT_INTERVAL = 5;
//每TMDB_CONNECT_TIMER_COUNT×TMDB_COMMIT_INTERVAL毫秒尝试重连一次数据库
//const int TMDB_CONNECT_TIMER_COUNT = 100;

const int TMDB_ACTION_MAX_LENGTH = 4096;
//*.dbcsv 可回收大小
const int TMDB_WF_MAXSIZE = 256 * 1024 * 1024;

#endif
