#include "public.h"
#include "MarketDataTimer.h"
#include "CopyField.h"

CDepthMarketDataTimer::CDepthMarketDataTimer(CMarketDataService *pMarketDataService) :CServiceTimer()
{
	m_pMarketDataService = pMarketDataService;
}

CDepthMarketDataTimer::~CDepthMarketDataTimer()
{
}

void CDepthMarketDataTimer::onTime(CCurrentTime *pServiceTime)
{
	//static int CountTime = 0;
	//CountTime++;
	//if (CountTime % 30 != 0)
	//	return;
	//CountTime = 0;
	CopyCurrentTimeEntity(&m_CurrentTimeField, pServiceTime);
	//m_pMarketDataService->PublishDepthMarketData(pServiceTime->CurrTime);
	m_pMarketDataService->PublishSnapMarketData(&m_CurrentTimeField);
	m_pMarketDataService->PublishMarketData(&m_CurrentTimeField);
	m_pMarketDataService->PublishDelayMarketData(&m_CurrentTimeField);
}