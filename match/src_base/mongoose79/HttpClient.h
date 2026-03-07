#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include <string>

using std::string;

class CHttpClient
{
public:
	CHttpClient();

	bool Post(const char* func, const char* data, const char* header, char* respData, int respDataLen);
	bool Put(const char* func, const char* data, const char* header, char* respData, int respDataLen);
	bool Delete(const char* func, const char* data, const char* header, char* respData, int respDataLen);
	bool Get(const char* func, const char* data, const char* header, char* respData, int respDataLen);

	void SetBaseUrl(const char* url) { m_strBaseUrl = url; }

	string& GetBaseUrl() { return m_strBaseUrl; }

private:
	bool Request(const char* method, const char* func, const char* data, const char* header, char* respData, int respDataLen);

private:
	string m_strBaseUrl;
};

#endif // __HTTP_CLIENT_H__