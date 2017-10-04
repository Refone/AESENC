 /*
  * A function with OpenSSL interface (using AES_KEY struct), to call the other key-
  * length specific key expansion functions
  */

#include "aes.h"

/************************************************************************************/
/* implement in assemble code */

/* key_expasion.s */
extern int AES_set_encrypt_key(const unsigned char *userKey, 
			       const int bits, 
			       AES_KEY *key);

extern int AES_set_decrypt_key(const unsigned char *userKey, 
			       const int bits, 
			       AES_KEY *key);

/* ecb.s */
/* cbc.s */
void AES_CBC_encrypt(const unsigned char *in, unsigned char *out, const unsigned char iv[16],
					 unsigned long length, const unsigned char *KS, int nr);
void AES_CBC_decrypt(const unsigned char *in, unsigned char *out, const unsigned char iv[16],
					 unsigned long length, const unsigned char *KS, int nr);

/* ctr.s */
void AES_CTR_encrypt(const unsigned char *in, unsigned char *out, 
					 const unsigned char ivec[8], const unsigned char nonce[4],
					 unsigned long length, const unsigned char *KS, int nr);
/************************************************************************************/

void AES_encrypt_ecb(void *paddr, unsigned char *key_addr, uint8_t length);
void AES_decrypt_ecb(void *paddr, unsigned char *key_addr, uint8_t length);

void AES_encrypt_cbc(void *paddr, unsigned char *key_addr, const unsigned char iv[16],
							uint8_t length);
void AES_decrypt_cbc(void *paddr, unsigned char *key_addr, const unsigned char iv[16],
							uint8_t length);

void AES_encrypt_ctr(void *paddr, unsigned char *key_addr, const unsigned char iv[8], 
							unsigned char nonce[4], uint8_t length);

void AES_128_Key_Expansion(const unsigned char* userKey, AES_KEY *key);
void AES_192_Key_Expansion(const unsigned char* userKey, AES_KEY *key);
void AES_256_Key_Expansion(const unsigned char* userKey, AES_KEY *key);

int AES_set_encrypt_key (const unsigned char *userKey,
						 const int bits,
						 AES_KEY *key)
{
		if (!userKey || !key)
				return -1;
		if (bits == 128)
		{
				AES_128_Key_Expansion(userKey, key);
				key->nr = 10;
				return 0;
		} 
		else if (bits == 192) 
		{
				AES_192_Key_Expansion(userKey, key);
				key->nr = 12;
				return 0;
		}
		else if (bits == 256)
		{
				AES_256_Key_Expansion(userKey, key);
				key->nr = 14;
				return 0;
		}
		return -2;
}
/*
int AES_set_decrypt_key (const unsigned char *userKey,
						 const int bits,
						 AES_KEY *key)
{
		int i,nr;;
		AES_KEY temp_key;
		ALIGN16 unsigned char *Key_Schedule = (ALIGN16 unsigned char *)key->KEY;
		ALIGN16 unsigned char *Temp_Key_Schedule = (ALIGN16 unsigned char*)temp_key.KEY;
		if (!userKey || !key)
				return -1;
		if (AES_set_encrypt_key(userKey,bits,&temp_key) == -2)
				return -2;

		printf("nr: %d\n", temp_key.nr);
		printf("sizeof ALIGN16: %d\n", sizeof(ALIGN16 unsigned char));
		nr = temp_key.nr;
		key->nr = nr;
		for (i=0; i<=nr; i++)
			memcpy(&Key_Schedule[(nr-i)*16], &Temp_Key_Schedule[i*16], 16);
		return 0;
}
*/
int AES_set_decrypt_key (const unsigned char *userKey,
						 const int bits,
						 AES_KEY *key)
{
		int i,nr;;
		AES_KEY temp_key;
		__m128i *Key_Schedule = (__m128i*)key->KEY;
		__m128i *Temp_Key_Schedule = (__m128i*)temp_key.KEY;
		if (!userKey || !key)
				return -1;
		if (AES_set_encrypt_key(userKey,bits,&temp_key) == -2)
				return -2;
		nr = temp_key.nr;
		key->nr = nr;
		Key_Schedule[nr] = Temp_Key_Schedule[0];
		for (i=1;i<nr;i++)
			Key_Schedule[nr-i] = _mm_aesimc_si128(Temp_Key_Schedule[i]);
		Key_Schedule[0] = Temp_Key_Schedule[nr];
		return 0;
}
void AES_encrypt_ecb(void *paddr, unsigned char *key_addr, uint8_t length)
{
	AES_set_encrypt_key(key_addr, KEY_LENGTH, &key);
	AES_ECB_encrypt(paddr, paddr, length, key.KEY, key.nr);
}

void AES_decrypt_ecb(void *paddr, unsigned char *key_addr, uint8_t length)
{
	AES_set_decrypt_key(key_addr, KEY_LENGTH, &key);
	AES_ECB_decrypt(paddr, paddr, length, key.KEY, key.nr);
}

void AES_encrypt_cbc(void *paddr, unsigned char *key_addr, const unsigned char iv[16],
							uint8_t length)
{
	AES_set_encrypt_key(key_addr, KEY_LENGTH, &key);
	AES_CBC_encrypt(paddr, paddr, iv, length, key.KEY, key.nr);
}

void AES_decrypt_cbc(void *paddr, unsigned char *key_addr, const unsigned char iv[16], 
							uint8_t length)
{
	AES_set_decrypt_key(key_addr, KEY_LENGTH, &key);
	AES_CBC_decrypt(paddr, paddr, iv, length, key.KEY, key.nr);
}

void AES_encrypt_ctr(void *paddr, unsigned char *key_addr, const unsigned char iv[8], 
							unsigned char nonce[4], uint8_t length)
{
	AES_set_encrypt_key(key_addr, KEY_LENGTH, &key);
	AES_CTR_encrypt(paddr, paddr, iv, nonce, length, key.KEY, key.nr);
}
