#include "../../includes/client/client.h"

void	rsa_encrypt(unsigned char *key, unsigned char *output)
{
 	FILE *f;
    int ret = 1;
    // int exit_code = MBEDTLS_EXIT_FAILURE;
    size_t i;
    mbedtls_rsa_context rsa;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    unsigned char input[32];
    unsigned char buf[256];

    const char *pers = "rsa";

    mbedtls_mpi N, E;

    // mbedtls_printf( "\n  . Seeding the random number generator..." );

    fflush( stdout );

    mbedtls_mpi_init( &N ); 
    mbedtls_mpi_init( &E );
    mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
    mbedtls_ctr_drbg_init( &ctr_drbg );
    mbedtls_entropy_init( &entropy );

    ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func,
                                 &entropy, (const unsigned char *) pers,
                                 strlen( pers ) );
    // if( ret != 0 )
    // {
    //     mbedtls_printf( " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n",
    //                     ret );
    //     // goto exit;
    // }

    // mbedtls_printf( "\n  . Reading public key from rsa_pub.txt" );

    fflush( stdout );

    if( ( f = fopen( "./rsa_keys/rsa_pub.txt", "rb" ) ) == NULL )
    {
        mbedtls_printf( " failed\n  ! Could not open rsa_pub.txt\n" \
                "  ! Please run rsa_genkey first\n\n" );
        // goto exit;
    }

    if( ( ret = mbedtls_mpi_read_file( &N, 16, f ) ) != 0 ||
        ( ret = mbedtls_mpi_read_file( &E, 16, f ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_mpi_read_file returned %d\n\n",
                        ret );
        fclose( f );
        // goto exit;
    }

    fclose( f );

    if( ( ret = mbedtls_rsa_import( &rsa, &N, NULL, NULL, NULL, &E ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_rsa_import returned %d\n\n", ret );
        // goto exit;
    }

    // if( strlen( argv[1] ) > 100 )
    // {
    //     mbedtls_printf( " Input data larger than 100 characters.\n\n" );
    //     goto exit;
    // }

    memcpy( input, key, 32 );

    /*
     * Calculate the RSA encryption of the hash.
     */

    // mbedtls_printf( "\n  . Generating the RSA encrypted value" );

    fflush( stdout );

    ret = mbedtls_rsa_pkcs1_encrypt( &rsa, mbedtls_ctr_drbg_random,
                                     &ctr_drbg, MBEDTLS_RSA_PUBLIC,
                                     32, input, buf );
    printf("\n");

    // printf("rsa_len -> %zu\n", rsa.len);

    // for( i = 0; i < rsa.len; i++ )
    // {
    //     printf("%02X", buf[i]);
    //     if (( i + 1 ) % 16 == 0 )
    //     	printf("\n");
    //     else
    //     	printf(" ");
    // }

    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_rsa_pkcs1_encrypt returned %d\n\n",
                        ret );
        // goto exit;
    }

    // for( i = 0; i < rsa.len; i++ )
    // 	result[i] = buf[i];

    // for( i = 0; i < rsa.len; i++ )
    // {
    //     printf("%02X", result[i]);
    //     if (( i + 1 ) % 16 == 0 )
    //     	printf("\n");
    //     else
    //     	printf(" ");
    // }


    /*
     * Write the signature into result-enc.txt
     */

    // if( ( f = fopen( "result-enc.txt", "wb+" ) ) == NULL )
    // {
    //     mbedtls_printf( " failed\n  ! Could not create %s\n\n", "result-enc.txt" );
    //     // goto exit;
    // }

    for( i = 0; i < rsa.len; i++ )
        output[i] = buf[i];

    // fclose( f );

    // mbedtls_printf( "\n  . Done (created \"%s\")\n\n", "result-enc.txt" );

    // exit_code = MBEDTLS_EXIT_SUCCESS;

    mbedtls_mpi_free( &N ); 
    mbedtls_mpi_free( &E );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );
    mbedtls_rsa_free( &rsa );
}
