#ifndef _MyResponser_H_
#define _MyResponser_H_

#include "FieldData.h"
#include "BaseFunction.h"
#include "CopyField.h"

class CMyXTPResponser : public CServiceResponser
{
public:
	CMyXTPResponser(COutFogServiceCallback *pServiceCallback) :CServiceResponser(pServiceCallback)
	{
	}

	virtual bool SimplePrepareMetaRef(CFieldDescribe *pFieldDescribe, char Type)
	{
		memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
		m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
		m_OutFogMetaRef.SessionNo = pFieldDescribe->m_FieldID;
		m_OutFogMetaRef.Reserve[0] = OUTFOG_RESERVE_SYNC_YES;
		m_OutFogMetaRef.Reserve[1] = OUTFOG_RESERVE_SYNC_YES;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_OutFogMetaRef.Type = Type;
		return true;
	}

	virtual bool PrepareMetaRef(CFieldDescribe *pFieldDescribe, void* pData, void* pOldData, char Type)
	{
		SimplePrepareMetaRef(pFieldDescribe, Type);

		switch (pFieldDescribe->m_FieldID)
		{
		case FID_Position:
		{
			//冻结持仓不发送，持仓归零需要发送
			if (m_OutFogMetaRef.Type != OUTFOG_TYPE_DELETE)
			{
				//有持仓的情况下才动
				if (((CPosition*)pData)->Position == 0 && ((CPosition*)pOldData)->Position == 0)
					return false;
				//if (((CPosition*)pData)->Position == ((CPosition*)pOldData)->Position)
				//{
				//	if (((CPosition*)pData)->SLTriggerPrice == ((CPosition*)pOldData)->SLTriggerPrice && ((CPosition*)pData)->TPTriggerPrice == ((CPosition*)pOldData)->TPTriggerPrice)
				//		return false;
				//}			
			}

			//如果是删除持仓，不需要发送状态，节省带宽，但是需要发送删除命令给redis和db
			if (m_OutFogMetaRef.Type != OUTFOG_TYPE_DELETE)
			{
				//通过pPosition来判断是否需要加入到FinishPositon
				if (((CPosition*)pData)->HighestPosition > 0 && ((CPosition*)pData)->Position == 0.0 && ((CPosition*)pData)->ShortFrozen == 0.0 && ((CPosition*)pData)->LongFrozen == 0.0)
				{
					m_OutFogMetaRef.RequestNo = 110;
					m_OutFogMetaRef.Type = OUTFOG_TYPE_INSERT;
					m_OutFogMetaRef.SessionNo = CFinishPositionField::m_Describe.m_FieldID;
					m_OutFogMetaRef.FieldNo = CFinishPositionField::m_Describe.m_FieldID;

					CFinishPositionField field;
					CopyFinishPositionEntity(&field, ((CPosition*)pOldData));
					CFinishPositionField::m_Describe.setSingleField(m_pStreamBuf, &field);
					m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);

					//重新让后续发送最新的Position
					m_OutFogMetaRef.RequestNo = 0;
					//m_OutFogMetaRef.Type = OUTFOG_TYPE_UPDATE;
					//让redis和数据库删除，这样没有持仓就查不到持仓了
					m_OutFogMetaRef.Type = OUTFOG_TYPE_DELETE;
					m_OutFogMetaRef.SessionNo = CPositionField::m_Describe.m_FieldID;
					m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;
				}
			}

			//风控需要得到所有仓位的变化信息
			m_OutFogMetaRef.MessageNo = TID_PushPosition;
			//m_OutFogMetaRef.SubjectNo = OF_Topic_Private_Risk;

			//持仓会删除的时候，这里就不需要再杠杆变化的时候通知客户了
			//如果杠杆有变化了，需要额外通知用户
			//if (pOldData != NULL && ((CPosition*)pData)->Leverage != ((CPosition*)pOldData)->Leverage)
			//{
			strcpy(m_OutFogMetaRef.SubIndexName, ((CPosition*)pData)->MemberID);
			//strcpy(m_OutFogMetaRef.UserID, ((CPosition*)pData)->UpdateUserID);
			sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_Risk, OF_Topic_Private_SnapShot);
			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
			//	}


			break;
		}
		case FID_Account:
		{
			m_OutFogMetaRef.MessageNo = TID_PushAccount;
			strcpy(m_OutFogMetaRef.SubIndexName, ((CAccount*)pData)->MemberID);
			//strcpy(m_OutFogMetaRef.UserID, ((CAccount*)pData)->UpdateUserID);
			sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_Risk, OF_Topic_Private_SnapShot);
			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
			break;
		}
		default:
		{
			m_OutFogMetaRef.MessageNo = OUTFOG_TNO_NULL;
			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_NULL;
		}
		}
		return true;
	}
private:
	CMDB* m_pMDB;
};

#endif
