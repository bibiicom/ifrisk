#include "public.h"
#include "MarketDataService.h"
#include "CopyField.h"
#include "CDate.h"
#include "monitorIndex.h"

CMarketDataService::CMarketDataService(CServiceResponser *pServiceResponser, CMDB *pMDB)
{
	//在此处将变量初始化为0，因为考虑到下面添加了m_pMDSgStatusTrigger后，对行情发布接口
	//会进行调用，里面就需要用到这个变量，因此要在该trigger初始化之前将此变量初始化；另外，
	//此变量还会在InitSgData()中，再次进行初始化操作

	m_pMDB = pMDB;
	m_nLastMarketDataNo = UF_INT8_ZERO;
	m_pServiceResponser = pServiceResponser;
}

CMarketDataService::~CMarketDataService()
{
}

bool CMarketDataService::PublishMarketData(CReadOnlySecondsType &CurrTime, CReadOnlyNumberType &CurrMillisec, CTransaction *pTransaction)
{
	//本轮最大变动序号
	UF_INT8 nMaxModifyNo = m_nLastMarketDataNo;

	CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	for (; pMarketData != NULL; pMarketData = m_pMDB->m_MarketDataFactory->getNext())
	{
		//如果本行情变动序号不大于上次行情变动序号，说明不需要更新深度行情
		if (pMarketData->TNO <= m_nLastMarketDataNo)
		{
			//三秒钟一次
			if (CurrTime % 3 == 0 && CurrMillisec <= 333)
			//一秒钟发送一次
			//if (CurrMillisec <= 333)
			{
				CWriteableMarketData theWriteableMarketData;
				m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theWriteableMarketData);
				PublishDepthMarketData(&theWriteableMarketData, CurrTime, CurrMillisec, pTransaction);
			}
			continue;
		}
		if (pMarketData->TNO > nMaxModifyNo)
			nMaxModifyNo = pMarketData->TNO;
		
		CWriteableMarketData theWriteableMarketData;
		m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theWriteableMarketData);

		PublishDepthMarketData(&theWriteableMarketData, CurrTime, CurrMillisec, pTransaction);
		Publish25MarketOrder(&theWriteableMarketData);
		PublishMarketOrder(&theWriteableMarketData);
		PublishLastKLine(&theWriteableMarketData);

		CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->findByInstrumentID(pMarketData->ExchangeID, pMarketData->InstrumentID);
		if (pInstrument != NULL)
		{
			if (!pInstrument->MarketTick1.isNull() && pInstrument->MarketTick1 > 0)
			{
				PublishTickUpDepthMarketData(&theWriteableMarketData, pInstrument->MarketTick1);
				PublishTickUp25MarketOrder(&theWriteableMarketData, pInstrument->MarketTick1);
			}
			if (!pInstrument->MarketTick2.isNull() && pInstrument->MarketTick2 > 0)
			{
				PublishTickUpDepthMarketData(&theWriteableMarketData, pInstrument->MarketTick2);
				PublishTickUp25MarketOrder(&theWriteableMarketData, pInstrument->MarketTick2);
			}
		}

		theWriteableMarketData.LastVolume = pMarketData->Volume;
		m_pMDB->m_MarketDataFactory->update(pMarketData, &theWriteableMarketData, pTransaction);
	}
	m_pMDB->m_MarketDataFactory->endGet();
	m_nLastMarketDataNo = nMaxModifyNo;
	return true;
}

bool CMarketDataService::PublishDepthMarketData(CWriteableMarketData *pMarketData, CReadOnlySecondsType &CurrTime, CReadOnlyNumberType &CurrMillisec, CTransaction *pTransaction)
{
	string SubIndexName = pMarketData->ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += pMarketData->InstrumentID.getValue();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
	//这里需要对UserID进行清空，要不然会产生IndexFlow的分支，浪费资源
	m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
	//	sprintf(m_pServiceResponser->GetMetaRef()->Remark, "%d,%d,%d", OF_Topic_Market_Depth_5, OF_Topic_Market_Depth_10, OF_Topic_Broad);

	CMarketDataOverViewField field;
	memset(&field, 0, sizeof(field));
	field.ProductGroup = pMarketData->ProductGroup;
	field.InstrumentStatus = pMarketData->InstrumentStatus;
	CopyMarketDataLastMatchEntity(&field, pMarketData);

	if (!g_bDoubleMarketVolume)
	{
		field.Volume = field.Volume / 2;
		field.OpenInterest = field.OpenInterest / 2;
		field.Turnover = field.Turnover / 2;
	}

	field.ExchangeID = pMarketData->ExchangeID;
	field.InstrumentID = pMarketData->InstrumentID;
	field.UpdateMilliSecond = pMarketData->UpdateMilliSecond;
	field.UpdateTime = pMarketData->UpdateTime;
	field.AskPrice1.clear();
	field.BidPrice1.clear();

	CFiveDepthMarketDataField fivefield;
	memset(&fivefield, 0, sizeof(fivefield));
	CopyMarketDataUpdateTimeEntity(&fivefield, &field);
	fivefield.BidPrice1.clear();
	fivefield.BidPrice2.clear();
	fivefield.BidPrice3.clear();
	fivefield.BidPrice4.clear();
	fivefield.BidPrice5.clear();
	fivefield.AskPrice1.clear();
	fivefield.AskPrice2.clear();
	fivefield.AskPrice3.clear();
	fivefield.AskPrice4.clear();
	fivefield.AskPrice5.clear();

	//CTenDepthMarketDataField tenfield;
	//memset(&tenfield, 0, sizeof(tenfield));
	//CopyMarketDataUpdateTimeEntity(&tenfield, &field);
	//tenfield.BidPrice1.clear();
	//tenfield.BidPrice2.clear();
	//tenfield.BidPrice3.clear();
	//tenfield.BidPrice4.clear();
	//tenfield.BidPrice5.clear();
	//tenfield.BidPrice6.clear();
	//tenfield.BidPrice7.clear();
	//tenfield.BidPrice8.clear();
	//tenfield.BidPrice9.clear();
	//tenfield.BidPrice10.clear();
	//tenfield.AskPrice1.clear();
	//tenfield.AskPrice2.clear();
	//tenfield.AskPrice3.clear();
	//tenfield.AskPrice4.clear();
	//tenfield.AskPrice5.clear();
	//tenfield.AskPrice6.clear();
	//tenfield.AskPrice7.clear();
	//tenfield.AskPrice8.clear();
	//tenfield.AskPrice9.clear();
	//tenfield.AskPrice10.clear();

	int nLevel = 1;
	CMarketOrders *pMarketOrders = m_pMDB->m_MarketOrdersFactory->findFirstByBuyMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
	while (pMarketOrders != NULL)
	{
		if (nLevel > 5)
			break;

		switch (nLevel)
		{
		case 1:
		{
			pMarketData->BidPrice1 = pMarketOrders->Price; pMarketData->BidVolume1 = pMarketOrders->Volume;
			field.BidPrice1 = pMarketOrders->Price; field.BidVolume1 = pMarketOrders->Volume;
			fivefield.BidPrice1 = pMarketOrders->Price; fivefield.BidVolume1 = pMarketOrders->Volume;
//			tenfield.BidPrice1 = pMarketOrders->Price; tenfield.BidVolume1 = pMarketOrders->Volume;
			break;
		}
		case 2:
		{
			fivefield.BidPrice2 = pMarketOrders->Price; fivefield.BidVolume2 = pMarketOrders->Volume;
//			tenfield.BidPrice2 = pMarketOrders->Price;	tenfield.BidVolume2 = pMarketOrders->Volume;
			break;
		}
		case 3:
		{
			fivefield.BidPrice3 = pMarketOrders->Price; fivefield.BidVolume3 = pMarketOrders->Volume;
//			tenfield.BidPrice3 = pMarketOrders->Price;	tenfield.BidVolume3 = pMarketOrders->Volume;
			break;
		}
		case 4:
		{
			fivefield.BidPrice4 = pMarketOrders->Price; fivefield.BidVolume4 = pMarketOrders->Volume;
//			tenfield.BidPrice4 = pMarketOrders->Price;	tenfield.BidVolume4 = pMarketOrders->Volume;
			break;
		}
		case 5:
		{
			fivefield.BidPrice5 = pMarketOrders->Price; fivefield.BidVolume5 = pMarketOrders->Volume;
//			tenfield.BidPrice5 = pMarketOrders->Price;	tenfield.BidVolume5 = pMarketOrders->Volume;
			break;
		}
		//case 6:
		//{
		//	tenfield.BidPrice6 = pMarketOrders->Price;	tenfield.BidVolume6 = pMarketOrders->Volume;
		//	break;
		//}
		//case 7:
		//{
		//	tenfield.BidPrice7 = pMarketOrders->Price;	tenfield.BidVolume7 = pMarketOrders->Volume;
		//	break;
		//}
		//case 8:
		//{
		//	tenfield.BidPrice8 = pMarketOrders->Price;	tenfield.BidVolume8 = pMarketOrders->Volume;
		//	break;
		//}
		//case 9:
		//{
		//	tenfield.BidPrice9 = pMarketOrders->Price;	tenfield.BidVolume9 = pMarketOrders->Volume;
		//	break;
		//}
		//case 10:
		//{
		//	tenfield.BidPrice10 = pMarketOrders->Price; tenfield.BidVolume9 = pMarketOrders->Volume;
		//	break;
		//}
		}

		pMarketOrders = m_pMDB->m_MarketOrdersFactory->findNextByBuyMBLData();
		nLevel++;
	}
	m_pMDB->m_MarketOrdersFactory->endFindByBuyMBLData();

	nLevel = 1;
	pMarketOrders = m_pMDB->m_MarketOrdersFactory->findFirstBySellMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
	while (pMarketOrders != NULL)
	{
		if (nLevel > 5)
			break;

		switch (nLevel)
		{
		case 1:
		{
			pMarketData->AskPrice1 = pMarketOrders->Price; pMarketData->AskVolume1 = pMarketOrders->Volume;
			field.AskPrice1 = pMarketOrders->Price; field.AskVolume1 = pMarketOrders->Volume;
			fivefield.AskPrice1 = pMarketOrders->Price; fivefield.AskVolume1 = pMarketOrders->Volume;
//			tenfield.AskPrice1 = pMarketOrders->Price; tenfield.AskVolume1 = pMarketOrders->Volume;
			break;
		}
		case 2:
		{
			fivefield.AskPrice2 = pMarketOrders->Price; fivefield.AskVolume2 = pMarketOrders->Volume;
//			tenfield.AskPrice2 = pMarketOrders->Price;	tenfield.AskVolume2 = pMarketOrders->Volume;
			break;
		}
		case 3:
		{
			fivefield.AskPrice3 = pMarketOrders->Price; fivefield.AskVolume3 = pMarketOrders->Volume;
//			tenfield.AskPrice3 = pMarketOrders->Price;	tenfield.AskVolume3 = pMarketOrders->Volume;
			break;
		}
		case 4:
		{
			fivefield.AskPrice4 = pMarketOrders->Price; fivefield.AskVolume4 = pMarketOrders->Volume;
//			tenfield.AskPrice4 = pMarketOrders->Price;	tenfield.AskVolume4 = pMarketOrders->Volume;
			break;
		}
		case 5:
		{
			fivefield.AskPrice5 = pMarketOrders->Price; fivefield.AskVolume5 = pMarketOrders->Volume;
//			tenfield.AskPrice5 = pMarketOrders->Price;	tenfield.AskVolume5 = pMarketOrders->Volume;
			break;
		}
		//case 6:
		//{
		//	tenfield.AskPrice6 = pMarketOrders->Price;	tenfield.AskVolume6 = pMarketOrders->Volume;
		//	break;
		//}
		//case 7:
		//{
		//	tenfield.AskPrice7 = pMarketOrders->Price;	tenfield.AskVolume7 = pMarketOrders->Volume;
		//	break;
		//}
		//case 8:
		//{
		//	tenfield.AskPrice8 = pMarketOrders->Price;	tenfield.AskVolume8 = pMarketOrders->Volume;
		//	break;
		//}
		//case 9:
		//{
		//	tenfield.AskPrice9 = pMarketOrders->Price;	tenfield.AskVolume9 = pMarketOrders->Volume;
		//	break;
		//}
		//case 10:
		//{
		//	tenfield.AskPrice10 = pMarketOrders->Price; tenfield.AskVolume10 = pMarketOrders->Volume;
		//	break;
		//}
		}

		pMarketOrders = m_pMDB->m_MarketOrdersFactory->findNextBySellMBLData();
		nLevel++;
	}
	m_pMDB->m_MarketOrdersFactory->endFindBySellMBLData();

	//if (pMarketData->UnderlyingPrice.isNull() && !field.AskPrice1.isNull() && !field.BidPrice1.isNull())
	//{
	//	pMarketData->MarkedPrice = (field.AskPrice1.getValue() + field.BidPrice1.getValue()) / 2;
	//}

	m_pServiceResponser->SendToSubjectID(TID_PushFiveDepthMarketData, &fivefield, &(fivefield.m_Describe), OF_Topic_Market_Depth_5, OUTFOG_CHAIN_SINGLE);
//	m_pServiceResponser->SendToSubjectID(TID_PushTenDepthMarketData, &tenfield, &(tenfield.m_Describe), OF_Topic_Market_Depth_10, OUTFOG_CHAIN_SINGLE);
	m_pServiceResponser->SendToSubjectID(TID_PushMarketDataOverView, &field, &(field.m_Describe), OF_Topic_Market_OverView, OUTFOG_CHAIN_SINGLE);

	if (CurrTime % 3 == 0 && CurrMillisec <= 333)
	//if (CurrMillisec <= 333)
	{
		sprintf(m_pServiceResponser->GetMetaRef()->SubIndexName, "%s", field.ProductGroup.getValue());
		m_pServiceResponser->SendToSubjectID(TID_PushMarketDataOverView, &field, &(field.m_Describe), OF_Topic_Market_OverView, OUTFOG_CHAIN_SINGLE);
	}

	return true;
}

bool CMarketDataService::PublishTickUpDepthMarketData(CWriteableMarketData *pMarketData, CPriceType Tick)
{
	string SubIndexName = pMarketData->ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += pMarketData->InstrumentID.getValue();
	SubIndexName += "_";
	SubIndexName += Tick.getString();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
	m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
	//sprintf(m_pServiceResponser->GetMetaRef()->Remark, "%d,%d,%d", OF_Topic_Market_Depth_5, OF_Topic_Market_Depth_10, OF_Topic_Broad);

	CFiveDepthMarketDataField fivefield;
	memset(&fivefield, 0, sizeof(fivefield));
	CopyMarketDataUpdateTimeEntity(&fivefield, pMarketData);
	fivefield.BidPrice1.clear();
	fivefield.BidPrice2.clear();
	fivefield.BidPrice3.clear();
	fivefield.BidPrice4.clear();
	fivefield.BidPrice5.clear();
	fivefield.AskPrice1.clear();
	fivefield.AskPrice2.clear();
	fivefield.AskPrice3.clear();
	fivefield.AskPrice4.clear();
	fivefield.AskPrice5.clear();

	int nLevel = 1;
	CTickMarketOrders *pMarketOrders = m_pMDB->m_TickMarketOrdersFactory->findFirstByBuyMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID, Tick);
	while (pMarketOrders != NULL)
	{
		if (nLevel > 5)
			break;
		if (pMarketOrders->Volume == 0.0)
		{
			pMarketOrders = m_pMDB->m_TickMarketOrdersFactory->findNextByBuyMBLData();
			continue;
		}
		
		switch (nLevel)
		{
		case 1:
			fivefield.BidPrice1 = pMarketOrders->Price; fivefield.BidVolume1 = pMarketOrders->Volume;
			break;
		case 2:
			fivefield.BidPrice2 = pMarketOrders->Price; fivefield.BidVolume2 = pMarketOrders->Volume;
			break;
		case 3:
			fivefield.BidPrice3 = pMarketOrders->Price; fivefield.BidVolume3 = pMarketOrders->Volume;
			break;
		case 4:
			fivefield.BidPrice4 = pMarketOrders->Price; fivefield.BidVolume4 = pMarketOrders->Volume;
			break;
		case 5:
			fivefield.BidPrice5 = pMarketOrders->Price; fivefield.BidVolume5 = pMarketOrders->Volume;
			break;
		}
		pMarketOrders = m_pMDB->m_TickMarketOrdersFactory->findNextByBuyMBLData();
		nLevel++;
	}
	m_pMDB->m_TickMarketOrdersFactory->endFindByBuyMBLData();

	nLevel = 1;
	pMarketOrders = m_pMDB->m_TickMarketOrdersFactory->findFirstBySellMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID, Tick);
	while (pMarketOrders != NULL)
	{
		if (nLevel > 5)
			break;
		if (pMarketOrders->Volume == 0.0)
		{
			pMarketOrders = m_pMDB->m_TickMarketOrdersFactory->findNextBySellMBLData();
			continue;
		}

		switch (nLevel)
		{
		case 1:
			fivefield.AskPrice1 = pMarketOrders->Price; fivefield.AskVolume1 = pMarketOrders->Volume;
			break;
		case 2:
			fivefield.AskPrice2 = pMarketOrders->Price; fivefield.AskVolume2 = pMarketOrders->Volume;
			break;
		case 3:
			fivefield.AskPrice3 = pMarketOrders->Price; fivefield.AskVolume3 = pMarketOrders->Volume;
			break;
		case 4:
			fivefield.AskPrice4 = pMarketOrders->Price; fivefield.AskVolume4 = pMarketOrders->Volume;
			break;
		case 5:
			fivefield.AskPrice5 = pMarketOrders->Price; fivefield.AskVolume5 = pMarketOrders->Volume;
			break;
		}
		pMarketOrders = m_pMDB->m_TickMarketOrdersFactory->findNextBySellMBLData();
		nLevel++;
	}
	m_pMDB->m_TickMarketOrdersFactory->endFindBySellMBLData();
	m_pServiceResponser->SendToSubjectID(TID_PushFiveDepthMarketData, &fivefield, &(fivefield.m_Describe), OF_Topic_Market_Depth_5_TickUp, OUTFOG_CHAIN_SINGLE);
	return true;
}

void CMarketDataService::SetMarketOrder(const char *IndexName, CMarketOrderField *pField)
{
	if (pField->Direction == D_Buy)
	{
		CInstrumentMarketOrderMap::iterator iter = m_BuyMarketOrderMap.find(IndexName);
		if (iter == m_BuyMarketOrderMap.end())
		{
			CMarketOrderMap MarketOrderMap;
			CMarketOrderField field;
			memcpy(&field, pField, sizeof(field));
			MarketOrderMap.insert(CMarketOrderMap::value_type(field.Price.getValue(), field));
			m_BuyMarketOrderMap.insert(CInstrumentMarketOrderMap::value_type(IndexName, MarketOrderMap));
		}
		else
		{
			CMarketOrderMap::iterator iter1 = iter->second.find(pField->Price.getValue());
			if (iter1 == iter->second.end())
			{
				CMarketOrderField field;
				memcpy(&field, pField, sizeof(field));
				iter->second.insert(CMarketOrderMap::value_type(field.Price.getValue(), field));
			}
			else
			{
				iter1->second.Volume = pField->Volume;
				iter1->second.OrderNumber = pField->OrderNumber;
//				iter1->second.ChangeType = pField->ChangeType;
			}
		}
	}
	else
	{
		CInstrumentMarketOrderMap::iterator iter = m_SellMarketOrderMap.find(IndexName);
		if (iter == m_SellMarketOrderMap.end())
		{
			CMarketOrderMap MarketOrderMap;
			CMarketOrderField field;
			memcpy(&field, pField, sizeof(field));
			MarketOrderMap.insert(CMarketOrderMap::value_type(field.Price.getValue(), field));
			m_SellMarketOrderMap.insert(CInstrumentMarketOrderMap::value_type(IndexName, MarketOrderMap));
		}
		else
		{
			CMarketOrderMap::iterator iter1 = iter->second.find(pField->Price.getValue());
			if (iter1 == iter->second.end())
			{
				CMarketOrderField field;
				memcpy(&field, pField, sizeof(field));
				iter->second.insert(CMarketOrderMap::value_type(field.Price.getValue(), field));
			}
			else
			{
				iter1->second.Volume = pField->Volume;
				iter1->second.OrderNumber = pField->OrderNumber;
//				iter1->second.ChangeType = pField->ChangeType;
			}
		}
	}
}

bool CMarketDataService::Publish25MarketOrder(CWriteableMarketData *pMarketData)
{
	string SubIndexName = pMarketData->ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += pMarketData->InstrumentID.getValue();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
	m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';

	m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, NULL, NULL, OF_Topic_Market_Depth_25, OUTFOG_CHAIN_FIRST);

	CMarketOrders* pMarketOrders = m_pMDB->m_MarketOrdersFactory->findFirstByBuyMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
	int i = 0;
	int nLevel = 0;
	CPriceType L25Price = pMarketData->L25LowerPrice;
	while (pMarketOrders != NULL)
	{
		if (nLevel >= 25)
			break;

		if (pMarketOrders->TNO > m_nLastMarketDataNo)
		{
			CMarketOrderField field;
			field.Price = pMarketOrders->Price;
			field.ExchangeID = pMarketOrders->ExchangeID;
			field.InstrumentID = pMarketOrders->InstrumentID;
//			field.ChangeType = DAT_Update;
			field.Direction = pMarketOrders->Direction;
			field.Volume = pMarketOrders->Volume;
			field.OrderNumber = pMarketOrders->OrderNumber;
			m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
			i++;
		}
		else
		{
			if (!pMarketData->L25LowerPrice.isNull() && pMarketOrders->Price < pMarketData->L25LowerPrice)
			{
				CMarketOrderField field;
				field.Price = pMarketOrders->Price;
				field.ExchangeID = pMarketOrders->ExchangeID;
				field.InstrumentID = pMarketOrders->InstrumentID;
//				field.ChangeType = DAT_Insert;
				field.Direction = pMarketOrders->Direction;
				field.Volume = pMarketOrders->Volume;
				field.OrderNumber = pMarketOrders->OrderNumber;
				m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
				i++;
			}
		}
		L25Price = pMarketOrders->Price;
		pMarketOrders = m_pMDB->m_MarketOrdersFactory->findNextByBuyMBLData();
		nLevel++;
	}
	pMarketData->L25LowerPrice = L25Price;
	m_pMDB->m_MarketOrdersFactory->endFindByBuyMBLData();

	//删除的订单需要考虑
	CInstrumentMarketOrderMap::iterator iter = m_BuyMarketOrderMap.find(SubIndexName.c_str());
	if (iter != m_BuyMarketOrderMap.end())
	{
		CMarketOrderMap::iterator iter1 = iter->second.begin();
		while (iter1 != iter->second.end())
		{
			if (iter1->second.Volume == 0.0)
			{
				m_pServiceResponser->SendNext(&(iter1->second), &(iter1->second.m_Describe), OUTFOG_CHAIN_CONTINUE);
				i++;
			}
			iter1++;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	pMarketOrders = m_pMDB->m_MarketOrdersFactory->findFirstBySellMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
	nLevel = 0;
	L25Price = pMarketData->L25UpperPrice;
	while (pMarketOrders != NULL)
	{
		if (nLevel >= 25)
			break;

		if (pMarketOrders->TNO > m_nLastMarketDataNo)
		{
			CMarketOrderField field;
			field.Price = pMarketOrders->Price;
			field.ExchangeID = pMarketOrders->ExchangeID;
			field.InstrumentID = pMarketOrders->InstrumentID;
//			field.ChangeType = DAT_Update;
			field.Direction = pMarketOrders->Direction;
			field.Volume = pMarketOrders->Volume;
			field.OrderNumber = pMarketOrders->OrderNumber;
			m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
			i++;
		}
		else
		{
			if (!pMarketData->L25UpperPrice.isNull() && pMarketOrders->Price > pMarketData->L25UpperPrice)
			{
				CMarketOrderField field;
				field.Price = pMarketOrders->Price;
				field.ExchangeID = pMarketOrders->ExchangeID;
				field.InstrumentID = pMarketOrders->InstrumentID;
//				field.ChangeType = DAT_Insert;
				field.Direction = pMarketOrders->Direction;
				field.Volume = pMarketOrders->Volume;
				field.OrderNumber = pMarketOrders->OrderNumber;
				m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
				i++;
			}
		}
		L25Price = pMarketOrders->Price;
		pMarketOrders = m_pMDB->m_MarketOrdersFactory->findNextBySellMBLData();
		nLevel++;
	}
	pMarketData->L25UpperPrice = L25Price;
	m_pMDB->m_MarketOrdersFactory->endFindBySellMBLData();

	iter = m_SellMarketOrderMap.find(SubIndexName.c_str());
	if (iter != m_SellMarketOrderMap.end())
	{
		CMarketOrderMap::iterator iter1 = iter->second.begin();
		while (iter1 != iter->second.end())
		{
			if (iter1->second.Volume == 0.0)
			{
				m_pServiceResponser->SendNext(&(iter1->second), &(iter1->second.m_Describe), OUTFOG_CHAIN_CONTINUE);
				i++;
			}
			iter1++;
		}
	}

	if (i > 0)
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);

	return true;
}

bool CMarketDataService::PublishTickUp25MarketOrder(CWriteableMarketData *pMarketData, CPriceType Tick)
{
	string SubIndexName = pMarketData->ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += pMarketData->InstrumentID.getValue();
	SubIndexName += "_";
	SubIndexName += Tick.getString();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
	m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
	m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, NULL, NULL, OF_Topic_Market_Depth_25_TickUp, OUTFOG_CHAIN_FIRST);

	CTickMarketOrders* pMarketOrders = m_pMDB->m_TickMarketOrdersFactory->findFirstByBuyMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID, Tick);
	int i = 0;
	int nLevel = 0;
	while (pMarketOrders != NULL)
	{
		if (nLevel >= 25)
			break;

		if (pMarketOrders->TNO > m_nLastMarketDataNo)
		{
			CMarketOrderField field;
			field.Price = pMarketOrders->Price;
			field.ExchangeID = pMarketOrders->ExchangeID;
			field.InstrumentID = pMarketOrders->InstrumentID;
//			field.ChangeType = DAT_Update;
			field.Direction = pMarketOrders->Direction;
			field.Volume = pMarketOrders->Volume;
			field.OrderNumber = pMarketOrders->OrderNumber;
			m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
			i++;
		}
		//if (pMarketOrders->Volume == 0.0)
			nLevel++;
		pMarketOrders = m_pMDB->m_TickMarketOrdersFactory->findNextByBuyMBLData();
	}
	m_pMDB->m_TickMarketOrdersFactory->endFindByBuyMBLData();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	pMarketOrders = m_pMDB->m_TickMarketOrdersFactory->findFirstBySellMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID, Tick);
	nLevel = 0;
	while (pMarketOrders != NULL)
	{
		if (nLevel >= 25)
			break;

		if (pMarketOrders->TNO > m_nLastMarketDataNo)
		{
			CMarketOrderField field;
			field.Price = pMarketOrders->Price;
			field.ExchangeID = pMarketOrders->ExchangeID;
			field.InstrumentID = pMarketOrders->InstrumentID;
//			field.ChangeType = DAT_Update;
			field.Direction = pMarketOrders->Direction;
			field.Volume = pMarketOrders->Volume;
			field.OrderNumber = pMarketOrders->OrderNumber;
			m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
			i++;
		}

		//if (pMarketOrders->Volume == 0.0)
			nLevel++;
		pMarketOrders = m_pMDB->m_TickMarketOrdersFactory->findNextBySellMBLData();
	}
	m_pMDB->m_TickMarketOrdersFactory->endFindBySellMBLData();

	if (i > 0)
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);

	return true;
}

bool CMarketDataService::PublishMarketOrder(CWriteableMarketData *pMarketData)
{
	string SubIndexName = pMarketData->ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += pMarketData->InstrumentID.getValue();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
	m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
	int i = 0;

	m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, NULL, NULL, OF_Topic_Market_Depth_All, OUTFOG_CHAIN_FIRST);

	CInstrumentMarketOrderMap::iterator iter = m_BuyMarketOrderMap.find(SubIndexName.c_str());
	if (iter != m_BuyMarketOrderMap.end())
	{
		CMarketOrderMap::iterator iter1 = iter->second.begin();
		while (iter1 != iter->second.end())
		{
			m_pServiceResponser->SendNext(&(iter1->second), &(iter1->second.m_Describe), OUTFOG_CHAIN_CONTINUE);
			iter1++;
			i++;
		}
		iter->second.clear();
	}
	iter = m_SellMarketOrderMap.find(SubIndexName.c_str());
	if (iter != m_SellMarketOrderMap.end())
	{
		CMarketOrderMap::iterator iter1 = iter->second.begin();
		while (iter1 != iter->second.end())
		{
			m_pServiceResponser->SendNext(&(iter1->second), &(iter1->second.m_Describe), OUTFOG_CHAIN_CONTINUE);
			iter1++;
			i++;
		}
		iter->second.clear();
	}

	if (i > 0)
		m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	return true;
}

bool CMarketDataService::PublishLastKLine(CWriteableMarketData *pMarketData)
{
	if (pMarketData->Volume == pMarketData->LastVolume)
		return false;
	string SubIndexName = pMarketData->ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += pMarketData->InstrumentID.getValue();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
	CLastKLineField field;
#if 0
	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriod(pMarketData->ExchangeID, pMarketData->InstrumentID, "1m");
	m_pMDB->m_LastKLineFactory->endFindByHisPeriod();
#else
	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriodBeginTime(pMarketData->ExchangeID, pMarketData->InstrumentID, "1m");
	m_pMDB->m_LastKLineFactory->endFindByHisPeriodBeginTime();
#endif
	if (pLastKLine != NULL)
	{
		CopyLastKLineEntity(&field, pLastKLine);
		m_pServiceResponser->SendToSubjectID(TID_PushKLine, &field, &(field.m_Describe), OF_Topic_Market_LastKLine, OUTFOG_CHAIN_SINGLE);
	}
	return true;
}