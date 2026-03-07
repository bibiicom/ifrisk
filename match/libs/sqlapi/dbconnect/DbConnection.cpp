#include "public.h"
#include "DbConnection.h"
#include "BaseFunction.h"

CDbConnection::CDbConnection() {}

CDbConnection::CDbConnection(string& dbName, string& userName, string& pwd)
{
	m_strDatabaseName = dbName;
	m_strUserName = userName;
	m_strPassword = pwd;
}
CDbConnection::~CDbConnection() {}

bool CDbConnection::ConnectDB(const char *pDatabaseName, const char *pUserName, const char *pPassword)
{
	m_strDatabaseName = pDatabaseName;
	m_strUserName = pUserName;
	m_strPassword = pPassword;

	return ConnectDB();
}

bool CDbConnection::ConnectDB()
{
	try
	{
		//this->setOption("CLIENT_MULTI_RESULTS")=true;
		//this->setOption("CLIENT_MULTI_STATEMENTS")=true;

		//建立数据库连接
		//连接格式为 MYSQL://192.168.0.1:3306@database or ORACLE://bizorz 等，先用数据库连接方式作为使用哪个数据库的标示
		//连接格式为 DBTYPE://DBSTRING 先用数据库连接方式作为使用哪个数据库的标示
		vector<string> dbnames = Txtsplit(m_strDatabaseName, "://");
		if (dbnames.size() != 2)
		{
			//如果不是两部分，说明有一些问题
			printf("Database Connect Database Error,DatabaseName not X://Y -> %s \n", m_strDatabaseName.c_str());
			return false;
		}

		if (dbnames[0] == "MYSQL")
		{
			m_saClient = SA_MySQL_Client;
			//连接格式为 MYSQL://192.168.0.1:3306@database
			setOption("CharacterSet") = "gbk";
			printf("Database DATA SOURCE[%s],USER[%s]\n", dbnames[1].c_str(), m_strUserName.c_str());
			Connect(dbnames[1].c_str(), m_strUserName.c_str(), m_strPassword.c_str(), SA_MySQL_Client);
		}
		else if (dbnames[0] == "ORACLE")
		{
			m_saClient = SA_Oracle_Client;
			//连接格式为 ORACLE://bizorz 等
			printf("Database DATA SOURCE[%s],USER[%s]\n", dbnames[1].c_str(), m_strUserName.c_str());
			Connect(dbnames[1].c_str(), m_strUserName.c_str(), m_strPassword.c_str(), SA_Oracle_Client);
		}
		else if (dbnames[0] == "SYBASE")
		{
			//m_saClient = SA_Sybase_Client;
		}
		else if (dbnames[0] == "DB2")
		{
			//m_saClient = SA_DB2_Client;	
		}
		else if (dbnames[0] == "SQLSERVER")
		{
			m_saClient = SA_SQLServer_Client;
#ifdef WIN32
			Connect(dbnames[1].c_str(), m_strUserName.c_str(), m_strPassword.c_str(), SA_SQLServer_Client);
#else
			char tmp[256] = { 0 };
			vector<string> svnames = Txtsplit(dbnames[1], "@");
			if (svnames.size() != 2)
			{
				//如果不是两部分，说明有一些问题
				printf("Database Connect Database Error,Server Name not ip@db -> %s \n", dbnames[1].c_str());
				return false;
			}
			sprintf(tmp, "DRIVER={FreeTDS};SERVER=%s;PORT=1433;DATABASE=%s;TDS_Version=7.4", svnames[0].c_str(), svnames[1].c_str());
			printf("Database DATA SOURCE[%s],USER[%s],PWD[%s]\n", tmp, m_strUserName.c_str(), m_strPassword.c_str());
			Connect(tmp, m_strUserName.c_str(), m_strPassword.c_str(), SA_ODBC_Client);
#endif
		}
		//setAutoCommit(SA_AutoCommitOff);
	}
	catch (SAException &x)
	{
		printf("Database Connect Database Error, errorid=%d, msg=%s \n", x.ErrNativeCode(), (const char*)x.ErrText());
		return false;
	}
	return true;
}

bool CDbConnection::KeepAlive()
{
	char sAliveSql[256];
	switch (m_saClient)
	{
	case SA_MySQL_Client:
	{
		sprintf(sAliveSql, "SELECT 1 FROM DUAL");
		break;
	}
	case SA_Oracle_Client:
	{
		sprintf(sAliveSql, "select * from dual where rownum=1");
		break;
	}
	default:
	{
		PRINT_TO_STD("Database KeepAlive Error No m_saClient");
	}
	}
	SACommand cmd(this, sAliveSql);
	try
	{
		cmd.Execute();
		while (cmd.FetchNext());
		return true;
	}
	catch (SAException &x)
	{
		printf("Database KeepAlive Error, errorid=%d, msg=%s", x.ErrNativeCode(), (const char*)x.ErrText());
		return false;
	}
}