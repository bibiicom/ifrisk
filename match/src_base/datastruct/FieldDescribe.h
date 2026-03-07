#ifndef _FIELDDESCRIBE_H__
#define _FIELDDESCRIBE_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "UFDataType.h"
#define MAX_MEMB				128
#define MAX_FIELD_NAME_LEN		32
#define MAX_FIELD_SIZE			4096 

//每种数据类型的标示符
enum MemberType
{
	FT_BYTE,
	FT_DWORD,
	FT_REAL8,
	FT_LONG
};

//类成员属性结构
struct TMemberDesc
{
	MemberType nType;					//成员类型
	size_t nStructOffset;				//成员在类中的偏移量
	size_t nStreamOffset;				//成员在字节流中的偏移量
	int nSize;							//成员长度
	char szName[26];					//成员名称

	char sApi[26];						//成员名称
	char sShort[26];					//成员短名称
	int nPrecision;						//精度
};
//#define DEF_SETUP_MEMBER(type,id) 	void SetupMember(type &,int offset,const char *name,int length){	SetupMember(id,offset,name,length); }

class  CFieldDescribe
{
public:
	typedef void(*describeFunc)(); /**< 定义一个指针函数类型 */

	///构造函数，并执行传进的指针函数
	///FieldID 域ID
	///nStructSize 域大小
	///pszFieldName 域名
	///pszComment 域说明
	///func    指向一个函数的指针
	CFieldDescribe(int FieldID, int nStructSize, const char *pszFieldName, const char *pszComment, describeFunc func);

	/**析构函数
	*/
	~CFieldDescribe();
public:
	void SetupMember(CUFBaseCharType&, size_t offset, const char *name, int length, const char *api, const char *shortname, int nPrecision)
	{
		SetupMember(FT_BYTE, offset, name, length, api, shortname, nPrecision);
	}
	void SetupMember(CUFBaseIntType&, size_t offset, const char *name, int length, const char *api, const char *shortname, int nPrecision)
	{
		SetupMember(FT_DWORD, offset, name, length, api, shortname, nPrecision);
	}
	void SetupMember(CUFBaseLongType&, size_t offset, const char *name, int length, const char *api, const char *shortname, int nPrecision)
	{
		SetupMember(FT_LONG, offset, name, length, api, shortname, nPrecision);
	}
	void SetupMember(CUFBaseFloatType&, size_t offset, const char *name, int length, const char *api, const char *shortname, int nPrecision)
	{
		SetupMember(FT_REAL8, offset, name, length, api, shortname, nPrecision);
	}
	void SetupMember(CUFBaseStringType&, size_t offset, const char *name, int length, const char *api, const char *shortname, int nPrecision)
	{
		SetupMember(FT_BYTE, offset, name, length, api, shortname, nPrecision);
	}

	///将对象转换为字节流
	/// @pStruct 要转换的对象
	/// @pStream 转换出的字节流
	/// 字节流中的成员变量是高位在前
	void StructToStream(char *pStream, const char *pStruct);

	///将字节流转换为对象
	///@param pStruct 转换的出对象
	///@param pStream 要转换的字节流
	///@remark 字节流中的成员变量必须是高位在前
	void StreamToStruct(char *pStruct, const char *pStream);

	bool StructToOutput(char* pOutput, const char *pStruct);
	bool StreamToOutput(char* pOutput, const char *pStream);
	bool StructToCSV(char* pCSV, const char *pStruct);
	bool StreamToCSV(char* pCSV, const char *pStream);
	bool StructToSingleCSV(char* pCSV, const char *pStruct);
	bool StreamToSingleCSV(char* pCSV, const char *pStream);

	bool StructToJson(char* pJson, const char *pStruct);
	bool StreamToJson(char* pJson, const char *pStream);
	bool StructToJsonArray(char* pJson, const char *pStruct);
	bool StructToShortJson(char* pJson, const char *pStruct);

	//全部是引号
	bool StructToJsonS(char* pJson, const char *pStruct);
	bool StructToShortJsonS(char* pJson, const char *pStruct);
	bool StructToJsonArrayS(char* pJson, const char *pStruct);

	//没有引号
	bool StructToJsonArrayC(char* pJson, const char *pStruct);
	bool StructToShortJsonC(char* pJson, const char *pStruct);

	///从网络流水获取到内部对象
	// @param pField 转换出的对象
	// @param pStream 要转换的字节流
	int getSingleField(void *pField, const void *pStream);

	//将内部对象转化为网络流水
	// @param pStream 转换出的字节流
	// @param pField 要转换的对象
	int setSingleField(void *pStream, const void *pField);

	inline int GetStructSize();

	//获取类转换成的字节流长度
	//return 返回类转换成的字节流长度	
	inline int GetStreamSize();

	///类成员个数
	///return 返回类成员个数
	inline int GetMemberCount();

	///获取域中每个成员的描述
	/// @param index 成员在描述对照表中的位置
	/// return 返回成员描述
	inline TMemberDesc *GetMemberDesc(int index);

	inline const char *GetFieldName();
private:

	///填充类型和类型Id对照表
	/// @param nType 成员类型
	/// @param nStructOffset 成员在类中的偏移量
	/// @param nSize 成员长度
	/// @param szName 成员名称
	inline void SetupMember(MemberType nType, size_t nStructOffset, const char *szName, int nSize, const char *api, const char *shortname, int nPrecision);
public:
	int m_FieldID;			/**<域ID */
	//char m_szComment[MAX_FIELD_NAME_LEN];		/**<域注释 */
	int m_nStructSize;			/**<域大小*/
	int m_nStreamSize;			/**<域转换成的字节流长度*/
private:
	int m_nTotalMember;			/**<域成员个数*/
	char m_szFieldName[MAX_FIELD_NAME_LEN];
	char m_szFieldTmp[MAX_FIELD_SIZE * 2];
	TMemberDesc m_MemberDesc[MAX_MEMB];	/**<域成员属性数组*/

};

inline void CFieldDescribe::SetupMember(MemberType nType, size_t nStructOffset, const char *szName, int nSize, const char *api, const char *shortname, int nPrecision)
{
	TMemberDesc *p = &m_MemberDesc[m_nTotalMember];
	p->nType = nType;
	p->nStructOffset = nStructOffset;
	p->nStreamOffset = m_nStreamSize;
	p->nSize = nSize;
	p->nPrecision = nPrecision;

	strncpy(p->szName, szName, sizeof(p->sApi) - 1);
	strncpy(p->sApi, api, sizeof(p->sApi) - 1);
	strncpy(p->sShort, shortname, sizeof(p->sShort) - 1);

	m_nStreamSize += nSize;
	m_nTotalMember++;
}

inline int CFieldDescribe::GetStructSize()
{
	return m_nStructSize;
}

inline int CFieldDescribe::GetStreamSize()
{
	return m_nStreamSize;
}

inline int CFieldDescribe::GetMemberCount()
{
	return m_nTotalMember;
}

inline TMemberDesc * CFieldDescribe::GetMemberDesc(int index)
{
	return &m_MemberDesc[index];
}

inline const char *CFieldDescribe::GetFieldName()
{
	return m_szFieldName;
}

#endif
