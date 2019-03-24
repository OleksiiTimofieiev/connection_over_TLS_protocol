#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_fprintf         fprintf
#define mbedtls_printf          printf
#define mbedtls_exit            exit
#define MBEDTLS_EXIT_SUCCESS    EXIT_SUCCESS
#define MBEDTLS_EXIT_FAILURE    EXIT_FAILURE
#endif /* MBEDTLS_PLATFORM_C */

/* generate aes key */
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

/* aes encription includes */
#include "mbedtls/aes.h"

#include <stdio.h> // printf
#include <string.h> // strlen 

// #define SIZE_OF_THE_DATA 128

#if !defined(MBEDTLS_AES_C) || !defined(MBEDTLS_SHA256_C) || \
    !defined(MBEDTLS_FS_IO) || !defined(MBEDTLS_MD_C) || \
    !defined(MBEDTLS_CTR_DRBG_C) || !defined(MBEDTLS_ENTROPY_C)
int main( void )
{
    mbedtls_printf("MBEDTLS_AES_C and/or MBEDTLS_SHA256_C "
                    "and/or MBEDTLS_FS_IO and/or MBEDTLS_MD_C "
                    "not defined.\n");
    return( 0 );
}

#else


int		main(void)
{

	/* key generation section */
	/* ----------------------------------------------------------------------------- */

	mbedtls_ctr_drbg_context 	ctr_drbg;

	mbedtls_entropy_context 	entropy;

	unsigned char key[32]; // container for the key ;

	memset(key, 0, 32);


	char *pers = "aes generate key woohoo"; // random line for the key generation ;
	
	int ret;

	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_init( &ctr_drbg );

	if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, (unsigned char *) pers, strlen( pers ) ) ) != 0 )
	{
	    printf( " failed\n ! mbedtls_ctr_drbg_init returned -0x%04x\n", -ret );
	    
	    // goto exit;
	}

	if( ( ret = mbedtls_ctr_drbg_random( &ctr_drbg, key, 32 ) ) != 0 )
	{
	    printf( " failed\n ! mbedtls_ctr_drbg_random returned -0x%04x\n", -ret );
	    
	    // goto exit;
	}

	// int j = 0;

	// printf("key -> ");

	// while (j < 32)
	// {
	// 	printf("%d", key[j++]);
	// }

	// printf("\n");


	/* aes encription section */
	/* ----------------------------------------------------------------------------- */

	mbedtls_aes_context aes;

	mbedtls_aes_init(&aes);


	// unsigned char iv[16];

	// memset(iv, 0, 16);

	// int v = 0;
	// unsigned char x = 'a';

	// while (v < 16)
	// {
	// 	iv[v] = x;
	// 	v++;
	// 	x++;
	// }

	unsigned char iv[16] = {0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9};
	unsigned char iv1[16] = {0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9};

		int j = 0;

	printf("iv -> ");

	while (j < 16)
	{
		printf("%x ", iv[j++]);
	}

	printf("\n");

	unsigned char input [128];
	unsigned char result [128];
	unsigned char output[128];


	memset(input, 0, 128);
	memset(result, 0, 128);
	memset(output, 0, 128);





	size_t input_len = 40;
	size_t output_len = 0;

	int i = 0;

	while (i < 48)
	{
		input[i] = 'z';
		i++;
	}

	i = 0;
	printf("input -> ");

	while (i < 128)
	{
		printf("%c", input[i]);
		i++;
	}

	printf("\n");

	// mbedtls_aes_init(&aes);


	ret = mbedtls_aes_setkey_enc( &aes, key, 256 );

	if (ret == 0)
		printf("%s\n", "successs on init of the key");
	else
		printf("%s\n", "wtf ?");


	ret = mbedtls_aes_crypt_cbc( &aes, MBEDTLS_AES_ENCRYPT, 128, iv, input, output );

	if (ret == 0)
		printf("%s\n", "successs on encription");
	else
		printf("%s\n", "wtf ?");




	ret = mbedtls_aes_setkey_dec( &aes, key, 256 );

	if (ret == 0)
		printf("%s\n", "successs on init of the key");
	else
		printf("%s\n", "wtf ?");

	ret = mbedtls_aes_crypt_cbc( &aes, MBEDTLS_AES_DECRYPT, 128, iv1, output, result );

	if (ret == 0)
		printf("%s\n", "successs on decription");
	else
		printf("%s\n", "wtf ?");


	i = 0;

	printf("output-> ");

	while (i < 128)
	{
		printf("%x", output[i]);
		i++;
	}


	i = 0;
	printf("\n");
	


	printf("result-> ");

	while (i < 128)
	{
		printf("%c ", result[i]);
		i++;
	}


	return (0);
}
#endif
