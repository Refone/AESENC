
void AES_ECB_encrypt(const unsigned char *in, 
		     unsigned char *out, 
		     unsigned long length,
		     const unsigned char *KS, int nr);

void AES_ECB_decrypt(const unsigned char *in, 
		     unsigned char *out, 
		     unsigned long length,
		     const unsigned char *KS, 
		     int nr);

