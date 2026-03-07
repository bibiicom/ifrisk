/////////////////////////////////////////////////////////////////////////
///CSequenceInterface.h
///定义了接口CSequenceInterface
/////////////////////////////////////////////////////////////////////////

#ifndef _CSEQUENCEINTERFACE_H
#define _CSEQUENCEINTERFACE_H

#include "platform.h"

//新的CSequence，能处理所有情况
class CSequence
{
public:
	//构造方法，创建此序列
	//每秒预计处理的数目，如果CurrentTime表清理了才需要重新计算差距
	//这里是一个破坏周期的累加值不可能大于一个一直每秒50000跑的系统
	CSequence(UF_INT8 nGapValue = 50000);

	///析构方法，将负责释放所占用的空间
	virtual ~CSequence();

	///获取此序列的下一个值
	///@return	下一个值
	virtual const char* getNextValueByString();
	virtual const UF_INT8 getNextValueByDouble();
	virtual const char* getNextValueByHex();

	//获取下一个，但是不递增
	virtual const char* touchNextValueByString(UF_INT8 nNext = 1.0);
	virtual const UF_INT8 touchNextValueByDouble(UF_INT8 nNext = 1.0);
	virtual const char* touchNextValueByHex(UF_INT8 nNext = 1.0);

	///获取此需要的当前值
	///@return	当前值
	virtual const char* getCurrentValueByString();
	virtual const double getCurrentValueByDouble();
	virtual const char* getCurrentValueByHex();
	///初始化该序列
	///@param nCurrValue 序列初始值
	virtual void initValueByTime(UF_INT8 nTime);
	virtual void initValueByValue(UF_INT8 nValue);
	virtual void initValueByValue(const char* sValue);
	virtual void initValueByHex(const char* sValue);
	virtual void initValueByValue(int nValue);

private:
	UF_INT8 m_digits;			//尾数的位数
	UF_INT8 m_curValue;			//计数器
	char m_seqString[17];		//用于返回值,不能大于IndexType的值
	char m_seqTouchString[17];	//用于Touch返回值,不能大于IndexType的值
	char m_seqHex[14];			//用于返回值，是十六进制返回
	char m_seqTouchHex[14];		//用于返回值，是十六进制Touch返回
};


#endif
