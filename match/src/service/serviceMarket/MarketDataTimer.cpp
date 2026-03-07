#include "public.h"
#include "MarketDataTimer.h"
#include "CConfig.h"

CDepthMarketDataTimer::CDepthMarketDataTimer(CMarketDataService *pMarketDataService):CServiceTimer()
{
	m_pMarketDataService = pMarketDataService;
}

CDepthMarketDataTimer::~CDepthMarketDataTimer()
{
}

void CDepthMarketDataTimer::onTime(CCurrentTime *pServiceTime)
{
	static int CountTime = 0;
	CountTime++;
	if (CountTime % 3 != 0)
		return;
	CountTime = 0;

	CTransaction transaction;
	m_pMarketDataService->PublishMarketData(pServiceTime->CurrTime, pServiceTime->CurrMilliSecond, &transaction);
	transaction.commit();
}