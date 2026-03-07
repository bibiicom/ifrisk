#include "TradeHandler.h"
#include "Config.h"


CTradeHandler::CTradeHandler(CDbHelper* pDbHelper) :m_pDbHelper(pDbHelper)
{
	m_bAsc = true;
}

CTradeHandler::~CTradeHandler()
{
}

CBaseDataContainer* CTradeHandler::Get(CQryTradeField* pField, TSessionRef* pSessionRef)
{
	m_pDbHelper->PrepareSelect("t_D_Trade");

	if (!pField->OrderID.isNull())
	{
		m_pDbHelper->AppendSelectParam("OrderID", pField->OrderID);
	}
	else
	{
		if (pField->MemberID.isNull())
			m_pDbHelper->AppendSelectParam("MemberID", pField->MemberID.getValue());

		if (!pField->AccountID.isNull())
			m_pDbHelper->AppendSelectParam("AccountID", pField->AccountID);

		if (!pField->InstrumentID.isNull())
			m_pDbHelper->AppendSelectParam("InstrumentID", pField->InstrumentID);

		if (!pField->ExchangeID.isNull())
			m_pDbHelper->AppendSelectParam("ExchangeID", pField->ExchangeID);

		if (!pField->Before.isNull())
			m_pDbHelper->AppendSelectParam("TradeID", pField->Before, ">");

		if (!pField->After.isNull())
			m_pDbHelper->AppendSelectParam("TradeID", pField->After, "<");
	}

	if (!pField->StartTime.isNull())
		m_pDbHelper->AppendSelectParam("CreateTime", pField->StartTime.getString().c_str(), ">=");

	if (!pField->EndTime.isNull())
		m_pDbHelper->AppendSelectParam("CreateTime", pField->EndTime.getString().c_str(), "<=");

	CBaseDataContainer* pDataContainer = NULL;
	int nLimit = 500;
	if (!pField->Limit.isNull())
	{
		nLimit = pField->Limit.getInt();
		if (nLimit > 500)
			nLimit = 500;
	}

	const char* szSort = m_bAsc ? "ASC" : "DESC";
	m_bAsc = true;

	char buff[128] = {};
	snprintf(buff, sizeof(buff) - 1, " ORDER BY TradeID %s LIMIT %d", szSort, nLimit);
	return m_pDbHelper->ExecSelectCmd(buff);
}
