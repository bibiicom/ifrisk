!!function!!
#include <vector>
#include <string>
#include <map>
using namespace std;

map<string, string> enumMap;
map<string, string> defaultMap;

void addEnum(string key, string value) {
	enumMap.insert(pair<string, string>(key, value));
}

void addDefault(string key, string value) {
	defaultMap.insert(pair<string, string>(key, value));
}

string getEnumStr(string key) {
	map<string, string>::iterator it = enumMap.find(key);
	if (it == enumMap.end())
		return "";
	return it->second;
}

string getDefaultStr(string key) {
	map<string, string>::iterator it = defaultMap.find(key);
	if (it == defaultMap.end())
		return "null";
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
				!!if @pumpid==0!!
					!!command addDefault(@typename, @value)!!
				!!endif!!
			!!next!!
			!!command addEnum(@typename, @valueStr)!!
		!!else!!	
			!!if !strcmp(@type, "string")!!
				!!command addDefault(@typename, "''")!!
			!!elseif !strcmp(@type, "int")!!
				!!command addDefault(@typename, "0")!!
			!!elseif !strcmp(@type, "double")!!
				!!command addDefault(@typename, "0.0")!!
			!!endif!!	
		!!endif!!
	!!next!!
!!leave!!
!!enter XTP!!
	!!travel fields!!
	!!if valid_name("usage") && !strcmp(@usage,"InitTable")!!
		!!let tableName=@name!!

/******************************
-- create !!@tableName!!
--******************************/
drop table if exists !!@pre!!!!@tableName!!;
create table !!@pre!!!!@tableName!!
(
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
			!!let defaultStr=getDefaultStr(@originaltype).c_str()!!
		!!else!!
			!!let colcomment=@label!!
			!!let defaultStr="null"!!
		!!endif!!	
	!!if @pumpid!=0!!,!!endif!!!!@name!! !!@type!!!!if !strcmp(@notnull,"yes")!! not null default !!if valid_name("default")!!!!@default!!!!else!!!!@defaultStr!!!!endif!!!!else!! default null!!endif!! COMMENT '!!@colcomment!!'
	!!next!!
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	!!if valid_name("DBPK")!!
	,primary key (!!@DBPK!!)
	!!endif!!
		!!if valid_name("DbKeys")!!
			!!command vector<string> vct = Txtsplit(@DbKeys,";");!!
			!!command for(int i =0; i<vct.size(); i++) {!!
			!!command char skeyname[8];sprintf(skeyname,"%d",i);!!
	,KEY mykey_!!command display_string(skeyname)!! (!!command display_string(vct[i].c_str())!!)
			!!command }!!
			!!endif!!
)ENGINE=InnoDB COMMENT='!!@comment!!';
	!!endif!!
	!!next!!
!!leave!!
