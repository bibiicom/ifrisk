// Rename this file DES.CPP
// DES Encryption and Decryption
// from efgh.com/software
#include <memory.h>
#include "Des.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

// 注意在Windows 平台下，long 是32位的类型；在Linux 64位CPU下， long 是64位的。
//#ifdef WINDOWS
//typedef unsigned long int;
//#else
//typedef unsigned int int;
//#endif

const int KEY_SHIFT[DES_ROUNDS] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

const unsigned char COMPRESSION_PERMUTATION[DES_SBUFFER_SIZE] =
{
	14,17,11,24, 1, 5, 3,28,15, 6,21,10,
	23,19,12, 4,26, 8,16, 7,27,20,13, 2,
	41,52,31,37,47,55,30,40,51,45,33,48,
	44,49,39,56,34,53,46,42,50,36,29,32
};

const unsigned char EXPANSION_PERMUTATION[DES_SBUFFER_SIZE] =
{
	32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
	8, 9,10,11,12,13,12,13,14,15,16,17,
	16,17,18,19,20,21,20,21,22,23,24,25,
	24,25,26,27,28,29,28,29,30,31,32, 1
};

/*
S-Boxes:

14  4 13  1  2 15 11  8  3 10  6 12  5  9  0  7
0 15  7  4 14  2 13  1 10  6 12 11  9  5  3  8
4  1 14  8 13  6  2 11 15 12  9  7  3 10  5  0
15 12  8  2  4  9  1  7  5 11  3 14 10  0  6 13

15  1  8 14  6 11  3  4  9  7  2 13 12  0  5 10
3 13  4  7 15  2  8 14 12  0  1 10  6  9 11  5
0 14  7 11 10  4 13  1  5  8 12  6  9  3  2 15
13  8 10  1  3 15  4  2 11  6  7 12  0  5 14  9

10  0  9 14  6  3 15  5  1 13 12  7 11  4  2  8
13  7  0  9  3  4  6 10  2  8  5 14 12 11 15  1
13  6  4  9  8 15  3  0 11  1  2 12  5 10 14  7
1 10 13  0  6  9  8  7  4 15 14  3 11  5  2 12

7 13 14  3  0  6  9 10  1  2  8  5 11 12  4 15
13  8 11  5  6 15  0  3  4  7  2 12  1 10 14  9
10  6  9  0 12 11  7 13 15  1  3 14  5  2  8  4
3 15  0  6 10  1 13  8  9  4  5 11 12  7  2 14

2 12  4  1  7 10 11  6  8  5  3 15 13  0 14  9
14 11  2 12  4  7 13  1  5  0 15 10  3  9  8  6
4  2  1 11 10 13  7  8 15  9 12  5  6  3  0 14
11  8 12  7  1 14  2 13  6 15  0  9 10  4  5  3

12  1 10 15  9  2  6  8  0 13  3  4 14  7  5 11
10 15  4  2  7 12  9  5  6  1 13 14  0 11  3  8
9 14 15  5  2  8 12  3  7  0  4 10  1 13 11  6
4  3  2 12  9  5 15 10 11 14  1  7  6  0  8 13

4 11  2 14 15  0  8 13  3 12  9  7  5 10  6  1
13  0 11  7  4  9  1 10 14  3  5 12  2 15  8  6
1  4 11 13 12  3  7 14 10 15  6  8  0  5  9  2
6 11 13  8  1  4 10  7  9  5  0 15 14  2  3 12

13  2  8  4  6 15 11  1 10  9  3 14  5  0 12  7
1 15 13  8 10  3  7  4 12  5  6 11  0 14  9  2
7 11  4  1  9 12 14  2  0  6 10 13 15  3  5  8
2  1 14  7  4 10  8 13 15 12  9  0  3  5  6 11

*/

// The following are the above S-boxes, packed one bit per byte:

const unsigned char MODIFIED_SBOX_1[4 * 64] =
{
	1,1,1,0,0,1,0,0,1,1,0,1,0,0,0,1,0,0,1,0,1,1,1,1,1,0,1,1,1,0,0,0,
	0,0,1,1,1,0,1,0,0,1,1,0,1,1,0,0,0,1,0,1,1,0,0,1,0,0,0,0,0,1,1,1,
	0,0,0,0,1,1,1,1,0,1,1,1,0,1,0,0,1,1,1,0,0,0,1,0,1,1,0,1,0,0,0,1,
	1,0,1,0,0,1,1,0,1,1,0,0,1,0,1,1,1,0,0,1,0,1,0,1,0,0,1,1,1,0,0,0,
	0,1,0,0,0,0,0,1,1,1,1,0,1,0,0,0,1,1,0,1,0,1,1,0,0,0,1,0,1,0,1,1,
	1,1,1,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0,1,1,1,0,1,0,0,1,0,1,0,0,0,0,
	1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1,0,1,1,1,
	0,1,0,1,1,0,1,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,0,0,0,1,1,0,1,1,0,1
};

const unsigned char MODIFIED_SBOX_2[4 * 64] =
{
	1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,0,0,
	1,0,0,1,0,1,1,1,0,0,1,0,1,1,0,1,1,1,0,0,0,0,0,0,0,1,0,1,1,0,1,0,
	0,0,1,1,1,1,0,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,0,1,1,1,0,
	1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,1,1,0,1,0,0,1,1,0,1,1,0,1,0,1,
	0,0,0,0,1,1,1,0,0,1,1,1,1,0,1,1,1,0,1,0,0,1,0,0,1,1,0,1,0,0,0,1,
	0,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,1,0,1,1,1,1,
	1,1,0,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,1,1,1,1,1,0,1,0,0,0,0,1,0,
	1,0,1,1,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,1,1,1,1,0,1,0,0,1
};

const unsigned char MODIFIED_SBOX_3[4 * 64] =
{
	1,0,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,1,1,1,0,1,0,1,
	0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,1,1,0,1,0,0,0,0,1,0,1,0,0,0,
	1,1,0,1,0,1,1,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,0,0,0,1,1,0,1,0,1,0,
	0,0,1,0,1,0,0,0,0,1,0,1,1,1,1,0,1,1,0,0,1,0,1,1,1,1,1,1,0,0,0,1,
	1,1,0,1,0,1,1,0,0,1,0,0,1,0,0,1,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,
	1,0,1,1,0,0,0,1,0,0,1,0,1,1,0,0,0,1,0,1,1,0,1,0,1,1,1,0,0,1,1,1,
	0,0,0,1,1,0,1,0,1,1,0,1,0,0,0,0,0,1,1,0,1,0,0,1,1,0,0,0,0,1,1,1,
	0,1,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,1,0,0,1,0,1,1,0,0
};

const unsigned char MODIFIED_SBOX_4[4 * 64] =
{
	0,1,1,1,1,1,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,0,0,1,1,0,1,0,
	0,0,0,1,0,0,1,0,1,0,0,0,0,1,0,1,1,0,1,1,1,1,0,0,0,1,0,0,1,1,1,1,
	1,1,0,1,1,0,0,0,1,0,1,1,0,1,0,1,0,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,
	0,1,0,0,0,1,1,1,0,0,1,0,1,1,0,0,0,0,0,1,1,0,1,0,1,1,1,0,1,0,0,1,
	1,0,1,0,0,1,1,0,1,0,0,1,0,0,0,0,1,1,0,0,1,0,1,1,0,1,1,1,1,1,0,1,
	1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,
	0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,1,0,1,1,0,0,0,
	1,0,0,1,0,1,0,0,0,1,0,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,1,0,1,1,1,0
};

const unsigned char MODIFIED_SBOX_5[4 * 64] =
{
	0,0,1,0,1,1,0,0,0,1,0,0,0,0,0,1,0,1,1,1,1,0,1,0,1,0,1,1,0,1,1,0,
	1,0,0,0,0,1,0,1,0,0,1,1,1,1,1,1,1,1,0,1,0,0,0,0,1,1,1,0,1,0,0,1,
	1,1,1,0,1,0,1,1,0,0,1,0,1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,
	0,1,0,1,0,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,1,0,0,1,1,0,0,0,0,1,1,0,
	0,1,0,0,0,0,1,0,0,0,0,1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1,0,0,0,
	1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,1,0,1,1,0,0,0,1,1,0,0,0,0,1,1,1,0,
	1,0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,0,1,1,0,1,
	0,1,1,0,1,1,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1
};

const unsigned char MODIFIED_SBOX_6[4 * 64] =
{
	1,1,0,0,0,0,0,1,1,0,1,0,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,0,1,0,0,0,
	0,0,0,0,1,1,0,1,0,0,1,1,0,1,0,0,1,1,1,0,0,1,1,1,0,1,0,1,1,0,1,1,
	1,0,1,0,1,1,1,1,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,1,0,1,0,1,
	0,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,0,0,0,0,1,0,1,1,0,0,1,1,1,0,0,0,
	1,0,0,1,1,1,1,0,1,1,1,1,0,1,0,1,0,0,1,0,1,0,0,0,1,1,0,0,0,0,1,1,
	0,1,1,1,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,1,1,1,0,1,1,0,1,1,0,1,1,0,
	0,1,0,0,0,0,1,1,0,0,1,0,1,1,0,0,1,0,0,1,0,1,0,1,1,1,1,1,1,0,1,0,
	1,0,1,1,1,1,1,0,0,0,0,1,0,1,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1,1,0,1
};

const unsigned char MODIFIED_SBOX_7[4 * 64] =
{
	0,1,0,0,1,0,1,1,0,0,1,0,1,1,1,0,1,1,1,1,0,0,0,0,1,0,0,0,1,1,0,1,
	0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,0,1,1,0,1,0,0,1,1,0,0,0,0,1,
	1,1,0,1,0,0,0,0,1,0,1,1,0,1,1,1,0,1,0,0,1,0,0,1,0,0,0,1,1,0,1,0,
	1,1,1,0,0,0,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,1,1,1,1,0,0,0,0,1,1,0,
	0,0,0,1,0,1,0,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,1,1,0,1,1,1,1,1,1,0,
	1,0,1,0,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,1,0,1,1,0,0,1,0,0,1,0,
	0,1,1,0,1,0,1,1,1,1,0,1,1,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,1,1,1,
	1,0,0,1,0,1,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,0,0
};

const unsigned char MODIFIED_SBOX_8[4 * 64] =
{
	1,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,0,1,1,1,1,1,0,1,1,0,0,0,1,
	1,0,1,0,1,0,0,1,0,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,1,1,0,0,0,1,1,1,
	0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,1,0,1,0,0,0,1,1,0,1,1,1,0,1,0,0,
	1,1,0,0,0,1,0,1,0,1,1,0,1,0,1,1,0,0,0,0,1,1,1,0,1,0,0,1,0,0,1,0,
	0,1,1,1,1,0,1,1,0,1,0,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,0,0,0,1,0,
	0,0,0,0,0,1,1,0,1,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,0,
	0,0,1,0,0,0,0,1,1,1,1,0,0,1,1,1,0,1,0,0,1,0,1,0,1,0,0,0,1,1,0,1,
	1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,0,1,0,1,1
};

const unsigned char PBOX_PERMUTATION[32] =
{
	16, 7,20,21,29,12,28,17, 1,15,23,26, 5,18,31,10,
	2, 8,24,14,32,27, 3, 9,19,13,30, 6,22,11, 4,25
};

static void shift_half_key_left(unsigned char *k)
{
	unsigned char x = *k;
	unsigned char *kmax = k + (DES_KEY_SIZE / 2 - 1);
	do k[0] = k[1]; while (++k < kmax);
	*k = x;
}

/*-----------------------------------------------------------------------------
This function performs the required shifts and precomputes the 16 shifted
and compressed keys.
-----------------------------------------------------------------------------*/

void des::initialize(const unsigned char key[DES_KEY_SIZE])
{
	unsigned char shifted_key[DES_KEY_SIZE];
	memcpy(shifted_key, key, DES_KEY_SIZE);
	for (int round = 0; round < DES_ROUNDS; round++)
	{
		shift_half_key_left(shifted_key);
		shift_half_key_left(shifted_key + DES_KEY_SIZE / 2);
		if (KEY_SHIFT[round] == 2)
		{
			shift_half_key_left(shifted_key);
			shift_half_key_left(shifted_key + DES_KEY_SIZE / 2);
		}
		for (int i = 0; i < DES_SBUFFER_SIZE; i++)
		{
			compressed_shifted_key[round][i] =
				shifted_key[COMPRESSION_PERMUTATION[i] - 1];
		}
	}
}

/*-----------------------------------------------------------------------------
This function performs a DES encryption or decryption on a single 64-bit block
of data, using the specified 56-bit key. The data and key are packed one bit
per byte. Every element of data[] must be either 0 or 1. The results
will be highly anomalous if this is not the case.

The encrypted or decrypted data is returned to the data[] buffer.
-----------------------------------------------------------------------------*/

void des::encrypt_decrypt(unsigned char data[DES_DATA_SIZE],
	int /* boolean */ encrypt)
{
	unsigned char sbuffer[DES_SBUFFER_SIZE];
	union pbuffer_tag
	{
		unsigned char byte[DES_DATA_SIZE / 2];
		int dword[DES_DATA_SIZE / 8];
	} pbuffer;
	for (int round = 0; round < DES_ROUNDS; round++)
	{
		/* XOR compressed key with expanded right half of data */
		{
			int i;
			for (i = 0; i < DES_SBUFFER_SIZE; i++)
			{
				sbuffer[i] = data[DES_DATA_SIZE / 2 + EXPANSION_PERMUTATION[i] - 1] ^
					compressed_shifted_key[encrypt ? round : 15 - round][i];
			}
		}
		/* S-Box substitutions */
		{
#define index(n) (sbuffer[n]<<(5+2) | sbuffer[n+5]<<(4+2) | \
	sbuffer[n+1]<<(3+2) | sbuffer[n+2]<<(2+2) | sbuffer[n+3]<<(1+2) | \
	sbuffer[n+4]<<(0+2))
			pbuffer.dword[0] = *(int *)(MODIFIED_SBOX_1 + index(0));
			pbuffer.dword[1] = *(int *)(MODIFIED_SBOX_2 + index(6));
			pbuffer.dword[2] = *(int *)(MODIFIED_SBOX_3 + index(12));
			pbuffer.dword[3] = *(int *)(MODIFIED_SBOX_4 + index(18));
			pbuffer.dword[4] = *(int *)(MODIFIED_SBOX_5 + index(24));
			pbuffer.dword[5] = *(int *)(MODIFIED_SBOX_6 + index(30));
			pbuffer.dword[6] = *(int *)(MODIFIED_SBOX_7 + index(36));
			pbuffer.dword[7] = *(int *)(MODIFIED_SBOX_8 + index(42));
		}

		/* XOR and swap */
		if (round < 15)
		{
			int i;
			for (i = 0; i < DES_DATA_SIZE / 2; i++)
			{
				unsigned char x = data[DES_DATA_SIZE / 2 + i];
				data[DES_DATA_SIZE / 2 + i] = data[i] ^ pbuffer.byte[PBOX_PERMUTATION[i] - 1];
				data[i] = x;
			}
		}
		else
		{
			int i;
			for (i = 0; i < DES_DATA_SIZE / 2; i++)
				data[i] = data[i] ^ pbuffer.byte[PBOX_PERMUTATION[i] - 1];
		}
	}
}

/*----------------------------------------------------------------------------
This function takes 64 bits (8 bytes) of a password, XOR's it into a buffer,
and DES encrypts the buffer contents with the specified key. It then returns a
pointer to the next character of the password.
----------------------------------------------------------------------------*/

const char *des::xmix(const char *password,
	unsigned char buffer[DES_DATA_SIZE], const unsigned char key[DES_KEY_SIZE])
{
	int i;
	for (i = 0; i < 64; i += 8)
	{
		int c = *password;
		if (c != 0) password++;
		buffer[i] ^= c >> 7 & 1;
		buffer[i + 1] ^= c >> 6 & 1;
		buffer[i + 2] ^= c >> 5 & 1;
		buffer[i + 3] ^= c >> 4 & 1;
		buffer[i + 4] ^= c >> 3 & 1;
		buffer[i + 5] ^= c >> 2 & 1;
		buffer[i + 6] ^= c >> 1 & 1;
		buffer[i + 7] ^= c & 1;
	}
	initialize(key);
	encrypt(buffer);
	return password;
}

static unsigned char MIXER1[DES_KEY_SIZE] =
{
	1,0,1,0,0,0,0,0,0,1,
	0,1,0,0,1,1,0,1,0,0,
	1,1,0,1,1,1,0,1,1,0,
	0,0,0,0,1,0,0,1,0,1,
	1,1,0,0,0,0,1,1,1,0,
	0,1,0,1,1,0
};
static unsigned char MIXER2[DES_KEY_SIZE] =
{
	1,0,1,0,1,1,0,0,0,0,
	0,1,1,1,1,0,1,0,0,1,
	1,1,1,0,1,0,0,0,0,0,
	0,1,1,0,1,0,1,0,0,0,
	1,1,0,0,0,0,1,1,0,0,
	0,0,0,0,0,0
};
static unsigned char MIXER3[DES_KEY_SIZE] =
{
	1,0,0,0,1,1,1,1,1,0,
	1,0,0,0,1,1,0,0,1,0,
	0,1,0,1,1,0,0,0,0,0,
	0,0,1,1,1,0,0,0,1,0,
	1,0,0,0,1,0,0,0,1,1,
	1,1,1,1,0,0
};
static unsigned char MIXER4[DES_KEY_SIZE] =
{
	0,1,1,0,1,0,0,0,0,1,
	0,1,0,0,0,1,1,0,0,1,
	0,1,0,1,1,1,0,0,0,1,
	0,0,1,1,1,0,1,1,0,1,
	1,1,1,1,1,0,0,0,1,0,
	0,1,1,1,0,0
};

#ifdef TRIPLEDES

static unsigned char MIXER5[DES_KEY_SIZE] =
{
	1,1,0,1,0,0,1,1,0,1,
	1,1,1,0,1,1,1,0,1,1,
	0,1,1,1,0,1,0,0,1,1,
	0,0,0,1,1,0,1,0,0,1,
	1,1,0,1,0,1,0,1,1,0,
	1,0,0,1,1,0
};

static unsigned char MIXER6[DES_KEY_SIZE] =
{
	0,1,0,1,1,1,1,0,0,1,
	1,1,1,0,1,0,0,1,1,1,
	0,0,0,1,1,1,0,0,1,1,
	0,0,0,1,0,0,0,0,1,1,
	0,1,0,0,1,1,1,0,0,1,
	0,1,1,1,1,0
};

static unsigned char MIXER7[DES_KEY_SIZE] =
{
	1,0,1,0,0,1,1,0,1,0,
	0,0,0,1,1,1,1,1,1,0,
	0,0,0,0,1,0,0,0,0,0,
	0,1,1,1,1,0,0,0,1,1,
	1,0,0,1,1,1,0,0,0,0,
	0,0,1,1,1,1
};

static unsigned char MIXER8[DES_KEY_SIZE] =
{
	1,0,0,1,1,0,0,1,0,0,
	1,1,1,0,0,1,1,1,0,1,
	0,0,1,0,1,0,0,1,1,1,
	0,1,0,1,0,1,1,1,1,0,
	0,1,1,0,0,1,1,1,0,0,
	1,1,0,0,1,1
};

static unsigned char MIXER9[DES_KEY_SIZE] =
{
	1,1,1,0,1,0,0,1,1,1,
	1,1,0,1,0,1,0,1,1,1,
	0,0,0,1,1,0,1,0,0,0,
	0,1,1,1,1,0,0,0,1,1,
	0,1,1,1,1,0,1,1,1,1,
	1,0,1,0,1,0
};

static unsigned char MIXER10[DES_KEY_SIZE] =
{
	0,0,1,0,1,0,1,1,0,0,
	1,0,1,1,0,0,1,0,1,0,
	0,1,1,1,1,0,0,0,0,0,
	0,1,1,1,0,1,0,0,1,1,
	0,0,0,0,0,0,1,0,1,0,
	1,0,0,0,0,1
};

static unsigned char MIXER11[DES_KEY_SIZE] =
{
	0,0,1,1,0,0,1,1,1,0,
	1,0,1,0,1,0,0,0,1,0,
	0,1,1,1,0,1,1,1,1,0,
	0,0,1,0,0,1,1,0,1,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,1
};

static unsigned char MIXER12[DES_KEY_SIZE] =
{
	0,1,0,1,1,0,0,1,0,1,
	0,1,0,0,1,1,0,1,0,0,
	0,1,0,0,0,0,0,0,1,0,
	1,1,0,0,1,0,1,0,1,0,
	0,1,0,1,0,1,0,1,0,0,
	0,1,0,0,0,0
};

static unsigned char MIXER13[DES_KEY_SIZE] =
{
	1,0,1,0,0,0,0,1,0,1,
	1,0,0,1,0,1,0,1,0,0,
	1,0,1,1,1,1,0,0,1,1,
	1,1,0,0,0,1,1,1,0,0,
	1,0,0,1,0,1,0,0,0,0,
	0,1,0,0,1,1
};

static unsigned char MIXER14[DES_KEY_SIZE] =
{
	1,0,0,0,0,0,1,0,0,1,
	1,1,0,0,1,1,0,1,0,0,
	0,1,0,1,1,0,1,0,0,1,
	1,1,0,0,0,0,1,0,1,0,
	1,0,1,1,0,0,0,0,0,1,
	1,1,1,0,0,0
};

static unsigned char MIXER15[DES_KEY_SIZE] =
{
	0,0,1,1,1,0,0,0,1,1,
	0,0,1,1,0,0,1,0,1,0,
	1,0,0,0,1,0,1,0,0,1,
	0,0,0,1,1,0,1,1,1,1,
	0,0,0,0,0,0,1,0,0,1,
	0,1,0,0,1,1
};

static unsigned char MIXER16[DES_KEY_SIZE] =
{
	0,0,0,0,1,1,0,1,0,0,
	0,0,1,0,0,1,1,1,1,1,
	0,0,0,0,1,1,0,1,0,1,
	0,0,0,1,1,0,0,1,0,1,
	1,0,0,0,1,0,1,0,0,1,
	1,0,0,0,0,0
};

static unsigned char MIXER17[DES_KEY_SIZE] =
{
	0,0,0,0,1,1,1,0,0,1,
	0,0,0,0,1,0,0,1,0,1,
	0,0,1,0,0,0,1,0,1,0,
	0,1,0,0,1,0,0,1,1,1,
	1,1,0,0,1,0,1,0,0,0,
	0,1,0,1,1,1
};

static unsigned char MIXER18[DES_KEY_SIZE] =
{
	1,1,1,0,0,0,0,1,1,0,
	1,1,0,1,0,1,1,1,0,1,
	0,0,1,0,1,0,1,1,1,0,
	0,1,0,0,1,0,1,0,1,1,
	0,1,1,1,0,1,1,1,0,1,
	0,0,1,0,0,1
};

#endif

/*----------------------------------------------------------------------------
This function hashes a password and turns it into a key. Then it initializes
the key.
----------------------------------------------------------------------------*/

void des::password(const char *p)
{
	unsigned char buffer[DES_DATA_SIZE];
	memset(buffer, 0, sizeof(buffer));
	p = xmix(p, buffer, MIXER1);
	p = xmix(p, buffer, MIXER2);
	p = xmix(p, buffer, MIXER3);
	xmix(p, buffer, MIXER4);
	initialize(buffer);
}

#ifdef TRIPLEDES

void triple_des::password(const char *pwd)
{
	unsigned char buffer[DES_DATA_SIZE];
	memset(buffer, 0, sizeof(buffer));
	const char *p = pwd;
	p = part[0].xmix(p, buffer, MIXER1);
	p = part[0].xmix(p, buffer, MIXER2);
	p = part[0].xmix(p, buffer, MIXER3);
	p = part[0].xmix(p, buffer, MIXER4);
	p = part[0].xmix(p, buffer, MIXER5);
	part[0].xmix(p, buffer, MIXER6);
	part[0].initialize(buffer);
	p = pwd;
	p = part[1].xmix(p, buffer, MIXER7);
	p = part[1].xmix(p, buffer, MIXER8);
	p = part[1].xmix(p, buffer, MIXER9);
	p = part[1].xmix(p, buffer, MIXER10);
	p = part[1].xmix(p, buffer, MIXER11);
	part[1].xmix(p, buffer, MIXER12);
	part[1].initialize(buffer);
	p = pwd;
	p = part[2].xmix(p, buffer, MIXER13);
	p = part[2].xmix(p, buffer, MIXER14);
	p = part[2].xmix(p, buffer, MIXER15);
	p = part[2].xmix(p, buffer, MIXER16);
	p = part[2].xmix(p, buffer, MIXER17);
	part[2].xmix(p, buffer, MIXER18);
	part[2].initialize(buffer);
}

#endif

char* des::encryptbuffer(const char *buff, int size, bool isEncrypt)
{
	static unsigned char buffer[64];
	int i = 0;
	int number = 0;
	while (size-- != 0)
	{
		char c = buff[number++];
		buffer[i++] = c >> 7 & 1;
		buffer[i++] = c >> 6 & 1;
		buffer[i++] = c >> 5 & 1;
		buffer[i++] = c >> 4 & 1;
		buffer[i++] = c >> 3 & 1;
		buffer[i++] = c >> 2 & 1;
		buffer[i++] = c >> 1 & 1;
		buffer[i++] = c & 1;
	}
	while (i < 64)
		buffer[i++] = 0;
	if (isEncrypt)
		this->encrypt(buffer);
	else
		this->decrypt(buffer);

	char* ret = new char[9];
	number = 0;
	for (i = 0; i < 64; i += 8)
	{
		ret[number++] =
			buffer[i] << 7 |
			buffer[i + 1] << 6 |
			buffer[i + 2] << 5 |
			buffer[i + 3] << 4 |
			buffer[i + 4] << 3 |
			buffer[i + 5] << 2 |
			buffer[i + 6] << 1 |
			buffer[i + 7];
	}
	ret[8] = 0;
	return ret;
}

string des::EncryptNBuffer(const char *buff, int buffersize, bool isEncrypt)
{
	string strRet;
	int totalsize = buffersize;
	static char Base64EncodeBuff[200];

	char* pszRet = new char[buffersize + 3];

	char* psz = new char[buffersize + 3];

	if (isEncrypt)
	{
		snprintf(psz, buffersize + 3, "%02d%s", buffersize, buff);
	}
	else
	{
		int size = DecodeBase64(buff, (unsigned char *)Base64EncodeBuff, buffersize);
		memcpy(psz, Base64EncodeBuff, size);
		totalsize = size - 2;
	}

	char *p = psz;
	int i = 0;
	for (i = 0; i < totalsize + 2; i += 8)
	{
		char* pValue = encryptbuffer(p, 8, isEncrypt);
		memcpy(pszRet + i, pValue, 8);
		p += 8;
		delete pValue;
	}

	delete[] psz;
	if (isEncrypt)
	{
		int size = EncodeBase64((const unsigned char *)pszRet, (char *)Base64EncodeBuff, i, 1024);
		Base64EncodeBuff[size] = 0;
		strRet = Base64EncodeBuff;
		return strRet;
	}
	else
	{
		strRet = pszRet;
		//printf("Value is %s\r\n",psz);
		int size = atoi(strRet.substr(0, 2).c_str());
		strRet = strRet.substr(2, size);
		return strRet;
	}
}

const char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen)
{
	unsigned char c1, c2, c3;    // 输入缓冲区读出3个字节
	int nDstLen = 0;             // 输出的字符计数
	int nLineLen = 0;            // 输出的行长度计数
	int nDiv = nSrcLen / 3;      // 输入数据长度除以3得到的倍数
	int nMod = nSrcLen % 3;      // 输入数据长度除以3得到的余数

	// 每次取3个字节，编码成4个字符
	for (int i = 0; i < nDiv; i++)
	{
		// 取3个字节
		c1 = *pSrc++;
		c2 = *pSrc++;
		c3 = *pSrc++;

		// 编码成4个字符
		*pDst++ = EnBase64Tab[c1 >> 2];
		*pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
		*pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
		*pDst++ = EnBase64Tab[c3 & 0x3f];
		nLineLen += 4;
		nDstLen += 4;

		// 输出换行？
		if (nLineLen > nMaxLineLen - 4)
		{
			*pDst++ = '\r';
			*pDst++ = '\n';
			nLineLen = 0;
			nDstLen += 2;
		}
	}

	// 编码余下的字节
	if (nMod == 1)
	{
		c1 = *pSrc++;
		*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
		*pDst++ = EnBase64Tab[((c1 & 0x03) << 4)];
		*pDst++ = '=';
		*pDst++ = '=';
		nLineLen += 4;
		nDstLen += 4;
	}
	else if (nMod == 2)
	{
		c1 = *pSrc++;
		c2 = *pSrc++;
		*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
		*pDst++ = EnBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xf0) >> 4)];
		*pDst++ = EnBase64Tab[((c2 & 0x0f) << 2)];
		*pDst++ = '=';
		nDstLen += 4;
	}

	// 输出加个结束符
	*pDst = '\0';

	return nDstLen;
}


const char DeBase64Tab[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	62,        // '+'
	0, 0, 0,
	63,        // '/'
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
	0, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
	13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
	0, 0, 0, 0, 0, 0,
	26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
};

int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen)
{
	int nDstLen;            // 输出的字符计数
	int nValue;             // 解码用到的长整数
	int i;

	i = 0;
	nDstLen = 0;

	// 取4个字符，解码到一个长整数，再经过移位得到3个字节
	while (i < nSrcLen)
	{
		if (*pSrc != '\r' && *pSrc != '\n')
		{
			nValue = DeBase64Tab[*pSrc++] << 18;
			nValue += DeBase64Tab[*pSrc++] << 12;
			*pDst++ = (nValue & 0x00ff0000) >> 16;
			nDstLen++;

			if (*pSrc != '=')
			{
				nValue += DeBase64Tab[*pSrc++] << 6;
				*pDst++ = (nValue & 0x0000ff00) >> 8;
				nDstLen++;

				if (*pSrc != '=')
				{
					nValue += DeBase64Tab[*pSrc++];
					*pDst++ = nValue & 0x000000ff;
					nDstLen++;
				}
			}

			i += 4;
		}
		else        // 回车换行，跳过
		{
			pSrc++;
			i++;
		}
	}

	// 输出加个结束符
	*pDst = '\0';
	return nDstLen;
}


