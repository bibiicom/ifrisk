/********************************************************
	读取配置文件类
	文件名: profile.h
**********************************************************/
#ifndef __PROFILE_H
#define __PROFILE_H

#include "platform.h"

char *mytrim(char *s);

class TIniFile
{
	//输入输出流
	FILE *fin, *fout;
	//返回指向section名称的字符串指针
	char *titlePos(char *buf, int *len);
	//是否是一个section字符串
	bool isTitleLine(char *bufPtr);
	//是否是指定的section
	bool containTitle(char *buf, const char *section);
	//移动文件指针到相应的section位置
	bool gotoSection(const char *section);
	//返回指向entry名称的字符串指针
	char *textPos(char *buf, const char *entry);
	//去除字符串前面的空格
	void stripQuotationChar(char *buf);
	//读取一个entry的值
	int readEntry(const char *entry, char *buf, int bufSize, bool strip);

public:
	// 函数名: TIniFile::ReadTString
	// 描述  : 新增的读取定义字段的字符串，且过滤左右空格和'\t'
	// 返回  : char * 
	// 参数  : const char *section
	// 参数  : const char *entry
	// 参数  : const char *defaultString
	// 参数  : char *buffer
	// 参数  : int bufLen
	char * ReadTString(const char *section, const char *entry, const char *defaultString, char *buffer, int bufLen);

	// 函数名: TIniFile::LRTrim
	// 描述  : 从str中将左右空格和'\t'截取掉
	// 返回  : char * 
	// 参数  : char *str
	static char * LRTrim(char *str);
	//配置文件名称
	char FileName[128];
	//打开配置文件
	bool Open(const char *filename);
	//读取一个int型entry的值
	int ReadInt(const char *section, const char *entry, int defaultInt);
	//读取一个string型的entry的值
	int ReadString(const char *section, const char *entry, const char *defaultString, char *buffer, int bufLen);
	//读取一个string型的entry的值
	void ReadStringList(const char *section, const char *entry, const char *defaultString, vector<string> *temp);
	//写一个string型的entry到指定的section中
	//写一个int型的entry到指定的section中
	//关闭配置文件
	void Close();

	TIniFile();
	~TIniFile();
private:
	char * FGetS(char *pbuf, int size, FILE *fp);
	void out_close();
	void in_close();
	char * percolate(char *str);
};
#endif
