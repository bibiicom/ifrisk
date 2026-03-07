#include "CServiceTimer.h"

CServiceTimer::CServiceTimer()
{
}
	
void CServiceTimer::commitUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pOldCurrentTime)
{
// 	int nNextClick = getMillisec(pOldCurrentTime->CurrTime,pOldCurrentTime->CurrMillisec)/m_everyMillisec+1;
// 	int nTargetClick = getMillisec(pCurrentTime->CurrTime,pCurrentTime->CurrMillisec)/m_everyMillisec;
// 	while (nNextClick <= nTargetClick)
// 	{
// 		CWriteableCurrentTime serviceTime;		
// 		serviceTime.CurrDate=pCurrentTime->CurrDate;
// 		getTime(&serviceTime.CurrTime,&serviceTime.CurrMillisec,nNextClick*m_everyMillisec);
// 		
// 		onTime(&serviceTime);
// 
// 		nNextClick++;
// 	}
	onTime(pCurrentTime);
}
