#include "ApiKeyUtil.h"
#include <openssl/hmac.h>

string GenerateSign(string SecretKey, string RequestTimestamp, string RequestMethod, string RequestPath, string RequestBody)
{
	string data = RequestTimestamp + RequestMethod + RequestPath + RequestBody;
	return HmacShaEncode(SecretKey.c_str(), data.c_str());
}

string HmacShaEncode(const char* Key, const char* Input)
{
	unsigned char* mac = NULL;
	unsigned int macLen = 0;
	const EVP_MD* engine = EVP_sha256();

	mac = (unsigned char*)malloc(EVP_MAX_MD_SIZE);

	HMAC_CTX* ctx = HMAC_CTX_new();
	HMAC_Init_ex(ctx, Key, (int)strlen(Key), engine, NULL);
	HMAC_Update(ctx, (unsigned char*)Input, strlen(Input));

	HMAC_Final(ctx, mac, &macLen);
	HMAC_CTX_free(ctx);

	char signature[256] = {};

	EVP_EncodeBlock((unsigned char *)signature, mac, macLen);
	return signature;
}

string HmacShaEncode2(const char* Key, const char* Input)
{
	unsigned char* mac = NULL;
	unsigned int macLen = 0;
	const EVP_MD* engine = EVP_sha256();

	mac = (unsigned char*)malloc(EVP_MAX_MD_SIZE);

	HMAC_CTX* ctx = HMAC_CTX_new();
	HMAC_Init_ex(ctx, Key, (int)strlen(Key), engine, NULL);
	HMAC_Update(ctx, (unsigned char*)Input, strlen(Input));

	HMAC_Final(ctx, mac, &macLen);
	HMAC_CTX_free(ctx);

	char* signature = (char*)malloc(macLen * 2 + 1);
	signature[2 * macLen] = 0;

	static const char bin2chars[] = "0123456789abcdef";
	for (size_t i = 0; i < macLen; i++)
	{
		signature[i * 2] = bin2chars[mac[i] >> 4];
		signature[i * 2 + 1] = bin2chars[mac[i] & 0x0f];
	}

	string ret = signature;
	free(signature);
	
	return ret;
}