#include "FinishTriggerOrderHandler.h"

CFinishTriggerOrderHandler::CFinishTriggerOrderHandler(CDbHelper* pDbHelper) :m_pDbHelper(pDbHelper)
{
	m_bAsc = true;
}

CFinishTriggerOrderHandler::~CFinishTriggerOrderHandler()
{
}

CBaseDataContainer* CFinishTriggerOrderHandler::Get(CQryFinishTriggerOrderField* pField)
{
	m_pDbHelper->PrepareSelect("t_D_FinishTriggerOrder");

	m_pDbHelper->AppendSelectParam("MemberID", pField->MemberID);

	if (!pField->AccountID.isNull())
		m_pDbHelper->AppendSelectParam("AccountID", pField->AccountID);

	if (!pField->InstrumentID.isNull())
		m_pDbHelper->AppendSelectParam("InstrumentID", pField->InstrumentID);

	if (pField->ExchangeID.isNull())
		m_pDbHelper->AppendSelectParam("ExchangeID", pField->ExchangeID);

	//if (pField->TriggerStatus.isNull())
	//{
	//	if (strlen(pField->TriggerStatus) == 1)
	//		m_pDbHelper->AppendSelectParam("TriggerStatus", field.TriggerStatus);
	//	else
	//		m_pDbHelper->AppendSelectParam("TriggerStatus", field.TriggerStatus, "INSTR($Param,TriggerStatus) > 0");
	//}

	//if (field.TriggerOrderType[0] != 0)
	//{
	//	if (strlen(field.TriggerOrderType) == 1)
	//		m_pDbHelper->AppendSelectParam("TriggerOrderType", field.TriggerOrderType);
	//	else
	//		m_pDbHelper->AppendSelectParam("TriggerOrderType", field.TriggerOrderType, "INSTR($Param,TriggerOrderType) > 0");
	//}

	char filter[128] = {};

	if (!pField->Before.isNull())
		m_pDbHelper->AppendSelectParam("OrderID", pField->Before, ">");
	if (!pField->After.isNull())
		m_pDbHelper->AppendSelectParam("OrderID", pField->After, "<");
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

	const char* szSort = m_bAsc ? "ASC" : "DESC";
	m_bAsc = true;

	if (!pField->Before.isNull() || !pField->After.isNull())
		snprintf(filter, sizeof(filter), " ORDER BY OrderID %s LIMIT %d", szSort, nLimit);
	else
		snprintf(filter, sizeof(filter), " ORDER BY UpdateTime %s LIMIT %d", szSort, nLimit);

	return m_pDbHelper->ExecSelectCmd(filter);
}