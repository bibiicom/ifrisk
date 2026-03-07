#ifndef _GW_UTILS_H
#define _GW_UTILS_H
/********************************************************
 * 文件名称：utils.h
 * 文件标识：公用函数集
 *********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//空格
#define SPACE_MARK ' '

/***字符串拷贝
* @param			目标串.
* @param			源串.
* @param	size_t	目标尺寸.
* @return	char *	目标串指针
* 功能同strncpy, 但保证有NULL结尾符
*/
char *StrCopy(char *, const char *, int);

/***裁减字符串前后空格
* @param			字符串.
* @return	char	目标串指针
*/
char *TrimAll(char *);

/***更换文件名的后缀
* @param			需要更换的文件名.
* @param			新的后缀，参数要带点如".txt".
* @return	char *  目标串指针
* 如果文件名没有后缀，则直接加上
*/
char *ChangeFileExt(char *, const char *);

/***取文件的路径
* @param			目的串.
* @param			源串.
* @return	char *  目标串指针
* 取出的目标串以'\'(win)或'/'(unix)结尾
* 如果源串不包含路径信息，则结果为空
*/
char *ExtractFilePath(char *, const char *);

/***取文件名
* @param			目的串.
* @param			源串.
* @return	char *  目标串指针
* 如果源串不包含文件名，则结果为空
*/
char *ExtractFileName(char *, const char *);

/***增加路径串的尾分割符
* @param			路径串.
* @return	char *  路径串指针
* 如果原串已经包含尾分割符，则无变化
*/
char *IncludeDelimiter(char *);

/***去除路径串的尾分割符
* @param			路径串.
* @return	char *  路径串指针
* 如果原串不包含尾分割符，则无变化
*/
char *ExcludeDelimiter(char *);

/**判断文件是否存在
* @param			文件名.
* @return	bool	true存在；false不存在
*/
bool FileExists(const char *);

/**判断路径是否存在
* @param			路径名.
* @return	bool	true存在；false不存在
* 源串不以分割符结尾
*/
bool PathExists(const char *);

/**生成路径
* @param			路径名.
* @return	bool	true成功；false失败
* Windows系统必须全路径名
*/
bool ForcePath(const char *);

/**初始化密码
* 全局调用一次
*/
void MMInit();

/**加密一个字符串
* 原串长度不超过255字节
* 目标长度不小于512字节
*/
char *EncodeStr(char *dest,const char *src);

/**解密一个字符串
* 目标长度不小于256字节
*/
char *DecodeStr(char *dest,const char *src);

void strrpl(char* pDstOut, const char* pSrcIn, const char* pSrcRpl, const char* pDstRpl);

#endif
