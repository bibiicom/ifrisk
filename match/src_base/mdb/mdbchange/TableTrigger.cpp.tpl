!!function!!
#include <vector>
#include <string>
#include <set>
using namespace std;
set<string> memDBSet;
void addMemDB(string key) {
	memDBSet.insert(key);
}
string getMemDB(string key) {
	set<string>::iterator it = memDBSet.find(key);
	if (it == memDBSet.end())
		return "";
	return *it;
}
!!endfunction!!
!!enter system!!
	!!travel tables!!
		!!if !strcmp(@hasTable,"yes")!!
			!!command addMemDB(@name)!!
		!!endif!!
	!!next!!
!!leave!!
#include "TableTrigger.h"
#include "CopyField.h"
#include "FieldData.h"

!!enter XTP!!
!!travel fields!!
!!if(valid_name("usage"))!!
	!!let tableName=@name!!
	!!let memDBStr=getMemDB(@name).c_str()!!
	!!if strcmp(@memDBStr, "")!!	
//////---------!!@comment!!
CTable!!@tableName!!Trigger::CTable!!@tableName!!Trigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pszObjectName = pszObjectName;
	m_pMDB = pMDB;
	m_pRsp = pResponser;
}

void CTable!!@tableName!!Trigger::commitAdd(C!!@tableName!!* p!!@tableName!!)
{
	if(m_pRsp->PrepareMetaRef(&C!!@tableName!!Field::m_Describe,(void*)p!!@tableName!!, (void*)p!!@tableName!!, OUTFOG_TYPE_INSERT))
		commitDo(p!!@tableName!!);
}

void CTable!!@tableName!!Trigger::commitUpdate(C!!@tableName!!* p!!@tableName!!, CWriteable!!@tableName!!* pOld!!@tableName!!)
{
	if(m_pRsp->PrepareMetaRef(&C!!@tableName!!Field::m_Describe,(void*)p!!@tableName!!, (void*)pOld!!@tableName!!, OUTFOG_TYPE_UPDATE))
		commitDo(p!!@tableName!!);
}

void CTable!!@tableName!!Trigger::commitRemove(CWriteable!!@tableName!!* p!!@tableName!!)
{
	if(m_pRsp->PrepareMetaRef(&C!!@tableName!!Field::m_Describe, (void*)p!!@tableName!!, (void*)p!!@tableName!!, OUTFOG_TYPE_DELETE))
		commitDo(p!!@tableName!!);
}

void CTable!!@tableName!!Trigger::commitDo(C!!@tableName!!* p!!@tableName!!)
{
	Copy!!@tableName!!Entity(&field,p!!@tableName!!);
	!!if valid_name("multi")!!
		!!if !strcmp(get_string("multiType"),"char")!!
	int nDispatchNo = p!!@tableName!!->!!@multiField!!.getValue()[strlen(p!!@tableName!!->!!@multiField!!.getValue())-1] - '0';
	if(nDispatchNo < 0 )
		nDispatchNo = -nDispatchNo;
		!!endif!!
		!!if !strcmp(get_string("multiType"),"int")!!
	int nDispatchNo = p!!@tableName!!->!!@multiField!!.getValue();
		!!endif!!
		!!if !strcmp(get_string("multiType"),"double")!!
	int nDispatchNo = (int)p!!@tableName!!->!!@multiField!!.getValue();
		!!endif!!
	nDispatchNo = abs(nDispatchNo);
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = !!@syncno!! + nDispatchNo % !!@multi!!;
	else
		m_pRsp->GetMetaRef()->RequestNo += nDispatchNo % !!@multi!!;
	!!else!!
		!!if valid_name("syncno")!!
	if(m_pRsp->GetMetaRef()->RequestNo == 0)
		m_pRsp->GetMetaRef()->RequestNo = !!@syncno!!;
		!!endif!!
	!!endif!!
	C!!@tableName!!Field::m_Describe.setSingleField(m_pRsp->GetStreamBuf(),&field);
	m_pRsp->GetCallback()->RspField(m_pRsp->GetMetaRef(),m_pRsp->GetStreamBuf());
}
	!!endif!!
!!endif!!
!!next!!
!!leave!!

CSyncTriggers::CSyncTriggers(CMDB *pMDB,CServiceResponser *pResponser)
{
	m_pMDB = pMDB;

!!enter XTP!!
!!travel fields!!
!!if(valid_name("usage"))!!
	!!let tableName=@name!!
	!!let memDBStr=getMemDB(@name).c_str()!!
	!!if strcmp(@memDBStr, "")!!	
	m_p!!@tableName!!Trigger = new CTable!!@tableName!!Trigger("!!@tableName!!",m_pMDB,pResponser);
	m_pMDB->m_!!@tableName!!Factory->addCommitTrigger(m_p!!@tableName!!Trigger);
	!!endif!!
!!endif!!
!!next!!
!!leave!!
}

CSyncTriggers::~CSyncTriggers()
{
!!enter XTP!!
!!travel fields!!
!!if(valid_name("usage"))!!
	!!let tableName=@name!!
	!!let memDBStr=getMemDB(@name).c_str()!!
	!!if strcmp(@memDBStr, "")!!
	m_pMDB->m_!!@tableName!!Factory->removeCommitTrigger(m_p!!@tableName!!Trigger);
	delete m_p!!@tableName!!Trigger;
	!!endif!!
!!endif!!
!!next!!
!!leave!!	
}