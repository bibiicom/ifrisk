#include "mdbStruct.h"
#include "mdbTable.h"
#include "mdbAction.h"
#include "monitorIndex.h"
#include "CopyField.h"

int compareForSystemIDIndexinCurrentTime(const void *pV1, const void *pV2)
{
	CCurrentTime *p1, *p2;
	p1 = (CCurrentTime *)pV1;
	p2 = (CCurrentTime *)pV2;
	switch (compare(p1->SystemID.getValue(), p2->SystemID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CCurrentTime* CCurrentTimeFactory::findBySystemID(const CReadOnlyShortIndexIDType& SystemID)
{	
	return NULL;
}

CCurrentTime *CCurrentTimeFactory::findByPK(const CReadOnlyShortIndexIDType& SystemID)
{
	return findBySystemID(SystemID);
}

bool CCurrentTimeFactory::removeByPK(const CReadOnlyShortIndexIDType& SystemID, CTransaction *pTransaction)
{
	CCurrentTime *pOriginalCurrentTime;
	pOriginalCurrentTime = findBySystemID(SystemID);
	if (pOriginalCurrentTime == NULL)
		return false;
	remove(pOriginalCurrentTime, pTransaction);
	return true;
}

CCurrentTime *CCurrentTimeFactory::findByPK(CCurrentTimeField *pCurrentTime)
{
	return findByPK(pCurrentTime->SystemID);
}

CCurrentTime *CCurrentTimeFactory::findByPK(CCurrentTime *pCurrentTime)
{
	return findByPK(pCurrentTime->SystemID);
}

bool CCurrentTimeFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "SystemID"))
		return true;
	return false;
}

bool CCurrentTimeFactory::retrieveByPK(CWriteableCurrentTime *pCurrentTime)
{
	CCurrentTime *pOriginalCurrentTime;
	pOriginalCurrentTime = findByPK(pCurrentTime);
	if (pOriginalCurrentTime == NULL)
		return false;
	retrieve(pOriginalCurrentTime, pCurrentTime);
	return true;
}

CCurrentTime *CCurrentTimeFactory::addByPK(CWriteableCurrentTime *pCurrentTime, CTransaction *pTransaction)
{
	CCurrentTime *pOriginalCurrentTime;
	pOriginalCurrentTime = findByPK(pCurrentTime);
	if (pOriginalCurrentTime != NULL)
		return NULL;
	return add(pCurrentTime, pTransaction);
}

bool CCurrentTimeFactory::updateByPK(CWriteableCurrentTime *pCurrentTime, CTransaction *pTransaction)
{
	CCurrentTime *pOriginalCurrentTime;
	pOriginalCurrentTime = findByPK(pCurrentTime);
	if (pOriginalCurrentTime == NULL)
		return false;
	pCurrentTime->calAllHash();
	update(pOriginalCurrentTime, pCurrentTime, pTransaction);
	return true;
}

CCurrentTime* CCurrentTimeFactory::addOrUpdateByPK(CWriteableCurrentTime *pCurrentTime, CTransaction *pTransaction)
{
	CCurrentTime *pOriginalCurrentTime;
	pOriginalCurrentTime = findByPK(pCurrentTime);
	if (pOriginalCurrentTime == NULL)
		return add(pCurrentTime, pTransaction);
	pCurrentTime->calAllHash();
	update(pOriginalCurrentTime, pCurrentTime, pTransaction);
	return pOriginalCurrentTime;
}

bool CCurrentTimeFactory::removeByPK(CCurrentTime *pCurrentTime, CTransaction *pTransaction)
{
	CCurrentTime *pOriginalCurrentTime;
	pOriginalCurrentTime = findByPK(pCurrentTime);
	if (pOriginalCurrentTime == NULL)
		return false;
	remove(pOriginalCurrentTime, pTransaction);
	return true;
}

bool CCurrentTimeFactory::removeByPK(CCurrentTimeField *pCurrentTime, CTransaction *pTransaction)
{
	CCurrentTime *pOriginalCurrentTime;
	pOriginalCurrentTime = findByPK(pCurrentTime);
	if (pOriginalCurrentTime == NULL)
		return false;
	CopyCurrentTimeEntity(pCurrentTime, pOriginalCurrentTime);
	remove(pOriginalCurrentTime, pTransaction);
	return true;
}
	
CMember* CMemberFactory::findByMemberID(const CReadOnlyLongIndexIDType& MemberID)
{	
	CHashIndexNode *pNode;
	compareObject.MemberID = MemberID.getValue();

	compareObject.calHashMemberID();
	for (pNode = pMemberIDHashIndex->searchFirstEqual(compareObject.HashMemberID);
		pNode != NULL;
		pNode = pMemberIDHashIndex->searchNextEqual(pNode))
	{
		if (((CMember *)(pNode->pObject))->HashMemberID != compareObject.HashMemberID)
			continue;
		if (compare(((CMember *)(pNode->pObject))->MemberID.getValue(), MemberID.getValue()) != 0)
			continue;
		return (CMember *)(pNode->pObject);
	}
	return NULL;
}

CMember *CMemberFactory::findByPK(const CReadOnlyLongIndexIDType& MemberID)
{
	return findByMemberID(MemberID);
}

bool CMemberFactory::removeByPK(const CReadOnlyLongIndexIDType& MemberID, CTransaction *pTransaction)
{
	CMember *pOriginalMember;
	pOriginalMember = findByMemberID(MemberID);
	if (pOriginalMember == NULL)
		return false;
	remove(pOriginalMember, pTransaction);
	return true;
}

CMember *CMemberFactory::findByPK(CMemberField *pMember)
{
	return findByPK(pMember->MemberID);
}

CMember *CMemberFactory::findByPK(CMember *pMember)
{
	return findByPK(pMember->MemberID);
}

bool CMemberFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "MemberID"))
		return true;
	return false;
}

bool CMemberFactory::retrieveByPK(CWriteableMember *pMember)
{
	CMember *pOriginalMember;
	pOriginalMember = findByPK(pMember);
	if (pOriginalMember == NULL)
		return false;
	retrieve(pOriginalMember, pMember);
	return true;
}

CMember *CMemberFactory::addByPK(CWriteableMember *pMember, CTransaction *pTransaction)
{
	CMember *pOriginalMember;
	pOriginalMember = findByPK(pMember);
	if (pOriginalMember != NULL)
		return NULL;
	return add(pMember, pTransaction);
}

bool CMemberFactory::updateByPK(CWriteableMember *pMember, CTransaction *pTransaction)
{
	CMember *pOriginalMember;
	pOriginalMember = findByPK(pMember);
	if (pOriginalMember == NULL)
		return false;
	pMember->calAllHash();
	update(pOriginalMember, pMember, pTransaction);
	return true;
}

CMember* CMemberFactory::addOrUpdateByPK(CWriteableMember *pMember, CTransaction *pTransaction)
{
	CMember *pOriginalMember;
	pOriginalMember = findByPK(pMember);
	if (pOriginalMember == NULL)
		return add(pMember, pTransaction);
	pMember->calAllHash();
	update(pOriginalMember, pMember, pTransaction);
	return pOriginalMember;
}

bool CMemberFactory::removeByPK(CMember *pMember, CTransaction *pTransaction)
{
	CMember *pOriginalMember;
	pOriginalMember = findByPK(pMember);
	if (pOriginalMember == NULL)
		return false;
	remove(pOriginalMember, pTransaction);
	return true;
}

bool CMemberFactory::removeByPK(CMemberField *pMember, CTransaction *pTransaction)
{
	CMember *pOriginalMember;
	pOriginalMember = findByPK(pMember);
	if (pOriginalMember == NULL)
		return false;
	CopyMemberEntity(pMember, pOriginalMember);
	remove(pOriginalMember, pTransaction);
	return true;
}
int compareForConfigIndexinServiceConfig(const void *pV1, const void *pV2)
{
	CServiceConfig *p1, *p2;
	p1 = (CServiceConfig *)pV1;
	p2 = (CServiceConfig *)pV2;
	switch (compare(p1->ConfigName.getValue(), p2->ConfigName.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Index1.getValue(), p2->Index1.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Index2.getValue(), p2->Index2.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CServiceConfig* CServiceConfigFactory::findByConfig(const CReadOnlyNameType& ConfigName, const CReadOnlyNameType& Index1, const CReadOnlyNameType& Index2)
{	
	CAVLNode *pNode;
	compareObject.ConfigName = ConfigName.getValue();
	compareObject.Index1 = Index1.getValue();
	compareObject.Index2 = Index2.getValue();
	pNode = pConfigIndex->searchFirstEqual(&compareObject);
	if (pNode == NULL)
		return NULL;
	return (CServiceConfig *)(pNode->pObject);
}

CServiceConfig *CServiceConfigFactory::findByPK(const CReadOnlyNameType& ConfigName, const CReadOnlyNameType& Index1, const CReadOnlyNameType& Index2)
{
	return findByConfig(ConfigName, Index1, Index2);
}

bool CServiceConfigFactory::removeByPK(const CReadOnlyNameType& ConfigName, const CReadOnlyNameType& Index1, const CReadOnlyNameType& Index2, CTransaction *pTransaction)
{
	CServiceConfig *pOriginalServiceConfig;
	pOriginalServiceConfig = findByConfig(ConfigName, Index1, Index2);
	if (pOriginalServiceConfig == NULL)
		return false;
	remove(pOriginalServiceConfig, pTransaction);
	return true;
}

CServiceConfig *CServiceConfigFactory::findByPK(CServiceConfigField *pServiceConfig)
{
	return findByPK(pServiceConfig->ConfigName, pServiceConfig->Index1, pServiceConfig->Index2);
}

CServiceConfig *CServiceConfigFactory::findByPK(CServiceConfig *pServiceConfig)
{
	return findByPK(pServiceConfig->ConfigName, pServiceConfig->Index1, pServiceConfig->Index2);
}

bool CServiceConfigFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "ConfigName"))
		return true;
	if (!strcmp(pName, "Index1"))
		return true;
	if (!strcmp(pName, "Index2"))
		return true;
	return false;
}

bool CServiceConfigFactory::retrieveByPK(CWriteableServiceConfig *pServiceConfig)
{
	CServiceConfig *pOriginalServiceConfig;
	pOriginalServiceConfig = findByPK(pServiceConfig);
	if (pOriginalServiceConfig == NULL)
		return false;
	retrieve(pOriginalServiceConfig, pServiceConfig);
	return true;
}

CServiceConfig *CServiceConfigFactory::addByPK(CWriteableServiceConfig *pServiceConfig, CTransaction *pTransaction)
{
	CServiceConfig *pOriginalServiceConfig;
	pOriginalServiceConfig = findByPK(pServiceConfig);
	if (pOriginalServiceConfig != NULL)
		return NULL;
	return add(pServiceConfig, pTransaction);
}

bool CServiceConfigFactory::updateByPK(CWriteableServiceConfig *pServiceConfig, CTransaction *pTransaction)
{
	CServiceConfig *pOriginalServiceConfig;
	pOriginalServiceConfig = findByPK(pServiceConfig);
	if (pOriginalServiceConfig == NULL)
		return false;
	pServiceConfig->calAllHash();
	update(pOriginalServiceConfig, pServiceConfig, pTransaction);
	return true;
}

CServiceConfig* CServiceConfigFactory::addOrUpdateByPK(CWriteableServiceConfig *pServiceConfig, CTransaction *pTransaction)
{
	CServiceConfig *pOriginalServiceConfig;
	pOriginalServiceConfig = findByPK(pServiceConfig);
	if (pOriginalServiceConfig == NULL)
		return add(pServiceConfig, pTransaction);
	pServiceConfig->calAllHash();
	update(pOriginalServiceConfig, pServiceConfig, pTransaction);
	return pOriginalServiceConfig;
}

bool CServiceConfigFactory::removeByPK(CServiceConfig *pServiceConfig, CTransaction *pTransaction)
{
	CServiceConfig *pOriginalServiceConfig;
	pOriginalServiceConfig = findByPK(pServiceConfig);
	if (pOriginalServiceConfig == NULL)
		return false;
	remove(pOriginalServiceConfig, pTransaction);
	return true;
}

bool CServiceConfigFactory::removeByPK(CServiceConfigField *pServiceConfig, CTransaction *pTransaction)
{
	CServiceConfig *pOriginalServiceConfig;
	pOriginalServiceConfig = findByPK(pServiceConfig);
	if (pOriginalServiceConfig == NULL)
		return false;
	CopyServiceConfigEntity(pServiceConfig, pOriginalServiceConfig);
	remove(pOriginalServiceConfig, pTransaction);
	return true;
}



CServiceConfig* CServiceConfigFactory::findFirstByIndex1(const CReadOnlyNameType& ConfigName, const CReadOnlyNameType& Index1)
{
	pLastFoundInSearchByIndex1 = NULL;
	///记录所有的查询值
	queryConfigNameInSearchByIndex1 = ConfigName.getValue();
	queryIndex1InSearchByIndex1 = Index1.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CServiceConfig *theServiceConfig;
	compareObject.ConfigName = ConfigName.getValue();
	compareObject.Index1 = Index1.getValue();
	compareObject.Index2 = "";

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByIndex1 = pConfigIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByIndex1 == NULL)
		return NULL;

	///获取找到的对象
	theServiceConfig = (CServiceConfig *)(pLastFoundInSearchByIndex1->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theServiceConfig->ConfigName.getValue(), ConfigName.getValue()))
	{
		pLastFoundInSearchByIndex1 = NULL;
		return NULL;
	}
	if (compare(theServiceConfig->Index1.getValue(), Index1.getValue()))
	{
		pLastFoundInSearchByIndex1 = NULL;
		return NULL;
	}

	return theServiceConfig;
}

CServiceConfig *CServiceConfigFactory::findNextByIndex1()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByIndex1 == NULL)
		return NULL;

	CServiceConfig *theServiceConfig;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByIndex1 = pConfigIndex->getNextNode(pLastFoundInSearchByIndex1);
	if (pLastFoundInSearchByIndex1 == NULL)
		return NULL;

	theServiceConfig = (CServiceConfig *)(pLastFoundInSearchByIndex1->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theServiceConfig->ConfigName, queryConfigNameInSearchByIndex1))
	{
		pLastFoundInSearchByIndex1 = NULL;
		return NULL;
	}
	if (compare(theServiceConfig->Index1, queryIndex1InSearchByIndex1))
	{
		pLastFoundInSearchByIndex1 = NULL;
		return NULL;
	}

	///返回找到的结果
	return theServiceConfig;
}

int compareForUserIDAndAppIDIndexinUserSession(const void *pV1, const void *pV2)
{
	CUserSession *p1, *p2;
	p1 = (CUserSession *)pV1;
	p2 = (CUserSession *)pV2;
	switch (compare(p1->UserID.getValue(), p2->UserID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->APPID.getValue(), p2->APPID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CUserSession* CUserSessionFactory::findByToken(const CReadOnlyShortContentType& Token)
{	
	CHashIndexNode *pNode;
	compareObject.Token = Token.getValue();

	compareObject.calHashToken();
	for (pNode = pTokenIndex->searchFirstEqual(compareObject.HashToken);
		pNode != NULL;
		pNode = pTokenIndex->searchNextEqual(pNode))
	{
		if (((CUserSession *)(pNode->pObject))->HashToken != compareObject.HashToken)
			continue;
		if (compare(((CUserSession *)(pNode->pObject))->Token.getValue(), Token.getValue()) != 0)
			continue;
		return (CUserSession *)(pNode->pObject);
	}
	return NULL;
}

CUserSession *CUserSessionFactory::findByPK(const CReadOnlyShortContentType& Token)
{
	return findByToken(Token);
}

bool CUserSessionFactory::removeByPK(const CReadOnlyShortContentType& Token, CTransaction *pTransaction)
{
	CUserSession *pOriginalUserSession;
	pOriginalUserSession = findByToken(Token);
	if (pOriginalUserSession == NULL)
		return false;
	remove(pOriginalUserSession, pTransaction);
	return true;
}

CUserSession *CUserSessionFactory::findByPK(CUserSessionField *pUserSession)
{
	return findByPK(pUserSession->Token);
}

CUserSession *CUserSessionFactory::findByPK(CUserSession *pUserSession)
{
	return findByPK(pUserSession->Token);
}

bool CUserSessionFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "Token"))
		return true;
	return false;
}

bool CUserSessionFactory::retrieveByPK(CWriteableUserSession *pUserSession)
{
	CUserSession *pOriginalUserSession;
	pOriginalUserSession = findByPK(pUserSession);
	if (pOriginalUserSession == NULL)
		return false;
	retrieve(pOriginalUserSession, pUserSession);
	return true;
}

CUserSession *CUserSessionFactory::addByPK(CWriteableUserSession *pUserSession, CTransaction *pTransaction)
{
	CUserSession *pOriginalUserSession;
	pOriginalUserSession = findByPK(pUserSession);
	if (pOriginalUserSession != NULL)
		return NULL;
	return add(pUserSession, pTransaction);
}

bool CUserSessionFactory::updateByPK(CWriteableUserSession *pUserSession, CTransaction *pTransaction)
{
	CUserSession *pOriginalUserSession;
	pOriginalUserSession = findByPK(pUserSession);
	if (pOriginalUserSession == NULL)
		return false;
	pUserSession->calAllHash();
	update(pOriginalUserSession, pUserSession, pTransaction);
	return true;
}

CUserSession* CUserSessionFactory::addOrUpdateByPK(CWriteableUserSession *pUserSession, CTransaction *pTransaction)
{
	CUserSession *pOriginalUserSession;
	pOriginalUserSession = findByPK(pUserSession);
	if (pOriginalUserSession == NULL)
		return add(pUserSession, pTransaction);
	pUserSession->calAllHash();
	update(pOriginalUserSession, pUserSession, pTransaction);
	return pOriginalUserSession;
}

bool CUserSessionFactory::removeByPK(CUserSession *pUserSession, CTransaction *pTransaction)
{
	CUserSession *pOriginalUserSession;
	pOriginalUserSession = findByPK(pUserSession);
	if (pOriginalUserSession == NULL)
		return false;
	remove(pOriginalUserSession, pTransaction);
	return true;
}

bool CUserSessionFactory::removeByPK(CUserSessionField *pUserSession, CTransaction *pTransaction)
{
	CUserSession *pOriginalUserSession;
	pOriginalUserSession = findByPK(pUserSession);
	if (pOriginalUserSession == NULL)
		return false;
	CopyUserSessionEntity(pUserSession, pOriginalUserSession);
	remove(pOriginalUserSession, pTransaction);
	return true;
}
CUserSession* CUserSessionFactory::findByUserIDAndAppID(const CReadOnlyLongIndexIDType& UserID, const CReadOnlyShortIndexIDType& APPID)
{	
	CAVLNode *pNode;
	compareObject.UserID = UserID.getValue();
	compareObject.APPID = APPID.getValue();
	pNode = pUserIDAndAppIDIndex->searchFirstEqual(&compareObject);
	if (pNode == NULL)
		return NULL;
	return (CUserSession *)(pNode->pObject);
}

int compareForSettlementGroupIndexinInstrument(const void *pV1, const void *pV2)
{
	CInstrument *p1, *p2;
	p1 = (CInstrument *)pV1;
	p2 = (CInstrument *)pV2;
	switch (compare(p1->SettlementGroup.getValue(), p2->SettlementGroup.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForProductGroupIndexinInstrument(const void *pV1, const void *pV2)
{
	CInstrument *p1, *p2;
	p1 = (CInstrument *)pV1;
	p2 = (CInstrument *)pV2;
	switch (compare(p1->ProductGroup.getValue(), p2->ProductGroup.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CInstrument* CInstrumentFactory::findByInstrumentID(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{	
	CHashIndexNode *pNode;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();

	compareObject.calHashInstrumentID();
	for (pNode = pInstrumentIDHashIndex->searchFirstEqual(compareObject.HashInstrumentID);
		pNode != NULL;
		pNode = pInstrumentIDHashIndex->searchNextEqual(pNode))
	{
		if (((CInstrument *)(pNode->pObject))->HashInstrumentID != compareObject.HashInstrumentID)
			continue;
		if (compare(((CInstrument *)(pNode->pObject))->ExchangeID.getValue(), ExchangeID.getValue()) != 0)
			continue;
		if (compare(((CInstrument *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		return (CInstrument *)(pNode->pObject);
	}
	return NULL;
}

CInstrument *CInstrumentFactory::findByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	return findByInstrumentID(ExchangeID, InstrumentID);
}

bool CInstrumentFactory::removeByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, CTransaction *pTransaction)
{
	CInstrument *pOriginalInstrument;
	pOriginalInstrument = findByInstrumentID(ExchangeID, InstrumentID);
	if (pOriginalInstrument == NULL)
		return false;
	remove(pOriginalInstrument, pTransaction);
	return true;
}

CInstrument *CInstrumentFactory::findByPK(CInstrumentField *pInstrument)
{
	return findByPK(pInstrument->ExchangeID, pInstrument->InstrumentID);
}

CInstrument *CInstrumentFactory::findByPK(CInstrument *pInstrument)
{
	return findByPK(pInstrument->ExchangeID, pInstrument->InstrumentID);
}

bool CInstrumentFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "ExchangeID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	return false;
}

bool CInstrumentFactory::retrieveByPK(CWriteableInstrument *pInstrument)
{
	CInstrument *pOriginalInstrument;
	pOriginalInstrument = findByPK(pInstrument);
	if (pOriginalInstrument == NULL)
		return false;
	retrieve(pOriginalInstrument, pInstrument);
	return true;
}

CInstrument *CInstrumentFactory::addByPK(CWriteableInstrument *pInstrument, CTransaction *pTransaction)
{
	CInstrument *pOriginalInstrument;
	pOriginalInstrument = findByPK(pInstrument);
	if (pOriginalInstrument != NULL)
		return NULL;
	return add(pInstrument, pTransaction);
}

bool CInstrumentFactory::updateByPK(CWriteableInstrument *pInstrument, CTransaction *pTransaction)
{
	CInstrument *pOriginalInstrument;
	pOriginalInstrument = findByPK(pInstrument);
	if (pOriginalInstrument == NULL)
		return false;
	pInstrument->calAllHash();
	update(pOriginalInstrument, pInstrument, pTransaction);
	return true;
}

CInstrument* CInstrumentFactory::addOrUpdateByPK(CWriteableInstrument *pInstrument, CTransaction *pTransaction)
{
	CInstrument *pOriginalInstrument;
	pOriginalInstrument = findByPK(pInstrument);
	if (pOriginalInstrument == NULL)
		return add(pInstrument, pTransaction);
	pInstrument->calAllHash();
	update(pOriginalInstrument, pInstrument, pTransaction);
	return pOriginalInstrument;
}

bool CInstrumentFactory::removeByPK(CInstrument *pInstrument, CTransaction *pTransaction)
{
	CInstrument *pOriginalInstrument;
	pOriginalInstrument = findByPK(pInstrument);
	if (pOriginalInstrument == NULL)
		return false;
	remove(pOriginalInstrument, pTransaction);
	return true;
}

bool CInstrumentFactory::removeByPK(CInstrumentField *pInstrument, CTransaction *pTransaction)
{
	CInstrument *pOriginalInstrument;
	pOriginalInstrument = findByPK(pInstrument);
	if (pOriginalInstrument == NULL)
		return false;
	CopyInstrumentEntity(pInstrument, pOriginalInstrument);
	remove(pOriginalInstrument, pTransaction);
	return true;
}



CInstrument* CInstrumentFactory::findFirstBySettlementGroup(const CReadOnlyShortIndexIDType& SettlementGroup)
{
	pLastFoundInSearchBySettlementGroup = NULL;
	///记录所有的查询值
	querySettlementGroupInSearchBySettlementGroup = SettlementGroup.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CInstrument *theInstrument;
	compareObject.SettlementGroup = SettlementGroup.getValue();

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchBySettlementGroup = pSettlementGroupIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchBySettlementGroup == NULL)
		return NULL;

	///获取找到的对象
	theInstrument = (CInstrument *)(pLastFoundInSearchBySettlementGroup->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theInstrument->SettlementGroup.getValue(), SettlementGroup.getValue()))
	{
		pLastFoundInSearchBySettlementGroup = NULL;
		return NULL;
	}

	return theInstrument;
}

CInstrument *CInstrumentFactory::findNextBySettlementGroup()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchBySettlementGroup == NULL)
		return NULL;

	CInstrument *theInstrument;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchBySettlementGroup = pSettlementGroupIndex->getNextNode(pLastFoundInSearchBySettlementGroup);
	if (pLastFoundInSearchBySettlementGroup == NULL)
		return NULL;

	theInstrument = (CInstrument *)(pLastFoundInSearchBySettlementGroup->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theInstrument->SettlementGroup, querySettlementGroupInSearchBySettlementGroup))
	{
		pLastFoundInSearchBySettlementGroup = NULL;
		return NULL;
	}

	///返回找到的结果
	return theInstrument;
}




CInstrument* CInstrumentFactory::findFirstByProductGroup(const CReadOnlyShortIndexIDType& ProductGroup)
{
	pLastFoundInSearchByProductGroup = NULL;
	///记录所有的查询值
	queryProductGroupInSearchByProductGroup = ProductGroup.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CInstrument *theInstrument;
	compareObject.ProductGroup = ProductGroup.getValue();

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByProductGroup = pProductGroupIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByProductGroup == NULL)
		return NULL;

	///获取找到的对象
	theInstrument = (CInstrument *)(pLastFoundInSearchByProductGroup->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theInstrument->ProductGroup.getValue(), ProductGroup.getValue()))
	{
		pLastFoundInSearchByProductGroup = NULL;
		return NULL;
	}

	return theInstrument;
}

CInstrument *CInstrumentFactory::findNextByProductGroup()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByProductGroup == NULL)
		return NULL;

	CInstrument *theInstrument;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByProductGroup = pProductGroupIndex->getNextNode(pLastFoundInSearchByProductGroup);
	if (pLastFoundInSearchByProductGroup == NULL)
		return NULL;

	theInstrument = (CInstrument *)(pLastFoundInSearchByProductGroup->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theInstrument->ProductGroup, queryProductGroupInSearchByProductGroup))
	{
		pLastFoundInSearchByProductGroup = NULL;
		return NULL;
	}

	///返回找到的结果
	return theInstrument;
}

	
CTradingRight* CTradingRightFactory::findByMemberIDAndInstrument(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyDirectionType& Direction)
{	
	CHashIndexNode *pNode;
	compareObject.MemberID = MemberID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Direction = Direction.getValue();

	compareObject.calHashMemberIDAndInstrument();
	for (pNode = pMemberIDAndInstrumentHashIndex->searchFirstEqual(compareObject.HashMemberIDAndInstrument);
		pNode != NULL;
		pNode = pMemberIDAndInstrumentHashIndex->searchNextEqual(pNode))
	{
		if (((CTradingRight *)(pNode->pObject))->HashMemberIDAndInstrument != compareObject.HashMemberIDAndInstrument)
			continue;
		if (compare(((CTradingRight *)(pNode->pObject))->MemberID.getValue(), MemberID.getValue()) != 0)
			continue;
		if (compare(((CTradingRight *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		if (compare(((CTradingRight *)(pNode->pObject))->Direction.getValue(), Direction.getValue()) != 0)
			continue;
		return (CTradingRight *)(pNode->pObject);
	}
	return NULL;
}

CTradingRight *CTradingRightFactory::findByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyDirectionType& Direction)
{
	return findByMemberIDAndInstrument(MemberID, InstrumentID, Direction);
}

bool CTradingRightFactory::removeByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyDirectionType& Direction, CTransaction *pTransaction)
{
	CTradingRight *pOriginalTradingRight;
	pOriginalTradingRight = findByMemberIDAndInstrument(MemberID, InstrumentID, Direction);
	if (pOriginalTradingRight == NULL)
		return false;
	remove(pOriginalTradingRight, pTransaction);
	return true;
}

CTradingRight *CTradingRightFactory::findByPK(CTradingRightField *pTradingRight)
{
	return findByPK(pTradingRight->MemberID, pTradingRight->InstrumentID, pTradingRight->Direction);
}

CTradingRight *CTradingRightFactory::findByPK(CTradingRight *pTradingRight)
{
	return findByPK(pTradingRight->MemberID, pTradingRight->InstrumentID, pTradingRight->Direction);
}

bool CTradingRightFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "MemberID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	if (!strcmp(pName, "Direction"))
		return true;
	return false;
}

bool CTradingRightFactory::retrieveByPK(CWriteableTradingRight *pTradingRight)
{
	CTradingRight *pOriginalTradingRight;
	pOriginalTradingRight = findByPK(pTradingRight);
	if (pOriginalTradingRight == NULL)
		return false;
	retrieve(pOriginalTradingRight, pTradingRight);
	return true;
}

CTradingRight *CTradingRightFactory::addByPK(CWriteableTradingRight *pTradingRight, CTransaction *pTransaction)
{
	CTradingRight *pOriginalTradingRight;
	pOriginalTradingRight = findByPK(pTradingRight);
	if (pOriginalTradingRight != NULL)
		return NULL;
	return add(pTradingRight, pTransaction);
}

bool CTradingRightFactory::updateByPK(CWriteableTradingRight *pTradingRight, CTransaction *pTransaction)
{
	CTradingRight *pOriginalTradingRight;
	pOriginalTradingRight = findByPK(pTradingRight);
	if (pOriginalTradingRight == NULL)
		return false;
	pTradingRight->calAllHash();
	update(pOriginalTradingRight, pTradingRight, pTransaction);
	return true;
}

CTradingRight* CTradingRightFactory::addOrUpdateByPK(CWriteableTradingRight *pTradingRight, CTransaction *pTransaction)
{
	CTradingRight *pOriginalTradingRight;
	pOriginalTradingRight = findByPK(pTradingRight);
	if (pOriginalTradingRight == NULL)
		return add(pTradingRight, pTransaction);
	pTradingRight->calAllHash();
	update(pOriginalTradingRight, pTradingRight, pTransaction);
	return pOriginalTradingRight;
}

bool CTradingRightFactory::removeByPK(CTradingRight *pTradingRight, CTransaction *pTransaction)
{
	CTradingRight *pOriginalTradingRight;
	pOriginalTradingRight = findByPK(pTradingRight);
	if (pOriginalTradingRight == NULL)
		return false;
	remove(pOriginalTradingRight, pTransaction);
	return true;
}

bool CTradingRightFactory::removeByPK(CTradingRightField *pTradingRight, CTransaction *pTransaction)
{
	CTradingRight *pOriginalTradingRight;
	pOriginalTradingRight = findByPK(pTradingRight);
	if (pOriginalTradingRight == NULL)
		return false;
	CopyTradingRightEntity(pTradingRight, pOriginalTradingRight);
	remove(pOriginalTradingRight, pTransaction);
	return true;
}
	
CMarginRate* CMarginRateFactory::findByMemberAndInstrument(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID)
{	
	CHashIndexNode *pNode;
	compareObject.MemberID = MemberID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();

	compareObject.calHashMemberAndInstrument();
	for (pNode = pMemberAndInstrumentHashIndex->searchFirstEqual(compareObject.HashMemberAndInstrument);
		pNode != NULL;
		pNode = pMemberAndInstrumentHashIndex->searchNextEqual(pNode))
	{
		if (((CMarginRate *)(pNode->pObject))->HashMemberAndInstrument != compareObject.HashMemberAndInstrument)
			continue;
		if (compare(((CMarginRate *)(pNode->pObject))->MemberID.getValue(), MemberID.getValue()) != 0)
			continue;
		if (compare(((CMarginRate *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		return (CMarginRate *)(pNode->pObject);
	}
	return NULL;
}

CMarginRate *CMarginRateFactory::findByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	return findByMemberAndInstrument(MemberID, InstrumentID);
}

bool CMarginRateFactory::removeByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, CTransaction *pTransaction)
{
	CMarginRate *pOriginalMarginRate;
	pOriginalMarginRate = findByMemberAndInstrument(MemberID, InstrumentID);
	if (pOriginalMarginRate == NULL)
		return false;
	remove(pOriginalMarginRate, pTransaction);
	return true;
}

CMarginRate *CMarginRateFactory::findByPK(CMarginRateField *pMarginRate)
{
	return findByPK(pMarginRate->MemberID, pMarginRate->InstrumentID);
}

CMarginRate *CMarginRateFactory::findByPK(CMarginRate *pMarginRate)
{
	return findByPK(pMarginRate->MemberID, pMarginRate->InstrumentID);
}

bool CMarginRateFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "MemberID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	return false;
}

bool CMarginRateFactory::retrieveByPK(CWriteableMarginRate *pMarginRate)
{
	CMarginRate *pOriginalMarginRate;
	pOriginalMarginRate = findByPK(pMarginRate);
	if (pOriginalMarginRate == NULL)
		return false;
	retrieve(pOriginalMarginRate, pMarginRate);
	return true;
}

CMarginRate *CMarginRateFactory::addByPK(CWriteableMarginRate *pMarginRate, CTransaction *pTransaction)
{
	CMarginRate *pOriginalMarginRate;
	pOriginalMarginRate = findByPK(pMarginRate);
	if (pOriginalMarginRate != NULL)
		return NULL;
	return add(pMarginRate, pTransaction);
}

bool CMarginRateFactory::updateByPK(CWriteableMarginRate *pMarginRate, CTransaction *pTransaction)
{
	CMarginRate *pOriginalMarginRate;
	pOriginalMarginRate = findByPK(pMarginRate);
	if (pOriginalMarginRate == NULL)
		return false;
	pMarginRate->calAllHash();
	update(pOriginalMarginRate, pMarginRate, pTransaction);
	return true;
}

CMarginRate* CMarginRateFactory::addOrUpdateByPK(CWriteableMarginRate *pMarginRate, CTransaction *pTransaction)
{
	CMarginRate *pOriginalMarginRate;
	pOriginalMarginRate = findByPK(pMarginRate);
	if (pOriginalMarginRate == NULL)
		return add(pMarginRate, pTransaction);
	pMarginRate->calAllHash();
	update(pOriginalMarginRate, pMarginRate, pTransaction);
	return pOriginalMarginRate;
}

bool CMarginRateFactory::removeByPK(CMarginRate *pMarginRate, CTransaction *pTransaction)
{
	CMarginRate *pOriginalMarginRate;
	pOriginalMarginRate = findByPK(pMarginRate);
	if (pOriginalMarginRate == NULL)
		return false;
	remove(pOriginalMarginRate, pTransaction);
	return true;
}

bool CMarginRateFactory::removeByPK(CMarginRateField *pMarginRate, CTransaction *pTransaction)
{
	CMarginRate *pOriginalMarginRate;
	pOriginalMarginRate = findByPK(pMarginRate);
	if (pOriginalMarginRate == NULL)
		return false;
	CopyMarginRateEntity(pMarginRate, pOriginalMarginRate);
	remove(pOriginalMarginRate, pTransaction);
	return true;
}
	
CCFDRate* CCFDRateFactory::findByMemberAndInstrument(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyLongIndexIDType& TriggerOrderType)
{	
	CHashIndexNode *pNode;
	compareObject.MemberID = MemberID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.TriggerOrderType = TriggerOrderType.getValue();

	compareObject.calHashMemberAndInstrument();
	for (pNode = pMemberAndInstrumentHashIndex->searchFirstEqual(compareObject.HashMemberAndInstrument);
		pNode != NULL;
		pNode = pMemberAndInstrumentHashIndex->searchNextEqual(pNode))
	{
		if (((CCFDRate *)(pNode->pObject))->HashMemberAndInstrument != compareObject.HashMemberAndInstrument)
			continue;
		if (compare(((CCFDRate *)(pNode->pObject))->MemberID.getValue(), MemberID.getValue()) != 0)
			continue;
		if (compare(((CCFDRate *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		if (compare(((CCFDRate *)(pNode->pObject))->TriggerOrderType.getValue(), TriggerOrderType.getValue()) != 0)
			continue;
		return (CCFDRate *)(pNode->pObject);
	}
	return NULL;
}

CCFDRate *CCFDRateFactory::findByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyLongIndexIDType& TriggerOrderType)
{
	return findByMemberAndInstrument(MemberID, InstrumentID, TriggerOrderType);
}

bool CCFDRateFactory::removeByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyLongIndexIDType& TriggerOrderType, CTransaction *pTransaction)
{
	CCFDRate *pOriginalCFDRate;
	pOriginalCFDRate = findByMemberAndInstrument(MemberID, InstrumentID, TriggerOrderType);
	if (pOriginalCFDRate == NULL)
		return false;
	remove(pOriginalCFDRate, pTransaction);
	return true;
}

CCFDRate *CCFDRateFactory::findByPK(CCFDRateField *pCFDRate)
{
	return findByPK(pCFDRate->MemberID, pCFDRate->InstrumentID, pCFDRate->TriggerOrderType);
}

CCFDRate *CCFDRateFactory::findByPK(CCFDRate *pCFDRate)
{
	return findByPK(pCFDRate->MemberID, pCFDRate->InstrumentID, pCFDRate->TriggerOrderType);
}

bool CCFDRateFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "MemberID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	if (!strcmp(pName, "TriggerOrderType"))
		return true;
	return false;
}

bool CCFDRateFactory::retrieveByPK(CWriteableCFDRate *pCFDRate)
{
	CCFDRate *pOriginalCFDRate;
	pOriginalCFDRate = findByPK(pCFDRate);
	if (pOriginalCFDRate == NULL)
		return false;
	retrieve(pOriginalCFDRate, pCFDRate);
	return true;
}

CCFDRate *CCFDRateFactory::addByPK(CWriteableCFDRate *pCFDRate, CTransaction *pTransaction)
{
	CCFDRate *pOriginalCFDRate;
	pOriginalCFDRate = findByPK(pCFDRate);
	if (pOriginalCFDRate != NULL)
		return NULL;
	return add(pCFDRate, pTransaction);
}

bool CCFDRateFactory::updateByPK(CWriteableCFDRate *pCFDRate, CTransaction *pTransaction)
{
	CCFDRate *pOriginalCFDRate;
	pOriginalCFDRate = findByPK(pCFDRate);
	if (pOriginalCFDRate == NULL)
		return false;
	pCFDRate->calAllHash();
	update(pOriginalCFDRate, pCFDRate, pTransaction);
	return true;
}

CCFDRate* CCFDRateFactory::addOrUpdateByPK(CWriteableCFDRate *pCFDRate, CTransaction *pTransaction)
{
	CCFDRate *pOriginalCFDRate;
	pOriginalCFDRate = findByPK(pCFDRate);
	if (pOriginalCFDRate == NULL)
		return add(pCFDRate, pTransaction);
	pCFDRate->calAllHash();
	update(pOriginalCFDRate, pCFDRate, pTransaction);
	return pOriginalCFDRate;
}

bool CCFDRateFactory::removeByPK(CCFDRate *pCFDRate, CTransaction *pTransaction)
{
	CCFDRate *pOriginalCFDRate;
	pOriginalCFDRate = findByPK(pCFDRate);
	if (pOriginalCFDRate == NULL)
		return false;
	remove(pOriginalCFDRate, pTransaction);
	return true;
}

bool CCFDRateFactory::removeByPK(CCFDRateField *pCFDRate, CTransaction *pTransaction)
{
	CCFDRate *pOriginalCFDRate;
	pOriginalCFDRate = findByPK(pCFDRate);
	if (pOriginalCFDRate == NULL)
		return false;
	CopyCFDRateEntity(pCFDRate, pOriginalCFDRate);
	remove(pOriginalCFDRate, pTransaction);
	return true;
}
	
CFee* CFeeFactory::findByMemberIDAndInstrument(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyMatchRoleType& MatchRole)
{	
	CHashIndexNode *pNode;
	compareObject.MemberID = MemberID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.MatchRole = MatchRole.getValue();

	compareObject.calHashMemberIDAndInstrument();
	for (pNode = pMemberIDAndInstrumentHashIndex->searchFirstEqual(compareObject.HashMemberIDAndInstrument);
		pNode != NULL;
		pNode = pMemberIDAndInstrumentHashIndex->searchNextEqual(pNode))
	{
		if (((CFee *)(pNode->pObject))->HashMemberIDAndInstrument != compareObject.HashMemberIDAndInstrument)
			continue;
		if (compare(((CFee *)(pNode->pObject))->MemberID.getValue(), MemberID.getValue()) != 0)
			continue;
		if (compare(((CFee *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		if (compare(((CFee *)(pNode->pObject))->MatchRole.getValue(), MatchRole.getValue()) != 0)
			continue;
		return (CFee *)(pNode->pObject);
	}
	return NULL;
}

CFee *CFeeFactory::findByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyMatchRoleType& MatchRole)
{
	return findByMemberIDAndInstrument(MemberID, InstrumentID, MatchRole);
}

bool CFeeFactory::removeByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyMatchRoleType& MatchRole, CTransaction *pTransaction)
{
	CFee *pOriginalFee;
	pOriginalFee = findByMemberIDAndInstrument(MemberID, InstrumentID, MatchRole);
	if (pOriginalFee == NULL)
		return false;
	remove(pOriginalFee, pTransaction);
	return true;
}

CFee *CFeeFactory::findByPK(CFeeField *pFee)
{
	return findByPK(pFee->MemberID, pFee->InstrumentID, pFee->MatchRole);
}

CFee *CFeeFactory::findByPK(CFee *pFee)
{
	return findByPK(pFee->MemberID, pFee->InstrumentID, pFee->MatchRole);
}

bool CFeeFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "MemberID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	if (!strcmp(pName, "MatchRole"))
		return true;
	return false;
}

bool CFeeFactory::retrieveByPK(CWriteableFee *pFee)
{
	CFee *pOriginalFee;
	pOriginalFee = findByPK(pFee);
	if (pOriginalFee == NULL)
		return false;
	retrieve(pOriginalFee, pFee);
	return true;
}

CFee *CFeeFactory::addByPK(CWriteableFee *pFee, CTransaction *pTransaction)
{
	CFee *pOriginalFee;
	pOriginalFee = findByPK(pFee);
	if (pOriginalFee != NULL)
		return NULL;
	return add(pFee, pTransaction);
}

bool CFeeFactory::updateByPK(CWriteableFee *pFee, CTransaction *pTransaction)
{
	CFee *pOriginalFee;
	pOriginalFee = findByPK(pFee);
	if (pOriginalFee == NULL)
		return false;
	pFee->calAllHash();
	update(pOriginalFee, pFee, pTransaction);
	return true;
}

CFee* CFeeFactory::addOrUpdateByPK(CWriteableFee *pFee, CTransaction *pTransaction)
{
	CFee *pOriginalFee;
	pOriginalFee = findByPK(pFee);
	if (pOriginalFee == NULL)
		return add(pFee, pTransaction);
	pFee->calAllHash();
	update(pOriginalFee, pFee, pTransaction);
	return pOriginalFee;
}

bool CFeeFactory::removeByPK(CFee *pFee, CTransaction *pTransaction)
{
	CFee *pOriginalFee;
	pOriginalFee = findByPK(pFee);
	if (pOriginalFee == NULL)
		return false;
	remove(pOriginalFee, pTransaction);
	return true;
}

bool CFeeFactory::removeByPK(CFeeField *pFee, CTransaction *pTransaction)
{
	CFee *pOriginalFee;
	pOriginalFee = findByPK(pFee);
	if (pOriginalFee == NULL)
		return false;
	CopyFeeEntity(pFee, pOriginalFee);
	remove(pOriginalFee, pTransaction);
	return true;
}
int compareForSettlementGroupIndexinCurrency(const void *pV1, const void *pV2)
{
	CCurrency *p1, *p2;
	p1 = (CCurrency *)pV1;
	p2 = (CCurrency *)pV2;
	switch (compare(p1->SettlementGroup.getValue(), p2->SettlementGroup.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CCurrency* CCurrencyFactory::findByCurrency(const CReadOnlyCurrencyType& Currency, const CReadOnlyShortIndexIDType& SettlementGroup)
{	
	CHashIndexNode *pNode;
	compareObject.Currency = Currency.getValue();
	compareObject.SettlementGroup = SettlementGroup.getValue();

	compareObject.calHashCurrency();
	for (pNode = pCurrencyHashIndex->searchFirstEqual(compareObject.HashCurrency);
		pNode != NULL;
		pNode = pCurrencyHashIndex->searchNextEqual(pNode))
	{
		if (((CCurrency *)(pNode->pObject))->HashCurrency != compareObject.HashCurrency)
			continue;
		if (compare(((CCurrency *)(pNode->pObject))->Currency.getValue(), Currency.getValue()) != 0)
			continue;
		if (compare(((CCurrency *)(pNode->pObject))->SettlementGroup.getValue(), SettlementGroup.getValue()) != 0)
			continue;
		return (CCurrency *)(pNode->pObject);
	}
	return NULL;
}

CCurrency *CCurrencyFactory::findByPK(const CReadOnlyCurrencyType& Currency, const CReadOnlyShortIndexIDType& SettlementGroup)
{
	return findByCurrency(Currency, SettlementGroup);
}

bool CCurrencyFactory::removeByPK(const CReadOnlyCurrencyType& Currency, const CReadOnlyShortIndexIDType& SettlementGroup, CTransaction *pTransaction)
{
	CCurrency *pOriginalCurrency;
	pOriginalCurrency = findByCurrency(Currency, SettlementGroup);
	if (pOriginalCurrency == NULL)
		return false;
	remove(pOriginalCurrency, pTransaction);
	return true;
}

CCurrency *CCurrencyFactory::findByPK(CCurrencyField *pCurrency)
{
	return findByPK(pCurrency->Currency, pCurrency->SettlementGroup);
}

CCurrency *CCurrencyFactory::findByPK(CCurrency *pCurrency)
{
	return findByPK(pCurrency->Currency, pCurrency->SettlementGroup);
}

bool CCurrencyFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "Currency"))
		return true;
	if (!strcmp(pName, "SettlementGroup"))
		return true;
	return false;
}

bool CCurrencyFactory::retrieveByPK(CWriteableCurrency *pCurrency)
{
	CCurrency *pOriginalCurrency;
	pOriginalCurrency = findByPK(pCurrency);
	if (pOriginalCurrency == NULL)
		return false;
	retrieve(pOriginalCurrency, pCurrency);
	return true;
}

CCurrency *CCurrencyFactory::addByPK(CWriteableCurrency *pCurrency, CTransaction *pTransaction)
{
	CCurrency *pOriginalCurrency;
	pOriginalCurrency = findByPK(pCurrency);
	if (pOriginalCurrency != NULL)
		return NULL;
	return add(pCurrency, pTransaction);
}

bool CCurrencyFactory::updateByPK(CWriteableCurrency *pCurrency, CTransaction *pTransaction)
{
	CCurrency *pOriginalCurrency;
	pOriginalCurrency = findByPK(pCurrency);
	if (pOriginalCurrency == NULL)
		return false;
	pCurrency->calAllHash();
	update(pOriginalCurrency, pCurrency, pTransaction);
	return true;
}

CCurrency* CCurrencyFactory::addOrUpdateByPK(CWriteableCurrency *pCurrency, CTransaction *pTransaction)
{
	CCurrency *pOriginalCurrency;
	pOriginalCurrency = findByPK(pCurrency);
	if (pOriginalCurrency == NULL)
		return add(pCurrency, pTransaction);
	pCurrency->calAllHash();
	update(pOriginalCurrency, pCurrency, pTransaction);
	return pOriginalCurrency;
}

bool CCurrencyFactory::removeByPK(CCurrency *pCurrency, CTransaction *pTransaction)
{
	CCurrency *pOriginalCurrency;
	pOriginalCurrency = findByPK(pCurrency);
	if (pOriginalCurrency == NULL)
		return false;
	remove(pOriginalCurrency, pTransaction);
	return true;
}

bool CCurrencyFactory::removeByPK(CCurrencyField *pCurrency, CTransaction *pTransaction)
{
	CCurrency *pOriginalCurrency;
	pOriginalCurrency = findByPK(pCurrency);
	if (pOriginalCurrency == NULL)
		return false;
	CopyCurrencyEntity(pCurrency, pOriginalCurrency);
	remove(pOriginalCurrency, pTransaction);
	return true;
}



CCurrency* CCurrencyFactory::findFirstBySettlementGroup(const CReadOnlyShortIndexIDType& SettlementGroup)
{
	pLastFoundInSearchBySettlementGroup = NULL;
	///记录所有的查询值
	querySettlementGroupInSearchBySettlementGroup = SettlementGroup.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CCurrency *theCurrency;
	compareObject.SettlementGroup = SettlementGroup.getValue();

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchBySettlementGroup = pSettlementGroupIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchBySettlementGroup == NULL)
		return NULL;

	///获取找到的对象
	theCurrency = (CCurrency *)(pLastFoundInSearchBySettlementGroup->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theCurrency->SettlementGroup.getValue(), SettlementGroup.getValue()))
	{
		pLastFoundInSearchBySettlementGroup = NULL;
		return NULL;
	}

	return theCurrency;
}

CCurrency *CCurrencyFactory::findNextBySettlementGroup()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchBySettlementGroup == NULL)
		return NULL;

	CCurrency *theCurrency;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchBySettlementGroup = pSettlementGroupIndex->getNextNode(pLastFoundInSearchBySettlementGroup);
	if (pLastFoundInSearchBySettlementGroup == NULL)
		return NULL;

	theCurrency = (CCurrency *)(pLastFoundInSearchBySettlementGroup->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theCurrency->SettlementGroup, querySettlementGroupInSearchBySettlementGroup))
	{
		pLastFoundInSearchBySettlementGroup = NULL;
		return NULL;
	}

	///返回找到的结果
	return theCurrency;
}

	
CLiquidity* CLiquidityFactory::findByLiquidity(const CReadOnlyShortIndexIDType& LiquidityGrade, const CReadOnlyLongIndexIDType& LiquidityGroup)
{	
	CHashIndexNode *pNode;
	compareObject.LiquidityGrade = LiquidityGrade.getValue();
	compareObject.LiquidityGroup = LiquidityGroup.getValue();

	compareObject.calHashLiquidity();
	for (pNode = pLiquidityHashIndex->searchFirstEqual(compareObject.HashLiquidity);
		pNode != NULL;
		pNode = pLiquidityHashIndex->searchNextEqual(pNode))
	{
		if (((CLiquidity *)(pNode->pObject))->HashLiquidity != compareObject.HashLiquidity)
			continue;
		if (compare(((CLiquidity *)(pNode->pObject))->LiquidityGrade.getValue(), LiquidityGrade.getValue()) != 0)
			continue;
		if (compare(((CLiquidity *)(pNode->pObject))->LiquidityGroup.getValue(), LiquidityGroup.getValue()) != 0)
			continue;
		return (CLiquidity *)(pNode->pObject);
	}
	return NULL;
}

CLiquidity *CLiquidityFactory::findByPK(const CReadOnlyShortIndexIDType& LiquidityGrade, const CReadOnlyLongIndexIDType& LiquidityGroup)
{
	return findByLiquidity(LiquidityGrade, LiquidityGroup);
}

bool CLiquidityFactory::removeByPK(const CReadOnlyShortIndexIDType& LiquidityGrade, const CReadOnlyLongIndexIDType& LiquidityGroup, CTransaction *pTransaction)
{
	CLiquidity *pOriginalLiquidity;
	pOriginalLiquidity = findByLiquidity(LiquidityGrade, LiquidityGroup);
	if (pOriginalLiquidity == NULL)
		return false;
	remove(pOriginalLiquidity, pTransaction);
	return true;
}

CLiquidity *CLiquidityFactory::findByPK(CLiquidityField *pLiquidity)
{
	return findByPK(pLiquidity->LiquidityGrade, pLiquidity->LiquidityGroup);
}

CLiquidity *CLiquidityFactory::findByPK(CLiquidity *pLiquidity)
{
	return findByPK(pLiquidity->LiquidityGrade, pLiquidity->LiquidityGroup);
}

bool CLiquidityFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "LiquidityGrade"))
		return true;
	if (!strcmp(pName, "LiquidityGroup"))
		return true;
	return false;
}

bool CLiquidityFactory::retrieveByPK(CWriteableLiquidity *pLiquidity)
{
	CLiquidity *pOriginalLiquidity;
	pOriginalLiquidity = findByPK(pLiquidity);
	if (pOriginalLiquidity == NULL)
		return false;
	retrieve(pOriginalLiquidity, pLiquidity);
	return true;
}

CLiquidity *CLiquidityFactory::addByPK(CWriteableLiquidity *pLiquidity, CTransaction *pTransaction)
{
	CLiquidity *pOriginalLiquidity;
	pOriginalLiquidity = findByPK(pLiquidity);
	if (pOriginalLiquidity != NULL)
		return NULL;
	return add(pLiquidity, pTransaction);
}

bool CLiquidityFactory::updateByPK(CWriteableLiquidity *pLiquidity, CTransaction *pTransaction)
{
	CLiquidity *pOriginalLiquidity;
	pOriginalLiquidity = findByPK(pLiquidity);
	if (pOriginalLiquidity == NULL)
		return false;
	pLiquidity->calAllHash();
	update(pOriginalLiquidity, pLiquidity, pTransaction);
	return true;
}

CLiquidity* CLiquidityFactory::addOrUpdateByPK(CWriteableLiquidity *pLiquidity, CTransaction *pTransaction)
{
	CLiquidity *pOriginalLiquidity;
	pOriginalLiquidity = findByPK(pLiquidity);
	if (pOriginalLiquidity == NULL)
		return add(pLiquidity, pTransaction);
	pLiquidity->calAllHash();
	update(pOriginalLiquidity, pLiquidity, pTransaction);
	return pOriginalLiquidity;
}

bool CLiquidityFactory::removeByPK(CLiquidity *pLiquidity, CTransaction *pTransaction)
{
	CLiquidity *pOriginalLiquidity;
	pOriginalLiquidity = findByPK(pLiquidity);
	if (pOriginalLiquidity == NULL)
		return false;
	remove(pOriginalLiquidity, pTransaction);
	return true;
}

bool CLiquidityFactory::removeByPK(CLiquidityField *pLiquidity, CTransaction *pTransaction)
{
	CLiquidity *pOriginalLiquidity;
	pOriginalLiquidity = findByPK(pLiquidity);
	if (pOriginalLiquidity == NULL)
		return false;
	CopyLiquidityEntity(pLiquidity, pOriginalLiquidity);
	remove(pOriginalLiquidity, pTransaction);
	return true;
}
int compareForMemberIndexinAccount(const void *pV1, const void *pV2)
{
	CAccount *p1, *p2;
	p1 = (CAccount *)pV1;
	p2 = (CAccount *)pV2;
	switch (compare(p1->MemberID.getValue(), p2->MemberID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->AccountID.getValue(), p2->AccountID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->SettlementGroup.getValue(), p2->SettlementGroup.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Currency.getValue(), p2->Currency.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForCurrencyIndexinAccount(const void *pV1, const void *pV2)
{
	CAccount *p1, *p2;
	p1 = (CAccount *)pV1;
	p2 = (CAccount *)pV2;
	switch (compare(p1->SettlementGroup.getValue(), p2->SettlementGroup.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Currency.getValue(), p2->Currency.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Balance.getValue(), p2->Balance.getValue()))
	{
	case 1:
		return -1;
	case -1:
		return 1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CAccount* CAccountFactory::findByAccountIDAndCurrency(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID, const CReadOnlyShortIndexIDType& SettlementGroup, const CReadOnlyCurrencyType& Currency)
{	
	CAVLNode *pNode;
	compareObject.MemberID = MemberID.getValue();
	compareObject.AccountID = AccountID.getValue();
	compareObject.SettlementGroup = SettlementGroup.getValue();
	compareObject.Currency = Currency.getValue();
	pNode = pMemberIndex->searchFirstEqual(&compareObject);
	if (pNode == NULL)
		return NULL;
	return (CAccount *)(pNode->pObject);
}

CAccount *CAccountFactory::findByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID, const CReadOnlyShortIndexIDType& SettlementGroup, const CReadOnlyCurrencyType& Currency)
{
	return findByAccountIDAndCurrency(MemberID, AccountID, SettlementGroup, Currency);
}

bool CAccountFactory::removeByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID, const CReadOnlyShortIndexIDType& SettlementGroup, const CReadOnlyCurrencyType& Currency, CTransaction *pTransaction)
{
	CAccount *pOriginalAccount;
	pOriginalAccount = findByAccountIDAndCurrency(MemberID, AccountID, SettlementGroup, Currency);
	if (pOriginalAccount == NULL)
		return false;
	remove(pOriginalAccount, pTransaction);
	return true;
}

CAccount *CAccountFactory::findByPK(CAccountField *pAccount)
{
	return findByPK(pAccount->MemberID, pAccount->AccountID, pAccount->SettlementGroup, pAccount->Currency);
}

CAccount *CAccountFactory::findByPK(CAccount *pAccount)
{
	return findByPK(pAccount->MemberID, pAccount->AccountID, pAccount->SettlementGroup, pAccount->Currency);
}

bool CAccountFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "MemberID"))
		return true;
	if (!strcmp(pName, "AccountID"))
		return true;
	if (!strcmp(pName, "SettlementGroup"))
		return true;
	if (!strcmp(pName, "Currency"))
		return true;
	return false;
}

bool CAccountFactory::retrieveByPK(CWriteableAccount *pAccount)
{
	CAccount *pOriginalAccount;
	pOriginalAccount = findByPK(pAccount);
	if (pOriginalAccount == NULL)
		return false;
	retrieve(pOriginalAccount, pAccount);
	return true;
}

CAccount *CAccountFactory::addByPK(CWriteableAccount *pAccount, CTransaction *pTransaction)
{
	CAccount *pOriginalAccount;
	pOriginalAccount = findByPK(pAccount);
	if (pOriginalAccount != NULL)
		return NULL;
	return add(pAccount, pTransaction);
}

bool CAccountFactory::updateByPK(CWriteableAccount *pAccount, CTransaction *pTransaction)
{
	CAccount *pOriginalAccount;
	pOriginalAccount = findByPK(pAccount);
	if (pOriginalAccount == NULL)
		return false;
	pAccount->calAllHash();
	update(pOriginalAccount, pAccount, pTransaction);
	return true;
}

CAccount* CAccountFactory::addOrUpdateByPK(CWriteableAccount *pAccount, CTransaction *pTransaction)
{
	CAccount *pOriginalAccount;
	pOriginalAccount = findByPK(pAccount);
	if (pOriginalAccount == NULL)
		return add(pAccount, pTransaction);
	pAccount->calAllHash();
	update(pOriginalAccount, pAccount, pTransaction);
	return pOriginalAccount;
}

bool CAccountFactory::removeByPK(CAccount *pAccount, CTransaction *pTransaction)
{
	CAccount *pOriginalAccount;
	pOriginalAccount = findByPK(pAccount);
	if (pOriginalAccount == NULL)
		return false;
	remove(pOriginalAccount, pTransaction);
	return true;
}

bool CAccountFactory::removeByPK(CAccountField *pAccount, CTransaction *pTransaction)
{
	CAccount *pOriginalAccount;
	pOriginalAccount = findByPK(pAccount);
	if (pOriginalAccount == NULL)
		return false;
	CopyAccountEntity(pAccount, pOriginalAccount);
	remove(pOriginalAccount, pTransaction);
	return true;
}



CAccount* CAccountFactory::findFirstByMemberID(const CReadOnlyLongIndexIDType& MemberID)
{
	pLastFoundInSearchByMemberID = NULL;
	///记录所有的查询值
	queryMemberIDInSearchByMemberID = MemberID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CAccount *theAccount;
	compareObject.MemberID = MemberID.getValue();
	compareObject.AccountID = "";
	compareObject.SettlementGroup = "";
	compareObject.Currency = "";

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByMemberID = pMemberIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByMemberID == NULL)
		return NULL;

	///获取找到的对象
	theAccount = (CAccount *)(pLastFoundInSearchByMemberID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theAccount->MemberID.getValue(), MemberID.getValue()))
	{
		pLastFoundInSearchByMemberID = NULL;
		return NULL;
	}

	return theAccount;
}

CAccount *CAccountFactory::findNextByMemberID()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByMemberID == NULL)
		return NULL;

	CAccount *theAccount;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByMemberID = pMemberIndex->getNextNode(pLastFoundInSearchByMemberID);
	if (pLastFoundInSearchByMemberID == NULL)
		return NULL;

	theAccount = (CAccount *)(pLastFoundInSearchByMemberID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theAccount->MemberID, queryMemberIDInSearchByMemberID))
	{
		pLastFoundInSearchByMemberID = NULL;
		return NULL;
	}

	///返回找到的结果
	return theAccount;
}




CAccount* CAccountFactory::findFirstByAccountID(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID)
{
	pLastFoundInSearchByAccountID = NULL;
	///记录所有的查询值
	queryMemberIDInSearchByAccountID = MemberID.getValue();
	queryAccountIDInSearchByAccountID = AccountID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CAccount *theAccount;
	compareObject.MemberID = MemberID.getValue();
	compareObject.AccountID = AccountID.getValue();
	compareObject.SettlementGroup = "";
	compareObject.Currency = "";

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByAccountID = pMemberIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	///获取找到的对象
	theAccount = (CAccount *)(pLastFoundInSearchByAccountID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theAccount->MemberID.getValue(), MemberID.getValue()))
	{
		pLastFoundInSearchByAccountID = NULL;
		return NULL;
	}
	if (compare(theAccount->AccountID.getValue(), AccountID.getValue()))
	{
		pLastFoundInSearchByAccountID = NULL;
		return NULL;
	}

	return theAccount;
}

CAccount *CAccountFactory::findNextByAccountID()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	CAccount *theAccount;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByAccountID = pMemberIndex->getNextNode(pLastFoundInSearchByAccountID);
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	theAccount = (CAccount *)(pLastFoundInSearchByAccountID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theAccount->MemberID, queryMemberIDInSearchByAccountID))
	{
		pLastFoundInSearchByAccountID = NULL;
		return NULL;
	}
	if (compare(theAccount->AccountID, queryAccountIDInSearchByAccountID))
	{
		pLastFoundInSearchByAccountID = NULL;
		return NULL;
	}

	///返回找到的结果
	return theAccount;
}




CAccount* CAccountFactory::findFirstBySettlementGroup(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID, const CReadOnlyShortIndexIDType& SettlementGroup)
{
	pLastFoundInSearchBySettlementGroup = NULL;
	///记录所有的查询值
	queryMemberIDInSearchBySettlementGroup = MemberID.getValue();
	queryAccountIDInSearchBySettlementGroup = AccountID.getValue();
	querySettlementGroupInSearchBySettlementGroup = SettlementGroup.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CAccount *theAccount;
	compareObject.MemberID = MemberID.getValue();
	compareObject.AccountID = AccountID.getValue();
	compareObject.SettlementGroup = SettlementGroup.getValue();
	compareObject.Currency = "";

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchBySettlementGroup = pMemberIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchBySettlementGroup == NULL)
		return NULL;

	///获取找到的对象
	theAccount = (CAccount *)(pLastFoundInSearchBySettlementGroup->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theAccount->MemberID.getValue(), MemberID.getValue()))
	{
		pLastFoundInSearchBySettlementGroup = NULL;
		return NULL;
	}
	if (compare(theAccount->AccountID.getValue(), AccountID.getValue()))
	{
		pLastFoundInSearchBySettlementGroup = NULL;
		return NULL;
	}
	if (compare(theAccount->SettlementGroup.getValue(), SettlementGroup.getValue()))
	{
		pLastFoundInSearchBySettlementGroup = NULL;
		return NULL;
	}

	return theAccount;
}

CAccount *CAccountFactory::findNextBySettlementGroup()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchBySettlementGroup == NULL)
		return NULL;

	CAccount *theAccount;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchBySettlementGroup = pMemberIndex->getNextNode(pLastFoundInSearchBySettlementGroup);
	if (pLastFoundInSearchBySettlementGroup == NULL)
		return NULL;

	theAccount = (CAccount *)(pLastFoundInSearchBySettlementGroup->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theAccount->MemberID, queryMemberIDInSearchBySettlementGroup))
	{
		pLastFoundInSearchBySettlementGroup = NULL;
		return NULL;
	}
	if (compare(theAccount->AccountID, queryAccountIDInSearchBySettlementGroup))
	{
		pLastFoundInSearchBySettlementGroup = NULL;
		return NULL;
	}
	if (compare(theAccount->SettlementGroup, querySettlementGroupInSearchBySettlementGroup))
	{
		pLastFoundInSearchBySettlementGroup = NULL;
		return NULL;
	}

	///返回找到的结果
	return theAccount;
}




CAccount* CAccountFactory::findFirstByAll()
{
	pLastFoundInSearchByAll = NULL;
	///记录所有的查询值
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CAccount *theAccount;
	compareObject.MemberID = "";
	compareObject.AccountID = "";
	compareObject.SettlementGroup = "";
	compareObject.Currency = "";

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByAll = pMemberIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByAll == NULL)
		return NULL;

	///获取找到的对象
	theAccount = (CAccount *)(pLastFoundInSearchByAll->pObject);

	///如果有一个条件不满足，则失败返回

	return theAccount;
}

CAccount *CAccountFactory::findNextByAll()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByAll == NULL)
		return NULL;

	CAccount *theAccount;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByAll = pMemberIndex->getNextNode(pLastFoundInSearchByAll);
	if (pLastFoundInSearchByAll == NULL)
		return NULL;

	theAccount = (CAccount *)(pLastFoundInSearchByAll->pObject);

	///如果有一个条件不满足，则失败返回

	///返回找到的结果
	return theAccount;
}




CAccount* CAccountFactory::findFirstByCurrency(const CReadOnlyShortIndexIDType& SettlementGroup, const CReadOnlyCurrencyType& Currency)
{
	pLastFoundInSearchByCurrency = NULL;
	///记录所有的查询值
	querySettlementGroupInSearchByCurrency = SettlementGroup.getValue();
	queryCurrencyInSearchByCurrency = Currency.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CAccount *theAccount;
	compareObject.SettlementGroup = SettlementGroup.getValue();
	compareObject.Currency = Currency.getValue();
	compareObject.Balance = 999999999999.0;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByCurrency = pCurrencyIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByCurrency == NULL)
		return NULL;

	///获取找到的对象
	theAccount = (CAccount *)(pLastFoundInSearchByCurrency->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theAccount->SettlementGroup.getValue(), SettlementGroup.getValue()))
	{
		pLastFoundInSearchByCurrency = NULL;
		return NULL;
	}
	if (compare(theAccount->Currency.getValue(), Currency.getValue()))
	{
		pLastFoundInSearchByCurrency = NULL;
		return NULL;
	}

	return theAccount;
}

CAccount *CAccountFactory::findNextByCurrency()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByCurrency == NULL)
		return NULL;

	CAccount *theAccount;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByCurrency = pCurrencyIndex->getNextNode(pLastFoundInSearchByCurrency);
	if (pLastFoundInSearchByCurrency == NULL)
		return NULL;

	theAccount = (CAccount *)(pLastFoundInSearchByCurrency->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theAccount->SettlementGroup, querySettlementGroupInSearchByCurrency))
	{
		pLastFoundInSearchByCurrency = NULL;
		return NULL;
	}
	if (compare(theAccount->Currency, queryCurrencyInSearchByCurrency))
	{
		pLastFoundInSearchByCurrency = NULL;
		return NULL;
	}

	///返回找到的结果
	return theAccount;
}

	
CMemberPosition* CMemberPositionFactory::findByInstrument(const CReadOnlyAccountIDType& AccountID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{	
	CHashIndexNode *pNode;
	compareObject.AccountID = AccountID.getValue();
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();

	compareObject.calHashInstrument();
	for (pNode = pInstrumentHashIndex->searchFirstEqual(compareObject.HashInstrument);
		pNode != NULL;
		pNode = pInstrumentHashIndex->searchNextEqual(pNode))
	{
		if (((CMemberPosition *)(pNode->pObject))->HashInstrument != compareObject.HashInstrument)
			continue;
		if (compare(((CMemberPosition *)(pNode->pObject))->AccountID.getValue(), AccountID.getValue()) != 0)
			continue;
		if (compare(((CMemberPosition *)(pNode->pObject))->ExchangeID.getValue(), ExchangeID.getValue()) != 0)
			continue;
		if (compare(((CMemberPosition *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		return (CMemberPosition *)(pNode->pObject);
	}
	return NULL;
}

CMemberPosition *CMemberPositionFactory::findByPK(const CReadOnlyAccountIDType& AccountID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	return findByInstrument(AccountID, ExchangeID, InstrumentID);
}

bool CMemberPositionFactory::removeByPK(const CReadOnlyAccountIDType& AccountID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, CTransaction *pTransaction)
{
	CMemberPosition *pOriginalMemberPosition;
	pOriginalMemberPosition = findByInstrument(AccountID, ExchangeID, InstrumentID);
	if (pOriginalMemberPosition == NULL)
		return false;
	remove(pOriginalMemberPosition, pTransaction);
	return true;
}

CMemberPosition *CMemberPositionFactory::findByPK(CMemberPositionField *pMemberPosition)
{
	return findByPK(pMemberPosition->AccountID, pMemberPosition->ExchangeID, pMemberPosition->InstrumentID);
}

CMemberPosition *CMemberPositionFactory::findByPK(CMemberPosition *pMemberPosition)
{
	return findByPK(pMemberPosition->AccountID, pMemberPosition->ExchangeID, pMemberPosition->InstrumentID);
}

bool CMemberPositionFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "AccountID"))
		return true;
	if (!strcmp(pName, "ExchangeID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	return false;
}

bool CMemberPositionFactory::retrieveByPK(CWriteableMemberPosition *pMemberPosition)
{
	CMemberPosition *pOriginalMemberPosition;
	pOriginalMemberPosition = findByPK(pMemberPosition);
	if (pOriginalMemberPosition == NULL)
		return false;
	retrieve(pOriginalMemberPosition, pMemberPosition);
	return true;
}

CMemberPosition *CMemberPositionFactory::addByPK(CWriteableMemberPosition *pMemberPosition, CTransaction *pTransaction)
{
	CMemberPosition *pOriginalMemberPosition;
	pOriginalMemberPosition = findByPK(pMemberPosition);
	if (pOriginalMemberPosition != NULL)
		return NULL;
	return add(pMemberPosition, pTransaction);
}

bool CMemberPositionFactory::updateByPK(CWriteableMemberPosition *pMemberPosition, CTransaction *pTransaction)
{
	CMemberPosition *pOriginalMemberPosition;
	pOriginalMemberPosition = findByPK(pMemberPosition);
	if (pOriginalMemberPosition == NULL)
		return false;
	pMemberPosition->calAllHash();
	update(pOriginalMemberPosition, pMemberPosition, pTransaction);
	return true;
}

CMemberPosition* CMemberPositionFactory::addOrUpdateByPK(CWriteableMemberPosition *pMemberPosition, CTransaction *pTransaction)
{
	CMemberPosition *pOriginalMemberPosition;
	pOriginalMemberPosition = findByPK(pMemberPosition);
	if (pOriginalMemberPosition == NULL)
		return add(pMemberPosition, pTransaction);
	pMemberPosition->calAllHash();
	update(pOriginalMemberPosition, pMemberPosition, pTransaction);
	return pOriginalMemberPosition;
}

bool CMemberPositionFactory::removeByPK(CMemberPosition *pMemberPosition, CTransaction *pTransaction)
{
	CMemberPosition *pOriginalMemberPosition;
	pOriginalMemberPosition = findByPK(pMemberPosition);
	if (pOriginalMemberPosition == NULL)
		return false;
	remove(pOriginalMemberPosition, pTransaction);
	return true;
}

bool CMemberPositionFactory::removeByPK(CMemberPositionField *pMemberPosition, CTransaction *pTransaction)
{
	CMemberPosition *pOriginalMemberPosition;
	pOriginalMemberPosition = findByPK(pMemberPosition);
	if (pOriginalMemberPosition == NULL)
		return false;
	CopyMemberPositionEntity(pMemberPosition, pOriginalMemberPosition);
	remove(pOriginalMemberPosition, pTransaction);
	return true;
}
int compareForAccountIndexinPosition(const void *pV1, const void *pV2)
{
	CPosition *p1, *p2;
	p1 = (CPosition *)pV1;
	p2 = (CPosition *)pV2;
	switch (compare(p1->AccountID.getValue(), p2->AccountID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->TradeUnitID.getValue(), p2->TradeUnitID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->PosiDirection.getValue(), p2->PosiDirection.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForLiquidPriceLongIndexinPosition(const void *pV1, const void *pV2)
{
	CPosition *p1, *p2;
	p1 = (CPosition *)pV1;
	p2 = (CPosition *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->LiquidPrice.getValue(), p2->LiquidPrice.getValue()))
	{
	case 1:
		return -1;
	case -1:
		return 1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForLiquidPriceShortIndexinPosition(const void *pV1, const void *pV2)
{
	CPosition *p1, *p2;
	p1 = (CPosition *)pV1;
	p2 = (CPosition *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->LiquidPrice.getValue(), p2->LiquidPrice.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CPosition* CPositionFactory::findByPositionID(const CReadOnlyLongIndexIDType& PositionID)
{	
	CHashIndexNode *pNode;
	compareObject.PositionID = PositionID.getValue();

	compareObject.calHashPositionID();
	for (pNode = pPositionIDHashIndex->searchFirstEqual(compareObject.HashPositionID);
		pNode != NULL;
		pNode = pPositionIDHashIndex->searchNextEqual(pNode))
	{
		if (((CPosition *)(pNode->pObject))->HashPositionID != compareObject.HashPositionID)
			continue;
		if (compare(((CPosition *)(pNode->pObject))->PositionID.getValue(), PositionID.getValue()) != 0)
			continue;
		return (CPosition *)(pNode->pObject);
	}
	return NULL;
}

CPosition *CPositionFactory::findByPK(const CReadOnlyLongIndexIDType& PositionID)
{
	return findByPositionID(PositionID);
}

bool CPositionFactory::removeByPK(const CReadOnlyLongIndexIDType& PositionID, CTransaction *pTransaction)
{
	CPosition *pOriginalPosition;
	pOriginalPosition = findByPositionID(PositionID);
	if (pOriginalPosition == NULL)
		return false;
	remove(pOriginalPosition, pTransaction);
	return true;
}

CPosition *CPositionFactory::findByPK(CPositionField *pPosition)
{
	return findByPK(pPosition->PositionID);
}

CPosition *CPositionFactory::findByPK(CPosition *pPosition)
{
	return findByPK(pPosition->PositionID);
}

bool CPositionFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "PositionID"))
		return true;
	return false;
}

bool CPositionFactory::retrieveByPK(CWriteablePosition *pPosition)
{
	CPosition *pOriginalPosition;
	pOriginalPosition = findByPK(pPosition);
	if (pOriginalPosition == NULL)
		return false;
	retrieve(pOriginalPosition, pPosition);
	return true;
}

CPosition *CPositionFactory::addByPK(CWriteablePosition *pPosition, CTransaction *pTransaction)
{
	CPosition *pOriginalPosition;
	pOriginalPosition = findByPK(pPosition);
	if (pOriginalPosition != NULL)
		return NULL;
	return add(pPosition, pTransaction);
}

bool CPositionFactory::updateByPK(CWriteablePosition *pPosition, CTransaction *pTransaction)
{
	CPosition *pOriginalPosition;
	pOriginalPosition = findByPK(pPosition);
	if (pOriginalPosition == NULL)
		return false;
	pPosition->calAllHash();
	update(pOriginalPosition, pPosition, pTransaction);
	return true;
}

CPosition* CPositionFactory::addOrUpdateByPK(CWriteablePosition *pPosition, CTransaction *pTransaction)
{
	CPosition *pOriginalPosition;
	pOriginalPosition = findByPK(pPosition);
	if (pOriginalPosition == NULL)
		return add(pPosition, pTransaction);
	pPosition->calAllHash();
	update(pOriginalPosition, pPosition, pTransaction);
	return pOriginalPosition;
}

bool CPositionFactory::removeByPK(CPosition *pPosition, CTransaction *pTransaction)
{
	CPosition *pOriginalPosition;
	pOriginalPosition = findByPK(pPosition);
	if (pOriginalPosition == NULL)
		return false;
	remove(pOriginalPosition, pTransaction);
	return true;
}

bool CPositionFactory::removeByPK(CPositionField *pPosition, CTransaction *pTransaction)
{
	CPosition *pOriginalPosition;
	pOriginalPosition = findByPK(pPosition);
	if (pOriginalPosition == NULL)
		return false;
	CopyPositionEntity(pPosition, pOriginalPosition);
	remove(pOriginalPosition, pTransaction);
	return true;
}



CPosition* CPositionFactory::findFirstByAccountInstrument(const CReadOnlyAccountIDType& AccountID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	pLastFoundInSearchByAccountInstrument = NULL;
	///记录所有的查询值
	queryAccountIDInSearchByAccountInstrument = AccountID.getValue();
	queryExchangeIDInSearchByAccountInstrument = ExchangeID.getValue();
	queryInstrumentIDInSearchByAccountInstrument = InstrumentID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CPosition *thePosition;
	compareObject.AccountID = AccountID.getValue();
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.TradeUnitID = "";
	compareObject.PosiDirection = "";

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByAccountInstrument = pAccountIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByAccountInstrument == NULL)
		return NULL;

	///获取找到的对象
	thePosition = (CPosition *)(pLastFoundInSearchByAccountInstrument->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePosition->AccountID.getValue(), AccountID.getValue()))
	{
		pLastFoundInSearchByAccountInstrument = NULL;
		return NULL;
	}
	if (compare(thePosition->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchByAccountInstrument = NULL;
		return NULL;
	}
	if (compare(thePosition->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByAccountInstrument = NULL;
		return NULL;
	}

	return thePosition;
}

CPosition *CPositionFactory::findNextByAccountInstrument()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByAccountInstrument == NULL)
		return NULL;

	CPosition *thePosition;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByAccountInstrument = pAccountIndex->getNextNode(pLastFoundInSearchByAccountInstrument);
	if (pLastFoundInSearchByAccountInstrument == NULL)
		return NULL;

	thePosition = (CPosition *)(pLastFoundInSearchByAccountInstrument->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePosition->AccountID, queryAccountIDInSearchByAccountInstrument))
	{
		pLastFoundInSearchByAccountInstrument = NULL;
		return NULL;
	}
	if (compare(thePosition->ExchangeID, queryExchangeIDInSearchByAccountInstrument))
	{
		pLastFoundInSearchByAccountInstrument = NULL;
		return NULL;
	}
	if (compare(thePosition->InstrumentID, queryInstrumentIDInSearchByAccountInstrument))
	{
		pLastFoundInSearchByAccountInstrument = NULL;
		return NULL;
	}

	///返回找到的结果
	return thePosition;
}

CPosition* CPositionFactory::findByTradeUnitID(const CReadOnlyAccountIDType& AccountID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyIndexIDType& TradeUnitID, const CReadOnlyPosiDirectionType& PosiDirection)
{	
	CAVLNode *pNode;
	compareObject.AccountID = AccountID.getValue();
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.TradeUnitID = TradeUnitID.getValue();
	compareObject.PosiDirection = PosiDirection.getValue();
	pNode = pAccountIndex->searchFirstEqual(&compareObject);
	if (pNode == NULL)
		return NULL;
	return (CPosition *)(pNode->pObject);
}




CPosition* CPositionFactory::findFirstByAccountID(const CReadOnlyAccountIDType& AccountID)
{
	pLastFoundInSearchByAccountID = NULL;
	///记录所有的查询值
	queryAccountIDInSearchByAccountID = AccountID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CPosition *thePosition;
	compareObject.AccountID = AccountID.getValue();
	compareObject.ExchangeID = "";
	compareObject.InstrumentID = "";
	compareObject.TradeUnitID = "";
	compareObject.PosiDirection = "";

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByAccountID = pAccountIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	///获取找到的对象
	thePosition = (CPosition *)(pLastFoundInSearchByAccountID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePosition->AccountID.getValue(), AccountID.getValue()))
	{
		pLastFoundInSearchByAccountID = NULL;
		return NULL;
	}

	return thePosition;
}

CPosition *CPositionFactory::findNextByAccountID()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	CPosition *thePosition;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByAccountID = pAccountIndex->getNextNode(pLastFoundInSearchByAccountID);
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	thePosition = (CPosition *)(pLastFoundInSearchByAccountID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePosition->AccountID, queryAccountIDInSearchByAccountID))
	{
		pLastFoundInSearchByAccountID = NULL;
		return NULL;
	}

	///返回找到的结果
	return thePosition;
}




CPosition* CPositionFactory::findFirstByLiquidPriceLong(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& LiquidPrice)
{
	pLastFoundInSearchByLiquidPriceLong = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchByLiquidPriceLong = ExchangeID.getValue();
	queryInstrumentIDInSearchByLiquidPriceLong = InstrumentID.getValue();
	queryLiquidPriceInSearchByLiquidPriceLong = LiquidPrice.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CPosition *thePosition;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.LiquidPrice = LiquidPrice.getValue();

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByLiquidPriceLong = pLiquidPriceLongIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByLiquidPriceLong == NULL)
		return NULL;

	///获取找到的对象
	thePosition = (CPosition *)(pLastFoundInSearchByLiquidPriceLong->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePosition->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchByLiquidPriceLong = NULL;
		return NULL;
	}
	if (compare(thePosition->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByLiquidPriceLong = NULL;
		return NULL;
	}

	return thePosition;
}

CPosition *CPositionFactory::findNextByLiquidPriceLong()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByLiquidPriceLong == NULL)
		return NULL;

	CPosition *thePosition;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByLiquidPriceLong = pLiquidPriceLongIndex->getNextNode(pLastFoundInSearchByLiquidPriceLong);
	if (pLastFoundInSearchByLiquidPriceLong == NULL)
		return NULL;

	thePosition = (CPosition *)(pLastFoundInSearchByLiquidPriceLong->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePosition->ExchangeID, queryExchangeIDInSearchByLiquidPriceLong))
	{
		pLastFoundInSearchByLiquidPriceLong = NULL;
		return NULL;
	}
	if (compare(thePosition->InstrumentID, queryInstrumentIDInSearchByLiquidPriceLong))
	{
		pLastFoundInSearchByLiquidPriceLong = NULL;
		return NULL;
	}

	///返回找到的结果
	return thePosition;
}




CPosition* CPositionFactory::findFirstByLiquidPriceShort(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& LiquidPrice)
{
	pLastFoundInSearchByLiquidPriceShort = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchByLiquidPriceShort = ExchangeID.getValue();
	queryInstrumentIDInSearchByLiquidPriceShort = InstrumentID.getValue();
	queryLiquidPriceInSearchByLiquidPriceShort = LiquidPrice.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CPosition *thePosition;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.LiquidPrice = LiquidPrice.getValue();

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByLiquidPriceShort = pLiquidPriceShortIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByLiquidPriceShort == NULL)
		return NULL;

	///获取找到的对象
	thePosition = (CPosition *)(pLastFoundInSearchByLiquidPriceShort->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePosition->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchByLiquidPriceShort = NULL;
		return NULL;
	}
	if (compare(thePosition->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByLiquidPriceShort = NULL;
		return NULL;
	}

	return thePosition;
}

CPosition *CPositionFactory::findNextByLiquidPriceShort()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByLiquidPriceShort == NULL)
		return NULL;

	CPosition *thePosition;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByLiquidPriceShort = pLiquidPriceShortIndex->getNextNode(pLastFoundInSearchByLiquidPriceShort);
	if (pLastFoundInSearchByLiquidPriceShort == NULL)
		return NULL;

	thePosition = (CPosition *)(pLastFoundInSearchByLiquidPriceShort->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePosition->ExchangeID, queryExchangeIDInSearchByLiquidPriceShort))
	{
		pLastFoundInSearchByLiquidPriceShort = NULL;
		return NULL;
	}
	if (compare(thePosition->InstrumentID, queryInstrumentIDInSearchByLiquidPriceShort))
	{
		pLastFoundInSearchByLiquidPriceShort = NULL;
		return NULL;
	}

	///返回找到的结果
	return thePosition;
}

int compareForProductGroupIndexinMarketData(const void *pV1, const void *pV2)
{
	CMarketData *p1, *p2;
	p1 = (CMarketData *)pV1;
	p2 = (CMarketData *)pV2;
	switch (compare(p1->ProductGroup.getValue(), p2->ProductGroup.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CMarketData* CMarketDataFactory::findByInstrumentID(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{	
	CHashIndexNode *pNode;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();

	compareObject.calHashInstrumentIDIndex();
	for (pNode = pInstrumentIDHashIndex->searchFirstEqual(compareObject.HashInstrumentIDIndex);
		pNode != NULL;
		pNode = pInstrumentIDHashIndex->searchNextEqual(pNode))
	{
		if (((CMarketData *)(pNode->pObject))->HashInstrumentIDIndex != compareObject.HashInstrumentIDIndex)
			continue;
		if (compare(((CMarketData *)(pNode->pObject))->ExchangeID.getValue(), ExchangeID.getValue()) != 0)
			continue;
		if (compare(((CMarketData *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		return (CMarketData *)(pNode->pObject);
	}
	return NULL;
}

CMarketData *CMarketDataFactory::findByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	return findByInstrumentID(ExchangeID, InstrumentID);
}

bool CMarketDataFactory::removeByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, CTransaction *pTransaction)
{
	CMarketData *pOriginalMarketData;
	pOriginalMarketData = findByInstrumentID(ExchangeID, InstrumentID);
	if (pOriginalMarketData == NULL)
		return false;
	remove(pOriginalMarketData, pTransaction);
	return true;
}

CMarketData *CMarketDataFactory::findByPK(CMarketDataField *pMarketData)
{
	return findByPK(pMarketData->ExchangeID, pMarketData->InstrumentID);
}

CMarketData *CMarketDataFactory::findByPK(CMarketData *pMarketData)
{
	return findByPK(pMarketData->ExchangeID, pMarketData->InstrumentID);
}

bool CMarketDataFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "ExchangeID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	return false;
}

bool CMarketDataFactory::retrieveByPK(CWriteableMarketData *pMarketData)
{
	CMarketData *pOriginalMarketData;
	pOriginalMarketData = findByPK(pMarketData);
	if (pOriginalMarketData == NULL)
		return false;
	retrieve(pOriginalMarketData, pMarketData);
	return true;
}

CMarketData *CMarketDataFactory::addByPK(CWriteableMarketData *pMarketData, CTransaction *pTransaction)
{
	CMarketData *pOriginalMarketData;
	pOriginalMarketData = findByPK(pMarketData);
	if (pOriginalMarketData != NULL)
		return NULL;
	return add(pMarketData, pTransaction);
}

bool CMarketDataFactory::updateByPK(CWriteableMarketData *pMarketData, CTransaction *pTransaction)
{
	CMarketData *pOriginalMarketData;
	pOriginalMarketData = findByPK(pMarketData);
	if (pOriginalMarketData == NULL)
		return false;
	pMarketData->calAllHash();
	update(pOriginalMarketData, pMarketData, pTransaction);
	return true;
}

CMarketData* CMarketDataFactory::addOrUpdateByPK(CWriteableMarketData *pMarketData, CTransaction *pTransaction)
{
	CMarketData *pOriginalMarketData;
	pOriginalMarketData = findByPK(pMarketData);
	if (pOriginalMarketData == NULL)
		return add(pMarketData, pTransaction);
	pMarketData->calAllHash();
	update(pOriginalMarketData, pMarketData, pTransaction);
	return pOriginalMarketData;
}

bool CMarketDataFactory::removeByPK(CMarketData *pMarketData, CTransaction *pTransaction)
{
	CMarketData *pOriginalMarketData;
	pOriginalMarketData = findByPK(pMarketData);
	if (pOriginalMarketData == NULL)
		return false;
	remove(pOriginalMarketData, pTransaction);
	return true;
}

bool CMarketDataFactory::removeByPK(CMarketDataField *pMarketData, CTransaction *pTransaction)
{
	CMarketData *pOriginalMarketData;
	pOriginalMarketData = findByPK(pMarketData);
	if (pOriginalMarketData == NULL)
		return false;
	CopyMarketDataEntity(pMarketData, pOriginalMarketData);
	remove(pOriginalMarketData, pTransaction);
	return true;
}



CMarketData* CMarketDataFactory::findFirstByProductGroup(const CReadOnlyShortIndexIDType& ProductGroup)
{
	pLastFoundInSearchByProductGroup = NULL;
	///记录所有的查询值
	queryProductGroupInSearchByProductGroup = ProductGroup.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CMarketData *theMarketData;
	compareObject.ProductGroup = ProductGroup.getValue();

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByProductGroup = pProductGroupIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByProductGroup == NULL)
		return NULL;

	///获取找到的对象
	theMarketData = (CMarketData *)(pLastFoundInSearchByProductGroup->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theMarketData->ProductGroup.getValue(), ProductGroup.getValue()))
	{
		pLastFoundInSearchByProductGroup = NULL;
		return NULL;
	}

	return theMarketData;
}

CMarketData *CMarketDataFactory::findNextByProductGroup()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByProductGroup == NULL)
		return NULL;

	CMarketData *theMarketData;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByProductGroup = pProductGroupIndex->getNextNode(pLastFoundInSearchByProductGroup);
	if (pLastFoundInSearchByProductGroup == NULL)
		return NULL;

	theMarketData = (CMarketData *)(pLastFoundInSearchByProductGroup->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theMarketData->ProductGroup, queryProductGroupInSearchByProductGroup))
	{
		pLastFoundInSearchByProductGroup = NULL;
		return NULL;
	}

	///返回找到的结果
	return theMarketData;
}

int compareForPositionIDIndexinOrder(const void *pV1, const void *pV2)
{
	COrder *p1, *p2;
	p1 = (COrder *)pV1;
	p2 = (COrder *)pV2;
	switch (compare(p1->PositionID.getValue(), p2->PositionID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForLocalIDIndexinOrder(const void *pV1, const void *pV2)
{
	COrder *p1, *p2;
	p1 = (COrder *)pV1;
	p2 = (COrder *)pV2;
	switch (compare(p1->AccountID.getValue(), p2->AccountID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->LocalID.getValue(), p2->LocalID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForInstrumentIDIndexinOrder(const void *pV1, const void *pV2)
{
	COrder *p1, *p2;
	p1 = (COrder *)pV1;
	p2 = (COrder *)pV2;
	switch (compare(p1->AccountID.getValue(), p2->AccountID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->OrderID.getValue(), p2->OrderID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
COrder* COrderFactory::findByOrderID(const CReadOnlyIndexIDType& OrderID)
{	
	CHashIndexNode *pNode;
	compareObject.OrderID = OrderID.getValue();

	compareObject.calHashOrderID();
	for (pNode = pOrderIDHashIndex->searchFirstEqual(compareObject.HashOrderID);
		pNode != NULL;
		pNode = pOrderIDHashIndex->searchNextEqual(pNode))
	{
		if (((COrder *)(pNode->pObject))->HashOrderID != compareObject.HashOrderID)
			continue;
		if (compare(((COrder *)(pNode->pObject))->OrderID.getValue(), OrderID.getValue()) != 0)
			continue;
		return (COrder *)(pNode->pObject);
	}
	return NULL;
}

COrder *COrderFactory::findByPK(const CReadOnlyIndexIDType& OrderID)
{
	return findByOrderID(OrderID);
}

bool COrderFactory::removeByPK(const CReadOnlyIndexIDType& OrderID, CTransaction *pTransaction)
{
	COrder *pOriginalOrder;
	pOriginalOrder = findByOrderID(OrderID);
	if (pOriginalOrder == NULL)
		return false;
	remove(pOriginalOrder, pTransaction);
	return true;
}

COrder *COrderFactory::findByPK(COrderField *pOrder)
{
	return findByPK(pOrder->OrderID);
}

COrder *COrderFactory::findByPK(COrder *pOrder)
{
	return findByPK(pOrder->OrderID);
}

bool COrderFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "OrderID"))
		return true;
	return false;
}

bool COrderFactory::retrieveByPK(CWriteableOrder *pOrder)
{
	COrder *pOriginalOrder;
	pOriginalOrder = findByPK(pOrder);
	if (pOriginalOrder == NULL)
		return false;
	retrieve(pOriginalOrder, pOrder);
	return true;
}

COrder *COrderFactory::addByPK(CWriteableOrder *pOrder, CTransaction *pTransaction)
{
	COrder *pOriginalOrder;
	pOriginalOrder = findByPK(pOrder);
	if (pOriginalOrder != NULL)
		return NULL;
	return add(pOrder, pTransaction);
}

bool COrderFactory::updateByPK(CWriteableOrder *pOrder, CTransaction *pTransaction)
{
	COrder *pOriginalOrder;
	pOriginalOrder = findByPK(pOrder);
	if (pOriginalOrder == NULL)
		return false;
	pOrder->calAllHash();
	update(pOriginalOrder, pOrder, pTransaction);
	return true;
}

COrder* COrderFactory::addOrUpdateByPK(CWriteableOrder *pOrder, CTransaction *pTransaction)
{
	COrder *pOriginalOrder;
	pOriginalOrder = findByPK(pOrder);
	if (pOriginalOrder == NULL)
		return add(pOrder, pTransaction);
	pOrder->calAllHash();
	update(pOriginalOrder, pOrder, pTransaction);
	return pOriginalOrder;
}

bool COrderFactory::removeByPK(COrder *pOrder, CTransaction *pTransaction)
{
	COrder *pOriginalOrder;
	pOriginalOrder = findByPK(pOrder);
	if (pOriginalOrder == NULL)
		return false;
	remove(pOriginalOrder, pTransaction);
	return true;
}

bool COrderFactory::removeByPK(COrderField *pOrder, CTransaction *pTransaction)
{
	COrder *pOriginalOrder;
	pOriginalOrder = findByPK(pOrder);
	if (pOriginalOrder == NULL)
		return false;
	CopyOrderEntity(pOrder, pOriginalOrder);
	remove(pOriginalOrder, pTransaction);
	return true;
}
COrder* COrderFactory::findByLocalID(const CReadOnlyAccountIDType& AccountID, const CReadOnlyIndexIDType& LocalID)
{	
	CAVLNode *pNode;
	compareObject.AccountID = AccountID.getValue();
	compareObject.LocalID = LocalID.getValue();
	pNode = pLocalIDIndex->searchFirstEqual(&compareObject);
	if (pNode == NULL)
		return NULL;
	return (COrder *)(pNode->pObject);
}




COrder* COrderFactory::findFirstByAccountID(const CReadOnlyAccountIDType& AccountID)
{
	pLastFoundInSearchByAccountID = NULL;
	///记录所有的查询值
	queryAccountIDInSearchByAccountID = AccountID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	COrder *theOrder;
	compareObject.AccountID = AccountID.getValue();
	compareObject.LocalID = "";

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByAccountID = pLocalIDIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	///获取找到的对象
	theOrder = (COrder *)(pLastFoundInSearchByAccountID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theOrder->AccountID.getValue(), AccountID.getValue()))
	{
		pLastFoundInSearchByAccountID = NULL;
		return NULL;
	}

	return theOrder;
}

COrder *COrderFactory::findNextByAccountID()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	COrder *theOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByAccountID = pLocalIDIndex->getNextNode(pLastFoundInSearchByAccountID);
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	theOrder = (COrder *)(pLastFoundInSearchByAccountID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theOrder->AccountID, queryAccountIDInSearchByAccountID))
	{
		pLastFoundInSearchByAccountID = NULL;
		return NULL;
	}

	///返回找到的结果
	return theOrder;
}




COrder* COrderFactory::findFirstByPositionID(const CReadOnlyLongIndexIDType& PositionID)
{
	pLastFoundInSearchByPositionID = NULL;
	///记录所有的查询值
	queryPositionIDInSearchByPositionID = PositionID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	COrder *theOrder;
	compareObject.PositionID = PositionID.getValue();

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByPositionID = pPositionIDIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByPositionID == NULL)
		return NULL;

	///获取找到的对象
	theOrder = (COrder *)(pLastFoundInSearchByPositionID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theOrder->PositionID.getValue(), PositionID.getValue()))
	{
		pLastFoundInSearchByPositionID = NULL;
		return NULL;
	}

	return theOrder;
}

COrder *COrderFactory::findNextByPositionID()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByPositionID == NULL)
		return NULL;

	COrder *theOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByPositionID = pPositionIDIndex->getNextNode(pLastFoundInSearchByPositionID);
	if (pLastFoundInSearchByPositionID == NULL)
		return NULL;

	theOrder = (COrder *)(pLastFoundInSearchByPositionID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theOrder->PositionID, queryPositionIDInSearchByPositionID))
	{
		pLastFoundInSearchByPositionID = NULL;
		return NULL;
	}

	///返回找到的结果
	return theOrder;
}




COrder* COrderFactory::findFirstByAccountIDPrivate(const CReadOnlyAccountIDType& AccountID)
{
	pLastFoundInSearchByAccountIDPrivate = NULL;
	///记录所有的查询值
	queryAccountIDInSearchByAccountIDPrivate = AccountID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	COrder *theOrder;
	compareObject.AccountID = AccountID.getValue();
	compareObject.InstrumentID = "";
	compareObject.OrderID = "";

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByAccountIDPrivate = pInstrumentIDIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByAccountIDPrivate == NULL)
		return NULL;

	///获取找到的对象
	theOrder = (COrder *)(pLastFoundInSearchByAccountIDPrivate->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theOrder->AccountID.getValue(), AccountID.getValue()))
	{
		pLastFoundInSearchByAccountIDPrivate = NULL;
		return NULL;
	}

	return theOrder;
}

COrder *COrderFactory::findNextByAccountIDPrivate()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByAccountIDPrivate == NULL)
		return NULL;

	COrder *theOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByAccountIDPrivate = pInstrumentIDIndex->getNextNode(pLastFoundInSearchByAccountIDPrivate);
	if (pLastFoundInSearchByAccountIDPrivate == NULL)
		return NULL;

	theOrder = (COrder *)(pLastFoundInSearchByAccountIDPrivate->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theOrder->AccountID, queryAccountIDInSearchByAccountIDPrivate))
	{
		pLastFoundInSearchByAccountIDPrivate = NULL;
		return NULL;
	}

	///返回找到的结果
	return theOrder;
}




COrder* COrderFactory::findFirstByInstrumentID(const CReadOnlyAccountIDType& AccountID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	pLastFoundInSearchByInstrumentID = NULL;
	///记录所有的查询值
	queryAccountIDInSearchByInstrumentID = AccountID.getValue();
	queryInstrumentIDInSearchByInstrumentID = InstrumentID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	COrder *theOrder;
	compareObject.AccountID = AccountID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.OrderID = "";

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByInstrumentID = pInstrumentIDIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByInstrumentID == NULL)
		return NULL;

	///获取找到的对象
	theOrder = (COrder *)(pLastFoundInSearchByInstrumentID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theOrder->AccountID.getValue(), AccountID.getValue()))
	{
		pLastFoundInSearchByInstrumentID = NULL;
		return NULL;
	}
	if (compare(theOrder->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByInstrumentID = NULL;
		return NULL;
	}

	return theOrder;
}

COrder *COrderFactory::findNextByInstrumentID()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByInstrumentID == NULL)
		return NULL;

	COrder *theOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByInstrumentID = pInstrumentIDIndex->getNextNode(pLastFoundInSearchByInstrumentID);
	if (pLastFoundInSearchByInstrumentID == NULL)
		return NULL;

	theOrder = (COrder *)(pLastFoundInSearchByInstrumentID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theOrder->AccountID, queryAccountIDInSearchByInstrumentID))
	{
		pLastFoundInSearchByInstrumentID = NULL;
		return NULL;
	}
	if (compare(theOrder->InstrumentID, queryInstrumentIDInSearchByInstrumentID))
	{
		pLastFoundInSearchByInstrumentID = NULL;
		return NULL;
	}

	///返回找到的结果
	return theOrder;
}

int compareForInsertTimeIndexinPositionOrder(const void *pV1, const void *pV2)
{
	CPositionOrder *p1, *p2;
	p1 = (CPositionOrder *)pV1;
	p2 = (CPositionOrder *)pV2;
	switch (compare(p1->MemberID.getValue(), p2->MemberID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->TradeUnitID.getValue(), p2->TradeUnitID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->PosiDirection.getValue(), p2->PosiDirection.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InsertTime.getValue(), p2->InsertTime.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CPositionOrder* CPositionOrderFactory::findByOrderID(const CReadOnlyIndexIDType& OrderID)
{	
	CHashIndexNode *pNode;
	compareObject.OrderID = OrderID.getValue();

	compareObject.calHashOrderID();
	for (pNode = pOrderIDHashIndex->searchFirstEqual(compareObject.HashOrderID);
		pNode != NULL;
		pNode = pOrderIDHashIndex->searchNextEqual(pNode))
	{
		if (((CPositionOrder *)(pNode->pObject))->HashOrderID != compareObject.HashOrderID)
			continue;
		if (compare(((CPositionOrder *)(pNode->pObject))->OrderID.getValue(), OrderID.getValue()) != 0)
			continue;
		return (CPositionOrder *)(pNode->pObject);
	}
	return NULL;
}

CPositionOrder *CPositionOrderFactory::findByPK(const CReadOnlyIndexIDType& OrderID)
{
	return findByOrderID(OrderID);
}

bool CPositionOrderFactory::removeByPK(const CReadOnlyIndexIDType& OrderID, CTransaction *pTransaction)
{
	CPositionOrder *pOriginalPositionOrder;
	pOriginalPositionOrder = findByOrderID(OrderID);
	if (pOriginalPositionOrder == NULL)
		return false;
	remove(pOriginalPositionOrder, pTransaction);
	return true;
}

CPositionOrder *CPositionOrderFactory::findByPK(CPositionOrderField *pPositionOrder)
{
	return findByPK(pPositionOrder->OrderID);
}

CPositionOrder *CPositionOrderFactory::findByPK(CPositionOrder *pPositionOrder)
{
	return findByPK(pPositionOrder->OrderID);
}

bool CPositionOrderFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "OrderID"))
		return true;
	return false;
}

bool CPositionOrderFactory::retrieveByPK(CWriteablePositionOrder *pPositionOrder)
{
	CPositionOrder *pOriginalPositionOrder;
	pOriginalPositionOrder = findByPK(pPositionOrder);
	if (pOriginalPositionOrder == NULL)
		return false;
	retrieve(pOriginalPositionOrder, pPositionOrder);
	return true;
}

CPositionOrder *CPositionOrderFactory::addByPK(CWriteablePositionOrder *pPositionOrder, CTransaction *pTransaction)
{
	CPositionOrder *pOriginalPositionOrder;
	pOriginalPositionOrder = findByPK(pPositionOrder);
	if (pOriginalPositionOrder != NULL)
		return NULL;
	return add(pPositionOrder, pTransaction);
}

bool CPositionOrderFactory::updateByPK(CWriteablePositionOrder *pPositionOrder, CTransaction *pTransaction)
{
	CPositionOrder *pOriginalPositionOrder;
	pOriginalPositionOrder = findByPK(pPositionOrder);
	if (pOriginalPositionOrder == NULL)
		return false;
	pPositionOrder->calAllHash();
	update(pOriginalPositionOrder, pPositionOrder, pTransaction);
	return true;
}

CPositionOrder* CPositionOrderFactory::addOrUpdateByPK(CWriteablePositionOrder *pPositionOrder, CTransaction *pTransaction)
{
	CPositionOrder *pOriginalPositionOrder;
	pOriginalPositionOrder = findByPK(pPositionOrder);
	if (pOriginalPositionOrder == NULL)
		return add(pPositionOrder, pTransaction);
	pPositionOrder->calAllHash();
	update(pOriginalPositionOrder, pPositionOrder, pTransaction);
	return pOriginalPositionOrder;
}

bool CPositionOrderFactory::removeByPK(CPositionOrder *pPositionOrder, CTransaction *pTransaction)
{
	CPositionOrder *pOriginalPositionOrder;
	pOriginalPositionOrder = findByPK(pPositionOrder);
	if (pOriginalPositionOrder == NULL)
		return false;
	remove(pOriginalPositionOrder, pTransaction);
	return true;
}

bool CPositionOrderFactory::removeByPK(CPositionOrderField *pPositionOrder, CTransaction *pTransaction)
{
	CPositionOrder *pOriginalPositionOrder;
	pOriginalPositionOrder = findByPK(pPositionOrder);
	if (pOriginalPositionOrder == NULL)
		return false;
	CopyPositionOrderEntity(pPositionOrder, pOriginalPositionOrder);
	remove(pOriginalPositionOrder, pTransaction);
	return true;
}



CPositionOrder* CPositionOrderFactory::findFirstByInsertTime(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyIndexIDType& TradeUnitID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPosiDirectionType& PosiDirection)
{
	pLastFoundInSearchByInsertTime = NULL;
	///记录所有的查询值
	queryMemberIDInSearchByInsertTime = MemberID.getValue();
	queryTradeUnitIDInSearchByInsertTime = TradeUnitID.getValue();
	queryExchangeIDInSearchByInsertTime = ExchangeID.getValue();
	queryInstrumentIDInSearchByInsertTime = InstrumentID.getValue();
	queryPosiDirectionInSearchByInsertTime = PosiDirection.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CPositionOrder *thePositionOrder;
	compareObject.MemberID = MemberID.getValue();
	compareObject.TradeUnitID = TradeUnitID.getValue();
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.PosiDirection = PosiDirection.getValue();
	compareObject.InsertTime = 0;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByInsertTime = pInsertTimeIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByInsertTime == NULL)
		return NULL;

	///获取找到的对象
	thePositionOrder = (CPositionOrder *)(pLastFoundInSearchByInsertTime->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePositionOrder->MemberID.getValue(), MemberID.getValue()))
	{
		pLastFoundInSearchByInsertTime = NULL;
		return NULL;
	}
	if (compare(thePositionOrder->TradeUnitID.getValue(), TradeUnitID.getValue()))
	{
		pLastFoundInSearchByInsertTime = NULL;
		return NULL;
	}
	if (compare(thePositionOrder->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchByInsertTime = NULL;
		return NULL;
	}
	if (compare(thePositionOrder->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByInsertTime = NULL;
		return NULL;
	}
	if (compare(thePositionOrder->PosiDirection.getValue(), PosiDirection.getValue()))
	{
		pLastFoundInSearchByInsertTime = NULL;
		return NULL;
	}

	return thePositionOrder;
}

CPositionOrder *CPositionOrderFactory::findNextByInsertTime()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByInsertTime == NULL)
		return NULL;

	CPositionOrder *thePositionOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByInsertTime = pInsertTimeIndex->getNextNode(pLastFoundInSearchByInsertTime);
	if (pLastFoundInSearchByInsertTime == NULL)
		return NULL;

	thePositionOrder = (CPositionOrder *)(pLastFoundInSearchByInsertTime->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePositionOrder->MemberID, queryMemberIDInSearchByInsertTime))
	{
		pLastFoundInSearchByInsertTime = NULL;
		return NULL;
	}
	if (compare(thePositionOrder->TradeUnitID, queryTradeUnitIDInSearchByInsertTime))
	{
		pLastFoundInSearchByInsertTime = NULL;
		return NULL;
	}
	if (compare(thePositionOrder->ExchangeID, queryExchangeIDInSearchByInsertTime))
	{
		pLastFoundInSearchByInsertTime = NULL;
		return NULL;
	}
	if (compare(thePositionOrder->InstrumentID, queryInstrumentIDInSearchByInsertTime))
	{
		pLastFoundInSearchByInsertTime = NULL;
		return NULL;
	}
	if (compare(thePositionOrder->PosiDirection, queryPosiDirectionInSearchByInsertTime))
	{
		pLastFoundInSearchByInsertTime = NULL;
		return NULL;
	}

	///返回找到的结果
	return thePositionOrder;
}

int compareForAccountIDLocalIDIndexinTriggerOrder(const void *pV1, const void *pV2)
{
	CTriggerOrder *p1, *p2;
	p1 = (CTriggerOrder *)pV1;
	p2 = (CTriggerOrder *)pV2;
	switch (compare(p1->AccountID.getValue(), p2->AccountID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->LocalID.getValue(), p2->LocalID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForActivePositionCloseIndexinTriggerOrder(const void *pV1, const void *pV2)
{
	CTriggerOrder *p1, *p2;
	p1 = (CTriggerOrder *)pV1;
	p2 = (CTriggerOrder *)pV2;
	switch (compare(p1->PositionID.getValue(), p2->PositionID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForBuySLTriggerPriceIndexinTriggerOrder(const void *pV1, const void *pV2)
{
	CTriggerOrder *p1, *p2;
	p1 = (CTriggerOrder *)pV1;
	p2 = (CTriggerOrder *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Direction.getValue(), p2->Direction.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->SLTriggerPrice.getValue(), p2->SLTriggerPrice.getValue()))
	{
	case 1:
		return -1;
	case -1:
		return 1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->TimeSortNo.getValue(), p2->TimeSortNo.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForSellSLTriggerPriceIndexinTriggerOrder(const void *pV1, const void *pV2)
{
	CTriggerOrder *p1, *p2;
	p1 = (CTriggerOrder *)pV1;
	p2 = (CTriggerOrder *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Direction.getValue(), p2->Direction.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->SLTriggerPrice.getValue(), p2->SLTriggerPrice.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->TimeSortNo.getValue(), p2->TimeSortNo.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForBuyTPTriggerPriceIndexinTriggerOrder(const void *pV1, const void *pV2)
{
	CTriggerOrder *p1, *p2;
	p1 = (CTriggerOrder *)pV1;
	p2 = (CTriggerOrder *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Direction.getValue(), p2->Direction.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->TPTriggerPrice.getValue(), p2->TPTriggerPrice.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->TimeSortNo.getValue(), p2->TimeSortNo.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForSellTPTriggerPriceIndexinTriggerOrder(const void *pV1, const void *pV2)
{
	CTriggerOrder *p1, *p2;
	p1 = (CTriggerOrder *)pV1;
	p2 = (CTriggerOrder *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Direction.getValue(), p2->Direction.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->TPTriggerPrice.getValue(), p2->TPTriggerPrice.getValue()))
	{
	case 1:
		return -1;
	case -1:
		return 1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->TimeSortNo.getValue(), p2->TimeSortNo.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CTriggerOrder* CTriggerOrderFactory::findByOrderID(const CReadOnlyIndexIDType& OrderID)
{	
	CHashIndexNode *pNode;
	compareObject.OrderID = OrderID.getValue();

	compareObject.calHashOrderID();
	for (pNode = pOrderIDHashIndex->searchFirstEqual(compareObject.HashOrderID);
		pNode != NULL;
		pNode = pOrderIDHashIndex->searchNextEqual(pNode))
	{
		if (((CTriggerOrder *)(pNode->pObject))->HashOrderID != compareObject.HashOrderID)
			continue;
		if (compare(((CTriggerOrder *)(pNode->pObject))->OrderID.getValue(), OrderID.getValue()) != 0)
			continue;
		return (CTriggerOrder *)(pNode->pObject);
	}
	return NULL;
}

CTriggerOrder *CTriggerOrderFactory::findByPK(const CReadOnlyIndexIDType& OrderID)
{
	return findByOrderID(OrderID);
}

bool CTriggerOrderFactory::removeByPK(const CReadOnlyIndexIDType& OrderID, CTransaction *pTransaction)
{
	CTriggerOrder *pOriginalTriggerOrder;
	pOriginalTriggerOrder = findByOrderID(OrderID);
	if (pOriginalTriggerOrder == NULL)
		return false;
	remove(pOriginalTriggerOrder, pTransaction);
	return true;
}

CTriggerOrder *CTriggerOrderFactory::findByPK(CTriggerOrderField *pTriggerOrder)
{
	return findByPK(pTriggerOrder->OrderID);
}

CTriggerOrder *CTriggerOrderFactory::findByPK(CTriggerOrder *pTriggerOrder)
{
	return findByPK(pTriggerOrder->OrderID);
}

bool CTriggerOrderFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "OrderID"))
		return true;
	return false;
}

bool CTriggerOrderFactory::retrieveByPK(CWriteableTriggerOrder *pTriggerOrder)
{
	CTriggerOrder *pOriginalTriggerOrder;
	pOriginalTriggerOrder = findByPK(pTriggerOrder);
	if (pOriginalTriggerOrder == NULL)
		return false;
	retrieve(pOriginalTriggerOrder, pTriggerOrder);
	return true;
}

CTriggerOrder *CTriggerOrderFactory::addByPK(CWriteableTriggerOrder *pTriggerOrder, CTransaction *pTransaction)
{
	CTriggerOrder *pOriginalTriggerOrder;
	pOriginalTriggerOrder = findByPK(pTriggerOrder);
	if (pOriginalTriggerOrder != NULL)
		return NULL;
	return add(pTriggerOrder, pTransaction);
}

bool CTriggerOrderFactory::updateByPK(CWriteableTriggerOrder *pTriggerOrder, CTransaction *pTransaction)
{
	CTriggerOrder *pOriginalTriggerOrder;
	pOriginalTriggerOrder = findByPK(pTriggerOrder);
	if (pOriginalTriggerOrder == NULL)
		return false;
	pTriggerOrder->calAllHash();
	update(pOriginalTriggerOrder, pTriggerOrder, pTransaction);
	return true;
}

CTriggerOrder* CTriggerOrderFactory::addOrUpdateByPK(CWriteableTriggerOrder *pTriggerOrder, CTransaction *pTransaction)
{
	CTriggerOrder *pOriginalTriggerOrder;
	pOriginalTriggerOrder = findByPK(pTriggerOrder);
	if (pOriginalTriggerOrder == NULL)
		return add(pTriggerOrder, pTransaction);
	pTriggerOrder->calAllHash();
	update(pOriginalTriggerOrder, pTriggerOrder, pTransaction);
	return pOriginalTriggerOrder;
}

bool CTriggerOrderFactory::removeByPK(CTriggerOrder *pTriggerOrder, CTransaction *pTransaction)
{
	CTriggerOrder *pOriginalTriggerOrder;
	pOriginalTriggerOrder = findByPK(pTriggerOrder);
	if (pOriginalTriggerOrder == NULL)
		return false;
	remove(pOriginalTriggerOrder, pTransaction);
	return true;
}

bool CTriggerOrderFactory::removeByPK(CTriggerOrderField *pTriggerOrder, CTransaction *pTransaction)
{
	CTriggerOrder *pOriginalTriggerOrder;
	pOriginalTriggerOrder = findByPK(pTriggerOrder);
	if (pOriginalTriggerOrder == NULL)
		return false;
	CopyTriggerOrderEntity(pTriggerOrder, pOriginalTriggerOrder);
	remove(pOriginalTriggerOrder, pTransaction);
	return true;
}
CTriggerOrder* CTriggerOrderFactory::findByLocalID(const CReadOnlyAccountIDType& AccountID, const CReadOnlyIndexIDType& LocalID)
{	
	CAVLNode *pNode;
	compareObject.AccountID = AccountID.getValue();
	compareObject.LocalID = LocalID.getValue();
	pNode = pAccountIDLocalIDIndex->searchFirstEqual(&compareObject);
	if (pNode == NULL)
		return NULL;
	return (CTriggerOrder *)(pNode->pObject);
}




CTriggerOrder* CTriggerOrderFactory::findFirstByBuySLTriggerPrice(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& SLTriggerPrice)
{
	pLastFoundInSearchByBuySLTriggerPrice = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchByBuySLTriggerPrice = ExchangeID.getValue();
	queryInstrumentIDInSearchByBuySLTriggerPrice = InstrumentID.getValue();
	querySLTriggerPriceInSearchByBuySLTriggerPrice = SLTriggerPrice.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CTriggerOrder *theTriggerOrder;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.SLTriggerPrice = SLTriggerPrice.getValue();
	compareObject.Direction = D_Buy;
	compareObject.TimeSortNo = 0;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByBuySLTriggerPrice = pBuySLTriggerPriceIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByBuySLTriggerPrice == NULL)
		return NULL;

	///获取找到的对象
	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchByBuySLTriggerPrice->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchByBuySLTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByBuySLTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->Direction, D_Buy))
	{
		pLastFoundInSearchByBuySLTriggerPrice = NULL;
		return NULL;
	}

	return theTriggerOrder;
}

CTriggerOrder *CTriggerOrderFactory::findNextByBuySLTriggerPrice()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByBuySLTriggerPrice == NULL)
		return NULL;

	CTriggerOrder *theTriggerOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByBuySLTriggerPrice = pBuySLTriggerPriceIndex->getNextNode(pLastFoundInSearchByBuySLTriggerPrice);
	if (pLastFoundInSearchByBuySLTriggerPrice == NULL)
		return NULL;

	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchByBuySLTriggerPrice->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->ExchangeID, queryExchangeIDInSearchByBuySLTriggerPrice))
	{
		pLastFoundInSearchByBuySLTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->InstrumentID, queryInstrumentIDInSearchByBuySLTriggerPrice))
	{
		pLastFoundInSearchByBuySLTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->Direction, D_Buy))
	{
		pLastFoundInSearchByBuySLTriggerPrice = NULL;
		return NULL;
	}

	///返回找到的结果
	return theTriggerOrder;
}




CTriggerOrder* CTriggerOrderFactory::findFirstBySellSLTriggerPrice(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& SLTriggerPrice)
{
	pLastFoundInSearchBySellSLTriggerPrice = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchBySellSLTriggerPrice = ExchangeID.getValue();
	queryInstrumentIDInSearchBySellSLTriggerPrice = InstrumentID.getValue();
	querySLTriggerPriceInSearchBySellSLTriggerPrice = SLTriggerPrice.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CTriggerOrder *theTriggerOrder;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.SLTriggerPrice = SLTriggerPrice.getValue();
	compareObject.Direction = D_Sell;
	compareObject.TimeSortNo = 0;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchBySellSLTriggerPrice = pSellSLTriggerPriceIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchBySellSLTriggerPrice == NULL)
		return NULL;

	///获取找到的对象
	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchBySellSLTriggerPrice->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchBySellSLTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchBySellSLTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->Direction, D_Sell))
	{
		pLastFoundInSearchBySellSLTriggerPrice = NULL;
		return NULL;
	}

	return theTriggerOrder;
}

CTriggerOrder *CTriggerOrderFactory::findNextBySellSLTriggerPrice()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchBySellSLTriggerPrice == NULL)
		return NULL;

	CTriggerOrder *theTriggerOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchBySellSLTriggerPrice = pSellSLTriggerPriceIndex->getNextNode(pLastFoundInSearchBySellSLTriggerPrice);
	if (pLastFoundInSearchBySellSLTriggerPrice == NULL)
		return NULL;

	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchBySellSLTriggerPrice->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->ExchangeID, queryExchangeIDInSearchBySellSLTriggerPrice))
	{
		pLastFoundInSearchBySellSLTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->InstrumentID, queryInstrumentIDInSearchBySellSLTriggerPrice))
	{
		pLastFoundInSearchBySellSLTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->Direction, D_Sell))
	{
		pLastFoundInSearchBySellSLTriggerPrice = NULL;
		return NULL;
	}

	///返回找到的结果
	return theTriggerOrder;
}




CTriggerOrder* CTriggerOrderFactory::findFirstByBuyTPTriggerPrice(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& TPTriggerPrice)
{
	pLastFoundInSearchByBuyTPTriggerPrice = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchByBuyTPTriggerPrice = ExchangeID.getValue();
	queryInstrumentIDInSearchByBuyTPTriggerPrice = InstrumentID.getValue();
	queryTPTriggerPriceInSearchByBuyTPTriggerPrice = TPTriggerPrice.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CTriggerOrder *theTriggerOrder;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.TPTriggerPrice = TPTriggerPrice.getValue();
	compareObject.Direction = D_Buy;
	compareObject.TimeSortNo = 0;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByBuyTPTriggerPrice = pBuyTPTriggerPriceIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByBuyTPTriggerPrice == NULL)
		return NULL;

	///获取找到的对象
	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchByBuyTPTriggerPrice->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchByBuyTPTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByBuyTPTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->Direction, D_Buy))
	{
		pLastFoundInSearchByBuyTPTriggerPrice = NULL;
		return NULL;
	}

	return theTriggerOrder;
}

CTriggerOrder *CTriggerOrderFactory::findNextByBuyTPTriggerPrice()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByBuyTPTriggerPrice == NULL)
		return NULL;

	CTriggerOrder *theTriggerOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByBuyTPTriggerPrice = pBuyTPTriggerPriceIndex->getNextNode(pLastFoundInSearchByBuyTPTriggerPrice);
	if (pLastFoundInSearchByBuyTPTriggerPrice == NULL)
		return NULL;

	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchByBuyTPTriggerPrice->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->ExchangeID, queryExchangeIDInSearchByBuyTPTriggerPrice))
	{
		pLastFoundInSearchByBuyTPTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->InstrumentID, queryInstrumentIDInSearchByBuyTPTriggerPrice))
	{
		pLastFoundInSearchByBuyTPTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->Direction, D_Buy))
	{
		pLastFoundInSearchByBuyTPTriggerPrice = NULL;
		return NULL;
	}

	///返回找到的结果
	return theTriggerOrder;
}




CTriggerOrder* CTriggerOrderFactory::findFirstBySellTPTriggerPrice(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& TPTriggerPrice)
{
	pLastFoundInSearchBySellTPTriggerPrice = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchBySellTPTriggerPrice = ExchangeID.getValue();
	queryInstrumentIDInSearchBySellTPTriggerPrice = InstrumentID.getValue();
	queryTPTriggerPriceInSearchBySellTPTriggerPrice = TPTriggerPrice.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CTriggerOrder *theTriggerOrder;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.TPTriggerPrice = TPTriggerPrice.getValue();
	compareObject.Direction = D_Sell;
	compareObject.TimeSortNo = 0;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchBySellTPTriggerPrice = pSellTPTriggerPriceIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchBySellTPTriggerPrice == NULL)
		return NULL;

	///获取找到的对象
	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchBySellTPTriggerPrice->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchBySellTPTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchBySellTPTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->Direction, D_Sell))
	{
		pLastFoundInSearchBySellTPTriggerPrice = NULL;
		return NULL;
	}

	return theTriggerOrder;
}

CTriggerOrder *CTriggerOrderFactory::findNextBySellTPTriggerPrice()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchBySellTPTriggerPrice == NULL)
		return NULL;

	CTriggerOrder *theTriggerOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchBySellTPTriggerPrice = pSellTPTriggerPriceIndex->getNextNode(pLastFoundInSearchBySellTPTriggerPrice);
	if (pLastFoundInSearchBySellTPTriggerPrice == NULL)
		return NULL;

	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchBySellTPTriggerPrice->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->ExchangeID, queryExchangeIDInSearchBySellTPTriggerPrice))
	{
		pLastFoundInSearchBySellTPTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->InstrumentID, queryInstrumentIDInSearchBySellTPTriggerPrice))
	{
		pLastFoundInSearchBySellTPTriggerPrice = NULL;
		return NULL;
	}
	if (compare(theTriggerOrder->Direction, D_Sell))
	{
		pLastFoundInSearchBySellTPTriggerPrice = NULL;
		return NULL;
	}

	///返回找到的结果
	return theTriggerOrder;
}




CTriggerOrder* CTriggerOrderFactory::findFirstByAccountID(const CReadOnlyAccountIDType& AccountID)
{
	pLastFoundInSearchByAccountID = NULL;
	///记录所有的查询值
	queryAccountIDInSearchByAccountID = AccountID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CTriggerOrder *theTriggerOrder;
	compareObject.AccountID = AccountID.getValue();

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByAccountID = pAccountIDLocalIDIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	///获取找到的对象
	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchByAccountID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->AccountID.getValue(), AccountID.getValue()))
	{
		pLastFoundInSearchByAccountID = NULL;
		return NULL;
	}

	return theTriggerOrder;
}

CTriggerOrder *CTriggerOrderFactory::findNextByAccountID()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	CTriggerOrder *theTriggerOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByAccountID = pAccountIDLocalIDIndex->getNextNode(pLastFoundInSearchByAccountID);
	if (pLastFoundInSearchByAccountID == NULL)
		return NULL;

	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchByAccountID->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->AccountID, queryAccountIDInSearchByAccountID))
	{
		pLastFoundInSearchByAccountID = NULL;
		return NULL;
	}

	///返回找到的结果
	return theTriggerOrder;
}




CTriggerOrder* CTriggerOrderFactory::findFirstByActivePositionClose(const CReadOnlyLongIndexIDType& PositionID)
{
	pLastFoundInSearchByActivePositionClose = NULL;
	///记录所有的查询值
	queryPositionIDInSearchByActivePositionClose = PositionID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CTriggerOrder *theTriggerOrder;
	compareObject.PositionID = PositionID.getValue();

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByActivePositionClose = pActivePositionCloseIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByActivePositionClose == NULL)
		return NULL;

	///获取找到的对象
	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchByActivePositionClose->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->PositionID.getValue(), PositionID.getValue()))
	{
		pLastFoundInSearchByActivePositionClose = NULL;
		return NULL;
	}

	return theTriggerOrder;
}

CTriggerOrder *CTriggerOrderFactory::findNextByActivePositionClose()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByActivePositionClose == NULL)
		return NULL;

	CTriggerOrder *theTriggerOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByActivePositionClose = pActivePositionCloseIndex->getNextNode(pLastFoundInSearchByActivePositionClose);
	if (pLastFoundInSearchByActivePositionClose == NULL)
		return NULL;

	theTriggerOrder = (CTriggerOrder *)(pLastFoundInSearchByActivePositionClose->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTriggerOrder->PositionID, queryPositionIDInSearchByActivePositionClose))
	{
		pLastFoundInSearchByActivePositionClose = NULL;
		return NULL;
	}

	///返回找到的结果
	return theTriggerOrder;
}

int compareForBuyMBLIndexinMarketOrder(const void *pV1, const void *pV2)
{
	CMarketOrder *p1, *p2;
	p1 = (CMarketOrder *)pV1;
	p2 = (CMarketOrder *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Price.getValue(), p2->Price.getValue()))
	{
	case 1:
		return -1;
	case -1:
		return 1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForSellMBLIndexinMarketOrder(const void *pV1, const void *pV2)
{
	CMarketOrder *p1, *p2;
	p1 = (CMarketOrder *)pV1;
	p2 = (CMarketOrder *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Price.getValue(), p2->Price.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CMarketOrder* CMarketOrderFactory::findByPrice(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Price)
{	
	CHashIndexNode *pNode;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Price = Price.getValue();

	compareObject.calHashPriceHashKey();
	for (pNode = pPriceHashIndex->searchFirstEqual(compareObject.HashPriceHashKey);
		pNode != NULL;
		pNode = pPriceHashIndex->searchNextEqual(pNode))
	{
		if (((CMarketOrder *)(pNode->pObject))->HashPriceHashKey != compareObject.HashPriceHashKey)
			continue;
		if (compare(((CMarketOrder *)(pNode->pObject))->ExchangeID.getValue(), ExchangeID.getValue()) != 0)
			continue;
		if (compare(((CMarketOrder *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		if (compare(((CMarketOrder *)(pNode->pObject))->Price.getValue(), Price.getValue()) != 0)
			continue;
		return (CMarketOrder *)(pNode->pObject);
	}
	return NULL;
}

CMarketOrder *CMarketOrderFactory::findByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Price)
{
	return findByPrice(ExchangeID, InstrumentID, Price);
}

bool CMarketOrderFactory::removeByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Price, CTransaction *pTransaction)
{
	CMarketOrder *pOriginalMarketOrder;
	pOriginalMarketOrder = findByPrice(ExchangeID, InstrumentID, Price);
	if (pOriginalMarketOrder == NULL)
		return false;
	remove(pOriginalMarketOrder, pTransaction);
	return true;
}

CMarketOrder *CMarketOrderFactory::findByPK(CMarketOrderField *pMarketOrder)
{
	return findByPK(pMarketOrder->ExchangeID, pMarketOrder->InstrumentID, pMarketOrder->Price);
}

CMarketOrder *CMarketOrderFactory::findByPK(CMarketOrder *pMarketOrder)
{
	return findByPK(pMarketOrder->ExchangeID, pMarketOrder->InstrumentID, pMarketOrder->Price);
}

bool CMarketOrderFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "ExchangeID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	if (!strcmp(pName, "Price"))
		return true;
	return false;
}

bool CMarketOrderFactory::retrieveByPK(CWriteableMarketOrder *pMarketOrder)
{
	CMarketOrder *pOriginalMarketOrder;
	pOriginalMarketOrder = findByPK(pMarketOrder);
	if (pOriginalMarketOrder == NULL)
		return false;
	retrieve(pOriginalMarketOrder, pMarketOrder);
	return true;
}

CMarketOrder *CMarketOrderFactory::addByPK(CWriteableMarketOrder *pMarketOrder, CTransaction *pTransaction)
{
	CMarketOrder *pOriginalMarketOrder;
	pOriginalMarketOrder = findByPK(pMarketOrder);
	if (pOriginalMarketOrder != NULL)
		return NULL;
	return add(pMarketOrder, pTransaction);
}

bool CMarketOrderFactory::updateByPK(CWriteableMarketOrder *pMarketOrder, CTransaction *pTransaction)
{
	CMarketOrder *pOriginalMarketOrder;
	pOriginalMarketOrder = findByPK(pMarketOrder);
	if (pOriginalMarketOrder == NULL)
		return false;
	pMarketOrder->calAllHash();
	update(pOriginalMarketOrder, pMarketOrder, pTransaction);
	return true;
}

CMarketOrder* CMarketOrderFactory::addOrUpdateByPK(CWriteableMarketOrder *pMarketOrder, CTransaction *pTransaction)
{
	CMarketOrder *pOriginalMarketOrder;
	pOriginalMarketOrder = findByPK(pMarketOrder);
	if (pOriginalMarketOrder == NULL)
		return add(pMarketOrder, pTransaction);
	pMarketOrder->calAllHash();
	update(pOriginalMarketOrder, pMarketOrder, pTransaction);
	return pOriginalMarketOrder;
}

bool CMarketOrderFactory::removeByPK(CMarketOrder *pMarketOrder, CTransaction *pTransaction)
{
	CMarketOrder *pOriginalMarketOrder;
	pOriginalMarketOrder = findByPK(pMarketOrder);
	if (pOriginalMarketOrder == NULL)
		return false;
	remove(pOriginalMarketOrder, pTransaction);
	return true;
}

bool CMarketOrderFactory::removeByPK(CMarketOrderField *pMarketOrder, CTransaction *pTransaction)
{
	CMarketOrder *pOriginalMarketOrder;
	pOriginalMarketOrder = findByPK(pMarketOrder);
	if (pOriginalMarketOrder == NULL)
		return false;
	CopyMarketOrderEntity(pMarketOrder, pOriginalMarketOrder);
	remove(pOriginalMarketOrder, pTransaction);
	return true;
}



CMarketOrder* CMarketOrderFactory::findFirstByBuyMBLData(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	pLastFoundInSearchByBuyMBLData = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchByBuyMBLData = ExchangeID.getValue();
	queryInstrumentIDInSearchByBuyMBLData = InstrumentID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CMarketOrder *theMarketOrder;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Price = 999999999;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByBuyMBLData = pBuyMBLIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByBuyMBLData == NULL)
		return NULL;

	///获取找到的对象
	theMarketOrder = (CMarketOrder *)(pLastFoundInSearchByBuyMBLData->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theMarketOrder->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchByBuyMBLData = NULL;
		return NULL;
	}
	if (compare(theMarketOrder->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByBuyMBLData = NULL;
		return NULL;
	}

	return theMarketOrder;
}

CMarketOrder *CMarketOrderFactory::findNextByBuyMBLData()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByBuyMBLData == NULL)
		return NULL;

	CMarketOrder *theMarketOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByBuyMBLData = pBuyMBLIndex->getNextNode(pLastFoundInSearchByBuyMBLData);
	if (pLastFoundInSearchByBuyMBLData == NULL)
		return NULL;

	theMarketOrder = (CMarketOrder *)(pLastFoundInSearchByBuyMBLData->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theMarketOrder->ExchangeID, queryExchangeIDInSearchByBuyMBLData))
	{
		pLastFoundInSearchByBuyMBLData = NULL;
		return NULL;
	}
	if (compare(theMarketOrder->InstrumentID, queryInstrumentIDInSearchByBuyMBLData))
	{
		pLastFoundInSearchByBuyMBLData = NULL;
		return NULL;
	}

	///返回找到的结果
	return theMarketOrder;
}




CMarketOrder* CMarketOrderFactory::findFirstBySellMBLData(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	pLastFoundInSearchBySellMBLData = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchBySellMBLData = ExchangeID.getValue();
	queryInstrumentIDInSearchBySellMBLData = InstrumentID.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CMarketOrder *theMarketOrder;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Price = -999999999;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchBySellMBLData = pSellMBLIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchBySellMBLData == NULL)
		return NULL;

	///获取找到的对象
	theMarketOrder = (CMarketOrder *)(pLastFoundInSearchBySellMBLData->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theMarketOrder->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchBySellMBLData = NULL;
		return NULL;
	}
	if (compare(theMarketOrder->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchBySellMBLData = NULL;
		return NULL;
	}

	return theMarketOrder;
}

CMarketOrder *CMarketOrderFactory::findNextBySellMBLData()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchBySellMBLData == NULL)
		return NULL;

	CMarketOrder *theMarketOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchBySellMBLData = pSellMBLIndex->getNextNode(pLastFoundInSearchBySellMBLData);
	if (pLastFoundInSearchBySellMBLData == NULL)
		return NULL;

	theMarketOrder = (CMarketOrder *)(pLastFoundInSearchBySellMBLData->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theMarketOrder->ExchangeID, queryExchangeIDInSearchBySellMBLData))
	{
		pLastFoundInSearchBySellMBLData = NULL;
		return NULL;
	}
	if (compare(theMarketOrder->InstrumentID, queryInstrumentIDInSearchBySellMBLData))
	{
		pLastFoundInSearchBySellMBLData = NULL;
		return NULL;
	}

	///返回找到的结果
	return theMarketOrder;
}

int compareForBuyMBLIndexinTickMarketOrder(const void *pV1, const void *pV2)
{
	CTickMarketOrder *p1, *p2;
	p1 = (CTickMarketOrder *)pV1;
	p2 = (CTickMarketOrder *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Tick.getValue(), p2->Tick.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Price.getValue(), p2->Price.getValue()))
	{
	case 1:
		return -1;
	case -1:
		return 1;
	case 0:
	default:
		break;
	}
	return 0;
}
int compareForSellMBLIndexinTickMarketOrder(const void *pV1, const void *pV2)
{
	CTickMarketOrder *p1, *p2;
	p1 = (CTickMarketOrder *)pV1;
	p2 = (CTickMarketOrder *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Tick.getValue(), p2->Tick.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Price.getValue(), p2->Price.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	return 0;
}
	



CTickMarketOrder* CTickMarketOrderFactory::findFirstByBuyMBLData(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Tick)
{
	pLastFoundInSearchByBuyMBLData = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchByBuyMBLData = ExchangeID.getValue();
	queryInstrumentIDInSearchByBuyMBLData = InstrumentID.getValue();
	queryTickInSearchByBuyMBLData = Tick.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CTickMarketOrder *theTickMarketOrder;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Tick = Tick.getValue();
	compareObject.Price = 999999999;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByBuyMBLData = pBuyMBLIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByBuyMBLData == NULL)
		return NULL;

	///获取找到的对象
	theTickMarketOrder = (CTickMarketOrder *)(pLastFoundInSearchByBuyMBLData->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTickMarketOrder->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchByBuyMBLData = NULL;
		return NULL;
	}
	if (compare(theTickMarketOrder->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByBuyMBLData = NULL;
		return NULL;
	}
	if (compare(theTickMarketOrder->Tick.getValue(), Tick.getValue()))
	{
		pLastFoundInSearchByBuyMBLData = NULL;
		return NULL;
	}

	return theTickMarketOrder;
}

CTickMarketOrder *CTickMarketOrderFactory::findNextByBuyMBLData()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByBuyMBLData == NULL)
		return NULL;

	CTickMarketOrder *theTickMarketOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByBuyMBLData = pBuyMBLIndex->getNextNode(pLastFoundInSearchByBuyMBLData);
	if (pLastFoundInSearchByBuyMBLData == NULL)
		return NULL;

	theTickMarketOrder = (CTickMarketOrder *)(pLastFoundInSearchByBuyMBLData->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTickMarketOrder->ExchangeID, queryExchangeIDInSearchByBuyMBLData))
	{
		pLastFoundInSearchByBuyMBLData = NULL;
		return NULL;
	}
	if (compare(theTickMarketOrder->InstrumentID, queryInstrumentIDInSearchByBuyMBLData))
	{
		pLastFoundInSearchByBuyMBLData = NULL;
		return NULL;
	}
	if (compare(theTickMarketOrder->Tick, queryTickInSearchByBuyMBLData))
	{
		pLastFoundInSearchByBuyMBLData = NULL;
		return NULL;
	}

	///返回找到的结果
	return theTickMarketOrder;
}




CTickMarketOrder* CTickMarketOrderFactory::findFirstBySellMBLData(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Tick)
{
	pLastFoundInSearchBySellMBLData = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchBySellMBLData = ExchangeID.getValue();
	queryInstrumentIDInSearchBySellMBLData = InstrumentID.getValue();
	queryTickInSearchBySellMBLData = Tick.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CTickMarketOrder *theTickMarketOrder;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Tick = Tick.getValue();
	compareObject.Price = -999999999;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchBySellMBLData = pSellMBLIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchBySellMBLData == NULL)
		return NULL;

	///获取找到的对象
	theTickMarketOrder = (CTickMarketOrder *)(pLastFoundInSearchBySellMBLData->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTickMarketOrder->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchBySellMBLData = NULL;
		return NULL;
	}
	if (compare(theTickMarketOrder->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchBySellMBLData = NULL;
		return NULL;
	}
	if (compare(theTickMarketOrder->Tick.getValue(), Tick.getValue()))
	{
		pLastFoundInSearchBySellMBLData = NULL;
		return NULL;
	}

	return theTickMarketOrder;
}

CTickMarketOrder *CTickMarketOrderFactory::findNextBySellMBLData()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchBySellMBLData == NULL)
		return NULL;

	CTickMarketOrder *theTickMarketOrder;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchBySellMBLData = pSellMBLIndex->getNextNode(pLastFoundInSearchBySellMBLData);
	if (pLastFoundInSearchBySellMBLData == NULL)
		return NULL;

	theTickMarketOrder = (CTickMarketOrder *)(pLastFoundInSearchBySellMBLData->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(theTickMarketOrder->ExchangeID, queryExchangeIDInSearchBySellMBLData))
	{
		pLastFoundInSearchBySellMBLData = NULL;
		return NULL;
	}
	if (compare(theTickMarketOrder->InstrumentID, queryInstrumentIDInSearchBySellMBLData))
	{
		pLastFoundInSearchBySellMBLData = NULL;
		return NULL;
	}
	if (compare(theTickMarketOrder->Tick, queryTickInSearchBySellMBLData))
	{
		pLastFoundInSearchBySellMBLData = NULL;
		return NULL;
	}

	///返回找到的结果
	return theTickMarketOrder;
}

CTickMarketOrder* CTickMarketOrderFactory::findByPrice(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Tick, const CReadOnlyPriceType& Price)
{	
	CHashIndexNode *pNode;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Tick = Tick.getValue();
	compareObject.Price = Price.getValue();

	compareObject.calHashPriceHashKey();
	for (pNode = pPriceHashIndex->searchFirstEqual(compareObject.HashPriceHashKey);
		pNode != NULL;
		pNode = pPriceHashIndex->searchNextEqual(pNode))
	{
		if (((CTickMarketOrder *)(pNode->pObject))->HashPriceHashKey != compareObject.HashPriceHashKey)
			continue;
		if (compare(((CTickMarketOrder *)(pNode->pObject))->ExchangeID.getValue(), ExchangeID.getValue()) != 0)
			continue;
		if (compare(((CTickMarketOrder *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		if (compare(((CTickMarketOrder *)(pNode->pObject))->Tick.getValue(), Tick.getValue()) != 0)
			continue;
		if (compare(((CTickMarketOrder *)(pNode->pObject))->Price.getValue(), Price.getValue()) != 0)
			continue;
		return (CTickMarketOrder *)(pNode->pObject);
	}
	return NULL;
}

	
	
int compareForBarInsertTimeIndexinPublishKLine(const void *pV1, const void *pV2)
{
	CPublishKLine *p1, *p2;
	p1 = (CPublishKLine *)pV1;
	p2 = (CPublishKLine *)pV2;
	switch (compare(p1->ExchangeID.getValue(), p2->ExchangeID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InstrumentID.getValue(), p2->InstrumentID.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->Bar.getValue(), p2->Bar.getValue()))
	{
	case 1:
		return 1;
	case -1:
		return -1;
	case 0:
	default:
		break;
	}
	switch (compare(p1->InsertTime.getValue(), p2->InsertTime.getValue()))
	{
	case 1:
		return -1;
	case -1:
		return 1;
	case 0:
	default:
		break;
	}
	return 0;
}
	
CPublishKLine* CPublishKLineFactory::findByInstrumentBar(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar, const CReadOnlyMilliSecondsType& InsertTime)
{	
	CAVLNode *pNode;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Bar = Bar.getValue();
	compareObject.InsertTime = InsertTime.getValue();
	pNode = pBarInsertTimeIndex->searchFirstEqual(&compareObject);
	if (pNode == NULL)
		return NULL;
	return (CPublishKLine *)(pNode->pObject);
}

CPublishKLine *CPublishKLineFactory::findByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar, const CReadOnlyMilliSecondsType& InsertTime)
{
	return findByInstrumentBar(ExchangeID, InstrumentID, Bar, InsertTime);
}

bool CPublishKLineFactory::removeByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar, const CReadOnlyMilliSecondsType& InsertTime, CTransaction *pTransaction)
{
	CPublishKLine *pOriginalPublishKLine;
	pOriginalPublishKLine = findByInstrumentBar(ExchangeID, InstrumentID, Bar, InsertTime);
	if (pOriginalPublishKLine == NULL)
		return false;
	remove(pOriginalPublishKLine, pTransaction);
	return true;
}

CPublishKLine *CPublishKLineFactory::findByPK(CPublishKLineField *pPublishKLine)
{
	return findByPK(pPublishKLine->ExchangeID, pPublishKLine->InstrumentID, pPublishKLine->Bar, pPublishKLine->InsertTime);
}

CPublishKLine *CPublishKLineFactory::findByPK(CPublishKLine *pPublishKLine)
{
	return findByPK(pPublishKLine->ExchangeID, pPublishKLine->InstrumentID, pPublishKLine->Bar, pPublishKLine->InsertTime);
}

bool CPublishKLineFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "ExchangeID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	if (!strcmp(pName, "Bar"))
		return true;
	if (!strcmp(pName, "InsertTime"))
		return true;
	return false;
}

bool CPublishKLineFactory::retrieveByPK(CWriteablePublishKLine *pPublishKLine)
{
	CPublishKLine *pOriginalPublishKLine;
	pOriginalPublishKLine = findByPK(pPublishKLine);
	if (pOriginalPublishKLine == NULL)
		return false;
	retrieve(pOriginalPublishKLine, pPublishKLine);
	return true;
}

CPublishKLine *CPublishKLineFactory::addByPK(CWriteablePublishKLine *pPublishKLine, CTransaction *pTransaction)
{
	CPublishKLine *pOriginalPublishKLine;
	pOriginalPublishKLine = findByPK(pPublishKLine);
	if (pOriginalPublishKLine != NULL)
		return NULL;
	return add(pPublishKLine, pTransaction);
}

bool CPublishKLineFactory::updateByPK(CWriteablePublishKLine *pPublishKLine, CTransaction *pTransaction)
{
	CPublishKLine *pOriginalPublishKLine;
	pOriginalPublishKLine = findByPK(pPublishKLine);
	if (pOriginalPublishKLine == NULL)
		return false;
	pPublishKLine->calAllHash();
	update(pOriginalPublishKLine, pPublishKLine, pTransaction);
	return true;
}

CPublishKLine* CPublishKLineFactory::addOrUpdateByPK(CWriteablePublishKLine *pPublishKLine, CTransaction *pTransaction)
{
	CPublishKLine *pOriginalPublishKLine;
	pOriginalPublishKLine = findByPK(pPublishKLine);
	if (pOriginalPublishKLine == NULL)
		return add(pPublishKLine, pTransaction);
	pPublishKLine->calAllHash();
	update(pOriginalPublishKLine, pPublishKLine, pTransaction);
	return pOriginalPublishKLine;
}

bool CPublishKLineFactory::removeByPK(CPublishKLine *pPublishKLine, CTransaction *pTransaction)
{
	CPublishKLine *pOriginalPublishKLine;
	pOriginalPublishKLine = findByPK(pPublishKLine);
	if (pOriginalPublishKLine == NULL)
		return false;
	remove(pOriginalPublishKLine, pTransaction);
	return true;
}

bool CPublishKLineFactory::removeByPK(CPublishKLineField *pPublishKLine, CTransaction *pTransaction)
{
	CPublishKLine *pOriginalPublishKLine;
	pOriginalPublishKLine = findByPK(pPublishKLine);
	if (pOriginalPublishKLine == NULL)
		return false;
	CopyPublishKLineEntity(pPublishKLine, pOriginalPublishKLine);
	remove(pOriginalPublishKLine, pTransaction);
	return true;
}



CPublishKLine* CPublishKLineFactory::findFirstByBar(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar)
{
	pLastFoundInSearchByBar = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchByBar = ExchangeID.getValue();
	queryInstrumentIDInSearchByBar = InstrumentID.getValue();
	queryBarInSearchByBar = Bar.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CPublishKLine *thePublishKLine;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Bar = Bar.getValue();
	compareObject.InsertTime = 99999999999999.0;

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByBar = pBarInsertTimeIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByBar == NULL)
		return NULL;

	///获取找到的对象
	thePublishKLine = (CPublishKLine *)(pLastFoundInSearchByBar->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePublishKLine->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchByBar = NULL;
		return NULL;
	}
	if (compare(thePublishKLine->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByBar = NULL;
		return NULL;
	}
	if (compare(thePublishKLine->Bar.getValue(), Bar.getValue()))
	{
		pLastFoundInSearchByBar = NULL;
		return NULL;
	}

	return thePublishKLine;
}

CPublishKLine *CPublishKLineFactory::findNextByBar()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByBar == NULL)
		return NULL;

	CPublishKLine *thePublishKLine;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByBar = pBarInsertTimeIndex->getNextNode(pLastFoundInSearchByBar);
	if (pLastFoundInSearchByBar == NULL)
		return NULL;

	thePublishKLine = (CPublishKLine *)(pLastFoundInSearchByBar->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePublishKLine->ExchangeID, queryExchangeIDInSearchByBar))
	{
		pLastFoundInSearchByBar = NULL;
		return NULL;
	}
	if (compare(thePublishKLine->InstrumentID, queryInstrumentIDInSearchByBar))
	{
		pLastFoundInSearchByBar = NULL;
		return NULL;
	}
	if (compare(thePublishKLine->Bar, queryBarInSearchByBar))
	{
		pLastFoundInSearchByBar = NULL;
		return NULL;
	}

	///返回找到的结果
	return thePublishKLine;
}




CPublishKLine* CPublishKLineFactory::findFirstByBarInsertTime(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar, const CReadOnlyMilliSecondsType& InsertTime)
{
	pLastFoundInSearchByBarInsertTime = NULL;
	///记录所有的查询值
	queryExchangeIDInSearchByBarInsertTime = ExchangeID.getValue();
	queryInstrumentIDInSearchByBarInsertTime = InstrumentID.getValue();
	queryBarInSearchByBarInsertTime = Bar.getValue();
	queryInsertTimeInSearchByBarInsertTime = InsertTime.getValue();
	///根据参数和缺省值，设置用于比较的对象的各个分量
	CPublishKLine *thePublishKLine;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Bar = Bar.getValue();
	compareObject.InsertTime = InsertTime.getValue();

	///在索引中寻找第一个满足条件的
	pLastFoundInSearchByBarInsertTime = pBarInsertTimeIndex->searchFirstGreatEqual(&compareObject);

	///如果没有找到，则返回
	if (pLastFoundInSearchByBarInsertTime == NULL)
		return NULL;

	///获取找到的对象
	thePublishKLine = (CPublishKLine *)(pLastFoundInSearchByBarInsertTime->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePublishKLine->ExchangeID.getValue(), ExchangeID.getValue()))
	{
		pLastFoundInSearchByBarInsertTime = NULL;
		return NULL;
	}
	if (compare(thePublishKLine->InstrumentID.getValue(), InstrumentID.getValue()))
	{
		pLastFoundInSearchByBarInsertTime = NULL;
		return NULL;
	}
	if (compare(thePublishKLine->Bar.getValue(), Bar.getValue()))
	{
		pLastFoundInSearchByBarInsertTime = NULL;
		return NULL;
	}

	return thePublishKLine;
}

CPublishKLine *CPublishKLineFactory::findNextByBarInsertTime()
{
	///还没有开始查找，或者已经找到最后一个了，就立即返回
	if (pLastFoundInSearchByBarInsertTime == NULL)
		return NULL;

	CPublishKLine *thePublishKLine;

	///找下一个节点，如果没有，则查找结束
	pLastFoundInSearchByBarInsertTime = pBarInsertTimeIndex->getNextNode(pLastFoundInSearchByBarInsertTime);
	if (pLastFoundInSearchByBarInsertTime == NULL)
		return NULL;

	thePublishKLine = (CPublishKLine *)(pLastFoundInSearchByBarInsertTime->pObject);

	///如果有一个条件不满足，则失败返回
	if (compare(thePublishKLine->ExchangeID, queryExchangeIDInSearchByBarInsertTime))
	{
		pLastFoundInSearchByBarInsertTime = NULL;
		return NULL;
	}
	if (compare(thePublishKLine->InstrumentID, queryInstrumentIDInSearchByBarInsertTime))
	{
		pLastFoundInSearchByBarInsertTime = NULL;
		return NULL;
	}
	if (compare(thePublishKLine->Bar, queryBarInSearchByBarInsertTime))
	{
		pLastFoundInSearchByBarInsertTime = NULL;
		return NULL;
	}

	///返回找到的结果
	return thePublishKLine;
}

	
CLastKLine* CLastKLineFactory::findByInstrumentBar(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar)
{	
	CHashIndexNode *pNode;
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();
	compareObject.Bar = Bar.getValue();

	compareObject.calHashInstrumentBarHashKey();
	for (pNode = pInstrumentBarHashIndex->searchFirstEqual(compareObject.HashInstrumentBarHashKey);
		pNode != NULL;
		pNode = pInstrumentBarHashIndex->searchNextEqual(pNode))
	{
		if (((CLastKLine *)(pNode->pObject))->HashInstrumentBarHashKey != compareObject.HashInstrumentBarHashKey)
			continue;
		if (compare(((CLastKLine *)(pNode->pObject))->ExchangeID.getValue(), ExchangeID.getValue()) != 0)
			continue;
		if (compare(((CLastKLine *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		if (compare(((CLastKLine *)(pNode->pObject))->Bar.getValue(), Bar.getValue()) != 0)
			continue;
		return (CLastKLine *)(pNode->pObject);
	}
	return NULL;
}

CLastKLine *CLastKLineFactory::findByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar)
{
	return findByInstrumentBar(ExchangeID, InstrumentID, Bar);
}

bool CLastKLineFactory::removeByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar, CTransaction *pTransaction)
{
	CLastKLine *pOriginalLastKLine;
	pOriginalLastKLine = findByInstrumentBar(ExchangeID, InstrumentID, Bar);
	if (pOriginalLastKLine == NULL)
		return false;
	remove(pOriginalLastKLine, pTransaction);
	return true;
}

CLastKLine *CLastKLineFactory::findByPK(CLastKLineField *pLastKLine)
{
	return findByPK(pLastKLine->ExchangeID, pLastKLine->InstrumentID, pLastKLine->Bar);
}

CLastKLine *CLastKLineFactory::findByPK(CLastKLine *pLastKLine)
{
	return findByPK(pLastKLine->ExchangeID, pLastKLine->InstrumentID, pLastKLine->Bar);
}

bool CLastKLineFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "ExchangeID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	if (!strcmp(pName, "Bar"))
		return true;
	return false;
}

bool CLastKLineFactory::retrieveByPK(CWriteableLastKLine *pLastKLine)
{
	CLastKLine *pOriginalLastKLine;
	pOriginalLastKLine = findByPK(pLastKLine);
	if (pOriginalLastKLine == NULL)
		return false;
	retrieve(pOriginalLastKLine, pLastKLine);
	return true;
}

CLastKLine *CLastKLineFactory::addByPK(CWriteableLastKLine *pLastKLine, CTransaction *pTransaction)
{
	CLastKLine *pOriginalLastKLine;
	pOriginalLastKLine = findByPK(pLastKLine);
	if (pOriginalLastKLine != NULL)
		return NULL;
	return add(pLastKLine, pTransaction);
}

bool CLastKLineFactory::updateByPK(CWriteableLastKLine *pLastKLine, CTransaction *pTransaction)
{
	CLastKLine *pOriginalLastKLine;
	pOriginalLastKLine = findByPK(pLastKLine);
	if (pOriginalLastKLine == NULL)
		return false;
	pLastKLine->calAllHash();
	update(pOriginalLastKLine, pLastKLine, pTransaction);
	return true;
}

CLastKLine* CLastKLineFactory::addOrUpdateByPK(CWriteableLastKLine *pLastKLine, CTransaction *pTransaction)
{
	CLastKLine *pOriginalLastKLine;
	pOriginalLastKLine = findByPK(pLastKLine);
	if (pOriginalLastKLine == NULL)
		return add(pLastKLine, pTransaction);
	pLastKLine->calAllHash();
	update(pOriginalLastKLine, pLastKLine, pTransaction);
	return pOriginalLastKLine;
}

bool CLastKLineFactory::removeByPK(CLastKLine *pLastKLine, CTransaction *pTransaction)
{
	CLastKLine *pOriginalLastKLine;
	pOriginalLastKLine = findByPK(pLastKLine);
	if (pOriginalLastKLine == NULL)
		return false;
	remove(pOriginalLastKLine, pTransaction);
	return true;
}

bool CLastKLineFactory::removeByPK(CLastKLineField *pLastKLine, CTransaction *pTransaction)
{
	CLastKLine *pOriginalLastKLine;
	pOriginalLastKLine = findByPK(pLastKLine);
	if (pOriginalLastKLine == NULL)
		return false;
	CopyLastKLineEntity(pLastKLine, pOriginalLastKLine);
	remove(pOriginalLastKLine, pTransaction);
	return true;
}
	
	
CSettleDetail* CSettleDetailFactory::findByInstrument(const CReadOnlyIndexIDType& SettleDetailID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{	
	CHashIndexNode *pNode;
	compareObject.SettleDetailID = SettleDetailID.getValue();
	compareObject.ExchangeID = ExchangeID.getValue();
	compareObject.InstrumentID = InstrumentID.getValue();

	compareObject.calHashInstrumentHashKey();
	for (pNode = pInstrumentHashIndex->searchFirstEqual(compareObject.HashInstrumentHashKey);
		pNode != NULL;
		pNode = pInstrumentHashIndex->searchNextEqual(pNode))
	{
		if (((CSettleDetail *)(pNode->pObject))->HashInstrumentHashKey != compareObject.HashInstrumentHashKey)
			continue;
		if (compare(((CSettleDetail *)(pNode->pObject))->SettleDetailID.getValue(), SettleDetailID.getValue()) != 0)
			continue;
		if (compare(((CSettleDetail *)(pNode->pObject))->ExchangeID.getValue(), ExchangeID.getValue()) != 0)
			continue;
		if (compare(((CSettleDetail *)(pNode->pObject))->InstrumentID.getValue(), InstrumentID.getValue()) != 0)
			continue;
		return (CSettleDetail *)(pNode->pObject);
	}
	return NULL;
}

CSettleDetail *CSettleDetailFactory::findByPK(const CReadOnlyIndexIDType& SettleDetailID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID)
{
	return findByInstrument(SettleDetailID, ExchangeID, InstrumentID);
}

bool CSettleDetailFactory::removeByPK(const CReadOnlyIndexIDType& SettleDetailID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, CTransaction *pTransaction)
{
	CSettleDetail *pOriginalSettleDetail;
	pOriginalSettleDetail = findByInstrument(SettleDetailID, ExchangeID, InstrumentID);
	if (pOriginalSettleDetail == NULL)
		return false;
	remove(pOriginalSettleDetail, pTransaction);
	return true;
}

CSettleDetail *CSettleDetailFactory::findByPK(CSettleDetailField *pSettleDetail)
{
	return findByPK(pSettleDetail->SettleDetailID, pSettleDetail->ExchangeID, pSettleDetail->InstrumentID);
}

CSettleDetail *CSettleDetailFactory::findByPK(CSettleDetail *pSettleDetail)
{
	return findByPK(pSettleDetail->SettleDetailID, pSettleDetail->ExchangeID, pSettleDetail->InstrumentID);
}

bool CSettleDetailFactory::isPK(const char* pName)
{
	if (!strcmp(pName, "SettleDetailID"))
		return true;
	if (!strcmp(pName, "ExchangeID"))
		return true;
	if (!strcmp(pName, "InstrumentID"))
		return true;
	return false;
}

bool CSettleDetailFactory::retrieveByPK(CWriteableSettleDetail *pSettleDetail)
{
	CSettleDetail *pOriginalSettleDetail;
	pOriginalSettleDetail = findByPK(pSettleDetail);
	if (pOriginalSettleDetail == NULL)
		return false;
	retrieve(pOriginalSettleDetail, pSettleDetail);
	return true;
}

CSettleDetail *CSettleDetailFactory::addByPK(CWriteableSettleDetail *pSettleDetail, CTransaction *pTransaction)
{
	CSettleDetail *pOriginalSettleDetail;
	pOriginalSettleDetail = findByPK(pSettleDetail);
	if (pOriginalSettleDetail != NULL)
		return NULL;
	return add(pSettleDetail, pTransaction);
}

bool CSettleDetailFactory::updateByPK(CWriteableSettleDetail *pSettleDetail, CTransaction *pTransaction)
{
	CSettleDetail *pOriginalSettleDetail;
	pOriginalSettleDetail = findByPK(pSettleDetail);
	if (pOriginalSettleDetail == NULL)
		return false;
	pSettleDetail->calAllHash();
	update(pOriginalSettleDetail, pSettleDetail, pTransaction);
	return true;
}

CSettleDetail* CSettleDetailFactory::addOrUpdateByPK(CWriteableSettleDetail *pSettleDetail, CTransaction *pTransaction)
{
	CSettleDetail *pOriginalSettleDetail;
	pOriginalSettleDetail = findByPK(pSettleDetail);
	if (pOriginalSettleDetail == NULL)
		return add(pSettleDetail, pTransaction);
	pSettleDetail->calAllHash();
	update(pOriginalSettleDetail, pSettleDetail, pTransaction);
	return pOriginalSettleDetail;
}

bool CSettleDetailFactory::removeByPK(CSettleDetail *pSettleDetail, CTransaction *pTransaction)
{
	CSettleDetail *pOriginalSettleDetail;
	pOriginalSettleDetail = findByPK(pSettleDetail);
	if (pOriginalSettleDetail == NULL)
		return false;
	remove(pOriginalSettleDetail, pTransaction);
	return true;
}

bool CSettleDetailFactory::removeByPK(CSettleDetailField *pSettleDetail, CTransaction *pTransaction)
{
	CSettleDetail *pOriginalSettleDetail;
	pOriginalSettleDetail = findByPK(pSettleDetail);
	if (pOriginalSettleDetail == NULL)
		return false;
	CopySettleDetailEntity(pSettleDetail, pOriginalSettleDetail);
	remove(pOriginalSettleDetail, pTransaction);
	return true;
}
