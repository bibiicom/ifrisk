#ifndef __SETTLE_DETAIL_HANDLER_H__
#define __SETTLE_DETAIL_HANDLER_H__
#include "FieldData.h"
#include "DbHelper.h"

class CSettleDetailHandler
{
public:
	CSettleDetailHandler(CDbHelper* pDbHelper);
	~CSettleDetailHandler();
	CBaseDataContainer* Get(CQrySettleDetailField* pField);

private:
	CDbHelper* m_pDbHelper;
};

#endif // __SETTLE_DETAIL_HANDLER_H__