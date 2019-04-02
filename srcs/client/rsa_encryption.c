#include "client/client.h"

void	rsa_encrypt(unsigned char *key, unsigned char *output)
{
 	FILE						*f;
	int 						ret = 1;
	size_t 						i;
	mbedtls_rsa_context 		rsa;
	mbedtls_entropy_context 	entropy;
	mbedtls_ctr_drbg_context 	ctr_drbg;
	unsigned char 				input[32];
	unsigned char 				buf[256];
	const char 					*pers = "rsa";
	mbedtls_mpi 				N, E;

	fflush( stdout );

	mbedtls_mpi_init( &N ); 
	mbedtls_mpi_init( &E );
	mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
	mbedtls_ctr_drbg_init( &ctr_drbg );
	mbedtls_entropy_init( &entropy );

	ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func,
								 &entropy, (const unsigned char *) pers,
								 strlen( pers ) );

	fflush( stdout );

	if( ( f = fopen(PUBLIC_KEY_PATH, "rb" ) ) == NULL )
	{
		mbedtls_printf( " failed\n  ! Could not open rsa_pub.txt\n" \
				"  ! Please run rsa_genkey first\n\n" );
	}

	if( ( ret = mbedtls_mpi_read_file( &N, 16, f ) ) != 0 ||
		( ret = mbedtls_mpi_read_file( &E, 16, f ) ) != 0 )
	{
		mbedtls_printf( " failed\n  ! mbedtls_mpi_read_file returned %d\n\n",
						ret );
		fclose( f );
	}

	fclose( f );

	if( ( ret = mbedtls_rsa_import( &rsa, &N, NULL, NULL, NULL, &E ) ) != 0 )
		mbedtls_printf( " failed\n  ! mbedtls_rsa_import returned %d\n\n", ret );

	memcpy( input, key, 32 );

	/*
	 * Calculate the RSA encryption of the hash.
	 */

	fflush( stdout );

	ret = mbedtls_rsa_pkcs1_encrypt( &rsa, mbedtls_ctr_drbg_random,
									 &ctr_drbg, MBEDTLS_RSA_PUBLIC,
									 32, input, buf );

	if( ret != 0 )
		mbedtls_printf( " failed\n  ! mbedtls_rsa_pkcs1_encrypt returned %d\n\n",ret );


	for( i = 0; i < rsa.len; i++ )
		output[i] = buf[i];

	mbedtls_mpi_free( &N ); 
	mbedtls_mpi_free( &E );
	mbedtls_ctr_drbg_free( &ctr_drbg );
	mbedtls_entropy_free( &entropy );
	mbedtls_rsa_free( &rsa );
}
