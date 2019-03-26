#include "../../includes/server/server.h"

void aes_decrypt(unsigned char *iv, unsigned char *key, unsigned char *input, unsigned char *output)
{
	int ret;
	mbedtls_aes_context aes;

	mbedtls_aes_init(&aes);

	ret = mbedtls_aes_setkey_dec(&aes, key, 256);

	// if (ret == 0)
	// 	printf("%s\n", "successs on init of the key");
	// else
	// 	printf("%s\n", "wtf ?");

	ret = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, INITIAL_PACKET_SIZE + DIGEST_SIZE, iv, input, output);

	// if (ret == 0)
	// 	printf("%s\n", "successs on decription");
	// else
	// 	printf("%s\n", "wtf ?");
}