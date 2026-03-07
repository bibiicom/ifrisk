#include "FinishOrderHandler.h"

CFinishOrderHandler::CFinishOrderHandler(CDbHelper* pDbHelper) :m_pDbHelper(pDbHelper)
{
}

CFinishOrderHandler::~CFinishOrderHandler()
{
}

CBaseDataContainer* CFinishOrderHandler::Get(CQryFinishOrderField* pField)
{
	m_pDbHelper->PrepareSelect("t_D_FinishOrder");
	m_pDbHelper->AppendSelectParam("MemberID", pField->MemberID.getValue());

	if (!pField->AccountID.isNull())
		m_pDbHelper->AppendSelectParam("AccountID", pField->AccountID);

	if (!pField->InstrumentID.isNull())
		m_pDbHelper->AppendSelectParam("InstrumentID", pField->InstrumentID);

	//if (!pField->OrderStatus.isNull())
	//{
	//	if (pField->OrderStatus.isNull())
	//		m_pDbHelper->AppendSelectParam("OrderStatus", pField->OrderStatus.getString().c_str());
	//	else
	//		m_pDbHelper->AppendSelectParam("OrderStatus", pField->OrderStatus.getString().c_str(), "INSTR($Param,OrderStatus) > 0");
	//}

	if (!pField->OrderID.isNull())
	{
		m_pDbHelper->AppendSelectParam("OrderID", pField->OrderID);
	}
	else
	{
		if (!pField->Before.isNull())
			m_pDbHelper->AppendSelectParam("OrderID", pField->Before, ">");

		if (!pField->After.isNull())
			m_pDbHelper->AppendSelectParam("OrderID", pField->After, "<");
	}

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

	char filter[128] = {};
	if (!pField->Before.isNull() || !pField->After.isNull())
		snprintf(filter, sizeof(filter), " ORDER BY OrderID DESC LIMIT %d ", nLimit);
	else
		snprintf(filter, sizeof(filter), " ORDER BY UpdateTime DESC LIMIT %d ", nLimit);

	return m_pDbHelper->ExecSelectCmd(filter);
}
