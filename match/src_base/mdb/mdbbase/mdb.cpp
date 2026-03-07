#include "mdb.h"
#include <map>
#include <string>
using namespace std;

#define MEMORYDB_ID			0
#define MAX_TABLE_COUNT		300				///引导块中每块放置的内存数据库表或者索引的数量

CMDB::CMDB()
{
	m_CurrentTimeFactory=NULL;
	m_MemberFactory=NULL;
	m_ServiceConfigFactory=NULL;
	m_UserSessionFactory=NULL;
	m_InstrumentFactory=NULL;
	m_TradingRightFactory=NULL;
	m_MarginRateFactory=NULL;
	m_CFDRateFactory=NULL;
	m_FeeFactory=NULL;
	m_CurrencyFactory=NULL;
	m_LiquidityFactory=NULL;
	m_AccountFactory=NULL;
	m_MemberPositionFactory=NULL;
	m_PositionFactory=NULL;
	m_MarketDataFactory=NULL;
	m_OrderFactory=NULL;
	m_PositionOrderFactory=NULL;
	m_TriggerOrderFactory=NULL;
	m_MarketOrderFactory=NULL;
	m_TickMarketOrderFactory=NULL;
	m_TradeFactory=NULL;
	m_KLineFactory=NULL;
	m_PublishKLineFactory=NULL;
	m_LastKLineFactory=NULL;
	m_AccountDetailFactory=NULL;
	m_SettleDetailFactory=NULL;
}
	
CMDB::~CMDB()
{
	if (m_CurrentTimeFactory!=NULL)
		delete m_CurrentTimeFactory;
	if (m_MemberFactory!=NULL)
		delete m_MemberFactory;
	if (m_ServiceConfigFactory!=NULL)
		delete m_ServiceConfigFactory;
	if (m_UserSessionFactory!=NULL)
		delete m_UserSessionFactory;
	if (m_InstrumentFactory!=NULL)
		delete m_InstrumentFactory;
	if (m_TradingRightFactory!=NULL)
		delete m_TradingRightFactory;
	if (m_MarginRateFactory!=NULL)
		delete m_MarginRateFactory;
	if (m_CFDRateFactory!=NULL)
		delete m_CFDRateFactory;
	if (m_FeeFactory!=NULL)
		delete m_FeeFactory;
	if (m_CurrencyFactory!=NULL)
		delete m_CurrencyFactory;
	if (m_LiquidityFactory!=NULL)
		delete m_LiquidityFactory;
	if (m_AccountFactory!=NULL)
		delete m_AccountFactory;
	if (m_MemberPositionFactory!=NULL)
		delete m_MemberPositionFactory;
	if (m_PositionFactory!=NULL)
		delete m_PositionFactory;
	if (m_MarketDataFactory!=NULL)
		delete m_MarketDataFactory;
	if (m_OrderFactory!=NULL)
		delete m_OrderFactory;
	if (m_PositionOrderFactory!=NULL)
		delete m_PositionOrderFactory;
	if (m_TriggerOrderFactory!=NULL)
		delete m_TriggerOrderFactory;
	if (m_MarketOrderFactory!=NULL)
		delete m_MarketOrderFactory;
	if (m_TickMarketOrderFactory!=NULL)
		delete m_TickMarketOrderFactory;
	if (m_TradeFactory!=NULL)
		delete m_TradeFactory;
	if (m_KLineFactory!=NULL)
		delete m_KLineFactory;
	if (m_PublishKLineFactory!=NULL)
		delete m_PublishKLineFactory;
	if (m_LastKLineFactory!=NULL)
		delete m_LastKLineFactory;
	if (m_AccountDetailFactory!=NULL)
		delete m_AccountDetailFactory;
	if (m_SettleDetailFactory!=NULL)
		delete m_SettleDetailFactory;
}
	
int CMDB::isA(char *objectType)
{
	if (!strcmp(objectType,"CMDB"))
		return 1;
	return CBaseObject::isA(objectType);
}

const char *CMDB::getType(void)
{
	return "CMDB";
}

int CMDB::init(int memorySize, int blockCount,IMemoryAllocator *pAllocator,bool reuse)
{
	if(pAllocator == NULL)
	{
		pAllocator = new CNormalMemoryAllocator(memorySize,blockCount);
		pAllocator->init(reuse);
	}
	if(!reuse)
	{
		map<string,int> indexMap;
		pMem=new CFixMem(sizeof(TableStruct),MAX_TABLE_COUNT,pAllocator,false,MEMORYDB_ID);
		TableStruct* pStru = NULL;
		
		m_CurrentTimeFactory=new CCurrentTimeFactory(this,pAllocator,&indexMap);
		if (m_CurrentTimeFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"CurrentTime");
		pStru->reuseID = m_CurrentTimeFactory->getReuseID();
				
		m_MemberFactory=new CMemberFactory(this,pAllocator,&indexMap);
		if (m_MemberFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"Member");
		pStru->reuseID = m_MemberFactory->getReuseID();
				
		m_ServiceConfigFactory=new CServiceConfigFactory(this,pAllocator,&indexMap);
		if (m_ServiceConfigFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"ServiceConfig");
		pStru->reuseID = m_ServiceConfigFactory->getReuseID();
				
		m_UserSessionFactory=new CUserSessionFactory(this,pAllocator,&indexMap);
		if (m_UserSessionFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"UserSession");
		pStru->reuseID = m_UserSessionFactory->getReuseID();
				
		m_InstrumentFactory=new CInstrumentFactory(this,pAllocator,&indexMap);
		if (m_InstrumentFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"Instrument");
		pStru->reuseID = m_InstrumentFactory->getReuseID();
				
		m_TradingRightFactory=new CTradingRightFactory(this,pAllocator,&indexMap);
		if (m_TradingRightFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"TradingRight");
		pStru->reuseID = m_TradingRightFactory->getReuseID();
				
		m_MarginRateFactory=new CMarginRateFactory(this,pAllocator,&indexMap);
		if (m_MarginRateFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"MarginRate");
		pStru->reuseID = m_MarginRateFactory->getReuseID();
				
		m_CFDRateFactory=new CCFDRateFactory(this,pAllocator,&indexMap);
		if (m_CFDRateFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"CFDRate");
		pStru->reuseID = m_CFDRateFactory->getReuseID();
				
		m_FeeFactory=new CFeeFactory(this,pAllocator,&indexMap);
		if (m_FeeFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"Fee");
		pStru->reuseID = m_FeeFactory->getReuseID();
				
		m_CurrencyFactory=new CCurrencyFactory(this,pAllocator,&indexMap);
		if (m_CurrencyFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"Currency");
		pStru->reuseID = m_CurrencyFactory->getReuseID();
				
		m_LiquidityFactory=new CLiquidityFactory(this,pAllocator,&indexMap);
		if (m_LiquidityFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"Liquidity");
		pStru->reuseID = m_LiquidityFactory->getReuseID();
				
		m_AccountFactory=new CAccountFactory(this,pAllocator,&indexMap);
		if (m_AccountFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"Account");
		pStru->reuseID = m_AccountFactory->getReuseID();
				
		m_MemberPositionFactory=new CMemberPositionFactory(this,pAllocator,&indexMap);
		if (m_MemberPositionFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"MemberPosition");
		pStru->reuseID = m_MemberPositionFactory->getReuseID();
				
		m_PositionFactory=new CPositionFactory(this,pAllocator,&indexMap);
		if (m_PositionFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"Position");
		pStru->reuseID = m_PositionFactory->getReuseID();
				
		m_MarketDataFactory=new CMarketDataFactory(this,pAllocator,&indexMap);
		if (m_MarketDataFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"MarketData");
		pStru->reuseID = m_MarketDataFactory->getReuseID();
				
		m_OrderFactory=new COrderFactory(this,pAllocator,&indexMap);
		if (m_OrderFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"Order");
		pStru->reuseID = m_OrderFactory->getReuseID();
				
		m_PositionOrderFactory=new CPositionOrderFactory(this,pAllocator,&indexMap);
		if (m_PositionOrderFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"PositionOrder");
		pStru->reuseID = m_PositionOrderFactory->getReuseID();
				
		m_TriggerOrderFactory=new CTriggerOrderFactory(this,pAllocator,&indexMap);
		if (m_TriggerOrderFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"TriggerOrder");
		pStru->reuseID = m_TriggerOrderFactory->getReuseID();
				
		m_MarketOrderFactory=new CMarketOrderFactory(this,pAllocator,&indexMap);
		if (m_MarketOrderFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"MarketOrder");
		pStru->reuseID = m_MarketOrderFactory->getReuseID();
				
		m_TickMarketOrderFactory=new CTickMarketOrderFactory(this,pAllocator,&indexMap);
		if (m_TickMarketOrderFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"TickMarketOrder");
		pStru->reuseID = m_TickMarketOrderFactory->getReuseID();
				
		m_TradeFactory=new CTradeFactory(this,pAllocator,&indexMap);
		if (m_TradeFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"Trade");
		pStru->reuseID = m_TradeFactory->getReuseID();
				
		m_KLineFactory=new CKLineFactory(this,pAllocator,&indexMap);
		if (m_KLineFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"KLine");
		pStru->reuseID = m_KLineFactory->getReuseID();
				
		m_PublishKLineFactory=new CPublishKLineFactory(this,pAllocator,&indexMap);
		if (m_PublishKLineFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"PublishKLine");
		pStru->reuseID = m_PublishKLineFactory->getReuseID();
				
		m_LastKLineFactory=new CLastKLineFactory(this,pAllocator,&indexMap);
		if (m_LastKLineFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"LastKLine");
		pStru->reuseID = m_LastKLineFactory->getReuseID();
				
		m_AccountDetailFactory=new CAccountDetailFactory(this,pAllocator,&indexMap);
		if (m_AccountDetailFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"AccountDetail");
		pStru->reuseID = m_AccountDetailFactory->getReuseID();
				
		m_SettleDetailFactory=new CSettleDetailFactory(this,pAllocator,&indexMap);
		if (m_SettleDetailFactory==NULL)
			return 0;
		pStru = (TableStruct *)pMem->alloc();
		strcpy(pStru->tableName,"SettleDetail");
		pStru->reuseID = m_SettleDetailFactory->getReuseID();
				
	
		map<string,int>::iterator it = indexMap.begin();
		for(;it!=indexMap.end();it++)
		{
			pStru = (TableStruct *)pMem->alloc();
			strcpy(pStru->tableName,it->first.c_str());
			pStru->reuseID = it->second;
		}
	}
	else
	{
		pMem=new CFixMem(sizeof(TableStruct),MAX_TABLE_COUNT,pAllocator,true,MEMORYDB_ID);
		map<string,int> blockMap;
		TableStruct* pStru = NULL;
		for(int i=0;i<pMem->getCount();i++)
		{
			pStru = (TableStruct *)pMem->getObject(i);
			blockMap.insert(map<string,int>::value_type(pStru->tableName,pStru->reuseID));
		}
		map<string,int>::iterator it;
			
		it = blockMap.find("CurrentTime");
		if(it == blockMap.end())
			m_CurrentTimeFactory=new CCurrentTimeFactory(this,pAllocator,&blockMap);
		else
			m_CurrentTimeFactory=new CCurrentTimeFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_CurrentTimeFactory==NULL)
			return 0;
				
		it = blockMap.find("Member");
		if(it == blockMap.end())
			m_MemberFactory=new CMemberFactory(this,pAllocator,&blockMap);
		else
			m_MemberFactory=new CMemberFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_MemberFactory==NULL)
			return 0;
				
		it = blockMap.find("ServiceConfig");
		if(it == blockMap.end())
			m_ServiceConfigFactory=new CServiceConfigFactory(this,pAllocator,&blockMap);
		else
			m_ServiceConfigFactory=new CServiceConfigFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_ServiceConfigFactory==NULL)
			return 0;
				
		it = blockMap.find("UserSession");
		if(it == blockMap.end())
			m_UserSessionFactory=new CUserSessionFactory(this,pAllocator,&blockMap);
		else
			m_UserSessionFactory=new CUserSessionFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_UserSessionFactory==NULL)
			return 0;
				
		it = blockMap.find("Instrument");
		if(it == blockMap.end())
			m_InstrumentFactory=new CInstrumentFactory(this,pAllocator,&blockMap);
		else
			m_InstrumentFactory=new CInstrumentFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_InstrumentFactory==NULL)
			return 0;
				
		it = blockMap.find("TradingRight");
		if(it == blockMap.end())
			m_TradingRightFactory=new CTradingRightFactory(this,pAllocator,&blockMap);
		else
			m_TradingRightFactory=new CTradingRightFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_TradingRightFactory==NULL)
			return 0;
				
		it = blockMap.find("MarginRate");
		if(it == blockMap.end())
			m_MarginRateFactory=new CMarginRateFactory(this,pAllocator,&blockMap);
		else
			m_MarginRateFactory=new CMarginRateFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_MarginRateFactory==NULL)
			return 0;
				
		it = blockMap.find("CFDRate");
		if(it == blockMap.end())
			m_CFDRateFactory=new CCFDRateFactory(this,pAllocator,&blockMap);
		else
			m_CFDRateFactory=new CCFDRateFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_CFDRateFactory==NULL)
			return 0;
				
		it = blockMap.find("Fee");
		if(it == blockMap.end())
			m_FeeFactory=new CFeeFactory(this,pAllocator,&blockMap);
		else
			m_FeeFactory=new CFeeFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_FeeFactory==NULL)
			return 0;
				
		it = blockMap.find("Currency");
		if(it == blockMap.end())
			m_CurrencyFactory=new CCurrencyFactory(this,pAllocator,&blockMap);
		else
			m_CurrencyFactory=new CCurrencyFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_CurrencyFactory==NULL)
			return 0;
				
		it = blockMap.find("Liquidity");
		if(it == blockMap.end())
			m_LiquidityFactory=new CLiquidityFactory(this,pAllocator,&blockMap);
		else
			m_LiquidityFactory=new CLiquidityFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_LiquidityFactory==NULL)
			return 0;
				
		it = blockMap.find("Account");
		if(it == blockMap.end())
			m_AccountFactory=new CAccountFactory(this,pAllocator,&blockMap);
		else
			m_AccountFactory=new CAccountFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_AccountFactory==NULL)
			return 0;
				
		it = blockMap.find("MemberPosition");
		if(it == blockMap.end())
			m_MemberPositionFactory=new CMemberPositionFactory(this,pAllocator,&blockMap);
		else
			m_MemberPositionFactory=new CMemberPositionFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_MemberPositionFactory==NULL)
			return 0;
				
		it = blockMap.find("Position");
		if(it == blockMap.end())
			m_PositionFactory=new CPositionFactory(this,pAllocator,&blockMap);
		else
			m_PositionFactory=new CPositionFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_PositionFactory==NULL)
			return 0;
				
		it = blockMap.find("MarketData");
		if(it == blockMap.end())
			m_MarketDataFactory=new CMarketDataFactory(this,pAllocator,&blockMap);
		else
			m_MarketDataFactory=new CMarketDataFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_MarketDataFactory==NULL)
			return 0;
				
		it = blockMap.find("Order");
		if(it == blockMap.end())
			m_OrderFactory=new COrderFactory(this,pAllocator,&blockMap);
		else
			m_OrderFactory=new COrderFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_OrderFactory==NULL)
			return 0;
				
		it = blockMap.find("PositionOrder");
		if(it == blockMap.end())
			m_PositionOrderFactory=new CPositionOrderFactory(this,pAllocator,&blockMap);
		else
			m_PositionOrderFactory=new CPositionOrderFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_PositionOrderFactory==NULL)
			return 0;
				
		it = blockMap.find("TriggerOrder");
		if(it == blockMap.end())
			m_TriggerOrderFactory=new CTriggerOrderFactory(this,pAllocator,&blockMap);
		else
			m_TriggerOrderFactory=new CTriggerOrderFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_TriggerOrderFactory==NULL)
			return 0;
				
		it = blockMap.find("MarketOrder");
		if(it == blockMap.end())
			m_MarketOrderFactory=new CMarketOrderFactory(this,pAllocator,&blockMap);
		else
			m_MarketOrderFactory=new CMarketOrderFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_MarketOrderFactory==NULL)
			return 0;
				
		it = blockMap.find("TickMarketOrder");
		if(it == blockMap.end())
			m_TickMarketOrderFactory=new CTickMarketOrderFactory(this,pAllocator,&blockMap);
		else
			m_TickMarketOrderFactory=new CTickMarketOrderFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_TickMarketOrderFactory==NULL)
			return 0;
				
		it = blockMap.find("Trade");
		if(it == blockMap.end())
			m_TradeFactory=new CTradeFactory(this,pAllocator,&blockMap);
		else
			m_TradeFactory=new CTradeFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_TradeFactory==NULL)
			return 0;
				
		it = blockMap.find("KLine");
		if(it == blockMap.end())
			m_KLineFactory=new CKLineFactory(this,pAllocator,&blockMap);
		else
			m_KLineFactory=new CKLineFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_KLineFactory==NULL)
			return 0;
				
		it = blockMap.find("PublishKLine");
		if(it == blockMap.end())
			m_PublishKLineFactory=new CPublishKLineFactory(this,pAllocator,&blockMap);
		else
			m_PublishKLineFactory=new CPublishKLineFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_PublishKLineFactory==NULL)
			return 0;
				
		it = blockMap.find("LastKLine");
		if(it == blockMap.end())
			m_LastKLineFactory=new CLastKLineFactory(this,pAllocator,&blockMap);
		else
			m_LastKLineFactory=new CLastKLineFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_LastKLineFactory==NULL)
			return 0;
				
		it = blockMap.find("AccountDetail");
		if(it == blockMap.end())
			m_AccountDetailFactory=new CAccountDetailFactory(this,pAllocator,&blockMap);
		else
			m_AccountDetailFactory=new CAccountDetailFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_AccountDetailFactory==NULL)
			return 0;
				
		it = blockMap.find("SettleDetail");
		if(it == blockMap.end())
			m_SettleDetailFactory=new CSettleDetailFactory(this,pAllocator,&blockMap);
		else
			m_SettleDetailFactory=new CSettleDetailFactory(this,pAllocator,&blockMap,true,it->second);
		if (m_SettleDetailFactory==NULL)
			return 0;
				
	}
	return 1;
}

int CMDB::load(const char *path)
{
	char fname[300];
	sprintf(fname, "%sCurrentTime.csv", path);
	if (!m_CurrentTimeFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sMember.csv", path);
	if (!m_MemberFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sServiceConfig.csv", path);
	if (!m_ServiceConfigFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sUserSession.csv", path);
	if (!m_UserSessionFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sInstrument.csv", path);
	if (!m_InstrumentFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sTradingRight.csv", path);
	if (!m_TradingRightFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sMarginRate.csv", path);
	if (!m_MarginRateFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sCFDRate.csv", path);
	if (!m_CFDRateFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sFee.csv", path);
	if (!m_FeeFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sCurrency.csv", path);
	if (!m_CurrencyFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sLiquidity.csv", path);
	if (!m_LiquidityFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sAccount.csv", path);
	if (!m_AccountFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sMemberPosition.csv", path);
	if (!m_MemberPositionFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sPosition.csv", path);
	if (!m_PositionFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sMarketData.csv", path);
	if (!m_MarketDataFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sOrder.csv", path);
	if (!m_OrderFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sPositionOrder.csv", path);
	if (!m_PositionOrderFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sTriggerOrder.csv", path);
	if (!m_TriggerOrderFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sMarketOrder.csv", path);
	if (!m_MarketOrderFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sTickMarketOrder.csv", path);
	if (!m_TickMarketOrderFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sTrade.csv", path);
	if (!m_TradeFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sKLine.csv", path);
	if (!m_KLineFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sPublishKLine.csv", path);
	if (!m_PublishKLineFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sLastKLine.csv", path);
	if (!m_LastKLineFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sAccountDetail.csv", path);
	if (!m_AccountDetailFactory->readCSV(fname))
		return 0;
	sprintf(fname, "%sSettleDetail.csv", path);
	if (!m_SettleDetailFactory->readCSV(fname))
		return 0;
	return 1;
}

int CMDB::save(const char *path, UF_INT8 nSequenceNo)
{
	int dealBegin;
	GET_ACCURATE_MSEC_TIME(dealBegin);	
	PRINT_TO_STD("Begin MDB Save to %s", path);
	makedir(path);
	char fname[300];
	sprintf(fname, "%sCurrentTime.csv", path);
	if (!m_CurrentTimeFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sMember.csv", path);
	if (!m_MemberFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sServiceConfig.csv", path);
	if (!m_ServiceConfigFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sUserSession.csv", path);
	if (!m_UserSessionFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sInstrument.csv", path);
	if (!m_InstrumentFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sTradingRight.csv", path);
	if (!m_TradingRightFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sMarginRate.csv", path);
	if (!m_MarginRateFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sCFDRate.csv", path);
	if (!m_CFDRateFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sFee.csv", path);
	if (!m_FeeFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sCurrency.csv", path);
	if (!m_CurrencyFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sLiquidity.csv", path);
	if (!m_LiquidityFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sAccount.csv", path);
	if (!m_AccountFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sMemberPosition.csv", path);
	if (!m_MemberPositionFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sPosition.csv", path);
	if (!m_PositionFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sMarketData.csv", path);
	if (!m_MarketDataFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sOrder.csv", path);
	if (!m_OrderFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sPositionOrder.csv", path);
	if (!m_PositionOrderFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sTriggerOrder.csv", path);
	if (!m_TriggerOrderFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sLastKLine.csv", path);
	if (!m_LastKLineFactory->writeCSV(fname))
		return 0;
	
	FILE* output = mfopen("dumpPhase.log", "wt");
	fprintf(output, "%.0f", nSequenceNo);
	fclose(output);	
	int dealEnd;
	GET_ACCURATE_MSEC_TIME(dealEnd);
	PRINT_TO_STD("Time MDB Save is %d ms", dealEnd - dealBegin);
	PRINT_TO_STD("End MDB Save to %s", path);	
	return 1;
}

int CMDB::savePreVersion(const char *path, UF_INT8 nSequenceNo)
{
	int dealBegin;
	GET_ACCURATE_MSEC_TIME(dealBegin);	
	PRINT_TO_STD("Begin MDB Save to %s", path);
	makedir(path);
	char fname[300];
	sprintf(fname, "%sCurrentTime.csv", path);
	if (!m_CurrentTimeFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sMember.csv", path);
	if (!m_MemberFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sServiceConfig.csv", path);
	if (!m_ServiceConfigFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sUserSession.csv", path);
	if (!m_UserSessionFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sInstrument.csv", path);
	if (!m_InstrumentFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sTradingRight.csv", path);
	if (!m_TradingRightFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sMarginRate.csv", path);
	if (!m_MarginRateFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sCFDRate.csv", path);
	if (!m_CFDRateFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sFee.csv", path);
	if (!m_FeeFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sCurrency.csv", path);
	if (!m_CurrencyFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sLiquidity.csv", path);
	if (!m_LiquidityFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sAccount.csv", path);
	if (!m_AccountFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sMemberPosition.csv", path);
	if (!m_MemberPositionFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sPosition.csv", path);
	if (!m_PositionFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sMarketData.csv", path);
	if (!m_MarketDataFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sOrder.csv", path);
	if (!m_OrderFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sPositionOrder.csv", path);
	if (!m_PositionOrderFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sTriggerOrder.csv", path);
	if (!m_TriggerOrderFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sLastKLine.csv", path);
	if (!m_LastKLineFactory->writeCSVPreVersion(fname))
		return 0;
	
	FILE* output = mfopen("dumpPhase.log", "wt");
	fprintf(output, "%.0f", nSequenceNo);
	fclose(output);	
	int dealEnd;
	GET_ACCURATE_MSEC_TIME(dealEnd);
	PRINT_TO_STD("Time MDB Save is %d ms", dealEnd - dealBegin);
	PRINT_TO_STD("End MDB Save to %s", path);	
	return 1;
}

int CMDB::saveAll(const char *path, UF_INT8 nSequenceNo)
{
	int dealBegin;
	GET_ACCURATE_MSEC_TIME(dealBegin);	
	PRINT_TO_STD("Begin MDB Save to %s", path);
	makedir(path);
	char fname[300];
	sprintf(fname, "%sCurrentTime.csv", path);
	if (!m_CurrentTimeFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sMember.csv", path);
	if (!m_MemberFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sServiceConfig.csv", path);
	if (!m_ServiceConfigFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sUserSession.csv", path);
	if (!m_UserSessionFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sInstrument.csv", path);
	if (!m_InstrumentFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sTradingRight.csv", path);
	if (!m_TradingRightFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sMarginRate.csv", path);
	if (!m_MarginRateFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sCFDRate.csv", path);
	if (!m_CFDRateFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sFee.csv", path);
	if (!m_FeeFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sCurrency.csv", path);
	if (!m_CurrencyFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sLiquidity.csv", path);
	if (!m_LiquidityFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sAccount.csv", path);
	if (!m_AccountFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sMemberPosition.csv", path);
	if (!m_MemberPositionFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sPosition.csv", path);
	if (!m_PositionFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sMarketData.csv", path);
	if (!m_MarketDataFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sOrder.csv", path);
	if (!m_OrderFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sPositionOrder.csv", path);
	if (!m_PositionOrderFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sTriggerOrder.csv", path);
	if (!m_TriggerOrderFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sMarketOrder.csv", path);
	if (!m_MarketOrderFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sTickMarketOrder.csv", path);
	if (!m_TickMarketOrderFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sTrade.csv", path);
	if (!m_TradeFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sKLine.csv", path);
	if (!m_KLineFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sPublishKLine.csv", path);
	if (!m_PublishKLineFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sLastKLine.csv", path);
	if (!m_LastKLineFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sAccountDetail.csv", path);
	if (!m_AccountDetailFactory->writeCSV(fname))
		return 0;
	sprintf(fname, "%sSettleDetail.csv", path);
	if (!m_SettleDetailFactory->writeCSV(fname))
		return 0;
	FILE* output = mfopen("dumpPhase.log", "wt");
	fprintf(output, "%.0f", nSequenceNo);
	fclose(output);	
	int dealEnd;
	GET_ACCURATE_MSEC_TIME(dealEnd);
	PRINT_TO_STD("Time MDB SaveAll is %d ms", dealEnd - dealBegin);
	PRINT_TO_STD("End MDB SaveAll to %s", path);	
	return 1;
}

int CMDB::saveAllPreVersion(const char *path, UF_INT8 nSequenceNo)
{
	int dealBegin;
	GET_ACCURATE_MSEC_TIME(dealBegin);	
	PRINT_TO_STD("Begin MDB Save to %s", path);
	makedir(path);
	char fname[300];
	sprintf(fname, "%sCurrentTime.csv", path);
	if (!m_CurrentTimeFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sMember.csv", path);
	if (!m_MemberFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sServiceConfig.csv", path);
	if (!m_ServiceConfigFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sUserSession.csv", path);
	if (!m_UserSessionFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sInstrument.csv", path);
	if (!m_InstrumentFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sTradingRight.csv", path);
	if (!m_TradingRightFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sMarginRate.csv", path);
	if (!m_MarginRateFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sCFDRate.csv", path);
	if (!m_CFDRateFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sFee.csv", path);
	if (!m_FeeFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sCurrency.csv", path);
	if (!m_CurrencyFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sLiquidity.csv", path);
	if (!m_LiquidityFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sAccount.csv", path);
	if (!m_AccountFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sMemberPosition.csv", path);
	if (!m_MemberPositionFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sPosition.csv", path);
	if (!m_PositionFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sMarketData.csv", path);
	if (!m_MarketDataFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sOrder.csv", path);
	if (!m_OrderFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sPositionOrder.csv", path);
	if (!m_PositionOrderFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sTriggerOrder.csv", path);
	if (!m_TriggerOrderFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sMarketOrder.csv", path);
	if (!m_MarketOrderFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sTickMarketOrder.csv", path);
	if (!m_TickMarketOrderFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sTrade.csv", path);
	if (!m_TradeFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sKLine.csv", path);
	if (!m_KLineFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sPublishKLine.csv", path);
	if (!m_PublishKLineFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sLastKLine.csv", path);
	if (!m_LastKLineFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sAccountDetail.csv", path);
	if (!m_AccountDetailFactory->writeCSVPreVersion(fname))
		return 0;
	sprintf(fname, "%sSettleDetail.csv", path);
	if (!m_SettleDetailFactory->writeCSVPreVersion(fname))
		return 0;
	FILE* output = mfopen("dumpPhase.log", "wt");
	fprintf(output, "%.0f", nSequenceNo);
	fclose(output);	
	int dealEnd;
	GET_ACCURATE_MSEC_TIME(dealEnd);
	PRINT_TO_STD("Time MDB SaveAll is %d ms", dealEnd - dealBegin);
	PRINT_TO_STD("End MDB SaveAll to %s", path);	
	return 1;
}

bool CMDB::snap(const char *path, UF_INT8 nSequenceNo)
{
	if (m_MDBLastSnap.m_Status != 0)
	{
		PRINT_TO_STD("MDBSnap Status is not 0, is %d", m_MDBLastSnap.m_Status);
		return false;
	}
	int dealBegin;
	GET_ACCURATE_MSEC_TIME(dealBegin);
	PRINT_TO_STD("Begin MDB Snap to %s", path);
	
	CCurrentTime* pCurrentTime = m_CurrentTimeFactory->getFirst();
	while (pCurrentTime != NULL)
	{
		m_MDBLastSnap.m_CurrentTime.push_back(*pCurrentTime);
		pCurrentTime = m_CurrentTimeFactory->getNext();
	}
	CMember* pMember = m_MemberFactory->getFirst();
	while (pMember != NULL)
	{
		m_MDBLastSnap.m_Member.push_back(*pMember);
		pMember = m_MemberFactory->getNext();
	}
	CServiceConfig* pServiceConfig = m_ServiceConfigFactory->getFirst();
	while (pServiceConfig != NULL)
	{
		m_MDBLastSnap.m_ServiceConfig.push_back(*pServiceConfig);
		pServiceConfig = m_ServiceConfigFactory->getNext();
	}
	CUserSession* pUserSession = m_UserSessionFactory->getFirst();
	while (pUserSession != NULL)
	{
		m_MDBLastSnap.m_UserSession.push_back(*pUserSession);
		pUserSession = m_UserSessionFactory->getNext();
	}
	CInstrument* pInstrument = m_InstrumentFactory->getFirst();
	while (pInstrument != NULL)
	{
		m_MDBLastSnap.m_Instrument.push_back(*pInstrument);
		pInstrument = m_InstrumentFactory->getNext();
	}
	CTradingRight* pTradingRight = m_TradingRightFactory->getFirst();
	while (pTradingRight != NULL)
	{
		m_MDBLastSnap.m_TradingRight.push_back(*pTradingRight);
		pTradingRight = m_TradingRightFactory->getNext();
	}
	CMarginRate* pMarginRate = m_MarginRateFactory->getFirst();
	while (pMarginRate != NULL)
	{
		m_MDBLastSnap.m_MarginRate.push_back(*pMarginRate);
		pMarginRate = m_MarginRateFactory->getNext();
	}
	CCFDRate* pCFDRate = m_CFDRateFactory->getFirst();
	while (pCFDRate != NULL)
	{
		m_MDBLastSnap.m_CFDRate.push_back(*pCFDRate);
		pCFDRate = m_CFDRateFactory->getNext();
	}
	CFee* pFee = m_FeeFactory->getFirst();
	while (pFee != NULL)
	{
		m_MDBLastSnap.m_Fee.push_back(*pFee);
		pFee = m_FeeFactory->getNext();
	}
	CCurrency* pCurrency = m_CurrencyFactory->getFirst();
	while (pCurrency != NULL)
	{
		m_MDBLastSnap.m_Currency.push_back(*pCurrency);
		pCurrency = m_CurrencyFactory->getNext();
	}
	CLiquidity* pLiquidity = m_LiquidityFactory->getFirst();
	while (pLiquidity != NULL)
	{
		m_MDBLastSnap.m_Liquidity.push_back(*pLiquidity);
		pLiquidity = m_LiquidityFactory->getNext();
	}
	CAccount* pAccount = m_AccountFactory->getFirst();
	while (pAccount != NULL)
	{
		m_MDBLastSnap.m_Account.push_back(*pAccount);
		pAccount = m_AccountFactory->getNext();
	}
	CMemberPosition* pMemberPosition = m_MemberPositionFactory->getFirst();
	while (pMemberPosition != NULL)
	{
		m_MDBLastSnap.m_MemberPosition.push_back(*pMemberPosition);
		pMemberPosition = m_MemberPositionFactory->getNext();
	}
	CPosition* pPosition = m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		m_MDBLastSnap.m_Position.push_back(*pPosition);
		pPosition = m_PositionFactory->getNext();
	}
	CMarketData* pMarketData = m_MarketDataFactory->getFirst();
	while (pMarketData != NULL)
	{
		m_MDBLastSnap.m_MarketData.push_back(*pMarketData);
		pMarketData = m_MarketDataFactory->getNext();
	}
	COrder* pOrder = m_OrderFactory->getFirst();
	while (pOrder != NULL)
	{
		m_MDBLastSnap.m_Order.push_back(*pOrder);
		pOrder = m_OrderFactory->getNext();
	}
	CPositionOrder* pPositionOrder = m_PositionOrderFactory->getFirst();
	while (pPositionOrder != NULL)
	{
		m_MDBLastSnap.m_PositionOrder.push_back(*pPositionOrder);
		pPositionOrder = m_PositionOrderFactory->getNext();
	}
	CTriggerOrder* pTriggerOrder = m_TriggerOrderFactory->getFirst();
	while (pTriggerOrder != NULL)
	{
		m_MDBLastSnap.m_TriggerOrder.push_back(*pTriggerOrder);
		pTriggerOrder = m_TriggerOrderFactory->getNext();
	}
	CLastKLine* pLastKLine = m_LastKLineFactory->getFirst();
	while (pLastKLine != NULL)
	{
		m_MDBLastSnap.m_LastKLine.push_back(*pLastKLine);
		pLastKLine = m_LastKLineFactory->getNext();
	}

	strcpy(m_sLastSnapPath, path);
	m_MDBLastSnap.m_Status = 1;
	m_nLastSequenceNo = nSequenceNo;
	int dealEnd;
	GET_ACCURATE_MSEC_TIME(dealEnd);
	PRINT_TO_STD("Time MDB Snap is %d ms", dealEnd - dealBegin);	
	PRINT_TO_STD("End MDB Snap to %s", path);
	return true;
}

bool CMDB::snapAll(const char *path, UF_INT8 nSequenceNo)
{
	if (m_MDBLastSnap.m_Status != 0)
	{
		PRINT_TO_STD("MDBSnap Status is not 0, is %d", m_MDBLastSnap.m_Status);
		return false;
	}
	int dealBegin;
	GET_ACCURATE_MSEC_TIME(dealBegin);
	PRINT_TO_STD("Begin MDB Snap to %s", path);
	
	CCurrentTime* pCurrentTime = m_CurrentTimeFactory->getFirst();
	while (pCurrentTime != NULL)
	{
		m_MDBLastSnap.m_CurrentTime.push_back(*pCurrentTime);
		pCurrentTime = m_CurrentTimeFactory->getNext();
	}
	CMember* pMember = m_MemberFactory->getFirst();
	while (pMember != NULL)
	{
		m_MDBLastSnap.m_Member.push_back(*pMember);
		pMember = m_MemberFactory->getNext();
	}
	CServiceConfig* pServiceConfig = m_ServiceConfigFactory->getFirst();
	while (pServiceConfig != NULL)
	{
		m_MDBLastSnap.m_ServiceConfig.push_back(*pServiceConfig);
		pServiceConfig = m_ServiceConfigFactory->getNext();
	}
	CUserSession* pUserSession = m_UserSessionFactory->getFirst();
	while (pUserSession != NULL)
	{
		m_MDBLastSnap.m_UserSession.push_back(*pUserSession);
		pUserSession = m_UserSessionFactory->getNext();
	}
	CInstrument* pInstrument = m_InstrumentFactory->getFirst();
	while (pInstrument != NULL)
	{
		m_MDBLastSnap.m_Instrument.push_back(*pInstrument);
		pInstrument = m_InstrumentFactory->getNext();
	}
	CTradingRight* pTradingRight = m_TradingRightFactory->getFirst();
	while (pTradingRight != NULL)
	{
		m_MDBLastSnap.m_TradingRight.push_back(*pTradingRight);
		pTradingRight = m_TradingRightFactory->getNext();
	}
	CMarginRate* pMarginRate = m_MarginRateFactory->getFirst();
	while (pMarginRate != NULL)
	{
		m_MDBLastSnap.m_MarginRate.push_back(*pMarginRate);
		pMarginRate = m_MarginRateFactory->getNext();
	}
	CCFDRate* pCFDRate = m_CFDRateFactory->getFirst();
	while (pCFDRate != NULL)
	{
		m_MDBLastSnap.m_CFDRate.push_back(*pCFDRate);
		pCFDRate = m_CFDRateFactory->getNext();
	}
	CFee* pFee = m_FeeFactory->getFirst();
	while (pFee != NULL)
	{
		m_MDBLastSnap.m_Fee.push_back(*pFee);
		pFee = m_FeeFactory->getNext();
	}
	CCurrency* pCurrency = m_CurrencyFactory->getFirst();
	while (pCurrency != NULL)
	{
		m_MDBLastSnap.m_Currency.push_back(*pCurrency);
		pCurrency = m_CurrencyFactory->getNext();
	}
	CLiquidity* pLiquidity = m_LiquidityFactory->getFirst();
	while (pLiquidity != NULL)
	{
		m_MDBLastSnap.m_Liquidity.push_back(*pLiquidity);
		pLiquidity = m_LiquidityFactory->getNext();
	}
	CAccount* pAccount = m_AccountFactory->getFirst();
	while (pAccount != NULL)
	{
		m_MDBLastSnap.m_Account.push_back(*pAccount);
		pAccount = m_AccountFactory->getNext();
	}
	CMemberPosition* pMemberPosition = m_MemberPositionFactory->getFirst();
	while (pMemberPosition != NULL)
	{
		m_MDBLastSnap.m_MemberPosition.push_back(*pMemberPosition);
		pMemberPosition = m_MemberPositionFactory->getNext();
	}
	CPosition* pPosition = m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		m_MDBLastSnap.m_Position.push_back(*pPosition);
		pPosition = m_PositionFactory->getNext();
	}
	CMarketData* pMarketData = m_MarketDataFactory->getFirst();
	while (pMarketData != NULL)
	{
		m_MDBLastSnap.m_MarketData.push_back(*pMarketData);
		pMarketData = m_MarketDataFactory->getNext();
	}
	COrder* pOrder = m_OrderFactory->getFirst();
	while (pOrder != NULL)
	{
		m_MDBLastSnap.m_Order.push_back(*pOrder);
		pOrder = m_OrderFactory->getNext();
	}
	CPositionOrder* pPositionOrder = m_PositionOrderFactory->getFirst();
	while (pPositionOrder != NULL)
	{
		m_MDBLastSnap.m_PositionOrder.push_back(*pPositionOrder);
		pPositionOrder = m_PositionOrderFactory->getNext();
	}
	CTriggerOrder* pTriggerOrder = m_TriggerOrderFactory->getFirst();
	while (pTriggerOrder != NULL)
	{
		m_MDBLastSnap.m_TriggerOrder.push_back(*pTriggerOrder);
		pTriggerOrder = m_TriggerOrderFactory->getNext();
	}
	CMarketOrder* pMarketOrder = m_MarketOrderFactory->getFirst();
	while (pMarketOrder != NULL)
	{
		m_MDBLastSnap.m_MarketOrder.push_back(*pMarketOrder);
		pMarketOrder = m_MarketOrderFactory->getNext();
	}
	CTickMarketOrder* pTickMarketOrder = m_TickMarketOrderFactory->getFirst();
	while (pTickMarketOrder != NULL)
	{
		m_MDBLastSnap.m_TickMarketOrder.push_back(*pTickMarketOrder);
		pTickMarketOrder = m_TickMarketOrderFactory->getNext();
	}
	CTrade* pTrade = m_TradeFactory->getFirst();
	while (pTrade != NULL)
	{
		m_MDBLastSnap.m_Trade.push_back(*pTrade);
		pTrade = m_TradeFactory->getNext();
	}
	CKLine* pKLine = m_KLineFactory->getFirst();
	while (pKLine != NULL)
	{
		m_MDBLastSnap.m_KLine.push_back(*pKLine);
		pKLine = m_KLineFactory->getNext();
	}
	CPublishKLine* pPublishKLine = m_PublishKLineFactory->getFirst();
	while (pPublishKLine != NULL)
	{
		m_MDBLastSnap.m_PublishKLine.push_back(*pPublishKLine);
		pPublishKLine = m_PublishKLineFactory->getNext();
	}
	CLastKLine* pLastKLine = m_LastKLineFactory->getFirst();
	while (pLastKLine != NULL)
	{
		m_MDBLastSnap.m_LastKLine.push_back(*pLastKLine);
		pLastKLine = m_LastKLineFactory->getNext();
	}
	CAccountDetail* pAccountDetail = m_AccountDetailFactory->getFirst();
	while (pAccountDetail != NULL)
	{
		m_MDBLastSnap.m_AccountDetail.push_back(*pAccountDetail);
		pAccountDetail = m_AccountDetailFactory->getNext();
	}
	CSettleDetail* pSettleDetail = m_SettleDetailFactory->getFirst();
	while (pSettleDetail != NULL)
	{
		m_MDBLastSnap.m_SettleDetail.push_back(*pSettleDetail);
		pSettleDetail = m_SettleDetailFactory->getNext();
	}

	strcpy(m_sLastSnapPath, path);
	m_MDBLastSnap.m_Status = 1;
	m_nLastSequenceNo = nSequenceNo;
	int dealEnd;
	GET_ACCURATE_MSEC_TIME(dealEnd);
	PRINT_TO_STD("Time MDB SnapAll is %d ms", dealEnd - dealBegin);	
	PRINT_TO_STD("End MDB SnapAll to %s", path);
	return true;
}

bool CMDB::canSnap()
{
	if (m_MDBLastSnap.m_Status != 0)
		return false;
	return true;
}

bool CMDB::saveSnap()
{
	if (m_MDBLastSnap.m_Status != 1)
		return false;
		
	int dealBegin;
	GET_ACCURATE_MSEC_TIME(dealBegin);	
	PRINT_TO_STD("Begin MDB SaveSnap to %s", m_sLastSnapPath);
	makedir(m_sLastSnapPath);
	char filename[300];
	FILE *output = NULL;
	
	sprintf(filename, "%sCurrentTime.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableCurrentTime::writeCSVHead(output);
	vector<CWriteableCurrentTime>::iterator CurrentTimeIter = m_MDBLastSnap.m_CurrentTime.begin();
	while (CurrentTimeIter != m_MDBLastSnap.m_CurrentTime.end())
	{
		CurrentTimeIter->writeCSV(output);
		CurrentTimeIter++;
	}
	fclose(output);
	sprintf(filename, "%sMember.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableMember::writeCSVHead(output);
	vector<CWriteableMember>::iterator MemberIter = m_MDBLastSnap.m_Member.begin();
	while (MemberIter != m_MDBLastSnap.m_Member.end())
	{
		MemberIter->writeCSV(output);
		MemberIter++;
	}
	fclose(output);
	sprintf(filename, "%sServiceConfig.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableServiceConfig::writeCSVHead(output);
	vector<CWriteableServiceConfig>::iterator ServiceConfigIter = m_MDBLastSnap.m_ServiceConfig.begin();
	while (ServiceConfigIter != m_MDBLastSnap.m_ServiceConfig.end())
	{
		ServiceConfigIter->writeCSV(output);
		ServiceConfigIter++;
	}
	fclose(output);
	sprintf(filename, "%sUserSession.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableUserSession::writeCSVHead(output);
	vector<CWriteableUserSession>::iterator UserSessionIter = m_MDBLastSnap.m_UserSession.begin();
	while (UserSessionIter != m_MDBLastSnap.m_UserSession.end())
	{
		UserSessionIter->writeCSV(output);
		UserSessionIter++;
	}
	fclose(output);
	sprintf(filename, "%sInstrument.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableInstrument::writeCSVHead(output);
	vector<CWriteableInstrument>::iterator InstrumentIter = m_MDBLastSnap.m_Instrument.begin();
	while (InstrumentIter != m_MDBLastSnap.m_Instrument.end())
	{
		InstrumentIter->writeCSV(output);
		InstrumentIter++;
	}
	fclose(output);
	sprintf(filename, "%sTradingRight.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableTradingRight::writeCSVHead(output);
	vector<CWriteableTradingRight>::iterator TradingRightIter = m_MDBLastSnap.m_TradingRight.begin();
	while (TradingRightIter != m_MDBLastSnap.m_TradingRight.end())
	{
		TradingRightIter->writeCSV(output);
		TradingRightIter++;
	}
	fclose(output);
	sprintf(filename, "%sMarginRate.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableMarginRate::writeCSVHead(output);
	vector<CWriteableMarginRate>::iterator MarginRateIter = m_MDBLastSnap.m_MarginRate.begin();
	while (MarginRateIter != m_MDBLastSnap.m_MarginRate.end())
	{
		MarginRateIter->writeCSV(output);
		MarginRateIter++;
	}
	fclose(output);
	sprintf(filename, "%sCFDRate.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableCFDRate::writeCSVHead(output);
	vector<CWriteableCFDRate>::iterator CFDRateIter = m_MDBLastSnap.m_CFDRate.begin();
	while (CFDRateIter != m_MDBLastSnap.m_CFDRate.end())
	{
		CFDRateIter->writeCSV(output);
		CFDRateIter++;
	}
	fclose(output);
	sprintf(filename, "%sFee.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableFee::writeCSVHead(output);
	vector<CWriteableFee>::iterator FeeIter = m_MDBLastSnap.m_Fee.begin();
	while (FeeIter != m_MDBLastSnap.m_Fee.end())
	{
		FeeIter->writeCSV(output);
		FeeIter++;
	}
	fclose(output);
	sprintf(filename, "%sCurrency.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableCurrency::writeCSVHead(output);
	vector<CWriteableCurrency>::iterator CurrencyIter = m_MDBLastSnap.m_Currency.begin();
	while (CurrencyIter != m_MDBLastSnap.m_Currency.end())
	{
		CurrencyIter->writeCSV(output);
		CurrencyIter++;
	}
	fclose(output);
	sprintf(filename, "%sLiquidity.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableLiquidity::writeCSVHead(output);
	vector<CWriteableLiquidity>::iterator LiquidityIter = m_MDBLastSnap.m_Liquidity.begin();
	while (LiquidityIter != m_MDBLastSnap.m_Liquidity.end())
	{
		LiquidityIter->writeCSV(output);
		LiquidityIter++;
	}
	fclose(output);
	sprintf(filename, "%sAccount.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableAccount::writeCSVHead(output);
	vector<CWriteableAccount>::iterator AccountIter = m_MDBLastSnap.m_Account.begin();
	while (AccountIter != m_MDBLastSnap.m_Account.end())
	{
		AccountIter->writeCSV(output);
		AccountIter++;
	}
	fclose(output);
	sprintf(filename, "%sMemberPosition.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableMemberPosition::writeCSVHead(output);
	vector<CWriteableMemberPosition>::iterator MemberPositionIter = m_MDBLastSnap.m_MemberPosition.begin();
	while (MemberPositionIter != m_MDBLastSnap.m_MemberPosition.end())
	{
		MemberPositionIter->writeCSV(output);
		MemberPositionIter++;
	}
	fclose(output);
	sprintf(filename, "%sPosition.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteablePosition::writeCSVHead(output);
	vector<CWriteablePosition>::iterator PositionIter = m_MDBLastSnap.m_Position.begin();
	while (PositionIter != m_MDBLastSnap.m_Position.end())
	{
		PositionIter->writeCSV(output);
		PositionIter++;
	}
	fclose(output);
	sprintf(filename, "%sMarketData.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableMarketData::writeCSVHead(output);
	vector<CWriteableMarketData>::iterator MarketDataIter = m_MDBLastSnap.m_MarketData.begin();
	while (MarketDataIter != m_MDBLastSnap.m_MarketData.end())
	{
		MarketDataIter->writeCSV(output);
		MarketDataIter++;
	}
	fclose(output);
	sprintf(filename, "%sOrder.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableOrder::writeCSVHead(output);
	vector<CWriteableOrder>::iterator OrderIter = m_MDBLastSnap.m_Order.begin();
	while (OrderIter != m_MDBLastSnap.m_Order.end())
	{
		OrderIter->writeCSV(output);
		OrderIter++;
	}
	fclose(output);
	sprintf(filename, "%sPositionOrder.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteablePositionOrder::writeCSVHead(output);
	vector<CWriteablePositionOrder>::iterator PositionOrderIter = m_MDBLastSnap.m_PositionOrder.begin();
	while (PositionOrderIter != m_MDBLastSnap.m_PositionOrder.end())
	{
		PositionOrderIter->writeCSV(output);
		PositionOrderIter++;
	}
	fclose(output);
	sprintf(filename, "%sTriggerOrder.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableTriggerOrder::writeCSVHead(output);
	vector<CWriteableTriggerOrder>::iterator TriggerOrderIter = m_MDBLastSnap.m_TriggerOrder.begin();
	while (TriggerOrderIter != m_MDBLastSnap.m_TriggerOrder.end())
	{
		TriggerOrderIter->writeCSV(output);
		TriggerOrderIter++;
	}
	fclose(output);
	if(m_MDBLastSnap.m_MarketOrder.size() > 0)
	{
		sprintf(filename, "%sMarketOrder.csv", m_sLastSnapPath);
		output = mfopen(filename, "wt");
		if (output == NULL)
			return false;
		CWriteableMarketOrder::writeCSVHead(output);
		vector<CWriteableMarketOrder>::iterator MarketOrderIter = m_MDBLastSnap.m_MarketOrder.begin();
		while (MarketOrderIter != m_MDBLastSnap.m_MarketOrder.end())
		{
			MarketOrderIter->writeCSV(output);
			MarketOrderIter++;
		}
		fclose(output);
	}	
	if(m_MDBLastSnap.m_TickMarketOrder.size() > 0)
	{
		sprintf(filename, "%sTickMarketOrder.csv", m_sLastSnapPath);
		output = mfopen(filename, "wt");
		if (output == NULL)
			return false;
		CWriteableTickMarketOrder::writeCSVHead(output);
		vector<CWriteableTickMarketOrder>::iterator TickMarketOrderIter = m_MDBLastSnap.m_TickMarketOrder.begin();
		while (TickMarketOrderIter != m_MDBLastSnap.m_TickMarketOrder.end())
		{
			TickMarketOrderIter->writeCSV(output);
			TickMarketOrderIter++;
		}
		fclose(output);
	}	
	if(m_MDBLastSnap.m_Trade.size() > 0)
	{
		sprintf(filename, "%sTrade.csv", m_sLastSnapPath);
		output = mfopen(filename, "wt");
		if (output == NULL)
			return false;
		CWriteableTrade::writeCSVHead(output);
		vector<CWriteableTrade>::iterator TradeIter = m_MDBLastSnap.m_Trade.begin();
		while (TradeIter != m_MDBLastSnap.m_Trade.end())
		{
			TradeIter->writeCSV(output);
			TradeIter++;
		}
		fclose(output);
	}	
	if(m_MDBLastSnap.m_KLine.size() > 0)
	{
		sprintf(filename, "%sKLine.csv", m_sLastSnapPath);
		output = mfopen(filename, "wt");
		if (output == NULL)
			return false;
		CWriteableKLine::writeCSVHead(output);
		vector<CWriteableKLine>::iterator KLineIter = m_MDBLastSnap.m_KLine.begin();
		while (KLineIter != m_MDBLastSnap.m_KLine.end())
		{
			KLineIter->writeCSV(output);
			KLineIter++;
		}
		fclose(output);
	}	
	if(m_MDBLastSnap.m_PublishKLine.size() > 0)
	{
		sprintf(filename, "%sPublishKLine.csv", m_sLastSnapPath);
		output = mfopen(filename, "wt");
		if (output == NULL)
			return false;
		CWriteablePublishKLine::writeCSVHead(output);
		vector<CWriteablePublishKLine>::iterator PublishKLineIter = m_MDBLastSnap.m_PublishKLine.begin();
		while (PublishKLineIter != m_MDBLastSnap.m_PublishKLine.end())
		{
			PublishKLineIter->writeCSV(output);
			PublishKLineIter++;
		}
		fclose(output);
	}	
	sprintf(filename, "%sLastKLine.csv", m_sLastSnapPath);
	output = mfopen(filename, "wt");
	if (output == NULL)
		return false;
	CWriteableLastKLine::writeCSVHead(output);
	vector<CWriteableLastKLine>::iterator LastKLineIter = m_MDBLastSnap.m_LastKLine.begin();
	while (LastKLineIter != m_MDBLastSnap.m_LastKLine.end())
	{
		LastKLineIter->writeCSV(output);
		LastKLineIter++;
	}
	fclose(output);
	if(m_MDBLastSnap.m_AccountDetail.size() > 0)
	{
		sprintf(filename, "%sAccountDetail.csv", m_sLastSnapPath);
		output = mfopen(filename, "wt");
		if (output == NULL)
			return false;
		CWriteableAccountDetail::writeCSVHead(output);
		vector<CWriteableAccountDetail>::iterator AccountDetailIter = m_MDBLastSnap.m_AccountDetail.begin();
		while (AccountDetailIter != m_MDBLastSnap.m_AccountDetail.end())
		{
			AccountDetailIter->writeCSV(output);
			AccountDetailIter++;
		}
		fclose(output);
	}	
	if(m_MDBLastSnap.m_SettleDetail.size() > 0)
	{
		sprintf(filename, "%sSettleDetail.csv", m_sLastSnapPath);
		output = mfopen(filename, "wt");
		if (output == NULL)
			return false;
		CWriteableSettleDetail::writeCSVHead(output);
		vector<CWriteableSettleDetail>::iterator SettleDetailIter = m_MDBLastSnap.m_SettleDetail.begin();
		while (SettleDetailIter != m_MDBLastSnap.m_SettleDetail.end())
		{
			SettleDetailIter->writeCSV(output);
			SettleDetailIter++;
		}
		fclose(output);
	}	
	
	//最后文件显示写入成功
	output = mfopen("dumpPhase.log", "wt");
	fprintf(output, "%.0f", m_nLastSequenceNo);
	fclose(output);
	int dealEnd;
	GET_ACCURATE_MSEC_TIME(dealEnd);
	PRINT_TO_STD("Time MDB SaveSnap is %d ms", dealEnd - dealBegin);
	PRINT_TO_STD("End MDB SaveSnap to %s", m_sLastSnapPath);
	clearSnap();	
	return true;
}
bool CMDB::clearSnap()
{
	m_MDBLastSnap.m_CurrentTime.clear();
	m_MDBLastSnap.m_Member.clear();
	m_MDBLastSnap.m_ServiceConfig.clear();
	m_MDBLastSnap.m_UserSession.clear();
	m_MDBLastSnap.m_Instrument.clear();
	m_MDBLastSnap.m_TradingRight.clear();
	m_MDBLastSnap.m_MarginRate.clear();
	m_MDBLastSnap.m_CFDRate.clear();
	m_MDBLastSnap.m_Fee.clear();
	m_MDBLastSnap.m_Currency.clear();
	m_MDBLastSnap.m_Liquidity.clear();
	m_MDBLastSnap.m_Account.clear();
	m_MDBLastSnap.m_MemberPosition.clear();
	m_MDBLastSnap.m_Position.clear();
	m_MDBLastSnap.m_MarketData.clear();
	m_MDBLastSnap.m_Order.clear();
	m_MDBLastSnap.m_PositionOrder.clear();
	m_MDBLastSnap.m_TriggerOrder.clear();
	m_MDBLastSnap.m_MarketOrder.clear();
	m_MDBLastSnap.m_TickMarketOrder.clear();
	m_MDBLastSnap.m_Trade.clear();
	m_MDBLastSnap.m_KLine.clear();
	m_MDBLastSnap.m_PublishKLine.clear();
	m_MDBLastSnap.m_LastKLine.clear();
	m_MDBLastSnap.m_AccountDetail.clear();
	m_MDBLastSnap.m_SettleDetail.clear();
	
	memset(m_sLastSnapPath, 0, sizeof(m_sLastSnapPath));
	m_MDBLastSnap.m_Status = 0;
	m_nLastSequenceNo = 0.0;
	return true;
}

void CMDB::clear()
{
	m_CurrentTimeFactory->clearAll();
	m_MemberFactory->clearAll();
	m_ServiceConfigFactory->clearAll();
	m_UserSessionFactory->clearAll();
	m_InstrumentFactory->clearAll();
	m_TradingRightFactory->clearAll();
	m_MarginRateFactory->clearAll();
	m_CFDRateFactory->clearAll();
	m_FeeFactory->clearAll();
	m_CurrencyFactory->clearAll();
	m_LiquidityFactory->clearAll();
	m_AccountFactory->clearAll();
	m_MemberPositionFactory->clearAll();
	m_PositionFactory->clearAll();
	m_MarketDataFactory->clearAll();
	m_OrderFactory->clearAll();
	m_PositionOrderFactory->clearAll();
	m_TriggerOrderFactory->clearAll();
	m_MarketOrderFactory->clearAll();
	m_TickMarketOrderFactory->clearAll();
	m_TradeFactory->clearAll();
	m_KLineFactory->clearAll();
	m_PublishKLineFactory->clearAll();
	m_LastKLineFactory->clearAll();
	m_AccountDetailFactory->clearAll();
	m_SettleDetailFactory->clearAll();
}

void CMDB::linkAll()
{
	m_InstrumentFactory->linkAllMarketData();
	m_InstrumentFactory->linkAllClearCurrency();
	m_InstrumentFactory->linkAllBaseCurrency();
	m_CurrencyFactory->linkAllMarketData();
	m_AccountFactory->linkAllCurrency();
	m_AccountFactory->linkAllMember();
	m_PositionFactory->linkAllInstrument();
	m_PositionFactory->linkAllAccount();
	m_MarketDataFactory->linkAllInstrument();
	m_OrderFactory->linkAllInstrument();
	m_OrderFactory->linkAllPosition();
	m_OrderFactory->linkAllMember();
	m_OrderFactory->linkAllTriggerOrder();
	m_TriggerOrderFactory->linkAllOrder();
	m_TriggerOrderFactory->linkAllMember();
	m_TriggerOrderFactory->linkAllInstrument();
	m_TriggerOrderFactory->linkAllMarketData();
}

void CMDB::checkNull(FILE *pFile) const
{
	///完成当前时间的NULL检查
	m_CurrentTimeFactory->checkNull(pFile);
	///完成成员的NULL检查
	m_MemberFactory->checkNull(pFile);
	///完成业务配置的NULL检查
	m_ServiceConfigFactory->checkNull(pFile);
	///完成交易员在线会话的NULL检查
	m_UserSessionFactory->checkNull(pFile);
	///完成标的的NULL检查
	m_InstrumentFactory->checkNull(pFile);
	///完成交易权限的NULL检查
	m_TradingRightFactory->checkNull(pFile);
	///完成标的保证金率的NULL检查
	m_MarginRateFactory->checkNull(pFile);
	///完成CFD溢价率的NULL检查
	m_CFDRateFactory->checkNull(pFile);
	///完成手续费率的NULL检查
	m_FeeFactory->checkNull(pFile);
	///完成资金账户币种信息的NULL检查
	m_CurrencyFactory->checkNull(pFile);
	///完成流动性设置表的NULL检查
	m_LiquidityFactory->checkNull(pFile);
	///完成资金账户的NULL检查
	m_AccountFactory->checkNull(pFile);
	///完成成员总持仓的NULL检查
	m_MemberPositionFactory->checkNull(pFile);
	///完成交易单元持仓的NULL检查
	m_PositionFactory->checkNull(pFile);
	///完成行情的NULL检查
	m_MarketDataFactory->checkNull(pFile);
	///完成报单的NULL检查
	m_OrderFactory->checkNull(pFile);
	///完成未平仓报单的NULL检查
	m_PositionOrderFactory->checkNull(pFile);
	///完成报单的NULL检查
	m_TriggerOrderFactory->checkNull(pFile);
	///完成分价表的NULL检查
	m_MarketOrderFactory->checkNull(pFile);
	///完成Tick聚集分价表的NULL检查
	m_TickMarketOrderFactory->checkNull(pFile);
	///完成成交的NULL检查
	m_TradeFactory->checkNull(pFile);
	///完成K线的NULL检查
	m_KLineFactory->checkNull(pFile);
	///完成PublishK线的NULL检查
	m_PublishKLineFactory->checkNull(pFile);
	///完成最新K线的NULL检查
	m_LastKLineFactory->checkNull(pFile);
	///完成资金明细的NULL检查
	m_AccountDetailFactory->checkNull(pFile);
	///完成结算明细的NULL检查
	m_SettleDetailFactory->checkNull(pFile);
}

void CMDB::checkLink(FILE *pFile) const
{
	m_InstrumentFactory->checkLinkMarketData(pFile);
	m_InstrumentFactory->checkLinkClearCurrency(pFile);
	m_InstrumentFactory->checkLinkBaseCurrency(pFile);
	m_CurrencyFactory->checkLinkMarketData(pFile);
	m_AccountFactory->checkLinkCurrency(pFile);
	m_AccountFactory->checkLinkMember(pFile);
	m_PositionFactory->checkLinkInstrument(pFile);
	m_PositionFactory->checkLinkAccount(pFile);
	m_MarketDataFactory->checkLinkInstrument(pFile);
	m_OrderFactory->checkLinkInstrument(pFile);
	m_OrderFactory->checkLinkPosition(pFile);
	m_OrderFactory->checkLinkMember(pFile);
	m_OrderFactory->checkLinkTriggerOrder(pFile);
	m_TriggerOrderFactory->checkLinkOrder(pFile);
	m_TriggerOrderFactory->checkLinkMember(pFile);
	m_TriggerOrderFactory->checkLinkInstrument(pFile);
	m_TriggerOrderFactory->checkLinkMarketData(pFile);
}

void CMDB::clearLink()
{
	m_InstrumentFactory->clearLinkMarketData();
	m_InstrumentFactory->clearLinkClearCurrency();
	m_InstrumentFactory->clearLinkBaseCurrency();
	m_CurrencyFactory->clearLinkMarketData();
	m_AccountFactory->clearLinkCurrency();
	m_AccountFactory->clearLinkMember();
	m_PositionFactory->clearLinkInstrument();
	m_PositionFactory->clearLinkAccount();
	m_MarketDataFactory->clearLinkInstrument();
	m_OrderFactory->clearLinkInstrument();
	m_OrderFactory->clearLinkPosition();
	m_OrderFactory->clearLinkMember();
	m_OrderFactory->clearLinkTriggerOrder();
	m_TriggerOrderFactory->clearLinkOrder();
	m_TriggerOrderFactory->clearLinkMember();
	m_TriggerOrderFactory->clearLinkInstrument();
	m_TriggerOrderFactory->clearLinkMarketData();
}

void CMDB::checkValid(FILE *pFile) const
{
	///完成当前时间的NULL检查
	m_CurrentTimeFactory->checkValid(pFile);
	///完成成员的NULL检查
	m_MemberFactory->checkValid(pFile);
	///完成业务配置的NULL检查
	m_ServiceConfigFactory->checkValid(pFile);
	///完成交易员在线会话的NULL检查
	m_UserSessionFactory->checkValid(pFile);
	///完成标的的NULL检查
	m_InstrumentFactory->checkValid(pFile);
	///完成交易权限的NULL检查
	m_TradingRightFactory->checkValid(pFile);
	///完成标的保证金率的NULL检查
	m_MarginRateFactory->checkValid(pFile);
	///完成CFD溢价率的NULL检查
	m_CFDRateFactory->checkValid(pFile);
	///完成手续费率的NULL检查
	m_FeeFactory->checkValid(pFile);
	///完成资金账户币种信息的NULL检查
	m_CurrencyFactory->checkValid(pFile);
	///完成流动性设置表的NULL检查
	m_LiquidityFactory->checkValid(pFile);
	///完成资金账户的NULL检查
	m_AccountFactory->checkValid(pFile);
	///完成成员总持仓的NULL检查
	m_MemberPositionFactory->checkValid(pFile);
	///完成交易单元持仓的NULL检查
	m_PositionFactory->checkValid(pFile);
	///完成行情的NULL检查
	m_MarketDataFactory->checkValid(pFile);
	///完成报单的NULL检查
	m_OrderFactory->checkValid(pFile);
	///完成未平仓报单的NULL检查
	m_PositionOrderFactory->checkValid(pFile);
	///完成报单的NULL检查
	m_TriggerOrderFactory->checkValid(pFile);
	///完成分价表的NULL检查
	m_MarketOrderFactory->checkValid(pFile);
	///完成Tick聚集分价表的NULL检查
	m_TickMarketOrderFactory->checkValid(pFile);
	///完成成交的NULL检查
	m_TradeFactory->checkValid(pFile);
	///完成K线的NULL检查
	m_KLineFactory->checkValid(pFile);
	///完成PublishK线的NULL检查
	m_PublishKLineFactory->checkValid(pFile);
	///完成最新K线的NULL检查
	m_LastKLineFactory->checkValid(pFile);
	///完成资金明细的NULL检查
	m_AccountDetailFactory->checkValid(pFile);
	///完成结算明细的NULL检查
	m_SettleDetailFactory->checkValid(pFile);
}
