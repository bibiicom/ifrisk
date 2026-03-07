#include "SMTPClient.h"
#include "mongoose.h"

//static const char* user = "894987198@qq.com";  // Change this! Your mail account
//static const char* pass = "miwjkytodgtubbga";  // Change this! Your mail password
//static const char* to = "66046720@qq.com";    // Change this! Destination email
//
//static const char* from = "My Mail Sender";
//static const char* subj = "Test!";
//static const char* mesg = "Hi!\nThis is a test message.\nBye.";

enum { EHLO, STARTTLS, STARTTLS_WAIT, AUTH, FROM, TO, DATA, BODY, QUIT, END };

typedef struct tagEmailInfo
{
    const char* user;
    const char* password;
    const char* from;
    const char* to;
    const char* subject;
    const char* body;
    bool finish;
} EmailInfo;

static void fn(struct mg_connection* c, int ev, void* ev_data, void* fn_data) {
    uint8_t* state = (uint8_t*)c->data;
    EmailInfo* pEmailInfo = (EmailInfo*)fn_data;
    if (ev == MG_EV_OPEN) {
        // c->is_hexdumping = 1;
    }
    else if (ev == MG_EV_READ) {
        if (c->recv.len > 0 && c->recv.buf[c->recv.len - 1] == '\n') {
            MG_INFO(("<-- %.*s", (int)c->recv.len - 2, c->recv.buf));
            c->recv.len = 0;
            if (*state == EHLO) {
                mg_printf(c, "EHLO myname\r\n");
                *state = STARTTLS;
            }
            else if (*state == STARTTLS) {
                mg_printf(c, "STARTTLS\r\n");
                *state = STARTTLS_WAIT;
            }
            else if (*state == STARTTLS_WAIT) {
                struct mg_tls_opts opts = { };
                //opts.ca = "ca.pem";
                mg_tls_init(c, &opts);
                *state = AUTH;
            }
            else if (*state == AUTH) {
                char a[100], b[300] = "";
                size_t n = mg_snprintf(a, sizeof(a), "%c%s%c%s", 0, pEmailInfo->user, 0, pEmailInfo->password);
                mg_base64_encode((uint8_t*)a, n, b);
                mg_printf(c, "AUTH PLAIN %s\r\n", b);
                *state = FROM;
            }
            else if (*state == FROM) {
                mg_printf(c, "MAIL FROM: <%s>\r\n", pEmailInfo->user);
                *state = TO;
            }
            else if (*state == TO) {
                const char* str = strstr(pEmailInfo->to, ";");
				if (str != NULL) {
					char buff[128] = {};
					strncpy(buff, pEmailInfo->to, str - pEmailInfo->to);
                    mg_printf(c, "RCPT TO: <%s>\r\n", buff);
					pEmailInfo->to = str + 1;
				}
				else {
                    mg_printf(c, "RCPT TO: <%s>\r\n", pEmailInfo->to);
					pEmailInfo->to = NULL;
				}

				if (NULL == pEmailInfo->to)
					*state = DATA;
				else
					*state = TO;
            }
            else if (*state == DATA) {
                mg_printf(c, "DATA\r\n");
                *state = BODY;
            }
            else if (*state == BODY) {
                mg_printf(c, "From: %s <%s>\r\n", pEmailInfo->from, pEmailInfo->user);  // Mail header
                mg_printf(c, "Subject: %s\r\n", pEmailInfo->subject);          // Mail header
                mg_printf(c, "\r\n");                           // End of headers
                mg_printf(c, "%s\r\n", pEmailInfo->body);                   // Mail body
                mg_printf(c, ".\r\n");                          // End of body
                *state = QUIT;
            }
            else if (*state == QUIT) {
                mg_printf(c, "QUIT\r\n");
                *state = END;
            }
            else {
                c->is_draining = 1;
                MG_INFO(("end"));
            }
            MG_INFO(("--> %.*s", (int)c->send.len - 2, c->send.buf));
        }
    }
    else if (ev == MG_EV_CLOSE) {
        pEmailInfo->finish = true;
    }
    else if (ev == MG_EV_TLS_HS) {
        MG_INFO(("TLS handshake done! Sending EHLO again"));
        mg_printf(c, "EHLO myname\r\n");
    }
    (void)fn_data, (void)ev_data;
}

CSMTPClient::CSMTPClient(const char* url, const char* user, const char* password)
    : m_strUrl(url)
    , m_strUser(user)
    , m_strPassword(password)
{
}

void CSMTPClient::SendEmail(const char* from, const char* to, const char* subject, const char* body)
{
    EmailInfo emailInfo = {m_strUser.c_str(), m_strPassword.c_str(), from, to, subject, body, false};

	struct mg_mgr mgr;
	mg_mgr_init(&mgr);
	// mg_log_set(MG_LL_VERBOSE);
	mg_connect(&mgr, m_strUrl.c_str(), fn, &emailInfo);
	while (!emailInfo.finish)
	{
		mg_mgr_poll(&mgr, 1000);

	}
}
