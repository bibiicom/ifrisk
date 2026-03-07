#ifndef __TRADE_HANDLER_H__
#define __TRADE_HANDLER_H__
#include "FieldData.h"
#include "DbHelper.h"

class CTradeHandler
{
public:
	CTradeHandler(CDbHelper* pDbHelper);
	~CTradeHandler();
	CBaseDataContainer* Get(CQryTradeField* pField, TSessionRef *pSessionRef);
private:
	CDbHelper* m_pDbHelper;
	bool m_bAsc;
};

#endif // __TRADE_HANDLER_H__