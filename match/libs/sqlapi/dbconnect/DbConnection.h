#ifndef _DBCONNECTION_H__
#define _DBCONNECTION_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "SQLAPI.h"
#include "FieldDescribe.h"

class CDbConnection : public SAConnection
{
public:
	CDbConnection();
	CDbConnection(string& dbName, string& userName, string& pwd);
	virtual ~CDbConnection();
	bool ConnectDB(const char *pDatabaseName, const char *pUserName, const char *pPassword);
	bool ConnectDB();
	bool KeepAlive();
private:

private:
	string m_strDatabaseName;
	string m_strUserName;
	string m_strPassword;
	eSAClient m_saClient;
};

#endif
