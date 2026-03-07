!!function!!
#include <vector>
#include <string>
#include <map>
using namespace std;

map<string, string> enumMap;

void addEnum(string key, string value) {
	enumMap.insert(pair<string, string>(key, value));
}

string getEnumStr(string key) {
	map<string, string>::iterator it = enumMap.find(key);
	if (it == enumMap.end())
		return "";
	return it->second;
}

!!endfunction!!
!!include common.function!!
!!enter UFDataTypes!!
	!!travel self!!
		!!if !strcmp(@name, "EnumChar")!!
			!!let valueStr=""!!
			!!travel self!!
				!!let valueStr=multiaddstring(5, @valueStr, @value, ":", @label, ";")!!
			!!next!!
			!!command addEnum(@typename, @valueStr)!!
		!!endif!!
	!!next!!
!!leave!!
!!enter XTP!!
	!!travel fields!!
	!!if valid_name("usage") && isDBPackage(@usage)!!
	!!let tableName=@name!!
	
--******************************
-- create @tableName
--******************************
drop table IF EXISTS !!@pre!!!!@tableName!!;
create table !!@pre!!!!@tableName!!
(
	!!travel self!!
	!!if @pumpid!=0!!,!!endif!!!!@name!! !!@type!! !!if valid_name("default")!!default !!@default!! !!endif!!!!if !strcmp(@notnull,"yes")!!not null!!endif!!
	!!next!!
	!!if valid_name("DBPK")!!
	,constraint pk_!!@name!! primary key (!!@DBPK!!)
	!!endif!!
);
comment on table !!@pre!!!!@tableName!! is '!!@comment!!';
!!travel self!!
	!!if valid_name("originaltype")!!
		!!let colcomment=""!!
		!!let enumStr=getEnumStr(@originaltype).c_str()!!
		!!if !strcmp(@enumStr, "")!!
			!!let colcomment=@label!!
		!!else!!
			!!let colcomment=multiaddstring(4, @label, "(", @enumStr, ")")!!
		!!endif!!
		!!if !strcmp(@colcomment, "")!!
			!!let colcomment=@label!!
		!!endif!!
	!!else!!
		!!let colcomment=@label!!
	!!endif!!
comment on column !!@pre!!!!@tableName!!.!!@name!! is '!!@colcomment!!';
!!next!!

	!!endif!!
	!!next!!
!!leave!!
