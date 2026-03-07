!!function!!
int isInitPackage(char *usage)
{
	if (strcmp(usage,"InitTable")==0)
	{
		return 1;
	}	
	return 0;
}
int isDownPackage(char *usage)
{
	if (strcmp(usage,"DownTable")==0)
	{
		return 1;
	}	
	return 0;
}
char *getTableName(char *name)
{
	return name;
}
!!endfunction!!
#include "TableSync.h"

//WorkerID为0表示同步工作线程
//WorkerID为-1表示结束
TTaskMethod g_TaskMethod[] =
{
!!enter XTP!!
!!travel fields!!
!!if(valid_name("usage"))!!
	!!let tableName=getTableName(@name)!!
	!!if isInitPackage(@usage)!!
		!!if(valid_name("multi"))!!
			!!let thisSyncNo=@syncno!!
			!!loop 1;atoi(get_string("multi"));1!!
	{!!@thisSyncNo!!,TMT_GN_INIT,TMT_TABLE,&(C!!@tableName!!Field::m_Describe),"C!!@tableName!!Field","!!@tableName!!","!!@DBPK!!",{0},{0},{0},{0}},
			!!let thisSyncNo=to_string(atoi(get_string("thisSyncNo"))+1)!!
			!!endloop!!
		!!else!!
	{!!@syncno!!,TMT_GN_INIT,TMT_TABLE,&(C!!@tableName!!Field::m_Describe),"C!!@tableName!!Field","!!@tableName!!","!!@DBPK!!",{0},{0},{0},{0}},		
		!!endif!!
	!!endif!!
	!!if isDownPackage(@usage)!!
		!!if(valid_name("multi"))!!
			!!let thisSyncNo=@syncno!!
			!!loop 1;atoi(get_string("multi"));1!!
	{!!@thisSyncNo!!,TMT_GN_DOWN,TMT_TABLE,&(C!!@tableName!!Field::m_Describe),"C!!@tableName!!Field","!!@tableName!!","!!@DBPK!!",{0},{0},{0},{0}},
			!!let thisSyncNo=to_string(atoi(get_string("thisSyncNo"))+1)!!
			!!endloop!!
		!!else!!
	{!!@syncno!!,TMT_GN_DOWN,TMT_TABLE,&(C!!@tableName!!Field::m_Describe),"C!!@tableName!!Field","!!@tableName!!","!!@DBPK!!",{0},{0},{0},{0}},		
		!!endif!!
	!!endif!!	
!!endif!!
!!next!!
	{-1,TMT_GN_INIT,TMT_TABLE,NULL,"","","",{0},{0},{0},{0}}
};

//TSingleMethod g_SingleMethod[] =
//{
//!!travel fields!!
//!!if(valid_name("usage"))!!
//	!!let tableName=getTableName(@name)!!
//	!!if isDownPackage(@usage)!!
//	{!!@syncno!!,TMT_GN_DOWN,TMT_TABLE,&(C!!@tableName!!Field::m_Describe),"C!!@tableName!!Field","!!@tableName!!",true},		
//	!!endif!!	
//!!endif!!
//!!next!!
//	{-1,TMT_GN_INIT,TMT_TABLE,NULL,"","",true}
//};

TTinitTableInfo g_LoadTableInfo[] = 
{
!!travel fields!!
!!if(valid_name("usage") && isInitPackage(@usage))!!
	!!let tableName=@name!!
	!!if !(valid_name("dbload") && !strcmp(@dbload,"no"))!!
	{"!!@tableName!!",&C!!@tableName!!Field::m_Describe},
	!!endif!!
!!endif!!
!!next!!
	{NULL,NULL}				//一定要有这一行，表示结束
};

!!leave!!
