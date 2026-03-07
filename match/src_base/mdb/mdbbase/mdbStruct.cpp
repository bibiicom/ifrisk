/////////////////////////////////////////////////////////////////////////
///@file BaseDataStruct.cpp
///@brief实现了内存数据库用的基本数据结构类，以及iterator、trigger的基类
///@history 
/////////////////////////////////////////////////////////////////////////

#include "mdbStruct.h"
#include "mdbTable.h"

int compare(const char *s1, const char *s2)
{
	int result=strcmp(s1,s2);
	if (result>0)
		return 1;
	else if (result<0)
		return -1;
	return 0;
}

int compare(const int v1,const int v2)
{
	if (v1>v2)
		return 1;
	if (v1<v2)
		return -1;
	return 0;
}

int compare(const double v1,const double v2)
{
	//if (v1>v2+SMALL_DOUBLE)
	//	return 1;
	//if (v1<v2-SMALL_DOUBLE)
	//	return -1;
	//return 0;

	//目前最大是12位小数，这里取12，如果系统的最大的小数变化，这里需要修改
	double compareSmall = doubleCompareSmall(v1, SMALLEST_PRECISION);
	if (v1 > v2 + compareSmall)
		return 1;
	if (v1 < v2 - compareSmall)
		return -1;
	return 0;
}

char *getNextToken(char *buffer)
{
	static char *last;
	char *from, *to;
	
	if (buffer==NULL)
	{
		from=last;
		if (from==NULL)
		{
			return NULL;
		}
	}
	else
	{
		from=buffer;
	}
	
	bool hasQuote=false;
	if (*from=='"')
	{
		from++;
		hasQuote=true;
	}
	to=from;
	while ((*to!='\0')&&(*to!=',')&&(*to!='\n'))
	{
		if (hasQuote && (*to=='"'))
		{
			to++;
			break;
		}
		to++;
	}
	if (*to!=',')
	{
		last=NULL;
	}
	else
	{
		last=to+1;
	}
	if (!hasQuote)
	{
		*to='\0';
	}
	else
	{
		*(to-1)='\0';
	}
	return from;
}


void CWriteableCurrentTime::writeCSVHead(FILE *output)
{
	fprintf(output,"\"SystemID*\",\"ZoneDate\",\"ZoneTime\",\"ZoneDays\",\"CurrTime\",\"InsertTime\",\"UpdateTime\",\"TimeZone\",\"LastTradeID\",\"LastOrderID\",\"LastAccountDetailID\",\"LastTimeSortNo\",\"LastBusinessNo\"\n");
}

void CWriteableCurrentTime::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"SystemID\",\"ZoneDate\",\"ZoneTime\",\"ZoneDays\",\"CurrTime\",\"InsertTime\",\"UpdateTime\",\"TimeZone\",\"LastTradeID\",\"LastOrderID\",\"LastAccountDetailID\",\"LastTimeSortNo\",\"LastBusinessNo\"\n");
}
	
void CWriteableCurrentTime::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	SystemID.init();
	ZoneDate.init();
	ZoneTime.init();
	ZoneDays.init();
	CurrTime.init();
	InsertTime.clear();
	UpdateTime.clear();
	TimeZone.init();
	LastTradeID.clear();
	LastOrderID.clear();
	LastAccountDetailID.clear();
	LastTimeSortNo.clear();
	LastBusinessNo.clear();
}

void CWriteableCurrentTime::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableCurrentTime::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableCurrentTime),input)!=sizeof(CWriteableCurrentTime))
		return 0;
	else
		return 1;
}
	
int CWriteableCurrentTime::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			SystemID=token;
		else
			SystemID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ZoneDate=token;
		else
			ZoneDate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ZoneTime=token;
		else
			ZoneTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ZoneDays=token;
		else
			ZoneDays="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CurrTime=token;
		else
			CurrTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TimeZone=token;
		else
			TimeZone="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LastTradeID=token;
		else
			LastTradeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LastOrderID=token;
		else
			LastOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LastAccountDetailID=token;
		else
			LastAccountDetailID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LastTimeSortNo=token;
		else
			LastTimeSortNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LastBusinessNo=token;
		else
			LastBusinessNo="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"SystemID"))
			{
				SystemID=token;
			}
			else if (!strcmp(fieldName,"ZoneDate"))
			{
				ZoneDate=token;
			}
			else if (!strcmp(fieldName,"ZoneTime"))
			{
				ZoneTime=token;
			}
			else if (!strcmp(fieldName,"ZoneDays"))
			{
				ZoneDays=token;
			}
			else if (!strcmp(fieldName,"CurrTime"))
			{
				CurrTime=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			else if (!strcmp(fieldName,"TimeZone"))
			{
				TimeZone=token;
			}
			else if (!strcmp(fieldName,"LastTradeID"))
			{
				LastTradeID=token;
			}
			else if (!strcmp(fieldName,"LastOrderID"))
			{
				LastOrderID=token;
			}
			else if (!strcmp(fieldName,"LastAccountDetailID"))
			{
				LastAccountDetailID=token;
			}
			else if (!strcmp(fieldName,"LastTimeSortNo"))
			{
				LastTimeSortNo=token;
			}
			else if (!strcmp(fieldName,"LastBusinessNo"))
			{
				LastBusinessNo=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableCurrentTime::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableCurrentTime),output)!=sizeof(CWriteableCurrentTime))
		return 0;
	else
		return 1;
}

int CWriteableCurrentTime::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",SystemID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ZoneDate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ZoneTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ZoneDays.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CurrTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TimeZone.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastTradeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastAccountDetailID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastTimeSortNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastBusinessNo.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableCurrentTime::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",SystemID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ZoneDate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ZoneTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ZoneDays.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CurrTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TimeZone.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastTradeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastAccountDetailID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastTimeSortNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastBusinessNo.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableCurrentTime::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCCurrentTime%s = {\n", index_buf);
	fprintf(fp,"\t\tSystemID = %s    [系统名]\n",SystemID.getString().c_str());	
	fprintf(fp,"\t\tZoneDate = %s    [本地日期]\n",ZoneDate.getString().c_str());	
	fprintf(fp,"\t\tZoneTime = %s    [本地时间]\n",ZoneTime.getString().c_str());	
	fprintf(fp,"\t\tZoneDays = %s    [本地天数]\n",ZoneDays.getString().c_str());	
	fprintf(fp,"\t\tCurrTime = %s    [当前时间（秒）]\n",CurrTime.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间（毫秒）]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t\tTimeZone = %s    [本地时区]\n",TimeZone.getString().c_str());	
	fprintf(fp,"\t\tLastTradeID = %s    [最新最新成交序列号]\n",LastTradeID.getString().c_str());	
	fprintf(fp,"\t\tLastOrderID = %s    [最新订单序列号]\n",LastOrderID.getString().c_str());	
	fprintf(fp,"\t\tLastAccountDetailID = %s    [最新资金明细序列号]\n",LastAccountDetailID.getString().c_str());	
	fprintf(fp,"\t\tLastTimeSortNo = %s    [最新排序序列号]\n",LastTimeSortNo.getString().c_str());	
	fprintf(fp,"\t\tLastBusinessNo = %s    [最新事务序列号]\n",LastBusinessNo.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableCurrentTime::dumpDiff(FILE *fp, const CWriteableCurrentTime *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCCurrentTime = {\n");
	if (compare(SystemID.getValue(),pOriginal->SystemID.getValue())!=0)
		fprintf(fp,"\t\tSystemID = %s -> %s    [系统名]\n",pOriginal->SystemID.getString().c_str(),SystemID.getString().c_str());
	else
		fprintf(fp,"\t\tSystemID = %s    [系统名]\n",SystemID.getString().c_str());
	if (compare(ZoneDate.getValue(),pOriginal->ZoneDate.getValue())!=0)
		fprintf(fp,"\t\tZoneDate = %s -> %s    [本地日期]\n",pOriginal->ZoneDate.getString().c_str(),ZoneDate.getString().c_str());
	else
		fprintf(fp,"\t\tZoneDate = %s    [本地日期]\n",ZoneDate.getString().c_str());
	if (compare(ZoneTime.getValue(),pOriginal->ZoneTime.getValue())!=0)
		fprintf(fp,"\t\tZoneTime = %s -> %s    [本地时间]\n",pOriginal->ZoneTime.getString().c_str(),ZoneTime.getString().c_str());
	else
		fprintf(fp,"\t\tZoneTime = %s    [本地时间]\n",ZoneTime.getString().c_str());
	if (compare(ZoneDays.getValue(),pOriginal->ZoneDays.getValue())!=0)
		fprintf(fp,"\t\tZoneDays = %s -> %s    [本地天数]\n",pOriginal->ZoneDays.getString().c_str(),ZoneDays.getString().c_str());
	else
		fprintf(fp,"\t\tZoneDays = %s    [本地天数]\n",ZoneDays.getString().c_str());
	if (compare(CurrTime.getValue(),pOriginal->CurrTime.getValue())!=0)
		fprintf(fp,"\t\tCurrTime = %s -> %s    [当前时间（秒）]\n",pOriginal->CurrTime.getString().c_str(),CurrTime.getString().c_str());
	else
		fprintf(fp,"\t\tCurrTime = %s    [当前时间（秒）]\n",CurrTime.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间（毫秒）]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间（毫秒）]\n",UpdateTime.getString().c_str());
	if (compare(TimeZone.getValue(),pOriginal->TimeZone.getValue())!=0)
		fprintf(fp,"\t\tTimeZone = %s -> %s    [本地时区]\n",pOriginal->TimeZone.getString().c_str(),TimeZone.getString().c_str());
	else
		fprintf(fp,"\t\tTimeZone = %s    [本地时区]\n",TimeZone.getString().c_str());
	if (compare(LastTradeID.getValue(),pOriginal->LastTradeID.getValue())!=0)
		fprintf(fp,"\t\tLastTradeID = %s -> %s    [最新最新成交序列号]\n",pOriginal->LastTradeID.getString().c_str(),LastTradeID.getString().c_str());
	else
		fprintf(fp,"\t\tLastTradeID = %s    [最新最新成交序列号]\n",LastTradeID.getString().c_str());
	if (compare(LastOrderID.getValue(),pOriginal->LastOrderID.getValue())!=0)
		fprintf(fp,"\t\tLastOrderID = %s -> %s    [最新订单序列号]\n",pOriginal->LastOrderID.getString().c_str(),LastOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tLastOrderID = %s    [最新订单序列号]\n",LastOrderID.getString().c_str());
	if (compare(LastAccountDetailID.getValue(),pOriginal->LastAccountDetailID.getValue())!=0)
		fprintf(fp,"\t\tLastAccountDetailID = %s -> %s    [最新资金明细序列号]\n",pOriginal->LastAccountDetailID.getString().c_str(),LastAccountDetailID.getString().c_str());
	else
		fprintf(fp,"\t\tLastAccountDetailID = %s    [最新资金明细序列号]\n",LastAccountDetailID.getString().c_str());
	if (compare(LastTimeSortNo.getValue(),pOriginal->LastTimeSortNo.getValue())!=0)
		fprintf(fp,"\t\tLastTimeSortNo = %s -> %s    [最新排序序列号]\n",pOriginal->LastTimeSortNo.getString().c_str(),LastTimeSortNo.getString().c_str());
	else
		fprintf(fp,"\t\tLastTimeSortNo = %s    [最新排序序列号]\n",LastTimeSortNo.getString().c_str());
	if (compare(LastBusinessNo.getValue(),pOriginal->LastBusinessNo.getValue())!=0)
		fprintf(fp,"\t\tLastBusinessNo = %s -> %s    [最新事务序列号]\n",pOriginal->LastBusinessNo.getString().c_str(),LastBusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tLastBusinessNo = %s    [最新事务序列号]\n",LastBusinessNo.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableCurrentTime::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",SystemID=%s",SystemID.getString().c_str());	
	fprintf(fp,",ZoneDate=%s",ZoneDate.getString().c_str());	
	fprintf(fp,",ZoneTime=%s",ZoneTime.getString().c_str());	
	fprintf(fp,",ZoneDays=%s",ZoneDays.getString().c_str());	
	fprintf(fp,",CurrTime=%s",CurrTime.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,",TimeZone=%s",TimeZone.getString().c_str());	
	fprintf(fp,",LastTradeID=%s",LastTradeID.getString().c_str());	
	fprintf(fp,",LastOrderID=%s",LastOrderID.getString().c_str());	
	fprintf(fp,",LastAccountDetailID=%s",LastAccountDetailID.getString().c_str());	
	fprintf(fp,",LastTimeSortNo=%s",LastTimeSortNo.getString().c_str());	
	fprintf(fp,",LastBusinessNo=%s",LastBusinessNo.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableCurrentTime::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"SystemID=%s,",SystemID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ZoneDate=%s,",ZoneDate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ZoneTime=%s,",ZoneTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ZoneDays=%s,",ZoneDays.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CurrTime=%s,",CurrTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TimeZone=%s,",TimeZone.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LastTradeID=%s,",LastTradeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LastOrderID=%s,",LastOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LastAccountDetailID=%s,",LastAccountDetailID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LastTimeSortNo=%s,",LastTimeSortNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LastBusinessNo=%s,",LastBusinessNo.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CCurrentTimeIterator::dump(FILE *output)
{
	CCurrentTime *pCurrentTime;
	while ((pCurrentTime=next())!=NULL)
	{
		pCurrentTime->dump(output);
	}
}



void CWriteableMember::writeCSVHead(FILE *output)
{
	fprintf(output,"\"MemberID*\",\"MemberType\",\"Grade\",\"Priority\",\"FeeGrade\",\"MarginRateGrade\",\"TradingRightGrade\",\"CFDGrade\",\"LiquidityGrade\",\"RiskLevel\",\"PositionType\",\"TriggerOrders\",\"OpenOrders\",\"OpenPositions\",\"MaxLocalID\",\"SinkType\",\"CopyMemberID\",\"Region\",\"FeeDeduceDisCount\",\"FeeDeduceCurrency\",\"CanBorrow\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}

void CWriteableMember::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"MemberID\",\"MemberType\",\"Grade\",\"Priority\",\"FeeGrade\",\"MarginRateGrade\",\"TradingRightGrade\",\"CFDGrade\",\"LiquidityGrade\",\"RiskLevel\",\"PositionType\",\"TriggerOrders\",\"OpenOrders\",\"OpenPositions\",\"MaxLocalID\",\"SinkType\",\"CopyMemberID\",\"Region\",\"FeeDeduceDisCount\",\"FeeDeduceCurrency\",\"CanBorrow\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}
	
void CWriteableMember::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	MemberID.init();
	MemberType.clear();
	Grade.clear();
	Priority.clear();
	FeeGrade.clear();
	MarginRateGrade.clear();
	TradingRightGrade.clear();
	CFDGrade.clear();
	LiquidityGrade.clear();
	RiskLevel.init();
	PositionType.clear();
	TriggerOrders.clear();
	OpenOrders.clear();
	OpenPositions.clear();
	MaxLocalID.clear();
	SinkType.clear();
	CopyMemberID.clear();
	Region.clear();
	FeeDeduceDisCount.clear();
	FeeDeduceCurrency.clear();
	CanBorrow.clear();
	Remark.clear();
	InsertTime.clear();
	UpdateTime.clear();
}

void CWriteableMember::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableMember::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableMember),input)!=sizeof(CWriteableMember))
		return 0;
	else
		return 1;
}
	
int CWriteableMember::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MemberType=token;
		else
			MemberType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Grade=token;
		else
			Grade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Priority=token;
		else
			Priority="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FeeGrade=token;
		else
			FeeGrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MarginRateGrade=token;
		else
			MarginRateGrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradingRightGrade=token;
		else
			TradingRightGrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CFDGrade=token;
		else
			CFDGrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LiquidityGrade=token;
		else
			LiquidityGrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			RiskLevel=token;
		else
			RiskLevel="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionType=token;
		else
			PositionType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerOrders=token;
		else
			TriggerOrders="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenOrders=token;
		else
			OpenOrders="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenPositions=token;
		else
			OpenPositions="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MaxLocalID=token;
		else
			MaxLocalID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SinkType=token;
		else
			SinkType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyMemberID=token;
		else
			CopyMemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Region=token;
		else
			Region="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FeeDeduceDisCount=token;
		else
			FeeDeduceDisCount="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FeeDeduceCurrency=token;
		else
			FeeDeduceCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CanBorrow=token;
		else
			CanBorrow="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"MemberType"))
			{
				MemberType=token;
			}
			else if (!strcmp(fieldName,"Grade"))
			{
				Grade=token;
			}
			else if (!strcmp(fieldName,"Priority"))
			{
				Priority=token;
			}
			else if (!strcmp(fieldName,"FeeGrade"))
			{
				FeeGrade=token;
			}
			else if (!strcmp(fieldName,"MarginRateGrade"))
			{
				MarginRateGrade=token;
			}
			else if (!strcmp(fieldName,"TradingRightGrade"))
			{
				TradingRightGrade=token;
			}
			else if (!strcmp(fieldName,"CFDGrade"))
			{
				CFDGrade=token;
			}
			else if (!strcmp(fieldName,"LiquidityGrade"))
			{
				LiquidityGrade=token;
			}
			else if (!strcmp(fieldName,"RiskLevel"))
			{
				RiskLevel=token;
			}
			else if (!strcmp(fieldName,"PositionType"))
			{
				PositionType=token;
			}
			else if (!strcmp(fieldName,"TriggerOrders"))
			{
				TriggerOrders=token;
			}
			else if (!strcmp(fieldName,"OpenOrders"))
			{
				OpenOrders=token;
			}
			else if (!strcmp(fieldName,"OpenPositions"))
			{
				OpenPositions=token;
			}
			else if (!strcmp(fieldName,"MaxLocalID"))
			{
				MaxLocalID=token;
			}
			else if (!strcmp(fieldName,"SinkType"))
			{
				SinkType=token;
			}
			else if (!strcmp(fieldName,"CopyMemberID"))
			{
				CopyMemberID=token;
			}
			else if (!strcmp(fieldName,"Region"))
			{
				Region=token;
			}
			else if (!strcmp(fieldName,"FeeDeduceDisCount"))
			{
				FeeDeduceDisCount=token;
			}
			else if (!strcmp(fieldName,"FeeDeduceCurrency"))
			{
				FeeDeduceCurrency=token;
			}
			else if (!strcmp(fieldName,"CanBorrow"))
			{
				CanBorrow=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableMember::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableMember),output)!=sizeof(CWriteableMember))
		return 0;
	else
		return 1;
}

int CWriteableMember::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Grade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Priority.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeGrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginRateGrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradingRightGrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CFDGrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LiquidityGrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",RiskLevel.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerOrders.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenOrders.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPositions.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxLocalID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SinkType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Region.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeDeduceDisCount.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeDeduceCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CanBorrow.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableMember::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Grade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Priority.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeGrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginRateGrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradingRightGrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CFDGrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LiquidityGrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",RiskLevel.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerOrders.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenOrders.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPositions.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxLocalID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SinkType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Region.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeDeduceDisCount.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeDeduceCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CanBorrow.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableMember::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCMember%s = {\n", index_buf);
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tMemberType = %s    [成员类型]\n",MemberType.getString().c_str());	
	fprintf(fp,"\t\tGrade = %s    [等级]\n",Grade.getString().c_str());	
	fprintf(fp,"\t\tPriority = %s    [优先权]\n",Priority.getString().c_str());	
	fprintf(fp,"\t\tFeeGrade = %s    [手续费等级]\n",FeeGrade.getString().c_str());	
	fprintf(fp,"\t\tMarginRateGrade = %s    [保证金等级]\n",MarginRateGrade.getString().c_str());	
	fprintf(fp,"\t\tTradingRightGrade = %s    [权限等级]\n",TradingRightGrade.getString().c_str());	
	fprintf(fp,"\t\tCFDGrade = %s    [差价等级]\n",CFDGrade.getString().c_str());	
	fprintf(fp,"\t\tLiquidityGrade = %s    [流动性等级]\n",LiquidityGrade.getString().c_str());	
	fprintf(fp,"\t\tRiskLevel = %s    [风控类型]\n",RiskLevel.getString().c_str());	
	fprintf(fp,"\t\tPositionType = %s    [持仓类型]\n",PositionType.getString().c_str());	
	fprintf(fp,"\t\tTriggerOrders = %s    [触发订单个数]\n",TriggerOrders.getString().c_str());	
	fprintf(fp,"\t\tOpenOrders = %s    [未成交订单数]\n",OpenOrders.getString().c_str());	
	fprintf(fp,"\t\tOpenPositions = %s    [用户持仓个数]\n",OpenPositions.getString().c_str());	
	fprintf(fp,"\t\tMaxLocalID = %s    [最大本地标识]\n",MaxLocalID.getString().c_str());	
	fprintf(fp,"\t\tSinkType = %s    [下游系统同步类型]\n",SinkType.getString().c_str());	
	fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());	
	fprintf(fp,"\t\tRegion = %s    []\n",Region.getString().c_str());	
	fprintf(fp,"\t\tFeeDeduceDisCount = %s    [作为手续费的抵扣优惠比例]\n",FeeDeduceDisCount.getString().c_str());	
	fprintf(fp,"\t\tFeeDeduceCurrency = %s    [作为手续费的抵扣比例]\n",FeeDeduceCurrency.getString().c_str());	
	fprintf(fp,"\t\tCanBorrow = %s    [是否需要借钱交易]\n",CanBorrow.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableMember::dumpDiff(FILE *fp, const CWriteableMember *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCMember = {\n");
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(MemberType.getValue(),pOriginal->MemberType.getValue())!=0)
		fprintf(fp,"\t\tMemberType = %s -> %s    [成员类型]\n",pOriginal->MemberType.getString().c_str(),MemberType.getString().c_str());
	else
		fprintf(fp,"\t\tMemberType = %s    [成员类型]\n",MemberType.getString().c_str());
	if (compare(Grade.getValue(),pOriginal->Grade.getValue())!=0)
		fprintf(fp,"\t\tGrade = %s -> %s    [等级]\n",pOriginal->Grade.getString().c_str(),Grade.getString().c_str());
	else
		fprintf(fp,"\t\tGrade = %s    [等级]\n",Grade.getString().c_str());
	if (compare(Priority.getValue(),pOriginal->Priority.getValue())!=0)
		fprintf(fp,"\t\tPriority = %s -> %s    [优先权]\n",pOriginal->Priority.getString().c_str(),Priority.getString().c_str());
	else
		fprintf(fp,"\t\tPriority = %s    [优先权]\n",Priority.getString().c_str());
	if (compare(FeeGrade.getValue(),pOriginal->FeeGrade.getValue())!=0)
		fprintf(fp,"\t\tFeeGrade = %s -> %s    [手续费等级]\n",pOriginal->FeeGrade.getString().c_str(),FeeGrade.getString().c_str());
	else
		fprintf(fp,"\t\tFeeGrade = %s    [手续费等级]\n",FeeGrade.getString().c_str());
	if (compare(MarginRateGrade.getValue(),pOriginal->MarginRateGrade.getValue())!=0)
		fprintf(fp,"\t\tMarginRateGrade = %s -> %s    [保证金等级]\n",pOriginal->MarginRateGrade.getString().c_str(),MarginRateGrade.getString().c_str());
	else
		fprintf(fp,"\t\tMarginRateGrade = %s    [保证金等级]\n",MarginRateGrade.getString().c_str());
	if (compare(TradingRightGrade.getValue(),pOriginal->TradingRightGrade.getValue())!=0)
		fprintf(fp,"\t\tTradingRightGrade = %s -> %s    [权限等级]\n",pOriginal->TradingRightGrade.getString().c_str(),TradingRightGrade.getString().c_str());
	else
		fprintf(fp,"\t\tTradingRightGrade = %s    [权限等级]\n",TradingRightGrade.getString().c_str());
	if (compare(CFDGrade.getValue(),pOriginal->CFDGrade.getValue())!=0)
		fprintf(fp,"\t\tCFDGrade = %s -> %s    [差价等级]\n",pOriginal->CFDGrade.getString().c_str(),CFDGrade.getString().c_str());
	else
		fprintf(fp,"\t\tCFDGrade = %s    [差价等级]\n",CFDGrade.getString().c_str());
	if (compare(LiquidityGrade.getValue(),pOriginal->LiquidityGrade.getValue())!=0)
		fprintf(fp,"\t\tLiquidityGrade = %s -> %s    [流动性等级]\n",pOriginal->LiquidityGrade.getString().c_str(),LiquidityGrade.getString().c_str());
	else
		fprintf(fp,"\t\tLiquidityGrade = %s    [流动性等级]\n",LiquidityGrade.getString().c_str());
	if (compare(RiskLevel.getValue(),pOriginal->RiskLevel.getValue())!=0)
		fprintf(fp,"\t\tRiskLevel = %s -> %s    [风控类型]\n",pOriginal->RiskLevel.getString().c_str(),RiskLevel.getString().c_str());
	else
		fprintf(fp,"\t\tRiskLevel = %s    [风控类型]\n",RiskLevel.getString().c_str());
	if (compare(PositionType.getValue(),pOriginal->PositionType.getValue())!=0)
		fprintf(fp,"\t\tPositionType = %s -> %s    [持仓类型]\n",pOriginal->PositionType.getString().c_str(),PositionType.getString().c_str());
	else
		fprintf(fp,"\t\tPositionType = %s    [持仓类型]\n",PositionType.getString().c_str());
	if (compare(TriggerOrders.getValue(),pOriginal->TriggerOrders.getValue())!=0)
		fprintf(fp,"\t\tTriggerOrders = %s -> %s    [触发订单个数]\n",pOriginal->TriggerOrders.getString().c_str(),TriggerOrders.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerOrders = %s    [触发订单个数]\n",TriggerOrders.getString().c_str());
	if (compare(OpenOrders.getValue(),pOriginal->OpenOrders.getValue())!=0)
		fprintf(fp,"\t\tOpenOrders = %s -> %s    [未成交订单数]\n",pOriginal->OpenOrders.getString().c_str(),OpenOrders.getString().c_str());
	else
		fprintf(fp,"\t\tOpenOrders = %s    [未成交订单数]\n",OpenOrders.getString().c_str());
	if (compare(OpenPositions.getValue(),pOriginal->OpenPositions.getValue())!=0)
		fprintf(fp,"\t\tOpenPositions = %s -> %s    [用户持仓个数]\n",pOriginal->OpenPositions.getString().c_str(),OpenPositions.getString().c_str());
	else
		fprintf(fp,"\t\tOpenPositions = %s    [用户持仓个数]\n",OpenPositions.getString().c_str());
	if (compare(MaxLocalID.getValue(),pOriginal->MaxLocalID.getValue())!=0)
		fprintf(fp,"\t\tMaxLocalID = %s -> %s    [最大本地标识]\n",pOriginal->MaxLocalID.getString().c_str(),MaxLocalID.getString().c_str());
	else
		fprintf(fp,"\t\tMaxLocalID = %s    [最大本地标识]\n",MaxLocalID.getString().c_str());
	if (compare(SinkType.getValue(),pOriginal->SinkType.getValue())!=0)
		fprintf(fp,"\t\tSinkType = %s -> %s    [下游系统同步类型]\n",pOriginal->SinkType.getString().c_str(),SinkType.getString().c_str());
	else
		fprintf(fp,"\t\tSinkType = %s    [下游系统同步类型]\n",SinkType.getString().c_str());
	if (compare(CopyMemberID.getValue(),pOriginal->CopyMemberID.getValue())!=0)
		fprintf(fp,"\t\tCopyMemberID = %s -> %s    [带单员代码]\n",pOriginal->CopyMemberID.getString().c_str(),CopyMemberID.getString().c_str());
	else
		fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());
	if (compare(Region.getValue(),pOriginal->Region.getValue())!=0)
		fprintf(fp,"\t\tRegion = %s -> %s    []\n",pOriginal->Region.getString().c_str(),Region.getString().c_str());
	else
		fprintf(fp,"\t\tRegion = %s    []\n",Region.getString().c_str());
	if (compare(FeeDeduceDisCount.getValue(),pOriginal->FeeDeduceDisCount.getValue())!=0)
		fprintf(fp,"\t\tFeeDeduceDisCount = %s -> %s    [作为手续费的抵扣优惠比例]\n",pOriginal->FeeDeduceDisCount.getString().c_str(),FeeDeduceDisCount.getString().c_str());
	else
		fprintf(fp,"\t\tFeeDeduceDisCount = %s    [作为手续费的抵扣优惠比例]\n",FeeDeduceDisCount.getString().c_str());
	if (compare(FeeDeduceCurrency.getValue(),pOriginal->FeeDeduceCurrency.getValue())!=0)
		fprintf(fp,"\t\tFeeDeduceCurrency = %s -> %s    [作为手续费的抵扣比例]\n",pOriginal->FeeDeduceCurrency.getString().c_str(),FeeDeduceCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tFeeDeduceCurrency = %s    [作为手续费的抵扣比例]\n",FeeDeduceCurrency.getString().c_str());
	if (compare(CanBorrow.getValue(),pOriginal->CanBorrow.getValue())!=0)
		fprintf(fp,"\t\tCanBorrow = %s -> %s    [是否需要借钱交易]\n",pOriginal->CanBorrow.getString().c_str(),CanBorrow.getString().c_str());
	else
		fprintf(fp,"\t\tCanBorrow = %s    [是否需要借钱交易]\n",CanBorrow.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableMember::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",MemberType=%s",MemberType.getString().c_str());	
	fprintf(fp,",Grade=%s",Grade.getString().c_str());	
	fprintf(fp,",Priority=%s",Priority.getString().c_str());	
	fprintf(fp,",FeeGrade=%s",FeeGrade.getString().c_str());	
	fprintf(fp,",MarginRateGrade=%s",MarginRateGrade.getString().c_str());	
	fprintf(fp,",TradingRightGrade=%s",TradingRightGrade.getString().c_str());	
	fprintf(fp,",CFDGrade=%s",CFDGrade.getString().c_str());	
	fprintf(fp,",LiquidityGrade=%s",LiquidityGrade.getString().c_str());	
	fprintf(fp,",RiskLevel=%s",RiskLevel.getString().c_str());	
	fprintf(fp,",PositionType=%s",PositionType.getString().c_str());	
	fprintf(fp,",TriggerOrders=%s",TriggerOrders.getString().c_str());	
	fprintf(fp,",OpenOrders=%s",OpenOrders.getString().c_str());	
	fprintf(fp,",OpenPositions=%s",OpenPositions.getString().c_str());	
	fprintf(fp,",MaxLocalID=%s",MaxLocalID.getString().c_str());	
	fprintf(fp,",SinkType=%s",SinkType.getString().c_str());	
	fprintf(fp,",CopyMemberID=%s",CopyMemberID.getString().c_str());	
	fprintf(fp,",Region=%s",Region.getString().c_str());	
	fprintf(fp,",FeeDeduceDisCount=%s",FeeDeduceDisCount.getString().c_str());	
	fprintf(fp,",FeeDeduceCurrency=%s",FeeDeduceCurrency.getString().c_str());	
	fprintf(fp,",CanBorrow=%s",CanBorrow.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableMember::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MemberType=%s,",MemberType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Grade=%s,",Grade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Priority=%s,",Priority.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FeeGrade=%s,",FeeGrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MarginRateGrade=%s,",MarginRateGrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradingRightGrade=%s,",TradingRightGrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CFDGrade=%s,",CFDGrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LiquidityGrade=%s,",LiquidityGrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"RiskLevel=%s,",RiskLevel.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionType=%s,",PositionType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerOrders=%s,",TriggerOrders.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenOrders=%s,",OpenOrders.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenPositions=%s,",OpenPositions.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MaxLocalID=%s,",MaxLocalID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SinkType=%s,",SinkType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyMemberID=%s,",CopyMemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Region=%s,",Region.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FeeDeduceDisCount=%s,",FeeDeduceDisCount.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FeeDeduceCurrency=%s,",FeeDeduceCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CanBorrow=%s,",CanBorrow.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CMemberIterator::dump(FILE *output)
{
	CMember *pMember;
	while ((pMember=next())!=NULL)
	{
		pMember->dump(output);
	}
}



void CWriteableServiceConfig::writeCSVHead(FILE *output)
{
	fprintf(output,"\"ConfigName*\",\"Index1*\",\"Index2*\",\"ConfigValue\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}

void CWriteableServiceConfig::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"ConfigName\",\"Index1\",\"Index2\",\"ConfigValue\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}
	
void CWriteableServiceConfig::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	ConfigName.init();
	Index1.setValue("default");
	Index2.setValue("default");
	ConfigValue.init();
	Remark.clear();
	InsertTime.clear();
	UpdateTime.clear();
}

void CWriteableServiceConfig::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableServiceConfig::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableServiceConfig),input)!=sizeof(CWriteableServiceConfig))
		return 0;
	else
		return 1;
}
	
int CWriteableServiceConfig::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			ConfigName=token;
		else
			ConfigName="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Index1=token;
		else
			Index1="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Index2=token;
		else
			Index2="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ConfigValue=token;
		else
			ConfigValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"ConfigName"))
			{
				ConfigName=token;
			}
			else if (!strcmp(fieldName,"Index1"))
			{
				Index1=token;
			}
			else if (!strcmp(fieldName,"Index2"))
			{
				Index2=token;
			}
			else if (!strcmp(fieldName,"ConfigValue"))
			{
				ConfigValue=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableServiceConfig::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableServiceConfig),output)!=sizeof(CWriteableServiceConfig))
		return 0;
	else
		return 1;
}

int CWriteableServiceConfig::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",ConfigName.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Index1.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Index2.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ConfigValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableServiceConfig::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",ConfigName.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Index1.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Index2.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ConfigValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableServiceConfig::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCServiceConfig%s = {\n", index_buf);
	fprintf(fp,"\t\tConfigName = %s    [配置名称]\n",ConfigName.getString().c_str());	
	fprintf(fp,"\t\tIndex1 = %s    [索引1]\n",Index1.getString().c_str());	
	fprintf(fp,"\t\tIndex2 = %s    [索引2]\n",Index2.getString().c_str());	
	fprintf(fp,"\t\tConfigValue = %s    [配置值]\n",ConfigValue.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableServiceConfig::dumpDiff(FILE *fp, const CWriteableServiceConfig *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCServiceConfig = {\n");
	if (compare(ConfigName.getValue(),pOriginal->ConfigName.getValue())!=0)
		fprintf(fp,"\t\tConfigName = %s -> %s    [配置名称]\n",pOriginal->ConfigName.getString().c_str(),ConfigName.getString().c_str());
	else
		fprintf(fp,"\t\tConfigName = %s    [配置名称]\n",ConfigName.getString().c_str());
	if (compare(Index1.getValue(),pOriginal->Index1.getValue())!=0)
		fprintf(fp,"\t\tIndex1 = %s -> %s    [索引1]\n",pOriginal->Index1.getString().c_str(),Index1.getString().c_str());
	else
		fprintf(fp,"\t\tIndex1 = %s    [索引1]\n",Index1.getString().c_str());
	if (compare(Index2.getValue(),pOriginal->Index2.getValue())!=0)
		fprintf(fp,"\t\tIndex2 = %s -> %s    [索引2]\n",pOriginal->Index2.getString().c_str(),Index2.getString().c_str());
	else
		fprintf(fp,"\t\tIndex2 = %s    [索引2]\n",Index2.getString().c_str());
	if (compare(ConfigValue.getValue(),pOriginal->ConfigValue.getValue())!=0)
		fprintf(fp,"\t\tConfigValue = %s -> %s    [配置值]\n",pOriginal->ConfigValue.getString().c_str(),ConfigValue.getString().c_str());
	else
		fprintf(fp,"\t\tConfigValue = %s    [配置值]\n",ConfigValue.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableServiceConfig::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",ConfigName=%s",ConfigName.getString().c_str());	
	fprintf(fp,",Index1=%s",Index1.getString().c_str());	
	fprintf(fp,",Index2=%s",Index2.getString().c_str());	
	fprintf(fp,",ConfigValue=%s",ConfigValue.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableServiceConfig::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"ConfigName=%s,",ConfigName.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Index1=%s,",Index1.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Index2=%s,",Index2.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ConfigValue=%s,",ConfigValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CServiceConfigIterator::dump(FILE *output)
{
	CServiceConfig *pServiceConfig;
	while ((pServiceConfig=next())!=NULL)
	{
		pServiceConfig->dump(output);
	}
}



void CWriteableUserSession::writeCSVHead(FILE *output)
{
	fprintf(output,"\"UserID\",\"Password\",\"UserProductID\",\"MacAddress\",\"ClientIPAddress\",\"HDSerialID\",\"AuthCode\",\"APPID\",\"Token*\",\"Remark\",\"APIID\",\"LoginTime\",\"ExpireTime\",\"IPAddress\",\"MaxLocalID\",\"SessionNo\",\"FrontNo\",\"AccessLimit\",\"UserType\",\"MemberID\",\"LimitAccesses\",\"SinkType\",\"InsertTime\",\"UpdateTime\"\n");
}

void CWriteableUserSession::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"UserID\",\"Password\",\"UserProductID\",\"MacAddress\",\"ClientIPAddress\",\"HDSerialID\",\"AuthCode\",\"APPID\",\"Token\",\"Remark\",\"APIID\",\"LoginTime\",\"ExpireTime\",\"IPAddress\",\"MaxLocalID\",\"SessionNo\",\"FrontNo\",\"AccessLimit\",\"UserType\",\"MemberID\",\"LimitAccesses\",\"SinkType\",\"InsertTime\",\"UpdateTime\"\n");
}
	
void CWriteableUserSession::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	UserID.clear();
	Password.clear();
	UserProductID.clear();
	MacAddress.clear();
	ClientIPAddress.clear();
	HDSerialID.clear();
	AuthCode.clear();
	APPID.clear();
	Token.init();
	Remark.clear();
	APIID.clear();
	LoginTime.clear();
	ExpireTime.clear();
	IPAddress.clear();
	MaxLocalID.clear();
	SessionNo.init();
	FrontNo.init();
	AccessLimit.clear();
	UserType.init();
	MemberID.clear();
	LimitAccesses.clear();
	SinkType.clear();
	InsertTime.clear();
	UpdateTime.clear();
}

void CWriteableUserSession::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableUserSession::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableUserSession),input)!=sizeof(CWriteableUserSession))
		return 0;
	else
		return 1;
}
	
int CWriteableUserSession::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			UserID=token;
		else
			UserID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Password=token;
		else
			Password="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UserProductID=token;
		else
			UserProductID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MacAddress=token;
		else
			MacAddress="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClientIPAddress=token;
		else
			ClientIPAddress="";
		token=getNextToken(NULL);
		if (token!=NULL)
			HDSerialID=token;
		else
			HDSerialID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AuthCode=token;
		else
			AuthCode="";
		token=getNextToken(NULL);
		if (token!=NULL)
			APPID=token;
		else
			APPID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Token=token;
		else
			Token="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			APIID=token;
		else
			APIID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LoginTime=token;
		else
			LoginTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExpireTime=token;
		else
			ExpireTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			IPAddress=token;
		else
			IPAddress="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MaxLocalID=token;
		else
			MaxLocalID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SessionNo=token;
		else
			SessionNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrontNo=token;
		else
			FrontNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccessLimit=token;
		else
			AccessLimit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UserType=token;
		else
			UserType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LimitAccesses=token;
		else
			LimitAccesses="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SinkType=token;
		else
			SinkType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"UserID"))
			{
				UserID=token;
			}
			else if (!strcmp(fieldName,"Password"))
			{
				Password=token;
			}
			else if (!strcmp(fieldName,"UserProductID"))
			{
				UserProductID=token;
			}
			else if (!strcmp(fieldName,"MacAddress"))
			{
				MacAddress=token;
			}
			else if (!strcmp(fieldName,"ClientIPAddress"))
			{
				ClientIPAddress=token;
			}
			else if (!strcmp(fieldName,"HDSerialID"))
			{
				HDSerialID=token;
			}
			else if (!strcmp(fieldName,"AuthCode"))
			{
				AuthCode=token;
			}
			else if (!strcmp(fieldName,"APPID"))
			{
				APPID=token;
			}
			else if (!strcmp(fieldName,"Token"))
			{
				Token=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"APIID"))
			{
				APIID=token;
			}
			else if (!strcmp(fieldName,"LoginTime"))
			{
				LoginTime=token;
			}
			else if (!strcmp(fieldName,"ExpireTime"))
			{
				ExpireTime=token;
			}
			else if (!strcmp(fieldName,"IPAddress"))
			{
				IPAddress=token;
			}
			else if (!strcmp(fieldName,"MaxLocalID"))
			{
				MaxLocalID=token;
			}
			else if (!strcmp(fieldName,"SessionNo"))
			{
				SessionNo=token;
			}
			else if (!strcmp(fieldName,"FrontNo"))
			{
				FrontNo=token;
			}
			else if (!strcmp(fieldName,"AccessLimit"))
			{
				AccessLimit=token;
			}
			else if (!strcmp(fieldName,"UserType"))
			{
				UserType=token;
			}
			else if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"LimitAccesses"))
			{
				LimitAccesses=token;
			}
			else if (!strcmp(fieldName,"SinkType"))
			{
				SinkType=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableUserSession::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableUserSession),output)!=sizeof(CWriteableUserSession))
		return 0;
	else
		return 1;
}

int CWriteableUserSession::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",UserID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Password.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UserProductID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MacAddress.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClientIPAddress.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",HDSerialID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AuthCode.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Token.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",APIID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoginTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExpireTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",IPAddress.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxLocalID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SessionNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrontNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccessLimit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UserType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LimitAccesses.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SinkType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableUserSession::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",UserID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Password.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UserProductID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MacAddress.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClientIPAddress.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",HDSerialID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AuthCode.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Token.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",APIID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoginTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExpireTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",IPAddress.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxLocalID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SessionNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrontNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccessLimit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UserType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LimitAccesses.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SinkType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableUserSession::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCUserSession%s = {\n", index_buf);
	fprintf(fp,"\t\tUserID = %s    [交易用户代码]\n",UserID.getString().c_str());	
	fprintf(fp,"\t\tPassword = %s    [密码]\n",Password.getString().c_str());	
	fprintf(fp,"\t\tUserProductID = %s    [用户端产品信息]\n",UserProductID.getString().c_str());	
	fprintf(fp,"\t\tMacAddress = %s    [Mac地址]\n",MacAddress.getString().c_str());	
	fprintf(fp,"\t\tClientIPAddress = %s    [终端IP地址]\n",ClientIPAddress.getString().c_str());	
	fprintf(fp,"\t\tHDSerialID = %s    [硬盘序列号]\n",HDSerialID.getString().c_str());	
	fprintf(fp,"\t\tAuthCode = %s    [授权编码]\n",AuthCode.getString().c_str());	
	fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());	
	fprintf(fp,"\t\tToken = %s    [令牌]\n",Token.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tAPIID = %s    [接口端产品信息]\n",APIID.getString().c_str());	
	fprintf(fp,"\t\tLoginTime = %s    [登录时间]\n",LoginTime.getString().c_str());	
	fprintf(fp,"\t\tExpireTime = %s    [失效时间]\n",ExpireTime.getString().c_str());	
	fprintf(fp,"\t\tIPAddress = %s    [允许登录的IP地址]\n",IPAddress.getString().c_str());	
	fprintf(fp,"\t\tMaxLocalID = %s    [最大本地标识]\n",MaxLocalID.getString().c_str());	
	fprintf(fp,"\t\tSessionNo = %s    [会话编号]\n",SessionNo.getString().c_str());	
	fprintf(fp,"\t\tFrontNo = %s    [前置编号]\n",FrontNo.getString().c_str());	
	fprintf(fp,"\t\tAccessLimit = %s    [每秒访问的次数限制]\n",AccessLimit.getString().c_str());	
	fprintf(fp,"\t\tUserType = %s    [用户类型]\n",UserType.getString().c_str());	
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tLimitAccesses = %s    [该秒已经访问的次数]\n",LimitAccesses.getString().c_str());	
	fprintf(fp,"\t\tSinkType = %s    [下游系统同步类型]\n",SinkType.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableUserSession::dumpDiff(FILE *fp, const CWriteableUserSession *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCUserSession = {\n");
	if (compare(UserID.getValue(),pOriginal->UserID.getValue())!=0)
		fprintf(fp,"\t\tUserID = %s -> %s    [交易用户代码]\n",pOriginal->UserID.getString().c_str(),UserID.getString().c_str());
	else
		fprintf(fp,"\t\tUserID = %s    [交易用户代码]\n",UserID.getString().c_str());
	if (compare(Password.getValue(),pOriginal->Password.getValue())!=0)
		fprintf(fp,"\t\tPassword = %s -> %s    [密码]\n",pOriginal->Password.getString().c_str(),Password.getString().c_str());
	else
		fprintf(fp,"\t\tPassword = %s    [密码]\n",Password.getString().c_str());
	if (compare(UserProductID.getValue(),pOriginal->UserProductID.getValue())!=0)
		fprintf(fp,"\t\tUserProductID = %s -> %s    [用户端产品信息]\n",pOriginal->UserProductID.getString().c_str(),UserProductID.getString().c_str());
	else
		fprintf(fp,"\t\tUserProductID = %s    [用户端产品信息]\n",UserProductID.getString().c_str());
	if (compare(MacAddress.getValue(),pOriginal->MacAddress.getValue())!=0)
		fprintf(fp,"\t\tMacAddress = %s -> %s    [Mac地址]\n",pOriginal->MacAddress.getString().c_str(),MacAddress.getString().c_str());
	else
		fprintf(fp,"\t\tMacAddress = %s    [Mac地址]\n",MacAddress.getString().c_str());
	if (compare(ClientIPAddress.getValue(),pOriginal->ClientIPAddress.getValue())!=0)
		fprintf(fp,"\t\tClientIPAddress = %s -> %s    [终端IP地址]\n",pOriginal->ClientIPAddress.getString().c_str(),ClientIPAddress.getString().c_str());
	else
		fprintf(fp,"\t\tClientIPAddress = %s    [终端IP地址]\n",ClientIPAddress.getString().c_str());
	if (compare(HDSerialID.getValue(),pOriginal->HDSerialID.getValue())!=0)
		fprintf(fp,"\t\tHDSerialID = %s -> %s    [硬盘序列号]\n",pOriginal->HDSerialID.getString().c_str(),HDSerialID.getString().c_str());
	else
		fprintf(fp,"\t\tHDSerialID = %s    [硬盘序列号]\n",HDSerialID.getString().c_str());
	if (compare(AuthCode.getValue(),pOriginal->AuthCode.getValue())!=0)
		fprintf(fp,"\t\tAuthCode = %s -> %s    [授权编码]\n",pOriginal->AuthCode.getString().c_str(),AuthCode.getString().c_str());
	else
		fprintf(fp,"\t\tAuthCode = %s    [授权编码]\n",AuthCode.getString().c_str());
	if (compare(APPID.getValue(),pOriginal->APPID.getValue())!=0)
		fprintf(fp,"\t\tAPPID = %s -> %s    [应用编号]\n",pOriginal->APPID.getString().c_str(),APPID.getString().c_str());
	else
		fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());
	if (compare(Token.getValue(),pOriginal->Token.getValue())!=0)
		fprintf(fp,"\t\tToken = %s -> %s    [令牌]\n",pOriginal->Token.getString().c_str(),Token.getString().c_str());
	else
		fprintf(fp,"\t\tToken = %s    [令牌]\n",Token.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(APIID.getValue(),pOriginal->APIID.getValue())!=0)
		fprintf(fp,"\t\tAPIID = %s -> %s    [接口端产品信息]\n",pOriginal->APIID.getString().c_str(),APIID.getString().c_str());
	else
		fprintf(fp,"\t\tAPIID = %s    [接口端产品信息]\n",APIID.getString().c_str());
	if (compare(LoginTime.getValue(),pOriginal->LoginTime.getValue())!=0)
		fprintf(fp,"\t\tLoginTime = %s -> %s    [登录时间]\n",pOriginal->LoginTime.getString().c_str(),LoginTime.getString().c_str());
	else
		fprintf(fp,"\t\tLoginTime = %s    [登录时间]\n",LoginTime.getString().c_str());
	if (compare(ExpireTime.getValue(),pOriginal->ExpireTime.getValue())!=0)
		fprintf(fp,"\t\tExpireTime = %s -> %s    [失效时间]\n",pOriginal->ExpireTime.getString().c_str(),ExpireTime.getString().c_str());
	else
		fprintf(fp,"\t\tExpireTime = %s    [失效时间]\n",ExpireTime.getString().c_str());
	if (compare(IPAddress.getValue(),pOriginal->IPAddress.getValue())!=0)
		fprintf(fp,"\t\tIPAddress = %s -> %s    [允许登录的IP地址]\n",pOriginal->IPAddress.getString().c_str(),IPAddress.getString().c_str());
	else
		fprintf(fp,"\t\tIPAddress = %s    [允许登录的IP地址]\n",IPAddress.getString().c_str());
	if (compare(MaxLocalID.getValue(),pOriginal->MaxLocalID.getValue())!=0)
		fprintf(fp,"\t\tMaxLocalID = %s -> %s    [最大本地标识]\n",pOriginal->MaxLocalID.getString().c_str(),MaxLocalID.getString().c_str());
	else
		fprintf(fp,"\t\tMaxLocalID = %s    [最大本地标识]\n",MaxLocalID.getString().c_str());
	if (compare(SessionNo.getValue(),pOriginal->SessionNo.getValue())!=0)
		fprintf(fp,"\t\tSessionNo = %s -> %s    [会话编号]\n",pOriginal->SessionNo.getString().c_str(),SessionNo.getString().c_str());
	else
		fprintf(fp,"\t\tSessionNo = %s    [会话编号]\n",SessionNo.getString().c_str());
	if (compare(FrontNo.getValue(),pOriginal->FrontNo.getValue())!=0)
		fprintf(fp,"\t\tFrontNo = %s -> %s    [前置编号]\n",pOriginal->FrontNo.getString().c_str(),FrontNo.getString().c_str());
	else
		fprintf(fp,"\t\tFrontNo = %s    [前置编号]\n",FrontNo.getString().c_str());
	if (compare(AccessLimit.getValue(),pOriginal->AccessLimit.getValue())!=0)
		fprintf(fp,"\t\tAccessLimit = %s -> %s    [每秒访问的次数限制]\n",pOriginal->AccessLimit.getString().c_str(),AccessLimit.getString().c_str());
	else
		fprintf(fp,"\t\tAccessLimit = %s    [每秒访问的次数限制]\n",AccessLimit.getString().c_str());
	if (compare(UserType.getValue(),pOriginal->UserType.getValue())!=0)
		fprintf(fp,"\t\tUserType = %s -> %s    [用户类型]\n",pOriginal->UserType.getString().c_str(),UserType.getString().c_str());
	else
		fprintf(fp,"\t\tUserType = %s    [用户类型]\n",UserType.getString().c_str());
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(LimitAccesses.getValue(),pOriginal->LimitAccesses.getValue())!=0)
		fprintf(fp,"\t\tLimitAccesses = %s -> %s    [该秒已经访问的次数]\n",pOriginal->LimitAccesses.getString().c_str(),LimitAccesses.getString().c_str());
	else
		fprintf(fp,"\t\tLimitAccesses = %s    [该秒已经访问的次数]\n",LimitAccesses.getString().c_str());
	if (compare(SinkType.getValue(),pOriginal->SinkType.getValue())!=0)
		fprintf(fp,"\t\tSinkType = %s -> %s    [下游系统同步类型]\n",pOriginal->SinkType.getString().c_str(),SinkType.getString().c_str());
	else
		fprintf(fp,"\t\tSinkType = %s    [下游系统同步类型]\n",SinkType.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableUserSession::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",UserID=%s",UserID.getString().c_str());	
	fprintf(fp,",Password=%s",Password.getString().c_str());	
	fprintf(fp,",UserProductID=%s",UserProductID.getString().c_str());	
	fprintf(fp,",MacAddress=%s",MacAddress.getString().c_str());	
	fprintf(fp,",ClientIPAddress=%s",ClientIPAddress.getString().c_str());	
	fprintf(fp,",HDSerialID=%s",HDSerialID.getString().c_str());	
	fprintf(fp,",AuthCode=%s",AuthCode.getString().c_str());	
	fprintf(fp,",APPID=%s",APPID.getString().c_str());	
	fprintf(fp,",Token=%s",Token.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",APIID=%s",APIID.getString().c_str());	
	fprintf(fp,",LoginTime=%s",LoginTime.getString().c_str());	
	fprintf(fp,",ExpireTime=%s",ExpireTime.getString().c_str());	
	fprintf(fp,",IPAddress=%s",IPAddress.getString().c_str());	
	fprintf(fp,",MaxLocalID=%s",MaxLocalID.getString().c_str());	
	fprintf(fp,",SessionNo=%s",SessionNo.getString().c_str());	
	fprintf(fp,",FrontNo=%s",FrontNo.getString().c_str());	
	fprintf(fp,",AccessLimit=%s",AccessLimit.getString().c_str());	
	fprintf(fp,",UserType=%s",UserType.getString().c_str());	
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",LimitAccesses=%s",LimitAccesses.getString().c_str());	
	fprintf(fp,",SinkType=%s",SinkType.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableUserSession::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"UserID=%s,",UserID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Password=%s,",Password.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UserProductID=%s,",UserProductID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MacAddress=%s,",MacAddress.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClientIPAddress=%s,",ClientIPAddress.getString().c_str());
	p+=strlen(p);
	sprintf(p,"HDSerialID=%s,",HDSerialID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AuthCode=%s,",AuthCode.getString().c_str());
	p+=strlen(p);
	sprintf(p,"APPID=%s,",APPID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Token=%s,",Token.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"APIID=%s,",APIID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LoginTime=%s,",LoginTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExpireTime=%s,",ExpireTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"IPAddress=%s,",IPAddress.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MaxLocalID=%s,",MaxLocalID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SessionNo=%s,",SessionNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrontNo=%s,",FrontNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccessLimit=%s,",AccessLimit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UserType=%s,",UserType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LimitAccesses=%s,",LimitAccesses.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SinkType=%s,",SinkType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CUserSessionIterator::dump(FILE *output)
{
	CUserSession *pUserSession;
	while ((pUserSession=next())!=NULL)
	{
		pUserSession->dump(output);
	}
}



void CWriteableInstrument::writeCSVHead(FILE *output)
{
	fprintf(output,"\"ExchangeID*\",\"InstrumentID*\",\"InstrumentName\",\"SettlementGroup\",\"ProductGroup\",\"UnderlyingID\",\"ProductClass\",\"ProductType\",\"StrikePrice\",\"OptionsType\",\"VolumeMultiple\",\"StartTime\",\"ExpireTime\",\"PriceCurrency\",\"ClearCurrency\",\"IsInverse\",\"BaseCurrency\",\"MarginPriceType\",\"BasisPrice\",\"MinOrderVolume\",\"MinOrderCost\",\"MaxOrderVolume\",\"PriceTick\",\"VolumeTick\",\"ShowVolumeTick\",\"TradingModel\",\"DayStartTime\",\"SortNumber\",\"Remark\",\"DefaultLeverage\",\"PriceLimitValueMode\",\"PriceLimitPrice\",\"PriceLimitUpperValue\",\"PriceLimitLowerValue\",\"MarginRateGroup\",\"CFDGroup\",\"FeeGroup\",\"TradingRightGroup\",\"MaxOpenInterest\",\"FundingRateGroup\",\"InsertTime\",\"UpdateTime\",\"MarkedPriceTimes\"\n");
}

void CWriteableInstrument::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"ExchangeID\",\"InstrumentID\",\"InstrumentName\",\"SettlementGroup\",\"ProductGroup\",\"UnderlyingID\",\"ProductClass\",\"ProductType\",\"StrikePrice\",\"OptionsType\",\"VolumeMultiple\",\"StartTime\",\"ExpireTime\",\"PriceCurrency\",\"ClearCurrency\",\"IsInverse\",\"BaseCurrency\",\"MarginPriceType\",\"BasisPrice\",\"MinOrderVolume\",\"MinOrderCost\",\"MaxOrderVolume\",\"PriceTick\",\"VolumeTick\",\"ShowVolumeTick\",\"TradingModel\",\"DayStartTime\",\"SortNumber\",\"Remark\",\"DefaultLeverage\",\"PriceLimitValueMode\",\"PriceLimitPrice\",\"PriceLimitUpperValue\",\"PriceLimitLowerValue\",\"MarginRateGroup\",\"CFDGroup\",\"FeeGroup\",\"TradingRightGroup\",\"MaxOpenInterest\",\"FundingRateGroup\",\"InsertTime\",\"UpdateTime\",\"MarkedPriceTimes\"\n");
}
	
void CWriteableInstrument::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	ExchangeID.init();
	InstrumentID.init();
	InstrumentName.clear();
	SettlementGroup.init();
	ProductGroup.clear();
	UnderlyingID.clear();
	ProductClass.init();
	ProductType.init();
	StrikePrice.clear();
	OptionsType.clear();
	VolumeMultiple.init();
	StartTime.clear();
	ExpireTime.clear();
	PriceCurrency.init();
	ClearCurrency.init();
	IsInverse.init();
	BaseCurrency.init();
	MarginPriceType.init();
	BasisPrice.clear();
	MinOrderVolume.clear();
	MinOrderCost.clear();
	MaxOrderVolume.clear();
	PriceTick.setValue(0.00000001);
	VolumeTick.setValue(0.00000001);
	ShowVolumeTick.clear();
	TradingModel.init();
	DayStartTime.clear();
	SortNumber.clear();
	Remark.clear();
	DefaultLeverage.clear();
	PriceLimitValueMode.clear();
	PriceLimitPrice.clear();
	PriceLimitUpperValue.clear();
	PriceLimitLowerValue.clear();
	MarginRateGroup.clear();
	CFDGroup.clear();
	FeeGroup.clear();
	TradingRightGroup.clear();
	MaxOpenInterest.clear();
	FundingRateGroup.clear();
	InsertTime.clear();
	UpdateTime.clear();
	MarkedPriceTimes.clear();
}

void CWriteableInstrument::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableInstrument::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableInstrument),input)!=sizeof(CWriteableInstrument))
		return 0;
	else
		return 1;
}
	
int CWriteableInstrument::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentName=token;
		else
			InstrumentName="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettlementGroup=token;
		else
			SettlementGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ProductGroup=token;
		else
			ProductGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UnderlyingID=token;
		else
			UnderlyingID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ProductClass=token;
		else
			ProductClass="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ProductType=token;
		else
			ProductType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			StrikePrice=token;
		else
			StrikePrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OptionsType=token;
		else
			OptionsType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeMultiple=token;
		else
			VolumeMultiple="";
		token=getNextToken(NULL);
		if (token!=NULL)
			StartTime=token;
		else
			StartTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExpireTime=token;
		else
			ExpireTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PriceCurrency=token;
		else
			PriceCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClearCurrency=token;
		else
			ClearCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			IsInverse=token;
		else
			IsInverse="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BaseCurrency=token;
		else
			BaseCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MarginPriceType=token;
		else
			MarginPriceType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BasisPrice=token;
		else
			BasisPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MinOrderVolume=token;
		else
			MinOrderVolume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MinOrderCost=token;
		else
			MinOrderCost="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MaxOrderVolume=token;
		else
			MaxOrderVolume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PriceTick=token;
		else
			PriceTick="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeTick=token;
		else
			VolumeTick="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ShowVolumeTick=token;
		else
			ShowVolumeTick="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradingModel=token;
		else
			TradingModel="";
		token=getNextToken(NULL);
		if (token!=NULL)
			DayStartTime=token;
		else
			DayStartTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SortNumber=token;
		else
			SortNumber="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			DefaultLeverage=token;
		else
			DefaultLeverage="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PriceLimitValueMode=token;
		else
			PriceLimitValueMode="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PriceLimitPrice=token;
		else
			PriceLimitPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PriceLimitUpperValue=token;
		else
			PriceLimitUpperValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PriceLimitLowerValue=token;
		else
			PriceLimitLowerValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MarginRateGroup=token;
		else
			MarginRateGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CFDGroup=token;
		else
			CFDGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FeeGroup=token;
		else
			FeeGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradingRightGroup=token;
		else
			TradingRightGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MaxOpenInterest=token;
		else
			MaxOpenInterest="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FundingRateGroup=token;
		else
			FundingRateGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MarkedPriceTimes=token;
		else
			MarkedPriceTimes="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"InstrumentName"))
			{
				InstrumentName=token;
			}
			else if (!strcmp(fieldName,"SettlementGroup"))
			{
				SettlementGroup=token;
			}
			else if (!strcmp(fieldName,"ProductGroup"))
			{
				ProductGroup=token;
			}
			else if (!strcmp(fieldName,"UnderlyingID"))
			{
				UnderlyingID=token;
			}
			else if (!strcmp(fieldName,"ProductClass"))
			{
				ProductClass=token;
			}
			else if (!strcmp(fieldName,"ProductType"))
			{
				ProductType=token;
			}
			else if (!strcmp(fieldName,"StrikePrice"))
			{
				StrikePrice=token;
			}
			else if (!strcmp(fieldName,"OptionsType"))
			{
				OptionsType=token;
			}
			else if (!strcmp(fieldName,"VolumeMultiple"))
			{
				VolumeMultiple=token;
			}
			else if (!strcmp(fieldName,"StartTime"))
			{
				StartTime=token;
			}
			else if (!strcmp(fieldName,"ExpireTime"))
			{
				ExpireTime=token;
			}
			else if (!strcmp(fieldName,"PriceCurrency"))
			{
				PriceCurrency=token;
			}
			else if (!strcmp(fieldName,"ClearCurrency"))
			{
				ClearCurrency=token;
			}
			else if (!strcmp(fieldName,"IsInverse"))
			{
				IsInverse=token;
			}
			else if (!strcmp(fieldName,"BaseCurrency"))
			{
				BaseCurrency=token;
			}
			else if (!strcmp(fieldName,"MarginPriceType"))
			{
				MarginPriceType=token;
			}
			else if (!strcmp(fieldName,"BasisPrice"))
			{
				BasisPrice=token;
			}
			else if (!strcmp(fieldName,"MinOrderVolume"))
			{
				MinOrderVolume=token;
			}
			else if (!strcmp(fieldName,"MinOrderCost"))
			{
				MinOrderCost=token;
			}
			else if (!strcmp(fieldName,"MaxOrderVolume"))
			{
				MaxOrderVolume=token;
			}
			else if (!strcmp(fieldName,"PriceTick"))
			{
				PriceTick=token;
			}
			else if (!strcmp(fieldName,"VolumeTick"))
			{
				VolumeTick=token;
			}
			else if (!strcmp(fieldName,"ShowVolumeTick"))
			{
				ShowVolumeTick=token;
			}
			else if (!strcmp(fieldName,"TradingModel"))
			{
				TradingModel=token;
			}
			else if (!strcmp(fieldName,"DayStartTime"))
			{
				DayStartTime=token;
			}
			else if (!strcmp(fieldName,"SortNumber"))
			{
				SortNumber=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"DefaultLeverage"))
			{
				DefaultLeverage=token;
			}
			else if (!strcmp(fieldName,"PriceLimitValueMode"))
			{
				PriceLimitValueMode=token;
			}
			else if (!strcmp(fieldName,"PriceLimitPrice"))
			{
				PriceLimitPrice=token;
			}
			else if (!strcmp(fieldName,"PriceLimitUpperValue"))
			{
				PriceLimitUpperValue=token;
			}
			else if (!strcmp(fieldName,"PriceLimitLowerValue"))
			{
				PriceLimitLowerValue=token;
			}
			else if (!strcmp(fieldName,"MarginRateGroup"))
			{
				MarginRateGroup=token;
			}
			else if (!strcmp(fieldName,"CFDGroup"))
			{
				CFDGroup=token;
			}
			else if (!strcmp(fieldName,"FeeGroup"))
			{
				FeeGroup=token;
			}
			else if (!strcmp(fieldName,"TradingRightGroup"))
			{
				TradingRightGroup=token;
			}
			else if (!strcmp(fieldName,"MaxOpenInterest"))
			{
				MaxOpenInterest=token;
			}
			else if (!strcmp(fieldName,"FundingRateGroup"))
			{
				FundingRateGroup=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			else if (!strcmp(fieldName,"MarkedPriceTimes"))
			{
				MarkedPriceTimes=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableInstrument::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableInstrument),output)!=sizeof(CWriteableInstrument))
		return 0;
	else
		return 1;
}

int CWriteableInstrument::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentName.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ProductGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UnderlyingID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ProductClass.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ProductType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",StrikePrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OptionsType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeMultiple.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",StartTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExpireTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClearCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsInverse.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BaseCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginPriceType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BasisPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MinOrderVolume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MinOrderCost.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxOrderVolume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceTick.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeTick.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShowVolumeTick.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradingModel.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",DayStartTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SortNumber.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",DefaultLeverage.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceLimitValueMode.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceLimitPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceLimitUpperValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceLimitLowerValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginRateGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CFDGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradingRightGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxOpenInterest.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FundingRateGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarkedPriceTimes.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableInstrument::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentName.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ProductGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UnderlyingID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ProductClass.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ProductType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",StrikePrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OptionsType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeMultiple.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",StartTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExpireTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClearCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsInverse.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BaseCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginPriceType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BasisPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MinOrderVolume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MinOrderCost.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxOrderVolume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceTick.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeTick.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShowVolumeTick.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradingModel.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",DayStartTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SortNumber.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",DefaultLeverage.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceLimitValueMode.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceLimitPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceLimitUpperValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceLimitLowerValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginRateGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CFDGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradingRightGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxOpenInterest.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FundingRateGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarkedPriceTimes.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableInstrument::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCInstrument%s = {\n", index_buf);
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentName = %s    [标的名称]\n",InstrumentName.getString().c_str());	
	fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());	
	fprintf(fp,"\t\tProductGroup = %s    [产品组]\n",ProductGroup.getString().c_str());	
	fprintf(fp,"\t\tUnderlyingID = %s    [基础标的代码]\n",UnderlyingID.getString().c_str());	
	fprintf(fp,"\t\tProductClass = %s    [产品大类]\n",ProductClass.getString().c_str());	
	fprintf(fp,"\t\tProductType = %s    [产品类型]\n",ProductType.getString().c_str());	
	fprintf(fp,"\t\tStrikePrice = %s    [执行价]\n",StrikePrice.getString().c_str());	
	fprintf(fp,"\t\tOptionsType = %s    [期权类型]\n",OptionsType.getString().c_str());	
	fprintf(fp,"\t\tVolumeMultiple = %s    [数量乘数]\n",VolumeMultiple.getString().c_str());	
	fprintf(fp,"\t\tStartTime = %s    [启动时间]\n",StartTime.getString().c_str());	
	fprintf(fp,"\t\tExpireTime = %s    [到期时间]\n",ExpireTime.getString().c_str());	
	fprintf(fp,"\t\tPriceCurrency = %s    [计价币种]\n",PriceCurrency.getString().c_str());	
	fprintf(fp,"\t\tClearCurrency = %s    [清算币种]\n",ClearCurrency.getString().c_str());	
	fprintf(fp,"\t\tIsInverse = %s    [是否反向标的]\n",IsInverse.getString().c_str());	
	fprintf(fp,"\t\tBaseCurrency = %s    [标的基础货币]\n",BaseCurrency.getString().c_str());	
	fprintf(fp,"\t\tMarginPriceType = %s    [保证金价格类型]\n",MarginPriceType.getString().c_str());	
	fprintf(fp,"\t\tBasisPrice = %s    [挂牌基准价]\n",BasisPrice.getString().c_str());	
	fprintf(fp,"\t\tMinOrderVolume = %s    [最小下单量]\n",MinOrderVolume.getString().c_str());	
	fprintf(fp,"\t\tMinOrderCost = %s    [最小下单额]\n",MinOrderCost.getString().c_str());	
	fprintf(fp,"\t\tMaxOrderVolume = %s    [最大下单量]\n",MaxOrderVolume.getString().c_str());	
	fprintf(fp,"\t\tPriceTick = %s    [最小变动价位]\n",PriceTick.getString().c_str());	
	fprintf(fp,"\t\tVolumeTick = %s    [最小变动量]\n",VolumeTick.getString().c_str());	
	fprintf(fp,"\t\tShowVolumeTick = %s    [前端行情显示最小变动量(服务端不用)]\n",ShowVolumeTick.getString().c_str());	
	fprintf(fp,"\t\tTradingModel = %s    [交易模式]\n",TradingModel.getString().c_str());	
	fprintf(fp,"\t\tDayStartTime = %s    [每日开始交易时间]\n",DayStartTime.getString().c_str());	
	fprintf(fp,"\t\tSortNumber = %s    [排序字段]\n",SortNumber.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tDefaultLeverage = %s    [默认杠杆倍数]\n",DefaultLeverage.getString().c_str());	
	fprintf(fp,"\t\tPriceLimitValueMode = %s    [取值方式]\n",PriceLimitValueMode.getString().c_str());	
	fprintf(fp,"\t\tPriceLimitPrice = %s    [限价价格类型]\n",PriceLimitPrice.getString().c_str());	
	fprintf(fp,"\t\tPriceLimitUpperValue = %s    [上限]\n",PriceLimitUpperValue.getString().c_str());	
	fprintf(fp,"\t\tPriceLimitLowerValue = %s    [下限]\n",PriceLimitLowerValue.getString().c_str());	
	fprintf(fp,"\t\tMarginRateGroup = %s    [保证金组]\n",MarginRateGroup.getString().c_str());	
	fprintf(fp,"\t\tCFDGroup = %s    [差价组]\n",CFDGroup.getString().c_str());	
	fprintf(fp,"\t\tFeeGroup = %s    [手续费组]\n",FeeGroup.getString().c_str());	
	fprintf(fp,"\t\tTradingRightGroup = %s    [权限组]\n",TradingRightGroup.getString().c_str());	
	fprintf(fp,"\t\tMaxOpenInterest = %s    [最大持仓量]\n",MaxOpenInterest.getString().c_str());	
	fprintf(fp,"\t\tFundingRateGroup = %s    [资金费用组]\n",FundingRateGroup.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t\tMarkedPriceTimes = %s    [标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价)]\n",MarkedPriceTimes.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableInstrument::dumpDiff(FILE *fp, const CWriteableInstrument *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCInstrument = {\n");
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(InstrumentName.getValue(),pOriginal->InstrumentName.getValue())!=0)
		fprintf(fp,"\t\tInstrumentName = %s -> %s    [标的名称]\n",pOriginal->InstrumentName.getString().c_str(),InstrumentName.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentName = %s    [标的名称]\n",InstrumentName.getString().c_str());
	if (compare(SettlementGroup.getValue(),pOriginal->SettlementGroup.getValue())!=0)
		fprintf(fp,"\t\tSettlementGroup = %s -> %s    [结算组编号]\n",pOriginal->SettlementGroup.getString().c_str(),SettlementGroup.getString().c_str());
	else
		fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());
	if (compare(ProductGroup.getValue(),pOriginal->ProductGroup.getValue())!=0)
		fprintf(fp,"\t\tProductGroup = %s -> %s    [产品组]\n",pOriginal->ProductGroup.getString().c_str(),ProductGroup.getString().c_str());
	else
		fprintf(fp,"\t\tProductGroup = %s    [产品组]\n",ProductGroup.getString().c_str());
	if (compare(UnderlyingID.getValue(),pOriginal->UnderlyingID.getValue())!=0)
		fprintf(fp,"\t\tUnderlyingID = %s -> %s    [基础标的代码]\n",pOriginal->UnderlyingID.getString().c_str(),UnderlyingID.getString().c_str());
	else
		fprintf(fp,"\t\tUnderlyingID = %s    [基础标的代码]\n",UnderlyingID.getString().c_str());
	if (compare(ProductClass.getValue(),pOriginal->ProductClass.getValue())!=0)
		fprintf(fp,"\t\tProductClass = %s -> %s    [产品大类]\n",pOriginal->ProductClass.getString().c_str(),ProductClass.getString().c_str());
	else
		fprintf(fp,"\t\tProductClass = %s    [产品大类]\n",ProductClass.getString().c_str());
	if (compare(ProductType.getValue(),pOriginal->ProductType.getValue())!=0)
		fprintf(fp,"\t\tProductType = %s -> %s    [产品类型]\n",pOriginal->ProductType.getString().c_str(),ProductType.getString().c_str());
	else
		fprintf(fp,"\t\tProductType = %s    [产品类型]\n",ProductType.getString().c_str());
	if (compare(StrikePrice.getValue(),pOriginal->StrikePrice.getValue())!=0)
		fprintf(fp,"\t\tStrikePrice = %s -> %s    [执行价]\n",pOriginal->StrikePrice.getString().c_str(),StrikePrice.getString().c_str());
	else
		fprintf(fp,"\t\tStrikePrice = %s    [执行价]\n",StrikePrice.getString().c_str());
	if (compare(OptionsType.getValue(),pOriginal->OptionsType.getValue())!=0)
		fprintf(fp,"\t\tOptionsType = %s -> %s    [期权类型]\n",pOriginal->OptionsType.getString().c_str(),OptionsType.getString().c_str());
	else
		fprintf(fp,"\t\tOptionsType = %s    [期权类型]\n",OptionsType.getString().c_str());
	if (compare(VolumeMultiple.getValue(),pOriginal->VolumeMultiple.getValue())!=0)
		fprintf(fp,"\t\tVolumeMultiple = %s -> %s    [数量乘数]\n",pOriginal->VolumeMultiple.getString().c_str(),VolumeMultiple.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeMultiple = %s    [数量乘数]\n",VolumeMultiple.getString().c_str());
	if (compare(StartTime.getValue(),pOriginal->StartTime.getValue())!=0)
		fprintf(fp,"\t\tStartTime = %s -> %s    [启动时间]\n",pOriginal->StartTime.getString().c_str(),StartTime.getString().c_str());
	else
		fprintf(fp,"\t\tStartTime = %s    [启动时间]\n",StartTime.getString().c_str());
	if (compare(ExpireTime.getValue(),pOriginal->ExpireTime.getValue())!=0)
		fprintf(fp,"\t\tExpireTime = %s -> %s    [到期时间]\n",pOriginal->ExpireTime.getString().c_str(),ExpireTime.getString().c_str());
	else
		fprintf(fp,"\t\tExpireTime = %s    [到期时间]\n",ExpireTime.getString().c_str());
	if (compare(PriceCurrency.getValue(),pOriginal->PriceCurrency.getValue())!=0)
		fprintf(fp,"\t\tPriceCurrency = %s -> %s    [计价币种]\n",pOriginal->PriceCurrency.getString().c_str(),PriceCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tPriceCurrency = %s    [计价币种]\n",PriceCurrency.getString().c_str());
	if (compare(ClearCurrency.getValue(),pOriginal->ClearCurrency.getValue())!=0)
		fprintf(fp,"\t\tClearCurrency = %s -> %s    [清算币种]\n",pOriginal->ClearCurrency.getString().c_str(),ClearCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tClearCurrency = %s    [清算币种]\n",ClearCurrency.getString().c_str());
	if (compare(IsInverse.getValue(),pOriginal->IsInverse.getValue())!=0)
		fprintf(fp,"\t\tIsInverse = %s -> %s    [是否反向标的]\n",pOriginal->IsInverse.getString().c_str(),IsInverse.getString().c_str());
	else
		fprintf(fp,"\t\tIsInverse = %s    [是否反向标的]\n",IsInverse.getString().c_str());
	if (compare(BaseCurrency.getValue(),pOriginal->BaseCurrency.getValue())!=0)
		fprintf(fp,"\t\tBaseCurrency = %s -> %s    [标的基础货币]\n",pOriginal->BaseCurrency.getString().c_str(),BaseCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tBaseCurrency = %s    [标的基础货币]\n",BaseCurrency.getString().c_str());
	if (compare(MarginPriceType.getValue(),pOriginal->MarginPriceType.getValue())!=0)
		fprintf(fp,"\t\tMarginPriceType = %s -> %s    [保证金价格类型]\n",pOriginal->MarginPriceType.getString().c_str(),MarginPriceType.getString().c_str());
	else
		fprintf(fp,"\t\tMarginPriceType = %s    [保证金价格类型]\n",MarginPriceType.getString().c_str());
	if (compare(BasisPrice.getValue(),pOriginal->BasisPrice.getValue())!=0)
		fprintf(fp,"\t\tBasisPrice = %s -> %s    [挂牌基准价]\n",pOriginal->BasisPrice.getString().c_str(),BasisPrice.getString().c_str());
	else
		fprintf(fp,"\t\tBasisPrice = %s    [挂牌基准价]\n",BasisPrice.getString().c_str());
	if (compare(MinOrderVolume.getValue(),pOriginal->MinOrderVolume.getValue())!=0)
		fprintf(fp,"\t\tMinOrderVolume = %s -> %s    [最小下单量]\n",pOriginal->MinOrderVolume.getString().c_str(),MinOrderVolume.getString().c_str());
	else
		fprintf(fp,"\t\tMinOrderVolume = %s    [最小下单量]\n",MinOrderVolume.getString().c_str());
	if (compare(MinOrderCost.getValue(),pOriginal->MinOrderCost.getValue())!=0)
		fprintf(fp,"\t\tMinOrderCost = %s -> %s    [最小下单额]\n",pOriginal->MinOrderCost.getString().c_str(),MinOrderCost.getString().c_str());
	else
		fprintf(fp,"\t\tMinOrderCost = %s    [最小下单额]\n",MinOrderCost.getString().c_str());
	if (compare(MaxOrderVolume.getValue(),pOriginal->MaxOrderVolume.getValue())!=0)
		fprintf(fp,"\t\tMaxOrderVolume = %s -> %s    [最大下单量]\n",pOriginal->MaxOrderVolume.getString().c_str(),MaxOrderVolume.getString().c_str());
	else
		fprintf(fp,"\t\tMaxOrderVolume = %s    [最大下单量]\n",MaxOrderVolume.getString().c_str());
	if (compare(PriceTick.getValue(),pOriginal->PriceTick.getValue())!=0)
		fprintf(fp,"\t\tPriceTick = %s -> %s    [最小变动价位]\n",pOriginal->PriceTick.getString().c_str(),PriceTick.getString().c_str());
	else
		fprintf(fp,"\t\tPriceTick = %s    [最小变动价位]\n",PriceTick.getString().c_str());
	if (compare(VolumeTick.getValue(),pOriginal->VolumeTick.getValue())!=0)
		fprintf(fp,"\t\tVolumeTick = %s -> %s    [最小变动量]\n",pOriginal->VolumeTick.getString().c_str(),VolumeTick.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeTick = %s    [最小变动量]\n",VolumeTick.getString().c_str());
	if (compare(ShowVolumeTick.getValue(),pOriginal->ShowVolumeTick.getValue())!=0)
		fprintf(fp,"\t\tShowVolumeTick = %s -> %s    [前端行情显示最小变动量(服务端不用)]\n",pOriginal->ShowVolumeTick.getString().c_str(),ShowVolumeTick.getString().c_str());
	else
		fprintf(fp,"\t\tShowVolumeTick = %s    [前端行情显示最小变动量(服务端不用)]\n",ShowVolumeTick.getString().c_str());
	if (compare(TradingModel.getValue(),pOriginal->TradingModel.getValue())!=0)
		fprintf(fp,"\t\tTradingModel = %s -> %s    [交易模式]\n",pOriginal->TradingModel.getString().c_str(),TradingModel.getString().c_str());
	else
		fprintf(fp,"\t\tTradingModel = %s    [交易模式]\n",TradingModel.getString().c_str());
	if (compare(DayStartTime.getValue(),pOriginal->DayStartTime.getValue())!=0)
		fprintf(fp,"\t\tDayStartTime = %s -> %s    [每日开始交易时间]\n",pOriginal->DayStartTime.getString().c_str(),DayStartTime.getString().c_str());
	else
		fprintf(fp,"\t\tDayStartTime = %s    [每日开始交易时间]\n",DayStartTime.getString().c_str());
	if (compare(SortNumber.getValue(),pOriginal->SortNumber.getValue())!=0)
		fprintf(fp,"\t\tSortNumber = %s -> %s    [排序字段]\n",pOriginal->SortNumber.getString().c_str(),SortNumber.getString().c_str());
	else
		fprintf(fp,"\t\tSortNumber = %s    [排序字段]\n",SortNumber.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(DefaultLeverage.getValue(),pOriginal->DefaultLeverage.getValue())!=0)
		fprintf(fp,"\t\tDefaultLeverage = %s -> %s    [默认杠杆倍数]\n",pOriginal->DefaultLeverage.getString().c_str(),DefaultLeverage.getString().c_str());
	else
		fprintf(fp,"\t\tDefaultLeverage = %s    [默认杠杆倍数]\n",DefaultLeverage.getString().c_str());
	if (compare(PriceLimitValueMode.getValue(),pOriginal->PriceLimitValueMode.getValue())!=0)
		fprintf(fp,"\t\tPriceLimitValueMode = %s -> %s    [取值方式]\n",pOriginal->PriceLimitValueMode.getString().c_str(),PriceLimitValueMode.getString().c_str());
	else
		fprintf(fp,"\t\tPriceLimitValueMode = %s    [取值方式]\n",PriceLimitValueMode.getString().c_str());
	if (compare(PriceLimitPrice.getValue(),pOriginal->PriceLimitPrice.getValue())!=0)
		fprintf(fp,"\t\tPriceLimitPrice = %s -> %s    [限价价格类型]\n",pOriginal->PriceLimitPrice.getString().c_str(),PriceLimitPrice.getString().c_str());
	else
		fprintf(fp,"\t\tPriceLimitPrice = %s    [限价价格类型]\n",PriceLimitPrice.getString().c_str());
	if (compare(PriceLimitUpperValue.getValue(),pOriginal->PriceLimitUpperValue.getValue())!=0)
		fprintf(fp,"\t\tPriceLimitUpperValue = %s -> %s    [上限]\n",pOriginal->PriceLimitUpperValue.getString().c_str(),PriceLimitUpperValue.getString().c_str());
	else
		fprintf(fp,"\t\tPriceLimitUpperValue = %s    [上限]\n",PriceLimitUpperValue.getString().c_str());
	if (compare(PriceLimitLowerValue.getValue(),pOriginal->PriceLimitLowerValue.getValue())!=0)
		fprintf(fp,"\t\tPriceLimitLowerValue = %s -> %s    [下限]\n",pOriginal->PriceLimitLowerValue.getString().c_str(),PriceLimitLowerValue.getString().c_str());
	else
		fprintf(fp,"\t\tPriceLimitLowerValue = %s    [下限]\n",PriceLimitLowerValue.getString().c_str());
	if (compare(MarginRateGroup.getValue(),pOriginal->MarginRateGroup.getValue())!=0)
		fprintf(fp,"\t\tMarginRateGroup = %s -> %s    [保证金组]\n",pOriginal->MarginRateGroup.getString().c_str(),MarginRateGroup.getString().c_str());
	else
		fprintf(fp,"\t\tMarginRateGroup = %s    [保证金组]\n",MarginRateGroup.getString().c_str());
	if (compare(CFDGroup.getValue(),pOriginal->CFDGroup.getValue())!=0)
		fprintf(fp,"\t\tCFDGroup = %s -> %s    [差价组]\n",pOriginal->CFDGroup.getString().c_str(),CFDGroup.getString().c_str());
	else
		fprintf(fp,"\t\tCFDGroup = %s    [差价组]\n",CFDGroup.getString().c_str());
	if (compare(FeeGroup.getValue(),pOriginal->FeeGroup.getValue())!=0)
		fprintf(fp,"\t\tFeeGroup = %s -> %s    [手续费组]\n",pOriginal->FeeGroup.getString().c_str(),FeeGroup.getString().c_str());
	else
		fprintf(fp,"\t\tFeeGroup = %s    [手续费组]\n",FeeGroup.getString().c_str());
	if (compare(TradingRightGroup.getValue(),pOriginal->TradingRightGroup.getValue())!=0)
		fprintf(fp,"\t\tTradingRightGroup = %s -> %s    [权限组]\n",pOriginal->TradingRightGroup.getString().c_str(),TradingRightGroup.getString().c_str());
	else
		fprintf(fp,"\t\tTradingRightGroup = %s    [权限组]\n",TradingRightGroup.getString().c_str());
	if (compare(MaxOpenInterest.getValue(),pOriginal->MaxOpenInterest.getValue())!=0)
		fprintf(fp,"\t\tMaxOpenInterest = %s -> %s    [最大持仓量]\n",pOriginal->MaxOpenInterest.getString().c_str(),MaxOpenInterest.getString().c_str());
	else
		fprintf(fp,"\t\tMaxOpenInterest = %s    [最大持仓量]\n",MaxOpenInterest.getString().c_str());
	if (compare(FundingRateGroup.getValue(),pOriginal->FundingRateGroup.getValue())!=0)
		fprintf(fp,"\t\tFundingRateGroup = %s -> %s    [资金费用组]\n",pOriginal->FundingRateGroup.getString().c_str(),FundingRateGroup.getString().c_str());
	else
		fprintf(fp,"\t\tFundingRateGroup = %s    [资金费用组]\n",FundingRateGroup.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	if (compare(MarkedPriceTimes.getValue(),pOriginal->MarkedPriceTimes.getValue())!=0)
		fprintf(fp,"\t\tMarkedPriceTimes = %s -> %s    [标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价)]\n",pOriginal->MarkedPriceTimes.getString().c_str(),MarkedPriceTimes.getString().c_str());
	else
		fprintf(fp,"\t\tMarkedPriceTimes = %s    [标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价)]\n",MarkedPriceTimes.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableInstrument::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",InstrumentName=%s",InstrumentName.getString().c_str());	
	fprintf(fp,",SettlementGroup=%s",SettlementGroup.getString().c_str());	
	fprintf(fp,",ProductGroup=%s",ProductGroup.getString().c_str());	
	fprintf(fp,",UnderlyingID=%s",UnderlyingID.getString().c_str());	
	fprintf(fp,",ProductClass=%s",ProductClass.getString().c_str());	
	fprintf(fp,",ProductType=%s",ProductType.getString().c_str());	
	fprintf(fp,",StrikePrice=%s",StrikePrice.getString().c_str());	
	fprintf(fp,",OptionsType=%s",OptionsType.getString().c_str());	
	fprintf(fp,",VolumeMultiple=%s",VolumeMultiple.getString().c_str());	
	fprintf(fp,",StartTime=%s",StartTime.getString().c_str());	
	fprintf(fp,",ExpireTime=%s",ExpireTime.getString().c_str());	
	fprintf(fp,",PriceCurrency=%s",PriceCurrency.getString().c_str());	
	fprintf(fp,",ClearCurrency=%s",ClearCurrency.getString().c_str());	
	fprintf(fp,",IsInverse=%s",IsInverse.getString().c_str());	
	fprintf(fp,",BaseCurrency=%s",BaseCurrency.getString().c_str());	
	fprintf(fp,",MarginPriceType=%s",MarginPriceType.getString().c_str());	
	fprintf(fp,",BasisPrice=%s",BasisPrice.getString().c_str());	
	fprintf(fp,",MinOrderVolume=%s",MinOrderVolume.getString().c_str());	
	fprintf(fp,",MinOrderCost=%s",MinOrderCost.getString().c_str());	
	fprintf(fp,",MaxOrderVolume=%s",MaxOrderVolume.getString().c_str());	
	fprintf(fp,",PriceTick=%s",PriceTick.getString().c_str());	
	fprintf(fp,",VolumeTick=%s",VolumeTick.getString().c_str());	
	fprintf(fp,",ShowVolumeTick=%s",ShowVolumeTick.getString().c_str());	
	fprintf(fp,",TradingModel=%s",TradingModel.getString().c_str());	
	fprintf(fp,",DayStartTime=%s",DayStartTime.getString().c_str());	
	fprintf(fp,",SortNumber=%s",SortNumber.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",DefaultLeverage=%s",DefaultLeverage.getString().c_str());	
	fprintf(fp,",PriceLimitValueMode=%s",PriceLimitValueMode.getString().c_str());	
	fprintf(fp,",PriceLimitPrice=%s",PriceLimitPrice.getString().c_str());	
	fprintf(fp,",PriceLimitUpperValue=%s",PriceLimitUpperValue.getString().c_str());	
	fprintf(fp,",PriceLimitLowerValue=%s",PriceLimitLowerValue.getString().c_str());	
	fprintf(fp,",MarginRateGroup=%s",MarginRateGroup.getString().c_str());	
	fprintf(fp,",CFDGroup=%s",CFDGroup.getString().c_str());	
	fprintf(fp,",FeeGroup=%s",FeeGroup.getString().c_str());	
	fprintf(fp,",TradingRightGroup=%s",TradingRightGroup.getString().c_str());	
	fprintf(fp,",MaxOpenInterest=%s",MaxOpenInterest.getString().c_str());	
	fprintf(fp,",FundingRateGroup=%s",FundingRateGroup.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,",MarkedPriceTimes=%s",MarkedPriceTimes.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableInstrument::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentName=%s,",InstrumentName.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettlementGroup=%s,",SettlementGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ProductGroup=%s,",ProductGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UnderlyingID=%s,",UnderlyingID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ProductClass=%s,",ProductClass.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ProductType=%s,",ProductType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"StrikePrice=%s,",StrikePrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OptionsType=%s,",OptionsType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeMultiple=%s,",VolumeMultiple.getString().c_str());
	p+=strlen(p);
	sprintf(p,"StartTime=%s,",StartTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExpireTime=%s,",ExpireTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PriceCurrency=%s,",PriceCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClearCurrency=%s,",ClearCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"IsInverse=%s,",IsInverse.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BaseCurrency=%s,",BaseCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MarginPriceType=%s,",MarginPriceType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BasisPrice=%s,",BasisPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MinOrderVolume=%s,",MinOrderVolume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MinOrderCost=%s,",MinOrderCost.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MaxOrderVolume=%s,",MaxOrderVolume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PriceTick=%s,",PriceTick.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeTick=%s,",VolumeTick.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ShowVolumeTick=%s,",ShowVolumeTick.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradingModel=%s,",TradingModel.getString().c_str());
	p+=strlen(p);
	sprintf(p,"DayStartTime=%s,",DayStartTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SortNumber=%s,",SortNumber.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"DefaultLeverage=%s,",DefaultLeverage.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PriceLimitValueMode=%s,",PriceLimitValueMode.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PriceLimitPrice=%s,",PriceLimitPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PriceLimitUpperValue=%s,",PriceLimitUpperValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PriceLimitLowerValue=%s,",PriceLimitLowerValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MarginRateGroup=%s,",MarginRateGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CFDGroup=%s,",CFDGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FeeGroup=%s,",FeeGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradingRightGroup=%s,",TradingRightGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MaxOpenInterest=%s,",MaxOpenInterest.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FundingRateGroup=%s,",FundingRateGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MarkedPriceTimes=%s,",MarkedPriceTimes.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CInstrumentIterator::dump(FILE *output)
{
	CInstrument *pInstrument;
	while ((pInstrument=next())!=NULL)
	{
		pInstrument->dump(output);
	}
}

const CWriteableMarketData *CWriteableInstrument::linkMarketData() const
{
	void *target=(void *)(&pMarketData);
	*((const CWriteableMarketData **)target)=m_pMDB->m_MarketDataFactory->findByInstrumentID(ExchangeID,InstrumentID);
	return pMarketData;
}

const CWriteableCurrency *CWriteableInstrument::linkClearCurrency() const
{
	void *target=(void *)(&pClearCurrency);
	*((const CWriteableCurrency **)target)=m_pMDB->m_CurrencyFactory->findByCurrency(ClearCurrency,SettlementGroup);
	return pClearCurrency;
}

const CWriteableCurrency *CWriteableInstrument::linkBaseCurrency() const
{
	void *target=(void *)(&pBaseCurrency);
	*((const CWriteableCurrency **)target)=m_pMDB->m_CurrencyFactory->findByCurrency(BaseCurrency,SettlementGroup);
	return pBaseCurrency;
}



void CWriteableTradingRight::writeCSVHead(FILE *output)
{
	fprintf(output,"\"MemberID*\",\"InstrumentID*\",\"Direction*\",\"TradingRight\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}

void CWriteableTradingRight::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"MemberID\",\"InstrumentID\",\"Direction\",\"TradingRight\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}
	
void CWriteableTradingRight::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	MemberID.init();
	InstrumentID.init();
	Direction.init();
	TradingRight.init();
	Remark.clear();
	InsertTime.clear();
	UpdateTime.clear();
}

void CWriteableTradingRight::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableTradingRight::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableTradingRight),input)!=sizeof(CWriteableTradingRight))
		return 0;
	else
		return 1;
}
	
int CWriteableTradingRight::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Direction=token;
		else
			Direction="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradingRight=token;
		else
			TradingRight="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"Direction"))
			{
				Direction=token;
			}
			else if (!strcmp(fieldName,"TradingRight"))
			{
				TradingRight=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableTradingRight::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableTradingRight),output)!=sizeof(CWriteableTradingRight))
		return 0;
	else
		return 1;
}

int CWriteableTradingRight::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradingRight.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableTradingRight::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradingRight.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableTradingRight::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCTradingRight%s = {\n", index_buf);
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());	
	fprintf(fp,"\t\tTradingRight = %s    [交易权限]\n",TradingRight.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableTradingRight::dumpDiff(FILE *fp, const CWriteableTradingRight *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCTradingRight = {\n");
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(Direction.getValue(),pOriginal->Direction.getValue())!=0)
		fprintf(fp,"\t\tDirection = %s -> %s    [买卖方向]\n",pOriginal->Direction.getString().c_str(),Direction.getString().c_str());
	else
		fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());
	if (compare(TradingRight.getValue(),pOriginal->TradingRight.getValue())!=0)
		fprintf(fp,"\t\tTradingRight = %s -> %s    [交易权限]\n",pOriginal->TradingRight.getString().c_str(),TradingRight.getString().c_str());
	else
		fprintf(fp,"\t\tTradingRight = %s    [交易权限]\n",TradingRight.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableTradingRight::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",Direction=%s",Direction.getString().c_str());	
	fprintf(fp,",TradingRight=%s",TradingRight.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableTradingRight::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Direction=%s,",Direction.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradingRight=%s,",TradingRight.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CTradingRightIterator::dump(FILE *output)
{
	CTradingRight *pTradingRight;
	while ((pTradingRight=next())!=NULL)
	{
		pTradingRight->dump(output);
	}
}



void CWriteableMarginRate::writeCSVHead(FILE *output)
{
	fprintf(output,"\"MemberID*\",\"InstrumentID*\",\"ValueMode\",\"MarginSequenceType\",\"RiskSequenceType\",\"LongMarginRatio\",\"LongMaintMarginRatio\",\"LongMarginStep\",\"LongMaintMarginStep\",\"LongRiskLimit\",\"LongRiskStep\",\"ShortMarginRatio\",\"ShortMaintMarginRatio\",\"ShortMarginStep\",\"ShortMaintMarginStep\",\"ShortRiskLimit\",\"ShortRiskStep\",\"DefaultLeverage\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}

void CWriteableMarginRate::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"MemberID\",\"InstrumentID\",\"ValueMode\",\"MarginSequenceType\",\"RiskSequenceType\",\"LongMarginRatio\",\"LongMaintMarginRatio\",\"LongMarginStep\",\"LongMaintMarginStep\",\"LongRiskLimit\",\"LongRiskStep\",\"ShortMarginRatio\",\"ShortMaintMarginRatio\",\"ShortMarginStep\",\"ShortMaintMarginStep\",\"ShortRiskLimit\",\"ShortRiskStep\",\"DefaultLeverage\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}
	
void CWriteableMarginRate::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	MemberID.init();
	InstrumentID.init();
	ValueMode.init();
	MarginSequenceType.clear();
	RiskSequenceType.clear();
	LongMarginRatio.init();
	LongMaintMarginRatio.init();
	LongMarginStep.init();
	LongMaintMarginStep.init();
	LongRiskLimit.init();
	LongRiskStep.init();
	ShortMarginRatio.init();
	ShortMaintMarginRatio.init();
	ShortMarginStep.init();
	ShortMaintMarginStep.init();
	ShortRiskLimit.init();
	ShortRiskStep.init();
	DefaultLeverage.clear();
	Remark.clear();
	InsertTime.clear();
	UpdateTime.clear();
}

void CWriteableMarginRate::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableMarginRate::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableMarginRate),input)!=sizeof(CWriteableMarginRate))
		return 0;
	else
		return 1;
}
	
int CWriteableMarginRate::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ValueMode=token;
		else
			ValueMode="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MarginSequenceType=token;
		else
			MarginSequenceType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			RiskSequenceType=token;
		else
			RiskSequenceType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LongMarginRatio=token;
		else
			LongMarginRatio="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LongMaintMarginRatio=token;
		else
			LongMaintMarginRatio="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LongMarginStep=token;
		else
			LongMarginStep="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LongMaintMarginStep=token;
		else
			LongMaintMarginStep="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LongRiskLimit=token;
		else
			LongRiskLimit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LongRiskStep=token;
		else
			LongRiskStep="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ShortMarginRatio=token;
		else
			ShortMarginRatio="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ShortMaintMarginRatio=token;
		else
			ShortMaintMarginRatio="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ShortMarginStep=token;
		else
			ShortMarginStep="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ShortMaintMarginStep=token;
		else
			ShortMaintMarginStep="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ShortRiskLimit=token;
		else
			ShortRiskLimit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ShortRiskStep=token;
		else
			ShortRiskStep="";
		token=getNextToken(NULL);
		if (token!=NULL)
			DefaultLeverage=token;
		else
			DefaultLeverage="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"ValueMode"))
			{
				ValueMode=token;
			}
			else if (!strcmp(fieldName,"MarginSequenceType"))
			{
				MarginSequenceType=token;
			}
			else if (!strcmp(fieldName,"RiskSequenceType"))
			{
				RiskSequenceType=token;
			}
			else if (!strcmp(fieldName,"LongMarginRatio"))
			{
				LongMarginRatio=token;
			}
			else if (!strcmp(fieldName,"LongMaintMarginRatio"))
			{
				LongMaintMarginRatio=token;
			}
			else if (!strcmp(fieldName,"LongMarginStep"))
			{
				LongMarginStep=token;
			}
			else if (!strcmp(fieldName,"LongMaintMarginStep"))
			{
				LongMaintMarginStep=token;
			}
			else if (!strcmp(fieldName,"LongRiskLimit"))
			{
				LongRiskLimit=token;
			}
			else if (!strcmp(fieldName,"LongRiskStep"))
			{
				LongRiskStep=token;
			}
			else if (!strcmp(fieldName,"ShortMarginRatio"))
			{
				ShortMarginRatio=token;
			}
			else if (!strcmp(fieldName,"ShortMaintMarginRatio"))
			{
				ShortMaintMarginRatio=token;
			}
			else if (!strcmp(fieldName,"ShortMarginStep"))
			{
				ShortMarginStep=token;
			}
			else if (!strcmp(fieldName,"ShortMaintMarginStep"))
			{
				ShortMaintMarginStep=token;
			}
			else if (!strcmp(fieldName,"ShortRiskLimit"))
			{
				ShortRiskLimit=token;
			}
			else if (!strcmp(fieldName,"ShortRiskStep"))
			{
				ShortRiskStep=token;
			}
			else if (!strcmp(fieldName,"DefaultLeverage"))
			{
				DefaultLeverage=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableMarginRate::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableMarginRate),output)!=sizeof(CWriteableMarginRate))
		return 0;
	else
		return 1;
}

int CWriteableMarginRate::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ValueMode.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginSequenceType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",RiskSequenceType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongMarginRatio.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongMaintMarginRatio.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongMarginStep.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongMaintMarginStep.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongRiskLimit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongRiskStep.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortMarginRatio.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortMaintMarginRatio.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortMarginStep.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortMaintMarginStep.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortRiskLimit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortRiskStep.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",DefaultLeverage.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableMarginRate::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ValueMode.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginSequenceType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",RiskSequenceType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongMarginRatio.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongMaintMarginRatio.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongMarginStep.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongMaintMarginStep.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongRiskLimit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongRiskStep.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortMarginRatio.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortMaintMarginRatio.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortMarginStep.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortMaintMarginStep.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortRiskLimit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortRiskStep.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",DefaultLeverage.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableMarginRate::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCMarginRate%s = {\n", index_buf);
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tValueMode = %s    [取值方式]\n",ValueMode.getString().c_str());	
	fprintf(fp,"\t\tMarginSequenceType = %s    [保证金数列]\n",MarginSequenceType.getString().c_str());	
	fprintf(fp,"\t\tRiskSequenceType = %s    [风险限额数列]\n",RiskSequenceType.getString().c_str());	
	fprintf(fp,"\t\tLongMarginRatio = %s    [多头起始保证金率]\n",LongMarginRatio.getString().c_str());	
	fprintf(fp,"\t\tLongMaintMarginRatio = %s    [多头起始维持保证金率]\n",LongMaintMarginRatio.getString().c_str());	
	fprintf(fp,"\t\tLongMarginStep = %s    [多头保证金率步长]\n",LongMarginStep.getString().c_str());	
	fprintf(fp,"\t\tLongMaintMarginStep = %s    [多头维持保证金率步长]\n",LongMaintMarginStep.getString().c_str());	
	fprintf(fp,"\t\tLongRiskLimit = %s    [多头起始风险限额]\n",LongRiskLimit.getString().c_str());	
	fprintf(fp,"\t\tLongRiskStep = %s    [多头风险限额步长]\n",LongRiskStep.getString().c_str());	
	fprintf(fp,"\t\tShortMarginRatio = %s    [空头起始保证金率]\n",ShortMarginRatio.getString().c_str());	
	fprintf(fp,"\t\tShortMaintMarginRatio = %s    [空头起始维持保证金率]\n",ShortMaintMarginRatio.getString().c_str());	
	fprintf(fp,"\t\tShortMarginStep = %s    [空头保证金率步长]\n",ShortMarginStep.getString().c_str());	
	fprintf(fp,"\t\tShortMaintMarginStep = %s    [空头维持保证金率步长]\n",ShortMaintMarginStep.getString().c_str());	
	fprintf(fp,"\t\tShortRiskLimit = %s    [空头起始风险限额]\n",ShortRiskLimit.getString().c_str());	
	fprintf(fp,"\t\tShortRiskStep = %s    [空头风险限额步长]\n",ShortRiskStep.getString().c_str());	
	fprintf(fp,"\t\tDefaultLeverage = %s    [默认杠杆倍数]\n",DefaultLeverage.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableMarginRate::dumpDiff(FILE *fp, const CWriteableMarginRate *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCMarginRate = {\n");
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(ValueMode.getValue(),pOriginal->ValueMode.getValue())!=0)
		fprintf(fp,"\t\tValueMode = %s -> %s    [取值方式]\n",pOriginal->ValueMode.getString().c_str(),ValueMode.getString().c_str());
	else
		fprintf(fp,"\t\tValueMode = %s    [取值方式]\n",ValueMode.getString().c_str());
	if (compare(MarginSequenceType.getValue(),pOriginal->MarginSequenceType.getValue())!=0)
		fprintf(fp,"\t\tMarginSequenceType = %s -> %s    [保证金数列]\n",pOriginal->MarginSequenceType.getString().c_str(),MarginSequenceType.getString().c_str());
	else
		fprintf(fp,"\t\tMarginSequenceType = %s    [保证金数列]\n",MarginSequenceType.getString().c_str());
	if (compare(RiskSequenceType.getValue(),pOriginal->RiskSequenceType.getValue())!=0)
		fprintf(fp,"\t\tRiskSequenceType = %s -> %s    [风险限额数列]\n",pOriginal->RiskSequenceType.getString().c_str(),RiskSequenceType.getString().c_str());
	else
		fprintf(fp,"\t\tRiskSequenceType = %s    [风险限额数列]\n",RiskSequenceType.getString().c_str());
	if (compare(LongMarginRatio.getValue(),pOriginal->LongMarginRatio.getValue())!=0)
		fprintf(fp,"\t\tLongMarginRatio = %s -> %s    [多头起始保证金率]\n",pOriginal->LongMarginRatio.getString().c_str(),LongMarginRatio.getString().c_str());
	else
		fprintf(fp,"\t\tLongMarginRatio = %s    [多头起始保证金率]\n",LongMarginRatio.getString().c_str());
	if (compare(LongMaintMarginRatio.getValue(),pOriginal->LongMaintMarginRatio.getValue())!=0)
		fprintf(fp,"\t\tLongMaintMarginRatio = %s -> %s    [多头起始维持保证金率]\n",pOriginal->LongMaintMarginRatio.getString().c_str(),LongMaintMarginRatio.getString().c_str());
	else
		fprintf(fp,"\t\tLongMaintMarginRatio = %s    [多头起始维持保证金率]\n",LongMaintMarginRatio.getString().c_str());
	if (compare(LongMarginStep.getValue(),pOriginal->LongMarginStep.getValue())!=0)
		fprintf(fp,"\t\tLongMarginStep = %s -> %s    [多头保证金率步长]\n",pOriginal->LongMarginStep.getString().c_str(),LongMarginStep.getString().c_str());
	else
		fprintf(fp,"\t\tLongMarginStep = %s    [多头保证金率步长]\n",LongMarginStep.getString().c_str());
	if (compare(LongMaintMarginStep.getValue(),pOriginal->LongMaintMarginStep.getValue())!=0)
		fprintf(fp,"\t\tLongMaintMarginStep = %s -> %s    [多头维持保证金率步长]\n",pOriginal->LongMaintMarginStep.getString().c_str(),LongMaintMarginStep.getString().c_str());
	else
		fprintf(fp,"\t\tLongMaintMarginStep = %s    [多头维持保证金率步长]\n",LongMaintMarginStep.getString().c_str());
	if (compare(LongRiskLimit.getValue(),pOriginal->LongRiskLimit.getValue())!=0)
		fprintf(fp,"\t\tLongRiskLimit = %s -> %s    [多头起始风险限额]\n",pOriginal->LongRiskLimit.getString().c_str(),LongRiskLimit.getString().c_str());
	else
		fprintf(fp,"\t\tLongRiskLimit = %s    [多头起始风险限额]\n",LongRiskLimit.getString().c_str());
	if (compare(LongRiskStep.getValue(),pOriginal->LongRiskStep.getValue())!=0)
		fprintf(fp,"\t\tLongRiskStep = %s -> %s    [多头风险限额步长]\n",pOriginal->LongRiskStep.getString().c_str(),LongRiskStep.getString().c_str());
	else
		fprintf(fp,"\t\tLongRiskStep = %s    [多头风险限额步长]\n",LongRiskStep.getString().c_str());
	if (compare(ShortMarginRatio.getValue(),pOriginal->ShortMarginRatio.getValue())!=0)
		fprintf(fp,"\t\tShortMarginRatio = %s -> %s    [空头起始保证金率]\n",pOriginal->ShortMarginRatio.getString().c_str(),ShortMarginRatio.getString().c_str());
	else
		fprintf(fp,"\t\tShortMarginRatio = %s    [空头起始保证金率]\n",ShortMarginRatio.getString().c_str());
	if (compare(ShortMaintMarginRatio.getValue(),pOriginal->ShortMaintMarginRatio.getValue())!=0)
		fprintf(fp,"\t\tShortMaintMarginRatio = %s -> %s    [空头起始维持保证金率]\n",pOriginal->ShortMaintMarginRatio.getString().c_str(),ShortMaintMarginRatio.getString().c_str());
	else
		fprintf(fp,"\t\tShortMaintMarginRatio = %s    [空头起始维持保证金率]\n",ShortMaintMarginRatio.getString().c_str());
	if (compare(ShortMarginStep.getValue(),pOriginal->ShortMarginStep.getValue())!=0)
		fprintf(fp,"\t\tShortMarginStep = %s -> %s    [空头保证金率步长]\n",pOriginal->ShortMarginStep.getString().c_str(),ShortMarginStep.getString().c_str());
	else
		fprintf(fp,"\t\tShortMarginStep = %s    [空头保证金率步长]\n",ShortMarginStep.getString().c_str());
	if (compare(ShortMaintMarginStep.getValue(),pOriginal->ShortMaintMarginStep.getValue())!=0)
		fprintf(fp,"\t\tShortMaintMarginStep = %s -> %s    [空头维持保证金率步长]\n",pOriginal->ShortMaintMarginStep.getString().c_str(),ShortMaintMarginStep.getString().c_str());
	else
		fprintf(fp,"\t\tShortMaintMarginStep = %s    [空头维持保证金率步长]\n",ShortMaintMarginStep.getString().c_str());
	if (compare(ShortRiskLimit.getValue(),pOriginal->ShortRiskLimit.getValue())!=0)
		fprintf(fp,"\t\tShortRiskLimit = %s -> %s    [空头起始风险限额]\n",pOriginal->ShortRiskLimit.getString().c_str(),ShortRiskLimit.getString().c_str());
	else
		fprintf(fp,"\t\tShortRiskLimit = %s    [空头起始风险限额]\n",ShortRiskLimit.getString().c_str());
	if (compare(ShortRiskStep.getValue(),pOriginal->ShortRiskStep.getValue())!=0)
		fprintf(fp,"\t\tShortRiskStep = %s -> %s    [空头风险限额步长]\n",pOriginal->ShortRiskStep.getString().c_str(),ShortRiskStep.getString().c_str());
	else
		fprintf(fp,"\t\tShortRiskStep = %s    [空头风险限额步长]\n",ShortRiskStep.getString().c_str());
	if (compare(DefaultLeverage.getValue(),pOriginal->DefaultLeverage.getValue())!=0)
		fprintf(fp,"\t\tDefaultLeverage = %s -> %s    [默认杠杆倍数]\n",pOriginal->DefaultLeverage.getString().c_str(),DefaultLeverage.getString().c_str());
	else
		fprintf(fp,"\t\tDefaultLeverage = %s    [默认杠杆倍数]\n",DefaultLeverage.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableMarginRate::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",ValueMode=%s",ValueMode.getString().c_str());	
	fprintf(fp,",MarginSequenceType=%s",MarginSequenceType.getString().c_str());	
	fprintf(fp,",RiskSequenceType=%s",RiskSequenceType.getString().c_str());	
	fprintf(fp,",LongMarginRatio=%s",LongMarginRatio.getString().c_str());	
	fprintf(fp,",LongMaintMarginRatio=%s",LongMaintMarginRatio.getString().c_str());	
	fprintf(fp,",LongMarginStep=%s",LongMarginStep.getString().c_str());	
	fprintf(fp,",LongMaintMarginStep=%s",LongMaintMarginStep.getString().c_str());	
	fprintf(fp,",LongRiskLimit=%s",LongRiskLimit.getString().c_str());	
	fprintf(fp,",LongRiskStep=%s",LongRiskStep.getString().c_str());	
	fprintf(fp,",ShortMarginRatio=%s",ShortMarginRatio.getString().c_str());	
	fprintf(fp,",ShortMaintMarginRatio=%s",ShortMaintMarginRatio.getString().c_str());	
	fprintf(fp,",ShortMarginStep=%s",ShortMarginStep.getString().c_str());	
	fprintf(fp,",ShortMaintMarginStep=%s",ShortMaintMarginStep.getString().c_str());	
	fprintf(fp,",ShortRiskLimit=%s",ShortRiskLimit.getString().c_str());	
	fprintf(fp,",ShortRiskStep=%s",ShortRiskStep.getString().c_str());	
	fprintf(fp,",DefaultLeverage=%s",DefaultLeverage.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableMarginRate::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ValueMode=%s,",ValueMode.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MarginSequenceType=%s,",MarginSequenceType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"RiskSequenceType=%s,",RiskSequenceType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LongMarginRatio=%s,",LongMarginRatio.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LongMaintMarginRatio=%s,",LongMaintMarginRatio.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LongMarginStep=%s,",LongMarginStep.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LongMaintMarginStep=%s,",LongMaintMarginStep.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LongRiskLimit=%s,",LongRiskLimit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LongRiskStep=%s,",LongRiskStep.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ShortMarginRatio=%s,",ShortMarginRatio.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ShortMaintMarginRatio=%s,",ShortMaintMarginRatio.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ShortMarginStep=%s,",ShortMarginStep.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ShortMaintMarginStep=%s,",ShortMaintMarginStep.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ShortRiskLimit=%s,",ShortRiskLimit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ShortRiskStep=%s,",ShortRiskStep.getString().c_str());
	p+=strlen(p);
	sprintf(p,"DefaultLeverage=%s,",DefaultLeverage.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CMarginRateIterator::dump(FILE *output)
{
	CMarginRate *pMarginRate;
	while ((pMarginRate=next())!=NULL)
	{
		pMarginRate->dump(output);
	}
}



void CWriteableCFDRate::writeCSVHead(FILE *output)
{
	fprintf(output,"\"MemberID*\",\"InstrumentID*\",\"TriggerOrderType*\",\"RateBegin\",\"RateEnd\",\"CompVolume\",\"CompRate\",\"HardRate\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}

void CWriteableCFDRate::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"MemberID\",\"InstrumentID\",\"TriggerOrderType\",\"RateBegin\",\"RateEnd\",\"CompVolume\",\"CompRate\",\"HardRate\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}
	
void CWriteableCFDRate::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	MemberID.init();
	InstrumentID.init();
	TriggerOrderType.setValue("default");
	RateBegin.init();
	RateEnd.init();
	CompVolume.init();
	CompRate.clear();
	HardRate.clear();
	Remark.clear();
	InsertTime.clear();
	UpdateTime.clear();
}

void CWriteableCFDRate::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableCFDRate::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableCFDRate),input)!=sizeof(CWriteableCFDRate))
		return 0;
	else
		return 1;
}
	
int CWriteableCFDRate::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerOrderType=token;
		else
			TriggerOrderType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			RateBegin=token;
		else
			RateBegin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			RateEnd=token;
		else
			RateEnd="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CompVolume=token;
		else
			CompVolume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CompRate=token;
		else
			CompRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			HardRate=token;
		else
			HardRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"TriggerOrderType"))
			{
				TriggerOrderType=token;
			}
			else if (!strcmp(fieldName,"RateBegin"))
			{
				RateBegin=token;
			}
			else if (!strcmp(fieldName,"RateEnd"))
			{
				RateEnd=token;
			}
			else if (!strcmp(fieldName,"CompVolume"))
			{
				CompVolume=token;
			}
			else if (!strcmp(fieldName,"CompRate"))
			{
				CompRate=token;
			}
			else if (!strcmp(fieldName,"HardRate"))
			{
				HardRate=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableCFDRate::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableCFDRate),output)!=sizeof(CWriteableCFDRate))
		return 0;
	else
		return 1;
}

int CWriteableCFDRate::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerOrderType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",RateBegin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",RateEnd.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CompVolume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CompRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",HardRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableCFDRate::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerOrderType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",RateBegin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",RateEnd.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CompVolume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CompRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",HardRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableCFDRate::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCCFDRate%s = {\n", index_buf);
	fprintf(fp,"\t\tMemberID = %s    [成员代码或组]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tTriggerOrderType = %s    [报单价格类型]\n",TriggerOrderType.getString().c_str());	
	fprintf(fp,"\t\tRateBegin = %s    [多头起始溢价率]\n",RateBegin.getString().c_str());	
	fprintf(fp,"\t\tRateEnd = %s    [多头最大溢价率]\n",RateEnd.getString().c_str());	
	fprintf(fp,"\t\tCompVolume = %s    [点差比较基准]\n",CompVolume.getString().c_str());	
	fprintf(fp,"\t\tCompRate = %s    [点差比较基准率]\n",CompRate.getString().c_str());	
	fprintf(fp,"\t\tHardRate = %s    [硬顶率]\n",HardRate.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableCFDRate::dumpDiff(FILE *fp, const CWriteableCFDRate *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCCFDRate = {\n");
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码或组]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码或组]\n",MemberID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(TriggerOrderType.getValue(),pOriginal->TriggerOrderType.getValue())!=0)
		fprintf(fp,"\t\tTriggerOrderType = %s -> %s    [报单价格类型]\n",pOriginal->TriggerOrderType.getString().c_str(),TriggerOrderType.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerOrderType = %s    [报单价格类型]\n",TriggerOrderType.getString().c_str());
	if (compare(RateBegin.getValue(),pOriginal->RateBegin.getValue())!=0)
		fprintf(fp,"\t\tRateBegin = %s -> %s    [多头起始溢价率]\n",pOriginal->RateBegin.getString().c_str(),RateBegin.getString().c_str());
	else
		fprintf(fp,"\t\tRateBegin = %s    [多头起始溢价率]\n",RateBegin.getString().c_str());
	if (compare(RateEnd.getValue(),pOriginal->RateEnd.getValue())!=0)
		fprintf(fp,"\t\tRateEnd = %s -> %s    [多头最大溢价率]\n",pOriginal->RateEnd.getString().c_str(),RateEnd.getString().c_str());
	else
		fprintf(fp,"\t\tRateEnd = %s    [多头最大溢价率]\n",RateEnd.getString().c_str());
	if (compare(CompVolume.getValue(),pOriginal->CompVolume.getValue())!=0)
		fprintf(fp,"\t\tCompVolume = %s -> %s    [点差比较基准]\n",pOriginal->CompVolume.getString().c_str(),CompVolume.getString().c_str());
	else
		fprintf(fp,"\t\tCompVolume = %s    [点差比较基准]\n",CompVolume.getString().c_str());
	if (compare(CompRate.getValue(),pOriginal->CompRate.getValue())!=0)
		fprintf(fp,"\t\tCompRate = %s -> %s    [点差比较基准率]\n",pOriginal->CompRate.getString().c_str(),CompRate.getString().c_str());
	else
		fprintf(fp,"\t\tCompRate = %s    [点差比较基准率]\n",CompRate.getString().c_str());
	if (compare(HardRate.getValue(),pOriginal->HardRate.getValue())!=0)
		fprintf(fp,"\t\tHardRate = %s -> %s    [硬顶率]\n",pOriginal->HardRate.getString().c_str(),HardRate.getString().c_str());
	else
		fprintf(fp,"\t\tHardRate = %s    [硬顶率]\n",HardRate.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableCFDRate::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",TriggerOrderType=%s",TriggerOrderType.getString().c_str());	
	fprintf(fp,",RateBegin=%s",RateBegin.getString().c_str());	
	fprintf(fp,",RateEnd=%s",RateEnd.getString().c_str());	
	fprintf(fp,",CompVolume=%s",CompVolume.getString().c_str());	
	fprintf(fp,",CompRate=%s",CompRate.getString().c_str());	
	fprintf(fp,",HardRate=%s",HardRate.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableCFDRate::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerOrderType=%s,",TriggerOrderType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"RateBegin=%s,",RateBegin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"RateEnd=%s,",RateEnd.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CompVolume=%s,",CompVolume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CompRate=%s,",CompRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"HardRate=%s,",HardRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CCFDRateIterator::dump(FILE *output)
{
	CCFDRate *pCFDRate;
	while ((pCFDRate=next())!=NULL)
	{
		pCFDRate->dump(output);
	}
}



void CWriteableFee::writeCSVHead(FILE *output)
{
	fprintf(output,"\"MemberID*\",\"InstrumentID*\",\"MatchRole*\",\"OpenFeeRate\",\"OpenFeeAmount\",\"CloseFeeRate\",\"CloseFeeAmount\",\"CloseTodayFeeRate\",\"CloseTodayFeeAmount\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}

void CWriteableFee::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"MemberID\",\"InstrumentID\",\"MatchRole\",\"OpenFeeRate\",\"OpenFeeAmount\",\"CloseFeeRate\",\"CloseFeeAmount\",\"CloseTodayFeeRate\",\"CloseTodayFeeAmount\",\"Remark\",\"InsertTime\",\"UpdateTime\"\n");
}
	
void CWriteableFee::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	MemberID.init();
	InstrumentID.init();
	MatchRole.init();
	OpenFeeRate.init();
	OpenFeeAmount.init();
	CloseFeeRate.init();
	CloseFeeAmount.init();
	CloseTodayFeeRate.init();
	CloseTodayFeeAmount.init();
	Remark.clear();
	InsertTime.clear();
	UpdateTime.clear();
}

void CWriteableFee::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableFee::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableFee),input)!=sizeof(CWriteableFee))
		return 0;
	else
		return 1;
}
	
int CWriteableFee::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MatchRole=token;
		else
			MatchRole="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenFeeRate=token;
		else
			OpenFeeRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenFeeAmount=token;
		else
			OpenFeeAmount="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseFeeRate=token;
		else
			CloseFeeRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseFeeAmount=token;
		else
			CloseFeeAmount="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseTodayFeeRate=token;
		else
			CloseTodayFeeRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseTodayFeeAmount=token;
		else
			CloseTodayFeeAmount="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"MatchRole"))
			{
				MatchRole=token;
			}
			else if (!strcmp(fieldName,"OpenFeeRate"))
			{
				OpenFeeRate=token;
			}
			else if (!strcmp(fieldName,"OpenFeeAmount"))
			{
				OpenFeeAmount=token;
			}
			else if (!strcmp(fieldName,"CloseFeeRate"))
			{
				CloseFeeRate=token;
			}
			else if (!strcmp(fieldName,"CloseFeeAmount"))
			{
				CloseFeeAmount=token;
			}
			else if (!strcmp(fieldName,"CloseTodayFeeRate"))
			{
				CloseTodayFeeRate=token;
			}
			else if (!strcmp(fieldName,"CloseTodayFeeAmount"))
			{
				CloseTodayFeeAmount=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableFee::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableFee),output)!=sizeof(CWriteableFee))
		return 0;
	else
		return 1;
}

int CWriteableFee::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MatchRole.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenFeeRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenFeeAmount.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseFeeRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseFeeAmount.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseTodayFeeRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseTodayFeeAmount.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableFee::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MatchRole.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenFeeRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenFeeAmount.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseFeeRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseFeeAmount.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseTodayFeeRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseTodayFeeAmount.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableFee::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCFee%s = {\n", index_buf);
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tMatchRole = %s    [成交角色]\n",MatchRole.getString().c_str());	
	fprintf(fp,"\t\tOpenFeeRate = %s    [开仓手续费按比例]\n",OpenFeeRate.getString().c_str());	
	fprintf(fp,"\t\tOpenFeeAmount = %s    [开仓手续费按手数]\n",OpenFeeAmount.getString().c_str());	
	fprintf(fp,"\t\tCloseFeeRate = %s    [平仓手续费按比例]\n",CloseFeeRate.getString().c_str());	
	fprintf(fp,"\t\tCloseFeeAmount = %s    [平仓手续费按手数]\n",CloseFeeAmount.getString().c_str());	
	fprintf(fp,"\t\tCloseTodayFeeRate = %s    [平今仓手续费按比例]\n",CloseTodayFeeRate.getString().c_str());	
	fprintf(fp,"\t\tCloseTodayFeeAmount = %s    [平今仓手续费按手数]\n",CloseTodayFeeAmount.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableFee::dumpDiff(FILE *fp, const CWriteableFee *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCFee = {\n");
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(MatchRole.getValue(),pOriginal->MatchRole.getValue())!=0)
		fprintf(fp,"\t\tMatchRole = %s -> %s    [成交角色]\n",pOriginal->MatchRole.getString().c_str(),MatchRole.getString().c_str());
	else
		fprintf(fp,"\t\tMatchRole = %s    [成交角色]\n",MatchRole.getString().c_str());
	if (compare(OpenFeeRate.getValue(),pOriginal->OpenFeeRate.getValue())!=0)
		fprintf(fp,"\t\tOpenFeeRate = %s -> %s    [开仓手续费按比例]\n",pOriginal->OpenFeeRate.getString().c_str(),OpenFeeRate.getString().c_str());
	else
		fprintf(fp,"\t\tOpenFeeRate = %s    [开仓手续费按比例]\n",OpenFeeRate.getString().c_str());
	if (compare(OpenFeeAmount.getValue(),pOriginal->OpenFeeAmount.getValue())!=0)
		fprintf(fp,"\t\tOpenFeeAmount = %s -> %s    [开仓手续费按手数]\n",pOriginal->OpenFeeAmount.getString().c_str(),OpenFeeAmount.getString().c_str());
	else
		fprintf(fp,"\t\tOpenFeeAmount = %s    [开仓手续费按手数]\n",OpenFeeAmount.getString().c_str());
	if (compare(CloseFeeRate.getValue(),pOriginal->CloseFeeRate.getValue())!=0)
		fprintf(fp,"\t\tCloseFeeRate = %s -> %s    [平仓手续费按比例]\n",pOriginal->CloseFeeRate.getString().c_str(),CloseFeeRate.getString().c_str());
	else
		fprintf(fp,"\t\tCloseFeeRate = %s    [平仓手续费按比例]\n",CloseFeeRate.getString().c_str());
	if (compare(CloseFeeAmount.getValue(),pOriginal->CloseFeeAmount.getValue())!=0)
		fprintf(fp,"\t\tCloseFeeAmount = %s -> %s    [平仓手续费按手数]\n",pOriginal->CloseFeeAmount.getString().c_str(),CloseFeeAmount.getString().c_str());
	else
		fprintf(fp,"\t\tCloseFeeAmount = %s    [平仓手续费按手数]\n",CloseFeeAmount.getString().c_str());
	if (compare(CloseTodayFeeRate.getValue(),pOriginal->CloseTodayFeeRate.getValue())!=0)
		fprintf(fp,"\t\tCloseTodayFeeRate = %s -> %s    [平今仓手续费按比例]\n",pOriginal->CloseTodayFeeRate.getString().c_str(),CloseTodayFeeRate.getString().c_str());
	else
		fprintf(fp,"\t\tCloseTodayFeeRate = %s    [平今仓手续费按比例]\n",CloseTodayFeeRate.getString().c_str());
	if (compare(CloseTodayFeeAmount.getValue(),pOriginal->CloseTodayFeeAmount.getValue())!=0)
		fprintf(fp,"\t\tCloseTodayFeeAmount = %s -> %s    [平今仓手续费按手数]\n",pOriginal->CloseTodayFeeAmount.getString().c_str(),CloseTodayFeeAmount.getString().c_str());
	else
		fprintf(fp,"\t\tCloseTodayFeeAmount = %s    [平今仓手续费按手数]\n",CloseTodayFeeAmount.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableFee::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",MatchRole=%s",MatchRole.getString().c_str());	
	fprintf(fp,",OpenFeeRate=%s",OpenFeeRate.getString().c_str());	
	fprintf(fp,",OpenFeeAmount=%s",OpenFeeAmount.getString().c_str());	
	fprintf(fp,",CloseFeeRate=%s",CloseFeeRate.getString().c_str());	
	fprintf(fp,",CloseFeeAmount=%s",CloseFeeAmount.getString().c_str());	
	fprintf(fp,",CloseTodayFeeRate=%s",CloseTodayFeeRate.getString().c_str());	
	fprintf(fp,",CloseTodayFeeAmount=%s",CloseTodayFeeAmount.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableFee::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MatchRole=%s,",MatchRole.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenFeeRate=%s,",OpenFeeRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenFeeAmount=%s,",OpenFeeAmount.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseFeeRate=%s,",CloseFeeRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseFeeAmount=%s,",CloseFeeAmount.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseTodayFeeRate=%s,",CloseTodayFeeRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseTodayFeeAmount=%s,",CloseTodayFeeAmount.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CFeeIterator::dump(FILE *output)
{
	CFee *pFee;
	while ((pFee=next())!=NULL)
	{
		pFee->dump(output);
	}
}



void CWriteableCurrency::writeCSVHead(FILE *output)
{
	fprintf(output,"\"SettlementGroup*\",\"Currency*\",\"Deposit\",\"Withdraw\",\"Balance\",\"PreBalance\",\"SortNumber\",\"CanReduce\",\"IsTrading\",\"SettleSegment\",\"MoneyTick\",\"ExchangeID\",\"InstrumentID\",\"LoanUse\",\"LoanLimit\",\"LoanBorrow\",\"LoanDeposit\",\"LoanBase\",\"LoanRate\",\"LoanHighBase\",\"LoanHighRate\",\"Remark\",\"CurrValue\",\"HairCut\",\"MaxValue\",\"MarginRateGroup\",\"PrivateEnd\",\"InsertTime\",\"UpdateTime\"\n");
}

void CWriteableCurrency::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"SettlementGroup\",\"Currency\",\"Deposit\",\"Withdraw\",\"Balance\",\"PreBalance\",\"SortNumber\",\"CanReduce\",\"IsTrading\",\"SettleSegment\",\"MoneyTick\",\"ExchangeID\",\"InstrumentID\",\"LoanUse\",\"LoanLimit\",\"LoanBorrow\",\"LoanDeposit\",\"LoanBase\",\"LoanRate\",\"LoanHighBase\",\"LoanHighRate\",\"Remark\",\"CurrValue\",\"HairCut\",\"MaxValue\",\"MarginRateGroup\",\"PrivateEnd\",\"InsertTime\",\"UpdateTime\"\n");
}
	
void CWriteableCurrency::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	SettlementGroup.init();
	Currency.init();
	Deposit.init();
	Withdraw.init();
	Balance.init();
	PreBalance.init();
	SortNumber.clear();
	CanReduce.setValue(1);
	IsTrading.setValue(1);
	SettleSegment.clear();
	MoneyTick.setValue(0.00000001);
	ExchangeID.init();
	InstrumentID.init();
	LoanUse.init();
	LoanLimit.setValue(0.8);
	LoanBorrow.init();
	LoanDeposit.init();
	LoanBase.setValue(0.01);
	LoanRate.setValue(0.1);
	LoanHighBase.setValue(0.9);
	LoanHighRate.setValue(1);
	Remark.clear();
	CurrValue.init();
	HairCut.setValue(0.9);
	MaxValue.setValue(10000);
	MarginRateGroup.clear();
	PrivateEnd.clear();
	InsertTime.clear();
	UpdateTime.clear();
}

void CWriteableCurrency::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableCurrency::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableCurrency),input)!=sizeof(CWriteableCurrency))
		return 0;
	else
		return 1;
}
	
int CWriteableCurrency::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			SettlementGroup=token;
		else
			SettlementGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Currency=token;
		else
			Currency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Deposit=token;
		else
			Deposit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Withdraw=token;
		else
			Withdraw="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Balance=token;
		else
			Balance="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PreBalance=token;
		else
			PreBalance="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SortNumber=token;
		else
			SortNumber="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CanReduce=token;
		else
			CanReduce="";
		token=getNextToken(NULL);
		if (token!=NULL)
			IsTrading=token;
		else
			IsTrading="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettleSegment=token;
		else
			SettleSegment="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MoneyTick=token;
		else
			MoneyTick="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LoanUse=token;
		else
			LoanUse="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LoanLimit=token;
		else
			LoanLimit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LoanBorrow=token;
		else
			LoanBorrow="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LoanDeposit=token;
		else
			LoanDeposit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LoanBase=token;
		else
			LoanBase="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LoanRate=token;
		else
			LoanRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LoanHighBase=token;
		else
			LoanHighBase="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LoanHighRate=token;
		else
			LoanHighRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CurrValue=token;
		else
			CurrValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			HairCut=token;
		else
			HairCut="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MaxValue=token;
		else
			MaxValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MarginRateGroup=token;
		else
			MarginRateGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PrivateEnd=token;
		else
			PrivateEnd="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"SettlementGroup"))
			{
				SettlementGroup=token;
			}
			else if (!strcmp(fieldName,"Currency"))
			{
				Currency=token;
			}
			else if (!strcmp(fieldName,"Deposit"))
			{
				Deposit=token;
			}
			else if (!strcmp(fieldName,"Withdraw"))
			{
				Withdraw=token;
			}
			else if (!strcmp(fieldName,"Balance"))
			{
				Balance=token;
			}
			else if (!strcmp(fieldName,"PreBalance"))
			{
				PreBalance=token;
			}
			else if (!strcmp(fieldName,"SortNumber"))
			{
				SortNumber=token;
			}
			else if (!strcmp(fieldName,"CanReduce"))
			{
				CanReduce=token;
			}
			else if (!strcmp(fieldName,"IsTrading"))
			{
				IsTrading=token;
			}
			else if (!strcmp(fieldName,"SettleSegment"))
			{
				SettleSegment=token;
			}
			else if (!strcmp(fieldName,"MoneyTick"))
			{
				MoneyTick=token;
			}
			else if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"LoanUse"))
			{
				LoanUse=token;
			}
			else if (!strcmp(fieldName,"LoanLimit"))
			{
				LoanLimit=token;
			}
			else if (!strcmp(fieldName,"LoanBorrow"))
			{
				LoanBorrow=token;
			}
			else if (!strcmp(fieldName,"LoanDeposit"))
			{
				LoanDeposit=token;
			}
			else if (!strcmp(fieldName,"LoanBase"))
			{
				LoanBase=token;
			}
			else if (!strcmp(fieldName,"LoanRate"))
			{
				LoanRate=token;
			}
			else if (!strcmp(fieldName,"LoanHighBase"))
			{
				LoanHighBase=token;
			}
			else if (!strcmp(fieldName,"LoanHighRate"))
			{
				LoanHighRate=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"CurrValue"))
			{
				CurrValue=token;
			}
			else if (!strcmp(fieldName,"HairCut"))
			{
				HairCut=token;
			}
			else if (!strcmp(fieldName,"MaxValue"))
			{
				MaxValue=token;
			}
			else if (!strcmp(fieldName,"MarginRateGroup"))
			{
				MarginRateGroup=token;
			}
			else if (!strcmp(fieldName,"PrivateEnd"))
			{
				PrivateEnd=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableCurrency::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableCurrency),output)!=sizeof(CWriteableCurrency))
		return 0;
	else
		return 1;
}

int CWriteableCurrency::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Currency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Deposit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Withdraw.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Balance.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreBalance.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SortNumber.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CanReduce.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsTrading.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettleSegment.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MoneyTick.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanUse.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanLimit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanBorrow.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanDeposit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanBase.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanHighBase.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanHighRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CurrValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",HairCut.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginRateGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PrivateEnd.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableCurrency::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Currency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Deposit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Withdraw.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Balance.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreBalance.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SortNumber.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CanReduce.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsTrading.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettleSegment.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MoneyTick.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanUse.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanLimit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanBorrow.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanDeposit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanBase.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanHighBase.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LoanHighRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CurrValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",HairCut.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginRateGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PrivateEnd.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableCurrency::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCCurrency%s = {\n", index_buf);
	fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());	
	fprintf(fp,"\t\tCurrency = %s    [币种]\n",Currency.getString().c_str());	
	fprintf(fp,"\t\tDeposit = %s    [入金金额]\n",Deposit.getString().c_str());	
	fprintf(fp,"\t\tWithdraw = %s    [出金金额]\n",Withdraw.getString().c_str());	
	fprintf(fp,"\t\tBalance = %s    [静态权益]\n",Balance.getString().c_str());	
	fprintf(fp,"\t\tPreBalance = %s    [上次静态权益]\n",PreBalance.getString().c_str());	
	fprintf(fp,"\t\tSortNumber = %s    [排序字段]\n",SortNumber.getString().c_str());	
	fprintf(fp,"\t\tCanReduce = %s    [是否可以出金]\n",CanReduce.getString().c_str());	
	fprintf(fp,"\t\tIsTrading = %s    [是否允许相关交易]\n",IsTrading.getString().c_str());	
	fprintf(fp,"\t\tSettleSegment = %s    [结算段]\n",SettleSegment.getString().c_str());	
	fprintf(fp,"\t\tMoneyTick = %s    [最小单位]\n",MoneyTick.getString().c_str());	
	fprintf(fp,"\t\tExchangeID = %s    [价格参考交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [价格参考标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tLoanUse = %s    [借款使用量]\n",LoanUse.getString().c_str());	
	fprintf(fp,"\t\tLoanLimit = %s    [最大可借比例]\n",LoanLimit.getString().c_str());	
	fprintf(fp,"\t\tLoanBorrow = %s    [借款利息]\n",LoanBorrow.getString().c_str());	
	fprintf(fp,"\t\tLoanDeposit = %s    [存款利息]\n",LoanDeposit.getString().c_str());	
	fprintf(fp,"\t\tLoanBase = %s    [利息起点]\n",LoanBase.getString().c_str());	
	fprintf(fp,"\t\tLoanRate = %s    [利息斜率]\n",LoanRate.getString().c_str());	
	fprintf(fp,"\t\tLoanHighBase = %s    [高利息起点]\n",LoanHighBase.getString().c_str());	
	fprintf(fp,"\t\tLoanHighRate = %s    [高利息斜率]\n",LoanHighRate.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tCurrValue = %s    [当前价值]\n",CurrValue.getString().c_str());	
	fprintf(fp,"\t\tHairCut = %s    [抵押比例]\n",HairCut.getString().c_str());	
	fprintf(fp,"\t\tMaxValue = %s    [最大抵押数额]\n",MaxValue.getString().c_str());	
	fprintf(fp,"\t\tMarginRateGroup = %s    [保证金组]\n",MarginRateGroup.getString().c_str());	
	fprintf(fp,"\t\tPrivateEnd = %s    [私有化停止交易线]\n",PrivateEnd.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableCurrency::dumpDiff(FILE *fp, const CWriteableCurrency *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCCurrency = {\n");
	if (compare(SettlementGroup.getValue(),pOriginal->SettlementGroup.getValue())!=0)
		fprintf(fp,"\t\tSettlementGroup = %s -> %s    [结算组编号]\n",pOriginal->SettlementGroup.getString().c_str(),SettlementGroup.getString().c_str());
	else
		fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());
	if (compare(Currency.getValue(),pOriginal->Currency.getValue())!=0)
		fprintf(fp,"\t\tCurrency = %s -> %s    [币种]\n",pOriginal->Currency.getString().c_str(),Currency.getString().c_str());
	else
		fprintf(fp,"\t\tCurrency = %s    [币种]\n",Currency.getString().c_str());
	if (compare(Deposit.getValue(),pOriginal->Deposit.getValue())!=0)
		fprintf(fp,"\t\tDeposit = %s -> %s    [入金金额]\n",pOriginal->Deposit.getString().c_str(),Deposit.getString().c_str());
	else
		fprintf(fp,"\t\tDeposit = %s    [入金金额]\n",Deposit.getString().c_str());
	if (compare(Withdraw.getValue(),pOriginal->Withdraw.getValue())!=0)
		fprintf(fp,"\t\tWithdraw = %s -> %s    [出金金额]\n",pOriginal->Withdraw.getString().c_str(),Withdraw.getString().c_str());
	else
		fprintf(fp,"\t\tWithdraw = %s    [出金金额]\n",Withdraw.getString().c_str());
	if (compare(Balance.getValue(),pOriginal->Balance.getValue())!=0)
		fprintf(fp,"\t\tBalance = %s -> %s    [静态权益]\n",pOriginal->Balance.getString().c_str(),Balance.getString().c_str());
	else
		fprintf(fp,"\t\tBalance = %s    [静态权益]\n",Balance.getString().c_str());
	if (compare(PreBalance.getValue(),pOriginal->PreBalance.getValue())!=0)
		fprintf(fp,"\t\tPreBalance = %s -> %s    [上次静态权益]\n",pOriginal->PreBalance.getString().c_str(),PreBalance.getString().c_str());
	else
		fprintf(fp,"\t\tPreBalance = %s    [上次静态权益]\n",PreBalance.getString().c_str());
	if (compare(SortNumber.getValue(),pOriginal->SortNumber.getValue())!=0)
		fprintf(fp,"\t\tSortNumber = %s -> %s    [排序字段]\n",pOriginal->SortNumber.getString().c_str(),SortNumber.getString().c_str());
	else
		fprintf(fp,"\t\tSortNumber = %s    [排序字段]\n",SortNumber.getString().c_str());
	if (compare(CanReduce.getValue(),pOriginal->CanReduce.getValue())!=0)
		fprintf(fp,"\t\tCanReduce = %s -> %s    [是否可以出金]\n",pOriginal->CanReduce.getString().c_str(),CanReduce.getString().c_str());
	else
		fprintf(fp,"\t\tCanReduce = %s    [是否可以出金]\n",CanReduce.getString().c_str());
	if (compare(IsTrading.getValue(),pOriginal->IsTrading.getValue())!=0)
		fprintf(fp,"\t\tIsTrading = %s -> %s    [是否允许相关交易]\n",pOriginal->IsTrading.getString().c_str(),IsTrading.getString().c_str());
	else
		fprintf(fp,"\t\tIsTrading = %s    [是否允许相关交易]\n",IsTrading.getString().c_str());
	if (compare(SettleSegment.getValue(),pOriginal->SettleSegment.getValue())!=0)
		fprintf(fp,"\t\tSettleSegment = %s -> %s    [结算段]\n",pOriginal->SettleSegment.getString().c_str(),SettleSegment.getString().c_str());
	else
		fprintf(fp,"\t\tSettleSegment = %s    [结算段]\n",SettleSegment.getString().c_str());
	if (compare(MoneyTick.getValue(),pOriginal->MoneyTick.getValue())!=0)
		fprintf(fp,"\t\tMoneyTick = %s -> %s    [最小单位]\n",pOriginal->MoneyTick.getString().c_str(),MoneyTick.getString().c_str());
	else
		fprintf(fp,"\t\tMoneyTick = %s    [最小单位]\n",MoneyTick.getString().c_str());
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [价格参考交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [价格参考交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [价格参考标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [价格参考标的代码]\n",InstrumentID.getString().c_str());
	if (compare(LoanUse.getValue(),pOriginal->LoanUse.getValue())!=0)
		fprintf(fp,"\t\tLoanUse = %s -> %s    [借款使用量]\n",pOriginal->LoanUse.getString().c_str(),LoanUse.getString().c_str());
	else
		fprintf(fp,"\t\tLoanUse = %s    [借款使用量]\n",LoanUse.getString().c_str());
	if (compare(LoanLimit.getValue(),pOriginal->LoanLimit.getValue())!=0)
		fprintf(fp,"\t\tLoanLimit = %s -> %s    [最大可借比例]\n",pOriginal->LoanLimit.getString().c_str(),LoanLimit.getString().c_str());
	else
		fprintf(fp,"\t\tLoanLimit = %s    [最大可借比例]\n",LoanLimit.getString().c_str());
	if (compare(LoanBorrow.getValue(),pOriginal->LoanBorrow.getValue())!=0)
		fprintf(fp,"\t\tLoanBorrow = %s -> %s    [借款利息]\n",pOriginal->LoanBorrow.getString().c_str(),LoanBorrow.getString().c_str());
	else
		fprintf(fp,"\t\tLoanBorrow = %s    [借款利息]\n",LoanBorrow.getString().c_str());
	if (compare(LoanDeposit.getValue(),pOriginal->LoanDeposit.getValue())!=0)
		fprintf(fp,"\t\tLoanDeposit = %s -> %s    [存款利息]\n",pOriginal->LoanDeposit.getString().c_str(),LoanDeposit.getString().c_str());
	else
		fprintf(fp,"\t\tLoanDeposit = %s    [存款利息]\n",LoanDeposit.getString().c_str());
	if (compare(LoanBase.getValue(),pOriginal->LoanBase.getValue())!=0)
		fprintf(fp,"\t\tLoanBase = %s -> %s    [利息起点]\n",pOriginal->LoanBase.getString().c_str(),LoanBase.getString().c_str());
	else
		fprintf(fp,"\t\tLoanBase = %s    [利息起点]\n",LoanBase.getString().c_str());
	if (compare(LoanRate.getValue(),pOriginal->LoanRate.getValue())!=0)
		fprintf(fp,"\t\tLoanRate = %s -> %s    [利息斜率]\n",pOriginal->LoanRate.getString().c_str(),LoanRate.getString().c_str());
	else
		fprintf(fp,"\t\tLoanRate = %s    [利息斜率]\n",LoanRate.getString().c_str());
	if (compare(LoanHighBase.getValue(),pOriginal->LoanHighBase.getValue())!=0)
		fprintf(fp,"\t\tLoanHighBase = %s -> %s    [高利息起点]\n",pOriginal->LoanHighBase.getString().c_str(),LoanHighBase.getString().c_str());
	else
		fprintf(fp,"\t\tLoanHighBase = %s    [高利息起点]\n",LoanHighBase.getString().c_str());
	if (compare(LoanHighRate.getValue(),pOriginal->LoanHighRate.getValue())!=0)
		fprintf(fp,"\t\tLoanHighRate = %s -> %s    [高利息斜率]\n",pOriginal->LoanHighRate.getString().c_str(),LoanHighRate.getString().c_str());
	else
		fprintf(fp,"\t\tLoanHighRate = %s    [高利息斜率]\n",LoanHighRate.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(CurrValue.getValue(),pOriginal->CurrValue.getValue())!=0)
		fprintf(fp,"\t\tCurrValue = %s -> %s    [当前价值]\n",pOriginal->CurrValue.getString().c_str(),CurrValue.getString().c_str());
	else
		fprintf(fp,"\t\tCurrValue = %s    [当前价值]\n",CurrValue.getString().c_str());
	if (compare(HairCut.getValue(),pOriginal->HairCut.getValue())!=0)
		fprintf(fp,"\t\tHairCut = %s -> %s    [抵押比例]\n",pOriginal->HairCut.getString().c_str(),HairCut.getString().c_str());
	else
		fprintf(fp,"\t\tHairCut = %s    [抵押比例]\n",HairCut.getString().c_str());
	if (compare(MaxValue.getValue(),pOriginal->MaxValue.getValue())!=0)
		fprintf(fp,"\t\tMaxValue = %s -> %s    [最大抵押数额]\n",pOriginal->MaxValue.getString().c_str(),MaxValue.getString().c_str());
	else
		fprintf(fp,"\t\tMaxValue = %s    [最大抵押数额]\n",MaxValue.getString().c_str());
	if (compare(MarginRateGroup.getValue(),pOriginal->MarginRateGroup.getValue())!=0)
		fprintf(fp,"\t\tMarginRateGroup = %s -> %s    [保证金组]\n",pOriginal->MarginRateGroup.getString().c_str(),MarginRateGroup.getString().c_str());
	else
		fprintf(fp,"\t\tMarginRateGroup = %s    [保证金组]\n",MarginRateGroup.getString().c_str());
	if (compare(PrivateEnd.getValue(),pOriginal->PrivateEnd.getValue())!=0)
		fprintf(fp,"\t\tPrivateEnd = %s -> %s    [私有化停止交易线]\n",pOriginal->PrivateEnd.getString().c_str(),PrivateEnd.getString().c_str());
	else
		fprintf(fp,"\t\tPrivateEnd = %s    [私有化停止交易线]\n",PrivateEnd.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableCurrency::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",SettlementGroup=%s",SettlementGroup.getString().c_str());	
	fprintf(fp,",Currency=%s",Currency.getString().c_str());	
	fprintf(fp,",Deposit=%s",Deposit.getString().c_str());	
	fprintf(fp,",Withdraw=%s",Withdraw.getString().c_str());	
	fprintf(fp,",Balance=%s",Balance.getString().c_str());	
	fprintf(fp,",PreBalance=%s",PreBalance.getString().c_str());	
	fprintf(fp,",SortNumber=%s",SortNumber.getString().c_str());	
	fprintf(fp,",CanReduce=%s",CanReduce.getString().c_str());	
	fprintf(fp,",IsTrading=%s",IsTrading.getString().c_str());	
	fprintf(fp,",SettleSegment=%s",SettleSegment.getString().c_str());	
	fprintf(fp,",MoneyTick=%s",MoneyTick.getString().c_str());	
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",LoanUse=%s",LoanUse.getString().c_str());	
	fprintf(fp,",LoanLimit=%s",LoanLimit.getString().c_str());	
	fprintf(fp,",LoanBorrow=%s",LoanBorrow.getString().c_str());	
	fprintf(fp,",LoanDeposit=%s",LoanDeposit.getString().c_str());	
	fprintf(fp,",LoanBase=%s",LoanBase.getString().c_str());	
	fprintf(fp,",LoanRate=%s",LoanRate.getString().c_str());	
	fprintf(fp,",LoanHighBase=%s",LoanHighBase.getString().c_str());	
	fprintf(fp,",LoanHighRate=%s",LoanHighRate.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",CurrValue=%s",CurrValue.getString().c_str());	
	fprintf(fp,",HairCut=%s",HairCut.getString().c_str());	
	fprintf(fp,",MaxValue=%s",MaxValue.getString().c_str());	
	fprintf(fp,",MarginRateGroup=%s",MarginRateGroup.getString().c_str());	
	fprintf(fp,",PrivateEnd=%s",PrivateEnd.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableCurrency::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"SettlementGroup=%s,",SettlementGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Currency=%s,",Currency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Deposit=%s,",Deposit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Withdraw=%s,",Withdraw.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Balance=%s,",Balance.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PreBalance=%s,",PreBalance.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SortNumber=%s,",SortNumber.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CanReduce=%s,",CanReduce.getString().c_str());
	p+=strlen(p);
	sprintf(p,"IsTrading=%s,",IsTrading.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettleSegment=%s,",SettleSegment.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MoneyTick=%s,",MoneyTick.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LoanUse=%s,",LoanUse.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LoanLimit=%s,",LoanLimit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LoanBorrow=%s,",LoanBorrow.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LoanDeposit=%s,",LoanDeposit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LoanBase=%s,",LoanBase.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LoanRate=%s,",LoanRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LoanHighBase=%s,",LoanHighBase.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LoanHighRate=%s,",LoanHighRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CurrValue=%s,",CurrValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"HairCut=%s,",HairCut.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MaxValue=%s,",MaxValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MarginRateGroup=%s,",MarginRateGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PrivateEnd=%s,",PrivateEnd.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CCurrencyIterator::dump(FILE *output)
{
	CCurrency *pCurrency;
	while ((pCurrency=next())!=NULL)
	{
		pCurrency->dump(output);
	}
}

const CWriteableMarketData *CWriteableCurrency::linkMarketData() const
{
	void *target=(void *)(&pMarketData);
	*((const CWriteableMarketData **)target)=m_pMDB->m_MarketDataFactory->findByInstrumentID(ExchangeID,InstrumentID);
	return pMarketData;
}



void CWriteableLiquidity::writeCSVHead(FILE *output)
{
	fprintf(output,"\"LiquidityGrade*\",\"LiquidityGroup*\",\"BasePriceType\",\"VolumeBegin\",\"BuyOpen\",\"BuyClose\",\"SellOpen\",\"SellClose\",\"InsertTime\",\"UpdateTime\"\n");
}

void CWriteableLiquidity::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"LiquidityGrade\",\"LiquidityGroup\",\"BasePriceType\",\"VolumeBegin\",\"BuyOpen\",\"BuyClose\",\"SellOpen\",\"SellClose\",\"InsertTime\",\"UpdateTime\"\n");
}
	
void CWriteableLiquidity::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	LiquidityGrade.init();
	LiquidityGroup.init();
	BasePriceType.clear();
	VolumeBegin.clear();
	BuyOpen.clear();
	BuyClose.clear();
	SellOpen.clear();
	SellClose.clear();
	InsertTime.clear();
	UpdateTime.clear();
}

void CWriteableLiquidity::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableLiquidity::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableLiquidity),input)!=sizeof(CWriteableLiquidity))
		return 0;
	else
		return 1;
}
	
int CWriteableLiquidity::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			LiquidityGrade=token;
		else
			LiquidityGrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LiquidityGroup=token;
		else
			LiquidityGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BasePriceType=token;
		else
			BasePriceType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeBegin=token;
		else
			VolumeBegin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BuyOpen=token;
		else
			BuyOpen="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BuyClose=token;
		else
			BuyClose="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SellOpen=token;
		else
			SellOpen="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SellClose=token;
		else
			SellClose="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"LiquidityGrade"))
			{
				LiquidityGrade=token;
			}
			else if (!strcmp(fieldName,"LiquidityGroup"))
			{
				LiquidityGroup=token;
			}
			else if (!strcmp(fieldName,"BasePriceType"))
			{
				BasePriceType=token;
			}
			else if (!strcmp(fieldName,"VolumeBegin"))
			{
				VolumeBegin=token;
			}
			else if (!strcmp(fieldName,"BuyOpen"))
			{
				BuyOpen=token;
			}
			else if (!strcmp(fieldName,"BuyClose"))
			{
				BuyClose=token;
			}
			else if (!strcmp(fieldName,"SellOpen"))
			{
				SellOpen=token;
			}
			else if (!strcmp(fieldName,"SellClose"))
			{
				SellClose=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableLiquidity::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableLiquidity),output)!=sizeof(CWriteableLiquidity))
		return 0;
	else
		return 1;
}

int CWriteableLiquidity::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",LiquidityGrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LiquidityGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BasePriceType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeBegin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BuyOpen.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BuyClose.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SellOpen.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SellClose.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableLiquidity::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",LiquidityGrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LiquidityGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BasePriceType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeBegin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BuyOpen.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BuyClose.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SellOpen.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SellClose.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableLiquidity::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCLiquidity%s = {\n", index_buf);
	fprintf(fp,"\t\tLiquidityGrade = %s    [流动性等级]\n",LiquidityGrade.getString().c_str());	
	fprintf(fp,"\t\tLiquidityGroup = %s    [标的代码]\n",LiquidityGroup.getString().c_str());	
	fprintf(fp,"\t\tBasePriceType = %s    [基于的价格]\n",BasePriceType.getString().c_str());	
	fprintf(fp,"\t\tVolumeBegin = %s    [起始量]\n",VolumeBegin.getString().c_str());	
	fprintf(fp,"\t\tBuyOpen = %s    [买开流动性比例]\n",BuyOpen.getString().c_str());	
	fprintf(fp,"\t\tBuyClose = %s    [买平流动性比例]\n",BuyClose.getString().c_str());	
	fprintf(fp,"\t\tSellOpen = %s    [卖开流动性比例]\n",SellOpen.getString().c_str());	
	fprintf(fp,"\t\tSellClose = %s    [卖平流动性比例]\n",SellClose.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableLiquidity::dumpDiff(FILE *fp, const CWriteableLiquidity *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCLiquidity = {\n");
	if (compare(LiquidityGrade.getValue(),pOriginal->LiquidityGrade.getValue())!=0)
		fprintf(fp,"\t\tLiquidityGrade = %s -> %s    [流动性等级]\n",pOriginal->LiquidityGrade.getString().c_str(),LiquidityGrade.getString().c_str());
	else
		fprintf(fp,"\t\tLiquidityGrade = %s    [流动性等级]\n",LiquidityGrade.getString().c_str());
	if (compare(LiquidityGroup.getValue(),pOriginal->LiquidityGroup.getValue())!=0)
		fprintf(fp,"\t\tLiquidityGroup = %s -> %s    [标的代码]\n",pOriginal->LiquidityGroup.getString().c_str(),LiquidityGroup.getString().c_str());
	else
		fprintf(fp,"\t\tLiquidityGroup = %s    [标的代码]\n",LiquidityGroup.getString().c_str());
	if (compare(BasePriceType.getValue(),pOriginal->BasePriceType.getValue())!=0)
		fprintf(fp,"\t\tBasePriceType = %s -> %s    [基于的价格]\n",pOriginal->BasePriceType.getString().c_str(),BasePriceType.getString().c_str());
	else
		fprintf(fp,"\t\tBasePriceType = %s    [基于的价格]\n",BasePriceType.getString().c_str());
	if (compare(VolumeBegin.getValue(),pOriginal->VolumeBegin.getValue())!=0)
		fprintf(fp,"\t\tVolumeBegin = %s -> %s    [起始量]\n",pOriginal->VolumeBegin.getString().c_str(),VolumeBegin.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeBegin = %s    [起始量]\n",VolumeBegin.getString().c_str());
	if (compare(BuyOpen.getValue(),pOriginal->BuyOpen.getValue())!=0)
		fprintf(fp,"\t\tBuyOpen = %s -> %s    [买开流动性比例]\n",pOriginal->BuyOpen.getString().c_str(),BuyOpen.getString().c_str());
	else
		fprintf(fp,"\t\tBuyOpen = %s    [买开流动性比例]\n",BuyOpen.getString().c_str());
	if (compare(BuyClose.getValue(),pOriginal->BuyClose.getValue())!=0)
		fprintf(fp,"\t\tBuyClose = %s -> %s    [买平流动性比例]\n",pOriginal->BuyClose.getString().c_str(),BuyClose.getString().c_str());
	else
		fprintf(fp,"\t\tBuyClose = %s    [买平流动性比例]\n",BuyClose.getString().c_str());
	if (compare(SellOpen.getValue(),pOriginal->SellOpen.getValue())!=0)
		fprintf(fp,"\t\tSellOpen = %s -> %s    [卖开流动性比例]\n",pOriginal->SellOpen.getString().c_str(),SellOpen.getString().c_str());
	else
		fprintf(fp,"\t\tSellOpen = %s    [卖开流动性比例]\n",SellOpen.getString().c_str());
	if (compare(SellClose.getValue(),pOriginal->SellClose.getValue())!=0)
		fprintf(fp,"\t\tSellClose = %s -> %s    [卖平流动性比例]\n",pOriginal->SellClose.getString().c_str(),SellClose.getString().c_str());
	else
		fprintf(fp,"\t\tSellClose = %s    [卖平流动性比例]\n",SellClose.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableLiquidity::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",LiquidityGrade=%s",LiquidityGrade.getString().c_str());	
	fprintf(fp,",LiquidityGroup=%s",LiquidityGroup.getString().c_str());	
	fprintf(fp,",BasePriceType=%s",BasePriceType.getString().c_str());	
	fprintf(fp,",VolumeBegin=%s",VolumeBegin.getString().c_str());	
	fprintf(fp,",BuyOpen=%s",BuyOpen.getString().c_str());	
	fprintf(fp,",BuyClose=%s",BuyClose.getString().c_str());	
	fprintf(fp,",SellOpen=%s",SellOpen.getString().c_str());	
	fprintf(fp,",SellClose=%s",SellClose.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableLiquidity::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"LiquidityGrade=%s,",LiquidityGrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LiquidityGroup=%s,",LiquidityGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BasePriceType=%s,",BasePriceType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeBegin=%s,",VolumeBegin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BuyOpen=%s,",BuyOpen.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BuyClose=%s,",BuyClose.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SellOpen=%s,",SellOpen.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SellClose=%s,",SellClose.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CLiquidityIterator::dump(FILE *output)
{
	CLiquidity *pLiquidity;
	while ((pLiquidity=next())!=NULL)
	{
		pLiquidity->dump(output);
	}
}



void CWriteableAccount::writeCSVHead(FILE *output)
{
	fprintf(output,"\"SettlementGroup*\",\"AccountID*\",\"Currency*\",\"MemberID*\",\"Available\",\"Withdrawable\",\"PreBalance\",\"Balance\",\"PositionCost\",\"UseMargin\",\"CrossMargin\",\"CloseProfit\",\"Deposit\",\"Withdraw\",\"FrozenMargin\",\"FrozenMoney\",\"FrozenFee\",\"Fee\",\"MoneyChange\",\"Reserve\",\"ReserveAvailable\",\"ReserveLimit\",\"ReserveProfit\",\"ReserveType\",\"ReserveActive\",\"TotalCloseProfit\",\"TotalDeposit\",\"TotalWithdraw\",\"TotalFee\",\"TotalMoneyChange\",\"InsertTime\",\"UpdateTime\",\"AccountType\",\"BusinessNo\",\"MarginRateGrade\",\"TradingRightGrade\",\"Frequency\",\"MaintMargin\",\"UnRealProfit\",\"CopyProfit\",\"CFDType\",\"FeeGrade\",\"CFDGrade\",\"Borrow\",\"Remark\",\"MaxLocalID\",\"CurrValue\",\"CanReduce\"\n");
}

void CWriteableAccount::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"SettlementGroup\",\"AccountID\",\"Currency\",\"MemberID\",\"Available\",\"Withdrawable\",\"PreBalance\",\"Balance\",\"PositionCost\",\"UseMargin\",\"CrossMargin\",\"CloseProfit\",\"Deposit\",\"Withdraw\",\"FrozenMargin\",\"FrozenMoney\",\"FrozenFee\",\"Fee\",\"MoneyChange\",\"Reserve\",\"ReserveAvailable\",\"ReserveLimit\",\"ReserveProfit\",\"ReserveType\",\"ReserveActive\",\"TotalCloseProfit\",\"TotalDeposit\",\"TotalWithdraw\",\"TotalFee\",\"TotalMoneyChange\",\"InsertTime\",\"UpdateTime\",\"AccountType\",\"BusinessNo\",\"MarginRateGrade\",\"TradingRightGrade\",\"Frequency\",\"MaintMargin\",\"UnRealProfit\",\"CopyProfit\",\"CFDType\",\"FeeGrade\",\"CFDGrade\",\"Borrow\",\"Remark\",\"MaxLocalID\",\"CurrValue\",\"CanReduce\"\n");
}
	
void CWriteableAccount::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	SettlementGroup.init();
	AccountID.init();
	Currency.init();
	MemberID.init();
	Available.init();
	Withdrawable.init();
	PreBalance.init();
	Balance.init();
	PositionCost.init();
	UseMargin.init();
	CrossMargin.init();
	CloseProfit.init();
	Deposit.init();
	Withdraw.init();
	FrozenMargin.init();
	FrozenMoney.init();
	FrozenFee.init();
	Fee.init();
	MoneyChange.init();
	Reserve.init();
	ReserveAvailable.init();
	ReserveLimit.init();
	ReserveProfit.init();
	ReserveType.init();
	ReserveActive.init();
	TotalCloseProfit.init();
	TotalDeposit.init();
	TotalWithdraw.init();
	TotalFee.init();
	TotalMoneyChange.init();
	InsertTime.clear();
	UpdateTime.clear();
	AccountType.clear();
	BusinessNo.clear();
	MarginRateGrade.clear();
	TradingRightGrade.clear();
	Frequency.clear();
	MaintMargin.init();
	UnRealProfit.init();
	CopyProfit.init();
	CFDType.clear();
	FeeGrade.clear();
	CFDGrade.clear();
	Borrow.init();
	Remark.clear();
	MaxLocalID.clear();
	CurrValue.init();
	CanReduce.setValue(1);
}

void CWriteableAccount::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableAccount::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableAccount),input)!=sizeof(CWriteableAccount))
		return 0;
	else
		return 1;
}
	
int CWriteableAccount::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			SettlementGroup=token;
		else
			SettlementGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccountID=token;
		else
			AccountID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Currency=token;
		else
			Currency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Available=token;
		else
			Available="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Withdrawable=token;
		else
			Withdrawable="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PreBalance=token;
		else
			PreBalance="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Balance=token;
		else
			Balance="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionCost=token;
		else
			PositionCost="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UseMargin=token;
		else
			UseMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CrossMargin=token;
		else
			CrossMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseProfit=token;
		else
			CloseProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Deposit=token;
		else
			Deposit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Withdraw=token;
		else
			Withdraw="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrozenMargin=token;
		else
			FrozenMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrozenMoney=token;
		else
			FrozenMoney="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrozenFee=token;
		else
			FrozenFee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Fee=token;
		else
			Fee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MoneyChange=token;
		else
			MoneyChange="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Reserve=token;
		else
			Reserve="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ReserveAvailable=token;
		else
			ReserveAvailable="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ReserveLimit=token;
		else
			ReserveLimit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ReserveProfit=token;
		else
			ReserveProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ReserveType=token;
		else
			ReserveType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ReserveActive=token;
		else
			ReserveActive="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TotalCloseProfit=token;
		else
			TotalCloseProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TotalDeposit=token;
		else
			TotalDeposit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TotalWithdraw=token;
		else
			TotalWithdraw="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TotalFee=token;
		else
			TotalFee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TotalMoneyChange=token;
		else
			TotalMoneyChange="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccountType=token;
		else
			AccountType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MarginRateGrade=token;
		else
			MarginRateGrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradingRightGrade=token;
		else
			TradingRightGrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Frequency=token;
		else
			Frequency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MaintMargin=token;
		else
			MaintMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UnRealProfit=token;
		else
			UnRealProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyProfit=token;
		else
			CopyProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CFDType=token;
		else
			CFDType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FeeGrade=token;
		else
			FeeGrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CFDGrade=token;
		else
			CFDGrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Borrow=token;
		else
			Borrow="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MaxLocalID=token;
		else
			MaxLocalID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CurrValue=token;
		else
			CurrValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CanReduce=token;
		else
			CanReduce="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"SettlementGroup"))
			{
				SettlementGroup=token;
			}
			else if (!strcmp(fieldName,"AccountID"))
			{
				AccountID=token;
			}
			else if (!strcmp(fieldName,"Currency"))
			{
				Currency=token;
			}
			else if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"Available"))
			{
				Available=token;
			}
			else if (!strcmp(fieldName,"Withdrawable"))
			{
				Withdrawable=token;
			}
			else if (!strcmp(fieldName,"PreBalance"))
			{
				PreBalance=token;
			}
			else if (!strcmp(fieldName,"Balance"))
			{
				Balance=token;
			}
			else if (!strcmp(fieldName,"PositionCost"))
			{
				PositionCost=token;
			}
			else if (!strcmp(fieldName,"UseMargin"))
			{
				UseMargin=token;
			}
			else if (!strcmp(fieldName,"CrossMargin"))
			{
				CrossMargin=token;
			}
			else if (!strcmp(fieldName,"CloseProfit"))
			{
				CloseProfit=token;
			}
			else if (!strcmp(fieldName,"Deposit"))
			{
				Deposit=token;
			}
			else if (!strcmp(fieldName,"Withdraw"))
			{
				Withdraw=token;
			}
			else if (!strcmp(fieldName,"FrozenMargin"))
			{
				FrozenMargin=token;
			}
			else if (!strcmp(fieldName,"FrozenMoney"))
			{
				FrozenMoney=token;
			}
			else if (!strcmp(fieldName,"FrozenFee"))
			{
				FrozenFee=token;
			}
			else if (!strcmp(fieldName,"Fee"))
			{
				Fee=token;
			}
			else if (!strcmp(fieldName,"MoneyChange"))
			{
				MoneyChange=token;
			}
			else if (!strcmp(fieldName,"Reserve"))
			{
				Reserve=token;
			}
			else if (!strcmp(fieldName,"ReserveAvailable"))
			{
				ReserveAvailable=token;
			}
			else if (!strcmp(fieldName,"ReserveLimit"))
			{
				ReserveLimit=token;
			}
			else if (!strcmp(fieldName,"ReserveProfit"))
			{
				ReserveProfit=token;
			}
			else if (!strcmp(fieldName,"ReserveType"))
			{
				ReserveType=token;
			}
			else if (!strcmp(fieldName,"ReserveActive"))
			{
				ReserveActive=token;
			}
			else if (!strcmp(fieldName,"TotalCloseProfit"))
			{
				TotalCloseProfit=token;
			}
			else if (!strcmp(fieldName,"TotalDeposit"))
			{
				TotalDeposit=token;
			}
			else if (!strcmp(fieldName,"TotalWithdraw"))
			{
				TotalWithdraw=token;
			}
			else if (!strcmp(fieldName,"TotalFee"))
			{
				TotalFee=token;
			}
			else if (!strcmp(fieldName,"TotalMoneyChange"))
			{
				TotalMoneyChange=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			else if (!strcmp(fieldName,"AccountType"))
			{
				AccountType=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			else if (!strcmp(fieldName,"MarginRateGrade"))
			{
				MarginRateGrade=token;
			}
			else if (!strcmp(fieldName,"TradingRightGrade"))
			{
				TradingRightGrade=token;
			}
			else if (!strcmp(fieldName,"Frequency"))
			{
				Frequency=token;
			}
			else if (!strcmp(fieldName,"MaintMargin"))
			{
				MaintMargin=token;
			}
			else if (!strcmp(fieldName,"UnRealProfit"))
			{
				UnRealProfit=token;
			}
			else if (!strcmp(fieldName,"CopyProfit"))
			{
				CopyProfit=token;
			}
			else if (!strcmp(fieldName,"CFDType"))
			{
				CFDType=token;
			}
			else if (!strcmp(fieldName,"FeeGrade"))
			{
				FeeGrade=token;
			}
			else if (!strcmp(fieldName,"CFDGrade"))
			{
				CFDGrade=token;
			}
			else if (!strcmp(fieldName,"Borrow"))
			{
				Borrow=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"MaxLocalID"))
			{
				MaxLocalID=token;
			}
			else if (!strcmp(fieldName,"CurrValue"))
			{
				CurrValue=token;
			}
			else if (!strcmp(fieldName,"CanReduce"))
			{
				CanReduce=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableAccount::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableAccount),output)!=sizeof(CWriteableAccount))
		return 0;
	else
		return 1;
}

int CWriteableAccount::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Currency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Available.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Withdrawable.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreBalance.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Balance.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionCost.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UseMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CrossMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Deposit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Withdraw.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMoney.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenFee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Fee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MoneyChange.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Reserve.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveAvailable.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveLimit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveActive.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalCloseProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalDeposit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalWithdraw.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalFee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalMoneyChange.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginRateGrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradingRightGrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Frequency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaintMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UnRealProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CFDType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeGrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CFDGrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Borrow.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxLocalID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CurrValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CanReduce.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableAccount::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Currency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Available.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Withdrawable.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreBalance.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Balance.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionCost.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UseMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CrossMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Deposit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Withdraw.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMoney.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenFee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Fee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MoneyChange.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Reserve.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveAvailable.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveLimit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveActive.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalCloseProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalDeposit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalWithdraw.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalFee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalMoneyChange.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarginRateGrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradingRightGrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Frequency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaintMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UnRealProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CFDType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeGrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CFDGrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Borrow.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaxLocalID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CurrValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CanReduce.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableAccount::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCAccount%s = {\n", index_buf);
	fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());	
	fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());	
	fprintf(fp,"\t\tCurrency = %s    [币种]\n",Currency.getString().c_str());	
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tAvailable = %s    [可用资金]\n",Available.getString().c_str());	
	fprintf(fp,"\t\tWithdrawable = %s    [可取资金]\n",Withdrawable.getString().c_str());	
	fprintf(fp,"\t\tPreBalance = %s    [上次静态权益]\n",PreBalance.getString().c_str());	
	fprintf(fp,"\t\tBalance = %s    [静态权益]\n",Balance.getString().c_str());	
	fprintf(fp,"\t\tPositionCost = %s    [持仓成本]\n",PositionCost.getString().c_str());	
	fprintf(fp,"\t\tUseMargin = %s    [所有占用保证金]\n",UseMargin.getString().c_str());	
	fprintf(fp,"\t\tCrossMargin = %s    [全仓保证金]\n",CrossMargin.getString().c_str());	
	fprintf(fp,"\t\tCloseProfit = %s    [平仓盈亏]\n",CloseProfit.getString().c_str());	
	fprintf(fp,"\t\tDeposit = %s    [入金金额]\n",Deposit.getString().c_str());	
	fprintf(fp,"\t\tWithdraw = %s    [出金金额]\n",Withdraw.getString().c_str());	
	fprintf(fp,"\t\tFrozenMargin = %s    [冻结保证金]\n",FrozenMargin.getString().c_str());	
	fprintf(fp,"\t\tFrozenMoney = %s    [冻结资金]\n",FrozenMoney.getString().c_str());	
	fprintf(fp,"\t\tFrozenFee = %s    [冻结手续费]\n",FrozenFee.getString().c_str());	
	fprintf(fp,"\t\tFee = %s    [手续费]\n",Fee.getString().c_str());	
	fprintf(fp,"\t\tMoneyChange = %s    [资金变化]\n",MoneyChange.getString().c_str());	
	fprintf(fp,"\t\tReserve = %s    [保留资金]\n",Reserve.getString().c_str());	
	fprintf(fp,"\t\tReserveAvailable = %s    [保留资金可用]\n",ReserveAvailable.getString().c_str());	
	fprintf(fp,"\t\tReserveLimit = %s    [保留资金限制]\n",ReserveLimit.getString().c_str());	
	fprintf(fp,"\t\tReserveProfit = %s    [保留资金盈亏]\n",ReserveProfit.getString().c_str());	
	fprintf(fp,"\t\tReserveType = %s    [保留资金类型]\n",ReserveType.getString().c_str());	
	fprintf(fp,"\t\tReserveActive = %s    [保留资金是否活跃]\n",ReserveActive.getString().c_str());	
	fprintf(fp,"\t\tTotalCloseProfit = %s    [总平仓盈亏]\n",TotalCloseProfit.getString().c_str());	
	fprintf(fp,"\t\tTotalDeposit = %s    [总入金金额]\n",TotalDeposit.getString().c_str());	
	fprintf(fp,"\t\tTotalWithdraw = %s    [总出金金额]\n",TotalWithdraw.getString().c_str());	
	fprintf(fp,"\t\tTotalFee = %s    [总手续费]\n",TotalFee.getString().c_str());	
	fprintf(fp,"\t\tTotalMoneyChange = %s    [总资金变化]\n",TotalMoneyChange.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t\tAccountType = %s    [账户类型]\n",AccountType.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t\tMarginRateGrade = %s    [保证金等级]\n",MarginRateGrade.getString().c_str());	
	fprintf(fp,"\t\tTradingRightGrade = %s    [权限等级]\n",TradingRightGrade.getString().c_str());	
	fprintf(fp,"\t\tFrequency = %s    [每秒更新的次数]\n",Frequency.getString().c_str());	
	fprintf(fp,"\t\tMaintMargin = %s    [维持保证金]\n",MaintMargin.getString().c_str());	
	fprintf(fp,"\t\tUnRealProfit = %s    [未实现盈亏]\n",UnRealProfit.getString().c_str());	
	fprintf(fp,"\t\tCopyProfit = %s    [带单盈利分配]\n",CopyProfit.getString().c_str());	
	fprintf(fp,"\t\tCFDType = %s    [点差接管类型]\n",CFDType.getString().c_str());	
	fprintf(fp,"\t\tFeeGrade = %s    [手续费等级]\n",FeeGrade.getString().c_str());	
	fprintf(fp,"\t\tCFDGrade = %s    [差价等级]\n",CFDGrade.getString().c_str());	
	fprintf(fp,"\t\tBorrow = %s    [借款周期结算量]\n",Borrow.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tMaxLocalID = %s    [最大本地标识]\n",MaxLocalID.getString().c_str());	
	fprintf(fp,"\t\tCurrValue = %s    [当前价值]\n",CurrValue.getString().c_str());	
	fprintf(fp,"\t\tCanReduce = %s    [是否可以出金]\n",CanReduce.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableAccount::dumpDiff(FILE *fp, const CWriteableAccount *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCAccount = {\n");
	if (compare(SettlementGroup.getValue(),pOriginal->SettlementGroup.getValue())!=0)
		fprintf(fp,"\t\tSettlementGroup = %s -> %s    [结算组编号]\n",pOriginal->SettlementGroup.getString().c_str(),SettlementGroup.getString().c_str());
	else
		fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());
	if (compare(AccountID.getValue(),pOriginal->AccountID.getValue())!=0)
		fprintf(fp,"\t\tAccountID = %s -> %s    [资金账号]\n",pOriginal->AccountID.getString().c_str(),AccountID.getString().c_str());
	else
		fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());
	if (compare(Currency.getValue(),pOriginal->Currency.getValue())!=0)
		fprintf(fp,"\t\tCurrency = %s -> %s    [币种]\n",pOriginal->Currency.getString().c_str(),Currency.getString().c_str());
	else
		fprintf(fp,"\t\tCurrency = %s    [币种]\n",Currency.getString().c_str());
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(Available.getValue(),pOriginal->Available.getValue())!=0)
		fprintf(fp,"\t\tAvailable = %s -> %s    [可用资金]\n",pOriginal->Available.getString().c_str(),Available.getString().c_str());
	else
		fprintf(fp,"\t\tAvailable = %s    [可用资金]\n",Available.getString().c_str());
	if (compare(Withdrawable.getValue(),pOriginal->Withdrawable.getValue())!=0)
		fprintf(fp,"\t\tWithdrawable = %s -> %s    [可取资金]\n",pOriginal->Withdrawable.getString().c_str(),Withdrawable.getString().c_str());
	else
		fprintf(fp,"\t\tWithdrawable = %s    [可取资金]\n",Withdrawable.getString().c_str());
	if (compare(PreBalance.getValue(),pOriginal->PreBalance.getValue())!=0)
		fprintf(fp,"\t\tPreBalance = %s -> %s    [上次静态权益]\n",pOriginal->PreBalance.getString().c_str(),PreBalance.getString().c_str());
	else
		fprintf(fp,"\t\tPreBalance = %s    [上次静态权益]\n",PreBalance.getString().c_str());
	if (compare(Balance.getValue(),pOriginal->Balance.getValue())!=0)
		fprintf(fp,"\t\tBalance = %s -> %s    [静态权益]\n",pOriginal->Balance.getString().c_str(),Balance.getString().c_str());
	else
		fprintf(fp,"\t\tBalance = %s    [静态权益]\n",Balance.getString().c_str());
	if (compare(PositionCost.getValue(),pOriginal->PositionCost.getValue())!=0)
		fprintf(fp,"\t\tPositionCost = %s -> %s    [持仓成本]\n",pOriginal->PositionCost.getString().c_str(),PositionCost.getString().c_str());
	else
		fprintf(fp,"\t\tPositionCost = %s    [持仓成本]\n",PositionCost.getString().c_str());
	if (compare(UseMargin.getValue(),pOriginal->UseMargin.getValue())!=0)
		fprintf(fp,"\t\tUseMargin = %s -> %s    [所有占用保证金]\n",pOriginal->UseMargin.getString().c_str(),UseMargin.getString().c_str());
	else
		fprintf(fp,"\t\tUseMargin = %s    [所有占用保证金]\n",UseMargin.getString().c_str());
	if (compare(CrossMargin.getValue(),pOriginal->CrossMargin.getValue())!=0)
		fprintf(fp,"\t\tCrossMargin = %s -> %s    [全仓保证金]\n",pOriginal->CrossMargin.getString().c_str(),CrossMargin.getString().c_str());
	else
		fprintf(fp,"\t\tCrossMargin = %s    [全仓保证金]\n",CrossMargin.getString().c_str());
	if (compare(CloseProfit.getValue(),pOriginal->CloseProfit.getValue())!=0)
		fprintf(fp,"\t\tCloseProfit = %s -> %s    [平仓盈亏]\n",pOriginal->CloseProfit.getString().c_str(),CloseProfit.getString().c_str());
	else
		fprintf(fp,"\t\tCloseProfit = %s    [平仓盈亏]\n",CloseProfit.getString().c_str());
	if (compare(Deposit.getValue(),pOriginal->Deposit.getValue())!=0)
		fprintf(fp,"\t\tDeposit = %s -> %s    [入金金额]\n",pOriginal->Deposit.getString().c_str(),Deposit.getString().c_str());
	else
		fprintf(fp,"\t\tDeposit = %s    [入金金额]\n",Deposit.getString().c_str());
	if (compare(Withdraw.getValue(),pOriginal->Withdraw.getValue())!=0)
		fprintf(fp,"\t\tWithdraw = %s -> %s    [出金金额]\n",pOriginal->Withdraw.getString().c_str(),Withdraw.getString().c_str());
	else
		fprintf(fp,"\t\tWithdraw = %s    [出金金额]\n",Withdraw.getString().c_str());
	if (compare(FrozenMargin.getValue(),pOriginal->FrozenMargin.getValue())!=0)
		fprintf(fp,"\t\tFrozenMargin = %s -> %s    [冻结保证金]\n",pOriginal->FrozenMargin.getString().c_str(),FrozenMargin.getString().c_str());
	else
		fprintf(fp,"\t\tFrozenMargin = %s    [冻结保证金]\n",FrozenMargin.getString().c_str());
	if (compare(FrozenMoney.getValue(),pOriginal->FrozenMoney.getValue())!=0)
		fprintf(fp,"\t\tFrozenMoney = %s -> %s    [冻结资金]\n",pOriginal->FrozenMoney.getString().c_str(),FrozenMoney.getString().c_str());
	else
		fprintf(fp,"\t\tFrozenMoney = %s    [冻结资金]\n",FrozenMoney.getString().c_str());
	if (compare(FrozenFee.getValue(),pOriginal->FrozenFee.getValue())!=0)
		fprintf(fp,"\t\tFrozenFee = %s -> %s    [冻结手续费]\n",pOriginal->FrozenFee.getString().c_str(),FrozenFee.getString().c_str());
	else
		fprintf(fp,"\t\tFrozenFee = %s    [冻结手续费]\n",FrozenFee.getString().c_str());
	if (compare(Fee.getValue(),pOriginal->Fee.getValue())!=0)
		fprintf(fp,"\t\tFee = %s -> %s    [手续费]\n",pOriginal->Fee.getString().c_str(),Fee.getString().c_str());
	else
		fprintf(fp,"\t\tFee = %s    [手续费]\n",Fee.getString().c_str());
	if (compare(MoneyChange.getValue(),pOriginal->MoneyChange.getValue())!=0)
		fprintf(fp,"\t\tMoneyChange = %s -> %s    [资金变化]\n",pOriginal->MoneyChange.getString().c_str(),MoneyChange.getString().c_str());
	else
		fprintf(fp,"\t\tMoneyChange = %s    [资金变化]\n",MoneyChange.getString().c_str());
	if (compare(Reserve.getValue(),pOriginal->Reserve.getValue())!=0)
		fprintf(fp,"\t\tReserve = %s -> %s    [保留资金]\n",pOriginal->Reserve.getString().c_str(),Reserve.getString().c_str());
	else
		fprintf(fp,"\t\tReserve = %s    [保留资金]\n",Reserve.getString().c_str());
	if (compare(ReserveAvailable.getValue(),pOriginal->ReserveAvailable.getValue())!=0)
		fprintf(fp,"\t\tReserveAvailable = %s -> %s    [保留资金可用]\n",pOriginal->ReserveAvailable.getString().c_str(),ReserveAvailable.getString().c_str());
	else
		fprintf(fp,"\t\tReserveAvailable = %s    [保留资金可用]\n",ReserveAvailable.getString().c_str());
	if (compare(ReserveLimit.getValue(),pOriginal->ReserveLimit.getValue())!=0)
		fprintf(fp,"\t\tReserveLimit = %s -> %s    [保留资金限制]\n",pOriginal->ReserveLimit.getString().c_str(),ReserveLimit.getString().c_str());
	else
		fprintf(fp,"\t\tReserveLimit = %s    [保留资金限制]\n",ReserveLimit.getString().c_str());
	if (compare(ReserveProfit.getValue(),pOriginal->ReserveProfit.getValue())!=0)
		fprintf(fp,"\t\tReserveProfit = %s -> %s    [保留资金盈亏]\n",pOriginal->ReserveProfit.getString().c_str(),ReserveProfit.getString().c_str());
	else
		fprintf(fp,"\t\tReserveProfit = %s    [保留资金盈亏]\n",ReserveProfit.getString().c_str());
	if (compare(ReserveType.getValue(),pOriginal->ReserveType.getValue())!=0)
		fprintf(fp,"\t\tReserveType = %s -> %s    [保留资金类型]\n",pOriginal->ReserveType.getString().c_str(),ReserveType.getString().c_str());
	else
		fprintf(fp,"\t\tReserveType = %s    [保留资金类型]\n",ReserveType.getString().c_str());
	if (compare(ReserveActive.getValue(),pOriginal->ReserveActive.getValue())!=0)
		fprintf(fp,"\t\tReserveActive = %s -> %s    [保留资金是否活跃]\n",pOriginal->ReserveActive.getString().c_str(),ReserveActive.getString().c_str());
	else
		fprintf(fp,"\t\tReserveActive = %s    [保留资金是否活跃]\n",ReserveActive.getString().c_str());
	if (compare(TotalCloseProfit.getValue(),pOriginal->TotalCloseProfit.getValue())!=0)
		fprintf(fp,"\t\tTotalCloseProfit = %s -> %s    [总平仓盈亏]\n",pOriginal->TotalCloseProfit.getString().c_str(),TotalCloseProfit.getString().c_str());
	else
		fprintf(fp,"\t\tTotalCloseProfit = %s    [总平仓盈亏]\n",TotalCloseProfit.getString().c_str());
	if (compare(TotalDeposit.getValue(),pOriginal->TotalDeposit.getValue())!=0)
		fprintf(fp,"\t\tTotalDeposit = %s -> %s    [总入金金额]\n",pOriginal->TotalDeposit.getString().c_str(),TotalDeposit.getString().c_str());
	else
		fprintf(fp,"\t\tTotalDeposit = %s    [总入金金额]\n",TotalDeposit.getString().c_str());
	if (compare(TotalWithdraw.getValue(),pOriginal->TotalWithdraw.getValue())!=0)
		fprintf(fp,"\t\tTotalWithdraw = %s -> %s    [总出金金额]\n",pOriginal->TotalWithdraw.getString().c_str(),TotalWithdraw.getString().c_str());
	else
		fprintf(fp,"\t\tTotalWithdraw = %s    [总出金金额]\n",TotalWithdraw.getString().c_str());
	if (compare(TotalFee.getValue(),pOriginal->TotalFee.getValue())!=0)
		fprintf(fp,"\t\tTotalFee = %s -> %s    [总手续费]\n",pOriginal->TotalFee.getString().c_str(),TotalFee.getString().c_str());
	else
		fprintf(fp,"\t\tTotalFee = %s    [总手续费]\n",TotalFee.getString().c_str());
	if (compare(TotalMoneyChange.getValue(),pOriginal->TotalMoneyChange.getValue())!=0)
		fprintf(fp,"\t\tTotalMoneyChange = %s -> %s    [总资金变化]\n",pOriginal->TotalMoneyChange.getString().c_str(),TotalMoneyChange.getString().c_str());
	else
		fprintf(fp,"\t\tTotalMoneyChange = %s    [总资金变化]\n",TotalMoneyChange.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	if (compare(AccountType.getValue(),pOriginal->AccountType.getValue())!=0)
		fprintf(fp,"\t\tAccountType = %s -> %s    [账户类型]\n",pOriginal->AccountType.getString().c_str(),AccountType.getString().c_str());
	else
		fprintf(fp,"\t\tAccountType = %s    [账户类型]\n",AccountType.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [业务序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());
	if (compare(MarginRateGrade.getValue(),pOriginal->MarginRateGrade.getValue())!=0)
		fprintf(fp,"\t\tMarginRateGrade = %s -> %s    [保证金等级]\n",pOriginal->MarginRateGrade.getString().c_str(),MarginRateGrade.getString().c_str());
	else
		fprintf(fp,"\t\tMarginRateGrade = %s    [保证金等级]\n",MarginRateGrade.getString().c_str());
	if (compare(TradingRightGrade.getValue(),pOriginal->TradingRightGrade.getValue())!=0)
		fprintf(fp,"\t\tTradingRightGrade = %s -> %s    [权限等级]\n",pOriginal->TradingRightGrade.getString().c_str(),TradingRightGrade.getString().c_str());
	else
		fprintf(fp,"\t\tTradingRightGrade = %s    [权限等级]\n",TradingRightGrade.getString().c_str());
	if (compare(Frequency.getValue(),pOriginal->Frequency.getValue())!=0)
		fprintf(fp,"\t\tFrequency = %s -> %s    [每秒更新的次数]\n",pOriginal->Frequency.getString().c_str(),Frequency.getString().c_str());
	else
		fprintf(fp,"\t\tFrequency = %s    [每秒更新的次数]\n",Frequency.getString().c_str());
	if (compare(MaintMargin.getValue(),pOriginal->MaintMargin.getValue())!=0)
		fprintf(fp,"\t\tMaintMargin = %s -> %s    [维持保证金]\n",pOriginal->MaintMargin.getString().c_str(),MaintMargin.getString().c_str());
	else
		fprintf(fp,"\t\tMaintMargin = %s    [维持保证金]\n",MaintMargin.getString().c_str());
	if (compare(UnRealProfit.getValue(),pOriginal->UnRealProfit.getValue())!=0)
		fprintf(fp,"\t\tUnRealProfit = %s -> %s    [未实现盈亏]\n",pOriginal->UnRealProfit.getString().c_str(),UnRealProfit.getString().c_str());
	else
		fprintf(fp,"\t\tUnRealProfit = %s    [未实现盈亏]\n",UnRealProfit.getString().c_str());
	if (compare(CopyProfit.getValue(),pOriginal->CopyProfit.getValue())!=0)
		fprintf(fp,"\t\tCopyProfit = %s -> %s    [带单盈利分配]\n",pOriginal->CopyProfit.getString().c_str(),CopyProfit.getString().c_str());
	else
		fprintf(fp,"\t\tCopyProfit = %s    [带单盈利分配]\n",CopyProfit.getString().c_str());
	if (compare(CFDType.getValue(),pOriginal->CFDType.getValue())!=0)
		fprintf(fp,"\t\tCFDType = %s -> %s    [点差接管类型]\n",pOriginal->CFDType.getString().c_str(),CFDType.getString().c_str());
	else
		fprintf(fp,"\t\tCFDType = %s    [点差接管类型]\n",CFDType.getString().c_str());
	if (compare(FeeGrade.getValue(),pOriginal->FeeGrade.getValue())!=0)
		fprintf(fp,"\t\tFeeGrade = %s -> %s    [手续费等级]\n",pOriginal->FeeGrade.getString().c_str(),FeeGrade.getString().c_str());
	else
		fprintf(fp,"\t\tFeeGrade = %s    [手续费等级]\n",FeeGrade.getString().c_str());
	if (compare(CFDGrade.getValue(),pOriginal->CFDGrade.getValue())!=0)
		fprintf(fp,"\t\tCFDGrade = %s -> %s    [差价等级]\n",pOriginal->CFDGrade.getString().c_str(),CFDGrade.getString().c_str());
	else
		fprintf(fp,"\t\tCFDGrade = %s    [差价等级]\n",CFDGrade.getString().c_str());
	if (compare(Borrow.getValue(),pOriginal->Borrow.getValue())!=0)
		fprintf(fp,"\t\tBorrow = %s -> %s    [借款周期结算量]\n",pOriginal->Borrow.getString().c_str(),Borrow.getString().c_str());
	else
		fprintf(fp,"\t\tBorrow = %s    [借款周期结算量]\n",Borrow.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(MaxLocalID.getValue(),pOriginal->MaxLocalID.getValue())!=0)
		fprintf(fp,"\t\tMaxLocalID = %s -> %s    [最大本地标识]\n",pOriginal->MaxLocalID.getString().c_str(),MaxLocalID.getString().c_str());
	else
		fprintf(fp,"\t\tMaxLocalID = %s    [最大本地标识]\n",MaxLocalID.getString().c_str());
	if (compare(CurrValue.getValue(),pOriginal->CurrValue.getValue())!=0)
		fprintf(fp,"\t\tCurrValue = %s -> %s    [当前价值]\n",pOriginal->CurrValue.getString().c_str(),CurrValue.getString().c_str());
	else
		fprintf(fp,"\t\tCurrValue = %s    [当前价值]\n",CurrValue.getString().c_str());
	if (compare(CanReduce.getValue(),pOriginal->CanReduce.getValue())!=0)
		fprintf(fp,"\t\tCanReduce = %s -> %s    [是否可以出金]\n",pOriginal->CanReduce.getString().c_str(),CanReduce.getString().c_str());
	else
		fprintf(fp,"\t\tCanReduce = %s    [是否可以出金]\n",CanReduce.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableAccount::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",SettlementGroup=%s",SettlementGroup.getString().c_str());	
	fprintf(fp,",AccountID=%s",AccountID.getString().c_str());	
	fprintf(fp,",Currency=%s",Currency.getString().c_str());	
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",Available=%s",Available.getString().c_str());	
	fprintf(fp,",Withdrawable=%s",Withdrawable.getString().c_str());	
	fprintf(fp,",PreBalance=%s",PreBalance.getString().c_str());	
	fprintf(fp,",Balance=%s",Balance.getString().c_str());	
	fprintf(fp,",PositionCost=%s",PositionCost.getString().c_str());	
	fprintf(fp,",UseMargin=%s",UseMargin.getString().c_str());	
	fprintf(fp,",CrossMargin=%s",CrossMargin.getString().c_str());	
	fprintf(fp,",CloseProfit=%s",CloseProfit.getString().c_str());	
	fprintf(fp,",Deposit=%s",Deposit.getString().c_str());	
	fprintf(fp,",Withdraw=%s",Withdraw.getString().c_str());	
	fprintf(fp,",FrozenMargin=%s",FrozenMargin.getString().c_str());	
	fprintf(fp,",FrozenMoney=%s",FrozenMoney.getString().c_str());	
	fprintf(fp,",FrozenFee=%s",FrozenFee.getString().c_str());	
	fprintf(fp,",Fee=%s",Fee.getString().c_str());	
	fprintf(fp,",MoneyChange=%s",MoneyChange.getString().c_str());	
	fprintf(fp,",Reserve=%s",Reserve.getString().c_str());	
	fprintf(fp,",ReserveAvailable=%s",ReserveAvailable.getString().c_str());	
	fprintf(fp,",ReserveLimit=%s",ReserveLimit.getString().c_str());	
	fprintf(fp,",ReserveProfit=%s",ReserveProfit.getString().c_str());	
	fprintf(fp,",ReserveType=%s",ReserveType.getString().c_str());	
	fprintf(fp,",ReserveActive=%s",ReserveActive.getString().c_str());	
	fprintf(fp,",TotalCloseProfit=%s",TotalCloseProfit.getString().c_str());	
	fprintf(fp,",TotalDeposit=%s",TotalDeposit.getString().c_str());	
	fprintf(fp,",TotalWithdraw=%s",TotalWithdraw.getString().c_str());	
	fprintf(fp,",TotalFee=%s",TotalFee.getString().c_str());	
	fprintf(fp,",TotalMoneyChange=%s",TotalMoneyChange.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,",AccountType=%s",AccountType.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,",MarginRateGrade=%s",MarginRateGrade.getString().c_str());	
	fprintf(fp,",TradingRightGrade=%s",TradingRightGrade.getString().c_str());	
	fprintf(fp,",Frequency=%s",Frequency.getString().c_str());	
	fprintf(fp,",MaintMargin=%s",MaintMargin.getString().c_str());	
	fprintf(fp,",UnRealProfit=%s",UnRealProfit.getString().c_str());	
	fprintf(fp,",CopyProfit=%s",CopyProfit.getString().c_str());	
	fprintf(fp,",CFDType=%s",CFDType.getString().c_str());	
	fprintf(fp,",FeeGrade=%s",FeeGrade.getString().c_str());	
	fprintf(fp,",CFDGrade=%s",CFDGrade.getString().c_str());	
	fprintf(fp,",Borrow=%s",Borrow.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",MaxLocalID=%s",MaxLocalID.getString().c_str());	
	fprintf(fp,",CurrValue=%s",CurrValue.getString().c_str());	
	fprintf(fp,",CanReduce=%s",CanReduce.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableAccount::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"SettlementGroup=%s,",SettlementGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccountID=%s,",AccountID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Currency=%s,",Currency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Available=%s,",Available.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Withdrawable=%s,",Withdrawable.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PreBalance=%s,",PreBalance.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Balance=%s,",Balance.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionCost=%s,",PositionCost.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UseMargin=%s,",UseMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CrossMargin=%s,",CrossMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseProfit=%s,",CloseProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Deposit=%s,",Deposit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Withdraw=%s,",Withdraw.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrozenMargin=%s,",FrozenMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrozenMoney=%s,",FrozenMoney.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrozenFee=%s,",FrozenFee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Fee=%s,",Fee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MoneyChange=%s,",MoneyChange.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Reserve=%s,",Reserve.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ReserveAvailable=%s,",ReserveAvailable.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ReserveLimit=%s,",ReserveLimit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ReserveProfit=%s,",ReserveProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ReserveType=%s,",ReserveType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ReserveActive=%s,",ReserveActive.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TotalCloseProfit=%s,",TotalCloseProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TotalDeposit=%s,",TotalDeposit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TotalWithdraw=%s,",TotalWithdraw.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TotalFee=%s,",TotalFee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TotalMoneyChange=%s,",TotalMoneyChange.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccountType=%s,",AccountType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MarginRateGrade=%s,",MarginRateGrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradingRightGrade=%s,",TradingRightGrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Frequency=%s,",Frequency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MaintMargin=%s,",MaintMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UnRealProfit=%s,",UnRealProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyProfit=%s,",CopyProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CFDType=%s,",CFDType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FeeGrade=%s,",FeeGrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CFDGrade=%s,",CFDGrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Borrow=%s,",Borrow.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MaxLocalID=%s,",MaxLocalID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CurrValue=%s,",CurrValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CanReduce=%s,",CanReduce.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CAccountIterator::dump(FILE *output)
{
	CAccount *pAccount;
	while ((pAccount=next())!=NULL)
	{
		pAccount->dump(output);
	}
}

const CWriteableCurrency *CWriteableAccount::linkCurrency() const
{
	void *target=(void *)(&pCurrency);
	*((const CWriteableCurrency **)target)=m_pMDB->m_CurrencyFactory->findByCurrency(Currency,SettlementGroup);
	return pCurrency;
}

const CWriteableMember *CWriteableAccount::linkMember() const
{
	void *target=(void *)(&pMember);
	*((const CWriteableMember **)target)=m_pMDB->m_MemberFactory->findByMemberID(MemberID);
	return pMember;
}



void CWriteableMemberPosition::writeCSVHead(FILE *output)
{
	fprintf(output,"\"MemberID\",\"AccountID*\",\"ExchangeID*\",\"InstrumentID*\",\"Position\",\"LongFrozen\",\"ShortFrozen\",\"DefaultLeverage\",\"DefaultCrossMargin\",\"InsertTime\",\"UpdateTime\"\n");
}

void CWriteableMemberPosition::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"MemberID\",\"AccountID\",\"ExchangeID\",\"InstrumentID\",\"Position\",\"LongFrozen\",\"ShortFrozen\",\"DefaultLeverage\",\"DefaultCrossMargin\",\"InsertTime\",\"UpdateTime\"\n");
}
	
void CWriteableMemberPosition::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	MemberID.init();
	AccountID.init();
	ExchangeID.init();
	InstrumentID.init();
	Position.init();
	LongFrozen.init();
	ShortFrozen.init();
	DefaultLeverage.clear();
	DefaultCrossMargin.init();
	InsertTime.clear();
	UpdateTime.clear();
}

void CWriteableMemberPosition::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableMemberPosition::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableMemberPosition),input)!=sizeof(CWriteableMemberPosition))
		return 0;
	else
		return 1;
}
	
int CWriteableMemberPosition::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccountID=token;
		else
			AccountID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Position=token;
		else
			Position="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LongFrozen=token;
		else
			LongFrozen="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ShortFrozen=token;
		else
			ShortFrozen="";
		token=getNextToken(NULL);
		if (token!=NULL)
			DefaultLeverage=token;
		else
			DefaultLeverage="";
		token=getNextToken(NULL);
		if (token!=NULL)
			DefaultCrossMargin=token;
		else
			DefaultCrossMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"AccountID"))
			{
				AccountID=token;
			}
			else if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"Position"))
			{
				Position=token;
			}
			else if (!strcmp(fieldName,"LongFrozen"))
			{
				LongFrozen=token;
			}
			else if (!strcmp(fieldName,"ShortFrozen"))
			{
				ShortFrozen=token;
			}
			else if (!strcmp(fieldName,"DefaultLeverage"))
			{
				DefaultLeverage=token;
			}
			else if (!strcmp(fieldName,"DefaultCrossMargin"))
			{
				DefaultCrossMargin=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableMemberPosition::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableMemberPosition),output)!=sizeof(CWriteableMemberPosition))
		return 0;
	else
		return 1;
}

int CWriteableMemberPosition::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Position.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongFrozen.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortFrozen.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",DefaultLeverage.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",DefaultCrossMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableMemberPosition::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Position.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongFrozen.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortFrozen.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",DefaultLeverage.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",DefaultCrossMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableMemberPosition::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCMemberPosition%s = {\n", index_buf);
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());	
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tPosition = %s    [总持仓]\n",Position.getString().c_str());	
	fprintf(fp,"\t\tLongFrozen = %s    [多头冻结]\n",LongFrozen.getString().c_str());	
	fprintf(fp,"\t\tShortFrozen = %s    [空头冻结]\n",ShortFrozen.getString().c_str());	
	fprintf(fp,"\t\tDefaultLeverage = %s    [默认杠杆倍数]\n",DefaultLeverage.getString().c_str());	
	fprintf(fp,"\t\tDefaultCrossMargin = %s    [默认是否全仓]\n",DefaultCrossMargin.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableMemberPosition::dumpDiff(FILE *fp, const CWriteableMemberPosition *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCMemberPosition = {\n");
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(AccountID.getValue(),pOriginal->AccountID.getValue())!=0)
		fprintf(fp,"\t\tAccountID = %s -> %s    [资金账号]\n",pOriginal->AccountID.getString().c_str(),AccountID.getString().c_str());
	else
		fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(Position.getValue(),pOriginal->Position.getValue())!=0)
		fprintf(fp,"\t\tPosition = %s -> %s    [总持仓]\n",pOriginal->Position.getString().c_str(),Position.getString().c_str());
	else
		fprintf(fp,"\t\tPosition = %s    [总持仓]\n",Position.getString().c_str());
	if (compare(LongFrozen.getValue(),pOriginal->LongFrozen.getValue())!=0)
		fprintf(fp,"\t\tLongFrozen = %s -> %s    [多头冻结]\n",pOriginal->LongFrozen.getString().c_str(),LongFrozen.getString().c_str());
	else
		fprintf(fp,"\t\tLongFrozen = %s    [多头冻结]\n",LongFrozen.getString().c_str());
	if (compare(ShortFrozen.getValue(),pOriginal->ShortFrozen.getValue())!=0)
		fprintf(fp,"\t\tShortFrozen = %s -> %s    [空头冻结]\n",pOriginal->ShortFrozen.getString().c_str(),ShortFrozen.getString().c_str());
	else
		fprintf(fp,"\t\tShortFrozen = %s    [空头冻结]\n",ShortFrozen.getString().c_str());
	if (compare(DefaultLeverage.getValue(),pOriginal->DefaultLeverage.getValue())!=0)
		fprintf(fp,"\t\tDefaultLeverage = %s -> %s    [默认杠杆倍数]\n",pOriginal->DefaultLeverage.getString().c_str(),DefaultLeverage.getString().c_str());
	else
		fprintf(fp,"\t\tDefaultLeverage = %s    [默认杠杆倍数]\n",DefaultLeverage.getString().c_str());
	if (compare(DefaultCrossMargin.getValue(),pOriginal->DefaultCrossMargin.getValue())!=0)
		fprintf(fp,"\t\tDefaultCrossMargin = %s -> %s    [默认是否全仓]\n",pOriginal->DefaultCrossMargin.getString().c_str(),DefaultCrossMargin.getString().c_str());
	else
		fprintf(fp,"\t\tDefaultCrossMargin = %s    [默认是否全仓]\n",DefaultCrossMargin.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableMemberPosition::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",AccountID=%s",AccountID.getString().c_str());	
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",Position=%s",Position.getString().c_str());	
	fprintf(fp,",LongFrozen=%s",LongFrozen.getString().c_str());	
	fprintf(fp,",ShortFrozen=%s",ShortFrozen.getString().c_str());	
	fprintf(fp,",DefaultLeverage=%s",DefaultLeverage.getString().c_str());	
	fprintf(fp,",DefaultCrossMargin=%s",DefaultCrossMargin.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableMemberPosition::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccountID=%s,",AccountID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Position=%s,",Position.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LongFrozen=%s,",LongFrozen.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ShortFrozen=%s,",ShortFrozen.getString().c_str());
	p+=strlen(p);
	sprintf(p,"DefaultLeverage=%s,",DefaultLeverage.getString().c_str());
	p+=strlen(p);
	sprintf(p,"DefaultCrossMargin=%s,",DefaultCrossMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CMemberPositionIterator::dump(FILE *output)
{
	CMemberPosition *pMemberPosition;
	while ((pMemberPosition=next())!=NULL)
	{
		pMemberPosition->dump(output);
	}
}



void CWriteablePosition::writeCSVHead(FILE *output)
{
	fprintf(output,"\"PositionID*\",\"MemberID\",\"TradeUnitID\",\"ExchangeID\",\"InstrumentID\",\"PositionClass\",\"PosiDirection\",\"PrePosition\",\"Position\",\"LongFrozen\",\"ShortFrozen\",\"PreLongFrozen\",\"PreShortFrozen\",\"HighestPosition\",\"ClosePosition\",\"PositionCost\",\"CostPrice\",\"UseMargin\",\"FrozenMargin\",\"LongFrozenMargin\",\"ShortFrozenMargin\",\"CloseProfit\",\"TotalPositionCost\",\"TotalCloseProfit\",\"OpenPrice\",\"ClosePrice\",\"TradeFee\",\"PositionFee\",\"Leverage\",\"AccountID\",\"PriceCurrency\",\"ClearCurrency\",\"SettlementGroup\",\"IsCrossMargin\",\"CloseOrderID\",\"SLTriggerPrice\",\"TPTriggerPrice\",\"BeginTime\",\"InsertTime\",\"LastOpenTime\",\"UpdateTime\",\"BusinessNo\",\"IsAutoAddMargin\",\"Frequency\",\"MaintMargin\",\"UnRealProfit\",\"LiquidPrice\",\"CreateTime\",\"CopyMemberID\",\"CopyProfitRate\",\"CopyProfit\",\"FirstTradeID\",\"LastTradeID\",\"BusinessType\",\"BusinessValue\",\"Reserve\",\"ReserveProfit\",\"Remark\"\n");
}

void CWriteablePosition::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"PositionID\",\"MemberID\",\"TradeUnitID\",\"ExchangeID\",\"InstrumentID\",\"PositionClass\",\"PosiDirection\",\"PrePosition\",\"Position\",\"LongFrozen\",\"ShortFrozen\",\"PreLongFrozen\",\"PreShortFrozen\",\"HighestPosition\",\"ClosePosition\",\"PositionCost\",\"CostPrice\",\"UseMargin\",\"FrozenMargin\",\"LongFrozenMargin\",\"ShortFrozenMargin\",\"CloseProfit\",\"TotalPositionCost\",\"TotalCloseProfit\",\"OpenPrice\",\"ClosePrice\",\"TradeFee\",\"PositionFee\",\"Leverage\",\"AccountID\",\"PriceCurrency\",\"ClearCurrency\",\"SettlementGroup\",\"IsCrossMargin\",\"CloseOrderID\",\"SLTriggerPrice\",\"TPTriggerPrice\",\"BeginTime\",\"InsertTime\",\"LastOpenTime\",\"UpdateTime\",\"BusinessNo\",\"IsAutoAddMargin\",\"Frequency\",\"MaintMargin\",\"UnRealProfit\",\"LiquidPrice\",\"CreateTime\",\"CopyMemberID\",\"CopyProfitRate\",\"CopyProfit\",\"FirstTradeID\",\"LastTradeID\",\"BusinessType\",\"BusinessValue\",\"Reserve\",\"ReserveProfit\",\"Remark\"\n");
}
	
void CWriteablePosition::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	PositionID.init();
	MemberID.init();
	TradeUnitID.init();
	ExchangeID.init();
	InstrumentID.init();
	PositionClass.clear();
	PosiDirection.init();
	PrePosition.init();
	Position.init();
	LongFrozen.init();
	ShortFrozen.init();
	PreLongFrozen.init();
	PreShortFrozen.init();
	HighestPosition.init();
	ClosePosition.clear();
	PositionCost.init();
	CostPrice.clear();
	UseMargin.init();
	FrozenMargin.init();
	LongFrozenMargin.init();
	ShortFrozenMargin.init();
	CloseProfit.init();
	TotalPositionCost.init();
	TotalCloseProfit.init();
	OpenPrice.clear();
	ClosePrice.clear();
	TradeFee.init();
	PositionFee.init();
	Leverage.clear();
	AccountID.init();
	PriceCurrency.clear();
	ClearCurrency.clear();
	SettlementGroup.init();
	IsCrossMargin.init();
	CloseOrderID.clear();
	SLTriggerPrice.clear();
	TPTriggerPrice.clear();
	BeginTime.clear();
	InsertTime.clear();
	LastOpenTime.clear();
	UpdateTime.clear();
	BusinessNo.clear();
	IsAutoAddMargin.clear();
	Frequency.clear();
	MaintMargin.clear();
	UnRealProfit.init();
	LiquidPrice.clear();
	CreateTime.clear();
	CopyMemberID.clear();
	CopyProfitRate.clear();
	CopyProfit.init();
	FirstTradeID.clear();
	LastTradeID.clear();
	BusinessType.clear();
	BusinessValue.clear();
	Reserve.clear();
	ReserveProfit.clear();
	Remark.clear();
}

void CWriteablePosition::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteablePosition::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteablePosition),input)!=sizeof(CWriteablePosition))
		return 0;
	else
		return 1;
}
	
int CWriteablePosition::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			PositionID=token;
		else
			PositionID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradeUnitID=token;
		else
			TradeUnitID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionClass=token;
		else
			PositionClass="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PosiDirection=token;
		else
			PosiDirection="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PrePosition=token;
		else
			PrePosition="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Position=token;
		else
			Position="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LongFrozen=token;
		else
			LongFrozen="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ShortFrozen=token;
		else
			ShortFrozen="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PreLongFrozen=token;
		else
			PreLongFrozen="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PreShortFrozen=token;
		else
			PreShortFrozen="";
		token=getNextToken(NULL);
		if (token!=NULL)
			HighestPosition=token;
		else
			HighestPosition="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClosePosition=token;
		else
			ClosePosition="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionCost=token;
		else
			PositionCost="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CostPrice=token;
		else
			CostPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UseMargin=token;
		else
			UseMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrozenMargin=token;
		else
			FrozenMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LongFrozenMargin=token;
		else
			LongFrozenMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ShortFrozenMargin=token;
		else
			ShortFrozenMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseProfit=token;
		else
			CloseProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TotalPositionCost=token;
		else
			TotalPositionCost="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TotalCloseProfit=token;
		else
			TotalCloseProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenPrice=token;
		else
			OpenPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClosePrice=token;
		else
			ClosePrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradeFee=token;
		else
			TradeFee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionFee=token;
		else
			PositionFee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Leverage=token;
		else
			Leverage="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccountID=token;
		else
			AccountID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PriceCurrency=token;
		else
			PriceCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClearCurrency=token;
		else
			ClearCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettlementGroup=token;
		else
			SettlementGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			IsCrossMargin=token;
		else
			IsCrossMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseOrderID=token;
		else
			CloseOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SLTriggerPrice=token;
		else
			SLTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TPTriggerPrice=token;
		else
			TPTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BeginTime=token;
		else
			BeginTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LastOpenTime=token;
		else
			LastOpenTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			IsAutoAddMargin=token;
		else
			IsAutoAddMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Frequency=token;
		else
			Frequency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MaintMargin=token;
		else
			MaintMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UnRealProfit=token;
		else
			UnRealProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LiquidPrice=token;
		else
			LiquidPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CreateTime=token;
		else
			CreateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyMemberID=token;
		else
			CopyMemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyProfitRate=token;
		else
			CopyProfitRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyProfit=token;
		else
			CopyProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FirstTradeID=token;
		else
			FirstTradeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LastTradeID=token;
		else
			LastTradeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessType=token;
		else
			BusinessType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessValue=token;
		else
			BusinessValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Reserve=token;
		else
			Reserve="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ReserveProfit=token;
		else
			ReserveProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"PositionID"))
			{
				PositionID=token;
			}
			else if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"TradeUnitID"))
			{
				TradeUnitID=token;
			}
			else if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"PositionClass"))
			{
				PositionClass=token;
			}
			else if (!strcmp(fieldName,"PosiDirection"))
			{
				PosiDirection=token;
			}
			else if (!strcmp(fieldName,"PrePosition"))
			{
				PrePosition=token;
			}
			else if (!strcmp(fieldName,"Position"))
			{
				Position=token;
			}
			else if (!strcmp(fieldName,"LongFrozen"))
			{
				LongFrozen=token;
			}
			else if (!strcmp(fieldName,"ShortFrozen"))
			{
				ShortFrozen=token;
			}
			else if (!strcmp(fieldName,"PreLongFrozen"))
			{
				PreLongFrozen=token;
			}
			else if (!strcmp(fieldName,"PreShortFrozen"))
			{
				PreShortFrozen=token;
			}
			else if (!strcmp(fieldName,"HighestPosition"))
			{
				HighestPosition=token;
			}
			else if (!strcmp(fieldName,"ClosePosition"))
			{
				ClosePosition=token;
			}
			else if (!strcmp(fieldName,"PositionCost"))
			{
				PositionCost=token;
			}
			else if (!strcmp(fieldName,"CostPrice"))
			{
				CostPrice=token;
			}
			else if (!strcmp(fieldName,"UseMargin"))
			{
				UseMargin=token;
			}
			else if (!strcmp(fieldName,"FrozenMargin"))
			{
				FrozenMargin=token;
			}
			else if (!strcmp(fieldName,"LongFrozenMargin"))
			{
				LongFrozenMargin=token;
			}
			else if (!strcmp(fieldName,"ShortFrozenMargin"))
			{
				ShortFrozenMargin=token;
			}
			else if (!strcmp(fieldName,"CloseProfit"))
			{
				CloseProfit=token;
			}
			else if (!strcmp(fieldName,"TotalPositionCost"))
			{
				TotalPositionCost=token;
			}
			else if (!strcmp(fieldName,"TotalCloseProfit"))
			{
				TotalCloseProfit=token;
			}
			else if (!strcmp(fieldName,"OpenPrice"))
			{
				OpenPrice=token;
			}
			else if (!strcmp(fieldName,"ClosePrice"))
			{
				ClosePrice=token;
			}
			else if (!strcmp(fieldName,"TradeFee"))
			{
				TradeFee=token;
			}
			else if (!strcmp(fieldName,"PositionFee"))
			{
				PositionFee=token;
			}
			else if (!strcmp(fieldName,"Leverage"))
			{
				Leverage=token;
			}
			else if (!strcmp(fieldName,"AccountID"))
			{
				AccountID=token;
			}
			else if (!strcmp(fieldName,"PriceCurrency"))
			{
				PriceCurrency=token;
			}
			else if (!strcmp(fieldName,"ClearCurrency"))
			{
				ClearCurrency=token;
			}
			else if (!strcmp(fieldName,"SettlementGroup"))
			{
				SettlementGroup=token;
			}
			else if (!strcmp(fieldName,"IsCrossMargin"))
			{
				IsCrossMargin=token;
			}
			else if (!strcmp(fieldName,"CloseOrderID"))
			{
				CloseOrderID=token;
			}
			else if (!strcmp(fieldName,"SLTriggerPrice"))
			{
				SLTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"TPTriggerPrice"))
			{
				TPTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"BeginTime"))
			{
				BeginTime=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"LastOpenTime"))
			{
				LastOpenTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			else if (!strcmp(fieldName,"IsAutoAddMargin"))
			{
				IsAutoAddMargin=token;
			}
			else if (!strcmp(fieldName,"Frequency"))
			{
				Frequency=token;
			}
			else if (!strcmp(fieldName,"MaintMargin"))
			{
				MaintMargin=token;
			}
			else if (!strcmp(fieldName,"UnRealProfit"))
			{
				UnRealProfit=token;
			}
			else if (!strcmp(fieldName,"LiquidPrice"))
			{
				LiquidPrice=token;
			}
			else if (!strcmp(fieldName,"CreateTime"))
			{
				CreateTime=token;
			}
			else if (!strcmp(fieldName,"CopyMemberID"))
			{
				CopyMemberID=token;
			}
			else if (!strcmp(fieldName,"CopyProfitRate"))
			{
				CopyProfitRate=token;
			}
			else if (!strcmp(fieldName,"CopyProfit"))
			{
				CopyProfit=token;
			}
			else if (!strcmp(fieldName,"FirstTradeID"))
			{
				FirstTradeID=token;
			}
			else if (!strcmp(fieldName,"LastTradeID"))
			{
				LastTradeID=token;
			}
			else if (!strcmp(fieldName,"BusinessType"))
			{
				BusinessType=token;
			}
			else if (!strcmp(fieldName,"BusinessValue"))
			{
				BusinessValue=token;
			}
			else if (!strcmp(fieldName,"Reserve"))
			{
				Reserve=token;
			}
			else if (!strcmp(fieldName,"ReserveProfit"))
			{
				ReserveProfit=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteablePosition::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteablePosition),output)!=sizeof(CWriteablePosition))
		return 0;
	else
		return 1;
}

int CWriteablePosition::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",PositionID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeUnitID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionClass.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PosiDirection.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PrePosition.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Position.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongFrozen.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortFrozen.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreLongFrozen.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreShortFrozen.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPosition.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePosition.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionCost.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CostPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UseMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongFrozenMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortFrozenMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalPositionCost.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalCloseProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeFee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionFee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Leverage.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClearCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsCrossMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SLTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TPTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BeginTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastOpenTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsAutoAddMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Frequency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaintMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UnRealProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LiquidPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfitRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FirstTradeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastTradeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Reserve.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteablePosition::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",PositionID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeUnitID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionClass.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PosiDirection.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PrePosition.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Position.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongFrozen.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortFrozen.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreLongFrozen.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreShortFrozen.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPosition.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePosition.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionCost.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CostPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UseMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LongFrozenMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ShortFrozenMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalPositionCost.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TotalCloseProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeFee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionFee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Leverage.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClearCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsCrossMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SLTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TPTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BeginTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastOpenTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsAutoAddMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Frequency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MaintMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UnRealProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LiquidPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfitRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FirstTradeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastTradeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Reserve.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteablePosition::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCPosition%s = {\n", index_buf);
	fprintf(fp,"\t\tPositionID = %s    [持仓代码]\n",PositionID.getString().c_str());	
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tTradeUnitID = %s    [交易单元代码]\n",TradeUnitID.getString().c_str());	
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tPositionClass = %s    [持仓类型]\n",PositionClass.getString().c_str());	
	fprintf(fp,"\t\tPosiDirection = %s    [持仓多空方向]\n",PosiDirection.getString().c_str());	
	fprintf(fp,"\t\tPrePosition = %s    [上次持仓]\n",PrePosition.getString().c_str());	
	fprintf(fp,"\t\tPosition = %s    [总持仓]\n",Position.getString().c_str());	
	fprintf(fp,"\t\tLongFrozen = %s    [多头冻结]\n",LongFrozen.getString().c_str());	
	fprintf(fp,"\t\tShortFrozen = %s    [空头冻结]\n",ShortFrozen.getString().c_str());	
	fprintf(fp,"\t\tPreLongFrozen = %s    [昨日多头冻结]\n",PreLongFrozen.getString().c_str());	
	fprintf(fp,"\t\tPreShortFrozen = %s    [昨日空头冻结]\n",PreShortFrozen.getString().c_str());	
	fprintf(fp,"\t\tHighestPosition = %s    [最大持仓]\n",HighestPosition.getString().c_str());	
	fprintf(fp,"\t\tClosePosition = %s    [可平持仓]\n",ClosePosition.getString().c_str());	
	fprintf(fp,"\t\tPositionCost = %s    [持仓成本]\n",PositionCost.getString().c_str());	
	fprintf(fp,"\t\tCostPrice = %s    [成本价]\n",CostPrice.getString().c_str());	
	fprintf(fp,"\t\tUseMargin = %s    [占用保证金]\n",UseMargin.getString().c_str());	
	fprintf(fp,"\t\tFrozenMargin = %s    [冻结保证金]\n",FrozenMargin.getString().c_str());	
	fprintf(fp,"\t\tLongFrozenMargin = %s    [多头冻结保证金]\n",LongFrozenMargin.getString().c_str());	
	fprintf(fp,"\t\tShortFrozenMargin = %s    [空头冻结保证金]\n",ShortFrozenMargin.getString().c_str());	
	fprintf(fp,"\t\tCloseProfit = %s    [平仓盈亏]\n",CloseProfit.getString().c_str());	
	fprintf(fp,"\t\tTotalPositionCost = %s    [开仓成本]\n",TotalPositionCost.getString().c_str());	
	fprintf(fp,"\t\tTotalCloseProfit = %s    [总平仓盈亏]\n",TotalCloseProfit.getString().c_str());	
	fprintf(fp,"\t\tOpenPrice = %s    [开仓均价]\n",OpenPrice.getString().c_str());	
	fprintf(fp,"\t\tClosePrice = %s    [平仓均价]\n",ClosePrice.getString().c_str());	
	fprintf(fp,"\t\tTradeFee = %s    [交易费用]\n",TradeFee.getString().c_str());	
	fprintf(fp,"\t\tPositionFee = %s    [持仓费用或者股票分红等]\n",PositionFee.getString().c_str());	
	fprintf(fp,"\t\tLeverage = %s    [杠杆倍数]\n",Leverage.getString().c_str());	
	fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());	
	fprintf(fp,"\t\tPriceCurrency = %s    [计价币种]\n",PriceCurrency.getString().c_str());	
	fprintf(fp,"\t\tClearCurrency = %s    [清算币种]\n",ClearCurrency.getString().c_str());	
	fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());	
	fprintf(fp,"\t\tIsCrossMargin = %s    [是否全仓]\n",IsCrossMargin.getString().c_str());	
	fprintf(fp,"\t\tCloseOrderID = %s    [平仓单代码]\n",CloseOrderID.getString().c_str());	
	fprintf(fp,"\t\tSLTriggerPrice = %s    [止损触发价]\n",SLTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tTPTriggerPrice = %s    [止盈触发价]\n",TPTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tBeginTime = %s    [持仓不为0的开始时间]\n",BeginTime.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间(新持仓时间)]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tLastOpenTime = %s    [最后开仓时间]\n",LastOpenTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t\tIsAutoAddMargin = %s    [是否自动追加保证金]\n",IsAutoAddMargin.getString().c_str());	
	fprintf(fp,"\t\tFrequency = %s    [每秒更新的次数]\n",Frequency.getString().c_str());	
	fprintf(fp,"\t\tMaintMargin = %s    [维持保证金]\n",MaintMargin.getString().c_str());	
	fprintf(fp,"\t\tUnRealProfit = %s    [未实现盈亏]\n",UnRealProfit.getString().c_str());	
	fprintf(fp,"\t\tLiquidPrice = %s    [清算价格]\n",LiquidPrice.getString().c_str());	
	fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());	
	fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());	
	fprintf(fp,"\t\tCopyProfitRate = %s    [带单分配比例]\n",CopyProfitRate.getString().c_str());	
	fprintf(fp,"\t\tCopyProfit = %s    [带单盈利分配]\n",CopyProfit.getString().c_str());	
	fprintf(fp,"\t\tFirstTradeID = %s    [初始成交订单号]\n",FirstTradeID.getString().c_str());	
	fprintf(fp,"\t\tLastTradeID = %s    [结束成交订单号]\n",LastTradeID.getString().c_str());	
	fprintf(fp,"\t\tBusinessType = %s    [业务类别]\n",BusinessType.getString().c_str());	
	fprintf(fp,"\t\tBusinessValue = %s    [业务值]\n",BusinessValue.getString().c_str());	
	fprintf(fp,"\t\tReserve = %s    [保留资金]\n",Reserve.getString().c_str());	
	fprintf(fp,"\t\tReserveProfit = %s    [保留资金盈亏]\n",ReserveProfit.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteablePosition::dumpDiff(FILE *fp, const CWriteablePosition *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCPosition = {\n");
	if (compare(PositionID.getValue(),pOriginal->PositionID.getValue())!=0)
		fprintf(fp,"\t\tPositionID = %s -> %s    [持仓代码]\n",pOriginal->PositionID.getString().c_str(),PositionID.getString().c_str());
	else
		fprintf(fp,"\t\tPositionID = %s    [持仓代码]\n",PositionID.getString().c_str());
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(TradeUnitID.getValue(),pOriginal->TradeUnitID.getValue())!=0)
		fprintf(fp,"\t\tTradeUnitID = %s -> %s    [交易单元代码]\n",pOriginal->TradeUnitID.getString().c_str(),TradeUnitID.getString().c_str());
	else
		fprintf(fp,"\t\tTradeUnitID = %s    [交易单元代码]\n",TradeUnitID.getString().c_str());
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(PositionClass.getValue(),pOriginal->PositionClass.getValue())!=0)
		fprintf(fp,"\t\tPositionClass = %s -> %s    [持仓类型]\n",pOriginal->PositionClass.getString().c_str(),PositionClass.getString().c_str());
	else
		fprintf(fp,"\t\tPositionClass = %s    [持仓类型]\n",PositionClass.getString().c_str());
	if (compare(PosiDirection.getValue(),pOriginal->PosiDirection.getValue())!=0)
		fprintf(fp,"\t\tPosiDirection = %s -> %s    [持仓多空方向]\n",pOriginal->PosiDirection.getString().c_str(),PosiDirection.getString().c_str());
	else
		fprintf(fp,"\t\tPosiDirection = %s    [持仓多空方向]\n",PosiDirection.getString().c_str());
	if (compare(PrePosition.getValue(),pOriginal->PrePosition.getValue())!=0)
		fprintf(fp,"\t\tPrePosition = %s -> %s    [上次持仓]\n",pOriginal->PrePosition.getString().c_str(),PrePosition.getString().c_str());
	else
		fprintf(fp,"\t\tPrePosition = %s    [上次持仓]\n",PrePosition.getString().c_str());
	if (compare(Position.getValue(),pOriginal->Position.getValue())!=0)
		fprintf(fp,"\t\tPosition = %s -> %s    [总持仓]\n",pOriginal->Position.getString().c_str(),Position.getString().c_str());
	else
		fprintf(fp,"\t\tPosition = %s    [总持仓]\n",Position.getString().c_str());
	if (compare(LongFrozen.getValue(),pOriginal->LongFrozen.getValue())!=0)
		fprintf(fp,"\t\tLongFrozen = %s -> %s    [多头冻结]\n",pOriginal->LongFrozen.getString().c_str(),LongFrozen.getString().c_str());
	else
		fprintf(fp,"\t\tLongFrozen = %s    [多头冻结]\n",LongFrozen.getString().c_str());
	if (compare(ShortFrozen.getValue(),pOriginal->ShortFrozen.getValue())!=0)
		fprintf(fp,"\t\tShortFrozen = %s -> %s    [空头冻结]\n",pOriginal->ShortFrozen.getString().c_str(),ShortFrozen.getString().c_str());
	else
		fprintf(fp,"\t\tShortFrozen = %s    [空头冻结]\n",ShortFrozen.getString().c_str());
	if (compare(PreLongFrozen.getValue(),pOriginal->PreLongFrozen.getValue())!=0)
		fprintf(fp,"\t\tPreLongFrozen = %s -> %s    [昨日多头冻结]\n",pOriginal->PreLongFrozen.getString().c_str(),PreLongFrozen.getString().c_str());
	else
		fprintf(fp,"\t\tPreLongFrozen = %s    [昨日多头冻结]\n",PreLongFrozen.getString().c_str());
	if (compare(PreShortFrozen.getValue(),pOriginal->PreShortFrozen.getValue())!=0)
		fprintf(fp,"\t\tPreShortFrozen = %s -> %s    [昨日空头冻结]\n",pOriginal->PreShortFrozen.getString().c_str(),PreShortFrozen.getString().c_str());
	else
		fprintf(fp,"\t\tPreShortFrozen = %s    [昨日空头冻结]\n",PreShortFrozen.getString().c_str());
	if (compare(HighestPosition.getValue(),pOriginal->HighestPosition.getValue())!=0)
		fprintf(fp,"\t\tHighestPosition = %s -> %s    [最大持仓]\n",pOriginal->HighestPosition.getString().c_str(),HighestPosition.getString().c_str());
	else
		fprintf(fp,"\t\tHighestPosition = %s    [最大持仓]\n",HighestPosition.getString().c_str());
	if (compare(ClosePosition.getValue(),pOriginal->ClosePosition.getValue())!=0)
		fprintf(fp,"\t\tClosePosition = %s -> %s    [可平持仓]\n",pOriginal->ClosePosition.getString().c_str(),ClosePosition.getString().c_str());
	else
		fprintf(fp,"\t\tClosePosition = %s    [可平持仓]\n",ClosePosition.getString().c_str());
	if (compare(PositionCost.getValue(),pOriginal->PositionCost.getValue())!=0)
		fprintf(fp,"\t\tPositionCost = %s -> %s    [持仓成本]\n",pOriginal->PositionCost.getString().c_str(),PositionCost.getString().c_str());
	else
		fprintf(fp,"\t\tPositionCost = %s    [持仓成本]\n",PositionCost.getString().c_str());
	if (compare(CostPrice.getValue(),pOriginal->CostPrice.getValue())!=0)
		fprintf(fp,"\t\tCostPrice = %s -> %s    [成本价]\n",pOriginal->CostPrice.getString().c_str(),CostPrice.getString().c_str());
	else
		fprintf(fp,"\t\tCostPrice = %s    [成本价]\n",CostPrice.getString().c_str());
	if (compare(UseMargin.getValue(),pOriginal->UseMargin.getValue())!=0)
		fprintf(fp,"\t\tUseMargin = %s -> %s    [占用保证金]\n",pOriginal->UseMargin.getString().c_str(),UseMargin.getString().c_str());
	else
		fprintf(fp,"\t\tUseMargin = %s    [占用保证金]\n",UseMargin.getString().c_str());
	if (compare(FrozenMargin.getValue(),pOriginal->FrozenMargin.getValue())!=0)
		fprintf(fp,"\t\tFrozenMargin = %s -> %s    [冻结保证金]\n",pOriginal->FrozenMargin.getString().c_str(),FrozenMargin.getString().c_str());
	else
		fprintf(fp,"\t\tFrozenMargin = %s    [冻结保证金]\n",FrozenMargin.getString().c_str());
	if (compare(LongFrozenMargin.getValue(),pOriginal->LongFrozenMargin.getValue())!=0)
		fprintf(fp,"\t\tLongFrozenMargin = %s -> %s    [多头冻结保证金]\n",pOriginal->LongFrozenMargin.getString().c_str(),LongFrozenMargin.getString().c_str());
	else
		fprintf(fp,"\t\tLongFrozenMargin = %s    [多头冻结保证金]\n",LongFrozenMargin.getString().c_str());
	if (compare(ShortFrozenMargin.getValue(),pOriginal->ShortFrozenMargin.getValue())!=0)
		fprintf(fp,"\t\tShortFrozenMargin = %s -> %s    [空头冻结保证金]\n",pOriginal->ShortFrozenMargin.getString().c_str(),ShortFrozenMargin.getString().c_str());
	else
		fprintf(fp,"\t\tShortFrozenMargin = %s    [空头冻结保证金]\n",ShortFrozenMargin.getString().c_str());
	if (compare(CloseProfit.getValue(),pOriginal->CloseProfit.getValue())!=0)
		fprintf(fp,"\t\tCloseProfit = %s -> %s    [平仓盈亏]\n",pOriginal->CloseProfit.getString().c_str(),CloseProfit.getString().c_str());
	else
		fprintf(fp,"\t\tCloseProfit = %s    [平仓盈亏]\n",CloseProfit.getString().c_str());
	if (compare(TotalPositionCost.getValue(),pOriginal->TotalPositionCost.getValue())!=0)
		fprintf(fp,"\t\tTotalPositionCost = %s -> %s    [开仓成本]\n",pOriginal->TotalPositionCost.getString().c_str(),TotalPositionCost.getString().c_str());
	else
		fprintf(fp,"\t\tTotalPositionCost = %s    [开仓成本]\n",TotalPositionCost.getString().c_str());
	if (compare(TotalCloseProfit.getValue(),pOriginal->TotalCloseProfit.getValue())!=0)
		fprintf(fp,"\t\tTotalCloseProfit = %s -> %s    [总平仓盈亏]\n",pOriginal->TotalCloseProfit.getString().c_str(),TotalCloseProfit.getString().c_str());
	else
		fprintf(fp,"\t\tTotalCloseProfit = %s    [总平仓盈亏]\n",TotalCloseProfit.getString().c_str());
	if (compare(OpenPrice.getValue(),pOriginal->OpenPrice.getValue())!=0)
		fprintf(fp,"\t\tOpenPrice = %s -> %s    [开仓均价]\n",pOriginal->OpenPrice.getString().c_str(),OpenPrice.getString().c_str());
	else
		fprintf(fp,"\t\tOpenPrice = %s    [开仓均价]\n",OpenPrice.getString().c_str());
	if (compare(ClosePrice.getValue(),pOriginal->ClosePrice.getValue())!=0)
		fprintf(fp,"\t\tClosePrice = %s -> %s    [平仓均价]\n",pOriginal->ClosePrice.getString().c_str(),ClosePrice.getString().c_str());
	else
		fprintf(fp,"\t\tClosePrice = %s    [平仓均价]\n",ClosePrice.getString().c_str());
	if (compare(TradeFee.getValue(),pOriginal->TradeFee.getValue())!=0)
		fprintf(fp,"\t\tTradeFee = %s -> %s    [交易费用]\n",pOriginal->TradeFee.getString().c_str(),TradeFee.getString().c_str());
	else
		fprintf(fp,"\t\tTradeFee = %s    [交易费用]\n",TradeFee.getString().c_str());
	if (compare(PositionFee.getValue(),pOriginal->PositionFee.getValue())!=0)
		fprintf(fp,"\t\tPositionFee = %s -> %s    [持仓费用或者股票分红等]\n",pOriginal->PositionFee.getString().c_str(),PositionFee.getString().c_str());
	else
		fprintf(fp,"\t\tPositionFee = %s    [持仓费用或者股票分红等]\n",PositionFee.getString().c_str());
	if (compare(Leverage.getValue(),pOriginal->Leverage.getValue())!=0)
		fprintf(fp,"\t\tLeverage = %s -> %s    [杠杆倍数]\n",pOriginal->Leverage.getString().c_str(),Leverage.getString().c_str());
	else
		fprintf(fp,"\t\tLeverage = %s    [杠杆倍数]\n",Leverage.getString().c_str());
	if (compare(AccountID.getValue(),pOriginal->AccountID.getValue())!=0)
		fprintf(fp,"\t\tAccountID = %s -> %s    [资金账号]\n",pOriginal->AccountID.getString().c_str(),AccountID.getString().c_str());
	else
		fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());
	if (compare(PriceCurrency.getValue(),pOriginal->PriceCurrency.getValue())!=0)
		fprintf(fp,"\t\tPriceCurrency = %s -> %s    [计价币种]\n",pOriginal->PriceCurrency.getString().c_str(),PriceCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tPriceCurrency = %s    [计价币种]\n",PriceCurrency.getString().c_str());
	if (compare(ClearCurrency.getValue(),pOriginal->ClearCurrency.getValue())!=0)
		fprintf(fp,"\t\tClearCurrency = %s -> %s    [清算币种]\n",pOriginal->ClearCurrency.getString().c_str(),ClearCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tClearCurrency = %s    [清算币种]\n",ClearCurrency.getString().c_str());
	if (compare(SettlementGroup.getValue(),pOriginal->SettlementGroup.getValue())!=0)
		fprintf(fp,"\t\tSettlementGroup = %s -> %s    [结算组编号]\n",pOriginal->SettlementGroup.getString().c_str(),SettlementGroup.getString().c_str());
	else
		fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());
	if (compare(IsCrossMargin.getValue(),pOriginal->IsCrossMargin.getValue())!=0)
		fprintf(fp,"\t\tIsCrossMargin = %s -> %s    [是否全仓]\n",pOriginal->IsCrossMargin.getString().c_str(),IsCrossMargin.getString().c_str());
	else
		fprintf(fp,"\t\tIsCrossMargin = %s    [是否全仓]\n",IsCrossMargin.getString().c_str());
	if (compare(CloseOrderID.getValue(),pOriginal->CloseOrderID.getValue())!=0)
		fprintf(fp,"\t\tCloseOrderID = %s -> %s    [平仓单代码]\n",pOriginal->CloseOrderID.getString().c_str(),CloseOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tCloseOrderID = %s    [平仓单代码]\n",CloseOrderID.getString().c_str());
	if (compare(SLTriggerPrice.getValue(),pOriginal->SLTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tSLTriggerPrice = %s -> %s    [止损触发价]\n",pOriginal->SLTriggerPrice.getString().c_str(),SLTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tSLTriggerPrice = %s    [止损触发价]\n",SLTriggerPrice.getString().c_str());
	if (compare(TPTriggerPrice.getValue(),pOriginal->TPTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tTPTriggerPrice = %s -> %s    [止盈触发价]\n",pOriginal->TPTriggerPrice.getString().c_str(),TPTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tTPTriggerPrice = %s    [止盈触发价]\n",TPTriggerPrice.getString().c_str());
	if (compare(BeginTime.getValue(),pOriginal->BeginTime.getValue())!=0)
		fprintf(fp,"\t\tBeginTime = %s -> %s    [持仓不为0的开始时间]\n",pOriginal->BeginTime.getString().c_str(),BeginTime.getString().c_str());
	else
		fprintf(fp,"\t\tBeginTime = %s    [持仓不为0的开始时间]\n",BeginTime.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间(新持仓时间)]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间(新持仓时间)]\n",InsertTime.getString().c_str());
	if (compare(LastOpenTime.getValue(),pOriginal->LastOpenTime.getValue())!=0)
		fprintf(fp,"\t\tLastOpenTime = %s -> %s    [最后开仓时间]\n",pOriginal->LastOpenTime.getString().c_str(),LastOpenTime.getString().c_str());
	else
		fprintf(fp,"\t\tLastOpenTime = %s    [最后开仓时间]\n",LastOpenTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [业务序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());
	if (compare(IsAutoAddMargin.getValue(),pOriginal->IsAutoAddMargin.getValue())!=0)
		fprintf(fp,"\t\tIsAutoAddMargin = %s -> %s    [是否自动追加保证金]\n",pOriginal->IsAutoAddMargin.getString().c_str(),IsAutoAddMargin.getString().c_str());
	else
		fprintf(fp,"\t\tIsAutoAddMargin = %s    [是否自动追加保证金]\n",IsAutoAddMargin.getString().c_str());
	if (compare(Frequency.getValue(),pOriginal->Frequency.getValue())!=0)
		fprintf(fp,"\t\tFrequency = %s -> %s    [每秒更新的次数]\n",pOriginal->Frequency.getString().c_str(),Frequency.getString().c_str());
	else
		fprintf(fp,"\t\tFrequency = %s    [每秒更新的次数]\n",Frequency.getString().c_str());
	if (compare(MaintMargin.getValue(),pOriginal->MaintMargin.getValue())!=0)
		fprintf(fp,"\t\tMaintMargin = %s -> %s    [维持保证金]\n",pOriginal->MaintMargin.getString().c_str(),MaintMargin.getString().c_str());
	else
		fprintf(fp,"\t\tMaintMargin = %s    [维持保证金]\n",MaintMargin.getString().c_str());
	if (compare(UnRealProfit.getValue(),pOriginal->UnRealProfit.getValue())!=0)
		fprintf(fp,"\t\tUnRealProfit = %s -> %s    [未实现盈亏]\n",pOriginal->UnRealProfit.getString().c_str(),UnRealProfit.getString().c_str());
	else
		fprintf(fp,"\t\tUnRealProfit = %s    [未实现盈亏]\n",UnRealProfit.getString().c_str());
	if (compare(LiquidPrice.getValue(),pOriginal->LiquidPrice.getValue())!=0)
		fprintf(fp,"\t\tLiquidPrice = %s -> %s    [清算价格]\n",pOriginal->LiquidPrice.getString().c_str(),LiquidPrice.getString().c_str());
	else
		fprintf(fp,"\t\tLiquidPrice = %s    [清算价格]\n",LiquidPrice.getString().c_str());
	if (compare(CreateTime.getValue(),pOriginal->CreateTime.getValue())!=0)
		fprintf(fp,"\t\tCreateTime = %s -> %s    [创建时间]\n",pOriginal->CreateTime.getString().c_str(),CreateTime.getString().c_str());
	else
		fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());
	if (compare(CopyMemberID.getValue(),pOriginal->CopyMemberID.getValue())!=0)
		fprintf(fp,"\t\tCopyMemberID = %s -> %s    [带单员代码]\n",pOriginal->CopyMemberID.getString().c_str(),CopyMemberID.getString().c_str());
	else
		fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());
	if (compare(CopyProfitRate.getValue(),pOriginal->CopyProfitRate.getValue())!=0)
		fprintf(fp,"\t\tCopyProfitRate = %s -> %s    [带单分配比例]\n",pOriginal->CopyProfitRate.getString().c_str(),CopyProfitRate.getString().c_str());
	else
		fprintf(fp,"\t\tCopyProfitRate = %s    [带单分配比例]\n",CopyProfitRate.getString().c_str());
	if (compare(CopyProfit.getValue(),pOriginal->CopyProfit.getValue())!=0)
		fprintf(fp,"\t\tCopyProfit = %s -> %s    [带单盈利分配]\n",pOriginal->CopyProfit.getString().c_str(),CopyProfit.getString().c_str());
	else
		fprintf(fp,"\t\tCopyProfit = %s    [带单盈利分配]\n",CopyProfit.getString().c_str());
	if (compare(FirstTradeID.getValue(),pOriginal->FirstTradeID.getValue())!=0)
		fprintf(fp,"\t\tFirstTradeID = %s -> %s    [初始成交订单号]\n",pOriginal->FirstTradeID.getString().c_str(),FirstTradeID.getString().c_str());
	else
		fprintf(fp,"\t\tFirstTradeID = %s    [初始成交订单号]\n",FirstTradeID.getString().c_str());
	if (compare(LastTradeID.getValue(),pOriginal->LastTradeID.getValue())!=0)
		fprintf(fp,"\t\tLastTradeID = %s -> %s    [结束成交订单号]\n",pOriginal->LastTradeID.getString().c_str(),LastTradeID.getString().c_str());
	else
		fprintf(fp,"\t\tLastTradeID = %s    [结束成交订单号]\n",LastTradeID.getString().c_str());
	if (compare(BusinessType.getValue(),pOriginal->BusinessType.getValue())!=0)
		fprintf(fp,"\t\tBusinessType = %s -> %s    [业务类别]\n",pOriginal->BusinessType.getString().c_str(),BusinessType.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessType = %s    [业务类别]\n",BusinessType.getString().c_str());
	if (compare(BusinessValue.getValue(),pOriginal->BusinessValue.getValue())!=0)
		fprintf(fp,"\t\tBusinessValue = %s -> %s    [业务值]\n",pOriginal->BusinessValue.getString().c_str(),BusinessValue.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessValue = %s    [业务值]\n",BusinessValue.getString().c_str());
	if (compare(Reserve.getValue(),pOriginal->Reserve.getValue())!=0)
		fprintf(fp,"\t\tReserve = %s -> %s    [保留资金]\n",pOriginal->Reserve.getString().c_str(),Reserve.getString().c_str());
	else
		fprintf(fp,"\t\tReserve = %s    [保留资金]\n",Reserve.getString().c_str());
	if (compare(ReserveProfit.getValue(),pOriginal->ReserveProfit.getValue())!=0)
		fprintf(fp,"\t\tReserveProfit = %s -> %s    [保留资金盈亏]\n",pOriginal->ReserveProfit.getString().c_str(),ReserveProfit.getString().c_str());
	else
		fprintf(fp,"\t\tReserveProfit = %s    [保留资金盈亏]\n",ReserveProfit.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteablePosition::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",PositionID=%s",PositionID.getString().c_str());	
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",TradeUnitID=%s",TradeUnitID.getString().c_str());	
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",PositionClass=%s",PositionClass.getString().c_str());	
	fprintf(fp,",PosiDirection=%s",PosiDirection.getString().c_str());	
	fprintf(fp,",PrePosition=%s",PrePosition.getString().c_str());	
	fprintf(fp,",Position=%s",Position.getString().c_str());	
	fprintf(fp,",LongFrozen=%s",LongFrozen.getString().c_str());	
	fprintf(fp,",ShortFrozen=%s",ShortFrozen.getString().c_str());	
	fprintf(fp,",PreLongFrozen=%s",PreLongFrozen.getString().c_str());	
	fprintf(fp,",PreShortFrozen=%s",PreShortFrozen.getString().c_str());	
	fprintf(fp,",HighestPosition=%s",HighestPosition.getString().c_str());	
	fprintf(fp,",ClosePosition=%s",ClosePosition.getString().c_str());	
	fprintf(fp,",PositionCost=%s",PositionCost.getString().c_str());	
	fprintf(fp,",CostPrice=%s",CostPrice.getString().c_str());	
	fprintf(fp,",UseMargin=%s",UseMargin.getString().c_str());	
	fprintf(fp,",FrozenMargin=%s",FrozenMargin.getString().c_str());	
	fprintf(fp,",LongFrozenMargin=%s",LongFrozenMargin.getString().c_str());	
	fprintf(fp,",ShortFrozenMargin=%s",ShortFrozenMargin.getString().c_str());	
	fprintf(fp,",CloseProfit=%s",CloseProfit.getString().c_str());	
	fprintf(fp,",TotalPositionCost=%s",TotalPositionCost.getString().c_str());	
	fprintf(fp,",TotalCloseProfit=%s",TotalCloseProfit.getString().c_str());	
	fprintf(fp,",OpenPrice=%s",OpenPrice.getString().c_str());	
	fprintf(fp,",ClosePrice=%s",ClosePrice.getString().c_str());	
	fprintf(fp,",TradeFee=%s",TradeFee.getString().c_str());	
	fprintf(fp,",PositionFee=%s",PositionFee.getString().c_str());	
	fprintf(fp,",Leverage=%s",Leverage.getString().c_str());	
	fprintf(fp,",AccountID=%s",AccountID.getString().c_str());	
	fprintf(fp,",PriceCurrency=%s",PriceCurrency.getString().c_str());	
	fprintf(fp,",ClearCurrency=%s",ClearCurrency.getString().c_str());	
	fprintf(fp,",SettlementGroup=%s",SettlementGroup.getString().c_str());	
	fprintf(fp,",IsCrossMargin=%s",IsCrossMargin.getString().c_str());	
	fprintf(fp,",CloseOrderID=%s",CloseOrderID.getString().c_str());	
	fprintf(fp,",SLTriggerPrice=%s",SLTriggerPrice.getString().c_str());	
	fprintf(fp,",TPTriggerPrice=%s",TPTriggerPrice.getString().c_str());	
	fprintf(fp,",BeginTime=%s",BeginTime.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",LastOpenTime=%s",LastOpenTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,",IsAutoAddMargin=%s",IsAutoAddMargin.getString().c_str());	
	fprintf(fp,",Frequency=%s",Frequency.getString().c_str());	
	fprintf(fp,",MaintMargin=%s",MaintMargin.getString().c_str());	
	fprintf(fp,",UnRealProfit=%s",UnRealProfit.getString().c_str());	
	fprintf(fp,",LiquidPrice=%s",LiquidPrice.getString().c_str());	
	fprintf(fp,",CreateTime=%s",CreateTime.getString().c_str());	
	fprintf(fp,",CopyMemberID=%s",CopyMemberID.getString().c_str());	
	fprintf(fp,",CopyProfitRate=%s",CopyProfitRate.getString().c_str());	
	fprintf(fp,",CopyProfit=%s",CopyProfit.getString().c_str());	
	fprintf(fp,",FirstTradeID=%s",FirstTradeID.getString().c_str());	
	fprintf(fp,",LastTradeID=%s",LastTradeID.getString().c_str());	
	fprintf(fp,",BusinessType=%s",BusinessType.getString().c_str());	
	fprintf(fp,",BusinessValue=%s",BusinessValue.getString().c_str());	
	fprintf(fp,",Reserve=%s",Reserve.getString().c_str());	
	fprintf(fp,",ReserveProfit=%s",ReserveProfit.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteablePosition::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"PositionID=%s,",PositionID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradeUnitID=%s,",TradeUnitID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionClass=%s,",PositionClass.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PosiDirection=%s,",PosiDirection.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PrePosition=%s,",PrePosition.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Position=%s,",Position.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LongFrozen=%s,",LongFrozen.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ShortFrozen=%s,",ShortFrozen.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PreLongFrozen=%s,",PreLongFrozen.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PreShortFrozen=%s,",PreShortFrozen.getString().c_str());
	p+=strlen(p);
	sprintf(p,"HighestPosition=%s,",HighestPosition.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClosePosition=%s,",ClosePosition.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionCost=%s,",PositionCost.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CostPrice=%s,",CostPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UseMargin=%s,",UseMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrozenMargin=%s,",FrozenMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LongFrozenMargin=%s,",LongFrozenMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ShortFrozenMargin=%s,",ShortFrozenMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseProfit=%s,",CloseProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TotalPositionCost=%s,",TotalPositionCost.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TotalCloseProfit=%s,",TotalCloseProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenPrice=%s,",OpenPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClosePrice=%s,",ClosePrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradeFee=%s,",TradeFee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionFee=%s,",PositionFee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Leverage=%s,",Leverage.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccountID=%s,",AccountID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PriceCurrency=%s,",PriceCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClearCurrency=%s,",ClearCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettlementGroup=%s,",SettlementGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"IsCrossMargin=%s,",IsCrossMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseOrderID=%s,",CloseOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SLTriggerPrice=%s,",SLTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TPTriggerPrice=%s,",TPTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BeginTime=%s,",BeginTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LastOpenTime=%s,",LastOpenTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"IsAutoAddMargin=%s,",IsAutoAddMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Frequency=%s,",Frequency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MaintMargin=%s,",MaintMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UnRealProfit=%s,",UnRealProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LiquidPrice=%s,",LiquidPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CreateTime=%s,",CreateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyMemberID=%s,",CopyMemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyProfitRate=%s,",CopyProfitRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyProfit=%s,",CopyProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FirstTradeID=%s,",FirstTradeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LastTradeID=%s,",LastTradeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessType=%s,",BusinessType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessValue=%s,",BusinessValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Reserve=%s,",Reserve.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ReserveProfit=%s,",ReserveProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CPositionIterator::dump(FILE *output)
{
	CPosition *pPosition;
	while ((pPosition=next())!=NULL)
	{
		pPosition->dump(output);
	}
}

const CWriteableInstrument *CWriteablePosition::linkInstrument() const
{
	void *target=(void *)(&pInstrument);
	*((const CWriteableInstrument **)target)=m_pMDB->m_InstrumentFactory->findByInstrumentID(ExchangeID,InstrumentID);
	return pInstrument;
}

const CWriteableAccount *CWriteablePosition::linkAccount() const
{
	void *target=(void *)(&pAccount);
	*((const CWriteableAccount **)target)=m_pMDB->m_AccountFactory->findByAccountIDAndCurrency(MemberID,AccountID,SettlementGroup,ClearCurrency);
	return pAccount;
}



void CWriteableMarketData::writeCSVHead(FILE *output)
{
	fprintf(output,"\"ExchangeID*\",\"InstrumentID*\",\"ProductGroup\",\"UpdateTime\",\"PreClosePrice\",\"PreSettlementPrice\",\"PreOpenInterest\",\"PrePositionFeeRate\",\"ClosePrice\",\"PositionFeeTime\",\"SettlementTime\",\"UpperLimitPrice\",\"LowerLimitPrice\",\"UnderlyingPrice\",\"MarkedPrice\",\"PositionFeeRate\",\"HighestPrice\",\"LowestPrice\",\"LastPrice\",\"Volume\",\"Turnover\",\"OpenInterest\",\"SettlementPrice\",\"OpenPrice\",\"IsPubMarketData\",\"InstrumentStatus\",\"Remark\",\"BusinessNo\",\"MarkedPriceTotal\",\"MarkedPriceWeight\",\"L25UpperPrice\",\"L25LowerPrice\",\"L25UpperPrice1\",\"L25LowerPrice1\",\"L25UpperPrice2\",\"L25LowerPrice2\",\"BidPrice1\",\"BidVolume1\",\"AskPrice1\",\"AskVolume1\",\"TheoryPrice\",\"HighestPrice24\",\"LowestPrice24\",\"Volume24\",\"Turnover24\",\"OpenPrice24\",\"Frequency\",\"InsertTime\"\n");
}

void CWriteableMarketData::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"ExchangeID\",\"InstrumentID\",\"ProductGroup\",\"UpdateTime\",\"PreClosePrice\",\"PreSettlementPrice\",\"PreOpenInterest\",\"PrePositionFeeRate\",\"ClosePrice\",\"PositionFeeTime\",\"SettlementTime\",\"UpperLimitPrice\",\"LowerLimitPrice\",\"UnderlyingPrice\",\"MarkedPrice\",\"PositionFeeRate\",\"HighestPrice\",\"LowestPrice\",\"LastPrice\",\"Volume\",\"Turnover\",\"OpenInterest\",\"SettlementPrice\",\"OpenPrice\",\"IsPubMarketData\",\"InstrumentStatus\",\"Remark\",\"BusinessNo\",\"MarkedPriceTotal\",\"MarkedPriceWeight\",\"L25UpperPrice\",\"L25LowerPrice\",\"L25UpperPrice1\",\"L25LowerPrice1\",\"L25UpperPrice2\",\"L25LowerPrice2\",\"BidPrice1\",\"BidVolume1\",\"AskPrice1\",\"AskVolume1\",\"TheoryPrice\",\"HighestPrice24\",\"LowestPrice24\",\"Volume24\",\"Turnover24\",\"OpenPrice24\",\"Frequency\",\"InsertTime\"\n");
}
	
void CWriteableMarketData::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	ExchangeID.init();
	InstrumentID.init();
	ProductGroup.clear();
	UpdateTime.init();
	PreClosePrice.clear();
	PreSettlementPrice.clear();
	PreOpenInterest.init();
	PrePositionFeeRate.clear();
	ClosePrice.clear();
	PositionFeeTime.clear();
	SettlementTime.clear();
	UpperLimitPrice.clear();
	LowerLimitPrice.clear();
	UnderlyingPrice.clear();
	MarkedPrice.clear();
	PositionFeeRate.clear();
	HighestPrice.clear();
	LowestPrice.clear();
	LastPrice.clear();
	Volume.init();
	Turnover.init();
	OpenInterest.init();
	SettlementPrice.clear();
	OpenPrice.clear();
	IsPubMarketData.init();
	InstrumentStatus.init();
	Remark.clear();
	BusinessNo.clear();
	MarkedPriceTotal.clear();
	MarkedPriceWeight.clear();
	L25UpperPrice.clear();
	L25LowerPrice.clear();
	L25UpperPrice1.clear();
	L25LowerPrice1.clear();
	L25UpperPrice2.clear();
	L25LowerPrice2.clear();
	BidPrice1.clear();
	BidVolume1.clear();
	AskPrice1.clear();
	AskVolume1.clear();
	TheoryPrice.clear();
	HighestPrice24.clear();
	LowestPrice24.clear();
	Volume24.clear();
	Turnover24.clear();
	OpenPrice24.clear();
	Frequency.clear();
	InsertTime.clear();
}

void CWriteableMarketData::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableMarketData::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableMarketData),input)!=sizeof(CWriteableMarketData))
		return 0;
	else
		return 1;
}
	
int CWriteableMarketData::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ProductGroup=token;
		else
			ProductGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PreClosePrice=token;
		else
			PreClosePrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PreSettlementPrice=token;
		else
			PreSettlementPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PreOpenInterest=token;
		else
			PreOpenInterest="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PrePositionFeeRate=token;
		else
			PrePositionFeeRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClosePrice=token;
		else
			ClosePrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionFeeTime=token;
		else
			PositionFeeTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettlementTime=token;
		else
			SettlementTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpperLimitPrice=token;
		else
			UpperLimitPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LowerLimitPrice=token;
		else
			LowerLimitPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UnderlyingPrice=token;
		else
			UnderlyingPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MarkedPrice=token;
		else
			MarkedPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionFeeRate=token;
		else
			PositionFeeRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			HighestPrice=token;
		else
			HighestPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LowestPrice=token;
		else
			LowestPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LastPrice=token;
		else
			LastPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Turnover=token;
		else
			Turnover="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenInterest=token;
		else
			OpenInterest="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettlementPrice=token;
		else
			SettlementPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenPrice=token;
		else
			OpenPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			IsPubMarketData=token;
		else
			IsPubMarketData="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentStatus=token;
		else
			InstrumentStatus="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MarkedPriceTotal=token;
		else
			MarkedPriceTotal="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MarkedPriceWeight=token;
		else
			MarkedPriceWeight="";
		token=getNextToken(NULL);
		if (token!=NULL)
			L25UpperPrice=token;
		else
			L25UpperPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			L25LowerPrice=token;
		else
			L25LowerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			L25UpperPrice1=token;
		else
			L25UpperPrice1="";
		token=getNextToken(NULL);
		if (token!=NULL)
			L25LowerPrice1=token;
		else
			L25LowerPrice1="";
		token=getNextToken(NULL);
		if (token!=NULL)
			L25UpperPrice2=token;
		else
			L25UpperPrice2="";
		token=getNextToken(NULL);
		if (token!=NULL)
			L25LowerPrice2=token;
		else
			L25LowerPrice2="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BidPrice1=token;
		else
			BidPrice1="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BidVolume1=token;
		else
			BidVolume1="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AskPrice1=token;
		else
			AskPrice1="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AskVolume1=token;
		else
			AskVolume1="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TheoryPrice=token;
		else
			TheoryPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			HighestPrice24=token;
		else
			HighestPrice24="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LowestPrice24=token;
		else
			LowestPrice24="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume24=token;
		else
			Volume24="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Turnover24=token;
		else
			Turnover24="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenPrice24=token;
		else
			OpenPrice24="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Frequency=token;
		else
			Frequency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"ProductGroup"))
			{
				ProductGroup=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			else if (!strcmp(fieldName,"PreClosePrice"))
			{
				PreClosePrice=token;
			}
			else if (!strcmp(fieldName,"PreSettlementPrice"))
			{
				PreSettlementPrice=token;
			}
			else if (!strcmp(fieldName,"PreOpenInterest"))
			{
				PreOpenInterest=token;
			}
			else if (!strcmp(fieldName,"PrePositionFeeRate"))
			{
				PrePositionFeeRate=token;
			}
			else if (!strcmp(fieldName,"ClosePrice"))
			{
				ClosePrice=token;
			}
			else if (!strcmp(fieldName,"PositionFeeTime"))
			{
				PositionFeeTime=token;
			}
			else if (!strcmp(fieldName,"SettlementTime"))
			{
				SettlementTime=token;
			}
			else if (!strcmp(fieldName,"UpperLimitPrice"))
			{
				UpperLimitPrice=token;
			}
			else if (!strcmp(fieldName,"LowerLimitPrice"))
			{
				LowerLimitPrice=token;
			}
			else if (!strcmp(fieldName,"UnderlyingPrice"))
			{
				UnderlyingPrice=token;
			}
			else if (!strcmp(fieldName,"MarkedPrice"))
			{
				MarkedPrice=token;
			}
			else if (!strcmp(fieldName,"PositionFeeRate"))
			{
				PositionFeeRate=token;
			}
			else if (!strcmp(fieldName,"HighestPrice"))
			{
				HighestPrice=token;
			}
			else if (!strcmp(fieldName,"LowestPrice"))
			{
				LowestPrice=token;
			}
			else if (!strcmp(fieldName,"LastPrice"))
			{
				LastPrice=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"Turnover"))
			{
				Turnover=token;
			}
			else if (!strcmp(fieldName,"OpenInterest"))
			{
				OpenInterest=token;
			}
			else if (!strcmp(fieldName,"SettlementPrice"))
			{
				SettlementPrice=token;
			}
			else if (!strcmp(fieldName,"OpenPrice"))
			{
				OpenPrice=token;
			}
			else if (!strcmp(fieldName,"IsPubMarketData"))
			{
				IsPubMarketData=token;
			}
			else if (!strcmp(fieldName,"InstrumentStatus"))
			{
				InstrumentStatus=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			else if (!strcmp(fieldName,"MarkedPriceTotal"))
			{
				MarkedPriceTotal=token;
			}
			else if (!strcmp(fieldName,"MarkedPriceWeight"))
			{
				MarkedPriceWeight=token;
			}
			else if (!strcmp(fieldName,"L25UpperPrice"))
			{
				L25UpperPrice=token;
			}
			else if (!strcmp(fieldName,"L25LowerPrice"))
			{
				L25LowerPrice=token;
			}
			else if (!strcmp(fieldName,"L25UpperPrice1"))
			{
				L25UpperPrice1=token;
			}
			else if (!strcmp(fieldName,"L25LowerPrice1"))
			{
				L25LowerPrice1=token;
			}
			else if (!strcmp(fieldName,"L25UpperPrice2"))
			{
				L25UpperPrice2=token;
			}
			else if (!strcmp(fieldName,"L25LowerPrice2"))
			{
				L25LowerPrice2=token;
			}
			else if (!strcmp(fieldName,"BidPrice1"))
			{
				BidPrice1=token;
			}
			else if (!strcmp(fieldName,"BidVolume1"))
			{
				BidVolume1=token;
			}
			else if (!strcmp(fieldName,"AskPrice1"))
			{
				AskPrice1=token;
			}
			else if (!strcmp(fieldName,"AskVolume1"))
			{
				AskVolume1=token;
			}
			else if (!strcmp(fieldName,"TheoryPrice"))
			{
				TheoryPrice=token;
			}
			else if (!strcmp(fieldName,"HighestPrice24"))
			{
				HighestPrice24=token;
			}
			else if (!strcmp(fieldName,"LowestPrice24"))
			{
				LowestPrice24=token;
			}
			else if (!strcmp(fieldName,"Volume24"))
			{
				Volume24=token;
			}
			else if (!strcmp(fieldName,"Turnover24"))
			{
				Turnover24=token;
			}
			else if (!strcmp(fieldName,"OpenPrice24"))
			{
				OpenPrice24=token;
			}
			else if (!strcmp(fieldName,"Frequency"))
			{
				Frequency=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableMarketData::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableMarketData),output)!=sizeof(CWriteableMarketData))
		return 0;
	else
		return 1;
}

int CWriteableMarketData::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ProductGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreClosePrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreSettlementPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreOpenInterest.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PrePositionFeeRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionFeeTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpperLimitPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowerLimitPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UnderlyingPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarkedPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionFeeRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowestPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenInterest.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsPubMarketData.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentStatus.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarkedPriceTotal.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarkedPriceWeight.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25UpperPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25LowerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25UpperPrice1.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25LowerPrice1.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25UpperPrice2.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25LowerPrice2.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BidPrice1.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BidVolume1.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AskPrice1.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AskVolume1.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TheoryPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPrice24.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowestPrice24.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume24.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover24.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice24.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Frequency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableMarketData::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ProductGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreClosePrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreSettlementPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreOpenInterest.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PrePositionFeeRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionFeeTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpperLimitPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowerLimitPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UnderlyingPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarkedPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionFeeRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowestPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenInterest.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsPubMarketData.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentStatus.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarkedPriceTotal.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MarkedPriceWeight.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25UpperPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25LowerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25UpperPrice1.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25LowerPrice1.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25UpperPrice2.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",L25LowerPrice2.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BidPrice1.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BidVolume1.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AskPrice1.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AskVolume1.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TheoryPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPrice24.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowestPrice24.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume24.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover24.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice24.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Frequency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableMarketData::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCMarketData%s = {\n", index_buf);
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tProductGroup = %s    [产品组]\n",ProductGroup.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t\tPreClosePrice = %s    [上次收盘]\n",PreClosePrice.getString().c_str());	
	fprintf(fp,"\t\tPreSettlementPrice = %s    [上次结算价]\n",PreSettlementPrice.getString().c_str());	
	fprintf(fp,"\t\tPreOpenInterest = %s    [上次持仓量]\n",PreOpenInterest.getString().c_str());	
	fprintf(fp,"\t\tPrePositionFeeRate = %s    [上次持仓费按比例]\n",PrePositionFeeRate.getString().c_str());	
	fprintf(fp,"\t\tClosePrice = %s    [收盘价]\n",ClosePrice.getString().c_str());	
	fprintf(fp,"\t\tPositionFeeTime = %s    [收取持仓费用时间]\n",PositionFeeTime.getString().c_str());	
	fprintf(fp,"\t\tSettlementTime = %s    [结算时间]\n",SettlementTime.getString().c_str());	
	fprintf(fp,"\t\tUpperLimitPrice = %s    [最高限价]\n",UpperLimitPrice.getString().c_str());	
	fprintf(fp,"\t\tLowerLimitPrice = %s    [最低限价]\n",LowerLimitPrice.getString().c_str());	
	fprintf(fp,"\t\tUnderlyingPrice = %s    [基础标的价格]\n",UnderlyingPrice.getString().c_str());	
	fprintf(fp,"\t\tMarkedPrice = %s    [标记价格]\n",MarkedPrice.getString().c_str());	
	fprintf(fp,"\t\tPositionFeeRate = %s    [预计持仓费按比例]\n",PositionFeeRate.getString().c_str());	
	fprintf(fp,"\t\tHighestPrice = %s    [当日最高价]\n",HighestPrice.getString().c_str());	
	fprintf(fp,"\t\tLowestPrice = %s    [当日最低价]\n",LowestPrice.getString().c_str());	
	fprintf(fp,"\t\tLastPrice = %s    [最新价]\n",LastPrice.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [当日数量]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tTurnover = %s    [当日成交金额]\n",Turnover.getString().c_str());	
	fprintf(fp,"\t\tOpenInterest = %s    [持仓量]\n",OpenInterest.getString().c_str());	
	fprintf(fp,"\t\tSettlementPrice = %s    [预估结算价]\n",SettlementPrice.getString().c_str());	
	fprintf(fp,"\t\tOpenPrice = %s    [当日开盘价]\n",OpenPrice.getString().c_str());	
	fprintf(fp,"\t\tIsPubMarketData = %s    [行情发布状态]\n",IsPubMarketData.getString().c_str());	
	fprintf(fp,"\t\tInstrumentStatus = %s    [标的交易状态]\n",InstrumentStatus.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t\tMarkedPriceTotal = %s    [标记价格汇总]\n",MarkedPriceTotal.getString().c_str());	
	fprintf(fp,"\t\tMarkedPriceWeight = %s    [标记价格权重]\n",MarkedPriceWeight.getString().c_str());	
	fprintf(fp,"\t\tL25UpperPrice = %s    [25档行情最高价]\n",L25UpperPrice.getString().c_str());	
	fprintf(fp,"\t\tL25LowerPrice = %s    [25档行情最低价]\n",L25LowerPrice.getString().c_str());	
	fprintf(fp,"\t\tL25UpperPrice1 = %s    [25档行情最高价1]\n",L25UpperPrice1.getString().c_str());	
	fprintf(fp,"\t\tL25LowerPrice1 = %s    [25档行情最低价1]\n",L25LowerPrice1.getString().c_str());	
	fprintf(fp,"\t\tL25UpperPrice2 = %s    [25档行情最高价2]\n",L25UpperPrice2.getString().c_str());	
	fprintf(fp,"\t\tL25LowerPrice2 = %s    [25档行情最低价2]\n",L25LowerPrice2.getString().c_str());	
	fprintf(fp,"\t\tBidPrice1 = %s    [申买价一]\n",BidPrice1.getString().c_str());	
	fprintf(fp,"\t\tBidVolume1 = %s    [申买量一]\n",BidVolume1.getString().c_str());	
	fprintf(fp,"\t\tAskPrice1 = %s    [申卖价一]\n",AskPrice1.getString().c_str());	
	fprintf(fp,"\t\tAskVolume1 = %s    [申卖量一]\n",AskVolume1.getString().c_str());	
	fprintf(fp,"\t\tTheoryPrice = %s    [理论价]\n",TheoryPrice.getString().c_str());	
	fprintf(fp,"\t\tHighestPrice24 = %s    [24小时最高价]\n",HighestPrice24.getString().c_str());	
	fprintf(fp,"\t\tLowestPrice24 = %s    [24小时最低价]\n",LowestPrice24.getString().c_str());	
	fprintf(fp,"\t\tVolume24 = %s    [24小时数量]\n",Volume24.getString().c_str());	
	fprintf(fp,"\t\tTurnover24 = %s    [24小时成交金额]\n",Turnover24.getString().c_str());	
	fprintf(fp,"\t\tOpenPrice24 = %s    [24小时开盘价]\n",OpenPrice24.getString().c_str());	
	fprintf(fp,"\t\tFrequency = %s    [每秒更新的次数]\n",Frequency.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableMarketData::dumpDiff(FILE *fp, const CWriteableMarketData *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCMarketData = {\n");
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(ProductGroup.getValue(),pOriginal->ProductGroup.getValue())!=0)
		fprintf(fp,"\t\tProductGroup = %s -> %s    [产品组]\n",pOriginal->ProductGroup.getString().c_str(),ProductGroup.getString().c_str());
	else
		fprintf(fp,"\t\tProductGroup = %s    [产品组]\n",ProductGroup.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	if (compare(PreClosePrice.getValue(),pOriginal->PreClosePrice.getValue())!=0)
		fprintf(fp,"\t\tPreClosePrice = %s -> %s    [上次收盘]\n",pOriginal->PreClosePrice.getString().c_str(),PreClosePrice.getString().c_str());
	else
		fprintf(fp,"\t\tPreClosePrice = %s    [上次收盘]\n",PreClosePrice.getString().c_str());
	if (compare(PreSettlementPrice.getValue(),pOriginal->PreSettlementPrice.getValue())!=0)
		fprintf(fp,"\t\tPreSettlementPrice = %s -> %s    [上次结算价]\n",pOriginal->PreSettlementPrice.getString().c_str(),PreSettlementPrice.getString().c_str());
	else
		fprintf(fp,"\t\tPreSettlementPrice = %s    [上次结算价]\n",PreSettlementPrice.getString().c_str());
	if (compare(PreOpenInterest.getValue(),pOriginal->PreOpenInterest.getValue())!=0)
		fprintf(fp,"\t\tPreOpenInterest = %s -> %s    [上次持仓量]\n",pOriginal->PreOpenInterest.getString().c_str(),PreOpenInterest.getString().c_str());
	else
		fprintf(fp,"\t\tPreOpenInterest = %s    [上次持仓量]\n",PreOpenInterest.getString().c_str());
	if (compare(PrePositionFeeRate.getValue(),pOriginal->PrePositionFeeRate.getValue())!=0)
		fprintf(fp,"\t\tPrePositionFeeRate = %s -> %s    [上次持仓费按比例]\n",pOriginal->PrePositionFeeRate.getString().c_str(),PrePositionFeeRate.getString().c_str());
	else
		fprintf(fp,"\t\tPrePositionFeeRate = %s    [上次持仓费按比例]\n",PrePositionFeeRate.getString().c_str());
	if (compare(ClosePrice.getValue(),pOriginal->ClosePrice.getValue())!=0)
		fprintf(fp,"\t\tClosePrice = %s -> %s    [收盘价]\n",pOriginal->ClosePrice.getString().c_str(),ClosePrice.getString().c_str());
	else
		fprintf(fp,"\t\tClosePrice = %s    [收盘价]\n",ClosePrice.getString().c_str());
	if (compare(PositionFeeTime.getValue(),pOriginal->PositionFeeTime.getValue())!=0)
		fprintf(fp,"\t\tPositionFeeTime = %s -> %s    [收取持仓费用时间]\n",pOriginal->PositionFeeTime.getString().c_str(),PositionFeeTime.getString().c_str());
	else
		fprintf(fp,"\t\tPositionFeeTime = %s    [收取持仓费用时间]\n",PositionFeeTime.getString().c_str());
	if (compare(SettlementTime.getValue(),pOriginal->SettlementTime.getValue())!=0)
		fprintf(fp,"\t\tSettlementTime = %s -> %s    [结算时间]\n",pOriginal->SettlementTime.getString().c_str(),SettlementTime.getString().c_str());
	else
		fprintf(fp,"\t\tSettlementTime = %s    [结算时间]\n",SettlementTime.getString().c_str());
	if (compare(UpperLimitPrice.getValue(),pOriginal->UpperLimitPrice.getValue())!=0)
		fprintf(fp,"\t\tUpperLimitPrice = %s -> %s    [最高限价]\n",pOriginal->UpperLimitPrice.getString().c_str(),UpperLimitPrice.getString().c_str());
	else
		fprintf(fp,"\t\tUpperLimitPrice = %s    [最高限价]\n",UpperLimitPrice.getString().c_str());
	if (compare(LowerLimitPrice.getValue(),pOriginal->LowerLimitPrice.getValue())!=0)
		fprintf(fp,"\t\tLowerLimitPrice = %s -> %s    [最低限价]\n",pOriginal->LowerLimitPrice.getString().c_str(),LowerLimitPrice.getString().c_str());
	else
		fprintf(fp,"\t\tLowerLimitPrice = %s    [最低限价]\n",LowerLimitPrice.getString().c_str());
	if (compare(UnderlyingPrice.getValue(),pOriginal->UnderlyingPrice.getValue())!=0)
		fprintf(fp,"\t\tUnderlyingPrice = %s -> %s    [基础标的价格]\n",pOriginal->UnderlyingPrice.getString().c_str(),UnderlyingPrice.getString().c_str());
	else
		fprintf(fp,"\t\tUnderlyingPrice = %s    [基础标的价格]\n",UnderlyingPrice.getString().c_str());
	if (compare(MarkedPrice.getValue(),pOriginal->MarkedPrice.getValue())!=0)
		fprintf(fp,"\t\tMarkedPrice = %s -> %s    [标记价格]\n",pOriginal->MarkedPrice.getString().c_str(),MarkedPrice.getString().c_str());
	else
		fprintf(fp,"\t\tMarkedPrice = %s    [标记价格]\n",MarkedPrice.getString().c_str());
	if (compare(PositionFeeRate.getValue(),pOriginal->PositionFeeRate.getValue())!=0)
		fprintf(fp,"\t\tPositionFeeRate = %s -> %s    [预计持仓费按比例]\n",pOriginal->PositionFeeRate.getString().c_str(),PositionFeeRate.getString().c_str());
	else
		fprintf(fp,"\t\tPositionFeeRate = %s    [预计持仓费按比例]\n",PositionFeeRate.getString().c_str());
	if (compare(HighestPrice.getValue(),pOriginal->HighestPrice.getValue())!=0)
		fprintf(fp,"\t\tHighestPrice = %s -> %s    [当日最高价]\n",pOriginal->HighestPrice.getString().c_str(),HighestPrice.getString().c_str());
	else
		fprintf(fp,"\t\tHighestPrice = %s    [当日最高价]\n",HighestPrice.getString().c_str());
	if (compare(LowestPrice.getValue(),pOriginal->LowestPrice.getValue())!=0)
		fprintf(fp,"\t\tLowestPrice = %s -> %s    [当日最低价]\n",pOriginal->LowestPrice.getString().c_str(),LowestPrice.getString().c_str());
	else
		fprintf(fp,"\t\tLowestPrice = %s    [当日最低价]\n",LowestPrice.getString().c_str());
	if (compare(LastPrice.getValue(),pOriginal->LastPrice.getValue())!=0)
		fprintf(fp,"\t\tLastPrice = %s -> %s    [最新价]\n",pOriginal->LastPrice.getString().c_str(),LastPrice.getString().c_str());
	else
		fprintf(fp,"\t\tLastPrice = %s    [最新价]\n",LastPrice.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [当日数量]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [当日数量]\n",Volume.getString().c_str());
	if (compare(Turnover.getValue(),pOriginal->Turnover.getValue())!=0)
		fprintf(fp,"\t\tTurnover = %s -> %s    [当日成交金额]\n",pOriginal->Turnover.getString().c_str(),Turnover.getString().c_str());
	else
		fprintf(fp,"\t\tTurnover = %s    [当日成交金额]\n",Turnover.getString().c_str());
	if (compare(OpenInterest.getValue(),pOriginal->OpenInterest.getValue())!=0)
		fprintf(fp,"\t\tOpenInterest = %s -> %s    [持仓量]\n",pOriginal->OpenInterest.getString().c_str(),OpenInterest.getString().c_str());
	else
		fprintf(fp,"\t\tOpenInterest = %s    [持仓量]\n",OpenInterest.getString().c_str());
	if (compare(SettlementPrice.getValue(),pOriginal->SettlementPrice.getValue())!=0)
		fprintf(fp,"\t\tSettlementPrice = %s -> %s    [预估结算价]\n",pOriginal->SettlementPrice.getString().c_str(),SettlementPrice.getString().c_str());
	else
		fprintf(fp,"\t\tSettlementPrice = %s    [预估结算价]\n",SettlementPrice.getString().c_str());
	if (compare(OpenPrice.getValue(),pOriginal->OpenPrice.getValue())!=0)
		fprintf(fp,"\t\tOpenPrice = %s -> %s    [当日开盘价]\n",pOriginal->OpenPrice.getString().c_str(),OpenPrice.getString().c_str());
	else
		fprintf(fp,"\t\tOpenPrice = %s    [当日开盘价]\n",OpenPrice.getString().c_str());
	if (compare(IsPubMarketData.getValue(),pOriginal->IsPubMarketData.getValue())!=0)
		fprintf(fp,"\t\tIsPubMarketData = %s -> %s    [行情发布状态]\n",pOriginal->IsPubMarketData.getString().c_str(),IsPubMarketData.getString().c_str());
	else
		fprintf(fp,"\t\tIsPubMarketData = %s    [行情发布状态]\n",IsPubMarketData.getString().c_str());
	if (compare(InstrumentStatus.getValue(),pOriginal->InstrumentStatus.getValue())!=0)
		fprintf(fp,"\t\tInstrumentStatus = %s -> %s    [标的交易状态]\n",pOriginal->InstrumentStatus.getString().c_str(),InstrumentStatus.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentStatus = %s    [标的交易状态]\n",InstrumentStatus.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [业务序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());
	if (compare(MarkedPriceTotal.getValue(),pOriginal->MarkedPriceTotal.getValue())!=0)
		fprintf(fp,"\t\tMarkedPriceTotal = %s -> %s    [标记价格汇总]\n",pOriginal->MarkedPriceTotal.getString().c_str(),MarkedPriceTotal.getString().c_str());
	else
		fprintf(fp,"\t\tMarkedPriceTotal = %s    [标记价格汇总]\n",MarkedPriceTotal.getString().c_str());
	if (compare(MarkedPriceWeight.getValue(),pOriginal->MarkedPriceWeight.getValue())!=0)
		fprintf(fp,"\t\tMarkedPriceWeight = %s -> %s    [标记价格权重]\n",pOriginal->MarkedPriceWeight.getString().c_str(),MarkedPriceWeight.getString().c_str());
	else
		fprintf(fp,"\t\tMarkedPriceWeight = %s    [标记价格权重]\n",MarkedPriceWeight.getString().c_str());
	if (compare(L25UpperPrice.getValue(),pOriginal->L25UpperPrice.getValue())!=0)
		fprintf(fp,"\t\tL25UpperPrice = %s -> %s    [25档行情最高价]\n",pOriginal->L25UpperPrice.getString().c_str(),L25UpperPrice.getString().c_str());
	else
		fprintf(fp,"\t\tL25UpperPrice = %s    [25档行情最高价]\n",L25UpperPrice.getString().c_str());
	if (compare(L25LowerPrice.getValue(),pOriginal->L25LowerPrice.getValue())!=0)
		fprintf(fp,"\t\tL25LowerPrice = %s -> %s    [25档行情最低价]\n",pOriginal->L25LowerPrice.getString().c_str(),L25LowerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tL25LowerPrice = %s    [25档行情最低价]\n",L25LowerPrice.getString().c_str());
	if (compare(L25UpperPrice1.getValue(),pOriginal->L25UpperPrice1.getValue())!=0)
		fprintf(fp,"\t\tL25UpperPrice1 = %s -> %s    [25档行情最高价1]\n",pOriginal->L25UpperPrice1.getString().c_str(),L25UpperPrice1.getString().c_str());
	else
		fprintf(fp,"\t\tL25UpperPrice1 = %s    [25档行情最高价1]\n",L25UpperPrice1.getString().c_str());
	if (compare(L25LowerPrice1.getValue(),pOriginal->L25LowerPrice1.getValue())!=0)
		fprintf(fp,"\t\tL25LowerPrice1 = %s -> %s    [25档行情最低价1]\n",pOriginal->L25LowerPrice1.getString().c_str(),L25LowerPrice1.getString().c_str());
	else
		fprintf(fp,"\t\tL25LowerPrice1 = %s    [25档行情最低价1]\n",L25LowerPrice1.getString().c_str());
	if (compare(L25UpperPrice2.getValue(),pOriginal->L25UpperPrice2.getValue())!=0)
		fprintf(fp,"\t\tL25UpperPrice2 = %s -> %s    [25档行情最高价2]\n",pOriginal->L25UpperPrice2.getString().c_str(),L25UpperPrice2.getString().c_str());
	else
		fprintf(fp,"\t\tL25UpperPrice2 = %s    [25档行情最高价2]\n",L25UpperPrice2.getString().c_str());
	if (compare(L25LowerPrice2.getValue(),pOriginal->L25LowerPrice2.getValue())!=0)
		fprintf(fp,"\t\tL25LowerPrice2 = %s -> %s    [25档行情最低价2]\n",pOriginal->L25LowerPrice2.getString().c_str(),L25LowerPrice2.getString().c_str());
	else
		fprintf(fp,"\t\tL25LowerPrice2 = %s    [25档行情最低价2]\n",L25LowerPrice2.getString().c_str());
	if (compare(BidPrice1.getValue(),pOriginal->BidPrice1.getValue())!=0)
		fprintf(fp,"\t\tBidPrice1 = %s -> %s    [申买价一]\n",pOriginal->BidPrice1.getString().c_str(),BidPrice1.getString().c_str());
	else
		fprintf(fp,"\t\tBidPrice1 = %s    [申买价一]\n",BidPrice1.getString().c_str());
	if (compare(BidVolume1.getValue(),pOriginal->BidVolume1.getValue())!=0)
		fprintf(fp,"\t\tBidVolume1 = %s -> %s    [申买量一]\n",pOriginal->BidVolume1.getString().c_str(),BidVolume1.getString().c_str());
	else
		fprintf(fp,"\t\tBidVolume1 = %s    [申买量一]\n",BidVolume1.getString().c_str());
	if (compare(AskPrice1.getValue(),pOriginal->AskPrice1.getValue())!=0)
		fprintf(fp,"\t\tAskPrice1 = %s -> %s    [申卖价一]\n",pOriginal->AskPrice1.getString().c_str(),AskPrice1.getString().c_str());
	else
		fprintf(fp,"\t\tAskPrice1 = %s    [申卖价一]\n",AskPrice1.getString().c_str());
	if (compare(AskVolume1.getValue(),pOriginal->AskVolume1.getValue())!=0)
		fprintf(fp,"\t\tAskVolume1 = %s -> %s    [申卖量一]\n",pOriginal->AskVolume1.getString().c_str(),AskVolume1.getString().c_str());
	else
		fprintf(fp,"\t\tAskVolume1 = %s    [申卖量一]\n",AskVolume1.getString().c_str());
	if (compare(TheoryPrice.getValue(),pOriginal->TheoryPrice.getValue())!=0)
		fprintf(fp,"\t\tTheoryPrice = %s -> %s    [理论价]\n",pOriginal->TheoryPrice.getString().c_str(),TheoryPrice.getString().c_str());
	else
		fprintf(fp,"\t\tTheoryPrice = %s    [理论价]\n",TheoryPrice.getString().c_str());
	if (compare(HighestPrice24.getValue(),pOriginal->HighestPrice24.getValue())!=0)
		fprintf(fp,"\t\tHighestPrice24 = %s -> %s    [24小时最高价]\n",pOriginal->HighestPrice24.getString().c_str(),HighestPrice24.getString().c_str());
	else
		fprintf(fp,"\t\tHighestPrice24 = %s    [24小时最高价]\n",HighestPrice24.getString().c_str());
	if (compare(LowestPrice24.getValue(),pOriginal->LowestPrice24.getValue())!=0)
		fprintf(fp,"\t\tLowestPrice24 = %s -> %s    [24小时最低价]\n",pOriginal->LowestPrice24.getString().c_str(),LowestPrice24.getString().c_str());
	else
		fprintf(fp,"\t\tLowestPrice24 = %s    [24小时最低价]\n",LowestPrice24.getString().c_str());
	if (compare(Volume24.getValue(),pOriginal->Volume24.getValue())!=0)
		fprintf(fp,"\t\tVolume24 = %s -> %s    [24小时数量]\n",pOriginal->Volume24.getString().c_str(),Volume24.getString().c_str());
	else
		fprintf(fp,"\t\tVolume24 = %s    [24小时数量]\n",Volume24.getString().c_str());
	if (compare(Turnover24.getValue(),pOriginal->Turnover24.getValue())!=0)
		fprintf(fp,"\t\tTurnover24 = %s -> %s    [24小时成交金额]\n",pOriginal->Turnover24.getString().c_str(),Turnover24.getString().c_str());
	else
		fprintf(fp,"\t\tTurnover24 = %s    [24小时成交金额]\n",Turnover24.getString().c_str());
	if (compare(OpenPrice24.getValue(),pOriginal->OpenPrice24.getValue())!=0)
		fprintf(fp,"\t\tOpenPrice24 = %s -> %s    [24小时开盘价]\n",pOriginal->OpenPrice24.getString().c_str(),OpenPrice24.getString().c_str());
	else
		fprintf(fp,"\t\tOpenPrice24 = %s    [24小时开盘价]\n",OpenPrice24.getString().c_str());
	if (compare(Frequency.getValue(),pOriginal->Frequency.getValue())!=0)
		fprintf(fp,"\t\tFrequency = %s -> %s    [每秒更新的次数]\n",pOriginal->Frequency.getString().c_str(),Frequency.getString().c_str());
	else
		fprintf(fp,"\t\tFrequency = %s    [每秒更新的次数]\n",Frequency.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableMarketData::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",ProductGroup=%s",ProductGroup.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,",PreClosePrice=%s",PreClosePrice.getString().c_str());	
	fprintf(fp,",PreSettlementPrice=%s",PreSettlementPrice.getString().c_str());	
	fprintf(fp,",PreOpenInterest=%s",PreOpenInterest.getString().c_str());	
	fprintf(fp,",PrePositionFeeRate=%s",PrePositionFeeRate.getString().c_str());	
	fprintf(fp,",ClosePrice=%s",ClosePrice.getString().c_str());	
	fprintf(fp,",PositionFeeTime=%s",PositionFeeTime.getString().c_str());	
	fprintf(fp,",SettlementTime=%s",SettlementTime.getString().c_str());	
	fprintf(fp,",UpperLimitPrice=%s",UpperLimitPrice.getString().c_str());	
	fprintf(fp,",LowerLimitPrice=%s",LowerLimitPrice.getString().c_str());	
	fprintf(fp,",UnderlyingPrice=%s",UnderlyingPrice.getString().c_str());	
	fprintf(fp,",MarkedPrice=%s",MarkedPrice.getString().c_str());	
	fprintf(fp,",PositionFeeRate=%s",PositionFeeRate.getString().c_str());	
	fprintf(fp,",HighestPrice=%s",HighestPrice.getString().c_str());	
	fprintf(fp,",LowestPrice=%s",LowestPrice.getString().c_str());	
	fprintf(fp,",LastPrice=%s",LastPrice.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",Turnover=%s",Turnover.getString().c_str());	
	fprintf(fp,",OpenInterest=%s",OpenInterest.getString().c_str());	
	fprintf(fp,",SettlementPrice=%s",SettlementPrice.getString().c_str());	
	fprintf(fp,",OpenPrice=%s",OpenPrice.getString().c_str());	
	fprintf(fp,",IsPubMarketData=%s",IsPubMarketData.getString().c_str());	
	fprintf(fp,",InstrumentStatus=%s",InstrumentStatus.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,",MarkedPriceTotal=%s",MarkedPriceTotal.getString().c_str());	
	fprintf(fp,",MarkedPriceWeight=%s",MarkedPriceWeight.getString().c_str());	
	fprintf(fp,",L25UpperPrice=%s",L25UpperPrice.getString().c_str());	
	fprintf(fp,",L25LowerPrice=%s",L25LowerPrice.getString().c_str());	
	fprintf(fp,",L25UpperPrice1=%s",L25UpperPrice1.getString().c_str());	
	fprintf(fp,",L25LowerPrice1=%s",L25LowerPrice1.getString().c_str());	
	fprintf(fp,",L25UpperPrice2=%s",L25UpperPrice2.getString().c_str());	
	fprintf(fp,",L25LowerPrice2=%s",L25LowerPrice2.getString().c_str());	
	fprintf(fp,",BidPrice1=%s",BidPrice1.getString().c_str());	
	fprintf(fp,",BidVolume1=%s",BidVolume1.getString().c_str());	
	fprintf(fp,",AskPrice1=%s",AskPrice1.getString().c_str());	
	fprintf(fp,",AskVolume1=%s",AskVolume1.getString().c_str());	
	fprintf(fp,",TheoryPrice=%s",TheoryPrice.getString().c_str());	
	fprintf(fp,",HighestPrice24=%s",HighestPrice24.getString().c_str());	
	fprintf(fp,",LowestPrice24=%s",LowestPrice24.getString().c_str());	
	fprintf(fp,",Volume24=%s",Volume24.getString().c_str());	
	fprintf(fp,",Turnover24=%s",Turnover24.getString().c_str());	
	fprintf(fp,",OpenPrice24=%s",OpenPrice24.getString().c_str());	
	fprintf(fp,",Frequency=%s",Frequency.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableMarketData::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ProductGroup=%s,",ProductGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PreClosePrice=%s,",PreClosePrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PreSettlementPrice=%s,",PreSettlementPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PreOpenInterest=%s,",PreOpenInterest.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PrePositionFeeRate=%s,",PrePositionFeeRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClosePrice=%s,",ClosePrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionFeeTime=%s,",PositionFeeTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettlementTime=%s,",SettlementTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpperLimitPrice=%s,",UpperLimitPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LowerLimitPrice=%s,",LowerLimitPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UnderlyingPrice=%s,",UnderlyingPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MarkedPrice=%s,",MarkedPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionFeeRate=%s,",PositionFeeRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"HighestPrice=%s,",HighestPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LowestPrice=%s,",LowestPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LastPrice=%s,",LastPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Turnover=%s,",Turnover.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenInterest=%s,",OpenInterest.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettlementPrice=%s,",SettlementPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenPrice=%s,",OpenPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"IsPubMarketData=%s,",IsPubMarketData.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentStatus=%s,",InstrumentStatus.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MarkedPriceTotal=%s,",MarkedPriceTotal.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MarkedPriceWeight=%s,",MarkedPriceWeight.getString().c_str());
	p+=strlen(p);
	sprintf(p,"L25UpperPrice=%s,",L25UpperPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"L25LowerPrice=%s,",L25LowerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"L25UpperPrice1=%s,",L25UpperPrice1.getString().c_str());
	p+=strlen(p);
	sprintf(p,"L25LowerPrice1=%s,",L25LowerPrice1.getString().c_str());
	p+=strlen(p);
	sprintf(p,"L25UpperPrice2=%s,",L25UpperPrice2.getString().c_str());
	p+=strlen(p);
	sprintf(p,"L25LowerPrice2=%s,",L25LowerPrice2.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BidPrice1=%s,",BidPrice1.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BidVolume1=%s,",BidVolume1.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AskPrice1=%s,",AskPrice1.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AskVolume1=%s,",AskVolume1.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TheoryPrice=%s,",TheoryPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"HighestPrice24=%s,",HighestPrice24.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LowestPrice24=%s,",LowestPrice24.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume24=%s,",Volume24.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Turnover24=%s,",Turnover24.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenPrice24=%s,",OpenPrice24.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Frequency=%s,",Frequency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CMarketDataIterator::dump(FILE *output)
{
	CMarketData *pMarketData;
	while ((pMarketData=next())!=NULL)
	{
		pMarketData->dump(output);
	}
}

const CWriteableInstrument *CWriteableMarketData::linkInstrument() const
{
	void *target=(void *)(&pInstrument);
	*((const CWriteableInstrument **)target)=m_pMDB->m_InstrumentFactory->findByInstrumentID(ExchangeID,InstrumentID);
	return pInstrument;
}



void CWriteableOrder::writeCSVHead(FILE *output)
{
	fprintf(output,"\"MemberID\",\"TradeUnitID\",\"AccountID\",\"LocalID\",\"ExchangeID\",\"InstrumentID\",\"OrderPriceType\",\"Direction\",\"OffsetFlag\",\"Price\",\"Volume\",\"VolumeDisplay\",\"VolumeMode\",\"Cost\",\"OrderType\",\"GTDTime\",\"MinVolume\",\"BusinessType\",\"BusinessValue\",\"CloseOrderID\",\"IsCrossMargin\",\"Remark\",\"OrderID*\",\"CopyMemberID\",\"CopyOrderID\",\"Leverage\",\"CopyProfitRate\",\"APPID\",\"PositionID\",\"TriggerPrice\",\"Reserve\",\"OrderStatus\",\"DeriveSource\",\"DeriveDetail\",\"VolumeTraded\",\"VolumeRemain\",\"VolumeCancled\",\"InsertTime\",\"UpdateTime\",\"Priority\",\"TimeSortNo\",\"FrontNo\",\"PriceCurrency\",\"FeeCurrency\",\"ClearCurrency\",\"FrozenMoney\",\"FrozenFee\",\"FrozenMargin\",\"Fee\",\"CloseProfit\",\"Turnover\",\"RelatedOrderID\",\"BusinessResult\",\"BusinessNo\",\"Tradable\",\"SettlementGroup\",\"PosiDirection\",\"TradePrice\",\"OpenPrice\",\"TriggerOrderID\",\"SLTriggerPrice\",\"TPTriggerPrice\",\"CopyProfit\",\"Position\",\"UserID\",\"LastPriceByInsert\",\"BidPrice1ByInsert\",\"AskPrice1ByInsert\",\"Available\",\"CreateTime\"\n");
}

void CWriteableOrder::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"MemberID\",\"TradeUnitID\",\"AccountID\",\"LocalID\",\"ExchangeID\",\"InstrumentID\",\"OrderPriceType\",\"Direction\",\"OffsetFlag\",\"Price\",\"Volume\",\"VolumeDisplay\",\"VolumeMode\",\"Cost\",\"OrderType\",\"GTDTime\",\"MinVolume\",\"BusinessType\",\"BusinessValue\",\"CloseOrderID\",\"IsCrossMargin\",\"Remark\",\"OrderID\",\"CopyMemberID\",\"CopyOrderID\",\"Leverage\",\"CopyProfitRate\",\"APPID\",\"PositionID\",\"TriggerPrice\",\"Reserve\",\"OrderStatus\",\"DeriveSource\",\"DeriveDetail\",\"VolumeTraded\",\"VolumeRemain\",\"VolumeCancled\",\"InsertTime\",\"UpdateTime\",\"Priority\",\"TimeSortNo\",\"FrontNo\",\"PriceCurrency\",\"FeeCurrency\",\"ClearCurrency\",\"FrozenMoney\",\"FrozenFee\",\"FrozenMargin\",\"Fee\",\"CloseProfit\",\"Turnover\",\"RelatedOrderID\",\"BusinessResult\",\"BusinessNo\",\"Tradable\",\"SettlementGroup\",\"PosiDirection\",\"TradePrice\",\"OpenPrice\",\"TriggerOrderID\",\"SLTriggerPrice\",\"TPTriggerPrice\",\"CopyProfit\",\"Position\",\"UserID\",\"LastPriceByInsert\",\"BidPrice1ByInsert\",\"AskPrice1ByInsert\",\"Available\",\"CreateTime\"\n");
}
	
void CWriteableOrder::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	MemberID.init();
	TradeUnitID.clear();
	AccountID.clear();
	LocalID.clear();
	ExchangeID.init();
	InstrumentID.init();
	OrderPriceType.init();
	Direction.init();
	OffsetFlag.init();
	Price.clear();
	Volume.clear();
	VolumeDisplay.clear();
	VolumeMode.clear();
	Cost.clear();
	OrderType.init();
	GTDTime.clear();
	MinVolume.clear();
	BusinessType.init();
	BusinessValue.clear();
	CloseOrderID.clear();
	IsCrossMargin.clear();
	Remark.clear();
	OrderID.init();
	CopyMemberID.clear();
	CopyOrderID.clear();
	Leverage.clear();
	CopyProfitRate.clear();
	APPID.clear();
	PositionID.init();
	TriggerPrice.clear();
	Reserve.clear();
	OrderStatus.init();
	DeriveSource.init();
	DeriveDetail.clear();
	VolumeTraded.init();
	VolumeRemain.init();
	VolumeCancled.init();
	InsertTime.clear();
	UpdateTime.clear();
	Priority.init();
	TimeSortNo.init();
	FrontNo.clear();
	PriceCurrency.clear();
	FeeCurrency.clear();
	ClearCurrency.clear();
	FrozenMoney.init();
	FrozenFee.init();
	FrozenMargin.init();
	Fee.init();
	CloseProfit.init();
	Turnover.init();
	RelatedOrderID.clear();
	BusinessResult.clear();
	BusinessNo.clear();
	Tradable.setValue(1);
	SettlementGroup.init();
	PosiDirection.init();
	TradePrice.clear();
	OpenPrice.clear();
	TriggerOrderID.clear();
	SLTriggerPrice.clear();
	TPTriggerPrice.clear();
	CopyProfit.init();
	Position.clear();
	UserID.init();
	LastPriceByInsert.clear();
	BidPrice1ByInsert.clear();
	AskPrice1ByInsert.clear();
	Available.clear();
	CreateTime.clear();
}

void CWriteableOrder::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableOrder::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableOrder),input)!=sizeof(CWriteableOrder))
		return 0;
	else
		return 1;
}
	
int CWriteableOrder::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradeUnitID=token;
		else
			TradeUnitID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccountID=token;
		else
			AccountID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LocalID=token;
		else
			LocalID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderPriceType=token;
		else
			OrderPriceType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Direction=token;
		else
			Direction="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OffsetFlag=token;
		else
			OffsetFlag="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Price=token;
		else
			Price="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeDisplay=token;
		else
			VolumeDisplay="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeMode=token;
		else
			VolumeMode="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Cost=token;
		else
			Cost="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderType=token;
		else
			OrderType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			GTDTime=token;
		else
			GTDTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MinVolume=token;
		else
			MinVolume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessType=token;
		else
			BusinessType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessValue=token;
		else
			BusinessValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseOrderID=token;
		else
			CloseOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			IsCrossMargin=token;
		else
			IsCrossMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderID=token;
		else
			OrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyMemberID=token;
		else
			CopyMemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyOrderID=token;
		else
			CopyOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Leverage=token;
		else
			Leverage="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyProfitRate=token;
		else
			CopyProfitRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			APPID=token;
		else
			APPID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionID=token;
		else
			PositionID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerPrice=token;
		else
			TriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Reserve=token;
		else
			Reserve="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderStatus=token;
		else
			OrderStatus="";
		token=getNextToken(NULL);
		if (token!=NULL)
			DeriveSource=token;
		else
			DeriveSource="";
		token=getNextToken(NULL);
		if (token!=NULL)
			DeriveDetail=token;
		else
			DeriveDetail="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeTraded=token;
		else
			VolumeTraded="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeRemain=token;
		else
			VolumeRemain="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeCancled=token;
		else
			VolumeCancled="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Priority=token;
		else
			Priority="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TimeSortNo=token;
		else
			TimeSortNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrontNo=token;
		else
			FrontNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PriceCurrency=token;
		else
			PriceCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FeeCurrency=token;
		else
			FeeCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClearCurrency=token;
		else
			ClearCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrozenMoney=token;
		else
			FrozenMoney="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrozenFee=token;
		else
			FrozenFee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrozenMargin=token;
		else
			FrozenMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Fee=token;
		else
			Fee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseProfit=token;
		else
			CloseProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Turnover=token;
		else
			Turnover="";
		token=getNextToken(NULL);
		if (token!=NULL)
			RelatedOrderID=token;
		else
			RelatedOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessResult=token;
		else
			BusinessResult="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Tradable=token;
		else
			Tradable="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettlementGroup=token;
		else
			SettlementGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PosiDirection=token;
		else
			PosiDirection="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradePrice=token;
		else
			TradePrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenPrice=token;
		else
			OpenPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerOrderID=token;
		else
			TriggerOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SLTriggerPrice=token;
		else
			SLTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TPTriggerPrice=token;
		else
			TPTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyProfit=token;
		else
			CopyProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Position=token;
		else
			Position="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UserID=token;
		else
			UserID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LastPriceByInsert=token;
		else
			LastPriceByInsert="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BidPrice1ByInsert=token;
		else
			BidPrice1ByInsert="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AskPrice1ByInsert=token;
		else
			AskPrice1ByInsert="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Available=token;
		else
			Available="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CreateTime=token;
		else
			CreateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"TradeUnitID"))
			{
				TradeUnitID=token;
			}
			else if (!strcmp(fieldName,"AccountID"))
			{
				AccountID=token;
			}
			else if (!strcmp(fieldName,"LocalID"))
			{
				LocalID=token;
			}
			else if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"OrderPriceType"))
			{
				OrderPriceType=token;
			}
			else if (!strcmp(fieldName,"Direction"))
			{
				Direction=token;
			}
			else if (!strcmp(fieldName,"OffsetFlag"))
			{
				OffsetFlag=token;
			}
			else if (!strcmp(fieldName,"Price"))
			{
				Price=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"VolumeDisplay"))
			{
				VolumeDisplay=token;
			}
			else if (!strcmp(fieldName,"VolumeMode"))
			{
				VolumeMode=token;
			}
			else if (!strcmp(fieldName,"Cost"))
			{
				Cost=token;
			}
			else if (!strcmp(fieldName,"OrderType"))
			{
				OrderType=token;
			}
			else if (!strcmp(fieldName,"GTDTime"))
			{
				GTDTime=token;
			}
			else if (!strcmp(fieldName,"MinVolume"))
			{
				MinVolume=token;
			}
			else if (!strcmp(fieldName,"BusinessType"))
			{
				BusinessType=token;
			}
			else if (!strcmp(fieldName,"BusinessValue"))
			{
				BusinessValue=token;
			}
			else if (!strcmp(fieldName,"CloseOrderID"))
			{
				CloseOrderID=token;
			}
			else if (!strcmp(fieldName,"IsCrossMargin"))
			{
				IsCrossMargin=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"OrderID"))
			{
				OrderID=token;
			}
			else if (!strcmp(fieldName,"CopyMemberID"))
			{
				CopyMemberID=token;
			}
			else if (!strcmp(fieldName,"CopyOrderID"))
			{
				CopyOrderID=token;
			}
			else if (!strcmp(fieldName,"Leverage"))
			{
				Leverage=token;
			}
			else if (!strcmp(fieldName,"CopyProfitRate"))
			{
				CopyProfitRate=token;
			}
			else if (!strcmp(fieldName,"APPID"))
			{
				APPID=token;
			}
			else if (!strcmp(fieldName,"PositionID"))
			{
				PositionID=token;
			}
			else if (!strcmp(fieldName,"TriggerPrice"))
			{
				TriggerPrice=token;
			}
			else if (!strcmp(fieldName,"Reserve"))
			{
				Reserve=token;
			}
			else if (!strcmp(fieldName,"OrderStatus"))
			{
				OrderStatus=token;
			}
			else if (!strcmp(fieldName,"DeriveSource"))
			{
				DeriveSource=token;
			}
			else if (!strcmp(fieldName,"DeriveDetail"))
			{
				DeriveDetail=token;
			}
			else if (!strcmp(fieldName,"VolumeTraded"))
			{
				VolumeTraded=token;
			}
			else if (!strcmp(fieldName,"VolumeRemain"))
			{
				VolumeRemain=token;
			}
			else if (!strcmp(fieldName,"VolumeCancled"))
			{
				VolumeCancled=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			else if (!strcmp(fieldName,"Priority"))
			{
				Priority=token;
			}
			else if (!strcmp(fieldName,"TimeSortNo"))
			{
				TimeSortNo=token;
			}
			else if (!strcmp(fieldName,"FrontNo"))
			{
				FrontNo=token;
			}
			else if (!strcmp(fieldName,"PriceCurrency"))
			{
				PriceCurrency=token;
			}
			else if (!strcmp(fieldName,"FeeCurrency"))
			{
				FeeCurrency=token;
			}
			else if (!strcmp(fieldName,"ClearCurrency"))
			{
				ClearCurrency=token;
			}
			else if (!strcmp(fieldName,"FrozenMoney"))
			{
				FrozenMoney=token;
			}
			else if (!strcmp(fieldName,"FrozenFee"))
			{
				FrozenFee=token;
			}
			else if (!strcmp(fieldName,"FrozenMargin"))
			{
				FrozenMargin=token;
			}
			else if (!strcmp(fieldName,"Fee"))
			{
				Fee=token;
			}
			else if (!strcmp(fieldName,"CloseProfit"))
			{
				CloseProfit=token;
			}
			else if (!strcmp(fieldName,"Turnover"))
			{
				Turnover=token;
			}
			else if (!strcmp(fieldName,"RelatedOrderID"))
			{
				RelatedOrderID=token;
			}
			else if (!strcmp(fieldName,"BusinessResult"))
			{
				BusinessResult=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			else if (!strcmp(fieldName,"Tradable"))
			{
				Tradable=token;
			}
			else if (!strcmp(fieldName,"SettlementGroup"))
			{
				SettlementGroup=token;
			}
			else if (!strcmp(fieldName,"PosiDirection"))
			{
				PosiDirection=token;
			}
			else if (!strcmp(fieldName,"TradePrice"))
			{
				TradePrice=token;
			}
			else if (!strcmp(fieldName,"OpenPrice"))
			{
				OpenPrice=token;
			}
			else if (!strcmp(fieldName,"TriggerOrderID"))
			{
				TriggerOrderID=token;
			}
			else if (!strcmp(fieldName,"SLTriggerPrice"))
			{
				SLTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"TPTriggerPrice"))
			{
				TPTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"CopyProfit"))
			{
				CopyProfit=token;
			}
			else if (!strcmp(fieldName,"Position"))
			{
				Position=token;
			}
			else if (!strcmp(fieldName,"UserID"))
			{
				UserID=token;
			}
			else if (!strcmp(fieldName,"LastPriceByInsert"))
			{
				LastPriceByInsert=token;
			}
			else if (!strcmp(fieldName,"BidPrice1ByInsert"))
			{
				BidPrice1ByInsert=token;
			}
			else if (!strcmp(fieldName,"AskPrice1ByInsert"))
			{
				AskPrice1ByInsert=token;
			}
			else if (!strcmp(fieldName,"Available"))
			{
				Available=token;
			}
			else if (!strcmp(fieldName,"CreateTime"))
			{
				CreateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableOrder::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableOrder),output)!=sizeof(CWriteableOrder))
		return 0;
	else
		return 1;
}

int CWriteableOrder::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeUnitID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderPriceType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OffsetFlag.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeDisplay.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeMode.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Cost.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",GTDTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MinVolume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsCrossMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Leverage.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfitRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Reserve.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderStatus.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",DeriveSource.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",DeriveDetail.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeTraded.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeRemain.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeCancled.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Priority.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TimeSortNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrontNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClearCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMoney.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenFee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Fee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",RelatedOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessResult.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Tradable.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PosiDirection.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradePrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SLTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TPTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Position.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UserID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastPriceByInsert.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BidPrice1ByInsert.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AskPrice1ByInsert.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Available.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableOrder::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeUnitID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderPriceType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OffsetFlag.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeDisplay.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeMode.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Cost.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",GTDTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MinVolume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsCrossMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Leverage.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfitRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Reserve.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderStatus.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",DeriveSource.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",DeriveDetail.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeTraded.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeRemain.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeCancled.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Priority.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TimeSortNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrontNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClearCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMoney.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenFee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Fee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",RelatedOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessResult.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Tradable.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PosiDirection.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradePrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SLTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TPTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Position.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UserID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastPriceByInsert.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BidPrice1ByInsert.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AskPrice1ByInsert.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Available.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableOrder::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCOrder%s = {\n", index_buf);
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tTradeUnitID = %s    [交易单元代码]\n",TradeUnitID.getString().c_str());	
	fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());	
	fprintf(fp,"\t\tLocalID = %s    [报单本地标识]\n",LocalID.getString().c_str());	
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tOrderPriceType = %s    [报单价格类型]\n",OrderPriceType.getString().c_str());	
	fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());	
	fprintf(fp,"\t\tOffsetFlag = %s    [开平标志]\n",OffsetFlag.getString().c_str());	
	fprintf(fp,"\t\tPrice = %s    [报单价格]\n",Price.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tVolumeDisplay = %s    [显示数量]\n",VolumeDisplay.getString().c_str());	
	fprintf(fp,"\t\tVolumeMode = %s    [数量取值方式]\n",VolumeMode.getString().c_str());	
	fprintf(fp,"\t\tCost = %s    [委托额]\n",Cost.getString().c_str());	
	fprintf(fp,"\t\tOrderType = %s    [订单类型]\n",OrderType.getString().c_str());	
	fprintf(fp,"\t\tGTDTime = %s    [GTD时间]\n",GTDTime.getString().c_str());	
	fprintf(fp,"\t\tMinVolume = %s    [最小成交量]\n",MinVolume.getString().c_str());	
	fprintf(fp,"\t\tBusinessType = %s    [业务类别]\n",BusinessType.getString().c_str());	
	fprintf(fp,"\t\tBusinessValue = %s    [业务值]\n",BusinessValue.getString().c_str());	
	fprintf(fp,"\t\tCloseOrderID = %s    [平仓指定开仓的订单号]\n",CloseOrderID.getString().c_str());	
	fprintf(fp,"\t\tIsCrossMargin = %s    [是否全仓]\n",IsCrossMargin.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tOrderID = %s    [报单系统唯一代码]\n",OrderID.getString().c_str());	
	fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());	
	fprintf(fp,"\t\tCopyOrderID = %s    [带单员报单号]\n",CopyOrderID.getString().c_str());	
	fprintf(fp,"\t\tLeverage = %s    [委托单杠杆倍数]\n",Leverage.getString().c_str());	
	fprintf(fp,"\t\tCopyProfitRate = %s    [带单分配比例]\n",CopyProfitRate.getString().c_str());	
	fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());	
	fprintf(fp,"\t\tPositionID = %s    [持仓代码]\n",PositionID.getString().c_str());	
	fprintf(fp,"\t\tTriggerPrice = %s    [触发价]\n",TriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tReserve = %s    [保留资金]\n",Reserve.getString().c_str());	
	fprintf(fp,"\t\tOrderStatus = %s    [报单状态]\n",OrderStatus.getString().c_str());	
	fprintf(fp,"\t\tDeriveSource = %s    [衍生来源]\n",DeriveSource.getString().c_str());	
	fprintf(fp,"\t\tDeriveDetail = %s    [衍生明细]\n",DeriveDetail.getString().c_str());	
	fprintf(fp,"\t\tVolumeTraded = %s    [成交数量]\n",VolumeTraded.getString().c_str());	
	fprintf(fp,"\t\tVolumeRemain = %s    [剩余数量]\n",VolumeRemain.getString().c_str());	
	fprintf(fp,"\t\tVolumeCancled = %s    [已经撤单数量]\n",VolumeCancled.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t\tPriority = %s    [优先权]\n",Priority.getString().c_str());	
	fprintf(fp,"\t\tTimeSortNo = %s    [按时间排队的序号]\n",TimeSortNo.getString().c_str());	
	fprintf(fp,"\t\tFrontNo = %s    [前置编号]\n",FrontNo.getString().c_str());	
	fprintf(fp,"\t\tPriceCurrency = %s    [计价币种]\n",PriceCurrency.getString().c_str());	
	fprintf(fp,"\t\tFeeCurrency = %s    [手续费币种]\n",FeeCurrency.getString().c_str());	
	fprintf(fp,"\t\tClearCurrency = %s    [清算币种]\n",ClearCurrency.getString().c_str());	
	fprintf(fp,"\t\tFrozenMoney = %s    [冻结资金]\n",FrozenMoney.getString().c_str());	
	fprintf(fp,"\t\tFrozenFee = %s    [冻结手续费]\n",FrozenFee.getString().c_str());	
	fprintf(fp,"\t\tFrozenMargin = %s    [冻结保证金]\n",FrozenMargin.getString().c_str());	
	fprintf(fp,"\t\tFee = %s    [手续费]\n",Fee.getString().c_str());	
	fprintf(fp,"\t\tCloseProfit = %s    [平仓盈亏]\n",CloseProfit.getString().c_str());	
	fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());	
	fprintf(fp,"\t\tRelatedOrderID = %s    [关联报单号]\n",RelatedOrderID.getString().c_str());	
	fprintf(fp,"\t\tBusinessResult = %s    [业务执行结果]\n",BusinessResult.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t\tTradable = %s    [是否可以参加交易]\n",Tradable.getString().c_str());	
	fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());	
	fprintf(fp,"\t\tPosiDirection = %s    [持仓多空方向]\n",PosiDirection.getString().c_str());	
	fprintf(fp,"\t\tTradePrice = %s    [成交均价]\n",TradePrice.getString().c_str());	
	fprintf(fp,"\t\tOpenPrice = %s    [平仓成交时的开仓均价]\n",OpenPrice.getString().c_str());	
	fprintf(fp,"\t\tTriggerOrderID = %s    [触发报单号]\n",TriggerOrderID.getString().c_str());	
	fprintf(fp,"\t\tSLTriggerPrice = %s    [开仓报单成交之后的止损触发价]\n",SLTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tTPTriggerPrice = %s    [开仓报单成交之后的止盈触发价]\n",TPTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tCopyProfit = %s    [带单盈利分配]\n",CopyProfit.getString().c_str());	
	fprintf(fp,"\t\tPosition = %s    [成交时的持仓量]\n",Position.getString().c_str());	
	fprintf(fp,"\t\tUserID = %s    [交易用户代码]\n",UserID.getString().c_str());	
	fprintf(fp,"\t\tLastPriceByInsert = %s    [插入时的最新价]\n",LastPriceByInsert.getString().c_str());	
	fprintf(fp,"\t\tBidPrice1ByInsert = %s    [插入时的买一价]\n",BidPrice1ByInsert.getString().c_str());	
	fprintf(fp,"\t\tAskPrice1ByInsert = %s    [插入时的卖一价]\n",AskPrice1ByInsert.getString().c_str());	
	fprintf(fp,"\t\tAvailable = %s    [可用资金]\n",Available.getString().c_str());	
	fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableOrder::dumpDiff(FILE *fp, const CWriteableOrder *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCOrder = {\n");
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(TradeUnitID.getValue(),pOriginal->TradeUnitID.getValue())!=0)
		fprintf(fp,"\t\tTradeUnitID = %s -> %s    [交易单元代码]\n",pOriginal->TradeUnitID.getString().c_str(),TradeUnitID.getString().c_str());
	else
		fprintf(fp,"\t\tTradeUnitID = %s    [交易单元代码]\n",TradeUnitID.getString().c_str());
	if (compare(AccountID.getValue(),pOriginal->AccountID.getValue())!=0)
		fprintf(fp,"\t\tAccountID = %s -> %s    [资金账号]\n",pOriginal->AccountID.getString().c_str(),AccountID.getString().c_str());
	else
		fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());
	if (compare(LocalID.getValue(),pOriginal->LocalID.getValue())!=0)
		fprintf(fp,"\t\tLocalID = %s -> %s    [报单本地标识]\n",pOriginal->LocalID.getString().c_str(),LocalID.getString().c_str());
	else
		fprintf(fp,"\t\tLocalID = %s    [报单本地标识]\n",LocalID.getString().c_str());
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(OrderPriceType.getValue(),pOriginal->OrderPriceType.getValue())!=0)
		fprintf(fp,"\t\tOrderPriceType = %s -> %s    [报单价格类型]\n",pOriginal->OrderPriceType.getString().c_str(),OrderPriceType.getString().c_str());
	else
		fprintf(fp,"\t\tOrderPriceType = %s    [报单价格类型]\n",OrderPriceType.getString().c_str());
	if (compare(Direction.getValue(),pOriginal->Direction.getValue())!=0)
		fprintf(fp,"\t\tDirection = %s -> %s    [买卖方向]\n",pOriginal->Direction.getString().c_str(),Direction.getString().c_str());
	else
		fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());
	if (compare(OffsetFlag.getValue(),pOriginal->OffsetFlag.getValue())!=0)
		fprintf(fp,"\t\tOffsetFlag = %s -> %s    [开平标志]\n",pOriginal->OffsetFlag.getString().c_str(),OffsetFlag.getString().c_str());
	else
		fprintf(fp,"\t\tOffsetFlag = %s    [开平标志]\n",OffsetFlag.getString().c_str());
	if (compare(Price.getValue(),pOriginal->Price.getValue())!=0)
		fprintf(fp,"\t\tPrice = %s -> %s    [报单价格]\n",pOriginal->Price.getString().c_str(),Price.getString().c_str());
	else
		fprintf(fp,"\t\tPrice = %s    [报单价格]\n",Price.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [数量]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());
	if (compare(VolumeDisplay.getValue(),pOriginal->VolumeDisplay.getValue())!=0)
		fprintf(fp,"\t\tVolumeDisplay = %s -> %s    [显示数量]\n",pOriginal->VolumeDisplay.getString().c_str(),VolumeDisplay.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeDisplay = %s    [显示数量]\n",VolumeDisplay.getString().c_str());
	if (compare(VolumeMode.getValue(),pOriginal->VolumeMode.getValue())!=0)
		fprintf(fp,"\t\tVolumeMode = %s -> %s    [数量取值方式]\n",pOriginal->VolumeMode.getString().c_str(),VolumeMode.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeMode = %s    [数量取值方式]\n",VolumeMode.getString().c_str());
	if (compare(Cost.getValue(),pOriginal->Cost.getValue())!=0)
		fprintf(fp,"\t\tCost = %s -> %s    [委托额]\n",pOriginal->Cost.getString().c_str(),Cost.getString().c_str());
	else
		fprintf(fp,"\t\tCost = %s    [委托额]\n",Cost.getString().c_str());
	if (compare(OrderType.getValue(),pOriginal->OrderType.getValue())!=0)
		fprintf(fp,"\t\tOrderType = %s -> %s    [订单类型]\n",pOriginal->OrderType.getString().c_str(),OrderType.getString().c_str());
	else
		fprintf(fp,"\t\tOrderType = %s    [订单类型]\n",OrderType.getString().c_str());
	if (compare(GTDTime.getValue(),pOriginal->GTDTime.getValue())!=0)
		fprintf(fp,"\t\tGTDTime = %s -> %s    [GTD时间]\n",pOriginal->GTDTime.getString().c_str(),GTDTime.getString().c_str());
	else
		fprintf(fp,"\t\tGTDTime = %s    [GTD时间]\n",GTDTime.getString().c_str());
	if (compare(MinVolume.getValue(),pOriginal->MinVolume.getValue())!=0)
		fprintf(fp,"\t\tMinVolume = %s -> %s    [最小成交量]\n",pOriginal->MinVolume.getString().c_str(),MinVolume.getString().c_str());
	else
		fprintf(fp,"\t\tMinVolume = %s    [最小成交量]\n",MinVolume.getString().c_str());
	if (compare(BusinessType.getValue(),pOriginal->BusinessType.getValue())!=0)
		fprintf(fp,"\t\tBusinessType = %s -> %s    [业务类别]\n",pOriginal->BusinessType.getString().c_str(),BusinessType.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessType = %s    [业务类别]\n",BusinessType.getString().c_str());
	if (compare(BusinessValue.getValue(),pOriginal->BusinessValue.getValue())!=0)
		fprintf(fp,"\t\tBusinessValue = %s -> %s    [业务值]\n",pOriginal->BusinessValue.getString().c_str(),BusinessValue.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessValue = %s    [业务值]\n",BusinessValue.getString().c_str());
	if (compare(CloseOrderID.getValue(),pOriginal->CloseOrderID.getValue())!=0)
		fprintf(fp,"\t\tCloseOrderID = %s -> %s    [平仓指定开仓的订单号]\n",pOriginal->CloseOrderID.getString().c_str(),CloseOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tCloseOrderID = %s    [平仓指定开仓的订单号]\n",CloseOrderID.getString().c_str());
	if (compare(IsCrossMargin.getValue(),pOriginal->IsCrossMargin.getValue())!=0)
		fprintf(fp,"\t\tIsCrossMargin = %s -> %s    [是否全仓]\n",pOriginal->IsCrossMargin.getString().c_str(),IsCrossMargin.getString().c_str());
	else
		fprintf(fp,"\t\tIsCrossMargin = %s    [是否全仓]\n",IsCrossMargin.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(OrderID.getValue(),pOriginal->OrderID.getValue())!=0)
		fprintf(fp,"\t\tOrderID = %s -> %s    [报单系统唯一代码]\n",pOriginal->OrderID.getString().c_str(),OrderID.getString().c_str());
	else
		fprintf(fp,"\t\tOrderID = %s    [报单系统唯一代码]\n",OrderID.getString().c_str());
	if (compare(CopyMemberID.getValue(),pOriginal->CopyMemberID.getValue())!=0)
		fprintf(fp,"\t\tCopyMemberID = %s -> %s    [带单员代码]\n",pOriginal->CopyMemberID.getString().c_str(),CopyMemberID.getString().c_str());
	else
		fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());
	if (compare(CopyOrderID.getValue(),pOriginal->CopyOrderID.getValue())!=0)
		fprintf(fp,"\t\tCopyOrderID = %s -> %s    [带单员报单号]\n",pOriginal->CopyOrderID.getString().c_str(),CopyOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tCopyOrderID = %s    [带单员报单号]\n",CopyOrderID.getString().c_str());
	if (compare(Leverage.getValue(),pOriginal->Leverage.getValue())!=0)
		fprintf(fp,"\t\tLeverage = %s -> %s    [委托单杠杆倍数]\n",pOriginal->Leverage.getString().c_str(),Leverage.getString().c_str());
	else
		fprintf(fp,"\t\tLeverage = %s    [委托单杠杆倍数]\n",Leverage.getString().c_str());
	if (compare(CopyProfitRate.getValue(),pOriginal->CopyProfitRate.getValue())!=0)
		fprintf(fp,"\t\tCopyProfitRate = %s -> %s    [带单分配比例]\n",pOriginal->CopyProfitRate.getString().c_str(),CopyProfitRate.getString().c_str());
	else
		fprintf(fp,"\t\tCopyProfitRate = %s    [带单分配比例]\n",CopyProfitRate.getString().c_str());
	if (compare(APPID.getValue(),pOriginal->APPID.getValue())!=0)
		fprintf(fp,"\t\tAPPID = %s -> %s    [应用编号]\n",pOriginal->APPID.getString().c_str(),APPID.getString().c_str());
	else
		fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());
	if (compare(PositionID.getValue(),pOriginal->PositionID.getValue())!=0)
		fprintf(fp,"\t\tPositionID = %s -> %s    [持仓代码]\n",pOriginal->PositionID.getString().c_str(),PositionID.getString().c_str());
	else
		fprintf(fp,"\t\tPositionID = %s    [持仓代码]\n",PositionID.getString().c_str());
	if (compare(TriggerPrice.getValue(),pOriginal->TriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tTriggerPrice = %s -> %s    [触发价]\n",pOriginal->TriggerPrice.getString().c_str(),TriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerPrice = %s    [触发价]\n",TriggerPrice.getString().c_str());
	if (compare(Reserve.getValue(),pOriginal->Reserve.getValue())!=0)
		fprintf(fp,"\t\tReserve = %s -> %s    [保留资金]\n",pOriginal->Reserve.getString().c_str(),Reserve.getString().c_str());
	else
		fprintf(fp,"\t\tReserve = %s    [保留资金]\n",Reserve.getString().c_str());
	if (compare(OrderStatus.getValue(),pOriginal->OrderStatus.getValue())!=0)
		fprintf(fp,"\t\tOrderStatus = %s -> %s    [报单状态]\n",pOriginal->OrderStatus.getString().c_str(),OrderStatus.getString().c_str());
	else
		fprintf(fp,"\t\tOrderStatus = %s    [报单状态]\n",OrderStatus.getString().c_str());
	if (compare(DeriveSource.getValue(),pOriginal->DeriveSource.getValue())!=0)
		fprintf(fp,"\t\tDeriveSource = %s -> %s    [衍生来源]\n",pOriginal->DeriveSource.getString().c_str(),DeriveSource.getString().c_str());
	else
		fprintf(fp,"\t\tDeriveSource = %s    [衍生来源]\n",DeriveSource.getString().c_str());
	if (compare(DeriveDetail.getValue(),pOriginal->DeriveDetail.getValue())!=0)
		fprintf(fp,"\t\tDeriveDetail = %s -> %s    [衍生明细]\n",pOriginal->DeriveDetail.getString().c_str(),DeriveDetail.getString().c_str());
	else
		fprintf(fp,"\t\tDeriveDetail = %s    [衍生明细]\n",DeriveDetail.getString().c_str());
	if (compare(VolumeTraded.getValue(),pOriginal->VolumeTraded.getValue())!=0)
		fprintf(fp,"\t\tVolumeTraded = %s -> %s    [成交数量]\n",pOriginal->VolumeTraded.getString().c_str(),VolumeTraded.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeTraded = %s    [成交数量]\n",VolumeTraded.getString().c_str());
	if (compare(VolumeRemain.getValue(),pOriginal->VolumeRemain.getValue())!=0)
		fprintf(fp,"\t\tVolumeRemain = %s -> %s    [剩余数量]\n",pOriginal->VolumeRemain.getString().c_str(),VolumeRemain.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeRemain = %s    [剩余数量]\n",VolumeRemain.getString().c_str());
	if (compare(VolumeCancled.getValue(),pOriginal->VolumeCancled.getValue())!=0)
		fprintf(fp,"\t\tVolumeCancled = %s -> %s    [已经撤单数量]\n",pOriginal->VolumeCancled.getString().c_str(),VolumeCancled.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeCancled = %s    [已经撤单数量]\n",VolumeCancled.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	if (compare(Priority.getValue(),pOriginal->Priority.getValue())!=0)
		fprintf(fp,"\t\tPriority = %s -> %s    [优先权]\n",pOriginal->Priority.getString().c_str(),Priority.getString().c_str());
	else
		fprintf(fp,"\t\tPriority = %s    [优先权]\n",Priority.getString().c_str());
	if (compare(TimeSortNo.getValue(),pOriginal->TimeSortNo.getValue())!=0)
		fprintf(fp,"\t\tTimeSortNo = %s -> %s    [按时间排队的序号]\n",pOriginal->TimeSortNo.getString().c_str(),TimeSortNo.getString().c_str());
	else
		fprintf(fp,"\t\tTimeSortNo = %s    [按时间排队的序号]\n",TimeSortNo.getString().c_str());
	if (compare(FrontNo.getValue(),pOriginal->FrontNo.getValue())!=0)
		fprintf(fp,"\t\tFrontNo = %s -> %s    [前置编号]\n",pOriginal->FrontNo.getString().c_str(),FrontNo.getString().c_str());
	else
		fprintf(fp,"\t\tFrontNo = %s    [前置编号]\n",FrontNo.getString().c_str());
	if (compare(PriceCurrency.getValue(),pOriginal->PriceCurrency.getValue())!=0)
		fprintf(fp,"\t\tPriceCurrency = %s -> %s    [计价币种]\n",pOriginal->PriceCurrency.getString().c_str(),PriceCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tPriceCurrency = %s    [计价币种]\n",PriceCurrency.getString().c_str());
	if (compare(FeeCurrency.getValue(),pOriginal->FeeCurrency.getValue())!=0)
		fprintf(fp,"\t\tFeeCurrency = %s -> %s    [手续费币种]\n",pOriginal->FeeCurrency.getString().c_str(),FeeCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tFeeCurrency = %s    [手续费币种]\n",FeeCurrency.getString().c_str());
	if (compare(ClearCurrency.getValue(),pOriginal->ClearCurrency.getValue())!=0)
		fprintf(fp,"\t\tClearCurrency = %s -> %s    [清算币种]\n",pOriginal->ClearCurrency.getString().c_str(),ClearCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tClearCurrency = %s    [清算币种]\n",ClearCurrency.getString().c_str());
	if (compare(FrozenMoney.getValue(),pOriginal->FrozenMoney.getValue())!=0)
		fprintf(fp,"\t\tFrozenMoney = %s -> %s    [冻结资金]\n",pOriginal->FrozenMoney.getString().c_str(),FrozenMoney.getString().c_str());
	else
		fprintf(fp,"\t\tFrozenMoney = %s    [冻结资金]\n",FrozenMoney.getString().c_str());
	if (compare(FrozenFee.getValue(),pOriginal->FrozenFee.getValue())!=0)
		fprintf(fp,"\t\tFrozenFee = %s -> %s    [冻结手续费]\n",pOriginal->FrozenFee.getString().c_str(),FrozenFee.getString().c_str());
	else
		fprintf(fp,"\t\tFrozenFee = %s    [冻结手续费]\n",FrozenFee.getString().c_str());
	if (compare(FrozenMargin.getValue(),pOriginal->FrozenMargin.getValue())!=0)
		fprintf(fp,"\t\tFrozenMargin = %s -> %s    [冻结保证金]\n",pOriginal->FrozenMargin.getString().c_str(),FrozenMargin.getString().c_str());
	else
		fprintf(fp,"\t\tFrozenMargin = %s    [冻结保证金]\n",FrozenMargin.getString().c_str());
	if (compare(Fee.getValue(),pOriginal->Fee.getValue())!=0)
		fprintf(fp,"\t\tFee = %s -> %s    [手续费]\n",pOriginal->Fee.getString().c_str(),Fee.getString().c_str());
	else
		fprintf(fp,"\t\tFee = %s    [手续费]\n",Fee.getString().c_str());
	if (compare(CloseProfit.getValue(),pOriginal->CloseProfit.getValue())!=0)
		fprintf(fp,"\t\tCloseProfit = %s -> %s    [平仓盈亏]\n",pOriginal->CloseProfit.getString().c_str(),CloseProfit.getString().c_str());
	else
		fprintf(fp,"\t\tCloseProfit = %s    [平仓盈亏]\n",CloseProfit.getString().c_str());
	if (compare(Turnover.getValue(),pOriginal->Turnover.getValue())!=0)
		fprintf(fp,"\t\tTurnover = %s -> %s    [成交金额]\n",pOriginal->Turnover.getString().c_str(),Turnover.getString().c_str());
	else
		fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());
	if (compare(RelatedOrderID.getValue(),pOriginal->RelatedOrderID.getValue())!=0)
		fprintf(fp,"\t\tRelatedOrderID = %s -> %s    [关联报单号]\n",pOriginal->RelatedOrderID.getString().c_str(),RelatedOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tRelatedOrderID = %s    [关联报单号]\n",RelatedOrderID.getString().c_str());
	if (compare(BusinessResult.getValue(),pOriginal->BusinessResult.getValue())!=0)
		fprintf(fp,"\t\tBusinessResult = %s -> %s    [业务执行结果]\n",pOriginal->BusinessResult.getString().c_str(),BusinessResult.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessResult = %s    [业务执行结果]\n",BusinessResult.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [业务序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());
	if (compare(Tradable.getValue(),pOriginal->Tradable.getValue())!=0)
		fprintf(fp,"\t\tTradable = %s -> %s    [是否可以参加交易]\n",pOriginal->Tradable.getString().c_str(),Tradable.getString().c_str());
	else
		fprintf(fp,"\t\tTradable = %s    [是否可以参加交易]\n",Tradable.getString().c_str());
	if (compare(SettlementGroup.getValue(),pOriginal->SettlementGroup.getValue())!=0)
		fprintf(fp,"\t\tSettlementGroup = %s -> %s    [结算组编号]\n",pOriginal->SettlementGroup.getString().c_str(),SettlementGroup.getString().c_str());
	else
		fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());
	if (compare(PosiDirection.getValue(),pOriginal->PosiDirection.getValue())!=0)
		fprintf(fp,"\t\tPosiDirection = %s -> %s    [持仓多空方向]\n",pOriginal->PosiDirection.getString().c_str(),PosiDirection.getString().c_str());
	else
		fprintf(fp,"\t\tPosiDirection = %s    [持仓多空方向]\n",PosiDirection.getString().c_str());
	if (compare(TradePrice.getValue(),pOriginal->TradePrice.getValue())!=0)
		fprintf(fp,"\t\tTradePrice = %s -> %s    [成交均价]\n",pOriginal->TradePrice.getString().c_str(),TradePrice.getString().c_str());
	else
		fprintf(fp,"\t\tTradePrice = %s    [成交均价]\n",TradePrice.getString().c_str());
	if (compare(OpenPrice.getValue(),pOriginal->OpenPrice.getValue())!=0)
		fprintf(fp,"\t\tOpenPrice = %s -> %s    [平仓成交时的开仓均价]\n",pOriginal->OpenPrice.getString().c_str(),OpenPrice.getString().c_str());
	else
		fprintf(fp,"\t\tOpenPrice = %s    [平仓成交时的开仓均价]\n",OpenPrice.getString().c_str());
	if (compare(TriggerOrderID.getValue(),pOriginal->TriggerOrderID.getValue())!=0)
		fprintf(fp,"\t\tTriggerOrderID = %s -> %s    [触发报单号]\n",pOriginal->TriggerOrderID.getString().c_str(),TriggerOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerOrderID = %s    [触发报单号]\n",TriggerOrderID.getString().c_str());
	if (compare(SLTriggerPrice.getValue(),pOriginal->SLTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tSLTriggerPrice = %s -> %s    [开仓报单成交之后的止损触发价]\n",pOriginal->SLTriggerPrice.getString().c_str(),SLTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tSLTriggerPrice = %s    [开仓报单成交之后的止损触发价]\n",SLTriggerPrice.getString().c_str());
	if (compare(TPTriggerPrice.getValue(),pOriginal->TPTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tTPTriggerPrice = %s -> %s    [开仓报单成交之后的止盈触发价]\n",pOriginal->TPTriggerPrice.getString().c_str(),TPTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tTPTriggerPrice = %s    [开仓报单成交之后的止盈触发价]\n",TPTriggerPrice.getString().c_str());
	if (compare(CopyProfit.getValue(),pOriginal->CopyProfit.getValue())!=0)
		fprintf(fp,"\t\tCopyProfit = %s -> %s    [带单盈利分配]\n",pOriginal->CopyProfit.getString().c_str(),CopyProfit.getString().c_str());
	else
		fprintf(fp,"\t\tCopyProfit = %s    [带单盈利分配]\n",CopyProfit.getString().c_str());
	if (compare(Position.getValue(),pOriginal->Position.getValue())!=0)
		fprintf(fp,"\t\tPosition = %s -> %s    [成交时的持仓量]\n",pOriginal->Position.getString().c_str(),Position.getString().c_str());
	else
		fprintf(fp,"\t\tPosition = %s    [成交时的持仓量]\n",Position.getString().c_str());
	if (compare(UserID.getValue(),pOriginal->UserID.getValue())!=0)
		fprintf(fp,"\t\tUserID = %s -> %s    [交易用户代码]\n",pOriginal->UserID.getString().c_str(),UserID.getString().c_str());
	else
		fprintf(fp,"\t\tUserID = %s    [交易用户代码]\n",UserID.getString().c_str());
	if (compare(LastPriceByInsert.getValue(),pOriginal->LastPriceByInsert.getValue())!=0)
		fprintf(fp,"\t\tLastPriceByInsert = %s -> %s    [插入时的最新价]\n",pOriginal->LastPriceByInsert.getString().c_str(),LastPriceByInsert.getString().c_str());
	else
		fprintf(fp,"\t\tLastPriceByInsert = %s    [插入时的最新价]\n",LastPriceByInsert.getString().c_str());
	if (compare(BidPrice1ByInsert.getValue(),pOriginal->BidPrice1ByInsert.getValue())!=0)
		fprintf(fp,"\t\tBidPrice1ByInsert = %s -> %s    [插入时的买一价]\n",pOriginal->BidPrice1ByInsert.getString().c_str(),BidPrice1ByInsert.getString().c_str());
	else
		fprintf(fp,"\t\tBidPrice1ByInsert = %s    [插入时的买一价]\n",BidPrice1ByInsert.getString().c_str());
	if (compare(AskPrice1ByInsert.getValue(),pOriginal->AskPrice1ByInsert.getValue())!=0)
		fprintf(fp,"\t\tAskPrice1ByInsert = %s -> %s    [插入时的卖一价]\n",pOriginal->AskPrice1ByInsert.getString().c_str(),AskPrice1ByInsert.getString().c_str());
	else
		fprintf(fp,"\t\tAskPrice1ByInsert = %s    [插入时的卖一价]\n",AskPrice1ByInsert.getString().c_str());
	if (compare(Available.getValue(),pOriginal->Available.getValue())!=0)
		fprintf(fp,"\t\tAvailable = %s -> %s    [可用资金]\n",pOriginal->Available.getString().c_str(),Available.getString().c_str());
	else
		fprintf(fp,"\t\tAvailable = %s    [可用资金]\n",Available.getString().c_str());
	if (compare(CreateTime.getValue(),pOriginal->CreateTime.getValue())!=0)
		fprintf(fp,"\t\tCreateTime = %s -> %s    [创建时间]\n",pOriginal->CreateTime.getString().c_str(),CreateTime.getString().c_str());
	else
		fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableOrder::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",TradeUnitID=%s",TradeUnitID.getString().c_str());	
	fprintf(fp,",AccountID=%s",AccountID.getString().c_str());	
	fprintf(fp,",LocalID=%s",LocalID.getString().c_str());	
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",OrderPriceType=%s",OrderPriceType.getString().c_str());	
	fprintf(fp,",Direction=%s",Direction.getString().c_str());	
	fprintf(fp,",OffsetFlag=%s",OffsetFlag.getString().c_str());	
	fprintf(fp,",Price=%s",Price.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",VolumeDisplay=%s",VolumeDisplay.getString().c_str());	
	fprintf(fp,",VolumeMode=%s",VolumeMode.getString().c_str());	
	fprintf(fp,",Cost=%s",Cost.getString().c_str());	
	fprintf(fp,",OrderType=%s",OrderType.getString().c_str());	
	fprintf(fp,",GTDTime=%s",GTDTime.getString().c_str());	
	fprintf(fp,",MinVolume=%s",MinVolume.getString().c_str());	
	fprintf(fp,",BusinessType=%s",BusinessType.getString().c_str());	
	fprintf(fp,",BusinessValue=%s",BusinessValue.getString().c_str());	
	fprintf(fp,",CloseOrderID=%s",CloseOrderID.getString().c_str());	
	fprintf(fp,",IsCrossMargin=%s",IsCrossMargin.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",OrderID=%s",OrderID.getString().c_str());	
	fprintf(fp,",CopyMemberID=%s",CopyMemberID.getString().c_str());	
	fprintf(fp,",CopyOrderID=%s",CopyOrderID.getString().c_str());	
	fprintf(fp,",Leverage=%s",Leverage.getString().c_str());	
	fprintf(fp,",CopyProfitRate=%s",CopyProfitRate.getString().c_str());	
	fprintf(fp,",APPID=%s",APPID.getString().c_str());	
	fprintf(fp,",PositionID=%s",PositionID.getString().c_str());	
	fprintf(fp,",TriggerPrice=%s",TriggerPrice.getString().c_str());	
	fprintf(fp,",Reserve=%s",Reserve.getString().c_str());	
	fprintf(fp,",OrderStatus=%s",OrderStatus.getString().c_str());	
	fprintf(fp,",DeriveSource=%s",DeriveSource.getString().c_str());	
	fprintf(fp,",DeriveDetail=%s",DeriveDetail.getString().c_str());	
	fprintf(fp,",VolumeTraded=%s",VolumeTraded.getString().c_str());	
	fprintf(fp,",VolumeRemain=%s",VolumeRemain.getString().c_str());	
	fprintf(fp,",VolumeCancled=%s",VolumeCancled.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,",Priority=%s",Priority.getString().c_str());	
	fprintf(fp,",TimeSortNo=%s",TimeSortNo.getString().c_str());	
	fprintf(fp,",FrontNo=%s",FrontNo.getString().c_str());	
	fprintf(fp,",PriceCurrency=%s",PriceCurrency.getString().c_str());	
	fprintf(fp,",FeeCurrency=%s",FeeCurrency.getString().c_str());	
	fprintf(fp,",ClearCurrency=%s",ClearCurrency.getString().c_str());	
	fprintf(fp,",FrozenMoney=%s",FrozenMoney.getString().c_str());	
	fprintf(fp,",FrozenFee=%s",FrozenFee.getString().c_str());	
	fprintf(fp,",FrozenMargin=%s",FrozenMargin.getString().c_str());	
	fprintf(fp,",Fee=%s",Fee.getString().c_str());	
	fprintf(fp,",CloseProfit=%s",CloseProfit.getString().c_str());	
	fprintf(fp,",Turnover=%s",Turnover.getString().c_str());	
	fprintf(fp,",RelatedOrderID=%s",RelatedOrderID.getString().c_str());	
	fprintf(fp,",BusinessResult=%s",BusinessResult.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,",Tradable=%s",Tradable.getString().c_str());	
	fprintf(fp,",SettlementGroup=%s",SettlementGroup.getString().c_str());	
	fprintf(fp,",PosiDirection=%s",PosiDirection.getString().c_str());	
	fprintf(fp,",TradePrice=%s",TradePrice.getString().c_str());	
	fprintf(fp,",OpenPrice=%s",OpenPrice.getString().c_str());	
	fprintf(fp,",TriggerOrderID=%s",TriggerOrderID.getString().c_str());	
	fprintf(fp,",SLTriggerPrice=%s",SLTriggerPrice.getString().c_str());	
	fprintf(fp,",TPTriggerPrice=%s",TPTriggerPrice.getString().c_str());	
	fprintf(fp,",CopyProfit=%s",CopyProfit.getString().c_str());	
	fprintf(fp,",Position=%s",Position.getString().c_str());	
	fprintf(fp,",UserID=%s",UserID.getString().c_str());	
	fprintf(fp,",LastPriceByInsert=%s",LastPriceByInsert.getString().c_str());	
	fprintf(fp,",BidPrice1ByInsert=%s",BidPrice1ByInsert.getString().c_str());	
	fprintf(fp,",AskPrice1ByInsert=%s",AskPrice1ByInsert.getString().c_str());	
	fprintf(fp,",Available=%s",Available.getString().c_str());	
	fprintf(fp,",CreateTime=%s",CreateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableOrder::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradeUnitID=%s,",TradeUnitID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccountID=%s,",AccountID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LocalID=%s,",LocalID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderPriceType=%s,",OrderPriceType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Direction=%s,",Direction.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OffsetFlag=%s,",OffsetFlag.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Price=%s,",Price.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeDisplay=%s,",VolumeDisplay.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeMode=%s,",VolumeMode.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Cost=%s,",Cost.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderType=%s,",OrderType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"GTDTime=%s,",GTDTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MinVolume=%s,",MinVolume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessType=%s,",BusinessType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessValue=%s,",BusinessValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseOrderID=%s,",CloseOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"IsCrossMargin=%s,",IsCrossMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderID=%s,",OrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyMemberID=%s,",CopyMemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyOrderID=%s,",CopyOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Leverage=%s,",Leverage.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyProfitRate=%s,",CopyProfitRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"APPID=%s,",APPID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionID=%s,",PositionID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerPrice=%s,",TriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Reserve=%s,",Reserve.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderStatus=%s,",OrderStatus.getString().c_str());
	p+=strlen(p);
	sprintf(p,"DeriveSource=%s,",DeriveSource.getString().c_str());
	p+=strlen(p);
	sprintf(p,"DeriveDetail=%s,",DeriveDetail.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeTraded=%s,",VolumeTraded.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeRemain=%s,",VolumeRemain.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeCancled=%s,",VolumeCancled.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Priority=%s,",Priority.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TimeSortNo=%s,",TimeSortNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrontNo=%s,",FrontNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PriceCurrency=%s,",PriceCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FeeCurrency=%s,",FeeCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClearCurrency=%s,",ClearCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrozenMoney=%s,",FrozenMoney.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrozenFee=%s,",FrozenFee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrozenMargin=%s,",FrozenMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Fee=%s,",Fee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseProfit=%s,",CloseProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Turnover=%s,",Turnover.getString().c_str());
	p+=strlen(p);
	sprintf(p,"RelatedOrderID=%s,",RelatedOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessResult=%s,",BusinessResult.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Tradable=%s,",Tradable.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettlementGroup=%s,",SettlementGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PosiDirection=%s,",PosiDirection.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradePrice=%s,",TradePrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenPrice=%s,",OpenPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerOrderID=%s,",TriggerOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SLTriggerPrice=%s,",SLTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TPTriggerPrice=%s,",TPTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyProfit=%s,",CopyProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Position=%s,",Position.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UserID=%s,",UserID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LastPriceByInsert=%s,",LastPriceByInsert.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BidPrice1ByInsert=%s,",BidPrice1ByInsert.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AskPrice1ByInsert=%s,",AskPrice1ByInsert.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Available=%s,",Available.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CreateTime=%s,",CreateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void COrderIterator::dump(FILE *output)
{
	COrder *pOrder;
	while ((pOrder=next())!=NULL)
	{
		pOrder->dump(output);
	}
}

const CWriteableInstrument *CWriteableOrder::linkInstrument() const
{
	void *target=(void *)(&pInstrument);
	*((const CWriteableInstrument **)target)=m_pMDB->m_InstrumentFactory->findByInstrumentID(ExchangeID,InstrumentID);
	return pInstrument;
}

const CWriteablePosition *CWriteableOrder::linkPosition() const
{
	void *target=(void *)(&pPosition);
	*((const CWriteablePosition **)target)=m_pMDB->m_PositionFactory->findByPositionID(PositionID);
	return pPosition;
}

const CWriteableMember *CWriteableOrder::linkMember() const
{
	void *target=(void *)(&pMember);
	*((const CWriteableMember **)target)=m_pMDB->m_MemberFactory->findByMemberID(MemberID);
	return pMember;
}

const CWriteableTriggerOrder *CWriteableOrder::linkTriggerOrder() const
{
	void *target=(void *)(&pTriggerOrder);
	*((const CWriteableTriggerOrder **)target)=m_pMDB->m_TriggerOrderFactory->findByOrderID(TriggerOrderID);
	return pTriggerOrder;
}



void CWriteablePositionOrder::writeCSVHead(FILE *output)
{
	fprintf(output,"\"MemberID\",\"TradeUnitID\",\"AccountID\",\"LocalID\",\"ExchangeID\",\"InstrumentID\",\"OrderPriceType\",\"Direction\",\"OffsetFlag\",\"Price\",\"Volume\",\"VolumeDisplay\",\"VolumeMode\",\"Cost\",\"OrderType\",\"GTDTime\",\"MinVolume\",\"BusinessType\",\"BusinessValue\",\"CloseOrderID\",\"IsCrossMargin\",\"Remark\",\"OrderID*\",\"CopyMemberID\",\"CopyOrderID\",\"Leverage\",\"CopyProfitRate\",\"APPID\",\"PositionID\",\"TriggerPrice\",\"Reserve\",\"OrderStatus\",\"DeriveSource\",\"DeriveDetail\",\"VolumeTraded\",\"VolumeRemain\",\"VolumeCancled\",\"InsertTime\",\"UpdateTime\",\"Priority\",\"TimeSortNo\",\"FrontNo\",\"PriceCurrency\",\"FeeCurrency\",\"ClearCurrency\",\"FrozenMoney\",\"FrozenFee\",\"FrozenMargin\",\"Fee\",\"CloseProfit\",\"Turnover\",\"RelatedOrderID\",\"BusinessResult\",\"BusinessNo\",\"Tradable\",\"SettlementGroup\",\"PosiDirection\",\"TradePrice\",\"OpenPrice\",\"TriggerOrderID\",\"SLTriggerPrice\",\"TPTriggerPrice\",\"CopyProfit\",\"Position\",\"UserID\",\"LastPriceByInsert\",\"BidPrice1ByInsert\",\"AskPrice1ByInsert\",\"Available\",\"CreateTime\",\"VolumeToClose\",\"VolumeClosed\",\"VolumeOnClose\"\n");
}

void CWriteablePositionOrder::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"MemberID\",\"TradeUnitID\",\"AccountID\",\"LocalID\",\"ExchangeID\",\"InstrumentID\",\"OrderPriceType\",\"Direction\",\"OffsetFlag\",\"Price\",\"Volume\",\"VolumeDisplay\",\"VolumeMode\",\"Cost\",\"OrderType\",\"GTDTime\",\"MinVolume\",\"BusinessType\",\"BusinessValue\",\"CloseOrderID\",\"IsCrossMargin\",\"Remark\",\"OrderID\",\"CopyMemberID\",\"CopyOrderID\",\"Leverage\",\"CopyProfitRate\",\"APPID\",\"PositionID\",\"TriggerPrice\",\"Reserve\",\"OrderStatus\",\"DeriveSource\",\"DeriveDetail\",\"VolumeTraded\",\"VolumeRemain\",\"VolumeCancled\",\"InsertTime\",\"UpdateTime\",\"Priority\",\"TimeSortNo\",\"FrontNo\",\"PriceCurrency\",\"FeeCurrency\",\"ClearCurrency\",\"FrozenMoney\",\"FrozenFee\",\"FrozenMargin\",\"Fee\",\"CloseProfit\",\"Turnover\",\"RelatedOrderID\",\"BusinessResult\",\"BusinessNo\",\"Tradable\",\"SettlementGroup\",\"PosiDirection\",\"TradePrice\",\"OpenPrice\",\"TriggerOrderID\",\"SLTriggerPrice\",\"TPTriggerPrice\",\"CopyProfit\",\"Position\",\"UserID\",\"LastPriceByInsert\",\"BidPrice1ByInsert\",\"AskPrice1ByInsert\",\"Available\",\"CreateTime\",\"VolumeToClose\",\"VolumeClosed\",\"VolumeOnClose\"\n");
}
	
void CWriteablePositionOrder::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	MemberID.init();
	TradeUnitID.clear();
	AccountID.clear();
	LocalID.clear();
	ExchangeID.init();
	InstrumentID.init();
	OrderPriceType.init();
	Direction.init();
	OffsetFlag.init();
	Price.clear();
	Volume.clear();
	VolumeDisplay.clear();
	VolumeMode.clear();
	Cost.clear();
	OrderType.init();
	GTDTime.clear();
	MinVolume.clear();
	BusinessType.init();
	BusinessValue.clear();
	CloseOrderID.clear();
	IsCrossMargin.clear();
	Remark.clear();
	OrderID.init();
	CopyMemberID.clear();
	CopyOrderID.clear();
	Leverage.clear();
	CopyProfitRate.clear();
	APPID.clear();
	PositionID.init();
	TriggerPrice.clear();
	Reserve.clear();
	OrderStatus.init();
	DeriveSource.init();
	DeriveDetail.clear();
	VolumeTraded.init();
	VolumeRemain.init();
	VolumeCancled.init();
	InsertTime.clear();
	UpdateTime.clear();
	Priority.init();
	TimeSortNo.init();
	FrontNo.clear();
	PriceCurrency.clear();
	FeeCurrency.clear();
	ClearCurrency.clear();
	FrozenMoney.init();
	FrozenFee.init();
	FrozenMargin.init();
	Fee.init();
	CloseProfit.init();
	Turnover.init();
	RelatedOrderID.clear();
	BusinessResult.clear();
	BusinessNo.clear();
	Tradable.setValue(1);
	SettlementGroup.init();
	PosiDirection.init();
	TradePrice.clear();
	OpenPrice.clear();
	TriggerOrderID.clear();
	SLTriggerPrice.clear();
	TPTriggerPrice.clear();
	CopyProfit.init();
	Position.clear();
	UserID.init();
	LastPriceByInsert.clear();
	BidPrice1ByInsert.clear();
	AskPrice1ByInsert.clear();
	Available.clear();
	CreateTime.clear();
	VolumeToClose.init();
	VolumeClosed.init();
	VolumeOnClose.init();
}

void CWriteablePositionOrder::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteablePositionOrder::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteablePositionOrder),input)!=sizeof(CWriteablePositionOrder))
		return 0;
	else
		return 1;
}
	
int CWriteablePositionOrder::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradeUnitID=token;
		else
			TradeUnitID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccountID=token;
		else
			AccountID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LocalID=token;
		else
			LocalID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderPriceType=token;
		else
			OrderPriceType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Direction=token;
		else
			Direction="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OffsetFlag=token;
		else
			OffsetFlag="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Price=token;
		else
			Price="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeDisplay=token;
		else
			VolumeDisplay="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeMode=token;
		else
			VolumeMode="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Cost=token;
		else
			Cost="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderType=token;
		else
			OrderType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			GTDTime=token;
		else
			GTDTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MinVolume=token;
		else
			MinVolume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessType=token;
		else
			BusinessType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessValue=token;
		else
			BusinessValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseOrderID=token;
		else
			CloseOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			IsCrossMargin=token;
		else
			IsCrossMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderID=token;
		else
			OrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyMemberID=token;
		else
			CopyMemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyOrderID=token;
		else
			CopyOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Leverage=token;
		else
			Leverage="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyProfitRate=token;
		else
			CopyProfitRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			APPID=token;
		else
			APPID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionID=token;
		else
			PositionID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerPrice=token;
		else
			TriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Reserve=token;
		else
			Reserve="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderStatus=token;
		else
			OrderStatus="";
		token=getNextToken(NULL);
		if (token!=NULL)
			DeriveSource=token;
		else
			DeriveSource="";
		token=getNextToken(NULL);
		if (token!=NULL)
			DeriveDetail=token;
		else
			DeriveDetail="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeTraded=token;
		else
			VolumeTraded="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeRemain=token;
		else
			VolumeRemain="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeCancled=token;
		else
			VolumeCancled="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Priority=token;
		else
			Priority="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TimeSortNo=token;
		else
			TimeSortNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrontNo=token;
		else
			FrontNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PriceCurrency=token;
		else
			PriceCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FeeCurrency=token;
		else
			FeeCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClearCurrency=token;
		else
			ClearCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrozenMoney=token;
		else
			FrozenMoney="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrozenFee=token;
		else
			FrozenFee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrozenMargin=token;
		else
			FrozenMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Fee=token;
		else
			Fee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseProfit=token;
		else
			CloseProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Turnover=token;
		else
			Turnover="";
		token=getNextToken(NULL);
		if (token!=NULL)
			RelatedOrderID=token;
		else
			RelatedOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessResult=token;
		else
			BusinessResult="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Tradable=token;
		else
			Tradable="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettlementGroup=token;
		else
			SettlementGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PosiDirection=token;
		else
			PosiDirection="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradePrice=token;
		else
			TradePrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenPrice=token;
		else
			OpenPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerOrderID=token;
		else
			TriggerOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SLTriggerPrice=token;
		else
			SLTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TPTriggerPrice=token;
		else
			TPTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyProfit=token;
		else
			CopyProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Position=token;
		else
			Position="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UserID=token;
		else
			UserID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LastPriceByInsert=token;
		else
			LastPriceByInsert="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BidPrice1ByInsert=token;
		else
			BidPrice1ByInsert="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AskPrice1ByInsert=token;
		else
			AskPrice1ByInsert="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Available=token;
		else
			Available="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CreateTime=token;
		else
			CreateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeToClose=token;
		else
			VolumeToClose="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeClosed=token;
		else
			VolumeClosed="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeOnClose=token;
		else
			VolumeOnClose="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"TradeUnitID"))
			{
				TradeUnitID=token;
			}
			else if (!strcmp(fieldName,"AccountID"))
			{
				AccountID=token;
			}
			else if (!strcmp(fieldName,"LocalID"))
			{
				LocalID=token;
			}
			else if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"OrderPriceType"))
			{
				OrderPriceType=token;
			}
			else if (!strcmp(fieldName,"Direction"))
			{
				Direction=token;
			}
			else if (!strcmp(fieldName,"OffsetFlag"))
			{
				OffsetFlag=token;
			}
			else if (!strcmp(fieldName,"Price"))
			{
				Price=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"VolumeDisplay"))
			{
				VolumeDisplay=token;
			}
			else if (!strcmp(fieldName,"VolumeMode"))
			{
				VolumeMode=token;
			}
			else if (!strcmp(fieldName,"Cost"))
			{
				Cost=token;
			}
			else if (!strcmp(fieldName,"OrderType"))
			{
				OrderType=token;
			}
			else if (!strcmp(fieldName,"GTDTime"))
			{
				GTDTime=token;
			}
			else if (!strcmp(fieldName,"MinVolume"))
			{
				MinVolume=token;
			}
			else if (!strcmp(fieldName,"BusinessType"))
			{
				BusinessType=token;
			}
			else if (!strcmp(fieldName,"BusinessValue"))
			{
				BusinessValue=token;
			}
			else if (!strcmp(fieldName,"CloseOrderID"))
			{
				CloseOrderID=token;
			}
			else if (!strcmp(fieldName,"IsCrossMargin"))
			{
				IsCrossMargin=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"OrderID"))
			{
				OrderID=token;
			}
			else if (!strcmp(fieldName,"CopyMemberID"))
			{
				CopyMemberID=token;
			}
			else if (!strcmp(fieldName,"CopyOrderID"))
			{
				CopyOrderID=token;
			}
			else if (!strcmp(fieldName,"Leverage"))
			{
				Leverage=token;
			}
			else if (!strcmp(fieldName,"CopyProfitRate"))
			{
				CopyProfitRate=token;
			}
			else if (!strcmp(fieldName,"APPID"))
			{
				APPID=token;
			}
			else if (!strcmp(fieldName,"PositionID"))
			{
				PositionID=token;
			}
			else if (!strcmp(fieldName,"TriggerPrice"))
			{
				TriggerPrice=token;
			}
			else if (!strcmp(fieldName,"Reserve"))
			{
				Reserve=token;
			}
			else if (!strcmp(fieldName,"OrderStatus"))
			{
				OrderStatus=token;
			}
			else if (!strcmp(fieldName,"DeriveSource"))
			{
				DeriveSource=token;
			}
			else if (!strcmp(fieldName,"DeriveDetail"))
			{
				DeriveDetail=token;
			}
			else if (!strcmp(fieldName,"VolumeTraded"))
			{
				VolumeTraded=token;
			}
			else if (!strcmp(fieldName,"VolumeRemain"))
			{
				VolumeRemain=token;
			}
			else if (!strcmp(fieldName,"VolumeCancled"))
			{
				VolumeCancled=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			else if (!strcmp(fieldName,"Priority"))
			{
				Priority=token;
			}
			else if (!strcmp(fieldName,"TimeSortNo"))
			{
				TimeSortNo=token;
			}
			else if (!strcmp(fieldName,"FrontNo"))
			{
				FrontNo=token;
			}
			else if (!strcmp(fieldName,"PriceCurrency"))
			{
				PriceCurrency=token;
			}
			else if (!strcmp(fieldName,"FeeCurrency"))
			{
				FeeCurrency=token;
			}
			else if (!strcmp(fieldName,"ClearCurrency"))
			{
				ClearCurrency=token;
			}
			else if (!strcmp(fieldName,"FrozenMoney"))
			{
				FrozenMoney=token;
			}
			else if (!strcmp(fieldName,"FrozenFee"))
			{
				FrozenFee=token;
			}
			else if (!strcmp(fieldName,"FrozenMargin"))
			{
				FrozenMargin=token;
			}
			else if (!strcmp(fieldName,"Fee"))
			{
				Fee=token;
			}
			else if (!strcmp(fieldName,"CloseProfit"))
			{
				CloseProfit=token;
			}
			else if (!strcmp(fieldName,"Turnover"))
			{
				Turnover=token;
			}
			else if (!strcmp(fieldName,"RelatedOrderID"))
			{
				RelatedOrderID=token;
			}
			else if (!strcmp(fieldName,"BusinessResult"))
			{
				BusinessResult=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			else if (!strcmp(fieldName,"Tradable"))
			{
				Tradable=token;
			}
			else if (!strcmp(fieldName,"SettlementGroup"))
			{
				SettlementGroup=token;
			}
			else if (!strcmp(fieldName,"PosiDirection"))
			{
				PosiDirection=token;
			}
			else if (!strcmp(fieldName,"TradePrice"))
			{
				TradePrice=token;
			}
			else if (!strcmp(fieldName,"OpenPrice"))
			{
				OpenPrice=token;
			}
			else if (!strcmp(fieldName,"TriggerOrderID"))
			{
				TriggerOrderID=token;
			}
			else if (!strcmp(fieldName,"SLTriggerPrice"))
			{
				SLTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"TPTriggerPrice"))
			{
				TPTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"CopyProfit"))
			{
				CopyProfit=token;
			}
			else if (!strcmp(fieldName,"Position"))
			{
				Position=token;
			}
			else if (!strcmp(fieldName,"UserID"))
			{
				UserID=token;
			}
			else if (!strcmp(fieldName,"LastPriceByInsert"))
			{
				LastPriceByInsert=token;
			}
			else if (!strcmp(fieldName,"BidPrice1ByInsert"))
			{
				BidPrice1ByInsert=token;
			}
			else if (!strcmp(fieldName,"AskPrice1ByInsert"))
			{
				AskPrice1ByInsert=token;
			}
			else if (!strcmp(fieldName,"Available"))
			{
				Available=token;
			}
			else if (!strcmp(fieldName,"CreateTime"))
			{
				CreateTime=token;
			}
			else if (!strcmp(fieldName,"VolumeToClose"))
			{
				VolumeToClose=token;
			}
			else if (!strcmp(fieldName,"VolumeClosed"))
			{
				VolumeClosed=token;
			}
			else if (!strcmp(fieldName,"VolumeOnClose"))
			{
				VolumeOnClose=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteablePositionOrder::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteablePositionOrder),output)!=sizeof(CWriteablePositionOrder))
		return 0;
	else
		return 1;
}

int CWriteablePositionOrder::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeUnitID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderPriceType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OffsetFlag.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeDisplay.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeMode.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Cost.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",GTDTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MinVolume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsCrossMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Leverage.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfitRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Reserve.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderStatus.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",DeriveSource.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",DeriveDetail.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeTraded.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeRemain.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeCancled.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Priority.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TimeSortNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrontNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClearCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMoney.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenFee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Fee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",RelatedOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessResult.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Tradable.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PosiDirection.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradePrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SLTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TPTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Position.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UserID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastPriceByInsert.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BidPrice1ByInsert.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AskPrice1ByInsert.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Available.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeToClose.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeClosed.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeOnClose.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteablePositionOrder::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeUnitID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderPriceType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OffsetFlag.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeDisplay.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeMode.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Cost.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",GTDTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MinVolume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsCrossMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Leverage.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfitRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Reserve.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderStatus.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",DeriveSource.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",DeriveDetail.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeTraded.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeRemain.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeCancled.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Priority.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TimeSortNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrontNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClearCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMoney.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenFee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrozenMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Fee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",RelatedOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessResult.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Tradable.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PosiDirection.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradePrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SLTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TPTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Position.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UserID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LastPriceByInsert.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BidPrice1ByInsert.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AskPrice1ByInsert.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Available.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeToClose.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeClosed.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeOnClose.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteablePositionOrder::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCPositionOrder%s = {\n", index_buf);
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tTradeUnitID = %s    [交易单元代码]\n",TradeUnitID.getString().c_str());	
	fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());	
	fprintf(fp,"\t\tLocalID = %s    [报单本地标识]\n",LocalID.getString().c_str());	
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tOrderPriceType = %s    [报单价格类型]\n",OrderPriceType.getString().c_str());	
	fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());	
	fprintf(fp,"\t\tOffsetFlag = %s    [开平标志]\n",OffsetFlag.getString().c_str());	
	fprintf(fp,"\t\tPrice = %s    [报单价格]\n",Price.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tVolumeDisplay = %s    [显示数量]\n",VolumeDisplay.getString().c_str());	
	fprintf(fp,"\t\tVolumeMode = %s    [数量取值方式]\n",VolumeMode.getString().c_str());	
	fprintf(fp,"\t\tCost = %s    [委托额]\n",Cost.getString().c_str());	
	fprintf(fp,"\t\tOrderType = %s    [订单类型]\n",OrderType.getString().c_str());	
	fprintf(fp,"\t\tGTDTime = %s    [GTD时间]\n",GTDTime.getString().c_str());	
	fprintf(fp,"\t\tMinVolume = %s    [最小成交量]\n",MinVolume.getString().c_str());	
	fprintf(fp,"\t\tBusinessType = %s    [业务类别]\n",BusinessType.getString().c_str());	
	fprintf(fp,"\t\tBusinessValue = %s    [业务值]\n",BusinessValue.getString().c_str());	
	fprintf(fp,"\t\tCloseOrderID = %s    [平仓指定开仓的订单号]\n",CloseOrderID.getString().c_str());	
	fprintf(fp,"\t\tIsCrossMargin = %s    [是否全仓]\n",IsCrossMargin.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tOrderID = %s    [报单系统唯一代码]\n",OrderID.getString().c_str());	
	fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());	
	fprintf(fp,"\t\tCopyOrderID = %s    [带单员报单号]\n",CopyOrderID.getString().c_str());	
	fprintf(fp,"\t\tLeverage = %s    [委托单杠杆倍数]\n",Leverage.getString().c_str());	
	fprintf(fp,"\t\tCopyProfitRate = %s    [带单分配比例]\n",CopyProfitRate.getString().c_str());	
	fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());	
	fprintf(fp,"\t\tPositionID = %s    [持仓代码]\n",PositionID.getString().c_str());	
	fprintf(fp,"\t\tTriggerPrice = %s    [触发价]\n",TriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tReserve = %s    [保留资金]\n",Reserve.getString().c_str());	
	fprintf(fp,"\t\tOrderStatus = %s    [报单状态]\n",OrderStatus.getString().c_str());	
	fprintf(fp,"\t\tDeriveSource = %s    [衍生来源]\n",DeriveSource.getString().c_str());	
	fprintf(fp,"\t\tDeriveDetail = %s    [衍生明细]\n",DeriveDetail.getString().c_str());	
	fprintf(fp,"\t\tVolumeTraded = %s    [成交数量]\n",VolumeTraded.getString().c_str());	
	fprintf(fp,"\t\tVolumeRemain = %s    [剩余数量]\n",VolumeRemain.getString().c_str());	
	fprintf(fp,"\t\tVolumeCancled = %s    [已经撤单数量]\n",VolumeCancled.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t\tPriority = %s    [优先权]\n",Priority.getString().c_str());	
	fprintf(fp,"\t\tTimeSortNo = %s    [按时间排队的序号]\n",TimeSortNo.getString().c_str());	
	fprintf(fp,"\t\tFrontNo = %s    [前置编号]\n",FrontNo.getString().c_str());	
	fprintf(fp,"\t\tPriceCurrency = %s    [计价币种]\n",PriceCurrency.getString().c_str());	
	fprintf(fp,"\t\tFeeCurrency = %s    [手续费币种]\n",FeeCurrency.getString().c_str());	
	fprintf(fp,"\t\tClearCurrency = %s    [清算币种]\n",ClearCurrency.getString().c_str());	
	fprintf(fp,"\t\tFrozenMoney = %s    [冻结资金]\n",FrozenMoney.getString().c_str());	
	fprintf(fp,"\t\tFrozenFee = %s    [冻结手续费]\n",FrozenFee.getString().c_str());	
	fprintf(fp,"\t\tFrozenMargin = %s    [冻结保证金]\n",FrozenMargin.getString().c_str());	
	fprintf(fp,"\t\tFee = %s    [手续费]\n",Fee.getString().c_str());	
	fprintf(fp,"\t\tCloseProfit = %s    [平仓盈亏]\n",CloseProfit.getString().c_str());	
	fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());	
	fprintf(fp,"\t\tRelatedOrderID = %s    [关联报单号]\n",RelatedOrderID.getString().c_str());	
	fprintf(fp,"\t\tBusinessResult = %s    [业务执行结果]\n",BusinessResult.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t\tTradable = %s    [是否可以参加交易]\n",Tradable.getString().c_str());	
	fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());	
	fprintf(fp,"\t\tPosiDirection = %s    [持仓多空方向]\n",PosiDirection.getString().c_str());	
	fprintf(fp,"\t\tTradePrice = %s    [成交均价]\n",TradePrice.getString().c_str());	
	fprintf(fp,"\t\tOpenPrice = %s    [平仓成交时的开仓均价]\n",OpenPrice.getString().c_str());	
	fprintf(fp,"\t\tTriggerOrderID = %s    [触发报单号]\n",TriggerOrderID.getString().c_str());	
	fprintf(fp,"\t\tSLTriggerPrice = %s    [开仓报单成交之后的止损触发价]\n",SLTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tTPTriggerPrice = %s    [开仓报单成交之后的止盈触发价]\n",TPTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tCopyProfit = %s    [带单盈利分配]\n",CopyProfit.getString().c_str());	
	fprintf(fp,"\t\tPosition = %s    [成交时的持仓量]\n",Position.getString().c_str());	
	fprintf(fp,"\t\tUserID = %s    [交易用户代码]\n",UserID.getString().c_str());	
	fprintf(fp,"\t\tLastPriceByInsert = %s    [插入时的最新价]\n",LastPriceByInsert.getString().c_str());	
	fprintf(fp,"\t\tBidPrice1ByInsert = %s    [插入时的买一价]\n",BidPrice1ByInsert.getString().c_str());	
	fprintf(fp,"\t\tAskPrice1ByInsert = %s    [插入时的卖一价]\n",AskPrice1ByInsert.getString().c_str());	
	fprintf(fp,"\t\tAvailable = %s    [可用资金]\n",Available.getString().c_str());	
	fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());	
	fprintf(fp,"\t\tVolumeToClose = %s    [需要平仓的数量]\n",VolumeToClose.getString().c_str());	
	fprintf(fp,"\t\tVolumeClosed = %s    [已经平仓的数量]\n",VolumeClosed.getString().c_str());	
	fprintf(fp,"\t\tVolumeOnClose = %s    [正在平仓未成交的数量]\n",VolumeOnClose.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteablePositionOrder::dumpDiff(FILE *fp, const CWriteablePositionOrder *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCPositionOrder = {\n");
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(TradeUnitID.getValue(),pOriginal->TradeUnitID.getValue())!=0)
		fprintf(fp,"\t\tTradeUnitID = %s -> %s    [交易单元代码]\n",pOriginal->TradeUnitID.getString().c_str(),TradeUnitID.getString().c_str());
	else
		fprintf(fp,"\t\tTradeUnitID = %s    [交易单元代码]\n",TradeUnitID.getString().c_str());
	if (compare(AccountID.getValue(),pOriginal->AccountID.getValue())!=0)
		fprintf(fp,"\t\tAccountID = %s -> %s    [资金账号]\n",pOriginal->AccountID.getString().c_str(),AccountID.getString().c_str());
	else
		fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());
	if (compare(LocalID.getValue(),pOriginal->LocalID.getValue())!=0)
		fprintf(fp,"\t\tLocalID = %s -> %s    [报单本地标识]\n",pOriginal->LocalID.getString().c_str(),LocalID.getString().c_str());
	else
		fprintf(fp,"\t\tLocalID = %s    [报单本地标识]\n",LocalID.getString().c_str());
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(OrderPriceType.getValue(),pOriginal->OrderPriceType.getValue())!=0)
		fprintf(fp,"\t\tOrderPriceType = %s -> %s    [报单价格类型]\n",pOriginal->OrderPriceType.getString().c_str(),OrderPriceType.getString().c_str());
	else
		fprintf(fp,"\t\tOrderPriceType = %s    [报单价格类型]\n",OrderPriceType.getString().c_str());
	if (compare(Direction.getValue(),pOriginal->Direction.getValue())!=0)
		fprintf(fp,"\t\tDirection = %s -> %s    [买卖方向]\n",pOriginal->Direction.getString().c_str(),Direction.getString().c_str());
	else
		fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());
	if (compare(OffsetFlag.getValue(),pOriginal->OffsetFlag.getValue())!=0)
		fprintf(fp,"\t\tOffsetFlag = %s -> %s    [开平标志]\n",pOriginal->OffsetFlag.getString().c_str(),OffsetFlag.getString().c_str());
	else
		fprintf(fp,"\t\tOffsetFlag = %s    [开平标志]\n",OffsetFlag.getString().c_str());
	if (compare(Price.getValue(),pOriginal->Price.getValue())!=0)
		fprintf(fp,"\t\tPrice = %s -> %s    [报单价格]\n",pOriginal->Price.getString().c_str(),Price.getString().c_str());
	else
		fprintf(fp,"\t\tPrice = %s    [报单价格]\n",Price.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [数量]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());
	if (compare(VolumeDisplay.getValue(),pOriginal->VolumeDisplay.getValue())!=0)
		fprintf(fp,"\t\tVolumeDisplay = %s -> %s    [显示数量]\n",pOriginal->VolumeDisplay.getString().c_str(),VolumeDisplay.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeDisplay = %s    [显示数量]\n",VolumeDisplay.getString().c_str());
	if (compare(VolumeMode.getValue(),pOriginal->VolumeMode.getValue())!=0)
		fprintf(fp,"\t\tVolumeMode = %s -> %s    [数量取值方式]\n",pOriginal->VolumeMode.getString().c_str(),VolumeMode.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeMode = %s    [数量取值方式]\n",VolumeMode.getString().c_str());
	if (compare(Cost.getValue(),pOriginal->Cost.getValue())!=0)
		fprintf(fp,"\t\tCost = %s -> %s    [委托额]\n",pOriginal->Cost.getString().c_str(),Cost.getString().c_str());
	else
		fprintf(fp,"\t\tCost = %s    [委托额]\n",Cost.getString().c_str());
	if (compare(OrderType.getValue(),pOriginal->OrderType.getValue())!=0)
		fprintf(fp,"\t\tOrderType = %s -> %s    [订单类型]\n",pOriginal->OrderType.getString().c_str(),OrderType.getString().c_str());
	else
		fprintf(fp,"\t\tOrderType = %s    [订单类型]\n",OrderType.getString().c_str());
	if (compare(GTDTime.getValue(),pOriginal->GTDTime.getValue())!=0)
		fprintf(fp,"\t\tGTDTime = %s -> %s    [GTD时间]\n",pOriginal->GTDTime.getString().c_str(),GTDTime.getString().c_str());
	else
		fprintf(fp,"\t\tGTDTime = %s    [GTD时间]\n",GTDTime.getString().c_str());
	if (compare(MinVolume.getValue(),pOriginal->MinVolume.getValue())!=0)
		fprintf(fp,"\t\tMinVolume = %s -> %s    [最小成交量]\n",pOriginal->MinVolume.getString().c_str(),MinVolume.getString().c_str());
	else
		fprintf(fp,"\t\tMinVolume = %s    [最小成交量]\n",MinVolume.getString().c_str());
	if (compare(BusinessType.getValue(),pOriginal->BusinessType.getValue())!=0)
		fprintf(fp,"\t\tBusinessType = %s -> %s    [业务类别]\n",pOriginal->BusinessType.getString().c_str(),BusinessType.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessType = %s    [业务类别]\n",BusinessType.getString().c_str());
	if (compare(BusinessValue.getValue(),pOriginal->BusinessValue.getValue())!=0)
		fprintf(fp,"\t\tBusinessValue = %s -> %s    [业务值]\n",pOriginal->BusinessValue.getString().c_str(),BusinessValue.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessValue = %s    [业务值]\n",BusinessValue.getString().c_str());
	if (compare(CloseOrderID.getValue(),pOriginal->CloseOrderID.getValue())!=0)
		fprintf(fp,"\t\tCloseOrderID = %s -> %s    [平仓指定开仓的订单号]\n",pOriginal->CloseOrderID.getString().c_str(),CloseOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tCloseOrderID = %s    [平仓指定开仓的订单号]\n",CloseOrderID.getString().c_str());
	if (compare(IsCrossMargin.getValue(),pOriginal->IsCrossMargin.getValue())!=0)
		fprintf(fp,"\t\tIsCrossMargin = %s -> %s    [是否全仓]\n",pOriginal->IsCrossMargin.getString().c_str(),IsCrossMargin.getString().c_str());
	else
		fprintf(fp,"\t\tIsCrossMargin = %s    [是否全仓]\n",IsCrossMargin.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(OrderID.getValue(),pOriginal->OrderID.getValue())!=0)
		fprintf(fp,"\t\tOrderID = %s -> %s    [报单系统唯一代码]\n",pOriginal->OrderID.getString().c_str(),OrderID.getString().c_str());
	else
		fprintf(fp,"\t\tOrderID = %s    [报单系统唯一代码]\n",OrderID.getString().c_str());
	if (compare(CopyMemberID.getValue(),pOriginal->CopyMemberID.getValue())!=0)
		fprintf(fp,"\t\tCopyMemberID = %s -> %s    [带单员代码]\n",pOriginal->CopyMemberID.getString().c_str(),CopyMemberID.getString().c_str());
	else
		fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());
	if (compare(CopyOrderID.getValue(),pOriginal->CopyOrderID.getValue())!=0)
		fprintf(fp,"\t\tCopyOrderID = %s -> %s    [带单员报单号]\n",pOriginal->CopyOrderID.getString().c_str(),CopyOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tCopyOrderID = %s    [带单员报单号]\n",CopyOrderID.getString().c_str());
	if (compare(Leverage.getValue(),pOriginal->Leverage.getValue())!=0)
		fprintf(fp,"\t\tLeverage = %s -> %s    [委托单杠杆倍数]\n",pOriginal->Leverage.getString().c_str(),Leverage.getString().c_str());
	else
		fprintf(fp,"\t\tLeverage = %s    [委托单杠杆倍数]\n",Leverage.getString().c_str());
	if (compare(CopyProfitRate.getValue(),pOriginal->CopyProfitRate.getValue())!=0)
		fprintf(fp,"\t\tCopyProfitRate = %s -> %s    [带单分配比例]\n",pOriginal->CopyProfitRate.getString().c_str(),CopyProfitRate.getString().c_str());
	else
		fprintf(fp,"\t\tCopyProfitRate = %s    [带单分配比例]\n",CopyProfitRate.getString().c_str());
	if (compare(APPID.getValue(),pOriginal->APPID.getValue())!=0)
		fprintf(fp,"\t\tAPPID = %s -> %s    [应用编号]\n",pOriginal->APPID.getString().c_str(),APPID.getString().c_str());
	else
		fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());
	if (compare(PositionID.getValue(),pOriginal->PositionID.getValue())!=0)
		fprintf(fp,"\t\tPositionID = %s -> %s    [持仓代码]\n",pOriginal->PositionID.getString().c_str(),PositionID.getString().c_str());
	else
		fprintf(fp,"\t\tPositionID = %s    [持仓代码]\n",PositionID.getString().c_str());
	if (compare(TriggerPrice.getValue(),pOriginal->TriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tTriggerPrice = %s -> %s    [触发价]\n",pOriginal->TriggerPrice.getString().c_str(),TriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerPrice = %s    [触发价]\n",TriggerPrice.getString().c_str());
	if (compare(Reserve.getValue(),pOriginal->Reserve.getValue())!=0)
		fprintf(fp,"\t\tReserve = %s -> %s    [保留资金]\n",pOriginal->Reserve.getString().c_str(),Reserve.getString().c_str());
	else
		fprintf(fp,"\t\tReserve = %s    [保留资金]\n",Reserve.getString().c_str());
	if (compare(OrderStatus.getValue(),pOriginal->OrderStatus.getValue())!=0)
		fprintf(fp,"\t\tOrderStatus = %s -> %s    [报单状态]\n",pOriginal->OrderStatus.getString().c_str(),OrderStatus.getString().c_str());
	else
		fprintf(fp,"\t\tOrderStatus = %s    [报单状态]\n",OrderStatus.getString().c_str());
	if (compare(DeriveSource.getValue(),pOriginal->DeriveSource.getValue())!=0)
		fprintf(fp,"\t\tDeriveSource = %s -> %s    [衍生来源]\n",pOriginal->DeriveSource.getString().c_str(),DeriveSource.getString().c_str());
	else
		fprintf(fp,"\t\tDeriveSource = %s    [衍生来源]\n",DeriveSource.getString().c_str());
	if (compare(DeriveDetail.getValue(),pOriginal->DeriveDetail.getValue())!=0)
		fprintf(fp,"\t\tDeriveDetail = %s -> %s    [衍生明细]\n",pOriginal->DeriveDetail.getString().c_str(),DeriveDetail.getString().c_str());
	else
		fprintf(fp,"\t\tDeriveDetail = %s    [衍生明细]\n",DeriveDetail.getString().c_str());
	if (compare(VolumeTraded.getValue(),pOriginal->VolumeTraded.getValue())!=0)
		fprintf(fp,"\t\tVolumeTraded = %s -> %s    [成交数量]\n",pOriginal->VolumeTraded.getString().c_str(),VolumeTraded.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeTraded = %s    [成交数量]\n",VolumeTraded.getString().c_str());
	if (compare(VolumeRemain.getValue(),pOriginal->VolumeRemain.getValue())!=0)
		fprintf(fp,"\t\tVolumeRemain = %s -> %s    [剩余数量]\n",pOriginal->VolumeRemain.getString().c_str(),VolumeRemain.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeRemain = %s    [剩余数量]\n",VolumeRemain.getString().c_str());
	if (compare(VolumeCancled.getValue(),pOriginal->VolumeCancled.getValue())!=0)
		fprintf(fp,"\t\tVolumeCancled = %s -> %s    [已经撤单数量]\n",pOriginal->VolumeCancled.getString().c_str(),VolumeCancled.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeCancled = %s    [已经撤单数量]\n",VolumeCancled.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	if (compare(Priority.getValue(),pOriginal->Priority.getValue())!=0)
		fprintf(fp,"\t\tPriority = %s -> %s    [优先权]\n",pOriginal->Priority.getString().c_str(),Priority.getString().c_str());
	else
		fprintf(fp,"\t\tPriority = %s    [优先权]\n",Priority.getString().c_str());
	if (compare(TimeSortNo.getValue(),pOriginal->TimeSortNo.getValue())!=0)
		fprintf(fp,"\t\tTimeSortNo = %s -> %s    [按时间排队的序号]\n",pOriginal->TimeSortNo.getString().c_str(),TimeSortNo.getString().c_str());
	else
		fprintf(fp,"\t\tTimeSortNo = %s    [按时间排队的序号]\n",TimeSortNo.getString().c_str());
	if (compare(FrontNo.getValue(),pOriginal->FrontNo.getValue())!=0)
		fprintf(fp,"\t\tFrontNo = %s -> %s    [前置编号]\n",pOriginal->FrontNo.getString().c_str(),FrontNo.getString().c_str());
	else
		fprintf(fp,"\t\tFrontNo = %s    [前置编号]\n",FrontNo.getString().c_str());
	if (compare(PriceCurrency.getValue(),pOriginal->PriceCurrency.getValue())!=0)
		fprintf(fp,"\t\tPriceCurrency = %s -> %s    [计价币种]\n",pOriginal->PriceCurrency.getString().c_str(),PriceCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tPriceCurrency = %s    [计价币种]\n",PriceCurrency.getString().c_str());
	if (compare(FeeCurrency.getValue(),pOriginal->FeeCurrency.getValue())!=0)
		fprintf(fp,"\t\tFeeCurrency = %s -> %s    [手续费币种]\n",pOriginal->FeeCurrency.getString().c_str(),FeeCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tFeeCurrency = %s    [手续费币种]\n",FeeCurrency.getString().c_str());
	if (compare(ClearCurrency.getValue(),pOriginal->ClearCurrency.getValue())!=0)
		fprintf(fp,"\t\tClearCurrency = %s -> %s    [清算币种]\n",pOriginal->ClearCurrency.getString().c_str(),ClearCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tClearCurrency = %s    [清算币种]\n",ClearCurrency.getString().c_str());
	if (compare(FrozenMoney.getValue(),pOriginal->FrozenMoney.getValue())!=0)
		fprintf(fp,"\t\tFrozenMoney = %s -> %s    [冻结资金]\n",pOriginal->FrozenMoney.getString().c_str(),FrozenMoney.getString().c_str());
	else
		fprintf(fp,"\t\tFrozenMoney = %s    [冻结资金]\n",FrozenMoney.getString().c_str());
	if (compare(FrozenFee.getValue(),pOriginal->FrozenFee.getValue())!=0)
		fprintf(fp,"\t\tFrozenFee = %s -> %s    [冻结手续费]\n",pOriginal->FrozenFee.getString().c_str(),FrozenFee.getString().c_str());
	else
		fprintf(fp,"\t\tFrozenFee = %s    [冻结手续费]\n",FrozenFee.getString().c_str());
	if (compare(FrozenMargin.getValue(),pOriginal->FrozenMargin.getValue())!=0)
		fprintf(fp,"\t\tFrozenMargin = %s -> %s    [冻结保证金]\n",pOriginal->FrozenMargin.getString().c_str(),FrozenMargin.getString().c_str());
	else
		fprintf(fp,"\t\tFrozenMargin = %s    [冻结保证金]\n",FrozenMargin.getString().c_str());
	if (compare(Fee.getValue(),pOriginal->Fee.getValue())!=0)
		fprintf(fp,"\t\tFee = %s -> %s    [手续费]\n",pOriginal->Fee.getString().c_str(),Fee.getString().c_str());
	else
		fprintf(fp,"\t\tFee = %s    [手续费]\n",Fee.getString().c_str());
	if (compare(CloseProfit.getValue(),pOriginal->CloseProfit.getValue())!=0)
		fprintf(fp,"\t\tCloseProfit = %s -> %s    [平仓盈亏]\n",pOriginal->CloseProfit.getString().c_str(),CloseProfit.getString().c_str());
	else
		fprintf(fp,"\t\tCloseProfit = %s    [平仓盈亏]\n",CloseProfit.getString().c_str());
	if (compare(Turnover.getValue(),pOriginal->Turnover.getValue())!=0)
		fprintf(fp,"\t\tTurnover = %s -> %s    [成交金额]\n",pOriginal->Turnover.getString().c_str(),Turnover.getString().c_str());
	else
		fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());
	if (compare(RelatedOrderID.getValue(),pOriginal->RelatedOrderID.getValue())!=0)
		fprintf(fp,"\t\tRelatedOrderID = %s -> %s    [关联报单号]\n",pOriginal->RelatedOrderID.getString().c_str(),RelatedOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tRelatedOrderID = %s    [关联报单号]\n",RelatedOrderID.getString().c_str());
	if (compare(BusinessResult.getValue(),pOriginal->BusinessResult.getValue())!=0)
		fprintf(fp,"\t\tBusinessResult = %s -> %s    [业务执行结果]\n",pOriginal->BusinessResult.getString().c_str(),BusinessResult.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessResult = %s    [业务执行结果]\n",BusinessResult.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [业务序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());
	if (compare(Tradable.getValue(),pOriginal->Tradable.getValue())!=0)
		fprintf(fp,"\t\tTradable = %s -> %s    [是否可以参加交易]\n",pOriginal->Tradable.getString().c_str(),Tradable.getString().c_str());
	else
		fprintf(fp,"\t\tTradable = %s    [是否可以参加交易]\n",Tradable.getString().c_str());
	if (compare(SettlementGroup.getValue(),pOriginal->SettlementGroup.getValue())!=0)
		fprintf(fp,"\t\tSettlementGroup = %s -> %s    [结算组编号]\n",pOriginal->SettlementGroup.getString().c_str(),SettlementGroup.getString().c_str());
	else
		fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());
	if (compare(PosiDirection.getValue(),pOriginal->PosiDirection.getValue())!=0)
		fprintf(fp,"\t\tPosiDirection = %s -> %s    [持仓多空方向]\n",pOriginal->PosiDirection.getString().c_str(),PosiDirection.getString().c_str());
	else
		fprintf(fp,"\t\tPosiDirection = %s    [持仓多空方向]\n",PosiDirection.getString().c_str());
	if (compare(TradePrice.getValue(),pOriginal->TradePrice.getValue())!=0)
		fprintf(fp,"\t\tTradePrice = %s -> %s    [成交均价]\n",pOriginal->TradePrice.getString().c_str(),TradePrice.getString().c_str());
	else
		fprintf(fp,"\t\tTradePrice = %s    [成交均价]\n",TradePrice.getString().c_str());
	if (compare(OpenPrice.getValue(),pOriginal->OpenPrice.getValue())!=0)
		fprintf(fp,"\t\tOpenPrice = %s -> %s    [平仓成交时的开仓均价]\n",pOriginal->OpenPrice.getString().c_str(),OpenPrice.getString().c_str());
	else
		fprintf(fp,"\t\tOpenPrice = %s    [平仓成交时的开仓均价]\n",OpenPrice.getString().c_str());
	if (compare(TriggerOrderID.getValue(),pOriginal->TriggerOrderID.getValue())!=0)
		fprintf(fp,"\t\tTriggerOrderID = %s -> %s    [触发报单号]\n",pOriginal->TriggerOrderID.getString().c_str(),TriggerOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerOrderID = %s    [触发报单号]\n",TriggerOrderID.getString().c_str());
	if (compare(SLTriggerPrice.getValue(),pOriginal->SLTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tSLTriggerPrice = %s -> %s    [开仓报单成交之后的止损触发价]\n",pOriginal->SLTriggerPrice.getString().c_str(),SLTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tSLTriggerPrice = %s    [开仓报单成交之后的止损触发价]\n",SLTriggerPrice.getString().c_str());
	if (compare(TPTriggerPrice.getValue(),pOriginal->TPTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tTPTriggerPrice = %s -> %s    [开仓报单成交之后的止盈触发价]\n",pOriginal->TPTriggerPrice.getString().c_str(),TPTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tTPTriggerPrice = %s    [开仓报单成交之后的止盈触发价]\n",TPTriggerPrice.getString().c_str());
	if (compare(CopyProfit.getValue(),pOriginal->CopyProfit.getValue())!=0)
		fprintf(fp,"\t\tCopyProfit = %s -> %s    [带单盈利分配]\n",pOriginal->CopyProfit.getString().c_str(),CopyProfit.getString().c_str());
	else
		fprintf(fp,"\t\tCopyProfit = %s    [带单盈利分配]\n",CopyProfit.getString().c_str());
	if (compare(Position.getValue(),pOriginal->Position.getValue())!=0)
		fprintf(fp,"\t\tPosition = %s -> %s    [成交时的持仓量]\n",pOriginal->Position.getString().c_str(),Position.getString().c_str());
	else
		fprintf(fp,"\t\tPosition = %s    [成交时的持仓量]\n",Position.getString().c_str());
	if (compare(UserID.getValue(),pOriginal->UserID.getValue())!=0)
		fprintf(fp,"\t\tUserID = %s -> %s    [交易用户代码]\n",pOriginal->UserID.getString().c_str(),UserID.getString().c_str());
	else
		fprintf(fp,"\t\tUserID = %s    [交易用户代码]\n",UserID.getString().c_str());
	if (compare(LastPriceByInsert.getValue(),pOriginal->LastPriceByInsert.getValue())!=0)
		fprintf(fp,"\t\tLastPriceByInsert = %s -> %s    [插入时的最新价]\n",pOriginal->LastPriceByInsert.getString().c_str(),LastPriceByInsert.getString().c_str());
	else
		fprintf(fp,"\t\tLastPriceByInsert = %s    [插入时的最新价]\n",LastPriceByInsert.getString().c_str());
	if (compare(BidPrice1ByInsert.getValue(),pOriginal->BidPrice1ByInsert.getValue())!=0)
		fprintf(fp,"\t\tBidPrice1ByInsert = %s -> %s    [插入时的买一价]\n",pOriginal->BidPrice1ByInsert.getString().c_str(),BidPrice1ByInsert.getString().c_str());
	else
		fprintf(fp,"\t\tBidPrice1ByInsert = %s    [插入时的买一价]\n",BidPrice1ByInsert.getString().c_str());
	if (compare(AskPrice1ByInsert.getValue(),pOriginal->AskPrice1ByInsert.getValue())!=0)
		fprintf(fp,"\t\tAskPrice1ByInsert = %s -> %s    [插入时的卖一价]\n",pOriginal->AskPrice1ByInsert.getString().c_str(),AskPrice1ByInsert.getString().c_str());
	else
		fprintf(fp,"\t\tAskPrice1ByInsert = %s    [插入时的卖一价]\n",AskPrice1ByInsert.getString().c_str());
	if (compare(Available.getValue(),pOriginal->Available.getValue())!=0)
		fprintf(fp,"\t\tAvailable = %s -> %s    [可用资金]\n",pOriginal->Available.getString().c_str(),Available.getString().c_str());
	else
		fprintf(fp,"\t\tAvailable = %s    [可用资金]\n",Available.getString().c_str());
	if (compare(CreateTime.getValue(),pOriginal->CreateTime.getValue())!=0)
		fprintf(fp,"\t\tCreateTime = %s -> %s    [创建时间]\n",pOriginal->CreateTime.getString().c_str(),CreateTime.getString().c_str());
	else
		fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());
	if (compare(VolumeToClose.getValue(),pOriginal->VolumeToClose.getValue())!=0)
		fprintf(fp,"\t\tVolumeToClose = %s -> %s    [需要平仓的数量]\n",pOriginal->VolumeToClose.getString().c_str(),VolumeToClose.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeToClose = %s    [需要平仓的数量]\n",VolumeToClose.getString().c_str());
	if (compare(VolumeClosed.getValue(),pOriginal->VolumeClosed.getValue())!=0)
		fprintf(fp,"\t\tVolumeClosed = %s -> %s    [已经平仓的数量]\n",pOriginal->VolumeClosed.getString().c_str(),VolumeClosed.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeClosed = %s    [已经平仓的数量]\n",VolumeClosed.getString().c_str());
	if (compare(VolumeOnClose.getValue(),pOriginal->VolumeOnClose.getValue())!=0)
		fprintf(fp,"\t\tVolumeOnClose = %s -> %s    [正在平仓未成交的数量]\n",pOriginal->VolumeOnClose.getString().c_str(),VolumeOnClose.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeOnClose = %s    [正在平仓未成交的数量]\n",VolumeOnClose.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteablePositionOrder::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",TradeUnitID=%s",TradeUnitID.getString().c_str());	
	fprintf(fp,",AccountID=%s",AccountID.getString().c_str());	
	fprintf(fp,",LocalID=%s",LocalID.getString().c_str());	
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",OrderPriceType=%s",OrderPriceType.getString().c_str());	
	fprintf(fp,",Direction=%s",Direction.getString().c_str());	
	fprintf(fp,",OffsetFlag=%s",OffsetFlag.getString().c_str());	
	fprintf(fp,",Price=%s",Price.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",VolumeDisplay=%s",VolumeDisplay.getString().c_str());	
	fprintf(fp,",VolumeMode=%s",VolumeMode.getString().c_str());	
	fprintf(fp,",Cost=%s",Cost.getString().c_str());	
	fprintf(fp,",OrderType=%s",OrderType.getString().c_str());	
	fprintf(fp,",GTDTime=%s",GTDTime.getString().c_str());	
	fprintf(fp,",MinVolume=%s",MinVolume.getString().c_str());	
	fprintf(fp,",BusinessType=%s",BusinessType.getString().c_str());	
	fprintf(fp,",BusinessValue=%s",BusinessValue.getString().c_str());	
	fprintf(fp,",CloseOrderID=%s",CloseOrderID.getString().c_str());	
	fprintf(fp,",IsCrossMargin=%s",IsCrossMargin.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",OrderID=%s",OrderID.getString().c_str());	
	fprintf(fp,",CopyMemberID=%s",CopyMemberID.getString().c_str());	
	fprintf(fp,",CopyOrderID=%s",CopyOrderID.getString().c_str());	
	fprintf(fp,",Leverage=%s",Leverage.getString().c_str());	
	fprintf(fp,",CopyProfitRate=%s",CopyProfitRate.getString().c_str());	
	fprintf(fp,",APPID=%s",APPID.getString().c_str());	
	fprintf(fp,",PositionID=%s",PositionID.getString().c_str());	
	fprintf(fp,",TriggerPrice=%s",TriggerPrice.getString().c_str());	
	fprintf(fp,",Reserve=%s",Reserve.getString().c_str());	
	fprintf(fp,",OrderStatus=%s",OrderStatus.getString().c_str());	
	fprintf(fp,",DeriveSource=%s",DeriveSource.getString().c_str());	
	fprintf(fp,",DeriveDetail=%s",DeriveDetail.getString().c_str());	
	fprintf(fp,",VolumeTraded=%s",VolumeTraded.getString().c_str());	
	fprintf(fp,",VolumeRemain=%s",VolumeRemain.getString().c_str());	
	fprintf(fp,",VolumeCancled=%s",VolumeCancled.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,",Priority=%s",Priority.getString().c_str());	
	fprintf(fp,",TimeSortNo=%s",TimeSortNo.getString().c_str());	
	fprintf(fp,",FrontNo=%s",FrontNo.getString().c_str());	
	fprintf(fp,",PriceCurrency=%s",PriceCurrency.getString().c_str());	
	fprintf(fp,",FeeCurrency=%s",FeeCurrency.getString().c_str());	
	fprintf(fp,",ClearCurrency=%s",ClearCurrency.getString().c_str());	
	fprintf(fp,",FrozenMoney=%s",FrozenMoney.getString().c_str());	
	fprintf(fp,",FrozenFee=%s",FrozenFee.getString().c_str());	
	fprintf(fp,",FrozenMargin=%s",FrozenMargin.getString().c_str());	
	fprintf(fp,",Fee=%s",Fee.getString().c_str());	
	fprintf(fp,",CloseProfit=%s",CloseProfit.getString().c_str());	
	fprintf(fp,",Turnover=%s",Turnover.getString().c_str());	
	fprintf(fp,",RelatedOrderID=%s",RelatedOrderID.getString().c_str());	
	fprintf(fp,",BusinessResult=%s",BusinessResult.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,",Tradable=%s",Tradable.getString().c_str());	
	fprintf(fp,",SettlementGroup=%s",SettlementGroup.getString().c_str());	
	fprintf(fp,",PosiDirection=%s",PosiDirection.getString().c_str());	
	fprintf(fp,",TradePrice=%s",TradePrice.getString().c_str());	
	fprintf(fp,",OpenPrice=%s",OpenPrice.getString().c_str());	
	fprintf(fp,",TriggerOrderID=%s",TriggerOrderID.getString().c_str());	
	fprintf(fp,",SLTriggerPrice=%s",SLTriggerPrice.getString().c_str());	
	fprintf(fp,",TPTriggerPrice=%s",TPTriggerPrice.getString().c_str());	
	fprintf(fp,",CopyProfit=%s",CopyProfit.getString().c_str());	
	fprintf(fp,",Position=%s",Position.getString().c_str());	
	fprintf(fp,",UserID=%s",UserID.getString().c_str());	
	fprintf(fp,",LastPriceByInsert=%s",LastPriceByInsert.getString().c_str());	
	fprintf(fp,",BidPrice1ByInsert=%s",BidPrice1ByInsert.getString().c_str());	
	fprintf(fp,",AskPrice1ByInsert=%s",AskPrice1ByInsert.getString().c_str());	
	fprintf(fp,",Available=%s",Available.getString().c_str());	
	fprintf(fp,",CreateTime=%s",CreateTime.getString().c_str());	
	fprintf(fp,",VolumeToClose=%s",VolumeToClose.getString().c_str());	
	fprintf(fp,",VolumeClosed=%s",VolumeClosed.getString().c_str());	
	fprintf(fp,",VolumeOnClose=%s",VolumeOnClose.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteablePositionOrder::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradeUnitID=%s,",TradeUnitID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccountID=%s,",AccountID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LocalID=%s,",LocalID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderPriceType=%s,",OrderPriceType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Direction=%s,",Direction.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OffsetFlag=%s,",OffsetFlag.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Price=%s,",Price.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeDisplay=%s,",VolumeDisplay.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeMode=%s,",VolumeMode.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Cost=%s,",Cost.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderType=%s,",OrderType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"GTDTime=%s,",GTDTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MinVolume=%s,",MinVolume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessType=%s,",BusinessType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessValue=%s,",BusinessValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseOrderID=%s,",CloseOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"IsCrossMargin=%s,",IsCrossMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderID=%s,",OrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyMemberID=%s,",CopyMemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyOrderID=%s,",CopyOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Leverage=%s,",Leverage.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyProfitRate=%s,",CopyProfitRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"APPID=%s,",APPID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionID=%s,",PositionID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerPrice=%s,",TriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Reserve=%s,",Reserve.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderStatus=%s,",OrderStatus.getString().c_str());
	p+=strlen(p);
	sprintf(p,"DeriveSource=%s,",DeriveSource.getString().c_str());
	p+=strlen(p);
	sprintf(p,"DeriveDetail=%s,",DeriveDetail.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeTraded=%s,",VolumeTraded.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeRemain=%s,",VolumeRemain.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeCancled=%s,",VolumeCancled.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Priority=%s,",Priority.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TimeSortNo=%s,",TimeSortNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrontNo=%s,",FrontNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PriceCurrency=%s,",PriceCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FeeCurrency=%s,",FeeCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClearCurrency=%s,",ClearCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrozenMoney=%s,",FrozenMoney.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrozenFee=%s,",FrozenFee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrozenMargin=%s,",FrozenMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Fee=%s,",Fee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseProfit=%s,",CloseProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Turnover=%s,",Turnover.getString().c_str());
	p+=strlen(p);
	sprintf(p,"RelatedOrderID=%s,",RelatedOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessResult=%s,",BusinessResult.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Tradable=%s,",Tradable.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettlementGroup=%s,",SettlementGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PosiDirection=%s,",PosiDirection.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradePrice=%s,",TradePrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenPrice=%s,",OpenPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerOrderID=%s,",TriggerOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SLTriggerPrice=%s,",SLTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TPTriggerPrice=%s,",TPTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyProfit=%s,",CopyProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Position=%s,",Position.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UserID=%s,",UserID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LastPriceByInsert=%s,",LastPriceByInsert.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BidPrice1ByInsert=%s,",BidPrice1ByInsert.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AskPrice1ByInsert=%s,",AskPrice1ByInsert.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Available=%s,",Available.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CreateTime=%s,",CreateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeToClose=%s,",VolumeToClose.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeClosed=%s,",VolumeClosed.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeOnClose=%s,",VolumeOnClose.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CPositionOrderIterator::dump(FILE *output)
{
	CPositionOrder *pPositionOrder;
	while ((pPositionOrder=next())!=NULL)
	{
		pPositionOrder->dump(output);
	}
}



void CWriteableTriggerOrder::writeCSVHead(FILE *output)
{
	fprintf(output,"\"MemberID\",\"TradeUnitID\",\"AccountID\",\"LocalID\",\"ExchangeID\",\"InstrumentID\",\"OrderPriceType\",\"Direction\",\"OffsetFlag\",\"Price\",\"Volume\",\"VolumeDisplay\",\"VolumeMode\",\"Cost\",\"OrderType\",\"GTDTime\",\"MinVolume\",\"BusinessType\",\"BusinessValue\",\"CloseOrderID\",\"IsCrossMargin\",\"Remark\",\"OrderID*\",\"CopyMemberID\",\"CopyOrderID\",\"Leverage\",\"CopyProfitRate\",\"APPID\",\"PositionID\",\"TriggerPrice\",\"Reserve\",\"SLPrice\",\"SLTriggerPrice\",\"TPPrice\",\"TPTriggerPrice\",\"RiskBefore\",\"TriggerOrderType\",\"TriggerDetail\",\"TriggerPriceType\",\"TriggerValue\",\"CloseSLPrice\",\"CloseSLTriggerPrice\",\"CloseTPPrice\",\"CloseTPTriggerPrice\",\"CloseOrderPriceType\",\"ClosePrice\",\"CloseTriggerPrice\",\"RelatedOrderID\",\"ActiveTime\",\"TriggerTime\",\"TimeSortNo\",\"TriggerStatus\",\"PosiDirection\",\"FrontNo\",\"ErrorNo\",\"ErrorMsg\",\"InsertTime\",\"UpdateTime\",\"BusinessNo\",\"CreateTime\"\n");
}

void CWriteableTriggerOrder::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"MemberID\",\"TradeUnitID\",\"AccountID\",\"LocalID\",\"ExchangeID\",\"InstrumentID\",\"OrderPriceType\",\"Direction\",\"OffsetFlag\",\"Price\",\"Volume\",\"VolumeDisplay\",\"VolumeMode\",\"Cost\",\"OrderType\",\"GTDTime\",\"MinVolume\",\"BusinessType\",\"BusinessValue\",\"CloseOrderID\",\"IsCrossMargin\",\"Remark\",\"OrderID\",\"CopyMemberID\",\"CopyOrderID\",\"Leverage\",\"CopyProfitRate\",\"APPID\",\"PositionID\",\"TriggerPrice\",\"Reserve\",\"SLPrice\",\"SLTriggerPrice\",\"TPPrice\",\"TPTriggerPrice\",\"RiskBefore\",\"TriggerOrderType\",\"TriggerDetail\",\"TriggerPriceType\",\"TriggerValue\",\"CloseSLPrice\",\"CloseSLTriggerPrice\",\"CloseTPPrice\",\"CloseTPTriggerPrice\",\"CloseOrderPriceType\",\"ClosePrice\",\"CloseTriggerPrice\",\"RelatedOrderID\",\"ActiveTime\",\"TriggerTime\",\"TimeSortNo\",\"TriggerStatus\",\"PosiDirection\",\"FrontNo\",\"ErrorNo\",\"ErrorMsg\",\"InsertTime\",\"UpdateTime\",\"BusinessNo\",\"CreateTime\"\n");
}
	
void CWriteableTriggerOrder::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	MemberID.init();
	TradeUnitID.clear();
	AccountID.clear();
	LocalID.clear();
	ExchangeID.init();
	InstrumentID.init();
	OrderPriceType.init();
	Direction.init();
	OffsetFlag.init();
	Price.clear();
	Volume.clear();
	VolumeDisplay.clear();
	VolumeMode.clear();
	Cost.clear();
	OrderType.init();
	GTDTime.clear();
	MinVolume.clear();
	BusinessType.init();
	BusinessValue.clear();
	CloseOrderID.clear();
	IsCrossMargin.clear();
	Remark.clear();
	OrderID.init();
	CopyMemberID.clear();
	CopyOrderID.clear();
	Leverage.clear();
	CopyProfitRate.clear();
	APPID.clear();
	PositionID.init();
	TriggerPrice.clear();
	Reserve.clear();
	SLPrice.clear();
	SLTriggerPrice.clear();
	TPPrice.clear();
	TPTriggerPrice.clear();
	RiskBefore.init();
	TriggerOrderType.init();
	TriggerDetail.clear();
	TriggerPriceType.setValue("0");
	TriggerValue.clear();
	CloseSLPrice.clear();
	CloseSLTriggerPrice.clear();
	CloseTPPrice.clear();
	CloseTPTriggerPrice.clear();
	CloseOrderPriceType.clear();
	ClosePrice.clear();
	CloseTriggerPrice.clear();
	RelatedOrderID.clear();
	ActiveTime.clear();
	TriggerTime.clear();
	TimeSortNo.init();
	TriggerStatus.init();
	PosiDirection.clear();
	FrontNo.clear();
	ErrorNo.clear();
	ErrorMsg.clear();
	InsertTime.init();
	UpdateTime.clear();
	BusinessNo.clear();
	CreateTime.clear();
}

void CWriteableTriggerOrder::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableTriggerOrder::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableTriggerOrder),input)!=sizeof(CWriteableTriggerOrder))
		return 0;
	else
		return 1;
}
	
int CWriteableTriggerOrder::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradeUnitID=token;
		else
			TradeUnitID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccountID=token;
		else
			AccountID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LocalID=token;
		else
			LocalID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderPriceType=token;
		else
			OrderPriceType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Direction=token;
		else
			Direction="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OffsetFlag=token;
		else
			OffsetFlag="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Price=token;
		else
			Price="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeDisplay=token;
		else
			VolumeDisplay="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeMode=token;
		else
			VolumeMode="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Cost=token;
		else
			Cost="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderType=token;
		else
			OrderType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			GTDTime=token;
		else
			GTDTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MinVolume=token;
		else
			MinVolume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessType=token;
		else
			BusinessType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessValue=token;
		else
			BusinessValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseOrderID=token;
		else
			CloseOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			IsCrossMargin=token;
		else
			IsCrossMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderID=token;
		else
			OrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyMemberID=token;
		else
			CopyMemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyOrderID=token;
		else
			CopyOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Leverage=token;
		else
			Leverage="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyProfitRate=token;
		else
			CopyProfitRate="";
		token=getNextToken(NULL);
		if (token!=NULL)
			APPID=token;
		else
			APPID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionID=token;
		else
			PositionID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerPrice=token;
		else
			TriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Reserve=token;
		else
			Reserve="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SLPrice=token;
		else
			SLPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SLTriggerPrice=token;
		else
			SLTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TPPrice=token;
		else
			TPPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TPTriggerPrice=token;
		else
			TPTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			RiskBefore=token;
		else
			RiskBefore="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerOrderType=token;
		else
			TriggerOrderType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerDetail=token;
		else
			TriggerDetail="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerPriceType=token;
		else
			TriggerPriceType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerValue=token;
		else
			TriggerValue="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseSLPrice=token;
		else
			CloseSLPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseSLTriggerPrice=token;
		else
			CloseSLTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseTPPrice=token;
		else
			CloseTPPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseTPTriggerPrice=token;
		else
			CloseTPTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseOrderPriceType=token;
		else
			CloseOrderPriceType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClosePrice=token;
		else
			ClosePrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseTriggerPrice=token;
		else
			CloseTriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			RelatedOrderID=token;
		else
			RelatedOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ActiveTime=token;
		else
			ActiveTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerTime=token;
		else
			TriggerTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TimeSortNo=token;
		else
			TimeSortNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerStatus=token;
		else
			TriggerStatus="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PosiDirection=token;
		else
			PosiDirection="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FrontNo=token;
		else
			FrontNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ErrorNo=token;
		else
			ErrorNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ErrorMsg=token;
		else
			ErrorMsg="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CreateTime=token;
		else
			CreateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"TradeUnitID"))
			{
				TradeUnitID=token;
			}
			else if (!strcmp(fieldName,"AccountID"))
			{
				AccountID=token;
			}
			else if (!strcmp(fieldName,"LocalID"))
			{
				LocalID=token;
			}
			else if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"OrderPriceType"))
			{
				OrderPriceType=token;
			}
			else if (!strcmp(fieldName,"Direction"))
			{
				Direction=token;
			}
			else if (!strcmp(fieldName,"OffsetFlag"))
			{
				OffsetFlag=token;
			}
			else if (!strcmp(fieldName,"Price"))
			{
				Price=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"VolumeDisplay"))
			{
				VolumeDisplay=token;
			}
			else if (!strcmp(fieldName,"VolumeMode"))
			{
				VolumeMode=token;
			}
			else if (!strcmp(fieldName,"Cost"))
			{
				Cost=token;
			}
			else if (!strcmp(fieldName,"OrderType"))
			{
				OrderType=token;
			}
			else if (!strcmp(fieldName,"GTDTime"))
			{
				GTDTime=token;
			}
			else if (!strcmp(fieldName,"MinVolume"))
			{
				MinVolume=token;
			}
			else if (!strcmp(fieldName,"BusinessType"))
			{
				BusinessType=token;
			}
			else if (!strcmp(fieldName,"BusinessValue"))
			{
				BusinessValue=token;
			}
			else if (!strcmp(fieldName,"CloseOrderID"))
			{
				CloseOrderID=token;
			}
			else if (!strcmp(fieldName,"IsCrossMargin"))
			{
				IsCrossMargin=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"OrderID"))
			{
				OrderID=token;
			}
			else if (!strcmp(fieldName,"CopyMemberID"))
			{
				CopyMemberID=token;
			}
			else if (!strcmp(fieldName,"CopyOrderID"))
			{
				CopyOrderID=token;
			}
			else if (!strcmp(fieldName,"Leverage"))
			{
				Leverage=token;
			}
			else if (!strcmp(fieldName,"CopyProfitRate"))
			{
				CopyProfitRate=token;
			}
			else if (!strcmp(fieldName,"APPID"))
			{
				APPID=token;
			}
			else if (!strcmp(fieldName,"PositionID"))
			{
				PositionID=token;
			}
			else if (!strcmp(fieldName,"TriggerPrice"))
			{
				TriggerPrice=token;
			}
			else if (!strcmp(fieldName,"Reserve"))
			{
				Reserve=token;
			}
			else if (!strcmp(fieldName,"SLPrice"))
			{
				SLPrice=token;
			}
			else if (!strcmp(fieldName,"SLTriggerPrice"))
			{
				SLTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"TPPrice"))
			{
				TPPrice=token;
			}
			else if (!strcmp(fieldName,"TPTriggerPrice"))
			{
				TPTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"RiskBefore"))
			{
				RiskBefore=token;
			}
			else if (!strcmp(fieldName,"TriggerOrderType"))
			{
				TriggerOrderType=token;
			}
			else if (!strcmp(fieldName,"TriggerDetail"))
			{
				TriggerDetail=token;
			}
			else if (!strcmp(fieldName,"TriggerPriceType"))
			{
				TriggerPriceType=token;
			}
			else if (!strcmp(fieldName,"TriggerValue"))
			{
				TriggerValue=token;
			}
			else if (!strcmp(fieldName,"CloseSLPrice"))
			{
				CloseSLPrice=token;
			}
			else if (!strcmp(fieldName,"CloseSLTriggerPrice"))
			{
				CloseSLTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"CloseTPPrice"))
			{
				CloseTPPrice=token;
			}
			else if (!strcmp(fieldName,"CloseTPTriggerPrice"))
			{
				CloseTPTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"CloseOrderPriceType"))
			{
				CloseOrderPriceType=token;
			}
			else if (!strcmp(fieldName,"ClosePrice"))
			{
				ClosePrice=token;
			}
			else if (!strcmp(fieldName,"CloseTriggerPrice"))
			{
				CloseTriggerPrice=token;
			}
			else if (!strcmp(fieldName,"RelatedOrderID"))
			{
				RelatedOrderID=token;
			}
			else if (!strcmp(fieldName,"ActiveTime"))
			{
				ActiveTime=token;
			}
			else if (!strcmp(fieldName,"TriggerTime"))
			{
				TriggerTime=token;
			}
			else if (!strcmp(fieldName,"TimeSortNo"))
			{
				TimeSortNo=token;
			}
			else if (!strcmp(fieldName,"TriggerStatus"))
			{
				TriggerStatus=token;
			}
			else if (!strcmp(fieldName,"PosiDirection"))
			{
				PosiDirection=token;
			}
			else if (!strcmp(fieldName,"FrontNo"))
			{
				FrontNo=token;
			}
			else if (!strcmp(fieldName,"ErrorNo"))
			{
				ErrorNo=token;
			}
			else if (!strcmp(fieldName,"ErrorMsg"))
			{
				ErrorMsg=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			else if (!strcmp(fieldName,"CreateTime"))
			{
				CreateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableTriggerOrder::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableTriggerOrder),output)!=sizeof(CWriteableTriggerOrder))
		return 0;
	else
		return 1;
}

int CWriteableTriggerOrder::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeUnitID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderPriceType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OffsetFlag.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeDisplay.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeMode.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Cost.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",GTDTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MinVolume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsCrossMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Leverage.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfitRate.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Reserve.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SLPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SLTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TPPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TPTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",RiskBefore.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerOrderType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerDetail.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerPriceType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerValue.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseSLPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseSLTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseTPPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseTPTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseOrderPriceType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseTriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",RelatedOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ActiveTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TimeSortNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerStatus.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PosiDirection.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrontNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ErrorNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ErrorMsg.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableTriggerOrder::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeUnitID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderPriceType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OffsetFlag.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeDisplay.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeMode.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Cost.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",GTDTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MinVolume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsCrossMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Leverage.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfitRate.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Reserve.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SLPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SLTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TPPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TPTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",RiskBefore.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerOrderType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerDetail.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerPriceType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerValue.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseSLPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseSLTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseTPPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseTPTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseOrderPriceType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseTriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",RelatedOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ActiveTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TimeSortNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerStatus.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PosiDirection.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FrontNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ErrorNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ErrorMsg.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableTriggerOrder::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCTriggerOrder%s = {\n", index_buf);
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tTradeUnitID = %s    [交易单元代码]\n",TradeUnitID.getString().c_str());	
	fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());	
	fprintf(fp,"\t\tLocalID = %s    [报单本地标识]\n",LocalID.getString().c_str());	
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tOrderPriceType = %s    [报单价格类型]\n",OrderPriceType.getString().c_str());	
	fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());	
	fprintf(fp,"\t\tOffsetFlag = %s    [开平标志]\n",OffsetFlag.getString().c_str());	
	fprintf(fp,"\t\tPrice = %s    [报单价格]\n",Price.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tVolumeDisplay = %s    [显示数量]\n",VolumeDisplay.getString().c_str());	
	fprintf(fp,"\t\tVolumeMode = %s    [数量取值方式]\n",VolumeMode.getString().c_str());	
	fprintf(fp,"\t\tCost = %s    [委托额]\n",Cost.getString().c_str());	
	fprintf(fp,"\t\tOrderType = %s    [订单类型]\n",OrderType.getString().c_str());	
	fprintf(fp,"\t\tGTDTime = %s    [GTD时间]\n",GTDTime.getString().c_str());	
	fprintf(fp,"\t\tMinVolume = %s    [最小成交量]\n",MinVolume.getString().c_str());	
	fprintf(fp,"\t\tBusinessType = %s    [业务类别]\n",BusinessType.getString().c_str());	
	fprintf(fp,"\t\tBusinessValue = %s    [业务值]\n",BusinessValue.getString().c_str());	
	fprintf(fp,"\t\tCloseOrderID = %s    [平仓指定开仓的订单号]\n",CloseOrderID.getString().c_str());	
	fprintf(fp,"\t\tIsCrossMargin = %s    [是否全仓]\n",IsCrossMargin.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tOrderID = %s    [报单系统唯一代码]\n",OrderID.getString().c_str());	
	fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());	
	fprintf(fp,"\t\tCopyOrderID = %s    [带单员报单号]\n",CopyOrderID.getString().c_str());	
	fprintf(fp,"\t\tLeverage = %s    [委托单杠杆倍数]\n",Leverage.getString().c_str());	
	fprintf(fp,"\t\tCopyProfitRate = %s    [带单分配比例]\n",CopyProfitRate.getString().c_str());	
	fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());	
	fprintf(fp,"\t\tPositionID = %s    [持仓代码]\n",PositionID.getString().c_str());	
	fprintf(fp,"\t\tTriggerPrice = %s    [触发价]\n",TriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tReserve = %s    [保留资金]\n",Reserve.getString().c_str());	
	fprintf(fp,"\t\tSLPrice = %s    [止损价]\n",SLPrice.getString().c_str());	
	fprintf(fp,"\t\tSLTriggerPrice = %s    [止损触发价]\n",SLTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tTPPrice = %s    [止盈价]\n",TPPrice.getString().c_str());	
	fprintf(fp,"\t\tTPTriggerPrice = %s    [止盈触发价]\n",TPTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tRiskBefore = %s    [是否提前检查风控]\n",RiskBefore.getString().c_str());	
	fprintf(fp,"\t\tTriggerOrderType = %s    [触发的订单类型]\n",TriggerOrderType.getString().c_str());	
	fprintf(fp,"\t\tTriggerDetail = %s    [触发类型明细]\n",TriggerDetail.getString().c_str());	
	fprintf(fp,"\t\tTriggerPriceType = %s    [触发价类型]\n",TriggerPriceType.getString().c_str());	
	fprintf(fp,"\t\tTriggerValue = %s    [触发单具体设置信息]\n",TriggerValue.getString().c_str());	
	fprintf(fp,"\t\tCloseSLPrice = %s    [平仓止损价]\n",CloseSLPrice.getString().c_str());	
	fprintf(fp,"\t\tCloseSLTriggerPrice = %s    [平仓止损触发价]\n",CloseSLTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tCloseTPPrice = %s    [平仓止盈价]\n",CloseTPPrice.getString().c_str());	
	fprintf(fp,"\t\tCloseTPTriggerPrice = %s    [平仓止盈触发价]\n",CloseTPTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tCloseOrderPriceType = %s    [报单价格类型]\n",CloseOrderPriceType.getString().c_str());	
	fprintf(fp,"\t\tClosePrice = %s    [平仓价]\n",ClosePrice.getString().c_str());	
	fprintf(fp,"\t\tCloseTriggerPrice = %s    [平仓触发价]\n",CloseTriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tRelatedOrderID = %s    [关联报单号]\n",RelatedOrderID.getString().c_str());	
	fprintf(fp,"\t\tActiveTime = %s    [激活时间]\n",ActiveTime.getString().c_str());	
	fprintf(fp,"\t\tTriggerTime = %s    [触发时间]\n",TriggerTime.getString().c_str());	
	fprintf(fp,"\t\tTimeSortNo = %s    [按时间排队的序号]\n",TimeSortNo.getString().c_str());	
	fprintf(fp,"\t\tTriggerStatus = %s    [触发报单状态]\n",TriggerStatus.getString().c_str());	
	fprintf(fp,"\t\tPosiDirection = %s    [持仓多空方向]\n",PosiDirection.getString().c_str());	
	fprintf(fp,"\t\tFrontNo = %s    [前置编号]\n",FrontNo.getString().c_str());	
	fprintf(fp,"\t\tErrorNo = %s    [错误代码]\n",ErrorNo.getString().c_str());	
	fprintf(fp,"\t\tErrorMsg = %s    [错误信息]\n",ErrorMsg.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableTriggerOrder::dumpDiff(FILE *fp, const CWriteableTriggerOrder *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCTriggerOrder = {\n");
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(TradeUnitID.getValue(),pOriginal->TradeUnitID.getValue())!=0)
		fprintf(fp,"\t\tTradeUnitID = %s -> %s    [交易单元代码]\n",pOriginal->TradeUnitID.getString().c_str(),TradeUnitID.getString().c_str());
	else
		fprintf(fp,"\t\tTradeUnitID = %s    [交易单元代码]\n",TradeUnitID.getString().c_str());
	if (compare(AccountID.getValue(),pOriginal->AccountID.getValue())!=0)
		fprintf(fp,"\t\tAccountID = %s -> %s    [资金账号]\n",pOriginal->AccountID.getString().c_str(),AccountID.getString().c_str());
	else
		fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());
	if (compare(LocalID.getValue(),pOriginal->LocalID.getValue())!=0)
		fprintf(fp,"\t\tLocalID = %s -> %s    [报单本地标识]\n",pOriginal->LocalID.getString().c_str(),LocalID.getString().c_str());
	else
		fprintf(fp,"\t\tLocalID = %s    [报单本地标识]\n",LocalID.getString().c_str());
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(OrderPriceType.getValue(),pOriginal->OrderPriceType.getValue())!=0)
		fprintf(fp,"\t\tOrderPriceType = %s -> %s    [报单价格类型]\n",pOriginal->OrderPriceType.getString().c_str(),OrderPriceType.getString().c_str());
	else
		fprintf(fp,"\t\tOrderPriceType = %s    [报单价格类型]\n",OrderPriceType.getString().c_str());
	if (compare(Direction.getValue(),pOriginal->Direction.getValue())!=0)
		fprintf(fp,"\t\tDirection = %s -> %s    [买卖方向]\n",pOriginal->Direction.getString().c_str(),Direction.getString().c_str());
	else
		fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());
	if (compare(OffsetFlag.getValue(),pOriginal->OffsetFlag.getValue())!=0)
		fprintf(fp,"\t\tOffsetFlag = %s -> %s    [开平标志]\n",pOriginal->OffsetFlag.getString().c_str(),OffsetFlag.getString().c_str());
	else
		fprintf(fp,"\t\tOffsetFlag = %s    [开平标志]\n",OffsetFlag.getString().c_str());
	if (compare(Price.getValue(),pOriginal->Price.getValue())!=0)
		fprintf(fp,"\t\tPrice = %s -> %s    [报单价格]\n",pOriginal->Price.getString().c_str(),Price.getString().c_str());
	else
		fprintf(fp,"\t\tPrice = %s    [报单价格]\n",Price.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [数量]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());
	if (compare(VolumeDisplay.getValue(),pOriginal->VolumeDisplay.getValue())!=0)
		fprintf(fp,"\t\tVolumeDisplay = %s -> %s    [显示数量]\n",pOriginal->VolumeDisplay.getString().c_str(),VolumeDisplay.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeDisplay = %s    [显示数量]\n",VolumeDisplay.getString().c_str());
	if (compare(VolumeMode.getValue(),pOriginal->VolumeMode.getValue())!=0)
		fprintf(fp,"\t\tVolumeMode = %s -> %s    [数量取值方式]\n",pOriginal->VolumeMode.getString().c_str(),VolumeMode.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeMode = %s    [数量取值方式]\n",VolumeMode.getString().c_str());
	if (compare(Cost.getValue(),pOriginal->Cost.getValue())!=0)
		fprintf(fp,"\t\tCost = %s -> %s    [委托额]\n",pOriginal->Cost.getString().c_str(),Cost.getString().c_str());
	else
		fprintf(fp,"\t\tCost = %s    [委托额]\n",Cost.getString().c_str());
	if (compare(OrderType.getValue(),pOriginal->OrderType.getValue())!=0)
		fprintf(fp,"\t\tOrderType = %s -> %s    [订单类型]\n",pOriginal->OrderType.getString().c_str(),OrderType.getString().c_str());
	else
		fprintf(fp,"\t\tOrderType = %s    [订单类型]\n",OrderType.getString().c_str());
	if (compare(GTDTime.getValue(),pOriginal->GTDTime.getValue())!=0)
		fprintf(fp,"\t\tGTDTime = %s -> %s    [GTD时间]\n",pOriginal->GTDTime.getString().c_str(),GTDTime.getString().c_str());
	else
		fprintf(fp,"\t\tGTDTime = %s    [GTD时间]\n",GTDTime.getString().c_str());
	if (compare(MinVolume.getValue(),pOriginal->MinVolume.getValue())!=0)
		fprintf(fp,"\t\tMinVolume = %s -> %s    [最小成交量]\n",pOriginal->MinVolume.getString().c_str(),MinVolume.getString().c_str());
	else
		fprintf(fp,"\t\tMinVolume = %s    [最小成交量]\n",MinVolume.getString().c_str());
	if (compare(BusinessType.getValue(),pOriginal->BusinessType.getValue())!=0)
		fprintf(fp,"\t\tBusinessType = %s -> %s    [业务类别]\n",pOriginal->BusinessType.getString().c_str(),BusinessType.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessType = %s    [业务类别]\n",BusinessType.getString().c_str());
	if (compare(BusinessValue.getValue(),pOriginal->BusinessValue.getValue())!=0)
		fprintf(fp,"\t\tBusinessValue = %s -> %s    [业务值]\n",pOriginal->BusinessValue.getString().c_str(),BusinessValue.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessValue = %s    [业务值]\n",BusinessValue.getString().c_str());
	if (compare(CloseOrderID.getValue(),pOriginal->CloseOrderID.getValue())!=0)
		fprintf(fp,"\t\tCloseOrderID = %s -> %s    [平仓指定开仓的订单号]\n",pOriginal->CloseOrderID.getString().c_str(),CloseOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tCloseOrderID = %s    [平仓指定开仓的订单号]\n",CloseOrderID.getString().c_str());
	if (compare(IsCrossMargin.getValue(),pOriginal->IsCrossMargin.getValue())!=0)
		fprintf(fp,"\t\tIsCrossMargin = %s -> %s    [是否全仓]\n",pOriginal->IsCrossMargin.getString().c_str(),IsCrossMargin.getString().c_str());
	else
		fprintf(fp,"\t\tIsCrossMargin = %s    [是否全仓]\n",IsCrossMargin.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(OrderID.getValue(),pOriginal->OrderID.getValue())!=0)
		fprintf(fp,"\t\tOrderID = %s -> %s    [报单系统唯一代码]\n",pOriginal->OrderID.getString().c_str(),OrderID.getString().c_str());
	else
		fprintf(fp,"\t\tOrderID = %s    [报单系统唯一代码]\n",OrderID.getString().c_str());
	if (compare(CopyMemberID.getValue(),pOriginal->CopyMemberID.getValue())!=0)
		fprintf(fp,"\t\tCopyMemberID = %s -> %s    [带单员代码]\n",pOriginal->CopyMemberID.getString().c_str(),CopyMemberID.getString().c_str());
	else
		fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());
	if (compare(CopyOrderID.getValue(),pOriginal->CopyOrderID.getValue())!=0)
		fprintf(fp,"\t\tCopyOrderID = %s -> %s    [带单员报单号]\n",pOriginal->CopyOrderID.getString().c_str(),CopyOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tCopyOrderID = %s    [带单员报单号]\n",CopyOrderID.getString().c_str());
	if (compare(Leverage.getValue(),pOriginal->Leverage.getValue())!=0)
		fprintf(fp,"\t\tLeverage = %s -> %s    [委托单杠杆倍数]\n",pOriginal->Leverage.getString().c_str(),Leverage.getString().c_str());
	else
		fprintf(fp,"\t\tLeverage = %s    [委托单杠杆倍数]\n",Leverage.getString().c_str());
	if (compare(CopyProfitRate.getValue(),pOriginal->CopyProfitRate.getValue())!=0)
		fprintf(fp,"\t\tCopyProfitRate = %s -> %s    [带单分配比例]\n",pOriginal->CopyProfitRate.getString().c_str(),CopyProfitRate.getString().c_str());
	else
		fprintf(fp,"\t\tCopyProfitRate = %s    [带单分配比例]\n",CopyProfitRate.getString().c_str());
	if (compare(APPID.getValue(),pOriginal->APPID.getValue())!=0)
		fprintf(fp,"\t\tAPPID = %s -> %s    [应用编号]\n",pOriginal->APPID.getString().c_str(),APPID.getString().c_str());
	else
		fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());
	if (compare(PositionID.getValue(),pOriginal->PositionID.getValue())!=0)
		fprintf(fp,"\t\tPositionID = %s -> %s    [持仓代码]\n",pOriginal->PositionID.getString().c_str(),PositionID.getString().c_str());
	else
		fprintf(fp,"\t\tPositionID = %s    [持仓代码]\n",PositionID.getString().c_str());
	if (compare(TriggerPrice.getValue(),pOriginal->TriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tTriggerPrice = %s -> %s    [触发价]\n",pOriginal->TriggerPrice.getString().c_str(),TriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerPrice = %s    [触发价]\n",TriggerPrice.getString().c_str());
	if (compare(Reserve.getValue(),pOriginal->Reserve.getValue())!=0)
		fprintf(fp,"\t\tReserve = %s -> %s    [保留资金]\n",pOriginal->Reserve.getString().c_str(),Reserve.getString().c_str());
	else
		fprintf(fp,"\t\tReserve = %s    [保留资金]\n",Reserve.getString().c_str());
	if (compare(SLPrice.getValue(),pOriginal->SLPrice.getValue())!=0)
		fprintf(fp,"\t\tSLPrice = %s -> %s    [止损价]\n",pOriginal->SLPrice.getString().c_str(),SLPrice.getString().c_str());
	else
		fprintf(fp,"\t\tSLPrice = %s    [止损价]\n",SLPrice.getString().c_str());
	if (compare(SLTriggerPrice.getValue(),pOriginal->SLTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tSLTriggerPrice = %s -> %s    [止损触发价]\n",pOriginal->SLTriggerPrice.getString().c_str(),SLTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tSLTriggerPrice = %s    [止损触发价]\n",SLTriggerPrice.getString().c_str());
	if (compare(TPPrice.getValue(),pOriginal->TPPrice.getValue())!=0)
		fprintf(fp,"\t\tTPPrice = %s -> %s    [止盈价]\n",pOriginal->TPPrice.getString().c_str(),TPPrice.getString().c_str());
	else
		fprintf(fp,"\t\tTPPrice = %s    [止盈价]\n",TPPrice.getString().c_str());
	if (compare(TPTriggerPrice.getValue(),pOriginal->TPTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tTPTriggerPrice = %s -> %s    [止盈触发价]\n",pOriginal->TPTriggerPrice.getString().c_str(),TPTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tTPTriggerPrice = %s    [止盈触发价]\n",TPTriggerPrice.getString().c_str());
	if (compare(RiskBefore.getValue(),pOriginal->RiskBefore.getValue())!=0)
		fprintf(fp,"\t\tRiskBefore = %s -> %s    [是否提前检查风控]\n",pOriginal->RiskBefore.getString().c_str(),RiskBefore.getString().c_str());
	else
		fprintf(fp,"\t\tRiskBefore = %s    [是否提前检查风控]\n",RiskBefore.getString().c_str());
	if (compare(TriggerOrderType.getValue(),pOriginal->TriggerOrderType.getValue())!=0)
		fprintf(fp,"\t\tTriggerOrderType = %s -> %s    [触发的订单类型]\n",pOriginal->TriggerOrderType.getString().c_str(),TriggerOrderType.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerOrderType = %s    [触发的订单类型]\n",TriggerOrderType.getString().c_str());
	if (compare(TriggerDetail.getValue(),pOriginal->TriggerDetail.getValue())!=0)
		fprintf(fp,"\t\tTriggerDetail = %s -> %s    [触发类型明细]\n",pOriginal->TriggerDetail.getString().c_str(),TriggerDetail.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerDetail = %s    [触发类型明细]\n",TriggerDetail.getString().c_str());
	if (compare(TriggerPriceType.getValue(),pOriginal->TriggerPriceType.getValue())!=0)
		fprintf(fp,"\t\tTriggerPriceType = %s -> %s    [触发价类型]\n",pOriginal->TriggerPriceType.getString().c_str(),TriggerPriceType.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerPriceType = %s    [触发价类型]\n",TriggerPriceType.getString().c_str());
	if (compare(TriggerValue.getValue(),pOriginal->TriggerValue.getValue())!=0)
		fprintf(fp,"\t\tTriggerValue = %s -> %s    [触发单具体设置信息]\n",pOriginal->TriggerValue.getString().c_str(),TriggerValue.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerValue = %s    [触发单具体设置信息]\n",TriggerValue.getString().c_str());
	if (compare(CloseSLPrice.getValue(),pOriginal->CloseSLPrice.getValue())!=0)
		fprintf(fp,"\t\tCloseSLPrice = %s -> %s    [平仓止损价]\n",pOriginal->CloseSLPrice.getString().c_str(),CloseSLPrice.getString().c_str());
	else
		fprintf(fp,"\t\tCloseSLPrice = %s    [平仓止损价]\n",CloseSLPrice.getString().c_str());
	if (compare(CloseSLTriggerPrice.getValue(),pOriginal->CloseSLTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tCloseSLTriggerPrice = %s -> %s    [平仓止损触发价]\n",pOriginal->CloseSLTriggerPrice.getString().c_str(),CloseSLTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tCloseSLTriggerPrice = %s    [平仓止损触发价]\n",CloseSLTriggerPrice.getString().c_str());
	if (compare(CloseTPPrice.getValue(),pOriginal->CloseTPPrice.getValue())!=0)
		fprintf(fp,"\t\tCloseTPPrice = %s -> %s    [平仓止盈价]\n",pOriginal->CloseTPPrice.getString().c_str(),CloseTPPrice.getString().c_str());
	else
		fprintf(fp,"\t\tCloseTPPrice = %s    [平仓止盈价]\n",CloseTPPrice.getString().c_str());
	if (compare(CloseTPTriggerPrice.getValue(),pOriginal->CloseTPTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tCloseTPTriggerPrice = %s -> %s    [平仓止盈触发价]\n",pOriginal->CloseTPTriggerPrice.getString().c_str(),CloseTPTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tCloseTPTriggerPrice = %s    [平仓止盈触发价]\n",CloseTPTriggerPrice.getString().c_str());
	if (compare(CloseOrderPriceType.getValue(),pOriginal->CloseOrderPriceType.getValue())!=0)
		fprintf(fp,"\t\tCloseOrderPriceType = %s -> %s    [报单价格类型]\n",pOriginal->CloseOrderPriceType.getString().c_str(),CloseOrderPriceType.getString().c_str());
	else
		fprintf(fp,"\t\tCloseOrderPriceType = %s    [报单价格类型]\n",CloseOrderPriceType.getString().c_str());
	if (compare(ClosePrice.getValue(),pOriginal->ClosePrice.getValue())!=0)
		fprintf(fp,"\t\tClosePrice = %s -> %s    [平仓价]\n",pOriginal->ClosePrice.getString().c_str(),ClosePrice.getString().c_str());
	else
		fprintf(fp,"\t\tClosePrice = %s    [平仓价]\n",ClosePrice.getString().c_str());
	if (compare(CloseTriggerPrice.getValue(),pOriginal->CloseTriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tCloseTriggerPrice = %s -> %s    [平仓触发价]\n",pOriginal->CloseTriggerPrice.getString().c_str(),CloseTriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tCloseTriggerPrice = %s    [平仓触发价]\n",CloseTriggerPrice.getString().c_str());
	if (compare(RelatedOrderID.getValue(),pOriginal->RelatedOrderID.getValue())!=0)
		fprintf(fp,"\t\tRelatedOrderID = %s -> %s    [关联报单号]\n",pOriginal->RelatedOrderID.getString().c_str(),RelatedOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tRelatedOrderID = %s    [关联报单号]\n",RelatedOrderID.getString().c_str());
	if (compare(ActiveTime.getValue(),pOriginal->ActiveTime.getValue())!=0)
		fprintf(fp,"\t\tActiveTime = %s -> %s    [激活时间]\n",pOriginal->ActiveTime.getString().c_str(),ActiveTime.getString().c_str());
	else
		fprintf(fp,"\t\tActiveTime = %s    [激活时间]\n",ActiveTime.getString().c_str());
	if (compare(TriggerTime.getValue(),pOriginal->TriggerTime.getValue())!=0)
		fprintf(fp,"\t\tTriggerTime = %s -> %s    [触发时间]\n",pOriginal->TriggerTime.getString().c_str(),TriggerTime.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerTime = %s    [触发时间]\n",TriggerTime.getString().c_str());
	if (compare(TimeSortNo.getValue(),pOriginal->TimeSortNo.getValue())!=0)
		fprintf(fp,"\t\tTimeSortNo = %s -> %s    [按时间排队的序号]\n",pOriginal->TimeSortNo.getString().c_str(),TimeSortNo.getString().c_str());
	else
		fprintf(fp,"\t\tTimeSortNo = %s    [按时间排队的序号]\n",TimeSortNo.getString().c_str());
	if (compare(TriggerStatus.getValue(),pOriginal->TriggerStatus.getValue())!=0)
		fprintf(fp,"\t\tTriggerStatus = %s -> %s    [触发报单状态]\n",pOriginal->TriggerStatus.getString().c_str(),TriggerStatus.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerStatus = %s    [触发报单状态]\n",TriggerStatus.getString().c_str());
	if (compare(PosiDirection.getValue(),pOriginal->PosiDirection.getValue())!=0)
		fprintf(fp,"\t\tPosiDirection = %s -> %s    [持仓多空方向]\n",pOriginal->PosiDirection.getString().c_str(),PosiDirection.getString().c_str());
	else
		fprintf(fp,"\t\tPosiDirection = %s    [持仓多空方向]\n",PosiDirection.getString().c_str());
	if (compare(FrontNo.getValue(),pOriginal->FrontNo.getValue())!=0)
		fprintf(fp,"\t\tFrontNo = %s -> %s    [前置编号]\n",pOriginal->FrontNo.getString().c_str(),FrontNo.getString().c_str());
	else
		fprintf(fp,"\t\tFrontNo = %s    [前置编号]\n",FrontNo.getString().c_str());
	if (compare(ErrorNo.getValue(),pOriginal->ErrorNo.getValue())!=0)
		fprintf(fp,"\t\tErrorNo = %s -> %s    [错误代码]\n",pOriginal->ErrorNo.getString().c_str(),ErrorNo.getString().c_str());
	else
		fprintf(fp,"\t\tErrorNo = %s    [错误代码]\n",ErrorNo.getString().c_str());
	if (compare(ErrorMsg.getValue(),pOriginal->ErrorMsg.getValue())!=0)
		fprintf(fp,"\t\tErrorMsg = %s -> %s    [错误信息]\n",pOriginal->ErrorMsg.getString().c_str(),ErrorMsg.getString().c_str());
	else
		fprintf(fp,"\t\tErrorMsg = %s    [错误信息]\n",ErrorMsg.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [业务序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());
	if (compare(CreateTime.getValue(),pOriginal->CreateTime.getValue())!=0)
		fprintf(fp,"\t\tCreateTime = %s -> %s    [创建时间]\n",pOriginal->CreateTime.getString().c_str(),CreateTime.getString().c_str());
	else
		fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableTriggerOrder::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",TradeUnitID=%s",TradeUnitID.getString().c_str());	
	fprintf(fp,",AccountID=%s",AccountID.getString().c_str());	
	fprintf(fp,",LocalID=%s",LocalID.getString().c_str());	
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",OrderPriceType=%s",OrderPriceType.getString().c_str());	
	fprintf(fp,",Direction=%s",Direction.getString().c_str());	
	fprintf(fp,",OffsetFlag=%s",OffsetFlag.getString().c_str());	
	fprintf(fp,",Price=%s",Price.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",VolumeDisplay=%s",VolumeDisplay.getString().c_str());	
	fprintf(fp,",VolumeMode=%s",VolumeMode.getString().c_str());	
	fprintf(fp,",Cost=%s",Cost.getString().c_str());	
	fprintf(fp,",OrderType=%s",OrderType.getString().c_str());	
	fprintf(fp,",GTDTime=%s",GTDTime.getString().c_str());	
	fprintf(fp,",MinVolume=%s",MinVolume.getString().c_str());	
	fprintf(fp,",BusinessType=%s",BusinessType.getString().c_str());	
	fprintf(fp,",BusinessValue=%s",BusinessValue.getString().c_str());	
	fprintf(fp,",CloseOrderID=%s",CloseOrderID.getString().c_str());	
	fprintf(fp,",IsCrossMargin=%s",IsCrossMargin.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",OrderID=%s",OrderID.getString().c_str());	
	fprintf(fp,",CopyMemberID=%s",CopyMemberID.getString().c_str());	
	fprintf(fp,",CopyOrderID=%s",CopyOrderID.getString().c_str());	
	fprintf(fp,",Leverage=%s",Leverage.getString().c_str());	
	fprintf(fp,",CopyProfitRate=%s",CopyProfitRate.getString().c_str());	
	fprintf(fp,",APPID=%s",APPID.getString().c_str());	
	fprintf(fp,",PositionID=%s",PositionID.getString().c_str());	
	fprintf(fp,",TriggerPrice=%s",TriggerPrice.getString().c_str());	
	fprintf(fp,",Reserve=%s",Reserve.getString().c_str());	
	fprintf(fp,",SLPrice=%s",SLPrice.getString().c_str());	
	fprintf(fp,",SLTriggerPrice=%s",SLTriggerPrice.getString().c_str());	
	fprintf(fp,",TPPrice=%s",TPPrice.getString().c_str());	
	fprintf(fp,",TPTriggerPrice=%s",TPTriggerPrice.getString().c_str());	
	fprintf(fp,",RiskBefore=%s",RiskBefore.getString().c_str());	
	fprintf(fp,",TriggerOrderType=%s",TriggerOrderType.getString().c_str());	
	fprintf(fp,",TriggerDetail=%s",TriggerDetail.getString().c_str());	
	fprintf(fp,",TriggerPriceType=%s",TriggerPriceType.getString().c_str());	
	fprintf(fp,",TriggerValue=%s",TriggerValue.getString().c_str());	
	fprintf(fp,",CloseSLPrice=%s",CloseSLPrice.getString().c_str());	
	fprintf(fp,",CloseSLTriggerPrice=%s",CloseSLTriggerPrice.getString().c_str());	
	fprintf(fp,",CloseTPPrice=%s",CloseTPPrice.getString().c_str());	
	fprintf(fp,",CloseTPTriggerPrice=%s",CloseTPTriggerPrice.getString().c_str());	
	fprintf(fp,",CloseOrderPriceType=%s",CloseOrderPriceType.getString().c_str());	
	fprintf(fp,",ClosePrice=%s",ClosePrice.getString().c_str());	
	fprintf(fp,",CloseTriggerPrice=%s",CloseTriggerPrice.getString().c_str());	
	fprintf(fp,",RelatedOrderID=%s",RelatedOrderID.getString().c_str());	
	fprintf(fp,",ActiveTime=%s",ActiveTime.getString().c_str());	
	fprintf(fp,",TriggerTime=%s",TriggerTime.getString().c_str());	
	fprintf(fp,",TimeSortNo=%s",TimeSortNo.getString().c_str());	
	fprintf(fp,",TriggerStatus=%s",TriggerStatus.getString().c_str());	
	fprintf(fp,",PosiDirection=%s",PosiDirection.getString().c_str());	
	fprintf(fp,",FrontNo=%s",FrontNo.getString().c_str());	
	fprintf(fp,",ErrorNo=%s",ErrorNo.getString().c_str());	
	fprintf(fp,",ErrorMsg=%s",ErrorMsg.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,",CreateTime=%s",CreateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableTriggerOrder::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradeUnitID=%s,",TradeUnitID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccountID=%s,",AccountID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LocalID=%s,",LocalID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderPriceType=%s,",OrderPriceType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Direction=%s,",Direction.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OffsetFlag=%s,",OffsetFlag.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Price=%s,",Price.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeDisplay=%s,",VolumeDisplay.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeMode=%s,",VolumeMode.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Cost=%s,",Cost.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderType=%s,",OrderType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"GTDTime=%s,",GTDTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MinVolume=%s,",MinVolume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessType=%s,",BusinessType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessValue=%s,",BusinessValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseOrderID=%s,",CloseOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"IsCrossMargin=%s,",IsCrossMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderID=%s,",OrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyMemberID=%s,",CopyMemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyOrderID=%s,",CopyOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Leverage=%s,",Leverage.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyProfitRate=%s,",CopyProfitRate.getString().c_str());
	p+=strlen(p);
	sprintf(p,"APPID=%s,",APPID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionID=%s,",PositionID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerPrice=%s,",TriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Reserve=%s,",Reserve.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SLPrice=%s,",SLPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SLTriggerPrice=%s,",SLTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TPPrice=%s,",TPPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TPTriggerPrice=%s,",TPTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"RiskBefore=%s,",RiskBefore.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerOrderType=%s,",TriggerOrderType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerDetail=%s,",TriggerDetail.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerPriceType=%s,",TriggerPriceType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerValue=%s,",TriggerValue.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseSLPrice=%s,",CloseSLPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseSLTriggerPrice=%s,",CloseSLTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseTPPrice=%s,",CloseTPPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseTPTriggerPrice=%s,",CloseTPTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseOrderPriceType=%s,",CloseOrderPriceType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClosePrice=%s,",ClosePrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseTriggerPrice=%s,",CloseTriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"RelatedOrderID=%s,",RelatedOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ActiveTime=%s,",ActiveTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerTime=%s,",TriggerTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TimeSortNo=%s,",TimeSortNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerStatus=%s,",TriggerStatus.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PosiDirection=%s,",PosiDirection.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FrontNo=%s,",FrontNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ErrorNo=%s,",ErrorNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ErrorMsg=%s,",ErrorMsg.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CreateTime=%s,",CreateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CTriggerOrderIterator::dump(FILE *output)
{
	CTriggerOrder *pTriggerOrder;
	while ((pTriggerOrder=next())!=NULL)
	{
		pTriggerOrder->dump(output);
	}
}

const CWriteableOrder *CWriteableTriggerOrder::linkOrder() const
{
	void *target=(void *)(&pOrder);
	*((const CWriteableOrder **)target)=m_pMDB->m_OrderFactory->findByOrderID(RelatedOrderID);
	return pOrder;
}

const CWriteableMember *CWriteableTriggerOrder::linkMember() const
{
	void *target=(void *)(&pMember);
	*((const CWriteableMember **)target)=m_pMDB->m_MemberFactory->findByMemberID(MemberID);
	return pMember;
}

const CWriteableInstrument *CWriteableTriggerOrder::linkInstrument() const
{
	void *target=(void *)(&pInstrument);
	*((const CWriteableInstrument **)target)=m_pMDB->m_InstrumentFactory->findByInstrumentID(ExchangeID,InstrumentID);
	return pInstrument;
}

const CWriteableMarketData *CWriteableTriggerOrder::linkMarketData() const
{
	void *target=(void *)(&pMarketData);
	*((const CWriteableMarketData **)target)=m_pMDB->m_MarketDataFactory->findByInstrumentID(ExchangeID,InstrumentID);
	return pMarketData;
}



void CWriteableMarketOrder::writeCSVHead(FILE *output)
{
	fprintf(output,"\"ExchangeID*\",\"InstrumentID*\",\"Direction\",\"Price*\",\"Volume\",\"Orders\",\"BusinessNo\",\"TrueVolume\"\n");
}

void CWriteableMarketOrder::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"ExchangeID\",\"InstrumentID\",\"Direction\",\"Price\",\"Volume\",\"Orders\",\"BusinessNo\",\"TrueVolume\"\n");
}
	
void CWriteableMarketOrder::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	ExchangeID.init();
	InstrumentID.init();
	Direction.init();
	Price.init();
	Volume.init();
	Orders.init();
	BusinessNo.clear();
	TrueVolume.clear();
}

void CWriteableMarketOrder::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableMarketOrder::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableMarketOrder),input)!=sizeof(CWriteableMarketOrder))
		return 0;
	else
		return 1;
}
	
int CWriteableMarketOrder::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Direction=token;
		else
			Direction="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Price=token;
		else
			Price="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Orders=token;
		else
			Orders="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TrueVolume=token;
		else
			TrueVolume="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"Direction"))
			{
				Direction=token;
			}
			else if (!strcmp(fieldName,"Price"))
			{
				Price=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"Orders"))
			{
				Orders=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			else if (!strcmp(fieldName,"TrueVolume"))
			{
				TrueVolume=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableMarketOrder::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableMarketOrder),output)!=sizeof(CWriteableMarketOrder))
		return 0;
	else
		return 1;
}

int CWriteableMarketOrder::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Orders.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TrueVolume.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableMarketOrder::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Orders.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TrueVolume.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableMarketOrder::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCMarketOrder%s = {\n", index_buf);
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());	
	fprintf(fp,"\t\tPrice = %s    [价格]\n",Price.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tOrders = %s    [订单数量]\n",Orders.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [最后变化序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t\tTrueVolume = %s    [真实报单数量]\n",TrueVolume.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableMarketOrder::dumpDiff(FILE *fp, const CWriteableMarketOrder *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCMarketOrder = {\n");
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(Direction.getValue(),pOriginal->Direction.getValue())!=0)
		fprintf(fp,"\t\tDirection = %s -> %s    [买卖方向]\n",pOriginal->Direction.getString().c_str(),Direction.getString().c_str());
	else
		fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());
	if (compare(Price.getValue(),pOriginal->Price.getValue())!=0)
		fprintf(fp,"\t\tPrice = %s -> %s    [价格]\n",pOriginal->Price.getString().c_str(),Price.getString().c_str());
	else
		fprintf(fp,"\t\tPrice = %s    [价格]\n",Price.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [数量]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());
	if (compare(Orders.getValue(),pOriginal->Orders.getValue())!=0)
		fprintf(fp,"\t\tOrders = %s -> %s    [订单数量]\n",pOriginal->Orders.getString().c_str(),Orders.getString().c_str());
	else
		fprintf(fp,"\t\tOrders = %s    [订单数量]\n",Orders.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [最后变化序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [最后变化序列号]\n",BusinessNo.getString().c_str());
	if (compare(TrueVolume.getValue(),pOriginal->TrueVolume.getValue())!=0)
		fprintf(fp,"\t\tTrueVolume = %s -> %s    [真实报单数量]\n",pOriginal->TrueVolume.getString().c_str(),TrueVolume.getString().c_str());
	else
		fprintf(fp,"\t\tTrueVolume = %s    [真实报单数量]\n",TrueVolume.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableMarketOrder::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",Direction=%s",Direction.getString().c_str());	
	fprintf(fp,",Price=%s",Price.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",Orders=%s",Orders.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,",TrueVolume=%s",TrueVolume.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableMarketOrder::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Direction=%s,",Direction.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Price=%s,",Price.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Orders=%s,",Orders.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TrueVolume=%s,",TrueVolume.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CMarketOrderIterator::dump(FILE *output)
{
	CMarketOrder *pMarketOrder;
	while ((pMarketOrder=next())!=NULL)
	{
		pMarketOrder->dump(output);
	}
}



void CWriteableTickMarketOrder::writeCSVHead(FILE *output)
{
	fprintf(output,"\"ExchangeID\",\"InstrumentID\",\"Direction\",\"Price\",\"Volume\",\"Orders\",\"BusinessNo\",\"TrueVolume\",\"Tick\",\"SubIndex\"\n");
}

void CWriteableTickMarketOrder::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"ExchangeID\",\"InstrumentID\",\"Direction\",\"Price\",\"Volume\",\"Orders\",\"BusinessNo\",\"TrueVolume\",\"Tick\",\"SubIndex\"\n");
}
	
void CWriteableTickMarketOrder::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	ExchangeID.init();
	InstrumentID.init();
	Direction.init();
	Price.init();
	Volume.init();
	Orders.init();
	BusinessNo.clear();
	TrueVolume.clear();
	Tick.init();
	SubIndex.clear();
}

void CWriteableTickMarketOrder::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableTickMarketOrder::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableTickMarketOrder),input)!=sizeof(CWriteableTickMarketOrder))
		return 0;
	else
		return 1;
}
	
int CWriteableTickMarketOrder::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Direction=token;
		else
			Direction="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Price=token;
		else
			Price="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Orders=token;
		else
			Orders="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TrueVolume=token;
		else
			TrueVolume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Tick=token;
		else
			Tick="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SubIndex=token;
		else
			SubIndex="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"Direction"))
			{
				Direction=token;
			}
			else if (!strcmp(fieldName,"Price"))
			{
				Price=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"Orders"))
			{
				Orders=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			else if (!strcmp(fieldName,"TrueVolume"))
			{
				TrueVolume=token;
			}
			else if (!strcmp(fieldName,"Tick"))
			{
				Tick=token;
			}
			else if (!strcmp(fieldName,"SubIndex"))
			{
				SubIndex=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableTickMarketOrder::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableTickMarketOrder),output)!=sizeof(CWriteableTickMarketOrder))
		return 0;
	else
		return 1;
}

int CWriteableTickMarketOrder::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Orders.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TrueVolume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Tick.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SubIndex.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableTickMarketOrder::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Orders.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TrueVolume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Tick.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SubIndex.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableTickMarketOrder::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCTickMarketOrder%s = {\n", index_buf);
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());	
	fprintf(fp,"\t\tPrice = %s    [价格]\n",Price.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tOrders = %s    [订单数量]\n",Orders.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [最后变化序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t\tTrueVolume = %s    [真实报单数量]\n",TrueVolume.getString().c_str());	
	fprintf(fp,"\t\tTick = %s    [最小变动价位]\n",Tick.getString().c_str());	
	fprintf(fp,"\t\tSubIndex = %s    [索引]\n",SubIndex.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableTickMarketOrder::dumpDiff(FILE *fp, const CWriteableTickMarketOrder *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCTickMarketOrder = {\n");
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(Direction.getValue(),pOriginal->Direction.getValue())!=0)
		fprintf(fp,"\t\tDirection = %s -> %s    [买卖方向]\n",pOriginal->Direction.getString().c_str(),Direction.getString().c_str());
	else
		fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());
	if (compare(Price.getValue(),pOriginal->Price.getValue())!=0)
		fprintf(fp,"\t\tPrice = %s -> %s    [价格]\n",pOriginal->Price.getString().c_str(),Price.getString().c_str());
	else
		fprintf(fp,"\t\tPrice = %s    [价格]\n",Price.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [数量]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());
	if (compare(Orders.getValue(),pOriginal->Orders.getValue())!=0)
		fprintf(fp,"\t\tOrders = %s -> %s    [订单数量]\n",pOriginal->Orders.getString().c_str(),Orders.getString().c_str());
	else
		fprintf(fp,"\t\tOrders = %s    [订单数量]\n",Orders.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [最后变化序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [最后变化序列号]\n",BusinessNo.getString().c_str());
	if (compare(TrueVolume.getValue(),pOriginal->TrueVolume.getValue())!=0)
		fprintf(fp,"\t\tTrueVolume = %s -> %s    [真实报单数量]\n",pOriginal->TrueVolume.getString().c_str(),TrueVolume.getString().c_str());
	else
		fprintf(fp,"\t\tTrueVolume = %s    [真实报单数量]\n",TrueVolume.getString().c_str());
	if (compare(Tick.getValue(),pOriginal->Tick.getValue())!=0)
		fprintf(fp,"\t\tTick = %s -> %s    [最小变动价位]\n",pOriginal->Tick.getString().c_str(),Tick.getString().c_str());
	else
		fprintf(fp,"\t\tTick = %s    [最小变动价位]\n",Tick.getString().c_str());
	if (compare(SubIndex.getValue(),pOriginal->SubIndex.getValue())!=0)
		fprintf(fp,"\t\tSubIndex = %s -> %s    [索引]\n",pOriginal->SubIndex.getString().c_str(),SubIndex.getString().c_str());
	else
		fprintf(fp,"\t\tSubIndex = %s    [索引]\n",SubIndex.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableTickMarketOrder::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",Direction=%s",Direction.getString().c_str());	
	fprintf(fp,",Price=%s",Price.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",Orders=%s",Orders.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,",TrueVolume=%s",TrueVolume.getString().c_str());	
	fprintf(fp,",Tick=%s",Tick.getString().c_str());	
	fprintf(fp,",SubIndex=%s",SubIndex.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableTickMarketOrder::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Direction=%s,",Direction.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Price=%s,",Price.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Orders=%s,",Orders.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TrueVolume=%s,",TrueVolume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Tick=%s,",Tick.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SubIndex=%s,",SubIndex.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CTickMarketOrderIterator::dump(FILE *output)
{
	CTickMarketOrder *pTickMarketOrder;
	while ((pTickMarketOrder=next())!=NULL)
	{
		pTickMarketOrder->dump(output);
	}
}



void CWriteableTrade::writeCSVHead(FILE *output)
{
	fprintf(output,"\"TradeID\",\"Direction\",\"OrderID\",\"MemberID\",\"PositionID\",\"AccountID\",\"ExchangeID\",\"InstrumentID\",\"OffsetFlag\",\"Price\",\"Volume\",\"DeriveSource\",\"MatchRole\",\"PriceCurrency\",\"ClearCurrency\",\"Fee\",\"FeeCurrency\",\"CloseProfit\",\"Turnover\",\"UseMargin\",\"Leverage\",\"OrderPrice\",\"TriggerPrice\",\"IsSelfTrade\",\"Remark\",\"BusinessNo\",\"OpenPrice\",\"APPID\",\"InsertTime\",\"CreateTime\",\"CopyOrderID\",\"CopyMemberID\",\"CopyProfit\",\"Position\",\"ReserveProfit\",\"ReserveFee\",\"TradeRemark\",\"BusinessType\",\"BusinessValue\"\n");
}

void CWriteableTrade::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"TradeID\",\"Direction\",\"OrderID\",\"MemberID\",\"PositionID\",\"AccountID\",\"ExchangeID\",\"InstrumentID\",\"OffsetFlag\",\"Price\",\"Volume\",\"DeriveSource\",\"MatchRole\",\"PriceCurrency\",\"ClearCurrency\",\"Fee\",\"FeeCurrency\",\"CloseProfit\",\"Turnover\",\"UseMargin\",\"Leverage\",\"OrderPrice\",\"TriggerPrice\",\"IsSelfTrade\",\"Remark\",\"BusinessNo\",\"OpenPrice\",\"APPID\",\"InsertTime\",\"CreateTime\",\"CopyOrderID\",\"CopyMemberID\",\"CopyProfit\",\"Position\",\"ReserveProfit\",\"ReserveFee\",\"TradeRemark\",\"BusinessType\",\"BusinessValue\"\n");
}
	
void CWriteableTrade::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	TradeID.init();
	Direction.init();
	OrderID.clear();
	MemberID.init();
	PositionID.clear();
	AccountID.init();
	ExchangeID.init();
	InstrumentID.init();
	OffsetFlag.init();
	Price.init();
	Volume.init();
	DeriveSource.init();
	MatchRole.init();
	PriceCurrency.clear();
	ClearCurrency.clear();
	Fee.init();
	FeeCurrency.clear();
	CloseProfit.init();
	Turnover.init();
	UseMargin.init();
	Leverage.clear();
	OrderPrice.init();
	TriggerPrice.clear();
	IsSelfTrade.init();
	Remark.clear();
	BusinessNo.clear();
	OpenPrice.clear();
	APPID.clear();
	InsertTime.clear();
	CreateTime.clear();
	CopyOrderID.clear();
	CopyMemberID.clear();
	CopyProfit.init();
	Position.clear();
	ReserveProfit.clear();
	ReserveFee.clear();
	TradeRemark.clear();
	BusinessType.clear();
	BusinessValue.clear();
}

void CWriteableTrade::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableTrade::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableTrade),input)!=sizeof(CWriteableTrade))
		return 0;
	else
		return 1;
}
	
int CWriteableTrade::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			TradeID=token;
		else
			TradeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Direction=token;
		else
			Direction="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderID=token;
		else
			OrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PositionID=token;
		else
			PositionID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccountID=token;
		else
			AccountID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OffsetFlag=token;
		else
			OffsetFlag="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Price=token;
		else
			Price="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			DeriveSource=token;
		else
			DeriveSource="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MatchRole=token;
		else
			MatchRole="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PriceCurrency=token;
		else
			PriceCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClearCurrency=token;
		else
			ClearCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Fee=token;
		else
			Fee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FeeCurrency=token;
		else
			FeeCurrency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CloseProfit=token;
		else
			CloseProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Turnover=token;
		else
			Turnover="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UseMargin=token;
		else
			UseMargin="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Leverage=token;
		else
			Leverage="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderPrice=token;
		else
			OrderPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TriggerPrice=token;
		else
			TriggerPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			IsSelfTrade=token;
		else
			IsSelfTrade="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenPrice=token;
		else
			OpenPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			APPID=token;
		else
			APPID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CreateTime=token;
		else
			CreateTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyOrderID=token;
		else
			CopyOrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyMemberID=token;
		else
			CopyMemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CopyProfit=token;
		else
			CopyProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Position=token;
		else
			Position="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ReserveProfit=token;
		else
			ReserveProfit="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ReserveFee=token;
		else
			ReserveFee="";
		token=getNextToken(NULL);
		if (token!=NULL)
			TradeRemark=token;
		else
			TradeRemark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessType=token;
		else
			BusinessType="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessValue=token;
		else
			BusinessValue="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"TradeID"))
			{
				TradeID=token;
			}
			else if (!strcmp(fieldName,"Direction"))
			{
				Direction=token;
			}
			else if (!strcmp(fieldName,"OrderID"))
			{
				OrderID=token;
			}
			else if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"PositionID"))
			{
				PositionID=token;
			}
			else if (!strcmp(fieldName,"AccountID"))
			{
				AccountID=token;
			}
			else if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"OffsetFlag"))
			{
				OffsetFlag=token;
			}
			else if (!strcmp(fieldName,"Price"))
			{
				Price=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"DeriveSource"))
			{
				DeriveSource=token;
			}
			else if (!strcmp(fieldName,"MatchRole"))
			{
				MatchRole=token;
			}
			else if (!strcmp(fieldName,"PriceCurrency"))
			{
				PriceCurrency=token;
			}
			else if (!strcmp(fieldName,"ClearCurrency"))
			{
				ClearCurrency=token;
			}
			else if (!strcmp(fieldName,"Fee"))
			{
				Fee=token;
			}
			else if (!strcmp(fieldName,"FeeCurrency"))
			{
				FeeCurrency=token;
			}
			else if (!strcmp(fieldName,"CloseProfit"))
			{
				CloseProfit=token;
			}
			else if (!strcmp(fieldName,"Turnover"))
			{
				Turnover=token;
			}
			else if (!strcmp(fieldName,"UseMargin"))
			{
				UseMargin=token;
			}
			else if (!strcmp(fieldName,"Leverage"))
			{
				Leverage=token;
			}
			else if (!strcmp(fieldName,"OrderPrice"))
			{
				OrderPrice=token;
			}
			else if (!strcmp(fieldName,"TriggerPrice"))
			{
				TriggerPrice=token;
			}
			else if (!strcmp(fieldName,"IsSelfTrade"))
			{
				IsSelfTrade=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			else if (!strcmp(fieldName,"OpenPrice"))
			{
				OpenPrice=token;
			}
			else if (!strcmp(fieldName,"APPID"))
			{
				APPID=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"CreateTime"))
			{
				CreateTime=token;
			}
			else if (!strcmp(fieldName,"CopyOrderID"))
			{
				CopyOrderID=token;
			}
			else if (!strcmp(fieldName,"CopyMemberID"))
			{
				CopyMemberID=token;
			}
			else if (!strcmp(fieldName,"CopyProfit"))
			{
				CopyProfit=token;
			}
			else if (!strcmp(fieldName,"Position"))
			{
				Position=token;
			}
			else if (!strcmp(fieldName,"ReserveProfit"))
			{
				ReserveProfit=token;
			}
			else if (!strcmp(fieldName,"ReserveFee"))
			{
				ReserveFee=token;
			}
			else if (!strcmp(fieldName,"TradeRemark"))
			{
				TradeRemark=token;
			}
			else if (!strcmp(fieldName,"BusinessType"))
			{
				BusinessType=token;
			}
			else if (!strcmp(fieldName,"BusinessValue"))
			{
				BusinessValue=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableTrade::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableTrade),output)!=sizeof(CWriteableTrade))
		return 0;
	else
		return 1;
}

int CWriteableTrade::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",TradeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OffsetFlag.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",DeriveSource.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MatchRole.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClearCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Fee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeCurrency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UseMargin.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Leverage.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsSelfTrade.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyOrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Position.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveProfit.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveFee.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeRemark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessType.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessValue.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableTrade::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",TradeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PositionID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OffsetFlag.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",DeriveSource.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MatchRole.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PriceCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClearCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Fee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FeeCurrency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CloseProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UseMargin.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Leverage.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TriggerPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",IsSelfTrade.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyOrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyMemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CopyProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Position.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveProfit.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveFee.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",TradeRemark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessType.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessValue.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableTrade::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCTrade%s = {\n", index_buf);
	fprintf(fp,"\t\tTradeID = %s    [成交代码]\n",TradeID.getString().c_str());	
	fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());	
	fprintf(fp,"\t\tOrderID = %s    [报单系统唯一代码]\n",OrderID.getString().c_str());	
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tPositionID = %s    [持仓代码]\n",PositionID.getString().c_str());	
	fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());	
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tOffsetFlag = %s    [开平标志]\n",OffsetFlag.getString().c_str());	
	fprintf(fp,"\t\tPrice = %s    [成交价格]\n",Price.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [成交数量]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tDeriveSource = %s    [衍生类型]\n",DeriveSource.getString().c_str());	
	fprintf(fp,"\t\tMatchRole = %s    [成交角色]\n",MatchRole.getString().c_str());	
	fprintf(fp,"\t\tPriceCurrency = %s    [计价币种]\n",PriceCurrency.getString().c_str());	
	fprintf(fp,"\t\tClearCurrency = %s    [清算币种]\n",ClearCurrency.getString().c_str());	
	fprintf(fp,"\t\tFee = %s    [手续费]\n",Fee.getString().c_str());	
	fprintf(fp,"\t\tFeeCurrency = %s    [手续费币种]\n",FeeCurrency.getString().c_str());	
	fprintf(fp,"\t\tCloseProfit = %s    [平仓盈亏]\n",CloseProfit.getString().c_str());	
	fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());	
	fprintf(fp,"\t\tUseMargin = %s    [占用或者减少的保证金]\n",UseMargin.getString().c_str());	
	fprintf(fp,"\t\tLeverage = %s    [杠杆倍数]\n",Leverage.getString().c_str());	
	fprintf(fp,"\t\tOrderPrice = %s    [委托价格]\n",OrderPrice.getString().c_str());	
	fprintf(fp,"\t\tTriggerPrice = %s    [触发价]\n",TriggerPrice.getString().c_str());	
	fprintf(fp,"\t\tIsSelfTrade = %s    [是否自成交]\n",IsSelfTrade.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t\tOpenPrice = %s    [开仓均价]\n",OpenPrice.getString().c_str());	
	fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());	
	fprintf(fp,"\t\tCopyOrderID = %s    [带单员报单号]\n",CopyOrderID.getString().c_str());	
	fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());	
	fprintf(fp,"\t\tCopyProfit = %s    [带单盈利分配]\n",CopyProfit.getString().c_str());	
	fprintf(fp,"\t\tPosition = %s    [成交时的持仓量]\n",Position.getString().c_str());	
	fprintf(fp,"\t\tReserveProfit = %s    [保留资金盈亏]\n",ReserveProfit.getString().c_str());	
	fprintf(fp,"\t\tReserveFee = %s    [保留资金手续费]\n",ReserveFee.getString().c_str());	
	fprintf(fp,"\t\tTradeRemark = %s    [Trade备注]\n",TradeRemark.getString().c_str());	
	fprintf(fp,"\t\tBusinessType = %s    [业务类别]\n",BusinessType.getString().c_str());	
	fprintf(fp,"\t\tBusinessValue = %s    [业务值]\n",BusinessValue.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableTrade::dumpDiff(FILE *fp, const CWriteableTrade *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCTrade = {\n");
	if (compare(TradeID.getValue(),pOriginal->TradeID.getValue())!=0)
		fprintf(fp,"\t\tTradeID = %s -> %s    [成交代码]\n",pOriginal->TradeID.getString().c_str(),TradeID.getString().c_str());
	else
		fprintf(fp,"\t\tTradeID = %s    [成交代码]\n",TradeID.getString().c_str());
	if (compare(Direction.getValue(),pOriginal->Direction.getValue())!=0)
		fprintf(fp,"\t\tDirection = %s -> %s    [买卖方向]\n",pOriginal->Direction.getString().c_str(),Direction.getString().c_str());
	else
		fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());
	if (compare(OrderID.getValue(),pOriginal->OrderID.getValue())!=0)
		fprintf(fp,"\t\tOrderID = %s -> %s    [报单系统唯一代码]\n",pOriginal->OrderID.getString().c_str(),OrderID.getString().c_str());
	else
		fprintf(fp,"\t\tOrderID = %s    [报单系统唯一代码]\n",OrderID.getString().c_str());
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(PositionID.getValue(),pOriginal->PositionID.getValue())!=0)
		fprintf(fp,"\t\tPositionID = %s -> %s    [持仓代码]\n",pOriginal->PositionID.getString().c_str(),PositionID.getString().c_str());
	else
		fprintf(fp,"\t\tPositionID = %s    [持仓代码]\n",PositionID.getString().c_str());
	if (compare(AccountID.getValue(),pOriginal->AccountID.getValue())!=0)
		fprintf(fp,"\t\tAccountID = %s -> %s    [资金账号]\n",pOriginal->AccountID.getString().c_str(),AccountID.getString().c_str());
	else
		fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(OffsetFlag.getValue(),pOriginal->OffsetFlag.getValue())!=0)
		fprintf(fp,"\t\tOffsetFlag = %s -> %s    [开平标志]\n",pOriginal->OffsetFlag.getString().c_str(),OffsetFlag.getString().c_str());
	else
		fprintf(fp,"\t\tOffsetFlag = %s    [开平标志]\n",OffsetFlag.getString().c_str());
	if (compare(Price.getValue(),pOriginal->Price.getValue())!=0)
		fprintf(fp,"\t\tPrice = %s -> %s    [成交价格]\n",pOriginal->Price.getString().c_str(),Price.getString().c_str());
	else
		fprintf(fp,"\t\tPrice = %s    [成交价格]\n",Price.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [成交数量]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [成交数量]\n",Volume.getString().c_str());
	if (compare(DeriveSource.getValue(),pOriginal->DeriveSource.getValue())!=0)
		fprintf(fp,"\t\tDeriveSource = %s -> %s    [衍生类型]\n",pOriginal->DeriveSource.getString().c_str(),DeriveSource.getString().c_str());
	else
		fprintf(fp,"\t\tDeriveSource = %s    [衍生类型]\n",DeriveSource.getString().c_str());
	if (compare(MatchRole.getValue(),pOriginal->MatchRole.getValue())!=0)
		fprintf(fp,"\t\tMatchRole = %s -> %s    [成交角色]\n",pOriginal->MatchRole.getString().c_str(),MatchRole.getString().c_str());
	else
		fprintf(fp,"\t\tMatchRole = %s    [成交角色]\n",MatchRole.getString().c_str());
	if (compare(PriceCurrency.getValue(),pOriginal->PriceCurrency.getValue())!=0)
		fprintf(fp,"\t\tPriceCurrency = %s -> %s    [计价币种]\n",pOriginal->PriceCurrency.getString().c_str(),PriceCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tPriceCurrency = %s    [计价币种]\n",PriceCurrency.getString().c_str());
	if (compare(ClearCurrency.getValue(),pOriginal->ClearCurrency.getValue())!=0)
		fprintf(fp,"\t\tClearCurrency = %s -> %s    [清算币种]\n",pOriginal->ClearCurrency.getString().c_str(),ClearCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tClearCurrency = %s    [清算币种]\n",ClearCurrency.getString().c_str());
	if (compare(Fee.getValue(),pOriginal->Fee.getValue())!=0)
		fprintf(fp,"\t\tFee = %s -> %s    [手续费]\n",pOriginal->Fee.getString().c_str(),Fee.getString().c_str());
	else
		fprintf(fp,"\t\tFee = %s    [手续费]\n",Fee.getString().c_str());
	if (compare(FeeCurrency.getValue(),pOriginal->FeeCurrency.getValue())!=0)
		fprintf(fp,"\t\tFeeCurrency = %s -> %s    [手续费币种]\n",pOriginal->FeeCurrency.getString().c_str(),FeeCurrency.getString().c_str());
	else
		fprintf(fp,"\t\tFeeCurrency = %s    [手续费币种]\n",FeeCurrency.getString().c_str());
	if (compare(CloseProfit.getValue(),pOriginal->CloseProfit.getValue())!=0)
		fprintf(fp,"\t\tCloseProfit = %s -> %s    [平仓盈亏]\n",pOriginal->CloseProfit.getString().c_str(),CloseProfit.getString().c_str());
	else
		fprintf(fp,"\t\tCloseProfit = %s    [平仓盈亏]\n",CloseProfit.getString().c_str());
	if (compare(Turnover.getValue(),pOriginal->Turnover.getValue())!=0)
		fprintf(fp,"\t\tTurnover = %s -> %s    [成交金额]\n",pOriginal->Turnover.getString().c_str(),Turnover.getString().c_str());
	else
		fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());
	if (compare(UseMargin.getValue(),pOriginal->UseMargin.getValue())!=0)
		fprintf(fp,"\t\tUseMargin = %s -> %s    [占用或者减少的保证金]\n",pOriginal->UseMargin.getString().c_str(),UseMargin.getString().c_str());
	else
		fprintf(fp,"\t\tUseMargin = %s    [占用或者减少的保证金]\n",UseMargin.getString().c_str());
	if (compare(Leverage.getValue(),pOriginal->Leverage.getValue())!=0)
		fprintf(fp,"\t\tLeverage = %s -> %s    [杠杆倍数]\n",pOriginal->Leverage.getString().c_str(),Leverage.getString().c_str());
	else
		fprintf(fp,"\t\tLeverage = %s    [杠杆倍数]\n",Leverage.getString().c_str());
	if (compare(OrderPrice.getValue(),pOriginal->OrderPrice.getValue())!=0)
		fprintf(fp,"\t\tOrderPrice = %s -> %s    [委托价格]\n",pOriginal->OrderPrice.getString().c_str(),OrderPrice.getString().c_str());
	else
		fprintf(fp,"\t\tOrderPrice = %s    [委托价格]\n",OrderPrice.getString().c_str());
	if (compare(TriggerPrice.getValue(),pOriginal->TriggerPrice.getValue())!=0)
		fprintf(fp,"\t\tTriggerPrice = %s -> %s    [触发价]\n",pOriginal->TriggerPrice.getString().c_str(),TriggerPrice.getString().c_str());
	else
		fprintf(fp,"\t\tTriggerPrice = %s    [触发价]\n",TriggerPrice.getString().c_str());
	if (compare(IsSelfTrade.getValue(),pOriginal->IsSelfTrade.getValue())!=0)
		fprintf(fp,"\t\tIsSelfTrade = %s -> %s    [是否自成交]\n",pOriginal->IsSelfTrade.getString().c_str(),IsSelfTrade.getString().c_str());
	else
		fprintf(fp,"\t\tIsSelfTrade = %s    [是否自成交]\n",IsSelfTrade.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [业务序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());
	if (compare(OpenPrice.getValue(),pOriginal->OpenPrice.getValue())!=0)
		fprintf(fp,"\t\tOpenPrice = %s -> %s    [开仓均价]\n",pOriginal->OpenPrice.getString().c_str(),OpenPrice.getString().c_str());
	else
		fprintf(fp,"\t\tOpenPrice = %s    [开仓均价]\n",OpenPrice.getString().c_str());
	if (compare(APPID.getValue(),pOriginal->APPID.getValue())!=0)
		fprintf(fp,"\t\tAPPID = %s -> %s    [应用编号]\n",pOriginal->APPID.getString().c_str(),APPID.getString().c_str());
	else
		fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(CreateTime.getValue(),pOriginal->CreateTime.getValue())!=0)
		fprintf(fp,"\t\tCreateTime = %s -> %s    [创建时间]\n",pOriginal->CreateTime.getString().c_str(),CreateTime.getString().c_str());
	else
		fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());
	if (compare(CopyOrderID.getValue(),pOriginal->CopyOrderID.getValue())!=0)
		fprintf(fp,"\t\tCopyOrderID = %s -> %s    [带单员报单号]\n",pOriginal->CopyOrderID.getString().c_str(),CopyOrderID.getString().c_str());
	else
		fprintf(fp,"\t\tCopyOrderID = %s    [带单员报单号]\n",CopyOrderID.getString().c_str());
	if (compare(CopyMemberID.getValue(),pOriginal->CopyMemberID.getValue())!=0)
		fprintf(fp,"\t\tCopyMemberID = %s -> %s    [带单员代码]\n",pOriginal->CopyMemberID.getString().c_str(),CopyMemberID.getString().c_str());
	else
		fprintf(fp,"\t\tCopyMemberID = %s    [带单员代码]\n",CopyMemberID.getString().c_str());
	if (compare(CopyProfit.getValue(),pOriginal->CopyProfit.getValue())!=0)
		fprintf(fp,"\t\tCopyProfit = %s -> %s    [带单盈利分配]\n",pOriginal->CopyProfit.getString().c_str(),CopyProfit.getString().c_str());
	else
		fprintf(fp,"\t\tCopyProfit = %s    [带单盈利分配]\n",CopyProfit.getString().c_str());
	if (compare(Position.getValue(),pOriginal->Position.getValue())!=0)
		fprintf(fp,"\t\tPosition = %s -> %s    [成交时的持仓量]\n",pOriginal->Position.getString().c_str(),Position.getString().c_str());
	else
		fprintf(fp,"\t\tPosition = %s    [成交时的持仓量]\n",Position.getString().c_str());
	if (compare(ReserveProfit.getValue(),pOriginal->ReserveProfit.getValue())!=0)
		fprintf(fp,"\t\tReserveProfit = %s -> %s    [保留资金盈亏]\n",pOriginal->ReserveProfit.getString().c_str(),ReserveProfit.getString().c_str());
	else
		fprintf(fp,"\t\tReserveProfit = %s    [保留资金盈亏]\n",ReserveProfit.getString().c_str());
	if (compare(ReserveFee.getValue(),pOriginal->ReserveFee.getValue())!=0)
		fprintf(fp,"\t\tReserveFee = %s -> %s    [保留资金手续费]\n",pOriginal->ReserveFee.getString().c_str(),ReserveFee.getString().c_str());
	else
		fprintf(fp,"\t\tReserveFee = %s    [保留资金手续费]\n",ReserveFee.getString().c_str());
	if (compare(TradeRemark.getValue(),pOriginal->TradeRemark.getValue())!=0)
		fprintf(fp,"\t\tTradeRemark = %s -> %s    [Trade备注]\n",pOriginal->TradeRemark.getString().c_str(),TradeRemark.getString().c_str());
	else
		fprintf(fp,"\t\tTradeRemark = %s    [Trade备注]\n",TradeRemark.getString().c_str());
	if (compare(BusinessType.getValue(),pOriginal->BusinessType.getValue())!=0)
		fprintf(fp,"\t\tBusinessType = %s -> %s    [业务类别]\n",pOriginal->BusinessType.getString().c_str(),BusinessType.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessType = %s    [业务类别]\n",BusinessType.getString().c_str());
	if (compare(BusinessValue.getValue(),pOriginal->BusinessValue.getValue())!=0)
		fprintf(fp,"\t\tBusinessValue = %s -> %s    [业务值]\n",pOriginal->BusinessValue.getString().c_str(),BusinessValue.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessValue = %s    [业务值]\n",BusinessValue.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableTrade::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",TradeID=%s",TradeID.getString().c_str());	
	fprintf(fp,",Direction=%s",Direction.getString().c_str());	
	fprintf(fp,",OrderID=%s",OrderID.getString().c_str());	
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",PositionID=%s",PositionID.getString().c_str());	
	fprintf(fp,",AccountID=%s",AccountID.getString().c_str());	
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",OffsetFlag=%s",OffsetFlag.getString().c_str());	
	fprintf(fp,",Price=%s",Price.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",DeriveSource=%s",DeriveSource.getString().c_str());	
	fprintf(fp,",MatchRole=%s",MatchRole.getString().c_str());	
	fprintf(fp,",PriceCurrency=%s",PriceCurrency.getString().c_str());	
	fprintf(fp,",ClearCurrency=%s",ClearCurrency.getString().c_str());	
	fprintf(fp,",Fee=%s",Fee.getString().c_str());	
	fprintf(fp,",FeeCurrency=%s",FeeCurrency.getString().c_str());	
	fprintf(fp,",CloseProfit=%s",CloseProfit.getString().c_str());	
	fprintf(fp,",Turnover=%s",Turnover.getString().c_str());	
	fprintf(fp,",UseMargin=%s",UseMargin.getString().c_str());	
	fprintf(fp,",Leverage=%s",Leverage.getString().c_str());	
	fprintf(fp,",OrderPrice=%s",OrderPrice.getString().c_str());	
	fprintf(fp,",TriggerPrice=%s",TriggerPrice.getString().c_str());	
	fprintf(fp,",IsSelfTrade=%s",IsSelfTrade.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,",OpenPrice=%s",OpenPrice.getString().c_str());	
	fprintf(fp,",APPID=%s",APPID.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",CreateTime=%s",CreateTime.getString().c_str());	
	fprintf(fp,",CopyOrderID=%s",CopyOrderID.getString().c_str());	
	fprintf(fp,",CopyMemberID=%s",CopyMemberID.getString().c_str());	
	fprintf(fp,",CopyProfit=%s",CopyProfit.getString().c_str());	
	fprintf(fp,",Position=%s",Position.getString().c_str());	
	fprintf(fp,",ReserveProfit=%s",ReserveProfit.getString().c_str());	
	fprintf(fp,",ReserveFee=%s",ReserveFee.getString().c_str());	
	fprintf(fp,",TradeRemark=%s",TradeRemark.getString().c_str());	
	fprintf(fp,",BusinessType=%s",BusinessType.getString().c_str());	
	fprintf(fp,",BusinessValue=%s",BusinessValue.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableTrade::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"TradeID=%s,",TradeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Direction=%s,",Direction.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderID=%s,",OrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PositionID=%s,",PositionID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccountID=%s,",AccountID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OffsetFlag=%s,",OffsetFlag.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Price=%s,",Price.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"DeriveSource=%s,",DeriveSource.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MatchRole=%s,",MatchRole.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PriceCurrency=%s,",PriceCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClearCurrency=%s,",ClearCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Fee=%s,",Fee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FeeCurrency=%s,",FeeCurrency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CloseProfit=%s,",CloseProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Turnover=%s,",Turnover.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UseMargin=%s,",UseMargin.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Leverage=%s,",Leverage.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderPrice=%s,",OrderPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TriggerPrice=%s,",TriggerPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"IsSelfTrade=%s,",IsSelfTrade.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenPrice=%s,",OpenPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"APPID=%s,",APPID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CreateTime=%s,",CreateTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyOrderID=%s,",CopyOrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyMemberID=%s,",CopyMemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CopyProfit=%s,",CopyProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Position=%s,",Position.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ReserveProfit=%s,",ReserveProfit.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ReserveFee=%s,",ReserveFee.getString().c_str());
	p+=strlen(p);
	sprintf(p,"TradeRemark=%s,",TradeRemark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessType=%s,",BusinessType.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessValue=%s,",BusinessValue.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CTradeIterator::dump(FILE *output)
{
	CTrade *pTrade;
	while ((pTrade=next())!=NULL)
	{
		pTrade->dump(output);
	}
}



void CWriteableKLine::writeCSVHead(FILE *output)
{
	fprintf(output,"\"ExchangeID\",\"InstrumentID\",\"Bar\",\"InsertTime\",\"OpenPrice\",\"HighestPrice\",\"LowestPrice\",\"ClosePrice\",\"Volume\",\"Turnover\"\n");
}

void CWriteableKLine::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"ExchangeID\",\"InstrumentID\",\"Bar\",\"InsertTime\",\"OpenPrice\",\"HighestPrice\",\"LowestPrice\",\"ClosePrice\",\"Volume\",\"Turnover\"\n");
}
	
void CWriteableKLine::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	ExchangeID.init();
	InstrumentID.init();
	Bar.init();
	InsertTime.clear();
	OpenPrice.clear();
	HighestPrice.clear();
	LowestPrice.clear();
	ClosePrice.clear();
	Volume.init();
	Turnover.init();
}

void CWriteableKLine::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableKLine::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableKLine),input)!=sizeof(CWriteableKLine))
		return 0;
	else
		return 1;
}
	
int CWriteableKLine::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Bar=token;
		else
			Bar="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenPrice=token;
		else
			OpenPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			HighestPrice=token;
		else
			HighestPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LowestPrice=token;
		else
			LowestPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClosePrice=token;
		else
			ClosePrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Turnover=token;
		else
			Turnover="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"Bar"))
			{
				Bar=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"OpenPrice"))
			{
				OpenPrice=token;
			}
			else if (!strcmp(fieldName,"HighestPrice"))
			{
				HighestPrice=token;
			}
			else if (!strcmp(fieldName,"LowestPrice"))
			{
				LowestPrice=token;
			}
			else if (!strcmp(fieldName,"ClosePrice"))
			{
				ClosePrice=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"Turnover"))
			{
				Turnover=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableKLine::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableKLine),output)!=sizeof(CWriteableKLine))
		return 0;
	else
		return 1;
}

int CWriteableKLine::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Bar.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowestPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableKLine::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Bar.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowestPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableKLine::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCKLine%s = {\n", index_buf);
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tBar = %s    [K线周期代码(n[m/h/d/o])]\n",Bar.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tOpenPrice = %s    [开盘价]\n",OpenPrice.getString().c_str());	
	fprintf(fp,"\t\tHighestPrice = %s    [最高价]\n",HighestPrice.getString().c_str());	
	fprintf(fp,"\t\tLowestPrice = %s    [最低价]\n",LowestPrice.getString().c_str());	
	fprintf(fp,"\t\tClosePrice = %s    [收盘价]\n",ClosePrice.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableKLine::dumpDiff(FILE *fp, const CWriteableKLine *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCKLine = {\n");
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(Bar.getValue(),pOriginal->Bar.getValue())!=0)
		fprintf(fp,"\t\tBar = %s -> %s    [K线周期代码(n[m/h/d/o])]\n",pOriginal->Bar.getString().c_str(),Bar.getString().c_str());
	else
		fprintf(fp,"\t\tBar = %s    [K线周期代码(n[m/h/d/o])]\n",Bar.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(OpenPrice.getValue(),pOriginal->OpenPrice.getValue())!=0)
		fprintf(fp,"\t\tOpenPrice = %s -> %s    [开盘价]\n",pOriginal->OpenPrice.getString().c_str(),OpenPrice.getString().c_str());
	else
		fprintf(fp,"\t\tOpenPrice = %s    [开盘价]\n",OpenPrice.getString().c_str());
	if (compare(HighestPrice.getValue(),pOriginal->HighestPrice.getValue())!=0)
		fprintf(fp,"\t\tHighestPrice = %s -> %s    [最高价]\n",pOriginal->HighestPrice.getString().c_str(),HighestPrice.getString().c_str());
	else
		fprintf(fp,"\t\tHighestPrice = %s    [最高价]\n",HighestPrice.getString().c_str());
	if (compare(LowestPrice.getValue(),pOriginal->LowestPrice.getValue())!=0)
		fprintf(fp,"\t\tLowestPrice = %s -> %s    [最低价]\n",pOriginal->LowestPrice.getString().c_str(),LowestPrice.getString().c_str());
	else
		fprintf(fp,"\t\tLowestPrice = %s    [最低价]\n",LowestPrice.getString().c_str());
	if (compare(ClosePrice.getValue(),pOriginal->ClosePrice.getValue())!=0)
		fprintf(fp,"\t\tClosePrice = %s -> %s    [收盘价]\n",pOriginal->ClosePrice.getString().c_str(),ClosePrice.getString().c_str());
	else
		fprintf(fp,"\t\tClosePrice = %s    [收盘价]\n",ClosePrice.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [数量]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());
	if (compare(Turnover.getValue(),pOriginal->Turnover.getValue())!=0)
		fprintf(fp,"\t\tTurnover = %s -> %s    [成交金额]\n",pOriginal->Turnover.getString().c_str(),Turnover.getString().c_str());
	else
		fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableKLine::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",Bar=%s",Bar.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",OpenPrice=%s",OpenPrice.getString().c_str());	
	fprintf(fp,",HighestPrice=%s",HighestPrice.getString().c_str());	
	fprintf(fp,",LowestPrice=%s",LowestPrice.getString().c_str());	
	fprintf(fp,",ClosePrice=%s",ClosePrice.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",Turnover=%s",Turnover.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableKLine::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Bar=%s,",Bar.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenPrice=%s,",OpenPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"HighestPrice=%s,",HighestPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LowestPrice=%s,",LowestPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClosePrice=%s,",ClosePrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Turnover=%s,",Turnover.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CKLineIterator::dump(FILE *output)
{
	CKLine *pKLine;
	while ((pKLine=next())!=NULL)
	{
		pKLine->dump(output);
	}
}



void CWriteablePublishKLine::writeCSVHead(FILE *output)
{
	fprintf(output,"\"ExchangeID*\",\"InstrumentID*\",\"Bar*\",\"InsertTime*\",\"OpenPrice\",\"HighestPrice\",\"LowestPrice\",\"ClosePrice\",\"Volume\",\"Turnover\"\n");
}

void CWriteablePublishKLine::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"ExchangeID\",\"InstrumentID\",\"Bar\",\"InsertTime\",\"OpenPrice\",\"HighestPrice\",\"LowestPrice\",\"ClosePrice\",\"Volume\",\"Turnover\"\n");
}
	
void CWriteablePublishKLine::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	ExchangeID.init();
	InstrumentID.init();
	Bar.init();
	InsertTime.clear();
	OpenPrice.clear();
	HighestPrice.clear();
	LowestPrice.clear();
	ClosePrice.clear();
	Volume.init();
	Turnover.init();
}

void CWriteablePublishKLine::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteablePublishKLine::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteablePublishKLine),input)!=sizeof(CWriteablePublishKLine))
		return 0;
	else
		return 1;
}
	
int CWriteablePublishKLine::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Bar=token;
		else
			Bar="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenPrice=token;
		else
			OpenPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			HighestPrice=token;
		else
			HighestPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LowestPrice=token;
		else
			LowestPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClosePrice=token;
		else
			ClosePrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Turnover=token;
		else
			Turnover="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"Bar"))
			{
				Bar=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"OpenPrice"))
			{
				OpenPrice=token;
			}
			else if (!strcmp(fieldName,"HighestPrice"))
			{
				HighestPrice=token;
			}
			else if (!strcmp(fieldName,"LowestPrice"))
			{
				LowestPrice=token;
			}
			else if (!strcmp(fieldName,"ClosePrice"))
			{
				ClosePrice=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"Turnover"))
			{
				Turnover=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteablePublishKLine::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteablePublishKLine),output)!=sizeof(CWriteablePublishKLine))
		return 0;
	else
		return 1;
}

int CWriteablePublishKLine::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Bar.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowestPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteablePublishKLine::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Bar.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowestPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteablePublishKLine::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCPublishKLine%s = {\n", index_buf);
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tBar = %s    [K线周期代码(n[m/h/d/o])]\n",Bar.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tOpenPrice = %s    [开盘价]\n",OpenPrice.getString().c_str());	
	fprintf(fp,"\t\tHighestPrice = %s    [最高价]\n",HighestPrice.getString().c_str());	
	fprintf(fp,"\t\tLowestPrice = %s    [最低价]\n",LowestPrice.getString().c_str());	
	fprintf(fp,"\t\tClosePrice = %s    [收盘价]\n",ClosePrice.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteablePublishKLine::dumpDiff(FILE *fp, const CWriteablePublishKLine *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCPublishKLine = {\n");
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(Bar.getValue(),pOriginal->Bar.getValue())!=0)
		fprintf(fp,"\t\tBar = %s -> %s    [K线周期代码(n[m/h/d/o])]\n",pOriginal->Bar.getString().c_str(),Bar.getString().c_str());
	else
		fprintf(fp,"\t\tBar = %s    [K线周期代码(n[m/h/d/o])]\n",Bar.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(OpenPrice.getValue(),pOriginal->OpenPrice.getValue())!=0)
		fprintf(fp,"\t\tOpenPrice = %s -> %s    [开盘价]\n",pOriginal->OpenPrice.getString().c_str(),OpenPrice.getString().c_str());
	else
		fprintf(fp,"\t\tOpenPrice = %s    [开盘价]\n",OpenPrice.getString().c_str());
	if (compare(HighestPrice.getValue(),pOriginal->HighestPrice.getValue())!=0)
		fprintf(fp,"\t\tHighestPrice = %s -> %s    [最高价]\n",pOriginal->HighestPrice.getString().c_str(),HighestPrice.getString().c_str());
	else
		fprintf(fp,"\t\tHighestPrice = %s    [最高价]\n",HighestPrice.getString().c_str());
	if (compare(LowestPrice.getValue(),pOriginal->LowestPrice.getValue())!=0)
		fprintf(fp,"\t\tLowestPrice = %s -> %s    [最低价]\n",pOriginal->LowestPrice.getString().c_str(),LowestPrice.getString().c_str());
	else
		fprintf(fp,"\t\tLowestPrice = %s    [最低价]\n",LowestPrice.getString().c_str());
	if (compare(ClosePrice.getValue(),pOriginal->ClosePrice.getValue())!=0)
		fprintf(fp,"\t\tClosePrice = %s -> %s    [收盘价]\n",pOriginal->ClosePrice.getString().c_str(),ClosePrice.getString().c_str());
	else
		fprintf(fp,"\t\tClosePrice = %s    [收盘价]\n",ClosePrice.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [数量]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());
	if (compare(Turnover.getValue(),pOriginal->Turnover.getValue())!=0)
		fprintf(fp,"\t\tTurnover = %s -> %s    [成交金额]\n",pOriginal->Turnover.getString().c_str(),Turnover.getString().c_str());
	else
		fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteablePublishKLine::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",Bar=%s",Bar.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",OpenPrice=%s",OpenPrice.getString().c_str());	
	fprintf(fp,",HighestPrice=%s",HighestPrice.getString().c_str());	
	fprintf(fp,",LowestPrice=%s",LowestPrice.getString().c_str());	
	fprintf(fp,",ClosePrice=%s",ClosePrice.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",Turnover=%s",Turnover.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteablePublishKLine::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Bar=%s,",Bar.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenPrice=%s,",OpenPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"HighestPrice=%s,",HighestPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LowestPrice=%s,",LowestPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClosePrice=%s,",ClosePrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Turnover=%s,",Turnover.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CPublishKLineIterator::dump(FILE *output)
{
	CPublishKLine *pPublishKLine;
	while ((pPublishKLine=next())!=NULL)
	{
		pPublishKLine->dump(output);
	}
}



void CWriteableLastKLine::writeCSVHead(FILE *output)
{
	fprintf(output,"\"ExchangeID*\",\"InstrumentID*\",\"Bar*\",\"InsertTime\",\"OpenPrice\",\"HighestPrice\",\"LowestPrice\",\"ClosePrice\",\"Volume\",\"Turnover\",\"UpdateTime\"\n");
}

void CWriteableLastKLine::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"ExchangeID\",\"InstrumentID\",\"Bar\",\"InsertTime\",\"OpenPrice\",\"HighestPrice\",\"LowestPrice\",\"ClosePrice\",\"Volume\",\"Turnover\",\"UpdateTime\"\n");
}
	
void CWriteableLastKLine::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	ExchangeID.init();
	InstrumentID.init();
	Bar.init();
	InsertTime.clear();
	OpenPrice.clear();
	HighestPrice.clear();
	LowestPrice.clear();
	ClosePrice.clear();
	Volume.init();
	Turnover.init();
	UpdateTime.init();
}

void CWriteableLastKLine::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableLastKLine::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableLastKLine),input)!=sizeof(CWriteableLastKLine))
		return 0;
	else
		return 1;
}
	
int CWriteableLastKLine::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Bar=token;
		else
			Bar="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OpenPrice=token;
		else
			OpenPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			HighestPrice=token;
		else
			HighestPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LowestPrice=token;
		else
			LowestPrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ClosePrice=token;
		else
			ClosePrice="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Turnover=token;
		else
			Turnover="";
		token=getNextToken(NULL);
		if (token!=NULL)
			UpdateTime=token;
		else
			UpdateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"Bar"))
			{
				Bar=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"OpenPrice"))
			{
				OpenPrice=token;
			}
			else if (!strcmp(fieldName,"HighestPrice"))
			{
				HighestPrice=token;
			}
			else if (!strcmp(fieldName,"LowestPrice"))
			{
				LowestPrice=token;
			}
			else if (!strcmp(fieldName,"ClosePrice"))
			{
				ClosePrice=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"Turnover"))
			{
				Turnover=token;
			}
			else if (!strcmp(fieldName,"UpdateTime"))
			{
				UpdateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableLastKLine::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableLastKLine),output)!=sizeof(CWriteableLastKLine))
		return 0;
	else
		return 1;
}

int CWriteableLastKLine::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Bar.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowestPrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableLastKLine::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Bar.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OpenPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",HighestPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LowestPrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ClosePrice.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Turnover.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",UpdateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableLastKLine::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCLastKLine%s = {\n", index_buf);
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tBar = %s    [K线周期代码(n[m/h/d/o])]\n",Bar.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tOpenPrice = %s    [开盘价]\n",OpenPrice.getString().c_str());	
	fprintf(fp,"\t\tHighestPrice = %s    [最高价]\n",HighestPrice.getString().c_str());	
	fprintf(fp,"\t\tLowestPrice = %s    [最低价]\n",LowestPrice.getString().c_str());	
	fprintf(fp,"\t\tClosePrice = %s    [收盘价]\n",ClosePrice.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());	
	fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableLastKLine::dumpDiff(FILE *fp, const CWriteableLastKLine *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCLastKLine = {\n");
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(Bar.getValue(),pOriginal->Bar.getValue())!=0)
		fprintf(fp,"\t\tBar = %s -> %s    [K线周期代码(n[m/h/d/o])]\n",pOriginal->Bar.getString().c_str(),Bar.getString().c_str());
	else
		fprintf(fp,"\t\tBar = %s    [K线周期代码(n[m/h/d/o])]\n",Bar.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(OpenPrice.getValue(),pOriginal->OpenPrice.getValue())!=0)
		fprintf(fp,"\t\tOpenPrice = %s -> %s    [开盘价]\n",pOriginal->OpenPrice.getString().c_str(),OpenPrice.getString().c_str());
	else
		fprintf(fp,"\t\tOpenPrice = %s    [开盘价]\n",OpenPrice.getString().c_str());
	if (compare(HighestPrice.getValue(),pOriginal->HighestPrice.getValue())!=0)
		fprintf(fp,"\t\tHighestPrice = %s -> %s    [最高价]\n",pOriginal->HighestPrice.getString().c_str(),HighestPrice.getString().c_str());
	else
		fprintf(fp,"\t\tHighestPrice = %s    [最高价]\n",HighestPrice.getString().c_str());
	if (compare(LowestPrice.getValue(),pOriginal->LowestPrice.getValue())!=0)
		fprintf(fp,"\t\tLowestPrice = %s -> %s    [最低价]\n",pOriginal->LowestPrice.getString().c_str(),LowestPrice.getString().c_str());
	else
		fprintf(fp,"\t\tLowestPrice = %s    [最低价]\n",LowestPrice.getString().c_str());
	if (compare(ClosePrice.getValue(),pOriginal->ClosePrice.getValue())!=0)
		fprintf(fp,"\t\tClosePrice = %s -> %s    [收盘价]\n",pOriginal->ClosePrice.getString().c_str(),ClosePrice.getString().c_str());
	else
		fprintf(fp,"\t\tClosePrice = %s    [收盘价]\n",ClosePrice.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [数量]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [数量]\n",Volume.getString().c_str());
	if (compare(Turnover.getValue(),pOriginal->Turnover.getValue())!=0)
		fprintf(fp,"\t\tTurnover = %s -> %s    [成交金额]\n",pOriginal->Turnover.getString().c_str(),Turnover.getString().c_str());
	else
		fprintf(fp,"\t\tTurnover = %s    [成交金额]\n",Turnover.getString().c_str());
	if (compare(UpdateTime.getValue(),pOriginal->UpdateTime.getValue())!=0)
		fprintf(fp,"\t\tUpdateTime = %s -> %s    [更新时间]\n",pOriginal->UpdateTime.getString().c_str(),UpdateTime.getString().c_str());
	else
		fprintf(fp,"\t\tUpdateTime = %s    [更新时间]\n",UpdateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableLastKLine::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",Bar=%s",Bar.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",OpenPrice=%s",OpenPrice.getString().c_str());	
	fprintf(fp,",HighestPrice=%s",HighestPrice.getString().c_str());	
	fprintf(fp,",LowestPrice=%s",LowestPrice.getString().c_str());	
	fprintf(fp,",ClosePrice=%s",ClosePrice.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",Turnover=%s",Turnover.getString().c_str());	
	fprintf(fp,",UpdateTime=%s",UpdateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableLastKLine::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Bar=%s,",Bar.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OpenPrice=%s,",OpenPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"HighestPrice=%s,",HighestPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LowestPrice=%s,",LowestPrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ClosePrice=%s,",ClosePrice.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Turnover=%s,",Turnover.getString().c_str());
	p+=strlen(p);
	sprintf(p,"UpdateTime=%s,",UpdateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CLastKLineIterator::dump(FILE *output)
{
	CLastKLine *pLastKLine;
	while ((pLastKLine=next())!=NULL)
	{
		pLastKLine->dump(output);
	}
}



void CWriteableAccountDetail::writeCSVHead(FILE *output)
{
	fprintf(output,"\"AccountDetailID\",\"MemberID\",\"ExchangeID\",\"InstrumentID\",\"SettlementGroup\",\"AccountID\",\"Currency\",\"Amount\",\"PreBalance\",\"Balance\",\"ReserveAmount\",\"ReserveBalance\",\"Source\",\"Remark\",\"LocalID\",\"SettleSegment\",\"BusinessNo\",\"RelatedID\",\"InsertTime\",\"CreateTime\"\n");
}

void CWriteableAccountDetail::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"AccountDetailID\",\"MemberID\",\"ExchangeID\",\"InstrumentID\",\"SettlementGroup\",\"AccountID\",\"Currency\",\"Amount\",\"PreBalance\",\"Balance\",\"ReserveAmount\",\"ReserveBalance\",\"Source\",\"Remark\",\"LocalID\",\"SettleSegment\",\"BusinessNo\",\"RelatedID\",\"InsertTime\",\"CreateTime\"\n");
}
	
void CWriteableAccountDetail::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	AccountDetailID.init();
	MemberID.clear();
	ExchangeID.clear();
	InstrumentID.clear();
	SettlementGroup.init();
	AccountID.init();
	Currency.init();
	Amount.init();
	PreBalance.init();
	Balance.init();
	ReserveAmount.init();
	ReserveBalance.init();
	Source.init();
	Remark.clear();
	LocalID.clear();
	SettleSegment.clear();
	BusinessNo.clear();
	RelatedID.clear();
	InsertTime.init();
	CreateTime.clear();
}

void CWriteableAccountDetail::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableAccountDetail::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableAccountDetail),input)!=sizeof(CWriteableAccountDetail))
		return 0;
	else
		return 1;
}
	
int CWriteableAccountDetail::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			AccountDetailID=token;
		else
			AccountDetailID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettlementGroup=token;
		else
			SettlementGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccountID=token;
		else
			AccountID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Currency=token;
		else
			Currency="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Amount=token;
		else
			Amount="";
		token=getNextToken(NULL);
		if (token!=NULL)
			PreBalance=token;
		else
			PreBalance="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Balance=token;
		else
			Balance="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ReserveAmount=token;
		else
			ReserveAmount="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ReserveBalance=token;
		else
			ReserveBalance="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Source=token;
		else
			Source="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LocalID=token;
		else
			LocalID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettleSegment=token;
		else
			SettleSegment="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
		token=getNextToken(NULL);
		if (token!=NULL)
			RelatedID=token;
		else
			RelatedID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			CreateTime=token;
		else
			CreateTime="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"AccountDetailID"))
			{
				AccountDetailID=token;
			}
			else if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"SettlementGroup"))
			{
				SettlementGroup=token;
			}
			else if (!strcmp(fieldName,"AccountID"))
			{
				AccountID=token;
			}
			else if (!strcmp(fieldName,"Currency"))
			{
				Currency=token;
			}
			else if (!strcmp(fieldName,"Amount"))
			{
				Amount=token;
			}
			else if (!strcmp(fieldName,"PreBalance"))
			{
				PreBalance=token;
			}
			else if (!strcmp(fieldName,"Balance"))
			{
				Balance=token;
			}
			else if (!strcmp(fieldName,"ReserveAmount"))
			{
				ReserveAmount=token;
			}
			else if (!strcmp(fieldName,"ReserveBalance"))
			{
				ReserveBalance=token;
			}
			else if (!strcmp(fieldName,"Source"))
			{
				Source=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"LocalID"))
			{
				LocalID=token;
			}
			else if (!strcmp(fieldName,"SettleSegment"))
			{
				SettleSegment=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			else if (!strcmp(fieldName,"RelatedID"))
			{
				RelatedID=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"CreateTime"))
			{
				CreateTime=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableAccountDetail::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableAccountDetail),output)!=sizeof(CWriteableAccountDetail))
		return 0;
	else
		return 1;
}

int CWriteableAccountDetail::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",AccountDetailID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Currency.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Amount.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreBalance.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Balance.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveAmount.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveBalance.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Source.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettleSegment.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",RelatedID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableAccountDetail::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",AccountDetailID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Currency.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Amount.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",PreBalance.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Balance.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveAmount.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ReserveBalance.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Source.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettleSegment.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",RelatedID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",CreateTime.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableAccountDetail::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCAccountDetail%s = {\n", index_buf);
	fprintf(fp,"\t\tAccountDetailID = %s    [资金明细号]\n",AccountDetailID.getString().c_str());	
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());	
	fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());	
	fprintf(fp,"\t\tCurrency = %s    [币种]\n",Currency.getString().c_str());	
	fprintf(fp,"\t\tAmount = %s    [发生额]\n",Amount.getString().c_str());	
	fprintf(fp,"\t\tPreBalance = %s    [上次静态权益]\n",PreBalance.getString().c_str());	
	fprintf(fp,"\t\tBalance = %s    [静态权益]\n",Balance.getString().c_str());	
	fprintf(fp,"\t\tReserveAmount = %s    [体验金发生额]\n",ReserveAmount.getString().c_str());	
	fprintf(fp,"\t\tReserveBalance = %s    [体验金静态权益]\n",ReserveBalance.getString().c_str());	
	fprintf(fp,"\t\tSource = %s    [财务流水类型]\n",Source.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tLocalID = %s    [本地标识]\n",LocalID.getString().c_str());	
	fprintf(fp,"\t\tSettleSegment = %s    [结算段]\n",SettleSegment.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t\tRelatedID = %s    [内外对账ID]\n",RelatedID.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableAccountDetail::dumpDiff(FILE *fp, const CWriteableAccountDetail *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCAccountDetail = {\n");
	if (compare(AccountDetailID.getValue(),pOriginal->AccountDetailID.getValue())!=0)
		fprintf(fp,"\t\tAccountDetailID = %s -> %s    [资金明细号]\n",pOriginal->AccountDetailID.getString().c_str(),AccountDetailID.getString().c_str());
	else
		fprintf(fp,"\t\tAccountDetailID = %s    [资金明细号]\n",AccountDetailID.getString().c_str());
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(SettlementGroup.getValue(),pOriginal->SettlementGroup.getValue())!=0)
		fprintf(fp,"\t\tSettlementGroup = %s -> %s    [结算组编号]\n",pOriginal->SettlementGroup.getString().c_str(),SettlementGroup.getString().c_str());
	else
		fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());
	if (compare(AccountID.getValue(),pOriginal->AccountID.getValue())!=0)
		fprintf(fp,"\t\tAccountID = %s -> %s    [资金账号]\n",pOriginal->AccountID.getString().c_str(),AccountID.getString().c_str());
	else
		fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());
	if (compare(Currency.getValue(),pOriginal->Currency.getValue())!=0)
		fprintf(fp,"\t\tCurrency = %s -> %s    [币种]\n",pOriginal->Currency.getString().c_str(),Currency.getString().c_str());
	else
		fprintf(fp,"\t\tCurrency = %s    [币种]\n",Currency.getString().c_str());
	if (compare(Amount.getValue(),pOriginal->Amount.getValue())!=0)
		fprintf(fp,"\t\tAmount = %s -> %s    [发生额]\n",pOriginal->Amount.getString().c_str(),Amount.getString().c_str());
	else
		fprintf(fp,"\t\tAmount = %s    [发生额]\n",Amount.getString().c_str());
	if (compare(PreBalance.getValue(),pOriginal->PreBalance.getValue())!=0)
		fprintf(fp,"\t\tPreBalance = %s -> %s    [上次静态权益]\n",pOriginal->PreBalance.getString().c_str(),PreBalance.getString().c_str());
	else
		fprintf(fp,"\t\tPreBalance = %s    [上次静态权益]\n",PreBalance.getString().c_str());
	if (compare(Balance.getValue(),pOriginal->Balance.getValue())!=0)
		fprintf(fp,"\t\tBalance = %s -> %s    [静态权益]\n",pOriginal->Balance.getString().c_str(),Balance.getString().c_str());
	else
		fprintf(fp,"\t\tBalance = %s    [静态权益]\n",Balance.getString().c_str());
	if (compare(ReserveAmount.getValue(),pOriginal->ReserveAmount.getValue())!=0)
		fprintf(fp,"\t\tReserveAmount = %s -> %s    [体验金发生额]\n",pOriginal->ReserveAmount.getString().c_str(),ReserveAmount.getString().c_str());
	else
		fprintf(fp,"\t\tReserveAmount = %s    [体验金发生额]\n",ReserveAmount.getString().c_str());
	if (compare(ReserveBalance.getValue(),pOriginal->ReserveBalance.getValue())!=0)
		fprintf(fp,"\t\tReserveBalance = %s -> %s    [体验金静态权益]\n",pOriginal->ReserveBalance.getString().c_str(),ReserveBalance.getString().c_str());
	else
		fprintf(fp,"\t\tReserveBalance = %s    [体验金静态权益]\n",ReserveBalance.getString().c_str());
	if (compare(Source.getValue(),pOriginal->Source.getValue())!=0)
		fprintf(fp,"\t\tSource = %s -> %s    [财务流水类型]\n",pOriginal->Source.getString().c_str(),Source.getString().c_str());
	else
		fprintf(fp,"\t\tSource = %s    [财务流水类型]\n",Source.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(LocalID.getValue(),pOriginal->LocalID.getValue())!=0)
		fprintf(fp,"\t\tLocalID = %s -> %s    [本地标识]\n",pOriginal->LocalID.getString().c_str(),LocalID.getString().c_str());
	else
		fprintf(fp,"\t\tLocalID = %s    [本地标识]\n",LocalID.getString().c_str());
	if (compare(SettleSegment.getValue(),pOriginal->SettleSegment.getValue())!=0)
		fprintf(fp,"\t\tSettleSegment = %s -> %s    [结算段]\n",pOriginal->SettleSegment.getString().c_str(),SettleSegment.getString().c_str());
	else
		fprintf(fp,"\t\tSettleSegment = %s    [结算段]\n",SettleSegment.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [业务序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());
	if (compare(RelatedID.getValue(),pOriginal->RelatedID.getValue())!=0)
		fprintf(fp,"\t\tRelatedID = %s -> %s    [内外对账ID]\n",pOriginal->RelatedID.getString().c_str(),RelatedID.getString().c_str());
	else
		fprintf(fp,"\t\tRelatedID = %s    [内外对账ID]\n",RelatedID.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [插入时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [插入时间]\n",InsertTime.getString().c_str());
	if (compare(CreateTime.getValue(),pOriginal->CreateTime.getValue())!=0)
		fprintf(fp,"\t\tCreateTime = %s -> %s    [创建时间]\n",pOriginal->CreateTime.getString().c_str(),CreateTime.getString().c_str());
	else
		fprintf(fp,"\t\tCreateTime = %s    [创建时间]\n",CreateTime.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableAccountDetail::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",AccountDetailID=%s",AccountDetailID.getString().c_str());	
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",SettlementGroup=%s",SettlementGroup.getString().c_str());	
	fprintf(fp,",AccountID=%s",AccountID.getString().c_str());	
	fprintf(fp,",Currency=%s",Currency.getString().c_str());	
	fprintf(fp,",Amount=%s",Amount.getString().c_str());	
	fprintf(fp,",PreBalance=%s",PreBalance.getString().c_str());	
	fprintf(fp,",Balance=%s",Balance.getString().c_str());	
	fprintf(fp,",ReserveAmount=%s",ReserveAmount.getString().c_str());	
	fprintf(fp,",ReserveBalance=%s",ReserveBalance.getString().c_str());	
	fprintf(fp,",Source=%s",Source.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",LocalID=%s",LocalID.getString().c_str());	
	fprintf(fp,",SettleSegment=%s",SettleSegment.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,",RelatedID=%s",RelatedID.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",CreateTime=%s",CreateTime.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableAccountDetail::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"AccountDetailID=%s,",AccountDetailID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettlementGroup=%s,",SettlementGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccountID=%s,",AccountID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Currency=%s,",Currency.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Amount=%s,",Amount.getString().c_str());
	p+=strlen(p);
	sprintf(p,"PreBalance=%s,",PreBalance.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Balance=%s,",Balance.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ReserveAmount=%s,",ReserveAmount.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ReserveBalance=%s,",ReserveBalance.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Source=%s,",Source.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LocalID=%s,",LocalID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettleSegment=%s,",SettleSegment.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	sprintf(p,"RelatedID=%s,",RelatedID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"CreateTime=%s,",CreateTime.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CAccountDetailIterator::dump(FILE *output)
{
	CAccountDetail *pAccountDetail;
	while ((pAccountDetail=next())!=NULL)
	{
		pAccountDetail->dump(output);
	}
}



void CWriteableSettleDetail::writeCSVHead(FILE *output)
{
	fprintf(output,"\"SettleDetailID*\",\"APPID\",\"LocalID\",\"ExchangeID*\",\"InstrumentID*\",\"SettlementGroup\",\"SettleAction\",\"Value\",\"Value1\",\"Value2\",\"Value3\",\"Remark\",\"FundingRateGroup\",\"SettleSegment\",\"InsertTime\",\"BusinessNo\"\n");
}

void CWriteableSettleDetail::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"SettleDetailID\",\"APPID\",\"LocalID\",\"ExchangeID\",\"InstrumentID\",\"SettlementGroup\",\"SettleAction\",\"Value\",\"Value1\",\"Value2\",\"Value3\",\"Remark\",\"FundingRateGroup\",\"SettleSegment\",\"InsertTime\",\"BusinessNo\"\n");
}
	
void CWriteableSettleDetail::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	SettleDetailID.init();
	APPID.clear();
	LocalID.clear();
	ExchangeID.clear();
	InstrumentID.clear();
	SettlementGroup.clear();
	SettleAction.init();
	Value.clear();
	Value1.clear();
	Value2.clear();
	Value3.clear();
	Remark.clear();
	FundingRateGroup.clear();
	SettleSegment.clear();
	InsertTime.clear();
	BusinessNo.clear();
}

void CWriteableSettleDetail::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableSettleDetail::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableSettleDetail),input)!=sizeof(CWriteableSettleDetail))
		return 0;
	else
		return 1;
}
	
int CWriteableSettleDetail::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			SettleDetailID=token;
		else
			SettleDetailID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			APPID=token;
		else
			APPID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LocalID=token;
		else
			LocalID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettlementGroup=token;
		else
			SettlementGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettleAction=token;
		else
			SettleAction="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Value=token;
		else
			Value="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Value1=token;
		else
			Value1="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Value2=token;
		else
			Value2="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Value3=token;
		else
			Value3="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
		token=getNextToken(NULL);
		if (token!=NULL)
			FundingRateGroup=token;
		else
			FundingRateGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			SettleSegment=token;
		else
			SettleSegment="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InsertTime=token;
		else
			InsertTime="";
		token=getNextToken(NULL);
		if (token!=NULL)
			BusinessNo=token;
		else
			BusinessNo="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"SettleDetailID"))
			{
				SettleDetailID=token;
			}
			else if (!strcmp(fieldName,"APPID"))
			{
				APPID=token;
			}
			else if (!strcmp(fieldName,"LocalID"))
			{
				LocalID=token;
			}
			else if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"SettlementGroup"))
			{
				SettlementGroup=token;
			}
			else if (!strcmp(fieldName,"SettleAction"))
			{
				SettleAction=token;
			}
			else if (!strcmp(fieldName,"Value"))
			{
				Value=token;
			}
			else if (!strcmp(fieldName,"Value1"))
			{
				Value1=token;
			}
			else if (!strcmp(fieldName,"Value2"))
			{
				Value2=token;
			}
			else if (!strcmp(fieldName,"Value3"))
			{
				Value3=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			else if (!strcmp(fieldName,"FundingRateGroup"))
			{
				FundingRateGroup=token;
			}
			else if (!strcmp(fieldName,"SettleSegment"))
			{
				SettleSegment=token;
			}
			else if (!strcmp(fieldName,"InsertTime"))
			{
				InsertTime=token;
			}
			else if (!strcmp(fieldName,"BusinessNo"))
			{
				BusinessNo=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableSettleDetail::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableSettleDetail),output)!=sizeof(CWriteableSettleDetail))
		return 0;
	else
		return 1;
}

int CWriteableSettleDetail::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",SettleDetailID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettleAction.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Value.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Value1.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Value2.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Value3.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",FundingRateGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettleSegment.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableSettleDetail::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",SettleDetailID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",APPID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettlementGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettleAction.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Value.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Value1.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Value2.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Value3.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",FundingRateGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",SettleSegment.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InsertTime.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",BusinessNo.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableSettleDetail::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCSettleDetail%s = {\n", index_buf);
	fprintf(fp,"\t\tSettleDetailID = %s    [操作明细号]\n",SettleDetailID.getString().c_str());	
	fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());	
	fprintf(fp,"\t\tLocalID = %s    [成交对本地标识]\n",LocalID.getString().c_str());	
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());	
	fprintf(fp,"\t\tSettleAction = %s    [结算操作]\n",SettleAction.getString().c_str());	
	fprintf(fp,"\t\tValue = %s    [操作使用值]\n",Value.getString().c_str());	
	fprintf(fp,"\t\tValue1 = %s    [发生值1]\n",Value1.getString().c_str());	
	fprintf(fp,"\t\tValue2 = %s    [发生值2]\n",Value2.getString().c_str());	
	fprintf(fp,"\t\tValue3 = %s    [发生值3]\n",Value3.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t\tFundingRateGroup = %s    [资金费用组]\n",FundingRateGroup.getString().c_str());	
	fprintf(fp,"\t\tSettleSegment = %s    [结算段]\n",SettleSegment.getString().c_str());	
	fprintf(fp,"\t\tInsertTime = %s    [创建时间]\n",InsertTime.getString().c_str());	
	fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableSettleDetail::dumpDiff(FILE *fp, const CWriteableSettleDetail *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCSettleDetail = {\n");
	if (compare(SettleDetailID.getValue(),pOriginal->SettleDetailID.getValue())!=0)
		fprintf(fp,"\t\tSettleDetailID = %s -> %s    [操作明细号]\n",pOriginal->SettleDetailID.getString().c_str(),SettleDetailID.getString().c_str());
	else
		fprintf(fp,"\t\tSettleDetailID = %s    [操作明细号]\n",SettleDetailID.getString().c_str());
	if (compare(APPID.getValue(),pOriginal->APPID.getValue())!=0)
		fprintf(fp,"\t\tAPPID = %s -> %s    [应用编号]\n",pOriginal->APPID.getString().c_str(),APPID.getString().c_str());
	else
		fprintf(fp,"\t\tAPPID = %s    [应用编号]\n",APPID.getString().c_str());
	if (compare(LocalID.getValue(),pOriginal->LocalID.getValue())!=0)
		fprintf(fp,"\t\tLocalID = %s -> %s    [成交对本地标识]\n",pOriginal->LocalID.getString().c_str(),LocalID.getString().c_str());
	else
		fprintf(fp,"\t\tLocalID = %s    [成交对本地标识]\n",LocalID.getString().c_str());
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(SettlementGroup.getValue(),pOriginal->SettlementGroup.getValue())!=0)
		fprintf(fp,"\t\tSettlementGroup = %s -> %s    [结算组编号]\n",pOriginal->SettlementGroup.getString().c_str(),SettlementGroup.getString().c_str());
	else
		fprintf(fp,"\t\tSettlementGroup = %s    [结算组编号]\n",SettlementGroup.getString().c_str());
	if (compare(SettleAction.getValue(),pOriginal->SettleAction.getValue())!=0)
		fprintf(fp,"\t\tSettleAction = %s -> %s    [结算操作]\n",pOriginal->SettleAction.getString().c_str(),SettleAction.getString().c_str());
	else
		fprintf(fp,"\t\tSettleAction = %s    [结算操作]\n",SettleAction.getString().c_str());
	if (compare(Value.getValue(),pOriginal->Value.getValue())!=0)
		fprintf(fp,"\t\tValue = %s -> %s    [操作使用值]\n",pOriginal->Value.getString().c_str(),Value.getString().c_str());
	else
		fprintf(fp,"\t\tValue = %s    [操作使用值]\n",Value.getString().c_str());
	if (compare(Value1.getValue(),pOriginal->Value1.getValue())!=0)
		fprintf(fp,"\t\tValue1 = %s -> %s    [发生值1]\n",pOriginal->Value1.getString().c_str(),Value1.getString().c_str());
	else
		fprintf(fp,"\t\tValue1 = %s    [发生值1]\n",Value1.getString().c_str());
	if (compare(Value2.getValue(),pOriginal->Value2.getValue())!=0)
		fprintf(fp,"\t\tValue2 = %s -> %s    [发生值2]\n",pOriginal->Value2.getString().c_str(),Value2.getString().c_str());
	else
		fprintf(fp,"\t\tValue2 = %s    [发生值2]\n",Value2.getString().c_str());
	if (compare(Value3.getValue(),pOriginal->Value3.getValue())!=0)
		fprintf(fp,"\t\tValue3 = %s -> %s    [发生值3]\n",pOriginal->Value3.getString().c_str(),Value3.getString().c_str());
	else
		fprintf(fp,"\t\tValue3 = %s    [发生值3]\n",Value3.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	if (compare(FundingRateGroup.getValue(),pOriginal->FundingRateGroup.getValue())!=0)
		fprintf(fp,"\t\tFundingRateGroup = %s -> %s    [资金费用组]\n",pOriginal->FundingRateGroup.getString().c_str(),FundingRateGroup.getString().c_str());
	else
		fprintf(fp,"\t\tFundingRateGroup = %s    [资金费用组]\n",FundingRateGroup.getString().c_str());
	if (compare(SettleSegment.getValue(),pOriginal->SettleSegment.getValue())!=0)
		fprintf(fp,"\t\tSettleSegment = %s -> %s    [结算段]\n",pOriginal->SettleSegment.getString().c_str(),SettleSegment.getString().c_str());
	else
		fprintf(fp,"\t\tSettleSegment = %s    [结算段]\n",SettleSegment.getString().c_str());
	if (compare(InsertTime.getValue(),pOriginal->InsertTime.getValue())!=0)
		fprintf(fp,"\t\tInsertTime = %s -> %s    [创建时间]\n",pOriginal->InsertTime.getString().c_str(),InsertTime.getString().c_str());
	else
		fprintf(fp,"\t\tInsertTime = %s    [创建时间]\n",InsertTime.getString().c_str());
	if (compare(BusinessNo.getValue(),pOriginal->BusinessNo.getValue())!=0)
		fprintf(fp,"\t\tBusinessNo = %s -> %s    [业务序列号]\n",pOriginal->BusinessNo.getString().c_str(),BusinessNo.getString().c_str());
	else
		fprintf(fp,"\t\tBusinessNo = %s    [业务序列号]\n",BusinessNo.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableSettleDetail::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",SettleDetailID=%s",SettleDetailID.getString().c_str());	
	fprintf(fp,",APPID=%s",APPID.getString().c_str());	
	fprintf(fp,",LocalID=%s",LocalID.getString().c_str());	
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",SettlementGroup=%s",SettlementGroup.getString().c_str());	
	fprintf(fp,",SettleAction=%s",SettleAction.getString().c_str());	
	fprintf(fp,",Value=%s",Value.getString().c_str());	
	fprintf(fp,",Value1=%s",Value1.getString().c_str());	
	fprintf(fp,",Value2=%s",Value2.getString().c_str());	
	fprintf(fp,",Value3=%s",Value3.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,",FundingRateGroup=%s",FundingRateGroup.getString().c_str());	
	fprintf(fp,",SettleSegment=%s",SettleSegment.getString().c_str());	
	fprintf(fp,",InsertTime=%s",InsertTime.getString().c_str());	
	fprintf(fp,",BusinessNo=%s",BusinessNo.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableSettleDetail::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"SettleDetailID=%s,",SettleDetailID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"APPID=%s,",APPID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LocalID=%s,",LocalID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettlementGroup=%s,",SettlementGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettleAction=%s,",SettleAction.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Value=%s,",Value.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Value1=%s,",Value1.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Value2=%s,",Value2.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Value3=%s,",Value3.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	sprintf(p,"FundingRateGroup=%s,",FundingRateGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"SettleSegment=%s,",SettleSegment.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InsertTime=%s,",InsertTime.getString().c_str());
	p+=strlen(p);
	sprintf(p,"BusinessNo=%s,",BusinessNo.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}

void CSettleDetailIterator::dump(FILE *output)
{
	CSettleDetail *pSettleDetail;
	while ((pSettleDetail=next())!=NULL)
	{
		pSettleDetail->dump(output);
	}
}



void CWriteableOrderAction::writeCSVHead(FILE *output)
{
	fprintf(output,"\"OrderID\",\"OrderLocalID\",\"ActionFlag\",\"LocalID\",\"MemberID\",\"AccountID\",\"Price\",\"Volume\",\"Direction\",\"VolumeDisplay\",\"ProductGroup\",\"ExchangeID\",\"InstrumentID\",\"Cost\",\"Remark\"\n");
}

void CWriteableOrderAction::writeCSVHeadPreVersion(FILE *output)
{
	fprintf(output,"\"OrderID\",\"OrderLocalID\",\"ActionFlag\",\"LocalID\",\"MemberID\",\"AccountID\",\"Price\",\"Volume\",\"Direction\",\"VolumeDisplay\",\"ProductGroup\",\"ExchangeID\",\"InstrumentID\",\"Cost\",\"Remark\"\n");
}
	
void CWriteableOrderAction::init()
{
	//初始化的时候已经clear了，这里只需要对notnull的init一下
	OrderID.clear();
	OrderLocalID.clear();
	ActionFlag.init();
	LocalID.clear();
	MemberID.init();
	AccountID.init();
	Price.clear();
	Volume.clear();
	Direction.init();
	VolumeDisplay.clear();
	ProductGroup.clear();
	ExchangeID.clear();
	InstrumentID.clear();
	Cost.clear();
	Remark.clear();
}

void CWriteableOrderAction::init(CMDB *pMDB)
{
	m_pMDB = pMDB;
	init();
}

int CWriteableOrderAction::read(FILE *input) 
{
	if (fread(this,1,sizeof(CWriteableOrderAction),input)!=sizeof(CWriteableOrderAction))
		return 0;
	else
		return 1;
}
	
int CWriteableOrderAction::readCSV(FILE *input,vector<char *> *pNames)
{
	char buffer[1025];
	char *token;
	if (fgets(buffer,1024,input)==NULL)
		return 0;
	if (pNames==NULL)
	{
		token=getNextToken(buffer);
		if (token!=NULL)
			OrderID=token;
		else
			OrderID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			OrderLocalID=token;
		else
			OrderLocalID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ActionFlag=token;
		else
			ActionFlag="";
		token=getNextToken(NULL);
		if (token!=NULL)
			LocalID=token;
		else
			LocalID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			MemberID=token;
		else
			MemberID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			AccountID=token;
		else
			AccountID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Price=token;
		else
			Price="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Volume=token;
		else
			Volume="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Direction=token;
		else
			Direction="";
		token=getNextToken(NULL);
		if (token!=NULL)
			VolumeDisplay=token;
		else
			VolumeDisplay="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ProductGroup=token;
		else
			ProductGroup="";
		token=getNextToken(NULL);
		if (token!=NULL)
			ExchangeID=token;
		else
			ExchangeID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			InstrumentID=token;
		else
			InstrumentID="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Cost=token;
		else
			Cost="";
		token=getNextToken(NULL);
		if (token!=NULL)
			Remark=token;
		else
			Remark="";
	}
	else
	{
		init();
		token=getNextToken(buffer);
		int i=0;
		while (token!=NULL)
		{
			char *fieldName;
			fieldName=pNames->at(i);
			if (fieldName==NULL)
			{
				break;
			}
			if (!strcmp(fieldName,"OrderID"))
			{
				OrderID=token;
			}
			else if (!strcmp(fieldName,"OrderLocalID"))
			{
				OrderLocalID=token;
			}
			else if (!strcmp(fieldName,"ActionFlag"))
			{
				ActionFlag=token;
			}
			else if (!strcmp(fieldName,"LocalID"))
			{
				LocalID=token;
			}
			else if (!strcmp(fieldName,"MemberID"))
			{
				MemberID=token;
			}
			else if (!strcmp(fieldName,"AccountID"))
			{
				AccountID=token;
			}
			else if (!strcmp(fieldName,"Price"))
			{
				Price=token;
			}
			else if (!strcmp(fieldName,"Volume"))
			{
				Volume=token;
			}
			else if (!strcmp(fieldName,"Direction"))
			{
				Direction=token;
			}
			else if (!strcmp(fieldName,"VolumeDisplay"))
			{
				VolumeDisplay=token;
			}
			else if (!strcmp(fieldName,"ProductGroup"))
			{
				ProductGroup=token;
			}
			else if (!strcmp(fieldName,"ExchangeID"))
			{
				ExchangeID=token;
			}
			else if (!strcmp(fieldName,"InstrumentID"))
			{
				InstrumentID=token;
			}
			else if (!strcmp(fieldName,"Cost"))
			{
				Cost=token;
			}
			else if (!strcmp(fieldName,"Remark"))
			{
				Remark=token;
			}
			i++;
			token=getNextToken(NULL);
		}
	}
	return 1;
}
	
int CWriteableOrderAction::write(FILE *output) const
{
	if (fwrite(this,1,sizeof(CWriteableOrderAction),output)!=sizeof(CWriteableOrderAction))
		return 0;
	else
		return 1;
}

int CWriteableOrderAction::writeCSV(FILE *output) const
{
	fprintf(output,"\"%s\"",OrderID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderLocalID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ActionFlag.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeDisplay.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ProductGroup.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Cost.getAllString().c_str());
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());
	fprintf(output,"\n");
	return 1;
}

int CWriteableOrderAction::writeCSVPreVersion(FILE *output) const
{
	fprintf(output,"\"%s\"",OrderID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",OrderLocalID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ActionFlag.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",LocalID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",MemberID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",AccountID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Price.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Volume.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Direction.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",VolumeDisplay.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ProductGroup.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",ExchangeID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",InstrumentID.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Cost.getAllString().c_str());	
	fprintf(output,",");
	fprintf(output,"\"%s\"",Remark.getAllString().c_str());	
	fprintf(output,"\n");
	return 1;
}

void CWriteableOrderAction::dump(FILE *fp,int index) const
{
	char index_buf[20];
	if(fp==NULL)
		fp=stdout;
	if (index>=0)
		sprintf(index_buf,"[%d]",index);
	else
		index_buf[0]='\0';
	fprintf(fp, "\tCOrderAction%s = {\n", index_buf);
	fprintf(fp,"\t\tOrderID = %s    [报单系统唯一代码]\n",OrderID.getString().c_str());	
	fprintf(fp,"\t\tOrderLocalID = %s    [报单本地标识]\n",OrderLocalID.getString().c_str());	
	fprintf(fp,"\t\tActionFlag = %s    [报单操作标志]\n",ActionFlag.getString().c_str());	
	fprintf(fp,"\t\tLocalID = %s    [操作本地标识]\n",LocalID.getString().c_str());	
	fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());	
	fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());	
	fprintf(fp,"\t\tPrice = %s    [价格]\n",Price.getString().c_str());	
	fprintf(fp,"\t\tVolume = %s    [数量变化]\n",Volume.getString().c_str());	
	fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());	
	fprintf(fp,"\t\tVolumeDisplay = %s    [显示数量]\n",VolumeDisplay.getString().c_str());	
	fprintf(fp,"\t\tProductGroup = %s    [产品组]\n",ProductGroup.getString().c_str());	
	fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());	
	fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());	
	fprintf(fp,"\t\tCost = %s    [委托额]\n",Cost.getString().c_str());	
	fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());	
	fprintf(fp,"\t}\n");
}

void CWriteableOrderAction::dumpDiff(FILE *fp, const CWriteableOrderAction *pOriginal) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp, "\tCOrderAction = {\n");
	if (compare(OrderID.getValue(),pOriginal->OrderID.getValue())!=0)
		fprintf(fp,"\t\tOrderID = %s -> %s    [报单系统唯一代码]\n",pOriginal->OrderID.getString().c_str(),OrderID.getString().c_str());
	else
		fprintf(fp,"\t\tOrderID = %s    [报单系统唯一代码]\n",OrderID.getString().c_str());
	if (compare(OrderLocalID.getValue(),pOriginal->OrderLocalID.getValue())!=0)
		fprintf(fp,"\t\tOrderLocalID = %s -> %s    [报单本地标识]\n",pOriginal->OrderLocalID.getString().c_str(),OrderLocalID.getString().c_str());
	else
		fprintf(fp,"\t\tOrderLocalID = %s    [报单本地标识]\n",OrderLocalID.getString().c_str());
	if (compare(ActionFlag.getValue(),pOriginal->ActionFlag.getValue())!=0)
		fprintf(fp,"\t\tActionFlag = %s -> %s    [报单操作标志]\n",pOriginal->ActionFlag.getString().c_str(),ActionFlag.getString().c_str());
	else
		fprintf(fp,"\t\tActionFlag = %s    [报单操作标志]\n",ActionFlag.getString().c_str());
	if (compare(LocalID.getValue(),pOriginal->LocalID.getValue())!=0)
		fprintf(fp,"\t\tLocalID = %s -> %s    [操作本地标识]\n",pOriginal->LocalID.getString().c_str(),LocalID.getString().c_str());
	else
		fprintf(fp,"\t\tLocalID = %s    [操作本地标识]\n",LocalID.getString().c_str());
	if (compare(MemberID.getValue(),pOriginal->MemberID.getValue())!=0)
		fprintf(fp,"\t\tMemberID = %s -> %s    [成员代码]\n",pOriginal->MemberID.getString().c_str(),MemberID.getString().c_str());
	else
		fprintf(fp,"\t\tMemberID = %s    [成员代码]\n",MemberID.getString().c_str());
	if (compare(AccountID.getValue(),pOriginal->AccountID.getValue())!=0)
		fprintf(fp,"\t\tAccountID = %s -> %s    [资金账号]\n",pOriginal->AccountID.getString().c_str(),AccountID.getString().c_str());
	else
		fprintf(fp,"\t\tAccountID = %s    [资金账号]\n",AccountID.getString().c_str());
	if (compare(Price.getValue(),pOriginal->Price.getValue())!=0)
		fprintf(fp,"\t\tPrice = %s -> %s    [价格]\n",pOriginal->Price.getString().c_str(),Price.getString().c_str());
	else
		fprintf(fp,"\t\tPrice = %s    [价格]\n",Price.getString().c_str());
	if (compare(Volume.getValue(),pOriginal->Volume.getValue())!=0)
		fprintf(fp,"\t\tVolume = %s -> %s    [数量变化]\n",pOriginal->Volume.getString().c_str(),Volume.getString().c_str());
	else
		fprintf(fp,"\t\tVolume = %s    [数量变化]\n",Volume.getString().c_str());
	if (compare(Direction.getValue(),pOriginal->Direction.getValue())!=0)
		fprintf(fp,"\t\tDirection = %s -> %s    [买卖方向]\n",pOriginal->Direction.getString().c_str(),Direction.getString().c_str());
	else
		fprintf(fp,"\t\tDirection = %s    [买卖方向]\n",Direction.getString().c_str());
	if (compare(VolumeDisplay.getValue(),pOriginal->VolumeDisplay.getValue())!=0)
		fprintf(fp,"\t\tVolumeDisplay = %s -> %s    [显示数量]\n",pOriginal->VolumeDisplay.getString().c_str(),VolumeDisplay.getString().c_str());
	else
		fprintf(fp,"\t\tVolumeDisplay = %s    [显示数量]\n",VolumeDisplay.getString().c_str());
	if (compare(ProductGroup.getValue(),pOriginal->ProductGroup.getValue())!=0)
		fprintf(fp,"\t\tProductGroup = %s -> %s    [产品组]\n",pOriginal->ProductGroup.getString().c_str(),ProductGroup.getString().c_str());
	else
		fprintf(fp,"\t\tProductGroup = %s    [产品组]\n",ProductGroup.getString().c_str());
	if (compare(ExchangeID.getValue(),pOriginal->ExchangeID.getValue())!=0)
		fprintf(fp,"\t\tExchangeID = %s -> %s    [交易所代码]\n",pOriginal->ExchangeID.getString().c_str(),ExchangeID.getString().c_str());
	else
		fprintf(fp,"\t\tExchangeID = %s    [交易所代码]\n",ExchangeID.getString().c_str());
	if (compare(InstrumentID.getValue(),pOriginal->InstrumentID.getValue())!=0)
		fprintf(fp,"\t\tInstrumentID = %s -> %s    [标的代码]\n",pOriginal->InstrumentID.getString().c_str(),InstrumentID.getString().c_str());
	else
		fprintf(fp,"\t\tInstrumentID = %s    [标的代码]\n",InstrumentID.getString().c_str());
	if (compare(Cost.getValue(),pOriginal->Cost.getValue())!=0)
		fprintf(fp,"\t\tCost = %s -> %s    [委托额]\n",pOriginal->Cost.getString().c_str(),Cost.getString().c_str());
	else
		fprintf(fp,"\t\tCost = %s    [委托额]\n",Cost.getString().c_str());
	if (compare(Remark.getValue(),pOriginal->Remark.getValue())!=0)
		fprintf(fp,"\t\tRemark = %s -> %s    [备注]\n",pOriginal->Remark.getString().c_str(),Remark.getString().c_str());
	else
		fprintf(fp,"\t\tRemark = %s    [备注]\n",Remark.getString().c_str());
	fprintf(fp,"\t}\n");
}

void CWriteableOrderAction::dumpInLine(FILE *fp) const
{
	if(fp==NULL)
		fp=stdout;
	fprintf(fp,",OrderID=%s",OrderID.getString().c_str());	
	fprintf(fp,",OrderLocalID=%s",OrderLocalID.getString().c_str());	
	fprintf(fp,",ActionFlag=%s",ActionFlag.getString().c_str());	
	fprintf(fp,",LocalID=%s",LocalID.getString().c_str());	
	fprintf(fp,",MemberID=%s",MemberID.getString().c_str());	
	fprintf(fp,",AccountID=%s",AccountID.getString().c_str());	
	fprintf(fp,",Price=%s",Price.getString().c_str());	
	fprintf(fp,",Volume=%s",Volume.getString().c_str());	
	fprintf(fp,",Direction=%s",Direction.getString().c_str());	
	fprintf(fp,",VolumeDisplay=%s",VolumeDisplay.getString().c_str());	
	fprintf(fp,",ProductGroup=%s",ProductGroup.getString().c_str());	
	fprintf(fp,",ExchangeID=%s",ExchangeID.getString().c_str());	
	fprintf(fp,",InstrumentID=%s",InstrumentID.getString().c_str());	
	fprintf(fp,",Cost=%s",Cost.getString().c_str());	
	fprintf(fp,",Remark=%s",Remark.getString().c_str());	
	fprintf(fp,"\n");
}

void CWriteableOrderAction::dumpString(char *target) const
{
	char *p=target;
	sprintf(p,"OrderID=%s,",OrderID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"OrderLocalID=%s,",OrderLocalID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ActionFlag=%s,",ActionFlag.getString().c_str());
	p+=strlen(p);
	sprintf(p,"LocalID=%s,",LocalID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"MemberID=%s,",MemberID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"AccountID=%s,",AccountID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Price=%s,",Price.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Volume=%s,",Volume.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Direction=%s,",Direction.getString().c_str());
	p+=strlen(p);
	sprintf(p,"VolumeDisplay=%s,",VolumeDisplay.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ProductGroup=%s,",ProductGroup.getString().c_str());
	p+=strlen(p);
	sprintf(p,"ExchangeID=%s,",ExchangeID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"InstrumentID=%s,",InstrumentID.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Cost=%s,",Cost.getString().c_str());
	p+=strlen(p);
	sprintf(p,"Remark=%s,",Remark.getString().c_str());
	p+=strlen(p);
	p--;
	*p='\0';
}


const CWriteableOrder *CWriteableOrderAction::linkOrder() const
{
	void *target=(void *)(&pOrder);
	*((const CWriteableOrder **)target)=m_pMDB->m_OrderFactory->findByOrderID(OrderID);
	return pOrder;
}

const CWriteableTriggerOrder *CWriteableOrderAction::linkTriggerOrder() const
{
	void *target=(void *)(&pTriggerOrder);
	*((const CWriteableTriggerOrder **)target)=m_pMDB->m_TriggerOrderFactory->findByOrderID(OrderID);
	return pTriggerOrder;
}

const CWriteableMember *CWriteableOrderAction::linkMember() const
{
	void *target=(void *)(&pMember);
	*((const CWriteableMember **)target)=m_pMDB->m_MemberFactory->findByMemberID(MemberID);
	return pMember;
}


