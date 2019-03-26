#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <ev.h>
#include "ev.h"

#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <unistd.h>

#define DEFAULT_PORT 3333
#define BUF_SIZE 1024

#define INITIAL_PACKET_SIZE 256
#define DIGEST_SIZE 16
#define LEN_OF_ENCPYPTED_AES_KEY 256

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
#define mbedtls_fprintf fprintf
#define mbedtls_printf printf
#define mbedtls_exit exit
#define MBEDTLS_EXIT_SUCCESS EXIT_SUCCESS
#define MBEDTLS_EXIT_FAILURE EXIT_FAILURE
#endif /* MBEDTLS_PLATFORM_C */

/* generate aes key */
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

/* hash function headers */
#if defined(MBEDTLS_SHA1_C)
#include "mbedtls/sha1.h"
#endif

/* rsa header */
#include "mbedtls/rsa.h"

// void rsa_decrypt(unsigned char *input, unsigned char *output);

void 	rsa_decrypt(unsigned char *input, unsigned char *output);

#endif