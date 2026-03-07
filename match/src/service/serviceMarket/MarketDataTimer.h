#ifndef _MARKETDATATIMER_H__
#define _MARKETDATATIMER_H__

#if _MSC_VER > 1000
#pragma once
#endif //

#include "CServiceTimer.h"
#include "CConfig.h"
#include "MarketDataService.h"

class CDepthMarketDataTimer : public CServiceTimer
{
public:
	CDepthMarketDataTimer(CMarketDataService *pMarketDataService);
	virtual ~CDepthMarketDataTimer();

	///到达服务时间触发的事件
	///@param	pServiceTime	到达的服务时间
	virtual void onTime(CCurrentTime *pServiceTime);
private:
	CMarketDataService *m_pMarketDataService;
};

#endif