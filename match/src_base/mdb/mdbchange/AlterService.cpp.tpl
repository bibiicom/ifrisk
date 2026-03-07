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

!!enter system!!
!!travel tables!!
!!if !strcmp(@manager,"Init")!!
	!!let tableName=@name!!
bool CAlterService::Sync!!@tableName!!Impl(int nTid, C!!@tableName!!Field *p!!@tableName!!, CTransaction *pTransaction)
{
	switch(nTid)
	{
	case TID_SendInit:
	{
		C!!@tableName!! *pOld!!@tableName!! = m_pMDB->m_!!@tableName!!Factory->findByPK(p!!@tableName!!);
		if(pOld!!@tableName!! != NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
		CWriteable!!@tableName!! the!!@tableName!!;
		Copy!!@tableName!!Entity(&the!!@tableName!!,p!!@tableName!!);
		m_pMDB->m_!!@tableName!!Factory->add(&the!!@tableName!!, pTransaction);
		return true;
	}	
	case TID_SendInsert:
	case TID_SendUpdate:
	case TID_SendInsUpd:
	case TID_SendClear:
	{		
		CWriteable!!@tableName!! the!!@tableName!!;
		C!!@tableName!! *pOld!!@tableName!! = m_pMDB->m_!!@tableName!!Factory->findByPK(p!!@tableName!!);
		if(pOld!!@tableName!! == NULL)
		{
			if( nTid == TID_SendUpdate || nTid == TID_SendClear )
				return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
			the!!@tableName!!.init();
		}
		else
		{
			if(nTid == TID_SendInsert)
				return m_pErrorEngine->reportError(ERROR_RECORD_EXIST);
			m_pMDB->m_!!@tableName!!Factory->retrieve(pOld!!@tableName!!, &the!!@tableName!!);
		}

	!!travel fields!!
		if (!p!!@tableName!!->!!@name!!.isNull())
		{
			if(nTid == TID_SendClear)
			{
				if(!m_pMDB->m_!!@tableName!!Factory->isPK("!!@name!!"))
					the!!@tableName!!.!!@name!!.clear();
			}
			else
			{
				if (p!!@tableName!!->!!@name!!.isValid())
				{
					the!!@tableName!!.!!@name!! = p!!@tableName!!->!!@name!!;
				}
				else
				{
					REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_!!@tableName!!'s !!@name!! is invalid for %s", p!!@tableName!!->!!@name!!.getString().c_str());
					return m_pErrorEngine->reportError(ERROR_INVALID_VALUE,"!!@name!!");
				}
			}
		}
		else
		{
			if(m_pMDB->m_!!@tableName!!Factory->isPK("!!@name!!"))
			{
				REPORT_EVENT(LOG_CRITICAL, "AlterMDBError", "FID_!!@tableName!!'s Primary Key !!@name!! is null");
				return m_pErrorEngine->reportError(ERROR_KEY_IS_NULL);	
			}
		}
	!!next!!
		//如果有外键需要进行link，要不然修改数据之后会找出link数据不存在，导致core发生
	!!travel foreigns!!
		the!!@tableName!!.m_pMDB = m_pMDB;
		the!!@tableName!!.link!!@name!!();
	!!next!!
		m_pMDB->m_!!@tableName!!Factory->addOrUpdate(pOld!!@tableName!!, &the!!@tableName!!, pTransaction);
		Copy!!@tableName!!Entity(p!!@tableName!!,&the!!@tableName!!);
		return true;
	}
	case TID_SendDelete:
	{
		if (!m_pMDB->m_!!@tableName!!Factory->removeByPK(p!!@tableName!!, pTransaction))
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		//如果删除了此表，所有的link到这个表的link全部需要变为null,虽然很耗性能但是不做指针引用不当就会core
		!!enter system!!
			!!travel tables!!
				!!let oTableName=@name!!
				!!travel foreigns!!
					!!if !strcmp(@hasTable,"yes")!!
						!!if !strcmp(@factory,@tableName)!!
		m_pMDB->m_!!@oTableName!!Factory->linkAll!!@name!!();
						!!endif!!
					!!endif!!
				!!next!!
			!!next!!
		!!leave!!
		return true;
	}
	case TID_SendQryOne:
	{
		C!!@tableName!! *pOld!!@tableName!! = m_pMDB->m_!!@tableName!!Factory->findByPK(p!!@tableName!!);
		if (pOld!!@tableName!! == NULL)
			return m_pErrorEngine->reportError(ERROR_RECORD_NOT_FOUND);
		Copy!!@tableName!!Entity(p!!@tableName!!,pOld!!@tableName!!);
		return true;
	}	
	default:
	{
		return m_pErrorEngine->reportError(ERROR_INVALID_ACTION);
	}
	}
	return true;
}
!!endif!!
!!next!!
!!leave!!

