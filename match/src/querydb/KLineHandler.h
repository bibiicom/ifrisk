#ifndef __KLINE_HANDLER_H__
#define __KLINE_HANDLER_H__
#include "FieldData.h"
#include "DbHelper.h"
#include <map>
#include <list>

using namespace std;

struct tagKLineType
{
	const char* queryBar;
	const char* baseBar;
	int multiple;
	int minutes;
	bool isCustomType; // 是否为客户定义的任意类型
	char customType;
	int customCount;
};

typedef list<CKLineBaseField> KLineData;

class CKLineHandler
{
	// Instrument:Type:CKLineField
	typedef	map< string, map< string, KLineData > > InstTypeKLineMap;
	typedef	pair< string, map< string, KLineData > > InstTypeKLinePair;
	typedef	map< string, map< string, KLineData > >::iterator InstTypeKLineIter;

	typedef map< string, KLineData > TypeKLineMap;
	typedef pair< string, KLineData > TypeKLinePair;
	typedef map< string, KLineData >::iterator TypeKLineIter;
public:
	CKLineHandler(CDbHelper* pDbHelper);
	~CKLineHandler();
	void UpdateKLines(tagKLineType* pKLineType, KLineData* pKLineData, CBaseDataContainer* pDataContainer);
	bool IsLocalSuffice(CQryKLineField* pField, tagKLineType* pKLineType, KLineData* pKLineData);
	//bool GetKLineFromLocal(tagReqKLineField* pField, tagKLineType* pKLineType, vector< CKLineField >& kLines);
	KLineData* GetKLine(CQryKLineField* pField, tagKLineType* pKLineType);
	KLineData* GetKLine(CQryKLineField* pField);

	bool GetDateYMD(const string& date, int& year, int& month, int& day);
	//bool CalcDateTimeMinuteSum(const string& date, const string& time, int& minuteSum);

	bool CompositeKLine(tagKLineType* pKLineType, const KLineData* source, KLineData* target);

	bool CompositeBaseKLine(tagKLineType* pKLineType, const list< CKLineBaseField >* source, list< CKLineBaseField >* target);

	bool CompositeCustomKLine(tagKLineType* pKLineType, const list< CKLineBaseField >* source, list< CKLineBaseField >* target);

	TypeKLineIter FindTypeLine(TypeKLineMap& typeKLines, string type)
	{
		TypeKLineIter iterTypeKLine = typeKLines.find(type);
		return iterTypeKLine;
	}

	void BuildKLine(const CKLineBaseField* pSource, CKLineBaseField* pTarget);
	KLineData* GetKlineData(const char* szInst, const char* bar);
	bool GetKLineFromDB(CQryKLineField* pField, tagKLineType* pKLineType, KLineData* pKLineData);
	CKLineBaseField* AddKlineInfo(tagKLineType* pKLineType, list<CKLineBaseField>* target, const CKLineBaseField& klineInfo);
	void CheckKLineData(KLineData* pKLineData);

private:
	CDbHelper* m_pDbHelper;
	InstTypeKLineMap m_kLines;
	map<string, tagKLineType> m_klineTypes;
	int m_nMaxCacheCount;
};

#endif
