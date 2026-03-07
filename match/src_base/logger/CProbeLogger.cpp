#include "CProbeLogger.h"

void CProbeLogger::SendProbeMessage(const char *parameter, const char *value)
{
	SendProbeMessageImpl(parameter,value);
}

void CProbeLogger::SendProbeMessage(const char *parameter, const int value)
{
	char buffer[100];
	sprintf(buffer,"%d",value);
	SendProbeMessageImpl(parameter,buffer);
}

void CProbeLogger::SendProbeMessage(const char *parameter, const double value)
{
	char buffer[100];
	sprintf(buffer,"%lf",value);
	SendProbeMessageImpl(parameter,buffer);
}

void CProbeLogger::SendPercentageProbeMessage(const char *parameter, const double value)
{
	char buffer[100];
	sprintf(buffer,"%.2lf%%",value*100);
	SendProbeMessageImpl(parameter,buffer);
}
