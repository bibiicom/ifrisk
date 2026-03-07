#ifndef Service_Share
#define Service_Share

#include "mdb.h"
#include "DataType.h"
#include "ServiceResponser.h"
#include "CErrorEngine.h"

// 分页查询的最大数量限制
extern int g_nPageMaxLimit;

void SendMarketOrder(CMDB* m_pMDB, CServiceResponser* pServiceResponser, CQryMarketOrderField& field, TSessionRef* pSession);
void SendTickMarketOrder(CMDB* m_pMDB, CServiceResponser* pServiceResponser, CQryMarketOrderField& field, TSessionRef* pSession);
void SendMarketData(CMDB* m_pMDB, CServiceResponser* pServiceResponser, CQryMarketDataField& field, TSessionRef* pSession);
void SendCurrency(CMDB* m_pMDB, CServiceResponser* pServiceResponser, CQryCurrencyField& field, TSessionRef* pSession);
void CurrencyValue(CMDB* m_pMDB, CCurrencyField* pField);
bool RspError(CErrorEngine* pErrorEngine, CServiceResponser* pServiceResponser, int nRecvTNo, TSessionRef* pSession, void* pField = NULL, CFieldDescribe* pFieldDescribe = NULL);
void RspMarketData(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, const char* ProductGroup, TSessionRef* pSession, int nTopicNo = OF_Topic_Market_OverView);
void RspMarketData(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, TSessionRef* pSession, int nTopicNo = OF_Topic_Market_OverView);
void RspInstrument(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, TSessionRef* pSession, int nTopicNo = OF_Topic_Market_Broad);
void RspCurrency(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, TSessionRef* pSession, int nTopicNo = OF_Topic_Market_Broad);


void RspMarketOrder(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, TSessionRef* pSession, int nTopicID = OF_Topic_Market_Depth_25);
bool RspMarketOrder(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, const char* pExchangeID, const char* pInstrumentID, const char* pTick, int nLevel, TSessionRef* pSession, int nTopicID = OF_Topic_Market_Depth_25);
bool RspMarketOrder(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, const char* pExchangeID, const char* pInstrumentID, int nLevel, TSessionRef* pSession, int nTopicID = OF_Topic_Market_Depth_25);
void RspKLine(CMDB* m_pMDB, CServiceResponser* m_pServiceResponser, TSessionRef* pSession);

#endif