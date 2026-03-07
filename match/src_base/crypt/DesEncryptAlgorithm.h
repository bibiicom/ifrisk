#include "Des.h"

class IEncryptAlgorithm
{
public:
	IEncryptAlgorithm(){};
	virtual ~IEncryptAlgorithm(){};
	virtual string Encrypt(string strInData) = 0;
	virtual string Decrypt(string strInData) = 0;
};

class CDesEncryptAlgorithm:public IEncryptAlgorithm
{
public:
	CDesEncryptAlgorithm()
	{
		crypto.password("cffex");
	}
	
	virtual ~CDesEncryptAlgorithm(){};
	virtual string Encrypt(string strInData)
	{
		return crypto.EncryptNBuffer(strInData.c_str(),strInData.length(),true);
		
	}
	virtual string Decrypt(string strInData)
	{ 
		return crypto.EncryptNBuffer(strInData.c_str(),strInData.length(),false);
	}	
private:
	des crypto;
};
