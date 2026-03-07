#ifdef UNIX

#include "CSyslogService.h"

void CSyslogService::SendProbeMessageImpl(const char *parameter, const char *value)
{
	syslog( LOG_INFO, "%s %s\n", parameter, value );
}

#endif
