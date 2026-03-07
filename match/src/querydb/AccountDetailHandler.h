#ifndef __ACCOUNT_DETAIL_HANDLER_H__
#define __ACCOUNT_DETAIL_HANDLER_H__
#include "FieldData.h"
#include "DbHelper.h"

class CAccountDetailHandler
{
public:
	CAccountDetailHandler(CDbHelper* pDbHelper);
	~CAccountDetailHandler();
	CBaseDataContainer* Get(CQryAccountDetailField* pField, TSessionRef* pSessionRef);
private:
	CDbHelper* m_pDbHelper;
	bool m_bAsc;
};

#endif // __ACCOUNT_DETAIL_HANDLER_H__