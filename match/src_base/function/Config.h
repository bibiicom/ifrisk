#ifndef _CONFIG_H__
#define _CONFIG_H__

#include "platform.h"

#if _MSC_VER > 1000
#pragma once
#endif

void GetConfig(const char *filename, char *key, char *val, int size);
int GetConfigInt(const char *filename, char *key);
double GetConfigDouble(const char *filename, char *key);
string GetConfigString(const char *filename, char *key);

#endif
