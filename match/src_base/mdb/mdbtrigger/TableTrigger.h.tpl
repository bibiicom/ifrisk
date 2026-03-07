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
#ifndef _TAABLETRIGGER_H_
#define _TAABLETRIGGER_H_

#include "mdb.h"
#include "ServiceResponser.h"

!!enter XTP!!
!!travel fields!!
!!if(valid_name("usage"))!!
	!!let tableName=@name!!
	!!let memDBStr=getMemDB(@name).c_str()!!
	!!if strcmp(@memDBStr, "")!!	
//////---------!!@comment!!	
class CTable!!@tableName!!Trigger:public C!!@tableName!!CommitTrigger
{
public:
	CTable!!@tableName!!Trigger(const char *pszObjectName,CMDB *pMDB,CServiceResponser *pResponser);
	virtual ~CTable!!@tableName!!Trigger(void){};
	virtual void commitAdd(C!!@tableName!!* p!!@tableName!!);
	virtual void commitUpdate(C!!@tableName!!* p!!@tableName!!, CWriteable!!@tableName!!* pOld!!@tableName!!);
	virtual void commitRemove(CWriteable!!@tableName!!* p!!@tableName!!);
	void commitDo(C!!@tableName!!* p!!@tableName!!);
protected:
	const char *m_pszObjectName;
	CMDB *m_pMDB;
	CServiceResponser *m_pRsp;
};
	!!endif!!
!!endif!!
!!next!!
!!leave!!

class CSyncTriggers
{
public:
	CSyncTriggers(CMDB* pMDB,CServiceResponser *pResponser);
	virtual ~CSyncTriggers();

!!enter XTP!!
!!travel fields!!
!!if(valid_name("usage"))!!
	!!let tableName=@name!!
	!!let memDBStr=getMemDB(@name).c_str()!!
	!!if strcmp(@memDBStr, "")!!	
	CTable!!@tableName!!Trigger *m_p!!@tableName!!Trigger;
	!!endif!!
!!endif!!
!!next!!
!!leave!!

private:
	CMDB *m_pMDB;
};

#endif