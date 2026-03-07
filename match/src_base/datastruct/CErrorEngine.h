/////////////////////////////////////////////////////////////////////////
///CErrorEngine.h
///定义了类CErrorEngine
/////////////////////////////////////////////////////////////////////////

#ifndef CERRORENGINE_H
#define CERRORENGINE_H

#include "platform.h"
//#include "CBaseObject.h"
//#include "DesignError.h"
#include "public.h"

#define MAX_ERROR_MSG 400

typedef struct
{
	int errorID;
	char *errorMsg;
}	TErrorType;

/////////////////////////////////////////////////////////////////////////
///CErrorEngine是一个错误引擎的类，提供了错误管理和报告的机制
///@author	tom
///@version	1.0,20041001
/////////////////////////////////////////////////////////////////////////
class CErrorEngine: public map<int, char *>
{
public:
	///构造方法，创建错误引擎
	CErrorEngine(void);
		
	///析构方法，将负责释放所占用的空间
	virtual ~CErrorEngine(void);
	
	///设置一个错误类型
	///@param	errorID	错误编号
	///@param	errorMsg	错误信息
	///@exception	如果该错误类型已经存在，则报出CDesignError
	void registerErrorType(int errorID, char *errorMsg);

	///设置一组错误类型
	///@param	pErrorType	错误类型数组，最后一个的errorID应当为0
	///@exception	如果该错误类型已经存在，则报出CDesignError
	void registerErrorType(TErrorType pErrorType[]);

	///报告一个错误
	///@param	errorID	要报告的错误编号
	bool reportError(const int errorID, const char *format = NULL, ...);
	
	///获取一个错误号对应的错误信息
	///@param	errorID	错误编号
	///@return	对应的错误信息
	char *getErrorMsg(const int errorID);

	///获取最近的错误，同时清除错误
	///@param	pLastErrorNo	返回最近的错误编号，0表示没有错误
	///@param	pLastErrorMsg	返回最近的错误信息
	virtual void getLastError(int *pLastErrorNo=NULL,char **pLastErrorMsg=NULL);

private:
	int m_nLastErrorNo;			//最近的错误编号
	char *m_pLastErrorMsg;		//最近的错误信息
	char m_sLastErrorMsg[128];	//长度收到接口的SubIndexName之后的所有字符串加起来的长度决定
};

#endif
