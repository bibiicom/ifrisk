#include "MarketWorker.h"
#include "profile.h"
#include "public.h"
#include "monitorIndex.h"

CMarketWorker::CMarketWorker(CExchangeIDType ExchangeID, CInstrumentIDType InstrumentID, CWorkerCallback* pMarketMaker)
	:CWorkerInterface(ExchangeID, InstrumentID, pMarketMaker)
{
	m_pMarketMaker = pMarketMaker;
	memset(&m_OrderInsertField, 0, sizeof(m_OrderInsertField));
	m_OrderInsertField.ExchangeID = ExchangeID;
	m_OrderInsertField.InstrumentID = InstrumentID;
	m_OrderInsertField.Cost = DBL_MAX;
	m_OrderInsertField.VolumeDisplay = DBL_MAX;
	m_OrderInsertField.IsCrossMargin = true;

	memset(&m_OTCTradeInsertField, 0, sizeof(m_OTCTradeInsertField));
	m_OTCTradeInsertField.ExchangeID = ExchangeID;
	m_OTCTradeInsertField.InstrumentID = InstrumentID;
	m_OTCTradeInsertField.OTCType = OTCT_WashOnly;
	m_QryOrderField.ExchangeID = ExchangeID;
	m_QryOrderField.InstrumentID = InstrumentID;
	sprintf(m_PrintInfo, "%s:%s", ExchangeID.getValue(), InstrumentID.getValue());

	memset(&m_MarketDataUpdateField, 0, sizeof(m_MarketDataUpdateField));
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
	m_PriceTick = 1000000;
	m_nPriceTickCount = 0;
}

CMarketWorker::~CMarketWorker()
{
}

bool CMarketWorker::Init()
{
	m_SellMarketOrders.clear();
	m_BuyMarketOrders.clear();
	return true;
}

bool CMarketWorker::InitOrder(COrderField* pField)
{
	if (pField == NULL)
	{
		REPORT_EVENT(LOG_ERROR, "m_BuyMarketOrders", "%s -> size:%d", m_PrintInfo, m_BuyMarketOrders.size());
		REPORT_EVENT(LOG_ERROR, "m_SellMarketOrders", "%s -> size:%d", m_PrintInfo, m_SellMarketOrders.size());
		return true;
	}

	if (pField->Direction == D_Buy)
	{
		CBuyOrderMap::iterator iter = m_BuyMarketOrders.find(pField->Price.getValue());
		if (iter == m_BuyMarketOrders.end())
		{
			m_BuyMarketOrders.insert(CBuyOrderMap::value_type(pField->Price.getValue(), *pField));
		}
		else
		{
			m_pMarketMaker->CancelOrder(&(iter->second));
			memcpy(&(iter->second), pField, sizeof(COrderField));
		}
	}
	else
	{
		CSellOrderMap::iterator iter = m_SellMarketOrders.find(pField->Price.getValue());
		if (iter == m_SellMarketOrders.end())
		{
			m_SellMarketOrders.insert(CSellOrderMap::value_type(pField->Price.getValue(), *pField));
		}
		else
		{
			m_pMarketMaker->CancelOrder(&(iter->second));
			memcpy(&(iter->second), pField, sizeof(COrderField));
		}
	}
	return true;
}

bool CMarketWorker::SaveMarketTrade(CMarketTradeField* pField)
{
	m_OTCTradeInsertField.Price = pField->Price;
	m_OTCTradeInsertField.TakerDirection = pField->Direction;
	m_OTCTradeInsertField.Volume = pField->Volume;
	m_pMarketMaker->SendOTCTrade(&m_OTCTradeInsertField);

	if (m_nPriceTickCount == 1000)
	{
		CInstrumentUpdateField outfield;
		outfield.ExchangeID = m_MarketDataUpdateField.ExchangeID;
		outfield.InstrumentID = m_MarketDataUpdateField.InstrumentID;
		outfield.VolumeTick.clear();
		outfield.MaxOpenInterest.clear();
		outfield.MaxOrderVolume.clear();
		outfield.MinOrderCost.clear();
		outfield.MinOrderVolume.clear();
		outfield.BasisPrice.clear();
		outfield.PriceTick = m_PriceTick;
		m_pMarketMaker->SendInstrumentUpdate(&outfield);
		REPORT_EVENT(LOG_ERROR, "SaveMarketTrade", "%s ->PriceTick:[%s][%s][%s]", m_PrintInfo, outfield.ExchangeID.getValue(),
			outfield.InstrumentID.getValue(), outfield.PriceTick.getString().c_str());
		m_nPriceTickCount++;
	}
	else
	{
		if (m_nPriceTickCount < 1000)
		{
			if (m_PriceTick > pField->Price.getTick())
				m_PriceTick = pField->Price.getTick();
			m_nPriceTickCount++;
		}
	}
	return true;
}

bool CMarketWorker::SaveMarketOrder(CMarketOrderField* pField)
{
	//先清除多余的订单
	if (pField->Direction == D_Buy)
	{
		if (m_BuyMarketOrders.size() > MaxOrders * 1.5)
		{
			REPORT_EVENT(LOG_ERROR, "SaveMarketOrder", "%s -> size:%d, erase(buy) begin", m_PrintInfo, m_BuyMarketOrders.size());
			//保留OrderID不为空的MaxOrders的个数，其余全部撤单
			CBuyOrderMap::iterator iter = m_BuyMarketOrders.begin();
			int i = 0;
			while (iter != m_BuyMarketOrders.end())
			{
				i++;
				if (i > MaxOrders)
				{
					if (m_pMarketMaker->CancelOrder(&(iter->second)))
					{
						m_BuyMarketOrders.erase(iter++);
						continue;
					}
				}
				iter++;
			}
			REPORT_EVENT(LOG_ERROR, "SaveMarketOrder", "%s -> size:%d, erase(buy) end", m_PrintInfo, m_BuyMarketOrders.size());
		}
		//如果订单簿超过的限制，全部清除，以免出现
		if (m_BuyMarketOrders.size() > MaxOrders * 2)
		{
			REPORT_EVENT(LOG_ERROR, "SaveMarketOrder", "%s -> buySize is:%d, clean all", m_PrintInfo, m_BuyMarketOrders.size());
			m_BuyMarketOrders.clear();
			m_pMarketMaker->GetOrder(&m_QryOrderField);
			return false;
		}
	}
	else
	{
		if (m_SellMarketOrders.size() > MaxOrders * 1.5)
		{
			REPORT_EVENT(LOG_ERROR, "SaveMarketOrder", "%s -> size:%d, erase(sell) begin", m_PrintInfo, m_SellMarketOrders.size());
			//保留OrderID不为空的MaxOrders的个数，其余全部撤单
			CSellOrderMap::iterator iter = m_SellMarketOrders.begin();
			int i = 0;
			while (iter != m_SellMarketOrders.end())
			{
				i++;
				if (i > MaxOrders)
				{
					if (m_pMarketMaker->CancelOrder(&(iter->second)))
					{
						m_SellMarketOrders.erase(iter++);
						continue;
					}
				}
				iter++;
			}
			REPORT_EVENT(LOG_ERROR, "SaveMarketOrder", "%s -> size:%d, erase(sell) end", m_PrintInfo, m_SellMarketOrders.size());
		}
		if (m_SellMarketOrders.size() > MaxOrders * 2)
		{
			REPORT_EVENT(LOG_ERROR, "SaveMarketOrder", "%s -> sellSize is:%d, clean all", m_PrintInfo, m_SellMarketOrders.size());
			m_SellMarketOrders.clear();
			m_pMarketMaker->GetOrder(&m_QryOrderField);
			return false;
		}
	}
	return ChangeOrder(pField->Price, pField->Volume, pField->Direction);
}

bool CMarketWorker::ChangeOrder(CPriceType LastOrderPrice, CVolumeType Volume, CDirectionType Direction)
{
	if (LastOrderPrice.isNull())
		return false;

	if (Direction == D_Buy)
	{
		//先让之前的订单撤单
		CBuyOrderMap::iterator iter = m_BuyMarketOrders.find(LastOrderPrice.getValue());
		if (iter != m_BuyMarketOrders.end())
		{
			if (iter->second.OrderID.isNull())
			{
				if (iter->second.Volume == Volume)
					return false;
			}
			else
			{
				if (iter->second.VolumeRemain == Volume)
					return false;
			}

			if (!m_pMarketMaker->CancelOrder(&(iter->second)))
				return false;
			m_BuyMarketOrders.erase(iter);
			if (Volume == 0.0)
				return true;
		}
		else
		{
			if (Volume == 0.0)
				return true;

			//让对手方先撤单，避免由于订单发出的成交让本地重复撤单
			//如果本地缓存不及时消失，导致清理订单时残留太多订单
			CSellOrderMap::iterator iterother = m_SellMarketOrders.begin();
			while (iterother != m_SellMarketOrders.end())
			{
				//PRINT_TO_STD("find %f in m_SellMarketOrders for %f", iterother->second.Price.getValue(), LastOrderPrice.getValue());
				if (LastOrderPrice < iterother->first)
					break;
				if (m_pMarketMaker->CancelOrder(&(iterother->second)))
					m_SellMarketOrders.erase(iterother++);
				else
					iterother++;
			}
		}

		m_OrderInsertField.Price = LastOrderPrice;
		m_OrderInsertField.Volume = Volume;
		m_OrderInsertField.Direction = Direction;
		m_OrderInsertField.LocalID.clear();
		if (m_pMarketMaker->SendOrder(&m_OrderInsertField))
		{
			COrderField pNewField;
			pNewField.OrderID = "";
			pNewField.Price = LastOrderPrice;
			pNewField.Volume = Volume;
			pNewField.Direction = Direction;
			pNewField.LocalID = m_OrderInsertField.LocalID;
			pNewField.ExchangeID = m_OrderInsertField.ExchangeID;
			pNewField.InstrumentID = m_OrderInsertField.InstrumentID;
			m_BuyMarketOrders.insert(CBuyOrderMap::value_type(pNewField.Price.getValue(), pNewField));
		}
	}
	else
	{
		CSellOrderMap::iterator iter = m_SellMarketOrders.find(LastOrderPrice.getValue());
		if (iter != m_SellMarketOrders.end())
		{
			if (iter->second.OrderID.isNull())
			{
				if (iter->second.Volume == Volume)
					return false;
			}
			else
			{
				if (iter->second.VolumeRemain == Volume)
					return false;
			}
			if (!m_pMarketMaker->CancelOrder(&(iter->second)))
				return false;
			m_SellMarketOrders.erase(iter);
			if (Volume == 0.0)
				return true;
		}
		else
		{
			if (Volume == 0.0)
				return true;
			//让对手方先撤单，避免由于订单发出的成交让本地重复撤单
			//如果本地缓存不及时消失，导致清理订单时残留太多订单
			CBuyOrderMap::iterator iterother = m_BuyMarketOrders.begin();
			while (iterother != m_BuyMarketOrders.end())
			{
				if (LastOrderPrice > iterother->first)
					break;
				//PRINT_TO_STD("find %f in m_BuyMarketOrders for %f", iterother->second.Price.getValue(), LastOrderPrice.getValue());
				if (m_pMarketMaker->CancelOrder(&(iterother->second)))
					m_BuyMarketOrders.erase(iterother++);
				else
					iterother++;
			}
		}

		m_OrderInsertField.Price = LastOrderPrice;
		m_OrderInsertField.Volume = Volume;
		m_OrderInsertField.Direction = Direction;
		m_OrderInsertField.LocalID.clear();
		if (m_pMarketMaker->SendOrder(&m_OrderInsertField))
		{
			COrderField pNewField;
			pNewField.OrderID = "";
			pNewField.Price = LastOrderPrice;
			pNewField.Volume = Volume;
			pNewField.Direction = Direction;
			pNewField.LocalID = m_OrderInsertField.LocalID;
			pNewField.ExchangeID = m_OrderInsertField.ExchangeID;
			pNewField.InstrumentID = m_OrderInsertField.InstrumentID;
			m_SellMarketOrders.insert(CSellOrderMap::value_type(pNewField.Price.getValue(), pNewField));
		}
	}
	return true;
}

bool CMarketWorker::SaveMarketOverView(CMarketOverViewField* pField)
{
	if (pField->UpdateTime % 1000 == 0)
		m_nPriceTickCount = 0;
	m_MarketDataUpdateField.UnderlyingPrice = pField->UnderlyingPrice;
	m_MarketDataUpdateField.PositionFeeRate = pField->PositionFeeRate;
	m_MarketDataUpdateField.PrePositionFeeRate = pField->PrePositionFeeRate;
	m_MarketDataUpdateField.MarkedPrice = pField->MarkedPrice;
	return m_pMarketMaker->SendMarketUpdate(&m_MarketDataUpdateField);
}