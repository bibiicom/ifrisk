#include "IndexWorker.h"
#include "profile.h"
#include "public.h"

CIndexWorker::CIndexWorker(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CWorkerCallback *pMarketMaker)
	:CWorkerInterface(ExchangeID, InstrumentID, pMarketMaker)
{
	m_pMarketMaker = pMarketMaker;
	m_OTCTradeInsertField.ExchangeID = ExchangeID;
	m_OTCTradeInsertField.InstrumentID = InstrumentID;

	m_MarketDataUpdateField.ExchangeID = ExchangeID;
	m_MarketDataUpdateField.InstrumentID = InstrumentID;

	m_MarketDataUpdateField.MarkedPrice.clear();
	m_MarketDataUpdateField.PreSettlementPrice.clear();
	m_MarketDataUpdateField.SettlementPrice.clear();
	m_MarketDataUpdateField.PrePositionFeeRate.clear();
	m_MarketDataUpdateField.InstrumentStatus.clear();
	m_MarketDataUpdateField.Volume.clear();
	m_MarketDataUpdateField.Turnover.clear();
	m_MarketDataUpdateField.UnderlyingPrice.clear();
	m_MarketDataUpdateField.PositionFeeRate.clear();
}

CIndexWorker::~CIndexWorker()
{
}

bool CIndexWorker::SaveMarketOverView(CMarketOverViewField *pField)
{
	m_MarketDataUpdateField.UnderlyingPrice = pField->UnderlyingPrice;
	m_MarketDataUpdateField.PositionFeeRate = pField->PositionFeeRate;
	m_MarketDataUpdateField.PrePositionFeeRate = pField->PrePositionFeeRate;
	return m_pMarketMaker->SendMarketUpdate(&m_MarketDataUpdateField);
}

bool CIndexWorker::SaveMarketTrade(CMarketTradeField *pField)
{
	//return m_pMarketMaker->SendMarketUpdate(&m_MarketDataUpdateField);
	return true;
}

