#include "public.h"
#include "MarketDataService.h"
#include "CopyField.h"
#include "CDate.h"
#include "monitorIndex.h"
#include "Config.h"

#define NETWORK_FILE_NAME "network.ini"

int g_nMDSnapDepth = 0;
//int g_nPrivateBase = 0;
//int g_nPrivateDispatch = 0;
UF_INT8 g_nBusinessNo = 0;

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

bool CMarketDataService::PublishSnapMarketData(CCurrentTimeField* pField)
{
	if (g_nMDSnapDepth <= 0)
		return false;

	static int SnapMarketDataCountTime = 0;
	SnapMarketDataCountTime++;
	if (SnapMarketDataCountTime % 5 != 0)
		return false;
	SnapMarketDataCountTime = 0;

	CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	for (; pMarketData != NULL; pMarketData = m_pMDB->m_MarketDataFactory->getNext())
	{
		PublishSnapMarketData(pMarketData, pField);
		InstDescInfo& descInfo = m_InstDescMap[pMarketData->ExchangeID][pMarketData->InstrumentID];
		if (descInfo.desc != 0.0)
		{
			for (int i = 0; i < descInfo.tick.size(); i++)
			{
				PublishSnapTickMarketData(pMarketData, pField, descInfo.tick[i]);
			}
		}
	}
	return true;
}

//bool CMarketDataService::PublishDepthMarketData(CReadOnlyMilliSecondsType &CurrTime)
//{
//	static int DepthMarketDataCountTime = 0;
//	DepthMarketDataCountTime++;
//	if (DepthMarketDataCountTime % 3 != 0)
//		return false;
//	DepthMarketDataCountTime = 0;
//
//	CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
//	for (; pMarketData != NULL; pMarketData = m_pMDB->m_MarketDataFactory->getNext())
//	{
//		PublishDepthMarketData(pMarketData, CurrTime);
//	}
//	return true;
//}

bool CMarketDataService::PublishMarketData(CCurrentTimeField* pField)
{
	static int MarketDataCountTime = 0;
	MarketDataCountTime++;
	if (MarketDataCountTime % 3 != 0)
		return false;
	MarketDataCountTime = 0;

	CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	for (; pMarketData != NULL; pMarketData = m_pMDB->m_MarketDataFactory->getNext())
	{
		CWriteableMarketData theWriteableMarketData;
		m_pMDB->m_MarketDataFactory->retrieve(pMarketData, &theWriteableMarketData);
		Publish25MarketOrder(&theWriteableMarketData, pField);

		InstDescInfo& descInfo = m_InstDescMap[pMarketData->ExchangeID][pMarketData->InstrumentID];
		if (descInfo.desc != 0.0)
		{
			for (int i = 0; i < descInfo.tick.size(); i++)
			{
				Publish25TickMarketOrder(&theWriteableMarketData, descInfo.tick[i], descInfo.index[i].c_str(), pField);
			}
		}
		m_pMDB->m_MarketDataFactory->update(pMarketData, &theWriteableMarketData);
	}
	m_nLastMarketDataNo = m_nMaxMarketDataNo;
	return true;
}

bool CMarketDataService::PublishDelayMarketData(CCurrentTimeField* pField)
{
	if (g_nMDSnapDepth <= 0)
		return false;

	static int DelayMarketDataCountTime = 0;
	DelayMarketDataCountTime++;
	if (DelayMarketDataCountTime % 3 != 0)
		return false;
	DelayMarketDataCountTime = 0;

	CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->getFirst();
	for (; pMarketData != NULL; pMarketData = m_pMDB->m_MarketDataFactory->getNext())
	{
		PublishDelayMarketOrder(pMarketData, pField);
	}
	return true;
}

bool CMarketDataService::PublishSnapMarketData(CMarketData *pMarketData, CCurrentTimeField* pField)
{
	string SubIndexName = pMarketData->ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += pMarketData->InstrumentID.getValue();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());

	CMarketOrderField field;
	memset(&field, 0, sizeof(field));
	field.ExchangeID = pMarketData->ExchangeID;
	field.InstrumentID = pMarketData->InstrumentID;
	field.Direction = D_Buy;
	m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, NULL, NULL, OF_Topic_Market_SnapShot, 4, UF_INT8_MAX, OUTFOG_CHAIN_FIRST);
	int nBLevel = 1;
	CMarketOrder *pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
	while (pMarketOrder != NULL)
	{
		if (nBLevel > g_nMDSnapDepth)
			break;
		field.Price = pMarketOrder->Price;
		field.Volume = pMarketOrder->Volume;
		m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, &field, &(field.m_Describe), OF_Topic_Market_SnapShot, 4, UF_INT8_MAX, OUTFOG_CHAIN_CONTINUE);
		pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextByBuyMBLData();
		nBLevel++;
	}

	int nSLevel = 1;
	field.Direction = D_Sell;
	pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
	while (pMarketOrder != NULL)
	{
		if (nSLevel > g_nMDSnapDepth)
			break;
		field.Price = pMarketOrder->Price;
		field.Volume = pMarketOrder->Volume;
		m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, &field, &(field.m_Describe), OF_Topic_Market_SnapShot, 4, UF_INT8_MAX, OUTFOG_CHAIN_CONTINUE);
		pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextBySellMBLData();
		nSLevel++;
	}

	m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, NULL, NULL, OF_Topic_Market_SnapShot, 4, UF_INT8_MAX, OUTFOG_CHAIN_LAST);
	return true;
}

bool CMarketDataService::PublishSnapTickMarketData(CMarketData* pMarketData, CCurrentTimeField* pField, CPriceType Tick)
{
	string SubIndexName = pMarketData->ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += pMarketData->InstrumentID.getValue();
	SubIndexName += "_";
	SubIndexName += Tick.getString();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());

	CMarketOrderField field;
	memset(&field, 0, sizeof(field));
	field.ExchangeID = pMarketData->ExchangeID;
	field.InstrumentID = pMarketData->InstrumentID;
	field.Direction = D_Buy;
	m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, NULL, NULL, OF_Topic_Market_SnapShot, 4, UF_INT8_MAX, OUTFOG_CHAIN_FIRST);
	int nBLevel = 1;
	CTickMarketOrder* pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findFirstByBuyMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID, Tick);
	while (pMarketOrder != NULL)
	{
		if (nBLevel > g_nMDSnapDepth)
			break;
		field.Price = pMarketOrder->Price;
		field.Volume = pMarketOrder->Volume;
		m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, &field, &(field.m_Describe), OF_Topic_Market_SnapShot, 4, UF_INT8_MAX, OUTFOG_CHAIN_CONTINUE);
		pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextByBuyMBLData();
		nBLevel++;
	}

	int nSLevel = 1;
	field.Direction = D_Sell;
	pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findFirstBySellMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID, Tick);
	while (pMarketOrder != NULL)
	{
		if (nSLevel > g_nMDSnapDepth)
			break;
		field.Price = pMarketOrder->Price;
		field.Volume = pMarketOrder->Volume;
		m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, &field, &(field.m_Describe), OF_Topic_Market_SnapShot, 4, UF_INT8_MAX, OUTFOG_CHAIN_CONTINUE);
		pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextBySellMBLData();
		nSLevel++;
	}
	m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, NULL, NULL, OF_Topic_Market_SnapShot, 4, UF_INT8_MAX, OUTFOG_CHAIN_LAST);
	return true;
}

//bool CMarketDataService::PublishDepthMarketData(CMarketData *pMarketData, CReadOnlyMilliSecondsType &CurrTime)
//{
//	string SubIndexName = pMarketData->ExchangeID.getValue();
//	SubIndexName += "_";
//	SubIndexName += pMarketData->InstrumentID.getValue();
//	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
//	//这里需要对UserID进行清空，要不然会产生IndexFlow的分支，浪费资源
//	//m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
//	//	sprintf(m_pServiceResponser->GetMetaRef()->Remark, "%d,%d,%d", OF_Topic_Market_Depth_5, OF_Topic_Market_Depth_10, OF_Topic_Broad);
//
//	//CMarketOverViewField field;
//	//memset(&field, 0, sizeof(field));
//	//field.ProductGroup = pMarketData->ProductGroup;
//	//field.InstrumentStatus = pMarketData->InstrumentStatus;
//	//CopyMarketDataLastMatchEntity(&field, pMarketData);
//
//	//if (!g_bDoubleMarketVolume)
//	//{
//	//	field.Volume = field.Volume / 2;
//	//	field.OpenInterest = field.OpenInterest / 2;
//	//	field.Turnover = field.Turnover / 2;
//	//}
//
//	//field.ExchangeID = pMarketData->ExchangeID;
//	//field.InstrumentID = pMarketData->InstrumentID;
//	//field.UpdateMilliSecond = pMarketData->UpdateMilliSecond;
//	//field.UpdateTime = pMarketData->UpdateTime;
//	//field.AskPrice1.clear();
//	//field.BidPrice1.clear();
//
//	CFiveDepthMarketDataField fivefield;
//	memset(&fivefield, 0, sizeof(fivefield));
//	CopyMarketDataUpdateTimeEntity(&fivefield, pMarketData);
//
//	fivefield.BidPrice1.clear();
//	fivefield.BidPrice2.clear();
//	fivefield.BidPrice3.clear();
//	fivefield.BidPrice4.clear();
//	fivefield.BidPrice5.clear();
//	fivefield.AskPrice1.clear();
//	fivefield.AskPrice2.clear();
//	fivefield.AskPrice3.clear();
//	fivefield.AskPrice4.clear();
//	fivefield.AskPrice5.clear();
//
//	//CTenDepthMarketDataField tenfield;
//	//memset(&tenfield, 0, sizeof(tenfield));
//	//CopyMarketDataUpdateTimeEntity(&tenfield, &field);
//	//tenfield.BidPrice1.clear();
//	//tenfield.BidPrice2.clear();
//	//tenfield.BidPrice3.clear();
//	//tenfield.BidPrice4.clear();
//	//tenfield.BidPrice5.clear();
//	//tenfield.BidPrice6.clear();
//	//tenfield.BidPrice7.clear();
//	//tenfield.BidPrice8.clear();
//	//tenfield.BidPrice9.clear();
//	//tenfield.BidPrice10.clear();
//	//tenfield.AskPrice1.clear();
//	//tenfield.AskPrice2.clear();
//	//tenfield.AskPrice3.clear();
//	//tenfield.AskPrice4.clear();
//	//tenfield.AskPrice5.clear();
//	//tenfield.AskPrice6.clear();
//	//tenfield.AskPrice7.clear();
//	//tenfield.AskPrice8.clear();
//	//tenfield.AskPrice9.clear();
//	//tenfield.AskPrice10.clear();
//
//	int nLevel = 1;
//	CMarketOrder *pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
//	while (pMarketOrder != NULL)
//	{
//		if (nLevel > 5)
//			break;
//
//		switch (nLevel)
//		{
//		case 1:
//		{
//			//pMarketData->BidPrice1 = pMarketOrder->Price; pMarketData->BidVolume1 = pMarketOrder->Volume;
//			//field.BidPrice1 = pMarketOrder->Price; field.BidVolume1 = pMarketOrder->Volume;
//			fivefield.BidPrice1 = pMarketOrder->Price; fivefield.BidVolume1 = pMarketOrder->Volume;
//			//			tenfield.BidPrice1 = pMarketOrder->Price; tenfield.BidVolume1 = pMarketOrder->Volume;
//			break;
//		}
//		case 2:
//		{
//			fivefield.BidPrice2 = pMarketOrder->Price; fivefield.BidVolume2 = pMarketOrder->Volume;
//			//			tenfield.BidPrice2 = pMarketOrder->Price;	tenfield.BidVolume2 = pMarketOrder->Volume;
//			break;
//		}
//		case 3:
//		{
//			fivefield.BidPrice3 = pMarketOrder->Price; fivefield.BidVolume3 = pMarketOrder->Volume;
//			//			tenfield.BidPrice3 = pMarketOrder->Price;	tenfield.BidVolume3 = pMarketOrder->Volume;
//			break;
//		}
//		case 4:
//		{
//			fivefield.BidPrice4 = pMarketOrder->Price; fivefield.BidVolume4 = pMarketOrder->Volume;
//			//			tenfield.BidPrice4 = pMarketOrder->Price;	tenfield.BidVolume4 = pMarketOrder->Volume;
//			break;
//		}
//		case 5:
//		{
//			fivefield.BidPrice5 = pMarketOrder->Price; fivefield.BidVolume5 = pMarketOrder->Volume;
//			//			tenfield.BidPrice5 = pMarketOrder->Price;	tenfield.BidVolume5 = pMarketOrder->Volume;
//			break;
//		}
//		//case 6:
//		//{
//		//	tenfield.BidPrice6 = pMarketOrder->Price;	tenfield.BidVolume6 = pMarketOrder->Volume;
//		//	break;
//		//}
//		//case 7:
//		//{
//		//	tenfield.BidPrice7 = pMarketOrder->Price;	tenfield.BidVolume7 = pMarketOrder->Volume;
//		//	break;
//		//}
//		//case 8:
//		//{
//		//	tenfield.BidPrice8 = pMarketOrder->Price;	tenfield.BidVolume8 = pMarketOrder->Volume;
//		//	break;
//		//}
//		//case 9:
//		//{
//		//	tenfield.BidPrice9 = pMarketOrder->Price;	tenfield.BidVolume9 = pMarketOrder->Volume;
//		//	break;
//		//}
//		//case 10:
//		//{
//		//	tenfield.BidPrice10 = pMarketOrder->Price; tenfield.BidVolume9 = pMarketOrder->Volume;
//		//	break;
//		//}
//		}
//
//		pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextByBuyMBLData();
//		nLevel++;
//	}
//	m_pMDB->m_MarketOrderFactory->endFindByBuyMBLData();
//
//	nLevel = 1;
//	pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
//	while (pMarketOrder != NULL)
//	{
//		if (nLevel > 5)
//			break;
//
//		switch (nLevel)
//		{
//		case 1:
//		{
//			//pMarketData->AskPrice1 = pMarketOrder->Price; pMarketData->AskVolume1 = pMarketOrder->Volume;
//			//field.AskPrice1 = pMarketOrder->Price; field.AskVolume1 = pMarketOrder->Volume;
//			fivefield.AskPrice1 = pMarketOrder->Price; fivefield.AskVolume1 = pMarketOrder->Volume;
//			//			tenfield.AskPrice1 = pMarketOrder->Price; tenfield.AskVolume1 = pMarketOrder->Volume;
//			break;
//		}
//		case 2:
//		{
//			fivefield.AskPrice2 = pMarketOrder->Price; fivefield.AskVolume2 = pMarketOrder->Volume;
//			//			tenfield.AskPrice2 = pMarketOrder->Price;	tenfield.AskVolume2 = pMarketOrder->Volume;
//			break;
//		}
//		case 3:
//		{
//			fivefield.AskPrice3 = pMarketOrder->Price; fivefield.AskVolume3 = pMarketOrder->Volume;
//			//			tenfield.AskPrice3 = pMarketOrder->Price;	tenfield.AskVolume3 = pMarketOrder->Volume;
//			break;
//		}
//		case 4:
//		{
//			fivefield.AskPrice4 = pMarketOrder->Price; fivefield.AskVolume4 = pMarketOrder->Volume;
//			//			tenfield.AskPrice4 = pMarketOrder->Price;	tenfield.AskVolume4 = pMarketOrder->Volume;
//			break;
//		}
//		case 5:
//		{
//			fivefield.AskPrice5 = pMarketOrder->Price; fivefield.AskVolume5 = pMarketOrder->Volume;
//			//			tenfield.AskPrice5 = pMarketOrder->Price;	tenfield.AskVolume5 = pMarketOrder->Volume;
//			break;
//		}
//		//case 6:
//		//{
//		//	tenfield.AskPrice6 = pMarketOrder->Price;	tenfield.AskVolume6 = pMarketOrder->Volume;
//		//	break;
//		//}
//		//case 7:
//		//{
//		//	tenfield.AskPrice7 = pMarketOrder->Price;	tenfield.AskVolume7 = pMarketOrder->Volume;
//		//	break;
//		//}
//		//case 8:
//		//{
//		//	tenfield.AskPrice8 = pMarketOrder->Price;	tenfield.AskVolume8 = pMarketOrder->Volume;
//		//	break;
//		//}
//		//case 9:
//		//{
//		//	tenfield.AskPrice9 = pMarketOrder->Price;	tenfield.AskVolume9 = pMarketOrder->Volume;
//		//	break;
//		//}
//		//case 10:
//		//{
//		//	tenfield.AskPrice10 = pMarketOrder->Price; tenfield.AskVolume10 = pMarketOrder->Volume;
//		//	break;
//		//}
//		}
//
//		pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextBySellMBLData();
//		nLevel++;
//	}
//	m_pMDB->m_MarketOrderFactory->endFindBySellMBLData();
//
//	//if (pMarketData->UnderlyingPrice.isNull() && !field.AskPrice1.isNull() && !field.BidPrice1.isNull())
//	//{
//	//	pMarketData->MarkedPrice = (field.AskPrice1.getValue() + field.BidPrice1.getValue()) / 2;
//	//}
//
//	m_pServiceResponser->SendToSubjectID(TID_PushFiveDepthMarketData, &fivefield, &(fivefield.m_Describe), OF_Topic_Market_Depth_5, UF_INT8_MAX, OUTFOG_CHAIN_SINGLE);
//	//	m_pServiceResponser->SendToSubjectID(TID_PushTenDepthMarketData, &tenfield, &(tenfield.m_Describe), OF_Topic_Market_Depth_10, OUTFOG_CHAIN_SINGLE);
//	//	m_pServiceResponser->SendToSubjectID(TID_PushMarketOverView, &field, &(field.m_Describe), OF_Topic_Market_OverView, OUTFOG_CHAIN_SINGLE);
//
//		//if (CurrTime % 3 == 0 && CurrMillisec <= 333)
//		////if (CurrMillisec <= 333)
//		//{
//		//	sprintf(m_pServiceResponser->GetMetaRef()->SubIndexName, "%s", field.ProductGroup.getValue());
//		//	m_pServiceResponser->SendToSubjectID(TID_PushMarketOverView, &field, &(field.m_Describe), OF_Topic_Market_OverView, OUTFOG_CHAIN_SINGLE);
//		//}
//
//	return true;
//}

//bool CMarketDataService::PublishTickUpDepthMarketData(CMarketData *pMarketData, CPriceType Tick)
//{
//	string SubIndexName = pMarketData->ExchangeID.getValue();
//	SubIndexName += "_";
//	SubIndexName += pMarketData->InstrumentID.getValue();
//	SubIndexName += "_";
//	SubIndexName += Tick.getString();
//	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
//	//m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
//	//sprintf(m_pServiceResponser->GetMetaRef()->Remark, "%d,%d,%d", OF_Topic_Market_Depth_5, OF_Topic_Market_Depth_10, OF_Topic_Broad);
//
//	CFiveDepthMarketDataField fivefield;
//	memset(&fivefield, 0, sizeof(fivefield));
//	CopyMarketDataUpdateTimeEntity(&fivefield, pMarketData);
//	fivefield.BidPrice1.clear();
//	fivefield.BidPrice2.clear();
//	fivefield.BidPrice3.clear();
//	fivefield.BidPrice4.clear();
//	fivefield.BidPrice5.clear();
//	fivefield.AskPrice1.clear();
//	fivefield.AskPrice2.clear();
//	fivefield.AskPrice3.clear();
//	fivefield.AskPrice4.clear();
//	fivefield.AskPrice5.clear();
//
//	int nLevel = 1;
//	CTickMarketOrder *pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findFirstByBuyMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID, Tick);
//	while (pMarketOrder != NULL)
//	{
//		if (nLevel > 5)
//			break;
//		if (pMarketOrder->Volume == 0.0)
//		{
//			pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextByBuyMBLData();
//			continue;
//		}
//
//		switch (nLevel)
//		{
//		case 1:
//			fivefield.BidPrice1 = pMarketOrder->Price; fivefield.BidVolume1 = pMarketOrder->Volume;
//			break;
//		case 2:
//			fivefield.BidPrice2 = pMarketOrder->Price; fivefield.BidVolume2 = pMarketOrder->Volume;
//			break;
//		case 3:
//			fivefield.BidPrice3 = pMarketOrder->Price; fivefield.BidVolume3 = pMarketOrder->Volume;
//			break;
//		case 4:
//			fivefield.BidPrice4 = pMarketOrder->Price; fivefield.BidVolume4 = pMarketOrder->Volume;
//			break;
//		case 5:
//			fivefield.BidPrice5 = pMarketOrder->Price; fivefield.BidVolume5 = pMarketOrder->Volume;
//			break;
//		}
//		pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextByBuyMBLData();
//		nLevel++;
//	}
//	m_pMDB->m_TickMarketOrderFactory->endFindByBuyMBLData();
//
//	nLevel = 1;
//	pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findFirstBySellMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID, Tick);
//	while (pMarketOrder != NULL)
//	{
//		if (nLevel > 5)
//			break;
//		if (pMarketOrder->Volume == 0.0)
//		{
//			pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextBySellMBLData();
//			continue;
//		}
//
//		switch (nLevel)
//		{
//		case 1:
//			fivefield.AskPrice1 = pMarketOrder->Price; fivefield.AskVolume1 = pMarketOrder->Volume;
//			break;
//		case 2:
//			fivefield.AskPrice2 = pMarketOrder->Price; fivefield.AskVolume2 = pMarketOrder->Volume;
//			break;
//		case 3:
//			fivefield.AskPrice3 = pMarketOrder->Price; fivefield.AskVolume3 = pMarketOrder->Volume;
//			break;
//		case 4:
//			fivefield.AskPrice4 = pMarketOrder->Price; fivefield.AskVolume4 = pMarketOrder->Volume;
//			break;
//		case 5:
//			fivefield.AskPrice5 = pMarketOrder->Price; fivefield.AskVolume5 = pMarketOrder->Volume;
//			break;
//		}
//		pMarketOrder = m_pMDB->m_TickMarketOrderFactory->findNextBySellMBLData();
//		nLevel++;
//	}
//	m_pMDB->m_TickMarketOrderFactory->endFindBySellMBLData();
//	m_pServiceResponser->SendToSubjectID(TID_PushFiveDepthMarketData, &fivefield, &(fivefield.m_Describe), OF_Topic_Market_Depth_5_TickUp, UF_INT8_MAX, OUTFOG_CHAIN_SINGLE);
//	return true;
//}

void CMarketDataService::SetMarketOrder(const char *IndexName, CMarketOrderField *pField)
{
	if (pField->Volume != 0)
		m_nMaxMarketDataNo = pField->BusinessNo.getValue();
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
				iter1->second.Orders = pField->Orders;
				iter1->second.TrueVolume = pField->TrueVolume;
				iter1->second.BusinessNo = pField->BusinessNo;
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
				iter1->second.Orders = pField->Orders;
				iter1->second.TrueVolume = pField->TrueVolume;
				iter1->second.BusinessNo = pField->BusinessNo;
			}
		}
	}
}

void CMarketDataService::SetDelayMarketOrder(const char *IndexName, CMarketOrderField *pField)
{
	if (pField->Direction == D_Buy)
	{
		CInstrumentMarketOrderMap::iterator iter = m_BuyDelayMarketOrderMap.find(IndexName);
		if (iter == m_BuyDelayMarketOrderMap.end())
		{
			CMarketOrderMap MarketOrderMap;
			CMarketOrderField field;
			memcpy(&field, pField, sizeof(field));
			MarketOrderMap.insert(CMarketOrderMap::value_type(field.Price.getValue(), field));
			m_BuyDelayMarketOrderMap.insert(CInstrumentMarketOrderMap::value_type(IndexName, MarketOrderMap));
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
				iter1->second.Orders = pField->Orders;
				iter1->second.TrueVolume = pField->TrueVolume;
				iter1->second.BusinessNo = pField->BusinessNo;
			}
		}
	}
	else
	{
		CInstrumentMarketOrderMap::iterator iter = m_SellDelayMarketOrderMap.find(IndexName);
		if (iter == m_SellDelayMarketOrderMap.end())
		{
			CMarketOrderMap MarketOrderMap;
			CMarketOrderField field;
			memcpy(&field, pField, sizeof(field));
			MarketOrderMap.insert(CMarketOrderMap::value_type(field.Price.getValue(), field));
			m_SellDelayMarketOrderMap.insert(CInstrumentMarketOrderMap::value_type(IndexName, MarketOrderMap));
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
				iter1->second.Orders = pField->Orders;
				iter1->second.TrueVolume = pField->TrueVolume;
				iter1->second.BusinessNo = pField->BusinessNo;
			}
		}
	}
}

void CMarketDataService::SetTickMarketOrder(const char* IndexName, CTickMarketOrderField* pField)
{
	if (pField->Direction == D_Buy)
	{
		CInstrumentTickMarketOrderMap::iterator iter = m_BuyTickMarketOrderMap.find(IndexName);
		if (iter == m_BuyTickMarketOrderMap.end())
		{
			CTickMarketOrderMap TickMarketOrderMap;
			CTickMarketOrderField field;
			memcpy(&field, pField, sizeof(field));
			TickMarketOrderMap.insert(CTickMarketOrderMap::value_type(field.Price.getValue(), field));
			m_BuyTickMarketOrderMap.insert(CInstrumentTickMarketOrderMap::value_type(IndexName, TickMarketOrderMap));
		}
		else
		{
			CTickMarketOrderMap::iterator iter1 = iter->second.find(pField->Price.getValue());
			if (iter1 == iter->second.end())
			{
				CTickMarketOrderField field;
				memcpy(&field, pField, sizeof(field));
				iter->second.insert(CTickMarketOrderMap::value_type(field.Price.getValue(), field));
			}
			else
			{
				iter1->second.Volume = pField->Volume;
				iter1->second.Orders = pField->Orders;
				iter1->second.TrueVolume = pField->TrueVolume;
				iter1->second.BusinessNo = pField->BusinessNo;
				iter1->second.Tick = pField->Tick;
			}
		}
	}
	else
	{
		CInstrumentTickMarketOrderMap::iterator iter = m_SellTickMarketOrderMap.find(IndexName);
		if (iter == m_SellTickMarketOrderMap.end())
		{
			CTickMarketOrderMap TickMarketOrderMap;
			CTickMarketOrderField field;
			memcpy(&field, pField, sizeof(field));
			TickMarketOrderMap.insert(CTickMarketOrderMap::value_type(field.Price.getValue(), field));
			m_SellTickMarketOrderMap.insert(CInstrumentTickMarketOrderMap::value_type(IndexName, TickMarketOrderMap));
		}
		else
		{
			CTickMarketOrderMap::iterator iter1 = iter->second.find(pField->Price.getValue());
			if (iter1 == iter->second.end())
			{
				CTickMarketOrderField field;
				memcpy(&field, pField, sizeof(field));
				iter->second.insert(CTickMarketOrderMap::value_type(field.Price.getValue(), field));
			}
			else
			{
				iter1->second.Volume = pField->Volume;
				iter1->second.Orders = pField->Orders;
				iter1->second.TrueVolume = pField->TrueVolume;
				iter1->second.BusinessNo = pField->BusinessNo;
				iter1->second.Tick = pField->Tick;
			}
		}
	}
}

bool CMarketDataService::Publish25MarketOrder(CWriteableMarketData *pMarketData, CCurrentTimeField* pField)
{
	string SubIndexName = pMarketData->ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += pMarketData->InstrumentID.getValue();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
	//m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
	m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, NULL, NULL, OF_Topic_Market_Depth_25, 4, g_nBusinessNo, OUTFOG_CHAIN_FIRST);
	CInstrumentMarketOrderMap::iterator iter = m_SellMarketOrderMap.find(SubIndexName.c_str());
	if (iter != m_SellMarketOrderMap.end())
	{
		CMarketOrderMap::iterator iter1 = iter->second.begin();
		while (iter1 != iter->second.end())
		{
			if (iter1->second.Orders == 0)
			{
				m_pServiceResponser->SendNext(&(iter1->second), &(iter1->second.m_Describe), OUTFOG_CHAIN_CONTINUE);
			}
			SetDelayMarketOrder(SubIndexName.c_str(), &(iter1->second));
			iter1++;
		}
		iter->second.clear();
	}

	CMarketOrder* pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstByBuyMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
	int nLevel = 0;
	//int i = 0;
	CPriceType L25LowerPrice;
	L25LowerPrice.clear();
	while (pMarketOrder != NULL)
	{
		if (nLevel >= 25)
			break;

		if (pMarketOrder->BusinessNo > m_nLastMarketDataNo)
		{
			CMarketOrderField field;
			field.Price = pMarketOrder->Price;
			field.ExchangeID = pMarketOrder->ExchangeID;
			field.InstrumentID = pMarketOrder->InstrumentID;
			//			field.ChangeType = DAT_Update;
			field.Direction = pMarketOrder->Direction;
			field.Volume = pMarketOrder->Volume;
			field.Orders = pMarketOrder->Orders;
			field.TrueVolume = pMarketOrder->TrueVolume;
			field.BusinessNo = pMarketOrder->BusinessNo;
			//if (field.InstrumentID == "ETHUSDT")
			//	PRINT_TO_STD("25SendMarketOrder:[%s_%s],[%c|%0.4f/%0.5f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.Direction.getValue(), field.Price.getValue(), field.Volume.getValue());
			m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
			//i++;
		}
		else
		{
			if (!pMarketData->L25LowerPrice.isNull() && pMarketOrder->Price < pMarketData->L25LowerPrice)
			{
				CMarketOrderField field;
				field.Price = pMarketOrder->Price;
				field.ExchangeID = pMarketOrder->ExchangeID;
				field.InstrumentID = pMarketOrder->InstrumentID;
				//				field.ChangeType = DAT_Insert;
				field.Direction = pMarketOrder->Direction;
				field.Volume = pMarketOrder->Volume;
				field.Orders = pMarketOrder->Orders;
				field.TrueVolume = pMarketOrder->TrueVolume;
				field.BusinessNo = pMarketOrder->BusinessNo;
				//if (field.InstrumentID == "ETHUSDT")
				//	PRINT_TO_STD("25LowerPriceSendMarketOrder:[%s_%s],[%c|%0.4f/%0.5f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.Direction.getValue(), field.Price.getValue(), field.Volume.getValue());
				m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
				///i++;
			}
		}
		//每次都需要赋值，确保不满足条件退出
		L25LowerPrice = pMarketOrder->Price;
		pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextByBuyMBLData();
		nLevel++;
	}
	if (!L25LowerPrice.isNull())
		pMarketData->L25LowerPrice = L25LowerPrice;

	//删除的订单需要考虑
	iter = m_BuyMarketOrderMap.find(SubIndexName.c_str());
	if (iter != m_BuyMarketOrderMap.end())
	{
		CMarketOrderMap::iterator iter1 = iter->second.begin();
		while (iter1 != iter->second.end())
		{
			if (iter1->second.Orders == 0)
			{
				m_pServiceResponser->SendNext(&(iter1->second), &(iter1->second.m_Describe), OUTFOG_CHAIN_CONTINUE);
				//i++;
			}
			SetDelayMarketOrder(SubIndexName.c_str(), &(iter1->second));
			iter1++;
		}
		iter->second.clear();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	pMarketOrder = m_pMDB->m_MarketOrderFactory->findFirstBySellMBLData(pMarketData->ExchangeID, pMarketData->InstrumentID);
	nLevel = 0;
	CPriceType L25UpperPrice;
	L25UpperPrice.clear();
	//pMarketData->L25UpperPrice.clear();
	while (pMarketOrder != NULL)
	{
		if (nLevel >= 25)
			break;

		if (pMarketOrder->BusinessNo > m_nLastMarketDataNo)
		{
			CMarketOrderField field;
			field.Price = pMarketOrder->Price;
			field.ExchangeID = pMarketOrder->ExchangeID;
			field.InstrumentID = pMarketOrder->InstrumentID;
			//			field.ChangeType = DAT_Update;
			field.Direction = pMarketOrder->Direction;
			field.Volume = pMarketOrder->Volume;
			field.Orders = pMarketOrder->Orders;
			field.TrueVolume = pMarketOrder->TrueVolume;
			field.BusinessNo = pMarketOrder->BusinessNo;
			//if (field.InstrumentID == "ETHUSDT")
			//	PRINT_TO_STD("25SendMarketOrder:[%s_%s],[%c|%0.4f/%0.5f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.Direction.getValue(), field.Price.getValue(), field.Volume.getValue());
			m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
			//i++;
		}
		else
		{
			if (!pMarketData->L25UpperPrice.isNull() && pMarketOrder->Price > pMarketData->L25UpperPrice)
			{
				CMarketOrderField field;
				field.Price = pMarketOrder->Price;
				field.ExchangeID = pMarketOrder->ExchangeID;
				field.InstrumentID = pMarketOrder->InstrumentID;
				//				field.ChangeType = DAT_Insert;
				field.Direction = pMarketOrder->Direction;
				field.Volume = pMarketOrder->Volume;
				field.Orders = pMarketOrder->Orders;
				field.TrueVolume = pMarketOrder->TrueVolume;
				field.BusinessNo = pMarketOrder->BusinessNo;
				//if (field.InstrumentID == "ETHUSDT")
				//	PRINT_TO_STD("25DeleteSendMarketOrder:[%s_%s],[%c|%0.4f/%0.5f]", field.ExchangeID.getValue(), field.InstrumentID.getValue(), field.Direction.getValue(), field.Price.getValue(), field.Volume.getValue());
				m_pServiceResponser->SendNext(&(field), &(field.m_Describe), OUTFOG_CHAIN_CONTINUE);
				//i++;
			}
		}
		L25UpperPrice = pMarketOrder->Price;
		pMarketOrder = m_pMDB->m_MarketOrderFactory->findNextBySellMBLData();
		nLevel++;
	}
	if (!L25UpperPrice.isNull())
		pMarketData->L25UpperPrice = L25UpperPrice;

	//if (i > 0)
	//	m_pServiceResponser->SendNoNext(pField, &(pField->m_Describe), OUTFOG_CHAIN_CONTINUE);
	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	return true;
}

bool CMarketDataService::Publish25TickMarketOrder(CWriteableMarketData* pMarketData, CPriceType Tick, const char* index, CCurrentTimeField* pField)
{
	//string SubIndexName = pMarketData->ExchangeID.getValue();
	//SubIndexName += "_";
	//SubIndexName += pMarketData->InstrumentID.getValue();
	//SubIndexName += "_";
	//SubIndexName += Tick.getString();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, index);
	//m_pServiceResponser->GetMetaRef()->UserID[0] = '\0';
	m_pServiceResponser->SendToSubjectID(TID_PushMarketOrder, NULL, NULL, OF_Topic_Market_Depth_25, 4, UF_INT8_MAX, OUTFOG_CHAIN_FIRST);
	int i = 0;

	CInstrumentTickMarketOrderMap::iterator iter = m_BuyTickMarketOrderMap.find(index);
	if (iter != m_BuyTickMarketOrderMap.end())
	{
		CTickMarketOrderMap::iterator iter1 = iter->second.begin();
		while (iter1 != iter->second.end())
		{
			m_pServiceResponser->SendNext(&(iter1->second), &(iter1->second.m_Describe), OUTFOG_CHAIN_CONTINUE);
			i++;
			iter1++;
		}
		iter->second.clear();
	}

	iter = m_SellTickMarketOrderMap.find(index);
	if (iter != m_SellTickMarketOrderMap.end())
	{
		CTickMarketOrderMap::iterator iter1 = iter->second.begin();
		while (iter1 != iter->second.end())
		{
			m_pServiceResponser->SendNext(&(iter1->second), &(iter1->second.m_Describe), OUTFOG_CHAIN_CONTINUE);
			i++;
			iter1++;
		}
		iter->second.clear();
	}
	//if (i > 0)
	//	m_pServiceResponser->SendNoNext(pField, &(pField->m_Describe), OUTFOG_CHAIN_CONTINUE);
	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	return true;
}

bool CMarketDataService::PublishDelayMarketOrder(CMarketData *pMarketData, CCurrentTimeField* pField)
{
	string SubIndexName = pMarketData->ExchangeID.getValue();
	SubIndexName += "_";
	SubIndexName += pMarketData->InstrumentID.getValue();
	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
	int i = 0;
	m_pServiceResponser->SendToSubjectID(TID_PushDelayMarketOrder, NULL, NULL, OF_Topic_Market_Depth_Delay, 4, UF_INT8_MAX, OUTFOG_CHAIN_FIRST);
	CInstrumentMarketOrderMap::iterator iter = m_BuyDelayMarketOrderMap.find(SubIndexName.c_str());
	if (iter != m_BuyDelayMarketOrderMap.end())
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
	iter = m_SellDelayMarketOrderMap.find(SubIndexName.c_str());
	if (iter != m_SellDelayMarketOrderMap.end())
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
	//if (i > 0)
	//	m_pServiceResponser->SendNoNext(pField, &(pField->m_Describe), OUTFOG_CHAIN_CONTINUE);
	m_pServiceResponser->SendNext(NULL, NULL, OUTFOG_CHAIN_LAST);
	return true;
}

//bool CMarketDataService::PublishLastKLine(CWriteableMarketData *pMarketData)
//{
//	if (pMarketData->Volume == pMarketData->LastVolume)
//		return false;
//	string SubIndexName = pMarketData->ExchangeID.getValue();
//	SubIndexName += "_";
//	SubIndexName += pMarketData->InstrumentID.getValue();
//	strcpy(m_pServiceResponser->GetMetaRef()->SubIndexName, SubIndexName.c_str());
//	CLastKLineField field;
//#if 0
//	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriod(pMarketData->ExchangeID, pMarketData->InstrumentID, "1m");
//	m_pMDB->m_LastKLineFactory->endFindByHisPeriod();
//#else
//	CLastKLine *pLastKLine = m_pMDB->m_LastKLineFactory->findFirstByHisPeriodBeginTime(pMarketData->ExchangeID, pMarketData->InstrumentID, "1m");
//	m_pMDB->m_LastKLineFactory->endFindByHisPeriodBeginTime();
//#endif
//	if (pLastKLine != NULL)
//	{
//		CopyLastKLineEntity(&field, pLastKLine);
//		m_pServiceResponser->SendToSubjectID(TID_PushKLine, &field, &(field.m_Describe), OF_Topic_Market_LastKLine, OUTFOG_CHAIN_SINGLE);
//	}
//	return true;
//}