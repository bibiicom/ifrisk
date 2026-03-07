/////////////////////////////////////////////////////////////////////////
///version.h
///一个程序的版本号
/////////////////////////////////////////////////////////////////////////

#ifndef VERSION_H
#define VERSION_H

#include "platform.h"

extern const char *version;

///根据命令行参数，显示版本号
///@param	argc	参数个数
///@param	argv	参数值
void showVersion(int argc,char *argv[]);
void checkVersion();

#endif
