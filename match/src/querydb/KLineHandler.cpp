#include "KLineHandler.h"
#include <sstream>
#include "monitorIndex.h"
#include <vector>
#include <sstream>
using namespace std;

#define EMPTY_RESP "[]"
#define MONTH_MINUTES	43200

// https://baike.baidu.com/item/基姆拉尔森计算公式
// 基姆拉尔森计算公式 W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400+1) mod 7
// 在公式中d表示日期中的日数，m表示月份数，y表示年数
// 注意：在公式中有个与其他公式不同的地方：
// 把一月和二月看成是上一年的十三月和十四月，例：如果是2004 - 1 - 10则换算成：2003 - 13 - 10来代入公式计算。
// return 0 周日 1 周一

int GetWeekDay(int y, int m, int d)
{
	if (m == 1 || m == 2) //把一月和二月换算成上一年的十三月和是四月
	{
		m += 12;
		y--;
	}
	return (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400 + 1) % 7;
}

bool FormatDatetime(char* buffer, size_t bufferSize, double t, const char* format)
{
	struct tm* startdatetime;
	time_t tx = floor(t / 1000);
	startdatetime = localtime(&tx);
	strftime(buffer, bufferSize, format, startdatetime);
	return true;
}

CKLineHandler::CKLineHandler(CDbHelper* pDbHelper)
{
	// 初始化基本类型
	m_klineTypes["1m"] = { "1m", "1m", 1, 1, false, 0, 0 };
	m_klineTypes["3m"] = { "3m", "1m", 3, 3, false, 0, 0 };
	m_klineTypes["5m"] = { "5m", "5m", 1, 5, false, 0, 0 };
	m_klineTypes["15m"] = { "15m", "5m", 3, 15, false, 0, 0 };
	m_klineTypes["30m"] = { "30m", "5m", 6, 30, false, 0, 0 };
	m_klineTypes["1h"] = { "1h", "1h", 1, 60, false, 0, 0 };
	m_klineTypes["2h"] = { "2h", "1h", 2, 2 * 60, false, 0, 0 };
	m_klineTypes["4h"] = { "4h", "1h", 4, 4 * 60, false, 0, 0 };
	m_klineTypes["6h"] = { "6h", "1h", 6, 6 * 60, false, 0, 0 };
	m_klineTypes["12h"] = { "12h", "1h", 12, 12 * 60, false, 0, 0 };
	m_klineTypes["1d"] = { "1d", "1d", 1, 24 * 60, false, 0, 0 };
	m_klineTypes["1w"] = { "1w", "1w", 1, 7 * 24 * 60, false, 0, 0 };
	m_klineTypes["1o"] = { "1o", "1o", 1, MONTH_MINUTES, false, 0, 0 };

	//m_nMaxCacheCount = GetConfigInt(INI_FILE_NAME, "MaxCacheCount");
	if (m_nMaxCacheCount <= 0)
		m_nMaxCacheCount = 10000;
	m_pDbHelper = pDbHelper;
}

CKLineHandler::~CKLineHandler()
{
}

void CKLineHandler::CheckKLineData(KLineData* pKLineData)
{
	int cnt = pKLineData->size() - m_nMaxCacheCount;
	while (--cnt >= 0)
	{
		pKLineData->pop_back();
	}
}

KLineData* CKLineHandler::GetKLine(CQryKLineField* pField)
{
	if (0 == pField->Bar.getValue()[0])
		return NULL;

	tagKLineType kLineType;
	map<string, tagKLineType>::iterator it = m_klineTypes.lower_bound(pField->Bar.getValue());
	if (it == m_klineTypes.end() || it->first != pField->Bar.getValue())
	{
		// 不在基本类型中
		memset(&kLineType, 0, sizeof(kLineType));
		kLineType.isCustomType = true;
		kLineType.customType = pField->Bar.getValue()[pField->Bar.getSize() - 1];
		kLineType.customCount = atoi(pField->Bar.getValue());
		kLineType.multiple = kLineType.customCount;
		kLineType.queryBar = pField->Bar.getValue();
		if (0 == kLineType.customCount)
			return NULL;
		switch (kLineType.customType)
		{
		default:
			return NULL;
		case 'm':
			kLineType.baseBar = "1m";
			kLineType.minutes = kLineType.customCount;
			break;
		case 'h':
			kLineType.baseBar = "1h";
			kLineType.minutes = kLineType.customCount * 60;
			break;
		case 'd':
			kLineType.baseBar = "1d";
			kLineType.minutes = kLineType.customCount * 24 * 60;
			break;
		}
		it = m_klineTypes.insert(it, pair<string, tagKLineType>(pField->Bar.getValue(), kLineType));
	}
	else
	{
		kLineType = it->second;
	}
	return GetKLine(pField, &kLineType);
}

KLineData* CKLineHandler::GetKLine(CQryKLineField* pField, tagKLineType* pKLineType)
{
	KLineData* pQueryKLineData = GetKlineData(pField->InstrumentID, pKLineType->queryBar);
	if (NULL == pQueryKLineData)
		return NULL;

	KLineData* pKLineData = pQueryKLineData;
	bool bComposite = (0 != strcmp(pKLineType->queryBar, pKLineType->baseBar));

	if (bComposite)
		pKLineData = GetKlineData(pField->InstrumentID, pKLineType->baseBar);

	if (NULL == pKLineData)
		return NULL; // 这里不可能为空

	if (!IsLocalSuffice(pField, pKLineType, pKLineData))
	{
		if (!GetKLineFromDB(pField, pKLineType, pKLineData))
			return NULL;
	}

	if (bComposite)
	{ // 合成K线
		if (!CompositeKLine(pKLineType, pKLineData, pQueryKLineData))
			return NULL;

		CheckKLineData(pKLineData);
		if (!pKLineType->isCustomType)
		{
			CheckKLineData(pQueryKLineData);
		}
		else
		{ // 自定义类型不需要保存
			pQueryKLineData->clear();
		}
		return pQueryKLineData;
	}
	else
	{
		CheckKLineData(pKLineData);
		return pKLineData;
	}
}

bool CKLineHandler::CompositeKLine(tagKLineType* pKLineType, const KLineData* source, KLineData* target)
{
	if (source->empty())
		return true;

	if (target->empty())
	{
		return (pKLineType->isCustomType ? CompositeCustomKLine(pKLineType, source, target)
			: CompositeBaseKLine(pKLineType, source, target));
	}

	list<CKLineBaseField> klineInfos;
	if ((pKLineType->isCustomType ? CompositeCustomKLine(pKLineType, source, &klineInfos)
		: CompositeBaseKLine(pKLineType, source, &klineInfos)))
	{
		list<CKLineBaseField>::iterator it = klineInfos.begin();
		for (; it != klineInfos.end(); it++)
		{
			if (it->InsertTime == target->back().InsertTime)
				break;
		}
		if (it != klineInfos.end())
		{
			target->pop_back();
			target->insert(target->end(), it, klineInfos.end());
		}
	}
	return true;
}

bool CKLineHandler::CompositeBaseKLine(tagKLineType* pKLineType, const list<CKLineBaseField>* source, list<CKLineBaseField>* target)
{
	CKLineBaseField* pKlineInfo = NULL;
	char buffer[64];
	for (list<CKLineBaseField>::const_iterator it = source->begin(); it != source->end(); it++)
	{
		int year, month, day, hour, minute;
		FormatDatetime(buffer, sizeof(buffer), it->InsertTime.getValue(), "%Y:%m:%d:%H:%M");
		if (5 != sscanf(buffer, "%d:%d:%d:%d:%d", &year, &month, &day, &hour, &minute))
			return false;

		if (pKLineType->minutes < 60)
		{
			if (60 % pKLineType->minutes != 0)
				return false;

			if (0 == minute % pKLineType->minutes)
			{
				pKlineInfo = AddKlineInfo(pKLineType, target, *it);
				continue;
			}
		}
		else if (pKLineType->minutes < 24 * 60)
		{
			if (24 % (pKLineType->minutes / 60) != 0)
				return false;

			if (0 == hour % (pKLineType->minutes / 60))
			{
				// 小时线是通过一小时合成 不需要关心分钟
				pKlineInfo = AddKlineInfo(pKLineType, target, *it);
				continue;
			}
		}
		else if (pKLineType->minutes == 7 * 24 * 60)
		{ //  周线
			int week = GetWeekDay(year, month, day);
			if (0 == week)
			{
				pKlineInfo = AddKlineInfo(pKLineType, target, *it);
				continue;
			}
		}
		if (NULL == pKlineInfo)
		{
			if (target->empty())
			{ // 第一条数据
				if (pKLineType->minutes < 60)
				{
					pKlineInfo = AddKlineInfo(pKLineType, target, *it);
					int gap = minute % pKLineType->minutes;
					pKlineInfo->InsertTime -= gap * 60 * 1000;
				}
				else if (pKLineType->minutes < 24 * 60)
				{
					pKlineInfo = AddKlineInfo(pKLineType, target, *it);
					int gap = hour % (pKLineType->minutes / 60);
					pKlineInfo->InsertTime -= gap * 60 * 60 * 1000;
				}
			}
			continue;
		}
		BuildKLine(&(*it), pKlineInfo);
	}
	return true;
}

bool CKLineHandler::CompositeCustomKLine(tagKLineType* pKLineType, const list<CKLineBaseField>* source, list<CKLineBaseField>* target)
{
	CKLineBaseField* pKlineInfo = NULL;
	char buffer[64];
	int timeZone = -1;
	for (list<CKLineBaseField>::const_iterator it = source->begin(); it != source->end(); it++)
	{
		int seconds = pKLineType->minutes * 60;
		time_t t = it->InsertTime / 1000;
		bool bAddKline = false;
		if (0 == strcmp(pKLineType->baseBar, "1d"))
		{
			if (-1 == timeZone)
			{
				timeZone = t % (24 * 60 * 60);
				if (timeZone > 12 * 60 * 60)
					timeZone = 24 * 60 * 60 - timeZone;
				else
					timeZone *= -1;
			}

			t += timeZone;

			int days = t / (24 * 60 * 60);
			int dayType = pKLineType->minutes / (24 * 60);

			if (0 == days % dayType)
				bAddKline = true;
		}
		else
		{
			if (0 == t % seconds)
				bAddKline = true;
		}

		if (bAddKline)
		{
			pKlineInfo = AddKlineInfo(pKLineType, target, *it);
			continue;
		}

		if (NULL == pKlineInfo)
		{
			if (target->empty())
			{ // 第一条数据
				pKlineInfo = AddKlineInfo(pKLineType, target, *it);
				int gap = t % seconds;
				pKlineInfo->InsertTime -= gap * 1000;
			}
			continue;
		}
		BuildKLine(&(*it), pKlineInfo);
	}
	return true;
}

void CKLineHandler::BuildKLine(const CKLineBaseField* pSource, CKLineBaseField* pTarget)
{
	pTarget->ClosePrice = pSource->ClosePrice;
	if (pTarget->HighestPrice < pSource->HighestPrice)
		pTarget->HighestPrice = pSource->HighestPrice;
	if (pTarget->LowestPrice > pSource->LowestPrice)
		pTarget->LowestPrice = pSource->LowestPrice;

	pTarget->Volume += pSource->Volume;
	pTarget->Turnover += pSource->Turnover;
}

KLineData* CKLineHandler::GetKlineData(const char* inst, const char* bar)
{
	KLineData kLineData;
	string strInst = inst;
	string strBar = bar;

	InstTypeKLineIter it = m_kLines.lower_bound(strInst);
	if (it == m_kLines.end() || it->first != strInst)
	{ // 合约不存在
		TypeKLineMap typeKLines;
		typeKLines[strBar] = kLineData;
		it = m_kLines.insert(it, InstTypeKLinePair(strInst, typeKLines));
	}
	else
	{
		TypeKLineIter typeKLineIter = it->second.lower_bound(strBar);
		if (typeKLineIter == it->second.end() || typeKLineIter->first != strBar)
			it->second.insert(typeKLineIter, TypeKLinePair(strBar, kLineData));
	}

	return &((it->second)[strBar]);
}

bool CKLineHandler::GetKLineFromDB(CQryKLineField* pField, tagKLineType* pKLineType, KLineData* pKLineData)
{
	char szFilter[256] = {};
	if (pKLineData->empty())
	{
		snprintf(szFilter, sizeof(szFilter)
			, "ExchangeID = '%s' AND InstrumentID = '%s' AND Bar='%s' ORDER BY InsertTime Desc Limit 10000"
			, pField->ExchangeID.getValue(), pField->InstrumentID.getValue(), pKLineType->baseBar);
	}
	else
	{
		snprintf(szFilter, sizeof(szFilter)
			, "ExchangeID = '%s' AND InstrumentID = '%s' AND InsertTime <= %lf AND Bar='%s' ORDER BY InsertTime Desc Limit 10000"
			, pField->ExchangeID.getValue(), pField->InstrumentID.getValue(), pKLineData->back().InsertTime.getValue(), pKLineType->baseBar);
	}

	CBaseDataContainer* pDataContainer = m_pDbHelper->Select("t_D_KLine", szFilter);
	if (NULL == pDataContainer)
	{
		REPORT_EVENT(LOG_CRITICAL, "GetKLine", "get kline from db error,filter:%s", szFilter);
		return false;
	}

	REPORT_EVENT(LOG_DEBUG, "GetKLine", "filter:%s,result:%d", szFilter, pDataContainer->size());

	UpdateKLines(pKLineType, pKLineData, pDataContainer);
	pDataContainer->clear();
	return true;
}

CKLineBaseField* CKLineHandler::AddKlineInfo(tagKLineType* pKLineType, list<CKLineBaseField>* target, const CKLineBaseField& klineInfo)
{
	CKLineBaseField* pKlineInfo = &(*(target->insert(target->end(), klineInfo)));
	//pKlineInfo->Bar = pKLineType->queryBar;
	return pKlineInfo;
}

void CKLineHandler::UpdateKLines(tagKLineType* pKLineType, KLineData* pKData, CBaseDataContainer* pDataContainer)
{
	if (NULL == pDataContainer || NULL == pKData)
		return;

	CKLineBaseField klineInfo;
	memset(&klineInfo, 0, sizeof(klineInfo));
	if (pKData->empty())
	{
		for (size_t i = 0; i < pDataContainer->size(); i++)
		{
			klineInfo.InsertTime = ((CKLineField*)pDataContainer->get(i))->InsertTime;
			klineInfo.OpenPrice = ((CKLineField*)pDataContainer->get(i))->OpenPrice;
			klineInfo.HighestPrice= ((CKLineField*)pDataContainer->get(i))->HighestPrice;
			klineInfo.LowestPrice = ((CKLineField*)pDataContainer->get(i))->LowestPrice;
			klineInfo.ClosePrice = ((CKLineField*)pDataContainer->get(i))->ClosePrice;
			klineInfo.Volume = ((CKLineField*)pDataContainer->get(i))->Volume;
			klineInfo.Turnover = ((CKLineField*)pDataContainer->get(i))->Turnover;
			pKData->push_back(klineInfo);
		}
	}
	else
	{
		for (size_t i = 0; i < pDataContainer->size(); i++)
		{
			if (((CKLineField*)pDataContainer->get(i))->InsertTime < pKData->back().InsertTime)
			{
				klineInfo.InsertTime = ((CKLineField*)pDataContainer->get(i))->InsertTime;
				klineInfo.OpenPrice = ((CKLineField*)pDataContainer->get(i))->OpenPrice;
				klineInfo.HighestPrice = ((CKLineField*)pDataContainer->get(i))->HighestPrice;
				klineInfo.LowestPrice = ((CKLineField*)pDataContainer->get(i))->LowestPrice;
				klineInfo.ClosePrice = ((CKLineField*)pDataContainer->get(i))->ClosePrice;
				klineInfo.Volume = ((CKLineField*)pDataContainer->get(i))->Volume;
				klineInfo.Turnover = ((CKLineField*)pDataContainer->get(i))->Turnover;
				pKData->push_back(klineInfo);
			}
		}
	}
}

bool CKLineHandler::GetDateYMD(const string& date, int& year, int& month, int& day)
{
	if (date.size() != 8)
		return false;
	year = atoi(date.substr(0, 4).c_str());
	month = atoi(date.substr(4, 2).c_str());
	day = atoi(date.substr(6, 2).c_str());

	return true;
}

//bool CKLineHandler::CalcDateTimeMinuteSum(const string& date, const string& time, int& minuteSum)
//{
//	tm _tm = {};
//	if (!GetDateYMD(date, _tm.tm_year, _tm.tm_mon, _tm.tm_mday))
//	{
//		REPORT_EVENT(LOG_INFO, "CalcDateTimeMinuteSum", "GetDateYMD error");
//		return false;
//	}
//	if (2 != sscanf(time.c_str(), "%d:%d", &_tm.tm_hour, &_tm.tm_min))
//	{
//		REPORT_EVENT(LOG_INFO, "CalcDateTimeMinuteSum", "fail to parse time:%s", time.c_str());
//		return false;
//	}
//	_tm.tm_year -= 1900;
//	_tm.tm_mon -= 1;
//	_tm.tm_sec = 0;
//
//	minuteSum = mktime(&_tm) / 60;
//
//	return true;
//}

bool CKLineHandler::IsLocalSuffice(CQryKLineField* pField, tagKLineType* pKLineType, KLineData* pKLineData)
{
	if (pKLineData->empty())
	{
		REPORT_EVENT(LOG_DEBUG, "GetKLineFromLocal", "can't get from local,the data is empty");
		return false;
	}
	if (pKLineData->front().InsertTime.getValue() < pField->EndTime)
	{
		pKLineData->clear();
		return false;
	}
	if (pKLineType->multiple * pField->Limit > pKLineData->size())
	{
		return false;
	}
	return true;
}