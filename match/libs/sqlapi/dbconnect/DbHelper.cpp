#include "DbHelper.h"
#include "CSVParser.h"
#include "public.h"
#include "utility.h"

CDbHelper::CDbHelper(string& dbName, string& userName, string& pwd)
	: m_lastAction(0)
	, m_nParamID(0)
	, m_pTaskMethod(NULL)
{
	m_pDbConnection = new CDbConnection(dbName, userName, pwd);
	m_pSACommand = new SACommand(m_pDbConnection);

	TDBMethod* pTaskMethod = g_HelperTaskMethod;
	while (pTaskMethod->pDescirbe != NULL)
	{
		PrepareTaskMethod(pTaskMethod);
		pTaskMethod++;
	}
}

CDbHelper::~CDbHelper()
{
	delete m_pSACommand;
	delete m_pDbConnection;
}

bool CDbHelper::ConnectDB()
{
	return m_pDbConnection->ConnectDB();
}

void CDbHelper::PrepareTaskMethod(TDBMethod* pTaskMethod)
{
	if (pTaskMethod->nMethodType == TDB_SP)
		return;

	int i = 0;
	int* pKeyPosition = pTaskMethod->keyPosition;

	//prepare key position
	CCSVParser csvParser(pTaskMethod->pszKeyName);
	do {
		char* pToken = csvParser.GetNextToken();
		if (pToken != NULL)
		{
			for (i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
			{
				if (strcmp(pToken, pTaskMethod->pDescirbe->GetMemberDesc(i)->szName) == 0)
				{
					*pKeyPosition = i + 1;
					pKeyPosition++;
					break;
				}
			}
			if (i == pTaskMethod->pDescirbe->GetMemberCount())
				EMERGENCY_EXIT("Can't find key name");
		}
	} while (csvParser.GetErrorCode() == CPE_HAS_NEXT);
	*pKeyPosition = 0;		//结束符

	char strSQL[MAX_DB_COMMAND_LEN];
	//prepare insert statement
	strcpy(strSQL, "REPLACE INTO ");
	strcat(strSQL, pTaskMethod->pszObjectName);
	strcat(strSQL, " (");
	for (i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
	{
		if (i != 0)
			strcat(strSQL, ", ");
		strcat(strSQL, pTaskMethod->pDescirbe->GetMemberDesc(i)->szName);
	}
	strcat(strSQL, ") VALUES (");
	for (i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
	{
		if (i != 0)
			strcat(strSQL, ", ");
		sprintf(strSQL + strlen(strSQL), ":%d", i + 1);
	}
	strcat(strSQL, ")");
	strcpy(pTaskMethod->szInsertCommand, strSQL);
	PRINT_TO_STD("SQL: %s", strSQL);

	//prepare update statement
	strcpy(strSQL, "UPDATE ");
	strcat(strSQL, pTaskMethod->pszObjectName);
	strcat(strSQL, " SET ");
	for (i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
	{
		if (i != 0)
			strcat(strSQL, ", ");
		strcat(strSQL, pTaskMethod->pDescirbe->GetMemberDesc(i)->szName);
		sprintf(strSQL + strlen(strSQL), " = :%d", i + 1);
	}
	strcat(strSQL, " WHERE ");

	i = 0;
	pKeyPosition = pTaskMethod->keyPosition;
	while (*pKeyPosition > 0)
	{
		if (i != 0)
			strcat(strSQL, " AND ");
		sprintf(strSQL + strlen(strSQL), "%s=:%d",
			pTaskMethod->pDescirbe->GetMemberDesc(*pKeyPosition - 1)->szName,
			i + pTaskMethod->pDescirbe->GetMemberCount() + 1);
		i++;
		pKeyPosition++;
	}
	strcpy(pTaskMethod->szUpdateCommand, strSQL);
	PRINT_TO_STD("SQL: %s", strSQL);

	//prepare delete statement
	strcpy(strSQL, "DELETE FROM ");
	strcat(strSQL, pTaskMethod->pszObjectName);
	strcat(strSQL, " WHERE ");

	i = 0;
	pKeyPosition = pTaskMethod->keyPosition;
	while (*pKeyPosition > 0)
	{
		if (i != 0)
			strcat(strSQL, " AND ");
		sprintf(strSQL + strlen(strSQL), "%s=:%d", pTaskMethod->pDescirbe->GetMemberDesc(*pKeyPosition - 1)->szName, i + 1);
		i++;
		pKeyPosition++;
	}
	strcpy(pTaskMethod->szDeleteCommand, strSQL);
	PRINT_TO_STD("SQL: %s", strSQL);

	//prepare select statement
	strcpy(strSQL, "SELECT ");
	for (i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
	{
		if (i != 0)
			strcat(strSQL, ", ");
		strcat(strSQL, pTaskMethod->pDescirbe->GetMemberDesc(i)->szName);
	}
	strcat(strSQL, " FROM ");
	strcat(strSQL, pTaskMethod->pszObjectName);

	strcpy(pTaskMethod->szSelectCommand, strSQL);
	PRINT_TO_STD("SQL: %s", strSQL);

	//prepare select count
	strcpy(strSQL, "SELECT COUNT(1) as cnt FROM ");
	strcat(strSQL, pTaskMethod->pszObjectName);

	strcpy(pTaskMethod->szQryCountCommand, strSQL);
	PRINT_TO_STD("SQL: %s", strSQL);
}

TDBMethod* CDbHelper::FindMethod(const char* pszObjectName)
{
	TDBMethod* pTaskMethod = g_HelperTaskMethod;
	while (pTaskMethod->pDescirbe != NULL)
	{
		if (0 == strcmp(pTaskMethod->pszObjectName, pszObjectName))
			return pTaskMethod;
		pTaskMethod++;
	}
	return NULL;
}


void CDbHelper::SetValue(TMemberDesc* pMemberDesc, char* pData, SACommand* pSACommand)
{
	char* pMemberData = pData + pMemberDesc->nStructOffset;
	switch (pMemberDesc->nType) {
	case FT_DWORD:						//long
		if (pSACommand->Field(pMemberDesc->szName).isNull())
			*((int*)pMemberData) = INT_MAX;
		else
			*((int*)pMemberData) = pSACommand->Field(pMemberDesc->szName).asLong();
		break;
	case FT_REAL8:						//double
		if (pSACommand->Field(pMemberDesc->szName).isNull())
			*((double*)pMemberData) = DBL_MAX;
		else
			*((double*)pMemberData) = pSACommand->Field(pMemberDesc->szName).asDouble();
		break;
	case FT_LONG:						//double
		if (pSACommand->Field(pMemberDesc->szName).isNull())
			*((double*)pMemberData) = DBL_MAX;
		else
			*((double*)pMemberData) = pSACommand->Field(pMemberDesc->szName).asDouble();
		break;
	case FT_BYTE:
		if (pMemberDesc->nSize == 1)
		{
			if (pSACommand->Field(pMemberDesc->szName).asString().GetLength() > 0)
				*pMemberData = pSACommand->Field(pMemberDesc->szName).asString()[0];
		}
		else
		{
			StrCCopy(pMemberData, pSACommand->Field(pMemberDesc->szName).asString(), pMemberDesc->nSize);
		}
		break;
	}
}

bool CDbHelper::CheckDbConnectionError(int nErrorNativeCode)
{
	const int arrErrorCode[] = {
		0, 12500, 1034, 3113, 3114, 12571, 1033, 1012, 28, 3135, 2006
	};
	for (size_t i = 0; i < sizeof(arrErrorCode) / sizeof(int); i++)
	{
		if (nErrorNativeCode == arrErrorCode[i])
		{
			m_pDbConnection->Disconnect();
			return true;
		}
	}
	return false;
}

CBaseDataContainer* CDbHelper::Select(const char* pszObjectName, const char* szFilter)
{
	/*if (!m_pDbConnection->isConnected())
	{
		m_pDbConnection->ConnectDB();
		PRINT_TO_STD("Connect DB...");
		if (!m_pDbConnection->isConnected())
		{
			PRINT_TO_STD("Connect DB Error");
			return NULL;
		}
	}*/

	TDBMethod* pTaskMethod = FindMethod(pszObjectName);

	if (pTaskMethod == NULL)
		return NULL;

	char buff[MAX_DB_COMMAND_LEN];
	const char* szSelect;
	if (szFilter != NULL && strlen(szFilter) > 0)
	{
		sprintf(buff, "%s WHERE %s", pTaskMethod->szSelectCommand, szFilter);
		szSelect = buff;
	}
	else
	{
		szSelect = pTaskMethod->szSelectCommand;
	}

	int ret = 0;
	for (int nRetryTimes = 2; nRetryTimes > 0; nRetryTimes--)		//重做2次
	{
		try
		{
			SACommand cmd(m_pDbConnection);
			cmd.setCommandText(szSelect, SA_CmdSQLStmt);
			cmd.Execute();

			CBaseDataContainer* pDataContainer = pTaskMethod->pDataContainer;
			pDataContainer->clear();
			while (cmd.FetchNext())
			{
				void* pData = pDataContainer->add();
				for (int i = 0; i < pTaskMethod->pDescirbe->GetMemberCount(); i++)
					SetValue(pTaskMethod->pDescirbe->GetMemberDesc(i), (char*)pData, &cmd);
			}

			nRetryTimes = 0;					// 成功了不需要重做
			ret = 0;
		}
		catch (SAException& x)
		{
			ret = -2;
			int nErrNativeCode = x.ErrNativeCode();
			PRINT_TO_STD("Database Operation Exception, Object=%s ErrNativeCode=%d Message=%s,isConnected:%d,isAlive:%d",
				pszObjectName, nErrNativeCode, (const char*)x.ErrText(), m_pDbConnection->isConnected(), m_pDbConnection->isAlive());
			try
			{
				if (!CheckDbConnectionError(nErrNativeCode))
					nRetryTimes = 0;
				else
				{
					if (!m_pDbConnection->ConnectDB())
						nRetryTimes = 0;
				}
			}
			catch (SAException&)
			{
			}
		}
	}
	m_lastAction = time(NULL);
	if (ret != 0)
	{
		PRINT_TO_STD("Database Operation Failed, Object=%s sql=%s ret=%d", pszObjectName, szSelect, ret);
		return NULL;
	}
	return pTaskMethod->pDataContainer;
}

void CDbHelper::PrepareSelect(const char* pszObjectName)
{
	m_pTaskMethod = FindMethod(pszObjectName);
	if (m_pTaskMethod == NULL)
	{
		PRINT_TO_STD("PrepareSelect can't find method:%s", pszObjectName);
		return;
	}
	m_nParamID = 0;
	m_pSACommand->Reset();
	m_strSelectCommand = m_pTaskMethod->szSelectCommand;
	m_parmValues.clear();
}

string ToString(int v)
{
	char buff[64];
	sprintf(buff, "%d", v);
	return buff;
}

void CDbHelper::AppendSelectParam(const char* szParamName, const char* szParamValue, const char* szOper/* = "="*/)
{
	if (0 == m_nParamID)
		m_strSelectCommand += " WHERE ";
	else
		m_strSelectCommand += " AND ";

	++m_nParamID;
	const char* p;
	if ((p = strstr(szOper, "$Param")) != NULL)
	{
		string s(szOper, p - szOper);
		s += ":";
		s += ToString(m_nParamID);
		s += (p + strlen("$Param"));
		m_strSelectCommand += s;
	}
	else
	{
		m_strSelectCommand += szParamName;
		m_strSelectCommand += szOper;
		m_strSelectCommand += ":";
		m_strSelectCommand += ToString(m_nParamID);
	}

	m_parmValues[m_nParamID] = szParamValue;
	//m_pSACommand->Param(m_nParamID).setAsString() = szParamValue;
}

CBaseDataContainer* CDbHelper::ExecSelectCmd(const char* tail)
{
	if (m_pTaskMethod == NULL)
		return NULL;

	/*if (!m_pDbConnection->isConnected())
	{
		m_pDbConnection->ConnectDB();
		PRINT_TO_STD("Connect DB ...");
		if (!m_pDbConnection->isConnected())
		{
			PRINT_TO_STD("Connect DB Error");
			return NULL;
		}
	}*/

	if (tail != NULL)
	{
		m_strSelectCommand += " ";
		m_strSelectCommand += tail;
	}

	m_pSACommand->setCommandText(m_strSelectCommand.c_str(), SA_CmdSQLStmt);

	for (map<int, string>::iterator it = m_parmValues.begin(); it != m_parmValues.end(); it++)
	{
		m_pSACommand->Param(it->first).setAsString() = it->second.c_str();
	}

	PRINT_TO_STD("SQL:%s", m_strSelectCommand.c_str());
	const char* x = m_pSACommand->CommandText().GetMultiByteChars();

	int ret = 0;
	for (int nRetryTimes = 2; nRetryTimes > 0; nRetryTimes--)		//重做2次
	{
		try
		{
			m_pSACommand->Execute();
			CBaseDataContainer* pDataContainer = m_pTaskMethod->pDataContainer;
			pDataContainer->clear();
			while (m_pSACommand->FetchNext())
			{
				void* pData = pDataContainer->add();
				for (int i = 0; i < m_pTaskMethod->pDescirbe->GetMemberCount(); i++)
					SetValue(m_pTaskMethod->pDescirbe->GetMemberDesc(i), (char*)pData, m_pSACommand);
			}
			nRetryTimes = 0;					// 成功了不需要重做
		}
		catch (SAException& x)
		{
			ret = -2;
			int nErrNativeCode = x.ErrNativeCode();
			PRINT_TO_STD("Database Operation Exception,SQL=%s, ErrNativeCode=%d,Message=%s,isConnected:%d,isAlive:%d",
				m_strSelectCommand.c_str(), nErrNativeCode, (const char*)x.ErrText(), m_pDbConnection->isConnected(), m_pDbConnection->isAlive());
			try
			{
				if (!CheckDbConnectionError(nErrNativeCode))
					nRetryTimes = 0;
				else
				{
					if (!m_pDbConnection->ConnectDB())
						nRetryTimes = 0;
				}
			}
			catch (SAException&)
			{
			}
		}
	}
	if (ret != 0)
		PRINT_TO_STD("Database Operation Failed,sql=%s ret=%d\n", m_strSelectCommand.c_str(), ret);
	m_lastAction = time(NULL);
	return m_pTaskMethod->pDataContainer;
}

int CDbHelper::QryCount(const char* pszObjectName, const char* szFilter)
{
	/*if (!m_pDbConnection->isConnected())
	{
		m_pDbConnection->ConnectDB();
		PRINT_TO_STD("Connect DB...\n");
		if (!m_pDbConnection->isConnected())
		{
			PRINT_TO_STD("Connect DB Error");
			return 0;
		}
	}*/

	TDBMethod* pTaskMethod = FindMethod(pszObjectName);

	if (pTaskMethod == NULL)
	{
		return 0;
	}

	char buff[MAX_DB_COMMAND_LEN];
	const char* szSelect;
	if (szFilter != NULL && strlen(szFilter) > 0)
	{
		sprintf(buff, "%s WHERE %s", pTaskMethod->szQryCountCommand, szFilter);
		szSelect = buff;
	}
	else
	{
		szSelect = pTaskMethod->szQryCountCommand;
	}

	int ret = 0;
	int count = 0;
	for (int nRetryTimes = 2; nRetryTimes > 0; nRetryTimes--)		//重做2次
	{
		try
		{
			SACommand cmd(m_pDbConnection);
			cmd.setCommandText(szSelect, SA_CmdSQLStmt);
			cmd.Execute();
			if (cmd.FetchFirst())
			{
				count = cmd.Field("cnt").asLong();
			}
			else
			{
				PRINT_TO_STD("fetch first fail,SQL:%s", szSelect);
			}

			nRetryTimes = 0;					// 成功了不需要重做
		}
		catch (SAException& x)
		{
			ret = -2;
			int nErrNativeCode = x.ErrNativeCode();
			PRINT_TO_STD("Database Operation Exception, Object=%s ErrNativeCode=%d Message=%s,isConnected:%d,isAlive:%d",
				pszObjectName, nErrNativeCode, (const char*)x.ErrText(), m_pDbConnection->isConnected(), m_pDbConnection->isAlive());
			try
			{
				if (!CheckDbConnectionError(nErrNativeCode))
					nRetryTimes = 0;
				else
				{
					if (!m_pDbConnection->ConnectDB())
						nRetryTimes = 0;
				}
			}
			catch (SAException&)
			{
			}
		}
	}
	if (ret != 0)
	{
		PRINT_TO_STD("Database Operation Failed, SQL=%s ret=%d", szSelect, ret);
	}

	m_lastAction = time(NULL);

	return count;
}
