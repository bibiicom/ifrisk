// Rename this file DES.H
// DES Encryption and Decryption
// from efgh.com/software
#include <string>
using namespace std;
const int DES_KEY_SIZE      =  56;
const int DES_DATA_SIZE     =  64;
const int DES_SBUFFER_SIZE  =  48;
const int DES_ROUNDS =  16;

int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);

class des
{
private:
	const char *xmix(const char *, unsigned char [DES_DATA_SIZE],
		const unsigned char [DES_KEY_SIZE]);
	void encrypt_decrypt(unsigned char [DES_DATA_SIZE], int /* boolean */);
	unsigned char compressed_shifted_key[DES_ROUNDS][DES_SBUFFER_SIZE];
public:
	void initialize(const unsigned char[DES_KEY_SIZE]);
	void password(const char *);
	void encrypt(unsigned char data[DES_DATA_SIZE])
	{
		encrypt_decrypt(data, 1 /* true */);
	}
	void decrypt(unsigned char data[DES_DATA_SIZE])
	{
		encrypt_decrypt(data, 0 /* false */);
	}
	
	char* encryptbuffer(const char *buff,int size,bool isEncrypt);
	string EncryptNBuffer(const char *buff,int buffersize,bool isEncrypt);
	//friend triple_des;
};





