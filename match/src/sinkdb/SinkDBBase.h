#ifndef _CSinkDBBase_H
#define _CSinkDBBase_H

#include "Config.h"
#include "SinkDBDispatcher.h"

extern bool g_bLoadCSV;

//这个类的出现就是为了能够自动生成代码，不影响主类
//所以这个类不能单独使用

class CSinkDBBase
{
public:
	CSinkDBBase(){};
	virtual ~CSinkDBBase(){};	
	bool DisPatch(const TOutFogMetaRef* pOutFogReqRef,const void *pStream)
	{
		switch(pOutFogReqRef->SessionNo)
		{
		case FID_ServiceConfig:
			{
				CServiceConfigField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CServiceConfigField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_Currency:
			{
				CCurrencyField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CCurrencyField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_MarketData:
			{
				CMarketDataField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CMarketDataField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_Instrument:
			{
				CInstrumentField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CInstrumentField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_CurrentTime:
			{
				CCurrentTimeField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CCurrentTimeField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_Member:
			{
				CMemberField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CMemberField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_UserSession:
			{
				CUserSessionField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CUserSessionField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_LastKLine:
			{
				CLastKLineField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CLastKLineField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_CFDRate:
			{
				CCFDRateField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CCFDRateField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_TradingRight:
			{
				CTradingRightField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CTradingRightField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_MarginRate:
			{
				CMarginRateField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CMarginRateField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_Fee:
			{
				CFeeField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CFeeField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_Liquidity:
			{
				CLiquidityField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CLiquidityField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_Account:
			{
				CAccountField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CAccountField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_MemberPosition:
			{
				CMemberPositionField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CMemberPositionField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_Position:
			{
				CPositionField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CPositionField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_Order:
			{
				COrderField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(COrderField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_TriggerOrder:
			{
				CTriggerOrderField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CTriggerOrderField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_PositionOrder:
			{
				CPositionOrderField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CPositionOrderField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_FinishOrder:
			{
				CFinishOrderField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CFinishOrderField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_Trade:
			{
				CTradeField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CTradeField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_FinishPosition:
			{
				CFinishPositionField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CFinishPositionField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_AccountDetail:
			{
				CAccountDetailField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CAccountDetailField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_KLine:
			{
				CKLineField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CKLineField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_SettleDetail:
			{
				CSettleDetailField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CSettleDetailField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		case FID_FinishTriggerOrder:
			{
				CFinishTriggerOrderField::m_Describe.getSingleField(m_pBuffer, pStream);
				m_pSinkDBDispatcher->Dispatch(pOutFogReqRef->RequestNo, pOutFogReqRef->GlobalNo, &(CFinishTriggerOrderField::m_Describe),m_pBuffer,m_ActionType);
				break;
			}
		default:
			{

			}
		}
		return true;
	}

	bool DisPatch(CFieldDescribe *pFieldDesc, const void *pRspBuffer)
	{
		//过滤从csvload进来的数据，不需要入库filter的数据
		switch(pFieldDesc->m_FieldID)
		{
			case FID_Position:
			{
				CPositionField* pField = (CPositionField*)pRspBuffer;
				if(false)
					return true;
				break;
			}			
		default:
			{
			}
		}
		
		m_ActionType = TSL_INSERT;
		switch(pFieldDesc->m_FieldID)
		{
			case FID_ServiceConfig:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CServiceConfigField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_Currency:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CCurrencyField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_MarketData:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CMarketDataField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_Instrument:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CInstrumentField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_CurrentTime:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CCurrentTimeField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_Member:
			{
				int nDispatchNo = ((CMemberField*)pRspBuffer)->MemberID.getValue()[strlen(((CMemberField*)pRspBuffer)->MemberID.getValue())-1] - '0';
				nDispatchNo = abs(nDispatchNo);
				int RequestNo = 5 + nDispatchNo % 10;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CMemberField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_UserSession:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CUserSessionField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_LastKLine:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CLastKLineField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_CFDRate:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CCFDRateField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_TradingRight:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CTradingRightField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_MarginRate:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CMarginRateField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_Fee:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CFeeField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_Liquidity:
			{
				int RequestNo = 1;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CLiquidityField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_Account:
			{
				int nDispatchNo = ((CAccountField*)pRspBuffer)->MemberID.getValue()[strlen(((CAccountField*)pRspBuffer)->MemberID.getValue())-1] - '0';
				nDispatchNo = abs(nDispatchNo);
				int RequestNo = 5 + nDispatchNo % 10;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CAccountField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_MemberPosition:
			{
				int RequestNo = 15;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CMemberPositionField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_Position:
			{
				int nDispatchNo = ((CPositionField*)pRspBuffer)->MemberID.getValue()[strlen(((CPositionField*)pRspBuffer)->MemberID.getValue())-1] - '0';
				nDispatchNo = abs(nDispatchNo);
				int RequestNo = 15 + nDispatchNo % 10;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CPositionField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_Order:
			{
				int nDispatchNo = ((COrderField*)pRspBuffer)->OrderID.getValue()[strlen(((COrderField*)pRspBuffer)->OrderID.getValue())-1] - '0';
				nDispatchNo = abs(nDispatchNo);
				int RequestNo = 25 + nDispatchNo % 10;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(COrderField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_TriggerOrder:
			{
				int RequestNo = 2;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CTriggerOrderField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_PositionOrder:
			{
				int nDispatchNo = ((CPositionOrderField*)pRspBuffer)->OrderID.getValue()[strlen(((CPositionOrderField*)pRspBuffer)->OrderID.getValue())-1] - '0';
				nDispatchNo = abs(nDispatchNo);
				int RequestNo = 15 + nDispatchNo % 10;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CPositionOrderField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_FinishOrder:
			{
				int nDispatchNo = ((CFinishOrderField*)pRspBuffer)->OrderID.getValue()[strlen(((CFinishOrderField*)pRspBuffer)->OrderID.getValue())-1] - '0';
				nDispatchNo = abs(nDispatchNo);
				int RequestNo = 100 + nDispatchNo % 10;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CFinishOrderField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_Trade:
			{
				int nDispatchNo = ((CTradeField*)pRspBuffer)->TradeID.getValue()[strlen(((CTradeField*)pRspBuffer)->TradeID.getValue())-1] - '0';
				nDispatchNo = abs(nDispatchNo);
				int RequestNo = 110 + nDispatchNo % 10;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CTradeField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_FinishPosition:
			{
				int nDispatchNo = ((CFinishPositionField*)pRspBuffer)->PositionID.getValue()[strlen(((CFinishPositionField*)pRspBuffer)->PositionID.getValue())-1] - '0';
				nDispatchNo = abs(nDispatchNo);
				int RequestNo = 110 + nDispatchNo % 10;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CFinishPositionField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_AccountDetail:
			{
				int nDispatchNo = ((CAccountDetailField*)pRspBuffer)->AccountDetailID.getValue()[strlen(((CAccountDetailField*)pRspBuffer)->AccountDetailID.getValue())-1] - '0';
				nDispatchNo = abs(nDispatchNo);
				int RequestNo = 120 + nDispatchNo % 10;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CAccountDetailField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_KLine:
			{
				int RequestNo = 130;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CKLineField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_SettleDetail:
			{
				int RequestNo = 130;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CSettleDetailField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
			case FID_FinishTriggerOrder:
			{
				int RequestNo = 130;
				m_pSinkDBDispatcher->Dispatch(RequestNo, -1, &(CFinishTriggerOrderField::m_Describe), pRspBuffer, m_ActionType);
				break;
			}
		default:
			{

			}
		}
		return true;
	}

protected:
	CSinkDBDispatcher *m_pSinkDBDispatcher;
	TSQLType m_ActionType;
	UF_INT8 m_nSeriesNo;
	char m_pBuffer[OUTFOG_MAX_PACKAGE_SIZE * 3];
};

#endif
