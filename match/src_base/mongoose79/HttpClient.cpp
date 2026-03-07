#include "HttpClient.h"
#include "mongoose.h"
#include "platform.h"

static const uint64_t s_timeout_ms = 15000;  // Connect timeout in milliseconds

typedef struct tagHttpReqInfo
{
    const char* method;
    const char* header;
    const char* data;
    const char* url;
    bool finish;
    char* respData;
    int respDataLen;
    bool hasError;
} HttpReqInfo;

static void fn(struct mg_connection* c, int ev, void* ev_data, void* fn_data) 
{
    HttpReqInfo* pHttpReqInfo = (HttpReqInfo*)fn_data;

    switch (ev)
    {
    case MG_EV_OPEN:
    {       
        // Connection created. Store connect expiration time in c->data
        *(uint64_t*)c->data = mg_millis() + s_timeout_ms;
        break;
    }
    case MG_EV_POLL:
    {
        if (mg_millis() > *(uint64_t*)c->data &&
            (c->is_connecting || c->is_resolving)) {
            mg_error(c, "Connect timeout");
        }
        break;
    }
    case MG_EV_CONNECT:
    {
        // Connected to server. Extract host name from URL
        struct mg_str host = mg_url_host(pHttpReqInfo->url);

        // If s_url is https://, tell client connection to use TLS
        if (mg_url_is_ssl(pHttpReqInfo->url)) {
            struct mg_tls_opts opts = {};
            //opts.ca = "*";
            opts.srvname = host;
            mg_tls_init(c, &opts);
        }

        // Send request
        if (NULL == pHttpReqInfo->header)
            pHttpReqInfo->header = "";

        int content_length = pHttpReqInfo->data ? strlen(pHttpReqInfo->data) : 0;
        mg_printf(c,
            "%s %s HTTP/1.1\r\n"
            "%s\r\n"
            "Host: %.*s\r\n"
            "Content-Type: octet-stream\r\n"
            "Content-Length: %d\r\n"
            "\r\n",
            pHttpReqInfo->method, mg_url_uri(pHttpReqInfo->url), pHttpReqInfo->header, (int)host.len,
            host.ptr, content_length);

		if (pHttpReqInfo->data)
			mg_send(c, pHttpReqInfo->data, content_length);
        break;
    }
    case MG_EV_HTTP_MSG:
    {
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;

        int len = MIN(pHttpReqInfo->respDataLen - 1, (int)hm->body.len);
        strncpy(pHttpReqInfo->respData, hm->body.ptr, len);
        pHttpReqInfo->respData[len] = 0;

        c->is_closing = 1;         // Tell mongoose to close this connection
        pHttpReqInfo->finish = true;  // Tell event loop to stop
        pHttpReqInfo->hasError = false;
        break;
    }
    case MG_EV_ERROR:
    {
        snprintf(pHttpReqInfo->respData, pHttpReqInfo->respDataLen - 1, "connect failed:%s", strerror(*(int*)ev_data));
        pHttpReqInfo->finish = true;  // Error, tell event loop to stop
        pHttpReqInfo->hasError = true;
        break;
    }
    case MG_EV_CLOSE:
    {
        pHttpReqInfo->finish = true;  // End, tell event loop to stop
        break;
    }
    default:
    {
        //printf("Error:%d\n", ev);
    }
    }
}

CHttpClient::CHttpClient()
{
}

bool CHttpClient::Post(const char* func, const char* data, const char* header, char* respData, int respDataLen)
{
    return Request("POST", func, data, header, respData, respDataLen);
}

bool CHttpClient::Put(const char* func, const char* data, const char* header, char* respData, int respDataLen)
{
    return Request("PUT", func, data, header, respData, respDataLen);
}

bool CHttpClient::Delete(const char* func, const char* data, const char* header, char* respData, int respDataLen)
{
    return Request("DELETE", func, data, header, respData, respDataLen);
}

bool CHttpClient::Get(const char* func, const char* data, const char* header, char* respData, int respDataLen)
{
    return Request("GET", func, data, header, respData, respDataLen);
}

bool CHttpClient::Request(const char* method, const char* func, const char* data, const char* header, char* respData, int respDataLen)
{
    struct mg_mgr mgr;              // Event manager
    char url[1024] = {};
    if (data != NULL && 0 != strcmp(method, "POST"))
	{
        snprintf(url, sizeof(url) - 1, "%s%s?%s", m_strBaseUrl.c_str(), func, data);
		data = NULL;
	}
    else
        snprintf(url, sizeof(url) - 1, "%s%s", m_strBaseUrl.c_str(), func);
    HttpReqInfo httpReqInfo = { method, header, data, url, false, respData, respDataLen, false };
    //mg_log_set(4);
    mg_mgr_init(&mgr);              // Initialise event manager
    mg_http_connect(&mgr, url, fn, &httpReqInfo);  // Create client connection
    while (!httpReqInfo.finish) mg_mgr_poll(&mgr, 2);      // Event manager loops until 'done'
    mg_mgr_free(&mgr);                        // Free resources

    return !httpReqInfo.hasError;
}
