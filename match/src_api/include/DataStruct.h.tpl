!!enter project!!
!!let apiname=@apiname!!
!!let apiid=@apiid!!
!!let APINAME=@APINAME!!
!!let APIID=@APIID!!
!!leave!!
!!function!!
#include <set>

using namespace std;

struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

typedef set<char *, ltstr> CTypeSet;
CTypeSet g_setUsedType;

bool InUse(char *name)
{
	char buf[100] = "C";
	strcpy(buf+1, name);
	strcat(buf,"Type");
	return (g_setUsedType.find(buf)!=g_setUsedType.end());
}

!!endfunction!!
!!enter FTD!!
!!enter fields!!
!!travel self!!
!!travel self!!
!!command g_setUsedType.insert(@type);!!
!!next!!
!!next!!
!!leave!!
!!leave!!
/////////////////////////////////////////////////////////////////////////
///!!@apiname!!!!@apiid!!DataStruct.h
///定义了客户端接口使用的业务数据结构
/////////////////////////////////////////////////////////////////////////

#ifndef __!!@APINAME!!!!@APIID!!_DATASTRUCT_H__
#define __!!@APINAME!!!!@APIID!!_DATASTRUCT_H__

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

namespace !!@apiname!!!!@apiid!!Byte
{
	#define !!@APINAME!!_CHAIN_FIRST			'F'		//第一个
	#define !!@APINAME!!_CHAIN_CONTINUE		'C'		//还有后续
	#define !!@APINAME!!_CHAIN_LAST			'L'		//最后一个
	#define !!@APINAME!!_CHAIN_SINGLE		'S'		//只有一个报文
	///订阅主题分类
	!!enter FTD!!
	!!enter topics!!	
	!!travel self!!
	#define !!@APINAME!!_!!@name!! "!!@value!!"	//!!@comment!!
	!!next!!
	!!leave!!	
	!!leave!!
	///如果有其他流水，应用程序中可以自行设置，但是不能和上面的重合

	!!enter FTD!!
	!!enter types!!
	!!travel self!!
	!!if InUse(@typename)!!
	///!!@label!!类型
	!!if !strcmp(@name,"RangeInt")!!
	typedef int !!@apiid!!!!@typename!!Type;
	!!elseif !strcmp(@name,"RangeChar")!!
	typedef char !!@apiid!!!!@typename!!Type;
	!!elseif !strcmp(@name,"String")!!
	typedef char !!@apiid!!!!@typename!!Type[!!show_value(atoi(@length)+1)!!];
	!!elseif !strcmp(@name,"VString")!!
	typedef char !!@apiid!!!!@typename!!Type[!!show_value(atoi(@length)+1)!!];
	!!elseif !strcmp(@name,"Array")!!
	typedef char !!@apiid!!!!@typename!!Type[!!show_value(atoi(@length)+1)!!];
	!!elseif !strcmp(@name,"Int")!!
	typedef int !!@apiid!!!!@typename!!Type;
	!!elseif !strcmp(@name,"Long")!!
	typedef double !!@apiid!!!!@typename!!Type;
	!!elseif !strcmp(@name,"Word")!!
	typedef short !!@apiid!!!!@typename!!Type;
	!!elseif !strcmp(@name,"Char")!!
	typedef char !!@apiid!!!!@typename!!Type;
	!!elseif !strcmp(@name,"EnumChar")!!
	typedef char !!@apiid!!!!@typename!!Type;
	!!travel self!!
	#define !!@apiid!!_!!@typename!!_!!@name!! '!!@value!!'	///!!@label!!
	!!next!!
	!!elseif !strcmp(@name,"Float")!!
	typedef double !!@apiid!!!!@typename!!Type;
	!!elseif !strcmp(@name,"FixNumber")!!
	typedef double !!@apiid!!!!@typename!!Type;
	!!elseif !strcmp(@name,"Pointer")!!
	typedef char *!!@apiid!!!!@typename!!Type;
	!!else!!
		!!error printf("unknown type %s in %s\n",@name,@typename);!!
	!!endif!!
	!!endif!!
	!!next!!
	!!leave!!
	!!leave!!

	!!enter FTD!!
	!!travel fields!!
	
	///!!@comment!!
	typedef struct !!@apiid!!!!@name!!Struct
	{
	!!travel self!!
		!!@apiid!!!!show_string(@type+1)!! !!@name!!;	///!!@label!!
	!!next!!
	} !!@apiid!!!!@name!!Struct;
	!!next!!
	!!leave!!  
}

#ifdef __cplusplus
}
#endif

#endif
