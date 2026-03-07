#ifndef __DB_HELPERE_H__
#define __DB_HELPERE_H__
#include "DbConnection.h"

enum TDBActionType
{
	TDB_INSERT,
	TDB_UPDATE,
	TDB_DELETE,
	TDB_COMMIT,
};

enum TDBMethodType
{
	TDB_TABLE,
	TDB_SP
};

const int MAX_DB_COMMAND_LEN = 4000;

#define TASK_METHOD_DEFINE(field, MethodType, ObjectName, KeyName, DataContainer)		\
{ &field::m_Describe, MethodType, ObjectName, KeyName, DataContainer, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }}

#define TASK_METHOD_END															\
{ NULL, TDB_TABLE, "", "", NULL, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }}

class CBaseDataContainer
{
public:
	virtual void* add() = 0;
	virtual void clear() = 0;
	virtual void* get(int idx) = 0;
	virtual size_t size() = 0;
	virtual void set_mode(int m) = 0;
protected:
	virtual ~CBaseDataContainer() {}
};

template <typename T>
class CDataContainer : public CBaseDataContainer
{
public:
	CDataContainer() : mode(0)
	{
	}

	virtual void* add()
	{
		T data;
		vct.push_back(data);
		return (void*) & (vct[vct.size() - 1]);
	}

	virtual void clear()
	{
		//vector<T>().swap(vct);
		vct.clear();
	}

	virtual void* get(int idx)
	{
		if (idx < 0 || idx >= vct.size())
			return NULL;

		if (1 == mode)
			return (void*)&vct[vct.size() - idx - 1];

		return (void*)& vct[idx];
	}

	virtual size_t size()
	{
		return vct.size();
	}

	virtual void set_mode(int m)
	{
		mode = m;
	}

private:
	vector<T> vct;
	int mode;
};

class CElementContainer
{
public:
	virtual string GetElementJson(int idx) = 0;
	virtual size_t Size() = 0;

protected:
	virtual ~CElementContainer() {}
};


template <typename T>
class CXTPElementContainer : public CElementContainer
{
public:
	CXTPElementContainer(CBaseDataContainer* pDataContainer)
		: m_pDataContainer(pDataContainer)
	{
	}

	// Í¨¹ý CElementContainer ¼Ì³Ð
	virtual string GetElementJson(int idx) override
	{
		T* pData = (T*)m_pDataContainer->get(idx);
		if (NULL == pData)
			return "";

		char buffer[10240] = {};
		string json = "{";
		pData->m_Describe.StructToJson(buffer, (char*)pData);
		json += buffer;
		json += "}";
		return json;
	}

	virtual size_t Size() override
	{
		return m_pDataContainer->size();
	}

private:
	CBaseDataContainer* m_pDataContainer;
};

template <typename T>
class CMapElementContainer : public CElementContainer
{
public:
	CMapElementContainer(T* pData) :m_pData(pData)
	{
	}

	virtual string GetElementJson(int idx) override
	{
		int i = 0;
		for (typename T::iterator it = m_pData->begin(); it != m_pData->end(); it++, i++)
		{
			if (i == idx)
				return it->second;
		}
		return "";
	}

	virtual size_t Size() override
	{
		return m_pData->size();
	}

private:
	T* m_pData;
};

class CArryElementContainer : public CElementContainer
{
public:
	CArryElementContainer(vector<string>* pData)
		: m_pData(pData)
	{

	}

	virtual string GetElementJson(int idx) override
	{
		if (idx < 0 || idx >= m_pData->size())
			return "";

		return (*m_pData)[idx];
	}

	virtual size_t Size() override
	{
		return m_pData->size();
	}

private:
	vector<string>* m_pData;
};

struct TDBMethod
{
	CFieldDescribe* pDescirbe;
	TDBMethodType nMethodType;
	const char* pszObjectName;
	const char* pszKeyName;
	CBaseDataContainer* pDataContainer;
	char szInsertCommand[MAX_DB_COMMAND_LEN];
	char szUpdateCommand[MAX_DB_COMMAND_LEN];
	char szDeleteCommand[MAX_DB_COMMAND_LEN];
	char szSelectCommand[MAX_DB_COMMAND_LEN];
	char szQryCountCommand[MAX_DB_COMMAND_LEN];
	int keyPosition[10];
};

extern TDBMethod g_HelperTaskMethod[];

class CDbHelper
{
public:
	CDbHelper(string& dbName, string& userName, string& pwd);
	virtual ~CDbHelper();

	bool ConnectDB();
	CBaseDataContainer* Select(const char* pszObjectName, const char* szFilter);
	void PrepareSelect(const char* pszObjectName);
	void AppendSelectParam(const char* szParamName, const char* szParamValue, const char* szOper = "=");
	CBaseDataContainer* ExecSelectCmd(const char* tail);

	int QryCount(const char* pszObjectName, const char* szFilter);
	TDBMethod* FindMethod(const char* pszObjectName);
private:
	void PrepareTaskMethod(TDBMethod* pTaskMethod);
	void SetValue(TMemberDesc* pMemberDesc, char* pData, SACommand* pSACommand);
	bool CheckDbConnectionError(int nErrorNativeCode);

private:
	SACommand* m_pSACommand;
	CDbConnection* m_pDbConnection;
	time_t m_lastAction;
	string m_strSelectCommand;
	int m_nParamID;
	map<int, string> m_parmValues;
	TDBMethod* m_pTaskMethod;
};

#endif // __DB_HELPERE_H__
