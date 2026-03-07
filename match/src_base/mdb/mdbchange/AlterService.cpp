/////////////////////////////////////////////////////////////////////////
///AlterService.h
///CAlterService
/////////////////////////////////////////////////////////////////////////

#include "AlterService.h"
#include "monitorIndex.h"
#include "FieldData.h"
#include "CopyField.h"

static CEventMonitor eventMonitor;

CAlterService::CAlterService(CMDB* pMDB, CErrorEngine* pErrorEngine)
{
	m_pMDB=pMDB;
	m_pErrorEngine=pErrorEngine;
}

CAlterService::~CAlterService()
{
}

void CAlterService::afterSync(CTransaction *pTransaction)
{
	m_pMDB->linkAll();
}

void CAlterService::checkLink(FILE *pFile) const
{
	m_pMDB->checkLink(pFile);
}

void CAlterService::checkNull(FILE *pFile) const
{
	m_pMDB->checkNull(pFile);
}

void CAlterService::checkValid(FILE *pFile) const
{
	m_pMDB->checkValid(pFile);
}

bool CAlterService::SyncCurrentTimeImpl(int nTid, CCurrentTimeField *pCurrentTime, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CCurrentTime *pOldCurrentTime = m_pMDB->m_CurrentTimeFactory->findByPK(pCurrentTime);
		if(pOldCurrentTime != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableCurrentTime theCurrentTime;
		CopyCurrentTimeEntity(&theCurrentTime,pCurrentTime);
		m_pMDB->m_CurrentTimeFactory->add(&theCurrentTime, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableCurrentTime theCurrentTime;
		CCurrentTime *pOldCurrentTime = m_pMDB->m_CurrentTimeFactory->findByPK(pCurrentTime);
		if(pOldCurrentTime == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theCurrentTime.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_CurrentTimeFactory->retrieve(pOldCurrentTime, &theCurrentTime);
		}

		if (!pCurrentTime->SystemID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("SystemID"))
					theCurrentTime.SystemID.clear();
			}
			else
			{
				if (pCurrentTime->SystemID.isValid())
				{
					theCurrentTime.SystemID = pCurrentTime->SystemID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's SystemID is invalid for %s", pCurrentTime->SystemID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SystemID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("SystemID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key SystemID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->ZoneDate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("ZoneDate"))
					theCurrentTime.ZoneDate.clear();
			}
			else
			{
				if (pCurrentTime->ZoneDate.isValid())
				{
					theCurrentTime.ZoneDate = pCurrentTime->ZoneDate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's ZoneDate is invalid for %s", pCurrentTime->ZoneDate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ZoneDate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("ZoneDate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key ZoneDate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->ZoneTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("ZoneTime"))
					theCurrentTime.ZoneTime.clear();
			}
			else
			{
				if (pCurrentTime->ZoneTime.isValid())
				{
					theCurrentTime.ZoneTime = pCurrentTime->ZoneTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's ZoneTime is invalid for %s", pCurrentTime->ZoneTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ZoneTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("ZoneTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key ZoneTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->ZoneDays.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("ZoneDays"))
					theCurrentTime.ZoneDays.clear();
			}
			else
			{
				if (pCurrentTime->ZoneDays.isValid())
				{
					theCurrentTime.ZoneDays = pCurrentTime->ZoneDays;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's ZoneDays is invalid for %s", pCurrentTime->ZoneDays.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ZoneDays");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("ZoneDays"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key ZoneDays is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->CurrTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("CurrTime"))
					theCurrentTime.CurrTime.clear();
			}
			else
			{
				if (pCurrentTime->CurrTime.isValid())
				{
					theCurrentTime.CurrTime = pCurrentTime->CurrTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's CurrTime is invalid for %s", pCurrentTime->CurrTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CurrTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("CurrTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key CurrTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("InsertTime"))
					theCurrentTime.InsertTime.clear();
			}
			else
			{
				if (pCurrentTime->InsertTime.isValid())
				{
					theCurrentTime.InsertTime = pCurrentTime->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's InsertTime is invalid for %s", pCurrentTime->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("UpdateTime"))
					theCurrentTime.UpdateTime.clear();
			}
			else
			{
				if (pCurrentTime->UpdateTime.isValid())
				{
					theCurrentTime.UpdateTime = pCurrentTime->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's UpdateTime is invalid for %s", pCurrentTime->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->TimeZone.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("TimeZone"))
					theCurrentTime.TimeZone.clear();
			}
			else
			{
				if (pCurrentTime->TimeZone.isValid())
				{
					theCurrentTime.TimeZone = pCurrentTime->TimeZone;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's TimeZone is invalid for %s", pCurrentTime->TimeZone.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TimeZone");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("TimeZone"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key TimeZone is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->LastTradeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("LastTradeID"))
					theCurrentTime.LastTradeID.clear();
			}
			else
			{
				if (pCurrentTime->LastTradeID.isValid())
				{
					theCurrentTime.LastTradeID = pCurrentTime->LastTradeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's LastTradeID is invalid for %s", pCurrentTime->LastTradeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LastTradeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("LastTradeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key LastTradeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->LastOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("LastOrderID"))
					theCurrentTime.LastOrderID.clear();
			}
			else
			{
				if (pCurrentTime->LastOrderID.isValid())
				{
					theCurrentTime.LastOrderID = pCurrentTime->LastOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's LastOrderID is invalid for %s", pCurrentTime->LastOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LastOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("LastOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key LastOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->LastAccountDetailID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("LastAccountDetailID"))
					theCurrentTime.LastAccountDetailID.clear();
			}
			else
			{
				if (pCurrentTime->LastAccountDetailID.isValid())
				{
					theCurrentTime.LastAccountDetailID = pCurrentTime->LastAccountDetailID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's LastAccountDetailID is invalid for %s", pCurrentTime->LastAccountDetailID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LastAccountDetailID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("LastAccountDetailID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key LastAccountDetailID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->LastTimeSortNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("LastTimeSortNo"))
					theCurrentTime.LastTimeSortNo.clear();
			}
			else
			{
				if (pCurrentTime->LastTimeSortNo.isValid())
				{
					theCurrentTime.LastTimeSortNo = pCurrentTime->LastTimeSortNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's LastTimeSortNo is invalid for %s", pCurrentTime->LastTimeSortNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LastTimeSortNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("LastTimeSortNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key LastTimeSortNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrentTime->LastBusinessNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrentTimeFactory->isPK("LastBusinessNo"))
					theCurrentTime.LastBusinessNo.clear();
			}
			else
			{
				if (pCurrentTime->LastBusinessNo.isValid())
				{
					theCurrentTime.LastBusinessNo = pCurrentTime->LastBusinessNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's LastBusinessNo is invalid for %s", pCurrentTime->LastBusinessNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LastBusinessNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrentTimeFactory->isPK("LastBusinessNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CurrentTime's Primary Key LastBusinessNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_CurrentTimeFactory->addOrUpdate(pOldCurrentTime, &theCurrentTime, pTransaction);
		CopyCurrentTimeEntity(pCurrentTime,&theCurrentTime);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_CurrentTimeFactory->removeByPK(pCurrentTime, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CCurrentTime *pOldCurrentTime = m_pMDB->m_CurrentTimeFactory->findByPK(pCurrentTime);
		if (pOldCurrentTime == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyCurrentTimeEntity(pCurrentTime,pOldCurrentTime);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncMemberImpl(int nTid, CMemberField *pMember, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CMember *pOldMember = m_pMDB->m_MemberFactory->findByPK(pMember);
		if(pOldMember != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableMember theMember;
		CopyMemberEntity(&theMember,pMember);
		m_pMDB->m_MemberFactory->add(&theMember, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableMember theMember;
		CMember *pOldMember = m_pMDB->m_MemberFactory->findByPK(pMember);
		if(pOldMember == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theMember.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_MemberFactory->retrieve(pOldMember, &theMember);
		}

		if (!pMember->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("MemberID"))
					theMember.MemberID.clear();
			}
			else
			{
				if (pMember->MemberID.isValid())
				{
					theMember.MemberID = pMember->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's MemberID is invalid for %s", pMember->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->MemberType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("MemberType"))
					theMember.MemberType.clear();
			}
			else
			{
				if (pMember->MemberType.isValid())
				{
					theMember.MemberType = pMember->MemberType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's MemberType is invalid for %s", pMember->MemberType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("MemberType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key MemberType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->Grade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("Grade"))
					theMember.Grade.clear();
			}
			else
			{
				if (pMember->Grade.isValid())
				{
					theMember.Grade = pMember->Grade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Grade is invalid for %s", pMember->Grade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Grade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("Grade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key Grade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->Priority.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("Priority"))
					theMember.Priority.clear();
			}
			else
			{
				if (pMember->Priority.isValid())
				{
					theMember.Priority = pMember->Priority;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Priority is invalid for %s", pMember->Priority.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Priority");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("Priority"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key Priority is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->FeeGrade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("FeeGrade"))
					theMember.FeeGrade.clear();
			}
			else
			{
				if (pMember->FeeGrade.isValid())
				{
					theMember.FeeGrade = pMember->FeeGrade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's FeeGrade is invalid for %s", pMember->FeeGrade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FeeGrade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("FeeGrade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key FeeGrade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->MarginRateGrade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("MarginRateGrade"))
					theMember.MarginRateGrade.clear();
			}
			else
			{
				if (pMember->MarginRateGrade.isValid())
				{
					theMember.MarginRateGrade = pMember->MarginRateGrade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's MarginRateGrade is invalid for %s", pMember->MarginRateGrade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MarginRateGrade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("MarginRateGrade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key MarginRateGrade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->TradingRightGrade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("TradingRightGrade"))
					theMember.TradingRightGrade.clear();
			}
			else
			{
				if (pMember->TradingRightGrade.isValid())
				{
					theMember.TradingRightGrade = pMember->TradingRightGrade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's TradingRightGrade is invalid for %s", pMember->TradingRightGrade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradingRightGrade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("TradingRightGrade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key TradingRightGrade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->CFDGrade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("CFDGrade"))
					theMember.CFDGrade.clear();
			}
			else
			{
				if (pMember->CFDGrade.isValid())
				{
					theMember.CFDGrade = pMember->CFDGrade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's CFDGrade is invalid for %s", pMember->CFDGrade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CFDGrade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("CFDGrade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key CFDGrade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->LiquidityGrade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("LiquidityGrade"))
					theMember.LiquidityGrade.clear();
			}
			else
			{
				if (pMember->LiquidityGrade.isValid())
				{
					theMember.LiquidityGrade = pMember->LiquidityGrade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's LiquidityGrade is invalid for %s", pMember->LiquidityGrade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LiquidityGrade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("LiquidityGrade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key LiquidityGrade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->RiskLevel.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("RiskLevel"))
					theMember.RiskLevel.clear();
			}
			else
			{
				if (pMember->RiskLevel.isValid())
				{
					theMember.RiskLevel = pMember->RiskLevel;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's RiskLevel is invalid for %s", pMember->RiskLevel.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"RiskLevel");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("RiskLevel"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key RiskLevel is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->PositionType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("PositionType"))
					theMember.PositionType.clear();
			}
			else
			{
				if (pMember->PositionType.isValid())
				{
					theMember.PositionType = pMember->PositionType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's PositionType is invalid for %s", pMember->PositionType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("PositionType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key PositionType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->TriggerOrders.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("TriggerOrders"))
					theMember.TriggerOrders.clear();
			}
			else
			{
				if (pMember->TriggerOrders.isValid())
				{
					theMember.TriggerOrders = pMember->TriggerOrders;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's TriggerOrders is invalid for %s", pMember->TriggerOrders.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerOrders");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("TriggerOrders"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key TriggerOrders is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->OpenOrders.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("OpenOrders"))
					theMember.OpenOrders.clear();
			}
			else
			{
				if (pMember->OpenOrders.isValid())
				{
					theMember.OpenOrders = pMember->OpenOrders;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's OpenOrders is invalid for %s", pMember->OpenOrders.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenOrders");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("OpenOrders"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key OpenOrders is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->OpenPositions.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("OpenPositions"))
					theMember.OpenPositions.clear();
			}
			else
			{
				if (pMember->OpenPositions.isValid())
				{
					theMember.OpenPositions = pMember->OpenPositions;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's OpenPositions is invalid for %s", pMember->OpenPositions.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenPositions");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("OpenPositions"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key OpenPositions is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->MaxLocalID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("MaxLocalID"))
					theMember.MaxLocalID.clear();
			}
			else
			{
				if (pMember->MaxLocalID.isValid())
				{
					theMember.MaxLocalID = pMember->MaxLocalID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's MaxLocalID is invalid for %s", pMember->MaxLocalID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MaxLocalID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("MaxLocalID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key MaxLocalID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->SinkType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("SinkType"))
					theMember.SinkType.clear();
			}
			else
			{
				if (pMember->SinkType.isValid())
				{
					theMember.SinkType = pMember->SinkType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's SinkType is invalid for %s", pMember->SinkType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SinkType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("SinkType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key SinkType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->CopyMemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("CopyMemberID"))
					theMember.CopyMemberID.clear();
			}
			else
			{
				if (pMember->CopyMemberID.isValid())
				{
					theMember.CopyMemberID = pMember->CopyMemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's CopyMemberID is invalid for %s", pMember->CopyMemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyMemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("CopyMemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key CopyMemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->Region.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("Region"))
					theMember.Region.clear();
			}
			else
			{
				if (pMember->Region.isValid())
				{
					theMember.Region = pMember->Region;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Region is invalid for %s", pMember->Region.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Region");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("Region"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key Region is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->FeeDeduceDisCount.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("FeeDeduceDisCount"))
					theMember.FeeDeduceDisCount.clear();
			}
			else
			{
				if (pMember->FeeDeduceDisCount.isValid())
				{
					theMember.FeeDeduceDisCount = pMember->FeeDeduceDisCount;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's FeeDeduceDisCount is invalid for %s", pMember->FeeDeduceDisCount.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FeeDeduceDisCount");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("FeeDeduceDisCount"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key FeeDeduceDisCount is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->FeeDeduceCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("FeeDeduceCurrency"))
					theMember.FeeDeduceCurrency.clear();
			}
			else
			{
				if (pMember->FeeDeduceCurrency.isValid())
				{
					theMember.FeeDeduceCurrency = pMember->FeeDeduceCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's FeeDeduceCurrency is invalid for %s", pMember->FeeDeduceCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FeeDeduceCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("FeeDeduceCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key FeeDeduceCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->CanBorrow.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("CanBorrow"))
					theMember.CanBorrow.clear();
			}
			else
			{
				if (pMember->CanBorrow.isValid())
				{
					theMember.CanBorrow = pMember->CanBorrow;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's CanBorrow is invalid for %s", pMember->CanBorrow.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CanBorrow");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("CanBorrow"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key CanBorrow is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("Remark"))
					theMember.Remark.clear();
			}
			else
			{
				if (pMember->Remark.isValid())
				{
					theMember.Remark = pMember->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Remark is invalid for %s", pMember->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("InsertTime"))
					theMember.InsertTime.clear();
			}
			else
			{
				if (pMember->InsertTime.isValid())
				{
					theMember.InsertTime = pMember->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's InsertTime is invalid for %s", pMember->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMember->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberFactory->isPK("UpdateTime"))
					theMember.UpdateTime.clear();
			}
			else
			{
				if (pMember->UpdateTime.isValid())
				{
					theMember.UpdateTime = pMember->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's UpdateTime is invalid for %s", pMember->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Member's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_MemberFactory->addOrUpdate(pOldMember, &theMember, pTransaction);
		CopyMemberEntity(pMember,&theMember);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_MemberFactory->removeByPK(pMember, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		m_pMDB->m_AccountFactory->linkAllMember();
		m_pMDB->m_OrderFactory->linkAllMember();
		m_pMDB->m_TriggerOrderFactory->linkAllMember();
		return true;
	}
	case TID_SendQryOne:
	{
		CMember *pOldMember = m_pMDB->m_MemberFactory->findByPK(pMember);
		if (pOldMember == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyMemberEntity(pMember,pOldMember);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncServiceConfigImpl(int nTid, CServiceConfigField *pServiceConfig, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CServiceConfig *pOldServiceConfig = m_pMDB->m_ServiceConfigFactory->findByPK(pServiceConfig);
		if(pOldServiceConfig != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableServiceConfig theServiceConfig;
		CopyServiceConfigEntity(&theServiceConfig,pServiceConfig);
		m_pMDB->m_ServiceConfigFactory->add(&theServiceConfig, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableServiceConfig theServiceConfig;
		CServiceConfig *pOldServiceConfig = m_pMDB->m_ServiceConfigFactory->findByPK(pServiceConfig);
		if(pOldServiceConfig == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theServiceConfig.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_ServiceConfigFactory->retrieve(pOldServiceConfig, &theServiceConfig);
		}

		if (!pServiceConfig->ConfigName.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_ServiceConfigFactory->isPK("ConfigName"))
					theServiceConfig.ConfigName.clear();
			}
			else
			{
				if (pServiceConfig->ConfigName.isValid())
				{
					theServiceConfig.ConfigName = pServiceConfig->ConfigName;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's ConfigName is invalid for %s", pServiceConfig->ConfigName.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ConfigName");
				}
			}
		}
		else
		{
			if(m_pMDB->m_ServiceConfigFactory->isPK("ConfigName"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's Primary Key ConfigName is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pServiceConfig->Index1.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_ServiceConfigFactory->isPK("Index1"))
					theServiceConfig.Index1.clear();
			}
			else
			{
				if (pServiceConfig->Index1.isValid())
				{
					theServiceConfig.Index1 = pServiceConfig->Index1;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's Index1 is invalid for %s", pServiceConfig->Index1.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Index1");
				}
			}
		}
		else
		{
			if(m_pMDB->m_ServiceConfigFactory->isPK("Index1"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's Primary Key Index1 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pServiceConfig->Index2.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_ServiceConfigFactory->isPK("Index2"))
					theServiceConfig.Index2.clear();
			}
			else
			{
				if (pServiceConfig->Index2.isValid())
				{
					theServiceConfig.Index2 = pServiceConfig->Index2;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's Index2 is invalid for %s", pServiceConfig->Index2.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Index2");
				}
			}
		}
		else
		{
			if(m_pMDB->m_ServiceConfigFactory->isPK("Index2"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's Primary Key Index2 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pServiceConfig->ConfigValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_ServiceConfigFactory->isPK("ConfigValue"))
					theServiceConfig.ConfigValue.clear();
			}
			else
			{
				if (pServiceConfig->ConfigValue.isValid())
				{
					theServiceConfig.ConfigValue = pServiceConfig->ConfigValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's ConfigValue is invalid for %s", pServiceConfig->ConfigValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ConfigValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_ServiceConfigFactory->isPK("ConfigValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's Primary Key ConfigValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pServiceConfig->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_ServiceConfigFactory->isPK("Remark"))
					theServiceConfig.Remark.clear();
			}
			else
			{
				if (pServiceConfig->Remark.isValid())
				{
					theServiceConfig.Remark = pServiceConfig->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's Remark is invalid for %s", pServiceConfig->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_ServiceConfigFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pServiceConfig->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_ServiceConfigFactory->isPK("InsertTime"))
					theServiceConfig.InsertTime.clear();
			}
			else
			{
				if (pServiceConfig->InsertTime.isValid())
				{
					theServiceConfig.InsertTime = pServiceConfig->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's InsertTime is invalid for %s", pServiceConfig->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_ServiceConfigFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pServiceConfig->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_ServiceConfigFactory->isPK("UpdateTime"))
					theServiceConfig.UpdateTime.clear();
			}
			else
			{
				if (pServiceConfig->UpdateTime.isValid())
				{
					theServiceConfig.UpdateTime = pServiceConfig->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's UpdateTime is invalid for %s", pServiceConfig->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_ServiceConfigFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_ServiceConfig's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_ServiceConfigFactory->addOrUpdate(pOldServiceConfig, &theServiceConfig, pTransaction);
		CopyServiceConfigEntity(pServiceConfig,&theServiceConfig);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_ServiceConfigFactory->removeByPK(pServiceConfig, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CServiceConfig *pOldServiceConfig = m_pMDB->m_ServiceConfigFactory->findByPK(pServiceConfig);
		if (pOldServiceConfig == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyServiceConfigEntity(pServiceConfig,pOldServiceConfig);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncUserSessionImpl(int nTid, CUserSessionField *pUserSession, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CUserSession *pOldUserSession = m_pMDB->m_UserSessionFactory->findByPK(pUserSession);
		if(pOldUserSession != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableUserSession theUserSession;
		CopyUserSessionEntity(&theUserSession,pUserSession);
		m_pMDB->m_UserSessionFactory->add(&theUserSession, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableUserSession theUserSession;
		CUserSession *pOldUserSession = m_pMDB->m_UserSessionFactory->findByPK(pUserSession);
		if(pOldUserSession == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theUserSession.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_UserSessionFactory->retrieve(pOldUserSession, &theUserSession);
		}

		if (!pUserSession->UserID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("UserID"))
					theUserSession.UserID.clear();
			}
			else
			{
				if (pUserSession->UserID.isValid())
				{
					theUserSession.UserID = pUserSession->UserID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's UserID is invalid for %s", pUserSession->UserID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UserID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("UserID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key UserID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->Password.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("Password"))
					theUserSession.Password.clear();
			}
			else
			{
				if (pUserSession->Password.isValid())
				{
					theUserSession.Password = pUserSession->Password;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Password is invalid for %s", pUserSession->Password.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Password");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("Password"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key Password is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->UserProductID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("UserProductID"))
					theUserSession.UserProductID.clear();
			}
			else
			{
				if (pUserSession->UserProductID.isValid())
				{
					theUserSession.UserProductID = pUserSession->UserProductID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's UserProductID is invalid for %s", pUserSession->UserProductID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UserProductID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("UserProductID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key UserProductID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->MacAddress.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("MacAddress"))
					theUserSession.MacAddress.clear();
			}
			else
			{
				if (pUserSession->MacAddress.isValid())
				{
					theUserSession.MacAddress = pUserSession->MacAddress;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's MacAddress is invalid for %s", pUserSession->MacAddress.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MacAddress");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("MacAddress"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key MacAddress is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->ClientIPAddress.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("ClientIPAddress"))
					theUserSession.ClientIPAddress.clear();
			}
			else
			{
				if (pUserSession->ClientIPAddress.isValid())
				{
					theUserSession.ClientIPAddress = pUserSession->ClientIPAddress;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's ClientIPAddress is invalid for %s", pUserSession->ClientIPAddress.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ClientIPAddress");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("ClientIPAddress"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key ClientIPAddress is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->HDSerialID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("HDSerialID"))
					theUserSession.HDSerialID.clear();
			}
			else
			{
				if (pUserSession->HDSerialID.isValid())
				{
					theUserSession.HDSerialID = pUserSession->HDSerialID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's HDSerialID is invalid for %s", pUserSession->HDSerialID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"HDSerialID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("HDSerialID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key HDSerialID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->AuthCode.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("AuthCode"))
					theUserSession.AuthCode.clear();
			}
			else
			{
				if (pUserSession->AuthCode.isValid())
				{
					theUserSession.AuthCode = pUserSession->AuthCode;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's AuthCode is invalid for %s", pUserSession->AuthCode.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AuthCode");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("AuthCode"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key AuthCode is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->APPID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("APPID"))
					theUserSession.APPID.clear();
			}
			else
			{
				if (pUserSession->APPID.isValid())
				{
					theUserSession.APPID = pUserSession->APPID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's APPID is invalid for %s", pUserSession->APPID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"APPID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("APPID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key APPID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->Token.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("Token"))
					theUserSession.Token.clear();
			}
			else
			{
				if (pUserSession->Token.isValid())
				{
					theUserSession.Token = pUserSession->Token;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Token is invalid for %s", pUserSession->Token.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Token");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("Token"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key Token is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("Remark"))
					theUserSession.Remark.clear();
			}
			else
			{
				if (pUserSession->Remark.isValid())
				{
					theUserSession.Remark = pUserSession->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Remark is invalid for %s", pUserSession->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->APIID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("APIID"))
					theUserSession.APIID.clear();
			}
			else
			{
				if (pUserSession->APIID.isValid())
				{
					theUserSession.APIID = pUserSession->APIID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's APIID is invalid for %s", pUserSession->APIID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"APIID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("APIID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key APIID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->LoginTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("LoginTime"))
					theUserSession.LoginTime.clear();
			}
			else
			{
				if (pUserSession->LoginTime.isValid())
				{
					theUserSession.LoginTime = pUserSession->LoginTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's LoginTime is invalid for %s", pUserSession->LoginTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LoginTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("LoginTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key LoginTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->ExpireTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("ExpireTime"))
					theUserSession.ExpireTime.clear();
			}
			else
			{
				if (pUserSession->ExpireTime.isValid())
				{
					theUserSession.ExpireTime = pUserSession->ExpireTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's ExpireTime is invalid for %s", pUserSession->ExpireTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExpireTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("ExpireTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key ExpireTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->IPAddress.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("IPAddress"))
					theUserSession.IPAddress.clear();
			}
			else
			{
				if (pUserSession->IPAddress.isValid())
				{
					theUserSession.IPAddress = pUserSession->IPAddress;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's IPAddress is invalid for %s", pUserSession->IPAddress.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"IPAddress");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("IPAddress"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key IPAddress is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->MaxLocalID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("MaxLocalID"))
					theUserSession.MaxLocalID.clear();
			}
			else
			{
				if (pUserSession->MaxLocalID.isValid())
				{
					theUserSession.MaxLocalID = pUserSession->MaxLocalID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's MaxLocalID is invalid for %s", pUserSession->MaxLocalID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MaxLocalID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("MaxLocalID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key MaxLocalID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->SessionNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("SessionNo"))
					theUserSession.SessionNo.clear();
			}
			else
			{
				if (pUserSession->SessionNo.isValid())
				{
					theUserSession.SessionNo = pUserSession->SessionNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's SessionNo is invalid for %s", pUserSession->SessionNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SessionNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("SessionNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key SessionNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->FrontNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("FrontNo"))
					theUserSession.FrontNo.clear();
			}
			else
			{
				if (pUserSession->FrontNo.isValid())
				{
					theUserSession.FrontNo = pUserSession->FrontNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's FrontNo is invalid for %s", pUserSession->FrontNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrontNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("FrontNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key FrontNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->AccessLimit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("AccessLimit"))
					theUserSession.AccessLimit.clear();
			}
			else
			{
				if (pUserSession->AccessLimit.isValid())
				{
					theUserSession.AccessLimit = pUserSession->AccessLimit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's AccessLimit is invalid for %s", pUserSession->AccessLimit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AccessLimit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("AccessLimit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key AccessLimit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->UserType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("UserType"))
					theUserSession.UserType.clear();
			}
			else
			{
				if (pUserSession->UserType.isValid())
				{
					theUserSession.UserType = pUserSession->UserType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's UserType is invalid for %s", pUserSession->UserType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UserType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("UserType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key UserType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("MemberID"))
					theUserSession.MemberID.clear();
			}
			else
			{
				if (pUserSession->MemberID.isValid())
				{
					theUserSession.MemberID = pUserSession->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's MemberID is invalid for %s", pUserSession->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->LimitAccesses.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("LimitAccesses"))
					theUserSession.LimitAccesses.clear();
			}
			else
			{
				if (pUserSession->LimitAccesses.isValid())
				{
					theUserSession.LimitAccesses = pUserSession->LimitAccesses;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's LimitAccesses is invalid for %s", pUserSession->LimitAccesses.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LimitAccesses");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("LimitAccesses"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key LimitAccesses is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->SinkType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("SinkType"))
					theUserSession.SinkType.clear();
			}
			else
			{
				if (pUserSession->SinkType.isValid())
				{
					theUserSession.SinkType = pUserSession->SinkType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's SinkType is invalid for %s", pUserSession->SinkType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SinkType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("SinkType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key SinkType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("InsertTime"))
					theUserSession.InsertTime.clear();
			}
			else
			{
				if (pUserSession->InsertTime.isValid())
				{
					theUserSession.InsertTime = pUserSession->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's InsertTime is invalid for %s", pUserSession->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pUserSession->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_UserSessionFactory->isPK("UpdateTime"))
					theUserSession.UpdateTime.clear();
			}
			else
			{
				if (pUserSession->UpdateTime.isValid())
				{
					theUserSession.UpdateTime = pUserSession->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's UpdateTime is invalid for %s", pUserSession->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_UserSessionFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_UserSession's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_UserSessionFactory->addOrUpdate(pOldUserSession, &theUserSession, pTransaction);
		CopyUserSessionEntity(pUserSession,&theUserSession);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_UserSessionFactory->removeByPK(pUserSession, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CUserSession *pOldUserSession = m_pMDB->m_UserSessionFactory->findByPK(pUserSession);
		if (pOldUserSession == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyUserSessionEntity(pUserSession,pOldUserSession);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncInstrumentImpl(int nTid, CInstrumentField *pInstrument, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CInstrument *pOldInstrument = m_pMDB->m_InstrumentFactory->findByPK(pInstrument);
		if(pOldInstrument != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableInstrument theInstrument;
		CopyInstrumentEntity(&theInstrument,pInstrument);
		m_pMDB->m_InstrumentFactory->add(&theInstrument, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableInstrument theInstrument;
		CInstrument *pOldInstrument = m_pMDB->m_InstrumentFactory->findByPK(pInstrument);
		if(pOldInstrument == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theInstrument.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_InstrumentFactory->retrieve(pOldInstrument, &theInstrument);
		}

		if (!pInstrument->ExchangeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("ExchangeID"))
					theInstrument.ExchangeID.clear();
			}
			else
			{
				if (pInstrument->ExchangeID.isValid())
				{
					theInstrument.ExchangeID = pInstrument->ExchangeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's ExchangeID is invalid for %s", pInstrument->ExchangeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExchangeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("ExchangeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key ExchangeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("InstrumentID"))
					theInstrument.InstrumentID.clear();
			}
			else
			{
				if (pInstrument->InstrumentID.isValid())
				{
					theInstrument.InstrumentID = pInstrument->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's InstrumentID is invalid for %s", pInstrument->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->InstrumentName.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("InstrumentName"))
					theInstrument.InstrumentName.clear();
			}
			else
			{
				if (pInstrument->InstrumentName.isValid())
				{
					theInstrument.InstrumentName = pInstrument->InstrumentName;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's InstrumentName is invalid for %s", pInstrument->InstrumentName.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentName");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("InstrumentName"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key InstrumentName is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->SettlementGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("SettlementGroup"))
					theInstrument.SettlementGroup.clear();
			}
			else
			{
				if (pInstrument->SettlementGroup.isValid())
				{
					theInstrument.SettlementGroup = pInstrument->SettlementGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's SettlementGroup is invalid for %s", pInstrument->SettlementGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SettlementGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("SettlementGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key SettlementGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->ProductGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("ProductGroup"))
					theInstrument.ProductGroup.clear();
			}
			else
			{
				if (pInstrument->ProductGroup.isValid())
				{
					theInstrument.ProductGroup = pInstrument->ProductGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's ProductGroup is invalid for %s", pInstrument->ProductGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ProductGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("ProductGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key ProductGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->UnderlyingID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("UnderlyingID"))
					theInstrument.UnderlyingID.clear();
			}
			else
			{
				if (pInstrument->UnderlyingID.isValid())
				{
					theInstrument.UnderlyingID = pInstrument->UnderlyingID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's UnderlyingID is invalid for %s", pInstrument->UnderlyingID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UnderlyingID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("UnderlyingID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key UnderlyingID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->ProductClass.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("ProductClass"))
					theInstrument.ProductClass.clear();
			}
			else
			{
				if (pInstrument->ProductClass.isValid())
				{
					theInstrument.ProductClass = pInstrument->ProductClass;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's ProductClass is invalid for %s", pInstrument->ProductClass.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ProductClass");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("ProductClass"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key ProductClass is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->ProductType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("ProductType"))
					theInstrument.ProductType.clear();
			}
			else
			{
				if (pInstrument->ProductType.isValid())
				{
					theInstrument.ProductType = pInstrument->ProductType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's ProductType is invalid for %s", pInstrument->ProductType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ProductType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("ProductType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key ProductType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->StrikePrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("StrikePrice"))
					theInstrument.StrikePrice.clear();
			}
			else
			{
				if (pInstrument->StrikePrice.isValid())
				{
					theInstrument.StrikePrice = pInstrument->StrikePrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's StrikePrice is invalid for %s", pInstrument->StrikePrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"StrikePrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("StrikePrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key StrikePrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->OptionsType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("OptionsType"))
					theInstrument.OptionsType.clear();
			}
			else
			{
				if (pInstrument->OptionsType.isValid())
				{
					theInstrument.OptionsType = pInstrument->OptionsType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's OptionsType is invalid for %s", pInstrument->OptionsType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OptionsType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("OptionsType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key OptionsType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->VolumeMultiple.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("VolumeMultiple"))
					theInstrument.VolumeMultiple.clear();
			}
			else
			{
				if (pInstrument->VolumeMultiple.isValid())
				{
					theInstrument.VolumeMultiple = pInstrument->VolumeMultiple;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's VolumeMultiple is invalid for %s", pInstrument->VolumeMultiple.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeMultiple");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("VolumeMultiple"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key VolumeMultiple is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->StartTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("StartTime"))
					theInstrument.StartTime.clear();
			}
			else
			{
				if (pInstrument->StartTime.isValid())
				{
					theInstrument.StartTime = pInstrument->StartTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's StartTime is invalid for %s", pInstrument->StartTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"StartTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("StartTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key StartTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->ExpireTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("ExpireTime"))
					theInstrument.ExpireTime.clear();
			}
			else
			{
				if (pInstrument->ExpireTime.isValid())
				{
					theInstrument.ExpireTime = pInstrument->ExpireTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's ExpireTime is invalid for %s", pInstrument->ExpireTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExpireTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("ExpireTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key ExpireTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->PriceCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("PriceCurrency"))
					theInstrument.PriceCurrency.clear();
			}
			else
			{
				if (pInstrument->PriceCurrency.isValid())
				{
					theInstrument.PriceCurrency = pInstrument->PriceCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's PriceCurrency is invalid for %s", pInstrument->PriceCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PriceCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("PriceCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key PriceCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->ClearCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("ClearCurrency"))
					theInstrument.ClearCurrency.clear();
			}
			else
			{
				if (pInstrument->ClearCurrency.isValid())
				{
					theInstrument.ClearCurrency = pInstrument->ClearCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's ClearCurrency is invalid for %s", pInstrument->ClearCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ClearCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("ClearCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key ClearCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->IsInverse.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("IsInverse"))
					theInstrument.IsInverse.clear();
			}
			else
			{
				if (pInstrument->IsInverse.isValid())
				{
					theInstrument.IsInverse = pInstrument->IsInverse;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's IsInverse is invalid for %s", pInstrument->IsInverse.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"IsInverse");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("IsInverse"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key IsInverse is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->BaseCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("BaseCurrency"))
					theInstrument.BaseCurrency.clear();
			}
			else
			{
				if (pInstrument->BaseCurrency.isValid())
				{
					theInstrument.BaseCurrency = pInstrument->BaseCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's BaseCurrency is invalid for %s", pInstrument->BaseCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BaseCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("BaseCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key BaseCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->MarginPriceType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("MarginPriceType"))
					theInstrument.MarginPriceType.clear();
			}
			else
			{
				if (pInstrument->MarginPriceType.isValid())
				{
					theInstrument.MarginPriceType = pInstrument->MarginPriceType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's MarginPriceType is invalid for %s", pInstrument->MarginPriceType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MarginPriceType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("MarginPriceType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key MarginPriceType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->BasisPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("BasisPrice"))
					theInstrument.BasisPrice.clear();
			}
			else
			{
				if (pInstrument->BasisPrice.isValid())
				{
					theInstrument.BasisPrice = pInstrument->BasisPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's BasisPrice is invalid for %s", pInstrument->BasisPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BasisPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("BasisPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key BasisPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->MinOrderVolume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("MinOrderVolume"))
					theInstrument.MinOrderVolume.clear();
			}
			else
			{
				if (pInstrument->MinOrderVolume.isValid())
				{
					theInstrument.MinOrderVolume = pInstrument->MinOrderVolume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's MinOrderVolume is invalid for %s", pInstrument->MinOrderVolume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MinOrderVolume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("MinOrderVolume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key MinOrderVolume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->MinOrderCost.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("MinOrderCost"))
					theInstrument.MinOrderCost.clear();
			}
			else
			{
				if (pInstrument->MinOrderCost.isValid())
				{
					theInstrument.MinOrderCost = pInstrument->MinOrderCost;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's MinOrderCost is invalid for %s", pInstrument->MinOrderCost.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MinOrderCost");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("MinOrderCost"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key MinOrderCost is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->MaxOrderVolume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("MaxOrderVolume"))
					theInstrument.MaxOrderVolume.clear();
			}
			else
			{
				if (pInstrument->MaxOrderVolume.isValid())
				{
					theInstrument.MaxOrderVolume = pInstrument->MaxOrderVolume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's MaxOrderVolume is invalid for %s", pInstrument->MaxOrderVolume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MaxOrderVolume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("MaxOrderVolume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key MaxOrderVolume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->PriceTick.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("PriceTick"))
					theInstrument.PriceTick.clear();
			}
			else
			{
				if (pInstrument->PriceTick.isValid())
				{
					theInstrument.PriceTick = pInstrument->PriceTick;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's PriceTick is invalid for %s", pInstrument->PriceTick.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PriceTick");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("PriceTick"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key PriceTick is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->VolumeTick.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("VolumeTick"))
					theInstrument.VolumeTick.clear();
			}
			else
			{
				if (pInstrument->VolumeTick.isValid())
				{
					theInstrument.VolumeTick = pInstrument->VolumeTick;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's VolumeTick is invalid for %s", pInstrument->VolumeTick.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeTick");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("VolumeTick"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key VolumeTick is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->ShowVolumeTick.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("ShowVolumeTick"))
					theInstrument.ShowVolumeTick.clear();
			}
			else
			{
				if (pInstrument->ShowVolumeTick.isValid())
				{
					theInstrument.ShowVolumeTick = pInstrument->ShowVolumeTick;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's ShowVolumeTick is invalid for %s", pInstrument->ShowVolumeTick.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ShowVolumeTick");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("ShowVolumeTick"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key ShowVolumeTick is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->TradingModel.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("TradingModel"))
					theInstrument.TradingModel.clear();
			}
			else
			{
				if (pInstrument->TradingModel.isValid())
				{
					theInstrument.TradingModel = pInstrument->TradingModel;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's TradingModel is invalid for %s", pInstrument->TradingModel.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradingModel");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("TradingModel"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key TradingModel is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->DayStartTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("DayStartTime"))
					theInstrument.DayStartTime.clear();
			}
			else
			{
				if (pInstrument->DayStartTime.isValid())
				{
					theInstrument.DayStartTime = pInstrument->DayStartTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's DayStartTime is invalid for %s", pInstrument->DayStartTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"DayStartTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("DayStartTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key DayStartTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->SortNumber.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("SortNumber"))
					theInstrument.SortNumber.clear();
			}
			else
			{
				if (pInstrument->SortNumber.isValid())
				{
					theInstrument.SortNumber = pInstrument->SortNumber;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's SortNumber is invalid for %s", pInstrument->SortNumber.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SortNumber");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("SortNumber"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key SortNumber is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("Remark"))
					theInstrument.Remark.clear();
			}
			else
			{
				if (pInstrument->Remark.isValid())
				{
					theInstrument.Remark = pInstrument->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Remark is invalid for %s", pInstrument->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->DefaultLeverage.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("DefaultLeverage"))
					theInstrument.DefaultLeverage.clear();
			}
			else
			{
				if (pInstrument->DefaultLeverage.isValid())
				{
					theInstrument.DefaultLeverage = pInstrument->DefaultLeverage;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's DefaultLeverage is invalid for %s", pInstrument->DefaultLeverage.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"DefaultLeverage");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("DefaultLeverage"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key DefaultLeverage is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->PriceLimitValueMode.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("PriceLimitValueMode"))
					theInstrument.PriceLimitValueMode.clear();
			}
			else
			{
				if (pInstrument->PriceLimitValueMode.isValid())
				{
					theInstrument.PriceLimitValueMode = pInstrument->PriceLimitValueMode;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's PriceLimitValueMode is invalid for %s", pInstrument->PriceLimitValueMode.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PriceLimitValueMode");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("PriceLimitValueMode"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key PriceLimitValueMode is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->PriceLimitPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("PriceLimitPrice"))
					theInstrument.PriceLimitPrice.clear();
			}
			else
			{
				if (pInstrument->PriceLimitPrice.isValid())
				{
					theInstrument.PriceLimitPrice = pInstrument->PriceLimitPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's PriceLimitPrice is invalid for %s", pInstrument->PriceLimitPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PriceLimitPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("PriceLimitPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key PriceLimitPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->PriceLimitUpperValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("PriceLimitUpperValue"))
					theInstrument.PriceLimitUpperValue.clear();
			}
			else
			{
				if (pInstrument->PriceLimitUpperValue.isValid())
				{
					theInstrument.PriceLimitUpperValue = pInstrument->PriceLimitUpperValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's PriceLimitUpperValue is invalid for %s", pInstrument->PriceLimitUpperValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PriceLimitUpperValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("PriceLimitUpperValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key PriceLimitUpperValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->PriceLimitLowerValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("PriceLimitLowerValue"))
					theInstrument.PriceLimitLowerValue.clear();
			}
			else
			{
				if (pInstrument->PriceLimitLowerValue.isValid())
				{
					theInstrument.PriceLimitLowerValue = pInstrument->PriceLimitLowerValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's PriceLimitLowerValue is invalid for %s", pInstrument->PriceLimitLowerValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PriceLimitLowerValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("PriceLimitLowerValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key PriceLimitLowerValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->MarginRateGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("MarginRateGroup"))
					theInstrument.MarginRateGroup.clear();
			}
			else
			{
				if (pInstrument->MarginRateGroup.isValid())
				{
					theInstrument.MarginRateGroup = pInstrument->MarginRateGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's MarginRateGroup is invalid for %s", pInstrument->MarginRateGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MarginRateGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("MarginRateGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key MarginRateGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->CFDGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("CFDGroup"))
					theInstrument.CFDGroup.clear();
			}
			else
			{
				if (pInstrument->CFDGroup.isValid())
				{
					theInstrument.CFDGroup = pInstrument->CFDGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's CFDGroup is invalid for %s", pInstrument->CFDGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CFDGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("CFDGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key CFDGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->FeeGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("FeeGroup"))
					theInstrument.FeeGroup.clear();
			}
			else
			{
				if (pInstrument->FeeGroup.isValid())
				{
					theInstrument.FeeGroup = pInstrument->FeeGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's FeeGroup is invalid for %s", pInstrument->FeeGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FeeGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("FeeGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key FeeGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->TradingRightGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("TradingRightGroup"))
					theInstrument.TradingRightGroup.clear();
			}
			else
			{
				if (pInstrument->TradingRightGroup.isValid())
				{
					theInstrument.TradingRightGroup = pInstrument->TradingRightGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's TradingRightGroup is invalid for %s", pInstrument->TradingRightGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradingRightGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("TradingRightGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key TradingRightGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->MaxOpenInterest.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("MaxOpenInterest"))
					theInstrument.MaxOpenInterest.clear();
			}
			else
			{
				if (pInstrument->MaxOpenInterest.isValid())
				{
					theInstrument.MaxOpenInterest = pInstrument->MaxOpenInterest;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's MaxOpenInterest is invalid for %s", pInstrument->MaxOpenInterest.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MaxOpenInterest");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("MaxOpenInterest"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key MaxOpenInterest is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->FundingRateGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("FundingRateGroup"))
					theInstrument.FundingRateGroup.clear();
			}
			else
			{
				if (pInstrument->FundingRateGroup.isValid())
				{
					theInstrument.FundingRateGroup = pInstrument->FundingRateGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's FundingRateGroup is invalid for %s", pInstrument->FundingRateGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FundingRateGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("FundingRateGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key FundingRateGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("InsertTime"))
					theInstrument.InsertTime.clear();
			}
			else
			{
				if (pInstrument->InsertTime.isValid())
				{
					theInstrument.InsertTime = pInstrument->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's InsertTime is invalid for %s", pInstrument->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("UpdateTime"))
					theInstrument.UpdateTime.clear();
			}
			else
			{
				if (pInstrument->UpdateTime.isValid())
				{
					theInstrument.UpdateTime = pInstrument->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's UpdateTime is invalid for %s", pInstrument->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pInstrument->MarkedPriceTimes.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_InstrumentFactory->isPK("MarkedPriceTimes"))
					theInstrument.MarkedPriceTimes.clear();
			}
			else
			{
				if (pInstrument->MarkedPriceTimes.isValid())
				{
					theInstrument.MarkedPriceTimes = pInstrument->MarkedPriceTimes;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's MarkedPriceTimes is invalid for %s", pInstrument->MarkedPriceTimes.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MarkedPriceTimes");
				}
			}
		}
		else
		{
			if(m_pMDB->m_InstrumentFactory->isPK("MarkedPriceTimes"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Instrument's Primary Key MarkedPriceTimes is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		theInstrument.m_pMDB = m_pMDB;
		theInstrument.linkMarketData();
		theInstrument.m_pMDB = m_pMDB;
		theInstrument.linkClearCurrency();
		theInstrument.m_pMDB = m_pMDB;
		theInstrument.linkBaseCurrency();
		m_pMDB->m_InstrumentFactory->addOrUpdate(pOldInstrument, &theInstrument, pTransaction);
		CopyInstrumentEntity(pInstrument,&theInstrument);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_InstrumentFactory->removeByPK(pInstrument, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		m_pMDB->m_PositionFactory->linkAllInstrument();
		m_pMDB->m_MarketDataFactory->linkAllInstrument();
		m_pMDB->m_OrderFactory->linkAllInstrument();
		m_pMDB->m_TriggerOrderFactory->linkAllInstrument();
		return true;
	}
	case TID_SendQryOne:
	{
		CInstrument *pOldInstrument = m_pMDB->m_InstrumentFactory->findByPK(pInstrument);
		if (pOldInstrument == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyInstrumentEntity(pInstrument,pOldInstrument);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncTradingRightImpl(int nTid, CTradingRightField *pTradingRight, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CTradingRight *pOldTradingRight = m_pMDB->m_TradingRightFactory->findByPK(pTradingRight);
		if(pOldTradingRight != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableTradingRight theTradingRight;
		CopyTradingRightEntity(&theTradingRight,pTradingRight);
		m_pMDB->m_TradingRightFactory->add(&theTradingRight, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableTradingRight theTradingRight;
		CTradingRight *pOldTradingRight = m_pMDB->m_TradingRightFactory->findByPK(pTradingRight);
		if(pOldTradingRight == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theTradingRight.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_TradingRightFactory->retrieve(pOldTradingRight, &theTradingRight);
		}

		if (!pTradingRight->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TradingRightFactory->isPK("MemberID"))
					theTradingRight.MemberID.clear();
			}
			else
			{
				if (pTradingRight->MemberID.isValid())
				{
					theTradingRight.MemberID = pTradingRight->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's MemberID is invalid for %s", pTradingRight->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TradingRightFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTradingRight->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TradingRightFactory->isPK("InstrumentID"))
					theTradingRight.InstrumentID.clear();
			}
			else
			{
				if (pTradingRight->InstrumentID.isValid())
				{
					theTradingRight.InstrumentID = pTradingRight->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's InstrumentID is invalid for %s", pTradingRight->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TradingRightFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTradingRight->Direction.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TradingRightFactory->isPK("Direction"))
					theTradingRight.Direction.clear();
			}
			else
			{
				if (pTradingRight->Direction.isValid())
				{
					theTradingRight.Direction = pTradingRight->Direction;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's Direction is invalid for %s", pTradingRight->Direction.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Direction");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TradingRightFactory->isPK("Direction"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's Primary Key Direction is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTradingRight->TradingRight.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TradingRightFactory->isPK("TradingRight"))
					theTradingRight.TradingRight.clear();
			}
			else
			{
				if (pTradingRight->TradingRight.isValid())
				{
					theTradingRight.TradingRight = pTradingRight->TradingRight;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's TradingRight is invalid for %s", pTradingRight->TradingRight.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradingRight");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TradingRightFactory->isPK("TradingRight"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's Primary Key TradingRight is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTradingRight->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TradingRightFactory->isPK("Remark"))
					theTradingRight.Remark.clear();
			}
			else
			{
				if (pTradingRight->Remark.isValid())
				{
					theTradingRight.Remark = pTradingRight->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's Remark is invalid for %s", pTradingRight->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TradingRightFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTradingRight->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TradingRightFactory->isPK("InsertTime"))
					theTradingRight.InsertTime.clear();
			}
			else
			{
				if (pTradingRight->InsertTime.isValid())
				{
					theTradingRight.InsertTime = pTradingRight->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's InsertTime is invalid for %s", pTradingRight->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TradingRightFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTradingRight->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TradingRightFactory->isPK("UpdateTime"))
					theTradingRight.UpdateTime.clear();
			}
			else
			{
				if (pTradingRight->UpdateTime.isValid())
				{
					theTradingRight.UpdateTime = pTradingRight->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's UpdateTime is invalid for %s", pTradingRight->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TradingRightFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TradingRight's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_TradingRightFactory->addOrUpdate(pOldTradingRight, &theTradingRight, pTransaction);
		CopyTradingRightEntity(pTradingRight,&theTradingRight);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_TradingRightFactory->removeByPK(pTradingRight, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CTradingRight *pOldTradingRight = m_pMDB->m_TradingRightFactory->findByPK(pTradingRight);
		if (pOldTradingRight == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyTradingRightEntity(pTradingRight,pOldTradingRight);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncMarginRateImpl(int nTid, CMarginRateField *pMarginRate, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CMarginRate *pOldMarginRate = m_pMDB->m_MarginRateFactory->findByPK(pMarginRate);
		if(pOldMarginRate != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableMarginRate theMarginRate;
		CopyMarginRateEntity(&theMarginRate,pMarginRate);
		m_pMDB->m_MarginRateFactory->add(&theMarginRate, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableMarginRate theMarginRate;
		CMarginRate *pOldMarginRate = m_pMDB->m_MarginRateFactory->findByPK(pMarginRate);
		if(pOldMarginRate == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theMarginRate.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_MarginRateFactory->retrieve(pOldMarginRate, &theMarginRate);
		}

		if (!pMarginRate->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("MemberID"))
					theMarginRate.MemberID.clear();
			}
			else
			{
				if (pMarginRate->MemberID.isValid())
				{
					theMarginRate.MemberID = pMarginRate->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's MemberID is invalid for %s", pMarginRate->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("InstrumentID"))
					theMarginRate.InstrumentID.clear();
			}
			else
			{
				if (pMarginRate->InstrumentID.isValid())
				{
					theMarginRate.InstrumentID = pMarginRate->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's InstrumentID is invalid for %s", pMarginRate->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->ValueMode.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("ValueMode"))
					theMarginRate.ValueMode.clear();
			}
			else
			{
				if (pMarginRate->ValueMode.isValid())
				{
					theMarginRate.ValueMode = pMarginRate->ValueMode;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's ValueMode is invalid for %s", pMarginRate->ValueMode.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ValueMode");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("ValueMode"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key ValueMode is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->MarginSequenceType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("MarginSequenceType"))
					theMarginRate.MarginSequenceType.clear();
			}
			else
			{
				if (pMarginRate->MarginSequenceType.isValid())
				{
					theMarginRate.MarginSequenceType = pMarginRate->MarginSequenceType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's MarginSequenceType is invalid for %s", pMarginRate->MarginSequenceType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MarginSequenceType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("MarginSequenceType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key MarginSequenceType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->RiskSequenceType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("RiskSequenceType"))
					theMarginRate.RiskSequenceType.clear();
			}
			else
			{
				if (pMarginRate->RiskSequenceType.isValid())
				{
					theMarginRate.RiskSequenceType = pMarginRate->RiskSequenceType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's RiskSequenceType is invalid for %s", pMarginRate->RiskSequenceType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"RiskSequenceType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("RiskSequenceType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key RiskSequenceType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->LongMarginRatio.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("LongMarginRatio"))
					theMarginRate.LongMarginRatio.clear();
			}
			else
			{
				if (pMarginRate->LongMarginRatio.isValid())
				{
					theMarginRate.LongMarginRatio = pMarginRate->LongMarginRatio;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's LongMarginRatio is invalid for %s", pMarginRate->LongMarginRatio.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LongMarginRatio");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("LongMarginRatio"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key LongMarginRatio is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->LongMaintMarginRatio.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("LongMaintMarginRatio"))
					theMarginRate.LongMaintMarginRatio.clear();
			}
			else
			{
				if (pMarginRate->LongMaintMarginRatio.isValid())
				{
					theMarginRate.LongMaintMarginRatio = pMarginRate->LongMaintMarginRatio;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's LongMaintMarginRatio is invalid for %s", pMarginRate->LongMaintMarginRatio.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LongMaintMarginRatio");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("LongMaintMarginRatio"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key LongMaintMarginRatio is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->LongMarginStep.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("LongMarginStep"))
					theMarginRate.LongMarginStep.clear();
			}
			else
			{
				if (pMarginRate->LongMarginStep.isValid())
				{
					theMarginRate.LongMarginStep = pMarginRate->LongMarginStep;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's LongMarginStep is invalid for %s", pMarginRate->LongMarginStep.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LongMarginStep");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("LongMarginStep"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key LongMarginStep is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->LongMaintMarginStep.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("LongMaintMarginStep"))
					theMarginRate.LongMaintMarginStep.clear();
			}
			else
			{
				if (pMarginRate->LongMaintMarginStep.isValid())
				{
					theMarginRate.LongMaintMarginStep = pMarginRate->LongMaintMarginStep;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's LongMaintMarginStep is invalid for %s", pMarginRate->LongMaintMarginStep.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LongMaintMarginStep");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("LongMaintMarginStep"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key LongMaintMarginStep is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->LongRiskLimit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("LongRiskLimit"))
					theMarginRate.LongRiskLimit.clear();
			}
			else
			{
				if (pMarginRate->LongRiskLimit.isValid())
				{
					theMarginRate.LongRiskLimit = pMarginRate->LongRiskLimit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's LongRiskLimit is invalid for %s", pMarginRate->LongRiskLimit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LongRiskLimit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("LongRiskLimit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key LongRiskLimit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->LongRiskStep.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("LongRiskStep"))
					theMarginRate.LongRiskStep.clear();
			}
			else
			{
				if (pMarginRate->LongRiskStep.isValid())
				{
					theMarginRate.LongRiskStep = pMarginRate->LongRiskStep;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's LongRiskStep is invalid for %s", pMarginRate->LongRiskStep.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LongRiskStep");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("LongRiskStep"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key LongRiskStep is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->ShortMarginRatio.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("ShortMarginRatio"))
					theMarginRate.ShortMarginRatio.clear();
			}
			else
			{
				if (pMarginRate->ShortMarginRatio.isValid())
				{
					theMarginRate.ShortMarginRatio = pMarginRate->ShortMarginRatio;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's ShortMarginRatio is invalid for %s", pMarginRate->ShortMarginRatio.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ShortMarginRatio");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("ShortMarginRatio"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key ShortMarginRatio is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->ShortMaintMarginRatio.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("ShortMaintMarginRatio"))
					theMarginRate.ShortMaintMarginRatio.clear();
			}
			else
			{
				if (pMarginRate->ShortMaintMarginRatio.isValid())
				{
					theMarginRate.ShortMaintMarginRatio = pMarginRate->ShortMaintMarginRatio;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's ShortMaintMarginRatio is invalid for %s", pMarginRate->ShortMaintMarginRatio.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ShortMaintMarginRatio");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("ShortMaintMarginRatio"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key ShortMaintMarginRatio is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->ShortMarginStep.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("ShortMarginStep"))
					theMarginRate.ShortMarginStep.clear();
			}
			else
			{
				if (pMarginRate->ShortMarginStep.isValid())
				{
					theMarginRate.ShortMarginStep = pMarginRate->ShortMarginStep;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's ShortMarginStep is invalid for %s", pMarginRate->ShortMarginStep.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ShortMarginStep");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("ShortMarginStep"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key ShortMarginStep is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->ShortMaintMarginStep.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("ShortMaintMarginStep"))
					theMarginRate.ShortMaintMarginStep.clear();
			}
			else
			{
				if (pMarginRate->ShortMaintMarginStep.isValid())
				{
					theMarginRate.ShortMaintMarginStep = pMarginRate->ShortMaintMarginStep;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's ShortMaintMarginStep is invalid for %s", pMarginRate->ShortMaintMarginStep.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ShortMaintMarginStep");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("ShortMaintMarginStep"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key ShortMaintMarginStep is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->ShortRiskLimit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("ShortRiskLimit"))
					theMarginRate.ShortRiskLimit.clear();
			}
			else
			{
				if (pMarginRate->ShortRiskLimit.isValid())
				{
					theMarginRate.ShortRiskLimit = pMarginRate->ShortRiskLimit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's ShortRiskLimit is invalid for %s", pMarginRate->ShortRiskLimit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ShortRiskLimit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("ShortRiskLimit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key ShortRiskLimit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->ShortRiskStep.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("ShortRiskStep"))
					theMarginRate.ShortRiskStep.clear();
			}
			else
			{
				if (pMarginRate->ShortRiskStep.isValid())
				{
					theMarginRate.ShortRiskStep = pMarginRate->ShortRiskStep;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's ShortRiskStep is invalid for %s", pMarginRate->ShortRiskStep.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ShortRiskStep");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("ShortRiskStep"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key ShortRiskStep is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->DefaultLeverage.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("DefaultLeverage"))
					theMarginRate.DefaultLeverage.clear();
			}
			else
			{
				if (pMarginRate->DefaultLeverage.isValid())
				{
					theMarginRate.DefaultLeverage = pMarginRate->DefaultLeverage;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's DefaultLeverage is invalid for %s", pMarginRate->DefaultLeverage.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"DefaultLeverage");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("DefaultLeverage"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key DefaultLeverage is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("Remark"))
					theMarginRate.Remark.clear();
			}
			else
			{
				if (pMarginRate->Remark.isValid())
				{
					theMarginRate.Remark = pMarginRate->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Remark is invalid for %s", pMarginRate->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("InsertTime"))
					theMarginRate.InsertTime.clear();
			}
			else
			{
				if (pMarginRate->InsertTime.isValid())
				{
					theMarginRate.InsertTime = pMarginRate->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's InsertTime is invalid for %s", pMarginRate->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarginRate->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarginRateFactory->isPK("UpdateTime"))
					theMarginRate.UpdateTime.clear();
			}
			else
			{
				if (pMarginRate->UpdateTime.isValid())
				{
					theMarginRate.UpdateTime = pMarginRate->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's UpdateTime is invalid for %s", pMarginRate->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarginRateFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarginRate's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_MarginRateFactory->addOrUpdate(pOldMarginRate, &theMarginRate, pTransaction);
		CopyMarginRateEntity(pMarginRate,&theMarginRate);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_MarginRateFactory->removeByPK(pMarginRate, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CMarginRate *pOldMarginRate = m_pMDB->m_MarginRateFactory->findByPK(pMarginRate);
		if (pOldMarginRate == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyMarginRateEntity(pMarginRate,pOldMarginRate);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncCFDRateImpl(int nTid, CCFDRateField *pCFDRate, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CCFDRate *pOldCFDRate = m_pMDB->m_CFDRateFactory->findByPK(pCFDRate);
		if(pOldCFDRate != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableCFDRate theCFDRate;
		CopyCFDRateEntity(&theCFDRate,pCFDRate);
		m_pMDB->m_CFDRateFactory->add(&theCFDRate, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableCFDRate theCFDRate;
		CCFDRate *pOldCFDRate = m_pMDB->m_CFDRateFactory->findByPK(pCFDRate);
		if(pOldCFDRate == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theCFDRate.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_CFDRateFactory->retrieve(pOldCFDRate, &theCFDRate);
		}

		if (!pCFDRate->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("MemberID"))
					theCFDRate.MemberID.clear();
			}
			else
			{
				if (pCFDRate->MemberID.isValid())
				{
					theCFDRate.MemberID = pCFDRate->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's MemberID is invalid for %s", pCFDRate->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCFDRate->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("InstrumentID"))
					theCFDRate.InstrumentID.clear();
			}
			else
			{
				if (pCFDRate->InstrumentID.isValid())
				{
					theCFDRate.InstrumentID = pCFDRate->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's InstrumentID is invalid for %s", pCFDRate->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCFDRate->TriggerOrderType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("TriggerOrderType"))
					theCFDRate.TriggerOrderType.clear();
			}
			else
			{
				if (pCFDRate->TriggerOrderType.isValid())
				{
					theCFDRate.TriggerOrderType = pCFDRate->TriggerOrderType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's TriggerOrderType is invalid for %s", pCFDRate->TriggerOrderType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerOrderType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("TriggerOrderType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key TriggerOrderType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCFDRate->RateBegin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("RateBegin"))
					theCFDRate.RateBegin.clear();
			}
			else
			{
				if (pCFDRate->RateBegin.isValid())
				{
					theCFDRate.RateBegin = pCFDRate->RateBegin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's RateBegin is invalid for %s", pCFDRate->RateBegin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"RateBegin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("RateBegin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key RateBegin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCFDRate->RateEnd.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("RateEnd"))
					theCFDRate.RateEnd.clear();
			}
			else
			{
				if (pCFDRate->RateEnd.isValid())
				{
					theCFDRate.RateEnd = pCFDRate->RateEnd;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's RateEnd is invalid for %s", pCFDRate->RateEnd.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"RateEnd");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("RateEnd"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key RateEnd is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCFDRate->CompVolume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("CompVolume"))
					theCFDRate.CompVolume.clear();
			}
			else
			{
				if (pCFDRate->CompVolume.isValid())
				{
					theCFDRate.CompVolume = pCFDRate->CompVolume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's CompVolume is invalid for %s", pCFDRate->CompVolume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CompVolume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("CompVolume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key CompVolume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCFDRate->CompRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("CompRate"))
					theCFDRate.CompRate.clear();
			}
			else
			{
				if (pCFDRate->CompRate.isValid())
				{
					theCFDRate.CompRate = pCFDRate->CompRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's CompRate is invalid for %s", pCFDRate->CompRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CompRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("CompRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key CompRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCFDRate->HardRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("HardRate"))
					theCFDRate.HardRate.clear();
			}
			else
			{
				if (pCFDRate->HardRate.isValid())
				{
					theCFDRate.HardRate = pCFDRate->HardRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's HardRate is invalid for %s", pCFDRate->HardRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"HardRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("HardRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key HardRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCFDRate->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("Remark"))
					theCFDRate.Remark.clear();
			}
			else
			{
				if (pCFDRate->Remark.isValid())
				{
					theCFDRate.Remark = pCFDRate->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Remark is invalid for %s", pCFDRate->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCFDRate->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("InsertTime"))
					theCFDRate.InsertTime.clear();
			}
			else
			{
				if (pCFDRate->InsertTime.isValid())
				{
					theCFDRate.InsertTime = pCFDRate->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's InsertTime is invalid for %s", pCFDRate->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCFDRate->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CFDRateFactory->isPK("UpdateTime"))
					theCFDRate.UpdateTime.clear();
			}
			else
			{
				if (pCFDRate->UpdateTime.isValid())
				{
					theCFDRate.UpdateTime = pCFDRate->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's UpdateTime is invalid for %s", pCFDRate->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CFDRateFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_CFDRate's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_CFDRateFactory->addOrUpdate(pOldCFDRate, &theCFDRate, pTransaction);
		CopyCFDRateEntity(pCFDRate,&theCFDRate);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_CFDRateFactory->removeByPK(pCFDRate, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CCFDRate *pOldCFDRate = m_pMDB->m_CFDRateFactory->findByPK(pCFDRate);
		if (pOldCFDRate == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyCFDRateEntity(pCFDRate,pOldCFDRate);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncFeeImpl(int nTid, CFeeField *pFee, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CFee *pOldFee = m_pMDB->m_FeeFactory->findByPK(pFee);
		if(pOldFee != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableFee theFee;
		CopyFeeEntity(&theFee,pFee);
		m_pMDB->m_FeeFactory->add(&theFee, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableFee theFee;
		CFee *pOldFee = m_pMDB->m_FeeFactory->findByPK(pFee);
		if(pOldFee == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theFee.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_FeeFactory->retrieve(pOldFee, &theFee);
		}

		if (!pFee->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("MemberID"))
					theFee.MemberID.clear();
			}
			else
			{
				if (pFee->MemberID.isValid())
				{
					theFee.MemberID = pFee->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's MemberID is invalid for %s", pFee->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("InstrumentID"))
					theFee.InstrumentID.clear();
			}
			else
			{
				if (pFee->InstrumentID.isValid())
				{
					theFee.InstrumentID = pFee->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's InstrumentID is invalid for %s", pFee->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->MatchRole.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("MatchRole"))
					theFee.MatchRole.clear();
			}
			else
			{
				if (pFee->MatchRole.isValid())
				{
					theFee.MatchRole = pFee->MatchRole;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's MatchRole is invalid for %s", pFee->MatchRole.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MatchRole");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("MatchRole"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key MatchRole is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->OpenFeeRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("OpenFeeRate"))
					theFee.OpenFeeRate.clear();
			}
			else
			{
				if (pFee->OpenFeeRate.isValid())
				{
					theFee.OpenFeeRate = pFee->OpenFeeRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's OpenFeeRate is invalid for %s", pFee->OpenFeeRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenFeeRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("OpenFeeRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key OpenFeeRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->OpenFeeAmount.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("OpenFeeAmount"))
					theFee.OpenFeeAmount.clear();
			}
			else
			{
				if (pFee->OpenFeeAmount.isValid())
				{
					theFee.OpenFeeAmount = pFee->OpenFeeAmount;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's OpenFeeAmount is invalid for %s", pFee->OpenFeeAmount.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenFeeAmount");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("OpenFeeAmount"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key OpenFeeAmount is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->CloseFeeRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("CloseFeeRate"))
					theFee.CloseFeeRate.clear();
			}
			else
			{
				if (pFee->CloseFeeRate.isValid())
				{
					theFee.CloseFeeRate = pFee->CloseFeeRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's CloseFeeRate is invalid for %s", pFee->CloseFeeRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseFeeRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("CloseFeeRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key CloseFeeRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->CloseFeeAmount.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("CloseFeeAmount"))
					theFee.CloseFeeAmount.clear();
			}
			else
			{
				if (pFee->CloseFeeAmount.isValid())
				{
					theFee.CloseFeeAmount = pFee->CloseFeeAmount;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's CloseFeeAmount is invalid for %s", pFee->CloseFeeAmount.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseFeeAmount");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("CloseFeeAmount"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key CloseFeeAmount is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->CloseTodayFeeRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("CloseTodayFeeRate"))
					theFee.CloseTodayFeeRate.clear();
			}
			else
			{
				if (pFee->CloseTodayFeeRate.isValid())
				{
					theFee.CloseTodayFeeRate = pFee->CloseTodayFeeRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's CloseTodayFeeRate is invalid for %s", pFee->CloseTodayFeeRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseTodayFeeRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("CloseTodayFeeRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key CloseTodayFeeRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->CloseTodayFeeAmount.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("CloseTodayFeeAmount"))
					theFee.CloseTodayFeeAmount.clear();
			}
			else
			{
				if (pFee->CloseTodayFeeAmount.isValid())
				{
					theFee.CloseTodayFeeAmount = pFee->CloseTodayFeeAmount;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's CloseTodayFeeAmount is invalid for %s", pFee->CloseTodayFeeAmount.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseTodayFeeAmount");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("CloseTodayFeeAmount"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key CloseTodayFeeAmount is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("Remark"))
					theFee.Remark.clear();
			}
			else
			{
				if (pFee->Remark.isValid())
				{
					theFee.Remark = pFee->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Remark is invalid for %s", pFee->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("InsertTime"))
					theFee.InsertTime.clear();
			}
			else
			{
				if (pFee->InsertTime.isValid())
				{
					theFee.InsertTime = pFee->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's InsertTime is invalid for %s", pFee->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pFee->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_FeeFactory->isPK("UpdateTime"))
					theFee.UpdateTime.clear();
			}
			else
			{
				if (pFee->UpdateTime.isValid())
				{
					theFee.UpdateTime = pFee->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's UpdateTime is invalid for %s", pFee->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_FeeFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Fee's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_FeeFactory->addOrUpdate(pOldFee, &theFee, pTransaction);
		CopyFeeEntity(pFee,&theFee);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_FeeFactory->removeByPK(pFee, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CFee *pOldFee = m_pMDB->m_FeeFactory->findByPK(pFee);
		if (pOldFee == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyFeeEntity(pFee,pOldFee);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncCurrencyImpl(int nTid, CCurrencyField *pCurrency, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CCurrency *pOldCurrency = m_pMDB->m_CurrencyFactory->findByPK(pCurrency);
		if(pOldCurrency != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableCurrency theCurrency;
		CopyCurrencyEntity(&theCurrency,pCurrency);
		m_pMDB->m_CurrencyFactory->add(&theCurrency, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableCurrency theCurrency;
		CCurrency *pOldCurrency = m_pMDB->m_CurrencyFactory->findByPK(pCurrency);
		if(pOldCurrency == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theCurrency.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_CurrencyFactory->retrieve(pOldCurrency, &theCurrency);
		}

		if (!pCurrency->SettlementGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("SettlementGroup"))
					theCurrency.SettlementGroup.clear();
			}
			else
			{
				if (pCurrency->SettlementGroup.isValid())
				{
					theCurrency.SettlementGroup = pCurrency->SettlementGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's SettlementGroup is invalid for %s", pCurrency->SettlementGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SettlementGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("SettlementGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key SettlementGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->Currency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("Currency"))
					theCurrency.Currency.clear();
			}
			else
			{
				if (pCurrency->Currency.isValid())
				{
					theCurrency.Currency = pCurrency->Currency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Currency is invalid for %s", pCurrency->Currency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Currency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("Currency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key Currency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->Deposit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("Deposit"))
					theCurrency.Deposit.clear();
			}
			else
			{
				if (pCurrency->Deposit.isValid())
				{
					theCurrency.Deposit = pCurrency->Deposit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Deposit is invalid for %s", pCurrency->Deposit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Deposit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("Deposit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key Deposit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->Withdraw.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("Withdraw"))
					theCurrency.Withdraw.clear();
			}
			else
			{
				if (pCurrency->Withdraw.isValid())
				{
					theCurrency.Withdraw = pCurrency->Withdraw;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Withdraw is invalid for %s", pCurrency->Withdraw.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Withdraw");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("Withdraw"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key Withdraw is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->Balance.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("Balance"))
					theCurrency.Balance.clear();
			}
			else
			{
				if (pCurrency->Balance.isValid())
				{
					theCurrency.Balance = pCurrency->Balance;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Balance is invalid for %s", pCurrency->Balance.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Balance");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("Balance"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key Balance is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->PreBalance.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("PreBalance"))
					theCurrency.PreBalance.clear();
			}
			else
			{
				if (pCurrency->PreBalance.isValid())
				{
					theCurrency.PreBalance = pCurrency->PreBalance;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's PreBalance is invalid for %s", pCurrency->PreBalance.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PreBalance");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("PreBalance"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key PreBalance is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->SortNumber.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("SortNumber"))
					theCurrency.SortNumber.clear();
			}
			else
			{
				if (pCurrency->SortNumber.isValid())
				{
					theCurrency.SortNumber = pCurrency->SortNumber;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's SortNumber is invalid for %s", pCurrency->SortNumber.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SortNumber");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("SortNumber"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key SortNumber is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->CanReduce.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("CanReduce"))
					theCurrency.CanReduce.clear();
			}
			else
			{
				if (pCurrency->CanReduce.isValid())
				{
					theCurrency.CanReduce = pCurrency->CanReduce;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's CanReduce is invalid for %s", pCurrency->CanReduce.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CanReduce");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("CanReduce"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key CanReduce is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->IsTrading.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("IsTrading"))
					theCurrency.IsTrading.clear();
			}
			else
			{
				if (pCurrency->IsTrading.isValid())
				{
					theCurrency.IsTrading = pCurrency->IsTrading;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's IsTrading is invalid for %s", pCurrency->IsTrading.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"IsTrading");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("IsTrading"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key IsTrading is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->SettleSegment.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("SettleSegment"))
					theCurrency.SettleSegment.clear();
			}
			else
			{
				if (pCurrency->SettleSegment.isValid())
				{
					theCurrency.SettleSegment = pCurrency->SettleSegment;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's SettleSegment is invalid for %s", pCurrency->SettleSegment.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SettleSegment");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("SettleSegment"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key SettleSegment is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->MoneyTick.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("MoneyTick"))
					theCurrency.MoneyTick.clear();
			}
			else
			{
				if (pCurrency->MoneyTick.isValid())
				{
					theCurrency.MoneyTick = pCurrency->MoneyTick;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's MoneyTick is invalid for %s", pCurrency->MoneyTick.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MoneyTick");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("MoneyTick"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key MoneyTick is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->ExchangeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("ExchangeID"))
					theCurrency.ExchangeID.clear();
			}
			else
			{
				if (pCurrency->ExchangeID.isValid())
				{
					theCurrency.ExchangeID = pCurrency->ExchangeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's ExchangeID is invalid for %s", pCurrency->ExchangeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExchangeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("ExchangeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key ExchangeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("InstrumentID"))
					theCurrency.InstrumentID.clear();
			}
			else
			{
				if (pCurrency->InstrumentID.isValid())
				{
					theCurrency.InstrumentID = pCurrency->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's InstrumentID is invalid for %s", pCurrency->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->LoanUse.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("LoanUse"))
					theCurrency.LoanUse.clear();
			}
			else
			{
				if (pCurrency->LoanUse.isValid())
				{
					theCurrency.LoanUse = pCurrency->LoanUse;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's LoanUse is invalid for %s", pCurrency->LoanUse.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LoanUse");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("LoanUse"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key LoanUse is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->LoanLimit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("LoanLimit"))
					theCurrency.LoanLimit.clear();
			}
			else
			{
				if (pCurrency->LoanLimit.isValid())
				{
					theCurrency.LoanLimit = pCurrency->LoanLimit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's LoanLimit is invalid for %s", pCurrency->LoanLimit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LoanLimit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("LoanLimit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key LoanLimit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->LoanBorrow.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("LoanBorrow"))
					theCurrency.LoanBorrow.clear();
			}
			else
			{
				if (pCurrency->LoanBorrow.isValid())
				{
					theCurrency.LoanBorrow = pCurrency->LoanBorrow;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's LoanBorrow is invalid for %s", pCurrency->LoanBorrow.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LoanBorrow");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("LoanBorrow"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key LoanBorrow is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->LoanDeposit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("LoanDeposit"))
					theCurrency.LoanDeposit.clear();
			}
			else
			{
				if (pCurrency->LoanDeposit.isValid())
				{
					theCurrency.LoanDeposit = pCurrency->LoanDeposit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's LoanDeposit is invalid for %s", pCurrency->LoanDeposit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LoanDeposit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("LoanDeposit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key LoanDeposit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->LoanBase.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("LoanBase"))
					theCurrency.LoanBase.clear();
			}
			else
			{
				if (pCurrency->LoanBase.isValid())
				{
					theCurrency.LoanBase = pCurrency->LoanBase;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's LoanBase is invalid for %s", pCurrency->LoanBase.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LoanBase");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("LoanBase"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key LoanBase is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->LoanRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("LoanRate"))
					theCurrency.LoanRate.clear();
			}
			else
			{
				if (pCurrency->LoanRate.isValid())
				{
					theCurrency.LoanRate = pCurrency->LoanRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's LoanRate is invalid for %s", pCurrency->LoanRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LoanRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("LoanRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key LoanRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->LoanHighBase.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("LoanHighBase"))
					theCurrency.LoanHighBase.clear();
			}
			else
			{
				if (pCurrency->LoanHighBase.isValid())
				{
					theCurrency.LoanHighBase = pCurrency->LoanHighBase;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's LoanHighBase is invalid for %s", pCurrency->LoanHighBase.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LoanHighBase");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("LoanHighBase"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key LoanHighBase is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->LoanHighRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("LoanHighRate"))
					theCurrency.LoanHighRate.clear();
			}
			else
			{
				if (pCurrency->LoanHighRate.isValid())
				{
					theCurrency.LoanHighRate = pCurrency->LoanHighRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's LoanHighRate is invalid for %s", pCurrency->LoanHighRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LoanHighRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("LoanHighRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key LoanHighRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("Remark"))
					theCurrency.Remark.clear();
			}
			else
			{
				if (pCurrency->Remark.isValid())
				{
					theCurrency.Remark = pCurrency->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Remark is invalid for %s", pCurrency->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->CurrValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("CurrValue"))
					theCurrency.CurrValue.clear();
			}
			else
			{
				if (pCurrency->CurrValue.isValid())
				{
					theCurrency.CurrValue = pCurrency->CurrValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's CurrValue is invalid for %s", pCurrency->CurrValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CurrValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("CurrValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key CurrValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->HairCut.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("HairCut"))
					theCurrency.HairCut.clear();
			}
			else
			{
				if (pCurrency->HairCut.isValid())
				{
					theCurrency.HairCut = pCurrency->HairCut;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's HairCut is invalid for %s", pCurrency->HairCut.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"HairCut");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("HairCut"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key HairCut is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->MaxValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("MaxValue"))
					theCurrency.MaxValue.clear();
			}
			else
			{
				if (pCurrency->MaxValue.isValid())
				{
					theCurrency.MaxValue = pCurrency->MaxValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's MaxValue is invalid for %s", pCurrency->MaxValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MaxValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("MaxValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key MaxValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->MarginRateGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("MarginRateGroup"))
					theCurrency.MarginRateGroup.clear();
			}
			else
			{
				if (pCurrency->MarginRateGroup.isValid())
				{
					theCurrency.MarginRateGroup = pCurrency->MarginRateGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's MarginRateGroup is invalid for %s", pCurrency->MarginRateGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MarginRateGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("MarginRateGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key MarginRateGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->PrivateEnd.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("PrivateEnd"))
					theCurrency.PrivateEnd.clear();
			}
			else
			{
				if (pCurrency->PrivateEnd.isValid())
				{
					theCurrency.PrivateEnd = pCurrency->PrivateEnd;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's PrivateEnd is invalid for %s", pCurrency->PrivateEnd.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PrivateEnd");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("PrivateEnd"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key PrivateEnd is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("InsertTime"))
					theCurrency.InsertTime.clear();
			}
			else
			{
				if (pCurrency->InsertTime.isValid())
				{
					theCurrency.InsertTime = pCurrency->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's InsertTime is invalid for %s", pCurrency->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pCurrency->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_CurrencyFactory->isPK("UpdateTime"))
					theCurrency.UpdateTime.clear();
			}
			else
			{
				if (pCurrency->UpdateTime.isValid())
				{
					theCurrency.UpdateTime = pCurrency->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's UpdateTime is invalid for %s", pCurrency->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_CurrencyFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Currency's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		theCurrency.m_pMDB = m_pMDB;
		theCurrency.linkMarketData();
		m_pMDB->m_CurrencyFactory->addOrUpdate(pOldCurrency, &theCurrency, pTransaction);
		CopyCurrencyEntity(pCurrency,&theCurrency);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_CurrencyFactory->removeByPK(pCurrency, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		m_pMDB->m_InstrumentFactory->linkAllClearCurrency();
		m_pMDB->m_InstrumentFactory->linkAllBaseCurrency();
		m_pMDB->m_AccountFactory->linkAllCurrency();
		return true;
	}
	case TID_SendQryOne:
	{
		CCurrency *pOldCurrency = m_pMDB->m_CurrencyFactory->findByPK(pCurrency);
		if (pOldCurrency == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyCurrencyEntity(pCurrency,pOldCurrency);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncLiquidityImpl(int nTid, CLiquidityField *pLiquidity, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CLiquidity *pOldLiquidity = m_pMDB->m_LiquidityFactory->findByPK(pLiquidity);
		if(pOldLiquidity != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableLiquidity theLiquidity;
		CopyLiquidityEntity(&theLiquidity,pLiquidity);
		m_pMDB->m_LiquidityFactory->add(&theLiquidity, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableLiquidity theLiquidity;
		CLiquidity *pOldLiquidity = m_pMDB->m_LiquidityFactory->findByPK(pLiquidity);
		if(pOldLiquidity == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theLiquidity.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_LiquidityFactory->retrieve(pOldLiquidity, &theLiquidity);
		}

		if (!pLiquidity->LiquidityGrade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LiquidityFactory->isPK("LiquidityGrade"))
					theLiquidity.LiquidityGrade.clear();
			}
			else
			{
				if (pLiquidity->LiquidityGrade.isValid())
				{
					theLiquidity.LiquidityGrade = pLiquidity->LiquidityGrade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's LiquidityGrade is invalid for %s", pLiquidity->LiquidityGrade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LiquidityGrade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LiquidityFactory->isPK("LiquidityGrade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's Primary Key LiquidityGrade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLiquidity->LiquidityGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LiquidityFactory->isPK("LiquidityGroup"))
					theLiquidity.LiquidityGroup.clear();
			}
			else
			{
				if (pLiquidity->LiquidityGroup.isValid())
				{
					theLiquidity.LiquidityGroup = pLiquidity->LiquidityGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's LiquidityGroup is invalid for %s", pLiquidity->LiquidityGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LiquidityGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LiquidityFactory->isPK("LiquidityGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's Primary Key LiquidityGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLiquidity->BasePriceType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LiquidityFactory->isPK("BasePriceType"))
					theLiquidity.BasePriceType.clear();
			}
			else
			{
				if (pLiquidity->BasePriceType.isValid())
				{
					theLiquidity.BasePriceType = pLiquidity->BasePriceType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's BasePriceType is invalid for %s", pLiquidity->BasePriceType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BasePriceType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LiquidityFactory->isPK("BasePriceType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's Primary Key BasePriceType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLiquidity->VolumeBegin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LiquidityFactory->isPK("VolumeBegin"))
					theLiquidity.VolumeBegin.clear();
			}
			else
			{
				if (pLiquidity->VolumeBegin.isValid())
				{
					theLiquidity.VolumeBegin = pLiquidity->VolumeBegin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's VolumeBegin is invalid for %s", pLiquidity->VolumeBegin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeBegin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LiquidityFactory->isPK("VolumeBegin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's Primary Key VolumeBegin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLiquidity->BuyOpen.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LiquidityFactory->isPK("BuyOpen"))
					theLiquidity.BuyOpen.clear();
			}
			else
			{
				if (pLiquidity->BuyOpen.isValid())
				{
					theLiquidity.BuyOpen = pLiquidity->BuyOpen;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's BuyOpen is invalid for %s", pLiquidity->BuyOpen.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BuyOpen");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LiquidityFactory->isPK("BuyOpen"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's Primary Key BuyOpen is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLiquidity->BuyClose.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LiquidityFactory->isPK("BuyClose"))
					theLiquidity.BuyClose.clear();
			}
			else
			{
				if (pLiquidity->BuyClose.isValid())
				{
					theLiquidity.BuyClose = pLiquidity->BuyClose;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's BuyClose is invalid for %s", pLiquidity->BuyClose.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BuyClose");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LiquidityFactory->isPK("BuyClose"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's Primary Key BuyClose is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLiquidity->SellOpen.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LiquidityFactory->isPK("SellOpen"))
					theLiquidity.SellOpen.clear();
			}
			else
			{
				if (pLiquidity->SellOpen.isValid())
				{
					theLiquidity.SellOpen = pLiquidity->SellOpen;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's SellOpen is invalid for %s", pLiquidity->SellOpen.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SellOpen");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LiquidityFactory->isPK("SellOpen"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's Primary Key SellOpen is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLiquidity->SellClose.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LiquidityFactory->isPK("SellClose"))
					theLiquidity.SellClose.clear();
			}
			else
			{
				if (pLiquidity->SellClose.isValid())
				{
					theLiquidity.SellClose = pLiquidity->SellClose;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's SellClose is invalid for %s", pLiquidity->SellClose.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SellClose");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LiquidityFactory->isPK("SellClose"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's Primary Key SellClose is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLiquidity->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LiquidityFactory->isPK("InsertTime"))
					theLiquidity.InsertTime.clear();
			}
			else
			{
				if (pLiquidity->InsertTime.isValid())
				{
					theLiquidity.InsertTime = pLiquidity->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's InsertTime is invalid for %s", pLiquidity->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LiquidityFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLiquidity->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LiquidityFactory->isPK("UpdateTime"))
					theLiquidity.UpdateTime.clear();
			}
			else
			{
				if (pLiquidity->UpdateTime.isValid())
				{
					theLiquidity.UpdateTime = pLiquidity->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's UpdateTime is invalid for %s", pLiquidity->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LiquidityFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Liquidity's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_LiquidityFactory->addOrUpdate(pOldLiquidity, &theLiquidity, pTransaction);
		CopyLiquidityEntity(pLiquidity,&theLiquidity);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_LiquidityFactory->removeByPK(pLiquidity, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CLiquidity *pOldLiquidity = m_pMDB->m_LiquidityFactory->findByPK(pLiquidity);
		if (pOldLiquidity == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyLiquidityEntity(pLiquidity,pOldLiquidity);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncAccountImpl(int nTid, CAccountField *pAccount, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CAccount *pOldAccount = m_pMDB->m_AccountFactory->findByPK(pAccount);
		if(pOldAccount != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableAccount theAccount;
		CopyAccountEntity(&theAccount,pAccount);
		m_pMDB->m_AccountFactory->add(&theAccount, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableAccount theAccount;
		CAccount *pOldAccount = m_pMDB->m_AccountFactory->findByPK(pAccount);
		if(pOldAccount == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theAccount.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_AccountFactory->retrieve(pOldAccount, &theAccount);
		}

		if (!pAccount->SettlementGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("SettlementGroup"))
					theAccount.SettlementGroup.clear();
			}
			else
			{
				if (pAccount->SettlementGroup.isValid())
				{
					theAccount.SettlementGroup = pAccount->SettlementGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's SettlementGroup is invalid for %s", pAccount->SettlementGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SettlementGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("SettlementGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key SettlementGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->AccountID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("AccountID"))
					theAccount.AccountID.clear();
			}
			else
			{
				if (pAccount->AccountID.isValid())
				{
					theAccount.AccountID = pAccount->AccountID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's AccountID is invalid for %s", pAccount->AccountID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AccountID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("AccountID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key AccountID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Currency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Currency"))
					theAccount.Currency.clear();
			}
			else
			{
				if (pAccount->Currency.isValid())
				{
					theAccount.Currency = pAccount->Currency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Currency is invalid for %s", pAccount->Currency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Currency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Currency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Currency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("MemberID"))
					theAccount.MemberID.clear();
			}
			else
			{
				if (pAccount->MemberID.isValid())
				{
					theAccount.MemberID = pAccount->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's MemberID is invalid for %s", pAccount->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Available.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Available"))
					theAccount.Available.clear();
			}
			else
			{
				if (pAccount->Available.isValid())
				{
					theAccount.Available = pAccount->Available;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Available is invalid for %s", pAccount->Available.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Available");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Available"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Available is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Withdrawable.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Withdrawable"))
					theAccount.Withdrawable.clear();
			}
			else
			{
				if (pAccount->Withdrawable.isValid())
				{
					theAccount.Withdrawable = pAccount->Withdrawable;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Withdrawable is invalid for %s", pAccount->Withdrawable.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Withdrawable");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Withdrawable"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Withdrawable is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->PreBalance.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("PreBalance"))
					theAccount.PreBalance.clear();
			}
			else
			{
				if (pAccount->PreBalance.isValid())
				{
					theAccount.PreBalance = pAccount->PreBalance;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's PreBalance is invalid for %s", pAccount->PreBalance.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PreBalance");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("PreBalance"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key PreBalance is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Balance.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Balance"))
					theAccount.Balance.clear();
			}
			else
			{
				if (pAccount->Balance.isValid())
				{
					theAccount.Balance = pAccount->Balance;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Balance is invalid for %s", pAccount->Balance.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Balance");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Balance"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Balance is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->PositionCost.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("PositionCost"))
					theAccount.PositionCost.clear();
			}
			else
			{
				if (pAccount->PositionCost.isValid())
				{
					theAccount.PositionCost = pAccount->PositionCost;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's PositionCost is invalid for %s", pAccount->PositionCost.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionCost");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("PositionCost"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key PositionCost is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->UseMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("UseMargin"))
					theAccount.UseMargin.clear();
			}
			else
			{
				if (pAccount->UseMargin.isValid())
				{
					theAccount.UseMargin = pAccount->UseMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's UseMargin is invalid for %s", pAccount->UseMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UseMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("UseMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key UseMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->CrossMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("CrossMargin"))
					theAccount.CrossMargin.clear();
			}
			else
			{
				if (pAccount->CrossMargin.isValid())
				{
					theAccount.CrossMargin = pAccount->CrossMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's CrossMargin is invalid for %s", pAccount->CrossMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CrossMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("CrossMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key CrossMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->CloseProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("CloseProfit"))
					theAccount.CloseProfit.clear();
			}
			else
			{
				if (pAccount->CloseProfit.isValid())
				{
					theAccount.CloseProfit = pAccount->CloseProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's CloseProfit is invalid for %s", pAccount->CloseProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("CloseProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key CloseProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Deposit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Deposit"))
					theAccount.Deposit.clear();
			}
			else
			{
				if (pAccount->Deposit.isValid())
				{
					theAccount.Deposit = pAccount->Deposit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Deposit is invalid for %s", pAccount->Deposit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Deposit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Deposit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Deposit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Withdraw.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Withdraw"))
					theAccount.Withdraw.clear();
			}
			else
			{
				if (pAccount->Withdraw.isValid())
				{
					theAccount.Withdraw = pAccount->Withdraw;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Withdraw is invalid for %s", pAccount->Withdraw.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Withdraw");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Withdraw"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Withdraw is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->FrozenMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("FrozenMargin"))
					theAccount.FrozenMargin.clear();
			}
			else
			{
				if (pAccount->FrozenMargin.isValid())
				{
					theAccount.FrozenMargin = pAccount->FrozenMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's FrozenMargin is invalid for %s", pAccount->FrozenMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrozenMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("FrozenMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key FrozenMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->FrozenMoney.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("FrozenMoney"))
					theAccount.FrozenMoney.clear();
			}
			else
			{
				if (pAccount->FrozenMoney.isValid())
				{
					theAccount.FrozenMoney = pAccount->FrozenMoney;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's FrozenMoney is invalid for %s", pAccount->FrozenMoney.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrozenMoney");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("FrozenMoney"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key FrozenMoney is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->FrozenFee.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("FrozenFee"))
					theAccount.FrozenFee.clear();
			}
			else
			{
				if (pAccount->FrozenFee.isValid())
				{
					theAccount.FrozenFee = pAccount->FrozenFee;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's FrozenFee is invalid for %s", pAccount->FrozenFee.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrozenFee");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("FrozenFee"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key FrozenFee is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Fee.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Fee"))
					theAccount.Fee.clear();
			}
			else
			{
				if (pAccount->Fee.isValid())
				{
					theAccount.Fee = pAccount->Fee;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Fee is invalid for %s", pAccount->Fee.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Fee");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Fee"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Fee is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->MoneyChange.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("MoneyChange"))
					theAccount.MoneyChange.clear();
			}
			else
			{
				if (pAccount->MoneyChange.isValid())
				{
					theAccount.MoneyChange = pAccount->MoneyChange;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's MoneyChange is invalid for %s", pAccount->MoneyChange.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MoneyChange");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("MoneyChange"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key MoneyChange is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Reserve.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Reserve"))
					theAccount.Reserve.clear();
			}
			else
			{
				if (pAccount->Reserve.isValid())
				{
					theAccount.Reserve = pAccount->Reserve;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Reserve is invalid for %s", pAccount->Reserve.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Reserve");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Reserve"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Reserve is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->ReserveAvailable.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("ReserveAvailable"))
					theAccount.ReserveAvailable.clear();
			}
			else
			{
				if (pAccount->ReserveAvailable.isValid())
				{
					theAccount.ReserveAvailable = pAccount->ReserveAvailable;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's ReserveAvailable is invalid for %s", pAccount->ReserveAvailable.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ReserveAvailable");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("ReserveAvailable"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key ReserveAvailable is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->ReserveLimit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("ReserveLimit"))
					theAccount.ReserveLimit.clear();
			}
			else
			{
				if (pAccount->ReserveLimit.isValid())
				{
					theAccount.ReserveLimit = pAccount->ReserveLimit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's ReserveLimit is invalid for %s", pAccount->ReserveLimit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ReserveLimit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("ReserveLimit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key ReserveLimit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->ReserveProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("ReserveProfit"))
					theAccount.ReserveProfit.clear();
			}
			else
			{
				if (pAccount->ReserveProfit.isValid())
				{
					theAccount.ReserveProfit = pAccount->ReserveProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's ReserveProfit is invalid for %s", pAccount->ReserveProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ReserveProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("ReserveProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key ReserveProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->ReserveType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("ReserveType"))
					theAccount.ReserveType.clear();
			}
			else
			{
				if (pAccount->ReserveType.isValid())
				{
					theAccount.ReserveType = pAccount->ReserveType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's ReserveType is invalid for %s", pAccount->ReserveType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ReserveType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("ReserveType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key ReserveType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->ReserveActive.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("ReserveActive"))
					theAccount.ReserveActive.clear();
			}
			else
			{
				if (pAccount->ReserveActive.isValid())
				{
					theAccount.ReserveActive = pAccount->ReserveActive;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's ReserveActive is invalid for %s", pAccount->ReserveActive.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ReserveActive");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("ReserveActive"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key ReserveActive is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->TotalCloseProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("TotalCloseProfit"))
					theAccount.TotalCloseProfit.clear();
			}
			else
			{
				if (pAccount->TotalCloseProfit.isValid())
				{
					theAccount.TotalCloseProfit = pAccount->TotalCloseProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's TotalCloseProfit is invalid for %s", pAccount->TotalCloseProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TotalCloseProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("TotalCloseProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key TotalCloseProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->TotalDeposit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("TotalDeposit"))
					theAccount.TotalDeposit.clear();
			}
			else
			{
				if (pAccount->TotalDeposit.isValid())
				{
					theAccount.TotalDeposit = pAccount->TotalDeposit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's TotalDeposit is invalid for %s", pAccount->TotalDeposit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TotalDeposit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("TotalDeposit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key TotalDeposit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->TotalWithdraw.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("TotalWithdraw"))
					theAccount.TotalWithdraw.clear();
			}
			else
			{
				if (pAccount->TotalWithdraw.isValid())
				{
					theAccount.TotalWithdraw = pAccount->TotalWithdraw;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's TotalWithdraw is invalid for %s", pAccount->TotalWithdraw.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TotalWithdraw");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("TotalWithdraw"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key TotalWithdraw is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->TotalFee.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("TotalFee"))
					theAccount.TotalFee.clear();
			}
			else
			{
				if (pAccount->TotalFee.isValid())
				{
					theAccount.TotalFee = pAccount->TotalFee;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's TotalFee is invalid for %s", pAccount->TotalFee.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TotalFee");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("TotalFee"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key TotalFee is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->TotalMoneyChange.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("TotalMoneyChange"))
					theAccount.TotalMoneyChange.clear();
			}
			else
			{
				if (pAccount->TotalMoneyChange.isValid())
				{
					theAccount.TotalMoneyChange = pAccount->TotalMoneyChange;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's TotalMoneyChange is invalid for %s", pAccount->TotalMoneyChange.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TotalMoneyChange");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("TotalMoneyChange"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key TotalMoneyChange is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("InsertTime"))
					theAccount.InsertTime.clear();
			}
			else
			{
				if (pAccount->InsertTime.isValid())
				{
					theAccount.InsertTime = pAccount->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's InsertTime is invalid for %s", pAccount->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("UpdateTime"))
					theAccount.UpdateTime.clear();
			}
			else
			{
				if (pAccount->UpdateTime.isValid())
				{
					theAccount.UpdateTime = pAccount->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's UpdateTime is invalid for %s", pAccount->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->AccountType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("AccountType"))
					theAccount.AccountType.clear();
			}
			else
			{
				if (pAccount->AccountType.isValid())
				{
					theAccount.AccountType = pAccount->AccountType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's AccountType is invalid for %s", pAccount->AccountType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AccountType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("AccountType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key AccountType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->BusinessNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("BusinessNo"))
					theAccount.BusinessNo.clear();
			}
			else
			{
				if (pAccount->BusinessNo.isValid())
				{
					theAccount.BusinessNo = pAccount->BusinessNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's BusinessNo is invalid for %s", pAccount->BusinessNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("BusinessNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key BusinessNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->MarginRateGrade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("MarginRateGrade"))
					theAccount.MarginRateGrade.clear();
			}
			else
			{
				if (pAccount->MarginRateGrade.isValid())
				{
					theAccount.MarginRateGrade = pAccount->MarginRateGrade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's MarginRateGrade is invalid for %s", pAccount->MarginRateGrade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MarginRateGrade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("MarginRateGrade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key MarginRateGrade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->TradingRightGrade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("TradingRightGrade"))
					theAccount.TradingRightGrade.clear();
			}
			else
			{
				if (pAccount->TradingRightGrade.isValid())
				{
					theAccount.TradingRightGrade = pAccount->TradingRightGrade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's TradingRightGrade is invalid for %s", pAccount->TradingRightGrade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradingRightGrade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("TradingRightGrade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key TradingRightGrade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Frequency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Frequency"))
					theAccount.Frequency.clear();
			}
			else
			{
				if (pAccount->Frequency.isValid())
				{
					theAccount.Frequency = pAccount->Frequency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Frequency is invalid for %s", pAccount->Frequency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Frequency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Frequency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Frequency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->MaintMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("MaintMargin"))
					theAccount.MaintMargin.clear();
			}
			else
			{
				if (pAccount->MaintMargin.isValid())
				{
					theAccount.MaintMargin = pAccount->MaintMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's MaintMargin is invalid for %s", pAccount->MaintMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MaintMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("MaintMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key MaintMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->UnRealProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("UnRealProfit"))
					theAccount.UnRealProfit.clear();
			}
			else
			{
				if (pAccount->UnRealProfit.isValid())
				{
					theAccount.UnRealProfit = pAccount->UnRealProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's UnRealProfit is invalid for %s", pAccount->UnRealProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UnRealProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("UnRealProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key UnRealProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->CopyProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("CopyProfit"))
					theAccount.CopyProfit.clear();
			}
			else
			{
				if (pAccount->CopyProfit.isValid())
				{
					theAccount.CopyProfit = pAccount->CopyProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's CopyProfit is invalid for %s", pAccount->CopyProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("CopyProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key CopyProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->CFDType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("CFDType"))
					theAccount.CFDType.clear();
			}
			else
			{
				if (pAccount->CFDType.isValid())
				{
					theAccount.CFDType = pAccount->CFDType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's CFDType is invalid for %s", pAccount->CFDType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CFDType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("CFDType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key CFDType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->FeeGrade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("FeeGrade"))
					theAccount.FeeGrade.clear();
			}
			else
			{
				if (pAccount->FeeGrade.isValid())
				{
					theAccount.FeeGrade = pAccount->FeeGrade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's FeeGrade is invalid for %s", pAccount->FeeGrade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FeeGrade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("FeeGrade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key FeeGrade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->CFDGrade.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("CFDGrade"))
					theAccount.CFDGrade.clear();
			}
			else
			{
				if (pAccount->CFDGrade.isValid())
				{
					theAccount.CFDGrade = pAccount->CFDGrade;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's CFDGrade is invalid for %s", pAccount->CFDGrade.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CFDGrade");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("CFDGrade"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key CFDGrade is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Borrow.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Borrow"))
					theAccount.Borrow.clear();
			}
			else
			{
				if (pAccount->Borrow.isValid())
				{
					theAccount.Borrow = pAccount->Borrow;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Borrow is invalid for %s", pAccount->Borrow.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Borrow");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Borrow"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Borrow is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("Remark"))
					theAccount.Remark.clear();
			}
			else
			{
				if (pAccount->Remark.isValid())
				{
					theAccount.Remark = pAccount->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Remark is invalid for %s", pAccount->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->MaxLocalID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("MaxLocalID"))
					theAccount.MaxLocalID.clear();
			}
			else
			{
				if (pAccount->MaxLocalID.isValid())
				{
					theAccount.MaxLocalID = pAccount->MaxLocalID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's MaxLocalID is invalid for %s", pAccount->MaxLocalID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MaxLocalID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("MaxLocalID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key MaxLocalID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->CurrValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("CurrValue"))
					theAccount.CurrValue.clear();
			}
			else
			{
				if (pAccount->CurrValue.isValid())
				{
					theAccount.CurrValue = pAccount->CurrValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's CurrValue is invalid for %s", pAccount->CurrValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CurrValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("CurrValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key CurrValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pAccount->CanReduce.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_AccountFactory->isPK("CanReduce"))
					theAccount.CanReduce.clear();
			}
			else
			{
				if (pAccount->CanReduce.isValid())
				{
					theAccount.CanReduce = pAccount->CanReduce;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's CanReduce is invalid for %s", pAccount->CanReduce.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CanReduce");
				}
			}
		}
		else
		{
			if(m_pMDB->m_AccountFactory->isPK("CanReduce"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Account's Primary Key CanReduce is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		theAccount.m_pMDB = m_pMDB;
		theAccount.linkCurrency();
		theAccount.m_pMDB = m_pMDB;
		theAccount.linkMember();
		m_pMDB->m_AccountFactory->addOrUpdate(pOldAccount, &theAccount, pTransaction);
		CopyAccountEntity(pAccount,&theAccount);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_AccountFactory->removeByPK(pAccount, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		m_pMDB->m_PositionFactory->linkAllAccount();
		return true;
	}
	case TID_SendQryOne:
	{
		CAccount *pOldAccount = m_pMDB->m_AccountFactory->findByPK(pAccount);
		if (pOldAccount == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyAccountEntity(pAccount,pOldAccount);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncMemberPositionImpl(int nTid, CMemberPositionField *pMemberPosition, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CMemberPosition *pOldMemberPosition = m_pMDB->m_MemberPositionFactory->findByPK(pMemberPosition);
		if(pOldMemberPosition != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableMemberPosition theMemberPosition;
		CopyMemberPositionEntity(&theMemberPosition,pMemberPosition);
		m_pMDB->m_MemberPositionFactory->add(&theMemberPosition, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableMemberPosition theMemberPosition;
		CMemberPosition *pOldMemberPosition = m_pMDB->m_MemberPositionFactory->findByPK(pMemberPosition);
		if(pOldMemberPosition == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theMemberPosition.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_MemberPositionFactory->retrieve(pOldMemberPosition, &theMemberPosition);
		}

		if (!pMemberPosition->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("MemberID"))
					theMemberPosition.MemberID.clear();
			}
			else
			{
				if (pMemberPosition->MemberID.isValid())
				{
					theMemberPosition.MemberID = pMemberPosition->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's MemberID is invalid for %s", pMemberPosition->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMemberPosition->AccountID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("AccountID"))
					theMemberPosition.AccountID.clear();
			}
			else
			{
				if (pMemberPosition->AccountID.isValid())
				{
					theMemberPosition.AccountID = pMemberPosition->AccountID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's AccountID is invalid for %s", pMemberPosition->AccountID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AccountID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("AccountID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key AccountID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMemberPosition->ExchangeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("ExchangeID"))
					theMemberPosition.ExchangeID.clear();
			}
			else
			{
				if (pMemberPosition->ExchangeID.isValid())
				{
					theMemberPosition.ExchangeID = pMemberPosition->ExchangeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's ExchangeID is invalid for %s", pMemberPosition->ExchangeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExchangeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("ExchangeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key ExchangeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMemberPosition->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("InstrumentID"))
					theMemberPosition.InstrumentID.clear();
			}
			else
			{
				if (pMemberPosition->InstrumentID.isValid())
				{
					theMemberPosition.InstrumentID = pMemberPosition->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's InstrumentID is invalid for %s", pMemberPosition->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMemberPosition->Position.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("Position"))
					theMemberPosition.Position.clear();
			}
			else
			{
				if (pMemberPosition->Position.isValid())
				{
					theMemberPosition.Position = pMemberPosition->Position;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Position is invalid for %s", pMemberPosition->Position.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Position");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("Position"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key Position is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMemberPosition->LongFrozen.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("LongFrozen"))
					theMemberPosition.LongFrozen.clear();
			}
			else
			{
				if (pMemberPosition->LongFrozen.isValid())
				{
					theMemberPosition.LongFrozen = pMemberPosition->LongFrozen;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's LongFrozen is invalid for %s", pMemberPosition->LongFrozen.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LongFrozen");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("LongFrozen"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key LongFrozen is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMemberPosition->ShortFrozen.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("ShortFrozen"))
					theMemberPosition.ShortFrozen.clear();
			}
			else
			{
				if (pMemberPosition->ShortFrozen.isValid())
				{
					theMemberPosition.ShortFrozen = pMemberPosition->ShortFrozen;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's ShortFrozen is invalid for %s", pMemberPosition->ShortFrozen.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ShortFrozen");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("ShortFrozen"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key ShortFrozen is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMemberPosition->DefaultLeverage.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("DefaultLeverage"))
					theMemberPosition.DefaultLeverage.clear();
			}
			else
			{
				if (pMemberPosition->DefaultLeverage.isValid())
				{
					theMemberPosition.DefaultLeverage = pMemberPosition->DefaultLeverage;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's DefaultLeverage is invalid for %s", pMemberPosition->DefaultLeverage.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"DefaultLeverage");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("DefaultLeverage"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key DefaultLeverage is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMemberPosition->DefaultCrossMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("DefaultCrossMargin"))
					theMemberPosition.DefaultCrossMargin.clear();
			}
			else
			{
				if (pMemberPosition->DefaultCrossMargin.isValid())
				{
					theMemberPosition.DefaultCrossMargin = pMemberPosition->DefaultCrossMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's DefaultCrossMargin is invalid for %s", pMemberPosition->DefaultCrossMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"DefaultCrossMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("DefaultCrossMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key DefaultCrossMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMemberPosition->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("InsertTime"))
					theMemberPosition.InsertTime.clear();
			}
			else
			{
				if (pMemberPosition->InsertTime.isValid())
				{
					theMemberPosition.InsertTime = pMemberPosition->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's InsertTime is invalid for %s", pMemberPosition->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMemberPosition->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MemberPositionFactory->isPK("UpdateTime"))
					theMemberPosition.UpdateTime.clear();
			}
			else
			{
				if (pMemberPosition->UpdateTime.isValid())
				{
					theMemberPosition.UpdateTime = pMemberPosition->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's UpdateTime is invalid for %s", pMemberPosition->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MemberPositionFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MemberPosition's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_MemberPositionFactory->addOrUpdate(pOldMemberPosition, &theMemberPosition, pTransaction);
		CopyMemberPositionEntity(pMemberPosition,&theMemberPosition);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_MemberPositionFactory->removeByPK(pMemberPosition, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CMemberPosition *pOldMemberPosition = m_pMDB->m_MemberPositionFactory->findByPK(pMemberPosition);
		if (pOldMemberPosition == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyMemberPositionEntity(pMemberPosition,pOldMemberPosition);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncPositionImpl(int nTid, CPositionField *pPosition, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CPosition *pOldPosition = m_pMDB->m_PositionFactory->findByPK(pPosition);
		if(pOldPosition != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteablePosition thePosition;
		CopyPositionEntity(&thePosition,pPosition);
		m_pMDB->m_PositionFactory->add(&thePosition, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteablePosition thePosition;
		CPosition *pOldPosition = m_pMDB->m_PositionFactory->findByPK(pPosition);
		if(pOldPosition == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			thePosition.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_PositionFactory->retrieve(pOldPosition, &thePosition);
		}

		if (!pPosition->PositionID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("PositionID"))
					thePosition.PositionID.clear();
			}
			else
			{
				if (pPosition->PositionID.isValid())
				{
					thePosition.PositionID = pPosition->PositionID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's PositionID is invalid for %s", pPosition->PositionID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("PositionID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key PositionID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("MemberID"))
					thePosition.MemberID.clear();
			}
			else
			{
				if (pPosition->MemberID.isValid())
				{
					thePosition.MemberID = pPosition->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's MemberID is invalid for %s", pPosition->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->TradeUnitID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("TradeUnitID"))
					thePosition.TradeUnitID.clear();
			}
			else
			{
				if (pPosition->TradeUnitID.isValid())
				{
					thePosition.TradeUnitID = pPosition->TradeUnitID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's TradeUnitID is invalid for %s", pPosition->TradeUnitID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradeUnitID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("TradeUnitID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key TradeUnitID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->ExchangeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("ExchangeID"))
					thePosition.ExchangeID.clear();
			}
			else
			{
				if (pPosition->ExchangeID.isValid())
				{
					thePosition.ExchangeID = pPosition->ExchangeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's ExchangeID is invalid for %s", pPosition->ExchangeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExchangeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("ExchangeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key ExchangeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("InstrumentID"))
					thePosition.InstrumentID.clear();
			}
			else
			{
				if (pPosition->InstrumentID.isValid())
				{
					thePosition.InstrumentID = pPosition->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's InstrumentID is invalid for %s", pPosition->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->PositionClass.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("PositionClass"))
					thePosition.PositionClass.clear();
			}
			else
			{
				if (pPosition->PositionClass.isValid())
				{
					thePosition.PositionClass = pPosition->PositionClass;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's PositionClass is invalid for %s", pPosition->PositionClass.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionClass");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("PositionClass"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key PositionClass is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->PosiDirection.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("PosiDirection"))
					thePosition.PosiDirection.clear();
			}
			else
			{
				if (pPosition->PosiDirection.isValid())
				{
					thePosition.PosiDirection = pPosition->PosiDirection;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's PosiDirection is invalid for %s", pPosition->PosiDirection.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PosiDirection");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("PosiDirection"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key PosiDirection is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->PrePosition.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("PrePosition"))
					thePosition.PrePosition.clear();
			}
			else
			{
				if (pPosition->PrePosition.isValid())
				{
					thePosition.PrePosition = pPosition->PrePosition;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's PrePosition is invalid for %s", pPosition->PrePosition.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PrePosition");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("PrePosition"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key PrePosition is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->Position.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("Position"))
					thePosition.Position.clear();
			}
			else
			{
				if (pPosition->Position.isValid())
				{
					thePosition.Position = pPosition->Position;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Position is invalid for %s", pPosition->Position.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Position");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("Position"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key Position is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->LongFrozen.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("LongFrozen"))
					thePosition.LongFrozen.clear();
			}
			else
			{
				if (pPosition->LongFrozen.isValid())
				{
					thePosition.LongFrozen = pPosition->LongFrozen;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's LongFrozen is invalid for %s", pPosition->LongFrozen.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LongFrozen");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("LongFrozen"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key LongFrozen is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->ShortFrozen.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("ShortFrozen"))
					thePosition.ShortFrozen.clear();
			}
			else
			{
				if (pPosition->ShortFrozen.isValid())
				{
					thePosition.ShortFrozen = pPosition->ShortFrozen;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's ShortFrozen is invalid for %s", pPosition->ShortFrozen.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ShortFrozen");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("ShortFrozen"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key ShortFrozen is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->PreLongFrozen.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("PreLongFrozen"))
					thePosition.PreLongFrozen.clear();
			}
			else
			{
				if (pPosition->PreLongFrozen.isValid())
				{
					thePosition.PreLongFrozen = pPosition->PreLongFrozen;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's PreLongFrozen is invalid for %s", pPosition->PreLongFrozen.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PreLongFrozen");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("PreLongFrozen"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key PreLongFrozen is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->PreShortFrozen.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("PreShortFrozen"))
					thePosition.PreShortFrozen.clear();
			}
			else
			{
				if (pPosition->PreShortFrozen.isValid())
				{
					thePosition.PreShortFrozen = pPosition->PreShortFrozen;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's PreShortFrozen is invalid for %s", pPosition->PreShortFrozen.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PreShortFrozen");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("PreShortFrozen"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key PreShortFrozen is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->HighestPosition.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("HighestPosition"))
					thePosition.HighestPosition.clear();
			}
			else
			{
				if (pPosition->HighestPosition.isValid())
				{
					thePosition.HighestPosition = pPosition->HighestPosition;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's HighestPosition is invalid for %s", pPosition->HighestPosition.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"HighestPosition");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("HighestPosition"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key HighestPosition is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->ClosePosition.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("ClosePosition"))
					thePosition.ClosePosition.clear();
			}
			else
			{
				if (pPosition->ClosePosition.isValid())
				{
					thePosition.ClosePosition = pPosition->ClosePosition;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's ClosePosition is invalid for %s", pPosition->ClosePosition.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ClosePosition");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("ClosePosition"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key ClosePosition is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->PositionCost.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("PositionCost"))
					thePosition.PositionCost.clear();
			}
			else
			{
				if (pPosition->PositionCost.isValid())
				{
					thePosition.PositionCost = pPosition->PositionCost;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's PositionCost is invalid for %s", pPosition->PositionCost.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionCost");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("PositionCost"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key PositionCost is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->CostPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("CostPrice"))
					thePosition.CostPrice.clear();
			}
			else
			{
				if (pPosition->CostPrice.isValid())
				{
					thePosition.CostPrice = pPosition->CostPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's CostPrice is invalid for %s", pPosition->CostPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CostPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("CostPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key CostPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->UseMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("UseMargin"))
					thePosition.UseMargin.clear();
			}
			else
			{
				if (pPosition->UseMargin.isValid())
				{
					thePosition.UseMargin = pPosition->UseMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's UseMargin is invalid for %s", pPosition->UseMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UseMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("UseMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key UseMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->FrozenMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("FrozenMargin"))
					thePosition.FrozenMargin.clear();
			}
			else
			{
				if (pPosition->FrozenMargin.isValid())
				{
					thePosition.FrozenMargin = pPosition->FrozenMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's FrozenMargin is invalid for %s", pPosition->FrozenMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrozenMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("FrozenMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key FrozenMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->LongFrozenMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("LongFrozenMargin"))
					thePosition.LongFrozenMargin.clear();
			}
			else
			{
				if (pPosition->LongFrozenMargin.isValid())
				{
					thePosition.LongFrozenMargin = pPosition->LongFrozenMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's LongFrozenMargin is invalid for %s", pPosition->LongFrozenMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LongFrozenMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("LongFrozenMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key LongFrozenMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->ShortFrozenMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("ShortFrozenMargin"))
					thePosition.ShortFrozenMargin.clear();
			}
			else
			{
				if (pPosition->ShortFrozenMargin.isValid())
				{
					thePosition.ShortFrozenMargin = pPosition->ShortFrozenMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's ShortFrozenMargin is invalid for %s", pPosition->ShortFrozenMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ShortFrozenMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("ShortFrozenMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key ShortFrozenMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->CloseProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("CloseProfit"))
					thePosition.CloseProfit.clear();
			}
			else
			{
				if (pPosition->CloseProfit.isValid())
				{
					thePosition.CloseProfit = pPosition->CloseProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's CloseProfit is invalid for %s", pPosition->CloseProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("CloseProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key CloseProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->TotalPositionCost.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("TotalPositionCost"))
					thePosition.TotalPositionCost.clear();
			}
			else
			{
				if (pPosition->TotalPositionCost.isValid())
				{
					thePosition.TotalPositionCost = pPosition->TotalPositionCost;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's TotalPositionCost is invalid for %s", pPosition->TotalPositionCost.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TotalPositionCost");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("TotalPositionCost"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key TotalPositionCost is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->TotalCloseProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("TotalCloseProfit"))
					thePosition.TotalCloseProfit.clear();
			}
			else
			{
				if (pPosition->TotalCloseProfit.isValid())
				{
					thePosition.TotalCloseProfit = pPosition->TotalCloseProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's TotalCloseProfit is invalid for %s", pPosition->TotalCloseProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TotalCloseProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("TotalCloseProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key TotalCloseProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->OpenPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("OpenPrice"))
					thePosition.OpenPrice.clear();
			}
			else
			{
				if (pPosition->OpenPrice.isValid())
				{
					thePosition.OpenPrice = pPosition->OpenPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's OpenPrice is invalid for %s", pPosition->OpenPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("OpenPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key OpenPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->ClosePrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("ClosePrice"))
					thePosition.ClosePrice.clear();
			}
			else
			{
				if (pPosition->ClosePrice.isValid())
				{
					thePosition.ClosePrice = pPosition->ClosePrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's ClosePrice is invalid for %s", pPosition->ClosePrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ClosePrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("ClosePrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key ClosePrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->TradeFee.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("TradeFee"))
					thePosition.TradeFee.clear();
			}
			else
			{
				if (pPosition->TradeFee.isValid())
				{
					thePosition.TradeFee = pPosition->TradeFee;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's TradeFee is invalid for %s", pPosition->TradeFee.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradeFee");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("TradeFee"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key TradeFee is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->PositionFee.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("PositionFee"))
					thePosition.PositionFee.clear();
			}
			else
			{
				if (pPosition->PositionFee.isValid())
				{
					thePosition.PositionFee = pPosition->PositionFee;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's PositionFee is invalid for %s", pPosition->PositionFee.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionFee");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("PositionFee"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key PositionFee is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->Leverage.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("Leverage"))
					thePosition.Leverage.clear();
			}
			else
			{
				if (pPosition->Leverage.isValid())
				{
					thePosition.Leverage = pPosition->Leverage;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Leverage is invalid for %s", pPosition->Leverage.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Leverage");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("Leverage"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key Leverage is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->AccountID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("AccountID"))
					thePosition.AccountID.clear();
			}
			else
			{
				if (pPosition->AccountID.isValid())
				{
					thePosition.AccountID = pPosition->AccountID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's AccountID is invalid for %s", pPosition->AccountID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AccountID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("AccountID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key AccountID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->PriceCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("PriceCurrency"))
					thePosition.PriceCurrency.clear();
			}
			else
			{
				if (pPosition->PriceCurrency.isValid())
				{
					thePosition.PriceCurrency = pPosition->PriceCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's PriceCurrency is invalid for %s", pPosition->PriceCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PriceCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("PriceCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key PriceCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->ClearCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("ClearCurrency"))
					thePosition.ClearCurrency.clear();
			}
			else
			{
				if (pPosition->ClearCurrency.isValid())
				{
					thePosition.ClearCurrency = pPosition->ClearCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's ClearCurrency is invalid for %s", pPosition->ClearCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ClearCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("ClearCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key ClearCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->SettlementGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("SettlementGroup"))
					thePosition.SettlementGroup.clear();
			}
			else
			{
				if (pPosition->SettlementGroup.isValid())
				{
					thePosition.SettlementGroup = pPosition->SettlementGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's SettlementGroup is invalid for %s", pPosition->SettlementGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SettlementGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("SettlementGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key SettlementGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->IsCrossMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("IsCrossMargin"))
					thePosition.IsCrossMargin.clear();
			}
			else
			{
				if (pPosition->IsCrossMargin.isValid())
				{
					thePosition.IsCrossMargin = pPosition->IsCrossMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's IsCrossMargin is invalid for %s", pPosition->IsCrossMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"IsCrossMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("IsCrossMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key IsCrossMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->CloseOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("CloseOrderID"))
					thePosition.CloseOrderID.clear();
			}
			else
			{
				if (pPosition->CloseOrderID.isValid())
				{
					thePosition.CloseOrderID = pPosition->CloseOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's CloseOrderID is invalid for %s", pPosition->CloseOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("CloseOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key CloseOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->SLTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("SLTriggerPrice"))
					thePosition.SLTriggerPrice.clear();
			}
			else
			{
				if (pPosition->SLTriggerPrice.isValid())
				{
					thePosition.SLTriggerPrice = pPosition->SLTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's SLTriggerPrice is invalid for %s", pPosition->SLTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SLTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("SLTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key SLTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->TPTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("TPTriggerPrice"))
					thePosition.TPTriggerPrice.clear();
			}
			else
			{
				if (pPosition->TPTriggerPrice.isValid())
				{
					thePosition.TPTriggerPrice = pPosition->TPTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's TPTriggerPrice is invalid for %s", pPosition->TPTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TPTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("TPTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key TPTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->BeginTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("BeginTime"))
					thePosition.BeginTime.clear();
			}
			else
			{
				if (pPosition->BeginTime.isValid())
				{
					thePosition.BeginTime = pPosition->BeginTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's BeginTime is invalid for %s", pPosition->BeginTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BeginTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("BeginTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key BeginTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("InsertTime"))
					thePosition.InsertTime.clear();
			}
			else
			{
				if (pPosition->InsertTime.isValid())
				{
					thePosition.InsertTime = pPosition->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's InsertTime is invalid for %s", pPosition->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->LastOpenTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("LastOpenTime"))
					thePosition.LastOpenTime.clear();
			}
			else
			{
				if (pPosition->LastOpenTime.isValid())
				{
					thePosition.LastOpenTime = pPosition->LastOpenTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's LastOpenTime is invalid for %s", pPosition->LastOpenTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LastOpenTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("LastOpenTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key LastOpenTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("UpdateTime"))
					thePosition.UpdateTime.clear();
			}
			else
			{
				if (pPosition->UpdateTime.isValid())
				{
					thePosition.UpdateTime = pPosition->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's UpdateTime is invalid for %s", pPosition->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->BusinessNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("BusinessNo"))
					thePosition.BusinessNo.clear();
			}
			else
			{
				if (pPosition->BusinessNo.isValid())
				{
					thePosition.BusinessNo = pPosition->BusinessNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's BusinessNo is invalid for %s", pPosition->BusinessNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("BusinessNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key BusinessNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->IsAutoAddMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("IsAutoAddMargin"))
					thePosition.IsAutoAddMargin.clear();
			}
			else
			{
				if (pPosition->IsAutoAddMargin.isValid())
				{
					thePosition.IsAutoAddMargin = pPosition->IsAutoAddMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's IsAutoAddMargin is invalid for %s", pPosition->IsAutoAddMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"IsAutoAddMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("IsAutoAddMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key IsAutoAddMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->Frequency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("Frequency"))
					thePosition.Frequency.clear();
			}
			else
			{
				if (pPosition->Frequency.isValid())
				{
					thePosition.Frequency = pPosition->Frequency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Frequency is invalid for %s", pPosition->Frequency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Frequency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("Frequency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key Frequency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->MaintMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("MaintMargin"))
					thePosition.MaintMargin.clear();
			}
			else
			{
				if (pPosition->MaintMargin.isValid())
				{
					thePosition.MaintMargin = pPosition->MaintMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's MaintMargin is invalid for %s", pPosition->MaintMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MaintMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("MaintMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key MaintMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->UnRealProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("UnRealProfit"))
					thePosition.UnRealProfit.clear();
			}
			else
			{
				if (pPosition->UnRealProfit.isValid())
				{
					thePosition.UnRealProfit = pPosition->UnRealProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's UnRealProfit is invalid for %s", pPosition->UnRealProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UnRealProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("UnRealProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key UnRealProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->LiquidPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("LiquidPrice"))
					thePosition.LiquidPrice.clear();
			}
			else
			{
				if (pPosition->LiquidPrice.isValid())
				{
					thePosition.LiquidPrice = pPosition->LiquidPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's LiquidPrice is invalid for %s", pPosition->LiquidPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LiquidPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("LiquidPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key LiquidPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->CreateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("CreateTime"))
					thePosition.CreateTime.clear();
			}
			else
			{
				if (pPosition->CreateTime.isValid())
				{
					thePosition.CreateTime = pPosition->CreateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's CreateTime is invalid for %s", pPosition->CreateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CreateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("CreateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key CreateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->CopyMemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("CopyMemberID"))
					thePosition.CopyMemberID.clear();
			}
			else
			{
				if (pPosition->CopyMemberID.isValid())
				{
					thePosition.CopyMemberID = pPosition->CopyMemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's CopyMemberID is invalid for %s", pPosition->CopyMemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyMemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("CopyMemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key CopyMemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->CopyProfitRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("CopyProfitRate"))
					thePosition.CopyProfitRate.clear();
			}
			else
			{
				if (pPosition->CopyProfitRate.isValid())
				{
					thePosition.CopyProfitRate = pPosition->CopyProfitRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's CopyProfitRate is invalid for %s", pPosition->CopyProfitRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyProfitRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("CopyProfitRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key CopyProfitRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->CopyProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("CopyProfit"))
					thePosition.CopyProfit.clear();
			}
			else
			{
				if (pPosition->CopyProfit.isValid())
				{
					thePosition.CopyProfit = pPosition->CopyProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's CopyProfit is invalid for %s", pPosition->CopyProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("CopyProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key CopyProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->FirstTradeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("FirstTradeID"))
					thePosition.FirstTradeID.clear();
			}
			else
			{
				if (pPosition->FirstTradeID.isValid())
				{
					thePosition.FirstTradeID = pPosition->FirstTradeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's FirstTradeID is invalid for %s", pPosition->FirstTradeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FirstTradeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("FirstTradeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key FirstTradeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->LastTradeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("LastTradeID"))
					thePosition.LastTradeID.clear();
			}
			else
			{
				if (pPosition->LastTradeID.isValid())
				{
					thePosition.LastTradeID = pPosition->LastTradeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's LastTradeID is invalid for %s", pPosition->LastTradeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LastTradeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("LastTradeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key LastTradeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->BusinessType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("BusinessType"))
					thePosition.BusinessType.clear();
			}
			else
			{
				if (pPosition->BusinessType.isValid())
				{
					thePosition.BusinessType = pPosition->BusinessType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's BusinessType is invalid for %s", pPosition->BusinessType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("BusinessType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key BusinessType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->BusinessValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("BusinessValue"))
					thePosition.BusinessValue.clear();
			}
			else
			{
				if (pPosition->BusinessValue.isValid())
				{
					thePosition.BusinessValue = pPosition->BusinessValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's BusinessValue is invalid for %s", pPosition->BusinessValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("BusinessValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key BusinessValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->Reserve.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("Reserve"))
					thePosition.Reserve.clear();
			}
			else
			{
				if (pPosition->Reserve.isValid())
				{
					thePosition.Reserve = pPosition->Reserve;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Reserve is invalid for %s", pPosition->Reserve.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Reserve");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("Reserve"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key Reserve is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->ReserveProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("ReserveProfit"))
					thePosition.ReserveProfit.clear();
			}
			else
			{
				if (pPosition->ReserveProfit.isValid())
				{
					thePosition.ReserveProfit = pPosition->ReserveProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's ReserveProfit is invalid for %s", pPosition->ReserveProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ReserveProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("ReserveProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key ReserveProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPosition->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionFactory->isPK("Remark"))
					thePosition.Remark.clear();
			}
			else
			{
				if (pPosition->Remark.isValid())
				{
					thePosition.Remark = pPosition->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Remark is invalid for %s", pPosition->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Position's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		thePosition.m_pMDB = m_pMDB;
		thePosition.linkInstrument();
		thePosition.m_pMDB = m_pMDB;
		thePosition.linkAccount();
		m_pMDB->m_PositionFactory->addOrUpdate(pOldPosition, &thePosition, pTransaction);
		CopyPositionEntity(pPosition,&thePosition);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_PositionFactory->removeByPK(pPosition, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		m_pMDB->m_OrderFactory->linkAllPosition();
		return true;
	}
	case TID_SendQryOne:
	{
		CPosition *pOldPosition = m_pMDB->m_PositionFactory->findByPK(pPosition);
		if (pOldPosition == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyPositionEntity(pPosition,pOldPosition);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncMarketDataImpl(int nTid, CMarketDataField *pMarketData, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CMarketData *pOldMarketData = m_pMDB->m_MarketDataFactory->findByPK(pMarketData);
		if(pOldMarketData != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableMarketData theMarketData;
		CopyMarketDataEntity(&theMarketData,pMarketData);
		m_pMDB->m_MarketDataFactory->add(&theMarketData, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableMarketData theMarketData;
		CMarketData *pOldMarketData = m_pMDB->m_MarketDataFactory->findByPK(pMarketData);
		if(pOldMarketData == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theMarketData.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_MarketDataFactory->retrieve(pOldMarketData, &theMarketData);
		}

		if (!pMarketData->ExchangeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("ExchangeID"))
					theMarketData.ExchangeID.clear();
			}
			else
			{
				if (pMarketData->ExchangeID.isValid())
				{
					theMarketData.ExchangeID = pMarketData->ExchangeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's ExchangeID is invalid for %s", pMarketData->ExchangeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExchangeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("ExchangeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key ExchangeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("InstrumentID"))
					theMarketData.InstrumentID.clear();
			}
			else
			{
				if (pMarketData->InstrumentID.isValid())
				{
					theMarketData.InstrumentID = pMarketData->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's InstrumentID is invalid for %s", pMarketData->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->ProductGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("ProductGroup"))
					theMarketData.ProductGroup.clear();
			}
			else
			{
				if (pMarketData->ProductGroup.isValid())
				{
					theMarketData.ProductGroup = pMarketData->ProductGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's ProductGroup is invalid for %s", pMarketData->ProductGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ProductGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("ProductGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key ProductGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("UpdateTime"))
					theMarketData.UpdateTime.clear();
			}
			else
			{
				if (pMarketData->UpdateTime.isValid())
				{
					theMarketData.UpdateTime = pMarketData->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's UpdateTime is invalid for %s", pMarketData->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->PreClosePrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("PreClosePrice"))
					theMarketData.PreClosePrice.clear();
			}
			else
			{
				if (pMarketData->PreClosePrice.isValid())
				{
					theMarketData.PreClosePrice = pMarketData->PreClosePrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's PreClosePrice is invalid for %s", pMarketData->PreClosePrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PreClosePrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("PreClosePrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key PreClosePrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->PreSettlementPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("PreSettlementPrice"))
					theMarketData.PreSettlementPrice.clear();
			}
			else
			{
				if (pMarketData->PreSettlementPrice.isValid())
				{
					theMarketData.PreSettlementPrice = pMarketData->PreSettlementPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's PreSettlementPrice is invalid for %s", pMarketData->PreSettlementPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PreSettlementPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("PreSettlementPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key PreSettlementPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->PreOpenInterest.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("PreOpenInterest"))
					theMarketData.PreOpenInterest.clear();
			}
			else
			{
				if (pMarketData->PreOpenInterest.isValid())
				{
					theMarketData.PreOpenInterest = pMarketData->PreOpenInterest;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's PreOpenInterest is invalid for %s", pMarketData->PreOpenInterest.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PreOpenInterest");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("PreOpenInterest"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key PreOpenInterest is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->PrePositionFeeRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("PrePositionFeeRate"))
					theMarketData.PrePositionFeeRate.clear();
			}
			else
			{
				if (pMarketData->PrePositionFeeRate.isValid())
				{
					theMarketData.PrePositionFeeRate = pMarketData->PrePositionFeeRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's PrePositionFeeRate is invalid for %s", pMarketData->PrePositionFeeRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PrePositionFeeRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("PrePositionFeeRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key PrePositionFeeRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->ClosePrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("ClosePrice"))
					theMarketData.ClosePrice.clear();
			}
			else
			{
				if (pMarketData->ClosePrice.isValid())
				{
					theMarketData.ClosePrice = pMarketData->ClosePrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's ClosePrice is invalid for %s", pMarketData->ClosePrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ClosePrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("ClosePrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key ClosePrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->PositionFeeTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("PositionFeeTime"))
					theMarketData.PositionFeeTime.clear();
			}
			else
			{
				if (pMarketData->PositionFeeTime.isValid())
				{
					theMarketData.PositionFeeTime = pMarketData->PositionFeeTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's PositionFeeTime is invalid for %s", pMarketData->PositionFeeTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionFeeTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("PositionFeeTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key PositionFeeTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->SettlementTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("SettlementTime"))
					theMarketData.SettlementTime.clear();
			}
			else
			{
				if (pMarketData->SettlementTime.isValid())
				{
					theMarketData.SettlementTime = pMarketData->SettlementTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's SettlementTime is invalid for %s", pMarketData->SettlementTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SettlementTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("SettlementTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key SettlementTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->UpperLimitPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("UpperLimitPrice"))
					theMarketData.UpperLimitPrice.clear();
			}
			else
			{
				if (pMarketData->UpperLimitPrice.isValid())
				{
					theMarketData.UpperLimitPrice = pMarketData->UpperLimitPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's UpperLimitPrice is invalid for %s", pMarketData->UpperLimitPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpperLimitPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("UpperLimitPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key UpperLimitPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->LowerLimitPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("LowerLimitPrice"))
					theMarketData.LowerLimitPrice.clear();
			}
			else
			{
				if (pMarketData->LowerLimitPrice.isValid())
				{
					theMarketData.LowerLimitPrice = pMarketData->LowerLimitPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's LowerLimitPrice is invalid for %s", pMarketData->LowerLimitPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LowerLimitPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("LowerLimitPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key LowerLimitPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->UnderlyingPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("UnderlyingPrice"))
					theMarketData.UnderlyingPrice.clear();
			}
			else
			{
				if (pMarketData->UnderlyingPrice.isValid())
				{
					theMarketData.UnderlyingPrice = pMarketData->UnderlyingPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's UnderlyingPrice is invalid for %s", pMarketData->UnderlyingPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UnderlyingPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("UnderlyingPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key UnderlyingPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->MarkedPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("MarkedPrice"))
					theMarketData.MarkedPrice.clear();
			}
			else
			{
				if (pMarketData->MarkedPrice.isValid())
				{
					theMarketData.MarkedPrice = pMarketData->MarkedPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's MarkedPrice is invalid for %s", pMarketData->MarkedPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MarkedPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("MarkedPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key MarkedPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->PositionFeeRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("PositionFeeRate"))
					theMarketData.PositionFeeRate.clear();
			}
			else
			{
				if (pMarketData->PositionFeeRate.isValid())
				{
					theMarketData.PositionFeeRate = pMarketData->PositionFeeRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's PositionFeeRate is invalid for %s", pMarketData->PositionFeeRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionFeeRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("PositionFeeRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key PositionFeeRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->HighestPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("HighestPrice"))
					theMarketData.HighestPrice.clear();
			}
			else
			{
				if (pMarketData->HighestPrice.isValid())
				{
					theMarketData.HighestPrice = pMarketData->HighestPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's HighestPrice is invalid for %s", pMarketData->HighestPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"HighestPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("HighestPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key HighestPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->LowestPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("LowestPrice"))
					theMarketData.LowestPrice.clear();
			}
			else
			{
				if (pMarketData->LowestPrice.isValid())
				{
					theMarketData.LowestPrice = pMarketData->LowestPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's LowestPrice is invalid for %s", pMarketData->LowestPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LowestPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("LowestPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key LowestPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->LastPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("LastPrice"))
					theMarketData.LastPrice.clear();
			}
			else
			{
				if (pMarketData->LastPrice.isValid())
				{
					theMarketData.LastPrice = pMarketData->LastPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's LastPrice is invalid for %s", pMarketData->LastPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LastPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("LastPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key LastPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->Volume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("Volume"))
					theMarketData.Volume.clear();
			}
			else
			{
				if (pMarketData->Volume.isValid())
				{
					theMarketData.Volume = pMarketData->Volume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Volume is invalid for %s", pMarketData->Volume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Volume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("Volume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key Volume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->Turnover.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("Turnover"))
					theMarketData.Turnover.clear();
			}
			else
			{
				if (pMarketData->Turnover.isValid())
				{
					theMarketData.Turnover = pMarketData->Turnover;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Turnover is invalid for %s", pMarketData->Turnover.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Turnover");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("Turnover"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key Turnover is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->OpenInterest.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("OpenInterest"))
					theMarketData.OpenInterest.clear();
			}
			else
			{
				if (pMarketData->OpenInterest.isValid())
				{
					theMarketData.OpenInterest = pMarketData->OpenInterest;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's OpenInterest is invalid for %s", pMarketData->OpenInterest.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenInterest");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("OpenInterest"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key OpenInterest is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->SettlementPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("SettlementPrice"))
					theMarketData.SettlementPrice.clear();
			}
			else
			{
				if (pMarketData->SettlementPrice.isValid())
				{
					theMarketData.SettlementPrice = pMarketData->SettlementPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's SettlementPrice is invalid for %s", pMarketData->SettlementPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SettlementPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("SettlementPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key SettlementPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->OpenPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("OpenPrice"))
					theMarketData.OpenPrice.clear();
			}
			else
			{
				if (pMarketData->OpenPrice.isValid())
				{
					theMarketData.OpenPrice = pMarketData->OpenPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's OpenPrice is invalid for %s", pMarketData->OpenPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("OpenPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key OpenPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->IsPubMarketData.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("IsPubMarketData"))
					theMarketData.IsPubMarketData.clear();
			}
			else
			{
				if (pMarketData->IsPubMarketData.isValid())
				{
					theMarketData.IsPubMarketData = pMarketData->IsPubMarketData;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's IsPubMarketData is invalid for %s", pMarketData->IsPubMarketData.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"IsPubMarketData");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("IsPubMarketData"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key IsPubMarketData is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->InstrumentStatus.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("InstrumentStatus"))
					theMarketData.InstrumentStatus.clear();
			}
			else
			{
				if (pMarketData->InstrumentStatus.isValid())
				{
					theMarketData.InstrumentStatus = pMarketData->InstrumentStatus;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's InstrumentStatus is invalid for %s", pMarketData->InstrumentStatus.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentStatus");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("InstrumentStatus"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key InstrumentStatus is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("Remark"))
					theMarketData.Remark.clear();
			}
			else
			{
				if (pMarketData->Remark.isValid())
				{
					theMarketData.Remark = pMarketData->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Remark is invalid for %s", pMarketData->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->BusinessNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("BusinessNo"))
					theMarketData.BusinessNo.clear();
			}
			else
			{
				if (pMarketData->BusinessNo.isValid())
				{
					theMarketData.BusinessNo = pMarketData->BusinessNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's BusinessNo is invalid for %s", pMarketData->BusinessNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("BusinessNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key BusinessNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->MarkedPriceTotal.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("MarkedPriceTotal"))
					theMarketData.MarkedPriceTotal.clear();
			}
			else
			{
				if (pMarketData->MarkedPriceTotal.isValid())
				{
					theMarketData.MarkedPriceTotal = pMarketData->MarkedPriceTotal;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's MarkedPriceTotal is invalid for %s", pMarketData->MarkedPriceTotal.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MarkedPriceTotal");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("MarkedPriceTotal"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key MarkedPriceTotal is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->MarkedPriceWeight.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("MarkedPriceWeight"))
					theMarketData.MarkedPriceWeight.clear();
			}
			else
			{
				if (pMarketData->MarkedPriceWeight.isValid())
				{
					theMarketData.MarkedPriceWeight = pMarketData->MarkedPriceWeight;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's MarkedPriceWeight is invalid for %s", pMarketData->MarkedPriceWeight.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MarkedPriceWeight");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("MarkedPriceWeight"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key MarkedPriceWeight is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->L25UpperPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("L25UpperPrice"))
					theMarketData.L25UpperPrice.clear();
			}
			else
			{
				if (pMarketData->L25UpperPrice.isValid())
				{
					theMarketData.L25UpperPrice = pMarketData->L25UpperPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's L25UpperPrice is invalid for %s", pMarketData->L25UpperPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"L25UpperPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("L25UpperPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key L25UpperPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->L25LowerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("L25LowerPrice"))
					theMarketData.L25LowerPrice.clear();
			}
			else
			{
				if (pMarketData->L25LowerPrice.isValid())
				{
					theMarketData.L25LowerPrice = pMarketData->L25LowerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's L25LowerPrice is invalid for %s", pMarketData->L25LowerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"L25LowerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("L25LowerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key L25LowerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->L25UpperPrice1.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("L25UpperPrice1"))
					theMarketData.L25UpperPrice1.clear();
			}
			else
			{
				if (pMarketData->L25UpperPrice1.isValid())
				{
					theMarketData.L25UpperPrice1 = pMarketData->L25UpperPrice1;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's L25UpperPrice1 is invalid for %s", pMarketData->L25UpperPrice1.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"L25UpperPrice1");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("L25UpperPrice1"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key L25UpperPrice1 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->L25LowerPrice1.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("L25LowerPrice1"))
					theMarketData.L25LowerPrice1.clear();
			}
			else
			{
				if (pMarketData->L25LowerPrice1.isValid())
				{
					theMarketData.L25LowerPrice1 = pMarketData->L25LowerPrice1;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's L25LowerPrice1 is invalid for %s", pMarketData->L25LowerPrice1.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"L25LowerPrice1");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("L25LowerPrice1"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key L25LowerPrice1 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->L25UpperPrice2.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("L25UpperPrice2"))
					theMarketData.L25UpperPrice2.clear();
			}
			else
			{
				if (pMarketData->L25UpperPrice2.isValid())
				{
					theMarketData.L25UpperPrice2 = pMarketData->L25UpperPrice2;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's L25UpperPrice2 is invalid for %s", pMarketData->L25UpperPrice2.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"L25UpperPrice2");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("L25UpperPrice2"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key L25UpperPrice2 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->L25LowerPrice2.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("L25LowerPrice2"))
					theMarketData.L25LowerPrice2.clear();
			}
			else
			{
				if (pMarketData->L25LowerPrice2.isValid())
				{
					theMarketData.L25LowerPrice2 = pMarketData->L25LowerPrice2;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's L25LowerPrice2 is invalid for %s", pMarketData->L25LowerPrice2.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"L25LowerPrice2");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("L25LowerPrice2"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key L25LowerPrice2 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->BidPrice1.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("BidPrice1"))
					theMarketData.BidPrice1.clear();
			}
			else
			{
				if (pMarketData->BidPrice1.isValid())
				{
					theMarketData.BidPrice1 = pMarketData->BidPrice1;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's BidPrice1 is invalid for %s", pMarketData->BidPrice1.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BidPrice1");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("BidPrice1"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key BidPrice1 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->BidVolume1.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("BidVolume1"))
					theMarketData.BidVolume1.clear();
			}
			else
			{
				if (pMarketData->BidVolume1.isValid())
				{
					theMarketData.BidVolume1 = pMarketData->BidVolume1;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's BidVolume1 is invalid for %s", pMarketData->BidVolume1.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BidVolume1");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("BidVolume1"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key BidVolume1 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->AskPrice1.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("AskPrice1"))
					theMarketData.AskPrice1.clear();
			}
			else
			{
				if (pMarketData->AskPrice1.isValid())
				{
					theMarketData.AskPrice1 = pMarketData->AskPrice1;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's AskPrice1 is invalid for %s", pMarketData->AskPrice1.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AskPrice1");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("AskPrice1"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key AskPrice1 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->AskVolume1.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("AskVolume1"))
					theMarketData.AskVolume1.clear();
			}
			else
			{
				if (pMarketData->AskVolume1.isValid())
				{
					theMarketData.AskVolume1 = pMarketData->AskVolume1;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's AskVolume1 is invalid for %s", pMarketData->AskVolume1.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AskVolume1");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("AskVolume1"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key AskVolume1 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->TheoryPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("TheoryPrice"))
					theMarketData.TheoryPrice.clear();
			}
			else
			{
				if (pMarketData->TheoryPrice.isValid())
				{
					theMarketData.TheoryPrice = pMarketData->TheoryPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's TheoryPrice is invalid for %s", pMarketData->TheoryPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TheoryPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("TheoryPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key TheoryPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->HighestPrice24.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("HighestPrice24"))
					theMarketData.HighestPrice24.clear();
			}
			else
			{
				if (pMarketData->HighestPrice24.isValid())
				{
					theMarketData.HighestPrice24 = pMarketData->HighestPrice24;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's HighestPrice24 is invalid for %s", pMarketData->HighestPrice24.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"HighestPrice24");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("HighestPrice24"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key HighestPrice24 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->LowestPrice24.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("LowestPrice24"))
					theMarketData.LowestPrice24.clear();
			}
			else
			{
				if (pMarketData->LowestPrice24.isValid())
				{
					theMarketData.LowestPrice24 = pMarketData->LowestPrice24;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's LowestPrice24 is invalid for %s", pMarketData->LowestPrice24.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LowestPrice24");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("LowestPrice24"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key LowestPrice24 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->Volume24.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("Volume24"))
					theMarketData.Volume24.clear();
			}
			else
			{
				if (pMarketData->Volume24.isValid())
				{
					theMarketData.Volume24 = pMarketData->Volume24;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Volume24 is invalid for %s", pMarketData->Volume24.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Volume24");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("Volume24"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key Volume24 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->Turnover24.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("Turnover24"))
					theMarketData.Turnover24.clear();
			}
			else
			{
				if (pMarketData->Turnover24.isValid())
				{
					theMarketData.Turnover24 = pMarketData->Turnover24;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Turnover24 is invalid for %s", pMarketData->Turnover24.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Turnover24");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("Turnover24"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key Turnover24 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->OpenPrice24.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("OpenPrice24"))
					theMarketData.OpenPrice24.clear();
			}
			else
			{
				if (pMarketData->OpenPrice24.isValid())
				{
					theMarketData.OpenPrice24 = pMarketData->OpenPrice24;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's OpenPrice24 is invalid for %s", pMarketData->OpenPrice24.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenPrice24");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("OpenPrice24"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key OpenPrice24 is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->Frequency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("Frequency"))
					theMarketData.Frequency.clear();
			}
			else
			{
				if (pMarketData->Frequency.isValid())
				{
					theMarketData.Frequency = pMarketData->Frequency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Frequency is invalid for %s", pMarketData->Frequency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Frequency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("Frequency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key Frequency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pMarketData->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_MarketDataFactory->isPK("InsertTime"))
					theMarketData.InsertTime.clear();
			}
			else
			{
				if (pMarketData->InsertTime.isValid())
				{
					theMarketData.InsertTime = pMarketData->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's InsertTime is invalid for %s", pMarketData->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_MarketDataFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_MarketData's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		theMarketData.m_pMDB = m_pMDB;
		theMarketData.linkInstrument();
		m_pMDB->m_MarketDataFactory->addOrUpdate(pOldMarketData, &theMarketData, pTransaction);
		CopyMarketDataEntity(pMarketData,&theMarketData);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_MarketDataFactory->removeByPK(pMarketData, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		m_pMDB->m_InstrumentFactory->linkAllMarketData();
		m_pMDB->m_CurrencyFactory->linkAllMarketData();
		m_pMDB->m_TriggerOrderFactory->linkAllMarketData();
		return true;
	}
	case TID_SendQryOne:
	{
		CMarketData *pOldMarketData = m_pMDB->m_MarketDataFactory->findByPK(pMarketData);
		if (pOldMarketData == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyMarketDataEntity(pMarketData,pOldMarketData);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncOrderImpl(int nTid, COrderField *pOrder, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		COrder *pOldOrder = m_pMDB->m_OrderFactory->findByPK(pOrder);
		if(pOldOrder != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableOrder theOrder;
		CopyOrderEntity(&theOrder,pOrder);
		m_pMDB->m_OrderFactory->add(&theOrder, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableOrder theOrder;
		COrder *pOldOrder = m_pMDB->m_OrderFactory->findByPK(pOrder);
		if(pOldOrder == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theOrder.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_OrderFactory->retrieve(pOldOrder, &theOrder);
		}

		if (!pOrder->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("MemberID"))
					theOrder.MemberID.clear();
			}
			else
			{
				if (pOrder->MemberID.isValid())
				{
					theOrder.MemberID = pOrder->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's MemberID is invalid for %s", pOrder->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->TradeUnitID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("TradeUnitID"))
					theOrder.TradeUnitID.clear();
			}
			else
			{
				if (pOrder->TradeUnitID.isValid())
				{
					theOrder.TradeUnitID = pOrder->TradeUnitID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's TradeUnitID is invalid for %s", pOrder->TradeUnitID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradeUnitID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("TradeUnitID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key TradeUnitID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->AccountID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("AccountID"))
					theOrder.AccountID.clear();
			}
			else
			{
				if (pOrder->AccountID.isValid())
				{
					theOrder.AccountID = pOrder->AccountID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's AccountID is invalid for %s", pOrder->AccountID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AccountID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("AccountID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key AccountID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->LocalID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("LocalID"))
					theOrder.LocalID.clear();
			}
			else
			{
				if (pOrder->LocalID.isValid())
				{
					theOrder.LocalID = pOrder->LocalID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's LocalID is invalid for %s", pOrder->LocalID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LocalID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("LocalID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key LocalID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->ExchangeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("ExchangeID"))
					theOrder.ExchangeID.clear();
			}
			else
			{
				if (pOrder->ExchangeID.isValid())
				{
					theOrder.ExchangeID = pOrder->ExchangeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's ExchangeID is invalid for %s", pOrder->ExchangeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExchangeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("ExchangeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key ExchangeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("InstrumentID"))
					theOrder.InstrumentID.clear();
			}
			else
			{
				if (pOrder->InstrumentID.isValid())
				{
					theOrder.InstrumentID = pOrder->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's InstrumentID is invalid for %s", pOrder->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->OrderPriceType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("OrderPriceType"))
					theOrder.OrderPriceType.clear();
			}
			else
			{
				if (pOrder->OrderPriceType.isValid())
				{
					theOrder.OrderPriceType = pOrder->OrderPriceType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's OrderPriceType is invalid for %s", pOrder->OrderPriceType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderPriceType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("OrderPriceType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key OrderPriceType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Direction.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Direction"))
					theOrder.Direction.clear();
			}
			else
			{
				if (pOrder->Direction.isValid())
				{
					theOrder.Direction = pOrder->Direction;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Direction is invalid for %s", pOrder->Direction.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Direction");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Direction"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Direction is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->OffsetFlag.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("OffsetFlag"))
					theOrder.OffsetFlag.clear();
			}
			else
			{
				if (pOrder->OffsetFlag.isValid())
				{
					theOrder.OffsetFlag = pOrder->OffsetFlag;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's OffsetFlag is invalid for %s", pOrder->OffsetFlag.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OffsetFlag");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("OffsetFlag"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key OffsetFlag is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Price.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Price"))
					theOrder.Price.clear();
			}
			else
			{
				if (pOrder->Price.isValid())
				{
					theOrder.Price = pOrder->Price;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Price is invalid for %s", pOrder->Price.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Price");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Price"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Price is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Volume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Volume"))
					theOrder.Volume.clear();
			}
			else
			{
				if (pOrder->Volume.isValid())
				{
					theOrder.Volume = pOrder->Volume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Volume is invalid for %s", pOrder->Volume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Volume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Volume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Volume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->VolumeDisplay.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("VolumeDisplay"))
					theOrder.VolumeDisplay.clear();
			}
			else
			{
				if (pOrder->VolumeDisplay.isValid())
				{
					theOrder.VolumeDisplay = pOrder->VolumeDisplay;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's VolumeDisplay is invalid for %s", pOrder->VolumeDisplay.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeDisplay");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("VolumeDisplay"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key VolumeDisplay is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->VolumeMode.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("VolumeMode"))
					theOrder.VolumeMode.clear();
			}
			else
			{
				if (pOrder->VolumeMode.isValid())
				{
					theOrder.VolumeMode = pOrder->VolumeMode;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's VolumeMode is invalid for %s", pOrder->VolumeMode.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeMode");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("VolumeMode"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key VolumeMode is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Cost.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Cost"))
					theOrder.Cost.clear();
			}
			else
			{
				if (pOrder->Cost.isValid())
				{
					theOrder.Cost = pOrder->Cost;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Cost is invalid for %s", pOrder->Cost.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Cost");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Cost"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Cost is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->OrderType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("OrderType"))
					theOrder.OrderType.clear();
			}
			else
			{
				if (pOrder->OrderType.isValid())
				{
					theOrder.OrderType = pOrder->OrderType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's OrderType is invalid for %s", pOrder->OrderType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("OrderType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key OrderType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->GTDTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("GTDTime"))
					theOrder.GTDTime.clear();
			}
			else
			{
				if (pOrder->GTDTime.isValid())
				{
					theOrder.GTDTime = pOrder->GTDTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's GTDTime is invalid for %s", pOrder->GTDTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"GTDTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("GTDTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key GTDTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->MinVolume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("MinVolume"))
					theOrder.MinVolume.clear();
			}
			else
			{
				if (pOrder->MinVolume.isValid())
				{
					theOrder.MinVolume = pOrder->MinVolume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's MinVolume is invalid for %s", pOrder->MinVolume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MinVolume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("MinVolume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key MinVolume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->BusinessType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("BusinessType"))
					theOrder.BusinessType.clear();
			}
			else
			{
				if (pOrder->BusinessType.isValid())
				{
					theOrder.BusinessType = pOrder->BusinessType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's BusinessType is invalid for %s", pOrder->BusinessType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("BusinessType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key BusinessType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->BusinessValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("BusinessValue"))
					theOrder.BusinessValue.clear();
			}
			else
			{
				if (pOrder->BusinessValue.isValid())
				{
					theOrder.BusinessValue = pOrder->BusinessValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's BusinessValue is invalid for %s", pOrder->BusinessValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("BusinessValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key BusinessValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->CloseOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("CloseOrderID"))
					theOrder.CloseOrderID.clear();
			}
			else
			{
				if (pOrder->CloseOrderID.isValid())
				{
					theOrder.CloseOrderID = pOrder->CloseOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's CloseOrderID is invalid for %s", pOrder->CloseOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("CloseOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key CloseOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->IsCrossMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("IsCrossMargin"))
					theOrder.IsCrossMargin.clear();
			}
			else
			{
				if (pOrder->IsCrossMargin.isValid())
				{
					theOrder.IsCrossMargin = pOrder->IsCrossMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's IsCrossMargin is invalid for %s", pOrder->IsCrossMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"IsCrossMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("IsCrossMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key IsCrossMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Remark"))
					theOrder.Remark.clear();
			}
			else
			{
				if (pOrder->Remark.isValid())
				{
					theOrder.Remark = pOrder->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Remark is invalid for %s", pOrder->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->OrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("OrderID"))
					theOrder.OrderID.clear();
			}
			else
			{
				if (pOrder->OrderID.isValid())
				{
					theOrder.OrderID = pOrder->OrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's OrderID is invalid for %s", pOrder->OrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("OrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key OrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->CopyMemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("CopyMemberID"))
					theOrder.CopyMemberID.clear();
			}
			else
			{
				if (pOrder->CopyMemberID.isValid())
				{
					theOrder.CopyMemberID = pOrder->CopyMemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's CopyMemberID is invalid for %s", pOrder->CopyMemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyMemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("CopyMemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key CopyMemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->CopyOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("CopyOrderID"))
					theOrder.CopyOrderID.clear();
			}
			else
			{
				if (pOrder->CopyOrderID.isValid())
				{
					theOrder.CopyOrderID = pOrder->CopyOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's CopyOrderID is invalid for %s", pOrder->CopyOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("CopyOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key CopyOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Leverage.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Leverage"))
					theOrder.Leverage.clear();
			}
			else
			{
				if (pOrder->Leverage.isValid())
				{
					theOrder.Leverage = pOrder->Leverage;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Leverage is invalid for %s", pOrder->Leverage.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Leverage");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Leverage"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Leverage is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->CopyProfitRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("CopyProfitRate"))
					theOrder.CopyProfitRate.clear();
			}
			else
			{
				if (pOrder->CopyProfitRate.isValid())
				{
					theOrder.CopyProfitRate = pOrder->CopyProfitRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's CopyProfitRate is invalid for %s", pOrder->CopyProfitRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyProfitRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("CopyProfitRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key CopyProfitRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->APPID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("APPID"))
					theOrder.APPID.clear();
			}
			else
			{
				if (pOrder->APPID.isValid())
				{
					theOrder.APPID = pOrder->APPID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's APPID is invalid for %s", pOrder->APPID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"APPID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("APPID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key APPID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->PositionID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("PositionID"))
					theOrder.PositionID.clear();
			}
			else
			{
				if (pOrder->PositionID.isValid())
				{
					theOrder.PositionID = pOrder->PositionID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's PositionID is invalid for %s", pOrder->PositionID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("PositionID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key PositionID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->TriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("TriggerPrice"))
					theOrder.TriggerPrice.clear();
			}
			else
			{
				if (pOrder->TriggerPrice.isValid())
				{
					theOrder.TriggerPrice = pOrder->TriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's TriggerPrice is invalid for %s", pOrder->TriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("TriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key TriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Reserve.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Reserve"))
					theOrder.Reserve.clear();
			}
			else
			{
				if (pOrder->Reserve.isValid())
				{
					theOrder.Reserve = pOrder->Reserve;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Reserve is invalid for %s", pOrder->Reserve.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Reserve");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Reserve"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Reserve is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->OrderStatus.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("OrderStatus"))
					theOrder.OrderStatus.clear();
			}
			else
			{
				if (pOrder->OrderStatus.isValid())
				{
					theOrder.OrderStatus = pOrder->OrderStatus;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's OrderStatus is invalid for %s", pOrder->OrderStatus.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderStatus");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("OrderStatus"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key OrderStatus is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->DeriveSource.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("DeriveSource"))
					theOrder.DeriveSource.clear();
			}
			else
			{
				if (pOrder->DeriveSource.isValid())
				{
					theOrder.DeriveSource = pOrder->DeriveSource;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's DeriveSource is invalid for %s", pOrder->DeriveSource.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"DeriveSource");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("DeriveSource"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key DeriveSource is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->DeriveDetail.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("DeriveDetail"))
					theOrder.DeriveDetail.clear();
			}
			else
			{
				if (pOrder->DeriveDetail.isValid())
				{
					theOrder.DeriveDetail = pOrder->DeriveDetail;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's DeriveDetail is invalid for %s", pOrder->DeriveDetail.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"DeriveDetail");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("DeriveDetail"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key DeriveDetail is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->VolumeTraded.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("VolumeTraded"))
					theOrder.VolumeTraded.clear();
			}
			else
			{
				if (pOrder->VolumeTraded.isValid())
				{
					theOrder.VolumeTraded = pOrder->VolumeTraded;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's VolumeTraded is invalid for %s", pOrder->VolumeTraded.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeTraded");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("VolumeTraded"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key VolumeTraded is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->VolumeRemain.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("VolumeRemain"))
					theOrder.VolumeRemain.clear();
			}
			else
			{
				if (pOrder->VolumeRemain.isValid())
				{
					theOrder.VolumeRemain = pOrder->VolumeRemain;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's VolumeRemain is invalid for %s", pOrder->VolumeRemain.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeRemain");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("VolumeRemain"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key VolumeRemain is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->VolumeCancled.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("VolumeCancled"))
					theOrder.VolumeCancled.clear();
			}
			else
			{
				if (pOrder->VolumeCancled.isValid())
				{
					theOrder.VolumeCancled = pOrder->VolumeCancled;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's VolumeCancled is invalid for %s", pOrder->VolumeCancled.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeCancled");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("VolumeCancled"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key VolumeCancled is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("InsertTime"))
					theOrder.InsertTime.clear();
			}
			else
			{
				if (pOrder->InsertTime.isValid())
				{
					theOrder.InsertTime = pOrder->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's InsertTime is invalid for %s", pOrder->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("UpdateTime"))
					theOrder.UpdateTime.clear();
			}
			else
			{
				if (pOrder->UpdateTime.isValid())
				{
					theOrder.UpdateTime = pOrder->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's UpdateTime is invalid for %s", pOrder->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Priority.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Priority"))
					theOrder.Priority.clear();
			}
			else
			{
				if (pOrder->Priority.isValid())
				{
					theOrder.Priority = pOrder->Priority;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Priority is invalid for %s", pOrder->Priority.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Priority");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Priority"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Priority is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->TimeSortNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("TimeSortNo"))
					theOrder.TimeSortNo.clear();
			}
			else
			{
				if (pOrder->TimeSortNo.isValid())
				{
					theOrder.TimeSortNo = pOrder->TimeSortNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's TimeSortNo is invalid for %s", pOrder->TimeSortNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TimeSortNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("TimeSortNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key TimeSortNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->FrontNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("FrontNo"))
					theOrder.FrontNo.clear();
			}
			else
			{
				if (pOrder->FrontNo.isValid())
				{
					theOrder.FrontNo = pOrder->FrontNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's FrontNo is invalid for %s", pOrder->FrontNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrontNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("FrontNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key FrontNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->PriceCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("PriceCurrency"))
					theOrder.PriceCurrency.clear();
			}
			else
			{
				if (pOrder->PriceCurrency.isValid())
				{
					theOrder.PriceCurrency = pOrder->PriceCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's PriceCurrency is invalid for %s", pOrder->PriceCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PriceCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("PriceCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key PriceCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->FeeCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("FeeCurrency"))
					theOrder.FeeCurrency.clear();
			}
			else
			{
				if (pOrder->FeeCurrency.isValid())
				{
					theOrder.FeeCurrency = pOrder->FeeCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's FeeCurrency is invalid for %s", pOrder->FeeCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FeeCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("FeeCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key FeeCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->ClearCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("ClearCurrency"))
					theOrder.ClearCurrency.clear();
			}
			else
			{
				if (pOrder->ClearCurrency.isValid())
				{
					theOrder.ClearCurrency = pOrder->ClearCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's ClearCurrency is invalid for %s", pOrder->ClearCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ClearCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("ClearCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key ClearCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->FrozenMoney.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("FrozenMoney"))
					theOrder.FrozenMoney.clear();
			}
			else
			{
				if (pOrder->FrozenMoney.isValid())
				{
					theOrder.FrozenMoney = pOrder->FrozenMoney;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's FrozenMoney is invalid for %s", pOrder->FrozenMoney.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrozenMoney");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("FrozenMoney"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key FrozenMoney is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->FrozenFee.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("FrozenFee"))
					theOrder.FrozenFee.clear();
			}
			else
			{
				if (pOrder->FrozenFee.isValid())
				{
					theOrder.FrozenFee = pOrder->FrozenFee;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's FrozenFee is invalid for %s", pOrder->FrozenFee.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrozenFee");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("FrozenFee"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key FrozenFee is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->FrozenMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("FrozenMargin"))
					theOrder.FrozenMargin.clear();
			}
			else
			{
				if (pOrder->FrozenMargin.isValid())
				{
					theOrder.FrozenMargin = pOrder->FrozenMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's FrozenMargin is invalid for %s", pOrder->FrozenMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrozenMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("FrozenMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key FrozenMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Fee.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Fee"))
					theOrder.Fee.clear();
			}
			else
			{
				if (pOrder->Fee.isValid())
				{
					theOrder.Fee = pOrder->Fee;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Fee is invalid for %s", pOrder->Fee.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Fee");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Fee"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Fee is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->CloseProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("CloseProfit"))
					theOrder.CloseProfit.clear();
			}
			else
			{
				if (pOrder->CloseProfit.isValid())
				{
					theOrder.CloseProfit = pOrder->CloseProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's CloseProfit is invalid for %s", pOrder->CloseProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("CloseProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key CloseProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Turnover.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Turnover"))
					theOrder.Turnover.clear();
			}
			else
			{
				if (pOrder->Turnover.isValid())
				{
					theOrder.Turnover = pOrder->Turnover;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Turnover is invalid for %s", pOrder->Turnover.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Turnover");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Turnover"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Turnover is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->RelatedOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("RelatedOrderID"))
					theOrder.RelatedOrderID.clear();
			}
			else
			{
				if (pOrder->RelatedOrderID.isValid())
				{
					theOrder.RelatedOrderID = pOrder->RelatedOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's RelatedOrderID is invalid for %s", pOrder->RelatedOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"RelatedOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("RelatedOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key RelatedOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->BusinessResult.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("BusinessResult"))
					theOrder.BusinessResult.clear();
			}
			else
			{
				if (pOrder->BusinessResult.isValid())
				{
					theOrder.BusinessResult = pOrder->BusinessResult;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's BusinessResult is invalid for %s", pOrder->BusinessResult.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessResult");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("BusinessResult"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key BusinessResult is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->BusinessNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("BusinessNo"))
					theOrder.BusinessNo.clear();
			}
			else
			{
				if (pOrder->BusinessNo.isValid())
				{
					theOrder.BusinessNo = pOrder->BusinessNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's BusinessNo is invalid for %s", pOrder->BusinessNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("BusinessNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key BusinessNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Tradable.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Tradable"))
					theOrder.Tradable.clear();
			}
			else
			{
				if (pOrder->Tradable.isValid())
				{
					theOrder.Tradable = pOrder->Tradable;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Tradable is invalid for %s", pOrder->Tradable.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Tradable");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Tradable"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Tradable is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->SettlementGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("SettlementGroup"))
					theOrder.SettlementGroup.clear();
			}
			else
			{
				if (pOrder->SettlementGroup.isValid())
				{
					theOrder.SettlementGroup = pOrder->SettlementGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's SettlementGroup is invalid for %s", pOrder->SettlementGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SettlementGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("SettlementGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key SettlementGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->PosiDirection.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("PosiDirection"))
					theOrder.PosiDirection.clear();
			}
			else
			{
				if (pOrder->PosiDirection.isValid())
				{
					theOrder.PosiDirection = pOrder->PosiDirection;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's PosiDirection is invalid for %s", pOrder->PosiDirection.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PosiDirection");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("PosiDirection"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key PosiDirection is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->TradePrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("TradePrice"))
					theOrder.TradePrice.clear();
			}
			else
			{
				if (pOrder->TradePrice.isValid())
				{
					theOrder.TradePrice = pOrder->TradePrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's TradePrice is invalid for %s", pOrder->TradePrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradePrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("TradePrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key TradePrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->OpenPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("OpenPrice"))
					theOrder.OpenPrice.clear();
			}
			else
			{
				if (pOrder->OpenPrice.isValid())
				{
					theOrder.OpenPrice = pOrder->OpenPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's OpenPrice is invalid for %s", pOrder->OpenPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("OpenPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key OpenPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->TriggerOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("TriggerOrderID"))
					theOrder.TriggerOrderID.clear();
			}
			else
			{
				if (pOrder->TriggerOrderID.isValid())
				{
					theOrder.TriggerOrderID = pOrder->TriggerOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's TriggerOrderID is invalid for %s", pOrder->TriggerOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("TriggerOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key TriggerOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->SLTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("SLTriggerPrice"))
					theOrder.SLTriggerPrice.clear();
			}
			else
			{
				if (pOrder->SLTriggerPrice.isValid())
				{
					theOrder.SLTriggerPrice = pOrder->SLTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's SLTriggerPrice is invalid for %s", pOrder->SLTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SLTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("SLTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key SLTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->TPTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("TPTriggerPrice"))
					theOrder.TPTriggerPrice.clear();
			}
			else
			{
				if (pOrder->TPTriggerPrice.isValid())
				{
					theOrder.TPTriggerPrice = pOrder->TPTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's TPTriggerPrice is invalid for %s", pOrder->TPTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TPTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("TPTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key TPTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->CopyProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("CopyProfit"))
					theOrder.CopyProfit.clear();
			}
			else
			{
				if (pOrder->CopyProfit.isValid())
				{
					theOrder.CopyProfit = pOrder->CopyProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's CopyProfit is invalid for %s", pOrder->CopyProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("CopyProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key CopyProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Position.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Position"))
					theOrder.Position.clear();
			}
			else
			{
				if (pOrder->Position.isValid())
				{
					theOrder.Position = pOrder->Position;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Position is invalid for %s", pOrder->Position.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Position");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Position"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Position is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->UserID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("UserID"))
					theOrder.UserID.clear();
			}
			else
			{
				if (pOrder->UserID.isValid())
				{
					theOrder.UserID = pOrder->UserID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's UserID is invalid for %s", pOrder->UserID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UserID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("UserID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key UserID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->LastPriceByInsert.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("LastPriceByInsert"))
					theOrder.LastPriceByInsert.clear();
			}
			else
			{
				if (pOrder->LastPriceByInsert.isValid())
				{
					theOrder.LastPriceByInsert = pOrder->LastPriceByInsert;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's LastPriceByInsert is invalid for %s", pOrder->LastPriceByInsert.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LastPriceByInsert");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("LastPriceByInsert"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key LastPriceByInsert is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->BidPrice1ByInsert.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("BidPrice1ByInsert"))
					theOrder.BidPrice1ByInsert.clear();
			}
			else
			{
				if (pOrder->BidPrice1ByInsert.isValid())
				{
					theOrder.BidPrice1ByInsert = pOrder->BidPrice1ByInsert;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's BidPrice1ByInsert is invalid for %s", pOrder->BidPrice1ByInsert.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BidPrice1ByInsert");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("BidPrice1ByInsert"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key BidPrice1ByInsert is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->AskPrice1ByInsert.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("AskPrice1ByInsert"))
					theOrder.AskPrice1ByInsert.clear();
			}
			else
			{
				if (pOrder->AskPrice1ByInsert.isValid())
				{
					theOrder.AskPrice1ByInsert = pOrder->AskPrice1ByInsert;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's AskPrice1ByInsert is invalid for %s", pOrder->AskPrice1ByInsert.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AskPrice1ByInsert");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("AskPrice1ByInsert"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key AskPrice1ByInsert is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->Available.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("Available"))
					theOrder.Available.clear();
			}
			else
			{
				if (pOrder->Available.isValid())
				{
					theOrder.Available = pOrder->Available;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Available is invalid for %s", pOrder->Available.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Available");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("Available"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key Available is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pOrder->CreateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_OrderFactory->isPK("CreateTime"))
					theOrder.CreateTime.clear();
			}
			else
			{
				if (pOrder->CreateTime.isValid())
				{
					theOrder.CreateTime = pOrder->CreateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's CreateTime is invalid for %s", pOrder->CreateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CreateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_OrderFactory->isPK("CreateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_Order's Primary Key CreateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		theOrder.m_pMDB = m_pMDB;
		theOrder.linkInstrument();
		theOrder.m_pMDB = m_pMDB;
		theOrder.linkPosition();
		theOrder.m_pMDB = m_pMDB;
		theOrder.linkMember();
		theOrder.m_pMDB = m_pMDB;
		theOrder.linkTriggerOrder();
		m_pMDB->m_OrderFactory->addOrUpdate(pOldOrder, &theOrder, pTransaction);
		CopyOrderEntity(pOrder,&theOrder);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_OrderFactory->removeByPK(pOrder, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		m_pMDB->m_TriggerOrderFactory->linkAllOrder();
		return true;
	}
	case TID_SendQryOne:
	{
		COrder *pOldOrder = m_pMDB->m_OrderFactory->findByPK(pOrder);
		if (pOldOrder == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyOrderEntity(pOrder,pOldOrder);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncPositionOrderImpl(int nTid, CPositionOrderField *pPositionOrder, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CPositionOrder *pOldPositionOrder = m_pMDB->m_PositionOrderFactory->findByPK(pPositionOrder);
		if(pOldPositionOrder != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteablePositionOrder thePositionOrder;
		CopyPositionOrderEntity(&thePositionOrder,pPositionOrder);
		m_pMDB->m_PositionOrderFactory->add(&thePositionOrder, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteablePositionOrder thePositionOrder;
		CPositionOrder *pOldPositionOrder = m_pMDB->m_PositionOrderFactory->findByPK(pPositionOrder);
		if(pOldPositionOrder == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			thePositionOrder.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_PositionOrderFactory->retrieve(pOldPositionOrder, &thePositionOrder);
		}

		if (!pPositionOrder->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("MemberID"))
					thePositionOrder.MemberID.clear();
			}
			else
			{
				if (pPositionOrder->MemberID.isValid())
				{
					thePositionOrder.MemberID = pPositionOrder->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's MemberID is invalid for %s", pPositionOrder->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->TradeUnitID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("TradeUnitID"))
					thePositionOrder.TradeUnitID.clear();
			}
			else
			{
				if (pPositionOrder->TradeUnitID.isValid())
				{
					thePositionOrder.TradeUnitID = pPositionOrder->TradeUnitID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's TradeUnitID is invalid for %s", pPositionOrder->TradeUnitID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradeUnitID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("TradeUnitID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key TradeUnitID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->AccountID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("AccountID"))
					thePositionOrder.AccountID.clear();
			}
			else
			{
				if (pPositionOrder->AccountID.isValid())
				{
					thePositionOrder.AccountID = pPositionOrder->AccountID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's AccountID is invalid for %s", pPositionOrder->AccountID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AccountID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("AccountID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key AccountID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->LocalID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("LocalID"))
					thePositionOrder.LocalID.clear();
			}
			else
			{
				if (pPositionOrder->LocalID.isValid())
				{
					thePositionOrder.LocalID = pPositionOrder->LocalID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's LocalID is invalid for %s", pPositionOrder->LocalID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LocalID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("LocalID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key LocalID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->ExchangeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("ExchangeID"))
					thePositionOrder.ExchangeID.clear();
			}
			else
			{
				if (pPositionOrder->ExchangeID.isValid())
				{
					thePositionOrder.ExchangeID = pPositionOrder->ExchangeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's ExchangeID is invalid for %s", pPositionOrder->ExchangeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExchangeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("ExchangeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key ExchangeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("InstrumentID"))
					thePositionOrder.InstrumentID.clear();
			}
			else
			{
				if (pPositionOrder->InstrumentID.isValid())
				{
					thePositionOrder.InstrumentID = pPositionOrder->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's InstrumentID is invalid for %s", pPositionOrder->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->OrderPriceType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("OrderPriceType"))
					thePositionOrder.OrderPriceType.clear();
			}
			else
			{
				if (pPositionOrder->OrderPriceType.isValid())
				{
					thePositionOrder.OrderPriceType = pPositionOrder->OrderPriceType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's OrderPriceType is invalid for %s", pPositionOrder->OrderPriceType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderPriceType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("OrderPriceType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key OrderPriceType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Direction.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Direction"))
					thePositionOrder.Direction.clear();
			}
			else
			{
				if (pPositionOrder->Direction.isValid())
				{
					thePositionOrder.Direction = pPositionOrder->Direction;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Direction is invalid for %s", pPositionOrder->Direction.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Direction");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Direction"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Direction is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->OffsetFlag.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("OffsetFlag"))
					thePositionOrder.OffsetFlag.clear();
			}
			else
			{
				if (pPositionOrder->OffsetFlag.isValid())
				{
					thePositionOrder.OffsetFlag = pPositionOrder->OffsetFlag;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's OffsetFlag is invalid for %s", pPositionOrder->OffsetFlag.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OffsetFlag");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("OffsetFlag"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key OffsetFlag is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Price.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Price"))
					thePositionOrder.Price.clear();
			}
			else
			{
				if (pPositionOrder->Price.isValid())
				{
					thePositionOrder.Price = pPositionOrder->Price;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Price is invalid for %s", pPositionOrder->Price.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Price");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Price"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Price is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Volume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Volume"))
					thePositionOrder.Volume.clear();
			}
			else
			{
				if (pPositionOrder->Volume.isValid())
				{
					thePositionOrder.Volume = pPositionOrder->Volume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Volume is invalid for %s", pPositionOrder->Volume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Volume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Volume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Volume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->VolumeDisplay.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("VolumeDisplay"))
					thePositionOrder.VolumeDisplay.clear();
			}
			else
			{
				if (pPositionOrder->VolumeDisplay.isValid())
				{
					thePositionOrder.VolumeDisplay = pPositionOrder->VolumeDisplay;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's VolumeDisplay is invalid for %s", pPositionOrder->VolumeDisplay.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeDisplay");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("VolumeDisplay"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key VolumeDisplay is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->VolumeMode.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("VolumeMode"))
					thePositionOrder.VolumeMode.clear();
			}
			else
			{
				if (pPositionOrder->VolumeMode.isValid())
				{
					thePositionOrder.VolumeMode = pPositionOrder->VolumeMode;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's VolumeMode is invalid for %s", pPositionOrder->VolumeMode.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeMode");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("VolumeMode"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key VolumeMode is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Cost.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Cost"))
					thePositionOrder.Cost.clear();
			}
			else
			{
				if (pPositionOrder->Cost.isValid())
				{
					thePositionOrder.Cost = pPositionOrder->Cost;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Cost is invalid for %s", pPositionOrder->Cost.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Cost");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Cost"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Cost is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->OrderType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("OrderType"))
					thePositionOrder.OrderType.clear();
			}
			else
			{
				if (pPositionOrder->OrderType.isValid())
				{
					thePositionOrder.OrderType = pPositionOrder->OrderType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's OrderType is invalid for %s", pPositionOrder->OrderType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("OrderType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key OrderType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->GTDTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("GTDTime"))
					thePositionOrder.GTDTime.clear();
			}
			else
			{
				if (pPositionOrder->GTDTime.isValid())
				{
					thePositionOrder.GTDTime = pPositionOrder->GTDTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's GTDTime is invalid for %s", pPositionOrder->GTDTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"GTDTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("GTDTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key GTDTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->MinVolume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("MinVolume"))
					thePositionOrder.MinVolume.clear();
			}
			else
			{
				if (pPositionOrder->MinVolume.isValid())
				{
					thePositionOrder.MinVolume = pPositionOrder->MinVolume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's MinVolume is invalid for %s", pPositionOrder->MinVolume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MinVolume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("MinVolume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key MinVolume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->BusinessType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("BusinessType"))
					thePositionOrder.BusinessType.clear();
			}
			else
			{
				if (pPositionOrder->BusinessType.isValid())
				{
					thePositionOrder.BusinessType = pPositionOrder->BusinessType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's BusinessType is invalid for %s", pPositionOrder->BusinessType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("BusinessType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key BusinessType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->BusinessValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("BusinessValue"))
					thePositionOrder.BusinessValue.clear();
			}
			else
			{
				if (pPositionOrder->BusinessValue.isValid())
				{
					thePositionOrder.BusinessValue = pPositionOrder->BusinessValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's BusinessValue is invalid for %s", pPositionOrder->BusinessValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("BusinessValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key BusinessValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->CloseOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("CloseOrderID"))
					thePositionOrder.CloseOrderID.clear();
			}
			else
			{
				if (pPositionOrder->CloseOrderID.isValid())
				{
					thePositionOrder.CloseOrderID = pPositionOrder->CloseOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's CloseOrderID is invalid for %s", pPositionOrder->CloseOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("CloseOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key CloseOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->IsCrossMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("IsCrossMargin"))
					thePositionOrder.IsCrossMargin.clear();
			}
			else
			{
				if (pPositionOrder->IsCrossMargin.isValid())
				{
					thePositionOrder.IsCrossMargin = pPositionOrder->IsCrossMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's IsCrossMargin is invalid for %s", pPositionOrder->IsCrossMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"IsCrossMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("IsCrossMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key IsCrossMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Remark"))
					thePositionOrder.Remark.clear();
			}
			else
			{
				if (pPositionOrder->Remark.isValid())
				{
					thePositionOrder.Remark = pPositionOrder->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Remark is invalid for %s", pPositionOrder->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->OrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("OrderID"))
					thePositionOrder.OrderID.clear();
			}
			else
			{
				if (pPositionOrder->OrderID.isValid())
				{
					thePositionOrder.OrderID = pPositionOrder->OrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's OrderID is invalid for %s", pPositionOrder->OrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("OrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key OrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->CopyMemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("CopyMemberID"))
					thePositionOrder.CopyMemberID.clear();
			}
			else
			{
				if (pPositionOrder->CopyMemberID.isValid())
				{
					thePositionOrder.CopyMemberID = pPositionOrder->CopyMemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's CopyMemberID is invalid for %s", pPositionOrder->CopyMemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyMemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("CopyMemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key CopyMemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->CopyOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("CopyOrderID"))
					thePositionOrder.CopyOrderID.clear();
			}
			else
			{
				if (pPositionOrder->CopyOrderID.isValid())
				{
					thePositionOrder.CopyOrderID = pPositionOrder->CopyOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's CopyOrderID is invalid for %s", pPositionOrder->CopyOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("CopyOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key CopyOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Leverage.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Leverage"))
					thePositionOrder.Leverage.clear();
			}
			else
			{
				if (pPositionOrder->Leverage.isValid())
				{
					thePositionOrder.Leverage = pPositionOrder->Leverage;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Leverage is invalid for %s", pPositionOrder->Leverage.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Leverage");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Leverage"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Leverage is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->CopyProfitRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("CopyProfitRate"))
					thePositionOrder.CopyProfitRate.clear();
			}
			else
			{
				if (pPositionOrder->CopyProfitRate.isValid())
				{
					thePositionOrder.CopyProfitRate = pPositionOrder->CopyProfitRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's CopyProfitRate is invalid for %s", pPositionOrder->CopyProfitRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyProfitRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("CopyProfitRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key CopyProfitRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->APPID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("APPID"))
					thePositionOrder.APPID.clear();
			}
			else
			{
				if (pPositionOrder->APPID.isValid())
				{
					thePositionOrder.APPID = pPositionOrder->APPID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's APPID is invalid for %s", pPositionOrder->APPID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"APPID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("APPID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key APPID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->PositionID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("PositionID"))
					thePositionOrder.PositionID.clear();
			}
			else
			{
				if (pPositionOrder->PositionID.isValid())
				{
					thePositionOrder.PositionID = pPositionOrder->PositionID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's PositionID is invalid for %s", pPositionOrder->PositionID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("PositionID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key PositionID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->TriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("TriggerPrice"))
					thePositionOrder.TriggerPrice.clear();
			}
			else
			{
				if (pPositionOrder->TriggerPrice.isValid())
				{
					thePositionOrder.TriggerPrice = pPositionOrder->TriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's TriggerPrice is invalid for %s", pPositionOrder->TriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("TriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key TriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Reserve.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Reserve"))
					thePositionOrder.Reserve.clear();
			}
			else
			{
				if (pPositionOrder->Reserve.isValid())
				{
					thePositionOrder.Reserve = pPositionOrder->Reserve;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Reserve is invalid for %s", pPositionOrder->Reserve.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Reserve");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Reserve"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Reserve is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->OrderStatus.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("OrderStatus"))
					thePositionOrder.OrderStatus.clear();
			}
			else
			{
				if (pPositionOrder->OrderStatus.isValid())
				{
					thePositionOrder.OrderStatus = pPositionOrder->OrderStatus;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's OrderStatus is invalid for %s", pPositionOrder->OrderStatus.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderStatus");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("OrderStatus"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key OrderStatus is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->DeriveSource.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("DeriveSource"))
					thePositionOrder.DeriveSource.clear();
			}
			else
			{
				if (pPositionOrder->DeriveSource.isValid())
				{
					thePositionOrder.DeriveSource = pPositionOrder->DeriveSource;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's DeriveSource is invalid for %s", pPositionOrder->DeriveSource.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"DeriveSource");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("DeriveSource"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key DeriveSource is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->DeriveDetail.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("DeriveDetail"))
					thePositionOrder.DeriveDetail.clear();
			}
			else
			{
				if (pPositionOrder->DeriveDetail.isValid())
				{
					thePositionOrder.DeriveDetail = pPositionOrder->DeriveDetail;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's DeriveDetail is invalid for %s", pPositionOrder->DeriveDetail.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"DeriveDetail");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("DeriveDetail"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key DeriveDetail is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->VolumeTraded.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("VolumeTraded"))
					thePositionOrder.VolumeTraded.clear();
			}
			else
			{
				if (pPositionOrder->VolumeTraded.isValid())
				{
					thePositionOrder.VolumeTraded = pPositionOrder->VolumeTraded;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's VolumeTraded is invalid for %s", pPositionOrder->VolumeTraded.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeTraded");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("VolumeTraded"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key VolumeTraded is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->VolumeRemain.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("VolumeRemain"))
					thePositionOrder.VolumeRemain.clear();
			}
			else
			{
				if (pPositionOrder->VolumeRemain.isValid())
				{
					thePositionOrder.VolumeRemain = pPositionOrder->VolumeRemain;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's VolumeRemain is invalid for %s", pPositionOrder->VolumeRemain.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeRemain");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("VolumeRemain"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key VolumeRemain is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->VolumeCancled.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("VolumeCancled"))
					thePositionOrder.VolumeCancled.clear();
			}
			else
			{
				if (pPositionOrder->VolumeCancled.isValid())
				{
					thePositionOrder.VolumeCancled = pPositionOrder->VolumeCancled;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's VolumeCancled is invalid for %s", pPositionOrder->VolumeCancled.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeCancled");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("VolumeCancled"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key VolumeCancled is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("InsertTime"))
					thePositionOrder.InsertTime.clear();
			}
			else
			{
				if (pPositionOrder->InsertTime.isValid())
				{
					thePositionOrder.InsertTime = pPositionOrder->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's InsertTime is invalid for %s", pPositionOrder->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("UpdateTime"))
					thePositionOrder.UpdateTime.clear();
			}
			else
			{
				if (pPositionOrder->UpdateTime.isValid())
				{
					thePositionOrder.UpdateTime = pPositionOrder->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's UpdateTime is invalid for %s", pPositionOrder->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Priority.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Priority"))
					thePositionOrder.Priority.clear();
			}
			else
			{
				if (pPositionOrder->Priority.isValid())
				{
					thePositionOrder.Priority = pPositionOrder->Priority;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Priority is invalid for %s", pPositionOrder->Priority.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Priority");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Priority"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Priority is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->TimeSortNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("TimeSortNo"))
					thePositionOrder.TimeSortNo.clear();
			}
			else
			{
				if (pPositionOrder->TimeSortNo.isValid())
				{
					thePositionOrder.TimeSortNo = pPositionOrder->TimeSortNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's TimeSortNo is invalid for %s", pPositionOrder->TimeSortNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TimeSortNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("TimeSortNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key TimeSortNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->FrontNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("FrontNo"))
					thePositionOrder.FrontNo.clear();
			}
			else
			{
				if (pPositionOrder->FrontNo.isValid())
				{
					thePositionOrder.FrontNo = pPositionOrder->FrontNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's FrontNo is invalid for %s", pPositionOrder->FrontNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrontNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("FrontNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key FrontNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->PriceCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("PriceCurrency"))
					thePositionOrder.PriceCurrency.clear();
			}
			else
			{
				if (pPositionOrder->PriceCurrency.isValid())
				{
					thePositionOrder.PriceCurrency = pPositionOrder->PriceCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's PriceCurrency is invalid for %s", pPositionOrder->PriceCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PriceCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("PriceCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key PriceCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->FeeCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("FeeCurrency"))
					thePositionOrder.FeeCurrency.clear();
			}
			else
			{
				if (pPositionOrder->FeeCurrency.isValid())
				{
					thePositionOrder.FeeCurrency = pPositionOrder->FeeCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's FeeCurrency is invalid for %s", pPositionOrder->FeeCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FeeCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("FeeCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key FeeCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->ClearCurrency.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("ClearCurrency"))
					thePositionOrder.ClearCurrency.clear();
			}
			else
			{
				if (pPositionOrder->ClearCurrency.isValid())
				{
					thePositionOrder.ClearCurrency = pPositionOrder->ClearCurrency;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's ClearCurrency is invalid for %s", pPositionOrder->ClearCurrency.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ClearCurrency");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("ClearCurrency"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key ClearCurrency is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->FrozenMoney.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("FrozenMoney"))
					thePositionOrder.FrozenMoney.clear();
			}
			else
			{
				if (pPositionOrder->FrozenMoney.isValid())
				{
					thePositionOrder.FrozenMoney = pPositionOrder->FrozenMoney;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's FrozenMoney is invalid for %s", pPositionOrder->FrozenMoney.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrozenMoney");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("FrozenMoney"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key FrozenMoney is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->FrozenFee.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("FrozenFee"))
					thePositionOrder.FrozenFee.clear();
			}
			else
			{
				if (pPositionOrder->FrozenFee.isValid())
				{
					thePositionOrder.FrozenFee = pPositionOrder->FrozenFee;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's FrozenFee is invalid for %s", pPositionOrder->FrozenFee.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrozenFee");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("FrozenFee"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key FrozenFee is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->FrozenMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("FrozenMargin"))
					thePositionOrder.FrozenMargin.clear();
			}
			else
			{
				if (pPositionOrder->FrozenMargin.isValid())
				{
					thePositionOrder.FrozenMargin = pPositionOrder->FrozenMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's FrozenMargin is invalid for %s", pPositionOrder->FrozenMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrozenMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("FrozenMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key FrozenMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Fee.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Fee"))
					thePositionOrder.Fee.clear();
			}
			else
			{
				if (pPositionOrder->Fee.isValid())
				{
					thePositionOrder.Fee = pPositionOrder->Fee;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Fee is invalid for %s", pPositionOrder->Fee.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Fee");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Fee"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Fee is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->CloseProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("CloseProfit"))
					thePositionOrder.CloseProfit.clear();
			}
			else
			{
				if (pPositionOrder->CloseProfit.isValid())
				{
					thePositionOrder.CloseProfit = pPositionOrder->CloseProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's CloseProfit is invalid for %s", pPositionOrder->CloseProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("CloseProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key CloseProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Turnover.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Turnover"))
					thePositionOrder.Turnover.clear();
			}
			else
			{
				if (pPositionOrder->Turnover.isValid())
				{
					thePositionOrder.Turnover = pPositionOrder->Turnover;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Turnover is invalid for %s", pPositionOrder->Turnover.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Turnover");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Turnover"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Turnover is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->RelatedOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("RelatedOrderID"))
					thePositionOrder.RelatedOrderID.clear();
			}
			else
			{
				if (pPositionOrder->RelatedOrderID.isValid())
				{
					thePositionOrder.RelatedOrderID = pPositionOrder->RelatedOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's RelatedOrderID is invalid for %s", pPositionOrder->RelatedOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"RelatedOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("RelatedOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key RelatedOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->BusinessResult.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("BusinessResult"))
					thePositionOrder.BusinessResult.clear();
			}
			else
			{
				if (pPositionOrder->BusinessResult.isValid())
				{
					thePositionOrder.BusinessResult = pPositionOrder->BusinessResult;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's BusinessResult is invalid for %s", pPositionOrder->BusinessResult.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessResult");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("BusinessResult"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key BusinessResult is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->BusinessNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("BusinessNo"))
					thePositionOrder.BusinessNo.clear();
			}
			else
			{
				if (pPositionOrder->BusinessNo.isValid())
				{
					thePositionOrder.BusinessNo = pPositionOrder->BusinessNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's BusinessNo is invalid for %s", pPositionOrder->BusinessNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("BusinessNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key BusinessNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Tradable.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Tradable"))
					thePositionOrder.Tradable.clear();
			}
			else
			{
				if (pPositionOrder->Tradable.isValid())
				{
					thePositionOrder.Tradable = pPositionOrder->Tradable;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Tradable is invalid for %s", pPositionOrder->Tradable.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Tradable");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Tradable"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Tradable is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->SettlementGroup.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("SettlementGroup"))
					thePositionOrder.SettlementGroup.clear();
			}
			else
			{
				if (pPositionOrder->SettlementGroup.isValid())
				{
					thePositionOrder.SettlementGroup = pPositionOrder->SettlementGroup;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's SettlementGroup is invalid for %s", pPositionOrder->SettlementGroup.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SettlementGroup");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("SettlementGroup"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key SettlementGroup is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->PosiDirection.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("PosiDirection"))
					thePositionOrder.PosiDirection.clear();
			}
			else
			{
				if (pPositionOrder->PosiDirection.isValid())
				{
					thePositionOrder.PosiDirection = pPositionOrder->PosiDirection;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's PosiDirection is invalid for %s", pPositionOrder->PosiDirection.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PosiDirection");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("PosiDirection"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key PosiDirection is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->TradePrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("TradePrice"))
					thePositionOrder.TradePrice.clear();
			}
			else
			{
				if (pPositionOrder->TradePrice.isValid())
				{
					thePositionOrder.TradePrice = pPositionOrder->TradePrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's TradePrice is invalid for %s", pPositionOrder->TradePrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradePrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("TradePrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key TradePrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->OpenPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("OpenPrice"))
					thePositionOrder.OpenPrice.clear();
			}
			else
			{
				if (pPositionOrder->OpenPrice.isValid())
				{
					thePositionOrder.OpenPrice = pPositionOrder->OpenPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's OpenPrice is invalid for %s", pPositionOrder->OpenPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("OpenPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key OpenPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->TriggerOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("TriggerOrderID"))
					thePositionOrder.TriggerOrderID.clear();
			}
			else
			{
				if (pPositionOrder->TriggerOrderID.isValid())
				{
					thePositionOrder.TriggerOrderID = pPositionOrder->TriggerOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's TriggerOrderID is invalid for %s", pPositionOrder->TriggerOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("TriggerOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key TriggerOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->SLTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("SLTriggerPrice"))
					thePositionOrder.SLTriggerPrice.clear();
			}
			else
			{
				if (pPositionOrder->SLTriggerPrice.isValid())
				{
					thePositionOrder.SLTriggerPrice = pPositionOrder->SLTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's SLTriggerPrice is invalid for %s", pPositionOrder->SLTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SLTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("SLTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key SLTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->TPTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("TPTriggerPrice"))
					thePositionOrder.TPTriggerPrice.clear();
			}
			else
			{
				if (pPositionOrder->TPTriggerPrice.isValid())
				{
					thePositionOrder.TPTriggerPrice = pPositionOrder->TPTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's TPTriggerPrice is invalid for %s", pPositionOrder->TPTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TPTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("TPTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key TPTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->CopyProfit.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("CopyProfit"))
					thePositionOrder.CopyProfit.clear();
			}
			else
			{
				if (pPositionOrder->CopyProfit.isValid())
				{
					thePositionOrder.CopyProfit = pPositionOrder->CopyProfit;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's CopyProfit is invalid for %s", pPositionOrder->CopyProfit.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyProfit");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("CopyProfit"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key CopyProfit is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Position.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Position"))
					thePositionOrder.Position.clear();
			}
			else
			{
				if (pPositionOrder->Position.isValid())
				{
					thePositionOrder.Position = pPositionOrder->Position;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Position is invalid for %s", pPositionOrder->Position.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Position");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Position"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Position is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->UserID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("UserID"))
					thePositionOrder.UserID.clear();
			}
			else
			{
				if (pPositionOrder->UserID.isValid())
				{
					thePositionOrder.UserID = pPositionOrder->UserID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's UserID is invalid for %s", pPositionOrder->UserID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UserID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("UserID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key UserID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->LastPriceByInsert.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("LastPriceByInsert"))
					thePositionOrder.LastPriceByInsert.clear();
			}
			else
			{
				if (pPositionOrder->LastPriceByInsert.isValid())
				{
					thePositionOrder.LastPriceByInsert = pPositionOrder->LastPriceByInsert;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's LastPriceByInsert is invalid for %s", pPositionOrder->LastPriceByInsert.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LastPriceByInsert");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("LastPriceByInsert"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key LastPriceByInsert is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->BidPrice1ByInsert.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("BidPrice1ByInsert"))
					thePositionOrder.BidPrice1ByInsert.clear();
			}
			else
			{
				if (pPositionOrder->BidPrice1ByInsert.isValid())
				{
					thePositionOrder.BidPrice1ByInsert = pPositionOrder->BidPrice1ByInsert;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's BidPrice1ByInsert is invalid for %s", pPositionOrder->BidPrice1ByInsert.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BidPrice1ByInsert");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("BidPrice1ByInsert"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key BidPrice1ByInsert is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->AskPrice1ByInsert.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("AskPrice1ByInsert"))
					thePositionOrder.AskPrice1ByInsert.clear();
			}
			else
			{
				if (pPositionOrder->AskPrice1ByInsert.isValid())
				{
					thePositionOrder.AskPrice1ByInsert = pPositionOrder->AskPrice1ByInsert;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's AskPrice1ByInsert is invalid for %s", pPositionOrder->AskPrice1ByInsert.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AskPrice1ByInsert");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("AskPrice1ByInsert"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key AskPrice1ByInsert is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->Available.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("Available"))
					thePositionOrder.Available.clear();
			}
			else
			{
				if (pPositionOrder->Available.isValid())
				{
					thePositionOrder.Available = pPositionOrder->Available;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Available is invalid for %s", pPositionOrder->Available.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Available");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("Available"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key Available is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->CreateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("CreateTime"))
					thePositionOrder.CreateTime.clear();
			}
			else
			{
				if (pPositionOrder->CreateTime.isValid())
				{
					thePositionOrder.CreateTime = pPositionOrder->CreateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's CreateTime is invalid for %s", pPositionOrder->CreateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CreateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("CreateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key CreateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->VolumeToClose.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("VolumeToClose"))
					thePositionOrder.VolumeToClose.clear();
			}
			else
			{
				if (pPositionOrder->VolumeToClose.isValid())
				{
					thePositionOrder.VolumeToClose = pPositionOrder->VolumeToClose;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's VolumeToClose is invalid for %s", pPositionOrder->VolumeToClose.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeToClose");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("VolumeToClose"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key VolumeToClose is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->VolumeClosed.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("VolumeClosed"))
					thePositionOrder.VolumeClosed.clear();
			}
			else
			{
				if (pPositionOrder->VolumeClosed.isValid())
				{
					thePositionOrder.VolumeClosed = pPositionOrder->VolumeClosed;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's VolumeClosed is invalid for %s", pPositionOrder->VolumeClosed.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeClosed");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("VolumeClosed"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key VolumeClosed is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pPositionOrder->VolumeOnClose.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_PositionOrderFactory->isPK("VolumeOnClose"))
					thePositionOrder.VolumeOnClose.clear();
			}
			else
			{
				if (pPositionOrder->VolumeOnClose.isValid())
				{
					thePositionOrder.VolumeOnClose = pPositionOrder->VolumeOnClose;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's VolumeOnClose is invalid for %s", pPositionOrder->VolumeOnClose.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeOnClose");
				}
			}
		}
		else
		{
			if(m_pMDB->m_PositionOrderFactory->isPK("VolumeOnClose"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_PositionOrder's Primary Key VolumeOnClose is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_PositionOrderFactory->addOrUpdate(pOldPositionOrder, &thePositionOrder, pTransaction);
		CopyPositionOrderEntity(pPositionOrder,&thePositionOrder);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_PositionOrderFactory->removeByPK(pPositionOrder, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CPositionOrder *pOldPositionOrder = m_pMDB->m_PositionOrderFactory->findByPK(pPositionOrder);
		if (pOldPositionOrder == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyPositionOrderEntity(pPositionOrder,pOldPositionOrder);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncTriggerOrderImpl(int nTid, CTriggerOrderField *pTriggerOrder, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CTriggerOrder *pOldTriggerOrder = m_pMDB->m_TriggerOrderFactory->findByPK(pTriggerOrder);
		if(pOldTriggerOrder != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableTriggerOrder theTriggerOrder;
		CopyTriggerOrderEntity(&theTriggerOrder,pTriggerOrder);
		m_pMDB->m_TriggerOrderFactory->add(&theTriggerOrder, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableTriggerOrder theTriggerOrder;
		CTriggerOrder *pOldTriggerOrder = m_pMDB->m_TriggerOrderFactory->findByPK(pTriggerOrder);
		if(pOldTriggerOrder == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theTriggerOrder.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_TriggerOrderFactory->retrieve(pOldTriggerOrder, &theTriggerOrder);
		}

		if (!pTriggerOrder->MemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("MemberID"))
					theTriggerOrder.MemberID.clear();
			}
			else
			{
				if (pTriggerOrder->MemberID.isValid())
				{
					theTriggerOrder.MemberID = pTriggerOrder->MemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's MemberID is invalid for %s", pTriggerOrder->MemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("MemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key MemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TradeUnitID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TradeUnitID"))
					theTriggerOrder.TradeUnitID.clear();
			}
			else
			{
				if (pTriggerOrder->TradeUnitID.isValid())
				{
					theTriggerOrder.TradeUnitID = pTriggerOrder->TradeUnitID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TradeUnitID is invalid for %s", pTriggerOrder->TradeUnitID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TradeUnitID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TradeUnitID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TradeUnitID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->AccountID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("AccountID"))
					theTriggerOrder.AccountID.clear();
			}
			else
			{
				if (pTriggerOrder->AccountID.isValid())
				{
					theTriggerOrder.AccountID = pTriggerOrder->AccountID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's AccountID is invalid for %s", pTriggerOrder->AccountID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"AccountID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("AccountID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key AccountID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->LocalID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("LocalID"))
					theTriggerOrder.LocalID.clear();
			}
			else
			{
				if (pTriggerOrder->LocalID.isValid())
				{
					theTriggerOrder.LocalID = pTriggerOrder->LocalID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's LocalID is invalid for %s", pTriggerOrder->LocalID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LocalID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("LocalID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key LocalID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->ExchangeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("ExchangeID"))
					theTriggerOrder.ExchangeID.clear();
			}
			else
			{
				if (pTriggerOrder->ExchangeID.isValid())
				{
					theTriggerOrder.ExchangeID = pTriggerOrder->ExchangeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's ExchangeID is invalid for %s", pTriggerOrder->ExchangeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExchangeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("ExchangeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key ExchangeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("InstrumentID"))
					theTriggerOrder.InstrumentID.clear();
			}
			else
			{
				if (pTriggerOrder->InstrumentID.isValid())
				{
					theTriggerOrder.InstrumentID = pTriggerOrder->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's InstrumentID is invalid for %s", pTriggerOrder->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->OrderPriceType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("OrderPriceType"))
					theTriggerOrder.OrderPriceType.clear();
			}
			else
			{
				if (pTriggerOrder->OrderPriceType.isValid())
				{
					theTriggerOrder.OrderPriceType = pTriggerOrder->OrderPriceType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's OrderPriceType is invalid for %s", pTriggerOrder->OrderPriceType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderPriceType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("OrderPriceType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key OrderPriceType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->Direction.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("Direction"))
					theTriggerOrder.Direction.clear();
			}
			else
			{
				if (pTriggerOrder->Direction.isValid())
				{
					theTriggerOrder.Direction = pTriggerOrder->Direction;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Direction is invalid for %s", pTriggerOrder->Direction.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Direction");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("Direction"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key Direction is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->OffsetFlag.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("OffsetFlag"))
					theTriggerOrder.OffsetFlag.clear();
			}
			else
			{
				if (pTriggerOrder->OffsetFlag.isValid())
				{
					theTriggerOrder.OffsetFlag = pTriggerOrder->OffsetFlag;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's OffsetFlag is invalid for %s", pTriggerOrder->OffsetFlag.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OffsetFlag");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("OffsetFlag"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key OffsetFlag is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->Price.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("Price"))
					theTriggerOrder.Price.clear();
			}
			else
			{
				if (pTriggerOrder->Price.isValid())
				{
					theTriggerOrder.Price = pTriggerOrder->Price;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Price is invalid for %s", pTriggerOrder->Price.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Price");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("Price"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key Price is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->Volume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("Volume"))
					theTriggerOrder.Volume.clear();
			}
			else
			{
				if (pTriggerOrder->Volume.isValid())
				{
					theTriggerOrder.Volume = pTriggerOrder->Volume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Volume is invalid for %s", pTriggerOrder->Volume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Volume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("Volume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key Volume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->VolumeDisplay.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("VolumeDisplay"))
					theTriggerOrder.VolumeDisplay.clear();
			}
			else
			{
				if (pTriggerOrder->VolumeDisplay.isValid())
				{
					theTriggerOrder.VolumeDisplay = pTriggerOrder->VolumeDisplay;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's VolumeDisplay is invalid for %s", pTriggerOrder->VolumeDisplay.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeDisplay");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("VolumeDisplay"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key VolumeDisplay is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->VolumeMode.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("VolumeMode"))
					theTriggerOrder.VolumeMode.clear();
			}
			else
			{
				if (pTriggerOrder->VolumeMode.isValid())
				{
					theTriggerOrder.VolumeMode = pTriggerOrder->VolumeMode;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's VolumeMode is invalid for %s", pTriggerOrder->VolumeMode.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"VolumeMode");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("VolumeMode"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key VolumeMode is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->Cost.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("Cost"))
					theTriggerOrder.Cost.clear();
			}
			else
			{
				if (pTriggerOrder->Cost.isValid())
				{
					theTriggerOrder.Cost = pTriggerOrder->Cost;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Cost is invalid for %s", pTriggerOrder->Cost.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Cost");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("Cost"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key Cost is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->OrderType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("OrderType"))
					theTriggerOrder.OrderType.clear();
			}
			else
			{
				if (pTriggerOrder->OrderType.isValid())
				{
					theTriggerOrder.OrderType = pTriggerOrder->OrderType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's OrderType is invalid for %s", pTriggerOrder->OrderType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("OrderType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key OrderType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->GTDTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("GTDTime"))
					theTriggerOrder.GTDTime.clear();
			}
			else
			{
				if (pTriggerOrder->GTDTime.isValid())
				{
					theTriggerOrder.GTDTime = pTriggerOrder->GTDTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's GTDTime is invalid for %s", pTriggerOrder->GTDTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"GTDTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("GTDTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key GTDTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->MinVolume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("MinVolume"))
					theTriggerOrder.MinVolume.clear();
			}
			else
			{
				if (pTriggerOrder->MinVolume.isValid())
				{
					theTriggerOrder.MinVolume = pTriggerOrder->MinVolume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's MinVolume is invalid for %s", pTriggerOrder->MinVolume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"MinVolume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("MinVolume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key MinVolume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->BusinessType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("BusinessType"))
					theTriggerOrder.BusinessType.clear();
			}
			else
			{
				if (pTriggerOrder->BusinessType.isValid())
				{
					theTriggerOrder.BusinessType = pTriggerOrder->BusinessType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's BusinessType is invalid for %s", pTriggerOrder->BusinessType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("BusinessType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key BusinessType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->BusinessValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("BusinessValue"))
					theTriggerOrder.BusinessValue.clear();
			}
			else
			{
				if (pTriggerOrder->BusinessValue.isValid())
				{
					theTriggerOrder.BusinessValue = pTriggerOrder->BusinessValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's BusinessValue is invalid for %s", pTriggerOrder->BusinessValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("BusinessValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key BusinessValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CloseOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CloseOrderID"))
					theTriggerOrder.CloseOrderID.clear();
			}
			else
			{
				if (pTriggerOrder->CloseOrderID.isValid())
				{
					theTriggerOrder.CloseOrderID = pTriggerOrder->CloseOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CloseOrderID is invalid for %s", pTriggerOrder->CloseOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CloseOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CloseOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->IsCrossMargin.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("IsCrossMargin"))
					theTriggerOrder.IsCrossMargin.clear();
			}
			else
			{
				if (pTriggerOrder->IsCrossMargin.isValid())
				{
					theTriggerOrder.IsCrossMargin = pTriggerOrder->IsCrossMargin;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's IsCrossMargin is invalid for %s", pTriggerOrder->IsCrossMargin.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"IsCrossMargin");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("IsCrossMargin"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key IsCrossMargin is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->Remark.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("Remark"))
					theTriggerOrder.Remark.clear();
			}
			else
			{
				if (pTriggerOrder->Remark.isValid())
				{
					theTriggerOrder.Remark = pTriggerOrder->Remark;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Remark is invalid for %s", pTriggerOrder->Remark.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Remark");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("Remark"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key Remark is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->OrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("OrderID"))
					theTriggerOrder.OrderID.clear();
			}
			else
			{
				if (pTriggerOrder->OrderID.isValid())
				{
					theTriggerOrder.OrderID = pTriggerOrder->OrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's OrderID is invalid for %s", pTriggerOrder->OrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("OrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key OrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CopyMemberID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CopyMemberID"))
					theTriggerOrder.CopyMemberID.clear();
			}
			else
			{
				if (pTriggerOrder->CopyMemberID.isValid())
				{
					theTriggerOrder.CopyMemberID = pTriggerOrder->CopyMemberID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CopyMemberID is invalid for %s", pTriggerOrder->CopyMemberID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyMemberID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CopyMemberID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CopyMemberID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CopyOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CopyOrderID"))
					theTriggerOrder.CopyOrderID.clear();
			}
			else
			{
				if (pTriggerOrder->CopyOrderID.isValid())
				{
					theTriggerOrder.CopyOrderID = pTriggerOrder->CopyOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CopyOrderID is invalid for %s", pTriggerOrder->CopyOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CopyOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CopyOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->Leverage.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("Leverage"))
					theTriggerOrder.Leverage.clear();
			}
			else
			{
				if (pTriggerOrder->Leverage.isValid())
				{
					theTriggerOrder.Leverage = pTriggerOrder->Leverage;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Leverage is invalid for %s", pTriggerOrder->Leverage.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Leverage");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("Leverage"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key Leverage is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CopyProfitRate.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CopyProfitRate"))
					theTriggerOrder.CopyProfitRate.clear();
			}
			else
			{
				if (pTriggerOrder->CopyProfitRate.isValid())
				{
					theTriggerOrder.CopyProfitRate = pTriggerOrder->CopyProfitRate;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CopyProfitRate is invalid for %s", pTriggerOrder->CopyProfitRate.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CopyProfitRate");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CopyProfitRate"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CopyProfitRate is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->APPID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("APPID"))
					theTriggerOrder.APPID.clear();
			}
			else
			{
				if (pTriggerOrder->APPID.isValid())
				{
					theTriggerOrder.APPID = pTriggerOrder->APPID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's APPID is invalid for %s", pTriggerOrder->APPID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"APPID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("APPID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key APPID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->PositionID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("PositionID"))
					theTriggerOrder.PositionID.clear();
			}
			else
			{
				if (pTriggerOrder->PositionID.isValid())
				{
					theTriggerOrder.PositionID = pTriggerOrder->PositionID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's PositionID is invalid for %s", pTriggerOrder->PositionID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PositionID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("PositionID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key PositionID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TriggerPrice"))
					theTriggerOrder.TriggerPrice.clear();
			}
			else
			{
				if (pTriggerOrder->TriggerPrice.isValid())
				{
					theTriggerOrder.TriggerPrice = pTriggerOrder->TriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TriggerPrice is invalid for %s", pTriggerOrder->TriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->Reserve.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("Reserve"))
					theTriggerOrder.Reserve.clear();
			}
			else
			{
				if (pTriggerOrder->Reserve.isValid())
				{
					theTriggerOrder.Reserve = pTriggerOrder->Reserve;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Reserve is invalid for %s", pTriggerOrder->Reserve.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Reserve");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("Reserve"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key Reserve is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->SLPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("SLPrice"))
					theTriggerOrder.SLPrice.clear();
			}
			else
			{
				if (pTriggerOrder->SLPrice.isValid())
				{
					theTriggerOrder.SLPrice = pTriggerOrder->SLPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's SLPrice is invalid for %s", pTriggerOrder->SLPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SLPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("SLPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key SLPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->SLTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("SLTriggerPrice"))
					theTriggerOrder.SLTriggerPrice.clear();
			}
			else
			{
				if (pTriggerOrder->SLTriggerPrice.isValid())
				{
					theTriggerOrder.SLTriggerPrice = pTriggerOrder->SLTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's SLTriggerPrice is invalid for %s", pTriggerOrder->SLTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"SLTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("SLTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key SLTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TPPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TPPrice"))
					theTriggerOrder.TPPrice.clear();
			}
			else
			{
				if (pTriggerOrder->TPPrice.isValid())
				{
					theTriggerOrder.TPPrice = pTriggerOrder->TPPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TPPrice is invalid for %s", pTriggerOrder->TPPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TPPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TPPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TPPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TPTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TPTriggerPrice"))
					theTriggerOrder.TPTriggerPrice.clear();
			}
			else
			{
				if (pTriggerOrder->TPTriggerPrice.isValid())
				{
					theTriggerOrder.TPTriggerPrice = pTriggerOrder->TPTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TPTriggerPrice is invalid for %s", pTriggerOrder->TPTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TPTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TPTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TPTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->RiskBefore.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("RiskBefore"))
					theTriggerOrder.RiskBefore.clear();
			}
			else
			{
				if (pTriggerOrder->RiskBefore.isValid())
				{
					theTriggerOrder.RiskBefore = pTriggerOrder->RiskBefore;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's RiskBefore is invalid for %s", pTriggerOrder->RiskBefore.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"RiskBefore");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("RiskBefore"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key RiskBefore is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TriggerOrderType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TriggerOrderType"))
					theTriggerOrder.TriggerOrderType.clear();
			}
			else
			{
				if (pTriggerOrder->TriggerOrderType.isValid())
				{
					theTriggerOrder.TriggerOrderType = pTriggerOrder->TriggerOrderType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TriggerOrderType is invalid for %s", pTriggerOrder->TriggerOrderType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerOrderType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TriggerOrderType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TriggerOrderType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TriggerDetail.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TriggerDetail"))
					theTriggerOrder.TriggerDetail.clear();
			}
			else
			{
				if (pTriggerOrder->TriggerDetail.isValid())
				{
					theTriggerOrder.TriggerDetail = pTriggerOrder->TriggerDetail;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TriggerDetail is invalid for %s", pTriggerOrder->TriggerDetail.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerDetail");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TriggerDetail"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TriggerDetail is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TriggerPriceType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TriggerPriceType"))
					theTriggerOrder.TriggerPriceType.clear();
			}
			else
			{
				if (pTriggerOrder->TriggerPriceType.isValid())
				{
					theTriggerOrder.TriggerPriceType = pTriggerOrder->TriggerPriceType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TriggerPriceType is invalid for %s", pTriggerOrder->TriggerPriceType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerPriceType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TriggerPriceType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TriggerPriceType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TriggerValue.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TriggerValue"))
					theTriggerOrder.TriggerValue.clear();
			}
			else
			{
				if (pTriggerOrder->TriggerValue.isValid())
				{
					theTriggerOrder.TriggerValue = pTriggerOrder->TriggerValue;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TriggerValue is invalid for %s", pTriggerOrder->TriggerValue.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerValue");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TriggerValue"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TriggerValue is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CloseSLPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CloseSLPrice"))
					theTriggerOrder.CloseSLPrice.clear();
			}
			else
			{
				if (pTriggerOrder->CloseSLPrice.isValid())
				{
					theTriggerOrder.CloseSLPrice = pTriggerOrder->CloseSLPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CloseSLPrice is invalid for %s", pTriggerOrder->CloseSLPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseSLPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CloseSLPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CloseSLPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CloseSLTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CloseSLTriggerPrice"))
					theTriggerOrder.CloseSLTriggerPrice.clear();
			}
			else
			{
				if (pTriggerOrder->CloseSLTriggerPrice.isValid())
				{
					theTriggerOrder.CloseSLTriggerPrice = pTriggerOrder->CloseSLTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CloseSLTriggerPrice is invalid for %s", pTriggerOrder->CloseSLTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseSLTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CloseSLTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CloseSLTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CloseTPPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CloseTPPrice"))
					theTriggerOrder.CloseTPPrice.clear();
			}
			else
			{
				if (pTriggerOrder->CloseTPPrice.isValid())
				{
					theTriggerOrder.CloseTPPrice = pTriggerOrder->CloseTPPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CloseTPPrice is invalid for %s", pTriggerOrder->CloseTPPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseTPPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CloseTPPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CloseTPPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CloseTPTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CloseTPTriggerPrice"))
					theTriggerOrder.CloseTPTriggerPrice.clear();
			}
			else
			{
				if (pTriggerOrder->CloseTPTriggerPrice.isValid())
				{
					theTriggerOrder.CloseTPTriggerPrice = pTriggerOrder->CloseTPTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CloseTPTriggerPrice is invalid for %s", pTriggerOrder->CloseTPTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseTPTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CloseTPTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CloseTPTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CloseOrderPriceType.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CloseOrderPriceType"))
					theTriggerOrder.CloseOrderPriceType.clear();
			}
			else
			{
				if (pTriggerOrder->CloseOrderPriceType.isValid())
				{
					theTriggerOrder.CloseOrderPriceType = pTriggerOrder->CloseOrderPriceType;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CloseOrderPriceType is invalid for %s", pTriggerOrder->CloseOrderPriceType.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseOrderPriceType");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CloseOrderPriceType"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CloseOrderPriceType is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->ClosePrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("ClosePrice"))
					theTriggerOrder.ClosePrice.clear();
			}
			else
			{
				if (pTriggerOrder->ClosePrice.isValid())
				{
					theTriggerOrder.ClosePrice = pTriggerOrder->ClosePrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's ClosePrice is invalid for %s", pTriggerOrder->ClosePrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ClosePrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("ClosePrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key ClosePrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CloseTriggerPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CloseTriggerPrice"))
					theTriggerOrder.CloseTriggerPrice.clear();
			}
			else
			{
				if (pTriggerOrder->CloseTriggerPrice.isValid())
				{
					theTriggerOrder.CloseTriggerPrice = pTriggerOrder->CloseTriggerPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CloseTriggerPrice is invalid for %s", pTriggerOrder->CloseTriggerPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CloseTriggerPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CloseTriggerPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CloseTriggerPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->RelatedOrderID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("RelatedOrderID"))
					theTriggerOrder.RelatedOrderID.clear();
			}
			else
			{
				if (pTriggerOrder->RelatedOrderID.isValid())
				{
					theTriggerOrder.RelatedOrderID = pTriggerOrder->RelatedOrderID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's RelatedOrderID is invalid for %s", pTriggerOrder->RelatedOrderID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"RelatedOrderID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("RelatedOrderID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key RelatedOrderID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->ActiveTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("ActiveTime"))
					theTriggerOrder.ActiveTime.clear();
			}
			else
			{
				if (pTriggerOrder->ActiveTime.isValid())
				{
					theTriggerOrder.ActiveTime = pTriggerOrder->ActiveTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's ActiveTime is invalid for %s", pTriggerOrder->ActiveTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ActiveTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("ActiveTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key ActiveTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TriggerTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TriggerTime"))
					theTriggerOrder.TriggerTime.clear();
			}
			else
			{
				if (pTriggerOrder->TriggerTime.isValid())
				{
					theTriggerOrder.TriggerTime = pTriggerOrder->TriggerTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TriggerTime is invalid for %s", pTriggerOrder->TriggerTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TriggerTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TriggerTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TimeSortNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TimeSortNo"))
					theTriggerOrder.TimeSortNo.clear();
			}
			else
			{
				if (pTriggerOrder->TimeSortNo.isValid())
				{
					theTriggerOrder.TimeSortNo = pTriggerOrder->TimeSortNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TimeSortNo is invalid for %s", pTriggerOrder->TimeSortNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TimeSortNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TimeSortNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TimeSortNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->TriggerStatus.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("TriggerStatus"))
					theTriggerOrder.TriggerStatus.clear();
			}
			else
			{
				if (pTriggerOrder->TriggerStatus.isValid())
				{
					theTriggerOrder.TriggerStatus = pTriggerOrder->TriggerStatus;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's TriggerStatus is invalid for %s", pTriggerOrder->TriggerStatus.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"TriggerStatus");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("TriggerStatus"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key TriggerStatus is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->PosiDirection.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("PosiDirection"))
					theTriggerOrder.PosiDirection.clear();
			}
			else
			{
				if (pTriggerOrder->PosiDirection.isValid())
				{
					theTriggerOrder.PosiDirection = pTriggerOrder->PosiDirection;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's PosiDirection is invalid for %s", pTriggerOrder->PosiDirection.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"PosiDirection");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("PosiDirection"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key PosiDirection is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->FrontNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("FrontNo"))
					theTriggerOrder.FrontNo.clear();
			}
			else
			{
				if (pTriggerOrder->FrontNo.isValid())
				{
					theTriggerOrder.FrontNo = pTriggerOrder->FrontNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's FrontNo is invalid for %s", pTriggerOrder->FrontNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"FrontNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("FrontNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key FrontNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->ErrorNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("ErrorNo"))
					theTriggerOrder.ErrorNo.clear();
			}
			else
			{
				if (pTriggerOrder->ErrorNo.isValid())
				{
					theTriggerOrder.ErrorNo = pTriggerOrder->ErrorNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's ErrorNo is invalid for %s", pTriggerOrder->ErrorNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ErrorNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("ErrorNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key ErrorNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->ErrorMsg.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("ErrorMsg"))
					theTriggerOrder.ErrorMsg.clear();
			}
			else
			{
				if (pTriggerOrder->ErrorMsg.isValid())
				{
					theTriggerOrder.ErrorMsg = pTriggerOrder->ErrorMsg;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's ErrorMsg is invalid for %s", pTriggerOrder->ErrorMsg.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ErrorMsg");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("ErrorMsg"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key ErrorMsg is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("InsertTime"))
					theTriggerOrder.InsertTime.clear();
			}
			else
			{
				if (pTriggerOrder->InsertTime.isValid())
				{
					theTriggerOrder.InsertTime = pTriggerOrder->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's InsertTime is invalid for %s", pTriggerOrder->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("UpdateTime"))
					theTriggerOrder.UpdateTime.clear();
			}
			else
			{
				if (pTriggerOrder->UpdateTime.isValid())
				{
					theTriggerOrder.UpdateTime = pTriggerOrder->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's UpdateTime is invalid for %s", pTriggerOrder->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->BusinessNo.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("BusinessNo"))
					theTriggerOrder.BusinessNo.clear();
			}
			else
			{
				if (pTriggerOrder->BusinessNo.isValid())
				{
					theTriggerOrder.BusinessNo = pTriggerOrder->BusinessNo;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's BusinessNo is invalid for %s", pTriggerOrder->BusinessNo.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"BusinessNo");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("BusinessNo"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key BusinessNo is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pTriggerOrder->CreateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_TriggerOrderFactory->isPK("CreateTime"))
					theTriggerOrder.CreateTime.clear();
			}
			else
			{
				if (pTriggerOrder->CreateTime.isValid())
				{
					theTriggerOrder.CreateTime = pTriggerOrder->CreateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's CreateTime is invalid for %s", pTriggerOrder->CreateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"CreateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_TriggerOrderFactory->isPK("CreateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_TriggerOrder's Primary Key CreateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		theTriggerOrder.m_pMDB = m_pMDB;
		theTriggerOrder.linkOrder();
		theTriggerOrder.m_pMDB = m_pMDB;
		theTriggerOrder.linkMember();
		theTriggerOrder.m_pMDB = m_pMDB;
		theTriggerOrder.linkInstrument();
		theTriggerOrder.m_pMDB = m_pMDB;
		theTriggerOrder.linkMarketData();
		m_pMDB->m_TriggerOrderFactory->addOrUpdate(pOldTriggerOrder, &theTriggerOrder, pTransaction);
		CopyTriggerOrderEntity(pTriggerOrder,&theTriggerOrder);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_TriggerOrderFactory->removeByPK(pTriggerOrder, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		m_pMDB->m_OrderFactory->linkAllTriggerOrder();
		return true;
	}
	case TID_SendQryOne:
	{
		CTriggerOrder *pOldTriggerOrder = m_pMDB->m_TriggerOrderFactory->findByPK(pTriggerOrder);
		if (pOldTriggerOrder == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyTriggerOrderEntity(pTriggerOrder,pOldTriggerOrder);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
bool CAlterService::SyncLastKLineImpl(int nTid, CLastKLineField *pLastKLine, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByPK(pLastKLine);
		if(pOldLastKLine != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteableLastKLine theLastKLine;
		CopyLastKLineEntity(&theLastKLine,pLastKLine);
		m_pMDB->m_LastKLineFactory->add(&theLastKLine, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteableLastKLine theLastKLine;
		CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByPK(pLastKLine);
		if(pOldLastKLine == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			theLastKLine.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_LastKLineFactory->retrieve(pOldLastKLine, &theLastKLine);
		}

		if (!pLastKLine->ExchangeID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("ExchangeID"))
					theLastKLine.ExchangeID.clear();
			}
			else
			{
				if (pLastKLine->ExchangeID.isValid())
				{
					theLastKLine.ExchangeID = pLastKLine->ExchangeID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's ExchangeID is invalid for %s", pLastKLine->ExchangeID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ExchangeID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("ExchangeID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key ExchangeID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLastKLine->InstrumentID.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("InstrumentID"))
					theLastKLine.InstrumentID.clear();
			}
			else
			{
				if (pLastKLine->InstrumentID.isValid())
				{
					theLastKLine.InstrumentID = pLastKLine->InstrumentID;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's InstrumentID is invalid for %s", pLastKLine->InstrumentID.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InstrumentID");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("InstrumentID"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key InstrumentID is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLastKLine->Bar.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("Bar"))
					theLastKLine.Bar.clear();
			}
			else
			{
				if (pLastKLine->Bar.isValid())
				{
					theLastKLine.Bar = pLastKLine->Bar;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Bar is invalid for %s", pLastKLine->Bar.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Bar");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("Bar"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key Bar is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLastKLine->InsertTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("InsertTime"))
					theLastKLine.InsertTime.clear();
			}
			else
			{
				if (pLastKLine->InsertTime.isValid())
				{
					theLastKLine.InsertTime = pLastKLine->InsertTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's InsertTime is invalid for %s", pLastKLine->InsertTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"InsertTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("InsertTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key InsertTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLastKLine->OpenPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("OpenPrice"))
					theLastKLine.OpenPrice.clear();
			}
			else
			{
				if (pLastKLine->OpenPrice.isValid())
				{
					theLastKLine.OpenPrice = pLastKLine->OpenPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's OpenPrice is invalid for %s", pLastKLine->OpenPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"OpenPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("OpenPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key OpenPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLastKLine->HighestPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("HighestPrice"))
					theLastKLine.HighestPrice.clear();
			}
			else
			{
				if (pLastKLine->HighestPrice.isValid())
				{
					theLastKLine.HighestPrice = pLastKLine->HighestPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's HighestPrice is invalid for %s", pLastKLine->HighestPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"HighestPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("HighestPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key HighestPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLastKLine->LowestPrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("LowestPrice"))
					theLastKLine.LowestPrice.clear();
			}
			else
			{
				if (pLastKLine->LowestPrice.isValid())
				{
					theLastKLine.LowestPrice = pLastKLine->LowestPrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's LowestPrice is invalid for %s", pLastKLine->LowestPrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"LowestPrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("LowestPrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key LowestPrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLastKLine->ClosePrice.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("ClosePrice"))
					theLastKLine.ClosePrice.clear();
			}
			else
			{
				if (pLastKLine->ClosePrice.isValid())
				{
					theLastKLine.ClosePrice = pLastKLine->ClosePrice;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's ClosePrice is invalid for %s", pLastKLine->ClosePrice.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"ClosePrice");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("ClosePrice"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key ClosePrice is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLastKLine->Volume.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("Volume"))
					theLastKLine.Volume.clear();
			}
			else
			{
				if (pLastKLine->Volume.isValid())
				{
					theLastKLine.Volume = pLastKLine->Volume;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Volume is invalid for %s", pLastKLine->Volume.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Volume");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("Volume"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key Volume is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLastKLine->Turnover.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("Turnover"))
					theLastKLine.Turnover.clear();
			}
			else
			{
				if (pLastKLine->Turnover.isValid())
				{
					theLastKLine.Turnover = pLastKLine->Turnover;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Turnover is invalid for %s", pLastKLine->Turnover.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"Turnover");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("Turnover"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key Turnover is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		if (!pLastKLine->UpdateTime.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_LastKLineFactory->isPK("UpdateTime"))
					theLastKLine.UpdateTime.clear();
			}
			else
			{
				if (pLastKLine->UpdateTime.isValid())
				{
					theLastKLine.UpdateTime = pLastKLine->UpdateTime;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's UpdateTime is invalid for %s", pLastKLine->UpdateTime.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"UpdateTime");
				}
			}
		}
		else
		{
			if(m_pMDB->m_LastKLineFactory->isPK("UpdateTime"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_LastKLine's Primary Key UpdateTime is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
		m_pMDB->m_LastKLineFactory->addOrUpdate(pOldLastKLine, &theLastKLine, pTransaction);
		CopyLastKLineEntity(pLastKLine,&theLastKLine);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_LastKLineFactory->removeByPK(pLastKLine, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		return true;
	}
	case TID_SendQryOne:
	{
		CLastKLine *pOldLastKLine = m_pMDB->m_LastKLineFactory->findByPK(pLastKLine);
		if (pOldLastKLine == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		CopyLastKLineEntity(pLastKLine,pOldLastKLine);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}

