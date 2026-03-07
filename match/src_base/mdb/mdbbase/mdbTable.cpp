#include "mdbStruct.h"
#include "mdbTable.h"
#include "mdbAction.h"
#include "monitorIndex.h"

///强制进行复制，不理会const标记
///@param	target	复制的目标地址
///@param	source	复制的源地址
///@param	size	复制的空间大小
static void forceCopy(const void *target, const void *source, int size)
{
	if ((source==NULL)||(target==NULL)){
		REPORT_EVENT(LOG_CRITICAL,"InternalError","copy from or to null");
		return;
	}
	memcpy((void *)target,source,size);
}

extern int compareForSystemIDIndexinCurrentTime(const void *pV1, const void *pV2);
void CCurrentTimeFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pSystemIDIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		if(!reuse) {
			pSystemIDIndex=new CAVLTree(maxUnit,compareForSystemIDIndexinCurrentTime,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("CurrentTime_SystemIDIndex",pSystemIDIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("CurrentTime_SystemIDIndex");
			if(it != pIndexMap->end()) {
				pSystemIDIndex=new CAVLTree(maxUnit,compareForSystemIDIndexinCurrentTime,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pSystemIDIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pActionTriggers=new vector<CCurrentTimeActionTrigger *>;
	pCommitTriggers=new vector<CCurrentTimeCommitTrigger *>;
}

CCurrentTimeFactory::CCurrentTimeFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CCurrentTime),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CCurrentTimeFactory::CCurrentTimeFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CCurrentTime),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CCurrentTimeFactory::~CCurrentTimeFactory()
{
	if (pSystemIDIndex!=NULL)
		delete pSystemIDIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CCurrentTimeFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableCurrentTime thisCurrentTime;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisCurrentTime.readCSV(input,pNames))
		add(&thisCurrentTime);
	fclose(input);
	delete pNames;
	return 1;
}

int CCurrentTimeFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "CurrentTime.csv");
	return readCSV(szFileName);
}

int CCurrentTimeFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableCurrentTime *pCurrentTime;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableCurrentTime::writeCSVHead(output);
	pCurrentTime=(CWriteableCurrentTime *)(pMem->getFirst());
	while (pCurrentTime!=NULL) {
		pCurrentTime->writeCSV(output);
		pCurrentTime=(CWriteableCurrentTime *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CCurrentTimeFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableCurrentTime *pCurrentTime;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableCurrentTime::writeCSVHeadPreVersion(output);
	pCurrentTime=(CWriteableCurrentTime *)(pMem->getFirst());
	while (pCurrentTime!=NULL) {
		pCurrentTime->writeCSVPreVersion(output);
		pCurrentTime=(CWriteableCurrentTime *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CCurrentTimeFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "CurrentTime.csv");
	return writeCSV(szFileName);
}

void CCurrentTimeFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CCurrentTimeFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableCurrentTime *pCurrentTime;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CCurrentTimeFactory={       Total Count=%d\n", pMem->getCount());
	pCurrentTime=(CWriteableCurrentTime *)(pMem->getFirst());
	while (pCurrentTime!=NULL) {
		pCurrentTime->dump(fp,index++);
		pCurrentTime=(CWriteableCurrentTime *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CCurrentTimeFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pSystemIDIndex != NULL)
		pSystemIDIndex->removeAll();
}

CCurrentTime *CCurrentTimeFactory::internalAdd(CWriteableCurrentTime *pCurrentTime, bool bNoTransaction)
{
	pCurrentTime->m_pMDB = m_pMDB;
	if(pCurrentTime->InsertTime.isNull())
		pCurrentTime->InsertTime = g_nCurrentTime;
	CCurrentTime *pTarget;
	//beforeAdd(pCurrentTime);
	pTarget=(CCurrentTime *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough CurrentTime in memory database");
		return NULL;
	}
	forceCopy(pTarget, pCurrentTime, sizeof(CCurrentTime));
	pMem->updateObject(pTarget);
	if (pSystemIDIndex != NULL)
		pSystemIDIndex->addObject(pTarget);
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CCurrentTime* CCurrentTimeFactory::add(CWriteableCurrentTime *pCurrentTime, CTransaction *pTransaction)
{
	pCurrentTime->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pCurrentTime,true);
	}else{
		CCurrentTime *pNewCurrentTime;
		pNewCurrentTime = internalAdd(pCurrentTime,false);
		pTransaction->addResource(CCurrentTimeResource::alloc(CREATE_ACTION,this,pNewCurrentTime));
		return pNewCurrentTime;
	}
}

CCurrentTime* CCurrentTimeFactory::internalUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pNewCurrentTime,bool bNoTransaction)
{
	pNewCurrentTime->UpdateTime = g_nCurrentTime;
	CWriteableCurrentTime theOldCurrentTime;
	if (bNoTransaction)
		forceCopy(&theOldCurrentTime,pCurrentTime,sizeof(CCurrentTime));	
	CCurrentTime *pTarget = pCurrentTime;
	forceCopy(pCurrentTime,pNewCurrentTime,sizeof(CCurrentTime));
	pMem->updateObject(pCurrentTime);
	if (bNoTransaction)
		commitUpdate(pCurrentTime,&theOldCurrentTime);
	return pCurrentTime;
}

CCurrentTime* CCurrentTimeFactory::update(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pNewCurrentTime, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pCurrentTime == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pCurrentTime,pNewCurrentTime,bNoTransaction);
	}else{
		pTransaction->addResource(CCurrentTimeResource::alloc(UPDATE_ACTION,this,pCurrentTime));
		return internalUpdate(pCurrentTime,pNewCurrentTime,false);
	}
}

void CCurrentTimeFactory::internalRemove(CCurrentTime *pCurrentTime, bool bNoTransaction)
{
	CWriteableCurrentTime theOldCurrentTime;
	//beforeRemove(pCurrentTime);
	if (bNoTransaction)
		forceCopy(&theOldCurrentTime,pCurrentTime,sizeof(CCurrentTime));
	CCurrentTime *pTarget = pCurrentTime;
	if (pSystemIDIndex != NULL)
		pSystemIDIndex->removeObject(pCurrentTime);	
	
	pMem->free(pCurrentTime);
	if(bNoTransaction)
		commitRemove(&theOldCurrentTime);
}

void CCurrentTimeFactory::remove(CCurrentTime *pCurrentTime, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pCurrentTime == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pCurrentTime, bNoTransaction);
	}else{
		pTransaction->addResource(CCurrentTimeResource::alloc(DELETE_ACTION,this,pCurrentTime));
		internalRemove(pCurrentTime,false);
	}
}

CCurrentTime* CCurrentTimeFactory::addOrUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pNewCurrentTime, CTransaction *pTransaction)
{
	if(pCurrentTime == NULL)
		return add(pNewCurrentTime,pTransaction);
	else
		return update(pCurrentTime,pNewCurrentTime,pTransaction);
}

void CCurrentTimeFactory::retrieve(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pTargetCurrentTime)
{
	if(pCurrentTime == NULL)
		return;
	forceCopy(pTargetCurrentTime, pCurrentTime, sizeof(CCurrentTime));
}
	
int CCurrentTimeFactory::addActionTrigger(CCurrentTimeActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CCurrentTimeFactory::removeActionTrigger(CCurrentTimeActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CCurrentTimeFactory::addCommitTrigger(CCurrentTimeCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CCurrentTimeFactory::removeCommitTrigger(CCurrentTimeCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CCurrentTime* CCurrentTimeFactory::getFirst()
{
	return (CCurrentTime *)(pMem->getFirst());
}
	
CCurrentTime* CCurrentTimeFactory::getNext()
{
	return (CCurrentTime *)(pMem->getNext());
}

void CCurrentTimeFactory::commitAdd(CCurrentTime *pCurrentTime)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pCurrentTime);
	}
}

void CCurrentTimeFactory::commitUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pOldCurrentTime)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pCurrentTime,pOldCurrentTime);
	}
}
	
void CCurrentTimeFactory::commitRemove(CWriteableCurrentTime *pCurrentTime)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pCurrentTime);
	}
}

void CCurrentTimeFactory::checkNull(FILE *pFile)
{
	CCurrentTime *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->SystemID.isNull()){
			bNull = true;
			fprintf(pFile, "CurrentTime->SystemID:NULL\n");
		}
		if(pTarget->ZoneDate.isNull()){
			bNull = true;
			fprintf(pFile, "CurrentTime->ZoneDate:NULL\n");
		}
		if(pTarget->ZoneTime.isNull()){
			bNull = true;
			fprintf(pFile, "CurrentTime->ZoneTime:NULL\n");
		}
		if(pTarget->ZoneDays.isNull()){
			bNull = true;
			fprintf(pFile, "CurrentTime->ZoneDays:NULL\n");
		}
		if(pTarget->CurrTime.isNull()){
			bNull = true;
			fprintf(pFile, "CurrentTime->CurrTime:NULL\n");
		}
		if(pTarget->TimeZone.isNull()){
			bNull = true;
			fprintf(pFile, "CurrentTime->TimeZone:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CCurrentTimeFactory::checkValid(FILE *pFile)
{
	CCurrentTime *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->SystemID.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->SystemID:inValid\n");
		}
		if(!pTarget->ZoneDate.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->ZoneDate:inValid\n");
		}
		if(!pTarget->ZoneTime.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->ZoneTime:inValid\n");
		}
		if(!pTarget->ZoneDays.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->ZoneDays:inValid\n");
		}
		if(!pTarget->CurrTime.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->CurrTime:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->UpdateTime:inValid\n");
		}
		if(!pTarget->TimeZone.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->TimeZone:inValid\n");
		}
		if(!pTarget->LastTradeID.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->LastTradeID:inValid\n");
		}
		if(!pTarget->LastOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->LastOrderID:inValid\n");
		}
		if(!pTarget->LastAccountDetailID.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->LastAccountDetailID:inValid\n");
		}
		if(!pTarget->LastTimeSortNo.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->LastTimeSortNo:inValid\n");
		}
		if(!pTarget->LastBusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "CurrentTime->LastBusinessNo:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CMemberFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pMemberIDHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 'r':
	case 'p':
	{
		int hashKeySize=1572869;
		if(!reuse) {
			pMemberIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Member_MemberIDHashIndex",pMemberIDHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("Member_MemberIDHashIndex");
			if(it != pIndexMap->end()) {
				pMemberIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pMemberIDHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pActionTriggers=new vector<CMemberActionTrigger *>;
	pCommitTriggers=new vector<CMemberCommitTrigger *>;
}

CMemberFactory::CMemberFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CMember),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CMemberFactory::CMemberFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CMember),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CMemberFactory::~CMemberFactory()
{
	if (pMemberIDHashIndex!=NULL)
		delete pMemberIDHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CMemberFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableMember thisMember;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisMember.readCSV(input,pNames))
		add(&thisMember);
	fclose(input);
	delete pNames;
	return 1;
}

int CMemberFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Member.csv");
	return readCSV(szFileName);
}

int CMemberFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableMember *pMember;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableMember::writeCSVHead(output);
	pMember=(CWriteableMember *)(pMem->getFirst());
	while (pMember!=NULL) {
		pMember->writeCSV(output);
		pMember=(CWriteableMember *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CMemberFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableMember *pMember;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableMember::writeCSVHeadPreVersion(output);
	pMember=(CWriteableMember *)(pMem->getFirst());
	while (pMember!=NULL) {
		pMember->writeCSVPreVersion(output);
		pMember=(CWriteableMember *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CMemberFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Member.csv");
	return writeCSV(szFileName);
}

void CMemberFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CMemberFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableMember *pMember;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CMemberFactory={       Total Count=%d\n", pMem->getCount());
	pMember=(CWriteableMember *)(pMem->getFirst());
	while (pMember!=NULL) {
		pMember->dump(fp,index++);
		pMember=(CWriteableMember *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CMemberFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pMemberIDHashIndex != NULL)
		pMemberIDHashIndex->removeAll();
}

CMember *CMemberFactory::internalAdd(CWriteableMember *pMember, bool bNoTransaction)
{
	pMember->m_pMDB = m_pMDB;
	if(pMember->TriggerOrders.isNull())
		pMember->TriggerOrders = 0;
	if(pMember->OpenOrders.isNull())
		pMember->OpenOrders = 0;
	if(pMember->OpenPositions.isNull())
		pMember->OpenPositions = 0;
	if(pMember->InsertTime.isNull())
		pMember->InsertTime = g_nCurrentTime;
	CMember *pTarget;
	//beforeAdd(pMember);
	pTarget=(CMember *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough Member in memory database");
		return NULL;
	}
	forceCopy(pTarget, pMember, sizeof(CMember));
	pMem->updateObject(pTarget);
	if (pMemberIDHashIndex != NULL)
		pMemberIDHashIndex->addObject(pTarget,pTarget->HashMemberID);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CMember* CMemberFactory::add(CWriteableMember *pMember, CTransaction *pTransaction)
{
	pMember->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pMember,true);
	}else{
		CMember *pNewMember;
		pNewMember = internalAdd(pMember,false);
		pTransaction->addResource(CMemberResource::alloc(CREATE_ACTION,this,pNewMember));
		return pNewMember;
	}
}

CMember* CMemberFactory::internalUpdate(CMember *pMember, CWriteableMember *pNewMember,bool bNoTransaction)
{
	pNewMember->UpdateTime = g_nCurrentTime;
	CWriteableMember theOldMember;
	if (bNoTransaction)
		forceCopy(&theOldMember,pMember,sizeof(CMember));	
	CMember *pTarget = pMember;
	forceCopy(pMember,pNewMember,sizeof(CMember));
	pMem->updateObject(pMember);
	if (bNoTransaction)
		commitUpdate(pMember,&theOldMember);
	return pMember;
}

CMember* CMemberFactory::update(CMember *pMember, CWriteableMember *pNewMember, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pMember == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pMember,pNewMember,bNoTransaction);
	}else{
		pTransaction->addResource(CMemberResource::alloc(UPDATE_ACTION,this,pMember));
		return internalUpdate(pMember,pNewMember,false);
	}
}

void CMemberFactory::internalRemove(CMember *pMember, bool bNoTransaction)
{
	CWriteableMember theOldMember;
	//beforeRemove(pMember);
	if (bNoTransaction)
		forceCopy(&theOldMember,pMember,sizeof(CMember));
	CMember *pTarget = pMember;
	
	if (pMemberIDHashIndex != NULL)
		pMemberIDHashIndex->removeObject(pMember,pMember->HashMemberID);		
	pMem->free(pMember);
	if(bNoTransaction)
		commitRemove(&theOldMember);
}

void CMemberFactory::remove(CMember *pMember, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pMember == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pMember, bNoTransaction);
	}else{
		pTransaction->addResource(CMemberResource::alloc(DELETE_ACTION,this,pMember));
		internalRemove(pMember,false);
	}
}

CMember* CMemberFactory::addOrUpdate(CMember *pMember, CWriteableMember *pNewMember, CTransaction *pTransaction)
{
	if(pMember == NULL)
		return add(pNewMember,pTransaction);
	else
		return update(pMember,pNewMember,pTransaction);
}

void CMemberFactory::retrieve(CMember *pMember, CWriteableMember *pTargetMember)
{
	if(pMember == NULL)
		return;
	forceCopy(pTargetMember, pMember, sizeof(CMember));
}
	
int CMemberFactory::addActionTrigger(CMemberActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CMemberFactory::removeActionTrigger(CMemberActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CMemberFactory::addCommitTrigger(CMemberCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CMemberFactory::removeCommitTrigger(CMemberCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CMember* CMemberFactory::getFirst()
{
	return (CMember *)(pMem->getFirst());
}
	
CMember* CMemberFactory::getNext()
{
	return (CMember *)(pMem->getNext());
}

void CMemberFactory::commitAdd(CMember *pMember)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pMember);
	}
}

void CMemberFactory::commitUpdate(CMember *pMember, CWriteableMember *pOldMember)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pMember,pOldMember);
	}
}
	
void CMemberFactory::commitRemove(CWriteableMember *pMember)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pMember);
	}
}

void CMemberFactory::checkNull(FILE *pFile)
{
	CMember *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "Member->MemberID:NULL\n");
		}
		if(pTarget->RiskLevel.isNull()){
			bNull = true;
			fprintf(pFile, "Member->RiskLevel:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CMemberFactory::checkValid(FILE *pFile)
{
	CMember *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "Member->MemberID:inValid\n");
		}
		if(!pTarget->MemberType.isValid()){
			bValid = false;
			fprintf(pFile, "Member->MemberType:inValid\n");
		}
		if(!pTarget->Grade.isValid()){
			bValid = false;
			fprintf(pFile, "Member->Grade:inValid\n");
		}
		if(!pTarget->Priority.isValid()){
			bValid = false;
			fprintf(pFile, "Member->Priority:inValid\n");
		}
		if(!pTarget->FeeGrade.isValid()){
			bValid = false;
			fprintf(pFile, "Member->FeeGrade:inValid\n");
		}
		if(!pTarget->MarginRateGrade.isValid()){
			bValid = false;
			fprintf(pFile, "Member->MarginRateGrade:inValid\n");
		}
		if(!pTarget->TradingRightGrade.isValid()){
			bValid = false;
			fprintf(pFile, "Member->TradingRightGrade:inValid\n");
		}
		if(!pTarget->CFDGrade.isValid()){
			bValid = false;
			fprintf(pFile, "Member->CFDGrade:inValid\n");
		}
		if(!pTarget->LiquidityGrade.isValid()){
			bValid = false;
			fprintf(pFile, "Member->LiquidityGrade:inValid\n");
		}
		if(!pTarget->RiskLevel.isValid()){
			bValid = false;
			fprintf(pFile, "Member->RiskLevel:inValid\n");
		}
		if(!pTarget->PositionType.isValid()){
			bValid = false;
			fprintf(pFile, "Member->PositionType:inValid\n");
		}
		if(!pTarget->TriggerOrders.isValid()){
			bValid = false;
			fprintf(pFile, "Member->TriggerOrders:inValid\n");
		}
		if(!pTarget->OpenOrders.isValid()){
			bValid = false;
			fprintf(pFile, "Member->OpenOrders:inValid\n");
		}
		if(!pTarget->OpenPositions.isValid()){
			bValid = false;
			fprintf(pFile, "Member->OpenPositions:inValid\n");
		}
		if(!pTarget->MaxLocalID.isValid()){
			bValid = false;
			fprintf(pFile, "Member->MaxLocalID:inValid\n");
		}
		if(!pTarget->SinkType.isValid()){
			bValid = false;
			fprintf(pFile, "Member->SinkType:inValid\n");
		}
		if(!pTarget->CopyMemberID.isValid()){
			bValid = false;
			fprintf(pFile, "Member->CopyMemberID:inValid\n");
		}
		if(!pTarget->Region.isValid()){
			bValid = false;
			fprintf(pFile, "Member->Region:inValid\n");
		}
		if(!pTarget->FeeDeduceDisCount.isValid()){
			bValid = false;
			fprintf(pFile, "Member->FeeDeduceDisCount:inValid\n");
		}
		if(!pTarget->FeeDeduceCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Member->FeeDeduceCurrency:inValid\n");
		}
		if(!pTarget->CanBorrow.isValid()){
			bValid = false;
			fprintf(pFile, "Member->CanBorrow:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "Member->Remark:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "Member->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Member->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

extern int compareForConfigIndexinServiceConfig(const void *pV1, const void *pV2);
void CServiceConfigFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pConfigIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		if(!reuse) {
			pConfigIndex=new CAVLTree(maxUnit,compareForConfigIndexinServiceConfig,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("ServiceConfig_ConfigIndex",pConfigIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("ServiceConfig_ConfigIndex");
			if(it != pIndexMap->end()) {
				pConfigIndex=new CAVLTree(maxUnit,compareForConfigIndexinServiceConfig,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pConfigIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pLastFoundInSearchByIndex1=NULL;
	pActionTriggers=new vector<CServiceConfigActionTrigger *>;
	pCommitTriggers=new vector<CServiceConfigCommitTrigger *>;
}

CServiceConfigFactory::CServiceConfigFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CServiceConfig),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CServiceConfigFactory::CServiceConfigFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CServiceConfig),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CServiceConfigFactory::~CServiceConfigFactory()
{
	if (pConfigIndex!=NULL)
		delete pConfigIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CServiceConfigFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableServiceConfig thisServiceConfig;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisServiceConfig.readCSV(input,pNames))
		add(&thisServiceConfig);
	fclose(input);
	delete pNames;
	return 1;
}

int CServiceConfigFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "ServiceConfig.csv");
	return readCSV(szFileName);
}

int CServiceConfigFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableServiceConfig *pServiceConfig;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableServiceConfig::writeCSVHead(output);
	pServiceConfig=(CWriteableServiceConfig *)(pMem->getFirst());
	while (pServiceConfig!=NULL) {
		pServiceConfig->writeCSV(output);
		pServiceConfig=(CWriteableServiceConfig *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CServiceConfigFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableServiceConfig *pServiceConfig;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableServiceConfig::writeCSVHeadPreVersion(output);
	pServiceConfig=(CWriteableServiceConfig *)(pMem->getFirst());
	while (pServiceConfig!=NULL) {
		pServiceConfig->writeCSVPreVersion(output);
		pServiceConfig=(CWriteableServiceConfig *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CServiceConfigFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "ServiceConfig.csv");
	return writeCSV(szFileName);
}

void CServiceConfigFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CServiceConfigFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableServiceConfig *pServiceConfig;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CServiceConfigFactory={       Total Count=%d\n", pMem->getCount());
	pServiceConfig=(CWriteableServiceConfig *)(pMem->getFirst());
	while (pServiceConfig!=NULL) {
		pServiceConfig->dump(fp,index++);
		pServiceConfig=(CWriteableServiceConfig *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CServiceConfigFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pConfigIndex != NULL)
		pConfigIndex->removeAll();
}

CServiceConfig *CServiceConfigFactory::internalAdd(CWriteableServiceConfig *pServiceConfig, bool bNoTransaction)
{
	pServiceConfig->m_pMDB = m_pMDB;
	if(pServiceConfig->Index1.isNull())
		pServiceConfig->Index1 = "default";
	if(pServiceConfig->Index2.isNull())
		pServiceConfig->Index2 = "default";
	if(pServiceConfig->InsertTime.isNull())
		pServiceConfig->InsertTime = g_nCurrentTime;
	CServiceConfig *pTarget;
	//beforeAdd(pServiceConfig);
	pTarget=(CServiceConfig *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough ServiceConfig in memory database");
		return NULL;
	}
	forceCopy(pTarget, pServiceConfig, sizeof(CServiceConfig));
	pMem->updateObject(pTarget);
	if (pConfigIndex != NULL)
		pConfigIndex->addObject(pTarget);
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CServiceConfig* CServiceConfigFactory::add(CWriteableServiceConfig *pServiceConfig, CTransaction *pTransaction)
{
	pServiceConfig->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pServiceConfig,true);
	}else{
		CServiceConfig *pNewServiceConfig;
		pNewServiceConfig = internalAdd(pServiceConfig,false);
		pTransaction->addResource(CServiceConfigResource::alloc(CREATE_ACTION,this,pNewServiceConfig));
		return pNewServiceConfig;
	}
}

CServiceConfig* CServiceConfigFactory::internalUpdate(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pNewServiceConfig,bool bNoTransaction)
{
	pNewServiceConfig->UpdateTime = g_nCurrentTime;
	CWriteableServiceConfig theOldServiceConfig;
	if (bNoTransaction)
		forceCopy(&theOldServiceConfig,pServiceConfig,sizeof(CServiceConfig));	
	CServiceConfig *pTarget = pServiceConfig;
	forceCopy(pServiceConfig,pNewServiceConfig,sizeof(CServiceConfig));
	pMem->updateObject(pServiceConfig);
	if (bNoTransaction)
		commitUpdate(pServiceConfig,&theOldServiceConfig);
	return pServiceConfig;
}

CServiceConfig* CServiceConfigFactory::update(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pNewServiceConfig, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pServiceConfig == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pServiceConfig,pNewServiceConfig,bNoTransaction);
	}else{
		pTransaction->addResource(CServiceConfigResource::alloc(UPDATE_ACTION,this,pServiceConfig));
		return internalUpdate(pServiceConfig,pNewServiceConfig,false);
	}
}

void CServiceConfigFactory::internalRemove(CServiceConfig *pServiceConfig, bool bNoTransaction)
{
	CWriteableServiceConfig theOldServiceConfig;
	//beforeRemove(pServiceConfig);
	if (bNoTransaction)
		forceCopy(&theOldServiceConfig,pServiceConfig,sizeof(CServiceConfig));
	CServiceConfig *pTarget = pServiceConfig;
	if (pConfigIndex != NULL)
		pConfigIndex->removeObject(pServiceConfig);	
	
	pMem->free(pServiceConfig);
	if(bNoTransaction)
		commitRemove(&theOldServiceConfig);
}

void CServiceConfigFactory::remove(CServiceConfig *pServiceConfig, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pServiceConfig == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pServiceConfig, bNoTransaction);
	}else{
		pTransaction->addResource(CServiceConfigResource::alloc(DELETE_ACTION,this,pServiceConfig));
		internalRemove(pServiceConfig,false);
	}
}

CServiceConfig* CServiceConfigFactory::addOrUpdate(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pNewServiceConfig, CTransaction *pTransaction)
{
	if(pServiceConfig == NULL)
		return add(pNewServiceConfig,pTransaction);
	else
		return update(pServiceConfig,pNewServiceConfig,pTransaction);
}

void CServiceConfigFactory::retrieve(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pTargetServiceConfig)
{
	if(pServiceConfig == NULL)
		return;
	forceCopy(pTargetServiceConfig, pServiceConfig, sizeof(CServiceConfig));
}
	
int CServiceConfigFactory::addActionTrigger(CServiceConfigActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CServiceConfigFactory::removeActionTrigger(CServiceConfigActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CServiceConfigFactory::addCommitTrigger(CServiceConfigCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CServiceConfigFactory::removeCommitTrigger(CServiceConfigCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CServiceConfig* CServiceConfigFactory::getFirst()
{
	return (CServiceConfig *)(pMem->getFirst());
}
	
CServiceConfig* CServiceConfigFactory::getNext()
{
	return (CServiceConfig *)(pMem->getNext());
}

void CServiceConfigFactory::commitAdd(CServiceConfig *pServiceConfig)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pServiceConfig);
	}
}

void CServiceConfigFactory::commitUpdate(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pOldServiceConfig)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pServiceConfig,pOldServiceConfig);
	}
}
	
void CServiceConfigFactory::commitRemove(CWriteableServiceConfig *pServiceConfig)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pServiceConfig);
	}
}

void CServiceConfigFactory::checkNull(FILE *pFile)
{
	CServiceConfig *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->ConfigName.isNull()){
			bNull = true;
			fprintf(pFile, "ServiceConfig->ConfigName:NULL\n");
		}
		if(pTarget->Index1.isNull()){
			bNull = true;
			fprintf(pFile, "ServiceConfig->Index1:NULL\n");
		}
		if(pTarget->Index2.isNull()){
			bNull = true;
			fprintf(pFile, "ServiceConfig->Index2:NULL\n");
		}
		if(pTarget->ConfigValue.isNull()){
			bNull = true;
			fprintf(pFile, "ServiceConfig->ConfigValue:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CServiceConfigFactory::checkValid(FILE *pFile)
{
	CServiceConfig *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->ConfigName.isValid()){
			bValid = false;
			fprintf(pFile, "ServiceConfig->ConfigName:inValid\n");
		}
		if(!pTarget->Index1.isValid()){
			bValid = false;
			fprintf(pFile, "ServiceConfig->Index1:inValid\n");
		}
		if(!pTarget->Index2.isValid()){
			bValid = false;
			fprintf(pFile, "ServiceConfig->Index2:inValid\n");
		}
		if(!pTarget->ConfigValue.isValid()){
			bValid = false;
			fprintf(pFile, "ServiceConfig->ConfigValue:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "ServiceConfig->Remark:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "ServiceConfig->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "ServiceConfig->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

extern int compareForUserIDAndAppIDIndexinUserSession(const void *pV1, const void *pV2);
void CUserSessionFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pUserIDAndAppIDIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 'p':
	{
		if(!reuse) {
			pUserIDAndAppIDIndex=new CAVLTree(maxUnit,compareForUserIDAndAppIDIndexinUserSession,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("UserSession_UserIDAndAppIDIndex",pUserIDAndAppIDIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("UserSession_UserIDAndAppIDIndex");
			if(it != pIndexMap->end()) {
				pUserIDAndAppIDIndex=new CAVLTree(maxUnit,compareForUserIDAndAppIDIndexinUserSession,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pUserIDAndAppIDIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pTokenIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 'p':
	case 'r':
	{
		int hashKeySize=102560;
		if(!reuse) {
			pTokenIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("UserSession_TokenIndex",pTokenIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("UserSession_TokenIndex");
			if(it != pIndexMap->end()) {
				pTokenIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pTokenIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pActionTriggers=new vector<CUserSessionActionTrigger *>;
	pCommitTriggers=new vector<CUserSessionCommitTrigger *>;
}

CUserSessionFactory::CUserSessionFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CUserSession),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CUserSessionFactory::CUserSessionFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CUserSession),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CUserSessionFactory::~CUserSessionFactory()
{
	if (pUserIDAndAppIDIndex!=NULL)
		delete pUserIDAndAppIDIndex;
	if (pTokenIndex!=NULL)
		delete pTokenIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CUserSessionFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableUserSession thisUserSession;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisUserSession.readCSV(input,pNames))
		add(&thisUserSession);
	fclose(input);
	delete pNames;
	return 1;
}

int CUserSessionFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "UserSession.csv");
	return readCSV(szFileName);
}

int CUserSessionFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableUserSession *pUserSession;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableUserSession::writeCSVHead(output);
	pUserSession=(CWriteableUserSession *)(pMem->getFirst());
	while (pUserSession!=NULL) {
		pUserSession->writeCSV(output);
		pUserSession=(CWriteableUserSession *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CUserSessionFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableUserSession *pUserSession;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableUserSession::writeCSVHeadPreVersion(output);
	pUserSession=(CWriteableUserSession *)(pMem->getFirst());
	while (pUserSession!=NULL) {
		pUserSession->writeCSVPreVersion(output);
		pUserSession=(CWriteableUserSession *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CUserSessionFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "UserSession.csv");
	return writeCSV(szFileName);
}

void CUserSessionFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CUserSessionFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableUserSession *pUserSession;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CUserSessionFactory={       Total Count=%d\n", pMem->getCount());
	pUserSession=(CWriteableUserSession *)(pMem->getFirst());
	while (pUserSession!=NULL) {
		pUserSession->dump(fp,index++);
		pUserSession=(CWriteableUserSession *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CUserSessionFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pUserIDAndAppIDIndex != NULL)
		pUserIDAndAppIDIndex->removeAll();
	if(pTokenIndex != NULL)
		pTokenIndex->removeAll();
}

CUserSession *CUserSessionFactory::internalAdd(CWriteableUserSession *pUserSession, bool bNoTransaction)
{
	pUserSession->m_pMDB = m_pMDB;
	if(pUserSession->InsertTime.isNull())
		pUserSession->InsertTime = g_nCurrentTime;
	CUserSession *pTarget;
	//beforeAdd(pUserSession);
	pTarget=(CUserSession *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough UserSession in memory database");
		return NULL;
	}
	forceCopy(pTarget, pUserSession, sizeof(CUserSession));
	pMem->updateObject(pTarget);
	if (pUserIDAndAppIDIndex != NULL)
		pUserIDAndAppIDIndex->addObject(pTarget);
	if (pTokenIndex != NULL)
		pTokenIndex->addObject(pTarget,pTarget->HashToken);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CUserSession* CUserSessionFactory::add(CWriteableUserSession *pUserSession, CTransaction *pTransaction)
{
	pUserSession->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pUserSession,true);
	}else{
		CUserSession *pNewUserSession;
		pNewUserSession = internalAdd(pUserSession,false);
		pTransaction->addResource(CUserSessionResource::alloc(CREATE_ACTION,this,pNewUserSession));
		return pNewUserSession;
	}
}

CUserSession* CUserSessionFactory::internalUpdate(CUserSession *pUserSession, CWriteableUserSession *pNewUserSession,bool bNoTransaction)
{
	pNewUserSession->UpdateTime = g_nCurrentTime;
	CWriteableUserSession theOldUserSession;
	if (bNoTransaction)
		forceCopy(&theOldUserSession,pUserSession,sizeof(CUserSession));	
	CUserSession *pTarget = pUserSession;
	if (pTokenIndex != NULL)
		pTokenIndex->removeObject(pUserSession,pUserSession->HashToken);					
	forceCopy(pUserSession,pNewUserSession,sizeof(CUserSession));
	pMem->updateObject(pUserSession);
	if (pTokenIndex != NULL){
		((CWriteableUserSession*)pUserSession)->calHashToken();
		pTokenIndex->addObject(pUserSession,pUserSession->HashToken);
	}
	if (bNoTransaction)
		commitUpdate(pUserSession,&theOldUserSession);
	return pUserSession;
}

CUserSession* CUserSessionFactory::update(CUserSession *pUserSession, CWriteableUserSession *pNewUserSession, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pUserSession == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pUserSession,pNewUserSession,bNoTransaction);
	}else{
		pTransaction->addResource(CUserSessionResource::alloc(UPDATE_ACTION,this,pUserSession));
		return internalUpdate(pUserSession,pNewUserSession,false);
	}
}

void CUserSessionFactory::internalRemove(CUserSession *pUserSession, bool bNoTransaction)
{
	CWriteableUserSession theOldUserSession;
	//beforeRemove(pUserSession);
	if (bNoTransaction)
		forceCopy(&theOldUserSession,pUserSession,sizeof(CUserSession));
	CUserSession *pTarget = pUserSession;
	if (pUserIDAndAppIDIndex != NULL)
		pUserIDAndAppIDIndex->removeObject(pUserSession);	
	
	if (pTokenIndex != NULL)
		pTokenIndex->removeObject(pUserSession,pUserSession->HashToken);		
	pMem->free(pUserSession);
	if(bNoTransaction)
		commitRemove(&theOldUserSession);
}

void CUserSessionFactory::remove(CUserSession *pUserSession, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pUserSession == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pUserSession, bNoTransaction);
	}else{
		pTransaction->addResource(CUserSessionResource::alloc(DELETE_ACTION,this,pUserSession));
		internalRemove(pUserSession,false);
	}
}

CUserSession* CUserSessionFactory::addOrUpdate(CUserSession *pUserSession, CWriteableUserSession *pNewUserSession, CTransaction *pTransaction)
{
	if(pUserSession == NULL)
		return add(pNewUserSession,pTransaction);
	else
		return update(pUserSession,pNewUserSession,pTransaction);
}

void CUserSessionFactory::retrieve(CUserSession *pUserSession, CWriteableUserSession *pTargetUserSession)
{
	if(pUserSession == NULL)
		return;
	forceCopy(pTargetUserSession, pUserSession, sizeof(CUserSession));
}
	
int CUserSessionFactory::addActionTrigger(CUserSessionActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CUserSessionFactory::removeActionTrigger(CUserSessionActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CUserSessionFactory::addCommitTrigger(CUserSessionCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CUserSessionFactory::removeCommitTrigger(CUserSessionCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CUserSession* CUserSessionFactory::getFirst()
{
	return (CUserSession *)(pMem->getFirst());
}
	
CUserSession* CUserSessionFactory::getNext()
{
	return (CUserSession *)(pMem->getNext());
}

void CUserSessionFactory::commitAdd(CUserSession *pUserSession)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pUserSession);
	}
}

void CUserSessionFactory::commitUpdate(CUserSession *pUserSession, CWriteableUserSession *pOldUserSession)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pUserSession,pOldUserSession);
	}
}
	
void CUserSessionFactory::commitRemove(CWriteableUserSession *pUserSession)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pUserSession);
	}
}

void CUserSessionFactory::checkNull(FILE *pFile)
{
	CUserSession *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->Token.isNull()){
			bNull = true;
			fprintf(pFile, "UserSession->Token:NULL\n");
		}
		if(pTarget->SessionNo.isNull()){
			bNull = true;
			fprintf(pFile, "UserSession->SessionNo:NULL\n");
		}
		if(pTarget->FrontNo.isNull()){
			bNull = true;
			fprintf(pFile, "UserSession->FrontNo:NULL\n");
		}
		if(pTarget->UserType.isNull()){
			bNull = true;
			fprintf(pFile, "UserSession->UserType:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CUserSessionFactory::checkValid(FILE *pFile)
{
	CUserSession *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->UserID.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->UserID:inValid\n");
		}
		if(!pTarget->Password.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->Password:inValid\n");
		}
		if(!pTarget->UserProductID.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->UserProductID:inValid\n");
		}
		if(!pTarget->MacAddress.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->MacAddress:inValid\n");
		}
		if(!pTarget->ClientIPAddress.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->ClientIPAddress:inValid\n");
		}
		if(!pTarget->HDSerialID.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->HDSerialID:inValid\n");
		}
		if(!pTarget->AuthCode.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->AuthCode:inValid\n");
		}
		if(!pTarget->APPID.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->APPID:inValid\n");
		}
		if(!pTarget->Token.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->Token:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->Remark:inValid\n");
		}
		if(!pTarget->APIID.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->APIID:inValid\n");
		}
		if(!pTarget->LoginTime.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->LoginTime:inValid\n");
		}
		if(!pTarget->ExpireTime.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->ExpireTime:inValid\n");
		}
		if(!pTarget->IPAddress.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->IPAddress:inValid\n");
		}
		if(!pTarget->MaxLocalID.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->MaxLocalID:inValid\n");
		}
		if(!pTarget->SessionNo.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->SessionNo:inValid\n");
		}
		if(!pTarget->FrontNo.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->FrontNo:inValid\n");
		}
		if(!pTarget->AccessLimit.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->AccessLimit:inValid\n");
		}
		if(!pTarget->UserType.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->UserType:inValid\n");
		}
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->MemberID:inValid\n");
		}
		if(!pTarget->LimitAccesses.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->LimitAccesses:inValid\n");
		}
		if(!pTarget->SinkType.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->SinkType:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "UserSession->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

extern int compareForSettlementGroupIndexinInstrument(const void *pV1, const void *pV2);
extern int compareForProductGroupIndexinInstrument(const void *pV1, const void *pV2);
void CInstrumentFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pSettlementGroupIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		if(!reuse) {
			pSettlementGroupIndex=new CAVLTree(maxUnit,compareForSettlementGroupIndexinInstrument,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Instrument_SettlementGroupIndex",pSettlementGroupIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Instrument_SettlementGroupIndex");
			if(it != pIndexMap->end()) {
				pSettlementGroupIndex=new CAVLTree(maxUnit,compareForSettlementGroupIndexinInstrument,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pSettlementGroupIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pProductGroupIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 'p':
	{
		if(!reuse) {
			pProductGroupIndex=new CAVLTree(maxUnit,compareForProductGroupIndexinInstrument,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Instrument_ProductGroupIndex",pProductGroupIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Instrument_ProductGroupIndex");
			if(it != pIndexMap->end()) {
				pProductGroupIndex=new CAVLTree(maxUnit,compareForProductGroupIndexinInstrument,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pProductGroupIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pInstrumentIDHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 't':
	case 'p':
	case 'r':
	{
		int hashKeySize=769;
		if(!reuse) {
			pInstrumentIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Instrument_InstrumentIDHashIndex",pInstrumentIDHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("Instrument_InstrumentIDHashIndex");
			if(it != pIndexMap->end()) {
				pInstrumentIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pInstrumentIDHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pLastFoundInSearchBySettlementGroup=NULL;
	pLastFoundInSearchByProductGroup=NULL;
	pActionTriggers=new vector<CInstrumentActionTrigger *>;
	pCommitTriggers=new vector<CInstrumentCommitTrigger *>;
}

CInstrumentFactory::CInstrumentFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CInstrument),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CInstrumentFactory::CInstrumentFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CInstrument),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CInstrumentFactory::~CInstrumentFactory()
{
	if (pSettlementGroupIndex!=NULL)
		delete pSettlementGroupIndex;
	if (pProductGroupIndex!=NULL)
		delete pProductGroupIndex;
	if (pInstrumentIDHashIndex!=NULL)
		delete pInstrumentIDHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CInstrumentFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableInstrument thisInstrument;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisInstrument.readCSV(input,pNames))
		add(&thisInstrument);
	fclose(input);
	delete pNames;
	return 1;
}

int CInstrumentFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Instrument.csv");
	return readCSV(szFileName);
}

int CInstrumentFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableInstrument *pInstrument;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableInstrument::writeCSVHead(output);
	pInstrument=(CWriteableInstrument *)(pMem->getFirst());
	while (pInstrument!=NULL) {
		pInstrument->writeCSV(output);
		pInstrument=(CWriteableInstrument *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CInstrumentFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableInstrument *pInstrument;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableInstrument::writeCSVHeadPreVersion(output);
	pInstrument=(CWriteableInstrument *)(pMem->getFirst());
	while (pInstrument!=NULL) {
		pInstrument->writeCSVPreVersion(output);
		pInstrument=(CWriteableInstrument *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CInstrumentFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Instrument.csv");
	return writeCSV(szFileName);
}

void CInstrumentFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CInstrumentFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableInstrument *pInstrument;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CInstrumentFactory={       Total Count=%d\n", pMem->getCount());
	pInstrument=(CWriteableInstrument *)(pMem->getFirst());
	while (pInstrument!=NULL) {
		pInstrument->dump(fp,index++);
		pInstrument=(CWriteableInstrument *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CInstrumentFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pSettlementGroupIndex != NULL)
		pSettlementGroupIndex->removeAll();
	if(pProductGroupIndex != NULL)
		pProductGroupIndex->removeAll();
	if(pInstrumentIDHashIndex != NULL)
		pInstrumentIDHashIndex->removeAll();
}

CInstrument *CInstrumentFactory::internalAdd(CWriteableInstrument *pInstrument, bool bNoTransaction)
{
	pInstrument->m_pMDB = m_pMDB;
	if(pInstrument->PriceTick.isNull())
		pInstrument->PriceTick = 0.00000001;
	if(pInstrument->VolumeTick.isNull())
		pInstrument->VolumeTick = 0.00000001;
	if(pInstrument->InsertTime.isNull())
		pInstrument->InsertTime = g_nCurrentTime;
	CInstrument *pTarget;
	//beforeAdd(pInstrument);
	pTarget=(CInstrument *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough Instrument in memory database");
		return NULL;
	}
	forceCopy(pTarget, pInstrument, sizeof(CInstrument));
	pMem->updateObject(pTarget);
	if (pSettlementGroupIndex != NULL)
		pSettlementGroupIndex->addObject(pTarget);
	if (pProductGroupIndex != NULL)
		pProductGroupIndex->addObject(pTarget);
	if (pInstrumentIDHashIndex != NULL)
		pInstrumentIDHashIndex->addObject(pTarget,pTarget->HashInstrumentID);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CInstrument* CInstrumentFactory::add(CWriteableInstrument *pInstrument, CTransaction *pTransaction)
{
	pInstrument->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pInstrument,true);
	}else{
		CInstrument *pNewInstrument;
		pNewInstrument = internalAdd(pInstrument,false);
		pTransaction->addResource(CInstrumentResource::alloc(CREATE_ACTION,this,pNewInstrument));
		return pNewInstrument;
	}
}

CInstrument* CInstrumentFactory::internalUpdate(CInstrument *pInstrument, CWriteableInstrument *pNewInstrument,bool bNoTransaction)
{
	pNewInstrument->UpdateTime = g_nCurrentTime;
	CWriteableInstrument theOldInstrument;
	if (bNoTransaction)
		forceCopy(&theOldInstrument,pInstrument,sizeof(CInstrument));	
	CAVLNode *pSettlementGroupIndexNode=NULL;
	CAVLNode *pProductGroupIndexNode=NULL;
	CInstrument *pTarget = pInstrument;
	if (pSettlementGroupIndex != NULL)
		pSettlementGroupIndexNode=pSettlementGroupIndex->findObject(pInstrument);	
	if (pProductGroupIndex != NULL)
		pProductGroupIndexNode=pProductGroupIndex->findObject(pInstrument);	
	forceCopy(pInstrument,pNewInstrument,sizeof(CInstrument));
	pMem->updateObject(pInstrument);
	if (pSettlementGroupIndex != NULL)
		pSettlementGroupIndex->updateNode(pSettlementGroupIndexNode);
	if (pProductGroupIndex != NULL)
		pProductGroupIndex->updateNode(pProductGroupIndexNode);
	if (bNoTransaction)
		commitUpdate(pInstrument,&theOldInstrument);
	return pInstrument;
}

CInstrument* CInstrumentFactory::update(CInstrument *pInstrument, CWriteableInstrument *pNewInstrument, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pInstrument == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pInstrument,pNewInstrument,bNoTransaction);
	}else{
		pTransaction->addResource(CInstrumentResource::alloc(UPDATE_ACTION,this,pInstrument));
		return internalUpdate(pInstrument,pNewInstrument,false);
	}
}

void CInstrumentFactory::internalRemove(CInstrument *pInstrument, bool bNoTransaction)
{
	CWriteableInstrument theOldInstrument;
	//beforeRemove(pInstrument);
	if (bNoTransaction)
		forceCopy(&theOldInstrument,pInstrument,sizeof(CInstrument));
	CInstrument *pTarget = pInstrument;
	if (pSettlementGroupIndex != NULL)
		pSettlementGroupIndex->removeObject(pInstrument);	
	if (pProductGroupIndex != NULL)
		pProductGroupIndex->removeObject(pInstrument);	
	
	if (pInstrumentIDHashIndex != NULL)
		pInstrumentIDHashIndex->removeObject(pInstrument,pInstrument->HashInstrumentID);		
	pMem->free(pInstrument);
	if(bNoTransaction)
		commitRemove(&theOldInstrument);
}

void CInstrumentFactory::remove(CInstrument *pInstrument, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pInstrument == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pInstrument, bNoTransaction);
	}else{
		pTransaction->addResource(CInstrumentResource::alloc(DELETE_ACTION,this,pInstrument));
		internalRemove(pInstrument,false);
	}
}

CInstrument* CInstrumentFactory::addOrUpdate(CInstrument *pInstrument, CWriteableInstrument *pNewInstrument, CTransaction *pTransaction)
{
	if(pInstrument == NULL)
		return add(pNewInstrument,pTransaction);
	else
		return update(pInstrument,pNewInstrument,pTransaction);
}

void CInstrumentFactory::retrieve(CInstrument *pInstrument, CWriteableInstrument *pTargetInstrument)
{
	if(pInstrument == NULL)
		return;
	forceCopy(pTargetInstrument, pInstrument, sizeof(CInstrument));
}
	
int CInstrumentFactory::addActionTrigger(CInstrumentActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CInstrumentFactory::removeActionTrigger(CInstrumentActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CInstrumentFactory::addCommitTrigger(CInstrumentCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CInstrumentFactory::removeCommitTrigger(CInstrumentCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CInstrument* CInstrumentFactory::getFirst()
{
	return (CInstrument *)(pMem->getFirst());
}
	
CInstrument* CInstrumentFactory::getNext()
{
	return (CInstrument *)(pMem->getNext());
}

void CInstrumentFactory::commitAdd(CInstrument *pInstrument)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pInstrument);
	}
}

void CInstrumentFactory::commitUpdate(CInstrument *pInstrument, CWriteableInstrument *pOldInstrument)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pInstrument,pOldInstrument);
	}
}
	
void CInstrumentFactory::commitRemove(CWriteableInstrument *pInstrument)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pInstrument);
	}
}

void CInstrumentFactory::checkNull(FILE *pFile)
{
	CInstrument *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->InstrumentID:NULL\n");
		}
		if(pTarget->SettlementGroup.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->SettlementGroup:NULL\n");
		}
		if(pTarget->ProductClass.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->ProductClass:NULL\n");
		}
		if(pTarget->ProductType.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->ProductType:NULL\n");
		}
		if(pTarget->VolumeMultiple.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->VolumeMultiple:NULL\n");
		}
		if(pTarget->PriceCurrency.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->PriceCurrency:NULL\n");
		}
		if(pTarget->ClearCurrency.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->ClearCurrency:NULL\n");
		}
		if(pTarget->IsInverse.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->IsInverse:NULL\n");
		}
		if(pTarget->BaseCurrency.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->BaseCurrency:NULL\n");
		}
		if(pTarget->MarginPriceType.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->MarginPriceType:NULL\n");
		}
		if(pTarget->PriceTick.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->PriceTick:NULL\n");
		}
		if(pTarget->VolumeTick.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->VolumeTick:NULL\n");
		}
		if(pTarget->TradingModel.isNull()){
			bNull = true;
			fprintf(pFile, "Instrument->TradingModel:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CInstrumentFactory::checkValid(FILE *pFile)
{
	CInstrument *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->InstrumentID:inValid\n");
		}
		if(!pTarget->InstrumentName.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->InstrumentName:inValid\n");
		}
		if(!pTarget->SettlementGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->SettlementGroup:inValid\n");
		}
		if(!pTarget->ProductGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->ProductGroup:inValid\n");
		}
		if(!pTarget->UnderlyingID.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->UnderlyingID:inValid\n");
		}
		if(!pTarget->ProductClass.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->ProductClass:inValid\n");
		}
		if(!pTarget->ProductType.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->ProductType:inValid\n");
		}
		if(!pTarget->StrikePrice.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->StrikePrice:inValid\n");
		}
		if(!pTarget->OptionsType.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->OptionsType:inValid\n");
		}
		if(!pTarget->VolumeMultiple.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->VolumeMultiple:inValid\n");
		}
		if(!pTarget->StartTime.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->StartTime:inValid\n");
		}
		if(!pTarget->ExpireTime.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->ExpireTime:inValid\n");
		}
		if(!pTarget->PriceCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->PriceCurrency:inValid\n");
		}
		if(!pTarget->ClearCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->ClearCurrency:inValid\n");
		}
		if(!pTarget->IsInverse.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->IsInverse:inValid\n");
		}
		if(!pTarget->BaseCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->BaseCurrency:inValid\n");
		}
		if(!pTarget->MarginPriceType.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->MarginPriceType:inValid\n");
		}
		if(!pTarget->BasisPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->BasisPrice:inValid\n");
		}
		if(!pTarget->MinOrderVolume.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->MinOrderVolume:inValid\n");
		}
		if(!pTarget->MinOrderCost.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->MinOrderCost:inValid\n");
		}
		if(!pTarget->MaxOrderVolume.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->MaxOrderVolume:inValid\n");
		}
		if(!pTarget->PriceTick.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->PriceTick:inValid\n");
		}
		if(!pTarget->VolumeTick.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->VolumeTick:inValid\n");
		}
		if(!pTarget->ShowVolumeTick.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->ShowVolumeTick:inValid\n");
		}
		if(!pTarget->TradingModel.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->TradingModel:inValid\n");
		}
		if(!pTarget->DayStartTime.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->DayStartTime:inValid\n");
		}
		if(!pTarget->SortNumber.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->SortNumber:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->Remark:inValid\n");
		}
		if(!pTarget->DefaultLeverage.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->DefaultLeverage:inValid\n");
		}
		if(!pTarget->PriceLimitValueMode.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->PriceLimitValueMode:inValid\n");
		}
		if(!pTarget->PriceLimitPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->PriceLimitPrice:inValid\n");
		}
		if(!pTarget->PriceLimitUpperValue.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->PriceLimitUpperValue:inValid\n");
		}
		if(!pTarget->PriceLimitLowerValue.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->PriceLimitLowerValue:inValid\n");
		}
		if(!pTarget->MarginRateGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->MarginRateGroup:inValid\n");
		}
		if(!pTarget->CFDGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->CFDGroup:inValid\n");
		}
		if(!pTarget->FeeGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->FeeGroup:inValid\n");
		}
		if(!pTarget->TradingRightGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->TradingRightGroup:inValid\n");
		}
		if(!pTarget->MaxOpenInterest.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->MaxOpenInterest:inValid\n");
		}
		if(!pTarget->FundingRateGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->FundingRateGroup:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->UpdateTime:inValid\n");
		}
		if(!pTarget->MarkedPriceTimes.isValid()){
			bValid = false;
			fprintf(pFile, "Instrument->MarkedPriceTimes:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CInstrumentFactory::linkAllMarketData()
{
	CInstrument *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkMarketData();
		pTarget=getNext();
	}
}

void CInstrumentFactory::linkAllClearCurrency()
{
	CInstrument *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkClearCurrency();
		pTarget=getNext();
	}
}

void CInstrumentFactory::linkAllBaseCurrency()
{
	CInstrument *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkBaseCurrency();
		pTarget=getNext();
	}
}

void CInstrumentFactory::checkLinkMarketData(FILE *pFile)
{
	CInstrument *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getMarketData() == NULL){
			if (pTarget->ExchangeID.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->InstrumentID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Instrument->MarketData:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CInstrumentFactory::checkLinkClearCurrency(FILE *pFile)
{
	CInstrument *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getClearCurrency() == NULL){
			if (pTarget->ClearCurrency.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->SettlementGroup.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Instrument->ClearCurrency:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CInstrumentFactory::checkLinkBaseCurrency(FILE *pFile)
{
	CInstrument *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getBaseCurrency() == NULL){
			if (pTarget->BaseCurrency.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->SettlementGroup.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Instrument->BaseCurrency:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CInstrumentFactory::clearLinkMarketData()
{
}

void CInstrumentFactory::clearLinkClearCurrency()
{
}

void CInstrumentFactory::clearLinkBaseCurrency()
{
}

void CTradingRightFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pMemberIDAndInstrumentHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		int hashKeySize=4096;
		if(!reuse) {
			pMemberIDAndInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TradingRight_MemberIDAndInstrumentHashIndex",pMemberIDAndInstrumentHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("TradingRight_MemberIDAndInstrumentHashIndex");
			if(it != pIndexMap->end()) {
				pMemberIDAndInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pMemberIDAndInstrumentHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pActionTriggers=new vector<CTradingRightActionTrigger *>;
	pCommitTriggers=new vector<CTradingRightCommitTrigger *>;
}

CTradingRightFactory::CTradingRightFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CTradingRight),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CTradingRightFactory::CTradingRightFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CTradingRight),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CTradingRightFactory::~CTradingRightFactory()
{
	if (pMemberIDAndInstrumentHashIndex!=NULL)
		delete pMemberIDAndInstrumentHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CTradingRightFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableTradingRight thisTradingRight;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisTradingRight.readCSV(input,pNames))
		add(&thisTradingRight);
	fclose(input);
	delete pNames;
	return 1;
}

int CTradingRightFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "TradingRight.csv");
	return readCSV(szFileName);
}

int CTradingRightFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableTradingRight *pTradingRight;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableTradingRight::writeCSVHead(output);
	pTradingRight=(CWriteableTradingRight *)(pMem->getFirst());
	while (pTradingRight!=NULL) {
		pTradingRight->writeCSV(output);
		pTradingRight=(CWriteableTradingRight *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CTradingRightFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableTradingRight *pTradingRight;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableTradingRight::writeCSVHeadPreVersion(output);
	pTradingRight=(CWriteableTradingRight *)(pMem->getFirst());
	while (pTradingRight!=NULL) {
		pTradingRight->writeCSVPreVersion(output);
		pTradingRight=(CWriteableTradingRight *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CTradingRightFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "TradingRight.csv");
	return writeCSV(szFileName);
}

void CTradingRightFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CTradingRightFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableTradingRight *pTradingRight;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CTradingRightFactory={       Total Count=%d\n", pMem->getCount());
	pTradingRight=(CWriteableTradingRight *)(pMem->getFirst());
	while (pTradingRight!=NULL) {
		pTradingRight->dump(fp,index++);
		pTradingRight=(CWriteableTradingRight *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CTradingRightFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pMemberIDAndInstrumentHashIndex != NULL)
		pMemberIDAndInstrumentHashIndex->removeAll();
}

CTradingRight *CTradingRightFactory::internalAdd(CWriteableTradingRight *pTradingRight, bool bNoTransaction)
{
	pTradingRight->m_pMDB = m_pMDB;
	if(pTradingRight->InsertTime.isNull())
		pTradingRight->InsertTime = g_nCurrentTime;
	CTradingRight *pTarget;
	//beforeAdd(pTradingRight);
	pTarget=(CTradingRight *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough TradingRight in memory database");
		return NULL;
	}
	forceCopy(pTarget, pTradingRight, sizeof(CTradingRight));
	pMem->updateObject(pTarget);
	if (pMemberIDAndInstrumentHashIndex != NULL)
		pMemberIDAndInstrumentHashIndex->addObject(pTarget,pTarget->HashMemberIDAndInstrument);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CTradingRight* CTradingRightFactory::add(CWriteableTradingRight *pTradingRight, CTransaction *pTransaction)
{
	pTradingRight->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pTradingRight,true);
	}else{
		CTradingRight *pNewTradingRight;
		pNewTradingRight = internalAdd(pTradingRight,false);
		pTransaction->addResource(CTradingRightResource::alloc(CREATE_ACTION,this,pNewTradingRight));
		return pNewTradingRight;
	}
}

CTradingRight* CTradingRightFactory::internalUpdate(CTradingRight *pTradingRight, CWriteableTradingRight *pNewTradingRight,bool bNoTransaction)
{
	pNewTradingRight->UpdateTime = g_nCurrentTime;
	CWriteableTradingRight theOldTradingRight;
	if (bNoTransaction)
		forceCopy(&theOldTradingRight,pTradingRight,sizeof(CTradingRight));	
	CTradingRight *pTarget = pTradingRight;
	forceCopy(pTradingRight,pNewTradingRight,sizeof(CTradingRight));
	pMem->updateObject(pTradingRight);
	if (bNoTransaction)
		commitUpdate(pTradingRight,&theOldTradingRight);
	return pTradingRight;
}

CTradingRight* CTradingRightFactory::update(CTradingRight *pTradingRight, CWriteableTradingRight *pNewTradingRight, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pTradingRight == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pTradingRight,pNewTradingRight,bNoTransaction);
	}else{
		pTransaction->addResource(CTradingRightResource::alloc(UPDATE_ACTION,this,pTradingRight));
		return internalUpdate(pTradingRight,pNewTradingRight,false);
	}
}

void CTradingRightFactory::internalRemove(CTradingRight *pTradingRight, bool bNoTransaction)
{
	CWriteableTradingRight theOldTradingRight;
	//beforeRemove(pTradingRight);
	if (bNoTransaction)
		forceCopy(&theOldTradingRight,pTradingRight,sizeof(CTradingRight));
	CTradingRight *pTarget = pTradingRight;
	
	if (pMemberIDAndInstrumentHashIndex != NULL)
		pMemberIDAndInstrumentHashIndex->removeObject(pTradingRight,pTradingRight->HashMemberIDAndInstrument);		
	pMem->free(pTradingRight);
	if(bNoTransaction)
		commitRemove(&theOldTradingRight);
}

void CTradingRightFactory::remove(CTradingRight *pTradingRight, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pTradingRight == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pTradingRight, bNoTransaction);
	}else{
		pTransaction->addResource(CTradingRightResource::alloc(DELETE_ACTION,this,pTradingRight));
		internalRemove(pTradingRight,false);
	}
}

CTradingRight* CTradingRightFactory::addOrUpdate(CTradingRight *pTradingRight, CWriteableTradingRight *pNewTradingRight, CTransaction *pTransaction)
{
	if(pTradingRight == NULL)
		return add(pNewTradingRight,pTransaction);
	else
		return update(pTradingRight,pNewTradingRight,pTransaction);
}

void CTradingRightFactory::retrieve(CTradingRight *pTradingRight, CWriteableTradingRight *pTargetTradingRight)
{
	if(pTradingRight == NULL)
		return;
	forceCopy(pTargetTradingRight, pTradingRight, sizeof(CTradingRight));
}
	
int CTradingRightFactory::addActionTrigger(CTradingRightActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CTradingRightFactory::removeActionTrigger(CTradingRightActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CTradingRightFactory::addCommitTrigger(CTradingRightCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CTradingRightFactory::removeCommitTrigger(CTradingRightCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CTradingRight* CTradingRightFactory::getFirst()
{
	return (CTradingRight *)(pMem->getFirst());
}
	
CTradingRight* CTradingRightFactory::getNext()
{
	return (CTradingRight *)(pMem->getNext());
}

void CTradingRightFactory::commitAdd(CTradingRight *pTradingRight)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pTradingRight);
	}
}

void CTradingRightFactory::commitUpdate(CTradingRight *pTradingRight, CWriteableTradingRight *pOldTradingRight)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pTradingRight,pOldTradingRight);
	}
}
	
void CTradingRightFactory::commitRemove(CWriteableTradingRight *pTradingRight)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pTradingRight);
	}
}

void CTradingRightFactory::checkNull(FILE *pFile)
{
	CTradingRight *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "TradingRight->MemberID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "TradingRight->InstrumentID:NULL\n");
		}
		if(pTarget->Direction.isNull()){
			bNull = true;
			fprintf(pFile, "TradingRight->Direction:NULL\n");
		}
		if(pTarget->TradingRight.isNull()){
			bNull = true;
			fprintf(pFile, "TradingRight->TradingRight:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CTradingRightFactory::checkValid(FILE *pFile)
{
	CTradingRight *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "TradingRight->MemberID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "TradingRight->InstrumentID:inValid\n");
		}
		if(!pTarget->Direction.isValid()){
			bValid = false;
			fprintf(pFile, "TradingRight->Direction:inValid\n");
		}
		if(!pTarget->TradingRight.isValid()){
			bValid = false;
			fprintf(pFile, "TradingRight->TradingRight:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "TradingRight->Remark:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "TradingRight->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "TradingRight->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CMarginRateFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pMemberAndInstrumentHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 'r':
	case 'p':
	{
		int hashKeySize=12289;
		if(!reuse) {
			pMemberAndInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("MarginRate_MemberAndInstrumentHashIndex",pMemberAndInstrumentHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("MarginRate_MemberAndInstrumentHashIndex");
			if(it != pIndexMap->end()) {
				pMemberAndInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pMemberAndInstrumentHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pActionTriggers=new vector<CMarginRateActionTrigger *>;
	pCommitTriggers=new vector<CMarginRateCommitTrigger *>;
}

CMarginRateFactory::CMarginRateFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CMarginRate),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CMarginRateFactory::CMarginRateFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CMarginRate),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CMarginRateFactory::~CMarginRateFactory()
{
	if (pMemberAndInstrumentHashIndex!=NULL)
		delete pMemberAndInstrumentHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CMarginRateFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableMarginRate thisMarginRate;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisMarginRate.readCSV(input,pNames))
		add(&thisMarginRate);
	fclose(input);
	delete pNames;
	return 1;
}

int CMarginRateFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "MarginRate.csv");
	return readCSV(szFileName);
}

int CMarginRateFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableMarginRate *pMarginRate;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableMarginRate::writeCSVHead(output);
	pMarginRate=(CWriteableMarginRate *)(pMem->getFirst());
	while (pMarginRate!=NULL) {
		pMarginRate->writeCSV(output);
		pMarginRate=(CWriteableMarginRate *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CMarginRateFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableMarginRate *pMarginRate;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableMarginRate::writeCSVHeadPreVersion(output);
	pMarginRate=(CWriteableMarginRate *)(pMem->getFirst());
	while (pMarginRate!=NULL) {
		pMarginRate->writeCSVPreVersion(output);
		pMarginRate=(CWriteableMarginRate *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CMarginRateFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "MarginRate.csv");
	return writeCSV(szFileName);
}

void CMarginRateFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CMarginRateFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableMarginRate *pMarginRate;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CMarginRateFactory={       Total Count=%d\n", pMem->getCount());
	pMarginRate=(CWriteableMarginRate *)(pMem->getFirst());
	while (pMarginRate!=NULL) {
		pMarginRate->dump(fp,index++);
		pMarginRate=(CWriteableMarginRate *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CMarginRateFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pMemberAndInstrumentHashIndex != NULL)
		pMemberAndInstrumentHashIndex->removeAll();
}

CMarginRate *CMarginRateFactory::internalAdd(CWriteableMarginRate *pMarginRate, bool bNoTransaction)
{
	pMarginRate->m_pMDB = m_pMDB;
	if(pMarginRate->InsertTime.isNull())
		pMarginRate->InsertTime = g_nCurrentTime;
	CMarginRate *pTarget;
	//beforeAdd(pMarginRate);
	pTarget=(CMarginRate *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough MarginRate in memory database");
		return NULL;
	}
	forceCopy(pTarget, pMarginRate, sizeof(CMarginRate));
	pMem->updateObject(pTarget);
	if (pMemberAndInstrumentHashIndex != NULL)
		pMemberAndInstrumentHashIndex->addObject(pTarget,pTarget->HashMemberAndInstrument);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CMarginRate* CMarginRateFactory::add(CWriteableMarginRate *pMarginRate, CTransaction *pTransaction)
{
	pMarginRate->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pMarginRate,true);
	}else{
		CMarginRate *pNewMarginRate;
		pNewMarginRate = internalAdd(pMarginRate,false);
		pTransaction->addResource(CMarginRateResource::alloc(CREATE_ACTION,this,pNewMarginRate));
		return pNewMarginRate;
	}
}

CMarginRate* CMarginRateFactory::internalUpdate(CMarginRate *pMarginRate, CWriteableMarginRate *pNewMarginRate,bool bNoTransaction)
{
	pNewMarginRate->UpdateTime = g_nCurrentTime;
	CWriteableMarginRate theOldMarginRate;
	if (bNoTransaction)
		forceCopy(&theOldMarginRate,pMarginRate,sizeof(CMarginRate));	
	CMarginRate *pTarget = pMarginRate;
	forceCopy(pMarginRate,pNewMarginRate,sizeof(CMarginRate));
	pMem->updateObject(pMarginRate);
	if (bNoTransaction)
		commitUpdate(pMarginRate,&theOldMarginRate);
	return pMarginRate;
}

CMarginRate* CMarginRateFactory::update(CMarginRate *pMarginRate, CWriteableMarginRate *pNewMarginRate, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pMarginRate == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pMarginRate,pNewMarginRate,bNoTransaction);
	}else{
		pTransaction->addResource(CMarginRateResource::alloc(UPDATE_ACTION,this,pMarginRate));
		return internalUpdate(pMarginRate,pNewMarginRate,false);
	}
}

void CMarginRateFactory::internalRemove(CMarginRate *pMarginRate, bool bNoTransaction)
{
	CWriteableMarginRate theOldMarginRate;
	//beforeRemove(pMarginRate);
	if (bNoTransaction)
		forceCopy(&theOldMarginRate,pMarginRate,sizeof(CMarginRate));
	CMarginRate *pTarget = pMarginRate;
	
	if (pMemberAndInstrumentHashIndex != NULL)
		pMemberAndInstrumentHashIndex->removeObject(pMarginRate,pMarginRate->HashMemberAndInstrument);		
	pMem->free(pMarginRate);
	if(bNoTransaction)
		commitRemove(&theOldMarginRate);
}

void CMarginRateFactory::remove(CMarginRate *pMarginRate, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pMarginRate == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pMarginRate, bNoTransaction);
	}else{
		pTransaction->addResource(CMarginRateResource::alloc(DELETE_ACTION,this,pMarginRate));
		internalRemove(pMarginRate,false);
	}
}

CMarginRate* CMarginRateFactory::addOrUpdate(CMarginRate *pMarginRate, CWriteableMarginRate *pNewMarginRate, CTransaction *pTransaction)
{
	if(pMarginRate == NULL)
		return add(pNewMarginRate,pTransaction);
	else
		return update(pMarginRate,pNewMarginRate,pTransaction);
}

void CMarginRateFactory::retrieve(CMarginRate *pMarginRate, CWriteableMarginRate *pTargetMarginRate)
{
	if(pMarginRate == NULL)
		return;
	forceCopy(pTargetMarginRate, pMarginRate, sizeof(CMarginRate));
}
	
int CMarginRateFactory::addActionTrigger(CMarginRateActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CMarginRateFactory::removeActionTrigger(CMarginRateActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CMarginRateFactory::addCommitTrigger(CMarginRateCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CMarginRateFactory::removeCommitTrigger(CMarginRateCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CMarginRate* CMarginRateFactory::getFirst()
{
	return (CMarginRate *)(pMem->getFirst());
}
	
CMarginRate* CMarginRateFactory::getNext()
{
	return (CMarginRate *)(pMem->getNext());
}

void CMarginRateFactory::commitAdd(CMarginRate *pMarginRate)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pMarginRate);
	}
}

void CMarginRateFactory::commitUpdate(CMarginRate *pMarginRate, CWriteableMarginRate *pOldMarginRate)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pMarginRate,pOldMarginRate);
	}
}
	
void CMarginRateFactory::commitRemove(CWriteableMarginRate *pMarginRate)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pMarginRate);
	}
}

void CMarginRateFactory::checkNull(FILE *pFile)
{
	CMarginRate *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->MemberID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->InstrumentID:NULL\n");
		}
		if(pTarget->ValueMode.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->ValueMode:NULL\n");
		}
		if(pTarget->LongMarginRatio.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->LongMarginRatio:NULL\n");
		}
		if(pTarget->LongMaintMarginRatio.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->LongMaintMarginRatio:NULL\n");
		}
		if(pTarget->LongMarginStep.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->LongMarginStep:NULL\n");
		}
		if(pTarget->LongMaintMarginStep.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->LongMaintMarginStep:NULL\n");
		}
		if(pTarget->LongRiskLimit.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->LongRiskLimit:NULL\n");
		}
		if(pTarget->LongRiskStep.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->LongRiskStep:NULL\n");
		}
		if(pTarget->ShortMarginRatio.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->ShortMarginRatio:NULL\n");
		}
		if(pTarget->ShortMaintMarginRatio.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->ShortMaintMarginRatio:NULL\n");
		}
		if(pTarget->ShortMarginStep.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->ShortMarginStep:NULL\n");
		}
		if(pTarget->ShortMaintMarginStep.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->ShortMaintMarginStep:NULL\n");
		}
		if(pTarget->ShortRiskLimit.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->ShortRiskLimit:NULL\n");
		}
		if(pTarget->ShortRiskStep.isNull()){
			bNull = true;
			fprintf(pFile, "MarginRate->ShortRiskStep:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CMarginRateFactory::checkValid(FILE *pFile)
{
	CMarginRate *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->MemberID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->InstrumentID:inValid\n");
		}
		if(!pTarget->ValueMode.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->ValueMode:inValid\n");
		}
		if(!pTarget->MarginSequenceType.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->MarginSequenceType:inValid\n");
		}
		if(!pTarget->RiskSequenceType.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->RiskSequenceType:inValid\n");
		}
		if(!pTarget->LongMarginRatio.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->LongMarginRatio:inValid\n");
		}
		if(!pTarget->LongMaintMarginRatio.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->LongMaintMarginRatio:inValid\n");
		}
		if(!pTarget->LongMarginStep.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->LongMarginStep:inValid\n");
		}
		if(!pTarget->LongMaintMarginStep.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->LongMaintMarginStep:inValid\n");
		}
		if(!pTarget->LongRiskLimit.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->LongRiskLimit:inValid\n");
		}
		if(!pTarget->LongRiskStep.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->LongRiskStep:inValid\n");
		}
		if(!pTarget->ShortMarginRatio.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->ShortMarginRatio:inValid\n");
		}
		if(!pTarget->ShortMaintMarginRatio.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->ShortMaintMarginRatio:inValid\n");
		}
		if(!pTarget->ShortMarginStep.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->ShortMarginStep:inValid\n");
		}
		if(!pTarget->ShortMaintMarginStep.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->ShortMaintMarginStep:inValid\n");
		}
		if(!pTarget->ShortRiskLimit.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->ShortRiskLimit:inValid\n");
		}
		if(!pTarget->ShortRiskStep.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->ShortRiskStep:inValid\n");
		}
		if(!pTarget->DefaultLeverage.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->DefaultLeverage:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->Remark:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "MarginRate->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CCFDRateFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pMemberAndInstrumentHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		int hashKeySize=12289;
		if(!reuse) {
			pMemberAndInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("CFDRate_MemberAndInstrumentHashIndex",pMemberAndInstrumentHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("CFDRate_MemberAndInstrumentHashIndex");
			if(it != pIndexMap->end()) {
				pMemberAndInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pMemberAndInstrumentHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pActionTriggers=new vector<CCFDRateActionTrigger *>;
	pCommitTriggers=new vector<CCFDRateCommitTrigger *>;
}

CCFDRateFactory::CCFDRateFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CCFDRate),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CCFDRateFactory::CCFDRateFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CCFDRate),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CCFDRateFactory::~CCFDRateFactory()
{
	if (pMemberAndInstrumentHashIndex!=NULL)
		delete pMemberAndInstrumentHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CCFDRateFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableCFDRate thisCFDRate;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisCFDRate.readCSV(input,pNames))
		add(&thisCFDRate);
	fclose(input);
	delete pNames;
	return 1;
}

int CCFDRateFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "CFDRate.csv");
	return readCSV(szFileName);
}

int CCFDRateFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableCFDRate *pCFDRate;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableCFDRate::writeCSVHead(output);
	pCFDRate=(CWriteableCFDRate *)(pMem->getFirst());
	while (pCFDRate!=NULL) {
		pCFDRate->writeCSV(output);
		pCFDRate=(CWriteableCFDRate *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CCFDRateFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableCFDRate *pCFDRate;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableCFDRate::writeCSVHeadPreVersion(output);
	pCFDRate=(CWriteableCFDRate *)(pMem->getFirst());
	while (pCFDRate!=NULL) {
		pCFDRate->writeCSVPreVersion(output);
		pCFDRate=(CWriteableCFDRate *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CCFDRateFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "CFDRate.csv");
	return writeCSV(szFileName);
}

void CCFDRateFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CCFDRateFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableCFDRate *pCFDRate;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CCFDRateFactory={       Total Count=%d\n", pMem->getCount());
	pCFDRate=(CWriteableCFDRate *)(pMem->getFirst());
	while (pCFDRate!=NULL) {
		pCFDRate->dump(fp,index++);
		pCFDRate=(CWriteableCFDRate *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CCFDRateFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pMemberAndInstrumentHashIndex != NULL)
		pMemberAndInstrumentHashIndex->removeAll();
}

CCFDRate *CCFDRateFactory::internalAdd(CWriteableCFDRate *pCFDRate, bool bNoTransaction)
{
	pCFDRate->m_pMDB = m_pMDB;
	if(pCFDRate->TriggerOrderType.isNull())
		pCFDRate->TriggerOrderType = "default";
	if(pCFDRate->InsertTime.isNull())
		pCFDRate->InsertTime = g_nCurrentTime;
	CCFDRate *pTarget;
	//beforeAdd(pCFDRate);
	pTarget=(CCFDRate *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough CFDRate in memory database");
		return NULL;
	}
	forceCopy(pTarget, pCFDRate, sizeof(CCFDRate));
	pMem->updateObject(pTarget);
	if (pMemberAndInstrumentHashIndex != NULL)
		pMemberAndInstrumentHashIndex->addObject(pTarget,pTarget->HashMemberAndInstrument);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CCFDRate* CCFDRateFactory::add(CWriteableCFDRate *pCFDRate, CTransaction *pTransaction)
{
	pCFDRate->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pCFDRate,true);
	}else{
		CCFDRate *pNewCFDRate;
		pNewCFDRate = internalAdd(pCFDRate,false);
		pTransaction->addResource(CCFDRateResource::alloc(CREATE_ACTION,this,pNewCFDRate));
		return pNewCFDRate;
	}
}

CCFDRate* CCFDRateFactory::internalUpdate(CCFDRate *pCFDRate, CWriteableCFDRate *pNewCFDRate,bool bNoTransaction)
{
	pNewCFDRate->UpdateTime = g_nCurrentTime;
	CWriteableCFDRate theOldCFDRate;
	if (bNoTransaction)
		forceCopy(&theOldCFDRate,pCFDRate,sizeof(CCFDRate));	
	CCFDRate *pTarget = pCFDRate;
	forceCopy(pCFDRate,pNewCFDRate,sizeof(CCFDRate));
	pMem->updateObject(pCFDRate);
	if (bNoTransaction)
		commitUpdate(pCFDRate,&theOldCFDRate);
	return pCFDRate;
}

CCFDRate* CCFDRateFactory::update(CCFDRate *pCFDRate, CWriteableCFDRate *pNewCFDRate, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pCFDRate == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pCFDRate,pNewCFDRate,bNoTransaction);
	}else{
		pTransaction->addResource(CCFDRateResource::alloc(UPDATE_ACTION,this,pCFDRate));
		return internalUpdate(pCFDRate,pNewCFDRate,false);
	}
}

void CCFDRateFactory::internalRemove(CCFDRate *pCFDRate, bool bNoTransaction)
{
	CWriteableCFDRate theOldCFDRate;
	//beforeRemove(pCFDRate);
	if (bNoTransaction)
		forceCopy(&theOldCFDRate,pCFDRate,sizeof(CCFDRate));
	CCFDRate *pTarget = pCFDRate;
	
	if (pMemberAndInstrumentHashIndex != NULL)
		pMemberAndInstrumentHashIndex->removeObject(pCFDRate,pCFDRate->HashMemberAndInstrument);		
	pMem->free(pCFDRate);
	if(bNoTransaction)
		commitRemove(&theOldCFDRate);
}

void CCFDRateFactory::remove(CCFDRate *pCFDRate, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pCFDRate == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pCFDRate, bNoTransaction);
	}else{
		pTransaction->addResource(CCFDRateResource::alloc(DELETE_ACTION,this,pCFDRate));
		internalRemove(pCFDRate,false);
	}
}

CCFDRate* CCFDRateFactory::addOrUpdate(CCFDRate *pCFDRate, CWriteableCFDRate *pNewCFDRate, CTransaction *pTransaction)
{
	if(pCFDRate == NULL)
		return add(pNewCFDRate,pTransaction);
	else
		return update(pCFDRate,pNewCFDRate,pTransaction);
}

void CCFDRateFactory::retrieve(CCFDRate *pCFDRate, CWriteableCFDRate *pTargetCFDRate)
{
	if(pCFDRate == NULL)
		return;
	forceCopy(pTargetCFDRate, pCFDRate, sizeof(CCFDRate));
}
	
int CCFDRateFactory::addActionTrigger(CCFDRateActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CCFDRateFactory::removeActionTrigger(CCFDRateActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CCFDRateFactory::addCommitTrigger(CCFDRateCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CCFDRateFactory::removeCommitTrigger(CCFDRateCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CCFDRate* CCFDRateFactory::getFirst()
{
	return (CCFDRate *)(pMem->getFirst());
}
	
CCFDRate* CCFDRateFactory::getNext()
{
	return (CCFDRate *)(pMem->getNext());
}

void CCFDRateFactory::commitAdd(CCFDRate *pCFDRate)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pCFDRate);
	}
}

void CCFDRateFactory::commitUpdate(CCFDRate *pCFDRate, CWriteableCFDRate *pOldCFDRate)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pCFDRate,pOldCFDRate);
	}
}
	
void CCFDRateFactory::commitRemove(CWriteableCFDRate *pCFDRate)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pCFDRate);
	}
}

void CCFDRateFactory::checkNull(FILE *pFile)
{
	CCFDRate *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "CFDRate->MemberID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "CFDRate->InstrumentID:NULL\n");
		}
		if(pTarget->TriggerOrderType.isNull()){
			bNull = true;
			fprintf(pFile, "CFDRate->TriggerOrderType:NULL\n");
		}
		if(pTarget->RateBegin.isNull()){
			bNull = true;
			fprintf(pFile, "CFDRate->RateBegin:NULL\n");
		}
		if(pTarget->RateEnd.isNull()){
			bNull = true;
			fprintf(pFile, "CFDRate->RateEnd:NULL\n");
		}
		if(pTarget->CompVolume.isNull()){
			bNull = true;
			fprintf(pFile, "CFDRate->CompVolume:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CCFDRateFactory::checkValid(FILE *pFile)
{
	CCFDRate *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->MemberID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->InstrumentID:inValid\n");
		}
		if(!pTarget->TriggerOrderType.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->TriggerOrderType:inValid\n");
		}
		if(!pTarget->RateBegin.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->RateBegin:inValid\n");
		}
		if(!pTarget->RateEnd.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->RateEnd:inValid\n");
		}
		if(!pTarget->CompVolume.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->CompVolume:inValid\n");
		}
		if(!pTarget->CompRate.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->CompRate:inValid\n");
		}
		if(!pTarget->HardRate.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->HardRate:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->Remark:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "CFDRate->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CFeeFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pMemberIDAndInstrumentHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 'r':
	{
		int hashKeySize=6151;
		if(!reuse) {
			pMemberIDAndInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Fee_MemberIDAndInstrumentHashIndex",pMemberIDAndInstrumentHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("Fee_MemberIDAndInstrumentHashIndex");
			if(it != pIndexMap->end()) {
				pMemberIDAndInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pMemberIDAndInstrumentHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pActionTriggers=new vector<CFeeActionTrigger *>;
	pCommitTriggers=new vector<CFeeCommitTrigger *>;
}

CFeeFactory::CFeeFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CFee),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CFeeFactory::CFeeFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CFee),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CFeeFactory::~CFeeFactory()
{
	if (pMemberIDAndInstrumentHashIndex!=NULL)
		delete pMemberIDAndInstrumentHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CFeeFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableFee thisFee;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisFee.readCSV(input,pNames))
		add(&thisFee);
	fclose(input);
	delete pNames;
	return 1;
}

int CFeeFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Fee.csv");
	return readCSV(szFileName);
}

int CFeeFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableFee *pFee;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableFee::writeCSVHead(output);
	pFee=(CWriteableFee *)(pMem->getFirst());
	while (pFee!=NULL) {
		pFee->writeCSV(output);
		pFee=(CWriteableFee *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CFeeFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableFee *pFee;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableFee::writeCSVHeadPreVersion(output);
	pFee=(CWriteableFee *)(pMem->getFirst());
	while (pFee!=NULL) {
		pFee->writeCSVPreVersion(output);
		pFee=(CWriteableFee *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CFeeFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Fee.csv");
	return writeCSV(szFileName);
}

void CFeeFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CFeeFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableFee *pFee;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CFeeFactory={       Total Count=%d\n", pMem->getCount());
	pFee=(CWriteableFee *)(pMem->getFirst());
	while (pFee!=NULL) {
		pFee->dump(fp,index++);
		pFee=(CWriteableFee *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CFeeFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pMemberIDAndInstrumentHashIndex != NULL)
		pMemberIDAndInstrumentHashIndex->removeAll();
}

CFee *CFeeFactory::internalAdd(CWriteableFee *pFee, bool bNoTransaction)
{
	pFee->m_pMDB = m_pMDB;
	if(pFee->InsertTime.isNull())
		pFee->InsertTime = g_nCurrentTime;
	CFee *pTarget;
	//beforeAdd(pFee);
	pTarget=(CFee *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough Fee in memory database");
		return NULL;
	}
	forceCopy(pTarget, pFee, sizeof(CFee));
	pMem->updateObject(pTarget);
	if (pMemberIDAndInstrumentHashIndex != NULL)
		pMemberIDAndInstrumentHashIndex->addObject(pTarget,pTarget->HashMemberIDAndInstrument);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CFee* CFeeFactory::add(CWriteableFee *pFee, CTransaction *pTransaction)
{
	pFee->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pFee,true);
	}else{
		CFee *pNewFee;
		pNewFee = internalAdd(pFee,false);
		pTransaction->addResource(CFeeResource::alloc(CREATE_ACTION,this,pNewFee));
		return pNewFee;
	}
}

CFee* CFeeFactory::internalUpdate(CFee *pFee, CWriteableFee *pNewFee,bool bNoTransaction)
{
	pNewFee->UpdateTime = g_nCurrentTime;
	CWriteableFee theOldFee;
	if (bNoTransaction)
		forceCopy(&theOldFee,pFee,sizeof(CFee));	
	CFee *pTarget = pFee;
	forceCopy(pFee,pNewFee,sizeof(CFee));
	pMem->updateObject(pFee);
	if (bNoTransaction)
		commitUpdate(pFee,&theOldFee);
	return pFee;
}

CFee* CFeeFactory::update(CFee *pFee, CWriteableFee *pNewFee, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pFee == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pFee,pNewFee,bNoTransaction);
	}else{
		pTransaction->addResource(CFeeResource::alloc(UPDATE_ACTION,this,pFee));
		return internalUpdate(pFee,pNewFee,false);
	}
}

void CFeeFactory::internalRemove(CFee *pFee, bool bNoTransaction)
{
	CWriteableFee theOldFee;
	//beforeRemove(pFee);
	if (bNoTransaction)
		forceCopy(&theOldFee,pFee,sizeof(CFee));
	CFee *pTarget = pFee;
	
	if (pMemberIDAndInstrumentHashIndex != NULL)
		pMemberIDAndInstrumentHashIndex->removeObject(pFee,pFee->HashMemberIDAndInstrument);		
	pMem->free(pFee);
	if(bNoTransaction)
		commitRemove(&theOldFee);
}

void CFeeFactory::remove(CFee *pFee, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pFee == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pFee, bNoTransaction);
	}else{
		pTransaction->addResource(CFeeResource::alloc(DELETE_ACTION,this,pFee));
		internalRemove(pFee,false);
	}
}

CFee* CFeeFactory::addOrUpdate(CFee *pFee, CWriteableFee *pNewFee, CTransaction *pTransaction)
{
	if(pFee == NULL)
		return add(pNewFee,pTransaction);
	else
		return update(pFee,pNewFee,pTransaction);
}

void CFeeFactory::retrieve(CFee *pFee, CWriteableFee *pTargetFee)
{
	if(pFee == NULL)
		return;
	forceCopy(pTargetFee, pFee, sizeof(CFee));
}
	
int CFeeFactory::addActionTrigger(CFeeActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CFeeFactory::removeActionTrigger(CFeeActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CFeeFactory::addCommitTrigger(CFeeCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CFeeFactory::removeCommitTrigger(CFeeCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CFee* CFeeFactory::getFirst()
{
	return (CFee *)(pMem->getFirst());
}
	
CFee* CFeeFactory::getNext()
{
	return (CFee *)(pMem->getNext());
}

void CFeeFactory::commitAdd(CFee *pFee)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pFee);
	}
}

void CFeeFactory::commitUpdate(CFee *pFee, CWriteableFee *pOldFee)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pFee,pOldFee);
	}
}
	
void CFeeFactory::commitRemove(CWriteableFee *pFee)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pFee);
	}
}

void CFeeFactory::checkNull(FILE *pFile)
{
	CFee *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "Fee->MemberID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "Fee->InstrumentID:NULL\n");
		}
		if(pTarget->MatchRole.isNull()){
			bNull = true;
			fprintf(pFile, "Fee->MatchRole:NULL\n");
		}
		if(pTarget->OpenFeeRate.isNull()){
			bNull = true;
			fprintf(pFile, "Fee->OpenFeeRate:NULL\n");
		}
		if(pTarget->OpenFeeAmount.isNull()){
			bNull = true;
			fprintf(pFile, "Fee->OpenFeeAmount:NULL\n");
		}
		if(pTarget->CloseFeeRate.isNull()){
			bNull = true;
			fprintf(pFile, "Fee->CloseFeeRate:NULL\n");
		}
		if(pTarget->CloseFeeAmount.isNull()){
			bNull = true;
			fprintf(pFile, "Fee->CloseFeeAmount:NULL\n");
		}
		if(pTarget->CloseTodayFeeRate.isNull()){
			bNull = true;
			fprintf(pFile, "Fee->CloseTodayFeeRate:NULL\n");
		}
		if(pTarget->CloseTodayFeeAmount.isNull()){
			bNull = true;
			fprintf(pFile, "Fee->CloseTodayFeeAmount:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CFeeFactory::checkValid(FILE *pFile)
{
	CFee *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->MemberID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->InstrumentID:inValid\n");
		}
		if(!pTarget->MatchRole.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->MatchRole:inValid\n");
		}
		if(!pTarget->OpenFeeRate.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->OpenFeeRate:inValid\n");
		}
		if(!pTarget->OpenFeeAmount.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->OpenFeeAmount:inValid\n");
		}
		if(!pTarget->CloseFeeRate.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->CloseFeeRate:inValid\n");
		}
		if(!pTarget->CloseFeeAmount.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->CloseFeeAmount:inValid\n");
		}
		if(!pTarget->CloseTodayFeeRate.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->CloseTodayFeeRate:inValid\n");
		}
		if(!pTarget->CloseTodayFeeAmount.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->CloseTodayFeeAmount:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->Remark:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Fee->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

extern int compareForSettlementGroupIndexinCurrency(const void *pV1, const void *pV2);
void CCurrencyFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pSettlementGroupIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 'p':
	{
		if(!reuse) {
			pSettlementGroupIndex=new CAVLTree(maxUnit,compareForSettlementGroupIndexinCurrency,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Currency_SettlementGroupIndex",pSettlementGroupIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Currency_SettlementGroupIndex");
			if(it != pIndexMap->end()) {
				pSettlementGroupIndex=new CAVLTree(maxUnit,compareForSettlementGroupIndexinCurrency,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pSettlementGroupIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pCurrencyHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 'r':
	case 'p':
	{
		int hashKeySize=98317;
		if(!reuse) {
			pCurrencyHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Currency_CurrencyHashIndex",pCurrencyHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("Currency_CurrencyHashIndex");
			if(it != pIndexMap->end()) {
				pCurrencyHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pCurrencyHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pLastFoundInSearchBySettlementGroup=NULL;
	pActionTriggers=new vector<CCurrencyActionTrigger *>;
	pCommitTriggers=new vector<CCurrencyCommitTrigger *>;
}

CCurrencyFactory::CCurrencyFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CCurrency),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CCurrencyFactory::CCurrencyFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CCurrency),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CCurrencyFactory::~CCurrencyFactory()
{
	if (pSettlementGroupIndex!=NULL)
		delete pSettlementGroupIndex;
	if (pCurrencyHashIndex!=NULL)
		delete pCurrencyHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CCurrencyFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableCurrency thisCurrency;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisCurrency.readCSV(input,pNames))
		add(&thisCurrency);
	fclose(input);
	delete pNames;
	return 1;
}

int CCurrencyFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Currency.csv");
	return readCSV(szFileName);
}

int CCurrencyFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableCurrency *pCurrency;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableCurrency::writeCSVHead(output);
	pCurrency=(CWriteableCurrency *)(pMem->getFirst());
	while (pCurrency!=NULL) {
		pCurrency->writeCSV(output);
		pCurrency=(CWriteableCurrency *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CCurrencyFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableCurrency *pCurrency;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableCurrency::writeCSVHeadPreVersion(output);
	pCurrency=(CWriteableCurrency *)(pMem->getFirst());
	while (pCurrency!=NULL) {
		pCurrency->writeCSVPreVersion(output);
		pCurrency=(CWriteableCurrency *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CCurrencyFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Currency.csv");
	return writeCSV(szFileName);
}

void CCurrencyFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CCurrencyFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableCurrency *pCurrency;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CCurrencyFactory={       Total Count=%d\n", pMem->getCount());
	pCurrency=(CWriteableCurrency *)(pMem->getFirst());
	while (pCurrency!=NULL) {
		pCurrency->dump(fp,index++);
		pCurrency=(CWriteableCurrency *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CCurrencyFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pSettlementGroupIndex != NULL)
		pSettlementGroupIndex->removeAll();
	if(pCurrencyHashIndex != NULL)
		pCurrencyHashIndex->removeAll();
}

CCurrency *CCurrencyFactory::internalAdd(CWriteableCurrency *pCurrency, bool bNoTransaction)
{
	pCurrency->m_pMDB = m_pMDB;
	if(pCurrency->CanReduce.isNull())
		pCurrency->CanReduce = 1;
	if(pCurrency->IsTrading.isNull())
		pCurrency->IsTrading = 1;
	if(pCurrency->MoneyTick.isNull())
		pCurrency->MoneyTick = 0.00000001;
	if(pCurrency->LoanLimit.isNull())
		pCurrency->LoanLimit = 0.8;
	if(pCurrency->LoanBase.isNull())
		pCurrency->LoanBase = 0.01;
	if(pCurrency->LoanRate.isNull())
		pCurrency->LoanRate = 0.1;
	if(pCurrency->LoanHighBase.isNull())
		pCurrency->LoanHighBase = 0.9;
	if(pCurrency->LoanHighRate.isNull())
		pCurrency->LoanHighRate = 1;
	if(pCurrency->HairCut.isNull())
		pCurrency->HairCut = 0.9;
	if(pCurrency->MaxValue.isNull())
		pCurrency->MaxValue = 10000;
	if(pCurrency->InsertTime.isNull())
		pCurrency->InsertTime = g_nCurrentTime;
	CCurrency *pTarget;
	//beforeAdd(pCurrency);
	pTarget=(CCurrency *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough Currency in memory database");
		return NULL;
	}
	forceCopy(pTarget, pCurrency, sizeof(CCurrency));
	pMem->updateObject(pTarget);
	if (pSettlementGroupIndex != NULL)
		pSettlementGroupIndex->addObject(pTarget);
	if (pCurrencyHashIndex != NULL)
		pCurrencyHashIndex->addObject(pTarget,pTarget->HashCurrency);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CCurrency* CCurrencyFactory::add(CWriteableCurrency *pCurrency, CTransaction *pTransaction)
{
	pCurrency->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pCurrency,true);
	}else{
		CCurrency *pNewCurrency;
		pNewCurrency = internalAdd(pCurrency,false);
		pTransaction->addResource(CCurrencyResource::alloc(CREATE_ACTION,this,pNewCurrency));
		return pNewCurrency;
	}
}

CCurrency* CCurrencyFactory::internalUpdate(CCurrency *pCurrency, CWriteableCurrency *pNewCurrency,bool bNoTransaction)
{
	pNewCurrency->UpdateTime = g_nCurrentTime;
	CWriteableCurrency theOldCurrency;
	if (bNoTransaction)
		forceCopy(&theOldCurrency,pCurrency,sizeof(CCurrency));	
	CCurrency *pTarget = pCurrency;
	forceCopy(pCurrency,pNewCurrency,sizeof(CCurrency));
	pMem->updateObject(pCurrency);
	if (bNoTransaction)
		commitUpdate(pCurrency,&theOldCurrency);
	return pCurrency;
}

CCurrency* CCurrencyFactory::update(CCurrency *pCurrency, CWriteableCurrency *pNewCurrency, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pCurrency == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pCurrency,pNewCurrency,bNoTransaction);
	}else{
		pTransaction->addResource(CCurrencyResource::alloc(UPDATE_ACTION,this,pCurrency));
		return internalUpdate(pCurrency,pNewCurrency,false);
	}
}

void CCurrencyFactory::internalRemove(CCurrency *pCurrency, bool bNoTransaction)
{
	CWriteableCurrency theOldCurrency;
	//beforeRemove(pCurrency);
	if (bNoTransaction)
		forceCopy(&theOldCurrency,pCurrency,sizeof(CCurrency));
	CCurrency *pTarget = pCurrency;
	if (pSettlementGroupIndex != NULL)
		pSettlementGroupIndex->removeObject(pCurrency);	
	
	if (pCurrencyHashIndex != NULL)
		pCurrencyHashIndex->removeObject(pCurrency,pCurrency->HashCurrency);		
	pMem->free(pCurrency);
	if(bNoTransaction)
		commitRemove(&theOldCurrency);
}

void CCurrencyFactory::remove(CCurrency *pCurrency, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pCurrency == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pCurrency, bNoTransaction);
	}else{
		pTransaction->addResource(CCurrencyResource::alloc(DELETE_ACTION,this,pCurrency));
		internalRemove(pCurrency,false);
	}
}

CCurrency* CCurrencyFactory::addOrUpdate(CCurrency *pCurrency, CWriteableCurrency *pNewCurrency, CTransaction *pTransaction)
{
	if(pCurrency == NULL)
		return add(pNewCurrency,pTransaction);
	else
		return update(pCurrency,pNewCurrency,pTransaction);
}

void CCurrencyFactory::retrieve(CCurrency *pCurrency, CWriteableCurrency *pTargetCurrency)
{
	if(pCurrency == NULL)
		return;
	forceCopy(pTargetCurrency, pCurrency, sizeof(CCurrency));
}
	
int CCurrencyFactory::addActionTrigger(CCurrencyActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CCurrencyFactory::removeActionTrigger(CCurrencyActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CCurrencyFactory::addCommitTrigger(CCurrencyCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CCurrencyFactory::removeCommitTrigger(CCurrencyCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CCurrency* CCurrencyFactory::getFirst()
{
	return (CCurrency *)(pMem->getFirst());
}
	
CCurrency* CCurrencyFactory::getNext()
{
	return (CCurrency *)(pMem->getNext());
}

void CCurrencyFactory::commitAdd(CCurrency *pCurrency)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pCurrency);
	}
}

void CCurrencyFactory::commitUpdate(CCurrency *pCurrency, CWriteableCurrency *pOldCurrency)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pCurrency,pOldCurrency);
	}
}
	
void CCurrencyFactory::commitRemove(CWriteableCurrency *pCurrency)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pCurrency);
	}
}

void CCurrencyFactory::checkNull(FILE *pFile)
{
	CCurrency *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->SettlementGroup.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->SettlementGroup:NULL\n");
		}
		if(pTarget->Currency.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->Currency:NULL\n");
		}
		if(pTarget->Deposit.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->Deposit:NULL\n");
		}
		if(pTarget->Withdraw.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->Withdraw:NULL\n");
		}
		if(pTarget->Balance.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->Balance:NULL\n");
		}
		if(pTarget->PreBalance.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->PreBalance:NULL\n");
		}
		if(pTarget->CanReduce.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->CanReduce:NULL\n");
		}
		if(pTarget->IsTrading.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->IsTrading:NULL\n");
		}
		if(pTarget->MoneyTick.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->MoneyTick:NULL\n");
		}
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->InstrumentID:NULL\n");
		}
		if(pTarget->LoanUse.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->LoanUse:NULL\n");
		}
		if(pTarget->LoanLimit.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->LoanLimit:NULL\n");
		}
		if(pTarget->LoanBorrow.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->LoanBorrow:NULL\n");
		}
		if(pTarget->LoanDeposit.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->LoanDeposit:NULL\n");
		}
		if(pTarget->LoanBase.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->LoanBase:NULL\n");
		}
		if(pTarget->LoanRate.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->LoanRate:NULL\n");
		}
		if(pTarget->LoanHighBase.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->LoanHighBase:NULL\n");
		}
		if(pTarget->LoanHighRate.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->LoanHighRate:NULL\n");
		}
		if(pTarget->CurrValue.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->CurrValue:NULL\n");
		}
		if(pTarget->HairCut.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->HairCut:NULL\n");
		}
		if(pTarget->MaxValue.isNull()){
			bNull = true;
			fprintf(pFile, "Currency->MaxValue:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CCurrencyFactory::checkValid(FILE *pFile)
{
	CCurrency *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->SettlementGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->SettlementGroup:inValid\n");
		}
		if(!pTarget->Currency.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->Currency:inValid\n");
		}
		if(!pTarget->Deposit.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->Deposit:inValid\n");
		}
		if(!pTarget->Withdraw.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->Withdraw:inValid\n");
		}
		if(!pTarget->Balance.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->Balance:inValid\n");
		}
		if(!pTarget->PreBalance.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->PreBalance:inValid\n");
		}
		if(!pTarget->SortNumber.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->SortNumber:inValid\n");
		}
		if(!pTarget->CanReduce.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->CanReduce:inValid\n");
		}
		if(!pTarget->IsTrading.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->IsTrading:inValid\n");
		}
		if(!pTarget->SettleSegment.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->SettleSegment:inValid\n");
		}
		if(!pTarget->MoneyTick.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->MoneyTick:inValid\n");
		}
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->InstrumentID:inValid\n");
		}
		if(!pTarget->LoanUse.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->LoanUse:inValid\n");
		}
		if(!pTarget->LoanLimit.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->LoanLimit:inValid\n");
		}
		if(!pTarget->LoanBorrow.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->LoanBorrow:inValid\n");
		}
		if(!pTarget->LoanDeposit.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->LoanDeposit:inValid\n");
		}
		if(!pTarget->LoanBase.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->LoanBase:inValid\n");
		}
		if(!pTarget->LoanRate.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->LoanRate:inValid\n");
		}
		if(!pTarget->LoanHighBase.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->LoanHighBase:inValid\n");
		}
		if(!pTarget->LoanHighRate.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->LoanHighRate:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->Remark:inValid\n");
		}
		if(!pTarget->CurrValue.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->CurrValue:inValid\n");
		}
		if(!pTarget->HairCut.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->HairCut:inValid\n");
		}
		if(!pTarget->MaxValue.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->MaxValue:inValid\n");
		}
		if(!pTarget->MarginRateGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->MarginRateGroup:inValid\n");
		}
		if(!pTarget->PrivateEnd.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->PrivateEnd:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Currency->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CCurrencyFactory::linkAllMarketData()
{
	CCurrency *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkMarketData();
		pTarget=getNext();
	}
}

void CCurrencyFactory::checkLinkMarketData(FILE *pFile)
{
	CCurrency *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getMarketData() == NULL){
			if (pTarget->ExchangeID.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->InstrumentID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Currency->MarketData:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CCurrencyFactory::clearLinkMarketData()
{
}

void CLiquidityFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pLiquidityHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		int hashKeySize=1024;
		if(!reuse) {
			pLiquidityHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Liquidity_LiquidityHashIndex",pLiquidityHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("Liquidity_LiquidityHashIndex");
			if(it != pIndexMap->end()) {
				pLiquidityHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pLiquidityHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pActionTriggers=new vector<CLiquidityActionTrigger *>;
	pCommitTriggers=new vector<CLiquidityCommitTrigger *>;
}

CLiquidityFactory::CLiquidityFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CLiquidity),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CLiquidityFactory::CLiquidityFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CLiquidity),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CLiquidityFactory::~CLiquidityFactory()
{
	if (pLiquidityHashIndex!=NULL)
		delete pLiquidityHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CLiquidityFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableLiquidity thisLiquidity;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisLiquidity.readCSV(input,pNames))
		add(&thisLiquidity);
	fclose(input);
	delete pNames;
	return 1;
}

int CLiquidityFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Liquidity.csv");
	return readCSV(szFileName);
}

int CLiquidityFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableLiquidity *pLiquidity;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableLiquidity::writeCSVHead(output);
	pLiquidity=(CWriteableLiquidity *)(pMem->getFirst());
	while (pLiquidity!=NULL) {
		pLiquidity->writeCSV(output);
		pLiquidity=(CWriteableLiquidity *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CLiquidityFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableLiquidity *pLiquidity;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableLiquidity::writeCSVHeadPreVersion(output);
	pLiquidity=(CWriteableLiquidity *)(pMem->getFirst());
	while (pLiquidity!=NULL) {
		pLiquidity->writeCSVPreVersion(output);
		pLiquidity=(CWriteableLiquidity *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CLiquidityFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Liquidity.csv");
	return writeCSV(szFileName);
}

void CLiquidityFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CLiquidityFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableLiquidity *pLiquidity;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CLiquidityFactory={       Total Count=%d\n", pMem->getCount());
	pLiquidity=(CWriteableLiquidity *)(pMem->getFirst());
	while (pLiquidity!=NULL) {
		pLiquidity->dump(fp,index++);
		pLiquidity=(CWriteableLiquidity *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CLiquidityFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pLiquidityHashIndex != NULL)
		pLiquidityHashIndex->removeAll();
}

CLiquidity *CLiquidityFactory::internalAdd(CWriteableLiquidity *pLiquidity, bool bNoTransaction)
{
	pLiquidity->m_pMDB = m_pMDB;
	if(pLiquidity->InsertTime.isNull())
		pLiquidity->InsertTime = g_nCurrentTime;
	CLiquidity *pTarget;
	//beforeAdd(pLiquidity);
	pTarget=(CLiquidity *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough Liquidity in memory database");
		return NULL;
	}
	forceCopy(pTarget, pLiquidity, sizeof(CLiquidity));
	pMem->updateObject(pTarget);
	if (pLiquidityHashIndex != NULL)
		pLiquidityHashIndex->addObject(pTarget,pTarget->HashLiquidity);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CLiquidity* CLiquidityFactory::add(CWriteableLiquidity *pLiquidity, CTransaction *pTransaction)
{
	pLiquidity->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pLiquidity,true);
	}else{
		CLiquidity *pNewLiquidity;
		pNewLiquidity = internalAdd(pLiquidity,false);
		pTransaction->addResource(CLiquidityResource::alloc(CREATE_ACTION,this,pNewLiquidity));
		return pNewLiquidity;
	}
}

CLiquidity* CLiquidityFactory::internalUpdate(CLiquidity *pLiquidity, CWriteableLiquidity *pNewLiquidity,bool bNoTransaction)
{
	pNewLiquidity->UpdateTime = g_nCurrentTime;
	CWriteableLiquidity theOldLiquidity;
	if (bNoTransaction)
		forceCopy(&theOldLiquidity,pLiquidity,sizeof(CLiquidity));	
	CLiquidity *pTarget = pLiquidity;
	forceCopy(pLiquidity,pNewLiquidity,sizeof(CLiquidity));
	pMem->updateObject(pLiquidity);
	if (bNoTransaction)
		commitUpdate(pLiquidity,&theOldLiquidity);
	return pLiquidity;
}

CLiquidity* CLiquidityFactory::update(CLiquidity *pLiquidity, CWriteableLiquidity *pNewLiquidity, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pLiquidity == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pLiquidity,pNewLiquidity,bNoTransaction);
	}else{
		pTransaction->addResource(CLiquidityResource::alloc(UPDATE_ACTION,this,pLiquidity));
		return internalUpdate(pLiquidity,pNewLiquidity,false);
	}
}

void CLiquidityFactory::internalRemove(CLiquidity *pLiquidity, bool bNoTransaction)
{
	CWriteableLiquidity theOldLiquidity;
	//beforeRemove(pLiquidity);
	if (bNoTransaction)
		forceCopy(&theOldLiquidity,pLiquidity,sizeof(CLiquidity));
	CLiquidity *pTarget = pLiquidity;
	
	if (pLiquidityHashIndex != NULL)
		pLiquidityHashIndex->removeObject(pLiquidity,pLiquidity->HashLiquidity);		
	pMem->free(pLiquidity);
	if(bNoTransaction)
		commitRemove(&theOldLiquidity);
}

void CLiquidityFactory::remove(CLiquidity *pLiquidity, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pLiquidity == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pLiquidity, bNoTransaction);
	}else{
		pTransaction->addResource(CLiquidityResource::alloc(DELETE_ACTION,this,pLiquidity));
		internalRemove(pLiquidity,false);
	}
}

CLiquidity* CLiquidityFactory::addOrUpdate(CLiquidity *pLiquidity, CWriteableLiquidity *pNewLiquidity, CTransaction *pTransaction)
{
	if(pLiquidity == NULL)
		return add(pNewLiquidity,pTransaction);
	else
		return update(pLiquidity,pNewLiquidity,pTransaction);
}

void CLiquidityFactory::retrieve(CLiquidity *pLiquidity, CWriteableLiquidity *pTargetLiquidity)
{
	if(pLiquidity == NULL)
		return;
	forceCopy(pTargetLiquidity, pLiquidity, sizeof(CLiquidity));
}
	
int CLiquidityFactory::addActionTrigger(CLiquidityActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CLiquidityFactory::removeActionTrigger(CLiquidityActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CLiquidityFactory::addCommitTrigger(CLiquidityCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CLiquidityFactory::removeCommitTrigger(CLiquidityCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CLiquidity* CLiquidityFactory::getFirst()
{
	return (CLiquidity *)(pMem->getFirst());
}
	
CLiquidity* CLiquidityFactory::getNext()
{
	return (CLiquidity *)(pMem->getNext());
}

void CLiquidityFactory::commitAdd(CLiquidity *pLiquidity)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pLiquidity);
	}
}

void CLiquidityFactory::commitUpdate(CLiquidity *pLiquidity, CWriteableLiquidity *pOldLiquidity)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pLiquidity,pOldLiquidity);
	}
}
	
void CLiquidityFactory::commitRemove(CWriteableLiquidity *pLiquidity)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pLiquidity);
	}
}

void CLiquidityFactory::checkNull(FILE *pFile)
{
	CLiquidity *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->LiquidityGrade.isNull()){
			bNull = true;
			fprintf(pFile, "Liquidity->LiquidityGrade:NULL\n");
		}
		if(pTarget->LiquidityGroup.isNull()){
			bNull = true;
			fprintf(pFile, "Liquidity->LiquidityGroup:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CLiquidityFactory::checkValid(FILE *pFile)
{
	CLiquidity *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->LiquidityGrade.isValid()){
			bValid = false;
			fprintf(pFile, "Liquidity->LiquidityGrade:inValid\n");
		}
		if(!pTarget->LiquidityGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Liquidity->LiquidityGroup:inValid\n");
		}
		if(!pTarget->BasePriceType.isValid()){
			bValid = false;
			fprintf(pFile, "Liquidity->BasePriceType:inValid\n");
		}
		if(!pTarget->VolumeBegin.isValid()){
			bValid = false;
			fprintf(pFile, "Liquidity->VolumeBegin:inValid\n");
		}
		if(!pTarget->BuyOpen.isValid()){
			bValid = false;
			fprintf(pFile, "Liquidity->BuyOpen:inValid\n");
		}
		if(!pTarget->BuyClose.isValid()){
			bValid = false;
			fprintf(pFile, "Liquidity->BuyClose:inValid\n");
		}
		if(!pTarget->SellOpen.isValid()){
			bValid = false;
			fprintf(pFile, "Liquidity->SellOpen:inValid\n");
		}
		if(!pTarget->SellClose.isValid()){
			bValid = false;
			fprintf(pFile, "Liquidity->SellClose:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "Liquidity->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Liquidity->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

extern int compareForMemberIndexinAccount(const void *pV1, const void *pV2);
extern int compareForCurrencyIndexinAccount(const void *pV1, const void *pV2);
void CAccountFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pMemberIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 'r':
	case 'p':
	{
		if(!reuse) {
			pMemberIndex=new CAVLTree(maxUnit,compareForMemberIndexinAccount,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Account_MemberIndex",pMemberIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Account_MemberIndex");
			if(it != pIndexMap->end()) {
				pMemberIndex=new CAVLTree(maxUnit,compareForMemberIndexinAccount,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pMemberIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pCurrencyIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 'p':
	{
		if(!reuse) {
			pCurrencyIndex=new CAVLTree(maxUnit,compareForCurrencyIndexinAccount,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Account_CurrencyIndex",pCurrencyIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Account_CurrencyIndex");
			if(it != pIndexMap->end()) {
				pCurrencyIndex=new CAVLTree(maxUnit,compareForCurrencyIndexinAccount,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pCurrencyIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pLastFoundInSearchByMemberID=NULL;
	pLastFoundInSearchByAccountID=NULL;
	pLastFoundInSearchBySettlementGroup=NULL;
	pLastFoundInSearchByAll=NULL;
	pLastFoundInSearchByCurrency=NULL;
	pActionTriggers=new vector<CAccountActionTrigger *>;
	pCommitTriggers=new vector<CAccountCommitTrigger *>;
}

CAccountFactory::CAccountFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CAccount),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CAccountFactory::CAccountFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CAccount),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CAccountFactory::~CAccountFactory()
{
	if (pMemberIndex!=NULL)
		delete pMemberIndex;
	if (pCurrencyIndex!=NULL)
		delete pCurrencyIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CAccountFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableAccount thisAccount;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisAccount.readCSV(input,pNames))
		add(&thisAccount);
	fclose(input);
	delete pNames;
	return 1;
}

int CAccountFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Account.csv");
	return readCSV(szFileName);
}

int CAccountFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableAccount *pAccount;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableAccount::writeCSVHead(output);
	pAccount=(CWriteableAccount *)(pMem->getFirst());
	while (pAccount!=NULL) {
		pAccount->writeCSV(output);
		pAccount=(CWriteableAccount *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CAccountFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableAccount *pAccount;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableAccount::writeCSVHeadPreVersion(output);
	pAccount=(CWriteableAccount *)(pMem->getFirst());
	while (pAccount!=NULL) {
		pAccount->writeCSVPreVersion(output);
		pAccount=(CWriteableAccount *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CAccountFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Account.csv");
	return writeCSV(szFileName);
}

void CAccountFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CAccountFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableAccount *pAccount;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CAccountFactory={       Total Count=%d\n", pMem->getCount());
	pAccount=(CWriteableAccount *)(pMem->getFirst());
	while (pAccount!=NULL) {
		pAccount->dump(fp,index++);
		pAccount=(CWriteableAccount *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CAccountFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pMemberIndex != NULL)
		pMemberIndex->removeAll();
	if(pCurrencyIndex != NULL)
		pCurrencyIndex->removeAll();
}

CAccount *CAccountFactory::internalAdd(CWriteableAccount *pAccount, bool bNoTransaction)
{
	pAccount->m_pMDB = m_pMDB;
	if(pAccount->InsertTime.isNull())
		pAccount->InsertTime = g_nCurrentTime;
	if(pAccount->CanReduce.isNull())
		pAccount->CanReduce = 1;
	CAccount *pTarget;
	//beforeAdd(pAccount);
	pTarget=(CAccount *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough Account in memory database");
		return NULL;
	}
	forceCopy(pTarget, pAccount, sizeof(CAccount));
	pMem->updateObject(pTarget);
	if (pMemberIndex != NULL)
		pMemberIndex->addObject(pTarget);
	if (pCurrencyIndex != NULL)
		pCurrencyIndex->addObject(pTarget);
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CAccount* CAccountFactory::add(CWriteableAccount *pAccount, CTransaction *pTransaction)
{
	pAccount->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pAccount,true);
	}else{
		CAccount *pNewAccount;
		pNewAccount = internalAdd(pAccount,false);
		pTransaction->addResource(CAccountResource::alloc(CREATE_ACTION,this,pNewAccount));
		return pNewAccount;
	}
}

CAccount* CAccountFactory::internalUpdate(CAccount *pAccount, CWriteableAccount *pNewAccount,bool bNoTransaction)
{
	pNewAccount->UpdateTime = g_nCurrentTime;
	CWriteableAccount theOldAccount;
	if (bNoTransaction)
		forceCopy(&theOldAccount,pAccount,sizeof(CAccount));	
	CAVLNode *pCurrencyIndexNode=NULL;
	CAccount *pTarget = pAccount;
	if (pCurrencyIndex != NULL)
		pCurrencyIndexNode=pCurrencyIndex->findObject(pAccount);	
	forceCopy(pAccount,pNewAccount,sizeof(CAccount));
	pMem->updateObject(pAccount);
	if (pCurrencyIndex != NULL)
		pCurrencyIndex->updateNode(pCurrencyIndexNode);
	if (bNoTransaction)
		commitUpdate(pAccount,&theOldAccount);
	return pAccount;
}

CAccount* CAccountFactory::update(CAccount *pAccount, CWriteableAccount *pNewAccount, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pAccount == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pAccount,pNewAccount,bNoTransaction);
	}else{
		pTransaction->addResource(CAccountResource::alloc(UPDATE_ACTION,this,pAccount));
		return internalUpdate(pAccount,pNewAccount,false);
	}
}

void CAccountFactory::internalRemove(CAccount *pAccount, bool bNoTransaction)
{
	CWriteableAccount theOldAccount;
	//beforeRemove(pAccount);
	if (bNoTransaction)
		forceCopy(&theOldAccount,pAccount,sizeof(CAccount));
	CAccount *pTarget = pAccount;
	if (pMemberIndex != NULL)
		pMemberIndex->removeObject(pAccount);	
	if (pCurrencyIndex != NULL)
		pCurrencyIndex->removeObject(pAccount);	
	
	pMem->free(pAccount);
	if(bNoTransaction)
		commitRemove(&theOldAccount);
}

void CAccountFactory::remove(CAccount *pAccount, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pAccount == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pAccount, bNoTransaction);
	}else{
		pTransaction->addResource(CAccountResource::alloc(DELETE_ACTION,this,pAccount));
		internalRemove(pAccount,false);
	}
}

CAccount* CAccountFactory::addOrUpdate(CAccount *pAccount, CWriteableAccount *pNewAccount, CTransaction *pTransaction)
{
	if(pAccount == NULL)
		return add(pNewAccount,pTransaction);
	else
		return update(pAccount,pNewAccount,pTransaction);
}

void CAccountFactory::retrieve(CAccount *pAccount, CWriteableAccount *pTargetAccount)
{
	if(pAccount == NULL)
		return;
	forceCopy(pTargetAccount, pAccount, sizeof(CAccount));
}
	
int CAccountFactory::addActionTrigger(CAccountActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CAccountFactory::removeActionTrigger(CAccountActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CAccountFactory::addCommitTrigger(CAccountCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CAccountFactory::removeCommitTrigger(CAccountCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CAccount* CAccountFactory::getFirst()
{
	return (CAccount *)(pMem->getFirst());
}
	
CAccount* CAccountFactory::getNext()
{
	return (CAccount *)(pMem->getNext());
}

void CAccountFactory::commitAdd(CAccount *pAccount)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pAccount);
	}
}

void CAccountFactory::commitUpdate(CAccount *pAccount, CWriteableAccount *pOldAccount)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pAccount,pOldAccount);
	}
}
	
void CAccountFactory::commitRemove(CWriteableAccount *pAccount)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pAccount);
	}
}

void CAccountFactory::checkNull(FILE *pFile)
{
	CAccount *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->SettlementGroup.isNull()){
			bNull = true;
			fprintf(pFile, "Account->SettlementGroup:NULL\n");
		}
		if(pTarget->AccountID.isNull()){
			bNull = true;
			fprintf(pFile, "Account->AccountID:NULL\n");
		}
		if(pTarget->Currency.isNull()){
			bNull = true;
			fprintf(pFile, "Account->Currency:NULL\n");
		}
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "Account->MemberID:NULL\n");
		}
		if(pTarget->Available.isNull()){
			bNull = true;
			fprintf(pFile, "Account->Available:NULL\n");
		}
		if(pTarget->Withdrawable.isNull()){
			bNull = true;
			fprintf(pFile, "Account->Withdrawable:NULL\n");
		}
		if(pTarget->PreBalance.isNull()){
			bNull = true;
			fprintf(pFile, "Account->PreBalance:NULL\n");
		}
		if(pTarget->Balance.isNull()){
			bNull = true;
			fprintf(pFile, "Account->Balance:NULL\n");
		}
		if(pTarget->PositionCost.isNull()){
			bNull = true;
			fprintf(pFile, "Account->PositionCost:NULL\n");
		}
		if(pTarget->UseMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Account->UseMargin:NULL\n");
		}
		if(pTarget->CrossMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Account->CrossMargin:NULL\n");
		}
		if(pTarget->CloseProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Account->CloseProfit:NULL\n");
		}
		if(pTarget->Deposit.isNull()){
			bNull = true;
			fprintf(pFile, "Account->Deposit:NULL\n");
		}
		if(pTarget->Withdraw.isNull()){
			bNull = true;
			fprintf(pFile, "Account->Withdraw:NULL\n");
		}
		if(pTarget->FrozenMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Account->FrozenMargin:NULL\n");
		}
		if(pTarget->FrozenMoney.isNull()){
			bNull = true;
			fprintf(pFile, "Account->FrozenMoney:NULL\n");
		}
		if(pTarget->FrozenFee.isNull()){
			bNull = true;
			fprintf(pFile, "Account->FrozenFee:NULL\n");
		}
		if(pTarget->Fee.isNull()){
			bNull = true;
			fprintf(pFile, "Account->Fee:NULL\n");
		}
		if(pTarget->MoneyChange.isNull()){
			bNull = true;
			fprintf(pFile, "Account->MoneyChange:NULL\n");
		}
		if(pTarget->Reserve.isNull()){
			bNull = true;
			fprintf(pFile, "Account->Reserve:NULL\n");
		}
		if(pTarget->ReserveAvailable.isNull()){
			bNull = true;
			fprintf(pFile, "Account->ReserveAvailable:NULL\n");
		}
		if(pTarget->ReserveLimit.isNull()){
			bNull = true;
			fprintf(pFile, "Account->ReserveLimit:NULL\n");
		}
		if(pTarget->ReserveProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Account->ReserveProfit:NULL\n");
		}
		if(pTarget->ReserveType.isNull()){
			bNull = true;
			fprintf(pFile, "Account->ReserveType:NULL\n");
		}
		if(pTarget->ReserveActive.isNull()){
			bNull = true;
			fprintf(pFile, "Account->ReserveActive:NULL\n");
		}
		if(pTarget->TotalCloseProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Account->TotalCloseProfit:NULL\n");
		}
		if(pTarget->TotalDeposit.isNull()){
			bNull = true;
			fprintf(pFile, "Account->TotalDeposit:NULL\n");
		}
		if(pTarget->TotalWithdraw.isNull()){
			bNull = true;
			fprintf(pFile, "Account->TotalWithdraw:NULL\n");
		}
		if(pTarget->TotalFee.isNull()){
			bNull = true;
			fprintf(pFile, "Account->TotalFee:NULL\n");
		}
		if(pTarget->TotalMoneyChange.isNull()){
			bNull = true;
			fprintf(pFile, "Account->TotalMoneyChange:NULL\n");
		}
		if(pTarget->MaintMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Account->MaintMargin:NULL\n");
		}
		if(pTarget->UnRealProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Account->UnRealProfit:NULL\n");
		}
		if(pTarget->CopyProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Account->CopyProfit:NULL\n");
		}
		if(pTarget->Borrow.isNull()){
			bNull = true;
			fprintf(pFile, "Account->Borrow:NULL\n");
		}
		if(pTarget->CurrValue.isNull()){
			bNull = true;
			fprintf(pFile, "Account->CurrValue:NULL\n");
		}
		if(pTarget->CanReduce.isNull()){
			bNull = true;
			fprintf(pFile, "Account->CanReduce:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CAccountFactory::checkValid(FILE *pFile)
{
	CAccount *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->SettlementGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Account->SettlementGroup:inValid\n");
		}
		if(!pTarget->AccountID.isValid()){
			bValid = false;
			fprintf(pFile, "Account->AccountID:inValid\n");
		}
		if(!pTarget->Currency.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Currency:inValid\n");
		}
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "Account->MemberID:inValid\n");
		}
		if(!pTarget->Available.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Available:inValid\n");
		}
		if(!pTarget->Withdrawable.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Withdrawable:inValid\n");
		}
		if(!pTarget->PreBalance.isValid()){
			bValid = false;
			fprintf(pFile, "Account->PreBalance:inValid\n");
		}
		if(!pTarget->Balance.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Balance:inValid\n");
		}
		if(!pTarget->PositionCost.isValid()){
			bValid = false;
			fprintf(pFile, "Account->PositionCost:inValid\n");
		}
		if(!pTarget->UseMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Account->UseMargin:inValid\n");
		}
		if(!pTarget->CrossMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Account->CrossMargin:inValid\n");
		}
		if(!pTarget->CloseProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Account->CloseProfit:inValid\n");
		}
		if(!pTarget->Deposit.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Deposit:inValid\n");
		}
		if(!pTarget->Withdraw.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Withdraw:inValid\n");
		}
		if(!pTarget->FrozenMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Account->FrozenMargin:inValid\n");
		}
		if(!pTarget->FrozenMoney.isValid()){
			bValid = false;
			fprintf(pFile, "Account->FrozenMoney:inValid\n");
		}
		if(!pTarget->FrozenFee.isValid()){
			bValid = false;
			fprintf(pFile, "Account->FrozenFee:inValid\n");
		}
		if(!pTarget->Fee.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Fee:inValid\n");
		}
		if(!pTarget->MoneyChange.isValid()){
			bValid = false;
			fprintf(pFile, "Account->MoneyChange:inValid\n");
		}
		if(!pTarget->Reserve.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Reserve:inValid\n");
		}
		if(!pTarget->ReserveAvailable.isValid()){
			bValid = false;
			fprintf(pFile, "Account->ReserveAvailable:inValid\n");
		}
		if(!pTarget->ReserveLimit.isValid()){
			bValid = false;
			fprintf(pFile, "Account->ReserveLimit:inValid\n");
		}
		if(!pTarget->ReserveProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Account->ReserveProfit:inValid\n");
		}
		if(!pTarget->ReserveType.isValid()){
			bValid = false;
			fprintf(pFile, "Account->ReserveType:inValid\n");
		}
		if(!pTarget->ReserveActive.isValid()){
			bValid = false;
			fprintf(pFile, "Account->ReserveActive:inValid\n");
		}
		if(!pTarget->TotalCloseProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Account->TotalCloseProfit:inValid\n");
		}
		if(!pTarget->TotalDeposit.isValid()){
			bValid = false;
			fprintf(pFile, "Account->TotalDeposit:inValid\n");
		}
		if(!pTarget->TotalWithdraw.isValid()){
			bValid = false;
			fprintf(pFile, "Account->TotalWithdraw:inValid\n");
		}
		if(!pTarget->TotalFee.isValid()){
			bValid = false;
			fprintf(pFile, "Account->TotalFee:inValid\n");
		}
		if(!pTarget->TotalMoneyChange.isValid()){
			bValid = false;
			fprintf(pFile, "Account->TotalMoneyChange:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "Account->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Account->UpdateTime:inValid\n");
		}
		if(!pTarget->AccountType.isValid()){
			bValid = false;
			fprintf(pFile, "Account->AccountType:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "Account->BusinessNo:inValid\n");
		}
		if(!pTarget->MarginRateGrade.isValid()){
			bValid = false;
			fprintf(pFile, "Account->MarginRateGrade:inValid\n");
		}
		if(!pTarget->TradingRightGrade.isValid()){
			bValid = false;
			fprintf(pFile, "Account->TradingRightGrade:inValid\n");
		}
		if(!pTarget->Frequency.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Frequency:inValid\n");
		}
		if(!pTarget->MaintMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Account->MaintMargin:inValid\n");
		}
		if(!pTarget->UnRealProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Account->UnRealProfit:inValid\n");
		}
		if(!pTarget->CopyProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Account->CopyProfit:inValid\n");
		}
		if(!pTarget->CFDType.isValid()){
			bValid = false;
			fprintf(pFile, "Account->CFDType:inValid\n");
		}
		if(!pTarget->FeeGrade.isValid()){
			bValid = false;
			fprintf(pFile, "Account->FeeGrade:inValid\n");
		}
		if(!pTarget->CFDGrade.isValid()){
			bValid = false;
			fprintf(pFile, "Account->CFDGrade:inValid\n");
		}
		if(!pTarget->Borrow.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Borrow:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "Account->Remark:inValid\n");
		}
		if(!pTarget->MaxLocalID.isValid()){
			bValid = false;
			fprintf(pFile, "Account->MaxLocalID:inValid\n");
		}
		if(!pTarget->CurrValue.isValid()){
			bValid = false;
			fprintf(pFile, "Account->CurrValue:inValid\n");
		}
		if(!pTarget->CanReduce.isValid()){
			bValid = false;
			fprintf(pFile, "Account->CanReduce:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CAccountFactory::linkAllCurrency()
{
	CAccount *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkCurrency();
		pTarget=getNext();
	}
}

void CAccountFactory::linkAllMember()
{
	CAccount *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkMember();
		pTarget=getNext();
	}
}

void CAccountFactory::checkLinkCurrency(FILE *pFile)
{
	CAccount *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getCurrency() == NULL){
			if (pTarget->Currency.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->SettlementGroup.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Account->Currency:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CAccountFactory::checkLinkMember(FILE *pFile)
{
	CAccount *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getMember() == NULL){
			if (pTarget->MemberID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Account->Member:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CAccountFactory::clearLinkCurrency()
{
}

void CAccountFactory::clearLinkMember()
{
}

void CMemberPositionFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pInstrumentHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		int hashKeySize=98317;
		if(!reuse) {
			pInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("MemberPosition_InstrumentHashIndex",pInstrumentHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("MemberPosition_InstrumentHashIndex");
			if(it != pIndexMap->end()) {
				pInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pInstrumentHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pActionTriggers=new vector<CMemberPositionActionTrigger *>;
	pCommitTriggers=new vector<CMemberPositionCommitTrigger *>;
}

CMemberPositionFactory::CMemberPositionFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CMemberPosition),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CMemberPositionFactory::CMemberPositionFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CMemberPosition),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CMemberPositionFactory::~CMemberPositionFactory()
{
	if (pInstrumentHashIndex!=NULL)
		delete pInstrumentHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CMemberPositionFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableMemberPosition thisMemberPosition;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisMemberPosition.readCSV(input,pNames))
		add(&thisMemberPosition);
	fclose(input);
	delete pNames;
	return 1;
}

int CMemberPositionFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "MemberPosition.csv");
	return readCSV(szFileName);
}

int CMemberPositionFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableMemberPosition *pMemberPosition;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableMemberPosition::writeCSVHead(output);
	pMemberPosition=(CWriteableMemberPosition *)(pMem->getFirst());
	while (pMemberPosition!=NULL) {
		pMemberPosition->writeCSV(output);
		pMemberPosition=(CWriteableMemberPosition *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CMemberPositionFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableMemberPosition *pMemberPosition;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableMemberPosition::writeCSVHeadPreVersion(output);
	pMemberPosition=(CWriteableMemberPosition *)(pMem->getFirst());
	while (pMemberPosition!=NULL) {
		pMemberPosition->writeCSVPreVersion(output);
		pMemberPosition=(CWriteableMemberPosition *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CMemberPositionFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "MemberPosition.csv");
	return writeCSV(szFileName);
}

void CMemberPositionFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CMemberPositionFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableMemberPosition *pMemberPosition;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CMemberPositionFactory={       Total Count=%d\n", pMem->getCount());
	pMemberPosition=(CWriteableMemberPosition *)(pMem->getFirst());
	while (pMemberPosition!=NULL) {
		pMemberPosition->dump(fp,index++);
		pMemberPosition=(CWriteableMemberPosition *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CMemberPositionFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pInstrumentHashIndex != NULL)
		pInstrumentHashIndex->removeAll();
}

CMemberPosition *CMemberPositionFactory::internalAdd(CWriteableMemberPosition *pMemberPosition, bool bNoTransaction)
{
	pMemberPosition->m_pMDB = m_pMDB;
	if(pMemberPosition->InsertTime.isNull())
		pMemberPosition->InsertTime = g_nCurrentTime;
	CMemberPosition *pTarget;
	//beforeAdd(pMemberPosition);
	pTarget=(CMemberPosition *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough MemberPosition in memory database");
		return NULL;
	}
	forceCopy(pTarget, pMemberPosition, sizeof(CMemberPosition));
	pMem->updateObject(pTarget);
	if (pInstrumentHashIndex != NULL)
		pInstrumentHashIndex->addObject(pTarget,pTarget->HashInstrument);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CMemberPosition* CMemberPositionFactory::add(CWriteableMemberPosition *pMemberPosition, CTransaction *pTransaction)
{
	pMemberPosition->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pMemberPosition,true);
	}else{
		CMemberPosition *pNewMemberPosition;
		pNewMemberPosition = internalAdd(pMemberPosition,false);
		pTransaction->addResource(CMemberPositionResource::alloc(CREATE_ACTION,this,pNewMemberPosition));
		return pNewMemberPosition;
	}
}

CMemberPosition* CMemberPositionFactory::internalUpdate(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pNewMemberPosition,bool bNoTransaction)
{
	pNewMemberPosition->UpdateTime = g_nCurrentTime;
	CWriteableMemberPosition theOldMemberPosition;
	if (bNoTransaction)
		forceCopy(&theOldMemberPosition,pMemberPosition,sizeof(CMemberPosition));	
	CMemberPosition *pTarget = pMemberPosition;
	forceCopy(pMemberPosition,pNewMemberPosition,sizeof(CMemberPosition));
	pMem->updateObject(pMemberPosition);
	if (bNoTransaction)
		commitUpdate(pMemberPosition,&theOldMemberPosition);
	return pMemberPosition;
}

CMemberPosition* CMemberPositionFactory::update(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pNewMemberPosition, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pMemberPosition == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pMemberPosition,pNewMemberPosition,bNoTransaction);
	}else{
		pTransaction->addResource(CMemberPositionResource::alloc(UPDATE_ACTION,this,pMemberPosition));
		return internalUpdate(pMemberPosition,pNewMemberPosition,false);
	}
}

void CMemberPositionFactory::internalRemove(CMemberPosition *pMemberPosition, bool bNoTransaction)
{
	CWriteableMemberPosition theOldMemberPosition;
	//beforeRemove(pMemberPosition);
	if (bNoTransaction)
		forceCopy(&theOldMemberPosition,pMemberPosition,sizeof(CMemberPosition));
	CMemberPosition *pTarget = pMemberPosition;
	
	if (pInstrumentHashIndex != NULL)
		pInstrumentHashIndex->removeObject(pMemberPosition,pMemberPosition->HashInstrument);		
	pMem->free(pMemberPosition);
	if(bNoTransaction)
		commitRemove(&theOldMemberPosition);
}

void CMemberPositionFactory::remove(CMemberPosition *pMemberPosition, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pMemberPosition == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pMemberPosition, bNoTransaction);
	}else{
		pTransaction->addResource(CMemberPositionResource::alloc(DELETE_ACTION,this,pMemberPosition));
		internalRemove(pMemberPosition,false);
	}
}

CMemberPosition* CMemberPositionFactory::addOrUpdate(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pNewMemberPosition, CTransaction *pTransaction)
{
	if(pMemberPosition == NULL)
		return add(pNewMemberPosition,pTransaction);
	else
		return update(pMemberPosition,pNewMemberPosition,pTransaction);
}

void CMemberPositionFactory::retrieve(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pTargetMemberPosition)
{
	if(pMemberPosition == NULL)
		return;
	forceCopy(pTargetMemberPosition, pMemberPosition, sizeof(CMemberPosition));
}
	
int CMemberPositionFactory::addActionTrigger(CMemberPositionActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CMemberPositionFactory::removeActionTrigger(CMemberPositionActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CMemberPositionFactory::addCommitTrigger(CMemberPositionCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CMemberPositionFactory::removeCommitTrigger(CMemberPositionCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CMemberPosition* CMemberPositionFactory::getFirst()
{
	return (CMemberPosition *)(pMem->getFirst());
}
	
CMemberPosition* CMemberPositionFactory::getNext()
{
	return (CMemberPosition *)(pMem->getNext());
}

void CMemberPositionFactory::commitAdd(CMemberPosition *pMemberPosition)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pMemberPosition);
	}
}

void CMemberPositionFactory::commitUpdate(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pOldMemberPosition)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pMemberPosition,pOldMemberPosition);
	}
}
	
void CMemberPositionFactory::commitRemove(CWriteableMemberPosition *pMemberPosition)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pMemberPosition);
	}
}

void CMemberPositionFactory::checkNull(FILE *pFile)
{
	CMemberPosition *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "MemberPosition->MemberID:NULL\n");
		}
		if(pTarget->AccountID.isNull()){
			bNull = true;
			fprintf(pFile, "MemberPosition->AccountID:NULL\n");
		}
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "MemberPosition->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "MemberPosition->InstrumentID:NULL\n");
		}
		if(pTarget->Position.isNull()){
			bNull = true;
			fprintf(pFile, "MemberPosition->Position:NULL\n");
		}
		if(pTarget->LongFrozen.isNull()){
			bNull = true;
			fprintf(pFile, "MemberPosition->LongFrozen:NULL\n");
		}
		if(pTarget->ShortFrozen.isNull()){
			bNull = true;
			fprintf(pFile, "MemberPosition->ShortFrozen:NULL\n");
		}
		if(pTarget->DefaultCrossMargin.isNull()){
			bNull = true;
			fprintf(pFile, "MemberPosition->DefaultCrossMargin:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CMemberPositionFactory::checkValid(FILE *pFile)
{
	CMemberPosition *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->MemberID:inValid\n");
		}
		if(!pTarget->AccountID.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->AccountID:inValid\n");
		}
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->InstrumentID:inValid\n");
		}
		if(!pTarget->Position.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->Position:inValid\n");
		}
		if(!pTarget->LongFrozen.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->LongFrozen:inValid\n");
		}
		if(!pTarget->ShortFrozen.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->ShortFrozen:inValid\n");
		}
		if(!pTarget->DefaultLeverage.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->DefaultLeverage:inValid\n");
		}
		if(!pTarget->DefaultCrossMargin.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->DefaultCrossMargin:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "MemberPosition->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

extern int compareForAccountIndexinPosition(const void *pV1, const void *pV2);
extern int compareForLiquidPriceLongIndexinPosition(const void *pV1, const void *pV2);
extern int compareForLiquidPriceShortIndexinPosition(const void *pV1, const void *pV2);
void CPositionFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pAccountIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 'r':
	case 'p':
	{
		if(!reuse) {
			pAccountIndex=new CAVLTree(maxUnit,compareForAccountIndexinPosition,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Position_AccountIndex",pAccountIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Position_AccountIndex");
			if(it != pIndexMap->end()) {
				pAccountIndex=new CAVLTree(maxUnit,compareForAccountIndexinPosition,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pAccountIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pLiquidPriceLongIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 'r':
	{
		if(!reuse) {
			pLiquidPriceLongIndex=new CAVLTree(maxUnit,compareForLiquidPriceLongIndexinPosition,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Position_LiquidPriceLongIndex",pLiquidPriceLongIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Position_LiquidPriceLongIndex");
			if(it != pIndexMap->end()) {
				pLiquidPriceLongIndex=new CAVLTree(maxUnit,compareForLiquidPriceLongIndexinPosition,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pLiquidPriceLongIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pLiquidPriceShortIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 'r':
	{
		if(!reuse) {
			pLiquidPriceShortIndex=new CAVLTree(maxUnit,compareForLiquidPriceShortIndexinPosition,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Position_LiquidPriceShortIndex",pLiquidPriceShortIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Position_LiquidPriceShortIndex");
			if(it != pIndexMap->end()) {
				pLiquidPriceShortIndex=new CAVLTree(maxUnit,compareForLiquidPriceShortIndexinPosition,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pLiquidPriceShortIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pPositionIDHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 'r':
	case 't':
	case 'p':
	{
		int hashKeySize=1572869;
		if(!reuse) {
			pPositionIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Position_PositionIDHashIndex",pPositionIDHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("Position_PositionIDHashIndex");
			if(it != pIndexMap->end()) {
				pPositionIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pPositionIDHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pLastFoundInSearchByAccountInstrument=NULL;
	pLastFoundInSearchByAccountID=NULL;
	pLastFoundInSearchByLiquidPriceLong=NULL;
	pLastFoundInSearchByLiquidPriceShort=NULL;
	pActionTriggers=new vector<CPositionActionTrigger *>;
	pCommitTriggers=new vector<CPositionCommitTrigger *>;
}

CPositionFactory::CPositionFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CPosition),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CPositionFactory::CPositionFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CPosition),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CPositionFactory::~CPositionFactory()
{
	if (pAccountIndex!=NULL)
		delete pAccountIndex;
	if (pLiquidPriceLongIndex!=NULL)
		delete pLiquidPriceLongIndex;
	if (pLiquidPriceShortIndex!=NULL)
		delete pLiquidPriceShortIndex;
	if (pPositionIDHashIndex!=NULL)
		delete pPositionIDHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CPositionFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteablePosition thisPosition;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisPosition.readCSV(input,pNames))
		add(&thisPosition);
	fclose(input);
	delete pNames;
	return 1;
}

int CPositionFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Position.csv");
	return readCSV(szFileName);
}

int CPositionFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteablePosition *pPosition;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteablePosition::writeCSVHead(output);
	pPosition=(CWriteablePosition *)(pMem->getFirst());
	while (pPosition!=NULL) {
		pPosition->writeCSV(output);
		pPosition=(CWriteablePosition *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CPositionFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteablePosition *pPosition;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteablePosition::writeCSVHeadPreVersion(output);
	pPosition=(CWriteablePosition *)(pMem->getFirst());
	while (pPosition!=NULL) {
		pPosition->writeCSVPreVersion(output);
		pPosition=(CWriteablePosition *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CPositionFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Position.csv");
	return writeCSV(szFileName);
}

void CPositionFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CPositionFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteablePosition *pPosition;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CPositionFactory={       Total Count=%d\n", pMem->getCount());
	pPosition=(CWriteablePosition *)(pMem->getFirst());
	while (pPosition!=NULL) {
		pPosition->dump(fp,index++);
		pPosition=(CWriteablePosition *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CPositionFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pAccountIndex != NULL)
		pAccountIndex->removeAll();
	if(pLiquidPriceLongIndex != NULL)
		pLiquidPriceLongIndex->removeAll();
	if(pLiquidPriceShortIndex != NULL)
		pLiquidPriceShortIndex->removeAll();
	if(pPositionIDHashIndex != NULL)
		pPositionIDHashIndex->removeAll();
}

CPosition *CPositionFactory::internalAdd(CWriteablePosition *pPosition, bool bNoTransaction)
{
	pPosition->m_pMDB = m_pMDB;
	if(pPosition->InsertTime.isNull())
		pPosition->InsertTime = g_nCurrentTime;
	CPosition *pTarget;
	//beforeAdd(pPosition);
	pTarget=(CPosition *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough Position in memory database");
		return NULL;
	}
	forceCopy(pTarget, pPosition, sizeof(CPosition));
	pMem->updateObject(pTarget);
	if (pAccountIndex != NULL)
		pAccountIndex->addObject(pTarget);
	if (pTarget->IsCrossMargin == 0 && pTarget->Position != 0 && ! (pTarget->PosiDirection == PD_Short || pTarget->Position < 0)){
		if (pLiquidPriceLongIndex != NULL)
			pLiquidPriceLongIndex->addObject(pTarget);
	}
	if (pTarget->IsCrossMargin == 0 && pTarget->Position != 0 && (pTarget->PosiDirection == PD_Short || pTarget->Position < 0)){
		if (pLiquidPriceShortIndex != NULL)
			pLiquidPriceShortIndex->addObject(pTarget);
	}
	if (pPositionIDHashIndex != NULL)
		pPositionIDHashIndex->addObject(pTarget,pTarget->HashPositionID);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CPosition* CPositionFactory::add(CWriteablePosition *pPosition, CTransaction *pTransaction)
{
	pPosition->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pPosition,true);
	}else{
		CPosition *pNewPosition;
		pNewPosition = internalAdd(pPosition,false);
		pTransaction->addResource(CPositionResource::alloc(CREATE_ACTION,this,pNewPosition));
		return pNewPosition;
	}
}

CPosition* CPositionFactory::internalUpdate(CPosition *pPosition, CWriteablePosition *pNewPosition,bool bNoTransaction)
{
	pNewPosition->UpdateTime = g_nCurrentTime;
	CWriteablePosition theOldPosition;
	if (bNoTransaction)
		forceCopy(&theOldPosition,pPosition,sizeof(CPosition));	
	CAVLNode *pLiquidPriceLongIndexNode=NULL;
	CAVLNode *pLiquidPriceShortIndexNode=NULL;
	CPosition *pTarget = pPosition;
	if (pTarget->IsCrossMargin == 0 && pTarget->Position != 0 && ! (pTarget->PosiDirection == PD_Short || pTarget->Position < 0))
		if (pLiquidPriceLongIndex != NULL)
			pLiquidPriceLongIndex->removeObject(pPosition);
	if (pTarget->IsCrossMargin == 0 && pTarget->Position != 0 && (pTarget->PosiDirection == PD_Short || pTarget->Position < 0))
		if (pLiquidPriceShortIndex != NULL)
			pLiquidPriceShortIndex->removeObject(pPosition);
	forceCopy(pPosition,pNewPosition,sizeof(CPosition));
	pMem->updateObject(pPosition);
	if (pTarget->IsCrossMargin == 0 && pTarget->Position != 0 && ! (pTarget->PosiDirection == PD_Short || pTarget->Position < 0))
		if (pLiquidPriceLongIndex != NULL)
			pLiquidPriceLongIndex->addObject(pTarget);
	if (pTarget->IsCrossMargin == 0 && pTarget->Position != 0 && (pTarget->PosiDirection == PD_Short || pTarget->Position < 0))
		if (pLiquidPriceShortIndex != NULL)
			pLiquidPriceShortIndex->addObject(pTarget);
	if (bNoTransaction)
		commitUpdate(pPosition,&theOldPosition);
	return pPosition;
}

CPosition* CPositionFactory::update(CPosition *pPosition, CWriteablePosition *pNewPosition, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pPosition == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pPosition,pNewPosition,bNoTransaction);
	}else{
		pTransaction->addResource(CPositionResource::alloc(UPDATE_ACTION,this,pPosition));
		return internalUpdate(pPosition,pNewPosition,false);
	}
}

void CPositionFactory::internalRemove(CPosition *pPosition, bool bNoTransaction)
{
	CWriteablePosition theOldPosition;
	//beforeRemove(pPosition);
	if (bNoTransaction)
		forceCopy(&theOldPosition,pPosition,sizeof(CPosition));
	CPosition *pTarget = pPosition;
	if (pAccountIndex != NULL)
		pAccountIndex->removeObject(pPosition);	
	if (pTarget->IsCrossMargin == 0 && pTarget->Position != 0 && ! (pTarget->PosiDirection == PD_Short || pTarget->Position < 0)){
		if (pLiquidPriceLongIndex != NULL)
			pLiquidPriceLongIndex->removeObject(pPosition);
	}
	if (pTarget->IsCrossMargin == 0 && pTarget->Position != 0 && (pTarget->PosiDirection == PD_Short || pTarget->Position < 0)){
		if (pLiquidPriceShortIndex != NULL)
			pLiquidPriceShortIndex->removeObject(pPosition);
	}
	
	if (pPositionIDHashIndex != NULL)
		pPositionIDHashIndex->removeObject(pPosition,pPosition->HashPositionID);		
	pMem->free(pPosition);
	if(bNoTransaction)
		commitRemove(&theOldPosition);
}

void CPositionFactory::remove(CPosition *pPosition, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pPosition == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pPosition, bNoTransaction);
	}else{
		pTransaction->addResource(CPositionResource::alloc(DELETE_ACTION,this,pPosition));
		internalRemove(pPosition,false);
	}
}

CPosition* CPositionFactory::addOrUpdate(CPosition *pPosition, CWriteablePosition *pNewPosition, CTransaction *pTransaction)
{
	if(pPosition == NULL)
		return add(pNewPosition,pTransaction);
	else
		return update(pPosition,pNewPosition,pTransaction);
}

void CPositionFactory::retrieve(CPosition *pPosition, CWriteablePosition *pTargetPosition)
{
	if(pPosition == NULL)
		return;
	forceCopy(pTargetPosition, pPosition, sizeof(CPosition));
}
	
int CPositionFactory::addActionTrigger(CPositionActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CPositionFactory::removeActionTrigger(CPositionActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CPositionFactory::addCommitTrigger(CPositionCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CPositionFactory::removeCommitTrigger(CPositionCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CPosition* CPositionFactory::getFirst()
{
	return (CPosition *)(pMem->getFirst());
}
	
CPosition* CPositionFactory::getNext()
{
	return (CPosition *)(pMem->getNext());
}

void CPositionFactory::commitAdd(CPosition *pPosition)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pPosition);
	}
}

void CPositionFactory::commitUpdate(CPosition *pPosition, CWriteablePosition *pOldPosition)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pPosition,pOldPosition);
	}
}
	
void CPositionFactory::commitRemove(CWriteablePosition *pPosition)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pPosition);
	}
}

void CPositionFactory::checkNull(FILE *pFile)
{
	CPosition *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->PositionID.isNull()){
			bNull = true;
			fprintf(pFile, "Position->PositionID:NULL\n");
		}
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "Position->MemberID:NULL\n");
		}
		if(pTarget->TradeUnitID.isNull()){
			bNull = true;
			fprintf(pFile, "Position->TradeUnitID:NULL\n");
		}
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "Position->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "Position->InstrumentID:NULL\n");
		}
		if(pTarget->PosiDirection.isNull()){
			bNull = true;
			fprintf(pFile, "Position->PosiDirection:NULL\n");
		}
		if(pTarget->PrePosition.isNull()){
			bNull = true;
			fprintf(pFile, "Position->PrePosition:NULL\n");
		}
		if(pTarget->Position.isNull()){
			bNull = true;
			fprintf(pFile, "Position->Position:NULL\n");
		}
		if(pTarget->LongFrozen.isNull()){
			bNull = true;
			fprintf(pFile, "Position->LongFrozen:NULL\n");
		}
		if(pTarget->ShortFrozen.isNull()){
			bNull = true;
			fprintf(pFile, "Position->ShortFrozen:NULL\n");
		}
		if(pTarget->PreLongFrozen.isNull()){
			bNull = true;
			fprintf(pFile, "Position->PreLongFrozen:NULL\n");
		}
		if(pTarget->PreShortFrozen.isNull()){
			bNull = true;
			fprintf(pFile, "Position->PreShortFrozen:NULL\n");
		}
		if(pTarget->HighestPosition.isNull()){
			bNull = true;
			fprintf(pFile, "Position->HighestPosition:NULL\n");
		}
		if(pTarget->PositionCost.isNull()){
			bNull = true;
			fprintf(pFile, "Position->PositionCost:NULL\n");
		}
		if(pTarget->UseMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Position->UseMargin:NULL\n");
		}
		if(pTarget->FrozenMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Position->FrozenMargin:NULL\n");
		}
		if(pTarget->LongFrozenMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Position->LongFrozenMargin:NULL\n");
		}
		if(pTarget->ShortFrozenMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Position->ShortFrozenMargin:NULL\n");
		}
		if(pTarget->CloseProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Position->CloseProfit:NULL\n");
		}
		if(pTarget->TotalPositionCost.isNull()){
			bNull = true;
			fprintf(pFile, "Position->TotalPositionCost:NULL\n");
		}
		if(pTarget->TotalCloseProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Position->TotalCloseProfit:NULL\n");
		}
		if(pTarget->TradeFee.isNull()){
			bNull = true;
			fprintf(pFile, "Position->TradeFee:NULL\n");
		}
		if(pTarget->PositionFee.isNull()){
			bNull = true;
			fprintf(pFile, "Position->PositionFee:NULL\n");
		}
		if(pTarget->AccountID.isNull()){
			bNull = true;
			fprintf(pFile, "Position->AccountID:NULL\n");
		}
		if(pTarget->SettlementGroup.isNull()){
			bNull = true;
			fprintf(pFile, "Position->SettlementGroup:NULL\n");
		}
		if(pTarget->IsCrossMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Position->IsCrossMargin:NULL\n");
		}
		if(pTarget->UnRealProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Position->UnRealProfit:NULL\n");
		}
		if(pTarget->CopyProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Position->CopyProfit:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CPositionFactory::checkValid(FILE *pFile)
{
	CPosition *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->PositionID.isValid()){
			bValid = false;
			fprintf(pFile, "Position->PositionID:inValid\n");
		}
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "Position->MemberID:inValid\n");
		}
		if(!pTarget->TradeUnitID.isValid()){
			bValid = false;
			fprintf(pFile, "Position->TradeUnitID:inValid\n");
		}
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "Position->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "Position->InstrumentID:inValid\n");
		}
		if(!pTarget->PositionClass.isValid()){
			bValid = false;
			fprintf(pFile, "Position->PositionClass:inValid\n");
		}
		if(!pTarget->PosiDirection.isValid()){
			bValid = false;
			fprintf(pFile, "Position->PosiDirection:inValid\n");
		}
		if(!pTarget->PrePosition.isValid()){
			bValid = false;
			fprintf(pFile, "Position->PrePosition:inValid\n");
		}
		if(!pTarget->Position.isValid()){
			bValid = false;
			fprintf(pFile, "Position->Position:inValid\n");
		}
		if(!pTarget->LongFrozen.isValid()){
			bValid = false;
			fprintf(pFile, "Position->LongFrozen:inValid\n");
		}
		if(!pTarget->ShortFrozen.isValid()){
			bValid = false;
			fprintf(pFile, "Position->ShortFrozen:inValid\n");
		}
		if(!pTarget->PreLongFrozen.isValid()){
			bValid = false;
			fprintf(pFile, "Position->PreLongFrozen:inValid\n");
		}
		if(!pTarget->PreShortFrozen.isValid()){
			bValid = false;
			fprintf(pFile, "Position->PreShortFrozen:inValid\n");
		}
		if(!pTarget->HighestPosition.isValid()){
			bValid = false;
			fprintf(pFile, "Position->HighestPosition:inValid\n");
		}
		if(!pTarget->ClosePosition.isValid()){
			bValid = false;
			fprintf(pFile, "Position->ClosePosition:inValid\n");
		}
		if(!pTarget->PositionCost.isValid()){
			bValid = false;
			fprintf(pFile, "Position->PositionCost:inValid\n");
		}
		if(!pTarget->CostPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Position->CostPrice:inValid\n");
		}
		if(!pTarget->UseMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Position->UseMargin:inValid\n");
		}
		if(!pTarget->FrozenMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Position->FrozenMargin:inValid\n");
		}
		if(!pTarget->LongFrozenMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Position->LongFrozenMargin:inValid\n");
		}
		if(!pTarget->ShortFrozenMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Position->ShortFrozenMargin:inValid\n");
		}
		if(!pTarget->CloseProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Position->CloseProfit:inValid\n");
		}
		if(!pTarget->TotalPositionCost.isValid()){
			bValid = false;
			fprintf(pFile, "Position->TotalPositionCost:inValid\n");
		}
		if(!pTarget->TotalCloseProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Position->TotalCloseProfit:inValid\n");
		}
		if(!pTarget->OpenPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Position->OpenPrice:inValid\n");
		}
		if(!pTarget->ClosePrice.isValid()){
			bValid = false;
			fprintf(pFile, "Position->ClosePrice:inValid\n");
		}
		if(!pTarget->TradeFee.isValid()){
			bValid = false;
			fprintf(pFile, "Position->TradeFee:inValid\n");
		}
		if(!pTarget->PositionFee.isValid()){
			bValid = false;
			fprintf(pFile, "Position->PositionFee:inValid\n");
		}
		if(!pTarget->Leverage.isValid()){
			bValid = false;
			fprintf(pFile, "Position->Leverage:inValid\n");
		}
		if(!pTarget->AccountID.isValid()){
			bValid = false;
			fprintf(pFile, "Position->AccountID:inValid\n");
		}
		if(!pTarget->PriceCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Position->PriceCurrency:inValid\n");
		}
		if(!pTarget->ClearCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Position->ClearCurrency:inValid\n");
		}
		if(!pTarget->SettlementGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Position->SettlementGroup:inValid\n");
		}
		if(!pTarget->IsCrossMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Position->IsCrossMargin:inValid\n");
		}
		if(!pTarget->CloseOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "Position->CloseOrderID:inValid\n");
		}
		if(!pTarget->SLTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Position->SLTriggerPrice:inValid\n");
		}
		if(!pTarget->TPTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Position->TPTriggerPrice:inValid\n");
		}
		if(!pTarget->BeginTime.isValid()){
			bValid = false;
			fprintf(pFile, "Position->BeginTime:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "Position->InsertTime:inValid\n");
		}
		if(!pTarget->LastOpenTime.isValid()){
			bValid = false;
			fprintf(pFile, "Position->LastOpenTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Position->UpdateTime:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "Position->BusinessNo:inValid\n");
		}
		if(!pTarget->IsAutoAddMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Position->IsAutoAddMargin:inValid\n");
		}
		if(!pTarget->Frequency.isValid()){
			bValid = false;
			fprintf(pFile, "Position->Frequency:inValid\n");
		}
		if(!pTarget->MaintMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Position->MaintMargin:inValid\n");
		}
		if(!pTarget->UnRealProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Position->UnRealProfit:inValid\n");
		}
		if(!pTarget->LiquidPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Position->LiquidPrice:inValid\n");
		}
		if(!pTarget->CreateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Position->CreateTime:inValid\n");
		}
		if(!pTarget->CopyMemberID.isValid()){
			bValid = false;
			fprintf(pFile, "Position->CopyMemberID:inValid\n");
		}
		if(!pTarget->CopyProfitRate.isValid()){
			bValid = false;
			fprintf(pFile, "Position->CopyProfitRate:inValid\n");
		}
		if(!pTarget->CopyProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Position->CopyProfit:inValid\n");
		}
		if(!pTarget->FirstTradeID.isValid()){
			bValid = false;
			fprintf(pFile, "Position->FirstTradeID:inValid\n");
		}
		if(!pTarget->LastTradeID.isValid()){
			bValid = false;
			fprintf(pFile, "Position->LastTradeID:inValid\n");
		}
		if(!pTarget->BusinessType.isValid()){
			bValid = false;
			fprintf(pFile, "Position->BusinessType:inValid\n");
		}
		if(!pTarget->BusinessValue.isValid()){
			bValid = false;
			fprintf(pFile, "Position->BusinessValue:inValid\n");
		}
		if(!pTarget->Reserve.isValid()){
			bValid = false;
			fprintf(pFile, "Position->Reserve:inValid\n");
		}
		if(!pTarget->ReserveProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Position->ReserveProfit:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "Position->Remark:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CPositionFactory::linkAllInstrument()
{
	CPosition *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkInstrument();
		pTarget=getNext();
	}
}

void CPositionFactory::linkAllAccount()
{
	CPosition *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkAccount();
		pTarget=getNext();
	}
}

void CPositionFactory::checkLinkInstrument(FILE *pFile)
{
	CPosition *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getInstrument() == NULL){
			if (pTarget->ExchangeID.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->InstrumentID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Position->Instrument:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CPositionFactory::checkLinkAccount(FILE *pFile)
{
	CPosition *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getAccount() == NULL){
			if(false){
				pTarget = getNext();
				continue;
			}
			if (pTarget->MemberID.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->AccountID.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->SettlementGroup.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->ClearCurrency.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Position->Account:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CPositionFactory::clearLinkInstrument()
{
}

void CPositionFactory::clearLinkAccount()
{
}

extern int compareForProductGroupIndexinMarketData(const void *pV1, const void *pV2);
void CMarketDataFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pProductGroupIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 'p':
	{
		if(!reuse) {
			pProductGroupIndex=new CAVLTree(maxUnit,compareForProductGroupIndexinMarketData,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("MarketData_ProductGroupIndex",pProductGroupIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("MarketData_ProductGroupIndex");
			if(it != pIndexMap->end()) {
				pProductGroupIndex=new CAVLTree(maxUnit,compareForProductGroupIndexinMarketData,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pProductGroupIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pInstrumentIDHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 'r':
	case 'p':
	case 't':
	{
		int hashKeySize=256;
		if(!reuse) {
			pInstrumentIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("MarketData_InstrumentIDHashIndex",pInstrumentIDHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("MarketData_InstrumentIDHashIndex");
			if(it != pIndexMap->end()) {
				pInstrumentIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pInstrumentIDHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pLastFoundInSearchByProductGroup=NULL;
	pActionTriggers=new vector<CMarketDataActionTrigger *>;
	pCommitTriggers=new vector<CMarketDataCommitTrigger *>;
}

CMarketDataFactory::CMarketDataFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CMarketData),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CMarketDataFactory::CMarketDataFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CMarketData),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CMarketDataFactory::~CMarketDataFactory()
{
	if (pProductGroupIndex!=NULL)
		delete pProductGroupIndex;
	if (pInstrumentIDHashIndex!=NULL)
		delete pInstrumentIDHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CMarketDataFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableMarketData thisMarketData;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisMarketData.readCSV(input,pNames))
		add(&thisMarketData);
	fclose(input);
	delete pNames;
	return 1;
}

int CMarketDataFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "MarketData.csv");
	return readCSV(szFileName);
}

int CMarketDataFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableMarketData *pMarketData;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableMarketData::writeCSVHead(output);
	pMarketData=(CWriteableMarketData *)(pMem->getFirst());
	while (pMarketData!=NULL) {
		pMarketData->writeCSV(output);
		pMarketData=(CWriteableMarketData *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CMarketDataFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableMarketData *pMarketData;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableMarketData::writeCSVHeadPreVersion(output);
	pMarketData=(CWriteableMarketData *)(pMem->getFirst());
	while (pMarketData!=NULL) {
		pMarketData->writeCSVPreVersion(output);
		pMarketData=(CWriteableMarketData *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CMarketDataFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "MarketData.csv");
	return writeCSV(szFileName);
}

void CMarketDataFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CMarketDataFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableMarketData *pMarketData;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CMarketDataFactory={       Total Count=%d\n", pMem->getCount());
	pMarketData=(CWriteableMarketData *)(pMem->getFirst());
	while (pMarketData!=NULL) {
		pMarketData->dump(fp,index++);
		pMarketData=(CWriteableMarketData *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CMarketDataFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pProductGroupIndex != NULL)
		pProductGroupIndex->removeAll();
	if(pInstrumentIDHashIndex != NULL)
		pInstrumentIDHashIndex->removeAll();
}

CMarketData *CMarketDataFactory::internalAdd(CWriteableMarketData *pMarketData, bool bNoTransaction)
{
	pMarketData->m_pMDB = m_pMDB;
	if(pMarketData->InsertTime.isNull())
		pMarketData->InsertTime = g_nCurrentTime;
	CMarketData *pTarget;
	//beforeAdd(pMarketData);
	pTarget=(CMarketData *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough MarketData in memory database");
		return NULL;
	}
	forceCopy(pTarget, pMarketData, sizeof(CMarketData));
	pMem->updateObject(pTarget);
	if (pProductGroupIndex != NULL)
		pProductGroupIndex->addObject(pTarget);
	if (pInstrumentIDHashIndex != NULL)
		pInstrumentIDHashIndex->addObject(pTarget,pTarget->HashInstrumentIDIndex);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CMarketData* CMarketDataFactory::add(CWriteableMarketData *pMarketData, CTransaction *pTransaction)
{
	pMarketData->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pMarketData,true);
	}else{
		CMarketData *pNewMarketData;
		pNewMarketData = internalAdd(pMarketData,false);
		pTransaction->addResource(CMarketDataResource::alloc(CREATE_ACTION,this,pNewMarketData));
		return pNewMarketData;
	}
}

CMarketData* CMarketDataFactory::internalUpdate(CMarketData *pMarketData, CWriteableMarketData *pNewMarketData,bool bNoTransaction)
{
	pNewMarketData->UpdateTime = g_nCurrentTime;
	CWriteableMarketData theOldMarketData;
	if (bNoTransaction)
		forceCopy(&theOldMarketData,pMarketData,sizeof(CMarketData));	
	CMarketData *pTarget = pMarketData;
	forceCopy(pMarketData,pNewMarketData,sizeof(CMarketData));
	pMem->updateObject(pMarketData);
	if (bNoTransaction)
		commitUpdate(pMarketData,&theOldMarketData);
	return pMarketData;
}

CMarketData* CMarketDataFactory::update(CMarketData *pMarketData, CWriteableMarketData *pNewMarketData, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pMarketData == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pMarketData,pNewMarketData,bNoTransaction);
	}else{
		pTransaction->addResource(CMarketDataResource::alloc(UPDATE_ACTION,this,pMarketData));
		return internalUpdate(pMarketData,pNewMarketData,false);
	}
}

void CMarketDataFactory::internalRemove(CMarketData *pMarketData, bool bNoTransaction)
{
	CWriteableMarketData theOldMarketData;
	//beforeRemove(pMarketData);
	if (bNoTransaction)
		forceCopy(&theOldMarketData,pMarketData,sizeof(CMarketData));
	CMarketData *pTarget = pMarketData;
	if (pProductGroupIndex != NULL)
		pProductGroupIndex->removeObject(pMarketData);	
	
	if (pInstrumentIDHashIndex != NULL)
		pInstrumentIDHashIndex->removeObject(pMarketData,pMarketData->HashInstrumentIDIndex);		
	pMem->free(pMarketData);
	if(bNoTransaction)
		commitRemove(&theOldMarketData);
}

void CMarketDataFactory::remove(CMarketData *pMarketData, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pMarketData == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pMarketData, bNoTransaction);
	}else{
		pTransaction->addResource(CMarketDataResource::alloc(DELETE_ACTION,this,pMarketData));
		internalRemove(pMarketData,false);
	}
}

CMarketData* CMarketDataFactory::addOrUpdate(CMarketData *pMarketData, CWriteableMarketData *pNewMarketData, CTransaction *pTransaction)
{
	if(pMarketData == NULL)
		return add(pNewMarketData,pTransaction);
	else
		return update(pMarketData,pNewMarketData,pTransaction);
}

void CMarketDataFactory::retrieve(CMarketData *pMarketData, CWriteableMarketData *pTargetMarketData)
{
	if(pMarketData == NULL)
		return;
	forceCopy(pTargetMarketData, pMarketData, sizeof(CMarketData));
}
	
int CMarketDataFactory::addActionTrigger(CMarketDataActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CMarketDataFactory::removeActionTrigger(CMarketDataActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CMarketDataFactory::addCommitTrigger(CMarketDataCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CMarketDataFactory::removeCommitTrigger(CMarketDataCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CMarketData* CMarketDataFactory::getFirst()
{
	return (CMarketData *)(pMem->getFirst());
}
	
CMarketData* CMarketDataFactory::getNext()
{
	return (CMarketData *)(pMem->getNext());
}

void CMarketDataFactory::commitAdd(CMarketData *pMarketData)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pMarketData);
	}
}

void CMarketDataFactory::commitUpdate(CMarketData *pMarketData, CWriteableMarketData *pOldMarketData)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pMarketData,pOldMarketData);
	}
}
	
void CMarketDataFactory::commitRemove(CWriteableMarketData *pMarketData)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pMarketData);
	}
}

void CMarketDataFactory::checkNull(FILE *pFile)
{
	CMarketData *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "MarketData->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "MarketData->InstrumentID:NULL\n");
		}
		if(pTarget->UpdateTime.isNull()){
			bNull = true;
			fprintf(pFile, "MarketData->UpdateTime:NULL\n");
		}
		if(pTarget->PreOpenInterest.isNull()){
			bNull = true;
			fprintf(pFile, "MarketData->PreOpenInterest:NULL\n");
		}
		if(pTarget->Volume.isNull()){
			bNull = true;
			fprintf(pFile, "MarketData->Volume:NULL\n");
		}
		if(pTarget->Turnover.isNull()){
			bNull = true;
			fprintf(pFile, "MarketData->Turnover:NULL\n");
		}
		if(pTarget->OpenInterest.isNull()){
			bNull = true;
			fprintf(pFile, "MarketData->OpenInterest:NULL\n");
		}
		if(pTarget->IsPubMarketData.isNull()){
			bNull = true;
			fprintf(pFile, "MarketData->IsPubMarketData:NULL\n");
		}
		if(pTarget->InstrumentStatus.isNull()){
			bNull = true;
			fprintf(pFile, "MarketData->InstrumentStatus:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CMarketDataFactory::checkValid(FILE *pFile)
{
	CMarketData *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->InstrumentID:inValid\n");
		}
		if(!pTarget->ProductGroup.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->ProductGroup:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->UpdateTime:inValid\n");
		}
		if(!pTarget->PreClosePrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->PreClosePrice:inValid\n");
		}
		if(!pTarget->PreSettlementPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->PreSettlementPrice:inValid\n");
		}
		if(!pTarget->PreOpenInterest.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->PreOpenInterest:inValid\n");
		}
		if(!pTarget->PrePositionFeeRate.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->PrePositionFeeRate:inValid\n");
		}
		if(!pTarget->ClosePrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->ClosePrice:inValid\n");
		}
		if(!pTarget->PositionFeeTime.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->PositionFeeTime:inValid\n");
		}
		if(!pTarget->SettlementTime.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->SettlementTime:inValid\n");
		}
		if(!pTarget->UpperLimitPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->UpperLimitPrice:inValid\n");
		}
		if(!pTarget->LowerLimitPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->LowerLimitPrice:inValid\n");
		}
		if(!pTarget->UnderlyingPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->UnderlyingPrice:inValid\n");
		}
		if(!pTarget->MarkedPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->MarkedPrice:inValid\n");
		}
		if(!pTarget->PositionFeeRate.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->PositionFeeRate:inValid\n");
		}
		if(!pTarget->HighestPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->HighestPrice:inValid\n");
		}
		if(!pTarget->LowestPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->LowestPrice:inValid\n");
		}
		if(!pTarget->LastPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->LastPrice:inValid\n");
		}
		if(!pTarget->Volume.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->Volume:inValid\n");
		}
		if(!pTarget->Turnover.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->Turnover:inValid\n");
		}
		if(!pTarget->OpenInterest.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->OpenInterest:inValid\n");
		}
		if(!pTarget->SettlementPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->SettlementPrice:inValid\n");
		}
		if(!pTarget->OpenPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->OpenPrice:inValid\n");
		}
		if(!pTarget->IsPubMarketData.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->IsPubMarketData:inValid\n");
		}
		if(!pTarget->InstrumentStatus.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->InstrumentStatus:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->Remark:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->BusinessNo:inValid\n");
		}
		if(!pTarget->MarkedPriceTotal.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->MarkedPriceTotal:inValid\n");
		}
		if(!pTarget->MarkedPriceWeight.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->MarkedPriceWeight:inValid\n");
		}
		if(!pTarget->L25UpperPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->L25UpperPrice:inValid\n");
		}
		if(!pTarget->L25LowerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->L25LowerPrice:inValid\n");
		}
		if(!pTarget->L25UpperPrice1.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->L25UpperPrice1:inValid\n");
		}
		if(!pTarget->L25LowerPrice1.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->L25LowerPrice1:inValid\n");
		}
		if(!pTarget->L25UpperPrice2.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->L25UpperPrice2:inValid\n");
		}
		if(!pTarget->L25LowerPrice2.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->L25LowerPrice2:inValid\n");
		}
		if(!pTarget->BidPrice1.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->BidPrice1:inValid\n");
		}
		if(!pTarget->BidVolume1.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->BidVolume1:inValid\n");
		}
		if(!pTarget->AskPrice1.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->AskPrice1:inValid\n");
		}
		if(!pTarget->AskVolume1.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->AskVolume1:inValid\n");
		}
		if(!pTarget->TheoryPrice.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->TheoryPrice:inValid\n");
		}
		if(!pTarget->HighestPrice24.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->HighestPrice24:inValid\n");
		}
		if(!pTarget->LowestPrice24.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->LowestPrice24:inValid\n");
		}
		if(!pTarget->Volume24.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->Volume24:inValid\n");
		}
		if(!pTarget->Turnover24.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->Turnover24:inValid\n");
		}
		if(!pTarget->OpenPrice24.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->OpenPrice24:inValid\n");
		}
		if(!pTarget->Frequency.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->Frequency:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "MarketData->InsertTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CMarketDataFactory::linkAllInstrument()
{
	CMarketData *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkInstrument();
		pTarget=getNext();
	}
}

void CMarketDataFactory::checkLinkInstrument(FILE *pFile)
{
	CMarketData *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getInstrument() == NULL){
			if (pTarget->ExchangeID.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->InstrumentID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "MarketData->Instrument:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CMarketDataFactory::clearLinkInstrument()
{
}

extern int compareForPositionIDIndexinOrder(const void *pV1, const void *pV2);
extern int compareForLocalIDIndexinOrder(const void *pV1, const void *pV2);
extern int compareForInstrumentIDIndexinOrder(const void *pV1, const void *pV2);
void COrderFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pPositionIDIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 's':
	{
		if(!reuse) {
			pPositionIDIndex=new CAVLTree(maxUnit,compareForPositionIDIndexinOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Order_PositionIDIndex",pPositionIDIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Order_PositionIDIndex");
			if(it != pIndexMap->end()) {
				pPositionIDIndex=new CAVLTree(maxUnit,compareForPositionIDIndexinOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pPositionIDIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pLocalIDIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 'p':
	{
		if(!reuse) {
			pLocalIDIndex=new CAVLTree(maxUnit,compareForLocalIDIndexinOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Order_LocalIDIndex",pLocalIDIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Order_LocalIDIndex");
			if(it != pIndexMap->end()) {
				pLocalIDIndex=new CAVLTree(maxUnit,compareForLocalIDIndexinOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pLocalIDIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pInstrumentIDIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 'p':
	{
		if(!reuse) {
			pInstrumentIDIndex=new CAVLTree(maxUnit,compareForInstrumentIDIndexinOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Order_InstrumentIDIndex",pInstrumentIDIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("Order_InstrumentIDIndex");
			if(it != pIndexMap->end()) {
				pInstrumentIDIndex=new CAVLTree(maxUnit,compareForInstrumentIDIndexinOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pInstrumentIDIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pOrderIDHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 'p':
	{
		int hashKeySize=1572869;
		if(!reuse) {
			pOrderIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("Order_OrderIDHashIndex",pOrderIDHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("Order_OrderIDHashIndex");
			if(it != pIndexMap->end()) {
				pOrderIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pOrderIDHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pLastFoundInSearchByAccountID=NULL;
	pLastFoundInSearchByPositionID=NULL;
	pLastFoundInSearchByAccountIDPrivate=NULL;
	pLastFoundInSearchByInstrumentID=NULL;
	pActionTriggers=new vector<COrderActionTrigger *>;
	pCommitTriggers=new vector<COrderCommitTrigger *>;
}

COrderFactory::COrderFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(COrder),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

COrderFactory::COrderFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(COrder),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

COrderFactory::~COrderFactory()
{
	if (pPositionIDIndex!=NULL)
		delete pPositionIDIndex;
	if (pLocalIDIndex!=NULL)
		delete pLocalIDIndex;
	if (pInstrumentIDIndex!=NULL)
		delete pInstrumentIDIndex;
	if (pOrderIDHashIndex!=NULL)
		delete pOrderIDHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int COrderFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableOrder thisOrder;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisOrder.readCSV(input,pNames))
		add(&thisOrder);
	fclose(input);
	delete pNames;
	return 1;
}

int COrderFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Order.csv");
	return readCSV(szFileName);
}

int COrderFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableOrder *pOrder;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableOrder::writeCSVHead(output);
	pOrder=(CWriteableOrder *)(pMem->getFirst());
	while (pOrder!=NULL) {
		pOrder->writeCSV(output);
		pOrder=(CWriteableOrder *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int COrderFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableOrder *pOrder;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableOrder::writeCSVHeadPreVersion(output);
	pOrder=(CWriteableOrder *)(pMem->getFirst());
	while (pOrder!=NULL) {
		pOrder->writeCSVPreVersion(output);
		pOrder=(CWriteableOrder *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int COrderFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Order.csv");
	return writeCSV(szFileName);
}

void COrderFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void COrderFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableOrder *pOrder;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"COrderFactory={       Total Count=%d\n", pMem->getCount());
	pOrder=(CWriteableOrder *)(pMem->getFirst());
	while (pOrder!=NULL) {
		pOrder->dump(fp,index++);
		pOrder=(CWriteableOrder *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void COrderFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pPositionIDIndex != NULL)
		pPositionIDIndex->removeAll();
	if(pLocalIDIndex != NULL)
		pLocalIDIndex->removeAll();
	if(pInstrumentIDIndex != NULL)
		pInstrumentIDIndex->removeAll();
	if(pOrderIDHashIndex != NULL)
		pOrderIDHashIndex->removeAll();
}

COrder *COrderFactory::internalAdd(CWriteableOrder *pOrder, bool bNoTransaction)
{
	pOrder->m_pMDB = m_pMDB;
	if(pOrder->InsertTime.isNull())
		pOrder->InsertTime = g_nCurrentTime;
	if(pOrder->Tradable.isNull())
		pOrder->Tradable = 1;
	COrder *pTarget;
	//beforeAdd(pOrder);
	pTarget=(COrder *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough Order in memory database");
		return NULL;
	}
	forceCopy(pTarget, pOrder, sizeof(COrder));
	pMem->updateObject(pTarget);
	if (pPositionIDIndex != NULL)
		pPositionIDIndex->addObject(pTarget);
	if (pLocalIDIndex != NULL)
		pLocalIDIndex->addObject(pTarget);
	if (pInstrumentIDIndex != NULL)
		pInstrumentIDIndex->addObject(pTarget);
	if (pOrderIDHashIndex != NULL)
		pOrderIDHashIndex->addObject(pTarget,pTarget->HashOrderID);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

COrder* COrderFactory::add(CWriteableOrder *pOrder, CTransaction *pTransaction)
{
	pOrder->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pOrder,true);
	}else{
		COrder *pNewOrder;
		pNewOrder = internalAdd(pOrder,false);
		pTransaction->addResource(COrderResource::alloc(CREATE_ACTION,this,pNewOrder));
		return pNewOrder;
	}
}

COrder* COrderFactory::internalUpdate(COrder *pOrder, CWriteableOrder *pNewOrder,bool bNoTransaction)
{
	pNewOrder->UpdateTime = g_nCurrentTime;
	CWriteableOrder theOldOrder;
	if (bNoTransaction)
		forceCopy(&theOldOrder,pOrder,sizeof(COrder));	
	COrder *pTarget = pOrder;
	forceCopy(pOrder,pNewOrder,sizeof(COrder));
	pMem->updateObject(pOrder);
	if (bNoTransaction)
		commitUpdate(pOrder,&theOldOrder);
	return pOrder;
}

COrder* COrderFactory::update(COrder *pOrder, CWriteableOrder *pNewOrder, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pOrder == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pOrder,pNewOrder,bNoTransaction);
	}else{
		pTransaction->addResource(COrderResource::alloc(UPDATE_ACTION,this,pOrder));
		return internalUpdate(pOrder,pNewOrder,false);
	}
}

void COrderFactory::internalRemove(COrder *pOrder, bool bNoTransaction)
{
	CWriteableOrder theOldOrder;
	//beforeRemove(pOrder);
	if (bNoTransaction)
		forceCopy(&theOldOrder,pOrder,sizeof(COrder));
	COrder *pTarget = pOrder;
	if (pPositionIDIndex != NULL)
		pPositionIDIndex->removeObject(pOrder);	
	if (pLocalIDIndex != NULL)
		pLocalIDIndex->removeObject(pOrder);	
	if (pInstrumentIDIndex != NULL)
		pInstrumentIDIndex->removeObject(pOrder);	
	
	if (pOrderIDHashIndex != NULL)
		pOrderIDHashIndex->removeObject(pOrder,pOrder->HashOrderID);		
	pMem->free(pOrder);
	if(bNoTransaction)
		commitRemove(&theOldOrder);
}

void COrderFactory::remove(COrder *pOrder, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pOrder == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pOrder, bNoTransaction);
	}else{
		pTransaction->addResource(COrderResource::alloc(DELETE_ACTION,this,pOrder));
		internalRemove(pOrder,false);
	}
}

COrder* COrderFactory::addOrUpdate(COrder *pOrder, CWriteableOrder *pNewOrder, CTransaction *pTransaction)
{
	if(pOrder == NULL)
		return add(pNewOrder,pTransaction);
	else
		return update(pOrder,pNewOrder,pTransaction);
}

void COrderFactory::retrieve(COrder *pOrder, CWriteableOrder *pTargetOrder)
{
	if(pOrder == NULL)
		return;
	forceCopy(pTargetOrder, pOrder, sizeof(COrder));
}
	
int COrderFactory::addActionTrigger(COrderActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int COrderFactory::removeActionTrigger(COrderActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int COrderFactory::addCommitTrigger(COrderCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int COrderFactory::removeCommitTrigger(COrderCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

COrder* COrderFactory::getFirst()
{
	return (COrder *)(pMem->getFirst());
}
	
COrder* COrderFactory::getNext()
{
	return (COrder *)(pMem->getNext());
}

void COrderFactory::commitAdd(COrder *pOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pOrder);
	}
}

void COrderFactory::commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pOrder,pOldOrder);
	}
}
	
void COrderFactory::commitRemove(CWriteableOrder *pOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pOrder);
	}
}

void COrderFactory::checkNull(FILE *pFile)
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "Order->MemberID:NULL\n");
		}
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "Order->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "Order->InstrumentID:NULL\n");
		}
		if(pTarget->OrderPriceType.isNull()){
			bNull = true;
			fprintf(pFile, "Order->OrderPriceType:NULL\n");
		}
		if(pTarget->Direction.isNull()){
			bNull = true;
			fprintf(pFile, "Order->Direction:NULL\n");
		}
		if(pTarget->OffsetFlag.isNull()){
			bNull = true;
			fprintf(pFile, "Order->OffsetFlag:NULL\n");
		}
		if(pTarget->OrderType.isNull()){
			bNull = true;
			fprintf(pFile, "Order->OrderType:NULL\n");
		}
		if(pTarget->BusinessType.isNull()){
			bNull = true;
			fprintf(pFile, "Order->BusinessType:NULL\n");
		}
		if(pTarget->OrderID.isNull()){
			bNull = true;
			fprintf(pFile, "Order->OrderID:NULL\n");
		}
		if(pTarget->PositionID.isNull()){
			bNull = true;
			fprintf(pFile, "Order->PositionID:NULL\n");
		}
		if(pTarget->OrderStatus.isNull()){
			bNull = true;
			fprintf(pFile, "Order->OrderStatus:NULL\n");
		}
		if(pTarget->DeriveSource.isNull()){
			bNull = true;
			fprintf(pFile, "Order->DeriveSource:NULL\n");
		}
		if(pTarget->VolumeTraded.isNull()){
			bNull = true;
			fprintf(pFile, "Order->VolumeTraded:NULL\n");
		}
		if(pTarget->VolumeRemain.isNull()){
			bNull = true;
			fprintf(pFile, "Order->VolumeRemain:NULL\n");
		}
		if(pTarget->VolumeCancled.isNull()){
			bNull = true;
			fprintf(pFile, "Order->VolumeCancled:NULL\n");
		}
		if(pTarget->Priority.isNull()){
			bNull = true;
			fprintf(pFile, "Order->Priority:NULL\n");
		}
		if(pTarget->TimeSortNo.isNull()){
			bNull = true;
			fprintf(pFile, "Order->TimeSortNo:NULL\n");
		}
		if(pTarget->FrozenMoney.isNull()){
			bNull = true;
			fprintf(pFile, "Order->FrozenMoney:NULL\n");
		}
		if(pTarget->FrozenFee.isNull()){
			bNull = true;
			fprintf(pFile, "Order->FrozenFee:NULL\n");
		}
		if(pTarget->FrozenMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Order->FrozenMargin:NULL\n");
		}
		if(pTarget->Fee.isNull()){
			bNull = true;
			fprintf(pFile, "Order->Fee:NULL\n");
		}
		if(pTarget->CloseProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Order->CloseProfit:NULL\n");
		}
		if(pTarget->Turnover.isNull()){
			bNull = true;
			fprintf(pFile, "Order->Turnover:NULL\n");
		}
		if(pTarget->Tradable.isNull()){
			bNull = true;
			fprintf(pFile, "Order->Tradable:NULL\n");
		}
		if(pTarget->SettlementGroup.isNull()){
			bNull = true;
			fprintf(pFile, "Order->SettlementGroup:NULL\n");
		}
		if(pTarget->PosiDirection.isNull()){
			bNull = true;
			fprintf(pFile, "Order->PosiDirection:NULL\n");
		}
		if(pTarget->CopyProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Order->CopyProfit:NULL\n");
		}
		if(pTarget->UserID.isNull()){
			bNull = true;
			fprintf(pFile, "Order->UserID:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void COrderFactory::checkValid(FILE *pFile)
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->MemberID:inValid\n");
		}
		if(!pTarget->TradeUnitID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->TradeUnitID:inValid\n");
		}
		if(!pTarget->AccountID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->AccountID:inValid\n");
		}
		if(!pTarget->LocalID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->LocalID:inValid\n");
		}
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->InstrumentID:inValid\n");
		}
		if(!pTarget->OrderPriceType.isValid()){
			bValid = false;
			fprintf(pFile, "Order->OrderPriceType:inValid\n");
		}
		if(!pTarget->Direction.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Direction:inValid\n");
		}
		if(!pTarget->OffsetFlag.isValid()){
			bValid = false;
			fprintf(pFile, "Order->OffsetFlag:inValid\n");
		}
		if(!pTarget->Price.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Price:inValid\n");
		}
		if(!pTarget->Volume.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Volume:inValid\n");
		}
		if(!pTarget->VolumeDisplay.isValid()){
			bValid = false;
			fprintf(pFile, "Order->VolumeDisplay:inValid\n");
		}
		if(!pTarget->VolumeMode.isValid()){
			bValid = false;
			fprintf(pFile, "Order->VolumeMode:inValid\n");
		}
		if(!pTarget->Cost.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Cost:inValid\n");
		}
		if(!pTarget->OrderType.isValid()){
			bValid = false;
			fprintf(pFile, "Order->OrderType:inValid\n");
		}
		if(!pTarget->GTDTime.isValid()){
			bValid = false;
			fprintf(pFile, "Order->GTDTime:inValid\n");
		}
		if(!pTarget->MinVolume.isValid()){
			bValid = false;
			fprintf(pFile, "Order->MinVolume:inValid\n");
		}
		if(!pTarget->BusinessType.isValid()){
			bValid = false;
			fprintf(pFile, "Order->BusinessType:inValid\n");
		}
		if(!pTarget->BusinessValue.isValid()){
			bValid = false;
			fprintf(pFile, "Order->BusinessValue:inValid\n");
		}
		if(!pTarget->CloseOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->CloseOrderID:inValid\n");
		}
		if(!pTarget->IsCrossMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Order->IsCrossMargin:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Remark:inValid\n");
		}
		if(!pTarget->OrderID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->OrderID:inValid\n");
		}
		if(!pTarget->CopyMemberID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->CopyMemberID:inValid\n");
		}
		if(!pTarget->CopyOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->CopyOrderID:inValid\n");
		}
		if(!pTarget->Leverage.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Leverage:inValid\n");
		}
		if(!pTarget->CopyProfitRate.isValid()){
			bValid = false;
			fprintf(pFile, "Order->CopyProfitRate:inValid\n");
		}
		if(!pTarget->APPID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->APPID:inValid\n");
		}
		if(!pTarget->PositionID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->PositionID:inValid\n");
		}
		if(!pTarget->TriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Order->TriggerPrice:inValid\n");
		}
		if(!pTarget->Reserve.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Reserve:inValid\n");
		}
		if(!pTarget->OrderStatus.isValid()){
			bValid = false;
			fprintf(pFile, "Order->OrderStatus:inValid\n");
		}
		if(!pTarget->DeriveSource.isValid()){
			bValid = false;
			fprintf(pFile, "Order->DeriveSource:inValid\n");
		}
		if(!pTarget->DeriveDetail.isValid()){
			bValid = false;
			fprintf(pFile, "Order->DeriveDetail:inValid\n");
		}
		if(!pTarget->VolumeTraded.isValid()){
			bValid = false;
			fprintf(pFile, "Order->VolumeTraded:inValid\n");
		}
		if(!pTarget->VolumeRemain.isValid()){
			bValid = false;
			fprintf(pFile, "Order->VolumeRemain:inValid\n");
		}
		if(!pTarget->VolumeCancled.isValid()){
			bValid = false;
			fprintf(pFile, "Order->VolumeCancled:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "Order->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Order->UpdateTime:inValid\n");
		}
		if(!pTarget->Priority.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Priority:inValid\n");
		}
		if(!pTarget->TimeSortNo.isValid()){
			bValid = false;
			fprintf(pFile, "Order->TimeSortNo:inValid\n");
		}
		if(!pTarget->FrontNo.isValid()){
			bValid = false;
			fprintf(pFile, "Order->FrontNo:inValid\n");
		}
		if(!pTarget->PriceCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Order->PriceCurrency:inValid\n");
		}
		if(!pTarget->FeeCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Order->FeeCurrency:inValid\n");
		}
		if(!pTarget->ClearCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Order->ClearCurrency:inValid\n");
		}
		if(!pTarget->FrozenMoney.isValid()){
			bValid = false;
			fprintf(pFile, "Order->FrozenMoney:inValid\n");
		}
		if(!pTarget->FrozenFee.isValid()){
			bValid = false;
			fprintf(pFile, "Order->FrozenFee:inValid\n");
		}
		if(!pTarget->FrozenMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Order->FrozenMargin:inValid\n");
		}
		if(!pTarget->Fee.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Fee:inValid\n");
		}
		if(!pTarget->CloseProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Order->CloseProfit:inValid\n");
		}
		if(!pTarget->Turnover.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Turnover:inValid\n");
		}
		if(!pTarget->RelatedOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->RelatedOrderID:inValid\n");
		}
		if(!pTarget->BusinessResult.isValid()){
			bValid = false;
			fprintf(pFile, "Order->BusinessResult:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "Order->BusinessNo:inValid\n");
		}
		if(!pTarget->Tradable.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Tradable:inValid\n");
		}
		if(!pTarget->SettlementGroup.isValid()){
			bValid = false;
			fprintf(pFile, "Order->SettlementGroup:inValid\n");
		}
		if(!pTarget->PosiDirection.isValid()){
			bValid = false;
			fprintf(pFile, "Order->PosiDirection:inValid\n");
		}
		if(!pTarget->TradePrice.isValid()){
			bValid = false;
			fprintf(pFile, "Order->TradePrice:inValid\n");
		}
		if(!pTarget->OpenPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Order->OpenPrice:inValid\n");
		}
		if(!pTarget->TriggerOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->TriggerOrderID:inValid\n");
		}
		if(!pTarget->SLTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Order->SLTriggerPrice:inValid\n");
		}
		if(!pTarget->TPTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Order->TPTriggerPrice:inValid\n");
		}
		if(!pTarget->CopyProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Order->CopyProfit:inValid\n");
		}
		if(!pTarget->Position.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Position:inValid\n");
		}
		if(!pTarget->UserID.isValid()){
			bValid = false;
			fprintf(pFile, "Order->UserID:inValid\n");
		}
		if(!pTarget->LastPriceByInsert.isValid()){
			bValid = false;
			fprintf(pFile, "Order->LastPriceByInsert:inValid\n");
		}
		if(!pTarget->BidPrice1ByInsert.isValid()){
			bValid = false;
			fprintf(pFile, "Order->BidPrice1ByInsert:inValid\n");
		}
		if(!pTarget->AskPrice1ByInsert.isValid()){
			bValid = false;
			fprintf(pFile, "Order->AskPrice1ByInsert:inValid\n");
		}
		if(!pTarget->Available.isValid()){
			bValid = false;
			fprintf(pFile, "Order->Available:inValid\n");
		}
		if(!pTarget->CreateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Order->CreateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void COrderFactory::linkAllInstrument()
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkInstrument();
		pTarget=getNext();
	}
}

void COrderFactory::linkAllPosition()
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkPosition();
		pTarget=getNext();
	}
}

void COrderFactory::linkAllMember()
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkMember();
		pTarget=getNext();
	}
}

void COrderFactory::linkAllTriggerOrder()
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkTriggerOrder();
		pTarget=getNext();
	}
}

void COrderFactory::checkLinkInstrument(FILE *pFile)
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getInstrument() == NULL){
			if (pTarget->ExchangeID.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->InstrumentID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Order->Instrument:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void COrderFactory::checkLinkPosition(FILE *pFile)
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getPosition() == NULL){
			if (pTarget->PositionID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Order->Position:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void COrderFactory::checkLinkMember(FILE *pFile)
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getMember() == NULL){
			if (pTarget->MemberID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Order->Member:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void COrderFactory::checkLinkTriggerOrder(FILE *pFile)
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getTriggerOrder() == NULL){
			if (pTarget->TriggerOrderID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "Order->TriggerOrder:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void COrderFactory::clearLinkInstrument()
{
}

void COrderFactory::clearLinkPosition()
{
}

void COrderFactory::clearLinkMember()
{
}

void COrderFactory::clearLinkTriggerOrder()
{
	COrder *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getTriggerOrder() == NULL){
			CWriteableOrder theTarget;
			retrieve(pTarget, &theTarget);
			theTarget.TriggerOrderID.clear();	
			update(pTarget, &theTarget);
		}
		pTarget = getNext();
	}
}

extern int compareForInsertTimeIndexinPositionOrder(const void *pV1, const void *pV2);
void CPositionOrderFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pInsertTimeIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		if(!reuse) {
			pInsertTimeIndex=new CAVLTree(maxUnit,compareForInsertTimeIndexinPositionOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("PositionOrder_InsertTimeIndex",pInsertTimeIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("PositionOrder_InsertTimeIndex");
			if(it != pIndexMap->end()) {
				pInsertTimeIndex=new CAVLTree(maxUnit,compareForInsertTimeIndexinPositionOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pInsertTimeIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pOrderIDHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		int hashKeySize=1572869;
		if(!reuse) {
			pOrderIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("PositionOrder_OrderIDHashIndex",pOrderIDHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("PositionOrder_OrderIDHashIndex");
			if(it != pIndexMap->end()) {
				pOrderIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pOrderIDHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pLastFoundInSearchByInsertTime=NULL;
	pActionTriggers=new vector<CPositionOrderActionTrigger *>;
	pCommitTriggers=new vector<CPositionOrderCommitTrigger *>;
}

CPositionOrderFactory::CPositionOrderFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CPositionOrder),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CPositionOrderFactory::CPositionOrderFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CPositionOrder),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CPositionOrderFactory::~CPositionOrderFactory()
{
	if (pInsertTimeIndex!=NULL)
		delete pInsertTimeIndex;
	if (pOrderIDHashIndex!=NULL)
		delete pOrderIDHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CPositionOrderFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteablePositionOrder thisPositionOrder;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisPositionOrder.readCSV(input,pNames))
		add(&thisPositionOrder);
	fclose(input);
	delete pNames;
	return 1;
}

int CPositionOrderFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "PositionOrder.csv");
	return readCSV(szFileName);
}

int CPositionOrderFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteablePositionOrder *pPositionOrder;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteablePositionOrder::writeCSVHead(output);
	pPositionOrder=(CWriteablePositionOrder *)(pMem->getFirst());
	while (pPositionOrder!=NULL) {
		pPositionOrder->writeCSV(output);
		pPositionOrder=(CWriteablePositionOrder *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CPositionOrderFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteablePositionOrder *pPositionOrder;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteablePositionOrder::writeCSVHeadPreVersion(output);
	pPositionOrder=(CWriteablePositionOrder *)(pMem->getFirst());
	while (pPositionOrder!=NULL) {
		pPositionOrder->writeCSVPreVersion(output);
		pPositionOrder=(CWriteablePositionOrder *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CPositionOrderFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "PositionOrder.csv");
	return writeCSV(szFileName);
}

void CPositionOrderFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CPositionOrderFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteablePositionOrder *pPositionOrder;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CPositionOrderFactory={       Total Count=%d\n", pMem->getCount());
	pPositionOrder=(CWriteablePositionOrder *)(pMem->getFirst());
	while (pPositionOrder!=NULL) {
		pPositionOrder->dump(fp,index++);
		pPositionOrder=(CWriteablePositionOrder *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CPositionOrderFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pInsertTimeIndex != NULL)
		pInsertTimeIndex->removeAll();
	if(pOrderIDHashIndex != NULL)
		pOrderIDHashIndex->removeAll();
}

CPositionOrder *CPositionOrderFactory::internalAdd(CWriteablePositionOrder *pPositionOrder, bool bNoTransaction)
{
	pPositionOrder->m_pMDB = m_pMDB;
	if(pPositionOrder->InsertTime.isNull())
		pPositionOrder->InsertTime = g_nCurrentTime;
	if(pPositionOrder->Tradable.isNull())
		pPositionOrder->Tradable = 1;
	CPositionOrder *pTarget;
	//beforeAdd(pPositionOrder);
	pTarget=(CPositionOrder *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough PositionOrder in memory database");
		return NULL;
	}
	forceCopy(pTarget, pPositionOrder, sizeof(CPositionOrder));
	pMem->updateObject(pTarget);
	if (pInsertTimeIndex != NULL)
		pInsertTimeIndex->addObject(pTarget);
	if (pOrderIDHashIndex != NULL)
		pOrderIDHashIndex->addObject(pTarget,pTarget->HashOrderID);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CPositionOrder* CPositionOrderFactory::add(CWriteablePositionOrder *pPositionOrder, CTransaction *pTransaction)
{
	pPositionOrder->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pPositionOrder,true);
	}else{
		CPositionOrder *pNewPositionOrder;
		pNewPositionOrder = internalAdd(pPositionOrder,false);
		pTransaction->addResource(CPositionOrderResource::alloc(CREATE_ACTION,this,pNewPositionOrder));
		return pNewPositionOrder;
	}
}

CPositionOrder* CPositionOrderFactory::internalUpdate(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pNewPositionOrder,bool bNoTransaction)
{
	pNewPositionOrder->UpdateTime = g_nCurrentTime;
	CWriteablePositionOrder theOldPositionOrder;
	if (bNoTransaction)
		forceCopy(&theOldPositionOrder,pPositionOrder,sizeof(CPositionOrder));	
	CPositionOrder *pTarget = pPositionOrder;
	forceCopy(pPositionOrder,pNewPositionOrder,sizeof(CPositionOrder));
	pMem->updateObject(pPositionOrder);
	if (bNoTransaction)
		commitUpdate(pPositionOrder,&theOldPositionOrder);
	return pPositionOrder;
}

CPositionOrder* CPositionOrderFactory::update(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pNewPositionOrder, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pPositionOrder == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pPositionOrder,pNewPositionOrder,bNoTransaction);
	}else{
		pTransaction->addResource(CPositionOrderResource::alloc(UPDATE_ACTION,this,pPositionOrder));
		return internalUpdate(pPositionOrder,pNewPositionOrder,false);
	}
}

void CPositionOrderFactory::internalRemove(CPositionOrder *pPositionOrder, bool bNoTransaction)
{
	CWriteablePositionOrder theOldPositionOrder;
	//beforeRemove(pPositionOrder);
	if (bNoTransaction)
		forceCopy(&theOldPositionOrder,pPositionOrder,sizeof(CPositionOrder));
	CPositionOrder *pTarget = pPositionOrder;
	if (pInsertTimeIndex != NULL)
		pInsertTimeIndex->removeObject(pPositionOrder);	
	
	if (pOrderIDHashIndex != NULL)
		pOrderIDHashIndex->removeObject(pPositionOrder,pPositionOrder->HashOrderID);		
	pMem->free(pPositionOrder);
	if(bNoTransaction)
		commitRemove(&theOldPositionOrder);
}

void CPositionOrderFactory::remove(CPositionOrder *pPositionOrder, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pPositionOrder == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pPositionOrder, bNoTransaction);
	}else{
		pTransaction->addResource(CPositionOrderResource::alloc(DELETE_ACTION,this,pPositionOrder));
		internalRemove(pPositionOrder,false);
	}
}

CPositionOrder* CPositionOrderFactory::addOrUpdate(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pNewPositionOrder, CTransaction *pTransaction)
{
	if(pPositionOrder == NULL)
		return add(pNewPositionOrder,pTransaction);
	else
		return update(pPositionOrder,pNewPositionOrder,pTransaction);
}

void CPositionOrderFactory::retrieve(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pTargetPositionOrder)
{
	if(pPositionOrder == NULL)
		return;
	forceCopy(pTargetPositionOrder, pPositionOrder, sizeof(CPositionOrder));
}
	
int CPositionOrderFactory::addActionTrigger(CPositionOrderActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CPositionOrderFactory::removeActionTrigger(CPositionOrderActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CPositionOrderFactory::addCommitTrigger(CPositionOrderCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CPositionOrderFactory::removeCommitTrigger(CPositionOrderCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CPositionOrder* CPositionOrderFactory::getFirst()
{
	return (CPositionOrder *)(pMem->getFirst());
}
	
CPositionOrder* CPositionOrderFactory::getNext()
{
	return (CPositionOrder *)(pMem->getNext());
}

void CPositionOrderFactory::commitAdd(CPositionOrder *pPositionOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pPositionOrder);
	}
}

void CPositionOrderFactory::commitUpdate(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pOldPositionOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pPositionOrder,pOldPositionOrder);
	}
}
	
void CPositionOrderFactory::commitRemove(CWriteablePositionOrder *pPositionOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pPositionOrder);
	}
}

void CPositionOrderFactory::checkNull(FILE *pFile)
{
	CPositionOrder *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->MemberID:NULL\n");
		}
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->InstrumentID:NULL\n");
		}
		if(pTarget->OrderPriceType.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->OrderPriceType:NULL\n");
		}
		if(pTarget->Direction.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->Direction:NULL\n");
		}
		if(pTarget->OffsetFlag.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->OffsetFlag:NULL\n");
		}
		if(pTarget->OrderType.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->OrderType:NULL\n");
		}
		if(pTarget->BusinessType.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->BusinessType:NULL\n");
		}
		if(pTarget->OrderID.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->OrderID:NULL\n");
		}
		if(pTarget->PositionID.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->PositionID:NULL\n");
		}
		if(pTarget->OrderStatus.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->OrderStatus:NULL\n");
		}
		if(pTarget->DeriveSource.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->DeriveSource:NULL\n");
		}
		if(pTarget->VolumeTraded.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->VolumeTraded:NULL\n");
		}
		if(pTarget->VolumeRemain.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->VolumeRemain:NULL\n");
		}
		if(pTarget->VolumeCancled.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->VolumeCancled:NULL\n");
		}
		if(pTarget->Priority.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->Priority:NULL\n");
		}
		if(pTarget->TimeSortNo.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->TimeSortNo:NULL\n");
		}
		if(pTarget->FrozenMoney.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->FrozenMoney:NULL\n");
		}
		if(pTarget->FrozenFee.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->FrozenFee:NULL\n");
		}
		if(pTarget->FrozenMargin.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->FrozenMargin:NULL\n");
		}
		if(pTarget->Fee.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->Fee:NULL\n");
		}
		if(pTarget->CloseProfit.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->CloseProfit:NULL\n");
		}
		if(pTarget->Turnover.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->Turnover:NULL\n");
		}
		if(pTarget->Tradable.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->Tradable:NULL\n");
		}
		if(pTarget->SettlementGroup.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->SettlementGroup:NULL\n");
		}
		if(pTarget->PosiDirection.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->PosiDirection:NULL\n");
		}
		if(pTarget->CopyProfit.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->CopyProfit:NULL\n");
		}
		if(pTarget->UserID.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->UserID:NULL\n");
		}
		if(pTarget->VolumeToClose.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->VolumeToClose:NULL\n");
		}
		if(pTarget->VolumeClosed.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->VolumeClosed:NULL\n");
		}
		if(pTarget->VolumeOnClose.isNull()){
			bNull = true;
			fprintf(pFile, "PositionOrder->VolumeOnClose:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CPositionOrderFactory::checkValid(FILE *pFile)
{
	CPositionOrder *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->MemberID:inValid\n");
		}
		if(!pTarget->TradeUnitID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->TradeUnitID:inValid\n");
		}
		if(!pTarget->AccountID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->AccountID:inValid\n");
		}
		if(!pTarget->LocalID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->LocalID:inValid\n");
		}
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->InstrumentID:inValid\n");
		}
		if(!pTarget->OrderPriceType.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->OrderPriceType:inValid\n");
		}
		if(!pTarget->Direction.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Direction:inValid\n");
		}
		if(!pTarget->OffsetFlag.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->OffsetFlag:inValid\n");
		}
		if(!pTarget->Price.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Price:inValid\n");
		}
		if(!pTarget->Volume.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Volume:inValid\n");
		}
		if(!pTarget->VolumeDisplay.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->VolumeDisplay:inValid\n");
		}
		if(!pTarget->VolumeMode.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->VolumeMode:inValid\n");
		}
		if(!pTarget->Cost.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Cost:inValid\n");
		}
		if(!pTarget->OrderType.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->OrderType:inValid\n");
		}
		if(!pTarget->GTDTime.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->GTDTime:inValid\n");
		}
		if(!pTarget->MinVolume.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->MinVolume:inValid\n");
		}
		if(!pTarget->BusinessType.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->BusinessType:inValid\n");
		}
		if(!pTarget->BusinessValue.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->BusinessValue:inValid\n");
		}
		if(!pTarget->CloseOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->CloseOrderID:inValid\n");
		}
		if(!pTarget->IsCrossMargin.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->IsCrossMargin:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Remark:inValid\n");
		}
		if(!pTarget->OrderID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->OrderID:inValid\n");
		}
		if(!pTarget->CopyMemberID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->CopyMemberID:inValid\n");
		}
		if(!pTarget->CopyOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->CopyOrderID:inValid\n");
		}
		if(!pTarget->Leverage.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Leverage:inValid\n");
		}
		if(!pTarget->CopyProfitRate.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->CopyProfitRate:inValid\n");
		}
		if(!pTarget->APPID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->APPID:inValid\n");
		}
		if(!pTarget->PositionID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->PositionID:inValid\n");
		}
		if(!pTarget->TriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->TriggerPrice:inValid\n");
		}
		if(!pTarget->Reserve.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Reserve:inValid\n");
		}
		if(!pTarget->OrderStatus.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->OrderStatus:inValid\n");
		}
		if(!pTarget->DeriveSource.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->DeriveSource:inValid\n");
		}
		if(!pTarget->DeriveDetail.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->DeriveDetail:inValid\n");
		}
		if(!pTarget->VolumeTraded.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->VolumeTraded:inValid\n");
		}
		if(!pTarget->VolumeRemain.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->VolumeRemain:inValid\n");
		}
		if(!pTarget->VolumeCancled.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->VolumeCancled:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->UpdateTime:inValid\n");
		}
		if(!pTarget->Priority.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Priority:inValid\n");
		}
		if(!pTarget->TimeSortNo.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->TimeSortNo:inValid\n");
		}
		if(!pTarget->FrontNo.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->FrontNo:inValid\n");
		}
		if(!pTarget->PriceCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->PriceCurrency:inValid\n");
		}
		if(!pTarget->FeeCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->FeeCurrency:inValid\n");
		}
		if(!pTarget->ClearCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->ClearCurrency:inValid\n");
		}
		if(!pTarget->FrozenMoney.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->FrozenMoney:inValid\n");
		}
		if(!pTarget->FrozenFee.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->FrozenFee:inValid\n");
		}
		if(!pTarget->FrozenMargin.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->FrozenMargin:inValid\n");
		}
		if(!pTarget->Fee.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Fee:inValid\n");
		}
		if(!pTarget->CloseProfit.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->CloseProfit:inValid\n");
		}
		if(!pTarget->Turnover.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Turnover:inValid\n");
		}
		if(!pTarget->RelatedOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->RelatedOrderID:inValid\n");
		}
		if(!pTarget->BusinessResult.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->BusinessResult:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->BusinessNo:inValid\n");
		}
		if(!pTarget->Tradable.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Tradable:inValid\n");
		}
		if(!pTarget->SettlementGroup.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->SettlementGroup:inValid\n");
		}
		if(!pTarget->PosiDirection.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->PosiDirection:inValid\n");
		}
		if(!pTarget->TradePrice.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->TradePrice:inValid\n");
		}
		if(!pTarget->OpenPrice.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->OpenPrice:inValid\n");
		}
		if(!pTarget->TriggerOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->TriggerOrderID:inValid\n");
		}
		if(!pTarget->SLTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->SLTriggerPrice:inValid\n");
		}
		if(!pTarget->TPTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->TPTriggerPrice:inValid\n");
		}
		if(!pTarget->CopyProfit.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->CopyProfit:inValid\n");
		}
		if(!pTarget->Position.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Position:inValid\n");
		}
		if(!pTarget->UserID.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->UserID:inValid\n");
		}
		if(!pTarget->LastPriceByInsert.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->LastPriceByInsert:inValid\n");
		}
		if(!pTarget->BidPrice1ByInsert.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->BidPrice1ByInsert:inValid\n");
		}
		if(!pTarget->AskPrice1ByInsert.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->AskPrice1ByInsert:inValid\n");
		}
		if(!pTarget->Available.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->Available:inValid\n");
		}
		if(!pTarget->CreateTime.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->CreateTime:inValid\n");
		}
		if(!pTarget->VolumeToClose.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->VolumeToClose:inValid\n");
		}
		if(!pTarget->VolumeClosed.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->VolumeClosed:inValid\n");
		}
		if(!pTarget->VolumeOnClose.isValid()){
			bValid = false;
			fprintf(pFile, "PositionOrder->VolumeOnClose:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

extern int compareForAccountIDLocalIDIndexinTriggerOrder(const void *pV1, const void *pV2);
extern int compareForActivePositionCloseIndexinTriggerOrder(const void *pV1, const void *pV2);
extern int compareForBuySLTriggerPriceIndexinTriggerOrder(const void *pV1, const void *pV2);
extern int compareForSellSLTriggerPriceIndexinTriggerOrder(const void *pV1, const void *pV2);
extern int compareForBuyTPTriggerPriceIndexinTriggerOrder(const void *pV1, const void *pV2);
extern int compareForSellTPTriggerPriceIndexinTriggerOrder(const void *pV1, const void *pV2);
void CTriggerOrderFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pAccountIDLocalIDIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		if(!reuse) {
			pAccountIDLocalIDIndex=new CAVLTree(maxUnit,compareForAccountIDLocalIDIndexinTriggerOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TriggerOrder_AccountIDLocalIDIndex",pAccountIDLocalIDIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("TriggerOrder_AccountIDLocalIDIndex");
			if(it != pIndexMap->end()) {
				pAccountIDLocalIDIndex=new CAVLTree(maxUnit,compareForAccountIDLocalIDIndexinTriggerOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pAccountIDLocalIDIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pActivePositionCloseIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		if(!reuse) {
			pActivePositionCloseIndex=new CAVLTree(maxUnit,compareForActivePositionCloseIndexinTriggerOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TriggerOrder_ActivePositionCloseIndex",pActivePositionCloseIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("TriggerOrder_ActivePositionCloseIndex");
			if(it != pIndexMap->end()) {
				pActivePositionCloseIndex=new CAVLTree(maxUnit,compareForActivePositionCloseIndexinTriggerOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pActivePositionCloseIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pBuySLTriggerPriceIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 't':
	{
		if(!reuse) {
			pBuySLTriggerPriceIndex=new CAVLTree(maxUnit,compareForBuySLTriggerPriceIndexinTriggerOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TriggerOrder_BuySLTriggerPriceIndex",pBuySLTriggerPriceIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("TriggerOrder_BuySLTriggerPriceIndex");
			if(it != pIndexMap->end()) {
				pBuySLTriggerPriceIndex=new CAVLTree(maxUnit,compareForBuySLTriggerPriceIndexinTriggerOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pBuySLTriggerPriceIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pSellSLTriggerPriceIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 't':
	{
		if(!reuse) {
			pSellSLTriggerPriceIndex=new CAVLTree(maxUnit,compareForSellSLTriggerPriceIndexinTriggerOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TriggerOrder_SellSLTriggerPriceIndex",pSellSLTriggerPriceIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("TriggerOrder_SellSLTriggerPriceIndex");
			if(it != pIndexMap->end()) {
				pSellSLTriggerPriceIndex=new CAVLTree(maxUnit,compareForSellSLTriggerPriceIndexinTriggerOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pSellSLTriggerPriceIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pBuyTPTriggerPriceIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 't':
	{
		if(!reuse) {
			pBuyTPTriggerPriceIndex=new CAVLTree(maxUnit,compareForBuyTPTriggerPriceIndexinTriggerOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TriggerOrder_BuyTPTriggerPriceIndex",pBuyTPTriggerPriceIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("TriggerOrder_BuyTPTriggerPriceIndex");
			if(it != pIndexMap->end()) {
				pBuyTPTriggerPriceIndex=new CAVLTree(maxUnit,compareForBuyTPTriggerPriceIndexinTriggerOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pBuyTPTriggerPriceIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pSellTPTriggerPriceIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 't':
	{
		if(!reuse) {
			pSellTPTriggerPriceIndex=new CAVLTree(maxUnit,compareForSellTPTriggerPriceIndexinTriggerOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TriggerOrder_SellTPTriggerPriceIndex",pSellTPTriggerPriceIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("TriggerOrder_SellTPTriggerPriceIndex");
			if(it != pIndexMap->end()) {
				pSellTPTriggerPriceIndex=new CAVLTree(maxUnit,compareForSellTPTriggerPriceIndexinTriggerOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pSellTPTriggerPriceIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pOrderIDHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 't':
	case 'p':
	{
		int hashKeySize=1572869;
		if(!reuse) {
			pOrderIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TriggerOrder_OrderIDHashIndex",pOrderIDHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("TriggerOrder_OrderIDHashIndex");
			if(it != pIndexMap->end()) {
				pOrderIDHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pOrderIDHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pLastFoundInSearchByBuySLTriggerPrice=NULL;
	pLastFoundInSearchBySellSLTriggerPrice=NULL;
	pLastFoundInSearchByBuyTPTriggerPrice=NULL;
	pLastFoundInSearchBySellTPTriggerPrice=NULL;
	pLastFoundInSearchByAccountID=NULL;
	pLastFoundInSearchByActivePositionClose=NULL;
	pActionTriggers=new vector<CTriggerOrderActionTrigger *>;
	pCommitTriggers=new vector<CTriggerOrderCommitTrigger *>;
}

CTriggerOrderFactory::CTriggerOrderFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CTriggerOrder),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CTriggerOrderFactory::CTriggerOrderFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CTriggerOrder),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CTriggerOrderFactory::~CTriggerOrderFactory()
{
	if (pAccountIDLocalIDIndex!=NULL)
		delete pAccountIDLocalIDIndex;
	if (pActivePositionCloseIndex!=NULL)
		delete pActivePositionCloseIndex;
	if (pBuySLTriggerPriceIndex!=NULL)
		delete pBuySLTriggerPriceIndex;
	if (pSellSLTriggerPriceIndex!=NULL)
		delete pSellSLTriggerPriceIndex;
	if (pBuyTPTriggerPriceIndex!=NULL)
		delete pBuyTPTriggerPriceIndex;
	if (pSellTPTriggerPriceIndex!=NULL)
		delete pSellTPTriggerPriceIndex;
	if (pOrderIDHashIndex!=NULL)
		delete pOrderIDHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CTriggerOrderFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableTriggerOrder thisTriggerOrder;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisTriggerOrder.readCSV(input,pNames))
		add(&thisTriggerOrder);
	fclose(input);
	delete pNames;
	return 1;
}

int CTriggerOrderFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "TriggerOrder.csv");
	return readCSV(szFileName);
}

int CTriggerOrderFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableTriggerOrder *pTriggerOrder;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableTriggerOrder::writeCSVHead(output);
	pTriggerOrder=(CWriteableTriggerOrder *)(pMem->getFirst());
	while (pTriggerOrder!=NULL) {
		pTriggerOrder->writeCSV(output);
		pTriggerOrder=(CWriteableTriggerOrder *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CTriggerOrderFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableTriggerOrder *pTriggerOrder;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableTriggerOrder::writeCSVHeadPreVersion(output);
	pTriggerOrder=(CWriteableTriggerOrder *)(pMem->getFirst());
	while (pTriggerOrder!=NULL) {
		pTriggerOrder->writeCSVPreVersion(output);
		pTriggerOrder=(CWriteableTriggerOrder *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CTriggerOrderFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "TriggerOrder.csv");
	return writeCSV(szFileName);
}

void CTriggerOrderFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CTriggerOrderFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableTriggerOrder *pTriggerOrder;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CTriggerOrderFactory={       Total Count=%d\n", pMem->getCount());
	pTriggerOrder=(CWriteableTriggerOrder *)(pMem->getFirst());
	while (pTriggerOrder!=NULL) {
		pTriggerOrder->dump(fp,index++);
		pTriggerOrder=(CWriteableTriggerOrder *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CTriggerOrderFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pAccountIDLocalIDIndex != NULL)
		pAccountIDLocalIDIndex->removeAll();
	if(pActivePositionCloseIndex != NULL)
		pActivePositionCloseIndex->removeAll();
	if(pBuySLTriggerPriceIndex != NULL)
		pBuySLTriggerPriceIndex->removeAll();
	if(pSellSLTriggerPriceIndex != NULL)
		pSellSLTriggerPriceIndex->removeAll();
	if(pBuyTPTriggerPriceIndex != NULL)
		pBuyTPTriggerPriceIndex->removeAll();
	if(pSellTPTriggerPriceIndex != NULL)
		pSellTPTriggerPriceIndex->removeAll();
	if(pOrderIDHashIndex != NULL)
		pOrderIDHashIndex->removeAll();
}

CTriggerOrder *CTriggerOrderFactory::internalAdd(CWriteableTriggerOrder *pTriggerOrder, bool bNoTransaction)
{
	pTriggerOrder->m_pMDB = m_pMDB;
	if(pTriggerOrder->TriggerPriceType.isNull())
		pTriggerOrder->TriggerPriceType = "0";
	if(pTriggerOrder->InsertTime.isNull())
		pTriggerOrder->InsertTime = g_nCurrentTime;
	CTriggerOrder *pTarget;
	//beforeAdd(pTriggerOrder);
	pTarget=(CTriggerOrder *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough TriggerOrder in memory database");
		return NULL;
	}
	forceCopy(pTarget, pTriggerOrder, sizeof(CTriggerOrder));
	pMem->updateObject(pTarget);
	if (pAccountIDLocalIDIndex != NULL)
		pAccountIDLocalIDIndex->addObject(pTarget);
	if (pTarget->OffsetFlag!=OF_Open && pTarget->TriggerStatus==TS_Active){
		if (pActivePositionCloseIndex != NULL)
			pActivePositionCloseIndex->addObject(pTarget);
	}
	if (pTarget->TriggerStatus!=TS_None && !pTarget->SLTriggerPrice.isNull()){
		if (pBuySLTriggerPriceIndex != NULL)
			pBuySLTriggerPriceIndex->addObject(pTarget);
	}
	if (pTarget->TriggerStatus!=TS_None && !pTarget->SLTriggerPrice.isNull()){
		if (pSellSLTriggerPriceIndex != NULL)
			pSellSLTriggerPriceIndex->addObject(pTarget);
	}
	if (pTarget->TriggerStatus!=TS_None && !pTarget->TPTriggerPrice.isNull()){
		if (pBuyTPTriggerPriceIndex != NULL)
			pBuyTPTriggerPriceIndex->addObject(pTarget);
	}
	if (pTarget->TriggerStatus!=TS_None && !pTarget->TPTriggerPrice.isNull()){
		if (pSellTPTriggerPriceIndex != NULL)
			pSellTPTriggerPriceIndex->addObject(pTarget);
	}
	if (pOrderIDHashIndex != NULL)
		pOrderIDHashIndex->addObject(pTarget,pTarget->HashOrderID);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CTriggerOrder* CTriggerOrderFactory::add(CWriteableTriggerOrder *pTriggerOrder, CTransaction *pTransaction)
{
	pTriggerOrder->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pTriggerOrder,true);
	}else{
		CTriggerOrder *pNewTriggerOrder;
		pNewTriggerOrder = internalAdd(pTriggerOrder,false);
		pTransaction->addResource(CTriggerOrderResource::alloc(CREATE_ACTION,this,pNewTriggerOrder));
		return pNewTriggerOrder;
	}
}

CTriggerOrder* CTriggerOrderFactory::internalUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pNewTriggerOrder,bool bNoTransaction)
{
	pNewTriggerOrder->UpdateTime = g_nCurrentTime;
	CWriteableTriggerOrder theOldTriggerOrder;
	if (bNoTransaction)
		forceCopy(&theOldTriggerOrder,pTriggerOrder,sizeof(CTriggerOrder));	
	CAVLNode *pActivePositionCloseIndexNode=NULL;
	CAVLNode *pBuySLTriggerPriceIndexNode=NULL;
	CAVLNode *pSellSLTriggerPriceIndexNode=NULL;
	CAVLNode *pBuyTPTriggerPriceIndexNode=NULL;
	CAVLNode *pSellTPTriggerPriceIndexNode=NULL;
	CTriggerOrder *pTarget = pTriggerOrder;
	if (pTarget->OffsetFlag!=OF_Open && pTarget->TriggerStatus==TS_Active)
		if (pActivePositionCloseIndex != NULL)
			pActivePositionCloseIndex->removeObject(pTriggerOrder);
	if (pTarget->TriggerStatus!=TS_None && !pTarget->SLTriggerPrice.isNull())
		if (pBuySLTriggerPriceIndex != NULL)
			pBuySLTriggerPriceIndex->removeObject(pTriggerOrder);
	if (pTarget->TriggerStatus!=TS_None && !pTarget->SLTriggerPrice.isNull())
		if (pSellSLTriggerPriceIndex != NULL)
			pSellSLTriggerPriceIndex->removeObject(pTriggerOrder);
	if (pTarget->TriggerStatus!=TS_None && !pTarget->TPTriggerPrice.isNull())
		if (pBuyTPTriggerPriceIndex != NULL)
			pBuyTPTriggerPriceIndex->removeObject(pTriggerOrder);
	if (pTarget->TriggerStatus!=TS_None && !pTarget->TPTriggerPrice.isNull())
		if (pSellTPTriggerPriceIndex != NULL)
			pSellTPTriggerPriceIndex->removeObject(pTriggerOrder);
	forceCopy(pTriggerOrder,pNewTriggerOrder,sizeof(CTriggerOrder));
	pMem->updateObject(pTriggerOrder);
	if (pTarget->OffsetFlag!=OF_Open && pTarget->TriggerStatus==TS_Active)
		if (pActivePositionCloseIndex != NULL)
			pActivePositionCloseIndex->addObject(pTarget);
	if (pTarget->TriggerStatus!=TS_None && !pTarget->SLTriggerPrice.isNull())
		if (pBuySLTriggerPriceIndex != NULL)
			pBuySLTriggerPriceIndex->addObject(pTarget);
	if (pTarget->TriggerStatus!=TS_None && !pTarget->SLTriggerPrice.isNull())
		if (pSellSLTriggerPriceIndex != NULL)
			pSellSLTriggerPriceIndex->addObject(pTarget);
	if (pTarget->TriggerStatus!=TS_None && !pTarget->TPTriggerPrice.isNull())
		if (pBuyTPTriggerPriceIndex != NULL)
			pBuyTPTriggerPriceIndex->addObject(pTarget);
	if (pTarget->TriggerStatus!=TS_None && !pTarget->TPTriggerPrice.isNull())
		if (pSellTPTriggerPriceIndex != NULL)
			pSellTPTriggerPriceIndex->addObject(pTarget);
	if (bNoTransaction)
		commitUpdate(pTriggerOrder,&theOldTriggerOrder);
	return pTriggerOrder;
}

CTriggerOrder* CTriggerOrderFactory::update(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pNewTriggerOrder, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pTriggerOrder == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pTriggerOrder,pNewTriggerOrder,bNoTransaction);
	}else{
		pTransaction->addResource(CTriggerOrderResource::alloc(UPDATE_ACTION,this,pTriggerOrder));
		return internalUpdate(pTriggerOrder,pNewTriggerOrder,false);
	}
}

void CTriggerOrderFactory::internalRemove(CTriggerOrder *pTriggerOrder, bool bNoTransaction)
{
	CWriteableTriggerOrder theOldTriggerOrder;
	//beforeRemove(pTriggerOrder);
	if (bNoTransaction)
		forceCopy(&theOldTriggerOrder,pTriggerOrder,sizeof(CTriggerOrder));
	CTriggerOrder *pTarget = pTriggerOrder;
	if (pAccountIDLocalIDIndex != NULL)
		pAccountIDLocalIDIndex->removeObject(pTriggerOrder);	
	if (pTarget->OffsetFlag!=OF_Open && pTarget->TriggerStatus==TS_Active){
		if (pActivePositionCloseIndex != NULL)
			pActivePositionCloseIndex->removeObject(pTriggerOrder);
	}
	if (pTarget->TriggerStatus!=TS_None && !pTarget->SLTriggerPrice.isNull()){
		if (pBuySLTriggerPriceIndex != NULL)
			pBuySLTriggerPriceIndex->removeObject(pTriggerOrder);
	}
	if (pTarget->TriggerStatus!=TS_None && !pTarget->SLTriggerPrice.isNull()){
		if (pSellSLTriggerPriceIndex != NULL)
			pSellSLTriggerPriceIndex->removeObject(pTriggerOrder);
	}
	if (pTarget->TriggerStatus!=TS_None && !pTarget->TPTriggerPrice.isNull()){
		if (pBuyTPTriggerPriceIndex != NULL)
			pBuyTPTriggerPriceIndex->removeObject(pTriggerOrder);
	}
	if (pTarget->TriggerStatus!=TS_None && !pTarget->TPTriggerPrice.isNull()){
		if (pSellTPTriggerPriceIndex != NULL)
			pSellTPTriggerPriceIndex->removeObject(pTriggerOrder);
	}
	
	if (pOrderIDHashIndex != NULL)
		pOrderIDHashIndex->removeObject(pTriggerOrder,pTriggerOrder->HashOrderID);		
	pMem->free(pTriggerOrder);
	if(bNoTransaction)
		commitRemove(&theOldTriggerOrder);
}

void CTriggerOrderFactory::remove(CTriggerOrder *pTriggerOrder, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pTriggerOrder == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pTriggerOrder, bNoTransaction);
	}else{
		pTransaction->addResource(CTriggerOrderResource::alloc(DELETE_ACTION,this,pTriggerOrder));
		internalRemove(pTriggerOrder,false);
	}
}

CTriggerOrder* CTriggerOrderFactory::addOrUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pNewTriggerOrder, CTransaction *pTransaction)
{
	if(pTriggerOrder == NULL)
		return add(pNewTriggerOrder,pTransaction);
	else
		return update(pTriggerOrder,pNewTriggerOrder,pTransaction);
}

void CTriggerOrderFactory::retrieve(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pTargetTriggerOrder)
{
	if(pTriggerOrder == NULL)
		return;
	forceCopy(pTargetTriggerOrder, pTriggerOrder, sizeof(CTriggerOrder));
}
	
int CTriggerOrderFactory::addActionTrigger(CTriggerOrderActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CTriggerOrderFactory::removeActionTrigger(CTriggerOrderActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CTriggerOrderFactory::addCommitTrigger(CTriggerOrderCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CTriggerOrderFactory::removeCommitTrigger(CTriggerOrderCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CTriggerOrder* CTriggerOrderFactory::getFirst()
{
	return (CTriggerOrder *)(pMem->getFirst());
}
	
CTriggerOrder* CTriggerOrderFactory::getNext()
{
	return (CTriggerOrder *)(pMem->getNext());
}

void CTriggerOrderFactory::commitAdd(CTriggerOrder *pTriggerOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pTriggerOrder);
	}
}

void CTriggerOrderFactory::commitUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pOldTriggerOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pTriggerOrder,pOldTriggerOrder);
	}
}
	
void CTriggerOrderFactory::commitRemove(CWriteableTriggerOrder *pTriggerOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pTriggerOrder);
	}
}

void CTriggerOrderFactory::checkNull(FILE *pFile)
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->MemberID:NULL\n");
		}
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->InstrumentID:NULL\n");
		}
		if(pTarget->OrderPriceType.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->OrderPriceType:NULL\n");
		}
		if(pTarget->Direction.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->Direction:NULL\n");
		}
		if(pTarget->OffsetFlag.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->OffsetFlag:NULL\n");
		}
		if(pTarget->OrderType.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->OrderType:NULL\n");
		}
		if(pTarget->BusinessType.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->BusinessType:NULL\n");
		}
		if(pTarget->OrderID.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->OrderID:NULL\n");
		}
		if(pTarget->PositionID.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->PositionID:NULL\n");
		}
		if(pTarget->RiskBefore.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->RiskBefore:NULL\n");
		}
		if(pTarget->TriggerOrderType.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->TriggerOrderType:NULL\n");
		}
		if(pTarget->TriggerPriceType.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->TriggerPriceType:NULL\n");
		}
		if(pTarget->TimeSortNo.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->TimeSortNo:NULL\n");
		}
		if(pTarget->TriggerStatus.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->TriggerStatus:NULL\n");
		}
		if(pTarget->InsertTime.isNull()){
			bNull = true;
			fprintf(pFile, "TriggerOrder->InsertTime:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CTriggerOrderFactory::checkValid(FILE *pFile)
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->MemberID:inValid\n");
		}
		if(!pTarget->TradeUnitID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TradeUnitID:inValid\n");
		}
		if(!pTarget->AccountID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->AccountID:inValid\n");
		}
		if(!pTarget->LocalID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->LocalID:inValid\n");
		}
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->InstrumentID:inValid\n");
		}
		if(!pTarget->OrderPriceType.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->OrderPriceType:inValid\n");
		}
		if(!pTarget->Direction.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->Direction:inValid\n");
		}
		if(!pTarget->OffsetFlag.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->OffsetFlag:inValid\n");
		}
		if(!pTarget->Price.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->Price:inValid\n");
		}
		if(!pTarget->Volume.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->Volume:inValid\n");
		}
		if(!pTarget->VolumeDisplay.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->VolumeDisplay:inValid\n");
		}
		if(!pTarget->VolumeMode.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->VolumeMode:inValid\n");
		}
		if(!pTarget->Cost.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->Cost:inValid\n");
		}
		if(!pTarget->OrderType.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->OrderType:inValid\n");
		}
		if(!pTarget->GTDTime.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->GTDTime:inValid\n");
		}
		if(!pTarget->MinVolume.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->MinVolume:inValid\n");
		}
		if(!pTarget->BusinessType.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->BusinessType:inValid\n");
		}
		if(!pTarget->BusinessValue.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->BusinessValue:inValid\n");
		}
		if(!pTarget->CloseOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CloseOrderID:inValid\n");
		}
		if(!pTarget->IsCrossMargin.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->IsCrossMargin:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->Remark:inValid\n");
		}
		if(!pTarget->OrderID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->OrderID:inValid\n");
		}
		if(!pTarget->CopyMemberID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CopyMemberID:inValid\n");
		}
		if(!pTarget->CopyOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CopyOrderID:inValid\n");
		}
		if(!pTarget->Leverage.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->Leverage:inValid\n");
		}
		if(!pTarget->CopyProfitRate.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CopyProfitRate:inValid\n");
		}
		if(!pTarget->APPID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->APPID:inValid\n");
		}
		if(!pTarget->PositionID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->PositionID:inValid\n");
		}
		if(!pTarget->TriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TriggerPrice:inValid\n");
		}
		if(!pTarget->Reserve.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->Reserve:inValid\n");
		}
		if(!pTarget->SLPrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->SLPrice:inValid\n");
		}
		if(!pTarget->SLTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->SLTriggerPrice:inValid\n");
		}
		if(!pTarget->TPPrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TPPrice:inValid\n");
		}
		if(!pTarget->TPTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TPTriggerPrice:inValid\n");
		}
		if(!pTarget->RiskBefore.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->RiskBefore:inValid\n");
		}
		if(!pTarget->TriggerOrderType.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TriggerOrderType:inValid\n");
		}
		if(!pTarget->TriggerDetail.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TriggerDetail:inValid\n");
		}
		if(!pTarget->TriggerPriceType.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TriggerPriceType:inValid\n");
		}
		if(!pTarget->TriggerValue.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TriggerValue:inValid\n");
		}
		if(!pTarget->CloseSLPrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CloseSLPrice:inValid\n");
		}
		if(!pTarget->CloseSLTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CloseSLTriggerPrice:inValid\n");
		}
		if(!pTarget->CloseTPPrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CloseTPPrice:inValid\n");
		}
		if(!pTarget->CloseTPTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CloseTPTriggerPrice:inValid\n");
		}
		if(!pTarget->CloseOrderPriceType.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CloseOrderPriceType:inValid\n");
		}
		if(!pTarget->ClosePrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->ClosePrice:inValid\n");
		}
		if(!pTarget->CloseTriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CloseTriggerPrice:inValid\n");
		}
		if(!pTarget->RelatedOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->RelatedOrderID:inValid\n");
		}
		if(!pTarget->ActiveTime.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->ActiveTime:inValid\n");
		}
		if(!pTarget->TriggerTime.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TriggerTime:inValid\n");
		}
		if(!pTarget->TimeSortNo.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TimeSortNo:inValid\n");
		}
		if(!pTarget->TriggerStatus.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->TriggerStatus:inValid\n");
		}
		if(!pTarget->PosiDirection.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->PosiDirection:inValid\n");
		}
		if(!pTarget->FrontNo.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->FrontNo:inValid\n");
		}
		if(!pTarget->ErrorNo.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->ErrorNo:inValid\n");
		}
		if(!pTarget->ErrorMsg.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->ErrorMsg:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->InsertTime:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->UpdateTime:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->BusinessNo:inValid\n");
		}
		if(!pTarget->CreateTime.isValid()){
			bValid = false;
			fprintf(pFile, "TriggerOrder->CreateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CTriggerOrderFactory::linkAllOrder()
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkOrder();
		pTarget=getNext();
	}
}

void CTriggerOrderFactory::linkAllMember()
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkMember();
		pTarget=getNext();
	}
}

void CTriggerOrderFactory::linkAllInstrument()
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkInstrument();
		pTarget=getNext();
	}
}

void CTriggerOrderFactory::linkAllMarketData()
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL) {
		pTarget->linkMarketData();
		pTarget=getNext();
	}
}

void CTriggerOrderFactory::checkLinkOrder(FILE *pFile)
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getOrder() == NULL){
			if (pTarget->RelatedOrderID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "TriggerOrder->Order:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CTriggerOrderFactory::checkLinkMember(FILE *pFile)
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getMember() == NULL){
			if (pTarget->MemberID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "TriggerOrder->Member:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CTriggerOrderFactory::checkLinkInstrument(FILE *pFile)
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getInstrument() == NULL){
			if (pTarget->ExchangeID.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->InstrumentID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "TriggerOrder->Instrument:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CTriggerOrderFactory::checkLinkMarketData(FILE *pFile)
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getMarketData() == NULL){
			if (pTarget->ExchangeID.isNull()){
				pTarget = getNext();
				continue;
			}
			if (pTarget->InstrumentID.isNull()){
				pTarget = getNext();
				continue;
			}
			fprintf(pFile, "TriggerOrder->MarketData:LinkToNull\n");
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CTriggerOrderFactory::clearLinkOrder()
{
	CTriggerOrder *pTarget = getFirst();
	while (pTarget != NULL){
		if (pTarget->getOrder() == NULL){
			CWriteableTriggerOrder theTarget;
			retrieve(pTarget, &theTarget);
			theTarget.RelatedOrderID.clear();	
			update(pTarget, &theTarget);
		}
		pTarget = getNext();
	}
}

void CTriggerOrderFactory::clearLinkMember()
{
}

void CTriggerOrderFactory::clearLinkInstrument()
{
}

void CTriggerOrderFactory::clearLinkMarketData()
{
}

extern int compareForBuyMBLIndexinMarketOrder(const void *pV1, const void *pV2);
extern int compareForSellMBLIndexinMarketOrder(const void *pV1, const void *pV2);
void CMarketOrderFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pBuyMBLIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 'p':
	{
		if(!reuse) {
			pBuyMBLIndex=new CAVLTree(maxUnit,compareForBuyMBLIndexinMarketOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("MarketOrder_BuyMBLIndex",pBuyMBLIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("MarketOrder_BuyMBLIndex");
			if(it != pIndexMap->end()) {
				pBuyMBLIndex=new CAVLTree(maxUnit,compareForBuyMBLIndexinMarketOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pBuyMBLIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pSellMBLIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case '0':
	case 'p':
	{
		if(!reuse) {
			pSellMBLIndex=new CAVLTree(maxUnit,compareForSellMBLIndexinMarketOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("MarketOrder_SellMBLIndex",pSellMBLIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("MarketOrder_SellMBLIndex");
			if(it != pIndexMap->end()) {
				pSellMBLIndex=new CAVLTree(maxUnit,compareForSellMBLIndexinMarketOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pSellMBLIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pPriceHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 'p':
	{
		int hashKeySize=100000;
		if(!reuse) {
			pPriceHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("MarketOrder_PriceHashIndex",pPriceHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("MarketOrder_PriceHashIndex");
			if(it != pIndexMap->end()) {
				pPriceHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pPriceHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pLastFoundInSearchByBuyMBLData=NULL;
	pLastFoundInSearchBySellMBLData=NULL;
	pActionTriggers=new vector<CMarketOrderActionTrigger *>;
	pCommitTriggers=new vector<CMarketOrderCommitTrigger *>;
}

CMarketOrderFactory::CMarketOrderFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CMarketOrder),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CMarketOrderFactory::CMarketOrderFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CMarketOrder),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CMarketOrderFactory::~CMarketOrderFactory()
{
	if (pBuyMBLIndex!=NULL)
		delete pBuyMBLIndex;
	if (pSellMBLIndex!=NULL)
		delete pSellMBLIndex;
	if (pPriceHashIndex!=NULL)
		delete pPriceHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CMarketOrderFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableMarketOrder thisMarketOrder;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisMarketOrder.readCSV(input,pNames))
		add(&thisMarketOrder);
	fclose(input);
	delete pNames;
	return 1;
}

int CMarketOrderFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "MarketOrder.csv");
	return readCSV(szFileName);
}

int CMarketOrderFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableMarketOrder *pMarketOrder;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableMarketOrder::writeCSVHead(output);
	pMarketOrder=(CWriteableMarketOrder *)(pMem->getFirst());
	while (pMarketOrder!=NULL) {
		pMarketOrder->writeCSV(output);
		pMarketOrder=(CWriteableMarketOrder *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CMarketOrderFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableMarketOrder *pMarketOrder;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableMarketOrder::writeCSVHeadPreVersion(output);
	pMarketOrder=(CWriteableMarketOrder *)(pMem->getFirst());
	while (pMarketOrder!=NULL) {
		pMarketOrder->writeCSVPreVersion(output);
		pMarketOrder=(CWriteableMarketOrder *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CMarketOrderFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "MarketOrder.csv");
	return writeCSV(szFileName);
}

void CMarketOrderFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CMarketOrderFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableMarketOrder *pMarketOrder;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CMarketOrderFactory={       Total Count=%d\n", pMem->getCount());
	pMarketOrder=(CWriteableMarketOrder *)(pMem->getFirst());
	while (pMarketOrder!=NULL) {
		pMarketOrder->dump(fp,index++);
		pMarketOrder=(CWriteableMarketOrder *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CMarketOrderFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pBuyMBLIndex != NULL)
		pBuyMBLIndex->removeAll();
	if(pSellMBLIndex != NULL)
		pSellMBLIndex->removeAll();
	if(pPriceHashIndex != NULL)
		pPriceHashIndex->removeAll();
}

CMarketOrder *CMarketOrderFactory::internalAdd(CWriteableMarketOrder *pMarketOrder, bool bNoTransaction)
{
	pMarketOrder->m_pMDB = m_pMDB;
	CMarketOrder *pTarget;
	//beforeAdd(pMarketOrder);
	pTarget=(CMarketOrder *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough MarketOrder in memory database");
		return NULL;
	}
	forceCopy(pTarget, pMarketOrder, sizeof(CMarketOrder));
	pMem->updateObject(pTarget);
	if (pTarget->Direction == D_Buy){
		if (pBuyMBLIndex != NULL)
			pBuyMBLIndex->addObject(pTarget);
	}
	if (pTarget->Direction == D_Sell){
		if (pSellMBLIndex != NULL)
			pSellMBLIndex->addObject(pTarget);
	}
	if (pPriceHashIndex != NULL)
		pPriceHashIndex->addObject(pTarget,pTarget->HashPriceHashKey);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CMarketOrder* CMarketOrderFactory::add(CWriteableMarketOrder *pMarketOrder, CTransaction *pTransaction)
{
	pMarketOrder->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pMarketOrder,true);
	}else{
		CMarketOrder *pNewMarketOrder;
		pNewMarketOrder = internalAdd(pMarketOrder,false);
		pTransaction->addResource(CMarketOrderResource::alloc(CREATE_ACTION,this,pNewMarketOrder));
		return pNewMarketOrder;
	}
}

CMarketOrder* CMarketOrderFactory::internalUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pNewMarketOrder,bool bNoTransaction)
{
	CWriteableMarketOrder theOldMarketOrder;
	if (bNoTransaction)
		forceCopy(&theOldMarketOrder,pMarketOrder,sizeof(CMarketOrder));	
	CMarketOrder *pTarget = pMarketOrder;
	forceCopy(pMarketOrder,pNewMarketOrder,sizeof(CMarketOrder));
	pMem->updateObject(pMarketOrder);
	if (bNoTransaction)
		commitUpdate(pMarketOrder,&theOldMarketOrder);
	return pMarketOrder;
}

CMarketOrder* CMarketOrderFactory::update(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pNewMarketOrder, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pMarketOrder == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pMarketOrder,pNewMarketOrder,bNoTransaction);
	}else{
		pTransaction->addResource(CMarketOrderResource::alloc(UPDATE_ACTION,this,pMarketOrder));
		return internalUpdate(pMarketOrder,pNewMarketOrder,false);
	}
}

void CMarketOrderFactory::internalRemove(CMarketOrder *pMarketOrder, bool bNoTransaction)
{
	CWriteableMarketOrder theOldMarketOrder;
	//beforeRemove(pMarketOrder);
	if (bNoTransaction)
		forceCopy(&theOldMarketOrder,pMarketOrder,sizeof(CMarketOrder));
	CMarketOrder *pTarget = pMarketOrder;
	if (pTarget->Direction == D_Buy){
		if (pBuyMBLIndex != NULL)
			pBuyMBLIndex->removeObject(pMarketOrder);
	}
	if (pTarget->Direction == D_Sell){
		if (pSellMBLIndex != NULL)
			pSellMBLIndex->removeObject(pMarketOrder);
	}
	
	if (pPriceHashIndex != NULL)
		pPriceHashIndex->removeObject(pMarketOrder,pMarketOrder->HashPriceHashKey);		
	pMem->free(pMarketOrder);
	if(bNoTransaction)
		commitRemove(&theOldMarketOrder);
}

void CMarketOrderFactory::remove(CMarketOrder *pMarketOrder, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pMarketOrder == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pMarketOrder, bNoTransaction);
	}else{
		pTransaction->addResource(CMarketOrderResource::alloc(DELETE_ACTION,this,pMarketOrder));
		internalRemove(pMarketOrder,false);
	}
}

CMarketOrder* CMarketOrderFactory::addOrUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pNewMarketOrder, CTransaction *pTransaction)
{
	if(pMarketOrder == NULL)
		return add(pNewMarketOrder,pTransaction);
	else
		return update(pMarketOrder,pNewMarketOrder,pTransaction);
}

void CMarketOrderFactory::retrieve(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pTargetMarketOrder)
{
	if(pMarketOrder == NULL)
		return;
	forceCopy(pTargetMarketOrder, pMarketOrder, sizeof(CMarketOrder));
}
	
int CMarketOrderFactory::addActionTrigger(CMarketOrderActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CMarketOrderFactory::removeActionTrigger(CMarketOrderActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CMarketOrderFactory::addCommitTrigger(CMarketOrderCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CMarketOrderFactory::removeCommitTrigger(CMarketOrderCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CMarketOrder* CMarketOrderFactory::getFirst()
{
	return (CMarketOrder *)(pMem->getFirst());
}
	
CMarketOrder* CMarketOrderFactory::getNext()
{
	return (CMarketOrder *)(pMem->getNext());
}

void CMarketOrderFactory::commitAdd(CMarketOrder *pMarketOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pMarketOrder);
	}
}

void CMarketOrderFactory::commitUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pOldMarketOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pMarketOrder,pOldMarketOrder);
	}
}
	
void CMarketOrderFactory::commitRemove(CWriteableMarketOrder *pMarketOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pMarketOrder);
	}
}

void CMarketOrderFactory::checkNull(FILE *pFile)
{
	CMarketOrder *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "MarketOrder->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "MarketOrder->InstrumentID:NULL\n");
		}
		if(pTarget->Direction.isNull()){
			bNull = true;
			fprintf(pFile, "MarketOrder->Direction:NULL\n");
		}
		if(pTarget->Price.isNull()){
			bNull = true;
			fprintf(pFile, "MarketOrder->Price:NULL\n");
		}
		if(pTarget->Volume.isNull()){
			bNull = true;
			fprintf(pFile, "MarketOrder->Volume:NULL\n");
		}
		if(pTarget->Orders.isNull()){
			bNull = true;
			fprintf(pFile, "MarketOrder->Orders:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CMarketOrderFactory::checkValid(FILE *pFile)
{
	CMarketOrder *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "MarketOrder->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "MarketOrder->InstrumentID:inValid\n");
		}
		if(!pTarget->Direction.isValid()){
			bValid = false;
			fprintf(pFile, "MarketOrder->Direction:inValid\n");
		}
		if(!pTarget->Price.isValid()){
			bValid = false;
			fprintf(pFile, "MarketOrder->Price:inValid\n");
		}
		if(!pTarget->Volume.isValid()){
			bValid = false;
			fprintf(pFile, "MarketOrder->Volume:inValid\n");
		}
		if(!pTarget->Orders.isValid()){
			bValid = false;
			fprintf(pFile, "MarketOrder->Orders:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "MarketOrder->BusinessNo:inValid\n");
		}
		if(!pTarget->TrueVolume.isValid()){
			bValid = false;
			fprintf(pFile, "MarketOrder->TrueVolume:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

extern int compareForBuyMBLIndexinTickMarketOrder(const void *pV1, const void *pV2);
extern int compareForSellMBLIndexinTickMarketOrder(const void *pV1, const void *pV2);
void CTickMarketOrderFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pBuyMBLIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 'p':
	{
		if(!reuse) {
			pBuyMBLIndex=new CAVLTree(maxUnit,compareForBuyMBLIndexinTickMarketOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TickMarketOrder_BuyMBLIndex",pBuyMBLIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("TickMarketOrder_BuyMBLIndex");
			if(it != pIndexMap->end()) {
				pBuyMBLIndex=new CAVLTree(maxUnit,compareForBuyMBLIndexinTickMarketOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pBuyMBLIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pSellMBLIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 'p':
	{
		if(!reuse) {
			pSellMBLIndex=new CAVLTree(maxUnit,compareForSellMBLIndexinTickMarketOrder,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TickMarketOrder_SellMBLIndex",pSellMBLIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("TickMarketOrder_SellMBLIndex");
			if(it != pIndexMap->end()) {
				pSellMBLIndex=new CAVLTree(maxUnit,compareForSellMBLIndexinTickMarketOrder,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pSellMBLIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pPriceHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case 's':
	case 'p':
	{
		int hashKeySize=100000;
		if(!reuse) {
			pPriceHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("TickMarketOrder_PriceHashIndex",pPriceHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("TickMarketOrder_PriceHashIndex");
			if(it != pIndexMap->end()) {
				pPriceHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pPriceHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pLastFoundInSearchByBuyMBLData=NULL;
	pLastFoundInSearchBySellMBLData=NULL;
	pActionTriggers=new vector<CTickMarketOrderActionTrigger *>;
	pCommitTriggers=new vector<CTickMarketOrderCommitTrigger *>;
}

CTickMarketOrderFactory::CTickMarketOrderFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CTickMarketOrder),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CTickMarketOrderFactory::CTickMarketOrderFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CTickMarketOrder),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CTickMarketOrderFactory::~CTickMarketOrderFactory()
{
	if (pBuyMBLIndex!=NULL)
		delete pBuyMBLIndex;
	if (pSellMBLIndex!=NULL)
		delete pSellMBLIndex;
	if (pPriceHashIndex!=NULL)
		delete pPriceHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CTickMarketOrderFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableTickMarketOrder thisTickMarketOrder;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisTickMarketOrder.readCSV(input,pNames))
		add(&thisTickMarketOrder);
	fclose(input);
	delete pNames;
	return 1;
}

int CTickMarketOrderFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "TickMarketOrder.csv");
	return readCSV(szFileName);
}

int CTickMarketOrderFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableTickMarketOrder *pTickMarketOrder;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableTickMarketOrder::writeCSVHead(output);
	pTickMarketOrder=(CWriteableTickMarketOrder *)(pMem->getFirst());
	while (pTickMarketOrder!=NULL) {
		pTickMarketOrder->writeCSV(output);
		pTickMarketOrder=(CWriteableTickMarketOrder *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CTickMarketOrderFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableTickMarketOrder *pTickMarketOrder;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableTickMarketOrder::writeCSVHeadPreVersion(output);
	pTickMarketOrder=(CWriteableTickMarketOrder *)(pMem->getFirst());
	while (pTickMarketOrder!=NULL) {
		pTickMarketOrder->writeCSVPreVersion(output);
		pTickMarketOrder=(CWriteableTickMarketOrder *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CTickMarketOrderFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "TickMarketOrder.csv");
	return writeCSV(szFileName);
}

void CTickMarketOrderFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CTickMarketOrderFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableTickMarketOrder *pTickMarketOrder;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CTickMarketOrderFactory={       Total Count=%d\n", pMem->getCount());
	pTickMarketOrder=(CWriteableTickMarketOrder *)(pMem->getFirst());
	while (pTickMarketOrder!=NULL) {
		pTickMarketOrder->dump(fp,index++);
		pTickMarketOrder=(CWriteableTickMarketOrder *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CTickMarketOrderFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pBuyMBLIndex != NULL)
		pBuyMBLIndex->removeAll();
	if(pSellMBLIndex != NULL)
		pSellMBLIndex->removeAll();
	if(pPriceHashIndex != NULL)
		pPriceHashIndex->removeAll();
}

CTickMarketOrder *CTickMarketOrderFactory::internalAdd(CWriteableTickMarketOrder *pTickMarketOrder, bool bNoTransaction)
{
	pTickMarketOrder->m_pMDB = m_pMDB;
	CTickMarketOrder *pTarget;
	//beforeAdd(pTickMarketOrder);
	pTarget=(CTickMarketOrder *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough TickMarketOrder in memory database");
		return NULL;
	}
	forceCopy(pTarget, pTickMarketOrder, sizeof(CTickMarketOrder));
	pMem->updateObject(pTarget);
	if (pTarget->Direction == D_Buy){
		if (pBuyMBLIndex != NULL)
			pBuyMBLIndex->addObject(pTarget);
	}
	if (pTarget->Direction == D_Sell){
		if (pSellMBLIndex != NULL)
			pSellMBLIndex->addObject(pTarget);
	}
	if (pPriceHashIndex != NULL)
		pPriceHashIndex->addObject(pTarget,pTarget->HashPriceHashKey);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CTickMarketOrder* CTickMarketOrderFactory::add(CWriteableTickMarketOrder *pTickMarketOrder, CTransaction *pTransaction)
{
	pTickMarketOrder->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pTickMarketOrder,true);
	}else{
		CTickMarketOrder *pNewTickMarketOrder;
		pNewTickMarketOrder = internalAdd(pTickMarketOrder,false);
		pTransaction->addResource(CTickMarketOrderResource::alloc(CREATE_ACTION,this,pNewTickMarketOrder));
		return pNewTickMarketOrder;
	}
}

CTickMarketOrder* CTickMarketOrderFactory::internalUpdate(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pNewTickMarketOrder,bool bNoTransaction)
{
	CWriteableTickMarketOrder theOldTickMarketOrder;
	if (bNoTransaction)
		forceCopy(&theOldTickMarketOrder,pTickMarketOrder,sizeof(CTickMarketOrder));	
	CTickMarketOrder *pTarget = pTickMarketOrder;
	forceCopy(pTickMarketOrder,pNewTickMarketOrder,sizeof(CTickMarketOrder));
	pMem->updateObject(pTickMarketOrder);
	if (bNoTransaction)
		commitUpdate(pTickMarketOrder,&theOldTickMarketOrder);
	return pTickMarketOrder;
}

CTickMarketOrder* CTickMarketOrderFactory::update(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pNewTickMarketOrder, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pTickMarketOrder == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pTickMarketOrder,pNewTickMarketOrder,bNoTransaction);
	}else{
		pTransaction->addResource(CTickMarketOrderResource::alloc(UPDATE_ACTION,this,pTickMarketOrder));
		return internalUpdate(pTickMarketOrder,pNewTickMarketOrder,false);
	}
}

void CTickMarketOrderFactory::internalRemove(CTickMarketOrder *pTickMarketOrder, bool bNoTransaction)
{
	CWriteableTickMarketOrder theOldTickMarketOrder;
	//beforeRemove(pTickMarketOrder);
	if (bNoTransaction)
		forceCopy(&theOldTickMarketOrder,pTickMarketOrder,sizeof(CTickMarketOrder));
	CTickMarketOrder *pTarget = pTickMarketOrder;
	if (pTarget->Direction == D_Buy){
		if (pBuyMBLIndex != NULL)
			pBuyMBLIndex->removeObject(pTickMarketOrder);
	}
	if (pTarget->Direction == D_Sell){
		if (pSellMBLIndex != NULL)
			pSellMBLIndex->removeObject(pTickMarketOrder);
	}
	
	if (pPriceHashIndex != NULL)
		pPriceHashIndex->removeObject(pTickMarketOrder,pTickMarketOrder->HashPriceHashKey);		
	pMem->free(pTickMarketOrder);
	if(bNoTransaction)
		commitRemove(&theOldTickMarketOrder);
}

void CTickMarketOrderFactory::remove(CTickMarketOrder *pTickMarketOrder, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pTickMarketOrder == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pTickMarketOrder, bNoTransaction);
	}else{
		pTransaction->addResource(CTickMarketOrderResource::alloc(DELETE_ACTION,this,pTickMarketOrder));
		internalRemove(pTickMarketOrder,false);
	}
}

CTickMarketOrder* CTickMarketOrderFactory::addOrUpdate(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pNewTickMarketOrder, CTransaction *pTransaction)
{
	if(pTickMarketOrder == NULL)
		return add(pNewTickMarketOrder,pTransaction);
	else
		return update(pTickMarketOrder,pNewTickMarketOrder,pTransaction);
}

void CTickMarketOrderFactory::retrieve(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pTargetTickMarketOrder)
{
	if(pTickMarketOrder == NULL)
		return;
	forceCopy(pTargetTickMarketOrder, pTickMarketOrder, sizeof(CTickMarketOrder));
}
	
int CTickMarketOrderFactory::addActionTrigger(CTickMarketOrderActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CTickMarketOrderFactory::removeActionTrigger(CTickMarketOrderActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CTickMarketOrderFactory::addCommitTrigger(CTickMarketOrderCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CTickMarketOrderFactory::removeCommitTrigger(CTickMarketOrderCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CTickMarketOrder* CTickMarketOrderFactory::getFirst()
{
	return (CTickMarketOrder *)(pMem->getFirst());
}
	
CTickMarketOrder* CTickMarketOrderFactory::getNext()
{
	return (CTickMarketOrder *)(pMem->getNext());
}

void CTickMarketOrderFactory::commitAdd(CTickMarketOrder *pTickMarketOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pTickMarketOrder);
	}
}

void CTickMarketOrderFactory::commitUpdate(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pOldTickMarketOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pTickMarketOrder,pOldTickMarketOrder);
	}
}
	
void CTickMarketOrderFactory::commitRemove(CWriteableTickMarketOrder *pTickMarketOrder)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pTickMarketOrder);
	}
}

void CTickMarketOrderFactory::checkNull(FILE *pFile)
{
	CTickMarketOrder *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "TickMarketOrder->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "TickMarketOrder->InstrumentID:NULL\n");
		}
		if(pTarget->Direction.isNull()){
			bNull = true;
			fprintf(pFile, "TickMarketOrder->Direction:NULL\n");
		}
		if(pTarget->Price.isNull()){
			bNull = true;
			fprintf(pFile, "TickMarketOrder->Price:NULL\n");
		}
		if(pTarget->Volume.isNull()){
			bNull = true;
			fprintf(pFile, "TickMarketOrder->Volume:NULL\n");
		}
		if(pTarget->Orders.isNull()){
			bNull = true;
			fprintf(pFile, "TickMarketOrder->Orders:NULL\n");
		}
		if(pTarget->Tick.isNull()){
			bNull = true;
			fprintf(pFile, "TickMarketOrder->Tick:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CTickMarketOrderFactory::checkValid(FILE *pFile)
{
	CTickMarketOrder *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "TickMarketOrder->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "TickMarketOrder->InstrumentID:inValid\n");
		}
		if(!pTarget->Direction.isValid()){
			bValid = false;
			fprintf(pFile, "TickMarketOrder->Direction:inValid\n");
		}
		if(!pTarget->Price.isValid()){
			bValid = false;
			fprintf(pFile, "TickMarketOrder->Price:inValid\n");
		}
		if(!pTarget->Volume.isValid()){
			bValid = false;
			fprintf(pFile, "TickMarketOrder->Volume:inValid\n");
		}
		if(!pTarget->Orders.isValid()){
			bValid = false;
			fprintf(pFile, "TickMarketOrder->Orders:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "TickMarketOrder->BusinessNo:inValid\n");
		}
		if(!pTarget->TrueVolume.isValid()){
			bValid = false;
			fprintf(pFile, "TickMarketOrder->TrueVolume:inValid\n");
		}
		if(!pTarget->Tick.isValid()){
			bValid = false;
			fprintf(pFile, "TickMarketOrder->Tick:inValid\n");
		}
		if(!pTarget->SubIndex.isValid()){
			bValid = false;
			fprintf(pFile, "TickMarketOrder->SubIndex:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CTradeFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pActionTriggers=new vector<CTradeActionTrigger *>;
	pCommitTriggers=new vector<CTradeCommitTrigger *>;
}

CTradeFactory::CTradeFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CTrade),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CTradeFactory::CTradeFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CTrade),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CTradeFactory::~CTradeFactory()
{
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CTradeFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableTrade thisTrade;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisTrade.readCSV(input,pNames))
		add(&thisTrade);
	fclose(input);
	delete pNames;
	return 1;
}

int CTradeFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Trade.csv");
	return readCSV(szFileName);
}

int CTradeFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableTrade *pTrade;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableTrade::writeCSVHead(output);
	pTrade=(CWriteableTrade *)(pMem->getFirst());
	while (pTrade!=NULL) {
		pTrade->writeCSV(output);
		pTrade=(CWriteableTrade *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CTradeFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableTrade *pTrade;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableTrade::writeCSVHeadPreVersion(output);
	pTrade=(CWriteableTrade *)(pMem->getFirst());
	while (pTrade!=NULL) {
		pTrade->writeCSVPreVersion(output);
		pTrade=(CWriteableTrade *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CTradeFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "Trade.csv");
	return writeCSV(szFileName);
}

void CTradeFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CTradeFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableTrade *pTrade;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CTradeFactory={       Total Count=%d\n", pMem->getCount());
	pTrade=(CWriteableTrade *)(pMem->getFirst());
	while (pTrade!=NULL) {
		pTrade->dump(fp,index++);
		pTrade=(CWriteableTrade *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CTradeFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
}

CTrade *CTradeFactory::internalAdd(CWriteableTrade *pTrade, bool bNoTransaction)
{
	pTrade->m_pMDB = m_pMDB;
	return NULL;
}

CTrade* CTradeFactory::add(CWriteableTrade *pTrade, CTransaction *pTransaction)
{
	pTrade->InsertTime = g_nCurrentTime;
	if (pTransaction==NULL) {
		commitAdd(pTrade);
	}else {
		pTransaction->addResource(CTradeResource::alloc(CREATE_ACTION,this,pTrade));
	}
	return NULL;
}

CTrade* CTradeFactory::internalUpdate(CTrade *pTrade, CWriteableTrade *pNewTrade,bool bNoTransaction)
{
	return NULL;
}

CTrade* CTradeFactory::update(CTrade *pTrade, CWriteableTrade *pNewTrade, CTransaction *pTransaction, bool bNoTransaction)
{
	return NULL;
}

void CTradeFactory::internalRemove(CTrade *pTrade, bool bNoTransaction)
{
}

void CTradeFactory::remove(CTrade *pTrade, CTransaction *pTransaction, bool bNoTransaction)
{
}

CTrade* CTradeFactory::addOrUpdate(CTrade *pTrade, CWriteableTrade *pNewTrade, CTransaction *pTransaction)
{
	return NULL;
}

void CTradeFactory::retrieve(CTrade *pTrade, CWriteableTrade *pTargetTrade)
{
}
	
int CTradeFactory::addActionTrigger(CTradeActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CTradeFactory::removeActionTrigger(CTradeActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CTradeFactory::addCommitTrigger(CTradeCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CTradeFactory::removeCommitTrigger(CTradeCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CTrade* CTradeFactory::getFirst()
{
	return (CTrade *)(pMem->getFirst());
}
	
CTrade* CTradeFactory::getNext()
{
	return (CTrade *)(pMem->getNext());
}

void CTradeFactory::commitAdd(CTrade *pTrade)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pTrade);
	}
}

void CTradeFactory::commitUpdate(CTrade *pTrade, CWriteableTrade *pOldTrade)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pTrade,pOldTrade);
	}
}
	
void CTradeFactory::commitRemove(CWriteableTrade *pTrade)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pTrade);
	}
}

void CTradeFactory::checkNull(FILE *pFile)
{
	CTrade *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->TradeID.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->TradeID:NULL\n");
		}
		if(pTarget->Direction.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->Direction:NULL\n");
		}
		if(pTarget->MemberID.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->MemberID:NULL\n");
		}
		if(pTarget->AccountID.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->AccountID:NULL\n");
		}
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->InstrumentID:NULL\n");
		}
		if(pTarget->OffsetFlag.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->OffsetFlag:NULL\n");
		}
		if(pTarget->Price.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->Price:NULL\n");
		}
		if(pTarget->Volume.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->Volume:NULL\n");
		}
		if(pTarget->DeriveSource.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->DeriveSource:NULL\n");
		}
		if(pTarget->MatchRole.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->MatchRole:NULL\n");
		}
		if(pTarget->Fee.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->Fee:NULL\n");
		}
		if(pTarget->CloseProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->CloseProfit:NULL\n");
		}
		if(pTarget->Turnover.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->Turnover:NULL\n");
		}
		if(pTarget->UseMargin.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->UseMargin:NULL\n");
		}
		if(pTarget->OrderPrice.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->OrderPrice:NULL\n");
		}
		if(pTarget->IsSelfTrade.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->IsSelfTrade:NULL\n");
		}
		if(pTarget->CopyProfit.isNull()){
			bNull = true;
			fprintf(pFile, "Trade->CopyProfit:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CTradeFactory::checkValid(FILE *pFile)
{
	CTrade *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->TradeID.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->TradeID:inValid\n");
		}
		if(!pTarget->Direction.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->Direction:inValid\n");
		}
		if(!pTarget->OrderID.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->OrderID:inValid\n");
		}
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->MemberID:inValid\n");
		}
		if(!pTarget->PositionID.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->PositionID:inValid\n");
		}
		if(!pTarget->AccountID.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->AccountID:inValid\n");
		}
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->InstrumentID:inValid\n");
		}
		if(!pTarget->OffsetFlag.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->OffsetFlag:inValid\n");
		}
		if(!pTarget->Price.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->Price:inValid\n");
		}
		if(!pTarget->Volume.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->Volume:inValid\n");
		}
		if(!pTarget->DeriveSource.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->DeriveSource:inValid\n");
		}
		if(!pTarget->MatchRole.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->MatchRole:inValid\n");
		}
		if(!pTarget->PriceCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->PriceCurrency:inValid\n");
		}
		if(!pTarget->ClearCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->ClearCurrency:inValid\n");
		}
		if(!pTarget->Fee.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->Fee:inValid\n");
		}
		if(!pTarget->FeeCurrency.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->FeeCurrency:inValid\n");
		}
		if(!pTarget->CloseProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->CloseProfit:inValid\n");
		}
		if(!pTarget->Turnover.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->Turnover:inValid\n");
		}
		if(!pTarget->UseMargin.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->UseMargin:inValid\n");
		}
		if(!pTarget->Leverage.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->Leverage:inValid\n");
		}
		if(!pTarget->OrderPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->OrderPrice:inValid\n");
		}
		if(!pTarget->TriggerPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->TriggerPrice:inValid\n");
		}
		if(!pTarget->IsSelfTrade.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->IsSelfTrade:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->Remark:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->BusinessNo:inValid\n");
		}
		if(!pTarget->OpenPrice.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->OpenPrice:inValid\n");
		}
		if(!pTarget->APPID.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->APPID:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->InsertTime:inValid\n");
		}
		if(!pTarget->CreateTime.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->CreateTime:inValid\n");
		}
		if(!pTarget->CopyOrderID.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->CopyOrderID:inValid\n");
		}
		if(!pTarget->CopyMemberID.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->CopyMemberID:inValid\n");
		}
		if(!pTarget->CopyProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->CopyProfit:inValid\n");
		}
		if(!pTarget->Position.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->Position:inValid\n");
		}
		if(!pTarget->ReserveProfit.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->ReserveProfit:inValid\n");
		}
		if(!pTarget->ReserveFee.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->ReserveFee:inValid\n");
		}
		if(!pTarget->TradeRemark.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->TradeRemark:inValid\n");
		}
		if(!pTarget->BusinessType.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->BusinessType:inValid\n");
		}
		if(!pTarget->BusinessValue.isValid()){
			bValid = false;
			fprintf(pFile, "Trade->BusinessValue:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CKLineFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pActionTriggers=new vector<CKLineActionTrigger *>;
	pCommitTriggers=new vector<CKLineCommitTrigger *>;
}

CKLineFactory::CKLineFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CKLine),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CKLineFactory::CKLineFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CKLine),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CKLineFactory::~CKLineFactory()
{
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CKLineFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableKLine thisKLine;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisKLine.readCSV(input,pNames))
		add(&thisKLine);
	fclose(input);
	delete pNames;
	return 1;
}

int CKLineFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "KLine.csv");
	return readCSV(szFileName);
}

int CKLineFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableKLine *pKLine;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableKLine::writeCSVHead(output);
	pKLine=(CWriteableKLine *)(pMem->getFirst());
	while (pKLine!=NULL) {
		pKLine->writeCSV(output);
		pKLine=(CWriteableKLine *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CKLineFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableKLine *pKLine;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableKLine::writeCSVHeadPreVersion(output);
	pKLine=(CWriteableKLine *)(pMem->getFirst());
	while (pKLine!=NULL) {
		pKLine->writeCSVPreVersion(output);
		pKLine=(CWriteableKLine *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CKLineFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "KLine.csv");
	return writeCSV(szFileName);
}

void CKLineFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CKLineFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableKLine *pKLine;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CKLineFactory={       Total Count=%d\n", pMem->getCount());
	pKLine=(CWriteableKLine *)(pMem->getFirst());
	while (pKLine!=NULL) {
		pKLine->dump(fp,index++);
		pKLine=(CWriteableKLine *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CKLineFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
}

CKLine *CKLineFactory::internalAdd(CWriteableKLine *pKLine, bool bNoTransaction)
{
	pKLine->m_pMDB = m_pMDB;
	return NULL;
}

CKLine* CKLineFactory::add(CWriteableKLine *pKLine, CTransaction *pTransaction)
{
	pKLine->InsertTime = g_nCurrentTime;
	if (pTransaction==NULL) {
		commitAdd(pKLine);
	}else {
		pTransaction->addResource(CKLineResource::alloc(CREATE_ACTION,this,pKLine));
	}
	return NULL;
}

CKLine* CKLineFactory::internalUpdate(CKLine *pKLine, CWriteableKLine *pNewKLine,bool bNoTransaction)
{
	return NULL;
}

CKLine* CKLineFactory::update(CKLine *pKLine, CWriteableKLine *pNewKLine, CTransaction *pTransaction, bool bNoTransaction)
{
	return NULL;
}

void CKLineFactory::internalRemove(CKLine *pKLine, bool bNoTransaction)
{
}

void CKLineFactory::remove(CKLine *pKLine, CTransaction *pTransaction, bool bNoTransaction)
{
}

CKLine* CKLineFactory::addOrUpdate(CKLine *pKLine, CWriteableKLine *pNewKLine, CTransaction *pTransaction)
{
	return NULL;
}

void CKLineFactory::retrieve(CKLine *pKLine, CWriteableKLine *pTargetKLine)
{
}
	
int CKLineFactory::addActionTrigger(CKLineActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CKLineFactory::removeActionTrigger(CKLineActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CKLineFactory::addCommitTrigger(CKLineCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CKLineFactory::removeCommitTrigger(CKLineCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CKLine* CKLineFactory::getFirst()
{
	return (CKLine *)(pMem->getFirst());
}
	
CKLine* CKLineFactory::getNext()
{
	return (CKLine *)(pMem->getNext());
}

void CKLineFactory::commitAdd(CKLine *pKLine)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pKLine);
	}
}

void CKLineFactory::commitUpdate(CKLine *pKLine, CWriteableKLine *pOldKLine)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pKLine,pOldKLine);
	}
}
	
void CKLineFactory::commitRemove(CWriteableKLine *pKLine)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pKLine);
	}
}

void CKLineFactory::checkNull(FILE *pFile)
{
	CKLine *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "KLine->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "KLine->InstrumentID:NULL\n");
		}
		if(pTarget->Bar.isNull()){
			bNull = true;
			fprintf(pFile, "KLine->Bar:NULL\n");
		}
		if(pTarget->Volume.isNull()){
			bNull = true;
			fprintf(pFile, "KLine->Volume:NULL\n");
		}
		if(pTarget->Turnover.isNull()){
			bNull = true;
			fprintf(pFile, "KLine->Turnover:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CKLineFactory::checkValid(FILE *pFile)
{
	CKLine *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "KLine->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "KLine->InstrumentID:inValid\n");
		}
		if(!pTarget->Bar.isValid()){
			bValid = false;
			fprintf(pFile, "KLine->Bar:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "KLine->InsertTime:inValid\n");
		}
		if(!pTarget->OpenPrice.isValid()){
			bValid = false;
			fprintf(pFile, "KLine->OpenPrice:inValid\n");
		}
		if(!pTarget->HighestPrice.isValid()){
			bValid = false;
			fprintf(pFile, "KLine->HighestPrice:inValid\n");
		}
		if(!pTarget->LowestPrice.isValid()){
			bValid = false;
			fprintf(pFile, "KLine->LowestPrice:inValid\n");
		}
		if(!pTarget->ClosePrice.isValid()){
			bValid = false;
			fprintf(pFile, "KLine->ClosePrice:inValid\n");
		}
		if(!pTarget->Volume.isValid()){
			bValid = false;
			fprintf(pFile, "KLine->Volume:inValid\n");
		}
		if(!pTarget->Turnover.isValid()){
			bValid = false;
			fprintf(pFile, "KLine->Turnover:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

extern int compareForBarInsertTimeIndexinPublishKLine(const void *pV1, const void *pV2);
void CPublishKLineFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pBarInsertTimeIndex = NULL;
	switch (g_cMDBRunLevel)
	{
	case 'p':
	{
		if(!reuse) {
			pBarInsertTimeIndex=new CAVLTree(maxUnit,compareForBarInsertTimeIndexinPublishKLine,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("PublishKLine_BarInsertTimeIndex",pBarInsertTimeIndex->getReuseID()));
		}else{
			map<string,int>::iterator it = pIndexMap->find("PublishKLine_BarInsertTimeIndex");
			if(it != pIndexMap->end()) {
				pBarInsertTimeIndex=new CAVLTree(maxUnit,compareForBarInsertTimeIndexinPublishKLine,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pBarInsertTimeIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");		
		break;
	}	
	}	
	
	pLastFoundInSearchByBar=NULL;
	pLastFoundInSearchByBarInsertTime=NULL;
	pActionTriggers=new vector<CPublishKLineActionTrigger *>;
	pCommitTriggers=new vector<CPublishKLineCommitTrigger *>;
}

CPublishKLineFactory::CPublishKLineFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CPublishKLine),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CPublishKLineFactory::CPublishKLineFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CPublishKLine),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CPublishKLineFactory::~CPublishKLineFactory()
{
	if (pBarInsertTimeIndex!=NULL)
		delete pBarInsertTimeIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CPublishKLineFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteablePublishKLine thisPublishKLine;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisPublishKLine.readCSV(input,pNames))
		add(&thisPublishKLine);
	fclose(input);
	delete pNames;
	return 1;
}

int CPublishKLineFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "PublishKLine.csv");
	return readCSV(szFileName);
}

int CPublishKLineFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteablePublishKLine *pPublishKLine;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteablePublishKLine::writeCSVHead(output);
	pPublishKLine=(CWriteablePublishKLine *)(pMem->getFirst());
	while (pPublishKLine!=NULL) {
		pPublishKLine->writeCSV(output);
		pPublishKLine=(CWriteablePublishKLine *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CPublishKLineFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteablePublishKLine *pPublishKLine;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteablePublishKLine::writeCSVHeadPreVersion(output);
	pPublishKLine=(CWriteablePublishKLine *)(pMem->getFirst());
	while (pPublishKLine!=NULL) {
		pPublishKLine->writeCSVPreVersion(output);
		pPublishKLine=(CWriteablePublishKLine *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CPublishKLineFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "PublishKLine.csv");
	return writeCSV(szFileName);
}

void CPublishKLineFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CPublishKLineFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteablePublishKLine *pPublishKLine;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CPublishKLineFactory={       Total Count=%d\n", pMem->getCount());
	pPublishKLine=(CWriteablePublishKLine *)(pMem->getFirst());
	while (pPublishKLine!=NULL) {
		pPublishKLine->dump(fp,index++);
		pPublishKLine=(CWriteablePublishKLine *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CPublishKLineFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pBarInsertTimeIndex != NULL)
		pBarInsertTimeIndex->removeAll();
}

CPublishKLine *CPublishKLineFactory::internalAdd(CWriteablePublishKLine *pPublishKLine, bool bNoTransaction)
{
	pPublishKLine->m_pMDB = m_pMDB;
	if(pPublishKLine->InsertTime.isNull())
		pPublishKLine->InsertTime = g_nCurrentTime;
	CPublishKLine *pTarget;
	//beforeAdd(pPublishKLine);
	pTarget=(CPublishKLine *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough PublishKLine in memory database");
		return NULL;
	}
	forceCopy(pTarget, pPublishKLine, sizeof(CPublishKLine));
	pMem->updateObject(pTarget);
	if (pBarInsertTimeIndex != NULL)
		pBarInsertTimeIndex->addObject(pTarget);
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CPublishKLine* CPublishKLineFactory::add(CWriteablePublishKLine *pPublishKLine, CTransaction *pTransaction)
{
	pPublishKLine->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pPublishKLine,true);
	}else{
		CPublishKLine *pNewPublishKLine;
		pNewPublishKLine = internalAdd(pPublishKLine,false);
		pTransaction->addResource(CPublishKLineResource::alloc(CREATE_ACTION,this,pNewPublishKLine));
		return pNewPublishKLine;
	}
}

CPublishKLine* CPublishKLineFactory::internalUpdate(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pNewPublishKLine,bool bNoTransaction)
{
	CWriteablePublishKLine theOldPublishKLine;
	if (bNoTransaction)
		forceCopy(&theOldPublishKLine,pPublishKLine,sizeof(CPublishKLine));	
	CPublishKLine *pTarget = pPublishKLine;
	forceCopy(pPublishKLine,pNewPublishKLine,sizeof(CPublishKLine));
	pMem->updateObject(pPublishKLine);
	if (bNoTransaction)
		commitUpdate(pPublishKLine,&theOldPublishKLine);
	return pPublishKLine;
}

CPublishKLine* CPublishKLineFactory::update(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pNewPublishKLine, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pPublishKLine == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pPublishKLine,pNewPublishKLine,bNoTransaction);
	}else{
		pTransaction->addResource(CPublishKLineResource::alloc(UPDATE_ACTION,this,pPublishKLine));
		return internalUpdate(pPublishKLine,pNewPublishKLine,false);
	}
}

void CPublishKLineFactory::internalRemove(CPublishKLine *pPublishKLine, bool bNoTransaction)
{
	CWriteablePublishKLine theOldPublishKLine;
	//beforeRemove(pPublishKLine);
	if (bNoTransaction)
		forceCopy(&theOldPublishKLine,pPublishKLine,sizeof(CPublishKLine));
	CPublishKLine *pTarget = pPublishKLine;
	if (pBarInsertTimeIndex != NULL)
		pBarInsertTimeIndex->removeObject(pPublishKLine);	
	
	pMem->free(pPublishKLine);
	if(bNoTransaction)
		commitRemove(&theOldPublishKLine);
}

void CPublishKLineFactory::remove(CPublishKLine *pPublishKLine, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pPublishKLine == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pPublishKLine, bNoTransaction);
	}else{
		pTransaction->addResource(CPublishKLineResource::alloc(DELETE_ACTION,this,pPublishKLine));
		internalRemove(pPublishKLine,false);
	}
}

CPublishKLine* CPublishKLineFactory::addOrUpdate(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pNewPublishKLine, CTransaction *pTransaction)
{
	if(pPublishKLine == NULL)
		return add(pNewPublishKLine,pTransaction);
	else
		return update(pPublishKLine,pNewPublishKLine,pTransaction);
}

void CPublishKLineFactory::retrieve(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pTargetPublishKLine)
{
	if(pPublishKLine == NULL)
		return;
	forceCopy(pTargetPublishKLine, pPublishKLine, sizeof(CPublishKLine));
}
	
int CPublishKLineFactory::addActionTrigger(CPublishKLineActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CPublishKLineFactory::removeActionTrigger(CPublishKLineActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CPublishKLineFactory::addCommitTrigger(CPublishKLineCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CPublishKLineFactory::removeCommitTrigger(CPublishKLineCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CPublishKLine* CPublishKLineFactory::getFirst()
{
	return (CPublishKLine *)(pMem->getFirst());
}
	
CPublishKLine* CPublishKLineFactory::getNext()
{
	return (CPublishKLine *)(pMem->getNext());
}

void CPublishKLineFactory::commitAdd(CPublishKLine *pPublishKLine)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pPublishKLine);
	}
}

void CPublishKLineFactory::commitUpdate(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pOldPublishKLine)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pPublishKLine,pOldPublishKLine);
	}
}
	
void CPublishKLineFactory::commitRemove(CWriteablePublishKLine *pPublishKLine)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pPublishKLine);
	}
}

void CPublishKLineFactory::checkNull(FILE *pFile)
{
	CPublishKLine *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "PublishKLine->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "PublishKLine->InstrumentID:NULL\n");
		}
		if(pTarget->Bar.isNull()){
			bNull = true;
			fprintf(pFile, "PublishKLine->Bar:NULL\n");
		}
		if(pTarget->Volume.isNull()){
			bNull = true;
			fprintf(pFile, "PublishKLine->Volume:NULL\n");
		}
		if(pTarget->Turnover.isNull()){
			bNull = true;
			fprintf(pFile, "PublishKLine->Turnover:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CPublishKLineFactory::checkValid(FILE *pFile)
{
	CPublishKLine *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "PublishKLine->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "PublishKLine->InstrumentID:inValid\n");
		}
		if(!pTarget->Bar.isValid()){
			bValid = false;
			fprintf(pFile, "PublishKLine->Bar:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "PublishKLine->InsertTime:inValid\n");
		}
		if(!pTarget->OpenPrice.isValid()){
			bValid = false;
			fprintf(pFile, "PublishKLine->OpenPrice:inValid\n");
		}
		if(!pTarget->HighestPrice.isValid()){
			bValid = false;
			fprintf(pFile, "PublishKLine->HighestPrice:inValid\n");
		}
		if(!pTarget->LowestPrice.isValid()){
			bValid = false;
			fprintf(pFile, "PublishKLine->LowestPrice:inValid\n");
		}
		if(!pTarget->ClosePrice.isValid()){
			bValid = false;
			fprintf(pFile, "PublishKLine->ClosePrice:inValid\n");
		}
		if(!pTarget->Volume.isValid()){
			bValid = false;
			fprintf(pFile, "PublishKLine->Volume:inValid\n");
		}
		if(!pTarget->Turnover.isValid()){
			bValid = false;
			fprintf(pFile, "PublishKLine->Turnover:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CLastKLineFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pInstrumentBarHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	case 'p':
	{
		int hashKeySize=10240;
		if(!reuse) {
			pInstrumentBarHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("LastKLine_InstrumentBarHashIndex",pInstrumentBarHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("LastKLine_InstrumentBarHashIndex");
			if(it != pIndexMap->end()) {
				pInstrumentBarHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pInstrumentBarHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pActionTriggers=new vector<CLastKLineActionTrigger *>;
	pCommitTriggers=new vector<CLastKLineCommitTrigger *>;
}

CLastKLineFactory::CLastKLineFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CLastKLine),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CLastKLineFactory::CLastKLineFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CLastKLine),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CLastKLineFactory::~CLastKLineFactory()
{
	if (pInstrumentBarHashIndex!=NULL)
		delete pInstrumentBarHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CLastKLineFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableLastKLine thisLastKLine;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisLastKLine.readCSV(input,pNames))
		add(&thisLastKLine);
	fclose(input);
	delete pNames;
	return 1;
}

int CLastKLineFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "LastKLine.csv");
	return readCSV(szFileName);
}

int CLastKLineFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableLastKLine *pLastKLine;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableLastKLine::writeCSVHead(output);
	pLastKLine=(CWriteableLastKLine *)(pMem->getFirst());
	while (pLastKLine!=NULL) {
		pLastKLine->writeCSV(output);
		pLastKLine=(CWriteableLastKLine *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CLastKLineFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableLastKLine *pLastKLine;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableLastKLine::writeCSVHeadPreVersion(output);
	pLastKLine=(CWriteableLastKLine *)(pMem->getFirst());
	while (pLastKLine!=NULL) {
		pLastKLine->writeCSVPreVersion(output);
		pLastKLine=(CWriteableLastKLine *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CLastKLineFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "LastKLine.csv");
	return writeCSV(szFileName);
}

void CLastKLineFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CLastKLineFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableLastKLine *pLastKLine;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CLastKLineFactory={       Total Count=%d\n", pMem->getCount());
	pLastKLine=(CWriteableLastKLine *)(pMem->getFirst());
	while (pLastKLine!=NULL) {
		pLastKLine->dump(fp,index++);
		pLastKLine=(CWriteableLastKLine *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CLastKLineFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pInstrumentBarHashIndex != NULL)
		pInstrumentBarHashIndex->removeAll();
}

CLastKLine *CLastKLineFactory::internalAdd(CWriteableLastKLine *pLastKLine, bool bNoTransaction)
{
	pLastKLine->m_pMDB = m_pMDB;
	if(pLastKLine->InsertTime.isNull())
		pLastKLine->InsertTime = g_nCurrentTime;
	CLastKLine *pTarget;
	//beforeAdd(pLastKLine);
	pTarget=(CLastKLine *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough LastKLine in memory database");
		return NULL;
	}
	forceCopy(pTarget, pLastKLine, sizeof(CLastKLine));
	pMem->updateObject(pTarget);
	if (pInstrumentBarHashIndex != NULL)
		pInstrumentBarHashIndex->addObject(pTarget,pTarget->HashInstrumentBarHashKey);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CLastKLine* CLastKLineFactory::add(CWriteableLastKLine *pLastKLine, CTransaction *pTransaction)
{
	pLastKLine->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pLastKLine,true);
	}else{
		CLastKLine *pNewLastKLine;
		pNewLastKLine = internalAdd(pLastKLine,false);
		pTransaction->addResource(CLastKLineResource::alloc(CREATE_ACTION,this,pNewLastKLine));
		return pNewLastKLine;
	}
}

CLastKLine* CLastKLineFactory::internalUpdate(CLastKLine *pLastKLine, CWriteableLastKLine *pNewLastKLine,bool bNoTransaction)
{
	pNewLastKLine->UpdateTime = g_nCurrentTime;
	CWriteableLastKLine theOldLastKLine;
	if (bNoTransaction)
		forceCopy(&theOldLastKLine,pLastKLine,sizeof(CLastKLine));	
	CLastKLine *pTarget = pLastKLine;
	forceCopy(pLastKLine,pNewLastKLine,sizeof(CLastKLine));
	pMem->updateObject(pLastKLine);
	if (bNoTransaction)
		commitUpdate(pLastKLine,&theOldLastKLine);
	return pLastKLine;
}

CLastKLine* CLastKLineFactory::update(CLastKLine *pLastKLine, CWriteableLastKLine *pNewLastKLine, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pLastKLine == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pLastKLine,pNewLastKLine,bNoTransaction);
	}else{
		pTransaction->addResource(CLastKLineResource::alloc(UPDATE_ACTION,this,pLastKLine));
		return internalUpdate(pLastKLine,pNewLastKLine,false);
	}
}

void CLastKLineFactory::internalRemove(CLastKLine *pLastKLine, bool bNoTransaction)
{
	CWriteableLastKLine theOldLastKLine;
	//beforeRemove(pLastKLine);
	if (bNoTransaction)
		forceCopy(&theOldLastKLine,pLastKLine,sizeof(CLastKLine));
	CLastKLine *pTarget = pLastKLine;
	
	if (pInstrumentBarHashIndex != NULL)
		pInstrumentBarHashIndex->removeObject(pLastKLine,pLastKLine->HashInstrumentBarHashKey);		
	pMem->free(pLastKLine);
	if(bNoTransaction)
		commitRemove(&theOldLastKLine);
}

void CLastKLineFactory::remove(CLastKLine *pLastKLine, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pLastKLine == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pLastKLine, bNoTransaction);
	}else{
		pTransaction->addResource(CLastKLineResource::alloc(DELETE_ACTION,this,pLastKLine));
		internalRemove(pLastKLine,false);
	}
}

CLastKLine* CLastKLineFactory::addOrUpdate(CLastKLine *pLastKLine, CWriteableLastKLine *pNewLastKLine, CTransaction *pTransaction)
{
	if(pLastKLine == NULL)
		return add(pNewLastKLine,pTransaction);
	else
		return update(pLastKLine,pNewLastKLine,pTransaction);
}

void CLastKLineFactory::retrieve(CLastKLine *pLastKLine, CWriteableLastKLine *pTargetLastKLine)
{
	if(pLastKLine == NULL)
		return;
	forceCopy(pTargetLastKLine, pLastKLine, sizeof(CLastKLine));
}
	
int CLastKLineFactory::addActionTrigger(CLastKLineActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CLastKLineFactory::removeActionTrigger(CLastKLineActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CLastKLineFactory::addCommitTrigger(CLastKLineCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CLastKLineFactory::removeCommitTrigger(CLastKLineCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CLastKLine* CLastKLineFactory::getFirst()
{
	return (CLastKLine *)(pMem->getFirst());
}
	
CLastKLine* CLastKLineFactory::getNext()
{
	return (CLastKLine *)(pMem->getNext());
}

void CLastKLineFactory::commitAdd(CLastKLine *pLastKLine)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pLastKLine);
	}
}

void CLastKLineFactory::commitUpdate(CLastKLine *pLastKLine, CWriteableLastKLine *pOldLastKLine)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pLastKLine,pOldLastKLine);
	}
}
	
void CLastKLineFactory::commitRemove(CWriteableLastKLine *pLastKLine)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pLastKLine);
	}
}

void CLastKLineFactory::checkNull(FILE *pFile)
{
	CLastKLine *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->ExchangeID.isNull()){
			bNull = true;
			fprintf(pFile, "LastKLine->ExchangeID:NULL\n");
		}
		if(pTarget->InstrumentID.isNull()){
			bNull = true;
			fprintf(pFile, "LastKLine->InstrumentID:NULL\n");
		}
		if(pTarget->Bar.isNull()){
			bNull = true;
			fprintf(pFile, "LastKLine->Bar:NULL\n");
		}
		if(pTarget->Volume.isNull()){
			bNull = true;
			fprintf(pFile, "LastKLine->Volume:NULL\n");
		}
		if(pTarget->Turnover.isNull()){
			bNull = true;
			fprintf(pFile, "LastKLine->Turnover:NULL\n");
		}
		if(pTarget->UpdateTime.isNull()){
			bNull = true;
			fprintf(pFile, "LastKLine->UpdateTime:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CLastKLineFactory::checkValid(FILE *pFile)
{
	CLastKLine *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->InstrumentID:inValid\n");
		}
		if(!pTarget->Bar.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->Bar:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->InsertTime:inValid\n");
		}
		if(!pTarget->OpenPrice.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->OpenPrice:inValid\n");
		}
		if(!pTarget->HighestPrice.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->HighestPrice:inValid\n");
		}
		if(!pTarget->LowestPrice.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->LowestPrice:inValid\n");
		}
		if(!pTarget->ClosePrice.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->ClosePrice:inValid\n");
		}
		if(!pTarget->Volume.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->Volume:inValid\n");
		}
		if(!pTarget->Turnover.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->Turnover:inValid\n");
		}
		if(!pTarget->UpdateTime.isValid()){
			bValid = false;
			fprintf(pFile, "LastKLine->UpdateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CAccountDetailFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pActionTriggers=new vector<CAccountDetailActionTrigger *>;
	pCommitTriggers=new vector<CAccountDetailCommitTrigger *>;
}

CAccountDetailFactory::CAccountDetailFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CAccountDetail),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CAccountDetailFactory::CAccountDetailFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CAccountDetail),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CAccountDetailFactory::~CAccountDetailFactory()
{
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CAccountDetailFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableAccountDetail thisAccountDetail;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisAccountDetail.readCSV(input,pNames))
		add(&thisAccountDetail);
	fclose(input);
	delete pNames;
	return 1;
}

int CAccountDetailFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "AccountDetail.csv");
	return readCSV(szFileName);
}

int CAccountDetailFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableAccountDetail *pAccountDetail;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableAccountDetail::writeCSVHead(output);
	pAccountDetail=(CWriteableAccountDetail *)(pMem->getFirst());
	while (pAccountDetail!=NULL) {
		pAccountDetail->writeCSV(output);
		pAccountDetail=(CWriteableAccountDetail *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CAccountDetailFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableAccountDetail *pAccountDetail;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableAccountDetail::writeCSVHeadPreVersion(output);
	pAccountDetail=(CWriteableAccountDetail *)(pMem->getFirst());
	while (pAccountDetail!=NULL) {
		pAccountDetail->writeCSVPreVersion(output);
		pAccountDetail=(CWriteableAccountDetail *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CAccountDetailFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "AccountDetail.csv");
	return writeCSV(szFileName);
}

void CAccountDetailFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CAccountDetailFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableAccountDetail *pAccountDetail;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CAccountDetailFactory={       Total Count=%d\n", pMem->getCount());
	pAccountDetail=(CWriteableAccountDetail *)(pMem->getFirst());
	while (pAccountDetail!=NULL) {
		pAccountDetail->dump(fp,index++);
		pAccountDetail=(CWriteableAccountDetail *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CAccountDetailFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
}

CAccountDetail *CAccountDetailFactory::internalAdd(CWriteableAccountDetail *pAccountDetail, bool bNoTransaction)
{
	pAccountDetail->m_pMDB = m_pMDB;
	return NULL;
}

CAccountDetail* CAccountDetailFactory::add(CWriteableAccountDetail *pAccountDetail, CTransaction *pTransaction)
{
	pAccountDetail->InsertTime = g_nCurrentTime;
	if (pTransaction==NULL) {
		commitAdd(pAccountDetail);
	}else {
		pTransaction->addResource(CAccountDetailResource::alloc(CREATE_ACTION,this,pAccountDetail));
	}
	return NULL;
}

CAccountDetail* CAccountDetailFactory::internalUpdate(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pNewAccountDetail,bool bNoTransaction)
{
	return NULL;
}

CAccountDetail* CAccountDetailFactory::update(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pNewAccountDetail, CTransaction *pTransaction, bool bNoTransaction)
{
	return NULL;
}

void CAccountDetailFactory::internalRemove(CAccountDetail *pAccountDetail, bool bNoTransaction)
{
}

void CAccountDetailFactory::remove(CAccountDetail *pAccountDetail, CTransaction *pTransaction, bool bNoTransaction)
{
}

CAccountDetail* CAccountDetailFactory::addOrUpdate(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pNewAccountDetail, CTransaction *pTransaction)
{
	return NULL;
}

void CAccountDetailFactory::retrieve(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pTargetAccountDetail)
{
}
	
int CAccountDetailFactory::addActionTrigger(CAccountDetailActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CAccountDetailFactory::removeActionTrigger(CAccountDetailActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CAccountDetailFactory::addCommitTrigger(CAccountDetailCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CAccountDetailFactory::removeCommitTrigger(CAccountDetailCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CAccountDetail* CAccountDetailFactory::getFirst()
{
	return (CAccountDetail *)(pMem->getFirst());
}
	
CAccountDetail* CAccountDetailFactory::getNext()
{
	return (CAccountDetail *)(pMem->getNext());
}

void CAccountDetailFactory::commitAdd(CAccountDetail *pAccountDetail)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pAccountDetail);
	}
}

void CAccountDetailFactory::commitUpdate(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pOldAccountDetail)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pAccountDetail,pOldAccountDetail);
	}
}
	
void CAccountDetailFactory::commitRemove(CWriteableAccountDetail *pAccountDetail)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pAccountDetail);
	}
}

void CAccountDetailFactory::checkNull(FILE *pFile)
{
	CAccountDetail *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->AccountDetailID.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->AccountDetailID:NULL\n");
		}
		if(pTarget->SettlementGroup.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->SettlementGroup:NULL\n");
		}
		if(pTarget->AccountID.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->AccountID:NULL\n");
		}
		if(pTarget->Currency.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->Currency:NULL\n");
		}
		if(pTarget->Amount.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->Amount:NULL\n");
		}
		if(pTarget->PreBalance.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->PreBalance:NULL\n");
		}
		if(pTarget->Balance.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->Balance:NULL\n");
		}
		if(pTarget->ReserveAmount.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->ReserveAmount:NULL\n");
		}
		if(pTarget->ReserveBalance.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->ReserveBalance:NULL\n");
		}
		if(pTarget->Source.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->Source:NULL\n");
		}
		if(pTarget->InsertTime.isNull()){
			bNull = true;
			fprintf(pFile, "AccountDetail->InsertTime:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CAccountDetailFactory::checkValid(FILE *pFile)
{
	CAccountDetail *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->AccountDetailID.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->AccountDetailID:inValid\n");
		}
		if(!pTarget->MemberID.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->MemberID:inValid\n");
		}
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->InstrumentID:inValid\n");
		}
		if(!pTarget->SettlementGroup.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->SettlementGroup:inValid\n");
		}
		if(!pTarget->AccountID.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->AccountID:inValid\n");
		}
		if(!pTarget->Currency.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->Currency:inValid\n");
		}
		if(!pTarget->Amount.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->Amount:inValid\n");
		}
		if(!pTarget->PreBalance.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->PreBalance:inValid\n");
		}
		if(!pTarget->Balance.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->Balance:inValid\n");
		}
		if(!pTarget->ReserveAmount.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->ReserveAmount:inValid\n");
		}
		if(!pTarget->ReserveBalance.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->ReserveBalance:inValid\n");
		}
		if(!pTarget->Source.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->Source:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->Remark:inValid\n");
		}
		if(!pTarget->LocalID.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->LocalID:inValid\n");
		}
		if(!pTarget->SettleSegment.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->SettleSegment:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->BusinessNo:inValid\n");
		}
		if(!pTarget->RelatedID.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->RelatedID:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->InsertTime:inValid\n");
		}
		if(!pTarget->CreateTime.isValid()){
			bValid = false;
			fprintf(pFile, "AccountDetail->CreateTime:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CSettleDetailFactory::init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly)
{
	pInstrumentHashIndex = NULL;

	switch (g_cMDBRunLevel)
	{
	case '0':
	case 's':
	{
		int hashKeySize=10240;
		if(!reuse) {
			pInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,reuseID,readOnly);
			if(pIndexMap != NULL)
				pIndexMap->insert(map<string,int>::value_type("SettleDetail_InstrumentHashIndex",pInstrumentHashIndex->getReuseID()));
		}else {
			map<string,int>::iterator it = pIndexMap->find("SettleDetail_InstrumentHashIndex");
			if(it != pIndexMap->end()) {
				pInstrumentHashIndex=new CHashIndex(hashKeySize,pAllocator,reuse,it->second,readOnly);
			}
		}
		if (pInstrumentHashIndex==NULL)
			EMERGENCY_EXIT("Insufficient memory space");	
	break;
	}
	}	
	
	pActionTriggers=new vector<CSettleDetailActionTrigger *>;
	pCommitTriggers=new vector<CSettleDetailCommitTrigger *>;
}

CSettleDetailFactory::CSettleDetailFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CSettleDetail),maxUnit,pAllocator,reuse,reuseID, readOnly)
{
	m_pMDB=pMDB;
	init(maxUnit,pAllocator,pIndexMap,reuse,reuseID,readOnly);
}

CSettleDetailFactory::CSettleDetailFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly) :
	CDatabaseDataFactory(sizeof(CSettleDetail),MDB_DEFAULT_BLOCK_SIZE,pAllocator,reuse,reuseID,readOnly)
{
	m_pMDB=pMDB;
	init(MDB_DEFAULT_BLOCK_SIZE,pAllocator,pIndexMap,reuse,reuseID,readOnly);		
}

CSettleDetailFactory::~CSettleDetailFactory()
{
	if (pInstrumentHashIndex!=NULL)
		delete pInstrumentHashIndex;
	if (pActionTriggers!=NULL)
		delete pActionTriggers;
	if (pCommitTriggers!=NULL)
		delete pCommitTriggers;
}

int CSettleDetailFactory::readCSV(char *filename)
{
	FILE *input;
	CWriteableSettleDetail thisSettleDetail;
	char buffer[1025];
	vector<char *> *pNames;
	char *token;
	input=mfopen(filename,"rt");
	if (input==NULL)
		return 0;
	pNames=new vector<char *>;
	fgets(buffer,1024,input);
	token=getNextToken(buffer);
	while (token!=NULL) {
		pNames->push_back(token);
		token=getNextToken(NULL);
	}
	while (thisSettleDetail.readCSV(input,pNames))
		add(&thisSettleDetail);
	fclose(input);
	delete pNames;
	return 1;
}

int CSettleDetailFactory::readCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "SettleDetail.csv");
	return readCSV(szFileName);
}

int CSettleDetailFactory::writeCSV(char *filename)
{
	FILE *output;
	CWriteableSettleDetail *pSettleDetail;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableSettleDetail::writeCSVHead(output);
	pSettleDetail=(CWriteableSettleDetail *)(pMem->getFirst());
	while (pSettleDetail!=NULL) {
		pSettleDetail->writeCSV(output);
		pSettleDetail=(CWriteableSettleDetail *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CSettleDetailFactory::writeCSVPreVersion(char *filename)
{
	FILE *output;
	CWriteableSettleDetail *pSettleDetail;
	output=mfopen(filename,"wt");
	if (output==NULL)
		return 0;
	CWriteableSettleDetail::writeCSVHeadPreVersion(output);
	pSettleDetail=(CWriteableSettleDetail *)(pMem->getFirst());
	while (pSettleDetail!=NULL) {
		pSettleDetail->writeCSVPreVersion(output);
		pSettleDetail=(CWriteableSettleDetail *)(pMem->getNext());
	}
	fclose(output);
	return 1;
}

int CSettleDetailFactory::writeCSVByPath(char *pPath)
{
	char szFileName[300];
	strcpy(szFileName, pPath);
	strcat(szFileName, "SettleDetail.csv");
	return writeCSV(szFileName);
}

void CSettleDetailFactory::dump(char *filename)
{
	FILE *fp;
	fp = mfopen(filename,"w+b");
	dump(fp);
	if (fp!=NULL) fclose(fp);
}	

void CSettleDetailFactory::dump(FILE *fp)
{
	int index=0; ///下标
	CWriteableSettleDetail *pSettleDetail;
	if(fp==NULL) fp=stdout;
	pMem->dump(fp);
	fprintf(fp,"CSettleDetailFactory={       Total Count=%d\n", pMem->getCount());
	pSettleDetail=(CWriteableSettleDetail *)(pMem->getFirst());
	while (pSettleDetail!=NULL) {
		pSettleDetail->dump(fp,index++);
		pSettleDetail=(CWriteableSettleDetail *)(pMem->getNext());
	}
	fprintf(fp,"}\n");
}

void CSettleDetailFactory::clearAll()
{
	CDatabaseDataFactory::clearAll();
	if(pInstrumentHashIndex != NULL)
		pInstrumentHashIndex->removeAll();
}

CSettleDetail *CSettleDetailFactory::internalAdd(CWriteableSettleDetail *pSettleDetail, bool bNoTransaction)
{
	pSettleDetail->m_pMDB = m_pMDB;
	if(pSettleDetail->InsertTime.isNull())
		pSettleDetail->InsertTime = g_nCurrentTime;
	CSettleDetail *pTarget;
	//beforeAdd(pSettleDetail);
	pTarget=(CSettleDetail *)createObject();
	if (pTarget==NULL) {
		EMERGENCY_EXIT("Can not alloc enough SettleDetail in memory database");
		return NULL;
	}
	forceCopy(pTarget, pSettleDetail, sizeof(CSettleDetail));
	pMem->updateObject(pTarget);
	if (pInstrumentHashIndex != NULL)
		pInstrumentHashIndex->addObject(pTarget,pTarget->HashInstrumentHashKey);			
	//afterAdd(pTarget);
	if (bNoTransaction)
		commitAdd(pTarget);
	return pTarget;
}

CSettleDetail* CSettleDetailFactory::add(CWriteableSettleDetail *pSettleDetail, CTransaction *pTransaction)
{
	pSettleDetail->calAllHash();
	if (pTransaction==NULL) {
		return internalAdd(pSettleDetail,true);
	}else{
		CSettleDetail *pNewSettleDetail;
		pNewSettleDetail = internalAdd(pSettleDetail,false);
		pTransaction->addResource(CSettleDetailResource::alloc(CREATE_ACTION,this,pNewSettleDetail));
		return pNewSettleDetail;
	}
}

CSettleDetail* CSettleDetailFactory::internalUpdate(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pNewSettleDetail,bool bNoTransaction)
{
	CWriteableSettleDetail theOldSettleDetail;
	if (bNoTransaction)
		forceCopy(&theOldSettleDetail,pSettleDetail,sizeof(CSettleDetail));	
	CSettleDetail *pTarget = pSettleDetail;
	forceCopy(pSettleDetail,pNewSettleDetail,sizeof(CSettleDetail));
	pMem->updateObject(pSettleDetail);
	if (bNoTransaction)
		commitUpdate(pSettleDetail,&theOldSettleDetail);
	return pSettleDetail;
}

CSettleDetail* CSettleDetailFactory::update(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pNewSettleDetail, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pSettleDetail == NULL)
		return NULL;
	if (pTransaction==NULL) {
		return internalUpdate(pSettleDetail,pNewSettleDetail,bNoTransaction);
	}else{
		pTransaction->addResource(CSettleDetailResource::alloc(UPDATE_ACTION,this,pSettleDetail));
		return internalUpdate(pSettleDetail,pNewSettleDetail,false);
	}
}

void CSettleDetailFactory::internalRemove(CSettleDetail *pSettleDetail, bool bNoTransaction)
{
	CWriteableSettleDetail theOldSettleDetail;
	//beforeRemove(pSettleDetail);
	if (bNoTransaction)
		forceCopy(&theOldSettleDetail,pSettleDetail,sizeof(CSettleDetail));
	CSettleDetail *pTarget = pSettleDetail;
	
	if (pInstrumentHashIndex != NULL)
		pInstrumentHashIndex->removeObject(pSettleDetail,pSettleDetail->HashInstrumentHashKey);		
	pMem->free(pSettleDetail);
	if(bNoTransaction)
		commitRemove(&theOldSettleDetail);
}

void CSettleDetailFactory::remove(CSettleDetail *pSettleDetail, CTransaction *pTransaction, bool bNoTransaction)
{
	if(pSettleDetail == NULL)
		return;
	if (pTransaction == NULL) {
		internalRemove(pSettleDetail, bNoTransaction);
	}else{
		pTransaction->addResource(CSettleDetailResource::alloc(DELETE_ACTION,this,pSettleDetail));
		internalRemove(pSettleDetail,false);
	}
}

CSettleDetail* CSettleDetailFactory::addOrUpdate(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pNewSettleDetail, CTransaction *pTransaction)
{
	if(pSettleDetail == NULL)
		return add(pNewSettleDetail,pTransaction);
	else
		return update(pSettleDetail,pNewSettleDetail,pTransaction);
}

void CSettleDetailFactory::retrieve(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pTargetSettleDetail)
{
	if(pSettleDetail == NULL)
		return;
	forceCopy(pTargetSettleDetail, pSettleDetail, sizeof(CSettleDetail));
}
	
int CSettleDetailFactory::addActionTrigger(CSettleDetailActionTrigger *pActionTrigger)
{
	pActionTrigger->setFactory(this);
	pActionTriggers->push_back(pActionTrigger);
	return 1;
}

int CSettleDetailFactory::removeActionTrigger(CSettleDetailActionTrigger *pActionTrigger)
{
	for (int i=0; i < (int)pActionTriggers->size(); i++) {
		if (pActionTriggers->at(i)==pActionTrigger) {
			pActionTriggers->erase(pActionTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

int CSettleDetailFactory::addCommitTrigger(CSettleDetailCommitTrigger *pCommitTrigger)
{
	pCommitTrigger->setFactory(this);
	pCommitTriggers->push_back(pCommitTrigger);
	return 1;
}

int CSettleDetailFactory::removeCommitTrigger(CSettleDetailCommitTrigger *pCommitTrigger)
{
	for (int i=0;i < (int)pCommitTriggers->size(); i++) {
		if (pCommitTriggers->at(i)==pCommitTrigger) {
			pCommitTriggers->erase(pCommitTriggers->begin()+i);
			return 1;
		}
	}
	return 0;
}

CSettleDetail* CSettleDetailFactory::getFirst()
{
	return (CSettleDetail *)(pMem->getFirst());
}
	
CSettleDetail* CSettleDetailFactory::getNext()
{
	return (CSettleDetail *)(pMem->getNext());
}

void CSettleDetailFactory::commitAdd(CSettleDetail *pSettleDetail)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitAdd(pSettleDetail);
	}
}

void CSettleDetailFactory::commitUpdate(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pOldSettleDetail)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitUpdate(pSettleDetail,pOldSettleDetail);
	}
}
	
void CSettleDetailFactory::commitRemove(CWriteableSettleDetail *pSettleDetail)
{
	for (int i = 0; i < (int)pCommitTriggers->size(); i++) {
		pCommitTriggers->at(i)->commitRemove(pSettleDetail);
	}
}

void CSettleDetailFactory::checkNull(FILE *pFile)
{
	CSettleDetail *pTarget = getFirst();
	while (pTarget != NULL)
	{
		bool bNull = false;
		if(pTarget->SettleDetailID.isNull()){
			bNull = true;
			fprintf(pFile, "SettleDetail->SettleDetailID:NULL\n");
		}
		if(pTarget->SettleAction.isNull()){
			bNull = true;
			fprintf(pFile, "SettleDetail->SettleAction:NULL\n");
		}
		if (bNull){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}

void CSettleDetailFactory::checkValid(FILE *pFile)
{
	CSettleDetail *pTarget = getFirst();
	while (pTarget != NULL)	{
		bool bValid = true;
		if(!pTarget->SettleDetailID.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->SettleDetailID:inValid\n");
		}
		if(!pTarget->APPID.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->APPID:inValid\n");
		}
		if(!pTarget->LocalID.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->LocalID:inValid\n");
		}
		if(!pTarget->ExchangeID.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->ExchangeID:inValid\n");
		}
		if(!pTarget->InstrumentID.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->InstrumentID:inValid\n");
		}
		if(!pTarget->SettlementGroup.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->SettlementGroup:inValid\n");
		}
		if(!pTarget->SettleAction.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->SettleAction:inValid\n");
		}
		if(!pTarget->Value.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->Value:inValid\n");
		}
		if(!pTarget->Value1.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->Value1:inValid\n");
		}
		if(!pTarget->Value2.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->Value2:inValid\n");
		}
		if(!pTarget->Value3.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->Value3:inValid\n");
		}
		if(!pTarget->Remark.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->Remark:inValid\n");
		}
		if(!pTarget->FundingRateGroup.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->FundingRateGroup:inValid\n");
		}
		if(!pTarget->SettleSegment.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->SettleSegment:inValid\n");
		}
		if(!pTarget->InsertTime.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->InsertTime:inValid\n");
		}
		if(!pTarget->BusinessNo.isValid()){
			bValid = false;
			fprintf(pFile, "SettleDetail->BusinessNo:inValid\n");
		}
		if (!bValid){
			pTarget->dumpInLine(pFile);
			fflush(pFile);
		}
		pTarget = getNext();
	}
}


