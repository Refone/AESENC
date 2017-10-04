#ifndef _REFONE_AES_NI_
#define _REFONE_AES_NI_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#if !defined (ALIGN16)
# if defined (__GNUC__)
# define ALIGN16 __attribute__((aligned(16)))
# else
# define ALIGN16 __declspec(align(16))
# endif
#endif

#define AES_LENGTH_128 128
#define AES_LENGTH_192 192
#define AES_LENGTH_256 256

typedef struct KEY_SCHEDULE{
	ALIGN16 unsigned char KEY[16*15];
	unsigned int nr;
}AES_KEY;

int Check_CPU_support_AES(void);
void AES_set_key(AES_KEY *enc_key, AES_KEY *dec_key, unsigned char *user_key, int len);

#endif /* _REFONE_AES_NI_ */
