#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#include <stdio.h>
#define mbedtls_printf       printf
#define mbedtls_exit         exit
#define MBEDTLS_EXIT_SUCCESS EXIT_SUCCESS
#define MBEDTLS_EXIT_FAILURE EXIT_FAILURE
#endif

#if defined(MBEDTLS_MD5_C)
#include "mbedtls/md5.h"
#endif

#if defined(MBEDTLS_SHA1_C)
#include "mbedtls/sha1.h"
#endif

#if !defined(MBEDTLS_MD5_C)
int main( void )
{
    mbedtls_printf("MBEDTLS_MD5_C not defined.\n");
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

int main( void )
{
    int i, ret;
    unsigned char digest[16];
    char str[] = "Hello, world!";

    mbedtls_printf( "\n  SHA-1('%s') = ", str );

    if( ( ret = mbedtls_sha1_ret( (unsigned char *) str, 13, digest ) ) != 0 )
        return( MBEDTLS_EXIT_FAILURE );

    for( i = 0; i < 16; i++ )
        mbedtls_printf( "%02x", digest[i] );

    mbedtls_printf( "\n\n" );

#if defined(_WIN32)
    mbedtls_printf( "  Press Enter to exit this program.\n" );
    fflush( stdout ); getchar();
#endif

    return( MBEDTLS_EXIT_SUCCESS );
}
#endif /* MBEDTLS_MD5_C */