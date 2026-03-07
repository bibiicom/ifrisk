#ifndef __FINISH_ORDER_HANDLER_H__
#define __FINISH_ORDER_HANDLER_H__
#include "FieldData.h"
#include "DbHelper.h"

class CFinishOrderHandler 
{
public:
	CFinishOrderHandler(CDbHelper* pDbHelper);
	~CFinishOrderHandler();
	CBaseDataContainer* Get(CQryFinishOrderField* pField);

private:
	CDbHelper* m_pDbHelper;
	bool m_bAsc;
};

#endif // __FINISH_ORDER_HANDLER_H__