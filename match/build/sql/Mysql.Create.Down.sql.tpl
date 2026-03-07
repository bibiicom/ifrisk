!!function!!
#include <vector>
#include <string>
#include <map>
using namespace std;
#include <time.h>

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

string getLocalTime()
{
	time_t now = time(NULL);
	char timeBuffer[100];
	strftime(timeBuffer, 100, "%Y%m%d", localtime(&now));
	return timeBuffer;
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
	!!if valid_name("usage") && !strcmp(@usage,"DownTable")!!
		!!let tableName=@name!!

/******************************
-- create !!@tableName!!
--******************************/
create table !!@pre!!!!@tableName!!
(
	!!let CreateTime="no"!!
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
		!!if !strcmp(@name,"CreateTime")!!
			!!let CreateTime="yes"!!
		!!else!!		
	!!if @pumpid!=0!!,!!endif!!!!@name!! !!@type!! !!if valid_name("default")!!default !!@default!! !!endif!!!!if strstr(@DBPK,@name)!!NOT NULL!!endif!! COMMENT '!!@colcomment!!'
		!!endif!!
	!!next!!
	!!if !strcmp(@CreateTime,"yes")!!
	,CreateTime datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '本地创建时间'
	!!endif!!	
	,DBTime datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '落库时间'
	!!if valid_name("DBPK")!!
	,PRIMARY KEY (!!@DBPK!!)
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

