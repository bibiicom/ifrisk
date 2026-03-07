#include "CPositionChangeList.h"
#include "monitorIndex.h"
#include "tradingTools.h"

//void CPositionChangeList::add(CPosiDirectionType posiDirection, TPositionChangeType positionChangeType, CVolumeType volumeChange, CReadOnlyPriceType &price, bool isOpen, CBoolType isInverse)
void CPositionChangeList::add(CPosiDirectionType posiDirection, TPositionChangeType positionChangeType, CVolumeType volumeChange, bool isOpen)
{
	if (changeCount >= 4)
	{
		REPORT_EVENT(LOG_CRITICAL, "InternalError", "Too many client position changes");
		return;
	}
	changes[changeCount].posiDirection = posiDirection;
	changes[changeCount].positionChangeType = positionChangeType;
	changes[changeCount].volumeChange = volumeChange;
//	changes[changeCount].price = price;
//	if (isInverse)
//		changes[changeCount].clearPrice = price.getInverse();
//	else
//		changes[changeCount].clearPrice = price;
	changes[changeCount].isOpen = isOpen;
	changeCount++;
}

CPositionChange *CPositionChangeList::get(int id)
{
	if ((id < 0) || (id >= changeCount))
	{
		return NULL;
	}
	return &(changes[id]);
}

void CPositionChangeList::clear()
{
	changeCount = 0;
	isToday = true;
	m_LongOpenVolume = 0;
	m_ShortOpenVolume = 0;
	pOldPosition = NULL;
	pNewPosition = NULL;
	pOrderCancle = NULL;
	pOldOrder = NULL;
	pOrderAction = NULL;
	pOldOrder = NULL;
	pOrder = NULL;
	pTrade = NULL;
	pRate = NULL;
}

bool CPositionChangeList::isChanged(bool *pLongChanged, bool *pShortChanged)
{
	int i;
	bool longChanged = false, shortChanged = false;
	for (i = 0; i < changeCount; i++)
	{
		if (changes[i].posiDirection == PD_Long)
			longChanged = true;
		else
			shortChanged = true;
	}

	if (pLongChanged != NULL)
		*pLongChanged = longChanged;

	if (pShortChanged != NULL)
		*pShortChanged = shortChanged;
	return longChanged | shortChanged;
}

bool CPositionChangeList::isOpen(bool *pLongOpen, bool *pShortOpen)
{
	if (pLongOpen != NULL)
		*pLongOpen = m_LongOpenVolume;

	if (pShortOpen != NULL)
		*pShortOpen = m_ShortOpenVolume;

	if (m_LongOpenVolume > 0)
		return true;

	if (m_ShortOpenVolume > 0)
		return true;

	return false;
}

void CPositionChangeList::calculateOpenVolume(CVolumeType *pLongVolume, CVolumeType *pShortVolume)
{
	int i;
	*pLongVolume = 0;
	*pShortVolume = 0;
	for (i = 0; i < changeCount; i++)
	{
		if ((changes[i].volumeChange > 0) && (changes[i].positionChangeType == IS_TRADE))
		{
			if (changes[i].posiDirection == PD_Long)
				*pLongVolume += changes[i].volumeChange;
			else
				*pShortVolume += changes[i].volumeChange;
		}
	}
}

void CPositionChangeList::calculateCloseVolume(CVolumeType *pLongVolume, CVolumeType *pShortVolume)
{
	int i;
	*pLongVolume = 0;
	*pShortVolume = 0;
	for (i = 0; i < changeCount; i++)
	{
		if ((changes[i].volumeChange < 0) && (changes[i].positionChangeType == IS_TRADE))
		{
			if (changes[i].posiDirection == PD_Long)
				*pLongVolume -= changes[i].volumeChange;
			else
				*pShortVolume -= changes[i].volumeChange;
		}
	}
}

//bool CPositionChangeList::hasTrade(CPriceType *pTradePrice)
//{
//	int i;
//	for (i = 0; i < changeCount; i++)
//	{
//		if (changes[i].positionChangeType == IS_TRADE)
//		{
//			*pTradePrice = changes[i].price;
//			return true;
//		}
//	}
//	return false;
//}
