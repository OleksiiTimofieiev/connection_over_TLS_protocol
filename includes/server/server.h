#ifndef 			SERVER_H
#define 			SERVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <unistd.h>
#include <stdbool.h>

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
#define 			mbedtls_fprintf fprintf
#define 			mbedtls_printf printf
#define 			mbedtls_exit exit
#define 			MBEDTLS_EXIT_SUCCESS EXIT_SUCCESS
#define 			MBEDTLS_EXIT_FAILURE EXIT_FAILURE
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

#define 			PRIVATE_KEY_PATH 			"./rsa_keys/rsa_priv.txt"
#define 			INITIAL_PACKET_SIZE         256
#define 			DIGEST_SIZE                 16
#define 			LEN_OF_ENCPYPTED_AES_KEY    256

/* threads statuses defines */
#define 			INACTIVE 	0
#define 			RUNNING 	1
#define 			FINISHED 	2

/* threads structs */
typedef struct 		s_thread_data
{
	short			current_status;
	unsigned char 	data[INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY];
}					t_thread_data;

typedef struct 		s_thread
{
	t_thread_data	data;
	pthread_t		thread;

}					t_thread;

/* linked list data structure */
#pragma pack(1)
typedef struct 		s_data
{
	unsigned char 	data[INITIAL_PACKET_SIZE + DIGEST_SIZE];
	struct s_data 	*next;
}					t_data;

void 				rsa_decrypt(unsigned char *input, unsigned char *output);
void 				aes_decrypt(unsigned char *iv, unsigned char *key, unsigned char *input, unsigned char *output);
bool 				check_sha1_sum(unsigned char *checksum, unsigned char *decrypted_full_packet);
void 				push_front(t_data **head_ref, unsigned char *new_data);

void 				thread_create(t_thread *thread_pool, unsigned char *buffer, int threads_limit);
void 				*handle_new_data(void *data);

#endif