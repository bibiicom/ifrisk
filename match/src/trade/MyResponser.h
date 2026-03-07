#ifndef _MyResponser_H_
#define _MyResponser_H_

#include "Engine.h"
#include "FieldData.h"
#include "tradingTools.h"
#include "BaseFunction.h"
#include "CopyField.h"
#include "Config.h"
#include "utility.h"

class CMyXTPResponser : public CServiceResponser
{
public:
	CMyXTPResponser(const char* sDumpPath, UF_INT8 nBeginNo, COutFogServiceCallback* pServiceCallback, CMDB* pMDB) :CServiceResponser(pServiceCallback)
	{
		m_pMDB = pMDB;
		string sIncPath = GetConfigString(INI_FILE_NAME, "IncPath");
		memset(m_sDumpPath, 0, sizeof(m_sDumpPath));
		if (sIncPath.size() != 0)
		{
			sprintf(m_sDumpPath, "%sinc.", sIncPath.c_str());

			char m_sDumpTmpPath[256];
			if (g_bOnlyRedo)
				sprintf(m_sDumpTmpPath, "%s%0.f.redo/", m_sDumpPath, nBeginNo);
			else
				sprintf(m_sDumpTmpPath, "%s%0.f/", m_sDumpPath, nBeginNo);

			makedir(m_sDumpTmpPath);

			sprintf(m_sFinishOrderDumpPathAll, "%sFinishOrder.csv", m_sDumpTmpPath);
			m_FinishOrderfileLog = mfopen(m_sFinishOrderDumpPathAll, "wt");
			CWriteableOrder::writeCSVHead(m_FinishOrderfileLog);
			fflush(m_FinishOrderfileLog);

			sprintf(m_sTradeDumpPathAll, "%sTrade.csv", m_sDumpTmpPath);
			m_TradefileLog = mfopen(m_sTradeDumpPathAll, "wt");
			CWriteableTrade::writeCSVHead(m_TradefileLog);
			fflush(m_TradefileLog);

			sprintf(m_sKLineDumpPathAll, "%sKLine.csv", m_sDumpTmpPath);
			m_KLinefileLog = mfopen(m_sKLineDumpPathAll, "wt");
			CWriteableKLine::writeCSVHead(m_KLinefileLog);
			fflush(m_KLinefileLog);

			sprintf(m_sAccountDetailDumpPathAll, "%sAccountDetail.csv", m_sDumpTmpPath);
			m_AccountDetailfileLog = mfopen(m_sAccountDetailDumpPathAll, "wt");
			CWriteableAccountDetail::writeCSVHead(m_AccountDetailfileLog);
			fflush(m_AccountDetailfileLog);

			sprintf(m_sSettleDetailDumpPathAll, "%sSettleDetail.csv", m_sDumpTmpPath);
			m_SettleDetailfileLog = mfopen(m_sSettleDetailDumpPathAll, "wt");
			CWriteableSettleDetail::writeCSVHead(m_SettleDetailfileLog);
			fflush(m_SettleDetailfileLog);

			sprintf(m_sFinishTriggerOrderDumpPathAll, "%sFinishTriggerOrder.csv", m_sDumpTmpPath);
			m_FinishTriggerOrderfileLog = mfopen(m_sFinishTriggerOrderDumpPathAll, "wt");
			CWriteableTriggerOrder::writeCSVHead(m_FinishTriggerOrderfileLog);
			fflush(m_FinishTriggerOrderfileLog);

			sprintf(m_sFinishPositionDumpPathAll, "%sFinishPosition.csv", m_sDumpTmpPath);
			m_FinishPositionfileLog = mfopen(m_sFinishPositionDumpPathAll, "wt");
			CWriteablePosition::writeCSVHead(m_FinishPositionfileLog);
			fflush(m_FinishPositionfileLog);
		}
		else
		{
			m_FinishOrderfileLog = NULL;
			m_TradefileLog = NULL;
			m_KLinefileLog = NULL;
			m_AccountDetailfileLog = NULL;
			m_SettleDetailfileLog = NULL;
			m_FinishTriggerOrderfileLog = NULL;
			m_FinishPositionfileLog = NULL;
		}
		m_nLastTime = 0;
		m_nFirstTime = 0;
		m_nLastKLineTimes = 0;
	}
	bool setdumpNo(UF_INT8 nSequenceNo)
	{
		char m_sDumpTmpPath[256];

		if (m_FinishOrderfileLog != NULL)
		{
			if (g_bOnlyRedo)
				sprintf(m_sDumpTmpPath, "%s%0.f.redo/", m_sDumpPath, nSequenceNo);
			else
				sprintf(m_sDumpTmpPath, "%s%0.f/", m_sDumpPath, nSequenceNo);

			makedir(m_sDumpTmpPath);

			fclose(m_FinishOrderfileLog);
			sprintf(m_sFinishOrderDumpPathAll, "%sFinishOrder.csv", m_sDumpTmpPath);
			m_FinishOrderfileLog = mfopen(m_sFinishOrderDumpPathAll, "wt");
			CWriteableOrder::writeCSVHead(m_FinishOrderfileLog);
			fflush(m_FinishOrderfileLog);
		}

		if (m_TradefileLog != NULL)
		{
			fclose(m_TradefileLog);
			sprintf(m_sTradeDumpPathAll, "%sTrade.csv", m_sDumpTmpPath);
			m_TradefileLog = mfopen(m_sTradeDumpPathAll, "wt");
			CWriteableTrade::writeCSVHead(m_TradefileLog);
			fflush(m_TradefileLog);
		}

		if (m_KLinefileLog != NULL)
		{
			fclose(m_KLinefileLog);
			sprintf(m_sKLineDumpPathAll, "%sKLine.csv", m_sDumpTmpPath);
			m_KLinefileLog = mfopen(m_sKLineDumpPathAll, "wt");
			CWriteableKLine::writeCSVHead(m_KLinefileLog);
			fflush(m_KLinefileLog);
		}

		if (m_AccountDetailfileLog != NULL)
		{
			fclose(m_AccountDetailfileLog);
			sprintf(m_sAccountDetailDumpPathAll, "%sAccountDetail.csv", m_sDumpTmpPath);
			m_AccountDetailfileLog = mfopen(m_sAccountDetailDumpPathAll, "wt");
			CWriteableAccountDetail::writeCSVHead(m_AccountDetailfileLog);
			fflush(m_AccountDetailfileLog);
		}

		if (m_SettleDetailfileLog != NULL)
		{
			fclose(m_SettleDetailfileLog);
			sprintf(m_sSettleDetailDumpPathAll, "%sSettleDetail.csv", m_sDumpTmpPath);
			m_SettleDetailfileLog = mfopen(m_sSettleDetailDumpPathAll, "wt");
			CWriteableSettleDetail::writeCSVHead(m_SettleDetailfileLog);
			fflush(m_SettleDetailfileLog);
		}

		if (m_FinishTriggerOrderfileLog != NULL)
		{
			fclose(m_FinishTriggerOrderfileLog);
			sprintf(m_sFinishTriggerOrderDumpPathAll, "%sFinishTriggerOrder.csv", m_sDumpTmpPath);
			m_FinishTriggerOrderfileLog = mfopen(m_sFinishTriggerOrderDumpPathAll, "wt");
			CWriteableTriggerOrder::writeCSVHead(m_FinishTriggerOrderfileLog);
			fflush(m_FinishTriggerOrderfileLog);
		}

		if (m_FinishPositionfileLog != NULL)
		{
			fclose(m_FinishPositionfileLog);
			sprintf(m_sFinishPositionDumpPathAll, "%sFinishPosition.csv", m_sDumpTmpPath);
			m_FinishPositionfileLog = mfopen(m_sFinishPositionDumpPathAll, "wt");
			CWriteablePosition::writeCSVHead(m_FinishPositionfileLog);
			fflush(m_FinishPositionfileLog);
		}

		return true;
	}

	virtual bool SimplePrepareMetaRef(CFieldDescribe* pFieldDescribe, char Type)
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

	void SendDelay()
	{
		memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Reserve[0] = OUTFOG_RESERVE_SYNC_YES;
		m_OutFogMetaRef.Reserve[1] = OUTFOG_RESERVE_SYNC_YES;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_UPDATE;

		{
			set<CAccount*>::iterator iter = m_DelayAccountSet.begin();
			while (iter != m_DelayAccountSet.end())
			{
				m_OutFogMetaRef.SessionNo = CAccountField::m_Describe.m_FieldID;
				m_OutFogMetaRef.FieldNo = CAccountField::m_Describe.m_FieldID;
				m_OutFogMetaRef.Length = CAccountField::m_Describe.m_nStreamSize;
				CAccountField field;
				CopyAccountEntity(&field, ((CAccount*)(*iter)));
				CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
				if (pMember == NULL)
				{
					iter++;
					continue;
				}

				strcpy(m_OutFogMetaRef.SubIndexName, field.MemberID.getValue());
				int nDispatchNo = field.MemberID.getValue()[strlen(field.MemberID.getValue()) - 1] - '0';
				if (nDispatchNo < 0)
					nDispatchNo = -nDispatchNo;
				nDispatchNo = abs(nDispatchNo);
				m_OutFogMetaRef.MessageNo = TID_PushAccount;
				m_OutFogMetaRef.RequestNo = 5 + nDispatchNo % 10;
				m_OutFogMetaRef.BusinessNo = field.BusinessNo;
				CAccountField::m_Describe.setSingleField(m_pStreamBuf, &field);

				sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_Risk, OF_Topic_Private_SnapShot);
				m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;

				m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
				iter++;
			}
			m_DelayAccountSet.clear();
		}

		{
			set<CPosition*>::iterator iter = m_DelayPositionSet.begin();
			while (iter != m_DelayPositionSet.end())
			{
				m_OutFogMetaRef.SessionNo = CPositionField::m_Describe.m_FieldID;
				m_OutFogMetaRef.FieldNo = CPositionField::m_Describe.m_FieldID;
				m_OutFogMetaRef.Length = CPositionField::m_Describe.m_nStreamSize;
				CPositionField field;
				CopyPositionEntity(&field, ((CPosition*)(*iter)));
				CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(field.MemberID);
				if (pMember == NULL)
				{
					iter++;
					continue;
				}
				strcpy(m_OutFogMetaRef.SubIndexName, field.MemberID.getValue());
				int nDispatchNo = field.MemberID.getValue()[strlen(field.MemberID.getValue()) - 1] - '0';
				if (nDispatchNo < 0)
					nDispatchNo = -nDispatchNo;
				nDispatchNo = abs(nDispatchNo);
				m_OutFogMetaRef.MessageNo = TID_PushPosition;
				m_OutFogMetaRef.RequestNo = 15 + nDispatchNo % 10;
				m_OutFogMetaRef.BusinessNo = field.BusinessNo;
				CPositionField::m_Describe.setSingleField(m_pStreamBuf, &field);
				m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
				if (field.CopyMemberID.isNull())
					sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_Risk, OF_Topic_Private_SnapShot);
				else
					sprintf(m_OutFogMetaRef.Remark, "%d,%d,%d", OF_Topic_Private_Risk, OF_Topic_Private_SnapShot, OF_Topic_Private_Copy_Finish);
				m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
				iter++;
			}
			m_DelayPositionSet.clear();
		}
	}

	virtual bool PrepareMetaRef(CFieldDescribe* pFieldDescribe, void* pData, void* pOldData, char Type)
	{
		SimplePrepareMetaRef(pFieldDescribe, Type);

		switch (pFieldDescribe->m_FieldID)
		{
		case FID_Order:
		{
			//订单的变化一定需要落redis，后续等做市完善了，考虑不落redis了，减少交互
			//对于落DB，由于DB是快照启动的，删除的订单一定要及时更新,如果其插入时间在m_nCurrTime之前，是一定要删除的
			//if (m_OutFogMetaRef.Type == OUTFOG_TYPE_DELETE && ((COrder*)pData)->InsertTime > m_nFirstTime)
			//	m_OutFogMetaRef.ServiceNo = -1;

			//if (!needSyncDB(m_pMDB, ((COrder*)pData)->getMember()))
			//	return false;

			if (((COrder*)pData)->getMember() == NULL)
				return false;
			if (((COrder*)pData)->getMember()->MemberType == PT_InnerMarketMaker)
				return false;

			//资金相关的订单不需要同步
			//if (((COrder*)pData)->DeriveSource == DS_FromAccountAction)
			//	return false;

			m_OutFogMetaRef.MessageNo = TID_PushOrder;
			strcpy(m_OutFogMetaRef.SubIndexName, ((COrder*)pData)->MemberID);
			//strcpy(m_OutFogMetaRef.UserID, ((COrder*)pData)->UserID);
			m_OutFogMetaRef.BusinessNo = ((COrder*)pData)->BusinessNo;

			if (((COrder*)pData)->CopyMemberID.isNull())
			{
				m_OutFogMetaRef.SubjectNo = OF_Topic_Private_Order;
			}
			else
			{
				sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_Order, OF_Topic_Private_Copy_Finish);
				m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
			}

			//如果是成交或者取消，直接发送删除报文
			if (((COrder*)pData)->OrderStatus == OS_PartCanceled || ((COrder*)pData)->OrderStatus == OS_AllCanceled || ((COrder*)pData)->OrderStatus == OS_AllTraded)
			{
				//time_t tm = ((COrder*)pData)->UpdateTime.getInt();
				//struct tm timeinfo = GetTM(&tm);
				//strftime(((CWriteableOrder*)pData)->CreateTime.getBuffer(), 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
				m_OutFogMetaRef.Type = OUTFOG_TYPE_DELETE;
			}
			break;
		}
		case FID_Trade:
		{
			//成交只落库，不做推送，Order的推送能解决显示问题
			//先判断是否对手方是做市商，只有对手方是做市商才不落地
			//因为返佣等下游系统通过两个TradeID确定事务，如果只落一遍，导致事务无法合并
			//做市自成交不落库
			//CMember * pMember = m_pMDB->m_MemberFactory->findByMemberID(((CTrade*)pData)->MemberID);
			//if (needSyncDB(m_pMDB, pMember))
			{
				m_OutFogMetaRef.MessageNo = TID_PushTrade;
				strcpy(m_OutFogMetaRef.SubIndexName, ((CTrade*)pData)->MemberID);
				//strcpy(m_OutFogMetaRef.UserID, ((CTrade*)pData)->UserID);
				m_OutFogMetaRef.BusinessNo = ((CTrade*)pData)->BusinessNo;
				m_OutFogMetaRef.SubjectNo = OF_Topic_Private_Trade;
				//time_t tm = ((CTrade*)pData)->InsertTime.getInt();
				//struct tm timeinfo = GetTM(&tm);
				//strftime(((CWriteableTrade*)pData)->CreateTime.getBuffer(), 20, "%Y-%m-%d %H:%M:%S", &timeinfo);

				if (m_TradefileLog != NULL)
				{
					((CTrade*)pData)->writeCSV(m_TradefileLog);
					fflush(m_TradefileLog);
				}
			}
			//	else
			//	{
					////做市不落库的情况下，只有自成交才不落库，其他需要落库
					//if (((CTrade*)pData)->IsSelfTrade)
					//	return false;

					//m_OutFogMetaRef.MessageNo = OUTFOG_TNO_NULL;
					//m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_NULL;
					//time_t tm = ((CTrade*)pData)->InsertTime.getInt();
					//struct tm timeinfo = GetTM(&tm);
					//strftime(((CWriteableTrade*)pData)->CreateTime.getBuffer(), 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
					//if (m_TradefileLog != NULL)
					//{
					//	((CTrade*)pData)->writeCSV(m_TradefileLog);
					//	fflush(m_TradefileLog);
					//}
			//	}
			break;
		}
		case FID_AccountDetail:
		{
			//if (!needSyncDB(m_pMDB, ((CAccountDetail*)pData)->MemberID))
			//	return false;
			//财务纪律需要落库，要不然会导致财务记录不连续
			//time_t tm = ((CAccountDetail*)pData)->InsertTime.getInt();
			//struct tm timeinfo = GetTM(&tm);
			//strftime(((CWriteableAccountDetail*)pData)->CreateTime.getBuffer(), 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
			if (m_AccountDetailfileLog != NULL)
			{
				((CAccountDetail*)pData)->writeCSV(m_AccountDetailfileLog);
				fflush(m_AccountDetailfileLog);
			}
			break;
		}
		case FID_SettleDetail:
		{
			if (m_SettleDetailfileLog != NULL)
			{
				((CSettleDetail*)pData)->writeCSV(m_SettleDetailfileLog);
				fflush(m_SettleDetailfileLog);
			}
			break;
		}
		case FID_Position:
		{
			//冻结持仓不发送，持仓归零需要发送
			//if (m_OutFogMetaRef.Type != OUTFOG_TYPE_DELETE)
			//{
			//	//有持仓的情况下才动
			//	if (((CPosition*)pData)->Position == 0 && ((CPosition*)pOldData)->Position == 0)
			//		return false;
			//	//if (((CPosition*)pData)->Position == ((CPosition*)pOldData)->Position)
			//	//{
			//	//	if (((CPosition*)pData)->SLTriggerPrice == ((CPosition*)pOldData)->SLTriggerPrice && ((CPosition*)pData)->TPTriggerPrice == ((CPosition*)pOldData)->TPTriggerPrice)
			//	//		return false;
			//	//}			
			//}

			//if (m_OutFogMetaRef.Type == OUTFOG_TYPE_UPDATE)

			//CMember * pMember = m_pMDB->m_MemberFactory->findByMemberID(((CPosition*)pData)->MemberID);
			//if (!needSyncDB(m_pMDB, pMember))
			//	return false;

			if (pOldData == pData || ((CPosition*)pOldData)->UpdateTime != ((CPosition*)pData)->UpdateTime || ((CPosition*)pData)->Frequency.isNull() || ((CPosition*)pOldData)->Position != ((CPosition*)pData)->Position)
			{
				((CWriteablePosition*)pData)->Frequency = 1;
				set<CPosition*>::iterator iter = m_DelayPositionSet.find((CPosition*)pData);
				if (iter != m_DelayPositionSet.end())
					m_DelayPositionSet.erase(iter);
			}
			else
			{
				if (((CPosition*)pData)->Frequency > 5)
				{
					m_DelayPositionSet.insert((CPosition*)pData);
					return false;
				}
				else
				{
					((CWriteablePosition*)pData)->Frequency = ((CPosition*)pData)->Frequency + 1;
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
			m_OutFogMetaRef.BusinessNo = ((CPosition*)pData)->BusinessNo;
			//strcpy(m_OutFogMetaRef.UserID, ((CPosition*)pData)->UpdateUserID);
			//10000000 以上不给风控

			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
			if (((CPosition*)pData)->CopyMemberID.isNull())
				sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_Risk, OF_Topic_Private_SnapShot);
			else
				sprintf(m_OutFogMetaRef.Remark, "%d,%d,%d", OF_Topic_Private_Risk, OF_Topic_Private_SnapShot, OF_Topic_Private_Copy_Finish);

			if (((CPosition*)pOldData)->Position != 0 && ((CPosition*)pData)->Position == 0.0 && ((CPosition*)pData)->ShortFrozen == 0.0 && ((CPosition*)pData)->LongFrozen == 0.0)
			{
				//与内存删除逻辑保持一致
				if (!((CPosition*)pData)->TradeUnitID.like(((CPosition*)pData)->AccountID))
					m_OutFogMetaRef.Type = OUTFOG_TYPE_DELETE;
				//time_t tm = ((COrder*)pData)->UpdateTime.getInt();
				//struct tm timeinfo = GetTM(&tm);
				//strftime(((CWriteableOrder*)pData)->CreateTime.getBuffer(), 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
			}
			break;
		}
		case FID_PositionOrder:
		{
			//if (m_OutFogMetaRef.Type == OUTFOG_TYPE_UPDATE)
			//if (!needSyncDB(m_pMDB, ((CPositionOrder*)pData)->MemberID))
			//	return false;

			m_OutFogMetaRef.MessageNo = OUTFOG_TNO_NULL;
			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_NULL;
			break;
		}
		case FID_Account:
		{
			//if (m_OutFogMetaRef.Type == OUTFOG_TYPE_UPDATE)
			//CMember * pMember = m_pMDB->m_MemberFactory->findByMemberID(((CAccount*)pData)->MemberID);
			//if (!needSyncDB(m_pMDB, pMember))
			//	return false;
			if (pOldData == pData || ((CAccount*)pOldData)->UpdateTime != ((CAccount*)pData)->UpdateTime || ((CAccount*)pData)->Frequency.isNull() || ((CAccount*)pOldData)->Balance != ((CAccount*)pData)->Balance)
			{
				((CWriteableAccount*)pData)->Frequency = 1;
				set<CAccount*>::iterator iter = m_DelayAccountSet.find((CAccount*)pData);
				if (iter != m_DelayAccountSet.end())
					m_DelayAccountSet.erase(iter);
			}
			else
			{
				if (((CAccount*)pData)->Frequency > 4)
				{
					m_DelayAccountSet.insert((CAccount*)pData);
					return false;
				}
				else
				{
					((CWriteableAccount*)pData)->Frequency = ((CAccount*)pData)->Frequency + 1;
				}
			}

			m_OutFogMetaRef.MessageNo = TID_PushAccount;
			strcpy(m_OutFogMetaRef.SubIndexName, ((CAccount*)pData)->MemberID);
			//strcpy(m_OutFogMetaRef.UserID, ((CAccount*)pData)->UpdateUserID);

			m_OutFogMetaRef.SubjectNo = OF_Topic_Private_SnapShot;
			sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_Risk, OF_Topic_Private_SnapShot);
			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;

			break;
		}
		case FID_MemberPosition:
		{
			//由于客户端不被使用，这里不做同步
			return false;

			m_OutFogMetaRef.MessageNo = OUTFOG_TNO_NULL;
			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_NULL;
			break;
		}
		case FID_CurrentTime:
		{
			m_OutFogMetaRef.Reserve[0] = OUTFOG_RESERVE_SYNC_NO;
			m_OutFogMetaRef.Reserve[1] = OUTFOG_RESERVE_SYNC_NO;

			if (m_nFirstTime == 0)
				m_nFirstTime = ((CCurrentTime*)pData)->CurrTime.getValue();
			m_OutFogMetaRef.MessageNo = TID_PushTimeSync;
			m_OutFogMetaRef.Type = OUTFOG_TYPE_TIMER;
			strcpy(m_OutFogMetaRef.SubIndexName, ((CCurrentTime*)pData)->ZoneDate.getValue());
			strcpy(m_OutFogMetaRef.UserID, ((CCurrentTime*)pData)->ZoneTime.getValue());
			strcpy(m_OutFogMetaRef.Remark, ((CCurrentTime*)pData)->ZoneDays.getValue());
			//m_OutFogMetaRef.SessionNo = ((CCurrentTime*)pData)->CurrTime.getValue();
			m_OutFogMetaRef.BusinessNo = ((CCurrentTime*)pData)->CurrTime.getValue();
			m_OutFogMetaRef.RequestNo = ((CCurrentTime*)pData)->UpdateTime.getValue();
			m_OutFogMetaRef.Reserve[0] = '3';

			//field.ZoneDate = pOutFogReqRef->SubIndexName;
			//field.ZoneTime = pOutFogReqRef->UserID;
			//field.ZoneDays = pOutFogReqRef->Remark;
			//field.CurrMillisec = pOutFogReqRef->RequestNo;
			//field.CurrTime = pOutFogReqRef->SessionNo;

			if (((CCurrentTime*)pData)->CurrTime >= (m_nLastTime + g_nPushTimeSeconds))
			{
				sprintf(m_OutFogMetaRef.Remark, "%d,%d,%d", OF_Topic_Private_Risk, OF_Topic_Market_Broad, OF_Topic_Market_Depth_All);
				m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
				m_nLastTime = ((CCurrentTime*)pData)->CurrTime.getValue();
			}
			else
			{
				sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_Risk, OF_Topic_Market_Depth_All);
				m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
				m_OutFogMetaRef.SubjectNo = OF_Topic_Market_Depth_All;
			}
			//推送累计的订单

			break;
		}
		case FID_MarketData:
		{
			//m_OutFogMetaRef.MessageNo = TID_PushMarketData;
			//sprintf(m_OutFogMetaRef.SubIndexName,"%s:%s",((CMarketData*)pData)->ExchangeID.getValue(),((CMarketData*)pData)->InstrumentID.getValue());

			//redis 1s更新一次，数据库10s更新一次
			m_OutFogMetaRef.Reserve[1] = OUTFOG_RESERVE_SYNC_YES;
			if (((CMarketData*)pData)->UpdateTime / 1000 == ((CMarketData*)pOldData)->UpdateTime / 1000)
				return false;
			if (fmod(((CMarketData*)pData)->UpdateTime.getValue() / 1000, 10) != 0)
				m_OutFogMetaRef.Reserve[0] = OUTFOG_RESERVE_SYNC_NO;

			m_OutFogMetaRef.MessageNo = OUTFOG_TNO_NULL;
			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_NULL;
			break;
		}
		//case FID_User:
		//{
		//	m_OutFogMetaRef.MessageNo = OUTFOG_TNO_NULL;
		//	m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_NULL;
		//	break;
		//}
		case FID_LastKLine:
		{
			m_OutFogMetaRef.MessageNo = TID_PushKLine;
			m_OutFogMetaRef.SubjectNo = OF_Topic_Market_LastKLine;

			char* nextBegin = fastStrcat(m_OutFogMetaRef.SubIndexName, ((CLastKLine*)pData)->ExchangeID.getValue());
			nextBegin = fastStrcat(nextBegin, ((CLastKLine*)pData)->InstrumentID.getValue(), '_');
			nextBegin = fastStrcat(nextBegin, ((CLastKLine*)pData)->Bar.getValue(), '_');

			if (pOldData != NULL && ((CLastKLine*)pOldData)->InsertTime < ((CLastKLine*)pData)->InsertTime)
			{
				if (m_KLinefileLog != NULL)
				{
					((CKLine*)pOldData)->writeCSV(m_KLinefileLog);
					fflush(m_KLinefileLog);
				}

				m_OutFogMetaRef.RequestNo = 118;
				m_OutFogMetaRef.Type = OUTFOG_TYPE_INSERT;
				m_OutFogMetaRef.SessionNo = CKLineField::m_Describe.m_FieldID;
				m_OutFogMetaRef.FieldNo = CKLineField::m_Describe.m_FieldID;
				m_OutFogMetaRef.Reserve[0] = OUTFOG_RESERVE_SYNC_YES;

				//老的KLine入库，并且入库
				CKLineField field;
				CopyKLineEntity(&field, ((CLastKLine*)pOldData));
				CKLineField::m_Describe.setSingleField(m_pStreamBuf, &field);
				m_OutFogMetaRef.SubjectNo = OF_Topic_Market_KLine;
				m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);

				//老的LastKLine需要发送，要不然无法
				CLastKLineField field1;
				CopyLastKLineEntity(&field1, ((CLastKLine*)pOldData));
				CLastKLineField::m_Describe.setSingleField(m_pStreamBuf, &field1);
				m_OutFogMetaRef.SubjectNo = OF_Topic_Market_LastKLine;
				m_OutFogMetaRef.Reserve[0] = OUTFOG_RESERVE_SYNC_NO;
				m_OutFogMetaRef.FieldNo = CLastKLineField::m_Describe.m_FieldID;
				m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);

				//重新让后续发送最新的LastKLine
				m_OutFogMetaRef.RequestNo = 0;
				m_OutFogMetaRef.Type = OUTFOG_TYPE_UPDATE;
				m_OutFogMetaRef.FieldNo = CLastKLineField::m_Describe.m_FieldID;
			}
			else
			{
				//一分钟K线一秒发送最多发送5次
				if (pOldData == NULL)
				{
					m_nLastKLineTimes = 0;
				}
				else
				{
					if (((CLastKLine*)pOldData)->UpdateTime / 1000 != ((CLastKLine*)pData)->UpdateTime / 1000)
						m_nLastKLineTimes = 0;
				}
				m_nLastKLineTimes++;
				if (m_nLastKLineTimes > 3)
					return false;
				m_OutFogMetaRef.Reserve[0] = OUTFOG_RESERVE_SYNC_NO;
			}
			break;
		}
		//case FID_KLine:
		//{
		//	//list1:K线可以任意分钟数目的，所以这里发送特定时间的K线意义不大
		//	//一分钟K线需要发送，要不然会由于定时发送的缺陷，扫掉最后一根K线
		//	//next:由于需要前端切换的时候直接获取最后的K线避免再去查找，这里将需要的K线进行推送
		//	if (m_KLinefileLog != NULL)
		//	{
		//		((CKLine*)pData)->writeCSV(m_KLinefileLog);
		//		fflush(m_KLinefileLog);
		//	}
		//	m_OutFogMetaRef.MessageNo = OUTFOG_TNO_NULL;
		//	m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_NULL;


		//	//基础K线是：1m 1h 1d 1w 1o
		//	//落库K线为：1m 1h 1d 1w 1o 5m 4h 1y
		//	//支持K线为：1m 1h 1d 1w 1o 5m 4h 1y 15m 30m 12h
		//	//不落库的有：15m 30m 12h
		//	//if (!(((CKLine*)pData)->PeriodID.getValue()[0] == '1' || ((CKLine*)pData)->PeriodID == "5m" || ((CKLine*)pData)->PeriodID == "4h"))
		//	//{
		//	//	m_OutFogMetaRef.FrontNo = REDIS_MAX_COUNT;
		//	//}
		//	//m_OutFogMetaRef.MessageNo = TID_PushKLine;
		//	//m_OutFogMetaRef.SubjectNo = OF_Topic_Market_KLine;
		//	//sprintf(m_OutFogMetaRef.SubIndexName, "%s_%s_%s", ((CKLine*)pData)->ExchangeID.getValue(), ((CKLine*)pData)->InstrumentID.getValue(), ((CKLine*)pData)->PeriodID.getValue());
		//	break;
		//}
		case FID_UserSession:
		{
			if (((CUserSession*)pData)->SinkType == ST_NoSink)
				return false;


			//if (((CAllow*)pData)->AccessType.getValue() == 'c') //APIKey类型
			//{
			//	if (Type == OUTFOG_TYPE_INSERT)
			//	{
			//		if (!strcmp(((CAllow*)pData)->AccessName.getValue(), "IPAddress"))
			//		{
			//			AddApiKeyAllowIP(((CAllow*)pData)->AllowID.getValue(), ((CAllow*)pData)->AccessValue.getValue());
			//			SimplePrepareMetaRef(pFieldDescribe, Type);
			//		}
			//	}
			//	else if (Type == OUTFOG_TYPE_DELETE)
			//	{
			//		if (!strcmp(((CAllow*)pData)->AccessName.getValue(), "IPAddress"))
			//		{
			//			DelApiKeyAllowIP(((CAllow*)pData)->AllowID.getValue(), ((CAllow*)pData)->AccessValue.getValue());
			//			SimplePrepareMetaRef(pFieldDescribe, Type);
			//		}
			//	}
			//}

			//UserSession不落数据库
			m_OutFogMetaRef.Reserve[0] = OUTFOG_RESERVE_SYNC_NO;
			m_OutFogMetaRef.Reserve[1] = OUTFOG_RESERVE_SYNC_YES;
			m_OutFogMetaRef.MessageNo = TID_PushUserSession;
			m_OutFogMetaRef.SubjectNo = OF_Topic_Private_UserSession;
			break;
		}
		//case FID_Allow:
		//{
		//	if (((CAllow*)pData)->AccessType.getValue() == 'c') //APIKey类型
		//	{
		//		if (Type == OUTFOG_TYPE_INSERT)
		//		{
		//			if (!strcmp(((CAllow*)pData)->AccessName.getValue(), "IPAddress"))
		//			{
		//				AddApiKeyAllowIP(((CAllow*)pData)->AllowID.getValue(), ((CAllow*)pData)->AccessValue.getValue());
		//				SimplePrepareMetaRef(pFieldDescribe, Type);
		//			}
		//		}
		//		else if (Type == OUTFOG_TYPE_DELETE)
		//		{
		//			if (!strcmp(((CAllow*)pData)->AccessName.getValue(), "IPAddress"))
		//			{
		//				DelApiKeyAllowIP(((CAllow*)pData)->AllowID.getValue(), ((CAllow*)pData)->AccessValue.getValue());
		//				SimplePrepareMetaRef(pFieldDescribe, Type);
		//			}
		//		}
		//	}
		//	m_OutFogMetaRef.MessageNo = OUTFOG_TNO_NULL;
		//	m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_NULL;
		//	break;
		//}
		case FID_ServiceConfig:
		{
			m_OutFogMetaRef.MessageNo = OUTFOG_TNO_NULL;
			m_OutFogMetaRef.SubjectNo = OF_Topic_Private_UserSession;

			CLongNameType ConfigValue = ((CServiceConfig*)pData)->ConfigValue;
			if (Type == OUTFOG_TYPE_DELETE)
				ConfigValue.clear();

			if (((CServiceConfig*)pData)->ConfigName == "CFDTaker")
			{
				g_sCFDTaker = ConfigValue.getValue();
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "KLineGap")
			{
				g_bKLineGap = ConfigValue.getValue()[0] - '0';
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "DoubleMarketVolume")
			{
				g_bDoubleMarketVolume = ConfigValue.getValue()[0] - '0';
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "SessionExpireTime")
			{
				g_nSessionExpireTime = atoi(ConfigValue.getValue());
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "BaseClearCurrency")
			{
				g_sBaseClearCurrency = ConfigValue.getValue();
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "MaxTriggerOrders")
			{
				g_nMaxTriggerOrders = atoi(ConfigValue.getValue());
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "MaxOpenOrders")
			{
				g_nMaxOpenOrders = atoi(ConfigValue.getValue());
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "MaxOpenPositions")
			{
				g_nMaxOpenPositions = atoi(ConfigValue.getValue());
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "PushTimeSeconds")
			{
				g_nPushTimeSeconds = atoi(ConfigValue.getValue());
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "NoTradeFinishTo")
			{
				g_bNoTradeFinishTo = ConfigValue.getValue()[0] - '0';
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "OrderPriceType")
			{
				g_cOrderPriceType = ConfigValue.getValue()[0];
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "LoanInterestFeeRate")
			{
				g_dLoanInterestFeeRate = atof(ConfigValue.getValue());
				break;
			}
			if (((CServiceConfig*)pData)->ConfigName == "CopyProfitAccountID")
			{
				g_sCopyProfitAccountID = ConfigValue.getValue();
				break;
			}

			break;
		}
		case FID_Member:
		{
			if (((CMember*)pData)->SinkType == ST_NoSink)
				return false;
			m_OutFogMetaRef.MessageNo = TID_PushMember;
			m_OutFogMetaRef.SubjectNo = OF_Topic_Private_Risk;
			break;
		}
		case FID_Currency:
		{
			m_OutFogMetaRef.MessageNo = TID_PushCurrency;
			//m_OutFogMetaRef.SubjectNo = OF_Topic_Private_Risk;
			sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_Risk, OF_Topic_Market_Broad);
			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
			break;
		}
		case FID_MarginRate:
		{
			m_OutFogMetaRef.MessageNo = TID_PushMarginRate;
			m_OutFogMetaRef.SubjectNo = OF_Topic_Private_Risk;
			break;
		}
		case FID_Fee:
		{
			m_OutFogMetaRef.MessageNo = TID_PushFee;
			m_OutFogMetaRef.SubjectNo = OF_Topic_Private_Risk;
			break;
		}
		case FID_TriggerOrder:
		{
			m_OutFogMetaRef.MessageNo = TID_PushTriggerOrder;
			strcpy(m_OutFogMetaRef.SubIndexName, ((CTriggerOrder*)pData)->MemberID);
			//strcpy(m_OutFogMetaRef.UserID, ((CTriggerOrder*)pData)->UserID);
			//m_OutFogMetaRef.SubjectNo = OF_Topic_Private_TriggerOrder;
			sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_TriggerOrder, OF_Topic_Private_Order);
			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
			if (((CTriggerOrder*)pData)->TriggerStatus >= TS_TriggerSuccess)
			{
				//time_t tm = ((CTriggerOrder*)pData)->UpdateTime.getInt();
				//struct tm timeinfo = GetTM(&tm);
				//strftime(((CWriteableTriggerOrder*)pData)->CreateTime.getBuffer(), 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
				m_OutFogMetaRef.Type = OUTFOG_TYPE_DELETE;
			}
			break;
		}
		case FID_FinishOrder:
		case FID_FinishTriggerOrder:
		{
			return false;
		}
		case FID_Instrument:
		{
			m_OutFogMetaRef.MessageNo = TID_PushInstrument;
			if (Type == OUTFOG_TYPE_DELETE)
				((CWriteableInstrument*)pData)->SortNumber = -9;
			sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Private_Risk, OF_Topic_Market_Broad);
			m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
			m_OutFogMetaRef.Reserve[0] = '2';
			break;
		}
		case FID_MarketOrder:
		{
			m_OutFogMetaRef.MessageNo = TID_PushMarketOrder;

			if (Type == OUTFOG_TYPE_DELETE)
			{
				((CWriteableMarketOrder*)pData)->Volume = 0.0;
				((CWriteableMarketOrder*)pData)->Orders = 0;
				((CWriteableMarketOrder*)pData)->TrueVolume = 0.0;
			}

			m_OutFogMetaRef.SubjectNo = OF_Topic_Market_Depth_All;
			//sprintf(m_OutFogMetaRef.Remark, "%d,%d", OF_Topic_Market_Depth_All, OF_Topic_Market_Depth_25);

			//char * nextBegin = fastStrcat(m_OutFogMetaRef.Remark, S_Topic_Market_Depth_All);
			//nextBegin = fastStrcat(nextBegin, S_Topic_Market_Depth_25, ',');
			//m_OutFogMetaRef.SubjectNo = OUTFOG_SUBJECTID_MULTI;
			////sprintf(m_OutFogMetaRef.SubIndexName, "%s_%s", ((CMarketOrder*)pData)->ExchangeID.getValue(), ((CMarketOrder*)pData)->InstrumentID.getValue());
			//nextBegin = fastStrcat(m_OutFogMetaRef.SubIndexName, ((CMarketOrder*)pData)->ExchangeID.getValue());
			//nextBegin = fastStrcat(nextBegin, ((CMarketOrder*)pData)->InstrumentID.getValue(), '_');
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
	char m_sDumpPath[256];

	char m_sFinishOrderDumpPathAll[256];
	FILE* m_FinishOrderfileLog;

	char m_sTradeDumpPathAll[256];
	FILE* m_TradefileLog;

	char m_sKLineDumpPathAll[256];
	FILE* m_KLinefileLog;

	char m_sAccountDetailDumpPathAll[256];
	FILE* m_AccountDetailfileLog;

	char m_sSettleDetailDumpPathAll[256];
	FILE* m_SettleDetailfileLog;

	char m_sFinishTriggerOrderDumpPathAll[256];
	FILE* m_FinishTriggerOrderfileLog;

	char m_sFinishPositionDumpPathAll[256];
	FILE* m_FinishPositionfileLog;

	UF_INT8 m_nLastTime;
	UF_INT8 m_nFirstTime;

	CMDB* m_pMDB;

	//如果时间改过来了，这个需要删除
	int m_nLastKLineTimes;


	set<CAccount*> m_DelayAccountSet;
	set<CPosition*> m_DelayPositionSet;
};

#endif
