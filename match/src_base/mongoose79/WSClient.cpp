#include "WSClient.h"
#include "platform.h"

int CWSClient::s_is_connected = 0;

void CWSClient::EvHhandler(struct mg_connection* c, int ev, void* ev_data, void* fn_data)
{
	CWSClient* pWSClient = (CWSClient*)fn_data;
	switch (ev)
	{
	case MG_EV_OPEN:
	{
		//c->is_hexdumping = 1; // 日志相关
		break;
	}
	case MG_EV_CONNECT:
	{
		if (mg_url_is_ssl(pWSClient->m_strUrl.c_str()))
		{
			struct mg_str host = mg_url_host(pWSClient->m_strUrl.c_str());
			struct mg_tls_opts opts = {};
			opts.srvname = host;
			mg_tls_init(c, &opts);
		}
		break;
	}
	case MG_EV_ERROR:
	{
		MG_ERROR(("MG_EV_ERROR:%p %s", c->fd, (char*)ev_data));
		break;
	}
	case MG_EV_WS_OPEN:
	{
		pWSClient->OnConnected();
		break;
	}
	case MG_EV_WS_MSG:
	{
		struct mg_ws_message* wm = (struct mg_ws_message*)ev_data;
		pWSClient->OnData(wm);
		break;
	}
	case MG_EV_POLL:
	{
		pWSClient->OnPoll();
		break;
	}
	case MG_EV_CLOSE:
	{
		pWSClient->OnDisConnected();
		break;
	}
	}
}

#ifdef _WIN32
DWORD WINAPI CWSClient::_ThreadEntry(LPVOID pParam)
#else
void* CWSClient::_ThreadEntry(void* pParam)
#endif
{
	CWSClient* pWSClient = (CWSClient*)pParam;

	pWSClient->Run();

	return NULL;
}

CWSClient::CWSClient()
	: m_nc(NULL)
	, m_hThread(NULL)
	, m_IDThread(0)
	, m_nStatus(0)
	, m_pSpi(NULL)
	, m_bStop(false)
{
	mg_mgr_init(&m_mgr);
	//mg_log_set(MG_LL_DEBUG);  // Set log level
	//mg_set_nameserver(&m_mgr, "8.8.8.8");
}

CWSClient::~CWSClient()
{
	mg_mgr_free(&m_mgr);
}

void CWSClient::SetUrl(const string& url)
{
	m_strUrl = url;
}

bool CWSClient::Start()
{
	//	if (m_nc == NULL)
	//		return false;

	if (m_hThread != (THREAD_HANDLE)0)
		return true;

	bool ret = true;
#ifdef _WIN32
	m_hThread = ::CreateThread(NULL, 0, _ThreadEntry, this, 0, &m_IDThread);
	if (m_hThread == NULL)
		ret = false;
#else
	ret = (::pthread_create(&m_hThread, NULL, &_ThreadEntry, this) == 0);
#endif
	return ret;
}

void CWSClient::Close()
{
	m_bStop = true;
}

int CWSClient::Connect()
{
	//m_nc = mg_connect_ws(&m_mgr, EvHhandler, url.c_str(), NULL, NULL);
	m_nc = mg_ws_connect(&m_mgr, m_strUrl.c_str(), EvHhandler, this, NULL);     // Create client
	if (m_nc == NULL)
	{
		fprintf(stderr, "Invalid address\n");
		return 1;
	}
	m_nStatus = 1;
	return 0;
}

void CWSClient::RegisterSpi(CWSClientSpi* pSpi)
{
	m_pSpi = pSpi;
}

int CWSClient::Send(const string& msg)
{
	if (m_bStop)
		return -1;

	if (2 != m_nStatus)
		return -1;

	mg_ws_send(m_nc, msg.c_str(), msg.size(), WEBSOCKET_OP_TEXT);

	return 0;
}

void CWSClient::Run()
{
	m_nStatus = 1;
	Connect();
	while (1)
	{
		if (m_bStop)
			m_nc->is_draining = true;

		mg_mgr_poll(&m_mgr, 1000);
		if (3 == m_nStatus)
		{
			if (!m_bStop)
			{
				//printf("断线重新连接");
				Connect();
			}
			else
			{
				//printf("已断开，退出!");
				break;
			}
		}
	}

	if (m_pSpi != NULL)
		m_pSpi->OnWSClosed();
}

void CWSClient::OnConnected()
{
	m_nStatus = 2;
	if (m_pSpi != NULL)
		m_pSpi->OnWSConnected();
}

void CWSClient::OnDisConnected()
{
	m_nStatus = 3;
	if (m_pSpi != NULL)
		m_pSpi->OnWSDisConnected();
}

void CWSClient::OnData(mg_ws_message* wm)
{
	//printf("%.*s\n", (int)wm->size, wm->data);
	if (m_pSpi != NULL)
		m_pSpi->OnData(wm->data.ptr, wm->data.len);
}

void CWSClient::OnPoll()
{
	if (m_pSpi != NULL)
		m_pSpi->OnPoll();
}
