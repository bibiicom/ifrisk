#include "DelayMDB.h"
#include "CopyField.h"
#include "monitorIndex.h"
#include "utility.h"

bool CDelayMDB::Dispatch(int nWorkerID, UF_INT8 nTaskNo, CFieldDescribe *pDescirbe, const void *pData, TSQLType nActionType)
{
	bool ret = false;
	if (m_bSinkInit && m_nDelaySeconds > 0)
	{
		switch (pDescirbe->m_FieldID)
		{
		case FID_Account:
		{
			string IndexID = ((CAccountField*)pData)->Currency.getString() + ((CAccountField*)pData)->SettlementGroup.getString()
				+ ((CAccountField*)pData)->AccountID.getString() + ((CAccountField*)pData)->MemberID.getString();
			switch (nActionType)
			{
			case TSL_INSERT:
			{
				CDelayAccountMap::iterator iter = m_InsertAccount.find(IndexID);
				if (iter == m_InsertAccount.end())
				{
					DelayAccount theStruct;
					memcpy(&(theStruct.Data), ((CAccountField*)pData), ((CAccountField*)pData)->m_Describe.m_nStructSize);
					theStruct.nTaskNo = nTaskNo;
					m_InsertAccount.insert(CDelayAccountMap::value_type(IndexID, theStruct));
				}
				else
				{
					DelayAccount* pStruct = &(iter->second);
					pStruct->nTaskNo = nTaskNo;
					memcpy(&(pStruct->Data), ((CAccountField*)pData), ((CAccountField*)pData)->m_Describe.m_nStructSize);
				}

				iter = m_UpdateAccount.find(IndexID);
				if (iter != m_UpdateAccount.end())
					m_UpdateAccount.erase(iter);

				iter = m_DeleteAccount.find(IndexID);
				if (iter != m_DeleteAccount.end())
					m_DeleteAccount.erase(iter);
				break;
			}
			case TSL_UPDATE:
			{
				CDelayAccountMap::iterator iter = m_InsertAccount.find(IndexID);
				if (iter == m_InsertAccount.end())
				{
					iter = m_UpdateAccount.find(IndexID);
					if (iter == m_UpdateAccount.end())
					{
						DelayAccount theStruct;
						memcpy(&(theStruct.Data), ((CAccountField*)pData), ((CAccountField*)pData)->m_Describe.m_nStructSize);
						theStruct.nTaskNo = nTaskNo;
						m_UpdateAccount.insert(CDelayAccountMap::value_type(IndexID, theStruct));
					}
					else
					{
						DelayAccount* pStruct = &(iter->second);
						pStruct->nTaskNo = nTaskNo;
						memcpy(&(pStruct->Data), ((CAccountField*)pData), ((CAccountField*)pData)->m_Describe.m_nStructSize);
					}
					iter = m_DeleteAccount.find(IndexID);
					if (iter != m_DeleteAccount.end())
						m_DeleteAccount.erase(iter);
				}
				else
				{
					DelayAccount* pStruct = &(iter->second);
					pStruct->nTaskNo = nTaskNo;
					memcpy(&(pStruct->Data), ((CAccountField*)pData), ((CAccountField*)pData)->m_Describe.m_nStructSize);
				}
				break;
			}
			case TSL_DELETE:
			{
				CDelayAccountMap::iterator iter = m_InsertAccount.find(IndexID);
				if (iter != m_InsertAccount.end())
				{
					m_InsertAccount.erase(iter);
				}
				else
				{
					iter = m_UpdateAccount.find(IndexID);
					if (iter != m_UpdateAccount.end())
						m_UpdateAccount.erase(iter);

					iter = m_DeleteAccount.find(IndexID);
					if (iter == m_DeleteAccount.end())
					{
						DelayAccount theStruct;
						memcpy(&(theStruct.Data), ((CAccountField*)pData), ((CAccountField*)pData)->m_Describe.m_nStructSize);
						theStruct.nTaskNo = nTaskNo;
						m_DeleteAccount.insert(CDelayAccountMap::value_type(IndexID, theStruct));
					}
				}
				break;
			}
			}
			ret = true;
			break;
		}
		case FID_Order:
		{
			ret = true;
			//遇到做市的订单，全部不处理
			if (((COrderField*)pData)->Priority == 0)
				break;				

			string IndexID = ((COrderField*)pData)->OrderID.getString();
			switch (nActionType)
			{
			case TSL_INSERT:
			{
				CDelayOrderMap::iterator iter = m_InsertOrder.find(IndexID);
				if (iter == m_InsertOrder.end())
				{
					DelayOrder theStruct;
					memcpy(&(theStruct.Data), ((COrderField*)pData), ((COrderField*)pData)->m_Describe.m_nStructSize);
					theStruct.nTaskNo = nTaskNo;
					m_InsertOrder.insert(CDelayOrderMap::value_type(IndexID, theStruct));
				}
				else
				{
					DelayOrder* pStruct = &(iter->second);
					pStruct->nTaskNo = nTaskNo;
					memcpy(&(pStruct->Data), ((COrderField*)pData), ((COrderField*)pData)->m_Describe.m_nStructSize);
				}

				iter = m_UpdateOrder.find(IndexID);
				if (iter != m_UpdateOrder.end())
					m_UpdateOrder.erase(iter);

				iter = m_DeleteOrder.find(IndexID);
				if (iter != m_DeleteOrder.end())
					m_DeleteOrder.erase(iter);
				break;
			}
			case TSL_UPDATE:
			{
				CDelayOrderMap::iterator iter = m_InsertOrder.find(IndexID);
				if (iter == m_InsertOrder.end())
				{
					iter = m_UpdateOrder.find(IndexID);
					if (iter == m_UpdateOrder.end())
					{
						DelayOrder theStruct;
						memcpy(&(theStruct.Data), ((COrderField*)pData), ((COrderField*)pData)->m_Describe.m_nStructSize);
						theStruct.nTaskNo = nTaskNo;
						m_UpdateOrder.insert(CDelayOrderMap::value_type(IndexID, theStruct));
					}
					else
					{
						DelayOrder* pStruct = &(iter->second);
						pStruct->nTaskNo = nTaskNo;
						memcpy(&(pStruct->Data), ((COrderField*)pData), ((COrderField*)pData)->m_Describe.m_nStructSize);
					}
					iter = m_DeleteOrder.find(IndexID);
					if (iter != m_DeleteOrder.end())
						m_DeleteOrder.erase(iter);
				}
				else
				{
					DelayOrder* pStruct = &(iter->second);
					pStruct->nTaskNo = nTaskNo;
					memcpy(&(pStruct->Data), ((COrderField*)pData), ((COrderField*)pData)->m_Describe.m_nStructSize);
				}
				break;
			}
			case TSL_DELETE:
			{
				CDelayOrderMap::iterator iter = m_InsertOrder.find(IndexID);
				if (iter != m_InsertOrder.end())
				{
					m_InsertOrder.erase(iter);
				}
				else
				{
					iter = m_UpdateOrder.find(IndexID);
					if (iter != m_UpdateOrder.end())
						m_UpdateOrder.erase(iter);

					iter = m_DeleteOrder.find(IndexID);
					if (iter == m_DeleteOrder.end())
					{
						DelayOrder theStruct;
						memcpy(&(theStruct.Data), ((COrderField*)pData), ((COrderField*)pData)->m_Describe.m_nStructSize);
						theStruct.nTaskNo = nTaskNo;
						m_DeleteOrder.insert(CDelayOrderMap::value_type(IndexID, theStruct));
					}
				}
				break;
			}
			}
			break;
		}
		case FID_Position:
		{
			string IndexID = ((CPositionField*)pData)->ExchangeID.getString();
			IndexID += ((CPositionField*)pData)->InstrumentID.getString();
			IndexID += ((CPositionField*)pData)->PosiDirection.getString();
			IndexID += ((CPositionField*)pData)->TradeUnitID.getString();
			IndexID += ((CPositionField*)pData)->MemberID.getString();

			switch (nActionType)
			{
			case TSL_INSERT:
			{
				CDelayPositionMap::iterator iter = m_InsertPosition.find(IndexID);
				if (iter == m_InsertPosition.end())
				{
					DelayPosition theStruct;
					memcpy(&(theStruct.Data), ((CPositionField*)pData), ((CPositionField*)pData)->m_Describe.m_nStructSize);
					theStruct.nTaskNo = nTaskNo;
					m_InsertPosition.insert(CDelayPositionMap::value_type(IndexID, theStruct));
				}
				else
				{
					DelayPosition* pStruct = &(iter->second);
					pStruct->nTaskNo = nTaskNo;
					memcpy(&(pStruct->Data), ((CPositionField*)pData), ((CPositionField*)pData)->m_Describe.m_nStructSize);
				}

				iter = m_UpdatePosition.find(IndexID);
				if (iter != m_UpdatePosition.end())
					m_UpdatePosition.erase(iter);

				iter = m_DeletePosition.find(IndexID);
				if (iter != m_DeletePosition.end())
					m_DeletePosition.erase(iter);
				break;
			}
			case TSL_UPDATE:
			{
				CDelayPositionMap::iterator iter = m_InsertPosition.find(IndexID);
				if (iter == m_InsertPosition.end())
				{
					iter = m_UpdatePosition.find(IndexID);
					if (iter == m_UpdatePosition.end())
					{
						DelayPosition theStruct;
						memcpy(&(theStruct.Data), ((CPositionField*)pData), ((CPositionField*)pData)->m_Describe.m_nStructSize);
						theStruct.nTaskNo = nTaskNo;
						m_UpdatePosition.insert(CDelayPositionMap::value_type(IndexID, theStruct));
					}
					else
					{
						DelayPosition* pStruct = &(iter->second);
						pStruct->nTaskNo = nTaskNo;
						memcpy(&(pStruct->Data), ((CPositionField*)pData), ((CPositionField*)pData)->m_Describe.m_nStructSize);
					}
					iter = m_DeletePosition.find(IndexID);
					if (iter != m_DeletePosition.end())
						m_DeletePosition.erase(iter);
				}
				else
				{
					DelayPosition* pStruct = &(iter->second);
					pStruct->nTaskNo = nTaskNo;
					memcpy(&(pStruct->Data), ((CPositionField*)pData), ((CPositionField*)pData)->m_Describe.m_nStructSize);
				}
				break;
			}
			case TSL_DELETE:
			{
				CDelayPositionMap::iterator iter = m_InsertPosition.find(IndexID);
				if (iter != m_InsertPosition.end())
				{
					m_InsertPosition.erase(iter);
				}
				else
				{
					iter = m_UpdatePosition.find(IndexID);
					if (iter != m_UpdatePosition.end())
						m_UpdatePosition.erase(iter);

					iter = m_DeletePosition.find(IndexID);
					if (iter == m_DeletePosition.end())
					{
						DelayPosition theStruct;
						memcpy(&(theStruct.Data), ((CPositionField*)pData), ((CPositionField*)pData)->m_Describe.m_nStructSize);
						theStruct.nTaskNo = nTaskNo;
						m_DeletePosition.insert(CDelayPositionMap::value_type(IndexID, theStruct));
					}
				}
				break;
			}
			}
			ret = true;
			break;
		}
		}
	}

	if (m_bSinkDown)
	{
		switch (pDescirbe->m_FieldID)
		{
		case FID_Order:
		{
			if (nActionType == TSL_DELETE)
			{
				if (((CFinishOrderField*)pData)->VolumeTraded == 0.0 && ((CFinishOrderField*)pData)->DeriveSource == DS_Normal)
				{
					if (((COrderField*)pData)->Priority == 0)
						return true;

					CFinishOrderMap::iterator iter = m_FinishOrder.find(((CFinishOrderField*)pData)->MemberID.getString());
					if (iter == m_FinishOrder.end())
					{
						m_FinishOrder.insert(CFinishOrderMap::value_type(((CFinishOrderField*)pData)->MemberID.getString(), 1));
					}
					else
					{
						iter->second++;
						if (iter->second > m_nFinishOrderCount)
							return true;
					}
				}
				time_t tm = ((CFinishOrderField*)pData)->UpdateTime / 1000;
				struct tm timeinfo = GetTM(&tm);
				strftime(((CFinishOrderField*)pData)->CreateTime.getBuffer(), 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
				m_pSinkDBDispatcher->DelayMDBCallback(nWorkerID + 75, nTaskNo, &(CFinishOrderField::m_Describe), pData, TSL_INSERT);
			}
			break;
		}
		case FID_Position:
		{
			if (nActionType == TSL_DELETE)
			{
				//todo
			}
			break;
		}
		case FID_TriggerOrder:
		{
			if (nActionType == TSL_DELETE)
			{
				if (((CFinishTriggerOrderField*)pData)->TriggerStatus == TS_Canceled)
				{
					CFinishOrderMap::iterator iter = m_FinishOrder.find(((CFinishTriggerOrderField*)pData)->MemberID.getString());
					if (iter == m_FinishOrder.end())
					{
						m_FinishOrder.insert(CFinishOrderMap::value_type(((CFinishTriggerOrderField*)pData)->MemberID.getString(), 1));
					}
					else
					{
						iter->second++;
						if (iter->second > m_nFinishOrderCount)
							return true;
					}
				}
				time_t tm = ((CFinishTriggerOrderField*)pData)->UpdateTime / 1000;
				struct tm timeinfo = GetTM(&tm);
				strftime(((CFinishTriggerOrderField*)pData)->CreateTime.getBuffer(), 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
				m_pSinkDBDispatcher->DelayMDBCallback(130, nTaskNo, &(CFinishTriggerOrderField::m_Describe), pData, TSL_INSERT);
			}
			break;
		}
		case FID_Trade:
		{
			time_t tm = ((CTradeField*)pData)->InsertTime / 1000;
			struct tm timeinfo = GetTM(&tm);
			strftime(((CTradeField*)pData)->CreateTime.getBuffer(), 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
			break;
		}
		case FID_AccountDetail:
		{
			time_t tm = ((CAccountDetailField*)pData)->InsertTime / 1000;
			struct tm timeinfo = GetTM(&tm);
			strftime(((CAccountDetailField*)pData)->CreateTime.getBuffer(), 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
			break;
		}
		}
		ret = false;
	}
	return ret;
}

void CDelayMDB::OnTimer(UF_INT8 nTime)
{
	if (nTime != m_nTime2)
	{
		m_nTime2 = nTime;
		if (fmod(nTime, m_nFinishOrderSeconds) == 0)
			m_FinishOrder.clear();
	}
}

void CDelayMDB::OnTimer(const char* ZoneTime, UF_INT8 nTime)
{
	if (ZoneTime != NULL)
	{
		if (m_nTime + m_nDelaySeconds > nTime)
			return;
		m_nTime = nTime;
	}

	if (m_bSinkInit && m_nDelaySeconds > 0)
	{
		REPORT_EVENT(LOG_WARNING, "CDelayMDB OnTimer", "m_InsertAccount:%d", m_InsertAccount.size());
		REPORT_EVENT(LOG_WARNING, "CDelayMDB OnTimer", "m_UpdateAccount:%d", m_UpdateAccount.size());
		REPORT_EVENT(LOG_WARNING, "CDelayMDB OnTimer", "m_DeleteAccount:%d", m_DeleteAccount.size());

		REPORT_EVENT(LOG_WARNING, "CDelayMDB OnTimer", "m_InsertOrder:%d", m_InsertOrder.size());
		REPORT_EVENT(LOG_WARNING, "CDelayMDB OnTimer", "m_UpdateOrder:%d", m_UpdateOrder.size());
		REPORT_EVENT(LOG_WARNING, "CDelayMDB OnTimer", "m_DeleteOrder:%d", m_DeleteOrder.size());

		REPORT_EVENT(LOG_WARNING, "CDelayMDB OnTimer", "m_InsertPosition:%d", m_InsertPosition.size());
		REPORT_EVENT(LOG_WARNING, "CDelayMDB OnTimer", "m_UpdatePosition:%d", m_UpdatePosition.size());
		REPORT_EVENT(LOG_WARNING, "CDelayMDB OnTimer", "m_DeletePosition:%d", m_DeletePosition.size());
	}

	{
		CDelayAccountMap::iterator iter = m_InsertAccount.begin();
		while (iter != m_InsertAccount.end())
		{
			int nDispatchNo = iter->first.c_str()[iter->first.length() - 1] - '0';
			nDispatchNo = abs(nDispatchNo);
			int RequestNo = 5 + nDispatchNo % 10;
			m_pSinkDBDispatcher->DelayMDBCallback(RequestNo, iter->second.nTaskNo, &(CAccountField::m_Describe), &(iter->second.Data), TSL_INSERT);
			iter++;
		}
		m_InsertAccount.clear();
		iter = m_UpdateAccount.begin();
		while (iter != m_UpdateAccount.end())
		{
			int nDispatchNo = iter->first.c_str()[iter->first.length() - 1] - '0';
			nDispatchNo = abs(nDispatchNo);
			int RequestNo = 5 + nDispatchNo % 10;
			m_pSinkDBDispatcher->DelayMDBCallback(RequestNo, iter->second.nTaskNo, &(CAccountField::m_Describe), &(iter->second.Data), TSL_UPDATE);
			iter++;
		}
		m_UpdateAccount.clear();
		iter = m_DeleteAccount.begin();
		while (iter != m_DeleteAccount.end())
		{
			int nDispatchNo = iter->first.c_str()[iter->first.length() - 1] - '0';
			nDispatchNo = abs(nDispatchNo);
			int RequestNo = 5 + nDispatchNo % 10;
			m_pSinkDBDispatcher->DelayMDBCallback(RequestNo, iter->second.nTaskNo, &(CAccountField::m_Describe), &(iter->second.Data), TSL_DELETE);
			iter++;
		}
		m_DeleteAccount.clear();
	}

	{
		//check Delete
		//CDelayOrderMap::iterator iter = m_DeleteOrder.begin();
		//while (iter != m_DeleteOrder.end())
		//{
		//	CDelayOrderMap::iterator iter1 = m_InsertOrder.find(iter->first);
		//	if (iter1 != m_InsertOrder.end())
		//	{
		//		REPORT_EVENT(LOG_CRITICAL, "CDelayMDB Check Delete", "%s Exsit", iter1->first.c_str());
		//	}
		//	iter++;
		//}

		CDelayOrderMap::iterator iter = m_InsertOrder.begin();
		while (iter != m_InsertOrder.end())
		{
			int nDispatchNo = iter->first.c_str()[iter->first.length() - 1] - '0';
			nDispatchNo = abs(nDispatchNo);
			int RequestNo = 25 + nDispatchNo % 10;
			m_pSinkDBDispatcher->DelayMDBCallback(RequestNo, iter->second.nTaskNo, &(COrderField::m_Describe), &(iter->second.Data), TSL_INSERT);
			iter++;
		}
		m_InsertOrder.clear();
		iter = m_UpdateOrder.begin();
		while (iter != m_UpdateOrder.end())
		{
			int nDispatchNo = iter->first.c_str()[iter->first.length() - 1] - '0';
			nDispatchNo = abs(nDispatchNo);
			int RequestNo = 25 + nDispatchNo % 10;
			m_pSinkDBDispatcher->DelayMDBCallback(RequestNo, iter->second.nTaskNo, &(COrderField::m_Describe), &(iter->second.Data), TSL_UPDATE);
			iter++;
		}
		m_UpdateOrder.clear();
		iter = m_DeleteOrder.begin();
		while (iter != m_DeleteOrder.end())
		{
			int nDispatchNo = iter->first.c_str()[iter->first.length() - 1] - '0';
			nDispatchNo = abs(nDispatchNo);
			int RequestNo = 25 + nDispatchNo % 10;
			m_pSinkDBDispatcher->DelayMDBCallback(RequestNo, iter->second.nTaskNo, &(COrderField::m_Describe), &(iter->second.Data), TSL_DELETE);
			iter++;
		}
		m_DeleteOrder.clear();
	}

	{
		CDelayPositionMap::iterator iter = m_InsertPosition.begin();
		while (iter != m_InsertPosition.end())
		{
			int nDispatchNo = iter->first.c_str()[iter->first.length() - 1] - '0';
			nDispatchNo = abs(nDispatchNo);
			int RequestNo = 15 + nDispatchNo % 10;
			m_pSinkDBDispatcher->DelayMDBCallback(RequestNo, iter->second.nTaskNo, &(CPositionField::m_Describe), &(iter->second.Data), TSL_INSERT);
			iter++;
		}
		m_InsertPosition.clear();
		iter = m_UpdatePosition.begin();
		while (iter != m_UpdatePosition.end())
		{
			int nDispatchNo = iter->first.c_str()[iter->first.length() - 1] - '0';
			nDispatchNo = abs(nDispatchNo);
			int RequestNo = 15 + nDispatchNo % 10;
			m_pSinkDBDispatcher->DelayMDBCallback(RequestNo, iter->second.nTaskNo, &(CPositionField::m_Describe), &(iter->second.Data), TSL_UPDATE);
			iter++;
		}
		m_UpdatePosition.clear();
		iter = m_DeletePosition.begin();
		while (iter != m_DeletePosition.end())
		{
			int nDispatchNo = iter->first.c_str()[iter->first.length() - 1] - '0';
			nDispatchNo = abs(nDispatchNo);
			int RequestNo = 15 + nDispatchNo % 10;
			m_pSinkDBDispatcher->DelayMDBCallback(RequestNo, iter->second.nTaskNo, &(CPositionField::m_Describe), &(iter->second.Data), TSL_DELETE);
			iter++;
		}
		m_DeletePosition.clear();
	}

}