/*
 *  RSA simple data encryption program
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

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

#if defined(MBEDTLS_BIGNUM_C) && defined(MBEDTLS_RSA_C) && \
    defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_FS_IO) && \
    defined(MBEDTLS_CTR_DRBG_C)
#include "mbedtls/rsa.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

#include <string.h>
#endif

#if !defined(MBEDTLS_BIGNUM_C) || !defined(MBEDTLS_RSA_C) ||  \
    !defined(MBEDTLS_ENTROPY_C) || !defined(MBEDTLS_FS_IO) || \
    !defined(MBEDTLS_CTR_DRBG_C)
int main( void )
{
    mbedtls_printf("MBEDTLS_BIGNUM_C and/or MBEDTLS_RSA_C and/or "
           "MBEDTLS_ENTROPY_C and/or MBEDTLS_FS_IO and/or "
           "MBEDTLS_CTR_DRBG_C not defined.\n");
    return( 0 );
}
#else

#if defined(MBEDTLS_CHECK_PARAMS)
#include "mbedtls/platform_util.h"
void mbedtls_param_failed( const char *failure_condition,
                           const char *file,
                           int line )
{
    mbedtls_printf( "%s:%i: Input param failed - %s\n",
                    file, line, failure_condition );
    mbedtls_exit( MBEDTLS_EXIT_FAILURE );
}
#endif

int main( int argc, char *argv[] )
{
    FILE *f;
    int ret = 1;
    int exit_code = MBEDTLS_EXIT_FAILURE;
    size_t i;
    mbedtls_rsa_context rsa;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    unsigned char input[1024];
    unsigned char buf[512];
    const char *pers = "rsa_encrypt";
    mbedtls_mpi N, E;

    if( argc != 2 )
    {
        mbedtls_printf( "usage: rsa_encrypt <string of max 100 characters>\n" );

#if defined(_WIN32)
        mbedtls_printf( "\n" );
#endif

        mbedtls_exit( exit_code );
    }

    mbedtls_printf( "\n  . Seeding the random number generator..." );
    fflush( stdout );

    mbedtls_mpi_init( &N ); mbedtls_mpi_init( &E );
    mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
    mbedtls_ctr_drbg_init( &ctr_drbg );
    mbedtls_entropy_init( &entropy );

    ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func,
                                 &entropy, (const unsigned char *) pers,
                                 strlen( pers ) );
    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n",
                        ret );
        goto exit;
    }

    mbedtls_printf( "\n  . Reading public key from rsa_pub.txt" );
    fflush( stdout );

    if( ( f = fopen( "rsa_pub.txt", "rb" ) ) == NULL )
    {
        mbedtls_printf( " failed\n  ! Could not open rsa_pub.txt\n" \
                "  ! Please run rsa_genkey first\n\n" );
        goto exit;
    }

    if( ( ret = mbedtls_mpi_read_file( &N, 16, f ) ) != 0 ||
        ( ret = mbedtls_mpi_read_file( &E, 16, f ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_mpi_read_file returned %d\n\n",
                        ret );
        fclose( f );
        goto exit;
    }
    fclose( f );

    if( ( ret = mbedtls_rsa_import( &rsa, &N, NULL, NULL, NULL, &E ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_rsa_import returned %d\n\n",
                        ret );
        goto exit;
    }

    if( strlen( argv[1] ) > 100 )
    {
        mbedtls_printf( " Input data larger than 100 characters.\n\n" );
        goto exit;
    }

    memcpy( input, argv[1], strlen( argv[1] ) );

    /*
     * Calculate the RSA encryption of the hash.
     */
    mbedtls_printf( "\n  . Generating the RSA encrypted value" );
    fflush( stdout );

    ret = mbedtls_rsa_pkcs1_encrypt( &rsa, mbedtls_ctr_drbg_random,
                                     &ctr_drbg, MBEDTLS_RSA_PUBLIC,
                                     strlen( argv[1] ), input, buf );
    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_rsa_pkcs1_encrypt returned %d\n\n",
                        ret );
        goto exit;
    }

    /*
     * Write the signature into result-enc.txt
     */
    if( ( f = fopen( "result-enc.txt", "wb+" ) ) == NULL )
    {
        mbedtls_printf( " failed\n  ! Could not create %s\n\n", "result-enc.txt" );
        goto exit;
    }

    for( i = 0; i < rsa.len; i++ )
        mbedtls_fprintf( f, "%02X%s", buf[i],
                 ( i + 1 ) % 16 == 0 ? "\r\n" : " " );

    fclose( f );

    mbedtls_printf( "\n  . Done (created \"%s\")\n\n", "result-enc.txt" );

    exit_code = MBEDTLS_EXIT_SUCCESS;

exit:
    mbedtls_mpi_free( &N ); mbedtls_mpi_free( &E );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );
    mbedtls_rsa_free( &rsa );

#if defined(_WIN32)
    mbedtls_printf( "  + Press Enter to exit this program.\n" );
    fflush( stdout ); getchar();
#endif

    return( exit_code );
}
#endif /* MBEDTLS_BIGNUM_C && MBEDTLS_RSA_C && MBEDTLS_ENTROPY_C &&
          MBEDTLS_FS_IO && MBEDTLS_CTR_DRBG_C */



35 61 8E 49 9F 00 C5 E6 BB 0D 71 3E 78 C4 35 CA
FC 4F BB F0 54 DA 3B F7 13 1D 74 68 64 5A E8 3F
6F 9A 7C E7 51 8F A1 A8 78 0E F7 A8 09 0D 30 7C
B4 A7 6F 92 75 84 FF 27 2E FF 55 68 9F C3 74 E8
2E F1 BF 0F B8 2A 66 63 09 13 F1 08 30 EC 65 20
2B 2A 24 22 B5 FA 5C 33 1B BD CB DD 5D 93 DA A8
02 46 BB CC A6 AB D8 BA A5 20 FB D5 4C 87 FA 9D
7C 1E 77 79 05 5D BF D9 99 81 FB 31 4E C0 F3 35
7C F2 11 EA 16 33 DB 16 F3 BD 15 5E C8 D0 D8 68
8D 53 58 EC F1 C4 88 1D 01 68 D1 DC 47 3E F9 40
C9 CD BC F2 F7 C8 6B 4B E4 AB E8 EA 03 A9 B3 F9
52 58 18 8F DD F3 BB 92 BD 9D 08 46 63 D1 21 80
87 40 19 E1 AF 42 D3 48 77 25 3D DB 20 59 06 1B
37 FB 8D B9 6F 95 59 C1 73 AD 17 13 F8 38 CD 9A
15 5B 86 55 8A FF 96 A4 B2 05 2D D8 2E 86 B5 2E
5D C9 F5 D4 63 36 0F 41 0F C3 8B AB E7 70 4E 5B