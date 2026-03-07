#ifndef CMYAlterService_H
#define CMYAlterService_H

#include "AlterService.h"
#include "DataType.h"
#include "TableDefine.h"
#include "CSequence.h"

/////////////////////////////////////////////////////////////////////////
///CAlterService是一个完成全部数据同步工作的类，主要完成了出入金和
///保证金率调整等工作
///////////////////////////////////////////////////////////////////////// 
class CMyAlterService :public CAlterService
{
public:
	///构造方法
	///@param	pMDB	指定的内存数据库
	///@param	pErrorEngine	指定的错误引擎
	CMyAlterService(CMDB *pMDB, CErrorEngine *pErrorEngine) :CAlterService(pMDB, pErrorEngine)
	{
		TTinitTableInfo *pDayTableInfo = g_InitTableInfo;
		while (pDayTableInfo->pFieldDescribe != NULL)
		{
			m_mapCommandInfo[pDayTableInfo->pszTable] = pDayTableInfo;
			pDayTableInfo++;
		}
		m_gSequence.initValueByValue(1);
	}

public:

	///同步数据库表增加会员客户关系的实现
	///@param	pInsTradeUnit	要同步的增加会员客户关系信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败

	///同步数据库表增加保值额度的实现
	///@param	pInsHedgeDetail	要同步的增加保值额度信息
	///@param	pTransaction	本操作属于的事务
	///@return	true表示成功，false表示失败
//	virtual bool SyncInsHedgeDetailImpl(CWriteableHedgeDetail *pHedgeDetail,CTransaction *pTransaction);

	//全部上场设置接口
	//bool ReqDBCommand(CDBCommandField *pDBCommandField, char *pDbmtStream, int &nFieldID, TSessionRef* pSession, CTransaction *pTransaction);

	//资金实时接口需要链接
//	virtual bool SyncAccountImpl(int nTid,CWriteableAccount *pAccount,CTransaction *pTransaction);
//	virtual bool SyncBaseReserveAccountImpl(int nTid,CWriteableBaseReserveAccount *pBaseReserveAccount,CTransaction *pTransaction);

	virtual bool SyncFeeImpl(int nTid, CFeeField *pFee, CTransaction *pTransaction);
	virtual bool SyncInstrumentImpl(int nTid, CInstrumentField *pInstrument, CTransaction *pTransaction);
	virtual bool SyncMarketDataImpl(int nTid, CMarketDataField *pMarketData, CTransaction *pTransaction);
	virtual bool SyncLastKLineImpl(int nTid, CLastKLineField *pLastKLine, CTransaction *pTransaction);
	virtual bool SyncServiceConfigImpl(int nTid, CServiceConfigField *pServiceConfig, CTransaction *pTransaction);
	virtual bool SyncAccountImpl(int nTid, CAccountField *pAccount, CTransaction *pTransaction);
	virtual bool SyncMemberImpl(int nTid, CMemberField *pMember, CTransaction *pTransaction);
	virtual bool SyncOrderImpl(int nTid, COrderField *pOrder, CTransaction *pTransaction);
	virtual bool SyncTriggerOrderImpl(int nTid, CTriggerOrderField *pTriggerOrder, CTransaction *pTransaction);
	virtual bool SyncCurrencyImpl(int nTid, CCurrencyField *pCurrency, CTransaction *pTransaction);
	virtual bool SyncPositionImpl(int nTid, CPositionField *pPosition, CTransaction *pTransaction);
	virtual bool SyncUserSessionImpl(int nTid, CUserSessionField *pUserSession, CTransaction *pTransaction);

	virtual void afterSync(CTransaction* pTransaction);

protected:
	struct ltstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return strcmp(s1, s2) < 0;
		}
	};

	typedef map<const char *, TTinitTableInfo *, ltstr> CCommandInfoMap;
	CCommandInfoMap m_mapCommandInfo;
	char m_pDbmtStruct[10000];
	CSequence m_gSequence;
};

#endif
