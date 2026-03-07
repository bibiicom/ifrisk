#ifndef __WSCLIENT_H__
#define __WSCLIENT_H__

#include "mongoose.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
typedef HANDLE THREAD_HANDLE;
#else
#include <pthread.h>
typedef pthread_t THREAD_HANDLE;
#endif

using std::string;

class CWSClientSpi
{
public:
	virtual void OnWSConnected() {}
	virtual void OnWSDisConnected() {}
	virtual void OnWSClosed() {}
	virtual void OnPoll() {}
	virtual void OnData(const char* data, size_t length) {}

protected:
	~CWSClientSpi() {}
};

class CWSClient
{
public:
	CWSClient();
	~CWSClient();

	void SetUrl(const string& url);
	bool Start();
	void Close();


	void RegisterSpi(CWSClientSpi* pSpi);

	int Send(const string& msg);

private:
#ifdef _WIN32
	static DWORD WINAPI _ThreadEntry(LPVOID pParam);
#else
	static void* _ThreadEntry(void* pParam);
#endif

	static void EvHhandler(struct mg_connection* c, int ev, void* ev_data, void* fn_data);

	void Run();
	int Connect();

	void OnConnected();
	void OnDisConnected();
	void OnData(mg_ws_message* wm);
	void OnPoll();

private:
	THREAD_HANDLE m_hThread;	/**< 线程句柄 */

#ifdef _WIN32
	DWORD m_IDThread;
#else
	int m_IDThread;
#endif
	struct mg_connection* m_nc;
	struct mg_mgr m_mgr;
	string m_strUrl;

	static int s_is_connected;
	int m_nStatus; // 0:未连接 1:正在连接 2:已连接 3:断线

	CWSClientSpi* m_pSpi;
	bool m_bStop;
};

#endif // __WSCLIENT_H__