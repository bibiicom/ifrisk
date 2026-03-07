#include "SnapCheckEngine.h"
#include "CSVToField.h"
#include "CFilelogService.h"
#include "BaseFunction.h"
#include "printinfo.h"
#include "utils.h"

CSnapCheckEngine::CSnapCheckEngine(const char* errorFileName) :CEngine()
{
	m_OutPut = fopen(errorFileName, "wb");
	if (m_OutPut == NULL)
	{
		printf("open errorFileName error");
		return;
	}

	CMonitorIndex::init(CFilelogService::CreateInstance(GetConfigString(INI_FILE_NAME, "SyslogPath").c_str()), GetConfigString(INI_FILE_NAME, "LogLevel").c_str());

	//初始化内存修改服务	
	m_pAlterService = new CAlterService(m_pMDB, m_pErrorEngine);
	m_pAlterServiceInterface = new CAlterServiceInterface(m_pAlterService, NULL);
	addServiceInterface(m_pAlterServiceInterface);

	m_SessionRef.frontID = 0;
	m_SessionRef.sessionID = 0;
	m_SessionRef.requestID = 0;
	m_SessionRef.pToken = "m";
}

CSnapCheckEngine::~CSnapCheckEngine(void)
{
}

const char* CSnapCheckEngine::getVersion()
{
	static char version[256];
	sprintf(version, "Service version: %s", "snapcheck");
	return version;
}

void CSnapCheckEngine::handleMessage(const TOutFogMetaRef* pOutFogReqRef, const void* pStream)
{
	iterator it;
	for (it = begin(); it < end(); it++)
	{
		if (!(*it)->handleMessage(pOutFogReqRef, pStream, &m_SessionRef))
		{
			//给出问题
			/*char msgno[256];
			sprintf(msgno, "MessageNo:%d,FieldNo:%d", pOutFogReqRef->MessageNo,pOutFogReqRef->FieldNo);
			REPORT_EVENT(LOG_ERROR, "handleMessage", msgno);*/
		}
	}
}

bool CSnapCheckEngine::SendTable(CFieldDescribe* pFieldDesc, const void* pRspBuffer)
{
	TOutFogMetaRef OutFogMetaRef;
	memset(&OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_KERNEL;
	OutFogMetaRef.FrontNo = 0;
	OutFogMetaRef.MessageNo = TID_SendInit;
	OutFogMetaRef.Type = OUTFOG_TYPE_INSERT;
	OutFogMetaRef.FieldNo = pFieldDesc->m_FieldID;
	OutFogMetaRef.Length = pFieldDesc->m_nStreamSize;
	pFieldDesc->setSingleField(m_pStreamBuf, pRspBuffer);
	handleMessage(&OutFogMetaRef, m_pStreamBuf);
	return true;
}

void CSnapCheckEngine::OutPutClose()
{
	if (m_OutPut != NULL)
		fclose(m_OutPut);
}

bool CSnapCheckEngine::LoadFromCSV(const char* pszCSVPath)
{
	char sTimeBuffer[36];
	printf("LoadFromCSV:%s\n", getLocalTime(sTimeBuffer));

	m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_KERNEL;
	m_OutFogMetaRef.FrontNo = 0;

	char szFileName[1000];
	TTinitTableInfo* pInfo = NULL;
	//ToDo:dump.-1完善之后再做

	pInfo = g_InitTableInfo;

	CCurrentTimeField field;
	m_OutFogMetaRef.MessageNo = TID_DataSyncStart;
	m_OutFogMetaRef.FieldNo = CCurrentTimeField::m_Describe.m_FieldID;
	m_OutFogMetaRef.Length = CCurrentTimeField::m_Describe.m_nStreamSize;
	CCurrentTimeField::m_Describe.setSingleField(m_pStreamBuf, &field);

	//ReqField(&m_OutFogMetaRef, m_pStreamBuf);
	handleMessage(&m_OutFogMetaRef, m_pStreamBuf);

	while (pInfo->pFieldDescribe != NULL)
	{
		//sprintf(szFileName, "%s%c%s.csv", pszCSVPath, PATH_SPLIT, pInfo->pszTable);
		sprintf(szFileName, "%s%c%s.csv", pszCSVPath, PATH_SPLIT, pInfo->pszTable);
		LoadTable(szFileName, pInfo->pFieldDescribe, this);
		pInfo++;
	}

	TOutFogMetaRef OutFogMetaRef;
	memset(&OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	OutFogMetaRef.MessageNo = TID_DataSyncEnd;
	OutFogMetaRef.FieldNo = CCurrentTimeField::m_Describe.m_FieldID;
	OutFogMetaRef.Length = CCurrentTimeField::m_Describe.m_nStreamSize;
	CCurrentTimeField::m_Describe.setSingleField(m_pStreamBuf, &field);
	handleMessage(&OutFogMetaRef, m_pStreamBuf);

	//REPORT_EVENT(LOG_CRITICAL, "Init", "Load Init Data OK");
	return true;
}

//user校验 ok
bool CSnapCheckEngine::checkMember()
{
	map<string, int> triggerOrdersMap;
	map<string, int> openOrdersMap;
	map<string, int> openPositionsMap;

	CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getFirst();
	while (pTriggerOrder != NULL)
	{
		map<string, int>::iterator iter = triggerOrdersMap.find(pTriggerOrder->MemberID.getValue());
		if (iter == triggerOrdersMap.end())
			triggerOrdersMap.insert(map<string, int>::value_type(pTriggerOrder->MemberID.getValue(), 1));
		else
			iter->second += 1;
		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getNext();
	}

	COrder* pOrder = m_pMDB->m_OrderFactory->getFirst();
	while (pOrder != NULL)
	{
		map<string, int>::iterator iter = openOrdersMap.find(pOrder->MemberID.getValue());
		if (iter == openOrdersMap.end())
			openOrdersMap.insert(map<string, int>::value_type(pOrder->MemberID.getValue(), 1));
		else
			iter->second += 1;
		pOrder = m_pMDB->m_OrderFactory->getNext();
	}

	CUserSession* pUserSession = m_pMDB->m_UserSessionFactory->getFirst();
	while (pUserSession != NULL)
	{
		CMember* pMember = m_pMDB->m_MemberFactory->findByMemberID(pUserSession->MemberID);
		if (pMember == NULL)
		{
			fprintf(m_OutPut, "UserSession[%s]:[%s] not found in Member!\n", pUserSession->Token.getValue(), pUserSession->MemberID.getString().c_str());
			fflush(m_OutPut);
		}
		pUserSession = m_pMDB->m_UserSessionFactory->getNext();
	}

	CPosition* pPosition = m_pMDB->m_PositionFactory->getFirst();
	while (pPosition != NULL)
	{
		if (pPosition->getInstrument()->ProductClass == PC_Premium || pPosition->getInstrument()->ProductClass == PC_Margin)
		{
			if (pPosition->Position != 0)
			{
				map<string, int>::iterator iter = openPositionsMap.find(pPosition->MemberID.getValue());
				if (iter == openPositionsMap.end())
					openPositionsMap.insert(map<string, int>::value_type(pPosition->MemberID.getValue(), 1));
				else
					iter->second += 1;
			}
		}
		pPosition = m_pMDB->m_PositionFactory->getNext();
	}

	CMember* pMember = m_pMDB->m_MemberFactory->getFirst();
	while (pMember != NULL)
	{
		if (triggerOrdersMap.find(pMember->MemberID.getValue()) != triggerOrdersMap.end())
		{
			int nSum = triggerOrdersMap[pMember->MemberID.getValue()];
			if (pMember->TriggerOrders != nSum)
			{
				fprintf(m_OutPut, "MemberID[%s]:TriggerOrders[%d]!=[%d]\n", pMember->MemberID.getValue(), pMember->TriggerOrders.getValue(), nSum);
				fflush(m_OutPut);
			}
		}
		else
		{
			if (pMember->TriggerOrders != 0)
			{
				fprintf(m_OutPut, "MemberID[%s]:TriggerOrders[%d]!=0\n", pMember->MemberID.getValue(), pMember->TriggerOrders.getValue());
				fflush(m_OutPut);
			}
		}

		if (openOrdersMap.find(pMember->MemberID.getValue()) != openOrdersMap.end())
		{
			int nSum = openOrdersMap[pMember->MemberID.getValue()];
			if (pMember->OpenOrders != nSum)
			{
				fprintf(m_OutPut, "MemberID[%s]:OpenOrders[%d]!=[%d]\n", pMember->MemberID.getValue(), pMember->OpenOrders.getValue(), nSum);
				fflush(m_OutPut);
			}
		}
		else
		{
			if (pMember->OpenOrders != 0)
			{
				fprintf(m_OutPut, "MemberID[%s]:OpenOrders[%d]!=0\n", pMember->MemberID.getValue(), pMember->OpenOrders.getValue());
				fflush(m_OutPut);
			}
		}

		if (openPositionsMap.find(pMember->MemberID.getValue()) != openPositionsMap.end())
		{
			int nSum = openPositionsMap[pMember->MemberID.getValue()];
			if (pMember->OpenPositions != nSum)
			{
				fprintf(m_OutPut, "MemberID[%s]:OpenPositions[%d]!=[%d]\n", pMember->MemberID.getValue(), pMember->OpenPositions.getValue(), nSum);
				fflush(m_OutPut);
			}
		}
		else
		{
			if (pMember->OpenPositions != 0)
			{
				fprintf(m_OutPut, "MemberID[%s]:OpenPositions[%d]!=0\n", pMember->MemberID.getValue(), pMember->OpenPositions.getValue());
				fflush(m_OutPut);
			}
		}
		pMember = m_pMDB->m_MemberFactory->getNext();
	}
	return true;
}

bool CSnapCheckEngine::checkMemberPostion()
{
	CMemberPosition* mp = m_pMDB->m_MemberPositionFactory->getFirst();
	while (mp != NULL)
	{
		CVolumeType pvolume = 0;
		CVolumeType longFrozen = 0;
		CVolumeType shortFrozen = 0;
		CPosition* p = m_pMDB->m_PositionFactory->findFirstByAccountInstrument(mp->AccountID,mp->ExchangeID, mp->InstrumentID);
		//CPosition *p = m_pMDB->m_PositionFactory->getFirst();
		while (p != NULL)
		{
			//判断账户与方向
			//if (p->MemberID==mp->MemberID&&p->InstrumentID==mp->InstrumentID&&p->ExchangeID==mp->ExchangeID && p->AccountID == mp->AccountID && p->PosiDirection == mp->PosiDirection )

			pvolume += p->Position.getValueAbs();
			longFrozen = longFrozen + p->LongFrozen;
			shortFrozen = shortFrozen + p->ShortFrozen;
			p = m_pMDB->m_PositionFactory->findNextByAccountInstrument();
			//p = m_pMDB->m_PositionFactory->getNext();
		}

		/*if (mp->AccountID == "444683")
		{
			printf("%s", mp->AccountID.getValue());
		}*/
		if (mp->Position != pvolume)
		{
			fprintf(m_OutPut, "MemberPostion Postion, Position:%s,caluPosition:%s,Diff:%s\n",
				mp->Position.getAllString().c_str(), pvolume.getAllString().c_str(), mp->Position.getDiffString(pvolume).c_str());
			mp->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		if (mp->LongFrozen != longFrozen)
		{
			fprintf(m_OutPut, "MemberPostion LongFrozen, LongFrozen:%s,caluLongFrozen:%s,Diff:%s\n",
				mp->LongFrozen.getAllString().c_str(), longFrozen.getAllString().c_str(), mp->LongFrozen.getDiffString(longFrozen).c_str());
			mp->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		if (mp->ShortFrozen != shortFrozen)
		{
			fprintf(m_OutPut, "MemberPostion ShortFrozen, ShortFrozen:%s,caluShortFrozen:%s,Diff:%s\n",
				mp->ShortFrozen.getAllString().c_str(), shortFrozen.getAllString().c_str(), mp->ShortFrozen.getDiffString(shortFrozen).c_str());
			mp->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}
		mp = m_pMDB->m_MemberPositionFactory->getNext();
	}
	return true;
}

//多空比例一致
bool CSnapCheckEngine::checkLongEqualShort()
{
	CPosition* p = m_pMDB->m_PositionFactory->getFirst();
	map<CInstrumentIDType, map<CPosiDirectionType, CVolumeType> > m1;

	while (p != NULL)
	{
		if (p->getInstrument()->ProductClass != PC_Margin || p->Position.getValue() == 0)
		{
			p = m_pMDB->m_PositionFactory->getNext();
			continue;
		}
		map<CInstrumentIDType, map<CPosiDirectionType, CVolumeType> >::iterator it = m1.find(p->InstrumentID);
		//if (m1.count(p->InstrumentID) == 0)
		if (it == m1.end())
		{
			//不存在交易对
			map<CPosiDirectionType, CVolumeType> m2;
			m2[p->PosiDirection] = p->Position;
			m1[p->InstrumentID] = m2;
		}
		else
		{
			//map<CPosiDirectionType, CVolumeType> m2 = m1[p->InstrumentID];
			map<CPosiDirectionType, CVolumeType> m2 = it->second;
			map<CPosiDirectionType, CVolumeType>::iterator it1 = m2.find(p->PosiDirection);
			//if (m2.count(p->PosiDirection) == 0)
			if (it1 == m2.end())
			{
				//不存在该方向
				m2[p->PosiDirection] = p->Position;
			}
			else
			{
				m2[p->PosiDirection] = m2[p->PosiDirection] + p->Position;
			}
			m1[p->InstrumentID] = m2;
		}

		p = m_pMDB->m_PositionFactory->getNext();
	}
	//m_pMDB->m_AccountFactory->endGet();

	map<CInstrumentIDType, map<CPosiDirectionType, CVolumeType> >::iterator it;
	for (it = m1.begin(); it != m1.end(); ++it)
	{
		CVolumeType longall = 0;
		CVolumeType shortall = 0;
		CVolumeType netall = 0;
		if (it->second.count(PD_Long) == 0)
			longall = 0;
		else
			longall = it->second[PD_Long];

		if (it->second.count(PD_Short) == 0)
			shortall = 0;
		else
			shortall = it->second[PD_Short];

		if (it->second.count(PD_Net) == 0)
		{
			netall = 0;
		}
		else
		{
			if (it->second[PD_Net] < 0)
			{
				shortall -= it->second[PD_Net];
			}
			else
			{
				longall += it->second[PD_Net];
			}
		}
		if (longall != shortall)
		{
			fprintf(m_OutPut, "longall != shortall InstrumentID:%s,longall:%s,shortall:%s,Diff:%s\n",
				it->first.getValue(), longall.getAllString().c_str(), shortall.getAllString().c_str(), longall.getDiffString(shortall).c_str());
			fflush(m_OutPut);
		}
		else
		{
			//CMarketData *pMarketData = m_pMDB->m_MarketDataFactory->findByInstrumentID(,it->first.getValue());
			//if (pMarketData != NULL)
			//{
			//	if (pMarketData->OpenInterest != longall)
			//	{
			//		fprintf(m_OutPut, "longall != OpenInterest InstrumentID:%s,longall:%s,OpenInterest:%s,Diff:%s\n",
			//			it->first.getValue(), longall.getAllString().c_str(), pMarketData->OpenInterest.getAllString().c_str(), longall.getDiffString(pMarketData->OpenInterest.getValue()).c_str());
			//		fflush(m_OutPut);
			//	}
			//}
			printf("    InstrumentID:%s: longall[%s] == shortall[%s]\n", it->first.getValue(), longall.getAllString().c_str(), shortall.getAllString().c_str());
		}
	}
	return true;
}

//it's ok
bool CSnapCheckEngine::checkPostion()
{
	CPosition* p = m_pMDB->m_PositionFactory->getFirst();
	while (p != NULL)
	{
		if (p->HighestPosition < p->Position)
		{
			fprintf(m_OutPut, "checkPostion HighestPosition, HighestPosition[%s]<Position[%s]\n", p->HighestPosition.getString().c_str(), p->Position.getString().c_str());
			p->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		if (p->getInstrument()->ProductClass == PC_AccountExchange)
		{
			if (p->PosiDirection != D_Buy)
			{
				fprintf(m_OutPut, "checkPostion Dircection, posDirection:%c is wrong\n", p->PosiDirection.getValue());
				p->dumpInLine(m_OutPut);
				fflush(m_OutPut);
				p = m_pMDB->m_PositionFactory->getNext();
				continue;
			}
			if (p->Position < 0)
			{
				fprintf(m_OutPut, "checkPostion Position, [%s] litter than 0\n", p->Position.getAllString().c_str());
				p->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
			CVolumeType longFrozen = 0;
			CVolumeType shortFrozen = 0;
			COrder* pOrder = m_pMDB->m_OrderFactory->findFirstByPositionID(p->PositionID);
			while (pOrder != NULL)
			{
				if (pOrder->Direction == D_Sell)
					shortFrozen += pOrder->VolumeRemain;
				else
					longFrozen += pOrder->VolumeRemain;
				pOrder = m_pMDB->m_OrderFactory->findNextByPositionID();
			}

			if (!(p->LongFrozen.equal(longFrozen, 128)))
			{
				fprintf(m_OutPut, "checkPostion LongFrozen, LongFrozen:%s,caulLongFrozen:%s,Diff:%s\n",
					p->LongFrozen.getAllString().c_str(), longFrozen.getAllString().c_str(), p->LongFrozen.getDiffString(longFrozen).c_str());
				p->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
			if (!(p->ShortFrozen.equal(shortFrozen, 128)))
			{
				fprintf(m_OutPut, "checkPostion ShortFrozen, ShortFrozen:%s,caulShortFrozen:%s,Diff:%s\n",
					p->ShortFrozen.getAllString().c_str(), shortFrozen.getAllString().c_str(), p->ShortFrozen.getDiffString(shortFrozen).c_str());
				p->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
		}
		else {
			//这里需要检验是否有MemberPosition
			if (p->Position > 0.0 || p->ShortFrozen > 0.0 || p->LongFrozen > 0.0)
			{
				CMemberPosition* pMemberPosition = m_pMDB->m_MemberPositionFactory->findByInstrument(p->AccountID, p->ExchangeID, p->InstrumentID);
				if (pMemberPosition == NULL)
				{
					fprintf(m_OutPut, "checkPostion No MemberPosition AccountID:%s,ExchangeID:%s,InstrumentID:%s\n",
						p->AccountID.getValue(), p->ExchangeID.getValue(), p->InstrumentID.getValue());
					p->dumpInLine(m_OutPut);
					fflush(m_OutPut);
				}
			}

			if (p->Leverage.getValue() <= 0.0)
			{
				fprintf(m_OutPut, "checkPostion Leverage litter than 0 Postion MemberID:%s,AccountID:%s,ExchangeID:%s,InstrumentID:%s,Leverage:%f\n",
					p->MemberID.getValue(), p->AccountID.getValue(), p->ExchangeID.getValue(), p->InstrumentID.getValue(), p->Leverage.getValue());
				p->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}

			switch (p->PosiDirection)
			{
			case PD_Long:
			{
				if (p->Position < 0.0)
				{
					fprintf(m_OutPut, "checkPostion Position litter than 0 Postion Position:%s\n", p->Position.getAllString().c_str());
					p->dumpInLine(m_OutPut);
					fflush(m_OutPut);
				}
				if (p->Position < p->ShortFrozen)
				{
					fprintf(m_OutPut, "checkPostion Position position[%s] < shortfrozen[%s]\n", p->Position.getString().c_str(), p->ShortFrozen.getString().c_str());
					p->dumpInLine(m_OutPut);
					fflush(m_OutPut);
				}
				if (p->Position != 0 || p->ShortFrozen != 0 || p->LongFrozen != 0)
				{
					CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByAccountID(p->AccountID);
					while (pPosition != NULL)
					{
						if (pPosition->getInstrument()->ProductClass == PC_Margin || pPosition->getInstrument()->ProductClass == PC_Premium)
						{
							if (pPosition->PosiDirection == PD_Net && (pPosition->Position != 0 || pPosition->ShortFrozen != 0 || pPosition->LongFrozen != 0))
							{
								fprintf(m_OutPut, "checkPostion Gross Position[%s] has Net Position[%s] \n", p->PositionID.getValue(), pPosition->PositionID.getValue());
								p->dumpInLine(m_OutPut);
								pPosition->dumpInLine(m_OutPut);
								fflush(m_OutPut);
							}
						}
						pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
					}
				}
				break;
			}
			case PD_Short:
			{
				if (p->Position < 0.0)
				{
					fprintf(m_OutPut, "checkPostion Position litter than 0 Postion Position:%s\n", p->Position.getAllString().c_str());
					p->dumpInLine(m_OutPut);
					fflush(m_OutPut);
				}
				if (p->Position < p->LongFrozen)
				{
					fprintf(m_OutPut, "checkPostion Position position[%s] < shortfrozen[%s]\n", p->Position.getString().c_str(), p->ShortFrozen.getString().c_str());
					p->dumpInLine(m_OutPut);
					fflush(m_OutPut);
				}
				if (p->Position != 0 || p->ShortFrozen != 0 || p->LongFrozen != 0)
				{
					CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByAccountID(p->AccountID);
					while (pPosition != NULL)
					{
						if (pPosition->getInstrument()->ProductClass == PC_Margin || pPosition->getInstrument()->ProductClass == PC_Premium)
						{
							if (pPosition->PosiDirection == PD_Net && (pPosition->Position != 0 || pPosition->ShortFrozen != 0 || pPosition->LongFrozen != 0))
							{
								fprintf(m_OutPut, "checkPostion Gross Position[%s] has Net Position[%s] \n", p->PositionID.getValue(), pPosition->PositionID.getValue());
								pPosition->dumpInLine(m_OutPut);
								fflush(m_OutPut);
							}
						}
						pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
					}
				}
				break;
			}
			case PD_Net:
			{
				//不可能存在双边持仓
				if (p->Position != 0 || p->ShortFrozen != 0 || p->LongFrozen != 0)
				{
					CPosition* pPosition = m_pMDB->m_PositionFactory->findFirstByAccountID(p->AccountID);
					while (pPosition != NULL)
					{
						if (pPosition->getInstrument()->ProductClass == PC_Margin || pPosition->getInstrument()->ProductClass == PC_Premium)
						{
							if (pPosition->PosiDirection != PD_Net && (pPosition->Position != 0 || pPosition->ShortFrozen != 0 || pPosition->LongFrozen != 0))
							{
								fprintf(m_OutPut, "checkPostion Net Position[%s] has Gross Position[%s] \n", p->PositionID.getValue(), pPosition->PositionID.getValue());
								p->dumpInLine(m_OutPut);
								pPosition->dumpInLine(m_OutPut);
								fflush(m_OutPut);
							}
						}
						pPosition = m_pMDB->m_PositionFactory->findNextByAccountID();
					}
				}
				break;
			}
			default:
			{
				fprintf(m_OutPut, "checkPostion Position litter than 0 Postion Position:%s\n", p->Position.getAllString().c_str());
				p->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
			}

			if (p->PosiDirection == PD_Net)
			{
				if (p->Position >= 0)
				{
					//正持仓：FrozenMargin = Max(useMargin + LongFrozenMargin, shortForzenMargin) - useMargin
					CTotalType frozen = MAX(p->UseMargin.getValue() + p->LongFrozenMargin.getValue(), p->ShortFrozenMargin.getValue()) - p->UseMargin.getValue();
					if (p->FrozenMargin != frozen)
					{
						fprintf(m_OutPut, "checkPostion FrozenMargin, FrozenMargin:%s,caulFrozenMargin:%s,Diff:%s\n",
							p->FrozenMargin.getAllString().c_str(), frozen.getAllString().c_str(), p->FrozenMargin.getDiffString(frozen).c_str());
						p->dumpInLine(m_OutPut);
						fflush(m_OutPut);
					}
				}
				else
				{
					//负持仓：FrozenMargin=Max(useMargin+shortForzenMargin , LongFrozenMargin)-useMargin
					CTotalType frozen = MAX(p->UseMargin.getValue() + p->ShortFrozenMargin.getValue(), p->LongFrozenMargin.getValue()) - p->UseMargin.getValue();
					if (p->FrozenMargin != frozen)
					{
						fprintf(m_OutPut, "checkPostion FrozenMargin, FrozenMargin:%s,caulFrozenMargin:%s,Diff:%s\n",
							p->FrozenMargin.getAllString().c_str(), frozen.getAllString().c_str(), p->FrozenMargin.getDiffString(frozen).c_str());
						p->dumpInLine(m_OutPut);
						fflush(m_OutPut);
					}
				}
			}
			else
			{
				if (p->FrozenMargin != (p->LongFrozenMargin + p->ShortFrozenMargin))
				{
					fprintf(m_OutPut, "checkPostion FrozenMargin,FrozenMargin:%s,LongFrozenMargin:%s,ShortFrozenMargin:%s,Diff:%s\n",
						p->FrozenMargin.getAllString().c_str(), p->LongFrozenMargin.getAllString().c_str(), p->ShortFrozenMargin.getAllString().c_str(),
						p->FrozenMargin.getDiffString(p->LongFrozenMargin.getDouble() + p->ShortFrozenMargin.getDouble()).c_str());
					p->dumpInLine(m_OutPut);
					fflush(m_OutPut);
				}
			}

			CVolumeType longFrozen = 0;
			CVolumeType shortFrozen = 0;
			CTotalType longFrozenMargin = 0;
			CTotalType shortFrozenMargin = 0;
			CTotalType frozenMargin = 0;

			COrder* pOrder = m_pMDB->m_OrderFactory->findFirstByPositionID(p->PositionID);
			while (pOrder != NULL)
			{
				if (pOrder->Direction == D_Sell)
				{
					shortFrozen += pOrder->VolumeRemain;
					shortFrozenMargin += pOrder->FrozenMargin;
				}
				else
				{
					longFrozen += pOrder->VolumeRemain;
					longFrozenMargin += pOrder->FrozenMargin;
				}
				pOrder = m_pMDB->m_OrderFactory->findNextByPositionID();
			}

			if (!p->LongFrozen.equal(longFrozen, 128))
			{
				fprintf(m_OutPut, "checkPostion LongFrozen, LongFrozen:%s,caulLongFrozen:%s,Diff:%s\n",
					p->LongFrozen.getAllString().c_str(), longFrozen.getAllString().c_str(), p->LongFrozen.getDiffString(longFrozen).c_str());
				p->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
			if (!p->ShortFrozen.equal(shortFrozen, 128))
			{
				fprintf(m_OutPut, "checkPostion ShortFrozen, ShortFrozen:%s,caulshortFrozen:%s,Diff:%s\n",
					p->ShortFrozen.getAllString().c_str(), shortFrozen.getAllString().c_str(), p->ShortFrozen.getDiffString(shortFrozen).c_str());
				p->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
			if (!p->LongFrozenMargin.equal(longFrozenMargin, 16384))
			{
				fprintf(m_OutPut, "checkPostion LongFrozenMargin, LongFrozenMargin:%s,caulLongFrozenMargin:%s,Diff:%s\n",
					p->LongFrozenMargin.getAllString().c_str(), longFrozenMargin.getAllString().c_str(), p->LongFrozenMargin.getDiffString(longFrozenMargin).c_str());
				p->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
			if (!p->ShortFrozenMargin.equal(shortFrozenMargin, 16384))
			{
				fprintf(m_OutPut, "checkPostion ShortFrozenMargin, ShortFrozenMargin:%s,caulShortFrozenMargin:%s,Diff:%s\n",
					p->ShortFrozenMargin.getAllString().c_str(), shortFrozenMargin.getAllString().c_str(), p->ShortFrozenMargin.getDiffString(shortFrozenMargin).c_str());
				p->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
		}
		p = m_pMDB->m_PositionFactory->getNext();
	}
	return true;
}

//it's ok
bool CSnapCheckEngine::checkAccount()
{
	map<string, double> BanlanceMap;
	//对账 balance avilable 
	CAccount* pAccount = m_pMDB->m_AccountFactory->getFirst();
	while (pAccount != NULL)
	{
		//如果找不到币种，报错
		CCurrency *pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pAccount->Currency, pAccount->SettlementGroup);
		if (pCurrency == NULL)
		{
			fprintf(m_OutPut, "Check Account Currency, Not Exsit\n");
			pAccount->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		CMoneyType balance =
			pAccount->PreBalance
			+ pAccount->MoneyChange
			+ pAccount->Deposit
			- pAccount->Withdraw
			+ pAccount->CloseProfit
			- pAccount->Fee;

		string balanceIndex = pAccount->SettlementGroup.getValue();
		balanceIndex += "_";
		balanceIndex += pAccount->Currency.getValue();
		map<string, double>::iterator iterBanlance = BanlanceMap.find(balanceIndex);
		if (iterBanlance == BanlanceMap.end())
		{
			BanlanceMap.insert(map<string, double>::value_type(balanceIndex, pAccount->Balance.getValue()+pAccount->Fee.getValue()));
			iterBanlance = BanlanceMap.find(balanceIndex);
		}	
		else
		{
			iterBanlance->second += pAccount->Balance.getValue() + pAccount->Fee.getValue();
		}

		CMoneyType available =
			pAccount->Balance
			- pAccount->UseMargin
			- pAccount->FrozenMargin
			- pAccount->FrozenMoney
			- pAccount->FrozenFee;

		CMoneyType reserve = pAccount->Reserve;
		if (reserve < 0.0)
			reserve = 0.0;

		CMoneyType withdrawable = pAccount->Available - reserve;

		if (pAccount->Balance != balance)
		{
			fprintf(m_OutPut, "Check Account Balance, accountBalance:%s,caluBalance:%s,Diff:%s\n",
				pAccount->Balance.getAllString().c_str(), balance.getAllString().c_str(), pAccount->Balance.getDiffString(balance).c_str());
			pAccount->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		if (pAccount->Available != available)
		{
			fprintf(m_OutPut, "check Account Available, accountAvailable:%s,caluAvailable:%s,Diff:%s\n",
				pAccount->Available.getAllString().c_str(), available.getAllString().c_str(), pAccount->Available.getDiffString(balance).c_str());
			pAccount->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		if (pAccount->Withdrawable != withdrawable)
		{
			fprintf(m_OutPut, "check Account Withdrawable, accountWithdrawable:%s,caluWithdrawable:%s,Diff:%s\n",
				pAccount->Withdrawable.getAllString().c_str(), withdrawable.getAllString().c_str(), pAccount->Withdrawable.getDiffString(balance).c_str());
			pAccount->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		CMoneyType useMargin = 0.0;
	//	CMoneyType positionCost = 0.0;
		CMoneyType frozenMargin = 0.0;
		CMoneyType frozenMoney = 0.0;
		CMoneyType frozenFee = 0.0;
		CVolumeType spotPosition = 0.0;

		CPosition* p = m_pMDB->m_PositionFactory->findFirstByAccountID(pAccount->AccountID);
		while (p != NULL)
		{
			if (p->SettlementGroup != pAccount->SettlementGroup || p->ClearCurrency != pAccount->Currency)
			{
				p = m_pMDB->m_PositionFactory->findNextByAccountID();
				continue;
			}
			useMargin += p->UseMargin;
			frozenMargin += p->FrozenMargin;
		//	positionCost += p->PositionCost;
			if (p->getInstrument()->ProductClass == PC_AccountExchange)
			{
				spotPosition += p->Position;
			}
			else
			{
				if (p->Position != 0.0)
				{
					//计算浮动盈亏
					CInstrument *pInstrument = p->getInstrument();
					CMarketData *pMarketData = pInstrument->getMarketData();
					CPriceType basePrice = pMarketData->LastPrice.getValue();
					if (!pMarketData->MarkedPrice.isNull())
						basePrice = pMarketData->MarkedPrice.getValue();

					double openPrice = fabs(p->PositionCost.getValue() / pInstrument->VolumeMultiple.getValue() / p->Position.getValue());
					if (p->PosiDirection == PD_Long || p->PosiDirection == PD_Net)
					{
						if (pInstrument->IsInverse)
							iterBanlance->second += pInstrument->VolumeMultiple.getValue() * p->Position.getValue()*(openPrice - 1 / basePrice.getValue());
						else
							iterBanlance->second += pInstrument->VolumeMultiple.getValue() * p->Position.getValue()*(basePrice.getValue() - openPrice);
					}
					else
					{
						if (pInstrument->IsInverse)
							iterBanlance->second += pInstrument->VolumeMultiple.getValue() * p->Position.getValue()*(1 / basePrice.getValue() - openPrice);
						else
							iterBanlance->second += pInstrument->VolumeMultiple.getValue() * p->Position.getValue()*(openPrice - basePrice.getValue());
					}
				}
			}
			p = m_pMDB->m_PositionFactory->findNextByAccountID();
		}

		COrder* pOrder = m_pMDB->m_OrderFactory->findFirstByAccountID(pAccount->AccountID);
		while (pOrder != NULL)
		{
			if (pOrder->SettlementGroup != pAccount->SettlementGroup || pOrder->ClearCurrency != pAccount->Currency)
			{
				pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
				continue;
			}
			frozenMoney += pOrder->FrozenMoney;
			frozenFee += pOrder->FrozenFee;
			pOrder = m_pMDB->m_OrderFactory->findNextByAccountID();
		}

		//if (!pAccount->PositionCost.equal(positionCost, 128))
		//{
		//	fprintf(m_OutPut, "check Account PositionCost, accountPositionCost:%s,PositionCost:%s,Diff:%s\n",
		//		pAccount->PositionCost.getAllString().c_str(), positionCost.getAllString().c_str(), pAccount->PositionCost.getDiffString(positionCost).c_str());
		//	pAccount->dumpInLine(m_OutPut);
		//	fflush(m_OutPut);
		//}

		if (!pAccount->UseMargin.equal(useMargin, 131072))
		{
			fprintf(m_OutPut, "checkAccount UseMargin, accountUseMargin:%s,caluUseMargin:%s,Diff:%s\n",
				pAccount->UseMargin.getAllString().c_str(), useMargin.getAllString().c_str(), pAccount->UseMargin.getDiffString(useMargin).c_str());
			pAccount->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		if (!pAccount->FrozenMargin.equal(frozenMargin, 16384))
		{
			fprintf(m_OutPut, "checkAccount FrozenMargin, accountFrozenMargin:%s,caluFrozenMargin:%s,Diff:%s\n",
				pAccount->FrozenMargin.getAllString().c_str(), frozenMargin.getAllString().c_str(), pAccount->FrozenMargin.getDiffString(frozenMargin).c_str());
			pAccount->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		if (!pAccount->FrozenMoney.equal(frozenMoney, 1024))
		{
			fprintf(m_OutPut, "checkAccount FrozenMoney, accountFrozenMoney:%s,caluFrozenMoney:%s,Diff:%s\n",
				pAccount->FrozenMoney.getAllString().c_str(), frozenMoney.getAllString().c_str(), pAccount->FrozenMoney.getDiffString(frozenMoney).c_str());
			pAccount->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		if (!pAccount->FrozenFee.equal(frozenFee, 16384))
		{
			fprintf(m_OutPut, "checkAccount FrozenFee, accountFrozenFee:%s,caluFrozenFee:%s,Diff:%s\n",
				pAccount->FrozenFee.getAllString().c_str(), frozenFee.getAllString().c_str(), pAccount->FrozenFee.getDiffString(frozenFee).c_str());
			pAccount->dumpInLine(m_OutPut);
			fflush(m_OutPut);
		}

		if (spotPosition > 0.0)
		{
			CCurrency* pCurrency = pAccount->getCurrency();
			if (!pAccount->Balance.equal(spotPosition, pCurrency->MoneyTick.getDouble()))
			{
				fprintf(m_OutPut, "checkAccount SpotPosition, accountBalance:%s,SpotPosition:%s,Diff:%s\n",
					pAccount->Balance.getAllString().c_str(), spotPosition.getAllString().c_str(), pAccount->Balance.getDiffString(spotPosition).c_str());
				pAccount->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
		}
		pAccount = m_pMDB->m_AccountFactory->getNext();
	}

	//对账：SUM(balance+浮动盈亏=prebalance+入金-出金)
	CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->getFirst();
	while (pCurrency != NULL)
	{
		string balanceIndex = pCurrency->SettlementGroup.getValue();
		balanceIndex += "_";
		balanceIndex += pCurrency->Currency.getValue();

		map<string, double>::iterator iter = BanlanceMap.find(balanceIndex);
		if (iter != BanlanceMap.end())
		{
			if (!pCurrency->Balance.equal(iter->second, 1048576))
			{
				fprintf(m_OutPut, "Check Account Currency Balance,[%s_%s][Currency:%s]!=[Account:%s][Diff:%s] \n", pCurrency->SettlementGroup.getValue(), pCurrency->Currency.getValue(),
					pCurrency->Balance.getString().c_str(), doubleToStringFormat(iter->second).c_str(), doubleToStringFormat(pCurrency->Balance.getValue() - iter->second).c_str());
				pCurrency->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
			else
			{
				printf("    CurrencyBalance[%s_%s]: Currency.Balance[%s] == AllAccountBlance[%s]\n", pCurrency->SettlementGroup.getValue(),pCurrency->Currency.getValue(),
					pCurrency->Balance.getString().c_str(), doubleToStringFormat(iter->second).c_str());
			}
		}
		else
		{
			if (pCurrency->Balance != 0.0)
			{
				fprintf(m_OutPut, "Check Account Currency, No Zero Currency Has No Account\n");
				pCurrency->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
		}
		pCurrency = m_pMDB->m_CurrencyFactory->getNext();
	}
	return true;
}

//===it's ok
bool CSnapCheckEngine::checkOrder()
{
	COrder* pOrder = m_pMDB->m_OrderFactory->getFirst();
	while (pOrder != NULL)
	{
		if (pOrder->Volume <= 0 || pOrder->VolumeDisplay < 0)
		{
			fprintf(m_OutPut, "volume or displayVolume litter than 0 AccountID:%s,OrderID:%s,InstrumentID:%s\n", pOrder->AccountID.getValue(), pOrder->OrderID.getValue(), pOrder->InstrumentID.getValue());
			pOrder->dumpInLine(m_OutPut);
			fflush(m_OutPut);
			pOrder = m_pMDB->m_OrderFactory->getNext();
			continue;
		}
		CInstrument* pInstrument = pOrder->getInstrument();
		if (pInstrument == NULL)
		{
			fprintf(m_OutPut, "InstrumentID is null AccountID:%s,OrderID:%s,InstrumentID:%s\n", pOrder->AccountID.getValue(), pOrder->OrderID.getValue(), pOrder->InstrumentID.getValue());
			pOrder->dumpInLine(m_OutPut);
			fflush(m_OutPut);
			pOrder = m_pMDB->m_OrderFactory->getNext();
			continue;
		}

		CWriteableOrder theOrder;
		m_pMDB->m_OrderFactory->retrieve(pOrder, &theOrder);

		switch (pInstrument->ProductClass)
		{
		case PC_Margin:
		case PC_Premium:
		{
			CFee* pFee = getFee(m_pMDB, MR_Taker, pInstrument, pOrder->getMember());

			CPriceType ClearPrice = pOrder->Price;
			if (pInstrument->IsInverse)
				ClearPrice = 1.0 / pOrder->Price;

			if (pOrder->OffsetFlag == OF_Open)
			{
				//冻结手续费
				theOrder.FrozenFee = pFee->OpenFeeAmount + pFee->OpenFeeRate * pOrder->VolumeRemain * ClearPrice * pInstrument->VolumeMultiple;
				//冻结保证金
				if (pOrder->Leverage > 0.0)
					theOrder.FrozenMargin = theOrder.VolumeRemain * pInstrument->VolumeMultiple * ClearPrice / pOrder->Leverage;
				else
					theOrder.FrozenMargin = -theOrder.VolumeRemain * pInstrument->VolumeMultiple * pOrder->Leverage;
			}
			else
			{
				double fee1 = pFee->CloseTodayFeeAmount + pFee->CloseTodayFeeRate * pOrder->VolumeRemain * ClearPrice * pInstrument->VolumeMultiple;
				double fee2 = pFee->CloseFeeAmount + pFee->CloseFeeRate * pOrder->VolumeRemain * ClearPrice * pInstrument->VolumeMultiple;
				theOrder.FrozenFee = MAX(fee1, fee2);
			}

			if (theOrder.FrozenFee != pOrder->FrozenFee)
			{
				fprintf(m_OutPut, "order FrozenFee is wrong, FrozenFee:%s,caluFrozenFee:%s,Diff:%s\n",
					pOrder->FrozenFee.getAllString().c_str(), theOrder.FrozenFee.getAllString().c_str(), pOrder->FrozenFee.getDiffString(theOrder.FrozenFee).c_str());
				pOrder->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
			if (theOrder.FrozenMargin != pOrder->FrozenMargin)
			{
				fprintf(m_OutPut, "order FrozenMargin is wrong, FrozenMargin:%s,caluFrozenMargin:%s,Diff:%s\n",
					pOrder->FrozenMargin.getAllString().c_str(), theOrder.FrozenMargin.getAllString().c_str(), pOrder->FrozenMargin.getDiffString(theOrder.FrozenMargin).c_str());
				pOrder->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
			break;
		}
		case PC_AccountExchange:
		{
			if (pOrder->Direction == D_Buy)
				theOrder.FrozenMoney = theOrder.VolumeRemain * theOrder.Price * pInstrument->VolumeMultiple;
			else
				theOrder.FrozenMoney = theOrder.VolumeRemain;

			if (theOrder.FrozenMoney != pOrder->FrozenMoney)
			{
				fprintf(m_OutPut, "order FrozenMoney is wrong, FrozenMoney:%s,caluFrozenMoney:%s,Diff:%s\n",
					pOrder->FrozenMoney.getAllString().c_str(), theOrder.FrozenMoney.getAllString().c_str(), pOrder->FrozenMoney.getDiffString(theOrder.FrozenMoney).c_str());
				pOrder->dumpInLine(m_OutPut);
				fflush(m_OutPut);
			}
		}
		default:
		{
			theOrder.FrozenFee = 0.0;
			theOrder.FrozenMargin = 0.0;
		}
		}
		pOrder = m_pMDB->m_OrderFactory->getNext();
	}
	return true;
}

bool CSnapCheckEngine::checkSpotTick()
{
	CInstrument* pInstrument = m_pMDB->m_InstrumentFactory->getFirst();
	while (pInstrument != NULL)
	{
		if (!pInstrument->ClearCurrency.isNull())
		{
			CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pInstrument->ClearCurrency, pInstrument->SettlementGroup);
			if (pCurrency == NULL)
			{
				fprintf(m_OutPut, "Instrument[%s] cannot find ClearCurrency[%s]\n", pInstrument->InstrumentID.getValue(), pInstrument->ClearCurrency.getValue());
				fflush(m_OutPut);
				pInstrument = m_pMDB->m_InstrumentFactory->getNext();
				continue;
			}
		}

		if (pInstrument->ProductClass == PC_AccountExchange)
		{
			CCurrency* pCurrency = m_pMDB->m_CurrencyFactory->findByCurrency(pInstrument->BaseCurrency, pInstrument->SettlementGroup);
			if (pCurrency == NULL)
			{
				fprintf(m_OutPut, "Instrument[%s] cannot find BaseCurrency[%s]\n", pInstrument->InstrumentID.getValue(), pInstrument->BaseCurrency.getValue());
				fflush(m_OutPut);
				pInstrument = m_pMDB->m_InstrumentFactory->getNext();
				continue;
			}

			if (!pCurrency->MoneyTick.isNull())
			{
				if (pInstrument->VolumeTick != pCurrency->MoneyTick)
				{
					fprintf(m_OutPut, "Instrument[%s] VolumeTick[%s] != Currency MoneyTick[%s]\n",
						pInstrument->InstrumentID.getValue(),pInstrument->VolumeTick.getAllString().c_str(), pCurrency->MoneyTick.getAllString().c_str());
					fflush(m_OutPut);
				}
			}
		}
		pInstrument = m_pMDB->m_InstrumentFactory->getNext();
	}
	return true;
}

bool CSnapCheckEngine::checkTriggerOrder()
{
	CTriggerOrder* pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getFirst();
	while (pTriggerOrder != NULL)
	{
		//开仓的TriggerOrder还没有进入队列，不需要生成持仓
		if (pTriggerOrder->OffsetFlag != OF_Open && pTriggerOrder->TriggerStatus != TS_None)
		{
			//平仓的TriggerOrder是一定又持仓的，要不然平什么呢
			if (pTriggerOrder->PositionID.isNull())
			{
				fprintf(m_OutPut, "TriggerOrder[%s] PositionID is Null\n", pTriggerOrder->OrderID.getValue());
				fflush(m_OutPut);
			}
			//如果是全平仓订单，需要posiition记录其id的
			if (pTriggerOrder->OffsetFlag == OF_CloseAll)
			{
				CPosition* pPosition = m_pMDB->m_PositionFactory->findByTradeUnitID(pTriggerOrder->AccountID, pTriggerOrder->ExchangeID, pTriggerOrder->InstrumentID,pTriggerOrder->TradeUnitID, pTriggerOrder->PosiDirection);
				if (pPosition == NULL)
				{
					fprintf(m_OutPut, "CloseAll TriggerOrder[%s] Position is Null\n", pTriggerOrder->OrderID.getValue());
					fflush(m_OutPut);
				}
				else
				{
					if (pPosition->CloseOrderID != pTriggerOrder->OrderID)
					{
						fprintf(m_OutPut, "CloseAll TriggerOrder[%s] != Position CloseOrderID[%s]\n", pTriggerOrder->OrderID.getValue(), pPosition->CloseOrderID.getValue());
						pTriggerOrder->dumpInLine(m_OutPut);
						fflush(m_OutPut);
					}
					if (pPosition->SLTriggerPrice != pTriggerOrder->SLTriggerPrice)
					{
						fprintf(m_OutPut, "CloseAll TriggerOrder[%s][%s] != Position SLTriggerPrice[%s]\n", pTriggerOrder->OrderID.getValue(),
							pTriggerOrder->SLTriggerPrice.getAllString().c_str(), pPosition->SLTriggerPrice.getAllString().c_str());
						fflush(m_OutPut);
					}
					if (pPosition->TPTriggerPrice != pTriggerOrder->TPTriggerPrice)
					{
						fprintf(m_OutPut, "CloseAll TriggerOrder[%s][%s] != Position TPTriggerPrice[%s]\n", pTriggerOrder->OrderID.getValue(),
							pTriggerOrder->TPTriggerPrice.getAllString().c_str(), pPosition->TPTriggerPrice.getAllString().c_str());
						fflush(m_OutPut);
					}

				}
			}
		}

		pTriggerOrder = m_pMDB->m_TriggerOrderFactory->getNext();
	}
	return true;
}

void CSnapCheckEngine::checkLink()
{
	m_pAlterService->checkLink(m_OutPut);
}

void CSnapCheckEngine::checkNull()
{
	m_pAlterService->checkNull(m_OutPut);
}

void CSnapCheckEngine::checkValid()
{
	m_pAlterService->checkValid(m_OutPut);
}

bool CSnapCheckEngine::SnapCheck()
{
	//校验非空
	checkNull();
	//校验link
	checkLink();
	//校验数据合法性
	checkValid();

	char sTimeBuffer[36];
	printf("checkOrder:%s\n", getLocalTime(sTimeBuffer));
	//校验order
	checkOrder();
	printf("checkPostion:%s\n", getLocalTime(sTimeBuffer));
	//校验仓位数据
	checkPostion();
	printf("checkAccount:%s\n", getLocalTime(sTimeBuffer));
	//账户相关校验
	checkAccount();
	printf("checkMemberPostion:%s\n", getLocalTime(sTimeBuffer));
	// memberPosition合计position(ok) ok
	checkMemberPostion();
	// user校验 ok
	printf("checkUser:%s\n", getLocalTime(sTimeBuffer));
	checkMember();
	printf("checkSpotTick:%s\n", getLocalTime(sTimeBuffer));
	checkSpotTick();
	printf("checkTriggerOrder:%s\n", getLocalTime(sTimeBuffer));
	checkTriggerOrder();

	//多空持仓量一样 
	printf("checkTriggerOrder:%s\n", getLocalTime(sTimeBuffer));
	checkLongEqualShort();

	//关闭文件
	OutPutClose();
	printf("checkSnapEnd:%s\n", getLocalTime(sTimeBuffer));
	return true;
}