/********************************************************
 * 文件名称：utils.cpp
 * 文件标识：公用函数集
 *********************************************************/
#include "utils.h"
#include "platform.h"
#ifdef WIN32
#include <windows.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#endif
#define _S_IFDIR        0040000         /* directory */
 /***字符串拷贝
 * @param			目标串.
 * @param			源串.
 * @param	size_t	目标尺寸.
 * @return	char *	目标串指针
 * 功能同strncpy, 但保证有NULL结尾符
 */
char *StrCopy(char *dest, const char *src, int size)
{
	if (size > 0)
	{
		int count = size - 1;
		strncpy(dest, src, count);
		dest[count] = 0;
	}
	return dest;
}

/***裁减字符串前后空格
* @param			字符串.
* @return	char	目标串指针
*/
char *TrimAll(char *sz)
{
	if (*sz != 0)
	{
		char *p1 = sz;
		while (*p1 == SPACE_MARK)
			//while ((unsigned)*p1 <= SPACE_MARK)
		{
			p1++;
		}
		if (*p1 == 0)
		{
			*sz = 0;
		}
		else
		{
			char *p2 = (sz + STRLEN(sz) - 1);
			while (*p2 == SPACE_MARK)
				//while ((unsigned)*p2 <= SPACE_MARK)
			{
				p2--;
			}
			int len = (int)(p2 - p1 + 1);
			if (p1 != sz)
			{
				memcpy(sz, p1, len);
			}
			sz[len] = 0;
		}
	}
	return sz;
}

#define WIN_DELIMITER '\\'
#define NIX_DELIMITER '/'
#ifdef WIN32
#define DELIMITER WIN_DELIMITER
#else
#define DELIMITER NIX_DELIMITER
#endif

/***更换文件名的后缀
* @param			需要更换的文件名.
* @param			新的后缀，参数要带点如".txt".
* @return	char *  目标串指针
* 如果文件名没有后缀，则直接加后缀
*/
char *ChangeFileExt(char *fname, const char *ext)
{
	char *p = 0;
	for (char *q = fname; *q; q++)
	{
		switch (*q)
		{
		case '.':
			p = q;
			break;
		case WIN_DELIMITER:
		case NIX_DELIMITER:
			p = 0;
		}
	}
	if (p) *p = 0;
	strcat(fname, ext);
	return fname;
}

/***取文件的路径
* @param			目的串.
* @param			源串.
* @return	char *  目标串指针
* 取出的目标串以'\'(win)或'/'(unix)结尾
* 如果源串不包含路径信息，则结果为空
*/
char *ExtractFilePath(char *path, const char *fname)
{
	const char *p = 0;
	for (const char *q = fname; *q; q++)
	{
		switch (*q)
		{
		case WIN_DELIMITER:
		case NIX_DELIMITER:
			p = q;
		}
	}
	if (p)
	{
		int len = (int)(p - fname + 1);
		memcpy(path, fname, len);
		path[len] = 0;
	}
	else
	{
		*path = 0;
	}
	return path;
}

/***取文件名
* @param			目的串.
* @param			源串.
* @return	char *  目标串指针
* 如果源串不包含文件名，则结果为空
*/
char *ExtractFileName(char *fname, const char *path)
{
	const char *p = 0;
	for (const char *q = path; *q; q++)
	{
		switch (*q)
		{
		case WIN_DELIMITER:
		case NIX_DELIMITER:
			p = q;
		}
	}
	if (p)
	{
		strcpy(fname, p + 1);
	}
	else
	{
		strcpy(fname, path);
	}
	return fname;
}

/***增加路径串的尾分割符
* @param			路径串.
* @return	char *  路径串指针
* 如果原串已经包含尾分割符，则无变化
*/
char *IncludeDelimiter(char *fname)
{
	int len = (int)strlen(fname);
	if (len)
	{
		switch (fname[len - 1])
		{
		case WIN_DELIMITER:
		case NIX_DELIMITER:
			return fname;
		}
	}
	fname[len] = DELIMITER;
	fname[len + 1] = 0;
	return fname;
}

/***去除路径串的尾分割符
* @param			路径串.
* @return	char *  路径串指针
* 如果原串不包含尾分割符，则无变化
*/
char *ExcludeDelimiter(char *fname)
{
	int len = (int)strlen(fname);
	if (len)
	{
		switch (fname[len - 1])
		{
		case WIN_DELIMITER:
		case NIX_DELIMITER:
			fname[len - 1] = 0;
		}
	}
	return fname;
}

/**判断文件是否存在
* @param			文件名.
* @return	bool	true存在；false不存在
*/
bool FileExists(const char *fname)
{
	bool ret = false;
	FILE *hnd;
	if (hnd = fopen(fname, "r"))
	{
		fclose(hnd);
		ret = true;
	}
	return ret;
}

/**判断路径是否存在
* @param			路径名.
* @return	bool	true存在；false不存在
* 源串不以分割符结尾
*/
bool PathExists(const char *path)
{
	bool ret = false;
#ifdef WIN32
	int code = ::GetFileAttributes(path);
	if (code != -1)
	{
		ret = ((FILE_ATTRIBUTE_DIRECTORY&code) != 0);
	}
#else
	struct stat st;
	if (stat(path, &st) == 0)
	{
		ret = ((st.st_mode&_S_IFDIR) != 0);
	}
#endif
	return ret;
}

/**生成路径
* @param			路径名.
* @return	bool	true成功；false失败
* Windows系统必须全路径名
*/
bool ForcePath(const char *path)
{
	if (*path == 0)
	{
		return false;
	}

	bool ret = true;
	char dir[1024];
	strcpy(dir, path);
	ExcludeDelimiter(dir);
#ifdef WIN32
	if (strlen(dir) >= 3 && !PathExists(dir))
	{
		char sz[1024];
		ExtractFilePath(sz, dir);
		if (strcmp(sz, dir) != 0)
		{
			ret = ForcePath(sz) && ::CreateDirectory(dir, NULL);
		}
	}
#else
	if (dir[0] != 0 && !PathExists(dir))
	{
		char sz[1024];
		ExtractFilePath(sz, dir);
		if (ForcePath(sz))
		{
			sprintf(sz, "mkdir %s", dir);
			::system(sz);
			ret = PathExists(dir);
		}
		else
		{
			ret = false;
		}
	}
#endif
	return ret;
}

//字符串加密、解密

static char MMEnum[64] =
{ '!','$','0','1','2','3','4','5','6','7','8','9','A','B','C','D',
'E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T',
'U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j',
'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

static char MMReve[128];

/**初始化密码
* 全局调用一次
*/
void MMInit()
{
	srand((unsigned)time(NULL));
	memset(MMReve, 0, sizeof(MMReve));
	for (int i = 0; i < 64; i++)
	{
		MMReve[MMEnum[i]] = i;
	}
}

//取随机数
int Random(int r)
{
	int ret = 0;
	if (r != 0)
	{
		ret = rand() % r;
	}
	return ret;
}

//拆开字节
void One2(char *cc, char c)
{
	*cc = MMEnum[((Random(4) << 4) | (c >> 4) & 0x0f) & 0xff];
	cc++;
	*cc = MMEnum[((Random(4) << 4) | (c & 0x0f)) & 0xff];
}

//合并字节
char Two1(char c1, char c2)
{
	return ((MMReve[c1] << 4) | (MMReve[c2] & 0x0f)) & 0xff;
}

/**加密一个字符串
* 原串长度不超过255字节
* 目标长度不小于512字节
*/
char *EncodeStr(char *dest, const char *src)
{
	int len = (int)strlen(src);
	if (len > 255)
	{
		len = 255;
	}
	One2(dest, len);
	dest += 2;
	int i;
	for (i = 0; i < len; i++, dest += 2)
	{
		One2(dest, src[i]);
	}
	for (int j = (i + 1) * 2; j < 30; j++, dest++)
	{
		*dest = MMEnum[Random(64)];
	}
	*dest = 0;
	return dest;
}

/**解密一个字符串
* 目标长度不小于256字节
*/
char *DecodeStr(char *dest, const char *src)
{
	if (strlen(src) >= 4)
	{
		int len = Two1(src[0], src[1]);
		for (int i = 0; i < len; i++, dest++)
		{
			*dest = Two1(src[i * 2 + 2], src[i * 2 + 3]);
		}
	}
	*dest = 0;
	return dest;
}

//************************************
// Method: strrpl
// FullName: strrpl
// Access: public 
// Returns: void
// Qualifier: 字符串替换函数,能替换所有的要替换的字符串,被替换的字符串和替换的字符串不一定一样长.
// Parameter: char * pDstOut,输出字符串,要保证足够的空间可以存储替换后的字符串.
// Parameter: char * pSrcIn,输入字符串.
// Parameter: char * pSrcRpl,被替换的字符串.
// Parameter: char * pDstRpl,替换后的字符串.
// 注意:以上的字符串均要以 '\0 '结尾.
//************************************
void strrpl(char* pDstOut, const char* pSrcIn, const char* pSrcRpl, const char* pDstRpl)
{
	char sTemp[256];
	strncpy(sTemp, pSrcIn, sizeof(sTemp));

	char* pi = sTemp;
	char* po = pDstOut;
	int nSrcRplLen = (int)strlen(pSrcRpl);
	int nDstRplLen = (int)strlen(pDstRpl);
	char *p = NULL;
	int nLen = 0;
	do
	{
		// 找到下一个替换点
		p = strstr(pi, pSrcRpl);
		if (p != NULL)
		{
			// 拷贝上一个替换点和下一个替换点中间的字符串
			nLen = (int)(p - pi);
			memcpy(po, pi, nLen);

			// 拷贝需要替换的字符串
			memcpy(po + nLen, pDstRpl, nDstRplLen);
		}
		else
		{
			strcpy(po, pi);
			//如果没有需要拷贝的字符串,说明循环应该结束
			break;
		}

		pi = p + nSrcRplLen;
		po = po + nLen + nDstRplLen;

	} while (p != NULL);
}
