/////////////////////////////////////////////////////////////////////////
///定义需要用到的公共函数
/////////////////////////////////////////////////////////////////////////

#ifndef _BaseFunction_H_
#define _BaseFunction_H_

#include "platform.h"

void PrintGodBless();

char *base64_encode(const char* data, int data_len);
static int find_pos(char ch);
char *base64_decode(const char *data, int data_len);
string& Txttrim(string &s);

vector<string> Txtsplit(const string& str, string separate_character);
void Txtsplit(const string& str, string separate_character, vector<string>& vct);

map<string, string> mapSplit(const string& str, string s1, string s2);
void mapSplit(const string& str, string s1, string s2, map<string,string>& mapStr);

//Tom=Happy&Jerry=Sad : s1=& s2==
map<string, string> mapSplit(char* str, int len, char s1, char s2);

struct json_str {
	const char *p;
	int len;
};

map<string, json_str> jsonSplit(char* str, int len);

void splitstr(const char* content, const char* token, vector<std::string>& vect);

void run_command(const char *format,...);

char* fastStrcat(char *pszDest, const char* pszSrc, char cSplit = '\0');
char* fastStrcat(char *pszDest, int nSrc, char cSplit = '\0');
char* fastStrcat(char *pszDest, char nSrc, char cSplit = '\0');
char* fastStrcatLimit(char *pszDest, const char* pszSrc, int nLength = 1024);
char* getSplit(char *pszDest, char cSplit);

#ifdef WIN32
//此函数在windows情况下使用
////////////////////////////////////////////////////////////////////////////
// 函数名： GetMacByCmd(char *lpszMac)
// 参数：
//      输入： void
//      输出： lpszMac,返回的MAC地址串
// 返回值：
//      TRUE:  获得MAC地址。
//      FALSE: 获取MAC地址失败。
// 过程：
//      1. 创建一个无名管道。
//      2. 创建一个IPCONFIG 的进程，并将输出重定向到管道。
//      3. 从管道获取命令行返回的所有信息放入缓冲区lpszBuffer。
//      4. 从缓冲区lpszBuffer中获得抽取出MAC串。
//
//  提示：可以方便的由此程序获得IP地址等其他信息。
//        对于其他的可以通过其他命令方式得到的信息只需改变strFetCmd 和 
//        str4Search的内容即可。
///////////////////////////////////////////////////////////////////////////
bool GetMacByCmd(char *lpszMac);
#endif



#endif
