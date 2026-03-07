#ifndef __SMTP_CLIENT_H__
#define __SMTP_CLIENT_H__
#include <string>

using std::string;
class CSMTPClient
{
public:
	CSMTPClient(const char* url, const char* user, const char* password);

	void SendEmail(const char* from, const char* to, const char* subject, const char* body);

private:
	string m_strUser;
	string m_strPassword;
	string m_strUrl;

};


#endif // __SMTP_CLIENT_H__