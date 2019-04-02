#include "../../includes/client/client.h"

void	aes_key_generation(unsigned char *key)
{
	mbedtls_ctr_drbg_context 	ctr_drbg;

	mbedtls_entropy_context 	entropy;

	memset(key, 0, 32);

	char *pers = "and here comes some line for key generation";
	
	int ret;

	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_init( &ctr_drbg );

	if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, (unsigned char *) pers, strlen( pers ) ) ) != 0 )
	    printf( " failed\n ! mbedtls_ctr_drbg_init returned -0x%04x\n", -ret );

	if( ( ret = mbedtls_ctr_drbg_random( &ctr_drbg, key, 32 ) ) != 0 )
	    printf( " failed\n ! mbedtls_ctr_drbg_random returned -0x%04x\n", -ret );
}

void	sha1_checksum_generation(unsigned char *digest, unsigned char *initial_packet)
{
	int ret;

	if( ( ret = mbedtls_sha1_ret( initial_packet, 256, digest ) ) != 0 )
        exit(1);
}

void	aes_encrypt(unsigned char *iv, unsigned char *key, unsigned char *initial_full_packet, unsigned char *encrypted_full_packet)
{
	mbedtls_aes_context aes;

	mbedtls_aes_init(&aes);

	mbedtls_aes_setkey_enc( &aes, key, 256 );

	mbedtls_aes_crypt_cbc( &aes, MBEDTLS_AES_ENCRYPT, INITIAL_PACKET_SIZE + DIGEST_SIZE, iv, initial_full_packet, encrypted_full_packet );
}