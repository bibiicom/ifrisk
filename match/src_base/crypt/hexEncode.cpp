#include "hexEncode.h"

static const char hexDigit[] = "0123456789abcdef";

CHexEncoder::CHexEncoder(void)
{
}

bool CHexEncoder::encode(char *buffer, int bufferLen, const char *input, int inputLen)
{
	if (inputLen == -1)
	{
		inputLen = (int)strlen(input);
	}
	if (bufferLen <= inputLen * 2)
	{
		return false;
	}
	char *pResult = buffer;
	for (int i = 0; i < inputLen; i++)
	{
		*pResult++ = hexDigit[(input[i] >> 4) & 0x0f];
		*pResult++ = hexDigit[input[i] & 0x0f];
	}
	*pResult = '\0';
	return true;
}

int CHexEncoder::encodeLen(const char *input, int inputLen)
{
	if (inputLen == -1)
	{
		inputLen = (int)strlen(input);
	}
	return inputLen * 2 + 1;
}
