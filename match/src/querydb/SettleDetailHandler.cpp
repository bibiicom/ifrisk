#include "SettleDetailHandler.h"

CSettleDetailHandler::CSettleDetailHandler(CDbHelper* pDbHelper):m_pDbHelper(pDbHelper)
{
}

CSettleDetailHandler::~CSettleDetailHandler()
{
}

CBaseDataContainer* CSettleDetailHandler::Get(CQrySettleDetailField* pField)
{
	m_pDbHelper->PrepareSelect("t_D_SettleDetail");
	
	m_pDbHelper->AppendSelectParam("ProductGroup", pField->ProductGroup.getValue());
	m_pDbHelper->AppendSelectParam("InstrumentID", pField->InstrumentID.getValue());

	if (pField->SettleAction.isNull())
		m_pDbHelper->AppendSelectParam("SettleAction", pField->SettleAction.getString().c_str());

	return m_pDbHelper->ExecSelectCmd(" AND SettleDetailID != '0' " );
}
