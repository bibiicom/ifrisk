#include "AccountDetailHandler.h"

CAccountDetailHandler::CAccountDetailHandler(CDbHelper* pDbHelper): m_pDbHelper(pDbHelper)
{
	m_bAsc = true;
}

CAccountDetailHandler::~CAccountDetailHandler()
{
}

CBaseDataContainer* CAccountDetailHandler::Get(CQryAccountDetailField* pField, TSessionRef* pSessionRef)
{
	m_pDbHelper->PrepareSelect("t_D_AccountDetail");

	if (!pField->AccountID.isNull())
		m_pDbHelper->AppendSelectParam("AccountID", pField->AccountID);
	if (!pField->Currency.isNull())
		m_pDbHelper->AppendSelectParam("Currency", pField->Currency);
	if (!pField->SettlementGroup.isNull())
		m_pDbHelper->AppendSelectParam("SettlementGroup", pField->SettlementGroup);
	if (!pField->Source.isNull())
		m_pDbHelper->AppendSelectParam("Source", pField->Source);
	if (!pField->InstrumentID.isNull())
		m_pDbHelper->AppendSelectParam("InstrumentID", pField->InstrumentID);
	if (!pField->StartTime.isNull())
		m_pDbHelper->AppendSelectParam("CreateTime", pField->StartTime.getString().c_str(), ">=");
	if (!pField->EndTime.isNull())
		m_pDbHelper->AppendSelectParam("CreateTime", pField->EndTime.getString().c_str(), "<=");

	int nLimit = 100;
	if (!pField->Limit.isNull())
	{
		nLimit = pField->Limit.getInt();
		if (nLimit > 100)
			nLimit = 100;
	}

	char filter[256] = {};
	snprintf(filter, sizeof(filter), " ORDER BY AccountDetailID DESC LIMIT %d ", nLimit);
	return m_pDbHelper->ExecSelectCmd(filter);
}
