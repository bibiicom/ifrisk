#ifndef CSERVICETIMER_H
#define CSERVICETIMER_H

#include "databaseData.h"

/////////////////////////////////////////////////////////////////////////
///CServiceTimer是一个提供系统服务的定时器，根据时钟同步的情况，发出恰当
///的准点的时钟信号，供业务系统使用
/////////////////////////////////////////////////////////////////////////
class CServiceTimer: public CCurrentTimeCommitTrigger
{
public:
	///构造函数
	CServiceTimer();
	
	///确认更新后触发
	///@param	pCurrentTime	被刷新的CCurrentTime
	///@param	poldCurrentTime	原来的值
	virtual void commitUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pOldCurrentTime);
	
	///到达服务时间触发的事件
	///@param	pServiceTime	到达的服务时间
	virtual void onTime(CCurrentTime *pServiceTime)=0;
};

#endif
