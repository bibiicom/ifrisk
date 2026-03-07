/////////////////////////////////////////////////////////////////////////
///CAlterService.h
///定义了CAlterService
/////////////////////////////////////////////////////////////////////////

#ifndef CAlterService_H
#define CAlterService_H

#include "CErrorEngine.h"
#include "tradingTools.h"
#include "ServiceInterface.h"

/////////////////////////////////////////////////////////////////////////
///CDbmtServiceBase是一个完成数据同步工作的类
///内存数据库的数据
///////////////////////////////////////////////////////////////////////// 
class CAlterService
{
public:
	///构造方法
	///@param	pMDB	指定的内存数据库
	///@param	pErrorEngine	指定的错误引擎
	CAlterService(CMDB *pMDB, CErrorEngine *pErrorEngine);

	///析构方法
	virtual ~CAlterService();
	
public:
!!enter system!!
!!travel tables!!
!!if !strcmp(@manager,"Init")!!
	///同步数据库表!!@comment!!的实现
	///@param	p!!@name!!	要同步的!!@comment!!信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
	virtual bool Sync!!@name!!Impl(int nTid, C!!@name!!Field *p!!@name!!, CTransaction *pTransaction);
!!endif!!	
!!next!!
!!leave!! 
	///数据同步后处理
	///@param	pTransaction	本处理属于的事务
	virtual void afterSync(CTransaction *pTransaction);
	
	///Link数据检查
	///@param	pFile	检查结果输出文件
	virtual void checkLink(FILE *pFile) const;
	
	///Null数据检查
	///@param	pFile	检查结果输出文件
	virtual void checkNull(FILE *pFile) const;
	
	///Valid数据检查
	///@param	pFile	检查结果输出文件
	virtual void checkValid(FILE *pFile) const;	
	
protected:
	CMDB *m_pMDB;
	CErrorEngine *m_pErrorEngine;
};
#endif
