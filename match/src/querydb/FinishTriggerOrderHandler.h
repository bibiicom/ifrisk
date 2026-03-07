#ifndef __HISTORY_TRIGGER_ORDER_HANDLER_H__
#define __HISTORY_TRIGGER_ORDER_HANDLER_H__
#include "FieldData.h"
#include "DbHelper.h"

class CFinishTriggerOrderHandler
{
public:
	CFinishTriggerOrderHandler(CDbHelper* pDbHelper);
	~CFinishTriggerOrderHandler();
	CBaseDataContainer* Get(CQryFinishTriggerOrderField* pField);
private:
	CDbHelper* m_pDbHelper;
	bool m_bAsc;
};

#endif